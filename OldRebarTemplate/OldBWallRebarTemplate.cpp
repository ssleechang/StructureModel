#include "StdAfx.h"
#include "OldBWallRebarTemplate.h"
#include "..\MSDeformedBar.h"
#include "..\..\GMLib\MSVersionInfo.h"
#include "..\BWallVerBarRebarTemplate.h"
#include "..\BWallDowelRebarTemplate.h"

IMPLEMENT_SERIAL(OldBWallRebarTemplate, OldBaseRebarTemplate, VERSIONABLE_SCHEMA | 1)
OldBWallRebarTemplate::OldBWallRebarTemplate() :OldBaseRebarTemplate()
{
	DataInit();
}


OldBWallRebarTemplate::~OldBWallRebarTemplate(void)
{
}

void OldBWallRebarTemplate::CopyFromMe(OldBWallRebarTemplate* pBWallRebarTemplate)
{

}

void OldBWallRebarTemplate::Dump(EFS::EFS_Buffer& buffer)
{
	buffer << m_bBWallVerInfoSet;
	buffer << m_bBWallPlaceEndBarSet;
	buffer << m_nBWallEndVerAddBarDist;
	buffer << m_nBWallVerBarTopLenType;
	buffer << m_nBWallCrossBarEAType;

	buffer << m_bBWallHorInfoSet;
	buffer << m_bBWallPlaceUBar4ShortWall;
	buffer << m_nBWallMaxUbarLegLength;
	buffer << m_bBWallPlaceStirrup4ShortWall;
	buffer << m_nBWallHBarMaxStirrupLen;
	buffer << m_bBWallHorBarStartSet;
	buffer << m_nBWallHorBarStartLength;

	buffer << m_bBWallDefaultUbarLen;

	buffer << m_bBWallDowelInfoSet;
	buffer << m_bBWallUserDowelHookLen;
	buffer << m_nBWallDowelBarAnchorType;
	buffer << m_nBWallDowelBarTopLenType;
	buffer << m_nBWallDowelSupportBarDiaIndex;
	buffer << m_nBWallDowelSupportBarNum;
	buffer << m_bBWallDowelSupportPlaceType;

	map<long, long>::iterator it;

	int size = (int)mm_BWallAddBarDiaIndex.size();
	buffer << size;
	for(it = mm_BWallAddBarDiaIndex.begin(); it != mm_BWallAddBarDiaIndex.end(); it++)
	{
		long index1 = it->first;
		long index2 = it->second;
		buffer << index1;
		buffer << index2;
	}

	size = (int)mm_BWallDefaultUbarLen.size();
	buffer << size;
	for(it = mm_BWallDefaultUbarLen.begin(); it != mm_BWallDefaultUbarLen.end(); it++)
	{
		long index = it->first;
		long uBarLen = it->second;
		buffer << index;
		buffer << uBarLen;
	}

	size = (int)mm_BWallUserDowelHookLen.size();
	buffer << size;
	for(it = mm_BWallUserDowelHookLen.begin(); it != mm_BWallUserDowelHookLen.end(); it++)
	{
		long nDiaIndex = it->first;
		long hookLen = it->second;
		buffer << nDiaIndex;
		buffer << hookLen;
	}

	size = (int)mm_BWallUserDowelSpliceLen.size();
	buffer << size;
	for(it = mm_BWallUserDowelSpliceLen.begin(); it != mm_BWallUserDowelSpliceLen.end(); it++)
	{
		long nDiaIndex = it->first;
		long spliceLen = it->second;
		buffer << nDiaIndex;
		buffer << spliceLen;
	}

	buffer << m_nBWallVerBarSpliceWay;
	buffer << m_bBWallVerBarAlterSpliceSet;
	buffer << m_nBWallVerBarAlterSpliceValue1;
	buffer << m_nBWallVerBarAlterSpliceValue2;
	buffer << m_bBWallStraightEndUBarPlaceSet;
	buffer << m_nBWallDowelBarSpliceWay;
	buffer << m_bBWallDowelBarAlterSpliceSet;
	buffer << m_nBWallDowelBarAlterSpliceValue1;
	buffer << m_nBWallDowelBarAlterSpliceValue2;
	buffer << m_bBWallDowelHookAnchorPlaceSpacingSet;
	buffer << m_nBWallDowelHookAnchorPlaceSpacing;

	buffer << m_nBWallEndVerAddBarCount;

	size = (int)mm_BWallEndBarDiaIndex.size();
	buffer << size;
	for (it = mm_BWallEndBarDiaIndex.begin(); it != mm_BWallEndBarDiaIndex.end(); it++)
	{
		long index1 = it->first;
		long index2 = it->second;
		buffer << index1;
		buffer << index2;
	}

	buffer << m_bBWallCrossBarUBarHalfPlace;

	buffer << m_nBWallVerNumBarCrossBarIncludeSet;

	buffer << m_bBWallSpacerInfoSet;
	buffer << m_nBWallSpacerDiaType;
	buffer << m_nBWallSpacerDiaIndex;
	buffer << m_nBWallSpacerHorSpacing;
	buffer << m_nBWallSpacerVerSpacingType;
	buffer << m_nBWallSpacerVerSpacing;
	buffer << m_nBWallSpacerVerSpacingRate;
	buffer << m_nBWallSpacerVerSpacingCount;
	buffer << m_nBWallDowelSpacerVerSpacingCount;
	buffer << m_nBWallSpacerLenType;
	buffer << m_nBWallSpacerLenCalWay;
	buffer << m_nBWallSpacerLen1;
	buffer << m_nBWallSpacerLen2;
	buffer << m_nBWallSpacerShape;
	buffer << m_bBWallSpacerRebarPlaceSet;
	buffer << m_nBWallSpacerRebarPlaceCondition;
}

