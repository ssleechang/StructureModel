#include "StdAfx.h"
#include "MSObject.h"
#include "PitRebarTemplate.h"
#include "MSDeformedBar.h"
#include "..\GMLib\MSVersionInfo.h"

PitRebarTemplate::PitRebarTemplate() :BaseRebarTemplate()
{
	DataInit();
}

PitRebarTemplate::~PitRebarTemplate(void)
{
}

void PitRebarTemplate::CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate)
{
	PitRebarTemplate* pPitRebarTemplate = dynamic_cast<PitRebarTemplate*>(pBaseRebarTemplate);
	if (pPitRebarTemplate == nullptr) return;

	m_nPitMaxLen2HookAnchorBar = pPitRebarTemplate->m_nPitMaxLen2HookAnchorBar;

	m_bPitSpliceDevConditionSet = pPitRebarTemplate->m_bPitSpliceDevConditionSet;
	m_nPitSpliceDevConditionLength = pPitRebarTemplate->m_nPitSpliceDevConditionLength;

	m_nPitVerOutTopHookType = pPitRebarTemplate->m_nPitVerOutTopHookType;
	m_nPitVerOutBotHookType = pPitRebarTemplate->m_nPitVerOutBotHookType;
	m_nPitVerInTopHookType = pPitRebarTemplate->m_nPitVerInTopHookType;
	m_nPitVerInBotHookType = pPitRebarTemplate->m_nPitVerInBotHookType;

	m_nPitHorOutHookType = pPitRebarTemplate->m_nPitHorOutHookType;
	m_nPitHorInHookType = pPitRebarTemplate->m_nPitHorInHookType;

	m_bPitTopSpacerSet = pPitRebarTemplate->m_bPitTopSpacerSet;
	m_nPitTopSpacerDiaType = pPitRebarTemplate->m_nPitTopSpacerDiaType;
	m_nPitTopSpacerSlabThick = pPitRebarTemplate->m_nPitTopSpacerSlabThick;
	m_nPitTopSpacerSlabThickOverDiaIndex = pPitRebarTemplate->m_nPitTopSpacerSlabThickOverDiaIndex;
	m_nPitTopSpacerSlabThickUnderDiaIndex = pPitRebarTemplate->m_nPitTopSpacerSlabThickUnderDiaIndex;
	m_nPitTopSpacerSpacingType = pPitRebarTemplate->m_nPitTopSpacerSpacingType;
	m_nPitTopSpacerSlabSpacing = pPitRebarTemplate->m_nPitTopSpacerSlabSpacing;
	m_nPitTopSpacerUserSpacing1 = pPitRebarTemplate->m_nPitTopSpacerUserSpacing1;
	m_nPitTopSpacerUserSpacing2 = pPitRebarTemplate->m_nPitTopSpacerUserSpacing2;
	m_nPitTopSpacerHeightType = pPitRebarTemplate->m_nPitTopSpacerHeightType;
	m_nPitTopSpacerUserHeight = pPitRebarTemplate->m_nPitTopSpacerUserHeight;
	m_nPitTopSpacerHookLenAType = pPitRebarTemplate->m_nPitTopSpacerHookLenAType;
	m_nPitTopSpacerMainBarHookLenA = pPitRebarTemplate->m_nPitTopSpacerMainBarHookLenA;
	m_nPitTopSpacerUserHookLenA = pPitRebarTemplate->m_nPitTopSpacerUserHookLenA;
	m_nPitTopSpacerHookLenBType = pPitRebarTemplate->m_nPitTopSpacerHookLenBType;
	m_nPitTopSpacerMainBarHookLenB = pPitRebarTemplate->m_nPitTopSpacerMainBarHookLenB;
	m_nPitTopSpacerUserHookLenB = pPitRebarTemplate->m_nPitTopSpacerUserHookLenB;
	m_nPitTopSpacerWidth = pPitRebarTemplate->m_nPitTopSpacerWidth;

	m_bPitBotSpacerSet = pPitRebarTemplate->m_bPitBotSpacerSet;
	m_nPitBotSpacerDiaType = pPitRebarTemplate->m_nPitBotSpacerDiaType;
	m_nPitBotSpacerDiaIndex = pPitRebarTemplate->m_nPitBotSpacerDiaIndex;
	m_nPitBotSpacerSpacingType = pPitRebarTemplate->m_nPitBotSpacerSpacingType;
	m_nPitBotSpacerSlabSpacing = pPitRebarTemplate->m_nPitBotSpacerSlabSpacing;
	m_nPitBotSpacerUserSpacing1 = pPitRebarTemplate->m_nPitBotSpacerUserSpacing1;
	m_nPitBotSpacerUserSpacing2 = pPitRebarTemplate->m_nPitBotSpacerUserSpacing2;
	m_nPitBotSpacerHeightType = pPitRebarTemplate->m_nPitBotSpacerHeightType;
	m_nPitBotSpacerUserHeight = pPitRebarTemplate->m_nPitBotSpacerUserHeight;
	m_nPitBotSpacerHookLenAType = pPitRebarTemplate->m_nPitBotSpacerHookLenAType;
	m_nPitBotSpacerMainBarHookLenA = pPitRebarTemplate->m_nPitBotSpacerMainBarHookLenA;
	m_nPitBotSpacerUserHookLenA = pPitRebarTemplate->m_nPitBotSpacerUserHookLenA;
	m_nPitBotSpacerHookLenBType = pPitRebarTemplate->m_nPitBotSpacerHookLenBType;
	m_nPitBotSpacerMainBarHookLenB = pPitRebarTemplate->m_nPitBotSpacerMainBarHookLenB;
	m_nPitBotSpacerUserHookLenB = pPitRebarTemplate->m_nPitBotSpacerUserHookLenB;
	m_nPitBotSpacerWidth = pPitRebarTemplate->m_nPitBotSpacerWidth;

	m_bPitSpacerSupportBarsSet = pPitRebarTemplate->m_bPitSpacerSupportBarsSet;
	m_bPitSpacerSupportBarsConditionSet = pPitRebarTemplate->m_bPitSpacerSupportBarsConditionSet;
	m_nPitSpacerSupportBarsCondition = pPitRebarTemplate->m_nPitSpacerSupportBarsCondition;
	m_nPitSpacerSupportBarsDiaType = pPitRebarTemplate->m_nPitSpacerSupportBarsDiaType;
	m_nPitSpacerSupportBarsUserDiaIndex = pPitRebarTemplate->m_nPitSpacerSupportBarsUserDiaIndex;
	m_nPitSpacerSupportBarsNumType = pPitRebarTemplate->m_nPitSpacerSupportBarsNumType;
	m_dPitSpacerSupportBarsTopSpacerNums = pPitRebarTemplate->m_dPitSpacerSupportBarsTopSpacerNums;
	m_nPitSpacerSupportBarsUserNums = pPitRebarTemplate->m_nPitSpacerSupportBarsUserNums;
	m_nPitSpacerSupportBarsLengthType = pPitRebarTemplate->m_nPitSpacerSupportBarsLengthType;
	m_nPitSpacerSupportBarsUserLength = pPitRebarTemplate->m_nPitSpacerSupportBarsUserLength;

	m_bPitSupportRebarsSet = pPitRebarTemplate->m_bPitSupportRebarsSet;
	m_nPitSupportRebarsDiaIndex = pPitRebarTemplate->m_nPitSupportRebarsDiaIndex;
	m_dPitSupportRebarsSlabLengthRate = pPitRebarTemplate->m_dPitSupportRebarsSlabLengthRate;
	m_nPitSupportRebarsOrderLength = pPitRebarTemplate->m_nPitSupportRebarsOrderLength;
	m_nPitSupportRebarsSpliceLength = pPitRebarTemplate->m_nPitSupportRebarsSpliceLength;
	m_nPitSupportRebarsCorrectiveLength = pPitRebarTemplate->m_nPitSupportRebarsCorrectiveLength;

	m_nPitHorOutHookLengthType = pPitRebarTemplate->m_nPitHorOutHookLengthType;
	UserDowelHookLenMapDataCopy(mm_PitUserOutHorBarHookLen, pPitRebarTemplate->mm_PitUserOutHorBarHookLen);
	m_dPitSpacerSupportBarsUserRatio = pPitRebarTemplate->m_dPitSpacerSupportBarsUserRatio;

	m_bPitBotSpacerUserSteelGradeS = pPitRebarTemplate->GetPitBotSpacerUserSteelGradeS();
	m_bPitTopSpacerUserSteelGradeS = pPitRebarTemplate->GetPitTopSpacerUserSteelGradeS();
	m_bPitSpacerSupportBarUserSteelGradeS = pPitRebarTemplate->GetPitSpacerSupportBarUserSteelGradeS();
	m_bPitSupportRebarUserSteelGradeS = pPitRebarTemplate->GetPitSupportRebarUserSteelGradeS();

	m_bPitWellAddBarSet = pPitRebarTemplate->GetPitWellAddBarSet();
	m_nPitWellAddBarDiaIndex = pPitRebarTemplate->GetPitWellAddBarDiaIndex();
	m_bPitWellAddBarUserSteelGradeS = pPitRebarTemplate->GetPitWellAddBarUserSteelGradeS();
	m_nPitWellAddBarNum = pPitRebarTemplate->GetPitWellAddBarNum();
	m_nPitWellAddBarDevType = pPitRebarTemplate->GetPitWellAddBarDevType();
	m_nPitWellAddBarDevUserLength = pPitRebarTemplate->GetPitWellAddBarDevUserLength();

	m_bPitWallSpacerInfoSet = pPitRebarTemplate->GetPitWallSpacerInfoSet();
	m_nPitWallSpacerDiaType = pPitRebarTemplate->GetPitWallSpacerDiaType();
	m_nPitWallSpacerDiaIndex = pPitRebarTemplate->GetPitWallSpacerDiaIndex();
	m_bPitWallSpacerUserSteelGradeS = pPitRebarTemplate->GetPitWallSpacerUserSteelGradeS();
	m_nPitWallSpacerHorSpacing = pPitRebarTemplate->GetPitWallSpacerHorSpacing();
	m_nPitWallSpacerVerSpacingType = pPitRebarTemplate->GetPitWallSpacerVerSpacingType();
	m_nPitWallSpacerVerSpacing = pPitRebarTemplate->GetPitWallSpacerVerSpacing();
	m_nPitWallSpacerVerSpacingRate = pPitRebarTemplate->GetPitWallSpacerVerSpacingRate();
	m_nPitWallSpacerVerSpacingCount = pPitRebarTemplate->GetPitWallSpacerVerSpacingCount();
	m_nPitWallSpacerLenType = pPitRebarTemplate->GetPitWallSpacerLenType();
	m_nPitWallSpacerLenCalWayA = pPitRebarTemplate->GetPitWallSpacerLenCalWayA();
	m_nPitWallSpacerLenCalWayB = pPitRebarTemplate->GetPitWallSpacerLenCalWayB();
	m_nPitWallSpacerLenA = pPitRebarTemplate->GetPitWallSpacerLenA();
	m_nPitWallSpacerLenB = pPitRebarTemplate->GetPitWallSpacerLenB();
	m_nPitWallSpacerShape = pPitRebarTemplate->GetPitWallSpacerShape();

	m_bPitTopSpacerPlaceConditionSet = pPitRebarTemplate->GetPitTopSpacerPlaceConditionSet();
	m_nPitTopSpacerPlaceCondition = pPitRebarTemplate->GetPitTopSpacerPlaceCondition();

	m_bPitPlaceBraceSupportBar = pPitRebarTemplate->GetPitPlaceBraceSupportBar();
	m_nPitBraceSupportBarDiaIndex = pPitRebarTemplate->GetPitBraceSupportBarDiaIndex();
	m_bPitBraceSupportBarUserSteelGradeS = pPitRebarTemplate->GetPitBraceSupportBarUserSteelGradeS();
	m_nPitBraceSupportBarNumIndex = pPitRebarTemplate->GetPitBraceSupportBarNumIndex();

	m_bPitSpliceAddLengthSet = pPitRebarTemplate->GetPitSpliceAddLengthSet();
	m_nPitSpliceAddLengthNumSplice = pPitRebarTemplate->GetPitSpliceAddLengthNumSplice();
}
long PitRebarTemplate::GetPitUserOutHorBarHookLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);

	map<long, long>::iterator it;
	it = mm_PitUserOutHorBarHookLen.find(DiaIndex);
	if (it != mm_PitUserOutHorBarHookLen.end())
		return it->second;

	return 0;
}
void PitRebarTemplate::Serialize(CArchive& ar)
{
	long nDefaultValue = 0;
	long nPitTopSpacerDiaIndex = 0;

	if(ar.IsStoring())
	{
		ar << m_bPitGeneralInfoSet;
		ar << m_nPitMaxLen2HookAnchorBar;

		ar << m_bPitVerInfoSet;
		ar << m_nPitVerOutTopHookType;
		ar << m_nPitVerOutBotHookType;
		ar << m_nPitVerInTopHookType;
		ar << m_nPitVerInBotHookType;

		ar << m_bPitHorInfoSet;
		ar << m_nPitHorOutHookType;
		ar << m_nPitHorInHookType;

		ar << m_bPitTopSpacerInfoSet;
		ar << m_bPitTopSpacerSet;
		ar << m_nPitTopSpacerDiaType;
		ar << nDefaultValue;
		ar << m_nPitTopSpacerSpacingType;
		ar << m_nPitTopSpacerSlabSpacing;
		ar << m_nPitTopSpacerUserSpacing1;
		ar << m_nPitTopSpacerUserSpacing2;
		ar << m_nPitTopSpacerHeightType;
		ar << m_nPitTopSpacerUserHeight;
		ar << m_nPitTopSpacerHookLenAType;
		ar << m_nPitTopSpacerMainBarHookLenA;
		ar << m_nPitTopSpacerUserHookLenA;
		ar << m_nPitTopSpacerHookLenBType;
		ar << m_nPitTopSpacerMainBarHookLenB;
		ar << m_nPitTopSpacerUserHookLenB;
		ar << m_nPitTopSpacerWidth;

		ar << m_bPitBotSpacerInfoSet;
		ar << m_bPitBotSpacerSet;
		ar << m_nPitBotSpacerDiaType;
		ar << m_nPitBotSpacerDiaIndex;
		ar << m_nPitBotSpacerSpacingType;
		ar << m_nPitBotSpacerSlabSpacing;
		ar << m_nPitBotSpacerUserSpacing1;
		ar << m_nPitBotSpacerUserSpacing2;
		ar << m_nPitBotSpacerHeightType;
		ar << m_nPitBotSpacerUserHeight;
		ar << m_nPitBotSpacerHookLenAType;
		ar << m_nPitBotSpacerMainBarHookLenA;
		ar << m_nPitBotSpacerUserHookLenA;
		ar << m_nPitBotSpacerHookLenBType;
		ar << m_nPitBotSpacerMainBarHookLenB;
		ar << m_nPitBotSpacerUserHookLenB;
		ar << m_nPitBotSpacerWidth;

		ar << m_bPitSpacerSupportBarsSet;
		ar << m_bPitSpacerSupportBarsConditionSet;
		ar << m_nPitSpacerSupportBarsCondition;
		ar << m_nPitSpacerSupportBarsDiaType;
		ar << m_nPitSpacerSupportBarsUserDiaIndex;
		ar << m_nPitSpacerSupportBarsNumType;
		ar << m_dPitSpacerSupportBarsTopSpacerNums;
		ar << m_nPitSpacerSupportBarsUserNums;
		ar << m_nPitSpacerSupportBarsLengthType;
		ar << m_nPitSpacerSupportBarsUserLength;

		ar << m_nPitTopSpacerSlabThick;
		ar << m_nPitTopSpacerSlabThickOverDiaIndex;
		ar << m_nPitTopSpacerSlabThickUnderDiaIndex;

		ar << m_bPitSpliceDevConditionSet;
		ar << m_nPitSpliceDevConditionLength;

		ar << m_nPitHorOutHookLengthType;
		map<long, long>::iterator it;
		ar << (int)mm_PitUserOutHorBarHookLen.size();
		for (it = mm_PitUserOutHorBarHookLen.begin(); it != mm_PitUserOutHorBarHookLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long spliceLen = it->second;
			ar << nDiaIndex;
			ar << spliceLen;
		}

		ar << m_dPitSpacerSupportBarsUserRatio;

		ar << m_bPitSupportRebarsSet;
		ar << m_nPitSupportRebarsDiaIndex;
		ar << m_dPitSupportRebarsSlabLengthRate;
		ar << m_nPitSupportRebarsOrderLength;
		ar << m_nPitSupportRebarsSpliceLength;
		ar << m_nPitSupportRebarsCorrectiveLength;

		ar << m_bPitBotSpacerUserSteelGradeS;
		ar << m_bPitTopSpacerUserSteelGradeS;
		ar << m_bPitSpacerSupportBarUserSteelGradeS;
		ar << m_bPitSupportRebarUserSteelGradeS;

		ar << m_bPitWellAddBarSet;
		ar << m_nPitWellAddBarDiaIndex;
		ar << m_bPitWellAddBarUserSteelGradeS;
		ar << m_nPitWellAddBarNum;
		ar << m_nPitWellAddBarDevType;
		ar << m_nPitWellAddBarDevUserLength;

		ar << m_bPitWallSpacerInfoSet;
		ar << m_nPitWallSpacerDiaType;
		ar << m_nPitWallSpacerDiaIndex;
		ar << m_bPitWallSpacerUserSteelGradeS;
		ar << m_nPitWallSpacerHorSpacing;
		ar << m_nPitWallSpacerVerSpacingType;
		ar << m_nPitWallSpacerVerSpacing;
		ar << m_nPitWallSpacerVerSpacingRate;
		ar << m_nPitWallSpacerVerSpacingCount;
		ar << m_nPitWallSpacerLenType;
		ar << m_nPitWallSpacerLenCalWayA;
		ar << m_nPitWallSpacerLenCalWayB;
		ar << m_nPitWallSpacerLenA;
		ar << m_nPitWallSpacerLenB;
		ar << m_nPitWallSpacerShape;

		ar << m_bPitTopSpacerPlaceConditionSet;
		ar << m_nPitTopSpacerPlaceCondition;

		ar << m_bPitPlaceBraceSupportBar;
		ar << m_nPitBraceSupportBarDiaIndex;
		ar << m_bPitBraceSupportBarUserSteelGradeS ;
		ar << m_nPitBraceSupportBarNumIndex;

		ar << m_bPitSpliceAddLengthSet;
		ar << m_nPitSpliceAddLengthNumSplice;
	}
	else
	{
		ar >> m_bPitGeneralInfoSet;
		ar >> m_nPitMaxLen2HookAnchorBar;

		ar >> m_bPitVerInfoSet;
		ar >> m_nPitVerOutTopHookType;
		ar >> m_nPitVerOutBotHookType;
		ar >> m_nPitVerInTopHookType;
		ar >> m_nPitVerInBotHookType;

		ar >> m_bPitHorInfoSet;
		ar >> m_nPitHorOutHookType;
		ar >> m_nPitHorInHookType;

		if (MSVersionInfo::GetCurrentVersion() >= 20170621)
		{
			ar >> m_bPitTopSpacerInfoSet;
			ar >> m_bPitTopSpacerSet;
			ar >> m_nPitTopSpacerDiaType;
			ar >> nPitTopSpacerDiaIndex;
			ar >> m_nPitTopSpacerSpacingType;
			ar >> m_nPitTopSpacerSlabSpacing;
			ar >> m_nPitTopSpacerUserSpacing1;
			ar >> m_nPitTopSpacerUserSpacing2;
			ar >> m_nPitTopSpacerHeightType;
			ar >> m_nPitTopSpacerUserHeight;
			ar >> m_nPitTopSpacerHookLenAType;
			ar >> m_nPitTopSpacerMainBarHookLenA;
			ar >> m_nPitTopSpacerUserHookLenA;
			ar >> m_nPitTopSpacerHookLenBType;
			ar >> m_nPitTopSpacerMainBarHookLenB;
			ar >> m_nPitTopSpacerUserHookLenB;
			ar >> m_nPitTopSpacerWidth;

			ar >> m_bPitBotSpacerInfoSet;
			ar >> m_bPitBotSpacerSet;
			ar >> m_nPitBotSpacerDiaType;
			ar >> m_nPitBotSpacerDiaIndex;
			ar >> m_nPitBotSpacerSpacingType;
			ar >> m_nPitBotSpacerSlabSpacing;
			ar >> m_nPitBotSpacerUserSpacing1;
			ar >> m_nPitBotSpacerUserSpacing2;
			ar >> m_nPitBotSpacerHeightType;
			ar >> m_nPitBotSpacerUserHeight;
			ar >> m_nPitBotSpacerHookLenAType;
			ar >> m_nPitBotSpacerMainBarHookLenA;
			ar >> m_nPitBotSpacerUserHookLenA;
			ar >> m_nPitBotSpacerHookLenBType;
			ar >> m_nPitBotSpacerMainBarHookLenB;
			ar >> m_nPitBotSpacerUserHookLenB;
			ar >> m_nPitBotSpacerWidth;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20170822)
		{
			ar >> m_bPitSpacerSupportBarsSet;
			ar >> m_bPitSpacerSupportBarsConditionSet;
			ar >> m_nPitSpacerSupportBarsCondition;
			ar >> m_nPitSpacerSupportBarsDiaType;
			ar >> m_nPitSpacerSupportBarsUserDiaIndex;
			ar >> m_nPitSpacerSupportBarsNumType;
			ar >> m_dPitSpacerSupportBarsTopSpacerNums;
			ar >> m_nPitSpacerSupportBarsUserNums;
			ar >> m_nPitSpacerSupportBarsLengthType;
			ar >> m_nPitSpacerSupportBarsUserLength;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20180717)
		{
			ar >> m_nPitTopSpacerSlabThick;
			ar >> m_nPitTopSpacerSlabThickOverDiaIndex;
			ar >> m_nPitTopSpacerSlabThickUnderDiaIndex;
		}
		else
		{
			if (m_nPitTopSpacerDiaType == 1)
				m_nPitTopSpacerSlabThickOverDiaIndex = nPitTopSpacerDiaIndex;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20190423)
		{
			ar >> m_bPitSpliceDevConditionSet;
			ar >> m_nPitSpliceDevConditionLength;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20191002)
		{
			ar >> m_nPitHorOutHookLengthType;
			int NumObject;

			ar >> NumObject;
			if (NumObject > 0)
				mm_PitUserOutHorBarHookLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, spliceLen = 0;

				ar >> nDiaIndex;
				ar >> spliceLen;
				mm_PitUserOutHorBarHookLen.insert(make_pair(nDiaIndex, spliceLen));
			}
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20191007)
		{
			ar >> m_dPitSpacerSupportBarsUserRatio;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20230419)
		{
			ar >> m_bPitSupportRebarsSet;
			ar >> m_nPitSupportRebarsDiaIndex;
			ar >> m_dPitSupportRebarsSlabLengthRate;
			ar >> m_nPitSupportRebarsOrderLength;
			ar >> m_nPitSupportRebarsSpliceLength;
			ar >> m_nPitSupportRebarsCorrectiveLength;
		}

		if (MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231010)
		{
			ar >> m_bPitBotSpacerUserSteelGradeS;
			ar >> m_bPitTopSpacerUserSteelGradeS;
			ar >> m_bPitSpacerSupportBarUserSteelGradeS;
			ar >> m_bPitSupportRebarUserSteelGradeS;
		}

		if((MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231227) || MSVersionInfo::IsOldTotalVersion())
		{
			ar >> m_bPitWellAddBarSet;
			ar >> m_nPitWellAddBarDiaIndex;
			ar >> m_bPitWellAddBarUserSteelGradeS;
			ar >> m_nPitWellAddBarNum;
			ar >> m_nPitWellAddBarDevType;
			ar >> m_nPitWellAddBarDevUserLength;

			ar >> m_bPitWallSpacerInfoSet;
			ar >> m_nPitWallSpacerDiaType;
			ar >> m_nPitWallSpacerDiaIndex;
			ar >> m_bPitWallSpacerUserSteelGradeS;
			ar >> m_nPitWallSpacerHorSpacing;
			ar >> m_nPitWallSpacerVerSpacingType;
			ar >> m_nPitWallSpacerVerSpacing;
			ar >> m_nPitWallSpacerVerSpacingRate;
			ar >> m_nPitWallSpacerVerSpacingCount;
			ar >> m_nPitWallSpacerLenType;
			ar >> m_nPitWallSpacerLenCalWayA;
			ar >> m_nPitWallSpacerLenCalWayB;
			ar >> m_nPitWallSpacerLenA;
			ar >> m_nPitWallSpacerLenB;
			ar >> m_nPitWallSpacerShape;
		}
		if(MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20240306)
		{
			ar >> m_bPitTopSpacerPlaceConditionSet;
			ar >> m_nPitTopSpacerPlaceCondition;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20241018)
		{
			ar >> m_bPitPlaceBraceSupportBar;
			ar >> m_nPitBraceSupportBarDiaIndex;
			ar >> m_bPitBraceSupportBarUserSteelGradeS ;
			ar >> m_nPitBraceSupportBarNumIndex;

			ar >> m_bPitSpliceAddLengthSet;
			ar >> m_nPitSpliceAddLengthNumSplice;
		}
		else
		{
			m_bPitSpliceAddLengthSet = FALSE;
		}
	}
}

