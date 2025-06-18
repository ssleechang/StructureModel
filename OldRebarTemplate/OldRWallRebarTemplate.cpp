#include "StdAfx.h"
#include "OldRWallRebarTemplate.h"
#include "..\MSDeformedBar.h"
#include "..\..\GMLib\MSVersionInfo.h"
#include "..\RWallVerBarRebarTemplate.h"
#include "..\RWallDowelRebarTemplate.h"


IMPLEMENT_SERIAL(OldRWallRebarTemplate, OldBaseRebarTemplate, VERSIONABLE_SCHEMA | 1)
OldRWallRebarTemplate::OldRWallRebarTemplate() :OldBaseRebarTemplate()
{
	DataInit();
}

OldRWallRebarTemplate::~OldRWallRebarTemplate(void)
{
}

void OldRWallRebarTemplate::CopyFromMe(OldRWallRebarTemplate* pRWallRebarTemplate)
{

}

void OldRWallRebarTemplate::Dump(EFS::EFS_Buffer& buffer)
{
	buffer << m_bRWallHorInfoSet;
	buffer << m_bRWallHorBarStartSet;
	buffer << m_nRWallHorBarStartLength;

	buffer << m_nRWallEndUbar;
	buffer << m_bRWallDefaultUbarLen;

	buffer << m_bRWallVerInfoSet;
	buffer << m_nRWallVerBarTopLenType;

	buffer << m_bRWallDowelInfoSet;
	buffer << m_bRWallUserDowelHookLen;
	buffer << m_nRWallDowelBarAnchorType;
	buffer << m_nRWallDowelBarTopLenType;
	buffer << m_nRWallDowelSupportBarDiaIndex;
	buffer << m_nRWallDowelSupportBarNum;
	buffer << m_bRWallDowelSupportPlaceType;

	map<long, long>::iterator it;

	int size = (int)mm_RWallDefaultUbarLen.size();
	buffer << size;
	for (it = mm_RWallDefaultUbarLen.begin(); it != mm_RWallDefaultUbarLen.end(); it++)
	{
		long nDiaIndex = it->first;
		long uBarLen = it->second;
		buffer << nDiaIndex;
		buffer << uBarLen;
	}

	size = (int)mm_RWallUserDowelHookLen.size();
	buffer << size;
	for (it = mm_RWallUserDowelHookLen.begin(); it != mm_RWallUserDowelHookLen.end(); it++)
	{
		long nDiaIndex = it->first;
		long hookLen = it->second;
		buffer << nDiaIndex;
		buffer << hookLen;
	}

	size = (int)mm_RWallUserDowelSpliceLen.size();
	buffer << size;
	for (it = mm_RWallUserDowelSpliceLen.begin(); it != mm_RWallUserDowelSpliceLen.end(); it++)
	{
		long nDiaIndex = it->first;
		long spliceLen = it->second;
		buffer << nDiaIndex;
		buffer << spliceLen;
	}

	buffer << m_nRWallVerBarSpliceWay;
	buffer << m_bRWallVerBarAlterSpliceSet;
	buffer << m_nRWallVerBarAlterSpliceValue1;
	buffer << m_nRWallVerBarAlterSpliceValue2;
	buffer << m_nRWallDowelBarSpliceWay;
	buffer << m_bRWallDowelBarAlterSpliceSet;
	buffer << m_nRWallDowelBarAlterSpliceValue1;
	buffer << m_nRWallDowelBarAlterSpliceValue2;
	buffer << m_bRWallDowelHookAnchorPlaceSpacingSet;
	buffer << m_nRWallDowelHookAnchorPlaceSpacing;
	buffer << m_bRWallDowelBotAddBarIntegratedPlaceSet;

	buffer << m_bRWallSpacerInfoSet;
	buffer << m_nRWallSpacerDiaType;
	buffer << m_nRWallSpacerDiaIndex;
	buffer << m_nRWallSpacerHorSpacing;
	buffer << m_nRWallSpacerVerSpacingType;
	buffer << m_nRWallSpacerVerSpacing;
	buffer << m_nRWallSpacerVerSpacingRate;
	buffer << m_nRWallSpacerVerSpacingCount;
	buffer << m_nRWallDowelSpacerVerSpacingCount;
	buffer << m_nRWallSpacerLenType;
	buffer << m_nRWallSpacerLenCalWay;
	buffer << m_nRWallSpacerLen1;
	buffer << m_nRWallSpacerLen2;
	buffer << m_nRWallSpacerShape;
	buffer << m_bRWallSpacerRebarPlaceSet;
	buffer << m_nRWallSpacerRebarPlaceCondition;
}

