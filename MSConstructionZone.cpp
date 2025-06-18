#include "StdAfx.h"
#include "MSConstructionZone.h"
#include "GMLib/MSVersionInfo.h"
#include "MSFloor.h"
#include "GMLib/GM2dVector.h"
#include "MSPolylineObject.h"
#include "MSRebarBuilding.h"

IMPLEMENT_SERIAL(MSConstructionZone, MSElement, VERSIONABLE_SCHEMA | 1)

MSConstructionZone::MSConstructionZone() 
{
	m_ID = 0;
	mp_PolylineInfo = new MSPolylineObject();
	mp_PolylineInfo->SetOwner(this);
	mp_Owner = NULL;
	m_nNumber = 0;
	m_Name = _T("");

	m_bDefaultCZone = false;
	m_PlanStartDate = InitialDate();
	m_PlanFinishDate = InitialDate();
	m_ActualStartDate = InitialDate();
	m_ActualFinishDate = InitialDate();
	m_eZoneType = MSConstructionZone::Horizontal;
	m_eZoneState = MSConstructionZone::Plan;
}

MSConstructionZone::~MSConstructionZone()
{
	
}

void MSConstructionZone::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	int zoneType = 0;
	int zoneState = 0;
	if(ar.IsStoring())
	{
		ar << m_ID;
		ar << m_nNumber;
		mp_PolylineInfo->mp_Profile->FixPolyLine();
		mp_PolylineInfo->Serialize(ar);
		ar << m_Name;
		
		ar << m_ActualStartDate;
		ar << m_ActualFinishDate;
		ar << m_bDefaultCZone;

		ar << m_PlanStartDate;
		ar << m_PlanFinishDate;
		zoneType = m_eZoneType;
		ar << zoneType;
		zoneState = m_eZoneState;
		ar << zoneState;
	}
	else
	{
		ar >> m_ID;
		ar >> m_nNumber;

		if (MSVersionInfo::GetCurrentVersion() < 20161207)
		{
			GM2DPolyline* pPoly = new GM2DPolyline;
			pPoly->Serialize(ar);
			mp_PolylineInfo->SetProfile(pPoly);
			delete pPoly;
		}
		else
			mp_PolylineInfo->Serialize(ar);
		
		ar >> m_Name;

		if(MSVersionInfo::GetCurrentVersion() >= 20140312)
		{
				ar >> m_ActualStartDate;
				ar >> m_ActualFinishDate;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20140314)
			ar >> m_bDefaultCZone;

		if(MSVersionInfo::GetCurrentVersion() >= 20140729)
			SetLastID(m_ID);

		if ((IsBHQorBHCEMode() && (MSVersionInfo::GetCurrentVersion() >= 20150312 && MSVersionInfo::GetCurrentVersion() < 20200728))
			|| (IsBHForBHAMode() && MSVersionInfo::GetCurrentVersion() >= 20150312 && MSVersionInfo::GetCurrentVersion() < 20240307))
		{
			RebarQuantityInfo rebarQuantityInfo;
			map<CString, RebarQuantityInfo> mapCompoGTypeToRebarQuantityInfo;

			rebarQuantityInfo.Serialize(ar);
			int size;
			ar >> size;

			for (int i = 0; i < size; i++)
			{
				RebarQuantityInfo info;
				CString type;
				ar >> type;
				info.Serialize(ar);

				mapCompoGTypeToRebarQuantityInfo[type] = info;
			}

			MSRebarBuilding* building = dynamic_cast<MSRebarBuilding*>(GetBuilding());
			if (building != nullptr)
			{
				for (int index = 0; index < rebarQuantityInfo.m_aActualQuantity.size(); index++)
				{
					RebarQuantity copyRebarQuantity = rebarQuantityInfo.m_aActualQuantity[index];
					building->m_rebarQuantityInfo.m_aActualQuantity.push_back(copyRebarQuantity);
				}
			}
		}

		if(MSVersionInfo::GetCurrentVersion() < 20150428)
		{
			GetProfile()->MakeCCW();
		}
		
		if (MSVersionInfo::GetCurrentVersion() >= 20161230 && MSVersionInfo::GetCurrentVersion() < 20171020)
		{
			long nContainingElementId = 0;
			ar >> nContainingElementId;
		}

		if (MSVersionInfo::GetCurrentVersion() < 20170712)
		{
			if (MSCompoM::IsExistedSpacialCharacters(m_Name))
			{
				CString newName = m_Name;
				char replacer = ' ';
				newName = MSCompoM::ReplaceSpacialCharactersName(newName, replacer);
				m_Name.Format(_T("%s(%ld)"), newName, m_ID);
			}
		}
		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20200727)
		{
			ar >> m_PlanStartDate;
			ar >> m_PlanFinishDate;
			ar >> zoneType;
			SetZoneType((eCZoneType)zoneType);
			ar >> zoneState;
			SetZoneState((eCZoneState)zoneState);
// 			if (GetZoneType() == MSConstructionZone::Vertical)	m_nNumber = 0;
		}
		mp_PolylineInfo->CheckInValidOpening();
		mp_PolylineInfo->mp_Profile->FixPolyLine();
	}
}

