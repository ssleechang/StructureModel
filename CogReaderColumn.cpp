#include "stdafx.h"

#include "GMLib/GMObject.h"
#include "GMLib/GM2DLineVector.h"
#include "GMLib/GMQuadTree.h"
#include "GMLib/GMText.h"

#include "StructureModel\MSRebarBuilding.h"
#include "StructureModel\MSGridLine.h"
#include "StructureModel\MSLinkMemberG.h"
#include "StructureModel\MSCompoG.h"

#include "CogReader.h"
#include "DG2DLine.h"
#include "DG2DArc.h"

#include "StructureModel\MSRcLineMemberM.h"
#include "StructureModel\MSRcIsoFootM.h"
#include "StructureModel\MSSteelColumnM.h"
#include "StructureModel\MSSteelSection.h"
#include "StructureModel\MSSrcColumnM.h"
#include <math.h>

void CogReader::ExtractColumns()
{
	if(m_MaterialType == MSMaterial::msSteel)
		ExtractSteelColumns();
	else
	{
		ExtractRectColumns();
		ExtractRoundColumns();
	}
}

void CogReader::AddSteelColumnDataByBlock(double xc, double yc, double rotate)
{
	GM2DVector* vec = new GM2DVector(xc, yc);
 	mm_SteelColumnData.insert(make_pair(vec, rotate));
}

void CogReader::ExtractSteelColumns()
{
	int nSteel = ma_SteelPolyline.GetSize();
	for(int i = 0; i < nSteel; i++)
	{
		GM2DPolyline* polyline = ma_SteelPolyline[i];
		vector<long> figureIDs;
		MakeSteelColumnG(polyline, figureIDs);
	}
	for (auto it = mm_SteelColumnData.begin(); it != mm_SteelColumnData.end(); it++)
	{
		CString strDefName = MakeDefaultName(_T("DefaultC1"));
		MSCompoM* pCompoM = GetCompoM(strDefName, GetMSCompoMType(DG_COLUMN));
		if (pCompoM == nullptr)
		{
			double dB = 500;
			double dH = 500;
			int SecShape = 1;
			pCompoM = CreateNewColumnM(dB, dH, SecShape, strDefName);
		}

		MSColumnG* pColumnG = (MSColumnG*)m_pBldg->CreateNewCompoG(MSCompoG::msColumnG);
		GM2DVector* insertVec = new GM2DVector(it->first->m_X, it->first->m_Y);
		pColumnG->ReplaceVec(0, insertVec);
		pColumnG->mp_CompoM = pCompoM;
		pColumnG->m_dAxisRotation = it->second;
		pColumnG->SetPointAlignType(COLUMNALIGN_CENTER);
		ma_ColumnG.Add(pColumnG);
	}
}

