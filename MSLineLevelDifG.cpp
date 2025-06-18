#include "StdAfx.h"
#include "MSLineLevelDifG.h"
#include "MSRebarBuilding.h"
#include "MSSlabG.h"
#include "MSLevelZone.h"
#include "MSLevelDifHaunch.h"
#include "GMLib/GM2DCurve.h"
#include "MSFloor.h"
#include "MSPolylineObject.h"
#include "MSOpening.h"
#include "GMLib/MSVersionInfo.h"
#include "MSConnection.h"
#include "MSStoryMaterial.h"

//#include "MSLineMemberM.h"

IMPLEMENT_SERIAL(MSLineLevelDifG, MSLinkMemberG,  VERSIONABLE_SCHEMA | 1)

MSLineLevelDifG::MSLineLevelDifG(MSLevelZone* pZone)
{
	mp_LeftSlab = NULL;
	mp_RightSlab = NULL;
	mp_LeftLevelZone = pZone;
	mp_RightLevelZone = NULL;
	m_InfillBeamFactor = 1.5;
	m_InfillSlabFactor = 1.5;
    mp_Curve = NULL;
	mp_LLine = NULL;
	mp_RLine = NULL;
	mp_SlabHaunch = NULL;
	mp_BeamHaunch = NULL;
	m_nLiveCnt = 0;
	m_nLeftLevelZoneID = 0;
	m_nRightLevelZoneID = 0;
	m_nLeftSlabID = 0;
	m_nRightSlabID = 0;
	m_dWallWidth = 0.0;
	if(pZone != NULL)
		m_dWallWidth = pZone->m_Width;
	m_bHasHaunch = false;
	m_bHasWall = true;
	m_dHaunchX = 0.0;
	m_dHaunchY = 0.0;
	m_bUserLevelDif = false;
	m_dWallHeight = 0.0;
	m_dHaunchAngle = 45;
	m_bUseHaunchMaterial = false;
	m_bUseBackFillFormwork = true;
	m_nHaunchMaterialID = 0;
	m_IsUsedStoryMaterial = true;

	CString sStirrupBar = MSDeformedBar::GetStirrupBarStr(300, 10);
	CString sMainBar = MSDeformedBar::GetMainBarStr(4, 10);
    m_nPlaceType = 0;
    m_IsNumPlaceBarVExt = false;
    m_SBarVExt.InitData(sStirrupBar);
    m_MainBarVExt = sMainBar;
    m_IsNumPlaceBarVInt = false;
    m_SBarVInt.InitData(sStirrupBar);
    m_MainBarVInt = sMainBar;
    m_IsNumPlaceBarHExt = false;
    m_SBarHExt.InitData(sStirrupBar);
    m_MainBarHExt = sMainBar;
    m_IsNumPlaceBarHInt = false;
    m_SBarHInt.InitData(sStirrupBar);
    m_MainBarHInt = sMainBar;

	if(pZone != NULL)
		SetOwner(pZone->GetOwner());
	Init(true);
}

MSLineLevelDifG::~MSLineLevelDifG(void)
{
	if(mp_SlabHaunch)
		delete mp_SlabHaunch;
	if(mp_BeamHaunch)
		delete mp_BeamHaunch;
	mp_SlabHaunch = NULL;
	mp_BeamHaunch = NULL;

	long NumConnection = ma_Connections.size();
	for (long iConn = NumConnection - 1; iConn >= 0; iConn--)
	{
		MSConnection* pConnection = ma_Connections[iConn];
		pConnection->SetDeleted(true);
	}

	ma_Connections.clear();

	//RemoveConnections() 함수는 InitTrimmedBoundary() 호출 해서 문제가 발생함
	//RemoveConnections();
	//DeleteSlabLineLevelDif();
}

bool MSLineLevelDifG::isDelete()
{
	m_nLiveCnt--;
	if(m_nLiveCnt > 0)
	{
		
		return false;
	}
	return true;
	//delete this;
}

void MSLineLevelDifG::Dump(EFS::EFS_Buffer& buffer)
{
	MSLinkMemberG::Dump(buffer);
	buffer << &mp_LeftLevelZone << &mp_RightLevelZone << &mp_LeftSlab << & mp_RightSlab;
	buffer << m_dWallWidth;
	buffer << m_bHasHaunch;
	buffer << m_bHasWall;
	buffer << m_dHaunchX;
	buffer << m_dHaunchY;
	buffer << m_bUserLevelDif;
	buffer << m_dWallHeight;
	buffer << m_dHaunchAngle;
	buffer << m_bUseHaunchMaterial;
	buffer << m_nHaunchMaterialID;

	long NumLinkMemberGID = ma_LinkMemberGID.size();
	buffer << NumLinkMemberGID;
	for(long i = 0 ; i < NumLinkMemberGID ; i++)
		buffer << ma_LinkMemberGID[i];

	buffer << m_bUseBackFillFormwork;
	buffer << m_IsUsedStoryMaterial;
//	buffer << &m_TopLine << &m_BotLine << m_TopCompoGID << m_BotCompoGID << m_dOffset;

    buffer << m_nPlaceType;
    buffer << m_IsNumPlaceBarVExt;
    m_SBarVExt.Dump(buffer);
    m_MainBarVExt.Dump(buffer);
    buffer << m_IsNumPlaceBarVInt;
    m_SBarVInt.Dump(buffer);
    m_MainBarVInt.Dump(buffer);
    buffer << m_IsNumPlaceBarHExt;
    m_SBarHExt.Dump(buffer);
    m_MainBarHExt.Dump(buffer);
    buffer << m_IsNumPlaceBarHInt;
    m_SBarHInt.Dump(buffer);
    m_MainBarHInt.Dump(buffer);
}

void MSLineLevelDifG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSLinkMemberG::Recovery(buffer);
	buffer >> &mp_LeftLevelZone >> &mp_RightLevelZone >> &mp_LeftSlab >> & mp_RightSlab;
	buffer >> m_dWallWidth;
	buffer >> m_bHasHaunch;
	buffer >> m_bHasWall;
	buffer >> m_dHaunchX;
	buffer >> m_dHaunchY;
	buffer >> m_bUserLevelDif;
	buffer >> m_dWallHeight;
	buffer >> m_dHaunchAngle;
	buffer >> m_bUseHaunchMaterial;
	buffer >> m_nHaunchMaterialID;

	long NumLinkMemberGID = 0;
	buffer >> NumLinkMemberGID;
	for(long i = 0 ; i < NumLinkMemberGID ; i++)
	{
		long GID = 0;
		buffer >> GID;
		ma_LinkMemberGID.push_back(GID);
	}

	buffer >> m_bUseBackFillFormwork;
	buffer >> m_IsUsedStoryMaterial;
//	buffer >> &m_TopLine >> &m_BotLine >> m_TopCompoGID >> m_BotCompoGID >> m_dOffset;

    buffer >> m_nPlaceType;
    buffer >> m_IsNumPlaceBarVExt;
    m_SBarVExt.Recovery(buffer);
    m_MainBarVExt.Recovery(buffer);
    buffer >> m_IsNumPlaceBarVInt;
    m_SBarVInt.Recovery(buffer);
    m_MainBarVInt.Recovery(buffer);
    buffer >> m_IsNumPlaceBarHExt;
    m_SBarHExt.Recovery(buffer);
    m_MainBarHExt.Recovery(buffer);
    buffer >> m_IsNumPlaceBarHInt;
    m_SBarHInt.Recovery(buffer);
    m_MainBarHInt.Recovery(buffer);
}


void MSLineLevelDifG::Serialize(CArchive &ar)
{

	MSLinkMemberG::Serialize(ar);

	if(ar.IsStoring())
	{
		int id = (mp_LeftLevelZone == NULL ? 0 : mp_LeftLevelZone->m_ID);
		ar << id;
		id = (mp_RightLevelZone == NULL ? 0 : mp_RightLevelZone->m_ID);
		ar << id;
		id = (mp_LeftSlab == NULL ? 0 : mp_LeftSlab->m_ID);
		ar << id;
		id = (mp_RightSlab == NULL ? 0 : mp_RightSlab->m_ID);
		ar << id;

		ar << m_InfillBeamFactor << m_InfillSlabFactor;
		ar << m_dWallWidth;
		ar << m_bHasHaunch;
		ar << m_bHasWall;
		ar << m_dHaunchX;
		ar << m_dHaunchY;
//		ar << m_dOffset;
		ar << m_bUserLevelDif;
		ar << m_dWallHeight;
		ar << m_dHaunchAngle;
		ar << m_bUseHaunchMaterial;
		ar << m_nHaunchMaterialID;

		if (m_bUserLevelDif)
		{
			GM2DCurve::SerializeWithNullFlag(ar, mp_Curve);
			GM2DCurve::SerializeWithNullFlag(ar, mp_LLine);
			GM2DCurve::SerializeWithNullFlag(ar, mp_RLine);
// 			if (ma_MemPart.size() > 0)
// 			{
// 				GM2DCurve::SerializeWithNullFlag(ar, ma_MemPart[0]->mp_CenLine);
// 				GM2DCurve::SerializeWithNullFlag(ar, ma_MemPart[0]->mp_LLine);
// 				GM2DCurve::SerializeWithNullFlag(ar, ma_MemPart[0]->mp_RLine);
// 			}
		}

		ar << m_bUseBackFillFormwork;
		ar << m_IsUsedStoryMaterial;

        ar << m_nPlaceType;
        ar << m_IsNumPlaceBarVExt;
        m_SBarVExt.Serialize(ar);
        m_MainBarVExt.Serialize(ar);
        ar << m_IsNumPlaceBarVInt;
        m_SBarVInt.Serialize(ar);
        m_MainBarVInt.Serialize(ar);
        ar << m_IsNumPlaceBarHExt;
        m_SBarHExt.Serialize(ar);
        m_MainBarHExt.Serialize(ar);
        ar << m_IsNumPlaceBarHInt;
        m_SBarHInt.Serialize(ar);
        m_MainBarHInt.Serialize(ar);
	}
	else
	{
		ar >> m_nLeftLevelZoneID;
		ar >> m_nRightLevelZoneID;
		ar >> m_nLeftSlabID;
		ar >> m_nRightSlabID;

		ar >> m_InfillBeamFactor >> m_InfillSlabFactor;
		ar >> m_dWallWidth;
		ar >> m_bHasHaunch;
		ar >> m_bHasWall;
		if(MSVersionInfo::GetCurrentVersion() >= 20140630)
		{
			ar >> m_dHaunchX;
			ar >> m_dHaunchY;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20141121)
		{
			ar >> m_bUserLevelDif;
			ar >> m_dWallHeight;
		}
		if(MSVersionInfo::GetCurrentVersion() < 20150311)
		{
			m_dWallHeight *= -1.0;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20150613)
			ar >> m_dHaunchAngle;
		if (MSVersionInfo::GetCurrentVersion() >= 20150630)
		{
			ar >> m_bUseHaunchMaterial;
			ar >> m_nHaunchMaterialID;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20141124)
		{
			if (m_bUserLevelDif)
			{
				GM2DCurve::SerializeWithNullFlag(ar, mp_Curve);
				GM2DCurve::SerializeWithNullFlag(ar, mp_LLine);
				GM2DCurve::SerializeWithNullFlag(ar, mp_RLine);
// 				if (ma_MemPart.size() > 0)
// 				{
// 					GM2DCurve::SerializeWithNullFlag(ar, ma_MemPart[0]->mp_CenLine);
// 					GM2DCurve::SerializeWithNullFlag(ar, ma_MemPart[0]->mp_LLine);
// 					GM2DCurve::SerializeWithNullFlag(ar, ma_MemPart[0]->mp_RLine);
// 				}
// 				else
				//{
				//	GM2DCurve* tmp1, *tmp2, *tmp3;
				//	GM2DCurve::SerializeWithNullFlag(ar, tmp1);
				//	GM2DCurve::SerializeWithNullFlag(ar, tmp2);
				//	GM2DCurve::SerializeWithNullFlag(ar, tmp3);
				//}
			}
		}
		
		if (MSVersionInfo::GetCurrentVersion() >= 20160309)
		{
			ar >> m_bUseBackFillFormwork;
		}
		
		if (MSVersionInfo::GetCurrentVersion() >= 20160310)
		{
			ar >> m_IsUsedStoryMaterial;
		}
//		ar >> m_dOffset;

        if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20200901)
        {
            ar >> m_nPlaceType;
            ar >> m_IsNumPlaceBarVExt;
            m_SBarVExt.Serialize(ar);
            m_MainBarVExt.Serialize(ar);
            ar >> m_IsNumPlaceBarVInt;
            m_SBarVInt.Serialize(ar);
            m_MainBarVInt.Serialize(ar);
            ar >> m_IsNumPlaceBarHExt;
            m_SBarHExt.Serialize(ar);
            m_MainBarHExt.Serialize(ar);
            ar >> m_IsNumPlaceBarHInt;
            m_SBarHInt.Serialize(ar);
            m_MainBarHInt.Serialize(ar);
        }
	}
}

