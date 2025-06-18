#include "StdAfx.h"
// #include "vdfigure.h"
// #include "vdraw.h"
// #include "vdole.h"
// #include "vdline.h"
// #include "vdselections.h"
// #include "vdselection.h"
// #include "vddocument.h"
// #include "vdcircle.h"
// #include "vdarc.h"
// #include "vdtext.h"

#include "CogReader.h"

#include "DG2DLine.h"
#include "DG2DArc.h"

#include "GMLib\GM2DVector.h"
#include "GMLib\GM2DLineVector.h"
#include "GMLib\GMVector.h"
#include "GMLib\GMText.h"
#include "GMLib\GMQuadTree.h"
#include "GMLib\GMLine.h"

#include <math.h>
#include <stdio.h>

long CogReader::CheckZCornerMember(DG2DLine* MyLine)
{
	long nPair = MyLine->ma_Pair.GetSize();
	if(nPair > 1)
		return 0;

	DG2DLine* GapLine;
	DG2DLine* MyLink2Gap, *MyLink2Corner;
	DG2DLine* GapLink;
	DG2DLine* GapPair, *GapPairLink;
	GM2DVector* GapLinkPnt;
	if(MyLine->m_Gapped0 && !MyLine->m_Gapped1)
	{
		GapLine = MyLine->m_Gapped0;
		long nGapLinePair = GapLine->ma_Pair.GetSize();
		if(nGapLinePair==0)
			return 0;
		if(MyLine->IsSameDir(GapLine))
		{
			GapLink = GapLine->m_Linked1;
			GapPair = GapLine->ma_Pair[nGapLinePair-1];
			GapLinkPnt = &(GapLine->m_P1);
		}
		else
		{
			GapLink = GapLine->m_Linked0;
			GapPair = GapLine->ma_Pair[0];
			GapLinkPnt = &(GapLine->m_P0);
		}
		MyLink2Gap = MyLine->m_Linked0;
		if(!GapLink || !MyLink2Gap || !GapLink->IsPair(MyLink2Gap))
			return 0;

		MyLink2Corner = MyLine->m_Linked1;
		GapPairLink = GapPair->IsSameDir(MyLine) ? GapPair->m_Linked1 : GapPair->m_Linked0;
		if(!MyLink2Corner || !GapPairLink || MyLink2Corner->IsPair(GapPairLink) || !MyLink2Corner->IsParallel4DTol(*GapPairLink))
			return 0;

		DG2DLine* PairZLine = GapPair->GetLinkNext(GapPairLink);
		DG2DLine* MyZLine = MyLine->GetLinkNext(MyLink2Corner);

		if(!PairZLine || !MyZLine || PairZLine==MyLine || !PairZLine->IsPair(MyZLine))
			return 0;

		GM2DVector* MyPnt = MyLine->EndOfGivenLink(MyLink2Corner);
		GM2DVector OrthoBase = PairZLine->OrthoBaseVector(*MyPnt);
		MyPnt->SetXY( (OrthoBase.m_X+MyPnt->m_X) / 2., (OrthoBase.m_Y+MyPnt->m_Y) / 2.);
		GM2DVector* GapPairEndPnt = PairZLine->EndOfGivenLink(GapPairLink);
		OrthoBase = GapLine->OrthoBaseVector(*GapPairEndPnt);
		GapPairEndPnt->SetXY( (OrthoBase.m_X+GapPairEndPnt->m_X) / 2., (OrthoBase.m_Y+GapPairEndPnt->m_Y) / 2.);

		GapLinkPnt->SetXY(MyLine->m_P1.m_X, MyLine->m_P1.m_Y);

		double MyLoc0 = MyLink2Corner->OrthoBaseLocValue(GapPairLink->m_P0);
		double MyLoc1 = MyLink2Corner->OrthoBaseLocValue(GapPairLink->m_P1);
		long Result = OverlapGroup(MyLoc0, MyLoc1, m_DTOL_DIST_LINKTEST/10000);

		MyLink2Corner->AddPairLine(GapPairLink, MyLoc0, MyLoc1, Result, m_MAX_GAP_LENGTH, m_DTOL_DIST_LINKTEST);
		ReleasePairs(MyLine, PairZLine);

		MyLink2Gap->ReleaseLinkLine(MyLine);
		GapLink->ReleaseLinkLine(GapLine);

		if(MyLine->IsSameDir(GapLine))
			GapLine->SetLinkLine(1, MyLink2Corner, m_DTOL_DIST_LINKTEST);
		else
			GapLine->SetLinkLine(0, MyLink2Corner, m_DTOL_DIST_LINKTEST);

		return 1;
	}
	else if(!MyLine->m_Gapped0 && MyLine->m_Gapped1)
	{
		GapLine = MyLine->m_Gapped1;
		long nGapLinePair = GapLine->ma_Pair.GetSize();
		if(nGapLinePair==0)
			return 0;
		if(MyLine->IsSameDir(GapLine))
		{
			GapLink = GapLine->m_Linked0;
			GapPair = GapLine->ma_Pair[0];
			GapLinkPnt = &(GapLine->m_P0);
		}
		else
		{
			GapLink = GapLine->m_Linked1;
			GapPair = GapLine->ma_Pair[nGapLinePair-1];
			GapLinkPnt = &(GapLine->m_P1);
		}
		MyLink2Gap = MyLine->m_Linked1;
		if(!GapLink || !MyLink2Gap || !GapLink->IsPair(MyLink2Gap))
			return 0;

		MyLink2Corner = MyLine->m_Linked0;
		GapPairLink = GapPair->IsSameDir(MyLine) ? GapPair->m_Linked1 : GapPair->m_Linked0;
		if(!MyLink2Corner || !GapPairLink || MyLink2Corner->IsPair(GapPairLink) || !MyLink2Corner->IsParallel4DTol(*GapPairLink))
			return 0;

		DG2DLine* PairZLine = GapPair->GetLinkNext(GapPairLink);
		DG2DLine* MyZLine = MyLine->GetLinkNext(MyLink2Corner);

		if(!PairZLine || !MyZLine || PairZLine->IsPair(MyLine) || !PairZLine->IsPair(MyZLine))
			return 0;

		GM2DVector* MyPnt = MyLine->EndOfGivenLink(MyLink2Corner);
		GM2DVector OrthoBase = PairZLine->OrthoBaseVector(*MyPnt);
		MyPnt->SetXY( (OrthoBase.m_X+MyPnt->m_X) / 2., (OrthoBase.m_Y+MyPnt->m_Y) / 2.);
		GM2DVector* GapPairEndPnt = PairZLine->EndOfGivenLink(GapPairLink);
		OrthoBase = GapLine->OrthoBaseVector(*GapPairEndPnt);
		GapPairEndPnt->SetXY( (OrthoBase.m_X+GapPairEndPnt->m_X) / 2., (OrthoBase.m_Y+GapPairEndPnt->m_Y) / 2.);

		GapLinkPnt->SetXY(MyLine->m_P0.m_X, MyLine->m_P0.m_Y);

		double MyLoc0 = MyLink2Corner->OrthoBaseLocValue(GapPairLink->m_P0);
		double MyLoc1 = MyLink2Corner->OrthoBaseLocValue(GapPairLink->m_P1);
		long Result = OverlapGroup(MyLoc0, MyLoc1, m_DTOL_DIST_LINKTEST/10000);

		MyLink2Corner->AddPairLine(GapPairLink, MyLoc0, MyLoc1, Result, m_MAX_GAP_LENGTH, m_DTOL_DIST_LINKTEST);
		ReleasePairs(MyLine, PairZLine);

// 		MyLink2Gap->ReleaseLinkLine(MyLine);
// 		GapLink->ReleaseLinkLine(GapLine);
// 
// 		if(MyLine->IsSameDir(GapLine))
// 			GapLine->SetLinkLine(0, MyLink2Corner, m_DTOL_DIST_LINKTEST);
// 		else
// 			GapLine->SetLinkLine(1, MyLink2Corner, m_DTOL_DIST_LINKTEST);

		return 1;
	}
	else
		return 0;
}