void CogReader::ExtractRectColumns()
{
	m_pQT->ResetGMTag(DG_COLUMN);
	MergeLineSegments(ma_DGColumnLines, DG_COLUMN);
	SortDGLines(ma_DGColumnLines);
	SortPairLines(ma_DGColumnLines);

	ResetTag(ma_DGColumnLines);
	long NumObj = ma_DGColumnLines.GetSize();

	m_pQT->ResetSearchSpace();
	PutDGLinesIntoQT(ma_DGColumnLines);
	for(long ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGColumnLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindLinkedLine(AObj, DG_COLUMN);
	}

	ResetTag(ma_DGColumnLines);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGColumnLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindGappedLine(AObj, DG_COLUMN);
	}

	ResetTag(ma_DGColumnLines);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGColumnLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(AObj);
		AObj->m_Tag = 1;

		SearchParallelLine(AObj, m_MIN_WALL_THICK , m_MAX_WALL_THICK, CrossQTs, DG_COLUMN);
	}

	ResetTag(ma_DGColumnLines);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* MyLine = ma_DGColumnLines[ObjNum];
		if(MyLine==NULL || MyLine->m_Tag!=0 || MyLine->Length()>m_MAX_WALL_THICK)  
			continue;
		long nPair = MyLine->ma_Pair.GetSize(); 

		DG2DLine* MyLink0 = MyLine->m_Linked0; 
		DG2DLine* MyLink1 = MyLine->m_Linked1;

		if(MyLink0 && MyLink1 && nPair==0 &&
			MyLink1->Length() <= m_MAX_WALL_THICK && MyLink0->Length() <= m_MAX_WALL_THICK &&
			MyLink0->IsPair(MyLink1))
		{
			GM2DVector Cen2DVec = (MyLink0->PointAtParam(0.5) + MyLink1->PointAtParam(0.5)) * 0.5;
			GMText* pGMTxt = GetNearestGMText(MyLine, DG_COLUMN, Cen2DVec);
			if(pGMTxt==NULL)
				continue;

			MSRcLineMemberM* pCompoM = (MSRcLineMemberM*) GetCompoM(pGMTxt->m_strText, GetMSCompoMType(DG_COLUMN));
			if(pCompoM==NULL || pCompoM->GetShape() != MSRcLineMemberM::RECT)
				continue;

			GM2DPolyline ThePLine(TRUE);

			double dWidth = pCompoM->GetWidth();
			double dDepth = pCompoM->GetDepth();
			double dMyLength = MyLine->Length();
			if(fabs(dMyLength-dWidth) < (5.-DTOL_GM) || fabs(dMyLength-dDepth) < (5.-DTOL_GM))
			{
				GM2DPolyline ThePLine(TRUE);
				vector<long> VdFigureIDArr;
				ThePLine.Add(new GM2DVector(MyLine->m_P0));
				ThePLine.Add(new GM2DVector(MyLine->m_P1));
				GM2DVector& Pnt2 = (MyLine->m_SharePntMask & NM_LINKED1)==0 ? MyLink1->m_P1 : MyLink0->m_P0;
				GM2DVector EdgeDir = Pnt2 - MyLine->m_P1;
				GM2DVector EdgeVec;
				if(fabs(dMyLength-dWidth) < (5.-DTOL_GM))
					EdgeVec = EdgeDir.UnitVector()*dDepth;
				else
					EdgeVec = EdgeDir.UnitVector()*dWidth;
				ThePLine.Add(new GM2DVector(MyLine->m_P1 + EdgeVec));
				ThePLine.Add(new GM2DVector(MyLine->m_P0 + EdgeVec));

				AddVdFigureID(VdFigureIDArr, FindGeometyToFigureIDs(MyLine));
				AddVdFigureID(VdFigureIDArr, FindGeometyToFigureIDs(MyLink0));
				AddVdFigureID(VdFigureIDArr, FindGeometyToFigureIDs(MyLink1));

				MakeColumnG(ThePLine, MyLine, VdFigureIDArr);
				ReleasePairs(MyLink0, MyLink1);

				MyLine->m_Tag = 1;
				MyLink0->m_Tag = 1;
				MyLink1->m_Tag = 1;
			}
			else
				continue;
		}

 		if(nPair != 1 && nPair != 2)
 			continue;

		BOOL bFailed = FALSE;
		vector<long> VdFigureIDArr;
		if(MyLink0 && MyLink1 && 
			 MyLink1->Length() <= m_MAX_WALL_THICK && MyLink0->Length() <= m_MAX_WALL_THICK &&
			 MyLink0->IsPair(MyLink1))
		{
			GM2DPolyline ThePLine(TRUE);
			ThePLine.Add(new GM2DVector(MyLine->m_P0));
			ThePLine.Add(new GM2DVector(MyLine->m_P1));
			DG2DLine* MyPair = MyLine->ma_Pair[0];
			GM2DVector CrossVec;
			double MyLoc = 0.0, UrLoc = 0.0;
			MyPair->Intersect(MyLink1, CrossVec, &MyLoc, &UrLoc);
			if((MyLine->m_SharePntMask & NM_LINKED1)==0 && UrLoc < -DTOL_GM)
				bFailed = TRUE;
			else if((MyLine->m_SharePntMask & NM_LINKED1)==1 && UrLoc > 1.+DTOL_GM)
				bFailed = TRUE;
			ThePLine.Add(new GM2DVector(CrossVec));
			MyPair->Intersect(MyLink0, CrossVec, &MyLoc, &UrLoc);
			if((MyLine->m_SharePntMask & NM_LINKED0)==0 && UrLoc < -DTOL_GM)
				bFailed = TRUE;
			else if((MyLine->m_SharePntMask & NM_LINKED0)==1 && UrLoc > 1.+DTOL_GM)
				bFailed = TRUE;
			ThePLine.Add(new GM2DVector(CrossVec));
			if(!bFailed)
			{
				AddVdFigureID(VdFigureIDArr, FindGeometyToFigureIDs(MyLine));
				AddVdFigureID(VdFigureIDArr, FindGeometyToFigureIDs(MyLink0));
				AddVdFigureID(VdFigureIDArr, FindGeometyToFigureIDs(MyLink1));
				AddVdFigureID(VdFigureIDArr, FindGeometyToFigureIDs(MyPair));

				MakeColumnG(ThePLine, MyLine, VdFigureIDArr);

				ReleasePairs(MyLine, MyPair);
				ReleasePairs(MyLink0, MyLink1);

				MyLine->m_Tag = 1;
				MyLink0->m_Tag = 1;
				MyLink1->m_Tag = 1;
				if(MyPair->ma_Pair.GetSize()==0)
					MyPair->m_Tag = 1;
			}
		}
		else if(MyLink0 && MyLink0->ma_Pair.GetSize()==1 &&
						MyLink0->Length() <= m_MAX_WALL_THICK)
		{
			GM2DPolyline ThePLine(TRUE);
			ThePLine.Add(new GM2DVector(MyLine->m_P0));
			DG2DLine* MyPair = MyLine->ma_Pair[0];
			DG2DLine* LinkPair = MyLink0->ma_Pair[0];

			GM2DVector CrossVec;
			double MyLoc = 0.0, UrLoc = 0.0;
			MyLine->Intersect(LinkPair, CrossVec, &MyLoc, &UrLoc);
			if(MyLoc <= 1.)
				bFailed = TRUE;
			ThePLine.Add(new GM2DVector(CrossVec));

			MyPair->Intersect(LinkPair, CrossVec, &MyLoc, &UrLoc);
			double LinkLoc = MyLink0->OrthoBaseLocValue(CrossVec);
			if((MyLine->m_SharePntMask & NM_LINKED0)==0 && LinkLoc < -DTOL_GM)
				bFailed = TRUE;
			else if((MyLine->m_SharePntMask & NM_LINKED0)==1 && LinkLoc > 1.+DTOL_GM)
				bFailed = TRUE;
			ThePLine.Add(new GM2DVector(CrossVec));

			MyPair->Intersect(MyLink0, CrossVec, &MyLoc, &UrLoc);
			if((MyLine->m_SharePntMask & NM_LINKED0)==0 && UrLoc < -DTOL_GM)
				bFailed = TRUE;
			else if((MyLine->m_SharePntMask & NM_LINKED0)==1 && UrLoc > 1.+DTOL_GM)
				bFailed = TRUE;
			ThePLine.Add(new GM2DVector(CrossVec));
			if(!bFailed)
			{
				AddVdFigureID(VdFigureIDArr, FindGeometyToFigureIDs(MyLine));
				AddVdFigureID(VdFigureIDArr, FindGeometyToFigureIDs(MyLink0));
				AddVdFigureID(VdFigureIDArr, FindGeometyToFigureIDs(MyPair));
				AddVdFigureID(VdFigureIDArr, FindGeometyToFigureIDs(LinkPair));

				MakeColumnG(ThePLine, MyLine, VdFigureIDArr);

				ReleasePairs(MyLine, MyPair);
				ReleasePairs(MyLink0, LinkPair);

				MyLine->m_Tag = 1;
				MyLink0->m_Tag = 1;
				if(MyPair->ma_Pair.GetSize()==0)
					MyPair->m_Tag = 1;
				if(LinkPair->ma_Pair.GetSize()==0)
					LinkPair->m_Tag = 1;
			}
		}
		else if(MyLink1 && MyLink1->ma_Pair.GetSize()==1 &&
			MyLink1->Length() <= m_MAX_WALL_THICK)
		{
			GM2DPolyline ThePLine(TRUE);
			ThePLine.Add(new GM2DVector(MyLine->m_P1));
			DG2DLine* MyPair = MyLine->ma_Pair[0];
			DG2DLine* LinkPair = MyLink1->ma_Pair[0];

			GM2DVector CrossVec;
			double MyLoc = 0, UrLoc = 0;
			MyLine->Intersect(LinkPair, CrossVec, &MyLoc, &UrLoc);
			if(MyLoc >= 0.)
				bFailed = TRUE;
			ThePLine.Add(new GM2DVector(CrossVec));

			MyPair->Intersect(LinkPair, CrossVec, &MyLoc, &UrLoc);
			double LinkLoc = MyLink1->OrthoBaseLocValue(CrossVec);
			if((MyLine->m_SharePntMask & NM_LINKED1)==0 && LinkLoc < -DTOL_GM)
				bFailed = TRUE;
			else if((MyLine->m_SharePntMask & NM_LINKED1)==1 && LinkLoc > 1.+DTOL_GM)
				bFailed = TRUE;
			ThePLine.Add(new GM2DVector(CrossVec));
			MyPair->Intersect(MyLink1, CrossVec, &MyLoc, &UrLoc);
			if((MyLine->m_SharePntMask & NM_LINKED1)==0 && UrLoc < -DTOL_GM)
				bFailed = TRUE;
			else if((MyLine->m_SharePntMask & NM_LINKED1)==1 && UrLoc > 1.+DTOL_GM)
				bFailed = TRUE;
			ThePLine.Add(new GM2DVector(CrossVec));

			if(!bFailed)
			{
				AddVdFigureID(VdFigureIDArr, FindGeometyToFigureIDs(MyLine));
				AddVdFigureID(VdFigureIDArr, FindGeometyToFigureIDs(MyLink1));
				AddVdFigureID(VdFigureIDArr, FindGeometyToFigureIDs(MyPair));
				AddVdFigureID(VdFigureIDArr, FindGeometyToFigureIDs(LinkPair));

				MakeColumnG(ThePLine, MyLine, VdFigureIDArr);

				ReleasePairs(MyLine, MyPair);
				ReleasePairs(MyLink1, LinkPair);

				MyLine->m_Tag = 1;
				MyLink1->m_Tag = 1;
				if(MyPair->ma_Pair.GetSize()==0)
					MyPair->m_Tag = 1;
				if(LinkPair->ma_Pair.GetSize()==0)
					LinkPair->m_Tag = 1;
			}
		}
	}

	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		GM2DPolyline ThePLine(TRUE);
		vector<DG2DLine*> DGLineArr;
		DG2DLine* pCurLine = ma_DGColumnLines[ObjNum];
		if(pCurLine==NULL || pCurLine->m_Tag!=0 || pCurLine->Length()>m_MAX_WALL_THICK)  
			continue;

		vector<long> VdFigureIDArr;
		ThePLine.Add(new GM2DVector(pCurLine->m_P0));
		GM2DVector* pPGonStartPnt = &(pCurLine->m_P0);
 		DG2DLine* pNxtLine;
 		GM2DVector* pCurEndPnt;
		long NxtEnd = 1;  
		while(ThePLine.GetSize() <= 10 && pCurLine && pCurLine->Length()<=m_MAX_WALL_THICK)
		{
			DGLineArr.push_back(pCurLine);
			if(NxtEnd == 0)
			{
				pCurEndPnt = &(pCurLine->m_P0);
				pNxtLine = pCurLine->m_Linked0;
				NxtEnd = (pCurLine->m_SharePntMask & NM_LINKED0) == 0 ? 1 : 0;
				if((pNxtLine==NULL || pNxtLine->Length()>m_MAX_WALL_THICK))
				{
					if(pCurLine->m_Gapped0 && pCurLine->m_Gapped0->m_Tag==0)
					{
						if((pCurLine->m_SharePntMask & NM_GAPPED0)==0)
						{
							pCurEndPnt = &(pCurLine->m_Gapped0->m_P1);
							pNxtLine = pCurLine->m_Gapped0->m_Linked1;
							NxtEnd = (pCurLine->m_Gapped0->m_SharePntMask & NM_LINKED1) == 0 ? 1 : 0;
						}
						else
						{
							pCurEndPnt = &(pCurLine->m_Gapped0->m_P0);
							pNxtLine = pCurLine->m_Gapped0->m_Linked0;
							NxtEnd = (pCurLine->m_Gapped0->m_SharePntMask & NM_LINKED0) == 0 ? 1 : 0;
						}
						if(pCurLine->m_P1.Distance(*pCurEndPnt) > m_MAX_WALL_THICK)
							break;
					}
					else
						break;
				}
			}
			else
			{
				pCurEndPnt = &(pCurLine->m_P1);
				pNxtLine = pCurLine->m_Linked1;
				NxtEnd = (pCurLine->m_SharePntMask & NM_LINKED1) == 0 ? 1 : 0;
				if((pNxtLine==NULL || pNxtLine->Length()>m_MAX_WALL_THICK))
				{
					if(pCurLine->m_Gapped1 && pCurLine->m_Gapped1->m_Tag==0)
					{
						if((pCurLine->m_SharePntMask & NM_GAPPED1)==0)
						{
							pCurEndPnt = &(pCurLine->m_Gapped1->m_P1);
							pNxtLine = pCurLine->m_Gapped1->m_Linked1;
							NxtEnd = (pCurLine->m_Gapped1->m_SharePntMask & NM_LINKED1) == 0 ? 1 : 0;
						}
						else
						{
							pCurEndPnt = &(pCurLine->m_Gapped1->m_P0);
							pNxtLine = pCurLine->m_Gapped1->m_Linked0;
							NxtEnd = (pCurLine->m_Gapped1->m_SharePntMask & NM_LINKED0) == 0 ? 1 : 0;
						}
						if(pCurLine->m_P0.Distance(*pCurEndPnt) > m_MAX_WALL_THICK)
							break;
					}
					else
						break;
				}
			}
			if(pCurEndPnt->Distance(*pPGonStartPnt) < m_DTOL_DIST_LINKTEST)
			{
				long nDG = DGLineArr.size();
				for(long iDG=0; iDG<nDG; iDG++)
				{
					DGLineArr[iDG]->m_Tag = 1;
					AddVdFigureID(DGLineArr[iDG], VdFigureIDArr);
				}
				MakeColumnG(ThePLine, ma_DGColumnLines[ObjNum], VdFigureIDArr);
				break;
			}
			ThePLine.Add(new GM2DVector(*pCurEndPnt));
			pCurLine = pNxtLine;
		}
	}
}

