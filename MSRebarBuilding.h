#pragma once
#include "MSBaseBuilding.h"

class MSFloor;
class MSGroup;
class MSLinkMemberG;
class MSCompoG;
class MSAssembly;
class MSRevision;
class MSMarkM;
class MSMarkG;
class MSReinforceBar;
class MSReinforceBarSetting;
class MSBlock;
class MSCheckModelOption;
class MSColumnG;
class MSBeamG;
class MSColineLinkMemberGs;
class MSRibTypeM;
class MSSpacerShape;
class MSStoryMaterial;
class MSWallEndRebarLine;
class MSSteelUtility;
class MSCustomSection;
class MSBuildingUCS;
class MSBuildingInfo;
class RebarQuantityInfo;

#include "GMLib/GM2DRectangle.h"
#include "RebarGlobalOptionManager.h"
#include "MSQuantityOption.h"
#include "MSCheckModelOption.h"
#include "RebarQuantity.h"
#include "MSSteelGradeManager.h"
#include "BaseRebarTemplate.h"
#include "RebarTemplateManager.h"
#include "MSFormWorkTypeManager.h"
#include "MSCustomM.h"
#include "FormWorkTemplate.h"
#include "FormWorkTemplateManager.h"
#include "MSAreaTemplateManager.h"
#include "MSBuildingInfoManager.h"
#include "MSBaseBuilding.h"

