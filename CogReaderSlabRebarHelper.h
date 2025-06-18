#pragma once
#include "CogReaderHelper.h"

#include "StructureModel/MSSlabG.h"
#include "CogReader.h"
class MSFloor;
class GMText;

class StructureModel_CLASS SlabRebarHelper_LocToCount : public CObject
{
public:
	SlabRebarHelper_LocToCount()
	{
		m_Loc = 0.0;
		m_Count = 1;
	}
	~SlabRebarHelper_LocToCount(){};

	double m_Loc;
	int m_Count;
	void IncreaseCount()
	{
		m_Count++;
	}
};

class StructureModel_CLASS CogReaderSlabRebarHelper : public CogReaderHelper
{
public:
	CogReaderSlabRebarHelper(MSBaseBuilding* pBldg, CogReader* pReader, MSFloor* pFloor);
	~CogReaderSlabRebarHelper(void);

	void MakeData();
	MSSlabRebarInfo* MakeRebarInfo(GM2DLineVector rebarLine, CString rebarText, MSSlabRebarInfo::RebarType rbType);
	GM2DPolyline* MakeBoundary(GM2DLineVector* pRebarLine, GM2DLineVector* pInfoLine);
	bool IsXDirection(GM2DLineVector* pRebarLine);

protected:
	void MergeLineSegments();
	void MakeRebarInfos();
	void MakeRebarInfosByPolyline();
	void MakeRebarInfosByInfoLine();
	void MakeRebarInfo(GM2DLineVector* pRebarLine);
	GM2DLineVector* FindNearestInfoLine(GM2DLineVector* pRebarLine);
	bool FindStartTieVector(GM2DLineVector* pRebarLine, GM2DVector& startVec);

	void SortSlabRebarInfoByArea();
	bool IsIncludeRebarLine(GM2DPolyline* pBoundary, GM2DLineVector* pRebarLine);
	bool MatchEndPointWithBoundary(GM2DPolyline* pBoundary, GM2DLineVector* pLine);

	bool IsInfoLine(GM2DLineVector* pMyLine, GM2DLineVector* pUrLine);
	GM2DCircle* GetMaxCircleOnRebarLine();
	bool FindOtherInfoLine(GM2DCircle* pCircle, GM2DLineVector* pMyLine);
	void MergeInfoLine(GM2DLineVector* pFirstLine, GM2DLineVector* pSecondLine, double linkTestTolerance);
	double GetMinValue(vector<double>& ValueArr);
	double GetMaxValue(vector<double>& ValueArr);

	void FindPairRebarLine(GM2DLineVector* pRebarLine);

	void ExtendInfoLineByArrow(GM2DLineVector* pInfoLine);
	bool FindRebarText(GM2DLineVector* pRebarLine, CString& rebarText);

	vector<GM2DCircle*> FindCirclesOnRebarLine(GM2DLineVector* pRebarLine, CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms);

	GMText* FindRebarText(GM2DLineVector* pRebarLine, CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms);

	GMText* FindRebarTextStartCircle(GM2DLineVector* pRebarLine, CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms);
	GMText* FindRebarTextStartLine(GM2DLineVector* pRebarLine, CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms);

	GMText* FindRebarTextEndTieLine(GM2DLineVector* pTieLine, GM2DVector* pContinueVec, CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms);

	GMText* FindRebarText(GM2DVector* pEndVec);

	GM2DLineVector* FindTieLineOnCircle(GM2DCircle* pCircle, CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms);
	GM2DLineVector* FindTieLineOnRebarLine(GM2DLineVector* pRebarLine, CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms, bool& IsNextFindP1);
	GMText* FindRebarTextTieLine(GM2DLineVector* pTieLine, bool& IsNextFindP1);
	GMText* FindRebarTextInTolerance(GM2DLineVector* pRebarLine);

	bool IsRebarText(CString& text);
	bool IsArrowLineVector(GM2DLineVector* pInfoLine, GM2DLineVector* pLineVec, map<int, SlabRebarHelper_LocToCount*>& LocToCountMap);

	void MakeSlabRebar(GM2DLineVector* pRebarLine, GM2DLineVector* pInfoLine, CString rebarText);
	void MakeSlabRebar(GM2DLineVector* pRebarLine, GM2DLineVector* pInfoLine, CString rebarText, MSSlabRebarInfo::RebarType rbType);
	void MakeSlabRebar(GM2DPolyline* pBoundary, GM2DLineVector* pRebarLine, CString rebarText);
	void MakeSlabRebar(GM2DPolyline* pBoundary, GM2DLineVector* pRebarLine, GM2DLineVector* pInfoLine, CString rebarText, MSSlabRebarInfo::RebarType rbType);

	void ExtendInfoLineByLoc(GM2DLineVector* pInfoLine, map<int, SlabRebarHelper_LocToCount*>& LocToCountMap);

	void DeleteLocToCountMap(map<int, SlabRebarHelper_LocToCount*>& LocToCountMap);

	void ClearTempVariables();
	void FindTextIndex(CString rebarText, int& indexT, int& indexB);
	double GetDegreeTolerance(double degree);
	bool IsPolylineType(GM2DPolyline* pBoundary, GM2DLineVector* pRebarLine);
	long FindSlabRebarType(GM2DLineVector* pRebarLine, CString rebarText);

protected:
	MSFloor* mp_Floor;

	double m_RebarTextHeight;

	// 하나의 Rebar를 인식할때만 사용되는 임시변수
	GM2DLineVector* m_pPairRebarLine;
	GM2DLineVector* m_pInfoLine;
	CogReader::eSlabError m_eErrorType;
	vector<GM2DCircle*> ma_Circle;
	void SetInfoLine(GM2DLineVector* pLineVec);
	bool m_IsExtendP0, m_IsExtendP1;
	GM2DVector m_ExtendP0, m_ExtendP1;
	map<GM2DGeometry*, int> mm_FindGeom;
};

