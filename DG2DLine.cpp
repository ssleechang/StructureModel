#include "StdAfx.h"
#include "DG2DLine.h"
#include "GMLib\GM2DLineVector.h"
#include "GMLib\GMQuadTree.h"
#include "GMLib\GM2DArc.h"

#include <math.h>

DG2DLine::DG2DLine(GM2DLineVector* BaseLine, GMQuadTree* pQT)
{
	m_pLine = new GM2DLineVector(*BaseLine);
	m_ObjType = GT_LINE;
	m_pQT = pQT;
	Init();
	SetP0(BaseLine->m_P0);
	SetP1(BaseLine->m_P1);
	AddGMLine(BaseLine);
}

DG2DLine::DG2DLine( GM2DArc* BaseArc , GMQuadTree* pQT)
{
	m_pLine = new GM2DArc(*BaseArc);
	m_ObjType = GT_ARC;
	m_pQT = pQT;
	Init();
	SetP0(BaseArc->GetStart2DVec());
	SetP1(BaseArc->GetEnd2DVec());
	AddGMLine(BaseArc);
}

void DG2DLine::Init()
{
	m_ID = 0;
	m_bSingleLine = FALSE;
	m_LinkColLVec0 = NULL;
	m_LinkColLVec1 = NULL;
	m_Linked0 = NULL;
	m_Linked1 = NULL;
	m_Gapped0 = NULL;
	m_Gapped1 = NULL;
	m_Staggered0 = NULL;
	m_Staggered1 = NULL;
	m_SharePntMask |= (0xFFFF ^ NM_LINKED0 ^ NM_LINKED1);
	m_SharePntMask |= (0xFFFF ^ NM_GAPPED0 ^ NM_GAPPED1);
	m_SharePntMask |= (0xFFFF ^ NM_STAGGERED0 ^ NM_STAGGERED1);
}

DG2DLine::~DG2DLine(void)
{
	if(m_pLine != NULL)
		delete m_pLine;
	// QT가 관리하는 GM들을 참조만 하므로 remove만 시킴
	ma_GMs.RemoveAll();
}

void DG2DLine::AddGMLine(GM2DGeometry* pNewLine)
{
	ma_GMs.Add(pNewLine);
	AddQTs(pNewLine);
}

void DG2DLine::AddQTs(GM2DGeometry* pNewLinee)
{
	vector<GMQuadTree*> NewQTs = m_pQT->FindCrossQts(pNewLinee);
	vector<GMQuadTree*> MyQTs = m_pQT->FindCrossQts(this);
	long NumNewQT = NewQTs.size();
	long NumMyQT = MyQTs.size();

	for(long QTNum=0; QTNum<NumNewQT; QTNum++)
	{
		BOOL IsExist = FALSE;
		for(long MyQTNum=0; MyQTNum<NumMyQT; MyQTNum++)
		{
			if(MyQTs[MyQTNum] == NewQTs[QTNum])
			{
				IsExist = TRUE;
				break;
			}
		}
		if(IsExist==FALSE)
			m_pQT->AddCrossQuadTree(this, NewQTs[QTNum]);
	}
}

BOOL DG2DLine::MergeWhenColinear1(GM2DLineVector* pNewLine, double DegreeTol, double DistTol, double linkTestLength)
{
	if(m_pLine->m_ObjType != GT_LINE)
		return FALSE;

	GM2DLineVector* pLine = (GM2DLineVector*)m_pLine;
	//	long Result = IsColinear4DTol(*pNewLine, DegreeTol, DistTol);
	long Result = pNewLine->IsColinear4DTol(*pLine, DegreeTol, DistTol);

	if(Result == 0)
		return FALSE;

	Result = pLine->Overlap4DTol(*pNewLine, DegreeTol, DistTol, linkTestLength);
	if(Result == 0)
		return FALSE;

	switch(Result)
	{
	case 1:
	return FALSE;
	case 2:
	case 3:
	case 4:
	case 8:
		SetP0(pNewLine->m_P0);
		pNewLine->m_Tag = -1;
	break;
	case 5:
	case 9:
	case 10:
	case 16:
	case 17:
	case 21:
	case 22:
		SetP0(pNewLine->m_P0);
		SetP1(pNewLine->m_P1);
		pNewLine->m_Tag = -1;
	break;
	case 6:
	case 7:
	case 11:
	case 12:
		SetP0(pNewLine->m_P1);
		pNewLine->m_Tag = -1;
	break;
	case 13:
	case 14:
		pNewLine->m_Tag = -1;
	break;
	case 15:
	case 20:
		SetP1(pNewLine->m_P1);
		pNewLine->m_Tag = -1;
	break;
	case 18:
	case 19:
	case 23:
	case 24:
		SetP1(pNewLine->m_P0);
		pNewLine->m_Tag = -1;
	break;
	case 25:
	return FALSE;
	}
	
 	AddGMLine(pNewLine);
	return TRUE;
}

BOOL DG2DLine::MergeWhenColinear2(GM2DLineVector* pNewLine, double DegreeTol, double DistTol, double linkTestLength)
{
	if(m_pLine->m_ObjType != GT_LINE)
		return FALSE;

	GM2DLineVector* pLine = (GM2DLineVector*)m_pLine;

	long Result = pNewLine->IsColinear4DTol(*pLine, DegreeTol, DistTol);

	if(Result == 0)
		return FALSE;

	Result = pLine->Overlap4DTol(*pNewLine, DegreeTol, DistTol, linkTestLength);
	if(Result == 0)
		return FALSE;

	switch(Result)
	{
	case 1:
	case 2:
	case 6:
	case 7:
	case 20:
	case 19:
	case 24:
	case 25:
		return FALSE;
	case 3:
	case 4:
		SetP0(pNewLine->m_P0);
		pNewLine->m_Tag = -1;
		break;
	case 8:
		pNewLine->m_Tag = -1;
		break;
	case 5:
		SetP0(pNewLine->m_P0);
		SetP1(pNewLine->m_P1);
		pNewLine->m_Tag = -1;
		break;
	case 9:
	case 17:
		pNewLine->m_Tag = -1;
		break;
	case 10:
		SetP1(pNewLine->m_P1);
		pNewLine->m_Tag = -1;
		break;
	case 16:
		SetP0(pNewLine->m_P0);
		pNewLine->m_Tag = -1;
		break;
	case 21:
		SetP0(pNewLine->m_P1);
		SetP1(pNewLine->m_P0);
		pNewLine->m_Tag = -1;
		break;
	case 22:
		SetP1(pNewLine->m_P0);
		pNewLine->m_Tag = -1;
		break;
	case 11:
		SetP0(pNewLine->m_P1);
		pNewLine->m_Tag = -1;
		break;
	case 12:
	case 13:
	case 14:
	case 18:
		pNewLine->m_Tag = -1;
		break;
	case 15:
		SetP1(pNewLine->m_P1);
		pNewLine->m_Tag = -1;
		break;
	case 23:
		SetP1(pNewLine->m_P0);
		pNewLine->m_Tag = -1;
		break;
	}
/*
	switch(Result)
	{
	case 1:
		return FALSE;
	case 2:
	case 3:
	case 4:
	case 8:
		m_P1 = pNewLine->m_P1;
		break;
	case 5:
	case 9:
	case 10:
	case 16:
	case 17:
	case 21:
	case 22:
		break;
	case 6:
	case 7:
	case 11:
	case 12:
		m_P0 = pNewLine->m_P1;
		break;
	case 13:
	case 14:
		m_P0 = pNewLine->m_P0;
		m_P1 = pNewLine->m_P1;
		break;
	case 15:
	case 20:
		m_P0 = pNewLine->m_P0;
		break;
	case 18:
	case 19:
	case 23:
	case 24:
		m_P1 = pNewLine->m_P0;
		break;
	case 25:
		return FALSE;
	}
*/
	AddGMLine(pNewLine);
	return TRUE;
}

void DG2DLine::InsertProperty(DG2DLine* PairLine, double Loc0, double Loc1, BOOL bSingleLine)
{
	if(Loc0 > Loc1)
	{
		double Tmp = Loc0;
		Loc0 = Loc1;
		Loc1 = Tmp;
	}

	long NumPair = ma_Pair.GetSize();
	for(long PairNum=0; PairNum<NumPair; PairNum++)
	{
		if(ma_Pair[PairNum]==PairLine)
			return;
	}

	for(long PairNum=0; PairNum<NumPair; PairNum++)
	{
		if(Loc0 > ma_PairLoc0[PairNum])
			continue;
		ma_Pair.InsertAt(PairNum, PairLine);
		ma_PairLoc0.InsertAt(PairNum, Loc0);
		ma_PairLoc1.InsertAt(PairNum, Loc1);
		ma_bSingleLine.InsertAt(PairNum, bSingleLine);
		return;
	}
	ma_Pair.Add(PairLine);
	ma_PairLoc0.Add(Loc0);
	ma_PairLoc1.Add(Loc1);
	ma_bSingleLine.Add(bSingleLine);
}

