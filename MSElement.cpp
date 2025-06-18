#include "StdAfx.h"
#include "MSCompoG.h"
#include "MSElement.h"
#include "GMLib\GM2DPolyline.h"
#include "MSPolylineObject.h"
#include "MSOpening.h"
#include "MSFloorQT.h"
#include "MSFloor.h"
#include "MSModelChecker.h"
#include "MSRebarBuilding.h"

IMPLEMENT_SERIAL(MSElement, MSObject, VERSIONABLE_SCHEMA | 1)
#define YEAR_START 2000

long MSElement::ms_LastID = 0;

MSElement::MSElement()
{
	mp_PolylineInfo = NULL;
	Init(true);
}

MSElement::~MSElement(void)
{
    if(mp_PolylineInfo)
        delete mp_PolylineInfo;
}

// m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
long MSElement::GetID()
{
	if(m_ID == 0) // 
	{
		m_ID = MSBaseBuilding::GetUID(UIDType::msElement);
		ms_LastID = m_ID;
		return m_ID;
	}

	return m_ID;
}

long MSElement::ResetGetID()
{
	m_ID = 0;
	return GetID();
}
void MSElement::SetLastID( long NewID )
{
	if(NewID > ms_LastID)
		ms_LastID = NewID;
}

MSObject* MSElement::Copy( bool bNewID /*= true*/ )
{
	MSElement* pNewObj;

	pNewObj = new MSElement();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSElement::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/ )
{
	if(pSource_in == nullptr) return;

	if( !pSource_in->IsElement() ) return;
	MSElement* pSource = (MSElement*)pSource_in;

	if(pSource->mp_PolylineInfo != NULL)
	{
		if(mp_PolylineInfo != NULL) delete mp_PolylineInfo;
		mp_PolylineInfo = (MSPolylineObject*)pSource->mp_PolylineInfo->Copy(false);
		mp_PolylineInfo->mp_Owner = this;
	}

	if(bNewID)
		GetID();	

	m_Note = pSource->GetNote();
	m_IsDummy = pSource->GetDummy();
	//m_nContainingElementId = pSource->m_nContainingElementId;
}

void MSElement::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_ID;

		ar << m_nContainingElementId;
		ar << m_Note;
		ar << m_IsDummy;
	}
	else 
	{
		ar >> m_ID;

		SetLastID(m_ID);

		if (MSVersionInfo::GetCurrentVersion() >= 20161230)
			ar >> m_nContainingElementId;
		if(IsMVAddToBHA())
		{
			if (MSVersionInfo::GetCurrentVersion() >= 20180611)
			{
				ar >> m_Note;

				if (m_Note != _T(""))
					m_Note = m_Note.Trim();

				if (MSVersionInfo::GetCurrentVersion() < 20181023)
				{
					MSBaseBuilding* pBuild = GetBuilding();
					if (pBuild != nullptr)
						pBuild->AddNote(m_Note);
				}
			}

			if (MSVersionInfo::GetCurrentVersion() >= 20200909)
			{
				ar >> m_IsDummy;
			}
		}
	}
}

void MSElement::Dump(EFS::EFS_Buffer& buffer)
{
	buffer << m_ID;
	buffer << m_nContainingElementId;
	buffer << m_Note;
	buffer << m_IsDummy;
}

void MSElement::Recovery(EFS::EFS_Buffer& buffer)
{
	buffer >> m_ID;
	buffer >> m_nContainingElementId;
	buffer >> m_Note;
	buffer >> m_IsDummy;
}

void MSElement::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSObject::Init(bInConstructor);

	m_ID = 0;
	m_Note = _T("");
	if(mp_PolylineInfo)
		delete mp_PolylineInfo;
    mp_PolylineInfo = NULL;
	m_nContainingElementId = 0;
	m_IsDummy = false;
	m_isCalculateUpdatedCompoGArray = true;
}

bool MSElement::IsLineSupportMember()
{
	return MSElement::IsLineSupportMember(GetType());
}

