#pragma once
#include "afx.h"

#include "GMLib/GM2DPolygon.h"

class GMRectangle;
class GMText;
class GM2DLineVector;
class GMCircle;

class MRRcSection;
class MREdge;

class MTReader;
class SRSectionTable;

class MSMainBar;
class MSStirrupBar;
class MSSpecificTieBar;

#include "StructureModel/MSRcStrip.h"

class SRDimension : public CObject
{
public:
	SRDimension(SRSectionTable* pOwner){
		mp_Owner = pOwner;
		mp_Text = NULL;
		mp_LVec = NULL;
		mp_EdgeLVec = NULL;
		m_EdgePosition = 0;
		m_NonScale = true;
		m_Radian = 0;
		m_Length = 1.0;
	};
	~SRDimension(){DeleteObjects();};
	void DeleteObjects();

	SRSectionTable* mp_Owner;
	GMText* mp_Text;
	GM2DLineVector* mp_LVec;
	GM2DLineVector* mp_EdgeLVec;
	long m_EdgePosition;  // 1 : 왼쪽, -1 : 오른쪽

	double m_Length;
	double m_Radian;
	bool m_NonScale;

	bool SetLength(CString strText);
	bool SetLVec(GM2DVector& defVec1, GM2DVector& defVec2, GM2DVector& PositionVec);
	SRDimension* Copy()
	{
		SRDimension* pNewDim = new SRDimension(mp_Owner);
		pNewDim->mp_Text = mp_Text;
		if(mp_LVec)
			pNewDim->mp_LVec = new GM2DLineVector(*mp_LVec);
		pNewDim->m_EdgePosition = m_EdgePosition;
		pNewDim->m_NonScale = m_NonScale;
		pNewDim->m_Radian = m_Radian;
		pNewDim->m_Length = m_Length;
		return pNewDim;
	};

};
class SRCircleKind : public CObject
{
public:
	SRCircleKind(GMCircle* pCircle) 
	{
		m_Radius = pCircle->m_A;
		ma_GM.Add(pCircle);
	};
	~SRCircleKind() 
	{
		ma_GM.RemoveAll();
	};

	double m_Radius;
	CTypedPtrArray<CObArray, GMCircle*> ma_GM;
};

class SRWallInfo : public CObject
{
public:
	SRWallInfo();
	~SRWallInfo();

	void Initialize();
	bool CheckData();
	bool CheckStory();
	bool IsSameData(SRWallInfo* urWallInfo);
	void AddTextFigureID(int nCellType, int nFigureID);
	vector<int> GetTextFigureIDs(int nCellType);

	bool GetIsMerged() { return m_IsMerged; };
	double GetStartLocation() { return m_StartLoc; };
	double GetEndLocation() { return m_EndLoc; };
	long GetFromStory() { return m_nFromStory; };
	long GetToStory() { return m_nToStory; };
	long GetMaterialID() { return m_nMaterialID; };
	long GetThick() { return m_nThick; };
	CString GetVBar() { return m_VBar; };
	CString GetHBar() { return m_HBar; };
	CString GetShearRatio() { return m_ShearRatio; };
	int GetRow() { return m_nRow; };

	void SetIsMerged(bool isMerged) { m_IsMerged = isMerged; };
	void SetStartLocation(double startLoc) { m_StartLoc = startLoc; };
	void SetEndLocation(double endLoc) { m_EndLoc = endLoc; };
	void SetFromStory(long story);
	void SetToStory(long story);
	void SetMaterialID(long materialID) { m_nMaterialID = materialID; };
	void SetThick(long thick) { m_nThick = thick; };
	void SetVBar(CString vBar) { m_VBar = vBar; };
	void SetHBar(CString hBar) { m_HBar = hBar; };
	void SetShearRatio(CString shearRatio) { m_ShearRatio = shearRatio; };
	void SetRow(int row) { m_nRow = row;};
protected:

	bool m_IsMerged;

