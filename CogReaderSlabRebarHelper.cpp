#include "StdAfx.h"
#include "CogReaderSlabRebarHelper.h"
#include "CogReader.h"
#include "SlabRebarOption.h"

#include "GMLib/GMQuadTree.h"
#include "GMLib/GMText.h"
#include "GMLib/GM2DBound.h"

#include "StructureModel/MSDeformedBar.h"
#include "StructureModel/MSRebarBuilding.h"

CogReaderSlabRebarHelper::CogReaderSlabRebarHelper(MSBaseBuilding* pBldg, CogReader* pReader, MSFloor* pFloor) : CogReaderHelper(pBldg, pReader)
{
	mp_Floor = pFloor;
	m_RebarTextHeight = 100;
	m_pInfoLine = NULL;
	m_pPairRebarLine = NULL;
	m_IsExtendP0 = false;
	m_IsExtendP1 = false;
}


CogReaderSlabRebarHelper::~CogReaderSlabRebarHelper(void)
{
	if(m_pInfoLine)
		delete m_pInfoLine;
}

void CogReaderSlabRebarHelper::ClearTempVariables()
{
	m_IsExtendP0 = false;
	m_IsExtendP1 = false;
	mm_FindGeom.clear();
	if(m_pInfoLine)
		delete m_pInfoLine;
	m_pInfoLine = NULL;
	m_pPairRebarLine = NULL;
	ma_Circle.clear();
	m_eErrorType = CogReader::NOT_FIND_INFOLINE;
}

void CogReaderSlabRebarHelper::MakeData()
{
//	MergeLineSegments();
	MakeRebarInfos();
}

void CogReaderSlabRebarHelper::MergeLineSegments()
{
	CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms = mp_Reader->GetGeometry(DG_REBARZONE);
	int NumGeoms = Geoms.GetSize();

	double degreeTol = 1.0;
	double distanceTol = 1.0;
	double linkTestLength = 1.0;
	for(int iGeom = 0; iGeom < NumGeoms; iGeom++)
	{
		GM2DGeometry* pIGeom = Geoms[iGeom];
		if(pIGeom->m_ObjType != GT_LINE)	continue;
		for(int jGeom = iGeom+1; jGeom < NumGeoms; jGeom++)
		{
			GM2DGeometry* pJGeom = Geoms[jGeom];
			if(pJGeom->m_ObjType != GT_LINE)	continue;

			GM2DLineVector* pILineVec = (GM2DLineVector*)pIGeom;
			GM2DLineVector* pJLineVec = (GM2DLineVector*)pJGeom;

			pILineVec->MergeWhenColinear(pJLineVec, degreeTol, distanceTol, linkTestLength);
		}
	}
}

void CogReaderSlabRebarHelper::SetInfoLine(GM2DLineVector* pLineVec)
{
	if(m_pInfoLine)	delete m_pInfoLine;
	m_pInfoLine = new GM2DLineVector(*pLineVec);
}

void CogReaderSlabRebarHelper::MakeRebarInfos()
{
	MakeRebarInfosByPolyline();
	MakeRebarInfosByInfoLine();
}

void CogReaderSlabRebarHelper::MakeRebarInfosByPolyline()
{
	SortSlabRebarInfoByArea();
	int numZone = mp_Reader->ma_SlabRebarArea.GetSize();
	CTypedPtrArray<CObArray, GM2DGeometry*>& RebarLines = mp_Reader->GetGeometry(DG_REBARLINE);
	int NumRebarLine = RebarLines.GetSize();

	for(int iRebar = 0; iRebar < NumRebarLine; iRebar++)
	{
		GM2DGeometry* pGeom = RebarLines[iRebar];
		if(pGeom->m_ObjType != GT_LINE)	continue;
		if(pGeom->m_Tag == 1)	continue;

		GM2DLineVector* pRebarLine = (GM2DLineVector*)pGeom;
		if(pRebarLine->m_Tag != 0)	continue;
		for(int iZone = 0; iZone < numZone; iZone++)
		{
			GM2DPolyline* pBoundary = mp_Reader->ma_SlabRebarArea[iZone];
			if(!IsIncludeRebarLine(pBoundary, pRebarLine))	continue;

			CString rebarText = _T("");
			if (!FindRebarText(pRebarLine, rebarText))
			{
				if (mp_Reader->GetSlabRebarOption()->GetIsUsedDefaultBar())
					rebarText = mp_Reader->GetSlabRebarOption()->GetDefaultBar();
				else
				{
					m_eErrorType = CogReader::NOT_FIND_TEXT;
					continue;
				}
			}

			ClearTempVariables();
			MakeSlabRebar(pBoundary, pRebarLine, rebarText);
			pRebarLine->m_Tag = 1;
		}
	}
}

void CogReaderSlabRebarHelper::MakeRebarInfosByInfoLine()
{
	CTypedPtrArray<CObArray, GM2DGeometry*>& RebarLines = mp_Reader->GetGeometry(DG_REBARLINE);
	int NumRebarLine = RebarLines.GetSize();
	for(int iRebar = 0; iRebar < NumRebarLine; iRebar++)
	{
		GM2DGeometry* pGeom = RebarLines[iRebar];
		if(pGeom->m_ObjType != GT_LINE)	continue;

		GM2DLineVector* pRebarLine = (GM2DLineVector*)pGeom;
		if(pRebarLine->m_Tag != 0)	continue;

		ClearTempVariables();
		MakeRebarInfo(pRebarLine);
	}
}

void CogReaderSlabRebarHelper::MakeRebarInfo(GM2DLineVector* pRebarLine)
{
	CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms = mp_Reader->GetGeometry(DG_INFOLINE);
	ma_Circle = FindCirclesOnRebarLine(pRebarLine, Geoms);

	GM2DLineVector* pInfoLine = FindNearestInfoLine(pRebarLine);
	if (pInfoLine == nullptr)	return;

	SetInfoLine(pInfoLine);
	CString rebarText = _T("");
	pInfoLine->m_Tag = 1;
	if (!FindRebarText(pRebarLine, rebarText))
	{
		if (mp_Reader->GetSlabRebarOption()->GetIsUsedDefaultBar())
			rebarText = mp_Reader->GetSlabRebarOption()->GetDefaultBar();
		else
		{
			pInfoLine->m_Tag = 0;
			m_eErrorType = CogReader::NOT_FIND_TEXT;
			mp_Reader->MakeErrorRange(pRebarLine, m_eErrorType, m_RebarTextHeight);
			return;
		}
	}
	FindPairRebarLine(pRebarLine);

	ExtendInfoLineByArrow(m_pInfoLine);
	MakeSlabRebar(pRebarLine, m_pInfoLine, rebarText);
	pInfoLine->m_Tag = 0;
}