void OldBWallRebarTemplate::Recovery(EFS::EFS_Buffer& buffer)
{
	buffer >> m_bBWallVerInfoSet;
	buffer >> m_bBWallPlaceEndBarSet;
	buffer >> m_nBWallEndVerAddBarDist;
	buffer >> m_nBWallVerBarTopLenType;
	buffer >> m_nBWallCrossBarEAType;

	buffer >> m_bBWallHorInfoSet;
	buffer >> m_bBWallPlaceUBar4ShortWall;
	buffer >> m_nBWallMaxUbarLegLength;
	buffer >> m_bBWallPlaceStirrup4ShortWall;
	buffer >> m_nBWallHBarMaxStirrupLen;
	buffer >> m_bBWallHorBarStartSet;
	buffer >> m_nBWallHorBarStartLength;

	buffer >> m_bBWallDefaultUbarLen;

	buffer >> m_bBWallDowelInfoSet;
	buffer >> m_bBWallUserDowelHookLen;
	buffer >> m_nBWallDowelBarAnchorType;
	buffer >> m_nBWallDowelBarTopLenType;
	buffer >> m_nBWallDowelSupportBarDiaIndex;
	buffer >> m_nBWallDowelSupportBarNum;
	buffer >> m_bBWallDowelSupportPlaceType;

	int NumObject;

	mm_BWallAddBarDiaIndex.clear();
	buffer >> NumObject;
	for(int index = 0; index < NumObject; index++)
	{
		long nDiaIndex1 = 0, nDiaIndex2 = 0;

		buffer >> nDiaIndex1;
		buffer >> nDiaIndex2;
		mm_BWallAddBarDiaIndex.insert(make_pair(nDiaIndex1,nDiaIndex2));
	}

	mm_BWallDefaultUbarLen.clear();
	buffer >> NumObject;
	for(int index = 0; index < NumObject; index++)
	{
		long nDiaIndex = 0, uBarLen = 0;

		buffer >> nDiaIndex;
		buffer >> uBarLen;
		mm_BWallDefaultUbarLen.insert(make_pair(nDiaIndex,uBarLen));
	}

	mm_BWallUserDowelHookLen.clear();
	buffer >> NumObject;
	for(int index = 0; index < NumObject; index++)
	{
		long nDiaIndex = 0, hookLen = 0;

		buffer >> nDiaIndex;
		buffer >> hookLen;
		mm_BWallUserDowelHookLen.insert(make_pair(nDiaIndex,hookLen));
	}

	mm_BWallUserDowelSpliceLen.clear();
	buffer >> NumObject;
	for(int index = 0; index < NumObject; index++)
	{
		long nDiaIndex = 0, spliceLen = 0;

		buffer >> nDiaIndex;
		buffer >> spliceLen;
		mm_BWallUserDowelSpliceLen.insert(make_pair(nDiaIndex,spliceLen));
	}

	buffer >> m_nBWallVerBarSpliceWay;
	buffer >> m_bBWallVerBarAlterSpliceSet;
	buffer >> m_nBWallVerBarAlterSpliceValue1;
	buffer >> m_nBWallVerBarAlterSpliceValue2;
	buffer >> m_bBWallStraightEndUBarPlaceSet;
	buffer >> m_nBWallDowelBarSpliceWay;
	buffer >> m_bBWallDowelBarAlterSpliceSet;
	buffer >> m_nBWallDowelBarAlterSpliceValue1;
	buffer >> m_nBWallDowelBarAlterSpliceValue2;
	buffer >> m_bBWallDowelHookAnchorPlaceSpacingSet;
	buffer >> m_nBWallDowelHookAnchorPlaceSpacing;

	buffer >> m_nBWallEndVerAddBarCount;

	mm_BWallEndBarDiaIndex.clear();
	buffer >> NumObject;
	for (int index = 0; index < NumObject; index++)
	{
		long nDiaIndex1 = 0, nDiaIndex2 = 0;

		buffer >> nDiaIndex1;
		buffer >> nDiaIndex2;
		mm_BWallEndBarDiaIndex.insert(make_pair(nDiaIndex1, nDiaIndex2));
	}

	buffer >> m_bBWallCrossBarUBarHalfPlace;

	buffer >> m_nBWallVerNumBarCrossBarIncludeSet;

	buffer >> m_bBWallSpacerInfoSet;
	buffer >> m_nBWallSpacerDiaType;
	buffer >> m_nBWallSpacerDiaIndex;
	buffer >> m_nBWallSpacerHorSpacing;
	buffer >> m_nBWallSpacerVerSpacingType;
	buffer >> m_nBWallSpacerVerSpacing;
	buffer >> m_nBWallSpacerVerSpacingRate;
	buffer >> m_nBWallSpacerVerSpacingCount;
	buffer >> m_nBWallDowelSpacerVerSpacingCount;
	buffer >> m_nBWallSpacerLenType;
	buffer >> m_nBWallSpacerLenCalWay;
	buffer >> m_nBWallSpacerLen1;
	buffer >> m_nBWallSpacerLen2;
	buffer >> m_nBWallSpacerShape;
	buffer >> m_bBWallSpacerRebarPlaceSet;
	buffer >> m_nBWallSpacerRebarPlaceCondition;
}

