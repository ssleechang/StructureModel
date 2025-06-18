#include "stdafx.h"
#include "NayukFormworkSetting.h"
#include "MSBaseBuilding.h"
#include "MSFormWorkTypeManager.h"

NayukFormworkSetting::NayukFormworkSetting(MSBaseBuilding* building)
{
	if(building != nullptr)
		InitFormCostSet(building);
}

NayukFormworkSetting::~NayukFormworkSetting()
{
}

void NayukFormworkSetting::InitFormCostSet(MSBaseBuilding* building)
{
	vector<CString> formWorkNameListAll = building->GetFormManager()->GetFormWorkTypeNameListAll();
	for (int i = 0; i < formWorkNameListAll.size(); i++)
	{
		if (formWorkNameListAll[i] == building->GetFormManager()->GetNoneFormWorkTypeName()) continue;

		ma_FormMaterialCost.push_back(new FormCost(formWorkNameListAll[i], _T(""), _T(""), _T(""), _T("")));
		ma_FormInstallCost.push_back(new FormCost(formWorkNameListAll[i], _T(""), _T(""), _T(""), _T("")));
	}
}

void NayukFormworkSetting::CreateFormWorkMaterialCost(int nMaterialCostNum)
{
	ma_FormMaterialCost.clear();

	for (int index = 0; index < nMaterialCostNum; index++)
	{
		ma_FormMaterialCost.push_back(new FormCost());
	}
}

void NayukFormworkSetting::CreateFormWorkInstallCost(int nInstallCostNum)
{
	ma_FormInstallCost.clear();

	for (int index = 0; index < nInstallCostNum; index++)
	{
		ma_FormInstallCost.push_back(new FormCost());
	}
}

MSObject* NayukFormworkSetting::Copy(bool bNewID /*= true*/)
{
	NayukFormworkSetting* pNewObj;
	pNewObj = new NayukFormworkSetting(nullptr);
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void NayukFormworkSetting::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;

	NayukFormworkSetting* Source = (NayukFormworkSetting*)pSource_in;
	ma_FormMaterialCost.clear();
	ma_FormInstallCost.clear();

	for (int index = 0; index < Source->ma_FormMaterialCost.size(); index++)
	{
		ma_FormMaterialCost.push_back(Source->ma_FormMaterialCost[index]);
	}

	for (int index = 0; index < Source->ma_FormInstallCost.size(); index++)
	{
		ma_FormInstallCost.push_back(Source->ma_FormInstallCost[index]);
	}

	MSObject::CopyFromMe(Source, bNewID);
}
void NayukFormworkSetting::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	int count = (int)ma_FormMaterialCost.size();
	buffer << count;
	for (int i = 0; i < ma_FormMaterialCost.size(); i++)
	{
		CString iFormName = ma_FormMaterialCost[i]->m_FormName;
		CString iCode = ma_FormMaterialCost[i]->m_Code;
		CString iName = ma_FormMaterialCost[i]->m_Name;
		CString iStandard = ma_FormMaterialCost[i]->m_Standard;
		CString iUnit = ma_FormMaterialCost[i]->m_Unit;
		buffer << iFormName;
		buffer << iCode;
		buffer << iName;
		buffer << iStandard;
		buffer << iUnit;
	}

	count = (int)ma_FormInstallCost.size();
	buffer << count;
	for (int i = 0; i < ma_FormInstallCost.size(); i++)
	{
		CString iFormName = ma_FormInstallCost[i]->m_FormName;
		CString iCode = ma_FormInstallCost[i]->m_Code;
		CString iName = ma_FormInstallCost[i]->m_Name;
		CString iStandard = ma_FormInstallCost[i]->m_Standard;
		CString iUnit = ma_FormInstallCost[i]->m_Unit;
		buffer << iFormName;
		buffer << iCode;
		buffer << iName;
		buffer << iStandard;
		buffer << iUnit;
	}
}

void NayukFormworkSetting::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);

	ma_FormMaterialCost.clear();
	ma_FormInstallCost.clear();
	int count = 0;
	buffer >> count;
	for (int i = 0; i < count; i++)
	{
		CString iFormName = _T("");
		CString iCode = _T("");
		CString iName = _T("");
		CString iStandard = _T("");
		CString iUnit = _T("");
		buffer >> iFormName;
		buffer >> iCode;
		buffer >> iName;
		buffer >> iStandard;
		buffer >> iUnit;
		ma_FormMaterialCost.push_back(new FormCost(iFormName, iCode, iName, iStandard, iUnit));
	}
	count = 0;
	buffer >> count;
	for (int i = 0; i < count; i++)
	{
		CString iFormName = _T("");
		CString iCode = _T("");
		CString iName = _T("");
		CString iStandard = _T("");
		CString iUnit = _T("");
		buffer >> iFormName;
		buffer >> iCode;
		buffer >> iName;
		buffer >> iStandard;
		buffer >> iUnit;
		ma_FormInstallCost.push_back(new FormCost(iFormName, iCode, iName, iStandard, iUnit));
	}
}

void NayukFormworkSetting::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << (int)ma_FormMaterialCost.size();
		for (int i = 0; i < ma_FormMaterialCost.size(); i++)
		{
			CString iFormName = ma_FormMaterialCost[i]->m_FormName;
			CString iCode = ma_FormMaterialCost[i]->m_Code;
			CString iName = ma_FormMaterialCost[i]->m_Name;
			CString iStandard = ma_FormMaterialCost[i]->m_Standard;
			CString iUnit = ma_FormMaterialCost[i]->m_Unit;
			ar << iFormName;
			ar << iCode;
			ar << iName;
			ar << iStandard;
			ar << iUnit;
		}
		ar << (int)ma_FormInstallCost.size();
		for (int i = 0; i < ma_FormInstallCost.size(); i++)
		{
			CString iFormName = ma_FormInstallCost[i]->m_FormName;
			CString iCode = ma_FormInstallCost[i]->m_Code;
			CString iName = ma_FormInstallCost[i]->m_Name;
			CString iStandard = ma_FormInstallCost[i]->m_Standard;
			CString iUnit = ma_FormInstallCost[i]->m_Unit;
			ar << iFormName;
			ar << iCode;
			ar << iName;
			ar << iStandard;
			ar << iUnit;
		}
	}
	else
	{
		ma_FormMaterialCost.clear();
		ma_FormInstallCost.clear();
		int count = 0; 
		ar >> count;
		for (int i = 0; i < count; i++)
		{
			CString iFormName = _T("");
			CString iCode = _T("");
			CString iName = _T("");
			CString iStandard = _T("");
			CString iUnit = _T("");
			ar >> iFormName;
			ar >> iCode;
			ar >> iName;
			ar >> iStandard;
			ar >> iUnit;
			ma_FormMaterialCost.push_back(new FormCost(iFormName, iCode, iName, iStandard, iUnit));	
		}
		count = 0;
		ar >> count;
		for (int i = 0; i < count; i++)
		{
			CString iFormName = _T("");
			CString iCode = _T("");
			CString iName = _T("");
			CString iStandard = _T("");
			CString iUnit = _T("");
			ar >> iFormName;
			ar >> iCode;
			ar >> iName;
			ar >> iStandard;
			ar >> iUnit;
			ma_FormInstallCost.push_back(new FormCost(iFormName, iCode, iName, iStandard, iUnit));
		}
	}
}
