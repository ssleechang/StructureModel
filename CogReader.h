
#pragma once
#include "afx.h"

#pragma once

#include "GMLib/GM2DVector.h"
#include "GMLib\GMLine.h"
#include "GMLib\GM2DGeometry.h"
#include "GMLib\GM2DPolyline.h"

#include "StructureModel\XMSRowM.h"
#include "StructureModel\MSFloor.h"

#define E_CODE_PAIR_NONE				9999
#define E_CODE_MIN_LENGTH			10000
#define E_CODE_MIN_THICK				10001
#define E_CODE_MAX_THICK				10002
#define E_CODE_DIFF_REINFORCEM	10003
#define E_CODE_POLYLINE_NOT_CLOSED 10004

#define W_CODE_SIZE_DIFF				11000
#define W_CODE_NAME_NONE		11001
#define W_CODE_SECTION_NONE	11002
#define W_CODE_REPEAT_NAME    11003

#define WARNING_MSIZE_DIFF					_T("단면과 크기 정보가 다릅니다.")
#define WARNING_NAME_NONE			_T("부재명을 찾을 수 없습니다.")
#define WARNING_SECTION_NONE	_T("해당 부재명을 갖는 단면정보가 없습니다.")
#define WARNING_REPEAT_NAME	_T("해당 부재명이 여러번 사용 되었습니다.")

#define ERROR_PAIR_NONE					_T("해당 엔터티의 페어정보가 없어서 부재 생성을 하지 않습니다.")
#define ERROR_MIN_LENGTH				_T("최소 길이 보다 작아서 부재 생성을 하지 않습니다.")
#define ERROR_MIN_THICK					_T("최소 두께보다 작아서 부재 생성을 하지 않습니다.")
#define ERROR_MAX_THICK					_T("최대 두께보다 커서 부재 생성을 하지 않습니다.")
#define ERROR_DIFF_REINFORCEM		_T("오프닝 보강근 이름이 상이 하여 부재 생성을 하지 않습니다.")

#define DG_STR_REBARZONE		_T("DG_REBARZONE")
#define DG_STR_REBARLINE		_T("DG_REBARLINE")

#define BOX_LAYER_COLUMN						_T("BOX_COLUMN")
#define BOX_LAYER_WALL							_T("BOX_WALL")
#define BOX_LAYER_RWALL							_T("BOX_RWALL")
#define BOX_LAYER_LOWWALL					_T("BOX_LOWWALL")
#define BOX_LAYER_SLAB								_T("BOX_SLAB")
#define BOX_LAYER_BEAM							_T("BOX_BEAM")
#define BOX_LAYER_PIT									_T("BOX_PIT")
#define BOX_LAYER_ISOFOOT						_T("BOX_ISOFOOT")
#define BOX_LAYER_WALLFOOT					_T("BOX_WALLFOOT")


enum DGType
{
	DG_BEAM    =  0,
	DG_COLUMN  =  1,
	DG_WALL    =  2,
	DG_RWALL	 =  3,
	DG_SLAB    =  4,
	DG_ISOFOOT =  5,
	DG_TAG     =  6,
	DG_TEXT    =  7,
	DG_REBARZONE =  8,
	DG_REBARLINE =  9,
	DG_GRIDLINE = 10,
	DG_GRIDPOINT = 11,
	DG_WFOOT = 12,
	DG_WALL_ARC = 13,
	DG_RWALL_ARC = 14,
	DG_BEAM_ARC = 15,
	DG_WFOOT_ARC = 16,
	DG_MATSLAB = 17,
	DG_MATZONE_TOPLX = 18,
	DG_MATZONE_TOPLY = 19,
	DG_MATZONE_BOTLX = 20,
	DG_MATZONE_BOTLY = 21,
	DG_SLABNAME = 22,
	DG_BOX	= 23,
	DG_RAMP_SLAB = 24,
	DG_RAMP_CROSS_BEAM	 = 25,
	DG_RAMP_CROSS_BEAM_ARC = 26,
	DG_RAMP_IN_BEAM = 27,
	DG_RAMP_IN_BEAM_ARC	= 28,
	DG_RAMP_OUT_BEAM	 = 29,	
	DG_RAMP_OUT_BEAM_ARC = 30,
	DG_OPENING = 31, 
	DG_WALL_ENDBARS = 32,
	DG_INFOLINE = 33,
	DG_LOWWALL = 34,
	DG_LOWWALL_ARC = 35,
	DG_PIT = 36,
	DG_ENUM_END = DG_PIT +1,
};


class GMQuadTree;
class GMText;
class GMCircle;
class XErrorContainer;
class MSBaseBuilding;
class MSJoint;
class MSColumnG;
class MSIsoFootG;
class MSCompoM;
class MSLineMemberG;
class MSLinkMemberG;

class DG2DObject;
class DG2DLine;
class DG2DArc;
class DGSlabEndLine;
class DGJoint;

class CRSBuilderDoc;
class MSWallEndRebarLine;
class XUtilVectorView;
class NXBoxData;
class SlabRebarOption;

class MSGridEntity : public CObject
{
public:
	MSGridEntity(void);
	~MSGridEntity(void);

	GM2DLineVector* mp_LVec;
	CTypedPtrArray<CObArray, DG2DLine*> ma_MyLine;
	CTypedPtrArray<CObArray, DG2DLine*> ma_UrLine;
};

class MSGridTextInfo : public CObject
{
public:
	MSGridTextInfo();
	~MSGridTextInfo();

	GMText* m_pText;
	GM2DCircle* m_pTextCircle;
	GM2DLineVector* m_pGuideLine;
	GM2DLineVector* m_pConnectedGuideLine;
};

class MSWallEndRebarInfo : public CObject
{
public:
	MSWallEndRebarInfo();
	~MSWallEndRebarInfo();

	MSWallEndRebarLine* mp_StartEndRebarLine;
	MSWallEndRebarLine* mp_EndEndRebarLine;
};

class MSDimensionInfo : public CObject
{
public:
	MSDimensionInfo(){};
	~MSDimensionInfo(){};

	bool isSameMSDimensionInfo(MSDimensionInfo* pDim)
	{
		if (m_DefPoint1.SameVector4DTOL(pDim->m_DefPoint1, DTOL_GM) == FALSE)
			return false;
		if (m_DefPoint2.SameVector4DTOL(pDim->m_DefPoint2, DTOL_GM) == FALSE)
			return false;
		if (m_LinePosition.SameVector4DTOL(pDim->m_LinePosition, DTOL_GM) == FALSE)
			return false;
		return true;
	}

