#include "StdAfx.h"
#include "MSModelChecker.h"

#include "MSRebarBuilding.h"
#include "MSFloor.h"
#include "MSCompoG.h"
#include "MSLinkMemberG.h"
#include "MSLevelZone.h"
#include "MSConnection.h"
#include "MSConstructionZone.h"
#include "MSSlabG.h"
#include "MSRcSlabM.h"
#include "MSPolylineObject.h"
#include "MSMemberLevelDif.h"
#include "MSOpeningG.h"
#include "MSCompoM.h"
#include "MSDoorM.h"
#include "MSPitG.h"
#include "MSElement.h"
#include "MSBuiltUpBeamG.h"
#include "MSTrench.h"
#include "MSWell.h"
#include "MSGenStairG.h"
#include "MDRcStairM.h"
#include "MSCompoGUtil.h"
#include "MSWallFootG.h"
#include "MSIsoFootG.h"
#include "MSAddRebarBase.h"
#include "MSAddRebarAreaBent.h"
#include "MSFWSoundAbsorptionG.h"

#include "GMLib/AcisGeomHelper.h"
#include "GMLib/GM2DBound.h"
#include "MSRcFaceMemberM.h"


#include "RADFCore_CLI/ProgressReporterWrapper.h"
#include "RADFCore_CLI/StatusReporterWrapper.h"
#include "RADFCore_CLI/LoggerWrapper.h"
#include "Radfcore_CLI/AppGlobalOptionWrapper.h"
#include "GMLib/Global.h"
#include "GMLib/Msg.h"


MSModelChecker::MSModelChecker(MSRebarBuilding* pBuild, bool bMakeErrorObject, bool bMakeOutputReport)
{
	mp_Build = pBuild;
	m_nBuildElementCnt = 0;
	m_bMakeErrorObject = bMakeErrorObject;
	m_bMakeOutputReport = bMakeOutputReport;
	m_nlast = 0;
}


MSModelChecker::~MSModelChecker(void)
{

}

int MSModelChecker::CheckGeometryModel()
{
	if(mp_Build == NULL)
		return 0;

	long AboveStory = mp_Build->GetNumAboveStory();
	long UnderStory = -(mp_Build->GetNumUnderStory()-1);
	int nErrorCnt = 0;

	m_strGroup = LocalDic::GetLocal(L"Geometry Error", L"StructureModel");

	CString strtmp;

	map<long, MSFloor*>::iterator itFloor;
	for(itFloor=mp_Build->mm_Floor.begin(); itFloor != mp_Build->mm_Floor.end(); itFloor++)
	{
		MSFloor* pCurFloor = itFloor->second;
		if(pCurFloor == NULL)	continue;

		int nFloorErrorCnt = CheckGeometryModel_Floor(pCurFloor);

		if(nFloorErrorCnt > 0)
		{
			if(m_bMakeOutputReport)
			{
				strtmp.Format(_T("%s%s%s"), _T("\n---------------------------------------------------------\n"),
					LocalDic::GetLocal(L"FloorName: %s, Geometry Error ModelCnt: %d", L"StructureModel"),
					_T("\n---------------------------------------------------------\n"));
				m_strErrorMsg.Format(strtmp, pCurFloor->GetName(), nFloorErrorCnt);
				StatusReporterWrapper::OutputMessage(m_strErrorMsg);
			}
		}
		nErrorCnt += nFloorErrorCnt;
	}

	if(m_bMakeOutputReport)
	{
		strtmp.Format(_T("%s%s%s"), _T("\n---------------------------------------------------------\n"),
			LocalDic::GetLocal(L"BuildingName: %s, Geometry Error ModelCnt: %d", L"StructureModel"),
			_T("\n---------------------------------------------------------\n"));
		m_strErrorMsg.Format(strtmp, mp_Build->GetName(), nErrorCnt);
		StatusReporterWrapper::OutputMessage(m_strErrorMsg);
	}
	return nErrorCnt;
}

int MSModelChecker::CheckGeometryModel_Floor(MSFloor* pFloor)
{
	int nErrorCnt = 0;
	if(pFloor == NULL)
		return nErrorCnt;

	map<long, MSCompoG*>::iterator itMem;
	for (itMem = pFloor->mm_CompoG.begin(); itMem != pFloor->mm_CompoG.end(); itMem++)
	{
		MSCompoG* pCompoG = itMem->second;
		nErrorCnt += CheckGeometryModel_CompoG(pCompoG);
	}

	long NumLevelZone = pFloor->ma_LevelZone.size();
	for(long iZone = 0 ; iZone < NumLevelZone ; iZone++)
	{
		MSLevelZone* pLevelZone = pFloor->ma_LevelZone[iZone];
		nErrorCnt += CheckGeometryModel_Profile(pLevelZone);
		nErrorCnt += CheckGeometryModel_LevelZoneDist(pLevelZone, iZone);
	}

	return nErrorCnt;
}

int MSModelChecker::CheckGeometryModel_CompoG(MSCompoG* pCompoG)
{
	//Error 종류
	// 길이가 짧다.
	// 부재의 영역이 작다.
	// 얇은 폴리라인이 생성됨
	// M정보 셋팅이 잘못됨

	int nErrorCnt = 0;
	nErrorCnt += CheckGeometryModel_CompoM(pCompoG);

	nErrorCnt += CheckGeometryModel_LinkMember(pCompoG);
	nErrorCnt += CheckGeometryModel_PointMember(pCompoG);
	nErrorCnt += CheckGeometryModel_Profile(pCompoG);
	nErrorCnt += CheckGeometryModel_FindSlabGap(pCompoG);
	
	return nErrorCnt;
}

int MSModelChecker::CheckGeometryModel_CompoM(MSCompoG* pCompoG)
{
	MSCompoM* pCompoM = pCompoG->GetCompoM();
	if(pCompoM == NULL)
		return 0;

	MSCompoM::Type_MSCompoM eMType = pCompoM->GetType();
	MSElement::Type_MSCompoG eGType = pCompoG->GetType();

	if (eGType == MSElement::msBeamG && (eMType == MSCompoM::msBeamM || eMType == MSCompoM::msPcBeamM || eMType == MSCompoM::msSrcBeamM || eMType == MSCompoM::msSteelBeamM || eMType == MSCompoM::msCustomBeamM))
		return 0;
	else if (eGType == MSElement::msBraceG && eMType == MSCompoM::msBraceM)
		return 0;
	else if (eGType == MSElement::msColumnG && (eMType == MSCompoM::msColumnM || eMType == MSCompoM::msButtressM || eMType == MSCompoM::msSrcColumnM || eMType == MSCompoM::msPcColumnM || eMType == MSCompoM::msSteelColumnM || eMType == MSCompoM::msCustomColumnM))
		return 0;
	else if (eGType == MSElement::msWallG && (eMType == MSCompoM::msBWallM || eMType == MSCompoM::msRWallM || eMType == MSCompoM::msCustomWallColumnM))
		return 0;
	else if (eGType == MSElement::msWallColumnG && eMType == MSCompoM::msCustomWallColumnM)
		return 0;
	else if(eGType == MSElement::msIsoFootG && eMType == MSCompoM::msIsoFootM)
		return 0;
	else if(eGType == MSElement::msWallFootG && eMType == MSCompoM::msWallFootM)
		return 0;
	else if(eGType == MSElement::msRampG && eMType == MSCompoM::msRampM)
		return 0;
	else if(eGType == MSElement::msSlabG && eMType == MSCompoM::msSlabM)
		return 0;
	else if(eGType == MSElement::msPitG && eMType == MSCompoM::msPitM)
		return 0;
	else if(eGType == MSElement::msStairG && (eMType == MSCompoM::msStairLandingM || eMType == MSCompoM::msStairStepM) )
		return 0;
	else if(eGType == MSElement::msHandRailG && eMType == MSCompoM::msHandRailM)
		return 0;
	else if(eGType == MSElement::msOpeningG)
		return 0;
	else if (eGType == MSElement::msHaunchG)
		return 0;
	else
	{
		ASSERT(0);
		return 0;
	}

	MSFloor* pFloor = pCompoG->GetFloor();
	m_strErrorMsg.Format(LocalDic::GetLocal(L"%s %s(%ld)의 CompoM 정보가 잘못 되었습니다.", L"StructureModel"), pCompoG->GetCompoM()->m_Name, pCompoG->GetGroupName(),  pCompoG->m_ID);
	
	if(m_bMakeErrorObject)
		MSModelChecker::MakeErrorObject(m_strGroup, m_strErrorMsg,1, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_COMPOM ,pCompoG);
	if(m_bMakeOutputReport)
		StatusReporterWrapper::OutputMessage(m_strErrorMsg);

	return 1;
}

int MSModelChecker::CheckGeometryModel_LinkMember(MSCompoG* pCompoG)
{
	if(pCompoG->IsLinkMember() == FALSE)
		return 0;

	MSFloor* pFloor = pCompoG->GetFloor();
	MSLinkMemberG* pLinkG = (MSLinkMemberG*)pCompoG;
	double dLen = pLinkG->GetLength();
	if(dLen < MSFloor::ms_DefLineHitDistTol)
	{

		m_strErrorMsg.Format(LocalDic::GetLocal(L"%s %s(%ld)의 길이가 DefHitDist 보다 짧습니다.", L"StructureModel"), pCompoG->GetCompoM()->m_Name, pCompoG->GetGroupName(),  pCompoG->m_ID);
		if(m_bMakeErrorObject)
			MSModelChecker::MakeErrorObject(m_strGroup, m_strErrorMsg,1, MESSAGE_LEVEL_WARNING, MESSAGE_WARNING_LINMEMBERLEN ,pCompoG);
		if(m_bMakeOutputReport)
			StatusReporterWrapper::OutputMessage(m_strErrorMsg);

		return 1;
	}

	return 0;
}

int MSModelChecker::CheckGeometryModel_PointMember(MSCompoG* pCompoG)
{
	return 0;
}

int MSModelChecker::CheckGeometryModel_Profile(MSElement* pElement)
{
	MSElement::Type_MSCompoG eType = pElement->GetType();
	if(eType != MSElement::msLevelZone && eType != MSElement::msSlabG)
		return 0;

	GM2DPolyline* pPoly = pElement->GetProfile();
	if(pPoly == NULL)
		return 0;

	MSFloor* pFloor = pElement->GetFloor();
	pPoly->MakeClosedPolyline(DTOL_GM);

	if(pPoly->CheckPolygon(false) == FALSE)
	{
		if(eType == MSElement::msLevelZone)
		{
			MSLevelZone* pLevelZone = (MSLevelZone*)pElement;
			m_strErrorMsg.Format(LocalDic::GetLocal(L"%s : %s(%ld)의 Profile 형상이 잘못 되었습니다.", L"StructureModel"), pLevelZone->GetUserFloorName(), pLevelZone->GetGroupName(),  pLevelZone->m_ID);
			
			if(m_bMakeErrorObject)
				MSModelChecker::MakeErrorObject(m_strGroup, m_strErrorMsg,0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_SLAB_PROFILE ,pLevelZone);
			if(m_bMakeOutputReport)
				StatusReporterWrapper::OutputMessage(m_strErrorMsg);
		}
		else
		{
			MSCompoG* pCompoG = (MSCompoG*)pElement;
			m_strErrorMsg.Format(LocalDic::GetLocal(L"%s : %s(%ld)의 Profile 형상이 잘못 되었습니다.", L"StructureModel"), pCompoG->GetCompoM()->m_Name, pCompoG->GetGroupName(),  pCompoG->m_ID);
		
			if(m_bMakeErrorObject)
				MSModelChecker::MakeErrorObject(m_strGroup, m_strErrorMsg,1, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_SLAB_PROFILE ,pElement);
			if(m_bMakeOutputReport)
				StatusReporterWrapper::OutputMessage(m_strErrorMsg);
		}
		return 1;
	}
	
	return 0;
}

int MSModelChecker::CheckGeometryModel_FindSlabGap(MSCompoG* pCompoG)
{
	MSFloor* pFloor = pCompoG->GetFloor();
	MSElement::Type_MSCompoG eType = pCompoG->GetType();
	if(eType != MSElement::msSlabG)
		return 0;

	MSSlabG* pSlab = (MSSlabG*)pCompoG;
	long NumSlabLevelZone = pSlab->ma_LevelPoly.size();
	for(long iSlabLevelZone = 0 ; iSlabLevelZone < NumSlabLevelZone ; iSlabLevelZone++)
	{
		MSSlabLevelZone* pSZone = pSlab->ma_LevelPoly[iSlabLevelZone];
		if(pSZone->m_IsDelete)
			continue;

		int Idx = pSlab->GetProfile()->CheckPolylineSegmentDist(pSZone->mp_Polyline, MSFloor::ms_DefLineHitDistTol);
		if(Idx != -1)
		{
			m_strErrorMsg.Format(LocalDic::GetLocal(L"%s %s(%ld)와 SlabLevelZone 사이에 좁은 간격이 존재 합니다.", L"StructureModel"), pCompoG->GetCompoM()->m_Name, pCompoG->GetGroupName(),  pCompoG->m_ID);
			if(m_bMakeErrorObject)
				MSModelChecker::MakeErrorObject(m_strGroup, m_strErrorMsg,1, MESSAGE_LEVEL_WARNING, MESSAGE_WARNING_POLYLINESEFMENTDIST ,pCompoG);
			if(m_bMakeOutputReport)
				StatusReporterWrapper::OutputMessage(m_strErrorMsg);

			return 1;
		}
	}

	GM2DPolyline* pUrPoly = pCompoG->GetProfile();

	vector<MSCompoG*> SlabArr = MSCompoGUtil::FindCompoG(pFloor, pUrPoly, FindType_MSElement::msSlabG, true, true, DefHitDist);//pCompoG->FindConnectMember(MSElement::msSlabG, 0);
	for(long iSlab = 0 ; iSlab < SlabArr.size() ; iSlab++)
	{
		MSSlabG* pCurSlabG = (MSSlabG*)SlabArr[iSlab];

		GM2DPolyline* pCurPoly = pCurSlabG->GetProfile();
		if(pUrPoly->SamePolyLine(pCurPoly))
			continue;

		int Idx = pUrPoly->CheckPolylineSegmentDist(pCurPoly, MSFloor::ms_DefLineHitDistTol);
		if(Idx != -1)
		{
			m_strErrorMsg.Format(LocalDic::GetLocal(L"%s %s(%ld)와 %s %s(%ld) 사이에 좁은 간격이 존재 합니다.", L"StructureModel"), pCompoG->GetCompoM()->m_Name, pCompoG->GetGroupName(), pCompoG->m_ID, pCurSlabG->GetCompoM()->m_Name, pCurSlabG->GetGroupName(), pCurSlabG->m_ID);
			if(m_bMakeErrorObject)
				MSModelChecker::MakeErrorObject(m_strGroup, m_strErrorMsg,2, MESSAGE_LEVEL_WARNING, MESSAGE_WARNING_POLYLINESEFMENTDIST ,pCompoG, pCurSlabG);
			if(m_bMakeOutputReport)
				StatusReporterWrapper::OutputMessage(m_strErrorMsg);
			return 1;
		}
	}

// 	map<long, MSCompoG*>::iterator itCompoG;
// 	for (itCompoG = pFloor->mm_CompoG.begin(); itCompoG != pFloor->mm_CompoG.end(); itCompoG++)
// 	{
// 		MSCompoG* pCurCompoG = itCompoG->second;
// 		if(pCurCompoG->GetID() >= pCompoG->GetID() || pCurCompoG->GetType() != MSElement::msSlabG) continue;
// 		MSSlabG* pCurSlabG = (MSSlabG*)pCurCompoG;
// 
// 		if(!pCompoG->IsLevelOverlap(pCurSlabG))
// 			continue;
// 
// 		GM2DPolyline* pCurPoly = pCurSlabG->GetProfile();
// 		if(pUrPoly->SamePolyLine(pCurPoly))
// 			continue;
// 
// 		int Idx = pUrPoly->CheckPolylineSegmentDist(pCurPoly, MSFloor::ms_DefLineHitDistTol);
// 		if(Idx != -1)
// 		{
// 			m_strErrorMsg.Format(_T("%s %s(%ld)와 %s %s(%ld) 사이에 좁은 간격이 존재 합니다."), pCompoG->GetCompoM()->m_Name, pCompoG->GetGroupName(), pCompoG->m_ID, pCurCompoG->GetCompoM()->m_Name, pCurCompoG->GetGroupName(), pCurCompoG->m_ID);
// 			if(m_bMakeErrorObject)
// 				MSModelChecker::MakeErrorObject(m_strGroup, m_strErrorMsg,2, MESSAGE_LEVEL_WARNING, MESSAGE_WARNING_POLYLINESEFMENTDIST ,pCompoG, pCurCompoG);
// 			if(m_bMakeOutputReport)
// 				StatusReporterWrapper::OutputMessage(m_strErrorMsg);
// 			return 1;
// 		}
// 	}

	return 0;
}

int MSModelChecker::CheckGeometryModel_LevelZoneDist(MSLevelZone* pLevelZone, int iZone)
{
	int nErrorCnt = 0;
	MSFloor* pFloor = pLevelZone->GetFloor();

	long NumLevelZone = pFloor->ma_LevelZone.size();
	for(long jZone = iZone + 1 ; jZone < NumLevelZone ; jZone++)
	{
		MSLevelZone* pUrLevelZone = pFloor->ma_LevelZone[jZone];
		int Idx = pLevelZone->GetProfile()->CheckPolylineSegmentDist(pUrLevelZone->GetProfile(), MSFloor::ms_DefLineHitDistTol);
		if(Idx != -1)
		{
			m_strErrorMsg.Format(LocalDic::GetLocal(L"%s : %s(%ld)와 %s(%ld)사이에 좁은 간격이 존재 합니다.", L"StructureModel"), pLevelZone->GetUserFloorName(),  pLevelZone->GetGroupName(),  pLevelZone->m_ID, pUrLevelZone->GetGroupName(), pUrLevelZone->m_ID);
			if(m_bMakeErrorObject)
				MSModelChecker::MakeErrorObject(m_strGroup, m_strErrorMsg, 0, MESSAGE_LEVEL_WARNING, MESSAGE_WARNING_POLYLINESEFMENTDIST, pLevelZone);
			if(m_bMakeOutputReport)
				StatusReporterWrapper::OutputMessage(m_strErrorMsg);
			nErrorCnt++;
		}
	}

	return nErrorCnt;
}

//--------------------------------------------- CheckModel Start--------------------------------------------------------

int MSModelChecker::CheckModel()
{
	//타입별 수직 부재 찾기
	long nErrorAllCnt = 0;
	long AboveStory = mp_Build->GetNumAboveStory();
	long UnderStory = -(mp_Build->GetNumUnderStory()-1);

	CString strMsg, GroupName;
	XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();
	int Level = 1;
	int ErrorCode = 0;
	int nErrorCnt = 0;
	int Flag_FM = 1;
	int Flag_VM = -1;

	bool isZOverLap = false;
	bool isZConnected = false;
	bool isBigCompoM = false;
	bool isSupportOnSlab = false;
	bool isSupportOnMember = false;
	bool isDefName = false;
	bool IsVerticalMemberEccOver = false;

	bool isErrorSlabProfile = true;
	
	InitProgressReporter();
	try
	{
		for(long iStory = AboveStory; iStory >= UnderStory; iStory--)
		{
			if(iStory == 0)	continue;
			MSFloor* pCurFloor = mp_Build->GetFloor(iStory);
			if(pCurFloor == NULL)	continue;

			MSFloor* pLowFloor = pCurFloor->GetLowerFloor();
			if(pLowFloor == NULL)	continue;

			long nErrorCnt = CheckModel_Floor(pCurFloor, true);
			long nLowErrorCnt = CheckModel_Floor(pLowFloor, false);

			nErrorAllCnt += nErrorCnt;
			nErrorAllCnt += nLowErrorCnt;
		}
		CloseProgressReporter();
	}
	catch (CException* e)
	{
		CloseProgressReporter();
	}

	return nErrorAllCnt;
}

long MSModelChecker::CheckModel_Floor(MSFloor* pFloor, bool bCheckVerticalMember)
{
	XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();
	int nErrorCnt = 0;
	
	long CheckCnt = MSCheckModelOption::GetInstance()->m_nCheckModelOpt.GetCheckCnt();
	if(CheckCnt == 0)
		return nErrorCnt;

	MSFloor* pCurFloor = pFloor;
	if(pCurFloor == NULL)	
		return nErrorCnt;

	// 수직 부재 검토는 LowFloor 필요함
	MSFloor* pLowFloor = pCurFloor->GetLowerFloor();
	if(bCheckVerticalMember && pLowFloor == NULL)	
		return nErrorCnt;

	MSFloor* pUpperFloor = pCurFloor->GetUpperFloor();
	if(pUpperFloor == NULL) 
		return nErrorCnt;

	// Beam
	map<long, MSLinkMemberG*> beamMap;
	vector<MSColineLinkMemberGs*> BeamLineArr;

	map<long, MSCompoG*>::iterator itCompoG;
	for( itCompoG = pCurFloor->mm_CompoG.begin(); itCompoG != pCurFloor->mm_CompoG.end(); itCompoG++)
	{
		MSCompoG* pCompoG = itCompoG->second;

		CheckModelOpt CheckFlag;
		CheckFlag.Init();

		if(pCompoG->GetType() == MSElement::msOpeningG)
		{
			if(bCheckVerticalMember == true)
			{
				CheckModel_PolyElement(pCompoG, CheckFlag);
				nErrorCnt += CheckModel_MakeErrorObjects(pCompoG, CheckFlag);
			}
			continue;
		}

		if(bCheckVerticalMember && !pCompoG->IsVerticalMember())	continue;
		else if(!bCheckVerticalMember && pCompoG->IsVerticalMember())	continue;

		bool IsSupported = false;

		CheckModel_DefName(pCompoG, CheckFlag);

		MSElement::Type_MSCompoG CurType = pCompoG->GetType();
		vector<MSElement::Type_MSCompoG> CheckTypeArr;

		if(CurType == MSElement::msColumnG)
		{
			MSColumnG* pColumnG = (MSColumnG*)pCompoG;

			vector<MSCompoG*> LowColumnArr = pColumnG->FindConnectMember(FindType_MSElement::msColumnG, -1);
			vector<MSCompoG*> LowIsoFootArr = pColumnG->FindConnectMember(FindType_MSElement::msIsoFootG, -1);
			if(LowColumnArr.size() > 0) 
			{
 				for(long iColumn = 0 ; iColumn < LowColumnArr.size() ; iColumn++)
				{
					MSColumnG* pLowColumn = (MSColumnG*)LowColumnArr[iColumn];
					//아래층 기둥 ZLevel 검토
					CheckModel_ZLevel_ConnectedOverLap(pColumnG, pLowColumn, CheckFlag);
					CheckModel_ColumnM(pColumnG, pLowColumn, CheckFlag);
					CheckModel_ColumnEccOver(pColumnG, pLowColumn, CheckFlag);

				}
			}
			if(LowIsoFootArr.size() > 0)
			{
				for(long iIsoFoot = 0 ; iIsoFoot < LowIsoFootArr.size() ; iIsoFoot++)
				{
					MSIsoFootG* pLowIsoFoot = (MSIsoFootG*)LowIsoFootArr[iIsoFoot];
					CheckModel_ZLevel_ConnectedOverLap(pColumnG, pLowIsoFoot, CheckFlag);
				}
			}

			//아래층에 연결된 기둥이 없고 벽, 줄기초 있으면 Warring
			CheckModel_SupportOnMember(pColumnG, pFloor, CheckFlag);
		}
		else if(CurType == MSElement::msWallG)
		{
			//아래층에 벽, 줄기초 있으면 OK
			CheckModel_SupportOnMember(pCompoG, pFloor, CheckFlag);
			
			MSWallMemberG* pWall = (MSWallMemberG*)pCompoG;
			if (MSModelChecker::CheckModel_WallOpening(pWall) == false)
				nErrorCnt++;
		}
		else if(CurType == MSElement::msIsoFootG)
		{
			MSIsoFootG* pIsoFoot = (MSIsoFootG*)pCompoG;
			MSColumnG* pConnectedColumn = pIsoFoot->GetConnectedColumnG();

			CheckModel_ZLevel_ConnectedOverLap(pIsoFoot, pConnectedColumn, CheckFlag);
		}
		else if(CurType == MSElement::msWallFootG)
		{
			CheckModel_SupportOnMember(pCompoG, pUpperFloor, CheckFlag);
		}
		else if(CurType == MSElement::msBeamG || CurType == MSElement::msHandRailG)
		{
			if(CurType == MSElement::msBeamG)
				CheckModel_MakeBeamLine((MSBeamG*)pCompoG, beamMap, BeamLineArr);
		}
		else if(CurType == MSElement::msSlabG)
		{
			MSSlabG* pSlabG = (MSSlabG*)pCompoG;
			CheckModel_PolyElement(pSlabG, CheckFlag);
			
		}
		else
		{

		}

		if(pCompoG->IsLineSupportMember())
			CheckModel_Support(pCompoG, CheckFlag);

		bool bCheckVerCantil = false;
		if(pCompoG->IsVerticalMember())
			bCheckVerCantil = true;

		CheckModel_Connection(pCompoG, CheckFlag, bCheckVerCantil);

		if(pCompoG->IsLinkMember())
		{
			CheckModel_HorOverLap((MSLinkMemberG*)pCompoG, CheckFlag);
			CheckModel_LineMemberOutLen(pCompoG, CheckFlag);
		}

		CheckModel_VerMember_Contain(pCompoG, CheckFlag);
		CheckModel_SameMember_OverLap(pCompoG, CheckFlag);

		UpdateProgressReporter();
		nErrorCnt += CheckModel_MakeErrorObjects(pCompoG, CheckFlag);
	}

	nErrorCnt += CheckModel_BeamLines(BeamLineArr);

	for(long i = 0 ; i < BeamLineArr.size() ; i++)
	{
		MSColineLinkMemberGs* pLines = BeamLineArr[i];
		delete pLines;
	}
	BeamLineArr.clear();

	if(bCheckVerticalMember == true)
	{
		map<long, MSConstructionZone*>::iterator it;
		for (it = pFloor->mm_CZone.begin() ; it != pFloor->mm_CZone.end() ; ++it)
		{
			MSConstructionZone* pCZone = it->second;

			CheckModelOpt CheckFlag;
			CheckFlag.Init();

			CheckModel_PolyElement(pCZone, CheckFlag);
			UpdateProgressReporter();

			nErrorCnt += CheckModel_MakeErrorObjects(pCZone, CheckFlag);
		}


		for (long i = 0 ; i < pFloor->ma_LevelZone.size() ; i++)
		{
			MSLevelZone* pLevelZone = pFloor->ma_LevelZone[i];

			CheckModelOpt CheckFlag;
			CheckFlag.Init();

			CheckModel_PolyElement(pLevelZone, CheckFlag);
			UpdateProgressReporter();

			nErrorCnt += CheckModel_MakeErrorObjects(pLevelZone, CheckFlag);
		}
	}
	//CheckCompoM(pCurFloor);

	return nErrorCnt;
}

bool MSModelChecker::CheckModel_DefName(MSCompoG* pCompoG, CheckModelOpt &CheckFlag)
{
	if(!MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_MODEL))
		return true;
	if(!(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_NONAME)))
		return true;
	if(CheckFlag.IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_NONAME))
		return false;

	MSCompoM* pCompoM = pCompoG->GetCompoM();
	CString strCompoMName = pCompoM->GetName().MakeUpper();
	CString strPreFixDefName = MSCheckModelOption::GetInstance()->m_strPreFixName.MakeUpper();
	long nRval = strCompoMName.Find(strPreFixDefName);
	if(nRval == -1)
		return true;

	CheckFlag.SetCheckedSecoundOption(CMO_MODEL, CMO_MODEL_NONAME, true);
	return false;
}

