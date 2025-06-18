#include "stdafx.h"

#include "GMLib/GMObject.h"
#include "GMLib/GM2DLineVector.h"
#include "GMLib/GMQuadTree.h"
#include "GMLib/GMText.h"
#include "GMLib/GM2DArc.h"

#include "GMLib/XErrorObject.h"
#include "GMLib/XErrorContainer.h"


#include "CogReader.h"
#include "DG2DLine.h"
#include "DG2DArc.h"

#include "StructureModel\MSRebarBuilding.h"
#include "StructureModel/MSWallMemberG.h"
#include "StructureModel/MSFaceMemberM.h"

#include "Radfcore_CLI/AppGlobalOptionWrapper.h"

#include <math.h>

void CogReader::MergeGMLineSegments(long LayerNum)
{
	CTypedPtrArray<CObArray, GM2DGeometry*>& GMLines = m_pQT->ma_GMs[LayerNum];

	long NumObj = GMLines.GetSize();
	for(long ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		GM2DGeometry* AObj = GMLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		if(AObj->m_ObjType != GT_LINE) continue;

		GM2DLineVector* ALineObj = (GM2DLineVector*)AObj;

		if(ALineObj->Length() < 1.)
		{
			ALineObj->m_Tag = -1;
			continue;
		}

		ALineObj->m_Tag = 1;
		FindColinearGMLine(LayerNum, ALineObj);
	}

	m_pQT->RemoveGMsByTag(LayerNum, -1);
}

void CogReader::FindColinearGMLine(long LayerNum, GM2DLineVector* AGmLine)
{
	CMap<GM2DLineVector*, GM2DLineVector*, GM2DLineVector*, GM2DLineVector*> ChkLVecMap;

	vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(AGmLine);
	long NumQT = CrossQTs.size();
	BOOL RtnVal = FALSE;
	for(long QTNum=0; QTNum<NumQT; QTNum++)
	{
		GMQuadTree* pQT = CrossQTs[QTNum];
		if(pQT->m_Rect.Contains(&AGmLine->m_P0) || pQT->m_Rect.Contains(&AGmLine->m_P1))
		{
			CTypedPtrArray<CObArray, GM2DGeometry*>& GMLines = pQT->ma_GMs[LayerNum];
			long NumObj = GMLines.GetSize();
			for(long ObjNum=0; ObjNum<NumObj; ObjNum++)
			{
				GM2DGeometry* AObj = GMLines[ObjNum];
				if(AObj == NULL || AObj->m_Tag != 0)  continue;
				if(AObj->m_ObjType != GT_LINE) continue;
				GM2DLineVector* ALineObj = (GM2DLineVector*)AObj;
				if(ALineObj->Length() < 1.)
				{
					ALineObj->m_Tag = -1;
					continue;
				}

				GM2DLineVector* rUrLine=NULL;
				if(ChkLVecMap.Lookup(ALineObj, rUrLine))	continue;
				ChkLVecMap.SetAt(ALineObj, ALineObj);

				double dDistTol = 1.0;
				if(AGmLine->MergeWhenColinear(ALineObj, m_DTOL_DEGREE_COLINEAR, dDistTol, m_DTOL_DIST_COLINEAR)==TRUE)
				{
					if(ALineObj->m_Tag != -1)
						ALineObj->m_Tag = 1;
					FindColinearGMLine(LayerNum, AGmLine);
					return;
				}
			}
		}
	}
}

void CogReader::MergeLineSegments(CTypedPtrArray<CObArray, DG2DLine*>& DGLines, long LayerNum)
{
	CTypedPtrArray<CObArray, GM2DGeometry*>& GMLines = m_pQT->ma_GMs[LayerNum];

	long NumObj = GMLines.GetSize();
	for(long ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		GM2DGeometry* AObj = GMLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		if(AObj->m_ObjType != GT_LINE) continue;

		GM2DLineVector* ALineObj = (GM2DLineVector*)AObj;

		if(ALineObj->Length() < 1.)
		{
			ALineObj->m_Tag = -1;
			continue;
		}

		ALineObj->m_Tag = 1;

		DG2DLine* ALine = new DG2DLine(ALineObj, m_pQT);		
		long FigID = 0;
		//CHECKES
		//createGM 시 작성 안한 듯. 검토 요망
		m_GMFigureMap.Lookup(ALineObj, FigID);	
		if(FigID != 0)
			AddGeometryToFigureIDs(ALine, FigID);
		ALine->ma_GMs.Add(ALineObj);

		DGLines.Add(ALine);
		FindColinearLine(ALine, LayerNum);
	}

	m_pQT->RemoveGMsByTag(LayerNum, -1);
}

void CogReader::FindColinearLine(DG2DLine* ADGLine, long LayerNum)
{
	if(m_bMergedLineSegment)
		FindColinearLineOption1(ADGLine, LayerNum);
	else
		FindColinearLineOption2(ADGLine, LayerNum);
}

void CogReader::FindColinearLineOption1(DG2DLine* ADGLine, long LayerNum)
{
	CMap<GM2DLineVector*, GM2DLineVector*, GM2DLineVector*, GM2DLineVector*> ChkLVecMap;

	double distTolerance = 1.0;
	vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(ADGLine);
	long NumQT = CrossQTs.size();
	for(long QTNum=0; QTNum<NumQT; QTNum++)
	{
		GMQuadTree* pQT = CrossQTs[QTNum];
		if(pQT->m_Rect.Contains(&ADGLine->m_P0) || pQT->m_Rect.Contains(&ADGLine->m_P1))
		{
			CTypedPtrArray<CObArray, GM2DGeometry*>& GMLines = pQT->ma_GMs[LayerNum];
			long NumObj = GMLines.GetSize();
			for(long ObjNum=0; ObjNum<NumObj; ObjNum++)
			{
				GM2DGeometry* AObj = GMLines[ObjNum];
				if(AObj == NULL || AObj->m_Tag != 0)  continue;
				if(AObj->m_ObjType != GT_LINE) continue;
				GM2DLineVector* ALineObj = (GM2DLineVector*)AObj;
				if(ALineObj->Length() < 1.)
				{
					ALineObj->m_Tag = -1;
					continue;
				}

				GM2DLineVector* rUrLine=NULL;
				if(ChkLVecMap.Lookup(ALineObj, rUrLine))	continue;
				ChkLVecMap.SetAt(ALineObj, ALineObj);

				if(ADGLine->MergeWhenColinear1(ALineObj, m_DTOL_DEGREE_COLINEAR, distTolerance, m_DTOL_DIST_COLINEAR)==TRUE)
				{
					if(ALineObj->m_Tag != -1)
						ALineObj->m_Tag = 1;
					long FigID;
					m_GMFigureMap.Lookup(ALineObj, FigID);	
					if(FigID != 0)
						AddGeometryToFigureIDs(ALineObj, FigID);
					FindColinearLine(ADGLine, LayerNum);
					///////
					return;
				}
			}
		}
	}
}

void CogReader::FindColinearLineOption2(DG2DLine* ADGLine, long LayerNum)
{
	CMap<GM2DLineVector*, GM2DLineVector*, GM2DLineVector*, GM2DLineVector*> ChkLVecMap;

	vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(ADGLine);
	long NumQT = CrossQTs.size();
	double distTolerance = 1.0;
	for(long QTNum=0; QTNum<NumQT; QTNum++)
	{
		GMQuadTree* pQT = CrossQTs[QTNum];
		if(pQT->m_Rect.Contains(&ADGLine->m_P0) || pQT->m_Rect.Contains(&ADGLine->m_P1))
		{
			CTypedPtrArray<CObArray, GM2DGeometry*>& GMLines = pQT->ma_GMs[LayerNum];
			long NumObj = GMLines.GetSize();
			for(long ObjNum=0; ObjNum<NumObj; ObjNum++)
			{
				GM2DGeometry* AObj = GMLines[ObjNum];
				if(AObj == NULL || AObj->m_Tag != 0)  continue;
				if(AObj->m_ObjType != GT_LINE) continue;
				GM2DLineVector* ALineObj = (GM2DLineVector*)AObj;

				GM2DLineVector* rUrLine=NULL;
				if(ChkLVecMap.Lookup(ALineObj, rUrLine))	continue;
				ChkLVecMap.SetAt(ALineObj, ALineObj);

				long RtnVal = ADGLine->MergeWhenColinear2(ALineObj, m_DTOL_DEGREE_COLINEAR, distTolerance, m_DTOL_DIST_COLINEAR);
				int a = 3;
			}
		}
	}
}


void CogReader::MergeArcSegments(CTypedPtrArray<CObArray, DG2DArc*>& DGArcs, long LayerNum)
{
	CTypedPtrArray<CObArray, GM2DGeometry*>& GMLines = m_pQT->ma_GMs[LayerNum];

	long NumObj = GMLines.GetSize();

	for(long ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		GM2DGeometry* AObj = GMLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		if(AObj->m_ObjType != GT_ARC) continue;

		GM2DArc* AArcObj = (GM2DArc*)AObj;
		AArcObj->m_Tag = 1;

		DG2DArc* AArc = new DG2DArc(m_pQT);
		AArc->SetArc(AArcObj);

		long FigID = 0;
		m_GMFigureMap.Lookup(AArcObj, FigID);	
		if(FigID != 0)
			AddGeometryToFigureIDs(AArc, FigID);
		AArc->ma_GMs.Add(AArcObj);

		DGArcs.Add(AArc);
		//		AArc->AddGMArc(AArcObj);

		FindColinearArc(AArc, LayerNum);
	}

	m_pQT->RemoveGMsByTag(LayerNum, -1);
}

void CogReader::FindColinearArc(DG2DArc* ADGArc, long LayerNum)
{
	CTypedPtrArray<CObArray, GM2DGeometry*>& GMArcs = m_pQT->ma_GMs[LayerNum];
	long NumObj = GMArcs.GetSize();
	for(long iObj=0; iObj<NumObj; iObj++)
	{
		GM2DGeometry* AObj = GMArcs[iObj];
		if(AObj==NULL || AObj->m_Tag != 0)	continue;
		if(AObj->m_ObjType != GT_ARC)	continue;

		GM2DArc* AArcObj = (GM2DArc*)AObj;
		if(ADGArc->MergeWhenSameArc(AArcObj, m_DTOL_DEGREE_COLINEAR, m_DTOL_DIST_COLINEAR)==TRUE)
		{
			AArcObj->m_Tag = 1;
			FindColinearArc(ADGArc, LayerNum);
		}
	}
}


void CogReader::SortDGLines(CTypedPtrArray<CObArray, DG2DLine*>& DGLines)
{
	long NumLines = DGLines.GetSize();
    if(NumLines==0)
        return;
	CArray<double, double> LenArr;
	LenArr.SetSize(NumLines);
	for(long LineNum=0; LineNum<NumLines; LineNum++)
		LenArr[LineNum] = DGLines[LineNum]->m_pLine->Length();
	for(long i=0; i<NumLines; i++)
	{
		for(long j=i+1; j<NumLines; j++)
		{
			if(LenArr[i] < LenArr[j])
			{
				double Tmp = LenArr[i];
				LenArr[i] = LenArr[j];
				LenArr[j] = Tmp;
				DG2DLine* TmpLine = DGLines[i];
				DGLines[i] = DGLines[j];
				DGLines[j] = TmpLine;
			}
		}
	}
	for(long i=0; i<NumLines; i++)
	{
		DG2DLine* TmpLine = DGLines[i];
		TmpLine->m_ID = i;
	}

    double MinLength = DGLines[NumLines-1]->Length();
    if(MinLength < m_DTOL_DIST_LINKTEST)
    {
        m_DTOL_DIST_LINKTEST = MinLength - 0.1;
    }
}


