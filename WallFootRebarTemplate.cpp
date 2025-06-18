#include "StdAfx.h"
#include "MSObject.h"
#include "WallFootRebarTemplate.h"
#include "MSDeformedBar.h"
#include "..\GMLib\MSVersionInfo.h"

WallFootRebarTemplate::WallFootRebarTemplate() :BaseRebarTemplate()
{
	DataInit();
	m_pSupportRebarsTemplate = new SlabHorSupportRebarsTemplate();
	m_pSupportRebarsTemplate->SetSupportRebarsLengthType(1);
}

WallFootRebarTemplate::~WallFootRebarTemplate(void)
{
	delete m_pSupportRebarsTemplate;
}

void WallFootRebarTemplate::CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate)
{
	WallFootRebarTemplate* pWallFootRebarTemplate = dynamic_cast<WallFootRebarTemplate*>(pBaseRebarTemplate);
	if (pWallFootRebarTemplate == nullptr) return;
	
	m_pSupportRebarsTemplate->CopyFromMe(pWallFootRebarTemplate->GetWallFootSupportRebarsTemplate());

	m_bWallFootSpliceDevConditionSet = pWallFootRebarTemplate->m_bWallFootSpliceDevConditionSet;
	m_nWallFootSpliceDevConditionLength = pWallFootRebarTemplate->m_nWallFootSpliceDevConditionLength;

	m_nWallFootDevType = pWallFootRebarTemplate->m_nWallFootDevType;

	m_bWallFootTopSpacerSet = pWallFootRebarTemplate->m_bWallFootTopSpacerSet;
	m_nWallFootTopSpacerDiaType = pWallFootRebarTemplate->m_nWallFootTopSpacerDiaType;
	m_nWallFootTopSpacerSlabThick = pWallFootRebarTemplate->m_nWallFootTopSpacerSlabThick;
	m_nWallFootTopSpacerSlabThickOverDiaIndex = pWallFootRebarTemplate->m_nWallFootTopSpacerSlabThickOverDiaIndex;
	m_nWallFootTopSpacerSlabThickUnderDiaIndex = pWallFootRebarTemplate->m_nWallFootTopSpacerSlabThickUnderDiaIndex;
	m_nWallFootTopSpacerSpacingType = pWallFootRebarTemplate->m_nWallFootTopSpacerSpacingType;
	m_nWallFootTopSpacerSlabSpacing = pWallFootRebarTemplate->m_nWallFootTopSpacerSlabSpacing;
	m_nWallFootTopSpacerUserSpacing1 = pWallFootRebarTemplate->m_nWallFootTopSpacerUserSpacing1;
	m_nWallFootTopSpacerUserSpacing2 = pWallFootRebarTemplate->m_nWallFootTopSpacerUserSpacing2;
	m_nWallFootTopSpacerHeightType = pWallFootRebarTemplate->m_nWallFootTopSpacerHeightType;
	m_nWallFootTopSpacerUserHeight = pWallFootRebarTemplate->m_nWallFootTopSpacerUserHeight;
	m_nWallFootTopSpacerHookLenAType = pWallFootRebarTemplate->m_nWallFootTopSpacerHookLenAType;
	m_nWallFootTopSpacerMainBarHookLenA = pWallFootRebarTemplate->m_nWallFootTopSpacerMainBarHookLenA;
	m_nWallFootTopSpacerUserHookLenA = pWallFootRebarTemplate->m_nWallFootTopSpacerUserHookLenA;
	m_nWallFootTopSpacerHookLenBType = pWallFootRebarTemplate->m_nWallFootTopSpacerHookLenBType;
	m_nWallFootTopSpacerMainBarHookLenB = pWallFootRebarTemplate->m_nWallFootTopSpacerMainBarHookLenB;
	m_nWallFootTopSpacerUserHookLenB = pWallFootRebarTemplate->m_nWallFootTopSpacerUserHookLenB;
	m_nWallFootTopSpacerWidth = pWallFootRebarTemplate->m_nWallFootTopSpacerWidth;

	m_bWallFootBotSpacerSet = pWallFootRebarTemplate->m_bWallFootBotSpacerSet;
	m_nWallFootBotSpacerDiaType = pWallFootRebarTemplate->m_nWallFootBotSpacerDiaType;
	m_nWallFootBotSpacerDiaIndex = pWallFootRebarTemplate->m_nWallFootBotSpacerDiaIndex;
	m_nWallFootBotSpacerSpacingType = pWallFootRebarTemplate->m_nWallFootBotSpacerSpacingType;
	m_nWallFootBotSpacerSlabSpacing = pWallFootRebarTemplate->m_nWallFootBotSpacerSlabSpacing;
	m_nWallFootBotSpacerUserSpacing1 = pWallFootRebarTemplate->m_nWallFootBotSpacerUserSpacing1;
	m_nWallFootBotSpacerUserSpacing2 = pWallFootRebarTemplate->m_nWallFootBotSpacerUserSpacing2;
	m_nWallFootBotSpacerHeightType = pWallFootRebarTemplate->m_nWallFootBotSpacerHeightType;
	m_nWallFootBotSpacerUserHeight = pWallFootRebarTemplate->m_nWallFootBotSpacerUserHeight;
	m_nWallFootBotSpacerHookLenAType = pWallFootRebarTemplate->m_nWallFootBotSpacerHookLenAType;
	m_nWallFootBotSpacerMainBarHookLenA = pWallFootRebarTemplate->m_nWallFootBotSpacerMainBarHookLenA;
	m_nWallFootBotSpacerUserHookLenA = pWallFootRebarTemplate->m_nWallFootBotSpacerUserHookLenA;
	m_nWallFootBotSpacerHookLenBType = pWallFootRebarTemplate->m_nWallFootBotSpacerHookLenBType;
	m_nWallFootBotSpacerMainBarHookLenB = pWallFootRebarTemplate->m_nWallFootBotSpacerMainBarHookLenB;
	m_nWallFootBotSpacerUserHookLenB = pWallFootRebarTemplate->m_nWallFootBotSpacerUserHookLenB;
	m_nWallFootBotSpacerWidth = pWallFootRebarTemplate->m_nWallFootBotSpacerWidth;

	m_bWallFootSpacerSupportBarsSet = pWallFootRebarTemplate->m_bWallFootSpacerSupportBarsSet;
	m_bWallFootSpacerSupportBarsConditionSet = pWallFootRebarTemplate->m_bWallFootSpacerSupportBarsConditionSet;
	m_nWallFootSpacerSupportBarsCondition = pWallFootRebarTemplate->m_nWallFootSpacerSupportBarsCondition;
	m_nWallFootSpacerSupportBarsDiaType = pWallFootRebarTemplate->m_nWallFootSpacerSupportBarsDiaType;
	m_nWallFootSpacerSupportBarsUserDiaIndex = pWallFootRebarTemplate->m_nWallFootSpacerSupportBarsUserDiaIndex;
	m_nWallFootSpacerSupportBarsNumType = pWallFootRebarTemplate->m_nWallFootSpacerSupportBarsNumType;
	m_dWallFootSpacerSupportBarsTopSpacerNums = pWallFootRebarTemplate->m_dWallFootSpacerSupportBarsTopSpacerNums;
	m_nWallFootSpacerSupportBarsUserNums = pWallFootRebarTemplate->m_nWallFootSpacerSupportBarsUserNums;
	m_nWallFootSpacerSupportBarsLengthType = pWallFootRebarTemplate->m_nWallFootSpacerSupportBarsLengthType;
	m_nWallFootSpacerSupportBarsUserLength = pWallFootRebarTemplate->m_nWallFootSpacerSupportBarsUserLength;
	m_dWallFootSpacerSupportBarsUserRatio = pWallFootRebarTemplate->m_dWallFootSpacerSupportBarsUserRatio;

	m_bWallFootBotSpacerUserSteelGradeS = pWallFootRebarTemplate->GetWallFootBotSpacerUserSteelGradeS();
	m_bWallFootTopSpacerUserSteelGradeS = pWallFootRebarTemplate->GetWallFootTopSpacerUserSteelGradeS();
	m_bWallFootSpacerSupportBarUserSteelGradeS = pWallFootRebarTemplate->GetWallFootSpacerSupportBarUserSteelGradeS();	

	m_bWallFootTopSpacerPlaceConditionSet = pWallFootRebarTemplate->GetWallFootTopSpacerPlaceConditionSet();
	m_nWallFootTopSpacerPlaceCondition = pWallFootRebarTemplate->GetWallFootTopSpacerPlaceCondition();

	m_bWallFootSpliceAddLengthSet = pWallFootRebarTemplate->GetWallFootSpliceAddLengthSet();
	m_nWallFootSpliceAddLengthNumSplice = pWallFootRebarTemplate->GetWallFootSpliceAddLengthNumSplice();
}