long CogReader::CheckCornerMember(DG2DLine* MyLine)
{
	long nPair = MyLine->ma_Pair.GetSize();
	if(nPair != 0)
		return 0;

	DG2DLine* GapLine;
	DG2DLine* MyLink2Gap, *MyLink2Corner;
	DG2DLine* GapLink;
	DG2DLine* GapPair, *GapPairLink;
	GM2DVector* GapLinkPnt;
	if(MyLine->m_Gapped0 && !MyLine->m_Gapped1)
	{
		GapLine = MyLine->m_Gapped0;
		long nGapLinePair = GapLine->ma_Pair.GetSize();
		if(nGapLinePair==0)
			return 0;
		if(MyLine->IsSameDir(GapLine))
		{
			GapLink = GapLine->m_Linked1;
			GapPair = GapLine->ma_Pair[nGapLinePair-1];
			GapLinkPnt = &(GapLine->m_P1);
		}
		else
		{
			GapLink = GapLine->m_Linked0;
			GapPair = GapLine->ma_Pair[0];
			GapLinkPnt = &(GapLine->m_P0);
		}
		MyLink2Gap = MyLine->m_Linked0;
		if(!GapLink || !MyLink2Gap || !GapLink->IsPair(MyLink2Gap))
			return 0;

		MyLink2Corner = MyLine->m_Linked1;
		GapPairLink = GapPair->IsSameDir(MyLine) ? GapPair->m_Linked1 : GapPair->m_Linked0;
		if(!MyLink2Corner || !GapPairLink || !MyLink2Corner->IsPair(GapPairLink))
			return 0;

		double CornerMemLength = max(MyLink2Corner->Length(), GapPairLink->Length());
		double GapMemLength = max(GapLink->Length(), GapPair->Length());

		if(CornerMemLength > GapMemLength)
			return 0;

		GM2DVector* MyPnt = &(MyLine->m_P0);
		GM2DVector MyExtendedPnt( (MyPnt->m_X+GapLinkPnt->m_X) / 2., (MyPnt->m_Y+GapLinkPnt->m_Y) / 2.);
// 		if(MyPnt->Distance(MyExtendedPnt) < m_DTOL_NODE)
// 			return 0;

		*MyPnt = MyExtendedPnt;
		GM2DVector* GapPairEndPnt = GapPair->IsSameDir(MyLine) ? &(GapPair->m_P1) : &(GapPair->m_P0);
		GM2DVector OrthoBase = MyLink2Corner->OrthoBaseVector(*GapPairEndPnt);
		GapPairEndPnt->SetXY( (OrthoBase.m_X+GapPairEndPnt->m_X) / 2., (OrthoBase.m_Y+GapPairEndPnt->m_Y) / 2.);

		double MyLoc0 = MyLine->OrthoBaseLocValue(GapPair->m_P0);
		double MyLoc1 = MyLine->OrthoBaseLocValue(GapPair->m_P1);
		long Result = OverlapGroup(MyLoc0, MyLoc1, m_DTOL_DIST_LINKTEST/10000);

		MyLine->AddPairLine(GapPair, MyLoc0, MyLoc1, Result, m_MAX_GAP_LENGTH, m_DTOL_DIST_LINKTEST);

		return 1;
	}
	else if(!MyLine->m_Gapped0 && MyLine->m_Gapped1)
	{
		GapLine = MyLine->m_Gapped1;
		long nGapLinePair = GapLine->ma_Pair.GetSize();
		if(nGapLinePair==0)
			return 0;
		if(MyLine->IsSameDir(GapLine))
		{
			GapLink = GapLine->m_Linked0;
			GapPair = GapLine->ma_Pair[0];
			GapLinkPnt = &(GapLine->m_P0);
		}
		else
		{
			GapLink = GapLine->m_Linked1;
			GapPair = GapLine->ma_Pair[nGapLinePair-1];
			GapLinkPnt = &(GapLine->m_P1);
		}
		MyLink2Gap = MyLine->m_Linked1;
		if(!GapLink || !MyLink2Gap || !GapLink->IsPair(MyLink2Gap))
			return 0;

		MyLink2Corner = MyLine->m_Linked0;
		GapPairLink = GapPair->IsSameDir(MyLine) ? GapPair->m_Linked0 : GapPair->m_Linked1;
		if(!MyLink2Corner || !GapPairLink || !MyLink2Corner->IsPair(GapPairLink))
			return 0;

		double CornerMemLength = max(MyLink2Corner->Length(), GapPairLink->Length());
		double GapMemLength = max(GapLink->Length(), GapPair->Length());

		if(CornerMemLength > GapMemLength)
			return 0;

		GM2DVector* MyPnt = &(MyLine->m_P1);
		GM2DVector MyExtendedPnt( (MyPnt->m_X+GapLinkPnt->m_X) / 2., (MyPnt->m_Y+GapLinkPnt->m_Y) / 2.);
// 		if(MyPnt->Distance(MyExtendedPnt) < m_DTOL_NODE)
// 			return 0;

		*MyPnt = MyExtendedPnt;

		GM2DVector* GapPairEndPnt = GapPair->IsSameDir(MyLine) ? &(GapPair->m_P0) : &(GapPair->m_P1);
		GM2DVector OrthoBase = MyLink2Corner->OrthoBaseVector(*GapPairEndPnt);
		GapPairEndPnt->SetXY( (OrthoBase.m_X+GapPairEndPnt->m_X) / 2., (OrthoBase.m_Y+GapPairEndPnt->m_Y) / 2.);

		double MyLoc0 = MyLine->OrthoBaseLocValue(GapPair->m_P0);
		double MyLoc1 = MyLine->OrthoBaseLocValue(GapPair->m_P1);
		long Result = OverlapGroup(MyLoc0, MyLoc1, m_DTOL_DIST_LINKTEST/10000);

//		MyLine->AddPairLine(GapPair, MyLoc0, MyLoc1, Result, m_MAX_GAP_LENGTH, m_DTOL_DIST_LINKTEST);

		return 1;
	}
	else
		return 0;


}