class StructureModel_CLASS MSRebarBuilding : public MSBaseBuilding
{
public:
	MSRebarBuilding(void);
	~MSRebarBuilding(void);
	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSRebarBuilding)

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	virtual void Init(bool bInConstructor = false);

	static double ms_CMColinearDegreeTol;
	static double ms_CMColumnRatio;
	static double ms_CMWallRatio;
	static double ms_CMWallFootRatio;
	static double ms_CMSlabRatio;

	static void SetCMColinearDegreeTol(double dTol);
	static void SetCMColumnRatio(double dRatio);
	static void SetCMWallRatio(double dRatio);
	static void SetCMWallFootRatio(double dRatio);
	static void SetCMSlabRatio(double dRatio);

	map<long, MSBlock*> mm_Block;
	map<long, MSCompoM*> mm_CompoM;
	virtual map<long, MSCompoM*> GetCompoMMap() override { return mm_CompoM; };
	map<long, MSMaterial*> mm_Material;
	map<long, MSAssembly*> mm_Assembly;
	map<long, MSGridLine*> mm_GridLine;
	map<long, MSFloor*> mm_Floor;
	map<long, MSGroup*> mm_Group;
	// add MSMakM - by gracias(2014.01.07)
	map<long, MSMarkM*> mm_MarkM;
	// add MSMakG - by gracias(2014.03.19)
	map<long, MSMarkG*> mm_MarkG;
	map<long, MSRibTypeM*> mm_RibM;
	// add MSSpacerShape - by gracias(2014.11.03)
	map<long, MSSpacerShape*> mm_Spacer;
	map<int, MSStoryMaterial*> mm_StoryMaterial;
	map<int, MSWallEndRebarLine*> mm_WallEndRebarLine;

	virtual int GetNextTagNum(int type) override;
	virtual MSMarkM* GetMarkMbyErrorCode(int type, int errorCode) override;

	virtual MSRebarBuilding* GetBuilding() { return this; }

	virtual vector<MSCompoM*> GetCompoMList(long nType) override;
	virtual MSCompoM* GetCompoM(long CompoMID) override;
	virtual MSCompoM* GetCompoM(CString strName, long nType, long nStory = 0) override;
	//MSCompoM* FindCompoM(CString& strName, long nFloorNum);
	virtual bool ExistCompoM(CString& strName, long nFloorNum) override;

	virtual bool Add(MSCompoM* pCompoM) override;
	virtual bool Add(vector<MSCompoM*> CompoMArr) override;

	virtual long Remove(MSCompoM* pCompoM) override;
	virtual bool Remove(vector<MSCompoM*> CompoMArr) override;

	long Delete(MSCompoM* pCompoM);
	void DeleteCompoMs();

	long GetRowCompoMs(long nType, CMap<CString, LPCTSTR, XMSRowM, XMSRowM>& RowMMap);
	long GetRowCompoM(long nType, long nRowMID, XMSRowM& aRowM);
	virtual Type_MSCompoG GetType() { return msBuilding; };

	long GetRowCompoMs(long nType, map<CString, vector<MSCompoM*>>& CompoMMap);
	void DeleteCompoGs();
	virtual MSCompoG* GetCompoG(long CompoGID) override;
	virtual MSCompoG* GetCompoGByFloors(long CompoGID) override;

	virtual MSMaterial* CreateNewMaterial(long nType) override;
	void DeleteMaterials();
	virtual MSMaterial* GetMaterial(long MaterialID) override;
	virtual MSMaterial* GetMaterial(CString strMateName) override;
	virtual MSMaterial* GetDefMaterial() override;
	virtual MSMaterial* GetDefMaterial(MSMaterial::Type_MSMaterial materialType) override;
	virtual MSMaterial* GetMaterialbyName(MSMaterial::Type_MSMaterial materialType, CString name) override;
	virtual map<long, MSMaterial*>* GetMaterialMap() override;
	virtual bool Add(MSMaterial* pMaterial) override;
	virtual long Remove(MSMaterial* pMaterial) override;
	virtual long Delete(MSMaterial* pMaterial) override;
	MSMaterial* GetRcMaterialbyName(CString name);
	MSMaterial* GetSteelMaterialbyName(CString name);
	MSMaterial* GetSrcMaterialbyName(CString name);
	MSMaterial* GetMaterial(MSMaterial::Type_MSMaterial type, CString name);
	virtual vector<MSMaterial*> GetMaterialsByType(MSMaterial::Type_MSMaterial type) override;

	void DeleteGridLines();
	virtual MSGridLine* GetGLine(long GLineID) override;
	bool Add(MSGridLine* pGridLine);
	long Remove(MSGridLine* pGridLine);
	long Delete(MSGridLine* pGridLine);

	void DeleteAssemblys();
	MSAssembly * GetAssembly(long AssemblyID);
	bool Add(MSAssembly* pAssembly);
	long Remove(MSAssembly* pAssembly);
	long Delete(MSAssembly* pAssembly);

	virtual void DeleteMarkMs() override;
	virtual MSMarkM* GetMarkM(long MarkerID) override;
	virtual bool Add(MSMarkM* pMarker) override;
	virtual long Remove(MSMarkM* pMarker) override;
	virtual long Delete(MSMarkM* pMarker) override;
	virtual MSMarkM* GetMarkM(int type, CString comment) override;

	virtual void DeleteMarkGs() override;
	virtual MSMarkG* GetMarkG(long MarkerID) override;
	virtual bool Add(MSMarkG* pMarker) override;
	virtual long Remove(MSMarkG* pMarker) override;
	virtual long Delete(MSMarkG* pMarker) override;
	virtual vector<MSMarkG*> GetMarkGsfromMID(long MID) override;
	virtual map<long, MSMarkG*>* GetMarkGMap() override { return &mm_MarkG; };

	void DeleteBlocks();
	virtual MSBlock* GetBlock(long blockID) override;
	bool Add(MSBlock* pBlock);
	long Remove(MSBlock* pBlock);
	long Delete(MSBlock* pBlock);

	MSRibTypeM* CreateNewRibM(int RibType);
	void DeleteRibTypes();
	MSRibTypeM* GetRibM(long ID);
	void Add(MSRibTypeM* pRibTypeM);
	void Remove(MSRibTypeM* pRibTypeM);
	void Delete(MSRibTypeM* pRibTypeM);

	void DeleteSpacerShapes();
	virtual MSSpacerShape* GetSpacerShape(long ID) override;
	virtual long GetSpacerShape(CString name) override;
	void Add(MSSpacerShape* pSpacerShape);
	void Remove(MSSpacerShape* pSpacerShape);
	void Delete(MSSpacerShape* pSpacerShape);

	MSStoryMaterial* CreateNewStoryMaterial();
	void DeleteStoryMaterials();
	virtual MSStoryMaterial* GetStoryMaterialByID(long nID) override;
	virtual MSStoryMaterial* GetStoryMaterialByFloorNum(long floorNum) override;
	vector<MSStoryMaterial*> GetStoryMaterialsOrderbyFloorNum();

	long GetFloorNumForUserbyStoryMaterial(long nFloorNum, MSMaterial::Type_MSMaterial matType, bool isVerMember, bool bFindLowFloor);

	void Add(MSStoryMaterial* pStoryMaterial);
	void Remove(MSStoryMaterial* pStoryMaterial);
	void Delete(MSStoryMaterial* pStoryMaterial);
	void MakeDefaultStoryMaterials();

	virtual MSWallEndRebarLine* CreateNewWallEndRebarLine() override;
	virtual void DeleteWallEndRebarLines() override;
	virtual MSWallEndRebarLine* GetWallEndRebarLine(long ID) override;
	virtual bool Add(MSWallEndRebarLine* pWallEndRebarLine) override;
	virtual long Remove(MSWallEndRebarLine* pWallEndRebarLine) override;
	virtual long Delete(MSWallEndRebarLine* pWallEndRebarLine) override;
	MSWallEndRebarLine* FindSameWallEndRebarLine(MSWallEndRebarLine* urEndRebarLine);
	virtual vector<int> GetAllWallGsWidthRebarLineID(int rebarLineID) override;

	

	CString m_strFilePath;
	double  m_dGroundLevel;
