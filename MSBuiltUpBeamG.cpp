
#include "StdAfx.h"
#include "MSBuiltUpBeamG.h"
#include "MSBeamG.h"
#include "GMLib/GM2DArc.h"
#include "MSConnection.h"
#include "MSFloor.h"
#include "MSRebarBuilding.h"
#include "MSStoryMaterial.h"
#include "MSMainBar.h"

IMPLEMENT_SERIAL(MSBuiltUpBeamG, MSElement, VERSIONABLE_SCHEMA | 1)

MSBuiltUpBeamG::MSBuiltUpBeamG(void)
{
	m_IsTop = true;
	m_StartHeight = 500;
	m_EndHeight = 500;

	m_dStartOffSet = 0;
	m_dEndOffSet = 0;
	m_dLength = 0;
	m_StartLocation = 0.0;
	m_EndLocation = 1.0;
	m_IsSameStartLocation = m_IsSameEndLocation = false;

	m_MainBarPlaceType = MSBuiltUpBeamG::MainBarPlace_Type_None;
	m_StirrupBarPlaceInfoType = MSBuiltUpBeamG::StirrupBarPlaceInfo_Type_Owner;

	CString sBar = _T("");
	sBar = MSDeformedBar::GetMainBarStr(4);
	m_MainBarSet.InitData(sBar);
	m_StirrupBar.InitData(MSDeformedBar::GetStirrupBarStr(300, 10));	
	mp_OwnerCompoG = NULL;

	m_nBuiltUpBeamMaterialID = -1;
	m_IsUsedStoryMaterial = false;

	m_SideBarPlaceType = MSBuiltUpBeamG::MainBarPlace_Type_None;

	m_bIsNumPlaceSideBar = TRUE;
	sBar = MSDeformedBar::GetMainBarStr(2);
	m_SideBars.InitData(sBar);
	sBar = MSDeformedBar::GetStirrupBarStr(200);
	m_SideStirrupBar.InitData(sBar);

	m_StirrupBarPlaceType = StirrupBarPlaceType_Auto;
}

MSBuiltUpBeamG::~MSBuiltUpBeamG(void)
{
	//beam의 Connection을 사용중이므로 Delete 하지 않는다.
	ma_Connections.clear();
}

