#pragma once
#include "MSAssembly.h"

#include "MSBeamG.h"
#include "MSBraceG.h"
#include "MSColumnG.h"
#include "MSContainer.h"
#include "MSGenStairG.h"
#include "MSWallMemberG.h"
#include "MSIsoFootG.h"
#include "MSLineMemberG.h"
#include "MSRampG.h"
#include "MSSlabG.h"
#include "MSWallFootG.h"

#include "GMLib/GMQuadTree.h"
#include "MSFloorQT.h"
#include "BaseRebarTemplate.h"
#include "StructureModel/FormWorkTemplate.h"

typedef struct _CompoMStr
{
public:
	_CompoMStr()
	{
		Initialize();
	};
	_CompoMStr(const _CompoMStr& aData)	{*this = aData;};

	_CompoMStr& operator = (const _CompoMStr& aData)
	{
		Initialize();
		pCompoM = aData.pCompoM;
		nCnt = aData.nCnt;
		return *this;
	};
	void Initialize()
	{
		nCnt = 0;
		pCompoM = NULL;
	};

public:
	// 0 둘다 1 top 2: bot
	long nCnt;
	MSCompoM* pCompoM;
}CompoMStr;

enum MSType
{
	MS_BEAM    =  0,
	MS_COLUMN  =  1,
	MS_WALL    =  2,
	MS_RWALL	 =  3,
	MS_SLAB    =  4,
	MS_ISOFOOT =  5,
	MS_TAG     =  6,
	MS_TEXT    =  7,
	MS_REBARINFO =  8,
	MS_REBARLINE =  9,
	MS_GRIDLINE = 10,
	MS_GRIDPOINT = 11,
	MS_WFOOT = 12,
	MS_WALL_ARC = 13,
	MS_RWALL_ARC = 14,
	MS_BEAM_ARC = 15,
	MS_WFOOT_ARC = 16,
	MS_MATSLAB = 17,
	MS_MATZONE_TOPLX = 18,
	MS_MATZONE_TOPLY = 19,
	MS_MATZONE_BOTLX = 20,
	MS_MATZONE_BOTLY = 21,
	MS_SLABNAME = 22
};
enum PlacementDirectionType
{
	PlacementDirectionType_Base = 0,
	PlacementDirectionType_Reverse,
};
class MSCompoG;
class MSConstructionZone;
class MSLevelZone;
class MSSectionView;
class XErrorContainer;
class MSMaterial;

class MSBlock;
class MSBlockInstance;

class StructureModel_CLASS MSSlabPolygon :	public CObject
{
public:
    MSSlabPolygon(void);
    ~MSSlabPolygon(void);

    // -2 : 2개 부재 양쪽 노드에서 중복, -1 : 연결안된 부재 추가, 0 : 포리곤 완성, n : n번째 부재 추가 완료
    // 	long AddSupport(MSLinkMemberG* pNewMem); 
    //long AddSupport( MSLinkMemPart* pNewMem );
    void Link2Supports(char Side);
    
    //CTypedPtrArray<CObArray, MSLinkMemPart*> ma_SptMember;
    // 	CTypedPtrArray<CObArray, MSLinkMemberG*> ma_SptMember;
    
    CArray<long, long> ma_SptDir; // 1: 0->1, -1 : 1->0
    long GetPolygon(GM2DPolyline& ThePolygon);
    //long GetInPolygon(GMPolygon& TheInPolygon);
};

class MSPile;
class StructureModel_CLASS MSFloor : 	public MSAssembly
{
protected:
	static bool ms_SplitLinkMember;
	long m_nFloorNum;
public:

    static double ms_LineHitDistTol;
	static double ms_DefLineHitDistTol;
	static double ms_CogReadLineHitDistTol;
    static double ms_ColinearDegreeTol;
    static long ms_MaxPointOfASlab;
	static double ms_MaxInnerDegreeTol;
	static long ms_TrimType;
	static bool ms_DrawHitJoint;
	static bool ms_FixLineMemVec;
	static double ms_MinLineLength;