protected:
	MSQuantityOption m_QuantityOption;
public:
	virtual MSQuantityOption* GetQuantityOption() { return &m_QuantityOption; };
	void InitQuantityOptionMaterial();

	bool IsSumSOGAsFooting() { return m_QuantityOption.m_bSumSOGAsFooting; };

	MSCheckModelOption* m_pCheckModelOption;
	RebarQuantityInfo m_rebarQuantityInfo;
	virtual RebarQuantityInfo* GetRebarQuantityInfo() override { return &m_rebarQuantityInfo; };
	void SetSiteRect();

	virtual bool WriteFileBinary() override;
	virtual bool WriteFileBinary(CString strFilePath) override;
	virtual bool ReadFileBinary(CString strFilePath) override;

	virtual void DeleteFloors() override;
	void DeleteGroups();
	bool DeleteGroup(long GroupNum);//by LSS NX
	bool DeleteGroup(CString GroupName);//by LSS NX

	MSFloor* AddNewFloor(long FloorNum);
	virtual void AddFloor(MSFloor* floor) override;
	virtual MSFloor* GetFloor(long FloorNum) override;
	virtual MSFloor* GetFloorByID(long FloorID) override;

	virtual long Remove(MSFloor* pFloor) override;
	virtual long Delete(MSFloor* pFloor) override;
	void ReMakeFloorMap(long deleteID = 0);
	bool IsBottomFloor(long FloorNum);
	bool IsRoofFloor(long FloorNum);

	MSConstructionZone* MSRebarBuilding::GetCZone(int ID);

	MSGroup* AddNewGroup(MSGroup* pGroup);
	MSGroup* GetGroup(long nGroupID);

	//2011.0406.덮어쓰기
	void GetCompoMFloorName(long nMID, CString& strFrom, CString& strTo);
	virtual CString GetFloorName(long nStroy);
	//
	virtual double GetGroundLevel() override { return m_dGroundLevel; };
	virtual void SetGroundLevel(double dGroundLevel) override;
	virtual double GetFloorLevel(long FloorNum) override;
	virtual double GetSlabLevel(long FloorNum) override;

	virtual vector<MSFloor*> GetFloorsByLevel(double dStartLevel, double dEndLevel) override;

	//void UpdateJointZc();
	double GetTotalLength();
	//void GetRebarTotalLengthByDia( map<double,double>& mapLengthByDia );
	virtual vector<long> GetCompoGs_InAllFloor(long nMID) override;
	BOOL IsDeleteCompoM(long nMID);
	bool IsUsedReinforceM(long reinforceMID);
	bool IsUsedColumnHeadM(long columnHeadMID);

	long ChangeCompoG_CompoMs(MSLinkMemberG* pLinkG, long nNewMID);
	// 수직부재 M정보 변경시 같은위치, 같은이름으로 되었는 MSCompoG들중 MID가 변경되어야 할 CompoGArr를 StoryMap으로 채워보낸다
	BOOL GetEditedCompoGs_byChangeMID(vector<MSCompoG*> CompoGArr, long nNewMID, long nMType, CMap<long, long, vector<MSCompoG*>, vector<MSCompoG*>>& StoryToCompoGArrMap);
	// 수직부재 M정보 변경시 같은위치, 같은이름으로 되었는 MSCompoG들중 변경될 MSCompoM이 해당층이 정의 안될때 삭제되어야 할 CompoG리턴
	BOOL GetDeletedCompoGs_byChangeMID(vector<MSCompoG*> CompoGArr, long nNewMID, long nMType, CMap<long, long, vector<MSCompoG*>, vector<MSCompoG*>>& StoryToCompoGArrMap);

	//by LSS beam/wall component의 volume및 formwork area계산을 위한 기본 준비/ 각 부재의 trim boundary를 만든다.
	//void MakeCompoBoundary();
	virtual long GetNumAboveStory() override;
	virtual long GetNumUnderStory() override;

	virtual vector<MSCompoG*> GetAllCompoGs() override;//by LSS NX
	bool bCalculationPrepared; //by LSS NX
	virtual vector<MSFloor*> GetAllFloors() override;
	virtual void ClearAllCashFloorLevel() override;

	// Floor삭제시 Roof층이 되는 MSFloor에 속해 있는 수직부재를 삭제한다.
	void RemoveRootFloor_VMembers();
	long MakePlaceStoryMap(map<long, vector<long>>& PlaceStoryMap);

	void Trim();
	map<long, MSCompoG*> GetAllCompoGs(long nFloorNum, long nGType);
	map<long, MSCompoG*> GetAllCompoGs(long nGType);
	map<long, MSCompoG*> GetAllCompoGs_SlabG(long nFloorNum, long nSlabType);
	map<long, MSCompoG*> GetAllCompoGs_BeamG(long nFloorNum, bool IsPC);
	virtual void GetAllCompoGsWithCompoM(MSCompoM* compoM, vector<MSCompoG*>& list) override;

	virtual vector<long> GetCompoMs(long nType) override;
	virtual vector<long> GetCompoMs() override;
	virtual vector<long> GetCompoGs(long nMID) override;
	virtual vector<MSCompoM*> GetAllColumnMsWidthColumnHeaderMID(long headerMID) override;
	//bool CheckMidNodes();

	int CheckModel(bool bMakeErrorObject = false, bool bMakeOutputReport = false);
	int CheckGeometryModel(bool bMakeErrorObject = false, bool bMakeOutputReport = false);

	CString GetWorkerA();
	void SetWorkerA(CString worker);

	virtual void ChangFloorQT(double dMinX, double dMinY, double dMaxX, double dMaxY) override;

	virtual void FixCompoGInfoAfterSerialize() override;
	virtual void CheckInValidElements() override;
	virtual void ReGenerateJoint() override;

	void FixBoundaryCompoGs();
	
	/**
	 * 현재 액티브한 리비전을 리턴하는 함수.
	 * 하위 클래스에서 자기 상황에 따라 오버라이드된 함수 호출됨.
	 * @return 현재 액티브한 리비전 객체
	 */
	virtual MSRevision* GetCurrentRevision() { return NULL; };

	RebarGlobalOptionManager m_rebarOptionManager;

	long GetModelCheckElementCnt();
	virtual MSSteelUtility* GetSteelUtili() override;
	void DeleteSteelUtil();
	
	virtual MSFormWorkTypeManager* GetFormManager() { return mp_FormManager; };
	void AllCompoGUsedFormWorkExistCheck();
	MSCustomSection* CreateNewCustomSection(int sectionType);


