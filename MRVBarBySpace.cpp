#include "StdAfx.h"
#include "MRVBarBySpace.h"

IMPLEMENT_SERIAL(MRVBarBySpace, MRVBar, VERSIONABLE_SCHEMA | 1)
MRVBarBySpace::MRVBarBySpace(void)
{
	m_Space = 300;
}

MRVBarBySpace::~MRVBarBySpace(void)
{
}

int MRVBarBySpace::GetBarLocArr( vector<double>&LocArr )
{

	return LocArr.size();

}
void MRVBarBySpace::Serialize(CArchive &ar)
{
	MRVBar::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_Space;
	}
	else
	{
		ar >> m_Space;
	}
}
