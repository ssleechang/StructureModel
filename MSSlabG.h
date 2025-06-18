#pragma once
#include "MSCompoG.h"

#include "GMLib\GMPolygon.h"
#include "GMLib\GM2DPolyline.h"
#include "GMLib\GM2DArc.h"
#include "GMLib\GM2DRectangle.h"
#include "MSStirrupBar.h"
#include "MSSlabRebarInfo.h"
#include "MDMember.h"
#include "MSSubHaunchG.h"
#include "MSMaterial.h"
#include "MSDeformedBar.h"

#include "MSReinforceBar.h"
#include "ReinforceBarSetting.h"
#include "MSFloor.h"

#include "MSSlabLineLevelDifG.h"
#include "MSSlabRebar.h"

class MSLinkMemberG;
class MSOpening;
class MSAssembly;
class MSLevelZone;
class MSLineLevelDifG;
class MSPolylineObject;
class MSWell;
class MSTrench;
class MSOpeningG;
class MSReinforceBar;
class MSReinforceBarSetting;

#pragma region --- MSSlabSptMemPart
class StructureModel_CLASS MSSlabSptMemPart : public MSObject
{
public:
	MSSlabSptMemPart() {};
	~MSSlabSptMemPart() {};

	//CTypedPtrArray<CObArray, MSLinkMemPart*> ma_MemPart;
};
#pragma endregion

#pragma region --- _LevelDifInfoStruct

typedef struct _LevelDifInfoStruct
{
public:
	_LevelDifInfoStruct()
	{
		Initialize();
	};
	_LevelDifInfoStruct(const _LevelDifInfoStruct& aData)	{*this = aData;};

	_LevelDifInfoStruct& operator = (const _LevelDifInfoStruct& aData)
	{
		Initialize();
		m_dWallWidth = aData.m_dWallWidth;
		m_bHasHaunch = aData.m_bHasHaunch;
		m_bHasWall = aData.m_bHasWall;
		m_dHaunchX = aData.m_dHaunchX;
		m_dHaunchY = aData.m_dHaunchY;
		m_dHaunchAngle = aData.m_dHaunchAngle;
		m_bUseHaunchMaterial = aData.m_bUseHaunchMaterial;
		m_nHaunchMaterialID = aData.m_nHaunchMaterialID;
		m_bUseBackFillFormwork = aData.m_bUseBackFillFormwork;
		m_IsUsedStoryMaterial = aData.m_IsUsedStoryMaterial;
		return *this;
	};
	void Initialize()
	{
		m_dWallWidth = 0;
		m_bHasHaunch = false;
		m_bHasWall = false;
		m_dHaunchX = 0.0;
		m_dHaunchY = 0.0;
		m_dHaunchAngle = 45;
		m_bUseHaunchMaterial = false;
		m_nHaunchMaterialID = 0;
		m_bUseBackFillFormwork = true;
		m_IsUsedStoryMaterial = true;
		//m_ID = 0;
	};

	void Serialize(CArchive &ar);

	void Dump(EFS::EFS_Buffer& buffer)
	{
		buffer << m_dWallWidth;
		buffer << m_bHasHaunch;
		buffer << m_bHasWall;
		buffer << m_dHaunchX;
		buffer << m_dHaunchY;
		buffer << m_dHaunchAngle;
		buffer << m_bUseHaunchMaterial;
		buffer << m_nHaunchMaterialID;
		buffer << m_bUseBackFillFormwork;
		buffer << m_IsUsedStoryMaterial;
	};

	void Recovery(EFS::EFS_Buffer& buffer)
	{
		buffer >> m_dWallWidth;
		buffer >> m_bHasHaunch;
		buffer >> m_bHasWall;
		buffer >> m_dHaunchX;
		buffer >> m_dHaunchY;
		buffer >> m_dHaunchAngle;
		buffer >> m_bUseHaunchMaterial;
		buffer >> m_nHaunchMaterialID;
		buffer >> m_bUseBackFillFormwork;
		buffer >> m_IsUsedStoryMaterial;
	};

	long GetHaunchMaterialID(MSFloor* pFloor);
	void SetHaunchMaterialID(long materialID)		{ m_nHaunchMaterialID = materialID; };
	bool GetIsUsedStoryMaterial()												{ return m_IsUsedStoryMaterial; };
	void SetIsUsedStoryMaterial(bool bUsedStoryMaterial)	{ m_IsUsedStoryMaterial = bUsedStoryMaterial; };

public:
	double m_dWallWidth;
	bool m_bHasHaunch;
	bool m_bHasWall;
	double m_dHaunchX;
	double m_dHaunchY;
	double m_dHaunchAngle;
	bool m_bUseHaunchMaterial;
	bool m_bUseBackFillFormwork;
	int m_nHaunchMaterialID;
	bool m_IsUsedStoryMaterial;
	//long m_ID;
}LevelDifInfoStruct;

