#pragma once
#include "MSCompoG.h"

#include "GMLib/GMLine.h"
#include "GMLib/GM2DLineVector.h"
#include "StructureModelEnum.h"

class MSSlabG;
class GM2DCurve;
class MSAssembly;
class MSMemberLevelDif;
class MSLineLevelDifG;


typedef struct _CompoMDataStr
{
public:
	_CompoMDataStr()
	{
		Initialize();
	};

	_CompoMDataStr(double _dData, long _nCnt)
	{
		Initialize();
		this->dData = _dData;
		this->nCnt = _nCnt;
	};
	_CompoMDataStr(const _CompoMDataStr& aData)	{*this = aData;};

	_CompoMDataStr& operator = (const _CompoMDataStr& aData)
	{
		Initialize();
		dData = aData.dData;
		nCnt = aData.nCnt;
		return *this;
	};
	void Initialize()
	{
		nCnt = 0;
		dData = 0;
	};

public:
	// 0 둘다 1 top 2: bot
	long nCnt;
	double dData;
}CompoMDataStr;

class MSLinkMemberG;
class StructureModel_CLASS MSColineLinkMemberGs : public CObject
{
public:
	MSColineLinkMemberGs();
	~MSColineLinkMemberGs();

	map<long, MSLinkMemberG*> mm_LinkMem;
	int Add(MSLinkMemberG* pLinkG);
	bool Find(MSLinkMemberG* pLinkG);

	double GetMostlyWidth();
	double GetMostlyDepth();

	double GetMostlyZLevel();

};

class StructureModel_CLASS MSLinkMemberG :	public MSCompoG
{
protected:
	bool MakeExtendedBoundary_NextStep(GM2DPolyline& pExtendedBoundary, GM2DCurve* pLCurve, GM2DCurve* pRCurve, long nExtendType = 0, double dExtendLen = 0);
	bool MakeExtendedBoundary_ToLine_NextStep(GM2DPolyline& pExtendedBoundary, GM2DLineVector& line, GM2DCurve* pLCurve, GM2DCurve* pRCurve);
	void FindMiterLineWithOtherLinkG_NextStep(MSLinkMemberG* link, GM2DLineVector& rLine, GM2DCurve* pLCurve, GM2DCurve* pRCurve);
public:
	GM2DPolyline* GetExtendedBoundary();
	// 0 : 시작 끝 / -1 시작 / 1 끝 연장
	virtual bool MakeExtendedBoundary(GM2DPolyline& pExtendedBoundary, long nExtendType = 0, double dExtendLen = 0);
	bool MakeTrimmedBoundary_Link2Link(MSLinkMemberG* pUrLink);
	void MakeTrimmedUnion(vector<GM2DPolyline*>& aIntersect, GM2DPolyline* pMyBoundary );
	virtual bool MakeExtendedBoundary_ToLine(GM2DPolyline& pExtendedBoundary, GM2DLineVector& line);
	virtual void FindMiterLineWithOtherLinkG(MSLinkMemberG* link, GM2DLineVector& rLine);

	virtual void BuildBoundaryByLevel(vector<GM2DPolyline>& BoundaryByLevelArr);
	static double ms_dDefExtendLen;

	static MSColineLinkMemberGs* MakeColineLinkMemberG(MSLinkMemberG* pFindLinkG, FindType_MSElement eType, map<long, MSLinkMemberG*>& UsedLinkMap);
public:
	MSLinkMemberG();
	~MSLinkMemberG(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSLinkMemberG)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual void Init(bool bInConstructor=false);
	virtual bool IsInvalidVecInput();

	virtual bool isArc();

	// 아크의 중점 백터
	virtual GM2DVector GetCenVec();
	void InitMemPart();
	bool MergeMemPart(long nMidJointIdx);
	virtual void InitializeParameter();
	virtual double GetBotOffSet() { return m_SOffsetZ; };
	double m_SOffsetZ, m_EOffsetZ;

	virtual void SetStartOffSet(double dOffSet) { m_SOffsetZ = dOffSet; };
	virtual void SetEndOffSet(double dOffSet) { m_EOffsetZ = dOffSet; };
	virtual double GetStartOffSet() { return m_SOffsetZ; };
	virtual double GetEndOffSet() { return m_EOffsetZ; };

