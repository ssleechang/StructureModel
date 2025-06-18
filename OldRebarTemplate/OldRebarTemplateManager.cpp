#include "StdAfx.h"
#include "OldRebarTemplateManager.h"
#include "..\..\GMLib\MSVersionInfo.h"
#include "..\RebarTemplate.h"

OldRebarTemplateManager* OldRebarTemplateManager::m_instance = NULL;
OldRebarTemplateManager* OldRebarTemplateManager::GetInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new OldRebarTemplateManager();
	}
	return m_instance;
}

OldRebarTemplateManager::OldRebarTemplateManager()
{
	m_nID = 0;
}

OldRebarTemplateManager::~OldRebarTemplateManager(void)
{
	DeleteBuildingRebarTemplates();
	DeleteFloorRebarTemplates();
	DeleteMemberTemplates();
}

void OldRebarTemplateManager::DeleteBuildingRebarTemplates()
{
	map<long, OldBuildingRebarTemplate*>::iterator it;
	for(it = mm_BuildingRebarTemplate.begin(); it != mm_BuildingRebarTemplate.end(); it++)
		delete it->second;
	mm_BuildingRebarTemplate.clear();
}

void OldRebarTemplateManager::DeleteFloorRebarTemplates()
{
	map<long, OldFloorRebarTemplate*>::iterator it;
	for(it = mm_FloorRebarTemplate.begin(); it != mm_FloorRebarTemplate.end(); it++)
		delete it->second;
	mm_FloorRebarTemplate.clear();
}

void OldRebarTemplateManager::DeleteMemberTemplates()
{
	map<long, OldMemberRebarTemplate*>::iterator it;
	for(it = mm_MemberRebarTemplate.begin(); it != mm_MemberRebarTemplate.end(); it++)
		delete it->second;
	mm_MemberRebarTemplate.clear();
}

bool OldRebarTemplateManager::WriteFileBinary(CString strFilePath)
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

bool OldRebarTemplateManager::ReadFileBinary(CString strFilePath)
{
	CFile file;
	CFileException e;
	if (file.Open(strFilePath, CFile::modeRead, &e) == FALSE)
	{
		if (e.m_cause == CFileException::fileNotFound)
			return false;
		else
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		}
	}
	if (file.GetLength() <= 0)	return false;

	CArchive ar(&file, CArchive::load);
	Serialize(ar);

	return true;
}

