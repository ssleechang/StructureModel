#pragma once
#include "MSAreaTemplate.h"

class StructureModel_CLASS MSAreaTemplateManager : public MSObject
{
public:
	MSAreaTemplateManager(void);
	~MSAreaTemplateManager(void);

	DECLARE_SERIAL(MSAreaTemplateManager)
	void Serialize(CArchive& archive);

	MSAreaTemplate* GetAreaTemplate(long Id);

	void Add(MSAreaTemplate* calcAreaItem);
	void Remove(long Id);
	void DeleteAreaTemplateDictionary();

	vector<MSAreaTemplate*> GetAreaTemplateList();
	MSAreaTemplate* GetCurrentAreaTemplate();
	vector<MSArea*> GetAreaList(long floorNum);
	void SetCurrentAreaTemplate(long ID);
private:
	long m_CurrentAreaTemplateID;
	map<long, MSAreaTemplate*> mm_AreaTemplate;

};
