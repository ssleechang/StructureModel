#include "StdAfx.h"
#include "MSObject.h"
#include "RWallRebarTemplate.h"
#include "MSDeformedBar.h"
#include "..\GMLib\MSVersionInfo.h"
#include "RWallVerBarRebarTemplate.h"
#include "RWallDowelRebarTemplate.h"
#include "VerMemberLinkDowelTemplate.h"

RWallRebarTemplate::RWallRebarTemplate() :BaseRebarTemplate()
{
	DataInit();
	m_pVerBarTemplateBelowDia = new RWallVerBarRebarTemplate();
	m_pVerBarTemplateExcessDia = new RWallVerBarRebarTemplate();
	m_pDowelTemplateBelowDia = new RWallDowelRebarTemplate();
	m_pDowelTemplateExcessDia = new RWallDowelRebarTemplate();
	m_pRWallVerMemberLinkDowelTemplate = new VerMemberLinkDowelTemplate();
}

RWallRebarTemplate::~RWallRebarTemplate(void)
{
	delete m_pVerBarTemplateBelowDia;
	delete m_pVerBarTemplateExcessDia;
	delete m_pDowelTemplateBelowDia;
	delete m_pDowelTemplateExcessDia;
	delete m_pRWallVerMemberLinkDowelTemplate;
}

void RWallRebarTemplate::DataInit()
{
	CStringArray BarDiaArr;
	MSDeformedBar::GetRebarNameList(BarDiaArr);

	m_nRWallVerBarPlaceTypeDiaIndex = BarDiaArr.GetSize() - 1;
	m_nRWallDowelPlaceTypeDiaIndex = BarDiaArr.GetSize() - 1;


	m_bRWallHorInfoSet = false;
	m_bRWallHorBarStartSet = false;
	m_nRWallHorBarStartLength = 50;
	m_nRWallEndUbar = 0;
	m_bRWallCrossEndUBarPlace = false;
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
	m_nRWallSpacerLenCalWayA = 0;
	m_nRWallSpacerLenCalWayB = 0;
	m_nRWallSpacerLenA = 350;
	m_nRWallSpacerLenB = 350;
	m_nRWallSpacerShape = 0;
	m_bRWallSpacerRebarPlaceSet = true;
	m_nRWallSpacerRebarPlaceCondition = 2000;

	m_bRWallSupportRebarsSet = false;
	m_nRWallSupportRebarsDiaIndex = 0;
	m_dRWallSupportRebarsTotalLengthRate = 2.0;
	m_nRWallSupportRebarsOrderLength = 8000;
	m_nRWallSupportRebarsSpliceLength = 300;
	m_nRWallSupportRebarsCorrectiveLength = 10;

	m_bRWallAddBarsSet = false;
	m_nRWallAddBarsDiaIndex = 0;
	m_nRWallAddBarsNumber = 4;
	m_nRWallAddBarsLengthType = 0;
	m_nRWallAddBarsTensionLengthType = 0;
	m_nRWallAddBarsUserLength = 1600;

	m_bRWallVerInfoSet = false;
	m_bRWallDowelInfoSet = false;
	m_bRWallDowelSupportPlaceType = false;
	
	InitDefaultUbarLenMap(mm_RWallDefaultUbarLen);
	InitUserDowelHookLenMap(mm_RWallUserDowelHookLen);
	InitUserDowelSpliceLenMap(mm_RWallUserDowelSpliceLen);

	m_nRWallShearBarShapeType = 0;	

	m_bRWallPlaceBraceSupportBar = FALSE;
	m_nRWallBraceSupportBarOverDiaIndex = 1;
	m_bRWallBraceSupportBarDiaConditionHeight = 3200;
	m_nRWallBraceSupportBarUnderDiaIndex = 1;
	m_nRWallBraceSupportBarWallLengthCondition = 8000;
	m_nRWallBraceSupportBarWallLengthConditionOverLength = 1;
	m_nRWallBraceSupportBarWallLengthConditionUnderLength = 0;
	m_bRWallBraceSupportNotPlaceConditionDiaSet = TRUE;
	m_nRWallBraceSupportNotPlaceConditionDiaIndex = 2;
	m_bRWallBraceSupportNotPlaceConditionLengthSet = TRUE;
	m_nRWallBraceSupportNotPlaceConditionLength = 4000;
	m_bRWallBraceSupportNotPlaceConditionBottomDowel = TRUE;

	m_bRWallBraceSupportNotPlaceConditionHeightSet = FALSE;
	m_nRWallBraceSupportNotPlaceConditionHeight = 3000;

	m_bRWallCrossBarUBarHalfPlace = FALSE;
	InitAddBarDiaIndexMap(mm_RWallCrossBarDiaIndex, true);

	m_bRWallPlaceCrossBar = FALSE;
	m_nRWallCrossBarEAType = 0;
	m_bRWallCrossBarEATypeExcessThickSet = FALSE;
	m_nRWallCrossBarEATypeExcessThick = 300;
	m_nRWallCrossBarStartSpacingType = 0;

	InitUserDiaIndexMap(mm_RWallDowelVerBarUserDiaIndex);

	m_bRWallDowelVerBarUserDiaSet = FALSE;

	m_bRWallUBarAddLengthSet = FALSE;
	m_nRWallUBarAddLength = 0;
	m_bRWallSpacerAddLengthSet = FALSE;
	m_nRWallSpacerAddLength = 0;
	m_bRWallBraceSupportBarUserSteelGradeS = FALSE;

	m_bRWallSpacerUserSteelGradeS = FALSE;
	m_bRWallSupportRebarsUserSteelGradeS = FALSE;
	m_bRWallAddBarsUserSteelGradeS = FALSE;	

	m_bRWallDowelVerBarUserSteelGradeS = FALSE;

	m_bRWallStraightCenterUmaShape = FALSE;
	m_nRWallUmaSpacerDiaType = 0;
	m_nRWallUmaSpacerWallThick = 700;
	m_nRWallUmaSpacerWallThickOverDiaIndex = 2;
	m_nRWallUmaSpacerWallThickUnderDiaIndex = 1;
	m_bRWallUmaSpacerUserSteelGradeS = FALSE;
	m_nRWallUmaSpacerSpacingType = 1;
	m_nRWallUmaSpacerWallSpacing = 3;
	m_nRWallUmaSpacerUserSpacing1 = 1500;
	m_nRWallUmaSpacerUserSpacing2 = 1500;
	m_nRWallUmaSpacerHeightType = 0;
	m_nRWallUmaSpacerUserHeight = 300;
	m_nRWallUmaSpacerHookLenAType = 1;
	m_nRWallUmaSpacerMainBarHookLenA = 100;
	m_nRWallUmaSpacerUserHookLenA = 100;
	m_nRWallUmaSpacerHookLenBType = 1;
	m_nRWallUmaSpacerMainBarHookLenB = 0;
	m_nRWallUmaSpacerUserHookLenB = 100;
	m_nRWallUmaSpacerWidth = 200;
	m_bRWallUShapeTopSpacer = FALSE;
	m_bRWallUShapeBotSpacer = FALSE;
	m_nRWallUShapeSpacerDiaType = 0;
	m_nRWallUShapeSpacerDiaIndex = 0;
	m_bRWallUShapeSpacerUserSteelGradeS = FALSE;
	m_nRWallUShapeSpacerHorSpacingType = 0;
	m_nRWallUShapeSpacerHorSpacing = 1500;
	m_nRWallUShapeSpacerHorSpacingRate = 3;
	m_nRWallUShapeSpacerHorSpacingCount = 3;
	m_nRWallUShapeSpacerLenType = 0;
	m_nRWallUShapeSpacerUserLen = 350;

	m_bRWallOpeningAddBarDeductionCondition = FALSE;

	m_bRWallUmaSpacerConditionSet = FALSE;
	m_nRWallUmaSpacerCondition = 5000;

	m_bRWallSpacerVerStartSpacingSet = FALSE;
	m_nRWallSpacerVerStartSpacing = 1500;

	m_bRWallSpliceAddLengthSet = TRUE;
	m_nRWallSpliceAddLengthNumSplice = 3;

	m_nRWallLCrossShapeType = 1;

	m_nRWallStraightCenterUmaPlaceType = 0;
}

