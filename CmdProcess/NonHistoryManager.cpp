#include "StdAfx.h"
#include "NonHistoryManager.h"


NonHistoryManager::NonHistoryManager(void)
{
}


NonHistoryManager::~NonHistoryManager(void)
{
}

void NonHistoryManager::UpdateModifyInfo()
{
	m_AddUpdateInfos = m_AddUpdateInfos_CurTR;
	m_DeleteUpdateInfos = m_DeleteUpdateInfos_CurTR;
	m_ModifyUpdateInfos = m_ModifyUpdateInfos_CurTR;

	m_AddUpdateInfos_CurTR.clear();
	m_DeleteUpdateInfos_CurTR.clear();
	m_ModifyUpdateInfos_CurTR.clear();
}


void NonHistoryManager::UpdateAdditionalModify()
{
	for(int i=0; i< m_AddUpdateInfos_CurTR.size(); i++)
		m_AddUpdateInfos.push_back(m_AddUpdateInfos_CurTR[i]);

	for(int i=0; i< m_DeleteUpdateInfos_CurTR.size(); i++)
		m_DeleteUpdateInfos.push_back(m_DeleteUpdateInfos_CurTR[i]);

	for(int i=0; i< m_ModifyUpdateInfos_CurTR.size(); i++)
		m_ModifyUpdateInfos.push_back(m_ModifyUpdateInfos_CurTR[i]);

	m_AddUpdateInfos_CurTR.clear();
	m_DeleteUpdateInfos_CurTR.clear();
	m_ModifyUpdateInfos_CurTR.clear();
}


void NonHistoryManager::AddAddUpdatedInfo( CommandUpdateElementType type, int elementID, int floorID, int extraID1/*=0*/, int extraID2/*=0*/)
{
	AddDelUpdateInfo addInfo(type, elementID, floorID, extraID1, extraID2);
	m_AddUpdateInfos_CurTR.push_back(addInfo);
}

void NonHistoryManager::AddDeleteUpdateInfo( CommandUpdateElementType type, int elementID, int floorID,  int extraID1/*=0*/, int extraID2/*=0*/)
{
	AddDelUpdateInfo delInfo(type, elementID, floorID, extraID1, extraID2);
	m_DeleteUpdateInfos_CurTR.push_back(delInfo);
}

void NonHistoryManager::AddModifyUpdateInfo( CommandUpdateElementType type, void* element, int elementID, int floorID, int ownerCompoGID, bool isGeometryModified)
{
	ModifyUpdateInfo modInfo(type, element, elementID, floorID, ownerCompoGID, isGeometryModified);
	m_ModifyUpdateInfos_CurTR.push_back(modInfo);
}

vector<AddDelUpdateInfo> NonHistoryManager::GetAddUpdateInfos()
{
	return m_AddUpdateInfos;
}
vector<AddDelUpdateInfo> NonHistoryManager::GetDeleteUpdateInfos()
{
	return m_DeleteUpdateInfos;
}

vector<ModifyUpdateInfo> NonHistoryManager::GetModifyUpdateInfos()
{
	return m_ModifyUpdateInfos;
}

void NonHistoryManager::Clear()
{
	m_AddUpdateInfos.clear();
	m_DeleteUpdateInfos.clear();
	m_ModifyUpdateInfos.clear();

	m_AddUpdateInfos_CurTR.clear();
	m_DeleteUpdateInfos_CurTR.clear();
	m_ModifyUpdateInfos_CurTR.clear();
}