	static map<long, MSCompoG*> UpdateConnectionCompoGMap;

	static void SetSplitLinkMember(bool bSplitLinkMember);
	static bool GetSplitLinkMember();
	static void SetLineHitDistTol(double dHisDist);
	static void SetFixLineMemVec(bool bFixLineMemVec);

	MSFloor(double dNodeTol=100., double dDegTol=1.);
	~MSFloor(void);
	void Serialize( CArchive& archive );
	void RecoverPointerAfterSerialize();
	void FixCompoGInfoAfterSerialize();
	void RecoverEachCompoMCustomProfile();

	DECLARE_SERIAL(MSFloor)
	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSFloor* pSource, bool bNewID=true, bool isClone = false);
	virtual void Init(bool bInConstructor=false);
	virtual bool CheckData();
	void CheckInValidElements();

	virtual Type_MSCompoG GetType(){return msFloor;};

	MSCompoG* GetCompoG(long CompoGID);
    MSLevelZone* GetLevelZone(long CompoGID);
	MSSectionView* GetSectionView(long CompoGID);
	MSConstructionZone* GetConstructionZone(long CompoGID);

	MSFloor* mp_Source;
	vector<MSFloor*> ma_Clone;

	map<long, MSGridLine*> mm_GridLine;
	map<long, MSSlabRebar*> mm_Rebar;
	map<long, MSPile*> mm_Pile;

// 
// 	void DeleteSlabRebarBox();
// 	void MakeSlabRebarBox();

	void DeleteRebars();

	static bool IsCloneMode();
	static bool ms_bCloneMode;

public: // for Block operation

	map<long, MSBlockInstance*> mm_BlockInstance;
	
	void Add(MSBlock* pBlock, GM2DVector& FirstPoint, GM2DVector& SecondPoint, bool bReverse);
// 	virtual void Add(MSBlockInstance* pInstance, bool bChangeOwner = false);
	virtual bool Remove(MSBlockInstance* pInstance);
	virtual bool Delete(MSBlockInstance* pInstance);

	void Add(MSSlabRebar* pRebar);
	virtual bool Remove(MSSlabRebar* pRebar);
	virtual bool Delete(MSSlabRebar* pRebar);
	MSSlabRebar* GetRebar(long ID);

public:
	virtual CString ToString();

    void InitElements();
    long CloneFrom(MSFloor* pSource, bool bReset, bool bLeaveChanged, CString* pBeamPrefix=NULL, CString* pSlabPrefix=NULL);
	bool CloneFrom(MSFloor* pSource);
	bool UnClone();
	void AddDefaultCZone(bool IsHorizontalZone);
	void AddDefaultCZone();
	

	void PreCheckAddbySpliteLinkMember(MSCompoG* pCompoG);
	void PreCheckRemoveSpliteLinkMember(MSCompoG* pCompoG);

	virtual void Add(MSCompoG* pCompoG, bool bChangeOwner = false, bool isDoit = true, bool bUpdateTrimmedBoundary = true);
	virtual bool Remove(MSCompoG* pCompoG);
	virtual bool Delete(MSCompoG* pCompoG);
	virtual bool Modify(MSCompoG* pOldCompoG, MSCompoG* pNewCompoG);
	virtual bool Modify(MSCompoG* pCompoG, bool bFirst = true);

	virtual void Add(vector<MSCompoG*> CompoGArr, bool bChangeOwner = false, bool isTrim = true);
	virtual bool Remove(vector<MSCompoG*> CompoGArr, bool isTrim = true);

	virtual void AddLinkedCompoG(MSCompoG* pCompoG);
	virtual void RemoveLinkedCompoG(MSCompoG* pCompoG);

	void CheckRebarLine();

	void CheckJointAll();
	void RegenMSModel_AllFloor();
	void CheckMSModel(MSCompoG* compoG);
	void CheckMSModel();
	void CheckMSModel_AllFloor();

    virtual void Add(MSGridLine* pGridLine);
    virtual bool Remove(MSGridLine* pGridLine);
    virtual bool Delete(MSGridLine* pGridLine);
	virtual bool IsFloor() {return true;};

	virtual void Add(MSLevelZone* pZone, bool isDoit = true);
	virtual bool Remove(MSLevelZone* pZone);
	virtual bool Modify(MSLevelZone* pZone, bool isDoit = true);
	bool ModifyLevelZoneByElement(MSLevelZone* pZone, bool bUpdateLevelZone = true);
	virtual void Add( MSLineLevelDifG* pLineLevel);
	bool CheckWhenDeleteLevelZone(int nLevelZoneID);

	virtual void Add(MSSectionView* pSectionView, bool isDoit = true);
	virtual bool Remove(MSSectionView* pSectionView);

    //virtual bool Add(MSOpening* pOpening);
	
	void ConverterOpeningG();


	MSGridLine* GetGLine(long GLineID);
	MSGridLine* GetGLine(CString name);

	virtual MSBaseBuilding* GetBuilding(){return mp_Building;};

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	virtual int GetRefFloorNum(){return 0;};
	virtual void SetRefFloorNum(int refFloorNum){};

	virtual int GetFloorNum(){return m_nFloorNum;};
	virtual void SetFloorNum(int nFloorNum){m_nFloorNum = nFloorNum;};

	// ckkim1308
