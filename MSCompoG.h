#pragma once
#include "MSElement.h"
#include "GMLib/GM2DPolygon.h"
#include "GMLib/GM2DPolyline.h"
#include "VolumeAreaInfo.h"
#include "VolumeAreaInfoEMS.h"
#include "VolumeAreaInfoEMS_NewEMS.h"
#include "RebarQuantityUser.h"

#include "StructureModelEnum.h"
#include "RebarQuantity.h"
#include "MSCheckModelOption.h"
#include "BaseRebarTemplate.h"
#include "RebarTemplateManager.h"
#include "MSConnection.h"
#include "StructureModel/FormWorkTemplate.h"
#include "FormWorkTemplateManager.h"
#include "MSBuildingInfo.h"
#include "ViewDisplaySetting.h"

#include "MSSteelGradeManager.h"
class MSCompoM;
class MSFloor;

class MSCompoM;
class MSFloorQT;
class MSOpening;
class MSContainer;
class MSAssembly;
class MDMember;
class MSMaterial;
class XErrorObject;
class MSConstructionZone;

class MSConnection;
class MSSubHaunchG;
class MSAddRebarBase;

typedef struct _TopBotLevelStruct
{
public:
	_TopBotLevelStruct()
	{
		Initialize();
	};
	_TopBotLevelStruct(double dTopLevel, double dBotLevel)
	{
		m_dTopLevel = dTopLevel;
		m_dBotLevel = dBotLevel;
	};
	_TopBotLevelStruct(const _TopBotLevelStruct& aData)	{*this = aData;};

	_TopBotLevelStruct& operator = (const _TopBotLevelStruct& aData)
	{
		Initialize();
		m_dTopLevel = aData.m_dTopLevel;
		m_dBotLevel = aData.m_dBotLevel;
		return *this;
	};
	void Initialize()
	{
		m_dTopLevel = 0;
		m_dBotLevel = 0;
	};

public:
	double m_dTopLevel;
	double m_dBotLevel;
}TopBotLevelStruct;