	bool MergeDimensionInfo(MSDimensionInfo* pDim, double DegreeTol, double DistTol, double linkTestLength)
	{
		GM2DLineVector MyLine(m_DefPoint1, m_DefPoint2);
		GM2DLineVector URLine(pDim->m_DefPoint1, pDim->m_DefPoint2);
		double dDistTol = 1.0;
		BOOL bRval = MyLine.MergeWhenColinear(&URLine, DegreeTol, DistTol, linkTestLength);
		if (bRval == TRUE)
		{
			m_DefPoint1 = MyLine.m_P0;
			m_DefPoint2 = MyLine.m_P1;
			return true;
		}
		return false;
	}

	CString m_Text;
	GM2DVector m_DefPoint1, m_DefPoint2, m_LinePosition;
	long m_FigureID;
};

class MSSlabRebarDwg : public CObject
{
public:
	MSSlabRebarDwg() {mp_LeaderLine=NULL, mp_InfoLine=NULL, m_bMerged=false;};
	~MSSlabRebarDwg();

	bool AddRebarText(GMText* pText, GM2DLineVector* pLeaderLine);
	void MergeInfoLine(GM2DLineVector* pInfoLine);
	bool GetRebarWidth(GM2DLineVector& RebarWidth);
	void AddBentLoc(double BentLoc, GM2DLineVector* pBentLine);

	vector<GMText*> ma_RebarText;
	vector<GM2DLineVector*> ma_LeaderLine;
	vector<GM2DCircle*> ma_InfoPointer;
	vector<GM2DCircle*> ma_RebarPointer;
	vector<GM2DPolyline*> ma_RebarLine;
	GM2DLineVector* mp_LeaderLine;
	GM2DLineVector* mp_InfoLine;

	vector<MSSlabRebarDwg*> ma_Merged;
	vector<GM2DLineVector*> ma_BentLine; // to delete
	vector<double> ma_BentLoc;
	bool m_bMerged;
};

class CR_SlabRebarInfo : public CObject
{
public:
	CR_SlabRebarInfo();
	~CR_SlabRebarInfo();

	vector<MSSlabRebar*> ma_SlabRebar;	// SlabRebar 객체
	GM2DPolyline* m_pBoundary;	// SlabRebar 배근 영역을 나타내는 경계선
	vector<GM2DLineVector*> ma_RebarLine;	// SlabRebar 영역에 들어온 배근기준선
	vector<GM2DLineVector*> ma_InfoLine;		// SlabRebar 영역을 표현하는 선
	vector<GMText*> ma_RebarText;	// 텍스트 정보 (배근 정보)
	vector<GM2DCircle*> ma_CircleText;	// 배근 정보와 연결된 원
	vector<GM2DCircle*> ma_CircleZone;	// 배근 영역을 나타내는 기준선과 (보통 화살표) 연결된 원
	int m_FigureID;
	vector<GM2DLineVector*> ma_LineVec;	// 배근기준선을 제외한 모든 라인
	vector<GM2DCircle*> ma_Circles;	// 배근 영역에 들어온 원 정보
	vector<GMText*> ma_Text;	// 텍스트 정보 (배근 정보)
	double diaTol;

	void Init();
	void SetBoundary(GM2DPolyline* pPoly);
	void SetFigureID(int figureID);

	int AddRebarLine(GM2DLineVector* pRebarLine);
	int AddLineVector(GM2DLineVector* pLineVec);
	int AddCircle(GM2DCircle* pCircle);
	int AddText(GMText* pText);

	void SetSlabRebarInfo(GM2DCircle* pCircle);
	bool Contains(GM2DLineVector* pLineVec);
	bool Contains(GM2DCircle* pCircle);
	bool Contains(GM2DVector* pVec);
	bool CheckIntersect(GM2DCircle* pCircle, GM2DLineVector* pLineVec);
	int MakeSlabRebars();
	MSSlabRebar* MakeSlabRebar(int rebarIndex, CString text, MSSlabRebarInfo::RebarType rbType, GM2DLineVector* pInfoLine);
	MSSlabRebarInfo* MakeRebarInfo(GM2DLineVector rebarLine, CString text, MSSlabRebarInfo::RebarType rbType);
	MSSlabRebar* GetSlabRebar(int index);
	bool IsXDirection(GM2DLineVector* pRebarLine);
	bool ValidateRebarLine(GM2DLineVector* pLine);	// RebarLine 유효성 검토
	bool MatchEndPointWithBoundary(GM2DLineVector* pLine);	// 끝점 맞추기
	void ValidateCircles(); // 배근기준선과 교차여부 체크
	void ValidateLines();	// 배근영역선과 텍스트 연결선 구분
	void ValidateTexts();	// 기타선으로 텍스트 유효성 검토
	void FilterLines();
	void FindInfoLine();
	bool FindTextAndSet(GM2DVector APoint, int iRebar);
	void SetInfoLine(GM2DLineVector* pLine);
	void SetInfoCircle(int iCircle);
	void SetTextCircle(int iCircle);
	void FindTextLine();
	GM2DLineVector* FindRebarInfoLine(int tagIndex);
	GM2DLineVector* JoinLines(vector<GM2DLineVector*> lines);
	void DeleteInfoLines();

};

class MSGridLineOption : public CObject
{
public:
	MSGridLineOption(void)
	{
		Init();
	}
	~MSGridLineOption(void){};

	void Init()
	{
		m_IsMarkerStart = false;
		m_IsMarkerEnd = false;
		m_TextType = 0;
	}

	void SetOption(bool IsMarkerStart, bool IsMarkerEnd, int TextType)
	{
		m_IsMarkerStart = IsMarkerStart;
		m_IsMarkerEnd = IsMarkerEnd;
		m_TextType = TextType;
	}

	bool GetMarkerStart()	{ return m_IsMarkerStart;};
	bool GetMarkerEnd()	{ return m_IsMarkerEnd;};
	int     GetTextType()		{ return m_TextType;};
	CString GetPrefix()			{ return m_Prefix;};
	CString GetSuffix()			{ return m_Suffix;};

protected:
	bool m_IsMarkerStart;
	bool m_IsMarkerEnd;
	int m_TextType;
	CString m_Prefix;
	CString m_Suffix;
};

