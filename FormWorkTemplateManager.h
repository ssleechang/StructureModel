#pragma once
#include "FormWorkTemplate.h"
#include "MSFormWorkTypeManager.h"
#include "MSFormWorkType.h"
#include "MSBaseBuilding.h"

class StructureModel_CLASS FormWorkTemplateManager
{
public:
	static FormWorkTemplateManager* GetInstance();
	~FormWorkTemplateManager(void);

	bool WriteFileBinary(CString strFilePath);
	int ReadFileBinary(CString strFilePath);
	bool WriteFormWorkTemplateFileBinary(MSBaseBuilding* building, CString strFilePath, CString sFormWorkTemplateName);
	FormWorkTemplate* ReadFormWorkTemplateFileBinary(CString strFilePath);

	MSFormWorkTypeManager* FormWorkTemplateManager::GetImportFormWorkTypeManager();
	MSFormWorkType* GetImportFormWorkTypeByName(CString formName);

	void Serialize(CArchive& archive);

	map<long, FormWorkTemplate*> mm_FormWorkTemplate;

	FormWorkTemplate* GetFormWorkTemplateByID(long nID);
	FormWorkTemplate* GetFormWorkTemplateByName(CString sName);

	void Add(FormWorkTemplate* formWorkTemplate);
	long Remove(long nID);
	void DeleteFormWorkTemplates();

	long GetBuildingTemplateID();
	void SetBuildingTemplateID(long nID);

	long m_nID;
	long GetNewID();
	static long ms_LastID;
	static void SetLastID(long NewID);

	CString GetNoneFormWorkTemplateName();

private:
	static FormWorkTemplateManager* m_instance;
	FormWorkTemplateManager();
	long m_nBuildingTemplateID;
	MSFormWorkTypeManager* importFormWorkTypeManager;
};
