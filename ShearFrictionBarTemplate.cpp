#include "stdafx.h"
#include "ShearFrictionBarTemplate.h"
#include "MSDeformedBar.h"
#include "..\GMLib\MSVersionInfo.h"
#include "CrackPreventionShearBarTemplate.h"
//
ShearFrictionBarRebarInfoByThick::ShearFrictionBarRebarInfoByThick()
{
	CString sBar = MSDeformedBar::GetStirrupBarStr(300);
	m_nTotalThick = 600;
	m_HBars.InitData(sBar);
	m_VBars.InitData(sBar);
	m_isNumSpacingType = FALSE;
	m_nNumBar = 0;
}
ShearFrictionBarRebarInfoByThick::ShearFrictionBarRebarInfoByThick(ShearFrictionBarRebarInfoByThick *pData)
{
	m_nTotalThick = pData->GetTotalThick();
	m_HBars.CopyFromMe(&pData->m_HBars);
	m_VBars.CopyFromMe(&pData->m_VBars);
	m_isNumSpacingType = pData->GetIsNumSpacingType();
	m_nNumBar = pData->GetNumBar();
}
ShearFrictionBarRebarInfoByThick::~ShearFrictionBarRebarInfoByThick()
{
}
void ShearFrictionBarRebarInfoByThick::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_nTotalThick;
		m_HBars.Serialize(ar);
		m_VBars.Serialize(ar);

		ar << m_isNumSpacingType;
		ar << m_nNumBar;
	}
	else
	{
		ar >> m_nTotalThick;
		m_HBars.Serialize(ar);
		m_VBars.Serialize(ar);

		if(MSVersionInfo::GetCurrentVersion() >= 20241018)
		{
			ar >> m_isNumSpacingType;
			ar >> m_nNumBar;
		}
	}	
}
BOOL ShearFrictionBarRebarInfoByThick::GetIsNumSpacingType()
{
	return m_isNumSpacingType;
}
void ShearFrictionBarRebarInfoByThick::SetIsNumSpacingType(BOOL value)
{
	m_isNumSpacingType = value;
}
long ShearFrictionBarRebarInfoByThick::GetTotalThick()
{
	return m_nTotalThick;
}
void ShearFrictionBarRebarInfoByThick::SetTotalThick(long value)
{
	m_nTotalThick = value;
}
long ShearFrictionBarRebarInfoByThick::GetNumBar()
{
	return m_nNumBar;
}
void ShearFrictionBarRebarInfoByThick::SetNumBar(long value)
{
	m_nNumBar = value;
}
//
ShearFrictionBarPlacementNumberByThick::ShearFrictionBarPlacementNumberByThick()
{
	m_nTotalThick = 2500;
	m_nPlacementNumber = 2;
	m_nFirstPlacementThick = 800;
}
ShearFrictionBarPlacementNumberByThick::ShearFrictionBarPlacementNumberByThick(ShearFrictionBarPlacementNumberByThick *pData)
{
	m_nTotalThick = pData->GetTotalThick();
	m_nPlacementNumber = pData->GetPlacementNumber();
	m_nFirstPlacementThick = pData->GetFirstPlacementThick();
}
ShearFrictionBarPlacementNumberByThick::~ShearFrictionBarPlacementNumberByThick()
{

}
void ShearFrictionBarPlacementNumberByThick::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_nTotalThick;
		ar << m_nPlacementNumber;
		ar << m_nFirstPlacementThick;
	}
	else
	{
		ar >> m_nTotalThick;
		ar >> m_nPlacementNumber;
		ar >> m_nFirstPlacementThick;
	}
}
long ShearFrictionBarPlacementNumberByThick::GetTotalThick()
{
	return m_nTotalThick;
}
long ShearFrictionBarPlacementNumberByThick::GetPlacementNumber()
{
	return m_nPlacementNumber;
}
long ShearFrictionBarPlacementNumberByThick::GetFirstPlacementThick()
{
	return m_nFirstPlacementThick;
}
void ShearFrictionBarPlacementNumberByThick::SetTotalThick(long value)
{
	m_nTotalThick = value;
}
void ShearFrictionBarPlacementNumberByThick::SetPlacementNumber(long value)
{
	m_nPlacementNumber = value;
}
void ShearFrictionBarPlacementNumberByThick::SetFirstPlacementThick(long value)
{
	m_nFirstPlacementThick = value;
}
//

ShearFrictionBarTemplate::ShearFrictionBarTemplate()
{
	DataInit();
}