void WallFootRebarTemplate::Serialize(CArchive& ar)
{
	long nDefaultValue = 0;
	long nWallFootTopSpacerDiaIndex = 0;

	if(ar.IsStoring())
	{
		ar << m_bWallFootTopSpacerInfoSet;
		ar << m_bWallFootTopSpacerSet;
		ar << m_nWallFootTopSpacerDiaType;
		ar << nDefaultValue;
		ar << m_nWallFootTopSpacerSpacingType;
		ar << m_nWallFootTopSpacerSlabSpacing;
		ar << m_nWallFootTopSpacerUserSpacing1;
		ar << m_nWallFootTopSpacerUserSpacing2;
		ar << m_nWallFootTopSpacerHeightType;
		ar << m_nWallFootTopSpacerUserHeight;
		ar << m_nWallFootTopSpacerHookLenAType;
		ar << m_nWallFootTopSpacerMainBarHookLenA;
		ar << m_nWallFootTopSpacerUserHookLenA;
		ar << m_nWallFootTopSpacerHookLenBType;
		ar << m_nWallFootTopSpacerMainBarHookLenB;
		ar << m_nWallFootTopSpacerUserHookLenB;
		ar << m_nWallFootTopSpacerWidth;

		ar << m_bWallFootBotSpacerInfoSet;
		ar << m_bWallFootBotSpacerSet;
		ar << m_nWallFootBotSpacerDiaType;
		ar << m_nWallFootBotSpacerDiaIndex;
		ar << m_nWallFootBotSpacerSpacingType;
		ar << m_nWallFootBotSpacerSlabSpacing;
		ar << m_nWallFootBotSpacerUserSpacing1;
		ar << m_nWallFootBotSpacerUserSpacing2;
		ar << m_nWallFootBotSpacerHeightType;
		ar << m_nWallFootBotSpacerUserHeight;
		ar << m_nWallFootBotSpacerHookLenAType;
		ar << m_nWallFootBotSpacerMainBarHookLenA;
		ar << m_nWallFootBotSpacerUserHookLenA;
		ar << m_nWallFootBotSpacerHookLenBType;
		ar << m_nWallFootBotSpacerMainBarHookLenB;
		ar << m_nWallFootBotSpacerUserHookLenB;
		ar << m_nWallFootBotSpacerWidth;

		ar << m_bWallFootSpacerSupportBarsSet;
		ar << m_bWallFootSpacerSupportBarsConditionSet;
		ar << m_nWallFootSpacerSupportBarsCondition;
		ar << m_nWallFootSpacerSupportBarsDiaType;
		ar << m_nWallFootSpacerSupportBarsUserDiaIndex;
		ar << m_nWallFootSpacerSupportBarsNumType;
		ar << m_dWallFootSpacerSupportBarsTopSpacerNums;
		ar << m_nWallFootSpacerSupportBarsUserNums;
		ar << m_nWallFootSpacerSupportBarsLengthType;
		ar << m_nWallFootSpacerSupportBarsUserLength;

		ar << m_nWallFootTopSpacerSlabThick;
		ar << m_nWallFootTopSpacerSlabThickOverDiaIndex;
		ar << m_nWallFootTopSpacerSlabThickUnderDiaIndex;

		ar << m_bWallFootSpliceDevConditionSet;
		ar << m_nWallFootSpliceDevConditionLength;

		ar << m_dWallFootSpacerSupportBarsUserRatio;

		ar << m_nWallFootDevType;

		m_pSupportRebarsTemplate->Serialize(ar);

		ar << m_bWallFootBotSpacerUserSteelGradeS;
		ar << m_bWallFootTopSpacerUserSteelGradeS;
		ar << m_bWallFootSpacerSupportBarUserSteelGradeS;

		ar << m_bWallFootTopSpacerPlaceConditionSet;
		ar << m_nWallFootTopSpacerPlaceCondition;

		ar << m_bWallFootSpliceAddLengthSet;
		ar << m_nWallFootSpliceAddLengthNumSplice;
	}
	else
	{
		ar >> m_bWallFootTopSpacerInfoSet;
		ar >> m_bWallFootTopSpacerSet;
		ar >> m_nWallFootTopSpacerDiaType;
		ar >> nWallFootTopSpacerDiaIndex;
		ar >> m_nWallFootTopSpacerSpacingType;
		ar >> m_nWallFootTopSpacerSlabSpacing;
		ar >> m_nWallFootTopSpacerUserSpacing1;
		ar >> m_nWallFootTopSpacerUserSpacing2;
		ar >> m_nWallFootTopSpacerHeightType;
		ar >> m_nWallFootTopSpacerUserHeight;
		ar >> m_nWallFootTopSpacerHookLenAType;
		ar >> m_nWallFootTopSpacerMainBarHookLenA;
		ar >> m_nWallFootTopSpacerUserHookLenA;
		ar >> m_nWallFootTopSpacerHookLenBType;
		ar >> m_nWallFootTopSpacerMainBarHookLenB;
		ar >> m_nWallFootTopSpacerUserHookLenB;
		ar >> m_nWallFootTopSpacerWidth;

		ar >> m_bWallFootBotSpacerInfoSet;
		ar >> m_bWallFootBotSpacerSet;
		ar >> m_nWallFootBotSpacerDiaType;
		ar >> m_nWallFootBotSpacerDiaIndex;
		ar >> m_nWallFootBotSpacerSpacingType;
		ar >> m_nWallFootBotSpacerSlabSpacing;
		ar >> m_nWallFootBotSpacerUserSpacing1;
		ar >> m_nWallFootBotSpacerUserSpacing2;
		ar >> m_nWallFootBotSpacerHeightType;
		ar >> m_nWallFootBotSpacerUserHeight;
		ar >> m_nWallFootBotSpacerHookLenAType;
		ar >> m_nWallFootBotSpacerMainBarHookLenA;
		ar >> m_nWallFootBotSpacerUserHookLenA;
		ar >> m_nWallFootBotSpacerHookLenBType;
		ar >> m_nWallFootBotSpacerMainBarHookLenB;
		ar >> m_nWallFootBotSpacerUserHookLenB;
		ar >> m_nWallFootBotSpacerWidth;

		if (MSVersionInfo::GetCurrentVersion() >= 20170822)
		{
			ar >> m_bWallFootSpacerSupportBarsSet;
			ar >> m_bWallFootSpacerSupportBarsConditionSet;
			ar >> m_nWallFootSpacerSupportBarsCondition;
			ar >> m_nWallFootSpacerSupportBarsDiaType;
			ar >> m_nWallFootSpacerSupportBarsUserDiaIndex;
			ar >> m_nWallFootSpacerSupportBarsNumType;
			ar >> m_dWallFootSpacerSupportBarsTopSpacerNums;
			ar >> m_nWallFootSpacerSupportBarsUserNums;
			ar >> m_nWallFootSpacerSupportBarsLengthType;
			ar >> m_nWallFootSpacerSupportBarsUserLength;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20180717)
		{
			ar >> m_nWallFootTopSpacerSlabThick;
			ar >> m_nWallFootTopSpacerSlabThickOverDiaIndex;
			ar >> m_nWallFootTopSpacerSlabThickUnderDiaIndex;
		}
		else
		{
			if (m_nWallFootTopSpacerDiaType == 1)
				m_nWallFootTopSpacerSlabThickOverDiaIndex = nWallFootTopSpacerDiaIndex;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20190423)
		{
			ar >> m_bWallFootSpliceDevConditionSet;
			ar >> m_nWallFootSpliceDevConditionLength;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20191007)
		{
			ar >> m_dWallFootSpacerSupportBarsUserRatio;
		}
		long nWallFootDevTypeLx = 0, nWallFootDevTypeLy = 0;
		if(MSVersionInfo::GetCurrentVersion() >= 20200109 && MSVersionInfo::GetCurrentVersion() < 20230419)
		{
			ar >> nWallFootDevTypeLx;
			ar >> nWallFootDevTypeLy;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20230419)
		{
			ar >> m_nWallFootDevType;
			m_pSupportRebarsTemplate->Serialize(ar);
		}
		else
		{
			m_nWallFootDevType = nWallFootDevTypeLx;
		}

		if (MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231010)
		{
			ar >> m_bWallFootBotSpacerUserSteelGradeS;
			ar >> m_bWallFootTopSpacerUserSteelGradeS;
			ar >> m_bWallFootSpacerSupportBarUserSteelGradeS;
		}
		if(MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20240306)
		{
			ar >> m_bWallFootTopSpacerPlaceConditionSet;
			ar >> m_nWallFootTopSpacerPlaceCondition;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20241018)
		{
			ar >> m_bWallFootSpliceAddLengthSet;
			ar >> m_nWallFootSpliceAddLengthNumSplice;
		}
		else
		{
			m_bWallFootSpliceAddLengthSet = FALSE;
		}
	}
}

