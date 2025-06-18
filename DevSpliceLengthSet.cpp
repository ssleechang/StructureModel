#include "StdAfx.h"
#include "DevSpliceLengthSet.h"
#include "MSDeformedBar.h"

#include "MDMember.h"
#include "MSCompoG.h"
#include "MSCompoM.h"
#include "MSConcrete.h"
#include "MSSrcMaterial.h"
#include "MSRcSlabM.h"
#include "MSConcrete.h"
#include "ReinforceBarSetting.h"
#include "MSReinforceBar.h"
#include "MSLineLevelDifG.h"
#include "MSSlabG.h"

#include "StructureModel/MSRebarBuilding.h"

using namespace RebarOption;

DevSpliceLengthSet::DevSpliceLengthSet(CString name)
{
	Init();
	m_sName = name;
}


DevSpliceLengthSet::DevSpliceLengthSet()
{
	Init();
}

DevSpliceLengthSet::~DevSpliceLengthSet(void)
{
}

void DevSpliceLengthSet::Init()
{
	m_sName = _T("");

	m_sReinforceBarSetting = _T("");
	m_nMemberType = 0;
	m_nDevSplType = DEVSPL_CALC_DETAIL;
	m_nCeilDigit = 0;
	m_bEpoxyFactor = FALSE;
	m_LightWeightConc = FALSE;
	m_SimpleCalculation = FALSE;
}

