#pragma once
#include "stdafx.h"
#include "MSBuildingInfoManager.h"
#include "MSBuildingInfo.h"
#include "MSBuildingInfoColorData.h"
#include "MSBuildingClassificationData.h"
#include "..\GMLib\MSVersionInfo.h"

IMPLEMENT_SERIAL(MSBuildingInfoManager, MSObject, VERSIONABLE_SCHEMA | 1)
MSBuildingInfoManager::MSBuildingInfoManager()
{
	mm_BuildingInfo.clear();
	mm_BuildingUsageColor.clear();
	mm_BuildingNameColor.clear();
}

MSBuildingInfoManager::~MSBuildingInfoManager()
{
	DeleteBuildingInfoMap();
	RemoveAllBuildingClassification();
}

void MSBuildingInfoManager::Serialize(CArchive& ar)
{
	MSObject::Serialize(ar);
	int count = 0;
	if (ar.IsStoring())
	{
		count = (int)mm_BuildingInfo.size();
		ar << count;
		for (auto it = mm_BuildingInfo.begin(); it != mm_BuildingInfo.end(); it++)
			it->second->Serialize(ar);

		count = (int)mm_BuildingUsageColor.size();
		ar << count;
		for (auto it = mm_BuildingUsageColor.begin(); it != mm_BuildingUsageColor.end(); it++)
		{
			ar << it->first;
			it->second->Serialize(ar);
		}
			

		count = (int)mm_BuildingNameColor.size();
		ar << count;
		for (auto it = mm_BuildingNameColor.begin(); it != mm_BuildingNameColor.end(); it++)
		{
			ar << it->first;
			it->second->Serialize(ar);
		}

		count = (int)m_BuildingClassificationArr.size();
		ar << count;
		for (int idx = 0; idx < count; idx++)
			m_BuildingClassificationArr[idx]->Serialize(ar);
	}
	else
	{
		ar >> count;
		for (int i = 0; i < count; i++)
		{
			auto calculationArea = new MSBuildingInfo(this);
			calculationArea->Serialize(ar);
			Add(calculationArea, false);
		}

		CString name = _T("");

		ar >> count;
		for (int i = 0; i < count; i++)
		{
			ar >> name;

			auto calculationArea = new MSBuildingInfoColorData();
			calculationArea->Serialize(ar);
			mm_BuildingUsageColor.insert(make_pair(name, calculationArea));
		}

		ar >> count;
		for (int i = 0; i < count; i++)
		{
			ar >> name;

			auto calculationArea = new MSBuildingInfoColorData();
			calculationArea->Serialize(ar);
			mm_BuildingNameColor.insert(make_pair(name, calculationArea));
		}

		if (MSVersionInfo::GetCurrentVersion() < 20230110)
		{
			if (m_BuildingClassificationArr.size() == 0)
				SetDefMSBuildingClassification();

			UpdateBuildingInfo();
		}
		else
		{
			ar >> count;
			for (int idx = 0; idx < count; idx++)
			{
				auto msBuildingClassification = new MSBuildingClassificationData();
				msBuildingClassification->Serialize(ar);
				m_BuildingClassificationArr.push_back(msBuildingClassification);
			}
		}

		BuildingInfoColorValideCheck();
	}
}

MSBuildingInfo* MSBuildingInfoManager::GetDefaultBuildingInfo()
{
	auto it = mm_BuildingInfo.begin();
	return it->second;
}

MSBuildingInfo* MSBuildingInfoManager::GetBuildingInfoByID(long Id)
{
	auto it = mm_BuildingInfo.find(Id);
	if (it != mm_BuildingInfo.end())	return it->second;
	return nullptr;
}

MSBuildingInfo* MSBuildingInfoManager::GetBuildingInfo(CString infoName)
{
	for (auto it = mm_BuildingInfo.begin(); it != mm_BuildingInfo.end(); it++)
	{
		MSBuildingInfo* buildingInfo = it->second;

		if (buildingInfo->GetName() == infoName)
			return buildingInfo;
	}

	return nullptr;
}