void OldBWallRebarTemplate::Serialize(CArchive& ar)
{
	long nDefaultValue = 0;

	if(ar.IsStoring())
	{
		ar << m_bBWallVerInfoSet;
		ar << m_bBWallPlaceEndBarSet;
		ar << m_nBWallEndVerAddBarDist;
		ar << m_nBWallVerBarTopLenType;
		ar << m_nBWallCrossBarEAType;

		ar << m_bBWallHorInfoSet;
		ar << m_bBWallPlaceUBar4ShortWall;
		ar << m_nBWallMaxUbarLegLength;
		ar << m_bBWallPlaceStirrup4ShortWall;
		ar << m_nBWallHBarMaxStirrupLen;
		ar << m_bBWallHorBarStartSet;
		ar << m_nBWallHorBarStartLength;

		ar << m_bBWallSpacerInfoSet;
		ar << nDefaultValue;
		ar << nDefaultValue;
		ar << nDefaultValue;
		ar << nDefaultValue;
		ar << nDefaultValue;
		ar << nDefaultValue;
		ar << nDefaultValue;
		ar << nDefaultValue;
		ar << m_bBWallDefaultUbarLen;

		ar << m_bBWallDowelInfoSet;
		ar << m_bBWallUserDowelHookLen;
		ar << m_nBWallDowelBarAnchorType;
		ar << m_nBWallDowelBarTopLenType;
		ar << m_nBWallDowelSupportBarDiaIndex;
		ar << m_nBWallDowelSupportBarNum;
		ar << m_bBWallDowelSupportPlaceType;

		map<long, long>::iterator it;

		ar << (int)mm_BWallAddBarDiaIndex.size();
		for(it = mm_BWallAddBarDiaIndex.begin(); it != mm_BWallAddBarDiaIndex.end(); it++)
		{
			long index1 = it->first;
			long index2 = it->second;
			ar << index1;
			ar << index2;
		}

		ar << (int)mm_BWallDefaultUbarLen.size();
		for(it = mm_BWallDefaultUbarLen.begin(); it != mm_BWallDefaultUbarLen.end(); it++)
		{
			long index = it->first;
			long uBarLen = it->second;
			ar << index;
			ar << uBarLen;
		}

		ar << (int)mm_BWallUserDowelHookLen.size();
		for(it = mm_BWallUserDowelHookLen.begin(); it != mm_BWallUserDowelHookLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}

		ar << (int)mm_BWallUserDowelSpliceLen.size();
		for(it = mm_BWallUserDowelSpliceLen.begin(); it != mm_BWallUserDowelSpliceLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long spliceLen = it->second;
			ar << nDiaIndex;
			ar << spliceLen;
		}

		ar << m_nBWallVerBarSpliceWay;
		ar << m_bBWallVerBarAlterSpliceSet;
		ar << m_nBWallVerBarAlterSpliceValue1;
		ar << m_nBWallVerBarAlterSpliceValue2;
		ar << m_bBWallStraightEndUBarPlaceSet;
		ar << m_nBWallDowelBarSpliceWay;
		ar << m_bBWallDowelBarAlterSpliceSet;
		ar << m_nBWallDowelBarAlterSpliceValue1;
		ar << m_nBWallDowelBarAlterSpliceValue2;
		ar << m_bBWallDowelHookAnchorPlaceSpacingSet;
		ar << m_nBWallDowelHookAnchorPlaceSpacing;

		ar << m_nBWallEndVerAddBarCount;

		ar << (int)mm_BWallEndBarDiaIndex.size();
		for (it = mm_BWallEndBarDiaIndex.begin(); it != mm_BWallEndBarDiaIndex.end(); it++)
		{
			long index1 = it->first;
			long index2 = it->second;
			ar << index1;
			ar << index2;
		}

		ar << m_bBWallCrossBarUBarHalfPlace;

		ar << m_nBWallVerNumBarCrossBarIncludeSet;

		ar << m_nBWallSpacerDiaType;
		ar << m_nBWallSpacerDiaIndex;
		ar << m_nBWallSpacerHorSpacing;
		ar << m_nBWallSpacerVerSpacingType;
		ar << m_nBWallSpacerVerSpacing;
		ar << m_nBWallSpacerVerSpacingRate;
		ar << m_nBWallSpacerVerSpacingCount;
		ar << m_nBWallDowelSpacerVerSpacingCount;
		ar << m_nBWallSpacerLenType;
		ar << m_nBWallSpacerLenCalWay;
		ar << m_nBWallSpacerLen1;
		ar << m_nBWallSpacerLen2;
		ar << m_nBWallSpacerShape;
		ar << m_bBWallSpacerRebarPlaceSet;
		ar << m_nBWallSpacerRebarPlaceCondition;
	}
	else
	{
		long nBWallGroundSpacerDiaIndex = 0, nBWallGroundSpacerSpacing = 0, nBWallGroundSpacerLen1 = 0, nBWallGroundSpacerLen2 = 0;

		ar >> m_bBWallVerInfoSet;
		ar >> m_bBWallPlaceEndBarSet;
		ar >> m_nBWallEndVerAddBarDist;
		ar >> m_nBWallVerBarTopLenType;
		ar >> m_nBWallCrossBarEAType;

		ar >> m_bBWallHorInfoSet;
		ar >> m_bBWallPlaceUBar4ShortWall;
		ar >> m_nBWallMaxUbarLegLength;
		ar >> m_bBWallPlaceStirrup4ShortWall;
		ar >> m_nBWallHBarMaxStirrupLen;
		ar >> m_bBWallHorBarStartSet;
		ar >> m_nBWallHorBarStartLength;

		ar >> m_bBWallSpacerInfoSet;
		ar >> nBWallGroundSpacerDiaIndex;
		ar >> nBWallGroundSpacerSpacing;
		ar >> nBWallGroundSpacerLen1;
		ar >> nBWallGroundSpacerLen2;
		ar >> nDefaultValue;
		ar >> nDefaultValue;
		ar >> nDefaultValue;
		ar >> nDefaultValue;
		ar >> m_bBWallDefaultUbarLen;

		ar >> m_bBWallDowelInfoSet;
		ar >> m_bBWallUserDowelHookLen;
		ar >> m_nBWallDowelBarAnchorType;
		ar >> m_nBWallDowelBarTopLenType;
		ar >> m_nBWallDowelSupportBarDiaIndex;
		ar >> m_nBWallDowelSupportBarNum;
		ar >> m_bBWallDowelSupportPlaceType;

		int NumObject;

		mm_BWallAddBarDiaIndex.clear();
		ar >> NumObject;
		for(int index = 0; index < NumObject; index++)
		{
			long nDiaIndex1 = 0, nDiaIndex2 = 0;

			ar >> nDiaIndex1;
			ar >> nDiaIndex2;
			mm_BWallAddBarDiaIndex.insert(make_pair(nDiaIndex1,nDiaIndex2));
		}

		mm_BWallDefaultUbarLen.clear();
		ar >> NumObject;
		for(int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, uBarLen = 0;

			ar >> nDiaIndex;
			ar >> uBarLen;
			mm_BWallDefaultUbarLen.insert(make_pair(nDiaIndex,uBarLen));
		}

		mm_BWallUserDowelHookLen.clear();
		ar >> NumObject;
		for(int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_BWallUserDowelHookLen.insert(make_pair(nDiaIndex,hookLen));
		}

		mm_BWallUserDowelSpliceLen.clear();
		ar >> NumObject;
		for(int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, spliceLen = 0;

			ar >> nDiaIndex;
			ar >> spliceLen;
			mm_BWallUserDowelSpliceLen.insert(make_pair(nDiaIndex,spliceLen));
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20160218)
		{
			ar >> m_nBWallVerBarSpliceWay;
			ar >> m_bBWallVerBarAlterSpliceSet;
			ar >> m_nBWallVerBarAlterSpliceValue1;
			ar >> m_nBWallVerBarAlterSpliceValue2;
			ar >> m_bBWallStraightEndUBarPlaceSet;
			ar >> m_nBWallDowelBarSpliceWay;
			ar >> m_bBWallDowelBarAlterSpliceSet;
			ar >> m_nBWallDowelBarAlterSpliceValue1;
			ar >> m_nBWallDowelBarAlterSpliceValue2;
			ar >> m_bBWallDowelHookAnchorPlaceSpacingSet;
			ar >> m_nBWallDowelHookAnchorPlaceSpacing;
		}
		else
		{
			if (m_nBWallVerBarTopLenType == 0)
				m_nBWallVerBarTopLenType = 1;
			else //if (m_nBWallVerBarTopLenType == 1 || m_nBWallVerBarTopLenType == 2)
				m_nBWallVerBarTopLenType = 0;

			if (m_nBWallCrossBarEAType == 0)
				m_nBWallCrossBarEAType = 1;
			else //if (m_nBWallCrossBarEAType == 1)
				m_nBWallCrossBarEAType = 0;

			if (m_nBWallDowelBarTopLenType == 0)
				m_nBWallDowelBarTopLenType = 1;
			else if (m_nBWallDowelBarTopLenType == 1)
				m_nBWallDowelBarTopLenType = 0;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20160803)
		{
			ar >> m_nBWallEndVerAddBarCount;

			mm_BWallEndBarDiaIndex.clear();
			ar >> NumObject;
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex1 = 0, nDiaIndex2 = 0;

				ar >> nDiaIndex1;
				ar >> nDiaIndex2;
				mm_BWallEndBarDiaIndex.insert(make_pair(nDiaIndex1, nDiaIndex2));
			}
		}
		else
		{
			mm_BWallEndBarDiaIndex.clear();

			map<long, long>::iterator it;
			for (it = mm_BWallAddBarDiaIndex.begin(); it != mm_BWallAddBarDiaIndex.end(); it++)
			{
				mm_BWallEndBarDiaIndex.insert(make_pair(it->first, it->second));
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20160818)
		{
			ar >> m_bBWallCrossBarUBarHalfPlace;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20160926)
		{
			ar >> m_nBWallVerNumBarCrossBarIncludeSet;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20170405)
		{
			ar >> m_nBWallSpacerDiaType;
			ar >> m_nBWallSpacerDiaIndex;
			ar >> m_nBWallSpacerHorSpacing;
			ar >> m_nBWallSpacerVerSpacingType;
			ar >> m_nBWallSpacerVerSpacing;
			ar >> m_nBWallSpacerVerSpacingRate;
			ar >> m_nBWallSpacerVerSpacingCount;
			ar >> m_nBWallDowelSpacerVerSpacingCount;
			ar >> m_nBWallSpacerLenType;
			ar >> m_nBWallSpacerLenCalWay;
			ar >> m_nBWallSpacerLen1;
			ar >> m_nBWallSpacerLen2;
			ar >> m_nBWallSpacerShape;
			ar >> m_bBWallSpacerRebarPlaceSet;
			ar >> m_nBWallSpacerRebarPlaceCondition;
		}
		else
		{
			m_nBWallSpacerDiaType = 1;
			m_nBWallSpacerDiaIndex = nBWallGroundSpacerDiaIndex;
			m_nBWallSpacerHorSpacing = nBWallGroundSpacerSpacing;
			m_nBWallSpacerVerSpacingType = 0;
			m_nBWallSpacerVerSpacing = nBWallGroundSpacerSpacing;
			m_nBWallSpacerVerSpacingRate = 3;
			m_nBWallSpacerVerSpacingCount = 3;
			m_nBWallDowelSpacerVerSpacingCount = 0;
			m_nBWallSpacerLenType = 0;
			m_nBWallSpacerLenCalWay = 1;
			m_nBWallSpacerLen1 = nBWallGroundSpacerLen1;
			m_nBWallSpacerLen2 = nBWallGroundSpacerLen2;
			m_nBWallSpacerShape = 0;
			m_bBWallSpacerRebarPlaceSet = FALSE;
			m_nBWallSpacerRebarPlaceCondition = 2000;
		}

		m_bBWallDowelSupportPlaceType = false;
	}
}

