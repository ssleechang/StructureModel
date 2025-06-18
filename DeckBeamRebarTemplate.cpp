#include "StdAfx.h"
#include "DeckBeamRebarTemplate.h"
#include "MSDeformedBar.h"
#include "..\GMLib\MSVersionInfo.h"

DeckBeamRebarTemplate::DeckBeamRebarTemplate() :BaseRebarTemplate()
{
	DataInit();
}

DeckBeamRebarTemplate::~DeckBeamRebarTemplate(void)
{
}

void DeckBeamRebarTemplate::CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate)
{
	DeckBeamRebarTemplate* pBeamRebarTemplate = dynamic_cast<DeckBeamRebarTemplate*>(pBaseRebarTemplate);
	if (pBeamRebarTemplate == nullptr) return;
	
	m_bDeckBeamGeneralInfoSet = pBeamRebarTemplate->m_bDeckBeamGeneralInfoSet;
	m_nDeckBeamTopMaxLen2HookAnchorBar = pBeamRebarTemplate->m_nDeckBeamTopMaxLen2HookAnchorBar;
	m_nDeckBeamBotMaxLen2HookAnchorBar = pBeamRebarTemplate->m_nDeckBeamBotMaxLen2HookAnchorBar;
	m_nDeckBeamBarLayerGap = pBeamRebarTemplate->m_nDeckBeamBarLayerGap;
	m_nDeckBeamMainBarConnectionTolerance = pBeamRebarTemplate->GetDeckBeamMainBarConnectionTolerance();
	m_nDeckBeamLineContinuityHorEcc = pBeamRebarTemplate->GetDeckBeamLineContinuityHorEcc();
	m_nDeckBeamLineContinuityVerEcc = pBeamRebarTemplate->GetDeckBeamLineContinuityVerEcc();

	m_bDeckBeamSpliceDevConditionSet = pBeamRebarTemplate->m_bDeckBeamSpliceDevConditionSet;
	m_nDeckBeamSpliceDevConditionLength = pBeamRebarTemplate->m_nDeckBeamSpliceDevConditionLength;

	m_bDeckBeamStirrupInfoSet = pBeamRebarTemplate->m_bDeckBeamStirrupInfoSet;
	m_nDeckBeamShearBarType = pBeamRebarTemplate->m_nDeckBeamShearBarType;
	m_nDeckBeamHorShearBarType = pBeamRebarTemplate->m_nDeckBeamHorShearBarType;
	m_nDeckBeamCapbarRatio = pBeamRebarTemplate->m_nDeckBeamCapbarRatio;
	m_nDeckBeamStirrupType = pBeamRebarTemplate->m_nDeckBeamStirrupType;
	m_nDeckBeamCoverBarType = pBeamRebarTemplate->m_nDeckBeamCoverBarType;

	m_bDeckBeamSubBeamInfoSet = pBeamRebarTemplate->m_bDeckBeamSubBeamInfoSet;
	m_nDeckBeamSubBeamStirrupType = pBeamRebarTemplate->m_nDeckBeamSubBeamStirrupType;
	m_nDeckBeamSubBeamCapbarRatio = pBeamRebarTemplate->m_nDeckBeamSubBeamCapbarRatio;
	m_nDeckBeamSubBeamStirrupShape = pBeamRebarTemplate->m_nDeckBeamSubBeamStirrupShape;
	m_bDeckBeamSubBeamOnlyCenterStirrupPlace = pBeamRebarTemplate->m_bDeckBeamSubBeamOnlyCenterStirrupPlace;

	m_bDeckBeamSubBarInfoSet = pBeamRebarTemplate->m_bDeckBeamSubBarInfoSet;
	m_nDeckBeamTopSubStirrupBarShape = pBeamRebarTemplate->m_nDeckBeamTopSubStirrupBarShape;
	m_nDeckBeamBotSubStirrupBarShape = pBeamRebarTemplate->m_nDeckBeamBotSubStirrupBarShape;
	m_nDeckBeamSubStirrupStartSpacing = pBeamRebarTemplate->m_nDeckBeamSubStirrupStartSpacing;
	m_nDeckBeamSubTopStirrupType = pBeamRebarTemplate->m_nDeckBeamSubTopStirrupType;
	m_nDeckBeamSubTopStirrupNum = pBeamRebarTemplate->m_nDeckBeamSubTopStirrupNum;
	m_nDeckBeamSubTopStirrupSpacing = pBeamRebarTemplate->m_nDeckBeamSubTopStirrupSpacing;
	m_nDeckBeamSubBotStirrupType = pBeamRebarTemplate->m_nDeckBeamSubBotStirrupType;
	m_nDeckBeamSubBotStirrupNum = pBeamRebarTemplate->m_nDeckBeamSubBotStirrupNum;
	m_nDeckBeamSubBotStirrupSpacing = pBeamRebarTemplate->m_nDeckBeamSubBotStirrupSpacing;
	m_nDeckBeamSubStirrupTopLen = pBeamRebarTemplate->m_nDeckBeamSubStirrupTopLen;
	m_nDeckBeamSubStirrupBotLen = pBeamRebarTemplate->m_nDeckBeamSubStirrupBotLen;
	m_bDeckBeamSubStirrupUserHookLen = pBeamRebarTemplate->m_bDeckBeamSubStirrupUserHookLen;

	SetDeckBeamTopSubStirrupHookLenMap(pBeamRebarTemplate->mm_DeckBeamTopSubStirrupHookLen);
	SetDeckBeamBotSubStirrupHookLenMap(pBeamRebarTemplate->mm_DeckBeamBotSubStirrupHookLen);
	SetDeckBeamShearBarHookLenAMap(pBeamRebarTemplate->mm_DeckBeamShearBarHookLenA);
	SetDeckBeamShearBarHookLenBMap(pBeamRebarTemplate->mm_DeckBeamShearBarHookLenB);
	SetDeckBeamTopSubStirrupDiaIndexMap(pBeamRebarTemplate->mm_DeckBeamTopSubStirrupDiaIndex);
	SetDeckBeamBotSubStirrupDiaIndexMap(pBeamRebarTemplate->mm_DeckBeamBotSubStirrupDiaIndex);
	
	//m_bBuiltUpBeamInfoSet = pBeamRebarTemplate->m_bBuiltUpBeamInfoSet;
	//m_nBuiltUpBeamPlacerType = pBeamRebarTemplate->m_nBuiltUpBeamPlacerType;

	m_nDeckBeamSubStirrupBarType = pBeamRebarTemplate->m_nDeckBeamSubStirrupBarType;

	m_nDeckBeamStartSpacingType = pBeamRebarTemplate->m_nDeckBeamStartSpacingType;
	m_nDeckBeamSubBeamStartSpacingType = pBeamRebarTemplate->m_nDeckBeamSubBeamStartSpacingType;

	m_nDeckBeamCapBarPlaceType = pBeamRebarTemplate->m_nDeckBeamCapBarPlaceType;
	m_nDeckBeamCapBarNum = pBeamRebarTemplate->m_nDeckBeamCapBarNum;

	m_nDeckBeamSubBeamCapBarPlaceType = pBeamRebarTemplate->m_nDeckBeamSubBeamCapBarPlaceType;
	m_nDeckBeamSubBeamCapBarNum = pBeamRebarTemplate->m_nDeckBeamSubBeamCapBarNum;

	m_bDeckBeamBottomBarEnd150Dev = pBeamRebarTemplate->m_bDeckBeamBottomBarEnd150Dev;
	m_bDeckBeamBottomBarLink150Dev = pBeamRebarTemplate->m_bDeckBeamBottomBarLink150Dev;

	m_bDeckBeamSubBeamBottomBarEnd150Dev = pBeamRebarTemplate->m_bDeckBeamSubBeamBottomBarEnd150Dev;
	m_bDeckBeamSubBeamBottomBarLink150Dev = pBeamRebarTemplate->m_bDeckBeamSubBeamBottomBarLink150Dev;
	
	m_nDeckBeamPlaceTypeDiaIndex = pBeamRebarTemplate->m_nDeckBeamPlaceTypeDiaIndex;

	m_nDeckBeamSpliceTypeBelowDia = pBeamRebarTemplate->m_nDeckBeamSpliceTypeBelowDia;
	m_nDeckBeamSpliceWayBelowDia = pBeamRebarTemplate->m_nDeckBeamSpliceWayBelowDia;
	m_nDeckBeamCZoneSpliceWayBelowDia = pBeamRebarTemplate->m_nDeckBeamCZoneSpliceWayBelowDia;
	m_nDeckBeamCZoneSpliceLengthBelowDia = pBeamRebarTemplate->m_nDeckBeamCZoneSpliceLengthBelowDia;

	m_nDeckBeamSpliceTypeExcessDia = pBeamRebarTemplate->m_nDeckBeamSpliceTypeExcessDia;
	m_nDeckBeamSpliceWayExcessDia = pBeamRebarTemplate->m_nDeckBeamSpliceWayExcessDia;
	m_nDeckBeamCZoneSpliceWayExcessDia = pBeamRebarTemplate->m_nDeckBeamCZoneSpliceWayExcessDia;
	m_nDeckBeamCZoneSpliceLengthExcessDia = pBeamRebarTemplate->m_nDeckBeamCZoneSpliceLengthExcessDia;

	 m_bDeckBeamSpliceAddLengthSet = pBeamRebarTemplate->GetDeckBeamSpliceAddLengthSet();
	 m_nDeckBeamSpliceAddLengthNumSplice = pBeamRebarTemplate->GetDeckBeamSpliceAddLengthNumSplice();

	 m_nDeckBeamSidePressureBarDiaIndex = pBeamRebarTemplate->GetDeckBeamSidePressureBarDiaIndex();	 
	 m_nDeckBeamSidePressureBarNum = pBeamRebarTemplate->GetDeckBeamSidePressureBarNum();
	 m_nDeckBeamSidePressureBarSpacing = pBeamRebarTemplate->GetDeckBeamSidePressureBarSpacing();
}

