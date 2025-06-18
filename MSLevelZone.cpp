#include "StdAfx.h"
#include "MSLevelZone.h"
#include "GMLib/GM2DVector.h"

#include "MSLineLevelDifG.h"
#include "MSFloor.h"
#include "MSFloorQT.h"
#include "MSPolylineObject.h"
#include "GMLib/MSVersionInfo.h"
#include "MSSlabG.h"
#include "MSCompoGUtil.h"

//IMPLEMENT_SERIAL(MSLevelZone, MSObject, VERSIONABLE_SCHEMA | 1)
IMPLEMENT_SERIAL(MSLevelZone, MSElement, VERSIONABLE_SCHEMA | 1)
//long MSLevelZone::ms_LastID = 0;

MSLevelZone::MSLevelZone(void) : m_Level(0) 
{
    m_ID = 0;
    mp_Owner = NULL;
    mp_PolylineInfo = new MSPolylineObject();
	mp_PolylineInfo->SetOwner(this);
	mp_Source = NULL;
	
	m_Width = 0;
	m_PreWidth = 0;
}

MSLevelZone::~MSLevelZone(void)
{
    DeleteLineLevelDifAll();    
	//DeleteSlabJoint();
}
/*
long MSLevelZone::GetID()
{
    if(m_ID == 0) // 
    {
        m_ID = ms_LastID + 1;
        ms_LastID = m_ID;
        return m_ID;
    }

    return m_ID;
}
*/

void MSLevelZone::Serialize( CArchive& ar )
{
	MSElement::Serialize(ar);

	if(ar.IsStoring())
	{
		ar << m_Level;
		ar << m_Width;
		ar << m_UseDefaultWidth;
		mp_PolylineInfo->Serialize(ar);
		int count = ma_LineLevelDif.size();
		ar << count;
		for (int i=0 ; i<ma_LineLevelDif.size() ; ++i)
		{
			ma_LineLevelDif[i]->Serialize(ar);
		}

	}
	else
	{
		ar >> m_Level;
		if(MSVersionInfo::GetCurrentVersion() >= 20140526)
		{
			ar >> m_Width;
			ar >> m_UseDefaultWidth;
		}
		mp_PolylineInfo->Serialize(ar);
		mp_PolylineInfo->SetOwner(this);
		if(MSVersionInfo::GetCurrentVersion() >= 20140527)
		{
			int count = 0;
			ar >> count;
			for (int i=0 ; i<count ; ++i)
			{
				MSLineLevelDifG* pCompoG = new MSLineLevelDifG(this);
				pCompoG->SetOwner(this->GetOwner());
				pCompoG->Serialize(ar);
				//AddLevelDifG(pCompoG);
				
				delete pCompoG;
			}
		}
	}
}

void MSLevelZone::Dump(EFS::EFS_Buffer& buffer)
{
	MSElement::Dump(buffer);
	buffer << m_Level << m_Width << m_UseDefaultWidth;
	//buffer << m_PreWidth;
	mp_PolylineInfo->Dump(buffer);
}

void MSLevelZone::Recovery(EFS::EFS_Buffer& buffer)
{
	MSElement::Recovery(buffer);
	buffer >> m_Level >> m_Width >> m_UseDefaultWidth;
	//buffer >> m_PreWidth;
	mp_PolylineInfo->Recovery(buffer);
}