	double m_StartLoc, m_EndLoc;
	long m_nFromStory, m_nToStory;
	long m_nMaterialID;
	long m_nThick;
	CString m_VBar, m_HBar, m_ShearRatio;
	int m_nRow;

	map<int/*MTReader::CellType*/, vector<int>> mm_CellTypeToFigureIDs;
};

class SRSteelInfo : public CObject
{
public:
	SRSteelInfo();
	~SRSteelInfo();

	void Initialize();
	bool CheckData();

	double m_StartLoc, m_EndLoc;
	CString m_Name;
	CString m_SectionName;
	CString m_StudBolt;
	long m_nMaterialID;
	vector<CString> ma_Name;

	map<int/*MTReader::CellType*/, vector<int>> mm_CellTypeToFigureIDs;
	void AddTextFigureID(int nCellType, int nFigureID);
	vector<int> GetTextFigureIDs(int nCellType);
};

class SRSlabInfo : public CObject
{
public:
	SRSlabInfo();
	~SRSlabInfo();

	void Initialize();
	MSRcStrip::STRIP_TYPE GetStripType(bool bShort, bool IsBent);
	CString m_Name;
	long m_nThick;
	bool isFlatSlabType;
	CString m_X1, m_X2, m_X3, m_X4, m_X5, m_X6, m_X7, m_X8, m_X9, m_X10;
	CString m_Y1, m_Y2, m_Y3, m_Y4, m_Y5, m_Y6, m_Y7, m_Y8, m_Y9, m_Y10;

	map<long, vector<GMText*>> m_CellTypeLocMap;
	map<int/*MTReader::CellType*/, vector<int>> mm_CellTypeToFigureIDs;
	void AddTextFigureID(int nCellType, int nFigureID);
	vector<int> GetTextFigureIDs(int nCellType);

	void RemoveCellType(int nCellType);
	void AnalyzeRebars(MTReader* pReader);
	void AnalyzeRebars(MTReader* pReader, int cellTypeLx, int cellTypeLy);
	CString AnalyzeRebar(vector<GMText*> textLit);
	void AnalyzeRebar(vector<GMText*> textLit, CString& result1, CString& result2);
    bool IsOverlapCellTypeLocation(MTReader* pReader, int cellTypeLx, int cellTypeLy);

	vector<GMText*> GetTextList(int nCellType);
	void AlignTextList(vector<GMText*>& textList);
	void SetSlabRebar(int cellType, CString strData);

	bool GetIsFlatSlabType();
	void SetIsFlatSlabType(bool isFlat);
};

