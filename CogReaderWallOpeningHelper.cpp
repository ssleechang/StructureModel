#include "StdAfx.h"
#include "CogReaderWallOpeningHelper.h"

#include "GMLib/GMQuadTree.h"
#include "GMLib/GMText.h"

#include "MSRebarBuilding.h"
#include "MSFloor.h"
#include "CogReader.h"

#include "MSOpeningM.h"
#include "MSWindowG.h"
#include "MSDoorG.h"
#include "MSWallOpeningExtraG.h"

CogReaderWallOpeningHelper::CogReaderWallOpeningHelper(MSBaseBuilding* pBldg, CogReader* pReader, MSFloor* pFromFloor, MSFloor* pToFloor) : CogReaderHelper(pBldg, pReader)
{
	mp_FromFloor = pFromFloor;
	mp_ToFloor = pToFloor;
}


CogReaderWallOpeningHelper::~CogReaderWallOpeningHelper(void)
{
}

bool CogReaderWallOpeningHelper::MakeData()
{
	if(mp_Bldg == NULL || mp_Reader == NULL)	return false;
	if(mp_FromFloor == NULL || mp_ToFloor == NULL)	return false;

	AnalyizeGeometry();

	GetExistedOpeningMs();
	int fromStory = mp_FromFloor->GetFloorNum();
	int toStory = mp_ToFloor->GetFloorNum();

	for(int iStory = fromStory; iStory <= toStory; iStory++)
	{
		if(iStory == 0)	continue;
		MakeWallOpenings(iStory);
	}

	return true;
}

void CogReaderWallOpeningHelper::AnalyizeGeometry()
{
	ma_Lines.clear();
	ma_Arc.clear();

	CTypedPtrArray<CObArray, GM2DGeometry*>& GMs = mp_Reader->GetGeometry(mp_Reader->GetMemberType());
	int count = GMs.GetSize();
	for(int i = 0; i < count; i++)
	{
		GM2DGeometry* pGeom = GMs[i];
		if(pGeom == NULL)	continue;
		if(pGeom->m_ObjType == GT_LINE)
		{
			ma_Lines.push_back((GM2DLineVector*)pGeom);
		}
		else if(pGeom->m_ObjType == GT_ARC)
		{
			ma_Arc.push_back((GM2DArc*)pGeom);
		}
	}
}

void CogReaderWallOpeningHelper::GetExistedOpeningMs()
{
	ma_OpeningM.clear();

	vector<long> MIDs = mp_Bldg->GetCompoMs(mp_Reader->m_WallOpeningMType);
	for(long iID = 0; iID < MIDs.size(); iID++)
	{
		MSCompoM* pCompoM = mp_Bldg->GetCompoM(MIDs[iID]);
		if(pCompoM)
			ma_OpeningM.push_back(pCompoM);
	}
}

void CogReaderWallOpeningHelper::MakeWallOpenings(int story)
{
	MSFloor* pFloor = mp_Bldg->GetFloor(story);
	if(pFloor == NULL)	return;

	vector<MSWallMemberG*> wallGs = GetWallGs(pFloor);

	MakeWallOpenings_Line(wallGs);
	MakeWallOpenings_Arc(wallGs);
}

vector<MSWallMemberG*> CogReaderWallOpeningHelper::GetWallGs(MSFloor* pFloor)
{
	vector<MSWallMemberG*> wallGs;

	map<long, MSCompoG*>::iterator it;
	for(it = pFloor->mm_CompoG.begin(); it != pFloor->mm_CompoG.end(); it++)
	{
		MSCompoG* pCompoG = it->second;
		if(pCompoG == NULL)	continue;
		if(pCompoG->GetType() != MSElement::msWallG)	continue;

		MSWallMemberG* pWallG = (MSWallMemberG*)pCompoG;
		if(pWallG->IsBearingWall() == TRUE)
			wallGs.push_back(pWallG);
	}

	return wallGs;
}