protected:
	long m_PlainConcreteReinforceBarSettingID;	// 무근콘크리트
	long m_DefaultReinforceBarSettingID;

	MSSteelUtility* m_pSteelUtil;
public:
	virtual void SetDefaultReinforceBarSettingID(long nID) override { m_DefaultReinforceBarSettingID = nID; };
	virtual long GetDefaultReinforceBarSettingID()	override { return m_DefaultReinforceBarSettingID; };
	virtual void SetPlainConcreteReinforceBarSettingID(long nID) override { m_PlainConcreteReinforceBarSettingID = nID; };
	virtual long GetPlainConcreteReinforceBarSettingID() override { return m_PlainConcreteReinforceBarSettingID; };

	map<long, MSReinforceBar*> mm_ReinforceBar;
	void DeleteReinforceBars();
	virtual MSReinforceBar* GetReinforceBar(long ID) override;
	MSReinforceBar* GetReinforceBar(CString Name);
	bool Add(MSReinforceBar* pReinforceBar);
	long Remove(MSReinforceBar* pReinforceBar);
	long Delete(MSReinforceBar* pReinforceBar);
	void CreateNewReinforceBars();

	map<long, MSReinforceBarSetting*> mm_ReinforceBarSetting;
	void DeleteReinforceBarSettings();
	virtual MSReinforceBarSetting* GetReinforceBarSetting(long ID) override;
	virtual MSReinforceBarSetting* GetReinforceBarSetting(CString name) override;

	bool Add(MSReinforceBarSetting* pReinforceBarSetting);
	long Remove(MSReinforceBarSetting* pReinforceBarSetting);
	long Delete(MSReinforceBarSetting* pReinforceBarSetting);

	MSSteelGradeManager* m_pSGradeManager;
	virtual	MSSteelGradeManager* GetMSSteelGradeManager() override { return m_pSGradeManager; };
	void SetReinforceBarManager();

	void CreateDefaultOpeningReinforceM();
	virtual MSCompoM* GetDefaultOpeningReinforceM() override;

	virtual void PopupCheckMsgNBuildingUpdate() override;
	void UpdateWallGtoWallEndRebarLineID(int wallEndRebarLineID);

	virtual int GetSectionViewCount() override;
	void ClearRegenList();