bool MSModelChecker::CheckModel_PolyElement(MSElement* pElement, CheckModelOpt &CheckFlag)
{
	GM2DPolyline* pPoly = pElement->GetProfile();
	if(pPoly == NULL || pPoly->GetNumLines() < 2)
		return true;

	pPoly->MakeClosedPolyline(DTOL_GM);
	CheckModel_PolyElement_Profile(pElement, CheckFlag);
	CheckModel_PolyElement_Dist(pElement, CheckFlag);
	CheckModel_Slab_Area(pElement, CheckFlag);

	if(CheckFlag.IsCheckedSecoundOption(CMO_GEN, CMO_GEN_SLAB_PROFILE) == true || CheckFlag.IsCheckedSecoundOption(CMO_GEN, CMO_GEN_SLAB_DIST) == true ||
		CheckFlag.IsCheckedSecoundOption(CMO_GEN, CMO_GEN_SLAB_AREA) == true)
		return false;

	return true;
}

bool MSModelChecker::CheckModel_PolyElement_Profile(MSElement* pElement, CheckModelOpt &CheckFlag)
{
	if(!(MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_GEN)))
		return true;
	if(!(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_GEN, CMO_GEN_SLAB_PROFILE)))
		return true;

	if(CheckFlag.IsCheckedSecoundOption(CMO_GEN, CMO_GEN_SLAB_PROFILE))
		return false;

	GM2DPolyline* pPoly = pElement->GetProfile();
	if(pPoly == NULL)
		return true;

	if(pPoly->CheckPolygon(false) == FALSE)
	{
		CheckFlag.SetCheckedSecoundOption(CMO_GEN, CMO_GEN_SLAB_PROFILE, true);
		return false;
	}

	return true;
}

bool MSModelChecker::CheckModel_PolyElement_Dist(MSElement* pElement, CheckModelOpt &CheckFlag)
{
	if(!(MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_GEN)))
		return true;
	if(!(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_GEN, CMO_GEN_SLAB_DIST)))
		return true;

	if(CheckFlag.IsCheckedSecoundOption(CMO_GEN, CMO_GEN_SLAB_DIST))
		return false;

	MSFloor* pFloor = pElement->GetFloor();

	GM2DPolyline* pPoly = pElement->GetProfile();
	double dTol = DefHitDist;

	vector<MSCompoG*> SlabArr = MSCompoGUtil::FindCompoG(pFloor, pPoly, FindType_MSElement::msSlabG, true, true, dTol);// pSlab->FindConnectMember(MSElement::msSlabG, 0);
	for(long iSlab = 0 ; iSlab < SlabArr.size() ; iSlab++)
	{
		MSSlabG* pCurSlabG = (MSSlabG*)SlabArr[iSlab];
		if(pElement == pCurSlabG)
			continue;

		GM2DPolyline* pCurPoly = pCurSlabG->GetProfile();
		if(pPoly->SamePolyLine(pCurPoly))
			continue;

		int Idx = pPoly->CheckPolylineSegmentDist(pCurPoly, MSFloor::ms_DefLineHitDistTol);
		if(Idx != -1)
		{
			CheckFlag.SetCheckedSecoundOption(CMO_GEN, CMO_GEN_SLAB_DIST, true);
			return false;
		}
	}

	vector<MSCompoG*> OpeningGArr = MSCompoGUtil::FindCompoG(pFloor, pPoly, FindType_MSElement::msOpeningG, true, true, dTol);// pSlab->FindConnectMember(MSElement::msSlabG, 0);
	for(long iOpen = 0 ; iOpen < OpeningGArr.size() ; iOpen++)
	{
		MSOpeningG* pCurOpeningG = (MSOpeningG*)OpeningGArr[iOpen];
		if(pCurOpeningG == pElement)
			continue;

		GM2DPolyline* pCurPoly = pCurOpeningG->GetProfile();
		if(pPoly->SamePolyLine(pCurPoly))
			continue;

		int Idx = pPoly->CheckPolylineSegmentDist(pCurPoly, MSFloor::ms_DefLineHitDistTol);
		if(Idx != -1)
		{
			CheckFlag.SetCheckedSecoundOption(CMO_GEN, CMO_GEN_SLAB_DIST, true);
			return false;
		}
	}
	
	map<long, MSConstructionZone*>::iterator it;
	for (it = pFloor->mm_CZone.begin() ; it != pFloor->mm_CZone.end() ; ++it)
	{
		MSConstructionZone* pCZone = it->second;
		if(pCZone == pElement)
			continue;

		GM2DPolyline* pCurPoly = pCZone->GetProfile();
		if(pPoly->SamePolyLine(pCurPoly))
			continue;

		int Idx = pPoly->CheckPolylineSegmentDist(pCurPoly, MSFloor::ms_DefLineHitDistTol);
		if(Idx != -1)
		{
			CheckFlag.SetCheckedSecoundOption(CMO_GEN, CMO_GEN_SLAB_DIST, true);
			return false;
		}
	}

	for (long i = 0 ; i < pFloor->ma_LevelZone.size() ; i++)
	{
		MSLevelZone* pLevelZone = pFloor->ma_LevelZone[i];
		if(pLevelZone == pElement)
			continue;

		GM2DPolyline* pCurPoly = pLevelZone->GetProfile();
		if(pPoly->SamePolyLine(pCurPoly))
			continue;

		int Idx = pPoly->CheckPolylineSegmentDist(pCurPoly, MSFloor::ms_DefLineHitDistTol);
		if(Idx != -1)
		{
			CheckFlag.SetCheckedSecoundOption(CMO_GEN, CMO_GEN_SLAB_DIST, true);
			return false;
		}
	}


// 	map<long, MSCompoG*>::iterator itCompoG;
// 	for (itCompoG = pFloor->mm_CompoG.begin(); itCompoG != pFloor->mm_CompoG.end(); itCompoG++)
// 	{
// 		MSCompoG* pCurCompoG = itCompoG->second;
// 		if(pCurCompoG->GetID() >= pSlab->GetID() || pCurCompoG->GetType() != MSElement::msSlabG) continue;
// 		MSSlabG* pCurSlabG = (MSSlabG*)pCurCompoG;
// 
// 		if(!pSlab->IsLevelOverlap(pCurSlabG))
// 			continue;
// 
// 		GM2DPolyline* pCurPoly = pCurSlabG->GetProfile();
// 		if(pPoly->SamePolyLine(pCurPoly))
// 			continue;
// 
// 		int Idx = pPoly->CheckPolylineSegmentDist(pCurPoly, MSFloor::ms_DefLineHitDistTol);
// 		if(Idx != -1)
// 		{
// 			CheckFlag.SetCheckedSecoundOption(CMO_GEN, CMO_GEN_SLAB_DIST, true);
// 			return false;
// 		}
// 	}

	return true;
}

bool MSModelChecker::CheckModel_Slab_Area(MSElement* pElement, CheckModelOpt &CheckFlag)
{
	if (!(MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_GEN)))
		return true;
	if (!(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_GEN, CMO_GEN_SLAB_AREA)))
		return true;
	if (pElement->GetType() != MSElement::msSlabG)
		return true;

	if (CheckFlag.IsCheckedSecoundOption(CMO_GEN, CMO_GEN_SLAB_AREA))
		return false;

	MSFloor* pFloor = pElement->GetFloor();

	MSSlabG* pSlab = (MSSlabG*)pElement;
	GM2DPolyline* pPoly = pSlab->GetTrimmedBoundary();
	double dSlabArea = pPoly->Area();

	MSCheckModelItem* pItem = MSCheckModelOption::GetInstance()->GetOptItem(CMO_GEN, CMO_GEN_SLAB_AREA);
	int nCheckVal = pItem->m_nCheckVal;
	if(nCheckVal >= dSlabArea)
		CheckFlag.SetCheckedSecoundOption(CMO_GEN, CMO_GEN_SLAB_AREA, true);

	return true;
}

bool MSModelChecker::CheckModel_Slab_LevelDif(MSSlabG* pSlab, CheckModelOpt &CheckFlag)
{
	if(!(MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_HOR_LEVELDIF)))
		return true;
	if(!(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_BELOW)) &&
		!(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_OVER)) )
		return true;

	if(pSlab->ma_LevelDif.size() > 0)
	{
		MSColineLinkMemberGs* pLevelDifs = new MSColineLinkMemberGs();
		for(long i = 0 ; i < pSlab->ma_LevelDif.size() ; i++)
		{
			MSLinkMemberG* pLinkG = (MSLinkMemberG*)pSlab->ma_LevelDif[i];
			pLevelDifs->Add(pLinkG);
		}

		double dMostlyBeamZLevel = pLevelDifs->GetMostlyZLevel();

		map<long, MSLinkMemberG*>::iterator it;
		for(it = pLevelDifs->mm_LinkMem.begin() ; it != pLevelDifs->mm_LinkMem.end() ; it++)
		{
			MSLinkMemberG* pLinkMem = it->second;

			double dSTopLevel = 0;
			double dSBotLevel = 0;
			double dETopLevel = 0;
			double dEBotLevel = 0;
			pLinkMem->GetZLevelsbyLoc(dSTopLevel, dSBotLevel, 0);
			pLinkMem->GetZLevelsbyLoc(dETopLevel, dEBotLevel, 1);

			double dCheckSTopLevel = fabs(dSTopLevel - dMostlyBeamZLevel);
			double dCheckETopLevel = fabs(dETopLevel - dMostlyBeamZLevel);
			SetCheckModelOptFlag(dCheckSTopLevel, CheckFlag, CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_BELOW);
			SetCheckModelOptFlag(dCheckETopLevel, CheckFlag, CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_BELOW);

			SetCheckModelOptFlag(dCheckSTopLevel, CheckFlag, CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_OVER);
			SetCheckModelOptFlag(dCheckETopLevel, CheckFlag, CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_OVER);
		}
	}


	if(pSlab->ma_EdgeWall.size() > 0)
	{
		MSColineLinkMemberGs* pLevelDifs = new MSColineLinkMemberGs();
		for(long i = 0 ; i < pSlab->ma_EdgeWall.size() ; i++)
		{
			MSLinkMemberG* pLinkG = (MSLinkMemberG*)pSlab->ma_EdgeWall[i];
			pLevelDifs->Add(pLinkG);
		}

		double dMostlyBeamZLevel = pLevelDifs->GetMostlyZLevel();

		map<long, MSLinkMemberG*>::iterator it;
		for(it = pLevelDifs->mm_LinkMem.begin() ; it != pLevelDifs->mm_LinkMem.end() ; it++)
		{
			MSLinkMemberG* pLinkMem = it->second;

			double dSTopLevel = 0;
			double dSBotLevel = 0;
			double dETopLevel = 0;
			double dEBotLevel = 0;
			pLinkMem->GetZLevelsbyLoc(dSTopLevel, dSBotLevel, 0);
			pLinkMem->GetZLevelsbyLoc(dETopLevel, dEBotLevel, 1);

			double dCheckSTopLevel = fabs(dSTopLevel - dMostlyBeamZLevel);
			double dCheckETopLevel = fabs(dETopLevel - dMostlyBeamZLevel);
			SetCheckModelOptFlag(dCheckSTopLevel, CheckFlag, CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_BELOW);
			SetCheckModelOptFlag(dCheckETopLevel, CheckFlag, CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_BELOW);

			SetCheckModelOptFlag(dCheckSTopLevel, CheckFlag, CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_OVER);
			SetCheckModelOptFlag(dCheckETopLevel, CheckFlag, CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_OVER);
		}
	}

	if(CheckFlag.IsCheckedSecoundOption(CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_BELOW) || CheckFlag.IsCheckedSecoundOption(CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_OVER))
		return false;
	
	return true;
}

bool MSModelChecker::CheckModel_Support(MSCompoG* pCompoG, CheckModelOpt &CheckFlag)
{
	if(!(MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_MODEL)))
		return true;
	if(!(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_HOR_CANTIL)))
		return true;

	MSElement::Type_MSCompoG CurType = pCompoG->GetType();
	bool IsStartSupport = false;
	bool IsEndSupport = false;

	//if(CurType == MSElement::msBeamG || CurType == MSElement::msHandRailG || CurType == MSElement::msWallFootG)
	if(pCompoG->IsLineSupportMember())
	{
		MSLinkMemberG* pLinkMember = (MSLinkMemberG*)pCompoG;
		double dDepth = pLinkMember->GetDepth();
		

		vector<MSConnection*>SConnArr = pLinkMember->GetConnectionByPos(CON_START);
		for(long iConn = 0 ; iConn < SConnArr.size() ; iConn++)
		{
			MSConnection* pConn = SConnArr[iConn];
			if(IsStartSupport == false)
			{
				MSCompoG* pPartnerG = pConn->GetPartner(pCompoG);
				if(pPartnerG == nullptr) continue;
				MSElement::Type_MSCompoG ePartnerType = pPartnerG->GetType();
				//if(ePartnerType == MSElement::msWallG || ePartnerType == MSElement::msColumnG)
				CheckModelOpt nFlag;
				if(pCompoG->IsLevelConnectedOverLap(pPartnerG, nFlag))
					IsStartSupport = true;
			}
		}

		vector<MSConnection*>EConnArr = pLinkMember->GetConnectionByPos(CON_END);
		for(long iConn = 0 ; iConn < EConnArr.size() ; iConn++)
		{
			MSConnection* pConn = EConnArr[iConn];
			if(IsEndSupport == false)
			{
				MSCompoG* pPartnerG = pConn->GetPartner(pCompoG);
				if(pPartnerG == nullptr) continue;
				MSElement::Type_MSCompoG ePartnerType = pPartnerG->GetType();
				//if(ePartnerType == MSElement::msWallG || ePartnerType == MSElement::msColumnG)
				CheckModelOpt nFlag;
				if(pCompoG->IsLevelConnectedOverLap(pPartnerG, nFlag))
					IsEndSupport = true;
			}
		}

		if(IsStartSupport && IsEndSupport)
			return true;

	}
	else
	{

	}

	if(IsStartSupport != IsEndSupport && (IsStartSupport == false || IsEndSupport == false))
		CheckFlag.SetCheckedSecoundOption(CMO_MODEL, CMO_MODEL_HOR_CANTIL, true);

	return false;
}

bool MSModelChecker::CheckModel_SupportOnMember(MSCompoG* pCompoG, MSFloor* pFloor, CheckModelOpt &CheckFlag)
{
	if(!(MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_VER_SUPPORT)) &&
		!(MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_VER_SEC)) &&
		!(MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_VER_OUTECC)))
		return true;

	bool bRval = false;
	MSElement::Type_MSCompoG CurType = pCompoG->GetType();
	vector<MSElement::Type_MSCompoG> CheckTypeArr;
	int nFloorType = true;
	if(CurType == MSElement::msColumnG)
	{
		//아래층에 벽, 줄기초 있으면 Warring
		CheckTypeArr.push_back(MSElement::msWallG);
		CheckTypeArr.push_back(MSElement::msWallFootG);
		CheckTypeArr.push_back(MSElement::msSlabG);
		nFloorType = -1;
	}
	else if(CurType == MSElement::msWallG)
	{
		//아래층에 벽, 줄기초 있으면 OK
		CheckTypeArr.push_back(MSElement::msWallG);
		CheckTypeArr.push_back(MSElement::msWallFootG);
		CheckTypeArr.push_back(MSElement::msIsoFootG);
		CheckTypeArr.push_back(MSElement::msSlabG);
		nFloorType = -1;
	}
	else if(CurType == MSElement::msWallFootG)
	{
		//위층에 벽 있으면 OK
		CheckTypeArr.push_back(MSElement::msWallG);
		nFloorType = 1;
	}

	bRval = CheckModel_SupportedOnMemberInFloor(pFloor, pCompoG, nFloorType, CheckTypeArr, CheckFlag);
	return bRval;
}

bool MSModelChecker::CheckModel_ColumnM(MSColumnG* pColumn, MSColumnG* pDnColumn, CheckModelOpt &CheckFlag)
{
	if(!MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_VER_SEC))
		return true;

	if(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_VER_SEC, CMO_VER_SEC_SIZE_BELOW))
	{
		double dWidth = pColumn->GetWidth();
		double dDepth = pColumn->GetDepth();
		double dThick = pColumn->GetThick();

		double dDnWidth = pDnColumn->GetWidth();
		double dDnDepth = pDnColumn->GetDepth();
		double dDnThick = pDnColumn->GetThick();

		MSCheckModelItem* pItem = MSCheckModelOption::GetInstance()->GetOptItem(CMO_VER_SEC, CMO_VER_SEC_SIZE_BELOW);
		int nCheckVal = pItem->m_nCheckVal;
		bool bCheckDownVal = pItem->m_bCheckDownVal;

		double dCheckWidth = fabs(dWidth-dDnWidth);
		double dCheckDepth = fabs(dDepth-dDnDepth);
		double dCheckThick = fabs(dThick-dDnThick);

		if(bCheckDownVal == true)
		{
			if(dCheckWidth <= nCheckVal && dCheckWidth > 0 ||
				dCheckDepth <= nCheckVal && dCheckDepth > 0 ||
				dCheckThick <= nCheckVal && dCheckThick > 0)
			{
				CheckFlag.SetCheckedSecoundOption(CMO_VER_SEC, CMO_VER_SEC_SIZE_BELOW, true);
				return false;
			}
		}
		else
		{
			if(dCheckWidth > nCheckVal ||
				dCheckDepth > nCheckVal ||
				dCheckThick > nCheckVal)
			{
				CheckFlag.SetCheckedSecoundOption(CMO_VER_SEC, CMO_VER_SEC_SIZE_BELOW, true);
				return false;
			}
		}
	}

	if(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_VER_SEC, CMO_VER_SEC_SIZE_OVER))
	{
		double dWidth = pColumn->GetWidth();
		double dDepth = pColumn->GetDepth();
		double dThick = pColumn->GetThick();

		double dDnWidth = pDnColumn->GetWidth();
		double dDnDepth = pDnColumn->GetDepth();
		double dDnThick = pDnColumn->GetThick();

		MSCheckModelItem* pItem = MSCheckModelOption::GetInstance()->GetOptItem(CMO_VER_SEC, CMO_VER_SEC_SIZE_OVER);
		int nCheckVal = pItem->m_nCheckVal;
		bool bCheckDownVal = pItem->m_bCheckDownVal;

		if(bCheckDownVal == true)
		{
			if(fabs(dWidth-dDnWidth) <= nCheckVal ||
				fabs(dDepth-dDnDepth) <= nCheckVal ||
				fabs(dThick-dDnThick) <= nCheckVal )
			{
				CheckFlag.SetCheckedSecoundOption(CMO_VER_SEC, CMO_VER_SEC_SIZE_OVER, true);
				return false;
			}
		}
		else
		{
			if(fabs(dWidth-dDnWidth) > nCheckVal ||
				fabs(dDepth-dDnDepth) > nCheckVal ||
				fabs(dThick-dDnThick) > nCheckVal )
			{
				CheckFlag.SetCheckedSecoundOption(CMO_VER_SEC, CMO_VER_SEC_SIZE_OVER, true);
				return false;
			}
		}
	}

	return true;
}

bool MSModelChecker::CheckModel_ColumnEccOver(MSCompoG* pMyCompoG, MSCompoG* pUrCompoG, CheckModelOpt &CheckFlag)
{
	if(!MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_VER_OUTECC) )
		return true;

	if(pMyCompoG == NULL || pUrCompoG == NULL)
		return true;
	
	MSColumnG* pMyColumnG = (MSColumnG*)pMyCompoG;
	MSColumnG* pUrColumnG = (MSColumnG*)pUrCompoG;
	bool bRval = false;

	if(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_VER_OUTECC, CMO_VER_OUTECC_BELOW))
	{
		MSCheckModelItem* pItem = MSCheckModelOption::GetInstance()->GetOptItem(CMO_VER_OUTECC, CMO_VER_OUTECC_BELOW);
		int nCheckVal = pItem->m_nCheckVal;
		bool bCheckDownVal = pItem->m_bCheckDownVal;

		GM2DPolyline MyPoly(TRUE);
		GM2DPolyline UrPoly(TRUE);
		GM2DPolyline UrNewPoly(TRUE);

		pMyColumnG->GetBoundary(MyPoly);
		pUrColumnG->GetBoundary(UrPoly);
		if(UrPoly.Contains(&MyPoly) == false)
		{
			UrPoly.OffSet_Acis(nCheckVal, TRUE, UrNewPoly);

			bRval = UrNewPoly.Contains(&MyPoly);
			if(bRval == bCheckDownVal)
				CheckFlag.SetCheckedSecoundOption(CMO_VER_OUTECC, CMO_VER_OUTECC_BELOW, true);
		}
	}

	if(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_VER_OUTECC, CMO_VER_OUTECC_OVER))
	{
		MSCheckModelItem* pItem = MSCheckModelOption::GetInstance()->GetOptItem(CMO_VER_OUTECC, CMO_VER_OUTECC_OVER);
		if (pItem == nullptr)	 return false;
		int nCheckVal = pItem->m_nCheckVal;
		bool bCheckDownVal = pItem->m_bCheckDownVal;
		GM2DPolyline MyPoly(TRUE);
		GM2DPolyline UrPoly(TRUE);
		GM2DPolyline UrNewPoly(TRUE);

		pMyColumnG->GetBoundary(MyPoly);
		pUrColumnG->GetBoundary(UrPoly);
		if(UrPoly.Contains(&MyPoly) == false)
		{
			UrPoly.OffSet_Acis(nCheckVal, TRUE, UrNewPoly);

			bRval = UrNewPoly.Contains(&MyPoly);
			if(bRval == bCheckDownVal)
				CheckFlag.SetCheckedSecoundOption(CMO_VER_OUTECC, CMO_VER_OUTECC_OVER, true);
		}
	}

	if(CheckFlag.IsCheckedSecoundOption(CMO_VER_OUTECC, CMO_VER_OUTECC_BELOW) || CheckFlag.IsCheckedSecoundOption(CMO_VER_OUTECC, CMO_VER_OUTECC_OVER))
		return false;

	return true;
}

bool MSModelChecker::CheckModel_ZLevel_ConnectedOverLap(MSCompoG* pMyCompoG, MSCompoG* pUrCompoG, CheckModelOpt &CheckFlag)
{
	bool bRval = true;
	if(pMyCompoG == NULL || pUrCompoG == NULL)
		return bRval;

	if(!(MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_MODEL)))
		return true;
	if(!(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_ZLEVELOVERLAP)))
		return true;

	bRval = pMyCompoG->IsLevelConnectedOverLap(pUrCompoG, CheckFlag);
	return bRval;
}