void CogReader::ExtractRoundColumns()
{
	if(m_MaterialType == MSMaterial::msConcrete)
		ExtractRcRoundColumns();
	else if(m_MaterialType == MSMaterial::msSteel)
		ExtractSteelRoundColumns();
	else if(m_MaterialType == MSMaterial::msSRC)
		ExtractSrcRoundColumns();
	else if(m_MaterialType == MSMaterial::msFormWork)
		ExtractRcRoundColumns();
}

void CogReader::MakeCircleDatas(CTypedPtrArray<CObArray, GM2DCircle*>& CircleArr)
{
	CTypedPtrArray<CObArray, GM2DGeometry*>& ColGMs = m_pQT->ma_GMs[DG_COLUMN];

	int NumGeom = ColGMs.GetSize();
	for(int i = 0; i < NumGeom; i++)
	{
		GM2DGeometry* pGeom = ColGMs[i];
		if(pGeom->m_ObjType == GT_CIRCLE)	CircleArr.Add((GM2DCircle*)pGeom);
		if(pGeom->m_ObjType == GT_ARC)
		{
			GM2DArc* pIArc = (GM2DArc*)pGeom;
			for(int j = i+1; j < NumGeom; j++)
			{
				GM2DGeometry* pJGeom = ColGMs[j];
				if(pGeom->m_ObjType == GT_ARC)
				{
					GM2DArc* pJArc = (GM2DArc*)pJGeom;
					if(fabs(pIArc->m_A - pJArc->m_A) < 1.0 && pIArc->m_Center.SameVector4DTOL(pJArc->m_Center))
					{
						if(pIArc->GetStart2DVec().SameVector4DTOL(pJArc->GetEnd2DVec()) && pIArc->GetEnd2DVec().SameVector4DTOL(pJArc->GetStart2DVec()))
						{
							GM2DCircle* pNewCircle = new GM2DCircle(pIArc->m_A, pIArc->m_Center.m_X, pIArc->m_Center.m_Y);
							pNewCircle->m_Tag = 1000;
							CircleArr.Add(pNewCircle);
							ChangeGeometryToFigureIDs(pNewCircle, pIArc, pJArc);
							break;
						}
					}
				}
			}
		}
	}
}

