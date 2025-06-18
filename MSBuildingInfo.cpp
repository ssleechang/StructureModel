#include "stdafx.h"
#include "MSBuildingInfo.h"
#include "MSBuildingClassificationData.h"
#include "MSBuildingInfoManager.h"
#include "..\GMLib\MSVersionInfo.h"

IMPLEMENT_SERIAL(MSBuildingInfo, MSObject, VERSIONABLE_SCHEMA | 1)

long MSBuildingInfo::ms_LastID = 0;
MSBuildingInfo::MSBuildingInfo(MSBuildingInfoManager* pBldgInfoManager)
{
	m_pBldgInfoManager = pBldgInfoManager;

	m_ID = 0;
	m_Name = _T("");
	m_dOverGrossFloor = 0;
	m_dUnderGrossFloor = 0;

	m_nBldgClassificationID = 0;
	m_strUsage = _T("");
}

MSBuildingInfo::~MSBuildingInfo(void)
{
}

long MSBuildingInfo::GetID()
{
	if (m_ID == 0) // 
	{
		m_ID = ms_LastID + 1;
		ms_LastID = m_ID;
		return m_ID;
	}

	return m_ID;
}

void MSBuildingInfo::SetLastID(long NewID)
{
	if (NewID > ms_LastID)
		ms_LastID = NewID;
}

MSObject* MSBuildingInfo::Copy(bool bNewID /*= true*/)
{
	MSBuildingInfo* pNewObj = new MSBuildingInfo(m_pBldgInfoManager);
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSBuildingInfo::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;

	MSBuildingInfo* pSource = dynamic_cast<MSBuildingInfo*>(pSource_in);
	if (pSource == nullptr)	return;

	m_Name = pSource->GetName();
	m_dOverGrossFloor = pSource->GetOverGrossFloor();
	m_dUnderGrossFloor = pSource->GetUnderGrossFloor();

	m_nBldgClassificationID = pSource->m_nBldgClassificationID;
}

void MSBuildingInfo::Serialize(CArchive &ar)
{
	CString buildingName = _T("");

	MSObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_ID;
		ar << buildingName;
		ar << m_Name;
		ar << m_dOverGrossFloor;
		ar << m_dUnderGrossFloor;
		ar << m_nBldgClassificationID;
	}
	else
	{
		ar >> m_ID;
		ar >> buildingName;
		ar >> m_Name;
		if (MSVersionInfo::GetCurrentVersion() < 20230110)
		{
			//CString strUsage; // 용도 ==> 사용 안함, 빌딩 분류의 건물 유형으로 변경
			ar >> m_strUsage;
		}

		ar >> m_dOverGrossFloor;
		ar >> m_dUnderGrossFloor;
		if (MSVersionInfo::GetCurrentVersion() >= 20230110)
			ar >> m_nBldgClassificationID;

		SetLastID(m_ID);
	}
}
void MSBuildingInfo::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	buffer << m_Name;
	buffer << m_dOverGrossFloor;
	buffer << m_dUnderGrossFloor;
}

void MSBuildingInfo::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	buffer >> m_Name;
	buffer >> m_dOverGrossFloor;
	buffer >> m_dUnderGrossFloor;
}

void MSBuildingInfo::SetMSBuildingInfoManager(MSBuildingInfoManager* pMSBuildingInfoManager)
{
	m_pBldgInfoManager = pMSBuildingInfoManager;
}

CString MSBuildingInfo::GetBuildingInfoClassification()
{
	MSBuildingClassificationData* pMSBuildingClassification = GetMSBuildingClassification();
	if (pMSBuildingClassification != nullptr)
		return pMSBuildingClassification->GetBuildingClassification();

	return _T("");
}

CString MSBuildingInfo::GetBuildingInfoType()
{
	MSBuildingClassificationData* pMSBuildingClassification = GetMSBuildingClassification();
	if (pMSBuildingClassification != nullptr)
		return pMSBuildingClassification->GetBuildingType();

	return _T("");
}

void MSBuildingInfo::SetBuildingInfoType(CString buildningType)
{
	MSBuildingClassificationData* pMSBuildingClassification = GetMSBuildingClassification();
	if (pMSBuildingClassification != nullptr)
		pMSBuildingClassification->SetBuildingType(buildningType);
}

long MSBuildingInfo::GetMSBuildingClassificationID()
{
	return m_nBldgClassificationID;
}

MSBuildingClassificationData* MSBuildingInfo::GetMSBuildingClassification()
{
	if (m_pBldgInfoManager != nullptr)
		return m_pBldgInfoManager->GetBuildingClassification(m_nBldgClassificationID);

	return nullptr;
}

void MSBuildingInfo::SetBuildingClassification(MSBuildingClassificationData* pBuildingClassification)
{
	if (pBuildingClassification != nullptr)
		m_nBldgClassificationID = pBuildingClassification->GetID();
}
