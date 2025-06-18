#include "stdafx.h"
#include "MSBuildingClassificationData.h"

IMPLEMENT_SERIAL(MSBuildingClassificationData, MSObject, VERSIONABLE_SCHEMA | 1)

long MSBuildingClassificationData::ms_LastID = 0;
MSBuildingClassificationData::MSBuildingClassificationData()
{
	m_ID = 0;
	m_BldgType = _T("");
	m_BldgClassification = _T("");
}

MSBuildingClassificationData::MSBuildingClassificationData(CString BldgType, CString BldgClassification)
{
	m_ID = 0;
	m_BldgType = BldgType;
	m_BldgClassification = BldgClassification;
}

MSBuildingClassificationData::~MSBuildingClassificationData(void)
{
}

long MSBuildingClassificationData::GetID()
{
	if (m_ID == 0) // 
	{
		m_ID = ms_LastID + 1;
		ms_LastID = m_ID;
		return m_ID;
	}

	return m_ID;
}

void MSBuildingClassificationData::SetLastID(long NewID)
{
	if (NewID > ms_LastID)
		ms_LastID = NewID;
}

MSObject* MSBuildingClassificationData::Copy(bool bNewID /*= true*/)
{
	MSBuildingClassificationData* pNewObj = new MSBuildingClassificationData();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSBuildingClassificationData::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;

	MSBuildingClassificationData* pSource = dynamic_cast<MSBuildingClassificationData*>(pSource_in);
	if (pSource == nullptr)	return;

	m_BldgClassification = pSource->GetBuildingClassification();
	m_BldgType = pSource->GetBuildingType();
}

void MSBuildingClassificationData::Serialize(CArchive &ar)
{
	CString buildingName = _T("");

	MSObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_ID;
		ar << m_BldgClassification;
		ar << m_BldgType;
	}
	else
	{
		ar >> m_ID;
		ar >> m_BldgClassification;
		ar >> m_BldgType;
		SetLastID(m_ID);
	}
}

void MSBuildingClassificationData::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	buffer << m_BldgClassification;
	buffer << m_BldgType;
}

void MSBuildingClassificationData::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	buffer >> m_BldgClassification;
	buffer >> m_BldgType;
}