void WallFootRebarTemplate::DataInit()
{
	m_bWallFootSpliceDevConditionSet = false;
	m_nWallFootSpliceDevConditionLength = 340;

	m_bWallFootTopSpacerInfoSet = false;
	m_bWallFootTopSpacerSet = false;
	m_nWallFootTopSpacerDiaType = 0;
	m_nWallFootTopSpacerSlabThick = 700;
	m_nWallFootTopSpacerSlabThickOverDiaIndex = 2;
	m_nWallFootTopSpacerSlabThickUnderDiaIndex = 1;
	m_nWallFootTopSpacerSpacingType = 1;
	m_nWallFootTopSpacerSlabSpacing = 3;
	m_nWallFootTopSpacerUserSpacing1 = 1500;
	m_nWallFootTopSpacerUserSpacing2 = 1500;
	m_nWallFootTopSpacerHeightType = 0;
	m_nWallFootTopSpacerUserHeight = 300;
	m_nWallFootTopSpacerHookLenAType = 1;
	m_nWallFootTopSpacerMainBarHookLenA = 100;
	m_nWallFootTopSpacerUserHookLenA = 100;
	m_nWallFootTopSpacerHookLenBType = 1;
	m_nWallFootTopSpacerMainBarHookLenB = 0;
	m_nWallFootTopSpacerUserHookLenB = 100;
	m_nWallFootTopSpacerWidth = 200;

	m_bWallFootBotSpacerInfoSet = false;
	m_bWallFootBotSpacerSet = false;
	m_nWallFootBotSpacerDiaType = 0;
	m_nWallFootBotSpacerDiaIndex = 0;
	m_nWallFootBotSpacerSpacingType = 1;
	m_nWallFootBotSpacerSlabSpacing = 3;
	m_nWallFootBotSpacerUserSpacing1 = 1500;
	m_nWallFootBotSpacerUserSpacing2 = 1500;
	m_nWallFootBotSpacerHeightType = 0;
	m_nWallFootBotSpacerUserHeight = 300;
	m_nWallFootBotSpacerHookLenAType = 1;
	m_nWallFootBotSpacerMainBarHookLenA = 100;
	m_nWallFootBotSpacerUserHookLenA = 100;
	m_nWallFootBotSpacerHookLenBType = 1;
	m_nWallFootBotSpacerMainBarHookLenB = 0;
	m_nWallFootBotSpacerUserHookLenB = 100;
	m_nWallFootBotSpacerWidth = 200;

	m_bWallFootSpacerSupportBarsSet = false;
	m_bWallFootSpacerSupportBarsConditionSet = true;
	m_nWallFootSpacerSupportBarsCondition = 1000;
	m_nWallFootSpacerSupportBarsDiaType = 1;
	m_nWallFootSpacerSupportBarsUserDiaIndex = 0;
	m_nWallFootSpacerSupportBarsNumType = 0;
	m_dWallFootSpacerSupportBarsTopSpacerNums = 0.5;
	m_nWallFootSpacerSupportBarsUserNums = 10;
	m_nWallFootSpacerSupportBarsLengthType = 1;
	m_nWallFootSpacerSupportBarsUserLength = 1600;

	m_dWallFootSpacerSupportBarsUserRatio = 1.4;

	m_nWallFootDevType = 0;
	
	m_bWallFootBotSpacerUserSteelGradeS = false;
	m_bWallFootTopSpacerUserSteelGradeS = false;
	m_bWallFootSpacerSupportBarUserSteelGradeS = false;

	m_bWallFootTopSpacerPlaceConditionSet = FALSE;
	m_nWallFootTopSpacerPlaceCondition = 0;

	m_bWallFootSpliceAddLengthSet = TRUE;
	m_nWallFootSpliceAddLengthNumSplice = 3;
}

SlabHorSupportRebarsTemplate *WallFootRebarTemplate::GetWallFootSupportRebarsTemplate()
{
	return m_pSupportRebarsTemplate;
}