MSObject* MSBuiltUpBeamG::Copy(bool bNewID /* = true*/)
{
	MSBuiltUpBeamG* pNewObj;

	pNewObj = new MSBuiltUpBeamG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSBuiltUpBeamG::CopyFromMe(MSBuiltUpBeamG* pSource, bool bNewID /* = true*/)
{
	MSLinkMemberG::CopyFromMe(pSource, bNewID);

	if(pSource)
	{
		m_IsTop = pSource->m_IsTop;
		m_StartHeight = pSource->m_StartHeight;
		m_EndHeight = pSource->m_EndHeight;

		m_StartLocation = pSource->m_StartLocation;
		m_EndLocation = pSource->m_EndLocation;
		m_IsSameStartLocation = pSource->m_IsSameStartLocation;
		m_IsSameEndLocation = pSource->m_IsSameEndLocation;

		m_MainBarPlaceType = pSource->m_MainBarPlaceType;
		m_StirrupBarPlaceInfoType = pSource->m_StirrupBarPlaceInfoType;

		m_MainBarSet.CopyFromMe(&pSource->m_MainBarSet);
		m_StirrupBar.CopyFromMe(&pSource->m_StirrupBar);

		m_dStartOffSet = pSource->m_dStartOffSet;
		m_dEndOffSet = pSource->m_dEndOffSet;
		m_dLength = pSource->m_dLength;

		m_nBuiltUpBeamMaterialID = pSource->m_nBuiltUpBeamMaterialID;
		m_IsUsedStoryMaterial = pSource->m_IsUsedStoryMaterial;

		m_SideBarPlaceType = pSource->m_SideBarPlaceType;
		m_bIsNumPlaceSideBar = pSource->m_bIsNumPlaceSideBar;
		m_SideBars.CopyFromMe(&pSource->m_SideBars);
		m_SideStirrupBar.CopyFromMe(&pSource->m_SideStirrupBar);

		m_StirrupBarPlaceType = pSource->GetStirrupBarPlaceType();
	}
}

void MSBuiltUpBeamG::Serialize(CArchive &ar)
{
	//BuiltUpbeam의 Connection 은 Beam에서 사용중인 Connection을 사용하고 있으므로 저장하지 않는다.
	//파일 오픈시 Beam::RecoverPointerAfterSerialize 에서 Connection을 셋팅해준다.
	ma_Connections.clear();
	MSLinkMemberG::Serialize(ar);
	for (int i = 0; i < ma_Connections.size(); i++)
		delete ma_Connections[i];
	ma_Connections.clear();

	int mainPlaceType = 0;
	int sidePlaceType = 0;
	int stirrupPlaceInfoType = 0;
	int stirrupPlaceType = 0;
	MSMainBar mainBars;
	if(ar.IsStoring())
	{
		ar << m_IsTop;
		ar << m_StartHeight;
		ar << m_EndHeight;

		ar << m_StartLocation;
		ar << m_EndLocation;

		mainPlaceType = (int)m_MainBarPlaceType;
		stirrupPlaceInfoType = (int)m_StirrupBarPlaceInfoType;

		ar << mainPlaceType;
		ar << stirrupPlaceInfoType;

		mainBars.Serialize(ar);
		m_StirrupBar.Serialize(ar);

		ar << m_nBuiltUpBeamMaterialID;
		ar << m_IsUsedStoryMaterial;

		m_MainBarSet.Serialize(ar);

		sidePlaceType = (int)m_SideBarPlaceType;
		ar << sidePlaceType;
		ar << m_bIsNumPlaceSideBar;
		m_SideBars.Serialize(ar);
		m_SideStirrupBar.Serialize(ar);

		stirrupPlaceType = m_StirrupBarPlaceType;
		ar << stirrupPlaceType;
	}
	else
	{
		ar >> m_IsTop;
		ar >> m_StartHeight;
		ar >> m_EndHeight;

		ar >> m_StartLocation;
		ar >> m_EndLocation;

		ar >> mainPlaceType;
		ar >> stirrupPlaceInfoType;

		m_MainBarPlaceType = (eMainBarPlace_Type)mainPlaceType;
		m_StirrupBarPlaceInfoType = (eStirrupBarPlaceInfo_Type)stirrupPlaceInfoType;

		mainBars.Serialize(ar);
		m_StirrupBar.Serialize(ar);

		SetStartLocation(m_StartLocation);
		SetEndLocation(m_EndLocation);

		ar >> m_nBuiltUpBeamMaterialID;
		ar >> m_IsUsedStoryMaterial;

		if (MSVersionInfo::GetCurrentVersion() >= 20160822)
			m_MainBarSet.Serialize(ar);
		else
			m_MainBarSet.SetDataByOldVersion(&mainBars);

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20180305)
		{
			ar >> sidePlaceType;
			m_SideBarPlaceType = (eMainBarPlace_Type)sidePlaceType;
			ar >> m_bIsNumPlaceSideBar;
			m_SideBars.Serialize(ar);
			m_SideStirrupBar.Serialize(ar);
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20230503)
		{
			ar >> stirrupPlaceType;
			m_StirrupBarPlaceType = (eStirrupBarPlaceType)stirrupPlaceType;
		}
	}
}

void MSBuiltUpBeamG::Dump(EFS::EFS_Buffer& buffer)
{
	MSLinkMemberG::Dump(buffer);
	buffer << m_IsTop << m_StartHeight << m_EndHeight << m_StartLocation << m_EndLocation;
	buffer << m_IsSameStartLocation << m_IsSameEndLocation;
		
	int mainPlaceType = 0;
	int stirrupPlaceInfoType = 0;

	mainPlaceType = (int)m_MainBarPlaceType;
	stirrupPlaceInfoType = (int)m_StirrupBarPlaceInfoType;

	buffer << mainPlaceType << stirrupPlaceInfoType;

	buffer << m_dLength;
	buffer << m_dStartOffSet;
	buffer << m_dEndOffSet;

	m_MainBarSet.Dump(buffer);
	m_StirrupBar.Dump(buffer);

	buffer << m_nBuiltUpBeamMaterialID;
	buffer << m_IsUsedStoryMaterial;
	buffer << mp_OwnerCompoG;

	int sidePlaceType = 0;
	sidePlaceType = (int)m_SideBarPlaceType;
	buffer << sidePlaceType;
	buffer << m_bIsNumPlaceSideBar;
	m_SideBars.Dump(buffer);
	m_SideStirrupBar.Dump(buffer);

	int stirrupPlaceType = 0;
	stirrupPlaceType = (int)m_StirrupBarPlaceType;
}