void DeckBeamRebarTemplate::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_bDeckBeamGeneralInfoSet;
		ar << m_nDeckBeamTopMaxLen2HookAnchorBar;
		ar << m_nDeckBeamBotMaxLen2HookAnchorBar;
		ar << m_nDeckBeamSpliceTypeBelowDia;

		ar << m_bDeckBeamStirrupInfoSet;
		ar << m_nDeckBeamShearBarType;
		ar << m_nDeckBeamCapbarRatio;
		ar << m_nDeckBeamStirrupType;
		ar << m_nDeckBeamCoverBarType;

		ar << m_bDeckBeamSubBeamInfoSet;
		ar << m_nDeckBeamSubBeamStirrupType;
		ar << m_nDeckBeamSubBeamCapbarRatio;
		ar << m_nDeckBeamSubBeamStirrupShape;
		ar << m_bDeckBeamSubBeamOnlyCenterStirrupPlace;

		ar << m_bDeckBeamSubBarInfoSet;
		ar << m_nDeckBeamTopSubStirrupBarShape;
		ar << m_nDeckBeamBotSubStirrupBarShape;
		ar << m_nDeckBeamSubTopStirrupNum;
		ar << m_nDeckBeamSubBotStirrupNum;
		ar << m_nDeckBeamSubStirrupTopLen;
		ar << m_nDeckBeamSubStirrupBotLen;
		ar << m_bDeckBeamSubStirrupUserHookLen;

		map<long, long>::iterator it;
		ar << (int)mm_DeckBeamTopSubStirrupHookLen.size();
		for (it = mm_DeckBeamTopSubStirrupHookLen.begin(); it != mm_DeckBeamTopSubStirrupHookLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}
		ar << (int)mm_DeckBeamShearBarHookLenA.size();
		for (it = mm_DeckBeamShearBarHookLenA.begin(); it != mm_DeckBeamShearBarHookLenA.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}
		ar << (int)mm_DeckBeamShearBarHookLenB.size();
		for (it = mm_DeckBeamShearBarHookLenB.begin(); it != mm_DeckBeamShearBarHookLenB.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}

		ar << m_nDeckBeamSpliceWayBelowDia;
		
		ar << (int)mm_DeckBeamTopSubStirrupDiaIndex.size();
		for (it = mm_DeckBeamTopSubStirrupDiaIndex.begin(); it != mm_DeckBeamTopSubStirrupDiaIndex.end(); it++)
		{
			long index1 = it->first;
			long index2 = it->second;
			ar << index1;
			ar << index2;
		}

		ar << m_nDeckBeamSubStirrupBarType;

		ar << m_nDeckBeamStartSpacingType;
		ar << m_nDeckBeamSubBeamStartSpacingType;

		ar << m_nDeckBeamSubStirrupStartSpacing;
		ar << m_nDeckBeamSubTopStirrupType;
		ar << m_nDeckBeamSubTopStirrupSpacing;
		ar << m_nDeckBeamSubBotStirrupType;
		ar << m_nDeckBeamSubBotStirrupSpacing;

		ar << m_nDeckBeamBarLayerGap;

		ar << m_bDeckBeamSpliceDevConditionSet;
		ar << m_nDeckBeamSpliceDevConditionLength;

		ar << m_nDeckBeamHorShearBarType;
		ar << m_nDeckBeamCapBarPlaceType;
		ar << m_nDeckBeamCapBarNum;

		ar << m_nDeckBeamSubBeamCapBarPlaceType;
		ar << m_nDeckBeamSubBeamCapBarNum;
		
		ar << m_nDeckBeamMainBarConnectionTolerance;
		ar << m_nDeckBeamLineContinuityHorEcc;
		ar << m_nDeckBeamLineContinuityVerEcc;

		ar << m_nDeckBeamCZoneSpliceWayBelowDia;
		ar << m_nDeckBeamCZoneSpliceLengthBelowDia;
		ar << m_bDeckBeamBottomBarEnd150Dev;
		ar << m_bDeckBeamBottomBarLink150Dev;

		ar << m_bDeckBeamSubBeamBottomBarEnd150Dev;
		ar << m_bDeckBeamSubBeamBottomBarLink150Dev;
		
		ar << (int)mm_DeckBeamBotSubStirrupHookLen.size();
		for (it = mm_DeckBeamBotSubStirrupHookLen.begin(); it != mm_DeckBeamBotSubStirrupHookLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}
		ar << (int)mm_DeckBeamBotSubStirrupDiaIndex.size();
		for (it = mm_DeckBeamBotSubStirrupDiaIndex.begin(); it != mm_DeckBeamBotSubStirrupDiaIndex.end(); it++)
		{
			long index1 = it->first;
			long index2 = it->second;
			ar << index1;
			ar << index2;
		}

		ar << m_nDeckBeamPlaceTypeDiaIndex;
		ar << m_nDeckBeamSpliceTypeExcessDia;
		ar << m_nDeckBeamSpliceWayExcessDia;
		ar << m_nDeckBeamCZoneSpliceWayExcessDia;
		ar << m_nDeckBeamCZoneSpliceLengthExcessDia;

		ar << m_bDeckBeamSpliceAddLengthSet;
		ar << m_nDeckBeamSpliceAddLengthNumSplice;

		ar << m_nDeckBeamSidePressureBarDiaIndex;
		ar << m_nDeckBeamSidePressureBarNum;
		ar << m_nDeckBeamSidePressureBarSpacing;
	}
	else
	{
		ar >> m_bDeckBeamGeneralInfoSet;
		ar >> m_nDeckBeamTopMaxLen2HookAnchorBar;
		ar >> m_nDeckBeamBotMaxLen2HookAnchorBar;
		ar >> m_nDeckBeamSpliceTypeBelowDia;

		ar >> m_bDeckBeamStirrupInfoSet;
		ar >> m_nDeckBeamShearBarType;
		ar >> m_nDeckBeamCapbarRatio;
		ar >> m_nDeckBeamStirrupType;
		ar >> m_nDeckBeamCoverBarType;

		ar >> m_bDeckBeamSubBeamInfoSet;
		ar >> m_nDeckBeamSubBeamStirrupType;
		ar >> m_nDeckBeamSubBeamCapbarRatio;
		ar >> m_nDeckBeamSubBeamStirrupShape;
		ar >> m_bDeckBeamSubBeamOnlyCenterStirrupPlace;

		ar >> m_bDeckBeamSubBarInfoSet;
		ar >> m_nDeckBeamTopSubStirrupBarShape;
		ar >> m_nDeckBeamBotSubStirrupBarShape;
		ar >> m_nDeckBeamSubTopStirrupNum;
		ar >> m_nDeckBeamSubBotStirrupNum;
		ar >> m_nDeckBeamSubStirrupTopLen;
		ar >> m_nDeckBeamSubStirrupBotLen;
		ar >> m_bDeckBeamSubStirrupUserHookLen;

		int NumObject;
		
		ar >> NumObject;
		if(NumObject > 0)
			mm_DeckBeamTopSubStirrupHookLen.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_DeckBeamTopSubStirrupHookLen.insert(make_pair(nDiaIndex, hookLen));
		}
		
		ar >> NumObject;
		if (NumObject > 0)
			mm_DeckBeamShearBarHookLenA.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_DeckBeamShearBarHookLenA.insert(make_pair(nDiaIndex, hookLen));
		}
		
		ar >> NumObject;
		if (NumObject > 0)
			mm_DeckBeamShearBarHookLenB.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_DeckBeamShearBarHookLenB.insert(make_pair(nDiaIndex, hookLen));
		}

		ar >> m_nDeckBeamSpliceWayBelowDia;
		
		ar >> NumObject;
		if (NumObject > 0)
			mm_DeckBeamTopSubStirrupDiaIndex.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex1 = 0, nDiaIndex2 = 0;

			ar >> nDiaIndex1;
			ar >> nDiaIndex2;
			mm_DeckBeamTopSubStirrupDiaIndex.insert(make_pair(nDiaIndex1, nDiaIndex2));
		}

		ar >> m_nDeckBeamSubStirrupBarType;

		ar >> m_nDeckBeamStartSpacingType;
		ar >> m_nDeckBeamSubBeamStartSpacingType;

		
		ar >> m_nDeckBeamSubStirrupStartSpacing;
		ar >> m_nDeckBeamSubTopStirrupType;
		ar >> m_nDeckBeamSubTopStirrupSpacing;
		ar >> m_nDeckBeamSubBotStirrupType;
		ar >> m_nDeckBeamSubBotStirrupSpacing;
		
		ar >> m_nDeckBeamBarLayerGap;
		
		ar >> m_bDeckBeamSpliceDevConditionSet;
		ar >> m_nDeckBeamSpliceDevConditionLength;
		
		ar >> m_nDeckBeamHorShearBarType;
	
		ar >> m_nDeckBeamCapBarPlaceType;
		ar >> m_nDeckBeamCapBarNum;
	
		ar >> m_nDeckBeamSubBeamCapBarPlaceType;
		ar >> m_nDeckBeamSubBeamCapBarNum;
	
		ar >> m_nDeckBeamMainBarConnectionTolerance;
		ar >> m_nDeckBeamLineContinuityHorEcc;
		ar >> m_nDeckBeamLineContinuityVerEcc;
			
		ar >> m_nDeckBeamCZoneSpliceWayBelowDia;
		ar >> m_nDeckBeamCZoneSpliceLengthBelowDia;
	
		ar >> m_bDeckBeamBottomBarEnd150Dev;
		ar >> m_bDeckBeamBottomBarLink150Dev;
	
		ar >> m_bDeckBeamSubBeamBottomBarEnd150Dev;
		ar >> m_bDeckBeamSubBeamBottomBarLink150Dev;
	
		
		ar >> NumObject;
		if (NumObject > 0)
			mm_DeckBeamBotSubStirrupHookLen.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_DeckBeamBotSubStirrupHookLen.insert(make_pair(nDiaIndex, hookLen));
		}
		ar >> NumObject;
		if (NumObject > 0)
			mm_DeckBeamBotSubStirrupDiaIndex.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex1 = 0, nDiaIndex2 = 0;

			ar >> nDiaIndex1;
			ar >> nDiaIndex2;
			mm_DeckBeamBotSubStirrupDiaIndex.insert(make_pair(nDiaIndex1, nDiaIndex2));
		}
		
		
		
		ar >> m_nDeckBeamPlaceTypeDiaIndex;
		ar >> m_nDeckBeamSpliceTypeExcessDia;
		ar >> m_nDeckBeamSpliceWayExcessDia;
		ar >> m_nDeckBeamCZoneSpliceWayExcessDia;
		ar >> m_nDeckBeamCZoneSpliceLengthExcessDia;
		
		ar >> m_bDeckBeamSpliceAddLengthSet;
		ar >> m_nDeckBeamSpliceAddLengthNumSplice;
		
		ar >> m_nDeckBeamSidePressureBarDiaIndex;
		ar >> m_nDeckBeamSidePressureBarNum;
		ar >> m_nDeckBeamSidePressureBarSpacing;
	}
}