MSObject* MSLineLevelDifG::Copy( bool bNewID /*= true*/ )
{
	MSLineLevelDifG* pNewObj;

	pNewObj = new MSLineLevelDifG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSLineLevelDifG::CopyFromMe( MSLineLevelDifG* pSource, bool bNewID/*=true*/)
{
	MSLinkMemberG::CopyFromMe(pSource, bNewID);

	if(pSource)
	{
		mp_LeftLevelZone = pSource->mp_LeftLevelZone;
		mp_RightLevelZone = pSource->mp_RightLevelZone;
		mp_LeftSlab = pSource->mp_LeftSlab;
		mp_RightSlab = pSource->mp_RightSlab;
		if(mp_Curve)
		{
			delete mp_Curve;
			mp_Curve = NULL;
		}
		if(pSource->mp_Curve)
			mp_Curve = pSource->mp_Curve->Copy();
		m_InfillBeamFactor = pSource->m_InfillBeamFactor;
		m_InfillSlabFactor = pSource->m_InfillSlabFactor;

		if(mp_BeamHaunch)
		{
			delete mp_BeamHaunch;
			mp_BeamHaunch = NULL;
		}
		if(pSource->mp_BeamHaunch)
			mp_BeamHaunch = pSource->mp_BeamHaunch->Copy();
		if(mp_SlabHaunch)
		{
			delete mp_SlabHaunch;
			mp_SlabHaunch = NULL;
		}
		if(pSource->mp_SlabHaunch)
			mp_SlabHaunch = pSource->mp_SlabHaunch->Copy();

		m_dWallWidth = pSource->m_dWallWidth;
		m_bHasHaunch = pSource->m_bHasHaunch;
		m_bHasWall = pSource->m_bHasWall;
		m_dHaunchX = pSource->m_dHaunchX;
		m_dHaunchY = pSource->m_dHaunchY;
		m_dHaunchAngle = pSource->m_dHaunchAngle;
		m_bUserLevelDif = pSource->m_bUserLevelDif;
		m_dWallHeight = pSource->m_dWallHeight;
		m_bUseHaunchMaterial = pSource->m_bUseHaunchMaterial;
		m_nHaunchMaterialID = pSource->m_nHaunchMaterialID;

		ma_LinkMemberGID = pSource->ma_LinkMemberGID;

		m_bUseBackFillFormwork = pSource->m_bUseBackFillFormwork;
		m_IsUsedStoryMaterial = pSource->m_IsUsedStoryMaterial;
		/*
		if (pSource->mp_LLine != NULL) mp_LLine = pSource->mp_LLine->Copy();
		if (pSource->mp_RLine != NULL) mp_RLine = pSource->mp_RLine->Copy();
		if (pSource->ma_MemPart[0]->mp_LLine != NULL) mp_LLine = pSource->ma_MemPart[0]->mp_LLine->Copy();
		if (pSource->ma_MemPart[0]->mp_RLine != NULL) mp_RLine = pSource->ma_MemPart[0]->mp_RLine->Copy();
		*/

        m_nPlaceType = pSource->m_nPlaceType;
        m_IsNumPlaceBarVExt = pSource->m_IsNumPlaceBarVExt;
        m_SBarVExt.CopyFromMe(&pSource->m_SBarVExt);
        m_MainBarVExt.CopyFromMe(&pSource->m_MainBarVExt);
        m_IsNumPlaceBarVInt = pSource->m_IsNumPlaceBarVInt;
        m_SBarVInt.CopyFromMe(&pSource->m_SBarVInt);
        m_MainBarVInt.CopyFromMe(&pSource->m_MainBarVInt);
        m_IsNumPlaceBarHExt = pSource->m_IsNumPlaceBarHExt;
        m_SBarHExt.CopyFromMe(&pSource->m_SBarHExt);
        m_MainBarHExt.CopyFromMe(&pSource->m_MainBarHExt);
        m_IsNumPlaceBarHInt = pSource->m_IsNumPlaceBarHInt;
        m_SBarHInt.CopyFromMe(&pSource->m_SBarHInt);
        m_MainBarHInt.CopyFromMe(&pSource->m_MainBarHInt);
	}
}

void MSLineLevelDifG::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSElement::Init(bInConstructor);

// 	m_dOffset = 0.;
// 	m_TopCompoGID = -1;
// 	m_BotCompoGID = -1;
}

void MSLineLevelDifG::SetCenCurve( GM2DCurve* pCurve )
{
	if (pCurve == NULL)
		return;

	if(mp_Curve != NULL && mp_Curve != pCurve)
		delete mp_Curve;
	if (pCurve->m_ObjType == GT_ARC)
	{
		GM2DVector SVec = pCurve->m_P0;
		GM2DVector EVec = pCurve->m_P1;
		GM2DVector CVec = pCurve->PointAtParam(0.5);
		GM2DArc TmpArc;
		if (TmpArc.Set3P(SVec, CVec, EVec) == false)
		{
			delete pCurve;
			pCurve = new GM2DLineVector(SVec, EVec);
		}
	}

	mp_Curve = pCurve;

	ma_Vec[0]->SetXY(pCurve->m_P0.m_X, pCurve->m_P0.m_Y);
	ma_Vec[1]->SetXY(pCurve->m_P1.m_X, pCurve->m_P1.m_Y);
}

bool MSLineLevelDifG::SplitAndSetRightLevelWhenOverlap( MSLineLevelDifG* pUrLevel, double& UrP0Loc, double& UrP1Loc )
{
	//my split : 11, 12, 13, 16, 17, 18, 21, 22, 23
	//ur split : 11, 16, 21,

	GM2DVector tmpVec;
	double FromLoc, ToLoc, tmpLoc;
	long Result = mp_Curve->Overlap4DTol(pUrLevel->mp_Curve, UrP0Loc, UrP1Loc, MSFloor::ms_ColinearDegreeTol, DTOL1);
// 	if( (fabs(UrP0Loc - 0) < DTOL_GM && fabs(UrP1Loc - 1) < DTOL_GM) ||
// 		fabs(UrP0Loc -1) < DTOL_GM && fabs(UrP1Loc - 0) < DTOL_GM)
// 		return false;

	switch (Result)
	{
	case 9:
		// 추가 나랑 너랑 똑같은 LevelDif 삭제 및 L R LevelZone 셋팅
		mp_Curve->m_P0 = mp_Curve->m_P1;
		mp_LeftLevelZone->AddLevelDifG(pUrLevel);
		if(pUrLevel->mp_LeftLevelZone == NULL)
			pUrLevel->mp_LeftLevelZone = mp_LeftLevelZone;
		if(pUrLevel->mp_RightLevelZone == NULL)
			pUrLevel->mp_RightLevelZone = mp_LeftLevelZone;

		return true;
	case 11:
		tmpVec = pUrLevel->mp_Curve->m_P0;
		if(pUrLevel->Split(0., pUrLevel->mp_Curve->OrthoBaseLocValue(mp_Curve->m_P0), mp_LeftLevelZone) != NULL)
			mp_Curve->m_P0.SetXY(tmpVec.m_X, tmpVec.m_Y);
		return true;
	case 16:
		if(pUrLevel->Split(0., pUrLevel->mp_Curve->OrthoBaseLocValue(mp_Curve->m_P0), mp_LeftLevelZone) != NULL)
			mp_Curve->m_P1.SetXY(mp_Curve->m_P0.m_X, mp_Curve->m_P0.m_Y);	// 삭제
		return true;
	case 12:
		// 길이를 0으로 셋팅 이후에 삭제함
		mp_Curve->m_P0.SetXY(pUrLevel->mp_Curve->m_P0.m_X, pUrLevel->mp_Curve->m_P0.m_Y);
		mp_LeftLevelZone->AddLevelDifG(pUrLevel);
		pUrLevel->mp_RightLevelZone = mp_LeftLevelZone;
		return true;
	case 13:
		FromLoc = mp_Curve->OrthoBaseLocValue(pUrLevel->mp_Curve->m_P0);
		ToLoc = mp_Curve->OrthoBaseLocValue(pUrLevel->mp_Curve->m_P1);
		if(FromLoc > ToLoc)
		{
			tmpLoc = ToLoc;
			ToLoc = FromLoc;
			FromLoc = tmpLoc;
		}
		Split(FromLoc, ToLoc, mp_LeftLevelZone);
		//pUrLevel->mp_Curve->m_P0.SetXY(pUrLevel->mp_Curve->m_P1.m_X, pUrLevel->mp_Curve->m_P1.m_Y);
		return true;
	case 17:
	case 18:
		// 길이를 0으로 셋팅 이후에 삭제함
		mp_Curve->m_P1.SetXY(pUrLevel->mp_Curve->m_P1.m_X, pUrLevel->mp_Curve->m_P1.m_Y);
		mp_LeftLevelZone->AddLevelDifG(pUrLevel);
		pUrLevel->mp_RightLevelZone = mp_LeftLevelZone;
		return true;
	case 21:
		if(pUrLevel->Split(pUrLevel->mp_Curve->OrthoBaseLocValue(mp_Curve->m_P1), pUrLevel->mp_Curve->OrthoBaseLocValue(mp_Curve->m_P0), mp_LeftLevelZone) != NULL)
			mp_Curve->m_P1.SetXY(mp_Curve->m_P0.m_X, mp_Curve->m_P0.m_Y);	//삭제
		return true;
	case 22:
	case 23:
		tmpVec = pUrLevel->mp_Curve->m_P1;
		if(pUrLevel->Split(pUrLevel->mp_Curve->OrthoBaseLocValue(mp_Curve->m_P1), 1., mp_LeftLevelZone) != NULL)
			mp_Curve->m_P1.SetXY(tmpVec.m_X, tmpVec.m_Y);
		return true;
	default:
		break;
	}

	return false;
}