void RWallRebarTemplate::CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate)
{
	RWallRebarTemplate* pRWallRebarTemplate = dynamic_cast<RWallRebarTemplate*>(pBaseRebarTemplate);
	if (pRWallRebarTemplate == nullptr) return;

	m_bRWallHorBarStartSet = pRWallRebarTemplate->m_bRWallHorBarStartSet;
	m_nRWallHorBarStartLength = pRWallRebarTemplate->m_nRWallHorBarStartLength;
	m_nRWallEndUbar = pRWallRebarTemplate->m_nRWallEndUbar;
	m_bRWallDefaultUbarLen = pRWallRebarTemplate->m_bRWallDefaultUbarLen;
	m_bRWallCrossEndUBarPlace = pRWallRebarTemplate->m_bRWallCrossEndUBarPlace;
	DefaultUbarLenMapDataCopy(mm_RWallDefaultUbarLen, pRWallRebarTemplate->mm_RWallDefaultUbarLen);

	m_nRWallSpacerDiaType = pRWallRebarTemplate->m_nRWallSpacerDiaType;
	m_nRWallSpacerDiaIndex = pRWallRebarTemplate->m_nRWallSpacerDiaIndex;
	m_nRWallSpacerHorSpacing = pRWallRebarTemplate->m_nRWallSpacerHorSpacing;
	m_nRWallSpacerVerSpacingType = pRWallRebarTemplate->m_nRWallSpacerVerSpacingType;
	m_nRWallSpacerVerSpacing = pRWallRebarTemplate->m_nRWallSpacerVerSpacing;
	m_nRWallSpacerVerSpacingRate = pRWallRebarTemplate->m_nRWallSpacerVerSpacingRate;
	m_nRWallSpacerVerSpacingCount = pRWallRebarTemplate->m_nRWallSpacerVerSpacingCount;
	m_nRWallDowelSpacerVerSpacingCount = pRWallRebarTemplate->m_nRWallDowelSpacerVerSpacingCount;
	m_nRWallSpacerLenType = pRWallRebarTemplate->m_nRWallSpacerLenType;
	m_nRWallSpacerLenCalWayA = pRWallRebarTemplate->m_nRWallSpacerLenCalWayA;
	m_nRWallSpacerLenCalWayB = pRWallRebarTemplate->m_nRWallSpacerLenCalWayB;
	m_nRWallSpacerLenA = pRWallRebarTemplate->m_nRWallSpacerLenA;
	m_nRWallSpacerLenB = pRWallRebarTemplate->m_nRWallSpacerLenB;
	m_nRWallSpacerShape = pRWallRebarTemplate->m_nRWallSpacerShape;
	m_bRWallSpacerRebarPlaceSet = pRWallRebarTemplate->m_bRWallSpacerRebarPlaceSet;
	m_nRWallSpacerRebarPlaceCondition = pRWallRebarTemplate->m_nRWallSpacerRebarPlaceCondition;

	m_bRWallSupportRebarsSet = pRWallRebarTemplate->m_bRWallSupportRebarsSet;
	m_nRWallSupportRebarsDiaIndex = pRWallRebarTemplate->m_nRWallSupportRebarsDiaIndex;
	m_dRWallSupportRebarsTotalLengthRate = pRWallRebarTemplate->m_dRWallSupportRebarsTotalLengthRate;
	m_nRWallSupportRebarsOrderLength = pRWallRebarTemplate->m_nRWallSupportRebarsOrderLength;
	m_nRWallSupportRebarsSpliceLength = pRWallRebarTemplate->m_nRWallSupportRebarsSpliceLength;
	m_nRWallSupportRebarsCorrectiveLength = pRWallRebarTemplate->m_nRWallSupportRebarsCorrectiveLength;

	m_bRWallAddBarsSet = pRWallRebarTemplate->m_bRWallAddBarsSet;
	m_nRWallAddBarsDiaIndex = pRWallRebarTemplate->m_nRWallAddBarsDiaIndex;
	m_nRWallAddBarsNumber = pRWallRebarTemplate->m_nRWallAddBarsNumber;
	m_nRWallAddBarsLengthType = pRWallRebarTemplate->m_nRWallAddBarsLengthType;
	m_nRWallAddBarsTensionLengthType = pRWallRebarTemplate->m_nRWallAddBarsTensionLengthType;
	m_nRWallAddBarsUserLength = pRWallRebarTemplate->m_nRWallAddBarsUserLength;

	m_bRWallDowelSupportPlaceType = pRWallRebarTemplate->m_bRWallDowelSupportPlaceType;
		
	UserDowelHookLenMapDataCopy(mm_RWallUserDowelHookLen, pRWallRebarTemplate->mm_RWallUserDowelHookLen);
	UserDowelSpliceLenMapDataCopy(mm_RWallUserDowelSpliceLen, pRWallRebarTemplate->mm_RWallUserDowelSpliceLen);

	m_nRWallShearBarShapeType = pRWallRebarTemplate->m_nRWallShearBarShapeType;


	m_nRWallVerBarPlaceTypeDiaIndex = pRWallRebarTemplate->GetRWallVerBarPlaceTypeDiaIndex();
	m_nRWallDowelPlaceTypeDiaIndex = pRWallRebarTemplate->GetRWallDowelPlaceTypeDiaIndex();

	m_pVerBarTemplateBelowDia->CopyFromMe(pRWallRebarTemplate->GetRWallVerBarTemplateBelowDia());
	m_pVerBarTemplateExcessDia->CopyFromMe(pRWallRebarTemplate->GetRWallVerBarTemplateExcessDia());

	m_pDowelTemplateBelowDia->CopyFromMe(pRWallRebarTemplate->GetRWallDowelTemplateBelowDia());
	m_pDowelTemplateExcessDia->CopyFromMe(pRWallRebarTemplate->GetRWallDowelTemplateExcessDia());

	m_bRWallPlaceBraceSupportBar = pRWallRebarTemplate->m_bRWallPlaceBraceSupportBar;								
	m_nRWallBraceSupportBarOverDiaIndex = pRWallRebarTemplate->m_nRWallBraceSupportBarOverDiaIndex;
	m_bRWallBraceSupportBarDiaConditionHeight = pRWallRebarTemplate->m_bRWallBraceSupportBarDiaConditionHeight;;
	m_nRWallBraceSupportBarUnderDiaIndex = pRWallRebarTemplate->m_nRWallBraceSupportBarUnderDiaIndex;;
	m_nRWallBraceSupportBarWallLengthCondition = pRWallRebarTemplate->m_nRWallBraceSupportBarWallLengthCondition;
	m_nRWallBraceSupportBarWallLengthConditionOverLength = pRWallRebarTemplate->m_nRWallBraceSupportBarWallLengthConditionOverLength;
	m_nRWallBraceSupportBarWallLengthConditionUnderLength = pRWallRebarTemplate->m_nRWallBraceSupportBarWallLengthConditionUnderLength;
	m_bRWallBraceSupportNotPlaceConditionDiaSet = pRWallRebarTemplate->m_bRWallBraceSupportNotPlaceConditionDiaSet;
	m_nRWallBraceSupportNotPlaceConditionDiaIndex = pRWallRebarTemplate->m_nRWallBraceSupportNotPlaceConditionDiaIndex;
	m_bRWallBraceSupportNotPlaceConditionLengthSet = pRWallRebarTemplate->m_bRWallBraceSupportNotPlaceConditionLengthSet;
	m_nRWallBraceSupportNotPlaceConditionLength = pRWallRebarTemplate->m_nRWallBraceSupportNotPlaceConditionLength;
	m_bRWallBraceSupportNotPlaceConditionBottomDowel = pRWallRebarTemplate->m_bRWallBraceSupportNotPlaceConditionBottomDowel;

	m_bRWallBraceSupportNotPlaceConditionHeightSet = pRWallRebarTemplate->m_bRWallBraceSupportNotPlaceConditionHeightSet;
	m_nRWallBraceSupportNotPlaceConditionHeight = pRWallRebarTemplate->m_nRWallBraceSupportNotPlaceConditionHeight;

	m_bRWallCrossBarUBarHalfPlace = pRWallRebarTemplate->m_bRWallCrossBarUBarHalfPlace;
	AddBarDiaIndexMapDataCopy(mm_RWallCrossBarDiaIndex, pRWallRebarTemplate->mm_RWallCrossBarDiaIndex);

	m_bRWallPlaceCrossBar = pRWallRebarTemplate->m_bRWallPlaceCrossBar;
	m_nRWallCrossBarEAType = pRWallRebarTemplate->m_nRWallCrossBarEAType;
	m_bRWallCrossBarEATypeExcessThickSet = pRWallRebarTemplate->m_bRWallCrossBarEATypeExcessThickSet;
	m_nRWallCrossBarEATypeExcessThick = pRWallRebarTemplate->m_nRWallCrossBarEATypeExcessThick;
	m_nRWallCrossBarStartSpacingType = pRWallRebarTemplate->m_nRWallCrossBarStartSpacingType;

	UserMapDataCopy(mm_RWallDowelVerBarUserDiaIndex, pRWallRebarTemplate->mm_RWallDowelVerBarUserDiaIndex);

	m_bRWallDowelVerBarUserDiaSet = pRWallRebarTemplate->GetRWallDowelVerBarUserDiaSet();

	m_pRWallVerMemberLinkDowelTemplate->CopyFromMe(pRWallRebarTemplate->GetRWallVerMemberLinkDowelTemplate());
	m_bRWallUBarAddLengthSet = pRWallRebarTemplate->GetRWallUBarAddLengthSet();
	m_nRWallUBarAddLength = pRWallRebarTemplate->GetRWallUBarAddLength();
	m_bRWallSpacerAddLengthSet = pRWallRebarTemplate->GetRWallSpacerAddLengthSet();
	m_nRWallSpacerAddLength = pRWallRebarTemplate->GetRWallSpacerAddLength();

	m_bRWallBraceSupportBarUserSteelGradeS = pRWallRebarTemplate->GetRWallBraceSupportBarUserSteelGradeS();

	m_bRWallSpacerUserSteelGradeS = pRWallRebarTemplate->GetRWallSpacerUserSteelGradeS();
	m_bRWallSupportRebarsUserSteelGradeS = pRWallRebarTemplate->GetRWallSupportRebarsUserSteelGradeS();
	m_bRWallAddBarsUserSteelGradeS = pRWallRebarTemplate->GetRWallAddBarsUserSteelGradeS();	

	m_bRWallDowelVerBarUserSteelGradeS = pRWallRebarTemplate->GetRWallDowelVerBarUserSteelGradeS();

	m_bRWallStraightCenterUmaShape = pRWallRebarTemplate->GetRWallStraightCenterUmaShape();
	m_nRWallUmaSpacerDiaType = pRWallRebarTemplate->GetRWallUmaSpacerDiaType();
	m_nRWallUmaSpacerWallThick = pRWallRebarTemplate->GetRWallUmaSpacerWallThick();
	m_nRWallUmaSpacerWallThickOverDiaIndex = pRWallRebarTemplate->GetRWallUmaSpacerWallThickOverDiaIndex();
	m_nRWallUmaSpacerWallThickUnderDiaIndex = pRWallRebarTemplate->GetRWallUmaSpacerWallThickUnderDiaIndex();
	m_bRWallUmaSpacerUserSteelGradeS = pRWallRebarTemplate->GetRWallUmaSpacerUserSteelGradeS();
	m_nRWallUmaSpacerSpacingType = pRWallRebarTemplate->GetRWallUmaSpacerSpacingType();
	m_nRWallUmaSpacerWallSpacing = pRWallRebarTemplate->GetRWallUmaSpacerWallSpacing();
	m_nRWallUmaSpacerUserSpacing1 = pRWallRebarTemplate->GetRWallUmaSpacerUserSpacing1();
	m_nRWallUmaSpacerUserSpacing2 = pRWallRebarTemplate->GetRWallUmaSpacerUserSpacing2();
	m_nRWallUmaSpacerHeightType = pRWallRebarTemplate->GetRWallUmaSpacerHeightType();
	m_nRWallUmaSpacerUserHeight = pRWallRebarTemplate->GetRWallUmaSpacerUserHeight();
	m_nRWallUmaSpacerHookLenAType = pRWallRebarTemplate->GetRWallUmaSpacerHookLenAType();
	m_nRWallUmaSpacerMainBarHookLenA = pRWallRebarTemplate->GetRWallUmaSpacerMainBarHookLenA();
	m_nRWallUmaSpacerUserHookLenA = pRWallRebarTemplate->GetRWallUmaSpacerUserHookLenA();
	m_nRWallUmaSpacerHookLenBType = pRWallRebarTemplate->GetRWallUmaSpacerHookLenBType();
	m_nRWallUmaSpacerMainBarHookLenB = pRWallRebarTemplate->GetRWallUmaSpacerMainBarHookLenB();
	m_nRWallUmaSpacerUserHookLenB = pRWallRebarTemplate->GetRWallUmaSpacerUserHookLenB();
	m_nRWallUmaSpacerWidth = pRWallRebarTemplate->GetRWallUmaSpacerWidth();
	m_bRWallUShapeTopSpacer = pRWallRebarTemplate->GetRWallUShapeTopSpacer();
	m_bRWallUShapeBotSpacer = pRWallRebarTemplate->GetRWallUShapeBotSpacer();
	m_nRWallUShapeSpacerDiaType = pRWallRebarTemplate->GetRWallUShapeSpacerDiaType();
	m_nRWallUShapeSpacerDiaIndex = pRWallRebarTemplate->GetRWallUShapeSpacerDiaIndex();
	m_bRWallUShapeSpacerUserSteelGradeS = pRWallRebarTemplate->GetRWallUShapeSpacerUserSteelGradeS();
	m_nRWallUShapeSpacerHorSpacingType = pRWallRebarTemplate->GetRWallUShapeSpacerHorSpacingType();
	m_nRWallUShapeSpacerHorSpacing = pRWallRebarTemplate->GetRWallUShapeSpacerHorSpacing();
	m_nRWallUShapeSpacerHorSpacingRate = pRWallRebarTemplate->GetRWallUShapeSpacerHorSpacingRate();
	m_nRWallUShapeSpacerHorSpacingCount = pRWallRebarTemplate->GetRWallUShapeSpacerHorSpacingCount();
	m_nRWallUShapeSpacerLenType = pRWallRebarTemplate->GetRWallUShapeSpacerLenType();
	m_nRWallUShapeSpacerUserLen = pRWallRebarTemplate->GetRWallUShapeSpacerUserLen();

	m_bRWallOpeningAddBarDeductionCondition = pRWallRebarTemplate->GetRWallOpeningAddBarDeductionCondition();

	m_bRWallUmaSpacerConditionSet = pRWallRebarTemplate->GetRWallUmaSpacerConditionSet();
	m_nRWallUmaSpacerCondition = pRWallRebarTemplate->GetRWallUmaSpacerCondition();

	m_bRWallSpacerVerStartSpacingSet = pRWallRebarTemplate->GetRWallSpacerVerStartSpacingSet();
	m_nRWallSpacerVerStartSpacing = pRWallRebarTemplate->GetRWallSpacerVerStartSpacing();


	m_bRWallSpliceAddLengthSet = pRWallRebarTemplate->GetRWallSpliceAddLengthSet();
	m_nRWallSpliceAddLengthNumSplice = pRWallRebarTemplate->GetRWallSpliceAddLengthNumSplice();

	m_nRWallLCrossShapeType = pRWallRebarTemplate->GetRWallLCrossShapeType();

	m_nRWallStraightCenterUmaPlaceType = pRWallRebarTemplate->GetRWallStraightCenterUmaPlaceType();
}