void MSBuiltUpBeamG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSLinkMemberG::Recovery(buffer);
	buffer >> m_IsTop >> m_StartHeight >> m_EndHeight >> m_StartLocation >> m_EndLocation;
	buffer >> m_IsSameStartLocation >> m_IsSameEndLocation;

	int mainPlaceType = 0;
	int stirrupPlaceInfoType = 0;

	buffer >> mainPlaceType >> stirrupPlaceInfoType;
	m_MainBarPlaceType = (eMainBarPlace_Type)mainPlaceType;
	m_StirrupBarPlaceInfoType = (eStirrupBarPlaceInfo_Type)stirrupPlaceInfoType;

	buffer >> m_dLength;
	buffer >> m_dStartOffSet;
	buffer >> m_dEndOffSet;

	m_MainBarSet.Recovery(buffer);
	m_StirrupBar.Recovery(buffer);

	buffer >> m_nBuiltUpBeamMaterialID;
	buffer >> m_IsUsedStoryMaterial;
	buffer >> mp_OwnerCompoG;

	int sidePlaceType = 0;
	buffer >> sidePlaceType;
	m_SideBarPlaceType = (eMainBarPlace_Type)sidePlaceType;
	buffer >> m_bIsNumPlaceSideBar;
	m_SideBars.Recovery(buffer);
	m_SideStirrupBar.Recovery(buffer);

	int stirrupPlaceType = 0;
	buffer >> stirrupPlaceType;
	m_StirrupBarPlaceType = (eStirrupBarPlaceType)stirrupPlaceType;
}

double MSBuiltUpBeamG::GetWidth()
{
	MSBeamG* pBeamG = (MSBeamG*)GetOwnerCompoG();
	if (pBeamG == NULL)
		return 0;

	return pBeamG->GetWidth();
}

double MSBuiltUpBeamG::GetDepth()
{
	return m_StartHeight;
}

double MSBuiltUpBeamG::GetThick()
{
	return m_StartHeight;
}

double MSBuiltUpBeamG::GetLength()
{
	return m_dLength;
}

void MSBuiltUpBeamG::SetLines()
{
	MSBeamG* pBeamG = (MSBeamG*)GetOwnerCompoG();
	if (pBeamG == NULL)
		return;
	shared_ptr<GM2DCurve> pCenCurve = pBeamG->GetCenCurve();

	GM2DVector* pSVec = GetSVec();
	GM2DVector* pEVec = GetEVec();
	if (pSVec == NULL || pEVec == NULL)
		return;

	GM2DVector SVec = pCenCurve->OrthoBaseVector(*pSVec);
	GM2DVector EVec = pCenCurve->OrthoBaseVector(*pEVec);
	pSVec->SetXY(SVec.m_X, SVec.m_Y);
	pEVec->SetXY(EVec.m_X, EVec.m_Y);

	MSLinkMemberG::SetLines();
	SetOffSetInfo();

}

void MSBuiltUpBeamG::SetOffSetInfo()
{
	MSBeamG* pBeamG = (MSBeamG*)GetOwnerCompoG();
	if (pBeamG == NULL)
		return;

	shared_ptr<GM2DCurve> pCenCurve = pBeamG->GetCenCurve();

	m_dLength = MSLinkMemberG::GetLength();
	m_dStartOffSet = ma_Vec[0]->Distance(pCenCurve->m_P0);
	m_dEndOffSet = ma_Vec[1]->Distance(pCenCurve->m_P1);
	if (isArc())
	{
		GM2DArc* pMyArc = (GM2DArc*)mp_Curve;
		GM2DArc* pBeamArc = (GM2DArc*)(&*pCenCurve);
		GM2DArc TheStartArc(pBeamArc->m_A, pBeamArc->m_Center.m_X, pBeamArc->m_Center.m_Y, pBeamArc->m_Theta1, pMyArc->m_Theta1);
		m_dStartOffSet = TheStartArc.Length();

		GM2DArc TheEndArc(pBeamArc->m_A, pBeamArc->m_Center.m_X, pBeamArc->m_Center.m_Y, pMyArc->m_Theta2, pBeamArc->m_Theta2);
		m_dEndOffSet = TheEndArc.Length();
	}
}