MSLineLevelDifG* MSLineLevelDifG::Split( double FromLoc, double ToLoc, MSLevelZone* pRightZone )
{
	double DistTolValue = MSFloor::ms_DefLineHitDistTol / mp_Curve->Length();
	GM2DVector FromVec = mp_Curve->PointAtParam(FromLoc);
	GM2DVector ToVec = mp_Curve->PointAtParam(ToLoc);

	GM2DCurve* pMyCurve = mp_Curve->Copy();
	pMyCurve->SetP0(FromVec);
	pMyCurve->SetP1(ToVec);
	double dLen = pMyCurve->Length();
	delete pMyCurve;

	if(dLen > MSFloor::ms_DefLineHitDistTol)
	{
		if(mp_RightLevelZone != NULL && mp_RightLevelZone != pRightZone)
			ASSERT(0);

		MSLineLevelDifG* pNewLevelDif;
		if(FromLoc > DistTolValue)
		{
			pNewLevelDif = (MSLineLevelDifG*)Copy();
			pNewLevelDif->mp_Curve->SetP1(FromVec);
			//mp_LeftLevelZone->ma_LineLevelDif.push_back(pNewLevelDif);
			mp_LeftLevelZone->AddLevelDifG(pNewLevelDif);
		}
		if(ToLoc < (1.-DistTolValue))
		{
			pNewLevelDif = (MSLineLevelDifG*)Copy();
			pNewLevelDif->mp_Curve->SetP0(ToVec);
			//mp_LeftLevelZone->ma_LineLevelDif.push_back(pNewLevelDif);
			mp_LeftLevelZone->AddLevelDifG(pNewLevelDif);
		}
		mp_RightLevelZone = pRightZone;
		mp_Curve->SetP0(FromVec);
		mp_Curve->SetP1(ToVec);
		//pRightZone->ma_LineLevelDif.push_back(this);
		// 수정 kkw
		pRightZone->AddLevelDifG(this);
		// 수정 kkw
		// 	pNewLevelDif = (MSLineLevelDifG*)Copy();
		// 	pRightZone->AddLevelDifG(pNewLevelDif);
	}
	else
		return NULL;

	return this;
}

void MSLineLevelDifG::SetEdgeWallFromProfile(GM2DPolyline* profile, int idx, double dLoc1, double dLoc2)
{
	if (idx >= profile->ma_Bulge.GetSize()) return;
	if (idx+1 >= profile->ma_Point.GetSize()) return;

	double bulge = 0.0;
	if (idx < profile->ma_Bulge.GetSize())
		bulge = profile->ma_Bulge[idx];

	if(dLoc1 > dLoc2)
		swap(dLoc1, dLoc2);

	/////////////////////////////////////////////////////////////////////////////
	// MAKE Center-Line
	GM2DCurve* pCenCurve = NULL;

	GM2DLineVector line(profile->ma_Point[idx]->m_X, profile->ma_Point[idx]->m_Y, profile->ma_Point[idx+1]->m_X, profile->ma_Point[idx+1]->m_Y);
	//GM2DVector* p1 = (GM2DVector*)profile->ma_Point[idx]->Copy();
	//GM2DVector* p2 = (GM2DVector*)profile->ma_Point[idx+1]->Copy();
	GM2DVector* p1 = (GM2DVector*)(line.PointAtParam(dLoc1).Copy());
	GM2DVector* p2 = (GM2DVector*)(line.PointAtParam(dLoc2).Copy());
	if (fabs(bulge) > 1.e-5) // ARC
	{
		GM2DLineVector line(*p1, *p2);
		GM2DArc* pArc = GM2DArc::MakeFromBulge(&line, bulge);
		pCenCurve = pArc->Copy();
		delete pArc;
	}
	else
	{
		pCenCurve = new GM2DLineVector(p1->m_X, p1->m_Y, p2->m_X, p2->m_Y);
	}

	//MSLineLevelDifG* dif = new MSLineLevelDifG();
	SetOwner(mp_Owner);
	SetCenCurve(pCenCurve);
	//dif->m_dWallWidth = width;
	//dif->m_dWallHeight = height;
	//dif->SetIsUserLevelDif(true);
	//dif->mp_LeftSlab = this;

	/////////////////////////////////////////////////////////////////////////////
	// MAKE LR Line
	MakeLRLine_UserType();
}

void MSLineLevelDifG::MakeLRLine_UserType()
{
	if (ma_Vec.size() != 2) return;
	if ( !IsUserLevelDif() ) return;
	if (mp_Curve == NULL) return;

	double WIDTH = m_dWallWidth;
	GM2DVector* p1 = ma_Vec[0];
	GM2DVector* p2 = ma_Vec[1];

	if (mp_RLine != NULL) { delete mp_RLine; mp_RLine = NULL; }
	if (mp_LLine != NULL) { delete mp_LLine; mp_LLine = NULL; }

	mp_LLine = mp_Curve->Copy();
	mp_RLine = mp_Curve->Copy();
	mp_LLine->NormalTranslateMe(WIDTH);
	if (mp_Curve->m_ObjType == GT_ARC)
	{
		GM2DVector LCenVec = mp_LLine->PointAtParam(0.5);
		MSSlabG* pSlab = (MSSlabG*)GetOwnerCompoG();
		if (pSlab != nullptr && pSlab->Contains(&LCenVec) == 1)
			mp_LLine->NormalTranslateMe(-WIDTH * 2);
	}
// 	mp_RLine->NormalTranslateMe(-WIDTH);
// 	if(mp_LeftSlab->Contains(&(mp_LLine->PointAtParam(0.5))) > 0)
// 		mp_LLine = mp_Curve->Copy();
// 	if(mp_LeftSlab->Contains(&(mp_RLine->PointAtParam(0.5))) > 0)
// 		mp_RLine = mp_Curve->Copy();

// 	
// 
// 	if (mp_Curve->m_ObjType == GT_ARC)
// 	{
// 		GM2DArc* pArc = (GM2DArc*)mp_Curve;
// 		bool reverse = (pArc->GetStartPoint().Distance(*p1) > 1.e-3);
// 		double delta_r = reverse ? WIDTH : -WIDTH;
// 		GM2DArc* pArcLeft = new GM2DArc(pArc->GetRadius()+delta_r, pArc->m_Center.m_X, pArc->m_Center.m_Y, pArc->m_Theta1, pArc->m_Theta2);
// 		//mp_Curve = pArc->Copy();
// 		
// 		mp_RLine = pArc->Copy();
// 		mp_LLine = pArcLeft;
// 	}
// 	else
// 	{
// 		double leng = p1->Distance(*p2);
// 		double uvec[2] = { (p2->m_X - p1->m_X)/leng, (p2->m_Y - p1->m_Y)/leng };
// 		double uvec_left[2] = { -uvec[1], uvec[0] };
// 		//mp_Curve = new GM2DLineVector(p1->m_X, p1->m_Y, p2->m_X, p2->m_Y);
// 		GM2DVector p1Left(p1->m_X+uvec_left[0]*WIDTH, p1->m_Y+uvec_left[1]*WIDTH);
// 		GM2DVector p2Left(p2->m_X+uvec_left[0]*WIDTH, p2->m_Y+uvec_left[1]*WIDTH);
// 
// 		mp_RLine = new GM2DLineVector(p1->m_X, p1->m_Y, p2->m_X, p2->m_Y);
// 		mp_LLine = new GM2DLineVector(p1Left.m_X, p1Left.m_Y, p2Left.m_X, p2Left.m_Y);
// 	}
}

double MSLineLevelDifG::GetThick()
{
	/*
	double dThick = 0.;
	double SlabThick = 0.;
	if(mp_LeftSlab != NULL)
		SlabThick = max(mp_LeftSlab->GetThick(), SlabThick);
	if(mp_RightSlab != NULL)
		SlabThick = max(mp_RightSlab->GetThick(), SlabThick);
	dThick = SlabThick * m_InfillSlabFactor;
	//BeamOrSlab==MSElement::msSlabG ? m_InfillSlabFactor : m_InfillBeamFactor;
	*/

	double dThick = m_dWallWidth;

	if (!m_bHasWall)
		dThick = 0.0;

	return dThick;
}


double MSLineLevelDifG::GetLength()
{
	return mp_Curve->Length();
}

MSMaterial* MSLineLevelDifG::GetMaterial()
{
	if (mp_LeftSlab != NULL)
		return mp_LeftSlab->GetMaterial();
	else if (mp_RightSlab != NULL)
		return mp_RightSlab->GetMaterial();
	
	ASSERT(0);
	return NULL;
}

MSElement* MSLineLevelDifG::GetOwnerCompoG()
{
	if (mp_LeftSlab != NULL)
		return mp_LeftSlab;

	if (mp_RightSlab != NULL)
		return mp_RightSlab;
	
	ASSERT(0);
	return NULL;
}

double MSLineLevelDifG::GetWidth()
{
	return GetThick();
}


void MSLineLevelDifG::SetLines(MSElement::Type_MSCompoG BeamOrSlab, double MemberThickness)
{
	if(m_bUserLevelDif)
	{
		MakeLRLine_UserType();
		return;
	}
	if (mp_Curve == nullptr)	return;

	if(mp_LLine)
		delete mp_LLine;
	if(mp_RLine)
		delete mp_RLine;

	mp_LLine = mp_Curve->Copy();
	mp_RLine = mp_Curve->Copy();

	if(MemberThickness > DTOL_GM)
	{
		GM2DCurve* pTranslateLine = NULL;
		double LeftLevel = mp_LeftLevelZone ? mp_LeftLevelZone->m_Level : 0.;
		double RightLevel = mp_RightLevelZone ? mp_RightLevelZone->m_Level : 0.;
		//double Thick = BeamOrSlab==MSElement::msSlabG ? m_InfillSlabFactor : m_InfillBeamFactor;
		//Thick *= MemberThickness;
		double Thick = GetThick();

		// mylee - 레벨존에서 지정한 Width 사용하는 경우
		/*
		if (mp_LeftLevelZone != NULL || mp_RightLevelZone != NULL)
		{
		MSLevelZone* zone = (mp_LeftLevelZone != NULL ? mp_LeftLevelZone : mp_RightLevelZone);
		if (zone->m_UseDefaultWidth == false)
		Thick = zone->m_Width;
		}
		*/

		if(LeftLevel > RightLevel)
		{
			mp_LLine->NormalTranslateMe(Thick);
			pTranslateLine = mp_LLine;
		}
		else
		{
			mp_RLine->NormalTranslateMe(-Thick);
			pTranslateLine = mp_RLine;
		}

		if(mp_RightSlab != NULL)
			CutLineBySlab(pTranslateLine, mp_RightSlab);
		else if(mp_LeftSlab != NULL)
			CutLineBySlab(pTranslateLine, mp_LeftSlab);
	}
}

