#include "stdafx.h"
#include "CrackPreventionShearBarTemplate.h"
#include "MSDeformedBar.h"
#include "..\GMLib\MSVersionInfo.h"

//
CrackPreventionShearBarTemplate::CrackPreventionShearBarTemplate()
{
	DataInit();
}
CrackPreventionShearBarTemplate::CrackPreventionShearBarTemplate(CrackPreventionShearBarTemplate *pData)
{
	
}
CrackPreventionShearBarTemplate::~CrackPreventionShearBarTemplate()
{
	DeleteCrackPreventionShearBarRebarInfoArr();
}
void CrackPreventionShearBarTemplate::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_bPlaceBar;
		ar << m_nDiaType;
		ar << m_nLxDiaIndex;
		ar << m_nLyDiaIndex;
		ar << m_bLxUserSteelGradeS;
		ar << m_bLyUserSteelGradeS;
		ar << m_nSpacingType;
		ar << m_nLxSpacing;
		ar << m_nLySpacing;
		ar << m_bPlaceSpacer;
		ar << m_nSpacerHookLenAType;
		ar << m_nSpacerHookLenA;
		ar << m_nSpacerHookLenAUser;
		ar << m_nSpacerHookLenBType;
		ar << m_nSpacerHookLenB;
		ar << m_nSpacerHookLenBUser;
		ar << m_nSpacerWidth;

		ar << (int)m_CrackPreventionShearBarRebarInfoArr.size();
		for(int i = 0 ; i < m_CrackPreventionShearBarRebarInfoArr.size() ; i++)
		{
			m_CrackPreventionShearBarRebarInfoArr[i]->Serialize(ar);
		}
	}
	else
	{
		ar >> m_bPlaceBar;
		ar >> m_nDiaType;
		ar >> m_nLxDiaIndex;
		ar >> m_nLyDiaIndex;
		ar >> m_bLxUserSteelGradeS;
		ar >> m_bLyUserSteelGradeS;
		ar >> m_nSpacingType;
		ar >> m_nLxSpacing;
		ar >> m_nLySpacing;
		ar >> m_bPlaceSpacer;
		ar >> m_nSpacerHookLenAType;
		ar >> m_nSpacerHookLenA;
		ar >> m_nSpacerHookLenAUser;
		ar >> m_nSpacerHookLenBType;
		ar >> m_nSpacerHookLenB;
		ar >> m_nSpacerHookLenBUser;
		ar >> m_nSpacerWidth;

		int NumObject = 0;
		ar >> NumObject;
		if(NumObject > 0)
			DeleteCrackPreventionShearBarRebarInfoArr();
		for(int i = 0 ; i < NumObject ; i++)
		{
			CrackPreventionShearBarRebarInfo *pInfo = new CrackPreventionShearBarRebarInfo();
			pInfo->Serialize(ar);
			m_CrackPreventionShearBarRebarInfoArr.push_back(pInfo);
		}
	}
}
void CrackPreventionShearBarTemplate::DataInit()
{
	m_bPlaceBar = FALSE;
	m_nDiaType = 0;
	m_nLxDiaIndex = 0;
	m_nLyDiaIndex = 0;
	m_bLxUserSteelGradeS = FALSE;
	m_bLyUserSteelGradeS = FALSE;
	m_nSpacingType = 0;
	m_nLxSpacing = 300;
	m_nLySpacing = 300;

	m_bPlaceSpacer = FALSE;
	m_nSpacerHookLenAType = 1;
	m_nSpacerHookLenA = 100;
	m_nSpacerHookLenAUser = 400;
	m_nSpacerHookLenBType = 1;
	m_nSpacerHookLenB = 0;
	m_nSpacerHookLenBUser = 400;
	m_nSpacerWidth = 300;

	CrackPreventionShearBarRebarInfo *pCrackPreventionShearBarRebarInfo = new CrackPreventionShearBarRebarInfo();
	m_CrackPreventionShearBarRebarInfoArr.push_back(pCrackPreventionShearBarRebarInfo);
}
void CrackPreventionShearBarTemplate::CopyFromMe(CrackPreventionShearBarTemplate* pRebarTemplate)
{
	m_bPlaceBar = pRebarTemplate->GetPlaceBar();
	m_nDiaType = pRebarTemplate->GetDiaType();
	m_nLxDiaIndex = pRebarTemplate->GetLxDiaIndex();
	m_nLyDiaIndex = pRebarTemplate->GetLyDiaIndex();
	m_bLxUserSteelGradeS = pRebarTemplate->GetLxUserSteelGradeS();
	m_bLyUserSteelGradeS = pRebarTemplate->GetLyUserSteelGradeS();
	m_nSpacingType = pRebarTemplate->GetSpacingType();
	m_nLxSpacing = pRebarTemplate->GetLxSpacing();
	m_nLySpacing = pRebarTemplate->GetLySpacing();

	m_bPlaceSpacer = pRebarTemplate->GetPlaceSpacer();
	m_nSpacerHookLenAType = pRebarTemplate->GetSpacerHookLenAType();
	m_nSpacerHookLenA = pRebarTemplate->GetSpacerHookLenA();
	m_nSpacerHookLenAUser = pRebarTemplate->GetSpacerHookLenAUser();
	m_nSpacerHookLenBType = pRebarTemplate->GetSpacerHookLenBType();
	m_nSpacerHookLenB = pRebarTemplate->GetSpacerHookLenB();
	m_nSpacerHookLenBUser = pRebarTemplate->GetSpacerHookLenBUser();
	m_nSpacerWidth = pRebarTemplate->GetSpacerWidth();

	DeleteCrackPreventionShearBarRebarInfoArr();
	for(int i = 0 ; i < pRebarTemplate->m_CrackPreventionShearBarRebarInfoArr.size(); i++)
	{
		CrackPreventionShearBarRebarInfo *pNewData = new CrackPreventionShearBarRebarInfo(pRebarTemplate->m_CrackPreventionShearBarRebarInfoArr[i]);
		m_CrackPreventionShearBarRebarInfoArr.push_back(pNewData);
	}
}
vector<CrackPreventionShearBarRebarInfo*> CrackPreventionShearBarTemplate::GetCrackPreventionShearBarRebarInfoArr()
{
	return m_CrackPreventionShearBarRebarInfoArr;
}
void CrackPreventionShearBarTemplate::SetCrackPreventionShearBarRebarInfoArr(vector<CrackPreventionShearBarRebarInfo*> arr)
{
	DeleteCrackPreventionShearBarRebarInfoArr();
	m_CrackPreventionShearBarRebarInfoArr = arr;
}
void CrackPreventionShearBarTemplate::DeleteCrackPreventionShearBarRebarInfoArr()
{
	for(int i = 0 ; i < m_CrackPreventionShearBarRebarInfoArr.size() ; i++)
	{
		delete m_CrackPreventionShearBarRebarInfoArr[i];
		m_CrackPreventionShearBarRebarInfoArr[i] = nullptr;
	}
	m_CrackPreventionShearBarRebarInfoArr.clear();
}