ShearFrictionBarTemplate::~ShearFrictionBarTemplate()
{
	DeleteShearFrictionBarRebarInfoByThickArr();
	DeleteShearFrictionBarPlacementNumberByThickArr();

	delete m_pCrackPreventionShearBarTemplate;
	m_pCrackPreventionShearBarTemplate = nullptr;
}
void ShearFrictionBarTemplate::DeleteShearFrictionBarRebarInfoByThickArr()
{
	for(int i = 0 ; i < m_ShearFrictionBarRebarInfoByThickArr.size() ; i++)
	{
		delete m_ShearFrictionBarRebarInfoByThickArr[i];
		m_ShearFrictionBarRebarInfoByThickArr[i] = nullptr;
	}
	m_ShearFrictionBarRebarInfoByThickArr.clear();
}
void ShearFrictionBarTemplate::DeleteShearFrictionBarPlacementNumberByThickArr()
{
	for(int i = 0 ; i < m_ShearFrictionBarPlacementNumberByThickArr.size() ; i++)
	{
		delete m_ShearFrictionBarPlacementNumberByThickArr[i];
		m_ShearFrictionBarPlacementNumberByThickArr[i] = nullptr;
	}
	m_ShearFrictionBarPlacementNumberByThickArr.clear();
}

void ShearFrictionBarTemplate::DataInit()
{
	m_bPlaceBar = FALSE;
	m_nPlaceConditionThick = 1500;
	m_nTopDevType = 0;
	m_nTopDevUserLength = 100;
	m_nBotDevType = 0;
	m_nBotDevUserLength = 100;

	CString sD10Bar = MSDeformedBar::GetStirrupBarStr(300, 10);
	CString sD13Bar = MSDeformedBar::GetStirrupBarStr(300, 13);
	CString sD16Bar = MSDeformedBar::GetStirrupBarStr(300, 16);

	ShearFrictionBarRebarInfoByThick *pRebarInfoByThick = new ShearFrictionBarRebarInfoByThick();
	pRebarInfoByThick->SetTotalThick(2500);
	pRebarInfoByThick->m_HBars.InitData(sD10Bar);
	pRebarInfoByThick->m_VBars.InitData(sD10Bar);
	m_ShearFrictionBarRebarInfoByThickArr.push_back(pRebarInfoByThick);

	//pRebarInfoByThick = new ShearFrictionBarRebarInfoByThick();
	//pRebarInfoByThick->SetTotalThick(800);
	//pRebarInfoByThick->m_HBars.InitData(sD13Bar);
	//pRebarInfoByThick->m_VBars.InitData(sD13Bar);
	//m_ShearFrictionBarRebarInfoByThickArr.push_back(pRebarInfoByThick);

	pRebarInfoByThick = new ShearFrictionBarRebarInfoByThick();
	pRebarInfoByThick->SetTotalThick(0);
	pRebarInfoByThick->m_HBars.InitData(sD16Bar);
	pRebarInfoByThick->m_VBars.InitData(sD16Bar);
	m_ShearFrictionBarRebarInfoByThickArr.push_back(pRebarInfoByThick);

	ShearFrictionBarPlacementNumberByThick *pPlacementNumberByThick = new ShearFrictionBarPlacementNumberByThick();
	pPlacementNumberByThick->SetTotalThick(2500);
	pPlacementNumberByThick->SetPlacementNumber(2);
	pPlacementNumberByThick->SetFirstPlacementThick(800);
	m_ShearFrictionBarPlacementNumberByThickArr.push_back(pPlacementNumberByThick);

	pPlacementNumberByThick = new ShearFrictionBarPlacementNumberByThick();
	pPlacementNumberByThick->SetTotalThick(0);
	pPlacementNumberByThick->SetPlacementNumber(3);
	pPlacementNumberByThick->SetFirstPlacementThick(1000);
	m_ShearFrictionBarPlacementNumberByThickArr.push_back(pPlacementNumberByThick);

	m_pCrackPreventionShearBarTemplate = new CrackPreventionShearBarTemplate();
}

