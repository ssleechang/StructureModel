 #include "stdafx.h"
#include "CogReader.h"

#include "GMLib/GMObject.h"
#include "GMLib/GM2DLineVector.h"
#include "GMLib/GMQuadTree.h"
#include "GMLib/GMText.h"
#include "GMLib/GM2DArc.h"
#include "GMLib/GM2DBound.h"

#include "StructureModel\MSCompoM.h"
#include "StructureModel\MSRebarBuilding.h"
#include "StructureModel\MSWallMemberG.h"
#include "StructureModel\MSCompoM.h"
#include "StructureModel\MSSlabG.h"
#include "StructureModel\MSDeformedBar.h"
#include "StructureModel\MSSlabRebarInfo.h"
#include "StructureModel\MSPolylineObject.h"
#include "StructureModel\MSStirrupBar.h"
#include "StructureModel\MSPcLineMemberM.h"
#include "StructureModel\MTReader.h"
#include "StructureModel\MSHandRailG.h"
#include "StructureModel\MSSteelBeamM.h"
#include "StructureModel\MSCompoGUtil.h"
#include "StructureModel/MSRcFWBeamM.h"

#include "CogReaderSlabRebarHelper.h"

#include "DG2DLine.h"
#include "DG2DArc.h"


#include <math.h>

void CogReader::ExtractIsoFoots()
{

	m_pQT->ResetGMTag(DG_ISOFOOT);
	MergeGMLineSegments(DG_ISOFOOT);
	m_pQT->ResetGMTag(DG_ISOFOOT);

	CTypedPtrArray<CObArray, GM2DGeometry*>& FootGMs = m_pQT->ma_GMs[DG_ISOFOOT];

	double dMargin = 10;
	CTypedPtrArray<CObArray, GMQuadTree*> RootQT;
	RootQT.Add(m_pQT);

	long NumObj = FootGMs.GetSize();
	for(long ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		GM2DGeometry* AObj = FootGMs[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;

		if(AObj->m_ObjType == GT_LINE)
		{
			GM2DLineVector* ALineObj = (GM2DLineVector*)AObj;

			if(SearchPolygonInIsoFootingLayer(ALineObj)==TRUE)
				AObj->m_Tag = 1;
		}
	}
}

BOOL CogReader::SearchPolygonInIsoFootingLayer(GM2DLineVector* Geo)
{
	vector<long> VdFigureIDArr;

	GM2DPolyline ThePLine(TRUE);
	CTypedPtrArray<CObArray, GM2DLineVector*> TheSelLines;
	CTypedPtrArray<CObArray, GMQuadTree*> RootQT;
	RootQT.Add(m_pQT);

	ThePLine.Add(new GM2DVector(Geo->m_P0));
	ThePLine.Add(new GM2DVector(Geo->m_P1));
	TheSelLines.Add(Geo);

	vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(Geo);
	Geo->m_Tag = 1;

	BOOL bPolygon = SearchLinkedLine(TheSelLines, ThePLine, CrossQTs, DG_ISOFOOT, false);
	if(ThePLine.GetSize() == 2)
	{
// 		XShopErrorObject* pErrObj = CreateErrorObject(50001, Geo, TRUE);
		return FALSE;		
	}

	if(bPolygon == FALSE)
	{
		long RtnVal = CompleteGMPolygon(ThePLine);
		if(RtnVal == 0)
		{
// 			CreatePolylineErrorObject(50002, TheSelLines);
			return FALSE;
		}
	}

	GMPolygon ThePGon(TRUE);
	ThePGon = ThePLine;
	long SecShape = 1;
	if(ThePGon.IsRectangle() == FALSE)
	{
// 		CreatePolylineErrorObject(50003, TheSelLines);
		return FALSE;
	}

	GMVector Cen3DVec = ThePGon.MassCenter();
	GM2DVector Cen2DVec(Cen3DVec.m_X, Cen3DVec.m_Y);
	double dB = ThePGon.GetLine(0).Length();
	double dH = ThePGon.GetLine(1).Length();

	if (dB < m_MIN_WFOOT_WIDTH || dB > m_MAX_WFOOT_WIDTH)	return FALSE;
	if(dH <  m_MIN_WFOOT_WIDTH || dH > m_MAX_WFOOT_WIDTH)	return FALSE;

	ThePGon.Translate(Cen3DVec);
	MSCompoM* pCompoM=NULL;

	GM2DPolyline* pOffsetPolyline = MakeOffsetPolyline(&ThePLine, m_DTOL_TEXT_DIST, TRUE);
	GMText* pGMTxt = GetGMText_ContainPolyline(pOffsetPolyline, DG_ISOFOOT, m_pQT->FindCrossQts(Geo));
	delete pOffsetPolyline;

//	GMText* pGMTxt = GetNearestGMText(Geo, DG_ISOFOOT, Cen2DVec);
	bool IsNoNameError = false;
	CString strDefName, noNameMsg, strMsg;
	strDefName = MakeDefaultName((long)dB, (long)dH, m_defaultDepth);
	if (GetRecognizeMode() == eRecognizeMode::Single)
	{
		if (pGMTxt != nullptr)
		{
			strDefName = MakeDefaultName(pGMTxt->m_strText);
			pCompoM = GetCompoM(strDefName, GetMSCompoMType(DG_ISOFOOT));
		}
		else
		{
			// Text를 못찾은 경우 warning
			noNameMsg.Format(_T("%s - %s"), strDefName, WARNING_NAME_NONE);
			IsNoNameError = true;
		}

		if (pCompoM == NULL)
		{
			if (pGMTxt != nullptr)
			{
				strMsg.Format(_T("%s - %s"), pGMTxt->m_strText, WARNING_SECTION_NONE);
				AddErrorObject(VdFigureIDArr, W_CODE_SECTION_NONE, WARNING_LEVEL, GetGroupName(), strMsg);
			}

			pCompoM = GetCompoM(strDefName, MSCompoM::msIsoFootM);
			if (pCompoM == NULL)
			{
				if (!m_IsSaveM)
				{
					if (pGMTxt)	strDefName += _T("(No Section)");
				}
				pCompoM = CreateNewIsoFootM(dB, dH, SecShape, strDefName);
			}
		}
	}
	else if (GetRecognizeMode() == eRecognizeMode::History)
	{
		if (pGMTxt != nullptr)
			strDefName = MakeDefaultName(pGMTxt->m_strText);
		pCompoM = CreateNewIsoFootM(dB, dH, SecShape, strDefName);

	}
	else
		return FALSE;

	if (pCompoM == nullptr)	return FALSE;
	double dRotation = 0.;
	BOOL bDifSection = FALSE;
	double dWidth = pCompoM->GetWidth();
	double dDepth = pCompoM->GetDepth();
	if(pCompoM)	// 기초는 무조건 RECT
	{
		GM2DLineVector FirstLine = ThePLine.GetLine(0);
		GM2DLineVector SecondLine = ThePLine.GetLine(1);
		double dFirstLength = FirstLine.Length();
		double dSecondLength = SecondLine.Length();

		if(fabs(dWidth - dFirstLength) < 1.)
		{
			dRotation = FirstLine.GetDegree();
			if(fabs(dDepth - dSecondLength) > 1.)
				bDifSection = TRUE;
		}
		else if(fabs(dWidth - dSecondLength) < 1.)
		{
			dRotation = SecondLine.GetDegree();
			if(fabs(dDepth - dFirstLength) > 1.)
				bDifSection = TRUE;
		}
		else if(fabs(dDepth - dFirstLength) < 1.)
		{
			dRotation = SecondLine.GetDegree();
			if(fabs(dWidth - dSecondLength) > 1.)
				bDifSection = TRUE;
		}
		else if(fabs(dDepth - dSecondLength) < 1.)
		{
			dRotation = FirstLine.GetDegree();
			if(fabs(dWidth - dFirstLength) > 1.)
				bDifSection = TRUE;
		}
		else
		{
			dRotation = FirstLine.GetDegree();
			if (dFirstLength < dSecondLength)	dRotation = SecondLine.GetDegree();
			if (dDepth > dWidth)	dRotation += 90;
			bDifSection = TRUE;
		}
		if (GetRecognizeMode() == History)
		{
			double dFirstLineDegree = FirstLine.GetDegree();
			double dSecondLineDegree = SecondLine.GetDegree();

			dRotation = min(fabs(dFirstLineDegree), fabs(dSecondLineDegree));
			if (ThePGon.GetSize() == 4)
			{
				double thirdDegree = ThePGon.GetLine(2).GetDegree();
				double fourthDegree = ThePGon.GetLine(3).GetDegree();
				dRotation = min(fabs(thirdDegree), dRotation);
				dRotation = min(fabs(fourthDegree), dRotation);
			}
		}
	}

	MSIsoFootG* pIsoFootG = (MSIsoFootG*)m_pBldg->CreateNewCompoG(MSCompoG::msIsoFootG);
	pIsoFootG->ReplaceVec(0, new GM2DVector(Cen3DVec.m_X, Cen3DVec.m_Y));
	pIsoFootG->mp_CompoM = pCompoM;
// 	if(dRotation >= 180)
// 		dRotation -= 180;
	if(dRotation > 90 && dRotation <= 270)
		dRotation -= 180;
	pIsoFootG->m_dAxisRotation = dRotation;
	pIsoFootG->SetPointAlignType(COLUMNALIGN_CENTER);
	pIsoFootG->m_SOffsetZ = m_BotOffset;
	ma_IsoFootG.Add(pIsoFootG);

	if (GetRecognizeMode() == eRecognizeMode::Single)
	{
		if (bDifSection)
		{
			vector<MSElement*> compoGArr;	compoGArr.push_back(pIsoFootG);
			strMsg.Format(_T("%s - 단면정보(%ldx%ld)와 평면크기(%ldx%ld)가 다릅니다."), pCompoM->m_Name, (long)dWidth, (long)dDepth, (long)dB, (long)dH);
			AddErrorObject(compoGArr, W_CODE_SIZE_DIFF, WARNING_LEVEL, GetGroupName(), strMsg);
		}
		if (IsNoNameError)
		{
			vector<MSElement*> CompoGArr;
			CompoGArr.push_back(pIsoFootG);
			AddErrorObject(CompoGArr, W_CODE_NAME_NONE, WARNING_LEVEL, GetGroupName(), noNameMsg);
		}
	}

	// 독립기초의 모양을 만들때 FigrueID들을 모아두어야 한다. -  by gracias
	vector<long> FigureIDs;
	mm_CompoGtoFigureIDs.insert(make_pair(pIsoFootG, FigureIDs));

	if(pGMTxt)
		AddUsedTextByCompoG(pGMTxt, pIsoFootG);

	return TRUE;
}

void CogReader::ExtractWallFoots()
{
	m_pQT->ResetGMTag(DG_WFOOT);

	MergeLineSegments(ma_DGWFootLines, DG_WFOOT);
	MergeArcSegments(ma_DGWFootArcs, DG_WFOOT_ARC);

	SortDGLines(ma_DGWFootLines);
	SortDGArcs(ma_DGWFootArcs);

	SortPairLines(ma_DGWFootLines);
	long NumLine = ma_DGWFootLines.GetSize();
	ResetTag(ma_DGWFootLines);
	ResetTag(ma_DGWFootArcs);
	m_pQT->ResetSearchSpace();
	PutDGLinesIntoQT(ma_DGWFootLines);
	PutDGArcsIntoQT(ma_DGWFootArcs);

	long NumObj = ma_DGWFootLines.GetSize();
	for(long ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGWFootLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(AObj);
		AObj->m_Tag = 1;
		SearchParallelLine(AObj, m_MIN_WFOOT_WIDTH, m_MAX_WFOOT_WIDTH, CrossQTs, DG_WFOOT);
	}

	ResetTag(ma_DGWFootLines);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGWFootLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindLinkedLine(AObj, DG_WFOOT);
	}

	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGWFootLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindGappedLine(AObj, DG_WFOOT);
	}

	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGWFootLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindStaggeredLine(AObj, DG_WFOOT);
	}

	ResetTag(ma_DGWFootLines);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGWFootLines[ObjNum];
		if(AObj == NULL)  continue;
		long NumMembers = CutEachMembers(AObj, DG_WFOOT);
	}

 	CutEachWFootArcs();
}