void OldRebarTemplateManager::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar << MSVersionInfo::GetLatestVersion();

		ar << (int)mm_BuildingRebarTemplate.size();
		map<long, OldBuildingRebarTemplate*>::iterator itBuilding;
		for(itBuilding = mm_BuildingRebarTemplate.begin(); itBuilding != mm_BuildingRebarTemplate.end(); itBuilding++)
			itBuilding->second->Serialize(ar);

		ar << (int)mm_FloorRebarTemplate.size();
		map<long, OldFloorRebarTemplate*>::iterator itFloor;
		for(itFloor = mm_FloorRebarTemplate.begin(); itFloor != mm_FloorRebarTemplate.end(); itFloor++)
			itFloor->second->Serialize(ar);

		ar << (int)mm_MemberRebarTemplate.size();
		map<long, OldMemberRebarTemplate*>::iterator itMember;
		for(itMember = mm_MemberRebarTemplate.begin(); itMember != mm_MemberRebarTemplate.end(); itMember++)
			itMember->second->Serialize(ar);
	}
	else
	{
		int firstVal, firstData;
		int version;

		ar >> firstVal;

		if (firstVal == 0 || firstVal == 1)
		{
			version = 0;
			MSVersionInfo::PushCurrentVersion(version);

			firstData = (BOOL)firstVal;
		}
		else
		{
			version = firstVal;
			MSVersionInfo::PushCurrentVersion(version);

			ar >> firstData;
		}

		mm_BuildingRebarTemplate.clear();
		mm_FloorRebarTemplate.clear();
		mm_MemberRebarTemplate.clear();

		if (firstVal < 20160122)
		{
			auto buildingRebarTemplate = new OldBuildingRebarTemplate();
			buildingRebarTemplate->SerializeReadForBefore20160122(ar, firstData);
			buildingRebarTemplate->m_sName = _T("Building Rebar Template");
			buildingRebarTemplate->m_nID = GetNewID();
			SetLastID(buildingRebarTemplate->m_nID);
			mm_BuildingRebarTemplate.insert(make_pair(buildingRebarTemplate->m_nID, buildingRebarTemplate));
		}
		else
		{
			int numArr;

			numArr = firstData;
			for (int i = 0; i < numArr; i++)
			{
				auto buildingRebarTemplate = new OldBuildingRebarTemplate();
				buildingRebarTemplate->Serialize(ar);
				mm_BuildingRebarTemplate.insert(make_pair(buildingRebarTemplate->m_nID, buildingRebarTemplate));
			}

			ar >> numArr;
			for (int i = 0; i < numArr; i++)
			{
				auto floorRebarTemplate = new OldFloorRebarTemplate();
				floorRebarTemplate->Serialize(ar);
				mm_FloorRebarTemplate.insert(make_pair(floorRebarTemplate->m_nID, floorRebarTemplate));
			}

			ar >> numArr;
			for (int i = 0; i < numArr; i++)
			{
				auto memberRebarTemplate = new OldMemberRebarTemplate();
				memberRebarTemplate->Serialize(ar);
				mm_MemberRebarTemplate.insert(make_pair(memberRebarTemplate->m_nID, memberRebarTemplate));
			}
		}

		//BH-2109 MemberTemplate에서 벽체,외벽 다월수평근 장대산출옵션 설정 불가능하게 적용
		//기존 MemberTemplate에서 이미 설정을 했을 경우 대비 상위 옵션을 따라가게 적용 코드 추가
		if (firstVal < 20170201)
			SetMemberRebarTemplateOptionModify();

		MSVersionInfo::PopCurrentVersion();
	}
}

long OldRebarTemplateManager::ms_LastID = 0;

long OldRebarTemplateManager::GetNewID()
{
	m_nID = ms_LastID + 1;
	ms_LastID = m_nID;
	return m_nID;
}

void OldRebarTemplateManager::SetLastID(long NewID)
{
	if (NewID > ms_LastID)
		ms_LastID = NewID;
}

OldBuildingRebarTemplate* OldRebarTemplateManager::GetBuildingTemplate()
{
	map<long, OldBuildingRebarTemplate*>::iterator it = mm_BuildingRebarTemplate.begin();
	if(it != mm_BuildingRebarTemplate.end())
		return it->second;
	return NULL;
}

OldBaseRebarTemplate* OldRebarTemplateManager::GetCompoGTypetoRebarTemplate(RT_OldMemberType type, OldCommonRebarTemplate* commonRebarTemplate)
{
	if (type == RT_OldColumn)
		return commonRebarTemplate->m_ColumnRebarTemplate;
	else if (type == RT_OldBeam)
		return commonRebarTemplate->m_BeamRebarTemplate;
	else if (type == RT_OldBWall)
		return commonRebarTemplate->m_BWallRebarTemplate;
	else if (type == RT_OldRWall)
		return commonRebarTemplate->m_RWallRebarTemplate;
	else if (type == RT_OldPit)
		return commonRebarTemplate->m_PitRebarTemplate;
	else if (type == RT_OldSlab)
		return commonRebarTemplate->m_SlabRebarTemplate;
	else if (type == RT_OldIsoFoot)
		return commonRebarTemplate->m_IsoFootRebarTemplate;
	else if (type == RT_OldWallFoot)
		return commonRebarTemplate->m_WallFootRebarTemplate;

	return NULL;
}