MSWallOpeningG* CogReaderWallOpeningHelper::CreateWallOpeningG()
{
	if(mp_Reader->m_WallOpeningMType == MSCompoM::msWindowM)
		return new MSWindowG();
	else if(mp_Reader->m_WallOpeningMType == MSCompoM::msDoorM)
		return new MSDoorG();
	else if(mp_Reader->m_WallOpeningMType == MSCompoM::msOpeningExtraM)
		return new MSWallOpeningExtraG;

	return NULL;
}

void CogReaderWallOpeningHelper::MakeWallOpenings_Line(vector<MSWallMemberG*>& wallGs)
{
	double tolerance = 1.0;
	double offsetTolerance = 200;
	int nWallG = wallGs.size();
	for(int iWallG = 0; iWallG < nWallG; iWallG++)
	{
		MSWallMemberG* pWallG = wallGs[iWallG];
		if(pWallG->isArc())	continue;

		GM2DPolyline boundary;
		pWallG->GetBoundary(boundary);
		GM2DLineVector wallCenter = pWallG->GetCenLVec();

		double maxLength = 0;
		GM2DLineVector* pFindLineVec = NULL;
		int nLine = ma_Lines.size();
		for(int iLine = 0; iLine < nLine; iLine++)
		{
			GM2DLineVector* pLineVec = ma_Lines[iLine];
			if(wallCenter.IsParallel4DTol(*pLineVec, tolerance) == 0)	continue;

			GM2DPolyline* pOffsetPolyline = mp_Reader->MakeOffsetPolyline(&boundary, offsetTolerance, TRUE);

			bool IsContainedP0 = false, IsContainedP1 = false;
			if(pOffsetPolyline->Contains(&pLineVec->m_P0, tolerance) <= 0)
				IsContainedP0 = true;
			if(pOffsetPolyline->Contains(&pLineVec->m_P1, tolerance) <= 0)
				IsContainedP1 = true;

			delete pOffsetPolyline;

			if(IsContainedP0 == false || IsContainedP1 == false)	continue;

			double length = pLineVec->Length();
			if(length > maxLength)
			{
				maxLength = length;
				pFindLineVec = pLineVec;
			}
		}

		if(pFindLineVec != NULL)
		{
			MSCompoM* pOpeningM = FindWallOpeningM(&boundary, pFindLineVec);
			if(pOpeningM == NULL)	continue;
			MakeWallOpenings_Line(pWallG, pFindLineVec, pOpeningM);
		}
	}
}

void CogReaderWallOpeningHelper::MakeWallOpenings_Arc(vector<MSWallMemberG*>& wallGs)
{
	double tolerance = 1.0;
	double offsetTolerance = 200;
	int nWallG = wallGs.size();
	for(int iWallG = 0; iWallG < nWallG; iWallG++)
	{
		MSWallMemberG* pWallG = wallGs[iWallG];
		if(pWallG->isArc() == false)	continue;

		GM2DPolyline boundary;
		pWallG->GetBoundary(boundary);
		shared_ptr<GM2DCurve> wallCurve = pWallG->GetCenCurve();
		GM2DArc* wallArc = (GM2DArc*)&*wallCurve;

		double maxLength = 0;
		GM2DArc* pFindArc = NULL;
		int nArc = ma_Arc.size();
		for(int iArc = 0; iArc < nArc; iArc++)
		{
			GM2DArc* pArc = ma_Arc[iArc];
			if(!wallArc->m_Center.SameVector4DTOL(pArc->m_Center, tolerance))	continue;

			GM2DPolyline* pOffsetPolyline = mp_Reader->MakeOffsetPolyline(&boundary, offsetTolerance, TRUE);

			bool IsContainedP0 = false, IsContainedP1 = false;
			if(pOffsetPolyline->Contains(&pArc->m_P0, tolerance) <= 0)
				IsContainedP0 = true;
			if(pOffsetPolyline->Contains(&pArc->m_P1, tolerance) <= 0)
				IsContainedP1 = true;

			delete pOffsetPolyline;

			if(IsContainedP0 == false || IsContainedP1 == false)	continue;

			double length = pArc->Length();
			if(length > maxLength)
			{
				maxLength = length;
				pFindArc = pArc;
			}
		}

		if(pFindArc != NULL)
		{
			MSCompoM* pOpeningM = FindWallOpeningM(&boundary, pFindArc);
			if(pOpeningM == NULL)	continue;
			MakeWallOpenings_Arc(pWallG, pFindArc, pOpeningM);
		}
	}
}