void MSBuiltUpBeamG::UpdateBoundaryVisibilities(long nUpdateConnIdx /*= 0*/)
{
	long nOverlap = this->ma_Connections.size();
	for (long iOverlap = nUpdateConnIdx; iOverlap < nOverlap; iOverlap++)
	{
		MSConnection* pConnection = this->ma_Connections[iOverlap];
		MSCompoG* pPartnerG = pConnection->GetPartner(this);
		if (pPartnerG == NULL) continue;
		if (pPartnerG->IsPolyMember()) continue;

		if (pPartnerG->IsFloorMember() == true &&
			pPartnerG->IsLevelOverlap(mp_OwnerCompoG) == false)
			continue;

		GM2DPolyline* pMyBoundary = this->GetTrimmedBoundary();
		GM2DPolyline* pUrBoundary = pPartnerG->GetTrimmedBoundary();
		double dTol = DTOL_GM;

		pMyBoundary->SegmentstoMe(pUrBoundary, dTol);
	}
	InitTrimmedBoundaryVisibility();

	for (long iOverlap = nUpdateConnIdx; iOverlap < nOverlap; iOverlap++)
	{
		MSConnection* pConnection = this->ma_Connections[iOverlap];
		MSCompoG* pPartnerG = pConnection->GetPartner(this);
		if (pPartnerG == NULL) continue;
		if (pPartnerG->IsPolyMember()) continue;

		if (pPartnerG->IsFloorMember() == true &&
			pPartnerG->IsLevelOverlap(mp_OwnerCompoG) == false)
			continue;

		MSLinkMemberG::UpdateBoundaryVisibilities(pPartnerG, pConnection);
	}
	
	MSCompoG::UpdateBoundaryVisibilities(nUpdateConnIdx);
}

void MSBuiltUpBeamG::SyncStartLocationVec2Loc()
{
	MSBeamG* pBeamG = (MSBeamG*)GetOwnerCompoG();
	if (pBeamG == NULL)
		return;
	
	shared_ptr<GM2DCurve> pCenCurve = pBeamG->GetCenCurve();
	GM2DVector* pSVec = GetSVec();
	double dSLoc = pCenCurve->OrthoBaseLocValue(*pSVec);
	if (dSLoc <= 0 || m_IsSameStartLocation)
	{
		pSVec->SetXY(pCenCurve->m_P0.m_X, pCenCurve->m_P0.m_Y);
		if (isArc())
			mp_Curve->SetP0(*pSVec);
	}
	
	SetStartLocation(dSLoc);
}

void MSBuiltUpBeamG::SyncEndLocationVec2Loc()
{
	MSBeamG* pBeamG = (MSBeamG*)GetOwnerCompoG();
	if (pBeamG == NULL)
		return;

	shared_ptr<GM2DCurve> pCenCurve = pBeamG->GetCenCurve();
	GM2DVector* pEVec = GetEVec();
	double dELoc = pCenCurve->OrthoBaseLocValue(*pEVec);
	if (dELoc >= 1 || m_IsSameEndLocation)
	{
		pEVec->SetXY(pCenCurve->m_P1.m_X, pCenCurve->m_P1.m_Y);
		if (isArc())
			mp_Curve->SetP1(*pEVec);
	}

	SetEndLocation(dELoc);
}

