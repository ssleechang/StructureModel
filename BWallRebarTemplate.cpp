#include "StdAfx.h"
#include "BWallRebarTemplate.h"
#include "MSDeformedBar.h"
#include "MSObject.h"
#include "..\GMLib\MSVersionInfo.h"
#include "BWallVerBarRebarTemplate.h"
#include "BWallDowelRebarTemplate.h"
#include "VerMemberLinkDowelTemplate.h"

BWallRebarTemplate::BWallRebarTemplate() :BaseRebarTemplate()
{
	DataInit();
	m_pVerBarTemplateBelowDia = new BWallVerBarRebarTemplate();
	m_pVerBarTemplateExcessDia = new BWallVerBarRebarTemplate();
	m_pDowelTemplateBelowDia = new BWallDowelRebarTemplate();
	m_pDowelTemplateExcessDia = new BWallDowelRebarTemplate();
	m_pBWallVerMemberLinkDowelTemplate = new VerMemberLinkDowelTemplate();
}

BWallRebarTemplate::~BWallRebarTemplate(void)
{
	delete m_pVerBarTemplateBelowDia;
	delete m_pVerBarTemplateExcessDia; 
	delete m_pDowelTemplateBelowDia;
	delete m_pDowelTemplateExcessDia;
	delete m_pBWallVerMemberLinkDowelTemplate;
}

void BWallRebarTemplate::DataInit()
{
	CStringArray BarDiaArr;
	MSDeformedBar::GetRebarNameList(BarDiaArr);

	m_nBWallVerBarPlaceTypeDiaIndex = BarDiaArr.GetSize() - 1;
	m_nBWallDowelPlaceTypeDiaIndex = BarDiaArr.GetSize() - 1;

	m_nBWallVerNumBarCrossBarIncludeSet = 0;
	m_nBWallCrossBarEAType = 0;
	m_nBWallCrossBarStartSpacingType = 1;
	m_bBWallPlaceEndBarSet = false;
	m_nBWallEndVerAddBarCount = 2;
	m_nBWallEndVerAddBarDist = 100;
	m_nBWallEndVerStartSpacingType = 1;
	
	InitAddBarDiaIndexMap(mm_BWallAddBarDiaIndex, true);
	InitEndBarDiaIndexMap(mm_BWallEndBarDiaIndex, true);

	m_bBWallPlaceUBar4ShortWall = true;
	m_nBWallMaxUbarLegLength = 2000;
	m_bBWallPlaceStirrup4ShortWall = false;
	m_nBWallHBarMaxStirrupLen = 1000;
	m_bBWallHorBarStartSet = false;
	m_nBWallHorBarStartLength = 50;
	m_bBWallDefaultUbarLen = false;
	m_bBWallCrossBarUBarHalfPlace = false;
	m_bBWallStraightEndUBarPlaceSet = true;
	
	InitDefaultUbarLenMap(mm_BWallDefaultUbarLen);

	m_nBWallSpacerDiaType = 0;
	m_nBWallSpacerDiaIndex = 0;
	m_nBWallSpacerHorSpacing = 1500;
	m_nBWallSpacerVerSpacingType = 0;
	m_nBWallSpacerVerSpacing = 1500;
	m_nBWallSpacerVerSpacingRate = 3;
	m_nBWallSpacerVerSpacingCount = 3;
	m_bBWallSpacerVerEndBarPlaceSet = false;
	m_nBWallSpacerVerEndBarSpacingType = 0;
	m_nBWallSpacerVerEndBarSpacing = 1500;
	m_nBWallSpacerVerEndBarSpacingRate = 3;
	m_nBWallSpacerVerEndBarSpacingCount = 3;
	m_nBWallDowelSpacerVerSpacingCount = 0;
	m_nBWallSpacerLenType = 0;
	m_nBWallSpacerLenCalWayA = 0;
	m_nBWallSpacerLenCalWayB = 0;
	m_nBWallSpacerLenA = 350;
	m_nBWallSpacerLenB = 350;
	m_nBWallSpacerShape = 0;
	m_bBWallSpacerRebarPlaceSet = true;
	m_nBWallSpacerRebarPlaceCondition = 2000;

	m_bBWallSupportRebarsSet = false;
	m_nBWallSupportRebarsDiaIndex = 0;
	m_dBWallSupportRebarsTotalLengthRate = 2.0;
	m_nBWallSupportRebarsOrderLength = 8000;
	m_nBWallSupportRebarsSpliceLength = 300;
	m_nBWallSupportRebarsCorrectiveLength = 10;

	m_bBWallAddBarsSet = false;
	m_nBWallAddBarsDiaIndex = 0;
	m_nBWallAddBarsNumber = 4;
	m_nBWallAddBarsLengthType = 0;
	m_nBWallAddBarsTensionLengthType = 0;
	m_nBWallAddBarsUserLength = 1600;
	m_bBWallDowelSupportPlaceType = false;
	
	InitUserDowelHookLenMap(mm_BWallUserDowelHookLen);
	InitUserDowelSpliceLenMap(mm_BWallUserDowelSpliceLen);

	m_bBWallShearBarSet = false;
	m_nBWallShearBarDiaType = 0;
	m_nBWallShearBarDiaIndex = 0;
	m_nBWallShearBarUserDiaType = 0;
	InitShearBarVerUserDiaIndexMap(mm_BWallShearBarVerUserDiaIndex);
	InitShearBarHorUserDiaIndexMap(mm_BWallShearBarHorUserDiaIndex);

	m_nBWallShearBarShapeType = 0;

	m_nBWallShearBarVerSpacingCondition = 150;
	m_nBWallShearBarVerBelowType = 1;
	m_nBWallShearBarVerBelowSpacing = 300;
	m_nBWallShearBarVerBelowRatio = 1;
	m_nBWallShearBarVerBelowNum = 5;
	m_nBWallShearBarVerOverType = 1;
	m_nBWallShearBarVerOverSpacing = 300;
	m_nBWallShearBarVerOverRatio = 1;
	m_nBWallShearBarVerOverNum = 5;

	m_nBWallShearBarHorSpacingCondition = 150;
	m_nBWallShearBarHorBelowType = 1;
	m_nBWallShearBarHorBelowSpacing = 300;
	m_nBWallShearBarHorBelowRatio = 1;
	m_nBWallShearBarHorBelowNum = 5;
	m_nBWallShearBarHorOverType = 1;
	m_nBWallShearBarHorOverSpacing = 300;
	m_nBWallShearBarHorOverRatio = 1;
	m_nBWallShearBarHorOverNum = 5;

	m_bBWallPlaceBraceSupportBar = FALSE;
	m_nBWallBraceSupportBarOverDiaIndex = 1;
	m_bBWallBraceSupportBarDiaConditionHeight = 3200;
	m_nBWallBraceSupportBarUnderDiaIndex = 1;
	m_nBWallBraceSupportBarWallLengthCondition = 8000;
	m_nBWallBraceSupportBarWallLengthConditionOverLength = 1;
	m_nBWallBraceSupportBarWallLengthConditionUnderLength = 0;
	m_bBWallBraceSupportNotPlaceConditionDiaSet = TRUE;
	m_nBWallBraceSupportNotPlaceConditionDiaIndex = 2;
	m_bBWallBraceSupportNotPlaceConditionLengthSet = TRUE;
	m_nBWallBraceSupportNotPlaceConditionLength = 4000;
	m_bBWallBraceSupportNotPlaceConditionBottomDowel = TRUE;

	m_bBWallBraceSupportNotPlaceConditionHeightSet = FALSE;
	m_nBWallBraceSupportNotPlaceConditionHeight = 3000;

	m_bBWallCrossBarEATypeExcessThickSet = FALSE;
	m_nBWallCrossBarEATypeExcessThick = 300;

	InitUserDiaIndexMap(mm_BWallDowelVerBarUserDiaIndex);

	m_bBWallDowelVerBarUserDiaSet = FALSE;

	m_bBWallUBarAddLengthSet = FALSE;
	m_nBWallUBarAddLength = 0;
	m_bBWallSpacerAddLengthSet = FALSE;
	m_nBWallSpacerAddLength = 0;
	m_bBWallBraceSupportBarUserSteelGradeS = FALSE;

	m_bBWallSpacerUserSteelGradeS = FALSE;
	m_bBWallSupportRebarsUserSteelGradeS = FALSE;
	m_bBWallAddBarsUserSteelGradeS = FALSE;
	m_bBWallShearBarUserSteelGradeS = FALSE;

	m_bBWallDowelVerBarUserSteelGradeS = FALSE;

	m_bBWallStraightCenterUmaShape = FALSE;
	m_nBWallUmaSpacerDiaType = 0;
	m_nBWallUmaSpacerWallThick = 700;
	m_nBWallUmaSpacerWallThickOverDiaIndex = 2;
	m_nBWallUmaSpacerWallThickUnderDiaIndex = 1;
	m_bBWallUmaSpacerUserSteelGradeS = FALSE;
	m_nBWallUmaSpacerSpacingType = 1;
	m_nBWallUmaSpacerWallSpacing = 3;
	m_nBWallUmaSpacerUserSpacing1 = 1500;
	m_nBWallUmaSpacerUserSpacing2 = 1500;
	m_nBWallUmaSpacerHeightType = 0;
	m_nBWallUmaSpacerUserHeight = 300;
	m_nBWallUmaSpacerHookLenAType = 1;
	m_nBWallUmaSpacerMainBarHookLenA = 100;
	m_nBWallUmaSpacerUserHookLenA = 100;
	m_nBWallUmaSpacerHookLenBType = 1;
	m_nBWallUmaSpacerMainBarHookLenB = 0;
	m_nBWallUmaSpacerUserHookLenB = 100;
	m_nBWallUmaSpacerWidth = 200;
	m_bBWallUShapeTopSpacer = FALSE;
	m_bBWallUShapeBotSpacer = FALSE;
	m_nBWallUShapeSpacerDiaType = 0;
	m_nBWallUShapeSpacerDiaIndex = 0;
	m_bBWallUShapeSpacerUserSteelGradeS = FALSE;
	m_nBWallUShapeSpacerHorSpacingType = 0;
	m_nBWallUShapeSpacerHorSpacing = 1500;
	m_nBWallUShapeSpacerHorSpacingRate = 3;
	m_nBWallUShapeSpacerHorSpacingCount = 3;
	m_nBWallUShapeSpacerLenType = 0;
	m_nBWallUShapeSpacerUserLen = 350;

	m_bBWallOpeningAddBarDeductionCondition = FALSE;

	m_bBWallUmaSpacerConditionSet = FALSE;
	m_nBWallUmaSpacerCondition = 5000;

	m_bBWallSpacerVerStartSpacingSet = FALSE;
	m_nBWallSpacerVerStartSpacing = 1500;

	m_bBWallSpliceAddLengthSet = TRUE;
	m_nBWallSpliceAddLengthNumSplice = 3;

	m_nBWallPlusCrossShapeType = 1;
	m_nBWallTCrossShapeType = 2;
	m_nBWallLCrossShapeType = 1;

	m_nBWallStraightCenterUmaPlaceType = 0;
}

