#include "StdAfx.h"
#include "ReinforceBarSetting.h"
#include "MSReinforceBar.h"
#include "MSRebarBuilding.h"
#include "MSGlobalOption.h"
#include "MSDeformedBar.h"
#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSReinforceBarSetting, MSObject, VERSIONABLE_SCHEMA | 1)
long MSReinforceBarSetting::ms_LastID = 0;
long MSReinforceBarSetting::ms_GlobalLastID = 0;

MSReinforceBarSetting::MSReinforceBarSetting(void)
{
	m_ID = 0;
	m_GlobalID = 0;
	m_Name = _T("");
	m_pBldg = NULL;
}


MSReinforceBarSetting::~MSReinforceBarSetting(void)
{
}

long MSReinforceBarSetting::GetID()
{
	if(m_ID == 0) // 
	{
		m_ID = ms_LastID + 1;
		ms_LastID = m_ID;
		return m_ID;
	}

	return m_ID;
}

void MSReinforceBarSetting::SetLastID( long NewID )
{
	if(NewID > ms_LastID)
		ms_LastID = NewID;
}

long MSReinforceBarSetting::GetGlobalID()
{
	if(m_GlobalID == 0) // 
	{
		m_GlobalID = ms_GlobalLastID + 1;
		ms_GlobalLastID = m_GlobalID;
		return m_GlobalID;
	}

	return m_GlobalID;
}

void MSReinforceBarSetting::SetGlobalLastID( long NewID )
{
	if(NewID > ms_GlobalLastID)
		ms_GlobalLastID = NewID;
}

MSObject* MSReinforceBarSetting::Copy( bool bNewID /*= true*/ )
{
	MSReinforceBarSetting* pNewObj;

	pNewObj = new MSReinforceBarSetting();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;

}

void MSReinforceBarSetting::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;

	MSReinforceBarSetting* pSource = (MSReinforceBarSetting*)pSource_in;
	m_Name = pSource->m_Name;
	mm_IndexToReinforceBar.clear();
	map<long, MSReinforceBar*>::iterator it;
	for( it = pSource->mm_IndexToReinforceBar.begin(); it != pSource->mm_IndexToReinforceBar.end(); it++)
	{
		mm_IndexToReinforceBar.insert(make_pair(it->first, it->second));
	}

	MSObject::CopyFromMe(pSource, bNewID);
}

void MSReinforceBarSetting::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	buffer << m_Name;

	int count = mm_IndexToReinforceBar.size();
	buffer << count;
	map<long, MSReinforceBar*>::iterator it;
	for(it = mm_IndexToReinforceBar.begin(); it != mm_IndexToReinforceBar.end(); it++)
	{
		int DiaIndex = it->first;
		buffer << DiaIndex;

		long id = it->second->m_ID;
		buffer << id;
	}
}

void MSReinforceBarSetting::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	buffer >> m_Name;

	mm_IndexToReinforceBar.clear();
	int DiaIndex = -1;

	int count = 0;
	long ReinforceBarID = 0;
	buffer >> count;
	for(int i = 0; i < count; i++)
	{
		buffer >> DiaIndex;
		buffer >> ReinforceBarID;

		mm_IndexToReinforceBar.insert(make_pair(DiaIndex, m_pBldg->GetReinforceBar(ReinforceBarID)));
	}
}