MSObject* MSConstructionZone::Copy( bool bNewID /*= true*/ )
{
	MSConstructionZone* pNewObj = new MSConstructionZone();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSConstructionZone::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/ )
{
	if(pSource_in == nullptr) return;

	MSConstructionZone* pSource = (MSConstructionZone*)pSource_in;

	mp_Owner = pSource->mp_Owner;
	m_nNumber = pSource->m_nNumber;
	m_Name	= pSource->m_Name;
	m_ActualStartDate = pSource->m_ActualStartDate;
	m_ActualFinishDate = pSource->m_ActualFinishDate;
	m_PlanStartDate = pSource->m_PlanStartDate;
	m_PlanFinishDate = pSource->m_PlanFinishDate;
	m_bDefaultCZone = pSource->m_bDefaultCZone;
	m_eZoneType = pSource->GetZoneType();
	m_eZoneState = pSource->GetZoneState();
	MSElement::CopyFromMe(pSource, bNewID);
}

void MSConstructionZone::Dump(EFS::EFS_Buffer& buffer)
{
	MSElement::Dump(buffer);
	buffer << mp_Owner;
	buffer << m_nNumber;
	buffer << m_Name;
	buffer << m_ActualStartDate;
	buffer << m_ActualFinishDate;
	buffer << m_PlanStartDate;
	buffer << m_PlanFinishDate;
	buffer << m_bDefaultCZone;
	
	int zoneType = (int)m_eZoneType;
	buffer << zoneType;
	int zoneState = (int)m_eZoneState;
	buffer << zoneState;

	if (mp_PolylineInfo != nullptr)
		mp_PolylineInfo->Dump(buffer);
}

void MSConstructionZone::Recovery(EFS::EFS_Buffer& buffer)
{
	MSElement::Recovery(buffer);
	buffer >> mp_Owner;
	buffer >> m_nNumber;
	buffer >> m_Name;
	buffer >> m_ActualStartDate;
	buffer >> m_ActualFinishDate;
	buffer >> m_PlanStartDate;
	buffer >> m_PlanFinishDate;
	buffer >> m_bDefaultCZone;
	
	int zoneType = 0;
	buffer >> zoneType;
	SetZoneType((eCZoneType)zoneType);

	int zoneState = 0;
	buffer >> zoneState;
	SetZoneState((eCZoneState)zoneState);
	if (mp_PolylineInfo != nullptr)
		mp_PolylineInfo->Recovery(buffer);
}


CTime MSConstructionZone::GetActualStartDate()
{
	return m_ActualStartDate;
}

void MSConstructionZone::SetActualStartDate( const CTime&  startDate )
{
	m_ActualStartDate = startDate;
	if(m_ActualFinishDate < m_ActualStartDate)
		m_ActualFinishDate = m_ActualStartDate;
}

CTime MSConstructionZone::GetActualFinishDate()
{
	return m_ActualFinishDate;
}

bool MSConstructionZone::SetActualFinishDate(const CTime& finishDate )
{
	if(m_ActualFinishDate < m_ActualStartDate)
		return false;

	m_ActualFinishDate = finishDate;
	return true;
}

CTime MSConstructionZone::GetPlanStartDate()
{
	return m_PlanStartDate;
}