BOOL CrackPreventionShearBarTemplate::GetPlaceBar()
{
	return m_bPlaceBar;
}
void CrackPreventionShearBarTemplate::SetPlaceBar(BOOL value)
{
	m_bPlaceBar = value;
}
long CrackPreventionShearBarTemplate::GetDiaType()
{
	return m_nDiaType;
}
long CrackPreventionShearBarTemplate::GetLxDiaIndex()
{
	return m_nLxDiaIndex;
}
long CrackPreventionShearBarTemplate::GetLyDiaIndex()
{
	return m_nLyDiaIndex;
}
BOOL CrackPreventionShearBarTemplate::GetLxUserSteelGradeS()
{
	return m_bLxUserSteelGradeS;
}
BOOL CrackPreventionShearBarTemplate::GetLyUserSteelGradeS()
{
	return m_bLyUserSteelGradeS;
}
long CrackPreventionShearBarTemplate::GetSpacingType()
{
	return m_nSpacingType;
}
long CrackPreventionShearBarTemplate::GetLxSpacing()
{
	return m_nLxSpacing;
}
long CrackPreventionShearBarTemplate::GetLySpacing()
{
	return m_nLySpacing;
}
BOOL CrackPreventionShearBarTemplate::GetPlaceSpacer()
{
	return m_bPlaceSpacer;
}
long CrackPreventionShearBarTemplate::GetSpacerHookLenAType()
{
	return m_nSpacerHookLenAType;
}
long CrackPreventionShearBarTemplate::GetSpacerHookLenA()
{
	return m_nSpacerHookLenA;
}
long CrackPreventionShearBarTemplate::GetSpacerHookLenAUser()
{
	return m_nSpacerHookLenAUser;
}
long CrackPreventionShearBarTemplate::GetSpacerHookLenBType()
{
	return m_nSpacerHookLenBType;
}
long CrackPreventionShearBarTemplate::GetSpacerHookLenB()
{
	return m_nSpacerHookLenB;
}
long CrackPreventionShearBarTemplate::GetSpacerHookLenBUser()
{
	return m_nSpacerHookLenBUser;
}
long CrackPreventionShearBarTemplate::GetSpacerWidth()
{
	return m_nSpacerWidth;
}

