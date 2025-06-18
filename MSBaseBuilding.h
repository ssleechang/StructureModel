#pragma once

#include "GMLib/GM2DRectangle.h"

#include "MSContainer.h"

class MSCustomM;
class MSCompoM;
class MSCustomSection;
class MSStoryMaterial;
class MSSteelUtility;
class MSRibTypeM;
class MSReinforceBar;
class MSBuildingUCS;
class TypeUIDFactory;
class MSFormWorkTypeManager;
class MSQuantityOption;
class MSBuildingInfo;
class MSBuildingInfoManager;
class FormWorkTemplate;
class MSAreaTemplateManager;
class MSMarkM;
class MSMarkG;
class RebarQuantityInfo;
class MSSteelGradeManager;
class MSWallEndRebarLine;
class MSSpacerShape;

class StructureModel_CLASS MSBaseBuilding : public MSContainer
{
private:
	static MSBaseBuilding* ms_ActiveBuilding;
public:
	static MSBaseBuilding* GetActiveBuilding();
	static void SetActiveBuilding(MSBaseBuilding* pActiveBuilding);
	static long GetUID(int type);

protected:
	TypeUIDFactory* m_pTypeUIDFactory;
	virtual void RecoverTypeUIDFactoryByMSObjectLastID() {};
public:
	TypeUIDFactory* GetTypeUIDFactory() { return m_pTypeUIDFactory; };

public:
	MSBaseBuilding(void);
	~MSBaseBuilding(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSBaseBuilding)

	virtual MRSection*   CreateNewMRSection(long nType);
	CString GetGUIDString();

	void SetID(int id);//Building Import시 ID부여 Project 에서 사용
protected:
	GUID m_GUID;

public:
#pragma region Read/Write File
	CString m_strFilePath;
	virtual bool WriteFileBinary() { return false; };
	virtual bool WriteFileBinary(CString strFilePath) { return false; };
	virtual bool ReadFileBinary(CString strFilePath) { return false; };

#pragma endregion

#pragma region MSBlock
	virtual MSBlock* GetBlock(long blockID) { return nullptr; };
#pragma endregion


#pragma region MSCompoM
	virtual MSCompoM* CreateNewCompoM(long nType);

	virtual bool Add(MSCompoM* pCompoM) {return false;};
	virtual bool Add(vector<MSCompoM*> CompoMArr) { return false; };
	virtual long Remove(MSCompoM* pCompoM) { return 0; };
	virtual bool Remove(vector<MSCompoM*> CompoMArr) { return false; };

	virtual vector<MSCompoM*> GetCompoMList(long nType) { vector<MSCompoM*> compoMList;  return compoMList; };
	virtual map<long, MSCompoM*> GetCompoMMap() { map<long, MSCompoM*> mapCompoM;  return mapCompoM; };
	virtual MSCompoM* GetCompoM(long CompoMID) {return nullptr;};
	virtual MSCompoM* GetCompoM(CString strName, long nType, long nStory = 0) { return nullptr; };

	virtual vector<long> GetCompoMs(long nType);
	virtual vector<long> GetCompoMs();
	virtual vector<long> GetCompoGs(long nMID);
	virtual bool ExistCompoM(CString& strName, long nFloorNum) {return false;};

	virtual void GetAllCompoGsWithCompoM(MSCompoM* compoM, vector<MSCompoG*>& list) {};
	virtual vector<MSCustomM*> GetCompoMsHasCustomSectionID(int sectionID);
	virtual vector<MSCompoM*> GetAllColumnMsWidthColumnHeaderMID(long headerMID);

	bool VerticalMNameAndStoryDuplicateCheck(MSCompoM* SrcCompoM, MSCompoM* DesCompoM, bool IsOnlyNameCheck);
#pragma endregion

#pragma region MSCustomSection
	virtual MSCustomSection* CreateNewCustomSection(int sectionType);
	virtual MSCustomSection* GetCustomSection(long ID) { return nullptr; };

	virtual bool Add(MSCustomSection* pCustomSection) { return false; };
	virtual long Remove(MSCustomSection* pCustomSection) { return 0; };
	virtual long Delete(MSCustomSection* pCustomSection) { return 0; };

#pragma endregion


#pragma region MSMaterial
	virtual MSMaterial* CreateNewMaterial(long nType);
	virtual MSMaterial* GetMaterial(long MaterialID) { return nullptr; };
	virtual MSMaterial* GetMaterial(CString strMateName) { return nullptr; };
	virtual MSMaterial* GetDefMaterial() { return nullptr; };
	virtual MSMaterial* GetDefMaterial(MSMaterial::Type_MSMaterial materialType) { return nullptr; };
	virtual MSMaterial* GetMaterialbyName(MSMaterial::Type_MSMaterial materialType, CString name) { return nullptr; };
	virtual vector<MSMaterial*> GetMaterialsByType(MSMaterial::Type_MSMaterial type);
	virtual map<long, MSMaterial*>* GetMaterialMap();// { map<long, MSMaterial*> materialMap; return materialMap; };