double DeckBeamRebarTemplate::GetDeckBeamTopSubStirrupHookLen(double dDia)
{
	CString sDiaName = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
	map<long, long>::iterator it = mm_DeckBeamTopSubStirrupHookLen.find(DiaIndex);
	if (it == mm_DeckBeamTopSubStirrupHookLen.end())
		return 0.;
	return it->second;
}
double DeckBeamRebarTemplate::GetDeckBeamBotSubStirrupHookLen(double dDia)
{
	CString sDiaName = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
	map<long, long>::iterator it = mm_DeckBeamBotSubStirrupHookLen.find(DiaIndex);
	if (it == mm_DeckBeamBotSubStirrupHookLen.end())
		return 0.;
	return it->second;
}
double DeckBeamRebarTemplate::GetDeckBeamShearBarHookLenA(double dDia)
{
	CString sDiaName = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
	map<long, long>::iterator it = mm_DeckBeamShearBarHookLenA.find(DiaIndex);
	if (it == mm_DeckBeamShearBarHookLenA.end())
		return 0.;
	return it->second;
}

double DeckBeamRebarTemplate::GetDeckBeamShearBarHookLenB(double dDia)
{
	CString sDiaName = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
	map<long, long>::iterator it = mm_DeckBeamShearBarHookLenB.find(DiaIndex);
	if (it == mm_DeckBeamShearBarHookLenB.end())
		return 0.;
	return it->second;
}