void OldRWallRebarTemplate::Recovery(EFS::EFS_Buffer& buffer)
{
	buffer >> m_bRWallHorInfoSet;
	buffer >> m_bRWallHorBarStartSet;
	buffer >> m_nRWallHorBarStartLength;

	buffer >> m_nRWallEndUbar;
	buffer >> m_bRWallDefaultUbarLen;

	buffer >> m_bRWallVerInfoSet;
	buffer >> m_nRWallVerBarTopLenType;

	buffer >> m_bRWallDowelInfoSet;
	buffer >> m_bRWallUserDowelHookLen;
	buffer >> m_nRWallDowelBarAnchorType;
	buffer >> m_nRWallDowelBarTopLenType;
	buffer >> m_nRWallDowelSupportBarDiaIndex;
	buffer >> m_nRWallDowelSupportBarNum;
	buffer >> m_bRWallDowelSupportPlaceType;

	int NumObject;

	mm_RWallDefaultUbarLen.clear();
	buffer >> NumObject;
	for (int index = 0; index < NumObject; index++)
	{
		long nDiaIndex = 0, uBarLen = 0;

		buffer >> nDiaIndex;
		buffer >> uBarLen;
		mm_RWallDefaultUbarLen.insert(make_pair(nDiaIndex, uBarLen));
	}

	mm_RWallUserDowelHookLen.clear();
	buffer >> NumObject;
	for (int index = 0; index < NumObject; index++)
	{
		long nDiaIndex = 0, hookLen = 0;

		buffer >> nDiaIndex;
		buffer >> hookLen;
		mm_RWallUserDowelHookLen.insert(make_pair(nDiaIndex, hookLen));
	}

	mm_RWallUserDowelSpliceLen.clear();
	buffer >> NumObject;
	for (int index = 0; index < NumObject; index++)
	{
		long nDiaIndex = 0, spliceLen = 0;

		buffer >> nDiaIndex;
		buffer >> spliceLen;
		mm_RWallUserDowelSpliceLen.insert(make_pair(nDiaIndex, spliceLen));
	}

	buffer >> m_nRWallVerBarSpliceWay;
	buffer >> m_bRWallVerBarAlterSpliceSet;
	buffer >> m_nRWallVerBarAlterSpliceValue1;
	buffer >> m_nRWallVerBarAlterSpliceValue2;
	buffer >> m_nRWallDowelBarSpliceWay;
	buffer >> m_bRWallDowelBarAlterSpliceSet;
	buffer >> m_nRWallDowelBarAlterSpliceValue1;
	buffer >> m_nRWallDowelBarAlterSpliceValue2;
	buffer >> m_bRWallDowelHookAnchorPlaceSpacingSet;
	buffer >> m_nRWallDowelHookAnchorPlaceSpacing;
	buffer >> m_bRWallDowelBotAddBarIntegratedPlaceSet;

	buffer >> m_bRWallSpacerInfoSet;
	buffer >> m_nRWallSpacerDiaType;
	buffer >> m_nRWallSpacerDiaIndex;
	buffer >> m_nRWallSpacerHorSpacing;
	buffer >> m_nRWallSpacerVerSpacingType;
	buffer >> m_nRWallSpacerVerSpacing;
	buffer >> m_nRWallSpacerVerSpacingRate;
	buffer >> m_nRWallSpacerVerSpacingCount;
	buffer >> m_nRWallDowelSpacerVerSpacingCount;
	buffer >> m_nRWallSpacerLenType;
	buffer >> m_nRWallSpacerLenCalWay;
	buffer >> m_nRWallSpacerLen1;
	buffer >> m_nRWallSpacerLen2;
	buffer >> m_nRWallSpacerShape;
	buffer >> m_bRWallSpacerRebarPlaceSet;
	buffer >> m_nRWallSpacerRebarPlaceCondition;
}

