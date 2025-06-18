#pragma once
#include "MSObject.h"

class MSBuildingInfo;
class MSBuildingInfoColorData;
class MSBuildingClassificationData;
class StructureModel_CLASS MSBuildingInfoManager : public MSObject
{
public:
	MSBuildingInfoManager(void);
	~MSBuildingInfoManager(void);

	DECLARE_SERIAL(MSBuildingInfoManager)
	void Serialize(CArchive& archive);

	MSBuildingInfo* GetBuildingInfoByID(long Id);
	MSBuildingInfo* GetBuildingInfo(CString infoName);
	MSBuildingInfo* GetDefaultBuildingInfo();

	void Add(MSBuildingInfo* buildingInfo, bool IsColorDataAdd = true);
	void Remove(long Id);
	void DeleteBuildingInfoMap();

	vector<MSBuildingInfo*> GetBuildingInfoList();

	vector<CString> GetBuildingUsageList();
	vector<CString> GetBuildingNameList();

	void CreateDefaultBuildingInfoAndAdd(CString sBuildingName, CString sName);

	MSBuildingInfoColorData* CreateColorByIndex(int index);
	MSBuildingInfoColorData* GetUsageColor(CString usage);
	MSBuildingInfoColorData* GetNameColor(CString usage);

	void ColorDataAdd(MSBuildingInfo* buildingInfo);
	void SetBuildingUsageChange(CString oldUsage, CString newUsage);
	void SetBuildingNameChange(CString oldName, CString newName);

	void BuildingInfoColorValideCheck();

	void UpdateBuildingInfo();
	void SetDefMSBuildingClassification();
	MSBuildingClassificationData* AddMSBuildingClassification(CString bldgType, CString bldgClassification);
	bool RemoveBuildingClassification(long Id);
	void RemoveAllBuildingClassification();
	MSBuildingClassificationData* GetBuildingClassification(long Id);
	MSBuildingClassificationData* GetBuildingClassification(CString buildingType);
	CString GetBuildingType(CString buildingClassification);
	
	vector<CString> GetBuildingTypeList();
	vector<CString> GetBuildingClassificationList();
	vector<MSBuildingClassificationData*> GetMSBuildingClassificationArr();
	
private:
	map<long, MSBuildingInfo*> mm_BuildingInfo;
	map<CString, MSBuildingInfoColorData*> mm_BuildingUsageColor;
	map<CString, MSBuildingInfoColorData*> mm_BuildingNameColor;

	// 건물 분류 설정
	vector<MSBuildingClassificationData*> m_BuildingClassificationArr;
};
