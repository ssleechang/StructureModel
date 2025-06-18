#include "stdafx.h"
#include "MSCustomBarUnit.h"

long MSCustomBarUnit::ms_LastID = 0;
IMPLEMENT_SERIAL(MSCustomBarUnit, MSRcObject, VERSIONABLE_SCHEMA | 1)

MSCustomBarUnit::MSCustomBarUnit(void)
{
	mp_Section = nullptr;
	m_ID = 0;
}

MSCustomBarUnit::~MSCustomBarUnit(void)
{
}

long MSCustomBarUnit::GetID()
{
	if (m_ID == 0) // 
	{
		m_ID = ms_LastID + 1;
		ms_LastID = m_ID;
		return m_ID;
	}

	return m_ID;
}

void MSCustomBarUnit::SetLastID(long NewID)
{
	if (NewID > ms_LastID)
		ms_LastID = NewID;
}

void MSCustomBarUnit::CopyFromMe(MSCustomBarUnit* pSource)
{
	if (pSource == NULL)	return;

}

void MSCustomBarUnit::Dump(EFS::EFS_Buffer& buffer)
{
	MSRcObject::Dump(buffer);

	buffer << m_ID;
}

void MSCustomBarUnit::Recovery(EFS::EFS_Buffer& buffer)
{
	MSRcObject::Recovery(buffer);

	buffer >> m_ID;
}

void MSCustomBarUnit::Serialize(CArchive &ar)
{
	MSRcObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_ID;
	}
	else
	{
		ar >> m_ID;
		SetLastID(m_ID);
	}
}

MSBaseBuilding* MSCustomBarUnit::GetBuilding()
{
	return GetCustomSection()->GetBuilding();
}

MSCustomSection::eSectionType MSCustomBarUnit::GetCustomSectionType()
{
	if (mp_Section == nullptr)
		return MSCustomSection::CUSTOM_NONE;
	return mp_Section->GetSectionType();
}