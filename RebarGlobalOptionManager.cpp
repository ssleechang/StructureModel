#include "StdAfx.h"
#include "RebarGlobalOptionManager.h"
#include "RebarGlobalOption.h"
#include "OrderLengthSettingSet.h"
#include "OrderLengthSet.h"
#include "CorrectiveLengthSet.h"
#include "CoverLengthSet.h"
#include "DevSpliceLengthSet.h"
#include "..\GMLib\MSVersionInfo.h"
#include "..\RADFCore_CLI\RadfContextWrapper.h"

using namespace RebarOption;
RebarGlobalOptionManager::RebarGlobalOptionManager(void)
{
	Initialize();
}


RebarGlobalOptionManager::~RebarGlobalOptionManager(void)
{
}


void RebarGlobalOptionManager::Serialize(CArchive &ar, CString sCurrentOption)
{
	if(ar.IsStoring())
	{
		ar << m_sCurrentOption;

		ar << (int)m_aOrderLengthSets.size();
		for(int i=0; i< m_aOrderLengthSets.size(); i++)
		{
			m_aOrderLengthSets[i]->Serialize(ar);
		}	

		ar << (int)m_aOrderLengthSettingSets.size();
		for(int i=0; i< m_aOrderLengthSettingSets.size(); i++)
		{
			m_aOrderLengthSettingSets[i]->Serialize(ar);
		}	

		ar << (int)m_aDevSpliceLengthSets.size();
		for(int i=0; i< m_aDevSpliceLengthSets.size(); i++)
		{
			m_aDevSpliceLengthSets[i]->Serialize(ar);
		}

		ar << (int)m_aCoverLengthSets.size();
		for(int i=0; i< m_aCoverLengthSets.size(); i++)
		{
			m_aCoverLengthSets[i]->Serialize(ar);
		}	

		ar << (int)m_aCorrectiveLengthSets.size();
		for(int i=0; i< m_aCorrectiveLengthSets.size(); i++)
		{
			m_aCorrectiveLengthSets[i]->Serialize(ar);
		}	

		ar << (int)m_aGlobalOptions.size();
		for(int i=0; i< m_aGlobalOptions.size(); i++)
		{
			m_aGlobalOptions[i]->Serialize(ar);
		}	
	}
	else
	{
		if (sCurrentOption == _T(""))
			ar >> m_sCurrentOption;
		else
			m_sCurrentOption = sCurrentOption;
		
		int numArr;

		ar >> numArr;
		m_aOrderLengthSets.clear();
		for(int i=0; i< numArr; i++)
		{
			auto orderLengthSet = make_shared<OrderLengthSet>();
			orderLengthSet->Serialize(ar);
			m_aOrderLengthSets.push_back(orderLengthSet);
		}

		ar >> numArr;
		m_aOrderLengthSettingSets.clear();
		for(int i=0; i< numArr; i++)
		{
			auto orderLengthSetting = make_shared<OrderLengthSettingSet>();
			orderLengthSetting->Serialize(ar);
			shared_ptr<OrderLengthSet> set = GetOrderLengthOption(orderLengthSetting->m_sDefaultOrderLengthSetName);
			if (set != nullptr) orderLengthSetting->SetDefaultOrderLengthSet(set);
			m_aOrderLengthSettingSets.push_back(orderLengthSetting);
		}

		ar >> numArr;
		m_aDevSpliceLengthSets.clear();
		for(int i=0; i< numArr; i++)
		{
			auto DevelopmentSpliceLength = make_shared<DevSpliceLengthSet>();
			DevelopmentSpliceLength->Serialize(ar);
			m_aDevSpliceLengthSets.push_back(DevelopmentSpliceLength);
		}

		ar >> numArr;
		m_aCoverLengthSets.clear();
		for(int i=0; i< numArr; i++)
		{
			auto coverLength = make_shared<CoverLengthSet>();
			coverLength->Serialize(ar);
			m_aCoverLengthSets.push_back(coverLength);
		}

		ar >> numArr;
		m_aCorrectiveLengthSets.clear();
		for(int i=0; i< numArr; i++)
		{
			auto correctiveLength = make_shared<CorrectiveLengthSet>();
			correctiveLength->Serialize(ar);
			m_aCorrectiveLengthSets.push_back(correctiveLength);
		}

		ar >> numArr;
		m_aGlobalOptions.clear();
		for(int i=0; i< numArr; i++)
		{
			auto option = make_shared<RebarGlobalOption>(L"",this);
			option->Serialize(ar);
			m_aGlobalOptions.push_back(option);
		}
	}
}