double OldBWallRebarTemplate::GetBWallAddBarDia(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);

	map<long, long>::iterator it = mm_BWallAddBarDiaIndex.find(DiaIndex);
	if (it == mm_BWallAddBarDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}

double OldBWallRebarTemplate::GetBWallEndBarDia(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);

	map<long, long>::iterator it = mm_BWallEndBarDiaIndex.find(DiaIndex);
	if (it == mm_BWallEndBarDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}

double OldBWallRebarTemplate::GetBWallDefaultUbarLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_BWallDefaultUbarLen.find(DiaIndex);
	if (it == mm_BWallDefaultUbarLen.end())
		return 0;
	return it->second;
}

double OldBWallRebarTemplate::GetBWallUserDowelHookLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_BWallUserDowelHookLen.find(DiaIndex);
	if (it == mm_BWallUserDowelHookLen.end())
		return 0;
	return it->second;
}

double OldBWallRebarTemplate::GetBWallUserDowelSpliceLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_BWallUserDowelSpliceLen.find(DiaIndex);
	if (it == mm_BWallUserDowelSpliceLen.end())
		return 0;
	return it->second;
}

void OldBWallRebarTemplate::SetBWallAddBarDiaIndexMap(map<long, long> mapAddBarDiaIndex)
{
	mm_BWallAddBarDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapAddBarDiaIndex.begin(); it != mapAddBarDiaIndex.end(); it++)
	{
		mm_BWallAddBarDiaIndex.insert(make_pair(it->first, it->second));
	}
}

