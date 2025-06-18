#include "StdAfx.h"

#include "CogReader.h"

#include "DG2DLine.h"
#include "DG2DArc.h"

#include "GMLib\GM2DVector.h"
#include "GMLib\GM2DLineVector.h"
#include "GMLib\GMVector.h"
#include "GMLib\GMText.h"
#include "GMLib\GMQuadTree.h"
#include "GMLib\GMLine.h"

#include "StructureModel\MSGridLine.h"

#include <math.h>
#include <stdio.h>

void CogReader::ExtractGridEntities()
{
	m_pQT->ResetGMTag(DG_GRIDLINE);
	BOOL bOldOption = m_bMergedLineSegment;
	MergeLineSegments(ma_DGWallLines, DG_GRIDLINE);
	m_bMergedLineSegment = bOldOption;
	SortDGLines(ma_DGWallLines);
	SortPairLines(ma_DGWallLines);

	ResetTag(ma_DGWallLines);

	long ObjNum;
	long NumObj = ma_DGWallLines.GetSize();

	m_pQT->ResetSearchSpace();
	PutDGLinesIntoQT(ma_DGWallLines);

	ResetTag(ma_DGWallLines);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGWallLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindLinkedLine(AObj, DG_GRIDLINE);
	}

	ResetTag(ma_DGWallLines);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGWallLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindGappedLine(AObj, DG_GRIDLINE);
	}

	ResetTag(ma_DGWallLines);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGWallLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(AObj);
		AObj->m_Tag = 1;
		SearchParallelLine(AObj, m_MIN_WALL_THICK, m_MAX_WALL_THICK, CrossQTs, DG_WFOOT);
	}

	ResetTag(ma_DGWallLines);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGWallLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = PrecheckWallShape(AObj);
	}

	ResetTag(ma_DGWallLines);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGWallLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		MSGridEntity* Result = MakeAGridEntity(AObj);
		if(!Result)
			continue;
		MergeGridEntity(Result);
	}

	NumObj = ma_GridEntity.GetSize();
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		MSGridEntity* pGrid = ma_GridEntity[ObjNum];
		pGrid->mp_LVec->ExtendLineVector(TRUE, m_dGridLineExtendLength, FALSE);
		pGrid->mp_LVec->ExtendLineVector(FALSE, m_dGridLineExtendLength, TRUE);

		long nMyLine = pGrid->ma_MyLine.GetSize();
		for(long iMyLine=0; iMyLine<nMyLine; iMyLine++)
		{
			DG2DLine* pMyLine = pGrid->ma_MyLine[iMyLine];
			long nUrLine = pMyLine->ma_Pair.GetSize();
			DG2DLine* pUrLine = pMyLine->ma_Pair[0];
			BOOL bSameDir = pMyLine->IsSameDir(pUrLine);
			if(bSameDir)
			{
				if(pMyLine->m_Linked0 && pUrLine->m_Linked0 && pMyLine->m_Linked0 == pUrLine->m_Linked0)
					ma_GridEntity.Add(MakeEdgeGridEntity(pMyLine->m_Linked0));
			}
			else
			{
				if(pMyLine->m_Linked0 && pUrLine->m_Linked1 && pMyLine->m_Linked0 == pUrLine->m_Linked1)
					ma_GridEntity.Add(MakeEdgeGridEntity(pMyLine->m_Linked0));
			}
			pUrLine = pMyLine->ma_Pair[nUrLine-1];
			bSameDir = pMyLine->IsSameDir(pUrLine);
			if(bSameDir)
			{
				if(pMyLine->m_Linked1 && pUrLine->m_Linked1 && pMyLine->m_Linked1 == pUrLine->m_Linked1)
					ma_GridEntity.Add(MakeEdgeGridEntity(pMyLine->m_Linked1));
			}
			else
			{
				if(pMyLine->m_Linked1 && pUrLine->m_Linked0 && pMyLine->m_Linked1 == pUrLine->m_Linked0)
					ma_GridEntity.Add(MakeEdgeGridEntity(pMyLine->m_Linked1));
			}
		}
	}

	CTypedPtrArray<CObArray, MSGridEntity*> VertArr, HoriArr;
	NumObj = ma_GridEntity.GetSize();
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		MSGridEntity* pGrid = ma_GridEntity[ObjNum];
		GM2DLineVector* pLineVec = pGrid->mp_LVec;
		double dDegree = pLineVec->GetDegree();
		if(45 <= dDegree && dDegree <= 315)
			VertArr.Add(pGrid);
		else
			HoriArr.Add(pGrid);
	}

	int NumVert = VertArr.GetSize();
	for(int iGrid = 0; iGrid < NumVert; iGrid++)
	{
		for( int jGrid = iGrid+1; jGrid < NumVert; jGrid++)
		{
			GM2DLineVector* pILVec = VertArr[iGrid]->mp_LVec;
			GM2DLineVector* pJLVec = VertArr[jGrid]->mp_LVec;
			double dICenX = (pILVec->m_P0.m_X+pILVec->m_P1.m_X)/2;
			double dJCenX = (pJLVec->m_P0.m_X+pJLVec->m_P1.m_X)/2;
			if(dICenX > dJCenX)
			{
				MSGridEntity* pTemp = VertArr[iGrid];
				VertArr[iGrid] = VertArr[jGrid];
				VertArr[jGrid]= pTemp;
			}
		}
	}

	int NumHori = HoriArr.GetSize();
	for(int iGrid = 0; iGrid < NumHori; iGrid++)
	{
		for( int jGrid = iGrid+1; jGrid < NumHori; jGrid++)
		{
			GM2DLineVector* pILVec = HoriArr[iGrid]->mp_LVec;
			GM2DLineVector* pJLVec = HoriArr[jGrid]->mp_LVec;
			double dICenY = (pILVec->m_P0.m_Y+pILVec->m_P1.m_Y)/2;
			double dJCenY = (pJLVec->m_P0.m_Y+pJLVec->m_P1.m_Y)/2;
			if(dICenY > dJCenY)
			{
				MSGridEntity* pTemp = HoriArr[iGrid];
				HoriArr[iGrid] = HoriArr[jGrid];
				HoriArr[jGrid]= pTemp;
			}
		}
	}

	bool bHasBubble1 = m_GridLineOption.GetMarkerStart();
	bool bHasBubble2 = m_GridLineOption.GetMarkerEnd();

	for(ObjNum=0; ObjNum<NumVert; ObjNum++)
	{
		MSGridEntity* pGrid = VertArr[ObjNum];
		
		MSGridLine* pGridLine = new MSGridLine();
		pGridLine->m_Name = "";
		pGridLine->m_X1 = pGrid->mp_LVec->m_P0.m_X;
		pGridLine->m_Y1 = pGrid->mp_LVec->m_P0.m_Y;
		pGridLine->m_X2 = pGrid->mp_LVec->m_P1.m_X;
		pGridLine->m_Y2 = pGrid->mp_LVec->m_P1.m_Y;
		pGridLine->m_HasBubble1 = bHasBubble1;
		pGridLine->m_HasBubble2 = bHasBubble2;

		ma_GridLine.Add(pGridLine);
	}

	for(ObjNum=0; ObjNum<NumHori; ObjNum++)
	{
		MSGridEntity* pGrid = HoriArr[ObjNum];

		MSGridLine* pGridLine = new MSGridLine();
		pGridLine->m_Name = "";
		pGridLine->m_X1 = pGrid->mp_LVec->m_P0.m_X;
		pGridLine->m_Y1 = pGrid->mp_LVec->m_P0.m_Y;
		pGridLine->m_X2 = pGrid->mp_LVec->m_P1.m_X;
		pGridLine->m_Y2 = pGrid->mp_LVec->m_P1.m_Y;
		pGridLine->m_HasBubble1 = bHasBubble1;
		pGridLine->m_HasBubble2 = bHasBubble2;

		ma_GridLine.Add(pGridLine);
	}
}