#include "CogReaderSlabHelper.h"
class StructureModel_CLASS CogReader
{
public:
	CogReader(void);
	~CogReader(void);

	enum eRecognizeMode { Single = 0, Box, History, Formwork};

	void SetBuilding(MSBaseBuilding* pBldg)				{m_pBldg = pBldg;};
	void SetFloor(MSFloor* pFloor)							{mp_MSFloor=pFloor;};
	void SetRecognizeMode(int recognizeMode)  { m_eRecognizeMode = (eRecognizeMode)recognizeMode; };
	eRecognizeMode   GetRecognizeMode() { return m_eRecognizeMode; };
	MSFloor* GetDrawingHistoryFloor();

	void SetDrawingHistoryLayerName(CString layername) { m_DrawingHistoryLayerName = layername; };
	void SetDrawingHistoryDistTolerance(double tolerance)			{ m_DrawingHistoryDistTolerance = tolerance; };
	void SetDrawingHistoryDistTolerancePercent(double percnet) { m_DrawingHistoryDistTolerancePercent = percnet; };
	void SetDrawingHisotryDegreeTolerance(double tolerance) { m_DrawingHistoryDegreeTolerance = tolerance; };
	CString GetDrawingHistoryLayerName() { return m_DrawingHistoryLayerName; };
	double GetDrawingHistoryDistTolerance() { return m_DrawingHistoryDistTolerance; };
	double GetDrawingHistoryDistTolerancePercnt() { return m_DrawingHistoryDistTolerancePercent; };
	double GetDrawingHistoryDegreeTolerance() { return m_DrawingHistoryDegreeTolerance; };

	void SetMemberType(long nMemberType)	{m_nMemberType=nMemberType;};
	void SetBasePoint(double xc, double yc)			{m_BasePnt.SetXY(xc, yc);};
	void SetBotOffset(double offset)								{m_BotOffset = offset;};
	void SetTopOffset(double offset) { m_TopOffset = offset; };
	void SetSplit(bool split)								{m_bSplit = split;};
	void SetMaterialType(MSMaterial::Type_MSMaterial type)	{ m_MaterialType = type;};
	void SetDefaultDepth(double depth) { m_defaultDepth = depth; }

	bool m_IsSaveM;
	bool m_IsAddBottomSlabLevel;
	bool m_IsAllowContainedMAT;
	void SetSaveM(bool isSaveM) { m_IsSaveM = isSaveM; };
	void SetAddBottomSlabLevel(bool isAddBottomSlabLevel) { m_IsAddBottomSlabLevel = isAddBottomSlabLevel; };
	void SetAllowContainedMAT(bool isAllowContainedMAT) { m_IsAllowContainedMAT = isAllowContainedMAT; };

	bool m_IsFindMName;
	CString m_DefaultMName;
	void SetNameOptions(bool isFindMName, CString defaultMName);
	MSCompoM* GetDefaultCompoM();
	int  GetRecognizeType()										{return m_nMemberType;};
	MSMaterial::Type_MSMaterial GetMaterialType()	{ return m_MaterialType;};
	MSBaseBuilding* GetBuilding()									{ return m_pBldg;};
	MSFloor*       GetFloor()										{ return mp_MSFloor;};
	GM2DVector GetBasePoint()								{ return m_BasePnt;};
	int m_SteelBeamLineOption;	// 0 : 1 line, 1 : 2 line
	void SetSteelBeamLineOption(int steelBeamLineOption) { m_SteelBeamLineOption = steelBeamLineOption; };
	int GetSteelBeamLineOption(){ return m_SteelBeamLineOption; };
	void SetIsBrickWall(bool isBrick) { m_IsBrickWall = isBrick; };
	bool GetIsBrickWall() { return m_IsBrickWall; };
	void SetIsReverseBeam(bool isReverse) { m_IsReverseBeam = isReverse; };
	bool GetIsReverseBeam() { return m_IsReverseBeam; };
	bool AnalyzeBeamDepthText(MSCompoM* pCompoM, GMText* pGMTxt);
	void SetDocID(int docID)	{m_nDocID = docID;};
	int  GetDocID()				{return m_nDocID;};
	bool AddLine(int figureID, double x1, double y1, double x2, double y2, CString layerName, CString lineType = _T("BYLAYER"));
	bool AddCircle(int figureID, double cenX, double cenY, double radius, CString layerName);
	bool AddArc(int figureID, double cenX, double cenY, double radius, double sAngle, double eAngle, CString layerName);
	bool AddText(int figureID, double cenX, double cenY, double insertX, double insertY, CString text, double degree, double height, 
								double leftBottomX, double leftBottomY, double rightBottomX, double rightBottomY, CString layerName);
	bool AddPolyline(int figureID, GM2DPolyline* pPoly, CString layerName);
	bool AddDimension(double defPointX1, double defPointY1, double defPointX2, double defPointY2, double linePointX, double linePointY, double txtX, double txtY, CString text, long figureID);
	bool MergeDimension(MSDimensionInfo* pDimension);
	bool IsExistedCircle(GM2DCircle* pCircle, long layerNum);


	map<GM2DVector*, double> mm_SteelColumnData;
	void AddSteelColumnDataByBlock(double xc, double yc, double rotate);
	void DeleteSteelColumnDatas();

	void AlignBottomSlabLevel(MSBeamG* pBeamG);
	vector<GM2DLineVector*> ma_SteelBeamData;
	void AddSteelBeamDataByBlock(double x1, double y1, double x2, double y2);
	void DeleteSteelBeamDatas();

	vector<CR_SlabRebarInfo*> ma_SlabRebarInfo;
	void DeleteSlabRebarInfoArr();
	vector<GM2DLineVector*> ma_RebarLine;

	vector<MSDimensionInfo*> ma_Dimension;
	void DeleteDimensionArr();

	void CRFindRebarLines(CR_SlabRebarInfo* pZone); // 영역 안에 들어온 배근기준선 찾기 + 영역경계를 기준으로 유효성 검토
	void CRFindCirclesAndLines(CR_SlabRebarInfo* pZone); // 영역 안에 들어온 원과 선 찾기
	void CRFindTexts(CR_SlabRebarInfo* pZone); // 영역 안에 들어온 텍스트 찾기
	