bool MSElement::IsFloorMember()
{
	return MSElement::IsFloorMember(GetType());
}

bool MSElement::IsSubMember()
{
	return MSElement::IsSubMember(GetType());
}

bool MSElement::IsWallMember()
{
	return MSElement::IsWallMember(GetType());
}

bool MSElement::IsVerticalMember()
{
	return MSElement::IsVerticalMember(GetType());
}

bool MSElement::IsLinkMember()
{
	return MSElement::IsLinkMember(GetType());
}

bool MSElement::IsPointMember()
{
	return MSElement::IsPointMember(GetType());
}

bool MSElement::IsPolyMember()
{
	return MSElement::IsPolyMember(GetType());
}

bool MSElement::IsLowWall()
{
	return MSElement::IsLowWall(GetType());
}

bool MSElement::IsWallOpeningG()
{
	return MSElement::IsWallOpeningG(GetType());
}

bool MSElement::IsLineSupportMember(Type_MSCompoG eType)
{
	switch (eType)
	{
	case msBeamG:
	case msBuiltUpBeam:
	case msWallFootG:
	case msLineLevelDifG:
	case msSubHaunchG:
	case msHandRailG:
	case msHaunchG:
		return true;
	default:
		return false;
	}
	return false;
}

bool MSElement::IsFloorMember(Type_MSCompoG eType)
{
	switch (eType)
	{
	case msBeamG:
	case msWallFootG:
	case msLineLevelDifG:
	case msSubHaunchG:
	case msHandRailG:
	case msIsoFootG:
	case msSlabG:
	case msPitG:
	case msOpeningG:
	case msConstructionZone:
	case msBuiltUpBeam:
	case msHaunchG:
		return true;
	default:
		return false;
	}
	return false;
}

bool MSElement::IsSubMember(Type_MSCompoG eType)
{
	switch (eType)
	{
	case msLineLevelDifG:
	case msWell:
	case msTrench:
	case msOpening:
	case msWindowG:
	case msDoorG:
	case msWallOpeningExtraG:
	case msStairPart:
	case msSubHaunchG:
	case msBuiltUpBeam:
	case msAddRebarAreaLine:
	case msAddRebarAreaBent:
	case msAddRebarAreaShape:
		return true;
	default:
		return false;
	}

	return false;
}

bool MSElement::IsWallMember(Type_MSCompoG eType)
{
	switch (eType)
	{
	case msWallG:
		return true;
	default:
		return false;
	}
	return false;
}

bool MSElement::IsVerticalMember(Type_MSCompoG eType)
{
	switch (eType)
	{
	case msWallG:
	case msColumnG:
	case msWallColumnG:
	case msStairG:
	case msStairPart:
		return true;
	default:
		return false;
	}
	return false;
}

bool MSElement::IsLowWall(Type_MSCompoG eType)
{
	if (eType == MSElement::msHandRailG)
		return true;
	else
		return false;
}

bool MSElement::IsLinkMember(Type_MSCompoG eType)
{
	if (MSElement::IsLineSupportMember(eType) || MSElement::IsWallMember(eType))
		return true;
	return false;
}

bool MSElement::IsPointMember(Type_MSCompoG eType)
{
	switch (eType)
	{
	case msIsoFootG:
	case msColumnG:
	case msWallColumnG:
		return true;
	default:
		return false;
	}
	return false;
}

bool MSElement::IsVerticalLineMember(Type_MSCompoG eType)
{
	switch (eType)
	{
	case msColumnG:
	case msWallColumnG:
		return true;
	default:
		break;
	}
	return false;
}

bool MSElement::IsPolyMember(Type_MSCompoG eType)
{
	switch (eType)
	{
	case msSlabG:
	case msPitG:
	case msStairG:
	case msStairPart:
	case msOpeningG:
	case msConstructionZone:
	case msLevelZone:
	case msSlabRebar:
	case msFWSoundAbsorptionG:
		return true;
	default:
		return false;
	}
	return false;
}