void DevSpliceLengthSet::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_sName;
		ar << m_nMemberType;
		ar << m_sReinforceBarSetting;
		ar << (int)m_nDevSplType;
		ar << m_nCeilDigit;
		ar << m_bEpoxyFactor;
		ar << m_LightWeightConc;
		ar << m_SimpleCalculation;
		ar << (int)mm_MemberLength.size();
		for(auto it = mm_MemberLength.begin(); it != mm_MemberLength.end(); it++)
		{
			ar << it->first;
			it->second.Serialize(ar);
		}
		ar << (int)mm_CommonLength.size();
		for(auto it = mm_CommonLength.begin(); it != mm_CommonLength.end(); it++)
		{
			ar << it->first;
			it->second.Serialize(ar);
		}
		ar << (int)mm_HookLength.size();
		for(auto it = mm_HookLength.begin(); it != mm_HookLength.end(); it++)
		{
			ar << it->first;
			it->second.Serialize(ar);
		}

	}
	else
	{
		ar >> m_sName;
		ar >> m_nMemberType;
		ar >> m_sReinforceBarSetting;
		int type;
		ar >> type;
		m_nDevSplType = (DevSpl_Type)type;
		ar >> m_nCeilDigit;
		ar >> m_bEpoxyFactor;
		ar >> m_LightWeightConc;
		ar >> m_SimpleCalculation;
		int size;
		CString key, newKey;
		ar >> size;
		mm_MemberLength.clear();
		for(int i=0; i< size; i++)
		{
			ar >> key;
			MemberLengthItem item;
			item.Serialize(ar);

			if (MSVersionInfo::GetCurrentVersion() < 20160719)
				newKey = DevSpliceLengthKeyParsing(key);
			else
				newKey = key;

			mm_MemberLength.insert(make_pair(newKey,item));
		}

		ar >> size;
		mm_CommonLength.clear();
		for(int i=0; i< size; i++)
		{
			ar >> key;
			CommonLengthItem item;
			item.Serialize(ar);

			if (MSVersionInfo::GetCurrentVersion() < 20160719)
				newKey = DevSpliceLengthKeyParsing(key);
			else
				newKey = key;

			mm_CommonLength.insert(make_pair(newKey,item));
		}
		ar >> size;
		mm_HookLength.clear();
		for(int i=0; i< size; i++)
		{
			ar >> key;
			HookLengthItem item;
			item.Serialize(ar);
			mm_HookLength.insert(make_pair(key,item));
		}

		if (MSVersionInfo::GetCurrentVersion() < 20160202)
			Initialize(false);

		if ((MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() < 20231224) || MSVersionInfo::IsOldTotalVersion())
		{
			if(m_nMemberType >= 4)
				m_nMemberType++;

			MakeLowWallFromBWall();			
		}
	}
}
void DevSpliceLengthSet::MakeLowWallFromBWall()
{
	for(auto Iter : mm_MemberLength)
	{
		CString sKey = Iter.first;
		if(sKey.Find(STR_MEMBERTYPE_WALL) == 0)
		{
			sKey.Replace(STR_MEMBERTYPE_WALL, STR_MEMBERTYPE_LOWWALL);
			if(mm_MemberLength.find(sKey) == mm_MemberLength.end())
			{
				MemberLengthItem lowWallDevSplInfo;
				lowWallDevSplInfo.Copy(Iter.second);
				mm_MemberLength.insert(make_pair(sKey, lowWallDevSplInfo));
			}
		}
	}
}
CString DevSpliceLengthSet::DevSpliceLengthKeyParsing(CString key)
{
	CString newKey = key;
	
	int nDotIndex = newKey.ReverseFind('.');
	int nDashIndex = newKey.ReverseFind('-');

	if (nDotIndex != -1 && (nDashIndex - nDotIndex == 2))
		return newKey;

	newKey.Insert(nDashIndex, _T(".0"));

	return newKey;
}
long DevSpliceLengthSet::GetFckNameList(CStringArray& arNameList)
{
	arNameList.RemoveAll();

	if (MSDeformedBar::GetRebarCode() == MSDeformedBar::REBAR_MATERIAL_GB)
	{
		arNameList.Add(_T("20.0"));
		arNameList.Add(_T("25.0"));
	}
	else
	{
		arNameList.Add(_T("21.0"));
		arNameList.Add(_T("24.0"));
	}

	return arNameList.GetSize();
}
long DevSpliceLengthSet::GetFyNameList(CStringArray& arNameList)
{
	arNameList.RemoveAll();
	arNameList.Add(_T("SD300"));
	arNameList.Add(_T("SD350"));
	arNameList.Add(_T("SD400"));
	arNameList.Add(_T("SD500"));		
	arNameList.Add(_T("SD600"));
	arNameList.Add(_T("SD700"));
	return arNameList.GetSize();
}
void DevSpliceLengthSet::Initialize(bool bFullUpdate)
{
	if (bFullUpdate)
	{
		m_nMemberType = MEMBERTYPE_FOUNDATION;
		m_sReinforceBarSetting = _T("Default");
		m_nDevSplType = DEVSPL_USER_GEN;
		m_nCeilDigit = 0;
		m_bEpoxyFactor = FALSE;
		m_LightWeightConc = FALSE;
		m_SimpleCalculation = TRUE;

		mm_MemberLength.clear();
		mm_CommonLength.clear();
		mm_HookLength.clear();
	}

	CStringArray MaterialNames;
	GetFckNameList(MaterialNames);
	
	CStringArray FyNames;
	GetFyNameList(FyNames);

	CStringArray arNameList;
	MSDeformedBar::GetRebarNameList(arNameList);

	CString strKey;
	double dDia = 0;
	for(long jFck = 0 ; jFck < MaterialNames.GetSize() ; jFck++)
	{
		for(long kDia = 0 ; kDia < arNameList.GetSize() ; kDia++)
		{
			CString strFck = MaterialNames[jFck];

			for(long iFy = 0 ; iFy < FyNames.GetSize() ; iFy++)
			{
				CString strFy = FyNames[iFy];
				//새로 정비
				//Slab///////////////////
				//General = 0---------
				MemberLengthItem dSlabDevSplInfo;
				dSlabDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_SLAB, arNameList[kDia], TRUE, strFck,strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dSlabDevSplInfo));

				// Top = 1-----------
				MemberLengthItem dTopSlabDevSplInfo;
				dTopSlabDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_SLAB, arNameList[kDia], FALSE, strFck,strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dTopSlabDevSplInfo));

				//DeckSlab///////////////////
				//General = 0---------
				MemberLengthItem dDeckSlabDevSplInfo;
				dDeckSlabDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_DECKSLAB, arNameList[kDia], TRUE, strFck, strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dDeckSlabDevSplInfo));

				// Top = 1-----------
				MemberLengthItem dTopDeckSlabDevSplInfo;
				dTopDeckSlabDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_DECKSLAB, arNameList[kDia], FALSE, strFck, strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dTopDeckSlabDevSplInfo));

				/////////////////////////
				//Wall///////////////////
				//General = 0---------
				MemberLengthItem dWallDevSplInfo;
				dWallDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_WALL, arNameList[kDia], TRUE, strFck,strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dWallDevSplInfo));

				// Top = 1-----------
				MemberLengthItem dTopWallDevSplInfo;
				dTopWallDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_WALL, arNameList[kDia], FALSE, strFck,strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dTopWallDevSplInfo));

				/////////////////////////
				//RWall///////////////////
				//General = 0---------
				MemberLengthItem dRWallDevSplInfo;
				dRWallDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_RWALL, arNameList[kDia], TRUE, strFck, strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dRWallDevSplInfo));

				// Top = 1-----------
				MemberLengthItem dTopRWallDevSplInfo;
				dTopRWallDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_RWALL, arNameList[kDia], FALSE, strFck, strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dTopRWallDevSplInfo));

				/////////////////////////
				//LowWall///////////////////
				//General = 0---------
				MemberLengthItem dLowWallDevSplInfo;
				dLowWallDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_LOWWALL, arNameList[kDia], TRUE, strFck, strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dLowWallDevSplInfo));

				// Top = 1-----------
				MemberLengthItem dTopLowWallDevSplInfo;
				dTopLowWallDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_LOWWALL, arNameList[kDia], FALSE, strFck, strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dTopLowWallDevSplInfo));

				/////////////////////////
				//Beam///////////////////
				//General = 0---------
				MemberLengthItem dBeamDevSplInfo;
				dBeamDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_BEAM, arNameList[kDia], TRUE, strFck,strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dBeamDevSplInfo));

				// Top = 1-----------
				MemberLengthItem dTopBeamDevSplInfo;
				dTopBeamDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_BEAM, arNameList[kDia], FALSE, strFck,strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dTopBeamDevSplInfo));


				/////////////////////////
				//Column///////////////////
				//General = 0---------
				MemberLengthItem dColumnDevSplInfo;
				dColumnDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_COLUMN, arNameList[kDia], TRUE, strFck,strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dColumnDevSplInfo));

				// Top = 1-----------
				MemberLengthItem dTopColumnDevSplInfo;
				dTopColumnDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_COLUMN, arNameList[kDia], FALSE, strFck,strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dTopColumnDevSplInfo));

				/////////////////////////
				//Footing///////////////////
				//General = 0---------
				MemberLengthItem dFoundDevSplInfo;
				dFoundDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_FOUNDATION, arNameList[kDia], TRUE, strFck,strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dFoundDevSplInfo));

				// Top = 1-----------
				MemberLengthItem dTopFoundDevSplInfo;
				dTopFoundDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_FOUNDATION, arNameList[kDia], FALSE, strFck,strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dTopFoundDevSplInfo));

				//SRC Column///////////////////
				//General = 0---------
				MemberLengthItem dSRCColumnDevSplInfo;
				dSRCColumnDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_SRCCOLUMN, arNameList[kDia], TRUE, strFck, strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dSRCColumnDevSplInfo));

				// Top = 1-----------
				MemberLengthItem dTopSRCColumnDevSplInfo;
				dTopSRCColumnDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_SRCCOLUMN, arNameList[kDia], FALSE, strFck, strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dTopSRCColumnDevSplInfo));

				//SRC Beam///////////////////
				//General = 0---------
				MemberLengthItem dSRCBeamDevSplInfo;
				dSRCBeamDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_SRCBEAM, arNameList[kDia], TRUE, strFck, strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dSRCBeamDevSplInfo));

				// Top = 1-----------
				MemberLengthItem dTopSRCBeamDevSplInfo;
				dTopSRCBeamDevSplInfo.Initialize();
				strKey.Format(_T("%s-%s-%ld-%s-%s"), STR_MEMBERTYPE_SRCBEAM, arNameList[kDia], FALSE, strFck, strFy);
				if (mm_MemberLength.find(strKey) == mm_MemberLength.end())
					mm_MemberLength.insert(make_pair(strKey, dTopSRCBeamDevSplInfo));

				//////////////////////////////////////////////////////////////////////////
				//Common
				CommonLengthItem dCommonInfo;
				dCommonInfo.Initialize();
				strKey.Format(_T("%s-%s-%s-%s"), STR_MEMBERTYPE_COMMON, arNameList[kDia], strFck,strFy);
				if (mm_CommonLength.find(strKey) == mm_CommonLength.end())
					mm_CommonLength.insert(make_pair(strKey, dCommonInfo));
			}

			//////////////////////////////////////////////////////////////////////////
			//Hook

			//MainBar
			dDia = MSDeformedBar::FindDeformedBarDiameterByIndex(kDia);
			strKey.Format(_T("%s-%s-%ld"), STR_MEMBERTYPE_HOOK, arNameList[kDia], FALSE);
			HookLengthItem dHookInfo;
			dHookInfo.Initialize();
			dHookInfo.m_dMainbarHookLength90  = ROUND(CEIL(CalcHookLength(dDia, false, 90), 10));
			dHookInfo.m_dMainbarHookLength180 = ROUND(CEIL(CalcHookLength(dDia, false, 180),10));
			dHookInfo.m_dStirrupHookLength90  = ROUND(CEIL(CalcHookLength(dDia, true, 90), 10));
			dHookInfo.m_dStirrupHookLength135 = ROUND(CEIL(CalcHookLength(dDia, true, 135),10));
			if (mm_HookLength.find(strKey) == mm_HookLength.end())
				mm_HookLength.insert(make_pair(strKey, dHookInfo));
		}
	}
}
CString DevSpliceLengthSet::GetHookKey(double  dDia, bool IsStirrup) const
{
	CString strMember = STR_MEMBERTYPE_HOOK;
	CString strKey;
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);

	strKey.Format(_T("%s-%s-%ld"), strMember, sDia, FALSE);
	return strKey;
}
CString DevSpliceLengthSet::GetHookKey4DiaIndex(long nDiaIndex)
{
	CString strMember = STR_MEMBERTYPE_HOOK;
	CString strKey;
	CString sDia = MSDeformedBar::FindBarDiameterNameByIndex(nDiaIndex);
	strKey.Format(_T("%s-%s-%ld"), strMember, sDia, FALSE);
	return strKey;
}
CString DevSpliceLengthSet::GetDevSplKey(CString strMember, double dDia, double dFck, long IsTop, double dFy) const
{
	CString strKey;
	CString strFck = GetConMatKeyName(dFck);
	CString strFy = GetFyKeyName(dFy);
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	strKey.Format(_T("%s-%s-%ld-%s-%s"), strMember, sDia, IsTop, strFck,strFy);
	return strKey;
}

