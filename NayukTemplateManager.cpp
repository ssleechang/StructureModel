#include "stdafx.h"
#include "NayukTemplateManager.h"
#include "..\GMLib\MSVersionInfo.h"
#include "MSBaseBuilding.h"

NayukTemplateManager* NayukTemplateManager::m_instance = NULL;
NayukTemplateManager* NayukTemplateManager::GetInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new NayukTemplateManager();
	}
	return m_instance;
}

NayukTemplateManager::NayukTemplateManager()
{
	m_nID = 0;
}

NayukTemplateManager::~NayukTemplateManager()
{
	DeleteNayukTemplates();
}

bool NayukTemplateManager::WriteFileBinary(CString strFilePath)
{
	CFile file;
	CFileException e;
	if (file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite, &e) == FALSE)
	{
		{
#ifdef _DEBUG
			afxDump << "File could not be opened" << e.m_cause << "\n";
#endif
		}
	}
	CArchive ar(&file, CArchive::store);
	Serialize(ar);
	return true;
}

int NayukTemplateManager::ReadFileBinary(CString strFilePath)
{
	CFile file;
	CFileException e;
	if (file.Open(strFilePath, CFile::modeRead, &e) == FALSE)
	{
		{
			if (e.m_cause == CFileException::fileNotFound)
				return e.m_cause;
			else
			{
#ifdef _DEBUG
				afxDump << "File could note be opened" << e.m_cause << "\n";
#endif
			}
		}
	}
	if (file.GetLength() <= 0) return e.m_cause;
	CArchive ar(&file, CArchive::load);
	Serialize(ar);
	return e.m_cause;
}

bool NayukTemplateManager::WriteNayukTemplateFileBinary(MSBaseBuilding* building, CString strFilePath, CString sNayukTemplateName)
{
	CFile file;
	CFileException e;
	if (file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite, &e) == FALSE)
	{
		{
#ifdef _DEBUG
			afxDump << "File could not be opened" << e.m_cause << "\n";
#endif
		}
	}

	CArchive ar(&file, CArchive::store);

	ar << MSVersionInfo::GetLatestVersion();
	NayukTemplate* nayukTemplate = GetNayukTemplateByName(sNayukTemplateName);
	nayukTemplate->Serialize(ar);

	return true;
}

NayukTemplate* NayukTemplateManager::ReadNayukTemplateFileBinary(CString strFilePath, MSBaseBuilding* building)
{
	CFile file;
	CFileException e;
	if (file.Open(strFilePath, CFile::modeRead, &e) == FALSE)
	{
		if (e.m_cause == CFileException::fileNotFound)
			return nullptr;
		else
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		}
	}
	if (file.GetLength() <= 0) return nullptr;
	CArchive ar(&file, CArchive::load);

	int version;
	ar >> version;
	MSVersionInfo::PushCurrentVersion(version);
	NayukTemplate* nayukTemplate = new NayukTemplate(building);
	nayukTemplate->Serialize(ar);

	MSVersionInfo::PopCurrentVersion();
	return nayukTemplate;
}

void NayukTemplateManager::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << MSVersionInfo::GetLatestVersion();

		ar << (int)mm_NayukTemplate.size();
		map<long, NayukTemplate*>::iterator itBuilding;
		for (itBuilding = mm_NayukTemplate.begin(); itBuilding != mm_NayukTemplate.end(); itBuilding++)
			itBuilding->second->Serialize(ar);

		ar << m_nBuildingTemplateID;
	}
	else
	{
		int version = 0;
		ar >> version;
		MSVersionInfo::PushCurrentVersion(version);
		DeleteNayukTemplates();

		int numArr = 0;
		ar >> numArr;
		for (int i = 0; i < numArr; i++)
		{
			auto nayukTemplate = new NayukTemplate(nullptr);
			nayukTemplate->Serialize(ar);
			mm_NayukTemplate.insert(make_pair(nayukTemplate->m_nID, nayukTemplate));
		}

		ar >> m_nBuildingTemplateID;
		MSVersionInfo::PopCurrentVersion();
	}
}

long NayukTemplateManager::ms_LastID = 0;

long NayukTemplateManager::GetNewID()
{
	m_nID = ms_LastID + 1;
	ms_LastID = m_nID;
	return m_nID;
}

void NayukTemplateManager::SetLastID(long NewID)
{
	if (NewID > ms_LastID)
		ms_LastID = NewID;
}

NayukTemplate* NayukTemplateManager::GetNayukTemplateByID(long nID)
{
	map<long, NayukTemplate*>::iterator it = mm_NayukTemplate.find(nID);
	if (it != mm_NayukTemplate.end())
		return it->second;

	return nullptr;
}

NayukTemplate* NayukTemplateManager::GetNayukTemplateByName(CString sName)
{
	map<long, NayukTemplate*>::iterator it;
	for (it = mm_NayukTemplate.begin(); it != mm_NayukTemplate.end(); it++)
	{
		if (it->second->m_sName != sName) continue;
		return it->second;
	}

	return nullptr;
}

void NayukTemplateManager::Add(NayukTemplate* nayukTemplate)
{
	mm_NayukTemplate.insert(make_pair(nayukTemplate->m_nID, nayukTemplate));
}

long NayukTemplateManager::Remove(long nID)
{
	map<long, NayukTemplate*>::iterator it = mm_NayukTemplate.find(nID);
	if (it != mm_NayukTemplate.end())
	{
		delete it->second;
		mm_NayukTemplate.erase(it);
	}

	return nID;
}

void NayukTemplateManager::DeleteNayukTemplates()
{
	map<long, NayukTemplate*>::iterator it;
	for (it = mm_NayukTemplate.begin(); it != mm_NayukTemplate.end(); it++)
		delete it->second;
	mm_NayukTemplate.clear();
}

long NayukTemplateManager::GetBuildingTemplateID()
{
	return m_nBuildingTemplateID;
}

void NayukTemplateManager::SetBuildingTemplateID(long nID)
{
	m_nBuildingTemplateID = nID;
}