public:
	
	static long LevelOverlap(MSLinkMemberG* pMyLink, MSLinkMemberG* pUrLink);
	void AddCrossMemberError(MSCompoG* pMyLink, MSCompoG* pUrMem, XErrorObject* pMsgObject, CString strGroup, CString& strMsg);
	void AddCrossMemberErrorCustom(MSCompoG* pMyLink, MSCompoG* pUrMem, XErrorObject* pMsgObject, CString strGroup, CString& strMsg);
	MSFloorQT* mp_QT;
	void BuildQT();
	void InitQT();
	long DeleteNewCompoG(MSCompoG* pNewCompoG);

	bool m_bMakeErrorObject;
	virtual bool IsBlock() { return false;};
	virtual bool ExportBlock(CString fileName){return false;};
	virtual bool ImportBlock(CString fileName){return false;};

// 	virtual bool IsProject() {return false;};
// 	virtual bool IsBuilding() {return false;};
// 	virtual bool IsSector() {return false;};
// 	virtual bool IsFloor() {return true;};

	bool IsContainSlab(MSCompoG* pCompoG, double dRatio = 100);

	virtual MSFloor* GetUpperFloor();
	virtual MSFloor* GetLowerFloor();
	//MSCompoM* FindCompoM(CString& strName);
	MSCompoM* GetCompoM(CString strName, long nType);
	bool ExistCompoM(CString& strName);

	bool IsRoof();
	
	long m_nCmdCnt;
	// 2개층 배근을 위한 정보 - by Gracias(2012.12.04)
	long m_nPlaceStory;	// 2개층(또는 더이상)되는 시작층의 FloorNum
	long GetCompoMsbyIncludeCompoG(long nMType, CMap<long, long, long, long>& MIDMap);	// 해당 평면에 존재하는 MIDs만 리턴

	
	// Outline 거푸집물량을 낼때 필요한 폴리곤 변수
	//CArray<GMPolyline, GMPolyline&> ma_Polyline;
	//void DeletePolylineArr();



	//friend class CFloorManipulator;
	// test func.
	//void ShowMembers();
	//void FindSlabSupports();
	//두개의 점으로 라인을 전달하여, Outline에 일치하는지의 여부를 체크
	//bool IsOutwardLine(GM2DVector& p1, GM2DVector& p2, double dTol = 100);//100보다 크게주면 양쪽 라인이 다 포함되는 경우가 생긴다.
	//const CArray<GM2DLineVector>& GetFloorOutLines();
	//bool IsContinuousSlabEnd(int Node1, int Node2, vector<int>& SlabIds);
	//void MakeSlabEndContinuityInfo();
	//void FindLevelDiffRelations();
	long GetMSCompoGArr(MSCompoM::Type_MSCompoM eMType, vector<MSCompoG*>& MemberArr);
	long GetMSCompoGArr(long memberType, vector<MSCompoG*>& MemberArr);
	long GetMSCompoGArr(long memberType, CTypedPtrArray<CObArray, MSCompoG*>& MemberArr);
	long GetPolylineObjectArr(vector<MSPolylineObject*>& PolyArr, Type_MSCompoG nType);

	map<long, MSConstructionZone*> mm_CZone;
	void AddCZone(MSConstructionZone* pZone);
	void RemoveCZone(MSConstructionZone* pZone);
	void RemoveCZone(long nCZoneID);
	MSConstructionZone* GetCZone(long nCZoneID);
	MSConstructionZone* GetFloorCZone(long nCZoneID);
	MSConstructionZone* GetCZoneDefault(bool IsHorizontalZone);
	void GetCZone(vector<int>& czone_id_list, vector<MSConstructionZone*>& czone_list);
	void SetNumberAndNameWhenDuplicate(MSConstructionZone* czone);
	MSConstructionZone* GetFirstCZone(bool IsHorizontalZone);
	MSConstructionZone* GetLastCZone(bool IsHorizontalZone);
	void GetAllCZone(vector<MSConstructionZone*>& czone_list, bool IsHorizontalZone, bool bWithDefault);
	
	void DeleteCZoneMaps();

	vector<int> GetOverlapCZoneIDs(MSCompoG* pCompoG);
	vector<int> GetOverlapCZoneIDs(MSCompoG* pCompoG, bool IsHorizontalZone);
	int GetMaxOverlapCZoneID(MSCompoG* pCompoG);
	int GetCenterPointCZoneID(MSCompoG* pCompoG, bool IsHorizontalZone);
	
	vector<GM2DCurve*> GetCenCurveByCZone(MSLinkMemberG* pCompoG, bool SortLongLength = false);
	vector<GM2DCurve*> GetCenCurveByCZoneByType(MSLinkMemberG* pCompoG, bool IsHorizontalZone, bool SortLongLength = false);

	vector<GM2DCurve*> GetCurveByCZone(GM2DCurve* pCurve, bool IsHorizontalZone, double dMinLen = 10);
	vector<GM2DPolyline*> GetPolyLineByCZone(GM2DPolyline* pPoly, bool IsHorizontalZone);
 	bool SpliteElementByCZone(MSElement* pOrgElement, vector<MSElement*>& SplitElements, bool bNewID = false);
 	bool SpliteElementByCZone2(MSElement* pOrgElement, vector<MSElement*>& SplitElements, bool bNewID = false);
	int GetOverlapCZoneID(double dXc, double dYc, bool IsHorizontalZone);
	int GetOverlapCZoneID(GM2DVector TheVec, bool IsHorizontalZone);
	vector<int> GetOverlapCZoneIDs(MSPolylineObject* pPolylineObject, bool IsHorizontalZone, double dTol);
	vector<int> GetOverlapCZoneIDs(GM2DPolyline* pPoly, bool IsHorizontalZone, double dTol);

	//virtual BOOL FindConnectivity(MSLinkMemberG* pMem, double dNodeTol);
	
	virtual vector<long> GetCompoGs(long nMID);
    
	double GetZLevel(double Xc, double Yc);
    
    long MergeFloorCrossMembers(MSLinkMemberG* pMyLink);
	long MergeTCrossLinkMembers(MSLinkMemberG* pMyLink, MSLinkMemberG* pUrLink);
	
    long MergeCrossPointLinkStep3(MSCompoG* pMyPoint, MSLinkMemberG* pUrLink, char cFloorVertical, char cToFloorOfYou);
    /*bool AddClonedFloor( MSFloor* floor );
	bool RemoveClonedFloor( MSFloor* floor );*/
    map<long, MSCompoG*> FindAndInsertLevelDif2LineMembers(MSLineLevelDifG* pLineDifG); // call by Add LevelZone
	map<long, MSCompoG*> FindAndRemoveLevelDif2LineMembers(MSLineLevelDifG* pLevelDifG); // callby Remove LevelZone
    void InsertLevelDif2ALinkMemberByLevelZone(MSLinkMemberG* pLineG);  // call by Add LinkMemberG
	void InsertLowLevelDif2AWallMemberByLevelZone(MSLinkMemberG* pLineG);  // call by Add WallMemberG
    void InsertLevelDif2SlabByLevelZone( MSSlabG* pSlabG, bool isDoit = true );  // call by Add SlabG
	//MSFloorQT* mp_QT;
	//void BuildQT();
	//long InsertNewCompoG( MSCompoG* pMem );
	//void MergeJoints(MSCompoG* pMyCompoG);
	//MSJoint* MergeAJoint(MSCompoG* pCompoG, MSJoint** pMyJoint, long StartOrEnd, CTypedPtrArray<CObArray, MSFloorQT*>& CrossQTArr);
	//MSJoint* MakeDefaultJoint(MSCompoG* pCompoG, MSJoint** pMyJoint, long StartOrEnd);
	void FindCenterLinesForFindSlab(vector<GM2DPolyline*>& vProfile_in, vector<GM2DCurve*>& vCurve);
	void FindCenterLinesForFindSlab__FindLeftRightProfile(vector<GM2DPolyline*>& vProfile_in, GM2DVector* p1, GM2DVector* p2, bool& exist_left, bool& exist_right);
	void FindCenterLinesForFindSlab_CalcProjection(GM2DVector& p1, double uvec[], GM2DVector& point, double& dist, double& h);
	void FindCenterLinesForFindSlab_CalcProjection(GM2DArc* arc, GM2DVector& point, double& dist, double& h);
	void FindCenterLinesForFindSlab_ConnectParallelLines(vector<GM2DCurve*>& lines, double ECC_TOL, double TOL);
	void OverlapAngle(double a1[2], double a2[2], bool& overlap, double joined[2]);
	bool IsAngleInside(double a1, double a2, double a_test);
    
	void FindOverlapLinkMembers(GM2DCurve* LVec,map<long,MSLinkMemberG*>& SptMemberMap, long nMemberType, double DegreeTol,double dDistDtol, double LinkTestLength);
    void SetSite(GM2DRectangle* SiteRect=NULL);
	
	void CheckFloorQT(vector<MSCompoG*> CompoGArr);
	void ResetFloorQTLeaves();

	MSObject* CheckCompoGOverlap_WithUpDownFloor(MSCompoG* pCompoG, long nOldCompoGID = 0, bool bMakeErrorObject = true, bool isCheckZLevel = true);
	MSObject* CheckCompoGOverlap(MSCompoG* pCompoG, long nOldCompoGID = 0, bool bMakeErrorObject = true, bool isCheckZLevel = true, bool bCheckJoint = true);
	MSObject* CheckSlabGOverlap(MSCompoG* pCompoG, long nOldCompoGID = 0);
	MSElement* CheckOverlapLevelZone(MSLevelZone* pZone);
	MSConstructionZone*  CheckOverlapConstructionZone(MSConstructionZone* pZone, long nOldCZoneID = 0);
	MSSlabRebar* CheckSlabRebarOverlap(MSSlabRebar* pUrSlabRebar, long nOldCompoGID, bool bMakeErrorObject=true);

	vector<long> CheckExtendOverlap(MSCompoG* pCompoG, bool isUpdateGeometry = true);

	MSSlabG* FindSlab();

	bool CheckLevelDifGSerializing();

	CTime GetStartDate();
	CTime GetFinishDate();

	MSCompoM* GetMostlyUsedCompoM(MSElement::Type_MSCompoG eType);

	MSCompoM* GetMostlyUsedSlabMByArea();
	double GetMostlyUsedSlabMDepth();
	double GetBiggerMemberDepth(MSElement::Type_MSCompoG eType);
	double GetLevelZoneLevel(bool isMax);
	MSLevelZone* GetContainingLevelZone(GM2DPolyline* pPoly);
	double GetWallSOffSet(bool isMax);
	double GetWallEOffSet(bool isMax);

	//void TrimExistingSlabs(MSSlabG* pSlabG);
	//void TrimExistPolyLineObjectStep1(MSPolylineObject* pPolylineObject);
	//void TrimExistPolyLineObjectSetp2(MSPolylineObject* pCurPolylineObject, MSPolylineObject* pPolylineObject);

	virtual double GetFloorLevel();
	virtual vector<MSFloor*> GetFloorsByLevel(double dStartLevel, double dEndLevel);

	//Back 도면
	vector<int> GetUsedReinforceBarSettingID(MSCompoM::Type_MSCompoM eMType);
	vector<int> GetUsedMaterialID(MSCompoM::Type_MSCompoM eMType);
	vector<double> GetUsedFck(MSCompoM::Type_MSCompoM eMType);
	
	//int nReinforceBarSettingID = 0 일경우 => 모든 ReinforceBarSetting을 찾는다.
	vector<double> GetUsedMainBarDia(MSCompoM::Type_MSCompoM eMType, int nReinforceBarSettingID = 0, bool isFoundation = false);
	vector<double> GetUsedStirrupBarDia(MSCompoM::Type_MSCompoM eMType, int nReinforceBarSettingID = 0, bool isFoundation = false);

	double GetHeight(){return m_dHeight;};
	void SetHeight(double dHeight, bool bChangInfo = false);
	bool SetHeightAndSlabLevel(double height, double slabLevel);
	bool CheckCompoGs();

	void SetRunModifyObjectCompoG(bool isRunModifyObjectCompoG){m_isRunModifyObjectCompoG = isRunModifyObjectCompoG;};
	bool GetRunModifyObjectCompoG(){return m_isRunModifyObjectCompoG;};

	MSCompoG* GetMSCompoG(GM2DVector* vec, Type_MSCompoG memberType);

	void UpdateCompoGs();
	void UpdateCompoGs(map<long, MSCompoG*> CompoGMap, bool IsRemoveConnection);

	void InitCompoGZLevel();
	void ReSetCompoGZLevel();

	double GetTopCompoGZLevel();
	double GetBotCompoGZLevel();

	void AddCompoGZLevel(double dZLevel);
	void DelCompoGZLevel(double dZLevel);

	double GetSlabLevelOffset()										{ return m_SlabLevelOffset;};
	void SetSlabLevelOffset(double slabLevelOffset);

	virtual double GetSlabLevel();
	double GetSlabHeight();
	void ClearCashFloorLevel();

	void FixBoundaryCompoGs();

	map<MSElement::Type_MSCompoG, vector<MSCompoG*>> GetCompGTypeListMap();
	map<CString, tuple<double, double>> GetCompoGsTypeGeoData(map<MSElement::Type_MSCompoG, vector<MSCompoG*>> CompoGTypeMap);
	void LoggingCompoGsTypeGeo(map<CString, tuple<double, double>> CompoGGeoMapData);
	void LogningConnections();