void DG2DLine::RemovePairLine( long Idx )
{
	if(Idx >= ma_Pair.GetSize())
		return;
	ma_Pair.RemoveAt(Idx);
	ma_PairLoc0.RemoveAt(Idx);
	ma_PairLoc1.RemoveAt(Idx);
	ma_bSingleLine.RemoveAt(Idx);
}

void DG2DLine::RemovePairLineByLine( DG2DLine* ALine )
{
	long NumPair = ma_Pair.GetSize();
	long PairNum;
	for(PairNum=0; PairNum<NumPair; PairNum++)
	{
		if(ma_Pair[PairNum]==ALine)
			break;
	}
	if(PairNum == NumPair)
		return;
	ma_Pair.RemoveAt(PairNum);
	ma_PairLoc0.RemoveAt(PairNum);
	ma_PairLoc1.RemoveAt(PairNum);
	ma_bSingleLine.RemoveAt(PairNum);
}

BOOL DG2DLine::AddPairLine(DG2DLine* PairLine, double Loc0, double Loc1, long Result, double GapTol, double LinkTestTol)
{
	if(Loc0 > Loc1)
	{
		double Temp = Loc0;
		Loc0 = Loc1;
		Loc1 = Temp;
	}

/*
	if(PairLine->m_Linked0 && PairLine->m_Linked1)
	{
		GM2DVector* pLinkOtherEndPnt0 = PairLine->EndOfLink(0);
		GM2DVector* pLinkOtherEndPnt1 = PairLine->EndOfLink(1);

		if(Distance(*pLinkOtherEndPnt0) < LinkTestTol && 
			Distance(*pLinkOtherEndPnt1) < LinkTestTol)
		{
			BOOL bIsSameDir = IsSameDir(PairLine);
			if(bIsSameDir && (m_Linked0 != PairLine->m_Linked0 || m_Linked1 != PairLine->m_Linked1))
			{
				PairLine->m_Tag = 1;
				return FALSE;
			}
			if(!bIsSameDir && (m_Linked1 != PairLine->m_Linked0 || m_Linked0 != PairLine->m_Linked1))
			{
				PairLine->m_Tag = 1;
				return FALSE;
			}
		}
	}
*/

	long NumPair = ma_Pair.GetSize();
	for(long PairNum=0; PairNum<NumPair; PairNum++)
	{
// 		if(Loc1 < ma_PairLoc0[PairNum] || Loc0 > ma_PairLoc1[PairNum])
		if(Loc1 <= (ma_PairLoc0[PairNum]+DTOL_GM) 
		|| Loc0 >= (ma_PairLoc1[PairNum]-DTOL_GM))
			continue;

		double OldLocLength = ma_PairLoc1[PairNum] - ma_PairLoc0[PairNum];
		double NewLocLength = Loc1 - Loc0; 
		if(OldLocLength >= NewLocLength)
		{
// 			PairLine->m_Tag = 1;
			return FALSE;
		}
		else
		{
			ma_Pair[PairNum]->RemovePairLineByLine(this);
			ma_Pair[PairNum]->m_Tag = 0;
			RemovePairLineByLine(ma_Pair[PairNum]);
			NumPair--;
		}
/*
		double OldDist = Distance(ma_Pair[PairNum]->m_P0);
		double NewDist = Distance(PairLine->m_P0);
		if(NewDist >= OldDist)
		{
			PairLine->m_Tag = 1;
			return FALSE;
		}
		else
		{
			ma_Pair[PairNum]->RemovePairLineByLine(this);
			ma_Pair[PairNum]->m_Tag = 0;
			RemovePairLineByLine(ma_Pair[PairNum]);
			NumPair--;
		}
*/
	}
/*
	for(long PairNum=0; PairNum<NumPair; PairNum++)
	{
		DG2DLine* OldLine = ma_Pair[PairNum];
		double OldLoc0 = ma_PairLoc0[PairNum];
		double OldLoc1 = ma_PairLoc1[PairNum];
		double NewLoc0, NewLoc1;
		if(Loc0 < Loc1)
		{
			NewLoc0 = Loc0;
			NewLoc1 = Loc1;
		}
		else
		{
			NewLoc0 = Loc1;
			NewLoc1 = Loc0;
		}
		if(NewLoc1 < OldLoc0 || NewLoc0 > OldLoc1)
			continue;
	}*/


	InsertProperty(PairLine, Loc0, Loc1);

	double UrLoc0 = PairLine->OrthoBaseLocValue(m_P0);
	double UrLoc1 = PairLine->OrthoBaseLocValue(m_P1);

	PairLine->InsertProperty(this, UrLoc0, UrLoc1);

	if(Result==8 || Result==9 || Result==12 || Result==13 || Result==14 || Result==17 || Result==18)
		PairLine->m_Tag = 1;

	double MaxGapValue = GapTol / m_pLine->Length();
	if(MaxGapValue > 0.05)
		MaxGapValue = 0.05;
	if(ma_PairLoc0[0] > 0.01)
		return FALSE;

	NumPair = ma_Pair.GetSize();
	for(long PairNum=1; PairNum<NumPair; PairNum++)
	{
		double GapValue = ma_PairLoc0[PairNum] - ma_PairLoc1[PairNum-1];
		if(GapValue > MaxGapValue)
			return FALSE;
	}
	if((1.-ma_PairLoc1[NumPair-1]) > 0.01)
		return FALSE;

	return TRUE;
}

BOOL DG2DLine::AddPairWall( DG2DLine* PairLine, double Loc0, double Loc1, long Result, double GapTol )
{
	InsertProperty(PairLine, Loc0, Loc1, TRUE);
	m_bSingleLine = TRUE;

	double UrLoc0 = PairLine->OrthoBaseLocValue(m_P0);
	double UrLoc1 = PairLine->OrthoBaseLocValue(m_P1);

	if(Result==8 || Result==9 || Result==12 || Result==13 || Result==14 || Result==17 || Result==18)
		PairLine->m_Tag = 1;

	double MaxGapValue = GapTol / m_pLine->Length();
	if(MaxGapValue > 0.1)
		MaxGapValue = 0.1;
	if(ma_PairLoc0[0] > MaxGapValue)
		return FALSE;

	long NumPair = ma_Pair.GetSize();
	for(long PairNum=1; PairNum<NumPair; PairNum++)
	{
		double GapValue = ma_PairLoc0[PairNum] - ma_PairLoc1[PairNum-1];
		if(GapValue > MaxGapValue)
			return FALSE;
	}
	if((1.-ma_PairLoc1[NumPair-1]) > MaxGapValue)
		return FALSE;

	return TRUE;
}

