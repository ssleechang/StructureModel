#include "StdAfx.h"
#include "OldBeamRebarTemplate.h"
#include "..\MSDeformedBar.h"
#include "..\..\GMLib\MSVersionInfo.h"

IMPLEMENT_SERIAL(OldBeamRebarTemplate, OldBaseRebarTemplate, VERSIONABLE_SCHEMA | 1)
OldBeamRebarTemplate::OldBeamRebarTemplate() :OldBaseRebarTemplate()
{
	DataInit();
}

OldBeamRebarTemplate::~OldBeamRebarTemplate(void)
{
}

void OldBeamRebarTemplate::CopyFromMe(OldBeamRebarTemplate* pBeamRebarTemplate)
{
	if (pBeamRebarTemplate == nullptr) return;

	m_bBeamGeneralInfoSet = pBeamRebarTemplate->m_bBeamGeneralInfoSet;
	m_nBeamTopMaxLen2HookAnchorBar = pBeamRebarTemplate->m_nBeamTopMaxLen2HookAnchorBar;
	m_nBeamBotMaxLen2HookAnchorBar = pBeamRebarTemplate->m_nBeamBotMaxLen2HookAnchorBar;
	m_nBeamTopSpliceType = pBeamRebarTemplate->m_nBeamTopSpliceType;

	m_bBeamStirrupInfoSet = pBeamRebarTemplate->m_bBeamStirrupInfoSet;
	m_nBeamShearBarType = pBeamRebarTemplate->m_nBeamShearBarType;
	m_nBeamCapbarRatio = pBeamRebarTemplate->m_nBeamCapbarRatio;
	m_nBeamStirrupType = pBeamRebarTemplate->m_nBeamStirrupType;
	m_nBeamCoverBarType = pBeamRebarTemplate->m_nBeamCoverBarType;

	m_bBeamSubBeamInfoSet = pBeamRebarTemplate->m_bBeamSubBeamInfoSet;
	m_nSubBeamStirrupType = pBeamRebarTemplate->m_nSubBeamStirrupType;
	m_nSubBeamCapbarRatio = pBeamRebarTemplate->m_nSubBeamCapbarRatio;
	m_nSubBeamStirrupShape = pBeamRebarTemplate->m_nSubBeamStirrupShape;
	m_bSubBeamOnlyCenterStirrupPlace = pBeamRebarTemplate->m_bSubBeamOnlyCenterStirrupPlace;

	m_bBeamSubBarInfoSet = pBeamRebarTemplate->m_bBeamSubBarInfoSet;
	m_nBeamTopSubStirrupBarShape = pBeamRebarTemplate->m_nBeamTopSubStirrupBarShape;
	m_nBeamBotSubStirrupBarShape = pBeamRebarTemplate->m_nBeamBotSubStirrupBarShape;
	m_nBeamSubTopStirrupNum = pBeamRebarTemplate->m_nBeamSubTopStirrupNum;
	m_nBeamSubBotStirrupNum = pBeamRebarTemplate->m_nBeamSubBotStirrupNum;
	m_nBeamSubStirrupTopLen = pBeamRebarTemplate->m_nBeamSubStirrupTopLen;
	m_nBeamSubStirrupBotLen = pBeamRebarTemplate->m_nBeamSubStirrupBotLen;
	m_bBeamSubStirrupUserHookLen = pBeamRebarTemplate->m_bBeamSubStirrupUserHookLen;

	SetBeamSubStirrupHookLenMap(pBeamRebarTemplate->mm_BeamSubStirrupHookLen);
	SetBeamShearBarHookLenAMap(pBeamRebarTemplate->mm_BeamShearBarHookLenA);
	SetBeamShearBarHookLenBMap(pBeamRebarTemplate->mm_BeamShearBarHookLenB);
	SetBeamSubStirrupDiaIndexMap(pBeamRebarTemplate->mm_BeamSubStirrupDiaIndex);

	m_nBeamTopSpliceWay = pBeamRebarTemplate->m_nBeamTopSpliceWay;
	m_bBeamDeepDualUBARPlaceSet = pBeamRebarTemplate->m_bBeamDeepDualUBARPlaceSet;
	m_nBeamDeepDepth = pBeamRebarTemplate->m_nBeamDeepDepth;
	m_nBeamDeepSpliceType = pBeamRebarTemplate->m_nBeamDeepSpliceType;

	m_bBuiltUpBeamInfoSet = pBeamRebarTemplate->m_bBuiltUpBeamInfoSet;
	m_nBuiltUpBeamPlacerType = pBeamRebarTemplate->m_nBuiltUpBeamPlacerType;

	m_nBeamSubStirrupBarType = pBeamRebarTemplate->m_nBeamSubStirrupBarType;

	m_nBeamStartSpacingType = pBeamRebarTemplate->m_nBeamStartSpacingType;
	m_nSubBeamStartSpacingType = pBeamRebarTemplate->m_nSubBeamStartSpacingType;
}