void CogReader::SortDGArcs(CTypedPtrArray<CObArray, DG2DArc*>& DGArcs)
{
	long NumArcs = DGArcs.GetSize();
	CArray<double, double> LenArr;
	LenArr.SetSize(NumArcs);
	for(long ArcNum=0; ArcNum<NumArcs; ArcNum++)
		LenArr[ArcNum] = DGArcs[ArcNum]->m_pArc->m_A;
	for(long i=0; i<NumArcs; i++)
	{
		for(long j=i+1; j<NumArcs; j++)
		{
			if(LenArr[i] < LenArr[j])
			{
				double Tmp = LenArr[i];
				LenArr[i] = LenArr[j];
				LenArr[j] = Tmp;
				DG2DArc* TmpArc = DGArcs[i];
				DGArcs[i] = DGArcs[j];
				DGArcs[j] = TmpArc;
			}
		}
	}
}


void CogReader::SortPairLines(CTypedPtrArray<CObArray, DG2DLine*>& DGLines)
{
	double dTempLoc0 = 0., dTempLoc1=0.;
	DG2DLine* pTempLine;
	long NumLines = DGLines.GetSize();
	for(long LineNum=0; LineNum<NumLines; LineNum++)
	{
		DG2DLine* TheLine = DGLines[LineNum];
		long NumPair = TheLine->ma_Pair.GetSize();
		for(long iPair=0; iPair<NumPair; iPair++)
		{
			for(long jPair=iPair+1; jPair<NumPair; jPair++)
			{
				if(TheLine->ma_PairLoc0[iPair] > TheLine->ma_PairLoc0[jPair])
				{
					dTempLoc0 = TheLine->ma_PairLoc0[iPair];
					TheLine->ma_PairLoc0[iPair] = TheLine->ma_PairLoc0[jPair];
					TheLine->ma_PairLoc0[jPair] = dTempLoc0;

					dTempLoc1 = TheLine->ma_PairLoc1[iPair];
					TheLine->ma_PairLoc0[iPair] = TheLine->ma_PairLoc0[jPair];
					TheLine->ma_PairLoc0[jPair] = dTempLoc1;

					pTempLine = TheLine->ma_Pair[iPair];
					TheLine->ma_Pair[iPair] = TheLine->ma_Pair[jPair];
					TheLine->ma_Pair[jPair] = pTempLine;
				}
			}
		}
	}
}


void CogReader::ResetTag(CTypedPtrArray<CObArray, DG2DLine*>& DGLines)
{
	long NumLines = DGLines.GetSize();
	for(long LineNum=0; LineNum<NumLines; LineNum++)
		DGLines[LineNum]->m_Tag = 0;
}

void CogReader::ResetTag(CTypedPtrArray<CObArray, DG2DArc*>& DGArcs)
{
	long NumArcs = DGArcs.GetSize();
	for(long ArcNum=0; ArcNum<NumArcs; ArcNum++)
		DGArcs[ArcNum]->m_Tag = 0;
}

void CogReader::PutDGLinesIntoQT(CTypedPtrArray<CObArray, DG2DLine*>& DGLines)
{
	long NumLine=DGLines.GetSize();
	for(long LineNum=0; LineNum<NumLine; LineNum++)
	{
		DG2DLine* pLine = DGLines[LineNum];
		vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(pLine);
		long NumQT = CrossQTs.size();
		for(long QTNum=0; QTNum<NumQT; QTNum++)
			CrossQTs[QTNum]->Add2SearchSpace(pLine);
	}
}

void CogReader::PutDGArcsIntoQT(CTypedPtrArray<CObArray, DG2DArc*>& DGArcs)
{
	long NumArc=DGArcs.GetSize();
	for(long ArcNum=0; ArcNum<NumArc; ArcNum++)
	{
		DG2DArc* pArc = DGArcs[ArcNum];
		vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(pArc);
		long NumQT = CrossQTs.size();
		for(long QTNum=0; QTNum<NumQT; QTNum++)
			CrossQTs[QTNum]->Add2SearchSpace(pArc);
	}
}


// -1 : 링크라인 없다., 0 : 시작점 링크, 1 : 끝점 링크, 2 : 양끝
long CogReader::FindLinkedLine(DG2DLine* MyLine, long nDGType)
{
	if(MyLine==NULL)	return -1;

	DG2DLine* UrLine = NULL;

	CMap<GM2DGeometry*, GM2DGeometry*, GM2DGeometry*, GM2DGeometry*> ChkLVecMap;
	vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(MyLine);
	long NumQT = CrossQTs.size();
	for(long QTNum=0; QTNum<NumQT; QTNum++)
	{
		GMQuadTree* CurQT = CrossQTs[QTNum];
		CTypedPtrArray<CObArray, GM2DGeometry*>& Space = CurQT->ma_SearchSpace;
		long NumGM = Space.GetSize();
		for(long GMNum=0; GMNum<NumGM; GMNum++)
		{
			GM2DGeometry* AGeom = Space[GMNum];
 			if(AGeom == NULL || AGeom->m_ObjType != GT_LINE)
				continue;
			UrLine = (DG2DLine*)AGeom;
			if(MyLine->m_ID == UrLine->m_ID)
				continue;
			if(UrLine->m_Linked0 && UrLine->m_Linked1)  // 이미 양단 링크라인이 설정된 라인은 스킾
				continue;

			GM2DGeometry* rUrLine=NULL;
			if(ChkLVecMap.Lookup(UrLine, rUrLine))	continue;
			ChkLVecMap.SetAt(UrLine, UrLine);

			if(MyLine->m_Linked0 == NULL)
				MyLine->SetLinkLine(0, UrLine, m_DTOL_DIST_LINKTEST);
			if(MyLine->m_Linked1 == NULL)
				MyLine->SetLinkLine(1, UrLine, m_DTOL_DIST_LINKTEST);
		}
		if(MyLine->m_Linked0 && MyLine->m_Linked1)
			break;
	}

	return MyLine->GetLinkState();
}

// 0 : 웨지 없음, 1 : 웨지 제거
long CogReader::RemoveWedgeLink(DG2DLine* MyLine, long nDGType)
{
	if(MyLine->ma_Pair.GetSize() > 0)
		return 0;
	if(MyLine->Length() > m_MAX_WEDGE_LENGTH)	
		return 0;
	if(!MyLine->m_Linked0 || !MyLine->m_Linked1)
		return 0;

	if(MyLine->m_Linked0->m_pLine->m_ObjType != GT_LINE || MyLine->m_Linked1->m_pLine->m_ObjType != GT_LINE)
		return 0;

	GM2DLineVector Link0LVec = *((GM2DLineVector*)(MyLine->m_Linked0->m_pLine));
	if((MyLine->m_SharePntMask & NM_LINKED0)==1 ) 
		Link0LVec.ReverseDirection();
	GM2DLineVector Link1LVec = *((GM2DLineVector*)(MyLine->m_Linked1->m_pLine));
	if((MyLine->m_SharePntMask & NM_LINKED1)==1 ) 
		Link1LVec.ReverseDirection();

	GM2DVector CrossVec;
	double MyLoc, UrLoc;
	long Result = Link0LVec.IntersectEndWithLine(Link1LVec, CrossVec, &MyLoc, &UrLoc);

// 	if(!(Result==0 || Result==-5))
// 		return 0;

	if (!(Result == 1 || Result == 2))
		return 0;

	if(MyLoc>0. || UrLoc>0.)
		return 0;

	if((MyLine->m_SharePntMask & NM_LINKED0)==0 ) 
	{
		MyLine->m_Linked0->m_P0 = CrossVec;
		MyLine->m_Linked0->m_Linked0 = MyLine->m_Linked1;
		MyLine->m_Linked0->SetMask(NM_LINKED0, MyLine->GetMask(NM_LINKED1));
	}
	else
	{
		MyLine->m_Linked0->m_P1 = CrossVec;
		MyLine->m_Linked0->m_Linked1 = MyLine->m_Linked1;
		MyLine->m_Linked0->SetMask(NM_LINKED1, MyLine->GetMask(NM_LINKED1));
	}

	if((MyLine->m_SharePntMask & NM_LINKED1)==0 ) 
	{
		MyLine->m_Linked1->m_P0 = CrossVec;
		MyLine->m_Linked1->m_Linked0 = MyLine->m_Linked0;
		MyLine->m_Linked1->SetMask(NM_LINKED0, MyLine->GetMask(NM_LINKED0));
	}

	else
	{
		MyLine->m_Linked1->m_P1 = CrossVec;
		MyLine->m_Linked1->m_Linked1 = MyLine->m_Linked0;
		MyLine->m_Linked1->SetMask(NM_LINKED1, MyLine->GetMask(NM_LINKED0));
	}

	MyLine->m_Linked0 = NULL;
	MyLine->m_Linked1 = NULL;

	return 1;
}

// -1 : 링크라인 없다., 0 : 시작점 링크, 1 : 끝점 링크, 2 : 양끝
long CogReader::FindLinkedColLine(DG2DLine* MyLine, long nDGType)
{
	if(MyLine==NULL)	return -1;

	DG2DLine* UrLine = NULL;

	CMap<GM2DLineVector*, GM2DLineVector*, GM2DLineVector*, GM2DLineVector*> ChkLVecMap;
	vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(MyLine);
	long NumQT = CrossQTs.size();
	for(long QTNum=0; QTNum<NumQT; QTNum++)
	{
		GMQuadTree* CurQT = CrossQTs[QTNum];
		CTypedPtrArray<CObArray, GM2DGeometry*>& ColLayer = CurQT->ma_GMs[DG_COLUMN];
		long NumGM = ColLayer.GetSize();
		for(long GMNum=0; GMNum<NumGM; GMNum++)
		{
			GM2DGeometry* AGeom = ColLayer[GMNum];
			if(AGeom == NULL || AGeom->m_ObjType != GT_LINE)
				continue;
			GM2DLineVector* ALine = (GM2DLineVector*)AGeom;
			if(ALine->m_Link0 && ALine->m_Link1)  // 이미 양단 링크라인이 설정된 라인은 스킾
				continue;

			GM2DLineVector* rUrLine=NULL;
			if(ChkLVecMap.Lookup(ALine, rUrLine))	continue;
			ChkLVecMap.SetAt(ALine, ALine);

			if(MyLine->m_P0.Distance(ALine->m_P0) < m_DTOL_DIST_LINKTEST)
			{
				MyLine->m_LinkColLVec0 = ALine;
				ALine->m_Link0 = MyLine;
			}
			else if(MyLine->m_P0.Distance(ALine->m_P1) < m_DTOL_DIST_LINKTEST)
			{
				MyLine->m_LinkColLVec0 = ALine;
				ALine->m_Link1 = MyLine;
			}
			if(MyLine->m_P1.Distance(ALine->m_P0) < m_DTOL_DIST_LINKTEST)
			{
				MyLine->m_LinkColLVec1 = ALine;
				ALine->m_Link0 = MyLine;
			}
			else if(MyLine->m_P1.Distance(ALine->m_P1) < m_DTOL_DIST_LINKTEST)
			{
				MyLine->m_LinkColLVec1 = ALine;
				ALine->m_Link1 = MyLine;
			}
		}
		if(MyLine->m_LinkColLVec0 && MyLine->m_LinkColLVec1)
			break;
	}

	return MyLine->GetColLinkState();
}

