#pragma once
#include "OldBuildingRebarTemplate.h"
#include "OldFloorRebarTemplate.h"
#include "OldMemberRebarTemplate.h"
#include "..\RebarTemplate.h"

enum RT_OldMemberType 
{
	RT_OldColumn = 0, RT_OldBeam, RT_OldBWall, RT_OldRWall, RT_OldPit, RT_OldSlab, RT_OldIsoFoot, RT_OldWallFoot
};

class StructureModel_CLASS OldRebarTemplateManager
{
public:
	static OldRebarTemplateManager* GetInstance();
	~OldRebarTemplateManager(void);



	bool WriteFileBinary(CString strFilePath);
	bool ReadFileBinary(CString strFilePath);
	void CreateRebarTemplateData();

	void Serialize(CArchive& archive);
	void SerializeReadForBefore20160122(CArchive &ar, int firstData);

	map<long, OldBuildingRebarTemplate*> mm_BuildingRebarTemplate;
	map<long, OldFloorRebarTemplate*>	mm_FloorRebarTemplate;
	map<long, OldMemberRebarTemplate*> mm_MemberRebarTemplate;

	OldBuildingRebarTemplate* GetBuildingTemplate();
	OldBaseRebarTemplate* GetRebarTemplate(RT_OldMemberType type, int nMemberTemplateID, int nFloorNum, int& rebarTemplateType, CString& rebarTemplateName);
	OldBaseRebarTemplate* GetCompoGTypetoRebarTemplate(RT_OldMemberType type, OldCommonRebarTemplate* commonRebarTemplate);
	OldBaseRebarTemplate* GetFloorNumtoBaseRebarTemplate(RT_OldMemberType type, int nFloorNum);
	OldMemberRebarTemplate* GetNameToMemberRebarTemplate(CString name);
	OldMemberRebarTemplate* GetIDToMemberRebarTemplate(long nID);

	void DeleteBuildingRebarTemplates();
	void DeleteFloorRebarTemplates();
	void DeleteMemberTemplates();
// 	vector<shared_ptr<OldBuildingRebarTemplate>> mm_BuildingRebarTemplate;
// 	vector<shared_ptr<OldFloorRebarTemplate>> mm_FloorRebarTemplate;
// 	vector<shared_ptr<OldMemberRebarTemplate>> mm_MemberRebarTemplate;

// 	shared_ptr<OldBaseRebarTemplate> GetRebarTemplate(RT_OldMemberType type, int nMemberTemplateID, int nFloorNum);
// 	shared_ptr<OldBaseRebarTemplate> GetCompoGTypetoRebarTemplate(RT_OldMemberType type, shared_ptr<OldCommonRebarTemplate> commonRebarTemplate);
// 	shared_ptr<OldBaseRebarTemplate> GetFloorNumtoBaseRebarTemplate(RT_OldMemberType type, int nFloorNum);
// 	shared_ptr<OldMemberRebarTemplate> GetNameToMemberRebarTemplate(CString name);
// 	shared_ptr<OldMemberRebarTemplate> GetIDToMemberRebarTemplate(long nID);
	CString GetBuildingTemplateName();
	long GetBuildingTemplateID();

	long m_nID;
	long GetNewID();
	static long ms_LastID;
	static void SetLastID(long NewID);

	void Add(OldFloorRebarTemplate* floorTemplate);
	long Remove(OldFloorRebarTemplate* floorTemplate);

	void Add(OldMemberRebarTemplate* memberTemplate);
	long Remove(OldMemberRebarTemplate* memberTemplate);

	OldBaseRebarTemplate* CreateRebarTemplate(RT_OldMemberType type);

	void SetMemberRebarTemplateOptionModify();
	OldCommonRebarTemplate* GetTopOptionIDtoRebarTemplate(int nTopOptionID);

	void OldBuildingDataCopy(RebarTemplate* rebarTemplate);

private:
	static OldRebarTemplateManager* m_instance;
	OldRebarTemplateManager();
};