bool MSModelChecker::CheckModel_SupportedOnMemberInFloor(MSFloor* pCheckFloor, MSCompoG* pCompoG, int nFloorType, vector<MSElement::Type_MSCompoG> CheckTypeArr, CheckModelOpt &CheckFlag)
{
	bool CheckSupportBelow = MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_VER_SUPPORT, CMO_VER_SUPPORT_BELOW);
	bool CheckSupportOver = MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_VER_SUPPORT, CMO_VER_SUPPORT_OVER);

	GM2DPolyline* pCompoGPoly = new GM2DPolyline(TRUE);
	pCompoG->GetBoundary(*pCompoGPoly);

	double dCompoGArea = AcisGeomHelper::GetArea(*pCompoGPoly);
	double dOrgCompoGArea = dCompoGArea;

	vector<GM2DPolyline*> CompoGPolyArr;
	CompoGPolyArr.push_back(pCompoGPoly);

	CTypedPtrArray<CObArray, MSFloorQT*> CrossQTArr;

	bool IsLineMember = pCompoG->IsLinkMember();
	shared_ptr<GM2DCurve> pMyCurve = NULL;
	if(IsLineMember)
		pMyCurve = ((MSLinkMemberG*)pCompoG)->GetCenCurve();

	bool HasCheckSlab = false;
	MSElement::Type_MSCompoG eMyType = pCompoG->GetType();

	MSFloor* pFloor = NULL;
	if(nFloorType == -1)
		pFloor = pCheckFloor->GetLowerFloor();
	else if(nFloorType == 1)
		pFloor = pCheckFloor->GetUpperFloor();
	else if(nFloorType == 0)
		pFloor = pCheckFloor;

	vector<MSCompoG*> FindCompoGArr = pCompoG->FindConnectMember(FindType_MSElement::msNULL, nFloorType);

	long NumType = CheckTypeArr.size();
	for(long iType = 0 ; iType < NumType ; iType++)
	{
		if(CheckTypeArr[iType] == MSElement::msSlabG)
		{
			HasCheckSlab = true;
			break;
		}
	}

	long nSupportCnt = 0;

	for(long iCompoG = 0 ; iCompoG < FindCompoGArr.size() ; iCompoG++)
	{
		MSCompoG* pQTCompoG = FindCompoGArr[iCompoG];
		if(pQTCompoG == NULL || pQTCompoG->GetType() == MSElement::msSlabG)
			continue;

		MSElement::Type_MSCompoG eUrType = pQTCompoG->GetType();

		bool IsContinue = true;

		long NumType = CheckTypeArr.size();
		for(long iType = 0 ; iType < NumType ; iType++)
		{
			if(eUrType == CheckTypeArr[iType])
			{
				IsContinue = false;
				break;
			}
		}

		if(IsContinue)
			continue;

		GM2DPolyline QTCompoGPoly(TRUE);
		
		GM2DLineVector MyLine;
		GM2DLineVector UrLine;
		
		shared_ptr<GM2DCurve> pUrCurve = NULL;

		//라인부재의 경우 평행한 라인부재 끼리 평행 검토함
		if(IsLineMember && pQTCompoG->IsLinkMember())
		{
			MSLinkMemberG* pLinkMem = (MSLinkMemberG*)pCompoG;
			MSLinkMemberG* pQTLinkMem = (MSLinkMemberG*)pQTCompoG;
			if(pLinkMem->IsParallel(pQTLinkMem, MSRebarBuilding::ms_CMColinearDegreeTol) == false)
				continue;

			// ZLevel, OverLap 검토
			bool IsConnected = pCompoG->IsLevelConnectedOverLap(pQTCompoG, CheckFlag);

			if(!IsConnected)
				continue;

			pUrCurve = pQTLinkMem->GetCenCurve();
		}

		// Wall일 경우 두깨, ECC 비교를 함
		if(eMyType == MSElement::msWallG && eUrType == MSElement::msWallG )
		{
			if(MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_VER_SEC) ||
				MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_VER_OUTECC))
			{
				if(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_VER_SEC, CMO_VER_SEC_SIZE_BELOW) == false ||
					MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_VER_SEC, CMO_VER_SEC_SIZE_OVER) == false)
				{
					double dMyThick = pCompoG->GetThick();
					double dUrthick = pQTCompoG->GetThick();
					double dThickDist = 0;

					if(nFloorType == 1)
						dThickDist = dMyThick - dUrthick;
					else if(nFloorType == -1)
						dThickDist = dUrthick - dMyThick;
					else
						dThickDist = fabs(dMyThick - dUrthick);
					SetCheckModelOptFlag(dThickDist, CheckFlag, CMO_VER_SEC, CMO_VER_SEC_SIZE_BELOW);
					SetCheckModelOptFlag(dThickDist, CheckFlag, CMO_VER_SEC, CMO_VER_SEC_SIZE_OVER);
				}

				if(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_VER_OUTECC, CMO_VER_OUTECC_BELOW) == false ||
					MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_VER_OUTECC, CMO_VER_OUTECC_OVER) == false)
				{
					double dLEcc = 0;
					double dREcc = 0;
					MSWallMemberG* pWall = (MSWallMemberG*)pCompoG;
					MSWallMemberG* pUrWall = (MSWallMemberG*)pQTCompoG;
					pWall->GetUrLinkMemberEcc(pUrWall, dLEcc, dREcc);
					
					double dEcc = max(fabs(dLEcc), fabs(dREcc));

					SetCheckModelOptFlag(dEcc, CheckFlag, CMO_VER_OUTECC, CMO_VER_OUTECC_BELOW);
					SetCheckModelOptFlag(dEcc, CheckFlag, CMO_VER_OUTECC, CMO_VER_OUTECC_OVER);
				}
			}
		}

		if(CheckSupportBelow == false && CheckSupportOver == false)
			continue;

		pQTCompoG->GetBoundary(QTCompoGPoly);
		nSupportCnt++;

		double dCheckArea = AcisGeomHelper::GetArea(QTCompoGPoly);

		for(long i = 0 ; i < CompoGPolyArr.size() ; i++)
		{
			GM2DPolyline* pPoly = CompoGPolyArr[i];
			GM2DBound MyBound;
			pPoly->Get2DBound(MyBound);

			vector<GM2DPolyline*> Joined_Polys;
			pPoly->BoolOperationNSortByArea('A', &QTCompoGPoly, Joined_Polys, DTOL_GM);
			//AcisGeomHelper::BooleanPolyline('A', *pPoly, QTCompoGPoly, Joined_Polys);
			
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
				double dSumArea = 0;
				double dOutArea = 0;
				long nCheck = 0;
				for(long j = 0 ; j < NumJoinedPolys ; j++)
				{
					GM2DPolyline* pJoinedPoly = Joined_Polys[j];
					dJoindArea = AcisGeomHelper::GetArea(*pJoinedPoly);

					//'A' BooleanPolyline 했지만 영역이 그대로인 경우
					if(NumJoinedPolys == 1)
					{
						if(pPoly->SamePolyLine(pJoinedPoly))
						{
							dSumArea = dCompoGArea;
							break;
						}
					}

					//완전 포함
					if( fabs(dCheckArea - dJoindArea) < DTOL_GM)
					{
						dCompoGArea = dCompoGArea - dJoindArea;
						dOutArea = dJoindArea;
					}
					else
					{
						if(nCheck == 0)
						{
							pPoly->CopyFrom(pJoinedPoly);
							delete pJoinedPoly;
						}
						else
							CompoGPolyArr.push_back(pJoinedPoly);
						dSumArea += dJoindArea;
						nCheck++;
					}
				}
				dCompoGArea = dSumArea - dOutArea;
			}

			if(dCompoGArea < DTOL_GM)
				break;
		}
		if(dCompoGArea < DTOL_GM)
			break;
	}
	
	if((CheckSupportBelow == true || CheckSupportOver == true ) && 
		HasCheckSlab && dCompoGArea > DTOL_GM && pFloor != NULL)
	{
		vector<MSCompoG*> SlabArr;
		if(IsLineMember)
		{
			MSLinkMemberG* pLinkMem = (MSLinkMemberG*)pCompoG;
			SlabArr = pLinkMem->FindConnectMember(CON_ALL, FindType_MSElement::msSlabG, nFloorType);
		}
		else
			SlabArr = pCompoG->FindConnectMember(FindType_MSElement::msSlabG, nFloorType);

		for(long iSlab = 0 ; iSlab < SlabArr.size() ; iSlab++)
		{
			MSSlabG* pSlabG = (MSSlabG*)SlabArr[iSlab];

			MSRcSlabM* pSlabM = (MSRcSlabM*)pSlabG->GetCompoM()->mp_RebarM;
			if(pSlabM == NULL)	continue;
			if(!(pSlabM->m_nSlabMType == MSRcSlabM::MAT_TYPE || pSlabM->m_nSlabMType == MSRcSlabM::FLAT_TYPE ||
				pSlabM->m_nSlabMType == MSRcSlabM::SOG || pSlabM->m_nSlabMType == MSRcSlabM::CIRCULAR_RAMP))
				continue;

			//아래 연결된 부재만 찾는다.
			CheckModelOpt SlabCheckOpt;
			bool bConnOverlap = pCompoG->IsLevelConnectedOverLap(pSlabG, SlabCheckOpt);
			if(bConnOverlap == false)
				continue;
			long NumCheck = SlabCheckOpt.GetCheckCnt();
			if(NumCheck > 0)
				continue;

			nSupportCnt++;
			MSPolylineObject* pCurPolylineObject = pSlabG->mp_PolylineInfo;
			GM2DPolyline* pSlabPoly = pCurPolylineObject->GetProfile();
			GM2DBound SlabBound;
			pSlabPoly->Get2DBound(SlabBound);

			for(long i = 0 ; i < CompoGPolyArr.size() ; i++)
			{
				GM2DPolyline* pPoly = CompoGPolyArr[i];
				GM2DBound MyBound;
				pPoly->Get2DBound(MyBound);

				if(MyBound.OverLapTo(&SlabBound))
				{
					vector<GM2DPolyline*> Joined_Polys;
					pPoly->BoolOperationNSortByArea('A', pSlabPoly, Joined_Polys, DTOL_GM);
					//AcisGeomHelper::BooleanPolyline('A', *pPoly, *pSlabPoly, Joined_Polys);
					bool bContain = pPoly->Contains(pSlabPoly);
					long NumJoinedPolys = Joined_Polys.size();
					double dJoindArea = 0;
					if (bContain)
					{
						dJoindArea = AcisGeomHelper::GetArea(*pSlabPoly);
						dCompoGArea = dCompoGArea - dJoindArea;
					}
					else if (NumJoinedPolys == 0)
					{
						//완전 포함
						dJoindArea = AcisGeomHelper::GetArea(*pPoly);
						dCompoGArea = dCompoGArea - dJoindArea;
					}
					else
					{
						for(long j = 0 ; j < NumJoinedPolys ; j++)
						{
							GM2DPolyline* pJoinedPoly = Joined_Polys[j];
							dJoindArea = AcisGeomHelper::GetArea(*pJoinedPoly);
							if(j == 0)
							{
								pPoly->CopyFrom(pJoinedPoly);
								delete pJoinedPoly;
							}
							else
								CompoGPolyArr.push_back(pJoinedPoly);

							if(dCompoGArea > dJoindArea)
								dCompoGArea = dJoindArea;
						}
					}
				}

				if(dCompoGArea < DTOL_GM)
					break;
			}
			if(dCompoGArea < DTOL_GM)
				break;
		}
	}

	for(long i = 0 ; i < CompoGPolyArr.size() ; i++)
		delete CompoGPolyArr[i];
	
	double dCompoGRatio = 100;
	if(dOrgCompoGArea != 0)
		dCompoGRatio= (dOrgCompoGArea - dCompoGArea) / dOrgCompoGArea * 100;

	if(nSupportCnt == 0)
		return true;

	if(CheckSupportBelow == true)
		SetCheckModelOptFlag(dCompoGRatio, CheckFlag, CMO_VER_SUPPORT, CMO_VER_SUPPORT_BELOW);
	if(CheckSupportOver == true)
		SetCheckModelOptFlag(dCompoGRatio, CheckFlag, CMO_VER_SUPPORT, CMO_VER_SUPPORT_OVER);

	if(dCompoGRatio > 100 -DTOL_GM)
		return true;

	if(CheckFlag.IsCheckedSecoundOption(CMO_VER_SUPPORT, CMO_VER_SUPPORT_BELOW) ||
		CheckFlag.IsCheckedSecoundOption(CMO_VER_SUPPORT, CMO_VER_SUPPORT_OVER))
		return false;
		
	return true;
}


XErrorObject* MSModelChecker::CheckModel_MakeErrObject(MSElement* pElement, eCheckModel eBaseOpt, long eSecondOpt)
{
	XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();
	XErrorObject::SelectType eSelType = XErrorObject::MSElementIDSelect;

	CString strMsgObj, strErrMsg, strMsg;
	CString ErrorCodeStr = CheckModelOpt::GetErrorCodeStr(eBaseOpt, eSecondOpt);
	long ErrorCode = CheckModelOpt::GetErrorCode(eBaseOpt, eSecondOpt);
	int Level = MSCheckModelOption::GetInstance()->GetErrorLevel(eBaseOpt, eSecondOpt);
	int nCheckVal = MSCheckModelOption::GetInstance()->GetCheckVal(eBaseOpt, eSecondOpt);
	bool isUseCheckVal = MSCheckModelOption::GetInstance()->isUseCheckVal(eBaseOpt, eSecondOpt);
	MSFloor* pFloor = pElement->GetFloor();
	MSBaseBuilding* pBuild = pFloor->GetBuilding();

	strMsg = LocalDic::GetLocal(ErrorCodeStr); //국제화에 따른 변경
	

	CFormatMsg strTmp;
	if(pElement->IsCompoG())
	{
		MSCompoG* pCompoG = (MSCompoG*)pElement;
		MSFloor* pUserFloor = pCompoG->GetFloorForUser();
//		int nFloorNum1 = pCompoG->GetFloorNumForUser();
		strMsgObj.Format(_T("%s %s"), pCompoG->GetCompoM()->m_Name, pCompoG->GetGroupName());
		if(isUseCheckVal)
		{	
			if(eBaseOpt == CMO_VER_SUPPORT && eSecondOpt == CMO_VER_SUPPORT_OVER)
				strTmp.Format(strMsg, strMsgObj, 100-nCheckVal);
			else
				strTmp.Format(strMsg, strMsgObj, nCheckVal);
		}
		else
			strTmp.Format(strMsg, strMsgObj);
		
		strErrMsg.Format(_T("%s : %s"), pUserFloor->GetName(), strTmp);
	}
	else if(pElement->GetType() == MSElement::msConstructionZone)
	{
		MSFloor* pUserFloor = pFloor->GetUpperFloor();
		if (pUserFloor == nullptr)
		{
			ASSERT(0);
			pUserFloor = pFloor;
		}

		MSConstructionZone* pCZone = (MSConstructionZone*)pElement;
		strMsgObj.Format(_T("%s %s"), pCZone->GetName(), pElement->GetGroupName());
		strTmp.Format(strMsg, strMsgObj);
		strErrMsg.Format(_T("%s : %s"), pUserFloor->GetName(), strTmp);
	}
	else if(pElement->GetType() == MSElement::msLevelZone)
	{
		MSFloor* pUserFloor = pFloor->GetUpperFloor();
		if (pUserFloor == nullptr)
		{
			ASSERT(0);
			pUserFloor = pFloor;
		}

		MSLevelZone* pLevelZone = (MSLevelZone*)pElement;
		strMsgObj.Format(LocalDic::GetLocal(L"%s(Level:%.0f)", L"StructureModel"), pElement->GetGroupName(), pLevelZone->m_Level);
		strTmp.Format(strMsg, strMsgObj);
		strErrMsg.Format(_T("%s :  %s"), pUserFloor->GetName(), strTmp);
	}
	else
	{
		ASSERT(0);
	}
	


	XErrorObject* pError = pMessageContainer->CreateErrorObject(ErrorCode, Level, strErrMsg, eSelType);
	pError->SetGroupName(pElement->GetGroupName());
	pError->AddCompoGID(pElement->m_ID);
	if(pFloor != NULL)
		pError->m_nFloorID = pFloor->m_ID;
	pMessageContainer->AddErrorObject(pError);

	return pError;
}

long MSModelChecker::CheckModel_MakeErrObjectbyCheckModel(MSElement* pElement, CheckModelOpt& CheckFlag, eCheckModel eBaseOpt, long nCnt)
{
	long nErrorCnt = 0;
	long nSecondOpt = 2;
	for(long i = 0 ; i < nCnt ; i++)
	{
		if(CheckFlag.IsCheckedSecoundOption(eBaseOpt, nSecondOpt) && MSCheckModelOption::GetInstance()->IsCheckedBaseOption(eBaseOpt) && MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(eBaseOpt, nSecondOpt))
		{
			if ((eBaseOpt == CMO_MODEL && nSecondOpt == CMO_MODEL_HOR_CANTIL) &&
				MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_MODEL) && MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_CONTAIN) &&
				CheckFlag.IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_CONTAIN))
			{
				nSecondOpt = nSecondOpt << 1;
				continue;
			}

			if ((eBaseOpt == CMO_MODEL && nSecondOpt == CMO_MODEL_SAMEMEMBER_OVERLAP) &&
				MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_MODEL) && MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_CONTAIN) &&
				CheckFlag.IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_CONTAIN))
			{
				nSecondOpt = nSecondOpt << 1;
				continue;
			}

			if ((eBaseOpt == CMO_MODEL && (nSecondOpt == CMO_MODEL_HOR_ZLEVELOVERLAP || nSecondOpt == CMO_MODEL_VER_ZLEVELOVERLAP)) &&
				MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_MODEL) && MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_SAMEMEMBER_OVERLAP) &&
				CheckFlag.IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_SAMEMEMBER_OVERLAP))
			{
				nSecondOpt = nSecondOpt << 1;
				continue;
			}

			if ((eBaseOpt == CMO_MODEL && (nSecondOpt == CMO_MODEL_HOR_CANTIL || nSecondOpt == CMO_MODEL_VER_CANTIL)) &&
				MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_MODEL) && MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_SAMEMEMBER_OVERLAP) &&
				CheckFlag.IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_SAMEMEMBER_OVERLAP))
			{
				nSecondOpt = nSecondOpt << 1;
				continue;
			}

			MSModelChecker::CheckModel_MakeErrObject(pElement, eBaseOpt, nSecondOpt);
			nErrorCnt++;
		}
		nSecondOpt = nSecondOpt << 1;
	}
	return nErrorCnt;
}

long MSModelChecker::CheckModel_MakeErrorObjects(MSElement* pElement, CheckModelOpt& CheckFlag)
{
	if(CheckFlag.GetCheckCnt() == 0)
		return 0;

	int nErrorCnt = 0;
	
	nErrorCnt += CheckModel_MakeErrObjectbyCheckModel(pElement, CheckFlag, CMO_GEN, CMO_GEN_CNT);
	nErrorCnt += CheckModel_MakeErrObjectbyCheckModel(pElement, CheckFlag, CMO_MODEL, CMO_MODEL_CNT);
	nErrorCnt += CheckModel_MakeErrObjectbyCheckModel(pElement, CheckFlag, CMO_VER_SEC, CMO_VER_SEC_CNT);
	nErrorCnt += CheckModel_MakeErrObjectbyCheckModel(pElement, CheckFlag, CMO_VER_OUTECC, CMO_VER_OUTECC_CNT);
	nErrorCnt += CheckModel_MakeErrObjectbyCheckModel(pElement, CheckFlag, CMO_VER_SUPPORT, CMO_VER_SUPPORT_CNT);
	nErrorCnt += CheckModel_MakeErrObjectbyCheckModel(pElement, CheckFlag, CMO_HOR_SEC, CMO_HOR_SEC_CNT);
	nErrorCnt += CheckModel_MakeErrObjectbyCheckModel(pElement, CheckFlag, CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_CNT);

	return nErrorCnt;
}

long MSModelChecker::CheckModel_BeamLines(vector<MSColineLinkMemberGs*>& BeamLineArr)
{
	long NumBeamLine = BeamLineArr.size();
	
	CString strMsg, GroupName;
	int Level = 1;
	int ErrorCode = 0;
	int nErrorCnt = 0;

	XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();
	XErrorObject::SelectType eSelType = XErrorObject::MSElementIDSelect;

	map<long, MSLinkMemberG*> BeamMapZLevel;
	map<long, MSLinkMemberG*> BeamMapWidth;
	map<long, MSLinkMemberG*> BeamMapDepth;
	
	for(long iBeamLine = 0 ; iBeamLine < NumBeamLine ; iBeamLine++)
	{
		MSColineLinkMemberGs* pBeamLine = BeamLineArr[iBeamLine];
		double dMostlyBeamWidth = pBeamLine->GetMostlyWidth();
		double dMostlyBeamDepth = pBeamLine->GetMostlyDepth();
		double dMostlyBeamZLevel = pBeamLine->GetMostlyZLevel();

		map<long, MSLinkMemberG*>::iterator it;
		for(it = pBeamLine->mm_LinkMem.begin() ; it != pBeamLine->mm_LinkMem.end() ; it++)
		{
			MSLinkMemberG* pLinkMem = it->second;
			double dWidth = pLinkMem->GetWidth();
			double dDepth = pLinkMem->GetDepth();

			CheckModelOpt CheckFlag;

			double dCheckWidth = fabs(dWidth - dMostlyBeamWidth);
			SetCheckModelOptFlag(dCheckWidth, CheckFlag, CMO_HOR_SEC, CMO_HOR_SEC_BEAM_WIDTH_BELOW);
			SetCheckModelOptFlag(dCheckWidth, CheckFlag, CMO_HOR_SEC, CMO_HOR_SEC_BEAM_WIDTH_OVER);

			double dCheckDepth = fabs(dDepth - dMostlyBeamDepth);
			SetCheckModelOptFlag(dCheckDepth, CheckFlag, CMO_HOR_SEC, CMO_HOR_SEC_BEAM_HEIGHT_BELOW);
			SetCheckModelOptFlag(dCheckDepth, CheckFlag, CMO_HOR_SEC, CMO_HOR_SEC_BEAM_HEIGHT_OVER);

			long NumDif = pLinkMem->ma_MemberLevelDif.GetSize();
			if(NumDif == 0)
			{
				double dSTopLevel = 0;
				double dSBotLevel = 0;
				double dETopLevel = 0;
				double dEBotLevel = 0;
				pLinkMem->GetZLevelsbyLoc(dSTopLevel, dSBotLevel, 0);
				pLinkMem->GetZLevelsbyLoc(dETopLevel, dEBotLevel, 1);

				double dCheckSTopLevel = fabs(dSTopLevel - dMostlyBeamZLevel);
				double dCheckETopLevel = fabs(dETopLevel - dMostlyBeamZLevel);
				SetCheckModelOptFlag(dCheckSTopLevel, CheckFlag, CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_BELOW);
				SetCheckModelOptFlag(dCheckETopLevel, CheckFlag, CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_BELOW);

				SetCheckModelOptFlag(dCheckSTopLevel, CheckFlag, CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_OVER);
				SetCheckModelOptFlag(dCheckETopLevel, CheckFlag, CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_OVER);
			}
			else
			{
				for(long i = 0 ; i < NumDif ; i++)
				{
					MSMemberLevelDif* pDif = pLinkMem->ma_MemberLevelDif[i];
					double dTopLevel = pDif->m_StartLevel + pLinkMem->GetOffSetbyLoc(pDif->m_Loc) + pLinkMem->GetNextSlabLevel();
					double dCheckTopLevel = fabs(dTopLevel - dMostlyBeamZLevel);
					
					SetCheckModelOptFlag(dCheckTopLevel, CheckFlag, CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_BELOW);
					SetCheckModelOptFlag(dCheckTopLevel, CheckFlag, CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_OVER);

					if(i == NumDif - 1)
					{
						dTopLevel = pDif->m_EndLevel + pLinkMem->GetOffSetbyLoc(pDif->m_Loc) + pLinkMem->GetNextSlabLevel();
						dCheckTopLevel = fabs(dTopLevel - dMostlyBeamZLevel);

						SetCheckModelOptFlag(dCheckTopLevel, CheckFlag, CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_BELOW);
						SetCheckModelOptFlag(dCheckTopLevel, CheckFlag, CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_OVER);
					}
				}
			}

			if(BeamMapWidth.find(pLinkMem->m_ID) == BeamMapWidth.end())
			{
				nErrorCnt += CheckModel_MakeErrorObjects(pLinkMem, CheckFlag);
				BeamMapWidth.insert(make_pair(pLinkMem->m_ID, pLinkMem));
			}
		}
	}
	return nErrorCnt;
}

void MSModelChecker::CheckModel_MakeBeamLine(MSBeamG* pBeam, map<long, MSLinkMemberG*>& beamMap, vector<MSColineLinkMemberGs*>& BeamLineArr)
{
	bool IsCheckedLevel = false;
	if(MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_HOR_LEVELDIF) == true &&
		(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_BELOW) == true ||
		MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_OVER) == true))
		IsCheckedLevel = true;

	bool IsCheckedDepth = false;
	if(MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_HOR_SEC) == true &&
		(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_HOR_SEC, CMO_HOR_SEC_BEAM_HEIGHT_BELOW) == true ||
		MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_HOR_SEC, CMO_HOR_SEC_BEAM_HEIGHT_OVER) == true))
		IsCheckedDepth = true;

	bool IsCheckedWidth = false;
	if(MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_HOR_SEC) == true &&
		(MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_HOR_SEC, CMO_HOR_SEC_BEAM_WIDTH_BELOW) == true ||
		MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_HOR_SEC, CMO_HOR_SEC_BEAM_WIDTH_OVER) == true))
		IsCheckedWidth = true;

	if(!IsCheckedLevel && !IsCheckedDepth && !IsCheckedWidth)
		return;

// 	map<long, MSBeamG*>::iterator it;
	map<long, MSLinkMemberG*>::iterator it;
	it = beamMap.find(pBeam->m_ID);
	if(it == beamMap.end())
	{
		MSColineLinkMemberGs* pBeamLine = MSLinkMemberG::MakeColineLinkMemberG(pBeam, FindType_MSElement::msBeamG, beamMap);
		BeamLineArr.push_back(pBeamLine);

		/*
		MSColineLinkMemberGs* pBeamLine = NULL;
		pBeamLine = new MSColineLinkMemberGs();
		BeamLineArr.push_back(pBeamLine);
		beamMap.insert(make_pair(pBeam->m_ID, pBeam));


		pBeamLine->Add((MSLinkMemberG*)pBeam);

		MSBeamG* pNextBeam = NULL;

		vector<MSCompoG*> NextBeamArr = pBeam->FindColinerLinkMember(CON_START, MSElement::msBeamG, 0, MSCheckModelOption::GetInstance()->m_dColinearDegreeTol, pBeam->GetWidth()/2, pBeam->GetLength() * 0.2);
		if(NextBeamArr.size() > 0)
			pNextBeam = (MSBeamG*)NextBeamArr[0];
		long nRval = pBeamLine->Add((MSLinkMemberG*)pNextBeam);
		if(nRval != 0)
			beamMap.insert(make_pair(pNextBeam->m_ID, pNextBeam));

		while(pNextBeam != NULL)
		{
			MSBeamG* pSBeam = NULL;
			MSBeamG* pEBeam = NULL;

			vector<MSCompoG*> SNextBeamArr = pNextBeam->FindColinerLinkMember(CON_START, MSElement::msBeamG, 0, MSCheckModelOption::GetInstance()->m_dColinearDegreeTol, pBeam->GetWidth()/2, pBeam->GetLength() * 0.2);
			if(SNextBeamArr.size() > 0)
				pSBeam = (MSBeamG*)SNextBeamArr[0];

			vector<MSCompoG*> ENextBeamArr = pNextBeam->FindColinerLinkMember(CON_END, MSElement::msBeamG, 0, MSCheckModelOption::GetInstance()->m_dColinearDegreeTol, pBeam->GetWidth()/2, pBeam->GetLength() * 0.2);
			if(ENextBeamArr.size() > 0)
				pEBeam = (MSBeamG*)ENextBeamArr[0];

			long nSRval = pBeamLine->Add((MSLinkMemberG*)pSBeam);
			long nERval = pBeamLine->Add((MSLinkMemberG*)pEBeam);

			if(nSRval != 0)
			{
				beamMap.insert(make_pair(pSBeam->m_ID, pSBeam));
				pNextBeam = pSBeam;
			}
			if(nERval != 0)
			{
				beamMap.insert(make_pair(pEBeam->m_ID, pEBeam));
				pNextBeam = pEBeam;
			}
			if(nSRval == 0 && nERval == 0)
				pNextBeam = NULL;

			if(nSRval != 0 && nERval != 0)
				ASSERT(0);
		}


		NextBeamArr = pBeam->FindColinerLinkMember(CON_END, MSElement::msBeamG, 0, MSCheckModelOption::GetInstance()->m_dColinearDegreeTol, pBeam->GetWidth()/2, pBeam->GetLength() * 0.2);
		if(NextBeamArr.size() > 0)
			pNextBeam = (MSBeamG*)NextBeamArr[0];

		nRval = pBeamLine->Add((MSLinkMemberG*)pNextBeam);
		if(nRval != 0)
			beamMap.insert(make_pair(pNextBeam->m_ID, pNextBeam));

		while(pNextBeam != NULL)
		{
			MSBeamG* pSBeam = NULL;
			MSBeamG* pEBeam = NULL;

			vector<MSCompoG*> SNextBeamArr = pNextBeam->FindColinerLinkMember(CON_START, MSElement::msBeamG, 0, MSCheckModelOption::GetInstance()->m_dColinearDegreeTol, pBeam->GetWidth()/2, pBeam->GetLength() * 0.2);
			if(SNextBeamArr.size() > 0)
				pSBeam = (MSBeamG*)SNextBeamArr[0];

			vector<MSCompoG*> ENextBeamArr = pNextBeam->FindColinerLinkMember(CON_END, MSElement::msBeamG, 0, MSCheckModelOption::GetInstance()->m_dColinearDegreeTol, pBeam->GetWidth()/2, pBeam->GetLength() * 0.2);
			if(ENextBeamArr.size() > 0)
				pEBeam = (MSBeamG*)ENextBeamArr[0];

			long nSRval = pBeamLine->Add((MSLinkMemberG*)pSBeam);
			long nERval = pBeamLine->Add((MSLinkMemberG*)pEBeam);

			if(nSRval != 0)
			{
				beamMap.insert(make_pair(pSBeam->m_ID, pSBeam));
				pNextBeam = pSBeam;
			}
			if(nERval != 0)
			{
				beamMap.insert(make_pair(pEBeam->m_ID, pEBeam));
				pNextBeam = pEBeam;
			}
			if(nSRval == 0 && nERval == 0)
				pNextBeam = NULL;

			if(nSRval != 0 && nERval != 0)
				ASSERT(0);
		}
		*/
	}
}

void MSModelChecker::CheckModel_Connection(MSCompoG* pCompoG, CheckModelOpt &CheckFlag, bool bCheckCantil)
{
	if(!MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_MODEL))
		return;

	bool bCheckConnection = MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_NOT_CONNECTION);
	if(bCheckCantil)
		bCheckCantil = MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_CANTIL);
	
	bool bCheckZLevelOverlap = MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_ZLEVELOVERLAP);


	if(pCompoG->IsVerticalMember())
	{
		if(bCheckConnection || bCheckCantil || bCheckZLevelOverlap)
		{
			// 수직 부재는 트림된 바운더리 사용 안함
			vector<MSCompoG*> LowerCompoGArr = pCompoG->FindConnectMember(FindType_MSElement::msNULL, -1, DTOL_GM, false);
			vector<MSCompoG*> CurCompoGArr = pCompoG->FindConnectMember(FindType_MSElement::msNULL, 0, DTOL_GM, false);
			vector<MSCompoG*> UpperCompoGArr = pCompoG->FindConnectMember(FindType_MSElement::msNULL, 1, DTOL_GM, false);

			long NumLowerCompoG = LowerCompoGArr.size();
			long NumCurCompoG = CurCompoGArr.size() + UpperCompoGArr.size();
			if( NumLowerCompoG > 0 && NumCurCompoG > 0)
			{
				if(bCheckCantil)
					CheckFlag.SetCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_CANTIL, false);

				CheckFlag.SetCheckedSecoundOption(CMO_MODEL, CMO_MODEL_NOT_CONNECTION, false);
			}
			else if( NumLowerCompoG == 0 && NumCurCompoG == 0)
			{
				if(bCheckConnection)
					CheckFlag.SetCheckedSecoundOption(CMO_MODEL, CMO_MODEL_NOT_CONNECTION, true);
			}
			else if( NumLowerCompoG == 0 || NumCurCompoG == 0)
			{
				if(bCheckCantil)
					CheckFlag.SetCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_CANTIL, true);
			}

			if(bCheckZLevelOverlap == true && CheckFlag.IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_ZLEVELOVERLAP) == false) 
			{
				for(long iLow = 0 ; iLow < LowerCompoGArr.size() ; iLow++)
				{
					MSCompoG* pLowCompoG = LowerCompoGArr[iLow];
					if(pLowCompoG->IsVerticalMember())
					{
						if(pCompoG->IsLinkMember() && pLowCompoG->IsLinkMember())
						{
							MSLinkMemberG* pLinkG = (MSLinkMemberG*)pCompoG;
							MSLinkMemberG* pLowLinkG = (MSLinkMemberG*)pLowCompoG;
							if(pLinkG->IsParallel(pLowLinkG))
								pCompoG->IsLevelConnectedOverLap(pLowCompoG, CheckFlag);
						}
						else
							pCompoG->IsLevelConnectedOverLap(pLowCompoG, CheckFlag);
					}
				}
			}
		}
	}
	else if(pCompoG->IsFloorMember())
	{
		if(bCheckConnection)
		{
			vector<MSCompoG*> CompoGArr = pCompoG->FindConnectMember(FindType_MSElement::msNULL, FIND_ALLFLOOR);
			if(CompoGArr.size() == 0 )
			{
				if(bCheckConnection)
				{
					CheckFlag.SetCheckedSecoundOption(CMO_MODEL, CMO_MODEL_NOT_CONNECTION, true);
					CheckFlag.SetCheckedSecoundOption(CMO_MODEL, CMO_MODEL_HOR_CANTIL, false);
				}
			}
		}
	}
	else
	{
		ASSERT(0);
	}
}
bool MSModelChecker::CheckModel_WallOpening(MSWallMemberG* pWall, bool bMakeErrorObject)
{
	map<long, MSWallOpeningG*>::iterator it;
	for (it = pWall->mm_WallOpening.begin(); it != pWall->mm_WallOpening.end();  it++)
	{
		MSWallOpeningG* pOpeningG = it->second;
		if (CheckModel_WallOpeningStep2(pWall, pOpeningG, bMakeErrorObject) == false)
			return false;
	}
	return true;
}