void CogReader::ExtractHandRails()
{
	m_pQT->ResetGMTag(DG_LOWWALL);

	CTypedPtrArray<CObArray, DG2DLine*> HandRailLines;
	CTypedPtrArray<CObArray, DG2DArc*> HandRailArcs;
	MergeLineSegments(HandRailLines, DG_LOWWALL);
	MergeArcSegments(HandRailArcs, DG_LOWWALL_ARC);

	SortDGLines(HandRailLines);
	SortDGArcs(HandRailArcs);

	SortPairLines(HandRailLines);
	long NumLine = HandRailLines.GetSize();
	ResetTag(HandRailLines);
	ResetTag(HandRailArcs);
	m_pQT->ResetSearchSpace();
	PutDGLinesIntoQT(HandRailLines);
	PutDGArcsIntoQT(HandRailArcs);

	long NumObj = HandRailLines.GetSize();
	for (long ObjNum = 0; ObjNum < NumObj; ObjNum++)
	{
		DG2DLine* AObj = HandRailLines[ObjNum];
		if (AObj == NULL || AObj->m_Tag != 0)  continue;
		vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(AObj);
		AObj->m_Tag = 1;
		// LowWall의 Min, Max값 추가해야 한다.
		SearchParallelLine(AObj, m_MIN_WFOOT_WIDTH, m_MAX_WFOOT_WIDTH, CrossQTs, DG_LOWWALL);
	}

	ResetTag(HandRailLines);
	for (ObjNum = 0; ObjNum < NumObj; ObjNum++)
	{
		DG2DLine* AObj = HandRailLines[ObjNum];
		if (AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindLinkedLine(AObj, DG_LOWWALL);
	}

	for (ObjNum = 0; ObjNum < NumObj; ObjNum++)
	{
		DG2DLine* AObj = HandRailLines[ObjNum];
		if (AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindGappedLine(AObj, DG_LOWWALL);
	}

	for (ObjNum = 0; ObjNum < NumObj; ObjNum++)
	{
		DG2DLine* AObj = HandRailLines[ObjNum];
		if (AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindStaggeredLine(AObj, DG_LOWWALL);
	}

	ResetTag(HandRailLines);
	for (ObjNum = 0; ObjNum < NumObj; ObjNum++)
	{
		DG2DLine* AObj = HandRailLines[ObjNum];
		if (AObj == NULL)  continue;
		long NumMembers = CutEachMembers(AObj, DG_LOWWALL);
	}

//	CutEachWFootArcs();
}

long CogReader::CutEachWFootArcs()
{
	long NumWallFoot = 0;
	double dMargin = 10;
	long NumArc = ma_DGWFootArcs.GetSize();

	CString GroupName = GetGroupName();
	CString strMsg, noNameMsg;

	CTypedPtrArray<CObArray, GMQuadTree*> CrossQTs;
	CrossQTs.Add(m_pQT);
	for(long iArc=0; iArc<NumArc; iArc++)
	{
		DG2DArc* MyArc = ma_DGWFootArcs[iArc];
		double dRadiance = MyArc->m_pArc->m_A;
		MyArc->m_Tag = 1;
		double dSAngle = MyArc->m_pArc->m_Theta1;
		double dEAngle = MyArc->m_pArc->m_Theta2;
		GMText* GMTxt = FindGMArcText(MyArc, DG_WFOOT);
		MSCompoM* pCompoM = NULL;
		CString strDefName;

		for(long jArc=iArc+1; jArc<NumArc; jArc++)
		{
			bool IsNoNameError = false;

			DG2DArc* UrArc = ma_DGWFootArcs[jArc];
			if(UrArc->m_Tag)	continue;
			if(MyArc->m_pArc->m_Center.SameVector4DTOL(UrArc->m_pArc->m_Center, m_DTOL_DIST_COLINEAR)==FALSE)	continue;

			double dPairRadiance = UrArc->m_pArc->m_A;
			double dDistance = fabs(dRadiance	-dPairRadiance);
			if(dDistance < m_MIN_WFOOT_WIDTH || dDistance > m_MAX_WFOOT_WIDTH)	continue;

			long Result = MyArc->m_pArc->Overlap4DTol(*UrArc->m_pArc, m_DTOL_DEGREE_COLINEAR, m_MAX_WFOOT_WIDTH);
			if(Result==1 || Result==2 || Result==12 || Result==0)	continue;

			vector<long> errorFigureIDs;
			AddVdFigureID(errorFigureIDs, FindGeometyToFigureIDs(MyArc));
			AddVdFigureID(errorFigureIDs, FindGeometyToFigureIDs(UrArc));
			double dB = dDistance;
			double dH = 700;

			strDefName = MakeDefaultName(dB, m_defaultDepth);
			if (GetRecognizeMode() == eRecognizeMode::Single)
			{
				if (GMTxt != nullptr)
				{
					strDefName = MakeDefaultName(GMTxt->m_strText);
					pCompoM = GetCompoM(strDefName, GetMSCompoMType(DG_WFOOT));
				}
				else
				{
					// Text를 못찾은 경우 warning
					noNameMsg.Format(_T("%s - %s"), strDefName, WARNING_NAME_NONE);
					IsNoNameError = true;
				}

				if (pCompoM == NULL)
				{
					if (GMTxt != nullptr)			// Text는 있지만, 단면정보가 없는 경우
					{
						strMsg.Format(_T("%s - %s"), GMTxt->m_strText, WARNING_SECTION_NONE);
						AddErrorObject(errorFigureIDs, W_CODE_SECTION_NONE, WARNING_LEVEL, GroupName, strMsg);
					}

					pCompoM = GetCompoM(strDefName, MSCompoM::msWallFootM);
					if (pCompoM == NULL)
					{
						if (!m_IsSaveM)
						{
							if (GMTxt)	strDefName += _T("(No Section)");
						}
						pCompoM = CreateNewWallFootM(dB, strDefName);
					}
				}
			}
			else if (GetRecognizeMode() == eRecognizeMode::History)
			{
				if (GMTxt != nullptr)
					strDefName = MakeDefaultName(GMTxt->m_strText);
				pCompoM = CreateNewWallFootM(dB, strDefName);
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

			MSWallFootG* pWallFootG = (MSWallFootG*)m_pBldg->CreateNewCompoG(MSCompoG::msWallFootG);
			pWallFootG->ReplaceVec(0, new GM2DVector(SVec.m_X, SVec.m_Y));
			pWallFootG->ReplaceVec(1, new GM2DVector(EVec.m_X, EVec.m_Y));
			pWallFootG->mp_CompoM = pCompoM;
			pWallFootG->SetLineAlignType(LINEALIGN_CENTER);
			ma_WallFootG.Add(pWallFootG);
			GM2DArc* pArc = new GM2DArc(TheArc.m_A, TheArc.m_Center.m_X, TheArc.m_Center.m_Y, dSAngle, dEAngle);
			pWallFootG->m_SOffsetZ = m_BotOffset;
			pWallFootG->m_EOffsetZ = m_BotOffset;
			pWallFootG->SetCenCurve(pArc);

			if (GetRecognizeMode() == eRecognizeMode::Single)
			{
				if (IsNoNameError)
				{
					vector<MSElement*> CompoGArr;
					CompoGArr.push_back(pWallFootG);
					AddErrorObject(CompoGArr, W_CODE_NAME_NONE, WARNING_LEVEL, GroupName, noNameMsg);
				}
				if (fabs(pCompoM->GetWidth() - dB) > 1.0)
				{
					vector<MSElement*> compoGArr;	compoGArr.push_back(pWallFootG);
					long oldWidth = (long)pCompoM->GetWidth();
					strMsg.Format(_T("%s - 단면정보(W:%ld)와 평면크기(W:%ld)가 다릅니다."), pCompoM->m_Name, (long)oldWidth, (long)dB);
					AddErrorObject(compoGArr, W_CODE_SIZE_DIFF, WARNING_LEVEL, GroupName, strMsg);
				}

				vector<long> FigureIDs;
				AddVdFigureID(MyArc, FigureIDs);
				AddVdFigureID(UrArc, FigureIDs);
				mm_CompoGtoFigureIDs.insert(make_pair(pWallFootG, FigureIDs));
			}
			if(GMTxt)
				AddUsedTextByCompoG(GMTxt, pWallFootG);

			NumWallFoot++;
		}
	}
	return NumWallFoot;
}

long CogReader::CutEachWFoots(DG2DLine* MyLine)
{
	if(MyLine==NULL)	return 0;
	long NumPairs = MyLine->ma_Pair.GetSize();
	if(NumPairs==0)	return 0;

	//100714	if(NumPairs==1 && MyLine->m_Tag && MyLine->m_bSingleLine==FALSE)
	if(NumPairs==1 && MyLine->m_Tag)
		return 0;

// 	MSFloor* pLowFloor = mp_MSFloor->GetLowerFloor();
// 	if(pLowFloor==NULL)
// 		return 0;
	double dLength = MyLine->Length();

	double dMargin = 10;
	long NumDGWFoot = 0;
	BOOL bPairSameDir = FALSE;
	BOOL bGapSameDir = FALSE;
	MSWallFootG* pWFoot = NULL;
	BOOL bMakeUrLine = FALSE;
	MyLine->m_Tag = 1;
	BOOL bSNodeFlag, bENodeFlag;
	bSNodeFlag = bENodeFlag = TRUE;

	double dH = 1000;

	// Error Message
	CString GroupName = GetGroupName();
	CString strMsg;

	BOOL MyResult = FALSE, LinkResult=FALSE, UrResult=FALSE;
	GM2DLineVector MainLVec, UrCenLVec, LinkCenLVec;

	BOOL bFirstLine = TRUE;
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

		if (SVec.Distance(EVec) <= m_MIN_BEAM_LENGTH)
		{
			strMsg = ERROR_MIN_LENGTH;
			AddErrorObject(errorFigureIDs, E_CODE_MIN_LENGTH, WARNING_LEVEL, GroupName, strMsg);
			continue;
		}

		double dB = MyLine->Distance(UrLine->m_P0); ;
		MSCompoM* pCompoM=NULL;
		GMText* pGMTxt = FindGMText(MyLine, UrLine, DG_WFOOT);
		CString strDefName;
		strDefName = MakeDefaultName(dB, m_defaultDepth);
		if (GetRecognizeMode() == eRecognizeMode::Single)
		{
			pCompoM = GetDefaultCompoM();
			if (pCompoM == nullptr)
			{
				if (pGMTxt != nullptr)
				{
					strDefName = MakeDefaultName(pGMTxt->m_strText);
					pCompoM = GetCompoM(strDefName, GetMSCompoMType(DG_WFOOT));
				}

				if (pCompoM == nullptr)
				{
					if (pGMTxt != nullptr)			// Text는 있지만, 단면정보가 없는 경우
					{
						strMsg.Format(_T("%s - %s"), pGMTxt->m_strText, WARNING_SECTION_NONE);
						AddErrorObject(errorFigureIDs, W_CODE_SECTION_NONE, WARNING_LEVEL, GroupName, strMsg);
					}

					pCompoM = GetCompoM(strDefName, MSCompoM::msWallFootM);
					if (pCompoM == NULL)
					{
					if (!m_IsSaveM)
					{
						if (pGMTxt)	strDefName += _T("(No Section)");
					}
						pCompoM = CreateNewWallFootM(dB, strDefName);
					}
				}
			}
		}
		else if (GetRecognizeMode() == eRecognizeMode::History)
		{
			if (pGMTxt != nullptr)
				strDefName = MakeDefaultName(pGMTxt->m_strText);
			pCompoM = CreateNewWallFootM(dB, strDefName);
		}
		else
			continue;

		if (pCompoM == nullptr)	continue;

		MSWallFootG* pWallFootG = (MSWallFootG*)m_pBldg->CreateNewCompoG(MSCompoG::msWallFootG);
		pWallFootG->ReplaceVec(0, new GM2DVector(SVec.m_X, SVec.m_Y));
		pWallFootG->ReplaceVec(1, new GM2DVector(EVec.m_X, EVec.m_Y));
		pWallFootG->mp_CompoM = pCompoM;
		pWallFootG->SetLineAlignType(LINEALIGN_CENTER);
		pWallFootG->m_SOffsetZ = m_BotOffset;
		pWallFootG->m_EOffsetZ = m_BotOffset;
		ma_WallFootG.Add(pWallFootG);

		if (GetRecognizeMode() == eRecognizeMode::Single)
		{
			if (fabs(pCompoM->GetWidth() - dB) > 1.0)
			{
				vector<MSElement*> compoGArr;	compoGArr.push_back(pWallFootG);
				long oldWidth = (long)pCompoM->GetWidth();
				strMsg.Format(_T("%s - 단면정보(W:%ld)와 평면크기(W:%ld)가 다릅니다."), pCompoM->m_Name, oldWidth, (long)dB);
				AddErrorObject(compoGArr, W_CODE_SIZE_DIFF, WARNING_LEVEL, GroupName, strMsg);
			}

			vector<long> FigureIDs;
			AddVdFigureID(MyLine, FigureIDs);
			AddVdFigureID(UrLine, FigureIDs);
			mm_CompoGtoFigureIDs.insert(make_pair(pWallFootG, FigureIDs));

		}
		if (pGMTxt)
			AddUsedTextByCompoG(pGMTxt, pWallFootG);

		NumDGWFoot++;
	}

	return NumDGWFoot;
}

void CogReader::ExtractGirders()
{
	if (GetRecognizeMode() == CogReader::Formwork)
		ExtractRcGirders();
	else
	{
		if (m_MaterialType == MSMaterial::msConcrete)
			ExtractRcGirders();
		else if (m_MaterialType == MSMaterial::msSteel)
			ExtractSteelGirders();
		else if (m_MaterialType == MSMaterial::msSRC)
			ExtractSrcGirders();
	}
}

void CogReader::MakeSpliteGirders()
{
	MSFloor* pFloor = mp_MSFloor->GetLowerFloor();
	if (pFloor == NULL)
		return;

	CTypedPtrArray<CObArray, MSBeamG*> SpliteBeamGArr;
	for (long iBeam = ma_BeamG.GetSize() -1 ; iBeam >= 0 ; iBeam--)
	{
		MSBeamG* pBeam = (MSBeamG*)ma_BeamG[iBeam];
		pBeam->SetOwner(pFloor);
		vector<MSCompoG*> BeamArr = pBeam->MakeSplitLinkMemberByColumn();
		if (BeamArr.size() > 1)
		{
			for (long i = 0; i < BeamArr.size(); i++)
			{
				MSBeamG* pSpliteBeam = (MSBeamG*)BeamArr[i];
				//if (pSpliteBeam != pBeam)
				SpliteBeamGArr.Add(pSpliteBeam);
			}
			ma_BeamG.RemoveAt(iBeam);
		}
	}

	GMText* pBaseGMText = NULL;
	MSCompoM* pBaseCompoM = NULL;
	CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = m_pQT->ma_GMs[DG_TEXT];
	for (long iSpliteBeam = 0; iSpliteBeam < SpliteBeamGArr.GetSize(); iSpliteBeam++)
	{
		MSBeamG* pSpliteBeam = SpliteBeamGArr[iSpliteBeam];
		shared_ptr<GM2DCurve> CenCurve = pSpliteBeam->GetCenCurve();
		if (CenCurve->m_ObjType != GT_LINE)	continue;

		double dDegree = CenCurve->GetDegree();
		double dDegreeTol = 5;
		double dMinDist = m_DTOL_TEXT_DIST;
		int nDGType = DG_BEAM;

		MSCompoM* pCompoM = NULL;
		GMText* pGMText = NULL;
		

		CString Name = _T("");

		long NumGMs = TxtGMs.GetSize();
		for (long iGMs = 0; iGMs < NumGMs; iGMs++)
		{
			GMText* TheGM = (GMText*)TxtGMs[iGMs];
			Name.Format(_T("%s%s%s"), m_Prefix, TheGM->m_strText, m_Suffix);
			// M이 정의 되어 있지 않는 객체는 "Def"라는 키워드를 넣어주기 위해, M이 없을때는 skip
			pCompoM = GetCompoM(Name, GetMSCompoMType(DG_BEAM));  //검토요. 시간 많이 걸릴텐데 꼭 필요한가?
			if (pCompoM == NULL) continue;

			double dDegree1 = fabs(dDegree - TheGM->m_dDegree);
			double dDegree2 = fabs(dDegree - (TheGM->m_dDegree + 180.));
			if (!((dDegree1 < dDegreeTol || fabs(dDegree1 - 360) < dDegreeTol || dDegree2 < dDegreeTol || fabs(dDegree2 - 360) < dDegreeTol)))
				continue;

			bool IsInnerText = true;
			// 내부점인지 검토 : CenterPoint
			double dLocValue = CenCurve->OrthoBaseLocValue(TheGM->m_Vec);
			if (!(-DTOL_GM < dLocValue &&  dLocValue < 1. + DTOL_GM))
				IsInnerText = false;

			// 내부점인지 검토 : InsertPoint
			if (IsInnerText == false)
			{
				dLocValue = CenCurve->OrthoBaseLocValue(TheGM->m_InsertVec);
				if ((-DTOL_GM < dLocValue &&  dLocValue < 1. + DTOL_GM))
					IsInnerText = true;
			}
			if (IsInnerText == false)
				continue;


			double MyDist = CenCurve->Distance(TheGM->m_Vec);
			MyDist = min(MyDist, CenCurve->Distance(TheGM->m_InsertVec));

			double TextDist = MyDist;
			if (TextDist < dMinDist)
			{
				pGMText = TheGM;
				dMinDist = TextDist;
			}
		}

		if (pGMText != NULL)
		{
			MSCompoM* pCompoM = GetCompoM(pGMText->m_strText, GetMSCompoMType(nDGType));
			if (pCompoM != NULL)
			{
				pSpliteBeam->SetCompoM(pCompoM, false);
				AddUsedTextByCompoG(pGMText, pSpliteBeam);
			}

			if (iSpliteBeam == 0)
			{
				pBaseCompoM = pCompoM;
				pBaseGMText = pGMText;
			}
		}
		else
		{
			if (pBaseCompoM != NULL && pSpliteBeam->mp_CompoM != pBaseCompoM)
				pSpliteBeam->SetCompoM(pBaseCompoM, false);
			if (pBaseGMText != NULL)
				AddUsedTextByCompoG(pBaseGMText, pSpliteBeam);
		}

		ma_BeamG.Add(pSpliteBeam);
	}
}

void CogReader::ExtractRcGirders()
{
	m_pQT->ResetGMTag(DG_BEAM);

	MergeLineSegments(ma_DGBeamLines, DG_BEAM);
	MergeArcSegments(ma_DGBeamArcs, DG_BEAM_ARC);

	SortDGLines(ma_DGBeamLines);
	SortDGArcs(ma_DGBeamArcs);

	SortPairLines(ma_DGBeamLines);
	long NumLine = ma_DGBeamLines.GetSize();

	ResetTag(ma_DGBeamLines);
	ResetTag(ma_DGBeamArcs);
	m_pQT->ResetSearchSpace();
	PutDGLinesIntoQT(ma_DGBeamLines);
	PutDGArcsIntoQT(ma_DGBeamArcs);

	long NumObj = ma_DGBeamLines.GetSize();
	for(long ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGBeamLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(AObj);
		AObj->m_Tag = 1;
		SearchParallelLine(AObj, m_MIN_BEAM_WIDTH, m_MAX_BEAM_WIDTH, CrossQTs, DG_BEAM);
	}

	ResetTag(ma_DGBeamLines);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGBeamLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindLinkedLine(AObj, DG_BEAM);
	}

	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGBeamLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindGappedLine(AObj, DG_BEAM);
	}

	ResetTag(ma_DGBeamLines);
	ResetTag(ma_DGBeamArcs);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGBeamLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		PrecheckWallShape(AObj);
	}

	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* MyLine = ma_DGBeamLines[ObjNum];
		if(MyLine == NULL)  
			continue;

		CheckCrossPair2(MyLine);

		long nPair = MyLine->ma_Pair.GetSize();
		if(MyLine->ma_Pair.GetSize() != 1)
			continue;


		DG2DLine* MyLink0 = MyLine->m_Linked0;
		DG2DLine* MyLink1 = MyLine->m_Linked1;

		if(!MyLink0 || !MyLink1 || !MyLink0->IsPair(MyLink1))
			continue;

		ReleasePairs(MyLine, MyLine->ma_Pair[0]);
		ReleasePairs(MyLink0, MyLink1);
	}

	ResetTag(ma_DGBeamLines);
	ResetTag(ma_DGWallLines);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGBeamLines[ObjNum];
		if(AObj == NULL)  continue;
		long NumMembers = CutEachMembers(AObj, DG_BEAM);
	}

	CutEachBeamArcs();

}

