#include "StdAfx.h"
#include "MSQuantityOption.h"
#include "MSCompoG.h"
#include "MSDeformedBar.h"
#include "GMLib/MSVersionInfo.h"
#include "MSBaseBuilding.h"
#include "StructureModel\MSFloor.h"

IMPLEMENT_SERIAL(MSQuantityOption, MSObject, VERSIONABLE_SCHEMA | 1)

MSQuantityOption::MSQuantityOption()
{
	m_aMemberTypeName.clear();
	m_aFactor.clear();

	m_nByOptionOrManual = 0;
	m_bSumSOGAsFooting = true;
	m_bIgnoreAreaColumnBeam = true;
	m_dIgnoreAreaColumnBeam = 1.0;
	m_bIgnoreAreaColumnWall = true;
	m_dIgnoreAreaColumnWall = 1.0;
	m_bIgnoreAreaGirderBeam = false;
	m_dIgnoreAreaGirderBeam = 1.0;

	m_bIgnoreOpening = false;
	m_dIgnoreOpeningOfWall = 1.0;
	m_dIgnoreOpeningOfSlab = 1.0;

	m_nBeamUnderCalcMethod = 1;
	m_bIsSteelQuantityExclude = false;

	m_nApplyBurimConc = 0;
	m_strMaterial = _T("");
	m_dBurimHeight = 60.0;
	m_dBurimOffset = 100.0;
	m_nBackFillMaterial = 0;
	m_bBackFillMatLevelZone = true;
	m_bBackFillMatOffset = true;
	m_bBackFillPIT = true;
	m_bIgnoreAreaVMemberUnderSlab = false;
	m_aAppliedFloorNum.clear();
	m_sBurimAreaType = _T("None");

	m_bRebarVolumeExclude = false; //콘크리트 체적에서 철근체적 공제
	m_nRebarVolumeExcludeType =0; // 0: 철근체적 자동계산 1: 철근중량 / 7.85
	m_dRebarDensityInput =7.85; // 밀도?중량 ? 
	m_bAdditionalConcreteNForm = false;
	m_aAdditioanlConcAndForm.clear();
	m_bConcDeduction = false; //시설물 콘크리트 공제
	m_dConcDeduction = 0.5;
	m_nRebarEmbeddedDeckExtra = 0;//데크여유길이
	m_nGeneralDeckExtra = 0;
	m_nPCSlabExtra = 0;
	m_nTouchSteelBeam = 0;
	m_dHaunchOutstretchedLength = 0; //헌치내민길이
	m_bIsIgnoreAreaWallEndFormWork = true;
}

bool MSQuantityOption::WriteFileBinary(CString strFilePath)
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

	Serialize(ar);

	return true;
}

bool MSQuantityOption::ReadFileBinary(CString strFilePath)
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

	int version;

	ar >> version;
	MSVersionInfo::PushCurrentVersion(version);

	Serialize(ar);

	MSVersionInfo::PopCurrentVersion();

	return true;
}

void MSQuantityOption::WeightFactorDataInitCheck()
{
	if (m_aMemberTypeName.size() > 0) return;

	m_aMemberTypeName.push_back(LocalDic::GetLocal(_T("기둥")));
	m_aMemberTypeName.push_back(LocalDic::GetLocal(_T("벽체")));
	m_aMemberTypeName.push_back(LocalDic::GetLocal(_T("지하옹벽")));
	m_aMemberTypeName.push_back(LocalDic::GetLocal(_T("슬래브")));
	m_aMemberTypeName.push_back(LocalDic::GetLocal(_T("매트 슬래브")));
	m_aMemberTypeName.push_back(LocalDic::GetLocal(_T("독립기초")));
	m_aMemberTypeName.push_back(LocalDic::GetLocal(_T("줄기초")));
	m_aMemberTypeName.push_back(LocalDic::GetLocal(_T("보")));
	m_aMemberTypeName.push_back(LocalDic::GetLocal(_T("계단")));
	m_aMemberTypeName.push_back(LocalDic::GetLocal(_T("피트")));

	m_aFactor.resize(m_aMemberTypeName.size());
}

