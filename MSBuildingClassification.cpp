#include "stdafx.h"
#include "MSBuildingClassification.h"
#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSBuildingClassification, MSObject, VERSIONABLE_SCHEMA | 1)

long MSBuildingClassification::ms_LastID = 0;
MSBuildingClassification::MSBuildingClassification()
{
	m_ID = 0;
	m_BldgType = _T("");
	m_BldgClassification = _T("");
}

MSBuildingClassification::MSBuildingClassification(CString BldgType, CString BldgClassification)
{
	m_ID = 0;
	m_BldgType = BldgType;
	m_BldgClassification = BldgClassification;
}

MSBuildingClassification::~MSBuildingClassification(void)
{
}

long MSBuildingClassification::GetID()
{
	if (m_ID == 0) // 
	{
		m_ID = ms_LastID + 1;
		ms_LastID = m_ID;
		return m_ID;
	}

	return m_ID;
}

void MSBuildingClassification::SetLastID(long NewID)
{
	if (NewID > ms_LastID)
		ms_LastID = NewID;
}

MSObject* MSBuildingClassification::Copy(bool bNewID /*= true*/)
{
	MSBuildingClassification* pNewObj = new MSBuildingClassification();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSBuildingClassification::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;

	MSBuildingClassification* pSource = dynamic_cast<MSBuildingClassification*>(pSource_in);
	if (pSource == nullptr)	return;

	m_BldgClassification = pSource->GetBuildingClassification();
	m_BldgType = pSource->GetBuildingType();
}

void MSBuildingClassification::Serialize(CArchive &ar)
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
		
		if (MSVersionInfo::GetCurrentVersion() < 20230209)
		{
			if (m_BldgType == "근성주차장")
				m_BldgType = "근생주차장";
		}

		SetLastID(m_ID);
	}
}

void MSBuildingClassification::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	buffer << m_BldgClassification;
	buffer << m_BldgType;
}

void MSBuildingClassification::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	buffer >> m_BldgClassification;
	buffer >> m_BldgType;
}