#pragma endregion

#pragma region --- MSSlabLevelZone

class StructureModel_CLASS MSSlabLevelZone : public MSObject
{
public:
	MSSlabLevelZone();
	~MSSlabLevelZone();

	long m_LevelZoneID;
	MSLevelZone* mp_LevelZone;
	GM2DPolyline* mp_Polyline;
	bool m_IsDelete;

	bool m_IsNewSlabZone;
	bool m_IsFullSlabZone; // 슬래브와 슬래브 래젤존의 크기가 같다. XMLExport시 처리 안한다.

	double m_dPreWidth;
	double m_dWidth;
	double m_dMaxWidth;

	vector<LevelDifInfoStruct> ma_LevelDifInfo;

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);

	long FindIdxByCurve(GM2DCurve* pCurve);
	//bool GetLevelDifID(MSLineLevelDifG* pLevelDifG, long& ID);
	//bool SetLevelDifID(MSLineLevelDifG* pLevelDifG, long ID);

	bool GetLevelDifWidth(MSLineLevelDifG* pLevelDifG, double& dWidth);
	bool SetLevelDifWidth(MSLineLevelDifG* pLevelDifG, double dWidth);

	bool GetLevelDifHasWall(MSLineLevelDifG* pLevelDifG, bool& HasWall);
	bool SetLevelDifHasWall(MSLineLevelDifG* pLevelDifG, bool HasWall);

	bool GetLevelDifHasHaunch(MSLineLevelDifG* pLevelDifG, bool& HasHaunch);
	bool SetLevelDifHasHaunch(MSLineLevelDifG* pLevelDifG, bool HasHaunch);

	bool GetLevelDifHaunchX(MSLineLevelDifG* pLevelDifG, double& HaunchX);
	bool SetLevelDifHaunchX(MSLineLevelDifG* pLevelDifG, double HaunchX);

	bool GetLevelDifHaunchY(MSLineLevelDifG* pLevelDifG, double& HaunchY);
	bool SetLevelDifHaunchY(MSLineLevelDifG* pLevelDifG, double HaunchY);

	bool GetLevelDifHaunchAngle(MSLineLevelDifG* pLevelDifG, double& HaunchAngle);
	bool SetLevelDifHaunchAngle(MSLineLevelDifG* pLevelDifG, double HaunchAngle);

	bool GetLevelDifUseHaunchMaterial(MSLineLevelDifG* pLevelDifG, bool& value);
	bool SetLevelDifUseHaunchMaterial(MSLineLevelDifG* pLevelDifG, bool value);

	bool GetLevelDifUseBackFillFormwork(MSLineLevelDifG* pLevelDifG, bool& value);
	bool SetLevelDifUseBackFillFormwork(MSLineLevelDifG* pLevelDifG, bool value);

	bool GetLevelDifHaunchMaterialID(MSLineLevelDifG* pLevelDifG, int& value);
	bool SetLevelDifHaunchMaterialID(MSLineLevelDifG* pLevelDifG, int value);

	bool GetLevelDifHaunchUsedStoryMaterial(MSLineLevelDifG* pLevelDifG, bool& value);
	bool SetLevelDifHaunchUsedStoryMaterial(MSLineLevelDifG* pLevelDifG, bool value);

	void Serialize( CArchive& archive );
};

#pragma endregion

#pragma region --- MSSlabG