void BWallRebarTemplate::CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate)
{
	BWallRebarTemplate *pBWallRebarTemplate = dynamic_cast<BWallRebarTemplate*>(pBaseRebarTemplate);
	if(pBWallRebarTemplate == nullptr)
		return;

	m_nBWallVerBarPlaceTypeDiaIndex = pBWallRebarTemplate->GetBWallVerBarPlaceTypeDiaIndex();
	m_nBWallDowelPlaceTypeDiaIndex = pBWallRebarTemplate->GetBWallDowelPlaceTypeDiaIndex();

	m_pVerBarTemplateBelowDia->CopyFromMe(pBWallRebarTemplate->GetBWallVerBarTemplateBelowDia());
	m_pVerBarTemplateExcessDia->CopyFromMe(pBWallRebarTemplate->GetBWallVerBarTemplateExcessDia());

	m_pDowelTemplateBelowDia->CopyFromMe(pBWallRebarTemplate->GetBWallDowelTemplateBelowDia());
	m_pDowelTemplateExcessDia->CopyFromMe(pBWallRebarTemplate->GetBWallDowelTemplateExcessDia());


	m_nBWallVerNumBarCrossBarIncludeSet = pBWallRebarTemplate->m_nBWallVerNumBarCrossBarIncludeSet;
	m_nBWallCrossBarEAType = pBWallRebarTemplate->m_nBWallCrossBarEAType;
	m_nBWallCrossBarStartSpacingType = pBWallRebarTemplate->m_nBWallCrossBarStartSpacingType;
	m_bBWallPlaceEndBarSet = pBWallRebarTemplate->m_bBWallPlaceEndBarSet;
	m_nBWallEndVerAddBarCount = pBWallRebarTemplate->m_nBWallEndVerAddBarCount;
	m_nBWallEndVerAddBarDist = pBWallRebarTemplate->m_nBWallEndVerAddBarDist;
	m_nBWallEndVerStartSpacingType = pBWallRebarTemplate->m_nBWallEndVerStartSpacingType;
	
	AddBarDiaIndexMapDataCopy(mm_BWallAddBarDiaIndex, pBWallRebarTemplate->mm_BWallAddBarDiaIndex);
	EndBarDiaIndexMapDataCopy(mm_BWallEndBarDiaIndex, pBWallRebarTemplate->mm_BWallEndBarDiaIndex);

	m_bBWallPlaceUBar4ShortWall = pBWallRebarTemplate->m_bBWallPlaceUBar4ShortWall;
	m_nBWallMaxUbarLegLength = pBWallRebarTemplate->m_nBWallMaxUbarLegLength;
	m_bBWallPlaceStirrup4ShortWall = pBWallRebarTemplate->m_bBWallPlaceStirrup4ShortWall;
	m_nBWallHBarMaxStirrupLen = pBWallRebarTemplate->m_nBWallHBarMaxStirrupLen;
	m_bBWallHorBarStartSet = pBWallRebarTemplate->m_bBWallHorBarStartSet;
	m_nBWallHorBarStartLength = pBWallRebarTemplate->m_nBWallHorBarStartLength;
	m_bBWallDefaultUbarLen = pBWallRebarTemplate->m_bBWallDefaultUbarLen;
	m_bBWallCrossBarUBarHalfPlace = pBWallRebarTemplate->m_bBWallCrossBarUBarHalfPlace;
	m_bBWallStraightEndUBarPlaceSet = pBWallRebarTemplate->m_bBWallStraightEndUBarPlaceSet;

	DefaultUbarLenMapDataCopy(mm_BWallDefaultUbarLen, pBWallRebarTemplate->mm_BWallDefaultUbarLen);

	m_nBWallSpacerDiaType = pBWallRebarTemplate->m_nBWallSpacerDiaType;
	m_nBWallSpacerDiaIndex = pBWallRebarTemplate->m_nBWallSpacerDiaIndex;
	m_nBWallSpacerHorSpacing = pBWallRebarTemplate->m_nBWallSpacerHorSpacing;
	m_nBWallSpacerVerSpacingType = pBWallRebarTemplate->m_nBWallSpacerVerSpacingType;
	m_nBWallSpacerVerSpacing = pBWallRebarTemplate->m_nBWallSpacerVerSpacing;
	m_nBWallSpacerVerSpacingRate = pBWallRebarTemplate->m_nBWallSpacerVerSpacingRate;
	m_nBWallSpacerVerSpacingCount = pBWallRebarTemplate->m_nBWallSpacerVerSpacingCount;
	m_bBWallSpacerVerEndBarPlaceSet = pBWallRebarTemplate->m_bBWallSpacerVerEndBarPlaceSet;
	m_nBWallSpacerVerEndBarSpacingType = pBWallRebarTemplate->m_nBWallSpacerVerEndBarSpacingType;
	m_nBWallSpacerVerEndBarSpacing = pBWallRebarTemplate->m_nBWallSpacerVerEndBarSpacing;
	m_nBWallSpacerVerEndBarSpacingRate = pBWallRebarTemplate->m_nBWallSpacerVerEndBarSpacingRate;
	m_nBWallSpacerVerEndBarSpacingCount = pBWallRebarTemplate->m_nBWallSpacerVerEndBarSpacingCount;
	m_nBWallDowelSpacerVerSpacingCount = pBWallRebarTemplate->m_nBWallDowelSpacerVerSpacingCount;
	m_nBWallSpacerLenType = pBWallRebarTemplate->m_nBWallSpacerLenType;
	m_nBWallSpacerLenCalWayA = pBWallRebarTemplate->m_nBWallSpacerLenCalWayA;
	m_nBWallSpacerLenCalWayB = pBWallRebarTemplate->m_nBWallSpacerLenCalWayB;
	m_nBWallSpacerLenA = pBWallRebarTemplate->m_nBWallSpacerLenA;
	m_nBWallSpacerLenB = pBWallRebarTemplate->m_nBWallSpacerLenB;
	m_nBWallSpacerShape = pBWallRebarTemplate->m_nBWallSpacerShape;
	m_bBWallSpacerRebarPlaceSet = pBWallRebarTemplate->m_bBWallSpacerRebarPlaceSet;
	m_nBWallSpacerRebarPlaceCondition = pBWallRebarTemplate->m_nBWallSpacerRebarPlaceCondition;

	m_bBWallSupportRebarsSet = pBWallRebarTemplate->m_bBWallSupportRebarsSet;
	m_nBWallSupportRebarsDiaIndex = pBWallRebarTemplate->m_nBWallSupportRebarsDiaIndex;
	m_dBWallSupportRebarsTotalLengthRate = pBWallRebarTemplate->m_dBWallSupportRebarsTotalLengthRate;
	m_nBWallSupportRebarsOrderLength = pBWallRebarTemplate->m_nBWallSupportRebarsOrderLength;
	m_nBWallSupportRebarsSpliceLength = pBWallRebarTemplate->m_nBWallSupportRebarsSpliceLength;
	m_nBWallSupportRebarsCorrectiveLength = pBWallRebarTemplate->m_nBWallSupportRebarsCorrectiveLength;

	m_bBWallAddBarsSet = pBWallRebarTemplate->m_bBWallAddBarsSet;
	m_nBWallAddBarsDiaIndex = pBWallRebarTemplate->m_nBWallAddBarsDiaIndex;
	m_nBWallAddBarsNumber = pBWallRebarTemplate->m_nBWallAddBarsNumber;
	m_nBWallAddBarsLengthType = pBWallRebarTemplate->m_nBWallAddBarsLengthType;
	m_nBWallAddBarsTensionLengthType = pBWallRebarTemplate->m_nBWallAddBarsTensionLengthType;
	m_nBWallAddBarsUserLength = pBWallRebarTemplate->m_nBWallAddBarsUserLength;
	
	m_bBWallDowelSupportPlaceType = pBWallRebarTemplate->m_bBWallDowelSupportPlaceType;

	
	UserDowelHookLenMapDataCopy(mm_BWallUserDowelHookLen, pBWallRebarTemplate->mm_BWallUserDowelHookLen);
	UserDowelSpliceLenMapDataCopy(mm_BWallUserDowelSpliceLen, pBWallRebarTemplate->mm_BWallUserDowelSpliceLen);

	m_bBWallShearBarSet = pBWallRebarTemplate->m_bBWallShearBarSet;
	m_nBWallShearBarDiaType = pBWallRebarTemplate->m_nBWallShearBarDiaType;
	m_nBWallShearBarDiaIndex = pBWallRebarTemplate->m_nBWallShearBarDiaIndex;
	m_nBWallShearBarUserDiaType = pBWallRebarTemplate->m_nBWallShearBarUserDiaType;
	ShearBarVerUserDiaIndexMapDataCopy(mm_BWallShearBarVerUserDiaIndex, pBWallRebarTemplate->mm_BWallShearBarVerUserDiaIndex);
	ShearBarHorUserDiaIndexMapDataCopy(mm_BWallShearBarHorUserDiaIndex, pBWallRebarTemplate->mm_BWallShearBarHorUserDiaIndex);

	m_nBWallShearBarShapeType = pBWallRebarTemplate->m_nBWallShearBarShapeType;

	m_nBWallShearBarVerSpacingCondition = pBWallRebarTemplate->m_nBWallShearBarVerSpacingCondition;
	m_nBWallShearBarVerBelowType = pBWallRebarTemplate->m_nBWallShearBarVerBelowType;
	m_nBWallShearBarVerBelowSpacing = pBWallRebarTemplate->m_nBWallShearBarVerBelowSpacing;
	m_nBWallShearBarVerBelowRatio = pBWallRebarTemplate->m_nBWallShearBarVerBelowRatio;
	m_nBWallShearBarVerBelowNum = pBWallRebarTemplate->m_nBWallShearBarVerBelowNum;
	m_nBWallShearBarVerOverType = pBWallRebarTemplate->m_nBWallShearBarVerOverType;
	m_nBWallShearBarVerOverSpacing = pBWallRebarTemplate->m_nBWallShearBarVerOverSpacing;
	m_nBWallShearBarVerOverRatio = pBWallRebarTemplate->m_nBWallShearBarVerOverRatio;
	m_nBWallShearBarVerOverNum = pBWallRebarTemplate->m_nBWallShearBarVerOverNum;

	m_nBWallShearBarHorSpacingCondition = pBWallRebarTemplate->m_nBWallShearBarHorSpacingCondition;
	m_nBWallShearBarHorBelowType = pBWallRebarTemplate->m_nBWallShearBarHorBelowType;
	m_nBWallShearBarHorBelowSpacing = pBWallRebarTemplate->m_nBWallShearBarHorBelowSpacing;
	m_nBWallShearBarHorBelowRatio = pBWallRebarTemplate->m_nBWallShearBarHorBelowRatio;
	m_nBWallShearBarHorBelowNum = pBWallRebarTemplate->m_nBWallShearBarHorBelowNum;
	m_nBWallShearBarHorOverType = pBWallRebarTemplate->m_nBWallShearBarHorOverType;
	m_nBWallShearBarHorOverSpacing = pBWallRebarTemplate->m_nBWallShearBarHorOverSpacing;
	m_nBWallShearBarHorOverRatio = pBWallRebarTemplate->m_nBWallShearBarHorOverRatio;
	m_nBWallShearBarHorOverNum = pBWallRebarTemplate->m_nBWallShearBarHorOverNum;

	m_bBWallPlaceBraceSupportBar = pBWallRebarTemplate->m_bBWallPlaceBraceSupportBar;
	m_nBWallBraceSupportBarOverDiaIndex = pBWallRebarTemplate->m_nBWallBraceSupportBarOverDiaIndex;
	m_bBWallBraceSupportBarDiaConditionHeight = pBWallRebarTemplate->m_bBWallBraceSupportBarDiaConditionHeight;;
	m_nBWallBraceSupportBarUnderDiaIndex = pBWallRebarTemplate->m_nBWallBraceSupportBarUnderDiaIndex;;
	m_nBWallBraceSupportBarWallLengthCondition = pBWallRebarTemplate->m_nBWallBraceSupportBarWallLengthCondition;
	m_nBWallBraceSupportBarWallLengthConditionOverLength = pBWallRebarTemplate->m_nBWallBraceSupportBarWallLengthConditionOverLength;
	m_nBWallBraceSupportBarWallLengthConditionUnderLength = pBWallRebarTemplate->m_nBWallBraceSupportBarWallLengthConditionUnderLength;
	m_bBWallBraceSupportNotPlaceConditionDiaSet = pBWallRebarTemplate->m_bBWallBraceSupportNotPlaceConditionDiaSet;
	m_nBWallBraceSupportNotPlaceConditionDiaIndex = pBWallRebarTemplate->m_nBWallBraceSupportNotPlaceConditionDiaIndex;
	m_bBWallBraceSupportNotPlaceConditionLengthSet = pBWallRebarTemplate->m_bBWallBraceSupportNotPlaceConditionLengthSet;
	m_nBWallBraceSupportNotPlaceConditionLength = pBWallRebarTemplate->m_nBWallBraceSupportNotPlaceConditionLength;
	m_bBWallBraceSupportNotPlaceConditionBottomDowel = pBWallRebarTemplate->m_bBWallBraceSupportNotPlaceConditionBottomDowel;

	m_bBWallBraceSupportNotPlaceConditionHeightSet = pBWallRebarTemplate->m_bBWallBraceSupportNotPlaceConditionHeightSet;
	m_nBWallBraceSupportNotPlaceConditionHeight = pBWallRebarTemplate->m_nBWallBraceSupportNotPlaceConditionHeight;

	m_bBWallCrossBarEATypeExcessThickSet = pBWallRebarTemplate->m_bBWallCrossBarEATypeExcessThickSet;
	m_nBWallCrossBarEATypeExcessThick = pBWallRebarTemplate->m_nBWallCrossBarEATypeExcessThick;
	
	UserMapDataCopy(mm_BWallDowelVerBarUserDiaIndex, pBWallRebarTemplate->mm_BWallDowelVerBarUserDiaIndex);

	m_bBWallDowelVerBarUserDiaSet = pBWallRebarTemplate->GetBWallDowelVerBarUserDiaSet();

	m_pBWallVerMemberLinkDowelTemplate->CopyFromMe(pBWallRebarTemplate->GetBWallVerMemberLinkDowelTemplate());
	m_bBWallUBarAddLengthSet = pBWallRebarTemplate->GetBWallUBarAddLengthSet();						
	m_nBWallUBarAddLength = pBWallRebarTemplate->GetBWallUBarAddLength();
	m_bBWallSpacerAddLengthSet = pBWallRebarTemplate->GetBWallSpacerAddLengthSet();
	m_nBWallSpacerAddLength = pBWallRebarTemplate->GetBWallSpacerAddLength();
	m_bBWallBraceSupportBarUserSteelGradeS = pBWallRebarTemplate->GetBWallBraceSupportBarUserSteelGradeS();

	m_bBWallSpacerUserSteelGradeS = pBWallRebarTemplate->GetBWallSpacerUserSteelGradeS();
	m_bBWallSupportRebarsUserSteelGradeS = pBWallRebarTemplate->GetBWallSupportRebarsUserSteelGradeS();
	m_bBWallAddBarsUserSteelGradeS = pBWallRebarTemplate->GetBWallAddBarsUserSteelGradeS();
	m_bBWallShearBarUserSteelGradeS = pBWallRebarTemplate->GetBWallShearBarUserSteelGradeS();

	m_bBWallDowelVerBarUserSteelGradeS = pBWallRebarTemplate->GetBWallDowelVerBarUserSteelGradeS();

	m_bBWallStraightCenterUmaShape = pBWallRebarTemplate->GetBWallStraightCenterUmaShape();
	m_nBWallUmaSpacerDiaType = pBWallRebarTemplate->GetBWallUmaSpacerDiaType();
	m_nBWallUmaSpacerWallThick = pBWallRebarTemplate->GetBWallUmaSpacerWallThick();
	m_nBWallUmaSpacerWallThickOverDiaIndex = pBWallRebarTemplate->GetBWallUmaSpacerWallThickOverDiaIndex();
	m_nBWallUmaSpacerWallThickUnderDiaIndex = pBWallRebarTemplate->GetBWallUmaSpacerWallThickUnderDiaIndex();
	m_bBWallUmaSpacerUserSteelGradeS = pBWallRebarTemplate->GetBWallUmaSpacerUserSteelGradeS();
	m_nBWallUmaSpacerSpacingType = pBWallRebarTemplate->GetBWallUmaSpacerSpacingType();
	m_nBWallUmaSpacerWallSpacing = pBWallRebarTemplate->GetBWallUmaSpacerWallSpacing();
	m_nBWallUmaSpacerUserSpacing1 = pBWallRebarTemplate->GetBWallUmaSpacerUserSpacing1();
	m_nBWallUmaSpacerUserSpacing2 = pBWallRebarTemplate->GetBWallUmaSpacerUserSpacing2();
	m_nBWallUmaSpacerHeightType = pBWallRebarTemplate->GetBWallUmaSpacerHeightType();
	m_nBWallUmaSpacerUserHeight = pBWallRebarTemplate->GetBWallUmaSpacerUserHeight();
	m_nBWallUmaSpacerHookLenAType = pBWallRebarTemplate->GetBWallUmaSpacerHookLenAType();
	m_nBWallUmaSpacerMainBarHookLenA = pBWallRebarTemplate->GetBWallUmaSpacerMainBarHookLenA();
	m_nBWallUmaSpacerUserHookLenA = pBWallRebarTemplate->GetBWallUmaSpacerUserHookLenA();
	m_nBWallUmaSpacerHookLenBType = pBWallRebarTemplate->GetBWallUmaSpacerHookLenBType();
	m_nBWallUmaSpacerMainBarHookLenB = pBWallRebarTemplate->GetBWallUmaSpacerMainBarHookLenB();
	m_nBWallUmaSpacerUserHookLenB = pBWallRebarTemplate->GetBWallUmaSpacerUserHookLenB();
	m_nBWallUmaSpacerWidth = pBWallRebarTemplate->GetBWallUmaSpacerWidth();
	m_bBWallUShapeTopSpacer = pBWallRebarTemplate->GetBWallUShapeTopSpacer();
	m_bBWallUShapeBotSpacer = pBWallRebarTemplate->GetBWallUShapeBotSpacer();
	m_nBWallUShapeSpacerDiaType = pBWallRebarTemplate->GetBWallUShapeSpacerDiaType();
	m_nBWallUShapeSpacerDiaIndex = pBWallRebarTemplate->GetBWallUShapeSpacerDiaIndex();
	m_bBWallUShapeSpacerUserSteelGradeS = pBWallRebarTemplate->GetBWallUShapeSpacerUserSteelGradeS();
	m_nBWallUShapeSpacerHorSpacingType = pBWallRebarTemplate->GetBWallUShapeSpacerHorSpacingType();
	m_nBWallUShapeSpacerHorSpacing = pBWallRebarTemplate->GetBWallUShapeSpacerHorSpacing();
	m_nBWallUShapeSpacerHorSpacingRate = pBWallRebarTemplate->GetBWallUShapeSpacerHorSpacingRate();
	m_nBWallUShapeSpacerHorSpacingCount = pBWallRebarTemplate->GetBWallUShapeSpacerHorSpacingCount();
	m_nBWallUShapeSpacerLenType = pBWallRebarTemplate->GetBWallUShapeSpacerLenType();
	m_nBWallUShapeSpacerUserLen = pBWallRebarTemplate->GetBWallUShapeSpacerUserLen();

	m_bBWallOpeningAddBarDeductionCondition = pBWallRebarTemplate->GetBWallOpeningAddBarDeductionCondition();

	m_bBWallUmaSpacerConditionSet = pBWallRebarTemplate->GetBWallUmaSpacerConditionSet();
	m_nBWallUmaSpacerCondition = pBWallRebarTemplate->GetBWallUmaSpacerCondition();

	m_bBWallSpacerVerStartSpacingSet = pBWallRebarTemplate->GetBWallSpacerVerStartSpacingSet();
	m_nBWallSpacerVerStartSpacing = pBWallRebarTemplate->GetBWallSpacerVerStartSpacing();

	m_bBWallSpliceAddLengthSet = pBWallRebarTemplate->GetBWallSpliceAddLengthSet();
	m_nBWallSpliceAddLengthNumSplice = pBWallRebarTemplate->GetBWallSpliceAddLengthNumSplice();

	m_nBWallPlusCrossShapeType = pBWallRebarTemplate->GetBWallPlusCrossShapeType();
	m_nBWallTCrossShapeType = pBWallRebarTemplate->GetBWallTCrossShapeType();
	m_nBWallLCrossShapeType = pBWallRebarTemplate->GetBWallLCrossShapeType();

	m_nBWallStraightCenterUmaPlaceType = pBWallRebarTemplate->GetBWallStraightCenterUmaPlaceType();
}