bool MSModelChecker::CheckModel_WallOpeningStep2(MSWallMemberG* pWall, MSWallOpeningG* pOpening, bool bMakeErrorObject)
{
	double dFloorLevel = pWall->GetFloorLevel();
	double dSlabLevel = pWall->GetSlabLevel();
	double dSlabLevelOffset = dSlabLevel - dFloorLevel;

	MSOpeningM* pOpeningM = (MSOpeningM*)pOpening->GetCompoM();
	if (pOpeningM == nullptr) return true;

	double dOpeningHeight = pOpeningM->GetHeight();
	double dOpeningBotLevel = 0;
	double dOpeningTopLevel = 0;

	bool IsOutLocation = false;
	double dSLoc= 2, dELoc = -1;

	double dWallLen = pWall->GetLength();
	double tolLoc = (1 + pOpening->GetExtraSize()) / dWallLen;

	shared_ptr<GM2DCurve> pCenCurve = pWall->GetCenCurve();
	shared_ptr<GM2DCurve> extendCenCurve = pWall->GetExtendLine('C');
	double sExLoc = pCenCurve->OrthoBaseLocValue(extendCenCurve->GetP0());
	double eExLoc = pCenCurve->OrthoBaseLocValue(extendCenCurve->GetP1());

	bool bRavl = pOpening->GetOpeningLoc(dSLoc, dELoc);
	if (bRavl == false || dSLoc + tolLoc < sExLoc || dELoc - tolLoc > eExLoc)
		IsOutLocation = true;

	if (IsOutLocation == false)
	{
		bool IsOpenToWallBottom = false;
		if (pOpeningM->GetType() == MSCompoM::msDoorM)
		{
			MSDoorM* pDoor = (MSDoorM*)pOpeningM;
			IsOpenToWallBottom = pDoor->GetIsOpenToWallBottom();
		}

		if (IsOpenToWallBottom)
		{
			double dWallTopZLevel = 0;
			double dWallBotZLevel = 0;
			pWall->GetZLevels(dWallTopZLevel, dWallBotZLevel);
			pOpening->GetZLevel(dOpeningTopLevel, dOpeningBotLevel, false, true, false);

			if (dOpeningBotLevel < dWallBotZLevel)
				dOpeningBotLevel = dWallBotZLevel;
			if (dOpeningTopLevel < dWallBotZLevel)
				dOpeningTopLevel = dWallBotZLevel;

			double dUsedOpeningHeight = dOpeningTopLevel - dOpeningBotLevel;

			// Door의 오프닝이 벽체 아래까지 존재 할경우 생성된 벽체 오프닝의 높이가 DTOL1 보다 커야함
			// 오프닝의 TopLevel이 벽체의 TopLevel 같거나 작아야함
			if (dUsedOpeningHeight > DTOL1 && dOpeningTopLevel <= dWallTopZLevel)
				return true;
		}
		else
		{
			// 마감 로직으로 처리
			return true;
		}
	}

	if (bMakeErrorObject == true)
	{
		CString strGroup = LocalDic::GetLocal(L"모델링 오류", L"StructureModel");
		CString strErrMsg;
		MSFloor* pFloor = pWall->GetFloor();
		strErrMsg.Format(LocalDic::GetLocal(L"Wall(%%d) 밖에 Opening이 존재 합니다.", L"StructureModel"));
		MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 1, MESSAGE_LEVEL_WARNING, MESSAGE_WARNING_WALLOPENINGLOC, pWall);
	}

	return false;
}

bool MSModelChecker::CheckModel_LineMemberOutLen(MSCompoG* pCompoG, CheckModelOpt& CheckFlag)
{
	if(!MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_GEN))
		return true;
	if(!MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_GEN, CMO_GEN_LINEMEMBEROUTLEN))
		return true;

	if(CheckFlag.IsCheckedSecoundOption(CMO_GEN, CMO_GEN_LINEMEMBEROUTLEN))
		return false;

	MSLinkMemberG* pLinkG = (MSLinkMemberG*)pCompoG;
	shared_ptr<GM2DCurve> pLLine, pRLine;
	shared_ptr<GM2DCurve> pLLine2, pRLine2;
	shared_ptr<GM2DCurve> pLLine3, pRLine3;

	double dCheckVal = MSCheckModelOption::GetInstance()->GetCheckVal(CMO_GEN, CMO_GEN_LINEMEMBEROUTLEN);
	//라인의 시작과 끝쪽에 dCheckVal 크기 이하의 선분이 있는지 비교
	pLinkG->GetTrimmedLRLines(pLLine, pRLine, true, true, dCheckVal);
	pLinkG->GetTrimmedLRLines(pLLine2, pRLine2, true, false, dCheckVal);
	pLinkG->GetTrimmedLRLines(pLLine3, pRLine3, false, true, dCheckVal);

	double dLRebarLen = pLLine->Length();
	double dRRebarLen = pRLine->Length();
	double dLLen = pLLine2->Length();
	double dRLen = pRLine2->Length();
	double dLLen3 = pLLine3->Length();
	double dRLen3 = pRLine3->Length();

	double dMinDist = min(fabs(dLRebarLen - dLLen), fabs(dRRebarLen - dRLen));

	if(dMinDist < MSFloor::ms_DefLineHitDistTol)
		return true;

	dMinDist = min(fabs(dLLen3 - dLLen), fabs(dRLen3 - dRLen));
	SetCheckModelOptFlag(dMinDist, CheckFlag, CMO_GEN, CMO_GEN_LINEMEMBEROUTLEN);

	if(CheckFlag.IsCheckedSecoundOption(CMO_GEN, CMO_GEN_LINEMEMBEROUTLEN))
		return false;

	return true;
}

bool MSModelChecker::CheckModel_HorOverLap(MSLinkMemberG* pLink, CheckModelOpt& CheckFlag)
{
	if(!MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_MODEL))
		return true;
	if(!MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_HOR_ZLEVELOVERLAP))
		return true;

	if(CheckFlag.IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_HOR_ZLEVELOVERLAP))
		return false;

	GM2DLineVector SLine(pLink->mp_LLine->m_P0, pLink->mp_RLine->m_P0);
	GM2DLineVector ELine(pLink->mp_LLine->m_P1, pLink->mp_RLine->m_P1);

	long NumConn = pLink->ma_Connections.size();
	for(long iConn = 0 ; iConn < NumConn ; iConn++)
	{
		MSConnection* pConn = pLink->ma_Connections[iConn];
		if(pConn->IsParallel() == false)
			continue;
		MSLinkMemberG* pUrLink = (MSLinkMemberG*)pConn->GetPartner(pLink);
		if(pUrLink == nullptr || pLink->GetType() != pUrLink->GetType())
			continue;

		vector<GM2DPolyline*> PolyArr = pConn->GetIntersects();
		for(long iPoly = 0 ; iPoly < PolyArr.size() ; iPoly++)
		{
			GM2DPolyline* pPoly = PolyArr[iPoly];
			long nSRVal = pPoly->Contains(&SLine.m_P0);
			long nMRVal = pPoly->Contains(&(SLine.PointAtParam(0.5)));
			long nERVal = pPoly->Contains(&SLine.m_P1);
			if((nSRVal < 1 && nMRVal < 1 && nERVal < 1 ) == false)
			{
				shared_ptr<GM2DCurve> pCenCurve = pLink->GetCenCurve();
				GM2DVector SVec = pCenCurve->PointAtParam(pConn->GetLinkMemberLoc(pLink));

				shared_ptr<GM2DCurve> pUrCenCurve = pUrLink->GetCenCurve();
				GM2DVector TheVec = pUrCenCurve->PointAtParam(pConn->GetLinkMemberLoc(pUrLink));
				GM2DVector EVec = pCenCurve->OrthoBaseVector(TheVec);

				CheckFlag.SetCheckedSecoundOption(CMO_MODEL, CMO_MODEL_HOR_ZLEVELOVERLAP, true);
				return true;
			}

			nSRVal = pPoly->Contains(&ELine.m_P0);
			nMRVal = pPoly->Contains(&(ELine.PointAtParam(0.5)));
			nERVal = pPoly->Contains(&ELine.m_P1);
			if((nSRVal < 1 && nMRVal < 1 && nERVal < 1 ) == false)
			{
				shared_ptr<GM2DCurve> pCenCurve = pLink->GetCenCurve();
				GM2DVector SVec = pCenCurve->PointAtParam(pConn->GetLinkMemberLoc(pLink));
				
				shared_ptr<GM2DCurve> pUrCenCurve = pUrLink->GetCenCurve();
				GM2DVector TheVec = pUrCenCurve->PointAtParam(pConn->GetLinkMemberLoc(pUrLink));
				GM2DVector EVec = pCenCurve->OrthoBaseVector(TheVec);

				CheckFlag.SetCheckedSecoundOption(CMO_MODEL, CMO_MODEL_HOR_ZLEVELOVERLAP, true);
				return true;
			}
		}
	}

	return false;
}

bool MSModelChecker::CheckModel_VerMember_Contain(MSCompoG* pCompoG, CheckModelOpt &CheckFlag)
{
	if (!MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_MODEL))
		return true;
	if (!MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_CONTAIN))
		return true;

	GM2DPolyline* pPoly = pCompoG->GetTrimmedBoundary();
	long NumConn = pCompoG->ma_Connections.size();
	for (long iConn = 0; iConn < NumConn; iConn++)
	{
		MSConnection* pConn = pCompoG->ma_Connections[iConn];
		MSCompoG* pUrCompoG = pConn->GetPartner(pCompoG);
		if(pUrCompoG == nullptr) continue;
		GM2DPolyline* pUrPoly = pUrCompoG->GetTrimmedBoundary();

		if (pUrCompoG->IsVerticalMember() == FALSE)
			continue;

		if (pUrPoly->Contains(pPoly) && pCompoG->IsLevelOverlap(pUrCompoG))
		{
			CheckFlag.SetCheckedSecoundOption(CMO_MODEL, CMO_MODEL_VER_CONTAIN, true);
			return true;
		}
	}

	return false;
}

bool MSModelChecker::CheckModel_SameMember_OverLap(MSCompoG* pCompoG, CheckModelOpt &CheckFlag)
{
	if (!MSCheckModelOption::GetInstance()->IsCheckedBaseOption(CMO_MODEL))
		return true;
	if (!MSCheckModelOption::GetInstance()->IsCheckedSecoundOption(CMO_MODEL, CMO_MODEL_SAMEMEMBER_OVERLAP))
		return true;

	GM2DPolyline* pPoly = pCompoG->GetTrimmedBoundary();
	long NumConn = pCompoG->ma_Connections.size();
	bool IsLinkMember = pCompoG->IsLinkMember();
	for (long iConn = 0; iConn < NumConn; iConn++)
	{
		MSConnection* pConn = pCompoG->ma_Connections[iConn];
		MSCompoG* pUrCompoG = pConn->GetPartner(pCompoG);
		if(pUrCompoG == nullptr) continue;
		bool IsUrLinkMember = pUrCompoG->IsLinkMember();
		
		if (pCompoG->GetType() != pUrCompoG->GetType())
			continue;

		//bool bMyContain = pCompoG->Contains(pUrCompoG, MSFloor::ms_DefLineHitDistTol, true, false);
		bool bUrContain = pUrCompoG->Contains(pCompoG, MSFloor::ms_DefLineHitDistTol, true, false);

		if (bUrContain && pCompoG->IsLevelOverlap(pUrCompoG))
		{
			CheckFlag.SetCheckedSecoundOption(CMO_MODEL, CMO_MODEL_SAMEMEMBER_OVERLAP, true);
			return true;
		}
	}

	return false;
}

long MSModelChecker::CheckCompoM(MSFloor* pFloor)
{
	int nErrorCnt = 0;
	map<long, MSCompoG*>::iterator it;
	for (it = pFloor->mm_CompoG.begin(); it != pFloor->mm_CompoG.end(); it++)
	{
		MSCompoG* pCompoG = it->second;

		if(pCompoG->CheckCompoM())
			continue;

		MSElement::Type_MSCompoG eGType = pCompoG->GetType();
		MSCompoM::Type_MSCompoM eMType = MSCompoM::msNONE;
		MSCompoM* pCompoM = pCompoG->GetCompoM();
		MSCompoM* pFindCompoM = NULL;

		switch (eGType)
		{
		case MSElement::msNULL:
			break;
		case MSElement::msBuilding:
			break;
		case MSElement::msFloor:
			break;
		case MSElement::msBeamG:
			eMType = MSCompoM::msBeamM;
			pFindCompoM = mp_Build->GetCompoM(pCompoM->GetName(), MSCompoM::msBeamM);
			if(pFindCompoM == NULL)				
				pFindCompoM = mp_Build->GetCompoM(pCompoM->GetName(), MSCompoM::msPcBeamM);
			break;
		case MSElement::msBraceG:
			eMType = MSCompoM::msBraceM;
			pFindCompoM = mp_Build->GetCompoM(pCompoM->GetName(), MSCompoM::msBraceM);
			break;
		case MSElement::msWallG:
			eMType = MSCompoM::msBWallM;
			pFindCompoM = mp_Build->GetCompoM(pCompoM->GetName(), MSCompoM::msBWallM);
			if(pFindCompoM == NULL)
				pFindCompoM = mp_Build->GetCompoM(pCompoM->GetName(), MSCompoM::msRWallM);
			break;
		case MSElement::msColumnG:
			eMType = MSCompoM::msColumnM;
			pFindCompoM = mp_Build->GetCompoM(pCompoM->GetName(), MSCompoM::msColumnM);
			if(pFindCompoM == NULL)
				pFindCompoM = mp_Build->GetCompoM(pCompoM->GetName(), MSCompoM::msButtressM);
			if(pFindCompoM == NULL)
				pFindCompoM = mp_Build->GetCompoM(pCompoM->GetName(), MSCompoM::msPcColumnM);
			break;
		case MSElement::msIsoFootG:
			eMType = MSCompoM::msIsoFootM;
			pFindCompoM = mp_Build->GetCompoM(pCompoM->GetName(), MSCompoM::msIsoFootM);
			break;
		case MSElement::msRampG:
			eMType = MSCompoM::msRampM;
			pFindCompoM = mp_Build->GetCompoM(pCompoM->GetName(), MSCompoM::msRampM);
			break;
		case MSElement::msSlabG:
			eMType = MSCompoM::msSlabM;
			pFindCompoM = mp_Build->GetCompoM(pCompoM->GetName(), MSCompoM::msSlabM);
			break;
		case MSElement::msStairG:
			eMType = MSCompoM::msStairLandingM;
			pFindCompoM = mp_Build->GetCompoM(pCompoM->GetName(), MSCompoM::msStairLandingM);
			if(pFindCompoM == NULL)
				pFindCompoM = mp_Build->GetCompoM(pCompoM->GetName(), MSCompoM::msStairStepM);
			break;
		case MSElement::msPitG:
			eMType = MSCompoM::msPitM;
			pFindCompoM = mp_Build->GetCompoM(pCompoM->GetName(), MSCompoM::msPitM);
			break;
		case MSElement::msWallFootG:
			eMType = MSCompoM::msWallFootM;
			pFindCompoM = mp_Build->GetCompoM(pCompoM->GetName(), MSCompoM::msWallFootM);
			break;
		case MSElement::msProject:
			break;
		case MSElement::msSector:
			break;
		case MSElement::msBeamLine:
			break;
		case MSElement::msSlabLineG:
			break;
		case MSElement::msLineLevelDifG:
			break;
		case MSElement::msPointLevelDifG:
			break;
		case MSElement::msHandRailG:
			eMType = MSCompoM::msHandRailM;
			pFindCompoM = mp_Build->GetCompoM(pCompoM->GetName(), MSCompoM::msHandRailM);
			break;
		case MSElement::msWallJointG:
			break;
		case MSElement::msLevelZone:
			break;
		case MSElement::msOpening:
			break;
		case MSElement::msWindowG:
			break;
		case MSElement::msDoorG:
			break;
		case MSElement::msStairPart:
			break;
		case MSElement::msSlabRebar:
			break;
		case MSElement::msWell:
			break;
		case MSElement::msTrench:
			break;
		default:
			break;
		}

		if(pFindCompoM == NULL)
		{
			pFindCompoM = mp_Build->CreateNewCompoM(eMType);
			pFindCompoM->m_Name = LocalDic::GetLocal(L"NONAME", L"StructureModel");
		}

		if(pFindCompoM)
		{
			pCompoG->mp_CompoM = pFindCompoM;

			CString strGroup = LocalDic::GetLocal(L"CompoM 오류", L"StructureModel");
			CString strErrMsg;
			strErrMsg.Format(LocalDic::GetLocal(L"부재의 CompoM정보가 불일치 합니다.(ID: %%d)", L"StructureModel"));
			MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 1, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_COMPOM, pCompoG);
			nErrorCnt++;
		}

		if(!pCompoG->CheckCompoM())
			ASSERT(0);
	}

	return nErrorCnt;
}

void MSModelChecker::SetCheckModelOptFlag(double dVal, CheckModelOpt &CheckFlag, eCheckModel eBaseOpt, long eSecondOpt)
{
	MSCheckModelItem* pItem = MSCheckModelOption::GetInstance()->GetOptItem(eBaseOpt, eSecondOpt);
	if(pItem == NULL)
		return;

	int nCheckVal = pItem->m_nCheckVal;
	bool bCheckDownVal = pItem->m_bCheckDownVal;

	if(bCheckDownVal)
	{
		if(eBaseOpt == CMO_VER_SUPPORT)
		{
			if(DTOL_GM < dVal && nCheckVal >= dVal)
				CheckFlag.SetCheckedSecoundOption(eBaseOpt, eSecondOpt, true);
		}
		else
		{
			if(DTOL_GM < dVal && nCheckVal >= dVal)
				CheckFlag.SetCheckedSecoundOption(eBaseOpt, eSecondOpt, true);
		}
	}
	else
	{
		if(eBaseOpt == CMO_VER_SUPPORT)
		{
			if(nCheckVal < dVal && 100 -DTOL_GM > dVal)
				CheckFlag.SetCheckedSecoundOption(eBaseOpt, eSecondOpt, true);
		}
		else
		{
			if(DTOL_GM < dVal && nCheckVal < dVal)
				CheckFlag.SetCheckedSecoundOption(eBaseOpt, eSecondOpt, true);
		}
	}
}

//--------------------------------------------- CheckModel End--------------------------------------------------------


// ------------------------------------ CheckOverLap Start -----------------------------------------------------------

bool MSModelChecker::IsInsertOverLapCompoGsMsgBox(map<MSCompoG*, MSObject*>& OverLapCompoGMap, CString strMsg)
{
	if (OverLapCompoGMap.size() == 0)
		return false;

	map<MSCompoG*, MSObject*>::iterator it;
	bool IsInsertOverLapCompoGs = false;
	bool HasCurFloor = false;
	bool HasLowerFloor = false;
	bool HasUpperFloor = false;
	
	int nOverlapCnt = 0;
	CString strName;
	map<CString, CString> strMap;
	for (it = OverLapCompoGMap.begin(); it != OverLapCompoGMap.end(); it++)
	{
		if(it->first == it->second)
			continue;
		MSCompoG* pCompoG = it->first;
		CString Name;
		if (pCompoG->mp_CompoM != NULL)
		Name = pCompoG->mp_CompoM->GetName();
		if (Name.GetLength() == 0 ||
			strMap.find(Name) != strMap.end())
			continue;

		if (it == OverLapCompoGMap.begin())
			strName = Name;
		else
			strName += _T(", ") + Name;

		strMap.insert(make_pair(Name, Name));

		MSCompoG* pUrCompoG = dynamic_cast<MSCompoG*>(it->second);
		if (pUrCompoG != NULL)
		{
			MSFloor* pFloor = pCompoG->GetFloor();
			MSFloor* pUrFloor = pUrCompoG->GetFloor();
			if (pFloor != NULL && pUrFloor != NULL)
			{
				if (pFloor->GetFloorNum() == pUrFloor->GetFloorNum())
					HasCurFloor = true;
				else if (pFloor->GetFloorNum() < pUrFloor->GetFloorNum())
					HasUpperFloor = true;
				else if (pFloor->GetFloorNum() > pUrFloor->GetFloorNum())
					HasLowerFloor = true;
			}
		}
		nOverlapCnt++;
	}
	
	if (nOverlapCnt == 0 && OverLapCompoGMap.size() > 1)
		return false;

	// 에러메시지 삭제
	XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();
	for (it = OverLapCompoGMap.begin(); it != OverLapCompoGMap.end(); it++)
	{
		if (it->first == it->second)
			continue;
		MSCompoG* pCompoG = dynamic_cast<MSCompoG*>(it->second);
		if (pCompoG == nullptr) continue;
		pMessageContainer->RemoveErrorObjectsByCompoGID(pCompoG->m_ID, MESSAGE_ERROR_OVERLAP);
	}

	CString strFloorPreFix;
	if (HasCurFloor) strFloorPreFix = LocalDic::GetLocal(L"현재층", L"StructureModel");
	if (HasUpperFloor)
	{
		if (strFloorPreFix.GetLength() > 0)	strFloorPreFix += _T(" ,");
		strFloorPreFix = LocalDic::GetLocal(L"상부", L"StructureModel");
	}
	if (HasLowerFloor)
	{
		if (strFloorPreFix.GetLength() > 0)
			strFloorPreFix += _T(" ,");
		strFloorPreFix = LocalDic::GetLocal(L"하부", L"StructureModel");
	}
	

	CString strMsgBox;
	if (strMsg.GetLength() == 0)
		strMsgBox.Format(LocalDic::GetLocal(L"부재 중첩 알림 메세지"), strName, strFloorPreFix);
	else
		strMsgBox.Format(strMsg, strName);
	int nRVal = MessageBox(NULL, strMsgBox, _T("ModelCheck"), MB_YESNO | MB_DEFBUTTON1 | MB_ICONWARNING | MB_TOPMOST);
	if (nRVal == IDYES)
		return true;

	return false;
}

map<MSCompoG*, MSObject*> MSModelChecker::CheckCompoGArrOverlap_WithUpDownFloor(MSFloor* pFloor, vector<MSCompoG*>& CompoGArr, bool bMakeErrorObject, bool isCheckZLevel)
{
	// pFloor에 있는 부재들과 체크한다.

	long NumCompoG = CompoGArr.size();

	map<MSCompoG*, MSObject*> rMap;
	
	int nlast = 0;
	for (long iCompoG = 0; iCompoG < NumCompoG; iCompoG++)
	{
		MSCompoG* pCompoG = CompoGArr[iCompoG];
		MSContainer* pOrgOwner = pCompoG->GetOwner();
		pCompoG->SetOwner(pFloor);
		MSObject* pOverLatObj = CheckCompoGOverlap_WithUpDownFloor(pFloor, pCompoG, 0, bMakeErrorObject, isCheckZLevel);
		if (pOverLatObj != NULL && pOverLatObj != pCompoG)
			rMap.insert(make_pair(pCompoG, pOverLatObj));

		pCompoG->SetOwner(pOrgOwner);
	}



	return rMap;
}