CString DevSpliceLengthSet::GetDevSplKey(long nDiaIndex, long IsTop, double dFy, double dFck)
{
	CString strMember = GetMemberName(m_nMemberType);
	double dDia = MSDeformedBar::FindDeformedBarDiameterByIndex(nDiaIndex);
	CString strKey = GetDevSplKey(strMember, dDia, dFck, IsTop, dFy);
	return strKey;
}

CString DevSpliceLengthSet::GetCommonKey(CString strMember,double dDia, double dFck,double dFy) const
{
	CString strKey;
	CString strFck = GetConMatKeyName(dFck);
	CString strFy = GetFyKeyName(dFy);
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);

	strKey.Format(_T("%s-%s-%s-%s"), _T("COMMON"), sDia, strFck,strFy);
	return strKey;
}

CString DevSpliceLengthSet::GetCommonKey(long nDiaIndex, double dFck, double dFy)
{
	double dDia = MSDeformedBar::FindDeformedBarDiameterByIndex(nDiaIndex);
	CString strKey = GetCommonKey(_T(""), dDia, dFck, dFy);
	return strKey;
}

double DevSpliceLengthSet::GetFck(MSCompoG* pMSCompoG) const
{
	if(pMSCompoG->GetMaterial())
		return pMSCompoG->GetMaterial()->GetFck();
	return 0;
}
double DevSpliceLengthSet::GetFck(MSMaterial* pMat) const
{
	if(pMat)
		return pMat->GetFck();
	return 0;
}
double DevSpliceLengthSet::GetFy(MSCompoG* pMSCompoG, double dDia) const
{
	MSCompoM* pMSCompoM = pMSCompoG->GetCompoM();
	if (pMSCompoM == NULL)
	{
		if (pMSCompoG->GetType() == MSElement::msLineLevelDifG)
		{
			MSLineLevelDifG* pMSLevelDifG = (MSLineLevelDifG*)pMSCompoG;
			if (pMSLevelDifG->mp_LeftSlab)
				pMSCompoM = pMSLevelDifG->mp_LeftSlab->GetCompoM();
		}
	}
	if (pMSCompoM == NULL)
		return 400;

	MSConcrete* pConcrete = NULL;
	MSMaterial* pMat = (MSMaterial*)pMSCompoG->GetMaterial();
	if (pMat)
		return GetFy(pMat,dDia);
	return 400;
}
double DevSpliceLengthSet::GetFy(MSMaterial* pMat,double dDia) const
{
	if (pMat == nullptr)
		return 400;
	CString sDiaName = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
	MSConcrete* pConcrete = NULL;

	if (pMat->GetType() == MSMaterial::msConcrete)
		pConcrete = (MSConcrete*)pMat;
	else if (pMat->GetType() == MSMaterial::msSRC)
		pConcrete = (MSConcrete*)((MSSrcMaterial*)pMat)->mp_RcMaterial;
	
	if (pConcrete != NULL && pConcrete->m_pReinforceBarSetting != NULL)
	{
		auto it = pConcrete->m_pReinforceBarSetting->mm_IndexToReinforceBar.find(DiaIndex);
		if (it != pConcrete->m_pReinforceBarSetting->mm_IndexToReinforceBar.end())
		{
			MSReinforceBar* pBarMat = (MSReinforceBar*)it->second;
			if(pBarMat)
				return pBarMat->m_dBarStrength;
		}
	}
	return 400;
}
CString DevSpliceLengthSet::GetConMatKeyName(double dFck) const
{
	CString strFck = _T("21");

	//소수점 1자리에서 반올림
	int p = pow(10, 1);
	double dNewValue = floor((dFck*p) + 0.5f) / p;
	strFck.Format(_T("%.1f"), dNewValue);
	
	return strFck;
}
CString DevSpliceLengthSet::GetFyKeyName(double dFy) const
{
	CString strFy = _T("SD400");
	if (300 - DTOL5 < ROUND(dFy) && ROUND(dFy) < 300 + DTOL5)  strFy = _T("SD300");
	else if(350 - DTOL5 < ROUND(dFy) && ROUND(dFy) < 350 + DTOL5)  strFy = _T("SD350");
	else if(400 - DTOL5 < ROUND(dFy) && ROUND(dFy) < 400 + DTOL5)  strFy = _T("SD400");
	else if(500 - DTOL5 < ROUND(dFy) && ROUND(dFy) < 500 + DTOL5)  strFy = _T("SD500");
	else if(600 - DTOL5 < ROUND(dFy) && ROUND(dFy) < 600 + DTOL5)  strFy = _T("SD600");
	else if(700 - DTOL5 < ROUND(dFy) && ROUND(dFy) < 700 + DTOL5)  strFy = _T("SD700");
	else if(800 - DTOL5 < ROUND(dFy) && ROUND(dFy) < 800 + DTOL5)  strFy = _T("SD800");
	return strFy;
}
CString DevSpliceLengthSet::GetMemberName(long MemberType)
{
	CString strFck = STR_MEMBERTYPE_FOUNDATION;
	switch (MemberType)
	{
	case MEMBERTYPE_COLUMN:
		strFck = STR_MEMBERTYPE_COLUMN;
		break;
	case MEMBERTYPE_WALL:
		strFck = STR_MEMBERTYPE_WALL;
		break;
	case MEMBERTYPE_RWALL:
		strFck = STR_MEMBERTYPE_RWALL;
		break;
	case MEMBERTYPE_LOWWALL:
		strFck = STR_MEMBERTYPE_LOWWALL;
		break;
	case MEMBERTYPE_BEAM:
		strFck = STR_MEMBERTYPE_BEAM;
		break;
	case MEMBERTYPE_SLAB:
		strFck = STR_MEMBERTYPE_SLAB;
		break;
	case MEMBERTYPE_DECKSLAB:
		strFck = STR_MEMBERTYPE_DECKSLAB;
		break;
	case MEMBERTYPE_SRCCOLUMN:
		strFck = STR_MEMBERTYPE_SRCCOLUMN;
		break;
	case MEMBERTYPE_SRCBEAM:
		strFck = STR_MEMBERTYPE_SRCBEAM;
		break;
	case MEMBERTYPE_COMMON:
		strFck = STR_MEMBERTYPE_COMMON;
		break;
	case MEMBERTYPE_HOOK:
		strFck = STR_MEMBERTYPE_HOOK;
		break;
	default:
		break;
	}
	return strFck;
}
double DevSpliceLengthSet::GetCover(MSCompoG* pMSCompoG) const
{
	MSCompoM* pMSCompoM = pMSCompoG->GetCompoM();
	if(pMSCompoM)
	{
		MDMember* pMDMember = pMSCompoG->GetRebarM();
		if(pMDMember == NULL)
			return 20;
		return pMDMember->GetCover();
	}
	return 20;
}
double DevSpliceLengthSet::GetDepthBelow(MSCompoG* pMSCompoG) const
{
	double dDep = 500;
	//상부일때만 함수가 호출된다.
	//슬래브, 벽수평근,기초,보
	long nType = pMSCompoG->GetType();
	if(nType == MSCompoG::msSlabG)
		dDep = pMSCompoG->GetThick();
	else if(nType ==  MSCompoG::msBeamG)
		dDep = pMSCompoG->GetDepth();

	return dDep;
}