void CogReader::AlignBottomSlabLevel(MSBeamG* pBeamG)
{
	if (m_IsAddBottomSlabLevel == false)	return;

	GM2DVector cenVec = pBeamG->GetCenVec();
	MSSlabG* pTargetSlab = nullptr;
	double dTargetLevel = 0;

	GM2DVector* SVec = pBeamG->GetSVec();
	GM2DVector* EVec = pBeamG->GetEVec();
	vector<MSCompoG*> findCompoGArr = MSCompoGUtil::FindCompoG(mp_MSFloor->GetLowerFloor(), &cenVec, FindType_MSElement::msSlabG);
	for (int iSlab = 0; iSlab < findCompoGArr.size(); iSlab++)
	{
		double dTopLevel = 0, dBotLevel = 0;
		findCompoGArr[iSlab]->GetZLevels(dTopLevel, dBotLevel);

		if (pTargetSlab == nullptr)
		{
			pTargetSlab = (MSSlabG*)findCompoGArr[iSlab];
			dTargetLevel = dTopLevel;
		}
		else
		{
			if (dTargetLevel > dTopLevel)
			{
				pTargetSlab = (MSSlabG*)findCompoGArr[iSlab];
				dTargetLevel = dTopLevel;
			}
		}
	}

	if (pTargetSlab != nullptr)
	{
		pBeamG->m_SOffsetZ = pTargetSlab->GetZOffsetOnSlabTopPlane(SVec->m_X, SVec->m_Y) - pTargetSlab->GetThick();
		pBeamG->m_EOffsetZ = pTargetSlab->GetZOffsetOnSlabTopPlane(EVec->m_X, EVec->m_Y) - pTargetSlab->GetThick();
	}
}
void CogReader::ExtractSteelGirders()
{
	if(GetSteelBeamLineOption() == 1)
		ExtractRcGirders();
	else
	{
		ExtractSteelGirders_DimensionStyle();
		ExtractSteelGirders_OneLineSytle();
		for (auto it = ma_SteelBeamData.begin(); it != ma_SteelBeamData.end(); it++)
		{
			CString strDefName = MakeDefaultName(_T("DefaultG1"));
			MSCompoM* pCompoM = GetCompoM(strDefName, GetMSCompoMType(DG_BEAM));
			if (pCompoM == nullptr)
			{
				double dB = 500;
				pCompoM = CreateNewBeamM(dB, strDefName);
			}
			MSBeamG* pBeamG = (MSBeamG*)m_pBldg->CreateNewCompoG(MSCompoG::msBeamG);

			GM2DVector SVec = (*it)->m_P0;
			GM2DVector EVec = (*it)->m_P1;

			pBeamG->ReplaceVec(0, new GM2DVector(SVec.m_X, SVec.m_Y));
			pBeamG->ReplaceVec(1, new GM2DVector(EVec.m_X, EVec.m_Y));
			pBeamG->m_SOffsetZ = m_BotOffset;
			pBeamG->m_EOffsetZ = m_BotOffset;

			if (m_IsAddBottomSlabLevel == true)	AlignBottomSlabLevel(pBeamG);

			pBeamG->mp_CompoM = pCompoM;
			pBeamG->SetLineAlignType(LINEALIGN_CENTER);
			ma_BeamG.Add(pBeamG);
		}
	}
}

void CogReader::AddSteelBeamDataByBlock(double x1, double y1, double x2, double y2)
{
	GM2DLineVector* lineVec = new GM2DLineVector(x1, y1, x2, y2);
	ma_SteelBeamData.push_back(lineVec);
}

void CogReader::ExtractSteelGirders_DimensionStyle()
{
	CString strMsg;
	CString GroupName = _T("Beam");

	int nData = ma_Dimension.size();
	for(int iData = 0; iData < nData; iData++)
	{
		MSDimensionInfo* pDimInfo = ma_Dimension[iData];
		CString strDefName;
		strDefName.Format(_T("%sG1"), m_DefNamePrefix);
		CString Name;
		Name.Format(_T("%s%s%s"), m_Prefix, pDimInfo->m_Text, m_Suffix);

		Name = MakeDefaultName(pDimInfo->m_Text);
		MSCompoM* pCompoM = GetCompoM(Name, GetMSCompoMType(DG_BEAM));
		strDefName = Name;

		vector<long> errorFigureIDs;
		if (GetRecognizeMode() == eRecognizeMode::Single)
		{
			if (pCompoM == NULL)
			{
				strMsg.Format(_T("%s - %s"), pDimInfo->m_Text, WARNING_SECTION_NONE);
				AddErrorObject(errorFigureIDs, W_CODE_SECTION_NONE, ERROR_LEVEL, GroupName, strMsg);
				continue;
			}
		}
		else if (GetRecognizeMode() == eRecognizeMode::History)
		{
			double dB = 300;
			pCompoM = CreateNewBeamM(dB, pDimInfo->m_Text);
			MSCompoM* oldCompoM = GetCompoM(pCompoM->GetName(), GetMSCompoMType(DG_BEAM));
			if (oldCompoM != nullptr)
				((MSSteelBeamM*)pCompoM)->m_pSection = ((MSSteelBeamM*)oldCompoM)->GetSteelSection();
		}
		else
			continue;

		if (pCompoM == nullptr)	continue;

		GM2DVector SVec, EVec;
		GM2DLineVector aLineVec(pDimInfo->m_DefPoint2, pDimInfo->m_LinePosition);
		GM2DVector orthoVec = aLineVec.OrthoBaseVector(pDimInfo->m_DefPoint1);
		GM2DLineVector orthoLineVec(pDimInfo->m_DefPoint1, orthoVec);
		if(orthoLineVec.Length() < DTOL_GM)	// linePosition과 DefPoint2가 같은 위치
		{
			SVec = pDimInfo->m_DefPoint1;
			EVec = pDimInfo->m_DefPoint2;
		}
		else
		{
			double dist = orthoLineVec.Distance(pDimInfo->m_LinePosition);
			GM2DLineVector aDimLVec = orthoLineVec.NormalTranslate(dist, pDimInfo->m_LinePosition); 

			SVec = aDimLVec.m_P0;
			EVec = aDimLVec.m_P1;
		}

		MSBeamG* pBeamG = (MSBeamG*)m_pBldg->CreateNewCompoG(MSCompoG::msBeamG);
		pBeamG->ReplaceVec(0, new GM2DVector(SVec.m_X, SVec.m_Y));
		pBeamG->ReplaceVec(1, new GM2DVector(EVec.m_X, EVec.m_Y));
		pBeamG->m_SOffsetZ = m_BotOffset;
		pBeamG->m_EOffsetZ = m_BotOffset;

		if (m_IsAddBottomSlabLevel == true)	AlignBottomSlabLevel(pBeamG);

		pBeamG->mp_CompoM = pCompoM;
		pBeamG->SetLineAlignType(LINEALIGN_CENTER);
		ma_BeamG.Add(pBeamG);
	}
}

void CogReader::ExtractSteelGirders_OneLineSytle()
{
	MergeLineSegments(ma_DGBeamLines, DG_BEAM);
//	MergeArcSegments(ma_DGBeamArcs, DG_BEAM_ARC);

	SortDGLines(ma_DGBeamLines);
//	SortDGArcs(ma_DGBeamArcs);

	CString strMsg, noNameMsg;
	long dB = 300;
	long dH = 700;
	int nLine = ma_DGBeamLines.GetSize();
	for(int iLine = 0; iLine < nLine; iLine++)
	{
		DG2DLine* pDGLine = ma_DGBeamLines[iLine];

		if(pDGLine->Length() < m_MIN_BEAM_LENGTH)	continue;
		GM2DVector SVec = pDGLine->m_P0;
		GM2DVector EVec = pDGLine->m_P1;
		MSCompoM* pCompoM = NULL;
		bool IsNoNameError = false;
		vector<long> errorFigureIDs;	

		GMText* pGMTxt = FindGMText(pDGLine, pDGLine, DG_BEAM);
		CString strDefName;
		strDefName = MakeDefaultName(dB);
		if (GetRecognizeMode() == eRecognizeMode::Single)
		{
			if (pGMTxt != nullptr)
			{
				strDefName = MakeDefaultName(pGMTxt->m_strText);
				pCompoM = GetCompoM(strDefName, GetMSCompoMType(DG_BEAM));
			}
			else
			{
				// Text를 못찾은 경우 warning
				noNameMsg.Format(_T("%s - %s"), strDefName, WARNING_NAME_NONE);
				IsNoNameError = TRUE;
			}

			if (pCompoM == NULL)
			{
				if (pGMTxt != nullptr)			// Text는 있지만, 단면정보가 없는 경우
				{
					strMsg.Format(_T("%s - %s"), pGMTxt->m_strText, WARNING_SECTION_NONE);
					AddErrorObject(errorFigureIDs, W_CODE_SECTION_NONE, WARNING_LEVEL, GetGroupName(), strMsg);
				}

				pCompoM = GetCompoM(strDefName, MSCompoM::msBeamM);
				if (pCompoM == NULL)
				{
					if (!m_IsSaveM)
					{
						if (pGMTxt)	strDefName += _T("(No Section)");
					}
					pCompoM = CreateNewBeamM(dB, strDefName);
				}
			}
		}
		else if (GetRecognizeMode() == eRecognizeMode::History)
		{
			if (pGMTxt != nullptr)
				strDefName = MakeDefaultName(pGMTxt->m_strText);
			pCompoM = CreateNewBeamM(dB, strDefName);
			MSCompoM* oldCompoM = GetCompoM(pCompoM->GetName(), GetMSCompoMType(DG_BEAM));
			if (oldCompoM != nullptr)
				((MSSteelBeamM*)pCompoM)->m_pSection = ((MSSteelBeamM*)oldCompoM)->GetSteelSection();
		}
		else
			continue;

		if (pCompoM == nullptr)	continue;

		long oldWidth = pCompoM->GetWidth();

		MSPcLineMemberM* pPcBeamM = dynamic_cast<MSPcLineMemberM*>(pCompoM);
		if(pPcBeamM != nullptr)	oldWidth = (long)pPcBeamM->GetPcWidth();

		MSBeamG* pBeamG = (MSBeamG*)m_pBldg->CreateNewCompoG(MSCompoG::msBeamG);
		pBeamG->ReplaceVec(0, new GM2DVector(SVec.m_X, SVec.m_Y));
		pBeamG->ReplaceVec(1, new GM2DVector(EVec.m_X, EVec.m_Y));
		pBeamG->m_SOffsetZ = m_BotOffset;
		pBeamG->m_EOffsetZ = m_BotOffset;

		if (m_IsAddBottomSlabLevel == true)	AlignBottomSlabLevel(pBeamG);

		pBeamG->mp_CompoM = pCompoM;
		pBeamG->SetLineAlignType(LINEALIGN_CENTER);
		ma_BeamG.Add(pBeamG);

		if(IsNoNameError)
		{
			vector<MSElement*> CompoGArr;
			CompoGArr.push_back(pBeamG);
			AddErrorObject(CompoGArr, W_CODE_NAME_NONE, WARNING_LEVEL, GetGroupName(), noNameMsg);
		}

		vector<long> FigureIDs;
		AddVdFigureID(FigureIDs, FindGeometyToFigureIDs(pDGLine));
		mm_CompoGtoFigureIDs.insert(make_pair(pBeamG, FigureIDs));

		if(pGMTxt)
			AddUsedTextByCompoG(pGMTxt, pBeamG);
	}

	ExtractSteelGirdersArc_OneLineSytle();
}

void CogReader::ExtractSrcGirders()
{
	ExtractRcGirders();
}

BOOL CogReader::TestWallGirder(DG2DLine* MyLine, double MinThick, double MaxThick)
{
	if(MyLine->m_pLine->m_ObjType == GT_LINE)
	{
		double MyLoc0, MyLoc1;
		GM2DVector BaseVec0;

		long nIndex = 0;

		map<long, MSCompoG*>::iterator itCompoG;
		for(itCompoG=mp_MSFloor->mm_CompoG.begin(); itCompoG != mp_MSFloor->mm_CompoG.end(); itCompoG++)
		{
			MSCompoG* pCompoG = itCompoG->second;

			if(pCompoG->GetType() != MSCompoG::msWallG)
				continue;

			MSWallMemberG* pWall = (MSWallMemberG*)pCompoG;
			GM2DLineVector* pFarWallLine;
			DG2DLine* pFarDGLine;
			double Dist;
			// 평행성 검사

			if(pWall->mp_LLine->m_ObjType != GT_LINE)
				return FALSE;

			long Result = ((GM2DLineVector*)MyLine->m_pLine)->IsParallel4DTol(*(GM2DLineVector*)pWall->mp_LLine, m_DTOL_PARALLEL_DEGREE);
			if(Result == 0)
				continue;
			double Dist2LLine = MyLine->Distance(pWall->mp_LLine->GetP0());
			double Dist2RLine = MyLine->Distance(pWall->mp_RLine->GetP0());
			if(Dist2LLine > Dist2RLine)
			{
				pFarWallLine = (GM2DLineVector*)pWall->mp_LLine;
				Dist = Dist2LLine;
			}
			else
			{
				pFarWallLine = (GM2DLineVector*)pWall->mp_RLine;
				Dist = Dist2RLine;
			}
			pFarDGLine = new DG2DLine(pFarWallLine, m_pQT);

			// 주어진 폭(width) 대비 m_DTOL_PAIR_DIST_ERRRATIO 이하 오차인지 검사
			if(Dist>MaxThick || Dist<MinThick) 
				continue;
			// 서로 m_DTOL_MIN_WALL_LENGTH 이상 중첩되는지 검사
			MyLoc0 = MyLine->OrthoBaseLocValue(pFarWallLine->m_P0);
			MyLoc1 = MyLine->OrthoBaseLocValue(pFarWallLine->m_P1);
			Result = OverlapGroup(MyLoc0, MyLoc1, m_DTOL_DIST_LINKTEST/10000.);
			// 1. 서로 중첩되지 않음
			if(Result==1 || Result==2 || Result==6 || Result==7 || Result==19 || Result==20 || Result==24 || Result==25)
				continue;

			long NumPairLine = MyLine->ma_Pair.GetSize();
			BOOL bIsOverlap = FALSE;
			for(long PairLineNum=0; PairLineNum<NumPairLine; PairLineNum++)
			{
				DG2DLine* pPairLine = MyLine->ma_Pair[PairLineNum];
				double PairLoc0 = pPairLine->OrthoBaseLocValue(pFarWallLine->m_P0);
				double PairLoc1 = pPairLine->OrthoBaseLocValue(pFarWallLine->m_P1);
				long PairResult =  OverlapGroup(PairLoc0, PairLoc1, m_DTOL_DIST_LINKTEST/10000.);
				// 1. 기존 패어라인과 중첩되면 무효
				if(!(PairResult==1 || PairResult==2 || PairResult==6 || 
					PairResult==20 || PairResult==24 || PairResult==25))
				{
					bIsOverlap = TRUE;
					break;
				}
			}
			if(bIsOverlap)	continue;

			BOOL MyLineFull = MyLine->AddPairWall(pFarDGLine, MyLoc0, MyLoc1, Result, 200);
			if(MyLineFull)
				return TRUE;
		}

		return FALSE;
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}
	return FALSE;
}

long CogReader::CutEachBeams(DG2DLine* MyLine)
{
	if(MyLine==NULL)	return 0;
	long NumPairs = MyLine->ma_Pair.GetSize();
	if(NumPairs==0)	
		return 0;

	// 마이라인이 페어라인 하나만 갖고, 이미 페어라인에 의하여 보생성되며 태그처리되었으면 처리안해도 됨
	if(NumPairs==1 && MyLine->m_Tag)
		return 0;
  // 마이라인으로 처리된 디지라인은 태그하여 페어에서 재생성되지 않도록 함
	MyLine->m_Tag = 1;

	BOOL bPairSameDir = FALSE;
	BOOL bGapSameDir = FALSE;
	double dMargin = 10.;
	long NumDGBeam = 0;  // 생성된 보의 수 축적하여 리턴함
	BOOL bMakeUrLine = FALSE;

	double dB = 300;
	double dH = 700;

	GM2DVector CrossVec;
	BOOL MyResult = FALSE, LinkResult=FALSE, UrResult=FALSE;
	GM2DLineVector MainLVec, UrCenLVec, LinkCenLVec;

	CheckStaggeredBeam(MyLine, TRUE);
	CheckStaggeredBeam(MyLine, FALSE);

	NumPairs = MyLine->ma_Pair.GetSize();
	if(NumPairs==0)
		return 0;

	// Error Message
	CString GroupName = GetGroupName();
	CString strMsg, noNameMsg;

	BOOL bFirstLine = TRUE;
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

		if(SVec.Distance(EVec) <= m_MIN_BEAM_LENGTH)
		{
			strMsg = ERROR_MIN_LENGTH;
			AddErrorObject(errorFigureIDs, E_CODE_MIN_LENGTH, ERROR_LEVEL, GroupName, strMsg);
			continue;
		}

		MSCompoM* pCompoM = NULL;
		double dB = MyLine->Distance(UrLine->m_P0);
		dB = ROUND(dB);
		double dH = m_defaultDepth;	// def depth
		bool IsNoNameError = false;

		GMText* pGMTxt = FindGMText(MyLine, UrLine, DG_BEAM);
		CString strDefName;
		strDefName = MakeDefaultName(dB, dH);
		if (GetRecognizeMode() == eRecognizeMode::Single)
		{
			if (pGMTxt != nullptr)
			{
				strDefName = MakeDefaultName(pGMTxt->m_strText);
				pCompoM = GetCompoM(strDefName, GetMSCompoMType(DG_BEAM));
			}
			else
			{
				// Text를 못찾은 경우 warning
				noNameMsg.Format(_T("%s - %s"), strDefName, WARNING_NAME_NONE);
				IsNoNameError = TRUE;
			}

			if (pCompoM == NULL)
			{
				if (pGMTxt != nullptr)			// Text는 있지만, 단면정보가 없는 경우
				{
					strMsg.Format(_T("%s - %s"), pGMTxt->m_strText, WARNING_SECTION_NONE);
					AddErrorObject(errorFigureIDs, W_CODE_SECTION_NONE, WARNING_LEVEL, GroupName, strMsg);
				}

				pCompoM = GetCompoM(strDefName, MSCompoM::msBeamM);
				if (pCompoM == NULL)
				{
					if (!m_IsSaveM)
					{
						if (pGMTxt)	strDefName += _T("(No Section)");
					}
					pCompoM = CreateNewBeamM(dB, strDefName);
				}
			}
		}
		else if (GetRecognizeMode() == eRecognizeMode::History)
		{
			if (pGMTxt != nullptr)
				strDefName = MakeDefaultName(pGMTxt->m_strText);
			pCompoM = CreateNewBeamM(dB, strDefName);
		}
		else if (GetRecognizeMode() == eRecognizeMode::Formwork)
		{
			pCompoM = CreateNewBeamM(dB, strDefName);
			if (pGMTxt != nullptr)
			{
				AnalyzeBeamDepthText(pCompoM, pGMTxt);
			}
		}
		else
			continue;

		if (pCompoM == nullptr)	continue;

		long oldWidth = pCompoM->GetWidth();
		MSPcLineMemberM* pPcBeamM = dynamic_cast<MSPcLineMemberM*>(pCompoM);
		if(pPcBeamM != nullptr)	oldWidth = (long)pPcBeamM->GetPcWidth();

		MSBeamG* pBeamG = (MSBeamG*)m_pBldg->CreateNewCompoG(MSCompoG::msBeamG);
		pBeamG->ReplaceVec(0, new GM2DVector(SVec.m_X, SVec.m_Y));
		pBeamG->ReplaceVec(1, new GM2DVector(EVec.m_X, EVec.m_Y));
		pBeamG->m_SOffsetZ = m_BotOffset;
		pBeamG->m_EOffsetZ = m_BotOffset;
		pBeamG->mp_CompoM = pCompoM;
		pBeamG->SetLineAlignType(LINEALIGN_CENTER);
        ma_BeamG.Add(pBeamG);

		if (GetRecognizeMode() == eRecognizeMode::Single)
		{
			if (fabs(oldWidth - dB) > 1.0)
			{
				vector<MSElement*> compoGArr;	compoGArr.push_back(pBeamG);
				strMsg.Format(_T("%s - 단면정보(W:%ld)와 평면크기(W:%ld)가 다릅니다."), pCompoM->m_Name, oldWidth, (long)dB);
				AddErrorObject(compoGArr, W_CODE_SIZE_DIFF, WARNING_LEVEL, GroupName, strMsg);
			}

			if (IsNoNameError)
			{
				vector<MSElement*> CompoGArr;
				CompoGArr.push_back(pBeamG);
				AddErrorObject(CompoGArr, W_CODE_NAME_NONE, WARNING_LEVEL, GroupName, noNameMsg);
			}

			vector<long> FigureIDs;
			AddVdFigureID(FigureIDs, FindGeometyToFigureIDs(MyLine));
			AddVdFigureID(FigureIDs, FindGeometyToFigureIDs(UrLine));
			mm_CompoGtoFigureIDs.insert(make_pair(pBeamG, FigureIDs));

		}
		if (pGMTxt)
			AddUsedTextByCompoG(pGMTxt, pBeamG);

		NumDGBeam++;
	}
	return NumDGBeam;
}