long CogReader::CheckCrossPair(DG2DLine* MyLine)
{
/*
	if(CheckCornerMember(MyLine))
		return 10;
	if(CheckZCornerMember(MyLine))
		return 11;
*/

	long nPair = MyLine->ma_Pair.GetSize();
	if(nPair == 0)
		return 0;

	DG2DLine* MyLink0 = MyLine->m_Linked0;
	DG2DLine* MyLink1 = MyLine->m_Linked1;

	GM2DVector* LinkEndPt0 = MyLine->EndOfGivenLink(MyLink0);
	GM2DVector* LinkEndPt1 = MyLine->EndOfGivenLink(MyLink1);

	DG2DLine* Link0Nxt; // MyLink0 타단의 링크라인
	DG2DLine* Link1Nxt; // MyLink1 타단의 링크라인

	Link0Nxt = MyLine->GetLinkNext(MyLink0); 
	Link1Nxt = MyLine->GetLinkNext(MyLink1);

// 	Link0Nxt = MyLine->GetLinkNext(0, TRUE, FALSE); 
// 	Link1Nxt = MyLine->GetLinkNext(1, TRUE, FALSE);

	if(!MyLink0 || !MyLink1 || !MyLink0->IsPair(MyLink1))
	{
		if(MyLink0 && MyLine->m_Gapped1)
		{
			MyLink1 = MyLine->GetGapEndLinkLine(1);
			if(!MyLink1 || !MyLink0->IsPair(MyLink1))
				return 0;
			else
			{
				LinkEndPt1 = MyLine->m_Gapped1->EndOfGivenLink(MyLink1);
				Link1Nxt = (MyLine->m_SharePntMask & NM_GAPPED1) == 0 ? 
					         MyLine->m_Gapped1->GetLinkNext(1, TRUE, FALSE) : 
				           MyLine->m_Gapped1->GetLinkNext(0, TRUE, FALSE);
			}
		}
		else if(MyLink1 && MyLine->m_Gapped0)
		{
			MyLink0 = MyLine->GetGapEndLinkLine(0);
			if(!MyLink0 || !MyLink1->IsPair(MyLink0))
				return 0;
			else
			{
				LinkEndPt0 = MyLine->m_Gapped0->EndOfGivenLink(MyLink0);
				Link0Nxt = (MyLine->m_SharePntMask & NM_GAPPED0) == 0 ? 
				  				 MyLine->m_Gapped0->GetLinkNext(1, TRUE, FALSE) : 
				           MyLine->m_Gapped0->GetLinkNext(0, TRUE, FALSE);
			}
		}
		else
			return 0;
	}
 
	DG2DLine* UrLine;
	DG2DLine* UrLink0;  // UrLine의 MyLinrpe 0번점 쪽 링크라인
	DG2DLine* UrLink1;  // UrLine의 MyLine 1번점 쪽 링크라인
	DG2DLine* UrLink0Nxt; // UrLink0 타단의 링크라인
	DG2DLine* UrLink1Nxt; // UrLink1 타단의 링크라인

	BOOL IsSameDir;  // MyLine과 UrLine의 방향이 동일하면 true
	
	if(nPair == 1)
	{
		UrLine = MyLine->ma_Pair[0];
		long nUrPair = UrLine->ma_Pair.GetSize();
		IsSameDir = MyLine->IsSameDir(UrLine);
		if(IsSameDir)
			UrLink0 = UrLine->m_Linked0, UrLink1 = UrLine->m_Linked1;
		else
			UrLink0 = UrLine->m_Linked1, UrLink1 = UrLine->m_Linked0;

// Box 
		if(nUrPair==1 && UrLink0 && MyLink0 == UrLink0 && UrLink1 && MyLink1 ==UrLink1)
		{
			if(MyLine->Length() >= MyLink0->Length())
				ReleasePairs(MyLink0, MyLink1);
			else
				ReleasePairs(MyLine, UrLine);
			return 1;
		}
 
// 		GM2DVector* LinkEndPt0 = MyLine->EndOfGivenLink(MyLink0);
//  		GM2DVector* LinkEndPt1 = MyLine->EndOfGivenLink(MyLink1);
		long nMyLink0Side = MyLine->WhichSide(*LinkEndPt0);
		long nMyLink1Side = MyLine->WhichSide(*LinkEndPt1);
		long nUrSide = MyLine->WhichSide(UrLine->m_P0);

// 		Link0Nxt = MyLine->GetLinkNext(0, TRUE, TRUE);
// 		Link1Nxt = MyLine->GetLinkNext(1, TRUE, TRUE);

// wing wall
		if(Link0Nxt && Link1Nxt && 
			 nMyLink0Side * nUrSide > 0 && nMyLink1Side*nUrSide > 0) 
		{
			if(Link0Nxt->IsPair(UrLine) && Link1Nxt->IsPair(UrLine))
			{
					ReleasePairs(MyLine, UrLine);
					return 5;
			}
			if(!IsSameDir && MyLink0->IsGapped(UrLink0) && MyLink1->IsGapped(UrLink1))
			{
				if(MyLine->Length() > MyLink0->Length())
				{
					ReleasePairs(MyLink0, MyLink1);
					return 5;
				}
				else
				{
					ReleasePairs(MyLine, UrLine);
					return 5;
				}
			}
			if(IsSameDir && MyLink0->IsGapped(UrLink1) && MyLink1->IsGapped(UrLink0))
			{
				if(MyLine->Length() > MyLink0->Length())
				{
					ReleasePairs(MyLink0, MyLink1);
					return 5;
				}
				else
				{
					ReleasePairs(MyLine, UrLine);
					return 5;
				}
			}
		}

// Neck Hole
		if(Link0Nxt && nMyLink0Side * nUrSide < 0)
		{
			if(Link0Nxt->IsPair(UrLine))
			{
				ReleasePairs(MyLink0, MyLink1);
				return 5;
			}
			if(UrLink0)
			{
				UrLink0Nxt = (UrLine->m_SharePntMask & NM_LINKED0) == 0 ? UrLink0->m_Linked1 : UrLink0->m_Linked0;
				if(UrLink0Nxt && Link0Nxt->IsPair(UrLink0Nxt))
				{
					ReleasePairs(MyLink0, MyLink1);
					return 5;
				}
			}
		}
		if(Link1Nxt && nMyLink0Side * nUrSide < 0)
		{
			if(Link1Nxt->IsPair(UrLine))
			{
				ReleasePairs(MyLink0, MyLink1);
				return 5;
			}
			if(UrLink1)
			{
				UrLink1Nxt = (UrLine->m_SharePntMask & NM_LINKED1) == 0 ? UrLink1->m_Linked1 : UrLink1->m_Linked0;
				if(UrLink1Nxt && Link1Nxt->IsPair(UrLink1Nxt))
				{
					ReleasePairs(MyLink0, MyLink1);
					return -5;
				}
			}
		}

// ] [ : 두개의 벽체 단부가 마주하고 있는 경우
    // nMyLink0Side * nUrSide < 0 : 마이링크와 유어라인이 마이라인 중심으로 양쪽
		if(UrLink0 && UrLink1 && UrLink0->IsPair(UrLink1) && nMyLink0Side * nUrSide < 0)
		{
			ReleasePairs(MyLine, UrLine);
			return 2;
		}

		if(!Link0Nxt || !Link1Nxt)
			return 0;

// L 단부
		if(UrLine == Link0Nxt && Link1Nxt->IsPair(UrLine))
		{
			ReleasePairs(MyLine, UrLine);
			return 3;
		}
		else if(UrLine==Link1Nxt && Link0Nxt->IsPair(UrLine))
		{
			ReleasePairs(MyLine, UrLine);
			return -3;
		}

// Z 단부 (편심 부분이 서로 오버랩되지 않는 경우)
		long bIsLinkNxtParallel = Link0Nxt->IsParallel4DTol(*Link1Nxt, 45.);
 		Link0Nxt = (MyLine->m_SharePntMask & NM_LINKED0) == 0 ? MyLink0->m_Linked1 : MyLink0->m_Linked0;
 		Link1Nxt = (MyLine->m_SharePntMask & NM_LINKED1) == 0 ? MyLink1->m_Linked1 : MyLink1->m_Linked0;

		GM2DVector* LnkNxEndPt0 = MyLink0->EndOfGivenLink(Link0Nxt);
		GM2DVector* LnkNxEndPt1 = MyLink1->EndOfGivenLink(Link1Nxt);
		if(!LnkNxEndPt0 || !LnkNxEndPt1)
			return 0;
		if(bIsLinkNxtParallel)
		{
			DG2DLine* Link0NxtNxt = MyLink0->GetLinkNext(Link0Nxt);
			DG2DLine* Link1NxtNxt = MyLink1->GetLinkNext(Link1Nxt);

			if(Link0NxtNxt && Link1NxtNxt && Link0NxtNxt->IsPair(Link1NxtNxt))
			{
				double Link0Length = MyLink0->Length();
				double Link1Length = MyLink1->Length();
				if(UrLine == Link0Nxt && Link0Length > Link1Length)
				{
					long Side0 = MyLink0->WhichSide(*LnkNxEndPt0);
					long Side1 = MyLink0->WhichSide(*LnkNxEndPt1);
					if(Side0*Side1 > 0)
					{
						ReleasePairs(MyLine, UrLine);
						return 4;
					}
				}
				else if(UrLine==Link1Nxt && Link1Length > Link0Length)
				{
					long Side0 = MyLink1->WhichSide(*LnkNxEndPt0);
					long Side1 = MyLink1->WhichSide(*LnkNxEndPt1);
					if(Side0*Side1 > 0)
					{
						ReleasePairs(MyLine, UrLine);
						return -4;
					}
				}
			}
		}
	}
	else if(nPair==2)
	{
// Cross T
		DG2DLine* UrLine0 = MyLine->ma_Pair[0];
		DG2DLine* UrLine1 = MyLine->ma_Pair[1];

		if(Link0Nxt && Link1Nxt && Link0Nxt==UrLine0 && Link1Nxt==UrLine1)
		{
			DG2DLine* Link0NxtNxt = MyLink0->GetLinkNext(Link0Nxt);
			DG2DLine* Link1NxtNxt = MyLink1->GetLinkNext(Link1Nxt);
			if(UrLine0->IsGapped(UrLine1) && Link0NxtNxt != nullptr && Link0NxtNxt->IsPair(Link1NxtNxt))
			{
				if(MyLine->Length() >= MyLink1->Length())
				{
					ReleasePairs(MyLink0, MyLink1);
					return 21;
				}
				else
				{
					ReleasePairs(MyLine, UrLine0);
					ReleasePairs(MyLine, UrLine1);
					return 22;
				}
			}
		}
	}
	return 0;
}

long CogReader::CheckCrossPair2(DG2DLine* MyLine)
{
	long nPair = MyLine->ma_Pair.GetSize();
	if(nPair != 1)
		return 0;

	DG2DLine* MyLink0 = MyLine->m_Linked0;
	DG2DLine* MyLink1 = MyLine->m_Linked1;

	if(!MyLink0 && !MyLink1)
		return 0;

	long nLink0Pair = MyLink0 ? MyLink0->ma_Pair.GetSize() : 0;
	long nLink1Pair = MyLink1 ? MyLink1->ma_Pair.GetSize() : 0;

	if(nLink0Pair==0 && nLink1Pair==0)
		return 0;

	GM2DVector SVec, EVec;
	DG2DLine* MyPairLine = MyLine->ma_Pair[0];
	if(MyPairLine->ma_Pair.GetSize() == 0)
		return 0;
	BOOL bPairSameDir = MyLine->IsSameDir(MyPairLine);

	if(MyLine->ma_PairLoc0[0] < -DTOL_GM)
	{
		if(bPairSameDir && MyLine==MyPairLine->ma_Pair[0])
			SVec = MyLine->m_pLine->OrthoBaseVector(MyPairLine->m_P0);
		else if(!bPairSameDir && MyLine==MyPairLine->ma_Pair[MyPairLine->ma_Pair.GetSize()-1])
			SVec = MyLine->m_pLine->OrthoBaseVector(MyPairLine->m_P1);
	}
	else
		SVec = MyLine->m_P0;
	if(MyLine->ma_PairLoc1[0] > 1.+DTOL_GM)
	{
		if(bPairSameDir && MyLine==MyPairLine->ma_Pair[MyPairLine->ma_Pair.GetSize()-1])
			EVec = MyLine->m_pLine->OrthoBaseVector(MyPairLine->m_P1);
		else if(!bPairSameDir && MyLine==MyPairLine->ma_Pair[0])
			EVec = MyLine->m_pLine->OrthoBaseVector(MyPairLine->m_P0);
	}
	else
		EVec = MyLine->m_P1;

	GM2DLineVector MyEdge0(SVec, MyPairLine->m_pLine->OrthoBaseVector(SVec));
	GM2DLineVector MyEdge1(EVec, MyPairLine->m_pLine->OrthoBaseVector(EVec));
	GM2DLineVector MyLVec(SVec, EVec);
	GM2DLineVector MyPairLVec(MyEdge0.m_P1, MyEdge1.m_P1);

	if(nLink0Pair>0 && nLink1Pair>0)
	{
		long LinkPairIndex = (MyLine->m_SharePntMask & NM_LINKED0) == 0 ? 0 : nLink0Pair-1;
		DG2DLine* Link0PairLine = MyLink0->ma_Pair[LinkPairIndex];
		LinkPairIndex = (MyLine->m_SharePntMask & NM_LINKED1) == 0 ? 0 : nLink1Pair-1;
		DG2DLine* Link1PairLine = MyLink1->ma_Pair[LinkPairIndex];

		if(MyEdge0.OverlapTest4DTol(*(GM2DLineVector*)(MyLink0->m_pLine)) &&
		  (MyLink0->IsPair(MyLink1) || MyLink0->IsPair(Link1PairLine)))
		{
			ASSERT(0);
			ReleasePairs(MyLink0, Link1PairLine);
			ReleasePairs(MyLink0, MyLink1);
// 			ReleasePairs(MyLine, MyPairLine);
			return 1;
		}
		if(MyEdge1.OverlapTest4DTol(*(GM2DLineVector*)(MyLink1->m_pLine)) &&
			(MyLink0->IsPair(MyLink1) || MyLink1->IsPair(Link0PairLine)))
		{
			ASSERT(0);
			ReleasePairs(MyLink0, Link1PairLine);
			ReleasePairs(MyLink0, MyLink1);
			// 			ReleasePairs(MyLine, MyPairLine);
			return 1;
		}
	}
	return 0;
}

