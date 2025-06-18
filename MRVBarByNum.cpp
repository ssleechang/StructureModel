#include "StdAfx.h"
#include "MRVBarByNum.h"

IMPLEMENT_SERIAL(MRVBarByNum, MRVBar, VERSIONABLE_SCHEMA | 1)
MRVBarByNum::MRVBarByNum(void)
{
	m_NumBar = 0;
	m_bHasCornerBar = true;
}

MRVBarByNum::~MRVBarByNum(void)
{
}
void MRVBarByNum::Serialize(CArchive &ar)
{
	MRVBar::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_NumBar;
		ar << m_bHasCornerBar;
	}
	else
	{
		ar >> m_NumBar;
		ar >> m_bHasCornerBar;
	}
}

int MRVBarByNum::GetBarLocArr( vector<double>&LocArr )
{
	double dLoc = 0.;
	double dRatio = 1.0/(m_NumBar-1);
	if(!m_bHasCornerBar)
	{
		dRatio = 1.0/(m_NumBar+1);
		dLoc+=dRatio;
	}
	for(int i = 0;i<m_NumBar;i++)
	{
		LocArr.push_back(dLoc);
		dLoc+=dRatio;
	}
	return LocArr.size();
}
