#include "stdafx.h"
#include "MSCustomRebar.h"

IMPLEMENT_SERIAL(MSCustomRebar, MSRcObject, VERSIONABLE_SCHEMA | 1)
MSCustomRebar::MSCustomRebar()
{
	m_DiaName = _T("10");
}

MSCustomRebar::~MSCustomRebar()
{
}

void MSCustomRebar::CopyFromMe(MSCustomRebar* pSource)
{
	if (pSource == NULL)	return;

	m_DiaName = pSource->GetDiaName();
	m_Location.CopyFrom(&pSource->GetLocation());

	MSRcObject::CopyFromMe(pSource);
}

void MSCustomRebar::Dump(EFS::EFS_Buffer& buffer)
{
	MSRcObject::Dump(buffer);

	buffer << m_DiaName;
	m_Location.Dump(buffer);
}

void MSCustomRebar::Recovery(EFS::EFS_Buffer& buffer)
{
	MSRcObject::Recovery(buffer);

	buffer >> m_DiaName;
	m_Location.Recovery(buffer);
}

void MSCustomRebar::Serialize(CArchive &ar)
{
	MSRcObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_DiaName;
		m_Location.Serialize(ar);
	}
	else
	{
		ar >> m_DiaName;
		m_Location.Serialize(ar);
	}
}
