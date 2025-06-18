#include "stdafx.h"
#include "NayukRebarSetting.h"
#include "ReinforceBarSetting.h"
#include "MSDeformedBar.h"
#include "MSReinforceBar.h"
#include "MSBaseBuilding.h"
#include "..\RADFCore_CLI\LocalDic.h"

NayukRebarSetting::NayukRebarSetting(MSBaseBuilding* building)
{
	InitData(building);
}


NayukRebarSetting::~NayukRebarSetting()
{
}

void NayukRebarSetting::InitData(MSBaseBuilding* building)
{
	m_nFabricateType = 0;

	vector<RebarInstallCost*> vecRebarFactoryInstallCost;
	vecRebarFactoryInstallCost.clear();
	mapWorkTypeToRebarInstallCost[0] = vecRebarFactoryInstallCost;

	vector<RebarInstallCost*> vecRebarSiteInstallCost;
	vecRebarSiteInstallCost.clear();
	mapWorkTypeToRebarInstallCost[1] = vecRebarSiteInstallCost;

	if (building != nullptr)
	{
		InitRebarMaterialCostSet(building);
		InitRebarInstallCostSet();
	}
}

void NayukRebarSetting::InitRebarMaterialCostSet(MSBaseBuilding* building)
{
	//직경&강종
	vector<CString> ma_Dia;
	vector<CString> ma_Material;
	
	map<long, MSMaterial*>::iterator it;
	for (it = building->GetMaterialMap()->begin(); it != building->GetMaterialMap()->end(); it++)
	{
		MSMaterial* pMaterial = it->second;
		if (pMaterial->GetType() == MSMaterial::msConcrete)
		{
			MSReinforceBarSetting* rebarSetting = pMaterial->GetReinforceBarSetting();
			if (rebarSetting == nullptr) continue;

			map<long, MSReinforceBar*>::iterator itBar;
			for (itBar = rebarSetting->mm_IndexToReinforceBar.begin(); itBar != rebarSetting->mm_IndexToReinforceBar.end(); itBar++)
			{
				long DiaIndex = itBar->first;
				MSReinforceBar* pReinforceBar = itBar->second;

				CString sDiaName = _T("D") + MSDeformedBar::FindBarDiameterNameByIndex(itBar->first);
				CString materialName = itBar->second->m_Name;
				
				bool isContained = false;
				for (int i = 0; i < ma_Dia.size(); i++)
				{
					if (ma_Dia[i] == sDiaName && ma_Material[i] == materialName)
						isContained = true;
				}
				if(!isContained)
				{
					ma_Dia.push_back(sDiaName);
					ma_Material.push_back(materialName);
				}
			}			
		}
	}
	
	for (int i = 0; i < ma_Dia.size(); i++)
	{
		ma_RebarMaterialCost.push_back(new RebarMaterialCost(ma_Dia[i], ma_Material[i], _T(""), _T(""), _T(""), _T("")));
	}
}

void NayukRebarSetting::InitRebarInstallCostSet()
{
	mapWorkTypeToRebarInstallCost[0].push_back(new RebarInstallCost(1, LocalDic::GetLocal(_T("복잡")), _T(""), _T(""), _T(""), _T("")));
	mapWorkTypeToRebarInstallCost[0].push_back(new RebarInstallCost(1, LocalDic::GetLocal(_T("보통")), _T(""), _T(""), _T(""), _T("")));
	mapWorkTypeToRebarInstallCost[0].push_back(new RebarInstallCost(2, LocalDic::GetLocal(_T("생산공장->가공공장")), _T(""), _T(""), _T(""), _T("")));
	mapWorkTypeToRebarInstallCost[0].push_back(new RebarInstallCost(2, LocalDic::GetLocal(_T("가공공장->현장")), _T(""), _T(""), _T(""), _T("")));
	mapWorkTypeToRebarInstallCost[0].push_back(new RebarInstallCost(3, LocalDic::GetLocal(_T("하차 및 선별")), _T(""), _T(""), _T(""), _T("")));
	mapWorkTypeToRebarInstallCost[0].push_back(new RebarInstallCost(4, LocalDic::GetLocal(_T("시공도제작")), _T(""), _T(""), _T(""), _T("")));

	mapWorkTypeToRebarInstallCost[1].push_back(new RebarInstallCost(1, LocalDic::GetLocal(_T("복잡")), _T(""), _T(""), _T(""), _T("")));
	mapWorkTypeToRebarInstallCost[1].push_back(new RebarInstallCost(1, LocalDic::GetLocal(_T("보통")), _T(""), _T(""), _T(""), _T("")));
	mapWorkTypeToRebarInstallCost[1].push_back(new RebarInstallCost(2, LocalDic::GetLocal(_T("생산공장->현장")), _T(""), _T(""), _T(""), _T("")));
	mapWorkTypeToRebarInstallCost[1].push_back(new RebarInstallCost(3, LocalDic::GetLocal(_T("하차")), _T(""), _T(""), _T(""), _T("")));
}