	double GetCompoMFloorLevelOffSet();

// 양쪽 외곽선
	GM2DCurve* mp_LLine;
	GM2DCurve* mp_RLine;
// Trimmed LR-Lines
	GM2DCurve* mp_LLineTR;
	GM2DCurve* mp_RLineTR;

	bool m_hasDelPart;
	bool m_isBuriedLinkMember;
	CTypedPtrArray<CObArray, MSMemberLevelDif*> ma_MemberLevelDif;
	
private:
	double m_dWidthSetback;  // CompoM과 단면 두께가 다른 벽체에 사용

public:
	virtual void Translate(GM2DVector &pDelta);
	virtual void MirrorByAxis(GM2DLineVector& pAxis);
	virtual void RotateZ(double theta, GM2DVector *pCenter);

	virtual bool MakeBoundary();
	virtual bool MakeSubMemberBoundary();
	virtual bool SetCompoM(long MID, bool bUpdateVecInput = true);
	virtual bool SetCompoM(MSCompoM* pCompoM, bool bUpdateVecInput = true);

// 	virtual bool UnlinkFromJoint();
	
	GM2DLineVector GetStartEndLine();
	GM2DLineVector GetEndEndLine();
	GM2DLineVector GetEndLineByParam(double param);

	GM2DLineVector GetEndLine4Joint2( long StartOrEnd, BOOL bShort = TRUE );
	
	virtual double GetLength();

	GM2DLineVector GetEndLine4Joint2_ExtendToOrthoPart( long StartOrEnd, BOOL bShort /*= TRUE*/ );
	//void FindOrthoMemParts( long StartOrEnd, vector<MSLinkMemPart*>& rParts, vector<GM2DLineVector*>& rLines);
	GM2DCurve* FindFarLine( vector<GM2DCurve*> vCurves, long StartOrEnd, GM2DCurve* pCenCurve );

	GM2DLineVector GetCenLVec();

	// 미드 조인트가 SConn EConn Mid 조인트에 머지 될 경우 머지 시킴
	bool MergeMidConnectivity();

	void FixTrimmedLine();

	virtual void SetLines();
	virtual void SetTrimmedLines();
	virtual double GetRelativeStartLevel();
	virtual double GetRelativeEndLevel();
	virtual double GetRelativeLevelbyLoc(double dLocVal);
    virtual double GetWidth();
    virtual double GetDepth();
    virtual double GetThick();

	virtual bool GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal = true, bool bCheckLevelZone = true );
	virtual double GetOffSetbyLoc(double dLoc);
	virtual double GetStartOffSetbyLoc(double dLoc);
	virtual bool GetZLevelsbyLoc(double& TopLevel, double& BottomLevel, double dLoc = 0.5);

	virtual bool GetOverLapPolyLineCenLoc(GM2DPolyline* pPoly, vector<GM2DCurve*>& NewCurveArr);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 
	virtual bool IsSlopeMember();
	virtual bool HasLevelDif();
	virtual CString GetKey(bool bUseID = true);
	//cLineType 'L'= LLine, 'R'= RLine , 'C'= CenLine
	shared_ptr<GM2DCurve> GetExtendLine(char cLineType);
	virtual bool HasSubMember() override;
	virtual void DeleteSubMembers() override;

public:
	GM2DCurve* GetLongLVec(bool bExtendToOrthoPart = true);
	GM2DCurve* GetShortLVec();
	// Trimmed Boundary의 Point 기준으로 가장 길게 늘린 Center Curve를 리턴
	shared_ptr<GM2DCurve> GetLongCenCurvebyTrimmedBoundary();
	// Trimmed Boundary를 Column과 Connected Member로 자른 Center Curve를 리턴
	shared_ptr<GM2DCurve> GetLongCenCurveTrimmedByTRBoundary();
	virtual shared_ptr<GM2DCurve> GetCenCurvebyConntion();

	virtual shared_ptr<GM2DCurve> GetCenCurve();
	virtual void SetCenCurve(GM2DCurve* pCurve);
	void SetCurve_Input(GM2DCurve* pCurve);
	shared_ptr<GM2DCurve> GetCurve_Input();

	virtual void UpdateVecInput();
	virtual void UpdateVecCenter();
	void UpdateCurveInputByCenCurve();
	void UpdateCenCurveByCurveInput();

	eLINEALIGN_TYPE GetLineAlignType();
	void SetLineAlignType(eLINEALIGN_TYPE eType, bool bUpdateCurveInput = true);

    bool HitTestWithVector(GM2DVector& TheVec, double DistTol=DTOL_GM, bool bOnlyInLine=true);