	virtual bool Add(MSMaterial* pMaterial) { return false; };
	virtual long Remove(MSMaterial* pMaterial) { return 0; };
	virtual long Delete(MSMaterial* pMaterial) { return 0; };

#pragma endregion

#pragma region MSStoryMaterial
	virtual MSStoryMaterial* GetStoryMaterialByID(long nID) { return nullptr; };
	virtual MSStoryMaterial* GetStoryMaterialByFloorNum(long floorNum) { return nullptr; };

#pragma endregion

#pragma region MSSteelUtil
	virtual MSSteelUtility* GetSteelUtili() { return nullptr; };

#pragma endregion


#pragma region MSReinforceBar & OpeningReinforceM
	virtual MSReinforceBar* GetReinforceBar(long ID) { return nullptr; };
	virtual MSReinforceBar* GetReinforceBar(CString name) { return nullptr; };

	virtual	MSSteelGradeManager* GetMSSteelGradeManager() { return nullptr; };

	virtual void SetDefaultReinforceBarSettingID(long nID) { };
	virtual long GetDefaultReinforceBarSettingID() { return 0; };
	virtual void SetPlainConcreteReinforceBarSettingID(long nID) { };
	virtual long GetPlainConcreteReinforceBarSettingID() { return 0; };

	virtual MSReinforceBarSetting* GetReinforceBarSetting(long ID) { return nullptr; };
	virtual MSReinforceBarSetting* GetReinforceBarSetting(CString name) { return nullptr; };

	virtual MSCompoM* GetDefaultOpeningReinforceM() { return nullptr; };

#pragma endregion

	virtual MSWallEndRebarLine* CreateNewWallEndRebarLine();
	virtual MSWallEndRebarLine* GetWallEndRebarLine(long ID) { return nullptr; };
	virtual void DeleteWallEndRebarLines() { return; };
	virtual bool Add(MSWallEndRebarLine* pWallEndRebarLine) { return false; };
	virtual long Remove(MSWallEndRebarLine* pWallEndRebarLine) { return 0; };
	virtual long Delete(MSWallEndRebarLine* pWallEndRebarLine) { return 0; };

	MSSpacerShape* CreateNewSpacerShape();
	virtual MSSpacerShape* GetSpacerShape(long ID) { return nullptr; };
	virtual long GetSpacerShape(CString name) { return 0; };

#pragma region MSFloor
	virtual MSFloor* GetFloor(long FloorNum) { return nullptr; };
	virtual long GetNumAboveStory() {return 0;};
	virtual long GetNumUnderStory() {return 0;};
	virtual MSFloor* GetFloorByID(long FloorID) { return nullptr; };
	virtual double GetGroundLevel() { return 0; };
	virtual void SetGroundLevel(double dGroundLevel) {};
	virtual double GetFloorLevel(long FloorNum) { return 0.0;};
	virtual double GetSlabLevel(long FloorNum) { return 0.0; };
	virtual vector<MSFloor*> GetFloorsByLevel(double dStartLevel, double dEndLevel);
	virtual vector<MSFloor*> GetAllFloors();
	virtual void ClearAllCashFloorLevel();
	virtual CString GetFloorName(long nStroy) { return _T(""); };

	virtual void AddFloor(MSFloor* floor) {};
	virtual long Remove(MSFloor* pFloor) { return 0; };
	virtual long Delete(MSFloor* pFloor) { return 0; };
	virtual void DeleteFloors() { return; };
#pragma endregion

#pragma region MSCompoGs
	virtual vector<int> GetAllWallGsWidthRebarLineID(int rebarLineID);
	virtual MSCompoG* GetCompoG(long CompoGID) { return nullptr; };
	virtual MSCompoG* GetCompoGByFloors(long CompoGID) {return nullptr;};
	virtual vector<long> GetCompoGs_InAllFloor(long nMID) ;
	virtual vector<MSCompoG*> GetAllCompoGs();

#pragma endregion

#pragma region MSMark
	virtual void DeleteMarkMs() {};
	virtual MSMarkM* GetMarkM(long MarkerID) { return nullptr; };
	virtual bool Add(MSMarkM* pMarker) { return false; };
	virtual long Remove(MSMarkM* pMarker) { return 0; };
	virtual long Delete(MSMarkM* pMarker) { return 0; };
	virtual MSMarkM* GetMarkM(int type, CString comment) { return nullptr; };

	virtual void DeleteMarkGs() {};
	virtual MSMarkG* GetMarkG(long MarkerID) { return nullptr; };
	virtual bool Add(MSMarkG* pMarker) { return false; };
	virtual long Remove(MSMarkG* pMarker) { return 0; };
	virtual long Delete(MSMarkG* pMarker) { return 0; };
	virtual vector<MSMarkG*> GetMarkGsfromMID(long MID) { vector<MSMarkG*> markGarr; return markGarr; };
	virtual map<long, MSMarkG*>* GetMarkGMap() { return nullptr; };