bool CogReader::AnalyzeBeamDepthText(MSCompoM* pCompoM, GMText* pGMTxt)
{
	CString prefix = _T("B:");
	CString suffix = _T("H");
	if (GetIsReverseBeam())	prefix = _T("U:");

	int prefixIndex = pGMTxt->m_strText.Find(prefix);
	int suffixIndex = pGMTxt->m_strText.Find(suffix);
	if (prefixIndex == -1 || suffixIndex == -1)	return false;

	int newDepth = MTReader::FindNumberData(prefixIndex + 2, pGMTxt->m_strText);
	if (newDepth > 50)
	{
		MSRcFWBeamM* pBeamM = dynamic_cast<MSRcFWBeamM*>(pCompoM);
		pBeamM->SetH(newDepth);
		pBeamM->MakeProfile();
		return true;
	}
	return false;
}

long CogReader::CutEachBeamArcs()
{
	long NumBeam = 0;
	double dMargin = 10;
	long NumArc = ma_DGBeamArcs.GetSize();

	CString GroupName = GetGroupName();
	CString strMsg, noNameMsg;
	CTypedPtrArray<CObArray, GMQuadTree*> CrossQTs;
	CrossQTs.Add(m_pQT);
	for(long iArc=0; iArc<NumArc; iArc++)
	{
		DG2DArc* MyArc = ma_DGBeamArcs[iArc];
		double dRadiance = MyArc->m_pArc->m_A;
		MyArc->m_Tag = 1;
		double dSAngle = MyArc->m_pArc->m_Theta1;
		double dEAngle = MyArc->m_pArc->m_Theta2;
		GMText* GMTxt = FindGMArcText(MyArc, DG_BEAM);
		MSCompoM* pCompoM = NULL;
		CString strDefName;
		for(long jArc=iArc+1; jArc<NumArc; jArc++)
		{
			DG2DArc* UrArc = ma_DGBeamArcs[jArc];
			if(UrArc->m_Tag)	continue;
			if(MyArc->m_pArc->m_Center.SameVector4DTOL(UrArc->m_pArc->m_Center, m_DTOL_DIST_COLINEAR)==FALSE)	continue;

			double dPairRadiance = UrArc->m_pArc->m_A;
			double dDistance = fabs(dRadiance	-dPairRadiance);
			if(dDistance < m_MIN_BEAM_WIDTH || dDistance > m_MAX_BEAM_WIDTH)	continue;

			long Result = MyArc->m_pArc->Overlap4DTol(*UrArc->m_pArc, m_DTOL_DEGREE_COLINEAR, m_MAX_BEAM_WIDTH);
			if(Result==1 || Result==2 || Result==12 || Result==0)	continue;

			vector<long> errorFigureIDs;
			AddVdFigureID(errorFigureIDs, FindGeometyToFigureIDs(MyArc));
			AddVdFigureID(errorFigureIDs, FindGeometyToFigureIDs(UrArc));

			double dB = dDistance;
			double dH = m_defaultDepth;
			bool IsNoNameError = false;
			strDefName = MakeDefaultName(dB, dH);
			if (GetRecognizeMode() == eRecognizeMode::Single)
			{
				if (GMTxt != nullptr)
				{
					strDefName = MakeDefaultName(GMTxt->m_strText);
					pCompoM = GetCompoM(strDefName, GetMSCompoMType(DG_BEAM));
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

					pCompoM = GetCompoM(strDefName, MSCompoM::msBeamM);
					if (pCompoM == NULL)
					{
						if (!m_IsSaveM)
						{
							if (GMTxt)	strDefName += _T("(No Section)");
						}
						pCompoM = CreateNewBeamM(dB, strDefName);
					}
				}
			}
			else if (GetRecognizeMode() == eRecognizeMode::History)
			{
				if (GMTxt != nullptr)
					strDefName = MakeDefaultName(GMTxt->m_strText);
				pCompoM = CreateNewBeamM(dB, strDefName);
			}
			else if (GetRecognizeMode() == eRecognizeMode::Formwork)
			{
				pCompoM = CreateNewBeamM(dB, strDefName);
			}
			else
				continue;

			if (pCompoM == nullptr)	continue;

			long oldWidth = (long)pCompoM->GetWidth();
			MSPcLineMemberM* pPcBeamM = dynamic_cast<MSPcLineMemberM*>(pCompoM);
			if(pPcBeamM != nullptr)	oldWidth = (long)pPcBeamM->GetPcWidth();

			if(UrArc->m_pArc->m_Theta1 < dSAngle)
				dSAngle = UrArc->m_pArc->m_Theta1;
			if(UrArc->m_pArc->m_Theta2 > dEAngle)
				dEAngle = UrArc->m_pArc->m_Theta2;

			GMArc TheArc(MyArc->m_pArc->m_A-dDistance/2, MyArc->m_pArc->m_Center.m_X, MyArc->m_pArc->m_Center.m_Y, dSAngle, dEAngle) ;
			GM2DVector SVec = TheArc.GetStart2DVec();
			GM2DVector EVec = TheArc.GetEnd2DVec();

			MSBeamG* pBeamG = (MSBeamG*)m_pBldg->CreateNewCompoG(MSCompoG::msBeamG);
			pBeamG->ReplaceVec(0, new GM2DVector(SVec.m_X, SVec.m_Y));
			pBeamG->ReplaceVec(1, new GM2DVector(EVec.m_X, EVec.m_Y));
			pBeamG->m_SOffsetZ = m_BotOffset;
			pBeamG->m_EOffsetZ = m_BotOffset;
			pBeamG->mp_CompoM = pCompoM;
			pBeamG->SetLineAlignType(LINEALIGN_CENTER);
			ma_BeamG.Add(pBeamG);
			GM2DArc* pArc = new GM2DArc(TheArc.m_A, TheArc.m_Center.m_X, TheArc.m_Center.m_Y, dSAngle, dEAngle);
			pBeamG->SetCenCurve(pArc);

			if (GetRecognizeMode() == eRecognizeMode::Single)
			{
				if (IsNoNameError)
				{
					vector<MSElement*> CompoGArr;
					CompoGArr.push_back(pBeamG);
					AddErrorObject(CompoGArr, W_CODE_NAME_NONE, WARNING_LEVEL, GroupName, noNameMsg);
				}
				if (fabs(oldWidth - dB) > 1.0)
				{
					vector<MSElement*> compoGArr;	compoGArr.push_back(pBeamG);
					strMsg.Format(_T("%s - 단면정보(W:%ld)와 평면크기(W:%ld)가 다릅니다."), pCompoM->m_Name, oldWidth, (long)dB);
					AddErrorObject(compoGArr, W_CODE_SIZE_DIFF, WARNING_LEVEL, GroupName, strMsg);
				}

				vector<long> FigureIDs;
				AddVdFigureID(FigureIDs, FindGeometyToFigureIDs(MyArc));
				AddVdFigureID(FigureIDs, FindGeometyToFigureIDs(UrArc));
				mm_CompoGtoFigureIDs.insert(make_pair(pBeamG, FigureIDs));
			}

			if(GMTxt)
				AddUsedTextByCompoG(GMTxt, pBeamG);

			NumBeam++;
		}
	}
	return NumBeam;
}

void CogReader::ExtractSteelGirdersArc_OneLineSytle()
{
	MergeArcSegments(ma_DGBeamArcs, DG_BEAM_ARC);
	
	double dMargin = 10;
	long NumArc = ma_DGBeamArcs.GetSize();

	CString GroupName = GetGroupName();
	CString strMsg, noNameMsg;
	CTypedPtrArray<CObArray, GMQuadTree*> CrossQTs;
	CrossQTs.Add(m_pQT);
	for(long iArc=0; iArc<NumArc; iArc++)
	{
		DG2DArc* MyArc = ma_DGBeamArcs[iArc];
		double dRadiance = MyArc->m_pArc->m_A;
		MyArc->m_Tag = 1;
		double dSAngle = MyArc->m_pArc->m_Theta1;
		double dEAngle = MyArc->m_pArc->m_Theta2;

		GMText* GMTxt = FindGMArcText(MyArc, DG_BEAM);
		MSCompoM* pCompoM = NULL;
		CString strDefName;

		vector<long> errorFigureIDs;
		AddVdFigureID(errorFigureIDs, FindGeometyToFigureIDs(MyArc));

		double dB = 300;
		double dH = 700;
		bool IsNoNameError = false;
		strDefName.Format(_T("%sG1"), m_DefNamePrefix);
		if (GetRecognizeMode() == eRecognizeMode::Single)
		{
			if (GMTxt != nullptr)
			{
				strDefName = MakeDefaultName(GMTxt->m_strText);
				pCompoM = GetCompoM(strDefName, GetMSCompoMType(DG_BEAM));
			}
			else
			{
				// Text를 못찾은 경우 warning
				noNameMsg.Format(_T("%s - %s"), strDefName, WARNING_NAME_NONE);
				IsNoNameError = true;
			}
			if (pCompoM == NULL)
			{
				if (GMTxt != nullptr)			// Text는 있지만, 단면정보가 없는 경우
				{
					strMsg.Format(_T("%s - %s"), GMTxt->m_strText, WARNING_SECTION_NONE);
					AddErrorObject(errorFigureIDs, W_CODE_SECTION_NONE, WARNING_LEVEL, GroupName, strMsg);
				}

				pCompoM = GetCompoM(strDefName, MSCompoM::msBeamM);
				if (pCompoM == NULL)
				{
					if (!m_IsSaveM)
					{
						if (GMTxt)	strDefName += _T("(No Section)");
					}
					pCompoM = CreateNewBeamM(dB, strDefName);
				}
			}
		}
		else if (GetRecognizeMode() == eRecognizeMode::History)
		{
			if (GMTxt != nullptr)
				strDefName = MakeDefaultName(GMTxt->m_strText);
			pCompoM = CreateNewBeamM(dB, strDefName);
		}
		else
			continue;;

		if (pCompoM == nullptr)	continue;

		GMArc TheArc(MyArc->m_pArc->m_A, MyArc->m_pArc->m_Center.m_X, MyArc->m_pArc->m_Center.m_Y, dSAngle, dEAngle) ;
		GM2DVector SVec = TheArc.GetStart2DVec();
		GM2DVector EVec = TheArc.GetEnd2DVec();

		MSBeamG* pBeamG = (MSBeamG*)m_pBldg->CreateNewCompoG(MSCompoG::msBeamG);
		pBeamG->ReplaceVec(0, new GM2DVector(SVec.m_X, SVec.m_Y));
		pBeamG->ReplaceVec(1, new GM2DVector(EVec.m_X, EVec.m_Y));
		pBeamG->m_SOffsetZ = m_BotOffset;
		pBeamG->m_EOffsetZ = m_BotOffset;

		if (m_IsAddBottomSlabLevel == true)	AlignBottomSlabLevel(pBeamG);

		pBeamG->mp_CompoM = pCompoM;
		pBeamG->SetLineAlignType(LINEALIGN_CENTER);
		ma_BeamG.Add(pBeamG);
		GM2DArc* pArc = new GM2DArc(TheArc.m_A, TheArc.m_Center.m_X, TheArc.m_Center.m_Y, dSAngle, dEAngle);
		pBeamG->SetCenCurve(pArc);

		if (GetRecognizeMode() == eRecognizeMode::Single)
		{
			if (IsNoNameError)
			{
				vector<MSElement*> CompoGArr;
				CompoGArr.push_back(pBeamG);
				AddErrorObject(CompoGArr, W_CODE_NAME_NONE, WARNING_LEVEL, GroupName, noNameMsg);
			}

			vector<long> FigureIDs;
			AddVdFigureID(FigureIDs, FindGeometyToFigureIDs(MyArc));
			mm_CompoGtoFigureIDs.insert(make_pair(pBeamG, FigureIDs));
		}

		if(GMTxt)
			AddUsedTextByCompoG(GMTxt, pBeamG);
	}
}

GMText* CogReader::FindGMArcText(DG2DArc* MyArc, long nDGType)
{
	CArray<CString, CString> KeyWordArr;
	CString KeyWord = _T("");
	BOOL bUseKeyWord = FALSE;
	switch(nDGType)
	{
	case DG_WALL:
		KeyWordArr.Add(_T("W"));
		KeyWordArr.Add(_T("BT"));
		bUseKeyWord = TRUE;
		break;
	case DG_RWALL:
		KeyWordArr.Add(_T("R"));
		bUseKeyWord = TRUE;
		break;
	case DG_BEAM:
		KeyWordArr.Add(_T("G"));
		KeyWordArr.Add(_T("B"));
		bUseKeyWord = TRUE;
		break;
	case DG_WFOOT:
		KeyWordArr.Add(_T("WF"));
		bUseKeyWord = TRUE;
		break;
	case DG_LOWWALL:
		KeyWordArr.Add(_T("HR"));
		bUseKeyWord = TRUE;
		break;
	}
	double dTolerance = m_DTOL_TEXT_DIST;
	GMText* pRtnGM = NULL;
	double dMinDist = 1/DTOL10;
	double dRadiance = MyArc->m_pArc->m_A;
	double dSAngle = MyArc->m_pArc->m_Theta1;
	double dEAngle = MyArc->m_pArc->m_Theta2;
	GMVector CenVec(MyArc->m_pArc->m_Center);

	CString Name;
	GMQuadTree* QTree = m_pQT;
	// Text찾는게 이상해서, 일단 전체에서 찾아본다.
	CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = QTree->ma_GMs[DG_TEXT];
	//		CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = m_pQT->ma_GMs[DG_TEXT];
	long NumGMs = TxtGMs.GetSize();
	for(long iGMs=0; iGMs<NumGMs; iGMs++)
	{
		GMText* TheGM = (GMText*)TxtGMs[iGMs];
		if(TheGM==NULL) continue;

		if (GetRecognizeMode() == eRecognizeMode::Single && !m_IsSaveM)
		{
			Name.Format(_T("%s%s%s"), m_Prefix, TheGM->m_strText, m_Suffix);
			// M이 정의 되어 있지 않는 객체는 "Def"라는 키워드를 넣어주기 위해, M이 없을때는 skip
			MSCompoM* pCompoM = GetCompoM(Name, GetMSCompoMType(nDGType));  //검토요. 시간 많이 걸릴텐데 꼭 필요한가?
			if (pCompoM == NULL)	continue;
		}

		GM2DVector TheVec(TheGM->m_Vec.m_X, TheGM->m_Vec.m_Y);
		GM2DVector OrthoVec = MyArc->m_pArc->OrthoBaseVector(TheVec);
		double dDist = OrthoVec.Distance(TheVec);
		if(dDist > dTolerance)
			continue;

		double dLocValue = MyArc->m_pArc->OrthoBaseLocValue(OrthoVec);
		if(!(-DTOL_GM < dLocValue &&  dLocValue < 1.+DTOL_GM))
			continue;
		if(dDist < dMinDist)
		{
			pRtnGM = TheGM;
			dMinDist = dDist;
		}
	}
	return pRtnGM;
}
/*

bool CogReader::IsRebarTextByNumber(GMText* pText)
{
	CString RebarPrefix = MSDeformedBar::GetDefaultRebarNamePrefix();

	if(pText->m_strText.Left(1) != RebarPrefix)
		return false;


	return true;
}

bool CogReader::IsRebarTextBySpace(GMText* pText)
{
	CString& RebarText = pText->m_strText;

	int CurPos = 0;
	CString RebarType = RebarText.Tokenize(_T("@"), CurPos);
	if(CurPos==-1)
		return false;
	CString Space = RebarText.Tokenize(_T("@"), CurPos);




	CString HiBarPrefix = MSDeformedBar::GetDefaultRebarNamePrefix(0); // 고강도 철근 프리픽스
	CString MildBarPrefix = MSDeformedBar::GetDefaultRebarNamePrefix(1); // 보통 철근 프리픽스
	
	if(pText->m_strText.Left(RebarPrefix.GetLength()) == RebarPrefix )
		return ;


	return true;
}
*/

