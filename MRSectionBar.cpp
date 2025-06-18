#include "StdAfx.h"
#include "MRSectionBar.h"
#include "GMLib\MSVersionInfo.h"

IMPLEMENT_SERIAL(MRSectionBar, MRObject, VERSIONABLE_SCHEMA | 1)
MRSectionBar::MRSectionBar(void)
{
	m_strBar = _T("");
	m_DiaName = _T("");
}

MRSectionBar::~MRSectionBar(void)
{
}

void MRSectionBar::Serialize(CArchive& ar)
{
	MRObject::Serialize(ar);
	long nEdge=0, nTieBar=0;
	if(ar.IsStoring())
	{
		ar << m_strBar;
		ar << m_DiaName;
	}
	else
	{
		ar >> m_strBar;
		if(MSVersionInfo::GetCurrentVersion() < 20160108)
		{
			int nDia;
			ar >> nDia;
			m_DiaName.Format(_T("%ld"), nDia);
		}
		else
			ar >> m_DiaName;
	}
}
void MRSectionBar::SetBarString( CString strBar )
{
	m_strBar = strBar;
	ParsingBarString();
}

bool MRSectionBar::ParsingBarString()
{
	return false;
}
