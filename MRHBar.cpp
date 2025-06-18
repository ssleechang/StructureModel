#include "StdAfx.h"
#include "MRHBar.h"

IMPLEMENT_SERIAL(MRHBar, MRSectionBar, VERSIONABLE_SCHEMA | 1)
MRHBar::MRHBar(void)
{
	m_Space = 300; 
	m_NumBar = 0;
}

MRHBar::~MRHBar(void)
{
}
void MRHBar::Serialize(CArchive& ar)
{
	MRSectionBar::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_Space;
		ar << m_NumBar;
	}
	else
	{
		ar >> m_Space;
		ar >> m_NumBar;
	}
}
bool MRHBar::ParsingBarString()
{
	return true;
}