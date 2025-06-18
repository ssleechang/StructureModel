#pragma once
#include "msobject.h"

// #include "MSLineMemberG.h"

#include "GMLib/GM2DVector.h"
#include "GMLib/GM2DPolygon.h"
#include "GMLib/GM2DPolyline.h"

//#include "MSElement.h"

class MSLineMemberG;
class MSBeamG;
class MSColumnG;
class MSIsoFootG;
class MSLinkMemberG;
class MSWallMemberG;
class MSGridLine;
class MSSlabG;
class MSGenStairG;
class MSElement;
class MSLinkMemPart;
class MSFloorQT;
class MSPolySection;
class MSAssembly;
class MSJointMemberG;
class MSCompoG;
class MSConnLevelZone;
class MSFloor;
class MSLineLevelDifG;
class MSLevelZone;
class MSCompoM;

class StructureModel_CLASS MSConnectivity : public MSObject
{
public:
	MSConnectivity();
	virtual ~MSConnectivity();
	void Serialize( CArchive& archive );
	void RecoverPointerAfterSerialize(MSFloor* floor);
	static void Serialize_AsID(CArchive& ar, MSConnectivity*& con);
	DECLARE_SERIAL(MSConnectivity)
	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSConnectivity* pSource, bool bNewID=true);
    virtual void Init(bool bInConstructor=false);
	static bool JOINT_SERIALIZE();

	MSConnectivity* mp_Source;
	vector<MSConnectivity*> ma_Clone;

    void Delete();
	bool IsDeleted();
	static void ClearAllDeletedJoint();

	bool CheckProfile();

	long m_ID;
	virtual long GetID();  // m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
	static long ms_LastID;
	static void SetLastID(long NewID); // NewID > ms_LastID 이면 치환. 아니면 무시

	long m_nFloorNum;
    MSFloor* mp_Floor;
	double m_dXc, m_dYc;

	static double ms_OverlapDistTol;

    void UnlinkUpdownFloorJoint();
    MSConnectivity* mp_UpFloorJoint;
    MSConnectivity* mp_DownFloorJoint;
	
	// 평면 및 Wall 부재의 경우 1개의 Floor에서 MSConnectivity를 나누어 관리 하기로함
	vector<MSConnectivity*> ma_UpLinkJoint;
	vector<MSConnectivity*> ma_DownLinkJoint;
	vector<long> ma_UpLinkJoint_Idx; // For Serialize Only
	vector<long> ma_DownLinkJoint_Idx; // For Serialize Only

	bool HasUpLinkJoint(MSConnectivity* pJoint);
	bool HasDownLinkJoint(MSConnectivity* pJoint);

// 	MSConnectivity* mp_UpLinkJoint;
// 	MSConnectivity* mp_DownLinkJoint;

	void AddFloorQT(MSFloorQT* pQT);
	CTypedPtrArray<CObArray, MSFloorQT*> ma_CrossQT;

    CTypedPtrArray<CObArray, MSConnLevelZone*> ma_LevelZone;
    CTypedPtrArray<CObArray, MSConnLevelZone*> ma_LevelZoneFloor;
    void BuildLevelZone();
    void DeleteLevelZone();