void CogReader::ReleasePairs(DG2DLine* MyLine, DG2DLine* UrLine)
{
	if(!MyLine || !UrLine)
		return;
	MyLine->RemovePairLineByLine(UrLine);
	UrLine->RemovePairLineByLine(MyLine);
}

BOOL CogReader::CheckNeckHole(DG2DLine* MyLine)
{
	if(MyLine->ma_Pair.GetSize()==0)
		return FALSE;

	DG2DLine* MyLink0 = MyLine->m_Linked0;
	DG2DLine* MyLink1 = MyLine->m_Linked1;

	if(!MyLink0 || !MyLink1 || !MyLink0->IsPair(MyLink1))
		return FALSE;

	DG2DLine* MyStag0 = (MyLine->m_SharePntMask & NM_LINKED0) == 0 ? MyLink0->m_Linked1 : MyLink0->m_Linked0;
	DG2DLine* MyStag1 = (MyLine->m_SharePntMask & NM_LINKED1) == 0 ? MyLink1->m_Linked1 : MyLink1->m_Linked0;

	if(!MyStag0 || !MyStag1)
		return FALSE;

	if(!MyStag0->IsParallel4DTol(*MyLine))
		return FALSE;
	if(!MyStag1->IsParallel4DTol(*MyLine))
		return FALSE;

	DG2DLine* UrLine = MyLine->ma_Pair[0];
	if(!UrLine)
		return FALSE;

	double MyDist = UrLine->Distance(MyLine->m_P0);
	double StagDist = UrLine->Distance(MyStag0->m_P0);
	if(StagDist < MyDist)
		return FALSE;
	StagDist = UrLine->Distance(MyStag1->m_P0);
	if(StagDist < MyDist)
		return FALSE;

	MyLink0->RemovePairLineByLine(MyLink1);
	MyLink1->RemovePairLineByLine(MyLink0);

	return TRUE;
}


BOOL CogReader::CheckBoxEnd(DG2DLine* MyLine)
{
	if(MyLine->ma_Pair.GetSize()!=1)
		return FALSE;

	DG2DLine* UrLine = MyLine->ma_Pair[0];
	BOOL IsSameDir = MyLine->IsSameDir(UrLine);

	DG2DLine* MyLink0 = MyLine->GetLinkLine(0);
	DG2DLine* MyLink1 = MyLine->GetLinkLine(1);

	if(!MyLink0 || !MyLink1 || !MyLink0->IsPair(MyLink1))
		return FALSE;

	if(IsSameDir)
	{
		if(MyLink0 != UrLine->GetLinkLine(0))
			return FALSE;
	  if(MyLink1 != UrLine->GetLinkLine(1))
		  return FALSE;
	}
	else
	{
		if(MyLink0 != UrLine->GetLinkLine(1))
			return FALSE;
		if(MyLink1 != UrLine->GetLinkLine(0))
			return FALSE;
	}

	MyLink0->RemovePairLine(0);
	MyLink1->RemovePairLine(0);

	return TRUE;
}

BOOL CogReader::CheckMyLEnd(DG2DLine* MyLine, BOOL IsAtStart)
{
	if(MyLine->ma_Pair.GetSize() == 0)
		return FALSE;

	DG2DLine* MyLink0, *MyLink1, *UrLink0, *UrLink1;
	DG2DLine* UrLine = IsAtStart ? MyLine->ma_Pair[0] : MyLine->ma_Pair[MyLine->ma_Pair.GetSize()-1];
	BOOL IsSameDir = MyLine->IsSameDir(UrLine);

	if(IsAtStart)
	{
		MyLink0 = MyLine->m_Linked0;
		MyLink1 = MyLine->m_Linked1;

		if(IsSameDir)
		{
			UrLink0 = UrLine->m_Linked0;
			UrLink1 = UrLine->m_Linked1;
		}
		else
		{
			UrLink0 = UrLine->m_Linked1;
			UrLink1 = UrLine->m_Linked0;
		}
	}
	else
	{
		MyLink0 = MyLine->m_Linked1;
		MyLink1 = MyLine->m_Linked0;

		if(IsSameDir)
		{
			UrLink0 = UrLine->m_Linked1;
			UrLink1 = UrLine->m_Linked0;
		}
		else
		{
			UrLink0 = UrLine->m_Linked0;
			UrLink1 = UrLine->m_Linked1;
		}
	}

	if(!MyLink0 || !UrLink0 || !UrLink1)
		return FALSE;

	if(!(MyLink0 == UrLink0 || MyLink0->IsGapped(UrLink0)))
		return FALSE;

	if(!UrLink0->IsPair(UrLink1))
		return FALSE;

	if(UrLink1 == MyLink1)
		return FALSE;

	UrLine->m_Tag = ES_L;

	MyLine->RemovePairLineByLine(UrLine);
	UrLine->RemovePairLineByLine(MyLine);

	return TRUE;
}

BOOL CogReader::CheckStaggeredBeam(DG2DLine* MyLine, BOOL IsAtStart)
{
	if(MyLine->ma_Pair.GetSize() == 0)
		return FALSE;

	DG2DLine* MyLink, *UrLink, *MyStag, *UrStag;

	DG2DLine* UrLine = IsAtStart ? MyLine->ma_Pair[0] : MyLine->ma_Pair[MyLine->ma_Pair.GetSize()-1];
	BOOL IsSameDir = MyLine->IsSameDir(UrLine);

	if(IsAtStart)
	{
		MyLink = MyLine->m_Linked0;
		if(!MyLink)
			return FALSE;
		//		MyLink1 = MyLine->m_Linked1;
		MyStag = (MyLine->m_SharePntMask & NM_LINKED0) == 0 ? MyLink->m_Linked1 : MyLink->m_Linked0;
		if(!MyStag)
			return FALSE;

		if(IsSameDir)
		{
			UrLink = UrLine->m_Linked0;
			if(!UrLink)
				return FALSE;
			UrStag = (UrLine->m_SharePntMask & NM_LINKED0) == 0 ? UrLink->m_Linked1 : UrLink->m_Linked0;
			if(!UrStag)
				return FALSE;
		}
		else
		{
			UrLink = UrLine->m_Linked1;
			if(!UrLink)
				return FALSE;
			UrStag = (UrLine->m_SharePntMask & NM_LINKED1) == 0 ? UrLink->m_Linked1 : UrLink->m_Linked0;
			if(!UrStag)
				return FALSE;
		}
	}
	else
	{
		MyLink = MyLine->m_Linked1;
		if(!MyLink)
			return FALSE;
		//		MyLink1 = MyLine->m_Linked0;
		MyStag = (MyLine->m_SharePntMask & NM_LINKED1) == 0 ? MyLink->m_Linked1 : MyLink->m_Linked0;
		if(!MyStag)
			return FALSE;

		if(IsSameDir)
		{
			UrLink = UrLine->m_Linked1;
			if(!UrLink)
				return FALSE;
			UrStag = (UrLine->m_SharePntMask & NM_LINKED1) == 0 ? UrLink->m_Linked1 : UrLink->m_Linked0;
			if(!UrStag)
				return FALSE;
		}
		else
		{
			UrLink = UrLine->m_Linked0;
			if(!UrLink)
				return FALSE;
			UrStag = (UrLine->m_SharePntMask & NM_LINKED0) == 0 ? UrLink->m_Linked1 : UrLink->m_Linked0;
			if(!UrStag)
				return FALSE;
		}
	}

	if(!MyStag->IsPair(UrStag))
		return FALSE;
	if(!(MyLink->IsParallel4DTol(*UrLink)))
		return FALSE;
	if(IsOverlap(MyLink, UrLink))
		return FALSE;
	if(MyLink->Length() > MyLine->Distance(UrLine->m_P0)+DTOL_GM )
		return FALSE;
	if(UrLink->Length() > MyLine->Distance(UrLine->m_P0)+DTOL_GM )
		return FALSE;

	if(MyLink->m_bSingleLine)
		MyLink->ma_Pair.RemoveAll();
	if(UrLink->m_bSingleLine)
		UrLink->ma_Pair.RemoveAll();

	return TRUE;
}