void RebarGlobalOptionManager::SelectOptionSerialize(CArchive &ar, CString selectOptionName)
{
	if (ar.IsStoring())
	{
		shared_ptr<RebarGlobalOption> rebarGlobalOption = GetGlobalOption(selectOptionName);

		shared_ptr<OrderLengthSet> orderLengthOption = rebarGlobalOption->GetOrderLengthOption();
		orderLengthOption->Serialize(ar);

		shared_ptr<CorrectiveLengthSet> correctiveLengthOption = rebarGlobalOption->GetCorrectiveLengthOption();
		correctiveLengthOption->Serialize(ar);

		shared_ptr<CoverLengthSet> coverLengthOption = rebarGlobalOption->GetCoverLengthOption();
		coverLengthOption->Serialize(ar);

		shared_ptr<DevSpliceLengthSet> developmentSpliceLengthOption = rebarGlobalOption->GetDevelopmentSpliceLengthSettingOption();
		developmentSpliceLengthOption->Serialize(ar);

		ar << rebarGlobalOption->m_sName;
	}
	else
	{
		//주문길이
		shared_ptr<OrderLengthSet> orderLengthSet = make_shared<OrderLengthSet>();
		orderLengthSet->Serialize(ar);

		shared_ptr<OrderLengthSet> findOrderLengthOption = GetOrderLengthOption(orderLengthSet->m_sName);
		if(findOrderLengthOption != NULL)
		{
			CString orderLengthName;

			int index = 1;
			orderLengthName.Format(_T("%s(%d)"), orderLengthSet->m_sName, index++);

			while (true)
			{
				if (GetOrderLengthOption(orderLengthName) == NULL)
					break;

				orderLengthName.Format(_T("%s(%d)"), orderLengthSet->m_sName, index++);
			}

			orderLengthSet->m_sName = orderLengthName;
		}
		m_aOrderLengthSets.push_back(orderLengthSet);

		//보정길이
		shared_ptr<CorrectiveLengthSet> correctiveLengthSet = make_shared<CorrectiveLengthSet>();
		correctiveLengthSet->Serialize(ar);

		shared_ptr<CorrectiveLengthSet> findCorrectiveLengthOption = GetCorrectiveLengthOption(correctiveLengthSet->m_sName);
		if (findCorrectiveLengthOption != NULL)
		{
			CString correctiveLengthName;

			int index = 1;
			correctiveLengthName.Format(_T("%s(%d)"), correctiveLengthSet->m_sName, index++);

			while (true)
			{
				if (GetCorrectiveLengthOption(correctiveLengthName) == NULL)
					break;

				correctiveLengthName.Format(_T("%s(%d)"), correctiveLengthSet->m_sName, index++);
			}

			correctiveLengthSet->m_sName = correctiveLengthName;
		}
		m_aCorrectiveLengthSets.push_back(correctiveLengthSet);

		//피복
		shared_ptr<CoverLengthSet> coverLengthSet = make_shared<CoverLengthSet>();
		coverLengthSet->Serialize(ar);

		shared_ptr<CoverLengthSet> findCoverLengthOption = GetCoverLengthOption(coverLengthSet->m_sName);
		if (findCoverLengthOption != NULL)
		{
			CString coverLengthName;

			int index = 1;
			coverLengthName.Format(_T("%s(%d)"), coverLengthSet->m_sName, index++);

			while (true)
			{
				if (GetCoverLengthOption(coverLengthName) == NULL)
					break;

				coverLengthName.Format(_T("%s(%d)"), coverLengthSet->m_sName, index++);
			}

			coverLengthSet->m_sName = coverLengthName;
		}
		m_aCoverLengthSets.push_back(coverLengthSet);

		//이음&정착
		shared_ptr<DevSpliceLengthSet> devSpliceLengthSet = make_shared<DevSpliceLengthSet>();
		devSpliceLengthSet->Serialize(ar);

		shared_ptr<DevSpliceLengthSet> findDevSpliceLengthOption = GetDevSpliceLengthOption(devSpliceLengthSet->m_sName);
		if (findDevSpliceLengthOption != NULL)
		{
			CString devSpliceLengthName;

			int index = 1;
			devSpliceLengthName.Format(_T("%s(%d)"), devSpliceLengthSet->m_sName, index++);

			while (true)
			{
				if (GetDevSpliceLengthOption(devSpliceLengthName) == NULL)
					break;

				devSpliceLengthName.Format(_T("%s(%d)"), devSpliceLengthSet->m_sName, index++);
			}

			devSpliceLengthSet->m_sName = devSpliceLengthName;
		}
		m_aDevSpliceLengthSets.push_back(devSpliceLengthSet);

		//글로벌옵션
		shared_ptr<RebarGlobalOption> rebarGlobalOption = make_shared<RebarGlobalOption>(L"", this);
		ar >> rebarGlobalOption->m_sName;

		rebarGlobalOption->SetOrderLengthOption(orderLengthSet);
		rebarGlobalOption->SetCorrectiveLengthOption(correctiveLengthSet);
		rebarGlobalOption->SetCoverLengthOption(coverLengthSet);
		rebarGlobalOption->SetDevelopmentSpliceLengthSettingOption(devSpliceLengthSet);

		shared_ptr<RebarGlobalOption> findRebarGlobalOption = GetGlobalOption(rebarGlobalOption->m_sName);
		if (findRebarGlobalOption != NULL)
		{
			CString rebarGlobalOptionName;

			int index = 1;
			rebarGlobalOptionName.Format(_T("%s(%d)"), rebarGlobalOption->m_sName, index++);

			while (true)
			{
				if (GetGlobalOption(rebarGlobalOptionName) == NULL)
					break;

				rebarGlobalOptionName.Format(_T("%s(%d)"), rebarGlobalOption->m_sName, index++);
			}

			rebarGlobalOption->m_sName = rebarGlobalOptionName;
		}
		m_aGlobalOptions.push_back(rebarGlobalOption);

		if (RadfContextWrapper::Instance()->GetAutoMultiQTSetCheck() == true)
			RadfContextWrapper::Instance()->SetSRPOManagerNameForAutoMultiQTSet(rebarGlobalOption->m_sName);
	}
}