void OldBeamRebarTemplate::Dump(EFS::EFS_Buffer& buffer)
{
	buffer << m_bBeamGeneralInfoSet;
	buffer << m_nBeamTopMaxLen2HookAnchorBar;
	buffer << m_nBeamBotMaxLen2HookAnchorBar;
	buffer << m_nBeamTopSpliceType;

	buffer << m_bBeamStirrupInfoSet;
	buffer << m_nBeamShearBarType;
	buffer << m_nBeamCapbarRatio;
	buffer << m_nBeamStirrupType;
	buffer << m_nBeamCoverBarType;

	buffer << m_bBeamSubBeamInfoSet;
	buffer << m_nSubBeamStirrupType;
	buffer << m_nSubBeamCapbarRatio;
	buffer << m_nSubBeamStirrupShape;
	buffer << m_bSubBeamOnlyCenterStirrupPlace;

	buffer << m_bBeamSubBarInfoSet;
	buffer << m_nBeamTopSubStirrupBarShape;
	buffer << m_nBeamBotSubStirrupBarShape;
	buffer << m_nBeamSubTopStirrupNum;
	buffer << m_nBeamSubBotStirrupNum;
	buffer << m_nBeamSubStirrupTopLen;
	buffer << m_nBeamSubStirrupBotLen;
	buffer << m_bBeamSubStirrupUserHookLen;

	map<long, long>::iterator it;
	long nSize = (long)mm_BeamSubStirrupHookLen.size();
	buffer << nSize;
	for (it = mm_BeamSubStirrupHookLen.begin(); it != mm_BeamSubStirrupHookLen.end(); it++)
	{
		long nDiaIndex = it->first;
		long hookLen = it->second;
		buffer << nDiaIndex;
		buffer << hookLen;
	}
	nSize = (long)mm_BeamShearBarHookLenA.size();
	buffer << nSize;
	for (it = mm_BeamShearBarHookLenA.begin(); it != mm_BeamShearBarHookLenA.end(); it++)
	{
		long nDiaIndex = it->first;
		long hookLen = it->second;
		buffer << nDiaIndex;
		buffer << hookLen;
	}
	nSize = (long)mm_BeamShearBarHookLenB.size();
	buffer << nSize;
	for (it = mm_BeamShearBarHookLenB.begin(); it != mm_BeamShearBarHookLenB.end(); it++)
	{
		long nDiaIndex = it->first;
		long hookLen = it->second;
		buffer << nDiaIndex;
		buffer << hookLen;
	}

	buffer << m_nBeamTopSpliceWay;
	buffer << m_bBeamDeepDualUBARPlaceSet;
	buffer << m_nBeamDeepDepth;
	buffer << m_nBeamDeepSpliceType;

	buffer << m_bBuiltUpBeamInfoSet;
	buffer << m_nBuiltUpBeamPlacerType;

	nSize = (long)mm_BeamSubStirrupDiaIndex.size();
	buffer << nSize;
	for (it = mm_BeamSubStirrupDiaIndex.begin(); it != mm_BeamSubStirrupDiaIndex.end(); it++)
	{
		long index1 = it->first;
		long index2 = it->second;
		buffer << index1;
		buffer << index2;
	}

	buffer << m_nBeamSubStirrupBarType;

	buffer << m_nBeamStartSpacingType;
	buffer << m_nSubBeamStartSpacingType;
}