void OldRWallRebarTemplate::Serialize(CArchive& ar)
{
	long nDefaultValue = 0;

	if(ar.IsStoring())
	{
		ar << m_bRWallHorInfoSet;
		ar << m_bRWallHorBarStartSet;
		ar << m_nRWallHorBarStartLength;

		ar << m_bRWallSpacerInfoSet;
		ar << nDefaultValue;
		ar << nDefaultValue;
		ar << nDefaultValue;
		ar << nDefaultValue;
		ar << m_nRWallEndUbar;
		ar << m_bRWallDefaultUbarLen;

		ar << m_bRWallVerInfoSet;
		ar << m_nRWallVerBarTopLenType;

		ar << m_bRWallDowelInfoSet;
		ar << m_bRWallUserDowelHookLen;
		ar << m_nRWallDowelBarAnchorType;
		ar << m_nRWallDowelBarTopLenType;
		ar << m_nRWallDowelSupportBarDiaIndex;
		ar << m_nRWallDowelSupportBarNum;
		ar << m_bRWallDowelSupportPlaceType;

		map<long, long>::iterator it;

		ar << (int)mm_RWallDefaultUbarLen.size();
		for (it = mm_RWallDefaultUbarLen.begin(); it != mm_RWallDefaultUbarLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long uBarLen = it->second;
			ar << nDiaIndex;
			ar << uBarLen;
		}

		ar << (int)mm_RWallUserDowelHookLen.size();
		for (it = mm_RWallUserDowelHookLen.begin(); it != mm_RWallUserDowelHookLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}

		ar << (int)mm_RWallUserDowelSpliceLen.size();
		for (it = mm_RWallUserDowelSpliceLen.begin(); it != mm_RWallUserDowelSpliceLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long spliceLen = it->second;
			ar << nDiaIndex;
			ar << spliceLen;
		}

		ar << m_nRWallVerBarSpliceWay;
		ar << m_bRWallVerBarAlterSpliceSet;
		ar << m_nRWallVerBarAlterSpliceValue1;
		ar << m_nRWallVerBarAlterSpliceValue2;
		ar << m_nRWallDowelBarSpliceWay;
		ar << m_bRWallDowelBarAlterSpliceSet;
		ar << m_nRWallDowelBarAlterSpliceValue1;
		ar << m_nRWallDowelBarAlterSpliceValue2;
		ar << m_bRWallDowelHookAnchorPlaceSpacingSet;
		ar << m_nRWallDowelHookAnchorPlaceSpacing;
		ar << m_bRWallDowelBotAddBarIntegratedPlaceSet;

		ar << m_nRWallSpacerDiaType;
		ar << m_nRWallSpacerDiaIndex;
		ar << m_nRWallSpacerHorSpacing;
		ar << m_nRWallSpacerVerSpacingType;
		ar << m_nRWallSpacerVerSpacing;
		ar << m_nRWallSpacerVerSpacingRate;
		ar << m_nRWallSpacerVerSpacingCount;
		ar << m_nRWallDowelSpacerVerSpacingCount;
		ar << m_nRWallSpacerLenType;
		ar << m_nRWallSpacerLenCalWay;
		ar << m_nRWallSpacerLen1;
		ar << m_nRWallSpacerLen2;
		ar << m_nRWallSpacerShape;
		ar << m_bRWallSpacerRebarPlaceSet;
		ar << m_nRWallSpacerRebarPlaceCondition;
	}
	else
	{
		long nRWallSpacerDiaIndex = 0, nRWallSpacerSpacing = 0, nRWallSpacerLen1 = 0, nRWallSpacerLen2 = 0;

		ar >> m_bRWallHorInfoSet;
		ar >> m_bRWallHorBarStartSet;
		ar >> m_nRWallHorBarStartLength;

		ar >> m_bRWallSpacerInfoSet;
		ar >> nRWallSpacerDiaIndex;
		ar >> nRWallSpacerSpacing;
		ar >> nRWallSpacerLen1;
		ar >> nRWallSpacerLen2;
		ar >> m_nRWallEndUbar;
		ar >> m_bRWallDefaultUbarLen;

		ar >> m_bRWallVerInfoSet;
		ar >> m_nRWallVerBarTopLenType;

		ar >> m_bRWallDowelInfoSet;
		ar >> m_bRWallUserDowelHookLen;
		ar >> m_nRWallDowelBarAnchorType;
		ar >> m_nRWallDowelBarTopLenType;
		ar >> m_nRWallDowelSupportBarDiaIndex;
		ar >> m_nRWallDowelSupportBarNum;
		ar >> m_bRWallDowelSupportPlaceType;

		int NumObject;

		mm_RWallDefaultUbarLen.clear();
		ar >> NumObject;
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, uBarLen = 0;

			ar >> nDiaIndex;
			ar >> uBarLen;
			mm_RWallDefaultUbarLen.insert(make_pair(nDiaIndex, uBarLen));
		}

		mm_RWallUserDowelHookLen.clear();
		ar >> NumObject;
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_RWallUserDowelHookLen.insert(make_pair(nDiaIndex, hookLen));
		}

		mm_RWallUserDowelSpliceLen.clear();
		ar >> NumObject;
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, spliceLen = 0;

			ar >> nDiaIndex;
			ar >> spliceLen;
			mm_RWallUserDowelSpliceLen.insert(make_pair(nDiaIndex, spliceLen));
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20160218)
		{
			ar >> m_nRWallVerBarSpliceWay;
			ar >> m_bRWallVerBarAlterSpliceSet;
			ar >> m_nRWallVerBarAlterSpliceValue1;
			ar >> m_nRWallVerBarAlterSpliceValue2;
			ar >> m_nRWallDowelBarSpliceWay;
			ar >> m_bRWallDowelBarAlterSpliceSet;
			ar >> m_nRWallDowelBarAlterSpliceValue1;
			ar >> m_nRWallDowelBarAlterSpliceValue2;
			ar >> m_bRWallDowelHookAnchorPlaceSpacingSet;
			ar >> m_nRWallDowelHookAnchorPlaceSpacing;
		}
		else
		{
			if (m_nRWallVerBarTopLenType == 0)
				m_nRWallVerBarTopLenType = 1;
			else if (m_nRWallVerBarTopLenType == 1)
				m_nRWallVerBarTopLenType = 0;

			if (m_nRWallDowelBarTopLenType == 0)
				m_nRWallDowelBarTopLenType = 1;
			else if (m_nRWallDowelBarTopLenType == 1)
				m_nRWallDowelBarTopLenType = 0;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20161104)
			ar >> m_bRWallDowelBotAddBarIntegratedPlaceSet;

		if (MSVersionInfo::GetCurrentVersion() >= 20170405)
		{
			ar >> m_nRWallSpacerDiaType;
			ar >> m_nRWallSpacerDiaIndex;
			ar >> m_nRWallSpacerHorSpacing;
			ar >> m_nRWallSpacerVerSpacingType;
			ar >> m_nRWallSpacerVerSpacing;
			ar >> m_nRWallSpacerVerSpacingRate;
			ar >> m_nRWallSpacerVerSpacingCount;
			ar >> m_nRWallDowelSpacerVerSpacingCount;
			ar >> m_nRWallSpacerLenType;
			ar >> m_nRWallSpacerLenCalWay;
			ar >> m_nRWallSpacerLen1;
			ar >> m_nRWallSpacerLen2;
			ar >> m_nRWallSpacerShape;
			ar >> m_bRWallSpacerRebarPlaceSet;
			ar >> m_nRWallSpacerRebarPlaceCondition;
		}
		else
		{
			m_nRWallSpacerDiaType = 1;
			m_nRWallSpacerDiaIndex = nRWallSpacerDiaIndex;
			m_nRWallSpacerHorSpacing = nRWallSpacerSpacing;
			m_nRWallSpacerVerSpacingType = 0;
			m_nRWallSpacerVerSpacing = nRWallSpacerSpacing;
			m_nRWallSpacerVerSpacingRate = 3;
			m_nRWallSpacerVerSpacingCount = 3;
			m_nRWallDowelSpacerVerSpacingCount = 0;
			m_nRWallSpacerLenType = 0;
			m_nRWallSpacerLenCalWay = 1;
			m_nRWallSpacerLen1 = nRWallSpacerLen1;
			m_nRWallSpacerLen2 = nRWallSpacerLen2;
			m_nRWallSpacerShape = 0;
			m_bRWallSpacerRebarPlaceSet = FALSE;
			m_nRWallSpacerRebarPlaceCondition = 2000;
		}

		m_bRWallDowelSupportPlaceType = false;
	}
}