GMText* CogReader::FindGMTextSlab(GM2DLineVector* pMyLine)
{
	if(pMyLine->m_ObjType == GT_LINE)
	{
		double dDegreeTol = 5;
		double dDegree = pMyLine->GetDegree();
		double dLastDist = m_DTOL_TEXT_DIST;
		double dLastLocValue = 100.;
		GMText* pLastText = NULL;

		vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(pMyLine);

		long NumCrossQT = CrossQTs.size();
		for(long iCrossQT=0; iCrossQT<NumCrossQT; iCrossQT++)
		{
			GMQuadTree* QTree = CrossQTs[iCrossQT];
			CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = QTree->ma_GMs[DG_TEXT];

			long NumGMs = TxtGMs.GetSize();
			for(long iGMs=0; iGMs<NumGMs; iGMs++)
			{
				GMText* pGMText = (GMText*)TxtGMs[iGMs];

				if(MSStirrupBar::IsCorrectRebarBySpace(pGMText->m_strText)==false)
					continue;

				double dDegree1 = fabs(dDegree - pGMText->m_dDegree);
				double dDegree2 = fabs(dDegree - (pGMText->m_dDegree+180.));
				if(!((dDegree1 < dDegreeTol || fabs(dDegree1-360) < dDegreeTol	 || dDegree2 < dDegreeTol || fabs(dDegree2-360) < dDegreeTol)))
					continue;

				double dLocValue = pMyLine->OrthoBaseLocValue(pGMText->m_Vec);	// 내부검토
				if(!(-DTOL_GM < dLocValue &&  dLocValue < 1.+DTOL_GM))
					continue;

				double MyDist = pMyLine->Distance(pGMText->m_Vec);

				double dHeight = pGMText->m_dHeight;
				if(MyDist > dHeight*1.5)
					continue;

				if(MyDist <= dLastDist && fabs(dLocValue-0.5) <= dLastLocValue)
				{
					pLastText = pGMText;
					dLastDist = MyDist;
					dLastLocValue = fabs(dLocValue-0.5);
				}
			}
		}

		return pLastText;
	}
	else
	{
		ASSERT(0);
		return NULL;
	}
}

void CogReader::ExtractFloorRebar()
{
	CogReaderSlabRebarHelper theHelper(m_pBldg, this, mp_MSFloor);
	theHelper.MakeData();

	return;

	ExtractFloorRebar_General();
	ExtractFloorRebar_MakeZone();
//	ExtractFloorRebar4GS1();
}

void CogReader::ExtractFloorRebar_MakeZone()
{
	CTypedPtrArray<CObArray, GM2DGeometry*>& RebarLines = m_pQT->ma_GMs[DG_REBARLINE];
	
}

void CogReader::ExtractFloorRebar_General()
{
	SortSlabRebarInfoByArea();
	int numZone = ma_SlabRebarInfo.size();
	
	for(int iZone = 0; iZone < numZone; iZone++)
	{
		CR_SlabRebarInfo* pZone = ma_SlabRebarInfo[iZone];

		CRFindRebarLines(pZone); // 영역 안에 들어온 배근기준선 찾기 + 영역경계를 기준으로 유효성 검토
		CRFindCirclesAndLines(pZone); // 영역 안에 들어온 원과 선 찾기 
		pZone->ValidateCircles(); // 배근기준선과 교차여부 체크
		pZone->ValidateLines(); // 배근영역선과 텍스트 연결선 구분
		CRFindTexts(pZone);	// 영역 안에 들어온 텍스트 찾기 + 기타선으로 텍스트 유효성 검토
		pZone->FindTextLine(); // 텍스트 연결선 찾기

	}

	CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms = m_pQT->ma_GMs[DG_REBARZONE];
	int numGeoms = Geoms.GetSize();

	for(int iZone = 0; iZone < numZone; iZone++)
	{
		CR_SlabRebarInfo* pZone = ma_SlabRebarInfo[iZone];
		int nRebar = pZone->MakeSlabRebars();
		for (int iRebar = 0; iRebar<nRebar;iRebar++)
		{
			MSSlabRebar* pRebar = pZone->GetSlabRebar(iRebar);
			pRebar->SetFloor(mp_MSFloor->GetLowerFloor());
			pRebar->SetOffset(m_BotOffset);
			pRebar->SetMaterialID(m_nMaterialID);
			this->ma_SlabRebar.Add(pRebar);
		}
	}
}

// 영역 안에 들어온 배근기준선 찾기 + 영역경계를 기준으로 유효성 검토
void CogReader::CRFindRebarLines(CR_SlabRebarInfo* pZone)
{
	CTypedPtrArray<CObArray, GM2DGeometry*>& RebarLines = m_pQT->ma_GMs[DG_REBARLINE];

	// 영역에 들어온 배근기준선 골라내기
	int numRebarLine = RebarLines.GetSize();
	for (int iRebarLine = 0; iRebarLine < numRebarLine; iRebarLine++)
	{
		GM2DGeometry* pGeom = RebarLines[iRebarLine];
		if (pGeom->m_ObjType != GT_LINE)	continue;
		if (pGeom->m_Tag != 0)	continue;	// 처음 사용하는 배근기준선 Tag = 0

		GM2DLineVector* pRebarLine = (GM2DLineVector*)pGeom;
		if (pZone->Contains(pRebarLine))	// 영역에 들어온 기준을 선의 중심으로 파악 (경계가 일치하지 않는 경우 많음)
			if (pZone->ValidateRebarLine(pRebarLine))	// 영역 경계와 가까운지 검토 (현재 영역 내부에 다른 영역이 있는 경우, 현재 영역이 포함된 더 큰 영역이 있는 경우 제외)
			{
				pZone->AddRebarLine(pRebarLine);
				//pRebarLine->m_Tag = 63;		// 사용한 선임을 표시
			}
	}

	// 필요한 경우 남은 선들을 가지고 기준선 붙이기 (하나의 라인으로 보이지만.. 두 개 이상인 경우 많음)
}

// 영역 안에 들어온 원과 선 찾기
void CogReader::CRFindCirclesAndLines(CR_SlabRebarInfo* pZone)
{
	CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms = m_pQT->ma_GMs[DG_REBARZONE];
	int numGeoms = Geoms.GetSize();

	for (int iGeom = 0; iGeom < numGeoms; iGeom++)
	{
		GM2DGeometry* pGeom = Geoms[iGeom];
		if (pGeom->m_ObjType == GT_LINE)
		{
			GM2DLineVector* pLine = (GM2DLineVector*)pGeom;
			if (pZone->Contains(pLine))
				pZone->AddLineVector(pLine);
		}
		else if (pGeom->m_ObjType == GT_CIRCLE)
		{
			GM2DCircle* pCircle = (GM2DCircle*)pGeom;
			if (pZone->Contains(pCircle))
				pZone->AddCircle(pCircle);
		}
	}
}

// 영역 안에 들어온 텍스트 찾기
void CogReader::CRFindTexts(CR_SlabRebarInfo* pZone)
{
	CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms = m_pQT->ma_GMs[DG_TEXT];
	MTReader mtReader;

	int numGeoms = Geoms.GetSize();
	for (int iGeom = 0; iGeom < numGeoms; iGeom++)
	{
		GMText* pText = (GMText*)Geoms[iGeom];
		if (pText == NULL) 
			continue;
		if (!mtReader.CheckBars(pText->m_strText))
			continue;

		if (pZone->Contains(&pText->m_Vec))
		{
			pZone->AddText(pText);
			continue;
		}

		GM2DBound bound;
		//pText->Get2DBound(bound);
		// pText 설정이 이상해서, 임시로 bound 객체 생성
		bound.AddVector(&pText->m_Vec);
		double width = pText->m_Origin.Distance(pText->m_Corner);
		bound.Extend(0.5*width, 0.5*pText->m_dHeight);
		//bound.Scale(3.0, 2.0);
		int nPt = bound.ma_Point.GetSize();
		for (int i = 0; i < nPt; i++)
		{
			GM2DVector* corner = bound.ma_Point[i];
			if (pZone->Contains(corner))
			{
				pZone->AddText(pText);
				break;
			}
		}
	}	
}

void CogReader::SortSlabRebarInfoByArea()
{
	int NumInfo = ma_SlabRebarInfo.size();
	for(int iInfo = 0; iInfo < NumInfo; iInfo++)
	{
		GM2DPolyline* pMyPoly = ma_SlabRebarInfo[iInfo]->m_pBoundary;
		double myArea = pMyPoly->Area();

		for(int jInfo = iInfo+1; jInfo < NumInfo; jInfo++)
		{
			GM2DPolyline* pUrPoly = ma_SlabRebarInfo[jInfo]->m_pBoundary;
			double urArea = pUrPoly->Area();
			if(myArea < urArea)
			{
				CR_SlabRebarInfo* pTemp = ma_SlabRebarInfo[iInfo];
				ma_SlabRebarInfo[iInfo] = ma_SlabRebarInfo[jInfo];
				ma_SlabRebarInfo[jInfo] = pTemp;
			}
		}
	}
}

void CogReader::ExtractFloorRebar4UAE1()
{
	MSDeformedBar::SetRebarCode(3);// MSDeformedBar::REBAR_MATERIAL_BS;
	MSDeformedBar::SetNationCode(1); // NATION_CODE_UAE;

	CTypedPtrArray<CObArray, GM2DGeometry*>& RebarLines = m_pQT->ma_GMs[DG_REBARLINE];
	CTypedPtrArray<CObArray, GM2DGeometry*>& RebarInfo = m_pQT->ma_GMs[DG_REBARZONE];
	CTypedPtrArray<CObArray, GM2DGeometry*>& RebarText = m_pQT->ma_GMs[DG_TEXT];

	GM2DLineVector* pLine, *pInfo;

	long nLine = RebarLines.GetSize();
	long nText = RebarText.GetSize();


	for(long iLine=nLine-1; iLine>=0; iLine--)
	{
		BOOL bIntersect = false;
		GM2DGeometry* pGMRebarLine = RebarLines[iLine];
		if(pGMRebarLine->m_ObjType != GT_LINE)	continue;
		pLine = (GM2DLineVector*)pGMRebarLine;
		if(pLine->Length() <= m_MIN_WALL_LENGTH)	continue;

		vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(pLine);

		long NumCrossQT = CrossQTs.size();
		for(long iCrossQT=0; iCrossQT<NumCrossQT; iCrossQT++)
		{
			GMQuadTree* pQT = CrossQTs[iCrossQT];
			CTypedPtrArray<CObArray, GM2DGeometry*>& InfoGMArr = pQT->ma_GMs[DG_REBARZONE];

			long nInfo = InfoGMArr.GetSize();
			for(long iInfo=0; iInfo<nInfo; iInfo++)
			{
				GM2DGeometry* pGMInfoLine = InfoGMArr[iInfo];
				if(pGMInfoLine->m_ObjectType != GT_LINE)	continue;
				pInfo = (GM2DLineVector*)pGMInfoLine;
				if(pInfo->Length() <= m_MIN_WALL_LENGTH)	continue;

				GM2DVector CrossVec;
				double MyLoc, UrLoc;
				bIntersect = pLine->IntersectIn(*pInfo, CrossVec, &MyLoc, &UrLoc);
				if(bIntersect == TRUE)
					break;
			}
			if(bIntersect)
				break;
		}

		if(bIntersect==false)
			continue;

		GMText* pRebarText = FindGMTextSlab(pLine);
		if(pRebarText==NULL)
			continue;

		CString Prefix = MSDeformedBar::GetDefaultRebarNamePrefix();
		pRebarText->m_strText.Replace(Prefix, _T("HD"));

		MSSlabRebar* pSlabRebar = new MSSlabRebar(); //m_IsBasicGrid_for_FloorRebar, m_IsTopBar_for_FloorRebar, m_IsBotBar_for_FloorRebar);
		pSlabRebar->SetFloor(mp_MSFloor);
//		pSlabRebar->m_SBar.InitData(pRebarText->m_strText);
		pSlabRebar->m_RebarLine = *pLine;
		pSlabRebar->m_RebarZone = *pInfo;
		pSlabRebar->SetZoneProfile();
		ma_SlabRebar.Add(pSlabRebar);

		vector<long> FigureIDs;
		AddVdFigureID(FigureIDs, FindGeometyToFigureIDs(pLine));
		AddVdFigureID(FigureIDs, FindGeometyToFigureIDs(pInfo));
		mm_CompoGtoFigureIDs.insert(make_pair(pSlabRebar, FigureIDs));
	}

	MSDeformedBar::SetRebarCode(0);	// REBAR_MATERIAL_KS;
	MSDeformedBar::SetNationCode(0); // NATION_CODE_KOREA;
}

//// MSSlabRebarDwg Methods

MSSlabRebarDwg::~MSSlabRebarDwg()
{
    long nBentLine = ma_BentLine.size();
    for(long iBentLine=0; iBentLine<nBentLine; iBentLine++)
        delete ma_BentLine[iBentLine];
    ma_BentLine.clear();
}

bool MSSlabRebarDwg::AddRebarText( GMText* pText, GM2DLineVector* pLeaderLine )
{
	long nText = ma_RebarText.size();

	if(nText>0)
	{
		GM2DLineVector* pUrLine = ma_LeaderLine[0];
		if(pUrLine->Distance(pLeaderLine->m_P0) > pText->m_dHeight*4.)
			return false;
		if(pUrLine->OverlapTest4DTol(*pLeaderLine, 5., pText->m_dHeight*4.)==FALSE)
			return false;
	}

	GM2DLineVector MyBottomLine;
	pText->GetBottomLine(MyBottomLine);
	for(long iText=0; iText<nText; iText++)
	{
		GMText* pUrText = ma_RebarText[iText];
		GM2DLineVector UrBottomLine;
		pUrText->GetBottomLine(UrBottomLine);
		double Dist = UrBottomLine.Distance(MyBottomLine.m_P0, true);
		if(Dist > 0.)
		{
			ma_RebarText.insert(ma_RebarText.begin()+iText, pText);
			ma_LeaderLine.insert(ma_LeaderLine.begin()+iText, pLeaderLine);
			return true;
		}
	}
	ma_RebarText.push_back(pText);
	ma_LeaderLine.push_back(pLeaderLine);
	return true;
}

void MSSlabRebarDwg::MergeInfoLine( GM2DLineVector* pInfoLine )
{
	if(mp_InfoLine==NULL)
	{
		mp_InfoLine = pInfoLine;
		return;
	}

	double LocValue = mp_InfoLine->OrthoBaseLocValue(pInfoLine->m_P0);
	if(LocValue<-DTOL_GM)
// 		mp_InfoLine->m_P0 = pInfoLine->m_P0;
        mp_InfoLine->m_P0 = mp_InfoLine->PointAtParam(LocValue);
	else if(LocValue>1.+DTOL_GM)
		mp_InfoLine->m_P1 = mp_InfoLine->PointAtParam(LocValue);

	LocValue = mp_InfoLine->OrthoBaseLocValue(pInfoLine->m_P1);
	if(LocValue<-DTOL_GM)
		mp_InfoLine->m_P0 = mp_InfoLine->PointAtParam(LocValue);
	else if(LocValue>1.+DTOL_GM)
		mp_InfoLine->m_P1 = mp_InfoLine->PointAtParam(LocValue);

	delete pInfoLine;
}