void BWallRebarTemplate::Serialize(CArchive& ar)
{
	long nDefaultValue = 0;
	long AlterSpliceValue1 = 0, AlterSpliceValue2 = 0, DowelAlterSpliceValue1 = 0, DowelAlterSpliceValue2 = 0;

	long nBWallVerBarSpliceWay = 0, nBWallVerBarTopLenType = 0, nBWallDowelBarSpliceWay = 0, nBWallDowelBarTopLenType = 0,
		nBWallDowelSupportBarDiaIndex = 0, nBWallDowelSupportBarNum = 0, nBWallDowelBarAnchorType = 0,
		nBWallDowelHookAnchorPlaceSpacing = 0, nBWallVerBarTopFloorDevType = 0, nBWallVerBarConstructionHeight = 0,
		nBWallVerBarAlterSpliceDist = 0, nBWallDowelConstructionHeight = 0, nBWallDowelAlterSpliceDist = 0;

	BOOL bBWallVerBarAlterSpliceSet = FALSE, bBWallDowelAlterSpliceSet = FALSE, bBWallDowelHookAnchorPlaceSpacingSet = FALSE, bBWallUserDowelHookLen = FALSE;

	if(ar.IsStoring())
	{
		ar << nBWallVerBarSpliceWay;
		ar << nBWallVerBarTopLenType;
		ar << bBWallVerBarAlterSpliceSet;
		ar << AlterSpliceValue1;
		ar << AlterSpliceValue2;
		ar << m_nBWallVerNumBarCrossBarIncludeSet;
		ar << m_nBWallCrossBarEAType;
		ar << m_bBWallPlaceEndBarSet;
		ar << m_nBWallEndVerAddBarCount;
		ar << m_nBWallEndVerAddBarDist;
		
		map<long, long>::iterator it;

		ar << (int)mm_BWallAddBarDiaIndex.size();
		for (it = mm_BWallAddBarDiaIndex.begin(); it != mm_BWallAddBarDiaIndex.end(); it++)
		{
			long index1 = it->first;
			long index2 = it->second;
			ar << index1;
			ar << index2;
		}

		ar << (int)mm_BWallEndBarDiaIndex.size();
		for (it = mm_BWallEndBarDiaIndex.begin(); it != mm_BWallEndBarDiaIndex.end(); it++)
		{
			long index1 = it->first;
			long index2 = it->second;
			ar << index1;
			ar << index2;
		}

		ar << m_bBWallPlaceUBar4ShortWall;
		ar << m_nBWallMaxUbarLegLength;
		ar << m_bBWallPlaceStirrup4ShortWall;
		ar << m_nBWallHBarMaxStirrupLen;
		ar << m_bBWallHorBarStartSet;
		ar << m_nBWallHorBarStartLength;
		ar << m_bBWallDefaultUbarLen;
		ar << m_bBWallCrossBarUBarHalfPlace;
		ar << m_bBWallStraightEndUBarPlaceSet;

		ar << (int)mm_BWallDefaultUbarLen.size();
		for (it = mm_BWallDefaultUbarLen.begin(); it != mm_BWallDefaultUbarLen.end(); it++)
		{
			long index = it->first;
			long uBarLen = it->second;
			ar << index;
			ar << uBarLen;
		}

		ar << m_nBWallSpacerDiaType;
		ar << m_nBWallSpacerDiaIndex;
		ar << m_nBWallSpacerHorSpacing;
		ar << m_nBWallSpacerVerSpacingType;
		ar << m_nBWallSpacerVerSpacing;
		ar << m_nBWallSpacerVerSpacingRate;
		ar << m_nBWallSpacerVerSpacingCount;
		ar << m_nBWallDowelSpacerVerSpacingCount;
		ar << m_nBWallSpacerLenType;
		ar << m_nBWallSpacerLenCalWayA;
		ar << m_nBWallSpacerLenA;
		ar << m_nBWallSpacerLenB;
		ar << m_nBWallSpacerShape;
		ar << m_bBWallSpacerRebarPlaceSet;
		ar << m_nBWallSpacerRebarPlaceCondition;

		ar << nBWallDowelBarSpliceWay;
		ar << nBWallDowelBarTopLenType;
		ar << bBWallDowelAlterSpliceSet;
		ar << DowelAlterSpliceValue1;
		ar << DowelAlterSpliceValue2;
		ar << nBWallDowelSupportBarDiaIndex;
		ar << nBWallDowelSupportBarNum;
		ar << m_bBWallDowelSupportPlaceType;
		ar << nBWallDowelBarAnchorType;
		ar << bBWallDowelHookAnchorPlaceSpacingSet;
		ar << nBWallDowelHookAnchorPlaceSpacing;
		ar << bBWallUserDowelHookLen;

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

		ar << m_bBWallSupportRebarsSet;
		ar << m_nBWallSupportRebarsDiaIndex;
		ar << m_dBWallSupportRebarsTotalLengthRate;
		ar << m_nBWallSupportRebarsOrderLength;
		ar << m_nBWallSupportRebarsSpliceLength;
		ar << m_nBWallSupportRebarsCorrectiveLength;

		ar << m_bBWallAddBarsSet;
		ar << m_nBWallAddBarsDiaIndex;
		ar << m_nBWallAddBarsNumber;
		ar << m_nBWallAddBarsLengthType;
		ar << m_nBWallAddBarsTensionLengthType;
		ar << m_nBWallAddBarsUserLength;

		ar << m_bBWallShearBarSet;
		ar << m_nBWallShearBarDiaType;
		ar << m_nBWallShearBarDiaIndex;
		ar << m_nBWallShearBarUserDiaType;

		ar << (int)mm_BWallShearBarVerUserDiaIndex.size();
		for (it = mm_BWallShearBarVerUserDiaIndex.begin(); it != mm_BWallShearBarVerUserDiaIndex.end(); it++)
		{
			long index1 = it->first;
			long index2 = it->second;
			ar << index1;
			ar << index2;
		}

		ar << (int)mm_BWallShearBarHorUserDiaIndex.size();
		for (it = mm_BWallShearBarHorUserDiaIndex.begin(); it != mm_BWallShearBarHorUserDiaIndex.end(); it++)
		{
			long index1 = it->first;
			long index2 = it->second;
			ar << index1;
			ar << index2;
		}

		ar << m_nBWallShearBarShapeType;

		ar << m_nBWallShearBarVerSpacingCondition;
		ar << m_nBWallShearBarVerBelowType;
		ar << m_nBWallShearBarVerBelowSpacing;
		ar << m_nBWallShearBarVerBelowRatio;
		ar << m_nBWallShearBarVerBelowNum;
		ar << m_nBWallShearBarVerOverType;
		ar << m_nBWallShearBarVerOverSpacing;
		ar << m_nBWallShearBarVerOverRatio;
		ar << m_nBWallShearBarVerOverNum;

		ar << m_nBWallShearBarHorSpacingCondition;
		ar << m_nBWallShearBarHorBelowType;
		ar << m_nBWallShearBarHorBelowSpacing;
		ar << m_nBWallShearBarHorBelowRatio;
		ar << m_nBWallShearBarHorBelowNum;
		ar << m_nBWallShearBarHorOverType;
		ar << m_nBWallShearBarHorOverSpacing;
		ar << m_nBWallShearBarHorOverRatio;
		ar << m_nBWallShearBarHorOverNum;

		ar << m_bBWallSpacerVerEndBarPlaceSet;
		ar << m_nBWallSpacerVerEndBarSpacingType;
		ar << m_nBWallSpacerVerEndBarSpacing;
		ar << m_nBWallSpacerVerEndBarSpacingRate;
		ar << m_nBWallSpacerVerEndBarSpacingCount;

		ar << m_nBWallCrossBarStartSpacingType;
		ar << m_nBWallEndVerStartSpacingType;

		ar << nBWallVerBarTopFloorDevType;
		ar << nBWallVerBarConstructionHeight;
		ar << nBWallVerBarAlterSpliceDist;
		ar << nBWallDowelConstructionHeight;
		ar << nBWallDowelAlterSpliceDist;


		ar << m_bBWallPlaceBraceSupportBar;
		ar << m_nBWallBraceSupportBarOverDiaIndex;
		ar << m_nBWallBraceSupportBarWallLengthCondition;
		ar << m_nBWallBraceSupportBarWallLengthConditionOverLength;
		ar << m_nBWallBraceSupportBarWallLengthConditionUnderLength;
		ar << m_bBWallBraceSupportNotPlaceConditionDiaSet;
		ar << m_nBWallBraceSupportNotPlaceConditionDiaIndex;
		ar << m_bBWallBraceSupportNotPlaceConditionLengthSet;
		ar << m_nBWallBraceSupportNotPlaceConditionLength;
		ar << m_bBWallBraceSupportNotPlaceConditionBottomDowel;

		ar << m_nBWallVerBarPlaceTypeDiaIndex;
		m_pVerBarTemplateBelowDia->Serialize(ar);
		m_pVerBarTemplateExcessDia->Serialize(ar);
		ar << m_nBWallDowelPlaceTypeDiaIndex;
		m_pDowelTemplateBelowDia->Serialize(ar);
		m_pDowelTemplateExcessDia->Serialize(ar);

		ar << m_nBWallSpacerLenCalWayB;

		ar << m_bBWallCrossBarEATypeExcessThickSet;
		ar << m_nBWallCrossBarEATypeExcessThick;

		ar << m_bBWallDowelVerBarUserDiaSet;

		ar << (int)mm_BWallDowelVerBarUserDiaIndex.size();
		for(it = mm_BWallDowelVerBarUserDiaIndex.begin(); it != mm_BWallDowelVerBarUserDiaIndex.end(); it++)
		{
			long key = it->first;
			long value = it->second;
			ar << key;
			ar << value;
		}
				
		ar << m_bBWallUBarAddLengthSet;
		ar << m_nBWallUBarAddLength;
		ar << m_bBWallSpacerAddLengthSet;
		ar << m_nBWallSpacerAddLength;
		m_pBWallVerMemberLinkDowelTemplate->Serialize(ar);

		ar << m_bBWallBraceSupportBarUserSteelGradeS;
		ar << m_bBWallSpacerUserSteelGradeS;
		ar << m_bBWallSupportRebarsUserSteelGradeS;
		ar << m_bBWallAddBarsUserSteelGradeS;
		ar << m_bBWallShearBarUserSteelGradeS;

		ar << m_bBWallDowelVerBarUserSteelGradeS;

		ar << m_bBWallBraceSupportNotPlaceConditionHeightSet;
		ar << m_nBWallBraceSupportNotPlaceConditionHeight;

		ar << m_bBWallStraightCenterUmaShape;
		ar << m_nBWallUmaSpacerDiaType;
		ar << m_nBWallUmaSpacerWallThick;
		ar << m_nBWallUmaSpacerWallThickOverDiaIndex;
		ar << m_nBWallUmaSpacerWallThickUnderDiaIndex;
		ar << m_bBWallUmaSpacerUserSteelGradeS;
		ar << m_nBWallUmaSpacerSpacingType;
		ar << m_nBWallUmaSpacerWallSpacing;
		ar << m_nBWallUmaSpacerUserSpacing1;
		ar << m_nBWallUmaSpacerUserSpacing2;
		ar << m_nBWallUmaSpacerHeightType;
		ar << m_nBWallUmaSpacerUserHeight;
		ar << m_nBWallUmaSpacerHookLenAType;
		ar << m_nBWallUmaSpacerMainBarHookLenA;
		ar << m_nBWallUmaSpacerUserHookLenA;
		ar << m_nBWallUmaSpacerHookLenBType;
		ar << m_nBWallUmaSpacerMainBarHookLenB;
		ar << m_nBWallUmaSpacerUserHookLenB;
		ar << m_nBWallUmaSpacerWidth;
		ar << m_bBWallUShapeTopSpacer;
		ar << m_bBWallUShapeBotSpacer;
		ar << m_nBWallUShapeSpacerDiaType;
		ar << m_nBWallUShapeSpacerDiaIndex;
		ar << m_bBWallUShapeSpacerUserSteelGradeS;
		ar << m_nBWallUShapeSpacerHorSpacingType;
		ar << m_nBWallUShapeSpacerHorSpacing;
		ar << m_nBWallUShapeSpacerHorSpacingRate;
		ar << m_nBWallUShapeSpacerHorSpacingCount;
		ar << m_nBWallUShapeSpacerLenType;
		ar << m_nBWallUShapeSpacerUserLen;

		ar << m_bBWallOpeningAddBarDeductionCondition;

		ar << m_bBWallUmaSpacerConditionSet;
		ar << m_nBWallUmaSpacerCondition;

		ar << m_bBWallBraceSupportBarDiaConditionHeight;
		ar << m_nBWallBraceSupportBarUnderDiaIndex;

		ar << m_bBWallSpacerVerStartSpacingSet;
		ar << m_nBWallSpacerVerStartSpacing;

		ar << m_bBWallSpliceAddLengthSet;
		ar << m_nBWallSpliceAddLengthNumSplice;

		ar << m_nBWallPlusCrossShapeType;
		ar << m_nBWallTCrossShapeType;
		ar << m_nBWallLCrossShapeType;

		ar << m_nBWallStraightCenterUmaPlaceType;
	}
	else
	{
		ar >> nBWallVerBarSpliceWay;
		ar >> nBWallVerBarTopLenType;
		ar >> bBWallVerBarAlterSpliceSet;
		ar >> AlterSpliceValue1;
		ar >> AlterSpliceValue2;
		ar >> m_nBWallVerNumBarCrossBarIncludeSet;
		ar >> m_nBWallCrossBarEAType;
		ar >> m_bBWallPlaceEndBarSet;
		ar >> m_nBWallEndVerAddBarCount;
		ar >> m_nBWallEndVerAddBarDist;

		int NumObject;

		ar >> NumObject;
		if (NumObject > 0)
			mm_BWallAddBarDiaIndex.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex1 = 0, nDiaIndex2 = 0;

			ar >> nDiaIndex1;
			ar >> nDiaIndex2;
			mm_BWallAddBarDiaIndex.insert(make_pair(nDiaIndex1, nDiaIndex2));
		}

		ar >> NumObject;
		if (NumObject > 0)
			mm_BWallEndBarDiaIndex.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex1 = 0, nDiaIndex2 = 0;

			ar >> nDiaIndex1;
			ar >> nDiaIndex2;
			mm_BWallEndBarDiaIndex.insert(make_pair(nDiaIndex1, nDiaIndex2));
		}

		ar >> m_bBWallPlaceUBar4ShortWall;
		ar >> m_nBWallMaxUbarLegLength;
		ar >> m_bBWallPlaceStirrup4ShortWall;
		ar >> m_nBWallHBarMaxStirrupLen;
		ar >> m_bBWallHorBarStartSet;
		ar >> m_nBWallHorBarStartLength;
		ar >> m_bBWallDefaultUbarLen;
		ar >> m_bBWallCrossBarUBarHalfPlace;
		ar >> m_bBWallStraightEndUBarPlaceSet;

		ar >> NumObject;
		if (NumObject > 0)
			mm_BWallDefaultUbarLen.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, uBarLen = 0;

			ar >> nDiaIndex;
			ar >> uBarLen;
			mm_BWallDefaultUbarLen.insert(make_pair(nDiaIndex, uBarLen));
		}

		ar >> m_nBWallSpacerDiaType;
		ar >> m_nBWallSpacerDiaIndex;
		ar >> m_nBWallSpacerHorSpacing;
		ar >> m_nBWallSpacerVerSpacingType;
		ar >> m_nBWallSpacerVerSpacing;
		ar >> m_nBWallSpacerVerSpacingRate;
		ar >> m_nBWallSpacerVerSpacingCount;
		ar >> m_nBWallDowelSpacerVerSpacingCount;
		ar >> m_nBWallSpacerLenType;
		ar >> m_nBWallSpacerLenCalWayA;
		ar >> m_nBWallSpacerLenA;
		ar >> m_nBWallSpacerLenB;
		ar >> m_nBWallSpacerShape;
		ar >> m_bBWallSpacerRebarPlaceSet;
		ar >> m_nBWallSpacerRebarPlaceCondition;

		ar >> nBWallDowelBarSpliceWay;
		ar >> nBWallDowelBarTopLenType;
		ar >> bBWallDowelAlterSpliceSet;
		ar >> DowelAlterSpliceValue1;
		ar >> DowelAlterSpliceValue2;
		ar >> nBWallDowelSupportBarDiaIndex;
		ar >> nBWallDowelSupportBarNum;
		ar >> m_bBWallDowelSupportPlaceType;
		ar >> nBWallDowelBarAnchorType;
		ar >> bBWallDowelHookAnchorPlaceSpacingSet;
		ar >> nBWallDowelHookAnchorPlaceSpacing;
		ar >> bBWallUserDowelHookLen;

		ar >> NumObject;
		if (NumObject > 0)
			mm_BWallUserDowelHookLen.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_BWallUserDowelHookLen.insert(make_pair(nDiaIndex, hookLen));
		}

		ar >> NumObject;
		if (NumObject > 0)
			mm_BWallUserDowelSpliceLen.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, spliceLen = 0;

			ar >> nDiaIndex;
			ar >> spliceLen;
			mm_BWallUserDowelSpliceLen.insert(make_pair(nDiaIndex, spliceLen));
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20170822)
		{
			ar >> m_bBWallSupportRebarsSet;
			ar >> m_nBWallSupportRebarsDiaIndex;
			ar >> m_dBWallSupportRebarsTotalLengthRate;
			ar >> m_nBWallSupportRebarsOrderLength;
			ar >> m_nBWallSupportRebarsSpliceLength;
			ar >> m_nBWallSupportRebarsCorrectiveLength;

			ar >> m_bBWallAddBarsSet;
			ar >> m_nBWallAddBarsDiaIndex;
			ar >> m_nBWallAddBarsNumber;
			ar >> m_nBWallAddBarsLengthType;
			ar >> m_nBWallAddBarsTensionLengthType;
			ar >> m_nBWallAddBarsUserLength;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20180201)
		{
			ar >> m_bBWallShearBarSet;
			ar >> m_nBWallShearBarDiaType;
			ar >> m_nBWallShearBarDiaIndex;
			ar >> m_nBWallShearBarUserDiaType;

			ar >> NumObject;
			if (NumObject > 0)
				mm_BWallShearBarVerUserDiaIndex.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex1 = 0, nDiaIndex2 = 0;

				ar >> nDiaIndex1;
				ar >> nDiaIndex2;
				mm_BWallShearBarVerUserDiaIndex.insert(make_pair(nDiaIndex1, nDiaIndex2));
			}

			ar >> NumObject;
			if (NumObject > 0)
				mm_BWallShearBarHorUserDiaIndex.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex1 = 0, nDiaIndex2 = 0;

				ar >> nDiaIndex1;
				ar >> nDiaIndex2;
				mm_BWallShearBarHorUserDiaIndex.insert(make_pair(nDiaIndex1, nDiaIndex2));
			}

			ar >> m_nBWallShearBarShapeType;

			ar >> m_nBWallShearBarVerSpacingCondition;
			ar >> m_nBWallShearBarVerBelowType;
			ar >> m_nBWallShearBarVerBelowSpacing;
			ar >> m_nBWallShearBarVerBelowRatio;
			ar >> m_nBWallShearBarVerBelowNum;
			ar >> m_nBWallShearBarVerOverType;
			ar >> m_nBWallShearBarVerOverSpacing;
			ar >> m_nBWallShearBarVerOverRatio;
			ar >> m_nBWallShearBarVerOverNum;

			ar >> m_nBWallShearBarHorSpacingCondition;
			ar >> m_nBWallShearBarHorBelowType;
			ar >> m_nBWallShearBarHorBelowSpacing;
			ar >> m_nBWallShearBarHorBelowRatio;
			ar >> m_nBWallShearBarHorBelowNum;
			ar >> m_nBWallShearBarHorOverType;
			ar >> m_nBWallShearBarHorOverSpacing;
			ar >> m_nBWallShearBarHorOverRatio;
			ar >> m_nBWallShearBarHorOverNum;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20180308)
		{
			ar >> m_bBWallSpacerVerEndBarPlaceSet;
			ar >> m_nBWallSpacerVerEndBarSpacingType;
			ar >> m_nBWallSpacerVerEndBarSpacing;
			ar >> m_nBWallSpacerVerEndBarSpacingRate;
			ar >> m_nBWallSpacerVerEndBarSpacingCount;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20180417)
		{
			ar >> m_nBWallCrossBarStartSpacingType;
			ar >> m_nBWallEndVerStartSpacingType;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20180426)
		{
			ar >> nBWallVerBarTopFloorDevType;
			ar >> nBWallVerBarConstructionHeight;
			ar >> nBWallVerBarAlterSpliceDist;
			ar >> nBWallDowelConstructionHeight;
			ar >> nBWallDowelAlterSpliceDist;
		}
		else
		{
			nBWallVerBarTopFloorDevType = 0;
			nBWallVerBarConstructionHeight = min(AlterSpliceValue1, AlterSpliceValue2);
			nBWallVerBarAlterSpliceDist = abs(AlterSpliceValue1 - AlterSpliceValue2);
			nBWallDowelConstructionHeight = min(DowelAlterSpliceValue1, DowelAlterSpliceValue2);
			nBWallDowelAlterSpliceDist = abs(DowelAlterSpliceValue1 - DowelAlterSpliceValue2);
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20191001)
		{
			ar >> m_bBWallPlaceBraceSupportBar;
			ar >> m_nBWallBraceSupportBarOverDiaIndex;
			ar >> m_nBWallBraceSupportBarWallLengthCondition;
			ar >> m_nBWallBraceSupportBarWallLengthConditionOverLength;
			ar >> m_nBWallBraceSupportBarWallLengthConditionUnderLength;
			ar >> m_bBWallBraceSupportNotPlaceConditionDiaSet;
			ar >> m_nBWallBraceSupportNotPlaceConditionDiaIndex;
			ar >> m_bBWallBraceSupportNotPlaceConditionLengthSet;
			ar >> m_nBWallBraceSupportNotPlaceConditionLength;
			ar >> m_bBWallBraceSupportNotPlaceConditionBottomDowel;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20200110)
		{
			ar >> m_nBWallVerBarPlaceTypeDiaIndex;
			m_pVerBarTemplateBelowDia->Serialize(ar);
			m_pVerBarTemplateExcessDia->Serialize(ar);
			ar >> m_nBWallDowelPlaceTypeDiaIndex;
			m_pDowelTemplateBelowDia->Serialize(ar);
			m_pDowelTemplateExcessDia->Serialize(ar);
		}
		else
		{
			m_pVerBarTemplateBelowDia->SetBarSpliceType(nBWallVerBarSpliceWay);
			m_pVerBarTemplateBelowDia->SetTopLenType(nBWallVerBarTopLenType);
			m_pVerBarTemplateBelowDia->SetTopFloorDevType(nBWallVerBarTopFloorDevType);
			m_pVerBarTemplateBelowDia->SetConstructionHeight(nBWallVerBarConstructionHeight);
			m_pVerBarTemplateBelowDia->SetAlterSpliceSet(bBWallVerBarAlterSpliceSet);
			m_pVerBarTemplateBelowDia->SetAlterSpliceDist(nBWallVerBarAlterSpliceDist);

			m_pVerBarTemplateExcessDia->CopyFromMe(m_pVerBarTemplateBelowDia);

			m_pDowelTemplateBelowDia->SetBarSpliceType(nBWallDowelBarSpliceWay);
			m_pDowelTemplateBelowDia->SetTopLenType(nBWallDowelBarTopLenType);
			m_pDowelTemplateBelowDia->SetConstructionHeight(nBWallDowelConstructionHeight);
			m_pDowelTemplateBelowDia->SetAlterSpliceSet(bBWallDowelAlterSpliceSet);
			m_pDowelTemplateBelowDia->SetAlterSpliceDist(nBWallDowelAlterSpliceDist);
			m_pDowelTemplateBelowDia->SetSupportBarDiaIndex(nBWallDowelSupportBarDiaIndex);
			m_pDowelTemplateBelowDia->SetSupportBarNum(nBWallDowelSupportBarNum);
			m_pDowelTemplateBelowDia->SetBarAnchorType(nBWallDowelBarAnchorType);
			m_pDowelTemplateBelowDia->SetHookAnchorPlaceSpacingSet(bBWallDowelHookAnchorPlaceSpacingSet);
			m_pDowelTemplateBelowDia->SetHookAnchorPlaceSpacing(nBWallDowelHookAnchorPlaceSpacing);
			m_pDowelTemplateBelowDia->SetUserHookLenSet(bBWallUserDowelHookLen);

			m_pDowelTemplateExcessDia->CopyFromMe(m_pDowelTemplateBelowDia);
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20200530)
		{
			ar >> m_nBWallSpacerLenCalWayB;
		}
		else
		{
			m_nBWallSpacerLenCalWayB = m_nBWallSpacerLenCalWayA;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20200708)
		{
			ar >> m_bBWallCrossBarEATypeExcessThickSet;
			ar >> m_nBWallCrossBarEATypeExcessThick;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20200907)
		{
			ar >> m_bBWallDowelVerBarUserDiaSet;

			ar >> NumObject;
			if (NumObject > 0)
				mm_BWallDowelVerBarUserDiaIndex.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long key = 0, value = 0;
				ar >> key;
				ar >> value;
				mm_BWallDowelVerBarUserDiaIndex.insert(make_pair(key, value));
			}
		}
		if(MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231010)
		{
			ar >> m_bBWallUBarAddLengthSet;
			ar >> m_nBWallUBarAddLength;
			ar >> m_bBWallSpacerAddLengthSet;
			ar >> m_nBWallSpacerAddLength;
		
			m_pBWallVerMemberLinkDowelTemplate->Serialize(ar);
		
			ar >> m_bBWallBraceSupportBarUserSteelGradeS;
			ar >> m_bBWallSpacerUserSteelGradeS;
			ar >> m_bBWallSupportRebarsUserSteelGradeS;
			ar >> m_bBWallAddBarsUserSteelGradeS;
			ar >> m_bBWallShearBarUserSteelGradeS;

			ar >> m_bBWallDowelVerBarUserSteelGradeS;
		}
		if ((MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231226) || MSVersionInfo::IsOldTotalVersion())
		{
			ar >> m_bBWallBraceSupportNotPlaceConditionHeightSet;
			ar >> m_nBWallBraceSupportNotPlaceConditionHeight;
		}
		if(MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20240325)
		{
			ar >> m_bBWallStraightCenterUmaShape;
			ar >> m_nBWallUmaSpacerDiaType;
			ar >> m_nBWallUmaSpacerWallThick;
			ar >> m_nBWallUmaSpacerWallThickOverDiaIndex;
			ar >> m_nBWallUmaSpacerWallThickUnderDiaIndex;
			ar >> m_bBWallUmaSpacerUserSteelGradeS;
			ar >> m_nBWallUmaSpacerSpacingType;
			ar >> m_nBWallUmaSpacerWallSpacing;
			ar >> m_nBWallUmaSpacerUserSpacing1;
			ar >> m_nBWallUmaSpacerUserSpacing2;
			ar >> m_nBWallUmaSpacerHeightType;
			ar >> m_nBWallUmaSpacerUserHeight;
			ar >> m_nBWallUmaSpacerHookLenAType;
			ar >> m_nBWallUmaSpacerMainBarHookLenA;
			ar >> m_nBWallUmaSpacerUserHookLenA;
			ar >> m_nBWallUmaSpacerHookLenBType;
			ar >> m_nBWallUmaSpacerMainBarHookLenB;
			ar >> m_nBWallUmaSpacerUserHookLenB;
			ar >> m_nBWallUmaSpacerWidth;
			ar >> m_bBWallUShapeTopSpacer;
			ar >> m_bBWallUShapeBotSpacer;
			ar >> m_nBWallUShapeSpacerDiaType;
			ar >> m_nBWallUShapeSpacerDiaIndex;
			ar >> m_bBWallUShapeSpacerUserSteelGradeS;
			ar >> m_nBWallUShapeSpacerHorSpacingType;
			ar >> m_nBWallUShapeSpacerHorSpacing;
			ar >> m_nBWallUShapeSpacerHorSpacingRate;
			ar >> m_nBWallUShapeSpacerHorSpacingCount;
			ar >> m_nBWallUShapeSpacerLenType;
			ar >> m_nBWallUShapeSpacerUserLen;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20240416)
		{
			ar >> m_bBWallOpeningAddBarDeductionCondition;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20240625)
		{
			ar >> m_bBWallUmaSpacerConditionSet;
			ar >> m_nBWallUmaSpacerCondition;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20241018)
		{
			ar >> m_bBWallBraceSupportBarDiaConditionHeight;
			ar >> m_nBWallBraceSupportBarUnderDiaIndex;
			ar >> m_bBWallSpacerVerStartSpacingSet;
			ar >> m_nBWallSpacerVerStartSpacing;
		}
		else
		{
			m_nBWallBraceSupportBarUnderDiaIndex = m_nBWallBraceSupportBarOverDiaIndex;
		}

		
		if(MSVersionInfo::GetCurrentVersion() >= 20241018)
		{
			ar >> m_bBWallSpliceAddLengthSet;
			ar >> m_nBWallSpliceAddLengthNumSplice;
		}
		else
		{
			m_bBWallSpliceAddLengthSet = FALSE;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20241115)
		{
			ar >> m_nBWallPlusCrossShapeType;
			ar >> m_nBWallTCrossShapeType;
			ar >> m_nBWallLCrossShapeType;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20250214)
			ar >> m_nBWallStraightCenterUmaPlaceType;
	}
}
BWallVerBarRebarTemplate *BWallRebarTemplate::GetBWallVerBarTemplateBelowDia()
{
	return m_pVerBarTemplateBelowDia;
}
BWallVerBarRebarTemplate *BWallRebarTemplate::GetBWallVerBarTemplateExcessDia()
{
	return m_pVerBarTemplateExcessDia;
}
BWallDowelRebarTemplate *BWallRebarTemplate::GetBWallDowelTemplateBelowDia()
{
	return m_pDowelTemplateBelowDia;
}
BWallDowelRebarTemplate *BWallRebarTemplate::GetBWallDowelTemplateExcessDia()
{
	return m_pDowelTemplateExcessDia;
}
double BWallRebarTemplate::GetBWallAddBarDia(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);

	map<long, long>::iterator it = mm_BWallAddBarDiaIndex.find(DiaIndex);
	if (it == mm_BWallAddBarDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}