BOOL CogReader::CheckMyZEnd(DG2DLine* MyLine, BOOL IsAtStart)
{
 	long nPair = MyLine->ma_Pair.GetSize();
 	if(nPair >= 2)
	{
		DG2DLine* MyLink0, *UrLink0=NULL, *UrLink1, *MyStag0, *UrStag1;
		GM2DVector *pMyLineEnd, *pMyStag0End, *pUrLineEnd, *pUrStag1End;

		DG2DLine* UrLine = IsAtStart ? MyLine->ma_Pair[0] : MyLine->ma_Pair[MyLine->ma_Pair.GetSize()-1];
		BOOL IsSameDir = MyLine->IsSameDir(UrLine);

		if(IsAtStart)
		{
			MyLink0 = MyLine->m_Linked0;
			pMyLineEnd = &(MyLine->m_P0);
			if(!MyLink0)
				return FALSE;
			//		MyLink1 = MyLine->m_Linked1;
			if((MyLine->m_SharePntMask & NM_LINKED0) == 0)
			{
				MyStag0 = MyLink0->m_Linked1;
				pMyStag0End=&(MyLink0->m_P1);
			}
			else
			{
				MyStag0 = MyLink0->m_Linked0;
				pMyStag0End=&(MyLink0->m_P0);
			}

			if(!MyStag0)
				return FALSE;

			if(IsSameDir)
			{
				UrLink1 = UrLine->m_Linked1;
				pUrLineEnd = &(UrLine->m_P1);
				if(!UrLink1)
					return FALSE;

				if((UrLine->m_SharePntMask & NM_LINKED1) == 0)
				{
					UrStag1 = UrLink1->m_Linked1;
					pUrStag1End=&(UrLink1->m_P1);
				}
				else
				{
					UrStag1 = UrLink1->m_Linked0;
					pUrStag1End=&(UrLink1->m_P0);
				}

				if(!UrStag1)
					return FALSE;
			}
			else
			{
				UrLink1 = UrLine->m_Linked0;
				pUrLineEnd = &(UrLine->m_P0);
				if(!UrLink1)
					return FALSE;

				if((UrLine->m_SharePntMask & NM_LINKED0) == 0)
				{
					UrStag1 = UrLink1->m_Linked1;
					pUrStag1End=&(UrLink1->m_P1);
				}
				else
				{
					UrStag1 = UrLink1->m_Linked0;
					pUrStag1End=&(UrLink1->m_P0);
				}

				if(!UrStag1)
					return FALSE;
			}
		}
		else
		{
			MyLink0 = MyLine->m_Linked1;
			pMyLineEnd = &(MyLine->m_P1);
			if(!MyLink0)
				return FALSE;
			//		MyLink1 = MyLine->m_Linked0;

			if((MyLine->m_SharePntMask & NM_LINKED1) == 0)
			{
				MyStag0 = MyLink0->m_Linked1;
				pMyStag0End=&(MyLink0->m_P1);
			}
			else
			{
				MyStag0 = MyLink0->m_Linked0;
				pMyStag0End=&(MyLink0->m_P0);
			}

			if(!MyStag0)
				return FALSE;

			IsSameDir = MyLine->IsSameDir(UrLine);
			if(IsSameDir)
			{
				UrLink0 = UrLine->m_Linked1;
				UrLink1 = UrLine->m_Linked0;
				pUrLineEnd = &(UrLine->m_P0);
				if(!UrLink1)
					return FALSE;

				if((UrLine->m_SharePntMask & NM_LINKED0) == 0)
				{
					UrStag1 = UrLink1->m_Linked1;
					pUrStag1End=&(UrLink1->m_P1);
				}
				else
				{
					UrStag1 = UrLink1->m_Linked0;
					pUrStag1End=&(UrLink1->m_P0);
				}

				if(!UrStag1)
					return FALSE;
			}
			else
			{
				UrLink0 = UrLine->m_Linked0;
				UrLink1 = UrLine->m_Linked1;
				pUrLineEnd = &(UrLine->m_P1);
				if(!UrLink1)
					return FALSE;

				if((UrLine->m_SharePntMask & NM_LINKED1) == 0)
				{
					UrStag1 = UrLink1->m_Linked1;
					pUrStag1End=&(UrLink1->m_P1);
				}
				else
				{
					UrStag1 = UrLink1->m_Linked0;
					pUrStag1End=&(UrLink1->m_P0);
				}

				if(!UrStag1)
					return FALSE;
			}
		}

		if(!MyLine->IsPair(UrStag1))
			return FALSE;
		if(!MyStag0->IsPair(UrLine))
			return FALSE;
		// 	{
		// 		if(!UrGapLine || !MyStag0->IsPair(UrGapLine))
		// 			return FALSE;
		// 
		// 		UrLine = UrGapLine;
		// 		IsSameDir = MyLine->IsSameDir(UrGapLine);
		// 		if(IsAtStart)
		// 			UrLink0 = IsSameDir ? UrLine->m_Linked0 : UrLine->m_Linked1;
		// 		else
		// 			UrLink0 = IsSameDir ? UrLine->m_Linked1 : UrLine->m_Linked0;
		// 	}

		long UrLineSide = MyLine->WhichSide(*pUrLineEnd);
		long MyStag0Side = MyLine->WhichSide(*pMyStag0End);
		long UrLineSide2UrStag1 = UrStag1->WhichSide(*pUrLineEnd);
		if(UrLineSide*MyStag0Side <= 0 || UrLineSide*UrLineSide2UrStag1 <=0) // 한쪽으로 편심되지 않고 
			return FALSE;

		double MyThick = MyLine->Distance(*pUrStag1End);
		double UrThick = UrLine->Distance(*pMyStag0End);
		double OverlapLength = UrLink1->Distance(*pMyStag0End);
		double OverlapThick = MyLine->Distance(*pUrLineEnd);

		long ZCaseNum = 1;

		if(ZCaseNum == 1)
		{
			GM2DVector OrthoBase = UrLine->OrthoBaseVector(*pMyStag0End);
			pMyStag0End->SetXY( (OrthoBase.m_X+pMyStag0End->m_X) / 2., (OrthoBase.m_Y+pMyStag0End->m_Y) / 2.);
			OrthoBase = MyLine->OrthoBaseVector(*pUrStag1End);
			pUrStag1End->SetXY( (OrthoBase.m_X+pUrStag1End->m_X) / 2., (OrthoBase.m_Y+pUrStag1End->m_Y) / 2.);

			ReleasePairs(MyLine, UrLine);

			if(!MyLink0->IsPair(UrLink1))
			{
				double MyLoc0 = MyLink0->OrthoBaseLocValue(UrLink1->m_P0);
				double MyLoc1 = MyLink0->OrthoBaseLocValue(UrLink1->m_P1);
				long Result = OverlapGroup(MyLoc0, MyLoc1, m_DTOL_DIST_LINKTEST/10000);

				MyLink0->AddPairLine(UrLink1, MyLoc0, MyLoc1, Result, m_MAX_GAP_LENGTH, m_DTOL_DIST_LINKTEST);
			}
		}

		// 일단 편심벽이 짧아서 편심벽 단부가 페어로 인식되어 있으면 해제
		if(MyStag0->IsLink(UrLink0))
		{
			//	UrLink0->m_Tag = 4;
			if(UrLink1->IsPair(UrLink0))
			{
				UrLink1->RemovePairLineByLine(UrLink0);
				UrLink0->RemovePairLineByLine(UrLink1);
			}
			if(MyLink0->IsPair(UrLink0))
			{
				MyLink0->RemovePairLineByLine(UrLink0);
				UrLink0->RemovePairLineByLine(MyLink0);
			}
		}

		return TRUE;
	}
	else if(nPair==1)
	{
		DG2DLine* UrLine = MyLine->ma_Pair[0];
		DG2DLine *MyLink, *UrLink;
		DG2DLine *MyStag, *UrStag;
		GM2DVector* pUrEnd;
		BOOL IsSameDir = MyLine->IsSameDir(UrLine);

		if(IsAtStart)
		{
			MyLink = MyLine->m_Linked0;
			if(IsSameDir)
			{
				UrLink = UrLine->m_Linked0;
				if(UrLink)
					pUrEnd = (UrLine->m_SharePntMask & NM_LINKED0) == 0 ? &(UrLink->m_P0) : &(UrLink->m_P1);
			}
			else
			{
				UrLink = UrLine->m_Linked1;
				if(UrLink)
					pUrEnd = (UrLine->m_SharePntMask & NM_LINKED1) == 0 ? &(UrLink->m_P0) : &(UrLink->m_P1);
			}
		}
		else
		{
			MyLink = MyLine->m_Linked1;
			if(IsSameDir)
			{
				UrLink = UrLine->m_Linked1;
				if(UrLink)
					pUrEnd = (UrLine->m_SharePntMask & NM_LINKED1) == 0 ? &(UrLink->m_P0) : &(UrLink->m_P1);
			}
			else
			{
				UrLink = UrLine->m_Linked0;
				if(UrLink)
					pUrEnd = (UrLine->m_SharePntMask & NM_LINKED0) == 0 ? &(UrLink->m_P0) : &(UrLink->m_P1);
			}
		}

		if(!MyLink || !UrLink || MyLink->IsPair(UrLink) || !MyLink->IsParallel4DTol(*UrLink))			
			return FALSE;

// Z자에서 연결 부위가 벽체 최대폭 2배이상 떨어져 있으면 리턴
		double dLineDist = MyLink->Distance(UrLink->m_P1);
		if(dLineDist > m_MAX_WALL_THICK*2.)
			return FALSE;
// 평행하고 MIN_WALL_THICK 범위 안에 일직선상에 있는 경우 리턴
		if (dLineDist < m_MIN_WALL_THICK)
			return FALSE;

		MyStag = MyLine->GetLinkNext(MyLink);
		UrStag = UrLine->GetLinkNext(UrLink);

		if(!MyStag || !UrStag || !MyStag->IsPair(UrStag))
			return FALSE;

		double StagPLoc0 = MyLine->OrthoBaseLocValue(MyStag->m_P0);
		double StagPLoc1 = MyLine->OrthoBaseLocValue(MyStag->m_P1);

		// Z형이 아니고 ㄷ형태로 도는 경우에는 리턴
		if( !( (StagPLoc0 > 0.99 && StagPLoc1 > 0.99) || (StagPLoc0 < 0.01 && StagPLoc1 < 0.01) ) ) 
			return FALSE;

		long UrLineSide = MyLine->WhichSide(UrLine->m_P0);
		long MyStagSide = MyLine->WhichSide(MyStag->m_P0);
		long UrStagSide2UrLine = UrLine->WhichSide(UrStag->m_P0);
		if(!IsSameDir)
			UrStagSide2UrLine *= -1;
		if(UrLineSide*MyStagSide <= 0 || UrLineSide*UrStagSide2UrLine <=0) // 한쪽으로 편심되지 않고 
			return FALSE;

// 		double MyThick = MyLine->Distance(UrLine->m_P0);
// 		double UrThick = UrLine->Distance(*pMyStag0End);
// 		double OverlapLength = UrLink->Distance(*pMyStag0End);
// 		double OverlapThick = MyLine->Distance(*pUrLineEnd);

		GM2DVector* pMyLinkEnd = MyLine->EndOfGivenLink(MyLink);
		GM2DVector OrthoBase = UrStag->OrthoBaseVector(*pMyLinkEnd);
		pMyLinkEnd->SetXY( (OrthoBase.m_X+pMyLinkEnd->m_X) / 2., (OrthoBase.m_Y+pMyLinkEnd->m_Y) / 2.);
		OrthoBase = MyLine->OrthoBaseVector(*pUrEnd);
		pUrEnd->SetXY( (OrthoBase.m_X+pUrEnd->m_X) / 2., (OrthoBase.m_Y+pUrEnd->m_Y) / 2.);

		double MyLoc0 = MyLink->OrthoBaseLocValue(UrLink->m_P0);
		double MyLoc1 = MyLink->OrthoBaseLocValue(UrLink->m_P1);
		long Result = OverlapGroup(MyLoc0, MyLoc1, m_DTOL_DIST_LINKTEST/10000);

		MyLink->AddPairLine(UrLink, MyLoc0, MyLoc1, Result, m_MAX_GAP_LENGTH, m_DTOL_DIST_LINKTEST);

		// 일단 편심벽이 짧아서 편심벽 단부가 페어로 인식되어 있으면 해제
		DG2DLine* UrStagNext = UrLink->GetLinkNext(UrStag);
		if(UrStagNext && UrStagNext->IsPair(UrLink))
			ReleasePairs(UrStagNext, UrLink);

		return TRUE;
  }
	return FALSE;
}