BOOL CogReader::IsOverlap(DG2DLine* MyLine, DG2DLine* UrLine)
{
	double MyLoc0 = UrLine->OrthoBaseLocValue(MyLine->m_P0);
	double MyLoc1 = UrLine->OrthoBaseLocValue(MyLine->m_P1);
	long Result = OverlapGroup(MyLoc0, MyLoc1, m_DTOL_DIST_LINKTEST/10000.);
	// 1. 서로 중첩안됨
	if(Result==1 || Result==2 || Result==6 || Result==7 || Result==19 || Result==20 || Result==24 || Result==25)
		return FALSE;
	else
		return TRUE;
}

long CogReader::OverlapGroup(double P0Loc, double P1Loc, double DistTol)
{
	if(P0Loc < -DistTol)
		if(P1Loc < -DistTol)
			return 1;
		else if( fabs(P1Loc) <= DistTol )
			return 2;
		else if( P1Loc > DistTol && P1Loc < 1. && (1.-P1Loc) > DistTol )
			return 3;
		else if( fabs(1.-P1Loc) <= DistTol )
			return 4;
		else 
			return 5;
	else if( fabs(P0Loc) <= DistTol )
		if(P1Loc < -DistTol)
			return 6;
		else if( fabs(P1Loc) <= DistTol )
			return 7;
		else if( P1Loc > DistTol && P1Loc < 1. && (1.-P1Loc) > DistTol )
			return 8;
		else if( fabs(1.-P1Loc) <= DistTol )
			return 9;
		else 
			return 10;
	else if( P0Loc > DistTol && P0Loc < 1. && (1.-P0Loc) > DistTol )
		if(P1Loc < -DistTol)
			return 11;
		else if( fabs(P1Loc) <= DistTol )
			return 12;
		else if( P1Loc > DistTol && P1Loc < 1. && (1.-P1Loc) > DistTol )
			return 13;
		else if( fabs(1.-P1Loc) <= DistTol )
			return 14;
		else 
			return 15;
	else if( fabs(1.-P0Loc) <= DistTol )
		if(P1Loc < -DistTol)
			return 16;
		else if( fabs(P1Loc) <= DistTol )
			return 17;
		else if( P1Loc > DistTol && P1Loc < 1. && (1.-P1Loc) > DistTol )
			return 18;
		else if( fabs(1.-P1Loc) <= DistTol )
			return 19;
		else 
			return 20;
	else
		if(P1Loc < -DistTol)
			return 21;
		else if( fabs(P1Loc) <= DistTol )
			return 22;
		else if( P1Loc > DistTol && P1Loc < 1. && (1.-P1Loc) > DistTol )
			return 23;
		else if( fabs(1.-P1Loc) <= DistTol )
			return 24;
		else 
			return 25;
}

XErrorObject* CogReader::AddErrorObject(vector<MSElement*>& Elements, long nErrorCode, long nLevel, CString strGroup, CString strMsg)
{
	XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();
	if(pMessageContainer == NULL)
	{
		ASSERT(0);
		return NULL;
	}

	XErrorObject* pMsgObject = pMessageContainer->CreateErrorObject(nErrorCode, nLevel, strMsg, XErrorObject::MSElementIDSelect);

	if(pMsgObject == NULL)
	{
		ASSERT(0);
		return NULL;
	}

	if (pMsgObject)
	{
		pMsgObject->SetGroupName(strGroup);
		pMsgObject->SetShowSelectCompoG(TRUE);
		pMessageContainer->AddErrorObject(pMsgObject);
		pMsgObject->SetDocID(m_nDocID);

		long NumIDs = Elements.size();
		for(int i = 0; i < NumIDs; i++)
			pMsgObject->ma_CompoGID.Add(Elements[i]->m_ID);

		mm_ErrorToMSElements.insert(make_pair(pMsgObject, Elements));

		CString log;
		log.Format(L"CogReader: %s", pMsgObject->ToString());
		LoggerWrapper::Log(log);
	}
	return pMsgObject;
}

void CogReader::RemoveErrorToMSElement(MSElement* pElement)
{
	vector<map<XErrorObject*, vector<MSElement*>>::iterator> ErrorObjectItArr;
	map<XErrorObject*, vector<MSElement*>>::iterator it ;
	for(it = mm_ErrorToMSElements.begin(); it != mm_ErrorToMSElements.end(); it++)
	{
		vector<MSElement*>& Elements = it->second;
		vector<MSElement*>::iterator itEle;
		
		for(int i = Elements.size()-1; i >= 0; i--)
		{
			if(Elements[i] == pElement)
			{
				Elements.erase(Elements.begin() + i);
				break;
			}
		}
		if (Elements.size() == 0)
			ErrorObjectItArr.push_back(it);
	}
	for (int i = 0; i < ErrorObjectItArr.size(); i++)
		mm_ErrorToMSElements.erase(ErrorObjectItArr[i]);
}

XErrorObject* CogReader::AddErrorObject(vector<long>& FigureIDs, long nErrorCode, long nLevel, CString strGroup, CString strMsg)
{
	XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();
	if(pMessageContainer == NULL)
	{
		ASSERT(0);
		return NULL;
	}

	XErrorObject* pMsgObject = pMessageContainer->CreateErrorObject(nErrorCode, nLevel, strMsg, XErrorObject::VDFigureIDSelect);

	if(pMsgObject == NULL)
	{
		ASSERT(0);
		return NULL;
	}

	if (pMsgObject)
	{
		pMsgObject->SetGroupName(strGroup);
		//		pMsgObject->AddCompoGID(pMyLink->m_ID);
		pMsgObject->SetShowSelectCompoG(FALSE);
		pMessageContainer->AddErrorObject(pMsgObject);
		pMsgObject->SetDocID(m_nDocID);

		long NumIDs = FigureIDs.size();
		for(int i = 0; i < NumIDs; i++)
			pMsgObject->ma_FigureID.Add(FigureIDs[i]);

		CString log;
		log.Format(L"CogReader: %s", pMsgObject->ToString());
		LoggerWrapper::Log(log);
	}
	return pMsgObject;
}

BOOL CogReader::SearchParallelLine(DG2DLine* MyLine, double MinThick, double MaxThick, vector<GMQuadTree*>& CrossQTs, long LayerNum)
{
	double dMinLength = m_MIN_WALL_LENGTH;
	if(m_nMemberType==DG_GRIDLINE)
		dMinLength = m_dGridLineMinLength;
	if(MyLine == NULL) 
		return NULL;

	MyLine->m_Tag = 1;

// 	double MinErrRatio = 100.;
	DG2DLine* UrLine = NULL;

	CMap<DG2DLine*, DG2DLine*, DG2DLine*, DG2DLine*> ChkDGLineMap;
	long NumQT = CrossQTs.size();
	for(long QTNum=0; QTNum<NumQT; QTNum++)
	{
		GMQuadTree* CurQT = CrossQTs[QTNum];
		CTypedPtrArray<CObArray, GM2DGeometry*>& Space = CurQT->ma_SearchSpace;
		long NumGM = Space.GetSize();
		for(long GMNum=0; GMNum<NumGM; GMNum++)
		{
			GM2DGeometry* pGeom = Space[GMNum];
			if(pGeom == MyLine || pGeom->m_Tag == 1 || pGeom->m_ObjType != GT_LINE)
				continue;
			// 평행성 검사
			UrLine = (DG2DLine*)pGeom;
			DG2DLine* rUrLine=NULL;

			// 다른 QT 안에 같은 DGLine이 중복 소속된 경우 페이라인으로 두번 등록되는 것을방지 
			if(ChkDGLineMap.Lookup(UrLine, rUrLine))
				continue;
			ChkDGLineMap.SetAt(UrLine, UrLine);

			// 서로 m_DTOL_MIN_WALL_LENGTH 이상 중첩되는지 검사
			double UrP0Loc, UrP1Loc;
			BOOL Result = MyLine->m_pLine->OverlapTest4DTol(UrLine->m_pLine, UrP0Loc, UrP1Loc, m_DTOL_PARALLEL_DEGREE, MaxThick);
			// 서로 중첩되지 않음
			if(!Result)
				continue;

			double Dist = MyLine->Distance(UrLine->m_P0, true);
			if(fabs(Dist)<MinThick) 
				continue;

			if(UrLine->GetLinkNext(UrLine->m_Linked0)==MyLine || UrLine->GetLinkNext(UrLine->m_Linked1)==MyLine)
			{
				BOOL MyLineFull = MyLine->AddPairLine(UrLine, MyLine->OrthoBaseLocValue(UrLine->m_P0), MyLine->OrthoBaseLocValue(UrLine->m_P1), Result, m_MAX_GAP_LENGTH / 4., m_DTOL_DIST_LINKTEST);
				if(MyLineFull)
					return TRUE;
				else
					continue;
			}

			double dMinDistOfPair = fabs(Dist);

			vector<DG2DLine*> PairAltArr;
			vector<double> PairAltDistArr;

			PairAltArr.push_back(UrLine);
			PairAltDistArr.push_back(Dist);
			CountFeasiblePairLines(dMinDistOfPair, PairAltArr, PairAltDistArr, MyLine, UrLine, MinThick, MaxThick);
			long nPairLinesUrSide = PairAltArr.size();
/*
			if((nPairLinesUrSide+nPairLinesOtherSide)%2==0)  // 유어라인 구간 내 마이라인 좌우로 페어 가능한 라인이 짝수이면, 마이 합쳐서 홀수가 되므로 페어링 포기. 사용자에게 메시지
			{
				ASSERT(0);
				continue;
			}
			else 
*/
			if(nPairLinesUrSide%2==0)  // 유어라인 쪽 짝수, 반대 쪽 홀수이므로 반대 쪽 라인 차례에 반대 쪽 라인과 페어링
				continue;

			// 유어라인 쪽에 3개 이상의 홀수 평행선이 있어서 별도의 부재 생성이 가능한 경우로서, 마이라인과 유어라인 사이에 더 가까운 평행선이 있으므로 스킾
			if(nPairLinesUrSide>=3 && nPairLinesUrSide%2==1 && dMinDistOfPair<fabs(Dist))
				continue;
			// 이후 조건 추가 검토 요망
			if(nPairLinesUrSide == 2 && UrLine->Length()<dMinLength)
				continue;
			if(nPairLinesUrSide%2 == 0)
			{
				if( (Dist-dMinDistOfPair) > DTOL_GM)
					continue;
				if(MyLine->ma_Pair.GetSize()>0)
				{
					DG2DLine* PairLine = MyLine->ma_Pair[0];
					long PairSide = MyLine->WhichSide(PairLine->m_P0);
					long UrSide = MyLine->WhichSide(UrLine->m_P0);
					if(PairSide*UrSide < 0)
						continue;
				}
			}

			BOOL MyLineFull = MyLine->AddPairLine(UrLine, MyLine->OrthoBaseLocValue(UrLine->m_P0), MyLine->OrthoBaseLocValue(UrLine->m_P1), Result, m_MAX_GAP_LENGTH / 4., m_DTOL_DIST_LINKTEST);
			//			BOOL MyLineFull = MyLine->AddPairLine(UrLine, MyLoc0, MyLoc1, Result, m_MAX_GAP_LENGTH);

			if(MyLineFull)
				return TRUE;
		}
	}
 	if(MyLine->ma_Pair.GetSize()==0 && MyLine->Length()>m_MIN_WALL_LENGTH)
 	{
		vector<long> FigureIDs;

		if(MyLine->m_Linked0 && MyLine->m_Linked1)
			return FALSE;

		AddVdFigureID(FigureIDs, FindGeometyToFigureIDs(MyLine));
		CString GroupName = GetGroupName();
		CString strMsg = ERROR_PAIR_NONE;
		if(AppGlobalOptionWrapper::Instance()->GetAppMode() != AppGlobalOptionWrapper::Formwork)
			AddErrorObject(FigureIDs, E_CODE_PAIR_NONE, ERROR_LEVEL, GroupName, strMsg);
	}
	return FALSE;
}