bool CogReaderSlabRebarHelper::FindStartTieVector(GM2DLineVector* pRebarLine, GM2DVector& startVec)
{
	double tolerance = mp_Reader->GetSlabRebarOption()->GetLeaderDist();

	bool IsFirst = true;
	double minDist = tolerance;
	CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms = mp_Reader->GetGeometry(DG_REBARZONE);
	for (int iGeom = 0; iGeom < Geoms.GetSize(); iGeom++)
	{
		GM2DGeometry* pGeom = Geoms[iGeom];
		if (pGeom->m_ObjType != GT_LINE)	continue;
		if (pGeom->m_Tag == 1)	continue;

		GM2DLineVector* pTieLine = (GM2DLineVector*)pGeom;
		GM2DVector ContinusVec;
		if (pRebarLine->IncludeVector4DTol(pTieLine->m_P0, tolerance))
		{
			if (IsFirst)
			{
				startVec = pTieLine->m_P0;
				minDist = pRebarLine->Distance(startVec);
				IsFirst = false;
			}
			else
			{
				double dist = pRebarLine->Distance(pTieLine->m_P0);
				if (dist < minDist)
				{
					startVec = pTieLine->m_P0;
					minDist = dist;
				}
			}
		}
		else if (pRebarLine->IncludeVector4DTol(pTieLine->m_P1, tolerance))
		{
			if (IsFirst)
			{
				startVec = pTieLine->m_P1;
				minDist = pRebarLine->Distance(startVec);
				IsFirst = false;
			}
			else
			{
				double dist = pRebarLine->Distance(pTieLine->m_P1);
				if (dist < minDist)
				{
					startVec = pTieLine->m_P1;
					minDist = dist;
				}
			}
		}
	}
	if (IsFirst == false)	return true;
	return false;
}
GM2DLineVector* CogReaderSlabRebarHelper::FindNearestInfoLine(GM2DLineVector* pRebarLine)
{
	double tolerance = mp_Reader->GetSlabRebarOption()->GetLeaderDist();

	CTypedPtrArray<CObArray, GM2DGeometry*>& InfoLines = mp_Reader->GetGeometry(DG_INFOLINE);
	CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms = mp_Reader->GetGeometry(DG_REBARZONE);

	GM2DVector startVec;
	if (!FindStartTieVector(pRebarLine, startVec))	return nullptr;

	GM2DLineVector* pNearestInfoLine = nullptr;
	double minDist = 10e10;
	for (int iInfo = 0; iInfo < InfoLines.GetSize(); iInfo++)
	{
		GM2DGeometry* pGeom = InfoLines[iInfo];
		if (pGeom->m_ObjType != GT_LINE)	continue;

		GM2DLineVector* pInfoLine = (GM2DLineVector*)pGeom;
		if (!IsInfoLine(pRebarLine, pInfoLine))	continue;

		double dist = pInfoLine->Distance(startVec);
		if (dist < minDist)
		{
			pNearestInfoLine = pInfoLine;
			minDist = dist;
		}
	}
	return pNearestInfoLine;
}
double CogReaderSlabRebarHelper::GetDegreeTolerance(double degree)
{
	double tolerance = DTOL1;
	if(fabs(degree) < DTOL1 || fabs(degree - 90) < DTOL1 || fabs(degree - 180) < DTOL1 || fabs(degree - 270) < DTOL1 || fabs(degree - 360) < DTOL1)
		return tolerance;
	
	tolerance = 1.0;
	return tolerance;
}

bool CogReaderSlabRebarHelper::IsInfoLine(GM2DLineVector* pMyLine, GM2DLineVector* pUrLine)
{
	GM2DVector CrossVec;
	double MyLoc = 0.0, UrLoc = 0.0;
	if(pMyLine->IntersectEndWithLine(*pUrLine, CrossVec, &MyLoc, &UrLoc) == FALSE)	
		return false;

	// 1. 외부 교점 또는 끝에서 만나는 선 제외
	if(MyLoc < DTOL5 || MyLoc > 1.0-DTOL5)
		return false;

	// 2. 90도 각도로 만나지 않는 선 제외
// 	if(!pMyLine->IsPerpendicular(*pUrLine))
// 		return false;
	double MyDegree = pMyLine->GetDegree();
	double DegreeTolerance = GetDegreeTolerance(MyDegree);
	
 	double Degree = fabs(pMyLine->GetDegree() - pUrLine->GetDegree());
 	if(!(fabs(Degree - 90) < DegreeTolerance || fabs(Degree - 270) < DegreeTolerance))
 		return false;

	// 3. 두 라인이 서로 안쪽에서 교차
	if(UrLoc > DTOL5 && UrLoc < 1.0-DTOL5)
	{
		double DistFromP0 = pUrLine->m_P0.Distance(CrossVec);
		double DistFromP1 = pUrLine->m_P1.Distance(CrossVec);

		// 3.1 교차점과 다른 점의 길이가 Tolerance이하이면 제외(Tolerance?)
		GM2DCircle* pMaxCircle = GetMaxCircleOnRebarLine();
		if(pMaxCircle)
		{
			double Tolerance = pMaxCircle->m_A*2;;
			if(DistFromP0 < Tolerance || DistFromP1 < Tolerance)
				return false;
		}
		return true;
	}

	// 4. 중간 원으로 연결되었는지 판단  ---0---
	GM2DVector CrossVec1, CrossVec2;
	int NumCircle = ma_Circle.size();
	for(int iCircle = 0; iCircle < NumCircle; iCircle++)
	{
		GM2DCircle* pCircle = ma_Circle[iCircle];
		if(pCircle->Intersect(*pUrLine, CrossVec1, CrossVec2) != 0)
		{
			if(FindOtherInfoLine(pCircle, pUrLine))
				return true;
		}
	}

	return false;
}