void OldBWallRebarTemplate::SetBWallEndBarDiaIndexMap(map<long, long> mapEndBarDiaIndex)
{
	mm_BWallEndBarDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapEndBarDiaIndex.begin(); it != mapEndBarDiaIndex.end(); it++)
	{
		mm_BWallEndBarDiaIndex.insert(make_pair(it->first, it->second));
	}

}
void OldBWallRebarTemplate::SetBWallDefaultUbarLenMap(map<long, long> mapDefaultUbarLen)
{
	mm_BWallDefaultUbarLen.clear();

	map<long, long>::iterator it;
	for (it = mapDefaultUbarLen.begin(); it != mapDefaultUbarLen.end(); it++)
	{
		mm_BWallDefaultUbarLen.insert(make_pair(it->first, it->second));
	}
}

void OldBWallRebarTemplate::SetBWallUserDowelHookLenMap(map<long, long> mapUserDowelHookLen)
{
	mm_BWallUserDowelHookLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserDowelHookLen.begin(); it != mapUserDowelHookLen.end(); it++)
	{
		mm_BWallUserDowelHookLen.insert(make_pair(it->first, it->second));
	}
}

void OldBWallRebarTemplate::SetBWallUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen)
{
	mm_BWallUserDowelSpliceLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserDowelSpliceLen.begin(); it != mapUserDowelSpliceLen.end(); it++)
	{
		mm_BWallUserDowelSpliceLen.insert(make_pair(it->first, it->second));
	}
}

void OldBWallRebarTemplate::DataInit()
{
	m_bBWallVerInfoSet = false;
	m_bBWallPlaceEndBarSet = false;
	m_nBWallEndVerAddBarDist = 100;
	m_nBWallVerBarTopLenType = 0;
	m_nBWallCrossBarEAType = 0;
	m_nBWallVerBarSpliceWay = 0;
	m_bBWallVerBarAlterSpliceSet = false;
	m_nBWallVerBarAlterSpliceValue1 = 300;
	m_nBWallVerBarAlterSpliceValue2 = 700;
	m_nBWallVerNumBarCrossBarIncludeSet = 0;

	m_bBWallHorInfoSet = false;
	m_bBWallPlaceUBar4ShortWall = true;
	m_nBWallMaxUbarLegLength = 2000;
	m_bBWallPlaceStirrup4ShortWall = false;
	m_nBWallHBarMaxStirrupLen = 1000;
	m_bBWallHorBarStartSet = false;
	m_nBWallHorBarStartLength = 50;
	m_bBWallStraightEndUBarPlaceSet = true;
	m_bBWallDefaultUbarLen = false;
	m_bBWallCrossBarUBarHalfPlace = false;

	m_bBWallSpacerInfoSet = false;
	m_nBWallSpacerDiaType = 0;
	m_nBWallSpacerDiaIndex = 0;
	m_nBWallSpacerHorSpacing = 1500;
	m_nBWallSpacerVerSpacingType = 0;
	m_nBWallSpacerVerSpacing = 1500;
	m_nBWallSpacerVerSpacingRate = 3;
	m_nBWallSpacerVerSpacingCount = 3;
	m_nBWallDowelSpacerVerSpacingCount = 0;
	m_nBWallSpacerLenType = 0;
	m_nBWallSpacerLenCalWay = 0;
	m_nBWallSpacerLen1 = 350;
	m_nBWallSpacerLen2 = 350;
	m_nBWallSpacerShape = 0;
	m_bBWallSpacerRebarPlaceSet = true;
	m_nBWallSpacerRebarPlaceCondition = 2000;

	m_bBWallDowelInfoSet = false;
	m_bBWallUserDowelHookLen = false;
	m_nBWallDowelBarAnchorType = 1;
	m_nBWallDowelBarTopLenType = 0;
	m_nBWallDowelSupportBarDiaIndex = 0; //10;
	m_nBWallDowelSupportBarNum = 3;
	m_bBWallDowelSupportPlaceType = false;
	m_nBWallDowelBarSpliceWay = 0;
	m_bBWallDowelBarAlterSpliceSet = false;
	m_nBWallDowelBarAlterSpliceValue1 = 300;
	m_nBWallDowelBarAlterSpliceValue2 = 700;
	m_bBWallDowelHookAnchorPlaceSpacingSet = false;
	m_nBWallDowelHookAnchorPlaceSpacing = 500;

	InitAddBarDiaIndexMap(mm_BWallAddBarDiaIndex);
	InitDefaultUbarLenMap(mm_BWallDefaultUbarLen);
	InitUserDowelHookLenMap(mm_BWallUserDowelHookLen);
	InitUserDowelSpliceLenMap(mm_BWallUserDowelSpliceLen);

	m_nBWallEndVerAddBarCount = 2;
	InitEndBarDiaIndexMap(mm_BWallEndBarDiaIndex);
}