class StructureModel_CLASS MSSlabG : public MSCompoG
{

public:
	enum SlopeMethod { ThreePoint_0, PointOffset_1, SlopeArrow_2 };
	MSSlabG();
	~MSSlabG(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSSlabG)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual void Init(bool bInConstructor=false);    virtual Type_MSCompoG GetType(){return msSlabG;};
	virtual long ResetGetID();
	bool CheckProfile();

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 
	virtual CString ToString();

	virtual void ReMakeLevelDif(bool bMakeSlab = true);

	virtual void RecoverPointerAfterSerialize(MSFloor* floor);
	virtual void RecoverPointerAfterSerialize_SubMembers();

	//vector<MSOpening*> ma_Opening;

	double m_dLxDeg;//주근의 배근각
	double m_dOffset;
	// PC or Deck용 골기준점
	GM2DVector m_RibPosition;

	void SetLxDeg(double deg);
	void UpdateLxDegWithRibDir(bool isXDir_Old, bool isXDir_New);

	vector<MSSlabLineLevelDifG*> ma_LevelDif;
	vector<MSLineLevelDifG*> ma_LevelDifOnEdge;
	vector<MSSlabLevelZone*> ma_LevelPoly;

	vector<MSSubHaunchG*> ma_Haunches ;
	
	vector<MSLinkMemberG*> GetLevelDifGAll(bool bGetAllEdgeWall = false, bool bGetSameLRZLevel = false);
	
	bool HasLevelZoneInfo();
	void SetLevelPolyDelete(MSLevelZone* pLevelZone);

// 	bool GetLevelDifID(MSLineLevelDifG* pLevelDifG, long& ID);
// 	bool SetLevelDifID(MSLineLevelDifG* pLevelDifG, long ID);

	bool GetLevelDifWidth(MSLineLevelDifG* pLevelDifG, double& dWidth);
	bool SetLevelDifWidth(MSLineLevelDifG* pLevelDifG, double dWidth);

	bool GetLevelDifHasWall(MSLineLevelDifG* pLevelDifG, bool& HasWall);
	bool SetLevelDifHasWall(MSLineLevelDifG* pLevelDifG, bool HasWall);

	bool GetLevelDifHasHaunch(MSLineLevelDifG* pLevelDifG, bool& HasHaunch);
	bool SetLevelDifHasHaunch(MSLineLevelDifG* pLevelDifG, bool HasHaunch);

	bool GetLevelDifHaunchX(MSLineLevelDifG* pLevelDifG, double& HaunchX);
	bool SetLevelDifHaunchX(MSLineLevelDifG* pLevelDifG, double HaunchX);

	bool GetLevelDifHaunchY(MSLineLevelDifG* pLevelDifG, double& HaunchY);
	bool SetLevelDifHaunchY(MSLineLevelDifG* pLevelDifG, double HaunchY);

	bool GetLevelDifHaunchAngle(MSLineLevelDifG* pLevelDifG, double& HaunchAngle);
	bool SetLevelDifHaunchAngle(MSLineLevelDifG* pLevelDifG, double HaunchAngle);

	bool GetLevelDifUseHaunchMaterial(MSLineLevelDifG* pLevelDifG, bool& value);
	bool SetLevelDifUseHaunchMaterial(MSLineLevelDifG* pLevelDifG, bool value);

	bool GetLevelDifUseBackFillFormwork(MSLineLevelDifG* pLevelDifG, bool& value);
	bool SetLevelDifUseBackFillFormwork(MSLineLevelDifG* pLevelDifG, bool value);

	bool GetLevelDifHaunchMaterialID(MSLineLevelDifG* pLevelDifG, int& value);
	bool SetLevelDifHaunchMaterialID(MSLineLevelDifG* pLevelDifG, int value);

	bool GetLevelDifHaunchUsedStoryMaterial(MSLineLevelDifG* pLevelDifG, bool& value);
	bool SetLevelDifHaunchUsedStoryMaterial(MSLineLevelDifG* pLevelDifG, bool value);

	void SetLevelDifWidthInSlabLevelZone();
	void GetLevelDifWidthInSlabLevelZone();

	long MakeSlabLevelZone(MSLevelZone* pLevelZone, bool& isDoit);
	void ResetSlabLevelZone();