OldBaseRebarTemplate* OldRebarTemplateManager::GetRebarTemplate(RT_OldMemberType type, int nMemberTemplateID, int nFloorNum, int& rebarTemplateType, CString& rebarTemplateName)
{
	map<long, OldMemberRebarTemplate*>::iterator itMember = mm_MemberRebarTemplate.find(nMemberTemplateID);
	if (itMember == mm_MemberRebarTemplate.end())
	{
		map<long, OldFloorRebarTemplate*>::iterator itFloor;
		for(itFloor = mm_FloorRebarTemplate.begin(); itFloor != mm_FloorRebarTemplate.end(); itFloor++)
		{
			if (nFloorNum >= itFloor->second->m_nFromStory && nFloorNum <= itFloor->second->m_nToStory)
			{
				rebarTemplateType = 1; //Floor RebarTemplate
				rebarTemplateName = itFloor->second->m_sName;
				return GetCompoGTypetoRebarTemplate(type, itFloor->second);
			}
				
		}

		rebarTemplateType = 0; //Building RebarTemplate
		rebarTemplateName = GetBuildingTemplate()->m_sName;
		return GetCompoGTypetoRebarTemplate(type, GetBuildingTemplate());
	}
	else
	{
		rebarTemplateType = 2; //Member RebarTemplate
		rebarTemplateName = itMember->second->m_sName;
		return GetCompoGTypetoRebarTemplate(type, itMember->second);
	}

	return NULL;
}

OldBaseRebarTemplate* OldRebarTemplateManager::GetFloorNumtoBaseRebarTemplate(RT_OldMemberType type, int nFloorNum)
{
	map<long, OldFloorRebarTemplate*>::iterator itFloor;
	for(itFloor = mm_FloorRebarTemplate.begin(); itFloor != mm_FloorRebarTemplate.end(); itFloor++)
	{
		if (nFloorNum >= itFloor->second->m_nFromStory && nFloorNum <= itFloor->second->m_nToStory)
			return GetCompoGTypetoRebarTemplate(type, itFloor->second);
	}

	return GetCompoGTypetoRebarTemplate(type, GetBuildingTemplate());
}

OldMemberRebarTemplate* OldRebarTemplateManager::GetNameToMemberRebarTemplate(CString name)
{
	map<long, OldMemberRebarTemplate*>::iterator it;
	for(it = mm_MemberRebarTemplate.begin(); it != mm_MemberRebarTemplate.end(); it++)
	{
		if (name == it->second->m_sName)
			return it->second;
	}

	return NULL;
}

OldMemberRebarTemplate* OldRebarTemplateManager::GetIDToMemberRebarTemplate(long nID)
{
	map<long, OldMemberRebarTemplate*>::iterator it =mm_MemberRebarTemplate.find(nID);
	if(it != mm_MemberRebarTemplate.end())
		return it->second;

	return NULL;
}

void OldRebarTemplateManager::CreateRebarTemplateData()
{
	DeleteBuildingRebarTemplates();
	DeleteFloorRebarTemplates();
	DeleteMemberTemplates();

	auto buildingRebarTemplate = new OldBuildingRebarTemplate();
	buildingRebarTemplate->CreateData();
	buildingRebarTemplate->m_nID = GetNewID();

	mm_BuildingRebarTemplate.insert(make_pair(buildingRebarTemplate->m_nID, buildingRebarTemplate));
}

CString OldRebarTemplateManager::GetBuildingTemplateName()
{
	OldBuildingRebarTemplate* buildingTemplate = GetBuildingTemplate();
	if(buildingTemplate != NULL)
		return buildingTemplate->m_sName;
	return _T("");
}

long OldRebarTemplateManager::GetBuildingTemplateID()
{
	OldBuildingRebarTemplate* buildingTemplate = GetBuildingTemplate();
	if(buildingTemplate != NULL)
		return buildingTemplate->m_nID;
	return 0;
}

void OldRebarTemplateManager::Add(OldFloorRebarTemplate* floorTemplate)
{
	mm_FloorRebarTemplate.insert(make_pair(floorTemplate->m_nID, floorTemplate));
}

long OldRebarTemplateManager::Remove(OldFloorRebarTemplate* floorTemplate)
{
	long nID = floorTemplate->m_nID;
	map<long, OldFloorRebarTemplate*>::iterator it = mm_FloorRebarTemplate.find(nID);
	if(it != mm_FloorRebarTemplate.end())
		mm_FloorRebarTemplate.erase(it);
	return nID;
}

void OldRebarTemplateManager::Add(OldMemberRebarTemplate* memberTemplate)
{
	mm_MemberRebarTemplate.insert(make_pair(memberTemplate->m_nID, memberTemplate));
}