double BWallRebarTemplate::GetBWallEndBarDia(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);

	map<long, long>::iterator it = mm_BWallEndBarDiaIndex.find(DiaIndex);
	if (it == mm_BWallEndBarDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}

double BWallRebarTemplate::GetBWallDefaultUbarLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_BWallDefaultUbarLen.find(DiaIndex);
	if (it == mm_BWallDefaultUbarLen.end())
		return 0;
	return it->second;
}

double BWallRebarTemplate::GetBWallUserDowelHookLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_BWallUserDowelHookLen.find(DiaIndex);
	if (it == mm_BWallUserDowelHookLen.end())
		return 0;
	return it->second;
}

double BWallRebarTemplate::GetBWallUserDowelSpliceLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_BWallUserDowelSpliceLen.find(DiaIndex);
	if (it == mm_BWallUserDowelSpliceLen.end())
		return 0;
	return it->second;
}

double BWallRebarTemplate::GetBWallShearBarVerUserDia(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);

	map<long, long>::iterator it = mm_BWallShearBarVerUserDiaIndex.find(DiaIndex);
	if (it == mm_BWallShearBarVerUserDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}

double BWallRebarTemplate::GetBWallShearBarHorUserDia(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);

	map<long, long>::iterator it = mm_BWallShearBarHorUserDiaIndex.find(DiaIndex);
	if (it == mm_BWallShearBarHorUserDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}