void OldBeamRebarTemplate::Recovery(EFS::EFS_Buffer& buffer)
{
	buffer >> m_bBeamGeneralInfoSet;
	buffer >> m_nBeamTopMaxLen2HookAnchorBar;
	buffer >> m_nBeamBotMaxLen2HookAnchorBar;
	buffer >> m_nBeamTopSpliceType;

	buffer >> m_bBeamStirrupInfoSet;
	buffer >> m_nBeamShearBarType;
	buffer >> m_nBeamCapbarRatio;
	buffer >> m_nBeamStirrupType;
	buffer >> m_nBeamCoverBarType;

	buffer >> m_bBeamSubBeamInfoSet;
	buffer >> m_nSubBeamStirrupType;
	buffer >> m_nSubBeamCapbarRatio;
	buffer >> m_nSubBeamStirrupShape;
	buffer >> m_bSubBeamOnlyCenterStirrupPlace;

	buffer >> m_bBeamSubBarInfoSet;
	buffer >> m_nBeamTopSubStirrupBarShape;
	buffer >> m_nBeamBotSubStirrupBarShape;
	buffer >> m_nBeamSubTopStirrupNum;
	buffer >> m_nBeamSubBotStirrupNum;
	buffer >> m_nBeamSubStirrupTopLen;
	buffer >> m_nBeamSubStirrupBotLen;
	buffer >> m_bBeamSubStirrupUserHookLen;

	long NumObject;
	mm_BeamSubStirrupHookLen.clear();
	buffer >> NumObject;
	for (int index = 0; index < NumObject; index++)
	{
		long nDiaIndex = 0, hookLen = 0;

		buffer >> nDiaIndex;
		buffer >> hookLen;
		mm_BeamSubStirrupHookLen.insert(make_pair(nDiaIndex, hookLen));
	}
	mm_BeamShearBarHookLenA.clear();
	buffer >> NumObject;
	for (int index = 0; index < NumObject; index++)
	{
		long nDiaIndex = 0, hookLen = 0;

		buffer >> nDiaIndex;
		buffer >> hookLen;
		mm_BeamShearBarHookLenA.insert(make_pair(nDiaIndex, hookLen));
	}
	mm_BeamShearBarHookLenB.clear();
	buffer >> NumObject;
	for (int index = 0; index < NumObject; index++)
	{
		long nDiaIndex = 0, hookLen = 0;

		buffer >> nDiaIndex;
		buffer >> hookLen;
		mm_BeamShearBarHookLenB.insert(make_pair(nDiaIndex, hookLen));
	}

	buffer >> m_nBeamTopSpliceWay;
	buffer >> m_bBeamDeepDualUBARPlaceSet;
	buffer >> m_nBeamDeepDepth;
	buffer >> m_nBeamDeepSpliceType;

	buffer >> m_bBuiltUpBeamInfoSet;
	buffer >> m_nBuiltUpBeamPlacerType;

	mm_BeamSubStirrupDiaIndex.clear();
	buffer >> NumObject;
	for (int index = 0; index < NumObject; index++)
	{
		long nDiaIndex1 = 0, nDiaIndex2 = 0;

		buffer >> nDiaIndex1;
		buffer >> nDiaIndex2;
		mm_BeamSubStirrupDiaIndex.insert(make_pair(nDiaIndex1, nDiaIndex2));
	}

	buffer >> m_nBeamSubStirrupBarType;

	buffer >> m_nBeamStartSpacingType;
	buffer >> m_nSubBeamStartSpacingType;
}

