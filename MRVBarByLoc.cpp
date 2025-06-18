#include "StdAfx.h"
#include "MRVBarByLoc.h"

IMPLEMENT_SERIAL(MRVBarByLoc, MRVBar, VERSIONABLE_SCHEMA | 1)
MRVBarByLoc::MRVBarByLoc(void)
{
	m_Loc = 0.0;
}

MRVBarByLoc::~MRVBarByLoc(void)
{
}
void MRVBarByLoc::Serialize(CArchive &ar)
{
	MRVBar::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_Loc;
	}
	else
	{
		ar >> m_Loc;
	}
}

int MRVBarByLoc::GetBarLocArr( vector<double>&LocArr )
{
	LocArr.push_back(m_Loc);
	return 1;
}