double BWallRebarTemplate::GetBWallDowelVerBarUserDia(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);

	map<long, long>::iterator it = mm_BWallDowelVerBarUserDiaIndex.find(DiaIndex);
	if (it == mm_BWallDowelVerBarUserDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}
void BWallRebarTemplate::SetBWallAddBarDiaIndexMap(map<long, long> mapAddBarDiaIndex)
{
	mm_BWallAddBarDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapAddBarDiaIndex.begin(); it != mapAddBarDiaIndex.end(); it++)
	{
		mm_BWallAddBarDiaIndex.insert(make_pair(it->first, it->second));
	}
}

void BWallRebarTemplate::SetBWallEndBarDiaIndexMap(map<long, long> mapEndBarDiaIndex)
{
	mm_BWallEndBarDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapEndBarDiaIndex.begin(); it != mapEndBarDiaIndex.end(); it++)
	{
		mm_BWallEndBarDiaIndex.insert(make_pair(it->first, it->second));
	}

}
void BWallRebarTemplate::SetBWallDefaultUbarLenMap(map<long, long> mapDefaultUbarLen)
{
	mm_BWallDefaultUbarLen.clear();

	map<long, long>::iterator it;
	for (it = mapDefaultUbarLen.begin(); it != mapDefaultUbarLen.end(); it++)
	{
		mm_BWallDefaultUbarLen.insert(make_pair(it->first, it->second));
	}
}