/*
BOOL DG2DLine::AddPairLine(DG2DLine* PairLine, double Loc0, double Loc1, long Result, double GapTol, double LinkTestTol)
{
	if(PairLine->m_Linked0 && PairLine->m_Linked1)
	{
		GM2DVector* pLinkOtherEndPnt0 = PairLine->EndOfLink(0);
		GM2DVector* pLinkOtherEndPnt1 = PairLine->EndOfLink(1);

		if(Distance(*pLinkOtherEndPnt0) < LinkTestTol && 
			 Distance(*pLinkOtherEndPnt1) < LinkTestTol)
		{
			BOOL bIsSameDir = IsSameDir(PairLine);
			if(bIsSameDir && (m_Linked0 != PairLine->m_Linked0 || m_Linked1 != PairLine->m_Linked1))
			{
				PairLine->m_Tag = 1;
				return FALSE;
			}
			if(!bIsSameDir && (m_Linked1 != PairLine->m_Linked0 || m_Linked0 != PairLine->m_Linked1))
			{
				PairLine->m_Tag = 1;
				return FALSE;
			}
		}
	}

	InsertProperty(PairLine, Loc0, Loc1);

	double UrLoc0 = PairLine->OrthoBaseLocValue(m_P0);
	double UrLoc1 = PairLine->OrthoBaseLocValue(m_P1);

	PairLine->InsertProperty(this, UrLoc0, UrLoc1);

	if(Result==8 || Result==9 || Result==12 || Result==13 || Result==14 || Result==17 || Result==18)
		PairLine->m_Tag = 1;

	double MaxGapValue = GapTol / m_pLine->Length();
	if(MaxGapValue > 0.1)
		MaxGapValue = 0.1;

	long NumPair = ma_Pair.GetSize();
	if(NumPair == 0)
		return FALSE;

	DG2DLine* FirstLink = IsSameDirWithPair(0) ? ma_Pair[0]->m_Linked0 : ma_Pair[0]->m_Linked1;
	if(!(m_Linked0 && FirstLink && m_Linked0->IsParallel4DTol(*FirstLink->m_pLine)) && 
		   ma_PairLoc0[0] > MaxGapValue)
		return FALSE;

	for(long PairNum=1; PairNum<NumPair; PairNum++)
	{
		DG2DLine* PrevLink = IsSameDirWithPair(PairNum-1) ? ma_Pair[PairNum-1]->m_Linked1 : ma_Pair[PairNum-1]->m_Linked0;
		DG2DLine* NextLink = IsSameDirWithPair(PairNum) ? ma_Pair[PairNum]->m_Linked0 : ma_Pair[PairNum]->m_Linked1;

		if(PrevLink && NextLink && PrevLink->IsParallel4DTol(*NextLink->m_pLine))
			continue;

		double GapValue = ma_PairLoc0[PairNum] - ma_PairLoc1[PairNum-1];
		if(GapValue > MaxGapValue)
			return FALSE;
	}

	DG2DLine* LastLink = IsSameDirWithPair(NumPair-1) ? ma_Pair[NumPair-1]->m_Linked1 : ma_Pair[NumPair-1]->m_Linked0;
	if(!(m_Linked1 && LastLink && m_Linked1->IsParallel4DTol(*LastLink->m_pLine)) && 
		  (1.-ma_PairLoc1[NumPair-1]) > MaxGapValue)
		return FALSE;

	return TRUE;
}

BOOL DG2DLine::AddPairWall( DG2DLine* PairLine, double Loc0, double Loc1, long Result, double GapTol )
{
	InsertProperty(PairLine, Loc0, Loc1, TRUE);

	double UrLoc0 = PairLine->OrthoBaseLocValue(m_P0);
	double UrLoc1 = PairLine->OrthoBaseLocValue(m_P1);

	if(Result==8 || Result==9 || Result==12 || Result==13 || Result==14 || Result==17 || Result==18)
		PairLine->m_Tag = 1;

	double MaxGapValue = GapTol / m_pLine->Length();
	if(MaxGapValue > 0.1)
		MaxGapValue = 0.1;

	long NumPair = ma_Pair.GetSize();
	if(NumPair == 0)
		return FALSE;

	DG2DLine* FirstLink = IsSameDirWithPair(0) ? ma_Pair[0]->m_Linked0 : ma_Pair[0]->m_Linked1;
	if(!(m_Linked0 && m_Linked0->IsPair(FirstLink)) && ma_PairLoc0[0] > MaxGapValue)
		return FALSE;

	for(long PairNum=1; PairNum<NumPair; PairNum++)
	{
		DG2DLine* PrevLink = IsSameDirWithPair(PairNum-1) ? ma_Pair[PairNum-1]->m_Linked1 : ma_Pair[PairNum-1]->m_Linked0;
		DG2DLine* NextLink = IsSameDirWithPair(PairNum) ? ma_Pair[PairNum]->m_Linked0 : ma_Pair[PairNum]->m_Linked1;
		
		if(PrevLink && NextLink && PrevLink->IsPair(NextLink))
			continue;

		double GapValue = ma_PairLoc0[PairNum] - ma_PairLoc1[PairNum-1];
		if(GapValue > MaxGapValue)
			return FALSE;
	}

	DG2DLine* LastLink = IsSameDirWithPair(NumPair-1) ? ma_Pair[NumPair-1]->m_Linked1 : ma_Pair[NumPair-1]->m_Linked0;
	if(!(m_Linked1 && m_Linked1->IsPair(LastLink)) && (1.-ma_PairLoc1[NumPair-1]) > MaxGapValue)
		return FALSE;

	return TRUE;
}
*/
double DG2DLine::Length()
{
	return m_pLine->Length();
}

double DG2DLine::PairLength()
{
	double dPairLenth = 0.;
	long NumPair = ma_Pair.GetSize();
	for(long iPair=0; iPair<NumPair; iPair++)
	{
		DG2DLine* pPairLine = ma_Pair[iPair];
		dPairLenth += pPairLine->Length();
	}
	return dPairLenth;
}

DG2DLine* DG2DLine::GetAPairLine(GM2DVector& LocVec)
{
	long NumPair = ma_Pair.GetSize();
	for(long iPair=0; iPair<NumPair; iPair++)
	{
		DG2DLine* pPairLine = ma_Pair[iPair];
		double LocVal =	OrthoBaseLocValue(LocVec);
		if(LocVal > -0.01 && LocVal < 1.01)
			return pPairLine;
	}
	return NULL;
}
BOOL DG2DLine::IsSameDirWithPair(long PairIdx)
{
	double Dir = DirVector() * ma_Pair[PairIdx]->DirVector();
	if(Dir > 0.)
		return TRUE;
	return FALSE;
}

BOOL DG2DLine::IsSameDir(DG2DLine* UrLine)
{
	double Dir = DirVector() * UrLine->DirVector();
	if(Dir > 0.)
		return TRUE;
	return FALSE;
}

BOOL DG2DLine::IsSameDir(GM2DLineVector* UrLine)
{
	double Dir = DirVector() * UrLine->DirVector();
	if(Dir > 0.)
		return TRUE;
	return FALSE;
}

GM2DVector* DG2DLine::EndOfLink(long StartOrEnd)
{
	if(StartOrEnd == 0)
	{
		if(m_Linked0 == NULL)
			return NULL;
		if(m_SharePntMask & NM_LINKED0)
			return &m_Linked0->m_P0;
		else
			return &m_Linked0->m_P1;
	}
	else
	{
		if(m_Linked1 == NULL)
			return NULL;
		if(m_SharePntMask & NM_LINKED1)
			return &m_Linked1->m_P0;
		else
			return &m_Linked1->m_P1;
	}
}

GM2DVector* DG2DLine::EndOfGivenLink(DG2DLine* pLinkLine)
{
	if(!pLinkLine)
		return NULL;
	if(!IsLink(pLinkLine))
		return NULL;

	if(pLinkLine == m_Linked0)
		return EndOfLink(0);
	else
		return EndOfLink(1);
}

DG2DLine* DG2DLine::GetLinkNext( DG2DLine* pLinkLine )
{
	if(!pLinkLine)
		return NULL;
	if(!IsLink(pLinkLine))
		return NULL;

	DG2DLine* pNxtLink=NULL;
	if(pLinkLine == m_Linked0)
		pNxtLink = (m_SharePntMask & NM_LINKED0) == 0 ? pLinkLine->m_Linked1 : pLinkLine->m_Linked0;
	else
		pNxtLink = (m_SharePntMask & NM_LINKED1) == 0 ? pLinkLine->m_Linked1 : pLinkLine->m_Linked0;

	return pNxtLink;
}

DG2DLine* DG2DLine::GetLinkNext( long StartOrEnd, bool bIncludeGap/*==TRUE*/, bool bIncludeStagger )
{
	DG2DLine* pLinkNext = NULL;
	DG2DLine* pLink = NULL;
	long NextEnd;

	if(StartOrEnd==0 && m_Linked0)
	{
		pLink = m_Linked0;
		NextEnd = m_SharePntMask & NM_LINKED0;
	}
	else if(StartOrEnd==1 && m_Linked1)
	{
		pLink = m_Linked1;
		NextEnd = m_SharePntMask & NM_LINKED1;
	}
	else
		return NULL;

	if(!bIncludeGap && !bIncludeStagger)
		return pLink;

	while(1)
	{
		DG2DLine* pNextLine = NULL;
		if(bIncludeGap)
		{
			if(NextEnd==0 && pLink->m_Gapped1)
			{
				NextEnd = pLink->m_SharePntMask & NM_GAPPED1;
				pNextLine = pLink->m_Gapped1;
			}
			else if(NextEnd==1 && pLink->m_Gapped0)
			{
				NextEnd = pLink->m_SharePntMask & NM_GAPPED0;
				pNextLine = pLink->m_Gapped0;
			}
			if(pNextLine)
			{
				pLink = pNextLine;
				continue;
			}
		}
		if(bIncludeStagger)
		{
			if(NextEnd==0 && pLink->m_Staggered1)
			{
				NextEnd = pLink->m_SharePntMask & NM_STAGGERED1;
				pNextLine = pLink->m_Staggered1;
			}
			else if(NextEnd==1 && pLink->m_Staggered0)
			{
				NextEnd = pLink->m_SharePntMask & NM_STAGGERED0;
				pNextLine = pLink->m_Staggered0;
			}
			if(pNextLine)
			{
				pLink = pNextLine;
				continue;
			}
		}
		break;
	}

	pLinkNext = NextEnd==0 ? pLink->m_Linked1 : pLink->m_Linked0;
	return pLinkNext;
}

DG2DLine* DG2DLine::GetGapEndLinkLine(long StartOrEnd)
{
	DG2DLine* pLinkNext = NULL;
	long NextEnd;

	if(StartOrEnd==0 && m_Gapped0)
	{
		NextEnd = m_SharePntMask & NM_GAPPED0;
		pLinkNext = NextEnd==0 ? m_Gapped0->m_Linked1 : m_Gapped0->m_Linked0;
	}
	else if(StartOrEnd==1 && m_Gapped1)
	{
		NextEnd = m_SharePntMask & NM_GAPPED1;
		pLinkNext = NextEnd==0 ? m_Gapped1->m_Linked1 : m_Gapped1->m_Linked0;
	}
	else
		return NULL;

	return pLinkNext;
}


GM2DVector DG2DLine::GetMidVectorNearest(GM2DVector* pVec, DG2DLine* UrLine)
{
	GM2DVector RtnVec, UrVec;
	if(pVec->CloserP(&UrLine->m_P0, &UrLine->m_P1)==0)
		UrVec = UrLine->m_P0;
	else
		UrVec = UrLine->m_P1;


	RtnVec.m_X = (pVec->m_X+UrVec.m_X)/2.;
	RtnVec.m_Y = (pVec->m_Y+UrVec.m_Y)/2.;
	return RtnVec;
}