MSCompoM* CogReaderWallOpeningHelper::FindWallOpeningM(GM2DPolyline* pBoundary, GM2DGeometry* pGeometry)
{
	if(mp_Reader->m_IsWallOpeningAutoM == false)
		return mp_Bldg->GetCompoM(mp_Reader->m_DefaultWallOpeningMID);
	else
	{
		GM2DPolyline* pOffsetPolyline = mp_Reader->MakeOffsetPolyline(pBoundary, mp_Reader->m_DTOL_TEXT_DIST, TRUE);
		vector<GMText*> TextArr = GetGMTexts_ContainPolyline(pOffsetPolyline, mp_Reader->FindCrossQts(pGeometry));
		delete pOffsetPolyline;

		return FindCompoMByCombinationName(TextArr);
	}
}

GMText* CogReaderWallOpeningHelper::GetGMText_ContainPolyline(GM2DPolyline* pPolyline, vector<GMQuadTree*>& CrossQTs)
{
	vector<GMText*> TextArr;
	double dMinDist = mp_Reader->m_DTOL_TEXT_DIST;
	GM2DVector CenVec = pPolyline->MassCenter();
	long NumCrossQT = CrossQTs.size();

	bool bFirstFind = false;
	GMText* pRtnText = NULL;
	if(NumCrossQT == 0)
	{
		CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = mp_Reader->GetGeometry(DG_TEXT);	
		long NumGMs = TxtGMs.GetSize();
		for(long iGMs=0; iGMs<NumGMs; iGMs++)
		{
			GMText* TheGM = (GMText*)TxtGMs[iGMs];
			if(TheGM==NULL) continue;

			bool IsContain = false;
			if(pPolyline->Contains(&TheGM->m_Vec) != 1 )	// 1:폴리곤 밖
				IsContain = true;
			if(IsContain == false)
			{
				if(pPolyline->Contains(&TheGM->m_InsertVec) != 1)
					IsContain = true;
			}

			if(IsContain == false)
				continue;

			CString Name = TheGM->m_strText.MakeUpper();
			Name.Replace(_T(" "), _T(""));
			MSCompoM* pCompoM = mp_Bldg->GetCompoM(Name, mp_Reader->m_WallOpeningMType);
			if(pCompoM==NULL)	continue;

			double dCenDist = CenVec.Distance(TheGM->m_Vec);
			double dInsertDist = CenVec.Distance(TheGM->m_InsertVec);
			double TextDist = min(dCenDist, dInsertDist);
			if(bFirstFind == false)
			{
				bFirstFind = true;	dMinDist = TextDist;	pRtnText = TheGM;	continue;
			}

			if(TextDist < dMinDist)
			{
				dMinDist = TextDist;
				pRtnText = TheGM;
			}
		}
	}
	else
	{
		for(long iCrossQT=0; iCrossQT<NumCrossQT; iCrossQT++) 
		{
			GMQuadTree* pQT = CrossQTs[iCrossQT];
			CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = pQT->ma_GMs[DG_TEXT];
			long NumGMs = TxtGMs.GetSize();
			for(long iGMs=0; iGMs<NumGMs; iGMs++)
			{
				GMText* TheGM = (GMText*)TxtGMs[iGMs];
				if(TheGM==NULL) continue;

				bool IsContain = false;
				if(pPolyline->Contains(&TheGM->m_Vec) != 1 )	// 1:폴리곤 밖
					IsContain = true;
				if(IsContain == false)
				{
					if(pPolyline->Contains(&TheGM->m_InsertVec) != 1)
						IsContain = true;
				}

				if(IsContain == false)
					continue;

				CString Name = TheGM->m_strText.MakeUpper();
				Name.Replace(_T(" "), _T(""));
				MSCompoM* pCompoM = mp_Bldg->GetCompoM(Name, mp_Reader->m_WallOpeningMType);
				if(pCompoM==NULL)	continue;

				double dCenDist = CenVec.Distance(TheGM->m_Vec);
				double dInsertDist = CenVec.Distance(TheGM->m_InsertVec);
				double TextDist = min(dCenDist, dInsertDist);
				if(bFirstFind == false)
				{
					bFirstFind = true;	dMinDist = TextDist;	pRtnText = TheGM;	continue;
				}

				if(TextDist < dMinDist)
				{
					dMinDist = TextDist;
					pRtnText = TheGM;
				}
			}
		}
	}

	return pRtnText;
}