/*
BOOL CogReader::CheckMyZEnd(DG2DLine* MyLine, BOOL IsAtStart)
{
 	long nPair = MyLine->ma_Pair.GetSize();
 	if(nPair < 2)
 		return FALSE;

	DG2DLine* MyLink0, *UrLink0=NULL, *UrLink1, *MyStag0, *UrStag1;
	GM2DVector *pMyLineEnd, *pMyStag0End, *pUrLineEnd, *pUrStag1End;

	DG2DLine* UrLine = IsAtStart ? MyLine->ma_Pair[0] : MyLine->ma_Pair[MyLine->ma_Pair.GetSize()-1];
	BOOL IsSameDir = MyLine->IsSameDir(UrLine);

	if(IsAtStart)
	{
		MyLink0 = MyLine->m_Linked0;
		pMyLineEnd = &(MyLine->m_P0);
		if(!MyLink0)
			return FALSE;
		//		MyLink1 = MyLine->m_Linked1;
		if((MyLine->m_SharePntMask & NM_LINKED0) == 0)
		{
			MyStag0 = MyLink0->m_Linked1;
			pMyStag0End=&(MyLink0->m_P1);
		}
		else
		{
			MyStag0 = MyLink0->m_Linked0;
			pMyStag0End=&(MyLink0->m_P0);
		}

		if(!MyStag0)
			return FALSE;

		if(IsSameDir)
		{
			UrLink1 = UrLine->m_Linked1;
			pUrLineEnd = &(UrLine->m_P1);
			if(!UrLink1)
				return FALSE;

			if((UrLine->m_SharePntMask & NM_LINKED1) == 0)
			{
				UrStag1 = UrLink1->m_Linked1;
				pUrStag1End=&(UrLink1->m_P1);
			}
			else
			{
				UrStag1 = UrLink1->m_Linked0;
				pUrStag1End=&(UrLink1->m_P0);
			}

			if(!UrStag1)
				return FALSE;
		}
		else
		{
			UrLink1 = UrLine->m_Linked0;
			pUrLineEnd = &(UrLine->m_P0);
			if(!UrLink1)
				return FALSE;

			if((UrLine->m_SharePntMask & NM_LINKED0) == 0)
			{
				UrStag1 = UrLink1->m_Linked1;
				pUrStag1End=&(UrLink1->m_P1);
			}
			else
			{
				UrStag1 = UrLink1->m_Linked0;
				pUrStag1End=&(UrLink1->m_P0);
			}

			if(!UrStag1)
				return FALSE;
		}
	}
	else
	{
		MyLink0 = MyLine->m_Linked1;
		pMyLineEnd = &(MyLine->m_P1);
		if(!MyLink0)
			return FALSE;
		//		MyLink1 = MyLine->m_Linked0;

		if((MyLine->m_SharePntMask & NM_LINKED1) == 0)
		{
			MyStag0 = MyLink0->m_Linked1;
			pMyStag0End=&(MyLink0->m_P1);
		}
		else
		{
			MyStag0 = MyLink0->m_Linked0;
			pMyStag0End=&(MyLink0->m_P0);
		}

		if(!MyStag0)
			return FALSE;

		IsSameDir = MyLine->IsSameDir(UrLine);
		if(IsSameDir)
		{
			UrLink0 = UrLine->m_Linked1;
			UrLink1 = UrLine->m_Linked0;
			pUrLineEnd = &(UrLine->m_P0);
			if(!UrLink1)
				return FALSE;

			if((UrLine->m_SharePntMask & NM_LINKED0) == 0)
			{
				UrStag1 = UrLink1->m_Linked1;
				pUrStag1End=&(UrLink1->m_P1);
			}
			else
			{
				UrStag1 = UrLink1->m_Linked0;
				pUrStag1End=&(UrLink1->m_P0);
			}

			if(!UrStag1)
				return FALSE;
		}
		else
		{
			UrLink0 = UrLine->m_Linked0;
			UrLink1 = UrLine->m_Linked1;
			pUrLineEnd = &(UrLine->m_P1);
			if(!UrLink1)
				return FALSE;

			if((UrLine->m_SharePntMask & NM_LINKED1) == 0)
			{
				UrStag1 = UrLink1->m_Linked1;
				pUrStag1End=&(UrLink1->m_P1);
			}
			else
			{
				UrStag1 = UrLink1->m_Linked0;
				pUrStag1End=&(UrLink1->m_P0);
			}

			if(!UrStag1)
				return FALSE;
		}
	}

	if(!MyLine->IsPair(UrStag1))
		return FALSE;
	if(!MyStag0->IsPair(UrLine))
		return FALSE;
// 	{
// 		if(!UrGapLine || !MyStag0->IsPair(UrGapLine))
// 			return FALSE;
// 
// 		UrLine = UrGapLine;
// 		IsSameDir = MyLine->IsSameDir(UrGapLine);
// 		if(IsAtStart)
// 			UrLink0 = IsSameDir ? UrLine->m_Linked0 : UrLine->m_Linked1;
// 		else
// 			UrLink0 = IsSameDir ? UrLine->m_Linked1 : UrLine->m_Linked0;
// 	}

	long UrLineSide = MyLine->WhichSide(*pUrLineEnd);
	long MyStag0Side = MyLine->WhichSide(*pMyStag0End);
	long UrLineSide2UrStag1 = UrStag1->WhichSide(*pUrLineEnd);
	if(UrLineSide*MyStag0Side <= 0 || UrLineSide*UrLineSide2UrStag1 <=0) // 한쪽으로 편심되지 않고 
		return FALSE;

	double MyThick = MyLine->Distance(*pUrStag1End);
	double UrThick = UrLine->Distance(*pMyStag0End);
	double OverlapLength = UrLink1->Distance(*pMyStag0End);
	double OverlapThick = MyLine->Distance(*pUrLineEnd);

	long ZCaseNum = 1;

	if(ZCaseNum == 1)
	{
		GM2DVector OrthoBase = UrLine->OrthoBaseVector(*pMyStag0End);
		pMyStag0End->SetXY( (OrthoBase.m_X+pMyStag0End->m_X) / 2., (OrthoBase.m_Y+pMyStag0End->m_Y) / 2.);
		OrthoBase = MyLine->OrthoBaseVector(*pUrStag1End);
		pUrStag1End->SetXY( (OrthoBase.m_X+pUrStag1End->m_X) / 2., (OrthoBase.m_Y+pUrStag1End->m_Y) / 2.);

		ReleasePairs(MyLine, UrLine);

		if(!MyLink0->IsPair(UrLink1))
		{
			double MyLoc0 = MyLink0->OrthoBaseLocValue(UrLink1->m_P0);
			double MyLoc1 = MyLink0->OrthoBaseLocValue(UrLink1->m_P1);
			long Result = OverlapGroup(MyLoc0, MyLoc1, m_DTOL_DIST_LINKTEST/10000);

			MyLink0->AddPairLine(UrLink1, MyLoc0, MyLoc1, Result, m_MAX_GAP_LENGTH, m_DTOL_DIST_LINKTEST);
		}
	}

// 일단 편심벽이 짧아서 편심벽 단부가 페어로 인식되어 있으면 해제
	if(MyStag0->IsLink(UrLink0))
	{
		//	UrLink0->m_Tag = 4;
		if(UrLink1->IsPair(UrLink0))
		{
			UrLink1->RemovePairLineByLine(UrLink0);
			UrLink0->RemovePairLineByLine(UrLink1);
		}
		if(MyLink0->IsPair(UrLink0))
		{
			MyLink0->RemovePairLineByLine(UrLink0);
			UrLink0->RemovePairLineByLine(MyLink0);
		}
	}

	return TRUE;
}

BOOL CogReader::CheckMyZEnd(DG2DLine* MyLine, BOOL IsAtStart)
{
	if(MyLine->ma_Pair.GetSize() == 0)
		return FALSE;

	DG2DLine* MyLink0, *UrLink0, *UrLink1, *MyStag0, *UrStag1;

	DG2DLine* UrLine = IsAtStart ? MyLine->ma_Pair[0] : MyLine->ma_Pair[MyLine->ma_Pair.GetSize()-1];
	BOOL IsSameDir = MyLine->IsSameDir(UrLine);

	if(IsAtStart)
	{
		MyLink0 = MyLine->m_Linked0;
		if(!MyLink0)
			return FALSE;
		//		MyLink1 = MyLine->m_Linked1;
		MyStag0 = (MyLine->m_SharePntMask & NM_LINKED0) == 0 ? MyLink0->m_Linked1 : MyLink0->m_Linked0;
		if(!MyStag0)
			return FALSE;

		if(IsSameDir)
		{
			UrLink0 = UrLine->m_Linked0;
			UrLink1 = UrLine->m_Linked1;
			if(!UrLink1)
				return FALSE;
			UrStag1 = (UrLine->m_SharePntMask & NM_LINKED1) == 0 ? UrLink1->m_Linked1 : UrLink1->m_Linked0;
			if(!UrStag1)
				return FALSE;
		}
		else
		{
			UrLink0 = UrLine->m_Linked1;
			UrLink1 = UrLine->m_Linked0;
			if(!UrLink1)
				return FALSE;
			UrStag1 = (UrLine->m_SharePntMask & NM_LINKED0) == 0 ? UrLink1->m_Linked1 : UrLink1->m_Linked0;
			if(!UrStag1)
				return FALSE;
		}
	}
	else
	{
		MyLink0 = MyLine->m_Linked1;
		if(!MyLink0)
			return FALSE;
		//		MyLink1 = MyLine->m_Linked0;
		MyStag0 = (MyLine->m_SharePntMask & NM_LINKED1) == 0 ? MyLink0->m_Linked1 : MyLink0->m_Linked0;
		if(!MyStag0)
			return FALSE;

		if(IsSameDir)
		{
			UrLink0 = UrLine->m_Linked1;
			UrLink1 = UrLine->m_Linked0;
			if(!UrLink1)
				return FALSE;
			UrStag1 = (UrLine->m_SharePntMask & NM_LINKED0) == 0 ? UrLink1->m_Linked1 : UrLink1->m_Linked0;
			if(!UrStag1)
				return FALSE;
		}
		else
		{
			UrLink0 = UrLine->m_Linked0;
			UrLink1 = UrLine->m_Linked1;
			if(!UrLink1)
				return FALSE;
			UrStag1 = (UrLine->m_SharePntMask & NM_LINKED1) == 0 ? UrLink1->m_Linked1 : UrLink1->m_Linked0;
			if(!UrStag1)
				return FALSE;
		}
	}

	if(!MyLine->IsPair(UrStag1))
		return FALSE;
	if(!MyStag0->IsPair(UrLine))
		return FALSE;

	MyLine->RemovePairLineByLine(UrLine);
	UrLine->RemovePairLineByLine(MyLine);

	if(!MyStag0->IsLink(UrLink0))
		return FALSE;
	if(!(MyLink0->IsParallel4DTol(*UrLink1->m_pLine) 
		&& MyLink0->Distance(UrLink1->m_P0) < 
		2.*MyLine->Distance(UrLine->m_P0) )   )
		return FALSE;

	//	UrLink0->m_Tag = 4;
	if(UrLink1->IsPair(UrLink0))
	{
		UrLink1->RemovePairLineByLine(UrLink0);
		UrLink0->RemovePairLineByLine(UrLink1);
	}
	if(MyLink0->IsPair(UrLink0))
	{
		MyLink0->RemovePairLineByLine(UrLink0);
		UrLink0->RemovePairLineByLine(MyLink0);
	}

	MyLine->RemovePairLineByLine(UrLine);
	UrLine->RemovePairLineByLine(MyLine);

	return TRUE;
}

*/