MSObject* MSLevelZone::Copy( bool bNewID /*= true*/ )
{
	MSLevelZone* pNewObj = new MSLevelZone();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSLevelZone::CopyFromMe( MSLevelZone* pSource, bool bNewID/*=true*/ )
{
	if(pSource == nullptr) return;
	if( !pSource->IsElement() || ((MSElement*)pSource)->GetType() != msLevelZone) { ASSERT(0); return; }
	mp_Owner = pSource->mp_Owner;
	m_Level = pSource->m_Level;
	m_Width = pSource->m_Width;
	m_UseDefaultWidth = pSource->m_UseDefaultWidth;
	MSElement::CopyFromMe(pSource, bNewID);

}

bool MSLevelZone::CheckData()
{
	if (MSElement::CheckData() == false)
		return false;

	MSFloor* floor = GetFloor();
	if (floor == NULL) return false;
	for (map<long, MSCompoG*>::iterator itr = floor->mm_CompoG.begin() ; itr != floor->mm_CompoG.end() ; ++itr)
	{
		if (itr->second->IsVerticalMember() == false) continue;
		if (itr->second->CheckHeight() == false) return false;
	}
	MSFloor* upper = floor->GetUpperFloor();
	if (upper != NULL)
	{
		floor = upper;
		for (map<long, MSCompoG*>::iterator itr = floor->mm_CompoG.begin() ; itr != floor->mm_CompoG.end() ; ++itr)
		{
			if (itr->second->IsVerticalMember() == false) continue;
			if (itr->second->CheckHeight() == false) return false;
		}
	}
	return true;
}

long MSLevelZone::Contains( double Xc, double Yc, double tolerance )
{
    if (this->mp_PolylineInfo == NULL)
        return -2;
    if (this->GetProfile() == NULL)
        return -2;

	return mp_PolylineInfo->Contains(Xc, Yc, tolerance);
}

bool MSLevelZone::OverLap(GM2DCurve* pCurve, double tolerance)
{
	if (this->mp_PolylineInfo == NULL)
		return FALSE;
	if (this->GetProfile() == NULL)
		return FALSE;

	return mp_PolylineInfo->OverLap(pCurve, tolerance);
}

//MSLineLevelDifG은 MSLevelZone::DeleteLineLevelDif()에서 삭제 해준다. 
//메모리 참조 오류를 피하기 위해 MSLineLevelDifG::m_nLiveCnt 구현 AddLevelDifG() 함수에서 증가 시켜줌
// 삭제 되기전에 m_nLiveCnt 감소 하고 m_nLiveCnt==0 일경우에 delete 시킨다.
// 추가 보완 예정임
bool MSLevelZone::DeleteLineLevelDif(MSLineLevelDifG* pDeletedDifG)
{
	MSFloor* pFloor = GetFloor();
	long nLine = ma_LineLevelDif.size();
	for(long iLine = nLine -1 ; iLine >= 0 ; iLine--)
	{
		MSLineLevelDifG* pDifG = ma_LineLevelDif[iLine];
		if(pDifG == pDeletedDifG)
		{
			pFloor->FindAndRemoveLevelDif2LineMembers(pDifG);

			if(pDifG->mp_LeftSlab != NULL)
				pDifG->mp_LeftSlab->RemoveLevelDif(pDifG);
			if(pDifG->mp_RightSlab!= NULL)
				pDifG->mp_RightSlab->RemoveLevelDif(pDifG);

			if(pDifG->mp_LeftLevelZone == this)
				pDifG->mp_LeftLevelZone = NULL;
			if(pDifG->mp_RightLevelZone == this)
				pDifG->mp_RightLevelZone = NULL;

			ma_LineLevelDif.erase(ma_LineLevelDif.begin() + iLine);
			if(pDifG->isDelete())
				delete pDifG;
			return true;
		}
	}
	return false;
}

void MSLevelZone::DeleteLineLevelDifAll()
{
	//MSFloor* pFloor = GetFloor();
    long nLine = ma_LineLevelDif.size();
    for(long iLine=nLine -1 ; iLine >= 0; iLine--)
    {
		MSLineLevelDifG* pDifG = ma_LineLevelDif[iLine];

		DeleteLineLevelDif(pDifG);

    }

    ma_LineLevelDif.clear();
}

void MSLevelZone::DeleteLineLevelDifAllOnEdge()
{
	//MSFloor* pFloor = GetFloor();
	long nLine = ma_LineLevelDif.size();
	for(long iLine=nLine -1 ; iLine >= 0; iLine--)
	{
		MSLineLevelDifG* pDifG = ma_LineLevelDif[iLine];
		if(!pDifG->IsOnEdgeLevelDifG())
			continue;

		DeleteLineLevelDif(pDifG);

	}
}


// -1 : 보-벽 조합으로 레벨 겹침.
// 0 : 레벨 불일치
// 1 : 한 부재 하단에 한 부재 상단이 접함
// 2 : 상호 어긋나게 교차
// 3 : my가 ur에 포함됨
// 4 : my가 ur를 포함함
// 5 : 완전 일치
long MSLevelZone::LevelOverlap(MSLevelZone* pUrLevelZone)
{
	return 5;
}

/*
double MSLevelZone::GetLevel4LevelZone( double Xc, double Yc, double tolerance )
{
	return mp_PolylineInfo->GetLevel4LevelZone(Xc, Yc, tolerance);
}
*/

MSFloor* MSLevelZone::GetFloor()
{
	MSFloor* pFloor = NULL;
	if(mp_Owner == NULL) 
	{
		ASSERT(0);
		return pFloor;
	}
	if(mp_Owner->GetType() == MSElement::msFloor)
		pFloor = (MSFloor*)mp_Owner;
	else
	{
		ASSERT(0);
		return pFloor;
	}
	return pFloor;
}

CString MSLevelZone::GetGroupName()
{
	CString strGroupName = _T("LevelZone");
	return strGroupName;
}

CString MSLevelZone::GetUserFloorName()
{
	MSFloor* pCurFloor = GetFloor();
	return pCurFloor->GetUpperFloor()->GetName();
}

void MSLevelZone::DeleteLevelZoneInfo()
{
	MSFloor* pFloor = GetFloor();
	if(mp_Owner == NULL) 
		return;

// 	vector<MSCompoG*> SlabArr;
// 	pFloor->GetMSCompoGArr(MSElement::msSlabG, SlabArr);
	vector<MSCompoG*> SlabArr = MSCompoGUtil::FindCompoG(pFloor, GetProfile(), FindType_MSElement::msSlabG, false);
	long nSlab = SlabArr.size();
	for(long iSlab=0; iSlab<nSlab; iSlab++)
	{
		MSSlabG* pSlab = (MSSlabG*)(SlabArr[iSlab]);
		pSlab->RemoveLevelZone(this);
	}

	DeleteLineLevelDifAll();
	ma_LineLevelDif.clear();
	//DeleteSlabJoint();
}

long MSLevelZone::MakeLevelZoneInfo(bool bMakeSlab)
{
	MSFloor* pFloor = GetFloor();
	if(mp_Owner == NULL) 
		return 0;

	// LevelDif 생성
	//mp_PolylineInfo->TrimExistPolyLineObject();
	long nLine = GetProfile()->GetNumLines();
	for(long iLine=0; iLine<nLine; iLine++)
	{
		//MSLineLevelDifG NewDifG(this);
		MSLineLevelDifG* pNewDifG = new MSLineLevelDifG(this);
		pNewDifG->SetCenCurve(GetProfile()->GetCurve(iLine));
		pNewDifG->mp_LeftLevelZone = this;
		pFloor->Add(pNewDifG);

// 		if(pNewDifG->mp_Curve->m_P0.SameVector4DTOL(pNewDifG->mp_Curve->m_P1))
// 		{
// 			if(pNewDifG->mp_LeftLevelZone != NULL)
// 				pNewDifG->mp_LeftLevelZone->DeleteLineLevelDif(pNewDifG);
// 			if(pNewDifG->mp_RightLevelZone != NULL)
// 				pNewDifG->mp_RightLevelZone->DeleteLineLevelDif(pNewDifG);
// 		}
	}

	long nLevelDif = ma_LineLevelDif.size();
	//길이, GetDepth()가 0인 LevelDif 삭제
	for(long iLevelDif = nLevelDif -1 ; iLevelDif >= 0 ; iLevelDif--)
	{
		MSLineLevelDifG* pLevelDif = ma_LineLevelDif[iLevelDif];
		if(pLevelDif == NULL) continue;
		double dLen = pLevelDif->GetLength();
		double dDepth = ma_LineLevelDif[iLevelDif]->GetDepth();
		//if(dLen < DTOL_GM || dDepth == 0)
		if(dLen < DTOL_GM)
		{
			if(ma_LineLevelDif[iLevelDif]->isDelete())
			{
				delete ma_LineLevelDif[iLevelDif];
				ma_LineLevelDif.erase(ma_LineLevelDif.begin() + iLevelDif);
			}
			continue;
		}
	}

	// 분리
// 	nLevelDif = ma_LineLevelDif.size();
// 	for(long iLevelDif=0; iLevelDif<nLevelDif; iLevelDif++)
// 	{
// 		pFloor->FindAndInsertLevelDif2LineMembers(this->ma_LineLevelDif[iLevelDif]);
// 	}

	long nSlabLevelZone = 0;
	if(bMakeSlab)
		nSlabLevelZone = MakeCrossThenAddLevelDif();
	return nSlabLevelZone;
}

long MSLevelZone::MakeCrossThenAddLevelDif()
{
	MSFloor* pFloor = GetFloor();
	if(pFloor == NULL)
		return 0;

	long nSlabLevelZone = 0;
// 	vector<MSCompoG*> SlabArr;
// 	pFloor->GetMSCompoGArr(MSElement::msSlabG, SlabArr);
	vector<MSCompoG*> SlabArr = MSCompoGUtil::FindCompoG(pFloor, GetProfile(), FindType_MSElement::msSlabG, false);

	long nSlab = SlabArr.size();
	for(long iSlab=0; iSlab<nSlab; iSlab++)
	{
		// Modify 시 isDoit = false
		bool isDoit = true;
		MSSlabG* pSlab = (MSSlabG*)(SlabArr[iSlab]);
		nSlabLevelZone += pSlab->MakeSlabLevelZone(this, isDoit);
		pSlab->CheckCrossThenAddLevelDif(this, isDoit);

		long NumLevelZone = pSlab->ma_LevelPoly.size();
		for(long iZone = 0 ; iZone < NumLevelZone ; iZone++)
		{
			MSSlabLevelZone* pSlabZone = pSlab->ma_LevelPoly[iZone];
			if(pSlabZone->mp_LevelZone == this || pSlabZone->m_IsDelete == true)
				continue;

			long nLevelDif = ma_LineLevelDif.size();
			for(long iLevelDif=0; iLevelDif<nLevelDif; iLevelDif++)
			{
				MSLineLevelDifG* pLevelDifG = ma_LineLevelDif[iLevelDif];
				pSlabZone->SetLevelDifWidth(pLevelDifG, pLevelDifG->GetThick());
			}
		}
	}

	return 1;
}

void MSLevelZone::ReMakeLevelDif(bool bMakeSlab )
{
	// 트림 이후 다시 LevelDif 만들때만 이전 Width 값을 0으로 초기화함
	m_PreWidth = 0;
	DeleteLevelZoneInfo();

	// LevelDif 삭제
// 	long nLevelDif = ma_LineLevelDif.size();
// 	for(long iLevelDif=0; iLevelDif<nLevelDif; iLevelDif++)
// 		pFloor->FindAndRemoveLevelDif2LineMembers(ma_LineLevelDif[iLevelDif]);

	MakeLevelZoneInfo(bMakeSlab);

	FindAndInsertLevelDif2LineMembers();

	TrimLevelDifbySlabG();
}

void MSLevelZone::FindAndInsertLevelDif2LineMembers()
{
	long nLevelDif = ma_LineLevelDif.size();
	for(long iLevelDif=0; iLevelDif<nLevelDif; iLevelDif++)
	{
		MSFloor* pFloor = GetFloor();
		pFloor->FindAndInsertLevelDif2LineMembers(ma_LineLevelDif[iLevelDif]);
	}
}

void MSLevelZone::AddLevelDifG(MSLineLevelDifG* pDifG)
{
	for (int i=0 ; i<ma_LineLevelDif.size() ; ++i)
	{
		if(ma_LineLevelDif[i] == pDifG)
			return;
	}
	//if(pDifG->mp_LeftLevelZone != this)
		pDifG->LiveCntUp();
	ma_LineLevelDif.push_back(pDifG);
}

void MSLevelZone::GetBoundary(GM2DPolyline& APolyLine)
{
	GM2DPolyline *pProfile = GetProfile();
	if(pProfile == NULL)
		return;
	APolyLine.CopyFrom(pProfile);
}

void MSLevelZone::RecoverPointerAfterSerialize()
{
	MSFloor* floor = GetFloor();
	if (floor == NULL) return;
	for (int i=0 ; i<ma_LineLevelDif.size() ; ++i)
		ma_LineLevelDif[i]->RecoverPointerAfterSerialize(floor);
}

/*

void MSLevelZone::MakeLineLevelDifFromProfile()
{
    DeleteLineLevelDif();
    long nLine = m_Profile.GetNumLines();
    for(long iLine=0; iLine<nLine; iLine++)
    {
        MSLineLevelDifG* pNewDifG = new MSLineLevelDifG();
        pNewDifG->SetCurve(m_Profile.GetCurve(iLine));
        pNewDifG->m_LeftSideLevel = m_Level;
//         ((MSFloor*)mp_Owner)->mp_QT->FindCrossQTs(pNewDifG, false);
        ma_LineLevelDif.Add(pNewDifG);
        ((MSFloor*)mp_Owner)->InsertLevelDif2LineMembers(pNewDifG);
    }
}

void MSLevelZone::MakeSlabLevelDifFromProfile()
{

}
*/

void MSLevelZone::Translate( GM2DVector &pDelta )
{
	MSElement::Translate(pDelta);
}

void MSLevelZone::MirrorByAxis( GM2DLineVector& pAxis )
{
	MSElement::MirrorByAxis(pAxis);
}

void MSLevelZone::RotateZ( double theta, GM2DVector *pCenter )
{
	MSElement::RotateZ(theta, pCenter);
}

void MSLevelZone::TrimLevelDifbySlabG()
{
	MSFloor* pFloor = GetFloor();
	map<long, MSSlabG*> SlabMap;

	long NumDif = ma_LineLevelDif.size();
/*

	//길이, GetDepth()가 0인 LevelDif 삭제
	for(long iLevelDif = NumDif -1 ; iLevelDif >= 0 ; iLevelDif--)
	{
		MSLineLevelDifG* pLevelDif = ma_LineLevelDif[iLevelDif];
		MSLineLevelDifG* pPreLevelDif = ma_LineLevelDif[(iLevelDif+ ma_LineLevelDif.size() -1)%ma_LineLevelDif.size()];
		MSLineLevelDifG* pNextLevelDif = ma_LineLevelDif[(iLevelDif+1)%ma_LineLevelDif.size()];
		if(pLevelDif == NULL) continue;
		double dLen = pLevelDif->GetLength();
		//if(dLen + DTOL_GM < m_Width)
		if(dLen < DTOL_GM)
		{
// 			if(pLevelDif->mp_Curve->IsColinear4DTol(pPreLevelDif->mp_Curve) != 0)
// 			{
// 				double dSLoc = pPreLevelDif->mp_Curve->OrthoBaseLocValue(pLevelDif->mp_Curve->m_P0);
// 				double dELoc = pPreLevelDif->mp_Curve->OrthoBaseLocValue(pLevelDif->mp_Curve->m_P1);
// 				double dMLoc = pPreLevelDif->mp_Curve->OrthoBaseLocValue(pLevelDif->mp_Curve->PointAtParam(0.5));
// 				if(dMLoc< 0.5)
// 				{
// 					pPreLevelDif->mp_Curve->m_P0 = pLevelDif->mp_Curve->PointAtParam(min(dSLoc, dELoc));
// 					pPreLevelDif->ma_Vec[0]->CopyFrom(&pPreLevelDif->mp_Curve->m_P0);
// 				}
// 				else
// 				{
// 					pPreLevelDif->mp_Curve->m_P1 = pLevelDif->mp_Curve->PointAtParam(max(dSLoc, dELoc));
// 					pPreLevelDif->ma_Vec[1]->CopyFrom(&pPreLevelDif->mp_Curve->m_P1);
// 				}
// 			}
// 
// 			if(pLevelDif->mp_Curve->IsColinear4DTol(pNextLevelDif->mp_Curve) != 0)
// 			{
// 				double dSLoc = pNextLevelDif->mp_Curve->OrthoBaseLocValue(pLevelDif->mp_Curve->m_P0);
// 				double dELoc = pNextLevelDif->mp_Curve->OrthoBaseLocValue(pLevelDif->mp_Curve->m_P1);
// 				double dMLoc = pNextLevelDif->mp_Curve->OrthoBaseLocValue(pLevelDif->mp_Curve->PointAtParam(0.5));
// 				if(dMLoc< 0.5)
// 				{
// 					pNextLevelDif->mp_Curve->m_P0 = pLevelDif->mp_Curve->PointAtParam(min(dSLoc, dELoc));
// 					pNextLevelDif->ma_Vec[0]->CopyFrom(&pNextLevelDif->mp_Curve->m_P0);
// 				}
// 				else
// 				{
// 					pNextLevelDif->mp_Curve->m_P1 = pLevelDif->mp_Curve->PointAtParam(max(dSLoc, dELoc));
// 					pNextLevelDif->ma_Vec[1]->CopyFrom(&pNextLevelDif->mp_Curve->m_P1);
// 				}
// 			}
			DeleteLineLevelDif(pLevelDif);
			continue;
		}
	}

	NumDif = ma_LineLevelDif.size();*/
	for(long iDif = 0 ;iDif < NumDif ; iDif++)
	{
		MSLineLevelDifG* pDif = ma_LineLevelDif[iDif];
		if(pDif == NULL) continue;

		map<long, long>::iterator it;
		for(it = pDif->mm_SlabGID.begin() ; it != pDif->mm_SlabGID.end() ; it++)
		{
			MSCompoG* pSlabG = pFloor->GetCompoG(it->second);
			if(pSlabG != NULL)
				SlabMap.insert(make_pair(pSlabG->m_ID, (MSSlabG*)pSlabG));
		}

// 		if(pDif->mp_LeftSlab != NULL)
// 		{
// 			SlabIt = SlabMap.find(pDif->mp_LeftSlab->m_ID);
// 			if(SlabIt == SlabMap.end())
// 				SlabMap.insert(make_pair(pDif->mp_LeftSlab->m_ID, pDif->mp_LeftSlab));
// 		}
// 
// 		if(pDif->mp_RightSlab!= NULL)
// 		{
// 			SlabIt = SlabMap.find(pDif->mp_RightSlab->m_ID);
// 			if(SlabIt == SlabMap.end())
// 				SlabMap.insert(make_pair(pDif->mp_RightSlab->m_ID, pDif->mp_RightSlab));
// 		}
	}

	TrimLevelDifbySlabGSetp2(SlabMap);
}

void MSLevelZone::TrimLevelDifbySlabGSetp2(map<long, MSSlabG*>& SlabMap)
{
	MSFloor* pFloor = GetFloor();
	map<long, MSSlabG*>::iterator SlabIt;

	long NumDif = ma_LineLevelDif.size();

	vector<MSLinkMemberG*> LevelDifArrAll;
	for (SlabIt = SlabMap.begin(); SlabIt != SlabMap.end(); SlabIt++)
	{
		MSSlabG* pSlab = SlabIt->second;

		pSlab->GetLevelDifWidthInSlabLevelZone();
		vector<MSLinkMemberG*> LevelDifArr = pSlab->GetLevelDifGAll(true);
		for (long iDif = 0; iDif < LevelDifArr.size(); iDif++)
		{
			MSLinkMemberG* pLevelDif = LevelDifArr[iDif];
			//pLevelDif->SetLines(msSlabG, pSlab->GetThick());
			//새로 만드는 Dif만 SetLines 한다. 위쪽으로 이동 시킴
			LevelDifArrAll.push_back(pLevelDif);
		}
	}

	/*for(long iDif = 0 ;iDif < NumDif ; iDif++)
	{
	MSLineLevelDifG* pDif = ma_LineLevelDif[iDif];
	if(pDif == NULL) continue;
	pDif->SetLines(msSlabG, pDif->m_dWallWidth);
	}*/

	long nLevelDif = LevelDifArrAll.size();
	for (long iLevelDif = 0; iLevelDif<nLevelDif; iLevelDif++)
	{
		MSLineLevelDifG* pLevelDif = (MSLineLevelDifG*)LevelDifArrAll[iLevelDif];
		pLevelDif->GetID();
		pLevelDif->RemoveConnections();
		pLevelDif->SetLines(msSlabG, pLevelDif->m_dWallWidth);
		if (pLevelDif->mp_LLine == NULL || pLevelDif->mp_RLine == NULL)
		{
			// 			if(pLevelDif->mp_LeftLevelZone != NULL)
			// 				pLevelDif->SetLines(msSlabG, pLevelDif->mp_LeftLevelZone->m_Width);
			// 			else if(pLevelDif->mp_RightLevelZone != NULL)
			// 				pLevelDif->SetLines(msSlabG, pLevelDif->mp_RightLevelZone->m_Width);
			// 			else
			ASSERT(0);
		}
	}

	for (long iLevelDif = 0; iLevelDif<nLevelDif; iLevelDif++)
	{
		MSLinkMemberG* pMyLevelDif = LevelDifArrAll[iLevelDif];
		vector<MSCompoG*> OverLapCompoGArr;

		GM2DPolyline MyPoly(TRUE);
		pMyLevelDif->GetBoundary(MyPoly);

		for (long jLevelDif = 0; jLevelDif<nLevelDif; jLevelDif++)
		{
			MSLinkMemberG* pUrLevelDif = LevelDifArrAll[jLevelDif];
			if (pMyLevelDif == pUrLevelDif)
				continue;

			CheckModelOpt tmpOpt;
			if (pMyLevelDif->IsLevelConnectedOverLap(pUrLevelDif, tmpOpt) == false)
				continue;

			GM2DPolyline UrPoly(TRUE);
			pUrLevelDif->GetBoundary(UrPoly);

			BOOL bOverlaped = MyPoly.OverlapEx(UrPoly, MSFloor::ms_DefLineHitDistTol);
			if (bOverlaped)
				OverLapCompoGArr.push_back(pUrLevelDif);
		}

		// 입력 순서대로 정렬
		sort(OverLapCompoGArr.begin(), OverLapCompoGArr.end(),
			[](const MSCompoG* a, const MSCompoG* b)->bool {if (a->m_nCmdCnt == b->m_nCmdCnt) return a->m_ID < b->m_ID; else return a->m_nCmdCnt < b->m_nCmdCnt; }
		);

		for (long i = 0; i < OverLapCompoGArr.size(); i++)
			pMyLevelDif->AddConnectionWithMember(OverLapCompoGArr[i]);


		pMyLevelDif->UpdateTrimmedBoundary();
	}

	for (long iLevelDif = 0; iLevelDif<nLevelDif; iLevelDif++)
	{
		MSLinkMemberG* pMyLevelDif = LevelDifArrAll[iLevelDif];
		pMyLevelDif->UpdateBoundaryVisibilities();
		pMyLevelDif->SetTrimmedLines();
		pFloor->CalculateUpdatedCompoGArray(pMyLevelDif, pFloor->CompoGIDList_NeedRegen_CurTR());
	}

	for (SlabIt = SlabMap.begin(); SlabIt != SlabMap.end(); SlabIt++)
	{
		MSSlabG* pSlab = SlabIt->second;

		pSlab->SetLevelDifWidthInSlabLevelZone();
	}
}

void MSLevelZone::ModifyObject(bool isDoit)
{
	MSFloor* pFloor = GetFloor();
// 	pFloor->Remove(this);
// 	pFloor->Add(this);
	pFloor->Modify(this, isDoit);
}