void CogReader::CountFeasiblePairLines(double& dMinDistOfPair, vector<DG2DLine*>& PairAltArr, vector<double>& PairAltDistArr, 
									   DG2DLine* RootLine, DG2DLine* MyLine, double MinThick, double MaxThick)
{
	GM2DVector BaseVec0;
	double OverlapLen = 0.;
	DG2DLine* UrLine;
	long OldMyTag = MyLine->m_Tag;

	long MySide = RootLine->m_pLine->WhichSide(MyLine->m_pLine->m_P0);

	CMap<DG2DLine*, DG2DLine*, DG2DLine*, DG2DLine*> ChkDGLineMap;
	vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(MyLine);
	long NumQT = CrossQTs.size();
	for(long QTNum=0; QTNum<NumQT; QTNum++)
	{
		GMQuadTree* CurQT = CrossQTs[QTNum];
		CTypedPtrArray<CObArray, GM2DGeometry*>& Space = CurQT->ma_SearchSpace;

		long NumGM = Space.GetSize();
		for(long GMNum=0; GMNum<NumGM; GMNum++)
		{
			GM2DGeometry* pGeom = Space[GMNum];
			long OldUrTag = pGeom->m_Tag;
			if(pGeom == RootLine || pGeom->m_ObjType != GT_LINE)
				continue;
			long NumPairAlt = PairAltArr.size();
			for(long AltNum=0; AltNum<NumPairAlt; AltNum++)
			{
				if(PairAltArr[AltNum]==pGeom)
					break;
			}
			if(AltNum<NumPairAlt)
				continue;

			// 평행성 검사
			UrLine = (DG2DLine*)pGeom;
			DG2DLine* rUrLine=NULL;
			if(ChkDGLineMap.Lookup(UrLine, rUrLine))
				continue;
			ChkDGLineMap.SetAt(UrLine, UrLine);

			if(UrLine->m_Linked0 && UrLine->m_Linked1 && UrLine->Length()<MaxThick 
				&& IsOverlap(UrLine->m_Linked0, UrLine->m_Linked1) 
				&& !MyLine->IsLink(UrLine->m_Linked0) && !MyLine->IsLink(UrLine->m_Linked1))
				continue;

			// 서로 m_DTOL_MIN_WALL_LENGTH 이상 중첩되는지 검사
			double UrP0Loc, UrP1Loc;
			BOOL Result = MyLine->m_pLine->OverlapTest4DTol(UrLine->m_pLine, UrP0Loc, UrP1Loc, m_DTOL_PARALLEL_DEGREE, MaxThick);
			// 서로 중첩되지 않음
			if(!Result)
				continue;
			Result = RootLine->m_pLine->OverlapTest4DTol(UrLine->m_pLine, UrP0Loc, UrP1Loc, m_DTOL_PARALLEL_DEGREE, 1.E10);
			// 서로 중첩되지 않음
			if(!Result)
				continue;

			double Dist = MyLine->Distance(UrLine->m_P0);
			if(Dist<MinThick) 
				continue;

			long UrSide = RootLine->m_pLine->WhichSide(UrLine->m_pLine->m_P0);
			double AltDist = RootLine->Distance(UrLine->m_P0,true);
			if(fabs(Dist-AltDist)<MinThick && !MyLine->m_pLine->OverlapTest4DTol(UrLine->m_pLine, UrP0Loc, UrP1Loc, m_DTOL_PARALLEL_DEGREE, MinThick))
				continue;
			if(MySide*UrSide <= 0)
				continue;
			{
				long NumPairAlt = PairAltArr.size();
				if(NumPairAlt==0)
				{
					if(dMinDistOfPair > fabs(AltDist))
						dMinDistOfPair = fabs(AltDist);
					PairAltArr.push_back(UrLine);
					PairAltDistArr.push_back(AltDist);
				}
				else
				{
					// 페어라인 후보자(MyLine)과 같은 방향으로 오버랩되는 라인들을 PairAltArr에 저장하는데
					// 기존 alt 오버랩되는 라인은 서로 짝을 이룰 수 있으므로 추가 저장하고,
					// 그렇지 않은 것은 짝이 될 가능성이 없으므로 스킾
					for(long AltNum=0; AltNum<NumPairAlt; AltNum++)
					{
						if(!PairAltArr[AltNum]->m_pLine->OverlapTest4DTol(UrLine->m_pLine, UrP0Loc, UrP1Loc, m_DTOL_PARALLEL_DEGREE, 1.E10))
// 						if(fabs(PairAltDistArr[AltNum]-AltDist)<MinThick && !PairAltArr[AltNum]->m_pLine->OverlapTest4DTol(UrLine->m_pLine, UrP0Loc, UrP1Loc, m_DTOL_PARALLEL_DEGREE, MinThick))
							break;
					}
					if(AltNum<NumPairAlt)
						continue;
					for(long AltNum=0; AltNum<NumPairAlt; AltNum++)
						if(PairAltArr[AltNum]->m_pLine->OverlapTest4DTol(UrLine->m_pLine, UrP0Loc, UrP1Loc, m_DTOL_PARALLEL_DEGREE, MaxThick))
							break;
					if(AltNum<NumPairAlt)
					{
						if(dMinDistOfPair > fabs(AltDist))
							dMinDistOfPair = fabs(AltDist);
						PairAltArr.push_back(UrLine);
						PairAltDistArr.push_back(AltDist);
					}
					if(AltNum==NumPairAlt)
						continue;
				}
				CountFeasiblePairLines(dMinDistOfPair, PairAltArr, PairAltDistArr, RootLine, UrLine, MinThick, MaxThick);
			}
		}
	}
	return;
}