void MSBuiltUpBeamG::SyncStartLocationLoc2Vec()
{
 	MSBeamG* pBeamG = (MSBeamG*)GetOwnerCompoG();
	if (pBeamG == NULL)
		return;
	shared_ptr<GM2DCurve> pCenCurve = pBeamG->GetCenCurve();
	double dLoc = pCenCurve->OrthoBaseLocValue(*ma_Vec[0]);
// 	if (dLoc < 0 || dLoc > 1) // 보위에 빌트업 보가 없는 경우 싱크하지 않는다.
// 		return;

	double dSLoc = GetStartLocation();
	GM2DVector SVec = pCenCurve->PointAtParam(dSLoc);
	
	if (ma_Vec.size() > 0)
	{
		ma_Vec[0]->SetXY(SVec.m_X, SVec.m_Y);
		if (isArc())
		{
			GM2DArc* pBeamArc = (GM2DArc*)&*pCenCurve;
			GM2DArc* pCurArc = (GM2DArc*)mp_Curve;
			pCurArc->m_Center = pBeamArc->m_Center;
			pCurArc->SetP0(SVec);
		}
	}
}

void MSBuiltUpBeamG::SyncEndLocationLoc2Vec()
{
	MSBeamG* pBeamG = (MSBeamG*)GetOwnerCompoG();
	if (pBeamG == NULL)
		return;
	shared_ptr<GM2DCurve> pCenCurve = pBeamG->GetCenCurve();
	double dLoc = pCenCurve->OrthoBaseLocValue(*ma_Vec[1]);
// 	if (dLoc < 0 || dLoc > 1) // 보위에 빌트업 보가 없는 경우 싱크하지 않는다.
// 		return;

	double dELoc = GetEndLocation();
	GM2DVector EVec = pCenCurve->PointAtParam(dELoc);

	if (ma_Vec.size() > 1)
	{
		ma_Vec[1]->SetXY(EVec.m_X, EVec.m_Y);
		if (isArc())
		{
			GM2DArc* pBeamArc = (GM2DArc*)&*pCenCurve;
			GM2DArc* pCurArc = (GM2DArc*)mp_Curve;
			pCurArc->m_Center = pBeamArc->m_Center;
			pCurArc->SetP1(EVec);
		}
	}
}