	virtual int GetNextTagNum(int type) { return 0; };
	virtual MSMarkM* GetMarkMbyErrorCode(int type, int errorCode) { return nullptr; };
#pragma endregion

#pragma region MSGridLine
	virtual MSGridLine* GetGLine(long GLineID) { return nullptr; };

#pragma endregion

#pragma region MSRibTypeM
	map<long, MSRibTypeM*> mm_RibM;

	MSRibTypeM* CreateNewRibM(int RibType);
	void DeleteRibTypes();
	MSRibTypeM* GetRibM(long ID);
	void Add(MSRibTypeM* pRibTypeM);
	void Remove(MSRibTypeM* pRibTypeM);
	void Delete(MSRibTypeM* pRibTypeM);

#pragma endregion

#pragma region Update Model
	bool m_bIsNeedRegenAfterLoad;
public:
	int GetUpdateModelVersion() { return m_nUpdateModelVersion; };
	void SetLastUpdateModelVersion();
	eBuildingUpdateType GetUpdateType() { return m_eUpdateType; };
	void SetUpdateType(eBuildingUpdateType type) { m_eUpdateType = type; }
	virtual void CalcUpdateType() {};
protected:
	int m_nUpdateModelVersion;
	eBuildingUpdateType m_eUpdateType; // 추가
#pragma endregion

public:
#pragma region MSConnection
	virtual void FixCompoGInfoAfterSerialize() {};
	virtual void CheckInValidElements() {};
	virtual void ReGenerateJoint() {};


#pragma endregion

public:

	GM2DRectangle m_SiteRect;
	virtual void ChangFloorQT(double dMinX, double dMinY, double dMaxX, double dMaxY) {};
	bool IsVerticalCompoM(long nType);
	virtual int GetSectionViewCount() { return 0; };

	virtual vector<long> GetDummyCompoGIDList() { vector<long>compoGIDArr; return compoGIDArr; };
	virtual void AddDummyCompoGID(long dummyCompoGID) { };
	virtual void DummyCompoGIDListClear() { };

	//GS건설 새UCS기능 업데이트
public:
	virtual GM2DVector GetUCSOriginPoint();
	virtual GM2DVector GetUCSXDir();
	virtual GM2DVector GetUCSYDir();

	virtual GM2DVector GetImportBldgUCSOriginPoint();
	virtual GM2DVector GetImportBldgUCSXDir();
	virtual GM2DVector GetImportBldgUCSYDir();
	virtual double GetUCSz();

	virtual void SetBuildingUCS(GM2DVector originPoint, GM2DVector xDir, GM2DVector yDir);
	virtual void SetBuildingUCSz(double z);
	virtual void SetImportBuildingUCS(GM2DVector originPoint, GM2DVector xDir, GM2DVector yDir);
protected:
	// 빌딩의 현재 UCS
	MSBuildingUCS* mp_UCS;
	// 빌딩 import 시의 UCS (최초 상태의 UCS)
	MSBuildingUCS* mp_UCS_import;

public:
	virtual void AddNote(CString sNote) {};
	virtual bool NoteExistCheck(CString sNote) { return false; };
	virtual void DelNote(int nNoteID) {};
	virtual void EditNote(CString sOldNote, CString sEditNote) {};
	virtual int NewNoteID() { return 0; };
	virtual void NoteClear() {};
	map<int, CString> mm_Note;

public:
	virtual int GetBuildingInfoID() { return 0; };
	virtual void SetBuildingInfoID(int buildingInfoID) { return; };
	virtual MSBuildingInfo* GetBuildingInfo() { return nullptr; };
	virtual CString GetBuildingInfoType() { return _T(""); };
	virtual vector<CString> GetBuildingInfoTypeList() { vector<CString> buildingInfoTypeList; return buildingInfoTypeList; };
	virtual MSBuildingInfoManager* GetBuildingInfoManager() { return nullptr; };


protected:
	MSFormWorkTypeManager* mp_FormManager;

public:
	virtual bool IsSumSOGAsFooting();

	virtual MSQuantityOption* GetQuantityOption() { return nullptr; };
	virtual RebarQuantityInfo* GetRebarQuantityInfo() { return nullptr; };
	virtual MSFormWorkTypeManager* GetFormManager() { return mp_FormManager; };
	virtual FormWorkTemplate* GetFormWorkTemplate() { return nullptr; };	
	virtual MSAreaTemplateManager* GetAreaTemplateManager() { return nullptr; };

	
protected:
		int m_nLicenseVersion;
public:
	int GetLicenseVersion();
	void SetLicenseVersion(int licenseVersion);
public:
	virtual BOOL GetTopDownPlace() { return FALSE; };
	virtual void SetTopDownPlace(BOOL value) {};
	virtual long GetTopDownStartFloor() { return -1; };
	virtual void SetTopDownStartFloor(long value) {};

	void FixGeometry();
	virtual void PopupCheckMsgNBuildingUpdate() {};
};