BOOL DG2DLine::IsPair(DG2DLine* UrLine)
{
	if(UrLine==NULL)
		return FALSE;
	long NumMyPair = ma_Pair.GetSize();
	for(long iPair=0; iPair<NumMyPair; iPair++)
	{
		if(ma_Pair[iPair]==UrLine)
			return TRUE;
	}
	long NumUrPair = UrLine->ma_Pair.GetSize();
	for(iPair=0; iPair<NumUrPair; iPair++)
	{
		if(UrLine->ma_Pair[iPair]==this)
			return TRUE;
	}
	return FALSE;
}

BOOL DG2DLine::IsLink(DG2DLine* UrLine)
{
	if(UrLine == NULL)
		return FALSE;
	if(m_Linked0 == UrLine || m_Linked1 == UrLine)
		return TRUE;
	return FALSE;
}

BOOL DG2DLine::IsGapped(DG2DLine* UrLine)
{
	if(UrLine == NULL)
		return FALSE;
	if(m_Gapped0 == UrLine || m_Gapped1 == UrLine)
		return TRUE;
	return FALSE;
}

long DG2DLine::GetLinkState()
{
	if(m_Linked0 == NULL && m_Linked1 == NULL)
		return -1;
	if(m_Linked0 && m_Linked1 == NULL)
		return 0;
	if(m_Linked0 == NULL && m_Linked1)
		return 1;
	//	if(m_Linked0 && m_Linked1)
	return 2;
}

long DG2DLine::GetColLinkState()
{
	if(m_LinkColLVec0 == NULL && m_LinkColLVec1 == NULL)
		return -1;
	if(m_LinkColLVec0 && m_LinkColLVec1 == NULL)
		return 0;
	if(m_LinkColLVec0 == NULL && m_LinkColLVec1)
		return 1;
	//	if(m_Linked0 && m_Linked1)
	return 2;
}

long DG2DLine::GetGapState()
{
	if(m_Gapped0 == NULL && m_Gapped1 == NULL)
		return -1;
	if(m_Gapped0 && m_Gapped1 == NULL)
		return 0;
	if(m_Gapped0 == NULL && m_Gapped1)
		return 1;
	//	if(m_Gapped0 && m_Gapped1)
	return 2;
}

long DG2DLine::GetStaggerState()
{
	if(m_Staggered0 == NULL && m_Staggered1 == NULL)
		return -1;
	if(m_Staggered0 && m_Staggered1 == NULL)
		return 0;
	if(m_Staggered0 == NULL && m_Staggered1)
		return 1;
	//	if(m_Staggered0 && m_Staggered1)
	return 2;
}

// Kind : NM_XXXX 매크로 지정, Value :  0 또는 1 값
void DG2DLine::SetMask( long Kind, long Value )
{
	if(Value==0)
		m_SharePntMask &= (0xFFFF ^ Kind);  // Kind bit를 0으로 세팅
	else
		m_SharePntMask |= Kind;  // Kind bit를 1로 세팅. 즉 마이시작점에 유어 끝점이 연결됨
}

long DG2DLine::GetMask( long Kind )
{
  return m_SharePntMask & Kind;
}

BOOL DG2DLine::SetLinkLine(long StartOrEnd, DG2DLine* UrLine, double dTol)
{
	if(UrLine == NULL)
		return FALSE;

	if(StartOrEnd == 0)
	{
		if(UrLine->m_Linked0==NULL && m_P0.Distance(UrLine->m_P0) < dTol)
		{
			m_Linked0 = UrLine;
			m_SharePntMask &= (0xFFFF ^ NM_LINKED0);  // NM_LINKED0 bit를 0으로 세팅
			UrLine->m_Linked0=this;
			UrLine->m_SharePntMask &= (0xFFFF ^ NM_LINKED0);
		}
		else if(UrLine->m_Linked1==NULL && m_P0.Distance(UrLine->m_P1) < dTol)
		{
			m_Linked0 = UrLine;
			m_SharePntMask |= NM_LINKED0;  // NM_LINKED0 bit를 1로 세팅. 즉 마이시작점에 유어 끝점이 연결됨
			UrLine->m_Linked1=this;
			UrLine->m_SharePntMask &= (0xFFFF ^ NM_LINKED1);
		}
		else
			return FALSE;
	}
	else
	{
		if(UrLine->m_Linked0==NULL && m_P1.Distance(UrLine->m_P0) < dTol)
		{
			m_Linked1 = UrLine;
			m_SharePntMask &= (0xFFFF ^ NM_LINKED1);
			UrLine->m_Linked0=this;
			UrLine->m_SharePntMask |= NM_LINKED0;
		}
		else if(UrLine->m_Linked1==NULL && m_P1.Distance(UrLine->m_P1) < dTol)
		{
			m_Linked1 = UrLine;
			m_SharePntMask |= NM_LINKED1;
			UrLine->m_Linked1=this;
			UrLine->m_SharePntMask |= NM_LINKED1;
		}
		else
			return FALSE;
	}
	return TRUE;
}

BOOL DG2DLine::ReleaseLinkLine(DG2DLine* UrLine)
{
	if(UrLine == NULL)
		return FALSE;

	if(m_Linked0 == UrLine)
	{
		m_Linked0 = NULL;
		if(m_P0.Distance(UrLine->m_P0) < m_P0.Distance(UrLine->m_P1))
			UrLine->m_Linked0 = NULL;
		else
			UrLine->m_Linked1 = NULL;
		return TRUE;
	}
	else if(m_Linked1 == UrLine)
	{
		m_Linked1 = NULL;
		if(m_P1.Distance(UrLine->m_P0) < m_P1.Distance(UrLine->m_P1))
			UrLine->m_Linked0 = NULL;
		else
			UrLine->m_Linked1 = NULL;
		return TRUE;
	}

	return FALSE;
}


BOOL DG2DLine::ExistCrossWall(long StartOrEnd, DG2DLine* UrLine, double DegreeTol, double StepTol)
{
	long Result = IsParallel4DTol(*UrLine, DegreeTol);
	if(Result == FALSE)
		return FALSE;

	double Step = UrLine->Distance(m_P0);
	if(Step > StepTol)
		return FALSE;

	BOOL bSameDir = IsSameDir(UrLine);
	DG2DLine* UrLinked = NULL;
	DG2DLine* MyLinked = NULL;

	MyLinked = StartOrEnd == 0 ? m_Linked0 : m_Linked1;
	if(MyLinked == NULL)
		return FALSE;

	if(StartOrEnd == 0)
	{
		if(bSameDir)
		{
			double UrLocVal = OrthoBaseLocValue(UrLine->m_P1);
			if(UrLocVal > -DTOL_GM)
				return FALSE;
			UrLinked = UrLine->m_Linked1;
		}
		else
		{
			double UrLocVal = OrthoBaseLocValue(UrLine->m_P0);
			if(UrLocVal > -DTOL_GM)
				return FALSE;
			UrLinked = UrLine->m_Linked0;
		}
	}
	else
	{
		if(bSameDir)
		{
			double UrLocVal = OrthoBaseLocValue(UrLine->m_P0);
			if(UrLocVal < 1.+DTOL_GM)
				return FALSE;
			UrLinked = UrLine->m_Linked0;
		}
		else
		{
			double UrLocVal = OrthoBaseLocValue(UrLine->m_P1);
			if(UrLocVal < 1.+DTOL_GM)
				return FALSE;
			UrLinked = UrLine->m_Linked1;
		}
	}

	if(UrLinked == NULL)
		return FALSE;

	return MyLinked->IsPair(UrLinked);
}

BOOL DG2DLine::ExistCrossColumn(long StartOrEnd, DG2DLine* UrLine, double DegreeTol, double StepTol)
{
	long Result = IsParallel4DTol(*UrLine, DegreeTol);
	if(Result == FALSE)
		return FALSE;

	double Step = UrLine->Distance(m_P0);
	if(Step > StepTol)
		return FALSE;

	BOOL bSameDir = IsSameDir(UrLine);
	GM2DLineVector* UrLinked = NULL;
	GM2DLineVector* MyLinked = NULL;

	MyLinked = (GM2DLineVector*)(StartOrEnd == 0 ? m_LinkColLVec0 : m_LinkColLVec1);
	if(MyLinked == NULL)
		return FALSE;

	if(StartOrEnd == 0)
	{
		if(bSameDir)
		{
			double UrLocVal = OrthoBaseLocValue(UrLine->m_P1);
			if(UrLocVal > -DTOL_GM)
				return FALSE;
			UrLinked = UrLine->m_LinkColLVec1;
		}
		else
		{
			double UrLocVal = OrthoBaseLocValue(UrLine->m_P0);
			if(UrLocVal > -DTOL_GM)
				return FALSE;
			UrLinked = UrLine->m_LinkColLVec0;
		}
	}
	else
	{
		if(bSameDir)
		{
			double UrLocVal = OrthoBaseLocValue(UrLine->m_P0);
			if(UrLocVal < 1.+DTOL_GM)
				return FALSE;
			UrLinked = UrLine->m_LinkColLVec0;
		}
		else
		{
			double UrLocVal = OrthoBaseLocValue(UrLine->m_P1);
			if(UrLocVal < 1.+DTOL_GM)
				return FALSE;
			UrLinked = UrLine->m_LinkColLVec1;
		}
	}

	if(UrLinked == NULL)
		return FALSE;

	if(MyLinked->IsParallel4DTol(*UrLinked, DegreeTol))
		return TRUE;
	else
		return FALSE;
}