shared_ptr<RebarGlobalOption> RebarGlobalOptionManager::GetCurrentGlobalOption()
{	
	shared_ptr<RebarGlobalOption> opt = GetGlobalOption(m_sCurrentOption);
	
	// Current RebarGlobalOption 못찾는 경우 첫번째 RebarGlobalOption Current로 설정하고 리턴한다.
	if(opt == nullptr && m_aGlobalOptions.size() > 0)
	{
		opt = m_aGlobalOptions[0];
		m_sCurrentOption = opt->m_sName;
	}
	return opt;
}


shared_ptr<OrderLengthSettingSet> RebarGlobalOptionManager::GetOrderLengthSettingOption(const CString& OrderLengthSettingOptionName )
{
	auto it = find_if(m_aOrderLengthSettingSets.begin(), m_aOrderLengthSettingSets.end(), 
					[OrderLengthSettingOptionName](shared_ptr<OrderLengthSettingSet> set){return set->m_sName == OrderLengthSettingOptionName;});
	if(it == m_aOrderLengthSettingSets.end())
		return NULL;
	else
		return (*it);
}

shared_ptr<CorrectiveLengthSet> RebarGlobalOptionManager::GetCorrectiveLengthOption(const CString& CorrectiveLengthOptionName )
{
	auto it = find_if(m_aCorrectiveLengthSets.begin(), m_aCorrectiveLengthSets.end(), 
		[CorrectiveLengthOptionName](shared_ptr<CorrectiveLengthSet> set){return set->m_sName == CorrectiveLengthOptionName;});
	if(it == m_aCorrectiveLengthSets.end())
		return NULL;
	else
		return (*it);
}
shared_ptr<DevSpliceLengthSet> RebarGlobalOptionManager::GetDevSpliceLengthOption( const CString& DevSpliceLengthOptionName )
{
	auto it = find_if(m_aDevSpliceLengthSets.begin(), m_aDevSpliceLengthSets.end(), 
		[DevSpliceLengthOptionName](shared_ptr<DevSpliceLengthSet> set){return set->m_sName == DevSpliceLengthOptionName;});
	if(it == m_aDevSpliceLengthSets.end())
		return NULL;
	else
		return (*it);
}