	// 슬래브 인식옵션
	CogReaderSlabHelper* mp_SlabHelper;
	void InitializeSlabHelper();
	void SetDefaultSlabThick(double thick);
	void SetDefaultSlabOffset(double offset);
	void SetIsUsedSlabThickOption(bool isUsed);
	void SetIsUsedSlabOffsetOption(bool isUsed);
	void AddSlabThickOptionText(CString text, CogReaderTextArray::eTextType textType);
	void AddSlabOffsetOptionText(CString text, CogReaderTextArray::eTextType textType);
	bool IsFindText(GM2DPolyline* pPoly, double& outData, bool isThick);

	//
	int makeData();

	XErrorObject* AddErrorObject(vector<long>& FigureIDs, long nErrorCode, long nLevel, CString strGroup, CString strMsg);
	XErrorObject* AddErrorObject(vector<MSElement*>& Elements, long nErrorCode, long nLevel, CString strGroup, CString strMsg);
	void RemoveErrorToMSElement(MSElement* pElement);

	// GridLine
	vector<MSGridTextInfo*> m_GridTextInfo;
	int m_GridTextHeight;
	void DeleteGridTextInfos();

	void MakeGridTextInfos(CTypedPtrArray<CObArray, DG2DLine*>& DGLines);
	void MakeGridLines(CTypedPtrArray<CObArray, DG2DLine*>& DGLines);
	void MakeGridLines(vector<GM2DLineVector*>& LineArr);
	GMText* FindGridLineText(GM2DCircle* pCircle);
	GM2DLineVector* FindGuideLine(GM2DCircle* pCircle, vector<GM2DLineVector*>& LineVecArr);
	MSGridTextInfo* FindGridTextInfo(GM2DLineVector* pLineVec, GM2DVector &ConnVec, bool IsStart);
	void FindConnectedGuideLine(MSGridTextInfo* pGridInfo, vector<GM2DLineVector*>& LineVecArr);
	GM2DLineVector* FindConnectedLine(GM2DLineVector* pMyLine, vector<GM2DLineVector*>& LineVecArr);
	bool IsVerticalRecognize();

	CString MakeDefaultName(CString text);
	CString MakeDefaultName(long width, long depth = 0, long thick = 0);
private:
	MSBaseBuilding* m_pBldg;
	MSFloor* mp_MSFloor;
	MSFloor* mp_DrawingHistoryMSFloor;
	eRecognizeMode m_eRecognizeMode;	// 0 : single, 1 : Box도면 인식, 2 : DrawingHistory
	GM2DVector m_BasePnt; //기준점  
	GMQuadTree* m_pQT;
	long m_nMemberType;
	MSMaterial::Type_MSMaterial m_MaterialType;
	bool m_IsBrickWall;
	bool m_IsReverseBeam;
	double m_defaultDepth;

	CString m_DrawingHistoryLayerName;
	double m_DrawingHistoryDistTolerance;
	double m_DrawingHistoryDistTolerancePercent;
	double m_DrawingHistoryDegreeTolerance;
public:
	CString GetstrMemberType();
	long    GetMemberType();

	int m_nDocID;
	long   m_ExtractSeq;
	double m_DTOL_QT_MARGIN; 
	double m_DTOL_DIST_LINKTEST; 
	double m_DTOL_PARALLEL_DEGREE; 
	double m_DTOL_PAIR_DIST_ERRRATIO; 
	double m_MIN_WALL_LENGTH; 
	double m_MIN_WALL_THICK; 
	double m_MAX_WALL_THICK; 
	double m_MAX_WALL_ECC;
	double m_MAX_GAPPED_STEP;
	double m_MAX_GAP_LENGTH;
	double m_DTOL_DIST_COLINEAR;
	double m_DTOL_DEGREE_COLINEAR;
	double m_DTOL_NODE;
	double m_DTOL_COLUMN;
	double m_Factor_Column_Link;

	double m_MIN_BEAM_WIDTH; 
	double m_MAX_BEAM_WIDTH; 
	double m_MIN_BEAM_LENGTH;

	double m_MIN_WFOOT_WIDTH; 
	double m_MAX_WFOOT_WIDTH; 
	double m_MIN_WFOOT_LENGTH;

	static double m_ARC_ANGLE;

	double m_MIN_RWALL_LENGTH; 
	double m_MIN_RWALL_THICK; 
	double m_MAX_RWALL_THICK; 
	double m_MAX_RWALL_ECC;

	double m_MAX_WALL_SETBACK;
	bool   m_SEARCH_LINEAR_WALL_FOR_NAME;
	void SetSearchLinearWallName(bool bSearch){ m_SEARCH_LINEAR_WALL_FOR_NAME = bSearch;};
	double m_DTOL_TEXT_DIST;
	void SetTextTolerance(double dTolerance){ m_DTOL_TEXT_DIST = dTolerance;};

	CString m_Prefix, m_Suffix;
	void SetPrefixSuffix(CString prefix, CString suffix);

	static double m_DTOL_COLUMN_RESION;	// DGColumnNode를 찾을때 DGColumn의 Resion을 키울 크기.(ex) FindDGColumnNode
	static double m_DTOL_WALL_RESION;		// DGWallNode를 찾을때 DGWall의 Resion을 키울크기.     (ex)	FindDGWallNode

	long m_MaxSizeOfQTLeaf;

	double m_MAX_WEDGE_LENGTH;
	BOOL m_bMergedLineSegment;	// Line들이 붙어 있으나, 각각의 부재이름으로 되어있는 도면은 한부재로 인식하면
	// 안되고, 각각의 부재명으로 인식해야한다. 따라서 각 라인들을 머지하면 안된다.
	bool m_bBreakCircleFig;

	BOOL m_BasePointKeep;
	enum {ES_NONE =0, ES_BOX, ES_L, ES_Z, ES_T, ES_CROSST};
	int m_nGridLineOption;
	double m_dGridLineMinLength;
	double m_dGridLineExtendLength;

	double m_MinX;
	double m_MaxX;
	double m_MinY;
	double m_MaxY;
	MSGridLineOption m_GridLineOption;
	void SetGridLineOption(bool IsMarkerStart, bool IsMarkerEnd, int TextType)
	{
		m_GridLineOption.SetOption(IsMarkerStart, IsMarkerEnd, TextType);
	}