class StructureModel_CLASS MSCompoG :
	public MSElement
{

/* Formwork 주부재에서 사용하는 시공단계 - hhlee*/
public:
	eConstructionPhase GetConstructionPhase();
	void SetConstructionPhase(eConstructionPhase constructionPhase);

protected:
	eConstructionPhase m_eConstructionPhase;

public:
	
	int GetTrimmedBoundaryVisibility(int idx);
	void SetTrimmedBoundaryVisibility(int idx, int bVal);

	vector<int> GetTrimmedBoundaryVisibilityVec();
	void SetTrimmedBoundaryVisibilityVec(vector<int>& vec);
protected:
	//vector<bool> 사용 금지 : ar << ma_TrimmedBoundaryVisible 연산에서 오류 발생
	// 0: 안그림, 1 실선, 2 점선
	vector<int> ma_TrimmedBoundaryVisible;

public:
	void RecoverConnectionInCompoGCopyNPaste(map<long, MSCompoG*> OldCompoGMap);
	virtual void RecoverPointerAfterSerialize_SubMembers() {};
	virtual void InitTrimmedBoundaryVisibility();

	//나한태 묻힌 부재의ID 지금은 슬래브에서만 사용
	vector<int> ma_ContainedMemberID;
	void ClearContainMemberID();
	void SortContainMemberID();

	bool m_DoUpdateTrimmedProfile;
	vector<MSConnection*> ma_Connections;
	void AddConnectionWithMember(MSCompoG* pMember);
	void AddConnectionWithMember_NotExistOnly(MSCompoG* pMember);
	MSConnection* GetConnection(MSCompoG* pMember);
	virtual void RemoveConnections(bool bNotRemoveSetByUser = false);
	
	//CopyConnection, RecoverCopyConnectionCompoGID 함께 사용해야함
	static CString GetKeybyIDtoFloorNum(int ID, int FloorNum);
	static void CopyConnection(MSCompoG* pSourceG, MSCompoG* pTargetG, map<CString, MSCompoG*>& NewCompoGMap);
	static void RecoveryCopyConnectionCompoGID(MSCompoG* pCompoG, map<CString, MSCompoG*>& NewCompoGMap, map<CString, MSConnection*>& RecoveryConnectionMap);
	
	virtual void DeleteConnections();
	virtual void RemoveConnection(MSConnection* pConnection);
	void AddConnection(MSConnection* pConnection);
	void CheckZLevelConnection();

	virtual vector<MSCompoG*> GetPartnerCompoGs(long GetIdx = 0);
	virtual void InitTrimmedBoundary(bool IsTopLevel = true);
	void DeleteTrimmedBoundary();
	virtual void UpdateTrimmedBoundary(long nUpdateConnIdx = 0);
	virtual void GetTrimmedBoundaryZLevel(GM2DPolyline& APoly, double dTopLevel, double dBotLevel);
	virtual void SetTrimmedLines();

	// 입력시 생성된 프로파일
	virtual GM2DPolyline* GetProfile();

	// 트림된 프로파일
	virtual GM2DPolyline* GetTrimmedBoundary(bool IsTop = true);
	void SetTrimmedBoundary(GM2DPolyline* pPoly);

	// 트림된 프로파일 선위의점 삭제된 프로파일
	virtual GM2DPolyline* GetTrimmedBoundary_Acis();
	void SetTrimmedBoundary_Acis(GM2DPolyline* pPoly);

	void DeleteTrimmedBoundary_Acis();

	void TrimMerge(long nUpdateConnIdx = 0);

	void TrimIntersect(long nUpdateConnIdx = 0);

	void TrimInitialize(long nUpdateConnIdx = 0);
	virtual void UpdateTrimmedBoundary_AfterCalculate(CON_POS ePos = CON_ALL) { return; };


	virtual void BuildBoundaryByLevel(vector<GM2DPolyline>& BoundaryByLevelArr);
	virtual void MergeIntersectToBoundary(MSConnection* pConnection);
	virtual void ApplyMiterOptionToBoundary(MSConnection* pConnection);
	virtual void ApplySquareOffOptionToBoundary(MSConnection* pConnection);
	virtual void RemoveInwardWedge(){};
	virtual void RemoveOutwardWedge(){};
	virtual void UpdateBoundaryVisibilities(long nUpdateConnIdx = 0);
	void UpdateBoundaryVisibilities_After();
	virtual void SetUserBoundary(GM2DPolyline* pUserBoundary);
	virtual GM2DPolyline* GetUserBoundary(){ return mp_UserBoundary; }
	void UnSetUserBoundary();
	bool IsUserBoundarySet(){ return m_IsUserBoundarySet; }
	void SetIsUserBoundarySet(bool isUserBoundarySet) { m_IsUserBoundarySet = isUserBoundarySet; }

	bool HasTrimOption(int nOpt, Type_MSCompoG ePartnerType = MSElement::msNULL);
	//  CompoG가 가지고 있는 트림 옵션에 의한 숫자
	int GetTrimOrder();
	//cPos 'S'=Start 'E'=End 'M'=mid 'A'=All
	//nFloorType==-1:Dow 0:Current 1:Upper	
	
	vector<MSConnection*> GetConnectionByPos(char cLoc);
	vector<MSConnection*> GetConnectionByPos(CON_POS eConPos, bool checkSameFloor = true);

	void CalculateConnectionsAngle();
	void FixCalculateConnectionsAngle();
	void CalculateLinkMemberTrimOption();
	bool IsSetbyUserConnection(CON_POS eConPos);

	//cPos에 Mid는 올수없음.

	//memberType이 msNULL이면 모든 부재 리턴

	static void SortByPriorityNum(vector<MSCompoG*>& CompoGArr);
protected:
	virtual void CalculateTrimmedBoundary_Acis();
	vector<MSFloor*> ma_OverlapFloor;
	
	// 트림된 부재의 프로파일
	GM2DPolyline* mp_TrimmedBoundary;

	// 선위에 점이 없는 트림된 부재의 프로파일
	GM2DPolyline* mp_TrimmedBoundary_Acis;

	// Tool에서 사용하는 임시 변수, 프로퍼티 변경시에도 사용
	bool m_IsTempCompoG;
protected:
	// 부재의 입력된 정보
	GM2DPolyline* mp_Boundary;
public:
	void ClearOverlapFloor();
	void SetOverlapFloor();
	vector<MSFloor*> GetOverlapFloor();
	virtual vector<MSCompoG*> FindConnectMember(FindType_MSElement memberType, long nFloorType, double dTol = DTOL_GM, bool bUseTrimedBoundry = true, bool bFindPolyMemberOnlyOnLine = false, bool bIncludeOnLine = true);



public:
	MSCompoG();
	~MSCompoG(void);
	void Serialize( CArchive& archive );
protected:
	void Serialize_EachCompoM(CArchive& ar);
	virtual void Serialize_EachCompoM_Rebar(CArchive& ar);
	virtual void Serialize_EachCompoM_Formwork(CArchive& ar);
public:
	bool CheckValidConnections(bool bRemove = true);
	virtual void RecoverPointerAfterSerialize(MSFloor* floor);
	virtual void RecoverConnectionAfterCopyNPaste(map<long, MSCompoG*>& OldCompoGMap);
	DECLARE_SERIAL(MSCompoG)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual void Init(bool bInConstructor=false);
	virtual bool IsInvalidVecInput(){ return false; };

	virtual GM2DVector* InsertVec(long Index, GM2DVector* pVec);
    virtual void ReplaceVec( long Index, GM2DVector* pVec);
	virtual GM2DVector* GetVec(long Index);
	virtual GM2DVector* GetSVec();
	virtual GM2DVector* GetEVec();
	
	GM2DVector* GetSVecInput();
	GM2DVector* GetEVecInput();

	virtual void UpdateVecInput(){return;};
	virtual void UpdateVecCenter(){return;};

	virtual bool CheckHeight(bool IsUseGroupName = false);
	double GetLevelZoneLevel(double dX,double dY);

	void RemoveContainedMemberID(long nCompoGID);
	virtual void InitializeParameter();

	virtual bool IsContainSlabinLowFloor();

	virtual bool IsSlopeMember(){return false;};
	virtual bool HasLevelDif(){return false;};
	virtual bool HasHaunch() { return false; };
	virtual void MakeCCW();
	virtual bool HasSubMember();
	virtual void DeleteSubMembers();

	bool IsEachCompoM();
	virtual bool IsCustomM() override;
	bool IsPolygonSection();
	bool IsCustomColumn();
	bool IsCustomWallColumn();
	bool IsCustomBeam();
	bool IsCustomFWBeam();

	MSCompoM* mp_CompoM;
	MSContainer* mp_Owner; 
	MSCompoG* mp_Source;

	XErrorObject * mp_MsgObject;

	vector<MSCompoG*> ma_Clone;
	vector<GM2DVector*> ma_Vec;
	vector<GM2DVector*> ma_Vec_Input;

	vector<VolumeAreaInfo> ma_VolumeInfo; // mylee //솔리드 콘크리트,거푸집 -> 버림 + 유저물량
	vector<VolumeAreaInfo> ma_AreaInfo; // mylee // 유저물량
	vector<VolumeAreaInfo> ma_UserFormWork;//ems 거푸집
	vector<RebarQuantityUser> ma_UserRebar;
	bool m_IsQuantityReplaced;

	void AddUserConcreteQuantity(vector<VolumeAreaInfo>& userQuanityArr);
	void AddUserAreaQuantity(vector<VolumeAreaInfo>& userQuanityArr);
	void AddUserRebarQuantity(vector<RebarQuantityUser>& userQuanityArr);
	void ClearUserQuantityData();
	void ClearQuantityData();
	bool HasUserQuantityData();

	VolumeAreaInfoEMS m_VolumeAreaEMS; //ems 콘크리트,거푸집 기본 정보
	VolumeAreaInfoEMS_NewEMS m_VolumeAreaEMS_NewEMS; //ems 콘크리트,거푸집 기본 정보
	RebarQuantityInfo m_RebarQuantityInfo;//철근물량



	void ClearAllQuantityData();

	vector<int> ma_CZoneIDFromQuantityCalc;		// 솔리드 물량계산시 사용되던 변수. 2020.04.02기준 지금은 m_VolumeAreaEMS.ma_CZoneID 변수를 사용한다.
	vector<MSConstructionZone*> GetCZones();
	MSConstructionZone* GetCZoneByID(int cZoneID);
	void SetCZoneIDToUserQuantity(int calcType);
	void SetCZoneIDToUserQuantity(bool isUpperCzoneNeeded, int calcType);
	bool IsLowWallTopQuantitySeperate();
	vector<int> GetCZoneIDs();
	int GetFirstCzoneIDInCompoGOnFloor(MSFloor* floor);
	bool GetVolume(double& value, CString& note);
	bool GetBurimVolume(double& value, CString& note);
	bool GetFormWorkArea(vector<CString>& arFWAreaType, vector<double>& arFWArea, vector<CString>& arFormWorkNote);
	//bool GetFormWorkArea(double& value, CString& note, double& value_out, CString& note_out);
	bool GetVolumeByCZone(int czone_id, double &rOriginValue, double& value, CString& note, int calcType, vector<CString>& userCategory, vector<CString>& userMaterial, vector<CString>& userNote, vector<double>& userValue, double &srcColInnerVolume, CString& srcColInnerNoteVolume, vector<double>& addedConcVolumes, vector<CString>& addedConcNotes, vector<long>& addedMatIds, vector<long>& subIds, vector<double>& subVolumes, vector<CString>& subNotes, vector<int>&matid, double rebarVolume = 0);
	bool GetBurimVolumeByCZone(int czone_id, double& value, CString& note);
	bool GetHaunchVolumeByCZone(int czone_id, vector<int>& matl_id_list, vector<double>& volume_list, vector<CString>& noteList);
	bool GetHaunchQuantityByCZone(int czone_id, vector<int>& matl_id_list, vector<double>& volume_list, vector<CString>& noteList,
		vector<CString>& haunch_arFWAreaType, vector<double>& haunch_arFWArea, vector<CString>& haunch_arFormWorkNote, vector<int>& haunchID, vector<int>& fwHaunchID, vector<double>& haunch_arFWOriginArea, vector<int>& haunch_matbool);
	bool GetBuiltUpBeamVolumeByCzone(int czone_id, vector<int>& matl_id_list, vector<double>& volume_list, vector<CString>& noteList);
	bool GetEgeWallVolumeByCzone(int czone_id, vector<int>& matl_id_list, vector<double>& volume_list, vector<CString>& noteList);
	bool GetFormWorkAreaByCZone(int czone_id, vector<CString>& arFWAreaDir, vector<CString>& arFWAreaType, vector<double>& arFWArea, vector<CString>& arFormWorkNote, vector<CString>& arFormWorkCategory,
		vector<CString>& arFWAreaDir_burim, vector<CString>& arFWAreaType_burim, vector<double>& arFWArea_burim, vector<CString>& arFormWorkNote_burim, int calcType, vector<CString>& arAddedFWAreaDir, vector<CString>& arAddedFWAreaType, vector<double>& arAddedFWArea, vector<CString>& arAddedFormWorkNote, vector<long>& arFWAreaID, vector<double>& arFWOriginArea);

	void AddAreaUserInputs(int czone_id, CString noneTypeName, vector<CString> &arFWAreaDir, vector<CString> &arFWAreaType, vector<double> &arFWArea, vector<CString> &arFormWorkNote, vector<CString> &arFWCategory, int calcType);

	bool GetFormWorkAreaEMSWithoutCZone(vector<CString>& arFWAreaDir, vector<CString>& arFWAreaType, vector<double>& arFWArea, vector<CString>& arFormWorkNote, vector<CString>& arFormWorkCategory, vector<CString>& arFWAreaDir_burim, vector<CString>& arFWAreaType_burim, vector<double>& arFWArea_burim, vector<CString>& arFormWorkNote_burim, int calcType, 
		vector<CString>& arAddedFWAreaDir, vector<CString>& arAddedFWAreaType, vector<double>& arAddedFWArea, vector<CString>& arAddedFormWorkNote);
	bool GetBurimFormWorkAreaByCZone(int czone_id, vector<CString>& arFWAreaDir_burim, vector<CString>& arFWAreaType_burim, vector<double>& arFWArea_burim, vector<CString>& arFormWorkNote_burim);
	static int NOTE_VERIFY;
	static void SetUseEMS(bool bUseEMS);
	static bool GetUseEMS();

	// add - by kkw
	bool Overlap(MSCompoG* pUrCompoG, double dTol = DTOL_GM, bool bIncludeOnLine = TRUE );
	bool Contains(MSCompoG* pUrCompoG, double dTol = DTOL_GM, bool bIncludeOnLine = TRUE, bool isCheckZLevel = true);
	// add - by gracias(2014.01.07)
	long m_MarkGID;

	virtual long GetPriorityNum() { return m_nCmdCnt; };
	virtual void SetPriorityNum(int Num) { return; };
	long m_nCmdCnt;

	vector<long> ma_BarSetID;

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	virtual void ModifyObject(bool isDoit = true);

	void DeleteVectorArr();
	
	bool m_IsScheduleAssigned;
	
	/// Start Date. If not assigned, it is earliest among All CZones and Assigned Date
	CTime GetStartDate();
	void SetStartDate(const CTime&  startDate);
	
	/// Finish Date. If not assigned, it is latest among All CZones and Assigned Date
	CTime GetFinishDate();
	bool SetFinishDate(const CTime&  finishDate);
	
	bool CheckCompoM();


	void DeleteVecInput();
	virtual MSMaterial* GetMaterial();
	vector<long> GetMaterialIDs();
	virtual MSMaterial* GetInnerConcMaterial();

	bool GetIsUsedStoryMaterial();
	
	int m_nRebarTemplateID;
	BaseRebarTemplate* GetRebarTemplate();
	CString GetRebarTemplateName();
	void SetRebarTemplateName(CString sName);
	RT_MemberType GetCompoGTypeToMemberTemplateType();
	int GetRebarTemplateID();

	int m_nFormWorkTemplateID;
	FormWorkTemplate* GetFormWorkTemplate();
	CString GetFormWorkTemplateName();
	void SetFormWorkTemplateName(CString sName);
	int GetFormWorkTemplateID();

public:
	// mylee : 이 CompoG 에 해당하는 Acis Body 가 생성된 경우 태그값을 저장한다. 기본값은 -1
	long m_nEntityTag_Cur;
	long m_nEntityTag_Org;
	long m_nEntityTag_SrcSteel;
	long m_nEntityTag_PC;
	BOOL AddErrorObject(long nErrorCode, long nLevel, CString strGroup, CString strMsg);

public: 
	bool ModifyClones();
// 	virtual bool UnlinkFromJoint() {return false;};
	virtual bool SetCloneJoint(MSAssembly* pCloneFloor);
	virtual void SetOwner(MSContainer* val);
	virtual MSContainer* GetOwner(){return mp_Owner;};
	virtual MSCompoM* GetCompoM(){return mp_CompoM;};
	virtual MDMember* GetRebarM();
	virtual bool SetCompoM(long MID, bool bUpdateVecInput = true);
	virtual bool SetCompoM(MSCompoM* pCompoM, bool bUpdateVecInput = true);
	virtual bool UpdateWidth(double dPreWidth, double dCurWidth){return false;};

	virtual MSBaseBuilding* GetBuilding();
	virtual double GetStartLevel();
	virtual double GetRelativeStartLevel(){return 0;};
	virtual double GetEndLevel();
	virtual double GetRelativeEndLevel(){return 0;};
    CString GetName() {return m_Name;};
    void SetName(CString name) {m_Name = name;};

	virtual double GetWidth();
	virtual double GetDepth();
	virtual double GetThick();
	virtual double GetPcThick();
	virtual double GetPcWidth();
	virtual double GetPcDepth();
	virtual double GetHeight();
	virtual void GetSecPolyline(GM2DPolyline& APGon);
	virtual void GetHeaderPolyline(GM2DPolyline& APGon);
	virtual vector<GM2DPolyline> GetSubHeaderPolyline();
	virtual double GetSlabLevel();
	virtual double GetNextSlabLevel();
	virtual double GetCover();
	virtual double GetFloorLevel();
	virtual double GetNextFloorLevel();
	
	virtual double GetBotOffSet() { return 0; };
	virtual MSFloor* GetFloor();

	virtual inline void CalcBoundRectBeforeAddQuadTree(){return;};


	void GetTopBotLevelArr(MSCompoG* pUrCompoG, CheckModelOpt &CheckFlag, bool bCheckBigVal, bool bCheckLevelZone, vector<TopBotLevelStruct>& MyLevelArr, vector<TopBotLevelStruct>& UrLevelArr);
	bool IsLevelConnectedOverLap(MSCompoG* pUrCompoG, CheckModelOpt &CheckFlag, bool bCheckBigVal = false);
	bool IsLevelOverlap(MSCompoG* pUrCompoG, bool bCheckLevelZone = false);
	bool IsLevelContain(MSCompoG* pUrCompoG);
	bool IsLevelConnected(MSCompoG* pUrCompoG);
	bool IsLevelTouch(MSCompoG* pUrCompoG);
	bool IsLevelEquals(MSCompoG* pUrCompoG);

	virtual bool GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal = true, bool bCheckLevelZone = true ) {
		TopLevel = 0;
		BottomLevel = 0;
		return false;
	};

	virtual long GetZLevelInfo(MSCompoG* pUrCompoG){return 0;};