double OldRWallRebarTemplate::GetRWallUserDowelHookLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_RWallUserDowelHookLen.find(DiaIndex);
	if (it == mm_RWallUserDowelHookLen.end())
		return 0;
	return it->second;
}
double OldRWallRebarTemplate::GetRWallUserDowelSpliceLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_RWallUserDowelSpliceLen.find(DiaIndex);
	if (it == mm_RWallUserDowelSpliceLen.end())
		return 0;
	return it->second;
}

void OldRWallRebarTemplate::SetRWallDefaultUbarLenMap(map<long, long> mapDefaultUbarDia)
{
	mm_RWallDefaultUbarLen.clear();

	map<long, long>::iterator it;
	for (it = mapDefaultUbarDia.begin(); it != mapDefaultUbarDia.end(); it++)
	{
		mm_RWallDefaultUbarLen.insert(make_pair(it->first, it->second));
	}
}

void OldRWallRebarTemplate::SetRWallUserDowelHookLenMap(map<long, long> mapUserDowelHookLen)
{
	mm_RWallUserDowelHookLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserDowelHookLen.begin(); it != mapUserDowelHookLen.end(); it++)
	{
		mm_RWallUserDowelHookLen.insert(make_pair(it->first, it->second));
	}
}

void OldRWallRebarTemplate::SetRWallUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen)
{
	mm_RWallUserDowelSpliceLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserDowelSpliceLen.begin(); it != mapUserDowelSpliceLen.end(); it++)
	{
		mm_RWallUserDowelSpliceLen.insert(make_pair(it->first, it->second));
	}
}