	bool m_IsTopBar_for_FloorRebar;
	bool m_IsBotBar_for_FloorRebar;
	bool m_IsBasicGrid_for_FloorRebar;
	int m_nMaterialID;
	double m_BotOffset;
	double m_TopOffset;
	bool m_bSplit;
	bool m_IsPairLine;

	void SetPairLine(bool IsPairLine) { m_IsPairLine = IsPairLine; };
	void SetFloorRebarOptions(int materialID, double offset);
	void SetFloorRebarOption(bool IsTopBar, bool IsBotBar, bool IsBasicGrid);

	void Init();
	void InitCogReaderData();
	void RemoveAllCompoG();
	void InitMessageContainer();
	
//	void SetOptions();
	void SetOptions(int IslineMerge, double MinLength, double MaxThick, double MinThick, bool IsCenterLine, double extendLength);
	CString m_DefNamePrefix;
	void SetDefaultNamePrefix(CString prefix){m_DefNamePrefix = prefix;};

	MSCompoM* CreateNewColumnM(double dB, double dH, long nShape, CString TheName);
	MSCompoM* CreateNewIsoFootM(double dB, double dH, long nShape, CString TheName);
	MSCompoM* CreateNewWallM(double dT, CString TheName, BOOL bWall);
	MSCompoM* CreateNewWallFootM(double dB, CString TheName);
	MSCompoM* CreateNewBeamM(double dB, CString TheName);
    MSCompoM* CreateNewSlabM(double dT, CString TheName);
	MSCompoM* CreateNewHandRailM(double dB, CString TheName);
	MSCompoM* FindSlabM(CString TheName);

	long GetLayerIndex(CString LayerName);
	long GetLayerIndex(long nToolID);
	void DeleteGMs();
	void BuildTree();
	void ExtractGridEntities();
	MSGridEntity* MakeAGridEntity(DG2DLine* MyLine);
	BOOL MergeGridEntity(MSGridEntity* pNewGrid);
	MSGridEntity* MakeEdgeGridEntity(DG2DLine* pLine);
	void ExtractMembers();
	void ExtractGridLines();
	void ExtractWalls();
	void ExtractRWalls();
	void ExtractColumns();
	void ExtractRectColumns();
	void ExtractRoundColumns();
	BOOL MakeColumnG(GM2DPolyline& ThePLine, DG2DLine* Geo, vector<long>& VdFigureIDArr); 
	void ExtractIsoFoots();
	void ExtractWallFoots();
	void ExtractGirders();
	void ExtractHandRails();
	void ExtractOpenings();
	void ExtractPits();
	void ExtractSlabs();

	void MakeSpliteGirders();
	BOOL MakeRcColumnG(GM2DPolyline& ThePLine, DG2DLine* Geo, vector<long>& VdFigureIDArr); 
	BOOL MakeSteelColumnG(GM2DPolyline* ThePLine, vector<long>& VdFigureIDArr); 
	BOOL MakeSrcColumnG(GM2DPolyline& ThePLine, DG2DLine* Geo, vector<long>& VdFigureIDArr); 

	void ExtractRcRoundColumns();
	void ExtractSteelRoundColumns();
	void ExtractSrcRoundColumns();

	void ExtractRcGirders();
	void ExtractSteelGirders();
	void ExtractSteelGirders_DimensionStyle();
	void ExtractSteelGirders_OneLineSytle();
	void ExtractSteelGirdersArc_OneLineSytle();
	void ExtractSrcGirders();

	void ExtractRcColumns();
	void ExtractSteelColumns();
	void ExtractSrcColumns();

	//void ExtractSlab();
	void ExtractFloorRebar();
	void ExtractFloorRebar4UAE1();
	void ExtractFloorRebar4GS1();
	void ExtractFloorRebar_General();
	void ExtractFloorRebar_MakeZone();
	CTypedPtrArray<CObArray, GM2DPolyline*> ma_SlabRebarArea;
	void DeleteSlabRebarAreaArr();

	GMText* GetNearestGMText(GM2DGeometry* AObj, long nDGType, GM2DVector TheVec);
	GMText* GetGMText_ContainCircle(GM2DCircle* AObj, long nDGType, vector<GMQuadTree*>& CrossQTs);
	GM2DPolyline* CogReader::GetSteelPolyline_ContainPolyline(GM2DPolyline* pPolyline);
	GMText* GetGMText_ContainPolyline(GM2DPolyline* pPolyline, long nDGType, vector<GMQuadTree*>& CrossQTs);
	GM2DPolyline* MakeOffsetPolyline(GM2DPolyline* pPolyline, double dDist, BOOL bExtDir);

	CMap<GM2DGeometry *, GM2DGeometry *, long , long > m_GMFigureMap;
	CTypedPtrArray<CObArray, GM2DGeometry*> m_GMs[DG_ENUM_END];

