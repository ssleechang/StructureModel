#include "StdAfx.h"
#include "MRHBarByEdge.h"

IMPLEMENT_SERIAL(MRHBarByEdge, MRHBar, VERSIONABLE_SCHEMA | 1)

MRHBarByEdge::MRHBarByEdge(void)
{
	m_StartAnchorType = AnchorType_End;
	m_EndAnchorType = AnchorType_End;

	m_dSLoc = 0.0;
	m_dELoc = 1.0;
}

MRHBarByEdge::~MRHBarByEdge(void)
{
}

void MRHBarByEdge::AddEdgeIdx( int nVal )
{
	ma_EdgeIdx.Add(nVal);
}

void MRHBarByEdge::Serialize( CArchive& archive )
{

}