void MSConstructionZone::SetPlanStartDate(const CTime&  startDate)
{
	m_PlanStartDate = startDate;
	if (m_PlanFinishDate < m_PlanStartDate)
		m_PlanFinishDate = m_PlanStartDate;
}

CTime MSConstructionZone::GetPlanFinishDate()
{
	return m_PlanFinishDate;
}

bool MSConstructionZone::SetPlanFinishDate(const CTime& finishDate)
{
	if (m_PlanFinishDate < m_PlanStartDate)
		return false;

	m_PlanFinishDate = finishDate;
	return true;
}

bool MSConstructionZone::Contains(double dXc, double dYc)
{
	if(m_bDefaultCZone)
	{
		bool IsHorizontalZone = GetZoneType() == MSConstructionZone::Horizontal ? true : false;
		vector<MSConstructionZone*> CZoneArr;
		mp_Owner->GetAllCZone(CZoneArr, IsHorizontalZone, false);
		long NumCZone = CZoneArr.size();
		for(long iCZone = 0 ; iCZone < NumCZone ; iCZone++)
		{
			bool isContain = CZoneArr[iCZone]->Contains(dXc, dYc);
			if(isContain)
				return false;
		}
		return true;
	}
	
	if (mp_PolylineInfo != nullptr)
	{
		long nRval = mp_PolylineInfo->Contains(dXc, dYc, DTOL_GM);
		if (nRval < 1)
			return true;
	}
	return false;
}

// GM2DPolyline으로 체크하므로 오프닝은 고려하지 않는다.
bool MSConstructionZone::OverLap(GM2DPolyline* pUrPoly, double dTol)
{
	GM2DPolyline offsetUrPoly; pUrPoly->OffSet_Acis(DTOL_MODELING, FALSE, offsetUrPoly);
	bool IsHorizontalZone = GetZoneType() == MSConstructionZone::Horizontal ? true : false;
	if (m_bDefaultCZone)
	{
		vector<MSConstructionZone*> CZoneArr;
		mp_Owner->GetAllCZone(CZoneArr, IsHorizontalZone, false);
		long NumCZone = CZoneArr.size();
		for (long iCZone = 0; iCZone < NumCZone; iCZone++)
		{
			bool isContain = CZoneArr[iCZone]->OverLap(&offsetUrPoly, dTol);
			if (isContain)
				return false;
		}
		return true;
	}

	return GetProfile()->OverlapEx(offsetUrPoly, dTol, FALSE);
}