GM2DCircle* CogReaderSlabRebarHelper::GetMaxCircleOnRebarLine()
{
	GM2DCircle* pMaxCircle = NULL;
	int NumCircle = ma_Circle.size();
	for(int iCircle = 0; iCircle < NumCircle; iCircle++)
	{
		GM2DCircle* pCircle = ma_Circle[iCircle];
		if(pMaxCircle == NULL)
		{
			pMaxCircle = pCircle;
			continue;
		}
		if(pCircle->m_A > pMaxCircle->m_A)
			pMaxCircle = pCircle;
	}
	return pMaxCircle;
}

bool CogReaderSlabRebarHelper::FindOtherInfoLine(GM2DCircle* pCircle, GM2DLineVector* pMyLine)
{
	CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms = mp_Reader->GetGeometry(DG_INFOLINE);
	for(int iGeom = 0; iGeom < Geoms.GetSize(); iGeom++)
	{
		GM2DGeometry* pGeom = Geoms[iGeom];
		if(pGeom->m_ObjType != GT_LINE)	continue;
		if(pGeom->m_Tag == 1)	continue;

		GM2DLineVector* pUrLine = (GM2DLineVector*)pGeom;
		if(pUrLine == pMyLine)	continue;

		GM2DVector CrossVec1, CrossVec2;
		if(pCircle->Intersect(*pUrLine, CrossVec1, CrossVec2) != 0)
		{
			double linkTestLength = pCircle->m_A*2;
			double distTolerance = 1.0;
			if(pMyLine->IsColinear4DTol(*pUrLine, 1.0, distTolerance))
			{
				MergeInfoLine(pMyLine, pUrLine, linkTestLength);
				pUrLine->m_Tag = 1;
				return true;
			}
		}
	}
	return false;
}

void CogReaderSlabRebarHelper::MergeInfoLine(GM2DLineVector* pFirstLine, GM2DLineVector* pSecondLine, double linkTestLength)
{
	double degreeTolerance = 1.0;
	double distTolerance = 1.0;
	m_pInfoLine->m_P0 = pFirstLine->m_P0;
	m_pInfoLine->m_P1 = pFirstLine->m_P1;
	m_pInfoLine->MergeWhenColinear(pSecondLine, degreeTolerance, distTolerance, linkTestLength);
// 
// 	double MinX, MaxX, MinY, MaxY;
// 
// 	vector<double> ValueArr;
// 	ValueArr.push_back(pFirstLine->m_P0.m_X);	ValueArr.push_back(pFirstLine->m_P1.m_X);
// 	ValueArr.push_back(pSecondLine->m_P0.m_X);	ValueArr.push_back(pSecondLine->m_P1.m_X);
// 
// 	MinX = GetMinValue(ValueArr);
// 	MaxX = GetMaxValue(ValueArr);
// 
// 	ValueArr.clear();
// 	ValueArr.push_back(pFirstLine->m_P0.m_Y);	ValueArr.push_back(pFirstLine->m_P1.m_Y);
// 	ValueArr.push_back(pSecondLine->m_P0.m_Y);	ValueArr.push_back(pSecondLine->m_P1.m_Y);
// 	MinY = GetMinValue(ValueArr);
// 	MaxY = GetMaxValue(ValueArr);
// 
// 	m_pInfoLine->m_P0.SetXY(MinX, MinY);
// 	m_pInfoLine->m_P1.SetXY(MaxX, MaxY);
}

double CogReaderSlabRebarHelper::GetMinValue(vector<double>& ValueArr)
{
	int MinValue = 10e10;
	for(int iValue = 0; iValue < ValueArr.size(); iValue++)
	{
		if(MinValue > ValueArr[iValue])
			MinValue = ValueArr[iValue];
	}
	return MinValue;
}

double CogReaderSlabRebarHelper::GetMaxValue(vector<double>& ValueArr)
{
	int MaxValue = -10e10;
	for(int iValue = 0; iValue < ValueArr.size(); iValue++)
	{
		if(MaxValue < ValueArr[iValue])
			MaxValue = ValueArr[iValue];
	}
	return MaxValue;

}

void CogReaderSlabRebarHelper::FindPairRebarLine(GM2DLineVector* pRebarLine)
{
	GM2DCircle* pMaxCircle = GetMaxCircleOnRebarLine();
	if(pMaxCircle == NULL)	return;

	CTypedPtrArray<CObArray, GM2DGeometry*>& RebarLines = mp_Reader->GetGeometry(DG_REBARLINE);
	int NumRebarLine = RebarLines.GetSize();
	for(int iRebarLine = 0; iRebarLine < NumRebarLine; iRebarLine++)
	{
		GM2DGeometry* pGeom = RebarLines[iRebarLine];
		if(pGeom->m_ObjType != GT_LINE)	continue;
		if(pGeom == pRebarLine)	continue;

		GM2DLineVector* pOtherRebarLine = (GM2DLineVector*)pGeom;
		GM2DVector CrossVec1, CrossVec2;
		if(pMaxCircle->Intersect(*pOtherRebarLine, CrossVec1, CrossVec2) == 2)
		{
			m_pPairRebarLine = pOtherRebarLine;
			pOtherRebarLine->m_Tag = 1;
			break;
		}
	}
}

void CogReaderSlabRebarHelper::ExtendInfoLineByArrow(GM2DLineVector* pInfoLine)
{
	CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms = mp_Reader->GetGeometry(DG_INFOLINE);
	map<int, SlabRebarHelper_LocToCount*> LocToCountMap;
	for(int iGeom = 0; iGeom < Geoms.GetSize(); iGeom++)
	{
		GM2DGeometry* pGeom = Geoms[iGeom];
		if(pGeom->m_ObjType != GT_LINE)	continue;
		if(pGeom->m_Tag == 1)	continue;

		GM2DLineVector* pLineVec = (GM2DLineVector*)pGeom;
		if(!IsArrowLineVector(pInfoLine, pLineVec, LocToCountMap))	continue;
	}

	ExtendInfoLineByLoc(pInfoLine, LocToCountMap);
	DeleteLocToCountMap(LocToCountMap);
}