/*
BOOL DG2DLine::SetGapLine(long StartOrEnd, DG2DLine* UrLine, double DegreeTol, double DistTol, double StepTol, double GapTol)
{
	if(UrLine == NULL)
		return FALSE;

	BOOL Result = ExistCrossWall(StartOrEnd, UrLine, DegreeTol, StepTol);
	if(Result==FALSE)
		Result = ExistCrossColumn(StartOrEnd, UrLine, DegreeTol, StepTol);
	if(Result==FALSE)
		return FALSE;
/ *
	long Result = IsParallel4DTol(*(UrLine->m_pLine), DegreeTol);
	if(Result == FALSE)
		return FALSE;

	double Step = UrLine->Distance(m_P0);
	if(Step > StepTol)
		return FALSE;
* /
	BOOL bSameDir = IsSameDir(UrLine);
	if(StartOrEnd == 0)
	{
		if(!bSameDir && UrLine->m_Gapped0==NULL && m_P0.Distance(UrLine->m_P0) < GapTol)
		{
			m_Gapped0 = UrLine;
			m_SharePntMask &= (0xFFFF ^ NM_GAPPED0);
			UrLine->m_Gapped0=this;
			UrLine->m_SharePntMask &= (0xFFFF ^ NM_GAPPED0);
		}
		else if(bSameDir && UrLine->m_Gapped1==NULL && m_P0.Distance(UrLine->m_P1) < GapTol)
		{
			m_Gapped0 = UrLine;
			m_SharePntMask |= NM_GAPPED0;
			UrLine->m_Gapped1=this;
			UrLine->m_SharePntMask &= (0xFFFF ^ NM_GAPPED0);
		}
		else
			return FALSE;
	}
	else
	{
		if(!bSameDir && UrLine->m_Gapped1==NULL && m_P1.Distance(UrLine->m_P1) < GapTol)
		{
			m_Gapped1 = UrLine;
			m_SharePntMask |= NM_GAPPED1;
			UrLine->m_Gapped1=this;
			UrLine->m_SharePntMask |= NM_GAPPED1;
		}
		else if(bSameDir && UrLine->m_Gapped0==NULL && m_P1.Distance(UrLine->m_P0) < GapTol)
		{
			m_Gapped1 = UrLine;
			m_SharePntMask &= (0xFFFF ^ NM_GAPPED1);
			UrLine->m_Gapped0=this;
			UrLine->m_SharePntMask &= (0xFFFF ^ NM_GAPPED1);
		}
		else
			return FALSE;
	}
	return TRUE;
}
*/


BOOL DG2DLine::SetGapLine(long StartOrEnd, DG2DLine* UrLine, double DegreeTol, double DistTol, double StepTol, double GapTol)
{
	if(UrLine == NULL)
		return FALSE;

// 	if(UrLine->Length() <= GapTol)
// 		return FALSE;

/*
	BOOL Result = ExistCrossWall(StartOrEnd, UrLine, DegreeTol, StepTol);
	if(Result==FALSE)
		Result = ExistCrossColumn(StartOrEnd, UrLine, DegreeTol, StepTol);
	if(Result==FALSE)
		return FALSE;
*/

	long Result = IsColinear4DTol(*UrLine);
	if(Result == FALSE)
		return FALSE;

	double Dist0, Dist1;
	BOOL bSameDir = IsSameDir(UrLine);
	if(StartOrEnd == 0)
	{
		Dist0 = m_P0.Distance(UrLine->m_P0);
		Dist1 = m_P0.Distance(UrLine->m_P1);
		if(!bSameDir && UrLine->m_Gapped0==NULL && 
			 Dist0 < Dist1 &&
			 m_P0.Distance(UrLine->m_P0) < GapTol && 
			 m_P0.Distance(UrLine->m_P0) > 10.)
		{
			m_Gapped0 = UrLine;
			m_SharePntMask &= (0xFFFF ^ NM_GAPPED0);
			UrLine->m_Gapped0=this;
			UrLine->m_SharePntMask &= (0xFFFF ^ NM_GAPPED0);
		}
		else if(bSameDir && UrLine->m_Gapped1==NULL && 
						Dist0 > Dist1 &&
			      m_P0.Distance(UrLine->m_P1) < GapTol && 
						m_P0.Distance(UrLine->m_P1) > 10.)
		{
			m_Gapped0 = UrLine;
			m_SharePntMask |= NM_GAPPED0;
			UrLine->m_Gapped1=this;
			UrLine->m_SharePntMask &= (0xFFFF ^ NM_GAPPED1);
		}
		else
			return FALSE;
	}
	else
	{
		Dist0 = m_P1.Distance(UrLine->m_P0);
		Dist1 = m_P1.Distance(UrLine->m_P1);
		if(!bSameDir && UrLine->m_Gapped1==NULL && 
			 Dist1 < Dist0 &&
			 m_P1.Distance(UrLine->m_P1) < GapTol && 
			 m_P1.Distance(UrLine->m_P1) > 10.)
		{
			m_Gapped1 = UrLine;
			m_SharePntMask |= NM_GAPPED1;
			UrLine->m_Gapped1=this;
			UrLine->m_SharePntMask |= NM_GAPPED1;
		}
		else if(bSameDir && UrLine->m_Gapped0==NULL && 
			      Dist0 < Dist1 &&
			      m_P1.Distance(UrLine->m_P0) < GapTol && 
						m_P1.Distance(UrLine->m_P0) > 10.)
		{
			m_Gapped1 = UrLine;
			m_SharePntMask &= (0xFFFF ^ NM_GAPPED1);
			UrLine->m_Gapped0=this;
			UrLine->m_SharePntMask |= NM_GAPPED0;
		}
		else
			return FALSE;
	}
	return TRUE;
}
/*
BOOL DG2DLine::SetStaggerLine(long StartOrEnd, DG2DLine* UrLine, double DegreeTol, double StepTol, double GapTol)
{
	if(UrLine == NULL)
		return FALSE;

	if(IsPair(UrLine))
		return FALSE;

	long Result = IsParallel4DTol(*(UrLine->m_pLine), DegreeTol);
	if(Result == FALSE)
		return FALSE;

	double Step = UrLine->Distance(m_P0);
	if(Step > StepTol)
		return FALSE;

	double P0LocDist = fabs(OrthoBaseLocValue(UrLine->m_P0) * Length());
	double P1LocDist = fabs(OrthoBaseLocValue(UrLine->m_P1) * Length());
	BOOL bSameDir = IsSameDir(UrLine);
	if(StartOrEnd == 0)
	{
		if(!bSameDir && UrLine->m_Staggered0==NULL && P0LocDist < GapTol)
		{
			m_Staggered0 = UrLine;
			m_SharePntMask &= (0xFFFF ^ NM_STAGGERED0);
			UrLine->m_Staggered0=this;
			UrLine->m_SharePntMask &= (0xFFFF ^ NM_STAGGERED0);
		}
		else if(bSameDir && UrLine->m_Staggered1==NULL && P1LocDist < GapTol)
		{
			m_Staggered0 = UrLine;
			m_SharePntMask |= NM_STAGGERED0;
			UrLine->m_Staggered1=this;
			UrLine->m_SharePntMask &= (0xFFFF ^ NM_STAGGERED0);
		}
		else
			return FALSE;
	}
	else
	{
		if(!bSameDir && UrLine->m_Staggered1==NULL && P1LocDist < GapTol)
		{
			m_Staggered1 = UrLine;
			m_SharePntMask |= NM_STAGGERED1;
			UrLine->m_Staggered1=this;
			UrLine->m_SharePntMask |= NM_STAGGERED1;
		}
		else if(bSameDir && UrLine->m_Staggered0==NULL && P0LocDist < GapTol)
		{
			m_Staggered1 = UrLine;
			m_SharePntMask &= (0xFFFF ^ NM_STAGGERED1);
			UrLine->m_Staggered0=this;
			UrLine->m_SharePntMask &= (0xFFFF ^ NM_STAGGERED1);
		}
		else
			return FALSE;
	}
	return TRUE;
}
*/
BOOL DG2DLine::SetStaggerLine(long StartOrEnd, DG2DLine* UrLine, double DegreeTol, double StepTol, double GapTol)
{
	if(UrLine == NULL)
		return FALSE;

	if(IsPair(UrLine))
		return FALSE;

	long Result = IsParallel4DTol(*UrLine, DegreeTol);
	if(Result == FALSE)
		return FALSE;

	double Step = UrLine->Distance(m_P0);
	if(Step > StepTol)
		return FALSE;

	double P0LocDist;
	double P1LocDist; 
	BOOL bSameDir = IsSameDir(UrLine);
	if(StartOrEnd == 0)
	{
		P0LocDist = fabs(OrthoBaseLocValue(UrLine->m_P0) * Length());
		P1LocDist = fabs(OrthoBaseLocValue(UrLine->m_P1) * Length());
		if(!bSameDir && UrLine->m_Staggered0==NULL && P0LocDist < GapTol)
		{
			m_Staggered0 = UrLine;
			m_SharePntMask &= (0xFFFF ^ NM_STAGGERED0);
			UrLine->m_Staggered0=this;
			UrLine->m_SharePntMask &= (0xFFFF ^ NM_STAGGERED0);
		}
		else if(bSameDir && UrLine->m_Staggered1==NULL && P1LocDist < GapTol)
		{
			m_Staggered0 = UrLine;
			m_SharePntMask |= NM_STAGGERED0;
			UrLine->m_Staggered1=this;
			UrLine->m_SharePntMask &= (0xFFFF ^ NM_STAGGERED1);
		}
		else
			return FALSE;
	}
	else
	{
		P0LocDist = fabs((OrthoBaseLocValue(UrLine->m_P0)-1) * Length());
		P1LocDist = fabs((OrthoBaseLocValue(UrLine->m_P1)-1) * Length());
		if(!bSameDir && UrLine->m_Staggered1==NULL && P1LocDist < GapTol)
		{
			m_Staggered1 = UrLine;
			m_SharePntMask |= NM_STAGGERED1;
			UrLine->m_Staggered1=this;
			UrLine->m_SharePntMask |= NM_STAGGERED1;
		}
		else if(bSameDir && UrLine->m_Staggered0==NULL && P0LocDist < GapTol)
		{
			m_Staggered1 = UrLine;
			m_SharePntMask &= (0xFFFF ^ NM_STAGGERED1);
			UrLine->m_Staggered0=this;
			UrLine->m_SharePntMask |= NM_STAGGERED0;
		}
		else
			return FALSE;
	}
	return TRUE;
}