	void DeleteSlabLineLevelDifAll();
	void DeleteSlabLineLevelDif(MSLineLevelDifG* pLineLevelDifG);
	void DeleteSlabLineLevelDifByLevelZone(MSLevelZone* pLevelZone);

	long CheckCrossThenAddLevelDif(MSLevelZone* pLevelZone, bool isDoit = true);
	void CheckCrossThenAddLevelDifStep2(GM2DCurve* pCurve,  MSLineLevelDifG* pLevelDif, MSLevelZone* pZone);
	bool RemoveLevelDif(MSLineLevelDifG* pLevelDif);
	bool HasLevelDif(MSLineLevelDifG* pLevelDif);
	void RemoveLevelZone(MSLevelZone* pLevelZone);
	//bool TrimLevelDif();

	bool AddLevelDif(MSSlabLineLevelDifG* pDif);

	virtual void ModifyObject(bool isDoit = true);

	MSSlabLevelZone* FindSlabLevelZone(MSLevelZone* pZone, GM2DVector TheVec, bool bFindDeleted = true);
	MSSlabLevelZone* FindSlabLevelZone(GM2DPolyline* pPoly, bool bFindDeleted = true);

	virtual bool HasLevelDif();
	virtual bool IsSlopeMember();
	void ClearSlopeInfo();
	SlopeMethod m_nSlopeMethod; // 0: three points, 1: move point, 2: slope arrow
	vector<GM2DVector> ma_SlopePoints; // used in method 0, 2
	vector<int> ma_SlopePointIdx; // used in method 1
	vector<int> ma_SlopeZOffset; // 경사슬래브 Point3 Z Offset, TODO change type into double
	double GetZOffsetOnSlabTopPlane(double x, double Y);
	void GetProfileZOffset_BySlope(vector<double>& zoffset_list);
	void GetZLevels_Slope(double& top, double& bot);
	void FindPlaneEquation(double p1[3], double p2[3], double p3[3], double ret[4]);
	bool GetPlaneEquation(double ret[4]);
	double GetFullSlabLevelZoneLevel();
	double GetThick_ZDir();

	bool IsSameSlopeSlab(MSSlabG* pUrSlabG);
	void RemoveSubMemberConnections();
	virtual bool HasSubMember() override;
	virtual void DeleteSubMembers() override;

public:
	virtual GM2DPolyline* GetProfile();
	virtual void SetProfile(GM2DPolyline* pPoly);

	void GetProfile_InSplitPointLevelZone(GM2DPolyline& ThePoly);
	virtual double GetRelativeStartLevel();
	long GetSlabMType();
	bool IsFlatSlabBType();
	virtual bool IsMatSlab();
	virtual bool IsFlatSlab();
	virtual bool IsDeckSlab();
	virtual bool IsRebarEmbeddedDeckSlab();
	virtual bool IsGeneralDeckSlab();
	virtual bool IsPC();
	virtual bool IsHalfPCSlab();
	virtual bool IsPCSlab();
	virtual bool IsIMTPCSlab();
	virtual bool IsHCSPCSlab();
	virtual bool IsRPSPCSlab();
	virtual bool IsGeneral(); // 4변 지지 슬래브
	virtual bool IsSOG();
	virtual bool IsCircularRamp();
		
	bool IsSlabGroup();
	bool IsFlatSlabGroup();
	bool IsFoundationSlabGroup();
	bool IsCircularRampGroup();
	bool IsDeckSlabGroup();
	bool IsPCSlabGroup();
	


	virtual double GetThick();
	virtual bool IsAutoDegreeBySlabType();
	virtual double GetBotOffSet() { return m_dOffset; };
	virtual vector<MSCompoG*> GetPartnerCompoGs(long GetIdx = 0);
	void MSSlabG::InitTrimmedBoundary(bool IsTopLevel = true);
 	virtual void InitTrimmedBoundaryVisibility();
	virtual void UpdateBoundaryVisibilities(long nUpdateConnIdx = 0);
//	virtual bool SetCloneJoint( MSAssembly* pCloneAssem );

	virtual CString GetKey(bool bUseID = true);

	// 0: �ȱ׷����¼�, 1: �׸��¼� 2: ����
	vector<GM2DCurve*> GetVisibileCurves(eVisibileCurveType nVisibile);