MSGridEntity* CogReader::MakeEdgeGridEntity(DG2DLine* pLine)
{
	MSGridEntity* pNewEntity = new MSGridEntity;
	if(pLine->m_pLine->m_ObjType == GT_LINE)
	{
		GM2DLineVector& aLVec = *((GM2DLineVector*)(pLine->m_pLine));
		pNewEntity->mp_LVec =new GM2DLineVector(aLVec);
		return pNewEntity;
	}
	else
		return NULL;
}

BOOL CogReader::MergeGridEntity(MSGridEntity* pNewGrid)
{
	long nGrid = ma_GridEntity.GetSize();
	for(long iGrid=0; iGrid<nGrid; iGrid++)
	{
		MSGridEntity* pGrid = ma_GridEntity[iGrid];
		if(!pNewGrid->mp_LVec->IsParallel4DTol(*pGrid->mp_LVec))
			continue;
		long MyLineSide = pGrid->mp_LVec->WhichSide(pNewGrid->ma_MyLine[0]->m_P0);
		long UrLineSide = pGrid->mp_LVec->WhichSide(pNewGrid->ma_UrLine[0]->m_P0);
		if(MyLineSide*UrLineSide>0)
			continue;
		double LValue0, LValue1;
		LValue0 = pGrid->mp_LVec->OrthoBaseLocValue(pNewGrid->mp_LVec->m_P0);
		LValue1 = pGrid->mp_LVec->OrthoBaseLocValue(pNewGrid->mp_LVec->m_P1);
		if(LValue0 < LValue1)
		{
			if(LValue0 < DTOL_GM)
				pGrid->mp_LVec->m_P0 = pGrid->mp_LVec->OrthoBaseVector(pNewGrid->mp_LVec->m_P0);
			if(LValue1 > 1.-DTOL_GM)
				pGrid->mp_LVec->m_P1 = pGrid->mp_LVec->OrthoBaseVector(pNewGrid->mp_LVec->m_P1);
		}
		else
		{
			if(LValue0 < DTOL_GM)
				pGrid->mp_LVec->m_P0 = pGrid->mp_LVec->OrthoBaseVector(pNewGrid->mp_LVec->m_P1);
			if(LValue1 > 1.-DTOL_GM)
				pGrid->mp_LVec->m_P1 = pGrid->mp_LVec->OrthoBaseVector(pNewGrid->mp_LVec->m_P0);
		}
		pGrid->ma_MyLine.Add(pNewGrid->ma_MyLine[0]);
		delete pNewGrid;
		return TRUE;
	}
	ma_GridEntity.Add(pNewGrid);
	return FALSE;
}