BOOL DG2DLine::GetCenterLVec(GM2DLineVector& CenLVec, long PairNum)
{
	if(PairNum >= ma_Pair.GetSize())
		return FALSE;
	DG2DLine* UrLine = NULL;
	if(PairNum==-1)
	{
		long NumPair = ma_Pair.GetSize();
		for(long iPair=0; iPair<NumPair; iPair++)
		{
			if(ma_Pair[iPair]->m_Tag!=2)
				break;
		}
		if(iPair < 0 || iPair == NumPair)
			return FALSE;
		UrLine = ma_Pair[iPair];
	}
	else
		UrLine = ma_Pair[PairNum];

	if(UrLine->m_pLine->m_ObjType != GT_LINE)
		return FALSE;

	GM2DVector OrthoVec = UrLine->OrthoBaseVector(m_P0);
	GM2DVector TranVec = (OrthoVec - m_P0) * 0.5;
	CenLVec = *((GM2DLineVector*)m_pLine);
	CenLVec.Translate(TranVec);
	return TRUE;
}

DG2DLine* DG2DLine::GetLinkLine(long StartOrEnd)
{
	if(StartOrEnd == 0)
		return m_Linked0;
	else
		return m_Linked1;
/*
	if(ma_Pair.GetSize()==0)
		return NULL;

	if(StartOrEnd==0)
		return ma_Pair[0];
	else
		return ma_Pair[ma_Pair.GetSize()-1];
*/
}

long DG2DLine::CheckEccCrossJoint(long StartOrEnd, GM2DVector& CrossPnt)
{
	if(ma_Pair.GetSize() == 0)
		return FALSE;

	DG2DLine* UrLine, *MyLinked, *UrLinked;
	DG2DLine* MyGapped, *UrGapped;
	BOOL SameDirMeU;
	long UrGappedEnd;

	if(StartOrEnd == 0)
	{
		UrLine = ma_Pair[0];
		SameDirMeU = IsSameDir(UrLine);
		MyGapped = m_Gapped0;
		UrGapped = SameDirMeU ? UrLine->m_Gapped0 : UrLine->m_Gapped1;
		if(UrGapped==NULL)	return FALSE;

		if(IsSameDir(UrGapped))
			UrGappedEnd = 1;
		else
			UrGappedEnd = 0;
		if(MyGapped==NULL || UrGapped==NULL || MyGapped->IsPair(UrGapped)==FALSE)
			return FALSE;
		MyLinked = m_Linked0;
		UrLinked = SameDirMeU ? UrLine->m_Linked0 : UrLine->m_Linked1;
	}
	else
	{
		UrLine = ma_Pair[ma_Pair.GetSize()-1];
		SameDirMeU = IsSameDir(UrLine);
		MyGapped = m_Gapped1;
		UrGapped = SameDirMeU ? UrLine->m_Gapped1 : UrLine->m_Gapped0;
		if(UrGapped==NULL)	return FALSE;
		if(IsSameDir(UrGapped))
			UrGappedEnd = 0;
		else
			UrGappedEnd = 1;
		if(MyGapped==NULL || UrGapped==NULL || MyGapped->IsPair(UrGapped)==FALSE)
			return FALSE;
		MyLinked = m_Linked1;
		UrLinked = SameDirMeU ? UrLine->m_Linked1 : UrLine->m_Linked0;
	}

	if(MyLinked == NULL || MyLinked->IsGapped(UrLinked)==FALSE)
		return FALSE;

	GM2DVector Cross1, Cross2;
	BOOL Result;
	Result = GetCrossPointWithLink(StartOrEnd, Cross1);
	if(Result!=0 && Result!=-5)
		return -1;
	Result = UrGapped->GetCrossPointWithLink(UrGappedEnd, Cross2);
	if(Result!=0 && Result!=-5)
		return -1;
	CrossPnt = (Cross1 + Cross2) * 0.5;
	return TRUE;
}

/*
{
if(ma_Pair.GetSize() == 0)
return FALSE;

DG2DLine* UrLine, *MyGapped, *UrGapped, *MyLinkPair, *UrLinkPair;
GM2DLineVector MyGapLVec, UrGapLVec, MyGappedLVec, UrGappedLVec;
BOOL SameDirMeU = IsSameDir(UrLine);

if(StartOrEnd == 0)
{
UrLine = ma_Pair[0];
MyGapped = m_Gapped0;
UrGapped = SameDirMeU ? UrLine->m_Gapped0 : UrLine->m_Gapped1;
if(MyGapped==NULL || UrGapped==NULL || MyGapped->IsPair(UrGapped)==FALSE)
return FALSE;

MyGapLVec.m_P0 = m_P0;
MyGapLVec.m_P1 = IsSameDir(MyGapped) ? MyGapped->m_P1 : MyGapped->m_P0;

if(SameDirMeU)
{
UrGapLVec.m_P0 = UrGapped->m_P0;
UrGapLVec.m_P1 = UrLine->IsSameDir(UrGapped) ? UrGapped->m_P1 : UrGapped->m_P0;
}
else
{
UrGapLVec.m_P0 = UrGapped->m_P1;
UrGapLVec.m_P1 = UrLine->IsSameDir(UrGapped) ? UrGapped->m_P0 : UrGapped->m_P1;
}
}
else
{
UrLine = ma_Pair[ma_Pair.GetSize()-1];
MyGapped = m_Gapped1;
UrGapped = SameDirMeU ? UrLine->m_Gapped1 : UrLine->m_Gapped0;
if(MyGapped==NULL || UrGapped==NULL || MyGapped->IsPair(UrGapped)==FALSE)
return FALSE;

MyGapLVec.m_P0 = m_P1;
MyGapLVec.m_P1 = IsSameDir(MyGapped) ? MyGapped->m_P0 : MyGapped->m_P1;

if(SameDirMeU)
{
UrGapLVec.m_P0 = UrGapped->m_P1;
UrGapLVec.m_P1 = UrLine->IsSameDir(UrGapped) ? UrGapped->m_P0 : UrGapped->m_P1;
}
else
{
UrGapLVec.m_P0 = UrGapped->m_P0;
UrGapLVec.m_P1 = UrLine->IsSameDir(UrGapped) ? UrGapped->m_P1 : UrGapped->m_P0;
}
}
double UrP0Loc, UrP1Loc;
UrP0Loc = MyGapLVec.OrthoBaseLocValue(UrGapLVec.m_P0);
UrP1Loc = MyGapLVec.OrthoBaseLocValue(UrGapLVec.m_P1);
}
*/