void RWallRebarTemplate::Serialize(CArchive& ar)
{
	long nDefaultValue = 0;
	long AlterSpliceValue1 = 0, AlterSpliceValue2 = 0, DowelAlterSpliceValue1 = 0, DowelAlterSpliceValue2 = 0;

	long nRWallVerBarTopLenType = 0, nRWallDowelBarAnchorType = 0, nRWallDowelBarTopLenType = 0, nRWallDowelSupportBarDiaIndex = 0, 
		nRWallDowelSupportBarNum = 0, nRWallVerBarSpliceWay = 0, nRWallDowelBarSpliceWay = 0, nRWallDowelHookAnchorPlaceSpacing = 0,
		nRWallVerBarTopFloorDevType = 0, nRWallVerBarConstructionHeight = 0, nRWallVerBarAlterSpliceDist = 0, nRWallDowelConstructionHeight = 0,
		nRWallDowelAlterSpliceDist = 0;
	BOOL bRWallUserDowelHookLen = FALSE, bRWallVerBarAlterSpliceSet = FALSE, bRWallDowelAlterSpliceSet = FALSE, 
		bRWallDowelHookAnchorPlaceSpacingSet = FALSE, bRWallDowelBotAddBarIntegratedPlaceSet = FALSE;
	

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
		ar << nRWallVerBarTopLenType;

		ar << m_bRWallDowelInfoSet;
		ar << bRWallUserDowelHookLen;
		ar << nRWallDowelBarAnchorType;
		ar << nRWallDowelBarTopLenType;
		ar << nRWallDowelSupportBarDiaIndex;
		ar << nRWallDowelSupportBarNum;
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
		
		ar << nRWallVerBarSpliceWay;
		ar << bRWallVerBarAlterSpliceSet;
		ar << AlterSpliceValue1;
		ar << AlterSpliceValue2;
		ar << nRWallDowelBarSpliceWay;
		ar << bRWallDowelAlterSpliceSet;
		ar << DowelAlterSpliceValue1;
		ar << DowelAlterSpliceValue2;
		ar << bRWallDowelHookAnchorPlaceSpacingSet;
		ar << nRWallDowelHookAnchorPlaceSpacing;
		ar << bRWallDowelBotAddBarIntegratedPlaceSet;

		ar << m_nRWallSpacerDiaType;
		ar << m_nRWallSpacerDiaIndex;
		ar << m_nRWallSpacerHorSpacing;
		ar << m_nRWallSpacerVerSpacingType;
		ar << m_nRWallSpacerVerSpacing;
		ar << m_nRWallSpacerVerSpacingRate;
		ar << m_nRWallSpacerVerSpacingCount;
		ar << m_nRWallDowelSpacerVerSpacingCount;
		ar << m_nRWallSpacerLenType;
		ar << m_nRWallSpacerLenCalWayA;
		ar << m_nRWallSpacerLenA;
		ar << m_nRWallSpacerLenB;
		ar << m_nRWallSpacerShape;
		ar << m_bRWallSpacerRebarPlaceSet;
		ar << m_nRWallSpacerRebarPlaceCondition;

		ar << m_bRWallSupportRebarsSet;
		ar << m_nRWallSupportRebarsDiaIndex;
		ar << m_dRWallSupportRebarsTotalLengthRate;
		ar << m_nRWallSupportRebarsOrderLength;
		ar << m_nRWallSupportRebarsSpliceLength;
		ar << m_nRWallSupportRebarsCorrectiveLength;

		ar << m_bRWallAddBarsSet;
		ar << m_nRWallAddBarsDiaIndex;
		ar << m_nRWallAddBarsNumber;
		ar << m_nRWallAddBarsLengthType;
		ar << m_nRWallAddBarsTensionLengthType;
		ar << m_nRWallAddBarsUserLength;

		ar << m_nRWallShearBarShapeType;

		ar << nRWallVerBarTopFloorDevType;
		ar << nRWallVerBarConstructionHeight;
		ar << nRWallVerBarAlterSpliceDist;
		ar << nRWallDowelConstructionHeight;
		ar << nRWallDowelAlterSpliceDist;

		ar << m_bRWallCrossEndUBarPlace;

		ar << m_nRWallVerBarPlaceTypeDiaIndex;
		m_pVerBarTemplateBelowDia->Serialize(ar);
		m_pVerBarTemplateExcessDia->Serialize(ar);
		ar << m_nRWallDowelPlaceTypeDiaIndex;
		m_pDowelTemplateBelowDia->Serialize(ar);
		m_pDowelTemplateExcessDia->Serialize(ar);

		ar << m_nRWallSpacerLenCalWayB;

		ar << m_bRWallPlaceBraceSupportBar;
		ar << m_nRWallBraceSupportBarOverDiaIndex;
		ar << m_nRWallBraceSupportBarWallLengthCondition;
		ar << m_nRWallBraceSupportBarWallLengthConditionOverLength;
		ar << m_nRWallBraceSupportBarWallLengthConditionUnderLength;
		ar << m_bRWallBraceSupportNotPlaceConditionDiaSet;
		ar << m_nRWallBraceSupportNotPlaceConditionDiaIndex;
		ar << m_bRWallBraceSupportNotPlaceConditionLengthSet;
		ar << m_nRWallBraceSupportNotPlaceConditionLength;
		ar << m_bRWallBraceSupportNotPlaceConditionBottomDowel;

		ar << m_bRWallPlaceCrossBar;
		ar << m_nRWallCrossBarEAType;
		ar << m_bRWallCrossBarEATypeExcessThickSet;
		ar << m_nRWallCrossBarEATypeExcessThick;
		ar << m_nRWallCrossBarStartSpacingType;

		ar << (int)mm_RWallCrossBarDiaIndex.size();
		for (it = mm_RWallCrossBarDiaIndex.begin(); it != mm_RWallCrossBarDiaIndex.end(); it++)
		{
			long nDiaIndex = it->first;
			long nDiaIndex2 = it->second;
			ar << nDiaIndex;
			ar << nDiaIndex2;
		}

		ar << m_bRWallDowelVerBarUserDiaSet;

		ar << (int)mm_RWallDowelVerBarUserDiaIndex.size();
		for(it = mm_RWallDowelVerBarUserDiaIndex.begin(); it != mm_RWallDowelVerBarUserDiaIndex.end(); it++)
		{
			long key = it->first;
			long value = it->second;
			ar << key;
			ar << value;
		}

		ar << m_bRWallCrossBarUBarHalfPlace;
				
		ar << m_bRWallUBarAddLengthSet;
		ar << m_nRWallUBarAddLength;
		ar << m_bRWallSpacerAddLengthSet;
		ar << m_nRWallSpacerAddLength;

		m_pRWallVerMemberLinkDowelTemplate->Serialize(ar);

		ar << m_bRWallBraceSupportBarUserSteelGradeS;
		ar << m_bRWallSpacerUserSteelGradeS;
		ar << m_bRWallSupportRebarsUserSteelGradeS;
		ar << m_bRWallAddBarsUserSteelGradeS;		

		ar << m_bRWallDowelVerBarUserSteelGradeS;

		ar << m_bRWallBraceSupportNotPlaceConditionHeightSet;
		ar << m_nRWallBraceSupportNotPlaceConditionHeight;

			   
		ar << m_bRWallStraightCenterUmaShape;
		ar << m_nRWallUmaSpacerDiaType;
		ar << m_nRWallUmaSpacerWallThick;
		ar << m_nRWallUmaSpacerWallThickOverDiaIndex;
		ar << m_nRWallUmaSpacerWallThickUnderDiaIndex;
		ar << m_bRWallUmaSpacerUserSteelGradeS;
		ar << m_nRWallUmaSpacerSpacingType;
		ar << m_nRWallUmaSpacerWallSpacing;
		ar << m_nRWallUmaSpacerUserSpacing1;
		ar << m_nRWallUmaSpacerUserSpacing2;
		ar << m_nRWallUmaSpacerHeightType;
		ar << m_nRWallUmaSpacerUserHeight;
		ar << m_nRWallUmaSpacerHookLenAType;
		ar << m_nRWallUmaSpacerMainBarHookLenA;
		ar << m_nRWallUmaSpacerUserHookLenA;
		ar << m_nRWallUmaSpacerHookLenBType;
		ar << m_nRWallUmaSpacerMainBarHookLenB;
		ar << m_nRWallUmaSpacerUserHookLenB;
		ar << m_nRWallUmaSpacerWidth;
		ar << m_bRWallUShapeTopSpacer;
		ar << m_bRWallUShapeBotSpacer;
		ar << m_nRWallUShapeSpacerDiaType;
		ar << m_nRWallUShapeSpacerDiaIndex;
		ar << m_bRWallUShapeSpacerUserSteelGradeS;
		ar << m_nRWallUShapeSpacerHorSpacingType;
		ar << m_nRWallUShapeSpacerHorSpacing;
		ar << m_nRWallUShapeSpacerHorSpacingRate;
		ar << m_nRWallUShapeSpacerHorSpacingCount;
		ar << m_nRWallUShapeSpacerLenType;
		ar << m_nRWallUShapeSpacerUserLen;

		ar << m_bRWallOpeningAddBarDeductionCondition;

		ar << m_bRWallUmaSpacerConditionSet;
		ar << m_nRWallUmaSpacerCondition;

		ar << m_bRWallBraceSupportBarDiaConditionHeight;
		ar << m_nRWallBraceSupportBarUnderDiaIndex;

		ar << m_bRWallSpacerVerStartSpacingSet;
		ar << m_nRWallSpacerVerStartSpacing;

		ar << m_bRWallSpliceAddLengthSet;
		ar << m_nRWallSpliceAddLengthNumSplice;

		ar << m_nRWallLCrossShapeType;

		ar << m_nRWallStraightCenterUmaPlaceType;
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
		ar >> nRWallVerBarTopLenType;

		ar >> m_bRWallDowelInfoSet;
		ar >> bRWallUserDowelHookLen;
		ar >> nRWallDowelBarAnchorType;
		ar >> nRWallDowelBarTopLenType;
		ar >> nRWallDowelSupportBarDiaIndex;
		ar >> nRWallDowelSupportBarNum;
		ar >> m_bRWallDowelSupportPlaceType;

		int NumObject;

		ar >> NumObject;
		if (NumObject > 0)
			mm_RWallDefaultUbarLen.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, uBarLen = 0;

			ar >> nDiaIndex;
			ar >> uBarLen;
			mm_RWallDefaultUbarLen.insert(make_pair(nDiaIndex, uBarLen));
		}

		ar >> NumObject;
		if (NumObject > 0)
			mm_RWallUserDowelHookLen.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_RWallUserDowelHookLen.insert(make_pair(nDiaIndex, hookLen));
		}

		ar >> NumObject;
		if (NumObject > 0)
			mm_RWallUserDowelSpliceLen.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, spliceLen = 0;

			ar >> nDiaIndex;
			ar >> spliceLen;
			mm_RWallUserDowelSpliceLen.insert(make_pair(nDiaIndex, spliceLen));
		}

		ar >> nRWallVerBarSpliceWay;
		ar >> bRWallVerBarAlterSpliceSet;
		ar >> AlterSpliceValue1;
		ar >> AlterSpliceValue2;
		ar >> nRWallDowelBarSpliceWay;
		ar >> bRWallDowelAlterSpliceSet;
		ar >> DowelAlterSpliceValue1;
		ar >> DowelAlterSpliceValue2;
		ar >> bRWallDowelHookAnchorPlaceSpacingSet;
		ar >> nRWallDowelHookAnchorPlaceSpacing;

		ar >> bRWallDowelBotAddBarIntegratedPlaceSet;

		ar >> m_nRWallSpacerDiaType;
		ar >> m_nRWallSpacerDiaIndex;
		ar >> m_nRWallSpacerHorSpacing;
		ar >> m_nRWallSpacerVerSpacingType;
		ar >> m_nRWallSpacerVerSpacing;
		ar >> m_nRWallSpacerVerSpacingRate;
		ar >> m_nRWallSpacerVerSpacingCount;
		ar >> m_nRWallDowelSpacerVerSpacingCount;
		ar >> m_nRWallSpacerLenType;
		ar >> m_nRWallSpacerLenCalWayA;
		ar >> m_nRWallSpacerLenA;
		ar >> m_nRWallSpacerLenB;
		ar >> m_nRWallSpacerShape;
		ar >> m_bRWallSpacerRebarPlaceSet;
		ar >> m_nRWallSpacerRebarPlaceCondition;

		m_bRWallDowelSupportPlaceType = false;

		if (MSVersionInfo::GetCurrentVersion() >= 20170822)
		{
			ar >> m_bRWallSupportRebarsSet;
			ar >> m_nRWallSupportRebarsDiaIndex;
			ar >> m_dRWallSupportRebarsTotalLengthRate;
			ar >> m_nRWallSupportRebarsOrderLength;
			ar >> m_nRWallSupportRebarsSpliceLength;
			ar >> m_nRWallSupportRebarsCorrectiveLength;

			ar >> m_bRWallAddBarsSet;
			ar >> m_nRWallAddBarsDiaIndex;
			ar >> m_nRWallAddBarsNumber;
			ar >> m_nRWallAddBarsLengthType;
			ar >> m_nRWallAddBarsTensionLengthType;
			ar >> m_nRWallAddBarsUserLength;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20180201)
		{
			ar >> m_nRWallShearBarShapeType;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20180426)
		{
			ar >> nRWallVerBarTopFloorDevType;
			ar >> nRWallVerBarConstructionHeight;
			ar >> nRWallVerBarAlterSpliceDist;
			ar >> nRWallDowelConstructionHeight;
			ar >> nRWallDowelAlterSpliceDist;
		}
		else
		{
			nRWallVerBarTopFloorDevType = 0;
			nRWallVerBarConstructionHeight = min(AlterSpliceValue1, AlterSpliceValue2);
			nRWallVerBarAlterSpliceDist = abs(AlterSpliceValue1 - AlterSpliceValue2);
			nRWallDowelConstructionHeight = min(DowelAlterSpliceValue1, DowelAlterSpliceValue2);
			nRWallDowelAlterSpliceDist = abs(DowelAlterSpliceValue1 - DowelAlterSpliceValue2);
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20191023)
		{
			ar >> m_bRWallCrossEndUBarPlace;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20200110)
		{
			ar >> m_nRWallVerBarPlaceTypeDiaIndex;
			m_pVerBarTemplateBelowDia->Serialize(ar);
			m_pVerBarTemplateExcessDia->Serialize(ar);
			ar >> m_nRWallDowelPlaceTypeDiaIndex;
			m_pDowelTemplateBelowDia->Serialize(ar);
			m_pDowelTemplateExcessDia->Serialize(ar);
		}
		else
		{
			m_pVerBarTemplateBelowDia->SetBarSpliceType(nRWallVerBarSpliceWay);
			m_pVerBarTemplateBelowDia->SetTopLenType(nRWallVerBarTopLenType);
			m_pVerBarTemplateBelowDia->SetTopFloorDevType(nRWallVerBarTopFloorDevType);
			m_pVerBarTemplateBelowDia->SetConstructionHeight(nRWallVerBarConstructionHeight);
			m_pVerBarTemplateBelowDia->SetAlterSpliceSet(bRWallVerBarAlterSpliceSet);
			m_pVerBarTemplateBelowDia->SetAlterSpliceDist(nRWallVerBarAlterSpliceDist);

			m_pVerBarTemplateExcessDia->CopyFromMe(m_pVerBarTemplateBelowDia);

			m_pDowelTemplateBelowDia->SetBarSpliceType(nRWallDowelBarSpliceWay);
			m_pDowelTemplateBelowDia->SetTopLenType(nRWallDowelBarTopLenType);
			m_pDowelTemplateBelowDia->SetConstructionHeight(nRWallDowelConstructionHeight);
			m_pDowelTemplateBelowDia->SetAlterSpliceSet(bRWallDowelAlterSpliceSet);
			m_pDowelTemplateBelowDia->SetAlterSpliceDist(nRWallDowelAlterSpliceDist);
			m_pDowelTemplateBelowDia->SetSupportBarDiaIndex(nRWallDowelSupportBarDiaIndex);
			m_pDowelTemplateBelowDia->SetSupportBarNum(nRWallDowelSupportBarNum);
			m_pDowelTemplateBelowDia->SetBarAnchorType(nRWallDowelBarAnchorType);
			m_pDowelTemplateBelowDia->SetHookAnchorPlaceSpacingSet(bRWallDowelHookAnchorPlaceSpacingSet);
			m_pDowelTemplateBelowDia->SetHookAnchorPlaceSpacing(nRWallDowelHookAnchorPlaceSpacing);
			m_pDowelTemplateBelowDia->SetUserHookLenSet(bRWallUserDowelHookLen);
			m_pDowelTemplateBelowDia->SetBotAddBarIntegratedPlaceSet(bRWallDowelBotAddBarIntegratedPlaceSet);

			m_pDowelTemplateExcessDia->CopyFromMe(m_pDowelTemplateBelowDia);
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20200530)
		{
			ar >> m_nRWallSpacerLenCalWayB;
		}
		else
		{
			m_nRWallSpacerLenCalWayB = m_nRWallSpacerLenCalWayA;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20200531)
		{
			ar >> m_bRWallPlaceBraceSupportBar;
			ar >> m_nRWallBraceSupportBarOverDiaIndex;
			ar >> m_nRWallBraceSupportBarWallLengthCondition;
			ar >> m_nRWallBraceSupportBarWallLengthConditionOverLength;
			ar >> m_nRWallBraceSupportBarWallLengthConditionUnderLength;
			ar >> m_bRWallBraceSupportNotPlaceConditionDiaSet;
			ar >> m_nRWallBraceSupportNotPlaceConditionDiaIndex;
			ar >> m_bRWallBraceSupportNotPlaceConditionLengthSet;
			ar >> m_nRWallBraceSupportNotPlaceConditionLength;
			ar >> m_bRWallBraceSupportNotPlaceConditionBottomDowel;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20200714)
		{
			ar >> m_bRWallPlaceCrossBar;
			ar >> m_nRWallCrossBarEAType;
			ar >> m_bRWallCrossBarEATypeExcessThickSet;
			ar >> m_nRWallCrossBarEATypeExcessThick;
			ar >> m_nRWallCrossBarStartSpacingType;

			int NumObject;
			ar >> NumObject;
			if (NumObject > 0)
				mm_RWallCrossBarDiaIndex.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex1 = 0, nDiaIndex2 = 0;

				ar >> nDiaIndex1;
				ar >> nDiaIndex2;
				mm_RWallCrossBarDiaIndex.insert(make_pair(nDiaIndex1, nDiaIndex2));
			}
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20200907)
		{
			ar >> m_bRWallDowelVerBarUserDiaSet;

			ar >> NumObject;
			if (NumObject > 0)
				mm_RWallDowelVerBarUserDiaIndex.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long key = 0, value = 0;
				ar >> key;
				ar >> value;
				mm_RWallDowelVerBarUserDiaIndex.insert(make_pair(key, value));
			}
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20200908)
		{
			ar >> m_bRWallCrossBarUBarHalfPlace;
		}
		if (MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231010)
		{
			ar >> m_bRWallUBarAddLengthSet;
			ar >> m_nRWallUBarAddLength;
			ar >> m_bRWallSpacerAddLengthSet;
			ar >> m_nRWallSpacerAddLength;

			m_pRWallVerMemberLinkDowelTemplate->Serialize(ar);

			ar >> m_bRWallBraceSupportBarUserSteelGradeS;
			ar >> m_bRWallSpacerUserSteelGradeS;
			ar >> m_bRWallSupportRebarsUserSteelGradeS;
			ar >> m_bRWallAddBarsUserSteelGradeS;

			ar >> m_bRWallDowelVerBarUserSteelGradeS;
		}
		if ((MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231226) || MSVersionInfo::IsOldTotalVersion())
		{
			ar >> m_bRWallBraceSupportNotPlaceConditionHeightSet;
			ar >> m_nRWallBraceSupportNotPlaceConditionHeight;
		}
		if(MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20240325)
		{
			ar >> m_bRWallStraightCenterUmaShape;
			ar >> m_nRWallUmaSpacerDiaType;
			ar >> m_nRWallUmaSpacerWallThick;
			ar >> m_nRWallUmaSpacerWallThickOverDiaIndex;
			ar >> m_nRWallUmaSpacerWallThickUnderDiaIndex;
			ar >> m_bRWallUmaSpacerUserSteelGradeS;
			ar >> m_nRWallUmaSpacerSpacingType;
			ar >> m_nRWallUmaSpacerWallSpacing;
			ar >> m_nRWallUmaSpacerUserSpacing1;
			ar >> m_nRWallUmaSpacerUserSpacing2;
			ar >> m_nRWallUmaSpacerHeightType;
			ar >> m_nRWallUmaSpacerUserHeight;
			ar >> m_nRWallUmaSpacerHookLenAType;
			ar >> m_nRWallUmaSpacerMainBarHookLenA;
			ar >> m_nRWallUmaSpacerUserHookLenA;
			ar >> m_nRWallUmaSpacerHookLenBType;
			ar >> m_nRWallUmaSpacerMainBarHookLenB;
			ar >> m_nRWallUmaSpacerUserHookLenB;
			ar >> m_nRWallUmaSpacerWidth;
			ar >> m_bRWallUShapeTopSpacer;
			ar >> m_bRWallUShapeBotSpacer;
			ar >> m_nRWallUShapeSpacerDiaType;
			ar >> m_nRWallUShapeSpacerDiaIndex;
			ar >> m_bRWallUShapeSpacerUserSteelGradeS;
			ar >> m_nRWallUShapeSpacerHorSpacingType;
			ar >> m_nRWallUShapeSpacerHorSpacing;
			ar >> m_nRWallUShapeSpacerHorSpacingRate;
			ar >> m_nRWallUShapeSpacerHorSpacingCount;
			ar >> m_nRWallUShapeSpacerLenType;
			ar >> m_nRWallUShapeSpacerUserLen;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20240416)
		{
			ar >> m_bRWallOpeningAddBarDeductionCondition;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20240625)
		{
			ar >> m_bRWallUmaSpacerConditionSet;
			ar >> m_nRWallUmaSpacerCondition;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20241018)
		{
			ar >> m_bRWallBraceSupportBarDiaConditionHeight;
			ar >> m_nRWallBraceSupportBarUnderDiaIndex;
			ar >> m_bRWallSpacerVerStartSpacingSet;
			ar >> m_nRWallSpacerVerStartSpacing;
		}
		else
		{
			m_nRWallBraceSupportBarUnderDiaIndex = m_nRWallBraceSupportBarOverDiaIndex;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20241018)
		{
			ar >> m_bRWallSpliceAddLengthSet;
			ar >> m_nRWallSpliceAddLengthNumSplice;
		}
		else
		{
			m_bRWallSpliceAddLengthSet = FALSE;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20241115)
		{
			ar >> m_nRWallLCrossShapeType;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20250214)
			ar >> m_nRWallStraightCenterUmaPlaceType;
	}
}