bool OldBWallRebarTemplate::TopOptionDataCopy(OldBWallRebarTemplate* rebarTemplate)
{
	bool bDataCopy = false;

	if (!m_bBWallVerInfoSet)
	{
		m_bBWallPlaceEndBarSet = rebarTemplate->m_bBWallPlaceEndBarSet;
		m_nBWallEndVerAddBarDist = rebarTemplate->m_nBWallEndVerAddBarDist;
		m_nBWallVerBarTopLenType = rebarTemplate->m_nBWallVerBarTopLenType;
		m_nBWallCrossBarEAType = rebarTemplate->m_nBWallCrossBarEAType;
		m_nBWallVerBarSpliceWay = rebarTemplate->m_nBWallVerBarSpliceWay;
		m_bBWallVerBarAlterSpliceSet = rebarTemplate->m_bBWallVerBarAlterSpliceSet;
		m_nBWallVerBarAlterSpliceValue1 = rebarTemplate->m_nBWallVerBarAlterSpliceValue1;
		m_nBWallVerBarAlterSpliceValue2 = rebarTemplate->m_nBWallVerBarAlterSpliceValue2;
		m_nBWallVerNumBarCrossBarIncludeSet = rebarTemplate->m_nBWallVerNumBarCrossBarIncludeSet;
		AddBarDiaIndexMapDataCopy(mm_BWallAddBarDiaIndex, rebarTemplate->mm_BWallAddBarDiaIndex);
		m_nBWallEndVerAddBarCount = rebarTemplate->m_nBWallEndVerAddBarCount;
		EndBarDiaIndexMapDataCopy(mm_BWallEndBarDiaIndex, rebarTemplate->mm_BWallEndBarDiaIndex);
		bDataCopy = true;
	}

	if (!m_bBWallHorInfoSet)
	{
		m_bBWallPlaceUBar4ShortWall = rebarTemplate->m_bBWallPlaceUBar4ShortWall;
		m_nBWallMaxUbarLegLength = rebarTemplate->m_nBWallMaxUbarLegLength;
		m_bBWallPlaceStirrup4ShortWall = rebarTemplate->m_bBWallPlaceStirrup4ShortWall;
		m_nBWallHBarMaxStirrupLen = rebarTemplate->m_nBWallHBarMaxStirrupLen;
		m_bBWallHorBarStartSet = rebarTemplate->m_bBWallHorBarStartSet;
		m_nBWallHorBarStartLength = rebarTemplate->m_nBWallHorBarStartLength;
		m_bBWallStraightEndUBarPlaceSet = rebarTemplate->m_bBWallStraightEndUBarPlaceSet;
		m_bBWallDefaultUbarLen = rebarTemplate->m_bBWallDefaultUbarLen;
		m_bBWallCrossBarUBarHalfPlace = rebarTemplate->m_bBWallCrossBarUBarHalfPlace;
		DefaultUbarLenMapDataCopy(mm_BWallDefaultUbarLen, rebarTemplate->mm_BWallDefaultUbarLen);
		bDataCopy = true;
	}

	if (!m_bBWallSpacerInfoSet)
	{
		m_nBWallSpacerDiaType = rebarTemplate->m_nBWallSpacerDiaType;
		m_nBWallSpacerDiaIndex = rebarTemplate->m_nBWallSpacerDiaIndex;
		m_nBWallSpacerHorSpacing = rebarTemplate->m_nBWallSpacerHorSpacing;
		m_nBWallSpacerVerSpacingType = rebarTemplate->m_nBWallSpacerVerSpacingType;
		m_nBWallSpacerVerSpacing = rebarTemplate->m_nBWallSpacerVerSpacing;
		m_nBWallSpacerVerSpacingRate = rebarTemplate->m_nBWallSpacerVerSpacingRate;
		m_nBWallSpacerVerSpacingCount = rebarTemplate->m_nBWallSpacerVerSpacingCount;
		m_nBWallDowelSpacerVerSpacingCount = rebarTemplate->m_nBWallDowelSpacerVerSpacingCount;
		m_nBWallSpacerLenType = rebarTemplate->m_nBWallSpacerLenType;
		m_nBWallSpacerLenCalWay = rebarTemplate->m_nBWallSpacerLenCalWay;
		m_nBWallSpacerLen1 = rebarTemplate->m_nBWallSpacerLen1;
		m_nBWallSpacerLen2 = rebarTemplate->m_nBWallSpacerLen2;
		m_nBWallSpacerShape = rebarTemplate->m_nBWallSpacerShape;
		m_bBWallSpacerRebarPlaceSet = rebarTemplate->m_bBWallSpacerRebarPlaceSet;
		m_nBWallSpacerRebarPlaceCondition = rebarTemplate->m_nBWallSpacerRebarPlaceCondition;
		bDataCopy = true;
	}

	if (!m_bBWallDowelInfoSet)
	{
		m_bBWallUserDowelHookLen = rebarTemplate->m_bBWallUserDowelHookLen;
		m_nBWallDowelBarAnchorType = rebarTemplate->m_nBWallDowelBarAnchorType;
		m_nBWallDowelBarTopLenType = rebarTemplate->m_nBWallDowelBarTopLenType;
		m_nBWallDowelSupportBarDiaIndex = rebarTemplate->m_nBWallDowelSupportBarDiaIndex;
		m_nBWallDowelSupportBarNum = rebarTemplate->m_nBWallDowelSupportBarNum;
		m_bBWallDowelSupportPlaceType = rebarTemplate->m_bBWallDowelSupportPlaceType;
		m_nBWallDowelBarSpliceWay = rebarTemplate->m_nBWallDowelBarSpliceWay;
		m_bBWallDowelBarAlterSpliceSet = rebarTemplate->m_bBWallDowelBarAlterSpliceSet;
		m_nBWallDowelBarAlterSpliceValue1 = rebarTemplate->m_nBWallDowelBarAlterSpliceValue1;
		m_nBWallDowelBarAlterSpliceValue2 = rebarTemplate->m_nBWallDowelBarAlterSpliceValue2;
		m_bBWallDowelHookAnchorPlaceSpacingSet = rebarTemplate->m_bBWallDowelHookAnchorPlaceSpacingSet;
		m_nBWallDowelHookAnchorPlaceSpacing = rebarTemplate->m_nBWallDowelHookAnchorPlaceSpacing;
		UserDowelHookLenMapDataCopy(mm_BWallUserDowelHookLen, rebarTemplate->mm_BWallUserDowelHookLen);
		UserDowelSpliceLenMapDataCopy(mm_BWallUserDowelSpliceLen, rebarTemplate->mm_BWallUserDowelSpliceLen);
		bDataCopy = true;
	}

	return bDataCopy;
}