void CogReader::ChangeGeometryToFigureIDs(GM2DGeometry* pNewGeom, GM2DGeometry* pOldGeom1, GM2DGeometry* pOldGeom2)
{
	vector<long> figureIDs1 = FindGeometyToFigureIDs(pOldGeom1);
	vector<long> figureIDs2 = FindGeometyToFigureIDs(pOldGeom2);

	AddVdFigureID(figureIDs1, figureIDs2);
	AddVdFigureID(pNewGeom, figureIDs1);
}

void CogReader::ExtractRcRoundColumns()
{
	m_pQT->ResetGMTag(DG_COLUMN);
	MergeGMLineSegments(DG_COLUMN);
	m_pQT->ResetGMTag(DG_COLUMN);

	CTypedPtrArray<CObArray, GM2DGeometry*>& ColGMs = m_pQT->ma_GMs[DG_COLUMN];

	CTypedPtrArray<CObArray, GM2DCircle*> CircleArr;
	MakeCircleDatas(CircleArr);

	double dMargin = 10;
	CTypedPtrArray<CObArray, GMQuadTree*> RootQT;
	RootQT.Add(m_pQT);

	CString GroupName = GetGroupName();
	CString strMsg, noNameMsg;

	long NumObj = CircleArr.GetSize();
	for(long ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		GM2DGeometry* AObj = CircleArr[ObjNum];
		if(AObj == NULL)  continue;
		if(AObj->m_ObjType != GT_CIRCLE)	continue;

		MSCompoM* pCompoM = NULL;
		bool IsNoNameError = false;
		GM2DCircle* GMObj = (GM2DCircle*)AObj;
		double dB = GMObj->m_A*2;
		double dH = dB;
		if(dB < m_MIN_WALL_THICK || dB > m_MAX_WALL_THICK)	continue;
		GM2DVector Cen2DVec = GMObj->m_Center;

		GM2DCircle offsetCircle(*GMObj);
		offsetCircle.SetRadius(GMObj->m_A+m_DTOL_TEXT_DIST);

		GMText* pGMTxt = GetGMText_ContainCircle(&offsetCircle, DG_COLUMN, m_pQT->FindCrossQts(AObj));
		CString strDefName;
		strDefName = MakeDefaultName((long)dB);

		if (GetRecognizeMode() == eRecognizeMode::Single)
		{
			if (pGMTxt != nullptr)
			{
				strDefName = MakeDefaultName(pGMTxt->m_strText);
				pCompoM = GetCompoM(pGMTxt->m_strText, GetMSCompoMType(DG_COLUMN));
			}
			else
			{
				// Text를 못찾은 경우 warning
				noNameMsg.Format(_T("%s - %s"), strDefName, WARNING_NAME_NONE);
				IsNoNameError = true;
			}
			if (pCompoM == NULL)
			{
				if (pGMTxt != nullptr)			// Text는 있지만, 단면정보가 없는 경우
				{
					strMsg.Format(_T("%s - %s"), pGMTxt->m_strText, WARNING_SECTION_NONE);
					AddErrorObject(FindGeometyToFigureIDs(AObj), W_CODE_SECTION_NONE, WARNING_LEVEL, GroupName, strMsg);
				}

				pCompoM = GetCompoM(strDefName, MSCompoM::msColumnM);
				if (pCompoM == NULL)
				{
					if (!m_IsSaveM)
					{
						if (pGMTxt)	strDefName += _T("(No Section)");
					}
					pCompoM = CreateNewColumnM(dB, dH, MSRcLineMemberM::CIRCLE, strDefName);
				}
			}
		}
		else if (GetRecognizeMode() == eRecognizeMode::History)
		{
			if (pGMTxt != nullptr)
				strDefName = MakeDefaultName(pGMTxt->m_strText);
			MSCompoM* oldCompoM = GetCompoM(strDefName, GetMSCompoMType(DG_COLUMN));
			if (oldCompoM != nullptr)
			{
				pCompoM = m_pBldg->CreateNewCompoM(oldCompoM->GetType());
				pCompoM->SetName(strDefName);
				((MSRcLineMemberM*)pCompoM)->SetWidth(dB);
				((MSRcLineMemberM*)pCompoM)->SetShape(MSRcLineMemberM::CIRCLE);
			}
			else
				pCompoM = CreateNewColumnM(dB, dH, MSRcLineMemberM::CIRCLE, strDefName);
		}
		else if (GetRecognizeMode() == eRecognizeMode::Formwork)
		{
			pCompoM = CreateNewColumnM(dB, dH, MSRcLineMemberM::CIRCLE, strDefName);
		}
		else
			continue;

		if (pCompoM == nullptr)	continue;

		MSColumnG* pColumnG = (MSColumnG*)m_pBldg->CreateNewCompoG(MSElement::msColumnG);
		pColumnG->ReplaceVec(0, new GM2DVector(Cen2DVec.m_X, Cen2DVec.m_Y));
		pColumnG->mp_CompoM = pCompoM;
		pColumnG->SetPointAlignType(COLUMNALIGN_CENTER);
		pColumnG->m_SOffsetZ = m_BotOffset;
		pColumnG->m_EOffsetZ = m_TopOffset;
		ma_ColumnG.Add(pColumnG);

		if (GetRecognizeMode() == eRecognizeMode::Single)
		{
			if (IsNoNameError)
			{
				vector<MSElement*> CompoGArr;
				CompoGArr.push_back(pColumnG);
				AddErrorObject(CompoGArr, W_CODE_NAME_NONE, WARNING_LEVEL, GroupName, noNameMsg);
			}
		}

		vector<long> FigureIDs;
		AddVdFigureID(AObj, FigureIDs);
		mm_CompoGtoFigureIDs.insert(make_pair(pColumnG, FigureIDs));

		if(pGMTxt)
			AddUsedTextByCompoG(pGMTxt, pColumnG);

		if(AObj->m_Tag == 1000)
			delete AObj;
		else
			AObj->m_Tag = 1;
	}
}