void OldBeamRebarTemplate::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_bBeamGeneralInfoSet;
		ar << m_nBeamTopMaxLen2HookAnchorBar;
		ar << m_nBeamBotMaxLen2HookAnchorBar;
		ar << m_nBeamTopSpliceType;

		ar << m_bBeamStirrupInfoSet;
		ar << m_nBeamShearBarType;
		ar << m_nBeamCapbarRatio;
		ar << m_nBeamStirrupType;
		ar << m_nBeamCoverBarType;

		ar << m_bBeamSubBeamInfoSet;
		ar << m_nSubBeamStirrupType;
		ar << m_nSubBeamCapbarRatio;
		ar << m_nSubBeamStirrupShape;
		ar << m_bSubBeamOnlyCenterStirrupPlace;

		ar << m_bBeamSubBarInfoSet;
		ar << m_nBeamTopSubStirrupBarShape;
		ar << m_nBeamBotSubStirrupBarShape;
		ar << m_nBeamSubTopStirrupNum;
		ar << m_nBeamSubBotStirrupNum;
		ar << m_nBeamSubStirrupTopLen;
		ar << m_nBeamSubStirrupBotLen;
		ar << m_bBeamSubStirrupUserHookLen;

		map<long, long>::iterator it;
		ar << (int)mm_BeamSubStirrupHookLen.size();
		for (it = mm_BeamSubStirrupHookLen.begin(); it != mm_BeamSubStirrupHookLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}
		ar << (int)mm_BeamShearBarHookLenA.size();
		for (it = mm_BeamShearBarHookLenA.begin(); it != mm_BeamShearBarHookLenA.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}
		ar << (int)mm_BeamShearBarHookLenB.size();
		for (it = mm_BeamShearBarHookLenB.begin(); it != mm_BeamShearBarHookLenB.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}

		ar << m_nBeamTopSpliceWay;
		ar << m_bBeamDeepDualUBARPlaceSet;
		ar << m_nBeamDeepDepth;
		ar << m_nBeamDeepSpliceType;

		ar << m_bBuiltUpBeamInfoSet;
		ar << m_nBuiltUpBeamPlacerType;

		ar << (int)mm_BeamSubStirrupDiaIndex.size();
		for (it = mm_BeamSubStirrupDiaIndex.begin(); it != mm_BeamSubStirrupDiaIndex.end(); it++)
		{
			long index1 = it->first;
			long index2 = it->second;
			ar << index1;
			ar << index2;
		}

		ar << m_nBeamSubStirrupBarType;

		ar << m_nBeamStartSpacingType;
		ar << m_nSubBeamStartSpacingType;
	}
	else
	{
		ar >> m_bBeamGeneralInfoSet;
		ar >> m_nBeamTopMaxLen2HookAnchorBar;
		ar >> m_nBeamBotMaxLen2HookAnchorBar;
		ar >> m_nBeamTopSpliceType;

		ar >> m_bBeamStirrupInfoSet;
		ar >> m_nBeamShearBarType;
		ar >> m_nBeamCapbarRatio;
		ar >> m_nBeamStirrupType;
		ar >> m_nBeamCoverBarType;

		ar >> m_bBeamSubBeamInfoSet;
		ar >> m_nSubBeamStirrupType;
		ar >> m_nSubBeamCapbarRatio;
		ar >> m_nSubBeamStirrupShape;
		ar >> m_bSubBeamOnlyCenterStirrupPlace;

		ar >> m_bBeamSubBarInfoSet;
		ar >> m_nBeamTopSubStirrupBarShape;
		ar >> m_nBeamBotSubStirrupBarShape;
		ar >> m_nBeamSubTopStirrupNum;
		ar >> m_nBeamSubBotStirrupNum;
		ar >> m_nBeamSubStirrupTopLen;
		ar >> m_nBeamSubStirrupBotLen;
		ar >> m_bBeamSubStirrupUserHookLen;

		int NumObject;
		mm_BeamSubStirrupHookLen.clear();
		ar >> NumObject;
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_BeamSubStirrupHookLen.insert(make_pair(nDiaIndex, hookLen));
		}
		mm_BeamShearBarHookLenA.clear();
		ar >> NumObject;
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_BeamShearBarHookLenA.insert(make_pair(nDiaIndex, hookLen));
		}
		mm_BeamShearBarHookLenB.clear();
		ar >> NumObject;
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_BeamShearBarHookLenB.insert(make_pair(nDiaIndex, hookLen));
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20160212)
		{
			ar >> m_nBeamTopSpliceWay;
			ar >> m_bBeamDeepDualUBARPlaceSet;
			ar >> m_nBeamDeepDepth;
			ar >> m_nBeamDeepSpliceType;
		}
		else
		{
			if (m_nBeamCoverBarType == 0)
				m_nBeamCoverBarType = 2;
			else if (m_nBeamCoverBarType == 1)
				m_nBeamCoverBarType = 0;
			else //if (m_nBeamCoverBarType == 2)
				m_nBeamCoverBarType = 1;

			if (m_nBeamStirrupType == 0)
				m_nBeamStirrupType = 2;
			else if (m_nBeamStirrupType == 1)
				m_nBeamStirrupType = 0;
			else //if (m_nBeamStirrupType == 2)
				m_nBeamStirrupType = 1;
			
			if (m_nBeamShearBarType == 0)
				m_nBeamShearBarType = 1;
			else if (m_nBeamShearBarType == 1)
				m_nBeamShearBarType = 0;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20160809)
		{
			ar >> m_bBuiltUpBeamInfoSet;
			ar >> m_nBuiltUpBeamPlacerType;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20160812)
		{
			mm_BeamSubStirrupDiaIndex.clear();
			ar >> NumObject;
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex1 = 0, nDiaIndex2 = 0;

				ar >> nDiaIndex1;
				ar >> nDiaIndex2;
				mm_BeamSubStirrupDiaIndex.insert(make_pair(nDiaIndex1, nDiaIndex2));
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20160823)
			ar >> m_nBeamSubStirrupBarType;

		if (MSVersionInfo::GetCurrentVersion() >= 20161115)
		{
			ar >> m_nBeamStartSpacingType;
			ar >> m_nSubBeamStartSpacingType;
		}
	}
}

