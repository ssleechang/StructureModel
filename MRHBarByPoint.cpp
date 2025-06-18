#include "StdAfx.h"
#include "MRHBarByPoint.h"
#include "MRHBarPointByLoc.h"

IMPLEMENT_SERIAL(MRHBarByPoint, MRHBar, VERSIONABLE_SCHEMA | 1)
MRHBarByPoint::MRHBarByPoint(void)
{
	m_StartAnchorType = AnchorType_End;
	m_EndAnchorType = AnchorType_End;
}

MRHBarByPoint::~MRHBarByPoint(void)
{
	DeleteHBarPoints();
}
void MRHBarByPoint::Serialize(CArchive &ar)
{
	MRHBar::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_StartAnchorType;
		ar << m_EndAnchorType;

		long nPoint = ma_Point.GetSize();
		ar << nPoint;
		for(long iPoint=0; iPoint<nPoint; iPoint++)
		{
			MRHBarPointByLoc* pPoint = ma_Point[iPoint];
			pPoint->Serialize(ar);
		}
	}
	else
	{
		int nStartType=0, nEndType=0;
		ar >> nStartType;
		ar >> nEndType;
		m_StartAnchorType = (AnchorType)nStartType;
		m_EndAnchorType = (AnchorType)nEndType;

		long nPoint = 0;
		ar >> nPoint;
		for(long iPoint=0; iPoint<nPoint; iPoint++)
		{
			MRHBarPointByLoc* pPoint = new MRHBarPointByLoc;
			pPoint->Serialize(ar);
			ma_Point.Add(pPoint);
		}
	}
}

void MRHBarByPoint::DeleteHBarPoints()
{
	long nPoint = ma_Point.GetSize();
	for(long iPoint=0; iPoint<nPoint; iPoint++)
		delete ma_Point[iPoint];
	ma_Point.RemoveAll();
}