void MSBuildingInfoManager::Add(MSBuildingInfo* buildingInfo, bool IsColorDataAdd)
{
	mm_BuildingInfo.insert(make_pair(buildingInfo->m_ID, buildingInfo));

	if(IsColorDataAdd == true)
		ColorDataAdd(buildingInfo);
}

void MSBuildingInfoManager::Remove(long Id)
{
	CString buildingInfoType = _T(""), buildingName = _T("");
	bool bUsageUsed = false, bNameUsed = false;

	auto it = mm_BuildingInfo.find(Id);
	if (it != mm_BuildingInfo.end())
	{
		buildingInfoType = it->second->GetBuildingInfoType();
		buildingName = it->second->GetName();

		mm_BuildingInfo.erase(it);

		for (auto it = mm_BuildingInfo.begin(); it != mm_BuildingInfo.end(); it++)
		{
			MSBuildingInfo* buildingInfo = it->second;
			if (buildingInfo->GetBuildingInfoType() == buildingInfoType) bUsageUsed = true;
			if (buildingInfo->GetName() == buildingName) bNameUsed = true;
		}
	}

	if (bUsageUsed == false)
	{
		auto it = mm_BuildingUsageColor.find(buildingInfoType);
		if (it != mm_BuildingUsageColor.end())
		{
			mm_BuildingUsageColor.erase(it);
		}
	}

	if (bNameUsed == false)
	{
		auto it = mm_BuildingNameColor.find(buildingName);
		if (it != mm_BuildingNameColor.end())
		{
			mm_BuildingNameColor.erase(it);
		}
	}

	return;
}

void MSBuildingInfoManager::DeleteBuildingInfoMap()
{
	for (auto it = mm_BuildingInfo.begin(); it != mm_BuildingInfo.end(); it++)
		delete it->second;
	mm_BuildingInfo.clear();

	mm_BuildingUsageColor.clear();
	mm_BuildingNameColor.clear();
}

vector<MSBuildingInfo*> MSBuildingInfoManager::GetBuildingInfoList()
{
	vector<MSBuildingInfo*> templateList;
	for (auto it = mm_BuildingInfo.begin(); it != mm_BuildingInfo.end(); it++)
		templateList.push_back(it->second);
	return templateList;
}

void MSBuildingInfoManager::CreateDefaultBuildingInfoAndAdd(CString sBuildingName, CString sName)
{
	DeleteBuildingInfoMap();

	MSBuildingInfo* buildingInfo = new MSBuildingInfo(this);
	buildingInfo->GetID();
	buildingInfo->SetName(sName);

	if (m_BuildingClassificationArr.size() == 0)
		SetDefMSBuildingClassification();
	buildingInfo->SetBuildingClassification(m_BuildingClassificationArr[0]);
	buildingInfo->SetBuildingInfoType(LocalDic::GetLocal(_T("아파트")));

	Add(buildingInfo);
}

vector<CString> MSBuildingInfoManager::GetBuildingUsageList()
{
	vector<CString> buildingUsageList;
	for (auto it = mm_BuildingUsageColor.begin(); it != mm_BuildingUsageColor.end(); it++)
		buildingUsageList.push_back(it->first);
	return buildingUsageList;
}

vector<CString> MSBuildingInfoManager::GetBuildingNameList()
{
	vector<CString> buildingNameList;
	for (auto it = mm_BuildingNameColor.begin(); it != mm_BuildingNameColor.end(); it++)
		buildingNameList.push_back(it->first);
	return buildingNameList;
}

void MSBuildingInfoManager::ColorDataAdd(MSBuildingInfo* buildingInfo)
{
	auto itUsage = mm_BuildingUsageColor.find(buildingInfo->GetBuildingInfoType());
	if (itUsage == mm_BuildingUsageColor.end())
	{
		int nColorIndex = mm_BuildingUsageColor.size();
		mm_BuildingUsageColor.insert(make_pair(buildingInfo->GetBuildingInfoType(), CreateColorByIndex(nColorIndex)));
	}

	auto itName = mm_BuildingNameColor.find(buildingInfo->GetName());
	if (itName == mm_BuildingNameColor.end())
	{
		int nColorIndex = mm_BuildingNameColor.size();
		mm_BuildingNameColor.insert(make_pair(buildingInfo->GetName(), CreateColorByIndex(nColorIndex)));
	}
}

