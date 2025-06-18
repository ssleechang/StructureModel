#include "StdAfx.h"
#include "MRHBarPointByLoc.h"

IMPLEMENT_SERIAL(MRHBarPointByLoc, MRObject, VERSIONABLE_SCHEMA | 1)
MRHBarPointByLoc::MRHBarPointByLoc(void)
{
	m_EdgeIdx = -1;
	m_LayerIdx = -1;
	m_Loc = 0.0;
}

MRHBarPointByLoc::~MRHBarPointByLoc(void)
{
}
void MRHBarPointByLoc::Serialize(CArchive& ar)
{
	MRObject::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_EdgeIdx;
		ar << m_LayerIdx;
		ar << m_Loc;
	}
	else
	{
		ar >> m_EdgeIdx;
		ar >> m_LayerIdx;
		ar >> m_Loc;
	}
}