// 	virtual double GetOffSetbyPos(char cPos){return false;};
// 	virtual bool GetZLevelsbyPos(double& TopLevel, double& BottomLevel, char cPos){return false;};

	virtual bool VisibleInRect( GM2DRectangle* pRect) {return false;};
	//virtual BOOL VisibleInRect( GM2DRectangle* pRect);
	virtual bool MakeBoundary(){return false;};
	virtual void GetBoundary( GM2DPolyline& APolyLine );
	virtual void GetBoundaryNSubMember(GM2DPolyline& APolyLine);

	// 파일 오픈시 ma_Clone의 정보를 채워주기 위한 임시 변수
	map<long, long> tmep_CloneMap;
    CString m_Name;

	virtual void Translate(GM2DVector &pDelta);
	virtual void MirrorByAxis(GM2DLineVector& pAxis);
	virtual void RotateZ(double theta, GM2DVector *pCenter);
	virtual void TranslateAddRebars(GM2DVector &pDelta);
	virtual void MirrorByAxisAddRebars(GM2DLineVector& pAxis);
	virtual void RotateZAddRebars(double theta, GM2DVector *pCenter);
	virtual void AddHaunch(double x, double y, double x2, double y2, double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto) { return;};
	virtual void AddHaunchesOnProfile(double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto) { return; };
	virtual void DeleteHaunchs() { return; };

	void AddCompoGZLevelInFloor();
	void DelCompoGZLevelInFloor();

	map<long, MSFloor*> GetConnectionSlabGID();
	void SetConnectionSlabGID();
	void UpdateConnectionSlabInnerProFile();
	static void GetMyUrBoundary(MSCompoG* pMyCompoG, MSCompoG* pUrCompoG, GM2DPolyline* &pMyBoundary, GM2DPolyline* &pUrBoundary);

	bool HasTrimOption(MSConnection::TRIM_OPTION FindOpt, CON_POS CheckPos = CON_ALL);
	void CheckTrimOpt();

	vector<double> GetUserRebarWeights();
	vector<CString> GetUserRebarCalcultingFormulas();
	vector<CString> GetUserRebarDiaPrintNames();
	vector<CString> GetUserRebarDiaIndexs();

	virtual bool IsSameGeometry(MSCompoG* urCompoG, double distTolerance, double degreeTolerance) { return false; };

	void LoggingConnections();

	map<long, MSAddRebarBase*> mm_AddRebar;
	bool Add(MSAddRebarBase* pRebar);
	long Remove(MSAddRebarBase* pRebar);
	long Delete(MSAddRebarBase* pRebar);
	void DeleteAddRebars();

	virtual bool UsedMaterial(long materialID);
	void SubMemberMaterialValidCheck();