BOOL CogReader::CheckLongitudinalTEnd(DG2DLine* MyLine, BOOL IsAtStart)
{
	double dMinLength = m_MIN_WALL_LENGTH;
	if(m_nMemberType==DG_GRIDLINE)
		dMinLength = m_dGridLineMinLength;
	if(MyLine->ma_Pair.GetSize() == 0)
		return FALSE;

	DG2DLine* MyLink, *UrLink;

	DG2DLine* UrLine = IsAtStart ? MyLine->ma_Pair[0] : MyLine->ma_Pair[MyLine->ma_Pair.GetSize()-1];
	BOOL IsSameDir = MyLine->IsSameDir(UrLine);

	if(IsAtStart)
	{
		if(MyLine->m_Staggered0 == NULL)
			return FALSE;
		MyLink = MyLine->m_Linked0;
		if(IsSameDir)
		{
			if(UrLine->m_Staggered0==NULL)
				return FALSE;
			UrLink = UrLine->m_Linked0;
		}
		else
		{
			if(UrLine->m_Staggered1==NULL)
				return FALSE;
			UrLink = UrLine->m_Linked1;
		}
	}
	else
	{
		if(MyLine->m_Staggered1 == NULL)
			return FALSE;
		MyLink = MyLine->m_Linked1;
		if(IsSameDir)
		{
			if(UrLine->m_Staggered1==NULL)
				return FALSE;
			UrLink = UrLine->m_Linked1;
		}
		else
		{
			if(UrLine->m_Staggered0==NULL)
				return FALSE;
			UrLink = UrLine->m_Linked0;
		}
	}
	if(MyLink==NULL || UrLink==NULL)	return FALSE;
	if(MyLink->ma_Pair.GetSize() != 1 || UrLink->ma_Pair.GetSize() != 1 || MyLink->ma_Pair[0] != UrLink->ma_Pair[0])
		return FALSE;

	DG2DLine* EndLine = MyLink->ma_Pair[0];
	if(EndLine->Length() > dMinLength)
		return FALSE;

	if(!EndLine->m_Linked0 || !EndLine->m_Linked1 || !EndLine->m_Linked0->IsPair(EndLine->m_Linked1))
		return FALSE;
/*
	double MyThick = MyLine->Distance(UrLine->m_P0);
	double EndThick = MyThick + MyLink->Length() + UrLink->Length();

	if(MyLink->Length() > m_MIN_WALL_LENGTH || 
		UrLink->Length() > m_MIN_WALL_LENGTH)
		return FALSE;
*/
	MyLink->RemovePairLineByLine(EndLine);
	UrLink->RemovePairLineByLine(EndLine);
	EndLine->RemovePairLineByLine(MyLink);
	EndLine->RemovePairLineByLine(UrLink);

	return TRUE;
}
BOOL CogReader::CheckKType(DG2DLine* MyLine, long nPairNum)
{
	if(MyLine->ma_Pair.GetSize() <= nPairNum+1)
		return FALSE;

	DG2DLine* UrLine = MyLine->ma_Pair[nPairNum];
	DG2DLine* NxLine = MyLine->ma_Pair[nPairNum+1];

	DG2DLine* UrLink, *UrLinkPair;

	BOOL IsSameDir = MyLine->IsSameDir(UrLine);
	
	if(IsSameDir)
	{
		UrLink = UrLine->m_Linked1;
		if(!UrLink || UrLink->ma_Pair.GetSize()<1)
			return FALSE;
		UrLinkPair = (UrLine->m_SharePntMask & NM_LINKED1) == 0 ? UrLink->ma_Pair[0] : UrLink->ma_Pair[UrLink->ma_Pair.GetSize()-1];
	}
	else
	{
		UrLink = UrLine->m_Linked0;
		if(!UrLink || UrLink->ma_Pair.GetSize()<1)
			return FALSE;
		UrLinkPair = (UrLine->m_SharePntMask & NM_LINKED0) == 0 ? UrLink->ma_Pair[0] : UrLink->ma_Pair[UrLink->ma_Pair.GetSize()-1];
	}
	if(UrLinkPair==NULL)	return FALSE;

	DG2DLine* NxLink, *NxLinkPair;

	IsSameDir = MyLine->IsSameDir(NxLine);

	if(IsSameDir)
	{
		NxLink = NxLine->m_Linked0;
		if(!NxLink || NxLink->ma_Pair.GetSize()<1)
			return FALSE;
		NxLinkPair = (NxLine->m_SharePntMask & NM_LINKED0) == 0 ? NxLink->ma_Pair[0] : NxLink->ma_Pair[NxLink->ma_Pair.GetSize()-1];
	}
	else
	{
		NxLink = NxLine->m_Linked1;
		if(!NxLink || NxLink->ma_Pair.GetSize()<1)
			return FALSE;
		NxLinkPair = (NxLine->m_SharePntMask & NM_LINKED1) == 0 ? NxLink->ma_Pair[0] : NxLink->ma_Pair[NxLink->ma_Pair.GetSize()-1];
	}
	if(NxLinkPair==NULL)	return FALSE;

	if(!UrLinkPair->IsLink(NxLinkPair))
		return FALSE;

	long NumUrLinkPair = UrLink->ma_Pair.GetSize();
	if(NumUrLinkPair==0)	return FALSE;

//	UrLink, UrLinkPair, NxLink, NxLinkPair에 각 SNode, ENode의 Alt Node셋팅을 해줘서
//	나중에 노드를 찾지 말고, Alt Node로 Ecc만 구해서 처리한다.

	return TRUE;
}