// MSPolylineObject의 오프닝까지 고려한다.
bool MSConstructionZone::OverLap(MSPolylineObject* pUrPolylineObject, double dTol)
{
	GM2DPolyline* pUrPoly = pUrPolylineObject->mp_Profile;

	if (m_bDefaultCZone)
	{
		vector<MSConstructionZone*> CZoneArr;
		bool IsHorizontalZone = GetZoneType() == MSConstructionZone::Horizontal ? true : false;
		mp_Owner->GetAllCZone(CZoneArr, IsHorizontalZone, false);
		long NumCZone = CZoneArr.size();
		for (long iCZone = 0; iCZone < NumCZone; iCZone++)
		{
			MSConstructionZone* pCZone = CZoneArr[iCZone];
			bool isContain = pCZone->OverLap(pUrPolylineObject, dTol);
			if (isContain)
				return false;
		}
		return true;
	}

	// 폴리라인과 오프닝이 같을 경우는 없다.

	// 오버랩 True
	// 1.1 CZone과 체크할 폴리라인이 같다

	// 오버랩 False
	// 2.1 체크할 폴리라인의 오프닝에 CZone이 완전 포함된다.
	// 2.2 CZone의 오프닝에 체크할 폴리라인이 완전 포함된다.
	// 2.3 CZone과 체크할 폴리라인의 오프닝이 오버랩된다.
	// ==> 단 CZone의 오프닝과 폴리라인의 오프닝이 같은 경우는 체크 하지 않는다.

	// 1.1 CZone과 체크할 폴리라인이 같다
	if (GetProfile()->SamePolyLine(pUrPoly))
		return true;

	// BH-4705 CZone에 오프닝만 있는 경우 오프닝 내에 있는 슬래브의 CZone 체크시 아래 코드로 인해 CZone의 오프닝 체크가 안되는 문제가 발생
	//// 같은 오너 타입인 경우에만 오프닝 체크를 한다. (CZone, CZone) (Slab, Slab)
	bool IsSameOwnerType = false;
	if (mp_PolylineInfo->GetOwnerType() == pUrPolylineObject->GetOwnerType())
		IsSameOwnerType = true;

	GM2DPolyline offsetUrPoly; pUrPoly->OffSet_Acis(DTOL_MODELING, FALSE, offsetUrPoly);

	if (IsSameOwnerType == true)
	{
		for (int iUrOpening = 0; iUrOpening < pUrPolylineObject->ma_Opening.size(); iUrOpening++)
		{
			MSOpening* pUrOpen = pUrPolylineObject->ma_Opening[iUrOpening];
			// 2.1 체크할 폴리라인의 오프닝에 CZone이 완전 포함된다.
			if (pUrOpen->GetProfile()->Contains(GetProfile()))
				return false;

			bool isSameOpening = false;
			for (int iMyOpening = 0; iMyOpening < mp_PolylineInfo->ma_Opening.size(); iMyOpening++)
			{
				// 2.3 CZone과 체크할 폴리라인의 오프닝이 오버랩된다.
				if (mp_PolylineInfo->ma_Opening[iMyOpening]->GetProfile()->SamePolyLine(pUrOpen->GetProfile()))
				{
					isSameOpening = true;
					break;
				}
			}
			if (isSameOpening)
				continue;

			// 2.3 CZone과 체크할 폴리라인의 오프닝이 오버랩된다.
			if (GetProfile()->OverlapEx(*pUrOpen->GetProfile()))
				return false;
		}

		for (int iMyOpening = 0; iMyOpening < mp_PolylineInfo->ma_Opening.size(); iMyOpening++)
		{
			// 2.2 CZone의 오프닝에 체크할 폴리라인이 완전 포함된다.
			if (mp_PolylineInfo->ma_Opening[iMyOpening]->GetProfile()->Contains(&offsetUrPoly))
				return false;
		}
	}

	return GetProfile()->OverlapEx(offsetUrPoly, dTol, FALSE);
}

bool MSConstructionZone::CheckData(int oldID)
{
	mp_PolylineInfo->CheckInValidOpening();
	MSFloor* floor = mp_Owner;
	if (floor->CheckOverlapConstructionZone(this, oldID) != NULL) return false;

	CString groupName(_T("CZone"));
	bool error = false;
	CString msg;
	CString sSameNumber = _T("같은 Number 를 가진 시공구역이 존재합니다.");
	CString sSameName = _T("같은 이름을 가진 시공구역이 존재합니다.");
	MSConstructionZone::eCZoneType myZoneType = m_eZoneType;
	for (map<long, MSConstructionZone*>::iterator itr = floor->mm_CZone.begin() ; itr != floor->mm_CZone.end() ; ++itr)
	{
		if (oldID > 0)
		{
			if (itr->second->m_ID == oldID) continue;
		}
		if (GetZoneType() != itr->second->GetZoneType())	continue;
		if (GetZoneType() == MSConstructionZone::Horizontal)
		{
			if (this->m_nNumber == itr->second->m_nNumber) { msg = sSameNumber;  error = true; break; }
		}
		if (this->m_Name == itr->second->m_Name) { msg = sSameName; error = true; break; }
	}
	if (error == false)
	{
		// 평면에 보이는 상태로 수직/수평 이름 중복검토
		MSFloor* otherFloor = nullptr;
		if (myZoneType == MSConstructionZone::Horizontal)
			otherFloor = floor->GetUpperFloor();
		else
			otherFloor = floor->GetLowerFloor();

		if (otherFloor == nullptr)	return true;
		for (auto it = otherFloor->mm_CZone.begin(); it != otherFloor->mm_CZone.end(); ++it)
		{
			if (myZoneType  == it->second->GetZoneType())	continue;
			if (m_Name == it->second->m_Name) { msg = sSameName; error = true; break; }
		}
	}

	if (error)
	{
		XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();
		XErrorObject* pMsgObject = pMessageContainer->CreateErrorObject(0, -1, msg, XErrorObject::MSElementIDSelect);
		pMessageContainer->AddErrorObject(pMsgObject);

		pMsgObject->SetGroupName(groupName);
		pMsgObject->m_Message.Add(msg);
		pMsgObject->AddCompoGID(GetID());
		pMsgObject->SetShowSelectCompoG(TRUE);
		return false;
	}

	return true;
}