	void MergeLineSegments(CTypedPtrArray<CObArray, DG2DLine*>& DGLines, long LayerNum);
	void FindColinearLine(DG2DLine* ADGLine, long LayerNum);
	void FindColinearLineOption1(DG2DLine* ADGLine, long LayerNum);
	void FindColinearLineOption2(DG2DLine* ADGLine, long LayerNum);
	void MergeArcSegments(CTypedPtrArray<CObArray, DG2DArc*>& DGArcs, long LayerNum);
	void SortDGLines(CTypedPtrArray<CObArray, DG2DLine*>& DGLines);
	void SortDGArcs(CTypedPtrArray<CObArray, DG2DArc*>& DGArcs);
	void SortSlabRebarInfoByArea();
	void SortPairLines(CTypedPtrArray<CObArray, DG2DLine*>& DGLines);
	void ResetTag(CTypedPtrArray<CObArray, DG2DLine*>& DGLines);
	void ResetTag(CTypedPtrArray<CObArray, DG2DArc*>& DGArcs);
	void PutDGLinesIntoQT(CTypedPtrArray<CObArray, DG2DLine*>& DGLines);
	void PutDGArcsIntoQT(CTypedPtrArray<CObArray, DG2DArc*>& DGArcs);
	long FindLinkedLine(DG2DLine* MyLine, long nDGType);
	long RemoveWedgeLink(DG2DLine* MyLine, long nDGType);
	long FindLinkedColLine(DG2DLine* MyLine, long nDGType);
	BOOL IsOverlap(DG2DLine* MyLine, DG2DLine* UrLine);
	long OverlapGroup(double P0Loc, double P1Loc, double DistTol);
	BOOL SearchParallelLine(DG2DLine* MyLine, double MinThick, double MaxThick, vector<GMQuadTree*>& CrossQTs, long LayerNum);
	long FindGappedLine(DG2DLine* MyLine, long nDGType);
	long FindStaggeredLine(DG2DLine* MyLine, long nDGType);
	long CutEachMembers(DG2DLine* MyLine, long nDGType);
	long CutEachWallArcs(BOOL bWall);
	void FindColinearArc(DG2DArc* ADGArc, long LayerNum);
	// 	void YouHaveNoFeasiblePair(long& NumFeasibleLines, GMLine& MyLineEq, DG2DLine* RootLine, DG2DLine* MyLine, double MinThick, double MaxThick, long LayerNum);
	// 	void YouHaveNoFeasiblePair(long& nPairLinesUrSide, long& nPairLinesOtherSide, GMLine& MyLineEq, DG2DLine* RootLine, DG2DLine* MyLine, double MinThick, double MaxThick, long LayerNum);
// 	void CountFeasiblePairLines(double& dMinDistOfPair, vector<DG2DLine*>& PairAltArr, vector<double>& PairAltDistArr, vector<DG2DLine*>& PairAltArrOther, vector<double>& PairAltDistArrOther,
// 		                        DG2DLine* RootLine, DG2DLine* MyLine, double MinThick, double MaxThick);
	void CountFeasiblePairLines(double& dMinDistOfPair, vector<DG2DLine*>& PairAltArr, vector<double>& PairAltDistArr, DG2DLine* RootLine, DG2DLine* MyLine, double MinThick, double MaxThick);
	long PrecheckWallShape(DG2DLine* MyLine);
	long CutEachWalls(DG2DLine* MyLine, long nDGType);
	long CheckCrossPair(DG2DLine* MyLine);
 	long CheckCrossPair2(DG2DLine* MyLine);
	long CheckCornerMember(DG2DLine* MyLine);
	long CheckZCornerMember(DG2DLine* MyLine);
	BOOL CheckNeckHole(DG2DLine* MyLine);
	BOOL CheckBoxEnd(DG2DLine* MyLine);
	BOOL CheckMyLEnd(DG2DLine* MyLine, BOOL IsAtStart);
	BOOL CheckStaggeredBeam(DG2DLine* MyLine, BOOL IsAtStart);
	BOOL CheckMyZEnd(DG2DLine* MyLine, BOOL IsAtStart);
	BOOL CheckLongitudinalTEnd(DG2DLine* MyLine, BOOL IsAtStart);
	BOOL CheckKType(DG2DLine* MyLine, long nPairNum);
	BOOL CheckWingWall(DG2DLine* MyLine);
	BOOL CheckCrossTEnd(DG2DLine* MyLine, BOOL IsAtStart);
	BOOL IsCrossTEndLine(DG2DLine* MyLine);
	void ReleasePairs(DG2DLine* MyLine, DG2DLine* UrLine);
	long MergeLineQTs(DG2DLine* MyLine, DG2DLine* UrLine, vector<GMQuadTree*>& CrossQTs);
	void InsertJoint(DGJoint* pJoint, double Margin, vector<GMQuadTree*>& CrossQTs);
	void InsertJoint(DGJoint* pJoint, double dMargin, GMQuadTree* pQT);
	GMText* FindGMText(DG2DLine* MyLine, DG2DLine* UrLine, long nDGType);
	MSCompoM* FindCompoM(GM2DVector* pSVec, GM2DVector* pEVec, double halfWidth, long nDGType);
	MSCompoM* FindTextNCreateLinkCompoMByHistory(GM2DVector* pSVec, GM2DVector* pEVec, double halfWidth, long nDGType);

	GMText* FindGridLineText(GM2DLineVector* pLVec);
	long GetMSCompoMType(long nDGType);
	void FindColinearGMLine(long LayerNum, GM2DLineVector* AGmLine);
	GM2DPolyline* FindGMPolyline(long LayerNum, GM2DLineVector* AGmLine);
	// 	bool IsRebarTextBySpace(GMText* pText);
// 	bool IsRebarTextByNumber(GMText* pText);
	GMText* FindGMTextSlab(GM2DLineVector* pMyLine);
	void MergeGMLineSegments(long LayerNum);
	void MergeGMPolyline(long LayerNum, vector<GM2DPolyline*>& BentLineArr);
    void FindJoinedPolylines(long LayerNum, vector<GM2DPolyline*>& BentLineArr);
    void JoinCurves( vector<GM2DPolyline*>& JoinedPolys, CTypedPtrArray<CObArray, GM2DCurve*>& Segments );
	BOOL SearchPolygonInColumnLayer(GM2DLineVector* Geo);
	BOOL SearchLinkedLine(CTypedPtrArray<CObArray, GM2DLineVector*>& TheSelLines, GM2DPolyline& ThePLine, vector<GMQuadTree*>& EndLineCrossQTs, long LayerNum, BOOL bSearchGappedLine=true);
	BOOL CompletePolygonInWallLayer(CTypedPtrArray<CObArray, GM2DLineVector*>& TheSelLines, GM2DPolyline& ThePLine, vector<GMQuadTree*>& CrossQTs, long LayerNum);
	long CompleteGMPolygon(GM2DPolyline& ThePLine);
	BOOL SearchGappedLine(CTypedPtrArray<CObArray, GM2DLineVector*>& TheSelLines, GM2DPolyline& ThePLine, vector<GMQuadTree*>& CrossQTs, long LayerNum);
	BOOL SearchPolygonInIsoFootingLayer(GM2DLineVector* Geo);
	long CutEachWFoots(DG2DLine* MyLine);
	BOOL TestWallGirder(DG2DLine* MyLine, double MinThick, double MaxThick);
	long CutEachBeams(DG2DLine* MyLine);
	long CutEachLowWalls(DG2DLine* MyLine);

	long CutEachBeamArcs();
	long CutEachWFootArcs();
	GMText* FindGMArcText(DG2DArc* MyArc, long nDGType);

