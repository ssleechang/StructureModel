#include "StdAfx.h"
#include "MSBeamG.h"
#include "MSRcLineMemberM.h"
#include "MSRebarBuilding.h"
#include "MSFloor.h"
#include "GMLib/MSVersionInfo.h"
#include "MSMemberLevelDif.h"
#include "MSRcBeamM.h"
#include "MDSrcBeamM.h"
#include "MSSteelBeamM.h"
#include "MSSrcBeamM.h"

#include "MSSteelUtility.h"
#include "MSSteelSection.h"
#include "MSBuiltUpBeamG.h"
#include "MSConnection.h"
#include "MSRcFWBeamM.h"

#include <math.h>

#include "GMLib/MSVersionInfo.h"
#include "Radfcore_CLI/AppGlobalOptionWrapper.h"

IMPLEMENT_SERIAL(MSBeamG, MSLineMemberG,  VERSIONABLE_SCHEMA | 1)
MSBeamG::MSBeamG()
{
	Init(true);
}

MSBeamG::~MSBeamG(void)
{
	DeleteBuiltGs();
}


void MSBeamG::Dump(EFS::EFS_Buffer& buffer)
{
	MSLineMemberG::Dump(buffer);
	buffer << m_bSubBeam;

	m_StartCondition.Dump(buffer);
	m_EndCondition.Dump(buffer);

	int NumBuiltUp = mm_BuiltUp.size();
	buffer << NumBuiltUp;
	map<long, MSBuiltUpBeamG*>::iterator it;
	for(it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
	{
		it->second->SetOwnerCompoG(this);
		it->second->SetOwner(GetOwner());
		it->second->Dump(buffer);
	}
}

void MSBeamG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSLineMemberG::Recovery(buffer);
	buffer >> m_bSubBeam;

	m_StartCondition.Recovery(buffer);
	m_EndCondition.Recovery(buffer);

	int NumBuiltUp = 0;
	buffer >> NumBuiltUp;
	mm_BuiltUp.clear();
	for(int i = 0; i < NumBuiltUp; i++)
	{
		MSBuiltUpBeamG* pBuiltUp = new MSBuiltUpBeamG();
		pBuiltUp->SetOwnerCompoG(this);
		pBuiltUp->SetOwner(GetOwner());
		pBuiltUp->Recovery(buffer);
		mm_BuiltUp.insert(make_pair(pBuiltUp->m_ID, pBuiltUp));
	}
}

void MSBeamG::UpdateTrimmedBoundary(long nUpdateConnIdx /*= 0*/)
{
	MSCompoG::UpdateTrimmedBoundary(nUpdateConnIdx);
}

