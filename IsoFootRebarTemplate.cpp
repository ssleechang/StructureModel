#include "StdAfx.h"
#include "MSObject.h"
#include "IsoFootRebarTemplate.h"
#include "MSDeformedBar.h"
#include "..\GMLib\MSVersionInfo.h"

IsoFootRebarTemplate::IsoFootRebarTemplate() :BaseRebarTemplate()
{
	DataInit();
}

IsoFootRebarTemplate::~IsoFootRebarTemplate(void)
{
}

void IsoFootRebarTemplate::CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate)
{
	IsoFootRebarTemplate* pIsoFootRebarTemplate = dynamic_cast<IsoFootRebarTemplate*>(pBaseRebarTemplate);
	if (pIsoFootRebarTemplate == nullptr) return;
	
	m_bIsoFootSpliceDevConditionSet = pIsoFootRebarTemplate->m_bIsoFootSpliceDevConditionSet;
	m_nIsoFootSpliceDevConditionLength = pIsoFootRebarTemplate->m_nIsoFootSpliceDevConditionLength;

	m_nIsoFootDevType = pIsoFootRebarTemplate->m_nIsoFootDevType;	

	m_bIsoFootTopSpacerSet = pIsoFootRebarTemplate->m_bIsoFootTopSpacerSet;
	m_nIsoFootTopSpacerDiaType = pIsoFootRebarTemplate->m_nIsoFootTopSpacerDiaType;
	m_nIsoFootTopSpacerSlabThick = pIsoFootRebarTemplate->m_nIsoFootTopSpacerSlabThick;
	m_nIsoFootTopSpacerSlabThickOverDiaIndex = pIsoFootRebarTemplate->m_nIsoFootTopSpacerSlabThickOverDiaIndex;
	m_nIsoFootTopSpacerSlabThickUnderDiaIndex = pIsoFootRebarTemplate->m_nIsoFootTopSpacerSlabThickUnderDiaIndex;
	m_nIsoFootTopSpacerSpacingType = pIsoFootRebarTemplate->m_nIsoFootTopSpacerSpacingType;
	m_nIsoFootTopSpacerSlabSpacing = pIsoFootRebarTemplate->m_nIsoFootTopSpacerSlabSpacing;
	m_nIsoFootTopSpacerUserSpacing1 = pIsoFootRebarTemplate->m_nIsoFootTopSpacerUserSpacing1;
	m_nIsoFootTopSpacerUserSpacing2 = pIsoFootRebarTemplate->m_nIsoFootTopSpacerUserSpacing2;
	m_nIsoFootTopSpacerHeightType = pIsoFootRebarTemplate->m_nIsoFootTopSpacerHeightType;
	m_nIsoFootTopSpacerUserHeight = pIsoFootRebarTemplate->m_nIsoFootTopSpacerUserHeight;
	m_nIsoFootTopSpacerHookLenAType = pIsoFootRebarTemplate->m_nIsoFootTopSpacerHookLenAType;
	m_nIsoFootTopSpacerMainBarHookLenA = pIsoFootRebarTemplate->m_nIsoFootTopSpacerMainBarHookLenA;
	m_nIsoFootTopSpacerUserHookLenA = pIsoFootRebarTemplate->m_nIsoFootTopSpacerUserHookLenA;
	m_nIsoFootTopSpacerHookLenBType = pIsoFootRebarTemplate->m_nIsoFootTopSpacerHookLenBType;
	m_nIsoFootTopSpacerMainBarHookLenB = pIsoFootRebarTemplate->m_nIsoFootTopSpacerMainBarHookLenB;
	m_nIsoFootTopSpacerUserHookLenB = pIsoFootRebarTemplate->m_nIsoFootTopSpacerUserHookLenB;
	m_nIsoFootTopSpacerWidth = pIsoFootRebarTemplate->m_nIsoFootTopSpacerWidth;

	m_bIsoFootBotSpacerSet = pIsoFootRebarTemplate->m_bIsoFootBotSpacerSet;
	m_nIsoFootBotSpacerDiaType = pIsoFootRebarTemplate->m_nIsoFootBotSpacerDiaType;
	m_nIsoFootBotSpacerDiaIndex = pIsoFootRebarTemplate->m_nIsoFootBotSpacerDiaIndex;
	m_nIsoFootBotSpacerSpacingType = pIsoFootRebarTemplate->m_nIsoFootBotSpacerSpacingType;
	m_nIsoFootBotSpacerSlabSpacing = pIsoFootRebarTemplate->m_nIsoFootBotSpacerSlabSpacing;
	m_nIsoFootBotSpacerUserSpacing1 = pIsoFootRebarTemplate->m_nIsoFootBotSpacerUserSpacing1;
	m_nIsoFootBotSpacerUserSpacing2 = pIsoFootRebarTemplate->m_nIsoFootBotSpacerUserSpacing2;
	m_nIsoFootBotSpacerHeightType = pIsoFootRebarTemplate->m_nIsoFootBotSpacerHeightType;
	m_nIsoFootBotSpacerUserHeight = pIsoFootRebarTemplate->m_nIsoFootBotSpacerUserHeight;
	m_nIsoFootBotSpacerHookLenAType = pIsoFootRebarTemplate->m_nIsoFootBotSpacerHookLenAType;
	m_nIsoFootBotSpacerMainBarHookLenA = pIsoFootRebarTemplate->m_nIsoFootBotSpacerMainBarHookLenA;
	m_nIsoFootBotSpacerUserHookLenA = pIsoFootRebarTemplate->m_nIsoFootBotSpacerUserHookLenA;
	m_nIsoFootBotSpacerHookLenBType = pIsoFootRebarTemplate->m_nIsoFootBotSpacerHookLenBType;
	m_nIsoFootBotSpacerMainBarHookLenB = pIsoFootRebarTemplate->m_nIsoFootBotSpacerMainBarHookLenB;
	m_nIsoFootBotSpacerUserHookLenB = pIsoFootRebarTemplate->m_nIsoFootBotSpacerUserHookLenB;
	m_nIsoFootBotSpacerWidth = pIsoFootRebarTemplate->m_nIsoFootBotSpacerWidth;

	m_bIsoFootSpacerSupportBarsSet = pIsoFootRebarTemplate->m_bIsoFootSpacerSupportBarsSet;
	m_bIsoFootSpacerSupportBarsConditionSet = pIsoFootRebarTemplate->m_bIsoFootSpacerSupportBarsConditionSet;
	m_nIsoFootSpacerSupportBarsCondition = pIsoFootRebarTemplate->m_nIsoFootSpacerSupportBarsCondition;
	m_nIsoFootSpacerSupportBarsDiaType = pIsoFootRebarTemplate->m_nIsoFootSpacerSupportBarsDiaType;
	m_nIsoFootSpacerSupportBarsUserDiaIndex = pIsoFootRebarTemplate->m_nIsoFootSpacerSupportBarsUserDiaIndex;
	m_nIsoFootSpacerSupportBarsNumType = pIsoFootRebarTemplate->m_nIsoFootSpacerSupportBarsNumType;
	m_dIsoFootSpacerSupportBarsTopSpacerNums = pIsoFootRebarTemplate->m_dIsoFootSpacerSupportBarsTopSpacerNums;
	m_nIsoFootSpacerSupportBarsUserNums = pIsoFootRebarTemplate->m_nIsoFootSpacerSupportBarsUserNums;
	m_nIsoFootSpacerSupportBarsLengthType = pIsoFootRebarTemplate->m_nIsoFootSpacerSupportBarsLengthType;
	m_nIsoFootSpacerSupportBarsUserLength = pIsoFootRebarTemplate->m_nIsoFootSpacerSupportBarsUserLength;
	m_dIsoFootSpacerSupportBarsUserRatio = pIsoFootRebarTemplate->m_dIsoFootSpacerSupportBarsUserRatio;

	m_bIsoFootSupportRebarsSet = pIsoFootRebarTemplate->m_bIsoFootSupportRebarsSet;
	m_nIsoFootSupportRebarsDiaIndex = pIsoFootRebarTemplate->m_nIsoFootSupportRebarsDiaIndex;
	m_dIsoFootSupportRebarsSlabLengthRate = pIsoFootRebarTemplate->m_dIsoFootSupportRebarsSlabLengthRate;
	m_nIsoFootSupportRebarsOrderLength = pIsoFootRebarTemplate->m_nIsoFootSupportRebarsOrderLength;
	m_nIsoFootSupportRebarsSpliceLength = pIsoFootRebarTemplate->m_nIsoFootSupportRebarsSpliceLength;
	m_nIsoFootSupportRebarsCorrectiveLength = pIsoFootRebarTemplate->m_nIsoFootSupportRebarsCorrectiveLength;

	m_bIsoFootBotSpacerUserSteelGradeS = pIsoFootRebarTemplate->GetIsoFootBotSpacerUserSteelGradeS();
	m_bIsoFootTopSpacerUserSteelGradeS = pIsoFootRebarTemplate->GetIsoFootTopSpacerUserSteelGradeS();
	m_bIsoFootSpacerSupportBarUserSteelGradeS = pIsoFootRebarTemplate->GetIsoFootSpacerSupportBarUserSteelGradeS();
	m_bIsoFootSupportRebarUserSteelGradeS = pIsoFootRebarTemplate->GetIsoFootSupportRebarUserSteelGradeS();

	m_bIsoFootTopSpacerPlaceConditionSet = pIsoFootRebarTemplate->GetIsoFootTopSpacerPlaceConditionSet();
	m_nIsoFootTopSpacerPlaceCondition = pIsoFootRebarTemplate->GetIsoFootTopSpacerPlaceCondition();

	m_bIsoFootSpliceAddLengthSet = pIsoFootRebarTemplate->GetIsoFootSpliceAddLengthSet();
	m_nIsoFootSpliceAddLengthNumSplice = pIsoFootRebarTemplate->GetIsoFootSpliceAddLengthNumSplice();
}