bool MSElement::IsWallOpeningG(Type_MSCompoG eType)
{
	switch (eType)
	{
	case msOpening:
	case msWindowG:
	case msDoorG:
		return true;
	default:
		return false;
	}
	return false;
}

bool MSElement::CompareElementPriority(MSElement* pMyElement, MSElement* pUrElement)
{
	Type_MSCompoG eMyOwnerType = pMyElement->GetType();
	Type_MSCompoG eUrOwnerType = pUrElement->GetType();

	if (eMyOwnerType == MSElement::msSlabG && eUrOwnerType == MSElement::msSlabG)
	{
		MSSlabG* pMySlabG = (MSSlabG*)pMyElement;
		MSSlabG* pUrSlabG = (MSSlabG*)pUrElement;
		MSConnection* pConn = pMySlabG->GetConnection(pUrSlabG);
		if (pConn != nullptr && pConn->GetFirstCompoGID() == pUrSlabG->m_ID)
			return true;
		else
			return false;
	}
	else if (eMyOwnerType == MSElement::msConstructionZone && eUrOwnerType == MSElement::msConstructionZone)
	{
		if (pMyElement->GetPriorityNum() > pUrElement->GetPriorityNum())
			return true;
	}
	return false;
}

GM2DPolyline* MSElement::GetProfile()
{
    if (mp_PolylineInfo == NULL)
	{        
		return NULL;
	}
    if (mp_PolylineInfo->GetProfile() == NULL)
        ASSERT(0);
    return mp_PolylineInfo->GetProfile();
}

void MSElement::SetProfile(GM2DPolyline* pPoly)
{
	GM2DPolyline* pProfile = GetProfile();
	if (pProfile != nullptr || pProfile != pPoly)
		pProfile->CopyFrom(pPoly);
}

void MSElement::DelDuplicatedPointOfProfile()
{
	if (mp_PolylineInfo == NULL) return;
	
	GM2DPolyline* poly = mp_PolylineInfo->mp_Profile;
	poly->DeleteDuplicatedPoints();
	return;

// 	if(poly != NULL)
// 	{
// 		for ( int i = poly->ma_Point.GetSize()-2 ; i >= 0 ; --i )
// 		{
// 			GM2DVector* v1 = poly->ma_Point[i];
// 			GM2DVector* v2 = poly->ma_Point[i+1];
// 			if( fabs(v1->m_X - v2->m_X) < DTOL_GM && fabs(v1->m_Y - v2->m_Y) < DTOL_GM)
// 			{
// 				poly->ma_Point.RemoveAt(i);
// 				poly->ma_Bulge.RemoveAt(i);
// 			}
// 		}
// 	}
// 
// 	for ( int ii=0 ; ii<mp_PolylineInfo->ma_Opening.size() ; ++ii)
// 	{
// 		GM2DPolyline* poly = mp_PolylineInfo->ma_Opening[ii]->GetProfile();
// 		if(poly != NULL)
// 		{
// 			for ( int i = poly->ma_Point.GetSize()-2 ; i >= 0 ; --i )
// 			{
// 				GM2DVector* v1 = poly->ma_Point[i];
// 				GM2DVector* v2 = poly->ma_Point[i+1];
// 				if( fabs(v1->m_X - v2->m_X) < DTOL_GM && fabs(v1->m_Y - v2->m_Y) < DTOL_GM)
// 				{
// 					poly->ma_Point.RemoveAt(i);
// 					poly->ma_Bulge.RemoveAt(i);
// 				}
// 			}
// 		}
// 	}
	
}

void MSElement::Translate( GM2DVector &pDelta )
{
	if(mp_PolylineInfo)
	{
		GM2DPolyline* pPoly = GetProfile();
		if(!pPoly)
			return;
		pPoly->Translate(pDelta);

        // for slab opening
        int openingCount = mp_PolylineInfo->ma_Opening.size();
        for(int iopening = 0; iopening < openingCount; iopening++)
        {
            MSOpening* pOpening = mp_PolylineInfo->ma_Opening[iopening];
            pOpening->Translate(pDelta);
        }
	}
}