bool CogReaderSlabRebarHelper::IsArrowLineVector(GM2DLineVector* pInfoLine, GM2DLineVector* pLineVec, map<int, SlabRebarHelper_LocToCount*>& LocToCountMap)
{
	GM2DVector CrossVec;
	double MyLoc=0.0, UrLoc = 0.0;
	if(pInfoLine->IntersectEndWithLine(*pLineVec, CrossVec, &MyLoc, &UrLoc) == FALSE)
		return false;

	if(-DTOL5 < MyLoc && MyLoc < 1+DTOL5)	return false;
	if(-DTOL1 > UrLoc || UrLoc > 1+DTOL1)	return false;

	double DistTolerance = 0.0;
	if(MyLoc < 0.5)
		DistTolerance = pInfoLine->m_P0.Distance(CrossVec);
	else
		DistTolerance = pInfoLine->m_P1.Distance(CrossVec);

	if(DistTolerance > m_RebarTextHeight*1.5)	return false;

	GM2DLineVector DirInfoLVec;
	DirInfoLVec.SetP0(CrossVec);
	if(CrossVec.Distance(pInfoLine->m_P0) < CrossVec.Distance(pInfoLine->m_P1))
		DirInfoLVec.SetP1(pInfoLine->m_P1);
	else
		DirInfoLVec.SetP1(pInfoLine->m_P0);

	GM2DLineVector DirArrawLVec;
	DirArrawLVec.SetP0(CrossVec);
	if(CrossVec.Distance(pLineVec->m_P0) < CrossVec.Distance(pLineVec->m_P1))
		DirArrawLVec.SetP1(pLineVec->m_P1);
	else
		DirArrawLVec.SetP1(pLineVec->m_P0);

	if (DirInfoLVec.UnitVector() * DirArrawLVec.UnitVector() <= 0)
		return false;

	int key = ROUND((MyLoc*100));	// Key값을 int로 하기위해 소수점 두째자리에서 반올림
	map<int, SlabRebarHelper_LocToCount*>::iterator it = LocToCountMap.find(key);
	if(it == LocToCountMap.end())
	{
		SlabRebarHelper_LocToCount* pLocToCount = new SlabRebarHelper_LocToCount();
		pLocToCount->m_Loc = MyLoc;
		LocToCountMap.insert(make_pair(key, pLocToCount));
	}
	else
		it->second->IncreaseCount();
	return true;
}

void CogReaderSlabRebarHelper::ExtendInfoLineByLoc(GM2DLineVector* pInfoLine, map<int, SlabRebarHelper_LocToCount*>& LocToCountMap)
{
	double MinLoc = 0, MaxLoc = 1;
	map<int, SlabRebarHelper_LocToCount*>::iterator it;
	for(it = LocToCountMap.begin(); it != LocToCountMap.end(); it++)
	{
		if(it->second->m_Count < 2)	continue;

		SlabRebarHelper_LocToCount* pLocToCount = it->second;
		if(MinLoc > pLocToCount->m_Loc)	MinLoc = pLocToCount->m_Loc;
		if(MaxLoc < pLocToCount->m_Loc)	MaxLoc = pLocToCount->m_Loc;
	}

	GM2DVector newP0 = pInfoLine->PointAtParam(MinLoc);
	GM2DVector newP1 = pInfoLine->PointAtParam(MaxLoc);
	if(MinLoc < -DTOL5)
	{
		m_IsExtendP0 = true;
		m_ExtendP0 = newP0;
	}
	if(MaxLoc > 1+DTOL5)
	{
		m_IsExtendP1 = true;
		m_ExtendP1 = newP1;
	}
}

void CogReaderSlabRebarHelper::DeleteLocToCountMap(map<int, SlabRebarHelper_LocToCount*>& LocToCountMap)
{
	map<int, SlabRebarHelper_LocToCount*>::iterator it;
	for(it = LocToCountMap.begin(); it != LocToCountMap.end(); it++)
		delete it->second;
	LocToCountMap.clear();
}

bool CogReaderSlabRebarHelper::FindRebarText(GM2DLineVector* pRebarLine, CString& rebarText)
{
	double dMinDist = 10e10;
	GMText* pFindText = NULL;

	CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms = mp_Reader->GetGeometry(DG_REBARZONE);

	GM2DLineVector* pTieLine = NULL;
	GMText* pGMText = FindRebarText(pRebarLine, Geoms);
	if(pGMText == NULL)	return false;
	 
	double MidDist = pRebarLine->Distance(pGMText->m_Vec);

	if (dMinDist > MidDist)
	{
		pFindText = pGMText;
		dMinDist = MidDist;
	}

	if (pFindText)
	{
		rebarText = pFindText->m_strText;
		// 		pFindText->m_Tag = 1;
		m_RebarTextHeight = pFindText->m_dHeight;
		return true;
	}
	return false;
}

GMText* CogReaderSlabRebarHelper::FindRebarText(GM2DLineVector* pRebarLine, CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms)
{
	GMText* pGMText = FindRebarTextStartCircle(pRebarLine, Geoms);
	if(pGMText == NULL)
		pGMText = FindRebarTextStartLine(pRebarLine, Geoms);
	if (pGMText == NULL)
	{
		pGMText = FindRebarTextInTolerance(pRebarLine);
		if (pGMText != NULL)
			m_eErrorType = CogReader::FINDTEXT_BY_DISTANCE;
	}

	return pGMText;
}
GMText* CogReaderSlabRebarHelper::FindRebarTextInTolerance(GM2DLineVector* pRebarLine)
{
	double MinDist = mp_Reader->GetSlabRebarOption()->GetLeaderDist();
	double degreeTolerance = 1.0;
	double radianTolerance = degreeTolerance * Deg2Rad;
	GMText* pFindText = NULL;
	CTypedPtrArray<CObArray, GM2DGeometry*>& TextArr = mp_Reader->GetGeometry(DG_TEXT);
	for (int iText = 0; iText < TextArr.GetSize(); iText++)
	{
		GMText* pText = (GMText*)TextArr[iText];
		if (pText->m_Tag == 1)	continue;
		if (!IsRebarText(pText->m_strText))	continue;
		double Degree = pRebarLine->GetDegree();
		double lineRadian = Degree * Deg2Rad;
		double textRadian = pText->m_dDegree * Deg2Rad;
		if (fabs(lineRadian - textRadian) < radianTolerance || fabs(2 * PI - fabs(lineRadian - textRadian)) < radianTolerance)
		{
			double MidDist = pRebarLine->Distance(pText->m_Vec);
			double OriginDist = pRebarLine->Distance(pText->m_Origin);
			double CornerDist = pRebarLine->Distance(pText->m_Corner);

			double TextTolerance = pText->m_dHeight * 2;

			vector<double> DistArr;
			DistArr.push_back(MidDist);	DistArr.push_back(OriginDist);	DistArr.push_back(CornerDist);
			double TextDist = GetMinValue(DistArr);

			if (TextTolerance > TextDist)
			{
				if (pFindText == NULL)
				{
					MinDist = TextDist;
					pFindText = pText;
					continue;
				}
			}

			if (TextDist < MinDist)
			{
				MinDist = TextDist;
				pFindText = pText;
			}
		}
	}
	return pFindText;
}