void BWallRebarTemplate::SetBWallUserDowelHookLenMap(map<long, long> mapUserDowelHookLen)
{
	mm_BWallUserDowelHookLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserDowelHookLen.begin(); it != mapUserDowelHookLen.end(); it++)
	{
		mm_BWallUserDowelHookLen.insert(make_pair(it->first, it->second));
	}
}

void BWallRebarTemplate::SetBWallUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen)
{
	mm_BWallUserDowelSpliceLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserDowelSpliceLen.begin(); it != mapUserDowelSpliceLen.end(); it++)
	{
		mm_BWallUserDowelSpliceLen.insert(make_pair(it->first, it->second));
	}
}

void BWallRebarTemplate::SetBWallShearBarVerUserDiaIndexMap(map<long, long> mapShearBarVerUserDiaIndex)
{
	mm_BWallShearBarVerUserDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapShearBarVerUserDiaIndex.begin(); it != mapShearBarVerUserDiaIndex.end(); it++)
	{
		mm_BWallShearBarVerUserDiaIndex.insert(make_pair(it->first, it->second));
	}
}

void BWallRebarTemplate::SetBWallShearBarHorUserDiaIndexMap(map<long, long> mapShearBarHorUserDiaIndex)
{
	mm_BWallShearBarHorUserDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapShearBarHorUserDiaIndex.begin(); it != mapShearBarHorUserDiaIndex.end(); it++)
	{
		mm_BWallShearBarHorUserDiaIndex.insert(make_pair(it->first, it->second));
	}
}
void BWallRebarTemplate::SetBWallDowelVerBarUserDiaIndexMap(map<long, long> dowelVerBarUserDiaIndex)
{
	mm_BWallDowelVerBarUserDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = dowelVerBarUserDiaIndex.begin(); it != dowelVerBarUserDiaIndex.end(); it++)
	{
		mm_BWallDowelVerBarUserDiaIndex.insert(make_pair(it->first, it->second));
	}
}