RWallVerBarRebarTemplate *RWallRebarTemplate::GetRWallVerBarTemplateBelowDia()
{
	return m_pVerBarTemplateBelowDia;
}
RWallVerBarRebarTemplate *RWallRebarTemplate::GetRWallVerBarTemplateExcessDia()
{
	return m_pVerBarTemplateExcessDia;
}
RWallDowelRebarTemplate *RWallRebarTemplate::GetRWallDowelTemplateBelowDia()
{
	return m_pDowelTemplateBelowDia;
}
RWallDowelRebarTemplate *RWallRebarTemplate::GetRWallDowelTemplateExcessDia()
{
	return m_pDowelTemplateExcessDia;
}
double RWallRebarTemplate::GetRWallDefaultUbarLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_RWallDefaultUbarLen.find(DiaIndex);
	if (it == mm_RWallDefaultUbarLen.end())
		return 0;
	return it->second;
}
double RWallRebarTemplate::GetRWallUserDowelHookLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_RWallUserDowelHookLen.find(DiaIndex);
	if (it == mm_RWallUserDowelHookLen.end())
		return 0;
	return it->second;
}
double RWallRebarTemplate::GetRWallUserDowelSpliceLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_RWallUserDowelSpliceLen.find(DiaIndex);
	if (it == mm_RWallUserDowelSpliceLen.end())
		return 0;
	return it->second;
}
double RWallRebarTemplate::GetRWallDowelVerBarUserDia(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);

	map<long, long>::iterator it = mm_RWallDowelVerBarUserDiaIndex.find(DiaIndex);
	if (it == mm_RWallDowelVerBarUserDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}