double DeckBeamRebarTemplate::GetDeckBeamTopSubStirrupDia(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);

	map<long, long>::iterator it = mm_DeckBeamTopSubStirrupDiaIndex.find(DiaIndex);
	if (it == mm_DeckBeamTopSubStirrupDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}
double DeckBeamRebarTemplate::GetDeckBeamBotSubStirrupDia(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);

	map<long, long>::iterator it = mm_DeckBeamBotSubStirrupDiaIndex.find(DiaIndex);
	if (it == mm_DeckBeamBotSubStirrupDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}

void DeckBeamRebarTemplate::SetDeckBeamTopSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen)
{
	mm_DeckBeamTopSubStirrupHookLen.clear();

	map<long, long>::iterator it;
	for (it = mapSubStirrupHookLen.begin(); it != mapSubStirrupHookLen.end(); it++)
	{
		mm_DeckBeamTopSubStirrupHookLen.insert(make_pair(it->first, it->second));
	}
}
void DeckBeamRebarTemplate::SetDeckBeamBotSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen)
{
	mm_DeckBeamBotSubStirrupHookLen.clear();

	map<long, long>::iterator it;
	for (it = mapSubStirrupHookLen.begin(); it != mapSubStirrupHookLen.end(); it++)
	{
		mm_DeckBeamBotSubStirrupHookLen.insert(make_pair(it->first, it->second));
	}
}
void DeckBeamRebarTemplate::SetDeckBeamShearBarHookLenAMap(map<long, long> mapShearBarHookLenA)
{
	mm_DeckBeamShearBarHookLenA.clear();

	map<long, long>::iterator it;
	for (it = mapShearBarHookLenA.begin(); it != mapShearBarHookLenA.end(); it++)
	{
		mm_DeckBeamShearBarHookLenA.insert(make_pair(it->first, it->second));
	}
}

