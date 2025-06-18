#include "stdafx.h"
#include "MSCustomLineBarLocation.h"

IMPLEMENT_SERIAL(MSCustomLineBarLocation, MSRcObject, VERSIONABLE_SCHEMA | 1)
MSCustomLineBarLocation::MSCustomLineBarLocation()
{
	m_PointBarUnitID = 0;
	m_PointBarIndex = -1;
}

MSCustomLineBarLocation::~MSCustomLineBarLocation()
{
}

void MSCustomLineBarLocation::CopyFromMe(MSCustomLineBarLocation* pSource)
{
	if (pSource == NULL)	return;

	m_PointBarUnitID = pSource->GetPointBarUnitID();
	m_PointBarIndex = pSource->GetPointBarIndex();

	MSRcObject::CopyFromMe(pSource);
}

void MSCustomLineBarLocation::Dump(EFS::EFS_Buffer& buffer)
{
	MSRcObject::Dump(buffer);
	buffer << m_PointBarUnitID << m_PointBarIndex;
}

void MSCustomLineBarLocation::Recovery(EFS::EFS_Buffer& buffer)
{
	MSRcObject::Recovery(buffer);
	buffer >> m_PointBarUnitID >> m_PointBarIndex;
}

void MSCustomLineBarLocation::Serialize(CArchive &ar)
{
	MSRcObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_PointBarUnitID;
		ar << m_PointBarIndex;
	}
	else
	{
		ar >> m_PointBarUnitID;
		ar >> m_PointBarIndex;
	}
}