void MSQuantityOption::GetFactorForMemberType(MSCompoG* compoG, IncreaseFactor& factor)
{
	CString str;
	switch(compoG->GetType())
	{
	case MSElement::msColumnG: str = LocalDic::GetLocal(_T("기둥")); break;
	case MSElement::msWallG: str = (compoG->IsRetainingWall() ? LocalDic::GetLocal(_T("지하옹벽")) : LocalDic::GetLocal(_T("벽체"))); break;
	case MSElement::msWallColumnG: 
	case MSElement::msHandRailG: 
		str = LocalDic::GetLocal(_T("벽체")); break;
	case MSElement::msSlabG: str = (compoG->IsMatSlab() ? LocalDic::GetLocal(_T("매트 슬래브")) : LocalDic::GetLocal(_T("슬래브"))); break;
	case MSElement::msIsoFootG: str = LocalDic::GetLocal(_T("독립기초")); break;
	case MSElement::msWallFootG: str = LocalDic::GetLocal(_T("줄기초")); break;
	case MSElement::msBeamG: str = LocalDic::GetLocal(_T("보")); break;
	case MSElement::msStairG: str = LocalDic::GetLocal(_T("계단")); break;
	case MSElement::msPitG: str = LocalDic::GetLocal(_T("피트")); break;
	default: str = LocalDic::GetLocal(_T("기타")); break;
	}

	WeightFactorDataInitCheck();
	for (int i=0 ; i<m_aFactor.size() ; ++i)
		if (m_aMemberTypeName[i] == str) factor = m_aFactor[i];
}