GMText* CogReaderSlabRebarHelper::FindRebarTextStartCircle(GM2DLineVector* pRebarLine, CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms)
{
	int NumCircle = ma_Circle.size();
	for(int iCircle = 0; iCircle < NumCircle; iCircle++)
	{
		GM2DCircle* pCircle = ma_Circle[iCircle];
		GM2DLineVector* pTieLine = FindTieLineOnCircle(pCircle, Geoms);
		if(pTieLine == NULL)	continue;

		GM2DVector ContinueVec;
		double DistP0FromCenter = pCircle->m_Center.Distance(pTieLine->m_P0);
		double DistP1FromCenter = pCircle->m_Center.Distance(pTieLine->m_P1);
		if(DistP0FromCenter < DistP1FromCenter)
			ContinueVec = pTieLine->m_P1;
		else
			ContinueVec = pTieLine->m_P0;

		GMText* pText = FindRebarTextEndTieLine(pTieLine, &ContinueVec, Geoms);
		if(pText == NULL)	continue;
		if(!IsRebarText(pText->m_strText))	continue;

		return pText;
	}
	return NULL;
}

GM2DLineVector* CogReaderSlabRebarHelper::FindTieLineOnCircle(GM2DCircle* pCircle, CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms)
{
	int NumGeom = Geoms.GetSize();
	for(int iGeom = 0; iGeom < NumGeom; iGeom++)
	{
		GM2DGeometry* pGeom = Geoms[iGeom];
		if(pGeom->m_ObjType != GT_LINE)	continue;
		if(pGeom->m_Tag	 == 1)	continue;

		GM2DLineVector* pLineVec = (GM2DLineVector*)pGeom;
		GM2DVector CrossVec1, CrossVec2;
		if(pCircle->Intersect(*pLineVec, CrossVec1, CrossVec2) == 0)	continue;

		return pLineVec;
	}
	return NULL;
}

GMText* CogReaderSlabRebarHelper::FindRebarTextEndTieLine(GM2DLineVector* pTieLine, GM2DVector* pContinueVec, CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms)
{
	if(mm_FindGeom.find(pTieLine) != mm_FindGeom.end())
		return NULL;

	mm_FindGeom.insert(make_pair(pTieLine, 1));
	GMText* pText = NULL;
	double tolerance = 10.0;
	GM2DCircle* pMaxCircle = GetMaxCircleOnRebarLine();
	if(pMaxCircle)
		tolerance = pMaxCircle->GetRadius();

	int NumGeom = Geoms.GetSize();
	for(int iGeom = 0; iGeom < NumGeom; iGeom++)
	{
		GM2DGeometry* pGeom = Geoms[iGeom];
		if(pGeom->m_ObjType != GT_LINE)	continue;
		if(pGeom == pTieLine)	continue;
		if(pGeom->m_Tag == 1)	continue;

		GM2DLineVector* pLineVec = (GM2DLineVector*)pGeom;
		GM2DVector FindVec;
		if(pLineVec->m_P0.SameVector4DTOL(*pContinueVec, tolerance))
			FindVec = pLineVec->m_P1;
		else if(pLineVec->m_P1.SameVector4DTOL(*pContinueVec, tolerance))
			FindVec = pLineVec->m_P0;
		else
			continue;

		pText = FindRebarTextEndTieLine(pLineVec, &FindVec, Geoms);

		if(pText)
			return pText;
	}
	if(pText == NULL)	pText = FindRebarText(pContinueVec);
	return pText;
}

GMText* CogReaderSlabRebarHelper::FindRebarText(GM2DVector* pEndVec)
{
	double MinDist = mp_Reader->GetSlabRebarOption()->GetLeaderDistToText();

	GMText* pFindText = NULL;
	CTypedPtrArray<CObArray, GM2DGeometry*>& TextArr = mp_Reader->GetGeometry(DG_TEXT);
	for(int iText = 0; iText < TextArr.GetSize(); iText++)
	{
		GMText* pText = (GMText*)TextArr[iText];
		if(pText->m_Tag == 1)	continue;

		double MidDist = pEndVec->Distance(pText->m_Vec);
		double OriginDist = pEndVec->Distance(pText->m_Origin);
		double CornerDist = pEndVec->Distance(pText->m_Corner);

		double TextTolerance = pText->m_dHeight * 2;

		vector<double> DistArr;
		DistArr.push_back(MidDist);	DistArr.push_back(OriginDist);	DistArr.push_back(CornerDist);
		double TextDist = GetMinValue(DistArr);

		if (TextTolerance > TextDist)
		{
			if (pFindText == NULL)
			{
				MinDist = TextDist;
				pFindText = pText;
				continue;
			}
		}

		if(TextDist < MinDist)
		{
			MinDist = TextDist;
			pFindText = pText;
		}
	}

	return pFindText;
}

vector<GM2DCircle*> CogReaderSlabRebarHelper::FindCirclesOnRebarLine(GM2DLineVector* pRebarLine, CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms)
{
	vector<GM2DCircle*> Circles;
	int NumGeom = Geoms.GetSize();
	for(int iGeom = 0; iGeom < NumGeom; iGeom++)
	{
		GM2DGeometry* pGeom = Geoms[iGeom];
		if(pGeom->m_ObjType != GT_CIRCLE)	continue;
		if(pGeom->m_Tag != 0)	continue;

		GM2DCircle* pCircle = (GM2DCircle*)pGeom;

		GM2DVector CrossVec1, CrossVec2;
		long rtnVal = pCircle->Intersect(*pRebarLine, CrossVec1, CrossVec2);
		if(rtnVal	 == 0)	continue;

		pCircle->m_Tag = 1;
		Circles.push_back(pCircle);
	}
	return Circles;
}