void OldBWallRebarTemplate::DataCopy(OldBWallRebarTemplate* rebarTemplate)
{
	m_bBWallPlaceEndBarSet = rebarTemplate->m_bBWallPlaceEndBarSet;
	m_nBWallEndVerAddBarDist = rebarTemplate->m_nBWallEndVerAddBarDist;
	m_nBWallVerBarTopLenType = rebarTemplate->m_nBWallVerBarTopLenType;
	m_nBWallCrossBarEAType = rebarTemplate->m_nBWallCrossBarEAType;
	m_nBWallVerBarSpliceWay = rebarTemplate->m_nBWallVerBarSpliceWay;
	m_bBWallVerBarAlterSpliceSet = rebarTemplate->m_bBWallVerBarAlterSpliceSet;
	m_nBWallVerBarAlterSpliceValue1 = rebarTemplate->m_nBWallVerBarAlterSpliceValue1;
	m_nBWallVerBarAlterSpliceValue2 = rebarTemplate->m_nBWallVerBarAlterSpliceValue2;
	m_nBWallVerNumBarCrossBarIncludeSet = rebarTemplate->m_nBWallVerNumBarCrossBarIncludeSet;
	AddBarDiaIndexMapDataCopy(mm_BWallAddBarDiaIndex, rebarTemplate->mm_BWallAddBarDiaIndex);
	m_nBWallEndVerAddBarCount = rebarTemplate->m_nBWallEndVerAddBarCount;
	EndBarDiaIndexMapDataCopy(mm_BWallEndBarDiaIndex, rebarTemplate->mm_BWallEndBarDiaIndex);

	m_bBWallPlaceUBar4ShortWall = rebarTemplate->m_bBWallPlaceUBar4ShortWall;
	m_nBWallMaxUbarLegLength = rebarTemplate->m_nBWallMaxUbarLegLength;
	m_bBWallPlaceStirrup4ShortWall = rebarTemplate->m_bBWallPlaceStirrup4ShortWall;
	m_nBWallHBarMaxStirrupLen = rebarTemplate->m_nBWallHBarMaxStirrupLen;
	m_bBWallHorBarStartSet = rebarTemplate->m_bBWallHorBarStartSet;
	m_nBWallHorBarStartLength = rebarTemplate->m_nBWallHorBarStartLength;
	m_bBWallStraightEndUBarPlaceSet = rebarTemplate->m_bBWallStraightEndUBarPlaceSet;
	m_bBWallDefaultUbarLen = rebarTemplate->m_bBWallDefaultUbarLen;
	m_bBWallCrossBarUBarHalfPlace = rebarTemplate->m_bBWallCrossBarUBarHalfPlace;
	DefaultUbarLenMapDataCopy(mm_BWallDefaultUbarLen, rebarTemplate->mm_BWallDefaultUbarLen);

	m_nBWallSpacerDiaType = rebarTemplate->m_nBWallSpacerDiaType;
	m_nBWallSpacerDiaIndex = rebarTemplate->m_nBWallSpacerDiaIndex;
	m_nBWallSpacerHorSpacing = rebarTemplate->m_nBWallSpacerHorSpacing;
	m_nBWallSpacerVerSpacingType = rebarTemplate->m_nBWallSpacerVerSpacingType;
	m_nBWallSpacerVerSpacing = rebarTemplate->m_nBWallSpacerVerSpacing;
	m_nBWallSpacerVerSpacingRate = rebarTemplate->m_nBWallSpacerVerSpacingRate;
	m_nBWallSpacerVerSpacingCount = rebarTemplate->m_nBWallSpacerVerSpacingCount;
	m_nBWallDowelSpacerVerSpacingCount = rebarTemplate->m_nBWallDowelSpacerVerSpacingCount;
	m_nBWallSpacerLenType = rebarTemplate->m_nBWallSpacerLenType;
	m_nBWallSpacerLenCalWay = rebarTemplate->m_nBWallSpacerLenCalWay;
	m_nBWallSpacerLen1 = rebarTemplate->m_nBWallSpacerLen1;
	m_nBWallSpacerLen2 = rebarTemplate->m_nBWallSpacerLen2;
	m_nBWallSpacerShape = rebarTemplate->m_nBWallSpacerShape;
	m_bBWallSpacerRebarPlaceSet = rebarTemplate->m_bBWallSpacerRebarPlaceSet;
	m_nBWallSpacerRebarPlaceCondition = rebarTemplate->m_nBWallSpacerRebarPlaceCondition;

	m_bBWallUserDowelHookLen = rebarTemplate->m_bBWallUserDowelHookLen;
	m_nBWallDowelBarAnchorType = rebarTemplate->m_nBWallDowelBarAnchorType;
	m_nBWallDowelBarTopLenType = rebarTemplate->m_nBWallDowelBarTopLenType;
	m_nBWallDowelSupportBarDiaIndex = rebarTemplate->m_nBWallDowelSupportBarDiaIndex;
	m_nBWallDowelSupportBarNum = rebarTemplate->m_nBWallDowelSupportBarNum;
	m_bBWallDowelSupportPlaceType = rebarTemplate->m_bBWallDowelSupportPlaceType;
	m_nBWallDowelBarSpliceWay = rebarTemplate->m_nBWallDowelBarSpliceWay;
	m_bBWallDowelBarAlterSpliceSet = rebarTemplate->m_bBWallDowelBarAlterSpliceSet;
	m_nBWallDowelBarAlterSpliceValue1 = rebarTemplate->m_nBWallDowelBarAlterSpliceValue1;
	m_nBWallDowelBarAlterSpliceValue2 = rebarTemplate->m_nBWallDowelBarAlterSpliceValue2;
	m_bBWallDowelHookAnchorPlaceSpacingSet = rebarTemplate->m_bBWallDowelHookAnchorPlaceSpacingSet;
	m_nBWallDowelHookAnchorPlaceSpacing = rebarTemplate->m_nBWallDowelHookAnchorPlaceSpacing;
	UserDowelHookLenMapDataCopy(mm_BWallUserDowelHookLen, rebarTemplate->mm_BWallUserDowelHookLen);
	UserDowelSpliceLenMapDataCopy(mm_BWallUserDowelSpliceLen, rebarTemplate->mm_BWallUserDowelSpliceLen);
}