protected:
	CArray<GM2DLineVector> ma_OutLines;
	map<int, vector<int>> m_mapTwoNodeToConnectedSlabs;//key : code startNode + EndNode
	double m_dHeight;
	double m_SlabLevelOffset;
	double m_dCashFloorLevel;
	double m_dCashSlabLevel;


	// ZLevel, Cnt
	map<double, int> mm_CompoGZLevel;

	// CompoG의 ModifyObject() 함수 호출시 True ==> LevelDif Width 값 변경시 슬래브의 LevelDif다시 안만들기 위해 사용
	bool m_isRunModifyObjectCompoG;
//
//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	// start of addition by ironbell
//protected:
//	virtual void InnerImportIFC(int pIfcId);
//	virtual int  InnerExportIFC();
//	// end of addition by ironbell
//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	map<long, long> tmep_CloneMap;
	long tmep_SourceID;

	// mylee - CompoG, Joint IDs which need to be updated
public:
	// 현재 동작에서 다시 그리기 실행되어야할 CompoGID 리스트
	static vector<int>& CompoGIDList_NeedRegen_CurTR();
	// 현재 동작 그리고 2DView에서만 다시 그리기 실행되어야할 CompoGID 리스트
	static vector<int>& CompoGIDList_NeedRegen_2DView_CurTR();

	// 3DView,2DView 에서 다시 그리기 실행되어야할 CompoGID 리스트
	static vector<int>& CompoGIDList_NeedRegen();
	// 2DView에서만 다시 그리기 실행되어야할 CompoGID 리스트
	static vector<int>& CompoGIDList_NeedRegen_2DView();