bool MSConstructionZone::CheckData()
{
	if (MSElement::CheckData() == false)
		return false;

	return CheckData(0);
}

GM2DPolyline* MSConstructionZone::GetProfile()
{
	if (mp_PolylineInfo != nullptr)
		return mp_PolylineInfo->GetProfile();
	return nullptr;
}

void MSConstructionZone::Translate(GM2DVector &pDelta)
{
	MSElement::Translate(pDelta);
}

void MSConstructionZone::MirrorByAxis(GM2DLineVector& pAxis)
{
	MSElement::MirrorByAxis(pAxis);
}

void MSConstructionZone::RotateZ(double theta, GM2DVector *pCenter)
{
	MSElement::RotateZ(theta, pCenter);
}

void MSConstructionZone::SetName(CString name)
{
	name.Trim();
	m_Name = name;
}

MSBaseBuilding* MSConstructionZone::GetBuilding()
{
	MSFloor* pFloor = GetFloor();
	if (pFloor == nullptr)
		return nullptr;

	return pFloor->GetBuilding();
}

void MSConstructionZone::UpdateOpeningProfile(GM2DPolyline* pProfile, long cZoneID)
{
	long NumOpen = mp_PolylineInfo->ma_Opening.size();
	for (long iOpen = NumOpen - 1; iOpen >= 0; iOpen--)
	{
		MSOpening* pOpen = mp_PolylineInfo->ma_Opening[iOpen];
		if (pOpen == nullptr || pOpen->ma_MadeByCompoGID.size() == 0) continue;

		if (pOpen->HasMadeByCompoGID(cZoneID))
			pOpen->SetProfile(pProfile);
	}
}

MSConstructionZone::eCZoneType MSConstructionZone::GetZoneType()
{
	return m_eZoneType; 
}
void MSConstructionZone::SetZoneType(MSConstructionZone::eCZoneType zoneType)
{
	m_eZoneType = zoneType; 

}

MSConstructionZone::eCZoneState MSConstructionZone::GetZoneState()
{
	return m_eZoneState;
}
void MSConstructionZone::SetZoneState(MSConstructionZone::eCZoneState zoneState)
{
	m_eZoneState = zoneState;
}

void MSConstructionZone::FixProfile()
{
	GM2DPolyline* pProFile = GetProfile();
	if (pProFile == nullptr) return;
	pProFile->DeleteInLinePoint(DTOL_MODELING);
	pProFile->MakeClosedPolyline(DTOL_MODELING);
}

void MSConstructionZone::RemoveDumpOpening()
{
	MSFloor* pFloor = GetFloor();
	vector<MSOpening*> removeOpeningArr;

	long NumOpen = mp_PolylineInfo->ma_Opening.size();
	for (long iOpen = NumOpen - 1; iOpen >= 0; iOpen--)
	{
		MSOpening* pOpen = mp_PolylineInfo->ma_Opening[iOpen];

		int nGIDCnt = pOpen->ma_MadeByCompoGID.size();
		if (nGIDCnt == 0) continue;

		if (pOpen == nullptr || pOpen->ma_MadeByCompoGID.size() == 0) continue;

		int nFindCnt = 0;
		for (int iG = 0; iG < pOpen->ma_MadeByCompoGID.size(); iG++)
		{
			MSConstructionZone* pFindCZone = pFloor->GetCZone(pOpen->ma_MadeByCompoGID[iG]);
			if (pFindCZone != nullptr && pFindCZone->GetFloor() == pFloor)
				nFindCnt++;
		}

		if (nFindCnt == 0)
			removeOpeningArr.push_back(pOpen);
	}

	for (int iOpen = 0; iOpen < removeOpeningArr.size(); iOpen++)
		mp_PolylineInfo->RemoveOpening(removeOpeningArr[iOpen]);

}
long MSConstructionZone::GetNumber()
{
	if (m_bDefaultCZone)
		return 99999;
	return m_nNumber;
}
void MSConstructionZone::SetNumber(long number)
{
	m_nNumber = number;
}