void MSBuiltUpBeamG::SyncLocationVec2Loc()
{
	MSBeamG* pBeamG = (MSBeamG*)GetOwnerCompoG();
	shared_ptr<GM2DCurve> pBeamCenCurve = pBeamG->GetCenCurve();
	shared_ptr<GM2DCurve> pCenCurve = GetCenCurve();

	int nRVal = pCenCurve->IsColinear4DTol(&*pBeamCenCurve);
	if (nRVal == -1)
	{
		GM2DVector* pVec = ma_Vec[0];
		ma_Vec[0] = ma_Vec[1];
		ma_Vec[1] = pVec;
	}

	if (nRVal == 0)
	{
		shared_ptr<GM2DCurve> pCenCurve = pBeamG->GetCenCurve();

		if (pCenCurve->m_ObjType == GT_LINE)
		{
			GM2DLineVector* pLine = (GM2DLineVector*)&*pCenCurve;
			GM2DVector UnitVec = pLine->UnitVector();
			GM2DVector TmpSVec;
			GM2DVector TmpEVec;
			if (m_IsSameStartLocation && m_IsSameEndLocation)
			{
				TmpSVec = pLine->m_P0;
				TmpEVec = pLine->m_P1;
			}
			else if (m_IsSameStartLocation)
			{
				TmpSVec = pLine->m_P0;
				TmpEVec = pLine->m_P1 + (UnitVec * -m_dEndOffSet);
			}
			else if (m_IsSameEndLocation)
			{
				TmpSVec = pLine->m_P0 + (UnitVec * m_dStartOffSet);
				TmpEVec = pLine->m_P1;
			}
			else
			{
				TmpSVec = pLine->m_P0 + (UnitVec * m_dStartOffSet);
				TmpEVec = pLine->m_P1 + (UnitVec * -m_dEndOffSet);
			}
			
			if (m_IsSameStartLocation)
				SyncStartLocationVec2Loc();
			else
				ma_Vec[0]->SetXY(TmpSVec.m_X, TmpSVec.m_Y);
			
			if (m_IsSameEndLocation)
				SyncEndLocationVec2Loc();
			else
				ma_Vec[1]->SetXY(TmpEVec.m_X, TmpEVec.m_Y);
		}
		else
		{
			GM2DArc* pArc = (GM2DArc*)&*pCenCurve;
			double dArcLen = pArc->Length();
			double dBuge = pArc->GetBulge();

			double dSRad = pArc->m_Theta1;
			double dERad = pArc->m_Theta2;

			GM2DArc* pMyArc = (GM2DArc*)mp_Curve;
			double dMyArcLen = pMyArc->Length();
			
			pMyArc->m_A = pMyArc->m_B = pArc->m_A;
			pMyArc->m_Center = pArc->m_Center;

			if (m_dLength > dArcLen)
			{
				m_dLength = dArcLen;
				m_IsSameStartLocation = m_IsSameEndLocation = true;
			}
			if (m_dLength + m_dStartOffSet > dArcLen)
				m_dStartOffSet = 0;

			GM2DVector TmpSVec;
			GM2DVector TmpEVec;
			if (m_IsSameStartLocation && m_IsSameEndLocation)
			{
				TmpSVec = pCenCurve->m_P0;
				TmpEVec = pCenCurve->m_P1;

				pMyArc->CopyFrom(pArc);
			}
			else if (m_IsSameStartLocation)
			{
				pMyArc->m_Theta1 = pArc->m_Theta1;
				pMyArc->m_Theta2 = pArc->Angle2DVector(m_dLength / dArcLen);

				TmpSVec = pCenCurve->m_P0;
				TmpEVec = pMyArc->GetEnd2DVec();
			}
			else if (m_IsSameEndLocation)
			{
				pMyArc->m_Theta1 = pArc->Angle2DVector((dArcLen - m_dLength) / dArcLen);
				pMyArc->m_Theta2 = pArc->m_Theta2;

				TmpSVec = pMyArc->GetStart2DVec(); 
				TmpEVec = pCenCurve->m_P1;
			}
			else
			{
				if ((m_StartLocation + m_EndLocation) / 2 < 0.5)
				{
					pMyArc->m_Theta1 = pArc->Angle2DVector(m_dStartOffSet / dArcLen);
					pMyArc->m_Theta2 = pArc->Angle2DVector((m_dStartOffSet + m_dLength) / dArcLen);
				}
				else
				{
					pMyArc->m_Theta1 = pArc->Angle2DVector((dArcLen - (m_dEndOffSet + m_dLength)) / dArcLen);
					pMyArc->m_Theta2 = pArc->Angle2DVector((dArcLen - m_dEndOffSet) / dArcLen);
				}

				TmpSVec = pMyArc->GetStart2DVec();
				TmpEVec = pMyArc->GetEnd2DVec();
			}
			
			if (m_IsSameStartLocation)
				SyncStartLocationVec2Loc();
			else
				ma_Vec[0]->SetXY(TmpSVec.m_X, TmpSVec.m_Y);
			
			if (m_IsSameEndLocation)
				SyncEndLocationVec2Loc();
			else
				ma_Vec[1]->SetXY(TmpEVec.m_X, TmpEVec.m_Y);
		}

		double dSLoc = pCenCurve->OrthoBaseLocValue(*GetSVec());
		double dELoc = pCenCurve->OrthoBaseLocValue(*GetEVec());
		
		SetStartLocation(dSLoc);
		SetEndLocation(dELoc);
	}
	else
	{
		SyncStartLocationVec2Loc();
		SyncEndLocationVec2Loc();
	}
	
	SetLines();
}

void MSBuiltUpBeamG::SyncLocationLoc2Vec()
{
	MSBeamG* pBeamG = (MSBeamG*)GetOwnerCompoG();
	shared_ptr<GM2DCurve> pBeamCenCurve = pBeamG->GetCenCurve();
	shared_ptr<GM2DCurve> pCenCurve = GetCenCurve();
	if (pBeamCenCurve->IsParallel4DTol(&*pCenCurve) == -1)
	{
		double dTmpLoc = m_StartLocation;
		m_StartLocation = m_EndLocation;
		m_EndLocation = dTmpLoc;
	}
	SyncStartLocationLoc2Vec();
	SyncEndLocationLoc2Vec();
	SetLines();
}