double DevSpliceLengthSet::SqrtFck(double Fck) const
{
	double SqrtFck = sqrt(fabs(Fck));

	//0508.2.1.2
	if(SqrtFck > 8.37)	SqrtFck = 8.37;
	return SqrtFck;
}

CString DevSpliceLengthSet::GetMemberType(MSCompoG* pMSCompoG) const
{
	CString strMemberType = 0;
	long nType = pMSCompoG->GetType();
	if(nType == MSCompoG::msOpeningG)
	{
		ASSERT(0);
		strMemberType = STR_MEMBERTYPE_SLAB;
	}
	if(nType == MSCompoG::msSlabG)
	{
		strMemberType = STR_MEMBERTYPE_SLAB;

		MSRcSlabM* pSlabM = (MSRcSlabM*)pMSCompoG->GetRebarM();
		if(pSlabM == NULL)
			ASSERT(0);
		
		if (pSlabM != NULL)
		{
			if (pSlabM->m_nSlabMType == MSRcSlabM::SLAB_TYPE::MAT_TYPE)//매트
				strMemberType = STR_MEMBERTYPE_FOUNDATION;
			else if (pSlabM->m_nSlabMType == MSRcSlabM::SLAB_TYPE::DECK_TYPE ||//Deck
				pSlabM->m_nSlabMType == MSRcSlabM::SLAB_TYPE::DECK_GENERAL)//골데크
				strMemberType = STR_MEMBERTYPE_DECKSLAB;
			//RebarTemplate 설정 반영
			else if(pMSCompoG->GetRebarTemplate()!=NULL)
			{
				if(pSlabM->m_nSlabMType == MSRcSlabM::SLAB_TYPE::SOG && pMSCompoG->GetRebarTemplate()->GetSOGSpliceDevSlabType()==0)
					strMemberType = STR_MEMBERTYPE_FOUNDATION;
				else if(pSlabM->m_nSlabMType == MSRcSlabM::SLAB_TYPE::CIRCULAR_RAMP && pMSCompoG->GetRebarTemplate()->GetCircularRampSpliceDevSlabType()==0)
					strMemberType = STR_MEMBERTYPE_FOUNDATION;
				else if(pSlabM->m_nSlabMType == MSRcSlabM::SLAB_TYPE::FLAT_TYPE && pMSCompoG->GetRebarTemplate()->GetFlatSpliceDevSlabType()==0)
					strMemberType = STR_MEMBERTYPE_FOUNDATION;
				else if(pSlabM->m_nSlabMType == MSRcSlabM::SLAB_TYPE::TRANSFER && pMSCompoG->GetRebarTemplate()->GetTransferSlabSpliceDevSlabType()==0)
					strMemberType = STR_MEMBERTYPE_FOUNDATION;
				else
					strMemberType = STR_MEMBERTYPE_SLAB;
			}

		}
	}
	else if (nType == MSCompoG::msWallG || nType == MSCompoG::msRampG)
	{
		if(nType == MSCompoG::msWallG && pMSCompoG->IsRetainingWall())
			strMemberType = STR_MEMBERTYPE_RWALL;
		else
			strMemberType = STR_MEMBERTYPE_WALL;
	}
	else if(nType ==  MSCompoG::msStairG)
		strMemberType = STR_MEMBERTYPE_SLAB;
	else if (nType == MSCompoG::msBeamG)
	{
		if (pMSCompoG->IsSRC())
			strMemberType = STR_MEMBERTYPE_SRCBEAM;
		else
			strMemberType = STR_MEMBERTYPE_BEAM;
	}
	else if (nType == MSCompoG::msColumnG)
	{
		if (pMSCompoG->IsSRC())
			strMemberType = STR_MEMBERTYPE_SRCCOLUMN;
		else
			strMemberType = STR_MEMBERTYPE_COLUMN;
	}
	else if(nType == MSCompoG::msIsoFootG|| nType== MSCompoG::msWallFootG||	nType == MSCompoG::msPitG)
		strMemberType = STR_MEMBERTYPE_FOUNDATION;
	else if(nType == MSCompoG::msHandRailG)
		strMemberType = STR_MEMBERTYPE_LOWWALL;
	else
	{
		auto pOwnerCompoG = pMSCompoG->GetOwnerCompoG();
		if(pOwnerCompoG != nullptr)
		{
			if(pOwnerCompoG->GetType() == MSCompoG::msWallG && pOwnerCompoG->IsRetainingWall())
				return STR_MEMBERTYPE_RWALL;
		}
		strMemberType = STR_MEMBERTYPE_WALL;
	}

	return strMemberType;
}

double DevSpliceLengthSet::GetHookLength( double dDia, bool IsStirrup /*= 0*/, long nDeg /*= 90*/ ) const
{
	double dHookLen = 0.;
	HookLengthItem HookInfo;	
	if(m_nDevSplType == DEVSPL_USER_GEN )
	{
		CString Key = GetHookKey(dDia,FALSE);
		auto it = mm_HookLength.find(Key);
		if(it != mm_HookLength.end())
			dHookLen = it->second.GetHookLength(IsStirrup, nDeg);
		else
		{
			//CLogger::GetInstance()->WriteLogFormat(_T("%s: %s,Dia:%ld,Deg:%ld"),_T("Error!"),_T("GetHookLength"),nDia,nDeg);
		ASSERT(0);
			return 10;
		}
	}
	else if(m_nDevSplType == DEVSPL_CALC_DETAIL || m_nDevSplType == DEVSPL_CALC_SIMPLE)
	{
		dHookLen = CalcHookLength(dDia, IsStirrup, nDeg);
		dHookLen = CEIL(dHookLen, m_nCeilDigit);
	}
	if(dHookLen<DTOL_GM)
	{
		//CLogger::GetInstance()->WriteLogFormat(_T("%s: %s,Dia:%ld,Deg:%ld"),_T("Error!"),_T("GetHookLength"),nDia,nDeg);
		return 10;
	}
	return dHookLen;
}