void MSReinforceBarSetting::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	int NumReinforceBar = 0;
	int DiaIndex = 0;
	int ReinforceID = 0;
	if(ar.IsStoring())
	{
		FixIndexToReinforceBar();
		ar <<		m_ID;
		ar <<       m_Name;
		NumReinforceBar = mm_IndexToReinforceBar.size();
		ar << NumReinforceBar;

		map<long, MSReinforceBar*>::iterator it;
		for(it = mm_IndexToReinforceBar.begin(); it != mm_IndexToReinforceBar.end(); it++)
		{
			DiaIndex = it->first;
			ar << DiaIndex;

			MSReinforceBar* pReinforceBar = it->second;
			ReinforceID = pReinforceBar->m_ID;
			ar << ReinforceID;
		}

	}
	else
	{
		ar >>		m_ID;
		ar >>       m_Name;
		m_Name = m_Name.MakeUpper();
		ar >> NumReinforceBar;

		if(MSVersionInfo::GetCurrentVersion() < 20160106)
		{
			long nDia = 0;
			CString sDiaName;
			for(int i = 0; i < NumReinforceBar; i++)
			{
				ar >> nDia;
				ar >> ReinforceID;

				sDiaName.Format(_T("%ld"), nDia);
				DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
				if(DiaIndex == -1)
				{
					ASSERT(0);
					continue;
				}
				MSRebarBuilding* pRebarBuilding = dynamic_cast<MSRebarBuilding*>(m_pBldg);
				if (pRebarBuilding != nullptr)
				{
					MSReinforceBar* pReinforceBar = pRebarBuilding->GetReinforceBar(ReinforceID);
					if (pReinforceBar == nullptr && pRebarBuilding->mm_ReinforceBar.size() != 0)
						pReinforceBar = pRebarBuilding->mm_ReinforceBar.rbegin()->second;
					mm_IndexToReinforceBar.insert(make_pair(DiaIndex, pReinforceBar));
				}
			}
		}
		else
		{
			for(int i = 0; i < NumReinforceBar; i++)
			{
				ar >> DiaIndex;
				ar >> ReinforceID;

				MSRebarBuilding* pRebarBuilding = dynamic_cast<MSRebarBuilding*>(m_pBldg);
				if (pRebarBuilding != nullptr)
				{
					MSReinforceBar* pReinforceBar = pRebarBuilding->GetReinforceBar(ReinforceID);
					if (pReinforceBar == nullptr && pRebarBuilding->mm_ReinforceBar.size() != 0)
						pReinforceBar = pRebarBuilding->mm_ReinforceBar.rbegin()->second;
					mm_IndexToReinforceBar.insert(make_pair(DiaIndex, pReinforceBar));
				}
			}
		}

		if (NumReinforceBar > 0)
			ReinforceBarDiaValidateCheck(false);

		SetLastID(m_ID);
	}
}

void MSReinforceBarSetting::ReinforceBarDiaValidateCheck(bool IsGlobalOption)
{
	MSRebarBuilding* pRebarBuilding = dynamic_cast<MSRebarBuilding*>(m_pBldg);
	if (pRebarBuilding == nullptr)	return;

	CString sDiaName;

	CStringArray DiaNameList;
	long numDia = MSDeformedBar::GetRebarNameList(DiaNameList);

	map<long, MSReinforceBar*>::iterator it;
	for (long index = 0; index < numDia; index++)
	{
		sDiaName = DiaNameList[index];
		int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
		if(DiaIndex == -1)	continue;
		it = mm_IndexToReinforceBar.find(DiaIndex);
		if (it == mm_IndexToReinforceBar.end())
		{
			MSReinforceBar* pReinforceBar = NULL;
			double dDia = MSDeformedBar::FindDeformedBarDiameterByIndex(DiaIndex);
			if (dDia <= 13)
			{
				if (IsGlobalOption)
					pReinforceBar = MSGlobalOption::GetInstance()->GetReinforceBar(_T("SD400"));
				else
					pReinforceBar = pRebarBuilding->GetReinforceBar(_T("SD400"));
			}
			else
			{
				if (IsGlobalOption)
					pReinforceBar = MSGlobalOption::GetInstance()->GetReinforceBar(_T("SD500"));
				else
					pReinforceBar = pRebarBuilding->GetReinforceBar(_T("SD500"));
			}
			if(pReinforceBar == NULL)
				continue;
			mm_IndexToReinforceBar.insert(make_pair(DiaIndex, pReinforceBar));
		}
	}
}	