void OldRWallRebarTemplate::DataInit()
{
	m_bRWallHorInfoSet = false;
	m_bRWallHorBarStartSet = false;
	m_nRWallHorBarStartLength = 50;
	m_nRWallEndUbar = 0;
	m_bRWallDefaultUbarLen = false;

	m_bRWallSpacerInfoSet = false;
	m_nRWallSpacerDiaType = 0;
	m_nRWallSpacerDiaIndex = 0;
	m_nRWallSpacerHorSpacing = 1500;
	m_nRWallSpacerVerSpacingType = 0;
	m_nRWallSpacerVerSpacing = 1500;
	m_nRWallSpacerVerSpacingRate = 3;
	m_nRWallSpacerVerSpacingCount = 3;
	m_nRWallDowelSpacerVerSpacingCount = 0;
	m_nRWallSpacerLenType = 0;
	m_nRWallSpacerLenCalWay = 0;
	m_nRWallSpacerLen1 = 350;
	m_nRWallSpacerLen2 = 350;
	m_nRWallSpacerShape = 0;
	m_bRWallSpacerRebarPlaceSet = true;
	m_nRWallSpacerRebarPlaceCondition = 2000;

	m_bRWallVerInfoSet = false;
	m_nRWallVerBarTopLenType = 0;
	m_nRWallVerBarSpliceWay = 0;
	m_bRWallVerBarAlterSpliceSet = false;
	m_nRWallVerBarAlterSpliceValue1 = 300;
	m_nRWallVerBarAlterSpliceValue2 = 700;

	m_bRWallDowelInfoSet = false;
	m_bRWallUserDowelHookLen = false;
	m_nRWallDowelBarAnchorType = 1;
	m_nRWallDowelBarTopLenType = 0;
	m_nRWallDowelSupportBarDiaIndex = 0; //10;
	m_nRWallDowelSupportBarNum = 3;
	m_bRWallDowelSupportPlaceType = false;
	m_nRWallDowelBarSpliceWay = 0;
	m_bRWallDowelBarAlterSpliceSet = false;
	m_nRWallDowelBarAlterSpliceValue1 = 300;
	m_nRWallDowelBarAlterSpliceValue2 = 700;
	m_bRWallDowelHookAnchorPlaceSpacingSet = false;
	m_nRWallDowelHookAnchorPlaceSpacing = 500;
	m_bRWallDowelBotAddBarIntegratedPlaceSet = false;

	InitDefaultUbarLenMap(mm_RWallDefaultUbarLen);
	InitUserDowelHookLenMap(mm_RWallUserDowelHookLen);
	InitUserDowelSpliceLenMap(mm_RWallUserDowelSpliceLen);
}