void MSElement::MirrorByAxis( GM2DLineVector& pAxis )
{
	if(mp_PolylineInfo)
	{
		GM2DPolyline* pPoly = GetProfile();
		if(!pPoly)
			return;
		pPoly->MirrorByAxis(pAxis);

        // for slab opening
        int openingCount = mp_PolylineInfo->ma_Opening.size();
        for(int iopening = 0; iopening < openingCount; iopening++)
        {
            MSOpening* pOpening = mp_PolylineInfo->ma_Opening[iopening];
            pOpening->MirrorByAxis(pAxis);
        }
	}
}

void MSElement::RotateZ( double theta, GM2DVector *pCenter )
{
	if(mp_PolylineInfo)
	{
		GM2DPolyline* pPoly = GetProfile();
		if(!pPoly)
			return;
		pPoly->RotateZ(theta, pCenter);

        // for slab opening
        int openingCount = mp_PolylineInfo->ma_Opening.size();
        for(int iopening = 0; iopening < openingCount; iopening++)
        {
            MSOpening* pOpening = mp_PolylineInfo->ma_Opening[iopening];
            pOpening->RotateZ(theta, pCenter);
        }
	}
}

void MSElement::AddFloorQT( MSFloorQT* pQT )
{
	if(!IsCompoG())
		return;

	// This가 QT에 들어가있는대 pFloor->mm_CompoG에 없는 경우가 발생
	// 1. Connection 복사, 2. 부재 입력시 Connection을 가지고 입력하는 경우 3. 기타등등
	MSFloor* pFloor = GetFloor();
	if (pFloor != nullptr && 
		pFloor->mm_CompoG.find(m_ID) == pFloor->mm_CompoG.end())
		return;

	if(IsVerticalMember())
		pQT->AddVerticalMem((MSCompoG*)this);
	else
		pQT->AddFloorMem((MSCompoG*)this);

	long NumQT = ma_CrossQT.GetSize();
	for(long iQT = 0 ; iQT <NumQT ; iQT++)
	{
		MSFloorQT* pCurQT = ma_CrossQT[iQT];
		if(pCurQT == pQT)
			return;
	}

	ma_CrossQT.Add(pQT);
}

bool MSElement::IsCompoG()
{
	return MSElement::IsCompoG(GetType());
}

FindType_MSElement MSElement::GetFindType()
{
	return (FindType_MSElement)GetType(); 
};

bool MSElement::IsSameFindType(MSElement* pElement)
{
	if(pElement == nullptr)
		return false;
	return MSElement::IsSameFindType(GetFindType(), pElement->GetFindType());
}

bool MSElement::IsSameFindType(FindType_MSElement eType)
{
	return MSElement::IsSameFindType(GetFindType(), eType);
}

bool MSElement::IsSameFindType(FindType_MSElement eType1, FindType_MSElement eType2)
{
	if (eType1 == eType2)
		return true;

	if (eType1 >= msFindEum && eType2 >= msFindEum)
		return false;

	if (eType2 >= msFindEum)
		std::swap(eType1, eType2);

	if (eType1 == msPointMember && MSElement::IsPointMember((Type_MSCompoG)eType2))
		return true;

	if (eType1 == msVerticalLineMember && MSElement::IsVerticalLineMember((Type_MSCompoG)eType2))
		return true;

	if (eType1 == msLinkMember && MSElement::IsLinkMember((Type_MSCompoG)eType2))
		return true;
	
	if (eType1 == msPolyMember && MSElement::IsPolyMember((Type_MSCompoG)eType2))
		return true;

	if (eType1 == msFloorPolyMember && MSElement::IsPolyMember((Type_MSCompoG)eType2) && MSElement::IsFloorMember((Type_MSCompoG)eType2))
		return true;
	
	if (eType1 == msVerticalPolyMember && MSElement::IsPolyMember((Type_MSCompoG)eType2) && MSElement::IsVerticalMember((Type_MSCompoG)eType2))
		return true;
	
	if (eType1 == msSlabOrOpening && (MSElement::msSlabG == eType2 || MSElement::msOpeningG == eType2))
		return true;		

	return false;
}