GMText* CogReaderSlabRebarHelper::FindRebarTextStartLine(GM2DLineVector* pRebarLine, CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms)
{
	double tolerance = mp_Reader->GetSlabRebarOption()->GetLeaderDist();
	GM2DCircle* pMaxCircle = GetMaxCircleOnRebarLine();
	if(pMaxCircle)
		tolerance = pMaxCircle->GetRadius();
	int NumGeom = Geoms.GetSize();

	GMText* pText = NULL;
	for(int iGeom = 0; iGeom < NumGeom; iGeom++)
	{
		GM2DGeometry* pGeom = Geoms[iGeom];
		if(pGeom->m_ObjType != GT_LINE)	continue;
		if(pGeom->m_Tag == 1)	continue;

		GM2DLineVector* pTieLine = (GM2DLineVector*)pGeom;
		GM2DVector ContinusVec;
		if(pRebarLine->IncludeVector4DTol(pTieLine->m_P0, tolerance))
			ContinusVec = pTieLine->m_P1;
		else if(pRebarLine->IncludeVector4DTol(pTieLine->m_P1, tolerance))
			ContinusVec = pTieLine->m_P0;
		else 
			continue;

		pText = FindRebarTextEndTieLine(pTieLine, &ContinusVec, Geoms);
		if(pText == NULL)	continue;
		if(!IsRebarText(pText->m_strText))	continue;

		return pText;
	}
	return NULL;
}

GM2DLineVector* CogReaderSlabRebarHelper::FindTieLineOnRebarLine(GM2DLineVector* pRebarLine, CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms, bool& IsNextFindP1)
{
	return NULL;
}

GMText* CogReaderSlabRebarHelper::FindRebarTextTieLine(GM2DLineVector* pTieLine, bool& IsNextFindP1)
{
	return NULL;
}

bool CogReaderSlabRebarHelper::IsXDirection(GM2DLineVector* pRebarLine)
{
	double degree = pRebarLine->GetDegree();
	if (degree < 45|| degree>315 || (135<degree&&degree<225))
		return true;
	return false;
}

void CogReaderSlabRebarHelper::MakeSlabRebar(GM2DPolyline* pBoundary, GM2DLineVector* pRebarLine, CString rebarText)
{
	bool xDir = IsXDirection(pRebarLine);

	double Degree = pRebarLine->GetDegree()+90;	// 직각으로 만나는 Infoline구하기
	GM2DLineVector InfoLine = pBoundary->GetLineVectorOnXY(Degree, pRebarLine->MassCenter());

	MSSlabRebarInfo::RebarType rbType;
	long slabRebarType = FindSlabRebarType(pRebarLine, rebarText);
	if (slabRebarType == 1)
	{
		rbType = MSSlabRebarInfo::TopY;
		if (xDir)	rbType = MSSlabRebarInfo::TopX;
		MakeSlabRebar(pBoundary, pRebarLine, &InfoLine, rebarText, rbType);
	}
	else if (slabRebarType == 0)
	{
		rbType = MSSlabRebarInfo::BottomY;
		if (xDir)	rbType = MSSlabRebarInfo::BottomX;
		MakeSlabRebar(pBoundary, pRebarLine, &InfoLine, rebarText, rbType);
	}

// 	int indexT = rebarText.Find(_T("T"));
// 	int indexB = rebarText.Find(_T("B"));
// 	MSSlabRebarInfo::RebarType rbType;
// 
// 	if(indexT >= 0) 
// 	{
// 		rbType = MSSlabRebarInfo::TopY;
// 		if (xDir)	rbType = MSSlabRebarInfo::TopX;
// 		MakeSlabRebar(pBoundary, pRebarLine, &InfoLine, rebarText, rbType);
// 	}
// 	if(indexB >= 0) 
// 	{
// 		rbType = MSSlabRebarInfo::BottomY;
// 		if (xDir)	rbType = MSSlabRebarInfo::BottomX;
// 		MakeSlabRebar(pBoundary, pRebarLine, &InfoLine, rebarText, rbType);
// 	}
// 
// 	if(indexT == -1 && indexB == -1)
// 	{
// 		rbType = MSSlabRebarInfo::TopY;
// 		if (xDir)	rbType = MSSlabRebarInfo::TopX;
// 		MakeSlabRebar(pBoundary, pRebarLine, &InfoLine, rebarText, rbType);
// 	}
}
void CogReaderSlabRebarHelper::MakeSlabRebar(GM2DLineVector* pRebarLine, GM2DLineVector* pInfoLine, CString rebarText)
{
	bool xDir = IsXDirection(pRebarLine);

	MSSlabRebarInfo::RebarType rbType;
	long slabRebarType = FindSlabRebarType(pRebarLine, rebarText);
	if (slabRebarType == 1)
	{
		rbType = MSSlabRebarInfo::TopY;
		if (xDir)	rbType = MSSlabRebarInfo::TopX;
		MakeSlabRebar(pRebarLine, pInfoLine, rebarText, rbType);
	}
	else if (slabRebarType == 0)
	{
		rbType = MSSlabRebarInfo::BottomY;
		if (xDir)	rbType = MSSlabRebarInfo::BottomX;
		MakeSlabRebar(pRebarLine, pInfoLine, rebarText, rbType);
	}

// 	int indexT = -1, indexB = -1;
// 	FindTextIndex(rebarText, indexT, indexB);
// 
// 	MSSlabRebarInfo::RebarType rbType;
// 
// 	if(indexT >= 0) 
// 	{
// 		rbType = MSSlabRebarInfo::TopY;
// 		if (xDir)	rbType = MSSlabRebarInfo::TopX;
// 		MakeSlabRebar(pRebarLine, pInfoLine, rebarText, rbType);
// 
// 		if(m_pPairRebarLine && indexB == -1)
// 		{
// 			rbType = MSSlabRebarInfo::BottomY;
// 			if (xDir)	rbType = MSSlabRebarInfo::BottomX;
// 			MakeSlabRebar(m_pPairRebarLine, pInfoLine, rebarText, rbType);
// 		}
// 	}
// 	if(indexB >= 0) 
// 	{
// 		rbType = MSSlabRebarInfo::BottomY;
// 		if (xDir)	rbType = MSSlabRebarInfo::BottomX;
// 		MakeSlabRebar(pRebarLine, pInfoLine, rebarText, rbType);
// 
// 		if(m_pPairRebarLine && indexT == -1)
// 		{
// 			rbType = MSSlabRebarInfo::TopY;
// 			if (xDir)	rbType = MSSlabRebarInfo::TopX;
// 			MakeSlabRebar(m_pPairRebarLine, pInfoLine, rebarText, rbType);
// 		}
// 	}
// 
// 	if(indexT == -1 && indexB == -1)
// 	{
// 		rbType = MSSlabRebarInfo::TopY;
// 		if (xDir)	rbType = MSSlabRebarInfo::TopX;
// 		MakeSlabRebar(pRebarLine, pInfoLine, rebarText, rbType);
// 
// 		if(m_pPairRebarLine)
// 		{
// 			rbType = MSSlabRebarInfo::BottomY;
// 			if (xDir)	rbType = MSSlabRebarInfo::BottomX;
// 			MakeSlabRebar(m_pPairRebarLine, pInfoLine, rebarText, rbType);
// 		}
// 	}
}