void ShearFrictionBarTemplate::CopyFromMe(ShearFrictionBarTemplate* pRebarTemplate)
{
	m_bPlaceBar = pRebarTemplate->GetPlaceBar();
	m_nPlaceConditionThick = pRebarTemplate->GetPlaceConditionThick();
	m_nTopDevType = pRebarTemplate->GetTopDevType();
	m_nTopDevUserLength = pRebarTemplate->GetTopDevUserLength();
	m_nBotDevType = pRebarTemplate->GetBotDevType();
	m_nBotDevUserLength = pRebarTemplate->GetBotDevUserLength();
	DeleteShearFrictionBarRebarInfoByThickArr();
	for(int i = 0 ; i < pRebarTemplate->m_ShearFrictionBarRebarInfoByThickArr.size(); i++)
	{
		ShearFrictionBarRebarInfoByThick *pNewData = new ShearFrictionBarRebarInfoByThick(pRebarTemplate->m_ShearFrictionBarRebarInfoByThickArr[i]);
		m_ShearFrictionBarRebarInfoByThickArr.push_back(pNewData);
	}

	DeleteShearFrictionBarPlacementNumberByThickArr();	
	for(int i = 0 ; i < pRebarTemplate->m_ShearFrictionBarPlacementNumberByThickArr.size(); i++)
	{
		ShearFrictionBarPlacementNumberByThick *pNewData = new ShearFrictionBarPlacementNumberByThick(pRebarTemplate->m_ShearFrictionBarPlacementNumberByThickArr[i]);
		m_ShearFrictionBarPlacementNumberByThickArr.push_back(pNewData);
	}
	m_pCrackPreventionShearBarTemplate->CopyFromMe(pRebarTemplate->GetCrackPreventionShearBarTemplate());
}

void ShearFrictionBarTemplate::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_bPlaceBar;	
		ar << m_nTopDevType;
		ar << m_nTopDevUserLength;
		ar << m_nBotDevType;
		ar << m_nBotDevUserLength;

		ar << (int)m_ShearFrictionBarRebarInfoByThickArr.size();
		for(int i = 0 ; i < m_ShearFrictionBarRebarInfoByThickArr.size() ; i++)
		{
			m_ShearFrictionBarRebarInfoByThickArr[i]->Serialize(ar);
		}
		ar << (int)m_ShearFrictionBarPlacementNumberByThickArr.size();
		for(int i = 0 ; i < m_ShearFrictionBarPlacementNumberByThickArr.size() ; i++)
		{
			m_ShearFrictionBarPlacementNumberByThickArr[i]->Serialize(ar);
		}

		ar << m_nPlaceConditionThick;

		m_pCrackPreventionShearBarTemplate->Serialize(ar);
	}
	else
	{
		ar >> m_bPlaceBar;	
		ar >> m_nTopDevType;
		ar >> m_nTopDevUserLength;
		ar >> m_nBotDevType;
		ar >> m_nBotDevUserLength;

		int NumObject = 0;
		ar >> NumObject;
		if(NumObject > 0)
			DeleteShearFrictionBarRebarInfoByThickArr();
		for(int i = 0 ; i < NumObject ; i++)
		{
			ShearFrictionBarRebarInfoByThick *pData = new ShearFrictionBarRebarInfoByThick();
			pData->Serialize(ar);
			m_ShearFrictionBarRebarInfoByThickArr.push_back(pData);
		}

		ar >> NumObject;
		if(NumObject > 0)
			DeleteShearFrictionBarPlacementNumberByThickArr();
		for(int i = 0 ; i < NumObject ; i++)
		{
			ShearFrictionBarPlacementNumberByThick *pData = new ShearFrictionBarPlacementNumberByThick();
			pData->Serialize(ar);
			m_ShearFrictionBarPlacementNumberByThickArr.push_back(pData);
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20230721)
			ar >> m_nPlaceConditionThick;

		if(MSVersionInfo::GetCurrentVersion() >= 20240626)
			m_pCrackPreventionShearBarTemplate->Serialize(ar);
	}
}
BOOL ShearFrictionBarTemplate::GetPlaceBar()
{
	return m_bPlaceBar;
}
long ShearFrictionBarTemplate::GetPlaceConditionThick()
{
	return m_nPlaceConditionThick;
}
long ShearFrictionBarTemplate::GetTopDevType()
{
	return m_nTopDevType;
}
long ShearFrictionBarTemplate::GetTopDevUserLength()
{
	return m_nTopDevUserLength;
}
long ShearFrictionBarTemplate::GetBotDevType()
{
	return m_nBotDevType;
}
long ShearFrictionBarTemplate::GetBotDevUserLength()
{
	return m_nBotDevUserLength;
}
vector<ShearFrictionBarRebarInfoByThick*> ShearFrictionBarTemplate::GetShearFrictionBarRebarInfoByThickArr()
{
	return m_ShearFrictionBarRebarInfoByThickArr;
}
vector<ShearFrictionBarPlacementNumberByThick*> ShearFrictionBarTemplate::GetShearFrictionBarPlacementNumberByThickArr()
{
	return m_ShearFrictionBarPlacementNumberByThickArr;
}

