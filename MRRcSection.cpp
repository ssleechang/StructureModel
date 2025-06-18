#include "StdAfx.h"
#include "MRRcSection.h"
#include "MREdge.h"
#include "MRHBar.h"
#include "MRHBarByPoint.h"
#include "GMLib\GM2DLineVector.h"

IMPLEMENT_SERIAL(MRRcSection, MRSection, VERSIONABLE_SCHEMA | 1)
MRRcSection::MRRcSection(void)
{
	m_nStirrupTempletID = 0;
	m_StirrupSpace = 300;
	m_StirrupNum = 0;
	m_TieBarSpace = 300;
	m_TieBarNum = 0;
}

MRRcSection::~MRRcSection(void)
{
	DeleteEdges();
	DeleteTieBars();
}

void MRRcSection::Serialize(CArchive& ar)
{
	MRSection::Serialize(ar);
	long nEdge=0, nTieBar=0;
	if(ar.IsStoring())
	{
		ar << m_StirrupSpace;
		ar << m_StirrupNum;
		ar << m_TieBarSpace;
		ar << m_TieBarNum;
		ar << m_nStirrupTempletID;

		nEdge = ma_Edge.GetSize();
		ar << nEdge;
		for(long iEdge=0; iEdge<nEdge; iEdge++)
		{
			MREdge* pEdge = ma_Edge[iEdge];
			pEdge->Serialize(ar);
		}
		nTieBar = ma_TieBar.GetSize();
		ar << nTieBar;
		for(long iTieBar=0; iTieBar<nTieBar; iTieBar++)
		{
			MRHBarByPoint* pTieBar = ma_TieBar[iTieBar];
			pTieBar->Serialize(ar);
		}
	}
	else
	{
		ar >> m_StirrupSpace;
		ar >> m_StirrupNum;
		ar >> m_TieBarSpace;
		ar >> m_TieBarNum;
		ar >> m_nStirrupTempletID;

		ar >> nEdge;
		for(long iEdge=0; iEdge<nEdge; iEdge++)
		{
			MREdge* pEdge = new MREdge;
			pEdge->Serialize(ar);
			ma_Edge.Add(pEdge);
		}
		ar >> nTieBar;
		for(long iTieBar=0; iTieBar<nTieBar; iTieBar++)
		{
			MRHBarByPoint* pTieBar = new MRHBarByPoint;
			pTieBar->Serialize(ar);
			ma_TieBar.Add(pTieBar);
		}
	}
}
void MRRcSection::DeleteEdges()
{
	long nEdge = ma_Edge.GetSize();
	for(long iEdge=0; iEdge < nEdge; iEdge++)
		delete ma_Edge[iEdge];
	ma_Edge.RemoveAll();
}
void MRRcSection::DeleteTieBars()
{
	long nTieBar = ma_TieBar.GetSize();
	for(long iTieBar=0; iTieBar<nTieBar; iTieBar++)
		delete ma_TieBar[iTieBar];
	ma_TieBar.RemoveAll();
}
void MRRcSection::AddEdge( MREdge* pEdge )
{
	ma_Edge.Add(pEdge);
}

void MRRcSection::SetStirrupTempletID(long nID)
{
	m_nStirrupTempletID = nID;
}

void MRRcSection::SetBarLine4Edge( double dCover, double dHBarDia, double dVBarDia )
{
	int numEdge = ma_Edge.GetSize();
	if(numEdge<3)
	{
		ASSERT(0);
		return; 
	}
	CTypedPtrArray<CObArray, GM2DLineVector*> HLineArr;
	CTypedPtrArray<CObArray, GM2DLineVector*> VLineArr;
	for(int iEdge = 0;iEdge<numEdge;iEdge++)
	{
		MREdge* pEdge = ma_Edge[iEdge];
		if(pEdge == NULL)	continue;
		GM2DLineVector* pEdgeLine = (GM2DLineVector*)pEdge->mp_Curve;
		GM2DLineVector* HBarLine = new GM2DLineVector(pEdgeLine->NormalTranslate(dCover));
		pEdge->mp_HBarCurve = HBarLine;
		HLineArr.Add(HBarLine);
		GM2DLineVector* VBarLine = new GM2DLineVector(pEdgeLine->NormalTranslate(dCover+dHBarDia+dVBarDia/2));
		pEdge->mp_VBarCurve = VBarLine;
		VLineArr.Add(HBarLine);
	}
	//교차가 반대로 일어나는경우 NULL
	GM2DLineVector* PreLine = NULL, *CurLine = NULL , *NextLine = NULL;
	GM2DVector crossPreVec, crossNextVec;
	double dPreMyLoc = 0, dPreUrLoc = 0;
	double dNextMyLoc = 0, dNextUrLoc = 0;
	for(iEdge = 0; iEdge < HLineArr.GetSize(); iEdge++)
	{
		if(iEdge == 0)
			PreLine = HLineArr[HLineArr.GetSize()-1];
		else
			PreLine = HLineArr[iEdge-1];
		CurLine = HLineArr[iEdge];
		if(iEdge == numEdge-1)
			NextLine = HLineArr[0];
		else
			NextLine = HLineArr[iEdge+1];
		CurLine->Intersect(*PreLine,crossPreVec,&dPreMyLoc, &dPreUrLoc);
		CurLine->Intersect(*NextLine,crossNextVec,&dNextMyLoc, &dNextUrLoc);
		if(dPreMyLoc > dNextMyLoc)
			CurLine = NULL;
	}
	for(iEdge = 0; iEdge < VLineArr.GetSize(); iEdge++)
	{
		if(iEdge == 0)
			PreLine = VLineArr[VLineArr.GetSize()-1];
		else
			PreLine = VLineArr[iEdge-1];
		CurLine = VLineArr[iEdge];
		if(iEdge == numEdge-1)
			NextLine = VLineArr[0];
		else
			NextLine = VLineArr[iEdge+1];
		CurLine->Intersect(*PreLine,crossPreVec,&dPreMyLoc, &dPreUrLoc);
		CurLine->Intersect(*NextLine,crossNextVec,&dNextMyLoc, &dNextUrLoc);
		if(dPreMyLoc > dNextMyLoc)
			CurLine = NULL;
	}

	//Trim
	for(iEdge = 0; iEdge < numEdge; iEdge++)
	{
		CurLine = HLineArr[iEdge];
		if(CurLine == NULL)	
			continue;
		for(int jEdge = iEdge+1; jEdge <= numEdge+2; jEdge++)
		{
			NextLine = HLineArr[jEdge%numEdge];
			if(NextLine == NULL)	
				continue;
			CurLine->Intersect(*NextLine,crossNextVec,&dNextMyLoc, &dNextUrLoc);
			CurLine->m_P1 = crossNextVec;
			NextLine->m_P0 = crossNextVec;
			break;
		}
	}
	for(iEdge = 0; iEdge < numEdge; iEdge++)
	{
		CurLine = VLineArr[iEdge];
		if(CurLine == NULL)	
			continue;
		for(int jEdge = iEdge+1; jEdge <= numEdge+2; jEdge++)
		{
			NextLine = VLineArr[jEdge%numEdge];
			if(NextLine == NULL)	
				continue;
			CurLine->Intersect(*NextLine,crossNextVec,&dNextMyLoc, &dNextUrLoc);
			CurLine->m_P1 = crossNextVec;
			NextLine->m_P0 = crossNextVec;
			break;
		}
	}
}