bool MSElement::IsCompoG(Type_MSCompoG eType)
{
	bool bRavl = false;
	switch (eType)
	{
	case MSElement::msNULL:
	case MSElement::msBuilding:
	case MSElement::msFloor:
		bRavl = false;
		break;
	case MSElement::msBeamG:
	case MSElement::msBraceG:
	case MSElement::msWallG:
	case MSElement::msColumnG:
	case MSElement::msWallColumnG:
	case MSElement::msIsoFootG:
	case MSElement::msRampG:
	case MSElement::msSlabG:
	case MSElement::msStairG:
	case MSElement::msPitG:
	case MSElement::msWallFootG:
	case MSElement::msOpeningG:
	case MSElement::msHandRailG:
	case MSElement::msFWSoundAbsorptionG:
	case MSElement::msFWSurfaceFoldingG:
		bRavl = true;
		break;
	case MSElement::msProject:
	case MSElement::msSector:
		bRavl = false;
		break;
	case MSElement::msBeamLine:
	case MSElement::msSlabLineG:
	case MSElement::msLineLevelDifG:
	case MSElement::msSubHaunchG:
	case MSElement::msPointLevelDifG:
	case MSElement::msHaunchG:
	case MSElement::msWallJointG:
		bRavl = true;
		break;
	case MSElement::msConstructionZone:
	case MSElement::msLevelZone:
	case MSElement::msOpening:
	case MSElement::msWindowG:
	case MSElement::msDoorG:
		bRavl = false;
		break;
	case MSElement::msStairPart:
	case MSElement::msSlabRebar:
	case MSElement::msWell:
	case MSElement::msTrench:
		bRavl = false;
		break;
	default:
		bRavl = false;
		break;
	}

	return bRavl;
}

CString MSElement::GetGroupName(Type_MSCompoG eType)
{
	CString groupName = _T("Element");
	switch (eType)
	{
	case MSElement::msBeamG:	groupName = _T("Beam");	break;
	case MSElement::msBraceG:	groupName = _T("Brace");	break;
	case MSElement::msColumnG:	groupName = _T("Column");	break;
	case MSElement::msHandRailG:	groupName = _T("LowWall");	break; // HandRail => LowWall 변경
	case MSElement::msWallG:	groupName = _T("Wall");	break;
	case MSElement::msWallFootG:	groupName = _T("WallFoot");	break;
	case MSElement::msIsoFootG:	groupName = _T("IsoFoot");	break;
	case MSElement::msSlabG:	groupName = _T("Slab");	break;
	case MSElement::msStairG:	groupName = _T("Stair");	break;
	case MSElement::msPitG:	groupName = _T("Pit");	break;
	case MSElement::msRampG:	groupName = _T("Ramp");	break;
	case MSElement::msWindowG:	groupName = _T("Window");	break;
	case MSElement::msDoorG:	groupName = _T("Door");	break;
	case MSElement::msWallOpeningExtraG:	groupName = _T("WallOpeningExtra");	break;
	case MSElement::msWell:	groupName = _T("Well");	break;
	case MSElement::msTrench:	groupName = _T("Trench");	break;
	case MSElement::msOpeningG:	groupName = _T("Opening");	break;
	case MSElement::msLevelZone:	groupName = _T("LevelZone");	break;
	case MSElement::msLineLevelDifG:	groupName = _T("LevelDiff");	break;
	case MSElement::msPointLevelDifG:	groupName = _T("PointLevelDiff");	break;
	case MSElement::msSubHaunchG:	groupName = _T("SubHaunch");	break;
	case MSElement::msBuiltUpBeam:	groupName = _T("BuiltUpBeam");	break;
	case MSElement::msWallColumnG:	groupName = _T("WallColumn");	break;
	case MSElement::msAddRebarAreaLine:	groupName = _T("RebarAreaLine");	break;
	case MSElement::msAddRebarAreaBent:	groupName = _T("RebarAreaBent");	break;
	case MSElement::msAddRebarAreaShape:	groupName = _T("RebarAreaShape");	break;
	case MSElement::msConstructionZone:	groupName = _T("ConstructionZone");	break;
	case MSElement::msSectionView:	groupName = _T("SectionView");	break;
	case MSElement::msMarkG:	groupName = _T("Mark");	break;
	case MSElement::msHaunchG:	 groupName = _T("Haunch");	break;
	case MSElement::msSlabRebar:	 groupName = _T("Slab Add. Bar");	break;
	case MSElement::msArea:	 groupName = _T("Area");	break;
	case MSElement::msPile:	 groupName = _T("Pile");	break;
	case MSElement::msFWSurfaceFoldingG:	 groupName = _T("SurfaceFolding");	break;
	case MSElement::msFWShadeG:	 groupName = _T("Shade");	break;
	case MSElement::msFWSwitchBoxG:	 groupName = _T("SwitchBox");	break;
	case MSElement::msFWSoundAbsorptionG:	 groupName = _T("SoundAbsorption");	break;
	default:
		groupName = _T("Element");
	}
	return LocalDic::GetLocal(groupName);
}