vector<GMText*> CogReaderWallOpeningHelper::GetGMTexts_ContainPolyline(GM2DPolyline* pPolyline, vector<GMQuadTree*>& CrossQTs)
{
	vector<GMText*> TextArr;
	double dMinDist = mp_Reader->m_DTOL_TEXT_DIST;
	GM2DVector CenVec = pPolyline->MassCenter();
	long NumCrossQT = CrossQTs.size();

	if(NumCrossQT == 0)
	{
		CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = mp_Reader->GetGeometry(DG_TEXT);	
		long NumGMs = TxtGMs.GetSize();
		for(long iGMs=0; iGMs<NumGMs; iGMs++)
		{
			GMText* TheGM = (GMText*)TxtGMs[iGMs];
			if(TheGM==NULL) continue;

			bool IsContain = false;
			if(pPolyline->Contains(&TheGM->m_Vec) != 1 )	// 1:폴리곤 밖
				IsContain = true;
			if(IsContain == false)
			{
				if(pPolyline->Contains(&TheGM->m_InsertVec) != 1)
					IsContain = true;
			}

			if(IsContain == false)
				continue;

			TextArr.push_back(TheGM);
		}
	}
	else
	{
		for(long iCrossQT=0; iCrossQT<NumCrossQT; iCrossQT++) 
		{
			GMQuadTree* pQT = CrossQTs[iCrossQT];
			CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = pQT->ma_GMs[DG_TEXT];
			long NumGMs = TxtGMs.GetSize();
			for(long iGMs=0; iGMs<NumGMs; iGMs++)
			{
				GMText* TheGM = (GMText*)TxtGMs[iGMs];
				if(TheGM==NULL) continue;

				bool IsContain = false;
				if(pPolyline->Contains(&TheGM->m_Vec) != 1 )	// 1:폴리곤 밖
					IsContain = true;
				if(IsContain == false)
				{
					if(pPolyline->Contains(&TheGM->m_InsertVec) != 1)
						IsContain = true;
				}

				if(IsContain == false)
					continue;

				TextArr.push_back(TheGM);
			}
		}
	}

	return TextArr;
}
MSCompoM* CogReaderWallOpeningHelper::FindCompoMByCombinationName(vector<GMText*>& TextArr)
{
	int NumOpeningM = ma_OpeningM.size();
	for(int iM = 0; iM < NumOpeningM; iM++)
	{
		MSCompoM* pCompoM = ma_OpeningM[iM];
		CString Name = pCompoM->m_Name;

		for(int i = 0; i < TextArr.size(); i++)
		{
			CString iText = TextArr[i]->m_strText;
			iText = iText.MakeUpper();
			iText.Replace(_T(" "), _T(""));
			if(iText == Name)
				return pCompoM;

			for(int j = i+1; j < TextArr.size(); j++)
			{
				CString jText = TextArr[j]->m_strText;
				jText = jText.MakeUpper();
				jText.Replace(_T(" "), _T(""));

				CString CompareName = iText+jText;
				if(Name == CompareName)
					return pCompoM;
				CompareName = jText+iText;
				if(Name == CompareName)
					return pCompoM;
			}
		}
	}
	return NULL;//return mp_Reader->GetDefaultOpeningM();
}

