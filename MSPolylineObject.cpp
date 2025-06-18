#include "StdAfx.h"
#include "GMLib\AcisGeomHelper.h"
#include "MSPolylineObject.h"
#include "MSElement.h"
#include "MSSlabG.h"
#include "MSLevelZone.h"
#include "MSOpening.h"
#include "MSWell.h"
#include "MSTrench.h"
#include "MSFloor.h"
#include "MSConstructionZone.h"
#include "GMLib\GM2DBound.h"
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

IMPLEMENT_SERIAL(MSPolylineObject, MSElement, VERSIONABLE_SCHEMA | 1)

MSPolylineObject::MSPolylineObject()
{
	mp_Owner = NULL;
	m_isAddPoly = FALSE;
    mp_Profile = new GM2DPolyline(TRUE);
}

MSPolylineObject::~MSPolylineObject(void)
{
    if(mp_Profile != NULL)
        delete mp_Profile;

	DeleteOpenings();
}

void MSPolylineObject::DeleteOpenings()
{
    while(!ma_Opening.empty()) 
    {
        delete ma_Opening.back();
        ma_Opening.pop_back();
    }
}

void MSPolylineObject::DeleteOpeningsByMadeByCompoG()
{
	for (int i = ma_Opening.size() - 1; i >= 0; i--)
	{
		if(ma_Opening[i]->IsMadeByCompoG() == false)
			continue;
		delete ma_Opening[i];
		ma_Opening.erase(ma_Opening.begin() + i);
	}
}

void MSPolylineObject::Serialize( CArchive& ar )
{
	MSElement::Serialize(ar);
	if(ar.IsStoring())
	{
		mp_Profile->Serialize(ar);
		ar << m_isAddPoly;

		int OpenningCount = ma_Opening.size();
		ar << OpenningCount;
		for(int iCount = 0; iCount < OpenningCount; iCount++)
		{
			ma_Opening[iCount]->Serialize(ar);
		}
	}
	else
	{
		mp_Profile->Serialize(ar);
		ar >> m_isAddPoly;

		int OpenningCount = 0;
		ar >> OpenningCount;
		for(int iCount = 0; iCount < OpenningCount; iCount++)
		{
			MSOpening* pOpenning = new MSOpening();
			pOpenning->Serialize(ar);
			pOpenning->SetOwner(this);
			ma_Opening.push_back(pOpenning);
		}

		if(MSVersionInfo::GetCurrentVersion() < 20150428)
		{
			if(mp_Profile->ma_Point.GetSize() > 0)
				mp_Profile->MakeCCW();
		}
	}
	// [BA-4255] 모델링을 잘못해서 오프닝이 여러개 생기는 경우 중복 오프닝 정리
	RemoveSameOpening();
}


void MSPolylineObject::Dump(EFS::EFS_Buffer& buffer)
{
	mp_Profile->Dump(buffer);
	buffer << m_isAddPoly;

	int OpenningCount = ma_Opening.size();
	buffer << OpenningCount;
	for(int iCount = 0; iCount < OpenningCount; iCount++)
	{
		ma_Opening[iCount]->Dump(buffer);
	}
}

void MSPolylineObject::Recovery(EFS::EFS_Buffer& buffer)
{
	mp_Profile->Recovery(buffer);
	buffer >> m_isAddPoly;

	while(!ma_Opening.empty()) 
	{
		delete ma_Opening.back();
		ma_Opening.pop_back();
	}

	int OpenningCount = 0;
	buffer >> OpenningCount;
	for(int iCount = 0; iCount < OpenningCount; iCount++)
	{
		MSOpening* pOpenning = new MSOpening();
		pOpenning->Recovery(buffer);
		pOpenning->SetOwner(this);
		ma_Opening.push_back(pOpenning);
	}
}