bool OldRWallRebarTemplate::TopOptionDataCopy(OldRWallRebarTemplate* rebarTemplate)
{
	bool bDataCopy = false;

	if (!m_bRWallHorInfoSet)
	{
		m_bRWallHorBarStartSet = rebarTemplate->m_bRWallHorBarStartSet;
		m_nRWallHorBarStartLength = rebarTemplate->m_nRWallHorBarStartLength;
		m_nRWallEndUbar = rebarTemplate->m_nRWallEndUbar;
		m_bRWallDefaultUbarLen = rebarTemplate->m_bRWallDefaultUbarLen;
		DefaultUbarLenMapDataCopy(mm_RWallDefaultUbarLen, rebarTemplate->mm_RWallDefaultUbarLen);
		bDataCopy = true;
	}

	if (!m_bRWallSpacerInfoSet)
	{
		m_nRWallSpacerDiaType = rebarTemplate->m_nRWallSpacerDiaType;
		m_nRWallSpacerDiaIndex = rebarTemplate->m_nRWallSpacerDiaIndex;
		m_nRWallSpacerHorSpacing = rebarTemplate->m_nRWallSpacerHorSpacing;
		m_nRWallSpacerVerSpacingType = rebarTemplate->m_nRWallSpacerVerSpacingType;
		m_nRWallSpacerVerSpacing = rebarTemplate->m_nRWallSpacerVerSpacing;
		m_nRWallSpacerVerSpacingRate = rebarTemplate->m_nRWallSpacerVerSpacingRate;
		m_nRWallSpacerVerSpacingCount = rebarTemplate->m_nRWallSpacerVerSpacingCount;
		m_nRWallDowelSpacerVerSpacingCount = rebarTemplate->m_nRWallDowelSpacerVerSpacingCount;
		m_nRWallSpacerLenType = rebarTemplate->m_nRWallSpacerLenType;
		m_nRWallSpacerLenCalWay = rebarTemplate->m_nRWallSpacerLenCalWay;
		m_nRWallSpacerLen1 = rebarTemplate->m_nRWallSpacerLen1;
		m_nRWallSpacerLen2 = rebarTemplate->m_nRWallSpacerLen2;
		m_nRWallSpacerShape = rebarTemplate->m_nRWallSpacerShape;
		m_bRWallSpacerRebarPlaceSet = rebarTemplate->m_bRWallSpacerRebarPlaceSet;
		m_nRWallSpacerRebarPlaceCondition = rebarTemplate->m_nRWallSpacerRebarPlaceCondition;
		bDataCopy = true;
	}

	if (!m_bRWallVerInfoSet)
	{
		m_nRWallVerBarTopLenType = rebarTemplate->m_nRWallVerBarTopLenType;
		m_nRWallVerBarSpliceWay = rebarTemplate->m_nRWallVerBarSpliceWay;
		m_bRWallVerBarAlterSpliceSet = rebarTemplate->m_bRWallVerBarAlterSpliceSet;
		m_nRWallVerBarAlterSpliceValue1 = rebarTemplate->m_nRWallVerBarAlterSpliceValue1;
		m_nRWallVerBarAlterSpliceValue2 = rebarTemplate->m_nRWallVerBarAlterSpliceValue2;
		bDataCopy = true;
	}

	if (!m_bRWallDowelInfoSet)
	{
		m_bRWallUserDowelHookLen = rebarTemplate->m_bRWallUserDowelHookLen;
		m_nRWallDowelBarAnchorType = rebarTemplate->m_nRWallDowelBarAnchorType;
		m_nRWallDowelBarTopLenType = rebarTemplate->m_nRWallDowelBarTopLenType;
		m_nRWallDowelSupportBarDiaIndex = rebarTemplate->m_nRWallDowelSupportBarDiaIndex;
		m_nRWallDowelSupportBarNum = rebarTemplate->m_nRWallDowelSupportBarNum;
		m_bRWallDowelSupportPlaceType = rebarTemplate->m_bRWallDowelSupportPlaceType;
		m_nRWallDowelBarSpliceWay = rebarTemplate->m_nRWallDowelBarSpliceWay;
		m_bRWallDowelBarAlterSpliceSet = rebarTemplate->m_bRWallDowelBarAlterSpliceSet;
		m_nRWallDowelBarAlterSpliceValue1 = rebarTemplate->m_nRWallDowelBarAlterSpliceValue1;
		m_nRWallDowelBarAlterSpliceValue2 = rebarTemplate->m_nRWallDowelBarAlterSpliceValue2;
		m_bRWallDowelHookAnchorPlaceSpacingSet = rebarTemplate->m_bRWallDowelHookAnchorPlaceSpacingSet;
		m_nRWallDowelHookAnchorPlaceSpacing = rebarTemplate->m_nRWallDowelHookAnchorPlaceSpacing;
		m_bRWallDowelBotAddBarIntegratedPlaceSet = rebarTemplate->m_bRWallDowelBotAddBarIntegratedPlaceSet;
		UserDowelHookLenMapDataCopy(mm_RWallUserDowelHookLen, rebarTemplate->mm_RWallUserDowelHookLen);
		UserDowelSpliceLenMapDataCopy(mm_RWallUserDowelSpliceLen, rebarTemplate->mm_RWallUserDowelSpliceLen);
		bDataCopy = true;
	}

	return bDataCopy;
}