void CogReaderSlabRebarHelper::MakeSlabRebar(GM2DLineVector* pRebarLine, GM2DLineVector* pInfoLine, CString rebarText, MSSlabRebarInfo::RebarType rbType)
{
	GM2DLineVector NewInfoLine;
	NewInfoLine.m_P0 = pInfoLine->m_P0;
	NewInfoLine.m_P1 = pInfoLine->m_P1;

	if(m_IsExtendP0)	NewInfoLine.m_P0 = m_ExtendP0;
	if(m_IsExtendP1)	NewInfoLine.m_P1 = m_ExtendP1;

	MSSlabRebar* pSlabRebar = new MSSlabRebar();
	pSlabRebar->SetFloor(mp_Floor->GetLowerFloor());
	pSlabRebar->SetOffset(mp_Reader->m_BotOffset);
	pSlabRebar->SetPolylineType(false);
	pSlabRebar->m_RebarLine.m_P0 = pRebarLine->m_P0;
	pSlabRebar->m_RebarLine.m_P1 = pRebarLine->m_P1;
	pSlabRebar->m_RebarZone.m_P0 = NewInfoLine.m_P0;
	pSlabRebar->m_RebarZone.m_P1 = NewInfoLine.m_P1;

	pSlabRebar->SetZoneProfile(MakeBoundary(pRebarLine, &NewInfoLine));
	pSlabRebar->SetRebarDirection(pSlabRebar->m_RebarLine.GetDegree());

	MSSlabRebarInfo* pSlabRebarInfo = MakeRebarInfo(pSlabRebar->m_RebarLine, rebarText, rbType);
	pSlabRebar->SetRebarInfo(pSlabRebarInfo);

	MSMaterial* pDefMaterial = mp_Bldg->GetMaterial(mp_Reader->m_nMaterialID);
	if (pDefMaterial == NULL)	pDefMaterial = mp_Bldg->GetDefMaterial(MSMaterial::msConcrete);
	pSlabRebar->SetMaterialID(pDefMaterial->m_ID);
	pSlabRebar->SetIsUsedStoryMaterial(mp_Reader->m_IsUsedStoryMaterial);

	pSlabRebar->CalulateRebarLineRebarZone();
	mp_Reader->AddSlabRebar(pSlabRebar);
}

void CogReaderSlabRebarHelper::MakeSlabRebar(GM2DPolyline* pBoundary, GM2DLineVector* pRebarLine, GM2DLineVector* pInfoLine, CString rebarText, MSSlabRebarInfo::RebarType rbType)
{
	GM2DLineVector NewInfoLine;
	NewInfoLine.m_P0 = pInfoLine->m_P0;
	NewInfoLine.m_P1 = pInfoLine->m_P1;
	if(m_IsExtendP0)	NewInfoLine.m_P0 = m_ExtendP0;
	if(m_IsExtendP1)	NewInfoLine.m_P1 = m_ExtendP1;

	MSSlabRebar* pSlabRebar = new MSSlabRebar();
	pSlabRebar->SetFloor(mp_Floor->GetLowerFloor());
	pSlabRebar->SetOffset(mp_Reader->m_BotOffset);
	pSlabRebar->SetPolylineType(IsPolylineType(pBoundary, pRebarLine));
	pSlabRebar->m_RebarLine.m_P0 = pRebarLine->m_P0;
	pSlabRebar->m_RebarLine.m_P1 = pRebarLine->m_P1;
	pSlabRebar->m_RebarZone.m_P0 = NewInfoLine.m_P0;
	pSlabRebar->m_RebarZone.m_P1 = NewInfoLine.m_P1;


	GM2DPolyline* pZoneProfile = new GM2DPolyline(*pBoundary);
	pSlabRebar->SetZoneProfile(pZoneProfile);
	pSlabRebar->SetRebarDirection(pSlabRebar->m_RebarLine.GetDegree());

	MSSlabRebarInfo* pSlabRebarInfo = MakeRebarInfo(pSlabRebar->m_RebarLine, rebarText, rbType);
	pSlabRebar->SetRebarInfo(pSlabRebarInfo);

	MSMaterial* pDefMaterial = mp_Bldg->GetMaterial(mp_Reader->m_nMaterialID);
	if (pDefMaterial == NULL)	pDefMaterial = mp_Bldg->GetDefMaterial(MSMaterial::msConcrete);
	pSlabRebar->SetMaterialID(pDefMaterial->m_ID);
	pSlabRebar->SetIsUsedStoryMaterial(mp_Reader->m_IsUsedStoryMaterial);

	pSlabRebar->CalulateRebarLineRebarZone();
	mp_Reader->AddSlabRebar(pSlabRebar);
}

MSSlabRebarInfo* CogReaderSlabRebarHelper::MakeRebarInfo(GM2DLineVector rebarLine, CString rebarText, MSSlabRebarInfo::RebarType rbType)
{
	long nPLIndex = rebarText.Find('(');
	if (nPLIndex != -1)
		rebarText = rebarText.Mid(0, nPLIndex);

	rebarText.Replace(_T("-"), _T(""));
	CString prefix = MSDeformedBar::GetDefaultRebarNamePrefix(2);
	int prefixIndex = rebarText.Find(prefix);
	if (prefixIndex == -1)	rebarText.Replace(_T("D"), _T("HD"));

	// Make RebarLine
	MSSlabRebarInfo* pRebarInfo = new MSSlabRebarInfo(rbType);
	pRebarInfo->SetRebarText(rebarText);

	return pRebarInfo;
}