map<MSCompoG*, MSObject*> MSModelChecker::CheckCompoGArrOverlap_WithCurFloor(vector<MSCompoG*>& CompoGArr, bool bMakeErrorObject, bool isCheckZLevel)
{
	//CompoGArr MSCompoG의 Floor가 셋팅 되어있어야 사용가능하다.
	// Floor가 없으면 체크하지 않는다.

	map<MSFloor*, vector<MSCompoG*>*> FloorMap;
	map<MSFloor*, vector<MSCompoG*>*>::iterator itFloor;

	for (long iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
	{
		MSCompoG* pCompoG = CompoGArr[iCompoG];
		if(pCompoG == nullptr)
			continue;
		MSFloor* pFloor = pCompoG->GetFloor();
		if (pFloor == NULL)
			continue;

		itFloor = FloorMap.find(pFloor);
		if (itFloor == FloorMap.end())
		{
			vector<MSCompoG*>* pCompoGArr = new vector<MSCompoG*>();
			FloorMap.insert(make_pair(pFloor, pCompoGArr));
			pCompoGArr->push_back(pCompoG);
		}
		else
			itFloor->second->push_back(pCompoG);
	}

	map<MSCompoG*, MSObject*> rMap;
	for (itFloor = FloorMap.begin(); itFloor != FloorMap.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->first;
		vector<MSCompoG*>* pCompoGArr = itFloor->second;
		
		int nlast = 0;
		for (long iCompoG = 0; iCompoG < pCompoGArr->size(); iCompoG++)
		{
			MSCompoG* pCompoG = pCompoGArr->at(iCompoG);
			MSContainer* pOrgOwner = pCompoG->GetOwner();
			pCompoG->SetOwner(pFloor);
			MSObject* pOverLatObj = CheckCompoGOverlap_WithUpDownFloor(pFloor, pCompoG, 0, bMakeErrorObject, isCheckZLevel);
			if (pOverLatObj != NULL && pOverLatObj != pCompoG)
				rMap.insert(make_pair(pCompoG, pOverLatObj));

			pCompoG->SetOwner(pOrgOwner);
		}

	}
	
	for (itFloor = FloorMap.begin(); itFloor != FloorMap.end(); itFloor++)
	{
		vector<MSCompoG*>* pCompoGArr = itFloor->second;
		delete pCompoGArr;
	}

	return rMap;
}

map<MSCompoG*, MSObject*> MSModelChecker::CheckCompoGArrOverlap(vector<MSCompoG*>& CompoGArr)
{
	map<MSCompoG*, MSObject*> rMap;
	for (int iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
	{
		MSCompoG* pICompoG = CompoGArr[iCompoG];

		if(rMap.find(pICompoG) != rMap.end())
			continue;

		for (int jCompoG = iCompoG + 1; jCompoG < CompoGArr.size(); jCompoG++)
		{
			MSCompoG* pJCompoG = CompoGArr[jCompoG];

			bool bOverLap = pICompoG->Contains(pJCompoG, DTOL_GM, true, false);
			if (bOverLap)
			{
				rMap.insert(make_pair(pICompoG, pJCompoG));
				rMap.insert(make_pair(pJCompoG, pICompoG));
				continue;
			}

			bOverLap = pJCompoG->Contains(pICompoG, DTOL_GM, true, false);
			if (bOverLap)
			{
				rMap.insert(make_pair(pICompoG, pJCompoG));
				rMap.insert(make_pair(pJCompoG, pICompoG));
				continue;;
			}
		}
	}
	return rMap;
}

bool MSModelChecker::HasOverLapCompoG(MSCompoG* pCompoG, long nOldCompoGID)
{
	bool rVal = false;
	MSFloor* pFloor = pCompoG->GetFloor();
	MSObject* pOverLapObj = pFloor->CheckCompoGOverlap_WithUpDownFloor(pCompoG, nOldCompoGID);
	if (pOverLapObj != NULL && pOverLapObj != pCompoG)
	{
		map<MSCompoG*, MSObject*> OverLapMap;
		OverLapMap.insert(make_pair(pCompoG, pOverLapObj));
		if (MSModelChecker::IsInsertOverLapCompoGsMsgBox(OverLapMap) == false)
			rVal = true;
	}
	if (rVal == false && pCompoG->GetType() == MSElement::msSlabG)
	{
		MSSlabG* pSlabG = (MSSlabG*)pCompoG;
		MSSlabG* pOldSlabG = (MSSlabG*)pFloor->GetCompoG(nOldCompoGID);
		if (pOldSlabG != nullptr)
		{
			if (pSlabG->m_HasOverLapSlopeSlab && pSlabG->GetProfile()->SamePolyLine(pOldSlabG->GetProfile()) == false)
			{
				map<MSCompoG*, MSObject*> OverLapMap;
				OverLapMap.insert(make_pair(pSlabG, pSlabG));
				if (MSModelChecker::IsInsertOverLapCompoGsMsgBox(OverLapMap, LocalDic::GetLocal(L"현재 슬래브, 오버랩되는 슬래브의 경사 정보는 삭제 됩니다.", L"StructureModel")) == false)
					rVal = true;
			}
		}
	}

	return rVal;
}

MSObject* MSModelChecker::CheckCompoGOverlap_WithUpDownFloor(MSFloor* pFloor, MSCompoG* pUrCompoG, long nOldCompoGID /*= 0*/, bool bMakeErrorObject /*= true*/, bool isCheckZLevel /*= true*/)
{
	if (pUrCompoG->GetIsTempCompoG() == true)
		return nullptr;

	XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();
	long OldErrorNum = pMessageContainer->GetNumErrors();

	MSObject* ret = CheckCompoGOverlap(pFloor, pUrCompoG, nOldCompoGID, bMakeErrorObject, isCheckZLevel);
	if (ret != NULL)
	{
		if(bMakeErrorObject == true)
		{
			long NewErrorNum = pMessageContainer->GetNumErrors();
			if(NewErrorNum > OldErrorNum)
				return ret;
		}
		else
			return ret;
	}

	bool bNeedCheckUpper = false;
	bool bNeedCheckLower = false;
	if (pUrCompoG != NULL && pUrCompoG->IsVerticalMember())
		bNeedCheckUpper = bNeedCheckLower = true;

	if (bNeedCheckUpper && pFloor->GetUpperFloor() != NULL)
	{
		ret = CheckCompoGOverlap(pFloor->GetUpperFloor(), pUrCompoG, nOldCompoGID, bMakeErrorObject, true, false);
		if(ret != NULL)
		{
			if(bMakeErrorObject == true)
			{
				long NewErrorNum = pMessageContainer->GetNumErrors();
				if(NewErrorNum > OldErrorNum)
					return ret;
			}
			else
				return ret;
		}
	}

	if (bNeedCheckLower && pFloor->GetLowerFloor() != NULL)
	{
		ret = CheckCompoGOverlap(pFloor->GetLowerFloor(), pUrCompoG, nOldCompoGID, bMakeErrorObject, true, false);
		if(ret != NULL)
		{
			if(bMakeErrorObject == true)
			{
				long NewErrorNum = pMessageContainer->GetNumErrors();
				if(NewErrorNum > OldErrorNum)
					return ret;
			}
			else
				return ret;
		}
	}
	return NULL;
}

MSObject* MSModelChecker::CheckCompoGOverlap(MSFloor* pFloor, MSCompoG* pUrOrgCompoG, long nOldCompoGID /*= 0*/, bool bMakeErrorObject /*= true*/, bool isCheckZLevel /*= true*/, bool bCheckJoint /*= true*/)
{
	shared_ptr<MSCompoG> pUrCompoG_shared = shared_ptr<MSCompoG>((MSCompoG*)pUrOrgCompoG->Copy(false));
	MSCompoG* pUrCompoG = &*pUrCompoG_shared;
	pUrCompoG->m_ID = pUrOrgCompoG->m_ID;

	if (pUrCompoG->IsLinkMember())
	{
		MSLineMemberG* pLineMemberG = (MSLineMemberG*)pUrCompoG;
		pLineMemberG->DeleteMemberLevelDifG();
		if (pUrCompoG->GetType() == MSElement::msWallG)
		{
			MSWallMemberG* pWall = (MSWallMemberG*)pLineMemberG;
			pWall->DeleteLowMemberLevelDifG();
		}

		pLineMemberG->SetLines();
		MSFloor* pMyFloor = pUrCompoG->GetFloor();
		if (pMyFloor != NULL)
			pMyFloor->InsertLevelDif2ALinkMemberByLevelZone((MSLinkMemberG*)pUrCompoG);
	}
	else if (pUrCompoG->GetType() == MSElement::msSlabG)
	{
		MSSlabG* pSlab = (MSSlabG*)pUrCompoG;
		pSlab->m_HasOverLapSlopeSlab = false;
		pSlab->DeleteSlabLineLevelDifAll();
		pSlab->ResetSlabLevelZone();

		MSFloor* pMyFloor = pUrCompoG->GetFloor();
		if (pMyFloor != NULL)
			pMyFloor->InsertLevelDif2SlabByLevelZone((MSSlabG*)pUrCompoG, true);
	}

	CTypedPtrArray<CObArray, MSFloorQT*> CrossQTArr;
	bool bResetFirst = true;

	CString strGroup = LocalDic::GetLocal(L"부재 중첩", L"StructureModel");
	CString strErrMsg;
	CString strErrMsg2;
	MSFloor* pUrFloor = pUrOrgCompoG->GetFloor();

	if (nOldCompoGID == 0)
	{
		if (pFloor->GetFloorNum() == pUrFloor->GetFloorNum())
			strErrMsg.Format(LocalDic::GetLocal(L"입력된 부재와 부재(%%d)가 중첩 되었습니다.", L"StructureModel"));
		else if (pFloor->GetFloorNum() > pUrFloor->GetFloorNum())
			strErrMsg.Format(LocalDic::GetLocal(L"입력된 부재와 상부층 부재(%%d)가 중첩 되었습니다.", L"StructureModel"));
		else if (pFloor->GetFloorNum() < pUrFloor->GetFloorNum())
			strErrMsg.Format(LocalDic::GetLocal(L"입력된 부재와 하부층 부재(%%d)가 중첩 되었습니다.", L"StructureModel"));
		strErrMsg2.Format(LocalDic::GetLocal(L"입력된 부재의 길이가 HitDist옵션에 비해 작습니다.", L"StructureModel"));
	}
	else
	{
		strErrMsg.Format(LocalDic::GetLocal(L"부재(%d)와 부재(%%d)가 중첩 되었습니다.", L"StructureModel"), nOldCompoGID);
		strErrMsg2.Format(LocalDic::GetLocal(L"부재(%d)의 길이가 HitDist옵션에 비해 작습니다.", L"StructureModel"), nOldCompoGID);
	}

	CString strGroup2 = LocalDic::GetLocal(L"부재", L"StructureModel");
	
	
	CString strErrMsg2_2;
	strErrMsg2_2.Format(LocalDic::GetLocal(L"시작점과 끝점의 길이가 HitDist옵션에 비해 작습니다.", L"StructureModel"));

	CString strGroup3 = LocalDic::GetLocal(L"모델링 오류", L"StructureModel");
	CString strErrMsg3;
	strErrMsg3.Format(LocalDic::GetLocal(L"폴리라인 입력이 잘못 되었습니다.", L"StructureModel"));

	if (pUrCompoG->GetType() == MSElement::msOpeningG)
	{
		MSOpeningG* pUrOpeningG = (MSOpeningG*)pUrCompoG;
		MSCompoG* pCompoG = (MSCompoG*)CheckSlabOpeningGOverlap(pFloor, pUrOpeningG, nOldCompoGID);
		if (pCompoG == pUrCompoG)
		{
			if (bMakeErrorObject)
				MSModelChecker::MakeErrorObject(strGroup3, strErrMsg3, 1, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_SLAB_PROFILE, pCompoG);

			return pCompoG;
		}
		else if (pCompoG != NULL)
		{
			if (bMakeErrorObject)
				MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 1, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_SLAB_PROFILE, pCompoG);

			return pCompoG;
		}

		return pCompoG;
	}
	if (pUrCompoG->GetType() == MSElement::msSlabG)
	{
		MSSlabG* pUrSlabG = (MSSlabG*)pUrCompoG;
		MSCompoG* pCompoG = (MSCompoG*)CheckSlabGOverlap(pFloor, pUrSlabG, nOldCompoGID);
		((MSSlabG*)pUrOrgCompoG)->m_HasOverLapSlopeSlab = pUrSlabG->m_HasOverLapSlopeSlab;
		if (pCompoG == pUrCompoG)
		{
			if (bMakeErrorObject)
				MSModelChecker::MakeErrorObject(strGroup3, strErrMsg3, 1, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_SLAB_PROFILE, pCompoG);

			return pUrOrgCompoG;
		}
		else if (pCompoG != NULL)
		{
			if (bMakeErrorObject)
				MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 1, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_SLAB_PROFILE, pCompoG);

			return pUrOrgCompoG;
		}

		GM2DPolyline* pUrPoly = pUrSlabG->GetProfile();
		vector<MSCompoG*> SlabArr = MSCompoGUtil::FindCompoG(pFloor, pUrPoly, FindType_MSElement::msSlabG, true, false);

		for (long iSlab = 0; iSlab < SlabArr.size(); iSlab++)
		{
			MSSlabG* pCurCompoG = (MSSlabG*)SlabArr[iSlab];
			if (pUrCompoG->m_ID == pCurCompoG->m_ID)
				continue;

			GM2DPolyline* pCurPoly = pCurCompoG->GetProfile();

			if (pCurPoly->OverlapEx(*pUrPoly))
				continue;

			int Idx = pUrPoly->CheckPolylineSegmentDist(pCurPoly, MSFloor::ms_DefLineHitDistTol);
			if (Idx != -1)
			{
				strErrMsg3.Format(LocalDic::GetLocal(L"%s %s(%ld)와 %s %s(%ld) 사이에 좁은 간격이 존재 합니다.", L"StructureModel"), pCurCompoG->GetCompoM()->m_Name, pCurCompoG->GetGroupName(), pCurCompoG->m_ID, pUrSlabG->GetCompoM()->m_Name, pUrSlabG->GetGroupName(), pUrSlabG->ms_LastID + 1);
				if (bMakeErrorObject)
					MSModelChecker::MakeErrorObject(strGroup3, strErrMsg3, 1, MESSAGE_LEVEL_WARNING, MESSAGE_WARNING_POLYLINESEFMENTDIST, pCurCompoG);
				return pCurCompoG;
			}
		}

		long NumLevelZone = pFloor->ma_LevelZone.size();
		for (long iZone = 0; iZone < NumLevelZone; iZone++)
		{
			MSLevelZone* pCurLevelZone = pFloor->ma_LevelZone[iZone];
			if (pCurLevelZone == NULL)
				continue;

			GM2DPolyline* pCurPoly = pCurLevelZone->GetProfile();

			int Idx = pCurPoly->CheckPolylineSegmentDist(pUrSlabG->GetProfile(), MSFloor::ms_DefLineHitDistTol);
			if (Idx != -1)
			{
				strErrMsg3.Format(LocalDic::GetLocal(L"%s : %s(%ld)와 %s %s(%ld)사이에 좁은 간격이 존재 합니다.", L"StructureModel"), pCurLevelZone->GetUserFloorName(), pCurLevelZone->GetGroupName(), pCurLevelZone->m_ID, pUrSlabG->GetCompoM()->m_Name, pUrSlabG->GetGroupName(), pUrSlabG->ms_LastID + 1);
				if (bMakeErrorObject)
					MSModelChecker::MakeErrorObject(strGroup3, strErrMsg3, 0, MESSAGE_LEVEL_WARNING, MESSAGE_WARNING_POLYLINESEFMENTDIST, pCurLevelZone);

				return pCurLevelZone;
			}
		}
		return NULL;
	}

	if (pUrCompoG->GetType() == MSElement::msPitG)
	{
		GM2DPolyline* pUrPoly = pUrCompoG->GetProfile();

		// 대강 찾고 찾은 후보군에서 새밀하게 검토
		vector<MSCompoG*> CompoGArr = MSCompoGUtil::FindCompoG(pFloor, pUrPoly, pUrCompoG->GetFindType(), false, false);

		for (long iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
		{
			MSCompoG* pCurCompoG = CompoGArr[iCompoG];
			if (pCurCompoG->m_ID == nOldCompoGID)
				continue;

			if (pUrCompoG->IsLevelOverlap(pCurCompoG) == false)
				continue;

			GM2DPolyline* pCurPoly = pCurCompoG->GetProfile();
			if (pUrPoly->OverlapEx(*pCurPoly, DTOL_GM, FALSE))
			{
				MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 1, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_SLAB_PROFILE, pCurCompoG);
				return pCurCompoG;
			}
		}
	}

	if (pUrCompoG->GetType() == MSElement::msStairG)
	{
		MSGenStairG* pStair = (MSGenStairG*)pUrCompoG;

		// 사각 계단만 층고 계산함
// 		if (pFloor == pStair->GetFloor())
// 		{
// 			double dHeight = pStair->GetDepthWithOutLowPart();
// 
// 			double dFloorHeight = pStair->GetFloor()->GetSlabHeight();
// 
// 			if (dHeight > dFloorHeight + DTOL_GM)
// 			{
// 				MSModelChecker::MakeErrorObject(strGroup, _T("부재의 높이가 층고 보다 높게 입력 되었습니다."), 1, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_COMPOG_DEPTH_OVER, pStair);
// 				if (pStair->m_IsRect == TRUE)
// 					return NULL;
// 				else
// 					return NULL;
// 			}
// 			else if (dHeight < dFloorHeight - DTOL_GM)
// 			{
// 				MSModelChecker::MakeErrorObject(strGroup, _T("부재의 높이가 층고 보다 낮게 입력 되었습니다."), 1, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_COMPOG_DEPTH_OVER, pStair);
// 				if (pStair->m_IsRect == TRUE)
// 					return NULL;
// 				else
// 					return NULL;
// 			}
// 		}

		GM2DBound UrBound;
		GM2DPolyline UrPoly(TRUE);
		vector<MSStairPart*> UrPartArr;

		for (long iStep = 0; iStep < pStair->ma_Step.GetSize(); iStep++)
		{
			UrBound.Add2DPolyline(pStair->ma_Step[iStep]->m_pPolyline);
			UrPartArr.push_back(pStair->ma_Step[iStep]);
		}

		for (long iLanding = 0; iLanding < pStair->ma_Landing.GetSize(); iLanding++)
		{
			UrBound.Add2DPolyline(pStair->ma_Landing[iLanding]->m_pPolyline);
			UrPartArr.push_back(pStair->ma_Landing[iLanding]);
		}

		UrPoly.AddPointAndBulge(new GM2DVector(*UrBound.GetTopLeft()), 0);
		UrPoly.AddPointAndBulge(new GM2DVector(*UrBound.GetTopRight()), 0);
		UrPoly.AddPointAndBulge(new GM2DVector(*UrBound.GetBottomRight()), 0);
		UrPoly.AddPointAndBulge(new GM2DVector(*UrBound.GetBottomLeft()), 0);
		UrPoly.AddPointAndBulge(new GM2DVector(*UrBound.GetTopLeft()), 0);

		// 대강 찾고 찾은 후보군에서 새밀하게 검토
		vector<MSCompoG*> CompoGArr = MSCompoGUtil::FindCompoG(pFloor, &UrPoly, pUrCompoG->GetFindType(), false, false);

		for (long iCompoG = 0; iCompoG < CompoGArr.size(); iCompoG++)
		{
			MSGenStairG* pCurStair = (MSGenStairG*)CompoGArr[iCompoG];
			if (pCurStair->m_ID == nOldCompoGID)
				continue;

			if (pUrCompoG->IsLevelOverlap(pCurStair) == false)
				continue;

			vector<MSStairPart*> PartArr;
			for (long iLanding = 0; iLanding < pCurStair->ma_Landing.GetSize(); iLanding++)
			{
				MSStairPart* pPart = pCurStair->ma_Landing[iLanding];
				PartArr.push_back(pPart);
			}
			for (long iStep = 0; iStep < pCurStair->ma_Step.GetSize(); iStep++)
			{
				MSStairPart* pPart = pCurStair->ma_Step[iStep];
				PartArr.push_back(pPart);
			}

			for (long iUrPart = 0; iUrPart < UrPartArr.size(); iUrPart++)
			{
				MSStairPart* pUrPart = UrPartArr[iUrPart];

				for (long iPart = 0; iPart < PartArr.size(); iPart++)
				{
					MSStairPart* pPart = PartArr[iPart];
					if (pUrPart->IsLevelOverlap(pPart) && 
						pUrPart->m_pPolyline->OverlapEx(*pPart->m_pPolyline, DTOL_GM, FALSE))
					{
						MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 1, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_SLAB_PROFILE, pCurStair);
						return pCurStair;
					}
				}
			}
		}

	}


	if (pUrCompoG->IsLinkMember())
	{
		MSLineMemberG* pLineMemberG = (MSLineMemberG*)pUrCompoG;
		double dLen = pLineMemberG->GetLength();
		if (dLen < MSFloor::ms_DefLineHitDistTol)
		{
			if (bMakeErrorObject) MSModelChecker::MakeErrorObject(strGroup2, strErrMsg2, 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pUrCompoG);
			return pUrCompoG;
		}
		else if(pLineMemberG->GetSVec()->Distance(*pLineMemberG->GetEVec()) < MSFloor::ms_DefLineHitDistTol )
		{
			if (bMakeErrorObject) MSModelChecker::MakeErrorObject(strGroup2, strErrMsg2_2, 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pUrCompoG);
			return pUrCompoG;
		}

		// Modify할경우에도 SetLine 처리 해야함
		//pLineMemberG->SetLines();

		shared_ptr<GM2DCurve> pCurve = pLineMemberG->GetCenCurve();
		pFloor->mp_QT->FindCrossQTs(&*pCurve, CrossQTArr, bResetFirst);

		if (pLineMemberG->m_isBuriedLinkMember)
			bCheckJoint = false;
	}
	else if (pUrCompoG->IsPointMember())
	{
		MSPointMemberG* pPointMemberG = (MSPointMemberG*)pUrCompoG;
		pFloor->mp_QT->FindCrossQTs(pPointMemberG->GetSVec(), CrossQTArr, bResetFirst);
	}
	
	bool IsCheckColumn = false;
	bool IsCheckLinkMember = false;

	vector<MSCompoG*> ColumnArr;
	vector<MSCompoG*> LinkMemberArr;

	if (pUrCompoG->GetType() == MSElement::msColumnG)
		IsCheckColumn = true;
	else if (pUrCompoG->IsLinkMember())
		IsCheckLinkMember = true;

	CMap<long, long, MSCompoG*, MSCompoG*> CheckedMemberMap;  // 하나의 CompoG가 여러 QT에 있을 경우 pCompoG와 한번만 검토되도록 하기 위하여 검토된 pUrCompoG 등록

	long numQT = CrossQTArr.GetSize();
	for (long iQT = 0; iQT < numQT; iQT++)
	{
		MSFloorQT* pQT = CrossQTArr[iQT];
		if (pUrCompoG->IsVerticalMember())
		{
			long NumMem = pQT->ma_VerticalMem.GetSize();
			for (long iMem = 0; iMem < NumMem; iMem++)
			{
				MSCompoG* pCompoG = pQT->ma_VerticalMem[iMem];
				if (pCompoG == NULL || pCompoG->m_ID == nOldCompoGID)
					continue;

				MSCompoG* pTempMem;
				if (CheckedMemberMap.Lookup(pCompoG->m_ID, pTempMem))
					continue;
				CheckedMemberMap.SetAt(pCompoG->m_ID, pCompoG);

				bool IsSameType = false;
				if (pCompoG->GetType() == pUrCompoG->GetType())
					IsSameType = true;

				if (IsSameType || 
					(IsCheckColumn && pCompoG->IsLinkMember()) ||
					(IsCheckLinkMember && pCompoG->GetType() == MSElement::msColumnG))
				{
					bool bMyContain = pCompoG->Contains(pUrCompoG, MSFloor::ms_DefLineHitDistTol, true, isCheckZLevel);
					bool bUrContain = pUrCompoG->Contains(pCompoG, MSFloor::ms_DefLineHitDistTol, true, isCheckZLevel);

					if ((bMyContain || bUrContain) && 
						IsSameType)
					{
						if (bMakeErrorObject) MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 1, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pCompoG);
						return pCompoG;
					}
					if (bUrContain && IsSameType)
					{
						if (bMakeErrorObject) MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 1, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pCompoG);
						return pCompoG;
					}

					if (IsCheckColumn && 
						(pCompoG->IsLinkMember() || pCompoG->GetType() == MSElement::msColumnG) &&
						(bMyContain || bUrContain || pCompoG->Overlap(pUrCompoG) == true) &&
						(pUrCompoG->IsLevelContain(pCompoG) || pCompoG->IsLevelContain(pUrCompoG)))
							LinkMemberArr.push_back(pCompoG);

					if (IsCheckLinkMember && 
						pCompoG->GetType() == MSElement::msColumnG &&
						pCompoG->Overlap(pUrCompoG) == true &&
						(pUrCompoG->IsLevelContain(pCompoG) || pCompoG->IsLevelContain(pUrCompoG)))
							ColumnArr.push_back(pCompoG);
				}
				
			}
		}
		else if (pUrCompoG->IsFloorMember())
		{
			for (long iMem = 0; iMem < pQT->ma_VerticalMem.GetSize(); iMem++)
			{
				MSCompoG* pCompoG = pQT->ma_VerticalMem[iMem];
				if (pCompoG == NULL || pCompoG->m_ID == nOldCompoGID)
					continue; 

				MSCompoG* pTempMem;
				if (CheckedMemberMap.Lookup(pCompoG->m_ID, pTempMem))
					continue;
				CheckedMemberMap.SetAt(pCompoG->m_ID, pCompoG);

				if (IsCheckLinkMember && pCompoG->GetType() == MSElement::msColumnG &&
					pUrCompoG->IsLevelContain(pCompoG) && pCompoG->Overlap(pUrCompoG) == true)
				{
					ColumnArr.push_back(pCompoG);
				}
			}

			long NumMem = pQT->ma_FloorMem.GetSize();
			for (long iMem = 0; iMem < NumMem; iMem++)
			{
				MSCompoG* pCompoG = pQT->ma_FloorMem[iMem];
				if (pCompoG == NULL || pCompoG->m_ID == nOldCompoGID)
					continue;

				MSCompoG* pTempMem;
				if (CheckedMemberMap.Lookup(pCompoG->m_ID, pTempMem))
					continue;
				CheckedMemberMap.SetAt(pCompoG->m_ID, pCompoG);

				bool IsSameType = false;
				if (pCompoG->GetType() == pUrCompoG->GetType())
					IsSameType = true;
				
				if (IsSameType ||
					(IsCheckColumn && pCompoG->IsLinkMember()) ||
					(IsCheckLinkMember && pCompoG->GetType() == MSElement::msColumnG))
				{
					bool bMyContain = pCompoG->Contains(pUrCompoG, MSFloor::ms_DefLineHitDistTol, true, isCheckZLevel);
					bool bUrContain = pUrCompoG->Contains(pCompoG, MSFloor::ms_DefLineHitDistTol, true, isCheckZLevel);

					if (bMyContain && IsSameType)
					{
						if (bMakeErrorObject) MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 1, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pCompoG);
						return pCompoG;
					}
					if (bUrContain && IsSameType)
					{
						if (bMakeErrorObject) MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 1, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pCompoG);
						return pCompoG;
					}

					if (IsCheckColumn && (pCompoG->IsLinkMember() || pCompoG->GetType() == MSElement::msColumnG) &&
						(bMyContain || bUrContain) &&
						(pUrCompoG->IsLevelContain(pCompoG) || pCompoG->IsLevelContain(pUrCompoG)))
						LinkMemberArr.push_back(pCompoG);

					if (IsCheckLinkMember && pCompoG->GetType() == MSElement::msColumnG &&
						pCompoG->Overlap(pUrCompoG) == true &&
						(pUrCompoG->IsLevelContain(pCompoG) || pCompoG->IsLevelContain(pUrCompoG)))
						ColumnArr.push_back(pCompoG);
				}
			}
		}
	}

	if (IsCheckColumn && LinkMemberArr.size() > 0)
	{
		MSColumnG* pColumnG = (MSColumnG*)pUrCompoG;
		
		GM2DPolyline TheColumnPoly;
		TheColumnPoly.CopyFrom(pColumnG->GetTrimmedBoundary());

		vector<GM2DPolyline*> Joined_Polys;
		for (long iLink = 0; iLink < LinkMemberArr.size(); iLink++)
		{
			MSCompoG* pLinkG = LinkMemberArr[iLink];
			GM2DPolyline* pLinkPoly = pLinkG->GetTrimmedBoundary();

			if (pLinkG->GetType() == MSElement::msColumnG)
			{
				TheColumnPoly.BoolOperationNSortByArea('U', pLinkPoly, Joined_Polys, DTOL_GM);

				if (Joined_Polys.size() > 0)
					TheColumnPoly.CopyFrom(Joined_Polys[0]);
				else
					ASSERT(0);

				GMObject::DeleteGMObject(Joined_Polys);
			}
		}

		for (long iLink = 0; iLink < LinkMemberArr.size(); iLink++)
		{
			MSCompoG* pLinkG = LinkMemberArr[iLink];
			if (pLinkG->GetType() == MSElement::msColumnG)
				continue;

			if (TheColumnPoly.Contains(pLinkG->GetTrimmedBoundary(), true, DTOL_MODELING))
			{
				if (bMakeErrorObject) MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 1, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, LinkMemberArr[0]);
				return pLinkG;
			}
		}
	}

	if (IsCheckLinkMember && ColumnArr.size() > 0)
	{
		MSLinkMemberG* pLinkG = (MSLinkMemberG*)pUrCompoG;
		GM2DPolyline ThePoly;
		vector<GM2DPolyline*> Joined_Polys;
		for (long iLink = 0; iLink < ColumnArr.size(); iLink++)
		{
			MSColumnG* pColumnG = (MSColumnG*)ColumnArr[iLink];
			GM2DPolyline* pLinkPoly = pColumnG->GetTrimmedBoundary();
			if (ThePoly.GetNumLines() > 0)
			{
				ThePoly.BoolOperationNSortByArea('U', pLinkPoly, Joined_Polys, DTOL_GM);

				if (Joined_Polys.size() > 0)
					ThePoly.CopyFrom(Joined_Polys[0]);
				else
					ASSERT(0);

				GMObject::DeleteGMObject(Joined_Polys);
			}
			else
				pColumnG->GetTrimmedBoundary()->CopyTo(&ThePoly);
		}

		if (ThePoly.Contains(pLinkG->GetTrimmedBoundary(), true, DTOL_MODELING))
		{
			if (bMakeErrorObject) MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 1, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, ColumnArr[0]);
			return ColumnArr[0];
		}
	}


	return NULL;
}