CString MSElement::GetGroupName()
{
	return MSElement::GetGroupName(GetType());
}

MSFloor* MSElement::GetFloorForUser()
{
	MSFloor* pCurFloor = GetFloor();
	if (pCurFloor == NULL)
		return NULL;

	if (IsVerticalMember())
		return pCurFloor;
	else
		return pCurFloor->GetUpperFloor();
}

CString MSElement::GetFloorNameForUser()
{
	CString FloorName;
	MSFloor* pUserFloor = GetFloorForUser();
	if (pUserFloor == NULL)
		return _T("NULL");

	return pUserFloor->GetName();
}

// mylee
int MSElement::GetFloorNumForUser()
{
	MSFloor* pUserFloor = GetFloorForUser();
	if (pUserFloor == NULL)
		return 0;

	return pUserFloor->GetFloorNum();
}

// mylee
int MSElement::GetFloorNum()
{
	MSFloor* pFloor = GetFloor();
	if (pFloor == NULL)
		//return INT_MIN;
		return 0;

	return pFloor->GetFloorNum();
}

bool MSElement::CheckData()
{
	return !MSModelChecker::CheckElement(this);
}

void MSElement::ModifyObject(bool isDoit)
{

}

MSElement* MSElement::GetContainingElement()
{
	MSFloor* pFloor = GetFloor();
	MSBaseBuilding* pBuild = GetBuilding();
	if (pBuild == nullptr)
		return nullptr;

	MSElement* pUrElement = nullptr;
	if (IsCompoG())
	{
		if (pFloor != nullptr)
		{
			pUrElement = (MSElement*)pFloor->GetCompoG(m_nContainingElementId);
			if (pUrElement == nullptr)
				pUrElement = (MSElement*)pBuild->GetCompoGByFloors(m_nContainingElementId);
		}
		else
			pUrElement = (MSElement*)pBuild->GetCompoGByFloors(m_nContainingElementId);
	}
	else
	{
		if(pFloor != nullptr)
			pUrElement = (MSElement*)pFloor->GetCZone(m_nContainingElementId);
	}

	return pUrElement;
}

void MSElement::InitializeParameter()
{
	//다른부재에 의해 만들어진 오프닝만 삭제해준다.
	RemoveOpeningInfoHasMadeByCompoGID();

	MSElement* pUrElement = GetContainingElement();
	if (pUrElement != NULL)
		pUrElement->RemoveOpeningInfo(m_ID);
	m_nContainingElementId = 0;
}