void ShearFrictionBarTemplate::SetPlaceBar(BOOL value)
{
	m_bPlaceBar = value;
}
void ShearFrictionBarTemplate::SetPlaceConditionThick(long value)
{
	m_nPlaceConditionThick = value;
}
void ShearFrictionBarTemplate::SetTopDevType(long value)
{
	m_nTopDevType = value;
}
void ShearFrictionBarTemplate::SetTopDevUserLength(long value)
{
	m_nTopDevUserLength = value;
}
void ShearFrictionBarTemplate::SetBotDevType(long value)
{
	m_nBotDevType = value;
}
void ShearFrictionBarTemplate::SetBotDevUserLength(long value)
{
	m_nBotDevUserLength = value;
}
void ShearFrictionBarTemplate::SetShearFrictionBarRebarInfoByThickArr(vector<ShearFrictionBarRebarInfoByThick*> arr)
{
	DeleteShearFrictionBarRebarInfoByThickArr();
	m_ShearFrictionBarRebarInfoByThickArr = arr;
}
void ShearFrictionBarTemplate::SetShearFrictionBarPlacementNumberByThickArr(vector<ShearFrictionBarPlacementNumberByThick*> arr)
{
	DeleteShearFrictionBarPlacementNumberByThickArr();
	m_ShearFrictionBarPlacementNumberByThickArr = arr;
}
ShearFrictionBarRebarInfoByThick *ShearFrictionBarTemplate::FindShearFrictionBarRebarInfoByThick(long nThick)
{
	if(nThick <= m_nPlaceConditionThick)
		return nullptr;

	ShearFrictionBarRebarInfoByThick *pDefaultInfo = nullptr;
	std::sort(m_ShearFrictionBarRebarInfoByThickArr.begin(), m_ShearFrictionBarRebarInfoByThickArr.end(), 
		[](ShearFrictionBarRebarInfoByThick *pInfo, ShearFrictionBarRebarInfoByThick *pCompareInfo) 
		{
			return pInfo->GetTotalThick() < pCompareInfo->GetTotalThick();
		}
	);
	for(int i = 0 ; i < m_ShearFrictionBarRebarInfoByThickArr.size() ; i++)
	{
		ShearFrictionBarRebarInfoByThick *pInfo = m_ShearFrictionBarRebarInfoByThickArr[i];
		long nTotalThick = pInfo->GetTotalThick();
		if(nTotalThick == 0)
		{
			pDefaultInfo = pInfo;
			continue;
		}
		if(nThick > nTotalThick)
			continue;
		
		if(nThick <= nTotalThick)
			return pInfo;
	}
	return pDefaultInfo;
}
ShearFrictionBarPlacementNumberByThick *ShearFrictionBarTemplate::FindShearFrictionBarPlacementNumberByThick(long nThick)
{
	if(nThick <= m_nPlaceConditionThick)
		return nullptr;

	ShearFrictionBarPlacementNumberByThick *pDefaultInfo = nullptr;
	std::sort(m_ShearFrictionBarPlacementNumberByThickArr.begin(), m_ShearFrictionBarPlacementNumberByThickArr.end(), 
		[](ShearFrictionBarPlacementNumberByThick *pInfo, ShearFrictionBarPlacementNumberByThick *pCompareInfo) 
		{
			return pInfo->GetTotalThick() < pCompareInfo->GetTotalThick();
		}
	);
	for(int i = 0 ; i < m_ShearFrictionBarPlacementNumberByThickArr.size() ; i++)
	{
		ShearFrictionBarPlacementNumberByThick *pInfo = m_ShearFrictionBarPlacementNumberByThickArr[i];
		long nTotalThick = pInfo->GetTotalThick();
		if(nTotalThick == 0)
		{
			pDefaultInfo = pInfo;
			continue;
		}
		if(nThick > nTotalThick)
			continue;
		
		if(nThick <= nTotalThick)
			return pInfo;
	}
	return pDefaultInfo;
}

CrackPreventionShearBarTemplate *ShearFrictionBarTemplate::GetCrackPreventionShearBarTemplate()
{
	return m_pCrackPreventionShearBarTemplate;
}