	bool HasHiddenBoundaryCurve();

	vector<MSCompoG*> GetOpeningGArr();
	void MSSlabG::GetInnerSlabGnPitGArr(vector<MSCompoG*>& inner);
	void MSSlabG::GetInnerSlabGnPitGArrAndCrossOpening(vector<MSCompoG*>& inner, vector<MSOpening*>& crossOpening);
	void GetInnerOrOverlapGArr(vector<MSCompoG*>& inner, vector<MSCompoG*>& overlap);

	map<long, long> mm_OpeningID;

	GM2DPolyline* mp_InnerProFile;
	GM2DPolyline* mp_InnerProFile_Bot;

	GM2DPolyline* mp_DeckSlabQTProFile;

	GM2DPolyline* mp_LxLyProFile;

	virtual bool MakeBoundary();

	void UpdateInnerProfile(bool isTop = true);
	
	GM2DPolyline GetInnerProfile(bool isTop = true);


	void UpdateDeckSlabQTProFile();
	GM2DPolyline GetDeckSlabQTProFile();

	GM2DPolyline* GetLxLyProFile();

	virtual void Translate(GM2DVector &pDelta);
	virtual void MirrorByAxis(GM2DLineVector& pAxis);
	virtual void RotateZ(double theta, GM2DVector *pCenter);
	
	void VerifyAndCureBoundaries(double dTol);//boundary와 opening들을 검사해서 중복점을 제거하고 모두 closed로 만든다.
	// VerifyAndCureBoundaries() 함수에서 선위에 있는 점을 삭제하면 물량이 달라지는 경우가 발생한다
	// FixProfile() 함수에서 선위에 있는 점을 삭제 하도록한다, 프로파일 에러를 처리하기위해 만든 함수이다.
	virtual void FixProfile() override;
    
    double GetStartLevel();
	
	// 슬래브 오프닝은 무시한다.
	double GetSlabLevel(GM2DVector TheVec);

    CTypedPtrArray<CObArray, MSSlabSptMemPart*> ma_EdgeSpt;
    
    vector<MSLinkMemberG*> ma_SptMember;

	bool IsSameGeometry(MSCompoG* urCompoG, double distTolerance, double degreeTolerance) override;
	void GetPolyline(GM2DPolyline& pPoly);
	/*void AddSlabLevelDif(MSLineLevelDifG* pSlabLevelDif);*/

	long LevelOverlap(MSSlabG* pUrSlab);
	virtual bool GetZLevels( double& TopLevel, double& BottomLevel, bool GetBigVal = true, bool bCheckLevelZone = true  );
	void GetZLevels(GM2DVector TheVec, double& TopLevel, double& BottomLevel, bool GetBigVal = true );
	void GetZLevels(GM2DCurve* pCurve, double& TopLevel, double& BottomLevel, bool GetBigVal = true );
	double GetTopLevel(GM2DCurve *pCurve, bool getBigVal = true);
	double GetBotLevel(GM2DCurve *pCurve, bool getBigVal = true);
	virtual bool GetZLevelsWidthOutZLevel( double& TopLevel, double& BottomLevel);
	virtual long GetZLevelInfo(MSCompoG* pUrCompoG);
	double GetSlabLevelZoneLevel(GM2DVector TheVec);

	bool HasFullSlabZone();

	virtual bool VisibleInRect( GM2DRectangle* pRect);
	virtual inline void CalcBoundRectBeforeAddQuadTree();

	bool AddOpening(MSOpening* pOpening);
	bool AddWell(MSWell* pWell);
	bool AddTrench(MSTrench* pTrench);
	void AddEdgeWall(double x, double y, double x2, double y2, double width, double height);
	bool Remove(MSLineLevelDifG* pLineLevelDifG);

	static void ModifyEdgeWallWhenModifySlab(MSSlabG* org, MSSlabG* modifed_slab, bool isMirror);

	void TrimSlabLineLevelDifbyOpening();
	bool TrimSlabLineLevelDifbyOpening(MSSlabLineLevelDifG* pDif);
	void TrimEdgeWallbyOpening();
	void TrimEdgeWallbyOpening(MSLineLevelDifG* pDif);
	vector<GM2DCurve*> SplitCurveByOpening(GM2DCurve* pCenCurve);