void MSQuantityOption::Serialize(CArchive &ar)
{
	int burimAreaType = 0;

	MSObject::Serialize(ar);
	if(ar.IsStoring())
	{
		WeightFactorDataInitCheck();

		int count = m_aMemberTypeName.size();
		ar << count;
		for (int i=0 ; i<count ; ++i)
			ar << m_aMemberTypeName[i];
		for (int i=0 ; i<count ; ++i)
		{
			ar << m_aFactor[i].formwork << m_aFactor[i].concrete;
			//ar << m_aFactor[i].d10 << m_aFactor[i].d13 << m_aFactor[i].d16 << m_aFactor[i].d19 << m_aFactor[i].d22 << m_aFactor[i].d25;
			//ar << m_aFactor[i].d29 << m_aFactor[i].d32 << m_aFactor[i].d35 << m_aFactor[i].d38 << m_aFactor[i].d41 << m_aFactor[i].d51;
			int count_sub = m_aFactor[i].factorByDia.size();
			ar << count_sub;
			for (int k = 0; k < count_sub; ++k)
			{
				double factor = m_aFactor[i].factorByDia[k];
				ar << factor;
			}
		}

		ar << m_bSumSOGAsFooting;
		ar << m_nByOptionOrManual << m_bIgnoreAreaColumnBeam << m_dIgnoreAreaColumnBeam << m_bIgnoreAreaColumnWall;
		ar << m_dIgnoreAreaColumnWall << m_bIgnoreAreaGirderBeam << m_dIgnoreAreaGirderBeam;
		ar << m_bIgnoreOpening << m_dIgnoreOpeningOfWall << m_dIgnoreOpeningOfSlab << m_nBeamUnderCalcMethod;

		ar << m_nApplyBurimConc << m_strMaterial << m_dBurimHeight << m_dBurimOffset;
		ar << m_nBackFillMaterial << m_bBackFillMatLevelZone << m_bBackFillMatOffset << m_bBackFillPIT;
		ar << m_bIgnoreAreaVMemberUnderSlab << burimAreaType << m_sBurimAreaType;
		ar << m_bIsSteelQuantityExclude;
		ar << m_bRebarVolumeExclude << m_nRebarVolumeExcludeType << m_dRebarDensityInput;
		ar << m_bAdditionalConcreteNForm;
		//m_aAdditioanlConcAndForm.clear();
		ar << m_bConcDeduction << m_dConcDeduction;
		ar << m_nRebarEmbeddedDeckExtra << m_nGeneralDeckExtra;

		ar << m_nPCSlabExtra;
		ar << m_bIsIgnoreAreaWallEndFormWork;
		
		int floorCount = m_aAppliedFloorNum.size();
		ar << floorCount;
		for (int i = 0; i < floorCount; ++i)
			ar << m_aAppliedFloorNum[i];

		ar << m_dHaunchOutstretchedLength;
	}
	else 
	{
		int count = 0;
		ar >> count;
		m_aMemberTypeName.resize(count);
		m_aFactor.resize(count);
		for (int i=0 ; i<count ; ++i)
			ar >> m_aMemberTypeName[i];

		m_aMemberTypeName.clear();
		WeightFactorDataInitCheck();

		for (int i=0 ; i<count ; ++i)
		{
			ar >> m_aFactor[i].formwork >> m_aFactor[i].concrete;
			/*
			ar >> m_aFactor[i].d10 >> m_aFactor[i].d13 >> m_aFactor[i].d16 >> m_aFactor[i].d19 >> m_aFactor[i].d22 >> m_aFactor[i].d25;
			if (MSVersionInfo::GetCurrentVersion() >= 20141112)
			{
			ar >> m_aFactor[i].d29 >> m_aFactor[i].d32 >> m_aFactor[i].d35 >> m_aFactor[i].d38 >> m_aFactor[i].d41 >> m_aFactor[i].d51;
			}
			*/
			int count_sub = 0;
			if (MSVersionInfo::GetCurrentVersion() >= 20150728)
				ar >> count_sub;
			else
			{
				if (MSVersionInfo::GetCurrentVersion() >= 20141112)
					count_sub = 12;
				else
					count_sub = 6;
			}

			for (int k = 0; k < count_sub; ++k)
			{
				double factor = 0;
				ar >> factor;
				if (k < m_aFactor[i].factorByDia.size())
					m_aFactor[i].factorByDia[k] = factor;
			}
			
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20150821)
		{
			ar >> m_bSumSOGAsFooting;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20141113)
		{
			ar >> m_nByOptionOrManual >> m_bIgnoreAreaColumnBeam >> m_dIgnoreAreaColumnBeam >> m_bIgnoreAreaColumnWall;
			ar >> m_dIgnoreAreaColumnWall >> m_bIgnoreAreaGirderBeam >> m_dIgnoreAreaGirderBeam;
			ar >> m_bIgnoreOpening >> m_dIgnoreOpeningOfWall >> m_dIgnoreOpeningOfSlab >> m_nBeamUnderCalcMethod;

			CString temp_material;
			ar >> m_nApplyBurimConc >> temp_material >> m_dBurimHeight >> m_dBurimOffset;
			ar >> m_nBackFillMaterial >> m_bBackFillMatLevelZone >> m_bBackFillMatOffset >> m_bBackFillPIT;

			MSMaterial* findMaterial = mp_owner->GetMaterialbyName(MSMaterial::Type_MSMaterial::msConcrete, temp_material);
			if (findMaterial != nullptr)
				m_strMaterial = temp_material;
			else
			{
				findMaterial = mp_owner->GetDefMaterial(MSMaterial::Type_MSMaterial::msConcrete);
				m_strMaterial = findMaterial->GetName();
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20160314)
		{
			ar >> m_bIgnoreAreaVMemberUnderSlab;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20160624)
		{
			ar >> burimAreaType;
		}

		if (m_nByOptionOrManual == 1)
			m_nByOptionOrManual = 0;

		if (MSVersionInfo::GetCurrentVersion() >= 20160905)
		{
			ar >> m_sBurimAreaType;
		}
		else
		{
			if (burimAreaType > 0)
			{
				CStringArray defaultFormTypeNames;
				long numFormWork = MSFormWorkTypeManager::GetKorDefaultFormTypeNames(defaultFormTypeNames);
				m_sBurimAreaType = defaultFormTypeNames[burimAreaType - 1];
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20160905 && MSVersionInfo::GetCurrentVersion() < 20170522)
		{
			if (m_sBurimAreaType == _T(""))
				m_sBurimAreaType = _T("None");
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20161209)
		{
			ar >> m_bIsSteelQuantityExclude;
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20180308)
		{
			ar >> m_bRebarVolumeExclude >> m_nRebarVolumeExcludeType >> m_dRebarDensityInput;
			ar >> m_bAdditionalConcreteNForm;
			//m_aAdditioanlConcAndForm.clear();
			ar >> m_bConcDeduction >> m_dConcDeduction;
		}

		if (IsMVAddToBHA())
		{
			if (MSVersionInfo::GetCurrentVersion() >= 20210909)
			{
				ar >> m_nRebarEmbeddedDeckExtra;
				ar >> m_nGeneralDeckExtra;
				ar >> m_nPCSlabExtra;
			}
			else
			{
				int tempValue = 0;
				if (MSVersionInfo::GetCurrentVersion() >= 20180416)
				{
					ar >> m_nRebarEmbeddedDeckExtra >> tempValue >> m_nGeneralDeckExtra >> tempValue;
				}
				if (MSVersionInfo::GetCurrentVersion() >= 20190822)
				{
					ar >> m_nPCSlabExtra >> tempValue;
				}
			}
			if (MSVersionInfo::GetCurrentVersion() >= 20200706)
			{
				ar >> m_bIsIgnoreAreaWallEndFormWork;
			}
			if (MSVersionInfo::GetCurrentVersion() >= 20201119)
			{
				int floorCount = 0;
				ar >> floorCount;
				if (floorCount > 0)
				{

					vector<CString> aAppliedFloorNum;
					if (MSVersionInfo::GetCurrentVersion() < 20211129)
					{
						aAppliedFloorNum.resize(floorCount);
						for (int i = 0; i < floorCount; ++i)
							ar >> aAppliedFloorNum[i];

						MakeAppliedFloorNum();
					}
					else
					{
						m_aAppliedFloorNum.resize(floorCount);
						for (int i = 0; i < floorCount; ++i)
							ar >> m_aAppliedFloorNum[i];
					}
				}
				else
				{
					MakeAppliedFloorNum();
				}
			}
		}

		if (IsBHQorBHCEMode() && (MSVersionInfo::GetCurrentVersion() >= 20230823) || IsBHForBHAMode() && MSVersionInfo::GetCurrentVersion() >= 20240307)
			ar >> m_dHaunchOutstretchedLength;
	}
}

void IncreaseFactor::Init()
{
	formwork = concrete = 1.0;
	
	factorByDia.clear();
	CStringArray rebarList; MSDeformedBar::GetRebarNameList(rebarList);
	int num = rebarList.GetSize();
	for (int k = 0; k < num; ++k)
		factorByDia.push_back(1.0);
};

void AdditionalConcAndForm::Init()
{
	//MSFormWorkTypeManager::GetFormWorkTypeListAllStatic();
}

void MSQuantityOption::SetBuilding(MSBaseBuilding* bldg)
{
	mp_owner = bldg;
}

void MSQuantityOption::MakeAppliedFloorNum()
{
	if (mp_owner != nullptr)
	{
		vector<MSFloor*> floors = mp_owner->GetAllFloors();
		m_aAppliedFloorNum.clear();
		for (int i = 0; i < floors.size(); i++)
		{
			m_aAppliedFloorNum.push_back(floors[i]->GetFloorNum());
		}
	}
}