void CogReader::ExtractSteelRoundColumns()
{

}

void CogReader::ExtractSrcRoundColumns()
{

}

BOOL CogReader::MakeColumnG(GM2DPolyline& ThePLine, DG2DLine* Geo, vector<long>& VdFigureIDArr)
{
	return MakeRcColumnG(ThePLine, Geo, VdFigureIDArr);

// 	if(m_MaterialType == MSMaterial::msConcrete)
// 		return MakeRcColumnG(ThePLine, Geo, VdFigureIDArr);
// 	else if(m_MaterialType == MSMaterial::msSRC)
// 		return MakeSrcColumnG(ThePLine, Geo, VdFigureIDArr);

	return FALSE;
}

BOOL CogReader::MakeRcColumnG(GM2DPolyline& ThePLine, DG2DLine* Geo, vector<long>& VdFigureIDArr)
{
	GMPolygon ThePGon(TRUE);
	ThePGon = ThePLine;
	long SecShape = MSRcLineMemberM::RECT;
	if(ThePGon.IsRectangle() == FALSE)
	{
//		SecShape = MSRcLineMemberM::POLYGON;
		return FALSE;
	}

	GMVector Cen3DVec = ThePGon.MassCenter();
	GM2DVector Cen2DVec(Cen3DVec.m_X, Cen3DVec.m_Y);
	ThePGon.Translate(Cen3DVec);
	GM2DLineVector FirstLine = ThePLine.GetLine(0);
	GM2DLineVector SecondLine = ThePLine.GetLine(1);

	double dB = 0, dH = 0;
	double firstDegree = FirstLine.GetDegree();
	double secondDegree = SecondLine.GetDegree();

	if(firstDegree > 90 && firstDegree <= 270)
	{
		FirstLine.ReverseDirection();
		firstDegree = FirstLine.GetDegree();
	}
	if(secondDegree > 90 && secondDegree <= 270)
	{
		SecondLine.ReverseDirection();
		secondDegree = SecondLine.GetDegree();
	}

	if((-45 <= firstDegree && firstDegree <= 45) || (135 <= firstDegree && firstDegree <= 225))
	{
		dB = FirstLine.Length();
		dH = SecondLine.Length();
	}
	else
	{
		dB = SecondLine.Length();
		dH = FirstLine.Length();
	}
	if (dB < m_MIN_WALL_THICK || dH < m_MIN_WALL_THICK)
		return FALSE;

	double dTolerance = 2.5;

	CString GroupName = GetGroupName();
	CString strMsg, noNameMsg;

	MSCompoM* pCompoM=NULL;

	GM2DPolyline* pOffsetPolyline = MakeOffsetPolyline(&ThePLine, m_DTOL_TEXT_DIST, TRUE);
	GMText* pGMTxt = GetGMText_ContainPolyline(pOffsetPolyline, DG_COLUMN, m_pQT->FindCrossQts(Geo));

	GM2DPolyline* pSteelPolyline = NULL;
	if (m_MaterialType == MSMaterial::msSRC)
	{
		if (!ThePLine.IsClosed())	ThePLine.MakeClosedPolyline(DTOL_GM);
		pSteelPolyline = GetSteelPolyline_ContainPolyline(&ThePLine);
	}

	delete pOffsetPolyline;

//	if(m_MaterialType == MSMaterial::msSRC && pSteelPolyline == NULL)	return FALSE;
	
	CString strDefName;
	bool IsNoNameError = false;

	strDefName = MakeDefaultName((long)dB, (long)dH);
	if (GetRecognizeMode() == eRecognizeMode::Single)
	{
		if (pGMTxt != nullptr)
		{
			strDefName = MakeDefaultName(pGMTxt->m_strText);
			pCompoM = GetCompoM(strDefName, GetMSCompoMType(DG_COLUMN));
		}
		else
		{
			// Text를 못찾은 경우 warning
			noNameMsg.Format(_T("%s - %s"), strDefName, WARNING_NAME_NONE);
			IsNoNameError = true;
		}
		if (pCompoM == NULL)
		{
			if (pGMTxt != nullptr)			// Text는 있지만, 단면정보가 없는 경우
			{
				strMsg.Format(_T("%s - %s"), pGMTxt->m_strText, WARNING_SECTION_NONE);
				AddErrorObject(VdFigureIDArr, W_CODE_SECTION_NONE, WARNING_LEVEL, GroupName, strMsg);
			}

			pCompoM = GetCompoM(strDefName, MSCompoM::msColumnM);
			if (pCompoM == NULL)
			{
				if (!m_IsSaveM)
				{
					if (pGMTxt)	strDefName += _T("(No Section)");
				}
				pCompoM = CreateNewColumnM(dB, dH, SecShape, strDefName);
			}
		}
	}
	else if (GetRecognizeMode() == eRecognizeMode::History)
	{
		if (pGMTxt != nullptr)
			strDefName = MakeDefaultName(pGMTxt->m_strText);

		if (m_MaterialType == MSMaterial::msConcrete)
		{
			MSCompoM* oldCompoM = GetCompoM(strDefName, GetMSCompoMType(DG_COLUMN));
			if (oldCompoM != nullptr)
			{
				pCompoM = m_pBldg->CreateNewCompoM(oldCompoM->GetType());
				pCompoM->SetName(strDefName);
				((MSRcLineMemberM*)pCompoM)->SetWidth(dB);
				((MSRcLineMemberM*)pCompoM)->SetDepth(dH);
				((MSRcLineMemberM*)pCompoM)->SetShape(MSRcLineMemberM::RECT);
			}
			else
				pCompoM = CreateNewColumnM(dB, dH, SecShape, strDefName);
		}
		else if (m_MaterialType == MSMaterial::msSRC)
		{
			pCompoM = CreateNewColumnM(dB, dH, SecShape, strDefName);
			MSCompoM* oldCompoM = GetCompoM(pCompoM->GetName(), GetMSCompoMType(DG_COLUMN));
			if (oldCompoM != nullptr)
				((MSSrcColumnM*)pCompoM)->m_pSection = ((MSSrcColumnM*)oldCompoM)->GetSteelSection();
		}
	}
	else if (GetRecognizeMode() == eRecognizeMode::Formwork)
	{
		pCompoM = CreateNewColumnM(dB, dH, MSRcLineMemberM::RECT, strDefName);
	}
	else
		return FALSE;

	if (pCompoM == nullptr)	return FALSE;

	double dRotation = 0.;
	BOOL bDifSection = FALSE;
	double dWidth = pCompoM->GetWidth();
	double dDepth = pCompoM->GetDepth();
	if(SecShape == MSRcLineMemberM::RECT)
	{
		bool IsSameSize = false;
		if(fabs(dWidth - dDepth) < DTOL1)
			IsSameSize = true;

		double dFirstLength = FirstLine.Length();
		double dSecondLength = SecondLine.Length();
		double dFirstLineDegree = FirstLine.GetDegree();
		double dSecondLineDegree = SecondLine.GetDegree();
		if(dFirstLineDegree >= 180)			dFirstLineDegree -= 360;
		if(dSecondLineDegree >= 180)	dSecondLineDegree -= 360;
		if(fabs(dWidth - dFirstLength) < dTolerance+1)
		{
			dRotation = dFirstLineDegree;
			if(fabs(dDepth - dSecondLength) > dTolerance+1.)
				bDifSection = TRUE;
		}
		else if(fabs(dWidth - dSecondLength) < dTolerance+1.)
		{
			dRotation = dSecondLineDegree;
			if(fabs(dDepth - dFirstLength) > dTolerance+1.)
				bDifSection = TRUE;
		}
		else if(fabs(dDepth - dFirstLength) < dTolerance+1.)
		{
			dRotation = dSecondLineDegree;
			if(fabs(dWidth - dSecondLength) > dTolerance+1.)
				bDifSection = TRUE;
		}
		else if(fabs(dDepth - dSecondLength) < dTolerance+1.)
		{
			dRotation = dFirstLineDegree;
			if(fabs(dWidth - dFirstLength) > dTolerance+1.)
				bDifSection = TRUE;
		}
		else
		{
			dRotation = dFirstLineDegree;
			if (dFirstLength < dSecondLength)	dRotation = dSecondLineDegree;
			if (dDepth > dWidth)	dRotation += 90;
			bDifSection = TRUE;
		}
		if( IsSameSize)
		{
			dRotation = min(dFirstLineDegree, dSecondLineDegree);
			if (GetRecognizeMode() == History)
			{
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
	}
	else if(SecShape==MSRcLineMemberM::POLYGON)
	{
		double dRotate = 0;
		GM2DPolyline SecPGon(true);
		pCompoM->GetSecPolyline(SecPGon);
		
		ASSERT(0);
		BOOL Result = ThePLine.SamePolyLine(&SecPGon);
		//BOOL Result = ThePGon.Compare(SecPGon, dRotate, bReverseLoop);


		dRotation = dRotate;
	}
	else
		dRotation = 0.;

    MSColumnG* pColumnG = (MSColumnG*)m_pBldg->CreateNewCompoG(MSCompoG::msColumnG);
    pColumnG->ReplaceVec(0, new GM2DVector(Cen3DVec.m_X, Cen3DVec.m_Y));
	pColumnG->mp_CompoM = pCompoM;
// 	if(dRotation >= 180)
// 		dRotation -= 180;
	if(dRotation > 90 && dRotation <= 270)
		dRotation -= 180;
	pColumnG->m_dAxisRotation = dRotation;
	pColumnG->SetPointAlignType(COLUMNALIGN_CENTER);
	pColumnG->m_SOffsetZ = m_BotOffset;
	pColumnG->m_EOffsetZ = m_TopOffset;

	if(pSteelPolyline != NULL)
	{
		GM2DVector SteelCenter = pSteelPolyline->MassCenter();
		pColumnG->SetSteelCenter(SteelCenter-Cen2DVec);
	}
    ma_ColumnG.Add(pColumnG);

	if (GetRecognizeMode() == eRecognizeMode::Single)
	{
		if (bDifSection)
		{
			vector<MSElement*> compoGArr;	compoGArr.push_back(pColumnG);
			strMsg.Format(_T("%s - 단면정보(%ldx%ld)와 평면크기(%ldx%ld)가 다릅니다."), pCompoM->m_Name, (long)dWidth, (long)dDepth, (long)dB, (long)dH);
			AddErrorObject(compoGArr, W_CODE_SIZE_DIFF, WARNING_LEVEL, GroupName, strMsg);
		}

		if (IsNoNameError)
		{
			vector<MSElement*> CompoGArr;
			CompoGArr.push_back(pColumnG);
			AddErrorObject(CompoGArr, W_CODE_NAME_NONE, WARNING_LEVEL, GroupName, noNameMsg);
		}
	}

	vector<long> FigureIDs;
	int NumIDs = VdFigureIDArr.size();
	for(int i = 0; i < NumIDs; i++)
		FigureIDs.push_back(VdFigureIDArr[i]);
	mm_CompoGtoFigureIDs.insert(make_pair(pColumnG, FigureIDs));

	if(pGMTxt)
		AddUsedTextByCompoG(pGMTxt, pColumnG);

	return TRUE;
}

BOOL CogReader::MakeSteelColumnG(GM2DPolyline* ThePLine, vector<long>& VdFigureIDArr)
{
	double dTolerance = 2.5;

	CString GroupName = GetGroupName();
	CString strMsg, noNameMsg;

	MSCompoM* pCompoM=NULL;

	GM2DPolyline* pOffsetPolyline = MakeOffsetPolyline(ThePLine, m_DTOL_TEXT_DIST, TRUE);
	vector<GMQuadTree*> CrossQTs;
	GMText* pGMTxt = GetGMText_ContainPolyline(pOffsetPolyline, DG_COLUMN, CrossQTs);
	delete pOffsetPolyline;

	//	GMText* pGMTxt = GetNearestGMText(Geo, DG_COLUMN, Cen2DVec);
	CString strDefName;
	bool IsNoNameError = false;

	double dB = 500;
	double dH = 500;
	long SecShape = MSRcLineMemberM::RECT;
	strDefName = MakeDefaultName((long)dB, (long)dH);
	if (GetRecognizeMode() == eRecognizeMode::Single)
	{
		if (pGMTxt != nullptr)
		{
			strDefName = MakeDefaultName(pGMTxt->m_strText);
			pCompoM = GetCompoM(strDefName, GetMSCompoMType(DG_COLUMN));
		}
		else
		{
			// Text를 못찾은 경우 warning
			noNameMsg.Format(_T("%s - %s"), strDefName, WARNING_NAME_NONE);
			IsNoNameError = true;
		}
		if (pCompoM == NULL)
		{
			if (pGMTxt != nullptr)			// Text는 있지만, 단면정보가 없는 경우
			{
				strMsg.Format(_T("%s - %s"), pGMTxt->m_strText, WARNING_SECTION_NONE);
				AddErrorObject(VdFigureIDArr, W_CODE_SECTION_NONE, WARNING_LEVEL, GroupName, strMsg);
			}

			pCompoM = GetCompoM(strDefName, MSCompoM::msColumnM);
			if (pCompoM == NULL)
			{
				if (!m_IsSaveM)
				{
					if (pGMTxt)	strDefName += _T("(No Section)");
				}
				pCompoM = CreateNewColumnM(dB, dH, SecShape, strDefName);
			}
		}
	}
	else if (GetRecognizeMode() == eRecognizeMode::History)
	{
		if(pGMTxt != nullptr)
			strDefName = MakeDefaultName(pGMTxt->m_strText);
		pCompoM = CreateNewColumnM(dB, dH, SecShape, strDefName);
		MSCompoM* oldCompoM = GetCompoM(pCompoM->GetName(), GetMSCompoMType(DG_COLUMN));
		if (oldCompoM != nullptr)
			((MSSteelColumnM*)pCompoM)->m_pSection = ((MSSteelColumnM*)oldCompoM)->GetSteelSection();
	}
	else
		return FALSE;

	if (pCompoM == nullptr)	return FALSE;


	GM2DVector CenVec = ThePLine->MassCenter();
	MSColumnG* pColumnG = (MSColumnG*)m_pBldg->CreateNewCompoG(MSCompoG::msColumnG);
	pColumnG->ReplaceVec(0, new GM2DVector(CenVec.m_X, CenVec.m_Y));
	pColumnG->mp_CompoM = pCompoM;
//	pColumnG->m_dAxisRotation = dRotation;
	pColumnG->SetPointAlignType(COLUMNALIGN_CENTER);
	ma_ColumnG.Add(pColumnG);
	pColumnG->m_SOffsetZ = m_BotOffset;
	pColumnG->m_EOffsetZ = m_TopOffset;

	if(IsNoNameError)
	{
		vector<MSElement*> CompoGArr;
		CompoGArr.push_back(pColumnG);
		AddErrorObject(CompoGArr, W_CODE_NAME_NONE, WARNING_LEVEL, GroupName, noNameMsg);
	}


	vector<long> FigureIDs;
	int NumIDs = VdFigureIDArr.size();
	for(int i = 0; i < NumIDs; i++)
		FigureIDs.push_back(VdFigureIDArr[i]);
	mm_CompoGtoFigureIDs.insert(make_pair(pColumnG, FigureIDs));

	if(pGMTxt)
		AddUsedTextByCompoG(pGMTxt, pColumnG);

	return TRUE;
}

BOOL CogReader::MakeSrcColumnG(GM2DPolyline& ThePLine, DG2DLine* Geo, vector<long>& VdFigureIDArr)
{
	return TRUE;
}

void CogReader::AddVdFigureID(GM2DGeometry* pGeom, vector<long>& FigureIDs)
{
	int numFigure = FigureIDs.size();
	for(int iFigure = 0; iFigure < numFigure; iFigure++)
		AddGeometryToFigureIDs(pGeom, FigureIDs[iFigure]);
}

void CogReader::AddVdFigureID(vector<long>& allFigureIDs, vector<long>& newFigureIDs)
{
	int NewCount = newFigureIDs.size();
	for(int i = 0; i < NewCount ; i++)
		allFigureIDs.push_back(newFigureIDs[i]);
}