	// RH-1520 버그수정
	void MakeCircleDatas(CTypedPtrArray<CObArray, GM2DCircle*>& CircleArr);
	void ChangeGeometryToFigureIDs(GM2DGeometry* pNewGeom, GM2DGeometry* pOldGoem1, GM2DGeometry* pOldGeom2);
	// End

	// 원(CvdCircle) 객체를 GM으로 변환할 때 두개의 반원 아크로 변환할지 말지. 
	// default는 생성자에서 true로 초기화되어 두개의 반원으로 변환됨
	MSCompoM* GetCompoM( CString Name, long Type );
	// 벽체 양단에 일정 두께차 이하의 리니어한 벽체의 CompoM 있으면 리턴
	MSCompoM* GetLinearWallCompoM( DG2DLine* MyLine, DG2DLine* UrLine, long Type );


	CMap<CString, LPCTSTR, XMSRowM, XMSRowM> m_RowMMap;
	vector<MSCompoM*>	ma_CompoM;
	void MakeCompoMData();
	CTypedPtrArray<CObArray, DG2DLine*> ma_DGWallLines;
	CTypedPtrArray<CObArray, DG2DLine*> ma_DGColumnLines;
	CTypedPtrArray<CObArray, DG2DLine*> ma_DGRWallLines;
	CTypedPtrArray<CObArray, DG2DLine*> ma_DGBeamLines;
	CTypedPtrArray<CObArray, DG2DLine*> ma_DGWFootLines;
	// 	CTypedPtrArray<CObArray, DGSlabEndLine*> ma_DGSlabLine;
	CTypedPtrArray<CObArray, DG2DArc*> ma_DGWallArcs;
	CTypedPtrArray<CObArray, DG2DArc*> ma_DGRWallArcs;
	CTypedPtrArray<CObArray, DG2DArc*> ma_DGBeamArcs;
	CTypedPtrArray<CObArray, DG2DArc*> ma_DGWFootArcs;
	CTypedPtrArray<CObArray, MSGridEntity*> ma_GridEntity;

//     CMap<MSBeamG*, MSBeamG*, MSCompoM*, MSCompoM*> mm_BeamGtoCompoM;
	CTypedPtrArray<CObArray, MSCompoG*>	ma_BeamG;
	CTypedPtrArray<CObArray, MSCompoG*>	ma_WallG;
	CTypedPtrArray<CObArray, MSCompoG*>	ma_ColumnG;
	CTypedPtrArray<CObArray, MSCompoG*>	ma_IsoFootG;
	CTypedPtrArray<CObArray, MSCompoG*>	ma_WallFootG;
	CTypedPtrArray<CObArray, MSCompoG*>	ma_SlabG;
	CTypedPtrArray<CObArray, MSCompoG*>	ma_PitG;
	CTypedPtrArray<CObArray, MSCompoG*>	ma_LowWallG;
	CTypedPtrArray<CObArray, MSGridLine*> ma_GridLine;
	CTypedPtrArray<CObArray, MSSlabRebar*> ma_SlabRebar;
	CTypedPtrArray<CObArray, MSCompoM*> ma_NewM;

	CTypedPtrArray<CObArray, GM2DPolyline*> ma_SteelPolyline;
	void DeleteSteelPolylines();
	CTypedPtrArray<CObArray, GM2DPolyline*> ma_SlabPolyline;
	map<GM2DPolyline*, long> mm_SlabPolylineToFigureID;
	void DeleteSlabPolylines();

	void DeleteDGMembers();
    void DeleteSlabRebarDWG();
	void AddVdFigureID(GM2DGeometry* pGeom, vector<long>& FigureIDs);
	void AddVdFigureID(vector<long>& allFigureIDs, vector<long>& newFigureIDs);
// 	map<MSCompoG*, vector<long>> mm_CompoGtoFigureIDs;
	void SetFloorQT();
	void AddSlabRebar(MSSlabRebar* pRebar);
	MSElement::Type_MSCompoG GetMSCompoGType();
// 	void CheckOverLapReader();
// 	void CheckOverLapCompoGs(CTypedPtrArray<CObArray, MSCompoG*>& CompoGArr, MSFloor* pFloor, CString strMemberType);
	
	MSCompoM::Type_MSCompoM m_WallOpeningMType;
	bool m_IsWallOpeningAutoM;
	int m_DefaultWallOpeningMID;
	int m_FromFloorID, m_ToFloorID;
	int m_WallOpeningLevelType;
	int m_WallOpeningBottomLevel;
	void SetWallOpeningType(MSCompoM::Type_MSCompoM type)			{ m_WallOpeningMType = type;};
	void SetWallOpeningAutoM(bool isAutoM)													{ m_IsWallOpeningAutoM = isAutoM;};
	void SetDefaultWallOpeningMID(int openingMID)										{ m_DefaultWallOpeningMID = openingMID;};
	void SetWallOpengingFromToFloorID(int fromFloorID, int toFloorID)		{ m_FromFloorID = fromFloorID;	m_ToFloorID = toFloorID;};
	void SetWallOpeningLevelType(int levelType)												{ m_WallOpeningLevelType = levelType;};
	void SetWallOpeningBottomLevel(int bottomLevel)									{ m_WallOpeningBottomLevel = bottomLevel;};

	CTypedPtrArray<CObArray, GM2DGeometry*>& GetGeometry(long geomIndex);
	vector<GMQuadTree*> FindCrossQts(GM2DGeometry* pGeom);

	void AddWallOpeningG(MSWallMemberG* pWallG, MSWallOpeningG* pOpeningG);
	map<MSWallMemberG*, vector<MSWallOpeningG*>> mm_WalltoOpeningG;

	bool m_IsUsedStoryMaterial;
	void setDefaultMaterial(CString defaultMaterialName);

public:
	XErrorContainer* mp_MessageContainer;
	CString GetGroupName();
	CString GetProcessingMessage();
    void InsertSlabRebarDWG( MSSlabRebarDwg* pDWG );
    void SetSlabRebarInfo( MSSlabRebarDwg* pDWG, MSSlabRebar* pRebar, bool bMerged );
    MSSlabRebarInfo::RebarType GetFloorRebarLineType( CString text);
    GM2DPolyline* GetBoxProfile(GM2DLineVector vertical, GM2DLineVector horizon);