//GM2DCurve* MSLineLevelDifG::GetCenLineVec()
shared_ptr<GM2DCurve> MSLineLevelDifG::GetCenCurve()
{
	if(mp_LLine == NULL || mp_RLine == NULL)
		SetLines(msSlabG, m_dWallWidth);

	shared_ptr<GM2DCurve> pCenCurve;
	if(isArc() == false)
	{
		GM2DLineVector SLine(mp_LLine->m_P0, mp_RLine->m_P0);
		GM2DLineVector ELine(mp_LLine->m_P1, mp_RLine->m_P1);
		pCenCurve = shared_ptr<GM2DCurve>(new GM2DLineVector(SLine.PointAtParam(0.5), ELine.PointAtParam(0.5)));
	}
	else
	{
		GM2DVector SVec = mp_LLine->m_P0 + mp_RLine->m_P0;
		GM2DVector EVec = mp_LLine->m_P1 + mp_RLine->m_P1;
		GM2DVector CVec = mp_LLine->PointAtParam(0.5) + mp_RLine->PointAtParam(0.5);
		SVec.m_X = SVec.m_X/2;
		SVec.m_Y = SVec.m_Y/2;
		EVec.m_X = EVec.m_X/2;
		EVec.m_Y = EVec.m_Y/2;
		CVec.m_X = CVec.m_X/2;
		CVec.m_Y = CVec.m_Y/2;

		GM2DArc* pArc = new GM2DArc();
		if(pArc->Set3P(SVec, CVec, EVec))
			pCenCurve = shared_ptr<GM2DCurve>(pArc);
		else
		{
			delete pArc;
			pCenCurve = shared_ptr<GM2DCurve>(new GM2DLineVector(mp_Curve->m_P0, mp_Curve->m_P1));
			SetCenCurve(pCenCurve->Copy());
			SetLines(msSlabG, m_dWallWidth);
		}
	}
	return pCenCurve;
}

GM2DCurve* MSLineLevelDifG::GetCurve()
{
	return mp_Curve;
}

GM2DVector MSLineLevelDifG::GetCenVec()
{
	if (mp_Curve != nullptr)
		return mp_Curve->PointAtParam(0.5);
	
	GM2DLineVector TheLine;
	TheLine.m_P0 = *GetSVec();
	TheLine.m_P1 = *GetEVec();
	return TheLine.PointAtParam(0.5);
}

GM2DCurve* MSLineLevelDifG::GetLongLVec(bool bExtendToOrthoPart)
{
	// 	shared_ptr<GM2DCurve> pLCurve, pRCurve;
	// 	GetTrimmedLRLines(pLCurve, pRCurve);
	GM2DCurve* pLCurve = mp_LLineTR;
	GM2DCurve* pRCurve = mp_RLineTR;

	if (pLCurve == NULL || pRCurve == NULL)
		return NULL;

	shared_ptr<GM2DCurve> pCenCurve = GetCenCurve();
	GM2DCurve* pLongCurve = pCenCurve->Copy();
	if (isArc())
	{
		double LLoc = pCenCurve->OrthoBaseLocValue(pLCurve->GetP0());
		double RLoc = pCenCurve->OrthoBaseLocValue(pRCurve->GetP0());
		pLongCurve->SetP0(pCenCurve->PointAtParam(min(LLoc, RLoc)));
		LLoc = mp_Curve->OrthoBaseLocValue(pLCurve->GetP1());
		RLoc = mp_Curve->OrthoBaseLocValue(pRCurve->GetP1());
		pLongCurve->SetP1(pCenCurve->PointAtParam(max(LLoc, RLoc)));
	}
	else
	{
		if (bExtendToOrthoPart == true/* && HasParallelLinkMember() == false*/)
		{
			GM2DLineVector SLine = GetEndLine4Joint2_ExtendToOrthoPart(0, FALSE);
			GM2DLineVector ELine = GetEndLine4Joint2_ExtendToOrthoPart(1, FALSE);
			pLongCurve->m_P0 = pCenCurve->OrthoBaseVector(SLine.m_P0);
			pLongCurve->m_P1 = pCenCurve->OrthoBaseVector(ELine.m_P0);
		}
		else
		{
			double LLoc = pCenCurve->OrthoBaseLocValue(pLCurve->GetP0());
			double RLoc = pCenCurve->OrthoBaseLocValue(pRCurve->GetP0());
			pLongCurve->SetP0(pCenCurve->PointAtParam(min(LLoc, RLoc)));
			LLoc = pCenCurve->OrthoBaseLocValue(pLCurve->GetP1());
			RLoc = pCenCurve->OrthoBaseLocValue(pRCurve->GetP1());
			pLongCurve->SetP1(pCenCurve->PointAtParam(max(LLoc, RLoc)));
		}
		double dDeg = pCenCurve->GetDegree();
		double dCurveDeg = pLongCurve->GetDegree();
		if (fabs(dDeg - dCurveDeg) > DTOL_GM)
			pLongCurve->ReverseDirection();
	}

	return pLongCurve;
}

void MSLineLevelDifG::CutLineBySlab2(GM2DCurve* pTranslateLine, GM2DPolyline* pPoly, MSSlabG* pSlab, bool isSlabPoly)
{
	double dMyLoc = 0., dUrLoc = 0.;

	vector<GM2DVector> CrossVecs;
	vector<double> MyLocArr, UrLocArr;
	vector<long> ResultArr;
	vector<GM2DVector> CrossVecs2;
	vector<double> MyLocArr2, UrLocArr2;
	vector<long> ResultArr2;
	bool bOnlyInCurve;

	long NumLine = pPoly->GetNumLines();
	for(long iLine = 0 ; iLine < NumLine ; iLine++)
	{
		GM2DCurve* CurLine = pPoly->GetCurve(iLine);
		if(CurLine->m_ObjType == GT_ARC && pTranslateLine->m_ObjType == GT_ARC)
			bOnlyInCurve = true;
		else
			bOnlyInCurve = false;

		// 슬래브 선중 오프닝 위에 있는 선은 교차 검사 하지 않는다.
		if(pSlab != NULL && isSlabPoly == true)
		{
			if(pSlab->IsSlabNOpeningOnLine(CurLine))
			{
				delete CurLine;
				continue;
			}
		}

		long nRval = mp_Curve->IntersectEnd2(CurLine, CrossVecs, MyLocArr, UrLocArr, ResultArr, bOnlyInCurve);
		nRval = nRval%10;
		if(nRval == 1)
		{
			long nRval2 = pTranslateLine->IntersectEnd2(CurLine, CrossVecs2, MyLocArr2, UrLocArr2, ResultArr2, bOnlyInCurve);
			nRval2 = nRval2%10;
			if(abs(nRval2) == 1 )
			//if((nRval2) == 1 )
			{
				bool isFind = false;
				long iResult = 0;
				for(iResult = 0 ; iResult < ResultArr2.size() ; iResult++)
				{
					if(ResultArr2[iResult] == 1) 
					{
						isFind = true;
						break;
					}
				}

				if(isFind == false) iResult = 0;

				long NumCrossVec = CrossVecs2.size();
				if(NumCrossVec == 0)
				{
					delete CurLine;
					continue;
				}

				if (UrLocArr2[iResult] > 0 - DTOL_GM && UrLocArr2[iResult] < 1 + DTOL_GM)
				{
					if (pPoly->Contains(&CrossVecs2[iResult], 1) < 1)
					{
						double dCrossLoc = pTranslateLine->OrthoBaseLocValue(CrossVecs2[iResult]);
						if (!(nRval2 == -1 && (dCrossLoc > 0 && dCrossLoc < 1)))
						{
							GM2DVector TheVec = pTranslateLine->PointAtParam(dCrossLoc);
							if (dCrossLoc < 0.5)
								pTranslateLine->SetP0(CrossVecs2[iResult]);
							else
								pTranslateLine->SetP1(CrossVecs2[iResult]);
						}
					}
				}
			}
			else if(abs(nRval2) == 2)
			//else if((nRval2) == 2)
			{
				bool isFind = false;
				long iResult = 0;
				for(iResult = 0 ; iResult < ResultArr.size() ; iResult++)
				{
					if(ResultArr[iResult] == 1) 
					{
						isFind = true;
						break;
					}
				}

				if(isFind == false) iResult = 0;
				if (UrLocArr2[iResult] > 0 - DTOL_GM && UrLocArr2[iResult] < 1 + DTOL_GM)
				{
					if (pPoly->Contains(&CrossVecs2[iResult], 1) < 1)
					{
						double dCrossLoc = pTranslateLine->OrthoBaseLocValue(CrossVecs2[iResult]);
						GM2DVector TheVec = pTranslateLine->PointAtParam(dCrossLoc);
						if (dCrossLoc < 0.5)
							pTranslateLine->SetP0(CrossVecs2[iResult]);
						else
							pTranslateLine->SetP1(CrossVecs2[iResult]);
					}
				}
			}
		}
		//else if(abs(nRval) == 2)
		else if((nRval) == 2)
		{
			long nRval2 = pTranslateLine->IntersectEnd2(CurLine, CrossVecs2, MyLocArr2, UrLocArr2, ResultArr2, bOnlyInCurve);
			nRval2 = nRval2%10;
			//if(abs(nRval2) == 2)
			if((nRval2) == 2)
			{
				if(pPoly->Contains(&CrossVecs2[0], 1) < 1 && pPoly->Contains(&CrossVecs2[1], 1) < 1)
				{
					double dCrossLoc = pTranslateLine->OrthoBaseLocValue(CrossVecs2[0]);
					if(dCrossLoc < 0.5)
					{
						pTranslateLine->SetP0(CrossVecs2[0]);
						pTranslateLine->SetP1(CrossVecs2[1]);
					}
					else
					{
						pTranslateLine->SetP0(CrossVecs2[1]);
						pTranslateLine->SetP1(CrossVecs2[0]);
					}
				}

			}
			//else if(abs(nRval2) == 1)
			else if((nRval2) == 1)
			{
				bool isFind = false;
				long iResult = 0;
				for(iResult = 0 ; iResult < ResultArr2.size() ; iResult++)
				{
					if(ResultArr2[iResult] == 1) 
					{
						isFind = true;
						break;
					}
				}

				if(isFind == false) iResult = 0;
				if (UrLocArr2[iResult] > 0 - DTOL_GM && UrLocArr2[iResult] < 1 + DTOL_GM)
				{
					if (pPoly->Contains(&CrossVecs2[iResult], 1) < 1)
					{
						double dCrossLoc = pTranslateLine->OrthoBaseLocValue(CrossVecs2[iResult]);
						GM2DVector TheVec = pTranslateLine->PointAtParam(dCrossLoc);
						if (dCrossLoc < 0.5)
							pTranslateLine->SetP0(CrossVecs2[iResult]);
						else
							pTranslateLine->SetP1(CrossVecs2[iResult]);
					}
				}
			}
		}
		delete CurLine;
	}
}