/*
BOOL CogReader::SearchParallelLine(DG2DLine* MyLine, double MinThick, double MaxThick, CTypedPtrArray<CObArray, GMQuadTree*>& CrossQTs, long LayerNum)
{
	double dMinLength = m_MIN_WALL_LENGTH;
	if(m_nMemberType==DG_GRIDLINE)
		dMinLength = m_dGridLineMinLength;
	if(MyLine == NULL) 
		return NULL;

	MyLine->m_Tag = 1;

// 	double MinErrRatio = 100.;
	DG2DLine* UrLine = NULL;

	CMap<DG2DLine*, DG2DLine*, DG2DLine*, DG2DLine*> ChkDGLineMap;
	long NumQT = CrossQTs.GetSize();
	for(long QTNum=0; QTNum<NumQT; QTNum++)
	{
		GMQuadTree* CurQT = CrossQTs[QTNum];
		CTypedPtrArray<CObArray, GM2DGeometry*>& Space = CurQT->ma_SearchSpace;
		long NumGM = Space.GetSize();
		for(long GMNum=0; GMNum<NumGM; GMNum++)
		{
			GM2DGeometry* pGeom = Space[GMNum];
			if(pGeom == MyLine || pGeom->m_Tag == 1 || pGeom->m_ObjType != GT_LINE)
				continue;
			// 평행성 검사
			UrLine = (DG2DLine*)pGeom;
			DG2DLine* rUrLine=NULL;

			// 다른 QT 안에 같은 DGLine이 중복 소속된 경우 페이라인으로 두번 등록되는 것을방지 
			if(ChkDGLineMap.Lookup(UrLine, rUrLine))
				continue;
			ChkDGLineMap.SetAt(UrLine, UrLine);

			// 서로 m_DTOL_MIN_WALL_LENGTH 이상 중첩되는지 검사
			double UrP0Loc, UrP1Loc;
			BOOL Result = MyLine->m_pLine->OverlapTest4DTol(UrLine->m_pLine, UrP0Loc, UrP1Loc, m_DTOL_PARALLEL_DEGREE, MaxThick);
			// 서로 중첩되지 않음
			if(!Result)
				continue;

			double Dist = MyLine->Distance(UrLine->m_P0, true);
			if(fabs(Dist)<MinThick) 
				continue;

			if(UrLine->GetLinkNext(UrLine->m_Linked0)==MyLine || UrLine->GetLinkNext(UrLine->m_Linked1)==MyLine)
			{
				BOOL MyLineFull = MyLine->AddPairLine(UrLine, MyLine->OrthoBaseLocValue(UrLine->m_P0), MyLine->OrthoBaseLocValue(UrLine->m_P0), Result, m_MAX_GAP_LENGTH / 4., m_DTOL_DIST_LINKTEST);
				if(MyLineFull)
					return TRUE;
				else
					continue;
			}

			long nPairLinesUrSide = 1;
			long nPairLinesOtherSide = 0;
			double dMinDistOfPair = fabs(Dist);

			vector<DG2DLine*> PairAltArr;
			vector<double> PairAltDistArr;
			vector<DG2DLine*> PairAltArrOther;
			vector<double> PairAltDistArrOther;

			PairAltArr.push_back(UrLine);
			PairAltDistArr.push_back(Dist);
			CountFeasiblePairLines(dMinDistOfPair, PairAltArr, PairAltDistArr, PairAltArrOther, PairAltDistArrOther, MyLine, UrLine, MinThick, MaxThick);
			nPairLinesUrSide = PairAltArr.size();
			nPairLinesOtherSide = PairAltArrOther.size();
/ *
			if((nPairLinesUrSide+nPairLinesOtherSide)%2==0)  // 유어라인 구간 내 마이라인 좌우로 페어 가능한 라인이 짝수이면, 마이 합쳐서 홀수가 되므로 페어링 포기. 사용자에게 메시지
			{
//				ASSERT(0);
				AddErrorObject(MyLine, COGREADER_ERROR_NOT_FIND_COMPOG, MESSAGE_LEVEL_ERROR, COGREADER_GROUP, COGREADER_ERROR_NOT_FIND_COMPOG_MSG);
				


				continue;
			}
			else 
* /
			if(nPairLinesUrSide%2==0)  // 유어라인 쪽 짝수, 반대 쪽 홀수이므로 반대 쪽 라인 차례에 반대 쪽 라인과 페어링
				continue;

			// 유어라인 쪽에 3개 이상의 홀수 평행선이 있어서 별도의 부재 생성이 가능한 경우로서, 마이라인과 유어라인 사이에 더 가까운 평행선이 있으므로 스킾
			if(nPairLinesUrSide>=3 && nPairLinesUrSide%2==1 && dMinDistOfPair<Dist)
				continue;
			// 이후 조건 추가 검토 요망
			if(nPairLinesUrSide == 2 && UrLine->Length()<dMinLength)
				continue;
			if(nPairLinesUrSide%2 == 0)
			{
				if(nPairLinesOtherSide%2 != 0)
					continue;
				if( (Dist-dMinDistOfPair) > DTOL_GM)
					continue;
				if(MyLine->ma_Pair.GetSize()>0)
				{
					DG2DLine* PairLine = MyLine->ma_Pair[0];
					long PairSide = MyLine->WhichSide(PairLine->m_P0);
					long UrSide = MyLine->WhichSide(UrLine->m_P0);
					if(PairSide*UrSide < 0)
						continue;
				}
			}

			BOOL MyLineFull = MyLine->AddPairLine(UrLine, MyLine->OrthoBaseLocValue(UrLine->m_P0), MyLine->OrthoBaseLocValue(UrLine->m_P0), Result, m_MAX_GAP_LENGTH / 4., m_DTOL_DIST_LINKTEST);
			//			BOOL MyLineFull = MyLine->AddPairLine(UrLine, MyLoc0, MyLoc1, Result, m_MAX_GAP_LENGTH);

			if(MyLineFull)
				return TRUE;
		}
	}
	if(MyLine->ma_Pair.GetSize()==0 && MyLine->Length()>m_MIN_WALL_LENGTH)
		ASSERT(0);
	return FALSE;
}

void CogReader::CountFeasiblePairLines(double& dMinDistOfPair, 
									   vector<DG2DLine*>& PairAltArr, vector<double>& PairAltDistArr, vector<DG2DLine*>& PairAltArrOther, vector<double>& PairAltDistArrOther,
									   DG2DLine* RootLine, DG2DLine* MyLine, double MinThick, double MaxThick)
{
	GM2DVector BaseVec0;
	double OverlapLen = 0.;
	DG2DLine* UrLine;
	long OldMyTag = MyLine->m_Tag;
	MyLine->m_Tag = -1;

	long MySide = RootLine->m_pLine->WhichSide(MyLine->m_pLine->m_P0);

	CMap<DG2DLine*, DG2DLine*, DG2DLine*, DG2DLine*> ChkDGLineMap;
	CTypedPtrArray<CObArray, GMQuadTree*>& CrossQTs = MyLine->m_CrossQTs;
	long NumQT = CrossQTs.GetSize();
	for(long QTNum=0; QTNum<NumQT; QTNum++)
	{
		GMQuadTree* CurQT = CrossQTs[QTNum];
		CTypedPtrArray<CObArray, GM2DGeometry*>& Space = CurQT->ma_SearchSpace;

		long NumGM = Space.GetSize();
		for(long GMNum=0; GMNum<NumGM; GMNum++)
		{
			GM2DGeometry* pGeom = Space[GMNum];
			long OldUrTag = pGeom->m_Tag;
			if(pGeom == RootLine || pGeom->m_ObjType != GT_LINE)
				continue;
			long NumPairAlt = PairAltArr.size();
			for(long AltNum=0; AltNum<NumPairAlt; AltNum++)
			{
				if(PairAltArr[AltNum]==pGeom)
					break;
			}
			if(AltNum<NumPairAlt)
				continue;
			NumPairAlt = PairAltArrOther.size();
			for(long AltNum=0; AltNum<NumPairAlt; AltNum++)
			{
				if(PairAltArrOther[AltNum]==pGeom)
					break;
			}
			if(AltNum<NumPairAlt)
				continue;

			// 평행성 검사
			UrLine = (DG2DLine*)pGeom;
			DG2DLine* rUrLine=NULL;
			if(ChkDGLineMap.Lookup(UrLine, rUrLine))
				continue;
			ChkDGLineMap.SetAt(UrLine, UrLine);

			if(UrLine->m_Linked0 && UrLine->m_Linked1 && UrLine->Length()<MaxThick 
				&& IsOverlap(UrLine->m_Linked0, UrLine->m_Linked1) 
				&& !MyLine->IsLink(UrLine->m_Linked0) && !MyLine->IsLink(UrLine->m_Linked1))
				continue;

			// 서로 m_DTOL_MIN_WALL_LENGTH 이상 중첩되는지 검사
			double UrP0Loc, UrP1Loc;
			BOOL Result = MyLine->m_pLine->OverlapTest4DTol(UrLine->m_pLine, UrP0Loc, UrP1Loc, m_DTOL_PARALLEL_DEGREE, MaxThick);
			// 서로 중첩되지 않음
			if(!Result)
				continue;
			Result = RootLine->m_pLine->OverlapTest4DTol(UrLine->m_pLine, UrP0Loc, UrP1Loc, m_DTOL_PARALLEL_DEGREE, 1.E10);
			// 서로 중첩되지 않음
			if(!Result)
				continue;

			double Dist = MyLine->Distance(UrLine->m_P0);
			if(Dist<MinThick) 
				continue;

			long UrSide = RootLine->m_pLine->WhichSide(UrLine->m_pLine->m_P0);
			double AltDist = RootLine->Distance(UrLine->m_P0,true);
			if(fabs(Dist-AltDist)<MinThick && !MyLine->m_pLine->OverlapTest4DTol(UrLine->m_pLine, UrP0Loc, UrP1Loc, m_DTOL_PARALLEL_DEGREE, MinThick))
				continue;
			if(MySide*UrSide > 0)
			{
				long NumPairAlt = PairAltArr.size();
				if(NumPairAlt==0)
				{
					if(dMinDistOfPair > fabs(AltDist))
						dMinDistOfPair = fabs(AltDist);
					PairAltArr.push_back(UrLine);
					PairAltDistArr.push_back(AltDist);
				}
				else
				{
					// 페어라인 후보자(MyLine)과 같은 방향으로 오버랩되는 라인들을 PairAltArr에 저장하는데
					// 기존 alt 오버랩되는 라인은 서로 짝을 이룰 수 있으므로 추가 저장하고,
					// 그렇지 않은 것은 짝이 될 가능성이 없으므로 스킾
					for(long AltNum=0; AltNum<NumPairAlt; AltNum++)
					{
						if(!PairAltArr[AltNum]->m_pLine->OverlapTest4DTol(UrLine->m_pLine, UrP0Loc, UrP1Loc, m_DTOL_PARALLEL_DEGREE, 1.E10))
// 						if(fabs(PairAltDistArr[AltNum]-AltDist)<MinThick && !PairAltArr[AltNum]->m_pLine->OverlapTest4DTol(UrLine->m_pLine, UrP0Loc, UrP1Loc, m_DTOL_PARALLEL_DEGREE, MinThick))
							break;
					}
					if(AltNum<NumPairAlt)
						continue;
					for(long AltNum=0; AltNum<NumPairAlt; AltNum++)
						if(PairAltArr[AltNum]->m_pLine->OverlapTest4DTol(UrLine->m_pLine, UrP0Loc, UrP1Loc, m_DTOL_PARALLEL_DEGREE, MaxThick))
							break;
					if(AltNum<NumPairAlt)
					{
						if(dMinDistOfPair > fabs(AltDist))
							dMinDistOfPair = fabs(AltDist);
						PairAltArr.push_back(UrLine);
						PairAltDistArr.push_back(AltDist);
					}
					if(AltNum==NumPairAlt)
						continue;
				}
				CountFeasiblePairLines(dMinDistOfPair, PairAltArr, PairAltDistArr, PairAltArrOther, PairAltDistArrOther, RootLine, UrLine, MinThick, MaxThick);
			}
			else
			{
				long NumPairAlt = PairAltArrOther.size();
				if(NumPairAlt==0)
				{
					if(dMinDistOfPair > fabs(AltDist))
						dMinDistOfPair = fabs(AltDist);
					PairAltArrOther.push_back(UrLine);
					PairAltDistArrOther.push_back(AltDist);
				}
				else
				{
					for(long AltNum=0; AltNum<NumPairAlt; AltNum++)
					{
// 						if(fabs(PairAltDistArrOther[AltNum]-AltDist)<MinThick && !PairAltArrOther[AltNum]->m_pLine->OverlapTest4DTol(UrLine->m_pLine, UrP0Loc, UrP1Loc, m_DTOL_PARALLEL_DEGREE, MinThick))
						if(!PairAltArrOther[AltNum]->m_pLine->OverlapTest4DTol(UrLine->m_pLine, UrP0Loc, UrP1Loc, m_DTOL_PARALLEL_DEGREE, 1.E10))
							break;
					}
					if(AltNum<NumPairAlt)
						continue;
					for(long AltNum=0; AltNum<NumPairAlt; AltNum++)
						if(PairAltArrOther[AltNum]->m_pLine->OverlapTest4DTol(UrLine->m_pLine, UrP0Loc, UrP1Loc, m_DTOL_PARALLEL_DEGREE, MaxThick))
							break;
					if(AltNum<NumPairAlt)
					{
						if(dMinDistOfPair > fabs(AltDist))
							dMinDistOfPair = fabs(AltDist);
						PairAltArrOther.push_back(UrLine);
						PairAltDistArrOther.push_back(AltDist);
					}
					if(AltNum==NumPairAlt)
						continue;
				}

				CountFeasiblePairLines(dMinDistOfPair, PairAltArr, PairAltDistArr, PairAltArrOther, PairAltDistArrOther, RootLine, UrLine, MinThick, MaxThick);
			}
		}
	}
	return;
}
*/