protected:
	CTime m_startDate;
	CTime m_finishDate;
	//bool m_IsDummy;
	bool m_IsRigidMember;
	bool m_IsReverseBox;
	GM2DVector m_LabelEcc;
	bool m_IsUserBoundarySet;
	GM2DPolyline* mp_UserBoundary;

	static bool _bUseEMS;

	//vector<long> ma_ConnectionSlabID;
	map<long, MSFloor*> mm_ConnectionSlabID;
public:
	
	bool GetRigidMember() { if(IsPC()) return true;
							else return m_IsRigidMember; };
	void SetRigidMember(bool value) { if(IsPC()) return;
									  else m_IsRigidMember = value; };
	bool GetReverseBox(){ return m_IsReverseBox;};
	void SetReverseBox(bool IsReverseBox){m_IsReverseBox = IsReverseBox;};
	GM2DVector GetLabelEcc() { return m_LabelEcc;};
	void SetLabelEcc(GM2DVector newEcc){m_LabelEcc = newEcc;};
	
	virtual double GetMemberDegree(){return 0;};
	virtual bool isXDirection();
	bool IsReverseDir(bool bApplyReverseBox = false);

	virtual bool IsRC();
	virtual bool IsSteel();
	virtual bool IsSRC();
	virtual bool IsReverseBeam();

	virtual long GetTopSpacerID() { return 0; };
	virtual long GetBotSpacerID() { return 0; };

	void RemoveSameConnection();

	virtual bool GetIsTempCompoG();
	void SetIsTempCompoG(bool value);

	void FormWorkDeleteByDir(CString dir);