double RWallRebarTemplate::GetRWallCrossBarDia(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);

	map<long, long>::iterator it = mm_RWallCrossBarDiaIndex.find(DiaIndex);
	if (it == mm_RWallCrossBarDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}
void RWallRebarTemplate::SetRWallDefaultUbarLenMap(map<long, long> mapDefaultUbarDia)
{
	mm_RWallDefaultUbarLen.clear();

	map<long, long>::iterator it;
	for (it = mapDefaultUbarDia.begin(); it != mapDefaultUbarDia.end(); it++)
	{
		mm_RWallDefaultUbarLen.insert(make_pair(it->first, it->second));
	}
}

void RWallRebarTemplate::SetRWallUserDowelHookLenMap(map<long, long> mapUserDowelHookLen)
{
	mm_RWallUserDowelHookLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserDowelHookLen.begin(); it != mapUserDowelHookLen.end(); it++)
	{
		mm_RWallUserDowelHookLen.insert(make_pair(it->first, it->second));
	}
}

void RWallRebarTemplate::SetRWallUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen)
{
	mm_RWallUserDowelSpliceLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserDowelSpliceLen.begin(); it != mapUserDowelSpliceLen.end(); it++)
	{
		mm_RWallUserDowelSpliceLen.insert(make_pair(it->first, it->second));
	}
}
void RWallRebarTemplate::SetRWallCrossBarDiaIndexMap(map<long, long> mapCrossBarDiaIndex)
{
	mm_RWallCrossBarDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapCrossBarDiaIndex.begin(); it != mapCrossBarDiaIndex.end(); it++)
	{
		mm_RWallCrossBarDiaIndex.insert(make_pair(it->first, it->second));
	}

}
void RWallRebarTemplate::SetRWallDowelVerBarUserDiaIndexMap(map<long, long> dowelVerBarUserDiaIndex)
{
	mm_RWallDowelVerBarUserDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = dowelVerBarUserDiaIndex.begin(); it != dowelVerBarUserDiaIndex.end(); it++)
	{
		mm_RWallDowelVerBarUserDiaIndex.insert(make_pair(it->first, it->second));
	}
}
long RWallRebarTemplate::GetRWallVerBarTopLenType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetTopLenType();
	else
		return m_pVerBarTemplateExcessDia->GetTopLenType();
}
long RWallRebarTemplate::GetRWallVerBarSpliceType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetBarSpliceType();
	else
		return m_pVerBarTemplateExcessDia->GetBarSpliceType();
}
long RWallRebarTemplate::GetRWallVerBarTopFloorDevType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetTopFloorDevType();
	else
		return m_pVerBarTemplateExcessDia->GetTopFloorDevType();
}
long RWallRebarTemplate::GetRWallVerBarConstructionHeight(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetConstructionHeight();
	else
		return m_pVerBarTemplateExcessDia->GetConstructionHeight();
}
BOOL RWallRebarTemplate::GetRWallVerBarAlterSpliceSet(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetAlterSpliceSet();
	else
		return m_pVerBarTemplateExcessDia->GetAlterSpliceSet();
}
long RWallRebarTemplate::GetRWallVerBarAlterSpliceDist(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetAlterSpliceDist();
	else
		return m_pVerBarTemplateExcessDia->GetAlterSpliceDist();
}
long RWallRebarTemplate::GetRWallVerBarAlterSpliceType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if (nDiaIndex <= m_nRWallVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetAlterSpliceType();
	else
		return m_pVerBarTemplateExcessDia->GetAlterSpliceType();
}
BOOL RWallRebarTemplate::GetRWallUserDowelHookLenSet(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetUserHookLenSet();
	else
		return m_pDowelTemplateExcessDia->GetUserHookLenSet();
}
long RWallRebarTemplate::GetRWallDowelBarAnchorType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetBarAnchorType();
	else
		return m_pDowelTemplateExcessDia->GetBarAnchorType();
}
long RWallRebarTemplate::GetRWallDowelBarTopLenType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetTopLenType();
	else
		return m_pDowelTemplateExcessDia->GetTopLenType();
}
long RWallRebarTemplate::GetRWallDowelDowelShape(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if (nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetDowelShape();
	else
		return m_pDowelTemplateExcessDia->GetDowelShape();
}
long RWallRebarTemplate::GetRWallDowelSupportBarDiaIndex(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarDiaIndex();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarDiaIndex();
}
BOOL RWallRebarTemplate::GetRWallDowelSupportBarUserSteelGradeS(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarUserSteelGradeS();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarUserSteelGradeS();
}
long RWallRebarTemplate::GetRWallDowelSupportBarNum(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarNum();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarNum();
}
long RWallRebarTemplate::GetRWallDowelBarSpliceType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetBarSpliceType();
	else
		return m_pDowelTemplateExcessDia->GetBarSpliceType();
}
long RWallRebarTemplate::GetRWallDowelConstructionHeight(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetConstructionHeight();
	else
		return m_pDowelTemplateExcessDia->GetConstructionHeight();
}
BOOL RWallRebarTemplate::GetRWallDowelAlterSpliceSet(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetAlterSpliceSet();
	else
		return m_pDowelTemplateExcessDia->GetAlterSpliceSet();
}
long RWallRebarTemplate::GetRWallDowelAlterSpliceDist(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetAlterSpliceDist();
	else
		return m_pDowelTemplateExcessDia->GetAlterSpliceDist();
}
long RWallRebarTemplate::GetRWallDowelAlterSpliceType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if (nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetAlterSpliceType();
	else
		return m_pDowelTemplateExcessDia->GetAlterSpliceType();
}
BOOL RWallRebarTemplate::GetRWallDowelHookAnchorPlaceSpacingSet(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetHookAnchorPlaceSpacingSet();
	else
		return m_pDowelTemplateExcessDia->GetHookAnchorPlaceSpacingSet();
}
long RWallRebarTemplate::GetRWallDowelHookAnchorPlaceSpacing(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetHookAnchorPlaceSpacing();
	else
		return m_pDowelTemplateExcessDia->GetHookAnchorPlaceSpacing();
}
BOOL RWallRebarTemplate::GetRWallDowelBotAddBarIntegratedPlaceSet(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetBotAddBarIntegratedPlaceSet();
	else
		return m_pDowelTemplateExcessDia->GetBotAddBarIntegratedPlaceSet();
}
long RWallRebarTemplate::GetRWallDowelBotAddBarIntegratedPlaceLengthConditionType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if (nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetBotAddBarIntegratedPlaceLengthConditionType();
	else
		return m_pDowelTemplateExcessDia->GetBotAddBarIntegratedPlaceLengthConditionType();
}
long RWallRebarTemplate::GetRWallDowelBotAddBarIntegratedPlaceLengthCondition(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if (nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetBotAddBarIntegratedPlaceLengthCondition();
	else
		return m_pDowelTemplateExcessDia->GetBotAddBarIntegratedPlaceLengthCondition();
}
BOOL RWallRebarTemplate::GetRWallDowelAdjustRatioSet(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if (nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetDowelAdjustRatioSet();
	else
		return m_pDowelTemplateExcessDia->GetDowelAdjustRatioSet();

}

int RWallRebarTemplate::GetRWallDowelAdjustRatio(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if (nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetDowelAdjustRatio();
	else
		return m_pDowelTemplateExcessDia->GetDowelAdjustRatio();

}

BOOL RWallRebarTemplate::GetRWallDowelVerBarUserDiaSet()
{
	return m_bRWallDowelVerBarUserDiaSet;
}
void RWallRebarTemplate::SetRWallDowelVerBarUserDiaSet(BOOL value)
{
	m_bRWallDowelVerBarUserDiaSet = value;
}

VerMemberLinkDowelTemplate *RWallRebarTemplate::GetRWallVerMemberLinkDowelTemplate()
{
	return m_pRWallVerMemberLinkDowelTemplate;
}
BOOL RWallRebarTemplate::GetRWallDowelPlaceSpacer(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetPlaceSpacer();
	else
		return m_pDowelTemplateExcessDia->GetPlaceSpacer();
}
long RWallRebarTemplate::GetRWallDowelSpacerDiaIndex(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerDiaIndex();
	else
		return m_pDowelTemplateExcessDia->GetSpacerDiaIndex();
}
long RWallRebarTemplate::GetRWallDowelSpacerSpacing(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerSpacing();
	else
		return m_pDowelTemplateExcessDia->GetSpacerSpacing();
}
BOOL RWallRebarTemplate::GetRWallDowelSpacerUserSteelGradeS(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerUserSteelGradeS();
	else
		return m_pDowelTemplateExcessDia->GetSpacerUserSteelGradeS();
}
long RWallRebarTemplate::GetRWallDowelSpacerHeightType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerHeightType();
	else
		return m_pDowelTemplateExcessDia->GetSpacerHeightType();
}
long RWallRebarTemplate::GetRWallDowelSpacerHeightSpliceAddLength(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerHeightSpliceAddLength();
	else
		return m_pDowelTemplateExcessDia->GetSpacerHeightSpliceAddLength();
}
long RWallRebarTemplate::GetRWallDowelSpacerUserHeight(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerUserHeight();
	else
		return m_pDowelTemplateExcessDia->GetSpacerUserHeight();
}
long RWallRebarTemplate::GetRWallDowelSpacerLengthA(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerLengthA();
	else
		return m_pDowelTemplateExcessDia->GetSpacerLengthA();
}
long RWallRebarTemplate::GetRWallDowelSpacerLengthB(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerLengthB();
	else
		return m_pDowelTemplateExcessDia->GetSpacerLengthB();
}
long RWallRebarTemplate::GetRWallDowelSpacerWidthType(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerWidthType();
	else
		return m_pDowelTemplateExcessDia->GetSpacerWidthType();
}
long RWallRebarTemplate::GetRWallDowelSpacerUserWidth(double dDia)
{
	int nDiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(nDiaIndex <= m_nRWallDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSpacerUserWidth();
	else
		return m_pDowelTemplateExcessDia->GetSpacerUserWidth();
}