void MSLineLevelDifG::CutLineBySlab(GM2DCurve* pTranslateLine, MSSlabG* pSlab)
{
	double dMyLoc = 0., dUrLoc = 0.;
	
	GM2DPolyline* pPoly = pSlab->GetProfile();
	//GM2DPolyline* pPoly = pSlab->GetTrimmedBoundary(true);
	GM2DLineVector TLineVec;

	vector<GM2DVector> CrossVecs;
	vector<double> MyLocArr, UrLocArr;
	vector<long> ResultArr;
	vector<long> ResultArr2;

	
	TLineVec.m_P0 = pTranslateLine->m_P0;
	TLineVec.m_P1 = pTranslateLine->m_P1;

/*

	long NumLine = pPoly->GetNumLines();
	for(long iLine = 0 ; iLine < NumLine ; iLine++)
	{
		GM2DCurve* CurLine = pPoly->GetCurve(iLine);
		if(CurLine->m_ObjType == GT_ARC && pTranslateLine->m_ObjType == GT_ARC)
			bOnlyInCurve = true;
		else
			bOnlyInCurve = false;

		long nRval2 = pTranslateLine->IntersectEnd2(CurLine, CrossVecs, MyLocArr, UrLocArr, ResultArr2, bOnlyInCurve);
		nRval2 = nRval2%10;
		if(abs(nRval2) == 1 )
		{
			bool isFind = false;
			long iResult = 0;
			for(iResult = 0 ; iResult < ResultArr2.size() ; iResult++)
			{
				if(ResultArr2[iResult] == 1) 
				{
					isFind = true;
					break;
				}
			}

			if(isFind == false) iResult = 0;

			long NumCrossVec = CrossVecs.size();
			if(NumCrossVec == 0)
				continue;

			if(pPoly->Contains(&CrossVecs[iResult], 1) < 1)
			{
				double dCrossLoc = pTranslateLine->OrthoBaseLocValue(CrossVecs[iResult]);
				//if(dCrossLoc < -0.2 || dCrossLoc > 1.2)
					//continue;

				if(pTranslateLine->m_ObjType == GT_ARC)
				{
					GM2DArc* pArc = (GM2DArc*)pTranslateLine;
					if(dCrossLoc < 0.5)
						pArc->SetStartPoint(CrossVecs[iResult]);
					else
						pArc->SetEndPoint(CrossVecs[iResult]);
				}

				if(dCrossLoc < 0.5)
					pTranslateLine->m_P0 = CrossVecs[iResult];
				else
					pTranslateLine->m_P1 = CrossVecs[iResult];
			}
		}
		else if(abs(nRval2) == 2)
		{
			bool isFind = false;
			long iResult = 0;
			for(iResult = 0 ; iResult < ResultArr2.size() ; iResult++)
			{
				if(ResultArr2[iResult] == 1) 
				{
					isFind = true;
					break;
				}
			}

			if(isFind == false) iResult = 0;

			if(pPoly->Contains(&CrossVecs[iResult], 1) < 1)
			{
				double dCrossLoc = pTranslateLine->OrthoBaseLocValue(CrossVecs[iResult]);
				//if(dCrossLoc < -0.2 || dCrossLoc > 1.2)
					//continue;

				if(pTranslateLine->m_ObjType == GT_ARC)
				{
					GM2DArc* pArc = (GM2DArc*)pTranslateLine;
					if(dCrossLoc < 0.5)
						pArc->SetStartPoint(CrossVecs[iResult]);
					else
						pArc->SetEndPoint(CrossVecs[iResult]);
				}

				if(dCrossLoc < 0.5)
					pTranslateLine->m_P0 = CrossVecs[iResult];
				else
					pTranslateLine->m_P1 = CrossVecs[iResult];
			}
		}

	}
*/

	long NumOpening = pSlab->mp_PolylineInfo->ma_Opening.size();
	for(long iOpening = 0 ; iOpening < NumOpening ; iOpening++)
	{
		MSOpening* pOpening = pSlab->mp_PolylineInfo->ma_Opening[iOpening];
		if(pOpening == NULL) continue;
		if (pOpening->IsMadeByCompoG())
		{
			MSBaseBuilding* pBuilding = GetBuilding();
			MSCompoG* pPartnerG = pBuilding->GetCompoGByFloors(pOpening->ma_MadeByCompoGID[0]);
			if(pPartnerG->GetType() == MSElement::msPitG)
				continue;
		}
		GM2DPolyline* pOpenPoly = pOpening->GetProfile();
		if(pOpenPoly->OverlapEx(pTranslateLine) == false)
			continue;
		CutLineBySlab2(pTranslateLine, pOpenPoly, pSlab, false);
	}

	CutLineBySlab2(pTranslateLine, pPoly, pSlab, true);

	/*long NumLine = pPoly->GetNumLines();
	for(long iLine = 0 ; iLine < NumLine ; iLine++)
	{
		GM2DCurve* CurLine = pPoly->GetCurve(iLine);
		if(CurLine->m_ObjType == GT_ARC && pTranslateLine->m_ObjType == GT_ARC)
			bOnlyInCurve = true;
		else
			bOnlyInCurve = false;

		long nRval = mp_Curve->IntersectEnd2(CurLine, CrossVecs, MyLocArr, UrLocArr, ResultArr, bOnlyInCurve);
		nRval = nRval%10;
		if(nRval == 1)
		{
			long nRval2 = pTranslateLine->IntersectEnd2(CurLine, CrossVecs, MyLocArr, UrLocArr, ResultArr2, bOnlyInCurve);
			nRval2 = nRval2%10;
			//if(abs(nRval2) == 1 )
			if((nRval2) == 1 )
			{
				bool isFind = false;
				long iResult = 0;
				for(iResult = 0 ; iResult < ResultArr2.size() ; iResult++)
				{
					if(ResultArr2[iResult] == 1) 
					{
						isFind = true;
						break;
					}
				}

				if(isFind == false) iResult = 0;

				long NumCrossVec = CrossVecs.size();
				if(NumCrossVec == 0)
					continue;

				if(pPoly->Contains(&CrossVecs[iResult], 1) < 1)
				{
					double dCrossLoc = pTranslateLine->OrthoBaseLocValue(CrossVecs[iResult]);

					if(pTranslateLine->m_ObjType == GT_ARC)
					{
						GM2DArc* pArc = (GM2DArc*)pTranslateLine;
						if(dCrossLoc < 0.5)
							pArc->SetStartPoint(CrossVecs[iResult]);
						else
							pArc->SetEndPoint(CrossVecs[iResult]);
					}

					if(dCrossLoc < 0.5)
						pTranslateLine->m_P0 = CrossVecs[iResult];
					else
						pTranslateLine->m_P1 = CrossVecs[iResult];
				}
			}
			//else if(abs(nRval2) == 2)
			else if((nRval2) == 2)
			{
				bool isFind = false;
				long iResult = 0;
				for(iResult = 0 ; iResult < ResultArr2.size() ; iResult++)
				{
					if(ResultArr2[iResult] == 1) 
					{
						isFind = true;
						break;
					}
				}
				
				if(isFind == false) iResult = 0;
				
				if(pPoly->Contains(&CrossVecs[iResult], 1) < 1)
				{
					double dCrossLoc = pTranslateLine->OrthoBaseLocValue(CrossVecs[iResult]);

					if(pTranslateLine->m_ObjType == GT_ARC)
					{
						GM2DArc* pArc = (GM2DArc*)pTranslateLine;
						if(dCrossLoc < 0.5)
							pArc->SetStartPoint(CrossVecs[iResult]);
						else
							pArc->SetEndPoint(CrossVecs[iResult]);
					}

					if(dCrossLoc < 0.5)
						pTranslateLine->m_P0 = CrossVecs[iResult];
					else
						pTranslateLine->m_P1 = CrossVecs[iResult];
				}
			}
		}
		//else if(abs(nRval) == 2)
		else if((nRval) == 2)
		{
			long nRval2 = pTranslateLine->IntersectEnd2(CurLine, CrossVecs, MyLocArr, UrLocArr, ResultArr2, bOnlyInCurve);
			nRval2 = nRval2%10;
			//if(abs(nRval2) == 2)
			if((nRval2) == 2)
			{
				if(pPoly->Contains(&CrossVecs[0], 1) < 1 && pPoly->Contains(&CrossVecs[1], 1) < 1)
				{
					double dCrossLoc = pTranslateLine->OrthoBaseLocValue(CrossVecs[0]);
					if(pTranslateLine->m_ObjType == GT_ARC)
					{
						GM2DArc* pArc = (GM2DArc*)pTranslateLine;
						if(dCrossLoc < 0.5)
						{
							pArc->SetStartPoint(CrossVecs[0]);
							pArc->SetEndPoint(CrossVecs[1]);
						}
						else
						{
							pArc->SetStartPoint(CrossVecs[1]);
							pArc->SetEndPoint(CrossVecs[0]);
						}
					}

					if(dCrossLoc < 0.5)
					{
						pTranslateLine->m_P0 = CrossVecs[0];
						pTranslateLine->m_P1 = CrossVecs[1];
					}
					else
					{
						pTranslateLine->m_P0 = CrossVecs[1];
						pTranslateLine->m_P1 = CrossVecs[0];
					}
				}

			}
			//else if(abs(nRval2) == 1)
			else if((nRval2) == 1)
			{
				bool isFind = false;
				long iResult = 0;
				for(iResult = 0 ; iResult < ResultArr2.size() ; iResult++)
				{
					if(ResultArr2[iResult] == 1) 
					{
						isFind = true;
						break;
					}
				}

				if(isFind == false) iResult = 0;
				if(pPoly->Contains(&CrossVecs[iResult], 1) < 1)
				{
					double dCrossLoc = pTranslateLine->OrthoBaseLocValue(CrossVecs[iResult]);

					if(pTranslateLine->m_ObjType == GT_ARC)
					{
						GM2DArc* pArc = (GM2DArc*)pTranslateLine;
						if(dCrossLoc < 0.5)
							pArc->SetStartPoint(CrossVecs[iResult]);
						else
							pArc->SetEndPoint(CrossVecs[iResult]);
					}

					if(dCrossLoc < 0.5)
						pTranslateLine->m_P0 = CrossVecs[iResult];
					else
						pTranslateLine->m_P1 = CrossVecs[iResult];
				}
			}
		}
	}*/
	
}

void MSLineLevelDifG::LiveCntUp()
{
	m_nLiveCnt++;
}

double MSLineLevelDifG::GetDepth()
{
	if(m_bUserLevelDif)
		return m_dWallHeight;

	if(GetThick() < DTOL_GM)
		return 0.;

// LevelDif에 왼쪽 오른쪽 LevelZone이 존재 했을 경우 사용한 로직임
	double dLLevel = 0.0;
	if(mp_LeftLevelZone != NULL)
		dLLevel = mp_LeftLevelZone->m_Level;
	
	double dRLevel = 0.0;
	if(mp_RightLevelZone != NULL)
		dRLevel = mp_RightLevelZone->m_Level;

	double dSlabThick = 0;
	MSSlabG* pLowSlab = GetLowLevelSlab();
	if(pLowSlab != NULL)
	{
		dSlabThick = pLowSlab->GetThick();
	}

	return fabs(dLLevel - dRLevel) + dSlabThick;
}