public:
	//static void CalcAffected_CompoGID_JointID(MSCompoG* pCompoG, vector<int>& vCompoGID, vector<int>& vJointID);
	static void CalcAffected_CompoGID_ByLevelZone(MSFloor* pFloor, MSLevelZone* pZone, vector<int>& vCompoGID);

	void FindOverlapMembersNCalcTrimOption(MSCompoG* pCompoG, bool bSecondTime = false);
	void FindOverlapMembers(MSCompoG* pCompoG, bool bSecondTime = false);
	void UpdateTrimmedBoundary( MSCompoG* pCompoG );
	void UpdateTrimmedBoundary(vector<MSCompoG*> PartnerCompoGArr);
	static void CalculateUpdatedCompoGArray( MSElement* pCompoG, vector<int>& param2, bool bUpdatePartner = true );

	void FindOverlapMembernUpdateTrimBoundary(MSCompoG* pCompoG, bool bSecondTime);

	MSSlabG* FindSlabGByMaxOverlapArea(GM2DPolyline* pPoly);

	int m_nRebarTemplateID;
	BaseRebarTemplate* GetRebarTemplate(RT_MemberType type);
	CString GetRebarTemplateName();
	void SetRebarTemplateName(CString sName);
	int GetRebarTemplateID();

	int m_nFormWorkTemplateID;
	FormWorkTemplate* GetFormWorkTemplate();
	CString GetFormWorkTemplateName();
	void SetFormWorkTemplateName(CString sName);
	int GetFormWorkTemplateID();

	void WriteLogInfo();
	MSPile* CreatePile();
	void Add(MSPile* pile);
	void Remove(MSPile* pile);
	void Delete(MSPile* pile);
	MSPile* GetPile(long nID);
	bool IsOverlapPile(MSPile* pile);

	void Convert_UType_Floor();
	vector<int> GetCompoGIDInFloorQT(GM2DRectangle* pRect);

	protected:
		CString m_FloorType;
		CString m_FloorClassification;
		PlacementDirectionType m_ePlacementDirectionType;
	public:
		CString GetFloorType() { return m_FloorType; };
		void SetFloorType(CString value) { m_FloorType = value; };
		CString GetFloorClassification() { return m_FloorClassification; };
		void SetFloorClassification(CString value) { m_FloorClassification = value; };

		void SetDefFloorTypeNClassification();

		PlacementDirectionType GetPlacementDirectionType();
		void SetPlacementDirectionType(PlacementDirectionType value);

		void FixCompoGProfile();
		vector<MSCompoG*> FixGeometry();
		vector<MSCompoG*> FixGeometry(map<long, MSCompoG*> CompoGMap);

		void UpdateSlabInnerProfile();
};