/*
BOOL CogReader::SearchParallelLine(DG2DLine* MyLine, double MinThick, double MaxThick, CTypedPtrArray<CObArray, GMQuadTree*>& CrossQTs, long LayerNum)
{
	double dMinLength = m_MIN_WALL_LENGTH;
	if(m_nMemberType==DG_GRIDLINE)
		dMinLength = m_dGridLineMinLength;
	if(MyLine == NULL) 
		return NULL;

	MyLine->m_Tag = 1;

	double MinErrRatio = 100.;
	DG2DLine* UrLine = NULL;
	DG2DLine* AltLine = NULL;

	double MyLoc0, MyLoc1;
	GM2DVector BaseVec0;
	double OverlapLen = 0.;

	long FeasibleDir = 0;

	CMap<DG2DLine*, DG2DLine*, DG2DLine*, DG2DLine*> ChkDGLineMap;
	long NumQT = CrossQTs.GetSize();
	for(long QTNum=0; QTNum<NumQT; QTNum++)
	{
		GMQuadTree* CurQT = CrossQTs[QTNum];
		CTypedPtrArray<CObArray, GM2DGeometry*>& Space = CurQT->ma_SearchSpace;
		long NumGM = Space.GetSize();
		for(long GMNum=0; GMNum<NumGM; GMNum++)
		{
			GM2DGeometry* AGeom = Space[GMNum];
			if(AGeom == NULL || AGeom->m_Tag != 0 || AGeom->m_ObjType != GT_LINE)
				continue;
// 평행성 검사
			UrLine = (DG2DLine*)AGeom;
			DG2DLine* rUrLine=NULL;

// 다른 QT 안에 같은 DGLine이 중복 소속된 경우 페이라인으로 두번 등록되는 것을방지 
			if(ChkDGLineMap.Lookup(UrLine, rUrLine))
				continue;
			ChkDGLineMap.SetAt(UrLine, UrLine);

			long Result = MyLine->IsParallel4DTol(*UrLine, m_DTOL_PARALLEL_DEGREE);
			if(Result == 0) 
				continue;
// 주어진 폭(width) 대비 m_DTOL_PAIR_DIST_ERRRATIO 이하 오차인지 검사
			MyLoc0 = MyLine->OrthoBaseLocValue(UrLine->m_P0);
			BaseVec0 = MyLine->Point2DVector(MyLoc0);
			double Dist = BaseVec0.Distance(UrLine->m_P0);
			if(Dist>MaxThick || Dist<MinThick) 
				continue;
// 서로 m_DTOL_MIN_WALL_LENGTH 이상 중첩되는지 검사
			MyLoc1 = MyLine->OrthoBaseLocValue(UrLine->m_P1);
  		Result = OverlapGroup(MyLoc0, MyLoc1, m_DTOL_DIST_LINKTEST/10000);
// 서로 중첩되지 않음
			if(Result==1 || Result==2 || Result==6 || Result==7 || Result==19 || Result==20 || Result==24 || Result==25)
				continue;

            if(UrLine->GetLinkNext(UrLine->m_Linked0)==MyLine || UrLine->GetLinkNext(UrLine->m_Linked1)==MyLine)
            {
                BOOL MyLineFull = MyLine->AddPairLine(UrLine, MyLoc0, MyLoc1, Result, m_MAX_GAP_LENGTH / 4., m_DTOL_DIST_LINKTEST);
                if(MyLineFull)
                    return TRUE;
            }

			long nPairLinesUrSide = 1;
			long nPairLinesOtherSide = 0;
			double dMinDistOfPair = Dist;
			//			YouHaveNoFeasiblePair(NumFeasibleLines, &MyLine->m_P0, UrLine, MinThick, MaxThick, LayerNum);
			GMLine MyLineEq;
			MyLineEq.SetTwoPoint(MyLine->m_P0.m_X, MyLine->m_P0.m_Y, 
				MyLine->m_P1.m_X, MyLine->m_P1.m_Y);
			YouHaveNoFeasiblePair(dMinDistOfPair, nPairLinesUrSide, nPairLinesOtherSide, MyLineEq, MyLine, UrLine, MinThick, MaxThick, LayerNum);
			if(nPairLinesUrSide%2==0 && nPairLinesOtherSide%2==0)
			{
				ASSERT(0);
				return FALSE;
			}

// 유어라인 쪽에 3개 이상의 홀수 평행선이 있어서 별도의 부재 생성이 가능한 경우로서
// 마이라인과 유어라인 사이에 더 가까운 평행선이 있으므로 스킾
			if(nPairLinesUrSide>=3 && nPairLinesUrSide%2==1 && nPairLinesOtherSide%2==0 && dMinDistOfPair<Dist)
				continue;
			if(nPairLinesUrSide == 2 && UrLine->Length()<dMinLength)
				continue;
			if(nPairLinesUrSide%2 == 0)
			{
				if(nPairLinesOtherSide%2 != 0)
					continue;
				if( (Dist-dMinDistOfPair) > DTOL_GM)
					continue;
 				if(MyLine->ma_Pair.GetSize()>0)
				{
					DG2DLine* PairLine = MyLine->ma_Pair[0];
					long PairSide = MyLine->WhichSide(PairLine->m_P0);
					long UrSide = MyLine->WhichSide(UrLine->m_P0);
					if(PairSide*UrSide < 0)
 						continue;
				}
			}

			BOOL MyLineFull = MyLine->AddPairLine(UrLine, MyLoc0, MyLoc1, Result, m_MAX_GAP_LENGTH / 4., m_DTOL_DIST_LINKTEST);
			//			BOOL MyLineFull = MyLine->AddPairLine(UrLine, MyLoc0, MyLoc1, Result, m_MAX_GAP_LENGTH);

			if(MyLineFull)
				return TRUE;
		}
	}
	return FALSE;
}

void CogReader::YouHaveNoFeasiblePair(double& dMinDistOfPair, long& nPairLinesUrSide, long& nPairLinesOtherSide, 
																					 GMLine& MyLineEq, 
																					 DG2DLine* RootLine, DG2DLine* MyLine, 
																					 double MinThick, double MaxThick, long LayerNum)
{
	double MyLoc0, MyLoc1;
	GM2DVector BaseVec0;
	double OverlapLen = 0.;
	DG2DLine* UrLine;
	long OldMyTag = MyLine->m_Tag;
	MyLine->m_Tag = -1;

	double MySide = MyLineEq.ComputeFxy(MyLine->m_P0.m_X, MyLine->m_P0.m_Y);

	CMap<DG2DLine*, DG2DLine*, DG2DLine*, DG2DLine*> ChkDGLineMap;
	CTypedPtrArray<CObArray, GMQuadTree*>& CrossQTs = MyLine->m_CrossQTs;
	long NumQT = CrossQTs.GetSize();
	for(long QTNum=0; QTNum<NumQT; QTNum++)
	{
		GMQuadTree* CurQT = CrossQTs[QTNum];
		CTypedPtrArray<CObArray, GM2DGeometry*>& Space = CurQT->ma_SearchSpace;
		CTypedPtrArray<CObArray, DG2DLine*> PairAltArr;
		CTypedPtrArray<CObArray, DG2DLine*> PairAltArrOther;

		long NumGM = Space.GetSize();
		for(long GMNum=0; GMNum<NumGM; GMNum++)
		{
			GM2DGeometry* AGeom = Space[GMNum];
			long OldUrTag = AGeom->m_Tag;
			if(AGeom == NULL || AGeom->m_Tag != -1 || AGeom->m_ObjType != GT_LINE)
				continue;
			// 평행성 검사
			AGeom->m_Tag = -1;
			UrLine = (DG2DLine*)AGeom;
			DG2DLine* rUrLine=NULL;
			if(ChkDGLineMap.Lookup(UrLine, rUrLine))
				continue;
			ChkDGLineMap.SetAt(UrLine, UrLine);

			if(UrLine->m_Linked0 && UrLine->m_Linked1 && UrLine->Length()<MaxThick 
				&& IsOverlap(UrLine->m_Linked0, UrLine->m_Linked1) 
				&& !MyLine->IsLink(UrLine->m_Linked0) && !MyLine->IsLink(UrLine->m_Linked1))
				continue;

			long Result = MyLine->IsParallel4DTol(*UrLine, m_DTOL_PARALLEL_DEGREE);
			if(Result == 0) 
				continue;
			// 주어진 폭(width) 대비 m_DTOL_PAIR_DIST_ERRRATIO 이하 오차인지 검사
			MyLoc0 = MyLine->OrthoBaseLocValue(UrLine->m_P0);
			BaseVec0 = MyLine->Point2DVector(MyLoc0);
			double Dist = BaseVec0.Distance(UrLine->m_P0);
			if(Dist>MaxThick || Dist<MinThick) 
				continue;
			// 서로 m_DTOL_MIN_WALL_LENGTH 이상 중첩되는지 검사
			MyLoc1 = MyLine->OrthoBaseLocValue(UrLine->m_P1);
			Result = OverlapGroup(MyLoc0, MyLoc1, m_DTOL_DIST_LINKTEST/10000.);
			// 1. 서로 중첩되지 않음
			if(Result==1 || Result==2 || Result==6 || Result==7 || Result==19 || Result==20 || Result==24 || Result==25)
				continue;

			if(IsOverlap(RootLine, UrLine) == FALSE)
				continue;

			double UrSide = MyLineEq.ComputeFxy(UrLine->m_P0.m_X, UrLine->m_P0.m_Y);
			double AltDist;
			if(MySide*UrSide > 0.)
			{
				long NumPairAlt = PairAltArr.GetSize();
				if(NumPairAlt==0)
				{
					AltDist = RootLine->Distance(UrLine->m_P0);
					if(dMinDistOfPair > AltDist)
						dMinDistOfPair = AltDist;
					PairAltArr.Add(UrLine);
				}
				else
				{
// 페어라인 후보자(MyLine)과 같은 방향으로 오버랩되는 라인들을 PairAltArr에 저장하는데
// 기존 alt 오버랩되는 라인은 서로 짝을 이룰 수 있으므로 추가 저장하고,
// 그렇지 않은 것은 짝이 될 가능성이 없으므로 스킾
					for(long AltNum=0; AltNum<NumPairAlt; AltNum++)
						if(PairAltArr[AltNum]->OverlapTest4DTol(*UrLine))
							break;
					if(AltNum<NumPairAlt)
					{
						AltDist = RootLine->Distance(UrLine->m_P0);
						if(dMinDistOfPair > AltDist)
							dMinDistOfPair = AltDist;
						PairAltArr.Add(UrLine);
					}
					if(AltNum==NumPairAlt)
						continue;
				}

				nPairLinesUrSide++;
				YouHaveNoFeasiblePair(dMinDistOfPair, nPairLinesUrSide, nPairLinesOtherSide, MyLineEq, RootLine, UrLine, MinThick, MaxThick, LayerNum);
			}
			else
			{
				long NumPairAlt = PairAltArrOther.GetSize();
				if(NumPairAlt==0)
				{
					AltDist = RootLine->Distance(UrLine->m_P0);
					if(dMinDistOfPair > AltDist)
						dMinDistOfPair = AltDist;
					PairAltArrOther.Add(UrLine);
				}
				else
				{
// 페어라인 후보자(MyLine)과 다른 방향으로 오버랩되는 라인들을 PairAltArr에 저장하는데
// 기존 alt 오버랩되는 라인은 서로 짝을 이룰 수 있으므로 추가 저장하고,
// 그렇지 않은 것은 짝이 될 가능성이 없으므로 스킾
					for(long AltNum=0; AltNum<NumPairAlt; AltNum++)
						if(PairAltArrOther[AltNum]->OverlapTest4DTol(*UrLine))
							break;
					if(AltNum<NumPairAlt)
					{
						AltDist = RootLine->Distance(UrLine->m_P0);
						if(dMinDistOfPair > AltDist)
							dMinDistOfPair = AltDist;
						PairAltArrOther.Add(UrLine);
					}
					if(AltNum==NumPairAlt)
						continue;
				}

				nPairLinesOtherSide++;
				YouHaveNoFeasiblePair(dMinDistOfPair, nPairLinesOtherSide, nPairLinesUrSide, MyLineEq, RootLine, UrLine, MinThick, MaxThick, LayerNum);
			}
			UrLine->m_Tag = OldUrTag;
		}
	}
	MyLine->m_Tag = OldMyTag;
	return;
}

*/
long CogReader::FindGappedLine(DG2DLine* MyLine, long nDGType)
{
	if(MyLine==NULL)	return -1;

	DG2DLine* UrLine = NULL;

	CMap<GM2DGeometry*, GM2DGeometry*, GM2DGeometry*, GM2DGeometry*> ChkLVecMap;
	vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(MyLine);
	long NumQT = CrossQTs.size();
	for(long QTNum=0; QTNum<NumQT; QTNum++)
	{
		GMQuadTree* CurQT = CrossQTs[QTNum];
		CTypedPtrArray<CObArray, GM2DGeometry*>& Space = CurQT->ma_SearchSpace;
		long NumGM = Space.GetSize();
		for(long GMNum=0; GMNum<NumGM; GMNum++)
		{
			GM2DGeometry* AGeom = Space[GMNum];
			if(AGeom == NULL || AGeom->m_ObjType != GT_LINE)
				continue;
			UrLine = (DG2DLine*)AGeom;
			if(MyLine->m_ID == UrLine->m_ID)
				continue;
			if(UrLine->m_Gapped0 && UrLine->m_Gapped1)  // 이미 양단 링크라인이 설정된 라인은 스킾
				continue;

			GM2DGeometry* rUrLine=NULL;
			if(ChkLVecMap.Lookup(UrLine, rUrLine))	continue;
			ChkLVecMap.SetAt(UrLine, UrLine);


			if(MyLine->m_Gapped0 == NULL)
				MyLine->SetGapLine(0, UrLine, m_DTOL_PARALLEL_DEGREE, m_DTOL_DIST_LINKTEST, m_MAX_GAPPED_STEP, m_MAX_GAP_LENGTH);
			if(MyLine->m_Gapped1 == NULL)
				MyLine->SetGapLine(1, UrLine, m_DTOL_PARALLEL_DEGREE, m_DTOL_DIST_LINKTEST, m_MAX_GAPPED_STEP, m_MAX_GAP_LENGTH);
		}
		if(MyLine->m_Gapped0 && MyLine->m_Gapped1)
			break;
	}

	return MyLine->GetGapState();
}