	virtual void AddHaunch(double x, double y, double x2, double y2, double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto);
	virtual void AddHaunchesOnProfile(double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto);
	virtual void DeleteHaunchs();
	virtual bool HasHaunch();

	void TrimHaunches();
	void RegenHaunch();

	void UpdateSubMember();

	void MergeWell();
	void MergeTrench();

	// 슬래브의 변경으로 Sub 부재들을 다시 Trim 해줘야 하는경우 호출 하는 함수들
	void TrimLevelDifbySlab();
	void TrimUserLevelDifbySlab();
	void TrimHaunchbySlab();
	void TrimWellsbySlab();
	void TrimTrenchsbySlab();
	void TrimTrenchbyWell(MSTrench* pTrench, MSWell* pWell);
	void TrimTrenchbyWellAll();
	// 슬래브의 변경으로 Sub 부재들을 다시 Trim 해줘야 하는경우 호출 하는 함수들

	bool OverlapWell(GM2DPolyline* pBasePoly);

	map<long, MSLevelZone*> FindLevelZonebyDifG();

	static void TrimLevelDifAllByLevelZone(MSSlabG* pSlab, map<long, MSSlabG*>& SlabMap);
	void TrimLevelDifAll();
	void TrimEdgeWallLevelDif();

	virtual GM2DVector* GetVec(long Index);

	virtual void InitializeParameter();

	long Contains( GM2DVector* pVec, double tolerance = DTOL_GM);
	bool Contains(GM2DCurve* pCurve, bool bIncludeOnLine, double tolerance = DTOL_GM);
	long Contains(GM2DPolyline* pPoly, bool bIncludeOnLine, double tolerance = DTOL_GM);
	
	bool IsSlabNOpeningOnLine(GM2DCurve* pCurve);

	void DeleteSlabRebar();

	vector<MSSlabRebar*> ma_Rebar;
	map<long, MSWell*> mm_Well;
	map<long, MSTrench*> mm_Trench;
	vector<MSLineLevelDifG*> ma_EdgeWall;

	vector<MSLineLevelDifG*> GetZLevelOverLapEdgeWall(MSSlabG* pUrSlabG);

	MSWell* GetWell(long nID);
	void DeleteWells();

	MSTrench* GetTrench(long nID);
	bool Remove(MSTrench* pTrench);
	bool Delete(MSTrench* pTrench);
	void DeleteTrenchs();

	void DeleteEdgeWalls();

	static void CalcInitialLxDegFromProfile(GM2DPolyline& profile, double& rDeg, GM2DVector& rib_pos);
	static void CalcInitialLxDegFromProfile_SlabRebar(GM2DPolyline& profile, double& rDeg, GM2DVector& rib_pos);
	double GetInitialLxDegFromProfile(GM2DVector& rib_pos);
	
	// 모델링중에 사용되는 임시 변수
	bool m_HasOverLapSlopeSlab;
	bool Is2WaySlab();

	void MakeSlabOpening();
	void DeleteSlabOpening();

	// targetProfile + Deg 값으로 longLx, longLy 계산하기
	void CalcLongXLongY(GM2DPolyline& targetProfile, GM2DLineVector& longLx, GM2DLineVector& longLy);
	// 슬래브 프로파일로 계산
	void GetSlabLxLy(GM2DLineVector& Lx, GM2DLineVector& Ly);
	// 슬래브의 외각선에 붙은 보의 센터로 계산
	void GetLxLy(GM2DLineVector& Lx, GM2DLineVector& Ly);
	// 이너프로파일로 계산
	void GetLnxLny(GM2DLineVector& Lnx, GM2DLineVector& Lny);
	// Lx, Ly를 1, 4사분면 방향으로 돌린다.
	void UpdateOneorFourQuadrantLine(GM2DLineVector& Lx, GM2DLineVector& Ly);

	bool IsColumnStrip();

	bool HasPathLine();
	GM2DLineVector GetPathLine();

	virtual GM2DPolyline* GetTrimmedBoundary(bool IsTop = true);

	vector<GM2DCurve*> GetSupportCurve();
	void SetSubMemberID();
	void UpdatePartnerSlab();

