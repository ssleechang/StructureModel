#pragma once
#include "NayukTemplate.h"

class StructureModel_CLASS NayukTemplateManager
{
public:
	static NayukTemplateManager* GetInstance();
	~NayukTemplateManager();

	bool WriteFileBinary(CString strFilePath);
	int ReadFileBinary(CString strFilePath);
	bool WriteNayukTemplateFileBinary(MSBaseBuilding* building, CString strFilePath, CString sFormWorkTemplateName);
	NayukTemplate* ReadNayukTemplateFileBinary(CString strFilePath, MSBaseBuilding* building);
	void Serialize(CArchive& archive);
	map<long, NayukTemplate*> mm_NayukTemplate;
	NayukTemplate* GetNayukTemplateByID(long nID);
	NayukTemplate* GetNayukTemplateByName(CString sName);
	void Add(NayukTemplate* nayukTemplate);
	long Remove(long nID);
	void DeleteNayukTemplates();

	//이 함수 두개는 당장 필요한가?
	long GetBuildingTemplateID();
	void SetBuildingTemplateID(long nID);

	long m_nID;
	long GetNewID();
	static long ms_LastID;
	static void SetLastID(long NewID);

private:
	static NayukTemplateManager* m_instance;
	NayukTemplateManager();
	long m_nBuildingTemplateID;
};