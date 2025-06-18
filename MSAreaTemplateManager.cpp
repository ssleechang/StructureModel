#pragma once
#include "stdafx.h"
#include "MSAreaTemplateManager.h"
#include "..\GMLib\MSVersionInfo.h"

IMPLEMENT_SERIAL(MSAreaTemplateManager, MSObject, VERSIONABLE_SCHEMA | 1)
MSAreaTemplateManager::MSAreaTemplateManager()
{
	m_CurrentAreaTemplateID = 0;
}

MSAreaTemplateManager::~MSAreaTemplateManager()
{
	DeleteAreaTemplateDictionary();
}

void MSAreaTemplateManager::Serialize(CArchive& ar)
{
	MSObject::Serialize(ar);
	int count = 0; 
	if (ar.IsStoring())
	{
		ar << m_CurrentAreaTemplateID;
		count = (int)mm_AreaTemplate.size();
		ar << count;
		for (auto it  = mm_AreaTemplate.begin(); it != mm_AreaTemplate.end(); it++)
			it->second->Serialize(ar);
	}
	else
	{
		ar >> m_CurrentAreaTemplateID;
		ar >> count;
		for (int i = 0; i < count; i++)
		{
			auto calculationArea = new MSAreaTemplate();
			calculationArea->Serialize(ar);
			Add(calculationArea);
		}
	}
}

MSAreaTemplate* MSAreaTemplateManager::GetAreaTemplate(long Id)
{
	auto it = mm_AreaTemplate.find(Id);
	if (it != mm_AreaTemplate.end())	return it->second;
	return nullptr;
}

void MSAreaTemplateManager::Add(MSAreaTemplate* areaTemplate)
{
	m_CurrentAreaTemplateID = areaTemplate->m_ID;
	mm_AreaTemplate.insert(make_pair(areaTemplate->m_ID, areaTemplate));
}

void MSAreaTemplateManager::Remove(long Id)
{
	auto it = mm_AreaTemplate.find(Id);
	if (it != mm_AreaTemplate.end())
	{
//		delete it->second; 
		mm_AreaTemplate.erase(it);
	}

	return;
}

void MSAreaTemplateManager::DeleteAreaTemplateDictionary()
{
	for (auto it = mm_AreaTemplate.begin(); it != mm_AreaTemplate.end(); it++)
		delete it->second;
	mm_AreaTemplate.clear();
}

vector<MSAreaTemplate*> MSAreaTemplateManager::GetAreaTemplateList()
{
	vector<MSAreaTemplate*> templateList;
	for (auto it = mm_AreaTemplate.begin(); it != mm_AreaTemplate.end(); it++)
		templateList.push_back(it->second);
	return templateList;
}

MSAreaTemplate* MSAreaTemplateManager::GetCurrentAreaTemplate()
{
	if (mm_AreaTemplate.size() == 0) return nullptr;

	auto it = mm_AreaTemplate.find(m_CurrentAreaTemplateID);
	if (it != mm_AreaTemplate.end())	return it->second;

	auto firstTemplate = mm_AreaTemplate.begin()->second;
	m_CurrentAreaTemplateID = firstTemplate->m_ID; 
	return firstTemplate;
}

void MSAreaTemplateManager::SetCurrentAreaTemplate(long ID)
{
	m_CurrentAreaTemplateID = ID;
}

vector<MSArea*> MSAreaTemplateManager::GetAreaList(long floorNum)
{
	vector<MSArea*> areaList;

	MSAreaTemplate* currentTemplate = GetCurrentAreaTemplate();
	if (currentTemplate == nullptr) return areaList;

	vector<MSArea*> allAreaList = currentTemplate->GetAreaList();
	for (int i = 0; i < allAreaList.size(); i++)
	{
		MSArea* area = allAreaList[i];
		if (area->GetFloorNum() == floorNum)
			areaList.push_back(area);
	}
	return areaList;
}