class MSMainBarSet;
class StructureModel_CLASS SRSectionBox :
	public CObject
{
public:
	SRSectionBox(SRSectionTable* pOwner);
	~SRSectionBox(void);

	SRSectionTable* mp_Owner;
	MTReader* mp_Reader;
	void SetMTReader(MTReader* pReader){mp_Reader = pReader;};

	GM2DRectangle* mp_Rect;
	GMText* mp_Name;
	GMText* mp_Story;
	GMText* mp_Size;

	long m_nFromStory, m_nToStory;
	double m_Width, m_Depth, m_Thick;
	double m_DimensionWidth, m_DimensionDepth;
	bool m_hasDimensionSize;
	long m_nMaterialID;
	MSMainBarSet* mp_MainBarSet1, *mp_MainBarSet2;
	MSMainBar*   mp_MainBar1, *mp_MainBar2, *mp_SideBar;
	MSStirrupBar* mp_StrpBar1;		// 중앙부
	MSStirrupBar* mp_StrpBar2;		// 단부
	vector<CString> ma_StirrupBar1;
	vector<CString> ma_StirrupBar2;

	MSStirrupBar* m_SideStirrupBar;
	long m_nStrp;	// 보 스트럽갯수

	// 셀에 해당되는 실제 Text
	CString m_sWidth, m_sDepth, m_sThick;
	CString m_sTopBar;
	CString m_sBotBar;
	CString m_sStirrupBar1;
	CString m_sStirrupBar2;
	CString m_sSideBar;
	CString m_sSubMainBar;

	int m_nSpliteDistTol;

	bool m_bStandardSection;
	long m_Shape;  //	enum MSLineMemberM :: SECT_SHAPE {RECT=1, CIRCLE, POLYGON, POLYGONINPLAN};
	long m_nVBar;
	CString m_SteelSectionName;
	CString m_StudBolt;

	// Wall인식을 위한 변수 및 함수
	void DeleteWallInfoArr();
	CTypedPtrArray<CObArray, SRWallInfo*> ma_WallInfo;
	SRWallInfo* FindWallInfo(double dLoc);
	SRWallInfo* MakeWallInfo(SRWallInfo* pWallInfo, GMText* pText, long nCellType);

	SRWallInfo* MakeWallInfo(int CellType, int Row, CString strData);
	void MakeData_Excel();
	void CheckWallData();

	// SteelInfo
	void DeleteSteelInfoArr();
	CTypedPtrArray<CObArray, SRSteelInfo*> ma_SteelInfo;
	SRSteelInfo* FindSteelInfo(double dLoc);
	SRSteelInfo* MakeSteelInfo(SRSteelInfo* pSteelInfo, GMText* pText, long nCellType);
	void ReNameByFloorNum();

	// Slab인식을 위한 변수 및 함수
	SRSlabInfo* mp_SlabInfo;
	void MakeSlabInfo(GMText* pText, long nCellType);

	// 인식오류 Text색상 바꾸기에 필요한 변수, 함수
	int FindTextFigureID(GMText* pText);
	map<int/*MTReader::CellType*/, vector<int>> mm_CellTypeToFigureIDs;
	vector<int> GetTextFigureIDs(int nCellType);

	GMText* GetGMText(int nCellType);
    CString makeMainBarString(CString diaName, vector<int>& barData);

protected:

	double GetScaleFactorFromDimension();
public:
	MRRcSection* mp_Section;
	long m_VdId;
	int m_BoxNumber;
	CTypedPtrArray<CObArray, GM2DGeometry*> ma_Curve;
	CTypedPtrArray<CObArray, SRDimension*> ma_SRDim;
	CTypedPtrArray<CObArray, GM2DGeometry*> ma_OutLine;

	CTypedPtrArray<CObArray, GMCircle*> ma_Circle;
	CTypedPtrArray<CObArray, GMCircle*> ma_MainVBarCircle;
	CTypedPtrArray<CObArray, GMCircle*> ma_SubVBarCircle;
	CTypedPtrArray<CObArray, GMCircle*> ma_SideVBarCircle;
	CTypedPtrArray<CObArray, GMCircle*> ma_AuxCircle;
	CTypedPtrArray<CObArray, GMCircle*> ma_SubMainBar;
	
	GMCircle* mp_SectCircle;
	GMCircle* mp_StrpCircle;
	CTypedPtrArray<CObArray, GMCircle*> ma_StrpCircle;
	CTypedPtrArray<CObArray, GM2DGeometry*> ma_StrpLine;
	CTypedPtrArray<CObArray, GM2DGeometry*> ma_TieLine;

	CTypedPtrArray<CObArray, GM2DLineVector*> ma_GMLine;
	CTypedPtrArray<CObArray, GMText*>   ma_GMText;

	CTypedPtrArray<CObArray, GM2DLineVector*> ma_BoxLVec;
	CTypedPtrArray<CObArray, GM2DLineVector*> ma_HoriLVec;
	CTypedPtrArray<CObArray, GM2DLineVector*> ma_VertLVec;
	CTypedPtrArray<CObArray, GM2DLineVector*> ma_LVec;
	CTypedPtrArray<CObArray, GMArc*> ma_Arc;

	CTypedPtrArray<CObArray, GM2DPolyline*> ma_Polyline;		// Specific Tie bar;

	double m_dFactor;

	double m_VBarDia;

	void DeleteObjectArr();
	void MakeData();
	void MakeCirclebyArc();
	bool MakeData_Column();
	bool MakeData_Beam();
	bool MakeData_Slab();
	bool MakeData_Buttress();
	bool MakeData_RWall();

	bool IsFlatSlabType();
protected:
	bool MakeData_RcColumn();
	bool MakeData_SteelColumn();
	bool MakeData_SrcColumn();

	bool MakeData_RcBeam();
	bool MakeData_SteelBeam();
	bool MakeData_SrcBeam();

	bool CheckLayerTexts();

	void MakeSteelDataInfo_fromName();
public:
	long GetFromToStory(long& nFrom, long& nTo);
	long GetSectionType();
	double GetSectionWidth();
	double GetSectionHeight();

	void Analyze_WallInfo();
	void SortWallInfo();

	void CurveGrouping_4Wall();

	void MakeData_Wall();
	void MakeWallDataInfo_fromFloor();
	void SetWallDataByColumns();
	void SetWallInfoData(SRWallInfo* pWall, long cellType, GMText* pText);
	void SetWallDataByColumn(vector<GMText*>& TextArr, long cellType);
	void SetWallData4ShearRatio();
	bool FindLine(double infoY, double textY, CArray<double, double>& intersectYArr, int* iLine);
	void SortWallInfo_byYCoord();
	void SortTextArray_byYCoord(vector<GMText*>& textArr);
	void CalculateYArr( double xCoord, CArray<double, double>& intersectYArr );
	void AddWallInfoTextFigureIDs();
	void MergeSameWallInfo();
	vector<GMText*> ma_Fck;
	vector<GMText*> ma_Thick;
	vector<GMText*> ma_HBar;
	vector<GMText*> ma_VBar;
	vector<GMText*> ma_ShearRatio;

	double m_LinkTol;
	long m_nBeamSection;	// 보 단면의 수

	vector<MSSpecificTieBar*> ma_SpecificTieBar;
	void DeleteSpecificTieBar();
protected:
	long m_nErrorCode;
	long m_nWarningCode;
public:
	long GetErrorCode(){return m_nErrorCode;};
	long GetWarningCode(){return m_nWarningCode;};
	long MakeErrorObject(long nNO);
	long MakeErrorObject_Column(long nNO);
	long MakeErrorObject_Wall(long nNO);
	long MakeErrorObject_Beam(long nNO);
	bool SetDimensionSize(double dimWidth, double dimDepth);
	bool Contains(GM2DVector* pVec1, GM2DVector*  pVec2);
	GMCircle* FindGMCircle(GM2DVector CenPnt, double dRadius);

	BOOL Convert2Numeric(CString& strDim);
	void DeleteSRCircleKindArr(CTypedPtrArray<CObArray, SRCircleKind*>& KindArr);
	void GetSectionPolygon(GM2DPolygon& SecPGon);
	
protected:
	BOOL FindDimensions();
	long FindEdges_ColumnH();
	// Type 1:main bar(콩이 제일 많은 주근), 2:sub bar(2번째 많은 주근), 3:side bar(X표시된 주근)
	BOOL FindMainBars_ColumnH(long Type = 1); 
	BOOL FindStrpBars_ColumnH();
	BOOL FindTieBars_ColumnH();
	BOOL FindMainBars_Beam();
	BOOL FindMainBars_Buttress();
	
	long MakeDimensionFromLine();
	long MakeDimensionFromCircle();
	long MakeDimensionFromSection();
	SRDimension* MakeDimensionFromLineVector(GM2DLineVector* pLineVec);
	SRDimension* MakeDimensionFromCircle(GMCircle* pCircle);

	BOOL HasSizeCell();
	BOOL SetSectionShape();

	bool FindEdgesByDim(SRDimension* pDim);
	GM2DLineVector* MatchEdgeWithStirrupLine(MREdge* pEdge);
	long BuildSectionPolygon();
	BOOL MakeSectionPolyline();
	bool AddSectionLine(GM2DGeometry* pSectionLine);
	void RemoveCurve(GM2DGeometry* pCurve);
	void FindLinkedSectionLine(GM2DVector* pEndVec, bool IsStartPoint=false);  //added
	long CompleteGMPolygon(GM2DPolyline& ThePLine);
	BOOL FindEdgesOn0Dim();
	BOOL FindEdgesOn1Dim();
	BOOL FindEdgesOn2Dim();
	GM2DGeometry* FindAndAddEdgeOnLVec(GM2DLineVector* pLVec);
	bool FindDimEdgeOnPoint(SRDimension* pDim, GM2DVector& OnVec);
	bool FindEdgesBy3MoreDim();

	void AnalyzeTexts_Column();
	void AnalyzeTexts_Beam();
	void AnalyzeTexts_Wall();
	void AnalyzeTexts_Slab();
	void AnalyzeTexts_Buttress();
	void AnalyzeTexts_RWall();
	void AnalyzeTextsByLayer_RWall();

	BOOL AnalyzeCircles();
	long AnalyzeCrossBars();
	long ParsingSize(CString strData, CStringArray& ValueArr);
	void CurveGrouping();
	bool AnalyzeSectionText();
	bool SetEdgeVBarProperty(long EdgeNum, long NumBarByText, CString BarType, CString sDiaName);
	void CalcVBarLocOfEdges();
	void CalcVBarVecOfEdges();
	bool IsCorrectiveDirection(GM2DLineVector* lineVec);

	bool CheckTieBarLocation(long EdgeIdx, double Location, double dTol);
	int FindSpecificTieBars_Column();
	int FindSpecificTieBars_RectColumn();
	int FindSpecificTieBars_RoundColumn();

	bool FindEdgeOnRebarCircle(GM2DPolyline* Polyline, int& EdgeIndex, int& RebarIndex, bool IsRevereFind);
	bool FindRebarIndexOnCircle(GM2DPolyline* Polyline, int& RebarIndex, bool IsRevereFind);

	GM2DLineVector* GetLongLineInPolyline(GM2DPolyline* Polyline);
	GM2DRectangle* MakeStrpRect();
	bool IsContainPolyineInRect(GM2DRectangle* pRect, GM2DPolyline* polyline, double tolerance = DTOL_GM);
	void FindSupportMainBar();
	void DeleteSubMainBars();
	bool IsCircleType_SpecificTieBar(GM2DPolyline* Polyline);
	bool IsContainPolyineInCircle(GMCircle* pCircle, GM2DPolyline* polyline, double tolernace);
	bool IsExistSpecificTieBar(MSSpecificTieBar* newTieBar);
	bool IsExistMainBarByLoc(GM2DVector* loc);
	double Get6Db();
	bool IsSameStrriuppLine(GM2DLineVector* pLine);
public:
	GM2DRectangle* GetOwnerRect();

public:
	MSMainBar* mp_SubMainBar;
	double m_EdgeToStrpDist;

	// Test용 ID
	long _sectionBoxID;
public:
	void SetSectionCircle(GMCircle* pCircle);
	void SetSectionRect(GM2DPolyline* pPolyline);

	void SetMainBarShape(vector<GMCircle*> circles);
	void SetTieBarShape(vector<GM2DLineVector*> tieBars);
	bool GetIsTieBarShapeByLayer() { return m_IsTieBarShapeByLayer; };
	void SetSubMainBarShape(vector<GM2DLineVector*> tieBars);
	bool GetIsSubMainBarShapeByLayer() { return m_IsSubMainBarShapeByLayer; };
private:
	bool m_IsMainBarShapeByLayer;
	bool m_IsTieBarShapeByLayer;
	CTypedPtrArray<CObArray, GM2DGeometry*> ma_TieBarShape;
	bool m_IsSubMainBarShapeByLayer;
	CTypedPtrArray<CObArray, GM2DGeometry*> ma_SubMainBarShape;

};