MSObject* MSModelChecker::CheckSlabGOverlap(MSFloor* pFloor, MSCompoG* pCompoG, long nOldCompoGID)
{
	if(pCompoG->GetType() != MSElement::msSlabG)
		return NULL;

	MSSlabG* pSlabG = (MSSlabG*)pCompoG;
	GM2DPolyline* pPoly = pSlabG->GetProfile();
	pPoly->MakeCCW();

	// 중복점 제거 kkw
	pPoly->CheckVecterArr(true);

	if(!pPoly->CheckPolygon(false))
		return pCompoG;

	vector<MSCompoG*> SlabArr = MSCompoGUtil::FindCompoG(pFloor, pPoly, FindType_MSElement::msSlabG, true, true);

	for(long iSlab = 0 ; iSlab < SlabArr.size() ;iSlab++)
	{
		MSSlabG* pCurSlabG = (MSSlabG*)SlabArr[iSlab];
		if (pCurSlabG->m_ID == nOldCompoGID)
			continue;
		if (pSlabG->m_HasOverLapSlopeSlab == false && pCurSlabG->IsSlopeMember())
		{
			if(pCurSlabG->GetProfile()->OverlapEx(*pPoly, DTOL_MODELING, false))
				pSlabG->m_HasOverLapSlopeSlab = true;
		}
		if(!pSlabG->IsLevelOverlap(pCurSlabG))
			continue;

		GM2DPolyline* pCurPoly = pCurSlabG->GetProfile();
		if(pPoly->SamePolyLine(pCurPoly))
			return pCurSlabG;
	}
	return NULL;
}

MSObject* MSModelChecker::CheckSlabOpeningGOverlap(MSFloor* pFloor, MSCompoG* pCompoG, long nOldCompoGID)
{
	if (pCompoG->GetType() != MSElement::msOpeningG)
		return NULL;

	MSOpeningG* pOpening = (MSOpeningG*)pCompoG;
	GM2DPolyline* pPoly = pOpening->GetProfile();
	pPoly->MakeCCW();

	// 중복점 제거 kkw
	pPoly->CheckVecterArr(true);

	if (!pPoly->CheckPolygon(false))
		return pCompoG;

	vector<MSCompoG*> OpeningArr = MSCompoGUtil::FindCompoG(pFloor, pPoly, FindType_MSElement::msOpeningG, true, false);

	for (long iOpening = 0; iOpening < OpeningArr.size(); iOpening++)
	{
		MSOpeningG* pCurOpeningG = (MSOpeningG*)OpeningArr[iOpening];
		if (pCurOpeningG->m_ID == nOldCompoGID)
			continue;
		if (!pOpening->IsLevelOverlap(pCurOpeningG))
			continue;

		GM2DPolyline* pCurPoly = pCurOpeningG->GetProfile();
		if (pPoly->SamePolyLine(pCurPoly))
			return pCurOpeningG;
	}
	return NULL;
}


void MSModelChecker::MakeErrorObject(CString strGroup, CString strErrMsg, long nElementType, long nErrorLevel, long nErrorCode, MSElement* pElement1, MSElement* pElement2)
{
	if (GetIsMakeErrorObject() == false)
		return;

	CString strMsg;

	XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();

	if(nElementType == 0)
		strMsg.Format(strErrMsg);
	else if(nElementType == 1)
		strMsg.Format(strErrMsg, pElement1->m_ID);
	else if(nElementType == 2)
		strMsg.Format(strErrMsg, pElement1->m_ID, pElement2->m_ID);
	else
		return;

	XErrorObject::SelectType eSelType = XErrorObject::MSElementIDSelect;
	//XErrorObject* pMsgObject = pMessageContainer->CreateErrorObject(MESSAGE_ERROR_OVERLAP, MESSAGE_LEVEL_ERROR, strMsg, eSelType);
	XErrorObject* pMsgObject = pMessageContainer->CreateErrorObject(nErrorCode, nErrorLevel, strMsg, eSelType);

	if (pMsgObject)
	{
		CString newMsg;
		pMsgObject->SetGroupName(strGroup);
		if(nElementType == 0)
		{
			pMsgObject->m_PopupMessage = strErrMsg; // mylee : Message Change
			pMsgObject->m_Message.Add(strErrMsg);
			pMsgObject->AddCompoGID(pElement1->m_ID);
			pMsgObject->SetShowSelectCompoG(TRUE);
		}
		else if(nElementType == 1)
		{
			MSCompoG* pCompoG1 = (MSCompoG*)pElement1;
			MSFloor* pUserFloor = pCompoG1->GetFloorForUser();
// 			int nFloorNum1 = pCompoG1->GetFloorNumForUser();
// 			MSRebarBuilding* pBuild = pCompoG1->GetBuilding();
			newMsg.Format(_T("%s : %s"), pUserFloor->GetName(), strMsg);
			pMsgObject->m_PopupMessage = newMsg; // mylee : Message Change
			pMsgObject->m_Message.RemoveAll();
			pMsgObject->m_Message.Add(newMsg);
			pMsgObject->AddCompoGID(pCompoG1->m_ID);
			pMsgObject->SetShowSelectCompoG(TRUE);
		}
		else if(nElementType == 2)
		{
			MSCompoG* pCompoG1 = (MSCompoG*)pElement1;
			MSCompoG* pCompoG2 = (MSCompoG*)pElement2;
			MSFloor* pUserFloor1 = pCompoG1->GetFloorForUser();
// 			int nFloorNum1 = pCompoG1->GetFloorNumForUser();
// 			int nFloorNum2 = pCompoG2->GetFloorNumForUser();
// 			MSRebarBuilding* pBuild = pCompoG1->GetBuilding();
			newMsg.Format(_T("%s : %s"), pUserFloor1->GetName(), strMsg);
			pMsgObject->m_PopupMessage = newMsg; // mylee : Message Change
			pMsgObject->m_Message.RemoveAll();
			pMsgObject->m_Message.Add(newMsg);
			pMsgObject->AddCompoGID(pCompoG1->m_ID);
			pMsgObject->AddCompoGID(pCompoG2->m_ID);
			pMsgObject->SetShowSelectCompoG(TRUE);
		}

		pMessageContainer->AddErrorObject(pMsgObject);
		
		CString log;
		log.Format(L"ModelChecker: %s", pMsgObject->ToString());
		LoggerWrapper::Log(log);

	}
}

MSElement* MSModelChecker::CheckOverlapLevelZone(MSFloor* pFloor, MSLevelZone* pZone)
{
	if(pZone == NULL)
		return NULL;

	MSElement* ret = NULL;

	XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();
	long OldErrorNum = pMessageContainer->GetNumErrors();

	CString strGroup = LocalDic::GetLocal(L"LevelZone 포함", L"StructureModel");
	CString strErrMsg;
	strErrMsg.Format(LocalDic::GetLocal(L"입력된 LevelZone이 기존 LevelZone에 포함됩니다.", L"StructureModel"));

	CString strGroup2 = LocalDic::GetLocal(L"모델링 오류", L"StructureModel");
	CString strErrMsg2;
	strErrMsg2.Format(LocalDic::GetLocal(L"폴리라인 입력이 잘못 되었습니다.", L"StructureModel"));
	CString strErrMsg3;
	strErrMsg3.Format(LocalDic::GetLocal(L"입력된 LevelZone와 주변 LevelZone 사이에 좁은 간격이 존재 합니다.", L"StructureModel"));
	CString strErrMsg4;
	strErrMsg4.Format(LocalDic::GetLocal(L"입력된 LevelZone와 주변 슬래브 사이에 좁은 간격이 존재 합니다.", L"StructureModel"));


	GM2DPolyline* pPoly = pZone->GetProfile();
	if(pPoly->CheckPolygon(false) == FALSE)
	{
		MSModelChecker::MakeErrorObject(strGroup2, strErrMsg2, 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pZone);
		ret = pZone;
	}

	if(ret == NULL)
	{
		long NumLevelZone = pFloor->ma_LevelZone.size();
		for(long iZone = 0 ; iZone < NumLevelZone ; iZone++)
		{
			MSLevelZone* pCurLevelZone = pFloor->ma_LevelZone[iZone];
			if(pCurLevelZone == NULL) 
				continue;

			GM2DPolyline* pCurPoly = pCurLevelZone->GetProfile();

			bool bRval = pCurPoly->Contains(pPoly);
			if(bRval == true)
			{
				MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pCurLevelZone);
				ret = pCurLevelZone;
				break;
			}

			bRval = pPoly->Contains(pCurPoly);
			if (bRval == true)
			{
				MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pCurLevelZone);
				ret = pCurLevelZone;
				break;
			}

			int Idx = pCurPoly->CheckPolylineSegmentDist(pPoly, MSFloor::ms_DefLineHitDistTol);
			if(Idx != -1)
			{
				MSModelChecker::MakeErrorObject(strGroup, strErrMsg3, 0, MESSAGE_LEVEL_WARNING, MESSAGE_WARNING_POLYLINESEFMENTDIST, pCurLevelZone);
				ret = pCurLevelZone;
				break;
			}

		}

		map<long, MSCompoG*>::iterator itCompoG;
		vector<MSCompoG*> SlabArr = MSCompoGUtil::FindCompoG(pFloor, pPoly, FindType_MSElement::msSlabG, true, true, DefHitDist);
		for(long iSlab = 0 ; iSlab < SlabArr.size() ;iSlab++)
		{
			MSSlabG* pCurSlabG = (MSSlabG*)SlabArr[iSlab];
			GM2DPolyline* pCurPoly = pCurSlabG->GetProfile();
			int Idx = pCurPoly->CheckPolylineSegmentDist(pPoly, MSFloor::ms_DefLineHitDistTol);
			if(Idx != -1)
			{
				MSModelChecker::MakeErrorObject(strGroup, strErrMsg4, 0, MESSAGE_LEVEL_WARNING, MESSAGE_WARNING_POLYLINESEFMENTDIST, pCurSlabG);
				ret = pCurSlabG;
				break;
			}
		}
	}

	long NewErrorNum = pMessageContainer->GetNumErrors();
	if(NewErrorNum > OldErrorNum)
		return ret;

	return NULL;
}

MSConstructionZone* MSModelChecker::CheckOverlapConstructionZone(MSFloor* pFloor, MSConstructionZone* pZone, long nOldCZoneID /*= 0*/)
{
	if(pZone == NULL)
		return NULL;

	CString strGroup = LocalDic::GetLocal(L"ConstructionZone 중첩", L"StructureModel");
	CString strErrMsg;
	strErrMsg.Format(LocalDic::GetLocal(L"입력된 ConstructionZone이 기존 ConstructionZone에 포함됩니다.", L"StructureModel"));

	map<long, MSConstructionZone*>::iterator it;
	for( it = pFloor->mm_CZone.begin(); it != pFloor->mm_CZone.end(); ++it)
	{
		if (nOldCZoneID > 0)
		{
			if (it->second->m_ID == nOldCZoneID) continue;
		}
		MSConstructionZone* pCurZone = it->second;
		if(pCurZone == NULL) continue;
		if (pCurZone->GetZoneType() != pZone->GetZoneType())	continue;
		//if(pCurZone->GetProfile()->OverlapEx(*pZone->GetProfile(), DTOL_GM, FALSE))
		if(pCurZone->GetProfile()->SamePolyLine(pZone->GetProfile()))
		{
			MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pCurZone);
			//return NULL;
			return pCurZone;
		}
	}

	return NULL;
}

MSSlabRebar* MSModelChecker::CheckSlabRebarOverlap(MSFloor* pFloor, MSSlabRebar* pUrSlabRebar, long nOldCompoGID, bool bMakeErrorObject/*=true*/)
{
	CTypedPtrArray<CObArray, MSFloorQT*> CrossQTArr;
	bool bResetFirst = true;

	CString strGroup = LocalDic::GetLocal(L"슬래브주근 중첩", L"StructureModel");
	CString strErrMsg;
	CString strFloorName = _T("");
	MSFloor* pUpperFloor = pFloor->GetUpperFloor();
	if(pUpperFloor != NULL)
		strFloorName = pUpperFloor->GetName();
	strErrMsg.Format(LocalDic::GetLocal(L"%s : 입력된 슬래브주근이 기존 슬래브주근과 중첩됩니다.", L"StructureModel"), strFloorName);

	map<long, MSSlabRebar*>::iterator it;
	for( it = pFloor->mm_Rebar.begin(); it != pFloor->mm_Rebar.end(); ++it)
	{
		MSSlabRebar* pSlabRebar = it->second;
		if(pSlabRebar == NULL) continue;
		if (pSlabRebar->m_ID == nOldCompoGID)
			continue;
		if(pSlabRebar->CheckOverlap(pUrSlabRebar))
		{
			MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pSlabRebar);
			return pSlabRebar;
		}
	}
	return NULL;
}
MSFWShadeG* MSModelChecker::CheckWallShadeOverlap_ADD(MSWallMemberG *pWall, MSFWShadeG *pShade, bool bMakeErrorObject)
{
	if (pWall == NULL || pShade == NULL)
		return NULL;

	CString strGroup = _T(""), strErrMsg = _T("");			
	if (pShade->CheckData(true) == false)
	{
		strGroup = LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strErrMsg = _T("");			
		strErrMsg.Format(LocalDic::GetLocal(L"Wall(%%d) 밖에 차양이 존재 합니다.", L"StructureModel"));
		MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 1, MESSAGE_LEVEL_WARNING, MESSAGE_WARNING_WALLOPENINGLOC, pWall);
		return pShade;
	}

	strGroup = LocalDic::GetLocal(L"벽체 차양 중첩", L"StructureModel");	
	MSFloor* pFloor = pWall->GetFloor();
	strErrMsg.Format(LocalDic::GetLocal(L"%s : 입력된 벽체 차양이 기존 차양과 중첩됩니다.", L"StructureModel"), pFloor->GetName());

	for(auto itShade = pWall->mm_ShadeG.begin() ; itShade != pWall->mm_ShadeG.end() ; itShade++)
	{
		MSFWShadeG *pCompareShade = itShade->second;
		if(pCompareShade->Overlap(pShade) || pShade->Overlap(pCompareShade))
		{
			MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pCompareShade);
			return pCompareShade;
		}
	}
	return NULL;
}
MSFWShadeG* MSModelChecker::CheckWallShadeOverlap_MOD(MSWallMemberG *pWall, bool bMakeErrorObject)
{
	if (pWall == NULL)
		return NULL;
	
	CString strGroup = LocalDic::GetLocal(L"벽체 오프닝 중첩", L"StructureModel");
	CString strErrMsgOverlapShade;
	MSFloor* pFloor = pWall->GetFloor();
	strErrMsgOverlapShade.Format(LocalDic::GetLocal(L"%s : 수정된 차양이 기존 차양과 중첩됩니다.", L"StructureModel"), pFloor->GetName());

	for (auto itShade = pWall->mm_ShadeG.begin(); itShade != pWall->mm_ShadeG.end(); itShade++)
	{
		MSFWShadeG* pCheckShade = itShade->second;

		if(pCheckShade->CheckData() == false)
			return pCheckShade;
		
		for (auto itShade2 = pWall->mm_ShadeG.begin(); itShade2 != pWall->mm_ShadeG.end(); itShade2++)
		{
			MSFWShadeG* pShade = itShade2->second;
			if (pCheckShade == pShade)
				continue;
			
			if (pShade->Overlap(pCheckShade))
			{
				MSModelChecker::MakeErrorObject(strGroup, strErrMsgOverlapShade, 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pShade);
				return pShade;
			}
			if (pCheckShade->Overlap(pShade))
			{
				MSModelChecker::MakeErrorObject(strGroup, strErrMsgOverlapShade, 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pShade);
				return pShade;
			}
		}
	}
	return NULL;
}
MSWallOpeningG* MSModelChecker::CheckWallOpeningOverlap_ADD(MSWallMemberG* pWall, MSWallOpeningG* pCheckOpening, bool bMakeErrorObject)
{
	if (pWall == NULL || pCheckOpening == NULL)
		return NULL;
	
	if (CheckModel_WallOpeningStep2(pWall, pCheckOpening, bMakeErrorObject) == false)
		return pCheckOpening;

	map<long, MSWallOpeningG*>::iterator itOpening;

	CString strGroup = LocalDic::GetLocal(L"벽체 오프닝 중첩", L"StructureModel");
	CString strErrMsg;
	MSFloor* pFloor = pWall->GetFloor();
	strErrMsg.Format(LocalDic::GetLocal(L"%s : 입력된 벽체 오프닝이 기존 벽체 오프닝과 중첩됩니다.", L"StructureModel"), pFloor->GetName());

	for (itOpening = pWall->mm_WallOpening.begin() ; itOpening != pWall->mm_WallOpening.end(); itOpening++)
	{
		MSWallOpeningG* pOpening = itOpening->second;
		if (pOpening->Overlap(pCheckOpening, DTOL_GM, false))
		{
			MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pOpening);
			return pOpening;
		}
	}
	return NULL;
}

MSWallOpeningG* MSModelChecker::CheckWallOpeningOverlap_MOD(MSWallMemberG* pWall, bool bMakeErrorObject)
{
	if (pWall == NULL)
		return NULL;

	map<long, MSWallOpeningG*>::iterator itOpening;
	map<long, MSWallOpeningG*>::iterator itOpening2;

	CString strGroup = LocalDic::GetLocal(L"벽체 오프닝 중첩", L"StructureModel");
	CString strErrMsgOverlapOpening;
	MSFloor* pFloor = pWall->GetFloor();
	strErrMsgOverlapOpening.Format(LocalDic::GetLocal(L"%s : 수정된 벽체 오프닝이 기존 벽체 오프닝과 중첩됩니다.", L"StructureModel"), pFloor->GetName());

	for (itOpening = pWall->mm_WallOpening.begin(); itOpening != pWall->mm_WallOpening.end(); itOpening++)
	{
		MSWallOpeningG* pCheckOpening = itOpening->second;
		if (CheckModel_WallOpeningStep2(pWall, pCheckOpening, true) == false)
			return pCheckOpening;

		for (itOpening2 = pWall->mm_WallOpening.begin(); itOpening2 != pWall->mm_WallOpening.end(); itOpening2++)
		{
			MSWallOpeningG* pOpening = itOpening2->second;
			if (pCheckOpening == pOpening)
				continue;

			if (pCheckOpening->IsLevelOverlap(pOpening) && pOpening->Overlap(pCheckOpening, DTOL_GM, false))
			{
				MSModelChecker::MakeErrorObject(strGroup, strErrMsgOverlapOpening, 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pOpening);
				return pOpening;
			}
		}
	}
	return NULL;
}

// ------------------------------------ CheckOverLap END -----------------------------------------------------------

vector<long> MSModelChecker::CheckExtendOverlap(MSFloor* pFloor, MSCompoG* pCompoG, bool isUpdateGeometry /*= true*/)
{
	vector<long> ModifyCompoGArr;
	if(pCompoG->IsLinkMember() == FALSE)
		return ModifyCompoGArr;

	// 검토 대상 포인터 부재 + 같은 타입의 부재
	//라인부재를 LineHitDist 만큼 시작 끝은 연장 시킨다.
	//
	MSLinkMemberG* pLinkG = (MSLinkMemberG*)pCompoG;
	
	pLinkG->SetLines();

	// 아크 부재는 연장하지 않는다.
	if(pLinkG->isArc())
		return ModifyCompoGArr;

	MSElement::Type_MSCompoG eType = pCompoG->GetType();

	double dSExtendDist = MSFloor::ms_LineHitDistTol;
	double dEExtendDist = MSFloor::ms_LineHitDistTol;
	bool IsSVecExtend = false;
	bool IsEVecExtend = false;
	bool bCheckDetail = true;	//연장 길이를 다시 계산해야할 필요가 있을 경우 (ex 수평부재입력시 수직 부재까지 길이 연장)

	shared_ptr<GM2DCurve> pCenCurve = pLinkG->GetCenCurve();

	GM2DLineVector OrgSLine, OrgELine;
	GM2DLineVector SLine, ELine;
	SLine.m_P0 = pLinkG->mp_LLine->m_P0;
	SLine.m_P1 = pLinkG->mp_RLine->m_P0;
	ELine.m_P0 = pLinkG->mp_LLine->m_P1;
	ELine.m_P1 = pLinkG->mp_RLine->m_P1;

	OrgSLine = SLine;
	OrgELine = ELine;

	long nExtendType = 0;
	GM2DPolyline MyExtBoundary(TRUE);
	pLinkG->MakeExtendedBoundary(MyExtBoundary, nExtendType, dSExtendDist);
	
	GM2DCurve* pExSLine = MyExtBoundary.GetCurve(3);
	GM2DCurve* pExELine = MyExtBoundary.GetCurve(1);
	pExELine->ReverseDirection();

	GM2DCurve* pExSLine2 = pExSLine->Copy();
	GM2DCurve* pExELine2 = pExELine->Copy();
	
	GM2DPolyline MyStartExtBoundary(TRUE);
	MyStartExtBoundary.AddCopy(SLine.m_P0);
	MyStartExtBoundary.AddCopy(SLine.m_P1);
	MyStartExtBoundary.ma_Bulge.Add(0);
	MyStartExtBoundary.AddCopy(pExSLine2->m_P1);
	MyStartExtBoundary.ma_Bulge.Add(0);
	MyStartExtBoundary.AddCopy(pExSLine2->m_P0);
	MyStartExtBoundary.ma_Bulge.Add(0);
	MyStartExtBoundary.AddCopy(SLine.m_P0);
	MyStartExtBoundary.ma_Bulge.Add(0);

	GM2DPolyline MyEndExtBoundary(TRUE);
	MyEndExtBoundary.AddCopy(ELine.m_P0);
	MyEndExtBoundary.AddCopy(ELine.m_P1);
	MyEndExtBoundary.ma_Bulge.Add(0);
	MyEndExtBoundary.AddCopy(pExELine2->m_P1);
	MyEndExtBoundary.ma_Bulge.Add(0);
	MyEndExtBoundary.AddCopy(pExELine2->m_P0);
	MyEndExtBoundary.ma_Bulge.Add(0);
	MyEndExtBoundary.AddCopy(ELine.m_P0);
	MyEndExtBoundary.ma_Bulge.Add(0);
	
	SLine.ExtendLineVector(TRUE, dSExtendDist, FALSE);
	SLine.ExtendLineVector(FALSE, dEExtendDist, TRUE);
	ELine.ExtendLineVector(TRUE, dSExtendDist, FALSE);
	ELine.ExtendLineVector(FALSE, dEExtendDist, TRUE);

	pExSLine2->ExtendLineVector(TRUE, dSExtendDist, FALSE);
	pExSLine2->ExtendLineVector(FALSE, dEExtendDist, TRUE);
	pExELine2->ExtendLineVector(TRUE, dSExtendDist, FALSE);
	pExELine2->ExtendLineVector(FALSE, dEExtendDist, TRUE);

	GM2DPolyline MyStartExtBoundary2(TRUE);
	MyStartExtBoundary2.AddCopy(SLine.m_P0);
	MyStartExtBoundary2.AddCopy(SLine.m_P1);
	MyStartExtBoundary2.ma_Bulge.Add(0);
	MyStartExtBoundary2.AddCopy(pExSLine2->m_P1);
	MyStartExtBoundary2.ma_Bulge.Add(0);
	MyStartExtBoundary2.AddCopy(pExSLine2->m_P0);
	MyStartExtBoundary2.ma_Bulge.Add(0);
	MyStartExtBoundary2.AddCopy(SLine.m_P0);
	MyStartExtBoundary2.ma_Bulge.Add(0);
	
	GM2DPolyline MyEndExtBoundary2(TRUE);
	MyEndExtBoundary2.AddCopy(ELine.m_P0);
	MyEndExtBoundary2.AddCopy(ELine.m_P1);
	MyEndExtBoundary2.ma_Bulge.Add(0);
	MyEndExtBoundary2.AddCopy(pExELine2->m_P1);
	MyEndExtBoundary2.ma_Bulge.Add(0);
	MyEndExtBoundary2.AddCopy(pExELine2->m_P0);
	MyEndExtBoundary2.ma_Bulge.Add(0);
	MyEndExtBoundary2.AddCopy(ELine.m_P0);
	MyEndExtBoundary2.ma_Bulge.Add(0);

	GM2DLineVector SLineTmp;
	SLineTmp.m_P0 = SLine.MassCenter();
	SLineTmp.m_P1 = pExSLine->PointAtParam(0.5);

	GM2DLineVector ELineTmp;
	ELineTmp.m_P0 = ELine.MassCenter();
	ELineTmp.m_P1 = pExELine->PointAtParam(0.5);

	delete pExSLine;
	delete pExELine;
	delete pExSLine2;
	delete pExELine2;

	vector<MSFloor*> overlapFloor = pCompoG->GetOverlapFloor();

	// 다른 부재를 연장 시켜야 할 필요가 있는 부재 찾기 위해 ExCompoGArr를 사용한다.
	//vector<MSCompoG*> ExCompoGArr = MSCompoGUtil::FindCompoG(pFloor, &MyExtBoundary, FindType_MSElement::msNULL, true, true, dSExtendDist);
	//vector<MSCompoG*> CompoGArr = MSCompoGUtil::FindCompoG(pFloor, &MyExtBoundary, FindType_MSElement::msNULL);

	vector<MSCompoG*> ExCompoGArr;
	vector<MSCompoG*> CompoGArr;
	for (int idx = 0; idx < overlapFloor.size(); idx++)
	{
		vector<MSCompoG*> ExCompoGArr2 = MSCompoGUtil::FindCompoG(overlapFloor[idx], &MyExtBoundary, FindType_MSElement::msNULL, true, true, dSExtendDist);
		vector<MSCompoG*> CompoGArr2 = MSCompoGUtil::FindCompoG(overlapFloor[idx], &MyExtBoundary, FindType_MSElement::msNULL);

		ExCompoGArr.insert(ExCompoGArr.end(), ExCompoGArr2.begin(), ExCompoGArr2.end());
		CompoGArr.insert(CompoGArr.end(), CompoGArr2.begin(), CompoGArr2.end());
	}

	for(long i = ExCompoGArr.size() -1 ; i >= 0 ; i--)
	{
		MSCompoG* pCurCompoG = ExCompoGArr[i];

		if(pCurCompoG->IsLevelOverlap(pCompoG) == false)
		{
			ExCompoGArr.erase(ExCompoGArr.begin() + i);
			continue;
		}

		if(pCurCompoG != pCompoG)
		{
			if(pCurCompoG->IsVerticalMember())
			continue;
			if(pCurCompoG->IsFloorMember() && pCompoG->IsFloorMember())
				continue;
		}

		ExCompoGArr.erase(ExCompoGArr.begin() + i);
	}

	for(long i = CompoGArr.size() -1 ; i >= 0 ; i--)
	{
		MSCompoG* pCurCompoG = CompoGArr[i];
// 		if(pCompoG->IsFloorMember() && pCurCompoG->IsVerticalMember())
// 			bCheckDetail = true;

		if(pCurCompoG->IsLevelOverlap(pCompoG) == false)
		{
			CompoGArr.erase(CompoGArr.begin() + i);
			continue;
		}

		if(pCurCompoG->IsLinkMember() && pCompoG->IsLinkMember())
		{
			MSLinkMemberG* pCurLinkG = (MSLinkMemberG*)pCurCompoG;
			MSLinkMemberG* pLinkG = (MSLinkMemberG*)pCompoG;
			if(pCurLinkG->IsParallel(pLinkG))
			{
				CompoGArr.erase(CompoGArr.begin() + i);
				continue;
			}
		}

		if(pCurCompoG != pCompoG)
		{
			if(pCurCompoG->IsVerticalMember())
				continue;
			if(pCurCompoG->IsFloorMember() && pCompoG->IsFloorMember())
				continue;
		}

		CompoGArr.erase(CompoGArr.begin() + i);
	}

	for(long i = ExCompoGArr.size() -1 ; i >= 0 ; i--)
	{
		MSCompoG* pCurCompoG = ExCompoGArr[i];
		for(long j = 0 ; j < CompoGArr.size() ; j++)
		{
			if(pCurCompoG == CompoGArr[j])
			{
				ExCompoGArr.erase(ExCompoGArr.begin() + i);
				break;
			}
		}
	}

	double dMaxExtendDist = 0;
	for (int i = 0; i < CompoGArr.size(); i++)
	{
		MSCompoG* pCurCompoG = CompoGArr[i];
		dMaxExtendDist = max(dMaxExtendDist, max(pCurCompoG->GetThick(), max(pCurCompoG->GetWidth(), pCurCompoG->GetDepth())));
	}
	dMaxExtendDist *= 1.5;
	
	for(int i = 0 ; i < CompoGArr.size() ; i++)
	{
		MSCompoG* pCurCompoG = CompoGArr[i];
		GM2DPolyline* pTrimedBoundry = pCurCompoG->GetTrimmedBoundary();

		if (pCurCompoG->IsSRC() && pCurCompoG->GetType() == msColumnG && (pLinkG->IsSteel() || pLinkG->IsSRC()))
		{
			if (pTrimedBoundry->OverlapEx(&OrgSLine) == TRUE ||
				pTrimedBoundry->OverlapEx(MyStartExtBoundary) == TRUE)
			{
				IsSVecExtend = true;
				if (bCheckDetail)
				{
					GM2DPolyline ThePoly;
					GM2DPolyline* pCheckedBoundry = pTrimedBoundry;

					MSColumnG* pColumnG = dynamic_cast<MSColumnG*>(pCurCompoG);
					pColumnG->GetSteelSecPolylineByInsertVec(ThePoly);
					pCheckedBoundry = &ThePoly;

					GM2DCurve* pExtendCenCurve = pCenCurve->Copy();
					if (fabs(dMaxExtendDist) > 0)
						pExtendCenCurve->ExtendLineVector(TRUE, dMaxExtendDist, FALSE);

					vector<GM2DVector> VecArr = pCheckedBoundry->IntersectEx(pExtendCenCurve);
					delete pExtendCenCurve;

					if (VecArr.size() > 0)
					{
						GM2DVector SMinVec = pCenCurve->m_P0.GetMinDistanceVector(VecArr);
						double dSLoc = pCenCurve->OrthoBaseLocValue(SMinVec);
						dSExtendDist = pCenCurve->m_P0.Distance(SMinVec);
						if (dSLoc > 0)
							dSExtendDist *= -1;
					}
					else
						dSExtendDist = 0;
				}
			}

			if (pTrimedBoundry->OverlapEx(&OrgELine) == TRUE ||
				pTrimedBoundry->OverlapEx(MyEndExtBoundary) == TRUE)
			{
				IsEVecExtend = true;
				if (bCheckDetail)
				{
					GM2DPolyline ThePoly;
					GM2DPolyline* pCheckedBoundry = pTrimedBoundry;

					MSColumnG* pColumnG = dynamic_cast<MSColumnG*>(pCurCompoG);
					pColumnG->GetSteelSecPolylineByInsertVec(ThePoly);
					pCheckedBoundry = &ThePoly;

					GM2DCurve* pExtendCenCurve = pCenCurve->Copy();
					if (fabs(dMaxExtendDist) > 0)
						pExtendCenCurve->ExtendLineVector(FALSE, dMaxExtendDist, TRUE);

					vector<GM2DVector> VecArr = pCheckedBoundry->IntersectEx(pExtendCenCurve);
					delete pExtendCenCurve;

					if (VecArr.size() > 0)
					{
						GM2DVector EMinVec = pCenCurve->m_P1.GetMinDistanceVector(VecArr);
						double dELoc = pCenCurve->OrthoBaseLocValue(EMinVec);
						dEExtendDist = pCenCurve->m_P1.Distance(EMinVec);
						if (dELoc < 1)
							dEExtendDist *= -1;
					}
					else
						dEExtendDist = 0;
				}
			}
		}
		else
		{
			if (pTrimedBoundry->OverlapEx(&*pCenCurve) == TRUE)
				continue;

			if (pTrimedBoundry->OverlapEx(&OrgSLine) == TRUE)
				dSExtendDist = 0;
			else
			{
				if (pTrimedBoundry->OverlapEx(MyStartExtBoundary) == TRUE)
				{
					IsSVecExtend = true;
					if (bCheckDetail)
					{
						double dMinDist = dSExtendDist;
						for (long i = 0; i < pTrimedBoundry->ma_Point.GetSize() - 1; i++)
							dMinDist = min(dMinDist, SLine.Distance(*(pTrimedBoundry->ma_Point[i])));
						dSExtendDist = dMinDist;
					}
				}
			}

			if (pTrimedBoundry->OverlapEx(&OrgELine) == TRUE)
				dEExtendDist = 0;
			else
			{
				if (pTrimedBoundry->OverlapEx(MyEndExtBoundary) == TRUE)
				{
					IsEVecExtend = true;
					if (bCheckDetail)
					{
						double dMinDist = dEExtendDist;
						for (long i = 0; i < pTrimedBoundry->ma_Point.GetSize() - 1; i++)
							dMinDist = min(dMinDist, ELine.Distance(*(pTrimedBoundry->ma_Point[i])));
						dEExtendDist = dMinDist;
					}
				}
			}
		}

		if(IsSVecExtend && IsEVecExtend)
			break;
	}
	
	for(long i = 0 ; i < ExCompoGArr.size() ; i++)
	{
		MSCompoG* pCurCompoG = ExCompoGArr[i];
		if(pCurCompoG->IsLinkMember() == FALSE)
			continue;

		// 수평 부재입력시 수직 부재는 길이 연장을 하지 않는다.
		if(pCompoG->IsFloorMember() && pCurCompoG->IsVerticalMember())
			continue;

		MSLinkMemberG* pCurLinkG = (MSLinkMemberG*)pCurCompoG;

		if(pCurLinkG->IsParallel(pLinkG))
			continue;

		GM2DPolyline* pCurPoly = pCurLinkG->GetTrimmedBoundary();
		long nExSval2 = pCurPoly->OverlapEx(MyStartExtBoundary2);
		if(nExSval2 > 0)
		{
			IsSVecExtend = true;
			if(bCheckDetail)
			{
				double dMinDist = dSExtendDist;
				for(long i = 0 ; i < pCurPoly->ma_Point.GetSize() -1 ; i++)
					dMinDist = min(dMinDist, SLine.Distance(*(pCurPoly->ma_Point[i])));
				dSExtendDist = dMinDist;
			}
			ModifyCompoGArr.push_back(pCurCompoG->m_ID);
			continue;
		}

		long nExEval2 = pCurPoly->OverlapEx(MyEndExtBoundary2);
		if(nExEval2 > 0)
		{
			IsEVecExtend = true;
			if(bCheckDetail)
			{
				double dMinDist = dEExtendDist;
				for(long i = 0 ; i < pCurPoly->ma_Point.GetSize() -1 ; i++)
					dMinDist = min(dMinDist, ELine.Distance(*(pCurPoly->ma_Point[i])));
				dEExtendDist = dMinDist;
			}
			ModifyCompoGArr.push_back(pCurCompoG->m_ID);
			continue;
		}

		// 연장할 대상 추가
		// pCompoG의 길이는 변경하지 않으면서 pCurCompoG가 현재 함수 종료후에 다시 검토할 필요성이 있는 경우 추가한다.
		// ex) pCurCompoG가 pCompoG의 중간에 위치하는 경우, dSExtendDist를 사용하여 찾은 부재를 다시 검토한다.
		ModifyCompoGArr.push_back(pCurCompoG->m_ID);
	}

	if(isUpdateGeometry)
	{
		if(IsSVecExtend)
		{
			pCenCurve->ExtendLineVector(TRUE, dSExtendDist, FALSE);
			pLinkG->ma_Vec[0]->SetXY(pCenCurve->m_P0.m_X, pCenCurve->m_P0.m_Y);
		}
		if(IsEVecExtend)
		{
			pCenCurve->ExtendLineVector(FALSE, dEExtendDist, TRUE);
			pLinkG->ma_Vec[1]->SetXY(pCenCurve->m_P1.m_X, pCenCurve->m_P1.m_Y);
		}

		if(IsSVecExtend || IsEVecExtend)
		{
			pLinkG->SetLines();
			if(pLinkG->GetTrimmedBoundary() != NULL)
				pLinkG->GetBoundary(*pLinkG->GetTrimmedBoundary());
		}
	}

	return ModifyCompoGArr;
}