	virtual bool UsedMaterial(long materialID) override;
	void SubMemberMaterialValidCheck();

	virtual vector<MSElement*> GetSubMembers() override;

	virtual void UpdateTrimmedBoundary_AfterCalculate(CON_POS ePos = CON_ALL);

	virtual void CalculateTrimmedBoundary_Acis() override;

	//HCS,RPS 정보 - 시작
	vector<GM2DLineVector> GetStartHoleLines(bool onlyFillHole = false, bool isPlaceRebar = false);
	vector<GM2DLineVector> GetEndHoleLines(bool onlyFillHole = false, bool isPlaceRebar = false);
	vector<GM2DLineVector> GetHoleLines(bool isStart, bool onlyFillHole = false, bool isPlaceRebar = false);
	vector<double> GetHoleLineLocs();
	vector<bool> GetIsFillHoles();

	double GetLastHoleDia();
	double GetHoleLineLength();

	vector<GM2DLineVector> GetStartHoleEndLines();
	vector<GM2DLineVector> GetEndHoleEndLines();
	vector<GM2DLineVector> GetHoleEndLines(bool isStart);
	vector<double> GetRPSLxBotSpliceBarLocs(long numBar, double dSideCover);

	vector<GM2DPolyline> GetStartRPSRibLines();
	vector<GM2DPolyline> GetEndRPSRibLines();
	vector<GM2DPolyline> GetRPSRibLines(bool isStart);
	//HCS,RPS 정보 - 끝

	// 폼웍 전용 함수 - start
	
	bool IsFWSlopeRectangleSlab();
	GM2DCurve* GetFWSlopeDirectionLine();
	GM2DPolyline* GetFWSlopeSectionProfile();
	GM2DCurve* GetFWSlopeBaseLine();

	// 폼웍 전용 함수 - end
	virtual bool RemoveSubMember(MSElement* element) override;
	vector<MSCompoG*> GetSoundAbsorptionGArr();

private:
	void Serialize_EachCompoM_Rebar(CArchive& ar);
	vector<int> ma_ProfileVisible;

	GM2DPolyline* mp_BotProfile;

	void DeleteSupport(bool isCallDestructors = false);
	bool NeedToUpdateInnerProFile();

	bool AddOpening(GM2DPolyline* pPoly);
	
	void AddWell(MSWell* pWell, GM2DPolyline* pPoly);
	bool Remove(MSWell* pWell);

	void AddTrench(MSTrench* pTrench, GM2DPolyline* pPoly, GM2DPolyline* pCenterLine);
	
	void AddHaunchGs(vector<MSSubHaunchG*> HaunchGArr);
	bool Remove(MSSubHaunchG* pWell);
	bool UpdateInnerPCSlabProfile(GM2DPolyline* pInnerProfile, bool isCalcLxLyProfile = false);
	void UpdateInnerPCSlabProfile(GM2DPolyline* pInnerProfile, GM2DPolyline& outerProfile, double dMaxCompoGWidth);

	GM2DPolyline* GetTempTrimmedDeckProfile();

////	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	// start of addition by ironbell
//protected:
//	virtual void InnerImportIFC(int pIfcId);
//	virtual int  ExportLocalPlacement();
//	virtual int  ExportGeometryModel();
//	// end of addition by ironbell
//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 우마 철근
public:
	long m_TopSpacerID;
	long m_BotSpacerID;
	virtual long GetTopSpacerID() { return m_TopSpacerID; };
	virtual long GetBotSpacerID() { return m_BotSpacerID; };

// BH-4522 PCSlabType추가
	bool GetIsHasRib();
	void SetIsHasRib(bool hasRib);
	bool GetIsRibXDirection();
	void SetIsRibXDirection(bool isRibXDirection);

	vector<GM2DPolyline> GetRibPolylineList();
	vector<GM2DPolyline> GetGeneralDeckRibPolylineList();

	void MakeCCW();
	void CheckHaunches();

	bool IsInValidSlopeInfo();
private:
	bool m_IsHasRib;
	bool m_IsRibXDirection;

public:
	virtual MSCompoM::Type_MSCompoM GetDefCompoMType() { return MSCompoM::Type_MSCompoM::msSlabM; };
};

#pragma endregion