bool MSSlabRebarDwg::GetRebarWidth( GM2DLineVector& RebarWidth )
{
	if(ma_RebarLine.size()==0)
		return false;

    if(!this->mp_InfoLine)
        return false;

	RebarWidth.m_P0 = this->mp_InfoLine->PointAtParam(0.5);
	RebarWidth.m_P1 = RebarWidth.m_P0+this->mp_InfoLine->DirVector().NormalVector();

	long nRebarLine = ma_RebarLine.size();
	for(long iRebarLine=0; iRebarLine<nRebarLine; iRebarLine++)
	{
		GM2DPolyline* pPoly = ma_RebarLine[iRebarLine];
		long nPoint = pPoly->GetSize();
		for(long iPoint=0; iPoint<nPoint; iPoint++)
		{
			double LocValue = RebarWidth.OrthoBaseLocValue(*(pPoly->ma_Point[iPoint]));
			if(LocValue<-DTOL_GM)
				RebarWidth.m_P0 = RebarWidth.PointAtParam(LocValue);
			else if(LocValue>1.+DTOL_GM)
				RebarWidth.m_P1 = RebarWidth.PointAtParam(LocValue);
		}
	}
	return true;
}

void MSSlabRebarDwg::AddBentLoc( double BentLoc, GM2DLineVector* pBentLine )
{
	long nBentLine = ma_BentLoc.size();
	for(long iBent=0; iBent<nBentLine; iBent++)
	{
		if(BentLoc<ma_BentLoc[iBent])
		{
			ma_BentLoc.insert(ma_BentLoc.begin()+iBent, BentLoc);
			ma_BentLine.insert(ma_BentLine.begin()+iBent, pBentLine);
			break;
		}
	}
	if(iBent==nBentLine)
	{
		ma_BentLoc.push_back(BentLoc);
		ma_BentLine.push_back(pBentLine);
	}
}

static void AssortRebarTextAndLine(CTypedPtrArray<CObArray, GM2DGeometry*>& RebarInfoArr, CTypedPtrArray<CObArray, GM2DGeometry*>& RebarTextArr, vector<MSSlabRebarDwg*>& SlabRebarDwgArr)
{
	long nText = RebarTextArr.GetSize();
	for(long iText=nText-1; iText>=0; iText--)
	{
		GMText* pText = (GMText*)RebarTextArr[iText];
		if(MSStirrupBar::IsCorrectRebarBySpace(pText->m_strText)==false)
			RebarTextArr.RemoveAt(iText);
	}

	nText = RebarTextArr.GetSize();
	if(nText==0)
		return;

	double TextHeight = ((GMText*)RebarTextArr[0])->m_dHeight;

	long nInfo = RebarInfoArr.GetSize();
	for(long iInfo=nInfo-1; iInfo>=0; iInfo--)
	{
		if(RebarInfoArr[iInfo]->m_ObjType !=GT_LINE)
			continue;
		GM2DLineVector* pLine = (GM2DLineVector*)RebarInfoArr[iInfo];
		double LineDeg = pLine->GetDegree();

		GM2DRectangle TextBound;
		for(long iText=0; iText<nText; iText++)
		{
			GMText* pText = (GMText*)RebarTextArr[iText];
			GM2DLineVector LeftLine, RightLine, BotLine, TopLine;
			pText->GetBoundLines(LeftLine, RightLine, BotLine, TopLine);
			if(pLine->IsParallel4DTol(BotLine, 5.)==0)
				continue;

			double LocValue = LeftLine.OrthoBaseLocValue(pLine->m_P0);
			if(LocValue<-DTOL_GM || LocValue>1.+DTOL_GM)
				continue;

			double Dist = min(LeftLine.Distance(pLine->m_P0), RightLine.Distance(pLine->m_P0));
			Dist = min(Dist, LeftLine.Distance(pLine->m_P1));
			Dist = min(Dist, RightLine.Distance(pLine->m_P1));

			if(Dist > pText->m_dHeight*3.)
				continue;

			long nRebarDWG = SlabRebarDwgArr.size();
			for(long iRebarDWG=0; iRebarDWG<nRebarDWG; iRebarDWG++)
			{
				MSSlabRebarDwg* pDWG = SlabRebarDwgArr[iRebarDWG];
				if(pDWG->AddRebarText(pText, pLine))
					break;
			}
			if(iRebarDWG==nRebarDWG)
			{
				MSSlabRebarDwg* pDWG = new MSSlabRebarDwg;
				SlabRebarDwgArr.push_back(pDWG);
				pDWG->AddRebarText(pText, pLine);
			}
			break;
		}

		if(iText<nText)
			RebarInfoArr.RemoveAt(iInfo);
	}
}

static void FindMainLeaderLine(CTypedPtrArray<CObArray, GM2DGeometry*>& RebarInfoArr, vector<MSSlabRebarDwg*>& SlabRebarDwgArr)
{
	long nInfo = RebarInfoArr.GetSize();
	for(long iInfo=nInfo-1; iInfo>=0; iInfo--)
	{
		if(RebarInfoArr[iInfo]->m_ObjType!=GT_LINE)
			continue;
		GM2DLineVector* pLine = (GM2DLineVector*)RebarInfoArr[iInfo];
		long nDWG = SlabRebarDwgArr.size();
		for(long iDWG=0; iDWG<nDWG; iDWG++)
		{
			MSSlabRebarDwg* pDWG = SlabRebarDwgArr[iDWG];
			long nTextLine = pDWG->ma_LeaderLine.size();
			for(long iTextLine=0; iTextLine<nTextLine; iTextLine++)
			{
				GM2DLineVector* pTextLine = pDWG->ma_LeaderLine[iTextLine];
				GM2DVector CrossVec;
				double MyLoc, UrLoc;
				if(pLine->IntersectIn(*pTextLine, CrossVec, &MyLoc, &UrLoc)==FALSE)
					break;
			}
			if(iTextLine<nTextLine)
				continue;
			pDWG->mp_LeaderLine = pLine;
			RebarInfoArr.RemoveAt(iInfo);
			break;
		}
	}
}

static void FindRebarLine(CTypedPtrArray<CObArray, GM2DGeometry*>& RebarInfoArr, vector<MSSlabRebarDwg*>& SlabRebarDwgArr, vector<GM2DPolyline*>& RebarLineArr)
{
	long nInfo = RebarInfoArr.GetSize();
	for(long iInfo=nInfo-1; iInfo>=0; iInfo--)
	{
		if(RebarInfoArr[iInfo]->m_ObjType!=GT_CIRCLE)
			continue;
		GM2DCircle* pCircle = (GM2DCircle*)RebarInfoArr[iInfo];
		long nDWG = SlabRebarDwgArr.size();
		for(long iDWG=0; iDWG<nDWG; iDWG++)
		{
			MSSlabRebarDwg* pDWG = SlabRebarDwgArr[iDWG];
			GM2DVector CrossVec1, CrossVec2;
			if(pDWG->mp_LeaderLine->Intersect(*pCircle, CrossVec1, CrossVec2, TRUE, 1.)>0)
			{
				long nRebarLine = RebarLineArr.size();
				for(long iRebarLine=nRebarLine-1; iRebarLine>=0; iRebarLine--)
				{
					GM2DPolyline* pPoly = (GM2DPolyline*)RebarLineArr[iRebarLine];
					if(pPoly->IntersectTest(*pCircle))
					{
						pDWG->ma_RebarPointer.push_back(pCircle);
						RebarInfoArr.RemoveAt(iInfo);
						pDWG->ma_RebarLine.push_back(pPoly);
						RebarLineArr.erase(RebarLineArr.begin()+iRebarLine);
						break;
					}
				}
				if(iRebarLine>=0)
					break;
			}
		}
	}
}

static void FindRebarInfoLine(CTypedPtrArray<CObArray, GM2DGeometry*>& RebarInfoArr, vector<MSSlabRebarDwg*>& SlabRebarDwgArr, vector<GM2DPolyline*>& RebarLineArr)
{
	long nInfo = RebarInfoArr.GetSize();
	for(long iInfo=nInfo-1; iInfo>=0; iInfo--)
	{
		if(RebarInfoArr[iInfo]->m_ObjType!=GT_CIRCLE)
			continue;
		GM2DCircle* pCircle = (GM2DCircle*)RebarInfoArr[iInfo];
		long nDWG = SlabRebarDwgArr.size();
		for(long iDWG=0; iDWG<nDWG; iDWG++)
		{
			MSSlabRebarDwg* pDWG = SlabRebarDwgArr[iDWG];
			long nRebarLine = pDWG->ma_RebarLine.size();
			for(long iRebarLine=0; iRebarLine<nRebarLine; iRebarLine++)
			{
				GM2DPolyline* pPoly = (GM2DPolyline*)pDWG->ma_RebarLine[iRebarLine];
				if(pPoly->IntersectTest(*pCircle))
				{
					pDWG->ma_InfoPointer.push_back(pCircle);
					RebarInfoArr.RemoveAt(iInfo);
					break;
				}
			}
			if(iRebarLine<nRebarLine)
				break;
		}
	}

	nInfo = RebarInfoArr.GetSize();
	for(long iInfo=nInfo-1; iInfo>=0; iInfo--)
	{
		if(RebarInfoArr[iInfo]->m_ObjType!=GT_LINE)
			continue;
		GM2DLineVector* pInfoLine = (GM2DLineVector*)RebarInfoArr[iInfo];
		long nDWG = SlabRebarDwgArr.size();
		for(long iDWG=0; iDWG<nDWG; iDWG++)
		{
			MSSlabRebarDwg* pDWG = SlabRebarDwgArr[iDWG];
			long nInfoPoint = pDWG->ma_InfoPointer.size();
			for(long iInfoPointer=0; iInfoPointer<nInfoPoint; iInfoPointer++)
			{
				GM2DCircle* pInfoPointer = (GM2DCircle*)pDWG->ma_InfoPointer[iInfoPointer];
				if(pInfoLine->IntersectTest(*pInfoPointer))
				{
					pDWG->MergeInfoLine(pInfoLine);
					RebarInfoArr.RemoveAt(iInfo);
					break;
				}
			}
			if(iInfoPointer<nInfoPoint)
				break;
		}
	}
}

static void Merge(vector<MSSlabRebarDwg*>& SlabRebarDwgArr)
{
	long nDWG = SlabRebarDwgArr.size();
	for(long iDWG=0; iDWG<nDWG; iDWG++)
	{
		MSSlabRebarDwg* pDWG = SlabRebarDwgArr[iDWG];
		if(pDWG->m_bMerged)
			continue;;
		for(long jDWG=iDWG+1; jDWG<nDWG; jDWG++)
		{
			MSSlabRebarDwg* pUrDWG = SlabRebarDwgArr[jDWG];
			if(pUrDWG->m_bMerged)
				continue;

			long Result = pDWG->mp_InfoLine->Overlap4DTol(*(pUrDWG->mp_InfoLine), 1., 1.E10, 10.);
			if(Result!=9 && Result!=17)
				continue;
			
			GM2DLineVector MyRebarWidth, UrRebarWidth;
			pDWG->GetRebarWidth(MyRebarWidth);
			pUrDWG->GetRebarWidth(UrRebarWidth);
			Result = MyRebarWidth.Overlap4DTol(UrRebarWidth, 1., 1.E10, 10.);
			if (Result == 8 || Result == 12 || Result == 13 || Result == 14 || Result == 18)// My가 Ur를 포함 또는 일치
			{
				pDWG->ma_Merged.push_back(pUrDWG);
				pUrDWG->m_bMerged = true;
			}
			else if (Result == 4 || Result == 5 || Result == 10 || Result == 16 || Result == 21 || Result == 22 || Result == 9 || Result == 17) // Ur가 My를 포함
			{
				pUrDWG->ma_Merged.push_back(pDWG);
				pDWG->m_bMerged = true;
				long nMerged = pDWG->ma_Merged.size();
				for(long iMergede=0; iMergede<nMerged; iMergede++)
					pUrDWG->ma_Merged.push_back(pDWG->ma_Merged[iMergede]);
			}
		}
	}
}

void CogReader::ExtractFloorRebar4GS1()
{
	MSDeformedBar::SetRebarCode(0);	// REBAR_MATERIAL_KS;
	MSDeformedBar::SetNationCode(0); // NATION_CODE_KOREA;

	CTypedPtrArray<CObArray, GM2DGeometry*>& RebarInfo = m_pQT->ma_GMs[DG_REBARZONE];
	CTypedPtrArray<CObArray, GM2DGeometry*>& RebarText = m_pQT->ma_GMs[DG_TEXT];

	vector<MSSlabRebarDwg*> SlabRebarDwgArr;
	//vector<GM2DPolyline*> RebarPolyArr;

	vector<GM2DPolyline*> BentLineArr;
	FindJoinedPolylines(DG_REBARLINE, BentLineArr);

	AssortRebarTextAndLine(RebarInfo, RebarText, SlabRebarDwgArr);
	FindMainLeaderLine(RebarInfo, SlabRebarDwgArr);
	FindRebarLine(RebarInfo, SlabRebarDwgArr, BentLineArr);
	FindRebarInfoLine(RebarInfo, SlabRebarDwgArr, BentLineArr);

	Merge(SlabRebarDwgArr);

    // Find BentLines & Add them to crossed RebarZones
	CTypedPtrArray<CObArray, GM2DGeometry*>& BentLine = m_pQT->ma_GMs[DG_GRIDLINE];
	GM2DLineVector RebarWidth;
	GM2DVector CrossVec;
	double MyLoc, UrLoc;
	long nBentLine = BentLine.GetSize();
	long nDWG = SlabRebarDwgArr.size();
	for(long iDWG=0; iDWG<nDWG; iDWG++)
	{
		MSSlabRebarDwg* pDWG = SlabRebarDwgArr[iDWG];
		pDWG->GetRebarWidth(RebarWidth);
		for(long iBentLine=0; iBentLine<nBentLine; iBentLine++)
		{
			GM2DLineVector* pBentLine = (GM2DLineVector*)BentLine[iBentLine];
			if(pBentLine->IntersectIn(RebarWidth, CrossVec, &MyLoc, &UrLoc)==FALSE)
				continue;

            GM2DVector temp0 = pBentLine->OrthoBaseVector(pDWG->mp_InfoLine->m_P0);
            GM2DVector temp1 = pBentLine->OrthoBaseVector(pDWG->mp_InfoLine->m_P1);
            
            pDWG->AddBentLoc(UrLoc, new GM2DLineVector(temp0, temp1));
		}
	}

    // Insert FloorRebarDwg - later, Change this with Command
    for(long iDWG=0; iDWG<nDWG; iDWG++)
    {
        MSSlabRebarDwg* pDWG = SlabRebarDwgArr[iDWG];
        if (!pDWG->m_bMerged)
            InsertSlabRebarDWG(pDWG);
        delete pDWG;
    }
    		
    nBentLine = BentLineArr.size();
    for(long iBentLine=0; iBentLine<nBentLine; iBentLine++)
    {
        BentLineArr[iBentLine]->FreeAll();
        delete BentLineArr[iBentLine];
    }


/*



// 	double Temp_DTOL_DIST_COLINEAR = m_DTOL_DIST_COLINEAR;
// 	m_DTOL_DIST_COLINEAR = 81.;
// 	MergeGMLineSegments(DG_REBARINFO);
// 	m_DTOL_DIST_COLINEAR = Temp_DTOL_DIST_COLINEAR;


	vector<GMCircle*> InforPointerArr;
	vector<GMCircle*> RebarPointerArr;

	double InforPointerDia = 85.;
	double RebarPointerDia = 65.;

	long nInfo = RebarInfo.GetSize();
	for(long iInfo=nInfo-1; iInfo>=0; iInfo++)
	{
		GM2DGeometry* pGM = RebarInfo[iInfo];
		if(pGM->GetObjectType()==GT_CIRCLE)
		{
			GMCircle* pCircle = (GMCircle*)pGM;
			if(pCircle->m_A < RebarPointerDia)
				RebarPointerArr.push_back(pCircle);
			else
				InforPointerArr.push_back(pCircle);
			RebarPointerArr.erase(RebarPointerArr.begin()+iInfo);
		}
	}

	GM2DLineVector* pInfoLine, *pInfo;

	long nInfoLine = RebarInfo.GetSize();
	long nText = RebarText.GetSize();
	
	for(long iInfoLine=0; iInfoLine<nInfoLine; iInfoLine++)
	{
		bool bIntersect = false;
		vector<GMCircle*> CrossInforPointerArr;

		pInfoLine = (GM2DLineVector*)RebarInfo[iInfoLine];

		long nInforPointer = InforPointerArr.size();
		for(long iInforPoint=nInforPointer-1; iInforPoint>=0; iInforPoint--)
		{
			GM2DVector CrossVec1, CrossVec2;
			if(pInfoLine->Intersect(*InforPointerArr[iInforPoint], CrossVec1, CrossVec2) == 2)
			{
				CrossInforPointerArr.push_back(InforPointerArr[iInforPoint]);
				CrossInforPointerArr.erase(InforPointerArr.begin()+iInforPoint);
			}
		}

		long nCrossRebar = CrossInforPointerArr.size();
		for(long iCrossRebar=0; iCrossRebar<nCrossRebar; iCrossRebar++)
		{
			long nRebarLine = RebarLines.GetSize();
// 			for(long iRebarLine=nRebarLine-1; iRebarLine>=0; iRebarLine--)

		}

		CTypedPtrArray<CObArray, GMQuadTree*>& CrossQTs = pInfoLine->m_CrossQTs;

		long NumCrossQT = CrossQTs.GetSize();
		for(long iCrossQT=0; iCrossQT<NumCrossQT; iCrossQT++)
		{
			GMQuadTree* pQT = CrossQTs[iCrossQT];
			CTypedPtrArray<CObArray, GM2DGeometry*>& InfoGMArr = pQT->ma_GMs[DG_REBARINFO];

			long nInfo = InfoGMArr.GetSize();
			for(long iInfo=0; iInfo<nInfo; iInfo++)
			{
				pInfo = (GM2DLineVector*)InfoGMArr[iInfo];
				GM2DVector CrossVec;
				double MyLoc, UrLoc;
				bIntersect = pInfoLine->IntersectIn(*pInfo, CrossVec, &MyLoc, &UrLoc);
				if(bIntersect)
					break;
			}
			if(bIntersect)
				break;
		}

		if(bIntersect==false)
			continue;

		GMText* pRebarText = FindGMTextSlab(pInfoLine);
		if(pRebarText==NULL)
			continue;

		CString Prefix = MSDeformedBar::GetDefaultRebarNamePrefix();
		pRebarText->m_strText.Replace(Prefix, _T("HD"));

		MSSlabRebar* pSlabRebar = new MSSlabRebar(m_IsTopBar_for_FloorRebar, m_IsBotBar_for_FloorRebar, m_IsBasicGrid_for_FloorRebar);
		pSlabRebar->SetFloor(mp_MSFloor);
		pSlabRebar->m_SBar.InitData(pRebarText->m_strText);
		pSlabRebar->m_RebarLine = *pInfoLine;
		pSlabRebar->m_RebarZone = *pInfo;
		pSlabRebar->SetZoneProfile();
		ma_SlabRebar.Add(pSlabRebar);

		vector<long> FigureIDs;
		AddVdFigureID(pInfoLine, FigureIDs);
		AddVdFigureID(pInfo, FigureIDs);
		mm_CompoGtoFigureIDs.insert(make_pair(pSlabRebar, FigureIDs));
	}
*/
}