double OldBeamRebarTemplate::GetBeamSubStirrupHookLen(double dDia)
{
	CString sDiaName = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
	map<long, long>::iterator it = mm_BeamSubStirrupHookLen.find(DiaIndex);
	if (it == mm_BeamSubStirrupHookLen.end())
		return 0.;
	return it->second;
}

double OldBeamRebarTemplate::GetBeamShearBarHookLenA(double dDia)
{
	CString sDiaName = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
	map<long, long>::iterator it = mm_BeamShearBarHookLenA.find(DiaIndex);
	if (it == mm_BeamShearBarHookLenA.end())
		return 0.;
	return it->second;
}

double OldBeamRebarTemplate::GetBeamShearBarHookLenB(double dDia)
{
	CString sDiaName = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
	map<long, long>::iterator it = mm_BeamShearBarHookLenB.find(DiaIndex);
	if (it == mm_BeamShearBarHookLenB.end())
		return 0.;
	return it->second;
}

double OldBeamRebarTemplate::GetBeamSubStirrupDia(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);

	map<long, long>::iterator it = mm_BeamSubStirrupDiaIndex.find(DiaIndex);
	if (it == mm_BeamSubStirrupDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}

void OldBeamRebarTemplate::SetBeamSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen)
{
	mm_BeamSubStirrupHookLen.clear();

	map<long, long>::iterator it;
	for (it = mapSubStirrupHookLen.begin(); it != mapSubStirrupHookLen.end(); it++)
	{
		mm_BeamSubStirrupHookLen.insert(make_pair(it->first, it->second));
	}
}

void OldBeamRebarTemplate::SetBeamShearBarHookLenAMap(map<long, long> mapShearBarHookLenA)
{
	mm_BeamShearBarHookLenA.clear();

	map<long, long>::iterator it;
	for (it = mapShearBarHookLenA.begin(); it != mapShearBarHookLenA.end(); it++)
	{
		mm_BeamShearBarHookLenA.insert(make_pair(it->first, it->second));
	}
}

void OldBeamRebarTemplate::SetBeamShearBarHookLenBMap(map<long, long> mapShearBarHookLenB)
{
	mm_BeamShearBarHookLenB.clear();

	map<long, long>::iterator it;
	for (it = mapShearBarHookLenB.begin(); it != mapShearBarHookLenB.end(); it++)
	{
		mm_BeamShearBarHookLenB.insert(make_pair(it->first, it->second));
	}
}

void OldBeamRebarTemplate::SetBeamSubStirrupDiaIndexMap(map<long, long> mapBeamSubStirrupDiaIndex)
{
	mm_BeamSubStirrupDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapBeamSubStirrupDiaIndex.begin(); it != mapBeamSubStirrupDiaIndex.end(); it++)
	{
		mm_BeamSubStirrupDiaIndex.insert(make_pair(it->first, it->second));
	}
}