void NayukRebarSetting::CreateRebarInstallCost(int nFactoryInstallCostNum, int nSiteInstallCostNum)
{
	mapWorkTypeToRebarInstallCost[0].clear();
	mapWorkTypeToRebarInstallCost[1].clear();

	for (int index = 0; index < nFactoryInstallCostNum; index++)
	{
		mapWorkTypeToRebarInstallCost[0].push_back(new RebarInstallCost());
	}

	for (int index = 0; index < nSiteInstallCostNum; index++)
	{
		mapWorkTypeToRebarInstallCost[1].push_back(new RebarInstallCost());
	}
}

void NayukRebarSetting::CreateRebarMaterialCost(int nMaterialCostNum)
{
	ma_RebarMaterialCost.clear();

	for (int index = 0; index < nMaterialCostNum; index++)
	{
		ma_RebarMaterialCost.push_back(new RebarMaterialCost());
	}
}

MSObject* NayukRebarSetting::Copy(bool bNewID /*= true*/)
{
	NayukRebarSetting* pNewObj;
	pNewObj = new NayukRebarSetting(nullptr);
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void NayukRebarSetting::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;

	NayukRebarSetting* Source = (NayukRebarSetting*)pSource_in;
	m_nFabricateType = Source->m_nFabricateType;

	for (int index = 0; index < Source->ma_RebarMaterialCost.size(); index++)
	{
		ma_RebarMaterialCost.push_back(Source->ma_RebarMaterialCost[index]);
	}

	vector<RebarInstallCost*> vecRebarFactoryInstallCost;
	for (int index = 0; index < Source->mapWorkTypeToRebarInstallCost[0].size(); index++)
	{
		vecRebarFactoryInstallCost.push_back(Source->mapWorkTypeToRebarInstallCost[0][index]);
	}
	mapWorkTypeToRebarInstallCost[0] = vecRebarFactoryInstallCost;

	vector<RebarInstallCost*> vecRebarSiteInstallCost;
	for (int index = 0; index < Source->mapWorkTypeToRebarInstallCost[1].size(); index++)
	{
		vecRebarSiteInstallCost.push_back(Source->mapWorkTypeToRebarInstallCost[1][index]);
	}
	mapWorkTypeToRebarInstallCost[1] = vecRebarSiteInstallCost;

	MSObject::CopyFromMe(Source, bNewID);
}

void NayukRebarSetting::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);

	buffer << m_nFabricateType;
	int count = (int)ma_RebarMaterialCost.size();
	buffer << count;
	for (int i = 0; i < ma_RebarMaterialCost.size(); i++)
	{
		CString iDia = ma_RebarMaterialCost[i]->m_Dia;
		CString iMaterial = ma_RebarMaterialCost[i]->m_Material;
		CString iCode = ma_RebarMaterialCost[i]->m_Code;
		CString iName = ma_RebarMaterialCost[i]->m_Name;
		CString iStandard = ma_RebarMaterialCost[i]->m_Standard;
		CString iUnit = ma_RebarMaterialCost[i]->m_Unit;
		buffer << iDia;
		buffer << iMaterial;
		buffer << iCode;
		buffer << iName;
		buffer << iStandard;
		buffer << iUnit;
	}

	for (int index = 0; index < mapWorkTypeToRebarInstallCost.size(); index++)
	{
		count = (int)mapWorkTypeToRebarInstallCost[index].size();
		buffer << count;
		for (int i = 0; i < count; i++)
		{
			int iBigCategory = mapWorkTypeToRebarInstallCost[index][i]->m_BigCategory;
			CString iSmallCategory = mapWorkTypeToRebarInstallCost[index][i]->m_SmallCategory;
			CString iCode = mapWorkTypeToRebarInstallCost[index][i]->m_Code;
			CString iName = mapWorkTypeToRebarInstallCost[index][i]->m_Name;
			CString iStandard = mapWorkTypeToRebarInstallCost[index][i]->m_Standard;
			CString iUnit = mapWorkTypeToRebarInstallCost[index][i]->m_Unit;
			buffer << iBigCategory;
			buffer << iSmallCategory;
			buffer << iCode;
			buffer << iName;
			buffer << iStandard;
			buffer << iUnit;
		}
	}
}