void CogReader::InsertSlabRebarDWG( MSSlabRebarDwg* pDWG )
{
    long nText = pDWG->ma_RebarText.size();
    long nLine = pDWG->ma_RebarLine.size();
    if (nLine != nText)
        return;

    MSSlabRebar* pRebar = (MSSlabRebar*)m_pBldg->CreateNewSlabRebar();

    // set boundary
    pRebar->m_RebarZone.m_P0 = pDWG->mp_InfoLine->m_P0;
    pRebar->m_RebarZone.m_P1 = pDWG->mp_InfoLine->m_P1;
    pDWG->GetRebarWidth(pRebar->m_RebarLine);
    pRebar->SetZoneProfile();

    SetSlabRebarInfo(pDWG, pRebar, false);

    long nMerged = pDWG->ma_Merged.size();
    for (long iMerged = 0; iMerged<nMerged; iMerged++)
    {
        MSSlabRebarDwg* pMerged = pDWG->ma_Merged[iMerged];
        nText = pMerged->ma_RebarText.size();
        nLine = pMerged->ma_RebarLine.size();
        if (nLine != nText)
            continue;
        SetSlabRebarInfo(pMerged, pRebar, true);
    }

    this->ma_SlabRebar.Add(pRebar);
}

void CogReader::SetSlabRebarInfo(MSSlabRebarDwg* pDWG, MSSlabRebar* pRebar, bool bMerged )
{
    // Make RebarLine
    long nBent = pDWG->ma_BentLine.size();
    for(long iBent = 0; iBent<nBent; iBent++)
        pRebar->ma_BentCutLines.push_back((GM2DLineVector*)(pDWG->ma_BentLine[iBent]->Copy()));

    double length = pRebar->m_RebarLine.Length();
    long nText = pDWG->ma_RebarText.size();
    for (long iText = 0; iText<nText; iText++)
    {
        //Check Type
        CString text = pDWG->ma_RebarText[iText]->m_strText;

        // function
        MSSlabRebarInfo::RebarType rbType = GetFloorRebarLineType(text);

        //Get RebarText
        MSStirrupBar rebar;
		rebar.InitData(text);
        bool bCut = false;
        if (pDWG->ma_RebarLine[iText]->GetLength() < length - 100) // temp
            bCut = true;
        
        // Make RebarLine
        MSSlabRebarInfo* pRebarInfo= new MSSlabRebarInfo(rbType);
//         pRebarLine->mp_RebarLine->CopyFrom(pDWG->ma_RebarLine[iText]);

        pRebar->SetRebarInfo(pRebarInfo);
    }

    if (!bMerged)        
    {
        int nBentLoc = pDWG->ma_BentLoc.size();
        pRebar->ma_BentLocValue.resize(nBentLoc);
        for(int iLoc = 0; iLoc<nBentLoc; iLoc++)
            pRebar->ma_BentLocValue[iLoc] = pDWG->ma_BentLoc[iLoc];
    }
}

MSSlabRebarInfo::RebarType CogReader::GetFloorRebarLineType( CString text)
{
	MSSlabRebarInfo::RebarType rbType = MSSlabRebarInfo::TopX;
    int index = text.Find(_T("(B)"));
    if (index>-1)
        rbType = MSSlabRebarInfo::BottomX;
    else
    {
        index = text.Find(_T("BENT"));
        if (index>-1)
            rbType = MSSlabRebarInfo::Bent;
    }
    return rbType;
}

void CogReader::FindJoinedPolylines(long LayerNum, vector<GM2DPolyline*>& BentLineArr)
{
    CTypedPtrArray<CObArray, GM2DGeometry*>& GMLines = m_pQT->ma_GMs[LayerNum];
    CTypedPtrArray<CObArray, GM2DCurve*> Segments;

    long NumObj = GMLines.GetSize();
    for(long ObjNum=0; ObjNum<NumObj; ObjNum++)
    {
        GM2DGeometry* AObj = GMLines[ObjNum];
        if(AObj == NULL || AObj->m_Tag != 0)  continue;
        if(AObj->m_ObjType != GT_LINE) continue;
        Segments.Add((GM2DLineVector*)AObj);
    }
    
    JoinCurves(BentLineArr, Segments);
}

void CogReader::JoinCurves( vector<GM2DPolyline*>& JoinedPolys, CTypedPtrArray<CObArray, GM2DCurve*>& Segments )
{
    if (Segments.GetSize()==0)
        return;

    GM2DPolyline* joined = new GM2DPolyline();
    //joined->Add(&Segments[0]->m_P0);

    int iSeg = 0;
    do
    {
        if(joined->AddSegment(joined, Segments[iSeg]))
        {
            Segments.RemoveAt(iSeg);
            iSeg =0;
        }
        else
            iSeg++;
    }
    while(iSeg<Segments.GetSize());

    if (joined->GetSize()>1)
        JoinedPolys.push_back(joined);
    if (Segments.GetSize()>0)
        JoinCurves(JoinedPolys, Segments);
}

void CogReader::MergeGMPolyline(long LayerNum, vector<GM2DPolyline*>& BentLineArr)
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
		GM2DPolyline* pBentLine = FindGMPolyline(LayerNum, ALineObj);
		if(pBentLine)
			BentLineArr.push_back(pBentLine);
	}

	m_pQT->RemoveGMsByTag(LayerNum, -1);

	NumObj = GMLines.GetSize();
	for(long ObjNum=0; ObjNum<NumObj; ObjNum++)
		GMLines[ObjNum]->m_Tag=0;
}

GM2DPolyline* CogReader::FindGMPolyline(long LayerNum, GM2DLineVector* AGmLine)
{
	GM2DPolyline* pNewPoly = new GM2DPolyline(TRUE);
	pNewPoly->AddCopy(AGmLine->m_P0);
	pNewPoly->AddCopy(AGmLine->m_P1);

	CMap<GM2DLineVector*, GM2DLineVector*, GM2DLineVector*, GM2DLineVector*> ChkLVecMap;

	vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(AGmLine);
	long NumQT = CrossQTs.size();
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

				long nVertex = pNewPoly->GetSize();
				if(pNewPoly->LastPnt().Distance(ALineObj->m_P0) < m_DTOL_DIST_LINKTEST)
					pNewPoly->AddCopy(ALineObj->m_P1);
				else if(pNewPoly->LastPnt().Distance(ALineObj->m_P1) < m_DTOL_DIST_LINKTEST)
					pNewPoly->AddCopy(ALineObj->m_P0);
				else if(pNewPoly->FirstPnt().Distance(ALineObj->m_P0) < m_DTOL_DIST_LINKTEST)
					pNewPoly->InsertAtCopy(0, ALineObj->m_P1);
				else if(pNewPoly->FirstPnt().Distance(ALineObj->m_P1) < m_DTOL_DIST_LINKTEST)
					pNewPoly->InsertAtCopy(0, ALineObj->m_P0);
				if(pNewPoly->GetSize() > nVertex)
					ALineObj->m_Tag = -1;
			}
		}
	}

    long nPoint = pNewPoly->ma_Point.GetSize();
    pNewPoly->ma_Bulge.SetSize(nPoint-1);
    for(long iPoint=0; iPoint<nPoint-1; iPoint++)
        pNewPoly->ma_Bulge[iPoint] = 0.0;

	return pNewPoly;

// 	if(pNewPoly->GetSize()>2)
// 		return pNewPoly;
// 
// 	delete pNewPoly;
// 	return NULL;
}

//////////////////////////////////////////////////////////////////////  CR_SlabRebarInfo /////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CR_SlabRebarInfo::CR_SlabRebarInfo()
{
	m_pBoundary = NULL;
	Init();
}

void CR_SlabRebarInfo::SetBoundary(GM2DPolyline* pPoly)
{
	m_pBoundary = pPoly;
}

void CR_SlabRebarInfo::SetFigureID(int figureID)
{
	m_FigureID = figureID;
}

CR_SlabRebarInfo::~CR_SlabRebarInfo()
{
	if(m_pBoundary)
		delete m_pBoundary;
	int numText = ma_RebarText.size();
	for(int iText = 0; iText < numText; iText++)
		delete ma_RebarText[iText];
	ma_RebarText.clear();

	DeleteInfoLines();
}

void CR_SlabRebarInfo::DeleteInfoLines()
{
	int nSize = ma_InfoLine.size();
	for(int i= 0; i< nSize; i++)
		delete ma_InfoLine[i];
	ma_InfoLine.clear();

}

void CR_SlabRebarInfo::Init()
{
	ma_RebarLine.clear();
	ma_CircleText.clear();
	ma_CircleZone.clear();
	ma_LineVec.clear();
	ma_Circles.clear();
	ma_Text.clear();
}

int CR_SlabRebarInfo::AddRebarLine(GM2DLineVector* pRebarLine)
{
	ma_RebarLine.push_back(pRebarLine);
	return (int)ma_RebarLine.size();
}

int CR_SlabRebarInfo::AddLineVector(GM2DLineVector* pLineVec)
{
	ma_LineVec.push_back(pLineVec);
	return (int)ma_RebarLine.size();
}

int CR_SlabRebarInfo::AddCircle(GM2DCircle* pCircle)
{
	ma_Circles.push_back(pCircle);
	return (int)ma_Circles.size();
}

int CR_SlabRebarInfo::AddText(GMText* pText)
{
	ma_Text.push_back(pText);
	return (int)ma_Text.size();
}

void CR_SlabRebarInfo::SetSlabRebarInfo(GM2DCircle* pCircle)
{

}

bool CR_SlabRebarInfo::Contains(GM2DLineVector* pLineVec)
{
	return this->m_pBoundary->Contains(&pLineVec->MassCenter()) < 0;
}

bool CR_SlabRebarInfo::Contains(GM2DCircle* pCircle)
{
	return this->m_pBoundary->Contains(&pCircle->m_Center) < 0;
}

bool CR_SlabRebarInfo::Contains(GM2DVector* pVec)
{
	return this->m_pBoundary->Contains(pVec) < 0;
}

bool CR_SlabRebarInfo::CheckIntersect(GM2DCircle* pCircle, GM2DLineVector* pLine)
{
	GM2DVector CrossVec1, CrossVec2;
	return pLine->Intersect(*pCircle, CrossVec1, CrossVec2, TRUE, 1.) > 0;
}

// RebarLine 유효성 검토
bool CR_SlabRebarInfo::ValidateRebarLine(GM2DLineVector* pLine)
{
	return MatchEndPointWithBoundary(pLine);
}

// 배근기준선과 교차여부 체크
void CR_SlabRebarInfo::ValidateCircles()
{
	int nRebar = this->ma_RebarLine.size();
	for (int iCircle = 0; iCircle < ma_Circles.size(); iCircle++)
	{
		GM2DCircle* pCircle = ma_Circles[iCircle];
		for (int iRebar = 0; iRebar < nRebar; iRebar++)
		{
			GM2DLineVector* pLine = ma_RebarLine[iRebar];
			if (CheckIntersect(pCircle, pLine))
			{
				pCircle->m_Tag = 314+iRebar;	// m_Tag의 기본값은 0임, iRebar가 0인 경우 고려
				break;
			}
		}
		if (pCircle->m_Tag > 0)
			pCircle->m_Tag -= 314;	// 교차하는 배근선의 인덱스 부여
		else // 교차하는 배근선이 없으면 유효하지 않으므로 버림
		{
			ma_Circles.erase(ma_Circles.begin() + iCircle);
			iCircle--;
		}
	}
}

// 영역 안에 들어온 기타선 찾기 + 배근영역선과 텍스트 연결선 구분
void CR_SlabRebarInfo::ValidateLines()
{
	// 선 붙이기
	// 하나의 라인으로 보이지만.. 두 개 이상인 경우 많음
	// 원을 경계로 떨어져있을 수도 있음
	// TEXT와 관계된 선은 꺾여있는 경우 있음
	FilterLines();
	FindInfoLine();
}

// RebarLine 과 평행하거나 수직한 선들만 걸러냄
void CR_SlabRebarInfo::FilterLines()
{
	vector<GM2DVector> vectors;
	int nRebar = this->ma_RebarLine.size();
	for (int iRebar = 0; iRebar < nRebar; iRebar++)
	{
		GM2DVector dirVec = ma_RebarLine[iRebar]->DirVector();
		vectors.push_back(dirVec);	// 평행 판단 벡터
		vectors.push_back(dirVec.NormalVector());	// 수직 판단 벡터
	}

	int nVec = vectors.size();
	for (int iLine = 0; iLine < ma_LineVec.size(); iLine++)
	{
		bool parallel = false;
		GM2DLineVector* pLine = ma_LineVec[iLine];
		for (int iVec = 0; iVec < nVec; iVec++)
		{
			GM2DVector vec = vectors[iVec];
			if (pLine->DirVector().IsParallel(vec) != 0)
			{
				parallel = true;
				break;
			}
		}
		if (!parallel)
		{
			ma_LineVec.erase(ma_LineVec.begin() + iLine);
			iLine--;
		}
	}
}

void CR_SlabRebarInfo::FindInfoLine()
{
	GM2DLineVector* lineVec = NULL;
	vector<GM2DLineVector*> lines;
	int nLines = ma_LineVec.size();

	for (int iCircle = 0; iCircle < ma_Circles.size(); iCircle++)
	{
		GM2DCircle* pCircle = ma_Circles[iCircle];
		// 원과 겹치는 선 찾기
		for (int iLine = 0; iLine < nLines; iLine++)
		{
			GM2DLineVector* pLine = ma_LineVec[iLine];
			if (CheckIntersect(pCircle, pLine))
			{
				lineVec = pLine;
				break;
			}
		}
		if(lineVec == NULL)	continue;
		// 연결 가능한 선 찾기
		for (int iLine = 0; iLine < nLines; iLine++)
		{
			GM2DLineVector* pLine = ma_LineVec[iLine];
			if (lineVec->IsColinear(*pLine)!=0) // 1또는 -1이 Colinear
				lines.push_back(pLine);
		}
		// 선 이어 붙이고 경계 선과 끝 맞추기
		if (lines.size() >0)
		{
			GM2DLineVector* pLine = JoinLines(lines);
			if (MatchEndPointWithBoundary(pLine))
			{
				pLine->m_Tag = pCircle->m_Tag;
				SetInfoCircle(iCircle);
				SetInfoLine(pLine);
				iCircle--;
			}
			delete pLine;
		}
		lines.clear();
	}
}

// 끝점 맞추기
bool CR_SlabRebarInfo::MatchEndPointWithBoundary(GM2DLineVector* pLine)
{
	double length = pLine->Length();
	GM2DLineVector LineCopy;
	LineCopy.CopyFrom(pLine);
	LineCopy.ExtendLineVector(true, 10 * length, false);
	LineCopy.ExtendLineVector(false, 10 * length, true);
	vector<GM2DVector> crossVecs = m_pBoundary->IntersectEx(&LineCopy);
	if (crossVecs.size() == 2)
	{
		double dist = crossVecs[0].Distance(crossVecs[1]);
		if (dist*0.8 < length&&length < dist*1.2)
		{
			// 필요한 경우 조건 추가 (끝점에서의 거리 등)
			pLine->SetP0(crossVecs[0]);
			pLine->SetP1(crossVecs[1]);
			return true;
		}
	}
	return false;
}