void OldBeamRebarTemplate::DataInit()
{
	m_bBeamGeneralInfoSet = false;
	m_nBeamTopMaxLen2HookAnchorBar = 2000;
	m_nBeamBotMaxLen2HookAnchorBar = 2000;
	m_nBeamTopSpliceType = 0;
	m_nBeamTopSpliceWay = 0;

	m_bBeamStirrupInfoSet = false;
	m_nBeamShearBarType = 1;
	m_nBeamCapbarRatio = 1;
	m_nBeamStirrupType = 2;
	m_nBeamCoverBarType = 1;
	m_bBeamDeepDualUBARPlaceSet = false;
	m_nBeamDeepDepth = 1500;
	m_nBeamDeepSpliceType = 0;

	m_bBeamSubBeamInfoSet = false;
	m_nSubBeamStirrupType = 1;
	m_nSubBeamCapbarRatio = 1;
	m_nSubBeamStirrupShape = 0;
	m_bSubBeamOnlyCenterStirrupPlace = false;

	m_bBeamSubBarInfoSet = false;
	m_nBeamTopSubStirrupBarShape = 3;
	m_nBeamBotSubStirrupBarShape = 3;
	m_nBeamSubTopStirrupNum = 4;
	m_nBeamSubBotStirrupNum = 3;
	m_nBeamSubStirrupTopLen = 120;
	m_nBeamSubStirrupBotLen = 100;
	m_bBeamSubStirrupUserHookLen = FALSE;
	InitSubStirrupHookLenMap(mm_BeamSubStirrupHookLen);
	InitShearBarHookLenMap(mm_BeamShearBarHookLenA, mm_BeamShearBarHookLenB);

	m_bBuiltUpBeamInfoSet = false;
	m_nBuiltUpBeamPlacerType = 0;

	InitSubStirrupDiaIndexMap(mm_BeamSubStirrupDiaIndex);
	for(int index = 0; index < 3; index++)
		mm_BeamSubStirrupDiaIndex[index] = index;

	m_nBeamSubStirrupBarType = 1;

	m_nBeamStartSpacingType = 0;
	m_nSubBeamStartSpacingType = 0;
}

bool OldBeamRebarTemplate::TopOptionDataCopy(OldBeamRebarTemplate* rebarTemplate)
{
	bool bDataCopy = false;

	if (!m_bBeamGeneralInfoSet)
	{
		m_nBeamTopMaxLen2HookAnchorBar = rebarTemplate->m_nBeamTopMaxLen2HookAnchorBar;
		m_nBeamBotMaxLen2HookAnchorBar = rebarTemplate->m_nBeamBotMaxLen2HookAnchorBar;
		m_nBeamTopSpliceType = rebarTemplate->m_nBeamTopSpliceType;
		m_nBeamTopSpliceWay = rebarTemplate->m_nBeamTopSpliceWay;
		bDataCopy = true;
	}

	if (!m_bBeamStirrupInfoSet)
	{
		m_nBeamShearBarType = rebarTemplate->m_nBeamShearBarType;
		m_nBeamCapbarRatio = rebarTemplate->m_nBeamCapbarRatio;
		m_nBeamStirrupType = rebarTemplate->m_nBeamStirrupType;
		m_nBeamCoverBarType = rebarTemplate->m_nBeamCoverBarType;
		m_bBeamDeepDualUBARPlaceSet = rebarTemplate->m_bBeamDeepDualUBARPlaceSet;
		m_nBeamDeepDepth = rebarTemplate->m_nBeamDeepDepth;
		m_nBeamDeepSpliceType = rebarTemplate->m_nBeamDeepSpliceType;
		m_nBeamStartSpacingType = rebarTemplate->m_nBeamStartSpacingType;
		bDataCopy = true;
	}
	
	if (!m_bBeamSubBeamInfoSet)
	{
		m_nSubBeamStirrupType = rebarTemplate->m_nSubBeamStirrupType;
		m_nSubBeamCapbarRatio = rebarTemplate->m_nSubBeamCapbarRatio;
		m_nSubBeamStirrupShape = rebarTemplate->m_nSubBeamStirrupShape;
		m_bSubBeamOnlyCenterStirrupPlace = rebarTemplate->m_bSubBeamOnlyCenterStirrupPlace;
		m_nSubBeamStartSpacingType = rebarTemplate->m_nSubBeamStartSpacingType;
		bDataCopy = true;
	}
	
	if (!m_bBeamSubBarInfoSet)
	{
		m_nBeamTopSubStirrupBarShape = rebarTemplate->m_nBeamTopSubStirrupBarShape;
		m_nBeamBotSubStirrupBarShape = rebarTemplate->m_nBeamBotSubStirrupBarShape;
		m_nBeamSubTopStirrupNum = rebarTemplate->m_nBeamSubTopStirrupNum;
		m_nBeamSubBotStirrupNum = rebarTemplate->m_nBeamSubBotStirrupNum;
		m_nBeamSubStirrupTopLen = rebarTemplate->m_nBeamSubStirrupTopLen;
		m_nBeamSubStirrupBotLen = rebarTemplate->m_nBeamSubStirrupBotLen;
		m_bBeamSubStirrupUserHookLen = rebarTemplate->m_bBeamSubStirrupUserHookLen;
		m_nBeamSubStirrupBarType = rebarTemplate->m_nBeamSubStirrupBarType;
		SubStirrupHookLenMapDataCopy(mm_BeamSubStirrupHookLen, rebarTemplate->mm_BeamSubStirrupHookLen);
		ShearBarHookLenMapDataCopy(mm_BeamShearBarHookLenA, mm_BeamShearBarHookLenB, rebarTemplate->mm_BeamShearBarHookLenA, rebarTemplate->mm_BeamShearBarHookLenB);
		SubStirrupDiaIndexMapDataCopy(mm_BeamSubStirrupDiaIndex, rebarTemplate->mm_BeamSubStirrupDiaIndex);
		bDataCopy = true;
	}

	if (!m_bBuiltUpBeamInfoSet)
	{
		m_nBuiltUpBeamPlacerType = rebarTemplate->m_nBuiltUpBeamPlacerType;
		bDataCopy = true;
	}

	return bDataCopy;
}