bool MSBuiltUpBeamG::IsContainLoc(double dLoc, double dTol)
{
	if (m_StartLocation < m_EndLocation)
	{
		if (m_StartLocation - dTol <= dLoc &&
			m_EndLocation + dTol >= dLoc)
			return true;
	}
	else
	{
		if (m_EndLocation - dTol <= dLoc &&
			m_StartLocation + dTol >= dLoc)
			return true;
	}
	return false;
}

void MSBuiltUpBeamG::SetStartLocation(double loc)
{ 
	if (loc <= 0)
	{
		loc = 0;
		m_IsSameStartLocation = true;
	}
	else 
		m_IsSameStartLocation = false;
	m_StartLocation = loc;
};
void MSBuiltUpBeamG::SetEndLocation(double loc)
{
	if (loc >= 1)
	{
		loc = 1;
		m_IsSameEndLocation = true;
	}
	else
		m_IsSameEndLocation = false;
	m_EndLocation = loc; 
};

void MSBuiltUpBeamG::SetIsSameStartLocation(bool bValue)
{
	m_IsSameStartLocation = bValue;
}
void MSBuiltUpBeamG::SetIsSameEndLocation(bool bValue)
{
	m_IsSameEndLocation = bValue;
}

bool MSBuiltUpBeamG::GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal, bool bCheckLevelZone)
{
	double dBeamTopLevel = 0, dBeamBotLevel = 0;
	//mp_OwnerCompoG->GetZLevelsbyLoc(dBeamTopLevel, dBeamBotLevel, (m_StartLocation + m_EndLocation)/2);
	mp_OwnerCompoG->GetZLevels(dBeamTopLevel, dBeamBotLevel, true, false);
	double dZLevel = GetFloor()->GetLevel4LevelZone(this);
	if (m_IsTop)
	{
		TopLevel = dBeamTopLevel + dZLevel;
		BottomLevel = dBeamTopLevel + dZLevel - GetDepth();
	}
	else
	{
		TopLevel = dBeamBotLevel + dZLevel + GetDepth();
		BottomLevel = dBeamBotLevel + dZLevel;
	}
	return true;
}

bool MSBuiltUpBeamG::GetZLevelsbyLoc(double& TopLevel, double& BottomLevel, double dLoc)
{
	double dBeamTopLevel = 0, dBeamBotLevel = 0;
	mp_OwnerCompoG->GetZLevelsbyLoc(dBeamTopLevel, dBeamBotLevel, dLoc);

	if (m_IsTop)
	{
		TopLevel = dBeamTopLevel;
		BottomLevel = dBeamTopLevel - GetDepth();
	}
	else
	{
		TopLevel = dBeamBotLevel + GetDepth();
		BottomLevel = dBeamBotLevel;
	}
	return true;
}

CString MSBuiltUpBeamG::GetMaterialName()
{
	CString sName;

	if (m_nBuiltUpBeamMaterialID == -1)
	{
		MSCompoM* pCompoM = mp_OwnerCompoG->GetCompoM();
		m_IsUsedStoryMaterial = pCompoM->GetIsUsedStoryMaterial();
		m_nBuiltUpBeamMaterialID = pCompoM->GetMaterial()->GetID();
	}

	if (m_IsUsedStoryMaterial)
		sName = USED_STORY_MATERIAL;
	else
	{
		MSMaterial* matl = GetMaterial();
		sName = matl->m_Name;
	}

	return sName;
}

void MSBuiltUpBeamG::SetMaterialName(MSMaterial::Type_MSMaterial materialType, CString sName)
{
	if (sName == USED_STORY_MATERIAL)
		m_IsUsedStoryMaterial = true;
	else
		m_IsUsedStoryMaterial = false;

	MSBaseBuilding* bldg = mp_OwnerCompoG->GetBuilding();
	MSMaterial* matl = bldg->GetMaterialbyName(materialType, sName);

	if (matl != NULL)
		m_nBuiltUpBeamMaterialID = matl->m_ID;
}

MSMaterial* MSBuiltUpBeamG::GetMaterial()
{
	MSBaseBuilding* bldg = mp_OwnerCompoG->GetBuilding();
	long nMaterialID = GetBuiltUpBeamMaterialID();
	MSMaterial* matl = bldg->GetMaterial(nMaterialID);

	if (matl == NULL)
		matl = mp_OwnerCompoG->GetMaterial();

	return matl;
}