double DevSpliceLengthSet::GetDevLengthTension(MSCompoG* pSupportG,MSCompoG* pMSCompoG,double dDia, long IsTop, double dBarDist,long nMaterialID) const
{
	double dDevelopLen =0;
	double dFck = GetFck(pSupportG);
	double dCover = GetCover(pSupportG);
	double dFy = GetFy(pMSCompoG,dDia);

	if (nMaterialID && pMSCompoG->GetBuilding())
	{
		MSMaterial* pMSMaterial = pMSCompoG->GetBuilding()->GetMaterial(nMaterialID);
		dFy = GetFy(pMSMaterial, dDia);
		dFck = GetFck(pMSMaterial);
	}

	if(m_nDevSplType == DEVSPL_USER_GEN)
	{
		CString Key = GetDevSplKey(GetMemberType(pMSCompoG),dDia, dFck,IsTop,dFy);
		auto it = mm_MemberLength.find(Key);
		if(it != mm_MemberLength.end())//부재별 정착길이 or A급 이음길이
			dDevelopLen = it->second.m_dDevelopmentLength;
		else
		{
			//	CLogger::GetInstance()->WriteLogFormat(_T("%s: %s,Dia:%ld,Spacing:%0.0f","Error!","GetDevLengthTension"),nDia,dBarDist);
//		ASSERT(0);
			return 10;
		}
	}
	else if(m_nDevSplType == DEVSPL_CALC_DETAIL || m_nDevSplType == DEVSPL_CALC_SIMPLE)
	{
		double DepthBelow = 299;
		if(IsTop)
			DepthBelow = GetDepthBelow(pMSCompoG);
		double CorrectionFactor = 1.0; //보정계수
		if(dDia <= 19)
		{
			if(dBarDist >= 2*dDia && dCover >= dDia) CorrectionFactor = 0.8;
			else CorrectionFactor = 1.2;
		}
		else
		{
			if(dBarDist >= 2*dDia && dCover >= dDia) CorrectionFactor = 1.0;
			else CorrectionFactor = 1.5;
		}

		double DistFactor = 1.0; //철근간격계수
		double C_Factor = 1.0;
		double D_Factor = 1.0;
		C_Factor = max(dBarDist - dDia,dDia);//dBarDist == 0인경우 마이너스값이 생길 수 있음, 최소값을 철근지름
		D_Factor = dCover+dDia/2;
		DistFactor = (min(C_Factor, D_Factor))/dDia;
		if(DistFactor > 2.5) DistFactor = 2.5;

		dDevelopLen = CalcDevLength(dDia, dFck, dFy, CorrectionFactor, m_nDevSplType, DistFactor, DepthBelow);
		dDevelopLen = CEIL(dDevelopLen, m_nCeilDigit);
	}
	if(dDevelopLen<DTOL_GM)
	{
	//	//	CLogger::GetInstance()->WriteLogFormat(_T("%s: %s,Dia:%ld,Spacing:%0.0f","Error!","GetDevLengthTension"),nDia,dBarDist);
	//	ASSERT(0);
		return 10;
	}
	return dDevelopLen;
}

double DevSpliceLengthSet::GetDevLengthComp(MSCompoG* pSupportG,MSCompoG* pMSCompoG,double dDia, long IsTop, double TieBarDist, double TieBarDia) const
{
	double dDevelopLen = 0;
	double dFck = GetFck(pSupportG);
	double dFy = GetFy(pMSCompoG,dDia);

	if(m_nDevSplType == DEVSPL_USER_GEN)
	{
#ifdef _DEBUG	
		StatusReporterWrapper::OutputWarning(_T("압축정착길이를 호출하였습니다"));
#endif
		return 200;//임시
		//long nListType = GetSplType(pMSCompoG);
		//CString Key = GetDevSplKey(nDia, dFck, dFy, IsTop, INPUT_COMMON);
		//RAPOption_DevSpliceLen::ms_Common_CompMap.Lookup(Key, dDevelopLen);
	}
	else if(m_nDevSplType == DEVSPL_CALC_DETAIL || m_nDevSplType == DEVSPL_CALC_SIMPLE)
	{
		double dCorrectiveFactor = 1.0;
		if(TieBarDist <=100 && TieBarDia == 13) dCorrectiveFactor *=0.75;

		dDevelopLen = CalcDevLengthComp(dDia, dFck, dFy, dCorrectiveFactor);
		dDevelopLen = CEIL(dDevelopLen, m_nCeilDigit);
	}
	if(dDevelopLen<DTOL_GM)
	{
		//	CLogger::GetInstance()->WriteLogFormat(_T("%s: %s,Dia:%ld","Error!","GetDevLengthComp"),nDia);
//		ASSERT(0);
		return 10;
	}

	return dDevelopLen;
}

double DevSpliceLengthSet::GetSpliceLengthA(MSCompoG* pMSCompoG,double dDia, long IsTop, double dBarDist) const
{
	double dSpliceLen = 0;
	double dFck = GetFck(pMSCompoG);
	double dCover = GetCover(pMSCompoG);
	double dFy = GetFy(pMSCompoG,dDia);

	if(m_nDevSplType == DEVSPL_USER_GEN)
	{
		//A급이음==인장정착길이
		CString Key = GetDevSplKey(GetMemberType(pMSCompoG),dDia, dFck,IsTop,dFy);
		auto it = mm_MemberLength.find(Key);
		if(it != mm_MemberLength.end())
			dSpliceLen = it->second.m_dDevelopmentLength;
		else
		{
			//	CLogger::GetInstance()->WriteLogFormat(_T("%s: %s,Dia:%ld","Error!","GetSpliceLengthA"),nDia);
//		ASSERT(0);
			return 10;
		}
	}
	else if(m_nDevSplType == DEVSPL_CALC_DETAIL || m_nDevSplType == DEVSPL_CALC_SIMPLE)
	{
		double DepthBelow = 299;
		if(IsTop)
			DepthBelow = GetDepthBelow(pMSCompoG);

		double CorrectionFactor = 1.0; //보정계수
		if(dDia - 19 <= 1)
		{
			if(dBarDist >= 2*dDia && dCover >= dDia) CorrectionFactor = 0.8;
			else CorrectionFactor = 1.2;
		}
		else
		{
			if(dBarDist >= 2*dDia && dCover >= dDia) CorrectionFactor = 1.0;
			else CorrectionFactor = 1.5;
		}

		double DistFactor = 1.0; //철근간격계수
		double C_Factor = 1.0;
		double D_Factor = 1.0;
		C_Factor = dBarDist-dDia;
		D_Factor = dCover+dDia/2;
		DistFactor = (min(C_Factor, D_Factor))/dDia;
		if(DistFactor > 2.5) DistFactor = 2.5;
		dSpliceLen = CalcSpliceLengthA(dDia, dFck, dFy,CorrectionFactor, m_nDevSplType, DistFactor, DepthBelow);
		dSpliceLen = CEIL(dSpliceLen, m_nCeilDigit);
	}
	if(dSpliceLen<DTOL_GM)
	{
		//CLogger::GetInstance()->WriteLogFormat(_T("%s: %s,Dia:%ld","Error!","GetSpliceLengthA"),nDia);
		ASSERT(0);
		return 10;
	}

	return dSpliceLen;
}

