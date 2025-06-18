#pragma once
#include "afx.h"

#include "StructureModel/MSRebarBuilding.h"
#include "StructureModel/MSMainBar.h"
#include "StructureModel/MSStirrupBar.h"

#define GROUP_COLUMN	0
#define GROUP_WALL			1
#define GROUP_BEAM			2
#define GROUP_SLAB			3
#define GROUP_RWALL		4
#define GROUP_BRACE		5
#define GROUP_BUTTRESS 6

#define TABLE_1						0
#define TABLE_2						1

#define DG_STR_TEXT			"DG_TEXT"
#define DG_OUT_LINE			"DG_OUTLINE"
#define DG_VCIRCL_BAR	"DG_V_CIRCLE_BAR"
#define DG_STRP_BAR		"DG_STRP_BAR"
#define DG_TIE_BAR				"DG_TIE_BAR"
#define DG_STR_BOX			_T("DG_BOX")
#define DG_SUBMAINBAR "DG_SUBMAINBAR"

#define DELETE_AND_REPLACE	1
#define KEEP_AND_REPLACE			2

#define STORY_ERROR						1001
#define STORY_OUT_OF_RANGE 1002
#define STORY_OVERLAP				1003
#define ERROR_THICK						1005
#define ERROR_REBAR						1009
#define ERROR_MAINBAR_NUM  1015
#define ERROR_STIRRUPBAR_NUM 1016
#define ERROR_USED_COMPOM	2000
#define ERROR_NAME_OVERLAP 2002
#define ERROR_SIZE_INFO				2003
#define TOPBAR_BOTBAR_DIFF    2004
#define LEFTBAR_RIGHTBAR_DIFF	2005
#define ERROR_INFO							2006
#define ERROR_DUPLICATED_DATA	2039
#define ERROR_IGNORE_REBARTEXT 2042
#define ERROR_SAMENAME_DIFF_DATA 2050
#define ERROR_NOT_FOUND_VERT_HORI 2051
#define ERROR_NOT_FOUND_NAME_THICK 2052
#define ERROR_STORY_REBAR_DIFF_COUNT 2053

#define WARNING_SIZE_DIFF					10001
#define WARNING_MATERIAL_INFO	10004

#define REMOVE_NAME_NONE			0
#define REMOVE_NAME_BRACKET		1

#define SLAB_X3Y3_TYPE_BENT		0
#define SLAB_X3Y3_TYPE_CUT		1

class SRSectionTable;
class SRSectionBox;
class SRSlabInfo;
class GMText;
class MSRcStrip;
class MSRcSlabM;
class SRWallInfo;
class SRDimension;

class MTError : public CObject
{
public:
	MTError()
	{
		m_GroupName = _T("");
		m_Level = 0;
		m_ErrorCode = 0;
		m_Message = _T("");
	};
	~MTError(){};

	CString m_GroupName;
	int m_Level;
	int m_ErrorCode;
	CString m_Message;
	CTypedPtrArray<CObArray, GM2DRectangle*> ma_Rect;
	int AddCopyArea(GM2DRectangle* pSource);
};

class MTOverlapArea : public CObject
{
public:
	MTOverlapArea(){};
	~MTOverlapArea(){};

	CString m_sKey;
	int m_ErrorCode;
	CTypedPtrArray<CObArray, GM2DRectangle*> ma_Area;

	int AddCopyArea(GM2DRectangle* pSource);

};

class SRSteelInfo;
class SRWallInfo;
class SRSlabInfo;
class XWallMInfo;
class MTRecognizeSection : public CObject
{
public:
	MTRecognizeSection()
	{
		mp_Table = NULL;
	};
	~MTRecognizeSection(){};

	void SetSectionTable(SRSectionTable* pTable){ mp_Table = pTable;};
	void AddCompoMs(vector<MSCompoM*>& compoMs)
	{
		int NumCompoM = compoMs.size();
		for(int i = 0; i < NumCompoM; i++)
		{
			ma_CompoM.push_back(compoMs[i]);
		}
	}

	map<SRSteelInfo*, MSCompoM*> mm_SteelInfoToCompoM;
	void AddSteelInfoToCompoMMap(map<SRSteelInfo*, MSCompoM*>& addMaps)
	{
		map<SRSteelInfo*, MSCompoM*>::iterator it;
		for(it = addMaps.begin(); it != addMaps.end(); it++)
		{
			mm_SteelInfoToCompoM.insert(make_pair(it->first, it->second));
		}
	}