void MSElement::RemoveOpeningInfoHasMadeByCompoGID()
{
	if (mp_PolylineInfo == NULL)
		return;

	MSFloor* pFloor = GetFloor();

	bool IsDeleteOpening = false;
	long NumOpen = mp_PolylineInfo->ma_Opening.size();
	for (long iOpen = NumOpen - 1; iOpen >= 0; iOpen--)
	{
		MSOpening* pOpen = mp_PolylineInfo->ma_Opening[iOpen];
		if (pOpen == NULL) continue;
		for (int i = 0; i < pOpen->ma_MadeByCompoGID.size(); i++)
		{
			MSCompoG* pCompoG = pFloor->GetCompoG(pOpen->ma_MadeByCompoGID[i]);
			if (pCompoG != nullptr && pCompoG->m_nContainingElementId == m_ID)
				pCompoG->m_nContainingElementId = 0;
		}

		if (pOpen->ma_MadeByCompoGID.size() > 0)
		{
			mp_PolylineInfo->ma_Opening.erase(mp_PolylineInfo->ma_Opening.begin() + iOpen);
			delete pOpen;
			IsDeleteOpening = true;
		}
	}

	if (IsDeleteOpening)
		pFloor->CalculateUpdatedCompoGArray(this, pFloor->CompoGIDList_NeedRegen_CurTR());
}

void MSElement::RemoveOpeningInfo(long nCompoGID)
{
	if (mp_PolylineInfo == NULL)
		return;

	bool IsDeleteOpening = false;
	long NumOpen = mp_PolylineInfo->ma_Opening.size();
	for (long iOpen = NumOpen - 1; iOpen >= 0; iOpen--)
	{
		MSOpening* pOpen = mp_PolylineInfo->ma_Opening[iOpen];
		if (pOpen == NULL) continue;
		// 오프닝에 X 표시
		//pOpen->RemoveMadeByCompoGID(nCompoGID);
		// 오프닝 삭제로 변경됨
		if (pOpen->HasMadeByCompoGID(nCompoGID))
		{
			mp_PolylineInfo->ma_Opening.erase(mp_PolylineInfo->ma_Opening.begin() + iOpen);
			delete pOpen;
			IsDeleteOpening = true;
		}
	}

	if (IsDeleteOpening)
	{
		MSFloor* pFloor = GetFloor();
		pFloor->CalculateUpdatedCompoGArray(this, pFloor->CompoGIDList_NeedRegen_CurTR());
	}
}

void MSElement::RemoveOpeningInfoAll()
{
	if (mp_PolylineInfo == NULL)
		return;

	// 오프닝에 X 표시
	// 	long NumOpen = mp_PolylineInfo->ma_Opening.size();
	// 	for(long iOpen = 0 ; iOpen < NumOpen ; iOpen++)
	// 	{
	// 		MSOpening* pOpen = mp_PolylineInfo->ma_Opening[iOpen];
	// 		if(pOpen == NULL) continue;
	// 		pOpen->RemoveMadeByCompoGIDAll();
	// 	}
	// 오프닝 삭제로 변경됨
	mp_PolylineInfo->DeleteOpenings();
	
	// BH-A 코드
	// 부재에의해 생성된 오프닝만 삭제하도록 수정
	//mp_PolylineInfo->DeleteOpeningsByMadeByCompoG();
}

void MSElement::SetNote(CString note)
{ 
	m_Note = note.Trim(); 

	if (m_Note == _T("")) return;

	MSBaseBuilding* pBuild = GetBuilding();
	if (pBuild == nullptr) return;

	if(pBuild->NoteExistCheck(m_Note) == false)
		pBuild->AddNote(m_Note);
};



CString MSElement::ToString()
{
	CString str;
	str.Format(L"ID:%d, Type:%d, ", m_ID, GetType());

	return str;
}

bool MSElement::RemoveSubMember(MSElement* element)
{
	if (element->GetType() == MSElement::msOpening)
	{
		if (mp_PolylineInfo != nullptr)
			return	mp_PolylineInfo->RemoveOpening((MSOpening*)element);
	}
	return false;
}

bool MSElement::IsGeneral()
{
	if (IsPC() || IsTransfer() || IsDeck() || IsCustomM())
		return false;
	
	return true;
}