double DevSpliceLengthSet::GetSpliceLengthB(MSCompoG* pMSCompoG, double dDia, long IsTop, double dBarDist,double dFckSupportG,long nMaterialID) const
{
	double dSpliceLen = 0.0;

	double dFck = 0;
	if (dFckSupportG > DTOL1)
		dFck = dFckSupportG;
	else
		dFck = GetFck(pMSCompoG);
	double dCover = GetCover(pMSCompoG);
	double dFy = GetFy(pMSCompoG,dDia);

	if (nMaterialID && pMSCompoG->GetBuilding())
	{
		MSMaterial* pMSMaterial = pMSCompoG->GetBuilding()->GetMaterial(nMaterialID);
		dFy = GetFy(pMSMaterial, dDia);
		if (dFckSupportG < DTOL1)
			dFck = GetFck(pMSMaterial);
	}

	if(m_nDevSplType == DEVSPL_USER_GEN)
	{
		CString Key = GetDevSplKey(GetMemberType(pMSCompoG),dDia, dFck,IsTop,dFy);
		auto it = mm_MemberLength.find(Key);
		if(it != mm_MemberLength.end())
			dSpliceLen = it->second.m_dSpliceLength;
		else
		{
			//CLogger::GetInstance()->WriteLogFormat(_T("%s: %s,Dia:%ld","Error!","GetSpliceLengthB"),nDia);
//		ASSERT(0);
			return 10;
		}
	}
	else if(m_nDevSplType == DEVSPL_CALC_DETAIL || m_nDevSplType == DEVSPL_CALC_SIMPLE)
	{
		double DepthBelow = 299;
		if(IsTop)
			DepthBelow = GetDepthBelow(pMSCompoG);

		double CorrectionFactor = 1.0; //보정계수
		if(dDia<19)
		{
			if(dBarDist >= 2*dDia && dCover >= dDia) CorrectionFactor = 0.8;
			else CorrectionFactor = 1.2;
		}
		else
		{
			if(dBarDist >= 2*dDia && dCover >= dDia) CorrectionFactor = 1.0;
			else CorrectionFactor = 1.5;
		}

		double DistFactor = 1.0; //철근간격계수
		double C_Factor = 1.0;
		double D_Factor = 1.0;
		C_Factor = (dBarDist)/2;
		D_Factor = dCover+dDia/2;
		DistFactor = (min(C_Factor, D_Factor))/dDia;
		if(DistFactor > 2.5) DistFactor = 2.5;
		dSpliceLen = CalcSpliceLengthB(dDia, dFck, dFy, CorrectionFactor, m_nDevSplType, DistFactor, DepthBelow);
		dSpliceLen = CEIL(dSpliceLen, m_nCeilDigit);
	}
	if(dSpliceLen<DTOL_GM)
	{
	//	ASSERT(0);
	//	//CLogger::GetInstance()->WriteLogFormat(_T("%s: %s,Dia:%ld","Error!","GetSpliceLengthB"),nDia);
		return 10;
	}

	return dSpliceLen;
}


double DevSpliceLengthSet::GetSpliceLengthComp(MSCompoG* pMSCompoG,double dDia, double dFckSupportG,long nMaterialID) const
{
	double dSpliceLen = 0.0;
	double dFck = 0;
	if (dFckSupportG > DTOL1)
		dFck = dFckSupportG;
	else
		dFck = GetFck(pMSCompoG);
	double dFy = GetFy(pMSCompoG,dDia);

	if (nMaterialID && pMSCompoG->GetBuilding())
	{
		MSMaterial* pMSMaterial = pMSCompoG->GetBuilding()->GetMaterial(nMaterialID);
		dFy = GetFy(pMSMaterial, dDia);
		if(dFckSupportG < DTOL1)
			dFck = GetFck(pMSMaterial);
	}

	if(m_nDevSplType == DEVSPL_USER_GEN)
	{
		CString Key = GetCommonKey(GetMemberType(pMSCompoG),dDia, dFck,dFy);
		auto it = mm_CommonLength.find(Key);
		if(it != mm_CommonLength.end())
			dSpliceLen = it->second.m_nCompLength;
		else
		{
			//	CLogger::GetInstance()->WriteLogFormat(_T("%s: %s,Dia:%ld","Error!","GetSpliceLengthComp"),nDia);
//		ASSERT(0);
			return 10;
		}

	}
	else if(m_nDevSplType == DEVSPL_CALC_DETAIL || m_nDevSplType == DEVSPL_CALC_SIMPLE)
	{
		dSpliceLen = CalcSpliceLengthComp(dDia, dFck, dFy);
		dSpliceLen = CEIL(dSpliceLen, m_nCeilDigit);
	}
	if(dSpliceLen<DTOL_GM)
	{
		//CLogger::GetInstance()->WriteLogFormat(_T("%s: %s,Dia:%ld","Error!","GetSpliceLengthComp"),nDia);
//		ASSERT(0);
		return 10;
	}

	return dSpliceLen;
}

double DevSpliceLengthSet::GetDevLength4Hook(MSCompoG* pSupportG,MSCompoG* pMSCompoG, double dDia, double EndCover, double StrpDist) const
{
	double dDevelopLen = 0.0;
	double dFck = GetFck(pSupportG);
	double SideCover = GetCover(pMSCompoG);
	double dFy = GetFy(pMSCompoG,dDia);

	if(m_nDevSplType == DEVSPL_USER_GEN)
	{
		CString Key = GetCommonKey(GetMemberType(pMSCompoG),dDia, dFck,dFy);
		auto it = mm_CommonLength.find(Key);
		if(it != mm_CommonLength.end())
			dDevelopLen = it->second.m_nDevLength4Hook;
		else
		{
			//	CLogger::GetInstance()->WriteLogFormat(_T("%s: %s,Dia:%ld","Error!","GetDevLength4Hook"),nDia);
//		ASSERT(0);
			return 10;
		}
	}
	else if(m_nDevSplType == DEVSPL_CALC_DETAIL || m_nDevSplType == DEVSPL_CALC_SIMPLE)
	{
		double dCorrectionFactor = 1.0; //보정계수
		if(SideCover>=70 && EndCover >=50) dCorrectionFactor *=0.7;
		if(dDia - 35 <= 1 && StrpDist <= 3*dDia) dCorrectionFactor *= 0.8;
		dCorrectionFactor *=dFy/(400/CODESTRESSUNIT);

		dDevelopLen = CalcDevLength4Hook(dDia, dFck, dFy, dCorrectionFactor);
		dDevelopLen = CEIL(dDevelopLen, m_nCeilDigit);
	}
	if(dDevelopLen<DTOL_GM)
	{
		//CLogger::GetInstance()->WriteLogFormat(_T("%s: %s,Dia:%ld","Error!","GetDevLength4Hook"),nDia);
		ASSERT(0);
		return 10;
	}

	return dDevelopLen;
}