	map<SRWallInfo*, MSCompoM*> mm_WallInfoToCompoM;
	void AddWallInfoToCompoMMap(map<SRWallInfo*, MSCompoM*>& addMaps)
	{
		map<SRWallInfo*, MSCompoM*>::iterator it;
		for(it = addMaps.begin(); it != addMaps.end(); it++)
		{
			mm_WallInfoToCompoM.insert(make_pair(it->first, it->second));
		}
	}

	map<SRSlabInfo*, MSCompoM*> mm_SlabInfoToCompoM;
	void AddSlabInfoToCompoMMap(map<SRSlabInfo*, MSCompoM*>& addMaps)
	{
		map<SRSlabInfo*, MSCompoM*>::iterator it;
		for(it = addMaps.begin(); it != addMaps.end(); it++)
		{
			mm_SlabInfoToCompoM.insert(make_pair(it->first, it->second));
		}
	}

	map<int, CString> mm_FigureIDToString_LH;
	void AddLHWallInfoToCompoMMap(map<int, CString>& addMaps)
	{
		map<int, CString>::iterator it;
		for (it = addMaps.begin(); it != addMaps.end(); it++)
		{
			mm_FigureIDToString_LH.insert(make_pair(it->first, it->second));
		}
	}


	SRSectionTable* mp_Table;
	vector<MSCompoM*> ma_CompoM;
};

class MTRecognizeData : public CObject
{
public:
	MTRecognizeData()
	{
	}
	~MTRecognizeData(){};

	CString m_sData;
	vector<int> m_nFigureIDs;
};
class MTRecognizeTextFilter : public CObject
{
public :
	MTRecognizeTextFilter()
	{
		Initialize();
	}
	~MTRecognizeTextFilter() {};

	enum eFilterType
	{
		Contain = 0,
		Prefix,
		Suffix
	};

protected:
	void Initialize();
	int m_CellType;
	eFilterType m_eFilterType;
	vector<CString> ma_TextList;
public:
	void SetCellType(int cellType) { m_CellType = cellType; }
	void SetFilterType(int filterType) { m_eFilterType = (eFilterType)filterType; }
	void SetTextList(vector<CString> textList) { ma_TextList = textList; }

	int AnalyizeText(CString text);
};

class MSRcLineMemberM;
class MTBWallMHelper_LH;
class MTResultElement;
class StructureModel_CLASS MTReader : public CObject
{
public:
	MTReader(void);
	~MTReader(void);

	enum eReadType {General = 0, Excel, LH_WALL};
	enum CellType{ctNone=0, ctName=1, ctStory, ctSize, ctMaterial, ctThick, ctWidth, ctDepth, ctMainBar, ctVBar, ctHBar=10, 
								ctSection = 11, ctTBar, ctBBar, ctStirrup, ctSBar1, ctSBar2, ctLoc, ctTieBar = 18, ctSpliteDistTol = 19,
								ctSlabLx1 = 20, ctSlabLx2, ctSlabLx3, ctSlabLx4, ctSlabLx5, ctSlabLx6, ctSlabLx7, ctSlabLx8, ctSlabLx9, ctSlabLx10,
								ctSlabLy1 = 30, ctSlabLy2, ctSlabLy3, ctSlabLy4, ctSlabLy5, ctSlabLy6, ctSlabLy7, ctSlabLy8, ctSlabLy9, ctSlabLy10,
								ctSubMainBar = 40,
								ctSectionName =50, ctStudBolt,
								ctVExt_MainBar = 100, ctVExt_TopBar, ctVExt_BotBar, ctVInt_MainBar, ctVInt_TopBar, ctVInt_BotBar,
								ctHExt_TopBar, ctHExt_MidBar, ctHExt_BotBar, ctHInt_TopBar, ctHInt_MidBar, ctHInt_BotBar,
								ctShear_TopBar, ctShear_BotBar, ctShear_StartBar, ctShear_EndBar, ctShearRatio,

								ctFromStory = 300, ctToStory,
								ctMainBarShape = 310, ctTieBarShape, ctSubMainBarShape,
								ctSlabFlatType = 1000,
								ctFlatLx1 = 1100, ctFlatLx2,
								ctFlatLy1 = 1150, ctFlatLy2
							};