//     void DeleteLevelZoneFloor();

	vector<MSLinkMemPart*> ma_MemPart;
	vector<double> ma_MemPartDegree;
	vector<long> ma_MemPart_Idx; // For Serialize Only

	vector<MSLinkMemberG*> ma_SptMember;
	vector<double> ma_Degree;

    CTypedPtrArray<CObArray, MSLinkMemberG*> ma_BuriedLinkMember;

	MSColumnG* mp_Column;
	MSIsoFootG* mp_IsoFoot;
	vector<MSLinkMemberG*> ma_MidJointMember;//by LSS  MidJoint처리를 위해
	vector<MSSlabG*> ma_SlabMember;
	vector<MSGenStairG*> ma_StairMember;

	MSJointMemberG* mp_JointMember;
    //Column, Iso 있을시 사용 / 현재는 사용안함
	//Serialize에서 사용 
	GM2DPolyline* mp_DefaultSection;

	operator GMVector();
	operator GM2DVector();
	MSConnectivity operator=(const MSConnectivity &);		// = 연산자 오버로딩
	MSConnectivity operator=(const GM2DVector &);		// = 연산자 오버로딩

    bool HasNoVerticalMemPart();
    bool HasNoFloorMemPart();
    bool HasOneLineMember();
    bool HasOneVerticalSupportMember();
    bool HasOneWallMember();
	bool HasHorizontalMemPart();
    bool HasOnlyOneMemPart();
	bool IsConnected(MSConnectivity* OtherConnectivity);// this, uplink, downlink 를 모두 other과 비교해서 하나라도 같으면 true;

	//20140320 Add start by kkw
	bool HasOnlyFloorMember(); // 조인트에 Floor부재만 있을 경우(라인부재 포함)
	bool HasOnlyVerticalMember(); // 조인트에 Vertical 부재만 있을 경우(라인부재 포함)
	
	bool HasOnlyFloorLineMember(); // 조인트에 Floor라인 부재만 있을 경우
	bool HasOnlyVerticalLineMember(); // 조인트에 Vertical라인 부재만 있을 경우

	bool HasArcMember();
	bool HasBaseSec();
	bool HasHandRail();
	bool HasCompoG();

	bool IsConnectedBelowColumn(MSColumnG* pUpColumn);
	bool IsConnectedFMember(double dZc, double dDepth);
	bool IsConnectedVMember(double dZc, double dDepth);

	bool IsConnectedVMemberInDownLinkJoint(double dZc, double dDepth);

	bool HasColumn();
	bool HasIsoFoot();
	bool HasFloorLineMember();
	bool HasWallMember();
	bool HasMemberType(int eType);

	bool HasMidJointBelow(); // 벽 부재로 인한 미드 조인트가 있는지
	bool HasMidJointOn(); // 평면 부재로 인한 미드 조인트가 있는지

	bool IsExist_VerticalMember();
	bool IsExist_HorizontalMember();

	bool HasMSType(int msType);
	//20140320 End by kkw

	bool HasWallFootGPartCnt(long NumWall);
	long GetMaxThickWallFootGID();
	double GetMaxWidthByLineMemberG();

	bool IsNullJoint(); // 조인트에 부재가 있는지 체크
	bool IsMidConn();
	long AddSptMember(long StartOrEnd, MSLinkMemberG* AMem);
	bool AddSptMember(MSCompoG* pMem);

	virtual BOOL VisibleInRect( GM2DRectangle* pRect );
	long HitTest( MSCompoG* pCompoG, long StartOrEnd );
	long HitTest( MSConnectivity* pJoint );
	long HitTest( GM2DLineVector& EndLVec );
	long HitTest( GM2DVector& PosVec );
    bool HitTestAsMidJoint( MSLinkMemberG* pLink);
    
	void MakeDefSection();
	GM2DPolyline* GetSection_Hit();
    GM2DPolyline* GetTopSection();
    GM2DPolyline* GetBottomSection();
	GM2DPolyline* GetDrawSection(bool current);
	
	void GetMergedSection(GM2DPolyline &Poly);
	void GetMergedSectionAll(GM2DPolyline &Poly);
	vector<long> GetLinkJointID();

	MSLinkMemPart* FindMemPart(MSConnectivity* pOtherJoint);
	MSLinkMemPart* FindMemPart(double MyDegree);
	long AddPartMember(long StartOrEnd, MSLinkMemPart* pPart, bool bIsMidJont=true); 
	void RemoveMemPart( MSLinkMemPart* pRemoved);
	void RemoveSptMembers();
	long MergeTo( MSConnectivity* Target, bool bDoTrim=true );
	void RemoveSptMember( MSCompoG* pRemove );
	void TrimMembersEx( bool bResetMemPartLines = false, MSLevelZone* pMyLevelZone = NULL, bool bLinkJointTrim = true );

	void MakeMemPartEndJointPoly();

	long MergeToMeAndDelete( MSAssembly* pOwner, MSConnectivity* DeletedJoint, bool bDoTrim=true );
	long LevelOverlap(MSLinkMemberG* pMyLink, MSLinkMemberG* pUrLink);

	MSLinkMemberG* IsLevelOverlapOfWithSameTypeMember(MSLinkMemberG* pLinkG);

    long CutLineWhenShortening(MSLinkMemPart* pMyMember, char MyLineSide, GM2DCurve* pMyLine, long MyEndNum, GM2DVector MyCrossVec, 
                               MSLinkMemPart* pUrMember, char UrLineSide, GM2DCurve* pUrLine, long UrEndNum, GM2DVector UrCrossVec);
    void GetZLevels( double& TopLevel, double& BotLevel);
    void BuildSectionZone(MSLevelZone* pMyLevelZone = NULL);

    bool IsSameOrUpDown(MSConnectivity* pConn);

    void ResetVerticalMemPartLines();
    void ResetMemPartLines();
    MSLinkMemPart* GetNextMember( MSLinkMemPart* MyMem, char Side, MSConnectivity** NextJoint, bool CheckMinDeg);
    double DistanceOnXY( MSConnectivity* pJoint );
    double DistanceOnXY( GM2DVector* pJoint );

    //jissi - check for drawing at current floor
    bool NeedToDraw();
    bool IsWallEndCap();

	bool IsHasOnlyCompoG(MSCompoG* pCompoG);
	MSLevelZone* GetLevelZone();
	MSConnectivity* GetCloneJoint(MSFloor* pFloor);
	map<long, MSCompoG*> GetAllCompoGMap();

	void CheckMemPart();
	// kkw Add 20140317 MemPart 중 가장 낮은 BotLevel 얻기
	bool GetMemPartMinBotLevel(double &dMinBotLevel);

	long GetNumMemberG();
	long GetNumMemPart();
	MSConnectivity* AddUpLinkJoint(MSConnectivity* pJoint);
	MSConnectivity* AddDownLinkJoint(MSConnectivity* pJoint);

	void RemoveUpLinkJointsByMe(MSConnectivity* pJoint);
	void RemoveDownLinkJointsByMe(MSConnectivity* pJoint);

	bool RemoveUpLinkJoint(MSConnectivity* pJoint);
	bool RemoveDownLinkJoint(MSConnectivity* pJoint);
	void ResetXYbySection();

	MSConnLevelZone* GetTopConLevelZone();
	MSConnLevelZone* GetBotConLevelZone();
	MSConnLevelZone* GetConLevelZoneByZC(double dZc);

	vector<MSConnectivity*> ma_UppperFloorHitVerJoit;
	vector<MSConnectivity*> GetUppperFloorHitVerticalJoint();

	MSConnectivity* GetLowerFloorHitFloorJoint();
	void LowFloorJointHandRailTrim();
	
	bool Contains(MSCompoG* pUrCompoG, double dTol = DTOL_GM, bool bIncludeOnLine = TRUE );

	MSCompoG* GetFirstCompoG();

	int GetOverlapZoneID();
	vector<GM2DPolyline*> JointPolybyLevelZone();

	MSLinkMemberG* GetColinerLinkMember(MSLinkMemberG* pMyLink, double dTolDeg);

	vector<MSCompoG*> GetLinkMemberG(long eGetType);

	
	// DownLinkJoint포함하여 가져옴
	MSColumnG* GetColumn();

	bool isNotContainColumnColinerLinkMember();

	// 기존 값에 ZLevel 값을 더한다.
	void UpdateZLevel(double dChangZLevel, bool bUpdateTop = true, bool bUpdateBot = true);
protected:
	static map<MSConnectivity*, bool> _deleted_map;
	bool m_bDeleted;
};
