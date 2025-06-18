#pragma once
#include "stdafx.h"
#include "ClassificationNameManager.h"
#include "MSBaseBuilding.h"
#include "..\GMLib\MSVersionInfo.h"

ClassificationNameManager* ClassificationNameManager::m_instance = NULL;
ClassificationNameManager* ClassificationNameManager::GetInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new ClassificationNameManager();
	}
	return m_instance;
}

ClassificationNameManager::ClassificationNameManager()
{
	m_nID = 0;
}

ClassificationNameManager::~ClassificationNameManager()
{
	DeleteClassificationNameDatas();
}

bool ClassificationNameManager::WriteFileBinary(CString strFilePath)
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

int ClassificationNameManager::ReadFileBinary(CString strFilePath)
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

bool ClassificationNameManager::WriteClassificationNameDataFileBinary(MSBaseBuilding* building, CString strFilePath)
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
	
	Serialize(ar);

	return true;
}

void ClassificationNameManager::ReadClassificationNameDataFileBinary(CString strFilePath)
{
	CFile file;
	CFileException e;
	if (file.Open(strFilePath, CFile::modeRead, &e) == FALSE)
	{
		if (e.m_cause == CFileException::fileNotFound)
			return;
		else
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		}
	}
	if (file.GetLength() <= 0) return;
	CArchive ar(&file, CArchive::load);

	int version;
	ar >> version;
	MSVersionInfo::PushCurrentVersion(version);
	Serialize(ar);

	MSVersionInfo::PopCurrentVersion();
}

void ClassificationNameManager::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << MSVersionInfo::GetLatestVersion();

		ar << (int)mm_ClassificationNameData.size();
		map<long, ClassificationNameData*>::iterator it;
		for (it = mm_ClassificationNameData.begin(); it != mm_ClassificationNameData.end(); it++)
			it->second->Serialize(ar);
	}
	else
	{
		int version = 0;
		ar >> version;
		MSVersionInfo::PushCurrentVersion(version);
		DeleteClassificationNameDatas();

		int numArr = 0;
		ar >> numArr;
		for (int i = 0; i < numArr; i++)
		{
			auto data = new ClassificationNameData();
			data->Serialize(ar);
			mm_ClassificationNameData.insert(make_pair(data->m_nID, data));
		}

		MSVersionInfo::PopCurrentVersion();
	}
}

long ClassificationNameManager::ms_LastID = 0;

long ClassificationNameManager::GetNewID()
{
	m_nID = ms_LastID + 1;
	ms_LastID = m_nID;
	return m_nID;
}

void ClassificationNameManager::SetLastID(long NewID)
{
	if (NewID > ms_LastID)
		ms_LastID = NewID;
}

ClassificationNameData* ClassificationNameManager::GetClassificationNameDataByID(long nID)
{
	map<long, ClassificationNameData*>::iterator it = mm_ClassificationNameData.find(nID);
	if (it != mm_ClassificationNameData.end())
		return it->second;

	return nullptr;
}

ClassificationNameData* ClassificationNameManager::GetClassificationNameDataByName(CString sName)
{
	map<long, ClassificationNameData*>::iterator it;
	for (it = mm_ClassificationNameData.begin(); it != mm_ClassificationNameData.end(); it++)
	{
		if (it->second->m_sName != sName) continue;
		return it->second;
	}

	return nullptr;
}

void ClassificationNameManager::Add(ClassificationNameData* classificationNameData)
{
	mm_ClassificationNameData.insert(make_pair(classificationNameData->m_nID, classificationNameData));
}

long ClassificationNameManager::Remove(long nID)
{
	map<long, ClassificationNameData*>::iterator it = mm_ClassificationNameData.find(nID);
	if (it != mm_ClassificationNameData.end())
	{
		delete it->second;
		mm_ClassificationNameData.erase(it);
	}

	return nID;
}

void ClassificationNameManager::DeleteClassificationNameDatas()
{
	map<long, ClassificationNameData*>::iterator it;
	for (it = mm_ClassificationNameData.begin(); it != mm_ClassificationNameData.end(); it++)
		delete it->second;
	mm_ClassificationNameData.clear();
}