	enum eBeamSectionType { Small_Mid_Large = 0, Large_Mid_Small = 1 };	// 외단 == Smaller?확인
	enum eSizetype { Width_X_Depth = 0, Depth_X_Width = 1,};
	enum eRecognizeSizeType { Size = 0, Dimension, NONE};
	enum eRecognizeStoryType { FloorNum = 0, FloorName};
	enum eRecognizeEndHoopType { CenEndHoop = 0, EndCenHoop};
// 	XUtilVectorView* m_pView;
	MSBaseBuilding* mp_Bldg;
	void SetBuilding(MSBaseBuilding* pBldg){mp_Bldg=pBldg;};
// 	void SetView(XUtilVectorView* pView){m_pView=pView;};

	CTypedPtrArray<CObArray, MSCompoM*> ma_NewM;

 	CTypedPtrArray<CObArray, SRSectionTable*> ma_Table;
	CTypedPtrArray<CObArray, GMText*>   ma_GMText;
	eReadType m_nReadType;	// 0 : General(.dwg), 1: Excel, 2 : LH벽체타입

	vector<CString> ma_SubBeamFlag;
	vector<CString> ma_SubBeamExcludeFlag;
	void AddSubBeamFlag(CString sFlag);
	void AddSubBeamExcludeFlag(CString sFlag);
	bool IsSubBeam(CString name);

	void DeleteTableArr();
	void DeleteGMTextArr();
	void Init();

	bool AddTable(double cenX, double cenY, double width, double height, int VdId);
	bool AddLine(double x1, double y1, double x2, double y2, CString layerName);
	bool AddCircle(double cenX, double cenY, double radius, CString layerName);
	bool AddArc(double cenX, double cenY, double radius, double sAngle, double eAngle, CString layerName, bool IsFromPolyline);
	bool AddText(GM2DVector insertVec, CString text, double degree, double height, GM2DVector origin, GM2DVector corner, CString layerName, int figureID);
	bool AddDimension(double defPointX1, double defPointY1, double defPointX2, double defPointY2, double linePointX, double linePointY, double txtX, double txtY, CString text, CString layerName);
	bool AddPolyline(GM2DPolyline* polyline, CString layerName);
	CString GetBoxLayerName();

// 	bool MakeSectionData(vdFigure aFig, bool bBlock=false);
	SRSectionTable* FindSectionTable(GM2DVector* pVec1, GM2DVector* pVec2);
	void Prompt(SRSectionTable* pTable);
	void Prompt_Column(SRSectionTable* pTable);
	void Prompt_Wall(SRSectionTable* pTable);
	void Prompt_Beam(SRSectionTable* pTable);
	void FindStoryInfo(SRSectionBox* pSectionBox);
	GMText* GetParentSectionName(SRSectionTable* pTable);

	void MakeSectionDatas();
	void MakeWallInfoArr(CTypedPtrArray<CObArray, SRWallInfo*>& WallInfoArr);
	SRWallInfo* MakeWallInfo(GMText* pText, long nCellType);
	void MakeData_Excel();
	long FindSectionName(SRSectionTable* pTable, CStringArray& NameArr);

//	long ExecuteSectionDatas(CTypedPtrArray<CObArray, MSCompoM*>& NewCompoMArr);
	long ExecuteSectionDatas();
	void MakeColumnSection(/*CTypedPtrArray<CObArray, MSCompoM*>& NewCompoMArr, */SRSectionTable* pTable);
	void MakeBeamSection(/*CTypedPtrArray<CObArray, MSCompoM*>& NewCompoMArr, */SRSectionTable* pTable);
	void MakeWallSection(SRSectionTable* pTable);
	void MakeSlabSection(SRSectionTable* pTable);
	void MakeButtressSection(SRSectionTable* pTable);

	void MakeRcColumnSection(SRSectionTable* pTable);
	void MakeSteelColumnSection(SRSectionTable* pTable);
	void MakeSrcColumnSection(SRSectionTable* pTable);

	void MakeRcBeamSection(SRSectionTable* pTable);
	void MakeSteelBeamSection(SRSectionTable* pTable);
	void MakeSrcBeamSection(SRSectionTable* pTable);

	void MakeRWallSection(SRSectionTable* pTable);
	
