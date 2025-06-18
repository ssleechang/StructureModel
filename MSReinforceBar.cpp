#include "StdAfx.h"
#include "MSReinforceBar.h"
#include "MSDeformedBar.h"
#include "GMLib/MSVersionInfo.h"
IMPLEMENT_SERIAL(MSReinforceBar, MSObject, VERSIONABLE_SCHEMA | 1)
long MSReinforceBar::ms_LastID = 0;
long MSReinforceBar::ms_GlobalLastID = 0;

MSReinforceBar::MSReinforceBar(void)
{
	m_ID = 0;
	m_GlobalID = 0;
	m_Name = _T("SD40");
	m_dBarStrength = 400;
//	m_PrintName = _T("HD");
	m_eSGrade = MSSteelGradeManager::SD300;
	m_pSGradeManager = NULL;
}

MSReinforceBar::~MSReinforceBar(void)
{
}

long MSReinforceBar::GetID()
{
	if(m_ID == 0) // 
	{
		m_ID = ms_LastID + 1;
		ms_LastID = m_ID;
		return m_ID;
	}

	return m_ID;
}

void MSReinforceBar::SetLastID( long NewID )
{
	if(NewID > ms_LastID)
		ms_LastID = NewID;
}

long MSReinforceBar::GetGlobalID()
{
	if(m_GlobalID == 0) // 
	{
		m_GlobalID = ms_GlobalLastID + 1;
		ms_GlobalLastID = m_GlobalID;
		return m_GlobalID;
	}

	return m_GlobalID;
}

void MSReinforceBar::SetGlobalLastID( long NewID )
{
	if(NewID > ms_GlobalLastID)
		ms_GlobalLastID = NewID;
}

MSObject* MSReinforceBar::Copy( bool bNewID /*= true*/ )
{
	MSReinforceBar* pNewObj;

	pNewObj = new MSReinforceBar();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;

}

void MSReinforceBar::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;
	
	MSReinforceBar* pSource = (MSReinforceBar*)pSource_in;
	m_Name = pSource->m_Name;
	m_dBarStrength = pSource->m_dBarStrength;
//	m_PrintName = pSource->m_PrintName;
	m_eSGrade = pSource->m_eSGrade;

	MSObject::CopyFromMe(pSource, bNewID);
}
void MSReinforceBar::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	buffer << m_Name << m_dBarStrength;// << m_PrintName;
	int eSGrade = (int)m_eSGrade;
	buffer << eSGrade;
}

void MSReinforceBar::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	buffer >> m_Name >> m_dBarStrength;// >> m_PrintName;
	int eSGrade = 0;
	buffer >> eSGrade;
	m_eSGrade = (MSSteelGradeManager::eSteelGrade)eSGrade;
}

void MSReinforceBar::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	CString oldPrintName = _T("");
	if(ar.IsStoring())
	{
		ar << m_ID;
		ar << m_Name;
		ar << m_dBarStrength;
		ar << oldPrintName;

		int eSGrade = (int)m_eSGrade;
		ar << eSGrade;
	}
	else
	{
		ar >> m_ID;
		ar >> m_Name;
		ar >> m_dBarStrength;
		ar >> oldPrintName;

		if(MSVersionInfo::GetCurrentVersion() >= 20150323)
		{
			int eSGrade = 0;
			ar >> eSGrade;
			m_eSGrade = (MSSteelGradeManager::eSteelGrade)eSGrade;
		}
		SetLastID(m_ID);
	}
}

void MSReinforceBar::GlobalOptionSerialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	CString oldPrintName = _T("");
	if(ar.IsStoring())
	{
		ar << m_GlobalID;
		ar << m_Name;
		ar << m_dBarStrength;
		ar << oldPrintName;
		int eSGrade = (int)m_eSGrade;
		ar << eSGrade;
	}
	else
	{
		ar >> m_GlobalID;
		ar >> m_Name;
		ar >> m_dBarStrength;
		ar >> oldPrintName;
		if(MSVersionInfo::GetCurrentVersion() >= 20150323)
		{
			int eSGrade = 0;
			ar >> eSGrade;
			m_eSGrade = (MSSteelGradeManager::eSteelGrade)eSGrade;
		}
		SetGlobalLastID(m_GlobalID);
	}
}

CString MSReinforceBar::GetPrintName(bool bChinaUsed)
{
	CString sPrintName = MSDeformedBar::m_sDefaultBarType;
	if(m_pSGradeManager)
		return m_pSGradeManager->GetPrintName(m_eSGrade, bChinaUsed);
	return sPrintName;
}

void MSReinforceBar::SetPrintName(CString printName)
{
	if(m_pSGradeManager)
		m_eSGrade = m_pSGradeManager->GetSGrade(printName);
}

MSReinforceBar* MSReinforceBar::CreateSD600SReinforceBar(bool IsGloblaOption)
{
	MSReinforceBar* pReinforcebar = new MSReinforceBar;
	pReinforcebar->m_Name = STR_SD600S;
	pReinforcebar->m_dBarStrength = 600;
	pReinforcebar->m_eSGrade = MSSteelGradeManager::SD600S;

	if(IsGloblaOption)
		pReinforcebar->GetGlobalID();
	else
		pReinforcebar->GetID();

	return pReinforcebar;
}
MSSteelGradeManager::eSteelGrade MSReinforceBar::GetSGrade()
{
	return m_eSGrade;
}