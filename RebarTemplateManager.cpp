#include "StdAfx.h"
#include "RebarTemplateManager.h"
#include "..\GMLib\MSVersionInfo.h"

RebarTemplateManager* RebarTemplateManager::m_instance = NULL;
RebarTemplateManager* RebarTemplateManager::GetInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new RebarTemplateManager();
	}
	return m_instance;
}

RebarTemplateManager::RebarTemplateManager()
{
	m_nID = 0;
}

RebarTemplateManager::~RebarTemplateManager(void)
{
	DeleteRebarTemplates();
}

bool RebarTemplateManager::WriteFileBinary(CString strFilePath)
{
	CFile file;
	CFileException e;
	if (file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite, &e) == FALSE)
	{
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		}
	}
	CArchive ar(&file, CArchive::store);
	Serialize(ar);
	return true;
}

int RebarTemplateManager::ReadFileBinary(CString strFilePath)
{
	CFile file;
	CFileException e;
	if (file.Open(strFilePath, CFile::modeRead, &e) == FALSE)
	{
		if (e.m_cause == CFileException::fileNotFound)
			return e.m_cause;
		else
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		}
	}
	if (file.GetLength() <= 0)	return e.m_cause;

	CArchive ar(&file, CArchive::load);
	Serialize(ar);

	return e.m_cause;
}

bool RebarTemplateManager::WriteRebarTemplateFileBinary(CString strFilePath, CString sRebarTemplateName)
{
	CFile file;
	CFileException e;
	if (file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite, &e) == FALSE)
	{
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		}
	}
	CArchive ar(&file, CArchive::store);

	ar << MSVersionInfo::GetLatestVersion();

	RebarTemplate* rebarTemplate = GetRebarTemplateByName(sRebarTemplateName);
	rebarTemplate->Serialize(ar);

	return true;
}

RebarTemplate* RebarTemplateManager::ReadRebarTemplateFileBinary(CString strFilePath)
{
	CFile file;
	CFileException e;
	if (file.Open(strFilePath, CFile::modeRead, &e) == FALSE)
	{
		if (e.m_cause != CFileException::fileNotFound)
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		}		
		return nullptr;
	}	

	if (file.GetLength() <= 0)	return nullptr;

	CArchive ar(&file, CArchive::load);

	int version;

	ar >> version;
	MSVersionInfo::PushCurrentVersion(version);

	RebarTemplate* rebarTemplate = new RebarTemplate();
	rebarTemplate->Serialize(ar);

	MSVersionInfo::PopCurrentVersion();

	return rebarTemplate;
}

void RebarTemplateManager::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar << MSVersionInfo::GetLatestVersion();

		ar << (int)mm_RebarTemplate.size();
		map<long, RebarTemplate*>::iterator itBuilding;
		for(itBuilding = mm_RebarTemplate.begin(); itBuilding != mm_RebarTemplate.end(); itBuilding++)
			itBuilding->second->Serialize(ar);

		ar << m_nBuildingTemplateID;
	}
	else
	{
		int version;

		ar >> version;
		MSVersionInfo::PushCurrentVersion(version);

		DeleteRebarTemplates();

		int numArr;

		ar >> numArr;
		for (int i = 0; i < numArr; i++)
		{
			auto rebarTemplate = new RebarTemplate();
			rebarTemplate->Serialize(ar);
			mm_RebarTemplate.insert(make_pair(rebarTemplate->m_nID, rebarTemplate));
		}

		ar >> m_nBuildingTemplateID;

		if (m_nBuildingTemplateID < 1 && mm_RebarTemplate.size() > 0)
		{
			map<long, RebarTemplate*>::iterator it = mm_RebarTemplate.begin();
			m_nBuildingTemplateID = it->first;
		}

		MSVersionInfo::PopCurrentVersion();
	}
}

long RebarTemplateManager::ms_LastID = 0;

long RebarTemplateManager::GetNewID()
{
	m_nID = ms_LastID + 1;
	ms_LastID = m_nID;
	return m_nID;
}

void RebarTemplateManager::SetLastID(long NewID)
{
	if (NewID > ms_LastID)
		ms_LastID = NewID;
}