protected:
	CString m_Worker;

	map<long, MSCustomSection*> mm_CustomSection;
public:
	bool IsCustomCompoM(MSCompoM* compoM);
	bool IsUsedCustomSection(int sectionID);

	void DeleteCustomSections();
	virtual MSCustomSection* GetCustomSection(long ID) override;
	map<long, MSCustomSection*> GetCustomSections() { return mm_CustomSection; };
	virtual bool Add(MSCustomSection* pCustomSection) override;
	virtual long Remove(MSCustomSection* pCustomSection) override;
	virtual long Delete(MSCustomSection* pCustomSection) override;

	virtual vector<MSCustomM*> GetCompoMsHasCustomSectionID(int sectionID) override;
	void LoggingCompoGsTypeGeo();
	void LogningConnections();
	//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	// start of addition by ironbell
//protected:
//	virtual void InnerImportIFC(int pIfcId);
//	virtual int  InnerExportIFC();
//	// end of addition by ironbell
//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:
	void Patch_Version20140729();
	void Patch_Material_Version20150701();
	void AddLevelZoneAfterSerialize();

	void RemoveSpacialCharacters_InCompoM();
	bool IsCorrectedStoryMaterials();
	void RepairStoryMaterials();
	void RecoverCustomProfile();

	void CheckFloorQT();

	void RecoverTypeUIDFactoryByMSObjectLastID() override;