GM2DPolyline* CogReaderSlabRebarHelper::MakeBoundary(GM2DLineVector* pRebarLine, GM2DLineVector* pInfoLine)
{
	return GM2DPolyline::MakeRectPolyLineBy2Line(*pRebarLine, *pInfoLine);
// 	GM2DVector* pVec1 = new GM2DVector(pRebarLine->m_P0);
// 	GM2DVector* pVec2 = new GM2DVector(pRebarLine->m_P1);
// 	GM2DVector* pVec3 = new GM2DVector(pInfoLine->m_P0);
// 	GM2DVector* pVec4 = new GM2DVector(pInfoLine->m_P1);
// 
// 	GM2DBound aBound;
// 	aBound.AddVector(pVec1);	aBound.AddVector(pVec2);	aBound.AddVector(pVec3);	aBound.AddVector(pVec4);
// //	aBound.RotateZ(pRebarLine->GetDegree());
// 	GM2DPolyline* pBoundary = new GM2DPolyline();
// 	GM2DVector* pBotLeft = aBound.GetBottomLeft();
// 	GM2DVector* pBotRight = aBound.GetBottomRight();
// 	GM2DVector* pTopRight = aBound.GetTopRight();
// 	GM2DVector* pTopLeft = aBound.GetTopLeft();
// 
// 	pBoundary->Add(new GM2DVector(*pBotLeft));
// 	pBoundary->Add(new GM2DVector(*pBotRight));
// 	pBoundary->Add(new GM2DVector(*pTopRight));
// 	pBoundary->Add(new GM2DVector(*pTopLeft));
// 	pBoundary->Add(new GM2DVector(*pBotLeft));
// 	for(int iBulge = 0; iBulge < 4; iBulge++)
// 		pBoundary->ma_Bulge.Add(0.0);
// 
// 	return pBoundary;
}

bool CogReaderSlabRebarHelper::IsRebarText(CString& text)
{
	if (mp_Reader->GetSlabRebarOption()->GetIsUsedMatchingText())
	{
		CString value = _T("");
		if (mp_Reader->GetSlabRebarOption()->GetMatchingText(text, value))
			text = value;
	}
	int IndexAt = text.Find('@');
	if (IndexAt == -1)	return false;

	int indexD = text.Find('D');
	if (indexD == -1)	return false;


	CString sDiaName = text.Mid(indexD+1, (IndexAt - (indexD+1)));
	if(MSDeformedBar::FindBarDiameterIndexByName(sDiaName) == -1)
		return false;

	return true;
}

void CogReaderSlabRebarHelper::SortSlabRebarInfoByArea()
{
	int NumInfo = mp_Reader->ma_SlabRebarArea.GetSize();
	for(int iInfo = 0; iInfo < NumInfo; iInfo++)
	{
		GM2DPolyline* pMyPoly = mp_Reader->ma_SlabRebarArea[iInfo];
		double myArea = pMyPoly->Area();

		for(int jInfo = iInfo+1; jInfo < NumInfo; jInfo++)
		{
			GM2DPolyline* pUrPoly = mp_Reader-> ma_SlabRebarArea[jInfo];
			double urArea = pUrPoly->Area();
			if(myArea < urArea)
			{
				GM2DPolyline* pTemp = mp_Reader->ma_SlabRebarArea[iInfo];
				mp_Reader->ma_SlabRebarArea[iInfo] = mp_Reader->ma_SlabRebarArea[jInfo];
				mp_Reader->ma_SlabRebarArea[jInfo] = pTemp;
			}
		}
	}
}

bool CogReaderSlabRebarHelper::IsIncludeRebarLine(GM2DPolyline* pBoundary, GM2DLineVector* pRebarLine)
{
	if(pBoundary->Contains(&pRebarLine->MassCenter()) >= 0)
		return false;

	if (!MatchEndPointWithBoundary(pBoundary, pRebarLine))
		return false;

	return true;
}

bool CogReaderSlabRebarHelper::MatchEndPointWithBoundary(GM2DPolyline* pBoundary, GM2DLineVector* pLine)
{
	double length = pLine->Length();
	GM2DLineVector LineCopy;
	LineCopy.CopyFrom(pLine);
	LineCopy.ExtendLineVector(true, 10 * length, false);
	LineCopy.ExtendLineVector(false, 10 * length, true);
	vector<GM2DVector> crossVecs = pBoundary->IntersectEx(&LineCopy);
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

void CogReaderSlabRebarHelper::FindTextIndex(CString rebarText, int& indexT, int& indexB)
{
	indexT = indexB = -1;

	CString TestText = rebarText;
	
	int indexparenthesis = TestText.Find('(');
	if (indexparenthesis == -1)
		return; 
	TestText = TestText.Mid(indexparenthesis + 1, TestText.GetLength() - (indexparenthesis + 1));

	if (TestText.Find(_T("BENT")) != -1)
		return;

	indexT = TestText.Find(_T("TOP"));
	indexB = TestText.Find(_T("BOT"));
	if (indexT != -1 || indexB != -1)
		return;

	indexT = TestText.Find(_T("T"));
	indexB = TestText.Find(_T("B"));
}

bool CogReaderSlabRebarHelper::IsPolylineType(GM2DPolyline* pBoundary, GM2DLineVector* pRebarLine)
{
	if(pBoundary->IsRectangle() == false)
		return true;

	int NumLine = pBoundary->GetNumLines();
	if(NumLine == 0)
		return true;
	GM2DLineVector FirstLine = pBoundary->GetLine(0);

	if(pRebarLine->IsParallel4DTol(FirstLine) || pRebarLine->IsPerpendicular(FirstLine))
		return false;

	return true;
}

long CogReaderSlabRebarHelper::FindSlabRebarType(GM2DLineVector* pRebarLine, CString rebarText)
{
	return mp_Reader->GetSlabRebarOption()->GetTopOrBottomRebar(pRebarLine, rebarText);
}