private:
	int m_nBuildingInfoID;
public:
	int GetBuildingInfoID();
	void SetBuildingInfoID(int buildingInfoID);

	MSBuildingInfo* GetBuildingInfo();
	CString GetBuildingInfoClassification();
	CString GetBuildingInfoType();
	CString GetBuildingInfoName();
	double GetBuildingInfoOverGrossFloorArea();
	double GetBuildingInfoUnderGrossFloorArea();

private:
	ViewDisplaySetting* m_pViewDisplaySetting;
public:
	ViewDisplaySetting* GetViewDisplaySetting() { return m_pViewDisplaySetting; };
	virtual CString ToString();

	virtual CString GetClassificationName();
	MSSteelGradeManager::eSteelGrade GetSteelGrade(double dDia);
	CString GetSteelGradeName(double dDia, bool bUserSteelGradeS);
	
	long GetMainCompoGID();
	void SetMainCompoGID(long nMainCompoGID);
	MSCompoG* GetMainCompoG();

	vector<long> GetLinkedCompoGIDArr();
	vector<MSCompoG*> GetLinkedCompoGArr();
	
	virtual bool RemoveSubMember(MSElement* element) override;
	bool HasRightAngleConnection();
	void SortNCalcTrimOption();
	void SetByUserInConnections(bool value);

	virtual MSCompoM::Type_MSCompoM GetDefCompoMType() { return MSCompoM::Type_MSCompoM::msNONE; };

	bool FixGeometry();
	//acisobject에서 사용하는 TypeAttribute 3DView의 색상 맵핑에서 사용하는 Key값
	CString GetTypeAttribute();
private:
	long m_nMainCompoGID;

protected:
	bool m_IsUserFloor;
	int m_nUserFloorID;
public:
	bool GetIsUserFloor();
	void SetIsUserFloor(bool value);
	int GetUserFloorID();
	void SetUserFloorID(int nFloorID);
	MSFloor* GetUserFloor();
	void SetUserFloor(MSFloor* refFloor);
	MSFloor* GetDefUserFloor();

#pragma region 전이 부재
public:
	virtual bool IsTransfer() override;
#pragma endregion
	virtual bool IsDeck() override;

};

#pragma make_public(MSCompoG)