double MSLineLevelDifG::GetDepth_XML()
{
	if (m_bUserLevelDif)
		return m_dWallHeight;

	if (GetThick() < DTOL_GM)
		return 0.;

	MSLevelZone* pLevelZone = NULL;

	if (pLevelZone == NULL)
		pLevelZone = mp_LeftLevelZone;
	if (pLevelZone == NULL)
		pLevelZone = mp_RightLevelZone;

	if (pLevelZone == NULL)
		return 0;
	return pLevelZone->m_Level;
}

double MSLineLevelDifG::GetLevelZoneZ()
{
	if (m_bUserLevelDif)
	{
		GM2DVector CenVec = GetCenCurve()->PointAtParam(0.5);
		double dLevel = GetFloor()->GetLevel4LevelZone(CenVec.m_X, CenVec.m_Y);
		return dLevel;
	}

	return 0;
}

void MSLineLevelDifG::ModifyObject(bool isDoit)
{
	if(mp_LeftSlab!= NULL && mp_LeftSlab == mp_RightSlab)
	{
		mp_LeftSlab->TrimLevelDifAll();
		MSFloor::CompoGIDList_NeedRegen_CurTR().push_back( mp_LeftSlab->m_ID );
	}
	else
	{
		if(mp_LeftSlab != NULL)
		{
			mp_LeftSlab->TrimLevelDifAll();
			MSFloor::CompoGIDList_NeedRegen_CurTR().push_back( mp_LeftSlab->m_ID );
		}
		if(mp_RightSlab != NULL)
		{
			mp_RightSlab->TrimLevelDifAll();
			MSFloor::CompoGIDList_NeedRegen_CurTR().push_back( mp_RightSlab->m_ID );
		}
	}
}

bool MSLineLevelDifG::GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal, bool bCheckLevelZone )
{
	double dTopOffSetZ = 0, dBotOffSetZ = 0;
	dTopOffSetZ = max(m_SOffsetZ, m_EOffsetZ);
	dBotOffSetZ = min(m_SOffsetZ, m_EOffsetZ);

 	if (IsUserLevelDif())
 	{
		if (mp_LeftSlab != NULL)
		{
			MSFloor* pCurFloor = (MSFloor*)mp_LeftSlab->GetOwner();
			GM2DVector TheVec = mp_Curve->PointAtParam(0.5);
			double dLevelZoneZ = pCurFloor->GetLevel4LevelZone(TheVec.m_X, TheVec.m_Y);
			TopLevel = GetFloorLevel() + pCurFloor->GetSlabHeight() + dTopOffSetZ + dLevelZoneZ;
			TopLevel += mp_LeftSlab->m_dOffset;
		}

		BottomLevel = TopLevel - fabs(GetDepth()) + dBotOffSetZ;
		if (m_dWallHeight > 0)
		{
			TopLevel += fabs(GetDepth());
			BottomLevel += fabs(GetDepth());
		}
		return true;
	}

	if(mp_LeftLevelZone == NULL && mp_RightLevelZone == NULL)
		return false;

	MSFloor* pCurFloor = NULL;

	if(pCurFloor == NULL && mp_LeftLevelZone != NULL)
		pCurFloor = (MSFloor*)mp_LeftLevelZone->GetOwner();
	if(pCurFloor == NULL && mp_RightLevelZone != NULL)
		pCurFloor = (MSFloor*)mp_RightLevelZone->GetOwner();
	if (pCurFloor == nullptr)	return false;

	double dTopZLevel = 0;

	shared_ptr<GM2DCurve> pCenCurve = GetCenCurve();
	GM2DVector TheVec = pCenCurve->PointAtParam(0.5);
	
	double dLevelZoneZ = 0;
	if(bCheckLevelZone)
		dLevelZoneZ = pCurFloor->GetLevel4LevelZone(TheVec.m_X, TheVec.m_Y);
	
	double dSlabOffset = 0;
	if(mp_LeftSlab)
		dSlabOffset = mp_LeftSlab->m_dOffset;
	else if(mp_RightSlab)
		dSlabOffset = mp_RightSlab->m_dOffset;

	if(pCurFloor->IsFloor())
    {
		TopLevel = GetFloorLevel() + pCurFloor->GetSlabHeight() + dTopOffSetZ + dLevelZoneZ + dSlabOffset;
		BottomLevel = TopLevel-GetDepth() + dBotOffSetZ;
    }
	
	return true;
}

bool MSLineLevelDifG::GetZLevelsbyLoc(double& TopLevel, double& BottomLevel, double dLoc)
{
	double dOffSet = GetOffSetbyLoc(dLoc);
	double dLevelZoneZ = 0;
	double dSlabOffSet = 0;

	if(mp_Owner && mp_Owner->IsFloor())
	{
		MSFloor* pCurFloor = (MSFloor*)mp_Owner;
		GM2DVector TheVec = NULL;
		/*GM2DCurve* pCenCurve = GetCurve();*/
		shared_ptr<GM2DCurve> pCenCurve = GetCenCurve();
		if(dLoc <= 0)
			TheVec = pCenCurve->PointAtParam(0);
		else if(dLoc >= 1)
			TheVec = pCenCurve->PointAtParam(1);
		else
			TheVec = pCenCurve->PointAtParam(dLoc);
		
		if(m_bUserLevelDif == false)
			dLevelZoneZ = pCurFloor->GetLevel4LevelZone(TheVec.m_X, TheVec.m_Y);

		if(mp_LeftSlab != NULL)
			dSlabOffSet = mp_LeftSlab->GetZOffsetOnSlabTopPlane(TheVec.m_X, TheVec.m_Y);

		TopLevel = GetFloorLevel() + ((MSFloor*)mp_Owner)->GetSlabHeight() + dOffSet + dSlabOffSet + dLevelZoneZ;
	}
	else
	{
		if(mp_LeftSlab != NULL)
			dSlabOffSet = mp_LeftSlab->m_dOffset;

		TopLevel = GetFloorLevel()+dOffSet + dSlabOffSet;
	}
	double dDepth = GetDepth();
	if(m_bUserLevelDif)
	{
		if(dDepth > 0)
		{
			BottomLevel = TopLevel - GetMaxSlabThick();
			TopLevel += fabs(dDepth);
		}
		else
		{
			BottomLevel = TopLevel - fabs(dDepth);
		}
	}
	else
	{
		BottomLevel = TopLevel - fabs(dDepth);
	}
	
	return true;
}

MSFloor* MSLineLevelDifG::GetFloor()
{
	MSFloor* pCurFloor = NULL;
	if(mp_LeftLevelZone != NULL)
		pCurFloor = (MSFloor*)mp_LeftLevelZone->GetOwner();
	else if(mp_RightLevelZone != NULL)
		pCurFloor = (MSFloor*)mp_RightLevelZone->GetOwner();
	else
		pCurFloor = (MSFloor*)mp_Owner;

	if(pCurFloor == NULL || pCurFloor->GetType() != MSElement::msFloor)
		return NULL;

	return pCurFloor;
}

double MSLineLevelDifG::GetFloorLevel()
{
	MSFloor* pCurFloor = GetFloor();
	if(pCurFloor == NULL)
		return 0.;
	
	return pCurFloor->GetBuilding()->GetSlabLevel(pCurFloor->GetFloorNum());
}

double MSLineLevelDifG::GetLeftLevel()
{
	if(mp_LeftLevelZone)
		return mp_LeftLevelZone->m_Level;
	return 0;

}

double MSLineLevelDifG::GetRightLevel()
{

	if(mp_RightLevelZone)
		return mp_RightLevelZone->m_Level;
	return 0;
}


void MSLineLevelDifG::RecoverPointerAfterSerialize(MSFloor* floor)
{
	mp_Owner = floor;
// 	if (m_nLeftLevelZoneID != 0)
// 	{
// 		mp_LeftLevelZone = NULL;
// 		for (int i=0 ; i<floor->ma_LevelZone.size() ; ++i)
// 		{
// 			if (floor->ma_LevelZone[i]->m_ID == m_nLeftLevelZoneID) { mp_LeftLevelZone = floor->ma_LevelZone[i]; break; }
// 		}
// 	}
// 	
// 	if (m_nRightLevelZoneID != 0)
// 	{
// 		mp_RightLevelZone = NULL;
// 		for (int i=0 ; i<floor->ma_LevelZone.size() ; ++i)
// 		{
// 			if (floor->ma_LevelZone[i]->m_ID == m_nRightLevelZoneID) { mp_RightLevelZone = floor->ma_LevelZone[i]; break; }
// 		}
// 	}
// 
// 	if ( m_nLeftSlabID!= 0 && (m_nLeftSlabID == m_nRightSlabID) )
// 	{
// 		mp_LeftSlab = NULL;
// 		MSCompoG* compoG = floor->GetCompoG(m_nLeftSlabID);
// 		if (compoG != NULL && compoG->GetType() == MSElement::msSlabG)
// 		{
// 			mp_LeftSlab = (MSSlabG*)compoG;
// 			mp_LeftSlab->ma_LevelDif.push_back(this);
// 		}
// 		mp_RightSlab = mp_LeftSlab;
// 	}
// 	else
// 	{
// 		mp_LeftSlab = NULL;
// 		MSCompoG* compoG = floor->GetCompoG(m_nLeftSlabID);
// 		if (compoG != NULL && compoG->GetType() == MSElement::msSlabG)
// 		{
// 			mp_LeftSlab = (MSSlabG*)compoG;
// 			//mp_LeftSlab->ma_LevelDifOnEdge.push_back(this);
// 		}
// 
// 		mp_RightSlab = NULL;
// 		compoG = floor->GetCompoG(m_nRightSlabID);
// 		if (compoG != NULL && compoG->GetType() == MSElement::msSlabG)
// 		{
// 			mp_RightSlab = (MSSlabG*)compoG;
// 			//mp_RightSlab->ma_LevelDifOnEdge.push_back(this);
// 		}
// 	}
}

double MSLineLevelDifG::GetLevelDifHeight()
{
	if(m_bUserLevelDif)
		return m_dWallHeight;
	
	double dSlabThick = 0;
	MSSlabG* pLowSlab = GetLowLevelSlab();
	if (pLowSlab != NULL)
	{
		dSlabThick = pLowSlab->GetThick();
	}

	return GetDepth() - dSlabThick;
}

bool MSLineLevelDifG::SetLevelDifHeight(double dHeight)
{
	//if(m_bUserLevelDif)
	{
		m_dWallHeight = dHeight;
		return true;
	}

	return false;
}

double MSLineLevelDifG::GetLevelDifWidth()
{
	//if(m_bUserLevelDif)
		return m_dWallWidth;

	bool bLSlabFind = false;
	bool bRSlabFind = false;
	double dLWidth = 0;
	double dRWidth = 0;

	if(mp_LeftSlab != NULL)
		bLSlabFind = mp_LeftSlab->GetLevelDifWidth(this, dLWidth);
	else if(mp_RightSlab != NULL)
		bRSlabFind = mp_RightSlab->GetLevelDifWidth(this, dRWidth);

	return max(dLWidth, dRWidth);

}