void CrackPreventionShearBarTemplate::SetDiaType(long value)
{
	m_nDiaType = value;
}
void CrackPreventionShearBarTemplate::SetLxDiaIndex(long value)
{
	m_nLxDiaIndex = value;
}
void CrackPreventionShearBarTemplate::SetLyDiaIndex(long value)
{
	m_nLyDiaIndex = value;
}
void CrackPreventionShearBarTemplate::SetLxUserSteelGradeS(BOOL value)
{
	m_bLxUserSteelGradeS = value;
}
void CrackPreventionShearBarTemplate::SetLyUserSteelGradeS(BOOL value)
{
	m_bLyUserSteelGradeS = value;
}
void CrackPreventionShearBarTemplate::SetSpacingType(long value)
{
	m_nSpacingType = value;
}
void CrackPreventionShearBarTemplate::SetLxSpacing(long value)
{
	m_nLxSpacing = value;
}
void CrackPreventionShearBarTemplate::SetLySpacing(long value)
{
	m_nLySpacing = value;
}
void CrackPreventionShearBarTemplate::SetPlaceSpacer(BOOL value)
{
	m_bPlaceSpacer = value;
}
void CrackPreventionShearBarTemplate::SetSpacerHookLenAType(long value)
{
	m_nSpacerHookLenAType = value;
}
void CrackPreventionShearBarTemplate::SetSpacerHookLenA(long value)
{
	m_nSpacerHookLenA = value;
}
void CrackPreventionShearBarTemplate::SetSpacerHookLenAUser(long value)
{
	m_nSpacerHookLenAUser = value;
}
void CrackPreventionShearBarTemplate::SetSpacerHookLenBType(long value)
{
	m_nSpacerHookLenBType = value;
}
void CrackPreventionShearBarTemplate::SetSpacerHookLenB(long value)
{
	m_nSpacerHookLenB = value;
}
void CrackPreventionShearBarTemplate::SetSpacerHookLenBUser(long value)
{
	m_nSpacerHookLenBUser = value;
}
void CrackPreventionShearBarTemplate::SetSpacerWidth(long value)
{
	m_nSpacerWidth = value;
}

//

CrackPreventionShearBarRebarInfo::CrackPreventionShearBarRebarInfo()
{
	CString sBar = MSDeformedBar::GetStirrupBarStr(1500);
	m_HBars.InitData(sBar);
	m_VBars.InitData(sBar);

	m_nStartPlacementNumber = 1;
	m_nEndPlacementNumber = 1;
}
CrackPreventionShearBarRebarInfo::CrackPreventionShearBarRebarInfo(CrackPreventionShearBarRebarInfo *pInfo)
{
	m_nStartPlacementNumber = pInfo->GetStartPlacementNumber();
	m_nEndPlacementNumber = pInfo->GetEndPlacementNumber();
	m_HBars.CopyFromMe(&pInfo->m_HBars);
	m_VBars.CopyFromMe(&pInfo->m_VBars);
}
CrackPreventionShearBarRebarInfo::~CrackPreventionShearBarRebarInfo()
{
}
void CrackPreventionShearBarRebarInfo::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_nStartPlacementNumber;
		ar << m_nEndPlacementNumber;

		m_HBars.Serialize(ar);
		m_VBars.Serialize(ar);
	}
	else
	{
		ar >> m_nStartPlacementNumber;
		ar >> m_nEndPlacementNumber;

		m_HBars.Serialize(ar);
		m_VBars.Serialize(ar);
	}	
}
long CrackPreventionShearBarRebarInfo::GetStartPlacementNumber()
{
	return m_nStartPlacementNumber;
}
void CrackPreventionShearBarRebarInfo::SetStartPlacementNumber(long value)
{
	m_nStartPlacementNumber = value;
}
long CrackPreventionShearBarRebarInfo::GetEndPlacementNumber()
{
	return m_nEndPlacementNumber;
}
void CrackPreventionShearBarRebarInfo::SetEndPlacementNumber(long value)
{
	m_nEndPlacementNumber = value;
}