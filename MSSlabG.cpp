#include "StdAfx.h"
#include "GMLib\AcisGeomHelper.h"
#include "GMLib\GM2DBound.h"
#include "MSSlabG.h"
#include "MSContainer.h"
#include "MSFloor.h"
#include "MSRcFaceMemberM.h"
#include "MSLinkMemberG.h"
#include "MSSlabLineG.h"
//#include "MSProject.h"
#include "MSRebarBuilding.h"
#include "MSAssembly.h"
#include "MSRcSlabM.h"
#include "MSSlabLineG.h"
#include "MSLineLevelDifG.h"
#include "MSSlabLevelDif.h"
#include "MSPolylineObject.h"
#include "MSLevelZone.h"
#include "MSWell.h"
#include "MSTrench.h"
#include "MSSlabRebarInfo.h"
#include "MSOpeningG.h"
#include "MSPitG.h"
#include "MSRcSlabM.h"
#include "MSRcFWSlabM.h"
#include "MSModelChecker.h"
#include "MSStoryMaterial.h"
#include "MSConnection.h"
#include "MSCompoGUtil.h"
#include "MSFWSoundAbsorptionG.h"
#include "Radfcore_CLI/AppGlobalOptionWrapper.h"

#include <stdio.h> 
#include "cstrapi.hxx" // api_cover_wires
#include "cover_options.hxx" // cover_options
#include "coverapi.hxx" // api_cover_wires
#include "boolapi.hxx" // api_boolean
#include "kernapi.hxx" // api_get_edges
#include <unitvec.hxx> 
#include <wire_offset_options.hxx> 
#include <off_wire.hxx> 
#include <ofstapi.hxx> 
#include <geom_utl.hxx> 
#include "get_top.hxx"
#include <curve.hxx> 
#include "acis.hxx"
#include "api.hxx"
#include "kernapi.hxx"
#include "cstrapi.hxx"
#include "lists.hxx"
#include "alltop.hxx"
#include "get_top.hxx"
#include "api.err"
#include "savres.hxx"
#include "versions.hxx"
#include "spa_unlock_result.hxx"
#include "ckoutcom.hxx"
#include <vector>
#include <set>
#include <eulerapi.hxx>
#include <bool_api_options.hxx> 
#include "..\RADFCore_CLI\StatusReporterWrapper.h"
#include <plane.hxx>
#include "fileinfo.hxx"
#include "GMLib/MSVersionInfo.h"
#include <shl_api.hxx> 
#include <sliver_ents.hxx>
#include <stchapi.hxx>
#include <sweepapi.hxx>
#include <boolapi.hxx>
#include <eulerapi.hxx>

#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSSlabG, MSElement, VERSIONABLE_SCHEMA | 1)

#pragma region --- MSSLabLevelZone

MSSlabLevelZone::MSSlabLevelZone()
{
	mp_LevelZone=NULL;
	mp_Polyline= new GM2DPolyline(TRUE);
	m_LevelZoneID = 0;
	m_IsDelete = false;
	m_IsNewSlabZone = true;
	m_IsFullSlabZone = false;
	m_dPreWidth = 0;
	m_dWidth = 0;
	m_dMaxWidth = 0;
}

MSSlabLevelZone::~MSSlabLevelZone()
{
	if(mp_Polyline) 
		delete mp_Polyline;
	m_IsDelete = true;
}

void MSSlabLevelZone::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	if(ar.IsStoring())
	{
		ar << m_LevelZoneID;
		mp_Polyline->Serialize(ar);

		long NumLevelDifInfo = ma_LevelDifInfo.size();
		ar << NumLevelDifInfo;
		for(long i = 0 ; i < NumLevelDifInfo ; i++)
			ma_LevelDifInfo[i].Serialize(ar);
	}
	else
	{
		ar >> m_LevelZoneID;
		if(mp_Polyline != NULL)
			delete mp_Polyline;
		mp_Polyline = new GM2DPolyline();
		mp_Polyline->Serialize(ar);

		long NumLevelDifInfo = 0;
		ar >> NumLevelDifInfo;
		for(long i = 0 ; i < NumLevelDifInfo ; i++)
		{
			LevelDifInfoStruct LevelDifInfo;
			LevelDifInfo.Serialize(ar);
			ma_LevelDifInfo.push_back(LevelDifInfo);
		}
	}
}

void MSSlabLevelZone::Dump(EFS::EFS_Buffer& buffer)
{
	buffer << m_LevelZoneID;
	mp_Polyline->Dump(buffer);
	buffer << m_IsDelete;
	
	buffer << m_dPreWidth;
	buffer << m_dWidth;

	int count = ma_LevelDifInfo.size();
	buffer << count;
	for(int i = 0 ; i < count ; i++)
		ma_LevelDifInfo[i].Dump(buffer);


}

void MSSlabLevelZone::Recovery(EFS::EFS_Buffer& buffer)
{
	buffer >> m_LevelZoneID;
	if(mp_Polyline == NULL)
		mp_Polyline = new GM2DPolyline(TRUE);
	mp_Polyline->Recovery(buffer);
	buffer >> m_IsDelete;

	buffer >> m_dPreWidth;
	buffer >> m_dWidth;

	int count = 0;
	buffer >> count;
	ma_LevelDifInfo.resize(count);
	for(int i = 0 ; i < count ; i++)
		ma_LevelDifInfo[i].Recovery(buffer);
}

MSObject* MSSlabLevelZone::Copy(bool bNewID)
{
	MSSlabLevelZone* pNewObj;

	pNewObj = new MSSlabLevelZone();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSSlabLevelZone::CopyFromMe(MSObject* pSource, bool bNewID)
{
	if(pSource == nullptr) return;

	MSSlabLevelZone* pSlabLevelZone = (MSSlabLevelZone*)pSource;

	if(pSource)
	{
		m_LevelZoneID = pSlabLevelZone->m_LevelZoneID;
		mp_LevelZone = pSlabLevelZone->mp_LevelZone;
		if(mp_Polyline == NULL)
			mp_Polyline = new GM2DPolyline(TRUE);
		mp_Polyline->CopyFrom(pSlabLevelZone->mp_Polyline);

		m_IsDelete = pSlabLevelZone->m_IsDelete;
		// 일단 정보만 가지고 있고 LevelZone 이 존재할경우 찾아서 셋팅한다.
		//m_IsDelete = true;

		m_dPreWidth = pSlabLevelZone->m_dPreWidth;
		m_dWidth = pSlabLevelZone->m_dWidth;

		long NumLevelDifInfo = pSlabLevelZone->ma_LevelDifInfo.size();
		for(long i = 0 ; i < NumLevelDifInfo ; i++)
			ma_LevelDifInfo.push_back(pSlabLevelZone->ma_LevelDifInfo[i]);
	}
}

long MSSlabLevelZone::FindIdxByCurve(GM2DCurve* pCurve)
{
	if(pCurve == NULL)
		return -1;
	
	double dTol = m_dMaxWidth + 20; // Width보다 작은 LevelDif는 삭제되기때문에 Dtol 값을 증가 시킴

	GM2DVector MidVec = pCurve->PointAtParam(0.5);
	long NumCurve = mp_Polyline->GetNumLines();
	for(long iCurve = 0 ; iCurve < NumCurve ; iCurve++)
	{
		GM2DCurve* pCurCurve = mp_Polyline->GetCurve(iCurve);
		bool isSVecSame = false;
		bool isEVecSame = false;
		bool isMVecSame = false;

		GM2DVector CurMidVec = pCurCurve->PointAtParam(0.5);

		if(pCurCurve->m_P0.SameVector4DTOL(pCurve->m_P0, dTol))
			isSVecSame = true;
		else if(pCurCurve->m_P0.SameVector4DTOL(pCurve->m_P1, dTol))
			isSVecSame = true;

		if(pCurCurve->m_P1.SameVector4DTOL(pCurve->m_P0, dTol))
			isEVecSame = true;
		else if(pCurCurve->m_P1.SameVector4DTOL(pCurve->m_P1, dTol))
			isEVecSame = true;

		if(CurMidVec.SameVector4DTOL(MidVec, dTol))
			isMVecSame = true;

		delete pCurCurve;

		if(isSVecSame == true && isEVecSame == true && isMVecSame == true)
			return iCurve;
	}
	return -1;
}

// bool MSSlabLevelZone::GetLevelDifID(MSLineLevelDifG* pLevelDifG, long& ID)
// {
// 	GM2DLineVector TheLine;
// 	GM2DCurve* pCurve;
// 	if(pLevelDifG->mp_Curve == NULL)
// 	{
// 		TheLine = pLevelDifG->GetCenLVec();
// 		pCurve = &TheLine;
// 	}
// 	else
// 		pCurve = pLevelDifG->mp_Curve;
// 
// 	long Idx = FindIdxByCurve(pCurve);
// 	if(Idx > -1)
// 	{
// 		ID = ma_LevelDifInfo[Idx].m_ID;
// 		return true;
// 	}
// 
// 	return false;
// }
// 
// bool MSSlabLevelZone::SetLevelDifID(MSLineLevelDifG* pLevelDifG, long ID)
// {
// 	GM2DLineVector TheLine;
// 	GM2DCurve* pCurve;
// 	if(pLevelDifG->mp_Curve == NULL)
// 	{
// 		TheLine = pLevelDifG->GetCenLVec();
// 		pCurve = &TheLine;
// 	}
// 	else
// 		pCurve = pLevelDifG->mp_Curve;
// 
// 	long Idx = FindIdxByCurve(pCurve);
// 	if(Idx > -1)
// 	{
// 		ma_LevelDifInfo[Idx].m_ID = ID;
// 		return true;
// 	}
// 	return false;
// }

bool MSSlabLevelZone::GetLevelDifWidth(MSLineLevelDifG* pLevelDifG, double& dWidth)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if(Idx > -1)
	{
		dWidth = ma_LevelDifInfo[Idx].m_dWallWidth;
		return true;
	}

	return false;
}

bool MSSlabLevelZone::SetLevelDifWidth(MSLineLevelDifG* pLevelDifG, double dWidth)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if(Idx > -1)
	{
		ma_LevelDifInfo[Idx].m_dWallWidth = dWidth;
		m_dMaxWidth = 0;
		for(long i = 0 ; i < ma_LevelDifInfo.size() ;  i++)
			m_dMaxWidth = max(m_dMaxWidth, ma_LevelDifInfo[i].m_dWallWidth);
		return true;
	}
	return false;
}

bool MSSlabLevelZone::GetLevelDifHasWall(MSLineLevelDifG* pLevelDifG, bool& HasWall)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if(Idx > -1)
	{
		HasWall = ma_LevelDifInfo[Idx].m_bHasWall;
		return true;
	}

	return false;
}

bool MSSlabLevelZone::SetLevelDifHasWall(MSLineLevelDifG* pLevelDifG, bool HasWall)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if(Idx > -1)
	{
		ma_LevelDifInfo[Idx].m_bHasWall = HasWall;
		return true;
	}

	return false;
}

bool MSSlabLevelZone::GetLevelDifHasHaunch(MSLineLevelDifG* pLevelDifG, bool& HasHaunch)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if(Idx > -1)
	{
		HasHaunch = ma_LevelDifInfo[Idx].m_bHasHaunch;
		return true;
	}

	return false;
}

bool MSSlabLevelZone::SetLevelDifHasHaunch(MSLineLevelDifG* pLevelDifG, bool HasHaunch)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if(Idx > -1)
	{
		ma_LevelDifInfo[Idx].m_bHasHaunch = HasHaunch;
		return true;
	}

	return false;
}

bool MSSlabLevelZone::GetLevelDifHaunchX(MSLineLevelDifG* pLevelDifG, double& HaunchX)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if(Idx > -1)
	{
		HaunchX = ma_LevelDifInfo[Idx].m_dHaunchX;
		return true;
	}

	return false;
}

bool MSSlabLevelZone::SetLevelDifHaunchX(MSLineLevelDifG* pLevelDifG, double HaunchX)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if(Idx > -1)
	{
		ma_LevelDifInfo[Idx].m_dHaunchX = HaunchX;
		return true;
	}

	return false;
}

bool MSSlabLevelZone::GetLevelDifHaunchY(MSLineLevelDifG* pLevelDifG, double& HaunchY)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if(Idx > -1)
	{
		HaunchY = ma_LevelDifInfo[Idx].m_dHaunchY;
		return true;
	}

	return false;
}

bool MSSlabLevelZone::SetLevelDifHaunchY(MSLineLevelDifG* pLevelDifG, double HaunchY)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if(Idx > -1)
	{
		ma_LevelDifInfo[Idx].m_dHaunchY = HaunchY;
		return true;
	}

	return false;
}

bool MSSlabLevelZone::GetLevelDifHaunchAngle(MSLineLevelDifG* pLevelDifG, double& HaunchAngle)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if (Idx > -1)
	{
		HaunchAngle = ma_LevelDifInfo[Idx].m_dHaunchAngle;
		return true;
	}

	return false;
}

bool MSSlabLevelZone::SetLevelDifHaunchAngle(MSLineLevelDifG* pLevelDifG, double HaunchAngle)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if (Idx > -1)
	{
		ma_LevelDifInfo[Idx].m_dHaunchAngle = HaunchAngle;
		return true;
	}

	return false;
}

bool MSSlabLevelZone::GetLevelDifUseHaunchMaterial(MSLineLevelDifG* pLevelDifG, bool& value)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if (Idx > -1)
	{
		value = ma_LevelDifInfo[Idx].m_bUseHaunchMaterial;
		return true;
	}

	return false;
}

bool MSSlabLevelZone::SetLevelDifUseHaunchMaterial(MSLineLevelDifG* pLevelDifG, bool value)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if (Idx > -1)
	{
		ma_LevelDifInfo[Idx].m_bUseHaunchMaterial = value;
		return true;
	}

	return false;
}

bool MSSlabLevelZone::GetLevelDifUseBackFillFormwork(MSLineLevelDifG* pLevelDifG, bool& value)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if (Idx > -1)
	{
		value = ma_LevelDifInfo[Idx].m_bUseBackFillFormwork;
		return true;
	}

	return false;
}

bool MSSlabLevelZone::SetLevelDifUseBackFillFormwork(MSLineLevelDifG* pLevelDifG, bool value)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if (Idx > -1)
	{
		ma_LevelDifInfo[Idx].m_bUseBackFillFormwork = value;
		return true;
	}

	return false;
}

bool MSSlabLevelZone::GetLevelDifHaunchMaterialID(MSLineLevelDifG* pLevelDifG, int& value)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if (Idx > -1)
	{
		value = ma_LevelDifInfo[Idx].GetHaunchMaterialID(pLevelDifG->GetFloor());
		return true;
	}

	return false;
}

bool MSSlabLevelZone::SetLevelDifHaunchMaterialID(MSLineLevelDifG* pLevelDifG, int value)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if (Idx > -1)
	{
		ma_LevelDifInfo[Idx].SetHaunchMaterialID(value);
		return true;
	}

	return false;
}

bool MSSlabLevelZone::GetLevelDifHaunchUsedStoryMaterial(MSLineLevelDifG* pLevelDifG, bool& value)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if (Idx > -1)
	{
		value = ma_LevelDifInfo[Idx].GetIsUsedStoryMaterial();
		return true;
	}

	return false;
}

bool MSSlabLevelZone::SetLevelDifHaunchUsedStoryMaterial(MSLineLevelDifG* pLevelDifG, bool value)
{
	GM2DLineVector TheLine;
	GM2DCurve* pCurve = pLevelDifG->GetCurve();

	long Idx = FindIdxByCurve(&*pCurve);
	if (Idx > -1)
	{
		ma_LevelDifInfo[Idx].SetIsUsedStoryMaterial(value);
		return true;
	}

	return false;
}
#pragma endregion

#pragma region --- MSSlabG
MSSlabG::MSSlabG()
{
	Init(true);
}

MSSlabG::~MSSlabG(void)
{

	//long nLevelDif = ma_LevelDif.size();
	//for(long iLevelDif=0; iLevelDif<nLevelDif; iLevelDif++)
	//	delete ma_LevelDif[iLevelDif];
	while(!ma_LevelDif.empty()) 
	{
		//        delete ma_LevelDif.back();
		ma_LevelDif.pop_back();
	}
	ma_LevelDif.clear();

	ma_LevelDifOnEdge.clear();

	//long nLevelPoly = ma_LevelPoly.size();
	//for(long iLevelPoly=0; iLevelPoly<nLevelPoly; iLevelPoly++)
	//	delete ma_LevelPoly[iLevelPoly];
	while(!ma_LevelPoly.empty()) 
	{
		delete ma_LevelPoly.back();
		ma_LevelPoly.pop_back();
	}

	DeleteSupport(true);
	DeleteWells();
	DeleteTrenchs();
	DeleteEdgeWalls();
	DeleteHaunchs();
	DeleteSlabLineLevelDifAll();
	
	DeleteSlabOpening();
	if(mp_InnerProFile != NULL)
		delete mp_InnerProFile;
	if (mp_InnerProFile_Bot != NULL)
		delete mp_InnerProFile_Bot;
	if (mp_DeckSlabQTProFile != NULL)
		delete mp_DeckSlabQTProFile;
	if (mp_LxLyProFile != NULL)
		delete mp_LxLyProFile;
	
}

void MSSlabG::Dump( EFS::EFS_Buffer& buffer )
{
	MSCompoG::Dump(buffer);
	buffer << m_dLxDeg << m_dOffset;
	mp_PolylineInfo->Dump(buffer);

	int method = m_nSlopeMethod;
	buffer << method;
	int count = 0;
	count = ma_SlopePoints.size();
	buffer << count;
	for(int i=0 ; i<count ; ++i) ma_SlopePoints[i].Dump(buffer);

	count = ma_SlopePointIdx.size();
	buffer << count;
	for(int i=0 ; i<count ; ++i) buffer << ma_SlopePointIdx[i];

	count = ma_SlopeZOffset.size();
	buffer << count;
	for(int i=0 ; i<count ; ++i) buffer << ma_SlopeZOffset[i];

	count = ma_LevelPoly.size();
	buffer << count;
	for(int i = 0 ; i < count ; i++)
		ma_LevelPoly[i]->Dump(buffer);

	count = mm_Well.size();
	buffer << count;
	map<long, MSWell*>::iterator itWell;
	for(itWell = mm_Well.begin(); itWell != mm_Well.end(); itWell++)
	{
		MSWell* pWell = itWell->second;
		pWell->Dump(buffer);
	}

	count = mm_Trench.size();
	buffer << count;
	map<long, MSTrench*>::iterator itTrench;
	for(itTrench = mm_Trench.begin(); itTrench != mm_Trench.end(); itTrench++)
	{
		MSTrench* pTrench = itTrench->second;
		pTrench->Dump(buffer);
	}

	count = ma_EdgeWall.size();
	buffer << count;
	for (int i=0 ; i<count ; ++i)
		ma_EdgeWall[i]->Dump(buffer);

	m_RibPosition.Dump(buffer);

	buffer << m_TopSpacerID << m_BotSpacerID;;

	bool HasInnerProFile = false;
	if(mp_InnerProFile != NULL)
	{
		HasInnerProFile = true;
		buffer << HasInnerProFile;
		mp_InnerProFile->Dump(buffer);
	}

	count = ma_Haunches .size();
	buffer << count;
	for (int i = 0; i < count; ++i)
		ma_Haunches [i]->Dump(buffer);

	bool HasBotProfile = false;
	if (mp_BotProfile != NULL)
	{
		HasBotProfile = true;
		buffer << HasBotProfile;
		mp_BotProfile->Dump(buffer);
	}

	bool HasInnerProFileBot = false;
	if (mp_InnerProFile_Bot != NULL)
	{
		HasInnerProFileBot = true;
		buffer << HasInnerProFileBot;
		mp_InnerProFile_Bot->Dump(buffer);
	}
	
	bool HasInnerProFileDeck = false;
	if (mp_DeckSlabQTProFile != NULL)
	{
		HasInnerProFileDeck = true;
		buffer << HasInnerProFileDeck;
		mp_DeckSlabQTProFile->Dump(buffer);
	}

	bool HasLxLyProFile = false;
	if (mp_LxLyProFile != NULL)
	{
		HasLxLyProFile = true;
		buffer << HasLxLyProFile;
		mp_LxLyProFile->Dump(buffer);
	}

	buffer << m_IsHasRib;
	buffer << m_IsRibXDirection;

}

void MSSlabG::Recovery( EFS::EFS_Buffer& buffer )
{
	MSCompoG::Recovery(buffer);
	buffer >> m_dLxDeg >> m_dOffset;
	mp_PolylineInfo->Recovery(buffer);

	int method = 0;
	buffer >> method;
	m_nSlopeMethod = (SlopeMethod)method;
	int count = 0;
	buffer >> count;
	ma_SlopePoints.resize(count);
	for(int i=0 ; i<count ; ++i) ma_SlopePoints[i].Recovery(buffer);

	buffer >> count;
	ma_SlopePointIdx.resize(count);
	for(int i=0 ; i<count ; ++i) buffer >> ma_SlopePointIdx[i];

	buffer >> count;
	ma_SlopeZOffset.resize(count);
	for(int i=0 ; i<count ; ++i) buffer >> ma_SlopeZOffset[i];

	MSFloor* pFloor = GetFloor();

	while(!ma_LevelPoly.empty()) 
	{
		delete ma_LevelPoly.back();
		ma_LevelPoly.pop_back();
	}
	ma_LevelPoly.clear();

	buffer >> count;
	for(int i = 0 ; i < count ; i++)
	{
		MSSlabLevelZone* pSlabLevelZone = new MSSlabLevelZone();
		pSlabLevelZone->Recovery(buffer);
		pSlabLevelZone->mp_LevelZone = pFloor->GetLevelZone(pSlabLevelZone->m_LevelZoneID);
		ma_LevelPoly.push_back(pSlabLevelZone);
	}
	DeleteWells();
	DeleteTrenchs();
	DeleteEdgeWalls();
	buffer >> count;
	for(int i = 0; i < count; i++)
	{
		MSWell* pWell = new MSWell;
		pWell->Recovery(buffer);
		pWell->SetOwner(this);
		mm_Well.insert(make_pair(pWell->m_ID, pWell));
	}

	buffer >> count;
	for(int i = 0; i < count; i++)
	{
		MSTrench* pTrench = new MSTrench;
		pTrench->Recovery(buffer);
		pTrench->SetOwner(this);
		mm_Trench.insert(make_pair(pTrench->m_ID, pTrench));
	}

	buffer >> count;
	for(int i = 0; i < count; i++)
	{
		MSLineLevelDifG* pDiff = new MSLineLevelDifG();
		pDiff->Recovery(buffer);
		pDiff->SetOwner(this->mp_Owner);
		pDiff->mp_LeftSlab = this;
		ma_EdgeWall.push_back(pDiff);
	}

	m_RibPosition.Recovery(buffer);

	buffer >> m_TopSpacerID >> m_BotSpacerID;

	bool HasInnerProFile = false;
	buffer >> HasInnerProFile;
	if(HasInnerProFile == true)
	{
		if(mp_InnerProFile == NULL)
			mp_InnerProFile = new GM2DPolyline(TRUE);
		mp_InnerProFile->Recovery(buffer);
	}
	count = 0;
	buffer >> count;
	for (int i = 0; i < ma_Haunches .size(); ++i)
		delete ma_Haunches [i];
	ma_Haunches .clear();
	for (int i = 0; i < count; ++i)
	{
		MSSubHaunchG* haunch = new MSSubHaunchG();
		haunch->mp_Owner = mp_Owner;
		haunch->Recovery(buffer);
		haunch->mp_OwnerCompoG = this;
		ma_Haunches .push_back(haunch);
	}

	bool HasBotProfile = false;
	buffer >> HasBotProfile;
	if (HasBotProfile == true)
	{
		if (mp_BotProfile == nullptr)
			mp_BotProfile = new GM2DPolyline(TRUE);
		mp_BotProfile->Recovery(buffer);
	}

	bool HasInnerProFileBot = false;
	buffer >> HasInnerProFileBot;
	if (HasInnerProFileBot == true)
	{
		if (mp_InnerProFile_Bot == NULL)
			mp_InnerProFile_Bot = new GM2DPolyline(TRUE);
		mp_InnerProFile_Bot->Recovery(buffer);
	}
	bool HasInnerDeckFile = false;
	buffer >> HasInnerDeckFile;
	if (HasInnerDeckFile == true)
	{
		if (mp_DeckSlabQTProFile == NULL)
			mp_DeckSlabQTProFile = new GM2DPolyline(TRUE);
		mp_DeckSlabQTProFile->Recovery(buffer);
	}

	bool HasHasLxLyProFile = false;
	buffer >> HasHasLxLyProFile;
	if (HasHasLxLyProFile == true)
	{
		if (mp_LxLyProFile == NULL)
			mp_LxLyProFile = new GM2DPolyline(TRUE);
		mp_LxLyProFile->Recovery(buffer);
	}

	buffer >> m_IsHasRib;
	buffer >> m_IsRibXDirection;

}

void MSSlabG::Serialize(CArchive &ar)
{
	MSCompoG::Serialize(ar);
	bool bEachCompoM = false;
	int count = 0;
	if (ar.IsStoring())
	{
		ar << m_dOffset;
		ar << m_dLxDeg;

		mp_PolylineInfo->Serialize(ar);

		if (mp_CompoM)
		{
			if (mp_CompoM->m_ID == 0)
				bEachCompoM = true;

			ar << bEachCompoM;
			if (bEachCompoM == true)
				Serialize_EachCompoM(ar);
		}

		ResetSlabLevelZone();
		long NumLevelPoly = ma_LevelPoly.size();
		ar << NumLevelPoly;
		for (long iLevelPoly = 0; iLevelPoly < NumLevelPoly; iLevelPoly++)
		{
			ma_LevelPoly[iLevelPoly]->Serialize(ar);
		}
		count = mm_Well.size();
		ar << count;
		map<long, MSWell*>::iterator itWell;
		for (itWell = mm_Well.begin(); itWell != mm_Well.end(); itWell++)
		{
			MSWell* pWell = itWell->second;
			pWell->Serialize(ar);
		}

		count = mm_Trench.size();
		ar << count;
		map<long, MSTrench*>::iterator itTrench;
		for (itTrench = mm_Trench.begin(); itTrench != mm_Trench.end(); itTrench++)
		{
			MSTrench* pTrench = itTrench->second;
			pTrench->Serialize(ar);
		}

		count = ma_EdgeWall.size();
		ar << count;
		for (int i = 0; i < ma_EdgeWall.size(); ++i)
		{
			ma_EdgeWall[i]->Serialize(ar);
		}

		int method = m_nSlopeMethod;
		ar << method;
		int count = 0;
		count = ma_SlopePoints.size();
		ar << count;
		for (int i = 0; i < count; ++i) ma_SlopePoints[i].Serialize(ar);

		count = ma_SlopePointIdx.size();
		ar << count;
		for (int i = 0; i < count; ++i) ar << ma_SlopePointIdx[i];

		count = ma_SlopeZOffset.size();
		ar << count;
		for (int i = 0; i < count; ++i) ar << ma_SlopeZOffset[i];

		m_RibPosition.Serialize(ar);

		ar << m_TopSpacerID;
		ar << m_BotSpacerID;

		GM2DPolyline::SerializeWithNullFlag(ar, mp_InnerProFile);

		count = ma_Haunches .size();
		ar << count;
		for (int i = 0; i < count; ++i)
			ma_Haunches [i]->Serialize(ar);

		count = mm_OpeningID.size();
		ar << count;
		for (auto it = mm_OpeningID.begin(); it != mm_OpeningID.end(); it++)
			ar << (long)it->first;

		GM2DPolyline::SerializeWithNullFlag(ar, mp_BotProfile);
		GM2DPolyline::SerializeWithNullFlag(ar, mp_InnerProFile_Bot);

		ar << m_IsHasRib;
		ar << m_IsRibXDirection;

		GM2DPolyline::SerializeWithNullFlag(ar, mp_LxLyProFile);
	}
	else
	{
		ar >> m_dOffset;
		if(MSVersionInfo::GetCurrentVersion() >= 20140828)
			ar >> m_dLxDeg;
		mp_PolylineInfo->Serialize(ar);
		mp_PolylineInfo->SetOwner(this);

		ar >> bEachCompoM;
		if(bEachCompoM)
			Serialize_EachCompoM(ar);

		if(MSVersionInfo::GetCurrentVersion() >= 20140626)
		{
			long NumLevelPoly;
			ar >> NumLevelPoly;
			for(long iLevelPoly = 0 ; iLevelPoly < NumLevelPoly ; iLevelPoly++)
			{
				MSSlabLevelZone* pSlabLevelZone = new MSSlabLevelZone();
				if(pSlabLevelZone->m_IsDelete) continue;;
				pSlabLevelZone->Serialize(ar);
				if(pSlabLevelZone->mp_Polyline->ma_Point.GetSize() == 0 || pSlabLevelZone->mp_Polyline->CheckPolygon(false) == false)
					delete pSlabLevelZone;
				else
					ma_LevelPoly.push_back(pSlabLevelZone);
			}
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20140627)
		{
			ar >> count;
			for(int i = 0; i < count; i++)
			{
				MSWell* pWall = new MSWell();
				pWall->SetOwner(this);
				pWall->Serialize(ar);
				mm_Well.insert(make_pair(pWall->m_ID, pWall));
			}
			ar >> count;
			for(int i = 0; i < count; i++)
			{
				MSTrench* pTrench = new MSTrench();
				pTrench->SetOwner(this);
				pTrench->Serialize(ar);
				mm_Trench.insert(make_pair(pTrench->m_ID, pTrench));
			}
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20141118)
		{
			ar >> count;
			for(int i = 0; i < count; i++)
			{
				MSLineLevelDifG* pDiff = new MSLineLevelDifG();
				pDiff->Serialize(ar);
				pDiff->SetOwner(this->mp_Owner);
				pDiff->mp_LeftSlab = this;
				ma_EdgeWall.push_back(pDiff);
			}
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20140701)
		{
			int method = 0;
			ar >> method;
			m_nSlopeMethod = (SlopeMethod)method;
			int count = 0;
			ar >> count;
			ma_SlopePoints.resize(count);
			for(int i=0 ; i<count ; ++i) ma_SlopePoints[i].Serialize(ar);

			ar >> count;
			ma_SlopePointIdx.resize(count);
			for(int i=0 ; i<count ; ++i) ar >> ma_SlopePointIdx[i];

			ar >> count;
			ma_SlopeZOffset.resize(count);
			for(int i=0 ; i<count ; ++i) ar >> ma_SlopeZOffset[i];
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20140901)
			m_RibPosition.Serialize(ar);
		if(MSVersionInfo::GetCurrentVersion() >= 20141103)
		{
			ar >> m_TopSpacerID;
			ar >> m_BotSpacerID;
		}
		
		if(MSVersionInfo::GetCurrentVersion() >= 20150416)
			GM2DPolyline::SerializeWithNullFlag(ar, mp_InnerProFile);

		if (MSVersionInfo::GetCurrentVersion() >= 20150724)
		{
			int count = 0;
			ar >> count;
			ma_Haunches .clear();
			for (int i = 0; i < count; ++i)
			{
				MSSubHaunchG* haunch = new MSSubHaunchG();
				haunch->mp_Owner = mp_Owner;
				haunch->Serialize(ar);
				haunch->mp_OwnerCompoG = this;
				ma_Haunches .push_back(haunch);
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20161029)
		{
			int count = mm_OpeningID.size();
			ar >> count;
			for (int i = 0; i < count; i++)
			{
				long nOpeningID = 0;
				ar >> nOpeningID;
				mm_OpeningID.insert(make_pair(nOpeningID, nOpeningID));
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20180511)
		{
			GM2DPolyline::SerializeWithNullFlag(ar, mp_BotProfile);
			GM2DPolyline::SerializeWithNullFlag(ar, mp_InnerProFile_Bot);
		}

		if (MSVersionInfo::GetCurrentVersion() < 20181127)
		{
			SubMemberMaterialValidCheck();
		}

		VerifyAndCureBoundaries(DTOL_MODELING);
		if(mp_InnerProFile->ma_Point.GetSize() == 0)
			mp_InnerProFile->CopyFrom(GetProfile());

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20190822)
		{
			ar >> m_IsHasRib;
			ar >> m_IsRibXDirection;
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20221205)
			GM2DPolyline::SerializeWithNullFlag(ar, mp_LxLyProFile);

		CheckHaunches();
	}
}

void MSSlabG::Serialize_EachCompoM_Rebar(CArchive& ar)
{
	if (ar.IsStoring())
	{
		MSRcFaceMemberM* pLineM = (MSRcFaceMemberM*)mp_CompoM;
		long MType = pLineM->m_nType;
		ar << MType;
		mp_CompoM->Serialize(ar);
	}
	else
	{
		if (MSVersionInfo::GetCurrentVersion() < 20220321)
		{
			MSBaseBuilding* pBldg = GetBuilding();
			long MType = 0;
			ar >> MType;

			MSRcFaceMemberM* pLineM = (MSRcFaceMemberM*)pBldg->CreateNewCompoM(MType);
			pLineM->m_nType = (MSCompoM::Type_MSCompoM)MType;

			long nMaterialID = 0;
			ar >> nMaterialID;
			pLineM->SetMaterial(pBldg->GetMaterial(nMaterialID));
			ar >> pLineM->m_Name;

			double dWidth = 0, dDepth = 0;
			ar >> dWidth;
			ar >> dDepth;

			pLineM->SetWidth(dWidth);
			pLineM->SetDepth(dDepth);

			pLineM->mp_Owner = pBldg;
			mp_CompoM = pLineM;
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

MSObject* MSSlabG::Copy( bool bNewID /*= true*/ )
{
	MSSlabG* pNewObj;

	pNewObj = new MSSlabG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSSlabG::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;
	if( !pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() != msSlabG) { ASSERT(0); return; }
	MSCompoG::CopyFromMe(pSource_in, bNewID);

	MSSlabG* pSource = (MSSlabG*)pSource_in;

	m_dLxDeg = pSource->m_dLxDeg;
	m_dOffset = pSource->m_dOffset;

	m_nSlopeMethod = pSource->m_nSlopeMethod;
	ma_SlopePoints = pSource->ma_SlopePoints;
	ma_SlopePointIdx = pSource->ma_SlopePointIdx;
	ma_SlopeZOffset = pSource->ma_SlopeZOffset;

	long NumSlabZone = pSource->ma_LevelPoly.size();
	for(long iSlabZone = 0 ; iSlabZone < NumSlabZone ; iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = pSource->ma_LevelPoly[iSlabZone];
		MSSlabLevelZone* pNewSlabZone = NULL;
		pNewSlabZone = (MSSlabLevelZone*)pSlabZone->Copy();
		pNewSlabZone->m_IsDelete = true;
		ma_LevelPoly.push_back(pNewSlabZone);
	}
	DeleteWells();
	DeleteTrenchs();
	DeleteEdgeWalls();
	map<long, MSWell*>::iterator itWell;
	for(itWell = pSource->mm_Well.begin(); itWell != pSource->mm_Well.end(); itWell++)
	{
		MSWell* pNewWell = new MSWell();
		MSWell* pSourceWell = itWell->second;
		pNewWell->CopyFromMe(pSourceWell, bNewID);
		pNewWell->SetOwner(this);
		mm_Well.insert(make_pair(pNewWell->GetID(), pNewWell));
	}

	map<long, MSTrench*>::iterator itTrench;
	for(itTrench = pSource->mm_Trench.begin(); itTrench != pSource->mm_Trench.end(); itTrench++)
	{
		MSTrench* pNewTrench = new MSTrench();
		MSTrench* pSourceTrench = itTrench->second;
		pNewTrench->CopyFromMe(pSourceTrench, bNewID);
		pNewTrench->SetOwner(this);
		mm_Trench.insert(make_pair(pNewTrench->GetID(), pNewTrench));
	}

	for (int i=0 ; i<pSource->ma_EdgeWall.size() ; ++i)
	{
		MSLineLevelDifG* pNewDiff = new MSLineLevelDifG();
		pNewDiff->CopyFromMe(pSource->ma_EdgeWall[i], bNewID);
		pNewDiff->SetOwner(this->mp_Owner);
		pNewDiff->mp_LeftSlab = this;
		ma_EdgeWall.push_back(pNewDiff);
	}

	m_RibPosition.CopyFrom(&pSource->m_RibPosition);

	m_TopSpacerID = pSource->m_TopSpacerID;
	m_BotSpacerID = pSource->m_BotSpacerID;

	for (int i = 0; i < ma_Haunches .size(); ++i)
		delete ma_Haunches [i];
	ma_Haunches .clear();
	for (int i = 0; i < pSource->ma_Haunches .size(); ++i)
	{
		MSSubHaunchG* haunch = new MSSubHaunchG();
		haunch->mp_Owner = mp_Owner;
		haunch->CopyFromMe(pSource->ma_Haunches [i], bNewID);
		haunch->mp_OwnerCompoG = this;
		ma_Haunches .push_back(haunch);
	}

	mp_InnerProFile->CopyFrom(pSource->mp_InnerProFile);
	mp_BotProfile->CopyFrom(pSource->mp_BotProfile);

	SetIsHasRib(pSource->GetIsHasRib());
	m_IsRibXDirection = pSource->GetIsRibXDirection();

	if (AppGlobalOptionWrapper::Instance()->GetAppMode() == AppGlobalOptionWrapper::Formwork)
	{
//		DeleteSoundAbsortpions();
//		for (auto it = pSource->mm_SoundAbsorptionG.begin(); it != pSource->mm_SoundAbsorptionG.end(); it++)
//		{
//			MSFWSoundAbsorptionG* pSound = new MSFWSoundAbsorptionG();
//			pSound->mp_Owner = mp_Owner;
//			pSound->CopyFromMe(it->second, bNewID);
//			pSound->SetOwnerCompoG(this);
//			AddSoundAbsorption(pSound);
//		}
	}

}

void MSSlabG::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSCompoG::Init(bInConstructor);

	m_HasOverLapSlopeSlab = false;

	m_dOffset = 0.;
	m_dLxDeg = 0.;

	mp_PolylineInfo = new MSPolylineObject();
	mp_PolylineInfo->SetOwner(this);

	m_nSlopeMethod = SlopeMethod::ThreePoint_0;
	ma_SlopePoints.clear();
	ma_SlopePointIdx.clear();
	ma_SlopeZOffset.clear();



	DeleteWells();
	DeleteTrenchs();
	DeleteEdgeWalls();
	m_RibPosition.SetXY(0, 0);

	m_TopSpacerID = 0;
	m_BotSpacerID = 0;

	mp_InnerProFile = new GM2DPolyline(TRUE);
	mp_BotProfile = new GM2DPolyline(TRUE);

	mp_InnerProFile_Bot = new GM2DPolyline(TRUE);
	mp_DeckSlabQTProFile = new GM2DPolyline(TRUE);

	mp_InnerProFile = new GM2DPolyline(TRUE);
	mp_LxLyProFile = new GM2DPolyline(TRUE);
	m_IsHasRib = true;
	m_IsRibXDirection = true;

}

void MSSlabG::RemoveSubMemberConnections()
{
	for (int i = 0; i < ma_EdgeWall.size(); i++)
		ma_EdgeWall[i]->RemoveConnections();

	for (int i = 0; i < ma_LevelDif.size(); i++)
		ma_LevelDif[i]->RemoveConnections();

	for (int i = 0; i < ma_Haunches .size(); i++)
		ma_Haunches [i]->RemoveConnections();
}

bool MSSlabG::CheckProfile()
{
	bool error = false;
	// ToDo: Profile Duplicate Check Code
	// ...

	GM2DPolyline* pSlabPoly = GetProfile();
	if(pSlabPoly == NULL)
		error = true;
	else
		error = pSlabPoly->CheckVecterArr(false);

	if (error)
	{
		CString groupName(_T("Slab"));
		CString  msg = LocalDic::GetLocal(_T("에러 메시지"), _T("CheckData"));
		mp_MsgObject = NULL;
		AddErrorObject(0, -1, groupName, msg);
		return false;
	}

	return true;
}

GM2DPolyline* MSSlabG::GetProfile()
{
	return mp_Boundary;
}

void MSSlabG::SetProfile(GM2DPolyline* pPoly)
{
	if(mp_Boundary != pPoly)
		mp_Boundary->CopyFrom(pPoly);	// Profile, OrgProfile에서 절점만 생성되는 프로파일, Hidden 라인 표현할때 사용
	mp_PolylineInfo->SetProfile(pPoly);	// OrgProfile: 사용자가 입력한 폴리라인
	SetTrimmedBoundary_Acis(pPoly);		// mp_PolylineInfo->mp_Profile 와 같도록 셋팅

	InitTrimmedBoundary(true);	// TopTrimmedProfile
	InitTrimmedBoundary(false);	// BotTrimmedProfile
}

void MSSlabG::GetProfile_InSplitPointLevelZone(GM2DPolyline& ThePoly)
{
	if (mp_PolylineInfo == NULL)
	{        
		ASSERT(0);
		return ;
	}
	GM2DPolyline* pProfile = GetProfile();
	if (pProfile == NULL)
		return;

	ThePoly.CopyFrom(pProfile);
	
	for(long i = 0 ; i < ma_LevelPoly.size() ; i++)
	{
		MSSlabLevelZone* pSlabLevelZone = ma_LevelPoly[i];
		if(pSlabLevelZone == NULL) 
			continue;

		ThePoly.SegmentstoMe(pSlabLevelZone->mp_Polyline);
	}
}

double MSSlabG::GetStartLevel()
{
	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;
	return GetNextSlabLevel()+GetRelativeStartLevel();
}

double MSSlabG::GetRelativeStartLevel()
{
	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;
	MSFloor* pFloor = (MSFloor*)mp_Owner;
	GM2DPolyline* poly =  GetProfile();
	if(poly->GetNumLines()<=0)
		return 0;
	return pFloor->GetLevel4LevelZone(poly->ma_Point[0]->m_X, poly->ma_Point[0]->m_Y)+m_dOffset;
}

long MSSlabG::GetSlabMType()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
		return MSRcSlabM::SLAB_TYPE::GENERAL_TYPE;

	return mySlabM->GetSlabMType();	
}

bool MSSlabG::IsFlatSlabBType()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
		return false;

	if 	(mySlabM->m_nSlabMType == MSRcSlabM::FLAT_TYPE &&
		(mySlabM->m_ShortStrip.m_StripType == MSRcStrip::TYPE_B ||
		mySlabM->m_LongStrip.m_StripType == MSRcStrip::TYPE_B))
		return true;

	return false;

}

double MSSlabG::GetThick()
{
	MSCompoM* pCompoM = GetCompoM();
	if (pCompoM == NULL)
	{
		AfxThrowUserException();
		return 0.0;
	}

	return pCompoM->GetThick();	
}

MSWell* MSSlabG::GetWell(long nID)
{
	map<long, MSWell*>::iterator it = mm_Well.find(nID);
	if(it != mm_Well.end())
		return it->second;
	return NULL;
}

void MSSlabG::DeleteWells()
{
	map<long, MSWell*>::iterator it;
	for(it = mm_Well.begin(); it != mm_Well.end(); it++)
		delete it->second;
	mm_Well.clear();
}

MSTrench* MSSlabG::GetTrench(long nID)
{
	map<long, MSTrench*>::iterator it = mm_Trench.find(nID);
	if(it != mm_Trench.end())
		return it->second;
	return NULL;
}

bool MSSlabG::Remove(MSTrench* pTrench)
{
	auto it = mm_Trench.find(pTrench->m_ID);
	if (it == mm_Trench.end())	return false;
	mm_Trench.erase(it);
	return true;
}
bool MSSlabG::Delete(MSTrench* pTrench)
{
	bool rtnVal = Remove(pTrench);
	delete pTrench;
	return rtnVal;
}

void MSSlabG::DeleteTrenchs()
{
	map<long, MSTrench*>::iterator it;
	for(it = mm_Trench.begin(); it != mm_Trench.end(); it++)
		delete it->second;
	mm_Trench.clear();
}

void MSSlabG::DeleteEdgeWalls()
{
	for (int i=0 ; i<ma_EdgeWall.size() ; ++i)
		delete ma_EdgeWall[i];
	ma_EdgeWall.clear();
}

void MSSlabG::CalcInitialLxDegFromProfile_SlabRebar(GM2DPolyline& profile_in, double& deg, GM2DVector& rib_pos)
{
	MSSlabG::CalcInitialLxDegFromProfile(profile_in, deg, rib_pos);
}

void MSSlabG::CalcInitialLxDegFromProfile(GM2DPolyline& profile_in, double& deg, GM2DVector& rib_pos)
{
	GM2DPolyline* poly = &profile_in;
	vector<double> angle_list, leng_list;
	// 각 커브의 길이와 각도를 구한다. (직선으로 간주)
	for (int i = 0; i < poly->ma_Point.GetSize() - 1; ++i)
	{
		double dx = poly->ma_Point[i + 1]->m_X - poly->ma_Point[i]->m_X;
		double dy = poly->ma_Point[i + 1]->m_Y - poly->ma_Point[i]->m_Y;
		double leng = dx*dx + dy*dy;
		double cur_deg = atan2(dy, dx) * (180.0 / PI);

		if (cur_deg >= 360.0) cur_deg -= 360.0;
		if (cur_deg < 0.0) cur_deg += 360.0;
		if (cur_deg >= 180.0) cur_deg -= 180.0;

		angle_list.push_back(cur_deg);
		leng_list.push_back(leng);
	}

	int max_idx = -1;
	double max_leng = 0;

	// 차이가 90도인 커브들중 가장 길이가 긴 쌍을 구한다.
	double ANG_TOL = 1.0;	
	for (int i = 0; i < angle_list.size(); ++i)
	{
		int i2 = (i + 1) % (angle_list.size());
		double diff = fabs(angle_list[i2] - angle_list[i]);
		if (fabs(90.0 - diff) < ANG_TOL)
		{
			double cur_leng = __max(leng_list[i], leng_list[i2]);
			if (max_idx == -1 || cur_leng > max_leng)
			{
				max_leng = cur_leng;
				max_idx = i;
			}
		}
	}

	// 최종 Edge 를 구한다. (찾은 경우는 둘 중 짧은 엣지, 못찾은 경우는 가장 짧은 엣지)
	int target_idx = -1;
	if (max_idx > -1)
	{
		int i1 = max_idx;
		int i2 = (max_idx + 1) % (leng_list.size());
		target_idx = i1;
		if (leng_list[i2] < leng_list[i1])
			target_idx = i2;
	}
	else
	{
		double min_leng = 0;
		for (int i = 0; i < leng_list.size(); ++i)
		{
			if (target_idx == -1 || leng_list[i] < min_leng)
			{
				min_leng = leng_list[i];
				target_idx = i;
			}
		}
	}

	deg = angle_list[target_idx];
	deg = RoundOff5(deg);
	if (deg >= 180.0) deg = 0.0;

	//////////////////////////////////////////////////////
	double dist_min = 0;
	double radian = deg * PI / 180.0;
	if (radian >= PI) radian -= PI;
	GM2DLineVector line(0, 0, cos(radian), sin(radian));
	for (int i=0 ; i<poly->ma_Point.GetSize()-1 ; ++i)
	{
		GM2DVector& cur = *(poly->ma_Point[i]);
		//bool right = (line.WhichSide(cur) == -1);
		double dist = -line.Distance(cur, true);
		if (i == 0 || dist < dist_min)
		{
			rib_pos = cur;
			dist_min = dist;
		}
	}
	//////////////////////////////////////////////////////
}

bool MSSlabG::IsAutoDegreeBySlabType()
{
	if(IsGeneral())	return true;
	
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
		return false;

	if(mySlabM->GetSlabMType() == MSRcSlabM::PC_TYPE || mySlabM->GetSlabMType()==MSRcSlabM::DECK_TYPE || mySlabM->GetSlabMType() == MSRcSlabM::DECK_GENERAL || mySlabM->GetSlabMType() == MSRcSlabM::PC_IMT)
		return true;

	// 무량판 슬래브
	if(mySlabM->GetSlabMType() == MSRcSlabM::FLAT_TYPE)
	{
		if(mySlabM->m_ShortStrip.m_StripType == MSRcStrip::TYPE_B)
			return true;
		if(mySlabM->m_LongStrip.m_StripType == MSRcStrip::TYPE_B)
			return true;
	}
	return false;
}

double MSSlabG::GetInitialLxDegFromProfile(GM2DVector& rib_pos)
{
	double deg = 0.0;

	if (IsAutoDegreeBySlabType())
	{
		MSSlabG::CalcInitialLxDegFromProfile(*(GetProfile()), deg, rib_pos);
		if (IsPCSlabGroup())
		{
			// BH-4522 PC Slab Type 추가
			// IMT PC Slab는 장변/단변 기준이 일반 슬래브랑 반대이므로 각도를 -90으로 돌려준다.
			deg -= 90;
		}

		return deg;
	}
	else
	{
		if (fabs(m_dLxDeg) > DTOL1)
		{
			deg = m_dLxDeg;

			if (deg >= 360.0) deg -= 360.0;
			if (deg < 0.0) deg += 360.0;
			if (deg >= 180.0) deg -= 180.0;
		}
		else
			deg = 0.0;

		deg = RoundOff5(deg);
		if (deg >= 180.0) deg = 0.0;
	}

	return deg;
}

bool MSSlabG::IsMatSlab()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
	{
		AfxThrowUserException();
		return false;
	}

	if(mySlabM->GetSlabMType()==MSRcSlabM::SLAB_TYPE::MAT_TYPE)
		return true;
	return false;
}

bool MSSlabG::IsFlatSlab()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
	{
		AfxThrowUserException();
		return false;
	}

	if(mySlabM->GetSlabMType()==MSRcSlabM::SLAB_TYPE::FLAT_TYPE ||
		mySlabM->GetSlabMType() == MSRcSlabM::SLAB_TYPE::SOG ||
		mySlabM->GetSlabMType() == MSRcSlabM::SLAB_TYPE::CIRCULAR_RAMP)
		return true;
	return false;
}

bool MSSlabG::IsDeckSlab()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
		return MSRcSlabM::SLAB_TYPE::GENERAL_TYPE;

	if(mySlabM->GetSlabMType()==MSRcSlabM::SLAB_TYPE::DECK_TYPE || mySlabM->GetSlabMType() == MSRcSlabM::SLAB_TYPE::DECK_GENERAL)
		return true;
	return false;
}

bool MSSlabG::IsRebarEmbeddedDeckSlab()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
	{
		AfxThrowUserException();
		return false;
	}

	if (mySlabM->GetSlabMType() == MSRcSlabM::SLAB_TYPE::DECK_TYPE)
		return true;
	return false;
}


bool MSSlabG::IsGeneralDeckSlab()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
	{
		AfxThrowUserException();
		return false;
	}

	if (mySlabM->GetSlabMType() == MSRcSlabM::SLAB_TYPE::DECK_GENERAL)
		return true;
	return false;
}

bool MSSlabG::IsPC()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
	{
		AfxThrowUserException();
		return false;
	}

	if(mySlabM->IsPC())
		return true;
	return false;
}

bool MSSlabG::IsHalfPCSlab()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
	{
		AfxThrowUserException();
		return false;
	}

	if (mySlabM->IsHalfPCSlab())
		return true;
	return false;
}

bool MSSlabG::IsPCSlab()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
	{
		AfxThrowUserException();
		return false;
	}

	if (mySlabM->IsPCSlab())
		return true;
	return false;
}

bool MSSlabG::IsIMTPCSlab()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
	{
		AfxThrowUserException();
		return false;
	}

	if (mySlabM->IsIMTPCSlab())
		return true;
	return false;
}

bool MSSlabG::IsHCSPCSlab()
{
	MSRcSlabM* mySlabM = (MSRcSlabM*)GetRebarM();
	if (mySlabM == NULL)
	{
		AfxThrowUserException();
		return false;
	}

	if (mySlabM->IsHCSPCSlab())
		return true;
	return false;
}

bool MSSlabG::IsRPSPCSlab()
{
	MSRcSlabM* mySlabM = (MSRcSlabM*)GetRebarM();
	if (mySlabM == NULL)
	{
		AfxThrowUserException();
		return false;
	}

	if (mySlabM->IsRPSPCSlab())
		return true;
	return false;
}


bool MSSlabG::IsGeneral()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
	{
		AfxThrowUserException();
		return false;
	}

	if(mySlabM->GetSlabMType()==MSRcSlabM::SLAB_TYPE::GENERAL_TYPE)
		return true;
	return false;
}

bool MSSlabG::IsSOG()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
	{
		AfxThrowUserException();
		return false;
	}

	if (mySlabM->GetSlabMType() == MSRcSlabM::SLAB_TYPE::SOG)
		return true;
	return false;
}

bool MSSlabG::IsCircularRamp()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
	{
		AfxThrowUserException();
		return false;
	}

	if (mySlabM->GetSlabMType() == MSRcSlabM::SLAB_TYPE::CIRCULAR_RAMP)
		return true;
	return false;
}

bool MSSlabG::IsSlabGroup()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
	{
		AfxThrowUserException();
		return false;
	}

	if(mySlabM->GetSlabMType()==MSRcSlabM::SLAB_TYPE::GENERAL_TYPE)
		return true;
	if(mySlabM->GetSlabMType() == MSRcSlabM::FLAT_TYPE)
	{
		if(mySlabM->m_ShortStrip.m_StripType == MSRcStrip::TYPE_B)
			return true;
		if(mySlabM->m_LongStrip.m_StripType == MSRcStrip::TYPE_B)
			return true;
	}
	return false;
}
bool MSSlabG::IsFlatSlabGroup()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
	{
		AfxThrowUserException();
		return false;
	}
	if(mySlabM->GetSlabMType() == MSRcSlabM::TRANSFER)
		return true;
	else if(mySlabM->GetSlabMType() == MSRcSlabM::FLAT_TYPE)
	{
		if( (mySlabM->m_ShortStrip.m_StripType == MSRcStrip::TYPE_A || mySlabM->m_ShortStrip.m_StripType == MSRcStrip::TYPE_ALL)
			&& (mySlabM->m_LongStrip.m_StripType == MSRcStrip::TYPE_A || mySlabM->m_LongStrip.m_StripType == MSRcStrip::TYPE_ALL ))
			return true;
	}
	return false;
}
bool MSSlabG::IsCircularRampGroup()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
	{
		AfxThrowUserException();
		return false;
	}
	if(mySlabM->GetSlabMType() == MSRcSlabM::CIRCULAR_RAMP)
		return true;
	return false;
}
bool MSSlabG::IsFoundationSlabGroup()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
	{
		AfxThrowUserException();
		return false;
	}
	if(mySlabM->GetSlabMType() == MSRcSlabM::MAT_TYPE || mySlabM->GetSlabMType() == MSRcSlabM::SOG)
		return true;

	return false;
}
bool MSSlabG::IsDeckSlabGroup()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
	{
		AfxThrowUserException();
		return false;
	}

	if(mySlabM->GetSlabMType()==MSRcSlabM::SLAB_TYPE::DECK_TYPE || mySlabM->GetSlabMType() == MSRcSlabM::SLAB_TYPE::DECK_GENERAL)
		return true;
	return false;
}
bool MSSlabG::IsPCSlabGroup()
{
	long slabMType = GetSlabMType();
	if(slabMType == MSRcSlabM::PC_TYPE
		|| slabMType == MSRcSlabM::PC_IMT
		|| slabMType == MSRcSlabM::PC_RPS
		|| slabMType == MSRcSlabM::PC_HCS)
		return true;

	return false;
}
void MSSlabG::DeleteSupport(bool isCallDestructors)
{
	long nSpt = ma_EdgeSpt.GetSize();
	for(long iSpt=0; iSpt<nSpt; iSpt++)
	{
		MSSlabSptMemPart* pSlabPart = ma_EdgeSpt[iSpt];
		if(pSlabPart == NULL) continue;

		delete ma_EdgeSpt[iSpt];
	}
	ma_EdgeSpt.RemoveAll();
}


/*
long MSSlabG::CheckCrossThenAddLevelDif( MSLevelZone* pZone)
{
GM2DPolyline pSlabPoly(true);
GetPolyline(pSlabPoly);
CTypedPtrArray<CObArray, GM2DPolyline*> LevelPolyArr;
pSlabPoly.CTypedPtrArray<CObArray, eration('I', &(pZone->m_Profile), LevelPolyArr, 1.);
long nLevelPoly = LevelPolyArr.GetSize();
for(long iLevelPoly=0; iLevelPoly<nLevelPoly; iLevelPoly++)
{
GM2DPolyline* pPoly = LevelPolyArr[iLevelPoly];
ma_LevelPoly.push_back(pPoly);
long nEdge = pPoly->GetSize();
for(long iEdge=0; iEdge<nEdge; iEdge++)
{
GM2DCurve* pEdge = pPoly->GetCurve(iEdge);
GM2DVector MidPoint = pEdge->Point2DVector(0.5);
if(pSlabPoly.Contains(&MidPoint)==-1)
{
MSSlabLevelDif* pSlabLevelDif = new MSSlabLevelDif;
pSlabLevelDif->mp_LevelZone = pZone;
pSlabLevelDif->mp_Curve = pEdge;
pSlabLevelDif->m_LeftLevel = pZone->m_Level;
pSlabLevelDif->m_RightLevel = m_dOffset;
pSlabLevelDif->m_InfillThick = GetThick()*1.5;
AddSlabLevelDif(pSlabLevelDif);
}
else
delete pEdge;
}
}
return 0;
}
*/

bool MSSlabG::AddLevelDif(MSSlabLineLevelDifG* pDif)
{
	if (pDif->GetLevelDifWidth() < 1) return false;

	for(long i = 0 ; i < ma_LevelDif.size(); i++)
	{
		MSSlabLineLevelDifG* pSlabDif = (MSSlabLineLevelDifG*)ma_LevelDif[i];
		if (pSlabDif->isSameDif(pDif))
		{
			if (pSlabDif->mp_LeftLevelZone == NULL)
				pSlabDif->mp_LeftLevelZone = pDif->mp_LeftLevelZone;

			if (pSlabDif->mp_RightLevelZone == NULL)
				pSlabDif->mp_RightLevelZone = pDif->mp_RightLevelZone;

			return false;
		}
	}

	ma_LevelDif.push_back(pDif);

	return true;
}

void MSSlabG::CheckCrossThenAddLevelDifStep2(GM2DCurve* pCurve,  MSLineLevelDifG* pLevelDif, MSLevelZone* pZone)
{
	GM2DPolyline* pSlabPoly = GetProfile();

	double dTol = 1;
	long Result = pSlabPoly->OverlapEx(pCurve);
	if(Result == 0)
		return;

	MSLevelZone* pLevelZone = pZone;

	if(pLevelZone != nullptr && pLevelZone->m_UseDefaultWidth && pLevelZone->m_Width >= 0)
	{
		double dWidth = 0;
		if(GetLevelDifWidth(pLevelDif, dWidth) && fabs(dWidth - pZone->m_PreWidth)> DTOL_GM)	//pZone->Width 변경
			pLevelDif->m_dWallWidth = dWidth;
		else if(pZone->m_PreWidth  < DTOL_GM)				// 최초 Width 값 셋팅
			pLevelDif->m_dWallWidth = pLevelZone->m_Width;
		// 나머지는 기존값 유지
	}
	else if(pLevelZone != nullptr && pLevelZone->m_Width < 0)
		pLevelDif->m_dWallWidth = 0;
	else
		pLevelDif->m_dWallWidth = GetThick() * pLevelDif->m_InfillSlabFactor;

	GM2DCurve* pDifCurve= pLevelDif->GetCurve();
	if (pDifCurve != nullptr)
	{
		long nContain = Contains(&pCurve->PointAtParam(0.5), dTol);
		if (nContain == -1)
		{
			MSSlabLineLevelDifG* pSlabLineDif = new MSSlabLineLevelDifG(pLevelDif, this);
			double dWidth = 0;
			pSlabLineDif->SetLoc(pDifCurve->OrthoBaseLocValue(pCurve->m_P0), pDifCurve->OrthoBaseLocValue(pCurve->m_P1));
			if (GetLevelDifWidth(pSlabLineDif, dWidth))
				pSlabLineDif->m_dWallWidth = dWidth;

			pSlabLineDif->SetLines(msSlabG, pSlabLineDif->m_dWallWidth);
			if (AddLevelDif(pSlabLineDif) == false)
				delete pSlabLineDif;
		}
	}
	return;
/*

	if(Result==-1)	// 폴리곤 안에 있는 선
	{

		if( pLevelDif->GetCurve()->m_P0.SameVector(pCurve->GetP0()) && pLevelDif->GetCurve()->m_P1.SameVector(pCurve->GetP1()) && pLevelDif->GetCurve()->GetBulge() == pCurve->GetBulge())
		{
			pLevelDif->SetCurve(pCurve->Copy());
			
			if(pLevelDif->isArc())
			{
				GM2DArc* pArc = (GM2DArc*)pLevelDif->GetCurve();
				pLevelDif->ma_Vec[0]->SetXY(pCurve->GetP0().m_X, pCurve->GetP0().m_Y);
				pLevelDif->ma_Vec[1]->SetXY(pCurve->GetP1().m_X, pCurve->GetP1().m_Y);
			}
			else
			{
				pLevelDif->ma_Vec[0]->SetXY(pCurve->GetP0().m_X, pCurve->GetP0().m_Y);
				pLevelDif->ma_Vec[1]->SetXY(pCurve->GetP1().m_X, pCurve->GetP1().m_Y);
			}
			pLevelDif->mp_LeftSlab = pLevelDif->mp_RightSlab= this;
			
			if(!HasLevelDif(pLevelDif))
				ma_LevelDif.push_back(pLevelDif);
			return;
		}
		else if( pLevelDif->GetCurve()->m_P0.SameVector(pCurve->GetP1()) && pLevelDif->GetCurve()->m_P1.SameVector(pCurve->GetP0()) && pLevelDif->GetCurve()->GetBulge() == pCurve->GetBulge())
		{
			pLevelDif->SetCurve(pCurve->Copy());
			if(pLevelDif->isArc())
			{
				GM2DArc* pArc = (GM2DArc*)pLevelDif->GetCurve();
				pLevelDif->ma_Vec[0]->SetXY(pCurve->GetP0().m_X, pCurve->GetP0().m_Y);
				pLevelDif->ma_Vec[1]->SetXY(pCurve->GetP1().m_X, pCurve->GetP1().m_Y);
			}
			else
			{
				pLevelDif->ma_Vec[0]->SetXY(pCurve->GetP0().m_X, pCurve->GetP0().m_Y);
				pLevelDif->ma_Vec[1]->SetXY(pCurve->GetP1().m_X, pCurve->GetP1().m_Y);
			}					
			pLevelDif->mp_LeftSlab = pLevelDif->mp_RightSlab= this;
			
			if(!HasLevelDif(pLevelDif))
				ma_LevelDif.push_back(pLevelDif);
			return;
		}
		else
		{
			GM2DCurve* pCenCurve = pLevelDif->GetCurve();

			double dLoc0 = pCenCurve->OrthoBaseLocValue(pCurve->m_P0);
			double dLoc1 = pCenCurve->OrthoBaseLocValue(pCurve->m_P1);
			GM2DVector Vec0 = pCenCurve->PointAtParam(dLoc0);
			GM2DVector Vec1 = pCenCurve->PointAtParam(dLoc1);

			if(dLoc0 > 0 + DTOL_GM)
			{
				MSLineLevelDifG* pLevelDifInSlab = (MSLineLevelDifG*)pLevelDif->Copy(false);
				pLevelDifInSlab->GetCurve()->SetP1(Vec0);
				pLevelDifInSlab->ma_Vec[1]->SetXY(Vec0.m_X, Vec0.m_Y);
				if(pLevelDif->mp_LeftLevelZone != NULL)
					pLevelDif->mp_LeftLevelZone->AddLevelDifG(pLevelDifInSlab);
				if(pLevelDif->mp_RightLevelZone != NULL)
					pLevelDif->mp_RightLevelZone->AddLevelDifG(pLevelDifInSlab);
			}

			if(dLoc1 < 1.0 - DTOL_GM)
			{
				MSLineLevelDifG* pLevelDifInSlab = (MSLineLevelDifG*)pLevelDif->Copy(false);
				pLevelDifInSlab->GetCurve()->SetP0(Vec1);
				pLevelDifInSlab->ma_Vec[0]->SetXY(Vec1.m_X, Vec1.m_Y);
				if(pLevelDif->mp_LeftLevelZone != NULL)
					pLevelDif->mp_LeftLevelZone->AddLevelDifG(pLevelDifInSlab);
				if(pLevelDif->mp_RightLevelZone != NULL)
					pLevelDif->mp_RightLevelZone->AddLevelDifG(pLevelDifInSlab);
			}

			pCenCurve->SetP0(Vec0);
			pLevelDif->ma_Vec[0]->SetXY(Vec0.m_X, Vec0.m_Y);
			pCenCurve->SetP1(Vec1);
			pLevelDif->ma_Vec[1]->SetXY(Vec1.m_X, Vec1.m_Y);
			pLevelDif->mp_LeftSlab = pLevelDif->mp_RightSlab = this;
			
			if(!HasLevelDif(pLevelDif))
				ma_LevelDif.push_back(pLevelDif);

// 			MSLineLevelDifG* pLevelDifInSlab = (MSLineLevelDifG*)pLevelDif->Copy(false);
// 			if(pLevelDifInSlab->mp_Curve != NULL)
// 				delete pLevelDifInSlab->mp_Curve;
// 			pLevelDifInSlab->mp_Curve = pCurve->Copy();
// 			if(pLevelDifInSlab->isArc())
// 			{
// 				GM2DArc* pArc = (GM2DArc*)pLevelDifInSlab->mp_Curve;
// 				pLevelDifInSlab->ma_Vec[0]->SetXY(pCurve->GetP0().m_X, pCurve->GetP0().m_Y);
// 				pLevelDifInSlab->ma_Vec[1]->SetXY(pCurve->GetP1().m_X, pCurve->GetP1().m_Y);
// 			}
// 			else
// 			{
// 				pLevelDifInSlab->ma_Vec[0]->SetXY(pCurve->GetP0().m_X, pCurve->GetP0().m_Y);
// 				pLevelDifInSlab->ma_Vec[1]->SetXY(pCurve->GetP1().m_X, pCurve->GetP1().m_Y);
// 			}
// 			pLevelDifInSlab->mp_LeftSlab = pLevelDifInSlab->mp_RightSlab= this;
// 			ma_LevelDif.push_back(pLevelDifInSlab);
// 			pLevelDif->mp_LeftLevelZone->AddLevelDifG(pLevelDifInSlab);
		}

	}
	else if(Result==0)	// 폴리곤 선상에 있는 선
	{
		//방향성 체크하기위해 슬래브 라인을 찾음
		GM2DCurve* SlabLine = NULL;
		long NumPolyLine = pSlabPoly->GetNumLines();
		for(long iLine = 0 ; iLine < NumPolyLine ; iLine++)
		{
			GM2DCurve* pCurve = pSlabPoly->GetCurve(iLine);
			if(pCurve->WhichSide(pLevelDif->GetCurve()->PointAtParam(0.5)) == 0)
			{
				SlabLine = pCurve;
				break;
			}
			delete pCurve;
		}

		GM2DCurve* pCenCurve = pLevelDif->GetCurve();
		GM2DCurve* pUrCurve;
		GM2DLineVector SegmentLine;
		if(SlabLine != NULL)
		{
			SegmentLine.m_P0 = SlabLine->m_P0;
			SegmentLine.m_P1 = SlabLine->m_P1;
			pUrCurve = &SegmentLine;
			delete SlabLine;
		}
		else
		{
			pUrCurve = pCurve;
		}

		long nRval = pCenCurve->IsParallel4DTol(pUrCurve);
		if(nRval == 1) // 같은 방향
		{
			// 선을 짤라주고 
			double dLoc0 = pCenCurve->OrthoBaseLocValue(pCurve->m_P0);
			double dLoc1 = pCenCurve->OrthoBaseLocValue(pCurve->m_P1);
			if( !((0 - DTOL_GM < dLoc0 && dLoc0 < 1 + DTOL_GM) &&
				(0 - DTOL_GM < dLoc1 && dLoc1 < 1 + DTOL_GM))
				)
				return;
// 			if( (dLoc0 < 0 - DTOL_GM && dLoc1 < 0 - DTOL_GM) ||
// 				(dLoc0 > 1 + DTOL_GM && dLoc1 > 1 + DTOL_GM)
// 				)
// 				return;

			GM2DVector Vec0 = pCenCurve->PointAtParam(dLoc0);
			GM2DVector Vec1 = pCenCurve->PointAtParam(dLoc1);

			if(dLoc0 > 0 + DTOL_GM)
			{
				MSLineLevelDifG* pLevelDifInSlab = (MSLineLevelDifG*)pLevelDif->Copy(false);
				pLevelDifInSlab->GetCurve()->SetP1(Vec0);
				pLevelDifInSlab->ma_Vec[1]->SetXY(Vec0.m_X, Vec0.m_Y);
				if(pLevelDif->mp_LeftLevelZone != NULL)
					pLevelDif->mp_LeftLevelZone->AddLevelDifG(pLevelDifInSlab);
				if(pLevelDif->mp_RightLevelZone != NULL)
					pLevelDif->mp_RightLevelZone->AddLevelDifG(pLevelDifInSlab);
			}

			if(dLoc1 < 1.0 - DTOL_GM)
			{
				MSLineLevelDifG* pLevelDifInSlab = (MSLineLevelDifG*)pLevelDif->Copy(false);
				pLevelDifInSlab->GetCurve()->SetP0(Vec1);
				pLevelDifInSlab->ma_Vec[0]->SetXY(Vec1.m_X, Vec1.m_Y);
				if(pLevelDif->mp_LeftLevelZone != NULL)
					pLevelDif->mp_LeftLevelZone->AddLevelDifG(pLevelDifInSlab);
				if(pLevelDif->mp_RightLevelZone != NULL)
					pLevelDif->mp_RightLevelZone->AddLevelDifG(pLevelDifInSlab);
			}

			pCenCurve->SetP0(Vec0);
			pLevelDif->ma_Vec[0]->SetXY(Vec0.m_X, Vec0.m_Y);
			pCenCurve->SetP1(Vec1);
			pLevelDif->ma_Vec[1]->SetXY(Vec1.m_X, Vec1.m_Y);

			GM2DVector SlabCenVec = pSlabPoly->MassCenter();
			long nWSVal = pCenCurve->WhichSide(SlabCenVec);
			if(nWSVal == -1)
				pLevelDif->mp_RightSlab = this;
			else if(nWSVal == 1)
				pLevelDif->mp_LeftSlab = this;
			
// 			if(!HasLevelDif(pLevelDif))
// 				ma_LevelDifOnEdge.push_back(pLevelDif);

			// 짤린 선중에 래밸존의 라인에 포함된 놈에게 왼쪽 슬라브 셋팅해줌, 래밸존에게 기존선 줄이고 짤린선 추가 시켜야함
		}
		else if(nRval == -1) // 다른 방향
		{
			// 선을 짤라주고 
			double dLoc0 = pCenCurve->OrthoBaseLocValue(pCurve->m_P0);
			double dLoc1 = pCenCurve->OrthoBaseLocValue(pCurve->m_P1);
			if( !((0 - DTOL_GM < dLoc0 && dLoc0 < 1 + DTOL_GM) &&
				(0 - DTOL_GM < dLoc1 && dLoc1 < 1 + DTOL_GM))
				)
				return;
// 			if( (dLoc0 < 0 - DTOL_GM && dLoc1 < 0 - DTOL_GM) ||
// 				(dLoc0 > 1 + DTOL_GM && dLoc1 > 1 + DTOL_GM)
// 				)
// 				return;

			GM2DVector Vec0 = pCenCurve->PointAtParam(dLoc0);
			GM2DVector Vec1 = pCenCurve->PointAtParam(dLoc1);

			if(dLoc0 > 0 + DTOL_GM)
			{
				MSLineLevelDifG* pLevelDifInSlab = (MSLineLevelDifG*)pLevelDif->Copy(false);
				pLevelDifInSlab->GetCurve()->SetP1(Vec0);
				pLevelDifInSlab->ma_Vec[1]->SetXY(Vec0.m_X, Vec0.m_Y);
				if(pLevelDif->mp_LeftLevelZone != NULL)
					pLevelDif->mp_LeftLevelZone->AddLevelDifG(pLevelDifInSlab);
				if(pLevelDif->mp_RightLevelZone != NULL)
					pLevelDif->mp_RightLevelZone->AddLevelDifG(pLevelDifInSlab);
			}

			if(dLoc1 < 1.0 - DTOL_GM)
			{
				MSLineLevelDifG* pLevelDifInSlab = (MSLineLevelDifG*)pLevelDif->Copy(false);
				pLevelDifInSlab->GetCurve()->SetP0(Vec1);
				pLevelDifInSlab->ma_Vec[0]->SetXY(Vec1.m_X, Vec1.m_Y);
				if(pLevelDif->mp_LeftLevelZone != NULL)
					pLevelDif->mp_LeftLevelZone->AddLevelDifG(pLevelDifInSlab);
				if(pLevelDif->mp_RightLevelZone != NULL)
					pLevelDif->mp_RightLevelZone->AddLevelDifG(pLevelDifInSlab);
			}

			pCenCurve->SetP0(Vec0);
			pLevelDif->ma_Vec[0]->SetXY(Vec0.m_X, Vec0.m_Y);
			pCenCurve->SetP1(Vec1);
			pLevelDif->ma_Vec[1]->SetXY(Vec1.m_X, Vec1.m_Y);

// 			if(!HasLevelDif(pLevelDif))
// 				ma_LevelDifOnEdge.push_back(pLevelDif);
			
			GM2DVector SlabCenVec = pSlabPoly->MassCenter();
			long nWSVal = pCenCurve->WhichSide(SlabCenVec);
			if(nWSVal == -1)
				pLevelDif->mp_RightSlab = this;
			else if(nWSVal == 1)
				pLevelDif->mp_LeftSlab = this;

			// 짤린 선중에 래밸존의 라인에 포함된 놈에게 오른쪽 슬라브 셋팅해줌, 래밸존에게 기존선 줄이고 짤린선 추가 시켜야함
		}
		else
		{
			ASSERT(0);
		}
	}*/
}

MSSlabLevelZone* MSSlabG::FindSlabLevelZone(MSLevelZone* pZone, GM2DVector TheVec, bool bFindDeleted)
{
	long NumSlabZone = ma_LevelPoly.size();
	for(long i = 0 ; i < NumSlabZone ; i++)
	{
		MSSlabLevelZone* pCurSlabZone =  ma_LevelPoly[i];
		if(pCurSlabZone == NULL || (bFindDeleted == false && pCurSlabZone->m_IsDelete == true)) continue;
		
		if(pCurSlabZone->mp_LevelZone != pZone) continue;
		if(pCurSlabZone->mp_Polyline->Contains(&TheVec) < 1)
		{
			return pCurSlabZone;
		}

	}
	return NULL;
}

MSSlabLevelZone* MSSlabG::FindSlabLevelZone(GM2DPolyline* pPoly, bool bFindDeleted)
{
	double dArea = 0;
	if(pPoly->HasBulge())
		dArea = AcisGeomHelper::GetArea(*pPoly);
	else
		dArea = pPoly->Area();

	GM2DVector CenVec = pPoly->MassCenter();
	long NumVec = pPoly->GetSize();

	long NumSlabZone = ma_LevelPoly.size();
	for(long i = 0 ; i < NumSlabZone ; i++)
	{
		MSSlabLevelZone* pCurSlabZone =  ma_LevelPoly[i];

		if(pCurSlabZone == NULL || (bFindDeleted == false && pCurSlabZone->m_IsDelete == true)) continue;

		double dCurArea = 0;
		if(pCurSlabZone->mp_Polyline->HasBulge())
			dCurArea = AcisGeomHelper::GetArea(*(pCurSlabZone->mp_Polyline));
		else
			dCurArea = pCurSlabZone->mp_Polyline->Area();
		GM2DVector CurCenVec = pCurSlabZone->mp_Polyline->MassCenter();
		long NumCurVec = pCurSlabZone->mp_Polyline->GetSize();

		// MSSlabLevelZone 오프닝에 의해서 점의 갯수가 달라지는 경우 발생함
		//if(NumVec == NumCurVec && dArea - dCurArea < DTOL_GM && CurCenVec.SameVector4DTOL(CenVec))
		if(dArea - dCurArea < DTOL_GM && CurCenVec.SameVector4DTOL(CenVec))
			return pCurSlabZone;
		// 		if(pCurSlabZone->mp_Polyline->SamePolyLine(pPoly))
		// 		{
		// 			return pCurSlabZone;
		// 		}

	}
	return NULL;
}

void MSSlabG::SetLevelPolyDelete(MSLevelZone* pLevelZone)
{
	long NumLevelPoly = ma_LevelPoly.size();
	for(long iLvelPoly = 0 ; iLvelPoly < NumLevelPoly ; iLvelPoly++)
	{
		if(ma_LevelPoly[iLvelPoly]->mp_LevelZone == pLevelZone)
			ma_LevelPoly[iLvelPoly]->m_IsDelete = true;
	}
}

long MSSlabG::MakeSlabLevelZone(MSLevelZone* pZone, bool& isDoit)
{
	GM2DPolyline* pSlabPoly = GetProfile();

	double dSlabArea = AcisGeomHelper::GetArea(*pSlabPoly);

	BOOL bRval = pSlabPoly->OverlapEx(*(pZone->GetProfile()));
	if(bRval == FALSE)
		return 0;
	
	double dSlabAreaTol = 300;
	long NumSlabZone = ma_LevelPoly.size();

	if(mp_Owner->m_bIsSerializing && NumSlabZone > 0)
	{
		bool isFind = false;
		bool isSetLevelDifInfo = false;

		MSSlabLevelZone* pSlabLevelZone = NULL;

		
		for(long i = 0 ; i < NumSlabZone ; i++)
		{
			MSSlabLevelZone* pCurSlabZone =  ma_LevelPoly[i];
			if( (pCurSlabZone->m_LevelZoneID != pZone->m_ID ) || pCurSlabZone->mp_LevelZone != NULL) 
				continue;

			pSlabLevelZone = pCurSlabZone;

			pZone->m_PreWidth = pSlabLevelZone->m_dWidth;
			pSlabLevelZone->m_IsNewSlabZone = false;

			double dArea =  AcisGeomHelper::GetArea(*pSlabLevelZone->mp_Polyline);
			if(dSlabArea - dArea < dSlabAreaTol)
				pSlabLevelZone->m_IsFullSlabZone = true;

			pSlabLevelZone->m_LevelZoneID = pZone->m_ID;
			pSlabLevelZone->mp_LevelZone = pZone;

			SetLevelPolyDelete(pZone);
			pSlabLevelZone->m_IsDelete = false;

			long NumLine = pSlabLevelZone->mp_Polyline ->GetNumLines();
			long NumLevelDifInfo = pSlabLevelZone->ma_LevelDifInfo.size();

			if(NumLevelDifInfo != NumLine)
			{
				pSlabLevelZone->ma_LevelDifInfo.clear();
				isSetLevelDifInfo = true;
				isDoit = true;

				pSlabLevelZone->m_dPreWidth = 0;
				pSlabLevelZone->m_dWidth = pZone->m_Width;
				pSlabLevelZone->m_IsNewSlabZone = true;
			}

			if(fabs(pSlabLevelZone->m_dWidth - pZone->m_Width) > DTOL_GM)
			{
				pSlabLevelZone->m_dPreWidth = pZone->m_PreWidth;
				pSlabLevelZone->m_dWidth = pZone->m_Width;
				pSlabLevelZone->m_IsNewSlabZone = true;
			}

			if(isSetLevelDifInfo == true)
			{
				long NumLine = pSlabLevelZone->mp_Polyline->GetNumLines();
				for(long i = 0 ; i < NumLine ; i++)
				{
					LevelDifInfoStruct LevelDifInfo;
					LevelDifInfo.m_dWallWidth = pZone->m_Width;
					pSlabLevelZone->ma_LevelDifInfo.push_back(LevelDifInfo);
				}
			}
		}
	}
	else
	{
		CTypedPtrArray<CObArray, GM2DPolyline*> LevelPolyArr;
		//pSlabPoly->BoolOperation('C', pZone->GetProfile(), LevelPolyArr, DTOL_GM);//using polyline boolean
		pSlabPoly->BoolOperation('I', pZone->GetProfile(), LevelPolyArr, DTOL_GM);//after using ACIS boolean
		long nLevelPoly = LevelPolyArr.GetSize();
		if(nLevelPoly==0)
			return 0;

		MSFloor* pFloor = pZone->GetFloor();
		for(long iLevelPoly=0; iLevelPoly<nLevelPoly; iLevelPoly++)
		{
			GM2DPolyline* pPoly = LevelPolyArr[iLevelPoly];

			//pPoly->MakeClosedPolyline(DTOL_GM);
			if(!pPoly->IsClosed())
				pPoly->SplitDump();
			if(pPoly->GetNumLines() <= 2 || !pPoly->IsClosed())
			{
				delete pPoly;
				continue;
			}

			long NumOpen = mp_PolylineInfo->ma_Opening.size();
			for(long iOpen = 0 ; iOpen < NumOpen ; iOpen++)
			{
				MSOpening* pOpen = mp_PolylineInfo->ma_Opening[iOpen];
				if(pOpen == NULL)
					continue;
				GM2DPolyline* pOpenPoly = pOpen->GetProfile();
				pPoly->SegmentstoMe(pOpenPoly);
			}

			GM2DVector CenVec = pPoly->MassCenter();

			bool isFind = false;
			bool isSetLevelDifInfo = false;
			// 		MSSlabLevelZone* pSlabLevelZone = FindSlabLevelZone(pZone, CenVec);
			// 		if(pSlabLevelZone == NULL)
			MSSlabLevelZone* pSlabLevelZone = FindSlabLevelZone(pPoly, true);
			if(pSlabLevelZone == NULL)
			{
				pSlabLevelZone = new MSSlabLevelZone;
				isSetLevelDifInfo = true;
			}
			else
			{
				pZone->m_PreWidth = pSlabLevelZone->m_dWidth;
				pSlabLevelZone->m_IsNewSlabZone = false;
				isFind = true;
			}

			double dArea =  AcisGeomHelper::GetArea(*pPoly);
			if(dSlabArea - dArea < dSlabAreaTol)
				pSlabLevelZone->m_IsFullSlabZone = true;

			pSlabLevelZone->m_LevelZoneID = pZone->m_ID;
			pSlabLevelZone->mp_LevelZone = pZone;

			if(isFind == false)
			{
				pSlabLevelZone->mp_Polyline->ResetPolyline();
				pSlabLevelZone->mp_Polyline->CopyFrom(pPoly);
			}

			delete pPoly;
			//pSlabLevelZone->mp_Polyline = pPoly;
			SetLevelPolyDelete(pZone);
			pSlabLevelZone->m_IsDelete = false;

			long NumLine = pSlabLevelZone->mp_Polyline ->GetNumLines();
			long NumLevelDifInfo = pSlabLevelZone->ma_LevelDifInfo.size();

			if(NumLevelDifInfo != NumLine)
			{
				pSlabLevelZone->ma_LevelDifInfo.clear();
				isSetLevelDifInfo = true;
				isDoit = true;

				//pSlabLevelZone->m_dPreWidth = pZone->m_PreWidth;
				pSlabLevelZone->m_dPreWidth = 0;
				pSlabLevelZone->m_dWidth = pZone->m_Width;
				pSlabLevelZone->m_IsNewSlabZone = true;
			}

			if(fabs(pSlabLevelZone->m_dWidth - pZone->m_Width) > DTOL_GM)
			{
				pSlabLevelZone->m_dPreWidth = pZone->m_PreWidth;
				pSlabLevelZone->m_dWidth = pZone->m_Width;
				pSlabLevelZone->m_IsNewSlabZone = true;

				for(long i = 0 ; i < NumLevelDifInfo ; i++)
				{
					pSlabLevelZone->ma_LevelDifInfo[i].m_dWallWidth = pZone->m_Width;
				}
			}

			if(isSetLevelDifInfo == true)
			{
				long NumLine = pSlabLevelZone->mp_Polyline->GetNumLines();
				for(long i = 0 ; i < NumLine ; i++)
				{
					LevelDifInfoStruct LevelDifInfo;
					LevelDifInfo.m_dWallWidth = pZone->m_Width;
					pSlabLevelZone->ma_LevelDifInfo.push_back(LevelDifInfo);
				}
			}

			if(isFind == false)
				ma_LevelPoly.push_back(pSlabLevelZone);
		}
	}
	return 1;
}

void MSSlabG::ResetSlabLevelZone()
{
	long NumSlabZone = ma_LevelPoly.size();
	for(long i = NumSlabZone -1 ; i >= 0 ; i--)
	{
		MSSlabLevelZone* pCurSlabZone =  ma_LevelPoly[i];
		if(pCurSlabZone == NULL)
			continue;
		if(pCurSlabZone->m_IsDelete || pCurSlabZone->mp_LevelZone == NULL || pCurSlabZone->mp_Polyline->ma_Point.GetSize() == 0)
		{
			delete pCurSlabZone;
			ma_LevelPoly.erase(ma_LevelPoly.begin() + i);
		}
	}
}

bool MSSlabG::HasLevelDif(MSLineLevelDifG* pLevelDif)
{
	long NumLevelDif = ma_LevelDif.size();
	for(long iDif = 0 ; iDif < NumLevelDif ; iDif++)
	{
		if(ma_LevelDif[iDif]->mp_LineLevelDifG == pLevelDif)
			return true;
	}

// 	NumLevelDif = ma_LevelDifOnEdge.size();
// 	for(long iDif = 0 ; iDif < NumLevelDif ; iDif++)
// 	{
// 		if(ma_LevelDifOnEdge[iDif] == pLevelDif)
// 			return true;
// 	}

	return false;
}

bool MSSlabG::HasLevelDif()
{
	if(ma_LevelDif.size() > 0)
		return true;
	if(ma_EdgeWall.size() > 0)
		return true;

	long NumSlabLevelZone = 0;
	for(long i = 0 ; i < ma_LevelPoly.size() ; i++)
	{
		if(ma_LevelPoly[i]->m_IsDelete)
			continue;
		NumSlabLevelZone++;
	}

	if(NumSlabLevelZone > 0)
		return true;

	return false;
}

void MSSlabG::DeleteSlabLineLevelDifAll()
{
	long NumDif = ma_LevelDif.size();
	for(long iDif = NumDif -1 ; iDif >= 0 ; iDif--)
	{
		MSSlabLineLevelDifG* pDif = ma_LevelDif[iDif];
		delete pDif;
	}
	ma_LevelDif.clear();
}

void MSSlabG::DeleteSlabLineLevelDif(MSLineLevelDifG* pLineLevelDifG)
{
	long NumDif = ma_LevelDif.size();
	for(long iDif = NumDif -1 ; iDif >= 0 ; iDif--)
	{
		MSSlabLineLevelDifG* pSlabDif = ma_LevelDif[iDif];
		if(pSlabDif->mp_LineLevelDifG == pLineLevelDifG)
		{
			delete pSlabDif;
			ma_LevelDif.erase(ma_LevelDif.begin() + iDif);
		}
	}
}

void MSSlabG::DeleteSlabLineLevelDifByLevelZone(MSLevelZone* pLevelZone)
{
	long NumDif = ma_LevelDif.size();
	for(long iDif = NumDif -1 ; iDif >= 0 ; iDif--)
	{
		MSSlabLineLevelDifG* pDif = ma_LevelDif[iDif];
		if(pDif->mp_LeftLevelZone == pLevelZone)
			pDif->mp_LeftLevelZone = NULL;
		if(pDif->mp_RightLevelZone == pLevelZone)
			pDif->mp_RightLevelZone = NULL;

		if(pDif->mp_LeftLevelZone == NULL && pDif->mp_RightLevelZone == NULL)
		{
			delete pDif;
			ma_LevelDif.erase(ma_LevelDif.begin() + iDif);
		}
	}
}

long MSSlabG::CheckCrossThenAddLevelDif( MSLevelZone* pZone, bool isDoit)
{
	DeleteSlabLineLevelDifByLevelZone(pZone);
	if(IsSlopeMember())
		return 0;

	GM2DPolyline* pSlabPoly = GetProfile();

	BOOL bRval = pSlabPoly->OverlapEx(*(pZone->GetProfile()));
	if(bRval == FALSE)
		return 0;

	MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(m_ID);

	vector<MSCompoG*> InnerSlabG = FindConnectMember(FindType_MSElement::msSlabG, FIND_CURFLOOR);
	for (int i = InnerSlabG.size()-1; i >= 0; i--)
	{
		if (pSlabPoly->Contains(InnerSlabG[i]->GetProfile()) == false)
			InnerSlabG.erase(InnerSlabG.begin() + i);
	}

	long NumLevelDif = pZone->ma_LineLevelDif.size();
	for(long iLevelDif=0; iLevelDif < pZone->ma_LineLevelDif.size() ; iLevelDif++)
	{
		MSLineLevelDifG* pLevelDif = pZone->ma_LineLevelDif[iLevelDif];
		if(pLevelDif->IsSameLRZLevel())
		{
			pLevelDif->DeleteSlabLineLevelDif();
			continue;
		}

		GM2DCurve* pMyCurve = pLevelDif->GetCurve();
		CTypedPtrArray<CObArray, GM2DCurve*> Segments;
		pMyCurve->GetSegments(Segments, pSlabPoly);

		int nSegment = Segments.GetSize();
		for(int k=0; k<nSegment; k++)
		{
			GM2DCurve* pCurCurve = Segments[k];

			if (InnerSlabG.size() > 0)
			{
				for (int i = 0; i < InnerSlabG.size(); i++)
				{
					GM2DPolyline* pInnerSlabPoly = InnerSlabG[i]->GetProfile();
					CTypedPtrArray<CObArray, GM2DCurve*> InnerSegments;

					bool HasColinear = pInnerSlabPoly->HasColinearCurve(pCurCurve, DTOL_GM, DTOL_GM);
					if (HasColinear == true)
						InnerSegments.Add(pCurCurve->Copy());
					else
						pCurCurve->GetSegments(InnerSegments, pInnerSlabPoly);

					for (int iInnerSeg = 0; iInnerSeg < InnerSegments.GetSize(); iInnerSeg++)
					{
						GM2DCurve* pInnerCurve = InnerSegments[iInnerSeg];
						if (pInnerSlabPoly->Contains(pInnerCurve, true, DTOL_GM) == true)
						{
							delete pInnerCurve;
							continue;
						}

						if (pInnerCurve->Length() > MSFloor::ms_DefLineHitDistTol)
							CheckCrossThenAddLevelDifStep2(pInnerCurve, pLevelDif, pZone);
					}
				}
			}
			else
			{
				if (pCurCurve->Length() > MSFloor::ms_DefLineHitDistTol)
					CheckCrossThenAddLevelDifStep2(pCurCurve, pLevelDif, pZone);
			}
		
			delete pCurCurve;
		}
	}

// 	long NumOpening = mp_PolylineInfo->ma_Opening.size();
// 	for(long iOpening = 0 ; iOpening < NumOpening ; iOpening++)
// 	{
// 		MSOpening* pOpening = mp_PolylineInfo->ma_Opening[iOpening];
// 		if(pOpening == NULL) continue;
// 		GM2DPolyline* pCurPoly = pOpening->GetProfile();
// 
// 		long NumLevelDif = pZone->ma_LineLevelDif.size();
// 		for(long iLevelDif=0; iLevelDif < pZone->ma_LineLevelDif.size() ; iLevelDif++)
// 		{
// 			MSLineLevelDifG* pLevelDif = pZone->ma_LineLevelDif[iLevelDif];
// 			shared_ptr<GM2DCurve> pMyCurve = pLevelDif->GetCenLineVec();
// 			CTypedPtrArray<CObArray, GM2DCurve*> Segments;
// 			pMyCurve->GetSegments(Segments, pCurPoly);
// 
// 			int nSegment = Segments.GetSize();
// 			bool bfirst = true;
// 
// 			for(int k=0; k<nSegment; k++)
// 			{
// 				GM2DCurve* pCurCurve = Segments[k];
// 				if(pCurCurve->Length() < MSFloor::ms_DefLineHitDistTol)
// 					continue;
// 
// 				long nRavl = Contains(pCurCurve);
// 
// 
// 				if(bfirst)
// 				{
// 					pLevelDif->SetCurve(pCurCurve->Copy());
// 					bfirst = false;
// 
// 					if(nRavl == 1)
// 					{
// 						if(pLevelDif->mp_LeftSlab == this)
// 							pLevelDif->mp_LeftSlab = NULL;
// 						if(pLevelDif->mp_RightSlab == this)
// 							pLevelDif->mp_RightSlab = NULL;
// 						RemoveLevelDif(pLevelDif);
// 					}
// 					else
// 						pLevelDif->mp_LeftSlab = pLevelDif->mp_RightSlab = this;
// 
// 					continue;
// 				}
// 
// 				MSLineLevelDifG* pNewDif = (MSLineLevelDifG*)pLevelDif->Copy();
// 				pNewDif->SetCurve(pCurCurve->Copy());
// 
// 				if(nRavl == 1)
// 				{
// 					if(pNewDif->mp_LeftSlab == this)
// 						pNewDif->mp_LeftSlab = NULL;
// 					if(pNewDif->mp_RightSlab == this)
// 						pNewDif->mp_RightSlab = NULL;
// 				}
// 				else
// 				{
// 					pNewDif->mp_LeftSlab = pNewDif->mp_RightSlab = this;
// 					ma_LevelDif.push_back(pNewDif);
// 				}
// 				
// 				if(pNewDif->mp_LeftLevelZone != NULL)
// 					pNewDif->mp_LeftLevelZone->AddLevelDifG(pNewDif);
// 				if(pNewDif->mp_RightLevelZone!= NULL)
// 					pNewDif->mp_RightLevelZone->AddLevelDifG(pNewDif);
// 			}
// 
// 			for(int k=0; k<nSegment; k++)
// 				delete Segments[k];
// 		}
// 	}

	if(isDoit == true)
		SetLevelDifWidthInSlabLevelZone();
	return 0;
}

/*
void MSSlabG::AddSlabLevelDif(MSLineLevelDifG* pLevelDif)
{
}
*/

void MSSlabG::GetPolyline( GM2DPolyline& pPoly )
{
	pPoly.CopyFrom(GetProfile());
}

bool MSSlabG::RemoveLevelDif(MSLineLevelDifG* pLevelDif)
{
	bool bRval = false;
	long nLevelDif = ma_LevelDif.size();
	for(long iLevelDif= nLevelDif - 1 ; iLevelDif>= 0; iLevelDif--)
	{
		if(ma_LevelDif[iLevelDif]->mp_LineLevelDifG == pLevelDif)
		{
			ma_LevelDif.erase(ma_LevelDif.begin() + iLevelDif);
			bRval = true;
		}
	}

// 	nLevelDif = ma_LevelDifOnEdge.size();
// 	//for(long iLevelDif=0; iLevelDif<nLevelDif; iLevelDif++)
// 	for(long iLevelDif= nLevelDif - 1 ; iLevelDif>= 0; iLevelDif--)
// 	{
// 		if(ma_LevelDifOnEdge[iLevelDif] == pLevelDif)
// 		{
// 			ma_LevelDifOnEdge.erase(ma_LevelDifOnEdge.begin() + iLevelDif);
// 			bRval = true;
// 		}
// 	}
	return bRval;
}

void MSSlabG::RemoveLevelZone( MSLevelZone* pLevelZone )
{
	long nLevelDif = ma_LevelDif.size();
	//for(long iLevelDif=0; iLevelDif<nLevelDif; iLevelDif++)
	for(long iLevelDif= nLevelDif - 1 ; iLevelDif>= 0; iLevelDif--)
	{
// 		MSLineLevelDifG* pDifG = ma_LevelDif[iLevelDif];
// 		if(pDifG->mp_LeftLevelZone==pLevelZone)
// 		{
// 			pDifG->mp_LeftLevelZone = NULL;
// 			if(pDifG->mp_RightLevelZone == NULL)
// 			{	
// 				MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(m_ID);
// 				MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(pDifG->m_ID);
// 				if(pDifG->mp_LeftSlab == this)
// 					pDifG->mp_LeftSlab = NULL;
// 				if(pDifG->mp_RightSlab== this)
// 					pDifG->mp_RightSlab = NULL;
// 
// 				ma_LevelDif.erase(ma_LevelDif.begin() + iLevelDif);
// 				continue;
// 				//delete ma_LevelDif[iLevelDif];
// 			}
// 		}
// 		if(pDifG->mp_RightLevelZone==pLevelZone)
// 		{
// 			pDifG->mp_RightLevelZone = NULL;
// 			if(pDifG->mp_LeftLevelZone == NULL)
// 			{	
// 				MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(m_ID);
// 				MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(pDifG->m_ID);
// 				if(pDifG->mp_LeftSlab == this)
// 					pDifG->mp_LeftSlab = NULL;
// 				if(pDifG->mp_RightSlab== this)
// 					pDifG->mp_RightSlab = NULL;
// 
// 				ma_LevelDif.erase(ma_LevelDif.begin() + iLevelDif);
// 				continue;
// 				//delete ma_LevelDif[iLevelDif];
// 			}
// 		}
// 		if (pDifG->mp_LeftLevelZone==NULL && pDifG->mp_RightLevelZone==NULL)
// 		{
// 			MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(m_ID);
// 			MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(pDifG->m_ID);
// 			ma_LevelDif.erase(ma_LevelDif.begin() + iLevelDif);
// 			continue;
// 		}
	}

// 	nLevelDif = ma_LevelDifOnEdge.size();
// 	for(long iLevelDif= nLevelDif - 1 ; iLevelDif>= 0; iLevelDif--)
// 	{
// 		MSLineLevelDifG* pDifG = ma_LevelDifOnEdge[iLevelDif];
// 		if(pDifG->mp_LeftLevelZone==pLevelZone)
// 		{
// 			pDifG->mp_LeftLevelZone = NULL;
// 			if(pDifG->mp_RightLevelZone == NULL)
// 			{	
// 				MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(m_ID);
// 				MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(pDifG->m_ID);
// 				if(pDifG->mp_LeftSlab == this)
// 					pDifG->mp_LeftSlab = NULL;
// 				if(pDifG->mp_RightSlab== this)
// 					pDifG->mp_RightSlab = NULL;
// 
// 				ma_LevelDifOnEdge.erase(ma_LevelDifOnEdge.begin() + iLevelDif);
// 				continue;
// 				//delete ma_LevelDif[iLevelDif];
// 			}
// 		}
// 		if(pDifG->mp_RightLevelZone==pLevelZone)
// 		{
// 			pDifG->mp_RightLevelZone = NULL;
// 			if(pDifG->mp_LeftLevelZone == NULL)
// 			{	
// 				MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(m_ID);
// 				MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(pDifG->m_ID);
// 				if(pDifG->mp_LeftSlab == this)
// 					pDifG->mp_LeftSlab = NULL;
// 				if(pDifG->mp_RightSlab== this)
// 					pDifG->mp_RightSlab = NULL;
// 
// 				ma_LevelDifOnEdge.erase(ma_LevelDifOnEdge.begin() + iLevelDif);
// 				continue;
// 				//delete ma_LevelDif[iLevelDif];
// 			}
// 		}
// 		if (pDifG->mp_LeftLevelZone==NULL && pDifG->mp_RightLevelZone==NULL)
// 		{
// 			MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(m_ID);
// 			MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(pDifG->m_ID);
// 			ma_LevelDifOnEdge.erase(ma_LevelDifOnEdge.begin() + iLevelDif);
// 			continue;
// 		}
// 	}

//	ma_LevelPoly은 삭제 안되고 수정만 됨
	SetLevelPolyDelete(pLevelZone);
	DeleteSlabLineLevelDifByLevelZone(pLevelZone);
	TrimLevelDifAll();

}

// -1 : 보-벽 조합으로 레벨 겹침.
// 0 : 레벨 불일치
// 1 : 한 부재 하단에 한 부재 상단이 접함
// 2 : 상호 어긋나게 교차
// 3 : my가 ur에 포함됨
// 4 : my가 ur를 포함함
// 5 : 완전 일치

long MSSlabG::LevelOverlap(MSSlabG* pUrSlab)
{
	double MyTopLevel, MyBotLevel;
	double UrTopLevel, UrBotLevel;
	if(!this->GetZLevelsWidthOutZLevel(MyTopLevel, MyBotLevel))
		ASSERT(0);
	if(!pUrSlab->GetZLevelsWidthOutZLevel(UrTopLevel, UrBotLevel))
		ASSERT(0);

	if(MyTopLevel <= UrBotLevel+DTOL1)
		return 0;
	if(UrTopLevel <= MyBotLevel+DTOL1)
		return 0;

	if(fabs(MyTopLevel-UrTopLevel) < DTOL1)
	{
		if(fabs(MyBotLevel-UrBotLevel) < DTOL1)
			return 1;
		else if(MyBotLevel>UrBotLevel)
			return 2;
		else if(MyBotLevel<UrBotLevel)
			return 3;
	}
	else if(MyTopLevel > UrTopLevel)
	{
		//트림을 하지 않음.
		if(fabs(MyBotLevel-UrBotLevel) < DTOL1)
			return -1;
		else if(MyBotLevel>UrBotLevel)
			return -2;
		else if(MyBotLevel<UrBotLevel)
			return -3;
	}
	else 
	{
		if(fabs(MyBotLevel-UrBotLevel) < DTOL1)
			return 4;
		else if(MyBotLevel>UrBotLevel)
			return 5;
		else if(MyBotLevel<UrBotLevel)
			return 6;
	}
	return 0;
}

double MSSlabG::GetFullSlabLevelZoneLevel()
{
	MSSlabLevelZone* pFullSlabLevelZone = NULL;
	for (int i = 0; i < ma_LevelPoly.size(); i++)
	{
		MSSlabLevelZone* pSlabLevelZone = ma_LevelPoly[i];
		if (pSlabLevelZone == NULL || pSlabLevelZone->m_IsDelete == true)
			continue;
		if (pSlabLevelZone->m_IsFullSlabZone)
		{
			pFullSlabLevelZone = pSlabLevelZone;
			break;
		}
	}

	if (pFullSlabLevelZone == NULL)
		return 0;

	return pFullSlabLevelZone->mp_LevelZone->m_Level;
}

bool MSSlabG::GetZLevels( double& TopLevel, double& BottomLevel, bool GetBigVal, bool bCheckLevelZone )
{
	if (IsSlopeMember())
	{
		GetZLevels_Slope(TopLevel, BottomLevel);
		return true;
	}

	if(bCheckLevelZone == false)
	{
		TopLevel = GetNextSlabLevel() + m_dOffset;
		BottomLevel = TopLevel - GetThick();

		return true;
	}

	double dMaxLevel = 0;
	double dMinLevel = 0;

	long NumLevelPoly = ma_LevelPoly.size();
	for(long iLevelPoly = 0 ; iLevelPoly < NumLevelPoly ; iLevelPoly++)
	{
		MSSlabLevelZone* pSlabLevelZone = ma_LevelPoly[iLevelPoly];
		if(pSlabLevelZone->m_IsDelete || pSlabLevelZone->mp_LevelZone == NULL)
			continue;
		if(pSlabLevelZone->m_IsFullSlabZone)
		{
			dMaxLevel = pSlabLevelZone->mp_LevelZone->m_Level;
			dMinLevel = pSlabLevelZone->mp_LevelZone->m_Level;
			break;
		}
		else
		{
			dMaxLevel = max(dMaxLevel, pSlabLevelZone->mp_LevelZone->m_Level);
			dMinLevel = min(dMinLevel, pSlabLevelZone->mp_LevelZone->m_Level);
		}
	}

	TopLevel = GetNextSlabLevel() + m_dOffset +  dMaxLevel;
	BottomLevel = TopLevel - dMaxLevel + dMinLevel - GetThick();

	if(GetBigVal == true)
	{
		vector<MSLinkMemberG*> LevelDifArr = GetLevelDifGAll(true);
		for(long iDif = 0 ; iDif < LevelDifArr.size() ; iDif++)
		{
			MSLinkMemberG* pDif = LevelDifArr[iDif];
			double dTopDifLevel = 0;
			double dBotDifLevel = 0;
			pDif->GetZLevels(dTopDifLevel, dBotDifLevel);

			TopLevel = max(TopLevel, dTopDifLevel);
			BottomLevel = min(BottomLevel, dBotDifLevel);
		}
	}

	return true;
}

bool MSSlabG::GetZLevelsWidthOutZLevel( double& TopLevel, double& BottomLevel)
{
	TopLevel = GetNextSlabLevel() + m_dOffset;
	BottomLevel = TopLevel - GetThick();

	return true;
}

void MSSlabG::GetZLevels(GM2DVector TheVec, double& TopLevel, double& BottomLevel, bool GetBigVal)
{
	MSFloor* pFloor = GetFloor();
	//double dZoneLevel = pFloor->GetLevel4LevelZone(TheVec.m_X, TheVec.m_Y);
	double dZoneLevel = GetSlabLevelZoneLevel(TheVec);

	if (IsSlopeMember())
	{
// 		GetZLevels_Slope(TopLevel, BottomLevel);
// 		TopLevel += dZoneLevel;
// 		BottomLevel += dZoneLevel - GetThick();
		TopLevel = GetNextSlabLevel() + dZoneLevel + GetZOffsetOnSlabTopPlane(TheVec.m_X, TheVec.m_Y);
		BottomLevel = TopLevel - GetThick();
	}
	else
	{
		TopLevel = GetNextSlabLevel() + m_dOffset + dZoneLevel;
		BottomLevel = TopLevel - GetThick();
	}
	if(GetBigVal == true)
	{
		vector<MSLinkMemberG*> LevelDifArr = GetLevelDifGAll(true);
		for(long iDif = 0 ; iDif < LevelDifArr.size() ; iDif++)
		{
			MSLinkMemberG* pDif = LevelDifArr[iDif];
			GM2DPolyline* pPoly = pDif->GetTrimmedBoundary();
			long nRval = pPoly->Contains(&TheVec);
			if(nRval == 1)
				continue;

			double dTopDifLevel = 0;
			double dBotDifLevel = 0;
			pDif->GetZLevels(dTopDifLevel, dBotDifLevel);

			TopLevel = max(TopLevel, dTopDifLevel);
			BottomLevel = min(BottomLevel, dBotDifLevel);
		}
	}
}

void MSSlabG::GetZLevels(GM2DCurve* pCurve, double& TopLevel, double& BottomLevel, bool GetBigVal)
{
	double dSTopLevel = 0 , dSBottomLevel = 0;
	double dETopLevel = 0 , dEBottomLevel = 0;
	double dMTopLevel = 0 , dMBottomLevel = 0;

	GetZLevels(pCurve->m_P0, dSTopLevel, dSBottomLevel, GetBigVal);
	GetZLevels(pCurve->PointAtParam(0.5), dMTopLevel, dMBottomLevel, GetBigVal);
	GetZLevels(pCurve->m_P1, dETopLevel, dEBottomLevel, GetBigVal);
	
	if(GetBigVal == true)
	{
		TopLevel = max(max(dSTopLevel, dMTopLevel), dETopLevel);
		BottomLevel = min(min(dSBottomLevel, dMBottomLevel), dEBottomLevel);
	}
	else
	{
		TopLevel = min(min(dSTopLevel, dMTopLevel), dETopLevel);
		BottomLevel = max(max(dSBottomLevel, dMBottomLevel), dEBottomLevel);
	}
}
double MSSlabG::GetTopLevel(GM2DCurve *pCurve, bool getBigVal)
{
	double dSTopLevel = 0 , dSBottomLevel = 0;
	double dETopLevel = 0 , dEBottomLevel = 0;
	double dMTopLevel = 0 , dMBottomLevel = 0;

	GetZLevels(pCurve->m_P0, dSTopLevel, dSBottomLevel, getBigVal);
	GetZLevels(pCurve->PointAtParam(0.5), dMTopLevel, dMBottomLevel, getBigVal);
	GetZLevels(pCurve->m_P1, dETopLevel, dEBottomLevel, getBigVal);

	if(getBigVal)
		return max(max(dSTopLevel, dMTopLevel), dETopLevel);
	
	return min(min(dSTopLevel, dMTopLevel), dETopLevel);
}
double MSSlabG::GetBotLevel(GM2DCurve *pCurve, bool getBigVal)
{
	double dSTopLevel = 0 , dSBottomLevel = 0;
	double dETopLevel = 0 , dEBottomLevel = 0;
	double dMTopLevel = 0 , dMBottomLevel = 0;

	GetZLevels(pCurve->m_P0, dSTopLevel, dSBottomLevel, getBigVal);
	GetZLevels(pCurve->PointAtParam(0.5), dMTopLevel, dMBottomLevel, getBigVal);
	GetZLevels(pCurve->m_P1, dETopLevel, dEBottomLevel, getBigVal);

	if(getBigVal)
		return max(max(dSBottomLevel, dMBottomLevel), dEBottomLevel);

	return min(min(dSBottomLevel, dMBottomLevel), dEBottomLevel);
}
double MSSlabG::GetSlabLevelZoneLevel(GM2DVector TheVec)
{
	double dLevel = 0;
	for(long i = 0 ; i < ma_LevelPoly.size() ; i++)
	{
		MSSlabLevelZone* pSlabLevelZone = ma_LevelPoly[i];
		if(pSlabLevelZone == NULL || pSlabLevelZone->m_IsDelete || pSlabLevelZone->mp_LevelZone == NULL)
			continue;
		if(pSlabLevelZone->mp_Polyline->Contains(&TheVec) < 1)
		{
			dLevel = pSlabLevelZone->mp_LevelZone->m_Level;
			break;
		}
	}

	return dLevel;
}

long MSSlabG::GetZLevelInfo(MSCompoG* pUrCompoG)
{
	long nRval = 0;

	bool bCheckLevelZone = false;
	MSFloor* pMyFloor = GetFloor();
	MSFloor* pUrFloor = pUrCompoG->GetFloor();
	if(pMyFloor != pUrFloor)
		bCheckLevelZone = true;
	// 체크할 부재의 층이 다를 경우 bCheckLevelZone을 True로 해준다.

	double MyTopLevel=0, MyBotLevel=0;
	double UrTopLevel=0, UrBotLevel=0;
	GetZLevels(MyTopLevel, MyBotLevel, false, bCheckLevelZone);
	pUrCompoG->GetZLevels(UrTopLevel, UrBotLevel, false, bCheckLevelZone);

	if(fabs(MyBotLevel - UrTopLevel) < DTOL1 || (fabs(UrBotLevel - MyTopLevel) < DTOL1))
		nRval |= ZLevelConnection;

	if(MyTopLevel > UrTopLevel &&
		UrTopLevel > MyBotLevel )
		nRval |= LowUrMember;

	if(!(MyTopLevel <= UrBotLevel + DTOL1) || (UrTopLevel <= MyBotLevel + DTOL1))
		nRval |= ZLevelOverLap;

	if (fabs(MyTopLevel - UrTopLevel) < DTOL1 || fabs(MyBotLevel - UrBotLevel) < DTOL1)
		nRval |= SameTopBotZLevel;


	return nRval;
}

bool MSSlabG::VisibleInRect( GM2DRectangle* pRect)
{
	if(mp_PolylineInfo == NULL)
		return false;
	if(mp_PolylineInfo->VisibleInRect(pRect))
		return true;
	return false;
}

void MSSlabG::CalcBoundRectBeforeAddQuadTree()
{
	if(mp_PolylineInfo == NULL)
		return;

	mp_PolylineInfo->CalcBoundRectBeforeAddQuadTree();
}

bool MSSlabG::AddOpening(GM2DPolyline* pPoly)
{
	// 	long nRval = Contains(pPoly);
	// 	if(nRval == -1)
	{
		mp_PolylineInfo->AddOpening(pPoly);
		return true;
	}
	//	return false;
}

bool MSSlabG::AddOpening(MSOpening* pOpening)
{
	//CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;

	//this->GetProfile()->BoolOperation('I', pOpening->GetProfile(), JoinedPolys, DTOL_GM);
	//int count = JoinedPolys.GetSize();

	if (pOpening->GetOpeningType() == MSOpening::General)
	{
		vector<GM2DPolyline*> JoinedPolys;
		AcisGeomHelper::BooleanPolyline('I', *this->GetProfile(), *pOpening->GetProfile(), JoinedPolys);

	// 	vector<GM2DPolyline*> TmpPolys;
	// 	long NumOpen = mp_PolylineInfo->ma_Opening.size();
	// 	for(long iOpen = 0 ; iOpen < NumOpen ; iOpen++)
	// 	{
	// 		MSOpening* pOpen = mp_PolylineInfo->ma_Opening[iOpen];
	// 		if(pOpen->ma_MadeByCompoGID.size() == 0)
	// 			continue;
	// 
	// 		GM2DPolyline* pOpenPoly = pOpen->GetProfile();
	// 
	// 		for(long iPoly = 0 ; iPoly < JoinedPolys.size() ; iPoly++)
	// 		{
	// 			GM2DPolyline* pNewOpenPoly = JoinedPolys[iPoly];
	// 			AcisGeomHelper::BooleanPolyline('A', *pNewOpenPoly, *pOpenPoly, TmpPolys);
	// 
	// 			for(long i = 0 ; i < TmpPolys.size() ; i++)
	// 			{
	// 				if(i == 0)
	// 				{
	// 					JoinedPolys[iPoly] = TmpPolys[0];
	// 					delete pNewOpenPoly;
	// 				}
	// 				else
	// 					JoinedPolys.push_back(TmpPolys[i]);
	// 			}
	// 		}
	// 	}

		long nAddCnt = 0;

		int count = JoinedPolys.size();
		for(int i=0; i<count; i++)
		{
			bool bAdd = AddOpening(JoinedPolys[i]);
			delete JoinedPolys[i];

			if(bAdd)
				nAddCnt++;
		}
		JoinedPolys.clear();

		if (nAddCnt > 0)
			return true;
	}
	else if(pOpening->GetOpeningType() == MSOpening::MaterialLift)
	{
		if (mp_PolylineInfo != nullptr)
		{
			MSOpening* newOpening = (MSOpening*)pOpening->Copy();
			newOpening->GetID();
			mp_PolylineInfo->ma_Opening.push_back(newOpening);
		}

		return true;
	}
	return false;
}

void MSSlabG::AddEdgeWall(double x, double y, double x2, double y2, double width, double height)
{
	/*
	double min_dist = 0.0;
	int idx = 0;
	for (int i=0 ; i<profile->ma_Point.GetSize()-1 ; ++i)
	{
		double midx = (profile->ma_Point[i]->m_X + profile->ma_Point[i+1]->m_X)/2.0;
		double midy = (profile->ma_Point[i]->m_Y + profile->ma_Point[i+1]->m_Y)/2.0;
		double dist = (x-midx)*(x-midx) + (y-midy)*(y-midy);
		if (i==0 || dist < min_dist) { idx = i; min_dist = dist; }
	}

	double bulge = 0.0;
	if (idx < profile->ma_Bulge.GetSize())
		bulge = profile->ma_Bulge[idx];
	*/

	GM2DPolyline* profile = GetProfile();
	GM2DCurve* pOnCurve = NULL;
	for (int i=0 ; i<profile->GetNumLines() ; ++i)
	{
		GM2DCurve* curve = profile->GetCurve(i);
		if (curve->HasOnLine(GM2DVector(x, y), DTOL_GM) && curve->HasOnLine(GM2DVector(x2, y2), DTOL_GM))
		{
			pOnCurve = curve;
			break;
		}
		delete curve;
	}
	if (pOnCurve == NULL) return;

	/////////////////////////////////////////////////////////////////////////////
	// MAKE Center-Line
	GM2DCurve* pCenCurve = NULL;
	if (pOnCurve->m_ObjType == GT_ARC)
	{
		GM2DArc* arc = (GM2DArc*)pOnCurve;
		double loc1 = arc->OrthoBaseLocValue(GM2DVector(x, y));
		double loc2 = arc->OrthoBaseLocValue(GM2DVector(x2, y2));
		double a1 = arc->Angle2DVector(__min(loc1, loc2));
		double a2 = arc->Angle2DVector(__max(loc1, loc2));
		pCenCurve = new GM2DArc(arc->GetRadius(), arc->m_Center.m_X, arc->m_Center.m_Y, a1, a2);
	}
	else
	{
		if (pOnCurve->DirVector()*GM2DVector(x2-x, y2-y) > 0.0)
			pCenCurve = new GM2DLineVector(x, y, x2, y2);
		else
			pCenCurve = new GM2DLineVector(x2, y2, x, y);
	}
	/*
	GM2DVector* p1 = (GM2DVector*)profile->ma_Point[idx]->Copy();
	GM2DVector* p2 = (GM2DVector*)profile->ma_Point[idx+1]->Copy();
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
	*/

	MSLineLevelDifG* dif = new MSLineLevelDifG();
	dif->SetOwner(mp_Owner);
	dif->SetCenCurve(pCenCurve);
	dif->m_dWallWidth = width;
	dif->m_dWallHeight = height;
	dif->SetIsUserLevelDif(true);
	dif->mp_LeftSlab = this;

	/////////////////////////////////////////////////////////////////////////////
	// MAKE LR Line
	dif->MakeLRLine_UserType(); // mylee 20150122
	
	/////////////////////////////////////////////////////////////////////////////
	// Trim
	//dif->CutLineBySlab(dif->mp_LLine, dif->mp_LeftSlab);

	ma_EdgeWall.push_back(dif);

	TrimEdgeWallbyOpening(dif);
	TrimLevelDifAll();
}

bool MSSlabG::Remove(MSLineLevelDifG* pLineLevelDifG)
{
	int findIdx = -1;
	for (int idx = 0; idx < ma_EdgeWall.size(); idx++)
	{
		if (ma_EdgeWall[idx] == pLineLevelDifG)
			findIdx = idx;
	}

	if (findIdx != -1)
	{
		ma_EdgeWall.erase(ma_EdgeWall.begin() + findIdx);
		return true;
	}

	return false;
}

void MSSlabG::ModifyEdgeWallWhenModifySlab(MSSlabG* org, MSSlabG* modifed_slab, bool isMirror)
{
	vector<int> idx_list;
	vector<MSLineLevelDifG*> dif_list;
	vector<double> loc_list1, loc_list2;

	GM2DPolyline* checkProfile = org->GetProfile(); 
	if (isMirror)	checkProfile = modifed_slab->GetProfile();
	for (int i = 0; i < modifed_slab->ma_EdgeWall.size(); ++i)
	{
		MSLineLevelDifG* cur = modifed_slab->ma_EdgeWall[i];
		for (int k = 0; k < checkProfile->ma_Point.GetSize() - 1; ++k)
		{
			if (checkProfile->ma_Point[k]->Distance((*cur->ma_Vec[0])) < 0.001 &&
				checkProfile->ma_Point[k + 1]->Distance((*cur->ma_Vec[1])) < 0.001)
			{
				idx_list.push_back(k);
				dif_list.push_back(cur);
				loc_list1.push_back(0.0);
				loc_list2.push_back(1.0);
			}
			else
			{
				GM2DLineVector line(checkProfile->ma_Point[k]->m_X, checkProfile->ma_Point[k]->m_Y,
					checkProfile->ma_Point[k + 1]->m_X, checkProfile->ma_Point[k + 1]->m_Y);
				if (line.HasOnLine((*cur->ma_Vec[0]), DTOL_GM) && line.HasOnLine((*cur->ma_Vec[1]), DTOL_GM))
				{
					idx_list.push_back(k);
					dif_list.push_back(cur);
					loc_list1.push_back(line.OrthoBaseLocValue((*cur->ma_Vec[0])));
					loc_list2.push_back(line.OrthoBaseLocValue((*cur->ma_Vec[1])));
				}
			}
		}
	}

	modifed_slab->ma_EdgeWall.clear();
	for (int k = 0; k < idx_list.size(); ++k)
	{
		dif_list[k]->SetEdgeWallFromProfile(modifed_slab->GetProfile(), idx_list[k], loc_list1[k], loc_list2[k]);
		//dif_list[k]->CutLineBySlab(dif_list[k]->mp_LLine, modifed_slab);
	}
	modifed_slab->ma_EdgeWall = dif_list;
}

vector<GM2DCurve*> MSSlabG::SplitCurveByOpening(GM2DCurve* pCenCurve)
{
	vector<GM2DCurve*> CurveArr;
	CurveArr.push_back(pCenCurve);

	vector<MSCompoG*> CompoGArr = GetOpeningGArr();
	
	MSFloor* pCurFloor = GetFloor();
	vector<MSCompoG*> PitArr = MSCompoGUtil::FindCompoG(pCurFloor, pCenCurve, FindType_MSElement::msPitG);
	//vector<MSCompoG*> PitArr = FindConnectMember(MSElement::msPitG, FIND_CURFLOOR);
	
	for (long iPit = 0; iPit < PitArr.size(); iPit++)
		CompoGArr.push_back(PitArr[iPit]);
	
	long NumOpening = CompoGArr.size();

	for(long iCurve = 0 ; iCurve < CurveArr.size() ; iCurve++)
	{
		GM2DCurve* pCurve = CurveArr[iCurve];
		//long NumOpening = mp_PolylineInfo->ma_Opening.size();
		for(long iOpening = 0 ; iOpening < NumOpening ; iOpening++)
		{
			if(pCurve == NULL) continue;

			MSCompoG* pCompoG = (MSCompoG*)CompoGArr[iOpening];
			if(pCompoG == NULL) continue;
			GM2DPolyline* pOpenPoly = pCompoG->GetProfile();

			CTypedPtrArray<CObArray, GM2DCurve*> tempSegments;
			pCurve->GetSegments(tempSegments, pOpenPoly, DTOL_GM, 1);

			long NumSeg = tempSegments.GetSize();
			for(long iSeg = NumSeg -1 ; iSeg >= 0 ; iSeg--)
			{
				GM2DCurve* pSeg = tempSegments[iSeg];
				GM2DVector CVec = pSeg->PointAtParam(0.5);
				long nRVal = pOpenPoly->Contains(&CVec);
				if(nRVal <= 0 || pSeg->Length() < MSFloor::ms_DefLineHitDistTol)
				{
					delete pSeg;
					tempSegments.RemoveAt(iSeg);
				}
			}

			NumSeg = tempSegments.GetSize();
			if(NumSeg > 0)
			{
				for(long iSeg = 0 ; iSeg < NumSeg ; iSeg++)
				{
					GM2DCurve* pSeg = tempSegments[iSeg];
					if(iSeg == 0)
					{
						pCurve->CopyFrom(pSeg);
						delete pSeg;
					}
					else
						CurveArr.push_back(pSeg);
				}
			}
			if(NumSeg == 0)
			{
				delete pCurve;
				CurveArr.erase(CurveArr.begin() + iCurve);
				iCurve--;
				pCurve = NULL;
			}
		}
	}
	return CurveArr;
}

void MSSlabG::TrimSlabLineLevelDifbyOpening()
{
	long NumDif = ma_LevelDif.size();
	for(long iDif = NumDif -1 ; iDif >= 0 ; iDif--)
	{
		MSSlabLineLevelDifG* pDif = ma_LevelDif[iDif];
		bool bRval = TrimSlabLineLevelDifbyOpening(pDif);
		if(bRval == false)
			DeleteSlabLineLevelDif(pDif->mp_LineLevelDifG);
	}
}

bool MSSlabG::TrimSlabLineLevelDifbyOpening(MSSlabLineLevelDifG* pDif)
{
	GM2DCurve* pCurve = pDif->GetCurve();
	GM2DCurve* pLineLevelDifCurve = pDif->mp_LineLevelDifG->GetCurve();
	vector<GM2DCurve*> CurveArr = SplitCurveByOpening(pCurve->Copy());
	long NumCurveArr = CurveArr.size();

	if(NumCurveArr == 0)
		return false;

	for(long iCurve = 0 ; iCurve < NumCurveArr ; iCurve++)
	{
		GM2DCurve* pSplitCurve = CurveArr[iCurve];
		if(iCurve == 0)
		{
			pDif->SetLoc(pLineLevelDifCurve->OrthoBaseLocValue(pSplitCurve->m_P0), pLineLevelDifCurve->OrthoBaseLocValue(pSplitCurve->m_P1));
		}
		else
		{
			MSSlabLineLevelDifG* pSplitDif = new MSSlabLineLevelDifG(pDif->mp_LineLevelDifG, pDif->m_pSlab);
			pSplitDif->CopyFromMe(pDif);
			pSplitDif->SetLoc(pLineLevelDifCurve->OrthoBaseLocValue(pSplitCurve->m_P0), pLineLevelDifCurve->OrthoBaseLocValue(pSplitCurve->m_P1));
			ma_LevelDif.push_back(pSplitDif);
		}
	}

	return true;
}

void MSSlabG::TrimEdgeWallbyOpening()
{
	long NumEdgeWall = ma_EdgeWall.size();
	for(long iEdgeWall = 0 ; iEdgeWall < ma_EdgeWall.size() ; iEdgeWall++)
	{
		MSLineLevelDifG* pDif = ma_EdgeWall[iEdgeWall];
		TrimEdgeWallbyOpening(pDif);
	}
}

void MSSlabG::TrimEdgeWallbyOpening(MSLineLevelDifG* pDif)
{

	GM2DCurve* pCurve = pDif->GetCurve();
	//vector<GM2DCurve*> CurveArr = SplitCurveByOpening(pCurve->Copy());
	vector<GM2DCurve*> CurveArr;
	CurveArr.push_back(pCurve);

	long NumCurveArr = CurveArr.size();

	for(long iCurve = 0 ; iCurve < NumCurveArr ; iCurve++)
	{
		GM2DCurve* pSplitCurve = CurveArr[iCurve];
		if(iCurve == 0)
		{
			pDif->SetCenCurve(pSplitCurve);
			pDif->MakeLRLine_UserType();
			pDif->CutLineBySlab(pDif->mp_LLine, pDif->mp_LeftSlab);
		}
		else
		{
			MSLineLevelDifG* pSplitDif = new MSLineLevelDifG();
			pSplitDif->SetOwner(mp_Owner);
			pSplitDif->SetCenCurve(pSplitCurve);
			pSplitDif->m_dWallWidth = pDif->m_dWallWidth;
			pSplitDif->m_dWallHeight = pDif->m_dWallHeight;
			pSplitDif->SetIsUserLevelDif(true);
			pSplitDif->mp_LeftSlab = this;

			/////////////////////////////////////////////////////////////////////////////
			// MAKE LR Line
			pSplitDif->MakeLRLine_UserType(); // mylee 20150122
			pSplitDif->CutLineBySlab(pSplitDif->mp_LLine, pSplitDif->mp_LeftSlab);
			ma_EdgeWall.push_back(pSplitDif);
		}
	}

}

void MSSlabG::AddWell(MSWell* pWell, GM2DPolyline* pPoly)
{
	MSWell* pNewWell = (MSWell*)pWell->Copy();
	pNewWell->SetProfile(pPoly);
	pNewWell->SetOwner(this);

	mm_Well.insert(make_pair(pNewWell->GetID(), pNewWell));
	MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(mp_Owner->m_ID);
}

bool MSSlabG::AddWell(MSWell* pWell)
{
	vector<GM2DPolyline*> JoinedPolys;
	AcisGeomHelper::BooleanPolyline('I', *this->GetProfile(), *pWell->GetProfile(), JoinedPolys);
	int count = JoinedPolys.size();
	for(int i=0; i<count; i++)
	{
		AddWell(pWell, JoinedPolys[i]);
		delete JoinedPolys[i];
	}
	JoinedPolys.clear();

	MergeWell();
	TrimTrenchbyWellAll();

	return count > 0;
}

bool MSSlabG::Remove(MSWell* pWell)
{
	auto it = mm_Well.find(pWell->m_ID);
	if (it == mm_Well.end())	return false;
	mm_Well.erase(it);
	return true;
}

void MSSlabG::AddTrench(MSTrench* pTrench, GM2DPolyline* pPoly, GM2DPolyline* pCenterLine)
{
	MSTrench* pNewTrench = (MSTrench*)pTrench->Copy();
	pNewTrench->SetProfile(pPoly);
    pNewTrench->GetCenterPolyline()->CopyFrom(pCenterLine);
	pNewTrench->SetOwner(this);

	mm_Trench.insert(make_pair(pNewTrench->GetID(), pNewTrench));
	MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(mp_Owner->m_ID);

}

bool MSSlabG::AddTrench(MSTrench* pTrench)
{
    // Profile
	vector<GM2DPolyline*> JoinedPolys;
	AcisGeomHelper::BooleanPolyline('I', *this->GetProfile(), *pTrench->GetProfile(), JoinedPolys);
	int countP = JoinedPolys.size();

    // CenterLine
    CTypedPtrArray<CObArray, GM2DPolyline*> JoinedCenter;
    CTypedPtrArray<CObArray, GM2DCurve*> Segments;
    CArray<long, long> inRegion;
    GM2DPolyline* pCenter = pTrench->GetCenterPolyline();
    long centerCount = pCenter->GetSegments(Segments, this->GetProfile(), inRegion);
    CTypedPtrArray<CObArray, GM2DCurve*> inSegments;
    for(int i=0;i<centerCount;i++)
        if(inRegion[i]==-1)
			//inSegments.Add(Segments[i]); // 아래서 Segments, inSegments 지워줌 같은 주소 2번 지우게 되어 Segments[i]->Copy() 변경
            inSegments.Add(Segments[i]->Copy());
    
	if(inSegments.GetSize() > 3)
		pCenter->JoinCurves(JoinedCenter, inSegments);
	else
	{
		GM2DPolyline* pPoly = new GM2DPolyline(TRUE);
		for(long iSeg = 0 ; iSeg < inSegments.GetSize() ; iSeg++)
			pPoly->AddSegment(pPoly, inSegments[iSeg]);
		
		JoinedCenter.Add(pPoly);
	}
    int countC = JoinedCenter.GetSize();

    //vector<GM2DPolyline*> JoinedCenter;
    //AcisGeomHelper::BooleanPolyline('I', *this->GetProfile(), *pTrench->GetCenterPolyline(), JoinedCenter);
    //int countC = JoinedCenter.size();

    if (countP == 1 && countC == 1) // temporary
        AddTrench(pTrench, JoinedPolys[0], JoinedCenter[0]);

	for(int i=0; i<countP; i++)
        delete JoinedPolys[i];
    JoinedPolys.clear();

    for(int i=0; i<countC; i++)
        delete JoinedCenter[i];
    JoinedCenter.RemoveAll();
    
    for(int i=0; i<Segments.GetSize(); i++)
        delete Segments[i];
    Segments.RemoveAll();
    
    for(int i=0; i<inSegments.GetSize(); i++)
        delete inSegments[i];
    inSegments.RemoveAll();

	MergeTrench();

	TrimTrenchbyWellAll();

	return countP > 0;
}

void MSSlabG::MergeWell()
{
	if(mm_Well.size() < 2)
		return;

	map<long, MSWell*>::iterator WellIt;
	map<long, MSWell*>::iterator WellIt2;

	for(WellIt = mm_Well.begin() ; WellIt != mm_Well.end() ; WellIt++)
	{
		MSWell* pWell =WellIt->second;

		for(WellIt2 = mm_Well.begin() ; WellIt2 != mm_Well.end() ; WellIt2++)
		{
			MSWell* pUrWell = WellIt2->second;
			if(pWell->MergeToMe(pUrWell))
			{
				mm_Well.erase(pUrWell->m_ID);
				delete pUrWell;
				break;
			}

		}
	}
}

void MSSlabG::MergeTrench()
{
	if(mm_Trench.size() < 2)
		return;

	map<long, MSTrench*>::iterator TrenchIt;
	map<long, MSTrench*>::iterator TrenchIt2;
	map<long, MSTrench*> DeleteTrenchMap;

	for(TrenchIt = mm_Trench.begin() ; TrenchIt != mm_Trench.end() ; TrenchIt++)
	{
		if (DeleteTrenchMap.find(TrenchIt->first) != DeleteTrenchMap.end())
			continue;

		MSTrench* pTrench=TrenchIt->second;
		if(pTrench->m_nType == 2) continue;
		for(TrenchIt2 = mm_Trench.begin() ; TrenchIt2 != mm_Trench.end() ; TrenchIt2++)
		{
			if (DeleteTrenchMap.find(TrenchIt2->first) != DeleteTrenchMap.end())
				continue;

			MSTrench* pUrTrench = TrenchIt2->second;
			if(pTrench == pUrTrench) continue;
			if(pUrTrench->m_nType == 2) continue;
			if(pTrench->m_nType == 1 && pUrTrench->m_nType == 1)
			{
				if(pTrench->MergeToMe(pUrTrench))
				{
					DeleteTrenchMap.insert(make_pair(pUrTrench->m_ID, pUrTrench));
					break;
				}
			}
			else if(pTrench->m_nType == 1)
			{
				if(pUrTrench->MergeToMe(pTrench))
				{
					DeleteTrenchMap.insert(make_pair(pTrench->m_ID, pTrench));
					break;
				}
			}
			else if(pUrTrench->m_nType == 1)
			{
				if(pTrench->MergeToMe(pUrTrench))
				{
					DeleteTrenchMap.insert(make_pair(pUrTrench->m_ID, pUrTrench));
					break;
				}
			}
			else
			{
				MSTrench* pNewTrench = MSTrench::Merge(pTrench, pUrTrench);
				if(pNewTrench != NULL)
				{
					mm_Trench.insert(make_pair(pNewTrench->m_ID, pNewTrench));
					break;
				}
			}
// 			if(pTrench->MergeToMe(pUrTrench))
// 			{
// 				mm_Trench.erase(pUrTrench->m_ID);
// 				delete pUrTrench;
// 				break;
// 			}
		}
	}

	for (TrenchIt = DeleteTrenchMap.begin(); TrenchIt != DeleteTrenchMap.end(); TrenchIt++)
	{
		mm_Trench.erase(TrenchIt->first);
		delete TrenchIt->second;
	}

	DeleteTrenchMap.clear();
}

void MSSlabG::TrimTrenchbyWellAll()
{
	map<long, MSTrench*>::iterator TrenchIt;
	for(TrenchIt = mm_Trench.begin() ; TrenchIt != mm_Trench.end() ; TrenchIt++)
	{
		MSTrench* pTrench=TrenchIt->second;
		
		map<long, MSWell*>::iterator WellIt;
		for(WellIt = mm_Well.begin() ; WellIt != mm_Well.end() ; WellIt++)
		{
			MSWell* pWell =WellIt->second;
			TrimTrenchbyWell(pTrench, pWell);
		}
	}
}

void MSSlabG::TrimUserLevelDifbySlab()
{
	GM2DPolyline* pSlabPoly = GetProfile();
	for (int iHaunch = ma_EdgeWall.size() - 1; iHaunch >= 0; iHaunch--)
	{
		MSLineLevelDifG* pLevelDif = ma_EdgeWall[iHaunch];
		GM2DCurve* pCurve = pLevelDif->GetCurve();

		BOOL nRval = pSlabPoly->OverlapEx(pCurve);
		if (nRval > 0)
		{
			vector<GM2DCurve*> NewCurveArr;
			pSlabPoly->GetCurveInPolygon(pCurve, NewCurveArr);

			if (NewCurveArr.size() > 0)
			{
				pLevelDif->SetCenCurve(NewCurveArr[0]->Copy());
				pLevelDif->MakeLRLine_UserType();
				TrimEdgeWallbyOpening(pLevelDif);
			}

			GMObject::DeleteGMObject(NewCurveArr);
		}
		else
		{
			ma_EdgeWall.erase(ma_EdgeWall.begin() + iHaunch);
			delete pLevelDif;
		}
	}
}

void MSSlabG::TrimLevelDifbySlab()
{
	TrimUserLevelDifbySlab();
	TrimLevelDifAll();
}

void MSSlabG::TrimHaunchbySlab()
{
	GM2DPolyline* pSlabPoly = GetProfile();
	for (int iHaunch = ma_Haunches .size() - 1; iHaunch >= 0; iHaunch--)
	{
		MSSubHaunchG* pHaunch = ma_Haunches [iHaunch];
		GM2DCurve* pCurve = pHaunch->GetCurve();

		BOOL nRval = pSlabPoly->OverlapEx(pCurve);
		if (nRval > 0)
		{
			vector<GM2DCurve*> NewCurveArr;
			long NumNewCurve = pSlabPoly->GetCurveInPolygon(pCurve, NewCurveArr);
			if (NumNewCurve > 0)
			{
				pHaunch->SetCenCurve(NewCurveArr[0]);
				pHaunch->SetLines();
			}
		}
		else
		{
			ma_Haunches .erase(ma_Haunches .begin() + iHaunch);
			delete pHaunch;
		}
	}
	TrimHaunches();
}

void MSSlabG::TrimWellsbySlab()
{
	vector<long> DeleteIDArr;
	GM2DPolyline* pSlabPoly = GetProfile();
	for (auto WellIt = mm_Well.begin(); WellIt != mm_Well.end(); WellIt++)
	{
		MSWell* pWell = WellIt->second;
		if (pSlabPoly->OverlapEx(*pWell->GetProfile(), false) == false)
		{
			DeleteIDArr.push_back(WellIt->first);
			delete pWell;
		}
	}

	for (int i = 0; i < DeleteIDArr.size(); i++)
		mm_Well.erase(DeleteIDArr[i]);

	for (auto WellIt = mm_Well.begin(); WellIt != mm_Well.end(); WellIt++)
	{
		MSWell* pWell = WellIt->second;

		vector<GM2DPolyline*> JoinedPolys;
		AcisGeomHelper::BooleanPolyline('I', *pSlabPoly, *pWell->GetProfile(), JoinedPolys);
		int countP = JoinedPolys.size();
		if (countP != 1)
			continue;
		pWell->SetProfile(JoinedPolys[0]);
	}
}

void MSSlabG::TrimTrenchsbySlab()
{
	vector<long> DeleteIDArr;
	GM2DPolyline* pSlabPoly = GetProfile();
	for (auto TrenchIt = mm_Trench.begin(); TrenchIt != mm_Trench.end(); TrenchIt++)
	{
		MSTrench* pTrench = TrenchIt->second;
		if (pSlabPoly->OverlapEx(*pTrench->GetProfile(), false) == false)
		{
			DeleteIDArr.push_back(TrenchIt->first);
			delete pTrench;
		}
	}

	for (int i = 0; i < DeleteIDArr.size(); i++)
		mm_Trench.erase(DeleteIDArr[i]);

	for (auto TrenchIt = mm_Trench.begin(); TrenchIt != mm_Trench.end(); TrenchIt++)
	{
		MSTrench* pTrench = TrenchIt->second;

		vector<GM2DPolyline*> JoinedPolys;
		AcisGeomHelper::BooleanPolyline('I', *pSlabPoly, *pTrench->GetProfile(), JoinedPolys);
		int countP = JoinedPolys.size();
		if(countP != 1) 
			continue;
		pTrench->SetProfile(JoinedPolys[0]);
	}
}

void MSSlabG::TrimTrenchbyWell(MSTrench* pTrench, MSWell* pWell)
{
	GM2DPolyline* pTrenchPoly = pTrench->GetProfile();
	GM2DPolyline* pWellPoly = pWell->GetProfile();
	if(pTrenchPoly == NULL) return;
	if(pWellPoly == NULL) return;

	vector<GM2DPolyline*> JoinedPolys;
	AcisGeomHelper::BooleanPolyline('A', *pTrenchPoly, *pWellPoly, JoinedPolys);
	
	long NumJoined = JoinedPolys.size();
	for(long iJoined = 0 ; iJoined < NumJoined ; iJoined++)
	{
		GM2DPolyline* pJoinedPoly = JoinedPolys[iJoined];
		if(iJoined == 0)
		{
			pTrenchPoly->CopyFrom(pJoinedPoly);
			delete pJoinedPoly;
		}
		else
		{
			//ASSERT(0);
			MSTrench* pNewTrench = (MSTrench*)pTrench->Copy();
			pNewTrench->SetProfile(pJoinedPoly);
			mm_Trench.insert(make_pair(pNewTrench->m_ID, pNewTrench));
		}
	}
}

bool MSSlabG::OverlapWell(GM2DPolyline* pBasePoly)
{
	map<long, MSWell*>::iterator WellIt;
	for(WellIt = mm_Well.begin() ; WellIt != mm_Well.end() ; WellIt++)
	{
		MSWell* pWell =WellIt->second;
		GM2DPolyline* pWellPoly = pWell->GetProfile();
		if(pWellPoly->OverlapEx(*pBasePoly))
		{
			return true;
		}
	}

	return false;
}

GM2DVector* MSSlabG::GetVec( long Index )
{
	return mp_PolylineInfo->GetVec(Index);
}

void MSSlabG::ReMakeLevelDif(bool bMakeSlab)
{
	MSFloor* pFloor = NULL;
	if(mp_Owner == NULL) 
	{
		ASSERT(0);
		return;
	}
	if(mp_Owner->GetType() == MSElement::msFloor)
		pFloor = (MSFloor*)mp_Owner;
	else
	{
		ASSERT(0);
		return;
	}
	// LevelDif 삭제
	long nLevelDif = ma_LevelDif.size();
	for(long iLevelDif=0; iLevelDif<nLevelDif; iLevelDif++)
	{
		//pFloor->FindAndRemoveLevelDif2LineMembers(ma_LevelDif[iLevelDif]);
		delete ma_LevelDif[iLevelDif];
	}
	ma_LevelDif.clear();

// 	nLevelDif = ma_LevelDifOnEdge.size();
// 	for(long iLevelDif=0; iLevelDif<nLevelDif; iLevelDif++)
// 		pFloor->FindAndRemoveLevelDif2LineMembers(ma_LevelDifOnEdge[iLevelDif]);
// 	ma_LevelDifOnEdge.clear();

	// SlabLevelZone 전부 삭제
	long NumLevelPoly = ma_LevelPoly.size();
	for(long iLevelPoly = 0 ; iLevelPoly < NumLevelPoly ; iLevelPoly++)
		ma_LevelPoly[iLevelPoly]->m_IsDelete = true;

	// LevelDif 생성
	if(mp_Owner->GetType() == MSElement::msFloor)
	{
		pFloor->InsertLevelDif2SlabByLevelZone(this);
	}

	TrimLevelDifAll();
}


vector<MSLinkMemberG*> MSSlabG::GetLevelDifGAll(bool bGetAllEdgeWall, bool bGetSameLRZLevel)
{
	vector<MSLinkMemberG*> LevelDifArr;
	long nLevelDif = ma_LevelDif.size();
	for(long iLevelDif=0; iLevelDif<nLevelDif; iLevelDif++)
	{
		LevelDifArr.push_back(ma_LevelDif[iLevelDif]);
	}

	if(bGetAllEdgeWall)
	{
		long nLevelDif = ma_EdgeWall.size();
		for(long iLevelDif=0; iLevelDif<nLevelDif; iLevelDif++)
			LevelDifArr.push_back(ma_EdgeWall[iLevelDif]);
	}

	// 엣지 LevelDif 는 사용 안함 // 20141120
// 	MSLineLevelDifG* tmpLine = NULL;
// 	nLevelDif = ma_LevelDifOnEdge.size();
// 	for(long iLevelDif=0; iLevelDif<nLevelDif; iLevelDif++)
// 	{
// 		if(bGetSameLRZLevel == false && ma_LevelDifOnEdge[iLevelDif]->IsSameLRZLevel()) continue;
// 
// 		tmpLine = ma_LevelDifOnEdge[iLevelDif];
// 		if(bGetAllDifOnEdge == true || tmpLine->mp_LeftSlab != NULL && tmpLine->mp_RightSlab != NULL)	//엣지 라인은 좌우에 슬래브가 있을때만 유효하다.
// 			LevelDifArr.push_back(tmpLine);
// 		else
// 			tmpLine->SetLines(msSlabG, 0);
// 	}

	return LevelDifArr;
}

// bool MSSlabG::GetLevelDifID(MSLineLevelDifG* pLevelDifG, long& ID)
// {
// 	long NumSlabZone = ma_LevelPoly.size();
// 	for(long iSlabZone = 0 ; iSlabZone < NumSlabZone ;iSlabZone++)
// 	{
// 		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
// 		if(pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;
// 
// 
// 		bool bRVal = pSlabZone->GetLevelDifID(pLevelDifG, ID);
// 		if(bRVal == true)
// 			return true;
// 	}
// 
// 	return false;
// }
// 
// bool MSSlabG::SetLevelDifID(MSLineLevelDifG* pLevelDifG, long ID)
// {
// 	return false;
// }

bool MSSlabG::GetLevelDifWidth(MSLineLevelDifG* pLevelDifG, double& dWidth)
{
	long NumSlabZone = ma_LevelPoly.size();
	for(long iSlabZone = 0 ; iSlabZone < NumSlabZone ;iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if(pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;


		bool bRVal = pSlabZone->GetLevelDifWidth(pLevelDifG, dWidth);
		if(dWidth < 0)
			dWidth = 0;
		if(bRVal == true)
			return true;
	}

	return false;
}

bool MSSlabG::SetLevelDifWidth(MSLineLevelDifG* pLevelDifG, double dWidth)
{
	if(dWidth < DTOL_GM)
		dWidth = -10;
	bool bRVal = false;

	long NumSlabZone = ma_LevelPoly.size();
	for(long iSlabZone = 0 ; iSlabZone < NumSlabZone ;iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if(pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;
		
		if(pSlabZone->SetLevelDifWidth(pLevelDifG, dWidth))
			bRVal = true;
	}

	return bRVal;
}

bool MSSlabG::GetLevelDifHasWall(MSLineLevelDifG* pLevelDifG, bool& HasWall)
{
	long NumSlabZone = ma_LevelPoly.size();
	for(long iSlabZone = 0 ; iSlabZone < NumSlabZone ;iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if(pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;


		bool bRVal = pSlabZone->GetLevelDifHasWall(pLevelDifG, HasWall);
		if(bRVal == true)
			return true;
	}

	return false;
}

bool MSSlabG::SetLevelDifHasWall(MSLineLevelDifG* pLevelDifG, bool HasWall)
{
	bool bRVal = false;

	long NumSlabZone = ma_LevelPoly.size();
	for(long iSlabZone = 0 ; iSlabZone < NumSlabZone ;iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if(pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;

		if(pSlabZone->SetLevelDifHasWall(pLevelDifG, HasWall))
			bRVal = true;
	}

	return bRVal;
}

bool MSSlabG::GetLevelDifHasHaunch(MSLineLevelDifG* pLevelDifG, bool& HasHaunch)
{
	long NumSlabZone = ma_LevelPoly.size();
	for(long iSlabZone = 0 ; iSlabZone < NumSlabZone ;iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if(pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;


		bool bRVal = pSlabZone->GetLevelDifHasHaunch(pLevelDifG, HasHaunch);
		if(bRVal == true)
			return true;
	}

	return false;
}

bool MSSlabG::SetLevelDifHasHaunch(MSLineLevelDifG* pLevelDifG, bool HasHaunch)
{
	bool bRVal = false;

	long NumSlabZone = ma_LevelPoly.size();
	for(long iSlabZone = 0 ; iSlabZone < NumSlabZone ;iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if(pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;

		if(pSlabZone->SetLevelDifHasHaunch(pLevelDifG, HasHaunch))
			bRVal = true;
	}

	return bRVal;
}

bool MSSlabG::GetLevelDifHaunchX(MSLineLevelDifG* pLevelDifG, double& HaunchX)
{
	long NumSlabZone = ma_LevelPoly.size();
	for(long iSlabZone = 0 ; iSlabZone < NumSlabZone ;iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if(pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;


		bool bRVal = pSlabZone->GetLevelDifHaunchX(pLevelDifG, HaunchX);
		if(bRVal == true)
			return true;
	}

	return false;
}

bool MSSlabG::SetLevelDifHaunchX(MSLineLevelDifG* pLevelDifG, double HaunchX)
{
	bool bRVal = false;

	long NumSlabZone = ma_LevelPoly.size();
	for(long iSlabZone = 0 ; iSlabZone < NumSlabZone ;iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if(pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;

		if(pSlabZone->SetLevelDifHaunchX(pLevelDifG, HaunchX))
			bRVal = true;
	}

	return bRVal;
}

bool MSSlabG::GetLevelDifHaunchY(MSLineLevelDifG* pLevelDifG, double& HaunchY)
{
	long NumSlabZone = ma_LevelPoly.size();
	for(long iSlabZone = 0 ; iSlabZone < NumSlabZone ;iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if(pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;


		bool bRVal = pSlabZone->GetLevelDifHaunchY(pLevelDifG, HaunchY);
		if(bRVal == true)
			return true;
	}

	return false;
}

bool MSSlabG::SetLevelDifHaunchY(MSLineLevelDifG* pLevelDifG, double HaunchY)
{
	bool bRVal = false;

	long NumSlabZone = ma_LevelPoly.size();
	for(long iSlabZone = 0 ; iSlabZone < NumSlabZone ;iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if(pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;

		if(pSlabZone->SetLevelDifHaunchY(pLevelDifG, HaunchY))
			bRVal = true;
	}

	return bRVal;
}

bool MSSlabG::GetLevelDifHaunchAngle(MSLineLevelDifG* pLevelDifG, double& HaunchAngle)
{
	long NumSlabZone = ma_LevelPoly.size();
	for (long iSlabZone = 0; iSlabZone < NumSlabZone; iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if (pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;


		bool bRVal = pSlabZone->GetLevelDifHaunchAngle(pLevelDifG, HaunchAngle);
		if (bRVal == true)
			return true;
	}

	return false;
}

bool MSSlabG::SetLevelDifHaunchAngle(MSLineLevelDifG* pLevelDifG, double HaunchAngle)
{
	bool bRVal = false;

	long NumSlabZone = ma_LevelPoly.size();
	for (long iSlabZone = 0; iSlabZone < NumSlabZone; iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if (pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;

		if (pSlabZone->SetLevelDifHaunchAngle(pLevelDifG, HaunchAngle))
			bRVal = true;
	}

	return bRVal;
}

bool MSSlabG::GetLevelDifUseHaunchMaterial(MSLineLevelDifG* pLevelDifG, bool& value)
{
	long NumSlabZone = ma_LevelPoly.size();
	for (long iSlabZone = 0; iSlabZone < NumSlabZone; iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if (pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;


		bool bRVal = pSlabZone->GetLevelDifUseHaunchMaterial(pLevelDifG, value);
		if (bRVal == true)
			return true;
	}

	return false;
}

bool MSSlabG::SetLevelDifUseHaunchMaterial(MSLineLevelDifG* pLevelDifG, bool value)
{
	bool bRVal = false;

	long NumSlabZone = ma_LevelPoly.size();
	for (long iSlabZone = 0; iSlabZone < NumSlabZone; iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if (pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;

		if (pSlabZone->SetLevelDifUseHaunchMaterial(pLevelDifG, value))
			bRVal = true;
	}

	return bRVal;
}

bool MSSlabG::GetLevelDifUseBackFillFormwork(MSLineLevelDifG* pLevelDifG, bool& value)
{
	long NumSlabZone = ma_LevelPoly.size();
	for (long iSlabZone = 0; iSlabZone < NumSlabZone; iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if (pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;


		bool bRVal = pSlabZone->GetLevelDifUseBackFillFormwork(pLevelDifG, value);
		if (bRVal == true)
			return true;
	}

	return false;
}

bool MSSlabG::SetLevelDifUseBackFillFormwork(MSLineLevelDifG* pLevelDifG, bool value)
{
	bool bRVal = false;

	long NumSlabZone = ma_LevelPoly.size();
	for (long iSlabZone = 0; iSlabZone < NumSlabZone; iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if (pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;

		if (pSlabZone->SetLevelDifUseBackFillFormwork(pLevelDifG, value))
			bRVal = true;
	}

	return bRVal;
}

bool MSSlabG::GetLevelDifHaunchMaterialID(MSLineLevelDifG* pLevelDifG, int& value)
{
	long NumSlabZone = ma_LevelPoly.size();
	for (long iSlabZone = 0; iSlabZone < NumSlabZone; iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if (pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;


		bool bRVal = pSlabZone->GetLevelDifHaunchMaterialID(pLevelDifG, value);
		if (bRVal == true)
			return true;
	}

	return false;
}

bool MSSlabG::SetLevelDifHaunchMaterialID(MSLineLevelDifG* pLevelDifG, int value)
{
	bool bRVal = false;

	long NumSlabZone = ma_LevelPoly.size();
	for (long iSlabZone = 0; iSlabZone < NumSlabZone; iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if (pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;

		if (pSlabZone->SetLevelDifHaunchMaterialID(pLevelDifG, value))
			bRVal = true;
	}

	return bRVal;
}

bool MSSlabG::GetLevelDifHaunchUsedStoryMaterial(MSLineLevelDifG* pLevelDifG, bool& value)
{
	long NumSlabZone = ma_LevelPoly.size();
	for (long iSlabZone = 0; iSlabZone < NumSlabZone; iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if (pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;


		bool bRVal = pSlabZone->GetLevelDifHaunchUsedStoryMaterial(pLevelDifG, value);
		if (bRVal == true)
			return true;
	}

	return false;
}

bool MSSlabG::SetLevelDifHaunchUsedStoryMaterial(MSLineLevelDifG* pLevelDifG, bool value)
{
	bool bRVal = false;

	long NumSlabZone = ma_LevelPoly.size();
	for (long iSlabZone = 0; iSlabZone < NumSlabZone; iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if (pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;

		if (pSlabZone->SetLevelDifHaunchUsedStoryMaterial(pLevelDifG, value))
			bRVal = true;
	}

	return bRVal;
}

void MSSlabG::SetLevelDifWidthInSlabLevelZone()
{
	vector<MSLinkMemberG*> LevelDifArr = GetLevelDifGAll(false, true);
	long nLevelDif = LevelDifArr.size();
	// pSlabZone->ma_PolyLineWidth <== LevelDif Width 입력
	//처음 생선된 MSSlabLevelZone만 LevelDifG의 정보를 셋팅 한다.

	for(long iLevelDif = 0 ; iLevelDif < nLevelDif ; iLevelDif++)
	{
		MSLinkMemberG* pLevelDifG = LevelDifArr[iLevelDif];
		if(pLevelDifG == NULL) continue;

		long NumSlabZone = ma_LevelPoly.size();
		for(long iSlabZone = 0 ; iSlabZone < NumSlabZone ;iSlabZone++)
		{
			MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
			if(pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;

			//if(pSlabZone->m_IsNewSlabZone == false) continue;

			double dWidth = 0;
			bool bRavl = pSlabZone->GetLevelDifWidth((MSLineLevelDifG*)pLevelDifG, dWidth);
			if(bRavl == true/* && fabs(pSlabZone->m_dPreWidth - dWidth) < DTOL_GM*/)
			{
				bool bRVal2 = pSlabZone->SetLevelDifWidth((MSLineLevelDifG*)pLevelDifG, pLevelDifG->GetThick());
				if(bRVal2 == true)
					break;
			}

// 			if(bRavl == true && dWidth < 0)
// 				pSlabZone->SetLevelDifWidth(pLevelDifG, 0);

// 			bool bRVal = pSlabZone->SetLevelDifWidth(pLevelDifG, pLevelDifG->GetThick());
// 			if(bRVal == true)
// 				break;
		}
	}

	/*vector<MSLineLevelDifG*> LevelDifArr = GetLevelDifGAll();
	long nLevelDif = LevelDifArr.size();

	// pSlabZone->ma_PolyLineWidth <== LevelDif Width 입력
	long NumSlabZone = ma_LevelPoly.size();
	for(long iSlabZone = 0 ; iSlabZone < NumSlabZone ;iSlabZone++)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
		if(pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;

		long NumCurve = pSlabZone->mp_Polyline->GetNumLines();
		for(long iCurve = 0 ; iCurve < NumCurve ; iCurve++)
		{
			GM2DCurve* pCurve = pSlabZone->mp_Polyline->GetCurve(iCurve);

			for(long iLevelDif = 0 ; iLevelDif < nLevelDif ; iLevelDif++)
			{
				MSLineLevelDifG* pLevelDifG = LevelDifArr[iLevelDif];
				if(pLevelDifG == NULL) continue;

				bool isSVecSame = false;
				bool isEVecSame = false;

				if(pCurve->m_P0.SameVector4DTOL(pLevelDifG->mp_Curve->m_P0))
					isSVecSame = true;
				else if(pCurve->m_P0.SameVector4DTOL(pLevelDifG->mp_Curve->m_P1))
					isSVecSame = true;

				if(pCurve->m_P1.SameVector4DTOL(pLevelDifG->mp_Curve->m_P0))
					isEVecSame = true;
				else if(pCurve->m_P1.SameVector4DTOL(pLevelDifG->mp_Curve->m_P1))
					isEVecSame = true;

				if(isSVecSame == true && isEVecSame == true)
					pSlabZone->ma_PolyLineWidth[iCurve] = pLevelDifG->GetThick();
			}
			delete pCurve;
		}
	}*/
}

void MSSlabG::GetLevelDifWidthInSlabLevelZone()
{
	vector<MSLinkMemberG*> LevelDifArr = GetLevelDifGAll(false, true);
	long nLevelDif = LevelDifArr.size();

	for(long iLevelDif=0; iLevelDif<nLevelDif; iLevelDif++)
	{
		MSLinkMemberG* pLevelDif = LevelDifArr[iLevelDif];

		// pSlabZone->ma_PolyLineWidth <== LevelDif Width 입력
		long NumSlabZone = ma_LevelPoly.size();
		for(long iSlabZone = 0 ; iSlabZone < NumSlabZone ;iSlabZone++)
		{
			MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
			if(pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;
			pSlabZone->m_IsNewSlabZone = false;

			double dWidth;
			bool bRval = pSlabZone->GetLevelDifWidth((MSLineLevelDifG*)pLevelDif, dWidth);
			if(bRval == true)
			{
				((MSLineLevelDifG*)pLevelDif)->m_dWallWidth = dWidth;
				break;
			}
		}
	}
/*
	vector<MSLineLevelDifG*> LevelDifArr = GetLevelDifGAll();
	long nLevelDif = LevelDifArr.size();

	for(long iLevelDif=0; iLevelDif<nLevelDif; iLevelDif++)
	{
		MSLineLevelDifG* pLevelDif = LevelDifArr[iLevelDif];

		// pSlabZone->ma_PolyLineWidth <== LevelDif Width 입력
		long NumSlabZone = ma_LevelPoly.size();
		for(long iSlabZone = 0 ; iSlabZone < NumSlabZone ;iSlabZone++)
		{
			MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabZone];
			if(pSlabZone == NULL || pSlabZone->m_IsDelete == true) continue;

			long NumCurve = pSlabZone->mp_Polyline->GetNumLines();
			for(long iCurve = 0 ; iCurve < NumCurve ; iCurve++)
			{
				GM2DCurve* pCurve = pSlabZone->mp_Polyline->GetCurve(iCurve);
				bool isSVecSame = false;
				bool isEVecSame = false;

				if(pCurve->m_P0.SameVector4DTOL(pLevelDif->mp_Curve->m_P0))
					isSVecSame = true;
				else if(pCurve->m_P0.SameVector4DTOL(pLevelDif->mp_Curve->m_P1))
					isSVecSame = true;

				if(pCurve->m_P1.SameVector4DTOL(pLevelDif->mp_Curve->m_P0))
					isEVecSame = true;
				else if(pCurve->m_P1.SameVector4DTOL(pLevelDif->mp_Curve->m_P1))
					isEVecSame = true;

				delete pCurve;
				if(isSVecSame == true && isEVecSame == true)
				{
					pLevelDif->m_dWallWidth = pSlabZone->ma_PolyLineWidth[iCurve];
					break;
				}
			}
		}
	}*/
}

void MSSlabG::InitializeParameter()
{
	DeleteSlabLineLevelDifAll();

	MSFloor* pFloor = GetFloor();
// 	map<long, long>::iterator it;
// 
// 	for(it = mm_OpeningID.begin() ; it != mm_OpeningID.end() ; it++)
// 	{
// 		MSCompoG* pCompoG = pFloor->GetCompoG(it->second);
//		if(pCompoG != NULL)
// 			pCompoG->UpdateBoundaryVisibilities();
// 	}
	mm_OpeningID.clear();

	map<long, MSLevelZone*>::iterator it;
	map<long, MSLevelZone*> ModifyLevelZoneMap;
	
	long NumSlabLevelZone = ma_LevelPoly.size();
	for (long iSlabLevelZone = NumSlabLevelZone-1 ; iSlabLevelZone >= 0; iSlabLevelZone--)
	{
		MSSlabLevelZone* pSlabZone = ma_LevelPoly[iSlabLevelZone];
		if(pSlabZone == NULL || pSlabZone->m_IsDelete == true || pSlabZone->mp_LevelZone == NULL) continue;
		MSLevelZone* pLevelZone = pSlabZone->mp_LevelZone;

		// Modify에서 실행함
		//RemoveLevelZone(pLevelZone);
		//pLevelZone->DeleteLineLevelDifAll(); // 다른부재가 LevelDif 사용함 삭제하면안됨
		if(!pFloor->GetRunModifyObjectCompoG())
			pLevelZone->DeleteLevelZoneInfo();
		ModifyLevelZoneMap.insert(make_pair(pLevelZone->m_ID, pLevelZone));
	}
	
	for(it = ModifyLevelZoneMap.begin() ; it != ModifyLevelZoneMap.end() ; it++)
	{
		MSLevelZone* pLevelZone = it->second;
		pFloor->ModifyLevelZoneByElement(pLevelZone);
	}
	
	DeleteSupport();

	//ma_LevelPoly.clear();
	ma_LevelDif.clear();
	ma_LevelDifOnEdge.clear();

	// 연결된 슬래브의 오프닝의 정보 삭제
	for (int iConn = 0; iConn < ma_Connections.size(); iConn++)
	{
		MSConnection* pConn = ma_Connections[iConn];
		MSSlabG* pPartner = (MSSlabG*)pConn->GetPartner(this);
		if(pPartner == nullptr || pPartner->GetType() != MSElement::msSlabG)
			continue;
		
		for (int iOpen = pPartner->mp_PolylineInfo->ma_Opening.size() -1 ; iOpen >= 0 ; iOpen--)
		{
			MSOpening* pOpening = pPartner->mp_PolylineInfo->ma_Opening[iOpen];
			if (pOpening->HasMadeByCompoGID(m_ID))
				pPartner->mp_PolylineInfo->RemoveOpening(pOpening);
		}
	}

	MSCompoG::InitializeParameter();

}

bool MSSlabG::MakeBoundary()
{
// 	GM2DPolyline* pProFile = GetProfile();
// 	if (pProFile->SamePolyLine(mp_Boundary) == true)
// 		return true;
// 
// 	mp_Boundary->ResetPolyline();
// 	
// 	if(pProFile == NULL)
// 		return false;
// 
// 	mp_Boundary->CopyFrom(pProFile);
// 	if(mp_Boundary->ma_Point.GetSize() > 0)
// 		mp_Boundary->MakeCCW();
	return true;
}

void MSSlabG::FindPlaneEquation(double p1[3], double p2[3], double p3[3], double ret[4])
{
	double a1 = p2[0] - p1[0];
	double a2 = p2[1] - p1[1];
	double a3 = p2[2] - p1[2];
	double b1 = p3[0] - p1[0];
	double b2 = p3[1] - p1[1];
	double b3 = p3[2] - p1[2];
	double normal[3] = { a2*b3-a3*b2, -(a1*b3-a3*b1), a1*b2-a2*b1 };

	ret[0] = normal[0];
	ret[1] = normal[1];
	ret[2] = normal[2];
	ret[3] = -(ret[0]*p1[0] + ret[1]*p1[1] + ret[2]*p1[2]);
}

bool MSSlabG::GetPlaneEquation(double ret[4])
{
	if (IsSlopeMember())
	{
		if (m_nSlopeMethod == SlopeMethod::ThreePoint_0)
		{
			if (ma_SlopePoints.size() == 3)
			{
				double p1[3] = { ma_SlopePoints[0].m_X, ma_SlopePoints[0].m_Y, ma_SlopeZOffset[0] };
				double p2[3] = { ma_SlopePoints[1].m_X, ma_SlopePoints[1].m_Y, ma_SlopeZOffset[1] };
				double p3[3] = { ma_SlopePoints[2].m_X, ma_SlopePoints[2].m_Y, ma_SlopeZOffset[2] };
				FindPlaneEquation(p1, p2, p3, ret);
				return true;
			}
		}
		else if (m_nSlopeMethod == SlopeMethod::SlopeArrow_2)
		{
			if (ma_SlopePoints.size() == 2)
			{
				double p1[3] = { ma_SlopePoints[0].m_X, ma_SlopePoints[0].m_Y, ma_SlopeZOffset[0] };
				double p2[3] = { ma_SlopePoints[1].m_X, ma_SlopePoints[1].m_Y, ma_SlopeZOffset[1] };
				double dx = p2[0] - p1[0];
				double dy = p2[1] - p1[1];
				double p3[3] = { p1[0] - dy, p1[1] + dx, ma_SlopeZOffset[0] };
				FindPlaneEquation(p1, p2, p3, ret);
				return true;
			}
		}
	}

	return false;
}

double MSSlabG::GetThick_ZDir()
{
	double thick = GetThick();
	double ret[4] = { 0 };

	// 경사 슬래브이고  (SlopeArrow / 3 Point) 의 경우 - Point Move 방식은 Circular Ramp 개발시 정비 예정임
	if (GetPlaneEquation(ret))
	{
		double leng2 = ret[0] * ret[0] + ret[1] * ret[1] + ret[2] * ret[2];
		double leng_on_xy_plane2 = leng2 - (ret[0] * ret[0] + ret[1] * ret[1]);
		if (leng_on_xy_plane2 != 0.0)
		{
			double factor = sqrt(leng2) / sqrt(leng_on_xy_plane2);
			thick *= factor;
		}
	}

	return thick;
}

bool MSSlabG::IsSlopeMember()
{
	long NumSlopePntIdx = ma_SlopePointIdx.size();
	long NumSlopePnt = ma_SlopePoints.size();

	if (NumSlopePnt + NumSlopePntIdx == 0)
		return false;
	else
		return true;
}

void MSSlabG::ClearSlopeInfo()
{
	m_nSlopeMethod = MSSlabG::ThreePoint_0;
	ma_SlopePoints.clear();
	ma_SlopePointIdx.clear();
	ma_SlopeZOffset.clear();
}

double MSSlabG::GetZOffsetOnSlabTopPlane(double x, double y)
{
	double offset = m_dOffset;
	if (ma_SlopeZOffset.size() == 0) return offset;

	if (m_nSlopeMethod==SlopeMethod::ThreePoint_0)
	{
		double p1[3] = { ma_SlopePoints[0].m_X, ma_SlopePoints[0].m_Y, ma_SlopeZOffset[0] };
		double p2[3] = { ma_SlopePoints[1].m_X, ma_SlopePoints[1].m_Y, ma_SlopeZOffset[1] };
		double p3[3] = { ma_SlopePoints[2].m_X, ma_SlopePoints[2].m_Y, ma_SlopeZOffset[2] };
		double ret[4] = { 0 };
		FindPlaneEquation(p1, p2, p3, ret);

		double z = (-ret[3] - ret[0]*x - ret[1]*y) / ret[2];
		offset += z;
	}
	else if (m_nSlopeMethod==SlopeMethod::SlopeArrow_2)
	{
		double p1[3] = { ma_SlopePoints[0].m_X, ma_SlopePoints[0].m_Y, ma_SlopeZOffset[0] };
		double p2[3] = { ma_SlopePoints[1].m_X, ma_SlopePoints[1].m_Y, ma_SlopeZOffset[1] };
		///////////////////////////////////////
		double uvec[2] = { p2[0]-p1[0], p2[1]-p1[1] };
		double uvec_left[2] = { -uvec[1], uvec[0] };
		double leng = fabs(p1[0]-p2[0]) + fabs(p1[1]-p2[1]);
		///////////////////////////////////////
		double p3[3] = { p1[0] + leng * uvec_left[0], p1[1] + leng * uvec_left[1], p1[2] };
		double ret[4] = { 0 };
		FindPlaneEquation(p1, p2, p3, ret);

		double z = (-ret[3] - ret[0]*x - ret[1]*y) / ret[2];
		offset += z;
	}
	else if (m_nSlopeMethod==SlopeMethod::PointOffset_1)
	{
		// Plane Only - not supported -> mylee added 0706
		double z = AcisGeomHelper::GetTopSurfaceZ(m_nEntityTag_Cur, x, y);
		MSFloor* pFloor = (MSFloor*)(this->mp_Owner);
		double floor_top = pFloor->GetSlabLevel() + pFloor->GetSlabHeight();
		offset = z - floor_top;
	}

	return offset;
}

void MSSlabG::GetProfileZOffset_BySlope(vector<double>& zoffset_list)
{
	zoffset_list.clear();
	if (ma_SlopeZOffset.size() == 0) return;

	GM2DPolyline* profile = GetProfile();

	if (m_nSlopeMethod==SlopeMethod::ThreePoint_0)
	{
		double p1[3] = { ma_SlopePoints[0].m_X, ma_SlopePoints[0].m_Y, ma_SlopeZOffset[0] };
		double p2[3] = { ma_SlopePoints[1].m_X, ma_SlopePoints[1].m_Y, ma_SlopeZOffset[1] };
		double p3[3] = { ma_SlopePoints[2].m_X, ma_SlopePoints[2].m_Y, ma_SlopeZOffset[2] };
		double ret[4] = { 0 };
		FindPlaneEquation(p1, p2, p3, ret);

		for (int i=0 ; i<profile->ma_Point.GetSize() ; ++i)
		{
			double x = profile->ma_Point[i]->m_X;
			double y = profile->ma_Point[i]->m_Y;
			double z = (-ret[3] - ret[0]*x - ret[1]*y) / ret[2];

			zoffset_list.push_back( z );
		}
	}
	else if (m_nSlopeMethod==SlopeMethod::PointOffset_1)
	{
		map<int, double> mapIdxToZOffset;
		for (int i=0 ; i<ma_SlopePointIdx.size() ; ++i)
		{
			int idx = ma_SlopePointIdx[i];
			mapIdxToZOffset[idx] = ma_SlopeZOffset[i];
		}
		for (int i=0 ; i<profile->ma_Point.GetSize() ; ++i)
		{
			if (mapIdxToZOffset.count(i) > 0)
				zoffset_list.push_back( mapIdxToZOffset[i] );
			else
				zoffset_list.push_back( 0.0 );
		}
	}
	else if (m_nSlopeMethod==SlopeMethod::SlopeArrow_2)
	{
		double p1[3] = { ma_SlopePoints[0].m_X, ma_SlopePoints[0].m_Y, ma_SlopeZOffset[0] };
		double p2[3] = { ma_SlopePoints[1].m_X, ma_SlopePoints[1].m_Y, ma_SlopeZOffset[1] };
		///////////////////////////////////////
		double uvec[2] = { p2[0]-p1[0], p2[1]-p1[1] };
		double uvec_left[2] = { -uvec[1], uvec[0] };
		double leng = fabs(p1[0]-p2[0]) + fabs(p1[1]-p2[1]);
		///////////////////////////////////////
		double p3[3] = { p1[0] + leng * uvec_left[0], p1[1] + leng * uvec_left[1], p1[2] };
		double ret[4] = { 0 };
		FindPlaneEquation(p1, p2, p3, ret);

		for (int i=0 ; i<profile->ma_Point.GetSize() ; ++i)
		{
			double x = profile->ma_Point[i]->m_X;
			double y = profile->ma_Point[i]->m_Y;
			double z = (-ret[3] - ret[0]*x - ret[1]*y) / ret[2];

			zoffset_list.push_back( z );
		}
	}
}

void MSSlabG::GetZLevels_Slope(double& top, double& bot)
{
	if (!IsSlopeMember()) return;
	top = 0;
	bot = 0;

	vector<double> zoffset_list;
	GetProfileZOffset_BySlope(zoffset_list);
	for (int i = 0; i < zoffset_list.size(); ++i)
	{
		double cur = zoffset_list[i];
		if (i == 0)
		{
			top = bot = cur;
		}
		else
		{
			if (cur > top) top = cur;
			if (cur < bot) bot = cur;
		}
	}

	double dLevel = GetNextSlabLevel() + m_dOffset;
	double dFullSlabLevelZoneLevel = GetFullSlabLevelZoneLevel();
	top += dLevel + dFullSlabLevelZoneLevel;
	bot += dLevel + dFullSlabLevelZoneLevel;
}

bool MSSlabG::IsSameSlopeSlab(MSSlabG* pUrSlabG)
{
	if (!IsSlopeMember() && !pUrSlabG->IsSlopeMember())
	{
//RH-1743 버그수정을 위해 주석처리
//		if (fabs(m_dOffset - pUrSlabG->m_dOffset) < DTOL_GM)
			return true;
//		else
//			return false;
	}

	if (IsSlopeMember() && pUrSlabG->IsSlopeMember())
	{
		bool bPlane = (m_nSlopeMethod == SlopeMethod::ThreePoint_0 || m_nSlopeMethod == SlopeMethod::SlopeArrow_2);
		bool bUrPlane = (pUrSlabG->m_nSlopeMethod == SlopeMethod::ThreePoint_0 || pUrSlabG->m_nSlopeMethod == SlopeMethod::SlopeArrow_2);
		if (bPlane && bUrPlane)
		{
			double My[4] = { 0 };
			double Ur[4] = { 0 };
			GetPlaneEquation(My);
			pUrSlabG->GetPlaneEquation(Ur);

			if (My[0]*Ur[0] + My[1]*Ur[1] + My[2]*Ur[2] > 0.99) // Same Direction
			{
				if (fabs(My[3] - Ur[3]) < DTOL_GM) // Same Z Position
					return true;
			}
		}
	}

	return false;
}

long MSSlabG::Contains( GM2DPolyline* pPoly, bool bIncludeOnLine, double tolerance)
{
	long nRValCnt = 0;

	GM2DVector CenVec = pPoly->MassCenter();
	long nCenContains = Contains(&CenVec, tolerance);

	long NumCurve = pPoly->GetNumLines();
	for (long i = 0 ; i	 < NumCurve; i++)
	{
		GM2DCurve* pCurve = pPoly->GetCurve(i);

		bool bRVal = Contains(pCurve, bIncludeOnLine, tolerance);
		delete pCurve;
		if(bRVal == true)
			nRValCnt++;

		if(bRVal == true && nCenContains != -1)
			return 1;
	}

	if(nRValCnt == 0)
		return 0;

	return -1;
}

bool MSSlabG::Contains(GM2DCurve* pCurve, bool bIncludeOnLine, double tolerance)
{
	//return mp_PolylineInfo->Contains(pCurve, bIncludeOnLine, tolerance);

	long CheckVal = 0;
	if (bIncludeOnLine == true)
		CheckVal = 1;

	//3점을 비교함
	GM2DVector Cpt = pCurve->PointAtParam(0.5);
	GM2DVector Spt = pCurve->PointAtParam(0.01);
	GM2DVector Ept = pCurve->PointAtParam(0.99);

	long nRCVal = Contains(&Cpt, tolerance);
	long nRSVal = Contains(&Spt, tolerance);
	long nREVal = Contains(&Ept, tolerance);

	if (nRCVal < CheckVal && nRSVal < CheckVal && nREVal < CheckVal)
		return true;

	return false;
}

long MSSlabG::Contains(GM2DVector* pVec, double tolerance)
{
	//return mp_PolylineInfo->Contains(pVec->m_X, pVec->m_Y, tolerance);
	long NumOpening = mp_PolylineInfo->ma_Opening.size();
	for(long iOpening = 0 ; iOpening < NumOpening ; iOpening++)
	{
		MSOpening* pOpening = mp_PolylineInfo->ma_Opening[iOpening];
		if(pOpening == NULL) continue;
		GM2DPolyline* pOpenPoly = pOpening->GetProfile();

		long nRval = pOpenPoly->Contains(pVec, tolerance);
		if(nRval < 1)
			return 1;
	}

	GM2DPolyline* pSlabPoly = GetProfile();

	long nRval = pSlabPoly->Contains(pVec, tolerance);

	return nRval;
}

bool MSSlabG::IsSlabNOpeningOnLine(GM2DCurve* pCurve)
{
	long NumOpen = mp_PolylineInfo->ma_Opening.size();
	if(NumOpen == 0)
		return false;
	for(long iOpen = 0 ; iOpen < NumOpen ; iOpen++)
	{
		MSOpening* pOpen = mp_PolylineInfo->ma_Opening[iOpen];
		GM2DPolyline* pOpenPoly = pOpen->GetProfile();
		if(pOpenPoly->OverlapEx(pCurve) == FALSE)
			continue;

		for(long iCurve = 0 ; iCurve < pOpenPoly->GetNumLines(); iCurve++)
		{
			GM2DCurve* pPolyCurve = pOpenPoly->GetCurve(iCurve);
			GM2DVector CenVec = pPolyCurve->PointAtParam(0.5);
			delete pPolyCurve;
			
			if(pCurve->HasOnLine(CenVec, DTOL_GM))
			{
				long nRVal = GetProfile()->Contains(&CenVec,DTOL_GM);
				if(nRVal == 0)
					return true;
			}
		}
	}
	return false;
}

void MSSlabG::VerifyAndCureBoundaries(double dTol)
{
	long NumOpening = mp_PolylineInfo->ma_Opening.size();
	for(long iOpening = 0 ; iOpening < NumOpening ; iOpening++)
	{
		MSOpening* pOpening = mp_PolylineInfo->ma_Opening[iOpening];
		if(pOpening == NULL) continue;
		GM2DPolyline* pOpenPoly = pOpening->GetProfile();

		pOpenPoly->DeleteDuplicatedPoints(dTol);
		pOpenPoly->MakeClosedPolyline(dTol);
		pOpenPoly->CheckAndFixFoldedEdges(dTol);
		pOpenPoly->CorrectiveBulges();
	}

	GM2DPolyline* pSlabPoly = GetProfile();
	pSlabPoly->DeleteDuplicatedPoints(dTol);
	pSlabPoly->MakeClosedPolyline(dTol);
	pSlabPoly->CheckAndFixFoldedEdges(dTol);
	pSlabPoly->CorrectiveBulges();

	/*
	// 모델링시 입력 받은 프로파일(입력, 인식, 등등)
	GM2DPolyline* pUserInputProfile = mp_PolylineInfo->GetProfile();
	pUserInputProfile->DeleteDuplicatedPoints(dTol);
	pUserInputProfile->MakeClosedPolyline(dTol);
	pUserInputProfile->CheckAndFixFoldedEdges(dTol);
	pUserInputProfile->CorrectiveBulges();
	*/

	if(mp_BotProfile != nullptr)
	{
		mp_BotProfile->DeleteDuplicatedPoints(dTol);
		mp_BotProfile->MakeClosedPolyline(dTol);
		mp_BotProfile->CheckAndFixFoldedEdges(dTol);
		mp_BotProfile->CorrectiveBulges();
	}
	if(mp_InnerProFile != nullptr)
	{
		mp_InnerProFile->DeleteDuplicatedPoints(dTol);
		mp_InnerProFile->MakeClosedPolyline(dTol);
		mp_InnerProFile->CheckAndFixFoldedEdges(dTol);
		mp_InnerProFile->CorrectiveBulges();
	}
	if(mp_InnerProFile_Bot != nullptr)
	{
		mp_InnerProFile_Bot->DeleteDuplicatedPoints(dTol);
		mp_InnerProFile_Bot->MakeClosedPolyline(dTol);
		mp_InnerProFile_Bot->CheckAndFixFoldedEdges(dTol);
		mp_InnerProFile_Bot->CorrectiveBulges();
	}
	if (mp_DeckSlabQTProFile != nullptr)
	{
		mp_DeckSlabQTProFile->DeleteDuplicatedPoints(dTol);
		mp_DeckSlabQTProFile->MakeClosedPolyline(dTol);
		mp_DeckSlabQTProFile->CheckAndFixFoldedEdges(dTol);
		mp_DeckSlabQTProFile->CorrectiveBulges();
	}

	if (mp_LxLyProFile != nullptr)
	{
		mp_LxLyProFile->DeleteDuplicatedPoints(dTol);
		mp_LxLyProFile->MakeClosedPolyline(dTol);
		mp_LxLyProFile->CheckAndFixFoldedEdges(dTol);
		mp_LxLyProFile->CorrectiveBulges();
	}
	MakeCCW();

}

void MSSlabG::FixProfile()
{
	double dTol = DTOL_MODELING;
	long NumOpening = mp_PolylineInfo->ma_Opening.size();
	for (long iOpening = 0; iOpening < NumOpening; iOpening++)
	{
		MSOpening* pOpening = mp_PolylineInfo->ma_Opening[iOpening];
		if (pOpening == NULL) continue;
		GM2DPolyline* pOpenPoly = pOpening->GetProfile();

		pOpenPoly->DeleteInLinePoint(dTol);
		pOpenPoly->MakeClosedPolyline(dTol);
	}

	GM2DPolyline* pSlabPoly = GetProfile();
	pSlabPoly->DeleteInLinePoint(dTol);
	pSlabPoly->MakeClosedPolyline(dTol);
	
	GM2DPolyline* pSlabPoly_Acis = GetTrimmedBoundary_Acis();
	pSlabPoly_Acis->DeleteInLinePoint(dTol);
	pSlabPoly_Acis->MakeClosedPolyline(dTol);
	
	// 모델링시 입력 받은 프로파일(입력, 인식, 등등)
	GM2DPolyline* pUserInputProfile = mp_PolylineInfo->GetProfile();
	pUserInputProfile->DeleteInLinePoint(dTol);
	pUserInputProfile->MakeClosedPolyline(dTol);


	if (mp_BotProfile != nullptr)
	{
		mp_BotProfile->DeleteInLinePoint(dTol);
		mp_BotProfile->MakeClosedPolyline(dTol);
	}
	if (mp_InnerProFile != nullptr)
	{
		mp_InnerProFile->DeleteInLinePoint(dTol);
		mp_InnerProFile->MakeClosedPolyline(dTol);
	}
	if (mp_InnerProFile_Bot != nullptr)
	{
		mp_InnerProFile_Bot->DeleteInLinePoint(dTol);
		mp_InnerProFile_Bot->MakeClosedPolyline(dTol);
	}
	if (mp_DeckSlabQTProFile != nullptr)
	{
		mp_DeckSlabQTProFile->DeleteInLinePoint(dTol);
		mp_DeckSlabQTProFile->MakeClosedPolyline(dTol);
	}

	if (mp_LxLyProFile != nullptr)
	{
		mp_LxLyProFile->DeleteInLinePoint(dTol);
		mp_LxLyProFile->MakeClosedPolyline(dTol);
	}
}

void MSSlabG::RecoverPointerAfterSerialize(MSFloor* floor)
{
	MSCompoG::RecoverPointerAfterSerialize(floor);
	/*
	MSFloor* pFloor = GetFloor();
	long NumSlabLevelZone = ma_LevelPoly.size();

	for(long iSlabLevel = NumSlabLevelZone - 1 ; iSlabLevel >= 0 ; iSlabLevel--)
	{
		MSSlabLevelZone* pSlabLevelZone = ma_LevelPoly[iSlabLevel];
		if(pSlabLevelZone == NULL) continue;
		MSLevelZone* pLevelZone = pFloor->GetLevelZone(pSlabLevelZone->m_LevelZoneID);
		if(pLevelZone != NULL)
			pSlabLevelZone->mp_LevelZone = pLevelZone;
		else
		{
			delete pSlabLevelZone;
			ma_LevelPoly.erase(ma_LevelPoly.begin() + iSlabLevel);
		}
	}
	*/

	MSSlabG* slab = this;
	vector<MSCompoG*> sub_list;
	for (int i = 0; i<slab->ma_Haunches .size(); ++i)
		sub_list.push_back((MSCompoG*)slab->ma_Haunches [i]);
	for (int i = 0; i<sub_list.size(); ++i)
	{
		for (int k = sub_list[i]->ma_Connections.size() - 1; k >= 0; k--)
		{
			if (sub_list[i]->ma_Connections[k]->RecoverPointerAfterSerialize_ForSubMembers(sub_list) == false)
			{
				delete sub_list[i]->ma_Connections[k];
				sub_list[i]->ma_Connections.erase(sub_list[i]->ma_Connections.begin() + k);
			}
		}

// 		for (int k = 0; k<sub_list[i]->ma_Connections.size(); ++k)
// 			sub_list[i]->ma_Connections[k]->RecoverPointerAfterSerialize_ForSubMembers(sub_list);
	}

	sub_list.clear();
	for (int i = 0; i<slab->ma_EdgeWall.size(); ++i)
		sub_list.push_back((MSCompoG*)slab->ma_EdgeWall[i]);
	for (int i = 0; i<sub_list.size(); ++i)
	{
		for (int k = sub_list[i]->ma_Connections.size() - 1; k >= 0; k--)
		{
			if (sub_list[i]->ma_Connections[k]->RecoverPointerAfterSerialize_ForSubMembers(sub_list) == false)
			{
				delete sub_list[i]->ma_Connections[k];
				sub_list[i]->ma_Connections.erase(sub_list[i]->ma_Connections.begin() + k);
			}
		}
// 		for (int k = 0; k<sub_list[i]->ma_Connections.size(); ++k)
// 			sub_list[i]->ma_Connections[k]->RecoverPointerAfterSerialize_ForSubMembers(sub_list);
	}

	//BH-1729
	for (int i = 0; i < ma_Haunches .size(); i++)
		ma_Haunches [i]->CheckValidConnections();
}

void MSSlabG::RecoverPointerAfterSerialize_SubMembers()
{
	MSSlabG* slab = (MSSlabG*)this;

	vector<MSCompoG*> sub_list;
	for (int i = 0; i<slab->ma_Haunches .size(); ++i)
		sub_list.push_back((MSCompoG*)slab->ma_Haunches [i]);
	for (int i = 0; i<sub_list.size(); ++i)
	{
		for (int k = sub_list[i]->ma_Connections.size() - 1; k >= 0; k--)
		{
			if (sub_list[i]->ma_Connections[k]->RecoverPointerAfterSerialize_ForSubMembers(sub_list) == false)
			{
				delete sub_list[i]->ma_Connections[k];
				sub_list[i]->ma_Connections.erase(sub_list[i]->ma_Connections.begin() + k);
			}
		}

// 		for (int k = 0; k<sub_list[i]->ma_Connections.size(); ++k)
// 			sub_list[i]->ma_Connections[k]->RecoverPointerAfterSerialize_ForSubMembers(sub_list);
	}

	//BH-1729
	for (int i = 0; i < ma_Haunches .size(); i++)
		ma_Haunches [i]->CheckValidConnections();
}

void MSSlabG::SetLxDeg(double deg)
{
	double cur_deg = deg;
	if (cur_deg >= 360.0) cur_deg -= 360.0;
	if (cur_deg < 0.0) cur_deg += 360.0;
	if (cur_deg >= 180.0) cur_deg -= 180.0;

	cur_deg = RoundOff5(cur_deg);
	if (cur_deg >= 180.0) cur_deg = 0.0;

	m_dLxDeg = cur_deg;
}

void MSSlabG::UpdateLxDegWithRibDir(bool isXDir_Old, bool isXDir_New)
{
	if (isXDir_Old == isXDir_New)
		return;

	if (IsPCSlabGroup())
	{
		if (isXDir_Old == true)
			m_dLxDeg -= 90;
		else
			m_dLxDeg += 90;
	}
}

void MSSlabG::DeleteSlabRebar()
{
	long nRebar = ma_Rebar.size();
	for(long iRebar =0; iRebar<nRebar; iRebar++)
	{
		MSSlabRebar* pRebar = ma_Rebar[iRebar];
		delete pRebar;
	}
	ma_Rebar.clear();
}

void MSSlabG::ModifyObject(bool isDoit)
{
	MSCompoG::ModifyObject(isDoit);
	return;
}

void MSSlabG::UpdateSubMember()
{
	TrimLevelDifbySlab();
	TrimHaunchbySlab();

	TrimWellsbySlab();
	MergeWell();

	TrimTrenchsbySlab();
	MergeTrench();
	TrimTrenchbyWellAll();
}

map<long, MSLevelZone*> MSSlabG::FindLevelZonebyDifG()
{
	map<long, MSLevelZone*> LevelZoneMap;
	map<long, MSLevelZone*>::iterator LevelZoneIt; 

	long nLevelDif = ma_LevelDif.size();
	for(long iLevelDif= nLevelDif - 1 ; iLevelDif>= 0; iLevelDif--)
	{
		MSLineLevelDifG* pDif = ma_LevelDif[iLevelDif]->mp_LineLevelDifG;
		MSLevelZone* pLevelZone = NULL;
		if(pDif->mp_LeftLevelZone != NULL && (pDif->mp_LeftLevelZone == pDif->mp_RightLevelZone) )
			pLevelZone = pDif->mp_LeftLevelZone;
		else if(pDif->mp_LeftLevelZone != NULL)
			pLevelZone = pDif->mp_LeftLevelZone;
		else if(pDif->mp_RightLevelZone != NULL)
			pLevelZone = pDif->mp_LeftLevelZone;

		if(pLevelZone != NULL)
		{
			LevelZoneIt = LevelZoneMap.find(pLevelZone->m_ID);
			if(LevelZoneIt == LevelZoneMap.end())
				LevelZoneMap.insert(make_pair(pLevelZone->m_ID, pLevelZone));
		}
	}

// 	nLevelDif = ma_LevelDifOnEdge.size();
// 	for(long iLevelDif= nLevelDif - 1 ; iLevelDif>= 0; iLevelDif--)
// 	{
// 		MSLineLevelDifG* pDif = ma_LevelDifOnEdge[iLevelDif];
// 		MSLevelZone* pLevelZone = NULL;
// 		if(pDif->mp_LeftLevelZone != NULL && (pDif->mp_LeftLevelZone == pDif->mp_RightLevelZone) )
// 			pLevelZone = pDif->mp_LeftLevelZone;
// 		else if(pDif->mp_LeftLevelZone != NULL)
// 			pLevelZone = pDif->mp_LeftLevelZone;
// 		else if(pDif->mp_RightLevelZone != NULL)
// 			pLevelZone = pDif->mp_LeftLevelZone;
// 
// 		if(pLevelZone != NULL)
// 		{
// 			LevelZoneIt = LevelZoneMap.find(pLevelZone->m_ID);
// 			if(LevelZoneIt == LevelZoneMap.end())
// 				LevelZoneMap.insert(make_pair(pLevelZone->m_ID, pLevelZone));
// 		}
// 	}
	
	return LevelZoneMap;
}

void MSSlabG::TrimEdgeWallLevelDif()
{
	long nLevelDif = ma_EdgeWall.size();
	for (long iLevelDif = 0; iLevelDif < nLevelDif; iLevelDif++)
	{
		MSLineLevelDifG* pMyLevelDif = ma_EdgeWall[iLevelDif];
		pMyLevelDif->GetID();
		pMyLevelDif->RemoveConnections();
	}

	for (long iLevelDif = 0; iLevelDif < nLevelDif; iLevelDif++)
	{
		MSLineLevelDifG* pMyLevelDif = ma_EdgeWall[iLevelDif];
		vector<MSCompoG*> OverLapCompoGArr;

		GM2DPolyline MyPoly(TRUE);
		pMyLevelDif->GetBoundary(MyPoly);

		for(long jLevelDif = 0 ; jLevelDif<nLevelDif; jLevelDif++)
		{
			MSLineLevelDifG* pUrLevelDif = ma_EdgeWall[jLevelDif];
			if(pMyLevelDif == pUrLevelDif)
				continue;

			CheckModelOpt tmpOpt;
			if(pMyLevelDif->IsLevelConnectedOverLap(pUrLevelDif, tmpOpt) == false)
				continue;

			pUrLevelDif->GetID();
			GM2DPolyline UrPoly(TRUE);
			pUrLevelDif->GetBoundary(UrPoly);

			if(MyPoly.OverlapEx(UrPoly, MSFloor::ms_DefLineHitDistTol))
				OverLapCompoGArr.push_back(pUrLevelDif);
		}

		// 입력 순서대로 정렬
		sort(OverLapCompoGArr.begin(), OverLapCompoGArr.end(), 
			[](const MSCompoG* a, const MSCompoG* b)->bool {if (a->m_nCmdCnt == b->m_nCmdCnt) return a->m_ID < b->m_ID; else return a->m_nCmdCnt < b->m_nCmdCnt; }
		);

		for (long i = 0 ; i < OverLapCompoGArr.size() ; i++)
			pMyLevelDif->AddConnectionWithMember(OverLapCompoGArr[i]);


		pMyLevelDif->UpdateTrimmedBoundary();
	}

	MSFloor* pFloor = GetFloor();

	for(long iLevelDif=0; iLevelDif<nLevelDif; iLevelDif++)
	{
		MSLineLevelDifG* pMyLevelDif = ma_EdgeWall[iLevelDif];
		pMyLevelDif->UpdateBoundaryVisibilities();					
		pMyLevelDif->SetTrimmedLines();
		pFloor->CalculateUpdatedCompoGArray(pMyLevelDif, pFloor->CompoGIDList_NeedRegen_CurTR());
	}
}

void MSSlabG::TrimLevelDifAll()
{
	TrimEdgeWallbyOpening();

	map<long, MSLevelZone*> LevelZoneMap = FindLevelZonebyDifG();
	map<long, MSLevelZone*>::iterator LevelZoneIt; 

	if(LevelZoneMap.size() == 0)
		TrimEdgeWallLevelDif();
	else
	{
		for(LevelZoneIt = LevelZoneMap.begin() ; LevelZoneIt != LevelZoneMap.end() ; LevelZoneIt++)
		{
			MSLevelZone* pLevelZone = LevelZoneIt->second;
			pLevelZone->TrimLevelDifbySlabG();
		}
	}
	
}

void MSSlabG::TrimLevelDifAllByLevelZone(MSSlabG* pSlab, map<long, MSSlabG*>& SlabMap)
{
	pSlab->TrimEdgeWallbyOpening();

	map<long, MSLevelZone*> LevelZoneMap = pSlab->FindLevelZonebyDifG();
	map<long, MSLevelZone*>::iterator LevelZoneIt;

	if (LevelZoneMap.size() == 0)
		pSlab->TrimEdgeWallLevelDif();
	else
	{
		for (LevelZoneIt = LevelZoneMap.begin(); LevelZoneIt != LevelZoneMap.end(); LevelZoneIt++)
		{
			MSLevelZone* pLevelZone = LevelZoneIt->second;
			pLevelZone->TrimLevelDifbySlabGSetp2(SlabMap);
		}
	}
}

double MSSlabG::GetSlabLevel(GM2DVector TheVec)
{
	MSFloor* pFloor = GetFloor();
	double dZoneLevel = pFloor->GetLevel4LevelZone(TheVec.m_X, TheVec.m_Y);
	
	long nRval = Contains(&TheVec);
	double dFloorLevel = GetNextSlabLevel();
	if(nRval >= 1)
		return dZoneLevel + dFloorLevel + m_dOffset;
	else
		return dZoneLevel + dFloorLevel + m_dOffset;
}

bool MSSlabG::Is2WaySlab()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
	{
		AfxThrowUserException();
		return false;
	}
	long slabMType = mySlabM->GetSlabMType();

	if(mySlabM->GetSlabMType()!=2)//GENERAL_TYPE가 아니면 
		return true; // mylee

	GM2DPolyline inPolySlab = GetInnerProfile();
	if (inPolySlab.ma_Point.GetSize() == 0) return false; // mylee - 예외처리
	GM2DPolyline outRect = inPolySlab.Get2DBoundingBox(m_dLxDeg);
	double dLxLength = outRect.ma_Point.GetAt(0)->Distance(*outRect.ma_Point.GetAt(1));
	double dLyLength = outRect.ma_Point.GetAt(0)->Distance(*outRect.ma_Point.GetAt(3));
	
	outRect.ResetPolyline();

	if(max(dLxLength,dLyLength) / min(dLxLength,dLyLength) >= 2)//1방향슬래브
		return false;

	return true;

}


//////////////////////////////////////////////////////////////
void _LevelDifInfoStruct::Serialize(CArchive &ar)
{
	if(ar.IsStoring())
	{
		ar << m_dWallWidth;
		ar << m_bHasHaunch;
		ar << m_bHasWall;
		ar << m_dHaunchX;
		ar << m_dHaunchY;
		ar << m_dHaunchAngle;
		ar << m_bUseHaunchMaterial;
		ar << m_nHaunchMaterialID;
		ar << m_bUseBackFillFormwork;
		ar << m_IsUsedStoryMaterial;
	}
	else
	{
		ar >> m_dWallWidth;
		ar >> m_bHasHaunch;
		ar >> m_bHasWall;
		if(MSVersionInfo::GetCurrentVersion() >= 20140630)
		{
			ar >> m_dHaunchX;
			ar >> m_dHaunchY;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20150613)
		{
			ar >> m_dHaunchAngle;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20150630)
		{
			ar >> m_bUseHaunchMaterial;
			ar >> m_nHaunchMaterialID;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20160309)
		{
			ar >> m_bUseBackFillFormwork;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20160310)
		{
			ar >> m_IsUsedStoryMaterial;
		}
	}
}

long _LevelDifInfoStruct::GetHaunchMaterialID(MSFloor* pFloor)
{
	if (m_IsUsedStoryMaterial)
	{
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

void MSSlabG::UpdateInnerProfile(bool isTop)
{
	// 마감에서 계산 시간이 오래걸릴수 있다. 
	// 성능 문제시 AppGlobalOptionWrapper::Instance()->GetProcessMode() 확인후 처리해도 괜찬을듯
	//if (AppGlobalOptionWrapper::Instance()->GetAppMode() == AppGlobalOptionWrapper::Finish)
		//return;

	GM2DPolyline* pInnerProfile = nullptr;
	if (isTop)
		pInnerProfile = mp_InnerProFile;
	else
		pInnerProfile = mp_InnerProFile_Bot;

	long SlabMType = GetSlabMType();
	if (IsPCSlabGroup() || IsDeckSlab())
	{
		// UpdateInnerPCSlabProfile()는 직사각형 슬래브 대상으로 로직이 개발됨
		bool isUpdateInner = UpdateInnerPCSlabProfile(pInnerProfile);
		bool isUpdateLxLy = UpdateInnerPCSlabProfile(mp_LxLyProFile, true);
		// UpdateInnerPCSlabProfile 로직에서 계산이 안되는 경우 이전에 개발한 일반적인 로직을 사용하도록 수정
		if (isUpdateInner && isUpdateLxLy)
		{
			VerifyAndCureBoundaries(DTOL_MODELING);
			pInnerProfile->CopyFrom(GetProfile());
			return;
		}
	}
	
	// 일반적인 로직
	double dSlabTopLevel = 0;
	double dSlabBotLevel = 0;
	GetZLevels(dSlabTopLevel, dSlabBotLevel);

	pInnerProfile->ResetPolyline();
	GM2DPolyline outerProfile(TRUE);

	bool isDebugPrint = false;
	double dTol = DTOL_MODELING;

	//outerProfile.CopyFrom(this->GetProfile());
	// 슬래브에 점선이 존재하는 경우 입력된 형상으로 계산한다.
	// 라인부재를 어느 폴리라인 기준으로 찾아야 하는것인지??
	bool HasHidden = HasHiddenBoundaryCurve();
	if(HasHidden == true)
		outerProfile.CopyFrom(GetProfile());
	else
		outerProfile.CopyFrom(GetTrimmedBoundary(isTop));
	outerProfile.DeleteInLinePoint(DTOL_MODELING);
	outerProfile.MakeClosedPolyline(DTOL_MODELING);

	CString msg;
	outcome out;

	try
	{
		if (outerProfile.ma_Point.GetCount()>2)
		{
			bool IsAPIFailed = false;
			API_NOP_BEGIN

				/*if (isDebugPrint)
			{
			CString msg;
			double dArea = AcisGeomHelper::GetArea(GivenPolygon);
			msg.Format(L"Slab %d profiling Area, %f", m_ID, dArea);
			StatusReporterWrapper::OutputMessage(msg);
			}*/

			double dMaxCompoGWidth = 0;
			long SlabMType = GetSlabMType();
			if (!IsAPIFailed && (SlabMType == MSRcSlabM::SLAB_TYPE::GENERAL_TYPE ||
				SlabMType == MSRcSlabM::SLAB_TYPE::PC_TYPE ||
				SlabMType == MSRcSlabM::SLAB_TYPE::PC_IMT ||
				SlabMType == MSRcSlabM::SLAB_TYPE::DECK_TYPE ||
				SlabMType == MSRcSlabM::SLAB_TYPE::DECK_GENERAL ||
				SlabMType == -1) &&//사변지지||PC||Deck||DECK_GENERAL 일경우만 내부폴리곤 계산. -1인 경우는 M이 할당되지 않은 단순 모양 모델링 경우
	
				NeedToUpdateInnerProFile() == true)
			{
				BODY* GivenPolygon = NULL;
				if (!AcisGeomHelper::MakeFaceFromPolyline(&outerProfile, GivenPolygon))
					IsAPIFailed = true;

				ENTITY_LIST subFaces;

				vector<MSCompoG*> compoGs = FindConnectMember(FindType_MSElement::msNULL, FIND_ALLFLOOR, dTol, true, true, isTop);
				
				for (int i = 0; i < compoGs.size(); i++)
				{
					MSCompoG* compoG = compoGs[i];
					if (compoG->GetType() != Type_MSCompoG::msBeamG && compoG->IsSteel())
						continue;

					double dTopLevel = 0;
					double dBotLevel = 0;
					compoG->GetZLevels(dTopLevel, dBotLevel);
					if (dSlabBotLevel > dTopLevel || dSlabTopLevel < dBotLevel)
						continue;
					
					// 상부에 터치한 객체는 처리하지 않는다.
					if (fabs(dSlabTopLevel - dBotLevel) < DTOL_GM)
						continue;

					// 폴리라인과 평행하지 않은 라인부재는 InnerProfile에 포함하지 않는다.
					if (compoG->IsLinkMember())
					{
						double dDegTol = 5;
						MSLinkMemberG* pLinkG = (MSLinkMemberG*)compoG;
						shared_ptr<GM2DCurve> pCurve = pLinkG->GetCenCurve();
						if (outerProfile.HasColinearCurve(&*pCurve, dDegTol, pLinkG->GetWidth() / 2 + DTOL_MODELING * 3) == false)
							continue;
					}
					else if (compoG->IsPointMember())
					{
						GM2DPolyline* pPoly = compoG->GetTrimmedBoundary();
						vector<GM2DPolyline*> joined_Polys;
						outerProfile.BoolOperation('A', pPoly, joined_Polys, dTol);
						int PolyCnt = joined_Polys.size();
						GMObject::DeleteGMObject(joined_Polys);
						if (PolyCnt > 1)
							continue;
					}

					if (compoG->GetType() == msBeamG || compoG->GetType() == msWallG || compoG->GetType() == msColumnG || compoG->GetType() == msWallColumnG)
					{
						vector<GM2DPolyline*> JoinedPolys;
						outerProfile.BoolOperation('A', compoG->GetTrimmedBoundary(), JoinedPolys, dTol);

						int polyCount = JoinedPolys.size();
						GMObject::DeleteGMObject(JoinedPolys);

						if (polyCount > 1)
							continue;

						BODY* subFace = NULL;
						if (!AcisGeomHelper::MakeFaceFromPolyline(compoG->GetTrimmedBoundary(), subFace))
						{

							msg.Format(L"Slab %d Error in making support face from %d", m_ID, compoG->m_ID);
							StatusReporterWrapper::OutputError(msg);
						}

						if (subFace == NULL || (FACE*)subFace->lump()->shell()->face() == NULL)
							continue;

						dMaxCompoGWidth = max(dMaxCompoGWidth, compoG->GetWidth());

						//BODY* outbody = NULL;
						//out = api_fafa_int((FACE*)GivenPolygon->lump()->shell()->face(), (FACE*)subFace->lump()->shell()->face(), outbody);
						//if (AcisGeomHelper::CheckAcisError(result, msg, true))
						//{
						//	if (outbody == NULL || outbody->lump() == NULL || outbody->lump()->shell() == NULL || outbody->lump()->shell()->wire() == NULL)
						//	{//no intersection => offset(expand) // boundary에 아주 가깝게 붙어있을 경우 살짝 키워서 사용하기 위함
						//		double offset_value = 0.1;
						//		out = api_offset_planar_face_loops((FACE*)subFace->lump()->shell()->face(), offset_value, outbody, sg_gap_type::natural);
						//		if (AcisGeomHelper::CheckAcisError(out, msg, true) && outbody != NULL)
						//		{
						//			subFace = outbody;
						//		}
						//	}
						//}  

						/*
						entity_clash_type clash_result;
						//api_fafa_int에 비해서 결코 빠르지 않음.
						result = api_clash_faces((FACE*)GivenPolygon->lump()->shell()->face(), (FACE*)subFace->lump()->shell()->face(), clash_result);
						if (clash_result == ENTITY_CLASH_NONE ||
							clash_result == ENTITY_CLASH_BINA )
						{
							//no clash => offset(expand) // boundary에 아주 가깝게 붙어있을 경우 살짝 키워서 사용하기 위함
							double offset_value = 0.01;
							BODY* outbody = NULL;
							out = api_offset_planar_face_loops((FACE*)subFace->lump()->shell()->face(), offset_value, outbody, sg_gap_type::natural);
							if (AcisGeomHelper::CheckAcisError(out, msg, true) && outbody != NULL)
							{
								subFace = outbody;
							}
						}*/

						subFaces.add(subFace);
					}

				}
				ENTITY* subFaces_iter;

				/*if (isDebugPrint)
				{
				subFaces.init();
				CStringA msg;
				msg.Format("E:\\ID_%d.sat", m_ID);

				AcisGeomHelper::SaveEntities(msg.operator LPCSTR(), subFaces, true);

				ENTITY_LIST mainshapelist;
				mainshapelist.add(GivenPolygon);
				msg.Format("E:\\ID_%d_Main.sat", m_ID);

				AcisGeomHelper::SaveEntities(msg.operator LPCSTR(), mainshapelist, true);

				}*/

				subFaces.init();
				while ((subFaces_iter = subFaces.next()) != NULL)
				{
					/*double dFaceArea =0;
					dFaceArea = AcisGeomHelper::GetArea((BODY*)subFaces_iter);
					if(dFaceArea <0.01) continue;					*/
					BODY* bodyToSubtract = (BODY*)subFaces_iter;

					API_TRIAL_BEGIN
						result = api_subtract(bodyToSubtract, GivenPolygon);
					API_TRIAL_END

						CString msg;
					msg.Format(L" while Boolean subtract %d ", m_ID);
					if (!AcisGeomHelper::CheckAcisError(result, msg, false))
					{
						//Subtract실패시, 

						//try remove-short-edges
						{
							ENTITY_LIST returned_list;
							API_TRIAL_BEGIN
								result = api_detect_short_edges(bodyToSubtract, returned_list, SPAresfit*0.1, true);
							API_TRIAL_END
								msg.Format(L" while detecting short edges %d ", m_ID);
							AcisGeomHelper::CheckAcisError(result, msg);

						}

						//sheet-thicken
						SPAposition box_h(0, 0, 0);
						SPAposition box_l(0, 0, 0);
						API_TRIAL_BEGIN
							result = api_sheet_thicken(bodyToSubtract, 1.0, true, box_l, box_h);
						API_TRIAL_END

							msg.Format(L" while sheet thicken %d ", m_ID);
						if (!AcisGeomHelper::CheckAcisError(result, msg))
						{
							if (isDebugPrint)
							{
								CStringA file;
								file.Format("E:\\ID_%d_ThickenFail%d.sat", m_ID);

								ENTITY_LIST resultshape;
								resultshape.add(bodyToSubtract);
								AcisGeomHelper::SaveEntities(file.operator LPCSTR(), resultshape, true);
							}
							IsAPIFailed = true;
						}
						else//try again 
						{
							API_TRIAL_BEGIN
								result = api_subtract(bodyToSubtract, GivenPolygon);//, &boolOpt);
							API_TRIAL_END
								msg.Format(L"while Boolean Subtract %d ", m_ID);
							if (!AcisGeomHelper::CheckAcisError(result, msg))
							{
								IsAPIFailed = true;
#ifndef DEBUG
								/*	ENTITY_LIST Bodies;
								Bodies.add(bodyToSubtract);
								Bodies.add(GivenPolygon);
								CStringA msg;

								msg.Format("E:\\InnerProfile_%d.sat", m_ID);

								AcisGeomHelper::SaveEntities(msg.operator LPCSTR(), Bodies, true);*/

#endif
							}
							else
							{
								IsAPIFailed = false;

								/*	CString msg;
								msg.Format(L"retry ID_%d.. success! ", m_ID);
								StatusReporterWrapper::OutputMessage(msg);*/
								//IsAPIFailed = false;
							}
						}
					}
					// body가 2개이상으로 분리되었을 경우, 가장 큰것만 남긴다. 
					// 애초에 가장자리에 걸린 하부 부재만을 가져오지만, 비뚤어진 하부부재가 있을경우, 자투리 형상이 생겨날 수 있다. 
					AcisGeomHelper::LeaveLargestFaceOnly(GivenPolygon);
				}

				if (!IsAPIFailed)
				{
					api_clean_entity(GivenPolygon);
					if (isDebugPrint)
					{
						CStringA file;
						file.Format("E:\\ID_%d_Result.sat", m_ID);

						ENTITY_LIST resultshape;
						resultshape.add(GivenPolygon);
						AcisGeomHelper::SaveEntities(file.operator LPCSTR(), resultshape, true);
					}

					// inner Profile로의 결과 변환
					AcisGeomHelper::ConvertBody(GivenPolygon, *pInnerProfile, false);
				}

// 				if (SlabMType == MSRcSlabM::SLAB_TYPE::PC_IMT)
// 				{
// 					UpdateInnerIMTSlabProfile(pInnerProfile, outerProfile, dMaxCompoGWidth);
// 				}
			}
			else 
				pInnerProfile->CopyFrom(&outerProfile);

			API_NOP_END

				if (IsAPIFailed || pInnerProfile == NULL || pInnerProfile->GetNumLines() < 2)
					pInnerProfile->CopyFrom(&outerProfile);
		}
	}
	catch (const std::exception& ex)
	{
		CString errorMsg;
		errorMsg.Format(L"Standard exception occurred in Slab %d while getting inner profile: %S", m_ID, ex.what());
		StatusReporterWrapper::OutputError(errorMsg);

		pInnerProfile->CopyFrom(&outerProfile);
	}
#ifndef _DEBUG
	catch (...)
	{
		CString msg;
		msg.Format(L"Unexpected exception occurred in Slab %d while getting inner profile", m_ID);
		StatusReporterWrapper::OutputMessage(msg);

		pInnerProfile->CopyFrom(&outerProfile);
	}
#endif

	pInnerProfile->MakeClosedPolyline(DTOL5);
	VerifyAndCureBoundaries(DTOL_MODELING);
}

bool MSSlabG::UpdateInnerPCSlabProfile(GM2DPolyline* pInnerProfile, bool isCalcLxLyProfile)
{
	bool bRval = true;
	if (NeedToUpdateInnerProFile() == false)
		return bRval;

	bool isTop = true;
	double dSlabTopLevel = 0;
	double dSlabBotLevel = 0;
	GetZLevels(dSlabTopLevel, dSlabBotLevel);

	pInnerProfile->ResetPolyline();
	GM2DPolyline outerProfile(TRUE);

	bool isDebugPrint = false;
	double dTol = 0.1;
	double dDegTol = 5;
	//outerProfile.CopyFrom(this->GetProfile());
	// 슬래브에 점선이 존재하는 경우 입력된 형상으로 계산한다.
	// 라인부재를 어느 폴리라인 기준으로 찾아야 하는것인지??
	bool HasHidden = HasHiddenBoundaryCurve();
	if (HasHidden == true)
		outerProfile.CopyFrom(GetProfile());
	else
		outerProfile.CopyFrom(GetTrimmedBoundary(isTop));
	outerProfile.DeleteInLinePoint(DTOL_MODELING);
	outerProfile.MakeClosedPolyline(DTOL_MODELING);

	GM2DBound boundBox;
	outerProfile.Get2DBound(boundBox);
	double maxLen = max(boundBox.GetWidth(), boundBox.GetHeight());

	outerProfile.SetStartIdxOnBottomLeft();

	vector<MSCompoG*> compoGs = FindConnectMember(FindType_MSElement::msNULL, FIND_ALLFLOOR, dTol, true, true, isTop);
	vector<MSCompoG*> targetLineGArr;
	//vector<MSCompoG*> targetPointGArr;
	// PC 슬래브의 이너프로파일은 라인부재만 처리하도록 변경
	for (int i = 0; i < compoGs.size(); i++)
	{
		MSCompoG* compoG = compoGs[i];
		if (compoG->GetType() != Type_MSCompoG::msBeamG && compoG->IsSteel())
			continue;

		double dTopLevel = 0;
		double dBotLevel = 0;

		compoG->GetZLevels(dTopLevel, dBotLevel);
		if (dSlabBotLevel > dTopLevel || dSlabTopLevel < dBotLevel)
			continue;
		
		// 상부에 터치한 객체는 처리하지 않는다.
		if (fabs(dSlabTopLevel - dBotLevel) < DTOL_GM)
			continue;

		// 폴리라인과 평행하지 않은 라인부재는 InnerProfile에 포함하지 않는다.
		if (compoG->IsLinkMember())
		{
			MSLinkMemberG* pLinkG = (MSLinkMemberG*)compoG;
			shared_ptr<GM2DCurve> pCurve = pLinkG->GetCenCurve();
			if (outerProfile.HasColinearCurve(&*pCurve, dDegTol, pLinkG->GetWidth() / 2 + DTOL_MODELING * 3) == false)
				continue;
		}
		else if (compoG->IsPointMember())
		{
			GM2DPolyline* pPoly = compoG->GetTrimmedBoundary();
			vector<GM2DPolyline*> joined_Polys;
			outerProfile.BoolOperation('A', pPoly, joined_Polys, dTol);
			int PolyCnt = joined_Polys.size();
			GMObject::DeleteGMObject(joined_Polys);
			if (PolyCnt > 1)
				continue;
		}

		if (isCalcLxLyProfile == true)
		{
			if (compoG->GetType() == msBeamG)
				targetLineGArr.push_back(compoG);
		}
		else
		{
			if (compoG->GetType() == msBeamG || compoG->GetType() == msWallG)
				targetLineGArr.push_back(compoG);
		}
// 		else if(compoG->GetType() == msColumnG || compoG->GetType() == msWallColumnG)
// 			targetPointGArr.push_back(compoG);
	}

	vector<GM2DCurve*> profileCurveList;
	vector<GM2DCurve*> outerCurveArr;
	outerProfile.GetSegments(outerCurveArr);

	//targetCompoGArr 찾는 조건에따라 정렬하는 로직 필요함

	for (int iCurve = 0; iCurve < outerCurveArr.size(); iCurve++)
	{
		GM2DCurve* pCurCurve = outerCurveArr[iCurve];
		pCurCurve->ExtendLineVector(true, -50, false);
		pCurCurve->ExtendLineVector(false, -50, true);

		GM2DVector midVec = pCurCurve->PointAtParam(0.5);

		GM2DCurve* pColinearCurver = nullptr;

		double dMaxCompoGWidth = 0;
		vector<std::tuple<double, GM2DCurve*>> widthCurveTupleArr;
		vector<GM2DCurve*> targetCompoGCurveArr;
		vector <MSLinkMemberG*> linkGArr;
		vector <MSCompoG*> pointGArr;
		bool hasBeam = false;
		for (int iG = 0; iG < targetLineGArr.size(); iG++)
		{
			MSCompoG* pCompoG = targetLineGArr[iG];
			MSLinkMemberG* pLinkG = dynamic_cast<MSLinkMemberG*>(pCompoG);
			if (pLinkG == nullptr) continue;

			long nRval = pCurCurve->IsColinear4DTol(&*pLinkG->GetCenCurve(), dDegTol, pLinkG->GetWidth() + DTOL_MODELING);
			if (nRval == 0)
				continue;

			if (pLinkG->GetProfile()->OverlapEx(pCurCurve) == true)
			{
				linkGArr.push_back(pLinkG);
				if (pLinkG->GetType() == MSElement::msBeamG)
					hasBeam = true;
			}
		}

		// 슬래브와 겹쳐진 라인부재가 있는 경우 슬래브 라인과 겹쳐지는 포인트 부재를 검토 하지 않는다.
// 		if (linkGArr.size() > 0)
// 		{
// 			for (int iG = 0; iG < targetPointGArr.size(); iG++)
// 			{
// 				MSCompoG* pCompoG = targetPointGArr[iG];
// 				if (pCompoG->GetProfile()->OverlapEx(pCurCurve) == true)
// 					pointGArr.push_back(pCompoG);
// 			}
// 		}

		if (linkGArr.size() > 0)
		{
			for (int iLinkG = 0; iLinkG < linkGArr.size(); iLinkG++)
			{
				MSLinkMemberG* pLinkG = linkGArr[iLinkG];
				
				// 보, 벽이 동시에 존재하는 경우 보를 우선적으로 처리한다
				// 보를 찾은 경우 벽을 찾지 않는다
				if (hasBeam == true && pLinkG->GetType() == MSElement::msWallG)
					continue;
				
				double dWidth = pLinkG->GetWidth();
				dMaxCompoGWidth = max(dMaxCompoGWidth, dWidth);

				if (isCalcLxLyProfile == true)
					widthCurveTupleArr.push_back(std::make_tuple(dWidth, pLinkG->GetCenCurve()->Copy()));
				else
				{
					if (pLinkG->mp_LLine != nullptr)
						widthCurveTupleArr.push_back(std::make_tuple(dWidth, pLinkG->mp_LLine->Copy()));
					if (pLinkG->mp_RLine != nullptr)
						widthCurveTupleArr.push_back(std::make_tuple(dWidth, pLinkG->mp_RLine->Copy()));
				}
			}
		}
		else
		{
			for (int iPointG = 0; iPointG < pointGArr.size(); iPointG++)
			{
				MSCompoG* pCompoG = pointGArr[iPointG];
				vector<GM2DCurve*> targetCurveArr;
				MSPointMemberG* pPointG = (MSPointMemberG*)pCompoG;
				if (pPointG != nullptr)
				{
					GM2DPolyline TheGon(TRUE);
					pPointG->GetSecPolylineByInsertVec(TheGon);
					TheGon.GetSegments(targetCurveArr);
				}
				else
					pCompoG->GetTrimmedBoundary()->GetSegments(targetCurveArr);

				double dWidth = max(pCompoG->GetWidth(), pCompoG->GetDepth());
				dMaxCompoGWidth = max(dMaxCompoGWidth, dWidth);

				for (int idx = 0 ; idx < targetCurveArr.size() ; idx++)
					widthCurveTupleArr.push_back(std::make_tuple(dWidth, targetCurveArr[idx]));
			}
		}


		double dMinDist = dMaxCompoGWidth * 2;

		for (int iTargetCurve = 0; iTargetCurve < widthCurveTupleArr.size(); iTargetCurve++)
		{
			double dWidth = std::get<0>(widthCurveTupleArr[iTargetCurve]);
			GM2DCurve* pTargetCurve = std::get<1>(widthCurveTupleArr[iTargetCurve]);
			if(pTargetCurve->Length() < 10)
				continue;

			long nRval = pCurCurve->IsColinear4DTol(pTargetCurve, DTOL_MODELING, dMaxCompoGWidth + DTOL_MODELING);
			if (nRval != 0)
			{
				if (isCalcLxLyProfile == true)
				{
					// 보의 Width가 가장 작은 선을 사용 한다.
					if (dMinDist > dWidth)
					{
						dMinDist = dWidth;
						pColinearCurver = pTargetCurve;
					}
				}
				else
				{
					double dist = pTargetCurve->Distance(midVec);
					GM2DVector targetMidVec = pTargetCurve->PointAtParam(0.5);
					// 슬래브 프로파일 선의 왼쪽에 있는 선만 사용한다.
					long nSide = pCurCurve->WhichSide(targetMidVec);

					if (dMinDist > dist && dist > 5 && nSide == 1)
					{
						if (nRval == -1)
							pTargetCurve->ReverseDirection();
						pColinearCurver = pTargetCurve;
						dMinDist = dist;
					}
				}
			}
		}

		if (pColinearCurver == nullptr)
			pColinearCurver = pCurCurve->Copy();
		else
			pColinearCurver = pColinearCurver->Copy();
			
		if (outerProfile.IsRectangle())
		{
			pColinearCurver->ExtendLineVector(true, maxLen, false);
			pColinearCurver->ExtendLineVector(false, maxLen, true);
			outerProfile.GetCurveInPolygon(pColinearCurver);
		}

		double curveLen = pColinearCurver->Length();
		if (curveLen > DTOL_MODELING)
			profileCurveList.push_back(pColinearCurver);
		else
			delete pColinearCurver;
		

		GMObject::DeleteGMObject(targetCompoGCurveArr);
	}

	int numCurve = profileCurveList.size();
	for (int i = 0; i < numCurve; i++)
	{
		GM2DCurve* pCurve = profileCurveList[i];
		GM2DCurve* pNextCurve = profileCurveList[(i + 1) % numCurve];

		vector<GM2DVector> CurCrossVecArr;
		vector<double> MyLocArr, UrLocArr;
		vector<long> ResultArr;

		pCurve->IntersectEnd2(pNextCurve, CurCrossVecArr, MyLocArr, UrLocArr, ResultArr, true, dTol, DTOL_MODELING, dTol);
		if (CurCrossVecArr.size() > 0)
		{
			GM2DVector checkMyCrossVec = CurCrossVecArr[0];
			if (MyLocArr.size() == 2)
			{
				if (fabs(MyLocArr[0]) > fabs(MyLocArr[1]))
					checkMyCrossVec = CurCrossVecArr[1];
			}

			if (pCurve->m_P0.Distance(checkMyCrossVec) > pCurve->m_P1.Distance(checkMyCrossVec))
				pCurve->SetP1(checkMyCrossVec);
			else
				pCurve->SetP0(checkMyCrossVec);

			GM2DVector checkUrCrossVec = CurCrossVecArr[0];
			if (UrLocArr.size() == 2)
			{
				if (fabs(UrLocArr[0]) > fabs(UrLocArr[1]))
					checkUrCrossVec = CurCrossVecArr[1];
			}

			if (pNextCurve->m_P0.Distance(checkUrCrossVec) > pNextCurve->m_P1.Distance(checkUrCrossVec))
				pNextCurve->SetP1(checkUrCrossVec);
			else
				pNextCurve->SetP0(checkUrCrossVec);
		}
	}

	pInnerProfile->ResetPolyline();
	pInnerProfile->AddSegments(profileCurveList);

	GMObject::DeleteGMObject(profileCurveList);
	GMObject::DeleteGMObject(outerCurveArr);

	if (pInnerProfile->IsClosed() == false)
	{
		pInnerProfile->CopyFrom(&outerProfile);
		bRval = false;
	}

	pInnerProfile->MakeClosedPolyline(DTOL5);
	pInnerProfile->MakeCCW();

	return bRval;
}

void MSSlabG::UpdateInnerPCSlabProfile(GM2DPolyline* pInnerProfile, GM2DPolyline& outerProfile, double dMaxCompoGWidth)
{
	double dTol = 0.1;
	if (IsPCSlabGroup())
	{
		pInnerProfile->MakeCCW();
		pInnerProfile->SetStartIdxOnBottomLeft();
		outerProfile.SetStartIdxOnBottomLeft();

		vector<GM2DCurve*> profileCurveList;

		vector<GM2DCurve*> innerCurveArr;
		pInnerProfile->GetSegments(innerCurveArr);
		vector<GM2DCurve*> outerCurveArr;
		outerProfile.GetSegments(outerCurveArr);

		GM2DBound boundBox;
		outerProfile.Get2DBound(boundBox);
		double maxLen = max(boundBox.GetWidth(), boundBox.GetHeight());

		//dMaxCompoGWidth
		for (int iCurve = 0; iCurve < outerCurveArr.size(); iCurve++)
		{
			GM2DCurve* pCurCurve = outerCurveArr[iCurve];
			GM2DVector midVec = pCurCurve->PointAtParam(0.5);

			double dMinDist = dMaxCompoGWidth * 2;
			GM2DCurve* pColinearCurver = pCurCurve;
			for (int jCurve = 0; jCurve < innerCurveArr.size(); jCurve++)
			{
				GM2DCurve* pInnerCurver = innerCurveArr[jCurve];
				if (pInnerCurver->Length() < dMaxCompoGWidth)
					continue;

				long nRval = pCurCurve->IsColinear4DTol(pInnerCurver, DTOL_MODELING, dMaxCompoGWidth + DTOL_MODELING);
				if (nRval != 0)
				{
					double dist = pInnerCurver->Distance(midVec);
					if (dMinDist > dist && dist > 5)
					{
						if (nRval == -1)
							pInnerCurver->ReverseDirection();
						pColinearCurver = pInnerCurver;
						dMinDist = dist;
					}
				}
			}

			pColinearCurver->ExtendLineVector(true, maxLen, false);
			pColinearCurver->ExtendLineVector(false, maxLen, true);
			outerProfile.GetCurveInPolygon(pColinearCurver);
			profileCurveList.push_back(pColinearCurver);
		}

// 		vector<GM2DPolyline*> polyList;
// 		AcisGeomHelper::FindClosedProfiles(profileCurveList, polyList);

		int numCurve = profileCurveList.size();
		for (int i = 0; i < numCurve; i++)
		{
			GM2DCurve* pCurve = profileCurveList[i];
			GM2DCurve* pNextCurve = profileCurveList[(i + 1) % numCurve];

			vector<GM2DVector> CurCrossVecArr;
			vector<double> MyLocArr, UrLocArr;
			vector<long> ResultArr;

			pCurve->IntersectEnd2(pNextCurve, CurCrossVecArr, MyLocArr, UrLocArr, ResultArr, true, dTol, DTOL_MODELING, dTol);
			if (CurCrossVecArr.size() > 0)
			{
				if(pCurve->m_P0.Distance(CurCrossVecArr[0]) > pCurve->m_P1.Distance(CurCrossVecArr[0]))
					pCurve->SetP1(CurCrossVecArr[0]);
				else
					pCurve->SetP0(CurCrossVecArr[0]);

				if (pNextCurve->m_P0.Distance(CurCrossVecArr[0]) > pNextCurve->m_P1.Distance(CurCrossVecArr[0]))
					pNextCurve->SetP1(CurCrossVecArr[0]);
				else
					pNextCurve->SetP0(CurCrossVecArr[0]);
			}
		}

		pInnerProfile->ResetPolyline();
		pInnerProfile->AddSegments(profileCurveList);

// 		if(polyList.size() > 0)
// 			pInnerProfile->CopyFrom(polyList[0]);

//		GMObject::DeleteGMObject(polyList);
		GMObject::DeleteGMObject(innerCurveArr);
		GMObject::DeleteGMObject(outerCurveArr);
	}
}

//copy constructor에 의해 copy됨
GM2DPolyline MSSlabG::GetInnerProfile(bool isTop)
{	
	if(isTop)
		return *mp_InnerProFile;
	
	return *mp_InnerProFile_Bot;
}

GM2DPolyline* MSSlabG::GetTempTrimmedDeckProfile()
{
	double dSlabTopLevel = 0;
	double dSlabBotLevel = 0;
	GetZLevels(dSlabTopLevel, dSlabBotLevel);
	bool isDebugPrint = false;
	double dTol = 0.1;
	GM2DPolyline outerProfile(TRUE);
	outerProfile.CopyFrom(GetTrimmedBoundary());
	outerProfile.DeleteInLinePoint(DTOL_MODELING);
	outerProfile.MakeClosedPolyline(DTOL_MODELING);
	GM2DPolyline* pTempProfile = new GM2DPolyline();
	pTempProfile->CopyFrom(&outerProfile);
	
	CString msg;
	outcome out;
	try
	{
		if (outerProfile.ma_Point.GetCount() > 2)
		{
			bool IsAPIFailed = false;
			API_NOP_BEGIN

			double dMaxCompoGWidth = 0;
			long SlabMType = GetSlabMType();
			if (!IsAPIFailed && (
				SlabMType == MSRcSlabM::SLAB_TYPE::PC_TYPE ||
				SlabMType == MSRcSlabM::SLAB_TYPE::DECK_TYPE ||
				SlabMType == MSRcSlabM::SLAB_TYPE::DECK_GENERAL ||
				SlabMType == MSRcSlabM::SLAB_TYPE::PC_IMT ||
				SlabMType == -1) &&//사변지지||PC||Deck||DECK_GENERAL 일경우만 내부폴리곤 계산. -1인 경우는 M이 할당되지 않은 단순 모양 모델링 경우

				NeedToUpdateInnerProFile() == true)
			{
				BODY* GivenPolygon = NULL;
				if (!AcisGeomHelper::MakeFaceFromPolyline(&outerProfile, GivenPolygon))
					IsAPIFailed = true;

				ENTITY_LIST subFaces;

				vector<MSCompoG*> compoGs = FindConnectMember(FindType_MSElement::msNULL, FIND_ALLFLOOR, dTol, true, true, true);
				bool hasSteel = false;

				for (int i = 0; i < compoGs.size(); i++)
				{
					MSCompoG* compoG = compoGs[i];
					Type_MSCompoG compoGType = compoG->GetType();
					if ((compoGType != Type_MSCompoG::msBeamG && compoGType != Type_MSCompoG::msWallG && compoGType != Type_MSCompoG::msColumnG))
						continue;

					if (hasSteel == false && compoG->IsSteel() == true)
						hasSteel = true;
				
					double dTopLevel = 0;
					double dBotLevel = 0;
					compoG->GetZLevels(dTopLevel, dBotLevel);
					if (dSlabBotLevel > dTopLevel || dSlabTopLevel < dBotLevel)
						continue;

					// 상부에 터치한 객체는 처리하지 않는다.
					if (fabs(dSlabTopLevel - dBotLevel) < DTOL_GM)
						continue;

					// 폴리라인과 평행하지 않은 라인부재는 InnerProfile에 포함하지 않는다.
					if (compoG->IsLinkMember())
					{
						double dDegTol = 5;
						MSLinkMemberG* pLinkG = (MSLinkMemberG*)compoG;
						shared_ptr<GM2DCurve> pCurve = pLinkG->GetCenCurve();
						if (this->GetTrimmedBoundary()->HasColinearCurve(&*pCurve, dDegTol, pLinkG->GetWidth() / 2 + DTOL_MODELING * 3) == false)
							continue;
					}
					else if (compoG->IsPointMember())
					{
						GM2DPolyline* pPoly = compoG->GetTrimmedBoundary();
						vector<GM2DPolyline*> joined_Polys;
						outerProfile.BoolOperation('A', pPoly, joined_Polys, dTol);
						int PolyCnt = joined_Polys.size();
						GMObject::DeleteGMObject(joined_Polys);
						if (PolyCnt > 1)
							continue;
					}

					BODY* subFace = NULL;
					if (!AcisGeomHelper::MakeFaceFromPolyline(compoG->GetTrimmedBoundary(), subFace))
					{

						msg.Format(L"Slab %d Error in making support face from %d", m_ID, compoG->m_ID);
						StatusReporterWrapper::OutputError(msg);
					}

					if (subFace == NULL || (FACE*)subFace->lump()->shell()->face() == NULL)
						continue;

					dMaxCompoGWidth = max(dMaxCompoGWidth, compoG->GetWidth());

					subFaces.add(subFace);

				}

				if(hasSteel)
					GetBuilding()->GetQuantityOption()->m_nTouchSteelBeam = 1; 
				else
					GetBuilding()->GetQuantityOption()->m_nTouchSteelBeam = 0;

				ENTITY* subFaces_iter;


				subFaces.init();
				while ((subFaces_iter = subFaces.next()) != NULL)
				{
					BODY* bodyToSubtract = (BODY*)subFaces_iter;

					API_TRIAL_BEGIN
						result = api_subtract(bodyToSubtract, GivenPolygon);
					API_TRIAL_END

						CString msg;
					msg.Format(L" while Boolean subtract %d ", m_ID);
					if (!AcisGeomHelper::CheckAcisError(result, msg, false))
					{
						//Subtract실패시, 
						//try remove-short-edges
						{
							ENTITY_LIST returned_list;
							API_TRIAL_BEGIN
								result = api_detect_short_edges(bodyToSubtract, returned_list, SPAresfit*0.1, true);
							API_TRIAL_END
								msg.Format(L" while detecting short edges %d ", m_ID);
							AcisGeomHelper::CheckAcisError(result, msg);

						}

						//sheet-thicken
						SPAposition box_h(0, 0, 0);
						SPAposition box_l(0, 0, 0);
						API_TRIAL_BEGIN
							result = api_sheet_thicken(bodyToSubtract, 1.0, true, box_l, box_h);
						API_TRIAL_END

							msg.Format(L" while sheet thicken %d ", m_ID);
						if (!AcisGeomHelper::CheckAcisError(result, msg))
						{
							if (isDebugPrint)
							{
								CStringA file;
								file.Format("E:\\ID_%d_ThickenFail%d.sat", m_ID);

								ENTITY_LIST resultshape;
								resultshape.add(bodyToSubtract);
								AcisGeomHelper::SaveEntities(file.operator LPCSTR(), resultshape, true);
							}
							IsAPIFailed = true;
						}
						else//try again 
						{
							API_TRIAL_BEGIN
								result = api_subtract(bodyToSubtract, GivenPolygon);//, &boolOpt);
							API_TRIAL_END
								msg.Format(L"while Boolean Subtract %d ", m_ID);
							if (!AcisGeomHelper::CheckAcisError(result, msg))
							{
								IsAPIFailed = true;

							}
							else
							{
								IsAPIFailed = false;
							}
						}
					}

					AcisGeomHelper::LeaveLargestFaceOnly(GivenPolygon);
				}

				//if (!IsAPIFailed)
				{
					api_clean_entity(GivenPolygon);
					if (isDebugPrint)
					{
						CStringA file;
						file.Format("E:\\ID_%d_Result.sat", m_ID);

						ENTITY_LIST resultshape;
						resultshape.add(GivenPolygon);
						AcisGeomHelper::SaveEntities(file.operator LPCSTR(), resultshape, true);
					}

					// inner Profile로의 결과 변환
					AcisGeomHelper::ConvertBody(GivenPolygon, *pTempProfile, false);

					// pTempProfile이 outerProfile 안에 포함되어 있으면 정상 포함되지 않으면 비정상
					bool isContain = pTempProfile->Contains(pTempProfile);
					if (isContain == false)
						IsAPIFailed = true;
					else
						IsAPIFailed = false;
				}

			}


			API_NOP_END

				if (IsAPIFailed || pTempProfile == NULL || pTempProfile->GetNumLines() < 2)
					pTempProfile->CopyFrom(&outerProfile);
		}
	}
	catch (const std::exception& ex)
	{
		CString errorMsg;
		errorMsg.Format(L"Standard exception occurred in Slab %d while getting inner profile: %S", m_ID, ex.what());
		StatusReporterWrapper::OutputError(errorMsg);

		pTempProfile->CopyFrom(&outerProfile);
	}
#ifndef _DEBUG
	catch (...)
	{
		CString msg;
		msg.Format(L"Unexpected exception occurred in Slab %d while getting inner profile", m_ID);
		StatusReporterWrapper::OutputMessage(msg);

		pTempProfile->CopyFrom(&outerProfile);
	}
#endif

	pTempProfile->MakeClosedPolyline(DTOL5);
	return pTempProfile;
}

void MSSlabG::UpdateDeckSlabQTProFile()
{
	// 0. 옵셋값과 슬래브 타입을 가지고 온다,
	double Deck_extraRebar = GetBuilding()->GetQuantityOption()->m_nRebarEmbeddedDeckExtra;
	double GenerDeck_extraRebar = GetBuilding()->GetQuantityOption()->m_nGeneralDeckExtra;
	double PC_extraRebar = GetBuilding()->GetQuantityOption()->m_nPCSlabExtra;
	long SlabMType = GetSlabMType();
	double extraShort;
	if (SlabMType == 4)
		extraShort = Deck_extraRebar;
	else if (SlabMType == 7)
		extraShort = GenerDeck_extraRebar;
	else if (SlabMType == 8)
		extraShort = PC_extraRebar;
	else
		extraShort = 0;
	// 1. 이너 프로파일 가지고 온다
	//if (mp_InnerProFile == nullptr || mp_InnerProFile->GetNumLines() == 0)
	GM2DPolyline* pTempP = GetTempTrimmedDeckProfile();
	
	// 2. 이너 프로파일을 옵셋
	GM2DPolyline offSetProFile;
	int option = GetBuilding()->GetQuantityOption()->m_nTouchSteelBeam;
	if (option == 1)
		pTempP->OffSet_Acis(extraShort, TRUE, offSetProFile);
	else
		pTempP->OffSet_Acis(0, TRUE, offSetProFile);
	delete pTempP;
	pTempP = nullptr;

	// 3. 원래 입력된 프로파일과 교차되는 영역을  DeckSlabQTProFile로 설정한다.
	vector<GM2DPolyline*> JoinedPolys;
	offSetProFile.BoolOperationNSortByArea('I', GetTrimmedBoundary(), JoinedPolys, DTOL_GM);

	if (mp_DeckSlabQTProFile == nullptr)
		mp_DeckSlabQTProFile = new GM2DPolyline();

	if (JoinedPolys.size() > 0)
		mp_DeckSlabQTProFile->CopyFrom(JoinedPolys[0]);

	GMObject::DeleteGMObject(JoinedPolys);
	//mp_DeckSlabQTProFile
}

GM2DPolyline MSSlabG::GetDeckSlabQTProFile()
{
	return *mp_DeckSlabQTProFile;
}

GM2DPolyline* MSSlabG::GetLxLyProFile()
{
	return mp_LxLyProFile;
}

void MSSlabG::Translate( GM2DVector &pDelta )
{
	MSCompoG::Translate(pDelta);
	//mp_Boundary->Translate(pDelta);	// MSCompoG::Translate(pDelta) 에서 처리함

	mp_InnerProFile->Translate(pDelta);
	mp_InnerProFile_Bot->Translate(pDelta);
	mp_DeckSlabQTProFile->Translate(pDelta);
	mp_LxLyProFile->Translate(pDelta);
	mp_PolylineInfo->GetProfile()->Translate(pDelta);// OrgProfile: 사용자가 입력한 폴리라인
	GetTrimmedBoundary(true)->Translate(pDelta);// TopTrimmedProfile
	GetTrimmedBoundary(false)->Translate(pDelta);// BotTrimmedProfile
	GetTrimmedBoundary_Acis()->Translate(pDelta);

	long nVec = ma_SlopePoints.size();
	for(long iVec=0; iVec<nVec; iVec++)
		ma_SlopePoints[iVec].Translate(pDelta);

	for(long i = 0 ; i < ma_LevelPoly.size() ; i++)
	{
		MSSlabLevelZone* pSlabLevelZone = ma_LevelPoly[i];
		if(pSlabLevelZone == NULL)
			continue;
		pSlabLevelZone->mp_Polyline->Translate(pDelta);
	}
	
	for (int i = 0; i < ma_Haunches .size(); ++i)
		ma_Haunches [i]->Translate(pDelta);

	for (auto it = mm_Well.begin(); it != mm_Well.end(); it++)
		it->second->Translate(pDelta);

	for (auto it = mm_Trench.begin(); it != mm_Trench.end(); it++)
		it->second->Translate(pDelta);

	for (int i = 0; i < ma_EdgeWall.size(); i++)
		ma_EdgeWall[i]->Translate(pDelta);
}

void MSSlabG::MirrorByAxis( GM2DLineVector& pAxis )
{
	MSCompoG::MirrorByAxis(pAxis);
	//mp_Boundary->MirrorByAxis(pAxis);	// MSCompoG::Translate(pDelta) 에서 처리함

	mp_InnerProFile->MirrorByAxis(pAxis);
	mp_InnerProFile_Bot->MirrorByAxis(pAxis);
	mp_DeckSlabQTProFile->MirrorByAxis(pAxis);
	mp_LxLyProFile->MirrorByAxis(pAxis);
	mp_PolylineInfo->GetProfile()->MirrorByAxis(pAxis);// OrgProfile: 사용자가 입력한 폴리라인
	GetTrimmedBoundary(true)->MirrorByAxis(pAxis);// TopTrimmedProfile
	GetTrimmedBoundary(false)->MirrorByAxis(pAxis);// BotTrimmedProfile
	GetTrimmedBoundary_Acis()->MirrorByAxis(pAxis);

	MakeCCW();

	long nVec = ma_SlopePoints.size();
	for(long iVec=0; iVec<nVec; iVec++)
		ma_SlopePoints[iVec].MirrorByAxis(pAxis);

	for(long i = 0 ; i < ma_LevelPoly.size() ; i++)
	{
		MSSlabLevelZone* pSlabLevelZone = ma_LevelPoly[i];
		if(pSlabLevelZone == NULL)
			continue;
		pSlabLevelZone->mp_Polyline->MirrorByAxis(pAxis);
	}

	for (int i = 0; i < ma_Haunches .size(); ++i)
		ma_Haunches [i]->MirrorByAxis(pAxis);

	for (auto it = mm_Well.begin(); it != mm_Well.end(); it++)
		it->second->MirrorByAxis(pAxis);

	for (auto it = mm_Trench.begin(); it != mm_Trench.end(); it++)
		it->second->MirrorByAxis(pAxis);

	for (int i = 0; i < ma_EdgeWall.size(); i++)
		ma_EdgeWall[i]->MirrorByAxis(pAxis);

	SetLxDeg(m_dLxDeg + (pAxis.GetDegree() - m_dLxDeg) * 2);

// 	// BH-4522 IMT PC Slab는 배근각도가 회전각도에 따라가야 한다.
// 	if (IsIMTPCSlab())
// 	{
// 		GM2DVector p1(0, 0);
// 		GM2DVector p2(p1, 1, m_dLxDeg);
// 		GM2DLineVector dir(p1, p2);
// 		dir.MirrorByAxis(pAxis);
// 
// 		m_dLxDeg += dir.GetDegree() * Rad2Deg;
// 	}
}

void MSSlabG::RotateZ( double theta, GM2DVector *pCenter )
{
	MSCompoG::RotateZ(theta, pCenter);
	//mp_Boundary->RotateZ(theta, pCenter);	// MSCompoG::Translate(pDelta) 에서 처리함

	mp_InnerProFile->RotateZ(theta, pCenter);
	mp_InnerProFile_Bot->RotateZ(theta, pCenter);
	mp_DeckSlabQTProFile->RotateZ(theta, pCenter);
	mp_LxLyProFile->RotateZ(theta, pCenter);
	mp_PolylineInfo->GetProfile()->RotateZ(theta, pCenter);// OrgProfile: 사용자가 입력한 폴리라인
	GetTrimmedBoundary(true)->RotateZ(theta, pCenter);// TopTrimmedProfile
	GetTrimmedBoundary(false)->RotateZ(theta, pCenter);// BotTrimmedProfile
	GetTrimmedBoundary_Acis()->RotateZ(theta, pCenter);

	long nVec = ma_SlopePoints.size();
	for(long iVec=0; iVec<nVec; iVec++)
		ma_SlopePoints[iVec].RotateZ(theta, pCenter);

	for(long i = 0 ; i < ma_LevelPoly.size() ; i++)
	{
		MSSlabLevelZone* pSlabLevelZone = ma_LevelPoly[i];
		if(pSlabLevelZone == NULL)
			continue;
		pSlabLevelZone->mp_Polyline->RotateZ(theta, pCenter);
	}

	for (int i = 0; i < ma_Haunches .size(); ++i)
		ma_Haunches [i]->RotateZ(theta, pCenter);

	for (auto it = mm_Well.begin(); it != mm_Well.end(); it++)
		it->second->RotateZ(theta, pCenter);

	for (auto it = mm_Trench.begin(); it != mm_Trench.end(); it++)
		it->second->RotateZ(theta, pCenter);

	for (int i = 0; i < ma_EdgeWall.size(); i++)
		ma_EdgeWall[i]->RotateZ(theta, pCenter);

	// BH-4522 IMT PC Slab는 배근각도가 회전각도에 따라가야 한다.
// 	if (IsIMTPCSlab())
		m_dLxDeg += theta * Rad2Deg;
}

void MSSlabG::MakeSlabOpening()
{
	if (mp_PolylineInfo == NULL)
		return;

	DeleteSlabOpening();

	vector<MSCompoG*> SlabArr = FindConnectMember(FindType_MSElement::msSlabG, FIND_CURFLOOR, DTOL_GM, true, false, false);
	GM2DPolyline* pCurPoly = GetProfile();

	for (long iSlab = 0; iSlab < SlabArr.size(); iSlab++)
	{
		MSSlabG* pSlab = (MSSlabG*)SlabArr[iSlab];
		GM2DPolyline* pChekPoly = pSlab->GetProfile();

		if (pCurPoly->Contains(pChekPoly, false))
			mp_PolylineInfo->AddOpening(pChekPoly, pSlab);
	}
}

void MSSlabG::DeleteSlabOpening()
{
	if (mp_PolylineInfo == NULL)
		return;

	for (long i = 0; i < mp_PolylineInfo->ma_Opening.size(); i++)
	{
		MSOpening* pOpening = mp_PolylineInfo->ma_Opening[i];
		delete pOpening;
	}

	mp_PolylineInfo->ma_Opening.clear();	
}

void MSSlabG::InitTrimmedBoundary(bool IsTopLevel)
{
	if (IsTopLevel == true)
		MSCompoG::InitTrimmedBoundary(IsTopLevel);
	else
	{
		if (mp_BotProfile == NULL)
			mp_BotProfile = new GM2DPolyline(TRUE);
		GetBoundary(*mp_BotProfile);
	}
}

void MSSlabG::InitTrimmedBoundaryVisibility()
{
	GM2DPolyline* pPoly = GetTrimmedBoundary();
	ma_TrimmedBoundaryVisible.clear();
	for(long iSeg = 0 ; iSeg < pPoly->GetNumLines() ; iSeg++)
		ma_TrimmedBoundaryVisible.push_back(true);
}

void MSSlabG::UpdateBoundaryVisibilities(long nUpdateConnIdx /*= 0*/)
{
	vector<GM2DCurve*> SegArr;
// 	InitTrimmedBoundary();
// 	GM2DPolyline* pPoly = GetTrimmedBoundary();
	GM2DPolyline* pTrimmedPoly = GetTrimmedBoundary();
	GM2DPolyline* pPoly = GetProfile();
	//pPoly->CopyFrom(GetTrimmedBoundary_Acis());
	pPoly->CopyFrom(mp_PolylineInfo->mp_Profile);

	double dTol = DTOL_MODELING;

	vector<MSOpening*> PartnerOpeningArr;
	GM2DPolyline* pTopTrimmedPolyLine = GetTrimmedBoundary();
	vector<MSCompoG*> OpeningGArr = GetOpeningGArr();
	for(long iOpen = 0 ; iOpen < OpeningGArr.size() ; iOpen++)
	{
		MSOpeningG* pOpeningG = (MSOpeningG*)OpeningGArr[iOpen];
		GM2DPolyline* pOpenPoly = pOpeningG->GetProfile();
		if(pOpenPoly == NULL)
			continue;

		pPoly->SegmentstoMe(pOpenPoly, dTol, FALSE);
		// 슬래브 MSSlabGCli::GetTrimmedBoundaryCurves(bool isTop) 함수 변경으로 인한 추가 작업
		pTopTrimmedPolyLine->SegmentstoMe(pOpenPoly, dTol, FALSE);
	}

	vector<MSCompoG*> PitArr = FindConnectMember(FindType_MSElement::msPitG, FIND_CURFLOOR);
	for(long iPit = PitArr.size() -1 ; iPit >=0 ; iPit--)
	{
		MSPitG* pPitG = (MSPitG*)PitArr[iPit];

		long eZLevelInfo = GetZLevelInfo(pPitG);
		if (eZLevelInfo & ZLevelConnection ||
			eZLevelInfo & LowUrMember)
		{
			PitArr.erase(PitArr.begin() + iPit);
			continue;
		}

		// 마감 프로젝트인 경우
		/*
		if (AppGlobalOptionWrapper::Instance()->GetAppMode() == AppGlobalOptionWrapper::Finish)
		{
			for (int idx = 0; idx < pPitG->ma_InPoly.GetSize(); idx++)
			{
				GM2DPolyline* pInPoly = pPitG->ma_InPoly[idx];
				if (pInPoly == NULL)
					continue;

				pPoly->SegmentstoMe(pInPoly, dTol, FALSE);
				// 슬래브 MSSlabGCli::GetTrimmedBoundaryCurves(bool isTop) 함수 변경으로 인한 추가 작업
				pTopTrimmedPolyLine->SegmentstoMe(pInPoly, dTol, FALSE);
			}
		}
		else
		*/
		{
			GM2DPolyline* pPitPoly = pPitG->mp_OutPoly;
			if (pPitPoly == NULL)
				continue;

			pPoly->SegmentstoMe(pPitPoly, dTol, FALSE);
			// 슬래브 MSSlabGCli::GetTrimmedBoundaryCurves(bool isTop) 함수 변경으로 인한 추가 작업
			pTopTrimmedPolyLine->SegmentstoMe(pPitPoly, dTol, FALSE);
		}
	}
	

	// MySlab를 점선으로 변경
	vector<MSCompoG*> HiddenSlabArr;

	// MySlab를 삭제
	//vector<MSCompoG*> InVisibleSlabArr = FindConnectMember(MSElement::msSlabG, FIND_CURFLOOR);
	vector<MSCompoG*> InVisibleSlabArr;
	for (int iConn = 0; iConn < ma_Connections.size(); iConn++)
	{
		MSConnection* pConn = ma_Connections[iConn];
		MSSlabG* pPartnerG = (MSSlabG*)pConn->GetPartner(this);
		if (pPartnerG != nullptr && pPartnerG->GetType() == MSElement::msSlabG)
		{
			bool isLevelOverlap = IsLevelOverlap(pPartnerG);
			if (isLevelOverlap == false) continue;

			if(GetContainingElementId() == pPartnerG->m_ID)
				continue;

			if (AppGlobalOptionWrapper::Instance()->GetAppMode() == AppGlobalOptionWrapper::Formwork)
			{
				// 모델링 된 오프닝과 동일한 프로파일을 가지는 슬래브는 그려줘야한다.
				bool hasSameOpening = false;
				for (int i = 0; i < pPartnerG->mp_PolylineInfo->ma_Opening.size(); i++)
				{
					GM2DPolyline* pOpeningPoly = pPartnerG->mp_PolylineInfo->ma_Opening[i]->GetProfile();
					if (pOpeningPoly->SamePolyLine(pTrimmedPoly))
					{
						hasSameOpening = true;
						break;
					}
				}
				if(hasSameOpening)
					continue;
			}

			//MSPolylineObject::TrimExistPolyLineObject 함수에서 pConn->GetFirstCompoG() == pMySlabG 인경우 교차점을 입력해줬으므로 여기서도 처리해야함
			if (pConn->GetFirstCompoG() == this)
				pPoly->SegmentstoMe(pPartnerG->GetProfile(), dTol, FALSE);
			
			if (pTrimmedPoly->OverlapEx(*pPartnerG->GetTrimmedBoundary()) == true)
				InVisibleSlabArr.push_back(pPartnerG);
		}
	}

	for (long iSlab = InVisibleSlabArr.size() - 1; iSlab >= 0; iSlab--)
	{
		MSSlabG* pSlab = (MSSlabG*)InVisibleSlabArr[iSlab];

		double MyTopLevel = 0, MyBotLevel = 0;
		double UrTopLevel = 0, UrBotLevel = 0;
		GetZLevels(MyTopLevel, MyBotLevel, false, false);
		pSlab->GetZLevels(UrTopLevel, UrBotLevel, false, false);

		if (fabs(MyBotLevel - UrTopLevel) < DTOL1 || (fabs(UrBotLevel - MyTopLevel) < DTOL1)) // 연결
		{
			InVisibleSlabArr.erase(InVisibleSlabArr.begin() + iSlab);
			continue;
		}

		if (fabs(MyTopLevel - UrTopLevel) < DTOL1) // 같은 Level에서만 Connection의 우선순위를 사용함
		{
			MSConnection* pConn = GetConnection(pSlab);
			if (pConn == nullptr || pConn->GetFirstCompoGID() == m_ID)
			{
				InVisibleSlabArr.erase(InVisibleSlabArr.begin() + iSlab);
				continue;
			}

			if (pSlab->mp_PolylineInfo->FindOpening_MadeByCompoG(m_ID) != nullptr)
			{
				InVisibleSlabArr.erase(InVisibleSlabArr.begin() + iSlab);
				continue;
			}

			if (pConn->GetSecondCompoGID() == m_ID && MyBotLevel < UrBotLevel)
			{
				HiddenSlabArr.push_back(pSlab);
				InVisibleSlabArr.erase(InVisibleSlabArr.begin() + iSlab);
			}
		}
		else
		{
			for (int iOpening = 0; iOpening < pSlab->mp_PolylineInfo->ma_Opening.size(); iOpening++)
			{
				MSOpening* pOpening = pSlab->mp_PolylineInfo->ma_Opening[iOpening];
				if (pOpening->IsMadeByCompoG())
					continue;

				GM2DPolyline* pOpeningPoly = pOpening->GetProfile();
				pPoly->SegmentstoMe(pOpeningPoly, dTol, FALSE);
				pTrimmedPoly->SegmentstoMe(pOpeningPoly, dTol, FALSE);
				PartnerOpeningArr.push_back(pOpening);
			}

			if (AppGlobalOptionWrapper::Instance()->GetAppMode() == AppGlobalOptionWrapper::Formwork)
			{
				// 1.나의 슬래브 폴리라인이 파트너 폴리라인에 완전히 포함되면 점선슬래브 리스트로 처리
				if (pSlab->GetTrimmedBoundary()->Contains(pTrimmedPoly) == true)
				{
					HiddenSlabArr.push_back(pSlab);
					InVisibleSlabArr.erase(InVisibleSlabArr.begin() + iSlab);
					continue;
				}
			}

			if (MyTopLevel < UrTopLevel && MyBotLevel < UrBotLevel)
			{
				HiddenSlabArr.push_back(pSlab);
				InVisibleSlabArr.erase(InVisibleSlabArr.begin() + iSlab);
			}
			else if ((MyTopLevel > UrTopLevel && MyBotLevel < UrBotLevel) ||
				(MyTopLevel > UrTopLevel && (MyTopLevel > UrTopLevel && MyBotLevel < UrTopLevel)))
			{
				InVisibleSlabArr.erase(InVisibleSlabArr.begin() + iSlab);
				continue;
			}
		}
		
		pPoly->SegmentstoMe(pSlab->GetProfile(), dTol, FALSE);
		pTrimmedPoly->SegmentstoMe(pSlab->GetProfile(), dTol, FALSE);
	}

	MSSlabG::SortByPriorityNum(HiddenSlabArr);
	MSSlabG::SortByPriorityNum(InVisibleSlabArr);

	pPoly->GetSegments(SegArr);
	ma_TrimmedBoundaryVisible.clear();
	bool bIncludeOnLine = true;

	for(long iSeg = 0 ; iSeg < SegArr.size() ; iSeg++)
	{
		GM2DCurve* pCurve = SegArr[iSeg];
		int nVisible = 1;
		for(long iOpen = 0 ; iOpen < OpeningGArr.size() ; iOpen++)
		{
			MSOpeningG* pOpeningG = (MSOpeningG*)OpeningGArr[iOpen];
			GM2DPolyline* pOpenPoly = pOpeningG->GetProfile();
			if(pOpenPoly == nullptr || nVisible == 0)
				continue;
			if (pOpenPoly->Contains(pCurve, bIncludeOnLine) == true)
			{
				nVisible = 0;
				break;
			}
		}

		if (AppGlobalOptionWrapper::Instance()->GetAppMode() == AppGlobalOptionWrapper::Finish)
		{
			for (long iPit = 0; iPit < PitArr.size(); iPit++)
			{
				MSPitG* pPit = (MSPitG*)PitArr[iPit];
				for (int idx = 0; idx < pPit->ma_InPoly.GetSize(); idx++)
				{
					GM2DPolyline* pInPoly = pPit->ma_InPoly[idx];
					if (pInPoly == nullptr || nVisible == 0)
						continue;

					if (pInPoly->Contains(pCurve, bIncludeOnLine) == true)
					{
						nVisible = 0;
						break;
					}
				}
				
				if(nVisible == 0)
					break;;
			}
		}
		/*
		else
		{
			for (long iPit = 0; iPit < PitArr.size(); iPit++)
			{
				MSPitG* pPit = (MSPitG*)PitArr[iPit];
				GM2DPolyline* pPitPoly = pPit->mp_OutPoly;
				if (pPitPoly == nullptr || nVisible == 0)
					continue;

				if (pPitPoly->Contains(pCurve, bIncludeOnLine) == true)
				{
					nVisible = 0;
					break;
				}
			}
		}
		*/

		for (long iSlab = 0; iSlab < HiddenSlabArr.size(); iSlab++)
		{
			MSSlabG* pSlab = (MSSlabG*)HiddenSlabArr[iSlab];
			GM2DPolyline* pSlabPoly = pSlab->GetProfile();
			if (pSlabPoly == nullptr || nVisible == 2)
				continue;
			
			if (pSlabPoly->Contains(pCurve, bIncludeOnLine) == true)
			{
				nVisible = 2;
				break;
			}
		}

		for (long iSlab = 0; iSlab < InVisibleSlabArr.size(); iSlab++)
		{
			MSSlabG* pSlab = (MSSlabG*)InVisibleSlabArr[iSlab];
			GM2DPolyline* pSlabPoly = pSlab->GetProfile();
			if (pSlabPoly == nullptr || nVisible == 0)
				continue;
			
			if (pSlabPoly->Overlap(pCurve) == true)
				continue;

			if (pSlabPoly->Contains(pCurve, bIncludeOnLine) == true)
			{
				nVisible = 0;
				break;
			}
		}

		// 파트너 오프닝에 포함되는 선은 실선으로 표시한다.
		for (long iOpening = 0; iOpening < PartnerOpeningArr.size(); iOpening++)
		{
			MSOpening* pOpening = PartnerOpeningArr[iOpening];
			GM2DPolyline* pOpeningPoly = pOpening->GetProfile();
			
			if (pOpeningPoly == nullptr || nVisible == 1)
				continue;

			if (pOpeningPoly->Contains(pCurve, bIncludeOnLine) == true)
				nVisible = 1;
		}

		ma_TrimmedBoundaryVisible.push_back(nVisible);
		delete pCurve;
	}

	UpdateInnerProfile();
	MSCompoG::UpdateBoundaryVisibilities(nUpdateConnIdx);

	if (GetContainingElementId() != 0)
	{
		MSFloor* pFloor = GetFloor();
		MSCompoG* pContainCompoG = pFloor->GetCompoG(GetContainingElementId());
		if (pContainCompoG != nullptr && pContainCompoG->GetContainingElementId() != m_ID)
			pContainCompoG->UpdateBoundaryVisibilities();
	}
}

vector<MSCompoG*> MSSlabG::GetOpeningGArr()
{
	mm_OpeningID.clear();
	vector<MSCompoG*> OpeningGArr;

	MSFloor* pFloor = GetFloor();
	GM2DPolyline* pMyBoundary = GetProfile();
	double dTol = DTOL_GM;
	double slabTop = 0;
	double slabBot = 0;
	GetZLevels(slabTop, slabBot, false);

	vector<MSCompoG*> openingGs = MSCompoGUtil::FindCompoG(pFloor, pMyBoundary, FindType_MSElement::msOpeningG, false, true, dTol, false);

	for(long i = 0 ; i < openingGs.size() ; i++)
	{
		MSCompoG* pCompoG = openingGs[i];
		if(pCompoG == this)	
			continue;
		MSOpeningG* pOpeningG = dynamic_cast<MSOpeningG*>(pCompoG);
		if (pOpeningG == nullptr) 
			continue;
		if (pOpeningG->GetUseOffset())
		{
			double openingTop = 0;
			double openingBot = 0;
			pCompoG->GetZLevels(openingTop, openingBot);
			
			if (openingBot >= slabTop)
				continue;
			if (openingTop <= slabBot)
				continue;
		}
		
		OpeningGArr.push_back(pCompoG);
		mm_OpeningID.insert(make_pair(pCompoG->m_ID, pCompoG->m_ID));
	}

	return OpeningGArr;
}

void MSSlabG::GetInnerSlabGnPitGArr(vector<MSCompoG*>& inner)
{
	//슬래브 안에 완전 포함되어 오프닝을 가지고 있는 슬래브들을 반환
	vector<MSOpening*> crossOpenings;
	
	GetInnerSlabGnPitGArrAndCrossOpening(inner, crossOpenings);
}

void MSSlabG::GetInnerSlabGnPitGArrAndCrossOpening(vector<MSCompoG*>& inner, vector<MSOpening*>& crossOpenings)
{
	//슬래브 오프닝과 슬래브 교차시의 opening을 반환
	inner.clear();
	crossOpenings.clear();
	MSFloor* pFloor = GetFloor();

	for (int i = 0; i < mp_PolylineInfo->ma_Opening.size(); i++)
	{
		bool isInnerSlab = false;
		MSOpening* pOpen = mp_PolylineInfo->ma_Opening[i];

		if (pOpen != nullptr)
		{
			for (int iCompoG = 0; iCompoG < pOpen->ma_MadeByCompoGID.size(); iCompoG++)
			{
				MSCompoG* pCompoG = pFloor->GetCompoG(pOpen->ma_MadeByCompoGID[iCompoG]);
				if (pCompoG != nullptr && pCompoG->GetContainingElementId() == m_ID)
				{
					inner.push_back(pCompoG);
					isInnerSlab = true;
				}		
			}

			if (!isInnerSlab)
			{
				crossOpenings.push_back(pOpen);
			}
		}
	}

	//19 버전 이전에서는 UpdateModelVersion() 이 0 이다. 이너 슬래브를 찾는 로직이 18->19 넘어오면서 변경되었으므로 사용자가 update를 하지 않는 경우에도
	// 같은 결과물을 보여주기 위한 코드이다. innerslab와 pit를 고려해서 슬래브 regen 을 한다.
	// 옛날 버전에서는 crossOpening을 보내주지 않는다.
	MSBaseBuilding* pBuilding = GetBuilding();
	if (pBuilding->GetUpdateModelVersion() == 0)
	{
		GM2DPolyline* pMyBoundary = GetTrimmedBoundary();
		double dTol = DTOL_GM;
		MSFloor* pFloor = GetFloor();
		vector<MSCompoG*> FindSlabGArr = MSCompoGUtil::FindCompoG(pFloor, pMyBoundary, FindType_MSElement::msSlabG, false, true, dTol);
		vector<MSCompoG*> FindPitGArr = MSCompoGUtil::FindCompoG(pFloor, pMyBoundary, FindType_MSElement::msPitG, false, true, dTol);
		FindSlabGArr.insert(FindSlabGArr.end(), FindPitGArr.begin(), FindPitGArr.end());

		for (int iCompoG = 0; iCompoG < FindSlabGArr.size(); iCompoG++)
		{
			MSCompoG* pCompoG = FindSlabGArr[iCompoG];
			if (pCompoG->m_ID == m_ID)
				continue;
			if (IsLevelOverlap(pCompoG) == false)
				continue;

			GM2DPolyline* pFindPoly = pCompoG->GetTrimmedBoundary();

			bool isContain = pMyBoundary->Contains(pFindPoly);
			if (isContain == true)
				inner.push_back(pCompoG);
		}

		sort(inner.begin(), inner.end(),
			[](MSCompoG* a, MSCompoG* b)->bool {return a->m_ID < b->m_ID; }
		);
		vector<MSCompoG*>::iterator new_end;
		new_end = unique(inner.begin(), inner.end());
		inner.erase(new_end, inner.end());
	}
}

void MSSlabG::GetInnerOrOverlapGArr(vector<MSCompoG*>& inner, vector<MSCompoG*>& overlap)
{
	//FindCompoG() 함수의 성능의 문제로 bFindOnlyOnLine 변수를 둠
	inner.clear();
	overlap.clear();

	GM2DPolyline* pMyBoundary = GetTrimmedBoundary();
	double dTol = DTOL_GM;
	MSFloor* pFloor = GetFloor();

	vector<MSCompoG*> FindCompoGArr = MSCompoGUtil::FindCompoG(pFloor, pMyBoundary, FindType_MSElement::msNULL, false, true, dTol);
	for(long iCompoG = 0 ; iCompoG < FindCompoGArr.size() ; iCompoG++)
	{
		MSCompoG* pFindCompoG = FindCompoGArr[iCompoG];
		if(pFindCompoG == this)
			continue;

		CheckModelOpt Checkopt;
		GM2DPolyline* pUrBoundary = pFindCompoG->GetTrimmedBoundary();
		if(IsLevelConnectedOverLap(pFindCompoG, Checkopt) == false)
			continue;
		
		if (pMyBoundary->Contains(pUrBoundary, true))
			inner.push_back(pFindCompoG);
		else
			overlap.push_back(pFindCompoG);
	}

	sort(inner.begin(), inner.end(),
		[](MSCompoG* a, MSCompoG* b)->bool {return a->m_ID < b->m_ID; }
	);
	vector<MSCompoG*>::iterator new_end;
	new_end = unique(inner.begin(), inner.end());
	inner.erase(new_end, inner.end());
}

vector<MSCompoG*> MSSlabG::GetPartnerCompoGs(long GetIdx)
{
	vector<MSCompoG*> CompoGArr = GetOpeningGArr();

	MSFloor* pFloor = GetFloor();
	map<long, long>::iterator it;
	for(it = mm_OpeningID.begin() ; it != mm_OpeningID.end() ; it++)
	{
		MSCompoG* pCompoG = pFloor->GetCompoG(it->second);
		if(pCompoG != NULL)
			CompoGArr.push_back(pCompoG);
	}

	for (int iConn = 0; iConn < ma_Connections.size(); iConn++)
	{
		MSConnection* pConn = ma_Connections[iConn];
		MSCompoG* pPartnerG = pConn->GetPartner(this);
		if(pPartnerG == nullptr)
			continue;
		CompoGArr.push_back(pPartnerG);
	}

	sort(CompoGArr.begin(), CompoGArr.end(), 
		[](const MSCompoG* a, const MSCompoG* b)->bool {if (a->m_nCmdCnt == b->m_nCmdCnt) return a->m_ID < b->m_ID; else return a->m_nCmdCnt < b->m_nCmdCnt; }
	);

	vector<MSCompoG*>::iterator new_end;
	new_end = unique(CompoGArr.begin(), CompoGArr.end());
	CompoGArr.erase(new_end, CompoGArr.end());

	return CompoGArr;
}

vector<GM2DCurve*> MSSlabG::GetVisibileCurves(eVisibileCurveType nVisibile)
{
// 	vector<GM2DCurve*> VisibileCurveArr;
// 	GM2DPolyline* pPoly = GetTrimmedBoundary();
// 	vector<GM2DCurve*> SegArr;
// 	pPoly->GetSegments(SegArr);
// 
// 	if(SegArr.size() != ma_TrimmedBoundaryVisible.size())
// 		UpdateBoundaryVisibilities();
// 
// 	for(long iSeg = 0 ; iSeg < SegArr.size() ; iSeg++)
// 	{
// 		if(ma_TrimmedBoundaryVisible[iSeg] == (BOOL)bVisibile)
// 			VisibileCurveArr.push_back(SegArr[iSeg]);
// 	}
// 	
// 	return VisibileCurveArr;

	vector<GM2DCurve*> VisibileCurveArr;
	GM2DPolyline* pPoly = GetProfile();
	vector<GM2DCurve*> SegArr;
	pPoly->GetSegments(SegArr);

	if (SegArr.size() != ma_TrimmedBoundaryVisible.size())
	{
		UpdateBoundaryVisibilities();
		for (long iSeg = 0; iSeg < SegArr.size(); iSeg++)
			delete SegArr[iSeg];
		SegArr.clear();
		pPoly = GetProfile();
		pPoly->GetSegments(SegArr);
	}

	for(long iSeg = 0 ; iSeg < SegArr.size() ; iSeg++)
	{
		if(ma_TrimmedBoundaryVisible[iSeg] == nVisibile)
			VisibileCurveArr.push_back(SegArr[iSeg]);
	}
	
	return VisibileCurveArr;
}

bool MSSlabG::HasHiddenBoundaryCurve()
{
	for(int i = 0 ; i < ma_TrimmedBoundaryVisible.size() ; i++)
		if (ma_TrimmedBoundaryVisible[i] == 2)
			return true;

	return false;
}

vector<MSLineLevelDifG*> MSSlabG::GetZLevelOverLapEdgeWall(MSSlabG* pUrSlabG)
{
	vector<MSLineLevelDifG*> LevelDifArr;

	GM2DPolyline* pUrPoly = pUrSlabG->GetProfile();

	for(long i = 0 ; i < ma_EdgeWall.size() ; i++)
	{
		MSLineLevelDifG* pLevelDif = ma_EdgeWall[i];
		long eZLevelInfo = pUrSlabG->GetZLevelInfo(pLevelDif);
		if(eZLevelInfo & ZLevelConnection ||
			eZLevelInfo & ZLevelOverLap)
		{
			GM2DCurve* pCurve = pLevelDif->GetCurve();

			bool bRval = pUrSlabG->Contains(pCurve, true, DTOL_MODELING);
			if(bRval == true)
				LevelDifArr.push_back(pLevelDif);
		}
	}

	for(long i = 0 ; i < pUrSlabG->ma_EdgeWall.size() ; i++)
	{
		MSLineLevelDifG* pLevelDif = pUrSlabG->ma_EdgeWall[i];
		long eZLevelInfo = GetZLevelInfo(pLevelDif);
		if(eZLevelInfo & ZLevelConnection ||
			eZLevelInfo & ZLevelOverLap)
		{
			GM2DCurve* pCurve = pLevelDif->GetCurve();

			bool bRval = Contains(pCurve, true, DTOL_MODELING);
			if(bRval == true)
				LevelDifArr.push_back(pLevelDif);
		}
	}

	return LevelDifArr;
}

bool MSSlabG::HasFullSlabZone()
{
	for(long i = 0 ; i < ma_LevelPoly.size() ; i++)
	{
		MSSlabLevelZone* pSlabLevelZone = ma_LevelPoly[i];
		if(pSlabLevelZone->m_IsDelete)
			continue;

		if(pSlabLevelZone->m_IsFullSlabZone)
			return true;
	}
	
	return false;
}

void MSSlabG::DeleteHaunchs()
{
	for (int i = 0; i < ma_Haunches .size(); i++)
		delete ma_Haunches [i];

	ma_Haunches .clear();
}

void MSSlabG::AddHaunchGs(vector<MSSubHaunchG*> HaunchGArr)
{
	for (int iHaunchG = 0; iHaunchG < HaunchGArr.size(); iHaunchG++)
	{
		MSSubHaunchG* pHaunchG = HaunchGArr[iHaunchG];
		pHaunchG->GetID();
		ma_Haunches.push_back(pHaunchG);
	}
}

void MSSlabG::AddHaunch(double x, double y, double x2, double y2, double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto)
{
	AddHaunchGs(MSCompoGUtil::MakeHaunchG(this, x, y, x2, y2, haunchAngle, haunchY, haunchY_Top, bTopAuto));
	TrimHaunches();
}

void MSSlabG::AddHaunchesOnProfile(double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto)
{
	DeleteHaunchs();
	AddHaunchGs(MSCompoGUtil::MakeHaunchGAllProfile(this, haunchAngle, haunchY, haunchY_Top, bTopAuto));
	TrimHaunches();
}

bool MSSlabG::Remove(MSSubHaunchG* pSubHaunchG)
{
	int findIdx = -1;
	for (int iHaunch = 0; iHaunch < ma_Haunches.size(); iHaunch++)
	{
		if (ma_Haunches[iHaunch] == pSubHaunchG)
			findIdx = iHaunch;
	}

	if (findIdx != -1)
	{
		ma_Haunches[findIdx]->RemoveConnections();
		ma_Haunches.erase(ma_Haunches.begin() + findIdx);
		return true;
	}

	return false;
}

void MSSlabG::TrimHaunches()
{
	CheckHaunches();

	for (int iHaunch = 0; iHaunch < ma_Haunches.size(); iHaunch++)
		ma_Haunches[iHaunch]->RemoveConnections();

	MSFloor* pFloor = GetFloor();
	MSCompoGUtil::TrimHaunches(ma_Haunches, pFloor);
}

void MSSlabG::RegenHaunch()
{
	for (int i = 0; i < ma_Haunches .size(); ++i)
		ma_Haunches [i]->SetLines();
	TrimHaunches();
}

bool MSSlabG::HasLevelZoneInfo()
{
	long nCnt = 0;
	nCnt = ma_LevelDif.size() + ma_LevelDifOnEdge.size();
	for (long iLevelPoly = 0; iLevelPoly < ma_LevelPoly.size(); iLevelPoly++)
	{
		MSSlabLevelZone* pSlabLevelZone = ma_LevelPoly[iLevelPoly];
		if(pSlabLevelZone == NULL || pSlabLevelZone->m_IsDelete == true)
			continue;
		nCnt++;
	}
	
	if (nCnt == 0)
		return false;

	return true;
}

CString MSSlabG::GetKey(bool bUseID)
{
	// ID, FloorNum, Type, CompoM, 좌표, 옵셋
	GM2DPolyline *pPoly = GetProfile();
	GM2DVector CenVec = pPoly->MassCenter();

	CString strCompoMKey;
	if (mp_CompoM == NULL || mp_CompoM->m_ID == 0)
		strCompoMKey.Format(_T("%d"), 0);
	else
		strCompoMKey.Format(_T("%d"), &mp_CompoM->m_ID);
	CString strGeo;
	strGeo.Format(_T("%d_%d_%d_%d_%d"), pPoly->GetNumLines(), (int)pPoly->GetLength(), (int)pPoly->Area(), (int)CenVec.m_X, (int)CenVec.m_Y);
	CString strOffSet;
	strOffSet.Format(_T("%d_%d_%d"), (int)IsSlopeMember(), (int)m_dOffset, (int)m_dLxDeg);

	CString strKey;
	strKey.Format(_T("%d_%d_%s_%s_%s"), GetFloorNum(), (int)GetType(), strCompoMKey, strGeo, strOffSet);
	if (bUseID)
	{
		CString strID;
		strID.Format(_T("%d_"), m_ID);
		strKey = strID + strKey;
	}

	return strKey;
}

void MSSlabG::CalcLongXLongY(GM2DPolyline& targetProfile, GM2DLineVector& longX, GM2DLineVector& longY)
{
	GM2DVector basePt(0, 0);
	GM2DVector LDVec, RDVec, LUVec, RUVec;

	GM2DPolyline outRect(TRUE);

	double dMinX, dMinY, dMaxX, dMaxY;
	targetProfile.RotateZ(-m_dLxDeg * Deg2Rad, &basePt);
	targetProfile.GetXYMinMax(dMinX, dMinY, dMaxX, dMaxY);
	LDVec.m_X = dMinX;
	LDVec.m_Y = dMinY;
	RDVec.m_X = dMaxX;
	RDVec.m_Y = dMinY;
	RUVec.m_X = dMaxX;
	RUVec.m_Y = dMaxY;
	LUVec.m_X = dMinX;
	LUVec.m_Y = dMaxY;
	outRect.ma_Point.Add(new GM2DVector(LDVec));//반시계방향으로 입력
	outRect.ma_Point.Add(new GM2DVector(RDVec));
	outRect.ma_Point.Add(new GM2DVector(RUVec));
	outRect.ma_Point.Add(new GM2DVector(LUVec));

	outRect.RotateZ(m_dLxDeg*Deg2Rad, &basePt);

	longX.m_P0 = *outRect.ma_Point.GetAt(0);
	longX.m_P1 = *outRect.ma_Point.GetAt(1);
	longY.m_P0 = *outRect.ma_Point.GetAt(0);
	longY.m_P1 = *outRect.ma_Point.GetAt(3);

	outRect.ResetPolyline();
}

void MSSlabG::GetSlabLxLy(GM2DLineVector& Lx, GM2DLineVector& Ly)
{
	GM2DPolyline slabProfile(TRUE);
	GetBoundary(slabProfile);
	CalcLongXLongY(slabProfile, Lx, Ly);
	UpdateOneorFourQuadrantLine(Lx, Ly);
}

void MSSlabG::GetLxLy(GM2DLineVector& Lx, GM2DLineVector& Ly)
{
	GM2DPolyline targrtProfile(TRUE);
	targrtProfile.CopyFrom(mp_LxLyProFile);
	CalcLongXLongY(targrtProfile, Lx, Ly);
	UpdateOneorFourQuadrantLine(Lx, Ly);
}

void MSSlabG::GetLnxLny(GM2DLineVector& Lnx, GM2DLineVector& Lny)
{
	GM2DPolyline targrtProfile(TRUE);
	targrtProfile.CopyFrom(mp_InnerProFile);
	CalcLongXLongY(targrtProfile, Lnx, Lny);
	UpdateOneorFourQuadrantLine(Lnx, Lny);
}

void MSSlabG::UpdateOneorFourQuadrantLine(GM2DLineVector& Lx, GM2DLineVector& Ly)
{
	// Lx, Ly를 1, 4사분면 방향으로 돌린다.
	double XLen = Lx.Length();
	double YLen = Ly.Length();
	double deg = Lx.GetDegree();
	if (deg > 91 && deg <= 270)
	{
		Ly.Translate(Lx.m_P1 - Lx.m_P0);
		Lx.ReverseDirection();
	}
	deg = Ly.GetDegree();
	if (deg > 91 && deg <= 270)
	{
		Lx.Translate(Ly.m_P1 - Ly.m_P0);
		Ly.ReverseDirection();
	}
}

bool MSSlabG::IsColumnStrip()
{
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
		return false;

	if (mySlabM->m_nSlabMType == MSRcSlabM::MAT_TYPE)
		return false;
	else if (mySlabM->m_nSlabMType == MSRcSlabM::FLAT_TYPE)
	{
		if (mySlabM->m_LongStrip.m_StripType == MSRcStrip::TYPE_B || mySlabM->m_ShortStrip.m_StripType == MSRcStrip::TYPE_B)
			return true;

		return false;
	}
	else if (mySlabM->m_nSlabMType == MSRcSlabM::GENERAL_TYPE)
		return true;
	else if (mySlabM->m_nSlabMType == MSRcSlabM::SOG)
		return false;
	else if (mySlabM->m_nSlabMType == MSRcSlabM::CIRCULAR_RAMP)
		return false;
	else if (mySlabM->m_nSlabMType == MSRcSlabM::DECK_TYPE)
	{
		if (mySlabM->m_IsLong1 == true && mySlabM->m_IsLong2 == true &&
			mySlabM->m_IsLong3 == true && mySlabM->m_IsLong4 == true)
		{
			if (mySlabM->m_LongStrip.m_Bar1.IsSameBar(mySlabM->m_LongStrip.m_Bar2) == true &&
				mySlabM->m_LongStrip.m_Bar3.IsSameBar(mySlabM->m_LongStrip.m_Bar4) == true)
				return false;
		}

		return true;
	}
	else if (mySlabM->m_nSlabMType == MSRcSlabM::DECK_GENERAL)
		return true;
	else if (mySlabM->m_nSlabMType == MSRcSlabM::PC_TYPE || mySlabM->GetSlabMType() == MSRcSlabM::PC_IMT)
		return false;

	return false;
}

bool MSSlabG::NeedToUpdateInnerProFile()
{
	if (AppGlobalOptionWrapper::Instance()->GetAppMode() == AppGlobalOptionWrapper::Formwork)
		return true;

	//슬래브타입이 아닌경우
	MSRcSlabM* mySlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (mySlabM == nullptr)
		return false;

	long slabMType = mySlabM->GetSlabMType();

	if (slabMType == MSRcSlabM::FLAT_TYPE &&
		!(mySlabM->m_ShortStrip.m_StripType == MSRcStrip::TYPE_B ||
			mySlabM->m_LongStrip.m_StripType == MSRcStrip::TYPE_B))
		return false;//무량판 제외
	else if (slabMType == MSRcSlabM::MAT_TYPE ||
		slabMType == MSRcSlabM::SOG || slabMType == MSRcSlabM::CIRCULAR_RAMP)
		return false; // FlatSlabType

	GM2DLineVector longLx, longLy;
	GetSlabLxLy(longLx, longLy);
	long numLine = mp_Boundary->GetNumLines();
	for (long iVec = 0; iVec < numLine; iVec++)
	{
		double xLoc = longLx.OrthoBaseLocValue(*mp_Boundary->ma_Point[iVec]);
		double yLoc = longLy.OrthoBaseLocValue(*mp_Boundary->ma_Point[iVec]);
		if ((xLoc > 0.25 && xLoc < 0.75) && (yLoc > 0.25 && yLoc < 0.75))
			return false;//주간대 내부에 Point가 있는경우
	}
	
	//내부에 오프닝이 있을경우
	//if (mm_OpeningID.size() > 0)
	//	return false;
	
	return true;
	
	////단차에 의해 분리되었을때
	////내부에 포함되는 슬래브가 있을 경우
	////내부에 피트가 있는 경우

}

long MSSlabG::ResetGetID()
{
	MSCompoG::ResetGetID();
	
	auto TmpWell = mm_Well;
	mm_Well.clear();
	for (auto it = TmpWell.begin(); it != TmpWell.end(); it++)
	{
		it->second->ResetGetID();
		mm_Well.insert(make_pair(it->second->m_ID, it->second));
	}
	
	auto TmpTrench = mm_Trench;
	mm_Trench.clear();
	for (auto it = TmpTrench.begin(); it != TmpTrench.end(); it++)
	{
		it->second->ResetGetID();
		mm_Trench.insert(make_pair(it->second->m_ID, it->second));
	}

	for (int i = 0 ; i < ma_EdgeWall.size() ; i++)
		ma_EdgeWall[i]->ResetGetID();

	for (int i = 0; i < ma_Haunches .size(); i++)
		ma_Haunches [i]->ResetGetID();
	return m_ID;
}

bool MSSlabG::HasPathLine()
{
	if (m_nSlopeMethod == MSSlabG::SlopeArrow_2)
	{
		if (ma_SlopePoints.size() == 2)
			return true;
	}
	return false;
}

GM2DLineVector MSSlabG::GetPathLine()
{
	GM2DLineVector PathLine;
	if (HasPathLine() == false)
		return PathLine;

	GM2DLineVector TheLine(ma_SlopePoints[0], ma_SlopePoints[1]);
	double dDeg = TheLine.GetDegree();
	GM2DPolyline* pProfile = GetProfile();
	GM2DVector CenVec = pProfile->MassCenter();
	PathLine = pProfile->GetLineVectorOnXY(dDeg, CenVec);

	return PathLine;
}

bool MSSlabG::HasHaunch()
{
	if (ma_Haunches .size() > 0)
		return true;
	return false;
}

GM2DPolyline* MSSlabG::GetTrimmedBoundary(bool IsTop)
{
	if (IsTop)
		return MSCompoG::GetTrimmedBoundary(IsTop);

	return mp_BotProfile;
}

void MSSlabG::UpdatePartnerSlab()
{
// 	vector<MSCompoG*> FindSlabGArr = FindConnectMember(MSElement::msSlabG, FIND_CURFLOOR);
// 
// 	for (int i = 0; i < FindSlabGArr.size(); i++)
// 	{
// 		FindSlabGArr[i]->mp_PolylineInfo->TrimExistPolyLineObject(true);
// 		FindSlabGArr[i]->mp_PolylineInfo->TrimExistPolyLineObject(false);
// 
// 		FindSlabGArr[i]->UpdateBoundaryVisibilities();
// 	}
	for (int iConn = 0; iConn < ma_Connections.size(); iConn++)
	{
		MSConnection* pConn = ma_Connections[iConn];
		MSCompoG* pPartnerG = pConn->GetPartner(this);
		if(pPartnerG == nullptr || pPartnerG->GetType() != msSlabG)
			continue;
		MSSlabG* pPartnerSlabG = (MSSlabG*)pPartnerG;
		pPartnerSlabG->mp_PolylineInfo->TrimExistPolyLineObject(true);
		pPartnerSlabG->mp_PolylineInfo->TrimExistPolyLineObject(false);
		pPartnerSlabG->UpdateBoundaryVisibilities();
		MSFloor* pFloor = GetFloor();
		if(pFloor!= nullptr)
			pFloor->CalculateUpdatedCompoGArray(pPartnerSlabG, pFloor->CompoGIDList_NeedRegen_CurTR(), false);
	}
}

bool MSSlabG::HasSubMember()
{
	if (HasHaunch())	return true;
	if (HasLevelDif())	return true;
	if (ma_EdgeWall.size() > 0)	return true;
	if (mm_Trench.size() > 0)	return true;
	if (mm_Well.size() > 0)	return true;

	return MSCompoG::HasSubMember();
}

vector<MSElement*> MSSlabG::GetSubMembers()
{
	vector<MSElement*> subList;
	subList = MSCompoG::GetSubMembers();

	for (int i = 0; i < ma_Haunches.size(); i++)
	{
		subList.push_back(ma_Haunches[i]);
	}

	for (int i = 0; i < ma_LevelDif.size(); i++)
	{
		subList.push_back(ma_LevelDif[i]);
	}

	for (int i = 0; i < ma_EdgeWall.size(); i++)
	{
		subList.push_back(ma_EdgeWall[i]);
	}
		
	return subList;
}

void MSSlabG::DeleteSubMembers()
{
	DeleteWells();
	DeleteTrenchs();
	DeleteEdgeWalls();
	DeleteHaunchs();
	DeleteSlabOpening();
	DeleteSlabLineLevelDifAll();

	MSCompoG::DeleteSubMembers();
}

void MSSlabG::UpdateTrimmedBoundary_AfterCalculate(CON_POS ePos)
{

	MSCompoG::UpdateTrimmedBoundary_AfterCalculate(ePos);

	//AppGlobalOptionWrapper::Instance()->GetTrim3DMode() == 0 인 경우에 호출된다
	// 수직 부재 우선순위 3D트림인 경우

	GM2DPolyline outerProfile(TRUE);
	outerProfile.CopyFrom(GetTrimmedBoundary());
	outerProfile.DeleteInLinePoint(DTOL_MODELING);
	outerProfile.MakeClosedPolyline(DTOL_MODELING);
	double dTol = DTOL_MODELING;

	double dSlabTopLevel = 0, dSlabBotLevel = 0;
	this->GetZLevels(dSlabTopLevel, dSlabBotLevel);

	for (int idx = 0; idx < ma_Connections.size(); idx++)
	{
		MSConnection* pConn = ma_Connections[idx];
		
		if (pConn->m_SetByUser) continue;
		if (pConn->GetSecondCompoGID() == m_ID) continue;

		MSCompoG* pPartnerG = pConn->GetPartner(this);
		// 슬래브와 다른부재의 우선순위는 기본적으로 슬래브가 First로 설정한다
		if (pPartnerG->IsPointMember())
		{
			GM2DPolyline* pPoly = pPartnerG->GetTrimmedBoundary();
			vector<GM2DPolyline*> joined_Polys;
			outerProfile.BoolOperation('A', pPoly, joined_Polys, dTol);
			int PolyCnt = joined_Polys.size();
			GMObject::DeleteGMObject(joined_Polys);
			if (PolyCnt > 1)
				continue;
			
			pConn->SwitchFirstSecond();
		}
		else if (pPartnerG->IsLinkMember())
		{
			double dDegTol = 5;
			MSLinkMemberG* pLinkG = (MSLinkMemberG*)pPartnerG;
			shared_ptr<GM2DCurve> pCurve = pLinkG->GetCenCurve();
			if (outerProfile.HasColinearCurve(&*pCurve, dDegTol, pLinkG->GetWidth() / 2 + DTOL_MODELING * 3) == false)
				continue;

			double dPartnerTopLevel = 0, dPartnerBotLevel = 0;
			pPartnerG->GetZLevels(dPartnerTopLevel, dPartnerBotLevel);
			if ((dSlabTopLevel + DTOL_GM - dPartnerTopLevel > 0) && (dSlabBotLevel - DTOL_GM - dPartnerBotLevel < 0) == true) // 슬래브 ZLevel에 포함되는 경우 
			{
			}
			else
				pConn->SwitchFirstSecond();
		}
		else if (pPartnerG->IsPolyMember())
		{

		}
	}
}

void MSSlabG::CalculateTrimmedBoundary_Acis()
{
	MSCompoG::CalculateTrimmedBoundary_Acis();
	/*
	if (AppGlobalOptionWrapper::Instance()->GetTrim3DMode() == 1)
	{
		MSCompoG::CalculateTrimmedBoundary_Acis();
		return;
	}

	if (mp_TrimmedBoundary_Acis == nullptr)
	{
		double dSlabTopLevel = 0;
		double dSlabBotLevel = 0;
		GetZLevels(dSlabTopLevel, dSlabBotLevel);

		double dTol = 10;
		GM2DPolyline outerProfile(TRUE);
		outerProfile.CopyFrom(GetTrimmedBoundary(true));
		outerProfile.DeleteInLinePoint(DTOL_MODELING);
		outerProfile.MakeClosedPolyline(DTOL_MODELING);

		if (m_IsRigidMember == false)
		{
			// MSConnection 사용 검토
			//vector<MSCompoG*> compoGs = FindConnectMember(FindType_MSElement::msNULL, FIND_ALLFLOOR, dTol, true, true, true);
			vector<MSCompoG*> compoGs = GetPartnerCompoGs();
			for (int i = 0; i < compoGs.size(); i++)
			{
				MSCompoG* compoG = compoGs[i];
				//if (compoG->GetType() != Type_MSCompoG::msBeamG && compoG->IsSteel())
					//continue;

				double dTopLevel = 0;
				double dBotLevel = 0;
				compoG->GetZLevels(dTopLevel, dBotLevel);
				if (dSlabBotLevel > dTopLevel || dSlabTopLevel < dBotLevel)
					continue;

				// 상부에 터치한 객체는 처리하지 않는다.
				if (fabs(dSlabTopLevel - dBotLevel) < DTOL_GM)
					continue;

				// 슬래브에 포함된 객체는 처리 하지 않는다
				if ((dSlabTopLevel + DTOL_GM - dTopLevel > 0) && (dSlabBotLevel - DTOL_GM - dBotLevel < 0) == true)
					continue;

				// 폴리라인과 평행하지 않은 라인부재는 포함하지 않는다.
				if (compoG->IsLinkMember())
				{
					double dDegTol = 5;
					MSLinkMemberG* pLinkG = (MSLinkMemberG*)compoG;
					shared_ptr<GM2DCurve> pCurve = pLinkG->GetCenCurve();
					if (outerProfile.HasColinearCurve(&*pCurve, dDegTol, pLinkG->GetWidth() / 2 + DTOL_MODELING * 3) == false)
						continue;
				}
				else if (compoG->IsPointMember())
				{
					GM2DPolyline* pPoly = compoG->GetTrimmedBoundary();
					vector<GM2DPolyline*> joined_Polys;
					outerProfile.BoolOperation_Base('A', pPoly, joined_Polys, dTol);
					int PolyCnt = joined_Polys.size();

					if (PolyCnt == 1)
						outerProfile.CopyFrom(joined_Polys[0]);

					GMObject::DeleteGMObject(joined_Polys);
				}

				if (compoG->GetType() == msBeamG || compoG->GetType() == msWallG || compoG->GetType() == msColumnG || compoG->GetType() == msWallColumnG)
				{
					vector<GM2DPolyline*> JoinedPolys;
					outerProfile.BoolOperation_Base('A', compoG->GetTrimmedBoundary(), JoinedPolys, dTol);

					int polyCount = JoinedPolys.size();
					if (polyCount == 1)
						outerProfile.CopyFrom(JoinedPolys[0]);

					GMObject::DeleteGMObject(JoinedPolys);
				}
			}
		}

		outerProfile.DeleteInLinePoint(DTOL_MODELING);
		outerProfile.MakeClosedPolyline(DTOL_MODELING);
		SetTrimmedBoundary_Acis(&outerProfile);
	}
	*/
}

bool MSSlabG::IsFWSlopeRectangleSlab()
{
	MSRcFWSlabM* pRcFWSlabM = dynamic_cast<MSRcFWSlabM*>(GetCompoM());
	GM2DPolyline* pPoly = GetProfile();
	if (pRcFWSlabM == nullptr || pPoly->IsRectangle() == false)
		return false;

	if (pRcFWSlabM->GetFWSlabType() == MSRcFWSlabM::TopSlope || pRcFWSlabM->GetFWSlabType() == MSRcFWSlabM::BottomSlope)
		return true;

	return false;
}

GM2DCurve* MSSlabG::GetFWSlopeDirectionLine()
{
	if (IsFWSlopeRectangleSlab() == false)
		return nullptr;

	GM2DPolyline* pPoly = GetProfile();
	GM2DCurve* pBaseCurve = pPoly->GetCurve(0);
	GM2DVector MidPnt = pBaseCurve->PointAtParam(0.5);
	GM2DVector CenPnt = pPoly->MassCenter();
	GM2DVector transtlateVec = CenPnt - MidPnt;
	pBaseCurve->Translate(transtlateVec);

	return pBaseCurve;
}

GM2DPolyline* MSSlabG::GetFWSlopeSectionProfile()
{
	if (IsFWSlopeRectangleSlab() == false)
		return nullptr;

	MSRcFWSlabM* pRcFWSlabM = dynamic_cast<MSRcFWSlabM*>(GetCompoM());
	GM2DPolyline* pPoly = GetProfile();
	GM2DCurve* pBaseCurve = pPoly->GetCurve(0);
	double dLen = pBaseCurve->Length();

	GM2DPolyline* pSectionProfile = new GM2DPolyline();
	pRcFWSlabM->MakeProfile(pSectionProfile, dLen);
	delete pBaseCurve;
	

	return pSectionProfile;
}

GM2DCurve* MSSlabG::GetFWSlopeBaseLine()
{
	if (IsFWSlopeRectangleSlab() == false)
		return nullptr;

	GM2DPolyline* pPoly = GetProfile();
	GM2DCurve* pBaseCurve = pPoly->GetCurve(3);
	return pBaseCurve;
}

vector<GM2DCurve*> MSSlabG::GetSupportCurve()
{
	vector<GM2DCurve*> CurCurveArr;
	if (GetContainingElementId() != 0) // [BH-3288] 완전포함된 슬래브의 도리근라인은 없어야한다.
	{
		MSElement *pContainingElement = GetContainingElement();
		if(pContainingElement == nullptr)
			return CurCurveArr;
		Type_MSCompoG compoGType = pContainingElement->GetType();
		if(compoGType != Type_MSCompoG::msSlabG)
			return CurCurveArr;
		
		double dMySlabTopLevel = 0, dMySlabBotLevel = 0, dContainingSlabTopLevel = 0, dContainingSlabBotLevel = 0;
		GetZLevels(dMySlabTopLevel, dMySlabBotLevel);

		MSSlabG *pContainingSlab = (MSSlabG*)pContainingElement;		
		pContainingSlab->GetZLevels(dContainingSlabTopLevel, dContainingSlabBotLevel);
		if(fabs(dMySlabBotLevel - dContainingSlabBotLevel) < DTOL5)
			return CurCurveArr;
	}

	vector<MSCompoG*> SupportSlabGArr = FindConnectMember(FindType_MSElement::msSlabG, FIND_CURFLOOR, DTOL_GM, true, true, true);
	GM2DPolyline* pCurPoly = new GM2DPolyline();
	pCurPoly->CopyFrom(GetTrimmedBoundary_Acis());

	for (int iSlab = 0; iSlab < SupportSlabGArr.size(); iSlab++)
	{
		GM2DPolyline* pUrPoly = SupportSlabGArr[iSlab]->GetTrimmedBoundary_Acis();
		pCurPoly->SegmentstoMe(pUrPoly);
	}
	
	pCurPoly->GetSegments(CurCurveArr);

	for (int iSlab = 0; iSlab < SupportSlabGArr.size(); iSlab++)
	{
		GM2DPolyline* pUrPoly = SupportSlabGArr[iSlab]->GetTrimmedBoundary_Acis();
	
		for (int iCurve = CurCurveArr.size() - 1; iCurve >= 0; iCurve--)
		{
 			GM2DCurve* pCurCurve = CurCurveArr[iCurve];
			GM2DCurve* pOnLineCurve = pUrPoly->GetOnLineCurve(pCurCurve, DTOL_GM);
			if (pOnLineCurve != nullptr)
			{
				CurCurveArr.erase(CurCurveArr.begin() + iCurve);
				delete pOnLineCurve;
				delete pCurCurve;
			}
		}
	}
	return CurCurveArr;
}

void MSSlabG::SetSubMemberID()
{
	for (map<long, MSWell*>::iterator it = mm_Well.begin(); it != mm_Well.end(); it++)
		it->second->GetID();

	for (map<long, MSTrench*>::iterator it = mm_Trench.begin(); it != mm_Trench.end(); it++)
		it->second->GetID();

	for (int i = 0; i < ma_EdgeWall.size(); ++i)
		ma_EdgeWall[i]->GetID();

	for (int i = 0; i < ma_Haunches.size(); i++)
		ma_Haunches[i]->GetID();

	for (int i = 0; i < mp_PolylineInfo->ma_Opening.size(); i++)
		mp_PolylineInfo->ma_Opening[i]->GetID();

	for (int i = 0 ; i< ma_LevelDif.size(); i++)
		ma_LevelDif[i]->GetID();
}

#pragma endregion


bool MSSlabG::RemoveSubMember(MSElement* element)
{
	bool rVal = false;
	switch (element->GetType())
	{
	case MSElement::msTrench:	
		return Remove((MSTrench*)element);
	case MSElement::msWell:
		return Remove((MSWell*)element);
	case MSElement::msSubHaunchG:
		rVal = Remove((MSSubHaunchG*)element);
		if (rVal == true)
			RegenHaunch();
		return rVal;
	case MSElement::msLineLevelDifG:
		rVal = Remove((MSLineLevelDifG*)element);
		if (rVal == true)
			TrimLevelDifAll();
		return rVal;
	}
	return MSCompoG::RemoveSubMember(element);
}

vector<MSCompoG*> MSSlabG::GetSoundAbsorptionGArr()
{
	return GetLinkedCompoGArr();
}

bool MSSlabG::IsSameGeometry(MSCompoG* urCompoG, double distTolerance, double degreeTolerance)
{
	//무조건 same만 체크
	GM2DPolyline* myPoly = GetProfile();
	GM2DPolyline* urPoly = urCompoG->GetProfile();
	if (!myPoly->SamePolyLine(urPoly))
		return false;

	return true;
}

bool MSSlabG::UsedMaterial(long materialID)
{
	for (int i = 0; i < ma_LevelDif.size(); i++)
	{
		long nHaunchMaterialID = ma_LevelDif[i]->GetHaunchMaterialID();
		if (nHaunchMaterialID == materialID)
			return true;
	}

	for (int i = 0; i < ma_Haunches.size(); i++)
	{
		long nHaunchMaterialID = ma_Haunches[i]->GetHaunchMaterialID();
		if (nHaunchMaterialID == materialID)
			return true;
	}

	for (int i = 0; i < ma_EdgeWall.size(); i++)
	{
		long nHaunchMaterialID = ma_EdgeWall[i]->GetHaunchMaterialID();
		if (nHaunchMaterialID == materialID)
			return true;
	}

	return MSCompoG::UsedMaterial(materialID);
}

void MSSlabG::SubMemberMaterialValidCheck()
{
	MSBaseBuilding* pBldg = GetBuilding();
	if (pBldg == nullptr) return;

	long defMaterialID = -1;

	for (int i = 0; i < ma_LevelDif.size(); i++)
	{
		if (ma_LevelDif[i]->GetIsUsedStoryMaterial() == true) continue;

		long nHaunchMaterialID = ma_LevelDif[i]->GetHaunchMaterialID();
		if (pBldg->GetMaterial(nHaunchMaterialID) != nullptr) continue;

		if (defMaterialID == -1)
		{
			MSMaterial* defMaterial = pBldg->GetDefMaterial();
			if (defMaterial == nullptr) return;
			defMaterialID = defMaterial->m_ID;
		}

		ma_LevelDif[i]->SetHaunchMaterialID(defMaterialID);
	}

	for (int i = 0; i < ma_Haunches.size(); i++)
	{
		if (ma_Haunches[i]->GetIsUsedStoryMaterial() == true) continue;

		long nHaunchMaterialID = ma_Haunches[i]->GetHaunchMaterialID();
		if (pBldg->GetMaterial(nHaunchMaterialID) != nullptr) continue;

		if (defMaterialID == -1)
		{
			MSMaterial* defMaterial = pBldg->GetDefMaterial();
			if (defMaterial == nullptr) return;
			defMaterialID = defMaterial->m_ID;
		}

		ma_Haunches[i]->SetHaunchMaterialID(defMaterialID);
	}

	for (int i = 0; i < ma_EdgeWall.size(); i++)
	{
		if (ma_EdgeWall[i]->GetIsUsedStoryMaterial() == true) continue;

		long nHaunchMaterialID = ma_EdgeWall[i]->GetHaunchMaterialID();
		if (pBldg->GetMaterial(nHaunchMaterialID) != nullptr) continue;

		if (defMaterialID == -1)
		{
			MSMaterial* defMaterial = pBldg->GetDefMaterial();
			if (defMaterial == nullptr) return;
			defMaterialID = defMaterial->m_ID;
		}

		ma_EdgeWall[i]->SetHaunchMaterialID(defMaterialID);
	}
}

bool MSSlabG::GetIsHasRib()
{
	if(IsPCSlabGroup())
		return m_IsHasRib; 
	return false;
}
void MSSlabG::SetIsHasRib(bool hasRib)
{
	if (IsPCSlabGroup())
		m_IsHasRib = hasRib;
}
bool MSSlabG::GetIsRibXDirection() 
{
	return m_IsRibXDirection;
}
void MSSlabG::SetIsRibXDirection(bool isRibXDirection)
{
	UpdateLxDegWithRibDir(m_IsRibXDirection, isRibXDirection);
	m_IsRibXDirection = isRibXDirection;
}

vector<GM2DPolyline> MSSlabG::GetRibPolylineList()
{
	vector<GM2DPolyline> ribPolylines;

	if (IsIMTPCSlab() == false)
		return ribPolylines;

	GM2DPolyline* outline = GetProfile();
	if (outline->IsRectangle() == false || outline->ma_Point.GetSize() != 5)
		return ribPolylines;
	MSRcFaceMemberM *pRcFaceMemberM = dynamic_cast<MSRcFaceMemberM*>(GetCompoM());
	if (pRcFaceMemberM == nullptr)
		return ribPolylines;
	MSRcSlabM* rcSlab = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (rcSlab == nullptr)
		return ribPolylines;

	double imt_L1 = pRcFaceMemberM->GetIMT_L1();
	double imt_L2 = pRcFaceMemberM->GetIMT_L2();
	double imt_L3 = pRcFaceMemberM->GetIMT_L3();

	// x, y방향 계산
	GM2DLineVector Lnx, Lny;
	GetLnxLny(Lnx, Lny);

	double ribOrthoLength = Lny.Length();

	vector<double> ribPos;
	double currPos = imt_L1;
	while (currPos + imt_L2 + imt_L3 < ribOrthoLength - (imt_L1 + imt_L2) + DTOL_GM)
	{
		ribPos.push_back(currPos);
		currPos += imt_L2 + imt_L3;
	}
	// 마지막 Rib 추가
	if (2 * imt_L1 + 2 * imt_L2 + imt_L3 < ribOrthoLength)
		ribPos.push_back(ribOrthoLength - imt_L1 - imt_L2);

	// Rib의 프로파일을 생성한다.
	for (int i = 0; i < ribPos.size(); i++)
	{
		GM2DVector ribP0;
		GM2DVector ribP1;
		GM2DVector ribP2;
		GM2DVector ribP3;

		ribP0 = Lny.PointAtParam(ribPos[i] / ribOrthoLength);
		ribP1 = ribP0 + Lnx;

		ribP3 = Lny.PointAtParam((ribPos[i] + imt_L2) / ribOrthoLength);
		ribP2 = ribP3 + Lnx;

		GM2DPolyline ribLine;
		ribLine.Add(new GM2DVector(ribP0));
		ribLine.Add(new GM2DVector(ribP1));
		ribLine.Add(new GM2DVector(ribP2));
		ribLine.Add(new GM2DVector(ribP3));
		ribLine.MakeClosedPolyline(DTOL_GM);

		if (ribLine.Area() > DTOL5)
			ribPolylines.push_back(ribLine);
	}

	return ribPolylines;
}

vector<GM2DPolyline> MSSlabG::GetGeneralDeckRibPolylineList()
{
	vector<GM2DPolyline> ribPolylines;

	if (IsGeneralDeckSlab() == false)
		return ribPolylines;

	GM2DPolyline* outline = GetProfile();
	if (outline->IsRectangle() == false || outline->ma_Point.GetSize() != 5) return ribPolylines;

	auto compoM = dynamic_cast<MSRcFaceMemberM*>(mp_CompoM);

	// x, y방향 계산
	GM2DPolyline slabProfile(TRUE);
	GetBoundary(slabProfile);
	GM2DLineVector longLx, longLy;
	CalcLongXLongY(slabProfile, longLx, longLy);

	double ribOrthoLength = longLy.Length();

	double topWidth = compoM->GetDeckFormTopWidth();
	double botWidth = compoM->GetDeckFormBotWidth();
	double height = compoM->GetDeckFormThick();
	double offset = compoM->GetDeckFormSpace();

	vector<double> ribPos;
	double currPos = 0;
	while (currPos + topWidth <= ribOrthoLength + DTOL_GM)
	{
		ribPos.push_back(currPos);
		currPos += offset;
	}
	
	// Rib의 프로파일을 생성한다.
	for (int i = 0; i < ribPos.size(); i++)
	{
		GM2DVector ribP0;
		GM2DVector ribP1;
		GM2DVector ribP2;
		GM2DVector ribP3;

		ribP0.m_X = ribPos[i];
		ribP0.m_Y = 0;
		ribP1.m_X = (ribPos[i] + (topWidth - botWidth) * 0.5);
		ribP1.m_Y = -height;
		ribP2.m_X = (ribPos[i] + (topWidth - botWidth) * 0.5 + botWidth);
		ribP2.m_Y = -height;
		ribP3.m_X = (ribPos[i] + topWidth);
		ribP3.m_Y = 0;

		GM2DPolyline ribLine;
		ribLine.Add(new GM2DVector(ribP0));
		ribLine.Add(new GM2DVector(ribP1));
		ribLine.Add(new GM2DVector(ribP2));
		ribLine.Add(new GM2DVector(ribP3));
		ribLine.MakeClosedPolyline(DTOL_GM);
		
		if (ribLine.Area() > DTOL5)
			ribPolylines.push_back(ribLine);
	}

	return ribPolylines;
}



CString MSSlabG::ToString()
{
	auto str = MSCompoG::ToString();
	str.AppendFormat(L"Profile:%s, OffsetZ:%f, ", GetProfile()->ToString(), m_dOffset);
	return str;
}

vector<GM2DLineVector> MSSlabG::GetStartHoleLines(bool onlyFillHole, bool isPlaceRebar)
{
	return GetHoleLines(true, onlyFillHole, isPlaceRebar);
}

vector<GM2DLineVector> MSSlabG::GetEndHoleLines(bool onlyFillHole, bool isPlaceRebar)
{
	return GetHoleLines(false, onlyFillHole, isPlaceRebar);
}

vector<double> MSSlabG::GetHoleLineLocs()
{
	// 배근전용 함수
	vector<double> barLocArr;

	MSRcFaceMemberM* pMSRcFaceMemberM = dynamic_cast<MSRcFaceMemberM*>(GetCompoM());
	if (pMSRcFaceMemberM == nullptr)
		return barLocArr;

	MSRcSlabM::SLAB_TYPE slabMType = (MSRcSlabM::SLAB_TYPE)pMSRcFaceMemberM->GetSlabMType();
	if (slabMType != MSRcSlabM::PC_HCS)
		return barLocArr;

	GM2DLineVector Lnx, Lny;
	GetLnxLny(Lnx, Lny);
	vector<GM2DLineVector> holeLines = GetHoleLines(true, true, true);
	barLocArr.resize(holeLines.size());
	std::transform(holeLines.begin(), holeLines.end(), barLocArr.begin(), [&](GM2DLineVector lineVec)->double { return Lny.OrthoBaseLocValue(lineVec.m_P1); });
	
	return barLocArr;
}

vector<double> MSSlabG::GetRPSLxBotSpliceBarLocs(long numBar, double dSideCover)
{
	// 배근전용 함수
	vector<double> barLocArr;
	if(numBar <= 0)
		return barLocArr;
	MSRcFaceMemberM* pMSRcFaceMemberM = dynamic_cast<MSRcFaceMemberM*>(GetCompoM());
	if (pMSRcFaceMemberM == nullptr)
		return barLocArr;

	MSRcSlabM::SLAB_TYPE slabMType = (MSRcSlabM::SLAB_TYPE)pMSRcFaceMemberM->GetSlabMType();
	if (slabMType != MSRcSlabM::PC_RPS)
		return barLocArr;

	GM2DLineVector Lnx, Lny;
	GetLnxLny(Lnx, Lny);
	auto holeEndLines = GetStartHoleEndLines();
	for(int iLine = 0 ; iLine < holeEndLines.size() ; iLine++)
	{
		auto curLine = holeEndLines[iLine];
		curLine.ExtendLineVector(TRUE, dSideCover, TRUE);
		curLine.ExtendLineVector(FALSE, dSideCover, FALSE);
		if(numBar == 1)
			barLocArr.push_back(Lny.OrthoBaseLocValue(curLine.MassCenter()));
		else
		{
			double dRatio = 1.0/(numBar-1);
			double dCurLoc = 0;
			for(int jLoc = 0 ; jLoc < numBar ; jLoc++, dCurLoc+=dRatio)
				barLocArr.push_back(Lny.OrthoBaseLocValue(curLine.PointVector(dCurLoc)));
		}
	}

	return barLocArr;
}
vector<GM2DLineVector> MSSlabG::GetHoleLines(bool isStart, bool onlyFillHole, bool isPlaceRebar)
{
	vector<GM2DLineVector> holeLineArr;

	MSRcFaceMemberM* pMSRcFaceMemberM = dynamic_cast<MSRcFaceMemberM*>(GetCompoM());
	if (pMSRcFaceMemberM == nullptr)
		return holeLineArr;

	GM2DLineVector Lnx, Lny;
	GetLnxLny(Lnx, Lny);
	if (isPlaceRebar)
	{
		GM2DLineVector Lx, Ly;
		GetLxLy(Lx, Ly);

		GM2DVector cenVec = Lnx.MassCenter();
		// 1 : 진행방향 왼쪽, 0 : 선상, -1, 진행방향 오른쪽
		long nSide = Lx.WhichSide(cenVec);
		double transDist = Lx.Distance(cenVec);
		if (nSide == -1)
			transDist *= -1;
		Lnx = Lx.NormalTranslate(transDist);
	}

	double dSyLen = Lny.Length();
	double dHoleDia = 140;
	double lastHoleDia = 140;
	double dEndDist = 400;
	int holeNum = 5;
	double dHoleLineLen = pMSRcFaceMemberM->GetFillHoleLength();

	vector<double> distArr;

	MSRcSlabM::SLAB_TYPE slabMType = (MSRcSlabM::SLAB_TYPE)pMSRcFaceMemberM->GetSlabMType();
	if (slabMType == MSRcSlabM::PC_HCS)
	{
		dHoleDia = pMSRcFaceMemberM->GetHCS_L1();
		dEndDist = pMSRcFaceMemberM->GetHCS_L2();
		holeNum = pMSRcFaceMemberM->GetHoleNum();
		lastHoleDia = dHoleDia;

		double holeSpace = (dSyLen) / 2;
		if (holeNum > 1)
			holeSpace = (dSyLen - (dEndDist * 2)) / (holeNum - 1);

		if(holeNum == 1)
			distArr.push_back(holeSpace);
		else
		{
			distArr.push_back(dEndDist);
			for (int i = 0; i < holeNum - 1; i++)
				distArr.push_back(holeSpace);
		}
	}
	else if (slabMType == MSRcSlabM::PC_RPS)
	{
		dHoleDia = pMSRcFaceMemberM->GetRPS_L2();
		dEndDist = pMSRcFaceMemberM->GetRPS_L1();
		double dRibWidth = pMSRcFaceMemberM->GetRPS_L3();
		lastHoleDia = GetLastHoleDia();

		holeNum = (dSyLen - dEndDist - dEndDist - lastHoleDia) / (dHoleDia + dRibWidth);
		double holeSpace = dHoleDia + dRibWidth;
		distArr.push_back(dEndDist + dHoleDia/2);
		for (int i = 0; i < holeNum - 1; i++)
			distArr.push_back(holeSpace);
		distArr.push_back(dHoleDia / 2 + dRibWidth + lastHoleDia / 2);
	}
	else
		return holeLineArr;

	if(holeNum == 0)
		return holeLineArr;

	
	GM2DLineVector baseLine;
	if (isStart)
	{
		baseLine.m_P0 = Lnx.m_P0;
		baseLine.m_P1 = baseLine.m_P0 + Lnx.UnitVector() * dHoleLineLen;
	}
	else
	{
		baseLine.m_P0 = Lnx.m_P1;
		baseLine.m_P1 = baseLine.m_P0 + Lnx.UnitVector() * -dHoleLineLen;
	}


	vector<bool> fillHoles = GetIsFillHoles();
	double curDist = 0;
	for (int i = 0; i < distArr.size(); i++)
	{
		curDist += distArr[i];
		if(onlyFillHole && !fillHoles[i])
			continue;
		GM2DLineVector holeLine = baseLine.NormalTranslate(curDist, Lny.m_P1);
		holeLineArr.push_back(holeLine);
	}

	return holeLineArr;
}

vector<GM2DLineVector> MSSlabG::GetStartHoleEndLines()
{
	return GetHoleEndLines(true);
}
vector<GM2DLineVector> MSSlabG::GetEndHoleEndLines()
{
	return GetHoleEndLines(false);
}

vector<GM2DLineVector> MSSlabG::GetHoleEndLines(bool isStart)
{
	vector<GM2DLineVector> holeEndLineArr;

	MSRcFaceMemberM* pMSRcFaceMemberM = dynamic_cast<MSRcFaceMemberM*>(GetCompoM());
	if (pMSRcFaceMemberM == nullptr)
		return holeEndLineArr;

	double dHoleDia = 140;
	MSRcSlabM::SLAB_TYPE slabMType = (MSRcSlabM::SLAB_TYPE)pMSRcFaceMemberM->GetSlabMType();
	if (slabMType == MSRcSlabM::PC_HCS)
		dHoleDia = pMSRcFaceMemberM->GetHCS_L1();
	else if (slabMType == MSRcSlabM::PC_RPS)
		dHoleDia = pMSRcFaceMemberM->GetRPS_L2();
	else
		return holeEndLineArr;
	
	double dLastHoleDia = GetLastHoleDia();

	double dHalfHoleDia = dHoleDia / 2;
	double dHalfLastHoleDia = dLastHoleDia / 2;

	double dAddDeg = 90;
	if (isStart)
		dAddDeg = -90;

	vector<GM2DLineVector> holeLineArr = GetHoleLines(isStart, false);
	for (int idx = 0; idx < holeLineArr.size(); idx++)
	{
		double transDist = dHalfHoleDia;
		if(slabMType == MSRcSlabM::PC_RPS && idx == holeLineArr.size() -1)
			transDist = dHalfLastHoleDia;

		GM2DLineVector curLine = holeLineArr[idx];
		double dDeg = curLine.GetDegree();

		GM2DLineVector endLine;
		endLine.m_P0 = curLine.m_P0.NormalTranslate(transDist, dDeg + dAddDeg);
		endLine.m_P1 = curLine.m_P0.NormalTranslate(transDist, dDeg - dAddDeg);
		holeEndLineArr.push_back(endLine);
	}

	return holeEndLineArr;
}


vector<bool> MSSlabG::GetIsFillHoles()
{
	vector<bool> isFillHoleArr;
	MSRcFaceMemberM* pMSRcFaceMemberM = dynamic_cast<MSRcFaceMemberM*>(GetCompoM());
	if (pMSRcFaceMemberM == nullptr)
		return isFillHoleArr;

	int holeNum = pMSRcFaceMemberM->GetHoleNum();
	int fillHoleNum = pMSRcFaceMemberM->GetFillHoleNum();

	if (holeNum == 0)
		return isFillHoleArr;

	if (holeNum < fillHoleNum)
		fillHoleNum = holeNum;

	if (fillHoleNum == 1)
	{
		if (holeNum == 1)
			isFillHoleArr.push_back(true);
		else
		{
			for (int i = 0; i < holeNum; i++)
				isFillHoleArr.push_back(false);
			isFillHoleArr[holeNum / 2] = true;
		}

		return isFillHoleArr;
	}
	else if (fillHoleNum == 0)
	{
		for (int i = 0; i < holeNum; i++)
			isFillHoleArr.push_back(false);
		return isFillHoleArr;
	}

	for (int i = 0; i < holeNum; i++)
		isFillHoleArr.push_back(true);

	int emptyCnt = holeNum - fillHoleNum;
	int firstIdx = 0;
	int lastIdx = holeNum - 1;
	int curEmptyCnt = 0;

	int cnt1 = 0;
	int cnt2 = 0;
	while (emptyCnt != cnt1 + cnt2)
	{
		int val = curEmptyCnt % 2;
		if (val == 1 && firstIdx <= holeNum - 1)
		{
			if (isFillHoleArr[firstIdx] == false)
				firstIdx--;

			if (isFillHoleArr[firstIdx] == true)
			{
				isFillHoleArr[firstIdx] = false;
				cnt1++;
			}

			firstIdx = firstIdx + 2;
		}
		else if (val == 0 && lastIdx >= 0)
		{
			if (isFillHoleArr[lastIdx] == false)
				lastIdx--;

			if (isFillHoleArr[lastIdx] == true)
			{
				isFillHoleArr[lastIdx] = false;
				cnt2++;
			}

			lastIdx = lastIdx - 2;

		}
		else if(firstIdx > holeNum - 1 && lastIdx < 0)
			break;

		curEmptyCnt++;
	}

	return isFillHoleArr;
}

double MSSlabG::GetLastHoleDia()
{
	double dHoleDia = 140;
	MSRcFaceMemberM* pMSRcFaceMemberM = dynamic_cast<MSRcFaceMemberM*>(GetCompoM());
	if (pMSRcFaceMemberM == nullptr)
		return dHoleDia;
	
	MSRcSlabM::SLAB_TYPE slabMType = (MSRcSlabM::SLAB_TYPE)pMSRcFaceMemberM->GetSlabMType();
	if (slabMType == MSRcSlabM::PC_HCS)
	{
		return pMSRcFaceMemberM->GetHCS_L1();
	}
	else if (slabMType == MSRcSlabM::PC_RPS)
	{
		GM2DLineVector Lnx, Lny;
		GetLnxLny(Lnx, Lny);

		double dLnyLen = Lny.Length();
		double dHoleDia = pMSRcFaceMemberM->GetRPS_L2();
		double dEndDist = pMSRcFaceMemberM->GetRPS_L1();
		double dRibWidth = pMSRcFaceMemberM->GetRPS_L3();

		int cnt = (dLnyLen - dEndDist - dEndDist) / (dHoleDia + dRibWidth);
		double lastHoleDia = (dLnyLen - dEndDist - dEndDist) - ((dHoleDia + dRibWidth) * cnt);
		if (lastHoleDia < 50) // 홀의 최소 크기
		{
			cnt--;
			lastHoleDia = (dLnyLen - dEndDist - dEndDist) - ((dHoleDia + dRibWidth) * cnt);
		}

		return lastHoleDia;

// 		int holeNum = 0;
// 		double dCurDist = dEndDist;
// 		while (dCurDist < dSyLen - dEndDist - dRibWidth)
// 		{
// 			dCurDist += dHoleDia;
// 			dCurDist += dRibWidth;
// 			holeNum++;
// 		}
// 
// 		if (holeNum > 0)
// 			dCurDist -= dRibWidth;
// 
// 		double lastHoleDia = dSyLen - dCurDist - dEndDist;
// 		if (lastHoleDia < dHoleDia)
// 			lastHoleDia = lastHoleDia + dHoleDia;
// 	
// 		return lastHoleDia;
	}

	return 0;
}

double MSSlabG::GetHoleLineLength()
{
	// 물량 계산시 사용
	MSRcFaceMemberM* pMSRcFaceMemberM = dynamic_cast<MSRcFaceMemberM*>(GetCompoM());
	if (pMSRcFaceMemberM == nullptr)
		return 0;

	MSRcSlabM::SLAB_TYPE slabMType = (MSRcSlabM::SLAB_TYPE)pMSRcFaceMemberM->GetSlabMType();
	if (slabMType == MSRcSlabM::PC_RPS || slabMType == MSRcSlabM::PC_HCS)
	{
		double dHoleLineLen = pMSRcFaceMemberM->GetFillHoleLength();
		GM2DLineVector Lnx, Lny;
		GetLnxLny(Lnx, Lny);

		double dLongLnxlen = Lnx.Length();

		if (dLongLnxlen > dHoleLineLen * 2)
			return dHoleLineLen;

		return dLongLnxlen / 2;
	}

	return 0;
}

vector<GM2DPolyline> MSSlabG::GetStartRPSRibLines()
{
	return GetRPSRibLines(true);
}

vector<GM2DPolyline> MSSlabG::GetEndRPSRibLines()
{
	return GetRPSRibLines(false);
}

vector<GM2DPolyline> MSSlabG::GetRPSRibLines(bool isStart)
{
	vector<GM2DPolyline> ribLineArr;

	MSRcFaceMemberM* pMSRcFaceMemberM = dynamic_cast<MSRcFaceMemberM*>(GetCompoM());
	if (pMSRcFaceMemberM == nullptr)
		return ribLineArr;

	double dHoleDia = 140;
	double dEndDist = 400;
	double dRibWidth = 100;

	MSRcSlabM::SLAB_TYPE slabMType = (MSRcSlabM::SLAB_TYPE)pMSRcFaceMemberM->GetSlabMType();
	if (slabMType == MSRcSlabM::PC_RPS)
	{
		dHoleDia = pMSRcFaceMemberM->GetRPS_L2();
		dEndDist = pMSRcFaceMemberM->GetRPS_L1();
		dRibWidth = pMSRcFaceMemberM->GetRPS_L3();
	}
	else
		return ribLineArr;

	GM2DLineVector Lnx, Lny;
	GetLnxLny(Lnx, Lny);

	double dSnyLen = Lny.Length();
	double dHoleLineLen = pMSRcFaceMemberM->GetFillHoleLength();
	double holeSpace = dHoleDia + (dRibWidth / 2);

	int ribNum = 2;
	if (holeSpace > 0)
		ribNum = ((dSnyLen - dEndDist) / holeSpace) + 2;

	GM2DLineVector baseLine;
	if (isStart)
	{
		baseLine.m_P0 = Lnx.m_P0;
		baseLine.m_P1 = baseLine.m_P0 + Lnx.UnitVector() * dHoleLineLen;
	}
	else
	{
		baseLine.m_P0 = Lnx.m_P1;
		baseLine.m_P1 = baseLine.m_P0 + Lnx.UnitVector() * -dHoleLineLen;
	}

	double curDist = 0;
	while(curDist < dSnyLen)
	{
		double dCurRibWidth = 0;
		if (curDist == 0)
			dCurRibWidth = dEndDist;
		else
		{
			dCurRibWidth = dRibWidth;
			curDist += dHoleDia;
		}

		if (curDist + dCurRibWidth > dSnyLen - dEndDist - DTOL_GM)
		{
			curDist = dSnyLen - dEndDist;
			dCurRibWidth = dEndDist;
		}

		GM2DLineVector ribLine = baseLine.NormalTranslate(curDist, Lny.m_P1);
		GM2DLineVector ribLine2 = baseLine.NormalTranslate(curDist + dCurRibWidth, Lny.m_P1);

		GM2DPolyline ribPoly;
		ribPoly.AddCopy(ribLine.m_P0);
		ribPoly.ma_Bulge.Add(0);
		ribPoly.AddCopy(ribLine.m_P1);
		ribPoly.ma_Bulge.Add(0);
		ribPoly.AddCopy(ribLine2.m_P1);
		ribPoly.ma_Bulge.Add(0);
		ribPoly.AddCopy(ribLine2.m_P0);
		ribPoly.ma_Bulge.Add(0);
		ribPoly.AddCopy(ribLine.m_P0);
		ribLineArr.push_back(ribPoly);

		curDist += dCurRibWidth;
	}

// 	double curDist = 0;
// 	for (int i = 0; i < ribNum; i++)
// 	{
// 		double dCurRibWidth = 0;
// 		if (i == 0 )
// 			dCurRibWidth = dEndDist;
// 		else if (i == ribNum - 1)
// 		{
// 			curDist = dSyLen - dEndDist;
// 			dCurRibWidth = dEndDist;
// 		}
// 		else
// 			dCurRibWidth = dRibWidth;
// 
// 		if(curDist + dCurRibWidth > dSyLen) continue;
// 
// 		GM2DLineVector ribLine = baseLine.NormalTranslate(curDist, longSy.m_P1);
// 		GM2DLineVector ribLine2 = baseLine.NormalTranslate(curDist + dCurRibWidth, longSy.m_P1);
// 
// 		GM2DPolyline ribPoly;
// 		ribPoly.AddCopy(ribLine.m_P0);
// 		ribPoly.ma_Bulge.Add(0);
// 		ribPoly.AddCopy(ribLine.m_P1);
// 		ribPoly.ma_Bulge.Add(0);
// 		ribPoly.AddCopy(ribLine2.m_P1);
// 		ribPoly.ma_Bulge.Add(0);
// 		ribPoly.AddCopy(ribLine2.m_P0);
// 		ribPoly.ma_Bulge.Add(0);
// 		ribPoly.AddCopy(ribLine.m_P0);
// 		ribLineArr.push_back(ribPoly);
// 
// 		curDist += dCurRibWidth + dHoleDia;
// 	}

	return ribLineArr;
}

void MSSlabG::MakeCCW()
{
	GM2DPolyline* trimmedBoundary_Top = GetTrimmedBoundary(true);// TopTrimmedProfile
	GM2DPolyline* trimmedBoundary_Bot = GetTrimmedBoundary(false);// BotTrimmedProfile
	GM2DPolyline* trimmedBoundary_Acis = GetTrimmedBoundary_Acis();
	GM2DPolyline* polylineInfoProfile = mp_PolylineInfo->GetProfile();

	if (mp_InnerProFile != nullptr)
		mp_InnerProFile->MakeCCW();
	if (mp_InnerProFile_Bot != nullptr)
		mp_InnerProFile_Bot->MakeCCW();
	if (mp_DeckSlabQTProFile != nullptr)
		mp_DeckSlabQTProFile->MakeCCW();
	if (mp_LxLyProFile != nullptr)
		mp_LxLyProFile->MakeCCW();
	if (polylineInfoProfile != nullptr)
		polylineInfoProfile->MakeCCW();
	if (mp_InnerProFile != nullptr)
		mp_InnerProFile->MakeCCW();

	if (trimmedBoundary_Top != nullptr)
		trimmedBoundary_Top->MakeCCW();
	if (trimmedBoundary_Bot != nullptr)
		trimmedBoundary_Bot->MakeCCW();
	if (trimmedBoundary_Acis != nullptr)
		trimmedBoundary_Acis->MakeCCW();

	if (mp_Boundary != nullptr)
		mp_Boundary->MakeCCW();
}

void MSSlabG::CheckHaunches()
{
	for (int iHaunch = ma_Haunches.size() - 1; iHaunch >= 0; iHaunch--)
	{
		MSSubHaunchG* pHaunch = ma_Haunches[iHaunch];
		GM2DCurve* pCurve = pHaunch->GetCurve();

		double dLen = pCurve->Length();
		if (dLen < DTOL_MODELING)
		{
			ma_Haunches.erase(ma_Haunches.begin() + iHaunch);
			delete pHaunch;
			continue;
		}
	}
}

bool MSSlabG::IsInValidSlopeInfo()
{
	bool isInValidSlope = false;
	// 경사 정보를 검토하여 이상하면 경사정보를 삭제한다
	bool isSlope = IsSlopeMember();
	if (isSlope)
	{
		GM2DPolyline* pTrimmedBoundary_Acis = GetTrimmedBoundary_Acis();
		int numPnt = pTrimmedBoundary_Acis->ma_Point.GetSize();

		if (m_nSlopeMethod == SlopeMethod::PointOffset_1)
		{
			int numSlopePntOffset = ma_SlopeZOffset.size();

			int nMaxSlopePntIdx = 0;
			int numSlopePnt = ma_SlopePointIdx.size();
			for (int idx = 0; idx < numSlopePnt; idx++)
				nMaxSlopePntIdx = max(nMaxSlopePntIdx, ma_SlopePointIdx[idx]);

			if (numSlopePnt > numSlopePntOffset || nMaxSlopePntIdx >= numPnt)
				isInValidSlope = true;
		}
		else
		{
			int numSlopePntOffset = ma_SlopeZOffset.size();

			int numSlopePnt = ma_SlopePoints.size();
			if (numSlopePnt != numSlopePntOffset)
				isInValidSlope = true;
		}

		if (isInValidSlope)
			ClearSlopeInfo();
	}
	return isInValidSlope;
}