BOOL CogReader::CheckWingWall(DG2DLine* MyLine)
{
	if(MyLine->ma_Pair.GetSize() == 0)
		return FALSE;

	DG2DLine* MyLink0 = MyLine->GetLinkLine(0);
	DG2DLine* MyLink1 = MyLine->GetLinkLine(1);

	CTypedPtrArray<CObArray, DG2DLine*> RemoveUrLineArr;
	long NumPair = MyLine->ma_Pair.GetSize();
	for(long iPair=0; iPair<NumPair; iPair++)
	{
		DG2DLine* UrLine = MyLine->ma_Pair[iPair];
		DG2DLine* UrLink0, *UrLink1;
		BOOL IsSameDir = MyLine->IsSameDir(UrLine);
		
		if(IsSameDir)
		{
			UrLink0 = UrLine->m_Linked0;
			UrLink1 = UrLine->m_Linked1;

			if(MyLink0 == UrLink0 && MyLink1 == UrLink1)
				continue;
		}
		else
		{
			UrLink0 = UrLine->m_Linked1;
			UrLink1 = UrLine->m_Linked0;

			if(MyLink0 == UrLink1 && MyLink1 == UrLink0)
				continue;
		}

		if(UrLink0==NULL || UrLink1==NULL)	continue;
		if(UrLink0->IsPair(UrLink1)==FALSE)	continue;

 		double UrDist = MyLine->Distance(UrLine->m_P0);
 		double LinkDist0 = MyLine->Distance(UrLink0->m_P0);
 		double LinkDist1 = MyLine->Distance(UrLink0->m_P1);
 		double LinkDist = LinkDist0 > LinkDist1 ? LinkDist0 : LinkDist1;
 		if(LinkDist >= UrDist)
 			continue;

		if(iPair>0)
		{
			if(!UrLink0->IsLink(MyLine->ma_Pair[iPair-1]))
				continue;
// 			if(LinkDist >= UrDist)
// 				continue;
 			double LinkDist = MyLine->Distance(MyLine->ma_Pair[iPair-1]->m_P0);
 			if(UrDist <= LinkDist)
 				continue;
		}
		if(iPair<NumPair-1)
		{
			if(!UrLink1->IsLink(MyLine->ma_Pair[iPair+1]))
				continue;
 			double LinkDist = MyLine->Distance(MyLine->ma_Pair[iPair+1]->m_P0);
 			if(UrDist <= LinkDist)
 				continue;
		}

		RemoveUrLineArr.Add(UrLine);
	}

	long NumRemoveUrLine = RemoveUrLineArr.GetSize();
	for(long iRemoveUrLine=0; iRemoveUrLine<NumRemoveUrLine; iRemoveUrLine++)
	{
		DG2DLine* UrLine = RemoveUrLineArr[iRemoveUrLine];
		MyLine->RemovePairLineByLine(UrLine);
		UrLine->RemovePairLineByLine(MyLine);
	}

	return TRUE;
}

BOOL CogReader::CheckCrossTEnd(DG2DLine* MyLine, BOOL IsAtStart)
{
	double dMinLength = m_MIN_WALL_LENGTH;
	if(m_nMemberType==DG_GRIDLINE)
		dMinLength = m_dGridLineMinLength;
	if(MyLine->ma_Pair.GetSize() == 0)
		return FALSE;

	DG2DLine* MyLink, *UrLink;

	DG2DLine* UrLine = IsAtStart ? MyLine->ma_Pair[0] : MyLine->ma_Pair[MyLine->ma_Pair.GetSize()-1];
	BOOL IsSameDir = MyLine->IsSameDir(UrLine);

	if(IsAtStart)
	{
		if(MyLine->m_Linked0 == NULL)
			return FALSE;
		MyLink = MyLine->m_Linked0;
		if(IsSameDir)
		{
			if(UrLine->m_Linked0==NULL)
				return FALSE;
			UrLink = UrLine->m_Linked0;
		}
		else
		{
			if(UrLine->m_Linked1==NULL)
				return FALSE;
			UrLink = UrLine->m_Linked1;
		}
	}
	else
	{
		if(MyLine->m_Linked1 == NULL)
			return FALSE;
		MyLink = MyLine->m_Linked1;
		if(IsSameDir)
		{
			if(UrLine->m_Linked1==NULL)
				return FALSE;
			UrLink = UrLine->m_Linked1;
		}
		else
		{
			if(UrLine->m_Linked0==NULL)
				return FALSE;
			UrLink = UrLine->m_Linked0;
		}
	}

	if(IsOverlap(MyLink, UrLink))
		return FALSE;

	if(MyLink->ma_Pair.GetSize() != 1 || UrLink->ma_Pair.GetSize() != 1 || MyLink->ma_Pair[0] != UrLink->ma_Pair[0])
		return FALSE;

	DG2DLine* EndCrossLine = MyLink->ma_Pair[0];
	DG2DLine* SideLine0 = EndCrossLine->m_Linked0;
	DG2DLine* SideLine1 = EndCrossLine->m_Linked1;
	if(!SideLine0 || !SideLine1 || !SideLine1->IsPair(SideLine0))
		return FALSE;

	double StepOfInCrossLine = MyLink->Distance(UrLink->m_P0);
	if(StepOfInCrossLine > dMinLength)
		return FALSE;

	long NumPair = SideLine0->ma_Pair.GetSize();
	for(long iPair=0; iPair<NumPair; iPair++)
		SideLine0->ma_Pair[iPair]->RemovePairLineByLine(SideLine0);
	SideLine0->ma_Pair.RemoveAll();

	NumPair = SideLine1->ma_Pair.GetSize();
	for(iPair=0; iPair<NumPair; iPair++)
		SideLine1->ma_Pair[iPair]->RemovePairLineByLine(SideLine1);
	SideLine1->ma_Pair.RemoveAll();

	return TRUE;
}

BOOL CogReader::IsCrossTEndLine(DG2DLine* MyLine)
{
	if(MyLine->ma_Pair.GetSize() != 2)
		return FALSE;

	DG2DLine* StemLine0, *StemLine1;
	DG2DLine* UrLine0 = MyLine->ma_Pair[0];
	DG2DLine* UrLine1 = MyLine->ma_Pair[1];

	if(MyLine->m_Linked0 == NULL)
		return FALSE;
	if(MyLine->m_Linked1 == NULL)
		return FALSE;
	if(!MyLine->m_Linked0->IsPair(MyLine->m_Linked1))
		return FALSE;

	if(MyLine->IsSameDir(UrLine0))
	{
		if(MyLine->m_Linked0 != UrLine0->m_Linked0)
			return FALSE;
		if(!UrLine0->m_Linked1)
			return FALSE;
		StemLine0 = UrLine0->m_Linked1;
	}
	else
	{
		if(MyLine->m_Linked0 != UrLine0->m_Linked1)
			return FALSE;
		if(!UrLine0->m_Linked0)
			return FALSE;
		StemLine0 = UrLine0->m_Linked0;
	}

	if(MyLine->IsSameDir(UrLine1))
	{
		if(MyLine->m_Linked1 != UrLine1->m_Linked1)
			return FALSE;
		if(!UrLine1->m_Linked0)
			return FALSE;
		StemLine1 = UrLine1->m_Linked0;
	}
	else
	{
		if(MyLine->m_Linked1 != UrLine1->m_Linked0)
			return FALSE;
		if(!UrLine1->m_Linked1)
			return FALSE;
		StemLine1 = UrLine1->m_Linked1;
	}

	if(!StemLine0->IsPair(StemLine1))
		return FALSE;

	StemLine0->RemovePairLineByLine(MyLine->m_Linked1);
	StemLine1->RemovePairLineByLine(MyLine->m_Linked0);
	MyLine->m_Linked0->ma_Pair.RemoveAll();
	MyLine->m_Linked1->ma_Pair.RemoveAll();
/*
	if(StemLine0->IsPair(MyLine->m_Linked1))
	{
		StemLine0->RemovePairLineByLine(MyLine->m_Linked1);
		MyLine->m_Linked1->RemovePairLineByLine(StemLine0);
	}
	if(StemLine1->IsPair(MyLine->m_Linked0))
	{
		StemLine1->RemovePairLineByLine(MyLine->m_Linked0);
		MyLine->m_Linked0->RemovePairLineByLine(StemLine1);
	}
*/
	return TRUE;
}