//     void SetLines_MemPart();

    virtual bool VisibleInRect( GM2DRectangle* pRect);
	
	bool IsParallel(MSLinkMemberG* pUrLink, double dDegTol = 5);
	bool IsColinear4DTol(MSLinkMemberG* pUrLink, double DegreeTol, double DistTol);

	virtual bool IsSameInfo(MSLinkMemberG* pUrLink);
    double GetMaxPartnerWidth();
	
	bool HasParallelLinkMember(bool CheckSameMemberType = true);

	void SetBuriedLinkMember();

	GM2DPolyline* GetLinkMemberBoundary(bool bShort = true);
	
	GM2DPolyline* GetLinkMemberTRBoundary();

	vector<GM2DPolyline*> GetLinkMemberBoundaryTrimmedByColumn();
	// Trimmed Boundary를 Column과 Connected Member로 자른 Polyline를 리턴
	vector<GM2DPolyline*> GetLinkMemberBoundaryTrimmedByTRBoundary();
	

	//by hana-20140407
	vector<double> GetLevelDifLevel();
	virtual vector<GM2DCurve*> GetLevelDifCurves();
	
	double GetLevelbyLevelDif(bool isMax = true);
	long CheckCrossThenAddLevelDif( MSLineLevelDifG* pLineLevelDifg, CTypedPtrArray<CObArray, MSMemberLevelDif*>& MemberLevelDifArr, MSFloor* pFloor );
	bool AddLevelDif( double Loc, double dLLoc, double dRLoc, MSLineLevelDifG* pLineLevelDifg, CTypedPtrArray<CObArray, MSMemberLevelDif*>& MemberLevelDifArr, MSFloor* pFloor);
	void RemoveLevelDif( MSLineLevelDifG* pLineLevelDifg );
	void DeleteMemberLevelDifG();
	void MoveToDeleted_MemberLevelDifG();

	vector<MSMemberLevelDif*> ma_DeletedDif;


	vector<int> GetCZoneIDsByCenCurve();
	vector<double> GetLevelDifLevelByCZone(int idx, bool isTop = false);
	vector<GM2DCurve*> GetLevelDifCurvesByCZone(int idx);

	void GetPolyBoundary(double dSLoc, double dELoc, GM2DPolyline* pPoly);
	vector<GM2DPolyline*> GetLineLevelDifPolyLineArr();

	
	bool IsPointOnLRLines(GM2DVector Pt, bool bCheckDeg = false, double dDeg = 0);
	bool IsPointOnLLine(GM2DVector Pt, bool bCheckDeg = false, double dDeg = 0);
	bool IsPointOnRLine(GM2DVector Pt, bool bCheckDeg = false, double dDeg = 0);
	long WhichSide( GM2DVector vec );
	void GetOppositeRect( GM2DVector vec, GM2DPolyline& OppositeRect );
	virtual void RemoveOutwardWedge();
	virtual void RemoveInwardWedge();
	virtual void UpdateBoundaryVisibilities(long nUpdateConnIdx = 0);
	
	// 0: 무교차, 1: 1점 교차 2: 평행
	long CheckIntersectLinkMember(MSLinkMemberG* pUrLinkG, GM2DVector& CrossVec, double& dMyLoc, double& dUrLoc, double& dMyLocTol, double& dUrLocTol);

	bool CheckExtendOverlap(MSCompoG* pUrCompoG, bool isUpdateGeometry = true);

	virtual vector<MSCompoG*> FindConnectMember(FindType_MSElement memberType, long nFloorType, double dTol = DTOL_GM, bool bUseTrimedBoundry = true, bool bFindPolyMemberOnlyOnLine = false, bool bIncludeOnLine = true);
	virtual vector<MSCompoG*> FindConnectMember(CON_POS eConPos, FindType_MSElement memberType, long nFloorType, bool isReverse = false, double dTol = DTOL_GM, bool bUseTrimedBoundry = true, bool bFindPolyMemberOnlyOnLine = false, bool bIncludeOnLine = true);
	vector<MSCompoG*> FindColinerLinkMember(CON_POS eConPos, FindType_MSElement memberType, long nFloorType, double dTolDeg, double DistTol, double LinkTestLength, bool isReverse = false, bool bUseTrimedBoundry = true);
	vector<MSCompoG*> FindOverlapLinkMember(FindType_MSElement memberType, long nFloorType, double dTolDeg, double DistTol, double LinkTestLength, bool bCheckMemberWidth = false, bool bUseTrimedBoundry = true, bool bIncludeOnLine = true);

	virtual vector<MSCompoG*> FindConnectMember(char cLoc, FindType_MSElement memberType, long nFloorType, bool isReverse = false, double dTol = DTOL_GM, bool bUseTrimedBoundry = true, bool bFindPolyMemberOnlyOnLine = false);
	bool IsConnectedLinkMemberGs(vector<MSLinkMemberG*> urLinkGList);
	bool IsConnectedMember(MSLinkMemberG* pUrLinkG);
	vector<MSCompoG*> FindColinerLinkMember(char cLoc, FindType_MSElement memberType, long nFloorType, double dTolDeg, double DistTol, double LinkTestLength, bool isReverse = false, bool bUseTrimedBoundry = true);

	virtual void GetLRLines4Rebar(shared_ptr<GM2DCurve>& pLCurve, shared_ptr<GM2DCurve>& pRCurve, double dCutMinLen = DefHitDist);
	double GetExtendLenbyOffset(GM2DCurve* pCurve);
	void GetTrimmedLRLines(shared_ptr<GM2DCurve>& pLCurve, shared_ptr<GM2DCurve>& pRCurve, bool bUseVisibility = true, bool bMakeRebarLine = false, double dMinLen = DefHitDist);

	double GetCenCurveLocbyConntion(bool isStart, bool isMaxLoc);

	vector<MSCompoG*> MakeSplitLinkMemberByColumn(double dMinSplitLen = DTOL_MODELING);

	long GetZLevelInfo(MSCompoG* pUrCompoG, MSConnection* pConnection, bool bCheckConnection = true);
