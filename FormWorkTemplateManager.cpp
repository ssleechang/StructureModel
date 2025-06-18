#pragma once
#include "stdafx.h"
#include "FormWorkTemplateManager.h"
#include "..\GMLib\MSVersionInfo.h"
#include "MSFormWorkTypeManager.h"
#include "MSRebarBuilding.h"
#include "MSFormWorkType.h"

FormWorkTemplateManager* FormWorkTemplateManager::m_instance = NULL;
FormWorkTemplateManager* FormWorkTemplateManager::GetInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new FormWorkTemplateManager();
	}
	return m_instance;
}

FormWorkTemplateManager::FormWorkTemplateManager()
{
	m_nID = 0;
	importFormWorkTypeManager = nullptr;
}


FormWorkTemplateManager::~FormWorkTemplateManager()
{
	DeleteFormWorkTemplates();
}

bool FormWorkTemplateManager::WriteFileBinary(CString strFilePath)
{
	CFile file;
	CFileException e;
	if(file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite, &e) == FALSE)
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

int FormWorkTemplateManager::ReadFileBinary(CString strFilePath)
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

bool FormWorkTemplateManager::WriteFormWorkTemplateFileBinary(MSBaseBuilding* building, CString strFilePath, CString sFormWorkTemplateName)
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
	FormWorkTemplate* formWorkTemplate = GetFormWorkTemplateByName(sFormWorkTemplateName);
	formWorkTemplate->Serialize(ar);

	MSFormWorkTypeManager* formWorkTypeManager = building->GetFormManager();
	formWorkTypeManager->Serialize(ar);

	return true;
}

FormWorkTemplate* FormWorkTemplateManager::ReadFormWorkTemplateFileBinary(CString strFilePath)
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
	FormWorkTemplate* formWorkTemplate = new FormWorkTemplate();
	formWorkTemplate->Serialize(ar);

	if (MSVersionInfo::GetCurrentVersion() >= 20181019)
	{
		importFormWorkTypeManager = new MSFormWorkTypeManager();
		importFormWorkTypeManager->Serialize(ar);
	}

	MSVersionInfo::PopCurrentVersion();
	return formWorkTemplate;
}

MSFormWorkTypeManager* FormWorkTemplateManager::GetImportFormWorkTypeManager()
{
	if (importFormWorkTypeManager == nullptr) return nullptr;

	return importFormWorkTypeManager;
}

MSFormWorkType* FormWorkTemplateManager::GetImportFormWorkTypeByName(CString formName)
{
	if (importFormWorkTypeManager == nullptr) return nullptr;

	return importFormWorkTypeManager->GetFormWorkTypeByName(formName);
}

void FormWorkTemplateManager::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << MSVersionInfo::GetLatestVersion();

		ar << (int)mm_FormWorkTemplate.size();
		map<long, FormWorkTemplate*>::iterator itBuilding;
		for (itBuilding = mm_FormWorkTemplate.begin(); itBuilding != mm_FormWorkTemplate.end(); itBuilding++)
			itBuilding->second->Serialize(ar);

		ar << m_nBuildingTemplateID;
	}
	else
	{
		int version =0;
		ar >> version;
		MSVersionInfo::PushCurrentVersion(version);
		DeleteFormWorkTemplates();

		int numArr =0;
		ar >> numArr;
		for (int i = 0; i < numArr; i++)
		{
			auto formWorkTemplate = new FormWorkTemplate();
			formWorkTemplate->Serialize(ar);
			mm_FormWorkTemplate.insert(make_pair(formWorkTemplate->m_nID, formWorkTemplate));
		}
		
		ar >> m_nBuildingTemplateID;
		MSVersionInfo::PopCurrentVersion();

		// https://changsoft.atlassian.net/browse/DQ1-427 
		// 1.4.X버전에 계단 거푸집 추가 이후, 1.3.X에도 반영이 되었는데, m_LatestVersion의 차이로 인해
		// 이미 계단 거푸집이 반영된 1.3.X의 모델을 1.4.X에서 로드 할 때 FormWorkTemplate::Serialize에서 계단 거푸집을 초기화 하는 것을 방지 하기 위해 추가
		// FormWorkTemplateManager::Serialize에서 load가 끝나면 플래그를 돌려 놓는다.
		MSVersionInfo::SetIsVersionChanged(false);
	}
}

long FormWorkTemplateManager::ms_LastID = 0;

long FormWorkTemplateManager::GetNewID()
{
	m_nID = ms_LastID + 1;
	ms_LastID = m_nID;
	return m_nID;
}

void FormWorkTemplateManager::SetLastID(long NewID)
{
	if (NewID > ms_LastID)
		ms_LastID = NewID;
}

FormWorkTemplate* FormWorkTemplateManager::GetFormWorkTemplateByID(long nID)
{
	map<long, FormWorkTemplate*>::iterator it = mm_FormWorkTemplate.find(nID);
	if (it != mm_FormWorkTemplate.end())
		return it->second;

	return nullptr;
}

FormWorkTemplate* FormWorkTemplateManager::GetFormWorkTemplateByName(CString sName)
{
	map<long, FormWorkTemplate*>::iterator it;
	for (it = mm_FormWorkTemplate.begin(); it != mm_FormWorkTemplate.end(); it++)
	{
		if (it->second->m_sName != sName) continue;
		return it->second;
	}
	
	return nullptr;
}

void FormWorkTemplateManager::Add(FormWorkTemplate* formWorkTemplate)
{
	mm_FormWorkTemplate.insert(make_pair(formWorkTemplate->m_nID, formWorkTemplate));
}

long FormWorkTemplateManager::Remove(long nID)
{
	map<long, FormWorkTemplate*>::iterator it = mm_FormWorkTemplate.find(nID);
	if (it != mm_FormWorkTemplate.end())
	{
		delete it->second;
		mm_FormWorkTemplate.erase(it);
	}

	return nID;
}

void FormWorkTemplateManager::DeleteFormWorkTemplates()
{
	map<long, FormWorkTemplate*>::iterator it;
	for (it = mm_FormWorkTemplate.begin(); it != mm_FormWorkTemplate.end(); it++)
		delete it->second;
	mm_FormWorkTemplate.clear();
}

long FormWorkTemplateManager::GetBuildingTemplateID()
{
	return m_nBuildingTemplateID;
}

void FormWorkTemplateManager::SetBuildingTemplateID(long nID)
{
	m_nBuildingTemplateID = nID;
}

CString FormWorkTemplateManager::GetNoneFormWorkTemplateName()
{
	return _T("None");
}