void MSBuildingInfoManager::SetBuildingUsageChange(CString oldUsage, CString newUsage)
{
	bool bOldUsageUsed = false;

	for (auto it = mm_BuildingInfo.begin(); it != mm_BuildingInfo.end(); it++)
	{
		MSBuildingInfo* buildingInfo = it->second;
		if (buildingInfo->GetBuildingInfoType() != oldUsage) continue;

		bOldUsageUsed = true;
		break;
	}

	MSBuildingInfoColorData* newColorData = nullptr;
	if (bOldUsageUsed == false)
	{
		newColorData = (MSBuildingInfoColorData*)mm_BuildingUsageColor[oldUsage]->Copy();
		mm_BuildingUsageColor.erase(oldUsage);
	}
	else
	{
		int nColorIndex = mm_BuildingUsageColor.size();
		newColorData = CreateColorByIndex(nColorIndex);
	}

	auto itColor = mm_BuildingUsageColor.find(newUsage);
	if (itColor == mm_BuildingUsageColor.end())
	{
		mm_BuildingUsageColor.insert(make_pair(newUsage, newColorData));
	}
}

void MSBuildingInfoManager::SetBuildingNameChange(CString oldName, CString newName)
{
	bool bOldNameUsed = false;

	for (auto it = mm_BuildingInfo.begin(); it != mm_BuildingInfo.end(); it++)
	{
		MSBuildingInfo* buildingInfo = it->second;
		if (buildingInfo->GetName() != oldName) continue;

		bOldNameUsed = true;
		break;
	}

	MSBuildingInfoColorData* newColorData = nullptr;
	if (bOldNameUsed == false)
	{
		newColorData = (MSBuildingInfoColorData*)mm_BuildingNameColor[oldName]->Copy();
		mm_BuildingNameColor.erase(oldName);
	}
	else
	{
		int nColorIndex = mm_BuildingNameColor.size();
		newColorData = CreateColorByIndex(nColorIndex);
	}

	auto itColor = mm_BuildingNameColor.find(newName);
	if (itColor == mm_BuildingNameColor.end())
	{
		mm_BuildingNameColor.insert(make_pair(newName, newColorData));
	}
}

MSBuildingInfoColorData* MSBuildingInfoManager::CreateColorByIndex(int index)
{
	int r, g, b;

	if (index == 0) { r = 255; g = 255; b = 0;	}
	else if (index == 1) { r = 0; g = 255; b = 255; }
	else if (index == 2) { r = 255; g = 128; b = 0; }
	else if (index == 3) { r = 0; g = 255; b = 0; }
	else if (index == 4) { r = 0; g = 0; b = 191; }
	else if (index == 5) { r = 255; g = 0; b = 0; }
	else if (index == 6) { r = 235; g = 0; b = 255; }
	else if (index == 7) { r = 255; g = 191; b = 0; }
	else if (index == 8) { r = 192; g = 192; b = 192; }
	else if (index == 9) { r = 128; g = 64; b = 64; }
	else if (index == 10) { r = 170; g = 0; b = 255; }
	else if (index == 11) { r = 255; g = 255; b = 255; }
	else if (index == 12) { r = 191; g = 191; b = 255; }
	else if (index == 13) { r = 105; g = 105; b = 105; }
	else if (index == 14) { r = 63; g = 191; b = 191; }
	else if (index == 15) { r = 128; g = 128; b = 0; }
	else if (index == 16) { r = 127; g = 255; b = 191; }
	else if (index == 17) { r = 255; g = 128; b = 128; }
	else if (index == 18) { r = 255; g = 103; b = 201; }
	else if (index == 19) { r = 255; g = 255; b = 200; }
	else if (index == 20) { r = 0; g = 128; b = 255; }
	else if (index == 21) { r = 160; g = 0; b = 0; }
	else if (index == 22) { r = 170; g = 255; b = 170; }
	else if (index == 23) { r = 238; g = 130; b = 238; }
	else { r = 0; g = 0; b = 0; } //if (index >= 24)

	return new MSBuildingInfoColorData(255, r, g, b, true);
}