void DeckBeamRebarTemplate::SetDeckBeamShearBarHookLenBMap(map<long, long> mapShearBarHookLenB)
{
	mm_DeckBeamShearBarHookLenB.clear();

	map<long, long>::iterator it;
	for (it = mapShearBarHookLenB.begin(); it != mapShearBarHookLenB.end(); it++)
	{
		mm_DeckBeamShearBarHookLenB.insert(make_pair(it->first, it->second));
	}
}

void DeckBeamRebarTemplate::SetDeckBeamTopSubStirrupDiaIndexMap(map<long, long> mapBeamSubStirrupDiaIndex)
{
	mm_DeckBeamTopSubStirrupDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapBeamSubStirrupDiaIndex.begin(); it != mapBeamSubStirrupDiaIndex.end(); it++)
	{
		mm_DeckBeamTopSubStirrupDiaIndex.insert(make_pair(it->first, it->second));
	}
}
void DeckBeamRebarTemplate::SetDeckBeamBotSubStirrupDiaIndexMap(map<long, long> mapBeamSubStirrupDiaIndex)
{
	mm_DeckBeamBotSubStirrupDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapBeamSubStirrupDiaIndex.begin(); it != mapBeamSubStirrupDiaIndex.end(); it++)
	{
		mm_DeckBeamBotSubStirrupDiaIndex.insert(make_pair(it->first, it->second));
	}
}
void DeckBeamRebarTemplate::DataInit()
{
	m_bDeckBeamGeneralInfoSet = false;
	m_nDeckBeamTopMaxLen2HookAnchorBar = 2000;
	m_nDeckBeamBotMaxLen2HookAnchorBar = 2000;
	m_nDeckBeamBarLayerGap = 25;
	m_nDeckBeamMainBarConnectionTolerance = 50;
	m_nDeckBeamLineContinuityHorEcc = 50;
	m_nDeckBeamLineContinuityVerEcc = 50;

	m_bDeckBeamSpliceDevConditionSet = false;
	m_nDeckBeamSpliceDevConditionLength = 340;

	m_bDeckBeamStirrupInfoSet = false;
	m_nDeckBeamShearBarType = 1;
	m_nDeckBeamHorShearBarType = 1;
	m_nDeckBeamCapbarRatio = 1;
	m_nDeckBeamStirrupType = 2;
	m_nDeckBeamCoverBarType = 1;

	m_bDeckBeamSubBeamInfoSet = false;
	m_nDeckBeamSubBeamStirrupType = 1;
	m_nDeckBeamSubBeamCapbarRatio = 1;
	m_nDeckBeamSubBeamStirrupShape = 0;
	m_bDeckBeamSubBeamOnlyCenterStirrupPlace = false;

	m_bDeckBeamSubBarInfoSet = false;
	m_nDeckBeamTopSubStirrupBarShape = 3;
	m_nDeckBeamBotSubStirrupBarShape = 3;
	m_nDeckBeamSubStirrupStartSpacing = 500;
	m_nDeckBeamSubTopStirrupType = 0;
	m_nDeckBeamSubTopStirrupNum = 4;
	m_nDeckBeamSubTopStirrupSpacing = 1000;
	m_nDeckBeamSubBotStirrupType = 0;
	m_nDeckBeamSubBotStirrupNum = 3;
	m_nDeckBeamSubBotStirrupSpacing = 1000;
	m_nDeckBeamSubStirrupTopLen = 120;
	m_nDeckBeamSubStirrupBotLen = 100;
	m_bDeckBeamSubStirrupUserHookLen = FALSE;
	InitSubStirrupHookLenMap(mm_DeckBeamTopSubStirrupHookLen);
	InitSubStirrupHookLenMap(mm_DeckBeamBotSubStirrupHookLen);
	InitShearBarHookLenMap(mm_DeckBeamShearBarHookLenA, mm_DeckBeamShearBarHookLenB);

	//m_bBuiltUpBeamInfoSet = false;
	//m_nBuiltUpBeamPlacerType = 0;

	InitSubStirrupDiaIndexMap(mm_DeckBeamTopSubStirrupDiaIndex);
	InitSubStirrupDiaIndexMap(mm_DeckBeamBotSubStirrupDiaIndex);
	for(int index = 0; index < 3; index++)
	{
		mm_DeckBeamTopSubStirrupDiaIndex[index] = index;
		mm_DeckBeamBotSubStirrupDiaIndex[index] = index;
	}

	m_nDeckBeamSubStirrupBarType = 1;

	m_nDeckBeamStartSpacingType = 0;
	m_nDeckBeamSubBeamStartSpacingType = 0;

	

	m_nDeckBeamCapBarPlaceType = 0;
	m_nDeckBeamCapBarNum = 4;

	m_nDeckBeamSubBeamCapBarPlaceType = 0;
	m_nDeckBeamSubBeamCapBarNum = 4;

	m_bDeckBeamBottomBarEnd150Dev = FALSE;
	m_bDeckBeamBottomBarLink150Dev = FALSE;

	m_bDeckBeamSubBeamBottomBarEnd150Dev = FALSE;
	m_bDeckBeamSubBeamBottomBarLink150Dev = FALSE;

	

	m_nDeckBeamPlaceTypeDiaIndex = 11;
	m_nDeckBeamSpliceTypeBelowDia = 0;
	m_nDeckBeamSpliceWayBelowDia = 0;
	m_nDeckBeamCZoneSpliceWayBelowDia = 0;
	m_nDeckBeamCZoneSpliceLengthBelowDia = 300;

	m_nDeckBeamSpliceTypeExcessDia = 0;
	m_nDeckBeamSpliceWayExcessDia = 0;
	m_nDeckBeamCZoneSpliceWayExcessDia = 0;
	m_nDeckBeamCZoneSpliceLengthExcessDia = 300;

	m_bDeckBeamSpliceAddLengthSet = TRUE;
	m_nDeckBeamSpliceAddLengthNumSplice = 3;

	m_nDeckBeamSidePressureBarDiaIndex = 0;
	m_nDeckBeamSidePressureBarNum = 4;
	m_nDeckBeamSidePressureBarSpacing = 500;
}

long DeckBeamRebarTemplate::GetDeckBeamSpliceType(double dDia)
{ 
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nDeckBeamPlaceTypeDiaIndex)
		return GetDeckBeamSpliceTypeBelowDia();
	else
		return GetDeckBeamSpliceTypeExcessDia();
}
long DeckBeamRebarTemplate::GetDeckBeamSpliceWay(double dDia)
{ 
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nDeckBeamPlaceTypeDiaIndex)
		return GetDeckBeamSpliceWayBelowDia();
	else
		return GetDeckBeamSpliceWayExcessDia();
}
long DeckBeamRebarTemplate::GetDeckBeamCZoneSpliceWay(double dDia)
{ 
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nDeckBeamPlaceTypeDiaIndex)
		return GetDeckBeamCZoneSpliceWayBelowDia();
	else
		return GetDeckBeamCZoneSpliceWayExcessDia();
}
long DeckBeamRebarTemplate::GetDeckBeamCZoneSpliceLength(double dDia)
{ 
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nDeckBeamPlaceTypeDiaIndex)
		return GetDeckBeamCZoneSpliceLengthBelowDia();
	else
		return GetDeckBeamCZoneSpliceLengthExcessDia();
}