MSGridEntity* CogReader::MakeAGridEntity(DG2DLine* MyLine)
{
	if(MyLine==NULL)	return 0;
	long NumPairs = MyLine->ma_Pair.GetSize();
	if(NumPairs==0)	return 0;
	if(NumPairs==1 && MyLine->m_Tag)
		return 0;

	// 	double dMargin = 10;
	long nGridEntity = 0;
	BOOL bPairSameDir = FALSE;

	GM2DLineVector* GridLVec = new GM2DLineVector;

	MyLine->m_Tag = 1;  

	double MinDist = 1.E30;
	long iMinDistLine = 0;
	MSGridEntity* pNewGrid;
	DG2DLine* UrLine;
	pNewGrid = new MSGridEntity;
	pNewGrid->ma_MyLine.Add(MyLine);
	for(long PairNum=0; PairNum<NumPairs; PairNum++)
	{
		UrLine = MyLine->ma_Pair[PairNum];
		double Dist = MyLine->Distance(UrLine->m_P0);
		if(Dist < MinDist)
		{
			MinDist = Dist;
			iMinDistLine = PairNum;
		}
		UrLine->m_Tag = 1;
	}
	pNewGrid->ma_UrLine.Add(MyLine->ma_Pair[iMinDistLine]);
	MyLine->GetCenterLVec(*GridLVec, iMinDistLine);
	pNewGrid->mp_LVec = GridLVec;
	return pNewGrid;
}