MSBuildingInfoColorData* MSBuildingInfoManager::GetUsageColor(CString usage)
{
	MSBuildingInfoColorData* buildingInfoColorData = nullptr;

	auto it = mm_BuildingUsageColor.find(usage);
	if (it != mm_BuildingUsageColor.end())
	{
		buildingInfoColorData = it->second;
	}

	return buildingInfoColorData;
}

MSBuildingInfoColorData* MSBuildingInfoManager::GetNameColor(CString usage)
{
	MSBuildingInfoColorData* buildingInfoColorData = nullptr;

	auto it = mm_BuildingNameColor.find(usage);
	if (it != mm_BuildingNameColor.end())
	{
		buildingInfoColorData = it->second;
	}

	return buildingInfoColorData;
}

void MSBuildingInfoManager::BuildingInfoColorValideCheck()
{
	vector<CString> buildingInfoCurUsageList, buildingInfoCurNameList;

	for (auto it = mm_BuildingInfo.begin(); it != mm_BuildingInfo.end(); it++)
	{
		MSBuildingInfo* buildingInfo = it->second;

		if (std::find(buildingInfoCurUsageList.begin(), buildingInfoCurUsageList.end(), buildingInfo->GetBuildingInfoType()) == buildingInfoCurUsageList.end())
		{
			buildingInfoCurUsageList.push_back(buildingInfo->GetBuildingInfoType());
		}

		if (std::find(buildingInfoCurNameList.begin(), buildingInfoCurNameList.end(), buildingInfo->GetName()) == buildingInfoCurNameList.end())
		{
			buildingInfoCurNameList.push_back(buildingInfo->GetName());
		}
	}

	bool IsColorDataReset = false;

	if (buildingInfoCurUsageList.size() != mm_BuildingUsageColor.size())
	{
		IsColorDataReset = true;
	}
	else
	{
		for (int index = 0; index < buildingInfoCurUsageList.size(); index++)
		{
			if (GetUsageColor(buildingInfoCurUsageList[index]) != nullptr) continue;

			IsColorDataReset = true;
			break;
		}
	}
	
	if (IsColorDataReset == true)
	{
		mm_BuildingUsageColor.clear();
		for (int index = 0; index < buildingInfoCurUsageList.size(); index++)
		{
			mm_BuildingUsageColor.insert(make_pair(buildingInfoCurUsageList[index], CreateColorByIndex(index)));
		}

		IsColorDataReset = false;
	}

	if (buildingInfoCurNameList.size() != mm_BuildingNameColor.size())
	{
		IsColorDataReset = true;
	}
	else
	{
		for (int index = 0; index < buildingInfoCurNameList.size(); index++)
		{
			if (GetNameColor(buildingInfoCurNameList[index]) != nullptr) continue;

			IsColorDataReset = true;
			break;
		}
	}

	if (IsColorDataReset == true)
	{
		mm_BuildingNameColor.clear();
		for (int index = 0; index < buildingInfoCurNameList.size(); index++)
		{
			mm_BuildingNameColor.insert(make_pair(buildingInfoCurNameList[index], CreateColorByIndex(index)));
		}
	}
}