void NayukRebarSetting::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);

	ma_RebarMaterialCost.clear();
	mapWorkTypeToRebarInstallCost[0].clear();
	mapWorkTypeToRebarInstallCost[1].clear();

	buffer >> m_nFabricateType;
	int count = 0;
	buffer >> count;
	for (int i = 0; i < count; i++)
	{
		CString iDia = _T("");
		CString iMaterial = _T("");
		CString iCode = _T("");
		CString iName = _T("");
		CString iStandard = _T("");
		CString iUnit = _T("");
		buffer >> iDia;
		buffer >> iMaterial;
		buffer >> iCode;
		buffer >> iName;
		buffer >> iStandard;
		buffer >> iUnit;
		ma_RebarMaterialCost.push_back(new RebarMaterialCost(iDia, iMaterial, iCode, iName, iStandard, iUnit));
	}

	for (int index = 0; index < mapWorkTypeToRebarInstallCost.size(); index++)
	{
		count = 0;
		buffer >> count;
		for (int i = 0; i < count; i++)
		{
			int iBigCategory = 0;
			CString iSmallCategory = _T("");
			CString iCode = _T("");
			CString iName = _T("");
			CString iStandard = _T("");
			CString iUnit = _T("");
			buffer >> iBigCategory;
			buffer >> iSmallCategory;
			buffer >> iCode;
			buffer >> iName;
			buffer >> iStandard;
			buffer >> iUnit;
			mapWorkTypeToRebarInstallCost[index].push_back(new RebarInstallCost(iBigCategory, iSmallCategory, iCode, iName, iStandard, iUnit));
		}
	}
}

void NayukRebarSetting::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_nFabricateType;
		int count = (int)ma_RebarMaterialCost.size();
		ar << count;
		for (int i = 0; i < ma_RebarMaterialCost.size(); i++)
		{
			CString iDia = ma_RebarMaterialCost[i]->m_Dia;
			CString iMaterial = ma_RebarMaterialCost[i]->m_Material;
			CString iCode = ma_RebarMaterialCost[i]->m_Code;
			CString iName = ma_RebarMaterialCost[i]->m_Name;
			CString iStandard = ma_RebarMaterialCost[i]->m_Standard;
			CString iUnit = ma_RebarMaterialCost[i]->m_Unit;
			ar << iDia;
			ar << iMaterial;
			ar << iCode;
			ar << iName;
			ar << iStandard;
			ar << iUnit;
		}

		for (int index = 0; index < mapWorkTypeToRebarInstallCost.size(); index++)
		{
			count = (int)mapWorkTypeToRebarInstallCost[index].size();
			ar << count;
			for (int i = 0; i < mapWorkTypeToRebarInstallCost[index].size(); i++)
			{
				int iBigCategory = mapWorkTypeToRebarInstallCost[index][i]->m_BigCategory;
				CString iSmallCategory = mapWorkTypeToRebarInstallCost[index][i]->m_SmallCategory;
				CString iCode = mapWorkTypeToRebarInstallCost[index][i]->m_Code;
				CString iName = mapWorkTypeToRebarInstallCost[index][i]->m_Name;
				CString iStandard = mapWorkTypeToRebarInstallCost[index][i]->m_Standard;
				CString iUnit = mapWorkTypeToRebarInstallCost[index][i]->m_Unit;
				ar << iBigCategory;
				ar << iSmallCategory;
				ar << iCode;
				ar << iName;
				ar << iStandard;
				ar << iUnit;
			}
		}
	}
	else
	{
		ma_RebarMaterialCost.clear();
		mapWorkTypeToRebarInstallCost[0].clear();
		mapWorkTypeToRebarInstallCost[1].clear();

		ar >> m_nFabricateType;
		int count = 0;
		ar >> count;
		for (int i = 0; i < count; i++)
		{
			CString iDia = _T("");
			CString iMaterial = _T("");
			CString iCode = _T("");
			CString iName = _T("");
			CString iStandard = _T("");
			CString iUnit = _T("");
			ar >> iDia;
			ar >> iMaterial;
			ar >> iCode;
			ar >> iName;
			ar >> iStandard;
			ar >> iUnit;
			ma_RebarMaterialCost.push_back(new RebarMaterialCost(iDia, iMaterial, iCode, iName, iStandard, iUnit));
		}

		for (int index = 0; index < mapWorkTypeToRebarInstallCost.size(); index++)
		{
			count = 0;
			ar >> count;
			for (int i = 0; i < count; i++)
			{
				int iBigCategory = 0;
				CString iSmallCategory = 0;
				CString iCode = _T("");
				CString iName = _T("");
				CString iStandard = _T("");
				CString iUnit = _T("");
				ar >> iBigCategory;
				ar >> iSmallCategory;
				ar >> iCode;
				ar >> iName;
				ar >> iStandard;
				ar >> iUnit;
				mapWorkTypeToRebarInstallCost[index].push_back(new RebarInstallCost(iBigCategory, iSmallCategory, iCode, iName, iStandard, iUnit));
			}
		}
	}
}