	// Setback 되는 벽체에서 벽체명 없는 경우 인접 부재명 가져오기 위한 함수
	MSCompoM* GetLinearWallCompoM( long Type );
	MSCompoM* FindGMTextInLinearWall( long MyLineDir, DG2DLine* MyLine, DG2DLine* UrLine, long Type );

	// GM2DGeometry의 ma_FigureID를 지우고, 여기서 관리
	map<MSElement*, vector<long>> mm_CompoGtoFigureIDs;
	map<GM2DGeometry*, vector<long>> mm_GeomToFigureIDs;
	void AddGeometryToFigureIDs(GM2DGeometry* pGeometry, long figureID);
	vector<long> FindGeometyToFigureIDs(GM2DGeometry* pGeometry);
	map<GMText*, vector<MSElement*>> mm_UsedTextToCompoGs;	// 동일한 이름이 여러번 사용되었을 경우 찾기 위한 map
	vector<long> m_RepeatedNamesToFigIds;
	void AddUsedTextByCompoG(GMText* pGMTxt, MSElement* pCompoG);
	void AddErrorObject_RepeatedName();

	// Error & CompoG map : 아직 ID가 생성되지 않아서, ErrorObject의 CompoG::ID는 0으로 설정되어 있다
	// 따라서 CogReaderCli::makeMSLibModelingCommand()후에(ID생성) ErrorObject의 CompoG::ID를 재 설정하기 위해 map으로 관리한다.
	map<XErrorObject*, vector<MSElement*>> mm_ErrorToMSElements;

	bool MakeWallEndRebar(CString name, GM2DVector sVec, GM2DVector eVec, vector<CString>& dataList, bool IsStart, int figureID);
	bool CheckStory(vector<CString>& DataList);
	bool Parsing_WallData(CString strData, CString& sStory, CString& sRebar);

	void DeleteWallEndRebarInfos();
	map<MSWallMemberG*, MSWallEndRebarInfo*> mm_WallEndRebarInfo;

	void ExtractColumnBoxs();
	void ExtractWallBoxs();
	void ExtractRWallBoxs();
	void ExtractLowWallBoxs();
	void ExtractSlabBoxs();
	void ExtractBeamBoxs();
	void ExtractPitBoxs();
	void ExtractIsoFootBoxs();
	void ExtractWallFootBoxs();

	vector<NXBoxData*> ma_BoxData;
	void DeleteBoxDatas();

protected:
	MSCompoM* mp_CreateReinforceM;
public:
	void SetCreateReinforceM(MSCompoM* pCreateM) { mp_CreateReinforceM = pCreateM; };
	MSCompoM* GetCreateReinforceM() { return mp_CreateReinforceM; };

	NXBoxData* AddColumnBox	(int version, int figureID, CString sName, GM2DVector cenVec, double width, double depth, int topLevel, int botLevel, double rotate);
	NXBoxData* AddColumnBox	(int version, int figureID, CString sName, GM2DVector cenVec, double radius, int topLevel, int botLevel);
	NXBoxData* AddBeamBox		(int version, int figureID, CString sName, GM2DVector sVec, GM2DVector eVec, double width, int level);
	NXBoxData* AddWallBox			(int version, int figureID, CString sName, GM2DVector sVec, GM2DVector eVec, double thick, int topLevel, int botLevel);
	NXBoxData* AddRWallBox		(int version, int figureID, CString sName, GM2DVector sVec, GM2DVector eVec, double thick, int topLevel, int botLevel, bool IsLeftInside);
	NXBoxData* AddLowWallBox	(int version, int figureID, CString sName, GM2DVector sVec, GM2DVector eVec, double thick, int level);
	NXBoxData* AddWallFootBox	(int version, int figureID, CString sName, GM2DVector sVec, GM2DVector eVec, double width, int level);
	NXBoxData* AddIsoFootBox		(int version, int figureID, CString sName, GM2DVector cenVec, double width, double depth, int level, double rotate);
	NXBoxData* AddPitBox				(int version, int figureID, CString sName, int pitDepth, int pitThick, bool hasWell, int wellDepth, GM2DPolyline* outLine, GM2DPolyline* innerLine, GM2DPolyline* wellLine);
	NXBoxData* AddSlabBox			(int version, int figureID, CString sName, int level, GM2DPolyline* boundary);

	MSCompoM::Type_MSCompoM GetMSCompoMType();
#pragma  region  --- 슬래브리바 인식모듈변경(2DShop처럼)
private:
	SlabRebarOption* mp_SlabRebarOption;
public:
	SlabRebarOption* GetSlabRebarOption() { return mp_SlabRebarOption; };
	void SetSlabRebarOption_LineFilter(CString rebarLayer, CString infoLayer, CString rangeLayer, int leaderDist);
	void SetSlabRebarOption_TopBottomFilter(int topBottomFilter, CString topBarLineType, CString botBarLineType,
		CString topBarText, CString botBarText, CString botBarLayer);
	void SetSlabRebarOption_TextFilter(int textDist, bool isUsedDefaultBar, CString defaultBar);
	void SetSlabRebarOption_MatchingFilter(bool isUsedMatchingText, map<CString, CString>& dicMatchingText);

	map<long, long> mm_IDtoLineType;	// LineType : 1= Top, 0=Bot

	enum eSlabError
	{
		NONE = 0,
		NOT_FIND_TEXT = 1,
		NOT_FIND_INFOLINE = 2,
		FINDTEXT_BY_DISTANCE = 3,
	};
	void MakeErrorRange(GM2DLineVector* pRebarLine, eSlabError errorType, double extendDist);
	void DeleteErrorRanges();

private:
	map<GM2DPolyline*, eSlabError> mm_SlabRebarError;
	CString m_RebarLineLayer;
	CString m_InfoLineLayer;
	CString m_BoundaryLayer;
	int m_LeaderDistTolerance;

	bool m_IsUsedLineType;
	int m_TopLineType;
	int m_BotLineType;

	bool m_IsUsedTextInfo;
	CString m_TopBarTextInfo;
	CString m_BotBarTextInfo;

	bool m_IsUsedBotRebarLineLayer;
	CString m_BotRebarLineLayer;

	int m_LeaderDistToText;
	bool m_IsUsedDefaultBar;
	CString m_DefaultBar;
	bool m_IsUsedMatchingDictionary;
	map<CString, CString> m_MatchingDictionary;
#pragma endregion 
};