BaseRebarTemplate* RebarTemplateManager::GetRebarTemplateByMemberType(RT_MemberType type, RebarTemplate* rebarTemplate)
{
	if (type == RT_Column)
		return rebarTemplate->GetColumnRebarTemplate();
	else if (type == RT_BWall)
		return rebarTemplate->GetBWallRebarTemplate();
	else if (type == RT_RWall)
		return rebarTemplate->GetRWallRebarTemplate();
	else if (type == RT_LowWall)
		return rebarTemplate->GetLowWallRebarTemplate();
	else if (type == RT_Beam)
		return rebarTemplate->GetBeamRebarTemplate();
	else if (type == RT_Slab)
		return rebarTemplate->GetSlabRebarTemplate();
	else if (type == RT_Pit)
		return rebarTemplate->GetPitRebarTemplate();
	else if (type == RT_IsoFoot)
		return rebarTemplate->GetIsoFootRebarTemplate();
	else if (type == RT_WallFoot)
		return rebarTemplate->GetWallFootRebarTemplate();
	else if(type == RT_TransferColumn)
		return rebarTemplate->GetTransferColumnRebarTemplate();
	else if(type == RT_TransferBeam)
		return rebarTemplate->GetTransferBeamRebarTemplate();
	else if(type == RT_TransferSlab)
		return rebarTemplate->GetTransferSlabRebarTemplate();
	else if(type == RT_DeckBeam)
		return rebarTemplate->GetDeckBeamRebarTemplate();

	return nullptr;
}

BaseRebarTemplate* RebarTemplateManager::GetRebarTemplate(RT_MemberType type, int nMemberTemplateID)
{
	map<long, RebarTemplate*>::iterator itMember = mm_RebarTemplate.find(nMemberTemplateID);
	if (itMember != mm_RebarTemplate.end())
	{
		return GetRebarTemplateByMemberType(type, itMember->second);
	}

	return nullptr;
}

RebarTemplate* RebarTemplateManager::GetRebarTemplateByID(long nID)
{
	map<long, RebarTemplate*>::iterator it =mm_RebarTemplate.find(nID);
	if(it != mm_RebarTemplate.end())
		return it->second;

	return nullptr;
}

RebarTemplate* RebarTemplateManager::GetRebarTemplateByName(CString sName)
{
	map<long, RebarTemplate*>::iterator it;
	for (it = mm_RebarTemplate.begin(); it != mm_RebarTemplate.end(); it++)
	{
		if (it->second->m_sName != sName) continue;

		return it->second;
	}

	return nullptr;
}

void RebarTemplateManager::Add(RebarTemplate* rebarTemplate)
{
	mm_RebarTemplate.insert(make_pair(rebarTemplate->m_nID, rebarTemplate));
}

long RebarTemplateManager::Remove(long nID)
{
	map<long, RebarTemplate*>::iterator it = mm_RebarTemplate.find(nID);
	if (it != mm_RebarTemplate.end())
	{
		delete it->second;
		mm_RebarTemplate.erase(it);
	}

	return nID;
}

void RebarTemplateManager::DeleteRebarTemplates()
{
	map<long, RebarTemplate*>::iterator it;
	for (it = mm_RebarTemplate.begin(); it != mm_RebarTemplate.end(); it++)
		delete it->second;
	mm_RebarTemplate.clear();
}

long RebarTemplateManager::GetBuildingTemplateID() 
{ 
	return m_nBuildingTemplateID; 
}

void RebarTemplateManager::SetBuildingTemplateID(long nID)
{
	m_nBuildingTemplateID = nID;
}

CString RebarTemplateManager::GetNoneRebarTemplateName()
{
	return _T("None");
}

BaseRebarTemplate* RebarTemplateManager::CreateRebarResultOption(RT_MemberType gType)
{
	BaseRebarTemplate* pResultOption = nullptr;
	switch (gType)
	{
	case RT_MemberType::RT_Column:	pResultOption = new ColumnRebarTemplate();	break;
	case RT_MemberType::RT_BWall:	    pResultOption = new BWallRebarTemplate();	break;
	case RT_MemberType::RT_RWall:	pResultOption = new RWallRebarTemplate();	break;
	case RT_MemberType::RT_LowWall:	    pResultOption = new LowWallRebarTemplate();	break;
	case RT_MemberType::RT_Beam:	pResultOption = new RWallRebarTemplate();	break;
	case RT_MemberType::RT_Slab:	    pResultOption = new SlabRebarTemplate();	break;
	case RT_MemberType::RT_Pit:	pResultOption = new PitRebarTemplate();	break;
	case RT_MemberType::RT_IsoFoot:	    pResultOption = new IsoFootRebarTemplate();	break;
	case RT_MemberType::RT_WallFoot:	pResultOption = new WallFootRebarTemplate();	break;
	case RT_MemberType::RT_WallColumn:	pResultOption = new ColumnRebarTemplate();	break;
	case RT_MemberType::RT_TransferColumn: pResultOption = new TransferColumnRebarTemplate(); break;
	case RT_MemberType::RT_TransferBeam: pResultOption = new TransferBeamRebarTemplate(); break;
	case RT_MemberType::RT_TransferSlab: pResultOption = new TransferSlabRebarTemplate();	break;
	case RT_MemberType::RT_DeckBeam: pResultOption = new DeckBeamRebarTemplate(); break;
	default:
		ASSERT(0);
	}
	return pResultOption;
}