void OldBWallRebarTemplate::OldDataCopy(BWallRebarTemplate* rebarTemplate)
{
	BWallVerBarRebarTemplate *pVerBarTemplateBelowDia = rebarTemplate->GetBWallVerBarTemplateBelowDia();
	BWallVerBarRebarTemplate *pVerBarTemplateExcessDia = rebarTemplate->GetBWallVerBarTemplateExcessDia();
	pVerBarTemplateBelowDia->SetTopLenType(m_nBWallVerBarTopLenType);
	pVerBarTemplateBelowDia->SetBarSpliceType(m_nBWallVerBarSpliceWay);
	pVerBarTemplateBelowDia->SetAlterSpliceSet(m_bBWallVerBarAlterSpliceSet);
	pVerBarTemplateBelowDia->SetConstructionHeight(min(m_nBWallVerBarAlterSpliceValue1, m_nBWallVerBarAlterSpliceValue2));
	pVerBarTemplateBelowDia->SetAlterSpliceDist(abs(m_nBWallVerBarAlterSpliceValue1- m_nBWallVerBarAlterSpliceValue2));

	pVerBarTemplateExcessDia->CopyFromMe(pVerBarTemplateBelowDia);

	BWallDowelRebarTemplate *pDowelTemplateBelowDia = rebarTemplate->GetBWallDowelTemplateBelowDia();
	BWallDowelRebarTemplate *pDowelTemplateExcessDia = rebarTemplate->GetBWallDowelTemplateExcessDia();
	pDowelTemplateBelowDia->SetBarAnchorType(m_nBWallDowelBarAnchorType);
	pDowelTemplateBelowDia->SetTopLenType(m_nBWallDowelBarTopLenType);
	pDowelTemplateBelowDia->SetSupportBarDiaIndex(m_nBWallDowelSupportBarDiaIndex);
	pDowelTemplateBelowDia->SetSupportBarNum(m_nBWallDowelSupportBarNum);
	pDowelTemplateBelowDia->SetBarSpliceType(m_nBWallDowelBarSpliceWay);
	pDowelTemplateBelowDia->SetAlterSpliceSet(m_bBWallDowelBarAlterSpliceSet);
	pDowelTemplateBelowDia->SetConstructionHeight(min(m_nBWallDowelBarAlterSpliceValue1, m_nBWallDowelBarAlterSpliceValue2));
	pDowelTemplateBelowDia->SetAlterSpliceDist(abs(m_nBWallDowelBarAlterSpliceValue1 - m_nBWallDowelBarAlterSpliceValue2));
	pDowelTemplateBelowDia->SetHookAnchorPlaceSpacingSet(m_bBWallDowelHookAnchorPlaceSpacingSet);
	pDowelTemplateBelowDia->SetHookAnchorPlaceSpacing(m_nBWallDowelHookAnchorPlaceSpacing);
	pDowelTemplateBelowDia->SetUserHookLenSet(m_bBWallUserDowelHookLen);

	pDowelTemplateExcessDia->CopyFromMe(pDowelTemplateBelowDia);

	rebarTemplate->SetBWallPlaceEndBarSet(m_bBWallPlaceEndBarSet);
	rebarTemplate->SetBWallEndVerAddBarDist(m_nBWallEndVerAddBarDist);	
	rebarTemplate->SetBWallCrossBarEAType(m_nBWallCrossBarEAType);	
	rebarTemplate->SetBWallVerNumBarCrossBarIncludeSet(m_nBWallVerNumBarCrossBarIncludeSet);
	rebarTemplate->SetBWallAddBarDiaIndexMap(mm_BWallAddBarDiaIndex);
	rebarTemplate->SetBWallEndVerAddBarCount(m_nBWallEndVerAddBarCount);
	rebarTemplate->SetBWallEndBarDiaIndexMap(mm_BWallEndBarDiaIndex);

	rebarTemplate->SetBWallPlaceUBar4ShortWall(m_bBWallPlaceUBar4ShortWall);
	rebarTemplate->SetBWallMaxUbarLegLength(m_nBWallMaxUbarLegLength);
	rebarTemplate->SetBWallPlaceStirrup4ShortWall(m_bBWallPlaceStirrup4ShortWall);
	rebarTemplate->SetBWallHBarMaxStirrupLen(m_nBWallHBarMaxStirrupLen);
	rebarTemplate->SetBWallHorBarStartSet(m_bBWallHorBarStartSet);
	rebarTemplate->SetBWallHorBarStartLength(m_nBWallHorBarStartLength);
	rebarTemplate->SetBWallStraightEndUBarPlaceSet(m_bBWallStraightEndUBarPlaceSet);
	rebarTemplate->SetBWallDefaultUbarLen(m_bBWallDefaultUbarLen);
	rebarTemplate->SetBWallCrossBarUBarHalfPlace(m_bBWallCrossBarUBarHalfPlace);
	rebarTemplate->SetBWallDefaultUbarLenMap(mm_BWallDefaultUbarLen);

	rebarTemplate->SetBWallSpacerDiaType(m_nBWallSpacerDiaType);
	rebarTemplate->SetBWallSpacerDiaIndex(m_nBWallSpacerDiaIndex);
	rebarTemplate->SetBWallSpacerHorSpacing(m_nBWallSpacerHorSpacing);
	rebarTemplate->SetBWallSpacerVerSpacingType(m_nBWallSpacerVerSpacingType);
	rebarTemplate->SetBWallSpacerVerSpacing(m_nBWallSpacerVerSpacing);
	rebarTemplate->SetBWallSpacerVerSpacingRate(m_nBWallSpacerVerSpacingRate);
	rebarTemplate->SetBWallSpacerVerSpacingCount(m_nBWallSpacerVerSpacingCount);
	rebarTemplate->SetBWallDowelSpacerVerSpacingCount(m_nBWallDowelSpacerVerSpacingCount);
	rebarTemplate->SetBWallSpacerLenType(m_nBWallSpacerLenType);
	rebarTemplate->SetBWallSpacerLenCalWayA(m_nBWallSpacerLenCalWay);
	rebarTemplate->SetBWallSpacerLenCalWayB(m_nBWallSpacerLenCalWay);
	rebarTemplate->SetBWallSpacerLenA(m_nBWallSpacerLen1);
	rebarTemplate->SetBWallSpacerLenB(m_nBWallSpacerLen2);
	rebarTemplate->SetBWallSpacerShape(m_nBWallSpacerShape);
	rebarTemplate->SetBWallSpacerRebarPlaceSet(m_bBWallSpacerRebarPlaceSet);
	rebarTemplate->SetBWallSpacerRebarPlaceCondition(m_nBWallSpacerRebarPlaceCondition);
	
	rebarTemplate->SetBWallDowelSupportPlaceType(m_bBWallDowelSupportPlaceType);	
	rebarTemplate->SetBWallUserDowelHookLenMap(mm_BWallUserDowelHookLen);
	rebarTemplate->SetBWallUserDowelSpliceLenMap(mm_BWallUserDowelSpliceLen);
}

void OldBWallRebarTemplate::InfoSetCheckBoxChecked()
{
	m_bBWallVerInfoSet = true;
	m_bBWallHorInfoSet = true;
	m_bBWallSpacerInfoSet = true;
	m_bBWallDowelInfoSet = true;
}
