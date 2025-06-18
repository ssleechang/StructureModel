#include "StdAfx.h"
#include "MSFloor.h"

#include "GMLib/MSVersionInfo.h"
#include "MSBaseBuilding.h"
#include "MSRebarBuilding.h"
#include "MSConstructionZone.h"
#include "MSLineLevelDifG.h"
#include "MSLevelZone.h"
#include "MSSectionView.h"
#include "MSConnectivityDummy.h"
#include "MSGridLine.h"
#include "MSFloorQT.h"
#include "MSLineLevelDifG.h"
#include "MSBeamLevelDif.h"
#include "MSSlabLevelDif.h"
#include "MSPolylineObject.h"
#include "MSPitG.h"
#include "MSBlock.h"
#include "MSBlockInstance.h"
#include "MSRcSlabM.h"
#include "MSConcrete.h"
#include "ReinforceBarSetting.h"
#include "MSConnection.h"
#include "MSModelChecker.h"
#include "MSOpeningG.h"
#include "MSHandRailG.h"
#include "MSCustomM.h"
#include "MSCompoGUtil.h"
#include "MSPile.h"

#include "CmdProcess\CmdProcessHelper.h"

#include "CmdProcess\CmdModifyVector.h"	
#include "CmdProcess\CmdModifyDictionary.h"

#include "GMLib\AcisGeomHelper.h"
#include "GMLib\GM2DBound.h"

#include "RADFCore_CLI/StatusReporterWrapper.h"
#include "RADFCore_CLI/ProgressReporterWrapper.h"
#include "RADFCore_CLI/RadfContextWrapper.h"
#include "Radfcore_CLI/AppGlobalOptionWrapper.h"
#include "RADFCore_CLI/LoggerWrapper.h"

// MSSlabG* MSFloor::sm_NullSlab = NULL;

IMPLEMENT_SERIAL(MSFloor, MSContainer, VERSIONABLE_SCHEMA | 1)

bool MSFloor::ms_bCloneMode = true;
double MSFloor::ms_LineHitDistTol = DefHitDist;
double MSFloor::ms_DefLineHitDistTol = DefHitDist;
double MSFloor::ms_ColinearDegreeTol = 5.;
double MSFloor::ms_MaxInnerDegreeTol = 5.;
long MSFloor::ms_MaxPointOfASlab = 40;
long MSFloor::ms_TrimType = 0;
bool MSFloor::ms_DrawHitJoint = false;
bool MSFloor::ms_FixLineMemVec = false;
bool MSFloor::ms_SplitLinkMember = false;
double MSFloor::ms_MinLineLength = 10;
map<long, MSCompoG*> MSFloor::UpdateConnectionCompoGMap;

// CTypedPtrArray<CObArray, MSConnectivity*>* MSFloor::tempArr = NULL;

void MSFloor::SetSplitLinkMember(bool bSplitLinkMember)
{
	MSFloor::ms_SplitLinkMember = bSplitLinkMember;
}

bool MSFloor::GetSplitLinkMember()
{
	return MSFloor::ms_SplitLinkMember;
}

void MSFloor::SetLineHitDistTol(double dHitDist)
{
	MSFloor::ms_LineHitDistTol = dHitDist;
}

void MSFloor::SetFixLineMemVec(bool bFixLineMemVec)
{
	MSFloor::ms_FixLineMemVec = bFixLineMemVec;
	if (bFixLineMemVec == false)
		SetLineHitDistTol(MSFloor::ms_DefLineHitDistTol);
}

MSFloor::MSFloor(double dNodeTol, double dDegTol)
{
	Init(true);
	InitQT();
}

MSFloor::~MSFloor(void)
{

	try
	{

		CString msg;
		msg.Format(L"deleting floor Floor Num %d", m_nFloorNum);
		LoggerWrapper::Log(msg);

		DeleteLevelZones(); // QT에서 정보 찾음 LevelZone 먼저 삭제해야함

		DeleteCompoGs();

		if (mp_QT)
			delete mp_QT;

		DeleteCZoneMaps();
		DeleteRebars();
		//DeleteSlabRebarBox();
	}
	catch (const std::exception& ex)
	{
		CString errorMsg;
		errorMsg.Format(L"Standard exception occurred in MSFloor Destructor: %S", ex.what());
		StatusReporterWrapper::OutputError(errorMsg);
	}
#ifndef _DEBUG
	catch (...)
	{
		StatusReporterWrapper::OutputError(L"Unexpected exception occurred in Floor destructor!");
	}
#endif
}

void MSFloor::DeleteRebars()
{
	map<long, MSSlabRebar*>::iterator it;
	for (it = mm_Rebar.begin(); it != mm_Rebar.end(); ++it)
		delete it->second;
	mm_Rebar.clear();
}
MSObject* MSFloor::Copy(bool bNewID /*= true*/)
{
	MSFloor* pNewObj = new MSFloor();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSFloor::CopyFromMe(MSFloor* pSource, bool bNewID/*=true*/, bool isClone)
{
	if (pSource == NULL) return;
	m_SiteRect.CopyFrom(&(pSource->m_SiteRect));

	mp_Source = pSource;
	pSource->ma_Clone.push_back(this);

	m_SlabLevelOffset = pSource->m_SlabLevelOffset;

	MSLevelZone* pNewZone;
	long nZone = pSource->ma_LevelZone.size();
	for (long iZone = 0; iZone < nZone; iZone++)
	{
		MSLevelZone* pZone = pSource->ma_LevelZone[iZone];
		if (isClone && CheckOverlapLevelZone(pZone) != NULL)
			continue;
		pNewZone = (MSLevelZone*)pZone->Copy();
		pNewZone->mp_Source = pZone;
		pNewZone->mp_Owner = this;
		pZone->ma_Clone.push_back(pNewZone);
		Add(pNewZone);
	}

	MSCompoG* pNewMem;
	map<long, MSCompoG*>::iterator itMem;
	for (itMem = pSource->mm_CompoG.begin(); itMem != pSource->mm_CompoG.end(); itMem++)
	{
		MSCompoG* pMem = itMem->second;
		if (isClone && CheckCompoGOverlap_WithUpDownFloor(pMem, 0, true, false) != NULL)
			continue;

		pNewMem = (MSCompoG*)pMem->Copy();
		if (pNewMem->mp_CompoM->m_ID == 0)
		{
			pNewMem->mp_CompoM = (MSCompoM*)mp_Building->CreateNewCompoM(pMem->mp_CompoM->GetType());
			pNewMem->mp_CompoM->m_Name = _T("NONAME");
		}
		else
		{
			pNewMem->mp_CompoM = GetCompoM(pMem->mp_CompoM->m_Name, pMem->mp_CompoM->GetType());
			if (pNewMem->mp_CompoM == NULL)
			{
				pNewMem->mp_CompoM = (MSCompoM*)mp_Building->CreateNewCompoM(pMem->mp_CompoM->GetType());
				pNewMem->mp_CompoM->m_Name = _T("NONAME");
			}
		}

		pNewMem->mp_Source = pMem;
		pMem->ma_Clone.push_back(pNewMem);
		Add(pNewMem, true);
	}

	m_nRebarTemplateID = pSource->m_nRebarTemplateID;
	m_nFormWorkTemplateID = pSource->m_nFormWorkTemplateID;
}

void MSFloor::Init(bool bInConstructor/*=false*/)
{
	if (!bInConstructor)
		MSAssembly::Init(bInConstructor);

	m_nFloorNum = 0;
	m_dHeight = 0.0;
	m_nPlaceStory = 0;
	mp_QT = NULL;
	mp_Source = NULL;

	m_bIsSerializing = false;
	m_nCmdCnt = 0;
	m_isRunModifyObjectCompoG = false;
	//  	m_SiteRect.m_Origin.SetXY(-20000., -10000.);
	//  	m_SiteRect.m_Corner.SetXY(50000., 50000.);
	m_SlabLevelOffset = 0.0;
	m_nRebarTemplateID = -1;
	m_nFormWorkTemplateID = -1;
	m_ePlacementDirectionType = PlacementDirectionType_Base;
	mm_Pile.clear();

	m_dCashFloorLevel = INT_MAX;
	m_dCashSlabLevel = INT_MAX;

	SetSite();
}

bool MSFloor::CheckData()
{
	for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
	{
		if (itr->second->IsVerticalMember() == false) continue;
		if (itr->second->CheckHeight() == false) return false;
	}
	return true;
}

void MSFloor::SetSite(GM2DRectangle* SiteRect)
{
	if (!SiteRect)
	{
		MSBaseBuilding* pBuilding = GetBuilding();
		if (pBuilding)
		{
			m_SiteRect.m_Origin = pBuilding->m_SiteRect.m_Origin;
			m_SiteRect.m_Corner = pBuilding->m_SiteRect.m_Corner;
		}
		else
		{
			m_SiteRect.m_Origin.SetXY(-30000., -133000.);
			m_SiteRect.m_Corner.SetXY(243000., 68000.);
		}
	}
	else
	{
		m_SiteRect.m_Origin = SiteRect->m_Origin;
		m_SiteRect.m_Corner = SiteRect->m_Corner;
	}

	InitQT();
}

MSFloor* MSFloor::GetUpperFloor()
{
	if (mp_Building == NULL)
		return NULL;

	long UpperFloorNum = m_nFloorNum + 1;
	if (UpperFloorNum == 0)
		UpperFloorNum = 1;

	return GetBuilding()->GetFloor(UpperFloorNum);
}

MSFloor* MSFloor::GetLowerFloor()
{
	if (mp_Building == NULL)
		return NULL;

	long LowerFloorNum = m_nFloorNum - 1;
	if (LowerFloorNum == 0)
		LowerFloorNum = -1;

	return GetBuilding()->GetFloor(LowerFloorNum);
}

// MSCompoM* MSFloor::FindCompoM(CString& strName)
// {
// 	if (mp_Building == NULL)
// 		return NULL;
// 
// 	return GetBuilding()->FindCompoM(strName, m_nFloorNum);
// }

MSCompoM* MSFloor::GetCompoM(CString strName, long nType)
{
	if (mp_Building == NULL)
		return NULL;

	return GetBuilding()->GetCompoM(strName, nType, m_nFloorNum);
}

bool MSFloor::ExistCompoM(CString& strName)
{
	if (mp_Building == NULL)
		return false;

	return GetBuilding()->ExistCompoM(strName, m_nFloorNum);
}

MSGridLine* MSFloor::GetGLine(long GLineID)
{
	map<long, MSGridLine*>::iterator it = mm_GridLine.find(GLineID);
	if (it == mm_GridLine.end())
	{
		if (mp_Building)
			return mp_Building->GetGLine(GLineID);
		else
			return NULL;
	}

	return it->second;
}

MSGridLine* MSFloor::GetGLine(CString name)
{
	map<long, MSGridLine*>::iterator it;
	for (it = mm_GridLine.begin(); it != mm_GridLine.end(); it++)
	{
		MSGridLine* pGrid = it->second;
		if (pGrid->m_Name == name)
			return pGrid;
	}
	return NULL;
}

long MSFloor::GetCompoMsbyIncludeCompoG(long nMType, CMap<long, long, long, long>& MIDMap)
{
	map<long, MSCompoG*>::iterator itCompoG;
	for (itCompoG = mm_CompoG.begin(); itCompoG != mm_CompoG.end(); itCompoG++)
	{
		MSCompoG* pCompoG = itCompoG->second;
		if (pCompoG == NULL)	continue;
		MSCompoM* pCompoM = pCompoG->GetCompoM();
		if (pCompoM == NULL)	continue;
		if (pCompoM->GetType() == nMType)
			MIDMap.SetAt(pCompoM->m_ID, pCompoM->m_ID);
	}
	return MIDMap.GetCount();
}

long MSFloor::GetMSCompoGArr(MSCompoM::Type_MSCompoM eMType, vector<MSCompoG*>& MemberArr)
{
	map<long, MSCompoG*>::iterator itCompoG;
	for (itCompoG = mm_CompoG.begin(); itCompoG != mm_CompoG.end(); itCompoG++)
	{
		MSCompoG* pCompoG = itCompoG->second;
		if (pCompoG == NULL)
			continue;
		if (pCompoG->GetCompoM()->GetType() != eMType)
			continue;
		MemberArr.push_back(pCompoG);
	}
	return MemberArr.size();
}

long MSFloor::GetMSCompoGArr(long memberType, vector<MSCompoG*>& MemberArr)
{
	map<long, MSCompoG*>::iterator itCompoG;
	for (itCompoG = mm_CompoG.begin(); itCompoG != mm_CompoG.end(); itCompoG++)
	{
		MSCompoG* pCompoG = itCompoG->second;
		if (pCompoG == NULL)
			continue;
		if (pCompoG->GetType() != memberType)
			continue;
		MemberArr.push_back(pCompoG);
	}
	return MemberArr.size();
}

long MSFloor::GetMSCompoGArr(long memberType, CTypedPtrArray<CObArray, MSCompoG*>& MemberArr)
{
	map<long, MSCompoG*>::iterator itCompoG;
	for (itCompoG = mm_CompoG.begin(); itCompoG != mm_CompoG.end(); itCompoG++)
	{
		MSCompoG* pCompoG = itCompoG->second;
		if (pCompoG == NULL)
			continue;
		if (pCompoG->GetType() != memberType)
			continue;
		MemberArr.Add(pCompoG);
	}
	return MemberArr.GetCount();
}

// -1 : 보-벽 조합으로 레벨 겹침.
// 0 : 레벨 불일치
// 1 : 한 부재 하단에 한 부재 상단이 접함
// 2 : 상호 어긋나게 교차
// 3 : my가 ur에 포함됨
// 4 : my가 ur를 포함함
// 5 : 완전 일치
long MSFloor::LevelOverlap(MSLinkMemberG* pMyLink, MSLinkMemberG* pUrLink)
{
	double MyTopLevel, MyBotLevel;
	double UrTopLevel, UrBotLevel;
	pMyLink->GetZLevels(MyTopLevel, MyBotLevel);
	pUrLink->GetZLevels(UrTopLevel, UrBotLevel);

	// 벽-보 false
	if ((pMyLink->IsLineSupportMember() && pUrLink->IsWallMember() ||
		(pMyLink->IsWallMember() && pUrLink->IsLineSupportMember())))
	{
		// 		if (MyTopLevel <= UrBotLevel + DTOL1)
		// 			return 0;
		// 		if (UrTopLevel <= MyBotLevel + DTOL1)
		// 			return 0;
		if (MyTopLevel - UrBotLevel < -DTOL1)
			return 0;
		if (UrTopLevel - MyBotLevel < -DTOL1)
			return 0;
		return -1;
	}

	if (fabs(MyTopLevel - UrTopLevel) < DTOL1 && fabs(MyBotLevel - UrBotLevel) < DTOL1)
		return 5;
	if (MyTopLevel >= UrTopLevel - DTOL1 && MyBotLevel <= UrBotLevel + DTOL1)
		return 4;
	if (MyTopLevel <= UrTopLevel + DTOL1 && MyBotLevel >= UrBotLevel - DTOL1)
		return 3;

	if (fabs(MyTopLevel - UrBotLevel) < DTOL1)
		return 1;
	if (fabs(MyBotLevel - UrTopLevel) < DTOL1)
		return 1;

	if (MyTopLevel <= UrBotLevel + DTOL1)
		return 0;
	if (UrTopLevel <= MyBotLevel + DTOL1)
		return 0;

	return 2;
}

void MSFloor::AddCrossMemberError(MSCompoG* pMyLink, MSCompoG* pUrMem, XErrorObject* pMsgObject, CString strGroup, CString& strMsg)
{
	XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();
	if (pMsgObject)
	{
		pMsgObject->SetGroupName(strGroup);
		CString newMsg;
		int nFloorNum1 = pMyLink->GetFloorNumForUser();
		if (GetOwner() == nullptr)	return;
		MSFloor* pFloor = this->GetOwner()->GetFloor(nFloorNum1);

		if (pFloor != nullptr)
			newMsg.Format(_T("%s : %s"), pFloor->m_Name, strMsg);

		pMsgObject->m_PopupMessage = newMsg; // mylee : Message Change
		pMsgObject->m_Message.Add(newMsg);
		pMsgObject->AddCompoGID(pMyLink->m_ID);
		pMsgObject->AddCompoGID(pUrMem->m_ID);
		pMsgObject->SetShowSelectCompoG(TRUE);
		pMessageContainer->AddErrorObject(pMsgObject);
	}
}

void MSFloor::AddCrossMemberErrorCustom(MSCompoG* pMyLink, MSCompoG* pUrMem, XErrorObject* pMsgObject, CString strGroup, CString& strMsg)
{
	// 있는 그대로 메시지 출력
	XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();
	if (pMsgObject)
	{
		pMsgObject->SetGroupName(strGroup);
		pMsgObject->m_Message.Add(strMsg);
		pMsgObject->AddCompoGID(pMyLink->m_ID);
		pMsgObject->AddCompoGID(pUrMem->m_ID);
		pMsgObject->SetShowSelectCompoG(TRUE);
		pMessageContainer->AddErrorObject(pMsgObject);
	}
}

vector<long> MSFloor::GetCompoGs(long nMID)
{
	vector<long> itID;
	map<long, MSCompoG*>::iterator itCompoG;
	for (itCompoG = mm_CompoG.begin(); itCompoG != mm_CompoG.end(); itCompoG++)
	{
		MSCompoG* pCompoG = itCompoG->second;
		if (pCompoG == NULL)	continue;
		MSCompoM* pCompoM = pCompoG->GetCompoM();
		if (pCompoM == NULL)	continue;
		if (pCompoM->m_ID == nMID)
			itID.push_back(pCompoG->m_ID);
	}
	return itID;
}

void MSFloor::CalcAffected_CompoGID_ByLevelZone(MSFloor* pFloor_in, MSLevelZone* pZone, vector<int>& vCompoGID)
{
	vCompoGID.push_back(pZone->GetID());

	// 같은층의 수직, 수평부재
	MSFloor* pFloor = pFloor_in;
	if (pFloor != NULL)
	{
		for (map<long, MSCompoG*>::iterator itr = pFloor->mm_CompoG.begin(); itr != pFloor->mm_CompoG.end(); ++itr)
		{
			MSCompoG* cur = itr->second;
			if (cur->GetType() == MSElement::msBeamG || cur->GetType() == MSElement::msWallG || cur->GetType() == MSElement::msColumnG ||
				cur->GetType() == MSElement::msIsoFootG || cur->GetType() == MSElement::msWallFootG ||
				cur->GetType() == MSElement::msHandRailG || cur->GetType() == MSElement::msStairG || cur->GetType() == MSElement::msPitG ||
				cur->GetType() == MSElement::msWallColumnG)
			{
				if (cur->ma_Vec.size() > 0 && pZone->Contains(cur->ma_Vec[0]->m_X, cur->ma_Vec[0]->m_Y, 0) < 1)
					vCompoGID.push_back(cur->m_ID);
			}
			else if (cur->GetType() == MSElement::msSlabG)
			{
				GM2DPolyline* poly = cur->GetProfile();
				if (poly != NULL && poly->ma_Point.GetSize() > 0 && pZone->Contains(poly->ma_Point[0]->m_X, poly->ma_Point[0]->m_Y, 0) < 1)
					vCompoGID.push_back(cur->m_ID);
			}
		}

	}

	// 상위층의 수직부재
	pFloor = pFloor->GetUpperFloor();
	if (pFloor != NULL)
	{
		for (map<long, MSCompoG*>::iterator itr = pFloor->mm_CompoG.begin(); itr != pFloor->mm_CompoG.end(); ++itr)
		{
			MSCompoG* cur = itr->second;
			if (cur->GetType() == MSElement::msBeamG || cur->GetType() == MSElement::msWallG || cur->GetType() == MSElement::msColumnG ||
				cur->GetType() == MSElement::msIsoFootG || cur->GetType() == MSElement::msSlabG || cur->GetType() == MSElement::msWallFootG ||
				cur->GetType() == MSElement::msHandRailG || cur->GetType() == MSElement::msStairG || cur->GetType() == MSElement::msPitG ||
				cur->GetType() == MSElement::msWallColumnG)
			{
				if (cur->ma_Vec.size() > 0 && pZone->Contains(cur->ma_Vec[0]->m_X, cur->ma_Vec[0]->m_Y, 0) < 1)
					vCompoGID.push_back(cur->m_ID);
			}
		}
	}
}

void MSFloor::Add(MSBlock* pBlock, GM2DVector& FirstPoint, GM2DVector& SecondPoint, bool bReverse)
{
	MSBlockInstance* pInstance = pBlock->MakeInstance(FirstPoint, SecondPoint, bReverse);
	pInstance->GetID();

	pInstance->mp_Block = pBlock;
	pair<map<long, MSBlockInstance*>::iterator, bool> bPair;
	bPair = mm_BlockInstance.insert(make_pair(pInstance->m_ID, pInstance));

	long nCompoG = pInstance->ma_CompoG.size();
	for (long iCompoG = 0; iCompoG < nCompoG; iCompoG++)
		Add(pInstance->ma_CompoG[iCompoG]);

	long nLevelZone = pInstance->ma_LevelZone.size();
	for (long iZone = 0; iZone < nLevelZone; iZone++)
		Add(pInstance->ma_LevelZone[iZone]);

	long nSlabRebar = pInstance->ma_SlabRebar.size();
	for (long iRebar = 0; iRebar < nSlabRebar; iRebar++)
		Add(pInstance->ma_SlabRebar[iRebar]);
}

bool MSFloor::Remove(MSBlockInstance* pInstance)
{
	return true;
}

bool MSFloor::Delete(MSBlockInstance* pInstance)
{
	return true;
}

void MSFloor::FindOverlapMembersNCalcTrimOption(MSCompoG* pCompoG, bool bSecondTime)
{
	FindOverlapMembers(pCompoG, bSecondTime);
	if (bSecondTime == false)
	{
		pCompoG->CalculateConnectionsAngle();
		pCompoG->CalculateLinkMemberTrimOption();
	}
}

void MSFloor::FindOverlapMembers(MSCompoG* pMyCompoG, bool bSecondTime)
{
	if (pMyCompoG->GetFloor() != this)
		return;

	// Floor에 없는 부재는 Connectino을 생성하지 않는다.
	map<long, MSCompoG*>::iterator it = mm_CompoG.find(pMyCompoG->m_ID);
	if (it == mm_CompoG.end())
		return;

	//	pMyCompoG->ma_Connections 초기화 필요
	GM2DPolyline MyPoly(TRUE);
	if (pMyCompoG->IsPolyMember())
		MyPoly.CopyFrom(pMyCompoG->GetProfile());
	else
	{
		pMyCompoG->GetBoundary(MyPoly);
		if (bSecondTime)
			MyPoly.CopyFrom(pMyCompoG->GetTrimmedBoundary());
	}

	CTypedPtrArray<CObArray, MSFloorQT*> QT;
	mp_QT->FindCrossQTs(pMyCompoG, true);
	QT.Copy(pMyCompoG->ma_CrossQT);

	vector<MSFloor*> OverLapFloorArr = pMyCompoG->GetOverlapFloor();
	for (int iFloor = 0; iFloor < OverLapFloorArr.size(); iFloor++)
	{
		if (OverLapFloorArr[iFloor] == this)
			continue;
		OverLapFloorArr[iFloor]->mp_QT->FindCrossQTs(pMyCompoG, QT, false);
	}

	vector<MSCompoG*> OverLapCompoGArr;

	long nQT = QT.GetSize();
	CMap<long, long, MSCompoG*, MSCompoG*> CheckedMemberMap;  // 하나의 CompoG가 여러 QT에 있을 경우 pCompoG와 한번만 검토되도록 하기 위하여 검토된 pUrCompoG 등록

	for (long iQT = 0; iQT < nQT; iQT++)
	{
		MSFloorQT* pCurQT = QT[iQT];
		long nMem = pCurQT->ma_FloorMem.GetSize();
		for (long iMem = 0; iMem < pCurQT->ma_FloorMem.GetSize(); iMem++)
		{
			MSCompoG* pUrCompoG = pCurQT->ma_FloorMem[iMem];
			if (pUrCompoG == pMyCompoG)
				continue;

			// 			if (pMyCompoG->IsPolyMember() && pUrCompoG->IsPolyMember())
			// 				continue;

			MSCompoG* pTempMem;
			if (CheckedMemberMap.Lookup(pUrCompoG->m_ID, pTempMem))
				continue;
			CheckedMemberMap.SetAt(pUrCompoG->m_ID, pUrCompoG);

			if (pMyCompoG->IsPolyMember() || pUrCompoG->IsPolyMember())
			{
				if (pMyCompoG->IsLevelOverlap(pUrCompoG, true) == false)
					continue;
			}
			else
			{
				CheckModelOpt CheckOpt;
				if (pMyCompoG->IsLevelConnectedOverLap(pUrCompoG, CheckOpt, true) == false)
					continue;
				if (pMyCompoG->GetFloor() != pUrCompoG->GetFloor() && CheckOpt.IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_ZLEVELOVERLAP) == false)
					continue;
			}

			GM2DPolyline UrPoly(TRUE);
			if (pUrCompoG->IsPolyMember())
				UrPoly.CopyFrom(pUrCompoG->GetProfile());
			else
			{
				pUrCompoG->GetBoundary(UrPoly);
				if (bSecondTime)
					UrPoly.CopyFrom(pUrCompoG->GetTrimmedBoundary());
			}

			// 			if (pMyCompoG->IsPolyMember() || pUrCompoG->IsPolyMember())
			// 			{
			// 				MSCompoG* pBaseCompoG = nullptr;
			// 				MSCompoG* pCheckCompoG = nullptr;
			// 				if (pMyCompoG->IsPolyMember())
			// 				{
			// 					pBaseCompoG = pMyCompoG;
			// 					pCheckCompoG = pUrCompoG;
			// 				}
			// 				else
			// 				{
			// 					pBaseCompoG = pUrCompoG;
			// 					pCheckCompoG = pMyCompoG;
			// 				}
			// 
			// 				bool isContains = false;
			// 				GM2DPolyline* pPartnerPoly = pCheckCompoG->GetTrimmedBoundary();
			// 
			// 				if (pBaseCompoG->mp_PolylineInfo != nullptr)
			// 				{
			// 					if (pBaseCompoG->mp_PolylineInfo->Contains(pPartnerPoly, true, DTOL_GM))
			// 						isContains = true;
			// 				}
			// 				else
			// 				{
			// 					GM2DPolyline* pPoly = pBaseCompoG->GetTrimmedBoundary();
			// 					if (pPoly->Contains(pPartnerPoly))
			// 						isContains = true;
			// 				}
			// 				if(isContains)
			// 					OverLapComopGArr.push_back(pUrCompoG);
			// 			}
			// 			else
			{
				BOOL bOverlaped = MyPoly.OverlapEx(UrPoly, MSFloor::ms_DefLineHitDistTol);
				if (bOverlaped)
					OverLapCompoGArr.push_back(pUrCompoG);
			}
		}
		nMem = pCurQT->ma_VerticalMem.GetSize();
		for (long iMem = 0; iMem < pCurQT->ma_VerticalMem.GetSize(); iMem++)
		{
			MSCompoG* pUrCompoG = pCurQT->ma_VerticalMem[iMem];
			if (pUrCompoG == pMyCompoG)
				continue;

			//if (pMyCompoG->IsPolyMember() && pUrCompoG->IsPolyMember())
				//continue;

			MSCompoG* pTempMem;
			if (CheckedMemberMap.Lookup(pUrCompoG->m_ID, pTempMem))
				continue;
			CheckedMemberMap.SetAt(pUrCompoG->m_ID, pUrCompoG);

			if (pMyCompoG->IsPolyMember() || pUrCompoG->IsPolyMember())
			{
				if (pMyCompoG->IsLevelOverlap(pUrCompoG) == false)
					continue;
			}
			else
			{
				CheckModelOpt CheckOpt;
				if (pMyCompoG->IsLevelConnectedOverLap(pUrCompoG, CheckOpt, true) == false)
					continue;
				if (pMyCompoG->GetFloor() != pUrCompoG->GetFloor() && CheckOpt.IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_ZLEVELOVERLAP) == false)
					continue;
			}

			GM2DPolyline UrPoly(TRUE);
			if (pUrCompoG->IsPolyMember())
				UrPoly.CopyFrom(pUrCompoG->GetProfile());
			else
			{
				pUrCompoG->GetBoundary(UrPoly);
				if (bSecondTime)
					UrPoly.CopyFrom(pUrCompoG->GetTrimmedBoundary());
			}

			// 			if (pMyCompoG->IsPolyMember() || pUrCompoG->IsPolyMember())
			// 			{
			// 				MSCompoG* pBaseCompoG = nullptr;
			// 				MSCompoG* pCheckCompoG = nullptr;
			// 				if (pMyCompoG->IsPolyMember())
			// 				{
			// 					pBaseCompoG = pMyCompoG;
			// 					pCheckCompoG = pUrCompoG;
			// 				}
			// 				else
			// 				{
			// 					pBaseCompoG = pUrCompoG;
			// 					pCheckCompoG = pMyCompoG;
			// 				}
			// 
			// 				bool isContains = false;
			// 				GM2DPolyline* pPartnerPoly = pCheckCompoG->GetTrimmedBoundary();
			// 
			// 				if (pBaseCompoG->mp_PolylineInfo != nullptr)
			// 				{
			// 					if (pBaseCompoG->mp_PolylineInfo->Contains(pPartnerPoly, true, DTOL_GM))
			// 						isContains = true;
			// 				}
			// 				else
			// 				{
			// 					GM2DPolyline* pPoly = pBaseCompoG->GetTrimmedBoundary();
			// 					if (pPoly->Contains(pPartnerPoly))
			// 						isContains = true;
			// 				}
			// 				if (isContains)
			// 					OverLapComopGArr.push_back(pUrCompoG);
			// 			}
			// 			else
			{
				bool bIncludeOnLine = true;
				if (pMyCompoG->GetType() == Type_MSCompoG::msColumnG && pUrCompoG->GetType() == Type_MSCompoG::msColumnG)
					bIncludeOnLine = false;

				BOOL bOverlaped = MyPoly.OverlapEx(UrPoly, MSFloor::ms_DefLineHitDistTol, bIncludeOnLine);
				if (bOverlaped)
					OverLapCompoGArr.push_back(pUrCompoG);
			}
		}
	}
	// 입력 순서대로 정렬
	sort(OverLapCompoGArr.begin(), OverLapCompoGArr.end(),
		[](const MSCompoG* a, const MSCompoG* b)->bool {if (a->m_nCmdCnt == b->m_nCmdCnt) return a->m_ID < b->m_ID; else return a->m_nCmdCnt < b->m_nCmdCnt; }
	);

	vector<MSCompoG*>::iterator new_end;
	new_end = unique(OverLapCompoGArr.begin(), OverLapCompoGArr.end());
	OverLapCompoGArr.erase(new_end, OverLapCompoGArr.end());

	if (bSecondTime)
	{
		for (long i = 0; i < OverLapCompoGArr.size(); i++)
			pMyCompoG->AddConnectionWithMember_NotExistOnly(OverLapCompoGArr[i]);
	}
	else
	{
		for (long i = 0; i < OverLapCompoGArr.size(); i++)
			pMyCompoG->AddConnectionWithMember(OverLapCompoGArr[i]);

		// mylee - AdjustTrimOption
		for (int i = 0; i < pMyCompoG->ma_Connections.size(); ++i)
			pMyCompoG->ma_Connections[i]->AdjustTrimOptionAfter();
	}
}

void MSFloor::PreCheckAddbySpliteLinkMember(MSCompoG* pCompoG)
{
	pCompoG->GetID();
	MSElement::Type_MSCompoG eType = pCompoG->GetType();

	// 본 부재의 오너로 현재 층 세팅
	pCompoG->SetOwner(this);

	// 링크 부재의 경우 부재와 멤파트의 LLine, RLine 초기화
	if (pCompoG->IsLinkMember())
		((MSLinkMemberG*)pCompoG)->SetLines();  // jissi / 처음 들어오는 부재가 아닐 경우에 대한 검토 필요

	//쿼드 트리 검색
	vector<MSCompoG*> compoGArr;
	compoGArr.push_back(pCompoG);
	CheckFloorQT(compoGArr);

	pair<map<long, MSCompoG*>::iterator, bool> bPair;
	if (IS_USE_NATIVE_CMD)
		CmdModifyMap<long, MSCompoG*>::InsertToMap(&mm_CompoG, pCompoG->m_ID, pCompoG);
	else
		bPair = mm_CompoG.insert(make_pair(pCompoG->m_ID, pCompoG));

	mp_QT->AddToLeaves(pCompoG);

	FindOverlapMembernUpdateTrimBoundary(pCompoG, false);
	vector<MSCompoG*> PartnerCompoGArr = pCompoG->GetPartnerCompoGs();
	for (long iPartner = 0; iPartner < PartnerCompoGArr.size(); iPartner++)
		FindOverlapMembernUpdateTrimBoundary(PartnerCompoGArr[iPartner], true);

}

void MSFloor::PreCheckRemoveSpliteLinkMember(MSCompoG* pCompoG)
{
	long nMyQT = pCompoG->ma_CrossQT.GetSize();
	for (long iMyQT = 0; iMyQT < nMyQT; iMyQT++)
		pCompoG->ma_CrossQT[iMyQT]->Remove(pCompoG);
	pCompoG->ma_CrossQT.RemoveAll();

	long nID = pCompoG->m_ID;
	map<long, MSCompoG*>::iterator it = mm_CompoG.find(nID);
	if (it == mm_CompoG.end())	return;

	MSElement::Type_MSCompoG eType = pCompoG->GetType();

	mm_CompoG.erase(it);

	pCompoG->InitializeParameter();

	vector<MSCompoG*> PartnerCompoGArr = pCompoG->GetPartnerCompoGs();
	pCompoG->RemoveConnections();

	for (long iPartner = 0; iPartner < PartnerCompoGArr.size(); iPartner++)
	{
		for (int iConn = 0; iConn < pCompoG->ma_Connections.size(); iConn++)
			PartnerCompoGArr[iPartner]->RemoveConnection(pCompoG->ma_Connections[iConn]);

		FindOverlapMembernUpdateTrimBoundary(PartnerCompoGArr[iPartner], false);
	}

	for (long iPartner = 0; iPartner < PartnerCompoGArr.size(); iPartner++)
		PartnerCompoGArr[iPartner]->UpdateBoundaryVisibilities();

	return;
}

void MSFloor::Add(MSCompoG* pCompoG, bool bChangeOwner /*= false*/, bool isDoit /*= true*/, bool bUpdateTrimmedBoundary)
{
	CString strLog;
	strLog.Format(_T("MSFloor::Add(GID:%d)"), pCompoG->m_ID);
	StatusReporterWrapper::WriteMethodProcessTime(strLog, true);
	UpdateConnectionCompoGMap.clear();

	// 본 부재의 오너로 현재 층 세팅
	if (bChangeOwner)
		pCompoG->SetOwner(this);

	// 링크 부재의 경우 부재와 멤파트의 LLine, RLine 초기화
	if (pCompoG->IsLinkMember())
		((MSLinkMemberG*)pCompoG)->SetLines();  // jissi / 처음 들어오는 부재가 아닐 경우에 대한 검토 필요
	else if (pCompoG->IsPointMember())
		((MSPointMemberG*)pCompoG)->UpdateVecInputByCenVec();
	pCompoG->InitTrimmedBoundary();
	pCompoG->MakeCCW();

	pCompoG->DelDuplicatedPointOfProfile();// mylee

	pair<map<long, MSCompoG*>::iterator, bool> bPair;
	if (IS_USE_NATIVE_CMD)
		CmdModifyMap<long, MSCompoG*>::InsertToMap(&mm_CompoG, pCompoG->m_ID, pCompoG);
	else
		bPair = mm_CompoG.insert(make_pair(pCompoG->m_ID, pCompoG));

	//쿼드 트리 검색
	vector<MSCompoG*> compoGArr;
	compoGArr.push_back(pCompoG);
	CheckFloorQT(compoGArr);
	mp_QT->AddToLeaves(pCompoG);

	map<long, MSCompoG*>::iterator it;
	map<long, MSCompoG*> CompoGMap;
	MSElement::Type_MSCompoG eType = pCompoG->GetType();

	if ((eType == MSElement::msSlabG || eType == MSElement::msOpeningG))
	{
		if (pCompoG->GetTrimmedBoundary() != NULL)
			pCompoG->DeleteTrimmedBoundary();
		// 자동생성된 오프닝은 입력시 삭제한다.
		pCompoG->RemoveOpeningInfoAll();
	}

	if (eType == MSElement::msHandRailG)
	{
		MSBaseBuilding* pBldg = GetBuilding();
		MSHandRailG* pHandRailG = dynamic_cast<MSHandRailG*>(pCompoG);
		//BH-5983 루프층이 아닌 경우 난간벽의 m_nTopQuantityType의 기본값을 true로 설정한다
		int roofFloorNum = pBldg->GetNumAboveStory();
		if (GetFloorNum() != roofFloorNum)
			pHandRailG->SetTopQuantityType(1);
	}

	m_nCmdCnt++;
	pCompoG->m_nCmdCnt = m_nCmdCnt;

	// 바닥부재의 경우 레벨존에 따라 부재 단차 배열 생성
	if (pCompoG->IsLinkMember())//-->hana:Wall포함
	{
		MSLinkMemberG* link = dynamic_cast<MSLinkMemberG*>(pCompoG);
		if (!m_bIsSerializing)
			link->MoveToDeleted_MemberLevelDifG();
		InsertLevelDif2ALinkMemberByLevelZone((MSLinkMemberG*)pCompoG);
	}

	pCompoG->AddCompoGZLevelInFloor();

	if (pCompoG->GetType() == MSElement::msStairG)
	{
		MSGenStairG* pStair = (MSGenStairG*)pCompoG;
		pStair->CalculatePartHeight();

		vector<MSCompoG*> UpStairArr = pStair->FindConnectMember(FindType_MSElement::msStairG, FIND_UPFLOOR);
		vector<MSCompoG*> DnStairArr = pStair->FindConnectMember(FindType_MSElement::msStairG, FIND_DNFLOOR);
		for (int i = 0; i < UpStairArr.size(); i++)
			CalculateUpdatedCompoGArray(UpStairArr[i], CompoGIDList_NeedRegen_CurTR(), false);
		for (int i = 0; i < DnStairArr.size(); i++)
			CalculateUpdatedCompoGArray(DnStairArr[i], CompoGIDList_NeedRegen_CurTR(), false);
	}

	if (pCompoG->GetType() == MSElement::msOpeningG)
	{
		MSOpeningG* pOpeningG = (MSOpeningG*)pCompoG;
		vector<MSCompoG*> SlabArr = pOpeningG->GetPartnerCompoGs();
		for (long iSlab = 0; iSlab < SlabArr.size(); iSlab++)
		{
			MSSlabG* pSlabG = (MSSlabG*)SlabArr[iSlab];
			pSlabG->TrimSlabLineLevelDifbyOpening();
			//pSlabG->TrimLevelDifAll();
			pSlabG->UpdateSubMember();
		}
	}

	// 오버랩 된 슬래브를 찾아줌
	if (pCompoG->GetType() == MSElement::msPitG)
	{
		MSPitG* pCurPit = (MSPitG*)pCompoG;
		pCurPit->MakeConnectedSlabInfo();
	}

	vector<MSCompoG*> PartnerCompoGArr;
	if (bUpdateTrimmedBoundary)
	{
		pCompoG->m_DoUpdateTrimmedProfile = true;
		FindOverlapMembernUpdateTrimBoundary(pCompoG, false);
		PartnerCompoGArr = pCompoG->GetPartnerCompoGs();
		for (long iPartner = 0; iPartner < PartnerCompoGArr.size(); iPartner++)
		{
			MSCompoG* pPartnerG = PartnerCompoGArr[iPartner];
			pPartnerG->DeleteTrimmedBoundary_Acis();
			FindOverlapMembernUpdateTrimBoundary(pPartnerG, true);
		}
	}
	else
	{
		FindOverlapMembersNCalcTrimOption(pCompoG, false);
	}

	// 슬래브의 경우 레벨존에 따라 부재 단차 배열 생성
	if (pCompoG->GetType() == MSElement::msSlabG)
	{
		MSSlabG* pSlabG = (MSSlabG*)pCompoG;
		pSlabG->VerifyAndCureBoundaries(DTOL_GM);

		GM2DPolyline* pPoly = pSlabG->GetProfile();
		long NumLine = pPoly->GetNumLines();
		if (NumLine > 2)
		{
			// mylee - 슬래브 트림 로직이 다른 슬래브에 간섭을 일으키는 경우가 많아 막음. 외부에서 트림해서 들어오는 방법이 안정적이므로 변경 필요.
			if (!m_bIsSerializing)
			{
				pSlabG->mp_PolylineInfo->TrimExistPolyLineObject(true);
				pSlabG->mp_PolylineInfo->TrimExistPolyLineObject(false);
			}
			InsertLevelDif2SlabByLevelZone(pSlabG, isDoit);
		}
		pSlabG->UpdateSubMember();
		//pSlabG->TrimLevelDifAll();

		//오버랩 되는 Pit를 찾아줌
		vector<MSCompoG*> PitArr;
		GetMSCompoGArr(MSElement::msPitG, PitArr);
		for (long iMem = 0; iMem < PitArr.size(); iMem++)
		{
			MSPitG* pCurPit = (MSPitG*)PitArr[iMem];
			pCurPit->MakeConnectedSlabInfoStep2(pCompoG->mp_PolylineInfo);
		}
		pSlabG->ResetSlabLevelZone();
	}

	if (bUpdateTrimmedBoundary)
	{
		pCompoG->UpdateBoundaryVisibilities();
		for (long iPartner = 0; iPartner < PartnerCompoGArr.size(); iPartner++)
		{
			MSCompoG* pPartnerG = PartnerCompoGArr[iPartner];
			// 점부재는 다시 그리지 않는다.
			if (pPartnerG->IsPointMember())
				continue;

			pPartnerG->UpdateBoundaryVisibilities();
		}

		if (!m_bIsSerializing)
		{
			CheckMSModel(pCompoG);
			for (long iPartner = 0; iPartner < PartnerCompoGArr.size(); iPartner++)
				CheckMSModel(PartnerCompoGArr[iPartner]);
		}

		pCompoG->SetConnectionSlabGID();
		pCompoG->UpdateConnectionSlabInnerProFile();
	}

	// MSConnection::SwitchFirstSecond()에서 변경된 경우 처리
	for (it = UpdateConnectionCompoGMap.begin(); it != UpdateConnectionCompoGMap.end(); it++)
	{
		MSCompoG* pPartnerG = it->second;
		pPartnerG->UpdateBoundaryVisibilities();
		CalculateUpdatedCompoGArray(pPartnerG, CompoGIDList_NeedRegen_CurTR(), false);
	}

	CalculateUpdatedCompoGArray(pCompoG, CompoGIDList_NeedRegen_CurTR());

	if (eType == MSElement::msSlabG)
	{
		MSSlabG* pSlabG = (MSSlabG*)pCompoG;
		pSlabG->UpdatePartnerSlab();
	}

	AddLinkedCompoG(pCompoG);

	pCompoG->m_isCalculateUpdatedCompoGArray = true;

	UpdateConnectionCompoGMap.clear();
	StatusReporterWrapper::WriteMethodProcessTime(strLog, false);
}

void MSFloor::Add(vector<MSCompoG*> CompoGArr, bool bChangeOwner, bool isTrim)
{
	sort(CompoGArr.begin(), CompoGArr.end(),
		[](MSCompoG* a, MSCompoG* b)-> bool {return a->GetID() < b->GetID(); }
	);

	CString strLog;
	strLog.Format(_T("MSFloor::Add(CompoG Count: %d)"), CompoGArr.size());
	StatusReporterWrapper::WriteMethodProcessTime(strLog, true);
	UpdateConnectionCompoGMap.clear();

	map<long, MSCompoG*> CompoGMap;
	map<long, MSCompoG*> UpdateCompoGMap;
	map<long, MSCompoG*> UpdateSlabGMap;

	for (long iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
	{
		MSCompoG* pCompoG = CompoGArr[iCompoG];
		// 본 부재의 오너로 현재 층 세팅
		if (bChangeOwner)
			pCompoG->SetOwner(this);

		pair<map<long, MSCompoG*>::iterator, bool> bPair;
		bPair = mm_CompoG.insert(make_pair(pCompoG->m_ID, pCompoG));

		// 링크 부재의 경우 부재와 멤파트의 LLine, RLine 초기화
		if (pCompoG->IsLinkMember())
			((MSLinkMemberG*)pCompoG)->SetLines();
		else if (pCompoG->IsPointMember())
			((MSPointMemberG*)pCompoG)->UpdateVecInputByCenVec();
		pCompoG->InitTrimmedBoundary();
		pCompoG->MakeCCW();

		pCompoG->DelDuplicatedPointOfProfile();// mylee

		if (pCompoG->GetType() == MSElement::msHandRailG)
		{
			MSBaseBuilding* pBldg = GetBuilding();
			MSHandRailG* pHandRailG = dynamic_cast<MSHandRailG*>(pCompoG);
			if (pBldg != nullptr && pHandRailG != nullptr)
			{
				//BH-5983 루프층이 아닌 경우 난간벽의 m_nTopQuantityType의 기본값을 true로 설정한다
				int roofFloorNum = pBldg->GetNumAboveStory();
				if (GetFloorNum() != roofFloorNum)
					pHandRailG->SetTopQuantityType(1);
			}
		}
	}

	//쿼드 트리 검색
	CheckFloorQT(CompoGArr);

	for (long iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
	{
		MSCompoG* pCompoG = CompoGArr[iCompoG];
		CompoGMap.insert(make_pair(pCompoG->m_ID, pCompoG));

		m_nCmdCnt++;
		pCompoG->m_nCmdCnt = m_nCmdCnt;

		// 바닥부재의 경우 레벨존에 따라 부재 단차 배열 생성
		if (pCompoG->IsLinkMember())//-->hana:Wall포함
			InsertLevelDif2ALinkMemberByLevelZone((MSLinkMemberG*)pCompoG);

		pair<map<long, MSCompoG*>::iterator, bool> bPair;
		bPair = mm_CompoG.insert(make_pair(pCompoG->m_ID, pCompoG));
		mp_QT->AddToLeaves(pCompoG);

		if (pCompoG->GetType() == MSElement::msOpeningG)
		{
			MSOpeningG* pOpeningG = (MSOpeningG*)pCompoG;
			vector<MSCompoG*> SlabArr = pOpeningG->GetPartnerCompoGs();
			for (long iSlab = 0; iSlab < SlabArr.size(); iSlab++)
			{
				MSSlabG* pSlabG = (MSSlabG*)SlabArr[iSlab];
				pSlabG->TrimSlabLineLevelDifbyOpening();
				pSlabG->UpdateSubMember();
				//pSlabG->TrimLevelDifAll();
			}
		}

		// 오버랩 된 슬래브를 찾아줌
		if (pCompoG->GetType() == MSElement::msPitG)
		{
			MSPitG* pCurPit = (MSPitG*)pCompoG;
			pCurPit->MakeConnectedSlabInfo();
		}

		if (pCompoG->GetType() == MSElement::msStairG)
		{
			MSGenStairG* pStair = (MSGenStairG*)pCompoG;
			pStair->CalculatePartHeight();

			vector<MSCompoG*> UpStairArr = pStair->FindConnectMember(FindType_MSElement::msStairG, FIND_UPFLOOR);
			vector<MSCompoG*> DnStairArr = pStair->FindConnectMember(FindType_MSElement::msStairG, FIND_DNFLOOR);
			for (int i = 0; i < UpStairArr.size(); i++)
				CalculateUpdatedCompoGArray(UpStairArr[i], CompoGIDList_NeedRegen_CurTR(), false);
			for (int i = 0; i < DnStairArr.size(); i++)
				CalculateUpdatedCompoGArray(DnStairArr[i], CompoGIDList_NeedRegen_CurTR(), false);
		}

		pCompoG->AddCompoGZLevelInFloor();
	}

	if (isTrim == false)
		return;

	for (long iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
	{
		MSCompoG* pCompoG = CompoGArr[iCompoG];
		pCompoG->m_DoUpdateTrimmedProfile = true;

		FindOverlapMembersNCalcTrimOption(pCompoG, false);

		vector<MSCompoG*> PartnerCompoGArr = pCompoG->GetPartnerCompoGs();
		for (int i = 0; i < PartnerCompoGArr.size(); ++i)
			for (int k = 0; k < PartnerCompoGArr[i]->ma_Connections.size(); ++k)
				PartnerCompoGArr[i]->ma_Connections[k]->AdjustTrimOptionAfter(); // mylee 0126

		UpdateTrimmedBoundary(pCompoG);
	}

	for (long iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
	{
		MSCompoG* pCompoG = CompoGArr[iCompoG];

		FindOverlapMembersNCalcTrimOption(pCompoG, true);
		vector<MSCompoG*> PartnerCompoGArr = pCompoG->GetPartnerCompoGs();

		for (int i = 0; i < PartnerCompoGArr.size(); ++i)
		{
			MSCompoG* pPartnerCompoG = PartnerCompoGArr[i];
			if (pPartnerCompoG == nullptr)
				continue;

			FindOverlapMembersNCalcTrimOption(pPartnerCompoG, true);

			if (pPartnerCompoG->IsPointMember())
				continue;
			// 			if (pCompoG->IsFloorMember() && pPartnerCompoG->IsVerticalMember())
			// 				continue;

			if (CompoGMap.find(pPartnerCompoG->m_ID) != CompoGMap.end())
				continue;

			UpdateCompoGMap.insert(make_pair(pPartnerCompoG->m_ID, pPartnerCompoG));
		}

		pCompoG->RemoveSameConnection();

		pCompoG->SetConnectionSlabGID();
		map<long, MSFloor*> SlabIdMap = pCompoG->GetConnectionSlabGID();
		map<long, MSFloor*>::iterator it;
		for (it = SlabIdMap.begin(); it != SlabIdMap.end(); it++)
		{
			MSFloor* pFloor = it->second;
			MSCompoG* pPartnerCompoG = pFloor->GetCompoG(it->first);
			if (pPartnerCompoG == nullptr)
				continue;

			if (CompoGMap.find(pPartnerCompoG->m_ID) != CompoGMap.end())
				continue;

			UpdateSlabGMap.insert(make_pair(pPartnerCompoG->m_ID, pPartnerCompoG));
		}

		// 슬래브의 경우 레벨존에 따라 부재 단차 배열 생성
		if (pCompoG->GetType() == MSElement::msSlabG)
		{
			MSSlabG* pSlabG = (MSSlabG*)pCompoG;
			// 자동생성된 오프닝은 입력시 삭제한다.
			pCompoG->RemoveOpeningInfoAll();

			pSlabG->VerifyAndCureBoundaries(DTOL_GM);

			GM2DPolyline* pPoly = pSlabG->GetProfile();
			pPoly->CheckAndFixFoldedEdges(DTOL_GM);
			long NumLine = pPoly->GetNumLines();
			if (NumLine > 2)
			{
				// mylee - 슬래브 트림 로직이 다른 슬래브에 간섭을 일으키는 경우가 많아 막음. 외부에서 트림해서 들어오는 방법이 안정적이므로 변경 필요.
				if (!m_bIsSerializing)
				{
					pSlabG->mp_PolylineInfo->TrimExistPolyLineObject(true);
					pSlabG->mp_PolylineInfo->TrimExistPolyLineObject(false);
				}
				InsertLevelDif2SlabByLevelZone(pSlabG, true);
			}
			//pSlabG->TrimLevelDifAll();
			pSlabG->UpdateSubMember();

			//오버랩 되는 Pit를 찾아줌
			vector<MSCompoG*> PitArr;
			GetMSCompoGArr(MSElement::msPitG, PitArr);
			for (long iMem = 0; iMem < PitArr.size(); iMem++)
			{
				MSPitG* pCurPit = (MSPitG*)PitArr[iMem];
				pCurPit->MakeConnectedSlabInfoStep2(pCompoG->mp_PolylineInfo);
			}
			pSlabG->ResetSlabLevelZone();

			if (pCompoG->GetType() == MSElement::msSlabG)
			{
				vector<MSCompoG*> FindCompoGArr = pCompoG->FindConnectMember(pCompoG->GetFindType(), FIND_CURFLOOR);
				for (int iSlab = 0; iSlab < FindCompoGArr.size(); iSlab++)
				{
					MSCompoG* pSlab = FindCompoGArr[iSlab];
					if (CompoGMap.find(pSlab->m_ID) != CompoGMap.end())
						continue;
					if (UpdateSlabGMap.find(pSlab->m_ID) != UpdateSlabGMap.end())
						continue;

					UpdateSlabGMap.insert(make_pair(pSlab->m_ID, pSlab));
				}
			}
		}
	}

	map<long, MSCompoG*>::iterator it;
	for (it = UpdateCompoGMap.begin(); it != UpdateCompoGMap.end(); it++)
	{
		it->second->DeleteTrimmedBoundary_Acis();
		FindOverlapMembernUpdateTrimBoundary(it->second, true);
	}

	for (it = UpdateCompoGMap.begin(); it != UpdateCompoGMap.end(); it++)
	{
		it->second->UpdateBoundaryVisibilities();
		CheckMSModel(it->second);
	}

	for (long iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
	{
		MSCompoG* pCompoG = CompoGArr[iCompoG];
		pCompoG->UpdateBoundaryVisibilities();
		CalculateUpdatedCompoGArray(pCompoG, CompoGIDList_NeedRegen_CurTR());
		CheckMSModel(pCompoG);

		if (pCompoG->GetType() == MSElement::msSlabG)
		{
			// 성능 이슈로 변경함
			//pSlabG->UpdatePartnerSlab();

			MSSlabG* pSlabG = (MSSlabG*)pCompoG;
			vector<MSCompoG*> partnerGArr = pSlabG->GetPartnerCompoGs();
			for (int iPartnerIdx = 0; iPartnerIdx < partnerGArr.size(); iPartnerIdx++)
			{
				MSCompoG* partnerG = partnerGArr[iPartnerIdx];
				if (partnerG != nullptr || partnerG->GetType() != msSlabG || CompoGMap.find(partnerG->m_ID) != CompoGMap.end())
					continue;

				partnerG->mp_PolylineInfo->TrimExistPolyLineObject(true);
				partnerG->mp_PolylineInfo->TrimExistPolyLineObject(false);
				partnerG->UpdateBoundaryVisibilities();
				CalculateUpdatedCompoGArray(partnerG, CompoGIDList_NeedRegen_CurTR(), false);
			}
		}
	}

	for (it = UpdateSlabGMap.begin(); it != UpdateSlabGMap.end(); it++)
	{
		MSSlabG* pSlabG = (MSSlabG*)it->second;
		if (UpdateCompoGMap.find(pSlabG->m_ID) != UpdateCompoGMap.end())
			continue;
		if (CompoGMap.find(pSlabG->m_ID) != CompoGMap.end())
			continue;

		//pSlabG->UpdateInnerProfile();
		//pSlabG->mp_PolylineInfo->TrimExistPolyLineObject(true);
		//pSlabG->mp_PolylineInfo->TrimExistPolyLineObject(false);
		pSlabG->UpdateBoundaryVisibilities();
		CalculateUpdatedCompoGArray(pSlabG, CompoGIDList_NeedRegen_2DView_CurTR(), false);
	}

	// MSConnection::SwitchFirstSecond()에서 변경된 경우 처리
	for (it = UpdateConnectionCompoGMap.begin(); it != UpdateConnectionCompoGMap.end(); it++)
	{
		MSCompoG* pPartnerG = it->second;
		pPartnerG->UpdateBoundaryVisibilities();
		CalculateUpdatedCompoGArray(pPartnerG, CompoGIDList_NeedRegen_CurTR(), false);
	}

	for (long iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
		AddLinkedCompoG(CompoGArr[iCompoG]);

	for (long iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
	{
		MSCompoG* pCompoG = CompoGArr[iCompoG];
		pCompoG->m_isCalculateUpdatedCompoGArray = true;
	}

	UpdateConnectionCompoGMap.clear();
	StatusReporterWrapper::WriteMethodProcessTime(strLog, false);
}

void MSFloor::AddLinkedCompoG(MSCompoG* pCompoG)
{
	if (pCompoG == nullptr)
		return;

	if (pCompoG->GetType() == MSElement::msFWSoundAbsorptionG)
	{
		MSCompoG* pOwnerG = (MSCompoG*)pCompoG->GetMainCompoG();
		if (pOwnerG == nullptr)
			return;
		MSCompoG* pFindCompoG = GetCompoG(pOwnerG->m_ID);
		if (pFindCompoG == nullptr)
			return;

		map<long, vector<long>>::iterator it = mm_GIDToLinkedCompoGIDArr.find(pOwnerG->m_ID);
		if (it == mm_GIDToLinkedCompoGIDArr.end())
		{
			vector<long> LinkCompoGIDArr;
			LinkCompoGIDArr.push_back(pCompoG->m_ID);
			mm_GIDToLinkedCompoGIDArr.insert(make_pair(pOwnerG->m_ID, LinkCompoGIDArr));
		}
		else
			it->second.push_back(pCompoG->m_ID);
	}
}

void MSFloor::RemoveLinkedCompoG(MSCompoG* pCompoG)
{
	if (pCompoG == nullptr)
		return;

	if (pCompoG->GetType() == MSElement::msFWSoundAbsorptionG)
	{
		long nMainCompoGID = pCompoG->GetMainCompoGID();
		map<long, vector<long>>::iterator it = mm_GIDToLinkedCompoGIDArr.find(nMainCompoGID);
		if (it != mm_GIDToLinkedCompoGIDArr.end())
		{
			bool bFind = false;
			for (int idx = 0; idx < it->second.size(); idx++)
			{
				if (it->second[idx] == pCompoG->m_ID)
				{
					bFind = true;
					break;
				}
			}
			if (bFind == true)
				it->second.erase(it->second.begin() + idx);
		}
	}
}

bool MSFloor::Remove(vector<MSCompoG*> CompoGArr, bool isTrim)
{
	sort(CompoGArr.begin(), CompoGArr.end(),
		[](MSCompoG* a, MSCompoG* b)-> bool {return a->GetID() < b->GetID(); }
	);

	CString strLog;
	strLog.Format(_T("MSFloor::Remove(Count:%d)"), CompoGArr.size());
	StatusReporterWrapper::WriteMethodProcessTime(strLog, true);
	UpdateConnectionCompoGMap.clear();

	map<long, MSCompoG*> ModifyCompoGMap;
	map<long, MSCompoG*> UpdateCompoGMap;
	map<long, MSLevelZone*> LevelZoneMap;

	for (long iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
	{
		ProgressReporterWrapper::AddProgress(1);
		MSCompoG* pCompoG = CompoGArr[iCompoG];
		MSElement::Type_MSCompoG eType = pCompoG->GetType();
		vector<MSCompoG*> PartnerCompoGArr = pCompoG->GetPartnerCompoGs();
		if (eType == MSElement::msSlabG)
		{
			vector<MSCompoG*> FindCompoGArr = pCompoG->FindConnectMember(pCompoG->GetFindType(), FIND_CURFLOOR);
			for (int iSlab = 0; iSlab < FindCompoGArr.size(); iSlab++)
			{
				MSCompoG* pSlab = FindCompoGArr[iSlab];
				map<long, MSCompoG*>::iterator it = UpdateCompoGMap.find(pSlab->m_ID);
				if (it == UpdateCompoGMap.end())
					UpdateCompoGMap.insert(make_pair(pSlab->m_ID, pSlab));
			}
		}

		long nMyQT = pCompoG->ma_CrossQT.GetSize();
		for (long iMyQT = 0; iMyQT < nMyQT; iMyQT++)
			pCompoG->ma_CrossQT[iMyQT]->Remove(pCompoG);
		pCompoG->ma_CrossQT.RemoveAll();

		long nID = pCompoG->m_ID;
		map<long, MSCompoG*>::iterator it = mm_CompoG.find(nID);
		if (it == mm_CompoG.end())	continue;

		if (pCompoG->GetType() == MSElement::msStairG)
		{
			MSGenStairG* pStair = (MSGenStairG*)pCompoG;

			vector<MSCompoG*> UpStairArr = pStair->FindConnectMember(FindType_MSElement::msStairG, FIND_UPFLOOR);
			vector<MSCompoG*> DnStairArr = pStair->FindConnectMember(FindType_MSElement::msStairG, FIND_DNFLOOR);
			for (int i = 0; i < UpStairArr.size(); i++)
				CalculateUpdatedCompoGArray(UpStairArr[i], CompoGIDList_NeedRegen_CurTR(), false);
			for (int i = 0; i < DnStairArr.size(); i++)
				CalculateUpdatedCompoGArray(DnStairArr[i], CompoGIDList_NeedRegen_CurTR(), false);
		}
		mm_CompoG.erase(it);

		if (eType == MSElement::msSlabG)
		{
			MSSlabG* pSlabG = (MSSlabG*)pCompoG;
			long NumLevelPoly = pSlabG->ma_LevelPoly.size();
			for (long iPoly = 0; iPoly < NumLevelPoly; iPoly++)
			{
				MSSlabLevelZone *pZone = pSlabG->ma_LevelPoly[iPoly];
				if (pZone->m_IsDelete == true) continue;

				if (pZone->mp_LevelZone != NULL)
					LevelZoneMap.insert(make_pair(pZone->m_LevelZoneID, pZone->mp_LevelZone));

				pSlabG->RemoveLevelZone(pZone->mp_LevelZone);
			}
		}

		if (pCompoG->GetType() == MSElement::msOpeningG)
		{
			MSOpeningG* pOpeningG = (MSOpeningG*)pCompoG;
			vector<MSCompoG*> SlabArr = pOpeningG->GetPartnerCompoGs();
			for (long iSlab = 0; iSlab < SlabArr.size(); iSlab++)
				ModifyCompoGMap.insert(make_pair(SlabArr[iSlab]->m_ID, SlabArr[iSlab]));
		}

		CalculateUpdatedCompoGArray(pCompoG, CompoGIDList_NeedRegen_CurTR());

		pCompoG->InitializeParameter();

		for (long iPartner = 0; iPartner < PartnerCompoGArr.size(); iPartner++)
		{
			MSCompoG* pPartnerG = PartnerCompoGArr[iPartner];
			for (int iConn = 0; iConn < pCompoG->ma_Connections.size(); iConn++)
				pPartnerG->RemoveConnection(pCompoG->ma_Connections[iConn]);
			long nMyQT = pPartnerG->ma_CrossQT.GetSize();
			for (long iMyQT = 0; iMyQT < nMyQT; iMyQT++)
				pPartnerG->ma_CrossQT[iMyQT]->Remove(pCompoG);

			if (pPartnerG->IsPointMember())
				continue;

			// 			if (pCompoG->IsFloorMember() && pPartnerG->IsVerticalMember())
			// 				continue;

			UpdateConnectionCompoGMap[pPartnerG->m_ID] = pPartnerG;
		}

		pCompoG->RemoveConnections();

		CheckMSModel(pCompoG);
		for (long iPartner = 0; iPartner < PartnerCompoGArr.size(); iPartner++)
			CheckMSModel(PartnerCompoGArr[iPartner]);

		pCompoG->DelCompoGZLevelInFloor();

		pCompoG->SetConnectionSlabGID();
		map<long, MSFloor*> SlabIdMap = pCompoG->GetConnectionSlabGID();
		map<long, MSFloor*>::iterator itMap;
		for (itMap = SlabIdMap.begin(); itMap != SlabIdMap.end(); itMap++)
		{
			MSFloor* pFloor = itMap->second;
			MSCompoG* pSlab = pFloor->GetCompoG(itMap->first);
			if (pSlab != NULL)
				UpdateCompoGMap.insert(make_pair(pSlab->m_ID, pSlab));
		}
	}

	if (isTrim == false)
		return true;


	map<long, MSLevelZone*>::iterator LevelZoneit;
	for (LevelZoneit = LevelZoneMap.begin(); LevelZoneit != LevelZoneMap.end(); LevelZoneit++)
	{
		MSLevelZone* pLevelZone = LevelZoneit->second;
		if (pLevelZone == NULL) continue;

		//pLevelZone->DeleteLineLevelDifAll(); // Modify에서 처리함
		ModifyLevelZoneByElement(pLevelZone);
	}

	map<long, MSCompoG*>::iterator it;

	for (long iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
	{
		MSCompoG* pCompoG = CompoGArr[iCompoG];

		it = ModifyCompoGMap.find(pCompoG->m_ID);
		if (it != ModifyCompoGMap.end())
			ModifyCompoGMap.erase(it);

		it = UpdateConnectionCompoGMap.find(pCompoG->m_ID);
		if (it != UpdateConnectionCompoGMap.end())
			UpdateConnectionCompoGMap.erase(it);

		it = UpdateCompoGMap.find(pCompoG->m_ID);
		if (it != UpdateCompoGMap.end())
			UpdateCompoGMap.erase(it);
	}

	for (it = ModifyCompoGMap.begin(); it != ModifyCompoGMap.end(); it++)
	{
		MSCompoG* pSlab = it->second;
		map<long, MSCompoG*>::iterator Slabit = mm_CompoG.find(pSlab->m_ID);
		if (Slabit != mm_CompoG.end())
			Modify(pSlab, false);
	}

	for (it = UpdateConnectionCompoGMap.begin(); it != UpdateConnectionCompoGMap.end(); it++)
	{
		MSCompoG* pCompoG = it->second;
		FindOverlapMembernUpdateTrimBoundary(it->second, false);
	}

	for (it = UpdateConnectionCompoGMap.begin(); it != UpdateConnectionCompoGMap.end(); it++)
	{
		MSCompoG* pPartnerG = it->second;
		pPartnerG->UpdateBoundaryVisibilities();
		CalculateUpdatedCompoGArray(pPartnerG, CompoGIDList_NeedRegen_CurTR(), false);
	}

	for (it = UpdateCompoGMap.begin(); it != UpdateCompoGMap.end(); it++)
	{
		MSCompoG* pPartnerG = it->second;
		if (pPartnerG->GetType() == MSElement::msSlabG)
		{
			pPartnerG->mp_PolylineInfo->TrimExistPolyLineObject(true);
			pPartnerG->mp_PolylineInfo->TrimExistPolyLineObject(false);
		}
	}

	// 한번으로 안되는 경우가 발생...
	for (it = UpdateCompoGMap.begin(); it != UpdateCompoGMap.end(); it++)
	{
		MSCompoG* pPartnerG = it->second;
		if (pPartnerG->GetType() == MSElement::msSlabG)
		{
			pPartnerG->mp_PolylineInfo->TrimExistPolyLineObject(true);
			pPartnerG->mp_PolylineInfo->TrimExistPolyLineObject(false);
		}
	}

	for (it = UpdateCompoGMap.begin(); it != UpdateCompoGMap.end(); it++)
	{
		MSCompoG* pPartnerG = it->second;
		pPartnerG->UpdateBoundaryVisibilities();
		if (pPartnerG->GetType() == MSElement::msSlabG)
			CalculateUpdatedCompoGArray(pPartnerG, CompoGIDList_NeedRegen_2DView_CurTR(), false);
	}

	for (long iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
	{
		MSCompoG* pCompoG = CompoGArr[iCompoG];
		vector<MSCompoG*> LinkedCompoGArr = pCompoG->GetLinkedCompoGArr();
		RemoveLinkedCompoG(CompoGArr[iCompoG]);
	}

	for (long iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
	{
		MSCompoG* pCompoG = CompoGArr[iCompoG];
		pCompoG->m_isCalculateUpdatedCompoGArray = true;
	}

	UpdateConnectionCompoGMap.clear();
	StatusReporterWrapper::WriteMethodProcessTime(strLog, false);
	return true;
}
// Test Code
// void MSFloor::Add(vector<MSCompoG*> CompoGArr, bool bChangeOwner)
// {
// 	map<long, MSCompoG*> UpdateCompoGMap;
// 
// 	for(long iCompoG = 0 ; iCompoG < CompoGArr.size() ; iCompoG++)
// 	{
// 		MSCompoG* pCompoG = CompoGArr[iCompoG];
// 
// 		m_nCmdCnt++;
// 		pCompoG->m_nCmdCnt = m_nCmdCnt;
// 
// 		// 본 부재의 오너로 현재 층 세팅
// 		if (bChangeOwner)
// 			pCompoG->SetOwner(this);
// 		
// 		// 링크 부재의 경우 부재와 멤파트의 LLine, RLine 초기화
// 		if (pCompoG->IsLinkMember())
// 			((MSLinkMemberG*)pCompoG)->SetLines();
// 
// 		pCompoG->DelDuplicatedPointOfProfile();// mylee
// 
// 		//쿼드 트리 검색
// 		CheckFloorQT(pCompoG);
// 
// 		// 바닥부재의 경우 레벨존에 따라 부재 단차 배열 생성
// 		if(pCompoG->IsLinkMember())//-->hana:Wall포함
// 			InsertLevelDif2ALinkMemberByLevelZone((MSLinkMemberG*)pCompoG);
// 
// 		pair<map<long, MSCompoG*>::iterator, bool> bPair;
// 		bPair = mm_CompoG.insert(make_pair(pCompoG->m_ID, pCompoG));
// 		mp_QT->AddToLeaves(pCompoG);
// 
// 
// 		// 슬래브의 경우 레벨존에 따라 부재 단차 배열 생성
// 		if(pCompoG->GetType()==MSElement::msSlabG)
// 		{
// 			MSSlabG* pSlabG = (MSSlabG*) pCompoG;
// 
// 			pSlabG->VerifyAndCureBoundaries(DTOL_GM);
// 
// 			GM2DPolyline* pPoly = pSlabG->GetProfile();		
// 			long NumLine = pPoly->GetNumLines();
// 			if(NumLine > 2)
// 			{
// 				pPoly->MakeCCW();
// 				// mylee - 슬래브 트림 로직이 다른 슬래브에 간섭을 일으키는 경우가 많아 막음. 외부에서 트림해서 들어오는 방법이 안정적이므로 변경 필요.
// 				if(!m_bIsSerializing)
// 					pSlabG->mp_PolylineInfo->TrimExistPolyLineObject();
// 				InsertLevelDif2SlabByLevelZone(pSlabG, true);
// 			}
// 			pSlabG->TrimLevelDifAll();
// 
// 			//오버랩 되는 Pit를 찾아줌
// 			vector<MSCompoG*> PitArr;
// 			GetMSCompoGArr(MSElement::msPitG, PitArr);
// 			for(long iMem = 0 ; iMem < PitArr.size() ; iMem++)
// 			{
// 				MSPitG* pCurPit = (MSPitG*)PitArr[iMem];
// 				pCurPit->MakeConnectedSlabInfoStep2(pCompoG->mp_PolylineInfo);
// 			}
// 		}
// 
// 		if(pCompoG->GetType() == MSElement::msOpeningG)
// 		{
// 			MSOpeningG* pOpeningG = (MSOpeningG*)pCompoG;
// 			pOpeningG->GetProfile()->MakeCCW();
// 			vector<MSCompoG*> SlabArr = pOpeningG->GetPartnerCompoGs();
// 			for(long iSlab = 0 ; iSlab < SlabArr.size() ; iSlab++)
// 			{
// 				MSSlabG* pSlabG = (MSSlabG*)SlabArr[iSlab];
// 				pSlabG->TrimSlabLineLevelDifbyOpening();
// 				pSlabG->TrimLevelDifAll();
// 			}
// 		}
// 
// 		// 오버랩 된 슬래브를 찾아줌
// 		if(pCompoG->GetType()==MSElement::msPitG)
// 		{
// 			MSPitG* pCurPit = (MSPitG*)pCompoG;
// 			pCurPit->MakeConnectedSlabInfo();
// 		}
// 
// 		if (!m_bIsSerializing)
// 			CheckMSModel();
// 
// 		pCompoG->AddCompoGZLevelInFloor();
// 	}
// 
// 	for(long iCompoG = 0 ; iCompoG < CompoGArr.size() ; iCompoG++)
// 	{
// 		MSCompoG* pCompoG = CompoGArr[iCompoG];
// 
// 		FindOverlapMembers(pCompoG);
// 		UpdateCompoGMap.insert(make_pair(pCompoG->m_ID, pCompoG));
// 
// 		vector<MSCompoG*> PartnerCompoGArr = pCompoG->GetPartnerCompoGs();
// 
// 		for (int i=0 ; i<PartnerCompoGArr.size() ; ++i)
// 		{
// 			MSCompoG* pPartnerCompoG = PartnerCompoGArr[i];
// 			UpdateCompoGMap.insert(make_pair(pPartnerCompoG->m_ID, pPartnerCompoG));
// 		}
// 
// 		pCompoG->SetConnectionSlabGID();
// 		vector<long> SlabIdArr = pCompoG->GetConnectionSlabGID();
// 		for (int i=0 ; i < SlabIdArr.size() ; ++i)
// 		{
// 			map<long, MSCompoG*>::iterator Slabit = mm_CompoG.find(SlabIdArr[i]);
// 			MSCompoG* pPartnerCompoG = Slabit->second;
// 			UpdateCompoGMap.insert(make_pair(pPartnerCompoG->m_ID, pPartnerCompoG));
// 		}
// 		//pCompoG->UpdateConnectionSlabInnerProFile();
// 
// 	}
// 
// 	map<long, MSCompoG*>::iterator it;
// 	for(it = UpdateCompoGMap.begin() ; it != UpdateCompoGMap.end() ; it++)
// 	{
// 		MSCompoG* pCompoG = it->second;
// 		UpdateTrimmedBoundary(pCompoG);
// 		pCompoG->UpdateBoundaryVisibilities();
// 		CalculateUpdatedCompoGArray(pCompoG, CompoGIDList_NeedRegen_CurTR());
// 	}
// }

// 연결된 부재에서 Modify시 bFirst = false
bool MSFloor::Modify(MSCompoG* pCompoG, bool bFirst)
{
	CString strLog;
	strLog.Format(_T("MSFloor::Modify(GID:%d, bFirst:%s)"), pCompoG->m_ID, bFirst ? _T("true") : _T("false"));
	UpdateConnectionCompoGMap.clear();
	//StatusReporterWrapper::WriteMethodProcessTime(strLog, true);

	// Modify시 TrimmedBoundary와 Boundary가 불일치 되는 경우 존재 함
	// InitTrimmedBoundary() 호출
	pCompoG->InitTrimmedBoundary();
	pCompoG->MakeCCW();

	map<long, MSCompoG*>::iterator it;
	map<long, MSCompoG*> CompoGMap;
	MSElement::Type_MSCompoG eType = pCompoG->GetType();
	if (bFirst == true &&
		(eType == MSElement::msSlabG || eType == MSElement::msOpeningG)
		)
	{
		GM2DPolyline* pPoly = pCompoG->GetProfile();
		pPoly->CheckAndFixFoldedEdges(DTOL_GM);
	}

	m_nCmdCnt++;
	pCompoG->m_nCmdCnt = m_nCmdCnt;

	// 변경될때 기존에 pCompoG에 연결되어 있는 CompoG를 업데이트 한다.
	CalculateUpdatedCompoGArray(pCompoG, CompoGIDList_NeedRegen_CurTR(), true);

	vector<MSCompoG*> PartnerCompoGArr = pCompoG->GetPartnerCompoGs();

	long nMyQT = pCompoG->ma_CrossQT.GetSize();
	for (long iMyQT = 0; iMyQT < nMyQT; iMyQT++)
		pCompoG->ma_CrossQT[iMyQT]->Remove(pCompoG);
	pCompoG->ma_CrossQT.RemoveAll();
	mp_QT->AddToLeaves(pCompoG);

	pCompoG->InitializeParameter();
	//쿼드 트리 검색
	vector<MSCompoG*> compoGArr;
	compoGArr.push_back(pCompoG);
	CheckFloorQT(compoGArr);

	if (pCompoG->IsLinkMember())
	{
		MSLinkMemberG* pLinkMemberG = (MSLinkMemberG*)pCompoG;
		if (pLinkMemberG->m_isBuriedLinkMember) return false;

		pLinkMemberG->SetLines();
		// 바닥부재의 경우 레벨존에 따라 부재 단차 배열 생성
		InsertLevelDif2ALinkMemberByLevelZone((MSLinkMemberG*)pCompoG);
	}
	else if (pCompoG->IsPointMember())
		((MSPointMemberG*)pCompoG)->UpdateVecInputByCenVec();

	if (pCompoG->GetType() == MSElement::msOpeningG)
	{
		MSOpeningG* pOpeningG = (MSOpeningG*)pCompoG;
		vector<MSCompoG*> SlabArr = pOpeningG->GetPartnerCompoGs();
		for (long iSlab = 0; iSlab < SlabArr.size(); iSlab++)
			Modify(SlabArr[iSlab], false);
	}

	// 오버랩 된 슬래브를 찾아줌
	if (pCompoG->GetType() == MSElement::msPitG)
	{
		MSPitG* pCurPit = (MSPitG*)pCompoG;
		for (long iSlab = 0; iSlab < pCurPit->ma_SlabGID.size(); iSlab++)
		{
			long nSlabID = pCurPit->ma_SlabGID[iSlab];
			MSSlabG* pSlabG = (MSSlabG*)GetCompoG(nSlabID);
			if (pSlabG != NULL)
				Modify(pSlabG, false);
		}
		pCurPit->MakeConnectedSlabInfo();
	}

	if (pCompoG->GetType() == MSElement::msStairG)
	{
		MSGenStairG* pStair = (MSGenStairG*)pCompoG;
		pStair->CalculatePartHeight();

		vector<MSCompoG*> UpStairArr = pStair->FindConnectMember(FindType_MSElement::msStairG, FIND_UPFLOOR);
		vector<MSCompoG*> DnStairArr = pStair->FindConnectMember(FindType_MSElement::msStairG, FIND_DNFLOOR);
		for (int i = 0; i < UpStairArr.size(); i++)
			CalculateUpdatedCompoGArray(UpStairArr[i], CompoGIDList_NeedRegen_CurTR(), false);
		for (int i = 0; i < DnStairArr.size(); i++)
			CalculateUpdatedCompoGArray(DnStairArr[i], CompoGIDList_NeedRegen_CurTR(), false);
	}

	/* // Old Logic
	vector<MSCompoG*> PartnerCompoGArr = pCompoG->GetPartnerCompoGs();
	pCompoG->RemoveConnections();
	UpdateTrimmedBoundary(PartnerCompoGArr);
	for (long iPartner = 0; iPartner < PartnerCompoGArr.size(); iPartner++)
		PartnerCompoGArr[iPartner]->UpdateBoundaryVisibilities();

	// 변경된 pCompoG에 연결되어 있는 CompoG를 업데이트 한다.
	FindOverlapMembers(pCompoG);
	UpdateTrimmedBoundary(pCompoG);

	PartnerCompoGArr = pCompoG->GetPartnerCompoGs();
	UpdateTrimmedBoundary(PartnerCompoGArr);

	/////////////////////////////////////////////////////////////////////////////
	// FindOverlap one-more-time after Trim
	FindOverlapMembers(pCompoG, true);
	for (int i=0 ; i<PartnerCompoGArr.size() ; ++i)
		FindOverlapMembers(PartnerCompoGArr[i], true);
	// FindOverlap one-more-time after Trim 이후 파트너CompoG가 변경된 경우가 존재하므로 다시 찾아준다.
	PartnerCompoGArr = pCompoG->GetPartnerCompoGs();
	/////////////////////////////////////////////////////////////////////////////
	*/

	if (eType == MSElement::msSlabG)
	{
		MSSlabG* pSlabG = (MSSlabG*)pCompoG;
		pSlabG->UpdatePartnerSlab();
	}

	pCompoG->RemoveConnections();
	for (long iPartner = 0; iPartner < PartnerCompoGArr.size(); iPartner++)
	{
		MSCompoG* pPartnerG = PartnerCompoGArr[iPartner];
		for (int iConn = 0; iConn < pCompoG->ma_Connections.size(); iConn++)
			pPartnerG->RemoveConnection(pCompoG->ma_Connections[iConn]);

		long nMyQT = PartnerCompoGArr[iPartner]->ma_CrossQT.GetSize();
		for (long iMyQT = 0; iMyQT < nMyQT; iMyQT++)
			pPartnerG->ma_CrossQT[iMyQT]->Remove(pCompoG);

		UpdateConnectionCompoGMap[pPartnerG->m_ID] = pPartnerG;
	}

	FindOverlapMembernUpdateTrimBoundary(pCompoG, false);

	// 슬래브의 경우 레벨존에 따라 부재 단차 배열 생성
	if (pCompoG->GetType() == MSElement::msSlabG)
	{
		MSSlabG* pSlabG = (MSSlabG*)pCompoG;
		pSlabG->mp_PolylineInfo->TrimExistPolyLineObject(true);
		pSlabG->mp_PolylineInfo->TrimExistPolyLineObject(false);
		InsertLevelDif2SlabByLevelZone(pSlabG, false);

		bool bOld = m_isRunModifyObjectCompoG;
		m_isRunModifyObjectCompoG = false;
		for (int i = 0; i < pSlabG->ma_LevelPoly.size(); i++)
		{
			MSSlabLevelZone* pSlabLevelZone = pSlabG->ma_LevelPoly[i];
			if (pSlabLevelZone->mp_LevelZone != NULL && pSlabLevelZone->m_IsDelete == false)
				ModifyLevelZoneByElement(pSlabLevelZone->mp_LevelZone);
		}
		m_isRunModifyObjectCompoG = bOld;
		pSlabG->ResetSlabLevelZone();

		//pSlabG->TrimLevelDifAll();
		pSlabG->UpdateSubMember();

		vector<MSCompoG*> PitArr;
		GetMSCompoGArr(MSElement::msPitG, PitArr);
		for (long iMem = 0; iMem < PitArr.size(); iMem++)
		{
			MSPitG* pCurPit = (MSPitG*)PitArr[iMem];
			pCurPit->RemoveSlabGID(pCompoG->m_ID);
			pCurPit->MakeConnectedSlabInfoStep2(pCompoG->mp_PolylineInfo);
		}
	}


	vector<MSCompoG*> newPartnerCompoGArr = pCompoG->GetPartnerCompoGs();
	for (long iPartner = 0; iPartner < newPartnerCompoGArr.size(); iPartner++)
	{
		MSCompoG* newPartnerG = newPartnerCompoGArr[iPartner];
		bool isFind = false;
		for (int i = 0; i < PartnerCompoGArr.size(); i++)
		{
			if (PartnerCompoGArr[i] == newPartnerG)
			{
				isFind = true;
				break;
			}
		}

		if (isFind == false)
			PartnerCompoGArr.push_back(newPartnerG);
	}

	for (long iPartner = 0; iPartner < PartnerCompoGArr.size(); iPartner++)
		FindOverlapMembernUpdateTrimBoundary(PartnerCompoGArr[iPartner], true);

	pCompoG->UpdateBoundaryVisibilities();


	map<long, MSCompoG*>::iterator itMem;
	for (itMem = UpdateConnectionCompoGMap.begin(); itMem != UpdateConnectionCompoGMap.end(); itMem++)
	{
		MSCompoG* pPartnerG = itMem->second;
		if (pPartnerG->IsPointMember())
			continue;
		//if (pCompoG->IsVerticalMember() || (pCompoG->IsFloorMember() && pPartnerG->IsFloorMember()))
		pPartnerG->UpdateBoundaryVisibilities();
		CalculateUpdatedCompoGArray(pPartnerG, CompoGIDList_NeedRegen_CurTR(), false);
	}

	CalculateUpdatedCompoGArray(pCompoG, CompoGIDList_NeedRegen_CurTR());

	pCompoG->AddCompoGZLevelInFloor();

	CheckMSModel(pCompoG);



	map<long, MSFloor*> SlabIDMap = pCompoG->GetConnectionSlabGID();
	pCompoG->SetConnectionSlabGID();
	map<long, MSFloor*> SlabIDMap2 = pCompoG->GetConnectionSlabGID();

	map<long, MSFloor*>::iterator itMap;
	for (itMap = SlabIDMap.begin(); itMap != SlabIDMap.end(); itMap++)
	{
		MSFloor* pFloor = itMap->second;
		MSSlabG* pSlabG = (MSSlabG*)pFloor->GetCompoG(itMap->first);
		if (pSlabG != NULL &&
			SlabIDMap2.find(pSlabG->m_ID) == SlabIDMap2.end())
		{
			//pSlabG->UpdateInnerProfile();
			pSlabG->UpdateBoundaryVisibilities();
			CalculateUpdatedCompoGArray(pSlabG, CompoGIDList_NeedRegen_2DView_CurTR(), false);
		}
	}

	for (itMap = SlabIDMap2.begin(); itMap != SlabIDMap2.end(); itMap++)
	{
		MSFloor* pFloor = itMap->second;
		MSSlabG* pSlabG = (MSSlabG*)pFloor->GetCompoG(itMap->first);
		if (pSlabG != NULL)
		{
			//pSlabG->UpdateInnerProfile();
			//pSlabG->mp_PolylineInfo->TrimExistPolyLineObject(true);
			//pSlabG->mp_PolylineInfo->TrimExistPolyLineObject(false);
			pSlabG->UpdateBoundaryVisibilities();
			CalculateUpdatedCompoGArray(pSlabG, CompoGIDList_NeedRegen_2DView_CurTR(), false);
		}
	}

	// 변경될 슬래브가 포함된 슬래브의 LevelDif를 업데이트 하기 위해 호출한다.
	if (bFirst == true && eType == MSElement::msSlabG)
	{
		MSSlabG* pSlabG = (MSSlabG*)pCompoG;
		if (pSlabG->GetContainingElementId() != 0)
		{
			MSSlabG* pOverLapSlab = (MSSlabG*)GetCompoG(pSlabG->GetContainingElementId());
			if (pOverLapSlab != nullptr)
				Modify(pOverLapSlab, false);
			else
				ASSERT(0);
		}
	}

	if (eType == MSElement::msSlabG)
	{
		MSSlabG* pSlabG = (MSSlabG*)pCompoG;
		pSlabG->UpdatePartnerSlab();
	}

	pCompoG->m_isCalculateUpdatedCompoGArray = true;

	UpdateConnectionCompoGMap.clear();
	//StatusReporterWrapper::WriteMethodProcessTime(strLog, false);
	return true;
}

void MSFloor::RegenMSModel_AllFloor()
{
	/*
	MSFloor* floor = this;
	MSBaseBuilding* bldg = floor->GetBuilding();
	vector<MSFloor*> floors;
	bldg->GetAllFloors(floors);

	vector<map<long, MSCompoG*>> floorBackUp;
	floorBackUp.resize(floors.size()); // 각각의 기본생성자 불림

	// 1) 백업 및 초기화
	map<MSLinkMemPart*, bool> deleteMemPart;
	map<MSConnectivity*, bool> deleteJoint;
	for (int i=0 ; i<floors.size() ; ++i)
	{
		floorBackUp[i] = this->mm_CompoG; // CompoG 는 재활용 (여기선 clear 해줘야 함)
		for(map<long, MSCompoG*>::iterator itr = floorBackUp[i].begin() ; itr != floorBackUp[i].end(); ++itr)
		{
			itr->second->ma_Joint.clear();
			MSLinkMemberG* link = dynamic_cast<MSLinkMemberG*>(itr->second);
			if (link != NULL)
			{
				for (int i=0 ; i<link->ma_MemPart.size() ; ++i)
					deleteMemPart[link->ma_MemPart[i]] = true;
				link->ma_MemPart.clear();
				link->ma_MidJoint.clear();
			}
		}
		floors[i]->mm_CompoG.clear();

		for (map<long, MSConnectivity*>::iterator itr = floors[i]->mm_Joint.begin() ; itr != floors[i]->mm_Joint.end() ; ++itr)
			deleteJoint[itr->second] = true;
		floors[i]->mm_Joint.clear();

		floors[i]->InitQT();
	}

	for (map<MSLinkMemPart*, bool>::iterator itr = deleteMemPart.begin() ; itr != deleteMemPart.end() ; ++itr)
		delete itr->first;
	for (map<MSConnectivity*, bool>::iterator itr = deleteJoint.begin() ; itr != deleteJoint.end() ; ++itr)
		delete itr->first;

	// 2) ADD
	for (int i=0 ; i<floorBackUp.size() ; ++i)
	{
		for(map<long, MSCompoG*>::iterator itr = floorBackUp[i].begin() ; itr != floorBackUp[i].end(); ++itr)
		{
			floors[i]->Add(itr->second);
		}
	}
*/
}

void MSFloor::CheckMSModel_AllFloor()
{
	/*
	AllocConsole();
	AttachConsole( GetCurrentProcessId() ) ;
	freopen( "CON", "w", stdout ) ;
	wprintf(_T("------------------------------------------------------------------\n"));
	*/

	MSFloor* floor = this;
	MSBaseBuilding* bldg = floor->GetBuilding();
	vector<MSFloor*> floors = bldg->GetAllFloors();
	for (int i = 0; i < floors.size(); ++i)
		floors[i]->CheckMSModel();

	/*
	wprintf(_T("------------------------------------------------------------------\n"));
	*/
}

void MSFloor::CheckMSModel(MSCompoG* compoG)
{
	vector<int> delIdx;
	vector<MSFloor*> FloorArr = compoG->GetOverlapFloor();

	for (int i = 0; i < compoG->ma_Connections.size(); ++i)
	{
		bool error = false;
		MSConnection* con = compoG->ma_Connections[i];
		if (con->GetFirstCompoG() == con->GetSecondCompoG())
			error = true;

		if (error == false)
		{
			if (con->GetFirstCompoG() == NULL)
				error = true;
			else
			{
				MSCompoG* pFindCompoG = NULL;
				for (int iFloor = 0; iFloor < FloorArr.size(); iFloor++)
				{
					MSFloor* pFloor = FloorArr[iFloor];
					pFindCompoG = pFloor->GetCompoG(con->GetFirstCompoG()->m_ID);
					if (pFindCompoG != NULL)
						break;
				}
				if (pFindCompoG == NULL)
					error = true;
			}
		}

		if (error = false)
		{
			if (con->GetSecondCompoG() == NULL)
				error = true;
			else
			{
				MSCompoG* pFindCompoG = NULL;
				for (int iFloor = 0; iFloor < FloorArr.size(); iFloor++)
				{
					MSFloor* pFloor = FloorArr[iFloor];
					pFindCompoG = pFloor->GetCompoG(con->GetSecondCompoG()->m_ID);
					if (pFindCompoG != NULL)
						break;
				}
				if (pFindCompoG == NULL)
					error = true;
			}
		}

		if (error) delIdx.push_back(i);
		//if (error) break;
	}

	if (delIdx.size() > 0)
	{
		int num = delIdx.size();
		for (int i = num - 1; i >= 0; --i)
		{
			int idx = delIdx[i];
			compoG->ma_Connections.erase(compoG->ma_Connections.begin() + idx);
			// ToDo : delete 는 일단 skip
		}

		CString str;
		str.Format(_T("CompoG(ID:%d) Connection Data Error!\n"), compoG->m_ID);
#ifdef DEBUG
		AllocConsole(); AttachConsole(GetCurrentProcessId()); freopen("CON", "w", stdout);
		wprintf(str);
#else
		StatusReporterWrapper::OutputError(str);
#endif


		//Modify(compoG); // fix error
	}
}

void MSFloor::CheckMSModel()
{
	for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
	{
		CheckMSModel(itr->second);
	}
	/*
	vector<CString> msg_list;
	CString str;

	MSFloor* floor = this;
	// CompoG JOINT 체크
	for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin() ; itr != mm_CompoG.end(); ++itr)
	{
		MSSlabG* slab = dynamic_cast<MSSlabG*>(itr->second);
		for (int i=0 ; i<itr->second->ma_Joint.size() ; ++i)
		{
			if (slab != NULL) continue;
			MSConnectivity* joint = itr->second->ma_Joint[i];
			if (joint ==NULL) {
				str.Format(_T("FLOOR %d: CompoG %d -> ma_Joint(idx:%d) is NULL\n"), floor->m_nFloorNum, itr->second->m_ID, i); msg_list.push_back(str); }
			else if (joint->IsDeleted()) {
				str.Format(_T("FLOOR %d: CompoG %d -> ma_Joint(idx:%d) is Deleted\n"), floor->m_nFloorNum, itr->second->m_ID, i); msg_list.push_back(str); }
			else if (itr->second->GetType()!=MSElement::msSlabG && floor->mm_Joint.count(joint->m_ID)==0) {
				str.Format(_T("FLOOR %d: CompoG %d -> ma_Joint(ID:%d) is not exist in floor mm_Joint\n"), floor->m_nFloorNum, itr->second->m_ID, joint->m_ID); msg_list.push_back(str); }
		}
		MSLinkMemberG* link = dynamic_cast<MSLinkMemberG*>(itr->second);
		if (link == NULL) continue;
		for (int i=0 ; i<link->ma_MidJoint.size() ; ++i)
		{
			MSConnectivity* joint = link->ma_MidJoint[i];
			if (joint ==NULL) {
				str.Format(_T("FLOOR %d: CompoG %d -> ma_MidJoint(idx:%d) is NULL\n"), floor->m_nFloorNum, itr->second->m_ID, i); msg_list.push_back(str); }
			else if (joint->IsDeleted()) {
				str.Format(_T("FLOOR %d: CompoG %d -> ma_MidJoint(idx:%d) is Deleted\n"), floor->m_nFloorNum, itr->second->m_ID, i); msg_list.push_back(str); }
			else if (floor->mm_Joint.count(joint->m_ID)==0) {
				str.Format(_T("FLOOR %d: CompoG %d -> ma_MidJoint(ID:%d) is not exist in floor mm_Joint\n"), floor->m_nFloorNum, itr->second->m_ID, joint->m_ID); msg_list.push_back(str); }
		}
	}

	// Joint MemPart 체크
	for (map<long, MSConnectivity*>::iterator itr = mm_Joint.begin() ; itr != mm_Joint.end() ; ++itr)
	{
		MSConnectivity* joint = itr->second;
		for (int i=0 ; i<joint->ma_MemPart.size() ; ++i)
		{
			MSLinkMemPart* part = joint->ma_MemPart[i];
			if (part == NULL) {
				str.Format(_T("FLOOR %d: JOINT %d(%g,%g): MemPart(idx:%d) is NULL\n"), floor->m_nFloorNum, joint->m_ID, joint->m_dXc, joint->m_dYc, i); msg_list.push_back(str); }
			else if (part->isDelete()) {
				str.Format(_T("FLOOR %d: JOINT %d(%g,%g): MemPart(idx:%d) is Deleted\n"), floor->m_nFloorNum, joint->m_ID, joint->m_dXc, joint->m_dYc, i); msg_list.push_back(str); }
			else if (part->mp_LinkMem == NULL) {
				str.Format(_T("FLOOR %d: JOINT %d(%g,%g): MemPart(idx:%d)->mp_LinkMem is NULL\n"), floor->m_nFloorNum, joint->m_ID, joint->m_dXc, joint->m_dYc, i); msg_list.push_back(str); }
			else if (floor->mm_CompoG.count(part->mp_LinkMem->m_ID) == 0) {
				str.Format(_T("FLOOR %d: JOINT %d(%g,%g): MemPart(idx:%d)->mp_LinkMem(ID:%d) is Not Exist in Floor mm_CompoG\n"), floor->m_nFloorNum, joint->m_ID, joint->m_dXc, joint->m_dYc, i, part->mp_LinkMem->m_ID); msg_list.push_back(str); }
		}
	}

	// Joint SptMember 체크
	for (map<long, MSConnectivity*>::iterator itr = mm_Joint.begin() ; itr != mm_Joint.end() ; ++itr)
	{
		MSConnectivity* joint = itr->second;
		for (int i=0 ; i<joint->ma_SptMember.size() ; ++i)
		{
			MSLinkMemberG* part = joint->ma_SptMember[i];
			if (part == NULL) {
				str.Format(_T("FLOOR %d: JOINT %d(%g,%g): ma_SptMember(idx:%d) is NULL\n"), floor->m_nFloorNum, joint->m_ID, joint->m_dXc, joint->m_dYc, i); msg_list.push_back(str); }
			else if (floor->mm_CompoG.count(part->m_ID) == 0) {
				str.Format(_T("FLOOR %d: JOINT %d(%g,%g): ma_SptMember(idx:%d, ID:%d) is Not Exist in Floor mm_CompoG\n"), floor->m_nFloorNum, joint->m_ID, joint->m_dXc, joint->m_dYc, i, part->m_ID); msg_list.push_back(str); }
		}
	}
	*/

	// Joint ConLevelZone MemPart 체크
	/*
	for (map<long, MSConnectivity*>::iterator itr = mm_Joint.begin() ; itr != mm_Joint.end() ; ++itr)
	{
		MSConnectivity* joint = itr->second;
		for (int i=0 ; i<joint->ma_LevelZone.GetSize() ; ++i)
		{
			for (int k=0 ; k<joint->ma_LevelZone[i]->ma_MemPart.GetSize() ; ++k)
			{
				MSLinkMemPart* part = joint->ma_LevelZone[i]->ma_MemPart[k];
				if (part == NULL) {
					str.Format(_T("FLOOR %d: JOINT %d LVZONE(%d): MemPart(idx:%d) is NULL\n"), floor->m_nFloorNum, joint->m_ID, i, k); msg_list.push_back(str); }
				else if (part->isDelete()) {
					str.Format(_T("FLOOR %d: JOINT %d LVZONE(%d): MemPart(idx:%d) is Deleted\n"), floor->m_nFloorNum, joint->m_ID, i, k); msg_list.push_back(str); }
				else if (part->mp_LinkMem == NULL) {
					str.Format(_T("FLOOR %d: JOINT %d LVZONE(%d): MemPart(idx:%d)->mp_LinkMem is NULL\n"), floor->m_nFloorNum, joint->m_ID, i, k); msg_list.push_back(str); }
				else if (floor->mm_CompoG.count(part->mp_LinkMem->m_ID) == 0) {
					str.Format(_T("FLOOR %d: JOINT %d LVZONE(%d): MemPart(idx:%d)->mp_LinkMem(ID:%d) is Not Exist in Floor mm_CompoG\n"), floor->m_nFloorNum, joint->m_ID, i, k, part->mp_LinkMem->m_ID); msg_list.push_back(str); }
			}
		}
	}
	*/

	/*
	// Joint MemPart, MemPartDegree 갯수 체크
	for (map<long, MSConnectivity*>::iterator itr = mm_Joint.begin() ; itr != mm_Joint.end() ; ++itr)
	{
		MSConnectivity* joint = itr->second;
		if (joint->ma_MemPart.size() != joint->ma_MemPartDegree.size())
		{
			str.Format(_T("FLOOR:%d, JOINT %d: MemPartNum(%d), MemPartDegreeNum(%d)\n"), floor->m_nFloorNum, joint->m_ID, joint->ma_MemPart.size(), joint->ma_MemPartDegree.size());
			msg_list.push_back(str);
		}
	}

	if (msg_list.size() > 0)
	{
		AllocConsole();
		AttachConsole( GetCurrentProcessId() ) ;
		freopen( "CON", "w", stdout ) ;
		for (int i=0 ; i<msg_list.size() ; ++i)
			wprintf(msg_list[i]);
	}
	*/
}

void MSFloor::CheckJointAll()
{
	return; // 체크시 여기 주석
	/*
	int count = 0;
	int count2 = 0;
	map<MSConnectivity*, bool> vJoint;
	for (map<long, MSConnectivity*>::iterator itr = mm_Joint.begin() ; itr != mm_Joint.end() ; ++itr)
	{
		MSConnectivity* joint = itr->second;
		for (int i=0 ; i<joint->ma_MemPart.size() ; ++i)
		{
			MSLinkMemberG* link = joint->ma_MemPart[i]->mp_LinkMem;
			if (mm_CompoG.count(link->m_ID)==0)
			{
				count++;
				vJoint[joint] = false;
				break;
			}
		}

		bool ok_exist = false;
		for (int i=0 ; i<joint->ma_MemPart.size() ; ++i)
		{
			MSLinkMemberG* link = joint->ma_MemPart[i]->mp_LinkMem;
			if (mm_CompoG.count(link->m_ID) > 0)
			{
				ok_exist = true;
				break;
			}
		}
		if (ok_exist == false)
		{
			count2++;
		}
	}
	if (count > 0 || count2 > 0)
	{
		for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin() ; itr != mm_CompoG.end(); ++itr)
		{
			for (int i=0 ; i<itr->second->ma_Joint.size() ; ++i)
			{
				if (vJoint.count(itr->second->ma_Joint[i]) > 0)
					vJoint[itr->second->ma_Joint[i]] = true;
			}
			MSLinkMemberG* link = dynamic_cast<MSLinkMemberG*>(itr->second);
			if (link == NULL) continue;
			for (int i=0 ; i<link->ma_MidJoint.size() ; ++i)
			{
				if (vJoint.count(link->ma_MidJoint[i]) > 0)
					vJoint[link->ma_MidJoint[i]] = true;
			}
		}
		int count3 = 0;
		for (map<MSConnectivity*, bool>::iterator itr = vJoint.begin() ; itr != vJoint.end() ; ++itr)
		{
			if (itr->second == false) count3++;
		}
		AllocConsole();
		AttachConsole( GetCurrentProcessId() ) ;
		freopen( "CON", "w", stdout ) ;
		CString str; str.Format(_T("JointNum with error MemPart : %d, JointNum with all error MemPart : %d, solo joint : %d\n"), count, count2, count3);
		wprintf(str);
	}
	*/
}

bool MSFloor::Remove(MSCompoG* pCompoG)
{
	CString strLog;
	strLog.Format(_T("MSFloor::Remove(GID:%d)"), pCompoG->m_ID);
	StatusReporterWrapper::WriteMethodProcessTime(strLog, true);
	UpdateConnectionCompoGMap.clear();

	MSElement::Type_MSCompoG eType = pCompoG->GetType();

	vector<MSCompoG*> PartnerCompoGArr = pCompoG->GetPartnerCompoGs();
	if (eType == MSElement::msSlabG)
	{
		vector<MSCompoG*> FindCompoGArr = pCompoG->FindConnectMember(pCompoG->GetFindType(), FIND_CURFLOOR);
		PartnerCompoGArr.insert(PartnerCompoGArr.end(), FindCompoGArr.begin(), FindCompoGArr.end());
	}

	long nMyQT = pCompoG->ma_CrossQT.GetSize();
	for (long iMyQT = 0; iMyQT < nMyQT; iMyQT++)
		pCompoG->ma_CrossQT[iMyQT]->Remove(pCompoG);
	pCompoG->ma_CrossQT.RemoveAll();

	long nID = pCompoG->m_ID;
	map<long, MSCompoG*>::iterator it = mm_CompoG.find(nID);
	if (it == mm_CompoG.end())	return false;

	map<long, MSLevelZone*> LevelZoneMap;
	if (eType == MSElement::msSlabG)
	{
		MSSlabG* pSlabG = (MSSlabG*)pCompoG;
		long NumLevelPoly = pSlabG->ma_LevelPoly.size();
		for (long iPoly = 0; iPoly < NumLevelPoly; iPoly++)
		{
			MSSlabLevelZone *pZone = pSlabG->ma_LevelPoly[iPoly];
			if (pZone->m_IsDelete == true) continue;

			if (pZone->mp_LevelZone != NULL)
				LevelZoneMap.insert(make_pair(pZone->m_LevelZoneID, pZone->mp_LevelZone));
		}
	}

	if (pCompoG->GetType() == MSElement::msStairG)
	{
		MSGenStairG* pStair = (MSGenStairG*)pCompoG;
		vector<MSCompoG*> UpStairArr = pStair->FindConnectMember(FindType_MSElement::msStairG, FIND_UPFLOOR);
		vector<MSCompoG*> DnStairArr = pStair->FindConnectMember(FindType_MSElement::msStairG, FIND_DNFLOOR);
		for (int i = 0; i < UpStairArr.size(); i++)
			CalculateUpdatedCompoGArray(UpStairArr[i], CompoGIDList_NeedRegen_CurTR(), false);
		for (int i = 0; i < DnStairArr.size(); i++)
			CalculateUpdatedCompoGArray(DnStairArr[i], CompoGIDList_NeedRegen_CurTR(), false);
	}

	mm_CompoG.erase(it);

	if (eType == MSElement::msSlabG)
	{
		MSSlabG* pSlabG = (MSSlabG*)pCompoG;
		map<long, MSLevelZone*>::iterator LevelZoneit;
		for (LevelZoneit = LevelZoneMap.begin(); LevelZoneit != LevelZoneMap.end(); LevelZoneit++)
		{
			MSLevelZone* pLevelZone = LevelZoneit->second;
			if (pLevelZone == NULL) continue;
			pSlabG->RemoveLevelZone(pLevelZone);
			//pLevelZone->DeleteLineLevelDifAll(); // Modify에서 처리함
			ModifyLevelZoneByElement(pLevelZone);
		}
	}

	// 오버랩 된 슬래브를 찾아줌
	if (pCompoG->GetType() == MSElement::msPitG)
	{
		MSPitG* pCurPit = (MSPitG*)pCompoG;
		for (long iSlab = 0; iSlab < pCurPit->ma_SlabGID.size(); iSlab++)
		{
			long nSlabID = pCurPit->ma_SlabGID[iSlab];
			MSSlabG* pSlabG = (MSSlabG*)GetCompoG(nSlabID);
			if (pSlabG != NULL)
				Modify(pSlabG, false);
		}
	}

	if (pCompoG->GetType() == MSElement::msOpeningG)
	{
		MSOpeningG* pOpeningG = (MSOpeningG*)pCompoG;
		vector<MSCompoG*> SlabArr = pOpeningG->GetPartnerCompoGs();
		for (long iSlab = 0; iSlab < SlabArr.size(); iSlab++)
			Modify(SlabArr[iSlab], false);
	}

	CalculateUpdatedCompoGArray(pCompoG, CompoGIDList_NeedRegen_CurTR());

	pCompoG->InitializeParameter();

	if (mp_Source != NULL)
		return true;

	/* // Old Logic
	vector<MSCompoG*> PartnerCompoGArr = pCompoG->GetPartnerCompoGs();
	pCompoG->RemoveConnections();
	UpdateTrimmedBoundary(PartnerCompoGArr);
	*/

	pCompoG->RemoveConnections();
	
	for (long iPartner = 0; iPartner < PartnerCompoGArr.size(); iPartner++)
	{
		MSCompoG* pPartnerG = PartnerCompoGArr[iPartner];
		for (int iConn = 0; iConn < pCompoG->ma_Connections.size(); iConn++)
			pPartnerG->RemoveConnection(pCompoG->ma_Connections[iConn]);

		long nMyQT = PartnerCompoGArr[iPartner]->ma_CrossQT.GetSize();
		for (long iMyQT = 0; iMyQT < nMyQT; iMyQT++)
			pPartnerG->ma_CrossQT[iMyQT]->Remove(pCompoG);

		UpdateConnectionCompoGMap[pPartnerG->m_ID] = pPartnerG;
	}

	for (long iPartner = 0; iPartner < PartnerCompoGArr.size(); iPartner++)
		FindOverlapMembernUpdateTrimBoundary(PartnerCompoGArr[iPartner], true);

	//for (long iPartner = 0; iPartner < PartnerCompoGArr.size(); iPartner++)
	map<long, MSCompoG*>::iterator itMem;
	for (itMem = UpdateConnectionCompoGMap.begin(); itMem != UpdateConnectionCompoGMap.end(); itMem++)
	{
		//MSCompoG* pPartnerG = PartnerCompoGArr[iPartner];
		MSCompoG* pPartnerG = itMem->second;
		if (pPartnerG->IsPointMember())
			continue;
		//if (pCompoG->IsVerticalMember() || (pCompoG->IsFloorMember() && pPartnerG->IsFloorMember()))
		if (pPartnerG->GetType() == MSElement::msSlabG)
		{
			MSSlabG* pSlabG = (MSSlabG*)pPartnerG;
			pSlabG->mp_PolylineInfo->TrimExistPolyLineObject(true);
			pSlabG->mp_PolylineInfo->TrimExistPolyLineObject(false);
		}
		pPartnerG->UpdateBoundaryVisibilities();
		CalculateUpdatedCompoGArray(pPartnerG, CompoGIDList_NeedRegen_CurTR(), false);
	}

	CheckMSModel(pCompoG);
	for (long iPartner = 0; iPartner < PartnerCompoGArr.size(); iPartner++)
		CheckMSModel(PartnerCompoGArr[iPartner]);

	pCompoG->DelCompoGZLevelInFloor();

	if (eType == MSElement::msOpeningG)
	{
		MSOpeningG* pOpeningG = (MSOpeningG*)pCompoG;
		vector<MSCompoG*> SlabGArr = pOpeningG->GetSlabGArr();

		for (int i = 0; i < SlabGArr.size(); i++)
		{
			MSSlabG* pSlab = (MSSlabG*)SlabGArr[i];
			pSlab->UpdateBoundaryVisibilities();
		}
	}

	pCompoG->SetConnectionSlabGID();
	pCompoG->UpdateConnectionSlabInnerProFile();

	vector<MSCompoG*> LinkedCompoGArr = pCompoG->GetLinkedCompoGArr();
	RemoveLinkedCompoG(pCompoG);

	pCompoG->m_isCalculateUpdatedCompoGArray = true;

	UpdateConnectionCompoGMap.clear();
	StatusReporterWrapper::WriteMethodProcessTime(strLog, false);
	return true;
}

// void MSFloor::Remove(vector<MSCompoG*> CompoGArr)
// {
// 	map<long, MSCompoG*> ModifyCompoGMap;
// 	map<long, MSCompoG*> UpdateCompoGMap;
// 	map<long, MSLevelZone*> LevelZoneMap;
// 
// 	for(long iCompoG = 0 ; iCompoG < CompoGArr.size() ; iCompoG++)
// 	{
// 		MSCompoG* pCompoG = CompoGArr[iCompoG];
// 
// 		long nMyQT = pCompoG->ma_CrossQT.GetSize();
// 		for(long iMyQT=0; iMyQT<nMyQT; iMyQT++)
// 			pCompoG->ma_CrossQT[iMyQT]->Remove(pCompoG);
// 		pCompoG->ma_CrossQT.RemoveAll();
// 
// 		long nID = pCompoG->m_ID;
// 		map<long, MSCompoG*>::iterator it = mm_CompoG.find(nID);
// 		if (it == mm_CompoG.end())	continue;
// 
// 		mm_CompoG.erase(it);
// 
// 		MSElement::Type_MSCompoG eType = pCompoG->GetType();
// 		if(eType == MSElement::msSlabG)
// 		{
// 			MSSlabG* pSlabG = (MSSlabG*)pCompoG;
// 			long NumLevelPoly = pSlabG->ma_LevelPoly.size();
// 			for(long iPoly = 0 ; iPoly < NumLevelPoly ; iPoly++)
// 			{
// 				MSSlabLevelZone *pZone = pSlabG->ma_LevelPoly[iPoly];
// 				if(pZone->m_IsDelete == true) continue;
// 
// 				if(pZone->mp_LevelZone != NULL)
// 					LevelZoneMap.insert(make_pair(pZone->m_LevelZoneID, pZone->mp_LevelZone));
// 
// 				pSlabG->RemoveLevelZone(pZone->mp_LevelZone);
// 			}
// 		}
// 
// 		if(pCompoG->GetType() == MSElement::msOpeningG)
// 		{
// 			MSOpeningG* pOpeningG = (MSOpeningG*)pCompoG;
// 			vector<MSCompoG*> SlabArr = pOpeningG->GetPartnerCompoGs();
// 			for(long iSlab = 0 ; iSlab < SlabArr.size() ; iSlab++)
// 				ModifyCompoGMap.insert(make_pair(SlabArr[iSlab]->m_ID, SlabArr[iSlab]));
// 		}
// 
// 		CalculateUpdatedCompoGArray(pCompoG, CompoGIDList_NeedRegen_CurTR());
// 
// 		pCompoG->ResetMempart(true);
// 		
// 		pCompoG->RemoveConnections();
// 
// 		vector<MSCompoG*> PartnerCompoGArr = pCompoG->GetPartnerCompoGs();
// 		for (long iPartner = 0; iPartner < PartnerCompoGArr.size(); iPartner++)
// 			UpdateCompoGMap.insert(make_pair(PartnerCompoGArr[iPartner]->m_ID, PartnerCompoGArr[iPartner]));
// 
// 
// 		UpdateTrimmedBoundary(PartnerCompoGArr);
// 		for (long iPartner = 0; iPartner < PartnerCompoGArr.size(); iPartner++)
// 			PartnerCompoGArr[iPartner]->UpdateBoundaryVisibilities();
// 
// 		CheckMSModel();
// 
// 		pCompoG->DelCompoGZLevelInFloor();
// 
// 		if(eType == MSElement::msOpeningG)
// 		{
// 			MSOpeningG* pOpeningG = (MSOpeningG*)pCompoG;
// 			vector<MSCompoG*> SlabGArr = pOpeningG->GetSlabGArr();
// 
// 			for(int i = 0 ; i < SlabGArr.size() ; i++)
// 			{
// 				MSSlabG* pSlab = (MSSlabG*)SlabGArr[i];
// 				UpdateCompoGMap.insert(make_pair(pSlab->m_ID, pSlab));
// 			}
// 		}
// 
// 		pCompoG->SetConnectionSlabGID();
// 		vector<long> SlabIdArr = pCompoG->GetConnectionSlabGID();
// 		for(long i = 0 ; i < SlabIdArr.size() ; i++)
// 		{
// 			map<long, MSCompoG*>::iterator Slabit = mm_CompoG.find(SlabIdArr[i]);
// 			MSCompoG* pSlab = Slabit->second;
// 			if(pSlab != NULL)
// 				UpdateCompoGMap.insert(make_pair(pSlab->m_ID, pSlab));
// 		}
// 	}
// 
// 
// 	map<long, MSLevelZone*>::iterator LevelZoneit;
// 	for(LevelZoneit = LevelZoneMap.begin() ; LevelZoneit != LevelZoneMap.end() ; LevelZoneit++)
// 	{
// 		MSLevelZone* pLevelZone = LevelZoneit->second;
// 		if(pLevelZone == NULL) continue;
// 		
// 		//pLevelZone->DeleteLineLevelDifAll(); // Modify에서 처리함
// 		Modify(pLevelZone);
// 	}
// 
// 	map<long, MSCompoG*>::iterator it;
// 	for(it = ModifyCompoGMap.begin() ; it != ModifyCompoGMap.end() ; it++)
// 	{
// 		MSCompoG* pSlab = it->second;
// 		map<long, MSCompoG*>::iterator Slabit = mm_CompoG.find(pSlab->m_ID);
// 		if (Slabit != mm_CompoG.end())
// 			Modify(pSlab, false);
// 	}
// 
// 	for (it = UpdateCompoGMap.begin() ; it != UpdateCompoGMap.end() ; it++)
// 	{
// 		UpdateTrimmedBoundary(it->second);
// 		it->second->UpdateBoundaryVisibilities();
// 	}
// 
// 
// }

bool MSFloor::Delete(MSCompoG* pCompoG)
{
	if (ms_bCloneMode)
	{
		if (!mp_Source) // Source 층일 경우
		{
			long nID = pCompoG->m_ID;
			map<long, MSCompoG*>::iterator it = mm_CompoG.find(nID);
			if (it == mm_CompoG.end())	return false;


			MSCompoG* pCloneMem;
			long nClone = pCompoG->ma_Clone.size();
			for (long iClone = 0; iClone < nClone; iClone++)
			{
				pCloneMem = pCompoG->ma_Clone[iClone];

				long nCloneFloor = ma_Clone.size();
				for (long iCloneFloor = 0; iCloneFloor < nCloneFloor; iCloneFloor++)
				{
					MSFloor* pCloneFloor = ma_Clone[iCloneFloor];
					if (pCloneFloor == pCloneMem->mp_Owner)
					{
						pCloneMem->mp_Owner->Delete(pCloneMem);
						break;
					}
				}
			}

			if (mp_QT)
				mp_QT->Remove(pCompoG);
			delete pCompoG;
			mm_CompoG.erase(it);
		}
		else  // clone 층일 경우
		{
			map<long, MSCompoG*>::iterator itCompoG = mm_CompoG.find(pCompoG->m_ID);
			if (itCompoG != mm_CompoG.end())
				mm_CompoG.erase(itCompoG);
			if (mp_QT)
				mp_QT->Remove(pCompoG);
			delete pCompoG;
		}
	}
	else
	{
		if (!mp_Source) // Source 층일 경우
		{
			long nID = pCompoG->m_ID;
			map<long, MSCompoG*>::iterator it = mm_CompoG.find(nID);
			if (it == mm_CompoG.end())	return false;

			long nCloneCompoG = pCompoG->ma_Clone.size();
			for (long iCloneCompoG = 0; iCloneCompoG < nCloneCompoG; iCloneCompoG++)
			{
				MSCompoG* pCloneCompoG = pCompoG->ma_Clone[iCloneCompoG];
				pCloneCompoG->mp_Source = NULL;
			}

			if (mp_QT)
				mp_QT->Remove(pCompoG);

			delete pCompoG;
			mm_CompoG.erase(it);
		}
		else  // clone 층일 경우
		{
			long nSourceID = 0;
			if (pCompoG->mp_Source != NULL)
			{
				nSourceID = pCompoG->mp_Source->m_ID;

				long nCloneCompoG = pCompoG->mp_Source->ma_Clone.size();
				for (long iClone = 0; iClone < nCloneCompoG; iClone++)
				{
					if (pCompoG->mp_Source->ma_Clone[iClone] == pCompoG)
					{
						pCompoG->mp_Source->ma_Clone.erase(pCompoG->mp_Source->ma_Clone.begin() + iClone);
						break;
					}
				}
			}
			map<long, MSCompoG*>::iterator itClone = mm_CompoG.find(pCompoG->m_ID);
			if (itClone != mm_CompoG.end())
				mm_CompoG.erase(itClone);

			if (mp_QT)
				mp_QT->Remove(pCompoG);

			delete pCompoG;
		}
	}
	return true;
}

bool MSFloor::Modify(MSCompoG* pOldCompoG, MSCompoG* pNewCompoG)
{
	bool Result = MSAssembly::Modify(pOldCompoG, pNewCompoG);
	if (!Result)
		return false;

	if (ms_bCloneMode && !mp_Source)
		pOldCompoG->ModifyClones();

	pNewCompoG->m_isCalculateUpdatedCompoGArray = true;
	return true;
}


MSCompoG* MSFloor::GetCompoG(long CompoGID)
{
	map<long, MSCompoG*>::iterator it = mm_CompoG.find(CompoGID);
	if (it == mm_CompoG.end())
	{
		return NULL;
	}
	return it->second;
}

MSLevelZone* MSFloor::GetLevelZone(long CompoGID)
{
	vector<MSLevelZone*>::iterator itLevelZone;
	for (itLevelZone = ma_LevelZone.begin(); itLevelZone != ma_LevelZone.end(); itLevelZone++)
	{
		MSLevelZone* levelzone = *itLevelZone;
		if (CompoGID == levelzone->m_ID)
			return levelzone;
	}
	return NULL;
}

MSSectionView* MSFloor::GetSectionView(long CompoGID)
{
	vector<MSSectionView*>::iterator itSectionView;
	for (itSectionView = ma_SectionView.begin(); itSectionView != ma_SectionView.end(); itSectionView++)
	{
		MSSectionView* levelzone = *itSectionView;
		if (CompoGID == levelzone->m_ID)
			return levelzone;
	}
	return NULL;
}

MSConstructionZone* MSFloor::GetConstructionZone(long CompoGID)
{
	map<long, MSConstructionZone*>::iterator itConstructionZone = mm_CZone.find(CompoGID);
	if (itConstructionZone != mm_CZone.end())
		return itConstructionZone->second;
	return NULL;
}

void MSFloor::Add(MSGridLine* pGridLine)
{
	pair<map<long, MSGridLine*>::iterator, bool> bPair;
	bPair = mm_GridLine.insert(make_pair(pGridLine->m_ID, pGridLine));
}

void MSFloor::Add(MSLineLevelDifG* pLineLevel)
{
	vector<double> StarLocArr;
	vector<double> EndLocArr;
	long nZone = ma_LevelZone.size();
	for (long iZone = 0; iZone < nZone; iZone++)
	{
		MSLevelZone* pZone = ma_LevelZone[iZone];
		if (pZone == pLineLevel->mp_LeftLevelZone)
			continue;
		long nLevel = pZone->ma_LineLevelDif.size();
		for (long iLevel = 0; iLevel < nLevel; iLevel++)
		{
			MSLineLevelDifG* pUrLevel = pZone->ma_LineLevelDif[iLevel];
			if (pUrLevel == NULL) continue;
			if (pLineLevel->GetLength() < MSFloor::ms_DefLineHitDistTol)
				break;;

			double UrP0LocToMe, UrP1LocToMe;
			pLineLevel->SplitAndSetRightLevelWhenOverlap(pUrLevel, UrP0LocToMe, UrP1LocToMe);
		}
	}

	if (pLineLevel->GetLength() < MSFloor::ms_DefLineHitDistTol)
	{
		delete pLineLevel;
		return;
	}

	pLineLevel->mp_LeftLevelZone->AddLevelDifG(pLineLevel);
}

bool MSFloor::CheckWhenDeleteLevelZone(int nLevelZoneID)
{
	bool ok = true;
	int idx = -1;
	for (int i = 0; i < ma_LevelZone.size(); ++i)
	{
		MSLevelZone* zone = ma_LevelZone[i];
		if (zone->m_ID == nLevelZoneID) { idx = i; break; }
	}

	if (idx == -1) return ok;

	MSLevelZone* pZone = ma_LevelZone[idx];

	// TEMP REMOVE
	ma_LevelZone.erase(ma_LevelZone.begin() + idx);

	GM2DPolyline* pPoly = pZone->GetProfile();
	vector<MSCompoG*> CompoGArr = MSCompoGUtil::FindCompoG(this, pPoly, FindType_MSElement::msNULL, false);
	for (int i = 0; i < CompoGArr.size(); ++i)
	{
		if (CompoGArr[i]->IsVerticalMember() && !CompoGArr[i]->CheckData()) { ok = false; break; }
	}

	MSFloor* upper = GetUpperFloor();
	if (upper != NULL)
	{
		CompoGArr = MSCompoGUtil::FindCompoG(upper, pPoly, FindType_MSElement::msNULL, false);
		for (int i = 0; i < CompoGArr.size(); ++i)
		{
			if (CompoGArr[i]->IsVerticalMember() && !CompoGArr[i]->CheckData()) { ok = false; break; }
		}
	}

	// TEMP REMOVE RECOVER
	ma_LevelZone.insert(ma_LevelZone.begin() + idx, pZone);

	return ok;
}

void MSFloor::Add(MSLevelZone* pZone, bool isDoit /*= true*/)
{
	if (!pZone->mp_PolylineInfo->GetProfile()->IsCCW())
		pZone->mp_PolylineInfo->GetProfile()->ReverseDir();

	GM2DPolyline* pZonePoly = pZone->GetProfile();
	vector<MSCompoG*> CompoGArr = MSCompoGUtil::FindCompoG(this, pZonePoly, FindType_MSElement::msNULL, false);
	for (long i = 0; i < CompoGArr.size(); i++)
	{
		MSCompoG* pCompoG = CompoGArr[i];;
		pCompoG->DelCompoGZLevelInFloor();
	}

	pZone->mp_PolylineInfo->TrimExistPolyLineObject();
	long nLine = pZonePoly->GetNumLines();
	for (long iLine = 0; iLine < nLine; iLine++)
	{
		MSLineLevelDifG* pNewDifG = new MSLineLevelDifG(pZone);
		pNewDifG->SetCenCurve(pZonePoly->GetCurve(iLine));
		pNewDifG->mp_LeftLevelZone = pZone;
		Add(pNewDifG);
	}

	long nLevelDif = pZone->ma_LineLevelDif.size();
	//길이, GetDepth()가 0인 LevelDif 삭제
	for (long iLevelDif = nLevelDif - 1; iLevelDif >= 0; iLevelDif--)
	{
		MSLineLevelDifG* pLevelDif = pZone->ma_LineLevelDif[iLevelDif];
		if (pLevelDif == NULL) continue;
		double dLen = pLevelDif->GetLength();
		double dDepth = pZone->ma_LineLevelDif[iLevelDif]->GetDepth();

		if (dLen < DTOL_GM)
		{
			pZone->DeleteLineLevelDif(pLevelDif);
			continue;
		}
	}

	MSAssembly::Add(pZone);


	nLevelDif = pZone->ma_LineLevelDif.size();
	for (long iLevelDif = 0; iLevelDif < nLevelDif; iLevelDif++)
		FindAndInsertLevelDif2LineMembers(pZone->ma_LineLevelDif[iLevelDif]);

	long nSlabLevelZone = 0;
	for (long iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
	{
		MSCompoG* pCompoG = CompoGArr[iCompoG];
		if (pCompoG->GetType() != MSElement::msSlabG)
			continue;

		MSSlabG* pSlab = (MSSlabG*)pCompoG;
		nSlabLevelZone += pSlab->MakeSlabLevelZone(pZone, isDoit);
		// CheckCrossThenAddLevelDif 함수에 CompoG 업데이트 포함됨
		//if (CheckLevelDifGSerializing()) // mylee - m_bIsSerializing START
		pSlab->CheckCrossThenAddLevelDif(pZone, isDoit);
		// mylee - m_bIsSerializing END 
		pSlab->TrimSlabLineLevelDifbyOpening();
	}

	pZone->TrimLevelDifbySlabG();

	long NumZone = pZone->mp_PolylineInfo->ma_UpdateMSPolylineObject.size();
	for (long iZone = 0; iZone < NumZone; iZone++)
	{
		MSLevelZone* pUpdateZone = (MSLevelZone*)pZone->mp_PolylineInfo->ma_UpdateMSPolylineObject[iZone]->GetOwner();
		if (pUpdateZone->GetType() != MSElement::msLevelZone)
			continue;
		pUpdateZone->MakeCrossThenAddLevelDif();
		pUpdateZone->TrimLevelDifbySlabG();
		CalcAffected_CompoGID_ByLevelZone(this, pUpdateZone, CompoGIDList_NeedRegen_CurTR());
	}
	pZone->mp_PolylineInfo->ma_UpdateMSPolylineObject.clear();

	map<long, MSCompoG*>::iterator it;
	for (long i = 0; i < CompoGArr.size(); i++)
	{
		MSCompoG* pCompoG = CompoGArr[i];;
		if (!pCompoG->IsPolyMember())
		{
			long nPreConnCnt = pCompoG->ma_Connections.size();
			FindOverlapMembersNCalcTrimOption(pCompoG);
			long nConnCnt = pCompoG->ma_Connections.size();
			if (nPreConnCnt != nConnCnt)
			{
				pCompoG->UpdateTrimmedBoundary(nPreConnCnt);
				pCompoG->UpdateBoundaryVisibilities(nPreConnCnt);
			}
			pCompoG->CheckZLevelConnection();
		}
		pCompoG->AddCompoGZLevelInFloor();
		CalculateUpdatedCompoGArray(pCompoG, CompoGIDList_NeedRegen_CurTR());
	}

	CalcAffected_CompoGID_ByLevelZone(this, pZone, CompoGIDList_NeedRegen_CurTR());

	//SlabLevelZone 변경된 경우만 m_PreWidth값을 변경
	if (nSlabLevelZone > 0)
		pZone->m_PreWidth = pZone->m_Width;
	//MSAssembly::Add(pZone);
}


bool MSFloor::Modify(MSLevelZone* pZone, bool isDoit)
{
	if (pZone == NULL) return false;

	map<long, MSCompoG*> CompoGMap;
	map<long, MSCompoG*> SlabGMap;
	long nLevelDif = pZone->ma_LineLevelDif.size();
	for (long iLevelDif = 0; iLevelDif < nLevelDif; iLevelDif++)
	{
		MSLineLevelDifG* pDif = pZone->ma_LineLevelDif[iLevelDif];
		for (int i = 0; i < pDif->ma_LinkMemberGID.size(); i++)
		{
			long nID = pDif->ma_LinkMemberGID[i];
			MSCompoG* pCompoG = GetCompoG(nID);
			if (pCompoG != nullptr)
				CompoGMap.insert(make_pair(nID, pCompoG));
		}
		for (auto it = pDif->mm_SlabGID.begin(); it != pDif->mm_SlabGID.end(); it++)
		{
			MSCompoG* pCompoG = GetCompoG(it->second);
			if (pCompoG != nullptr)
				SlabGMap.insert(make_pair(it->second, pCompoG));
		}

		FindAndRemoveLevelDif2LineMembers(pZone->ma_LineLevelDif[iLevelDif]);
	}

	for (auto it = CompoGMap.begin(); it != CompoGMap.end(); it++)
		it->second->DelCompoGZLevelInFloor();
	for (auto it = SlabGMap.begin(); it != SlabGMap.end(); it++)
		((MSSlabG*)(it->second))->RemoveLevelZone(pZone);


	map<long, MSLevelZone*> ChangLevelZone;
	long NumDifG = pZone->ma_LineLevelDif.size();
	for (long iDif = 0; iDif < NumDifG; iDif++)
	{
		MSLineLevelDifG* pDifG = pZone->ma_LineLevelDif[iDif];
		if (pDifG == NULL) continue;
		if ((pDifG->mp_LeftLevelZone != NULL && pDifG->mp_LeftLevelZone != pZone))
			ChangLevelZone.insert(make_pair(pDifG->mp_LeftLevelZone->m_ID, pDifG->mp_LeftLevelZone));
		if ((pDifG->mp_RightLevelZone != NULL && pDifG->mp_RightLevelZone != pZone))
			ChangLevelZone.insert(make_pair(pDifG->mp_RightLevelZone->m_ID, pDifG->mp_RightLevelZone));
	}
	pZone->mp_PolylineInfo->GetProfile()->DeleteInLinePoint();
	pZone->DeleteLineLevelDifAll();
	bool bRval = MSAssembly::Remove(pZone);

	map<long, MSLevelZone*>::iterator iter;
	for (iter = ChangLevelZone.begin(); iter != ChangLevelZone.end(); iter++)
	{
		MSLevelZone* pChaneLevelZone = (MSLevelZone*)iter->second;
		ModifyLevelZoneByElement(pChaneLevelZone);
	}

	map<long, MSCompoG*>::iterator it;
	for (auto it = CompoGMap.begin(); it != CompoGMap.end(); it++)
	{
		MSCompoG* pCompoG = it->second;
		long nPreConnCnt = pCompoG->ma_Connections.size();
		FindOverlapMembersNCalcTrimOption(pCompoG);
		long nConnCnt = pCompoG->ma_Connections.size();
		if (nPreConnCnt < nConnCnt)
		{
			pCompoG->UpdateTrimmedBoundary(nPreConnCnt);
			pCompoG->UpdateBoundaryVisibilities(nPreConnCnt);
		}
		pCompoG->CheckZLevelConnection();
		pCompoG->AddCompoGZLevelInFloor();

		CalculateUpdatedCompoGArray(pCompoG, CompoGIDList_NeedRegen_CurTR());
	}

	for (auto it = SlabGMap.begin(); it != SlabGMap.end(); it++)
		CalculateUpdatedCompoGArray(it->second, CompoGIDList_NeedRegen_CurTR());

	// add
	if (!pZone->mp_PolylineInfo->GetProfile()->IsCCW())
		pZone->mp_PolylineInfo->GetProfile()->ReverseDir();

	GM2DPolyline* pZonePoly = pZone->GetProfile();
	vector<MSCompoG*> CompoGArr = MSCompoGUtil::FindCompoG(this, pZonePoly, FindType_MSElement::msNULL, false);
	for (long i = 0; i < CompoGArr.size(); i++)
	{
		MSCompoG* pCompoG = CompoGArr[i];;
		pCompoG->DelCompoGZLevelInFloor();
	}

	pZone->mp_PolylineInfo->TrimExistPolyLineObject();
	long nLine = pZonePoly->GetNumLines();
	for (long iLine = 0; iLine < nLine; iLine++)
	{
		MSLineLevelDifG* pNewDifG = new MSLineLevelDifG(pZone);
		pNewDifG->SetCenCurve(pZonePoly->GetCurve(iLine));
		pNewDifG->mp_LeftLevelZone = pZone;
		Add(pNewDifG);
	}

	nLevelDif = pZone->ma_LineLevelDif.size();
	//길이, GetDepth()가 0인 LevelDif 삭제
	for (long iLevelDif = nLevelDif - 1; iLevelDif >= 0; iLevelDif--)
	{
		MSLineLevelDifG* pLevelDif = pZone->ma_LineLevelDif[iLevelDif];
		if (pLevelDif == NULL) continue;
		double dLen = pLevelDif->GetLength();
		double dDepth = pZone->ma_LineLevelDif[iLevelDif]->GetDepth();

		if (dLen < DTOL_GM)
		{
			pZone->DeleteLineLevelDif(pLevelDif);
			continue;
		}
	}

	MSAssembly::Add(pZone);

	nLevelDif = pZone->ma_LineLevelDif.size();
	for (long iLevelDif = 0; iLevelDif < nLevelDif; iLevelDif++)
		FindAndInsertLevelDif2LineMembers(pZone->ma_LineLevelDif[iLevelDif]);

	long nSlabLevelZone = 0;
	for (long iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
	{
		MSCompoG* pCompoG = CompoGArr[iCompoG];
		if (pCompoG->GetType() != MSElement::msSlabG)
			continue;

		MSSlabG* pSlab = (MSSlabG*)pCompoG;
		nSlabLevelZone += pSlab->MakeSlabLevelZone(pZone, isDoit);
		// CheckCrossThenAddLevelDif 함수에 CompoG 업데이트 포함됨
		//if (CheckLevelDifGSerializing()) // mylee - m_bIsSerializing START
		pSlab->CheckCrossThenAddLevelDif(pZone, isDoit);
		// mylee - m_bIsSerializing END 
		pSlab->TrimSlabLineLevelDifbyOpening();
	}

	pZone->TrimLevelDifbySlabG();

	long NumZone = pZone->mp_PolylineInfo->ma_UpdateMSPolylineObject.size();
	for (long iZone = 0; iZone < NumZone; iZone++)
	{
		MSLevelZone* pUpdateZone = (MSLevelZone*)pZone->mp_PolylineInfo->ma_UpdateMSPolylineObject[iZone]->GetOwner();
		if (pUpdateZone->GetType() != MSElement::msLevelZone)
			continue;
		pUpdateZone->MakeCrossThenAddLevelDif();
		pUpdateZone->TrimLevelDifbySlabG();
		CalcAffected_CompoGID_ByLevelZone(this, pUpdateZone, CompoGIDList_NeedRegen_CurTR());
	}
	pZone->mp_PolylineInfo->ma_UpdateMSPolylineObject.clear();

	for (long i = 0; i < CompoGArr.size(); i++)
	{
		MSCompoG* pCompoG = CompoGArr[i];;
		CompoGMap.insert(make_pair(pCompoG->m_ID, pCompoG));
	}

	for (auto it = CompoGMap.begin(); it != CompoGMap.end(); it++)
	{
		MSCompoG* pCompoG = it->second;
		if (!pCompoG->IsPolyMember())
		{
			long nPreConnCnt = pCompoG->ma_Connections.size();
			FindOverlapMembersNCalcTrimOption(pCompoG);
			long nConnCnt = pCompoG->ma_Connections.size();
			if (nPreConnCnt != nConnCnt)
			{
				pCompoG->UpdateTrimmedBoundary(nPreConnCnt);
				pCompoG->UpdateBoundaryVisibilities(nPreConnCnt);
			}
			pCompoG->CheckZLevelConnection();
		}
		pCompoG->AddCompoGZLevelInFloor();
		CalculateUpdatedCompoGArray(pCompoG, CompoGIDList_NeedRegen_CurTR());
	}
	CalcAffected_CompoGID_ByLevelZone(this, pZone, CompoGIDList_NeedRegen_CurTR());

	//SlabLevelZone 변경된 경우만 m_PreWidth값을 변경
	if (nSlabLevelZone > 0)
		pZone->m_PreWidth = pZone->m_Width;
	return true;

	/*
	if (pZone == NULL) return false;

	pZone->mp_PolylineInfo->GetProfile()->DeleteInLinePoint();
	if(!pZone->mp_PolylineInfo->GetProfile()->IsCCW())
		pZone->mp_PolylineInfo->GetProfile()->ReverseDir();

	GM2DPolyline* pPoly = pZone->GetProfile();
	vector<MSCompoG*> CompoGArr = FindCompoG(pPoly, MSElement::msNULL, false);
	for(long i = 0 ; i < CompoGArr.size() ; i++)
	{
		MSCompoG* pCompoG = CompoGArr[i];
		pCompoG->DelCompoGZLevelInFloor();
	}

	long nSlabLevelZone = 0;
	if(bUpdateLevelZone && !m_isRunModifyObjectCompoG)
	{
		pZone->DeleteLevelZoneInfo();
		pZone->mp_PolylineInfo->TrimExistPolyLineObject();
		nSlabLevelZone = pZone->MakeLevelZoneInfo(bUpdateLevelZone);
		pZone->TrimLevelDifbySlabG();

		long NumZone = pZone->mp_PolylineInfo->ma_UpdateMSPolylineObject.size();
		for(long iZone = 0 ; iZone < NumZone ; iZone++)
		{
			MSLevelZone* pUpdateZone = (MSLevelZone*)pZone->mp_PolylineInfo->ma_UpdateMSPolylineObject[iZone]->GetOwner();
			pUpdateZone->MakeCrossThenAddLevelDif();
		}
		pZone->FindAndInsertLevelDif2LineMembers();
	}

	map<long, MSCompoG*>::iterator it;
	for(long i = 0 ; i < CompoGArr.size() ; i++)
	{
		MSCompoG* pCompoG = CompoGArr[i];
		long nPreConnCnt = pCompoG->ma_Connections.size();
		FindOverlapMembers(pCompoG);
		long nConnCnt = pCompoG->ma_Connections.size();
		if(nPreConnCnt < nConnCnt)
		{
			pCompoG->UpdateTrimmedBoundary(nPreConnCnt);
			pCompoG->UpdateBoundaryVisibilities(nPreConnCnt);
		}
		pCompoG->CheckZLevelConnection();
		pCompoG->AddCompoGZLevelInFloor();

		CalculateUpdatedCompoGArray(pCompoG, CompoGIDList_NeedRegen_CurTR());
	}

	// mylee
	CalcAffected_CompoGID_ByLevelZone(this, pZone, CompoGIDList_NeedRegen_CurTR());

	//SlabLevelZone 변경된 경우만 m_PreWidth값을 변경
	if(nSlabLevelZone > 0)
		pZone->m_PreWidth = pZone->m_Width;
	return true;
	*/
}

bool MSFloor::ModifyLevelZoneByElement(MSLevelZone* pZone, bool bUpdateLevelZone)
{
	if (pZone == NULL) return false;

	pZone->mp_PolylineInfo->GetProfile()->DeleteInLinePoint();
	if (!pZone->mp_PolylineInfo->GetProfile()->IsCCW())
		pZone->mp_PolylineInfo->GetProfile()->ReverseDir();

	GM2DPolyline* pPoly = pZone->GetProfile();
	vector<MSCompoG*> CompoGArr = MSCompoGUtil::FindCompoG(this, pPoly, FindType_MSElement::msNULL, false);
	for (long i = 0; i < CompoGArr.size(); i++)
	{
		MSCompoG* pCompoG = CompoGArr[i];
		pCompoG->DelCompoGZLevelInFloor();
	}

	long nSlabLevelZone = 0;
	if (bUpdateLevelZone && !m_isRunModifyObjectCompoG)
	{
		pZone->DeleteLevelZoneInfo();
		pZone->mp_PolylineInfo->TrimExistPolyLineObject();
		nSlabLevelZone = pZone->MakeLevelZoneInfo(bUpdateLevelZone);
		pZone->TrimLevelDifbySlabG();

		long NumZone = pZone->mp_PolylineInfo->ma_UpdateMSPolylineObject.size();
		for (long iZone = 0; iZone < NumZone; iZone++)
		{
			MSLevelZone* pUpdateZone = (MSLevelZone*)pZone->mp_PolylineInfo->ma_UpdateMSPolylineObject[iZone]->GetOwner();
			if (pUpdateZone->GetType() != MSElement::msLevelZone)
				continue;
			pUpdateZone->MakeCrossThenAddLevelDif();
		}
		pZone->FindAndInsertLevelDif2LineMembers();

		pZone->mp_PolylineInfo->ma_UpdateMSPolylineObject.clear();
	}

	map<long, MSCompoG*>::iterator it;
	for (long i = 0; i < CompoGArr.size(); i++)
	{
		MSCompoG* pCompoG = CompoGArr[i];
		long nPreConnCnt = pCompoG->ma_Connections.size();
		FindOverlapMembersNCalcTrimOption(pCompoG);
		long nConnCnt = pCompoG->ma_Connections.size();
		if (nPreConnCnt < nConnCnt)
		{
			pCompoG->UpdateTrimmedBoundary(nPreConnCnt);
			pCompoG->UpdateBoundaryVisibilities(nPreConnCnt);
		}
		pCompoG->CheckZLevelConnection();
		pCompoG->AddCompoGZLevelInFloor();

		CalculateUpdatedCompoGArray(pCompoG, CompoGIDList_NeedRegen_CurTR());
	}

	// mylee
	CalcAffected_CompoGID_ByLevelZone(this, pZone, CompoGIDList_NeedRegen_CurTR());

	//SlabLevelZone 변경된 경우만 m_PreWidth값을 변경
	if (nSlabLevelZone > 0)
		pZone->m_PreWidth = pZone->m_Width;
	return true;
}

bool MSFloor::Remove(MSLevelZone* pZone)
{
	GM2DPolyline* pPoly = pZone->GetProfile();
	vector<MSCompoG*> CompoGArr = MSCompoGUtil::FindCompoG(this, pPoly, FindType_MSElement::msNULL, false);

	for (long i = 0; i < CompoGArr.size(); i++)
	{
		MSCompoG* pCompoG = CompoGArr[i];
		pCompoG->DelCompoGZLevelInFloor();
		if (pCompoG->GetType() == MSElement::msSlabG)
			((MSSlabG*)(pCompoG))->RemoveLevelZone(pZone);
	}

	long nLevelDif = pZone->ma_LineLevelDif.size();
	for (long iLevelDif = 0; iLevelDif < nLevelDif; iLevelDif++)
		FindAndRemoveLevelDif2LineMembers(pZone->ma_LineLevelDif[iLevelDif]);

	map<long, MSLevelZone*> ChangLevelZone;
	long NumDifG = pZone->ma_LineLevelDif.size();
	for (long iDif = 0; iDif < NumDifG; iDif++)
	{
		MSLineLevelDifG* pDifG = pZone->ma_LineLevelDif[iDif];
		if (pDifG == NULL) continue;
		if ((pDifG->mp_LeftLevelZone != NULL && pDifG->mp_LeftLevelZone != pZone))
			ChangLevelZone.insert(make_pair(pDifG->mp_LeftLevelZone->m_ID, pDifG->mp_LeftLevelZone));
		if ((pDifG->mp_RightLevelZone != NULL && pDifG->mp_RightLevelZone != pZone))
			ChangLevelZone.insert(make_pair(pDifG->mp_RightLevelZone->m_ID, pDifG->mp_RightLevelZone));
	}

	pZone->mp_PolylineInfo->GetProfile()->DeleteInLinePoint();
	pZone->DeleteLineLevelDifAll();

	bool bRval = MSAssembly::Remove(pZone);
	double OldLevel = pZone->m_Level;
	pZone->m_Level = 0;
	pZone->m_Level = OldLevel;

	map<long, MSLevelZone*>::iterator iter;
	for (iter = ChangLevelZone.begin(); iter != ChangLevelZone.end(); iter++)
	{
		MSLevelZone* pChaneLevelZone = (MSLevelZone*)iter->second;
		ModifyLevelZoneByElement(pChaneLevelZone, false);
	}

	map<long, MSCompoG*>::iterator it;
	for (long i = 0; i < CompoGArr.size(); i++)
	{
		MSCompoG* pCompoG = CompoGArr[i];
		long nPreConnCnt = pCompoG->ma_Connections.size();
		FindOverlapMembersNCalcTrimOption(pCompoG);
		long nConnCnt = pCompoG->ma_Connections.size();
		if (nPreConnCnt < nConnCnt)
		{
			pCompoG->UpdateTrimmedBoundary(nPreConnCnt);
			pCompoG->UpdateBoundaryVisibilities(nPreConnCnt);
		}
		pCompoG->CheckZLevelConnection();
		pCompoG->AddCompoGZLevelInFloor();

		CalculateUpdatedCompoGArray(pCompoG, CompoGIDList_NeedRegen_CurTR());
	}

	CalcAffected_CompoGID_ByLevelZone(this, pZone, CompoGIDList_NeedRegen_CurTR());

	return bRval;
}

void MSFloor::Add(MSSectionView* pSectionView, bool isDoit /*= true*/)
{
	MSAssembly::Add(pSectionView, isDoit);
}

bool MSFloor::Remove(MSSectionView* pSectionView)
{
	return MSAssembly::Remove(pSectionView);
}

bool MSFloor::Remove(MSGridLine* pGridLine)
{
	long nID = pGridLine->m_ID;
	map<long, MSGridLine*>::iterator itGridLine = mm_GridLine.find(nID);
	if (itGridLine == mm_GridLine.end())	return false;
	mm_GridLine.erase(itGridLine);
	return true;
}

bool MSFloor::Delete(MSGridLine* pGridLine)
{
	if (!Remove(pGridLine))
		return false;
	delete pGridLine;
	return true;
}

// bool MSFloor::Add( MSOpening* pOpening )
// {
// //     vector<MSCompoG*> SlabArr;
// //     GetMSCompoGArr(MSElement::msSlabG, SlabArr);
// 	vector<MSCompoG*> SlabArr = FindCompoG(pOpening->GetProfile(), MSElement::msSlabG, false);
// 
//     long nSlab = SlabArr.size();
//     for(long iSlab=0; iSlab<nSlab; iSlab++)
//     {
//         MSSlabG* pSlab = (MSSlabG*)(SlabArr[iSlab]);
//         pSlab->AddOpening(pOpening);
//     }
// 
//     return true;
// }

bool MSFloor::CloneFrom(MSFloor* pSource)
{
	if (mp_Source != NULL) return false;

	// 	CString nameBackup = this->GetName();//CopyFrom은 name도 복사해오므로 미리 백업해둔다.

	//mp_Source = pSource;
	CopyFromMe(pSource, true, true);
	// 	this->SetName(nameBackup);
	return 0;
}

bool MSFloor::UnClone()
{
	MSFloor* oldSource = mp_Source;
	mp_Source = NULL;
	if (!oldSource)
		return false;

	//CmdModifyVector<MSFloor*> cmd(&(oldSource->ma_Clone), eCollectionModifyType::CollectionDelete, this);
	//cmd.Doit();

	long nCloneFloor = oldSource->ma_Clone.size();
	for (long iCloneFloor = 0; iCloneFloor < nCloneFloor; iCloneFloor++)
	{
		MSFloor* pCloneFloor = oldSource->ma_Clone[iCloneFloor];
		if (pCloneFloor == this)
		{
			map<long, MSCompoG*>::iterator itMem;
			for (itMem = oldSource->mm_CompoG.begin(); itMem != oldSource->mm_CompoG.end(); itMem++)
			{
				MSCompoG* pMem = itMem->second;
				long NumCloneMem = pMem->ma_Clone.size();
				for (long iCloneMem = 0; iCloneMem < NumCloneMem; iCloneMem++)
				{
					MSCompoG* pCloneMem = pMem->ma_Clone[iCloneMem];
					if (pCloneMem->mp_Owner == this)
					{
						pMem->ma_Clone.erase(pMem->ma_Clone.begin() + iCloneMem);
						break;
					}
				}
			}
			oldSource->ma_Clone.erase(oldSource->ma_Clone.begin() + iCloneFloor);
			return true;
		}
	}

	return false;
}
void MSFloor::AddDefaultCZone()
{
	bool IsHorizontalZone = true;
	MSConstructionZone* pDefCZone = GetCZoneDefault(IsHorizontalZone);
	if (pDefCZone == NULL)
	{
		AddDefaultCZone(IsHorizontalZone);
	}
	else if (MSVersionInfo::GetCurrentVersion() < 20200727)
		pDefCZone->SetName(_T("Default CZone(H)"));

	IsHorizontalZone = false;
	pDefCZone = GetCZoneDefault(IsHorizontalZone);
	if (pDefCZone == nullptr)
		AddDefaultCZone(IsHorizontalZone);
}
void MSFloor::AddDefaultCZone(bool IsHorizontalZone)
{
	for (auto it = mm_CZone.begin(); it != mm_CZone.end(); it++)
	{
		bool isZoneType = it->second->GetZoneType() == MSConstructionZone::Horizontal ? true : false;
		if (isZoneType != IsHorizontalZone)	continue;
		if (it->second->m_bDefaultCZone == true)
			return;
	}

	if (IsHorizontalZone == true)
	{
		if (m_dHeight == 0) return;
	}
	else
	{
		CString sFloorName = GetName();
		if (sFloorName == L"" && GetLowerFloor() == nullptr) // for serialize
			return;
		else if (sFloorName == L"BOTTOM")
			return;
	}

	MSConstructionZone* czone = new MSConstructionZone();
	czone->GetID();
	//czone->m_ID = 0;
	czone->mp_Owner = this;
	czone->SetNumber(0);
	//CString str; str.Format(_T("%s_Default CZone"), GetName()); // this func is called after floor name set
	czone->m_bDefaultCZone = true;
	if (IsHorizontalZone)
	{
		czone->SetName(L"Default CZone(H)");
		czone->SetZoneType(MSConstructionZone::Horizontal);
	}
	else
	{
		czone->SetName(L"Default CZone(V)");
		czone->SetZoneType(MSConstructionZone::Vertical);
	}

	mm_CZone[czone->m_ID] = czone;
}

void MSFloor::InitElements()
{
	map<long, MSCompoG*>::iterator itMem;
	for (itMem = mm_CompoG.begin(); itMem != mm_CompoG.end(); itMem++)
	{
		MSCompoG* pMem = itMem->second;
		Delete(pMem);
	}
	mm_CompoG.clear();

	long nZone = ma_LevelZone.size();
	for (long iZone = 0; iZone < nZone; iZone++)
	{
		MSLevelZone* pZone = ma_LevelZone[iZone];
		delete pZone;
	}
	ma_LevelZone.clear();
}

long MSFloor::CloneFrom(MSFloor* pSource, bool bReset, bool bLeaveChanged, CString* pBeamPrefix/*=NULL*/, CString* pSlabPrefix/*=NULL*/)
{
	if (pSource == NULL)
		return 1;

	// 클론을 갖는 소스층은 다른 층에 클론이 될 수 없다.
	if (ma_Clone.size() != 0)
		return 2;

	if (bReset)  // 현재 층을 리셋하고 소스 층에서 카피. 쿼드트리 세업되지 않음
	{
		InitElements();
		CopyFromMe(pSource, true);
		return 0;
	}

	if (mp_Source == NULL) // 클론된 적이 없는 층이다.
	{
		if (mm_CompoG.size() > 0) // 이미 부재나 조인트를 갖고 있으면 클론 안됨.  부재 중복 등 검토 어려워. Reset 옵션 켜야 함.
			return 3;
		CopyFromMe(pSource, true);
		return 0;
	}
	else if (mp_Source != pSource)  // 다른 층에 클론되어 있거나, 과거에 클론된 적이 있다.
	{
		if (mm_CompoG.size() > 0) // 이미 부재나 조인트를 갖고 있으면 클론 안됨.  부재 중복 등 검토 어려워. Reset 옵션 켜야 함.
			return 4;
		UnClone();  // 기존 소스층과 클론 관계 단절
		CopyFromMe(pSource, true);
		return 0;
	}

	return -1;
}

void MSFloor::InitQT()
{
	if (mp_QT)
		delete mp_QT;

	mp_QT = new MSFloorQT(10, m_SiteRect.m_Origin, m_SiteRect.m_Corner);
	mp_QT->BuildLeaves(0, 1000.);
}


void MSFloor::AddCZone(MSConstructionZone* pZone)
{
	//////////////////////////////////////////////////////////////
	// Temp Code mylee - for only one default czone
	if (pZone->m_bDefaultCZone)
	{
		bool exist = false;
		for (map<long, MSConstructionZone*>::iterator itr = mm_CZone.begin(); itr != mm_CZone.end(); ++itr)
		{
			if (itr->second->m_bDefaultCZone && itr->second->GetZoneType() == pZone->GetZoneType())
			{
				exist = true; break;
			}
		}
		if (exist)
			return;
	}
	//////////////////////////////////////////////////////////////
	if (pZone->m_ID == 0)	pZone->GetID();

	vector<MSConstructionZone*> OverlapCZoneArr;
	for (auto it = mm_CZone.begin(); it != mm_CZone.end(); it++)
	{
		MSConstructionZone* pUrCZone = it->second;
		if (pUrCZone->m_bDefaultCZone == true)
			continue;

		if (pUrCZone->GetZoneType() != pZone->GetZoneType())	continue;
		if (pUrCZone->OverLap(pZone->GetProfile()) == false)	continue;

		pUrCZone->StartEdit(CmdProcess::CommandUpdateElementType::ConstructionZone, it->first, this->m_ID, false);
		OverlapCZoneArr.push_back(pUrCZone);
	}


	// CZone Number 중복 체크 중복이 있으면 마지막 Number +1 한다.
	SetNumberAndNameWhenDuplicate(pZone);

	if (pZone->m_bDefaultCZone == false)
		pZone->mp_PolylineInfo->TrimExistPolyLineObject();

	mm_CZone.insert(make_pair(pZone->m_ID, pZone));

	for (int i = 0; i < OverlapCZoneArr.size(); i++)
	{
		MSConstructionZone* pUrCZone = OverlapCZoneArr[i];
		pUrCZone->EndEdit_SkipTrimAgain();
	}
}

// CZone Number 중복 체크 중복이 있으면 마지막 Number +1 한다.
void MSFloor::SetNumberAndNameWhenDuplicate(MSConstructionZone* pZone)
{
	bool bHasSameCZoneNumber = false;
	int nMaxCZoneNum = 0;

	for (map<long, MSConstructionZone*>::iterator itr = mm_CZone.begin(); itr != mm_CZone.end(); ++itr)
	{
		MSConstructionZone* pCurZone = itr->second;
		int nCurZoneNumber = pCurZone->m_bDefaultCZone ? 0 : pCurZone->GetNumber();
		if (pCurZone->GetZoneType() != pZone->GetZoneType())
			continue;
		if (nCurZoneNumber == pZone->GetNumber())
			bHasSameCZoneNumber = true;

		nMaxCZoneNum = max(nMaxCZoneNum, nCurZoneNumber);
	}

	if (bHasSameCZoneNumber)
		pZone->SetNumber(nMaxCZoneNum + 1);
	//////////////////////////////////////////////////////////////////////////////
	// Name Duplicate
	CString orgName = pZone->GetName();
	int idx = 1;
	while (true)
	{
		bool bExistSameName = false;
		for (map<long, MSConstructionZone*>::iterator itr = mm_CZone.begin(); itr != mm_CZone.end(); ++itr)
		{
			MSConstructionZone* pCurZone = itr->second;
			//if (pCurZone->GetZoneType() != pZone->GetZoneType())	continue;
			if (pCurZone->GetName() == pZone->GetName())
			{
				bExistSameName = true;
				CString newName; newName.Format(_T("%s_%d"), orgName, idx);
				pZone->SetName(newName);
				break;
			}
		}

		++idx;
		if (!bExistSameName)
			break;
	}

}

MSConstructionZone* MSFloor::GetFirstCZone(bool IsHorizontalZone)
{
	MSConstructionZone* pFirstCZone = NULL;

	long nMinNO = 0;
	map<long, MSConstructionZone*>::iterator it;
	for (it = mm_CZone.begin(); it != mm_CZone.end(); it++)
	{
		MSConstructionZone* pZone = it->second;
		if (pZone == NULL)	continue;
		bool isZoneType = pZone->GetZoneType() == MSConstructionZone::Horizontal ? true : false;
		if (IsHorizontalZone != isZoneType)	continue;
		if (nMinNO == 0)
		{
			nMinNO = pZone->GetNumber();
			pFirstCZone = pZone;
			continue;
		}
		if (nMinNO > pZone->GetNumber())
		{
			nMinNO = pZone->GetNumber();
			pFirstCZone = pZone;
		}
	}
	return pFirstCZone;
}

MSConstructionZone* MSFloor::GetLastCZone(bool IsHorizontalZone)
{
	MSConstructionZone* pLastCZone = NULL;

	long nMaxNO = 0;
	map<long, MSConstructionZone*>::iterator it;
	for (it = mm_CZone.begin(); it != mm_CZone.end(); it++)
	{
		MSConstructionZone* pZone = it->second;
		if (pZone == NULL)	continue;
		bool isZoneType = pZone->GetZoneType() == MSConstructionZone::Horizontal ? true : false;
		if (IsHorizontalZone != isZoneType)	continue;
		if (nMaxNO <= pZone->GetNumber())
		{
			nMaxNO = pZone->GetNumber();
			pLastCZone = pZone;
		}
	}
	return pLastCZone;
}

MSConstructionZone* MSFloor::GetCZone(long nID)
{
	auto it = mm_CZone.find(nID);
	if (it == mm_CZone.end())
	{
		MSBaseBuilding* pBldg = GetBuilding();
		if (pBldg == nullptr) 
			return nullptr;
		vector<MSFloor*> floorArr = pBldg->GetAllFloors();
		for (int idx = 0 ; idx < floorArr.size() ; idx++)
		{
			MSFloor* pFloor = floorArr[idx];
			if (pFloor == nullptr || pFloor == this) continue;

			auto curit = pFloor->mm_CZone.find(nID);
			if (curit != pFloor->mm_CZone.end() && curit->second != nullptr)
				return curit->second;
		}
		return nullptr;
	}
	return it->second;
}

MSConstructionZone* MSFloor::GetFloorCZone(long nID)
{
	map<long, MSConstructionZone*>::iterator it = mm_CZone.find(nID);
	if (it == mm_CZone.end())	return NULL;
	return it->second;
}

void MSFloor::GetCZone(vector<int>& czone_id_list, vector<MSConstructionZone*>& czone_list)
{
	czone_list.clear();
	for (int i = 0; i < czone_id_list.size(); ++i)
	{
		MSConstructionZone* zone = GetCZone(czone_id_list[i]);
		if (zone != NULL) czone_list.push_back(zone);
	}
}

MSConstructionZone* MSFloor::GetCZoneDefault(bool IsHorizontalZone)
{
	for (map<long, MSConstructionZone*>::iterator it = mm_CZone.begin(); it != mm_CZone.end(); ++it)
	{
		bool isZoneType = it->second->GetZoneType() == MSConstructionZone::Horizontal ? true : false;
		if (IsHorizontalZone != isZoneType)	continue;
		if (it->second->m_bDefaultCZone) return it->second;
	}
	return NULL;
}

void MSFloor::GetAllCZone(vector<MSConstructionZone*>& czone_list, bool IsHorizontalZone, bool bWithDefault)
{
	map<long, MSConstructionZone*>::iterator it;
	for (it = mm_CZone.begin(); it != mm_CZone.end(); ++it)
	{
		bool isZoneType = it->second->GetZoneType() == MSConstructionZone::Horizontal ? true : false;
		if (IsHorizontalZone != isZoneType)	continue;
		if (bWithDefault == false)
		{
			if (it->second->m_bDefaultCZone) continue;
		}
		czone_list.push_back(it->second);
	}
}

void MSFloor::RemoveCZone(MSConstructionZone* pZone)
{
	long nID = pZone->m_ID;
	map<long, MSConstructionZone*>::iterator it = mm_CZone.find(nID);
	if (it == mm_CZone.end())	return;
	if (it->second->m_bDefaultCZone) return; // mylee
	//delete pZone; // for undo.. etc

	pZone->InitializeParameter();

	mm_CZone.erase(it);
}
void MSFloor::RemoveCZone(long nID)
{
	map<long, MSConstructionZone*>::iterator it = mm_CZone.find(nID);
	if (it == mm_CZone.end())	return;
	if (it->second->m_bDefaultCZone) return; // mylee
	//delete it->second; // for undo.. etc

	it->second->InitializeParameter();

	mm_CZone.erase(it);
}
void MSFloor::DeleteCZoneMaps()
{
	map<long, MSConstructionZone*>::iterator it;
	for (it = mm_CZone.begin(); it != mm_CZone.end(); it++)
		delete it->second;
	mm_CZone.clear();
}

void MSFloor::Serialize(CArchive &ar)
{
	m_bIsSerializing = true;

	if (ar.IsStoring())
		CheckMSModel();

	long NumPoint = 0;
	long NumBulge = 0;
	if (ar.IsStoring())
	{
		ar << m_nFloorNum;
		ar << m_dHeight;

		int CZoneCount = 0;
		map<long, MSConstructionZone*>::iterator itCZone;
		for (itCZone = mm_CZone.begin(); itCZone != mm_CZone.end(); itCZone++)
		{
			MSConstructionZone* pCZone = itCZone->second;
			if (pCZone != NULL) ++CZoneCount;
		}
		ar << CZoneCount;

		for (itCZone = mm_CZone.begin(); itCZone != mm_CZone.end(); itCZone++)
		{
			MSConstructionZone* pCZone = itCZone->second;
			if (pCZone == NULL)	continue;
			pCZone->Serialize(ar);
		}

		ar << m_nPlaceStory;

		int CloneCount = ma_Clone.size();
		ar << CloneCount;
		vector<MSFloor*>::iterator it;
		for (it = ma_Clone.begin(); it != ma_Clone.end(); it++)
		{
			MSFloor* pClone = *it;
			ar << pClone->m_ID;
		}

		if (mp_Source)
			ar << mp_Source->m_ID;
		else
			ar << 0;

		int RebarCount = mm_Rebar.size();
		ar << RebarCount;
		map<long, MSSlabRebar*>::iterator itRebar;
		for (itRebar = mm_Rebar.begin(); itRebar != mm_Rebar.end(); itRebar++)
		{
			MSSlabRebar* pRebar = itRebar->second;
			pRebar->Serialize(ar);
		}

		int GridLineCount = mm_GridLine.size();
		ar << GridLineCount;
		map<long, MSGridLine*>::iterator itGrid;
		for (itGrid = mm_GridLine.begin(); itGrid != mm_GridLine.end(); itGrid++)
		{
			MSGridLine* pGrid = itGrid->second;
			pGrid->Serialize(ar);
		}

		ar << m_SlabLevelOffset;

		ar << m_nRebarTemplateID;
		ar << m_nFormWorkTemplateID;

		int nPile = mm_Pile.size();
		ar << nPile;
		for (auto it = mm_Pile.begin(); it != mm_Pile.end(); it++)
			it->second->Serialize(ar);

		ar << m_FloorType;
		ar << m_FloorClassification;
		int tmp = m_ePlacementDirectionType;
		ar << tmp;
	}
	else
	{
		//	double dBulge;
		ar >> m_nFloorNum;
		ar >> m_dHeight;

		int NumCZone = 0;
		ar >> NumCZone;
		for (long iZone = 0; iZone < NumCZone; iZone++)
		{
			MSConstructionZone* pCZone = new MSConstructionZone;
			pCZone->mp_Owner = this;
			pCZone->Serialize(ar);
			if ((pCZone->m_bDefaultCZone == false && pCZone->GetProfile()->CheckPolygon() == false)
				|| (m_dHeight == 0 && pCZone->m_bDefaultCZone == true && pCZone->GetZoneType() == MSConstructionZone::Horizontal))
				delete pCZone;
			else
				mm_CZone.insert(make_pair(pCZone->m_ID, pCZone));
			//AddCZone(pCZone);
		}

		// 잘못 만들어진 CZone의 오프닝을 삭제한다.
		for (auto it = mm_CZone.begin(); it != mm_CZone.end(); it++)
		{
			MSConstructionZone* pCZone = it->second;
			if (pCZone == nullptr)
				continue;
			pCZone->RemoveDumpOpening();
		}

		// CZone Serialize완료후 디폴트 시존이 없는 경우 다시 만들어 준다.
		AddDefaultCZone();

		ar >> m_nPlaceStory;

		int CloneCount = 0;
		ar >> CloneCount;
		for (int iClone = 0; iClone < CloneCount; iClone++)
		{
			int CloneOwnerID = 0;
			int CloneID = 0;
			ar >> CloneID;

			tmep_CloneMap.insert(make_pair(CloneID, CloneID));
		}
		ar >> tmep_SourceID;

		if (MSVersionInfo::GetCurrentVersion() >= 20140606)
		{
			int RebarCount = 0;
			ar >> RebarCount;
			for (int iRebar = 0; iRebar < RebarCount; iRebar++)
			{
				MSSlabRebar* pRebar = new MSSlabRebar;
				pRebar->SetFloor(this);
				pRebar->Serialize(ar);
				if (pRebar->IsValidProfile())
					mm_Rebar.insert(make_pair(pRebar->m_ID, pRebar));
			}
		}

		// JOINT_SERIALIZE
		if (MSVersionInfo::GetCurrentVersion() >= 20141003 && MSVersionInfo::GetCurrentVersion() < 20150107)
		{
			int count = 0;
			ar >> count;
			for (int i = 0; i < count; ++i)
			{
				MSConnectivityDummy tmp;
				tmp.Serialize(ar);
				// 				if (MSConnectivity::JOINT_SERIALIZE())
				// 				{
				// 					MSConnectivity* pConn = new MSConnectivity();
				// 					pConn->Serialize(ar);
				// 					mm_Joint[pConn->m_ID] = pConn;
				// 					//mp_QT->AddToLeaves(pConn, true, true);
				// 				}
				// 				else
				// 				{
				// 					MSConnectivity tmp;
				// 					tmp.Serialize(ar);
				// 				}
			}
		}

		if ((IsBHForBHAMode() && (MSVersionInfo::GetCurrentVersion() >= 20150312 && MSVersionInfo::GetCurrentVersion() < 20240307))
			|| (IsBHQorBHCEMode() && ((MSVersionInfo::GetCurrentVersion() >= 20150312 && MSVersionInfo::GetCurrentVersion() < 20200728)))
		)
		{
			RebarQuantityInfo rebarQuantityInfo;
			rebarQuantityInfo.Serialize(ar);

			MSRebarBuilding* pRebarBuilding = dynamic_cast<MSRebarBuilding*>(GetBuilding());
			if (pRebarBuilding != nullptr)
			{
				for (int index = 0; index < rebarQuantityInfo.m_aActualQuantity.size(); index++)
				{
					RebarQuantity copyRebarQuantity = rebarQuantityInfo.m_aActualQuantity[index];
					pRebarBuilding->m_rebarQuantityInfo.m_aActualQuantity.push_back(copyRebarQuantity);
				}
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20150619)
		{
			int GridLineCount = 0;
			ar >> GridLineCount;
			for (int i = 0; i < GridLineCount; i++)
			{
				MSGridLine* pGrid = new MSGridLine;
				pGrid->Serialize(ar);
				pGrid->m_pOwner = this;
				mm_GridLine.insert(make_pair(pGrid->m_ID, pGrid));
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20150825)
			ar >> m_SlabLevelOffset;

		if (MSVersionInfo::GetCurrentVersion() >= 20170715)
		{
			ar >> m_nRebarTemplateID;
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20180622)
		{
			ar >> m_nFormWorkTemplateID;
		}

		if (IsBHForBHAMode() && MSVersionInfo::GetCurrentVersion() >= 20220824 && MSVersionInfo::GetCurrentVersion() < 20240307)
		{
			int value = 0;
			ar >> value;
			//m_floorClassify = (eMSFloorClassify)value;
			ar >> value;
			//m_floorType = (eMSFloorType)value;
		}
		SetDefFloorTypeNClassification();
		if (IsBHCEMode()
			|| ((IsBHQMode() || IsBHAMode() || IsBHFMode()) && (MSVersionInfo::GetCurrentVersion() >= 20240425) || MSVersionInfo::IsOldTotalVersion()))
		{
			int nPile = 0;
			ar >> nPile;
			for (int i = 0; i < nPile; i++)
			{
				MSPile* pile = CreatePile();
				pile->Serialize(ar);
				Add(pile);
			}
		}

		if(IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231010)
		{
			ar >> m_FloorType;
			ar >> m_FloorClassification;
			int tmp = 0;
			ar >> tmp;
			m_ePlacementDirectionType = (PlacementDirectionType)tmp;
		}
	}

	MSAssembly::Serialize(ar);

	m_bIsSerializing = false;

	if (!ar.IsStoring())
	{
		if (MSVersionInfo::GetCurrentVersion() < 20140807)
		{
			vector<MSCompoG*> SlabArr;
			GetMSCompoGArr(MSElement::msSlabG, SlabArr);
			long NumSlab = SlabArr.size();
			for (long iSlab = 0; iSlab < NumSlab; iSlab++)
			{
				MSSlabG* pSlabG = (MSSlabG*)SlabArr[iSlab];
				if (pSlabG->mp_PolylineInfo->ma_Opening.size() > 0)
					pSlabG->mp_PolylineInfo->TrimExistPolyLineObject();
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20141007 && MSVersionInfo::GetCurrentVersion() < 20150210)
		{
			for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
			{
				MSCompoG* pCompopG = itr->second;
				pCompopG->UpdateBoundaryVisibilities();
			}
		}		
	}
}

void MSFloor::RecoverPointerAfterSerialize()
{
	for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
	{
		long ID = itr->first;
		itr->second->RecoverPointerAfterSerialize((MSFloor*)this);
	}
}

void MSFloor::FixCompoGInfoAfterSerialize()
{
	// MSVersionInfo::GetCurrentVersion() 와 함께 사용할것

	if (MSVersionInfo::GetCurrentVersion() < 20160324)
	{
		for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
			itr->second->CalculateConnectionsAngle();
	}

	// [BH-1358] m_dInnerDeg 음수인 경우 CalculateContainedAngle(); 호출 하여 다시 계산해야함
	if (MSVersionInfo::GetCurrentVersion() < 20161121)
	{
		for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
			itr->second->FixCalculateConnectionsAngle();
	}

	// [BH-1685] [KD건설] 보BOX도면 내보내기시 정착선 찾기 에러
	// 덧살보 작업 사이드 이펙트 LineTR계산 버그
	// [BH-1696] MSCompoG의 mp_TrimmedBoundary, ma_TrimmedBoundaryVisible의 갯수 불일치 문제
	if (MSVersionInfo::GetCurrentVersion() >= 20160809 && MSVersionInfo::GetCurrentVersion() < 20161028)
	{
		for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
		{
			if (itr->second->GetType() == MSElement::msBeamG)
				((MSBeamG*)itr->second)->SetTrimmedLines();
		}
	}

	// [BH-1813] 짧은 부재 + 묻힘 부재일 경우 CheckIntersectLinkMember() 다시 호출 LocTol 버그 수정
	if (MSVersionInfo::GetCurrentVersion() < 20161121)
		for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
		{
			if (itr->second->IsLinkMember() == true)
			{
				MSLinkMemberG* pLink = (MSLinkMemberG*)itr->second;
				if (pLink->GetMaxPartnerWidth() > pLink->GetLength())
					pLink->CalculateConnectionsAngle();
			}
		}

	//[BH-2364] 난간벽 Serialize 버그 수정
	if (MSVersionInfo::GetCurrentVersion() < 20170303)
	{
		bool isUpdate = false;
		for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
		{
			if (itr->second->GetType() == MSElement::msHandRailG)
			{
				MSHandRailG* pHandRailG = (MSHandRailG*)itr->second;
				pHandRailG->SetLines();
			}
		}

		for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
		{
			if (itr->second->GetType() == MSElement::msHandRailG)
			{
				MSHandRailG* pHandRailG = (MSHandRailG*)itr->second;
				pHandRailG->UpdateTrimmedBoundary();
				pHandRailG->UpdateBoundaryVisibilities();
				if (pHandRailG->HasParaPet() == true)
					isUpdate = true;
			}
		}

		if (isUpdate == true)
			GetBuilding()->m_bIsNeedRegenAfterLoad = true;
	}

	//[BH-2473],Connection Pos 계산 로직 수정 짧은 부재인경우 Mid 처리 로직 추가 [BH-3791] 라인부재의 Connection 각도 재계산(거의 평행하면서 부재가 짧은경우 버그 수정)
	if (MSVersionInfo::GetCurrentVersion() < 20180619)
	{
		for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
		{
			if (itr->second->IsLinkMember())
			{
				MSLinkMemberG* pLinkG = (MSLinkMemberG*)itr->second;
				pLinkG->CalculateConnectionsAngle();
			}
		}
	}

	//[BH-2518] 슬래브 개선 작업
	if (MSVersionInfo::GetCurrentVersion() < 20180511)
	{
		for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
		{
			if (itr->second->GetType() == MSElement::msSlabG)
			{
				MSSlabG* pSlabG = (MSSlabG*)itr->second;
				FindOverlapMembersNCalcTrimOption(pSlabG);
				pSlabG->UpdateTrimmedBoundary();
				pSlabG->RemoveSameConnection();
			}
		}
	}

	//[BH-3730] GM2DPolyline::CheckAndFixFoldedEdges BugFix, 계단만 자동 업데이트로 처리
	if (MSVersionInfo::GetCurrentVersion() < 20180515)
	{
		for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
		{
			if (itr->second->GetType() == MSElement::msStairG)
			{
				MSGenStairG* pStairG = (MSGenStairG*)itr->second;
				pStairG->FixCompoGInfoAfterSerialize();
			}
		}
	}

	//[BH-3971] [대림] 특정슬래브의 기본수정작업 작동하지 않는 오류
	if (MSVersionInfo::GetCurrentVersion() >= 20180426 && MSVersionInfo::GetCurrentVersion() < 20180824)
	{
		for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
		{
			if (itr->second->GetType() == msSlabG)
			{
				MSCompoG* pCompoG = (MSCompoG*)itr->second;
				GM2DPolyline* pPoly = pCompoG->GetProfile();
				bool isDeleted = pPoly->DeleteDuplicatedPoints(DTOL_MODELING);
				if (isDeleted)
				{
					//MSSlabG::SetProfile() 와 유사하게 구현, 
					//mp_PolylineInfo->SetProfile()함수의 pSlab->ClearSlopeInfo();호출 안하도록 처리
					if (pCompoG->mp_PolylineInfo->mp_Profile != pPoly)
						pCompoG->mp_PolylineInfo->mp_Profile->CopyFrom(pPoly);
					pCompoG->UpdateBoundaryVisibilities();
					pCompoG->SetTrimmedBoundary_Acis(pPoly);
					pCompoG->InitTrimmedBoundary(true);
					pCompoG->InitTrimmedBoundary(false);
				}
			}
		}
	}

	//[BH-4535] [SK건설] 경사 라인부재 트림 로직 수정
	// 경사 부재인 경우에만 트림 바운더리 다시 계산
	if (MSVersionInfo::GetCurrentVersion() < 20190906)
	{
		for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
		{
			if (itr->second->IsSlopeMember() == true)
			{
				MSCompoG* pCompoG = (MSCompoG*)itr->second;
				if (pCompoG->IsLinkMember() == true)
					pCompoG->UpdateBoundaryVisibilities();
			}
		}
	}

	if (MSVersionInfo::GetCurrentVersion() < 20191113)
	{
		for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
		{
			if (itr->second->GetType() == Type_MSCompoG::msSlabG)
			{
				MSSlabG* pSlabG = dynamic_cast<MSSlabG*>(itr->second);
				if (pSlabG != nullptr && pSlabG->IsIMTPCSlab() == true)
					pSlabG->UpdateInnerProfile();
			}
		}
	}
	if (MSVersionInfo::GetCurrentVersion() < 20221011)
	{
		for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
		{
			if (itr->second->GetType() == Type_MSCompoG::msSlabG)
			{
				MSSlabG* pSlabG = dynamic_cast<MSSlabG*>(itr->second);
				if (pSlabG != nullptr && pSlabG->IsHCSPCSlab() == true)
					pSlabG->UpdateInnerProfile();
			}
		}
	}

	if (MSVersionInfo::GetCurrentVersion() < 20230209)
	{
		for (auto itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
		{
			if (itr->second->GetType() == Type_MSCompoG::msWallG)
			{
				MSWallMemberG* pWallG = dynamic_cast<MSWallMemberG*>(itr->second);
				if (pWallG != nullptr && pWallG->mm_WallOpening.size() > 0)
					pWallG->InitTrimmedBoundary();
			}
		}
	}

	// [Q23-318] #comment 기둥 Connection 조건 변경
	if (MSVersionInfo::GetCurrentVersion() < 20230821)
	{
		for (auto itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
		{
			if (itr->second->GetType() == Type_MSCompoG::msColumnG)
			{
				MSColumnG* pColumnG = dynamic_cast<MSColumnG*>(itr->second);
				if (pColumnG != nullptr)
				{
					vector< MSConnection*> removeConnArr;
					for (int idx = 0; idx < pColumnG->ma_Connections.size(); idx++)
					{
						MSConnection* pConn = pColumnG->ma_Connections[idx];
						MSCompoG* pPartnerG = pConn->GetPartner(pColumnG);
						if (pPartnerG->GetType() != MSElement::msColumnG)
							continue;

						if (pColumnG->GetProfile()->OverlapEx(*pPartnerG->GetProfile(), MSFloor::ms_DefLineHitDistTol, false) == false)
						{
							removeConnArr.push_back(pConn);
							pPartnerG->RemoveConnection(pConn);
						}
					}

					for (int idx = 0; idx < removeConnArr.size(); idx++)
					{
						MSConnection* pConn = removeConnArr[idx];
						pColumnG->RemoveConnection(pConn);
						bool isRemove = CmdProcessHelper::GetHistoryManager()->RemoveHistoryObject(pConn);
						delete removeConnArr[idx];
					}
				}
			}
		}
	}

	if (MSVersionInfo::GetCurrentVersion() < 20231229)
	{
		UpdateSlabInnerProfile();
	}

	// [Q24-1361] 거푸집 미산출/ 평행한 라인부재의 Connection 정보를 다시 계산해줘야 한다.
	for (auto itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
	{
		MSCompoG* pCompoG = itr->second;
		for (int idx = 0; idx < pCompoG->ma_Connections.size(); idx++)
		{
			MSConnection* pConn = pCompoG->ma_Connections[idx];
			if (pConn == nullptr) continue;
			
			if (pConn->IsParallel())
				pConn->CalculateContainedAngle();
		}
	}	

	for (auto itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
	{
		MSElement* pMSElement = dynamic_cast<MSElement*>(itr->second);
		pMSElement->FixCompoGInfoAfterSerialize();
	}	

	//코드 추가시 MSBuilding::FixCompoGInfoAfterSerialize() 함수의 마지막 버전 체크 같이 변경해야 함
}

void MSFloor::UpdateSlabInnerProfile()
{
	for (auto itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
	{
		if (itr->second->GetType() == Type_MSCompoG::msSlabG)
		{
			MSSlabG* pSlab = dynamic_cast<MSSlabG*>(itr->second);
			pSlab->UpdateInnerProfile();
		}
	}
}

bool MSFloor::IsCloneMode()
{
	return ms_bCloneMode;

}

void MSFloor::Dump(EFS::EFS_Buffer& buffer)
{
	MSAssembly::Dump(buffer);
	buffer << &mp_Source << &mp_QT << m_nFloorNum << m_dHeight << m_nPlaceStory;
	buffer << m_SlabLevelOffset;
	buffer << m_nRebarTemplateID << m_nFormWorkTemplateID;

	buffer << m_FloorType << m_FloorClassification;

	int tmp = m_ePlacementDirectionType;
	buffer << tmp;
}

void MSFloor::Recovery(EFS::EFS_Buffer& buffer)
{
	MSAssembly::Recovery(buffer);
	buffer >> &mp_Source >> &mp_QT >> m_nFloorNum >> m_dHeight >> m_nPlaceStory;
	buffer >> m_SlabLevelOffset;
	buffer >> m_nRebarTemplateID >> m_nFormWorkTemplateID;

	buffer >> m_FloorType >> m_FloorClassification;

	int tmp = 0;
	buffer >> tmp;
	m_ePlacementDirectionType = (PlacementDirectionType)tmp;
}

double MSFloor::GetZLevel(double Xc, double Yc)
{
	double FloorLevel = GetSlabLevel();
	long nZone = ma_LevelZone.size();
	for (long iZone = 0; iZone < nZone; iZone++)
	{
		MSLevelZone* pZone = ma_LevelZone[iZone];
		if (pZone->Contains(Xc, Yc, 0.01) < 1)
			return pZone->m_Level + FloorLevel;
	}
	return FloorLevel;
}

vector<int>& MSFloor::CompoGIDList_NeedRegen()
{
	static vector<int> compoGList;
	return compoGList;
}

vector<int>& MSFloor::CompoGIDList_NeedRegen_2DView()
{
	static vector<int> compoGList;
	return compoGList;
}

vector<int>& MSFloor::CompoGIDList_NeedRegen_CurTR()
{
	static vector<int> compoGList;
	return compoGList;
}

vector<int>& MSFloor::CompoGIDList_NeedRegen_2DView_CurTR()
{
	static vector<int> compoGList;
	return compoGList;
}

void MSFloor::FindCenterLinesForFindSlab_ConnectParallelLines(vector<GM2DCurve*>& lines, double ECC_TOL, double TOL)
{
	/////////////////////////////////////////////////////////////////////
	// Line 통합처리 - 동일 각도, 선상에 있는 직선들을 통합해준다.
	vector<double> angles;
	for (int i = 0; i < lines.size(); ++i)
	{
		double dx = lines[i]->GetP1().m_X - lines[i]->GetP0().m_X;
		double dy = lines[i]->GetP1().m_Y - lines[i]->GetP0().m_Y;
		double rad = atan2(dy, dx);
		double ang = rad / PI * 180.0;
		if (ang < 0.0) ang += 360.0;
		if (ang >= 180.0) ang -= 180.0;
		angles.push_back(ang);
	}

	map<int, bool> map_delete;
	for (int i = 0; i < lines.size(); ++i)
	{
		if (map_delete.count(i) > 0) continue;
		double cur_ang = angles[i];
		GM2DVector p1 = lines[i]->GetP0();
		GM2DVector p2 = lines[i]->GetP1();
		double leng = p2.Distance(p1);
		double uvec[2] = { (p2.m_X - p1.m_X) / leng, (p2.m_Y - p1.m_Y) / leng };
		vector<double> r1_list, r2_list;
		vector<int> idx_list;
		for (int k = i + 1; k < lines.size(); ++k)
		{
			if (map_delete.count(k) > 0) continue;
			if (fabs(angles[k] - cur_ang) < 1.0 || fabs(angles[k] - cur_ang) > 179.0) {}
			else continue;
			double dist1 = 0.0; double h1 = 0.0;
			FindCenterLinesForFindSlab_CalcProjection(p1, uvec, lines[k]->GetP0(), dist1, h1);
			if (h1 > ECC_TOL) continue;
			double dist2 = 0.0; double h2 = 0.0;
			FindCenterLinesForFindSlab_CalcProjection(p1, uvec, lines[k]->GetP1(), dist2, h2);
			if (h2 > ECC_TOL) continue;
			if (dist2 < dist1) { double tmp = dist1; dist1 = dist2; dist2 = tmp; }
			r1_list.push_back(dist1);
			r2_list.push_back(dist2);
			idx_list.push_back(k);
		}
		double cur_r1 = 0.0;
		double cur_r2 = leng;

		// idx_list 돌면서 통합처리 (cur_r1, cur_r2, map_delete)
		int max_loop = idx_list.size();
		for (int k = 0; k < max_loop; ++k) // 겹쳐진 직선 (TOL 안에 있는 직선) 을 이어준다.
		{
			int processed_num = 0;
			for (int m = 0; m < idx_list.size(); ++m)
			{
				int idx = idx_list[m];
				if (map_delete.count(idx) > 0) continue;
				double r1 = r1_list[m];
				double r2 = r2_list[m];
				if (r2 < cur_r1 - TOL) continue;
				if (r1 > cur_r2 + TOL) continue;
				if (r1 < cur_r1) cur_r1 = r1;
				if (r2 > cur_r2) cur_r2 = r2;
				processed_num++;
				map_delete[idx] = true;
			}
			if (processed_num == 0) break; // 0 개이면 통과
		}

		// 모든 직선을 여유분으로 10 만큼 더 뺀다
		cur_r1 -= 10.0;
		cur_r2 += 10.0;
		// cur_r1, cur_r2 로 변경
		GM2DVector p1New(p1.m_X + cur_r1 * uvec[0], p1.m_Y + cur_r1 * uvec[1]);
		GM2DVector p2New(p1.m_X + cur_r2 * uvec[0], p1.m_Y + cur_r2 * uvec[1]);
		lines[i]->SetP0(p1New);
		lines[i]->SetP1(p2New);
	}

	/////////////////////////////////////////////////////////////////////
	// process deleted
	for (int i = lines.size() - 1; i >= 0; --i)
	{
		if (map_delete.count(i) > 0)
			lines.erase(lines.begin() + i);
	}
}

void MSFloor::FindCenterLinesForFindSlab(vector<GM2DPolyline*>& vProfile_in, vector<GM2DCurve*>& vCurve)
{
	double TOL = 500.0;
	double ECC_TOL = 110.0;
	double COLUMN_CLOSE = 1000.0;
	//double LINE_EXTEND = 500.0;
	vCurve.clear();

	//////////////////////////////////////////////////////////////////////////
	// Ignore Small Profiles
	vector<GM2DPolyline*> vProfile;
	for (int i = 0; i < vProfile_in.size(); ++i)
	{
		double min[2] = { 0 }; double max[2] = { 0 };
		for (int k = 0; k < vProfile_in[i]->ma_Point.GetSize() - 1; ++k)
		{
			double x = vProfile_in[i]->ma_Point[k]->m_X;
			double y = vProfile_in[i]->ma_Point[k]->m_Y;
			if (k == 0) { min[0] = max[0] = x; min[1] = max[1] = y; }
			if (x < min[0]) min[0] = x;
			else if (x > max[0]) max[0] = x;
			if (y < min[1]) min[1] = y;
			else if (y > max[1]) max[1] = y;
		}
		double dx = max[0] - min[0]; double dy = max[1] - min[1];
		if (dx + dy > 2000.0) vProfile.push_back(vProfile_in[i]);
	}

	vector<MSCompoG*> columns;
	for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
	{
		MSCompoG* compoG = itr->second;
		if (compoG->GetType() == MSElement::msColumnG)
			columns.push_back(compoG);
	}

	vector<MSCompoG*> floorG, beam_list;
	for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
	{
		MSCompoG* compoG = itr->second;
		if (compoG->GetType() == MSElement::msBeamG)
		{
			floorG.push_back(compoG);
			beam_list.push_back(compoG);
		}
	}
	for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
	{
		MSCompoG* compoG = itr->second;
		if (compoG->GetType() == MSElement::msWallG)
		{
			///////////////////////////////////////////////////////////////
			// 벽의 경우는 보와 평행하고, 겹칠경우 Skip
			MSWallMemberG* wall = (MSWallMemberG*)compoG;
			GM2DLineVector wallLine(*wall->GetSVec(), *wall->GetEVec());
			GM2DCurve* wall_curve = &wallLine;

			if (!wall->isArc())
			{
				bool overlap_exist = false;
				for (int i = 0; i < beam_list.size(); ++i)
				{
					MSBeamG* beam = (MSBeamG*)beam_list[i];
					GM2DLineVector beamLine(*beam->GetSVec(), *beam->GetEVec());
					GM2DCurve* beam_curve = &beamLine;
					if (!beam->isArc())
					{
						double dDegreeTol = 5.0;
						double dDistTol = (wall->GetWidth() + beam->GetWidth()) / 2.0;
						double WALL_BEAM_OVERLAP = 100.0;
						double beam_leng = beam_curve->Length();
						double dOverlapTol = fabs(WALL_BEAM_OVERLAP / beam_leng);
						// Parallel
						if (0 != beam_curve->IsColinear4DTol(wall_curve, dDegreeTol, dDistTol))
						{
							double loc1 = beam_curve->OrthoBaseLocValue(wall_curve->GetP0());
							double loc2 = beam_curve->OrthoBaseLocValue(wall_curve->GetP1());
							if ((dOverlapTol < loc1 && loc1 < 1.0 - dOverlapTol) || (dOverlapTol < loc2 && loc2 < 1.0 - dOverlapTol))
								overlap_exist = true;
						}
					}
				}
				if (overlap_exist) continue;
			}
			///////////////////////////////////////////////////////////////

			floorG.push_back(compoG);
		}
	}

	vector<GM2DCurve*> lines;
	vector<GM2DCurve*> lines_moved;
	vector<GM2DArc*> arcs;
	map<GM2DCurve*, MSCompoG*> mapLineToCompoG;
	for (int ii = 0; ii < floorG.size(); ++ii)
	{
		MSCompoG* compoG = floorG[ii];

		//if (compoG->GetType() == MSElement::msWallG || compoG->GetType() == MSElement::msBeamG)
		{
			MSLinkMemberG* link = dynamic_cast<MSLinkMemberG*>(compoG);
			if (link == NULL) continue;

			GM2DVector* p1 = (GM2DVector*)link->GetSVec()->Copy();
			GM2DVector* p2 = (GM2DVector*)link->GetEVec()->Copy();
			bool exist_left = false, exist_right = false;
			FindCenterLinesForFindSlab__FindLeftRightProfile(vProfile, p1, p2, exist_left, exist_right);
			bool use_left = (!exist_left && exist_right);
			bool use_right = (exist_left && !exist_right);

			if (link->isArc())
			{
				shared_ptr<GM2DCurve> pCenCurve = link->GetCenCurve();
				GM2DCurve* curve = pCenCurve->Copy();
				//vCurve.push_back(curve);

				arcs.push_back((GM2DArc*)curve);
				mapLineToCompoG[(GM2DArc*)curve] = compoG;
			}
			else
			{
				double leng = p2->Distance(*p1);
				double uvec[2] = { (p2->m_X - p1->m_X) / leng, (p2->m_Y - p1->m_Y) / leng };

				GM2DLineVector* line = NULL;
				if (use_left || use_right)
				{
					double uvec_left[2] = { -uvec[1], uvec[0] };
					if (use_right) { uvec_left[0] = -uvec_left[0]; uvec_left[1] = -uvec_left[1]; }
					double width = compoG->GetWidth() * 0.5;
					p1->m_X = p1->m_X + width * uvec_left[0]; p1->m_Y = p1->m_Y + width * uvec_left[1];
					p2->m_X = p2->m_X + width * uvec_left[0]; p2->m_Y = p2->m_Y + width * uvec_left[1];

					line = new GM2DLineVector(*p1, *p2);
					lines_moved.push_back(line);
					mapLineToCompoG[line] = compoG;
				}
				else
				{
					line = new GM2DLineVector(*p1, *p2);
					lines.push_back(line);
					mapLineToCompoG[line] = compoG;
				}


				/////////////////////////////////////////////////////////////////////////////
				// Extend to close columns
				for (int i = 0; i < columns.size(); ++i)
				{
					if (columns[i]->ma_Vec.size() == 0) continue;
					double x = columns[i]->ma_Vec[0]->m_X;
					double y = columns[i]->ma_Vec[0]->m_Y;
					if ((fabs(p1->m_X - x) + fabs(p1->m_Y - y)) < COLUMN_CLOSE)
					{
						double rDist = 0.0; double h = 0.0;
						FindCenterLinesForFindSlab_CalcProjection(*p1, uvec, GM2DVector(x, y), rDist, h);
						if (rDist < 0) { GM2DVector p1New(p1->m_X + rDist * uvec[0], p1->m_Y + rDist * uvec[1]); line->SetP0(p1New); }
					}
					if ((fabs(p2->m_X - x) + fabs(p2->m_Y - y)) < COLUMN_CLOSE)
					{
						double rDist = 0.0; double h = 0.0;
						FindCenterLinesForFindSlab_CalcProjection(*p1, uvec, GM2DVector(x, y), rDist, h);
						if (rDist > leng) { GM2DVector p2New(p1->m_X + rDist * uvec[0], p1->m_Y + rDist * uvec[1]); line->SetP1(p2New); }
					}
				}
				/////////////////////////////////////////////////////////////////////////////
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Priority first - moved line
	lines.insert(lines.begin(), lines_moved.begin(), lines_moved.end());

	/////////////////////////////////////////////////////////////////////
	// Arc 통합처리
	double ARC_CEN_TOL = 500.0;
	double ARC_R_TOL = 110.0;
	for (int i = arcs.size() - 1; i >= 0; --i)
	{
		GM2DArc* arc1 = arcs[i];
		bool modified = false;
		for (int k = 0; k < i; ++k)
		{
			GM2DArc* arc2 = arcs[k];
			if (arc2->m_Center.Distance(arc1->m_Center) < ARC_CEN_TOL)
			{
				if (fabs(arc2->GetRadius() - arc1->GetRadius()) < ARC_R_TOL)
				{
					double a1[2] = { arc1->m_Theta1, arc1->m_Theta2 };
					double a2[2] = { arc2->m_Theta1, arc2->m_Theta2 };
					bool overlap = false;
					double joined[2] = { 0 };
					OverlapAngle(a1, a2, overlap, joined);
					if (overlap)
					{
						modified = true;
						arc2->m_Theta1 = joined[0];
						arc2->m_Theta2 = joined[1];
						arc2->m_P0 = arc2->GetStartPoint();
						arc2->m_P1 = arc2->GetEndPoint();
						break;
					}
				}
			}
		}
		if (modified)
			arcs.erase(arcs.begin() + i);
	}

	/////////////////////////////////////////////////////////////////////
	// Line 통합처리 - 동일 각도, 선상에 있는 직선들을 통합해준다.
	FindCenterLinesForFindSlab_ConnectParallelLines(lines, ECC_TOL, TOL);

	/////////////////////////////////////////////////////////////////////
	// Extend Line - 수직이나 비스듬하게 가까이 있는 직선까지 연장해준다.
	// 수직으로 10도 내외에 있는 선은 1500 까지, 더 비스듬하게 만난 선은 500 떨어져 있는 것까지 붙여준다.
	double LINE_EXTEND_TOL_GLOBAL = 1500.0;
	double LINE_EXTEND_TOL2 = 500.0;
	vector<int> result1, result2;
	vector<double> result3;
	vector<GM2DVector> result4;
	for (int i = 0; i < lines.size(); ++i)
	{
		GM2DVector p1 = lines[i]->GetP0();
		GM2DVector p2 = lines[i]->GetP1();
		double leng = p2.Distance(p1);
		double uvec[2] = { (p2.m_X - p1.m_X) / leng, (p2.m_Y - p1.m_Y) / leng };
		vector<int> extended_idx;
		vector<int> extended_ij;
		for (int k = 0; k < lines.size(); ++k)
		{
			if (mapLineToCompoG[lines[i]]->GetType() != mapLineToCompoG[lines[k]]->GetType()) continue;
			if (i == k) continue;
			double LINE_EXTEND_TOL = __min(LINE_EXTEND_TOL_GLOBAL, mapLineToCompoG[lines[k]]->GetWidth()*3.0);
			double dist1 = 0.0; double h1 = 0.0;
			for (int kk = 0; kk < 2; ++kk)
			{
				GM2DVector cur_p1 = kk == 0 ? lines[k]->GetP0() : lines[k]->GetP1();
				GM2DVector cur_p2 = kk == 0 ? lines[k]->GetP1() : lines[k]->GetP0();
				FindCenterLinesForFindSlab_CalcProjection(p1, uvec, cur_p1, dist1, h1);
				bool in_tol1 = false;
				bool in_tol2 = false;
				if (fabs(h1) < LINE_EXTEND_TOL && (0 - LINE_EXTEND_TOL < dist1 && dist1 < leng + LINE_EXTEND_TOL))
					in_tol1 = true;
				if (fabs(h1) < LINE_EXTEND_TOL2 && (0 - LINE_EXTEND_TOL2 < dist1 && dist1 < leng + LINE_EXTEND_TOL2))
					in_tol2 = true;
				if (in_tol1 || in_tol2)
				{
					GM2DVector point_new(p1.m_X + dist1 * uvec[0], p1.m_Y + dist1 * uvec[1]);
					double cur_leng = cur_p2.Distance(cur_p1);
					double uvec_1[2] = { (cur_p1.m_X - cur_p2.m_X) / cur_leng, (cur_p1.m_Y - cur_p2.m_Y) / cur_leng };
					double leng_to_point = point_new.Distance(cur_p1);
					double uvec_2[2] = { (point_new.m_X - cur_p1.m_X) / leng_to_point, (point_new.m_Y - cur_p1.m_Y) / leng_to_point };

					double inner = uvec_1[0] * uvec_2[0] + uvec_1[1] * uvec_2[1];
					double TOL_THETA = 60.0 / 180.0 * PI;
					if (in_tol2 == false) TOL_THETA = 10.0 / 180.0 * PI;
					if (inner > cos(TOL_THETA))
					{
						double extend = h1 / inner + 10.0;
						GM2DVector newP0(cur_p1.m_X + extend * uvec_1[0], cur_p1.m_Y + extend * uvec_1[1]);
						//if (kk == 0)	lines[k]->SetP0(newP0);
						//else			lines[k]->SetP1(newP0);
						result1.push_back(k);
						result2.push_back(kk);
						result3.push_back(extend);
						result4.push_back(newP0);
					}
				}
			}
		}
	}

	/////////////////////////////////////////////////////////////////////
	// Extend Line To Arc
	for (int i = 0; i < arcs.size(); ++i)
	{
		for (int k = 0; k < lines.size(); ++k)
		{
			if (mapLineToCompoG[arcs[i]]->GetType() != mapLineToCompoG[lines[k]]->GetType()) continue;
			double LINE_EXTEND_TOL = __min(LINE_EXTEND_TOL_GLOBAL, mapLineToCompoG[lines[k]]->GetWidth()*3.0);
			double angle = 0.0; double h1 = 0.0;
			for (int kk = 0; kk < 2; ++kk)
			{
				GM2DVector cur_p1 = kk == 0 ? lines[k]->GetP0() : lines[k]->GetP1();
				GM2DVector cur_p2 = kk == 0 ? lines[k]->GetP1() : lines[k]->GetP0();
				FindCenterLinesForFindSlab_CalcProjection(arcs[i], cur_p1, angle, h1);
				bool in_tol1 = false;
				bool in_tol2 = false;
				double angle2 = angle;
				if (angle2 < arcs[i]->m_Theta1) angle2 += 2.0*PI;
				if (fabs(h1) < LINE_EXTEND_TOL && (arcs[i]->m_Theta1 <= angle2 && angle2 <= arcs[i]->m_Theta2))
					in_tol1 = true;
				if (fabs(h1) < LINE_EXTEND_TOL2 && (arcs[i]->m_Theta1 <= angle2 && angle2 <= arcs[i]->m_Theta2))
					in_tol2 = true;
				if (in_tol1 || in_tol2)
				{
					GM2DVector point_new = arcs[i]->PolarVector(angle);
					double cur_leng = cur_p2.Distance(cur_p1);
					double uvec_1[2] = { (cur_p1.m_X - cur_p2.m_X) / cur_leng, (cur_p1.m_Y - cur_p2.m_Y) / cur_leng };
					double leng_to_point = point_new.Distance(cur_p1);
					double uvec_2[2] = { (point_new.m_X - cur_p1.m_X) / leng_to_point, (point_new.m_Y - cur_p1.m_Y) / leng_to_point };

					double inner = uvec_1[0] * uvec_2[0] + uvec_1[1] * uvec_2[1];
					double TOL_THETA = 60.0 / 180.0 * PI;
					if (in_tol2 == false) TOL_THETA = 10.0 / 180.0 * PI;
					if (inner > cos(TOL_THETA))
					{
						double extend = fabs(h1) / inner + 10.0;
						GM2DVector newP0(cur_p1.m_X + extend * uvec_1[0], cur_p1.m_Y + extend * uvec_1[1]);
						//if (kk == 0)	lines[k]->SetP0(newP0);
						//else			lines[k]->SetP1(newP0);
						result1.push_back(k);
						result2.push_back(kk);
						result3.push_back(extend);
						result4.push_back(newP0);
					}
				}
			}
		}
	}

	// 가장 짧은 선까지만 연장
	/*
	for (int i=result1.size()-1 ; i>=0 ; --i)
	{
		bool exist = false;
		for (int k=0 ; k<i ; ++k)
		{
			if (result1[i]==result1[k] && result2[i]==result2[k] && result3[k] < result3[i])
			{
				exist = true;
				break;
			}
		}
		if (exist)
		{
			result1.erase(result1.begin() + i);
			result2.erase(result2.begin() + i);
			result3.erase(result3.begin() + i);
			result4.erase(result4.begin() + i);
		}
	}
	*/

	// 실제 연장은 여기서 실행
	for (int i = 0; i < result1.size(); ++i)
	{
		int idx = result1[i];
		if (result2[i] == 0)
			lines[idx]->SetP0(result4[i]);
		else
			lines[idx]->SetP1(result4[i]);
	}

	/////////////////////////////////////////////////////////////////////
	// Line 통합처리2 - 동일 각도, 선상에 있는 직선들을 통합해준다.
	FindCenterLinesForFindSlab_ConnectParallelLines(lines, ECC_TOL, 50);

	/////////////////////////////////////////////////////////////////////

	for (int i = 0; i < lines.size(); ++i)
	{
		vCurve.push_back(lines[i]);
	}
	for (int i = 0; i < arcs.size(); ++i)
	{
		vCurve.push_back(arcs[i]);
	}
}

void MSFloor::OverlapAngle(double a1[2], double a2[2], bool& overlap, double joined[2])
{
	overlap = false;
	bool in1 = IsAngleInside(a1[0], a2[0], a2[1]);
	bool in2 = IsAngleInside(a1[1], a2[0], a2[1]);
	if (in1 || in2)
	{
		overlap = true;
		if (in1 && in2) { joined[0] = a2[0]; joined[1] = a2[1]; }
		else if (in1 && !in2) { joined[0] = a2[0]; joined[1] = a1[1]; }
		else if (!in1 && in2) { joined[0] = a1[0]; joined[1] = a2[1]; }
	}
	else
	{
		if (IsAngleInside(a2[0], a1[0], a1[1]))
		{
			overlap = true;
			if (IsAngleInside(a2[1], a1[0], a1[1])) { joined[0] = a1[0]; joined[1] = a1[1]; }
			else { joined[0] = a1[0]; joined[1] = a2[1]; }
		}
	}
	if (joined[1] < joined[0]) joined[1] += 2.0*PI;
}

bool MSFloor::IsAngleInside(double a_test, double a1, double a2)
{
	if (a2 < a1) a2 += 2.0*PI;
	if (a_test < a1) a_test += 2.0*PI;
	else if (a_test > a1 + 2.0*PI) a_test -= 2.0*PI;
	if (a1 <= a_test && a_test <= a2) return true;
	return false;
}

void MSFloor::FindCenterLinesForFindSlab__FindLeftRightProfile(vector<GM2DPolyline*>& vProfile_in, GM2DVector* p1, GM2DVector* p2, bool& exist_left, bool& exist_right)
{
	if (vProfile_in.empty()) return;
	if (p1 == NULL || p2 == NULL) return;

	double TOL = 500.0;
	exist_left = false;
	exist_right = false;

	for (int i = 0; i < vProfile_in.size(); ++i)
	{
		if (exist_left && exist_right) break;
		for (int k = 0; k < vProfile_in[i]->ma_Point.GetSize() - 1; ++k)
		{
			if (exist_left && exist_right) break;
			GM2DVector* cur_p1 = vProfile_in[i]->ma_Point[k];
			GM2DVector* cur_p2 = vProfile_in[i]->ma_Point[k + 1];
			double leng = cur_p2->Distance((*cur_p1));
			double uvec[2] = { (cur_p2->m_X - cur_p1->m_X) / leng, (cur_p2->m_Y - cur_p1->m_Y) / leng };
			double dist1 = 0.0; double h1 = 0.0;
			FindCenterLinesForFindSlab_CalcProjection((*cur_p1), uvec, (*p1), dist1, h1);
			double dist2 = 0.0; double h2 = 0.0;
			FindCenterLinesForFindSlab_CalcProjection((*cur_p1), uvec, (*p2), dist2, h2);
			if (h1 <= TOL && h2 <= TOL)
			{
				bool out = (__max(dist1, dist2) < -TOL || __min(dist1, dist2) > leng + TOL);
				if (!out)
				{
					if (dist1 < dist2) exist_left = true;
					else exist_right = true;
				}
			}
		}
	}
}

void MSFloor::FindCenterLinesForFindSlab_CalcProjection(GM2DArc* arc_in, GM2DVector& point_in, double& angle, double& h)
{
	GM2DVector& cen = arc_in->m_Center;
	double r = arc_in->GetRadius();
	double dist_from_cen2 = point_in.Distance(cen);
	angle = atan2(point_in.m_Y - cen.m_Y, point_in.m_X - cen.m_X);
	if (angle < 0.0) angle += 2.0 * PI;
	h = r - dist_from_cen2;
}

void MSFloor::FindCenterLinesForFindSlab_CalcProjection(GM2DVector& p1, double uvec[], GM2DVector& point, double& dist, double& h)
{
	double vec_b[2] = { point.m_X - p1.m_X, point.m_Y - p1.m_Y };
	double b2 = vec_b[0] * vec_b[0] + vec_b[1] * vec_b[1];
	dist = uvec[0] * vec_b[0] + uvec[1] * vec_b[1]; // Proj Dist : 내적/a(==1)
	double h2 = b2 - dist * dist;
	if (h2 < 0.0) h2 = 0.0;
	h = sqrt(h2);
}

void MSFloor::Add(MSSlabRebar* pRebar)
{
	pair<map<long, MSSlabRebar*>::iterator, bool> bPair;
	bPair = mm_Rebar.insert(make_pair(pRebar->m_ID, pRebar));
}

bool MSFloor::Remove(MSSlabRebar* pRebar)
{
	long nID = pRebar->m_ID;
	map<long, MSSlabRebar*>::iterator itRebar = mm_Rebar.find(nID);
	if (itRebar == mm_Rebar.end())	return false;
	mm_Rebar.erase(itRebar);
	return true;
}

bool MSFloor::Delete(MSSlabRebar* pRebar)
{
	if (!Remove(pRebar))
		return false;
	delete pRebar;
	return true;
}

MSSlabRebar* MSFloor::GetRebar(long ID)
{
	map<long, MSSlabRebar*>::iterator it = mm_Rebar.find(ID);
	if (it == mm_Rebar.end())
	{
		return NULL;
	}
	return it->second;

}

void MSFloor::FindOverlapLinkMembers(GM2DCurve* LVec, map<long, MSLinkMemberG*>& SptMemberMap,
	long nMemberType, double DegreeTol, double dDistDtol, double LinkTestLength)
{
	map<long, MSLinkMemberG*>::iterator itWall;

	CTypedPtrArray<CObArray, MSFloorQT*> CrossQTArr;
	bool bResetFirst = true;
	if (LVec->m_ObjType == GT_ARC)
	{
		GM2DArc* pArc = (GM2DArc*)LVec;
		mp_QT->FindCrossQTs(pArc, CrossQTArr, bResetFirst);
	}
	else if (LVec->m_ObjType == GT_LINE)
	{
		GM2DLineVector* pLine = (GM2DLineVector*)LVec;
		mp_QT->FindCrossQTs(*pLine, CrossQTArr, bResetFirst);
	}
	long numQT = CrossQTArr.GetSize();
	double UrP0Loc = 0;
	double UrP1Loc = 0;
	for (long iQT = 0; iQT < numQT; iQT++)
	{
		MSFloorQT* pQT = CrossQTArr[iQT];
		if (nMemberType == MSCompoG::msWallG)
		{
			long numMem = pQT->ma_VerticalMem.GetSize();
			for (long iMem = 0; iMem < numMem; iMem++)
			{
				MSCompoG* pCompoG = pQT->ma_VerticalMem[iMem];
				if (pCompoG == NULL)	continue;
				if (pCompoG->GetType() != nMemberType) continue;
				itWall = SptMemberMap.find(pCompoG->m_ID);
				if (itWall != SptMemberMap.end())	continue;
				MSLinkMemberG* pUrWall = (MSLinkMemberG*)pCompoG;
				GM2DCurve* UrLVec = pUrWall->GetShortLVec();
				long res = LVec->Overlap4DTol(UrLVec, UrP0Loc, UrP1Loc, DegreeTol, dDistDtol);
				if (res == 1 || res == 2 || res == 6 || res == 7 || res == 19 || res == 20 || res == 24 || res == 25)
					continue;
				SptMemberMap.insert(make_pair(pUrWall->m_ID, pUrWall));

				delete UrLVec;
			}
		}
		else
		{
			long numMem = pQT->ma_FloorMem.GetSize();
			for (long iMem = 0; iMem < numMem; iMem++)
			{
				MSCompoG* pCompoG = pQT->ma_FloorMem[iMem];
				if (pCompoG == NULL)	continue;
				if (pCompoG->GetType() != nMemberType) continue;
				itWall = SptMemberMap.find(pCompoG->m_ID);
				if (itWall != SptMemberMap.end())	continue;
				MSLinkMemberG* pUrWall = (MSLinkMemberG*)pCompoG;
				GM2DCurve* UrLVec = pUrWall->GetShortLVec();
				long res = LVec->Overlap4DTol(UrLVec, UrP0Loc, UrP1Loc, DegreeTol, dDistDtol);
				if (res == 1 || res == 2 || res == 6 || res == 7 || res == 19 || res == 20 || res == 24 || res == 25)
					continue;
				SptMemberMap.insert(make_pair(pUrWall->m_ID, pUrWall));

				delete UrLVec;
			}
		}

	}
	// 	map<long,MSLinkMemberG*>::iterator itWall;
	// 
	// 	CTypedPtrArray<CObArray, MSFloorQT*> CrossQTArr;
	// 	bool bResetFirst = true;
	// 	mp_QT->FindCrossQTs(LVec,CrossQTArr,bResetFirst);
	// 	long numQT = CrossQTArr.GetSize();
	// 	for(long iQT = 0;iQT<numQT;iQT++)
	// 	{
	// 		MSFloorQT* pQT = CrossQTArr[iQT];
	// 		if(nMemberType == MSCompoG::msWallG)
	// 		{
	// 			long numMem = pQT->ma_VerticalMem.GetSize();
	// 			for(long iMem = 0;iMem<numMem;iMem++)
	// 			{
	// 				MSCompoG* pCompoG = pQT->ma_VerticalMem[iMem];
	// 				if(pCompoG == NULL)	continue;
	// 				if(pCompoG->GetType() != nMemberType) continue;
	// 				itWall = SptMemberMap.find(pCompoG->m_ID);
	// 				if(itWall != SptMemberMap.end())	continue;
	// 				MSLinkMemberG* pUrWall = (MSLinkMemberG*)pCompoG;
	// 				GM2DLineVector UrLVec = pUrWall->GetShortLVec();
	// 				long res = LVec.Overlap4DTol(UrLVec,DegreeTol,dDistDtol,LinkTestLength);
	// 				if(res==0||res==2||res==6||res==7||res==19||res==20||res==24||res==25)
	// 					continue;
	// 				SptMemberMap.insert(make_pair(pUrWall->m_ID, pUrWall));
	// 			}
	// 		}
	// 		else
	// 		{
	// 			long numMem = pQT->ma_FloorMem.GetSize();
	// 			for(long iMem = 0;iMem<numMem;iMem++)
	// 			{
	// 				MSCompoG* pCompoG = pQT->ma_FloorMem[iMem];
	// 				if(pCompoG == NULL)	continue;
	// 				if(pCompoG->GetType() != nMemberType) continue;
	// 				itWall = SptMemberMap.find(pCompoG->m_ID);
	// 				if(itWall != SptMemberMap.end())	continue;
	// 				MSLinkMemberG* pUrWall = (MSLinkMemberG*)pCompoG;
	// 				GM2DLineVector UrLVec = pUrWall->GetShortLVec();
	// 				long res = LVec.Overlap4DTol(UrLVec,DegreeTol,dDistDtol,LinkTestLength);
	// 				if(res==0||res==2||res==6||res==7||res==19||res==20||res==24||res==25)
	// 					continue;
	// 				SptMemberMap.insert(make_pair(pUrWall->m_ID, pUrWall));
	// 			}
	// 		}
	// 
	// 	}
}

MSSlabPolygon::MSSlabPolygon(void)
{
}

MSSlabPolygon::~MSSlabPolygon(void)
{
}

void MSSlabPolygon::Link2Supports(char Side)
{
	//     long NumSpt = ma_SptMember.GetSize();
	//     for(long Idx=0; Idx<NumSpt; Idx++)
	//     {
	//         MSLinkMemPart* pMem = ma_SptMember[Idx];
	//         long MemDir = ma_SptDir[Idx];
	//         if((Side == 'L' && MemDir == 1) || (Side == 'R' && MemDir == -1))
	//         {
	//             pMem->mp_LSlab = (MSSlabG*)1;
	//         }
	//         else
	//         {
	//             pMem->mp_RSlab = (MSSlabG*)1;
	//         }
	//     }
}

//TODO Polygon을 Polyline으로 바꾸고, Mempart의 중심선을 연결하는 Polyline을 생성해야함
//Mempart간의 차이는 직선으로 연결, Connectivity의 중심과 MempartEnd를 연결
long MSSlabPolygon::GetPolygon(GM2DPolyline& ThePolygon)
{

	// 	 GM2DVector* lastPnt = NULL;
	//     for(int i=0; i< ma_SptMember.GetSize(); i++)
	// 	{
	// 		auto memPart = ma_SptMember[i];
	// 		GM2DVector* vecS = new GM2DVector();
	// 		GM2DVector* vecE = new GM2DVector();
	// 		*vecS = (memPart->mp_LLine->m_P0 +memPart->mp_RLine->m_P0) *0.5;
	// 		*vecE = (memPart->mp_LLine->m_P1 +memPart->mp_RLine->m_P1) *0.5;
	// 
	// 		int bulgeSign = 1;
	// 		if(ma_SptDir[i] == -1)
	// 		{
	// 			GM2DVector* temp = vecS;
	// 			vecS = vecE;
	// 			vecE = temp;
	// 			bulgeSign = -1;
	// 		}
	// 		
	// 		if(i==0 || vecS->Distance(*lastPnt) > 1e-3)
	// 		{
	// 			if(i>0)
	// 			{
	// 				ThePolygon.ma_Bulge.Add(0.);
	// 			}
	// 			ThePolygon.Add(vecS);
	// 			
	// 		}
	// 
	// 		ThePolygon.ma_Bulge.Add((memPart->mp_LLine->GetBulge() + memPart->mp_RLine->GetBulge()) *0.5 * bulgeSign);
	// 		ThePolygon.Add(vecE);
	// 		lastPnt = vecE;
	// 	}
	// 
	// 	GM2DVector* vec = ThePolygon.ma_Point[0];
	// 
	// 	if(vec->Distance(*lastPnt) > 1e-3)
	// 	{	
	// 		ThePolygon.ma_Bulge.Add(0.);
	// 		ThePolygon.AddCopy(*vec);
	// 	}


	return 0;
}

//long MSSlabPolygon::GetInPolygon(GMPolygon& TheInPolygon)
//{
//    CTypedPtrArray<CObArray, GM2DLineVector*> SptInLineArr;
//
//    long NumSptLine = ma_SptMember.GetSize();
//    for(long iSptLine=0; iSptLine<NumSptLine; iSptLine++)
//    {
//        GM2DLineVector* SptInLine;
//        MSLinkMemPart* pSptLine = ma_SptMember[iSptLine];
//        if(ma_SptDir[iSptLine] == 1)
//            SptInLine = new GM2DLineVector(pSptLine->mp_LLine->GetP0().m_X, pSptLine->mp_LLine->GetP0().m_Y, pSptLine->mp_LLine->GetP1().m_X, pSptLine->mp_LLine->GetP1().m_Y);
//        else
//        {
//            SptInLine = new GM2DLineVector(pSptLine->mp_RLine->GetP0().m_X, pSptLine->mp_RLine->GetP0().m_Y, pSptLine->mp_RLine->GetP1().m_X, pSptLine->mp_RLine->GetP1().m_Y);
//            SptInLine->ReverseDirection();
//        }
//
//        SptInLineArr.Add(SptInLine);
//    }
//
//    //Trim
//    GMVector* pVec;
//    for(iSptLine=0; iSptLine<NumSptLine; iSptLine++)
//    {
//        GM2DVector crossVec;
//        double MyLoc=0,UrLoc=0;
//        GM2DLineVector* SptLine1 = SptInLineArr[iSptLine];
//        GM2DLineVector* SptLine2 = SptInLineArr[(iSptLine+1)%NumSptLine];
//        long SptDir1 = ma_SptDir[iSptLine];
//        long SptDir2 = ma_SptDir[(iSptLine+1)%NumSptLine];
//
//        long Result = SptLine1->Intersect(*SptLine2, crossVec, &MyLoc, &UrLoc);
//        if(Result==-1 || Result==-2 || Result==-3)
//        {
//            pVec = new GMVector(SptLine1->m_P1.m_X, SptLine1->m_P1.m_Y);
//            TheInPolygon.ma_Vector.Add(pVec);
//        }
//        else if(Result==-5 || Result==0 || Result==1)
//        {
//            pVec = new GMVector(crossVec.m_X, crossVec.m_Y);
//            TheInPolygon.ma_Vector.Add(pVec);
//        }
//        else
//            ASSERT(0);
//    }
//
//    for(iSptLine=0; iSptLine<NumSptLine; iSptLine++)
//        delete SptInLineArr[iSptLine];
//    SptInLineArr.RemoveAll();
//
//    if(TheInPolygon.CheckPolygon() < 0)	return -1;
//
//    return 0;
//}

map<long, MSCompoG*> MSFloor::FindAndInsertLevelDif2LineMembers(MSLineLevelDifG* pLevelDifG)
{
	CMap<long, long, MSCompoG*, MSCompoG*> CheckedMemberMap;  // 하나의 CompoG가 여러 QT에 있을 경우 pCompoG와 한번만 검토되도록 하기 위하여 검토된 pUrCompoG 등록
	CTypedPtrArray<CObArray, MSFloorQT*> CrossQTArr;
	map<long, MSCompoG*> CompoGMap;

	shared_ptr<GM2DCurve> pCurve = pLevelDifG->GetCenCurve();

	mp_QT->FindCrossQTs(&*pCurve, CrossQTArr, false);
	long nQT = CrossQTArr.GetSize();
	for (long iQT = 0; iQT < nQT; iQT++)
	{
		MSFloorQT* pCurQT = CrossQTArr[iQT];

		long nMem = pCurQT->ma_FloorMem.GetSize();
		for (long iMem = 0; iMem < nMem; iMem++)
		{
			MSCompoG* pUrCompoG = pCurQT->ma_FloorMem[iMem];

			MSCompoG* pTempMem;
			if (CheckedMemberMap.Lookup(pUrCompoG->m_ID, pTempMem))
				continue;
			CheckedMemberMap.SetAt(pUrCompoG->m_ID, pUrCompoG);

			if (!pUrCompoG->IsLinkMember())
				continue;
			if (pUrCompoG->GetFloor() != this)
				continue;

			MSLinkMemberG* pUrLink = (MSLinkMemberG*)pUrCompoG;
			long nRval = pUrLink->CheckCrossThenAddLevelDif(pLevelDifG, pUrLink->ma_MemberLevelDif, this);
			if (nRval > 0)
			{
				CompoGMap.insert(make_pair(pUrLink->m_ID, pUrLink));
			}
		}
		nMem = pCurQT->ma_VerticalMem.GetSize();//WallMember
		for (long iMem = 0; iMem < nMem; iMem++)
		{
			MSCompoG* pUrCompoG = pCurQT->ma_VerticalMem[iMem];

			MSCompoG* pTempMem;
			if (CheckedMemberMap.Lookup(pUrCompoG->m_ID, pTempMem))
				continue;
			CheckedMemberMap.SetAt(pUrCompoG->m_ID, pUrCompoG);

			if (!pUrCompoG->IsLinkMember())
				continue;
			if (pUrCompoG->GetFloor() != this)
				continue;

			MSLinkMemberG* pUrLink = (MSLinkMemberG*)pUrCompoG;
			long nRval = pUrLink->CheckCrossThenAddLevelDif(pLevelDifG, pUrLink->ma_MemberLevelDif, this);
			if (nRval > 0)
			{
				CompoGMap.insert(make_pair(pUrLink->m_ID, pUrLink));
			}
		}
	}
	//상부평면에 WallMember
	MSFloor* pUpperFloor = GetUpperFloor();
	if (pUpperFloor)
	{
		pUpperFloor->mp_QT->FindCrossQTs(&*pCurve, CrossQTArr, true);
		long nQT = CrossQTArr.GetSize();
		for (long iQT = 0; iQT < nQT; iQT++)
		{
			MSFloorQT* pCurQT = CrossQTArr[iQT];
			long nMem = pCurQT->ma_VerticalMem.GetSize();//WallMember
			for (long iMem = 0; iMem < nMem; iMem++)
			{
				MSCompoG* pUrCompoG = pCurQT->ma_VerticalMem[iMem];

				MSCompoG* pTempMem;
				if (CheckedMemberMap.Lookup(pUrCompoG->m_ID, pTempMem))
					continue;
				CheckedMemberMap.SetAt(pUrCompoG->m_ID, pUrCompoG);

				if (!pUrCompoG->IsWallMember())
					continue;
				if (pUrCompoG->GetFloor() != pUpperFloor)
					continue;

				MSWallMemberG* pUrLink = (MSWallMemberG*)pUrCompoG;
				long nRval = pUrLink->CheckCrossThenAddLevelDif(pLevelDifG, pUrLink->ma_MemberLevelDifLow, this); // 20150901 LevelZone Bug Fix
				if (nRval > 0)
				{
					CompoGMap.insert(make_pair(pUrLink->m_ID, pUrLink));
				}
			}
		}

	}
	return CompoGMap;
}

map<long, MSCompoG*> MSFloor::FindAndRemoveLevelDif2LineMembers(MSLineLevelDifG* pLevelDifG)
{
	MSFloor* pUpperFloor = GetUpperFloor();
	map<long, MSCompoG*> CompoGMap;
	long NumLinkMemberGID = pLevelDifG->ma_LinkMemberGID.size();
	for (long i = 0; i < NumLinkMemberGID; i++)
	{
		long GID = pLevelDifG->ma_LinkMemberGID[i];
		MSLinkMemberG* pLink = (MSLinkMemberG*)GetCompoG(GID);
		if (pLink == NULL)
		{
			if (pUpperFloor)
			{
				MSLinkMemberG* pUpLink = (MSLinkMemberG*)pUpperFloor->GetCompoG(GID);
				if (pUpLink == NULL)
					continue;
				if (pUpLink->IsWallMember())
				{
					MSWallMemberG* pWallG = (MSWallMemberG*)pUpLink;
					pWallG->RemoveLevelDif(pLevelDifG);
					pWallG->RemoveLevelDifLow(pLevelDifG);
				}
			}
			continue;
		}
		CompoGMap.insert(make_pair(pLink->m_ID, pLink));
		pLink->RemoveLevelDif(pLevelDifG);
		if (pLink->IsWallMember())
		{
			MSWallMemberG* pWallG = (MSWallMemberG*)pLink;
			pWallG->RemoveLevelDifLow(pLevelDifG);
		}
	}

	pLevelDifG->ma_LinkMemberGID.clear();
	/*
	CMap<long, long, MSCompoG*, MSCompoG*> CheckedMemberMap;  // 하나의 CompoG가 여러 QT에 있을 경우 pCompoG와 한번만 검토되도록 하기 위하여 검토된 pUrCompoG 등록
	CTypedPtrArray<CObArray, MSFloorQT*> CrossQTArr;
	//map<long, MSCompoG*> CompoGMap;

	mp_QT->FindCrossQTs(pLevelDifG->mp_Curve, CrossQTArr, false);
	long nQT = CrossQTArr.GetSize();
	for (long iQT = 0; iQT < nQT; iQT++)
	{
		MSFloorQT* pCurQT = CrossQTArr[iQT];

		long nMem = pCurQT->ma_FloorMem.GetSize();
		for (long iMem = 0; iMem < nMem; iMem++)
		{
			MSCompoG* pUrCompoG = pCurQT->ma_FloorMem[iMem];

			MSCompoG* pTempMem;
			if (CheckedMemberMap.Lookup(pUrCompoG->m_ID, pTempMem))
				continue;
			CheckedMemberMap.SetAt(pUrCompoG->m_ID, pUrCompoG);

			if(!pUrCompoG->IsLinkMember())
				continue;

			MSLinkMemberG* pUrLink = (MSLinkMemberG*)pUrCompoG;
			pUrLink->RemoveLevelDif(pLevelDifG);

			CompoGMap.insert(make_pair(pUrLink->m_ID, pUrLink));
		}
		nMem = pCurQT->ma_VerticalMem.GetSize();//WallMember
		for (long iMem = 0; iMem < nMem; iMem++)
		{
			MSCompoG* pUrCompoG = pCurQT->ma_VerticalMem[iMem];

			MSCompoG* pTempMem;
			if (CheckedMemberMap.Lookup(pUrCompoG->m_ID, pTempMem))
				continue;
			CheckedMemberMap.SetAt(pUrCompoG->m_ID, pUrCompoG);

			if(!pUrCompoG->IsLinkMember())
				continue;

			MSLinkMemberG* pUrLink = (MSLinkMemberG*)pUrCompoG;
			pUrLink->RemoveLevelDif(pLevelDifG);

			CompoGMap.insert(make_pair(pUrLink->m_ID, pUrLink));
		}
	}

	//상부평면에 WallMember
	//MSFloor* pUpperFloor = GetUpperFloor();
	if(pUpperFloor)
	{
		pUpperFloor->mp_QT->FindCrossQTs(pLevelDifG->mp_Curve, CrossQTArr, true);
		long nQT = CrossQTArr.GetSize();
		for (long iQT = 0; iQT < nQT; iQT++)
		{
			MSFloorQT* pCurQT = CrossQTArr[iQT];
			long nMem = pCurQT->ma_VerticalMem.GetSize();//WallMember
			for (long iMem = 0; iMem < nMem; iMem++)
			{
				MSCompoG* pUrCompoG = pCurQT->ma_VerticalMem[iMem];

				MSCompoG* pTempMem;
				if (CheckedMemberMap.Lookup(pUrCompoG->m_ID, pTempMem))
					continue;
				CheckedMemberMap.SetAt(pUrCompoG->m_ID, pUrCompoG);

				if(!pUrCompoG->IsWallMember())
					continue;

				MSWallMemberG* pUrLink = (MSWallMemberG*)pUrCompoG;
				pUrLink->RemoveLevelDifLow(pLevelDifG);

				CompoGMap.insert(make_pair(pUrLink->m_ID, pUrLink));
			}
		}
	}
	*/
	return CompoGMap;
}

void MSFloor::InsertLevelDif2ALinkMemberByLevelZone(MSLinkMemberG* pLineG)
{
	long nZone = ma_LevelZone.size();
	for (long iZone = 0; iZone < nZone; iZone++)
	{
		MSLevelZone* pZone = ma_LevelZone[iZone];
		long nLine = pZone->ma_LineLevelDif.size();
		for (long iLine = 0; iLine < nLine; iLine++)
		{
			MSLineLevelDifG* pDifLine = pZone->ma_LineLevelDif[iLine];
			pLineG->CheckCrossThenAddLevelDif(pDifLine, pLineG->ma_MemberLevelDif, this);
		}
	}
	//hana-벽일경우 하부층에 LevelZone도 검토필요
	if (pLineG->IsWallMember())
		InsertLowLevelDif2AWallMemberByLevelZone(pLineG);

}

void MSFloor::InsertLowLevelDif2AWallMemberByLevelZone(MSLinkMemberG* pLineG)
{
	MSFloor* pLowerFloor = GetLowerFloor();
	if (pLowerFloor)
	{
		MSWallMemberG* pWallG = (MSWallMemberG*)pLineG;
		long nZone = pLowerFloor->ma_LevelZone.size();
		for (long iZone = 0; iZone < nZone; iZone++)
		{
			MSLevelZone* pZone = pLowerFloor->ma_LevelZone[iZone];
			long nLine = pZone->ma_LineLevelDif.size();
			for (long iLine = 0; iLine < nLine; iLine++)
			{
				MSLineLevelDifG* pDifLine = pZone->ma_LineLevelDif[iLine];
				pWallG->CheckCrossThenAddLevelDif(pDifLine, pWallG->ma_MemberLevelDifLow, pLowerFloor);
			}
		}
	}
}

void MSFloor::InsertLevelDif2SlabByLevelZone(MSSlabG* pSlabG, bool isDoit)
{
	long nZone = ma_LevelZone.size();
	for (long iZone = 0; iZone < nZone; iZone++)
	{
		MSLevelZone* pZone = ma_LevelZone[iZone];
		pSlabG->MakeSlabLevelZone(pZone, isDoit);
		if (CheckLevelDifGSerializing())
			pSlabG->CheckCrossThenAddLevelDif(pZone, isDoit);
	}

	pSlabG->TrimSlabLineLevelDifbyOpening();
}

/*
void MSFloor::TrimExistingSlabs(MSSlabG* pSlabG)
{
	vector<MSCompoG*> SlabArr;
	GetMSCompoGArr(MSElement::msSlabG, SlabArr);
	long nSlab = SlabArr.size();
	for(long iSlab=0; iSlab<nSlab; iSlab++)
	{
		MSSlabG* pCurSlab = (MSSlabG*)(SlabArr[iSlab]);

		BOOL bRval = pSlabG->GetProfile()->OverlapEx(*(pCurSlab->GetProfile()));
		if(bRval == FALSE)
			continue;

		int nOverlap = pSlabG->LevelOverlap(pCurSlab);
		if(nOverlap == 0)
			continue;

		CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;

		pCurSlab->GetProfile()->BoolOperation('A', pSlabG->GetProfile(), JoinedPolys, DTOL_GM);

		long NumJoinedPoly = JoinedPolys.GetCount();

		if(NumJoinedPoly == 1)
		{
			if(JoinedPolys[0]->IsClosed())
				pCurSlab->GetProfile()->CopyFrom(JoinedPolys[0]);
		}
		else if(NumJoinedPoly == 2)
		{
			pCurSlab->AddOpening(pSlabG->GetProfile());
		}
		else if(NumJoinedPoly == 0)
		{
			pSlabG->AddOpening(pCurSlab->GetProfile());
		}

		for(int ijoined = 0 ; ijoined < NumJoinedPoly ; ijoined++)
		{
			delete JoinedPolys[ijoined];
		}
		JoinedPolys.RemoveAll();
	}

}
*/

/*
// pPolylineObject 유지 시킴
void MSFloor::TrimExistPolyLineObjectStep1(MSPolylineObject* pPolylineObject)
{
	if(pPolylineObject == NULL) return;

	vector<MSPolylineObject*> PolylineObjectArr;
	GetPolylineObjectArr(PolylineObjectArr, pPolylineObject->GetOwnerType());

	long nPolylineObject = PolylineObjectArr.size();
	for(long iPolylineObject=0; iPolylineObject < nPolylineObject; iPolylineObject++)
	{
		MSPolylineObject* pCurPolylineObject = (MSPolylineObject*)(PolylineObjectArr[iPolylineObject]);

		BOOL bRval = pPolylineObject->mp_Profile->OverlapEx(*(pCurPolylineObject->mp_Profile));
		if(bRval == FALSE)
			continue;

		int nOverlap = pPolylineObject->LevelOverlap(pCurPolylineObject);
		if(nOverlap == 0)
			continue;
		// 슬래브 두께가 중첩 되었을경우에 처리

		//pCurPolylineObject inner 폴리곤 Contains
		long iInner = 0;

		for( iInner = pCurPolylineObject->ma_Opening.size() -1 ; iInner >= 0  ; iInner--)
		{
			MSOpening* pCurOpening = pCurPolylineObject->ma_Opening[iInner];
			TrimExistPolyLineObjectSetp2(pCurOpening->mp_PolylineInfo, pPolylineObject);

		}

		TrimExistPolyLineObjectSetp2(pCurPolylineObject, pPolylineObject);
	}
}
*/
/*
void MSFloor::TrimExistPolyLineObjectSetp2(MSPolylineObject* pCurPolylineObject, MSPolylineObject* pPolylineObject)
{
	CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;

	Type_MSCompoG pCurType = pCurPolylineObject->GetOwnerType();
	Type_MSCompoG pPolyType = pPolylineObject->GetOwnerType();
	BOOL isAddPoly = FALSE;

	if(pCurType == pPolyType || pCurType == MSElement::msOpening )
		pCurPolylineObject->mp_Profile->BoolOperation('A', pPolylineObject->mp_Profile, JoinedPolys, DTOL_GM);
	else
		return;

	long NumJoinedPoly = JoinedPolys.GetCount();

	if(NumJoinedPoly == 1)
	{
		if(JoinedPolys[0]->IsClosed())
		{
			//폴리 라인 형상이 바뀌면 LevelDif 다시 만들어 줘야함
			pCurPolylineObject->mp_Profile->CopyFrom(JoinedPolys[0]);
			pCurPolylineObject->ReMakeLevelDif();
		}
	}
	else if(NumJoinedPoly == 2 && pCurType != MSElement::msOpening)
	{
		if(pPolylineObject->GetOwnerType() == MSElement::msSlabG)
			pCurPolylineObject->AddOpening(pPolylineObject->GetProfile());
		else if(pPolylineObject->GetOwnerType() == MSElement::msLevelZone)
			pCurPolylineObject->AddOpening(pPolylineObject->GetProfile());
	}
	else if(NumJoinedPoly == 0)
	{
		if(pCurType == MSElement::msOpening)		// 입력 받은 PolyObject 가 CurPolyobject를 완전 포함함 삭제 해줘야함
		{
			MSOpening* pOpening = (MSOpening*)pCurPolylineObject->GetOwner();
			MSPolylineObject* pPrentPoly = (MSPolylineObject*)pOpening->GetOwner();
			pPrentPoly->RemoveOpeing((MSOpening*)pCurPolylineObject->GetOwner());
		}
		else
		{
			if(pPolylineObject->GetOwnerType() == MSElement::msSlabG)
				pPolylineObject->AddOpening(pCurPolylineObject->GetProfile());
			else if(pPolylineObject->GetOwnerType() == MSElement::msLevelZone)
				pPolylineObject->AddOpening(pCurPolylineObject->GetProfile());
		}
	}

	for(int ijoined = 0 ; ijoined < NumJoinedPoly ; ijoined++)
	{
		delete JoinedPolys[ijoined];
	}
	JoinedPolys.RemoveAll();
}
*/

long MSFloor::GetPolylineObjectArr(vector<MSPolylineObject*>& PolyArr, Type_MSCompoG nType)
{
	if (nType == MSElement::msLevelZone)
	{
		vector<MSLevelZone*>::iterator itLevelZone;
		for (itLevelZone = ma_LevelZone.begin(); itLevelZone != ma_LevelZone.end(); itLevelZone++)
		{
			MSLevelZone* levelzone = *itLevelZone;
			if (levelzone == NULL)
				continue;
			if (levelzone->mp_PolylineInfo == NULL)
				continue;
			PolyArr.push_back(levelzone->mp_PolylineInfo);
		}
	}
	else if (nType == MSElement::msSlabG || nType == MSElement::msOpening)
	{
		map<long, MSCompoG*>::iterator itCompoG;
		for (itCompoG = mm_CompoG.begin(); itCompoG != mm_CompoG.end(); itCompoG++)
		{
			MSCompoG* pCompoG = itCompoG->second;
			if (pCompoG == NULL)
				continue;
			if (pCompoG->GetType() != MSElement::msSlabG)
				continue;
			if (pCompoG->mp_PolylineInfo == NULL)
				continue;
			if (nType == MSElement::msSlabG)
				PolyArr.push_back(pCompoG->mp_PolylineInfo);
			else if (nType == MSElement::msOpening)
			{
				long NumOpen = pCompoG->mp_PolylineInfo->ma_Opening.size();
				for (long iOpen = 0; iOpen < NumOpen; iOpen++)
				{
					MSOpening* pOpen = pCompoG->mp_PolylineInfo->ma_Opening[iOpen];
					PolyArr.push_back(pOpen->mp_PolylineInfo);

				}
			}
		}
	}
	else if (nType == MSElement::msConstructionZone)
	{
		for (auto it = mm_CZone.begin(); it != mm_CZone.end(); it++)
		{
			MSConstructionZone* pCZone = it->second;
			if (pCZone == NULL)
				continue;
			if (pCZone->m_bDefaultCZone == true || pCZone->mp_PolylineInfo == NULL)
				continue;
			PolyArr.push_back(pCZone->mp_PolylineInfo);
		}
	}
	return PolyArr.size();
}


CTime MSFloor::GetStartDate()
{
	bool IsHorizontalZone = true;
	CTime horizontalEarliestDay = NULL;
	CTime verticalEarliestDay = NULL;

	if (GetFirstCZone(IsHorizontalZone) != nullptr)
		horizontalEarliestDay = GetFirstCZone(IsHorizontalZone)->GetActualStartDate();

	if (GetFirstCZone(!IsHorizontalZone) != nullptr)
		verticalEarliestDay = GetFirstCZone(!IsHorizontalZone)->GetActualStartDate();

	//CTime horizontalEarliestDay = GetFirstCZone(IsHorizontalZone)->GetActualStartDate();
	//CTime verticalEarliestDay = GetFirstCZone(!IsHorizontalZone)->GetActualStartDate();
	CTime earliestDay;
	if (horizontalEarliestDay == NULL || verticalEarliestDay == NULL)
	{
		if (horizontalEarliestDay == NULL) earliestDay = verticalEarliestDay;
		else earliestDay = horizontalEarliestDay;
	}
	else
	{
		earliestDay = horizontalEarliestDay;
		if (earliestDay > verticalEarliestDay)	earliestDay = verticalEarliestDay;
	}

	map<long, MSConstructionZone*>::iterator it;
	for (it = mm_CZone.begin(); it != mm_CZone.end(); ++it)
	{
		MSConstructionZone* zone = it->second;
		if (earliestDay > zone->GetActualStartDate())
			earliestDay = zone->GetActualStartDate();
	}

	return earliestDay;
}

CTime MSFloor::GetFinishDate()
{
	CTime latestDay = MSConstructionZone::InitialDate();

	map<long, MSConstructionZone*>::iterator it;
	for (it = mm_CZone.begin(); it != mm_CZone.end(); ++it)
	{
		MSConstructionZone* zone = it->second;
		if (latestDay < zone->GetActualFinishDate())
			latestDay = zone->GetActualFinishDate();
	}
	return latestDay;
}

void MSFloor::CheckFloorQT(vector<MSCompoG*> CompoGArr)
{
	if (CompoGArr.size() == 0)
		return;

	GM2DBound PolyBound;
	for (int iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
	{
		MSCompoG* pCompoG = CompoGArr[iCompoG];
		GM2DVector* pSVec = NULL;
		GM2DVector* pEVec = NULL;

		// jissi / Slab 인 경우 0번 점과 적당히 반대쪽을 사용한다?? GM2DRectangle 로 교체 GetSVec() override
		//if(pCompoG->GetType() == MSElement::msSlabG)
		if (pCompoG->IsPolyMember())
		{
			GM2DPolyline* pPoly = pCompoG->GetProfile();

			for (int iVec = 0; iVec < pPoly->ma_Point.GetSize(); iVec++)
			{
				pSVec = pPoly->ma_Point[iVec];
				if (pSVec != nullptr)
					PolyBound.AddVector(pSVec);
			}
		}
		else
		{
			pSVec = pCompoG->GetSVec();
			pEVec = pCompoG->GetEVec();
			if (pSVec != nullptr)
				PolyBound.AddVector(pSVec);
			if (pEVec != nullptr)
				PolyBound.AddVector(pEVec);
		}
	}

	GM2DVector TheVec1, TheVec2, CenVec;

	TheVec1 = *PolyBound.GetBottomLeft();
	TheVec2 = *PolyBound.GetTopRight();

	CenVec.m_X = (TheVec1.m_X + TheVec1.m_X) / 2.0;
	CenVec.m_Y = (TheVec2.m_Y + TheVec2.m_Y) / 2.0;

	MSBaseBuilding* pBuilding = GetBuilding();
	if (pBuilding == NULL) return;

	// jissi // BuildingSite (QT?)초기화?? CheckSite() > InitSite()
	if (pBuilding->m_SiteRect.m_Origin.SameVector4DTOL(pBuilding->m_SiteRect.m_Corner, 10))
	{
		pBuilding->ChangFloorQT(CenVec.m_X - 300000, CenVec.m_Y - 300000, CenVec.m_X + 300000, CenVec.m_Y + 300000);
		return;
	}

	// jissi // 왜 TheVec 만 체크하고 반대쪽은 안 하는지..
	//if(!mp_QT->m_Rect.Contains(&TheVec))
	BOOL nRval1 = mp_QT->m_Rect.Contains(&TheVec1);
	BOOL nRval2 = mp_QT->m_Rect.Contains(&TheVec2);

	if (!nRval1 ||
		!nRval2)
	{
		/*
		CString strMsg;
		strMsg.Format(_T("%d 부재가 쿼드 트리 범위를 벗어 났습니다.."), pCompoG->m_ID);
		XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();
		XErrorObject* pMsgObject = pMessageContainer->CreateErrorObject(MESSAGE_ERROR_OVERLAP, MESSAGE_LEVEL_ERROR, strMsg);

		if (pMsgObject)
		{
			pMsgObject->AddCompoGID(pCompoG->m_ID);
			pMsgObject->SetShowSelectCompoG(TRUE);
			pMessageContainer->AddErrorObject(pMsgObject);
		}
		*/

		GM2DVector OriVec, CorVec;
		double dMaxX = 0;
		double dMaxY = 0;
		double dMinX = 0;
		double dMinY = 0;

		// jissi // 1.5배의 의미는..?? 적당히 키운다..?? 서로 부호가 다르지 않을 때는..??
		dMinX = pBuilding->m_SiteRect.m_Origin.m_X - (pBuilding->m_SiteRect.m_Origin.m_X / 2);
		dMinY = pBuilding->m_SiteRect.m_Origin.m_Y - (pBuilding->m_SiteRect.m_Origin.m_Y / 2);
		dMaxX = pBuilding->m_SiteRect.m_Corner.m_X + (pBuilding->m_SiteRect.m_Corner.m_X / 2);
		dMaxY = pBuilding->m_SiteRect.m_Corner.m_Y + (pBuilding->m_SiteRect.m_Corner.m_Y / 2);
		if (!nRval1)
		{
			if (TheVec1.m_X > dMaxX)
				dMaxX = TheVec1.m_X;
			else if (TheVec1.m_X < dMinX)
				dMinX = TheVec1.m_X;

			if (TheVec1.m_Y > dMaxY)
				dMaxY = TheVec1.m_Y;
			else if (TheVec1.m_Y < dMinY)
				dMinY = TheVec1.m_Y;
		}

		if (!nRval2)
		{
			if (TheVec2.m_X > dMaxX)
				dMaxX = TheVec2.m_X;
			else if (TheVec2.m_X < dMinX)
				dMinX = TheVec2.m_X;

			if (TheVec2.m_Y > dMaxY)
				dMaxY = TheVec2.m_Y;
			else if (TheVec2.m_Y < dMinY)
				dMinY = TheVec2.m_Y;
		}
		pBuilding->ChangFloorQT(dMinX, dMinY, dMaxX, dMaxY);
	}
}

void MSFloor::ResetFloorQTLeaves()
{
	map<long, MSCompoG*>::iterator itMem;
	for (itMem = mm_CompoG.begin(); itMem != mm_CompoG.end(); itMem++)
	{
		MSCompoG* pMem = itMem->second;
		pMem->ma_CrossQT.RemoveAll();
		mp_QT->AddToLeaves(pMem);
	}
}

MSObject* MSFloor::CheckCompoGOverlap_WithUpDownFloor(MSCompoG* pUrCompoG, long nOldCompoGID, bool bMakeErrorObject, bool isCheckZLevel)
{
	return MSModelChecker::CheckCompoGOverlap_WithUpDownFloor(this, pUrCompoG, nOldCompoGID, bMakeErrorObject, isCheckZLevel);
}

MSObject* MSFloor::CheckCompoGOverlap(MSCompoG* pUrCompoG, long nOldCompoGID, bool bMakeErrorObject, bool isCheckZLevel, bool bCheckJoint)
{
	return MSModelChecker::CheckCompoGOverlap(this, pUrCompoG, nOldCompoGID, bMakeErrorObject, isCheckZLevel, bCheckJoint);
}

MSSlabRebar* MSFloor::CheckSlabRebarOverlap(MSSlabRebar* pUrSlabRebar, long nOldCompoGID, bool bMakeErrorObject)
{
	return MSModelChecker::CheckSlabRebarOverlap(this, pUrSlabRebar, nOldCompoGID, bMakeErrorObject);
}

MSObject* MSFloor::CheckSlabGOverlap(MSCompoG* pCompoG, long nOldCompoGID)
{
	return MSModelChecker::CheckSlabGOverlap(this, pCompoG, nOldCompoGID);
}

MSConstructionZone* MSFloor::CheckOverlapConstructionZone(MSConstructionZone* pZone, long nOldCZoneID)
{
	return MSModelChecker::CheckOverlapConstructionZone(this, pZone, nOldCZoneID);
}

MSElement* MSFloor::CheckOverlapLevelZone(MSLevelZone* pZone)
{
	return MSModelChecker::CheckOverlapLevelZone(this, pZone);
}

vector<long> MSFloor::CheckExtendOverlap(MSCompoG* pCompoG, bool isUpdateGeometry)
{
	return MSModelChecker::CheckExtendOverlap(this, pCompoG, isUpdateGeometry);
}

vector<int> MSFloor::GetOverlapCZoneIDs(MSCompoG* pCompoG, bool IsHorizontalZone)
{
	vector<int> CZoneIDArr;
	vector<MSConstructionZone*> CZoneArr;
	MSConstructionZone* DefCZone = GetCZoneDefault(IsHorizontalZone);
	long nDefCZoneID = 0;
	if (DefCZone != nullptr)
		nDefCZoneID = DefCZone->m_ID;

	GetAllCZone(CZoneArr, IsHorizontalZone, false);

	Type_MSCompoG CompoGType = pCompoG->GetType();

	long NumCZone = CZoneArr.size();
	if (NumCZone == 0)
		CZoneIDArr.push_back(nDefCZoneID);
	else
	{
		if (pCompoG->IsLinkMember())
		{
			// 처리함
			MSLinkMemberG* pLinkG = (MSLinkMemberG*)pCompoG;
			vector<GM2DCurve*> CenCurveArr = GetCenCurveByCZoneByType(pLinkG, IsHorizontalZone);

			long NumCenCurve = CenCurveArr.size();
			for (long iCurve = 0; iCurve < NumCenCurve; iCurve++)
			{
				GM2DCurve* pCurve = CenCurveArr[iCurve];
				GM2DVector MidVec = pCurve->PointAtParam(0.5);
				long CZoneID = GetOverlapCZoneID(MidVec.m_X, MidVec.m_Y, IsHorizontalZone);
				CZoneIDArr.push_back(CZoneID);
				delete pCurve;
			}
			CenCurveArr.clear();
		}
		else if (pCompoG->IsPointMember())
		{
			GM2DVector* pVec = pCompoG->GetSVec();
			long CZoneID = GetOverlapCZoneID(pVec->m_X, pVec->m_Y, IsHorizontalZone);
			CZoneIDArr.push_back(CZoneID);
		}
		else if (CompoGType == MSElement::msSlabG)
		{
			pCompoG->MakeBoundary();
			vector<int> CZones = GetOverlapCZoneIDs(pCompoG->GetProfile(), IsHorizontalZone, DTOL_GM);
			for (long i = 0; i < CZones.size(); i++)
				CZoneIDArr.push_back(CZones[i]);
		}
		else
		{
			pCompoG->MakeBoundary();
			vector<int> CZones = GetOverlapCZoneIDs(pCompoG->GetProfile(), IsHorizontalZone, DTOL_GM);
			for (long i = 0; i < CZones.size(); i++)
			{
				CZoneIDArr.push_back(CZones[i]);
				break;
			}
		}
	}

	return CZoneIDArr;
}

vector<int> MSFloor::GetOverlapCZoneIDs(MSCompoG* pCompoG)
{
	vector<int> CZoneIDArr;
	vector<MSConstructionZone*> CZoneArr;
	bool IsHorizontalZone = pCompoG->IsVerticalMember() == true ? false : true;
	MSConstructionZone* DefCZone = GetCZoneDefault(IsHorizontalZone);
	long nDefCZoneID = 0;
	if (DefCZone != nullptr)
		nDefCZoneID = DefCZone->m_ID;

	GetAllCZone(CZoneArr, IsHorizontalZone, false);

	Type_MSCompoG CompoGType = pCompoG->GetType();

	long NumCZone = CZoneArr.size();
	if (NumCZone == 0)
		CZoneIDArr.push_back(nDefCZoneID);
	else
	{
		if (CompoGType == MSElement::msBeamG && pCompoG->IsPC())
		{
			pCompoG->MakeBoundary();
			vector<int> CZones = GetOverlapCZoneIDs(pCompoG->GetProfile(), IsHorizontalZone, DTOL_GM);
			int nDefaultCZoneID = this->GetCZoneDefault(IsHorizontalZone)->GetID();
			for (long i = 0; i < CZones.size(); i++)
			{
				if (nDefaultCZoneID != CZones[i])
					CZoneIDArr.push_back(CZones[i]);
			}
		}
		else if (pCompoG->IsLinkMember())
		{
			// 처리함
			MSLinkMemberG* pLinkG = (MSLinkMemberG*)pCompoG;
			vector<GM2DCurve*> CenCurveArr = GetCenCurveByCZone(pLinkG);

			long NumCenCurve = CenCurveArr.size();
			for (long iCurve = 0; iCurve < NumCenCurve; iCurve++)
			{
				GM2DCurve* pCurve = CenCurveArr[iCurve];
				GM2DVector MidVec = pCurve->PointAtParam(0.5);
				long CZoneID = GetOverlapCZoneID(MidVec.m_X, MidVec.m_Y, IsHorizontalZone);
				CZoneIDArr.push_back(CZoneID);
				delete pCurve;
			}
			CenCurveArr.clear();
		}
		else if (pCompoG->IsPointMember())
		{
			GM2DVector* pVec = pCompoG->GetSVec();
			long CZoneID = GetOverlapCZoneID(pVec->m_X, pVec->m_Y, IsHorizontalZone);
			CZoneIDArr.push_back(CZoneID);
		}
		else if (CompoGType == MSElement::msSlabG)
		{
			pCompoG->MakeBoundary();
			vector<int> CZones = GetOverlapCZoneIDs(pCompoG->GetProfile(), IsHorizontalZone, DTOL_GM);
			for (long i = 0; i < CZones.size(); i++)
				CZoneIDArr.push_back(CZones[i]);
		}
		else
		{
			pCompoG->MakeBoundary();
			vector<int> CZones = GetOverlapCZoneIDs(pCompoG->GetProfile(), IsHorizontalZone, DTOL_GM);
			for (long i = 0; i < CZones.size(); i++)
			{
				CZoneIDArr.push_back(CZones[i]);
			}
		}
	}
	if (CZoneIDArr.size() == 0)
		CZoneIDArr.push_back(nDefCZoneID);
	return CZoneIDArr;
}

int MSFloor::GetCenterPointCZoneID(MSCompoG* pCompoG, bool IsHorizontalZone)
{
	vector<int> CZoneIDArr;
	vector<MSConstructionZone*> CZoneArr;
	//bool IsHorizontalZone = true; //pCompoG->IsVerticalMember() == true ? false : true;
	MSConstructionZone* DefCZone = GetCZoneDefault(IsHorizontalZone);
	long nDefCZoneID = 0;
	if (DefCZone != nullptr)
		nDefCZoneID = DefCZone->m_ID;

	GetAllCZone(CZoneArr, IsHorizontalZone, false);

	Type_MSCompoG CompoGType = pCompoG->GetType();

	long NumCZone = CZoneArr.size();
	if (NumCZone == 0)
		CZoneIDArr.push_back(nDefCZoneID);

	GM2DPolyline boundary;
	pCompoG->GetBoundary(boundary);
	GM2DVector center = boundary.MassCenter();
	int CZoneID = GetOverlapCZoneID(center, IsHorizontalZone);

	return CZoneID;
}

int MSFloor::GetMaxOverlapCZoneID(MSCompoG* pCompoG)
{
	vector<MSConstructionZone*> CZoneArr;
	bool IsHorizontalZone = pCompoG->IsVerticalMember() == true ? false : true;
	MSConstructionZone* DefCZone = GetCZoneDefault(IsHorizontalZone);
	long nDefCZoneID = 0;
	if (DefCZone != nullptr)
		nDefCZoneID = DefCZone->m_ID;

	GetAllCZone(CZoneArr, IsHorizontalZone, false);

	long NumCZone = CZoneArr.size();
	if (NumCZone == 0)
		return nDefCZoneID;

	pCompoG->MakeBoundary();
	GM2DPolyline* compoGProfile = pCompoG->GetProfile();
	vector<int> CZoneIDList = GetOverlapCZoneIDs(compoGProfile, IsHorizontalZone, DTOL_GM);

	// 시공구역 물량 개선 - PC부재는 overlap된 CZone중 면적이 가장 넓은것으로 밀어넣는다.
	MSPitG* pitG = dynamic_cast<MSPitG*>(pCompoG);
	MSGenStairG* stairG = dynamic_cast<MSGenStairG*>(pCompoG);

	vector<MSConstructionZone*> overlapCZoneList;
	for (int i = 0; i < CZoneIDList.size(); ++i)
	{
		int overlapID = CZoneIDList[i];
		for (int j = 0; j < CZoneArr.size(); ++j)
		{
			int curID = CZoneArr[j]->m_ID;
			if (overlapID == curID)
			{
				overlapCZoneList.push_back(CZoneArr[j]);
				break;
			}
		}
	}

	double maxArea = 0;
	int maxOverlapCZoneID = 0;
	for (int i = 0; i < overlapCZoneList.size(); ++i)
	{
		MSConstructionZone* curCZone = overlapCZoneList[i];
		GM2DPolyline* curCZoneProfile = curCZone->GetProfile();
		vector<GM2DPolyline*> JoinedPolys;
		compoGProfile->BoolOperation('I', curCZoneProfile, JoinedPolys, DTOL_GM);
		if (JoinedPolys.size() == 1)
		{
			double area = JoinedPolys[0]->Area();
			if (area > maxArea)
			{
				maxArea = area;
				maxOverlapCZoneID = curCZone->m_ID;
			}
		}
	}
	if (maxOverlapCZoneID == 0)
		maxOverlapCZoneID = nDefCZoneID;
	return maxOverlapCZoneID;
}

int MSFloor::GetOverlapCZoneID(double dXc, double dYc, bool IsHorizontalZone)
{
	GM2DVector TheVec(dXc, dYc);
	return GetOverlapCZoneID(TheVec, IsHorizontalZone);
}

int MSFloor::GetOverlapCZoneID(GM2DVector TheVec, bool IsHorizontalZone)
{
	vector<MSConstructionZone*> CZoneArr;
	GetAllCZone(CZoneArr, IsHorizontalZone, false);

	long NumCZone = CZoneArr.size();
	for (long iZone = 0; iZone < NumCZone; iZone++)
	{
		MSConstructionZone* pCZone = CZoneArr[iZone];
		bool isZoneType = pCZone->GetZoneType() == MSConstructionZone::Horizontal ? true : false;
		if (IsHorizontalZone != isZoneType)	continue;
		if (pCZone->Contains(TheVec.m_X, TheVec.m_Y))
			return pCZone->m_ID;
		//long nRval = pCZone->GetProfile()->Contains(&TheVec);
		//if (nRval < 1)
		//	return pCZone->m_ID;
	}

	MSConstructionZone* DefCZone = GetCZoneDefault(IsHorizontalZone);
	if (DefCZone == nullptr)	return 0;
	return DefCZone->m_ID;
}

vector<int> MSFloor::GetOverlapCZoneIDs(MSPolylineObject* pPolylineObject, bool IsHorizontalZone, double dTol)
{
	long NumOpen = pPolylineObject->ma_Opening.size();

	vector<int> CZoneIDArr;
	vector<MSConstructionZone*> CZoneArr;
	//MSConstructionZone* DefCZone = GetCZoneDefault();
	GetAllCZone(CZoneArr, IsHorizontalZone, true);

	long NumCZone = CZoneArr.size();
	for (long iZone = 0; iZone < NumCZone; iZone++)
	{
		MSConstructionZone* pCZone = CZoneArr[iZone];
		bool isZoneType = pCZone->GetZoneType() == MSConstructionZone::Horizontal ? true : false;
		if (IsHorizontalZone != isZoneType)	continue;
		if (pCZone->OverLap(pPolylineObject, dTol))
			CZoneIDArr.push_back(pCZone->m_ID);
	}

	return CZoneIDArr;
}

vector<int> MSFloor::GetOverlapCZoneIDs(GM2DPolyline* pPoly, bool IsHorizontalZone, double dTol)
{
	vector<GM2DPolyline*> PolyArr = GetPolyLineByCZone(pPoly, IsHorizontalZone);

	vector<int> CZoneIDArr;
	vector<MSConstructionZone*> CZoneArr;
	GetAllCZone(CZoneArr, IsHorizontalZone, true);

	int nDefaultCZoneID = -1;
	long NumCZone = CZoneArr.size();
	for (long iZone = 0; iZone < NumCZone; iZone++)
	{
		MSConstructionZone* pCZone = CZoneArr[iZone];
		bool isZoneType = pCZone->GetZoneType() == MSConstructionZone::Horizontal ? true : false;
		if (IsHorizontalZone != isZoneType)	continue;

		for (long i = 0; i < PolyArr.size(); i++)
		{
			GM2DPolyline* pCurPoly = PolyArr[i];

			BOOL bRavl = pCZone->OverLap(pCurPoly, dTol);
			if (bRavl == TRUE)
			{
				if (pCZone->m_bDefaultCZone)
				{
					nDefaultCZoneID = pCZone->m_ID;
					break;
				}
				CZoneIDArr.push_back(pCZone->m_ID);
			}
			/* // 중점 구하는대 문제 발생함(L 자 모양의 슬래브)
			GM2DVector TheVec =  pCurPoly->MassCenter();
			if(pCZone->Contains(TheVec.m_X, TheVec.m_Y))
				CZoneIDArr.push_back(pCZone->m_ID);
				*/
		}
	}
	if (nDefaultCZoneID > -1)
		CZoneIDArr.push_back(nDefaultCZoneID);

	for (long i = 0; i < PolyArr.size(); i++)
		delete PolyArr[i];

	return CZoneIDArr;
}

vector<GM2DCurve*> MSFloor::GetCurveByCZone(GM2DCurve* pCurCurve, bool IsHorizontalZone, double dMinLen)
{
	vector<GM2DCurve*> CurCurveArr;
	vector<GM2DCurve*> CurveArr;
	vector<MSConstructionZone*> CZoneArr;
	GetAllCZone(CZoneArr, IsHorizontalZone, false);

	CurCurveArr.push_back(pCurCurve->Copy());

	double dTol = 1.0;
	double dDegreeTol = 5.0;
	long NumCZone = CZoneArr.size();
	for (long iZone = 0; iZone < NumCZone; iZone++)
	{
		MSConstructionZone* pCZone = CZoneArr[iZone];
		bool isZoneType = pCZone->GetZoneType() == MSConstructionZone::Horizontal ? true : false;
		if (isZoneType != IsHorizontalZone)	continue;
		for (long iCurve = 0; iCurve < CurCurveArr.size(); iCurve++)
		{
			GM2DCurve* pCurve = CurCurveArr[iCurve];

			long nRval = pCZone->GetProfile()->OverlapEx(pCurve);
			if (nRval > 0)
			{
				CTypedPtrArray<CObArray, GM2DCurve*> mySegments;
				pCurve->GetSegments(mySegments, pCZone->GetProfile(), dTol, dDegreeTol);
				long NumSeg = mySegments.GetSize();
				if (NumSeg > 1)
				{
					//길이가 짧게 나왔을 경우 나누지 않는다.
					bool HasMinLen = false;
					for (long iSeg = 0; iSeg < NumSeg; iSeg++)
					{
						GM2DCurve* pSeg = mySegments[iSeg];
						double dLen = pSeg->Length();
						if (dLen < dMinLen)
						{
							HasMinLen = true;
							break;
						}
					}
					if (HasMinLen == false)
					{
						CurCurveArr.erase(CurCurveArr.begin() + iCurve);
						delete pCurve;

						for (long iSeg = 0; iSeg < NumSeg; iSeg++)
						{
							GM2DCurve* pSeg = mySegments[iSeg];
							CurCurveArr.push_back(pSeg);
						}
					}
					else
					{
						for (long iSeg = 0; iSeg < NumSeg; iSeg++)
						{
							GM2DCurve* pSeg = mySegments[iSeg];
							delete pSeg;
						}
					}
				}
			}
		}
	}

	for (long iCurve = 0; iCurve < CurCurveArr.size(); iCurve++)
	{
		GM2DCurve* pCurve = CurCurveArr[iCurve];
		CurveArr.push_back(pCurve);
	}
	return CurveArr;
}

vector<GM2DCurve*> MSFloor::GetCenCurveByCZoneByType(MSLinkMemberG* pLinkMemberG, bool IsHorizontalZone, bool SortLongLength)
{
	shared_ptr<GM2DCurve> pMyCenCurve = pLinkMemberG->GetCenCurvebyConntion();
	GM2DCurve* pMyCurve = pMyCenCurve->Copy();
	vector<GM2DCurve*> CurveArr = GetCurveByCZone(pMyCurve, IsHorizontalZone);

	if (SortLongLength == true)
	{
		sort(CurveArr.begin(), CurveArr.end(),
			[](GM2DCurve* a, GM2DCurve* b)-> bool {return a->Length() > b->Length(); }
		);
	}

	return CurveArr;
}

vector<GM2DCurve*> MSFloor::GetCenCurveByCZone(MSLinkMemberG* pLinkMemberG, bool SortLongLength)
{
	shared_ptr<GM2DCurve> pMyCenCurve = pLinkMemberG->GetCenCurvebyConntion();
	//shared_ptr<GM2DCurve> pMyCenCurve = pLinkMemberG->GetLongCenCurveTrimmedByTRBoundary(); //두번째
	GM2DCurve* pMyCurve = pMyCenCurve->Copy();
	bool IsHorizontalZone = pLinkMemberG->IsVerticalMember() == true ? false : true;
	vector<GM2DCurve*> CurveArr = GetCurveByCZone(pMyCurve, IsHorizontalZone);

	if (SortLongLength == true)
	{
		sort(CurveArr.begin(), CurveArr.end(),
			[](GM2DCurve* a, GM2DCurve* b)-> bool {return a->Length() > b->Length(); }
		);
	}

	return CurveArr;
}

vector<GM2DPolyline*> MSFloor::GetPolyLineByCZone(GM2DPolyline* pPoly, bool IsHorizontalZone)
{
	vector<MSConstructionZone*> CZoneArr;
	GetAllCZone(CZoneArr, IsHorizontalZone, false);

	double dTol = DTOL_GM;
	vector<GM2DPolyline*> RValPolyArr;
	vector<GM2DPolyline*> PolyArr;

	GM2DPolyline* pNewPoly = new GM2DPolyline(TRUE);
	pNewPoly->CopyFrom(pPoly);
	pNewPoly->DeleteInLinePoint();
	pNewPoly->DeleteDuplicatedPoints(DTOL_MODELING);
	pNewPoly->CheckAndFixFoldedEdges(DTOL_MODELING);
	PolyArr.push_back(pNewPoly);

	GM2DPolyline* pNewOrgPoly = new GM2DPolyline(TRUE);
	pNewOrgPoly->CopyFrom(pNewPoly);

	long NumCZone = CZoneArr.size();
	for (long iZone = 0; iZone < NumCZone; iZone++)
	{
		MSConstructionZone* pCZone = CZoneArr[iZone];
		bool isZoneType = pCZone->GetZoneType() == MSConstructionZone::Horizontal ? true : false;
		if (IsHorizontalZone != isZoneType)	continue;
		for (long iPoly = 0; iPoly < PolyArr.size(); iPoly++)
		{
			GM2DPolyline* pCurPoly = PolyArr[iPoly];
			if (pCurPoly->GetNumLines() <= 1)
				continue;

			vector<GM2DPolyline*> JoinedPolys;
			pCurPoly->SegmentstoMe(pCZone->GetProfile(), dTol);
			pCurPoly->BoolOperation('I', pCZone->GetProfile(), JoinedPolys, dTol);

			long NumJoind = JoinedPolys.size();
			if (NumJoind == 0)
				continue;
			for (long i = 0; i < NumJoind; i++)
			{
				RValPolyArr.push_back(JoinedPolys[i]);
			}
			JoinedPolys.clear();

			//bool bRavl = pCurPoly->Contains(pCZone->m_pPoly, false);
			bool bRavl = pCurPoly->Contains(pCZone->GetProfile(), true);
			if (bRavl == true)
				continue;

			pCurPoly->BoolOperationNSortByArea('A', pCZone->GetProfile(), JoinedPolys, dTol);

			NumJoind = JoinedPolys.size();
			if (NumJoind == 0)
			{
				pCurPoly->ResetPolyline();
			}
			else
			{
				int cnt = 0;
				for (long i = 0; i < NumJoind; i++)
				{
					// BoolOperation이후 작은 영역을 처리 하지 않기 위한 코드 -Start
					double dXMin = 0, dYMin = 0, dXMax = 0, dYMax = 0;
					double area = JoinedPolys[i]->Area_ACIS();
					JoinedPolys[i]->GetXYMinMax(dXMin, dYMin, dXMax, dYMax);
					if (fabs(dXMax - dXMin) < DTOL_MODELING || fabs(dYMax - dYMin) < DTOL_MODELING)
						continue;

					double dMaxXY = max(fabs(dXMax - dXMin), fabs(dYMax - dYMin));
					if (dMaxXY != 0 && (area / dMaxXY) < DTOL_MODELING)
						continue;
					// BoolOperation이후 작은 영역을 처리 하지 않기 위한 코드 -End

					if (cnt == 0)
					{
						pCurPoly->CopyFrom(JoinedPolys[i]);
						delete JoinedPolys[i];
					}
					else
						PolyArr.push_back(JoinedPolys[i]);

					cnt++;
				}
			}
		}
	}

	long NumPoly = RValPolyArr.size();
	if (NumPoly == 0)
	{
		GM2DPolyline* pCurPoly = new GM2DPolyline(TRUE);
		pCurPoly->CopyFrom(pNewPoly);
		RValPolyArr.push_back(pCurPoly);
	}
	else
	{
		for (int iPoly = PolyArr.size() - 1; iPoly >= 0; iPoly--)
		{
			GM2DPolyline* pCurPoly = PolyArr[iPoly];
			if (pCurPoly->GetNumLines() <= 2)
				continue;
			if (!pNewOrgPoly->SamePolyLine(pCurPoly))
			{
				RValPolyArr.push_back(pCurPoly);
				PolyArr.erase(PolyArr.begin() + iPoly);
			}
		}
	}

	for (int iPoly = PolyArr.size() - 1; iPoly >= 0; iPoly--)
	{
		GM2DPolyline* pCurPoly = PolyArr[iPoly];
		delete pCurPoly;
	}
	PolyArr.clear();

	delete pNewOrgPoly;

	return RValPolyArr;
}

bool MSFloor::SpliteElementByCZone(MSElement* pOrgElement, vector<MSElement*>& SplitElements, bool bNewID)
{
	if (pOrgElement->GetProfile() == NULL)
		return false;

	vector<MSConstructionZone*> CZoneArr;
	bool IsHorizontalZone = pOrgElement->IsVerticalMember() == true ? false : true;
	GetAllCZone(CZoneArr, IsHorizontalZone, false);

	double dTol = DTOL_GM;
	vector<MSElement*> TempPolyArr;

	MSElement* pNewElement = (MSElement*)pOrgElement->Copy(bNewID);
	TempPolyArr.push_back(pNewElement);

	GM2DPolyline* pNewPoly = pNewElement->GetProfile();
	pNewPoly->DeleteInLinePoint();
	pNewPoly->DeleteDuplicatedPoints(DTOL_MODELING);
	pNewPoly->CheckAndFixFoldedEdges(DTOL_MODELING);

	GM2DPolyline* pNewOrgPoly = new GM2DPolyline(TRUE);
	pNewOrgPoly->CopyFrom(pNewPoly);
	long NumOpening = pNewElement->mp_PolylineInfo->ma_Opening.size();

	long NumCZone = CZoneArr.size();
	for (long iZone = 0; iZone < NumCZone; iZone++)
	{
		MSConstructionZone* pCZone = CZoneArr[iZone];

		for (long iPoly = 0; iPoly < TempPolyArr.size(); iPoly++)
		{
			MSElement* pCurElement = TempPolyArr[iPoly];
			GM2DPolyline* pCurPoly = pCurElement->GetProfile();
			if (pCurPoly->GetNumLines() <= 1)
				continue;

			CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;
			pCurPoly->SegmentstoMe(pCZone->GetProfile(), dTol);
			pCurPoly->BoolOperation('I', pCZone->GetProfile(), JoinedPolys, dTol);

			long NumJoind = JoinedPolys.GetSize();
			if (NumJoind == 0) continue;
			for (long i = 0; i < NumJoind; i++)
			{
				MSElement* pSplitElement = (MSElement*)pCurElement->Copy(bNewID);
				pSplitElement->SetProfile(JoinedPolys[i]);
				pSplitElement->mp_PolylineInfo->TrimOpenings();
				SplitElements.push_back(pSplitElement);
			}
			GMObject::DeleteGMObject(JoinedPolys);
			JoinedPolys.RemoveAll();

			//bool bRavl = pCurPoly->Contains(pCZone->m_pPoly, false);
			bool bRavl = pCurPoly->Contains(pCZone->GetProfile(), false);
			if (bRavl == true)
			{
				pCurElement->mp_PolylineInfo->AddOpening(pCZone->GetProfile(), pCZone);
				continue;
			}

			pCurPoly->BoolOperation('A', pCZone->GetProfile(), JoinedPolys, dTol);

			NumJoind = JoinedPolys.GetSize();
			if (NumJoind == 0)
			{
				pCurPoly->ResetPolyline();
			}
			else
			{
				for (long i = 0; i < NumJoind; i++)
				{
					if (i == 0)
					{
						pCurPoly->CopyFrom(JoinedPolys[i]);
						pCurElement->mp_PolylineInfo->TrimOpenings();
						delete JoinedPolys[i];
					}
					else
					{
						MSElement* pNewSplitElement = (MSElement*)pCurElement->Copy(bNewID);
						pNewSplitElement->SetProfile(JoinedPolys[i]);
						pNewSplitElement->mp_PolylineInfo->TrimOpenings();
						TempPolyArr.push_back(pNewSplitElement);
					}
				}
			}
		}
	}

	long NumPoly = SplitElements.size();
	if (NumPoly == 0)
	{
		SplitElements.push_back(pNewElement);
	}
	else
	{
		for (int iPoly = TempPolyArr.size() - 1; iPoly >= 0; iPoly--)
		{
			MSElement* pElement = TempPolyArr[iPoly];
			GM2DPolyline* pCurPoly = pElement->GetProfile();
			if (pCurPoly->GetNumLines() <= 1)
				continue;
			bool IsSamePoly = pNewOrgPoly->SamePolyLine(pCurPoly);

			if (IsSamePoly == false ||
				(IsSamePoly == true && pElement->mp_PolylineInfo->ma_Opening.size() != NumOpening))
			{
				pElement->mp_PolylineInfo->TrimOpenings();
				SplitElements.push_back(pElement);
				TempPolyArr.erase(TempPolyArr.begin() + iPoly);
			}
		}

		for (int iPoly = TempPolyArr.size() - 1; iPoly >= 0; iPoly--)
			delete TempPolyArr[iPoly];
	}

	TempPolyArr.clear();

	delete pNewOrgPoly;

	return true;
}

bool MSFloor::SpliteElementByCZone2(MSElement* pOrgElement, vector<MSElement*>& SplitElements, bool bNewID)
{
	GM2DPolyline* pPoly = pOrgElement->GetProfile();
	if (pPoly == NULL)
		return false;

	vector<GM2DPolyline*> poly_list;
	vector<MSConstructionZone*> CZoneArr;
	bool IsHorizontalZone = pOrgElement->IsVerticalMember() == true ? false : true;
	GetAllCZone(CZoneArr, IsHorizontalZone, false);

	vector<GM2DCurve*> curveList;
	for (int iCZone = 0; iCZone < CZoneArr.size(); iCZone++)
	{
		MSConstructionZone* pCZone = CZoneArr[iCZone];
		if (pCZone->OverLap(pPoly) == true)
		{
			pCZone->GetProfile()->GetSegments(curveList);
			for (int i = 0; i < pCZone->mp_PolylineInfo->ma_Opening.size(); i++)
				pCZone->mp_PolylineInfo->ma_Opening[i]->GetProfile()->GetSegments(curveList);
		}
	}

	if (curveList.size() == 0)
	{
		SplitElements.push_back((MSElement*)pOrgElement->Copy(bNewID));
		return true;
	}

	// 오프닝 위에 존재하는 Curve 삭제
// 	if (pOrgElement->mp_PolylineInfo != nullptr)
// 	{
// 		for (int iOpening = 0; iOpening < pOrgElement->mp_PolylineInfo->ma_Opening.size(); iOpening++)
// 		{
// 			MSOpening* pOpening = pOrgElement->mp_PolylineInfo->ma_Opening[iOpening];
// 
// 			for (int iCurve = curveList.size() - 1; iCurve >= 0; iCurve--)
// 			{
// 				GM2DCurve* pCurve = curveList[iCurve];
// 				GM2DCurve* pOpeningCurve = pOpening->GetProfile()->GetOnLineCurve(pCurve, DTOL_GM);
// 				if (pOpeningCurve != nullptr)
// 				{
// 					delete pOpeningCurve;
// 					delete pCurve;
// 					curveList.erase(curveList.begin() + iCurve);
// 				}
// 			}
// 		}
// 	}

	AcisGeomHelper::DividePolyline(curveList, pPoly, poly_list);

	// poly_list에 에러 폴라라인이 있으면 pOrgElement의 폴리라인을 반환
	for (int iPoly = poly_list.size() - 1; iPoly >= 0; iPoly--)
	{
		GM2DPolyline* pNewPoly = poly_list[iPoly];
		pNewPoly->DeleteInLinePoint(DTOL_MODELING);
		double dArea = pNewPoly->Area_ACIS();
		if (dArea < 100)
		{
			poly_list.erase(poly_list.begin() + iPoly);
			delete pNewPoly;
			continue;
		}

		// 		if (pNewPoly->CheckPolygon(false) == false)
		// 		{
		// 			for (int i = 0; i < curveList.size(); i++)
		// 				delete curveList[i];
		// 			GMObject::DeleteGMObject(poly_list);
		// 
		// 			SplitElements.push_back((MSElement*)pOrgElement->Copy(bNewID));
		// 			return false;
		// 		}
	}

	//poly_list 크기순으로 정렬
	sort(poly_list.begin(), poly_list.end(),
		[](GM2DPolyline* a, GM2DPolyline* b)->bool {return a->Area() > b->Area(); }
	);

	for (int iPoly = 0; iPoly < poly_list.size(); iPoly++)
	{
		GM2DPolyline* pNewPoly = poly_list[iPoly];
		if (pOrgElement->mp_PolylineInfo->Contains(pNewPoly, true, DTOL_GM) == false)
			continue;

		MSElement* pNewElement = (MSElement*)pOrgElement->Copy(bNewID);
		pNewElement->SetProfile(pNewPoly);
		for (int jPoly = 0; jPoly < poly_list.size(); jPoly++)
		{
			if (iPoly == jPoly)
				continue;

			GM2DPolyline* pNewPoly2 = poly_list[jPoly];
			bool bRavl = pNewPoly->Contains(pNewPoly2, false);
			if (bRavl == true)
			{
				pNewElement->mp_PolylineInfo->AddOpening(pNewPoly2, NULL);
				continue;
			}
		}

		if (pNewElement->mp_PolylineInfo != nullptr)
			pNewElement->mp_PolylineInfo->TrimOpenings();

		SplitElements.push_back(pNewElement);
	}

	for (int i = 0; i < curveList.size(); i++)
		delete curveList[i];

	GMObject::DeleteGMObject(poly_list);
	return true;
}

bool MSFloor::CheckLevelDifGSerializing()
{
	bool bOldVersionSerialize = (m_bIsSerializing && MSVersionInfo::GetCurrentVersion() < 20140527);
	return (bOldVersionSerialize || !m_bIsSerializing);
}

bool MSFloor::IsContainSlab(MSCompoG* pCompoG, double dRatio)
{
	BOOL bIncludeOnLine = FALSE;

	GM2DPolyline CompoGPoly(TRUE);
	pCompoG->GetBoundary(CompoGPoly);

	GM2DPolyline* pCompoGPoly = new GM2DPolyline(TRUE);
	pCompoGPoly->CopyFrom(&CompoGPoly);

	double dCompoGArea = AcisGeomHelper::GetArea(*pCompoGPoly);
	double dOrgCompoGArea = dCompoGArea;

	vector<GM2DPolyline*> CompoGPolyArr;
	CompoGPolyArr.push_back(pCompoGPoly);

	vector<MSPolylineObject*> PolylineObjectArr;
	GetPolylineObjectArr(PolylineObjectArr, MSElement::msSlabG);

	long NumSlab = PolylineObjectArr.size();
	for (long iSlab = 0; iSlab < NumSlab; iSlab++)
	{
		MSPolylineObject* pCurPolylineObject = (MSPolylineObject*)(PolylineObjectArr[iSlab]);
		GM2DPolyline* pSlabPoly = pCurPolylineObject->GetProfile();
		GM2DBound SlabBound;
		pSlabPoly->Get2DBound(SlabBound);

		for (long i = 0; i < CompoGPolyArr.size(); i++)
		{
			GM2DPolyline* pPoly = CompoGPolyArr[i];
			GM2DBound MyBound;
			pPoly->Get2DBound(MyBound);

			if (MyBound.OverLapTo(&SlabBound))
			{
				vector<GM2DPolyline*> Joined_Polys;
				AcisGeomHelper::BooleanPolyline('A', *pPoly, *pSlabPoly, Joined_Polys);
				long NumJoinedPolys = Joined_Polys.size();
				double dJoindArea = 0;
				if (NumJoinedPolys == 0)
				{
					//완전 포함
					dJoindArea = AcisGeomHelper::GetArea(*pPoly);
					dCompoGArea = dCompoGArea - dJoindArea;
				}
				else
				{
					for (long j = 0; j < NumJoinedPolys; j++)
					{
						GM2DPolyline* pJoinedPoly = Joined_Polys[j];
						dJoindArea = AcisGeomHelper::GetArea(*pJoinedPoly);
						if (j == 0)
						{
							pPoly->CopyFrom(pJoinedPoly);
							delete pJoinedPoly;
						}
						else
							CompoGPolyArr.push_back(pJoinedPoly);

						dCompoGArea = dJoindArea;
					}
				}
			}

			if (dCompoGArea < DTOL_GM)
				break;
		}
		if (dCompoGArea < DTOL_GM)
			break;
	}

	for (long i = 0; i < CompoGPolyArr.size(); i++)
		delete CompoGPolyArr[i];

	double dCompoGRatio = 100;
	if (dOrgCompoGArea != 0)
		dCompoGRatio = (dOrgCompoGArea - dCompoGArea) / dOrgCompoGArea * 100;

	if (fabs(dRatio - dCompoGRatio) < DTOL_GM)
		return true;

	return false;
}

double MSFloor::GetBiggerMemberDepth(MSElement::Type_MSCompoG eType)
{
	double dBiggerDepth = 0;
	map<long, MSCompoG*>::iterator itMem;
	for (itMem = mm_CompoG.begin(); itMem != mm_CompoG.end(); itMem++)
	{
		MSCompoG* pCompoG = itMem->second;
		MSElement::Type_MSCompoG eCurType = pCompoG->GetType();
		if (eCurType == eType)
			dBiggerDepth = max(dBiggerDepth, pCompoG->GetDepth());

	}
	return dBiggerDepth;
}

MSCompoM* MSFloor::GetMostlyUsedSlabMByArea()
{
	// Depth, Area
	map<MSCompoM*, double> SlabDepthAreaMap;
	map<MSCompoM*, double>::iterator it;
	map<long, MSCompoG*>::iterator itMem;
	for (itMem = mm_CompoG.begin(); itMem != mm_CompoG.end(); itMem++)
	{
		MSCompoG* pCompoG = itMem->second;
		MSElement::Type_MSCompoG eCurType = pCompoG->GetType();
		if (eCurType == MSElement::msSlabG)
		{
			MSSlabG* pSlabG = (MSSlabG*)pCompoG;
			MSCompoM* pCompoM = pSlabG->GetCompoM();
			GM2DPolyline* pPoly = pSlabG->GetProfile();
			double dArea = 0;
			if (pPoly->HasBulge())
				dArea = AcisGeomHelper::GetArea(*pPoly);
			else
				dArea = pPoly->Area();

			it = SlabDepthAreaMap.find(pCompoM);
			if (it == SlabDepthAreaMap.end())
				SlabDepthAreaMap.insert(make_pair(pCompoM, dArea));
			else
				it->second += dArea;
		}
	}

	MSCompoM* pSlabM = NULL;
	double dMaxArea = 0;
	for (it = SlabDepthAreaMap.begin(); it != SlabDepthAreaMap.end(); it++)
	{
		if (dMaxArea < it->second)
			pSlabM = it->first;
	}

	return pSlabM;
}

double MSFloor::GetMostlyUsedSlabMDepth()
{
	MSCompoM* pSlabM = GetMostlyUsedSlabMByArea();
	if (pSlabM == NULL)
		return 0;
	return pSlabM->GetDepth();
}

double MSFloor::GetLevelZoneLevel(bool isMax)
{
	double dZLevel = 0;
	if (isMax)
		dZLevel = MININT;
	else
		dZLevel = MAXINT;

	long NumZone = ma_LevelZone.size();
	for (long iZone = 0; iZone < NumZone; iZone++)
	{
		MSLevelZone* pZone = ma_LevelZone[iZone];
		if (isMax)
			dZLevel = max(pZone->m_Level, dZLevel);
		else
			dZLevel = min(pZone->m_Level, dZLevel);
	}
	if (NumZone == 0)
		return 0;

	return dZLevel;
}

double MSFloor::GetWallSOffSet(bool isMax)
{
	double dOffSet = 0;
	if (isMax)
		dOffSet = MININT;
	else
		dOffSet = MAXINT;

	map<long, MSCompoG*>::iterator it;
	for (it = mm_CompoG.begin(); it != mm_CompoG.end(); it++)
	{
		MSCompoG* pCompoG = it->second;
		if (pCompoG->GetType() == MSElement::msWallG)
		{
			MSWallMemberG* pWall = (MSWallMemberG*)pCompoG;
			if (isMax)
				dOffSet = max(pWall->m_SOffsetZ, dOffSet);
			else
				dOffSet = min(pWall->m_SOffsetZ, dOffSet);
		}

	}
	if (mm_CompoG.size() == 0)
		return 0;

	return dOffSet;
}

double MSFloor::GetWallEOffSet(bool isMax)
{
	double dOffSet = 0;
	if (isMax)
		dOffSet = MININT;
	else
		dOffSet = MAXINT;

	map<long, MSCompoG*>::iterator it;
	for (it = mm_CompoG.begin(); it != mm_CompoG.end(); it++)
	{
		MSCompoG* pCompoG = it->second;
		if (pCompoG->GetType() == MSElement::msWallG)
		{
			MSWallMemberG* pWall = (MSWallMemberG*)pCompoG;
			if (isMax)
				dOffSet = max(pWall->m_EOffsetZ, dOffSet);
			else
				dOffSet = min(pWall->m_EOffsetZ, dOffSet);
		}

	}
	if (mm_CompoG.size() == 0)
		return 0;

	return dOffSet;
}

vector<int> MSFloor::GetUsedReinforceBarSettingID(MSCompoM::Type_MSCompoM eMType)
{
	vector<MSCompoG*> MemberArr;
	GetMSCompoGArr(eMType, MemberArr);

	vector<int> ReinforceBarSettingIDArr;

	long NumMem = MemberArr.size();
	for (long i = 0; i < NumMem; i++)
	{
		MSCompoG* pCompoG = MemberArr[i];
		if (pCompoG == NULL || pCompoG->GetDummy())
			continue;

		MSMaterial* pCurMaterial = pCompoG->GetMaterial();
		if (pCurMaterial->GetType() != MSMaterial::msConcrete)
			continue;

		MSConcrete* pConcrete = (MSConcrete*)pCurMaterial;
		ReinforceBarSettingIDArr.push_back(pConcrete->m_pReinforceBarSetting->m_ID);
	}

	sort(ReinforceBarSettingIDArr.begin(), ReinforceBarSettingIDArr.end(),
		[](const int& a, const int& b)->bool {return a < b; }
	);

	vector<int>::iterator new_end;
	new_end = unique(ReinforceBarSettingIDArr.begin(), ReinforceBarSettingIDArr.end());
	ReinforceBarSettingIDArr.erase(new_end, ReinforceBarSettingIDArr.end());

	return ReinforceBarSettingIDArr;
}

vector<int> MSFloor::GetUsedMaterialID(MSCompoM::Type_MSCompoM eMType)
{
	vector<MSCompoG*> MemberArr;
	GetMSCompoGArr(eMType, MemberArr);

	vector<int> MaterialIDArr;

	long NumMem = MemberArr.size();
	for (long i = 0; i < NumMem; i++)
	{
		MSCompoG* pCompoG = MemberArr[i];
		if (pCompoG == NULL || pCompoG->GetDummy())
			continue;

		MSMaterial* pCurMaterial = pCompoG->GetMaterial();
		MaterialIDArr.push_back(pCurMaterial->m_ID);
	}

	sort(MaterialIDArr.begin(), MaterialIDArr.end(),
		[](const int& a, const int& b)->bool {return a < b; }
	);

	vector<int>::iterator new_end;
	new_end = unique(MaterialIDArr.begin(), MaterialIDArr.end());
	MaterialIDArr.erase(new_end, MaterialIDArr.end());

	return MaterialIDArr;
}

vector<double> MSFloor::GetUsedFck(MSCompoM::Type_MSCompoM eMType)
{
	vector<MSCompoG*> MemberArr;
	GetMSCompoGArr(eMType, MemberArr);

	vector<double> FckArr;

	long NumMem = MemberArr.size();
	for (long i = 0; i < NumMem; i++)
	{
		MSCompoG* pCompoG = MemberArr[i];
		if (pCompoG == NULL || pCompoG->GetDummy())
			continue;

		MSMaterial* pCurMaterial = pCompoG->GetMaterial();
		if (pCurMaterial->GetType() != MSMaterial::msConcrete)
			continue;

		MSConcrete* pConcrete = (MSConcrete*)pCurMaterial;
		FckArr.push_back(pConcrete->m_ConcStrength);
	}

	sort(FckArr.begin(), FckArr.end(),
		[](const double& a, const double& b)->bool {return a < b; }
	);

	vector<double>::iterator new_end;
	new_end = unique(FckArr.begin(), FckArr.end());
	FckArr.erase(new_end, FckArr.end());

	return FckArr;
}

vector<double> MSFloor::GetUsedMainBarDia(MSCompoM::Type_MSCompoM eMType, int nReinforceBarSettingID, bool isFoundation)
{
	vector<double> DiaArr;

	vector<MSCompoG*> MemberArr;
	GetMSCompoGArr(eMType, MemberArr);

	long NumMem = MemberArr.size();
	for (long i = 0; i < NumMem; i++)
	{
		MSCompoG* pCompoG = MemberArr[i];
		if (pCompoG == NULL || pCompoG->GetDummy())
			continue;

		if (nReinforceBarSettingID != 0)
		{
			MSMaterial* pCurMaterial = pCompoG->GetMaterial();
			if (pCurMaterial->GetType() != MSMaterial::msConcrete)
				continue;

			MSConcrete* pConcrete = (MSConcrete*)pCurMaterial;
			if (pConcrete->m_pReinforceBarSetting->m_ID != nReinforceBarSettingID)
				continue;
		}

		MDMember* pRebarM = pCompoG->GetCompoM()->GetRebarM();
		if (eMType == MSCompoM::msSlabM)
		{
			//enum SLAB_TYPE {MAT_TYPE=0, FLAT_TYPE, GENERAL_TYPE, PC_TYPE, DECK_TYPE};
			MSRcSlabM* pSlabM = (MSRcSlabM*)pRebarM;
			long nSlabMType = pSlabM->GetSlabMType();
			if (isFoundation && (nSlabMType != 0 && nSlabMType != 1))
				continue;
			else if (!isFoundation && (nSlabMType == 0 || nSlabMType == 1))
				continue;
		}

		vector<double> CurDiaArr = pRebarM->GetMainBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), CurDiaArr.begin(), CurDiaArr.end());
	}

	sort(DiaArr.begin(), DiaArr.end(),
		[](const double& a, const double& b)->bool {return a < b; }
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MSFloor::GetUsedStirrupBarDia(MSCompoM::Type_MSCompoM eMType, int nReinforceBarSettingID, bool isFoundation)
{
	vector<double> DiaArr;

	vector<MSCompoG*> MemberArr;
	GetMSCompoGArr(eMType, MemberArr);

	long NumMem = MemberArr.size();
	for (long i = 0; i < NumMem; i++)
	{
		MSCompoG* pCompoG = MemberArr[i];
		if (pCompoG == NULL || pCompoG->GetDummy())
			continue;

		if (nReinforceBarSettingID != 0)
		{
			MSMaterial* pCurMaterial = pCompoG->GetMaterial();
			if (pCurMaterial->GetType() != MSMaterial::msConcrete)
				continue;

			MSConcrete* pConcrete = (MSConcrete*)pCurMaterial;
			if (pConcrete->m_pReinforceBarSetting->m_ID != nReinforceBarSettingID)
				continue;
		}

		MSCompoM* pCompoM = pCompoG->GetCompoM();
		MDMember* pRebarM = pCompoM->GetRebarM();
		if (eMType == MSCompoM::msSlabM)
		{
			//enum SLAB_TYPE {MAT_TYPE=0, FLAT_TYPE, GENERAL_TYPE, PC_TYPE, DECK_TYPE};
			MSRcSlabM* pSlabM = (MSRcSlabM*)pRebarM;
			long nSlabMType = pSlabM->GetSlabMType();
			if (isFoundation && (nSlabMType != 0 && nSlabMType != 1))
				continue;
			else if (!isFoundation && (nSlabMType == 0 || nSlabMType == 1))
				continue;
		}

		if (eMType == MSCompoM::msBWallM)
		{
			double dDia = MSDeformedBar::FindDeformedBarDiameterByIndex(pCompoG->GetRebarTemplate()->GetBWallSpacerDiaIndex());
			DiaArr.push_back(dDia);
		}
		else if (eMType == MSCompoM::msRWallM)
		{
			double dDia = MSDeformedBar::FindDeformedBarDiameterByIndex(pCompoG->GetRebarTemplate()->GetRWallSpacerDiaIndex());
			DiaArr.push_back(dDia);
		}

		vector<double> CurDiaArr = pRebarM->GetStirrupBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), CurDiaArr.begin(), CurDiaArr.end());
	}

	sort(DiaArr.begin(), DiaArr.end(),
		[](const double& a, const double& b)->bool {return a < b; }
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

double MSFloor::GetFloorLevel()
{
	if (fabs(m_dCashFloorLevel - INT_MAX) > DTOL_GM)
		return m_dCashFloorLevel;

	MSBaseBuilding* pBuild = GetBuilding();
	if (pBuild != NULL)
		m_dCashFloorLevel = pBuild->GetFloorLevel(m_nFloorNum);

	return m_dCashFloorLevel;
}

vector<MSFloor*> MSFloor::GetFloorsByLevel(double dStartLevel, double dEndLevel)
{
	MSBaseBuilding* pBuild = GetBuilding();
	if (pBuild == nullptr)
	{
		ASSERT(0);
		vector<MSFloor*> floorArr;
		return floorArr;
	}

	return pBuild->GetFloorsByLevel(dStartLevel, dEndLevel);
}

void MSFloor::SetSlabLevelOffset(double slabLevelOffset)
{
	if (fabs(m_SlabLevelOffset - slabLevelOffset) < DTOL_GM)
		return;

	m_SlabLevelOffset = slabLevelOffset;
	ReSetCompoGZLevel();

	MSBaseBuilding* pBuilding = GetBuilding();
	if (pBuilding != nullptr)
		pBuilding->ClearAllCashFloorLevel();
}

double MSFloor::GetSlabLevel()
{
	if (fabs(m_dCashSlabLevel - INT_MAX) > DTOL_GM)
		return m_dCashSlabLevel;

	MSBaseBuilding* pBuild = GetBuilding();
	if (pBuild != NULL)
		m_dCashSlabLevel = pBuild->GetSlabLevel(m_nFloorNum);

	return m_dCashSlabLevel;
}

double MSFloor::GetSlabHeight()
{
	MSFloor* pUpperFloor = GetUpperFloor();
	if (pUpperFloor == NULL)
		return GetHeight();

	return pUpperFloor->GetSlabLevel() - GetSlabLevel();
}

void MSFloor::SetHeight(double dHeight, bool bChangInfo)
{
	if (fabs(m_dHeight - dHeight) < DTOL_GM)
		return;

	double dChangHeight = dHeight - m_dHeight;
	m_dHeight = dHeight;

	if (bChangInfo == true && fabs(dChangHeight) > DTOL_GM)
	{
		for (auto it = mm_CompoG.begin(); it != mm_CompoG.end(); it++)
		{
			MSCompoG* pCompoG = it->second;
			pCompoG->CheckData();
		}
		ReSetCompoGZLevel();
	}

	MSBaseBuilding* pBuilding = GetBuilding();
	if (pBuilding != nullptr)
		pBuilding->ClearAllCashFloorLevel();
}

void MSFloor::ClearCashFloorLevel()
{
	m_dCashFloorLevel = INT_MAX;
	m_dCashSlabLevel = INT_MAX;
}

bool MSFloor::SetHeightAndSlabLevel(double height, double slabLevel)
{
	if (fabs(m_dHeight - height) < DTOL_GM && fabs(m_SlabLevelOffset - slabLevel) < DTOL_GM)
		return true;

	double oldSlabLevel = m_SlabLevelOffset;
	m_SlabLevelOffset = slabLevel;

	double oldHeight = m_dHeight;
	bool HasCheckDataFalse = false;
	m_dHeight = height;


	MSFloor* LowFloor = GetLowerFloor();
	MSFloor* UpperFloor = GetUpperFloor();
	vector<MSFloor*> floorList;
	floorList.push_back(this);
	if (LowFloor != nullptr)	floorList.push_back(LowFloor);
	if (UpperFloor != nullptr)	 floorList.push_back(UpperFloor);

	bool IsOK = true;
	for (auto it = floorList.begin(); it != floorList.end(); it++)
		(*it)->ReSetCompoGZLevel();

	for (auto it = floorList.begin(); it != floorList.end(); it++)
		(*it)->CheckCompoGs();

	return IsOK;
}

bool MSFloor::CheckCompoGs()
{
	bool IsOK = true;
	for (auto it = mm_CompoG.begin(); it != mm_CompoG.end(); it++)
	{
		MSCompoG* pCompoG = it->second;
		if (pCompoG->CheckData() == false)
			IsOK = false;;
	}
	return IsOK;
}

void MSFloor::UpdateTrimmedBoundary(MSCompoG* pCompoG)
{
	// Floor에 없는 부재는 리턴한다.
	map<long, MSCompoG*>::iterator it = mm_CompoG.find(pCompoG->m_ID);
	if (it == mm_CompoG.end())
		return;

	pCompoG->UpdateTrimmedBoundary();
}

void MSFloor::CalculateUpdatedCompoGArray(MSElement* pElement, vector<int>& vCompoGID, bool bUpdatePartner)
{
	if (pElement->GetFloor() != NULL && pElement->GetFloor()->m_bIsSerializing) return; // mylee
	vCompoGID.push_back(pElement->m_ID);

	if (pElement->IsCompoG() == true)
	{
		MSCompoG* pCompoG = (MSCompoG*)pElement;

		if (bUpdatePartner == true)
		{
			if (pCompoG->ma_ContainedMemberID.size() > 0 && pCompoG->IsPolyMember())
				vCompoGID.insert(vCompoGID.end(), pCompoG->ma_ContainedMemberID.begin(), pCompoG->ma_ContainedMemberID.end());

			vector<MSCompoG*> PartnerCompoGArr = pCompoG->GetPartnerCompoGs();
			for (long i = 0; i < PartnerCompoGArr.size(); i++)
			{
				MSCompoG* pPartnerG = PartnerCompoGArr[i];
				// 점부재는 업데이트 안함
				if (pPartnerG->IsPointMember())
					continue;
				// 수직 부재가 업데이트 되면 모든 연결된 부재를 업데이트함
				// 층 부재가 업데이트 되면 연결된 층 부재만 업데이트함
// 					if (pCompoG->IsVerticalMember() ||
// 						(pCompoG->IsFloorMember() && pPartnerG->IsFloorMember()) ||
// 						fabs(pCompoG->GetMemberDegree() - pPartnerG->GetMemberDegree()) < DTOL_GM) // 나란한 부재의 경우 다시 업데이트 한다.
				vCompoGID.push_back(PartnerCompoGArr[i]->m_ID);
			}

			// BH-4522
			// IMT PC Slab와 다른부재 (보, 기둥 등)간의 트림은 한쪽이 다른걸 잘라버리는 것이 아니라 서로간에 일부분씩 잘라내야 한다.
			// 트림로직상 슬래브 우선순위가 가장 높으므로 일반적인 방법으로는 슬래브의 업데이트가 일어나지 않기때문에 여기서 IMT PC Slab를 추가해 준다.
			vector<MSCompoG*> FindSlabList = pCompoG->FindConnectMember(FindType_MSElement::msSlabG, FIND_ALLFLOOR);
			for (int i = 0; i < FindSlabList.size(); i++)
			{
				if (FindSlabList[i]->IsPC())
					vCompoGID.push_back(FindSlabList[i]->m_ID);
			}

			if (pCompoG->GetType() == MSElement::msSlabG)
			{
				vector<MSCompoG*> FindStairList = pCompoG->FindConnectMember(FindType_MSElement::msStairG, FIND_CURFLOOR);
				for (int i = 0; i < FindStairList.size(); i++)
					vCompoGID.push_back(FindStairList[i]->m_ID);
			}
		}
	}
}

void MSFloor::FindOverlapMembernUpdateTrimBoundary(MSCompoG* pCompoG, bool bSecondTime)
{
	FindOverlapMembersNCalcTrimOption(pCompoG, bSecondTime);

	vector<MSCompoG*> PartnerCompoGArr = pCompoG->GetPartnerCompoGs();
	for (int i = 0; i < PartnerCompoGArr.size(); ++i)
		for (int k = 0; k < PartnerCompoGArr[i]->ma_Connections.size(); ++k)
			PartnerCompoGArr[i]->ma_Connections[k]->AdjustTrimOptionAfter(); // mylee 0126

	UpdateTrimmedBoundary(pCompoG);

	if (bSecondTime == false)
	{
		/////////////////////////////////////////////////////////////////////////////
		// FindOverlap one-more-time after Trim
		FindOverlapMembersNCalcTrimOption(pCompoG, true);
		for (int i = 0; i < PartnerCompoGArr.size(); ++i)
			FindOverlapMembersNCalcTrimOption(PartnerCompoGArr[i], true);
		// FindOverlap one-more-time after Trim 이후 파트너CompoG가 변경된 경우가 존재하므로 다시 찾아준다.
		//PartnerCompoGArr = pCompoG->GetPartnerCompoGs();
	}

	pCompoG->RemoveSameConnection();
}

void MSFloor::UpdateTrimmedBoundary(vector<MSCompoG*> CompoGArr)
{
	for (long i = 0; i < CompoGArr.size(); i++)
		UpdateTrimmedBoundary(CompoGArr[i]);
}

MSCompoG* MSFloor::GetMSCompoG(GM2DVector* vec, Type_MSCompoG memberType)
{
	for (auto it = mm_CompoG.begin(); it != mm_CompoG.end(); it++)
	{
		if (it->second->GetType() == memberType || memberType == Type_MSCompoG::msNULL)
		{
			MSCompoG* pMSCompoG = it->second;
			GM2DPolyline bound(TRUE);
			pMSCompoG->GetBoundary(bound);
			if (bound.Contains(vec, DTOL_GM) <= 0)
				return pMSCompoG;
		}
	}
	return NULL;
}

MSSlabG* MSFloor::FindSlabGByMaxOverlapArea(GM2DPolyline* pPoly)
{
	MSSlabG* pMaxOverlapSlabG = NULL;
	double maxArea = 0.0;
	vector<MSCompoG*> CompoGArr = MSCompoGUtil::FindCompoG(this, pPoly, FindType_MSElement::msSlabG);
	vector<GM2DPolyline*> JoinedPolys;
	for (int iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
	{
		MSSlabG* pCurSlabG = (MSSlabG*)CompoGArr[iCompoG];
		GM2DPolyline* SlabPolyline = pCurSlabG->GetProfile();

		// 기획안 Page3. 3-1 : 보강근 영역의 중심포인트가 포함된 슬래브 & 반대 경우
		if (pPoly->Contains(&SlabPolyline->MassCenter()) == -1 || SlabPolyline->Contains(&pPoly->MassCenter()) == -1)
		{
			SlabPolyline->BoolOperation('I', pPoly, JoinedPolys, DTOL_GM);
			double curArea = 0;
			for (int iArea = 0; iArea < JoinedPolys.size(); iArea++)
				curArea += JoinedPolys[iArea]->Area();

			// 3-2 : 교차면적이 비슷한 경우는 하단 슬래브
			if (fabs(curArea - maxArea) < DTOL1 && pMaxOverlapSlabG != NULL)
			{
				if (pCurSlabG->m_dOffset < pMaxOverlapSlabG->m_dOffset)
				{
					maxArea = curArea;
					pMaxOverlapSlabG = pCurSlabG;
				}
				continue;
			}

			if (curArea > maxArea)
			{
				maxArea = curArea;
				pMaxOverlapSlabG = pCurSlabG;
			}
		}
	}
	return pMaxOverlapSlabG;
}

void MSFloor::UpdateCompoGs()
{
	UpdateCompoGs(mm_CompoG, true);
}

void MSFloor::UpdateCompoGs(map<long, MSCompoG*> CompoGMap, bool IsRemoveConnection)
{
	(*MSConnection::IGNORE_HISTORY()) = true;
	vector<MSCompoG*> SlabArr;
	vector<MSCompoG*> pitArr;
	map<long, MSCompoG*>::iterator itCompoG;

	for (itCompoG = CompoGMap.begin(); itCompoG != CompoGMap.end(); itCompoG++)
	{
		MSCompoG* pCompoG = itCompoG->second;
		if (pCompoG == NULL) continue;
		pCompoG->ClearOverlapFloor();
		if (IsRemoveConnection == true)
			pCompoG->RemoveConnections(true);

		if (pCompoG->GetType() == MSElement::msSlabG)
		{
			SlabArr.push_back(pCompoG);

			//MSSlabG::SetProfile() 와 유사하게 구현, 
			//mp_PolylineInfo->SetProfile()함수의 pSlab->ClearSlopeInfo();호출 안하도록 처리
			
			/*
			// 3DTrim 옵션이 추가되면서 사용하지 않도록 변경함
			GM2DPolyline* pPoly = pCompoG->GetTrimmedBoundary_Acis();
			GM2DPolyline* pProfile = pCompoG->GetProfile();
			pProfile->FixPolyLine();

			if (pProfile != pPoly)
				pProfile->CopyFrom(pPoly);
			pCompoG->mp_PolylineInfo->mp_Profile->CopyFrom(pPoly);
			*/
			pCompoG->SetProfile(pCompoG->mp_PolylineInfo->GetProfile());
			pCompoG->InitTrimmedBoundary(true);
			pCompoG->InitTrimmedBoundary(false);
		}

		if (pCompoG->GetType() == MSElement::msPitG)
		{
			MSPitG* pPit = (MSPitG*)pCompoG;
			pPit->ma_SlabGID.clear();
			pitArr.push_back(pPit);
		}

		if (pCompoG->IsLinkMember())
		{
			MSLinkMemberG* pLinkG = dynamic_cast<MSLinkMemberG*>(pCompoG);
			pLinkG->SetLines();
		}

		pCompoG->SetContainingElementId(0);
	}

	for (itCompoG = CompoGMap.begin(); itCompoG != CompoGMap.end(); itCompoG++)
	{
		MSCompoG* pCompoG = itCompoG->second;
		if (pCompoG == NULL)	continue;
		FindOverlapMembersNCalcTrimOption(pCompoG);
		pCompoG->CheckTrimOpt();
	}

	for (itCompoG = CompoGMap.begin(); itCompoG != CompoGMap.end(); itCompoG++)
	{
		MSCompoG* pCompoG = itCompoG->second;
		if (pCompoG == NULL)	continue;
		pCompoG->TrimInitialize();
	}

	map<MSConnection*, MSConnection*> connections;
	for (itCompoG = CompoGMap.begin(); itCompoG != CompoGMap.end(); itCompoG++)
	{
		MSCompoG* pCompoG = itCompoG->second;
		if (pCompoG == NULL)	continue;
		for (int i = 0; i < pCompoG->ma_Connections.size(); i++)
		{
			pCompoG->ApplyMiterOptionToBoundary(pCompoG->ma_Connections[i]);
			pCompoG->ApplySquareOffOptionToBoundary(pCompoG->ma_Connections[i]);
			auto iter = connections.find(pCompoG->ma_Connections[i]);
			if (iter == connections.end())
				connections.insert(make_pair(pCompoG->ma_Connections[i], pCompoG->ma_Connections[i]));
		}
	}

	for (auto iter = connections.begin(); iter != connections.end(); iter++)
	{
		MSConnection* pConnection = iter->second;
		pConnection->MakeIntersectBoundary();
	}

	for (itCompoG = CompoGMap.begin(); itCompoG != CompoGMap.end(); itCompoG++)
	{
		MSCompoG* pCompoG = itCompoG->second;
		if (pCompoG == NULL)	continue;
		pCompoG->TrimMerge();
		pCompoG->UpdateTrimmedBoundary_AfterCalculate();
		GM2DPolyline* pPoly = pCompoG->GetTrimmedBoundary();
		if (pPoly != nullptr)
		{
			pPoly->MakeClosedPolyline(DTOL_MODELING);
			pPoly->CheckAndFixFoldedEdges(DTOL_MODELING);
		}
	}

	for (long iSlab = 0; iSlab < SlabArr.size(); iSlab++)
	{
		MSSlabG* pSlab = (MSSlabG*)SlabArr[iSlab];
		pSlab->RemoveOpeningInfoAll();
		InsertLevelDif2SlabByLevelZone(pSlab);
	}

	for (long iMem = 0; iMem < pitArr.size(); iMem++)
	{
		MSPitG* pCurPit = (MSPitG*)pitArr[iMem];
		pCurPit->MakeConnectedSlabInfo();
	}

	for (itCompoG = CompoGMap.begin(); itCompoG != CompoGMap.end(); itCompoG++)
	{
		MSCompoG* pCompoG = itCompoG->second;
		if (pCompoG == NULL)	continue;
		FindOverlapMembersNCalcTrimOption(pCompoG, true);
	}

	MSCompoG::SortByPriorityNum(SlabArr);
	for (long iSlab = 0; iSlab < SlabArr.size(); iSlab++)
	{
		MSSlabG* pSlab = (MSSlabG*)SlabArr[iSlab];
		pSlab->mp_PolylineInfo->TrimExistPolyLineObject(true);
		pSlab->mp_PolylineInfo->TrimExistPolyLineObject(false);
		//pSlab->TrimLevelDifAll();
		pSlab->UpdateSubMember();
	}

	for (itCompoG = CompoGMap.begin(); itCompoG != CompoGMap.end(); itCompoG++)
	{
		MSCompoG* pCompoG = itCompoG->second;
		if (pCompoG == NULL)	continue;
		pCompoG->UpdateBoundaryVisibilities();
		CalculateUpdatedCompoGArray(pCompoG, CompoGIDList_NeedRegen_CurTR(), false);
	}

	(*MSConnection::IGNORE_HISTORY()) = false;
}


void MSFloor::InitCompoGZLevel()
{
	mm_CompoGZLevel.clear();
	mm_CompoGZLevel.insert(make_pair(GetSlabLevel(), -1));
	mm_CompoGZLevel.insert(make_pair(GetSlabLevel() + GetSlabHeight(), -1));
}

void MSFloor::ReSetCompoGZLevel()
{
	InitCompoGZLevel();
	map<long, MSCompoG*>::iterator itMem;
	for (itMem = mm_CompoG.begin(); itMem != mm_CompoG.end(); itMem++)
	{
		MSCompoG* pMem = itMem->second;
		if (pMem->GetType() == msStairG)
		{
			MSGenStairG *pGenStairG = (MSGenStairG*)pMem;
			pGenStairG->CalculatePartHeight();
		}
		else
		{
			double dTopLevel = 0;
			double dBotLevel = 0;
			if (pMem->GetZLevels(dTopLevel, dBotLevel))
			{
				AddCompoGZLevel(dTopLevel);
				AddCompoGZLevel(dBotLevel);
			}
		}
	}
}

double MSFloor::GetTopCompoGZLevel()
{
	if (mm_CompoGZLevel.size() == 0)
		InitCompoGZLevel();

	map<double, int>::iterator it;
	it = mm_CompoGZLevel.end();
	it--;

	return it->first;
}

double MSFloor::GetBotCompoGZLevel()
{
	if (mm_CompoGZLevel.size() == 0)
		InitCompoGZLevel();

	map<double, int>::iterator it;
	it = mm_CompoGZLevel.begin();

	return it->first;
}

void MSFloor::AddCompoGZLevel(double dZLevel)
{
	map<double, int>::iterator it = mm_CompoGZLevel.find(dZLevel);
	if (it == mm_CompoGZLevel.end())
		mm_CompoGZLevel.insert(make_pair(dZLevel, 1));
	else
	{
		if (it->second != -1)
			it->second++;
	}
}

void MSFloor::DelCompoGZLevel(double dZLevel)
{
	map<double, int>::iterator it = mm_CompoGZLevel.find(dZLevel);
	if (it != mm_CompoGZLevel.end())
	{
		if (it->second != -1)
			it->second--;

		if (it->second == 0)
			mm_CompoGZLevel.erase(it);
	}
}

void MSFloor::ConverterOpeningG()
{
	vector<MSCompoG*> SlabArr;
	vector<MSOpening*> OpeningArr;
	map<long, MSCompoG*>::iterator it;
	for (it = mm_CompoG.begin(); it != mm_CompoG.end(); it++)
	{
		MSCompoG* pCompoG = it->second;
		if (pCompoG->GetType() == MSElement::msSlabG)
		{
			MSSlabG* pSlab = (MSSlabG*)pCompoG;

			long NumOpen = pSlab->mp_PolylineInfo->ma_Opening.size();
			for (long i = 0; i < NumOpen; i++)
			{
				MSOpening* pOpen = pSlab->mp_PolylineInfo->ma_Opening[i];
				if (pOpen->ma_MadeByCompoGID.size() == 0)
					OpeningArr.push_back(pOpen);
			}

			if (NumOpen > 0)
				SlabArr.push_back(pCompoG);
		}
	}

	vector<MSOpeningG*> NewOpeningArr;
	for (long iOpen = 0; iOpen < OpeningArr.size(); iOpen++)
	{
		MSOpening* pOpen = OpeningArr[iOpen];
		bool bMerged = false;
		GM2DPolyline* pPoly = pOpen->GetProfile();

		for (long iNewOpen = 0; iNewOpen < NewOpeningArr.size(); iNewOpen++)
		{
			MSOpeningG* pNewOpening = NewOpeningArr[iNewOpen];
			GM2DPolyline* pNewPoly = pNewOpening->GetProfile();

			vector<GM2DPolyline*> JoinedPolys;
			pNewPoly->BoolOperation('U', pPoly, JoinedPolys, DTOL_GM);

			long NumJoinedPolys = JoinedPolys.size();
			if (NumJoinedPolys == 1)
			{
				if (JoinedPolys[0]->SamePolyLine(pPoly) == false)
				{
					pNewPoly->CopyFrom(JoinedPolys[0]);
					bMerged = true;
					break;
				}
			}
		}

		if (bMerged == false)
		{
			MSCompoM* pCompoM = GetBuilding()->GetDefaultOpeningReinforceM();
			if (pCompoM == NULL)
			{
				ASSERT(0);
				return;
			}

			MSOpeningG* pNewOpening = new MSOpeningG();
			pNewOpening->GetID();
			pNewOpening->SetOwner(this);
			pNewOpening->SetCompoM(pCompoM->m_ID);
			GM2DPolyline* pNewPoly = pNewOpening->GetProfile();
			pNewPoly->CopyFrom(pPoly);
			NewOpeningArr.push_back(pNewOpening);
		}
	}

	for (long iSlab = 0; iSlab < SlabArr.size(); iSlab++)
	{
		MSSlabG* pSlab = (MSSlabG*)SlabArr[iSlab];
		pSlab->RemoveOpeningInfoAll();
	}


	for (long iNewOpen = 0; iNewOpen < NewOpeningArr.size(); iNewOpen++)
	{
		Add(NewOpeningArr[iNewOpen]);
	}
}


MSLevelZone* MSFloor::GetContainingLevelZone(GM2DPolyline* pPoly)
{
	for (int i = 0; i < ma_LevelZone.size(); i++)
	{
		MSLevelZone* pLevelZone = ma_LevelZone[i];
		GM2DPolyline* pLevelZonePoly = pLevelZone->GetProfile();

		bool bRVal = pLevelZonePoly->Contains(pPoly);
		if (bRVal == true)
			return pLevelZone;
	}

	return NULL;
}
void MSFloor::CheckRebarLine()
{
	map<long, MSSlabRebar*>::iterator it;
	for (it = mm_Rebar.begin(); it != mm_Rebar.end(); it++)
		it->second->CheckRebarLine(true);
}
// 
// void MSFloor::DeleteSlabRebarBox()
// {
// 	for (long iBox = 0; iBox < ma_SlabRebarBox.size(); iBox++)
// 		delete ma_SlabRebarBox[iBox];
// 
// 	ma_SlabRebarBox.clear();
// }
// 
// void MSFloor::MakeSlabRebarBox()
// {
// 	DeleteSlabRebarBox();
// 	map<long, MSSlabRebar*>::iterator it;
// 
// 	for (it = mm_Rebar.begin(); it != mm_Rebar.end(); it++)
// 	{
// 		SlabRebarBox* pSlabRebarBox = new SlabRebarBox(it->second);
// 		ma_SlabRebarBox.push_back(pSlabRebarBox);
// 	}
// }

void MSFloor::FixBoundaryCompoGs()
{
	map<long, MSCompoG*> FixCompoGMap;
	for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
	{
		MSCompoG* pCompoG = itr->second;
		if (pCompoG->IsPolyMember() == true || pCompoG->GetTrimmedBoundary() == NULL)
			continue;

		bool isFix = false;

		if (pCompoG->GetTrimmedBoundary()->ma_Point.GetSize() > 25)
			isFix = true;
		else
		{
			for (int iConn = pCompoG->ma_Connections.size() - 1; iConn >= 0; iConn--)
			{
				MSConnection* pConn = pCompoG->ma_Connections[iConn];
				if (pConn->GetFirstCompoG() == NULL || pConn->GetSecondCompoG() == NULL)
				{
					pCompoG->RemoveConnection(pConn);
					isFix = true;
					continue;
				}

				MSCompoG* pPartnerG = pConn->GetPartner(pCompoG);
				if (pPartnerG == nullptr) continue;
				if (pPartnerG->GetFloor() != pCompoG->GetFloor())
				{
					pCompoG->RemoveConnection(pConn);
					pPartnerG->RemoveConnection(pConn);
					isFix = true;
				}

				if (pCompoG->GetType() == MSElement::msHandRailG || pPartnerG->GetType() == MSElement::msHandRailG)
					isFix = true;
			}
		}

		if (isFix == true)
			FixCompoGMap.insert(make_pair(pCompoG->m_ID, pCompoG));
	}

	UpdateCompoGs(FixCompoGMap, false);
}

void MSFloor::CheckInValidElements()
{
	map<long, MSCompoG*> FixCompoGMap;
	for (map<long, MSCompoG*>::iterator itr = mm_CompoG.begin(); itr != mm_CompoG.end(); ++itr)
	{
		MSCompoG* pCompoG = itr->second;
		MSModelChecker::CheckElement(pCompoG);
	}
}

bool MSFloor::IsRoof()
{
	if (GetUpperFloor() == nullptr)	return true;
	return false;
}


void MSFloor::RecoverEachCompoMCustomProfile()
{
	MSBaseBuilding* pBuilding = GetBuilding();
	for (auto it = mm_CompoG.begin(); it != mm_CompoG.end(); it++)
	{
		MSCompoG* pCompoG = it->second;
		MSCompoM* pCompoM = pCompoG->GetCompoM();
		if (pCompoM == nullptr)
			continue;
		if (pCompoM->m_ID == 0 && pCompoM->IsCustomM())
		{
			MSCustomM* pCustomM = (MSCustomM*)pCompoM;
			MSCustomSection* pSection = pBuilding->GetCustomSection(pCustomM->GetSectionID());
			pCustomM->SetMSCustomSection(pSection);
		}
	}
}


map<MSElement::Type_MSCompoG, vector<MSCompoG*>> MSFloor::GetCompGTypeListMap()
{
	map<MSElement::Type_MSCompoG, vector<MSCompoG*>> CompoGTypeMap;
	//Type_MSCompoG
	for (auto it = mm_CompoG.begin(); it != mm_CompoG.end(); it++)
	{
		MSCompoG* pCompoG = it->second;
		Type_MSCompoG nGType = pCompoG->GetType();
		auto Findit = CompoGTypeMap.find(nGType);
		if (Findit == CompoGTypeMap.end())
		{
			vector<MSCompoG*> CompoGList;
			CompoGList.push_back(pCompoG);
			CompoGTypeMap.insert(make_pair(nGType, CompoGList));
		}
		else
			Findit->second.push_back(pCompoG);
	}

	return CompoGTypeMap;
}

map<CString, tuple<double, double>> MSFloor::GetCompoGsTypeGeoData(map<MSElement::Type_MSCompoG, vector<MSCompoG*>> CompoGTypeMap)
{
	map<CString, tuple<double, double>> CompoGGeoMap;
	for (auto it = CompoGTypeMap.begin(); it != CompoGTypeMap.end(); it++)
	{
		if (it->second.size() == 0)
			continue;

		double dSunBoundaryLen = 0;
		double dSumLineLen = 0;

		CString strGroupName;
		for (int i = 0; i < it->second.size(); i++)
		{
			MSCompoG* pCompoG = it->second[i];
			if (i == 0)
				strGroupName = pCompoG->GetGroupName();

			if (pCompoG->IsLinkMember())
			{
				MSLinkMemberG* pLinkG = (MSLinkMemberG*)pCompoG;
				//dSumLineLen += pLinkG->GetLength();
				dSumLineLen += pLinkG->mp_LLineTR->Length();
				dSumLineLen += pLinkG->mp_RLineTR->Length();
			}
			GM2DPolyline* pPoly = pCompoG->GetTrimmedBoundary();
			//dSunBoundaryLen += pPoly->GetLength();
			dSunBoundaryLen += pPoly->Area();
		}

		CompoGGeoMap.insert(make_pair(strGroupName, make_tuple(dSunBoundaryLen, dSumLineLen)));
	}
	return CompoGGeoMap;
}

void MSFloor::LoggingCompoGsTypeGeo(map<CString, tuple<double, double>> CompoGGeoMapData)
{
	for (auto it = CompoGGeoMapData.begin(); it != CompoGGeoMapData.end(); it++)
	{
		CString strLog;
		strLog.Format(_T("FloorNum:%3d, Type:%10s, Sum_BoundaryLen:%f, Sum_LineLen:%f"), m_nFloorNum, it->first, get<0>(it->second), get<1>(it->second));
		StatusReporterWrapper::OutputMessage(strLog);
		LoggerWrapper::Log(strLog);
	}
}

void MSFloor::LogningConnections()
{
	for (auto it = mm_CompoG.begin(); it != mm_CompoG.end(); it++)
	{
		MSCompoG* pCompoG = it->second;
		pCompoG->LoggingConnections();
	}
}

BaseRebarTemplate* MSFloor::GetRebarTemplate(RT_MemberType type)
{
	BaseRebarTemplate* rebarTemplate = nullptr;
	MSRebarBuilding* pRebarBuilding = dynamic_cast<MSRebarBuilding*>(mp_Building);
	if (pRebarBuilding == nullptr)	return rebarTemplate;

	int nRebarTemplateID = GetRebarTemplateID();

	if (nRebarTemplateID != -1)
		rebarTemplate = RebarTemplateManager::GetInstance()->GetRebarTemplate(type, nRebarTemplateID);
	else
		rebarTemplate = pRebarBuilding->GetRebarTemplate(type);

	return rebarTemplate;
}

CString MSFloor::GetRebarTemplateName()
{
	CString sRebarTemplaeName = RebarTemplateManager::GetInstance()->GetNoneRebarTemplateName();

	int nRebarTemplateID = GetRebarTemplateID();

	if (nRebarTemplateID != -1)
		sRebarTemplaeName = RebarTemplateManager::GetInstance()->GetRebarTemplateByID(nRebarTemplateID)->m_sName;

	return sRebarTemplaeName;
}

void MSFloor::SetRebarTemplateName(CString sName)
{
	if (sName == RebarTemplateManager::GetInstance()->GetNoneRebarTemplateName())
		m_nRebarTemplateID = -1;
	else
	{
		RebarTemplate* pRebarTemplate = RebarTemplateManager::GetInstance()->GetRebarTemplateByName(sName);
		m_nRebarTemplateID = pRebarTemplate->m_nID;
	}
}

int MSFloor::GetRebarTemplateID()
{
	RebarTemplate* rebarTemplate = RebarTemplateManager::GetInstance()->GetRebarTemplateByID(m_nRebarTemplateID);
	if (rebarTemplate == nullptr)
		m_nRebarTemplateID = -1;

	return m_nRebarTemplateID;
}

FormWorkTemplate* MSFloor::GetFormWorkTemplate()
{
	FormWorkTemplate* formworkTemplate = nullptr;

	int nFormWorkTemplateID = GetFormWorkTemplateID();

	if (nFormWorkTemplateID != -1)
		formworkTemplate = FormWorkTemplateManager::GetInstance()->GetFormWorkTemplateByID(nFormWorkTemplateID);
	else
		formworkTemplate = mp_Building->GetFormWorkTemplate();

	return formworkTemplate;
}

CString MSFloor::GetFormWorkTemplateName()
{
	CString sFormWorkTemplaeName = FormWorkTemplateManager::GetInstance()->GetNoneFormWorkTemplateName();

	int nFormWorkTemplateID = GetFormWorkTemplateID();

	if (nFormWorkTemplateID != -1)
		sFormWorkTemplaeName = FormWorkTemplateManager::GetInstance()->GetFormWorkTemplateByID(nFormWorkTemplateID)->m_sName;

	return sFormWorkTemplaeName;
}

void MSFloor::SetFormWorkTemplateName(CString sName)
{
	if (sName == FormWorkTemplateManager::GetInstance()->GetNoneFormWorkTemplateName())
		m_nFormWorkTemplateID = -1;
	else
	{
		FormWorkTemplate* pFormWorkTemplate = FormWorkTemplateManager::GetInstance()->GetFormWorkTemplateByName(sName);
		m_nFormWorkTemplateID = pFormWorkTemplate->m_nID;
	}
}

int MSFloor::GetFormWorkTemplateID()
{
	FormWorkTemplate* formworkTemplate = FormWorkTemplateManager::GetInstance()->GetFormWorkTemplateByID(m_nFormWorkTemplateID);
	if (formworkTemplate == nullptr)
		m_nFormWorkTemplateID = -1;

	return m_nFormWorkTemplateID;
}

void MSFloor::WriteLogInfo()
{
	CString msg;
	msg.Format(L"[MSFloor Info] \tFloorNum:%d \tFloorHeight:%f \tMSCompoGCount:%d", m_nFloorNum, m_dHeight, mm_CompoG.size());

	LoggerWrapper::Log(msg);
}

CString MSFloor::ToString()
{
	CString log;
	log.Format(L"FloorNum:%d, Name:%s, Height:%f", m_nFloorNum, GetName(), GetHeight());
	return log;
}

void MSFloor::SetDefFloorTypeNClassification()
{
	
	m_FloorType = LocalDic::GetLocal(L"일반층", nullptr);
	if (m_nFloorNum < 0)
		m_FloorClassification = LocalDic::GetLocal(L"지하층", nullptr);
	else
		m_FloorClassification = LocalDic::GetLocal(L"지상층", nullptr);
}

MSPile* MSFloor::CreatePile()
{
	MSPile* pile = new MSPile();
	pile->SetFloor(this);
	return pile;
}
void MSFloor::Add(MSPile* pile)
{
	long nID = pile->GetID();
	mm_Pile.insert(make_pair(nID, pile));
}
void MSFloor::Remove(MSPile* pile)
{
	auto it = mm_Pile.find(pile->m_ID);
	if (it != mm_Pile.end())
		mm_Pile.erase(it);
}
void MSFloor::Delete(MSPile* pile)
{
	Remove(pile);
	delete pile;
}
MSPile* MSFloor::GetPile(long nID)
{
	auto it = mm_Pile.find(nID);
	if (it != mm_Pile.end())	return it->second;
	return nullptr;
}

bool MSFloor::IsOverlapPile(MSPile* pile)
{
	for (auto it = mm_Pile.begin(); it != mm_Pile.end(); it++)
	{
		if (pile->GetProfile()->OverlapEx_Acis(*it->second->GetProfile()))
			return true;
	}
	return false;
}

void MSFloor::Convert_UType_Floor()
{
	auto lowFloor = GetLowerFloor();
	if (lowFloor == nullptr)	return;

	vector<int> movedIDList;
	for (auto it = mm_CompoG.begin(); it != mm_CompoG.end(); it++)
	{
		auto compoG = it->second;
		if (compoG->IsVerticalMember())	continue;

		lowFloor->mm_CompoG.insert(make_pair(compoG->m_ID, compoG));
		movedIDList.push_back(compoG->m_ID);
	}

	for (int i = 0; i < movedIDList.size(); i++)
	{
		auto Id = movedIDList[i];
		auto it = mm_CompoG.find(Id);
		if (it != mm_CompoG.end())
			mm_CompoG.erase(it);
	}
}

vector<int> MSFloor::GetCompoGIDInFloorQT(GM2DRectangle* pRect)
{
	CTypedPtrArray<CObArray, MSFloorQT*> crossQTArr;
	mp_QT->FindCrossQTs(pRect, crossQTArr, false);

	vector<int> gIDArr;

	for (int idx = 0; idx < crossQTArr.GetSize(); idx++)
	{
		for (int jdx = 0; jdx < crossQTArr[idx]->ma_FloorMem.GetSize(); jdx++)
			gIDArr.push_back(crossQTArr[idx]->ma_FloorMem[jdx]->m_ID);

		for (int jdx = 0; jdx < crossQTArr[idx]->ma_VerticalMem.GetSize(); jdx++)
			gIDArr.push_back(crossQTArr[idx]->ma_VerticalMem[jdx]->m_ID);
	}

	sort(gIDArr.begin(), gIDArr.end(),
		[](const int& a, const int& b)->bool {return a < b; }
	);

	vector<int>::iterator new_end;
	new_end = unique(gIDArr.begin(), gIDArr.end());
	gIDArr.erase(new_end, gIDArr.end());

	return gIDArr;
}

PlacementDirectionType MSFloor::GetPlacementDirectionType()
{
	return m_ePlacementDirectionType;
}
void MSFloor::SetPlacementDirectionType(PlacementDirectionType value)
{
	m_ePlacementDirectionType = value;
}

void MSFloor::FixCompoGProfile()
{
	map<long, MSCompoG*> polyMemberMap;
	map<long, MSCompoG*>::iterator itMem;
	for (itMem = mm_CompoG.begin(); itMem != mm_CompoG.end(); itMem++)
	{
		MSCompoG* pMem = itMem->second;
		if (pMem->IsPolyMember())
		{
			pMem->FixProfile();
			polyMemberMap.insert(make_pair(itMem->first, itMem->second));
		}
	}
	
	UpdateCompoGs(polyMemberMap, false);
}

vector<MSCompoG*> MSFloor::FixGeometry()
{
	return FixGeometry(mm_CompoG);
}

vector<MSCompoG*> MSFloor::FixGeometry(map<long, MSCompoG*> CompoGMap)
{
	vector<MSCompoG*> fixCompoGArr;
	map<long, MSCompoG*>::iterator itCompoG;
	for (itCompoG = CompoGMap.begin(); itCompoG != CompoGMap.end(); itCompoG++)
	{
		MSCompoG* pCompoG = itCompoG->second;
		if (pCompoG == NULL) continue;
		
		pCompoG->DeleteTrimmedBoundary_Acis();
		bool isFix = pCompoG->FixGeometry();
		if (isFix)
			fixCompoGArr.push_back(pCompoG);
	}

	return fixCompoGArr;
}