protected:
	void UpdateBoundaryVisibilities(MSCompoG* pUrCompoG, MSConnection* pConnection);
	
	double CalcCenCurveLocbyConntion(bool isStart, bool isMaxLoc);
	

	// By kkw 20141201
	
	// 절대위치(ma_Vec) 간 중심선. 직선부재 NULL. 아크일 경우 GM2DArc 객체
	GM2DCurve* mp_Curve;
	eLINEALIGN_TYPE m_eLineAlign;
	// By kkw 20150403

public:
	bool IsHorizontalityBarDirX();
	
	virtual double GetMemberDegree();
	//270~90 사이이면 false 91~269 사이이면 true
	void ReverseLevelDif();
	void MakeSquareEnd(MSLinkMemberG* link_other);

	GM2DPolyline MakeBoundaryFrom2Lines(GM2DLineVector* insideLine, GM2DLineVector* outsideLine);

	void ReplaceNearEndPoint(GM2DLineVector* insideLine, GM2DVector &insideEndPnt);

	void MakeCutEnd(MSLinkMemberG* link_other);


	virtual bool HasOverLapWall(){return false;};
	bool IsSameGeometry(MSCompoG* urCompoG, double distTolerance, double degreeTolerance) override;

	double GetWidthSetback();
	void SetWidthSetback(double thickSetback);

	virtual bool UsedMaterial(long materialID) override;
	void SubMemberMaterialValidCheck();

	virtual CString ToString();

	virtual void FixCompoGInfoAfterSerialize() override;
};