void IsoFootRebarTemplate::Serialize(CArchive& ar)
{
	long nDefaultValue = 0;
	long nIsoFootTopSpacerDiaIndex = 0;

	if(ar.IsStoring())
	{
		ar << m_bIsoFootTopSpacerInfoSet;
		ar << m_bIsoFootTopSpacerSet;
		ar << m_nIsoFootTopSpacerDiaType;
		ar << nDefaultValue;
		ar << m_nIsoFootTopSpacerSpacingType;
		ar << m_nIsoFootTopSpacerSlabSpacing;
		ar << m_nIsoFootTopSpacerUserSpacing1;
		ar << m_nIsoFootTopSpacerUserSpacing2;
		ar << m_nIsoFootTopSpacerHeightType;
		ar << m_nIsoFootTopSpacerUserHeight;
		ar << m_nIsoFootTopSpacerHookLenAType;
		ar << m_nIsoFootTopSpacerMainBarHookLenA;
		ar << m_nIsoFootTopSpacerUserHookLenA;
		ar << m_nIsoFootTopSpacerHookLenBType;
		ar << m_nIsoFootTopSpacerMainBarHookLenB;
		ar << m_nIsoFootTopSpacerUserHookLenB;
		ar << m_nIsoFootTopSpacerWidth;

		ar << m_bIsoFootBotSpacerInfoSet;
		ar << m_bIsoFootBotSpacerSet;
		ar << m_nIsoFootBotSpacerDiaType;
		ar << m_nIsoFootBotSpacerDiaIndex;
		ar << m_nIsoFootBotSpacerSpacingType;
		ar << m_nIsoFootBotSpacerSlabSpacing;
		ar << m_nIsoFootBotSpacerUserSpacing1;
		ar << m_nIsoFootBotSpacerUserSpacing2;
		ar << m_nIsoFootBotSpacerHeightType;
		ar << m_nIsoFootBotSpacerUserHeight;
		ar << m_nIsoFootBotSpacerHookLenAType;
		ar << m_nIsoFootBotSpacerMainBarHookLenA;
		ar << m_nIsoFootBotSpacerUserHookLenA;
		ar << m_nIsoFootBotSpacerHookLenBType;
		ar << m_nIsoFootBotSpacerMainBarHookLenB;
		ar << m_nIsoFootBotSpacerUserHookLenB;
		ar << m_nIsoFootBotSpacerWidth;

		ar << m_bIsoFootSpacerSupportBarsSet;
		ar << m_bIsoFootSpacerSupportBarsConditionSet;
		ar << m_nIsoFootSpacerSupportBarsCondition;
		ar << m_nIsoFootSpacerSupportBarsDiaType;
		ar << m_nIsoFootSpacerSupportBarsUserDiaIndex;
		ar << m_nIsoFootSpacerSupportBarsNumType;
		ar << m_dIsoFootSpacerSupportBarsTopSpacerNums;
		ar << m_nIsoFootSpacerSupportBarsUserNums;
		ar << m_nIsoFootSpacerSupportBarsLengthType;
		ar << m_nIsoFootSpacerSupportBarsUserLength;

		ar << m_nIsoFootTopSpacerSlabThick;
		ar << m_nIsoFootTopSpacerSlabThickOverDiaIndex;
		ar << m_nIsoFootTopSpacerSlabThickUnderDiaIndex;

		ar << m_bIsoFootSpliceDevConditionSet;
		ar << m_nIsoFootSpliceDevConditionLength;

		ar << m_dIsoFootSpacerSupportBarsUserRatio;

		ar << m_nIsoFootDevType;

		ar << m_bIsoFootSupportRebarsSet;
		ar << m_nIsoFootSupportRebarsDiaIndex;
		ar << m_dIsoFootSupportRebarsSlabLengthRate;
		ar << m_nIsoFootSupportRebarsOrderLength;
		ar << m_nIsoFootSupportRebarsSpliceLength;
		ar << m_nIsoFootSupportRebarsCorrectiveLength;

		ar << m_bIsoFootBotSpacerUserSteelGradeS;
		ar << m_bIsoFootTopSpacerUserSteelGradeS;
		ar << m_bIsoFootSpacerSupportBarUserSteelGradeS;
		ar << m_bIsoFootSupportRebarUserSteelGradeS;

		ar << m_bIsoFootTopSpacerPlaceConditionSet;
		ar << m_nIsoFootTopSpacerPlaceCondition;

		ar << m_bIsoFootSpliceAddLengthSet;
		ar << m_nIsoFootSpliceAddLengthNumSplice;
	}
	else
	{
		ar >> m_bIsoFootTopSpacerInfoSet;
		ar >> m_bIsoFootTopSpacerSet;
		ar >> m_nIsoFootTopSpacerDiaType;
		ar >> nIsoFootTopSpacerDiaIndex;
		ar >> m_nIsoFootTopSpacerSpacingType;
		ar >> m_nIsoFootTopSpacerSlabSpacing;
		ar >> m_nIsoFootTopSpacerUserSpacing1;
		ar >> m_nIsoFootTopSpacerUserSpacing2;
		ar >> m_nIsoFootTopSpacerHeightType;
		ar >> m_nIsoFootTopSpacerUserHeight;
		ar >> m_nIsoFootTopSpacerHookLenAType;
		ar >> m_nIsoFootTopSpacerMainBarHookLenA;
		ar >> m_nIsoFootTopSpacerUserHookLenA;
		ar >> m_nIsoFootTopSpacerHookLenBType;
		ar >> m_nIsoFootTopSpacerMainBarHookLenB;
		ar >> m_nIsoFootTopSpacerUserHookLenB;
		ar >> m_nIsoFootTopSpacerWidth;

		ar >> m_bIsoFootBotSpacerInfoSet;
		ar >> m_bIsoFootBotSpacerSet;
		ar >> m_nIsoFootBotSpacerDiaType;
		ar >> m_nIsoFootBotSpacerDiaIndex;
		ar >> m_nIsoFootBotSpacerSpacingType;
		ar >> m_nIsoFootBotSpacerSlabSpacing;
		ar >> m_nIsoFootBotSpacerUserSpacing1;
		ar >> m_nIsoFootBotSpacerUserSpacing2;
		ar >> m_nIsoFootBotSpacerHeightType;
		ar >> m_nIsoFootBotSpacerUserHeight;
		ar >> m_nIsoFootBotSpacerHookLenAType;
		ar >> m_nIsoFootBotSpacerMainBarHookLenA;
		ar >> m_nIsoFootBotSpacerUserHookLenA;
		ar >> m_nIsoFootBotSpacerHookLenBType;
		ar >> m_nIsoFootBotSpacerMainBarHookLenB;
		ar >> m_nIsoFootBotSpacerUserHookLenB;
		ar >> m_nIsoFootBotSpacerWidth;

		if (MSVersionInfo::GetCurrentVersion() >= 20170822)
		{
			ar >> m_bIsoFootSpacerSupportBarsSet;
			ar >> m_bIsoFootSpacerSupportBarsConditionSet;
			ar >> m_nIsoFootSpacerSupportBarsCondition;
			ar >> m_nIsoFootSpacerSupportBarsDiaType;
			ar >> m_nIsoFootSpacerSupportBarsUserDiaIndex;
			ar >> m_nIsoFootSpacerSupportBarsNumType;
			ar >> m_dIsoFootSpacerSupportBarsTopSpacerNums;
			ar >> m_nIsoFootSpacerSupportBarsUserNums;
			ar >> m_nIsoFootSpacerSupportBarsLengthType;
			ar >> m_nIsoFootSpacerSupportBarsUserLength;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20180717)
		{
			ar >> m_nIsoFootTopSpacerSlabThick;
			ar >> m_nIsoFootTopSpacerSlabThickOverDiaIndex;
			ar >> m_nIsoFootTopSpacerSlabThickUnderDiaIndex;
		}
		else
		{
			if(m_nIsoFootTopSpacerDiaType == 1)
				m_nIsoFootTopSpacerSlabThickOverDiaIndex = nIsoFootTopSpacerDiaIndex;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20190423)
		{
			ar >> m_bIsoFootSpliceDevConditionSet;
			ar >> m_nIsoFootSpliceDevConditionLength;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20191007)
		{
			ar >> m_dIsoFootSpacerSupportBarsUserRatio;
		}
		long nIsoFootDevTypeLx = 0, nIsoFootDevTypeLy = 0;
		if(MSVersionInfo::GetCurrentVersion() >= 20200109 && MSVersionInfo::GetCurrentVersion() < 20230419)
		{			
			ar >> nIsoFootDevTypeLx;
			ar >> nIsoFootDevTypeLy;			
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20230419)
		{
			ar >> m_nIsoFootDevType;

			ar >> m_bIsoFootSupportRebarsSet;
			ar >> m_nIsoFootSupportRebarsDiaIndex;
			ar >> m_dIsoFootSupportRebarsSlabLengthRate;
			ar >> m_nIsoFootSupportRebarsOrderLength;
			ar >> m_nIsoFootSupportRebarsSpliceLength;
			ar >> m_nIsoFootSupportRebarsCorrectiveLength;
		}
		else
		{
			m_nIsoFootDevType = nIsoFootDevTypeLx;
		}
		if (MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231010)
		{
			ar >> m_bIsoFootBotSpacerUserSteelGradeS;
			ar >> m_bIsoFootTopSpacerUserSteelGradeS;
			ar >> m_bIsoFootSpacerSupportBarUserSteelGradeS;
			ar >> m_bIsoFootSupportRebarUserSteelGradeS;
		}
		if(MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20240306)
		{
			ar >> m_bIsoFootTopSpacerPlaceConditionSet;
			ar >> m_nIsoFootTopSpacerPlaceCondition;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20241018)
		{
			ar >> m_bIsoFootSpliceAddLengthSet;
			ar >> m_nIsoFootSpliceAddLengthNumSplice;
		}
		else
		{
			m_bIsoFootSpliceAddLengthSet = FALSE;
		}
	}
}