//120215.ES.약산식/정밀식 반영-표준갈고리-KBC0505.2 표준갈고리
//구부림 최소반경값+지름 적용
double DevSpliceLengthSet::CalcHookLength(double Dia, bool IsStirrup, double dDeg) const
{
	double dA1 = 0;
	double dA2 = 0;
	double dHookLength = 0.;
	double dBendLen = 0;


	if(IsStirrup)//스터럽또는 띠철근일경우
	{
		if(Dia < 17)
			dA2 = 2*Dia+Dia;
		else
			dA2 = 3*Dia+Dia;
		if(fabs(fabs(dDeg) - 180) < DTOL1)
			dA1 = max(4 * Dia, 60);
		else if(fabs(fabs(dDeg) - 135) < DTOL1)
			dA1 = 6 * Dia;
		else //90Deg
		{
			if(Dia < 17)
				dA1 = 6 * Dia;
			else
				dA1 = 12 * Dia;
		}
	}
	else//주근일경우
	{
		if(Dia < 27)
			dA2 = 3*Dia+Dia;
		else if(Dia<37)
			dA2 = 4*Dia+Dia;
		else
			dA2 = 5*Dia+Dia;
		if(fabs(fabs(dDeg) - 180) < DTOL1)
			dA1 = max(4 * Dia, 60);
		else if(fabs(fabs(dDeg) - 135) < DTOL1)
			dA1 = 6 * Dia;
		else //90Deg
			dA1 = 12 * Dia;

	}

	if(fabs(fabs(dDeg) - 180) < DTOL1)
		dBendLen = dA2*2;
	else //90Deg
		dBendLen = dA2*1;

	dHookLength = ROUND((dA1+dBendLen)/10)*10;
	return dHookLength;
}

//120215.ES.약산식/정밀식 반영-인장정착길이
double DevSpliceLengthSet::CalcDevLength(double Dia, double Fck, double Fy, double CorrectionFactor, long Simplified, double DistFactor, double DepthBelow) const
{
	double Leng = 0.;
	Fck *= CODESTRESSUNIT;
	double Alpha = 1.0;  //배치위치계수
	if(DepthBelow > 300) Alpha = 1.3;

	double Beta = 1.0;  // beta(에폭시 도막계수)->1.5,1.2
	if(m_bEpoxyFactor) Beta =1.5;

	double LimitFactor = Alpha * Beta;
	if( LimitFactor > 1.7) LimitFactor = 1.7;

	double Ramada = 1.0; // Ramda(경량콘크리트계수)->1.3
	if(m_LightWeightConc) Ramada = 1.3;

	double Gamma = 1.0; //Gamma(철근 크기계수)
	if((Dia-19) <= 1) Gamma = 0.8;

	double StressRatio = CorrectionFactor * LimitFactor * Ramada;

	if(Simplified) //약산식
	{
		Leng = (0.6 * Dia * Fy) / SqrtFck(Fck) * StressRatio;
	}
	else //정밀식
	{
		Leng = ((0.9 * Dia * Fy) / SqrtFck(Fck)) *StressRatio * Gamma / DistFactor;
	}

	Leng = max(Leng, 300.); //KBC2009.0508.2 정착길이 최소값은 300mm이상
	return Leng;
}

////120215.ES.약산식/정밀식 반영-표준갈고리를 갖는 인장이형철근의 정착-KBC2009.0508.2.5
double DevSpliceLengthSet::CalcDevLength4Hook(double Dia, double Fck, double Fy, double CorrectionFactor) const
{
	double Leng = 0.;
	Fck *= CODESTRESSUNIT;
	double Beta = 1.0;  // beta(에폭시 도막계수)->1.2
	if(m_bEpoxyFactor) Beta =1.2;

	double Ramada = 1.0; // Ramda(경량콘크리트계수)->1.3
	if(m_LightWeightConc) Ramada = 1.3;

	Leng = (100. *Dia) /SqrtFck(Fck) * Beta * Ramada * CorrectionFactor;

	Leng = max(Leng, 150.);
	Leng = max(Leng, 8*Dia);

	return Leng;
}

double DevSpliceLengthSet::CalcDevLengthComp(double Dia, double Fck, double Fy, double CorrectiveFactor) const
{
	double Leng = 0.;		
	Fck *= CODESTRESSUNIT;
	Leng = ((0.25 * Dia * Fy) / SqrtFck(Fck)) * CorrectiveFactor;
	Leng = max(Leng, 0.043*Dia*Fy);
	Leng = max(Leng, 200.);
	return Leng;
}

//0508.6.2	//GetDevLength 와 같음
double DevSpliceLengthSet::CalcSpliceLengthA(double Dia, double Fck, double Fy,double CorrectionFactor, long Simplified, double DistFactor, double DepthBelow) const
{
	double Leng = 0.;
	Leng = CalcDevLength(Dia, Fck, Fy, CorrectionFactor, Simplified, DistFactor, DepthBelow);
	Leng = max(Leng, 300);
	return Leng;
}

double DevSpliceLengthSet::CalcSpliceLengthB(double Dia, double Fck, double Fy,double CorrectionFactor, long Simplified, double DistFactor, double DepthBelow) const
{
	double Leng = 0.;
	Leng = CalcDevLength(Dia, Fck, Fy, CorrectionFactor, Simplified, DistFactor, DepthBelow);
	Leng = max(Leng, 300);
	Leng *= 1.3;
	return Leng;
}
//120215.ES.약산식/정밀식 반영-압축이형철근의이음--KBC2009.0508.6.3
double DevSpliceLengthSet::CalcSpliceLengthComp(double Dia, double Fck, double Fy) const
{
	double Leng = 0.;
	Fck *= CODESTRESSUNIT;

	if((Fy - 400) <= DTOL1)
		Leng = 0.072*Fy*Dia;
	else
		Leng = (0.13*Fy - 24)*Dia;

	Leng = max(Leng, 300);

	if((Fck - 21) < DTOL1)	Leng *= (4./3.);

	return Leng;
}

CString DevSpliceLengthSet::GetDiaName( long index )
{
	return MSDeformedBar::FindBarDiameterNameByIndex(index);
}