void MSBuildingInfoManager::SetDefMSBuildingClassification()
{
	RemoveAllBuildingClassification();

	// 건물 유형, 건물 분류
	// bldgType, bldgClassification
	AddMSBuildingClassification(LocalDic::GetLocal(_T("아파트")), LocalDic::GetLocal(_T("동")));
	AddMSBuildingClassification(LocalDic::GetLocal(_T("지하주차장")), LocalDic::GetLocal(_T("지하주차장")));
	AddMSBuildingClassification(LocalDic::GetLocal(_T("근생주차장")), LocalDic::GetLocal(_T("지하주차장")));
	AddMSBuildingClassification(LocalDic::GetLocal(_T("주민공동시설")), LocalDic::GetLocal(_T("부속동")));
	AddMSBuildingClassification(LocalDic::GetLocal(_T("어린이집")), LocalDic::GetLocal(_T("부속동")));
	AddMSBuildingClassification(LocalDic::GetLocal(_T("도서관")), LocalDic::GetLocal(_T("부속동")));
	AddMSBuildingClassification(LocalDic::GetLocal(_T("관리사무소")), LocalDic::GetLocal(_T("부속동")));
	AddMSBuildingClassification(LocalDic::GetLocal(_T("경비실")), LocalDic::GetLocal(_T("부속동")));
	AddMSBuildingClassification(LocalDic::GetLocal(_T("근린생활시설")), LocalDic::GetLocal(_T("부속동")));
	AddMSBuildingClassification(LocalDic::GetLocal(_T("경로당")), LocalDic::GetLocal(_T("부속동")));
	AddMSBuildingClassification(LocalDic::GetLocal(_T("주민운동시설")), LocalDic::GetLocal(_T("부속동")));
	AddMSBuildingClassification(LocalDic::GetLocal(_T("게스트하우스")), LocalDic::GetLocal(_T("부속동")));

}

MSBuildingClassificationData* MSBuildingInfoManager::AddMSBuildingClassification(CString bldgType, CString bldgClassification)
{
	for (int idx = 0; idx < m_BuildingClassificationArr.size(); idx++)
	{
		if (m_BuildingClassificationArr[idx]->GetBuildingType() == bldgType)
			return nullptr;
	}
	
	if (bldgType == _T("") || bldgClassification == _T(""))
		return nullptr;

	MSBuildingClassificationData* pNewBuildingClassification = new MSBuildingClassificationData(bldgType, bldgClassification);

	pNewBuildingClassification->GetID();
	m_BuildingClassificationArr.push_back(pNewBuildingClassification);
	
	return pNewBuildingClassification;
}

bool MSBuildingInfoManager::RemoveBuildingClassification(long Id)
{
	if (m_BuildingClassificationArr.size() == 1)
		return false;

	for (auto it = mm_BuildingInfo.begin(); it != mm_BuildingInfo.end(); it++)
	{
		MSBuildingInfo* buildingInfo = it->second;

		if (buildingInfo->GetMSBuildingClassificationID() == Id)
			return false;
	}

	for (int idx = 0; idx < m_BuildingClassificationArr.size(); idx++)
	{
		if (m_BuildingClassificationArr[idx]->GetID() == Id)
		{
			delete m_BuildingClassificationArr[idx];
			m_BuildingClassificationArr.erase(m_BuildingClassificationArr.begin() + idx);
			return true;
		}
	}
	return false;
}

void MSBuildingInfoManager::RemoveAllBuildingClassification()
{
	for (int idx = 0; idx < m_BuildingClassificationArr.size(); idx++)
		delete m_BuildingClassificationArr[idx];

	m_BuildingClassificationArr.clear();
}


MSBuildingClassificationData* MSBuildingInfoManager::GetBuildingClassification(long Id)
{
	for (int idx = 0; idx < m_BuildingClassificationArr.size(); idx++)
	{
		if (m_BuildingClassificationArr[idx]->GetID() == Id)
			return m_BuildingClassificationArr[idx];
	}

	return nullptr;
}

MSBuildingClassificationData* MSBuildingInfoManager::GetBuildingClassification(CString buildingType)
{
	for (int idx = 0; idx < m_BuildingClassificationArr.size(); idx++)
	{
		if (m_BuildingClassificationArr[idx]->GetBuildingType() == buildingType)
			return m_BuildingClassificationArr[idx];
	}

	return nullptr;
}