shared_ptr<CoverLengthSet> RebarGlobalOptionManager::GetCoverLengthOption( const CString& CoverLengthOptionName )
{
	auto it = find_if(m_aCoverLengthSets.begin(), m_aCoverLengthSets.end(), 
		[CoverLengthOptionName](shared_ptr<CoverLengthSet> set){return set->m_sName == CoverLengthOptionName;});
	if(it == m_aCoverLengthSets.end())
		return NULL;
	else
		return (*it);
}
shared_ptr<OrderLengthSet> RebarGlobalOptionManager::GetOrderLengthOption( const CString& OrderLengthOptionName )
{
	auto it = find_if(m_aOrderLengthSets.begin(), m_aOrderLengthSets.end(), 
		[OrderLengthOptionName](shared_ptr<OrderLengthSet> set){return set->m_sName == OrderLengthOptionName;});
	if(it == m_aOrderLengthSets.end())
		return NULL;
	else
		return (*it);
}

void RebarGlobalOptionManager::Initialize()
{
	auto User01 = make_shared<OrderLengthSet>(L"Default");
	User01->Initialize();
	m_aOrderLengthSets.push_back(User01);

	auto Set01 = make_shared<OrderLengthSettingSet>(L"Order");
	//Set01->SetDefaultOrderLengthSetName(User01->m_sName);
	Set01->SetDefaultOrderLengthSet(User01);
	m_aOrderLengthSettingSets.push_back(Set01);

	auto CLSet01 = make_shared<CorrectiveLengthSet>(L"Corrective");
	CLSet01->Initialize();
	m_aCorrectiveLengthSets.push_back(CLSet01);

	auto CovLenSet01 = make_shared<CoverLengthSet>(L"Cover");
	CovLenSet01->Initialize();
	m_aCoverLengthSets.push_back(CovLenSet01);
	
	auto DSLSet01 = make_shared<DevSpliceLengthSet>(L"Dev&Splice");
	DSLSet01->Initialize();
	m_aDevSpliceLengthSets.push_back(DSLSet01);


	auto Manager01 = make_shared<RebarGlobalOption>(L"Manager", this);
	Manager01->SetCorrectiveLengthOption(CLSet01->m_sName);
	Manager01->SetOrderLengthOption(User01->m_sName);
	Manager01->SetCoverLengthOption(CovLenSet01->m_sName);
	Manager01->SetDevelopmentSpliceLengthSettingOption(DSLSet01->m_sName);

	m_aGlobalOptions.push_back(Manager01);

	m_sCurrentOption = Manager01->m_sName;
}

shared_ptr<RebarGlobalOption> RebarGlobalOptionManager::GetGlobalOption( CString name )
{
	auto it = find_if(m_aGlobalOptions.begin(), m_aGlobalOptions.end(), 
		[name](shared_ptr<RebarGlobalOption> opt){return opt->m_sName == name;});
	if(it == m_aGlobalOptions.end())
		return NULL;
	else
		return (*it);
}

void RebarGlobalOptionManager::SetCurrentOption( CString& optionName )
{
	if(GetGlobalOption(optionName) == NULL)
		throw invalid_argument("No global option is defined for the name");
	m_sCurrentOption = optionName;
}
void RebarGlobalOptionManager::SetDefaultOption()
{
	if(m_aGlobalOptions.size() == 0)
		return;

	SetCurrentOption(m_aGlobalOptions.front()->m_sName);
	
}
CString RebarGlobalOptionManager::GetCurrentOptionName()
{
	return m_sCurrentOption;
}

void RebarGlobalOptionManager::SetCurrentOptionName( CString newName )
{
	m_sCurrentOption = newName;
}

bool RebarGlobalOptionManager::WriteFileBinary(CString filePath, CString selectOptionName)
{
	CFile file;
	CFileException e;
	if (file.Open(filePath, CFile::modeCreate | CFile::modeWrite, &e) == FALSE)
	{
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		}
	}
	CArchive ar(&file, CArchive::store);

	int nVersion = MSVersionInfo::GetLatestVersion();
	CString sVersion = _T("");
	sVersion.Format(_T("%d"), nVersion);

	ar << sVersion;

	if(selectOptionName == _T(""))
		Serialize(ar);
	else
	{
		shared_ptr<RebarGlobalOption> rebarGlobalOption = GetGlobalOption(selectOptionName);
		if (rebarGlobalOption == NULL)
			return false;

		SelectOptionSerialize(ar, selectOptionName);
	}

	return true;
}