void MSReinforceBarSetting::GlobalOptionSerialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	int NumReinforceBar = 0;
	int DiaIndex = 0;
	int ReinforceID = 0;
	if(ar.IsStoring())
	{
		FixIndexToReinforceBar();
		ar <<		m_GlobalID;
		ar <<       m_Name;
		NumReinforceBar = mm_IndexToReinforceBar.size();
		ar << NumReinforceBar;
		map<long, MSReinforceBar*>::iterator it;
		for(it = mm_IndexToReinforceBar.begin(); it != mm_IndexToReinforceBar.end(); it++)
		{
			DiaIndex = it->first;
			ar << DiaIndex;

			MSReinforceBar* pReinforceBar = it->second;
			ReinforceID = pReinforceBar->m_GlobalID;
			ar << ReinforceID;
		}
	}
	else
	{
		ar >>		m_GlobalID;
		ar >>       m_Name;
		m_Name = m_Name.MakeUpper();
		ar >> NumReinforceBar;
		if(MSVersionInfo::GetCurrentVersion() < 20160106)
		{
			long nDia = 0;
			CString sDiaName;
			for(int i = 0; i < NumReinforceBar; i++)
			{
				ar >> nDia;
				ar >> ReinforceID;

				sDiaName.Format(_T("%ld"), nDia);
				DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
				if(DiaIndex == -1)
				{
					ASSERT(0);
					continue;
				}
				MSReinforceBar* pReinforceBar = MSGlobalOption::GetInstance()->GetReinforceBar(ReinforceID);
				MSRebarBuilding* pRebarBuilding = dynamic_cast<MSRebarBuilding*>(m_pBldg);
				if (pRebarBuilding != nullptr)
				{
					if (pReinforceBar == nullptr && pRebarBuilding->mm_ReinforceBar.size() != 0)
						pReinforceBar = pRebarBuilding->mm_ReinforceBar.rbegin()->second;
				}
				mm_IndexToReinforceBar.insert(make_pair(DiaIndex, pReinforceBar));
			}
		}
		else
		{
			for(int i = 0; i < NumReinforceBar; i++)
			{
				ar >> DiaIndex;
				ar >> ReinforceID;
				MSReinforceBar* pReinforceBar = MSGlobalOption::GetInstance()->GetReinforceBar(ReinforceID);
				MSRebarBuilding* pRebarBuilding = dynamic_cast<MSRebarBuilding*>(m_pBldg);
				if (pRebarBuilding != nullptr)
				{
					if (pReinforceBar == nullptr && pRebarBuilding->mm_ReinforceBar.size() != 0)
						pReinforceBar = pRebarBuilding->mm_ReinforceBar.rbegin()->second;
				}
				mm_IndexToReinforceBar.insert(make_pair(DiaIndex, pReinforceBar));
			}
		}
		if (NumReinforceBar > 0)
			ReinforceBarDiaValidateCheck(true);

		SetGlobalLastID(m_GlobalID);
	}
}

void MSReinforceBarSetting::FixIndexToReinforceBar()
{
	CString strErr;
	for (auto it = mm_IndexToReinforceBar.rbegin(); it != mm_IndexToReinforceBar.rend(); it++)
	{
		if (it->second == nullptr)
		{
			strErr.Format(_T("Delete null MSReinforceBar | MSReinforceBarSettingID:%d, ReinforceBarDiaIndex:%d"), m_ID, it->first);
			LoggerWrapper::Log(_T(""));
			StatusReporterWrapper::OutputError(strErr);

			mm_IndexToReinforceBar.erase(it->first);
		}
	}
}

MSSteelGradeManager::eSteelGrade MSReinforceBarSetting::GetSteelGrade(int barIndex)
{
	auto it = mm_IndexToReinforceBar.find(barIndex);
	if (it == mm_IndexToReinforceBar.end())	return MSSteelGradeManager::SD300;
	return it->second->m_eSGrade;
}
CString MSReinforceBarSetting::GetMSReinforceBarName(int barIndex)
{
	auto it = mm_IndexToReinforceBar.find(barIndex);
	if (it == mm_IndexToReinforceBar.end())	
		return _T("");
	return it->second->m_Name;
}
vector<MSSteelGradeManager::eSteelGrade> MSReinforceBarSetting::GetSteelGradeList()
{
	vector<MSSteelGradeManager::eSteelGrade> steelGradeList;
	for (auto it = mm_IndexToReinforceBar.begin(); it != mm_IndexToReinforceBar.end(); it++)
		steelGradeList.push_back(it->second->m_eSGrade);
	return steelGradeList;
}

bool MSReinforceBarSetting::IsPlainConc()
{
	CString strChk = LocalDic::GetLocal(_T("무근콘크리트"));
	if (m_Name == strChk)
		return true;

	if (m_Name == _T("무근콘크리트") || m_Name == _T("CONC W/O REBAR") || m_Name == _T("Plain Concrete") ||
		m_Name == _T("無筋コンクリート") || m_Name == _T("Bê tông không cốt thép") || m_Name == _T("无筋混凝土"))
		return true;

	return false;
}