CString MSBuildingInfoManager::GetBuildingType(CString buildingClassification)
{
	for (int idx = 0; idx < m_BuildingClassificationArr.size(); idx++)
	{
		if (m_BuildingClassificationArr[idx]->GetBuildingClassification() == buildingClassification)
			return m_BuildingClassificationArr[idx]->GetBuildingType();
	}

	return _T("");
}

vector<CString> MSBuildingInfoManager::GetBuildingTypeList()
{
	vector<CString> buildingTypeArr;
	for (int idx = 0; idx < m_BuildingClassificationArr.size(); idx++)
		buildingTypeArr.push_back(m_BuildingClassificationArr[idx]->GetBuildingType());

	return buildingTypeArr;
}

vector<CString> MSBuildingInfoManager::GetBuildingClassificationList()
{
	vector<CString> buildingClassificationArr;
	buildingClassificationArr.push_back(LocalDic::GetLocal(_T("동")));
	buildingClassificationArr.push_back(LocalDic::GetLocal(_T("지하주차장")));
	buildingClassificationArr.push_back(LocalDic::GetLocal(_T("부속동")));

	for (int idx = 0; idx < m_BuildingClassificationArr.size(); idx++)
		buildingClassificationArr.push_back(m_BuildingClassificationArr[idx]->GetBuildingClassification());

	sort(buildingClassificationArr.begin(), buildingClassificationArr.end(),
		[](const CString& a, const CString& b)->bool {return a < b; }
	);

	vector<CString>::iterator new_end;
	new_end = unique(buildingClassificationArr.begin(), buildingClassificationArr.end());
	buildingClassificationArr.erase(new_end, buildingClassificationArr.end());

	return buildingClassificationArr;
}

vector<MSBuildingClassificationData*> MSBuildingInfoManager::GetMSBuildingClassificationArr()
{
	return m_BuildingClassificationArr;
}

void MSBuildingInfoManager::UpdateBuildingInfo()
{
	// 용도를 건물 분류타입으로 변경 
	// 건물 분류::건물 유형 = 용도
	// 건물 분류::건물 분류 = 동 을 기본값으로 변경함

	map<CString, vector<MSBuildingInfo*>> strToBuildingInfoArrMap;

	for (auto it = mm_BuildingInfo.begin() ; it != mm_BuildingInfo.end() ; it++)
	{
		MSBuildingInfo* buildingInfo = it->second;

		CString strUsage = buildingInfo->GetBuildingInfoType();
		if (strUsage == _T(""))
			strUsage = buildingInfo->GetUsage();
		
		if (strUsage == _T(""))
			strUsage = LocalDic::GetLocal(_T("아파트"));

		MSBuildingClassificationData* pFindMSBuildingClassification = GetBuildingClassification(strUsage);
		if (pFindMSBuildingClassification == nullptr)
		{
			AddMSBuildingClassification(strUsage, LocalDic::GetLocal(_T("동")));
			pFindMSBuildingClassification = GetBuildingClassification(strUsage);
		}
		buildingInfo->SetBuildingClassification(pFindMSBuildingClassification);

		CString buildingInfoName = buildingInfo->GetName();
		auto findIt = strToBuildingInfoArrMap.find(buildingInfoName);
		if (findIt == strToBuildingInfoArrMap.end())
		{
			vector<MSBuildingInfo*> msBuildingInfoArr;
			strToBuildingInfoArrMap.insert(make_pair(buildingInfo->GetName(), msBuildingInfoArr));
		}
		strToBuildingInfoArrMap[buildingInfoName].push_back(buildingInfo);
	}

	for (auto it = strToBuildingInfoArrMap.begin(); it != strToBuildingInfoArrMap.end(); it++)
	{
		int nArrCnt = it->second.size();
		for (int idx = 0; idx < nArrCnt; idx++)
		{
			if (idx == 0) continue;
			MSBuildingInfo* pMSBuildingInfo = it->second[idx];

			CString strName;
			strName.Format(_T("%s_%d"), it->first, idx);
			pMSBuildingInfo->SetName(strName);
		}
	}
}