void MSModelChecker::FixPolylineGap_Sort(vector<MSElement*>& elementArr)
{
	map<int, MSElement*> mapElem[4];
	for (int i=0 ; i<elementArr.size() ; ++i)
	{
		MSElement* elem = elementArr[i];
		switch(elem->GetType())
		{
		case MSElement::msSlabG: mapElem[0][ elem->m_ID ] = elem; break;
		case MSElement::msOpeningG: mapElem[1][ elem->m_ID ] = elem; break;
		case MSElement::msLevelZone: mapElem[2][ elem->m_ID ] = elem; break;
		case MSElement::msConstructionZone: mapElem[3][ elem->m_ID ] = elem; break;
		}
	}

	elementArr.clear();
	for (int i=0 ; i<4 ; ++i)
	{
		for (map<int, MSElement*>::iterator itr = mapElem[i].begin() ; itr != mapElem[i].end() ; ++itr)
			elementArr.push_back(itr->second);
	}
}

void MSModelChecker::FixPolylineGap_StartEdit(map<MSElement*, int>& mapProcessed, MSFloor* floor, MSElement* Element)
{
	if (mapProcessed.count(Element) == 0)
	{
		if(Element->IsCompoG())
			Element->StartEdit(CmdProcess::CommandUpdateElementType::CompoG, Element->m_ID, floor->m_ID, true);
		else if(Element->GetType() == MSElement::msLevelZone)
			Element->StartEdit(CmdProcess::CommandUpdateElementType::LevelZone, Element->m_ID, floor->m_ID, true);
		else if(Element->GetType() == MSElement::msConstructionZone)
			Element->StartEdit(CmdProcess::CommandUpdateElementType::ConstructionZone, Element->m_ID, floor->m_ID, true);
		mapProcessed[ Element ] = 1;
	}
}

void MSModelChecker::FixPolylineGap(vector<MSElement*>& elementArr)
{
	//AllocConsole();
	//AttachConsole( GetCurrentProcessId() ) ;
	//freopen( "CON", "w", stdout ) ;

	FixPolylineGap_Sort(elementArr);

	map<MSElement*, bool> mapTarget;
	for (int i=0 ; i<elementArr.size() ; ++i)
		mapTarget[elementArr[i]] = true;

	CString str;
	//try
	//{
	//reporter = ProgressReporterWrapper::Instance()->GetProgressReporterWithBeginWaiting("Fix Element Gap");
	ProgressReporterWrapper::Instance()->SetProgressMaxValue(100);
	int last = 0;

	map<MSElement*, int> mapProcessed;

	////////////////////////////////////////////////////////////////////////////////////////////
	// 1) Point Check
	for (int i=0 ; i< elementArr.size() ; ++i)
	{
		int percent = (int)((i * 50.0)/(elementArr.size()));
		if (percent % 5 == 0 && percent != last)
		{
			ProgressReporterWrapper::Instance()->AddProgress(5);
			last = percent;
			CString strMsg;
			strMsg.Format(LocalDic::GetLocal(L"Processing fix Element-gap (%d %%)", L"StructureModel"), percent);
			ProgressReporterWrapper::Instance()->AddStatus( strMsg);
		}

		//MSElement* Element = dynamic_cast<MSElement*>(elementList[i]->NativeMSPtr());
		MSElement* Element = elementArr[i];
		GM2DPolyline* profile = Element->GetProfile();
		//str.Format(_T("- Checking Element #%d\n"), Element->m_ID); wprintf(str);

		MSFloor* floor = dynamic_cast<MSFloor*>(Element->GetFloor());
		if(floor == NULL)
			continue;

		//////////////////////////////////////////////////////////////////////////////////////
		// Self Check
		for (int k=profile->ma_Point.GetSize()-1 ; k>=1 ; --k)
		{
			if (profile->ma_Point[k]->Distance(*profile->ma_Point[k-1]) < DefHitDist)
			{
				//str.Format(_T("    - FIXING DEL : Element(%d) - (%g, %g)\n"), Element->m_ID, profile->ma_Point[k]->m_X, profile->ma_Point[k]->m_Y);
				//wprintf(str);
				FixPolylineGap_StartEdit(mapProcessed, floor, Element);
				profile->ma_Point.RemoveAt(k);
				profile->ma_Bulge.RemoveAt(k-1);
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////

		//vector<MSCompoG*> neighbor = floor->FindCompoG(Element->GetProfile(), MSElement::msElementG, false, true, DefHitDist);
		vector<MSElement*> neighbor = MSCompoGUtil::FindPolyElement(floor, Element->GetProfile(), false, true, DefHitDist);

		vector<MSElement*> ElementList_neighbor;
		for (int k=0 ; k<neighbor.size() ; ++k)
		{
			MSElement* tgtElement = dynamic_cast<MSElement*>(neighbor[k]);
			if (tgtElement == NULL) continue;
			if (tgtElement->m_ID == Element->m_ID) continue;
			if (tgtElement == Element) continue;
			ElementList_neighbor.push_back(tgtElement);
		}

		bool error = false;
		for (int k=0 ; k<ElementList_neighbor.size() ; ++k)
		{
			MSElement* tgtElement = ElementList_neighbor[k];
			GM2DPolyline* tgtProfile = tgtElement->GetProfile();
			int Idx = profile->CheckPolylineSegmentDist(tgtProfile, 10);
			//str.Format(_T("  - Checking with Element #%d .....  %s\n"), tgtElement->m_ID, ok ? _T("OK") : _T("ERROR!")); wprintf(str);
			if (Idx != -1) error = true;
		}

		if (!error) continue;
		
		str.Format(LocalDic::GetLocal(L"- ERROR found in Element #%d", L"StructureModel"), Element->m_ID);
		//ProgressReporterWrapper::Instance()->AddStatus(marshal_as<String^>(str));
		//wprintf(str);
		
		//wprintf(_T("  1) Point Check\n"));
		for (int k=0 ; k<profile->ma_Point.GetSize()-1 ; ++k)
		{
			vector<MSElement*> errorPtElement;
			vector<int> errorPtIdx;
			vector<double> errorDist;
			for (int m=0 ; m<ElementList_neighbor.size() ; ++m)
			{
				MSElement* tgtElement = ElementList_neighbor[m];
				GM2DPolyline* tgtProfile = tgtElement->GetProfile();
				for (int n=0 ; n<tgtProfile->ma_Point.GetSize()-1 ; ++n)
				{
					double dist = profile->ma_Point[k]->Distance(*tgtProfile->ma_Point[n]);
					if (DTOL_GM < dist && dist <= DefHitDist  )
					{
						errorPtElement.push_back(tgtElement); errorPtIdx.push_back(n); errorDist.push_back(dist);
						if (n == 0)
						{
							errorPtElement.push_back(tgtElement); errorPtIdx.push_back(tgtProfile->ma_Point.GetSize()-1); errorDist.push_back(dist);
						}
					}
				}
			}
			
			if (errorPtIdx.size() == 0)
			{
				//str.Format(_T("    - Checking Point %d : OK\n"), k); wprintf(str);
			}
			else
			{
				GM2DVector* pt = profile->ma_Point[k];
				//str.Format(_T("    - Checking Point %d : ERROR (%g, %g) - ErrorPtNum(%d)\n"), k, pt->m_X, pt->m_Y, errorPtIdx.size());
				//wprintf(str);

				for (int m=0 ; m<errorPtElement.size() ; ++m)
				{
					FixPolylineGap_StartEdit(mapProcessed, floor, errorPtElement[m]);

					GM2DVector* ref = profile->ma_Point[k];

					//GM2DVector* src = errorPtElement[m]->GetProfile()->ma_Point[ errorPtIdx[m] ];
					//str.Format(_T("    - FIXING PT : Element:%d (%g, %g) -> (%g, %g) \n"), errorPtElement[m]->m_ID, src->m_X, src->m_Y, ref->m_X, ref->m_Y);
					//wprintf(str);
					
					errorPtElement[m]->GetProfile()->ma_Point[ errorPtIdx[m] ]->CopyFrom( ref );
				}

			}
		}
	}
	// 1) Point Check END
	////////////////////////////////////////////////////////////////////////////////////////////

	// Delete Duplicate Point (before Edge Check)
	for (map<MSElement*, int>::iterator itr = mapProcessed.begin() ; itr != mapProcessed.end(); ++itr)
	{
		GM2DPolyline* profile = itr->first->GetProfile();

		for (int i=profile->ma_Point.GetSize()-1 ; i>=1 ; --i)
		{
			if (profile->ma_Point[i]->Distance(*profile->ma_Point[i-1]) < DTOL_GM)
			{
				profile->ma_Point.RemoveAt(i);
				profile->ma_Bulge.RemoveAt(i-1);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	// 2) Edge Check
	for (int i=0 ; i<elementArr.size(); ++i)
	{
		int percent = 50 + (int)((i * 50.0)/(elementArr.size()));
		if (percent % 5 == 0 && percent != last)
		{
			ProgressReporterWrapper::Instance()->AddProgress(5);
			last = percent;
			CString strMsg;
			strMsg.Format(LocalDic::GetLocal(L"Processing fix Element-gap (%d %%)", L"StructureModel"), percent);
			ProgressReporterWrapper::Instance()->AddStatus(strMsg);
		}

		MSElement* Element = elementArr[i];
		GM2DPolyline* profile = Element->GetProfile();
		//str.Format(_T("- Checking Element #%d\n"), Element->m_ID); wprintf(str);

		MSFloor* floor = dynamic_cast<MSFloor*>(Element->GetFloor());

		vector<MSElement*> neighbor = MSCompoGUtil::FindPolyElement(floor, Element->GetProfile(), false, true, DefHitDist);
		vector<MSElement*> ElementList_neighbor;
		for (int k=0 ; k<neighbor.size() ; ++k)
		{
			MSElement* tgtElement = dynamic_cast<MSElement*>(neighbor[k]);
			if (tgtElement == NULL) continue;
			if (tgtElement->m_ID == Element->m_ID) continue;
			ElementList_neighbor.push_back(tgtElement);
		}

		bool error = false;
		for (int k=0 ; k<ElementList_neighbor.size() ; ++k)
		{
			MSElement* tgtElement = ElementList_neighbor[k];
			GM2DPolyline* tgtProfile = tgtElement->GetProfile();
			int Idx = profile->CheckPolylineSegmentDist(tgtProfile, 10);
			//str.Format(_T("  - Checking with Element #%d .....  %s\n"), tgtElement->m_ID, ok ? _T("OK") : _T("ERROR!")); wprintf(str);
			if (Idx != -1) error = true;
		}

		if (!error) continue;
		
		str.Format(LocalDic::GetLocal(L"- ERROR found in Element #%d", L"StructureModel"), Element->m_ID);
		//ProgressReporterWrapper::Instance()->AddStatus(marshal_as<String^>(str));
		//wprintf(str);

		
		vector<int> edgeIdxList;
		vector<GM2DVector> orthoPointList;
		for (int k=0 ; k<profile->ma_Point.GetSize()-1 ; ++k)
		{
			map<double, GM2DVector> mapLocToPt;
			bool exist = false;

			GM2DVector* p1 = profile->ma_Point[k];
			GM2DVector* p2 = profile->ma_Point[k+1];
			GM2DLineVector edge(p1->m_X, p1->m_Y, p2->m_X, p2->m_Y);
			for (int m=0 ; m<ElementList_neighbor.size() ; ++m)
			{
				MSElement* tgtElement = ElementList_neighbor[m];
				GM2DPolyline* tgtProfile = tgtElement->GetProfile();
				for (int n=0 ; n<tgtProfile->ma_Point.GetSize()-1 ; ++n)
				{
					GM2DVector* point = tgtProfile->ma_Point[n];
					double loc = edge.OrthoBaseLocValue( *point );
					if (0.001 < loc && loc < 0.999)
					{
						GM2DVector refPt = edge.OrthoBaseVector( *point );
						double dist = refPt.Distance( *point );
						if (dist <= DTOL_GM)
						{
							mapLocToPt[loc] = (*point);
						}
						else if (DTOL_GM < dist && dist <= DefHitDist  )
						{
							//str.Format(_T("        Point %d with Element %d edge %d: ERROR (dist:%g)\n"), k, tgtElement->m_ID, n, dist); wprintf(str);
							//closeElementList.push_back(tgtElement);
							mapLocToPt[loc] = (*point);
							exist = true;
						}
					}
				}
			}
			if (exist)
			{
				for (map<double, GM2DVector>::iterator itr = mapLocToPt.begin() ; itr != mapLocToPt.end() ; ++itr)
				{
					edgeIdxList.push_back(k);
					orthoPointList.push_back(itr->second);
				}
			}
		}

		if (edgeIdxList.size() > 0)
		{
			FixPolylineGap_StartEdit(mapProcessed, floor, Element);

			for (int k=(int)edgeIdxList.size()-1 ; k>=0 ; --k)
			{
				int edge_idx = edgeIdxList[k];
				GM2DPolyline* profile = Element->GetProfile();
				profile->ma_Point.InsertAt(edge_idx + 1, new GM2DVector(orthoPointList[k].m_X, orthoPointList[k].m_Y));
				profile->ma_Bulge.InsertAt(edge_idx + 1, 0.0);
				//str.Format(_T("    - FIXING EDGE : Element(%d) - (%g, %g)\n"), Element->m_ID, orthoPointList[k].m_X, orthoPointList[k].m_Y);
				//wprintf(str);
			}

			while(true)
			{
				GM2DPolyline* profile = Element->GetProfile();
				int delStart = 0;
				int delEnd = 0;
				for (int k=0 ; k<profile->ma_Point.GetSize()-1 ; ++k)
				{
					for (int m=k+1 ; m<profile->ma_Point.GetSize()-1 ; ++m)
					{
						if (profile->ma_Point[k]->Distance( *profile->ma_Point[m] ) < DTOL_GM )
						{
							delStart = k;
							delEnd = m;
						}
					}
				}
				if (delEnd != 0)
				{
					double dist1 = 0, dist2 = 0;
					for (int k=0 ; k<profile->ma_Point.GetSize()-1 ; ++k)
					{
						if (delStart <= k && k < delEnd)
							dist1 += profile->ma_Point[k]->Distance( *profile->ma_Point[k+1]);
						else
							dist2 += profile->ma_Point[k]->Distance( *profile->ma_Point[k+1]);
					}
					if (dist1 < dist2)
					{
						for (int k=delEnd ; k>delStart ; --k)
						{
							profile->ma_Point.RemoveAt(k);
							profile->ma_Bulge.RemoveAt(k);
						}
					}
					else
					{
						for (int k=profile->ma_Point.GetSize()-1 ; k>=0; --k)
						{
							if (k < delStart || k > delEnd) profile->ma_Point.RemoveAt(k);
						}
						profile->ma_Bulge.SetSize(profile->ma_Point.GetSize()-1);
					}
				}
				else
					break;
			}
		}
	}
	// 2) Edge Check END
	////////////////////////////////////////////////////////////////////////////////////////////

	StatusReporterWrapper::OutputMessageWithFocus(_T("-----------------------------------------------------------------"));
	StatusReporterWrapper::OutputMessage(LocalDic::GetLocal(L"Fix Element Gap Report", L"StructureModel"));

	//ProgressReporter::AddStatus(_T("-----------------------------------------------------------------"));
	//ProgressReporter::AddStatus(LocalDic::GetLocal(L"Fix Element Gap Report", L"StructureModel"));

	for (map<MSElement*, int>::iterator itr = mapProcessed.begin() ; itr != mapProcessed.end(); ++itr)
	{
		GM2DPolyline* profile = itr->first->GetProfile();
		int num = profile->ma_Point.GetSize();
		profile->ma_Point[num-1]->m_X = profile->ma_Point[0]->m_X;
		profile->ma_Point[num-1]->m_Y = profile->ma_Point[0]->m_Y;

		str.Format(LocalDic::GetLocal(L"Element ID:%d Modified.", L"StructureModel"), itr->first->m_ID);
		StatusReporterWrapper::OutputMessage(str);
		//ProgressReporter::AddStatus(marshal_as<String^>(str));

		MSElement* pElement = itr->first;
		if(pElement->IsCompoG())
		{
			MSCompoG* pCompoG = (MSCompoG*)pElement;
			pCompoG->UpdateBoundaryVisibilities();
		}
		itr->first->EndEdit_SkipTrimAgain();
	}

	str.Format(LocalDic::GetLocal(L"Fix Element Gap Finished. - %d Elements Modified", L"StructureModel"), mapProcessed.size());
	StatusReporterWrapper::OutputMessage(str);
	//ProgressReporter::AddStatus(marshal_as<String^>(str));
	StatusReporterWrapper::OutputMessage(_T("-----------------------------------------------------------------"));

	//wprintf(_T("\nFix Element Gap Finished.\n"));
	//wprintf(_T("Do not close this window. Generationg model data...\n"));
	StatusReporterWrapper::OutputMessage(LocalDic::GetLocal(L"\nFix Element Gap Finished.", L"StructureModel"));
	StatusReporterWrapper::OutputMessage(LocalDic::GetLocal(L"Generationg model data...Please Wait.", L"StructureModel"));
	StatusReporterWrapper::OutputMessage(_T("-----------------------------------------------------------------"));
	//FreeConsole();
	//}
	//catch (CException*)
	//{
	//}
}

void MSModelChecker::InitProgressReporter(CString strTitle)
{
	m_nCheckCnt = 0;
	m_nlast = 0;
	m_nBuildElementCnt = mp_Build->GetModelCheckElementCnt();
	if(m_nBuildElementCnt > 30)
	{
		ProgressReporterWrapper::GetProgressReporterWithBeginWaiting(strTitle);
		ProgressReporterWrapper::Instance()->SetProgressMaxValue(100);
	}
}

void MSModelChecker::UpdateProgressReporter()
{
	if(m_nBuildElementCnt > 30)
	{
		m_nCheckCnt++;
		int nProgresStep = 5;
		int percent = (int)((m_nCheckCnt*100)/(m_nBuildElementCnt));
		while (percent > m_nlast)
		{
			ProgressReporterWrapper::Instance()->AddProgress(nProgresStep);
			m_nlast += nProgresStep;
			CString strMsg;
			strMsg.Format(LocalDic::GetLocal(L"Processing Check Model (%d %%)"), m_nlast);
			ProgressReporterWrapper::Instance()->AddStatus( strMsg);
		}
	}
}

void MSModelChecker::CloseProgressReporter()
{
	ProgressReporterWrapper::DisposeProgressReporter();
}

bool MSModelChecker::CheckElement(MSElement* pElement)
{
	if (pElement->GetType() == MSElement::msNULL)
		return false;

	if (pElement->GetIsTempCompoG() == true)
		return false;

	CString strGroupName = GetStrGroupName(pElement);

	if (pElement->IsPolyMember())
	{
		GM2DPolyline* pNewPoly = pElement->GetProfile();
		if (pNewPoly == NULL)
			return false;

		if (pNewPoly->CheckPolygon(false) == FALSE)
		{
			MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"폴리라인 입력이 잘못 되었습니다.", L"StructureModel"), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pElement);
			return true;
		}
	}
	else if (pElement->IsLinkMember())
	{
		MSLinkMemberG* pLinkG = (MSLinkMemberG*)pElement;
		double dLinkGLength = pLinkG->GetLength();
		
		if (CheckLength(dLinkGLength, _T("<"), MSFloor::ms_DefLineHitDistTol) == true)
		{
			MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"라인부재의 길이가 10 보다 작습니다.", L"StructureModel"), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pLinkG);
			return true;
		}
		if (pLinkG->GetWidth() <= 0)
		{
			MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"라인부재의 Width가 0 보다 작습니다.", L"StructureModel"), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pLinkG);
			return true;
		}
	}
	return CheckInValidElement(pElement);
}