long CogReader::FindStaggeredLine(DG2DLine* MyLine, long nDGType)
{
	if(MyLine==NULL)	return -1;

	DG2DLine* UrLine = NULL;

	CMap<GM2DGeometry*, GM2DGeometry*, GM2DGeometry*, GM2DGeometry*> ChkLVecMap;
	vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(MyLine);
	long NumQT = CrossQTs.size();
	for(long QTNum=0; QTNum<NumQT; QTNum++)
	{
		GMQuadTree* CurQT = CrossQTs[QTNum];
		CTypedPtrArray<CObArray, GM2DGeometry*>& Space = CurQT->ma_SearchSpace;
		long NumGM = Space.GetSize();
		for(long GMNum=0; GMNum<NumGM; GMNum++)
		{
			GM2DGeometry* AGeom = Space[GMNum];
			if(AGeom == NULL || AGeom->m_ObjType != GT_LINE)
				continue;
			UrLine = (DG2DLine*)AGeom;
			if(MyLine->m_ID == UrLine->m_ID)
				continue;
			if(UrLine->m_Staggered0 && UrLine->m_Staggered1)  // 이미 양단 링크라인이 설정된 라인은 스킾
				continue;

			GM2DGeometry* rUrLine=NULL;
			if(ChkLVecMap.Lookup(UrLine, rUrLine))	continue;
			ChkLVecMap.SetAt(UrLine, UrLine);

			if(MyLine->m_Staggered0 == NULL)
				MyLine->SetStaggerLine(0, UrLine, m_DTOL_PARALLEL_DEGREE, m_MAX_WALL_ECC, m_DTOL_DIST_LINKTEST);
			if(MyLine->m_Staggered1 == NULL)
				MyLine->SetStaggerLine(1, UrLine, m_DTOL_PARALLEL_DEGREE, m_MAX_WALL_ECC, m_DTOL_DIST_LINKTEST);
		}
		if(MyLine->m_Staggered0 && MyLine->m_Staggered1)
			break;
	}

	return MyLine->GetStaggerState();
}


long CogReader::CutEachMembers(DG2DLine* MyLine, long nDGType)
{
	switch(nDGType)
	{
	case DG_RWALL:
	case DG_WALL:
		return CutEachWalls(MyLine, nDGType);
 	case DG_BEAM:
 		return CutEachBeams(MyLine);
 	case DG_WFOOT:
 		return CutEachWFoots(MyLine);
	case DG_LOWWALL:
		return CutEachLowWalls(MyLine);
 	}
	return 0;
}

long CogReader::PrecheckWallShape(DG2DLine* MyLine)
{
	if(MyLine==NULL)	return 0;

	long StartState = ES_NONE;
	long EndState = ES_NONE;

// 	CheckNeckHole(MyLine);
  long Result = CheckCrossPair(MyLine);
 
 	if(CheckMyZEnd(MyLine, TRUE))
 		StartState = ES_Z;
 	if(CheckMyZEnd(MyLine, FALSE))
 		EndState = ES_Z;
/*

	CheckNeckHole(MyLine);

	if(CheckBoxEnd(MyLine))
		StartState = EndState = ES_BOX;

	if(CheckMyLEnd(MyLine, TRUE))
		StartState = ES_L;
	if(CheckMyLEnd(MyLine, FALSE))
		EndState = ES_L;

	if(CheckMyZEnd(MyLine, TRUE))
		StartState = ES_Z;
	if(CheckMyZEnd(MyLine, FALSE))
		EndState = ES_Z;

	IsCrossTEndLine(MyLine);

	if(CheckLongitudinalTEnd(MyLine, TRUE))
		StartState = ES_T;
	if(CheckLongitudinalTEnd(MyLine, FALSE))
		EndState = ES_T;

	if(CheckCrossTEnd(MyLine, TRUE))
		StartState = ES_CROSST;
	if(CheckCrossTEnd(MyLine, FALSE))
		EndState = ES_CROSST;

	CheckWingWall(MyLine);
*/

	return 0;
}

long CogReader::CutEachWalls(DG2DLine* MyLine, long nDGType)
{
	if(MyLine==NULL)	return 0;
	long NumPairs = MyLine->ma_Pair.GetSize();
	if(NumPairs==0)	return 0;
	if(NumPairs==1 && MyLine->m_Tag)
		return 0;

	BOOL bWall = TRUE;
	if(nDGType==DG_WALL)	bWall = TRUE;
	else									bWall = FALSE;

	long NumDGWall = 0;
	BOOL bPairSameDir = FALSE;

	MyLine->m_Tag = 1;  

	if (GetRecognizeMode() != CogReader::Formwork)
	{
		MSFloor* pUpperFloor = this->mp_MSFloor->GetUpperFloor();
		if (pUpperFloor == NULL)	return 0;
	}

	double CurZc = mp_MSFloor->GetSlabLevel();
	double UpperZc = mp_MSFloor->GetSlabLevel();
	GM2DLineVector UrCenLVec, LinkCenLVec;
	BOOL UrResult, LinkResult;

	// Error Message
	CString GroupName = GetGroupName();
	CString strMsg, noNameMsg;

	MSWallMemberG* pPrevWallG = NULL;
	double PrevThick = 0;
	for(long PairNum=0; PairNum<NumPairs; PairNum++)
	{
		DG2DLine* UrLine = MyLine->ma_Pair[PairNum];

		if(UrLine->m_Tag)  // 필요한가?
			continue;

		vector<long> errorFigureIDs;
		AddVdFigureID(errorFigureIDs, FindGeometyToFigureIDs(MyLine));
		AddVdFigureID(errorFigureIDs, FindGeometyToFigureIDs(UrLine));

		GM2DVector SVec, EVec;
		bPairSameDir = MyLine->IsSameDirWithPair(PairNum);

		UrResult = MyLine->GetCenterLVec(UrCenLVec, PairNum);

		LinkResult = FALSE;
		// SVec, EVec : SNode, ENode 절점의 좌표 관리
		// UrVec : 실제 부재 끝 좌표 관리
		// SEcc, EEcc ; 실제 좌표와 절점 좌표간 편심 = UrVec - SVec
		if(PairNum==0)
		{
			if(MyLine->ma_PairLoc0[PairNum] > -DTOL_GM)
				SVec = UrCenLVec.OrthoBaseVector(MyLine->m_P0);
			else
			{
				if(bPairSameDir && MyLine==UrLine->ma_Pair[0])
					SVec = UrCenLVec.OrthoBaseVector(UrLine->m_P0);
				else if(!bPairSameDir && MyLine==UrLine->ma_Pair[UrLine->ma_Pair.GetSize()-1])
					SVec = UrCenLVec.OrthoBaseVector(UrLine->m_P1);
				else
					SVec = UrCenLVec.OrthoBaseVector(MyLine->m_P0);
			}
		}
		else
		{
			if(bPairSameDir)
				SVec = UrCenLVec.OrthoBaseVector(UrLine->m_P0);
			else
				SVec = UrCenLVec.OrthoBaseVector(UrLine->m_P1);
		}

		if(PairNum==NumPairs-1)
		{
			if(MyLine->ma_PairLoc1[PairNum] < 1.+DTOL_GM)
				EVec = UrCenLVec.OrthoBaseVector(MyLine->m_P1);
			else
			{
				if(bPairSameDir && MyLine==UrLine->ma_Pair[UrLine->ma_Pair.GetSize()-1])
					EVec = UrCenLVec.OrthoBaseVector(UrLine->m_P1);
				else if(!bPairSameDir && MyLine==UrLine->ma_Pair[0])
					EVec = UrCenLVec.OrthoBaseVector(UrLine->m_P0);
				else
					EVec = UrCenLVec.OrthoBaseVector(MyLine->m_P1);
			}
		}
		else
		{
			if(bPairSameDir)
				EVec = UrCenLVec.OrthoBaseVector(UrLine->m_P1);
			else
				EVec = UrCenLVec.OrthoBaseVector(UrLine->m_P0);
		}
		if(SVec.Distance(EVec) <= m_MIN_WALL_LENGTH)// MinLength보다 작은경우 error
		{
			strMsg = ERROR_MIN_LENGTH;
			AddErrorObject(errorFigureIDs, E_CODE_MIN_LENGTH, WARNING_LEVEL, GroupName, strMsg);
			continue;
		}

		double dT = MyLine->Distance(UrLine->m_P0); 
		dT = ROUND(dT);

		MSCompoM* pCompoM=NULL;
		MSCompoM* pLinearCompoM=NULL;
		GMText* pGMTxt = FindGMText(MyLine, MyLine->ma_Pair[PairNum], nDGType);
		CString strDefName;
		strDefName = MakeDefaultName(dT);
		
		bool IsNoNameError = false;
		if (GetRecognizeMode() == eRecognizeMode::Single)
		{
			if (pGMTxt != nullptr)
			{
				strDefName = MakeDefaultName(pGMTxt->m_strText);
				pCompoM = GetCompoM(strDefName, GetMSCompoMType(nDGType));
			}
			else
			{
				pCompoM = GetDefaultCompoM();
				if (pCompoM == nullptr && m_SEARCH_LINEAR_WALL_FOR_NAME) // 벽체 인식 시, 부재명 없는 부재의 경우 인접 리니어 벽체명 사용할지 디폴트로 만들지 세팅
					pCompoM = GetLinearWallCompoM(MyLine, MyLine->ma_Pair[PairNum], nDGType);
				if (pCompoM == NULL)
				{
					// Text를 못찾은 경우 warning
					noNameMsg.Format(_T("%s - %s"), strDefName, WARNING_NAME_NONE);
					IsNoNameError = true;
				}
				else
					strDefName = pCompoM->m_Name;
			}

			if (pCompoM == NULL)
			{
				if (pGMTxt != nullptr)			// Text는 있지만, 단면정보가 없는 경우
				{
					strMsg.Format(_T("%s - %s"), pGMTxt->m_strText, WARNING_SECTION_NONE);
					AddErrorObject(errorFigureIDs, W_CODE_SECTION_NONE, WARNING_LEVEL, GroupName, strMsg);
				}

				pCompoM = GetCompoM(strDefName, GetMSCompoMType(nDGType));
				if (pCompoM == NULL)
				{
					if (!m_IsSaveM)
					{
						if (pGMTxt)	strDefName += _T("(No Section)");
					}
					pCompoM = CreateNewWallM(dT, strDefName.MakeUpper(), bWall);
				}
			}
		}
		else if (GetRecognizeMode() == eRecognizeMode::History)
		{
			if(pGMTxt != nullptr)
				strDefName = MakeDefaultName(pGMTxt->m_strText);
			pCompoM = CreateNewWallM(dT, strDefName, bWall);
		}
		else if (GetRecognizeMode() == eRecognizeMode::Formwork)
		{
			pCompoM = CreateNewWallM(dT, strDefName, bWall);
		}
		else
			continue;

		if (pCompoM == nullptr)	continue;
		bool IsSetBack = false;
		if(fabs(pCompoM->GetThick() - dT) > 1.0)
		{
			long oldThick = (long)pCompoM->GetThick();
			if(m_SEARCH_LINEAR_WALL_FOR_NAME)	// 인접벽체명 사용체크시만 SetBack적용(REB-1505)
				IsSetBack = true;
		}

		if(pPrevWallG != NULL && IsSetBack == false)
		{
			if(pPrevWallG->mp_CompoM == pCompoM && fabs(PrevThick - dT) < DTOL1)
			{
				pPrevWallG->ReplaceVec(1, new GM2DVector(EVec.m_X, EVec.m_Y));
				continue;
			}
		}

		// jissi edited -? BWall / RWall
		MSWallMemberG* pWallG = NULL;
		pWallG = (MSWallMemberG*)m_pBldg->CreateNewCompoG(MSCompoG::msWallG);
		pWallG->ReplaceVec(0, new GM2DVector(SVec.m_X, SVec.m_Y));
		pWallG->ReplaceVec(1, new GM2DVector(EVec.m_X, EVec.m_Y));
		pWallG->mp_CompoM = pCompoM;
		pWallG->m_SOffsetZ = m_BotOffset;
		pWallG->m_SOffsetZ_End = m_BotOffset;
		pWallG->m_EOffsetZ = m_TopOffset;
		pWallG->m_EOffsetZ_End = m_TopOffset;
		pWallG->SetLineAlignType(LINEALIGN_CENTER);

		if (GetRecognizeMode() == eRecognizeMode::Single)
		{
			if (IsNoNameError)
			{
				vector<MSElement*> CompoGArr;
				CompoGArr.push_back(pWallG);
				AddErrorObject(CompoGArr, W_CODE_NAME_NONE, WARNING_LEVEL, GroupName, noNameMsg);
			}

			if (fabs(pCompoM->GetThick() - dT) > 1.0)
			{
				vector<MSElement*> compoGArr;	compoGArr.push_back(pWallG);
				long oldThick = (long)pCompoM->GetThick();
				if (m_SEARCH_LINEAR_WALL_FOR_NAME)	// 인접벽체명 사용체크시만 SetBack적용(REB-1505)
					pWallG->SetThickOfCompoG(dT);
				strMsg.Format(_T("%s - 단면정보(T:%ld)와 평면크기(T:%ld)가 다릅니다."), pCompoM->m_Name, oldThick, (long)dT);
				AddErrorObject(compoGArr, W_CODE_SIZE_DIFF, WARNING_LEVEL, GroupName, strMsg);
			}

			vector<long> FigureIDs;
			AddVdFigureID(MyLine, FigureIDs);
			AddVdFigureID(UrLine, FigureIDs);
			mm_CompoGtoFigureIDs.insert(make_pair(pWallG, FigureIDs));

		}
		if (pGMTxt)
			AddUsedTextByCompoG(pGMTxt, pWallG);

		pPrevWallG = pWallG;
		PrevThick = dT;

		ma_WallG.Add(pWallG);
		NumDGWall++;
	}
	return NumDGWall;
}