long DG2DLine::CheckYJoint(long StartOrEnd, GM2DVector& CrossPnt)
{
	if(ma_Pair.GetSize() == 0)
		return FALSE;

	DG2DLine* UrLine, *MyLinked, *UrLinked, *MyLinkPair, *UrLinkPair;

	if(StartOrEnd == 0)
	{
		UrLine = ma_Pair[0];
		MyLinked = m_Linked0;
		UrLinked = IsSameDir(UrLine) ? UrLine->m_Linked0 : UrLine->m_Linked1;
		if(MyLinked==NULL || UrLinked==NULL || MyLinked->ma_Pair.GetSize() == 0 || UrLinked->ma_Pair.GetSize() == 0)
			return FALSE;
		MyLinkPair = (m_SharePntMask & NM_LINKED0) == 0 ? MyLinked->ma_Pair[0] : MyLinked->ma_Pair[MyLinked->ma_Pair.GetSize()-1];
		if(IsSameDir(UrLine))
			UrLinkPair = (UrLine->m_SharePntMask & NM_LINKED0) == 0 ? UrLinked->ma_Pair[0] : UrLinked->ma_Pair[UrLinked->ma_Pair.GetSize()-1];
		else
			UrLinkPair = (UrLine->m_SharePntMask & NM_LINKED1) == 0 ? UrLinked->ma_Pair[0] : UrLinked->ma_Pair[UrLinked->ma_Pair.GetSize()-1];
	}
	else
	{
		UrLine = ma_Pair[ma_Pair.GetSize()-1];
		MyLinked = m_Linked1;
		UrLinked = IsSameDir(UrLine) ? UrLine->m_Linked1 : UrLine->m_Linked0;
		if(MyLinked==NULL || UrLinked==NULL || MyLinked->ma_Pair.GetSize() == 0 || UrLinked->ma_Pair.GetSize() == 0)
			return FALSE;
		MyLinkPair = (m_SharePntMask & NM_LINKED1) == 0 ? MyLinked->ma_Pair[0] : MyLinked->ma_Pair[MyLinked->ma_Pair.GetSize()-1];
		if(IsSameDir(UrLine))
			UrLinkPair = (UrLine->m_SharePntMask & NM_LINKED1) == 0 ? UrLinked->ma_Pair[0] : UrLinked->ma_Pair[UrLinked->ma_Pair.GetSize()-1];
		else
			UrLinkPair = (UrLine->m_SharePntMask & NM_LINKED0) == 0 ? UrLinked->ma_Pair[0] : UrLinked->ma_Pair[UrLinked->ma_Pair.GetSize()-1];
	}

	if(MyLinkPair==NULL || UrLinkPair==NULL || MyLinkPair->IsLink(UrLinkPair)==FALSE)
		return FALSE;

	GM2DLineVector MyLVec, MyLinkLVec, UrLinkLVec;
	BOOL MyResult, MyLinkResult, UrLinkResult;

	if(StartOrEnd == 0)
	{
		MyResult = GetCenterLVec(MyLVec, 0);
		if((m_SharePntMask & NM_LINKED0) == 0)
			MyLinkResult = MyLinked->GetCenterLVec(MyLinkLVec, 0);
		else
			MyLinkResult = MyLinked->GetCenterLVec(MyLinkLVec, MyLinked->ma_Pair.GetSize()-1);
		if(IsSameDir(UrLine))
			if((UrLine->m_SharePntMask & NM_LINKED0) == 0)
				UrLinkResult = UrLinked->GetCenterLVec(UrLinkLVec, 0);
			else
				UrLinkResult = UrLinked->GetCenterLVec(UrLinkLVec, UrLinked->ma_Pair.GetSize()-1);
		else
			if((UrLine->m_SharePntMask & NM_LINKED1) == 0)
				UrLinkResult = UrLinked->GetCenterLVec(UrLinkLVec, 0);
			else
				UrLinkResult = UrLinked->GetCenterLVec(UrLinkLVec, UrLinked->ma_Pair.GetSize()-1);
	}
	else
	{
		MyResult = GetCenterLVec(MyLVec, ma_Pair.GetSize()-1);
		if((m_SharePntMask & NM_LINKED1) == 0)
			MyLinkResult = MyLinked->GetCenterLVec(MyLinkLVec, 0);
		else
			MyLinkResult = MyLinked->GetCenterLVec(MyLinkLVec, MyLinked->ma_Pair.GetSize()-1);
		if(IsSameDir(UrLine))
			if((UrLine->m_SharePntMask & NM_LINKED1) == 0)
				UrLinkResult = UrLinked->GetCenterLVec(UrLinkLVec, 0);
			else
				UrLinkResult = UrLinked->GetCenterLVec(UrLinkLVec, UrLinked->ma_Pair.GetSize()-1);
		else
			if((UrLine->m_SharePntMask & NM_LINKED0) == 0)
				UrLinkResult = UrLinked->GetCenterLVec(UrLinkLVec, 0);
			else
				UrLinkResult = UrLinked->GetCenterLVec(UrLinkLVec, UrLinked->ma_Pair.GetSize()-1);
	}

	if(MyResult==FALSE || MyLinkResult==FALSE || UrLinkResult==FALSE) // 교점은 못 구했지만 Y 절점임은 확인했으므로
		return -1;

	GM2DVector Cross1, Cross2, Cross3;
	long Result;
	double MyLoc, UrLoc;
	Result = MyLVec.Intersect(MyLinkLVec, Cross1, &MyLoc, &UrLoc);
	if(Result!=0 && Result!=-5)
		return -1;
	Result = MyLinkLVec.Intersect(UrLinkLVec, Cross2, &MyLoc, &UrLoc);
	if(Result!=0 && Result!=-5)
		return -1;
	Result = MyLVec.Intersect(UrLinkLVec, Cross3, &MyLoc, &UrLoc);
	if(Result!=0 && Result!=-5)
		return -1;

	CrossPnt = (Cross1 + Cross2 + Cross3) * (1./3.);
	return TRUE;
}

BOOL DG2DLine::GetCrossPointWithLink(long StartOrEnd, GM2DVector& CrossPnt)
{
	if(ma_Pair.GetSize() == 0)
		return FALSE;

	GM2DLineVector MyLVec, MyLinkLVec;
	BOOL MyResult, MyLinkResult;

	if(StartOrEnd == 0)
	{
		MyResult = GetCenterLVec(MyLVec, 0);
		if((m_SharePntMask & NM_LINKED0) == 0)
			MyLinkResult = m_Linked0->GetCenterLVec(MyLinkLVec, 0);
		else
			MyLinkResult = m_Linked0->GetCenterLVec(MyLinkLVec, m_Linked0->ma_Pair.GetSize()-1);
	}
	else
	{
		MyResult = GetCenterLVec(MyLVec, ma_Pair.GetSize()-1);
		if((m_SharePntMask & NM_LINKED1) == 0)
			MyLinkResult = m_Linked1->GetCenterLVec(MyLinkLVec, 0);
		else
			MyLinkResult = m_Linked1->GetCenterLVec(MyLinkLVec, m_Linked1->ma_Pair.GetSize()-1);
	}

	if(MyResult==FALSE || MyLinkResult==FALSE)
		return FALSE;

	double MyLoc, UrLoc;
	MyResult = MyLVec.Intersect(MyLinkLVec, CrossPnt, &MyLoc, &UrLoc);
	if(MyResult!=0 && MyResult!=-5)
		return TRUE;
	return FALSE;
}

BOOL DG2DLine::GetLinkLine(long StartOrEnd, GM2DLineVector& LinkLVec)
{
	if(StartOrEnd==0 && m_Linked0==NULL)
		return FALSE;
	if(StartOrEnd==1 && m_Linked1==NULL)
		return FALSE;

	GM2DGeometry* pLink = StartOrEnd==0 ? m_Linked0->m_pLine : m_Linked1->m_pLine;
	if(pLink->m_ObjType != GT_LINE)
		return FALSE;

	LinkLVec = *((GM2DLineVector*)pLink);
	if( (StartOrEnd==0 && (m_SharePntMask & NM_LINKED0)) ||
		(StartOrEnd==1 && (m_SharePntMask & NM_LINKED1)) )
		LinkLVec.ReverseDirection();

	return TRUE;
}

BOOL DG2DLine::GetGappedLine(long StartOrEnd, GM2DLineVector& LinkLVec)
{
	if(StartOrEnd==0 && m_Gapped0==NULL)
		return FALSE;
	if(StartOrEnd==1 && m_Gapped1==NULL)
		return FALSE;

	GM2DGeometry* pLink = StartOrEnd==0 ? m_Gapped0->m_pLine : m_Gapped1->m_pLine;
	if(pLink->m_ObjType != GT_LINE)
		return FALSE;

	LinkLVec = *((GM2DLineVector*)pLink);
	if( (StartOrEnd==0 && (m_SharePntMask & NM_GAPPED0)) ||
		(StartOrEnd==1 && (m_SharePntMask & NM_GAPPED1)) )
		LinkLVec.ReverseDirection();

	return TRUE;
}

BOOL DG2DLine::GetGapLVec(long StartOrEnd, GM2DLineVector& LinkLVec)
{
	if(StartOrEnd==0 && m_Gapped0==NULL)
		return FALSE;
	if(StartOrEnd==1 && m_Gapped1==NULL)
		return FALSE;

	GM2DGeometry* pLink = StartOrEnd==0 ? m_Gapped0->m_pLine : m_Gapped1->m_pLine;
	if(pLink->m_ObjType != GT_LINE)
		return FALSE;

	LinkLVec = *((GM2DLineVector*)pLink);

	if( (StartOrEnd==0 && (m_SharePntMask & NM_GAPPED0)) ||
		(StartOrEnd==1 && (m_SharePntMask & NM_GAPPED1)) )
		LinkLVec.ReverseDirection();

	return TRUE;
}

GMArc* DG2DLine::GetArc()
{
	if(m_pLine->m_ObjType != GT_ARC)
		return NULL;
	return (GMArc*)m_pLine;
}