long BWallRebarTemplate::GetBWallVerBarSpliceType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetBarSpliceType();
	else
		return m_pVerBarTemplateExcessDia->GetBarSpliceType();
}
long BWallRebarTemplate::GetBWallVerBarTopLenType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetTopLenType();
	else
		return m_pVerBarTemplateExcessDia->GetTopLenType();
}
long BWallRebarTemplate::GetBWallVerBarTopFloorDevType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetTopFloorDevType();
	else
		return m_pVerBarTemplateExcessDia->GetTopFloorDevType();
}
long BWallRebarTemplate::GetBWallVerBarConstructionHeight(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetConstructionHeight();
	else
		return m_pVerBarTemplateExcessDia->GetConstructionHeight();
}
BOOL BWallRebarTemplate::GetBWallVerBarAlterSpliceSet(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetAlterSpliceSet();
	else
		return m_pVerBarTemplateExcessDia->GetAlterSpliceSet();
}
long BWallRebarTemplate::GetBWallVerBarAlterSpliceDist(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetAlterSpliceDist();
	else
		return m_pVerBarTemplateExcessDia->GetAlterSpliceDist();
}
long BWallRebarTemplate::GetBWallVerBarAlterSpliceType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if (nDiaIndex <= m_nBWallVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetAlterSpliceType();
	else
		return m_pVerBarTemplateExcessDia->GetAlterSpliceType();
}
long BWallRebarTemplate::GetBWallDowelBarSpliceType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetBarSpliceType();
	else
		return m_pDowelTemplateExcessDia->GetBarSpliceType();
}
long BWallRebarTemplate::GetBWallDowelBarTopLenType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetTopLenType();
	else
		return m_pDowelTemplateExcessDia->GetTopLenType();
}
long BWallRebarTemplate::GetBWallDowelDowelShape(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if (nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetDowelShape();
	else
		return m_pDowelTemplateExcessDia->GetDowelShape();
}
long BWallRebarTemplate::GetBWallDowelConstructionHeight(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetConstructionHeight();
	else
		return m_pDowelTemplateExcessDia->GetConstructionHeight();
}
BOOL BWallRebarTemplate::GetBWallDowelAlterSpliceSet(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetAlterSpliceSet();
	else
		return m_pDowelTemplateExcessDia->GetAlterSpliceSet();
}
long BWallRebarTemplate::GetBWallDowelAlterSpliceDist(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetAlterSpliceDist();
	else
		return m_pDowelTemplateExcessDia->GetAlterSpliceDist();
}
long BWallRebarTemplate::GetBWallDowelAlterSpliceType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if (nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetAlterSpliceType();
	else
		return m_pDowelTemplateExcessDia->GetAlterSpliceType();
}
long BWallRebarTemplate::GetBWallDowelSupportBarDiaIndex(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarDiaIndex();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarDiaIndex();
}
BOOL BWallRebarTemplate::GetBWallDowelSupportBarUserSteelGradeS(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarUserSteelGradeS();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarUserSteelGradeS();
}
long BWallRebarTemplate::GetBWallDowelSupportBarNum(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarNum();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarNum();
}
long BWallRebarTemplate::GetBWallDowelBarAnchorType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetBarAnchorType();
	else
		return m_pDowelTemplateExcessDia->GetBarAnchorType();
}
BOOL BWallRebarTemplate::GetBWallDowelHookAnchorPlaceSpacingSet(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetHookAnchorPlaceSpacingSet();
	else
		return m_pDowelTemplateExcessDia->GetHookAnchorPlaceSpacingSet();
}
long BWallRebarTemplate::GetBWallDowelHookAnchorPlaceSpacing(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetHookAnchorPlaceSpacing();
	else
		return m_pDowelTemplateExcessDia->GetHookAnchorPlaceSpacing();
}
BOOL BWallRebarTemplate::GetBWallUserDowelHookLenSet(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetUserHookLenSet();
	else
		return m_pDowelTemplateExcessDia->GetUserHookLenSet();
}
BOOL BWallRebarTemplate::GetBWallDowelVerBarUserDiaSet()
{
	return m_bBWallDowelVerBarUserDiaSet;
}
void BWallRebarTemplate::SetBWallDowelVerBarUserDiaSet(BOOL value)
{
	m_bBWallDowelVerBarUserDiaSet = value;
}
VerMemberLinkDowelTemplate *BWallRebarTemplate::GetBWallVerMemberLinkDowelTemplate()
{
	return m_pBWallVerMemberLinkDowelTemplate;
}
BOOL BWallRebarTemplate::GetBWallDowelPlaceSpacer(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetPlaceSpacer();
	else
		return m_pDowelTemplateExcessDia->GetPlaceSpacer();
}
long BWallRebarTemplate::GetBWallDowelSpacerDiaIndex(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerDiaIndex();
	else
		return m_pDowelTemplateExcessDia->GetSpacerDiaIndex();
}
long BWallRebarTemplate::GetBWallDowelSpacerSpacing(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerSpacing();
	else
		return m_pDowelTemplateExcessDia->GetSpacerSpacing();
}
BOOL BWallRebarTemplate::GetBWallDowelSpacerUserSteelGradeS(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerUserSteelGradeS();
	else
		return m_pDowelTemplateExcessDia->GetSpacerUserSteelGradeS();
}
long BWallRebarTemplate::GetBWallDowelSpacerHeightType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerHeightType();
	else
		return m_pDowelTemplateExcessDia->GetSpacerHeightType();
}
long BWallRebarTemplate::GetBWallDowelSpacerHeightSpliceAddLength(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerHeightSpliceAddLength();
	else
		return m_pDowelTemplateExcessDia->GetSpacerHeightSpliceAddLength();
}
long BWallRebarTemplate::GetBWallDowelSpacerUserHeight(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerUserHeight();
	else
		return m_pDowelTemplateExcessDia->GetSpacerUserHeight();
}
long BWallRebarTemplate::GetBWallDowelSpacerLengthA(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerLengthA();
	else
		return m_pDowelTemplateExcessDia->GetSpacerLengthA();
}
long BWallRebarTemplate::GetBWallDowelSpacerLengthB(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerLengthB();
	else
		return m_pDowelTemplateExcessDia->GetSpacerLengthB();
}
long BWallRebarTemplate::GetBWallDowelSpacerWidthType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerWidthType();
	else
		return m_pDowelTemplateExcessDia->GetSpacerWidthType();
}
long BWallRebarTemplate::GetBWallDowelSpacerUserWidth(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nBWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerUserWidth();
	else
		return m_pDowelTemplateExcessDia->GetSpacerUserWidth();
}