void OldBeamRebarTemplate::DataCopy(OldBeamRebarTemplate* rebarTemplate)
{
	m_nBeamTopMaxLen2HookAnchorBar = rebarTemplate->m_nBeamTopMaxLen2HookAnchorBar;
	m_nBeamBotMaxLen2HookAnchorBar = rebarTemplate->m_nBeamBotMaxLen2HookAnchorBar;
	m_nBeamTopSpliceType = rebarTemplate->m_nBeamTopSpliceType;
	m_nBeamTopSpliceWay = rebarTemplate->m_nBeamTopSpliceWay;

	m_nBeamShearBarType = rebarTemplate->m_nBeamShearBarType;
	m_nBeamCapbarRatio = rebarTemplate->m_nBeamCapbarRatio;
	m_nBeamStirrupType = rebarTemplate->m_nBeamStirrupType;
	m_nBeamCoverBarType = rebarTemplate->m_nBeamCoverBarType;
	m_bBeamDeepDualUBARPlaceSet = rebarTemplate->m_bBeamDeepDualUBARPlaceSet;
	m_nBeamDeepDepth = rebarTemplate->m_nBeamDeepDepth;
	m_nBeamDeepSpliceType = rebarTemplate->m_nBeamDeepSpliceType;
	m_nBeamStartSpacingType = rebarTemplate->m_nBeamStartSpacingType;

	m_nSubBeamStirrupType = rebarTemplate->m_nSubBeamStirrupType;
	m_nSubBeamCapbarRatio = rebarTemplate->m_nSubBeamCapbarRatio;
	m_nSubBeamStirrupShape = rebarTemplate->m_nSubBeamStirrupShape;
	m_bSubBeamOnlyCenterStirrupPlace = rebarTemplate->m_bSubBeamOnlyCenterStirrupPlace;
	m_nSubBeamStartSpacingType = rebarTemplate->m_nSubBeamStartSpacingType;

	m_nBeamTopSubStirrupBarShape = rebarTemplate->m_nBeamTopSubStirrupBarShape;
	m_nBeamBotSubStirrupBarShape = rebarTemplate->m_nBeamBotSubStirrupBarShape;
	m_nBeamSubTopStirrupNum = rebarTemplate->m_nBeamSubTopStirrupNum;
	m_nBeamSubBotStirrupNum = rebarTemplate->m_nBeamSubBotStirrupNum;
	m_nBeamSubStirrupTopLen = rebarTemplate->m_nBeamSubStirrupTopLen;
	m_nBeamSubStirrupBotLen = rebarTemplate->m_nBeamSubStirrupBotLen;
	m_bBeamSubStirrupUserHookLen = rebarTemplate->m_bBeamSubStirrupUserHookLen;
	m_nBeamSubStirrupBarType = rebarTemplate->m_nBeamSubStirrupBarType;
	SubStirrupHookLenMapDataCopy(mm_BeamSubStirrupHookLen, rebarTemplate->mm_BeamSubStirrupHookLen);
	ShearBarHookLenMapDataCopy(mm_BeamShearBarHookLenA, mm_BeamShearBarHookLenB, rebarTemplate->mm_BeamShearBarHookLenA, rebarTemplate->mm_BeamShearBarHookLenB);
	SubStirrupDiaIndexMapDataCopy(mm_BeamSubStirrupDiaIndex, rebarTemplate->mm_BeamSubStirrupDiaIndex);

	m_nBuiltUpBeamPlacerType = rebarTemplate->m_nBuiltUpBeamPlacerType;
}