	bool CheckSlabBars(bool bShortStrip, SRSlabInfo* pSlabInfo, SRSectionTable* pTable);
	bool CheckSlabBars_AllType(bool bShortStrip, SRSlabInfo* pSlabInfo, SRSectionTable* pTable);
	void SetStripInfo(bool bShortStrip, MSRcSlabM* pSlabM, SRSlabInfo* pSlabInfo);
	int MakeSlabTable();
	long GetCellType(GMText* theText, bool isFlatSlab = false);
	CString GetGroupName();

	long m_Group;  // 0:Column, 1:Wall, 2:RWall, 3:Brace, 4:Beam, 5:Slab
	long m_Type;   // 0:Table1, 1:Table2,
	BOOL m_bHorType;	// Column이 가로방향인 도면
	double m_dTol;		// Tolerance (Text의 Height)
	long m_nMaterialID;
	bool m_IsUsedStoryMaterial;
	MSMaterial::Type_MSMaterial m_MaterialType;
	bool m_IsBoxOnly;
	int m_ReplceType;	// 1 : Delete and Replace, 2 : Keep And Replace

	vector<CString> ma_NameSeperate;
	vector<CString> ma_StorySeperate;
	int m_RemoveNameOption;	// 0 : NONE, 1: ( )
	int m_SlabX3Y3Type;		// 0 : Bent, 1:Cut
	eBeamSectionType m_eBeamSectionType;	// 0 : S-M-L, 1 : L-S-M
	eSizetype m_eSizeType;
	vector<CString> ma_FlatSlabType;
	eRecognizeSizeType m_eRecognizeSizeType;
	eRecognizeStoryType m_eRecognizeStoryType;
	eRecognizeEndHoopType m_eRecognizeEndHoopType;
	void AddNameSeperate(CString sWord)		{ ma_NameSeperate.push_back(sWord);};
	void AddStorySeperate(CString sWord)		{ ma_StorySeperate.push_back(sWord);};
	void SetRemoveNameOption(int option) { m_RemoveNameOption = option;};
	void SetSlabX3Y3Type(int option) { m_SlabX3Y3Type = option; };
	void SetBeamSectionType(int option)		{ m_eBeamSectionType = (eBeamSectionType)option;};
	void SetSizeType(int option) { m_eSizeType = (eSizetype)option; };
	void SetRecognizeSizeType(int option) { m_eRecognizeSizeType = (eRecognizeSizeType)option; };
	void SetRecognizeStoryType(int option) { m_eRecognizeStoryType = (eRecognizeStoryType)option; };
	void SetRecognizeEndHoopType(int option) { m_eRecognizeEndHoopType = (eRecognizeEndHoopType)option; };
	void SetFlatSlabTypeOption(vector<CString> textList);
	bool IsFlatSlabType(CString text);
	BOOL IsSettingReader();
	int CreateSectionTable(CString Name);
	int AddSectionBoxData(int tableIndex, CellType type, int row, CString data);

//	void SetCellTypeDictionaryMap();
	long GetCellTypebyDictionary(CString strKey);
	void SetCellTypebyDictionary(CString strKey, int cellType);

	void DeleteCellTypeTextFilter();
	void setCellTypeTextFilter(int cellType, int condition, vector<CString> textList);

	CString GetStringByCellType(CellType celltype);

// 	void SetCellTypeDictionaryMap_Column();
// 	void SetCellTypeDictionaryMap_Wall();
// 	void SetCellTypeDictionaryMap_Beam();

//	CMap<long, long, GMText*, GMText*> mm_CellTypeGMText;
	map<long, vector<GMText*>> mm_CellTypeGMTexts;
	void DeleteCellTypeGMTextMap();
	CMap<long, long, long, long> mm_Material;
	void SetMaterialMap();
	long GetMaterialKind(CString strKind);
	void setDefaultMaterial(CString defaultMaterialName);
	bool IsPlainConcreteReinforceBar();

	BOOL CheckMainBars(CString strBar);
	BOOL CheckStrpBars(CString strBar);
	BOOL CheckShearAddBar(CString strBar, long nShearAddLength);
	BOOL CheckBars(CString strBar);
	long GetBarNumArrbyString(CArray<long, long>& BarNumArr, CString strData);
	BOOL CheckBeamMainBars(CString strData);