void PitRebarTemplate::DataInit()
{
	m_bPitGeneralInfoSet = false;
	m_nPitMaxLen2HookAnchorBar = 6000;

	m_bPitSpliceDevConditionSet = false;
	m_nPitSpliceDevConditionLength = 340;

	m_bPitVerInfoSet = false;
	m_nPitVerOutTopHookType = 1;
	m_nPitVerOutBotHookType = 1;
	m_nPitVerInTopHookType = 1;
	m_nPitVerInBotHookType = 0;

	m_bPitHorInfoSet = false;
	m_nPitHorOutHookType = 0;
	m_nPitHorInHookType = 0;

	m_bPitTopSpacerInfoSet = false;
	m_bPitTopSpacerSet = false;
	m_nPitTopSpacerDiaType = 0;
	m_nPitTopSpacerSlabThick = 700;
	m_nPitTopSpacerSlabThickOverDiaIndex = 2;
	m_nPitTopSpacerSlabThickUnderDiaIndex = 1;
	m_nPitTopSpacerSpacingType = 1;
	m_nPitTopSpacerSlabSpacing = 3;
	m_nPitTopSpacerUserSpacing1 = 1500;
	m_nPitTopSpacerUserSpacing2 = 1500;
	m_nPitTopSpacerHeightType = 0;
	m_nPitTopSpacerUserHeight = 300;
	m_nPitTopSpacerHookLenAType = 1;
	m_nPitTopSpacerMainBarHookLenA = 100;
	m_nPitTopSpacerUserHookLenA = 100;
	m_nPitTopSpacerHookLenBType = 1;
	m_nPitTopSpacerMainBarHookLenB = 0;
	m_nPitTopSpacerUserHookLenB = 100;
	m_nPitTopSpacerWidth = 200;

	m_bPitBotSpacerInfoSet = false;
	m_bPitBotSpacerSet = false;
	m_nPitBotSpacerDiaType = 0;
	m_nPitBotSpacerDiaIndex = 0;
	m_nPitBotSpacerSpacingType = 1;
	m_nPitBotSpacerSlabSpacing = 3;
	m_nPitBotSpacerUserSpacing1 = 1500;
	m_nPitBotSpacerUserSpacing2 = 1500;
	m_nPitBotSpacerHeightType = 0;
	m_nPitBotSpacerUserHeight = 300;
	m_nPitBotSpacerHookLenAType = 1;
	m_nPitBotSpacerMainBarHookLenA = 100;
	m_nPitBotSpacerUserHookLenA = 100;
	m_nPitBotSpacerHookLenBType = 1;
	m_nPitBotSpacerMainBarHookLenB = 0;
	m_nPitBotSpacerUserHookLenB = 100;
	m_nPitBotSpacerWidth = 200;

	m_bPitSpacerSupportBarsSet = false;
	m_bPitSpacerSupportBarsConditionSet = true;
	m_nPitSpacerSupportBarsCondition = 1000;
	m_nPitSpacerSupportBarsDiaType = 1;
	m_nPitSpacerSupportBarsUserDiaIndex = 0;
	m_nPitSpacerSupportBarsNumType = 0;
	m_dPitSpacerSupportBarsTopSpacerNums = 0.5;
	m_nPitSpacerSupportBarsUserNums = 10;
	m_nPitSpacerSupportBarsLengthType = 1;
	m_dPitSpacerSupportBarsUserRatio = 1.4;
	m_nPitSpacerSupportBarsUserLength = 1600;

	m_bPitSupportRebarsSet = false;
	m_nPitSupportRebarsDiaIndex = 0;
	m_dPitSupportRebarsSlabLengthRate = 1;
	m_nPitSupportRebarsOrderLength = 8000;
	m_nPitSupportRebarsSpliceLength = 300;
	m_nPitSupportRebarsCorrectiveLength = 10;

	m_nPitHorOutHookLengthType = 0;
	InitUserDowelHookLenMap(mm_PitUserOutHorBarHookLen);

	m_bPitBotSpacerUserSteelGradeS = false;
	m_bPitTopSpacerUserSteelGradeS = false;
	m_bPitSpacerSupportBarUserSteelGradeS = false;
	m_bPitSupportRebarUserSteelGradeS = false;

	m_bPitWellAddBarSet = FALSE;
	m_nPitWellAddBarDiaIndex = 2;
	m_bPitWellAddBarUserSteelGradeS = FALSE;
	m_nPitWellAddBarNum = 2;

	m_nPitWellAddBarDevType = 0;
	m_nPitWellAddBarDevUserLength = 0;

	m_bPitWallSpacerInfoSet = FALSE;
	m_nPitWallSpacerDiaType = 0;
	m_nPitWallSpacerDiaIndex = 0;
	m_bPitWallSpacerUserSteelGradeS = FALSE;
	m_nPitWallSpacerHorSpacing = 1500;
	m_nPitWallSpacerVerSpacingType = 0;
	m_nPitWallSpacerVerSpacing = 1500;
	m_nPitWallSpacerVerSpacingRate = 3;
	m_nPitWallSpacerVerSpacingCount = 3;
	m_nPitWallSpacerLenType = 0;
	m_nPitWallSpacerLenCalWayA = 0;
	m_nPitWallSpacerLenCalWayB = 0;
	m_nPitWallSpacerLenA = 350;
	m_nPitWallSpacerLenB = 350;
	m_nPitWallSpacerShape = 0;

	m_bPitTopSpacerPlaceConditionSet = FALSE;
	m_nPitTopSpacerPlaceCondition = 0;

	m_bPitPlaceBraceSupportBar = FALSE;
	m_nPitBraceSupportBarDiaIndex = 1;
	m_bPitBraceSupportBarUserSteelGradeS = FALSE;
	m_nPitBraceSupportBarNumIndex = 0;	

	m_bPitSpliceAddLengthSet = TRUE;
	m_nPitSpliceAddLengthNumSplice = 3;
}