GM2DLineVector* DG2DLine::GetLVec()
{
	if(m_pLine->m_ObjType != GT_LINE)
		return NULL;
	return (GM2DLineVector*)m_pLine;
}

GM2DVector DG2DLine::PointAtParam(double LocValue)
{
	if(m_pLine->m_ObjType == GT_LINE)
		return m_P0 + (m_P1-m_P0) * LocValue;
	else
	{
		GMArc* pArc = GetArc();
		double Radian = pArc->m_Theta1 + pArc->GetBetweenAngle() * LocValue;
		GM2DVector LocVec;
		LocVec.m_X = pArc->m_Center.m_X + (pArc->m_A * cos(Radian));
		LocVec.m_Y = pArc->m_Center.m_Y + (pArc->m_A * sin(Radian));
		return LocVec;
	}
}

double DG2DLine::Distance(const GM2DVector& AVec, bool bSigned)
{
	if(m_pLine->m_ObjType == GT_LINE)
	{
		GM2DLineVector* pLine = GetLVec();
		return pLine->Distance(AVec, bSigned);
	}
	else // ARC
	{
		GMArc* pArc = GetArc();
		GMVector aVec3(AVec.m_X, AVec.m_Y);
		double Dist = pArc->m_A - pArc->m_Center.Distance(aVec3);
		if(!bSigned)
			return fabs(Dist);
		else
			return Dist;
	}
}

///////////////////////////////////////////////////////////////////////////////
// this벡터와  ALineVec벡터의 교차점 벡터 CrossVec와 절대 위치(0.~1.) MyLoc, YourLoc 계산
// return : -1 : Parallel but not overlap, therefore no intersection
//          -2 : colinear but not overlap, therefore no intersection
//          -3 : colinear and overlap 
//          -4 : notknown error
//          -5 : not Parallel and not intersection
//           0 : Intersect at 1 point
//           1 : colinear and intersect at 1 end point
///////////////////////////////////////////////////////////////////////////////

long DG2DLine::Intersect(DG2DLine* pUrLine, GM2DVector& CrossVec, double* MyLoc, double* UrLoc)
{
	if(m_pLine->m_ObjType == GT_LINE)
	{
		GM2DLineVector* pLVec = GetLVec();
		if (pLVec == nullptr) return -4;
		if(pUrLine->m_pLine->m_ObjType == GT_LINE)
		{
			GM2DLineVector* pUrLVec = pUrLine->GetLVec();
			if (pUrLVec == nullptr)	return -4;
			return pLVec->Intersect(*pUrLVec, CrossVec, MyLoc, UrLoc);
		}
		else
		{
			GMArc* pUrArc = pUrLine->GetArc();
			if (pUrArc == nullptr)	return -4;
			GM2DVector CrossVec2;
			return 10 + pLVec->IntersectArc(*pUrArc, CrossVec, CrossVec2);
		}
	}
	else // ARC
	{
		GMArc* pArc = GetArc();
		if (pArc == nullptr)	return -4;
		if(pUrLine->m_pLine->m_ObjType == GT_LINE)
		{
			GM2DLineVector* pUrLVec = pUrLine->GetLVec();
			if (pUrLVec == nullptr)	return -4;
			GM2DVector CrossVec2;
			return 20 + pUrLVec->Intersect(*pArc, CrossVec, CrossVec2);
		}
		else
		{
			GMArc* pUrArc = pUrLine->GetArc();
			if (pUrArc == nullptr)	return -4;
			GMVector CrossVec1, CrossVec2;
			return 30 + pArc->Intersect(*pUrArc, CrossVec1, CrossVec2);
		}
	}

	return 0;
}

double DG2DLine::OrthoBaseLocValue( const GM2DVector& AVec )
{
	if(m_pLine->m_ObjType == GT_LINE)
	{
		GM2DLineVector* pLine = GetLVec();
		if (pLine == nullptr)	return 1 / DTOL_GM;
		return pLine->OrthoBaseLocValue(AVec);
	}
	else // ARC
	{
		GMArc* pArc = GetArc();
		if (pArc == nullptr)	return 1 / DTOL_GM;
		GM2DVector MyDirVec(AVec.m_X-pArc->m_Center.m_X, AVec.m_Y-pArc->m_Center.m_Y);
		double MyRadian = MyDirVec.GetRadian() - pArc->m_Theta1;
		if(MyRadian < 0.)
			MyRadian += 2.*PI;

		return MyRadian / pArc->GetBetweenAngle();
	}
}

bool DG2DLine::SetP0(GM2DVector& NewP0)
{
	m_P0 = NewP0;
	if(m_pLine->m_ObjType==GT_LINE)
		GetLVec()->m_P0 = NewP0;
	else
	{
		GMArc* pArc = GetArc();
		GMVector SVec = NewP0;
		double DistFromCenter = pArc->m_Center.Distance(SVec);
		if(fabs(pArc->m_A-DistFromCenter)>DTOL1)
			return false;
		pArc->SetStartPoint(SVec);
	}
	return true;
}

bool DG2DLine::SetP1(GM2DVector& NewP1)
{
	m_P1 = NewP1;
	if(m_pLine->m_ObjType==GT_LINE)
		GetLVec()->m_P1 = NewP1;
	else
	{
		GMArc* pArc = GetArc();
		GMVector EVec = NewP1;
		double DistFromCenter = pArc->m_Center.Distance(EVec);
		if(fabs(pArc->m_A-DistFromCenter)>DTOL1)
			return false;
		pArc->SetEndPoint(EVec);
	}
	return true;
}

GM2DVector DG2DLine::DirVector()
{
	if(m_pLine->m_ObjType==GT_LINE)
		return m_P1-m_P0;
	else
	{
		GM2DVector aLocVec;
		ASSERT(0);
		return aLocVec;
	}
}

long DG2DLine::IsParallel4DTol(DG2DLine& UrLine, double dTol)
{
	if(m_pLine->m_ObjType == GT_LINE)
	{
		GM2DLineVector* pLine = GetLVec();
		return pLine->IsParallel4DTol(*UrLine.GetLVec(), dTol);
	}
	else // ARC
	{
		GMArc* pArc = GetArc();
		
		if(UrLine.m_pLine->m_ObjType != GT_ARC)
			return 0;
		GMArc* pUrArc = UrLine.GetArc();
		if(pArc->m_Center.Distance(pUrArc->m_Center) > dTol)
			return 0;

		return 1;
	}
}

long DG2DLine::IsColinear4DTol(DG2DLine& UrLine, double DegreeTol, double DistTol)
{
	if(m_pLine->m_ObjType == GT_LINE)
	{
		GM2DLineVector* pLine = GetLVec();
		return pLine->IsColinear4DTol(*UrLine.GetLVec(), DegreeTol, DistTol);
	}
	else // ARC
	{
		GMArc* pArc = GetArc();

		if(UrLine.m_pLine->m_ObjType != GT_ARC)
			return 0;
		GMArc* pUrArc = UrLine.GetArc();
		if(pArc->m_Center.Distance(pUrArc->m_Center) > DistTol)
			return 0;

		if(fabs(pArc->m_A - pUrArc->m_A) > DistTol)
			return 0;

		return 1;
	}
}

GM2DVector DG2DLine::OrthoBaseVector(const GM2DVector& AVec)
{
	GM2DVector BaseVec;
	double BaseLocValue = OrthoBaseLocValue(AVec);
	BaseVec = PointAtParam(BaseLocValue);

	return BaseVec;
}

long DG2DLine::WhichSide( GM2DVector& PntVec, double dTol)
{
	if(m_pLine->m_ObjType==GT_LINE)
	{
		GM2DLineVector* pLine = GetLVec();
		if (pLine == nullptr)	return 0;
		return pLine->WhichSide(PntVec, dTol);
	}
	else
	{
		GMArc* pArc = GetArc();
		if (pArc == nullptr)	return 0;
		GM2DVector CenVec(pArc->m_Center.m_X, pArc->m_Center.m_Y);
		double UrDist = PntVec.Distance(CenVec) - pArc->m_A;
	
		if(UrDist > dTol) return -1; // 원 외부
		else if(UrDist < -dTol) return 1; // 원 내부
		else return 0;  // 원 선상
	}
}

BOOL DG2DLine::OverlapTest4DTol( DG2DLine& UrLine, double dTol/*=1.0*/ )
{
	if(m_pLine->m_ObjType==GT_LINE)
	{
		if(UrLine.m_pLine->m_ObjType != GT_LINE)
			return FALSE;
		GM2DLineVector* pLine = GetLVec();
		return pLine->OverlapTest4DTol(*UrLine.GetLVec(), dTol);
	}
	else
	{
		GMArc* pArc = GetArc();

		if(UrLine.m_pLine->m_ObjType != GT_ARC)
			return FALSE;
		GMArc* pUrArc = UrLine.GetArc();

		long Result = pArc->Overlap4DTol(*pUrArc, 1., dTol) ;

		if(Result==0)
			return FALSE;
		switch(Result)
		{
		case 1:
		case 2:
		case 12:
			return FALSE;
		default:
			return TRUE;
		}
	}
}