	CString GetDia2MainBar(CString strBar);
	CString GetDia2StrpBar(CString strBar);
	BOOL CheckDia(double dDia);
	BOOL GetNumBars(CString strBar);
	BOOL CheckMainBarDia(CString strTopBar, CString strLeftBar);
	BOOL CheckStrpBarDia(CString strTopBar, CString strCenBar);
	BOOL CheckTieBar(double dLength, long NumMainBars, long NumTieBars);

	bool Convert_MSMainBar(CString strBar, MSMainBar& aMainBar);
	bool Convert_MSStirrupBar(CString strBar, MSStirrupBar& aStirrupBar);
	BOOL Convert_MSStirrupBar(CString strBar, MSStirrupBar& aStirrupBar, long& NumBars);

	CArray<long, long> ma_CreateMID;
	CArray<long, long> ma_ReplaceMID;
	CStringArray ma_DeletedName;
//	CStringArray ma_ErrorInfo;
	CTypedPtrArray<CObArray, MTError*> ma_Error;
	void DeleteMTErrorArr();

	int CreateErrorObject(CString GroupName, int Level, int ErrorCode, CString Message, CTypedPtrArray<CObArray, GM2DRectangle*>& Areas);
	int CreateErrorObject(CString GroupName, int Level, int ErrorCode, CString Message, GM2DRectangle* pArea);
	void Parsing_strStory(CString strStory, long& nFrom, long& nTo);
protected:
	void Parsing_StroyByFloorNum(CString strStory, long& nFrom, long& nTo);
	void Parsing_StoryByFloorName(CString strStory, long& nFrom, long& nTo);
	long FindFloorNumByName(CString name);
public:
	bool IsTopStory(CString strStory);
	bool IsBotStory(CString strStory);
	bool IsUnderStory(CString strStory);
	bool IsAboveStory(CString strStory);

	bool CheckOverlapStory();
	bool CheckOverlapName();
	bool CheckOverlapStory_Wall();
	bool CheckWallThick();
	bool CheckOverlapName_SteelBeam();
	bool CheckOverlapStory_SteelColumn();

	void DeleteOverlapAreas();
	map<CString, MTOverlapArea*> mm_Overlap;
	MTOverlapArea* MakeOverlapArea(CString sKey, int errorCode, CTypedPtrArray<CObArray, GM2DRectangle*>& Areas);
	
	map<MTResultElement*, GM2DRectangle*> mm_RecognizeBox;
	void DeleteRecognizeBoxs();
	GM2DRectangle* MakeRecognizeBox(MTResultElement* firstResult, GM2DRectangle* pBoxRect);
	vector<GM2DRectangle*> ma_DisplaySectionRect;
	void AddDisplaySectionRect(GM2DRectangle* pRect) { ma_DisplaySectionRect.push_back(pRect); };
	void DeleteDisplaySectionRects();

	// 인식오류 Text색상 바꾸기에 필요한 변수, 함수
	map<GMText*, int> mm_TextToFigureID;
	int FindTextFigureID(GMText* pText);
	vector<int> ma_ErrorFigureID;
	void AddErrorFigureID(int figureID);
	void AddErrorFigureIDs(vector<int> figureIDs);
	void CreateSlabErrorObject(SRSectionTable* pTable, SRSlabInfo* pSlabInfo, CString errorBar, int cellType);

	vector<GM2DPolyline*> ma_Polyline;
	void DeletePolylineArr();
	int ParsingTextData(CString text, vector<CString>& dataArr, CString parser);

	void AddErrorName(CString Name);
	vector<CString> ma_ErrorName;

    vector<MSCompoM*> ma_RegisterM;
    void AddRegisterM(MSCompoM* pCompoM);

	bool m_IsMakeBuiltUp;

	void DeleteRecognizeSectionArr();

	MTRecognizeSection* MakeRecognizeSection(GM2DRectangle* pRect, SRSectionTable* pTable, vector<MSCompoM*>& CompoMs);
	map<GM2DRectangle*, MTRecognizeSection*> mm_RectToSection;

	bool MakeDisplayCompoMDatas(GM2DRectangle* pRect, vector<MTRecognizeData*>& DataArr);