bool MSLineLevelDifG::SetLevelDifWidth(double dWidth)
{
	if(m_bUserLevelDif)
	{
		m_dWallWidth = dWidth;
		MakeLRLine_UserType();
		if(mp_LeftSlab != NULL)
			CutLineBySlab(mp_LLine, mp_LeftSlab);
		return true;
	}

	bool bLRval = false;
	bool bRRval = false;
	if(mp_LeftSlab != NULL)
		bLRval = mp_LeftSlab->SetLevelDifWidth(this, dWidth);
	else if(mp_RightSlab != NULL)
		bRRval = mp_RightSlab->SetLevelDifWidth(this, dWidth);

	if(bLRval || bRRval)
		return true;

	return false;
}

bool MSLineLevelDifG::GetLevelDifHasWall()
{
	if(m_bUserLevelDif)
		return m_bHasWall;

	bool bLSlabFind = false;
	bool bRSlabFind = false;
	bool LHasWall = false;
	bool RHasWall = false;

	if(mp_LeftSlab != NULL)
	{
		bLSlabFind = mp_LeftSlab->GetLevelDifHasWall(this, LHasWall);
	}
	else if(mp_RightSlab != NULL)
	{
		bRSlabFind = mp_RightSlab->GetLevelDifHasWall(this, RHasWall);
	}

	if(LHasWall || RHasWall)
		return true;

	return false;
}

bool MSLineLevelDifG::SetLevelDifHasWall(bool HasWall)
{
	if(m_bUserLevelDif)
	{
		m_bHasWall = HasWall;
		return true;
	}

	bool bLRval = false;
	bool bRRval = false;
	if(mp_LeftSlab != NULL)
		bLRval = mp_LeftSlab->SetLevelDifHasWall(this, HasWall);
	else if(mp_RightSlab != NULL)
		bRRval = mp_RightSlab->SetLevelDifHasWall(this, HasWall);

	if(bLRval || bRRval)
		return true;

	return false;
}

bool MSLineLevelDifG::GetLevelDifHasHaunch()
{
	if(m_bUserLevelDif)
		return m_bHasHaunch;

	bool bLSlabFind = false;
	bool bRSlabFind = false;
	bool LHasHaunch = false;
	bool RHasHaunch = false;

	if(mp_LeftSlab != NULL)
		bLSlabFind = mp_LeftSlab->GetLevelDifHasHaunch(this, LHasHaunch);
	if(mp_RightSlab != NULL)
		bRSlabFind = mp_RightSlab->GetLevelDifHasHaunch(this, RHasHaunch);

	if(LHasHaunch || RHasHaunch)
		return true;

	return false;
}

bool MSLineLevelDifG::SetLevelDifHasHaunch(bool HasHaunch)
{
	if(m_bUserLevelDif)
	{
		m_bHasHaunch = HasHaunch;
		return true;
	}

	bool bLRval = false;
	bool bRRval = false;
	if(mp_LeftSlab != NULL)
		bLRval = mp_LeftSlab->SetLevelDifHasHaunch(this, HasHaunch);
	if(mp_RightSlab != NULL)
		bRRval = mp_RightSlab->SetLevelDifHasHaunch(this, HasHaunch);

	if(bLRval || bRRval)
		return true;

	return false;
}

double MSLineLevelDifG::GetLevelDifHaunchX()
{
	if(m_bUserLevelDif)
		return m_dHaunchX;

	bool bLSlabFind = false;
	bool bRSlabFind = false;
	double LHaunchX = 0.0;
	double RHaunchX = 0.0;

	if(mp_LeftSlab != NULL)
		bLSlabFind = mp_LeftSlab->GetLevelDifHaunchX(this, LHaunchX);
	if(mp_RightSlab != NULL)
		bRSlabFind = mp_RightSlab->GetLevelDifHaunchX(this, RHaunchX);

	if (bLSlabFind) return LHaunchX;
	if (bRSlabFind) return RHaunchX;

	return 0.0;
}

bool MSLineLevelDifG::SetLevelDifHaunchX(double HaunchX)
{
	if(m_bUserLevelDif)
	{
		m_dHaunchX = HaunchX;
		return true;
	}

	bool bLRval = false;
	bool bRRval = false;
	if(mp_LeftSlab != NULL)
		bLRval = mp_LeftSlab->SetLevelDifHaunchX(this, HaunchX);
	if(mp_RightSlab != NULL)
		bRRval = mp_RightSlab->SetLevelDifHaunchX(this, HaunchX);

	if(bLRval || bRRval)
		return true;

	return false;
}

double MSLineLevelDifG::GetLevelDifHaunchY()
{
	if(m_bUserLevelDif)
		return m_dHaunchY;

	bool bLSlabFind = false;
	bool bRSlabFind = false;
	double LHaunchY = 0.0;
	double RHaunchY = 0.0;

	if(mp_LeftSlab != NULL)
		bLSlabFind = mp_LeftSlab->GetLevelDifHaunchY(this, LHaunchY);
	if(mp_RightSlab != NULL)
		bRSlabFind = mp_RightSlab->GetLevelDifHaunchY(this, RHaunchY);

	if (bLSlabFind) return LHaunchY;
	if (bRSlabFind) return RHaunchY;

	return 0.0;
}

bool MSLineLevelDifG::SetLevelDifHaunchY(double HaunchY)
{
	if(m_bUserLevelDif)
	{
		m_dHaunchY = HaunchY;
		return true;
	}

	bool bLRval = false;
	bool bRRval = false;
	if(mp_LeftSlab != NULL)
		bLRval = mp_LeftSlab->SetLevelDifHaunchY(this, HaunchY);
	if(mp_RightSlab != NULL)
		bRRval = mp_RightSlab->SetLevelDifHaunchY(this, HaunchY);

	if(bLRval || bRRval)
		return true;

	return false;
}

double MSLineLevelDifG::GetLevelDifHaunchAngle()
{
	if (m_bUserLevelDif)
		return m_dHaunchAngle;

	bool bLSlabFind = false;
	bool bRSlabFind = false;
	double LHaunchAngle = 0.0;
	double RHaunchAngle = 0.0;

	if (mp_LeftSlab != NULL)
		bLSlabFind = mp_LeftSlab->GetLevelDifHaunchAngle(this, LHaunchAngle);
	if (mp_RightSlab != NULL)
		bRSlabFind = mp_RightSlab->GetLevelDifHaunchAngle(this, RHaunchAngle);

	if (bLSlabFind) return LHaunchAngle;
	if (bRSlabFind) return RHaunchAngle;

	return 0.0;
}

bool MSLineLevelDifG::SetLevelDifHaunchAngle(double HaunchAngle)
{
	if (m_bUserLevelDif)
	{
		m_dHaunchAngle = HaunchAngle;
		return true;
	}
	m_dHaunchAngle = HaunchAngle;

	bool bLRval = false;
	bool bRRval = false;
	if (mp_LeftSlab != NULL)
		bLRval = mp_LeftSlab->SetLevelDifHaunchAngle(this, HaunchAngle);
	if (mp_RightSlab != NULL)
		bRRval = mp_RightSlab->SetLevelDifHaunchAngle(this, HaunchAngle);

	if (bLRval || bRRval)
		return true;

	return false;
}

bool MSLineLevelDifG::GetLevelDifUseHaunchMaterial()
{
	if (m_bUserLevelDif)
		return m_bUseHaunchMaterial;

	bool bLSlabFind = false;
	bool bRSlabFind = false;
	bool l_value = false;
	bool r_value = false;

	if (mp_LeftSlab != NULL)
		bLSlabFind = mp_LeftSlab->GetLevelDifUseHaunchMaterial(this, l_value);
	if (mp_RightSlab != NULL)
		bRSlabFind = mp_RightSlab->GetLevelDifUseHaunchMaterial(this, r_value);

	if (bLSlabFind) return l_value;
	if (bRSlabFind) return r_value;

	return 0.0;
}

bool MSLineLevelDifG::SetLevelDifUseHaunchMaterial(bool value)
{
	if (m_bUserLevelDif)
	{
		m_bUseHaunchMaterial = value;
		return true;
	}
	m_bUseHaunchMaterial = value;

	bool bLRval = false;
	bool bRRval = false;
	if (mp_LeftSlab != NULL)
		bLRval = mp_LeftSlab->SetLevelDifUseHaunchMaterial(this, value);
	if (mp_RightSlab != NULL)
		bRRval = mp_RightSlab->SetLevelDifUseHaunchMaterial(this, value);

	if (bLRval || bRRval)
		return true;

	return false;
}

bool MSLineLevelDifG::GetLevelDifUseBackFillFormwork()
{
	if (m_bUserLevelDif)
		return m_bUseBackFillFormwork;

	bool bLSlabFind = false;
	bool bRSlabFind = false;
	bool l_value = false;
	bool r_value = false;

	if (mp_LeftSlab != NULL)
		bLSlabFind = mp_LeftSlab->GetLevelDifUseBackFillFormwork(this, l_value);
	if (mp_RightSlab != NULL)
		bRSlabFind = mp_RightSlab->GetLevelDifUseBackFillFormwork(this, r_value);

	if (bLSlabFind) return l_value;
	if (bRSlabFind) return r_value;

	return m_bUseBackFillFormwork;
}

void MSLineLevelDifG::SetLevelDifUseBackFillFormwork(bool value)
{
	if (m_bUserLevelDif)
	{
		m_bUseBackFillFormwork = value;
		return;
	}

	m_bUseBackFillFormwork = value;

	if (mp_LeftSlab != NULL)
		mp_LeftSlab->SetLevelDifUseBackFillFormwork(this, value);
	if (mp_RightSlab != NULL)
		mp_RightSlab->SetLevelDifUseBackFillFormwork(this, value);
}

int MSLineLevelDifG::GetLevelDifHaunchMaterialID()
{
	if (m_bUserLevelDif)
		return GetHaunchMaterialID();

	bool bLSlabFind = false;
	bool bRSlabFind = false;
	int l_value = false;
	int r_value = false;

	if (mp_LeftSlab != NULL)
		bLSlabFind = mp_LeftSlab->GetLevelDifHaunchMaterialID(this, l_value);
	if (mp_RightSlab != NULL)
		bRSlabFind = mp_RightSlab->GetLevelDifHaunchMaterialID(this, r_value);

	if (bLSlabFind) return l_value;
	if (bRSlabFind) return r_value;

	return 0.0;
}

bool MSLineLevelDifG::SetLevelDifHaunchMaterialID(int value)
{
	if (m_bUserLevelDif)
	{
		SetHaunchMaterialID(value);
		return true;
	}
	SetHaunchMaterialID(value);

	bool bLRval = false;
	bool bRRval = false;
	if (mp_LeftSlab != NULL)
		bLRval = mp_LeftSlab->SetLevelDifHaunchMaterialID(this, value);
	if (mp_RightSlab != NULL)
		bRRval = mp_RightSlab->SetLevelDifHaunchMaterialID(this, value);

	if (bLRval || bRRval)
		return true;

	return false;
}