bool MSModelChecker::CheckInValidElement(MSElement* pElement, bool checkSubElement)
{
	MSElement::Type_MSCompoG eType = pElement->GetType();

	if (eType == MSElement::msPitG)
		return CheckInValidElement((MSPitG*)pElement, checkSubElement);
	else if (eType == MSElement::msWallG)
		return CheckInValidElement((MSWallMemberG*)pElement, checkSubElement);
	else if (eType == MSElement::msSlabRebar)
		return CheckInValidElement((MSSlabRebar*)pElement);
	else if (eType == MSElement::msSlabG)
		return CheckInValidElement((MSSlabG*)pElement, checkSubElement);
	else if (eType == MSElement::msStairG)
		return CheckInValidElement((MSGenStairG*)pElement, checkSubElement);
	else if (eType == MSElement::msBeamG)
		return CheckInValidElement((MSBeamG*)pElement, checkSubElement);
	else if (eType == MSElement::msColumnG)
		return CheckInValidElement((MSColumnG*)pElement, checkSubElement);
	else if (eType == MSElement::msWallFootG)
		return CheckInValidElement((MSWallFootG*)pElement, checkSubElement);
	else if (eType == MSElement::msFWSoundAbsorptionG)
		return CheckInValidElement((MSFWSoundAbsorptionG*)pElement);
	return false;
}

bool MSModelChecker::CheckInValidElement(MSPitG* pPit, bool checkSubElement)
{
	bool bOverLap = false;

	for (long iPoly = 0; iPoly < pPit->ma_InPoly.GetSize(); iPoly++)
	{
		GM2DPolyline* pInPoly = pPit->ma_InPoly[iPoly];
		if (pInPoly->CheckPolygon(false) == FALSE)
		{
			bOverLap = true;
			break;
		}

		if (pPit->mp_OutPoly->Overlap(*pInPoly, DTOL_GM))
		{
			bOverLap = true;
			break;
		}
	}

	if (bOverLap == false)
	{
		for (long iPit = 0; iPit < pPit->ma_InPoly.GetSize(); iPit++)
		{
			GM2DPolyline* pIPoly = pPit->ma_InPoly[iPit];
			for (long jPit = 0; jPit < pPit->ma_WellPoly.GetSize(); jPit++)
			{
				GM2DPolyline* pJPoly = pPit->ma_WellPoly[jPit];
				if (pJPoly->CheckPolygon(false) == FALSE)
				{
					bOverLap = true;
					break;
				}
				BOOL IsPolylineOverlap = pIPoly->OverlapEx(*pJPoly, DTOL_GM, TRUE);
				if(IsPolylineOverlap)
				{
					if (!pIPoly->Contains(pJPoly, TRUE, DTOL_GM))
					{
						bOverLap = true;
						break;
					}
				}
			}
			if (bOverLap)	break;
		}
	}

	if (bOverLap == false)
	{
		for (long iPit = 0; iPit < pPit->ma_InPoly.GetSize(); iPit++)
		{
			GM2DPolyline* pIPoly = pPit->ma_InPoly[iPit];
			for (long jPit = iPit + 1; jPit < pPit->ma_InPoly.GetSize(); jPit++)
			{
				GM2DPolyline* pJPoly = pPit->ma_InPoly[jPit];

				if (pIPoly->OverlapEx(*pJPoly, DTOL_GM, FALSE))
				{
					bOverLap = true;
					break;
				}
			}
		}
	}

	if (bOverLap == false)
	{
		for (long iWell = 0; iWell < pPit->ma_WellPoly.GetSize(); iWell++)
		{
			GM2DPolyline* pIPoly = pPit->ma_WellPoly[iWell];
			for (long jWell = iWell + 1; jWell < pPit->ma_WellPoly.GetSize(); jWell++)
			{
				GM2DPolyline* pJPoly = pPit->ma_WellPoly[jWell];

				if (pIPoly->OverlapEx(*pJPoly, DTOL_GM, FALSE))
				{
					bOverLap = true;
					break;
				}
			}
		}
	}

	bool IsInValidHaunch = CheckInValidHaunch(pPit, pPit->ma_Haunches);
	if (IsInValidHaunch)
		return true;

	CString strGroupName = GetStrGroupName(pPit);

	bool bRval = bOverLap;
	if (bOverLap)
		MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"폴리라인 입력이 잘못 되었습니다.", L"StructureModel"), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pPit);
	else if (pPit->m_dInnerDepth >= pPit->m_dThick)
	{
		MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName +LocalDic::GetLocal(L"집수정의 깊이가 피트 두께보다 클수 없습니다.", L"StructureModel"), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pPit);
		bRval = true;
	}
	
	if (bRval == true)	return bRval;

	bool bCheck = checkInValidAddRebarArea(pPit, !checkSubElement);
	return checkSubElement ? bCheck : false;
}

bool MSModelChecker::CheckInValidElement(MSWallMemberG* pWall, bool checkSubElement)
{
	if (pWall->CheckHeight(true) == false)
		return true;

	if (MSModelChecker::CheckWallOpeningOverlap_MOD(pWall, true) != NULL)
		return true;
	
	if(pWall->CheckShadeOverlap() == true)
		return true;

	bool bCheck = checkInValidAddRebarArea(pWall, !checkSubElement);
	return checkSubElement ? bCheck : false;
}

bool MSModelChecker::CheckInValidElement(MSColumnG* pColumn, bool checkSubElement)
{
	if (pColumn->CheckHeight(true) == false)
		return true;

	bool bCheck = checkInValidAddRebarArea(pColumn, !checkSubElement);
	return checkSubElement ? bCheck : false;
}

bool MSModelChecker::CheckInValidElement(MSWallFootG* pWallFoot, bool checkSubElement)
{
	bool IsInValidHaunch = CheckInValidHaunch(pWallFoot, pWallFoot->ma_Haunches);
	if (IsInValidHaunch)
		return true;

	bool bCheck = checkInValidAddRebarArea(pWallFoot, !checkSubElement);
	return checkSubElement ? bCheck : false;
}

bool MSModelChecker::CheckInValidElement(MSIsoFootG* pIsoFoot, bool checkSubElement)
{
	bool IsInValidHaunch = CheckInValidHaunch(pIsoFoot, pIsoFoot->ma_Haunches);
	if (IsInValidHaunch)
		return true;

	bool bCheck = checkInValidAddRebarArea(pIsoFoot, !checkSubElement);
	return checkSubElement ? bCheck : false;
}

bool MSModelChecker::CheckInValidHaunch(MSCompoG* pCompoG, vector<MSSubHaunchG*> Haunches)
{
	CString strGroupName = GetStrGroupName(pCompoG);

	for (int iHaunch = 0; iHaunch < Haunches.size(); iHaunch++)
	{
		MSSubHaunchG* pHaunchG = Haunches[iHaunch];
		CString strSubGroupName = _T("");
		if (pHaunchG->m_ID != 0)
			strSubGroupName.Format(_T(", %s(ID:%d)]: "), pHaunchG->GetGroupName(), pHaunchG->m_ID);

		if (pHaunchG->GetThick() <= DTOL_MODELING)
		{
			MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + strSubGroupName + LocalDic::GetLocal(L"헌치의 두께가 너무 작습니다.", L"StructureModel"), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pCompoG);
			return true;
		}

		if (pHaunchG->GetWidth() <= DTOL_MODELING)
		{
			MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + strSubGroupName + LocalDic::GetLocal(L"헌치의 넓이가 너무 작습니다.", L"StructureModel"), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pCompoG);
			return true;
		}
	}
	
	return false;
}

bool MSModelChecker::CheckValidMSSlabRebarBentLine(MSSlabRebar* pRebar)
{
	MSSlabRebarInfo* pRebarInfo = pRebar->GetRebarInfo();
	if (pRebarInfo->GetRebarType() != MSSlabRebarInfo::Bent)
		return true;

	long NumVec = pRebar->m_BentLine.GetSize();
	if (NumVec == 0)
		return false;

	GM2DPolyline* pRebarPoly = pRebar->GetProfile();

	long nRVal = pRebarPoly->Contains(&pRebar->m_BentLine.FirstPnt());
	if (nRVal > 0)
		return false;

	nRVal = pRebarPoly->Contains(&pRebar->m_BentLine.LastPnt());
	if (nRVal > 0)
		return false;

	return true;
}

bool MSModelChecker::CheckInValidElement(MSSlabRebar* pRebar)
{
	if (CheckValidMSSlabRebarBentLine(pRebar) == false)
	{
		MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), LocalDic::GetLocal(L"벤트라인 입력이 잘못 되었습니다.", L"StructureModel"), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pRebar);
		return true;
	}
	return false;
}

bool MSModelChecker::checkInValidAddRebarArea(MSCompoG* compoG, bool checkAll)
{
	CString strFloorNum = _T("");
	int nFloorNum = compoG->GetFloorNumForUser();
	if (nFloorNum > 0)
		strFloorNum.Format(_T("%dF : "), nFloorNum);
	else if (nFloorNum < 0)
		strFloorNum.Format(_T("B%dF : "), nFloorNum);

	CString strGroupName = GetStrGroupName(compoG);

	for (auto it = compoG->mm_AddRebar.begin(); it != compoG->mm_AddRebar.end(); it++)
	{
		MSAddRebarBase* pAddRebar = it->second;
		GM2DPolyline* pProfile = pAddRebar->GetProfile();		
		
		if (pProfile->CheckPolygon(false) == FALSE)
		{			
			if(checkAll)
				strGroupName.Format(_T("[%s%s(Owner ID:%d, ID:%d)]: "), strFloorNum, compoG->GetGroupName(), compoG->m_ID, pAddRebar->m_ID);
			MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"폴리라인 입력이 잘못 되었습니다.", L"StructureModel"), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, compoG);			
			if(!checkAll)
				return true;
		}
		if (pAddRebar->GetType() == MSElement::msAddRebarAreaBent)
		{
			// 절곡점 검토필요
			MSAddRebarAreaBent* pAreaBent = dynamic_cast<MSAddRebarAreaBent*>(pAddRebar);
			if (pAddRebar == nullptr)	continue;
			if(pAreaBent->CheckBentLocation(pProfile) == false)
			{
				if(checkAll)
					strGroupName.Format(_T("[%s%s(Owner ID:%d, ID:%d)]: "), strFloorNum, compoG->GetGroupName(), compoG->m_ID, pAddRebar->m_ID);
				MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"폴리라인 입력이 잘못 되었습니다.", L"StructureModel"), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, compoG);			
				if(!checkAll)
					return true;
			}
		}
	}
	return false;
}

bool MSModelChecker::CheckInValidElement(MSSlabG* pSlab, bool checkSubElement)
{
	CString strGroupName = GetStrGroupName(pSlab);

	// 경사 정보를 검토하여 이상하면 경사정보를 삭제한다	
	if (pSlab->IsInValidSlopeInfo())
		MSModelChecker::MakeErrorObject(LocalDic::GetLocal(_T("모델링 오류")), strGroupName + LocalDic::GetLocal(_T("경사정보가 손상되어 초기화 되었습니다.")), 0, MESSAGE_LEVEL_WARNING, MESSAGE_ERROR_OVERLAP, pSlab);

	GM2DPolyline* pProfile = pSlab->GetProfile();
	if (pProfile->CheckPolygon(false) == FALSE)
	{
		MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"폴리라인 입력이 잘못 되었습니다.", L"StructureModel"), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pSlab);
		return true;
	}

	double dSlabThick = pSlab->GetThick();
	for (auto it = pSlab->mm_Trench.begin(); it != pSlab->mm_Trench.end(); it++)
	{
		MSTrench* pTrench = it->second;
		if (pTrench->GetProfile()->CheckPolygon(false) == FALSE)
		{
			MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"폴리라인 입력이 잘못 되었습니다.", L"StructureModel"), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pSlab);
			return true;
		}
		if (pTrench->m_Depth > dSlabThick)
		{
			MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"슬래브 두께보다 트랜치 깊이가 큽니다."), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pSlab);
			return true;
		}
	}

	for (auto it = pSlab->mm_Well.begin(); it != pSlab->mm_Well.end(); it++)
	{
		MSWell* pWell = it->second;
		if (pWell->GetProfile()->CheckPolygon(false) == FALSE)
		{
			MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"폴리라인 입력이 잘못 되었습니다.", L"StructureModel"), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pSlab);
			return true;
		}
		if (pWell->m_Depth > dSlabThick)
		{
			MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName +LocalDic::GetLocal(L"슬래브 두께보다 집수정 깊이가 큽니다."), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pSlab);
			return true;
		}
	}

	bool IsInValidHaunch = CheckInValidHaunch(pSlab, pSlab->ma_Haunches);
	if (IsInValidHaunch)
		return true;

	if (pSlab->IsSlopeMember())
	{
		if(pSlab->m_nSlopeMethod == MSSlabG::PointOffset_1)
			if (pSlab->GetTrimmedBoundary_Acis()->GetSize() - 1 > 4)
			{
				MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"슬래브의 가장자리가 5개 이상일 때는 절점 Z좌표 변경을 사용할 수 없습니다."), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pSlab);
				pSlab->ClearSlopeInfo();
			}
	}	
	bool bCheck = checkInValidAddRebarArea(pSlab, !checkSubElement);
	return checkSubElement ? bCheck : false;
}

bool MSModelChecker::CheckInValidElement(MSGenStairG* pStair, bool checkSubElement)
{
	long NumLanding = pStair->ma_Landing.GetSize();
	long NumStep = pStair->ma_Step.GetSize();
	if (NumLanding + NumStep == 0)
		return true;

	bool IsPolyLineError = false;
	bool IsStepsNumError = false;
	bool IsLevelError = false;
	bool IsShellPlanLenError = false;
	bool IsPartLevelError = false;

	bool IsStepMError = false;
	bool IsLandingMError = false;
	MSBaseBuilding* pBuild = pStair->GetBuilding();

	for (int i = 0; i < NumLanding; i++)
	{
		if(IsPolyLineError == true)
			break;
		MSStairPart* pLanding = pStair->ma_Landing[i];
		if (pLanding->m_pPolyline->CheckPolygon(false) == FALSE)
			IsPolyLineError = true;
		if (pLanding->GetStepsNum() < 1)
			IsStepsNumError = true;

		MSCompoM* pLandingM = pLanding->GetCompoM();
		if (pLandingM->GetType() != MSCompoM::msStairLandingM)
		{
			IsLandingMError = true;
			MSCompoM* pUpdateLandingM = pBuild->GetCompoM(pLandingM->GetName(), MSCompoM::msStairLandingM);
			if (pUpdateLandingM != nullptr)
				pLanding->SetCompoM(pUpdateLandingM);
			else
			{
				MSCompoM* pNewCompoM = pBuild->CreateNewCompoM(MSCompoM::msStairLandingM);
				MDMember* pRebarM = pNewCompoM->GetRebarM();
				((MDRcStairM*)pRebarM)->CopyFromMe(pLandingM->GetRebarM());

				pNewCompoM->m_Name = pLandingM->m_Name;
				pNewCompoM->mp_Owner = pLandingM->mp_Owner;
				pNewCompoM->SetMaterial(pLandingM->GetMaterial());
				pNewCompoM->m_nFromStory = pLandingM->m_nFromStory;
				pNewCompoM->m_nToStory = pLandingM->m_nToStory;
				pNewCompoM->SetInsertedDate(CTime::GetCurrentTime());
				pNewCompoM->SetIsUsedStoryMaterial(pLandingM->GetIsUsedStoryMaterial());
				
				if (pNewCompoM->GetName().MakeUpper() != _T("NONAME"))
				{
					pNewCompoM->GetID();
					pBuild->Add(pNewCompoM);
				}
				pLanding->SetCompoM(pNewCompoM);
			}
		}
	}

	for (int i = 0; i < NumStep; i++)
	{
		if (IsPolyLineError == true)
			break;
		MSStairPart* pStep = pStair->ma_Step[i];
		if (pStep->m_pPolyline->CheckPolygon(false) == FALSE)
			IsPolyLineError = true;
		if (pStep->GetStepsNum() < 3)
			IsStepsNumError = true;
// 		if(pStep->GetStepsNum() * pStep->GetOneStepHeight() > (pStep->m_LevelE - pStep->GetLevelS()))
// 			IsStepsNumError = true;
		if (pStep->GetBottomLevel() > pStep->GetTopLevel())
			IsLevelError = true;

		MSCompoM* pStepM = pStep->GetCompoM();
		if (pStepM->GetType() != MSCompoM::msStairStepM)
		{
			IsStepMError = true;
			MSCompoM* pUpdateStepM = pBuild->GetCompoM(pStepM->GetName(), MSCompoM::msStairStepM);
			if (pUpdateStepM != nullptr)
				pStep->SetCompoM(pUpdateStepM);
			else
			{
				MSCompoM* pNewCompoM = pBuild->CreateNewCompoM(MSCompoM::msStairStepM);
				MDMember* pRebarM = pNewCompoM->GetRebarM();
				((MDRcStairM*)pRebarM)->CopyFromMe(pStepM->GetRebarM());

				pNewCompoM->m_Name = pStepM->m_Name;
				pNewCompoM->mp_Owner = pStepM->mp_Owner;
				pNewCompoM->SetMaterial(pStepM->GetMaterial());
				pNewCompoM->m_nFromStory = pStepM->m_nFromStory;
				pNewCompoM->m_nToStory = pStepM->m_nToStory;
				pNewCompoM->SetInsertedDate(CTime::GetCurrentTime());
				pNewCompoM->SetIsUsedStoryMaterial(pStepM->GetIsUsedStoryMaterial());

				if (pNewCompoM->GetName().MakeUpper() != _T("NONAME"))
				{
					pNewCompoM->GetID();
					pBuild->Add(pNewCompoM);
				}
				pStep->SetCompoM(pNewCompoM);
			}
		}
	}

	if (pStair->GetStairType() == MSGenStairG::StairType_General || pStair->GetStairType() == MSGenStairG::StairType_Double)
	{
		vector<MSStairPart*> PartArr;
		for (int i = 0; i < pStair->ma_Landing.GetSize(); i++)
			PartArr.push_back(pStair->ma_Landing[i]);

		for (int i = 0; i < pStair->ma_Step.GetSize(); i++)
			PartArr.push_back(pStair->ma_Step[i]);

		sort(PartArr.begin(), PartArr.end(),
			[](MSStairPart* a, MSStairPart* b)->bool { return a->GetSequence() < b->GetSequence(); });

		// BA-519 계단 레벨 검토 안하는 것으로 처리(특정 계단 모델링이 안되는 것을 전제로 처리한 모델이 많이 존재한다고함)
		for (int i = 0; i < PartArr.size() - 1; i++)
		{
			if (PartArr[i]->IsConnect(PartArr[i + 1]) == false)
			{
				IsPartLevelError = true;
				break;
			}
		}
	}
	
	CString strGroupName = GetStrGroupName(pStair);

	bool bRval = false;
	if (IsPolyLineError)
	{
		MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"폴리라인 입력이 잘못 되었습니다.", L"StructureModel"), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pStair);
		bRval = true;
	}
	if (IsStepsNumError)
	{
		MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"Step 갯수 입력이 잘못 되었습니다."), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pStair);
		bRval = true;
	}
	if (IsLevelError || IsPartLevelError)
	{
		MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"계단 Step의 Top,Bottom Level 입력이 잘못 되었습니다."), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pStair);
		bRval = true;
	}

	if (IsLandingMError)
	{
		MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"계단 LandingM의 정보가 변경 되었습니다."), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pStair);
		bRval = true;
	}

	if (IsStepMError)
	{
		MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"계단 StepM의 정보가 변경 되었습니다."), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pStair);
		bRval = true;
	}

	if (bRval == true)	return bRval;

	bool bCheck = checkInValidAddRebarArea(pStair, !checkSubElement);
	return checkSubElement ? bCheck : false;
}

bool MSModelChecker::CheckInValidElement(MSBeamG* pBeam, bool checkSubElement)
{
	double dBeamLen = pBeam->GetLength();
	vector<MSBuiltUpBeamG*> BuiltUpArr;
	GM2DPolyline Poly;
	pBeam->GetBoundary(Poly);
	map<long, MSBuiltUpBeamG*>::iterator it;
	for (it = pBeam->mm_BuiltUp.begin(); it != pBeam->mm_BuiltUp.end(); it++)
	{
		MSBuiltUpBeamG* pBuiltUpi = it->second;
		BuiltUpArr.push_back(pBuiltUpi);
	}

	CString strGroupName = GetStrGroupName(pBeam);

	for (int i = 0; i < BuiltUpArr.size(); i++)
	{
		MSBuiltUpBeamG* pBuiltUpi = BuiltUpArr[i];
		if (MSMainBarSet::CheckMainBarSet(pBuiltUpi->m_MainBarSet.GetBarString()) == false)
		{
			MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"덧살보 보강근 갯수에 오류가 있습니다."), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pBeam);
			return true;
		}

		if (pBuiltUpi->GetHeight() < 3)
		{
			MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"덧살보의 높이가 작습니다."), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pBeam);
			return true;
		}

		if (pBuiltUpi->GetLength() < 0)
		{
			MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"덧살보의 길이는 음수가 될수 없습니다."), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pBeam);
			return true;
		}

		double dBuiltUpLen = pBuiltUpi->GetLength();
		bool IsBuiltUpLocError = false;
		if (dBuiltUpLen > dBeamLen + DTOL_MODELING)
			IsBuiltUpLocError = true;

		if (IsBuiltUpLocError == false)
		{
			shared_ptr<GM2DCurve> pCenCurve = pBuiltUpi->GetCenCurve();
			long nRval = Poly.OverlapEx(&*pCenCurve, DTOL_GM, false);
			if (nRval == 0)
				IsBuiltUpLocError = true;
		}
		
		if (IsBuiltUpLocError == true)
		{
			MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"덧살보의 위치가 잘못 되었습니다."), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pBeam);
			return true;
		}
		
		for (int j = i + 1; j < BuiltUpArr.size(); j++)
		{
			MSBuiltUpBeamG* pBuiltUpj = BuiltUpArr[j];
			if (pBuiltUpi == pBuiltUpj)
				continue;
			if (pBuiltUpi->Overlap(pBuiltUpj, DTOL_GM, false))
			{
				MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"덧살보가 오버랩 되었습니다."), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pBeam);
				return true;
			}
		}
	}

	bool bCheck = checkInValidAddRebarArea(pBeam, !checkSubElement);
	return checkSubElement ? bCheck : false;
}

bool MSModelChecker::CheckInValidBeamBuiltUp(MSBeamG* pBeam, MSBuiltUpBeamG* pBuiltUp)
{
	if (pBuiltUp->GetOwnerCompoG() == NULL)
		pBuiltUp->SetOwnerCompoG(pBeam);
	if (pBuiltUp->GetFloor() == NULL)
		pBuiltUp->SetOwner(pBeam->GetFloor());

	CString strGroupName = GetStrGroupName(pBeam);

	GM2DPolyline* pPoly = pBeam->GetTrimmedBoundary();

	shared_ptr<GM2DCurve> pCenCurve = pBuiltUp->GetCenCurve();
	long nRval = pPoly->OverlapEx(&*pCenCurve, DTOL_GM, false);
	if (nRval == 0)
	{
		MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"덧살보의 위치가 잘못 되었습니다."), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pBeam);
		return true;
	}

	map<long, MSBuiltUpBeamG*>::iterator it;
	for (it = pBeam->mm_BuiltUp.begin(); it != pBeam->mm_BuiltUp.end(); it++)
	{
		MSBuiltUpBeamG* pBuiltUpi = it->second;
		if (pBuiltUpi == pBuiltUp || pBuiltUpi->m_ID == pBuiltUp->m_ID)
			continue;
		
		if (pBuiltUpi->Overlap(pBuiltUp, DTOL_GM, false))
		{
			MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"덧살보가 오버랩 되었습니다."), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pBeam);
			return true;
		}
	}

	return false;
}

bool MSModelChecker::CheckInValidElement(MSFWSoundAbsorptionG* pFWSoundAbsorptionG)
{
	MSCompoG* pMainCompoG = pFWSoundAbsorptionG->GetMainCompoG();
	GM2DPolyline* pTargetProfile = pFWSoundAbsorptionG->GetProfile();
	if (pMainCompoG == nullptr || pTargetProfile== nullptr)
		return true;

	CString strGroupName = GetStrGroupName(pFWSoundAbsorptionG);

	GM2DPolyline* pPoly = pMainCompoG->GetProfile();
	if (pPoly->Contains(pTargetProfile) == false)
	{
		MSModelChecker::MakeErrorObject(LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strGroupName + LocalDic::GetLocal(L"흡음재 위치가 잘못 되었습니다."), 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pFWSoundAbsorptionG);
		return true;
	}

	return false;
}

vector<long> MSModelChecker::FindMinMemberProfileArea(MSFloor* pFloor, MSElement::Type_MSCompoG eType, int MinArea)
{
	vector<long> MinSlabIDList;
	map<long, MSCompoG*>::iterator itMem;
	for (itMem = pFloor->mm_CompoG.begin(); itMem != pFloor->mm_CompoG.end(); itMem++)
	{
		MSCompoG* pComopG = itMem->second;
		if (eType == pComopG->GetType())
		{
			MSSlabG* pSlab = (MSSlabG*)pComopG;
			GM2DPolyline* pPoly = pSlab->GetProfile();
			double dSlabArea = pPoly->Area();
			if (MinArea >= dSlabArea)
			{
				MinSlabIDList.push_back(pSlab->m_ID);
				CheckModel_MakeErrObject(pSlab, CMO_GEN, CMO_GEN_SLAB_AREA);
			}
		}
	}

	return MinSlabIDList;
}

bool MSModelChecker::CheckLength(double dValue1, CString sSymbol, double dValue2)
{
	bool sameValue = fabs(dValue1 - dValue2) < DTOL_GM ? true:false;
	CString sReplaceSymbol = sSymbol;
	sReplaceSymbol.Replace(_T(" "), _T(""));

	if(sSymbol == _T("=="))
		return sameValue;
    else if(sSymbol == _T("!="))
		return !sameValue;
    else if(sSymbol == _T(">"))
		return sameValue == false && dValue1 > dValue2;
    else if(sSymbol == _T(">="))
		return sameValue == true || dValue1 >= dValue2;
    else if(sSymbol == _T("<"))
		return sameValue == false && dValue1 < dValue2;
    else if(sSymbol == _T("<="))
		return sameValue == true || dValue1 <= dValue2;                
	else
		ASSERT(0);
	return false;
}

bool MSModelChecker::CheckModel_MSCompoM(MSCompoM* pCompoM)
{
	bool bRval = true;
	if (pCompoM->mp_Owner == nullptr)
		return bRval;

	MSRcFaceMemberM* msRcFaceMemberM = dynamic_cast<MSRcFaceMemberM*>(pCompoM);
	if(msRcFaceMemberM != nullptr)
	{
		
		vector<MSCompoG*> compoGArr;
		pCompoM->mp_Owner->GetAllCompoGsWithCompoM(pCompoM, compoGArr);
		for (int iG = 0; iG < compoGArr.size(); iG++)
		{
			if (compoGArr[iG]->CheckData() == false)
				bRval = false;
		}

		return bRval;
	}

	return bRval;
}

bool MSModelChecker::m_bMakeErrorObject = true;
bool MSModelChecker::m_bMakeOutputReport = true;
bool MSModelChecker::GetIsMakeErrorObject()
{
	return m_bMakeErrorObject;
}

void MSModelChecker::SetIsMakeErrorObject(bool value)
{
	m_bMakeErrorObject = value;
}

bool MSModelChecker::GetIsMakeOutputReport()
{
	return m_bMakeOutputReport;
}

void MSModelChecker::SetIsMakeOutputReport(bool value)
{
	m_bMakeOutputReport = value;
}

CString MSModelChecker::GetStrGroupName(MSElement* pElement)
{
	CString strGroupName = _T("");
	if (pElement->m_ID != 0)
		strGroupName.Format(_T("[%s %s(ID:%d)]: "), pElement->GetFloorNameForUser(), pElement->GetGroupName(), pElement->m_ID);

	return strGroupName;
}