long MSBuiltUpBeamG::GetBuiltUpBeamMaterialID()
{
	MSBaseBuilding* pBldg = nullptr;
	MSFloor* pFloor = GetFloor();
	if (pFloor != nullptr)
	{
		pBldg = pFloor->GetBuilding();
		if (pBldg != nullptr)
		{
			MSMaterial* pMaterial = pBldg->GetMaterial(m_nBuiltUpBeamMaterialID);
			if (pMaterial == nullptr)
				m_nBuiltUpBeamMaterialID = -1;
		}
	}

	if (m_nBuiltUpBeamMaterialID == -1)
	{
		MSCompoM* pCompoM = mp_OwnerCompoG->GetCompoM();
		m_IsUsedStoryMaterial = pCompoM->GetIsUsedStoryMaterial();
		
		MSMaterial* pMaterial = pCompoM->GetMaterial();
		if(pMaterial == nullptr && pBldg != nullptr)
			pMaterial = pBldg->GetDefMaterial();

		if (pMaterial != nullptr)
			m_nBuiltUpBeamMaterialID = pMaterial->GetID();
	}

	if (m_IsUsedStoryMaterial)
	{
		if(pFloor != nullptr && pBldg != nullptr)
		{
			MSStoryMaterial* pStoryMaterial = pBldg->GetStoryMaterialByFloorNum(pFloor->GetFloorNum());
			if (pStoryMaterial == nullptr)
			{
				ASSERT(0);
				return m_nBuiltUpBeamMaterialID;
			}

			MSMaterial* pMaterial = pStoryMaterial->GetMaterial(MSMaterial::msConcrete, false);
			if (pMaterial)	
				return pMaterial->m_ID;
		}
		ASSERT(0);
	}
	return m_nBuiltUpBeamMaterialID;
}

void MSBuiltUpBeamG::SetLength(double dLen)
{
	if (dLen < 0)
	{
		m_dLength = dLen;
		return;
	}
	MSBeamG* pBeamG = (MSBeamG*)GetOwnerCompoG();
	shared_ptr<GM2DCurve> pBeamCenCurve = pBeamG->GetCenCurve();
	double dBeamLen = pBeamG->GetLength();
	if (fabs(dBeamLen - dLen) < DTOL_MODELING)
		dLen = dBeamLen;

	shared_ptr<GM2DCurve> pCenCurve = GetCenCurve();
	double dCurLen = pCenCurve->Length();
	double dExtendLen = dCurLen - dLen;
	if (dExtendLen == 0)
		return;

	if (dCurLen > dLen)
		pCenCurve->ExtendLineVector(FALSE, dExtendLen, FALSE);
	else
	{
		pCenCurve->ExtendLineVector(FALSE, dExtendLen, FALSE);
		double dELoc = pBeamCenCurve->OrthoBaseLocValue(pCenCurve->m_P1);

		if (dELoc > 1)
		{
			pCenCurve->SetP1(pBeamCenCurve->m_P1);
			double dNewLen = pCenCurve->Length();
			double dNewExtendLen = dLen - dNewLen;
			pCenCurve->ExtendLineVector(TRUE, dNewExtendLen, FALSE);
		}
	}
	SetCenCurve(pCenCurve->Copy());
	SetLines();
}

bool MSBuiltUpBeamG::IsOnBuiltUpBeamPnt(GM2DVector& Vec)
{
	GM2DCurve* pCenCurve = GetLongLVec(false);
	if (pCenCurve == nullptr)	return false;
	double dLoc = pCenCurve->OrthoBaseLocValue(Vec);
	if (dLoc > DTOL_GM && dLoc < 1 - DTOL_GM)
		return true;

	return false;
}
long MSBuiltUpBeamG::CalcStirrupBarPlaceType()
{
	if (m_StirrupBarPlaceType == StirrupBarPlaceType_Auto)
	{
		if (this->GetDepth() > 150)
			return 1;
		else
			return 0;
	}
	return m_StirrupBarPlaceType == StirrupBarPlaceType_Type1 ? 0 : 1;
}