bool MSLineLevelDifG::GetLevelDifHaunchUsedStoryMaterial()
{
	if (m_bUserLevelDif)
		return GetIsUsedStoryMaterial();

	bool bLSlabFind = false;
	bool bRSlabFind = false;
	bool l_value = false;
	bool r_value = false;

	if (mp_LeftSlab != NULL)
		bLSlabFind = mp_LeftSlab->GetLevelDifHaunchUsedStoryMaterial(this, l_value);
	if (mp_RightSlab != NULL)
		bRSlabFind = mp_RightSlab->GetLevelDifHaunchUsedStoryMaterial(this, r_value);

	if (bLSlabFind) return l_value;
	if (bRSlabFind) return r_value;

	return GetIsUsedStoryMaterial();
}

void MSLineLevelDifG::SetLevelDifHaunchUsedStoryMaterial(bool value)
{
	if (m_bUserLevelDif)
	{
		SetIsUsedStoryMaterial(value);
		return;
	}

	SetIsUsedStoryMaterial(value);

	if (mp_LeftSlab != NULL)
		mp_LeftSlab->SetLevelDifHaunchUsedStoryMaterial(this, value);
	if (mp_RightSlab != NULL)
		mp_RightSlab->SetLevelDifHaunchUsedStoryMaterial(this, value);
}

int MSLineLevelDifG::GetLevelDifPlaceType()
{
    return m_nPlaceType;
}

void MSLineLevelDifG::SetLevelDifPlaceType(int value)
{
    m_nPlaceType = value;
}

void MSLineLevelDifG::SetLevelDifVBarExt(CString value)
{
    int indexAt = value.Find('@');
    if (indexAt == -1)
    {
        m_IsNumPlaceBarVExt = true;
        m_MainBarVExt.InitData(value);
    }
    else
    {
        m_IsNumPlaceBarVExt = false;
        m_SBarVExt.InitData(value);
    }
}

void MSLineLevelDifG::SetLevelDifVBarInt(CString value)
{
    int indexAt = value.Find('@');
    if (indexAt == -1)
    {
        m_IsNumPlaceBarVInt = true;
        m_MainBarVInt.InitData(value);
    }
    else
    {
        m_IsNumPlaceBarVInt = false;
        m_SBarVInt.InitData(value);
    }
}

void MSLineLevelDifG::SetLevelDifHBarExt(CString value)
{
    int indexAt = value.Find('@');
    if (indexAt == -1)
    {
        m_IsNumPlaceBarHExt = true;
        m_MainBarHExt.InitData(value);
    }
    else
    {
        m_IsNumPlaceBarHExt = false;
        m_SBarHExt.InitData(value);
    }
}

void MSLineLevelDifG::SetLevelDifHBarInt(CString value)
{
    int indexAt = value.Find('@');
    if (indexAt == -1)
    {
        m_IsNumPlaceBarHInt = true;
        m_MainBarHInt.InitData(value);
    }
    else
    {
        m_IsNumPlaceBarHInt = false;
        m_SBarHInt.InitData(value);
    }
}

long MSLineLevelDifG::GetHaunchMaterialID()
{
	if (m_IsUsedStoryMaterial)
	{
		MSFloor* pFloor = GetFloor();

		if (pFloor == NULL)
		{
			ASSERT(0);
			return m_nHaunchMaterialID;
		}
		else
		{
			MSBaseBuilding* pBldg = pFloor->GetBuilding();
			MSStoryMaterial* pStoryMaterial = pBldg->GetStoryMaterialByFloorNum(pFloor->GetFloorNum());
			if (pStoryMaterial == NULL)
			{
				ASSERT(0);
				return m_nHaunchMaterialID;
			}

			MSMaterial* pMaterial = pStoryMaterial->GetMaterial(MSMaterial::msConcrete, false);
			if (pMaterial)	return pMaterial->m_ID;
		}
	}
	return m_nHaunchMaterialID;
}

bool MSLineLevelDifG::IsSameLRZLevel()
{
	if( mp_LeftLevelZone != NULL && mp_RightLevelZone != NULL)
	{
		if(mp_LeftLevelZone->m_Level == mp_RightLevelZone->m_Level)
		{
			return true;
		}
	}

	if(mp_LeftSlab != NULL && mp_RightSlab != NULL && mp_LeftSlab != mp_RightSlab)
	{
		double dLLevel = GetLeftSlabLevel();
		double dRLevel = GetRightSlabLevel();

		if( fabs(dLLevel - dRLevel) < DTOL_GM)
			return true;
	}

	return false;
}

double MSLineLevelDifG::GetLeftSlabLevel()
{
	if(mp_LeftSlab == NULL)
		return 0;

	GM2DLineVector TheLine;
	GM2DCurve* pCurve = NULL;
	if(isArc() == false)
	{
		TheLine = GetCenLVec();
		pCurve = &TheLine;
	}
	else
		pCurve = mp_Curve;

	GM2DVector CenVec =  pCurve->PointAtParam(0.5);

	GM2DVector LCenVec = mp_LeftSlab->GetProfile()->MassCenter();

	GM2DLineVector LLine(LCenVec, CenVec);

	GM2DVector LUnitVec = LLine.DirVector().UnitVector();
	LCenVec = CenVec - LUnitVec*5;
	double dLLevel = mp_LeftSlab->GetSlabLevel(LCenVec);

	return dLLevel;
}
double MSLineLevelDifG::GetRightSlabLevel()
{
	if(mp_RightSlab == NULL)
		return 0;

	GM2DLineVector TheLine;
	GM2DCurve* pCurve = NULL;
	if(isArc() == false)
	{
		TheLine = GetCenLVec();
		pCurve = &TheLine;
	}
	else
		pCurve = mp_Curve;

	GM2DVector CenVec =  pCurve->PointAtParam(0.5);

	GM2DVector RCenVec = mp_RightSlab->GetProfile()->MassCenter();

	GM2DLineVector LLine(RCenVec, CenVec);

	GM2DVector LUnitVec = LLine.DirVector().UnitVector();
	RCenVec = CenVec - LUnitVec*5;
	double dRLevel = mp_RightSlab->GetSlabLevel(RCenVec);

	return dRLevel;
}


bool MSLineLevelDifG::IsOnEdgeLevelDifG()
{
	if(/*mp_LeftSlab != NULL && mp_RightSlab != NULL &&*/
		mp_LeftSlab != mp_RightSlab)
		return true;
	return false;
}

MSSlabG* MSLineLevelDifG::GetLowLevelSlab()
{
	if (mp_LeftSlab != NULL)
		return mp_LeftSlab;
	else if (mp_RightSlab != NULL)
		return mp_RightSlab;

	return NULL;
	
	/*
	if(mp_LeftSlab != NULL && mp_RightSlab != NULL)
	{
		double dLLevel = GetLeftSlabLevel();
		double dRLevel = GetRightSlabLevel();
		if(dLLevel < dRLevel)
			return mp_LeftSlab;
		else
			return mp_RightSlab;
	}
	else if(mp_LeftSlab != NULL)
		return mp_LeftSlab;
	else if(mp_RightSlab != NULL)
		return mp_RightSlab;

	return NULL;
	*/
}

int MSLineLevelDifG::GetLevelZoneID()
{
	if (mp_LeftLevelZone != NULL)
		return mp_LeftLevelZone->m_ID;
	else if (mp_RightLevelZone != NULL)
		return mp_RightLevelZone->m_ID;
	else
		return 0;
}

void MSLineLevelDifG::AddConnectionWithMember( MSCompoG* pPartnerMember )
{
	MSConnection* pConnection = NULL;
	long NumOverMem = ma_Connections.size();
	for(long iOverMem = 0 ; iOverMem < NumOverMem ; iOverMem++)
	{
		MSConnection* pConnection_Cur = ma_Connections[iOverMem];
		if(pConnection_Cur->GetFirstCompoG() == pPartnerMember || pConnection_Cur->GetSecondCompoG() == pPartnerMember)
		{
			pPartnerMember->AddConnection(pConnection_Cur);
			pConnection_Cur->CalculateContainedAngle();
			pConnection_Cur->CalculateLinkMemberTrimOption();
			return;
		}
	}

	if(pConnection == NULL)
	{	
		MSCompoG* pFirstCompoG = this;
		MSCompoG* pSecondCompoG = pPartnerMember;
		MSConnection::CalculateFirstSecondCompoG(pFirstCompoG, pSecondCompoG);
		pConnection = new MSConnection(pFirstCompoG, pSecondCompoG);
		pConnection->GetFirstCompoG()->AddConnection(pConnection);
		pConnection->GetSecondCompoG()->AddConnection(pConnection);
		pConnection->CalculateContainedAngle();
		pConnection->CalculateLinkMemberTrimOption();
	}
}

void MSLineLevelDifG::DeleteConnections()
{
	long NumOverMem = ma_Connections.size();
	for(long iOverMem = 0 ; iOverMem < NumOverMem ; iOverMem++)
	{
		MSConnection* pConnection = ma_Connections[iOverMem];
		MSCompoG* pPartnerG = pConnection->GetPartner(this);
		if(pPartnerG == NULL)
			continue;
		pPartnerG->RemoveConnection(pConnection);
		delete pConnection;
	}
	ma_Connections.clear();
}

void MSLineLevelDifG::DeleteSlabLineLevelDif()
{

	MSFloor* pFloor = GetFloor();
	map<long, long>::iterator it;
	for(it = mm_SlabGID.begin() ; it != mm_SlabGID.end() ; it++)
	{
		MSSlabG* pSlabG = (MSSlabG*)pFloor->GetCompoG(it->second);
		if(pSlabG != NULL)
			pSlabG->DeleteSlabLineLevelDif(this);
	}
}

double MSLineLevelDifG::GetMaxSlabThick()
{
	double dMaxSlabThick = 0;
	if(mp_LeftSlab != NULL)
		dMaxSlabThick = max(dMaxSlabThick, mp_LeftSlab->GetThick());
	if(mp_RightSlab != NULL)
		dMaxSlabThick = max(dMaxSlabThick, mp_RightSlab->GetThick());

	return dMaxSlabThick;
}

void MSLineLevelDifG::Translate( GM2DVector &pDelta )
{
	MSLinkMemberG::Translate(pDelta);
	if (mp_Curve != NULL && mp_Curve->m_ObjType == GT_LINE)
		((GM2DLineVector*)mp_Curve)->Translate(pDelta);

	SetLines(msSlabG, m_dWallWidth);
}

void MSLineLevelDifG::MirrorByAxis( GM2DLineVector& pAxis )
{
	MSLinkMemberG::MirrorByAxis(pAxis);
	if (mp_Curve != NULL && mp_Curve->m_ObjType == GT_LINE)
		((GM2DLineVector*)mp_Curve)->MirrorByAxis(pAxis);

	SetLines(msSlabG, m_dWallWidth);
}

void MSLineLevelDifG::RotateZ( double theta, GM2DVector *pCenter )
{
	MSLinkMemberG::RotateZ(theta, pCenter);
	if (mp_Curve != NULL && mp_Curve->m_ObjType == GT_LINE)
		((GM2DLineVector*)mp_Curve)->RotateZ(theta, pCenter);

	SetLines(msSlabG, m_dWallWidth);
}