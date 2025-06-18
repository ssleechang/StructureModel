#include "stdafx.h"
#include "NayukConcreteSetting.h"
#include "MSFloor.h"

NayukConcreteSetting::NayukConcreteSetting()
{
	InitData();
}

NayukConcreteSetting::~NayukConcreteSetting()
{
}

void NayukConcreteSetting::InitData()
{
	m_UsingPump = false;
	m_nPumpSectionType = 0;
	m_nOneSectionFloorNum = 11;
	m_nTwoSectionFloorNum = 16;

	InitConcMaterialCostSet();
	InitConcInstallCostSet();
}

void NayukConcreteSetting::InitConcMaterialCostSet()
{
	ma_ConcMaterialCost.push_back(new ConcMaterialCost(25, 21, 18, false, _T(""), _T(""), _T(""), _T("")));
	ma_ConcMaterialCost.push_back(new ConcMaterialCost(25, 21, 18, true, _T(""), _T(""), _T(""), _T("")));
	ma_ConcMaterialCost.push_back(new ConcMaterialCost(25, 24, 18, false, _T(""), _T(""), _T(""), _T("")));
}

void NayukConcreteSetting::InitConcInstallCostSet()
{
	ma_ConcInstallCost.push_back(new ConcInstallCost(false, 0, _T(""), _T(""), _T(""), _T("")));
	ma_ConcInstallCost.push_back(new ConcInstallCost(false, 1, _T(""), _T(""), _T(""), _T("")));
	ma_ConcInstallCost.push_back(new ConcInstallCost(false, 2, _T(""), _T(""), _T(""), _T("")));
	ma_ConcInstallCost.push_back(new ConcInstallCost(true, 0, _T(""), _T(""), _T(""), _T("")));
	ma_ConcInstallCost.push_back(new ConcInstallCost(true, 1, _T(""), _T(""), _T(""), _T("")));
	ma_ConcInstallCost.push_back(new ConcInstallCost(true, 2, _T(""), _T(""), _T(""), _T("")));
}

void NayukConcreteSetting::CreateConcMaterialCost(int materialCostNum)
{
	ma_ConcMaterialCost.clear();

	for (int index = 0; index < materialCostNum; index++)
	{
		ma_ConcMaterialCost.push_back(new ConcMaterialCost());
	}
}

void NayukConcreteSetting::CreateConcInstallCost(int installCostNum)
{
	ma_ConcInstallCost.clear();

	for (int index = 0; index < installCostNum; index++)
	{
		ma_ConcInstallCost.push_back(new ConcInstallCost());
	}
}

MSObject* NayukConcreteSetting::Copy(bool bNewID /*= true*/)
{
	NayukConcreteSetting* pNewObj;
	pNewObj = new NayukConcreteSetting();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void NayukConcreteSetting::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;

	NayukConcreteSetting* Source = (NayukConcreteSetting*)pSource_in;
	m_UsingPump = Source->m_UsingPump;
	m_nPumpSectionType = Source->m_nPumpSectionType;
	m_nOneSectionFloorNum = Source->m_nOneSectionFloorNum;
	m_nTwoSectionFloorNum = Source->m_nTwoSectionFloorNum;
	ma_ConcMaterialCost.clear();
	ma_ConcInstallCost.clear();

	for (int index = 0; index < Source->ma_ConcMaterialCost.size(); index++)
	{
		ma_ConcMaterialCost.push_back(Source->ma_ConcMaterialCost[index]);
	}

	for (int index = 0; index < Source->ma_ConcInstallCost.size(); index++)
	{
		ma_ConcInstallCost.push_back(Source->ma_ConcInstallCost[index]);
	}

	MSObject::CopyFromMe(Source, bNewID);
}

void NayukConcreteSetting::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	
	buffer << m_UsingPump;
	buffer << m_nPumpSectionType;
	buffer << m_nOneSectionFloorNum;
	buffer << m_nTwoSectionFloorNum;
	int count = (int)ma_ConcMaterialCost.size();
	buffer << count;
	for (int i = 0; i < ma_ConcMaterialCost.size(); i++)
	{
		int iAggregate = ma_ConcMaterialCost[i]->m_Aggregate;
		int iStrength = ma_ConcMaterialCost[i]->m_Strength;
		int iSlump = ma_ConcMaterialCost[i]->m_Slump;
		bool iIsPlain = ma_ConcMaterialCost[i]->m_IsPlainConc;
		CString iCode = ma_ConcMaterialCost[i]->m_Code;
		CString iName = ma_ConcMaterialCost[i]->m_Name;
		CString iStandard = ma_ConcMaterialCost[i]->m_Standard;
		CString iUnit = ma_ConcMaterialCost[i]->m_Unit;
		buffer << iAggregate;
		buffer << iStrength;
		buffer << iSlump;
		buffer << iIsPlain;
		buffer << iCode;
		buffer << iName;
		buffer << iStandard;
		buffer << iUnit;
	}

	count = (int)ma_ConcInstallCost.size();
	buffer << count;
	for (int i = 0; i < ma_ConcInstallCost.size(); i++)
	{
		bool iIsPlain = ma_ConcInstallCost[i]->m_IsPlainConc;
		int placementType = ma_ConcInstallCost[i]->m_nPlacementType;
		CString iCode = ma_ConcInstallCost[i]->m_Code;
		CString iName = ma_ConcInstallCost[i]->m_Name;
		CString iStandard = ma_ConcInstallCost[i]->m_Standard;
		CString iUnit = ma_ConcInstallCost[i]->m_Unit;
		buffer << iIsPlain;
		buffer << placementType;
		buffer << iCode;
		buffer << iName;
		buffer << iStandard;
		buffer << iUnit;
	}
}