long CogReader::CutEachWallArcs(BOOL bWall)
{
	long NumWall = 0;
	double dMargin = 10;
	CTypedPtrArray<CObArray, DG2DArc*>* ArcArray = NULL;
	int Layer = DG_WALL;
	ArcArray = &ma_DGWallArcs;
	if( bWall == FALSE)
	{
		Layer = DG_RWALL;
		ArcArray = &ma_DGRWallArcs;
	}
	long NumArc = ArcArray->GetSize();
	CTypedPtrArray<CObArray, GMQuadTree*> CrossQTs;
	CrossQTs.Add(m_pQT);

	CString GroupName = GetGroupName();
	CString strMsg, noNameMsg;

	for(long iArc=0; iArc<NumArc; iArc++)
	{
		DG2DArc* MyArc = ArcArray->GetAt(iArc);
		if(MyArc->m_Tag == 1)	continue;
		double dRadiance = MyArc->m_pArc->m_A;
		MyArc->m_Tag = 1;
		double dSAngle = MyArc->m_pArc->m_Theta1;
		double dEAngle = MyArc->m_pArc->m_Theta2;
		GMText* GMTxt = FindGMArcText(MyArc, Layer);
		MSCompoM* pCompoM = NULL;
		CString strDefName;
		for(long jArc=iArc+1; jArc<NumArc; jArc++)
		{
			bool IsNoNameError = false;

			DG2DArc* UrArc = ArcArray->GetAt(jArc);
			if(UrArc->m_Tag)	continue;
			if(MyArc->m_pArc->m_Center.SameVector4DTOL(UrArc->m_pArc->m_Center, m_DTOL_DIST_COLINEAR)==FALSE)
				continue;

			double dPairRadiance = UrArc->m_pArc->m_A;
			double dDistance = fabs(dRadiance	-dPairRadiance);
			if(dDistance < m_MIN_WALL_THICK || dDistance > m_MAX_WALL_THICK)
				continue;

			long Result = MyArc->m_pArc->Overlap4DTol(*UrArc->m_pArc, m_DTOL_DEGREE_COLINEAR, m_MAX_WALL_THICK);
			if(Result==1 || Result==2 || Result==12 || Result==0)
				continue;

			vector<long> errorFigureIDs;
			AddVdFigureID(errorFigureIDs, FindGeometyToFigureIDs(MyArc));
			AddVdFigureID(errorFigureIDs, FindGeometyToFigureIDs(UrArc));

			double dIEccX = 0, dIEccY = 0;
			double dJEccX = 0, dJEccY = 0;
			double dT = dDistance;
			strDefName = MakeDefaultName(dT);
			if (GetRecognizeMode() == eRecognizeMode::Single)
			{
				if (GMTxt != nullptr)
				{
					strDefName = MakeDefaultName(GMTxt->m_strText);
					pCompoM = GetCompoM(strDefName, GetMSCompoMType(Layer));
				}
				else
				{
					// Text를 못찾은 경우 warning
					noNameMsg.Format(_T("%s - %s"), strDefName, WARNING_NAME_NONE);
					IsNoNameError = true;
				}

				if (pCompoM == nullptr)
				{
					if (GMTxt != nullptr)			// Text는 있지만, 단면정보가 없는 경우
					{
						strMsg.Format(_T("%s - %s"), GMTxt->m_strText, WARNING_SECTION_NONE);
						AddErrorObject(errorFigureIDs, W_CODE_SECTION_NONE, WARNING_LEVEL, GroupName, strMsg);
					}

					pCompoM = GetCompoM(strDefName, GetMSCompoMType(Layer));
					if (pCompoM == NULL)
					{
						if (!m_IsSaveM)
						{
							if (GMTxt)	strDefName += _T("(No Section)");
						}
						pCompoM = CreateNewWallM(dT, strDefName, bWall);
						// 					mp_MessageContainer->Insert(new XErrCreateSection(strDefName, MyArc, UrArc));
					}
				}
			}
			else if (GetRecognizeMode() == eRecognizeMode::History)
			{
				if (GMTxt != nullptr)
					strDefName = MakeDefaultName(GMTxt->m_strText);
				pCompoM = CreateNewWallM(dT, strDefName, bWall);
			}
			else if (GetRecognizeMode() == eRecognizeMode::Formwork)
			{
				pCompoM = CreateNewWallM(dT, strDefName, bWall);
			}
			else
				continue;

			if (pCompoM == nullptr)	continue;

			if(UrArc->m_pArc->m_Theta1 < dSAngle)
				dSAngle = UrArc->m_pArc->m_Theta1;
			if(UrArc->m_pArc->m_Theta2 > dEAngle)
				dEAngle = UrArc->m_pArc->m_Theta2;

			GMArc TheArc(MyArc->m_pArc->m_A-dDistance/2, MyArc->m_pArc->m_Center.m_X, MyArc->m_pArc->m_Center.m_Y, dSAngle, dEAngle) ;
			GM2DVector SVec = TheArc.GetStart2DVec();
			GM2DVector EVec = TheArc.GetEnd2DVec();

			MSWallMemberG* pWallG = (MSWallMemberG*)m_pBldg->CreateNewCompoG(MSCompoG::msWallG);
			pWallG->ReplaceVec(0, new GM2DVector(SVec.m_X, SVec.m_Y));
			pWallG->ReplaceVec(1, new GM2DVector(EVec.m_X, EVec.m_Y));
			pWallG->mp_CompoM = pCompoM;
			pWallG->m_SOffsetZ = m_BotOffset;
			pWallG->m_SOffsetZ_End = m_BotOffset;
			pWallG->m_EOffsetZ = m_TopOffset;
			pWallG->m_EOffsetZ_End = m_TopOffset;
			pWallG->SetLineAlignType(LINEALIGN_CENTER);
			ma_WallG.Add(pWallG);
			GM2DArc* pArc = new GM2DArc(TheArc.m_A, TheArc.m_Center.m_X, TheArc.m_Center.m_Y, dSAngle, dEAngle);
			pWallG->SetCenCurve(pArc);

			if (GetRecognizeMode() == eRecognizeMode::Single)
			{
				if (IsNoNameError)
				{
					vector<MSElement*> CompoGArr;
					CompoGArr.push_back(pWallG);
					AddErrorObject(CompoGArr, W_CODE_NAME_NONE, WARNING_LEVEL, GroupName, noNameMsg);
				}

				if (fabs(pCompoM->GetThick() - dT) > 1.0)
				{
					vector<MSElement*> compoGArr;	compoGArr.push_back(pWallG);
					long oldThick = (long)pCompoM->GetThick();
					strMsg.Format(_T("%s - 단면정보(T:%ld)와 평면크기(T:%ld)가 다릅니다."), pCompoM->m_Name, oldThick, (long)dT);
					AddErrorObject(compoGArr, W_CODE_SIZE_DIFF, WARNING_LEVEL, GroupName, strMsg);
				}

				vector<long> FigureIDs;
				AddVdFigureID(MyArc, FigureIDs);
				AddVdFigureID(UrArc, FigureIDs);
				mm_CompoGtoFigureIDs.insert(make_pair(pWallG, FigureIDs));
			}

			if(GMTxt)
				AddUsedTextByCompoG(GMTxt, pWallG);

			UrArc->m_Tag = 1;
			NumWall++;
		}
	}
	return NumWall;
}

long CogReader::MergeLineQTs(DG2DLine* MyLine, DG2DLine* UrLine, vector<GMQuadTree*>& CrossQTs)
{
	vector<GMQuadTree*> MyCrossQTs = m_pQT->FindCrossQts(MyLine);
	vector<GMQuadTree*> UrCrossQTs = m_pQT->FindCrossQts(UrLine);
	long NumMyQT = MyCrossQTs.size();
	long NumUrQT = UrCrossQTs.size();

	for(long iMyQT=0; iMyQT<NumMyQT; iMyQT++)
		CrossQTs.push_back(MyCrossQTs[iMyQT]);

	long NumCrossQT = CrossQTs.size();
	for(long iUrQT=0; iUrQT<NumUrQT; iUrQT++)
	{
		BOOL IsExist = FALSE;
		for(long iCrossQT=0; iCrossQT<NumCrossQT; iCrossQT++)
		{
			if(CrossQTs[iCrossQT] == UrCrossQTs[iUrQT])
			{
				IsExist = TRUE;
				break;
			}
		}
		if(IsExist==FALSE)
			CrossQTs.push_back(UrCrossQTs[iUrQT]);
	}

	return CrossQTs.size();
}

