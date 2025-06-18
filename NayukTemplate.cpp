#include "stdafx.h"
#include "NayukTemplate.h"
#include "NayukTemplateManager.h"

NayukTemplate::NayukTemplate(MSBaseBuilding* building)
{
	InitData(building);
}

NayukTemplate::~NayukTemplate()
{
}

void NayukTemplate::InitData(MSBaseBuilding* building)
{
	m_nID = -1;
	m_sName = _T("");
	m_ConcSetting = new NayukConcreteSetting();
	m_FormSetting = new NayukFormworkSetting(building);
	m_RebarSetting = new NayukRebarSetting(building);
}

MSObject* NayukTemplate::Copy(bool bNewID /*= true*/)
{
	NayukTemplate* pNewObj;
	pNewObj = new NayukTemplate(nullptr);
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void NayukTemplate::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;

	NayukTemplate* Source = (NayukTemplate*)pSource_in;

	m_nID = Source->m_nID;
	m_sName = Source->m_sName;
	m_ConcSetting->CopyFromMe(Source->m_ConcSetting);
	m_FormSetting->CopyFromMe(Source->m_FormSetting);
	m_RebarSetting->CopyFromMe(Source->m_RebarSetting);
	MSObject::CopyFromMe(Source, bNewID);
}
void NayukTemplate::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	buffer << m_nID;
	buffer << m_sName;
	m_ConcSetting->Dump(buffer);
	m_FormSetting->Dump(buffer);
	m_RebarSetting->Dump(buffer);
}

void NayukTemplate::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);

	buffer >> m_nID;
	buffer >> m_sName;
	m_ConcSetting->Recovery(buffer);
	m_FormSetting->Recovery(buffer);
	m_RebarSetting->Recovery(buffer);
}

void NayukTemplate::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_nID;
		ar << m_sName;
		m_ConcSetting->Serialize(ar);
		m_FormSetting->Serialize(ar);
		m_RebarSetting->Serialize(ar);
	}
	else
	{
		ar >> m_nID;
		ar >> m_sName;
		m_ConcSetting->Serialize(ar);
		m_FormSetting->Serialize(ar);
		m_RebarSetting->Serialize(ar);
		NayukTemplateManager::SetLastID(m_nID);
	}
}