void OldBeamRebarTemplate::OldDataCopy(BeamRebarTemplate* rebarTemplate)
{
	rebarTemplate->SetBeamTopMaxLen2HookAnchorBar(m_nBeamTopMaxLen2HookAnchorBar);
	rebarTemplate->SetBeamBotMaxLen2HookAnchorBar(m_nBeamBotMaxLen2HookAnchorBar);
	rebarTemplate->SetBeamSpliceTypeBelowDia(m_nBeamTopSpliceType);
	rebarTemplate->SetBeamSpliceWayBelowDia(m_nBeamTopSpliceWay);
	rebarTemplate->SetBeamSpliceTypeExcessDia(m_nBeamTopSpliceType);
	rebarTemplate->SetBeamSpliceWayExcessDia(m_nBeamTopSpliceWay);

	rebarTemplate->SetBeamShearBarType(m_nBeamShearBarType);
	rebarTemplate->SetBeamCapbarRatio(m_nBeamCapbarRatio);
	rebarTemplate->SetBeamStirrupType(m_nBeamStirrupType);
	rebarTemplate->SetBeamCoverBarType(m_nBeamCoverBarType);
	rebarTemplate->SetBeamDeepDualUBARPlaceSet(m_bBeamDeepDualUBARPlaceSet);
	rebarTemplate->SetBeamDeepDepth(m_nBeamDeepDepth);
	rebarTemplate->SetBeamDeepSpliceType(m_nBeamDeepSpliceType);
	rebarTemplate->SetBeamStartSpacingType(m_nBeamStartSpacingType);

	rebarTemplate->SetSubBeamStirrupType(m_nSubBeamStirrupType);
	rebarTemplate->SetSubBeamCapbarRatio(m_nSubBeamCapbarRatio);
	rebarTemplate->SetSubBeamStirrupShape(m_nSubBeamStirrupShape);
	rebarTemplate->SetSubBeamOnlyCenterStirrupPlace(m_bSubBeamOnlyCenterStirrupPlace);
	rebarTemplate->SetSubBeamStartSpacingType(m_nSubBeamStartSpacingType);

	rebarTemplate->SetBeamTopSubStirrupBarShape(m_nBeamTopSubStirrupBarShape);
	rebarTemplate->SetBeamBotSubStirrupBarShape(m_nBeamBotSubStirrupBarShape);
	rebarTemplate->SetBeamSubTopStirrupNum(m_nBeamSubTopStirrupNum);
	rebarTemplate->SetBeamSubBotStirrupNum(m_nBeamSubBotStirrupNum);
	rebarTemplate->SetBeamSubStirrupTopLen(m_nBeamSubStirrupTopLen);
	rebarTemplate->SetBeamSubStirrupBotLen(m_nBeamSubStirrupBotLen);
	rebarTemplate->SetBeamSubStirrupUserHookLen(m_bBeamSubStirrupUserHookLen);
	rebarTemplate->SetBeamSubStirrupBarType(m_nBeamSubStirrupBarType);
	rebarTemplate->SetBeamTopSubStirrupHookLenMap(mm_BeamSubStirrupHookLen);
	rebarTemplate->SetBeamBotSubStirrupHookLenMap(mm_BeamSubStirrupHookLen);
	rebarTemplate->SetBeamShearBarHookLenAMap(mm_BeamShearBarHookLenA);
	rebarTemplate->SetBeamShearBarHookLenBMap(mm_BeamShearBarHookLenB);
	rebarTemplate->SetBeamTopSubStirrupDiaIndexMap(mm_BeamSubStirrupDiaIndex);
	rebarTemplate->SetBeamBotSubStirrupDiaIndexMap(mm_BeamSubStirrupDiaIndex);

	//rebarTemplate->SetBuiltUpBeamPlacerType(m_nBuiltUpBeamPlacerType);
}

void OldBeamRebarTemplate::InfoSetCheckBoxChecked()
{
	m_bBeamGeneralInfoSet = true;
	m_bBeamStirrupInfoSet = true;
	m_bBeamSubBeamInfoSet = true;
	m_bBeamSubBarInfoSet = true;
	m_bBuiltUpBeamInfoSet = true;
}