bool RebarGlobalOptionManager::ReadFileBinary(CString filePath)
{
	CFile file;
	CFileException e;
	if (file.Open(filePath, CFile::modeRead, &e) == FALSE)
	{
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		}

		return false;
	}
	if (file.GetLength() <= 0)	return false;
	CArchive ar(&file, CArchive::load);

	int index = filePath.ReverseFind('.');
	CString sExtension = filePath.Right(filePath.GetLength() - index - 1);

	if (sExtension.MakeLower() == _T("srpo"))
	{
		CString sVersion;
		ar >> sVersion;

		int nVersion = _ttol(sVersion);
		MSVersionInfo::PushCurrentVersion(nVersion);

		SelectOptionSerialize(ar);

		MSVersionInfo::PopCurrentVersion();
	}
	else
	{
		CString sFirstVal;
		ar >> sFirstVal;

		CString sCurrentOption;

		bool bVersionPop = false;
		if (isNumberCheck(sFirstVal) == true && sFirstVal.GetLength() == 8)
		{
			int nVersion = _ttol(sFirstVal);
			if (nVersion >= 20160509)
			{
				bVersionPop = true;
				MSVersionInfo::PushCurrentVersion(nVersion);
				ar >> sCurrentOption;
			}
			else
				sCurrentOption = sFirstVal;
		}
		else
			sCurrentOption = sFirstVal;

		Serialize(ar, sCurrentOption);

		if (bVersionPop == true)
			MSVersionInfo::PopCurrentVersion();
	}

	return true;
}

bool RebarGlobalOptionManager::isNumberCheck(CString str)
{
	int count = str.GetLength();

	for (int i = 0; i < count; i++)
	{
		char temp = str.GetAt(i);

		// 입력된 키가 0 ~ 9 사이인가를 체크. 
		if (temp >= '0' && temp <= '9') continue;
		else break;
	}

	if (i == count) return true;
	else return false;
}

void RebarGlobalOptionManager::Copy(RebarGlobalOptionManager* desManager)
{
	m_sCurrentOption = desManager->m_sCurrentOption;

	m_aOrderLengthSets.clear();
	for (int i = 0; i < desManager->m_aOrderLengthSets.size(); i++)
	{
		auto orderLengthSet = make_shared<OrderLengthSet>();
		orderLengthSet->Copy(desManager->m_aOrderLengthSets[i]);
		m_aOrderLengthSets.push_back(orderLengthSet);
	}

	m_aOrderLengthSettingSets.clear();
	for (int i = 0; i < desManager->m_aOrderLengthSettingSets.size(); i++)
	{
		auto orderLengthSetting = make_shared<OrderLengthSettingSet>();
		orderLengthSetting->Copy(desManager->m_aOrderLengthSettingSets[i]);
		shared_ptr<OrderLengthSet> set = GetOrderLengthOption(orderLengthSetting->m_sDefaultOrderLengthSetName);
		if (set != nullptr) orderLengthSetting->SetDefaultOrderLengthSet(set);
		m_aOrderLengthSettingSets.push_back(orderLengthSetting);
	}

	m_aDevSpliceLengthSets.clear();
	for (int i = 0; i < desManager->m_aDevSpliceLengthSets.size(); i++)
	{
		auto DevelopmentSpliceLength = make_shared<DevSpliceLengthSet>();
		DevelopmentSpliceLength->Copy(desManager->m_aDevSpliceLengthSets[i]);
		m_aDevSpliceLengthSets.push_back(DevelopmentSpliceLength);
	}

	m_aCoverLengthSets.clear();
	for (int i = 0; i < desManager->m_aCoverLengthSets.size(); i++)
	{
		auto coverLength = make_shared<CoverLengthSet>();
		coverLength->Copy(desManager->m_aCoverLengthSets[i]);
		m_aCoverLengthSets.push_back(coverLength);
	}

	m_aCorrectiveLengthSets.clear();
	for (int i = 0; i < desManager->m_aCorrectiveLengthSets.size(); i++)
	{
		auto correctiveLength = make_shared<CorrectiveLengthSet>();
		correctiveLength->Copy(desManager->m_aCorrectiveLengthSets[i]);
		m_aCorrectiveLengthSets.push_back(correctiveLength);
	}

	m_aGlobalOptions.clear();
	for (int i = 0; i < desManager->m_aGlobalOptions.size(); i++)
	{
		auto option = make_shared<RebarGlobalOption>(L"", this);
		option->Copy(desManager->m_aGlobalOptions[i]);
		m_aGlobalOptions.push_back(option);
	}
}