void IsoFootRebarTemplate::DataInit()
{
	m_bIsoFootSpliceDevConditionSet = false;
	m_nIsoFootSpliceDevConditionLength = 340;

	m_bIsoFootTopSpacerInfoSet = false;
	m_bIsoFootTopSpacerSet = false;
	m_nIsoFootTopSpacerDiaType = 0;
	m_nIsoFootTopSpacerSlabThick = 700;
	m_nIsoFootTopSpacerSlabThickOverDiaIndex = 2;
	m_nIsoFootTopSpacerSlabThickUnderDiaIndex = 1;
	m_nIsoFootTopSpacerSpacingType = 1;
	m_nIsoFootTopSpacerSlabSpacing = 3;
	m_nIsoFootTopSpacerUserSpacing1 = 1500;
	m_nIsoFootTopSpacerUserSpacing2 = 1500;
	m_nIsoFootTopSpacerHeightType = 0;
	m_nIsoFootTopSpacerUserHeight = 300;
	m_nIsoFootTopSpacerHookLenAType = 1;
	m_nIsoFootTopSpacerMainBarHookLenA = 100;
	m_nIsoFootTopSpacerUserHookLenA = 100;
	m_nIsoFootTopSpacerHookLenBType = 1;
	m_nIsoFootTopSpacerMainBarHookLenB = 0;
	m_nIsoFootTopSpacerUserHookLenB = 100;
	m_nIsoFootTopSpacerWidth = 200;

	m_bIsoFootBotSpacerInfoSet = false;
	m_bIsoFootBotSpacerSet = false;
	m_nIsoFootBotSpacerDiaType = 0;
	m_nIsoFootBotSpacerDiaIndex = 0;
	m_nIsoFootBotSpacerSpacingType = 1;
	m_nIsoFootBotSpacerSlabSpacing = 3;
	m_nIsoFootBotSpacerUserSpacing1 = 1500;
	m_nIsoFootBotSpacerUserSpacing2 = 1500;
	m_nIsoFootBotSpacerHeightType = 0;
	m_nIsoFootBotSpacerUserHeight = 300;
	m_nIsoFootBotSpacerHookLenAType = 1;
	m_nIsoFootBotSpacerMainBarHookLenA = 100;
	m_nIsoFootBotSpacerUserHookLenA = 100;
	m_nIsoFootBotSpacerHookLenBType = 1;
	m_nIsoFootBotSpacerMainBarHookLenB = 0;
	m_nIsoFootBotSpacerUserHookLenB = 100;
	m_nIsoFootBotSpacerWidth = 200;

	m_bIsoFootSpacerSupportBarsSet = false;
	m_bIsoFootSpacerSupportBarsConditionSet = true;
	m_nIsoFootSpacerSupportBarsCondition = 1000;
	m_nIsoFootSpacerSupportBarsDiaType = 1;
	m_nIsoFootSpacerSupportBarsUserDiaIndex = 0;
	m_nIsoFootSpacerSupportBarsNumType = 0;
	m_dIsoFootSpacerSupportBarsTopSpacerNums = 0.5;
	m_nIsoFootSpacerSupportBarsUserNums = 10;
	m_nIsoFootSpacerSupportBarsLengthType = 1;
	m_nIsoFootSpacerSupportBarsUserLength = 1600;

	m_dIsoFootSpacerSupportBarsUserRatio = 1.4;

	m_nIsoFootDevType = 0;	

	m_bIsoFootSupportRebarsSet = false;
	m_nIsoFootSupportRebarsDiaIndex = 0;
	m_dIsoFootSupportRebarsSlabLengthRate = 1;
	m_nIsoFootSupportRebarsOrderLength = 8000;
	m_nIsoFootSupportRebarsSpliceLength = 300;
	m_nIsoFootSupportRebarsCorrectiveLength = 10;

	m_bIsoFootBotSpacerUserSteelGradeS = false;
	m_bIsoFootTopSpacerUserSteelGradeS = false;
	m_bIsoFootSpacerSupportBarUserSteelGradeS = false;
	m_bIsoFootSupportRebarUserSteelGradeS = false;

	m_bIsoFootTopSpacerPlaceConditionSet = FALSE;
	m_nIsoFootTopSpacerPlaceCondition = 0;

	m_bIsoFootSpliceAddLengthSet = TRUE;
	m_nIsoFootSpliceAddLengthNumSplice = 3;
}