	bool MakeDisplayColumnMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);
	bool MakeDisplayRcColumnMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);
	bool MakeDisplaySteelColumnMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);
	bool MakeDisplaySrcColumnMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);

	bool MakeDisplayBeamMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);
	bool MakeDisplayRcBeamMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);
	bool MakeDisplaySteelBeamMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);
	bool MakeDisplaySrcBeamMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);

	bool MakeDisplayWallMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);
	bool MakeDisplaySlabMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);
	bool MakeDisplayButtressMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);
	bool MakeDisplayRWallMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);

	void AddTextByLayerName(SRSectionTable* pTable, CString layerName, GMText* pText);
	void AddDimensionByLayerName(SRSectionTable* pTable, CString layerName, SRDimension* pDimension);

	void SetLayerNameToCellTypesMap(CString layerName, int cellType);
	vector<int> GetCellTypesByLayerName(CString layerName);
	bool IsVerticalMember();

	static long FindNumberData(int startIndex, CString sData);
	static int FindFirstNumberIndex(CString sData);
	static bool CorrectiveMainBarSet(CString& sData);
	static bool CorrectiveStirrupBar(CString& sData);
	static bool CorrectiveMainBar(CString& sData);
	static bool RemoveParenthesesInString(CString& sData);
	static void RemoveParenthesesInString(int startIndex, CString& sData);
	//TestMsg
	void TestMsg();

	bool MakeSideBars(SRSectionBox* pSectionBox, MSMainBar* pBoxBar, MSMainBar* pSideBar, CString name);
	bool MakeSideBars(SRSectionBox* pSectionBox, MSStirrupBar* pBoxBar, MSStirrupBar* pSideBar, CString name);

	// 벽체인식 LH
	void AddTable_LH(GM2DVector cenVec, double width, double height, int figureID);
	void AddText_LH(GM2DVector insertVec, CString text, GM2DVector originVec, GM2DVector cornerVec, int figureID);
	
	void MakeWallM_LH();
	map<CString, vector<GM2DRectangle*>> CheckWallData_LH(vector<MTBWallMHelper_LH*>& helperList);
	void MakeBWallMData_LH(vector<MTBWallMHelper_LH*>& helperList, map<CString, vector<GM2DRectangle*>>& errorNameMap);

	CString m_VMark_LH, m_HMark_LH;
	int m_DefaultWallThick_LH;
	int GetDefaultWallThick_LH() { return m_DefaultWallThick_LH; };
	void SetDefaultWallThick_LH(int thick) { m_DefaultWallThick_LH = thick; };
	void SortSRSectionTable();
private:
	map<CString, vector<int>> mm_LayerNameToCellTypes;

#pragma region BH-5356 단면인식 개선 - 레이이 기반

public:
	void InitailizeLayerToCellTypeDictionary();
	MSCompoM::Type_MSCompoM GetCompoMType();
	vector<MTResultElement*> GetResultMList();
protected:
	bool AddDictionaryTextToLayer(GMText* text, CString layer);
	CellType GetCellTypeByLayer(CString layerName);
	CellType GetCellTypeByLayer(GMText* Text);
	CellType GetCellTypeByLocation(GMText* theText, bool isFlatSlab);
public:
	CellType GetCellTypeByTextFilter(GMText* theText);
public:
        GMText* GetCellTypeGMText(int cellType);
private:

	map<GMText*, CString> mm_TextToLayer;
	vector<MTResultElement*> ma_ResultM;
	void DeleteResultMArr();
#pragma endregion

	map<CString, long> mm_CellTypeDictionary;
	CMap<double, double, long, long>	m_LocCellTypeMap;
	vector<MTRecognizeTextFilter*> ma_TextFilter;

#pragma region FlatSlab용
	map<CString, long> mm_CellTypeDictionaryFlatSlab;
	CMap<double, double, long, long>	m_LocCellTypeMapFlatSlab;

public:
	long GetCellTypebyDictionaryFlatSlab(CString strKey);
	void SetCellTypebyDictionaryFlatSlab(CString strKey, int cellType);

	bool AddTextBySlab(GM2DVector insertVec, CString text, double degree, double height, GM2DVector origin, GM2DVector corner, CString layerName, int figureID);
	GMText* MakeGMText(GM2DVector insertVec, CString text, double height, GM2DVector origin, GM2DVector corner);
	void AddCellTypeToGMText(int nCellType, GMText* pText);

#pragma endregion

};

