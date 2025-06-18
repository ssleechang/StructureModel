#include "stdafx.h"
#include "MSAreaTemplate.h"
#include "MSAreaTemplateManager.h"
#include "..\RADFCore_CLI\AppOptionsWrapper.h"
#include "..\GMLib\MSVersionInfo.h"

IMPLEMENT_SERIAL(MSAreaTemplate, MSObject, VERSIONABLE_SCHEMA | 1)

long MSAreaTemplate::ms_LastID = 0;
MSAreaTemplate::MSAreaTemplate(void)
{
	m_ID = 0;
	m_Name = _T("");
}

MSAreaTemplate::~MSAreaTemplate(void)
{
	DeleteAreaList();
}

void MSAreaTemplate::DeleteAreaList()
{
	map<long, MSArea*>::iterator it;
	for (it = mm_Area.begin(); it != mm_Area.end(); it++)
		delete it->second;
	mm_Area.clear();
}

long MSAreaTemplate::GetID()
{
	if (m_ID == 0) // 
	{
		m_ID = ms_LastID + 1;
		ms_LastID = m_ID;
		return m_ID;
	}

	return m_ID;
}

void MSAreaTemplate::SetLastID(long NewID)
{
	if (NewID > ms_LastID)
		ms_LastID = NewID;
}

MSObject* MSAreaTemplate::Copy(bool bNewID /*= true*/)
{
	MSAreaTemplate* pNewObj = new MSAreaTemplate();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSAreaTemplate::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;

	MSAreaTemplate* Source = dynamic_cast<MSAreaTemplate*>(pSource_in);
	if (Source == nullptr)	return;

	m_Name = Source->m_Name;

	DeleteAreaList();
	for (auto it = Source->mm_Area.begin(); it != Source->mm_Area.end(); it++)
	{
		MSArea* data = new MSArea();
		data->GetID();
		data->CopyFromMe(it->second);
		Add(data);
	}
}
void MSAreaTemplate::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	buffer << m_Name;
}
void MSAreaTemplate::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	buffer >> m_Name;
}

void MSAreaTemplate::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_ID;
		ar << m_Name;

		int count = (int)mm_Area.size();
		ar << count;
		for (auto it = mm_Area.begin(); it != mm_Area.end(); it++)
		{
			it->second->Serialize(ar);
		}
	}
	else
	{
		ar >> m_ID;
		ar >> m_Name;
		int count = 0;
		ar >> count;

		for (int i = 0; i < count; i++)
		{
			MSArea* pArea = new MSArea();
			pArea->Serialize(ar);
			Add(pArea);
		}
		SetLastID(m_ID);
	}
}

void MSAreaTemplate::Add(MSArea* area)
{
	if (area == nullptr)	return;
	if (area->m_ID == 0)	area->GetID();
	mm_Area.insert(make_pair(area->m_ID, area));
}

bool MSAreaTemplate::Remove(long Id)
{
	auto it = mm_Area.find(Id);
	if (it != mm_Area.end())
	{
		mm_Area.erase(it);
		return true;
	}
	return false;
}

vector<MSArea*> MSAreaTemplate::GetAreaList()
{
	vector<MSArea*> areaList;
	for (auto it = mm_Area.begin(); it != mm_Area.end(); it++)
		areaList.push_back(it->second);
	return areaList;
}
MSArea* MSAreaTemplate::GetArea(long Id)
{
	auto it = mm_Area.find(Id);
	if (it != mm_Area.end())	return it->second;
	return nullptr;

}