void OldRWallRebarTemplate::DataCopy(OldRWallRebarTemplate* rebarTemplate)
{
	m_bRWallHorBarStartSet = rebarTemplate->m_bRWallHorBarStartSet;
	m_nRWallHorBarStartLength = rebarTemplate->m_nRWallHorBarStartLength;
	m_nRWallEndUbar = rebarTemplate->m_nRWallEndUbar;
	m_bRWallDefaultUbarLen = rebarTemplate->m_bRWallDefaultUbarLen;
	DefaultUbarLenMapDataCopy(mm_RWallDefaultUbarLen, rebarTemplate->mm_RWallDefaultUbarLen);

	m_nRWallSpacerDiaType = rebarTemplate->m_nRWallSpacerDiaType;
	m_nRWallSpacerDiaIndex = rebarTemplate->m_nRWallSpacerDiaIndex;
	m_nRWallSpacerHorSpacing = rebarTemplate->m_nRWallSpacerHorSpacing;
	m_nRWallSpacerVerSpacingType = rebarTemplate->m_nRWallSpacerVerSpacingType;
	m_nRWallSpacerVerSpacing = rebarTemplate->m_nRWallSpacerVerSpacing;
	m_nRWallSpacerVerSpacingRate = rebarTemplate->m_nRWallSpacerVerSpacingRate;
	m_nRWallSpacerVerSpacingCount = rebarTemplate->m_nRWallSpacerVerSpacingCount;
	m_nRWallDowelSpacerVerSpacingCount = rebarTemplate->m_nRWallDowelSpacerVerSpacingCount;
	m_nRWallSpacerLenType = rebarTemplate->m_nRWallSpacerLenType;
	m_nRWallSpacerLenCalWay = rebarTemplate->m_nRWallSpacerLenCalWay;
	m_nRWallSpacerLen1 = rebarTemplate->m_nRWallSpacerLen1;
	m_nRWallSpacerLen2 = rebarTemplate->m_nRWallSpacerLen2;
	m_nRWallSpacerShape = rebarTemplate->m_nRWallSpacerShape;
	m_bRWallSpacerRebarPlaceSet = rebarTemplate->m_bRWallSpacerRebarPlaceSet;
	m_nRWallSpacerRebarPlaceCondition = rebarTemplate->m_nRWallSpacerRebarPlaceCondition;

	m_nRWallVerBarTopLenType = rebarTemplate->m_nRWallVerBarTopLenType;
	m_nRWallVerBarSpliceWay = rebarTemplate->m_nRWallVerBarSpliceWay;
	m_bRWallVerBarAlterSpliceSet = rebarTemplate->m_bRWallVerBarAlterSpliceSet;
	m_nRWallVerBarAlterSpliceValue1 = rebarTemplate->m_nRWallVerBarAlterSpliceValue1;
	m_nRWallVerBarAlterSpliceValue2 = rebarTemplate->m_nRWallVerBarAlterSpliceValue2;

	m_bRWallUserDowelHookLen = rebarTemplate->m_bRWallUserDowelHookLen;
	m_nRWallDowelBarAnchorType = rebarTemplate->m_nRWallDowelBarAnchorType;
	m_nRWallDowelBarTopLenType = rebarTemplate->m_nRWallDowelBarTopLenType;
	m_nRWallDowelSupportBarDiaIndex = rebarTemplate->m_nRWallDowelSupportBarDiaIndex;
	m_nRWallDowelSupportBarNum = rebarTemplate->m_nRWallDowelSupportBarNum;
	m_bRWallDowelSupportPlaceType = rebarTemplate->m_bRWallDowelSupportPlaceType;
	m_nRWallDowelBarSpliceWay = rebarTemplate->m_nRWallDowelBarSpliceWay;
	m_bRWallDowelBarAlterSpliceSet = rebarTemplate->m_bRWallDowelBarAlterSpliceSet;
	m_nRWallDowelBarAlterSpliceValue1 = rebarTemplate->m_nRWallDowelBarAlterSpliceValue1;
	m_nRWallDowelBarAlterSpliceValue2 = rebarTemplate->m_nRWallDowelBarAlterSpliceValue2;
	m_bRWallDowelHookAnchorPlaceSpacingSet = rebarTemplate->m_bRWallDowelHookAnchorPlaceSpacingSet;
	m_nRWallDowelHookAnchorPlaceSpacing = rebarTemplate->m_nRWallDowelHookAnchorPlaceSpacing;
	m_bRWallDowelBotAddBarIntegratedPlaceSet = rebarTemplate->m_bRWallDowelBotAddBarIntegratedPlaceSet;
	UserDowelHookLenMapDataCopy(mm_RWallUserDowelHookLen, rebarTemplate->mm_RWallUserDowelHookLen);
	UserDowelSpliceLenMapDataCopy(mm_RWallUserDowelSpliceLen, rebarTemplate->mm_RWallUserDowelSpliceLen);
}