void CogReaderWallOpeningHelper::MakeWallOpenings_Line(MSWallMemberG* pWallG, GM2DLineVector* pLineVec, MSCompoM* pCompoM)
{
	GM2DLineVector cenVec = pWallG->GetCenLVec();
	double wallDegree = cenVec.GetDegree();
	double lineDegree = pLineVec->GetDegree();
	if(wallDegree * lineDegree < 0)
		pLineVec->ReverseDirection();

	MSFloor* pFloor = (MSFloor*)pWallG->GetOwner();
	if (pFloor == nullptr)	return;

	MSOpeningM* pOpeningM = (MSOpeningM*)pCompoM;
	MSWallOpeningG* pOpeningG = CreateWallOpeningG();

	if(fabs(pOpeningM->GetWidth() - pLineVec->Length()) > DTOL1)
	{
		AddErrorObject(pWallG, W_CODE_SIZE_DIFF, WARNING_LEVEL, _T("Opening"), WARNING_MSIZE_DIFF);
	}

	pOpeningG->SetOwner(pFloor);

	GM2DVector OrthoVecP0 = cenVec.OrthoBaseVector(pLineVec->m_P0);
	GM2DVector OrthoVecP1 = cenVec.OrthoBaseVector(pLineVec->m_P1);

	pOpeningG->SetCompoM(pOpeningM);
	pOpeningG->m_BaseOffset.m_X = OrthoVecP0.Distance(cenVec.m_P0);
	pOpeningG->m_BaseOffset.m_Y = GetBottomOffset(pFloor);

	if (mp_Reader->m_WallOpeningLevelType == 1)
		pOpeningG->m_IsUseFloorLevel = false;
	else
		pOpeningG->m_IsUseFloorLevel = true;

	mp_Reader->AddWallOpeningG(pWallG, pOpeningG);
}

void CogReaderWallOpeningHelper::MakeWallOpenings_Arc(MSWallMemberG* pWallG, GM2DArc* pArc, MSCompoM* pCompoM)
{
	shared_ptr<GM2DCurve> wallCurve = pWallG->GetCenCurve();
	GM2DArc* wallArc = (GM2DArc*)&*wallCurve;

	GM2DLineVector aLineVec;	
	aLineVec.m_P0 = pArc->m_P0;	aLineVec.m_P1 = pArc->m_Center;

	GM2DVector crossVec1, crossVec2;
	long rtnVal = aLineVec.IntersectArc(*wallArc, crossVec1, crossVec2);
	if(rtnVal == 0)	return;

	GM2DArc aArc;
	aArc.SetCenAnd2P(wallArc->m_Center, wallArc->m_P0, crossVec1);

	MSOpeningM* pOpeningM = (MSOpeningM*)pCompoM;
	MSWallOpeningG* pOpeningG = CreateWallOpeningG();

 	if(fabs(pOpeningM->GetWidth() - aArc.GetArcLength()) > DTOL1)
 	{
 		AddErrorObject(pWallG, W_CODE_SIZE_DIFF, WARNING_LEVEL, _T("Opening"), WARNING_MSIZE_DIFF);
 	}

	MSFloor* pFloor = (MSFloor*)pWallG->GetOwner();
	pOpeningG->SetOwner(pFloor);
	pOpeningG->SetCompoM(pOpeningM);

	pOpeningG->m_BaseOffset.m_X = aArc.GetArcLength();
	pOpeningG->m_BaseOffset.m_Y = GetBottomOffset(pFloor);

	if (mp_Reader->m_WallOpeningLevelType == 1)
		pOpeningG->m_IsUseFloorLevel = false;
	else
		pOpeningG->m_IsUseFloorLevel = true;

	mp_Reader->AddWallOpeningG(pWallG, pOpeningG);
}

double CogReaderWallOpeningHelper::GetBottomOffset(MSFloor* pFloor)
{
	return mp_Reader->m_WallOpeningBottomLevel;
// 	int floorLevel = 0;
// 	if(mp_Reader->m_WallOpeningLevelType == 1)
// 		floorLevel += pFloor->GetSlabLevelOffset();
// 
// 	return floorLevel + mp_Reader->m_WallOpeningBottomLevel;
}