void NayukConcreteSetting::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);

	ma_ConcMaterialCost.clear();
	ma_ConcInstallCost.clear();

	buffer >> m_UsingPump;
	buffer >> m_nPumpSectionType;
	buffer >> m_nOneSectionFloorNum;
	buffer >> m_nTwoSectionFloorNum;
	int count = 0;
	buffer >> count;
	for (int i = 0; i < count; i++)
	{
		int iAggregate = -1;
		int iStrength = -1;
		int iSlump = -1;
		bool iIsPlain = false;//만에 하나 걸러내지 못할수도 있긴하지만 그냥 초기값으로 사용
		CString iCode = _T("");
		CString iName = _T("");
		CString iStandard = _T("");
		CString iUnit = _T("");
		buffer >> iAggregate;
		buffer >> iStrength;
		buffer >> iSlump;
		buffer >> iIsPlain;
		buffer >> iCode;
		buffer >> iName;
		buffer >> iStandard;
		buffer >> iUnit;
		ma_ConcMaterialCost.push_back(new ConcMaterialCost(iAggregate, iStrength, iSlump, iIsPlain, iCode, iName, iStandard, iUnit));
	}
	count = 0;
	buffer >> count;
	for (int i = 0; i < count; i++)
	{
		bool iIsPlain = false;
		int placementType = 0;
		CString iCode = _T("");
		CString iName = _T("");
		CString iStandard = _T("");
		CString iUnit = _T("");
		buffer >> iIsPlain;
		buffer >> placementType;
		buffer >> iCode;
		buffer >> iName;
		buffer >> iStandard;
		buffer >> iUnit;
		ma_ConcInstallCost.push_back(new ConcInstallCost(iIsPlain, placementType, iCode, iName, iStandard, iUnit));
	}
}

void NayukConcreteSetting::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_UsingPump;
		ar << m_nPumpSectionType;
		ar << m_nOneSectionFloorNum;
		ar << m_nTwoSectionFloorNum;
		int count = (int)ma_ConcMaterialCost.size();
		ar << count;
		for (int i = 0; i < ma_ConcMaterialCost.size(); i++)
		{
			int iAggregate = ma_ConcMaterialCost[i]->m_Aggregate;
			int iStrength = ma_ConcMaterialCost[i]->m_Strength;
			int iSlump = ma_ConcMaterialCost[i]->m_Slump;
			bool iIsPlain = ma_ConcMaterialCost[i]->m_IsPlainConc;
			CString iCode = ma_ConcMaterialCost[i]->m_Code;
			CString iName = ma_ConcMaterialCost[i]->m_Name;
			CString iStandard = ma_ConcMaterialCost[i]->m_Standard;
			CString iUnit = ma_ConcMaterialCost[i]->m_Unit;
			ar << iAggregate;
			ar << iStrength;
			ar << iSlump;
			ar << iIsPlain;
			ar << iCode;
			ar << iName;
			ar << iStandard;
			ar << iUnit;
		}

		count = (int)ma_ConcInstallCost.size();
		ar << count;
		for (int i = 0; i < ma_ConcInstallCost.size(); i++)
		{
			bool iIsPlain = ma_ConcInstallCost[i]->m_IsPlainConc;
			int placementType = ma_ConcInstallCost[i]->m_nPlacementType;
			CString iCode = ma_ConcInstallCost[i]->m_Code;
			CString iName = ma_ConcInstallCost[i]->m_Name;
			CString iStandard = ma_ConcInstallCost[i]->m_Standard;
			CString iUnit = ma_ConcInstallCost[i]->m_Unit;
			ar << iIsPlain;
			ar << placementType;
			ar << iCode;
			ar << iName;
			ar << iStandard;
			ar << iUnit;
		}
	}
	else
	{
		ma_ConcMaterialCost.clear();
		ma_ConcInstallCost.clear();

		ar >> m_UsingPump;
		ar >> m_nPumpSectionType;
		ar >> m_nOneSectionFloorNum;
		ar >> m_nTwoSectionFloorNum;
		int count = 0;
		ar >> count;
		for (int i = 0; i < count; i++)
		{
			int iAggregate = -1;
			int iStrength = -1;
			int iSlump = -1;
			bool iIsPlain = false;//만에 하나 걸러내지 못할수도 있긴하지만 그냥 초기값으로 사용
			CString iCode = _T("");
			CString iName = _T("");
			CString iStandard = _T("");
			CString iUnit = _T("");
			ar >> iAggregate;
			ar >> iStrength;
			ar >> iSlump;
			ar >> iIsPlain;
			ar >> iCode;
			ar >> iName;
			ar >> iStandard;
			ar >> iUnit;
			ma_ConcMaterialCost.push_back(new ConcMaterialCost(iAggregate, iStrength, iSlump, iIsPlain, iCode, iName, iStandard, iUnit));
		}
		count = 0;
		ar >> count;
		for (int i = 0; i < count; i++)
		{
			bool iIsPlain = false;
			int placementType = 0;
			CString iCode = _T("");
			CString iName = _T("");
			CString iStandard = _T("");
			CString iUnit = _T("");
			ar >> iIsPlain;
			ar >> placementType;
			ar >> iCode;
			ar >> iName;
			ar >> iStandard;
			ar >> iUnit;
			ma_ConcInstallCost.push_back(new ConcInstallCost(iIsPlain, placementType, iCode, iName, iStandard, iUnit));
		}
	}
}