void MSBeamG::UpdateTrimmedBoundary_AfterCalculate(CON_POS ePos)
{
	vector<MSConnection*> BeamConnArr;
	for (long iConn = 0; iConn < ma_Connections.size(); iConn++)
	{
		MSConnection* pConn = ma_Connections[iConn];
		MSCompoG* pPartnerG = pConn->GetPartner(this);
		if (pPartnerG == nullptr)
			continue;
		if (pPartnerG->GetType() == MSElement::msBeamG || pPartnerG->IsVerticalMember())
			BeamConnArr.push_back(pConn);
	}

	map<long, MSBuiltUpBeamG*>::iterator it;
	for (it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
	{
		MSBuiltUpBeamG* pBuiltUpBeam = it->second;
		pBuiltUpBeam->MakeBoundary();
		pBuiltUpBeam->TrimInitialize();
		pBuiltUpBeam->ma_Connections.clear();

		for (int iBeamConn = 0; iBeamConn < BeamConnArr.size(); iBeamConn++)
		{
			MSConnection* pConn = BeamConnArr[iBeamConn];
			vector<GM2DPolyline*> InterSectArr = pConn->GetIntersects();
			for (int i = 0; i < InterSectArr.size(); i++)
			{
				GM2DPolyline* pInterPoly = InterSectArr[i];
				long nRval = pBuiltUpBeam->GetProfile()->OverlapEx(*pInterPoly);
				if (nRval > 0)
				{
					pBuiltUpBeam->MergeIntersectToBoundary(pConn);
					pBuiltUpBeam->AddConnection(pConn);
				}
			}
		}

		pBuiltUpBeam->RemoveOutwardWedge();
		GM2DPolyline* pBuiltUpBoundary = pBuiltUpBeam->GetTrimmedBoundary();
		pBuiltUpBoundary->DeleteInLinePoint();
	}
}

void MSBeamG::UpdateBoundaryVisibilities(long nUpdateConnIdx /*= 0*/)
{
	long nOverlap = this->ma_Connections.size();
	for (long iOverlap = nUpdateConnIdx; iOverlap < nOverlap; iOverlap++)
	{
		MSConnection* pConnection = this->ma_Connections[iOverlap];
		MSCompoG* pPartnerG = pConnection->GetPartner(this);
		if (pPartnerG == NULL)
		{
			ASSERT(0);
			continue;
		}
		if (pPartnerG->IsPolyMember()) continue;

		GM2DPolyline* pMyBoundary = this->GetTrimmedBoundary();
		GM2DPolyline* pUrBoundary = pPartnerG->GetTrimmedBoundary();
		double dTol = DTOL_MODELING;

		pMyBoundary->SegmentstoMe(pUrBoundary, dTol);
	}
	
	GM2DPolyline* pMyBoundary = this->GetTrimmedBoundary();

	map<long, MSBuiltUpBeamG*>::iterator it;
	for (it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
	{
		MSBuiltUpBeamG* pBuiltUpBeam = it->second;
		pBuiltUpBeam->UpdateBoundaryVisibilities();

		GM2DPolyline* pUrBoundary = pBuiltUpBeam->GetTrimmedBoundary();
		//double dTol = DTOL_GM;
		//pMyBoundary->SegmentstoMe(pUrBoundary, dTol);

		double dTol = DTOL_GM;
		if (pBuiltUpBeam->GetConnectionByPos(CON_START).size() == 0)
		{
			GM2DLineVector SLine(pBuiltUpBeam->mp_LLineTR->m_P0, pBuiltUpBeam->mp_RLineTR->m_P0);
			pMyBoundary->SegmentstoMe(&SLine, dTol);
		}
		if (pBuiltUpBeam->GetConnectionByPos(CON_END).size() == 0)
		{
			GM2DLineVector ELine(pBuiltUpBeam->mp_LLineTR->m_P1, pBuiltUpBeam->mp_RLineTR->m_P1);
			pMyBoundary->SegmentstoMe(&ELine, dTol);
		}
	}

	pMyBoundary->DeleteDuplicatedPoints(DTOL_MODELING);
	InitTrimmedBoundaryVisibility();

	for (long iOverlap = nUpdateConnIdx; iOverlap < nOverlap; iOverlap++)
	{
		MSConnection* pConnection = this->ma_Connections[iOverlap];
		MSCompoG* pPartnerG = pConnection->GetPartner(this);
		if (pPartnerG == NULL)
		{
			ASSERT(0);
			continue;
		}
		if (pPartnerG->IsPolyMember()) continue;

		MSLinkMemberG::UpdateBoundaryVisibilities(pPartnerG, pConnection);
	}
	
	MSCompoG::UpdateBoundaryVisibilities(nUpdateConnIdx);
}

bool MSBeamG::GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal, bool bCheckLevelZone)
{
	MSLinkMemberG::GetZLevels(TopLevel, BottomLevel, GetBigVal, bCheckLevelZone);

	double dTopBuiltUpBeamHeight = 0, dBotBuiltUpBeamHeight = 0;
	map<long, MSBuiltUpBeamG*>::iterator it;
	for (it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
	{
		MSBuiltUpBeamG* pBuiltUpBeamG = it->second;
		if (pBuiltUpBeamG->GetIsTop())
			dTopBuiltUpBeamHeight = max(dTopBuiltUpBeamHeight, pBuiltUpBeamG->GetHeight());
		else
			dBotBuiltUpBeamHeight = max(dBotBuiltUpBeamHeight, pBuiltUpBeamG->GetHeight());
	}
	TopLevel += dTopBuiltUpBeamHeight;
	BottomLevel -= dBotBuiltUpBeamHeight;
	
	MSRcFWBeamM* pMSRcFWBeamM = dynamic_cast<MSRcFWBeamM*>(GetCompoM());
	if (pMSRcFWBeamM != nullptr && pMSRcFWBeamM->IsReversBeamM())
	{
		MSFloor* pCurFloor = GetFloor();
		double dSlabHeight = pCurFloor->GetSlabHeight();
		double dMaxH = pMSRcFWBeamM->GetMaxH();
		TopLevel += -dSlabHeight + dMaxH;
		BottomLevel += -dSlabHeight + dMaxH;
	}

	return true;
}

bool MSBeamG::GetZLevelsbyLoc(double& TopLevel, double& BottomLevel, double dLoc /*= 0.5*/)
{
	MSLinkMemberG::GetZLevelsbyLoc(TopLevel, BottomLevel, dLoc);
	
	// dLoc가 0~1 넘는 경우가 발생할수 있음, 빌트업인 경우 0~1 사이로 보정해준다.
	if (dLoc < 0) dLoc = 0;
	if (dLoc > 1) dLoc = 1;

	map<long, MSBuiltUpBeamG*>::iterator it;
	for (it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
	{
		MSBuiltUpBeamG* pBuiltUpBeamG = it->second;
		if (pBuiltUpBeamG->IsContainLoc(dLoc))
		{
			if (pBuiltUpBeamG->GetIsTop())
				TopLevel += pBuiltUpBeamG->GetHeight();
			else
				BottomLevel -= pBuiltUpBeamG->GetHeight();
		}
	}

	MSRcFWBeamM* pMSRcFWBeamM = dynamic_cast<MSRcFWBeamM*>(GetCompoM());
	if (pMSRcFWBeamM != nullptr && pMSRcFWBeamM->IsReversBeamM())
	{
		MSFloor* pCurFloor = GetFloor();
		double dSlabHeight = pCurFloor->GetSlabHeight();
		double dMaxH = pMSRcFWBeamM->GetMaxH();
		TopLevel += -dSlabHeight + dMaxH;
		BottomLevel += -dSlabHeight + dMaxH;
	}

	return true;
}

bool MSBeamG::GetZLevelsOnlyBeam(double& TopLevel, double& BottomLevel, bool GetBigVal, bool bCheckLevelZone)
{
	return MSLinkMemberG::GetZLevels(TopLevel, BottomLevel, GetBigVal, bCheckLevelZone);
}

void MSBeamG::Serialize(CArchive &ar)
{
	MSLineMemberG::Serialize(ar);
	bool bEachCompoM = false;
	if(ar.IsStoring())
	{
		ar << m_bSubBeam;
		
		if(mp_CompoM)
		{
			if(mp_CompoM->m_ID == 0)
				bEachCompoM = true;

			ar << bEachCompoM;
			if(bEachCompoM == true)
			{
				Serialize_EachCompoM(ar);
			}
		}
		m_StartCondition.Serialize(ar);
		m_EndCondition.Serialize(ar);

		int NumBuiltUp = mm_BuiltUp.size();
		ar << NumBuiltUp;
		map<long, MSBuiltUpBeamG*>::iterator it;
		for(it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
		{
			it->second->Serialize(ar);
		}
	}
	else
	{
		ar >> m_bSubBeam;
		
		ar >> bEachCompoM;
		if(bEachCompoM)
		{
			Serialize_EachCompoM(ar);
		}

		if (MSVersionInfo::GetCurrentVersion() <= 20140807)
		{
			m_EOffsetZ = m_SOffsetZ;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20150722)
		{
			m_StartCondition.Serialize(ar);
			m_EndCondition.Serialize(ar);
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20160809)
		{
			int NumBuiltUp = 0;
			ar >> NumBuiltUp;
			for(int i = 0; i < NumBuiltUp; i++)
			{
				MSBuiltUpBeamG* pBuiltUp = new MSBuiltUpBeamG();
				pBuiltUp->SetOwnerCompoG(this);
				pBuiltUp->SetOwner(GetOwner());
				pBuiltUp->Serialize(ar);
				//AddBuiltUpBeamG(pBuiltUp, false);
				pBuiltUp->SetOffSetInfo();
				pBuiltUp->SetDummy(this->GetDummy());
				mm_BuiltUp.insert(make_pair(pBuiltUp->m_ID, pBuiltUp));
			}
		}

		if (MSVersionInfo::GetCurrentVersion() < 20181127)
		{
			SubMemberMaterialValidCheck();
		}
	}
}

void MSBeamG::Serialize_EachCompoM_Rebar(CArchive& ar)
{
	if (ar.IsStoring())
	{
		long nType = mp_CompoM->m_nType;
		ar << nType;
		mp_CompoM->Serialize(ar);
	}
	else
	{
		if ((MSObject::IsBHQorBHCEMode() && (MSVersionInfo::GetCurrentVersion() < 20220321)
			|| (MSObject::IsBHForBHAMode()) && MSVersionInfo::GetCurrentVersion() < 20240307))
		{
			MSBaseBuilding* pBldg = GetBuilding();
			long MType = 0;
			ar >> MType;

			MSCompoM* pCompoM = pBldg->CreateNewCompoM(MType);
			pCompoM->m_nType = (MSCompoM::Type_MSCompoM)MType;

			long nMaterialID = 0;
			ar >> nMaterialID;
			pCompoM->SetMaterial(pBldg->GetMaterial(nMaterialID));
			ar >> pCompoM->m_Name;

			if (pCompoM->GetType() == MSCompoM::msBeamM || pCompoM->GetType() == MSCompoM::msSrcBeamM)
			{
				MSRcLineMemberM* pLineM = (MSRcLineMemberM*)pCompoM;
				double dWidth = 0, dDepth = 0;
				ar >> dWidth;
				ar >> dDepth;
				pLineM->SetWidth(dWidth);
				pLineM->SetDepth(dDepth);

				if (pCompoM->GetType() == MSCompoM::msSrcBeamM)
				{
					long sectionType = 0, sectionID = 0;
					ar >> sectionType;
					ar >> sectionID;
					MSSrcBeamM* pSrcBeamM = (MSSrcBeamM*)pCompoM;
					pSrcBeamM->m_nSecType = sectionType;
					MSRebarBuilding* pRebarBuilding = dynamic_cast<MSRebarBuilding*>(GetBuilding());
					if(pRebarBuilding != nullptr)
						pSrcBeamM->m_pSection = pRebarBuilding->GetSteelUtili()->GetSection(sectionType, sectionID);
				}
			}
			else if (pCompoM->GetType() == MSCompoM::msSteelBeamM)
			{
				long sectionType = 0, sectionID = 0;
				ar >> sectionType;
				ar >> sectionID;
				MSSteelBeamM* pSteelBeamM = (MSSteelBeamM*)pCompoM;
				pSteelBeamM->m_nSecType = sectionType;
				MSRebarBuilding* pRebarBuilding = dynamic_cast<MSRebarBuilding*>(GetBuilding());
				if (pRebarBuilding != nullptr)
					pSteelBeamM->m_pSection = pRebarBuilding->GetSteelUtili()->GetSection(sectionType, sectionID);
			}
			pCompoM->mp_Owner = pBldg;
			mp_CompoM = pCompoM;
		}
		else
		{
			long nType = 0;
			ar >> nType;
			mp_CompoM = GetBuilding()->CreateNewCompoM(nType);
			mp_CompoM->Serialize(ar);
		}
	}
}

MSObject* MSBeamG::Copy( bool bNewID /*= true*/ )
{
	MSBeamG* pNewObj;

	pNewObj = new MSBeamG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSBeamG::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;
	if( !pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() != msBeamG) { ASSERT(0); return; }
	MSLineMemberG::CopyFromMe(pSource_in, bNewID);
	
	MSBeamG* pSource = (MSBeamG*)pSource_in;

	if(pSource)
	{
		m_bSubBeam = pSource->m_bSubBeam;
	}

	m_StartCondition.CopyFromMe(&pSource->m_StartCondition);
	m_EndCondition.CopyFromMe(&pSource->m_EndCondition);

	DeleteBuiltGs();
	map<long, MSBuiltUpBeamG*>::iterator it;
	for(it = pSource->mm_BuiltUp.begin(); it != pSource->mm_BuiltUp.end(); it++)
	{
		MSBuiltUpBeamG* pMyBuilt = new MSBuiltUpBeamG();
		pMyBuilt->CopyFromMe(it->second, bNewID);
		pMyBuilt->SetOwnerCompoG(this);
		pMyBuilt->SetOwner(GetOwner());
		pMyBuilt->GetID();
		mm_BuiltUp.insert(make_pair(pMyBuilt->m_ID, pMyBuilt));
	}
}

void MSBeamG::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSLineMemberG::Init(bInConstructor);
	m_bSubBeam = false;
}

double MSBeamG::GetThick()
{
    return MSLinkMemberG::GetThick();
}

bool MSBeamG::IsPC()
{

	if(mp_CompoM==NULL)
		return false;

	if(mp_CompoM->GetType() == MSCompoM::msPcBeamM)
		return true;

	return false;

}

double MSBeamG::GetDevLen(CON_POS ePos, bool bOnly2DShop)
{
	// bool bOnly2DShop 사용
	// 2DShop Box용 : Box에서 사용되는 정착 길이는 기둥이 있을 경우에 기둥에 우선 정착한다. 기둥 없고 벽이 평행할 경우 벽의 두께로 처리
	//독립기초 체크
	double dBeamTopLevel = 0;
	double dBeamBotLevel = 0;
	if (ePos == CON_START)
		GetZLevelsbyLoc(dBeamTopLevel, dBeamBotLevel, 0);
	else if (ePos == CON_END)
		GetZLevelsbyLoc(dBeamTopLevel, dBeamBotLevel, 1);
	else
		return 0;

	eFIND_FLOOR eFloorType = FIND_ALLFLOOR;
	//부재가 옵셋을 가지고 있어 자신의 층Level을 넘어서 존재할 경우 전층을 검색한다.
	//MSFloor* pFloor = GetFloor();
	//if (pFloor != NULL)
	//{
	//	double dBotFloorLevel = pFloor->GetSlabLevel();
	//	double dTopFloorLevel = dBotFloorLevel + pFloor->GetSlabHeight();
	//	if (dBotFloorLevel > dBeamBotLevel || dTopFloorLevel < dBeamTopLevel)
	//		eFloorType = FIND_ALLFLOOR;
	//}

	GM2DVector CheckVec;
	GM2DCurve* pBaseCurve = NULL;
	GM2DCurve* pLongCurve = GetLongLVec(true);
	GM2DCurve* pLBaseCurve = mp_LLineTR->Copy();
	GM2DCurve* pRBaseCurve = mp_RLineTR->Copy();
	shared_ptr<GM2DCurve> pCenCurve = GetCenCurve();
	GM2DCurve* pUsedCenCurve = pCenCurve->Copy();
	double dLLoc = 0;
	double dRLoc = 0;
	if (ePos == CON_START)
	{
		if (pCenCurve->m_ObjType == GT_ARC)
			pBaseCurve = pCenCurve->Copy();
		else
		{
			dLLoc = pCenCurve->OrthoBaseLocValue(mp_LLineTR->m_P0);
			dRLoc = pCenCurve->OrthoBaseLocValue(mp_RLineTR->m_P0);

			if (dLLoc < dRLoc)
				pBaseCurve = mp_RLineTR->Copy();
			else
				pBaseCurve = mp_LLineTR->Copy();
		}
		CheckVec = pBaseCurve->m_P0;
	}
	else if (ePos == CON_END)
	{
		if (pCenCurve->m_ObjType == GT_ARC)
			pBaseCurve = pCenCurve->Copy();
		else
		{
			dLLoc = pCenCurve->OrthoBaseLocValue(mp_LLineTR->m_P1);
			dRLoc = pCenCurve->OrthoBaseLocValue(mp_RLineTR->m_P1);
			if (dLLoc > dRLoc)
				pBaseCurve = mp_RLineTR->Copy();
			else
				pBaseCurve = mp_LLineTR->Copy();
		}
		CheckVec = pBaseCurve->m_P1;
	}

	double dBaseCurveLen = pBaseCurve->Length();
	if (dBaseCurveLen < DTOL_GM)
		return 0;

	double dExtendRatio = 100000 / pBaseCurve->Length();
	if (ePos == CON_START)
	{
		GM2DVector SVec = pBaseCurve->PointAtParam(-dExtendRatio);
		pBaseCurve->SetP0(SVec);
		SVec = pUsedCenCurve->PointAtParam(-dExtendRatio);
		pUsedCenCurve->SetP0(SVec);

		SVec = pLBaseCurve->PointAtParam(-dExtendRatio);
		pLBaseCurve->SetP0(SVec);
		SVec = pRBaseCurve->PointAtParam(-dExtendRatio);
		pRBaseCurve->SetP0(SVec);
	}
	else if (ePos == CON_END)
	{
		GM2DVector EVec = pBaseCurve->PointAtParam(1 + dExtendRatio);
		pBaseCurve->SetP1(EVec);
		EVec = pUsedCenCurve->PointAtParam(1 + dExtendRatio);
		pUsedCenCurve->SetP1(EVec);

		EVec = pLBaseCurve->PointAtParam(1 + dExtendRatio);
		pLBaseCurve->SetP1(EVec);
		EVec = pRBaseCurve->PointAtParam(1 + dExtendRatio);
		pRBaseCurve->SetP1(EVec);
	}

	// 트림된 LR 라인중 짧은 선에 히트 되는 부재에서 정착길이를 계산한다.

	double DistTol = DTOL_GM;
	double dTolDeg = MSFloor::ms_ColinearDegreeTol;
	double LinkTestLength = GetLength()/10;
	double dUrP0Loc, dUrP1Loc;
	
	// 기둥이 존재 하면 평행한 벽체는 찾지 않는다.
	bool HasColumn = false;
	bool HasParallelWall = false;
	double dTopLevel = 0;
	double dBotLevel = 0;

	MSCompoG* pColumnG = NULL;
	vector<MSCompoG*> ContainCompoGArr;
	vector<MSCompoG*> CompoGArr = FindConnectMember(ePos, FindType_MSElement::msNULL, eFloorType);
	for (long iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
	{
		MSCompoG* pCompoG = CompoGArr[iCompoG];
		bool isPolyMember = pCompoG->IsPolyMember();
		if (isPolyMember)
		{
			if (!( !this->IsTransfer() && !this->IsDeck() && pCompoG->GetType() == MSElement::msSlabG && pCompoG->IsTransfer()))
				continue;
		}

		//if (pCompoG->IsPolyMember() && !(pCompoG->GetType() == MSElement::msSlabG && pCompoG->IsTransfer()))
		//	continue;
		
		//���� TopLevel, BotLevel�� ���°� ����� ����� ���� ���� ����ϴ� ���翡�� ���ܽ�Ų��.
		pCompoG->GetZLevels(dTopLevel, dBotLevel);
		if (dTopLevel == dBeamBotLevel || dBotLevel == dBeamTopLevel)
			continue;

		//IsoFooting LevelCheck
		if(pCompoG->GetType()==MSElement::msIsoFootG || pCompoG->GetType() == MSElement::msSlabG)
		{
			pCompoG->GetZLevels(dTopLevel,dBotLevel);
			if(dTopLevel+DTOL1 < dBeamTopLevel)	continue;
			if(dBotLevel-DTOL1 > dBeamBotLevel) continue;
		}

		if (pCompoG->IsLinkMember())
		{
			MSLinkMemberG* pUrLinkG = (MSLinkMemberG*)pCompoG;
			if(IsLevelOverlap(pUrLinkG) == false)
				continue;
			Type_MSCompoG eType = pUrLinkG->GetType();
			bool isParallel = IsParallel(pUrLinkG);
			if (isParallel)
			{
				if (eType == MSElement::msBeamG)
					continue;
				else if (eType == MSElement::msWallG)
				{
					if (HasParallelWall == false)
						HasParallelWall = true;
					shared_ptr<GM2DCurve> pUrCenCurve = pUrLinkG->GetCenCurve();

					if (isArc() && pUrLinkG->isArc())
						DistTol = max(GetWidth(), pUrLinkG->GetWidth()) + DTOL_MODELING;

					long res = pCenCurve->IsParallel4DTol(&*pUrCenCurve, dTolDeg);
					if (res != 0)
					{
						//벽의 두께가 보폭보다 작은 경우 제외
						if(pUrLinkG->GetWidth()+DTOL1 < GetWidth())	continue;
						//DistTol 변경
						if (isArc() && pUrLinkG->isArc())
							DistTol = max(GetWidth(), pUrLinkG->GetWidth()) + DTOL_MODELING;
						else
							DistTol = GetWidth() / 2 + pUrLinkG->GetWidth() / 2;

						if (pUrCenCurve->Length() <= LinkTestLength)
							LinkTestLength = pUrCenCurve->Length()*0.2;
						long res = pCenCurve->Overlap4DTol(&*pUrCenCurve, dUrP0Loc, dUrP1Loc, dTolDeg, DistTol, LinkTestLength);
						//Overlap 이면 continue;
						if (!(res == 1 || res == 2 || res == 6 || res == 7 || res == 19 || res == 20 || res == 24 || res == 25))
							continue;
					}

				}
			}
		}
		GM2DPolyline* pCompoGPoly = pCompoG->GetTrimmedBoundary();
		if (pCompoGPoly->OverlapEx(pLongCurve, DTOL_MODELING) > 0)
		{
			if (pCompoG->GetType() == MSElement::msColumnG)
			{
				HasColumn = true;
				pColumnG = pCompoG;
			}
			ContainCompoGArr.push_back(pCompoG);
		}
	}
	delete pLongCurve;
	pLongCurve = nullptr;

	if (ContainCompoGArr.size() == 0)
		return 0;

	// 연장된 LRLine에 교차되는 기둥이 존재할 경우 기둥의 폴리라인으로 계산함
	if (pColumnG != NULL)
	{
		GM2DPolyline* pColumnPoly = pColumnG->GetTrimmedBoundary();
		long nLRval = pColumnPoly->OverlapEx(pLBaseCurve);
		long nRRval = pColumnPoly->OverlapEx(pRBaseCurve);
		if (nLRval > 0 && nRRval > 0)
		{
			if (pColumnPoly->GetCurveInPolygon(pBaseCurve, DTOL_MODELING))
			{
				//double dLen = pBaseCurve->Length();
				double dLen = max(CheckVec.Distance(pBaseCurve->m_P0), CheckVec.Distance(pBaseCurve->m_P1));
				delete pBaseCurve;
				delete pUsedCenCurve;
				delete pLBaseCurve;
				delete pRBaseCurve;
				return dLen;
			}
			else if (pColumnPoly->GetCurveInPolygon(pUsedCenCurve, DTOL_MODELING))
			{
				double dLen = pUsedCenCurve->Length();
				delete pBaseCurve;
				delete pUsedCenCurve;
				delete pLBaseCurve;
				delete pRBaseCurve;
				return dLen;
			}
		}
	}
	delete pLBaseCurve;
	delete pRBaseCurve;
	pLBaseCurve = pRBaseCurve = NULL;

	if (bOnly2DShop == true && 
		HasParallelWall == true && ContainCompoGArr.size() == 1)
	{
		double dLen = ContainCompoGArr[0]->GetWidth();
		delete pBaseCurve;
		delete pUsedCenCurve;
		return dLen;
	}

	for (long i = ContainCompoGArr.size() -1 ; i >= 0; i--)
	{
		MSCompoG* pICompoG = ContainCompoGArr[i];
		if (pICompoG->GetType() == MSElement::msWallG)
		{
			if (HasColumn == true)
			{
				ContainCompoGArr.erase(ContainCompoGArr.begin() + i);
				continue;
			}

			for (long j = 0; j < ContainCompoGArr.size(); j++)
			{
				MSCompoG* pJCompoG = ContainCompoGArr[j];
				if (pICompoG == pJCompoG)
					continue;

				if (pJCompoG->GetType() == MSElement::msBeamG)
				{
					MSLinkMemberG* pILinkG = (MSLinkMemberG*)pICompoG;
					MSLinkMemberG* pJLinkG = (MSLinkMemberG*)pJCompoG;
					if (pILinkG->IsParallel(pJLinkG))
					{
						ContainCompoGArr.erase(ContainCompoGArr.begin() + i);
						break;
					}
				}
			}
		}
	}

	GM2DPolyline CheckPoly(TRUE);

	for (long i = 0; i < ContainCompoGArr.size(); i++)
	{
		MSCompoG* pCurCompoG = ContainCompoGArr[i];
		if (i == 0)
			CheckPoly.CopyFrom(pCurCompoG->GetTrimmedBoundary());
		else
		{
			vector<GM2DPolyline*> Joined_Polys;
			CheckPoly.BoolOperationNSortByArea('U', pCurCompoG->GetTrimmedBoundary(), Joined_Polys, DTOL_MODELING);
			if (Joined_Polys.size() > 0)
				CheckPoly.CopyFrom(Joined_Polys[0]);
			else
				ASSERT(0);
		}
	}

	if (CheckPoly.GetCurveInPolygon(pBaseCurve, DTOL_MODELING))
	{
		//double dLen = pBaseCurve->Length();
		double dLen = max(CheckVec.Distance(pBaseCurve->m_P0), CheckVec.Distance(pBaseCurve->m_P1));
		delete pBaseCurve;
		delete pUsedCenCurve;
		return dLen;
	}
	else if (CheckPoly.GetCurveInPolygon(pUsedCenCurve, DTOL_MODELING))
	{
		double dLen = pUsedCenCurve->Length();
		delete pBaseCurve;
		delete pUsedCenCurve;
		return dLen;
	}
	
	double dLen = 0;
	for (long i = 0; i < ContainCompoGArr.size(); i++)
	{
		MSCompoG* pCurCompoG = ContainCompoGArr[i];
		dLen = max(dLen, max(pCurCompoG->GetWidth(), pCurCompoG->GetDepth()));
	}

	delete pBaseCurve;
	delete pUsedCenCurve;

	return dLen;
}

bool MSBeamG::HasOverLapWall()
{
	double dDegTol = 2; // MSFloor::ms_ColinearDegreeTol

	vector<MSCompoG*> OverLapWallArr = FindOverlapLinkMember(FindType_MSElement::msWallG, 0, dDegTol, 0, 0, true);
	if(OverLapWallArr.size() > 0)
		return true;
	return false;
}

MSBoundaryCondition::Type_Condition MSBeamG::GetStartBoundaryCondition()
{
	return m_StartCondition.GetCondition();
}

MSBoundaryCondition::Type_Condition MSBeamG::GetEndBoundaryCondition()
{
	return m_EndCondition.GetCondition();
}

void MSBeamG::SetStartBoundaryCondition(MSBoundaryCondition::Type_Condition condition)
{
	m_StartCondition.SetCondition(condition);
}
void MSBeamG::SetEndBoundaryCondition(MSBoundaryCondition::Type_Condition condition)
{
	m_EndCondition.SetCondition(condition);
}

bool MSBeamG::IsCornerPlacing()
{
	if (IsRC())
	{
		MSRcBeamM* pCompoM = (MSRcBeamM*)GetRebarM();
		return pCompoM->GetIsCornerPlacing();
	}
	else if (IsSRC())
	{
		MDSrcBeamM* pCompoM = (MDSrcBeamM*)GetRebarM();
		return pCompoM->GetIsCornerPlacing();
	}

	return false;
}

void MSBeamG::AddBuiltUpBeamG(MSBuiltUpBeamG* pBuiltG, bool bUpdate)
{
	// Add는 StartVec,EndVec 기준으로 입력된 BuiltupBeamg으로 가정한다.
	MSBuiltUpBeamG* pNewBuiltG = (MSBuiltUpBeamG*)pBuiltG->Copy();
	pNewBuiltG->SetOwner(GetOwner());
	pNewBuiltG->SetOwnerCompoG(this);
	pNewBuiltG->SetDummy(this->GetDummy());
	//pNewBuiltG을 계산 해야하는지??
	// 계산한다면 그 기준은 ?? 

	//if (IsAddMode) // BuiltUpBeam을 입력 했을 경우는 Vector 을 기준으로 Loc 값을 셋팅함
		pNewBuiltG->SyncLocationVec2Loc();
// 	else           // Beam을 수정 했을 경우 Loc 값을 기준으로 Vector값을 변경함
// 		pNewBuiltG->SyncLocationLoc2Vec();
	pNewBuiltG->SetLines();
	mm_BuiltUp.insert(make_pair(pNewBuiltG->GetID(), pNewBuiltG));

	if (bUpdate == true)
	{
		MSFloor* pFloor = GetFloor();
		pFloor->FindOverlapMembernUpdateTrimBoundary(this, false);
		vector<MSCompoG*> PartnerCompoGArr = GetPartnerCompoGs();
		for (long iPartner = 0; iPartner < PartnerCompoGArr.size(); iPartner++)
			pFloor->FindOverlapMembernUpdateTrimBoundary(PartnerCompoGArr[iPartner], true);

		UpdateTrimmedBoundary();
		UpdateBoundaryVisibilities();

		pFloor->CalculateUpdatedCompoGArray(this, pFloor->CompoGIDList_NeedRegen_CurTR());
	}
}

void MSBeamG::Remove(long ID)
{
	map<long, MSBuiltUpBeamG*>::iterator it = mm_BuiltUp.find(ID);
	if(it != mm_BuiltUp.end())
		mm_BuiltUp.erase(it);
}

void MSBeamG::DeleteBuiltGs()
{
	map<long, MSBuiltUpBeamG*>::iterator it;
	for(it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
		delete it->second;
	mm_BuiltUp.clear();
}

MSBuiltUpBeamG* MSBeamG::GetBuiltG(long ID)
{
	map<long, MSBuiltUpBeamG*>::iterator it = mm_BuiltUp.find(ID);
	if(it != mm_BuiltUp.end())
		return it->second;
	return NULL;

}

void MSBeamG::SyncBuiltdUpBeamLocationByLoc()
{
	map<long, MSBuiltUpBeamG*>::iterator it;
	for (it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
	{
		MSBuiltUpBeamG* pBuiltUpBeamG = it->second;
		pBuiltUpBeamG->SyncLocationLoc2Vec();
	}
}

void MSBeamG::SyncBuiltdUpBeamStartEndLocationByLoc()
{
	map<long, MSBuiltUpBeamG*>::iterator it;
	for (it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
	{
		MSBuiltUpBeamG* pBuiltUpBeamG = it->second;
		if (pBuiltUpBeamG->GetIsSameStartLocation() == true)
			pBuiltUpBeamG->SyncStartLocationLoc2Vec();

		if (pBuiltUpBeamG->GetIsSameEndLocation() == true)
			pBuiltUpBeamG->SyncEndLocationLoc2Vec();

		if (pBuiltUpBeamG->GetIsSameStartLocation() == true ||
			pBuiltUpBeamG->GetIsSameEndLocation() == true)
			pBuiltUpBeamG->SetLines();
	}
}

bool MSBeamG::SetCompoM(long MID, bool bUpdateVecInput)
{
	bool bRval = MSLinkMemberG::SetCompoM(MID);
	if (bRval == false)
		return false;

	map<long, MSBuiltUpBeamG*>::iterator it;
	for (it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
		it->second->SetLines();
	
	return true;
}

bool MSBeamG::SetCompoM(MSCompoM* pCompoM, bool bUpdateVecInput)
{
	if (pCompoM == NULL)
		return false;

	bool bRval = MSLinkMemberG::SetCompoM(pCompoM);
	if (bRval == false)
		return false;

	return true;
}

void MSBeamG::SetLines()
{
	MSLineMemberG::SetLines();

	map<long, MSBuiltUpBeamG*>::iterator it;
	for (it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
		it->second->SyncLocationVec2Loc();
}

bool MSBeamG::HasBuiltUpBeamG()
{
	if (mm_BuiltUp.size() == 0)
		return false;

	return true;
}

void MSBeamG::DeleteErrorBuiltUpBeam()
{
	if (mm_BuiltUp.size() == 0)
		return;

	GM2DVector CenVec = GetCenVec();
	shared_ptr<GM2DCurve> pCenCurve = GetCenCurve();

	vector<long> DeleteIDs;
	map<long, MSBuiltUpBeamG*>::iterator it;
	for (it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
	{
		MSBuiltUpBeamG* pBuiltUp = it->second;
		GM2DVector CenVec = pBuiltUp->GetCenVec();
		if(pCenCurve->HasOnLine(CenVec, DTOL_GM) == false)
			DeleteIDs.push_back(it->first);
	}

	for (int i = 0; i < DeleteIDs.size(); i++)
	{
		it = mm_BuiltUp.find(DeleteIDs[i]);
		if(it == mm_BuiltUp.end())
			continue;

		MSBuiltUpBeamG* pBuiltUp = it->second;
		mm_BuiltUp.erase(DeleteIDs[i]);
		delete pBuiltUp;		
	}
}

void MSBeamG::RecoverPointerAfterSerialize(MSFloor* floor)
{
	MSCompoG::RecoverPointerAfterSerialize(floor);

	vector<MSConnection*> BeamConnArr;
	for (long iConn = 0; iConn < ma_Connections.size(); iConn++)
	{
		MSConnection* pConn = ma_Connections[iConn];
		MSCompoG* pPartnerG = pConn->GetPartner(this);
		if(pPartnerG == nullptr)
			continue;
		if (pPartnerG->GetType() == MSElement::msBeamG || pPartnerG->IsVerticalMember())
			BeamConnArr.push_back(pConn);
	}

	map<long, MSBuiltUpBeamG*>::iterator it;
	for (it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
	{
		MSBuiltUpBeamG* pBuiltUpBeam = it->second;
		pBuiltUpBeam->ma_Connections.clear();

		for (int iBeamConn = 0; iBeamConn < BeamConnArr.size(); iBeamConn++)
		{
			MSConnection* pConn = BeamConnArr[iBeamConn];
			vector<GM2DPolyline*> InterSectArr = pConn->GetIntersects();
			for (int i = 0; i < InterSectArr.size(); i++)
			{
				GM2DPolyline* pInterPoly = InterSectArr[i];
				long nRval = pBuiltUpBeam->GetProfile()->OverlapEx(*pInterPoly);
				if (nRval > 0)
					pBuiltUpBeam->AddConnection(pConn);
			}
		}
	}
}

void MSBeamG::Translate(GM2DVector &pDelta)
{
	MSLineMemberG::Translate(pDelta);
	for (auto it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
		it->second->Translate(pDelta);
}

void MSBeamG::MirrorByAxis(GM2DLineVector& pAxis)
{
	MSLineMemberG::MirrorByAxis(pAxis);
	for (auto it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
		it->second->MirrorByAxis(pAxis);
}

void MSBeamG::RotateZ(double theta, GM2DVector *pCenter)
{
	MSLineMemberG::RotateZ(theta, pCenter);
	for (auto it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
		it->second->RotateZ(theta, pCenter);
}

long MSBeamG::ResetGetID()
{
	MSCompoG::ResetGetID();
	
	auto TmpBuiltUp = mm_BuiltUp;
	mm_BuiltUp.clear();
	for (auto it = TmpBuiltUp.begin(); it != TmpBuiltUp.end(); it++)
	{
		it->second->ResetGetID();
		mm_BuiltUp.insert(make_pair(it->second->m_ID, it->second));
	}
	return m_ID;
}

void MSBeamG::RemoveConnections(bool bNotRemoveSetByUser)
{
	MSCompoG::RemoveConnections(bNotRemoveSetByUser);
	
	for (auto it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
		it->second->RemoveConnections(bNotRemoveSetByUser);
}

void MSBeamG::DeleteConnections()
{
	MSCompoG::DeleteConnections();
	for (auto it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
		it->second->DeleteConnections();
}

void MSBeamG::RemoveConnection(MSConnection* pConnection)
{
	MSCompoG::RemoveConnection(pConnection);
	for (auto it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
		it->second->RemoveConnection(pConnection);
}

bool MSBeamG::HasSubMember()
{
	if (mm_BuiltUp.size() > 0)	return true;
	return MSLineMemberG::HasSubMember();
}

vector<MSElement*> MSBeamG::GetSubMembers()
{
	vector<MSElement*> subList;
	subList = MSLineMemberG::GetSubMembers();
	for (auto it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
	{
		subList.push_back(it->second);
	}
	return subList;
}

void MSBeamG::DeleteSubMembers()
{
	DeleteBuiltGs();
	MSLineMemberG::DeleteSubMembers();
}

bool MSBeamG::UsedMaterial(long materialID)
{
	map<long, MSBuiltUpBeamG*>::iterator it;
	for (it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
	{
		long nBuiltUpBeamMaterialID = it->second->GetBuiltUpBeamMaterialID();
		if (nBuiltUpBeamMaterialID == materialID)
			return true;
	}

	return MSLinkMemberG::UsedMaterial(materialID);
}

void MSBeamG::SubMemberMaterialValidCheck()
{
	MSBaseBuilding* pBldg = GetBuilding();
	if (pBldg == nullptr) return;

	MSLinkMemberG::SubMemberMaterialValidCheck();

	long defMaterialID = -1;

	map<long, MSBuiltUpBeamG*>::iterator it;
	for (it = mm_BuiltUp.begin(); it != mm_BuiltUp.end(); it++)
	{
		if (it->second->GetIsUsedStoryMaterial() == true) continue;

		long nBuiltUpBeamMaterialID = it->second->GetBuiltUpBeamMaterialID();
		if(pBldg->GetMaterial(nBuiltUpBeamMaterialID) != nullptr) continue;

		if (defMaterialID == -1)
		{
			MSMaterial* defMaterial = pBldg->GetDefMaterial();
			if (defMaterial == nullptr) return;
			defMaterialID = defMaterial->m_ID;
		}

		it->second->SetBuiltUpBeamMaterialID(defMaterialID);
	}
}
void MSBeamG::SetDummy(bool IsDummy)
{
	m_IsDummy = IsDummy;
	for(auto Iter = mm_BuiltUp.begin() ; Iter != mm_BuiltUp.end(); Iter++)
		Iter->second->SetDummy(IsDummy);
}

bool MSBeamG::IsReversBeamM()
{
	MSRcFWBeamM* pMSRcFWBeamM = dynamic_cast<MSRcFWBeamM*>(GetCompoM());
	if (pMSRcFWBeamM != nullptr && pMSRcFWBeamM->IsReversBeamM())
		return true;

	return false;
}

bool MSBeamG::IsCustomFWBeam()
{
	MSRcFWBeamM* pMSRcFWBeamM = dynamic_cast<MSRcFWBeamM*>(GetCompoM());
	if (pMSRcFWBeamM == nullptr)
		return false;
	return pMSRcFWBeamM->IsCustomM();
}