void OldRWallRebarTemplate::OldDataCopy(RWallRebarTemplate* rebarTemplate)
{
	RWallVerBarRebarTemplate *pVerBarTemplateBelowDia = rebarTemplate->GetRWallVerBarTemplateBelowDia();
	RWallVerBarRebarTemplate *pVerBarTemplateExcessDia = rebarTemplate->GetRWallVerBarTemplateExcessDia();
	pVerBarTemplateBelowDia->SetTopLenType(m_nRWallVerBarTopLenType);
	pVerBarTemplateBelowDia->SetBarSpliceType(m_nRWallVerBarSpliceWay);
	pVerBarTemplateBelowDia->SetAlterSpliceSet(m_bRWallVerBarAlterSpliceSet);
	pVerBarTemplateBelowDia->SetConstructionHeight(min(m_nRWallVerBarAlterSpliceValue1, m_nRWallVerBarAlterSpliceValue2));
	pVerBarTemplateBelowDia->SetAlterSpliceDist(abs(m_nRWallVerBarAlterSpliceValue1 - m_nRWallVerBarAlterSpliceValue2));

	pVerBarTemplateExcessDia->CopyFromMe(pVerBarTemplateBelowDia);

	RWallDowelRebarTemplate *pDowelTemplateBelowDia = rebarTemplate->GetRWallDowelTemplateBelowDia();
	RWallDowelRebarTemplate *pDowelTemplateExcessDia = rebarTemplate->GetRWallDowelTemplateExcessDia();
	pDowelTemplateBelowDia->SetUserHookLenSet(m_bRWallUserDowelHookLen);
	pDowelTemplateBelowDia->SetBarAnchorType(m_nRWallDowelBarAnchorType);
	pDowelTemplateBelowDia->SetTopLenType(m_nRWallDowelBarTopLenType);
	pDowelTemplateBelowDia->SetSupportBarDiaIndex(m_nRWallDowelSupportBarDiaIndex);
	pDowelTemplateBelowDia->SetSupportBarNum(m_nRWallDowelSupportBarNum);	
	pDowelTemplateBelowDia->SetBarSpliceType(m_nRWallDowelBarSpliceWay);
	pDowelTemplateBelowDia->SetAlterSpliceSet(m_bRWallDowelBarAlterSpliceSet);
	pDowelTemplateBelowDia->SetConstructionHeight(min(m_nRWallDowelBarAlterSpliceValue1, m_nRWallDowelBarAlterSpliceValue2));
	pDowelTemplateBelowDia->SetAlterSpliceDist(abs(m_nRWallDowelBarAlterSpliceValue1 - m_nRWallDowelBarAlterSpliceValue2));
	pDowelTemplateBelowDia->SetHookAnchorPlaceSpacingSet(m_bRWallDowelHookAnchorPlaceSpacingSet);
	pDowelTemplateBelowDia->SetHookAnchorPlaceSpacing(m_nRWallDowelHookAnchorPlaceSpacing);
	pDowelTemplateBelowDia->SetBotAddBarIntegratedPlaceSet(m_bRWallDowelBotAddBarIntegratedPlaceSet);
	
	pDowelTemplateExcessDia->CopyFromMe(pDowelTemplateBelowDia);
	
	rebarTemplate->SetRWallHorBarStartSet(m_bRWallHorBarStartSet);
	rebarTemplate->SetRWallHorBarStartLength(m_nRWallHorBarStartLength);
	rebarTemplate->SetRWallEndUbar(m_nRWallEndUbar);
	rebarTemplate->SetRWallDefaultUbarLen(m_bRWallDefaultUbarLen);
	rebarTemplate->SetRWallDefaultUbarLenMap(mm_RWallDefaultUbarLen);

	rebarTemplate->SetRWallSpacerDiaType(m_nRWallSpacerDiaType);
	rebarTemplate->SetRWallSpacerDiaIndex(m_nRWallSpacerDiaIndex);
	rebarTemplate->SetRWallSpacerHorSpacing(m_nRWallSpacerHorSpacing);
	rebarTemplate->SetRWallSpacerVerSpacingType(m_nRWallSpacerVerSpacingType);
	rebarTemplate->SetRWallSpacerVerSpacing(m_nRWallSpacerVerSpacing);
	rebarTemplate->SetRWallSpacerVerSpacingRate(m_nRWallSpacerVerSpacingRate);
	rebarTemplate->SetRWallSpacerVerSpacingCount(m_nRWallSpacerVerSpacingCount);
	rebarTemplate->SetRWallDowelSpacerVerSpacingCount(m_nRWallDowelSpacerVerSpacingCount);
	rebarTemplate->SetRWallSpacerLenType(m_nRWallSpacerLenType);
	rebarTemplate->SetRWallSpacerLenCalWayA(m_nRWallSpacerLenCalWay);
	rebarTemplate->SetRWallSpacerLenCalWayB(m_nRWallSpacerLenCalWay);
	rebarTemplate->SetRWallSpacerLenA(m_nRWallSpacerLen1);
	rebarTemplate->SetRWallSpacerLenB(m_nRWallSpacerLen2);
	rebarTemplate->SetRWallSpacerShape(m_nRWallSpacerShape);
	rebarTemplate->SetRWallSpacerRebarPlaceSet(m_bRWallSpacerRebarPlaceSet);
	rebarTemplate->SetRWallSpacerRebarPlaceCondition(m_nRWallSpacerRebarPlaceCondition);
	rebarTemplate->SetRWallDowelSupportPlaceType(m_bRWallDowelSupportPlaceType);
			
	rebarTemplate->SetRWallUserDowelHookLenMap(mm_RWallUserDowelHookLen);
	rebarTemplate->SetRWallUserDowelSpliceLenMap(mm_RWallUserDowelSpliceLen);
}

void OldRWallRebarTemplate::InfoSetCheckBoxChecked()
{
	m_bRWallHorInfoSet = true;
	m_bRWallSpacerInfoSet = true;
	m_bRWallVerInfoSet = true;
	m_bRWallDowelInfoSet = true;
}