long OldRebarTemplateManager::Remove(OldMemberRebarTemplate* memberTemplate)
{
	long nID = memberTemplate->m_nID;
	map<long, OldMemberRebarTemplate*>::iterator it = mm_MemberRebarTemplate.find(nID);
	if(it != mm_MemberRebarTemplate.end())
		mm_MemberRebarTemplate.erase(it);

	return nID;
}

OldBaseRebarTemplate* OldRebarTemplateManager::CreateRebarTemplate(RT_OldMemberType type)
{
	switch(type)
	{
	case RT_OldColumn:	return new OldColumnRebarTemplate();
	case RT_OldBeam:		return new OldBeamRebarTemplate();
	case RT_OldBWall:		return new OldBWallRebarTemplate();
	case RT_OldRWall:		return new OldRWallRebarTemplate();
	case RT_OldPit:				return new OldPitRebarTemplate();
	case RT_OldSlab:			return new OldSlabRebarTemplate();
	case RT_OldIsoFoot:		return new OldIsoFootRebarTemplate();
	case RT_OldWallFoot:		return new OldWallFootRebarTemplate();
	}
	ASSERT(0);
	return NULL;
}

void OldRebarTemplateManager::SetMemberRebarTemplateOptionModify()
{
	map<long, OldMemberRebarTemplate*>::iterator itMember;
	for (itMember = mm_MemberRebarTemplate.begin(); itMember != mm_MemberRebarTemplate.end(); itMember++)
	{
		if(itMember->second->m_nMemberType != RT_OldBWall && itMember->second->m_nMemberType != RT_OldRWall) continue;

		OldBaseRebarTemplate* memberRebarTemplate = GetCompoGTypetoRebarTemplate((RT_OldMemberType)itMember->second->m_nMemberType, itMember->second);
		if (itMember->second->m_nMemberType == RT_OldBWall)
		{
			if(memberRebarTemplate->GetBWallDowelInfoSet() == FALSE) continue;

			OldCommonRebarTemplate* commonRebarTemplate = GetTopOptionIDtoRebarTemplate(itMember->second->m_nTopOptionID);

			BOOL bTopOptionDowelSupportPlaceType = commonRebarTemplate->m_BWallRebarTemplate->GetBWallDowelSupportPlaceType();
			memberRebarTemplate->SetBWallDowelSupportPlaceType(bTopOptionDowelSupportPlaceType);
		}
		else if (itMember->second->m_nMemberType == RT_OldRWall)
		{
			if (memberRebarTemplate->GetRWallDowelInfoSet() == FALSE) continue;

			OldCommonRebarTemplate* commonRebarTemplate = GetTopOptionIDtoRebarTemplate(itMember->second->m_nTopOptionID);

			BOOL bTopOptionDowelSupportPlaceType = commonRebarTemplate->m_RWallRebarTemplate->GetRWallDowelSupportPlaceType();
			memberRebarTemplate->SetRWallDowelSupportPlaceType(bTopOptionDowelSupportPlaceType);
		}
	}
}

OldCommonRebarTemplate* OldRebarTemplateManager::GetTopOptionIDtoRebarTemplate(int nTopOptionID)
{
	map<long, OldBuildingRebarTemplate*>::iterator it;
	for (it = mm_BuildingRebarTemplate.begin(); it != mm_BuildingRebarTemplate.end(); it++)
	{
		if (it->first == nTopOptionID)
			return it->second;
	}
		
	map<long, OldFloorRebarTemplate*>::iterator itFloor;
	for (itFloor = mm_FloorRebarTemplate.begin(); itFloor != mm_FloorRebarTemplate.end(); itFloor++)
	{
		if (itFloor->first == nTopOptionID)
			return itFloor->second;
	}

	return nullptr;
}

void OldRebarTemplateManager::OldBuildingDataCopy(RebarTemplate* rebarTemplate)
{
	OldBuildingRebarTemplate* buildingTemplate = GetBuildingTemplate();
	if (buildingTemplate == NULL) return;

	buildingTemplate->OldBuildingDataCopy(rebarTemplate);
}