long DevSpliceLengthSet::GetDevLength(long nDiaIndex, long nLoc, double dFy, double dFck)
{
	CString Key = GetDevSplKey(nDiaIndex, nLoc, dFy, dFck);
	auto it = mm_MemberLength.find(Key);
	if(it != mm_MemberLength.end())
		return it->second.m_dDevelopmentLength;
	return 10;
}

long DevSpliceLengthSet::GetSpliceLength(long nDiaIndex, long nLoc, double dFy, double dFck)
{
	CString Key = GetDevSplKey(nDiaIndex, nLoc, dFy, dFck);
	auto it = mm_MemberLength.find(Key);
	if(it != mm_MemberLength.end())
		return it->second.m_dSpliceLength;
	return 10;
}

void DevSpliceLengthSet::SetDevLength(long nDiaIndex, long nLoc, long nVal, double dFy, double dFck)
{
	CString Key = GetDevSplKey(nDiaIndex, nLoc, dFy, dFck);
	auto it = mm_MemberLength.find(Key);
	if(it != mm_MemberLength.end())
		it->second.m_dDevelopmentLength = (double)nVal;
	else
	{
		MemberLengthItem MemberItem;
		MemberItem.Initialize();
		MemberItem.m_dDevelopmentLength = (double)nVal;
		mm_MemberLength.insert(make_pair(Key,MemberItem));
	}
}

void DevSpliceLengthSet::SetSpliceLength(long nDiaIndex, long nLoc, long nVal, double dFy, double dFck)
{
	CString Key = GetDevSplKey(nDiaIndex, nLoc, dFy, dFck);
	auto it = mm_MemberLength.find(Key);
	if(it != mm_MemberLength.end())
		it->second.m_dSpliceLength =(double)nVal;
	else
	{
		MemberLengthItem MemberItem;
		MemberItem.Initialize();
		MemberItem.m_dSpliceLength = (double)nVal;
		mm_MemberLength.insert(make_pair(Key,MemberItem));
	}
}

long DevSpliceLengthSet::GetNumDia()
{
	CStringArray arNameList;
	MSDeformedBar::GetRebarNameList(arNameList);
	return arNameList.GetSize();
}

long DevSpliceLengthSet::GetCompSpliceLength(long nDiaIndex, double dFck, double dFy)
{
	CString Key = GetCommonKey(nDiaIndex, dFck, dFy);
	auto it = mm_CommonLength.find(Key);
	if(it != mm_CommonLength.end())
		return it->second.m_nCompLength;
	return 10;
}

void DevSpliceLengthSet::SetCompSpliceLength(long nDiaIndex, double dFck, double dFy, long nVal)
{
	CString Key = GetCommonKey(nDiaIndex, dFck, dFy);
	auto it = mm_CommonLength.find(Key);
	if(it != mm_CommonLength.end())
		it->second.m_nCompLength =(double)nVal;
	else
	{
		CommonLengthItem Commontem;
		Commontem.Initialize();
		Commontem.m_nCompLength = (double)nVal;
		mm_CommonLength.insert(make_pair(Key,Commontem));
	}
}

long DevSpliceLengthSet::GetLDHLength(long nDiaIndex, double dFck, double dFy)
{
	CString Key = GetCommonKey(nDiaIndex, dFck, dFy);
	auto it = mm_CommonLength.find(Key);
	if(it != mm_CommonLength.end())
		return it->second.m_nDevLength4Hook;
	return 10;
}

void DevSpliceLengthSet::SetLDHLength(long nDiaIndex, double dFck, double dFy, long nVal)
{
	CString Key = GetCommonKey(nDiaIndex, dFck, dFy);
	auto it = mm_CommonLength.find(Key);
	if(it != mm_CommonLength.end())
		it->second.m_nDevLength4Hook =(double)nVal;
	else
	{
		CommonLengthItem Commontem;
		Commontem.Initialize();
		Commontem.m_nDevLength4Hook = (double)nVal;
		mm_CommonLength.insert(make_pair(Key,Commontem));
	}
}

HookLengthItem * DevSpliceLengthSet::GetHookLengthItem( long nDiaIndex )
{
	CString Key = GetHookKey4DiaIndex(nDiaIndex);
	auto it = mm_HookLength.find(Key);
	if(it != mm_HookLength.end())
		return &it->second;
	else
	{
		double dDia = MSDeformedBar::FindDeformedBarDiameterByIndex(nDiaIndex);

		HookLengthItem pHookLenItem;
		pHookLenItem.Initialize();
		pHookLenItem.m_dMainbarHookLength90 = ROUND(CEIL(CalcHookLength(dDia, false, 90), 10));
		pHookLenItem.m_dMainbarHookLength180 = ROUND(CEIL(CalcHookLength(dDia, false, 180), 10));
		pHookLenItem.m_dStirrupHookLength90 = ROUND(CEIL(CalcHookLength(dDia, true, 90), 10));
		pHookLenItem.m_dStirrupHookLength135 = ROUND(CEIL(CalcHookLength(dDia, true, 135), 10));
		mm_HookLength.insert(make_pair(Key,pHookLenItem));

		auto it = mm_HookLength.find(Key);
		if (it != mm_HookLength.end())
			return &it->second;
	}
	return NULL;
}

void DevSpliceLengthSet::Copy(shared_ptr<DevSpliceLengthSet> desDevSpliceLengthSet)
{
	m_sName = desDevSpliceLengthSet->m_sName;
	m_nMemberType = desDevSpliceLengthSet->m_nMemberType;
	m_sReinforceBarSetting = desDevSpliceLengthSet->m_sReinforceBarSetting;
	m_nDevSplType = desDevSpliceLengthSet->m_nDevSplType;
	m_nCeilDigit = desDevSpliceLengthSet->m_nCeilDigit;
	m_bEpoxyFactor = desDevSpliceLengthSet->m_bEpoxyFactor;
	m_LightWeightConc = desDevSpliceLengthSet->m_LightWeightConc;
	m_SimpleCalculation = desDevSpliceLengthSet->m_SimpleCalculation;

	mm_MemberLength.clear();
	for (auto it = desDevSpliceLengthSet->mm_MemberLength.begin(); it != desDevSpliceLengthSet->mm_MemberLength.end(); it++)
	{
		CString key = it->first;

		MemberLengthItem item;
		item.Copy(it->second);

		mm_MemberLength.insert(make_pair(key, item));
	}

	mm_CommonLength.clear();
	for (auto it = desDevSpliceLengthSet->mm_CommonLength.begin(); it != desDevSpliceLengthSet->mm_CommonLength.end(); it++)
	{
		CString key = it->first;

		CommonLengthItem item;
		item.Copy(it->second);

		mm_CommonLength.insert(make_pair(key, item));
	}

	mm_HookLength.clear();
	for (auto it = desDevSpliceLengthSet->mm_HookLength.begin(); it != desDevSpliceLengthSet->mm_HookLength.end(); it++)
	{
		CString key = it->first;

		HookLengthItem item;
		item.Copy(it->second);

		mm_HookLength.insert(make_pair(key, item));
	}
}