void CR_SlabRebarInfo::FindTextLine()
{
	GM2DLineVector* lineVec = NULL;
	vector<GM2DLineVector*> coLines;
	vector<GM2DLineVector*> perLines;
	
	int nLines = ma_LineVec.size();
	for (int iCircle = 0; iCircle < ma_Circles.size(); iCircle++)
	{
		coLines.clear();
		perLines.clear();

		bool CircleUsed = false;
		GM2DCircle* pCircle = ma_Circles[iCircle];
		// 원과 겹치는 선 찾기
		for (int iLine = 0; iLine < nLines; iLine++)
		{
			GM2DLineVector* pLine = ma_LineVec[iLine];
			if (CheckIntersect(pCircle, pLine))
			{
				lineVec = pLine;
				break;
			}
		}
		if(lineVec == NULL)	continue;
		// 연결 가능한 선 찾기
		for (int iLine = 0; iLine < nLines; iLine++)
		{
			GM2DLineVector* pLine = ma_LineVec[iLine];
			if (lineVec->IsColinear(*pLine) != 0) // 1또는 -1이 Colinear
				coLines.push_back(pLine);
		}
		// 수직으로 만나는 선 찾기
		// 기울어진 선을 찾을 일이 생기면 소스 조정
		for (int iLine = 0; iLine < nLines; iLine++)
		{
			GM2DLineVector* pLine = ma_LineVec[iLine];
			if (lineVec->IsPerpendicular(*pLine))
				perLines.push_back(pLine);
		}
		// 선 이어 붙이기
		if (coLines.size() >0)
		{
			GM2DLineVector* pLeadLine = JoinLines(coLines);
			int nPLines = perLines.size();
			for (int iPLine = 0; iPLine < nPLines; iPLine++)
			{
				GM2DLineVector* pCurrent = perLines[iPLine];
				
				GM2DVector CrossVec;
				double MyLoc, UrLoc;
				if (1 != pCurrent->IntersectEndWithLine(*pLeadLine, CrossVec, &MyLoc, &UrLoc))
					continue;
				
				// 텍스트 찾기
				GM2DVector APoint = pCurrent->m_P1;
				if (MyLoc>0.5)
					APoint = pCurrent->m_P0;
				if (FindTextAndSet(APoint, pCircle->m_Tag))
					CircleUsed = true;
			}

			if (!CircleUsed)
			{
				// 텍스트 찾기
				GM2DVector APoint = pLeadLine->m_P0;
				if (FindTextAndSet(APoint, pCircle->m_Tag))
					CircleUsed = true;
				else
				{
					APoint = pLeadLine->m_P1;
					if (FindTextAndSet(APoint, pCircle->m_Tag))
						CircleUsed = true;
				}
			}
			delete pLeadLine;
		}
		if (CircleUsed)
		{
			SetTextCircle(iCircle);
			iCircle--;
		}
	}
}

bool CR_SlabRebarInfo::FindTextAndSet(GM2DVector APoint, int iRebar)
{
	int nText = ma_Text.size();
	for (int iText = 0; iText < nText; iText++)
	{
		GMText* pText = ma_Text[iText];
		GM2DBound bound;
		//pText->Get2DBound(bound);
		// pText 설정이 이상해서, 임시로 bound 객체 생성
		bound.AddVector(&pText->m_Vec);
		double width = pText->m_Origin.Distance(pText->m_Corner);
		bound.Extend(0.5*width, 0.5*pText->m_dHeight);
		bound.Scale(1.2,1.1);
		if (bound.Contains(&APoint))
		{
			GMText* pCopy = new GMText(*pText);
			pCopy->m_Tag = iRebar;
			ma_RebarText.push_back(pCopy);
			return true;
		}
	}
	return false;
}

void CR_SlabRebarInfo::SetInfoLine(GM2DLineVector* pLine)
{
	GM2DLineVector* pCopy = new GM2DLineVector();
	pCopy->CopyFrom(pLine);
	pCopy->m_Tag = pLine->m_Tag;
	ma_InfoLine.push_back(pCopy);
}

void CR_SlabRebarInfo::SetInfoCircle(int iCircle)
{
	ma_CircleZone.push_back(ma_Circles[iCircle]);
	ma_Circles.erase(ma_Circles.begin() + iCircle);
}

void CR_SlabRebarInfo::SetTextCircle(int iCircle)
{
	ma_CircleText.push_back(ma_Circles[iCircle]);
	ma_Circles.erase(ma_Circles.begin() + iCircle);
}

// 같은 선상에 있는 라인들 합치기
// 필요한 경우 간격 체크 추가
GM2DLineVector* CR_SlabRebarInfo::JoinLines(vector<GM2DLineVector*> lines)
{
	GM2DLineVector* pLine = new GM2DLineVector;
	if (lines.size() < 1) return pLine;
	pLine->CopyFrom(lines[0]);
	vector<double> locValues;
	int nLines = lines.size();
	for (int iLine = 0; iLine < nLines; iLine++)
	{
		GM2DLineVector* curLine = lines[iLine];
		locValues.push_back(pLine->OrthoBaseLocValue(curLine->m_P0));
		locValues.push_back(pLine->OrthoBaseLocValue(curLine->m_P1));
	}
	sort(locValues.begin(), locValues.end());
	pLine->SetP0(pLine->PointAtParam(locValues[0]));
	pLine->SetP1(pLine->PointAtParam(locValues[locValues.size() - 1]));
	return pLine;
}

int CR_SlabRebarInfo::MakeSlabRebars()
{
	int nText = ma_RebarText.size();
	for (int iText=0; iText < nText; iText++)
	{
		GMText* pText = ma_RebarText[iText];
		int rebarIndex = pText->m_Tag;
		if (rebarIndex >= ma_RebarLine.size())
			continue;
		GM2DLineVector* pInfoLine = FindRebarInfoLine(rebarIndex); // T/B 가 인식되면 vector로 받아야 함
		if (pInfoLine == NULL)
			continue;

		GM2DLineVector* pRebarLine = ma_RebarLine[rebarIndex];
		//pRebarLine->m_Tag = 63;		// 사용한 선임을 표시
		bool xDir = IsXDirection(pRebarLine);

		CString text = pText->m_strText;
		int indexT = text.Find(_T("T"));
		int indexB = text.Find(_T("B"));
		MSSlabRebarInfo::RebarType rbType;

		if(indexT >= 0) 
		{
			if (xDir)
				rbType = MSSlabRebarInfo::TopX;
			else
				rbType = MSSlabRebarInfo::TopY;

			MSSlabRebar* pSlabRebar = MakeSlabRebar(rebarIndex, text, rbType, pInfoLine); // SlabRebar 만들기
			ma_SlabRebar.push_back(pSlabRebar);
		}

		if(indexB >= 0) 
		{
			if (xDir)
				rbType = MSSlabRebarInfo::BottomX;
			else
				rbType = MSSlabRebarInfo::BottomY;

			MSSlabRebar* pSlabRebar = MakeSlabRebar(rebarIndex, text, rbType, pInfoLine); // SlabRebar 만들기
			ma_SlabRebar.push_back(pSlabRebar);
		}
	}
	return ma_SlabRebar.size();
}

bool CR_SlabRebarInfo::IsXDirection(GM2DLineVector* pRebarLine)
{
	double degree = pRebarLine->GetDegree();
	if (degree < 45|| degree>315 || (135<degree&&degree<225))
		return true;
	return false;
}

GM2DLineVector* CR_SlabRebarInfo::FindRebarInfoLine(int tagIndex)
{
	int nInfo = ma_InfoLine.size();
	for (int i=0; i < nInfo; i++)
	{
		GM2DLineVector* pLine = ma_InfoLine[i];
		if (pLine->m_Tag == tagIndex)
			return pLine;
	}
	return NULL;
}

MSSlabRebar* CR_SlabRebarInfo::GetSlabRebar(int index)
{
	if (index < ma_SlabRebar.size())
		return ma_SlabRebar[index];
	return NULL;
}

MSSlabRebar* CR_SlabRebarInfo::MakeSlabRebar(int rebarIndex, CString text, MSSlabRebarInfo::RebarType rbType, GM2DLineVector* pInfoLine)
{
	MSSlabRebar* pRebar = new MSSlabRebar();

	// set variables
	pRebar->m_RebarLine = *ma_RebarLine[rebarIndex];
	pRebar->m_RebarZone.m_P0 = pInfoLine->m_P0;
	pRebar->m_RebarZone.m_P1 = pInfoLine->m_P1;

	GM2DPolyline* pNewBoundary = new GM2DPolyline(*m_pBoundary);
	pRebar->SetZoneProfile(pNewBoundary);
	if (m_pBoundary->GetSize() > 5)
		pRebar->SetPolylineType(true);
	pRebar->SetRebarDirection(0.0); // pRebar->m_RebarLine.GetDegree());

	MSSlabRebarInfo* pRebarInfo= MakeRebarInfo(pRebar->m_RebarLine, text, rbType);
	pRebar->SetRebarInfo(pRebarInfo);
	return pRebar;
}

MSSlabRebarInfo* CR_SlabRebarInfo::MakeRebarInfo(GM2DLineVector rebarLine, CString text, MSSlabRebarInfo::RebarType rbType)
{
	long nPLIndex = text.Find('(');
	if (nPLIndex != -1)
		text = text.Mid(0, nPLIndex);

	//Get RebarText
	MSStirrupBar rebar;
	rebar.InitData(text);

	// Make RebarLine
	MSSlabRebarInfo* pRebarInfo = new MSSlabRebarInfo(rbType);
	pRebarInfo->SetRebarText(text);
// 	GM2DPolyline* pLine = new GM2DPolyline();
// 	pLine->AddLine(rebarLine);
//	pRebarLine->mp_RebarLine = pLine;
	return pRebarInfo;
}

long CogReader::CutEachLowWalls(DG2DLine* MyLine)
{
	if (MyLine == NULL)	return 0;
	long NumPairs = MyLine->ma_Pair.GetSize();
	if (NumPairs == 0)	return 0;

	//100714	if(NumPairs==1 && MyLine->m_Tag && MyLine->m_bSingleLine==FALSE)
	if (NumPairs == 1 && MyLine->m_Tag)
		return 0;

	// 	MSFloor* pLowFloor = mp_MSFloor->GetLowerFloor();
	// 	if(pLowFloor==NULL)
	// 		return 0;
	double dLength = MyLine->Length();

	double dMargin = 10;
	long NumDGWFoot = 0;
	BOOL bPairSameDir = FALSE;
	BOOL bGapSameDir = FALSE;
	MSWallFootG* pWFoot = NULL;
	BOOL bMakeUrLine = FALSE;
	MyLine->m_Tag = 1;
	BOOL bSNodeFlag, bENodeFlag;
	bSNodeFlag = bENodeFlag = TRUE;

	double dH = 1000;

	// Error Message
	CString GroupName = GetGroupName();
	CString strMsg;

	BOOL MyResult = FALSE, LinkResult = FALSE, UrResult = FALSE;
	GM2DLineVector MainLVec, UrCenLVec, LinkCenLVec;

	BOOL bFirstLine = TRUE;
	for (long PairNum = 0; PairNum < NumPairs; PairNum++)
	{
		DG2DLine* UrLine = MyLine->ma_Pair[PairNum];

		if (UrLine->m_Tag)  // 필요한가?
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
		if (PairNum == 0)
		{
			if (MyLine->ma_PairLoc0[PairNum] > -DTOL_GM)
				SVec = UrCenLVec.OrthoBaseVector(MyLine->m_P0);
			else
			{
				if (bPairSameDir && MyLine == UrLine->ma_Pair[0])
					SVec = UrCenLVec.OrthoBaseVector(UrLine->m_P0);
				else if (!bPairSameDir && MyLine == UrLine->ma_Pair[UrLine->ma_Pair.GetSize() - 1])
					SVec = UrCenLVec.OrthoBaseVector(UrLine->m_P1);
				else
					SVec = UrCenLVec.OrthoBaseVector(MyLine->m_P0);
			}
		}
		else
		{
			if (bPairSameDir)
				SVec = UrCenLVec.OrthoBaseVector(UrLine->m_P0);
			else
				SVec = UrCenLVec.OrthoBaseVector(UrLine->m_P1);
		}

		if (PairNum == NumPairs - 1)
		{
			if (MyLine->ma_PairLoc1[PairNum] < 1. + DTOL_GM)
				EVec = UrCenLVec.OrthoBaseVector(MyLine->m_P1);
			else
			{
				if (bPairSameDir && MyLine == UrLine->ma_Pair[UrLine->ma_Pair.GetSize() - 1])
					EVec = UrCenLVec.OrthoBaseVector(UrLine->m_P1);
				else if (!bPairSameDir && MyLine == UrLine->ma_Pair[0])
					EVec = UrCenLVec.OrthoBaseVector(UrLine->m_P0);
				else
					EVec = UrCenLVec.OrthoBaseVector(MyLine->m_P1);
			}
		}
		else
		{
			if (bPairSameDir)
				EVec = UrCenLVec.OrthoBaseVector(UrLine->m_P1);
			else
				EVec = UrCenLVec.OrthoBaseVector(UrLine->m_P0);
		}

		if (SVec.Distance(EVec) <= m_DTOL_NODE)
			continue;

		double dB = MyLine->Distance(UrLine->m_P0); ;
		MSCompoM* pCompoM = NULL;
		GMText* pGMTxt = FindGMText(MyLine, UrLine, DG_LOWWALL);
		CString strDefName, noNameMsg;
		strDefName = MakeDefaultName(dB);
		bool IsNoNameError = false;
		if (GetRecognizeMode() == eRecognizeMode::Single)
		{
			if (pGMTxt != nullptr)
			{
				strDefName = MakeDefaultName(pGMTxt->m_strText);
				pCompoM = GetCompoM(strDefName, GetMSCompoMType(DG_LOWWALL));
			}
			else
			{
				strDefName.Format(_T("%sHR1(%ld x %ld)"), m_DefNamePrefix, long(dB), long(dH));
				// Text를 못찾은 경우 warning
				noNameMsg.Format(_T("%s - %s"), strDefName, WARNING_NAME_NONE);
				IsNoNameError = TRUE;
			}

			if (pCompoM == NULL)
			{
				if (pGMTxt != nullptr)			// Text는 있지만, 단면정보가 없는 경우
				{
					strMsg.Format(_T("%s - %s"), pGMTxt->m_strText, WARNING_SECTION_NONE);
					AddErrorObject(errorFigureIDs, W_CODE_SECTION_NONE, WARNING_LEVEL, GroupName, strMsg);
				}

				pCompoM = GetCompoM(strDefName, MSCompoM::msHandRailM);
				if (pCompoM == NULL)
				{
					if (!m_IsSaveM)
					{
						if (pGMTxt)	strDefName += _T("(No Section)");
					}
					pCompoM = CreateNewHandRailM(dB, strDefName);
				}
			}
		}
		else if (GetRecognizeMode() == eRecognizeMode::History)
		{
			if (pGMTxt != nullptr)
				strDefName = MakeDefaultName(pGMTxt->m_strText);
			pCompoM = CreateNewHandRailM(dB, strDefName);
		}
		else
			continue;

		if (pCompoM == nullptr)	continue;
		MSHandRailG* pLowWallG = (MSHandRailG*)m_pBldg->CreateNewCompoG(MSCompoG::msHandRailG);
		pLowWallG->ReplaceVec(0, new GM2DVector(SVec.m_X, SVec.m_Y));
		pLowWallG->ReplaceVec(1, new GM2DVector(EVec.m_X, EVec.m_Y));
		pLowWallG->mp_CompoM = pCompoM;
		pLowWallG->SetLineAlignType(LINEALIGN_CENTER);
		pLowWallG->m_SOffsetZ = m_BotOffset;
		pLowWallG->m_EOffsetZ = m_BotOffset;
		ma_WallFootG.Add(pLowWallG);

		if (GetRecognizeMode() == eRecognizeMode::Single)
		{
			if (fabs(pCompoM->GetWidth() - dB) > 1.0)
			{
				vector<MSElement*> compoGArr;	compoGArr.push_back(pLowWallG);
				long oldWidth = (long)pCompoM->GetWidth();
				strMsg.Format(_T("%s - 단면정보(W:%ld)와 평면크기(W:%ld)가 다릅니다."), pCompoM->m_Name, oldWidth, (long)dB);
				AddErrorObject(compoGArr, W_CODE_SIZE_DIFF, WARNING_LEVEL, GroupName, strMsg);
			}

			vector<long> FigureIDs;
			AddVdFigureID(MyLine, FigureIDs);
			AddVdFigureID(UrLine, FigureIDs);
			mm_CompoGtoFigureIDs.insert(make_pair(pLowWallG, FigureIDs));

		}
		if (pGMTxt)
			AddUsedTextByCompoG(pGMTxt, pLowWallG);

		NumDGWFoot++;
	}

	return NumDGWFoot;
}
