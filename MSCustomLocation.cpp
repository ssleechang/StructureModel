#include "stdafx.h"
#include "MSCustomLocation.h"
#include "MSCustomSection.h"
#include "MSCustomReBarLine.h"
IMPLEMENT_SERIAL(MSCustomLocation, MSRcObject, VERSIONABLE_SCHEMA | 1)
MSCustomLocation::MSCustomLocation()
{
	Initialize();
}

MSCustomLocation::~MSCustomLocation()
{
}

void MSCustomLocation::Initialize()
{
	m_RebarLineID1 = 0;
	m_RebarLineID2 = 0;
}

void MSCustomLocation::CopyFromMe(MSCustomLocation* pSource)
{
	if (pSource == NULL)	return;

	m_RebarLineID1 = pSource->GetRebarLineID1();
	m_RebarLineID2 = pSource->GetRebarLineID2();

	MSRcObject::CopyFromMe(pSource);
}

void MSCustomLocation::Dump(EFS::EFS_Buffer& buffer)
{
	MSRcObject::Dump(buffer);
	buffer << m_RebarLineID1 << m_RebarLineID2;
}

void MSCustomLocation::Recovery(EFS::EFS_Buffer& buffer)
{
	MSRcObject::Recovery(buffer);
	buffer >> m_RebarLineID1 >> m_RebarLineID2;
}

void MSCustomLocation::Serialize(CArchive &ar)
{
	MSRcObject::Serialize(ar);
	int eType = 0;
	if (ar.IsStoring())
	{
		ar << m_RebarLineID1;
		ar << m_RebarLineID2;
	}
	else
	{
		ar >> m_RebarLineID1;
		ar >> m_RebarLineID2;
	}
}

bool MSCustomLocation::HasRebarLineID(long rebarLineID)
{
	if (rebarLineID == 0)	return false;
	if (m_RebarLineID1 == rebarLineID)	return true;
	if (m_RebarLineID2 == rebarLineID)	return true;
	return false;
}

CString MSCustomLocation::GetName(MSCustomSection* pSection)
{
	if (pSection == nullptr)	return _T("");
	MSCustomReBarLine* pRebarLine1 = pSection->GetMSRebarLine(m_RebarLineID1);
	MSCustomReBarLine* pRebarLine2 = pSection->GetMSRebarLine(m_RebarLineID2);
	if (pRebarLine1 == nullptr || pRebarLine2 == nullptr)	return _T("");

	CString sName;
	sName.Format(_T("%s;%s"), pRebarLine1->GetName(), pRebarLine2->GetName());
	return sName;
}