public:
	void CalcUpdateType() override;

	int m_nRebarTemplateID;
	BaseRebarTemplate* GetRebarTemplate(RT_MemberType type);
	CString GetRebarTemplateName();
	void SetRebarTemplateName(CString sName);
	int GetRebarTemplateID();

	int m_nFormWorkTemplateID;
	virtual FormWorkTemplate* GetFormWorkTemplate() override;
	CString GetFormWorkTemplateName();
	void SetFormWorkTemplateByName(CString sName);
	int GetFormWorkTemplateID();

	int m_nNayukTemplateID;
	CString GetNayukTemplateName();
	void SetNayukTemplateByName(CString sName);
	int GetNayukTemplateID();

	// temp 강차장님 부탁
	void WriteFile_4KangjiHawan();
private:
	void UpdateReinforceBarGB_BH3374();
	void ConvertFloor_UType();
private:
	// 소스코드에서 더이상 사용하지 않음
	int facilitySeparationType;

	//GS건설 새UCS기능 업데이트
public:
	virtual GM2DVector GetUCSOriginPoint() override;
	virtual GM2DVector GetUCSXDir() override;
	virtual GM2DVector GetUCSYDir() override;
	virtual GM2DVector GetImportBldgUCSOriginPoint() override;
	virtual GM2DVector GetImportBldgUCSXDir() override;
	virtual GM2DVector GetImportBldgUCSYDir() override;
	virtual double GetUCSz() override;

	virtual void SetBuildingUCS(GM2DVector originPoint, GM2DVector xDir, GM2DVector yDir) override;
	virtual void SetBuildingUCSz(double z) override;
	virtual void SetImportBuildingUCS(GM2DVector originPoint, GM2DVector xDir, GM2DVector yDir) override;
	BOOL m_bTopDownPlace;
	long m_nTopDownStartFloor;
public:
	virtual vector<long> GetDummyCompoGIDList() override;
	virtual void AddDummyCompoGID(long dummyCompoGID) override;
	virtual void DummyCompoGIDListClear() override;

	void TotalColumnMNameDuplicateCheckAndNameModify();
	void ColumnMNameDuplicateCheckAndNameModify(MSCompoM* SrcCompoM, vector<MSCompoM*> desColumnMList);
	void ColumnNameModify(MSCompoM* SrcCompoM);

	virtual BOOL GetTopDownPlace() override;
	virtual void SetTopDownPlace(BOOL value) override;
	virtual long GetTopDownStartFloor() override;
	virtual void SetTopDownStartFloor(long value) override;
private:
	vector<long> DummyCompoGIDList;

public:
	virtual void AddNote(CString sNote);
	virtual bool NoteExistCheck(CString sNote);
	virtual void DelNote(int nNoteID);
	virtual void EditNote(CString sOldNote, CString sEditNote);
	virtual int NewNoteID();
	virtual void NoteClear();
	map<int, CString> mm_Note;

	MSAreaTemplateManager* m_pAreaTemplateManager;
public:
	virtual MSAreaTemplateManager* GetAreaTemplateManager() override { return m_pAreaTemplateManager; };

private:
	int m_nBuildingInfoID;
	MSBuildingInfoManager* m_pBuildingInfoManager;
public:
	virtual int GetBuildingInfoID();
	virtual void SetBuildingInfoID(int buildingInfoID) { m_nBuildingInfoID = buildingInfoID; }
	virtual MSBuildingInfo* GetBuildingInfo();
	virtual CString GetBuildingInfoType();
	virtual vector<CString> GetBuildingInfoTypeList();
	virtual MSBuildingInfoManager* GetBuildingInfoManager() { return m_pBuildingInfoManager; };
	void WriteLogInfo();

private:
	map<long, MSCompoM*> mm_DefaultCompoM;
public:
	MSCompoM *GetDefaultCompoM(long compoMType);
	void SetDefaultCompoM(MSCompoM* defCompoM);
	void RemoveDefaultCompoM(long compoMType);
private:
	void makeVerticalConstructionZones();

public:
	CString GetSectionViewNameByCopyToPaste(CString name);

};
