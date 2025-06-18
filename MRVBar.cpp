#include "StdAfx.h"
#include "MRVBar.h"
#include "GMLib\GM2DLineVector.h"

IMPLEMENT_SERIAL(MRVBar, MRSectionBar, VERSIONABLE_SCHEMA | 1)
MRVBar::MRVBar(void)
{
	mp_Edge = NULL;
	mp_StrpCurve = NULL;
	m_LayerDistFromEdge = 0.0;
}

MRVBar::~MRVBar(void)
{
}
void MRVBar::Serialize(CArchive &ar)
{
	MRSectionBar::Serialize(ar);
	bool UnUsed=0;
	if(ar.IsStoring())
	{
		if(mp_StrpCurve)
		{
			ar << mp_StrpCurve->m_ObjType;
			mp_StrpCurve->Serialize(ar);
		}
		else
			ar << UnUsed;

		ar << m_LayerDistFromEdge;
		long nVBarLoc = ma_VBarLoc.GetSize();
		ar << nVBarLoc; 
		for(long iVBarLoc=0; iVBarLoc<nVBarLoc; iVBarLoc++)
			ar << ma_VBarLoc[iVBarLoc];
	}
	else
	{
		long nObjType=0;
		ar >> nObjType;
		if(nObjType==GT_LINE)			mp_StrpCurve = new GM2DLineVector();
		else if(nObjType==GT_ARC)		mp_StrpCurve = new GMArc();
		else if(nObjType==GT_CIRCLE)	mp_StrpCurve = new GMCircle;

		if(mp_StrpCurve)		mp_StrpCurve->Serialize(ar);

		ar >> m_LayerDistFromEdge;
		long nVBarLoc = 0;
		ar >> nVBarLoc; 
		for(long iVBarLoc=0; iVBarLoc<nVBarLoc; iVBarLoc++)
		{
			double dLoc = 0;
			ar >> dLoc;
			ma_VBarLoc.Add(dLoc);
		}
	}
}