MSElement* MSPolylineObject::Copy( bool bNewID /*= true*/ )
{
	MSPolylineObject* pNewObj = new MSPolylineObject();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSPolylineObject::CopyFromMe( MSPolylineObject* pSource, bool bNewID/*=true*/ )
{
	MSElement::CopyFromMe(pSource, bNewID);

	if(pSource)
	{
		mp_Owner = pSource->mp_Owner;
		mp_Profile->CopyFrom(pSource->mp_Profile); // mylee - 나중에 이 함수 이름 바꿀 필요
		m_isAddPoly = pSource->m_isAddPoly;

		long NumOpening = pSource->ma_Opening.size();
		for(long iOpen = 0 ; iOpen < NumOpening ; iOpen++)
		{
			MSOpening* pOpen = new MSOpening();
			pOpen->CopyFromMe(pSource->ma_Opening[iOpen]);
			pOpen->SetOwner(this);
			ma_Opening.push_back(pOpen);
		}
	}
}

void MSPolylineObject::SetOwner(MSElement* pOwner)
{
	mp_Owner = pOwner;
}

MSElement::Type_MSCompoG MSPolylineObject::GetOwnerType()
{
	if(mp_Owner == NULL)
		return MSElement::msNULL;
	return mp_Owner->GetType();
}



// -2 : 폴리라인 이상
// -1 : 오프닝에 포함
// 0 : 폴리라인 선위에 포함 
// 1 : 폴리라인 안에 포함됨
long MSPolylineObject::Contains( double Xc, double Yc, double tolerance )
{
    if (&mp_Profile == NULL)
        return -2;

    GM2DVector vec(Xc, Yc);
	return Contains(&vec, tolerance);
}

long MSPolylineObject::Contains(GM2DVector* pVec, double tolerance)
{
	long NumOpening = ma_Opening.size();
	for (long iOpening = 0; iOpening < NumOpening; iOpening++)
	{
		if (ma_Opening[iOpening] == NULL) continue;
		long nRval = ma_Opening[iOpening]->GetProfile()->Contains(pVec, tolerance);
		if (nRval == -1)
			return 1;
		else if (nRval == 0)
			return -1;
	}

	return mp_Profile->Contains(pVec, tolerance);
}

bool MSPolylineObject::Contains(GM2DCurve* pCurve,bool bIncludeOnLine, double tolerance)
{
	// 오프닝 검토시 Curve가 오프닝의 선위에 있다면 포함이다.
	// ==> 추가 정보 입력해야할 경우가 존재함
	long NumOpening = ma_Opening.size();
	for (long iOpening = 0; iOpening < NumOpening; iOpening++)
	{
		if (ma_Opening[iOpening] == NULL) continue;
		bool bRval = ma_Opening[iOpening]->GetProfile()->Contains(pCurve, false, tolerance);
		if (bRval == true)
			return false;
	}

	return mp_Profile->Contains(pCurve, bIncludeOnLine, tolerance);
}

bool MSPolylineObject::Contains(GM2DPolyline* pPoly, bool bIncludeOnLine, double tolerance)
{
	// 오프닝 검토시 폴리라인이 오프닝과 같으면 포함되지 않는다.
	// ==> 추가 정보 입력할 필요가 없다.
	long NumOpening = ma_Opening.size();
	for (long iOpening = 0; iOpening < NumOpening; iOpening++)
	{
		if (ma_Opening[iOpening] == NULL) continue;
		bool bRval = ma_Opening[iOpening]->GetProfile()->Contains(pPoly, true, tolerance);
		if (bRval == true)
			return false;
	}

	return mp_Profile->Contains(pPoly, bIncludeOnLine, tolerance);
}


bool MSPolylineObject::OverLap(GM2DCurve* pCurve, double tolerance)
{
	if (&mp_Profile == NULL)
		return FALSE;

	long nRval = mp_Profile->OverlapEx(pCurve, tolerance);
	if(nRval > 0)
		return true;
	else
		return false;
}

void MSPolylineObject::ReMakeLevelDif(bool bMakeSlab)
{
	GetOwner()->ReMakeLevelDif(bMakeSlab);
}

GM2DPolyline* MSPolylineObject::GetProfile()
{
	if(mp_Profile == NULL)
		ASSERT(0);
	return mp_Profile;
}

GM2DPolyline* MSPolylineObject::GetTrimmedBoundary(bool IsTop)
{
	MSElement* pOwner = GetOwner();
	if (pOwner->IsCompoG() == true)
		return ((MSCompoG*)pOwner)->GetTrimmedBoundary(IsTop);

	return mp_Profile;
}

void MSPolylineObject::SetTrimmedBoundary(GM2DPolyline* pPoly, bool isTop)
{
	GM2DPolyline* pOrgPoly = GetTrimmedBoundary(isTop);
	if (pOrgPoly->SamePolyLine(pPoly))
		return;
	pOrgPoly->CopyFrom(pPoly);
	pOrgPoly->MakeClosedPolyline(DTOL_MODELING);
	if (mp_Owner->GetType() == MSElement::msSlabG)
	{
		MSSlabG* pSlab = (MSSlabG*)mp_Owner;
		pSlab->ClearSlopeInfo();
// 		if(isTop == true)
// 			pSlab->UpdateBoundaryVisibilities();
	}
	else if (mp_Owner->GetType() == MSElement::msLevelZone)
	{

	}
	// CZone의 프로파일이 변경되었을경우 CZone에 의해 생성된 오프닝의 프로파일도 변경해준다.
	else if (mp_Owner->GetType() == MSElement::msConstructionZone)
	{
		MSConstructionZone* pFindCZone = dynamic_cast<MSConstructionZone*>(mp_Owner->GetContainingElement());
		if (pFindCZone != nullptr)
			pFindCZone->UpdateOpeningProfile(pPoly, mp_Owner->m_ID);
	}
}

void MSPolylineObject::InitTrimmedBoundary(bool isTop)
{
	if (mp_Owner->GetType() == MSElement::msSlabG)
	{
		MSSlabG* pSlab = (MSSlabG*)mp_Owner;
		pSlab->InitTrimmedBoundary(isTop);
	}
}

void MSPolylineObject::SetProfile(GM2DPolyline* pPoly)
{
	if (pPoly == nullptr || mp_Profile == pPoly || pPoly->GetNumLines() == 0)
		return;
	//[BH-3971] [대림] 특정슬래브의 기본수정작업 작동하지 않는 오류
	// 수정된 프로파일이 셋팅되지 않는 경우가 발생하여 SamePolyLine() 검사 제거
	if (mp_Profile == pPoly) //|| mp_Profile->SamePolyLine(pPoly))
		return;
	mp_Profile->CopyFrom(pPoly);
	if(mp_Owner->GetType() == MSElement::msSlabG)
	{
		MSSlabG* pSlab = (MSSlabG*)mp_Owner;
		// 프로파일 변경시 경사 정보 유지하도록 변경
		//pSlab->ClearSlopeInfo();
		pSlab->UpdateBoundaryVisibilities();
	}
	else if(mp_Owner->GetType() == MSElement::msLevelZone)
	{

	}
}

bool MSPolylineObject::LevelOverlap(MSPolylineObject* pUrPolylineObject, bool IsTop)
{

	if(mp_Owner->GetType() == MSElement::msSlabG)
	{
		if(pUrPolylineObject->mp_Owner->GetType() == MSElement::msSlabG)		// this : Slab  Ur : Slab
		{
			//if (IsTop)
			{
				if (((MSSlabG*)mp_Owner)->LevelOverlap((MSSlabG*)pUrPolylineObject->mp_Owner) != 0)
					return true;
				else
					return false;
			}
// 			MSSlabG* pMySlabG = (MSSlabG*)mp_Owner;
// 			MSSlabG* pUrSlabG = (MSSlabG*)pUrPolylineObject->mp_Owner;
// 			double MyTopLevel, MyBotLevel;
// 			double UrTopLevel, UrBotLevel;
// 			if (!pMySlabG->GetZLevelsWidthOutZLevel(MyTopLevel, MyBotLevel))
// 				ASSERT(0);
// 			if (!pUrSlabG->GetZLevelsWidthOutZLevel(UrTopLevel, UrBotLevel))
// 				ASSERT(0);
// 
// 			if (IsTop)
// 			{
// 				if (MyTopLevel < UrTopLevel + DTOL_GM && MyTopLevel > UrBotLevel)
// 					return true;
// 				if (MyTopLevel > UrTopLevel + DTOL_GM && MyTopLevel > UrBotLevel &&
// 					MyBotLevel < UrTopLevel + DTOL_GM && MyBotLevel > UrBotLevel)
// 					return true;
// 			}
// 			else
// 			{
// 				if (fabs(MyBotLevel - UrBotLevel) < DTOL_GM)
// 					return true;
// 			}
			return false;
		}
		else   // this : Slab  Ur : LevelZone
		{

		}
	}
	else if(mp_Owner->GetType() == MSElement::msLevelZone)
	{
		if(pUrPolylineObject->mp_Owner->GetType() == MSElement::msSlabG)		// this : LevelZone  Ur : Slab
		{
			
		}
		else   // This : LevelZone  Ur : LevelZone
		{
			if (((MSLevelZone*)mp_Owner)->LevelOverlap((MSLevelZone*)pUrPolylineObject->mp_Owner) != 0)
				return true;
			else
				return false;
		}
	}
	else if (mp_Owner->GetType() == MSElement::msConstructionZone)
	{
		return true;
	}

	return false;
}

GM2DVector* MSPolylineObject::InsertVec( long Index, GM2DVector* pVec )
{
    if(Index>=mp_Profile->ma_Point.GetSize())
        return NULL;
    GM2DVector* pOldVec = mp_Profile->ma_Point[Index];
    mp_Profile->ma_Point.InsertAt(Index, pVec, 1);
    mp_Profile->ma_Bulge.InsertAt(Index, 0, 1);
    return pOldVec;
}

void MSPolylineObject::ReplaceVec( long Index, GM2DVector* pVec )
{
    if(Index>=mp_Profile->ma_Point.GetSize())
        return;
    GM2DVector* pOldVec = mp_Profile->ma_Point[Index];
    mp_Profile->ma_Point[Index]=pVec;
    delete pOldVec;
}

GM2DVector* MSPolylineObject::GetVec( long Index )
{
    if(Index>=mp_Profile->ma_Point.GetSize())
        return NULL;
    return mp_Profile->ma_Point[Index];
}

bool MSPolylineObject::AddOpening(MSOpening* opening)
{
	if (opening != nullptr && AppGlobalOptionWrapper::Instance()->GetAppMode() == AppGlobalOptionWrapper::Formwork)
		return false;

	ma_Opening.push_back(opening);
	return true;
}
bool MSPolylineObject::AddOpening(GM2DPolyline* pPoly, MSElement* pElement, bool isContain)
{
	if (pElement != nullptr && AppGlobalOptionWrapper::Instance()->GetAppMode() == AppGlobalOptionWrapper::Formwork)
		return false;

// 	if(pElement->GetType() == GetOwnerType() && pElement->GetType() == MSElement::msSlabG)
// 		return false;

	// 시공존인 경우 같은층인 경우에만 처리한다, 층이 다르면 오프닝을 입력하지 않는다
	if (pElement->GetType() == MSElement::msConstructionZone && GetOwnerType() == MSElement::msConstructionZone)
	{
		MSFloor* pFloor = pElement->GetFloor();
		MSFloor* pCurFloor = GetFloor();
		if (pFloor != nullptr && pCurFloor != nullptr && pFloor != pCurFloor)
			return false;
	}

	GM2DPolyline* pProfile = GetProfile();
	if (pProfile->SamePolyLine(pPoly) == true)
		return false;

	bool isMyUrSlab = false;
	if(pElement != NULL &&
		((pElement->GetType() == MSElement::msSlabG && GetOwnerType() == MSElement::msSlabG) ||
		(pElement->GetType() == MSElement::msConstructionZone && GetOwnerType() == MSElement::msConstructionZone) ||
		pElement->GetType() == MSElement::msPitG)
		)
		isMyUrSlab = true;

	
// 	MSSlabG* pMySlab = (MSSlabG*)GetOwner();
// 	long nRval = pMySlab->Contains(pPoly);
// 	if(nRval != -1)
// 		return;

	bool bFindOpening = false;
	MSOpening* pOpenning = nullptr;
	for (int iOpening = 0; iOpening < ma_Opening.size(); iOpening++)
	{
		MSOpening* pMyOpening = ma_Opening[iOpening];
		if (pPoly->SamePolyLine(pMyOpening->GetProfile()))
			pOpenning = pMyOpening;
	}

	if (pOpenning == nullptr)
	{
		pOpenning = new MSOpening();
		pOpenning->SetOwner(this);
		pOpenning->SetProfile(pPoly);
		bFindOpening = true;
	}

	if(isMyUrSlab)
	{
		MSElement* pMySlab = (MSElement*)GetOwner();
		MSElement* pUrSlab = (MSElement*)pElement;
		if(isContain)
			pUrSlab->SetContainingElementId(pMySlab->m_ID);
		pOpenning->AddMadeByCompoGID(pElement->m_ID);
	}
	
	if (bFindOpening == true)
	{
		pOpenning->GetID();
		ma_Opening.push_back(pOpenning);
	}

// 	GM2DPolyline* pProfile = GetProfile();
// 	pProfile->SegmentstoMe(pPoly);

	MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(mp_Owner->m_ID);

//    MergeOpenings();

	return true;
}

bool MSPolylineObject::RemoveOpening(MSOpening* pDelOpening)
{
	if(pDelOpening == NULL)
		return false;
	long NumOpen = ma_Opening.size()-1;
	for(long iOpen = NumOpen ; iOpen >= 0  ; iOpen--)
	{
		if(ma_Opening[iOpen] == pDelOpening)
		{
			delete ma_Opening[iOpen];
			ma_Opening.erase(ma_Opening.begin()+iOpen);
			return true;
		}
	}
	return false;
}

bool MSPolylineObject::CheckInValidOpening()
{
	// 현재는 오프닝을 수동으로 입력할수 없다.
	// 슬래브 안에 슬래브, CZone안에 CZone에서만 처리하고 있음
	// 다른 부재는 은 처리 하지 않음
	for (int i = ma_Opening.size() - 1; i >= 0; i--)
	{
		MSOpening* pOpening = ma_Opening[i];
		if (pOpening->ma_MadeByCompoGID.size() == 0)
		{
			ma_Opening.erase(ma_Opening.begin() + i);
				delete pOpening;
		}
	}
	return false;
}

void MSPolylineObject::TrimOpenings()
{
	double dTol = DTOL_GM;
	for (int i = 0; i < ma_Opening.size(); i++)
	{
		MSOpening* pOpening = ma_Opening[i];
		GM2DPolyline* pOpeningPoly = pOpening->GetProfile();
		if(mp_Profile->Contains(pOpeningPoly, false) == true)
			continue;

		if (mp_Profile->OverlapEx(*pOpeningPoly, DTOL_GM, FALSE) == false)
		{
			delete pOpening;
			ma_Opening.erase(ma_Opening.begin() + i);
			continue;
		}

		vector<GM2DPolyline*> Joined_Polys;
		mp_Profile->BoolOperation('A', pOpeningPoly, Joined_Polys, dTol);
		if (Joined_Polys.size() == 1)
		{
			mp_Profile->CopyFrom(Joined_Polys[0]);
			delete pOpening;
			ma_Opening.erase(ma_Opening.begin() + i);
			GMObject::DeleteGMObject(Joined_Polys);
			continue;
		}
	}
}

void MSPolylineObject::MergeOpenings()
{
    if(ma_Opening.size()<2)
        return;

	for(int i = 0 ; i < ma_Opening.size() ; i++)
	{
		MSOpening* pOpening = ma_Opening[i];
		for(int j = ma_Opening.size() -1 ; j >= 0 ; j--)
		{
			MSOpening* pUrOpening = ma_Opening[j];
			if(pOpening->MergeToMe(pUrOpening))
			{
				ma_Opening.erase(ma_Opening.begin() +j);
				delete pUrOpening;
			}
		}
	}

    /*for(int i=0;i<ma_Opening.size()-1;i++)
    {
        int index = i+1;
        while(index<ma_Opening.size())
        {
            if(ma_Opening[i]->MergeToMe(ma_Opening[index]))
            {
				//delete ma_Opening[index];
				ma_Opening.erase(ma_Opening.begin() + index );
			}
            else
                index++;
        }
    }*/
}

void MSPolylineObject::RemoveSameOpening()
{
	for (int i = ma_Opening.size() - 1; i >= 0; i--)
	{
		MSOpening* pOpening = ma_Opening[i];
		if (pOpening == nullptr) continue;

		for (int j = i-1 ; j >=0 ; j--)
		{
			MSOpening* pJOpening = ma_Opening[j];
			if (pOpening->IsSameOpening(pJOpening))
			{
				delete pOpening;
				pOpening = nullptr;
				ma_Opening.erase(ma_Opening.begin() + i);
				break;
			}
		}
		
		if (pOpening == nullptr) continue;
		// 잘못들어간 오프닝 삭제하기 위해 사용함
		GM2DVector cenVec = pOpening->GetProfile()->MassCenter();
		if (mp_Profile->Contains(&cenVec) > 0)
		{
			delete pOpening;
			pOpening = nullptr;
			ma_Opening.erase(ma_Opening.begin() + i);
			continue;
		}
	}
}


MSFloor* MSPolylineObject::GetFloor()
{
	MSFloor* pFloor = NULL;
	Type_MSCompoG nOwnerType = GetOwnerType();
	if(nOwnerType == MSElement::msSlabG)
	{
		MSSlabG* pSlab = (MSSlabG*)GetOwner();
		pFloor = (MSFloor*)pSlab->GetOwner();
		if(pFloor->GetType() != MSElement::msFloor)
			return NULL;
	}
	else if(nOwnerType == MSElement::msLevelZone)
	{
		MSLevelZone* pLevelZone = (MSLevelZone*)GetOwner();
		pFloor = (MSFloor*)pLevelZone->GetOwner();
		if(pFloor->GetType() != MSElement::msFloor)
			return NULL;
	}
	else if(nOwnerType == MSElement::msOpening)
	{
		MSOpening* pOpening = (MSOpening*)GetOwner();
		MSElement* pPrentElement = (MSElement*)pOpening->GetOwnerCompoG();
		pFloor = pPrentElement->GetFloor();
	}
	else if(nOwnerType == MSElement::msWell)
	{
		MSWell* pWell = (MSWell*)GetOwner();
		MSPolylineObject* pPrentPoly = (MSPolylineObject*)pWell->GetOwner();
		pFloor = pPrentPoly->GetFloor();
	}
	else if(nOwnerType == MSElement::msTrench)
	{
		MSTrench* pTrench = (MSTrench*)GetOwner();
		MSPolylineObject* pPrentPoly = (MSPolylineObject*)pTrench->GetOwner();
		pFloor = pPrentPoly->GetFloor();
	}
	else if (nOwnerType == MSElement::msConstructionZone)
	{
		MSConstructionZone* pCZone = (MSConstructionZone*)GetOwner();
		pFloor = pCZone->GetFloor();
	}
	return pFloor;
}

bool MSPolylineObject::AddOpeningInEdgeSlab(MSSlabG* pMySlabG, MSSlabG* pUrSlabG)
{
	MSConnection* pCon = pMySlabG->GetConnection(pUrSlabG);
	if (pCon == nullptr)
		return false;

	vector<GM2DPolyline*> IntersectPolys = pCon->GetIntersects();
	if (IntersectPolys.size() == 0)
		return false;

	vector<GM2DCurve*> CurveArr = pMySlabG->GetProfile()->GetColinearCurve(pUrSlabG->GetProfile(), DTOL_GM, DTOL_GM);
	bool HasColinearCurve = false;
	if (CurveArr.size() > 0)
		HasColinearCurve = true;

	for (int iCurve = 0; iCurve < CurveArr.size(); iCurve++)
		delete CurveArr[iCurve];

	if (HasColinearCurve == false)
		return false;

	bool IsMyProfileInUrProfile = pMySlabG->GetProfile()->Contains(pUrSlabG->GetProfile(), true, DTOL_GM);
	if (IsMyProfileInUrProfile == true)
	{
		pMySlabG->mp_PolylineInfo->AddOpening(IntersectPolys[0], pUrSlabG);
		return true;
	}
	else
	{
		bool IsUrProfileInMyProfile = pUrSlabG->GetProfile()->Contains(pMySlabG->GetProfile(), true, DTOL_GM);
		if (IsUrProfileInMyProfile == true)
		{
			pUrSlabG->mp_PolylineInfo->AddOpening(IntersectPolys[0], pMySlabG);
			return true;
		}
	}
	return false;
}

void MSPolylineObject::TrimExistPolyLineObject(bool isTop)
{
	ma_UpdateMSPolylineObject.clear();

	MSFloor *pFloor = GetFloor();
	if(pFloor == NULL) return;

	InitTrimmedBoundary(isTop);
	GM2DPolyline* pMyProfile = GetTrimmedBoundary(isTop);

	if (!pMyProfile->IsClosed())
	{
		ASSERT(FALSE);
		pMyProfile->CopyFrom(GetOwnerCompoG()->GetProfile());
	}

	Type_MSCompoG eMyOwnerType = this->GetOwnerType();
	
	vector<MSPolylineObject*> PolylineObjectArr;
	pFloor->GetPolylineObjectArr(PolylineObjectArr, GetOwnerType()); 

	long nPolylineObject = PolylineObjectArr.size();
	for(long iPolylineObject=0; iPolylineObject < nPolylineObject; iPolylineObject++)
	{
		MSPolylineObject* pMyPolylineObject = this;
		MSPolylineObject* pUrPolylineObject = (MSPolylineObject*)(PolylineObjectArr[iPolylineObject]);
		if(pUrPolylineObject->GetOwner()->m_ID == GetOwner()->m_ID) 
			continue;

		MSConstructionZone* myCZone = dynamic_cast<MSConstructionZone*>(pMyPolylineObject->mp_Owner);
		MSConstructionZone* urCZone = dynamic_cast<MSConstructionZone*>(pUrPolylineObject->mp_Owner);
		if (myCZone != nullptr && urCZone != nullptr)
		{
			if (myCZone->GetZoneType() != urCZone->GetZoneType())	continue;
		}
		GM2DPolyline* pUrProfile = pUrPolylineObject->GetTrimmedBoundary(isTop);

        BOOL bRval = pMyProfile->OverlapEx(*pUrProfile);
        if(bRval == FALSE)											
            continue;

		bool bOverlap = pMyPolylineObject->LevelOverlap(pUrPolylineObject, isTop);
		if (bOverlap == false)
			continue;

		Type_MSCompoG eUrOwnerType = pUrPolylineObject->GetOwnerType();


		if (eMyOwnerType == MSElement::msSlabG && eUrOwnerType == MSElement::msSlabG)
		{
			// TopZLevel 비교, pUrSlabG가 더 높은경우 pMySlabG의 프로파일을 변경한다.
			MSSlabG* pMySlabG = (MSSlabG*)pMyPolylineObject->GetOwner();
			MSSlabG* pUrSlabG = (MSSlabG*)pUrPolylineObject->GetOwner();

			if(pMySlabG->IsSlopeMember() || pUrSlabG->IsSlopeMember())
				continue;

			MSConnection* pConn = pMySlabG->GetConnection(pUrSlabG);
			if (pConn != nullptr && isTop && pConn->GetFirstCompoG() == pMySlabG)
			{
				pMySlabG->GetProfile()->BoolOperation('I', pUrSlabG->GetProfile(), pConn->GetIntersects(), DTOL_GM);
				pConn->CheckIntersect();
				pMyProfile->SegmentstoMe(pUrProfile);
			}

			double MyTopLevel, MyBotLevel;
			double UrTopLevel, UrBotLevel;
			if (!pMySlabG->GetZLevelsWidthOutZLevel(MyTopLevel, MyBotLevel))
				ASSERT(0);
			if (!pUrSlabG->GetZLevelsWidthOutZLevel(UrTopLevel, UrBotLevel))
				ASSERT(0);
			
			if (isTop)
			{
				if (AddOpeningInEdgeSlab(pMySlabG, pUrSlabG) == true)
					continue;
			}

			if (fabs(MyTopLevel - UrTopLevel) < DTOL_GM && fabs(MyBotLevel - UrBotLevel) < DTOL_GM)
			{
				if (MSElement::CompareElementPriority(pMyPolylineObject->mp_Owner, pUrPolylineObject->mp_Owner))
					std::swap(pMyPolylineObject, pUrPolylineObject);
				else
					continue;
			}
			else
			{
				if (isTop)
				{
					if (pUrProfile->Contains(pMyProfile) == false)
					{
						/*
						if (MyTopLevel > UrTopLevel)
							continue;
						if (MSElement::CompareElementPriority(pMyPolylineObject->mp_Owner, pUrPolylineObject->mp_Owner))
							std::swap(pMyPolylineObject, pUrPolylineObject);
						else
							continue;
							*/
						if (MSElement::CompareElementPriority(pMyPolylineObject->mp_Owner, pUrPolylineObject->mp_Owner))
							std::swap(pMyPolylineObject, pUrPolylineObject);
					}
				}
				else
				{
					/*
					if (MyBotLevel < UrBotLevel)
						continue;
					if (MSElement::CompareElementPriority(pMyPolylineObject->mp_Owner, pUrPolylineObject->mp_Owner))
						std::swap(pMyPolylineObject, pUrPolylineObject);
					else
						continue;
						*/
					if (MSElement::CompareElementPriority(pMyPolylineObject->mp_Owner, pUrPolylineObject->mp_Owner))
						std::swap(pMyPolylineObject, pUrPolylineObject);
				}
			}
		}
		else
		{
			if (MSElement::CompareElementPriority(pMyPolylineObject->mp_Owner, pUrPolylineObject->mp_Owner))
			{
				if ((eMyOwnerType == MSElement::msConstructionZone && eUrOwnerType == MSElement::msConstructionZone))
					std::swap(pMyPolylineObject, pUrPolylineObject);
			}
		}

		// 슬래브 두께가 중첩 되었을경우에 처리

		if(eUrOwnerType == MSElement::msSlabG)
		{
			MSSlabG* pUrSlab = (MSSlabG*)pUrPolylineObject->GetOwner();
			if(pUrSlab->GetContainingElementId() == GetOwner()->m_ID)
				continue;
		}

		if (pUrPolylineObject->GetOwner()->GetType() == MSElement::msSlabG)
			MSFloor::CalculateUpdatedCompoGArray((MSCompoG*)pUrPolylineObject->GetOwner(), MSFloor::CompoGIDList_NeedRegen_CurTR(), false);

		//완전 포함된 슬래브 끼리 오프닝은 머지시키지 않는다. //start
// 		GM2DPolyline* pMyPoly = pMyPolylineObject->GetProfile();
// 		GM2DPolyline* pCurPoly = pUrPolylineObject->GetProfile();
		GM2DPolyline* pMyPoly = pMyPolylineObject->GetTrimmedBoundary(isTop);
		GM2DPolyline* pCurPoly = pUrPolylineObject->GetTrimmedBoundary(isTop);

		bool IsContain = pMyPoly->Contains(pCurPoly);

		//pCurPolylineObject inner 폴리곤 Contains					
		if (isTop == true)
		{
			for (long iInner = pUrPolylineObject->ma_Opening.size() - 1; iInner >= 0; iInner--)
			{
				MSOpening* pCurOpening = pUrPolylineObject->ma_Opening[iInner];
				
				if(AppGlobalOptionWrapper::Instance()->GetAppMode() == AppGlobalOptionWrapper::Formwork && 
					pCurOpening->IsMadeByCompoG() == false)
						continue;

				if (!IsContain)
				{
					TrimExistPolyLineObjectSetp2(pCurOpening->mp_PolylineInfo, pMyPolylineObject, isTop, true);
				}
			}
		}

		//완전 포함된 슬래브 끼리 오프닝은 머지시키지 않는다. //end
		
		TrimExistPolyLineObjectSetp2(pUrPolylineObject, pMyPolylineObject, isTop, false);

		//LevelZone의 경우 반대도 해준다.
		if (eMyOwnerType == MSElement::msLevelZone && 
			eUrOwnerType == MSElement::msLevelZone)
			TrimExistPolyLineObjectSetp2(pMyPolylineObject, pUrPolylineObject, isTop, true);
	}

// 	GM2DPolyline* pMyPoly = GetProfile();
// 	if(eMyOwnerType == MSElement::msLevelZone)
// 	{
// 		pFloor->GetPolylineObjectArr(PolylineObjectArr, MSElement::msLevelZone); 
// 		nPolylineObject = PolylineObjectArr.size();
// 		for(long iPolylineObject=0; iPolylineObject < nPolylineObject; iPolylineObject++)
// 		{
// 			MSPolylineObject* pUrPolylineObject = (MSPolylineObject*)(PolylineObjectArr[iPolylineObject]);
// 			
// 			GM2DPolyline* pUrPoly = pUrPolylineObject->GetProfile();
// 			pMyPoly->SegmentstoMe(pUrPoly);
// 			
// 			long nOldNumLines = pUrPoly->GetNumLines();
// 			pUrPoly->SegmentstoMe(pMyPoly);
// 			long nCurNumLines = pUrPoly->GetNumLines();
// 
// 			if(nOldNumLines < nCurNumLines)
// 				ma_UpdateMSPolylineObject.push_back(pUrPolylineObject);
// 		}
// 	}
	
	PolylineObjectArr.clear();

	//??? by LSS
	// 자신의 오프닝도 트림
	if (isTop == true)
	{
		long NumOpening = ma_Opening.size();
		for (long iOpening = NumOpening - 1; iOpening >= 0; iOpening--)
		{
			MSOpening* pCurOpening = ma_Opening[iOpening];
			if (AppGlobalOptionWrapper::Instance()->GetAppMode() == AppGlobalOptionWrapper::Formwork)
			{
				if(pCurOpening->IsMadeByCompoG() == false)
					continue;
			}

			if (!pCurOpening->GetProfile()->IsClosed())
			{
				ASSERT(FALSE);
			}
			bool bRval = TrimExistPolyLineObjectSetp2(pCurOpening->mp_PolylineInfo, this, isTop, true);
			if (bRval == false)
			{
				ma_Opening.erase(ma_Opening.begin() + iOpening);
				delete pCurOpening;
			}
			else
			{
				pMyProfile->SegmentstoMe(pCurOpening->GetProfile(), DTOL_GM);
			}
		}
	}
}

// 우선 Slab에 대한 Trim 로직만 GM Boolean -> ACIS Boolean으로 바꿈.
// 테스트 후 주석 처리부분 지울 예정.
bool MSPolylineObject::TrimExistPolyLineObjectSetp2(MSPolylineObject* pCurPolylineObject, MSPolylineObject* pUrPolylineObject, bool isTop, bool bUpdatePartner)
{
	vector<GM2DPolyline*> Joined_Polys;
	MSFloor* pCurFloor = pCurPolylineObject->GetFloor();
	MSElement* pCurElement = (MSElement*)pCurPolylineObject->GetOwner();
	Type_MSCompoG pCurType = pCurPolylineObject->GetOwnerType();
	Type_MSCompoG pUrType = pUrPolylineObject->GetOwnerType();

// 	GM2DPolyline* pCurPolyLine = pCurPolylineObject->mp_Profile;
// 	GM2DPolyline* pUrPolyLine = pUrPolylineObject->mp_Profile;
	GM2DPolyline* pCurPolyLine = pCurPolylineObject->GetTrimmedBoundary(isTop);
	GM2DPolyline* pUrPolyLine = pUrPolylineObject->GetTrimmedBoundary(isTop);

	BOOL isAddPoly = FALSE;
	double dTol = 1;
	
	if(pCurType == pUrType || pCurType == MSElement::msOpening )
	{
		BOOL IsOverLap = pCurPolyLine->OverlapEx(*pUrPolyLine, DTOL_GM, FALSE);
		if(IsOverLap)
			pCurPolyLine->BoolOperation('A', pUrPolyLine, Joined_Polys, dTol);
		else
		{
			for (int iJoined = 0; iJoined < Joined_Polys.size(); iJoined++)
				delete Joined_Polys[iJoined];

			// 오프닝이 필요한 경우가 발생함
			// 기존 프로파일의 형상이 여러번 변경되는 경우
			// 입력된 프로파일에 포함되는 경우 오프닝을 생성해준다
			if (pUrPolylineObject->mp_Profile->Contains(pCurPolyLine) == true)
				return true;

			return false;
		}
	}
	else
		return true;

	long NumJoinedPolys = Joined_Polys.size();
	bool HasErrorPolyline = false;
	for (int i = 0; i < NumJoinedPolys; i++)
	{
		GM2DPolyline* pJoinedPoly = Joined_Polys[i];
		pJoinedPoly->DeleteInLinePoint();

		bool bRval = pJoinedPoly->CheckPolygon(false, DTOL_GM);
		if (bRval == false)
			HasErrorPolyline = true;
		pJoinedPoly->SegmentstoMe(pCurPolyLine);
		pJoinedPoly->SegmentstoMe(pUrPolyLine);
	}

	if (HasErrorPolyline == true)
	{
		if (isTop == true)
		{
			MSCompoG* pUrCompoG = NULL;
			if (pUrType == MSElement::msSlabG)
				pUrCompoG = (MSCompoG*)pUrPolylineObject->GetOwner();
			pCurPolylineObject->AddOpening(pUrPolyLine, pUrCompoG, false);
		}
	}
	else if(NumJoinedPolys == 1)
	{
// 		if (MSElement::CompareElementPriority(pUrPolylineObject->mp_Owner, pCurPolylineObject->mp_Owner))
// 		{
// 			GMObject::DeleteGMObject(Joined_Polys);
// 			return false;
// 		}

		if(Joined_Polys[0]->IsClosed())					// 닫혀있는지를 확인
		{
// 			if(pCurType == MSElement::msOpening)
// 			{
// 				MSOpening* pOpening = (MSOpening*)pCurPolylineObject->GetOwner();
// 				MSPolylineObject* pPrentPoly = (MSPolylineObject*)pOpening->GetOwner();
// 				if(pPrentPoly->GetOwner() == pPolylineObject->GetOwner())					// 자신의 슬라브 오프닝 처리
// 				{
// 					vector<GM2DPolyline*> Joined_Polys2;
// 					pCurPolyLine->BoolOperation('I', pPolylineObject->mp_Profile, Joined_Polys2, DTOL_GM);
// 					if(Joined_Polys2.size() > 0) // ToDo: 버그 나서 임시 조치
// 						pCurPolyLine->CopyFrom(Joined_Polys2[0]);
// 					else
// 						ASSERT(0); // Error Check
// 				}
// 			}
			//else if(!pCurPolyLine->SamePolyLine(Joined_Polys[0]))
			if(!pCurPolyLine->SamePolyLine(Joined_Polys[0]))
			{

				if (pCurPolylineObject->GetOwnerType() == MSElement::msLevelZone)
					pCurPolylineObject->SetProfile(Joined_Polys[0]);
				else
					pCurPolylineObject->SetTrimmedBoundary(Joined_Polys[0], isTop);

				//나의 폴리라인 오프젝트의 LevelDif는 트림후에 만들어준다.
				if(pCurPolylineObject != pUrPolylineObject)
				{
					bool bMakeSlab = true;
					if(pUrType == MSElement::msLevelZone && pCurType == MSElement::msLevelZone)
					{
						bMakeSlab = false;
						pUrPolylineObject->ma_UpdateMSPolylineObject.push_back(pCurPolylineObject);
					}

					// LevelZone의 LevelDif는 TrimExistPolyLineObjectSetp1 함수가 끝나고나서 만들어준다.
					pCurPolylineObject->ReMakeLevelDif(bMakeSlab);
				}

				if(pCurPolylineObject->GetOwnerType() == MSElement::msSlabG)
				{
					if (isTop == true)
					{
						MSSlabG* pSlab = (MSSlabG*)pCurPolylineObject->GetOwner();

						long NumOpening = pCurPolylineObject->ma_Opening.size();
						for (long iOpening = 0; iOpening < NumOpening; iOpening++)
						{
							MSOpening* pCurOpening = pCurPolylineObject->ma_Opening[iOpening];
							TrimExistPolyLineObjectSetp2(pCurOpening->mp_PolylineInfo, pCurPolylineObject, isTop, true);
						}

						// 오프닝이 필요한 경우가 발생함
						// 기존 프로파일의 형상이 여러번 변경되는 경우
						// 입력된 프로파일에 포함되는 경우 오프닝을 생성해준다
						if (pCurPolylineObject->mp_Profile->Contains(pUrPolyLine) == true)
						{
							pCurPolylineObject->AddOpening(pUrPolyLine, pUrPolylineObject->GetOwner());
						}

					}
				}

				if(pCurType == MSElement::msLevelZone)
					MSFloor::CalcAffected_CompoGID_ByLevelZone(pCurFloor, (MSLevelZone*)pCurPolylineObject->GetOwner(), MSFloor::CompoGIDList_NeedRegen_CurTR());
				else if(pCurType == MSElement::msConstructionZone)
					MSFloor::CompoGIDList_NeedRegen_2DView_CurTR().push_back(pCurElement->m_ID);
			}
		}
		else
		{
			ASSERT(FALSE);
		}
		
	}
	else if(NumJoinedPolys == 2 && pCurType != MSElement::msOpening)
	{
		MSElement* pUrElement = NULL;
		if(pUrType == MSElement::msSlabG || pUrType == MSElement::msConstructionZone)  
			pUrElement = pUrPolylineObject->GetOwner();

		if (pCurPolyLine->Contains(pUrPolyLine) == true)
		{
			if (isTop == true)
			{
				//if (MSElement::CompareElementPriority(pCurPolylineObject->mp_Owner, pUrPolylineObject->mp_Owner))
					pCurPolylineObject->AddOpening(pUrPolyLine, pUrElement);
			}
		}
		else if (pUrPolyLine->Contains(pCurPolyLine) == true)
		{
			if (isTop == true)
			{
				//if (MSElement::CompareElementPriority(pUrPolylineObject->mp_Owner, pCurPolylineObject->mp_Owner))
					pUrPolylineObject->AddOpening(pCurPolyLine, pCurElement);
			}
		}
		else
		{
			if (MSElement::CompareElementPriority(pUrPolylineObject->mp_Owner, pCurPolylineObject->mp_Owner))
			{
				GMObject::DeleteGMObject(Joined_Polys);
				return false;
			}

			vector<GM2DPolyline*> Joined_Polys2;
			AcisGeomHelper::BooleanPolyline('I', *pCurPolyLine, *pUrPolyLine, Joined_Polys2);

			long NumJoinedPolys2 = Joined_Polys2.size();
			if (isTop == true)
			{
				for (long iJoind = 0; iJoind < NumJoinedPolys2; iJoind++)
				{
					bool bRVal = pCurPolylineObject->AddOpening(Joined_Polys2[iJoind], pUrElement, false);
					if (bRVal == false)
						pUrPolylineObject->AddOpening(Joined_Polys2[iJoind], pCurElement, false);
				}
			}

			GMObject::DeleteGMObject(Joined_Polys2);
		}

		if(pCurType == MSElement::msSlabG)
			MSFloor::CalculateUpdatedCompoGArray(pCurElement, MSFloor::CompoGIDList_NeedRegen_CurTR(), bUpdatePartner);
		else if(pCurType == MSElement::msLevelZone)
			MSFloor::CalcAffected_CompoGID_ByLevelZone(pCurFloor, (MSLevelZone*)pCurPolylineObject->GetOwner(), MSFloor::CompoGIDList_NeedRegen_CurTR());
	}
	else if(NumJoinedPolys == 0)
	{
		if(pCurType == MSElement::msOpening)		// 입력 받은 PolyObject 가 CurPolyobject를 완전 포함함 삭제 해줘야함
		{
			MSOpening* pOpening = (MSOpening*)pCurPolylineObject->GetOwner();
			MSElement* pPartner = (MSElement*)pOpening->GetOwnerCompoG();
			MSPolylineObject* pPrentPoly = (MSPolylineObject*)pPartner->mp_PolylineInfo;

			if (AppGlobalOptionWrapper::Instance()->GetAppMode() == AppGlobalOptionWrapper::Rebar)
			{
				if (pPrentPoly != pUrPolylineObject)
				{
					if (pPrentPoly->RemoveOpening((MSOpening*)pCurPolylineObject->GetOwner()))
						pCurPolylineObject = NULL;
				}
			}
			else if (AppGlobalOptionWrapper::Instance()->GetAppMode() == AppGlobalOptionWrapper::Formwork)
			{
				if (pPrentPoly != pUrPolylineObject && pOpening->IsMadeByCompoG())
				{
					if (pPrentPoly->RemoveOpening((MSOpening*)pCurPolylineObject->GetOwner()))
						pCurPolylineObject = NULL;
				}
			}
		}
		else
		{	
			MSElement* pUrElement = NULL;
			if (pUrType == MSElement::msSlabG || pUrType == MSElement::msConstructionZone)
				pUrElement = pUrPolylineObject->GetOwner();
			if (isTop == true)
			{
				if (pCurPolyLine->Contains(pUrPolyLine) == true)
				{
					//if (MSElement::CompareElementPriority(pCurPolylineObject->mp_Owner, pUrPolylineObject->mp_Owner))
						pCurPolylineObject->AddOpening(pUrPolyLine, pUrElement);
				}
				else if (pUrPolyLine->Contains(pCurPolyLine) == true)
				{
					//if (MSElement::CompareElementPriority(pUrPolylineObject->mp_Owner, pCurPolylineObject->mp_Owner))
						pUrPolylineObject->AddOpening(pCurPolyLine, pCurElement);
				}
			}
		}
	}

	for(int iJoined = 0 ; iJoined < NumJoinedPolys ; iJoined++)
		delete Joined_Polys[iJoined];

	Joined_Polys.clear();

	if(NumJoinedPolys > 0)
	{
// 		if(pPolylineObject != NULL)
// 			pPolylineObject->GetProfile()->MergeVectorArr(1);
// 		if(pCurPolylineObject != NULL)
// 			pCurPolylineObject->GetProfile()->MergeVectorArr(1);
		// 기존 BoolOperation결과 값이 Close되지 않은 폴리 라인이 나오는 경우가 발생함
		// ==> ACIS BoolOperation으로 변경 후 필요 없는 코드임
		/*
		if(pPolylineObject != NULL)
			pPolylineObject->GetProfile()->CheckVecterArr(true, 1);
		if(pCurPolylineObject != NULL)
			pCurPolylineObject->GetProfile()->CheckVecterArr(true, 1);
		*/
	}

	return true;
}

void MSPolylineObject::CalcBoundRectBeforeAddQuadTree()
{
	MSElement* pElement = GetOwner();
	GM2DBound MyBound;
	pElement->GetProfile()->Get2DBound(MyBound);
	GM2DVector* pOrigin = MyBound.GetBottomLeft();
	GM2DVector* pCorner = MyBound.GetTopRight();

	if (pOrigin == NULL || pCorner == NULL) return;
	m_RectForQuadTree.m_Origin.m_X = pOrigin->m_X; m_RectForQuadTree.m_Origin.m_Y = pOrigin->m_Y;
	m_RectForQuadTree.m_Corner.m_X = pCorner->m_X; m_RectForQuadTree.m_Corner.m_Y = pCorner->m_Y;
}

bool MSPolylineObject::VisibleInRect(GM2DRectangle* pRect)
{
	GM2DPolyline* profile = GetProfile();

	if (m_RectForQuadTree.Right() < pRect->Left()) return FALSE;
	else if (m_RectForQuadTree.Left() > pRect->Right()) return FALSE;
	if (m_RectForQuadTree.Top() < pRect->Bottom()) return FALSE;
	else if (m_RectForQuadTree.Bottom() > pRect->Top()) return FALSE;

	if (pRect->Left() <= m_RectForQuadTree.Left() && m_RectForQuadTree.Right() <= pRect->Right())
	{
		if (pRect->Bottom() <= m_RectForQuadTree.Bottom() && m_RectForQuadTree.Top() <= pRect->Top())
			return TRUE;
	}

	GM2DPolyline UrPoly(TRUE);
	pRect->GetBoundPolyline(UrPoly);
	if(profile->OverlapEx(UrPoly))	return true;

	return false;
}

MSOpening* MSPolylineObject::FindOpening_MadeByCompoG(int nCompoGID)
{
	for (int iOpening = 0; iOpening < ma_Opening.size(); iOpening++)
	{
		MSOpening* pCurOpening = ma_Opening[iOpening];
		if (pCurOpening->HasMadeByCompoGID(nCompoGID))
			return pCurOpening;
	}
	return nullptr;
}

vector<GM2DPolyline*> MSPolylineObject::GetMergeOpeningProfiles()
{
	vector<GM2DPolyline*> poly_in;
	vector<GM2DPolyline*> poly_out;

	for (int i = 0; i < ma_Opening.size(); i++)
	{
		GM2DPolyline* pPoly = ma_Opening[i]->GetProfile();
		if(pPoly == nullptr || pPoly->CheckPolygon(false, DTOL_GM) == FALSE)
			continue;
		poly_in.push_back(pPoly);
	}

	AcisGeomHelper::UnionPolylines(poly_in, poly_out);

	return poly_out;
}
