#include "StdAfx.h"
#include "MSObject.h"
#include "SlabRebarTemplate.h"
#include "MSDeformedBar.h"
#include "..\GMLib\MSVersionInfo.h"
#include "SlabHorSupportRebarsTemplate.h"
#include "ShearFrictionBarTemplate.h"

SlabRebarTemplate::SlabRebarTemplate() :BaseRebarTemplate()
{
	DataInit();
	m_pPCSlabHorSupportRebarsTemplate = new SlabHorSupportRebarsTemplate();
	m_pPCIMTSlabHorSupportRebarsTemplate = new SlabHorSupportRebarsTemplate();
	m_pGeneralDeckHorSupportRebarsTemplate = new SlabHorSupportRebarsTemplate();
	m_pEmbeddedDeckHorSupportRebarsTemplate = new SlabHorSupportRebarsTemplate();
	m_pPCHCSSlabHorSupportRebarsTemplate = new SlabHorSupportRebarsTemplate();
	m_pPCRPSSlabHorSupportRebarsTemplate = new SlabHorSupportRebarsTemplate();
	m_pMatShearFrictionBarTemplate = new ShearFrictionBarTemplate();
	m_pFlatShearFrictionBarTemplate = new ShearFrictionBarTemplate();
}

SlabRebarTemplate::~SlabRebarTemplate(void)
{
	delete m_pPCSlabHorSupportRebarsTemplate;
	delete m_pPCIMTSlabHorSupportRebarsTemplate;
	delete m_pGeneralDeckHorSupportRebarsTemplate;
	delete m_pEmbeddedDeckHorSupportRebarsTemplate;
	delete m_pPCHCSSlabHorSupportRebarsTemplate;
	delete m_pPCRPSSlabHorSupportRebarsTemplate;

	delete m_pMatShearFrictionBarTemplate;
	delete m_pFlatShearFrictionBarTemplate;
}

void SlabRebarTemplate::CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate)
{
	SlabRebarTemplate* pSlabRebarTemplate = dynamic_cast<SlabRebarTemplate*>(pBaseRebarTemplate);
	if (pSlabRebarTemplate == nullptr) return;

	m_pPCSlabHorSupportRebarsTemplate->CopyFromMe(pBaseRebarTemplate->GetPCSlabHorSupportRebarsTemplate());
	m_pPCIMTSlabHorSupportRebarsTemplate->CopyFromMe(pBaseRebarTemplate->GetPCIMTSlabHorSupportRebarsTemplate());
	m_pGeneralDeckHorSupportRebarsTemplate->CopyFromMe(pBaseRebarTemplate->GetGeneralDeckHorSupportRebarsTemplate());
	m_pEmbeddedDeckHorSupportRebarsTemplate->CopyFromMe(pBaseRebarTemplate->GetEmbeddedDeckHorSupportRebarsTemplate());
	m_pPCHCSSlabHorSupportRebarsTemplate->CopyFromMe(pBaseRebarTemplate->GetPCHCSSlabHorSupportRebarsTemplate());
	m_pPCRPSSlabHorSupportRebarsTemplate->CopyFromMe(pBaseRebarTemplate->GetPCRPSSlabHorSupportRebarsTemplate());
	m_pMatShearFrictionBarTemplate->CopyFromMe(pBaseRebarTemplate->GetMatShearFrictionBarTemplate());
	m_pFlatShearFrictionBarTemplate->CopyFromMe(pBaseRebarTemplate->GetFlatShearFrictionBarTemplate());
	// MAT Slab
	m_nMatMaxDualHookLength = pSlabRebarTemplate->m_nMatMaxDualHookLength;
	m_nMatSpliceWay = pSlabRebarTemplate->m_nMatSpliceWay;
	m_nMatSpliceType = pSlabRebarTemplate->m_nMatSpliceType;
		
	m_nMatTopEndHookType = pSlabRebarTemplate->m_nMatTopEndHookType;
	m_nMatBotEndHookType = pSlabRebarTemplate->m_nMatBotEndHookType;
	m_nMatIsoFootDevType = pSlabRebarTemplate->m_nMatIsoFootDevType;

	m_nMatTopEndDevTypeUserLength = pSlabRebarTemplate->m_nMatTopEndDevTypeUserLength;
	m_nMatBotEndDevTypeUserLength = pSlabRebarTemplate->m_nMatBotEndDevTypeUserLength;
	m_nMatOpeningTopDevType = pSlabRebarTemplate->m_nMatOpeningTopDevType;
	m_nMatOpeningBotDevType = pSlabRebarTemplate->m_nMatOpeningBotDevType;

	m_nMatLevelDifPlaceShape = pSlabRebarTemplate->m_nMatLevelDifPlaceShape;
	m_nMatLevelDifHaunchPlaceType = pSlabRebarTemplate->m_nMatLevelDifHaunchPlaceType;

	m_bMatSupportRebarsSet = pSlabRebarTemplate->m_bMatSupportRebarsSet;
	m_nMatSupportRebarsDiaIndex = pSlabRebarTemplate->m_nMatSupportRebarsDiaIndex;
	m_nMatSupportRebarsLengthType = pSlabRebarTemplate->m_nMatSupportRebarsLengthType;
	m_dMatSupportRebarsRWallLengthRate = pSlabRebarTemplate->m_dMatSupportRebarsRWallLengthRate;
	m_dMatSupportRebarsSlabLengthRate = pSlabRebarTemplate->m_dMatSupportRebarsSlabLengthRate;
	m_nMatSupportRebarsOrderLength = pSlabRebarTemplate->m_nMatSupportRebarsOrderLength;
	m_nMatSupportRebarsSpliceLength = pSlabRebarTemplate->m_nMatSupportRebarsSpliceLength;
	m_nMatSupportRebarsCorrectiveLength = pSlabRebarTemplate->m_nMatSupportRebarsCorrectiveLength;

	m_nMatRebarTolDist = pSlabRebarTemplate->m_nMatRebarTolDist;
	m_nMatRebarTolLenDif = pSlabRebarTemplate->m_nMatRebarTolLenDif;

	m_bMatSpliceDevConditionSet = pSlabRebarTemplate->m_bMatSpliceDevConditionSet;
	m_nMatSpliceDevConditionLength = pSlabRebarTemplate->m_nMatSpliceDevConditionLength;

	m_bMatSpliceAddLengthSet = pSlabRebarTemplate->m_bMatSpliceAddLengthSet;
	m_nMatSpliceAddLengthNumSplice = pSlabRebarTemplate->m_nMatSpliceAddLengthNumSplice;

	m_bMatTopSpacerSet = pSlabRebarTemplate->m_bMatTopSpacerSet;
	m_nMatTopSpacerDiaType = pSlabRebarTemplate->m_nMatTopSpacerDiaType;
	m_nMatTopSpacerSlabThick = pSlabRebarTemplate->m_nMatTopSpacerSlabThick;
	m_nMatTopSpacerSlabThickOverDiaIndex = pSlabRebarTemplate->m_nMatTopSpacerSlabThickOverDiaIndex;
	m_nMatTopSpacerSlabThickUnderDiaIndex = pSlabRebarTemplate->m_nMatTopSpacerSlabThickUnderDiaIndex;
	m_nMatTopSpacerSpacingType = pSlabRebarTemplate->m_nMatTopSpacerSpacingType;
	m_nMatTopSpacerSlabSpacing = pSlabRebarTemplate->m_nMatTopSpacerSlabSpacing;
	m_nMatTopSpacerUserSpacing1 = pSlabRebarTemplate->m_nMatTopSpacerUserSpacing1;
	m_nMatTopSpacerUserSpacing2 = pSlabRebarTemplate->m_nMatTopSpacerUserSpacing2;
	m_nMatTopSpacerHeightType = pSlabRebarTemplate->m_nMatTopSpacerHeightType;
	m_nMatTopSpacerUserHeight = pSlabRebarTemplate->m_nMatTopSpacerUserHeight;
	m_nMatTopSpacerHookLenAType = pSlabRebarTemplate->m_nMatTopSpacerHookLenAType;
	m_nMatTopSpacerMainBarHookLenA = pSlabRebarTemplate->m_nMatTopSpacerMainBarHookLenA;
	m_nMatTopSpacerUserHookLenA = pSlabRebarTemplate->m_nMatTopSpacerUserHookLenA;
	m_nMatTopSpacerHookLenBType = pSlabRebarTemplate->m_nMatTopSpacerHookLenBType;
	m_nMatTopSpacerMainBarHookLenB = pSlabRebarTemplate->m_nMatTopSpacerMainBarHookLenB;
	m_nMatTopSpacerUserHookLenB = pSlabRebarTemplate->m_nMatTopSpacerUserHookLenB;
	m_nMatTopSpacerWidth = pSlabRebarTemplate->m_nMatTopSpacerWidth;

	m_bMatBotSpacerSet = pSlabRebarTemplate->m_bMatBotSpacerSet;
	m_nMatBotSpacerDiaType = pSlabRebarTemplate->m_nMatBotSpacerDiaType;
	m_nMatBotSpacerDiaIndex = pSlabRebarTemplate->m_nMatBotSpacerDiaIndex;
	m_nMatBotSpacerSpacingType = pSlabRebarTemplate->m_nMatBotSpacerSpacingType;
	m_nMatBotSpacerSlabSpacing = pSlabRebarTemplate->m_nMatBotSpacerSlabSpacing;
	m_nMatBotSpacerUserSpacing1 = pSlabRebarTemplate->m_nMatBotSpacerUserSpacing1;
	m_nMatBotSpacerUserSpacing2 = pSlabRebarTemplate->m_nMatBotSpacerUserSpacing2;
	m_nMatBotSpacerHeightType = pSlabRebarTemplate->m_nMatBotSpacerHeightType;
	m_nMatBotSpacerUserHeight = pSlabRebarTemplate->m_nMatBotSpacerUserHeight;
	m_nMatBotSpacerHookLenAType = pSlabRebarTemplate->m_nMatBotSpacerHookLenAType;
	m_nMatBotSpacerMainBarHookLenA = pSlabRebarTemplate->m_nMatBotSpacerMainBarHookLenA;
	m_nMatBotSpacerUserHookLenA = pSlabRebarTemplate->m_nMatBotSpacerUserHookLenA;
	m_nMatBotSpacerHookLenBType = pSlabRebarTemplate->m_nMatBotSpacerHookLenBType;
	m_nMatBotSpacerMainBarHookLenB = pSlabRebarTemplate->m_nMatBotSpacerMainBarHookLenB;
	m_nMatBotSpacerUserHookLenB = pSlabRebarTemplate->m_nMatBotSpacerUserHookLenB;
	m_nMatBotSpacerWidth = pSlabRebarTemplate->m_nMatBotSpacerWidth;

	m_bMatSpacerSupportBarsSet = pSlabRebarTemplate->m_bMatSpacerSupportBarsSet;
	m_bMatSpacerSupportBarsConditionSet = pSlabRebarTemplate->m_bMatSpacerSupportBarsConditionSet;
	m_nMatSpacerSupportBarsCondition = pSlabRebarTemplate->m_nMatSpacerSupportBarsCondition;
	m_nMatSpacerSupportBarsDiaType = pSlabRebarTemplate->m_nMatSpacerSupportBarsDiaType;
	m_nMatSpacerSupportBarsUserDiaIndex = pSlabRebarTemplate->m_nMatSpacerSupportBarsUserDiaIndex;
	m_nMatSpacerSupportBarsNumType = pSlabRebarTemplate->m_nMatSpacerSupportBarsNumType;
	m_dMatSpacerSupportBarsTopSpacerNums = pSlabRebarTemplate->m_dMatSpacerSupportBarsTopSpacerNums;
	m_nMatSpacerSupportBarsUserNums = pSlabRebarTemplate->m_nMatSpacerSupportBarsUserNums;
	m_nMatSpacerSupportBarsLengthType = pSlabRebarTemplate->m_nMatSpacerSupportBarsLengthType;
	m_dMatSpacerSupportBarsUserRatio = pSlabRebarTemplate->m_dMatSpacerSupportBarsUserRatio;
	m_nMatSpacerSupportBarsUserLength = pSlabRebarTemplate->m_nMatSpacerSupportBarsUserLength;

	m_nMatCZoneSpliceWay = pSlabRebarTemplate->m_nMatCZoneSpliceWay;
	m_nMatCZoneSpliceLength = pSlabRebarTemplate->m_nMatCZoneSpliceLength;
	// FLAT Slab
	m_nFlatMaxDualHookLength = pSlabRebarTemplate->m_nFlatMaxDualHookLength;
	m_nFlatSpliceWay = pSlabRebarTemplate->m_nFlatSpliceWay;
	m_nFlatSpliceType = pSlabRebarTemplate->m_nFlatSpliceType;
		
	m_nFlatTopEndHookType = pSlabRebarTemplate->m_nFlatTopEndHookType;
	m_nFlatBotEndHookType = pSlabRebarTemplate->m_nFlatBotEndHookType;
	m_nFlatIsoFootDevType = pSlabRebarTemplate->m_nFlatIsoFootDevType;
	m_nFlatSpliceDevSlabType = pSlabRebarTemplate->m_nFlatSpliceDevSlabType;

	m_nFlatTopEndDevTypeUserLength = pSlabRebarTemplate->m_nFlatTopEndDevTypeUserLength;
	m_nFlatBotEndDevTypeUserLength = pSlabRebarTemplate->m_nFlatBotEndDevTypeUserLength;
	m_nFlatOpeningTopDevType = pSlabRebarTemplate->m_nFlatOpeningTopDevType;
	m_nFlatOpeningBotDevType = pSlabRebarTemplate->m_nFlatOpeningBotDevType;

	m_nFlatLevelDifPlaceShape = pSlabRebarTemplate->m_nFlatLevelDifPlaceShape;
	m_nFlatLevelDifHaunchPlaceType = pSlabRebarTemplate->m_nFlatLevelDifHaunchPlaceType;
		
	m_nFlatRebarTolDist = pSlabRebarTemplate->m_nFlatRebarTolDist;
	m_nFlatRebarTolLenDif = pSlabRebarTemplate->m_nFlatRebarTolLenDif;

	m_bFlatSpliceDevConditionSet = pSlabRebarTemplate->m_bFlatSpliceDevConditionSet;
	m_nFlatSpliceDevConditionLength = pSlabRebarTemplate->m_nFlatSpliceDevConditionLength;

	m_bFlatSpliceAddLengthSet = pSlabRebarTemplate->m_bFlatSpliceAddLengthSet;
	m_nFlatSpliceAddLengthNumSplice = pSlabRebarTemplate->m_nFlatSpliceAddLengthNumSplice;

	m_bFlatTopSpacerSet = pSlabRebarTemplate->m_bFlatTopSpacerSet;
	m_nFlatTopSpacerDiaType = pSlabRebarTemplate->m_nFlatTopSpacerDiaType;
	m_nFlatTopSpacerSlabThick = pSlabRebarTemplate->m_nFlatTopSpacerSlabThick;
	m_nFlatTopSpacerSlabThickOverDiaIndex = pSlabRebarTemplate->m_nFlatTopSpacerSlabThickOverDiaIndex;
	m_nFlatTopSpacerSlabThickUnderDiaIndex = pSlabRebarTemplate->m_nFlatTopSpacerSlabThickUnderDiaIndex;
	m_nFlatTopSpacerSpacingType = pSlabRebarTemplate->m_nFlatTopSpacerSpacingType;
	m_nFlatTopSpacerSlabSpacing = pSlabRebarTemplate->m_nFlatTopSpacerSlabSpacing;
	m_nFlatTopSpacerUserSpacing1 = pSlabRebarTemplate->m_nFlatTopSpacerUserSpacing1;
	m_nFlatTopSpacerUserSpacing2 = pSlabRebarTemplate->m_nFlatTopSpacerUserSpacing2;
	m_nFlatTopSpacerHeightType = pSlabRebarTemplate->m_nFlatTopSpacerHeightType;
	m_nFlatTopSpacerUserHeight = pSlabRebarTemplate->m_nFlatTopSpacerUserHeight;
	m_nFlatTopSpacerHookLenAType = pSlabRebarTemplate->m_nFlatTopSpacerHookLenAType;
	m_nFlatTopSpacerMainBarHookLenA = pSlabRebarTemplate->m_nFlatTopSpacerMainBarHookLenA;
	m_nFlatTopSpacerUserHookLenA = pSlabRebarTemplate->m_nFlatTopSpacerUserHookLenA;
	m_nFlatTopSpacerHookLenBType = pSlabRebarTemplate->m_nFlatTopSpacerHookLenBType;
	m_nFlatTopSpacerMainBarHookLenB = pSlabRebarTemplate->m_nFlatTopSpacerMainBarHookLenB;
	m_nFlatTopSpacerUserHookLenB = pSlabRebarTemplate->m_nFlatTopSpacerUserHookLenB;
	m_nFlatTopSpacerWidth = pSlabRebarTemplate->m_nFlatTopSpacerWidth;

	m_bFlatBotSpacerSet = pSlabRebarTemplate->m_bFlatBotSpacerSet;
	m_nFlatBotSpacerDiaType = pSlabRebarTemplate->m_nFlatBotSpacerDiaType;
	m_nFlatBotSpacerDiaIndex = pSlabRebarTemplate->m_nFlatBotSpacerDiaIndex;
	m_nFlatBotSpacerSpacingType = pSlabRebarTemplate->m_nFlatBotSpacerSpacingType;
	m_nFlatBotSpacerSlabSpacing = pSlabRebarTemplate->m_nFlatBotSpacerSlabSpacing;
	m_nFlatBotSpacerUserSpacing1 = pSlabRebarTemplate->m_nFlatBotSpacerUserSpacing1;
	m_nFlatBotSpacerUserSpacing2 = pSlabRebarTemplate->m_nFlatBotSpacerUserSpacing2;
	m_nFlatBotSpacerHeightType = pSlabRebarTemplate->m_nFlatBotSpacerHeightType;
	m_nFlatBotSpacerUserHeight = pSlabRebarTemplate->m_nFlatBotSpacerUserHeight;
	m_nFlatBotSpacerHookLenAType = pSlabRebarTemplate->m_nFlatBotSpacerHookLenAType;
	m_nFlatBotSpacerMainBarHookLenA = pSlabRebarTemplate->m_nFlatBotSpacerMainBarHookLenA;
	m_nFlatBotSpacerUserHookLenA = pSlabRebarTemplate->m_nFlatBotSpacerUserHookLenA;
	m_nFlatBotSpacerHookLenBType = pSlabRebarTemplate->m_nFlatBotSpacerHookLenBType;
	m_nFlatBotSpacerMainBarHookLenB = pSlabRebarTemplate->m_nFlatBotSpacerMainBarHookLenB;
	m_nFlatBotSpacerUserHookLenB = pSlabRebarTemplate->m_nFlatBotSpacerUserHookLenB;
	m_nFlatBotSpacerWidth = pSlabRebarTemplate->m_nFlatBotSpacerWidth;

	m_bFlatSpacerSupportBarsSet = pSlabRebarTemplate->m_bFlatSpacerSupportBarsSet;
	m_bFlatSpacerSupportBarsConditionSet = pSlabRebarTemplate->m_bFlatSpacerSupportBarsConditionSet;
	m_nFlatSpacerSupportBarsCondition = pSlabRebarTemplate->m_nFlatSpacerSupportBarsCondition;
	m_nFlatSpacerSupportBarsDiaType = pSlabRebarTemplate->m_nFlatSpacerSupportBarsDiaType;
	m_nFlatSpacerSupportBarsUserDiaIndex = pSlabRebarTemplate->m_nFlatSpacerSupportBarsUserDiaIndex;
	m_nFlatSpacerSupportBarsNumType = pSlabRebarTemplate->m_nFlatSpacerSupportBarsNumType;
	m_dFlatSpacerSupportBarsTopSpacerNums = pSlabRebarTemplate->m_dFlatSpacerSupportBarsTopSpacerNums;
	m_nFlatSpacerSupportBarsUserNums = pSlabRebarTemplate->m_nFlatSpacerSupportBarsUserNums;
	m_nFlatSpacerSupportBarsLengthType = pSlabRebarTemplate->m_nFlatSpacerSupportBarsLengthType;
	m_dFlatSpacerSupportBarsUserRatio = pSlabRebarTemplate->m_dFlatSpacerSupportBarsUserRatio;
	m_nFlatSpacerSupportBarsUserLength = pSlabRebarTemplate->m_nFlatSpacerSupportBarsUserLength;

	m_nFlatCZoneSpliceWay = pSlabRebarTemplate->m_nFlatCZoneSpliceWay;
	m_nFlatCZoneSpliceLength = pSlabRebarTemplate->m_nFlatCZoneSpliceLength;
	// 사변지지 SLAB
	m_nSlabMaxDualHookLength = pSlabRebarTemplate->m_nSlabMaxDualHookLength;
	m_nSlabSpliceWay = pSlabRebarTemplate->m_nSlabSpliceWay;
	m_nSlabSpliceType = pSlabRebarTemplate->m_nSlabSpliceType;
		
	m_nSlabOneWayCutBarLenType = pSlabRebarTemplate->m_nSlabOneWayCutBarLenType;
	m_nSlabTwoWayCutBarLenType = pSlabRebarTemplate->m_nSlabTwoWayCutBarLenType;

	m_nSlabTopUnStructuredEndHookType = pSlabRebarTemplate->m_nSlabTopUnStructuredEndHookType;
	m_nSlabBotUnStructuredEndHookType = pSlabRebarTemplate->m_nSlabBotUnStructuredEndHookType;

	m_nSlabTopEndDevTypeUserLength = pSlabRebarTemplate->m_nSlabTopEndDevTypeUserLength;
	m_nSlabBotEndDevTypeUserLength = pSlabRebarTemplate->m_nSlabBotEndDevTypeUserLength;
	m_nSlabOpeningTopDevType = pSlabRebarTemplate->m_nSlabOpeningTopDevType;
	m_nSlabOpeningBotDevType = pSlabRebarTemplate->m_nSlabOpeningBotDevType;

	m_nSlabLevelDifPlaceShape = pSlabRebarTemplate->m_nSlabLevelDifPlaceShape;
	m_nSlabLevelDifHaunchPlaceType = pSlabRebarTemplate->m_nSlabLevelDifHaunchPlaceType;
		
	m_nSlabRebarTolDist = pSlabRebarTemplate->m_nSlabRebarTolDist;
	m_nSlabRebarTolLenDif = pSlabRebarTemplate->m_nSlabRebarTolLenDif;

	m_bSlabSpliceDevConditionSet = pSlabRebarTemplate->m_bSlabSpliceDevConditionSet;
	m_nSlabSpliceDevConditionLength = pSlabRebarTemplate->m_nSlabSpliceDevConditionLength;

	m_bSlabSpliceAddLengthSet = pSlabRebarTemplate->m_bSlabSpliceAddLengthSet;
	m_nSlabSpliceAddLengthNumSplice = pSlabRebarTemplate->m_nSlabSpliceAddLengthNumSplice;

	m_bSlabTopSpacerSet = pSlabRebarTemplate->m_bSlabTopSpacerSet;
	m_nSlabTopSpacerDiaType = pSlabRebarTemplate->m_nSlabTopSpacerDiaType;
	m_nSlabTopSpacerSlabThick = pSlabRebarTemplate->m_nSlabTopSpacerSlabThick;
	m_nSlabTopSpacerSlabThickOverDiaIndex = pSlabRebarTemplate->m_nSlabTopSpacerSlabThickOverDiaIndex;
	m_nSlabTopSpacerSlabThickUnderDiaIndex = pSlabRebarTemplate->m_nSlabTopSpacerSlabThickUnderDiaIndex;
	m_nSlabTopSpacerSpacingType = pSlabRebarTemplate->m_nSlabTopSpacerSpacingType;
	m_nSlabTopSpacerSlabSpacing = pSlabRebarTemplate->m_nSlabTopSpacerSlabSpacing;
	m_nSlabTopSpacerUserSpacing1 = pSlabRebarTemplate->m_nSlabTopSpacerUserSpacing1;
	m_nSlabTopSpacerUserSpacing2 = pSlabRebarTemplate->m_nSlabTopSpacerUserSpacing2;
	m_nSlabTopSpacerHeightType = pSlabRebarTemplate->m_nSlabTopSpacerHeightType;
	m_nSlabTopSpacerUserHeight = pSlabRebarTemplate->m_nSlabTopSpacerUserHeight;
	m_nSlabTopSpacerHookLenAType = pSlabRebarTemplate->m_nSlabTopSpacerHookLenAType;
	m_nSlabTopSpacerMainBarHookLenA = pSlabRebarTemplate->m_nSlabTopSpacerMainBarHookLenA;
	m_nSlabTopSpacerUserHookLenA = pSlabRebarTemplate->m_nSlabTopSpacerUserHookLenA;
	m_nSlabTopSpacerHookLenBType = pSlabRebarTemplate->m_nSlabTopSpacerHookLenBType;
	m_nSlabTopSpacerMainBarHookLenB = pSlabRebarTemplate->m_nSlabTopSpacerMainBarHookLenB;
	m_nSlabTopSpacerUserHookLenB = pSlabRebarTemplate->m_nSlabTopSpacerUserHookLenB;
	m_nSlabTopSpacerWidth = pSlabRebarTemplate->m_nSlabTopSpacerWidth;

	m_bSlabBotSpacerSet = pSlabRebarTemplate->m_bSlabBotSpacerSet;
	m_nSlabBotSpacerDiaType = pSlabRebarTemplate->m_nSlabBotSpacerDiaType;
	m_nSlabBotSpacerDiaIndex = pSlabRebarTemplate->m_nSlabBotSpacerDiaIndex;
	m_nSlabBotSpacerSpacingType = pSlabRebarTemplate->m_nSlabBotSpacerSpacingType;
	m_nSlabBotSpacerSlabSpacing = pSlabRebarTemplate->m_nSlabBotSpacerSlabSpacing;
	m_nSlabBotSpacerUserSpacing1 = pSlabRebarTemplate->m_nSlabBotSpacerUserSpacing1;
	m_nSlabBotSpacerUserSpacing2 = pSlabRebarTemplate->m_nSlabBotSpacerUserSpacing2;
	m_nSlabBotSpacerHeightType = pSlabRebarTemplate->m_nSlabBotSpacerHeightType;
	m_nSlabBotSpacerUserHeight = pSlabRebarTemplate->m_nSlabBotSpacerUserHeight;
	m_nSlabBotSpacerHookLenAType = pSlabRebarTemplate->m_nSlabBotSpacerHookLenAType;
	m_nSlabBotSpacerMainBarHookLenA = pSlabRebarTemplate->m_nSlabBotSpacerMainBarHookLenA;
	m_nSlabBotSpacerUserHookLenA = pSlabRebarTemplate->m_nSlabBotSpacerUserHookLenA;
	m_nSlabBotSpacerHookLenBType = pSlabRebarTemplate->m_nSlabBotSpacerHookLenBType;
	m_nSlabBotSpacerMainBarHookLenB = pSlabRebarTemplate->m_nSlabBotSpacerMainBarHookLenB;
	m_nSlabBotSpacerUserHookLenB = pSlabRebarTemplate->m_nSlabBotSpacerUserHookLenB;
	m_nSlabBotSpacerWidth = pSlabRebarTemplate->m_nSlabBotSpacerWidth;

	m_bSlabSpacerSupportBarsSet = pSlabRebarTemplate->m_bSlabSpacerSupportBarsSet;
	m_bSlabSpacerSupportBarsConditionSet = pSlabRebarTemplate->m_bSlabSpacerSupportBarsConditionSet;
	m_nSlabSpacerSupportBarsCondition = pSlabRebarTemplate->m_nSlabSpacerSupportBarsCondition;
	m_nSlabSpacerSupportBarsDiaType = pSlabRebarTemplate->m_nSlabSpacerSupportBarsDiaType;
	m_nSlabSpacerSupportBarsUserDiaIndex = pSlabRebarTemplate->m_nSlabSpacerSupportBarsUserDiaIndex;
	m_nSlabSpacerSupportBarsNumType = pSlabRebarTemplate->m_nSlabSpacerSupportBarsNumType;
	m_dSlabSpacerSupportBarsTopSpacerNums = pSlabRebarTemplate->m_dSlabSpacerSupportBarsTopSpacerNums;
	m_nSlabSpacerSupportBarsUserNums = pSlabRebarTemplate->m_nSlabSpacerSupportBarsUserNums;
	m_nSlabSpacerSupportBarsLengthType = pSlabRebarTemplate->m_nSlabSpacerSupportBarsLengthType;
	m_dSlabSpacerSupportBarsUserRatio = pSlabRebarTemplate->m_dSlabSpacerSupportBarsUserRatio;
	m_nSlabSpacerSupportBarsUserLength = pSlabRebarTemplate->m_nSlabSpacerSupportBarsUserLength;

	m_nSlabCZoneSpliceWay = pSlabRebarTemplate->m_nSlabCZoneSpliceWay;
	m_nSlabCZoneSpliceLength = pSlabRebarTemplate->m_nSlabCZoneSpliceLength;
	// SOG Slab
	m_nSOGMaxDualHookLength = pSlabRebarTemplate->m_nSOGMaxDualHookLength;
	m_nSOGSpliceWay = pSlabRebarTemplate->m_nSOGSpliceWay;
	m_nSOGSpliceType = pSlabRebarTemplate->m_nSOGSpliceType;
		
	m_nSOGTopEndHookType = pSlabRebarTemplate->m_nSOGTopEndHookType;
	m_nSOGBotEndHookType = pSlabRebarTemplate->m_nSOGBotEndHookType;
	m_nSOGIsoFootDevType = pSlabRebarTemplate->m_nSOGIsoFootDevType;
	m_nSOGSpliceDevSlabType = pSlabRebarTemplate->m_nSOGSpliceDevSlabType;

	m_nSOGTopEndDevTypeUserLength = pSlabRebarTemplate->m_nSOGTopEndDevTypeUserLength;
	m_nSOGBotEndDevTypeUserLength = pSlabRebarTemplate->m_nSOGBotEndDevTypeUserLength;
	m_nSOGOpeningTopDevType = pSlabRebarTemplate->m_nSOGOpeningTopDevType;
	m_nSOGOpeningBotDevType = pSlabRebarTemplate->m_nSOGOpeningBotDevType;

	m_nSOGLevelDifPlaceShape = pSlabRebarTemplate->m_nSOGLevelDifPlaceShape;
	m_nSOGLevelDifHaunchPlaceType = pSlabRebarTemplate->m_nSOGLevelDifHaunchPlaceType;

	m_bSOGSupportRebarsSet = pSlabRebarTemplate->m_bSOGSupportRebarsSet;
	m_nSOGSupportRebarsDiaIndex = pSlabRebarTemplate->m_nSOGSupportRebarsDiaIndex;
	m_nSOGSupportRebarsLengthType = pSlabRebarTemplate->m_nSOGSupportRebarsLengthType;
	m_dSOGSupportRebarsRWallLengthRate = pSlabRebarTemplate->m_dSOGSupportRebarsRWallLengthRate;
	m_dSOGSupportRebarsSlabLengthRate = pSlabRebarTemplate->m_dSOGSupportRebarsSlabLengthRate;
	m_nSOGSupportRebarsOrderLength = pSlabRebarTemplate->m_nSOGSupportRebarsOrderLength;
	m_nSOGSupportRebarsSpliceLength = pSlabRebarTemplate->m_nSOGSupportRebarsSpliceLength;
	m_nSOGSupportRebarsCorrectiveLength = pSlabRebarTemplate->m_nSOGSupportRebarsCorrectiveLength;
		
	m_nSOGRebarTolDist = pSlabRebarTemplate->m_nSOGRebarTolDist;
	m_nSOGRebarTolLenDif = pSlabRebarTemplate->m_nSOGRebarTolLenDif;

	m_bSOGSpliceDevConditionSet = pSlabRebarTemplate->m_bSOGSpliceDevConditionSet;
	m_nSOGSpliceDevConditionLength = pSlabRebarTemplate->m_nSOGSpliceDevConditionLength;

	m_bSOGSpliceAddLengthSet = pSlabRebarTemplate->m_bSOGSpliceAddLengthSet;
	m_nSOGSpliceAddLengthNumSplice = pSlabRebarTemplate->m_nSOGSpliceAddLengthNumSplice;

	m_bSOGTopSpacerSet = pSlabRebarTemplate->m_bSOGTopSpacerSet;
	m_nSOGTopSpacerDiaType = pSlabRebarTemplate->m_nSOGTopSpacerDiaType;
	m_nSOGTopSpacerSlabThick = pSlabRebarTemplate->m_nSOGTopSpacerSlabThick;
	m_nSOGTopSpacerSlabThickOverDiaIndex = pSlabRebarTemplate->m_nSOGTopSpacerSlabThickOverDiaIndex;
	m_nSOGTopSpacerSlabThickUnderDiaIndex = pSlabRebarTemplate->m_nSOGTopSpacerSlabThickUnderDiaIndex;
	m_nSOGTopSpacerSpacingType = pSlabRebarTemplate->m_nSOGTopSpacerSpacingType;
	m_nSOGTopSpacerSlabSpacing = pSlabRebarTemplate->m_nSOGTopSpacerSlabSpacing;
	m_nSOGTopSpacerUserSpacing1 = pSlabRebarTemplate->m_nSOGTopSpacerUserSpacing1;
	m_nSOGTopSpacerUserSpacing2 = pSlabRebarTemplate->m_nSOGTopSpacerUserSpacing2;
	m_nSOGTopSpacerHeightType = pSlabRebarTemplate->m_nSOGTopSpacerHeightType;
	m_nSOGTopSpacerUserHeight = pSlabRebarTemplate->m_nSOGTopSpacerUserHeight;
	m_nSOGTopSpacerHookLenAType = pSlabRebarTemplate->m_nSOGTopSpacerHookLenAType;
	m_nSOGTopSpacerMainBarHookLenA = pSlabRebarTemplate->m_nSOGTopSpacerMainBarHookLenA;
	m_nSOGTopSpacerUserHookLenA = pSlabRebarTemplate->m_nSOGTopSpacerUserHookLenA;
	m_nSOGTopSpacerHookLenBType = pSlabRebarTemplate->m_nSOGTopSpacerHookLenBType;
	m_nSOGTopSpacerMainBarHookLenB = pSlabRebarTemplate->m_nSOGTopSpacerMainBarHookLenB;
	m_nSOGTopSpacerUserHookLenB = pSlabRebarTemplate->m_nSOGTopSpacerUserHookLenB;
	m_nSOGTopSpacerWidth = pSlabRebarTemplate->m_nSOGTopSpacerWidth;

	m_bSOGBotSpacerSet = pSlabRebarTemplate->m_bSOGBotSpacerSet;
	m_nSOGBotSpacerDiaType = pSlabRebarTemplate->m_nSOGBotSpacerDiaType;
	m_nSOGBotSpacerDiaIndex = pSlabRebarTemplate->m_nSOGBotSpacerDiaIndex;
	m_nSOGBotSpacerSpacingType = pSlabRebarTemplate->m_nSOGBotSpacerSpacingType;
	m_nSOGBotSpacerSlabSpacing = pSlabRebarTemplate->m_nSOGBotSpacerSlabSpacing;
	m_nSOGBotSpacerUserSpacing1 = pSlabRebarTemplate->m_nSOGBotSpacerUserSpacing1;
	m_nSOGBotSpacerUserSpacing2 = pSlabRebarTemplate->m_nSOGBotSpacerUserSpacing2;
	m_nSOGBotSpacerHeightType = pSlabRebarTemplate->m_nSOGBotSpacerHeightType;
	m_nSOGBotSpacerUserHeight = pSlabRebarTemplate->m_nSOGBotSpacerUserHeight;
	m_nSOGBotSpacerHookLenAType = pSlabRebarTemplate->m_nSOGBotSpacerHookLenAType;
	m_nSOGBotSpacerMainBarHookLenA = pSlabRebarTemplate->m_nSOGBotSpacerMainBarHookLenA;
	m_nSOGBotSpacerUserHookLenA = pSlabRebarTemplate->m_nSOGBotSpacerUserHookLenA;
	m_nSOGBotSpacerHookLenBType = pSlabRebarTemplate->m_nSOGBotSpacerHookLenBType;
	m_nSOGBotSpacerMainBarHookLenB = pSlabRebarTemplate->m_nSOGBotSpacerMainBarHookLenB;
	m_nSOGBotSpacerUserHookLenB = pSlabRebarTemplate->m_nSOGBotSpacerUserHookLenB;
	m_nSOGBotSpacerWidth = pSlabRebarTemplate->m_nSOGBotSpacerWidth;

	m_bSOGSpacerSupportBarsSet = pSlabRebarTemplate->m_bSOGSpacerSupportBarsSet;
	m_bSOGSpacerSupportBarsConditionSet = pSlabRebarTemplate->m_bSOGSpacerSupportBarsConditionSet;
	m_nSOGSpacerSupportBarsCondition = pSlabRebarTemplate->m_nSOGSpacerSupportBarsCondition;
	m_nSOGSpacerSupportBarsDiaType = pSlabRebarTemplate->m_nSOGSpacerSupportBarsDiaType;
	m_nSOGSpacerSupportBarsUserDiaIndex = pSlabRebarTemplate->m_nSOGSpacerSupportBarsUserDiaIndex;
	m_nSOGSpacerSupportBarsNumType = pSlabRebarTemplate->m_nSOGSpacerSupportBarsNumType;
	m_dSOGSpacerSupportBarsTopSpacerNums = pSlabRebarTemplate->m_dSOGSpacerSupportBarsTopSpacerNums;
	m_nSOGSpacerSupportBarsUserNums = pSlabRebarTemplate->m_nSOGSpacerSupportBarsUserNums;
	m_nSOGSpacerSupportBarsLengthType = pSlabRebarTemplate->m_nSOGSpacerSupportBarsLengthType;
	m_dSOGSpacerSupportBarsUserRatio = pSlabRebarTemplate->m_dSOGSpacerSupportBarsUserRatio;
	m_nSOGSpacerSupportBarsUserLength = pSlabRebarTemplate->m_nSOGSpacerSupportBarsUserLength;

	m_nSOGCZoneSpliceWay = pSlabRebarTemplate->m_nSOGCZoneSpliceWay;
	m_nSOGCZoneSpliceLength = pSlabRebarTemplate->m_nSOGCZoneSpliceLength;
	// Circular Ramp
	m_nCircularRampMaxDualHookLength = pSlabRebarTemplate->m_nCircularRampMaxDualHookLength;
	m_nCircularRampSpliceWay = pSlabRebarTemplate->m_nCircularRampSpliceWay;
	m_nCircularRampSpliceType = pSlabRebarTemplate->m_nCircularRampSpliceType;
		
	m_nCircularRampTopEndHookType = pSlabRebarTemplate->m_nCircularRampTopEndHookType;
	m_nCircularRampBotEndHookType = pSlabRebarTemplate->m_nCircularRampBotEndHookType;
	m_nCircularRampIsoFootDevType = pSlabRebarTemplate->m_nCircularRampIsoFootDevType;
	m_nCircularRampSpliceDevSlabType = pSlabRebarTemplate->m_nCircularRampSpliceDevSlabType;

	m_nCircularRampTopEndDevTypeUserLength = pSlabRebarTemplate->m_nCircularRampTopEndDevTypeUserLength;
	m_nCircularRampBotEndDevTypeUserLength = pSlabRebarTemplate->m_nCircularRampBotEndDevTypeUserLength;
	m_nCircularRampOpeningTopDevType = pSlabRebarTemplate->m_nCircularRampOpeningTopDevType;
	m_nCircularRampOpeningBotDevType = pSlabRebarTemplate->m_nCircularRampOpeningBotDevType;

	m_nCircularRampLevelDifPlaceShape = pSlabRebarTemplate->m_nCircularRampLevelDifPlaceShape;
	m_nCircularRampLevelDifHaunchPlaceType = pSlabRebarTemplate->m_nCircularRampLevelDifHaunchPlaceType;
		
	m_nCircularRampRebarTolDist = pSlabRebarTemplate->m_nCircularRampRebarTolDist;
	m_nCircularRampRebarTolLenDif = pSlabRebarTemplate->m_nCircularRampRebarTolLenDif;

	m_bCircularRampSpliceDevConditionSet = pSlabRebarTemplate->m_bCircularRampSpliceDevConditionSet;
	m_nCircularRampSpliceDevConditionLength = pSlabRebarTemplate->m_nCircularRampSpliceDevConditionLength;

	m_bCircularRampSpliceAddLengthSet = pSlabRebarTemplate->m_bCircularRampSpliceAddLengthSet;
	m_nCircularRampSpliceAddLengthNumSplice = pSlabRebarTemplate->m_nCircularRampSpliceAddLengthNumSplice;

	m_bCircularRampTopSpacerSet = pSlabRebarTemplate->m_bCircularRampTopSpacerSet;
	m_nCircularRampTopSpacerDiaType = pSlabRebarTemplate->m_nCircularRampTopSpacerDiaType;
	m_nCircularRampTopSpacerSlabThick = pSlabRebarTemplate->m_nCircularRampTopSpacerSlabThick;
	m_nCircularRampTopSpacerSlabThickOverDiaIndex = pSlabRebarTemplate->m_nCircularRampTopSpacerSlabThickOverDiaIndex;
	m_nCircularRampTopSpacerSlabThickUnderDiaIndex = pSlabRebarTemplate->m_nCircularRampTopSpacerSlabThickUnderDiaIndex;
	m_nCircularRampTopSpacerSpacingType = pSlabRebarTemplate->m_nCircularRampTopSpacerSpacingType;
	m_nCircularRampTopSpacerSlabSpacing = pSlabRebarTemplate->m_nCircularRampTopSpacerSlabSpacing;
	m_nCircularRampTopSpacerUserSpacing1 = pSlabRebarTemplate->m_nCircularRampTopSpacerUserSpacing1;
	m_nCircularRampTopSpacerUserSpacing2 = pSlabRebarTemplate->m_nCircularRampTopSpacerUserSpacing2;
	m_nCircularRampTopSpacerHeightType = pSlabRebarTemplate->m_nCircularRampTopSpacerHeightType;
	m_nCircularRampTopSpacerUserHeight = pSlabRebarTemplate->m_nCircularRampTopSpacerUserHeight;
	m_nCircularRampTopSpacerHookLenAType = pSlabRebarTemplate->m_nCircularRampTopSpacerHookLenAType;
	m_nCircularRampTopSpacerMainBarHookLenA = pSlabRebarTemplate->m_nCircularRampTopSpacerMainBarHookLenA;
	m_nCircularRampTopSpacerUserHookLenA = pSlabRebarTemplate->m_nCircularRampTopSpacerUserHookLenA;
	m_nCircularRampTopSpacerHookLenBType = pSlabRebarTemplate->m_nCircularRampTopSpacerHookLenBType;
	m_nCircularRampTopSpacerMainBarHookLenB = pSlabRebarTemplate->m_nCircularRampTopSpacerMainBarHookLenB;
	m_nCircularRampTopSpacerUserHookLenB = pSlabRebarTemplate->m_nCircularRampTopSpacerUserHookLenB;
	m_nCircularRampTopSpacerWidth = pSlabRebarTemplate->m_nCircularRampTopSpacerWidth;

	m_bCircularRampBotSpacerSet = pSlabRebarTemplate->m_bCircularRampBotSpacerSet;
	m_nCircularRampBotSpacerDiaType = pSlabRebarTemplate->m_nCircularRampBotSpacerDiaType;
	m_nCircularRampBotSpacerDiaIndex = pSlabRebarTemplate->m_nCircularRampBotSpacerDiaIndex;
	m_nCircularRampBotSpacerSpacingType = pSlabRebarTemplate->m_nCircularRampBotSpacerSpacingType;
	m_nCircularRampBotSpacerSlabSpacing = pSlabRebarTemplate->m_nCircularRampBotSpacerSlabSpacing;
	m_nCircularRampBotSpacerUserSpacing1 = pSlabRebarTemplate->m_nCircularRampBotSpacerUserSpacing1;
	m_nCircularRampBotSpacerUserSpacing2 = pSlabRebarTemplate->m_nCircularRampBotSpacerUserSpacing2;
	m_nCircularRampBotSpacerHeightType = pSlabRebarTemplate->m_nCircularRampBotSpacerHeightType;
	m_nCircularRampBotSpacerUserHeight = pSlabRebarTemplate->m_nCircularRampBotSpacerUserHeight;
	m_nCircularRampBotSpacerHookLenAType = pSlabRebarTemplate->m_nCircularRampBotSpacerHookLenAType;
	m_nCircularRampBotSpacerMainBarHookLenA = pSlabRebarTemplate->m_nCircularRampBotSpacerMainBarHookLenA;
	m_nCircularRampBotSpacerUserHookLenA = pSlabRebarTemplate->m_nCircularRampBotSpacerUserHookLenA;
	m_nCircularRampBotSpacerHookLenBType = pSlabRebarTemplate->m_nCircularRampBotSpacerHookLenBType;
	m_nCircularRampBotSpacerMainBarHookLenB = pSlabRebarTemplate->m_nCircularRampBotSpacerMainBarHookLenB;
	m_nCircularRampBotSpacerUserHookLenB = pSlabRebarTemplate->m_nCircularRampBotSpacerUserHookLenB;
	m_nCircularRampBotSpacerWidth = pSlabRebarTemplate->m_nCircularRampBotSpacerWidth;

	m_bCircularRampSpacerSupportBarsSet = pSlabRebarTemplate->m_bCircularRampSpacerSupportBarsSet;
	m_bCircularRampSpacerSupportBarsConditionSet = pSlabRebarTemplate->m_bCircularRampSpacerSupportBarsConditionSet;
	m_nCircularRampSpacerSupportBarsCondition = pSlabRebarTemplate->m_nCircularRampSpacerSupportBarsCondition;
	m_nCircularRampSpacerSupportBarsDiaType = pSlabRebarTemplate->m_nCircularRampSpacerSupportBarsDiaType;
	m_nCircularRampSpacerSupportBarsUserDiaIndex = pSlabRebarTemplate->m_nCircularRampSpacerSupportBarsUserDiaIndex;
	m_nCircularRampSpacerSupportBarsNumType = pSlabRebarTemplate->m_nCircularRampSpacerSupportBarsNumType;
	m_dCircularRampSpacerSupportBarsTopSpacerNums = pSlabRebarTemplate->m_dCircularRampSpacerSupportBarsTopSpacerNums;
	m_nCircularRampSpacerSupportBarsUserNums = pSlabRebarTemplate->m_nCircularRampSpacerSupportBarsUserNums;
	m_nCircularRampSpacerSupportBarsLengthType = pSlabRebarTemplate->m_nCircularRampSpacerSupportBarsLengthType;
	m_dCircularRampSpacerSupportBarsUserRatio = pSlabRebarTemplate->m_dCircularRampSpacerSupportBarsUserRatio;
	m_nCircularRampSpacerSupportBarsUserLength = pSlabRebarTemplate->m_nCircularRampSpacerSupportBarsUserLength;

	m_nCircularRampCZoneSpliceWay = pSlabRebarTemplate->m_nCircularRampCZoneSpliceWay;
	m_nCircularRampCZoneSpliceLength = pSlabRebarTemplate->m_nCircularRampCZoneSpliceLength;
	// PC Slab
	m_nPCSlabMaxDualHookLength = pSlabRebarTemplate->m_nPCSlabMaxDualHookLength;
	m_nPCSlabSpliceWay = pSlabRebarTemplate->m_nPCSlabSpliceWay;
	m_nPCSlabSpliceType = pSlabRebarTemplate->m_nPCSlabSpliceType;
	
	m_nPCSlabTopEndAddBarLxLnxType = pSlabRebarTemplate->m_nPCSlabTopEndAddBarLxLnxType;
	m_nPCSlabTopEndAddBarLengthType = pSlabRebarTemplate->m_nPCSlabTopEndAddBarLengthType;
	m_nPCSlabTopCenAddBarLxLnxType = pSlabRebarTemplate->m_nPCSlabTopCenAddBarLxLnxType;
	m_nPCSlabTopCenAddBarLengthType = pSlabRebarTemplate->m_nPCSlabTopCenAddBarLengthType;

	m_nPCSlabTopEndDevTypeUserLength = pSlabRebarTemplate->m_nPCSlabTopEndDevTypeUserLength;
	m_nPCSlabBotEndDevTypeUserLength = pSlabRebarTemplate->m_nPCSlabBotEndDevTypeUserLength;

	m_bPCSlabShortCenSpliceBarPlace = pSlabRebarTemplate->m_bPCSlabShortCenSpliceBarPlace;
	m_nPCSlabShortCenSpliceBarLengthType = pSlabRebarTemplate->m_nPCSlabShortCenSpliceBarLengthType;
	m_bPCSlabShortEndSpliceBarPlace = pSlabRebarTemplate->m_bPCSlabShortEndSpliceBarPlace;
	m_nPCSlabShortEndSpliceBarLengthType = pSlabRebarTemplate->m_nPCSlabShortEndSpliceBarLengthType;
	m_bPCSlabLongCenSpliceBarPlace = pSlabRebarTemplate->m_bPCSlabLongCenSpliceBarPlace;
	m_nPCSlabLongCenSpliceBarLengthType = pSlabRebarTemplate->m_nPCSlabLongCenSpliceBarLengthType;
	m_bPCSlabLongEndSpliceBarPlace = pSlabRebarTemplate->m_bPCSlabLongEndSpliceBarPlace;
	m_nPCSlabLongEndSpliceBarLengthType = pSlabRebarTemplate->m_nPCSlabLongEndSpliceBarLengthType;

	m_nPCSlabTopUnStructuredEndHookType = pSlabRebarTemplate->m_nPCSlabTopUnStructuredEndHookType;
	m_nPCSlabBotUnStructuredEndHookType = pSlabRebarTemplate->m_nPCSlabBotUnStructuredEndHookType;
	
	m_nPCSlabLevelDifPlaceShape = pSlabRebarTemplate->m_nPCSlabLevelDifPlaceShape;
	m_nPCSlabLevelDifHaunchPlaceType = pSlabRebarTemplate->m_nPCSlabLevelDifHaunchPlaceType;

	m_nPCSlabOpeningTopDevType = pSlabRebarTemplate->m_nPCSlabOpeningTopDevType;
	m_nPCSlabOpeningBotDevType = pSlabRebarTemplate->m_nPCSlabOpeningBotDevType;

	m_nPCSlabRebarTolDist = pSlabRebarTemplate->m_nPCSlabRebarTolDist;
	m_nPCSlabRebarTolLenDif = pSlabRebarTemplate->m_nPCSlabRebarTolLenDif;

	m_bPCSlabSpliceDevConditionSet = pSlabRebarTemplate->m_bPCSlabSpliceDevConditionSet;
	m_nPCSlabSpliceDevConditionLength = pSlabRebarTemplate->m_nPCSlabSpliceDevConditionLength;

	m_bPCSlabSpliceAddLengthSet = pSlabRebarTemplate->m_bPCSlabSpliceAddLengthSet;
	m_nPCSlabSpliceAddLengthNumSplice = pSlabRebarTemplate->m_nPCSlabSpliceAddLengthNumSplice;

	// 슬래브 보강근
	m_nSlabRebarRebarTolDist = pSlabRebarTemplate->m_nSlabRebarRebarTolDist;
	m_nSlabRebarRebarTolLenDif = pSlabRebarTemplate->m_nSlabRebarRebarTolLenDif;

	// 철근 일체형 데크 슬래브
	m_nEmbeddedDeckMaxDualHookLength = pSlabRebarTemplate->m_nEmbeddedDeckMaxDualHookLength;
	m_nEmbeddedDeckSpliceWay = pSlabRebarTemplate->m_nEmbeddedDeckSpliceWay;
	m_nEmbeddedDeckSpliceType = pSlabRebarTemplate->m_nEmbeddedDeckSpliceType;
		
	m_nEmbeddedDeckTopEndAddBarLxLnxType = pSlabRebarTemplate->m_nEmbeddedDeckTopEndAddBarLxLnxType;
	m_nEmbeddedDeckTopEndAddBarLengthType = pSlabRebarTemplate->m_nEmbeddedDeckTopEndAddBarLengthType;
	m_nEmbeddedDeckTopCenAddBarLxLnxType = pSlabRebarTemplate->m_nEmbeddedDeckTopCenAddBarLxLnxType;
	m_nEmbeddedDeckTopCenAddBarLengthType = pSlabRebarTemplate->m_nEmbeddedDeckTopCenAddBarLengthType;

	m_bEmbeddedDeckSpliceBarPlace = pSlabRebarTemplate->m_bEmbeddedDeckSpliceBarPlace;
	m_nEmbeddedDeckSpliceBarLengthType = pSlabRebarTemplate->m_nEmbeddedDeckSpliceBarLengthType;

	m_nEmbeddedDeckTopEndDevTypeUserLength = pSlabRebarTemplate->m_nEmbeddedDeckTopEndDevTypeUserLength;
	m_nEmbeddedDeckBotEndDevTypeUserLength = pSlabRebarTemplate->m_nEmbeddedDeckBotEndDevTypeUserLength;

	m_nEmbeddedDeckTopUnStructuredEndHookType = pSlabRebarTemplate->m_nEmbeddedDeckTopUnStructuredEndHookType;
	m_nEmbeddedDeckBotUnStructuredEndHookType = pSlabRebarTemplate->m_nEmbeddedDeckBotUnStructuredEndHookType;

	m_nEmbeddedDeckLevelDifPlaceShape = pSlabRebarTemplate->m_nEmbeddedDeckLevelDifPlaceShape;
	m_nEmbeddedDeckLevelDifHaunchPlaceType = pSlabRebarTemplate->m_nEmbeddedDeckLevelDifHaunchPlaceType;

	m_nEmbeddedDeckOpeningTopDevType = pSlabRebarTemplate->m_nEmbeddedDeckOpeningTopDevType;
	m_nEmbeddedDeckOpeningBotDevType = pSlabRebarTemplate->m_nEmbeddedDeckOpeningBotDevType;

	m_nEmbeddedDeckRebarTolDist = pSlabRebarTemplate->m_nEmbeddedDeckRebarTolDist;
	m_nEmbeddedDeckRebarTolLenDif = pSlabRebarTemplate->m_nEmbeddedDeckRebarTolLenDif;

	m_bEmbeddedDeckSpliceDevConditionSet = pSlabRebarTemplate->m_bEmbeddedDeckSpliceDevConditionSet;
	m_nEmbeddedDeckSpliceDevConditionLength = pSlabRebarTemplate->m_nEmbeddedDeckSpliceDevConditionLength;

	m_bEmbeddedDeckSpliceAddLengthSet = pSlabRebarTemplate->m_bEmbeddedDeckSpliceAddLengthSet;
	m_nEmbeddedDeckSpliceAddLengthNumSplice = pSlabRebarTemplate->m_nEmbeddedDeckSpliceAddLengthNumSplice;

	UserDowelHookLenMapDataCopy(mm_EmbeddedDeckTopEndAddBarUserLen, pSlabRebarTemplate->mm_EmbeddedDeckTopEndAddBarUserLen);
	UserDowelHookLenMapDataCopy(mm_EmbeddedDeckTopCenAddBarUserLen, pSlabRebarTemplate->mm_EmbeddedDeckTopCenAddBarUserLen);
	UserDowelHookLenMapDataCopy(mm_EmbeddedDeckSpliceBarUserLen, pSlabRebarTemplate->mm_EmbeddedDeckSpliceBarUserLen);
	// 일반 데크 슬래브
	m_nGeneralDeckMaxDualHookLength = pSlabRebarTemplate->m_nGeneralDeckMaxDualHookLength;
	m_nGeneralDeckSpliceWay = pSlabRebarTemplate->m_nGeneralDeckSpliceWay;
	m_nGeneralDeckSpliceType = pSlabRebarTemplate->m_nGeneralDeckSpliceType;
		
	m_nGeneralDeckTopEndAddBarLxLnxType = pSlabRebarTemplate->m_nGeneralDeckTopEndAddBarLxLnxType;
	m_nGeneralDeckTopEndAddBarLengthType = pSlabRebarTemplate->m_nGeneralDeckTopEndAddBarLengthType;
	m_nGeneralDeckTopCenAddBarLxLnxType = pSlabRebarTemplate->m_nGeneralDeckTopCenAddBarLxLnxType;
	m_nGeneralDeckTopCenAddBarLengthType = pSlabRebarTemplate->m_nGeneralDeckTopCenAddBarLengthType;

	m_bGeneralDeckSpliceBarPlace = pSlabRebarTemplate->m_bGeneralDeckSpliceBarPlace;
	m_nGeneralDeckSpliceBarLengthType = pSlabRebarTemplate->m_nGeneralDeckSpliceBarLengthType;

	m_nGeneralDeckTopEndDevTypeUserLength = pSlabRebarTemplate->m_nGeneralDeckTopEndDevTypeUserLength;
	m_nGeneralDeckBotEndDevTypeUserLength = pSlabRebarTemplate->m_nGeneralDeckBotEndDevTypeUserLength;

	m_nGeneralDeckTopUnStructuredEndHookType = pSlabRebarTemplate->m_nGeneralDeckTopUnStructuredEndHookType;
	m_nGeneralDeckBotUnStructuredEndHookType = pSlabRebarTemplate->m_nGeneralDeckBotUnStructuredEndHookType;

	m_nGeneralDeckLevelDifPlaceShape = pSlabRebarTemplate->m_nGeneralDeckLevelDifPlaceShape;
	m_nGeneralDeckLevelDifHaunchPlaceType = pSlabRebarTemplate->m_nGeneralDeckLevelDifHaunchPlaceType;

	m_nGeneralDeckOpeningTopDevType = pSlabRebarTemplate->m_nGeneralDeckOpeningTopDevType;
	m_nGeneralDeckOpeningBotDevType = pSlabRebarTemplate->m_nGeneralDeckOpeningBotDevType;

	m_nGeneralDeckRebarTolDist = pSlabRebarTemplate->m_nGeneralDeckRebarTolDist;
	m_nGeneralDeckRebarTolLenDif = pSlabRebarTemplate->m_nGeneralDeckRebarTolLenDif;

	m_bGeneralDeckSpliceDevConditionSet = pSlabRebarTemplate->m_bGeneralDeckSpliceDevConditionSet;
	m_nGeneralDeckSpliceDevConditionLength = pSlabRebarTemplate->m_nGeneralDeckSpliceDevConditionLength;

	m_bGeneralDeckSpliceAddLengthSet = pSlabRebarTemplate->m_bGeneralDeckSpliceAddLengthSet;
	m_nGeneralDeckSpliceAddLengthNumSplice = pSlabRebarTemplate->m_nGeneralDeckSpliceAddLengthNumSplice;

	UserDowelHookLenMapDataCopy(mm_GeneralDeckTopEndAddBarUserLen, pSlabRebarTemplate->mm_GeneralDeckTopEndAddBarUserLen);
	UserDowelHookLenMapDataCopy(mm_GeneralDeckTopCenAddBarUserLen, pSlabRebarTemplate->mm_GeneralDeckTopCenAddBarUserLen);
	UserDowelHookLenMapDataCopy(mm_GeneralDeckSpliceBarUserLen, pSlabRebarTemplate->mm_GeneralDeckSpliceBarUserLen);
	// PC IMT Slab
	m_bPCIMTSlabSpliceDevConditionSet = pSlabRebarTemplate->m_bPCIMTSlabSpliceDevConditionSet;
	m_nPCIMTSlabSpliceDevConditionLength = pSlabRebarTemplate->m_nPCIMTSlabSpliceDevConditionLength;

	m_bPCIMTSlabSpliceAddLengthSet = pSlabRebarTemplate->m_bPCIMTSlabSpliceAddLengthSet;
	m_nPCIMTSlabSpliceAddLengthNumSplice = pSlabRebarTemplate->m_nPCIMTSlabSpliceAddLengthNumSplice;

	m_nPCIMTSlabMaxDualHookLength = pSlabRebarTemplate->m_nPCIMTSlabMaxDualHookLength;
	m_nPCIMTSlabSpliceWay = pSlabRebarTemplate->m_nPCIMTSlabSpliceWay;
	m_nPCIMTSlabSpliceType = pSlabRebarTemplate->m_nPCIMTSlabSpliceType;
		
	m_nPCIMTSlabTopHookType = pSlabRebarTemplate->m_nPCIMTSlabTopHookType;
	m_nPCIMTSlabBotHookType = pSlabRebarTemplate->m_nPCIMTSlabBotHookType;
	m_nPCIMTSlabTopEndDevTypeUserLength = pSlabRebarTemplate->m_nPCIMTSlabTopEndDevTypeUserLength;
	m_nPCIMTSlabBotEndDevTypeUserLength = pSlabRebarTemplate->m_nPCIMTSlabBotEndDevTypeUserLength;
	m_nPCIMTSlabOpeningTopDevType = pSlabRebarTemplate->m_nPCIMTSlabOpeningTopDevType;
	m_nPCIMTSlabOpeningBotDevType = pSlabRebarTemplate->m_nPCIMTSlabOpeningBotDevType;

	m_nPCIMTSlabTopEndAddBarLxLnxType = pSlabRebarTemplate->m_nPCIMTSlabTopEndAddBarLxLnxType;
	m_nPCIMTSlabTopEndAddBarLengthType = pSlabRebarTemplate->m_nPCIMTSlabTopEndAddBarLengthType;
	m_nPCIMTSlabTopCenAddBarLxLnxType = pSlabRebarTemplate->m_nPCIMTSlabTopCenAddBarLxLnxType;
	m_nPCIMTSlabTopCenAddBarLengthType = pSlabRebarTemplate->m_nPCIMTSlabTopCenAddBarLengthType;

	m_bPCIMTSlabShortSpliceBarPlace = pSlabRebarTemplate->m_bPCIMTSlabShortSpliceBarPlace;
	m_nPCIMTSlabShortSpliceBarLengthType = pSlabRebarTemplate->m_nPCIMTSlabShortSpliceBarLengthType;
	m_bPCIMTSlabLongSpliceBarPlace = pSlabRebarTemplate->m_bPCIMTSlabLongSpliceBarPlace;

	m_nPCIMTSlabRebarTolDist = pSlabRebarTemplate->m_nPCIMTSlabRebarTolDist;
	m_nPCIMTSlabRebarTolLenDif = pSlabRebarTemplate->m_nPCIMTSlabRebarTolLenDif;

	m_nPCIMTSlabLongSpliceBarType = pSlabRebarTemplate->m_nPCIMTSlabLongSpliceBarType;

	UserDowelHookLenMapDataCopy(mm_PCIMTSlabTopEndAddBarUserLen, pSlabRebarTemplate->mm_PCIMTSlabTopEndAddBarUserLen);
	UserDowelHookLenMapDataCopy(mm_PCIMTSlabTopCenAddBarUserLen, pSlabRebarTemplate->mm_PCIMTSlabTopCenAddBarUserLen);
	UserDowelHookLenMapDataCopy(mm_PCIMTSlabShortSpliceBarUserLen, pSlabRebarTemplate->mm_PCIMTSlabShortSpliceBarUserLen);

	//PC HCS Slab
	m_bPCHCSSlabSpliceDevConditionSet = pSlabRebarTemplate->m_bPCHCSSlabSpliceDevConditionSet;
	m_nPCHCSSlabSpliceDevConditionLength = pSlabRebarTemplate->m_nPCHCSSlabSpliceDevConditionLength;

	m_bPCHCSSlabSpliceAddLengthSet = pSlabRebarTemplate->m_bPCHCSSlabSpliceAddLengthSet;
	m_nPCHCSSlabSpliceAddLengthNumSplice = pSlabRebarTemplate->m_nPCHCSSlabSpliceAddLengthNumSplice;

	m_nPCHCSSlabMaxDualHookLength = pSlabRebarTemplate->m_nPCHCSSlabMaxDualHookLength;
	m_nPCHCSSlabSpliceWay = pSlabRebarTemplate->m_nPCHCSSlabSpliceWay;
	m_nPCHCSSlabSpliceType = pSlabRebarTemplate->m_nPCHCSSlabSpliceType;
	
	m_nPCHCSSlabTopHookType = pSlabRebarTemplate->m_nPCHCSSlabTopHookType;
	m_nPCHCSSlabBotHookType = pSlabRebarTemplate->m_nPCHCSSlabBotHookType;
	m_nPCHCSSlabTopEndDevTypeUserLength = pSlabRebarTemplate->m_nPCHCSSlabTopEndDevTypeUserLength;
	m_nPCHCSSlabBotEndDevTypeUserLength = pSlabRebarTemplate->m_nPCHCSSlabBotEndDevTypeUserLength;
	m_nPCHCSSlabOpeningTopDevType = pSlabRebarTemplate->m_nPCHCSSlabOpeningTopDevType;
	m_nPCHCSSlabOpeningBotDevType = pSlabRebarTemplate->m_nPCHCSSlabOpeningBotDevType;

	m_nPCHCSSlabTopEndAddBarLxLnxType = pSlabRebarTemplate->m_nPCHCSSlabTopEndAddBarLxLnxType;
	m_nPCHCSSlabTopEndAddBarLengthType = pSlabRebarTemplate->m_nPCHCSSlabTopEndAddBarLengthType;
	m_nPCHCSSlabTopCenAddBarLxLnxType = pSlabRebarTemplate->m_nPCHCSSlabTopCenAddBarLxLnxType;
	m_nPCHCSSlabTopCenAddBarLengthType = pSlabRebarTemplate->m_nPCHCSSlabTopCenAddBarLengthType;
	m_nPCHCSSlabDevSubBarLengthType = pSlabRebarTemplate->m_nPCHCSSlabDevSubBarLengthType;
	m_nPCHCSSlabShortCenSpliceBarLengthType = pSlabRebarTemplate->m_nPCHCSSlabShortCenSpliceBarLengthType;
	m_nPCHCSSlabShortEndSpliceBarLengthType = pSlabRebarTemplate->m_nPCHCSSlabShortEndSpliceBarLengthType;
	m_nPCHCSSlabLongCenSpliceBarLengthType = pSlabRebarTemplate->m_nPCHCSSlabLongCenSpliceBarLengthType;
	m_nPCHCSSlabLongEndSpliceBarLengthType = pSlabRebarTemplate->m_nPCHCSSlabLongEndSpliceBarLengthType;

	m_nPCHCSSlabRebarTolDist = pSlabRebarTemplate->m_nPCHCSSlabRebarTolDist;
	m_nPCHCSSlabRebarTolLenDif = pSlabRebarTemplate->m_nPCHCSSlabRebarTolLenDif;

	m_bPCHCSSlabShortCenSpliceBarPlace = pSlabRebarTemplate->m_bPCHCSSlabShortCenSpliceBarPlace;
	m_bPCHCSSlabShortEndSpliceBarPlace = pSlabRebarTemplate->m_bPCHCSSlabShortEndSpliceBarPlace;
	m_bPCHCSSlabLongCenSpliceBarPlace = pSlabRebarTemplate->m_bPCHCSSlabLongCenSpliceBarPlace;
	m_bPCHCSSlabLongEndSpliceBarPlace = pSlabRebarTemplate->m_bPCHCSSlabLongEndSpliceBarPlace;

	UserDowelHookLenMapDataCopy(mm_PCHCSSlabTopEndAddBarUserLen, pSlabRebarTemplate->mm_PCHCSSlabTopEndAddBarUserLen);
	UserDowelHookLenMapDataCopy(mm_PCHCSSlabTopCenAddBarUserLen, pSlabRebarTemplate->mm_PCHCSSlabTopCenAddBarUserLen);
	UserDowelHookLenMapDataCopy(mm_PCHCSSlabDevSubBarUserLen, pSlabRebarTemplate->mm_PCHCSSlabDevSubBarUserLen);
	UserDowelHookLenMapDataCopy(mm_PCHCSSlabShortCenSpliceBarUserLen, pSlabRebarTemplate->mm_PCHCSSlabShortCenSpliceBarUserLen);
	UserDowelHookLenMapDataCopy(mm_PCHCSSlabShortEndSpliceBarUserLen, pSlabRebarTemplate->mm_PCHCSSlabShortEndSpliceBarUserLen);
	UserDowelHookLenMapDataCopy(mm_PCHCSSlabLongCenSpliceBarUserLen, pSlabRebarTemplate->mm_PCHCSSlabLongCenSpliceBarUserLen);
	UserDowelHookLenMapDataCopy(mm_PCHCSSlabLongEndSpliceBarUserLen, pSlabRebarTemplate->mm_PCHCSSlabLongEndSpliceBarUserLen);


	//PC RPS Slab
	m_bPCRPSSlabSpliceDevConditionSet = pSlabRebarTemplate->m_bPCRPSSlabSpliceDevConditionSet;
	m_nPCRPSSlabSpliceDevConditionLength = pSlabRebarTemplate->m_nPCRPSSlabSpliceDevConditionLength;

	m_bPCRPSSlabSpliceAddLengthSet = pSlabRebarTemplate->m_bPCRPSSlabSpliceAddLengthSet;
	m_nPCRPSSlabSpliceAddLengthNumSplice = pSlabRebarTemplate->m_nPCRPSSlabSpliceAddLengthNumSplice;

	m_nPCRPSSlabMaxDualHookLength = pSlabRebarTemplate->m_nPCRPSSlabMaxDualHookLength;
	m_nPCRPSSlabSpliceWay = pSlabRebarTemplate->m_nPCRPSSlabSpliceWay;
	m_nPCRPSSlabSpliceType = pSlabRebarTemplate->m_nPCRPSSlabSpliceType;
	
	m_nPCRPSSlabTopHookType = pSlabRebarTemplate->m_nPCRPSSlabTopHookType;
	m_nPCRPSSlabBotHookType = pSlabRebarTemplate->m_nPCRPSSlabBotHookType;
	m_nPCRPSSlabTopEndDevTypeUserLength = pSlabRebarTemplate->m_nPCRPSSlabTopEndDevTypeUserLength;
	m_nPCRPSSlabBotEndDevTypeUserLength = pSlabRebarTemplate->m_nPCRPSSlabBotEndDevTypeUserLength;
	m_nPCRPSSlabOpeningTopDevType = pSlabRebarTemplate->m_nPCRPSSlabOpeningTopDevType;
	m_nPCRPSSlabOpeningBotDevType = pSlabRebarTemplate->m_nPCRPSSlabOpeningBotDevType;


	m_nPCRPSSlabTopEndAddBarLxLnxType = pSlabRebarTemplate->m_nPCRPSSlabTopEndAddBarLxLnxType;
	m_nPCRPSSlabTopEndAddBarLengthType = pSlabRebarTemplate->m_nPCRPSSlabTopEndAddBarLengthType;
	m_nPCRPSSlabTopCenAddBarLxLnxType = pSlabRebarTemplate->m_nPCRPSSlabTopCenAddBarLxLnxType;
	m_nPCRPSSlabTopCenAddBarLengthType = pSlabRebarTemplate->m_nPCRPSSlabTopCenAddBarLengthType;
	m_nPCRPSSlabDevSubBarLengthType = pSlabRebarTemplate->m_nPCRPSSlabDevSubBarLengthType;
	m_nPCRPSSlabLongCenSpliceBarLengthType = pSlabRebarTemplate->m_nPCRPSSlabLongCenSpliceBarLengthType;
	m_nPCRPSSlabLongEndSpliceBarLengthType = pSlabRebarTemplate->m_nPCRPSSlabLongEndSpliceBarLengthType;

	m_nPCRPSSlabRebarTolDist = pSlabRebarTemplate->m_nPCRPSSlabRebarTolDist;
	m_nPCRPSSlabRebarTolLenDif = pSlabRebarTemplate->m_nPCRPSSlabRebarTolLenDif;

	m_bPCRPSSlabLongCenSpliceBarPlace = pSlabRebarTemplate->m_bPCRPSSlabLongCenSpliceBarPlace;
	m_bPCRPSSlabLongEndSpliceBarPlace = pSlabRebarTemplate->m_bPCRPSSlabLongEndSpliceBarPlace;

	UserDowelHookLenMapDataCopy(mm_PCRPSSlabTopEndAddBarUserLen, pSlabRebarTemplate->mm_PCRPSSlabTopEndAddBarUserLen);
	UserDowelHookLenMapDataCopy(mm_PCRPSSlabTopCenAddBarUserLen, pSlabRebarTemplate->mm_PCRPSSlabTopCenAddBarUserLen);
	UserDowelHookLenMapDataCopy(mm_PCRPSSlabDevSubBarUserLen, pSlabRebarTemplate->mm_PCRPSSlabDevSubBarUserLen);
	UserDowelHookLenMapDataCopy(mm_PCRPSSlabLongCenSpliceBarUserLen, pSlabRebarTemplate->mm_PCRPSSlabLongCenSpliceBarUserLen);
	UserDowelHookLenMapDataCopy(mm_PCRPSSlabLongEndSpliceBarUserLen, pSlabRebarTemplate->mm_PCRPSSlabLongEndSpliceBarUserLen);

	m_bMatBotSpacerUserSteelGradeS = pSlabRebarTemplate->m_bMatBotSpacerUserSteelGradeS;
	m_bMatTopSpacerUserSteelGradeS = pSlabRebarTemplate->m_bMatTopSpacerUserSteelGradeS;
	m_bMatSpacerSupportBarUserSteelGradeS = pSlabRebarTemplate->m_bMatSpacerSupportBarUserSteelGradeS;
	m_bMatSupportRebarUserSteelGradeS = pSlabRebarTemplate->m_bMatSupportRebarUserSteelGradeS;

	m_bFlatBotSpacerUserSteelGradeS = pSlabRebarTemplate->m_bFlatBotSpacerUserSteelGradeS;
	m_bFlatTopSpacerUserSteelGradeS = pSlabRebarTemplate->m_bFlatTopSpacerUserSteelGradeS;
	m_bFlatSpacerSupportBarUserSteelGradeS = pSlabRebarTemplate->m_bFlatSpacerSupportBarUserSteelGradeS;

	m_bSlabBotSpacerUserSteelGradeS = pSlabRebarTemplate->m_bSlabBotSpacerUserSteelGradeS;
	m_bSlabTopSpacerUserSteelGradeS = pSlabRebarTemplate->m_bSlabTopSpacerUserSteelGradeS;
	m_bSlabSpacerSupportBarUserSteelGradeS = pSlabRebarTemplate->m_bSlabSpacerSupportBarUserSteelGradeS;

	m_bSOGBotSpacerUserSteelGradeS = pSlabRebarTemplate->m_bSOGBotSpacerUserSteelGradeS;
	m_bSOGTopSpacerUserSteelGradeS = pSlabRebarTemplate->m_bSOGTopSpacerUserSteelGradeS;
	m_bSOGSpacerSupportBarUserSteelGradeS = pSlabRebarTemplate->m_bSOGSpacerSupportBarUserSteelGradeS;
	m_bSOGSupportRebarUserSteelGradeS = pSlabRebarTemplate->m_bSOGSupportRebarUserSteelGradeS;

	m_bCircularRampBotSpacerUserSteelGradeS = pSlabRebarTemplate->m_bCircularRampBotSpacerUserSteelGradeS;
	m_bCircularRampTopSpacerUserSteelGradeS = pSlabRebarTemplate->m_bCircularRampTopSpacerUserSteelGradeS;
	m_bCircularRampSpacerSupportBarUserSteelGradeS = pSlabRebarTemplate->m_bCircularRampSpacerSupportBarUserSteelGradeS;

	m_nEmbeddedDeckCZoneSpliceWay = pSlabRebarTemplate->m_nEmbeddedDeckCZoneSpliceWay;
	m_nEmbeddedDeckCZoneSpliceLength = pSlabRebarTemplate->m_nEmbeddedDeckCZoneSpliceLength;
	m_nGeneralDeckCZoneSpliceWay = pSlabRebarTemplate->m_nGeneralDeckCZoneSpliceWay;
	m_nGeneralDeckCZoneSpliceLength = pSlabRebarTemplate->m_nGeneralDeckCZoneSpliceLength;
	m_nPCSlabCZoneSpliceWay = pSlabRebarTemplate->m_nPCSlabCZoneSpliceWay;
	m_nPCSlabCZoneSpliceLength = pSlabRebarTemplate->m_nPCSlabCZoneSpliceLength;
	m_nPCIMTSlabCZoneSpliceWay = pSlabRebarTemplate->m_nPCIMTSlabCZoneSpliceWay;
	m_nPCIMTSlabCZoneSpliceLength = pSlabRebarTemplate->m_nPCIMTSlabCZoneSpliceLength;
	m_nPCHCSSlabCZoneSpliceWay = pSlabRebarTemplate->m_nPCHCSSlabCZoneSpliceWay;
	m_nPCHCSSlabCZoneSpliceLength = pSlabRebarTemplate->m_nPCHCSSlabCZoneSpliceLength;
	m_nPCRPSSlabCZoneSpliceWay = pSlabRebarTemplate->m_nPCRPSSlabCZoneSpliceWay;
	m_nPCRPSSlabCZoneSpliceLength = pSlabRebarTemplate->m_nPCRPSSlabCZoneSpliceLength;

	m_bMatTopSpacerPlaceConditionSet = pSlabRebarTemplate->GetMatTopSpacerPlaceConditionSet();
	m_nMatTopSpacerPlaceCondition = pSlabRebarTemplate->GetMatTopSpacerPlaceCondition();
	m_bFlatTopSpacerPlaceConditionSet = pSlabRebarTemplate->GetFlatTopSpacerPlaceConditionSet();
	m_nFlatTopSpacerPlaceCondition = pSlabRebarTemplate->GetFlatTopSpacerPlaceCondition();
	m_bSlabTopSpacerPlaceConditionSet = pSlabRebarTemplate->GetSlabTopSpacerPlaceConditionSet();
	m_nSlabTopSpacerPlaceCondition = pSlabRebarTemplate->GetSlabTopSpacerPlaceCondition();
	m_bSOGTopSpacerPlaceConditionSet = pSlabRebarTemplate->GetSOGTopSpacerPlaceConditionSet();
	m_nSOGTopSpacerPlaceCondition = pSlabRebarTemplate->GetSOGTopSpacerPlaceCondition();
	m_bCircularRampTopSpacerPlaceConditionSet = pSlabRebarTemplate->GetCircularRampTopSpacerPlaceConditionSet();
	m_nCircularRampTopSpacerPlaceCondition = pSlabRebarTemplate->GetCircularRampTopSpacerPlaceCondition();

	m_bSlabOpeningAddBarDeductionCondition = pSlabRebarTemplate->GetSlabOpeningAddBarDeductionCondition();
	m_nSlabOpeningAddBarDeductionPercent = pSlabRebarTemplate->GetSlabOpeningAddBarDeductionPercent();

	mm_MatThickToDiaIndex = pSlabRebarTemplate->mm_MatThickToDiaIndex;
	mm_FlatThickToDiaIndex = pSlabRebarTemplate->mm_FlatThickToDiaIndex;
	mm_SlabThickToDiaIndex = pSlabRebarTemplate->mm_SlabThickToDiaIndex;
	mm_SOGThickToDiaIndex = pSlabRebarTemplate->mm_SOGThickToDiaIndex;
	mm_CircularRampThickToDiaIndex = pSlabRebarTemplate->mm_CircularRampThickToDiaIndex;

	m_bSlabLongEndPlaceAreaSet = pSlabRebarTemplate->GetSlabLongEndPlaceAreaSet();
	m_nSlabLongEndPlaceAreaOneWaySlabType = pSlabRebarTemplate->GetSlabLongEndPlaceAreaOneWaySlabType();
	m_nSlabLongEndPlaceAreaTwoWaySlabType = pSlabRebarTemplate->GetSlabLongEndPlaceAreaTwoWaySlabType();
	m_bSlabBotAddBarLengthSet = pSlabRebarTemplate->GetSlabBotAddBarLengthSet();
	m_nSlabBotAddBarLengthType = pSlabRebarTemplate->GetSlabBotAddBarLengthType();
	m_bSlabBotAddBarCutBarLenExclusionSet = pSlabRebarTemplate->GetSlabBotAddBarCutBarLenExclusionSet();

	m_nSlabOpeningAddBarDevLengthType = pSlabRebarTemplate->GetSlabOpeningAddBarDevLengthType();
}

void SlabRebarTemplate::Serialize(CArchive& ar)
{
	BOOL bDefaultValue = true;
	long nDefaultValue = 0;
	double dDefaultValue = 0.;
	long nMatTopSpacerDiaIndex = 0, nFlatTopSpacerDiaIndex = 0, nSlabTopSpacerDiaIndex = 0;
	long nSOGTopSpacerDiaIndex = 0, nCircularRampTopSpacerDiaIndex = 0;
	map<long, long>::iterator it;

	if(ar.IsStoring())
	{
		// MAT
		ar << bDefaultValue;
		ar << nDefaultValue;

		ar << bDefaultValue;
		ar << m_nMatIsoFootDevType;

		ar << m_bMatLevelDifInfoSet;
		ar << nDefaultValue;
		ar << nDefaultValue;

		ar << m_bMatEndInfoSet;
		ar << m_nMatTopEndHookType;
		ar << m_nMatBotEndHookType;

		ar << bDefaultValue;
		ar << nDefaultValue;
		ar << bDefaultValue;
		ar << bDefaultValue;
		ar << nDefaultValue;
		ar << bDefaultValue;
		ar << bDefaultValue;

		ar << m_bMatRebarAreaInfoSet;
		ar << m_nMatRebarTolDist;
		ar << m_nMatRebarTolLenDif;

		// FLAT
		ar << bDefaultValue;
		ar << nDefaultValue;

		ar << bDefaultValue;
		ar << m_nFlatIsoFootDevType;

		ar << m_bFlatOpeningInfoSet;
		ar << m_nFlatOpeningTopDevType;
		ar << m_nFlatOpeningBotDevType;

		ar << m_bFlatLevelDifInfoSet;
		ar << nDefaultValue;
		ar << nDefaultValue;

		ar << m_bFlatEndInfoSet;
		ar << m_nFlatTopEndHookType;
		ar << m_nFlatBotEndHookType;

		ar << bDefaultValue;
		ar << nDefaultValue;
		ar << bDefaultValue;
		ar << bDefaultValue;
		ar << nDefaultValue;
		ar << bDefaultValue;
		ar << bDefaultValue;
		
		ar << m_bFlatRebarAreaInfoSet;
		ar << m_nFlatRebarTolDist;
		ar << m_nFlatRebarTolLenDif;

		// SLAB
		ar << bDefaultValue;
		ar << nDefaultValue;
		
		ar << m_bSlabTopCutBarLenInfoSet;
		ar << m_nSlabOneWayCutBarLenType;
		ar << m_nSlabTwoWayCutBarLenType;

		// Mat
		ar << m_nMatLevelDifPlaceShape;
		ar << m_nMatLevelDifHaunchPlaceType;
		ar << m_bMatMainBarInfoSet;
		ar << m_nMatMaxDualHookLength;
		ar << m_nMatSpliceWay;
		ar << m_nMatSpliceType;

		// Flat
		ar << m_nFlatLevelDifPlaceShape;
		ar << m_nFlatLevelDifHaunchPlaceType;
		ar << m_bFlatMainBarInfoSet;
		ar << m_nFlatMaxDualHookLength;
		ar << m_nFlatSpliceWay;
		ar << m_nFlatSpliceType;

		// Slab
		ar << m_bSlabMainBarInfoSet;
		ar << m_nSlabMaxDualHookLength;
		ar << m_nSlabSpliceWay;
		ar << m_nSlabSpliceType;
		ar << nDefaultValue;
		ar << nDefaultValue;

		ar << bDefaultValue;
		ar << m_nSOGSpliceDevSlabType;
		ar << m_nFlatSpliceDevSlabType;
		ar << m_nCircularRampSpliceDevSlabType;

		ar << m_bSlabUnStructuredEndInfoSet;
		ar << m_nSlabTopUnStructuredEndHookType;
		ar << m_nSlabBotUnStructuredEndHookType;

		ar << m_bSlabLevelDifInfoSet;
		ar << m_nSlabLevelDifPlaceShape;
		ar << m_nSlabLevelDifHaunchPlaceType;

		ar << m_bSlabOpeningInfoSet;
		ar << m_nSlabOpeningTopDevType;
		ar << m_nSlabOpeningBotDevType;

		ar << m_bSlabRebarAreaInfoSet;
		ar << m_nSlabRebarTolDist;
		ar << m_nSlabRebarTolLenDif;

		// SOG Slab
		ar << m_bSOGMainBarInfoSet;
		ar << m_nSOGMaxDualHookLength;
		ar << m_nSOGSpliceWay;
		ar << m_nSOGSpliceType;

		ar << m_bSOGEndInfoSet;
		ar << m_nSOGTopEndHookType;
		ar << m_nSOGBotEndHookType;
		ar << m_nSOGIsoFootDevType;

		ar << m_bSOGLevelDifInfoSet;
		ar << m_nSOGLevelDifPlaceShape;
		ar << m_nSOGLevelDifHaunchPlaceType;

		ar << m_bSOGOpeningInfoSet;
		ar << m_nSOGOpeningTopDevType;
		ar << m_nSOGOpeningBotDevType;

		ar << m_bSOGRebarAreaInfoSet;
		ar << m_nSOGRebarTolDist;
		ar << m_nSOGRebarTolLenDif;

		// CircularRamp
		ar << m_bCircularRampMainBarInfoSet;
		ar << m_nCircularRampMaxDualHookLength;
		ar << m_nCircularRampSpliceWay;
		ar << m_nCircularRampSpliceType;

		ar << m_bCircularRampEndInfoSet;
		ar << m_nCircularRampTopEndHookType;
		ar << m_nCircularRampBotEndHookType;
		ar << m_nCircularRampIsoFootDevType;

		ar << m_bCircularRampLevelDifInfoSet;
		ar << m_nCircularRampLevelDifPlaceShape;
		ar << m_nCircularRampLevelDifHaunchPlaceType;

		ar << m_bCircularRampOpeningInfoSet;
		ar << m_nCircularRampOpeningTopDevType;
		ar << m_nCircularRampOpeningBotDevType;

		ar << m_bCircularRampRebarAreaInfoSet;
		ar << m_nCircularRampRebarTolDist;
		ar << m_nCircularRampRebarTolLenDif;

		// PC Slab
		ar << m_bPCSlabMainBarInfoSet;
		ar << m_nPCSlabMaxDualHookLength;
		ar << m_nPCSlabSpliceWay;
		ar << m_nPCSlabSpliceType;
		
		ar << m_bPCSlabUnStructuredEndInfoSet;
		ar << m_nPCSlabTopUnStructuredEndHookType;
		ar << m_nPCSlabBotUnStructuredEndHookType;
		
		ar << m_bPCSlabLevelDifInfoSet;
		ar << m_nPCSlabLevelDifPlaceShape;
		ar << m_nPCSlabLevelDifHaunchPlaceType;

		ar << m_bPCSlabOpeningInfoSet;
		ar << m_nPCSlabOpeningTopDevType;
		ar << m_nPCSlabOpeningBotDevType;

		ar << m_bPCSlabRebarAreaInfoSet;
		ar << m_nPCSlabRebarTolDist;
		ar << m_nPCSlabRebarTolLenDif;

		// 슬래브 보강근
		ar << m_nSlabRebarRebarTolDist;
		ar << m_nSlabRebarRebarTolLenDif;

		// 철근 일체형 데크 슬래브
		ar << m_bEmbeddedDeckMainBarInfoSet;
		ar << m_nEmbeddedDeckMaxDualHookLength;
		ar << m_nEmbeddedDeckSpliceWay;
		ar << m_nEmbeddedDeckSpliceType;
		
		ar << m_bEmbeddedDeckUnStructuredEndInfoSet;
		ar << m_nEmbeddedDeckTopUnStructuredEndHookType;
		ar << m_nEmbeddedDeckBotUnStructuredEndHookType;

		ar << m_bEmbeddedDeckLevelDifInfoSet;
		ar << m_nEmbeddedDeckLevelDifPlaceShape;
		ar << m_nEmbeddedDeckLevelDifHaunchPlaceType;

		ar << m_bEmbeddedDeckOpeningInfoSet;
		ar << m_nEmbeddedDeckOpeningTopDevType;
		ar << m_nEmbeddedDeckOpeningBotDevType;

		ar << m_bEmbeddedDeckRebarAreaInfoSet;
		ar << m_nEmbeddedDeckRebarTolDist;
		ar << m_nEmbeddedDeckRebarTolLenDif;

		// 일반 데크 슬래브
		ar << m_bGeneralDeckMainBarInfoSet;
		ar << m_nGeneralDeckMaxDualHookLength;
		ar << m_nGeneralDeckSpliceWay;
		ar << m_nGeneralDeckSpliceType;
		
		ar << m_bGeneralDeckUnStructuredEndInfoSet;
		ar << m_nGeneralDeckTopUnStructuredEndHookType;
		ar << m_nGeneralDeckBotUnStructuredEndHookType;		

		ar << m_bGeneralDeckLevelDifInfoSet;
		ar << m_nGeneralDeckLevelDifPlaceShape;
		ar << m_nGeneralDeckLevelDifHaunchPlaceType;

		ar << m_bGeneralDeckOpeningInfoSet;
		ar << m_nGeneralDeckOpeningTopDevType;
		ar << m_nGeneralDeckOpeningBotDevType;

		ar << m_bGeneralDeckRebarAreaInfoSet;
		ar << m_nGeneralDeckRebarTolDist;
		ar << m_nGeneralDeckRebarTolLenDif;

		ar << m_bMatTopSpacerInfoSet;
		ar << m_bMatTopSpacerSet;
		ar << m_nMatTopSpacerDiaType;
		ar << nDefaultValue;
		ar << m_nMatTopSpacerSpacingType;
		ar << m_nMatTopSpacerSlabSpacing;
		ar << m_nMatTopSpacerUserSpacing1;
		ar << m_nMatTopSpacerUserSpacing2;
		ar << m_nMatTopSpacerHeightType;
		ar << m_nMatTopSpacerUserHeight;
		ar << m_nMatTopSpacerHookLenAType;
		ar << m_nMatTopSpacerMainBarHookLenA;
		ar << m_nMatTopSpacerUserHookLenA;
		ar << m_nMatTopSpacerHookLenBType;
		ar << m_nMatTopSpacerMainBarHookLenB;
		ar << m_nMatTopSpacerUserHookLenB;
		ar << m_nMatTopSpacerWidth;

		ar << m_bMatBotSpacerInfoSet;
		ar << m_bMatBotSpacerSet;
		ar << m_nMatBotSpacerDiaType;
		ar << m_nMatBotSpacerDiaIndex;
		ar << m_nMatBotSpacerSpacingType;
		ar << m_nMatBotSpacerSlabSpacing;
		ar << m_nMatBotSpacerUserSpacing1;
		ar << m_nMatBotSpacerUserSpacing2;
		ar << m_nMatBotSpacerHeightType;
		ar << m_nMatBotSpacerUserHeight;
		ar << m_nMatBotSpacerHookLenAType;
		ar << m_nMatBotSpacerMainBarHookLenA;
		ar << m_nMatBotSpacerUserHookLenA;
		ar << m_nMatBotSpacerHookLenBType;
		ar << m_nMatBotSpacerMainBarHookLenB;
		ar << m_nMatBotSpacerUserHookLenB;

		ar << m_bFlatTopSpacerInfoSet;
		ar << m_bFlatTopSpacerSet;
		ar << m_nFlatTopSpacerDiaType;
		ar << nDefaultValue;
		ar << m_nFlatTopSpacerSpacingType;
		ar << m_nFlatTopSpacerSlabSpacing;
		ar << m_nFlatTopSpacerUserSpacing1;
		ar << m_nFlatTopSpacerUserSpacing2;
		ar << m_nFlatTopSpacerHeightType;
		ar << m_nFlatTopSpacerUserHeight;
		ar << m_nFlatTopSpacerHookLenAType;
		ar << m_nFlatTopSpacerMainBarHookLenA;
		ar << m_nFlatTopSpacerUserHookLenA;
		ar << m_nFlatTopSpacerHookLenBType;
		ar << m_nFlatTopSpacerMainBarHookLenB;
		ar << m_nFlatTopSpacerUserHookLenB;
		ar << m_nFlatTopSpacerWidth;

		ar << m_bFlatBotSpacerInfoSet;
		ar << m_bFlatBotSpacerSet;
		ar << m_nFlatBotSpacerDiaType;
		ar << m_nFlatBotSpacerDiaIndex;
		ar << m_nFlatBotSpacerSpacingType;
		ar << m_nFlatBotSpacerSlabSpacing;
		ar << m_nFlatBotSpacerUserSpacing1;
		ar << m_nFlatBotSpacerUserSpacing2;
		ar << m_nFlatBotSpacerHeightType;
		ar << m_nFlatBotSpacerUserHeight;
		ar << m_nFlatBotSpacerHookLenAType;
		ar << m_nFlatBotSpacerMainBarHookLenA;
		ar << m_nFlatBotSpacerUserHookLenA;
		ar << m_nFlatBotSpacerHookLenBType;
		ar << m_nFlatBotSpacerMainBarHookLenB;
		ar << m_nFlatBotSpacerUserHookLenB;

		ar << m_bSlabTopSpacerInfoSet;
		ar << m_bSlabTopSpacerSet;
		ar << m_nSlabTopSpacerDiaType;
		ar << nDefaultValue;
		ar << m_nSlabTopSpacerSpacingType;
		ar << m_nSlabTopSpacerSlabSpacing;
		ar << m_nSlabTopSpacerUserSpacing1;
		ar << m_nSlabTopSpacerUserSpacing2;
		ar << m_nSlabTopSpacerHeightType;
		ar << m_nSlabTopSpacerUserHeight;
		ar << m_nSlabTopSpacerHookLenAType;
		ar << m_nSlabTopSpacerMainBarHookLenA;
		ar << m_nSlabTopSpacerUserHookLenA;
		ar << m_nSlabTopSpacerHookLenBType;
		ar << m_nSlabTopSpacerMainBarHookLenB;
		ar << m_nSlabTopSpacerUserHookLenB;
		ar << m_nSlabTopSpacerWidth;

		ar << m_bSlabBotSpacerInfoSet;
		ar << m_bSlabBotSpacerSet;
		ar << m_nSlabBotSpacerDiaType;
		ar << m_nSlabBotSpacerDiaIndex;
		ar << m_nSlabBotSpacerSpacingType;
		ar << m_nSlabBotSpacerSlabSpacing;
		ar << m_nSlabBotSpacerUserSpacing1;
		ar << m_nSlabBotSpacerUserSpacing2;
		ar << m_nSlabBotSpacerHeightType;
		ar << m_nSlabBotSpacerUserHeight;
		ar << m_nSlabBotSpacerHookLenAType;
		ar << m_nSlabBotSpacerMainBarHookLenA;
		ar << m_nSlabBotSpacerUserHookLenA;
		ar << m_nSlabBotSpacerHookLenBType;
		ar << m_nSlabBotSpacerMainBarHookLenB;
		ar << m_nSlabBotSpacerUserHookLenB;

		ar << m_bSOGTopSpacerInfoSet;
		ar << m_bSOGTopSpacerSet;
		ar << m_nSOGTopSpacerDiaType;
		ar << nDefaultValue;
		ar << m_nSOGTopSpacerSpacingType;
		ar << m_nSOGTopSpacerSlabSpacing;
		ar << m_nSOGTopSpacerUserSpacing1;
		ar << m_nSOGTopSpacerUserSpacing2;
		ar << m_nSOGTopSpacerHeightType;
		ar << m_nSOGTopSpacerUserHeight;
		ar << m_nSOGTopSpacerHookLenAType;
		ar << m_nSOGTopSpacerMainBarHookLenA;
		ar << m_nSOGTopSpacerUserHookLenA;
		ar << m_nSOGTopSpacerHookLenBType;
		ar << m_nSOGTopSpacerMainBarHookLenB;
		ar << m_nSOGTopSpacerUserHookLenB;
		ar << m_nSOGTopSpacerWidth;

		ar << m_bSOGBotSpacerInfoSet;
		ar << m_bSOGBotSpacerSet;
		ar << m_nSOGBotSpacerDiaType;
		ar << m_nSOGBotSpacerDiaIndex;
		ar << m_nSOGBotSpacerSpacingType;
		ar << m_nSOGBotSpacerSlabSpacing;
		ar << m_nSOGBotSpacerUserSpacing1;
		ar << m_nSOGBotSpacerUserSpacing2;
		ar << m_nSOGBotSpacerHeightType;
		ar << m_nSOGBotSpacerUserHeight;
		ar << m_nSOGBotSpacerHookLenAType;
		ar << m_nSOGBotSpacerMainBarHookLenA;
		ar << m_nSOGBotSpacerUserHookLenA;
		ar << m_nSOGBotSpacerHookLenBType;
		ar << m_nSOGBotSpacerMainBarHookLenB;
		ar << m_nSOGBotSpacerUserHookLenB;

		ar << m_bCircularRampTopSpacerInfoSet;
		ar << m_bCircularRampTopSpacerSet;
		ar << m_nCircularRampTopSpacerDiaType;
		ar << nDefaultValue;
		ar << m_nCircularRampTopSpacerSpacingType;
		ar << m_nCircularRampTopSpacerSlabSpacing;
		ar << m_nCircularRampTopSpacerUserSpacing1;
		ar << m_nCircularRampTopSpacerUserSpacing2;
		ar << m_nCircularRampTopSpacerHeightType;
		ar << m_nCircularRampTopSpacerUserHeight;
		ar << m_nCircularRampTopSpacerHookLenAType;
		ar << m_nCircularRampTopSpacerMainBarHookLenA;
		ar << m_nCircularRampTopSpacerUserHookLenA;
		ar << m_nCircularRampTopSpacerHookLenBType;
		ar << m_nCircularRampTopSpacerMainBarHookLenB;
		ar << m_nCircularRampTopSpacerUserHookLenB;
		ar << m_nCircularRampTopSpacerWidth;

		ar << m_bCircularRampBotSpacerInfoSet;
		ar << m_bCircularRampBotSpacerSet;
		ar << m_nCircularRampBotSpacerDiaType;
		ar << m_nCircularRampBotSpacerDiaIndex;
		ar << m_nCircularRampBotSpacerSpacingType;
		ar << m_nCircularRampBotSpacerSlabSpacing;
		ar << m_nCircularRampBotSpacerUserSpacing1;
		ar << m_nCircularRampBotSpacerUserSpacing2;
		ar << m_nCircularRampBotSpacerHeightType;
		ar << m_nCircularRampBotSpacerUserHeight;
		ar << m_nCircularRampBotSpacerHookLenAType;
		ar << m_nCircularRampBotSpacerMainBarHookLenA;
		ar << m_nCircularRampBotSpacerUserHookLenA;
		ar << m_nCircularRampBotSpacerHookLenBType;
		ar << m_nCircularRampBotSpacerMainBarHookLenB;
		ar << m_nCircularRampBotSpacerUserHookLenB;

		ar << m_bPCSlabTopSpacerInfoSet;
		ar << m_bPCSlabTopSpacerSet;
		ar << m_nPCSlabTopSpacerDiaType;
		ar << m_nPCSlabTopSpacerDiaIndex;
		ar << m_nPCSlabTopSpacerSpacingType;
		ar << m_nPCSlabTopSpacerSlabSpacing;
		ar << m_nPCSlabTopSpacerUserSpacing1;
		ar << m_nPCSlabTopSpacerUserSpacing2;
		ar << m_nPCSlabTopSpacerHeightType;
		ar << m_nPCSlabTopSpacerUserHeight;
		ar << m_nPCSlabTopSpacerHookLenAType;
		ar << m_nPCSlabTopSpacerMainBarHookLenA;
		ar << m_nPCSlabTopSpacerUserHookLenA;
		ar << m_nPCSlabTopSpacerHookLenBType;
		ar << m_nPCSlabTopSpacerMainBarHookLenB;
		ar << m_nPCSlabTopSpacerUserHookLenB;
		ar << m_nPCSlabTopSpacerWidth;

		ar << m_bPCSlabBotSpacerInfoSet;
		ar << m_bPCSlabBotSpacerSet;
		ar << m_nPCSlabBotSpacerDiaType;
		ar << m_nPCSlabBotSpacerDiaIndex;
		ar << m_nPCSlabBotSpacerSpacingType;
		ar << m_nPCSlabBotSpacerSlabSpacing;
		ar << m_nPCSlabBotSpacerUserSpacing1;
		ar << m_nPCSlabBotSpacerUserSpacing2;
		ar << m_nPCSlabBotSpacerHeightType;
		ar << m_nPCSlabBotSpacerUserHeight;
		ar << m_nPCSlabBotSpacerHookLenAType;
		ar << m_nPCSlabBotSpacerMainBarHookLenA;
		ar << m_nPCSlabBotSpacerUserHookLenA;
		ar << m_nPCSlabBotSpacerHookLenBType;
		ar << m_nPCSlabBotSpacerMainBarHookLenB;
		ar << m_nPCSlabBotSpacerUserHookLenB;

		ar << m_nMatBotSpacerWidth;
		ar << m_nFlatBotSpacerWidth;
		ar << m_nSlabBotSpacerWidth;
		ar << m_nSOGBotSpacerWidth;
		ar << m_nCircularRampBotSpacerWidth;
		ar << m_nPCSlabBotSpacerWidth;

		ar << m_bMatSpacerSupportBarsSet;
		ar << m_bMatSpacerSupportBarsConditionSet;
		ar << m_nMatSpacerSupportBarsCondition;
		ar << m_nMatSpacerSupportBarsDiaType;
		ar << m_nMatSpacerSupportBarsUserDiaIndex;
		ar << m_nMatSpacerSupportBarsNumType;
		ar << m_dMatSpacerSupportBarsTopSpacerNums;
		ar << m_nMatSpacerSupportBarsUserNums;
		ar << m_nMatSpacerSupportBarsLengthType;
		ar << m_nMatSpacerSupportBarsUserLength;

		ar << m_bFlatSpacerSupportBarsSet;
		ar << m_bFlatSpacerSupportBarsConditionSet;
		ar << m_nFlatSpacerSupportBarsCondition;
		ar << m_nFlatSpacerSupportBarsDiaType;
		ar << m_nFlatSpacerSupportBarsUserDiaIndex;
		ar << m_nFlatSpacerSupportBarsNumType;
		ar << m_dFlatSpacerSupportBarsTopSpacerNums;
		ar << m_nFlatSpacerSupportBarsUserNums;
		ar << m_nFlatSpacerSupportBarsLengthType;
		ar << m_nFlatSpacerSupportBarsUserLength;

		ar << m_bSlabSpacerSupportBarsSet;
		ar << m_bSlabSpacerSupportBarsConditionSet;
		ar << m_nSlabSpacerSupportBarsCondition;
		ar << m_nSlabSpacerSupportBarsDiaType;
		ar << m_nSlabSpacerSupportBarsUserDiaIndex;
		ar << m_nSlabSpacerSupportBarsNumType;
		ar << m_dSlabSpacerSupportBarsTopSpacerNums;
		ar << m_nSlabSpacerSupportBarsUserNums;
		ar << m_nSlabSpacerSupportBarsLengthType;
		ar << m_nSlabSpacerSupportBarsUserLength;

		ar << m_bSOGSpacerSupportBarsSet;
		ar << m_bSOGSpacerSupportBarsConditionSet;
		ar << m_nSOGSpacerSupportBarsCondition;
		ar << m_nSOGSpacerSupportBarsDiaType;
		ar << m_nSOGSpacerSupportBarsUserDiaIndex;
		ar << m_nSOGSpacerSupportBarsNumType;
		ar << m_dSOGSpacerSupportBarsTopSpacerNums;
		ar << m_nSOGSpacerSupportBarsUserNums;
		ar << m_nSOGSpacerSupportBarsLengthType;
		ar << m_nSOGSpacerSupportBarsUserLength;

		ar << m_bCircularRampSpacerSupportBarsSet;
		ar << m_bCircularRampSpacerSupportBarsConditionSet;
		ar << m_nCircularRampSpacerSupportBarsCondition;
		ar << m_nCircularRampSpacerSupportBarsDiaType;
		ar << m_nCircularRampSpacerSupportBarsUserDiaIndex;
		ar << m_nCircularRampSpacerSupportBarsNumType;
		ar << m_dCircularRampSpacerSupportBarsTopSpacerNums;
		ar << m_nCircularRampSpacerSupportBarsUserNums;
		ar << m_nCircularRampSpacerSupportBarsLengthType;
		ar << m_nCircularRampSpacerSupportBarsUserLength;

		ar << m_bMatSupportRebarsSet;
		ar << m_nMatSupportRebarsDiaIndex;
		ar << m_nMatSupportRebarsLengthType;
		ar << m_dMatSupportRebarsRWallLengthRate;
		ar << m_dMatSupportRebarsSlabLengthRate;
		ar << m_nMatSupportRebarsOrderLength;
		ar << m_nMatSupportRebarsSpliceLength;
		ar << m_nMatSupportRebarsCorrectiveLength;
		
		ar << m_bSOGSupportRebarsSet;
		ar << m_nSOGSupportRebarsDiaIndex;
		ar << m_nSOGSupportRebarsLengthType;
		ar << m_dSOGSupportRebarsRWallLengthRate;
		ar << m_dSOGSupportRebarsSlabLengthRate;
		ar << m_nSOGSupportRebarsOrderLength;
		ar << m_nSOGSupportRebarsSpliceLength;
		ar << m_nSOGSupportRebarsCorrectiveLength;
		
		ar << m_nMatTopSpacerSlabThick;
		ar << m_nMatTopSpacerSlabThickOverDiaIndex;
		ar << m_nMatTopSpacerSlabThickUnderDiaIndex;

		ar << m_nFlatTopSpacerSlabThick;
		ar << m_nFlatTopSpacerSlabThickOverDiaIndex;
		ar << m_nFlatTopSpacerSlabThickUnderDiaIndex;

		ar << m_nSlabTopSpacerSlabThick;
		ar << m_nSlabTopSpacerSlabThickOverDiaIndex;
		ar << m_nSlabTopSpacerSlabThickUnderDiaIndex;

		ar << m_nSOGTopSpacerSlabThick;
		ar << m_nSOGTopSpacerSlabThickOverDiaIndex;
		ar << m_nSOGTopSpacerSlabThickUnderDiaIndex;

		ar << m_nCircularRampTopSpacerSlabThick;
		ar << m_nCircularRampTopSpacerSlabThickOverDiaIndex;
		ar << m_nCircularRampTopSpacerSlabThickUnderDiaIndex;

		ar << m_bMatSpliceDevConditionSet;
		ar << m_nMatSpliceDevConditionLength;
		ar << m_bFlatSpliceDevConditionSet;
		ar << m_nFlatSpliceDevConditionLength;
		ar << m_bSlabSpliceDevConditionSet;
		ar << m_nSlabSpliceDevConditionLength;
		ar << m_bSOGSpliceDevConditionSet;
		ar << m_nSOGSpliceDevConditionLength;
		ar << m_bCircularRampSpliceDevConditionSet;
		ar << m_nCircularRampSpliceDevConditionLength;
		ar << m_bPCSlabSpliceDevConditionSet;
		ar << m_nPCSlabSpliceDevConditionLength;
		ar << m_bEmbeddedDeckSpliceDevConditionSet;
		ar << m_nEmbeddedDeckSpliceDevConditionLength;
		ar << m_bGeneralDeckSpliceDevConditionSet;
		ar << m_nGeneralDeckSpliceDevConditionLength;

		// PC IMT Slab
		ar << m_bPCIMTSlabSpliceDevConditionSet;
		ar << m_nPCIMTSlabSpliceDevConditionLength;

		ar << m_nPCIMTSlabMaxDualHookLength;
		ar << m_nPCIMTSlabSpliceWay;
		ar << m_nPCIMTSlabSpliceType;
									
		ar << m_nPCIMTSlabRebarTolDist;
		ar << m_nPCIMTSlabRebarTolLenDif;
		
		ar << m_dMatSpacerSupportBarsUserRatio;
		ar << m_dFlatSpacerSupportBarsUserRatio;
		ar << m_dSlabSpacerSupportBarsUserRatio;
		ar << m_dSOGSpacerSupportBarsUserRatio;
		ar << m_dCircularRampSpacerSupportBarsUserRatio;


		ar << m_bMatSpliceAddLengthSet;
		ar << m_nMatSpliceAddLengthNumSplice;
		ar << m_bFlatSpliceAddLengthSet;
		ar << m_nFlatSpliceAddLengthNumSplice;
		ar << m_bSOGSpliceAddLengthSet;
		ar << m_nSOGSpliceAddLengthNumSplice;

		ar << m_bSlabSpliceAddLengthSet;
		ar << m_nSlabSpliceAddLengthNumSplice;
		ar << m_bCircularRampSpliceAddLengthSet;
		ar << m_nCircularRampSpliceAddLengthNumSplice;
		ar << m_bEmbeddedDeckSpliceAddLengthSet;
		ar << m_nEmbeddedDeckSpliceAddLengthNumSplice;
		ar << m_bGeneralDeckSpliceAddLengthSet;
		ar << m_nGeneralDeckSpliceAddLengthNumSplice;
		ar << m_bPCSlabSpliceAddLengthSet;
		ar << m_nPCSlabSpliceAddLengthNumSplice;
		ar << m_bPCIMTSlabSpliceAddLengthSet;
		ar << m_nPCIMTSlabSpliceAddLengthNumSplice;

		ar << m_nPCIMTSlabLongSpliceBarType;

		m_pPCSlabHorSupportRebarsTemplate->Serialize(ar);
		m_pPCIMTSlabHorSupportRebarsTemplate->Serialize(ar);
		m_pGeneralDeckHorSupportRebarsTemplate->Serialize(ar);
		m_pEmbeddedDeckHorSupportRebarsTemplate->Serialize(ar);
		
		ar << (int)mm_EmbeddedDeckSpliceBarUserLen.size();
		for (it = mm_EmbeddedDeckSpliceBarUserLen.begin(); it != mm_EmbeddedDeckSpliceBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}
		
		ar << m_bPCHCSSlabSpliceDevConditionSet;
		ar << m_nPCHCSSlabSpliceDevConditionLength;
		ar << m_bPCHCSSlabSpliceAddLengthSet;
		ar << m_nPCHCSSlabSpliceAddLengthNumSplice;
		ar << m_nPCHCSSlabMaxDualHookLength;	
		ar << m_nPCHCSSlabSpliceWay;	
		ar << m_nPCHCSSlabSpliceType;
		
		ar << m_nPCHCSSlabTopEndAddBarLengthType;
		ar << m_nPCHCSSlabDevSubBarLengthType;
		ar << m_nPCHCSSlabShortCenSpliceBarLengthType;
		ar << m_nPCHCSSlabLongCenSpliceBarLengthType;

		ar << m_nPCHCSSlabRebarTolDist;
		ar << m_nPCHCSSlabRebarTolLenDif;

		m_pPCHCSSlabHorSupportRebarsTemplate->Serialize(ar);

		ar << (int)mm_PCHCSSlabShortCenSpliceBarUserLen.size();
		for (it = mm_PCHCSSlabShortCenSpliceBarUserLen.begin(); it != mm_PCHCSSlabShortCenSpliceBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}

		ar << (int)mm_PCHCSSlabLongCenSpliceBarUserLen.size();
		for (it = mm_PCHCSSlabLongCenSpliceBarUserLen.begin(); it != mm_PCHCSSlabLongCenSpliceBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}

		ar << (int)mm_PCHCSSlabTopEndAddBarUserLen.size();
		for (it = mm_PCHCSSlabTopEndAddBarUserLen.begin(); it != mm_PCHCSSlabTopEndAddBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}

		ar << (int)mm_PCHCSSlabDevSubBarUserLen.size();
		for (it = mm_PCHCSSlabDevSubBarUserLen.begin(); it != mm_PCHCSSlabDevSubBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}
		//RPS
		ar << m_bPCRPSSlabSpliceDevConditionSet;
		ar << m_nPCRPSSlabSpliceDevConditionLength;
		ar << m_bPCRPSSlabSpliceAddLengthSet;
		ar << m_nPCRPSSlabSpliceAddLengthNumSplice;
		ar << m_nPCRPSSlabMaxDualHookLength;	
		ar << m_nPCRPSSlabSpliceWay;	
		ar << m_nPCRPSSlabSpliceType;
				
		ar << m_nPCRPSSlabTopEndAddBarLengthType;
		ar << m_nPCRPSSlabDevSubBarLengthType;
		ar << m_nPCRPSSlabLongCenSpliceBarLengthType;

		ar << m_nPCRPSSlabRebarTolDist;
		ar << m_nPCRPSSlabRebarTolLenDif;

		m_pPCRPSSlabHorSupportRebarsTemplate->Serialize(ar);
		
		ar << (int)mm_PCRPSSlabLongCenSpliceBarUserLen.size();
		for (it = mm_PCRPSSlabLongCenSpliceBarUserLen.begin(); it != mm_PCRPSSlabLongCenSpliceBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}

		ar << (int)mm_PCRPSSlabTopEndAddBarUserLen.size();
		for (it = mm_PCRPSSlabTopEndAddBarUserLen.begin(); it != mm_PCRPSSlabTopEndAddBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}

		ar << (int)mm_PCRPSSlabDevSubBarUserLen.size();
		for (it = mm_PCRPSSlabDevSubBarUserLen.begin(); it != mm_PCRPSSlabDevSubBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}
		//
		ar << m_nPCHCSSlabTopEndAddBarLxLnxType;
		ar << m_nPCRPSSlabTopEndAddBarLxLnxType;

		//Q22-806
		ar <<  m_bPCHCSSlabShortCenSpliceBarPlace;
		ar <<  m_bPCHCSSlabShortEndSpliceBarPlace;
		ar <<  m_bPCHCSSlabLongCenSpliceBarPlace;
		ar <<  m_bPCHCSSlabLongEndSpliceBarPlace;
		ar << m_nPCHCSSlabShortEndSpliceBarLengthType;
		ar << m_nPCHCSSlabLongEndSpliceBarLengthType;
		ar << (int)mm_PCHCSSlabShortEndSpliceBarUserLen.size();
		for (it = mm_PCHCSSlabShortEndSpliceBarUserLen.begin(); it != mm_PCHCSSlabShortEndSpliceBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}

		ar << (int)mm_PCHCSSlabLongEndSpliceBarUserLen.size();
		for (it = mm_PCHCSSlabLongEndSpliceBarUserLen.begin(); it != mm_PCHCSSlabLongEndSpliceBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}

		ar << m_nPCRPSSlabLongEndSpliceBarLengthType;
		ar <<  m_bPCRPSSlabLongCenSpliceBarPlace;
		ar <<  m_bPCRPSSlabLongEndSpliceBarPlace;
		ar << (int)mm_PCRPSSlabLongEndSpliceBarUserLen.size();
		for (it = mm_PCRPSSlabLongEndSpliceBarUserLen.begin(); it != mm_PCRPSSlabLongEndSpliceBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}
		//Q22-875 골데크
		ar << m_nPCHCSSlabTopCenAddBarLxLnxType;
		ar << m_nPCHCSSlabTopCenAddBarLengthType;
		ar << (int)mm_PCHCSSlabTopCenAddBarUserLen.size();
		for (it = mm_PCHCSSlabTopCenAddBarUserLen.begin(); it != mm_PCHCSSlabTopCenAddBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}
		ar << m_nPCRPSSlabTopCenAddBarLxLnxType;
		ar << m_nPCRPSSlabTopCenAddBarLengthType;
		ar << (int)mm_PCRPSSlabTopCenAddBarUserLen.size();
		for (it = mm_PCRPSSlabTopCenAddBarUserLen.begin(); it != mm_PCRPSSlabTopCenAddBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}

		ar << m_nGeneralDeckTopEndAddBarLxLnxType;
		ar << m_nGeneralDeckTopEndAddBarLengthType;
		ar << m_nGeneralDeckTopCenAddBarLxLnxType;
		ar << m_nGeneralDeckTopCenAddBarLengthType;
		ar << (int)mm_GeneralDeckTopEndAddBarUserLen.size();
		for (it = mm_GeneralDeckTopEndAddBarUserLen.begin(); it != mm_GeneralDeckTopEndAddBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}
		ar << (int)mm_GeneralDeckTopCenAddBarUserLen.size();
		for (it = mm_GeneralDeckTopCenAddBarUserLen.begin(); it != mm_GeneralDeckTopCenAddBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}

		ar << m_nMatCZoneSpliceWay;
		ar << m_nMatCZoneSpliceLength;
		ar << m_nFlatCZoneSpliceWay;
		ar << m_nFlatCZoneSpliceLength;
		ar << m_nSlabCZoneSpliceWay;
		ar << m_nSlabCZoneSpliceLength;
		ar << m_nSOGCZoneSpliceWay;
		ar << m_nSOGCZoneSpliceLength;
		ar << m_nCircularRampCZoneSpliceWay;
		ar << m_nCircularRampCZoneSpliceLength;


		ar << m_nMatTopEndDevTypeUserLength;
		ar << m_nMatBotEndDevTypeUserLength;
		ar << m_nMatOpeningTopDevType;
		ar << m_nMatOpeningBotDevType;

		ar << m_nSOGTopEndDevTypeUserLength;
		ar << m_nSOGBotEndDevTypeUserLength;
		
		ar << m_nFlatTopEndDevTypeUserLength;
		ar << m_nFlatBotEndDevTypeUserLength;
		
		ar << m_nCircularRampTopEndDevTypeUserLength;
		ar << m_nCircularRampBotEndDevTypeUserLength;
		
		ar << m_nSlabTopEndDevTypeUserLength;
		ar << m_nSlabBotEndDevTypeUserLength;		

		ar << m_nEmbeddedDeckTopEndAddBarLxLnxType;
		ar << m_nEmbeddedDeckTopEndAddBarLengthType;
		ar << m_nEmbeddedDeckTopCenAddBarLxLnxType;
		ar << m_nEmbeddedDeckTopCenAddBarLengthType;

		ar << m_bEmbeddedDeckSpliceBarPlace;
		ar << m_nEmbeddedDeckSpliceBarLengthType;

		ar << m_nEmbeddedDeckTopEndDevTypeUserLength;
		ar << m_nEmbeddedDeckBotEndDevTypeUserLength;

		ar << m_bGeneralDeckSpliceBarPlace;
		ar << m_nGeneralDeckSpliceBarLengthType;
		ar << m_nGeneralDeckTopEndDevTypeUserLength;
		ar << m_nGeneralDeckBotEndDevTypeUserLength;

		ar << m_nPCSlabTopEndAddBarLxLnxType;
		ar << m_nPCSlabTopEndAddBarLengthType;
		ar << m_nPCSlabTopCenAddBarLxLnxType;
		ar << m_nPCSlabTopCenAddBarLengthType;

		ar << m_nPCSlabTopEndDevTypeUserLength;
		ar << m_nPCSlabBotEndDevTypeUserLength;

		ar << m_bPCSlabShortCenSpliceBarPlace;
		ar << m_nPCSlabShortCenSpliceBarLengthType;
		ar << m_bPCSlabShortEndSpliceBarPlace;
		ar << m_nPCSlabShortEndSpliceBarLengthType;
		ar << m_bPCSlabLongCenSpliceBarPlace;
		ar << m_nPCSlabLongCenSpliceBarLengthType;
		ar << m_bPCSlabLongEndSpliceBarPlace;
		ar << m_nPCSlabLongEndSpliceBarLengthType;

		ar << (int)mm_PCSlabTopEndAddBarUserLen.size();
		for (it = mm_PCSlabTopEndAddBarUserLen.begin(); it != mm_PCSlabTopEndAddBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}
		ar << (int)mm_PCSlabTopCenAddBarUserLen.size();
		for (it = mm_PCSlabTopCenAddBarUserLen.begin(); it != mm_PCSlabTopCenAddBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}
		ar << (int)mm_PCSlabShortCenSpliceBarUserLen.size();
		for (it = mm_PCSlabShortCenSpliceBarUserLen.begin(); it != mm_PCSlabShortCenSpliceBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}
		ar << (int)mm_PCSlabShortEndSpliceBarUserLen.size();
		for (it = mm_PCSlabShortEndSpliceBarUserLen.begin(); it != mm_PCSlabShortEndSpliceBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}
		ar << (int)mm_PCSlabLongCenSpliceBarUserLen.size();
		for (it = mm_PCSlabLongCenSpliceBarUserLen.begin(); it != mm_PCSlabLongCenSpliceBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}
		ar << (int)mm_PCSlabLongEndSpliceBarUserLen.size();
		for (it = mm_PCSlabLongEndSpliceBarUserLen.begin(); it != mm_PCSlabLongEndSpliceBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}

		ar << m_nPCHCSSlabTopHookType;
		ar << m_nPCHCSSlabBotHookType;
		ar << m_nPCHCSSlabTopEndDevTypeUserLength;
		ar << m_nPCHCSSlabBotEndDevTypeUserLength;
		ar << m_nPCHCSSlabOpeningTopDevType;
		ar << m_nPCHCSSlabOpeningBotDevType;

		ar << m_nPCRPSSlabTopHookType;
		ar << m_nPCRPSSlabBotHookType;
		ar << m_nPCRPSSlabTopEndDevTypeUserLength;
		ar << m_nPCRPSSlabBotEndDevTypeUserLength;
		ar << m_nPCRPSSlabOpeningTopDevType;
		ar << m_nPCRPSSlabOpeningBotDevType;

		ar << m_nPCIMTSlabTopHookType;
		ar << m_nPCIMTSlabBotHookType;
		ar << m_nPCIMTSlabTopEndDevTypeUserLength;
		ar << m_nPCIMTSlabBotEndDevTypeUserLength;
		ar << m_nPCIMTSlabOpeningTopDevType;
		ar << m_nPCIMTSlabOpeningBotDevType;
		ar << m_nPCIMTSlabTopEndAddBarLxLnxType;
		ar << m_nPCIMTSlabTopEndAddBarLengthType;
		ar << m_nPCIMTSlabTopCenAddBarLxLnxType;
		ar << m_nPCIMTSlabTopCenAddBarLengthType;
		ar << m_bPCIMTSlabShortSpliceBarPlace;
		ar << m_nPCIMTSlabShortSpliceBarLengthType;
		ar << m_bPCIMTSlabLongSpliceBarPlace;

		ar << (int)mm_PCIMTSlabTopEndAddBarUserLen.size();
		for (it = mm_PCIMTSlabTopEndAddBarUserLen.begin(); it != mm_PCIMTSlabTopEndAddBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}
		ar << (int)mm_PCIMTSlabTopCenAddBarUserLen.size();
		for (it = mm_PCIMTSlabTopCenAddBarUserLen.begin(); it != mm_PCIMTSlabTopCenAddBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}
		ar << (int)mm_PCIMTSlabShortSpliceBarUserLen.size();
		for (it = mm_PCIMTSlabShortSpliceBarUserLen.begin(); it != mm_PCIMTSlabShortSpliceBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}

		ar << (int)mm_EmbeddedDeckTopEndAddBarUserLen.size();
		for (it = mm_EmbeddedDeckTopEndAddBarUserLen.begin(); it != mm_EmbeddedDeckTopEndAddBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}
		ar << (int)mm_EmbeddedDeckTopCenAddBarUserLen.size();
		for (it = mm_EmbeddedDeckTopCenAddBarUserLen.begin(); it != mm_EmbeddedDeckTopCenAddBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}
		ar << (int)mm_GeneralDeckSpliceBarUserLen.size();
		for (it = mm_GeneralDeckSpliceBarUserLen.begin(); it != mm_GeneralDeckSpliceBarUserLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long len = it->second;
			ar << nDiaIndex;
			ar << len;
		}
		m_pMatShearFrictionBarTemplate->Serialize(ar);
		m_pFlatShearFrictionBarTemplate->Serialize(ar);

		ar << m_bMatBotSpacerUserSteelGradeS;
		ar << m_bMatTopSpacerUserSteelGradeS;
		ar << m_bMatSpacerSupportBarUserSteelGradeS;
		ar << m_bMatSupportRebarUserSteelGradeS;
		
		ar << m_bFlatBotSpacerUserSteelGradeS;
		ar << m_bFlatTopSpacerUserSteelGradeS;
		ar << m_bFlatSpacerSupportBarUserSteelGradeS;
		
		ar << m_bSlabBotSpacerUserSteelGradeS;
		ar << m_bSlabTopSpacerUserSteelGradeS;
		ar << m_bSlabSpacerSupportBarUserSteelGradeS;
		
		ar << m_bSOGBotSpacerUserSteelGradeS;
		ar << m_bSOGTopSpacerUserSteelGradeS;
		ar << m_bSOGSpacerSupportBarUserSteelGradeS;
		ar << m_bSOGSupportRebarUserSteelGradeS;
		
		ar << m_bCircularRampBotSpacerUserSteelGradeS;
		ar << m_bCircularRampTopSpacerUserSteelGradeS;
		ar << m_bCircularRampSpacerSupportBarUserSteelGradeS;

		ar << m_nEmbeddedDeckCZoneSpliceWay;
		ar << m_nEmbeddedDeckCZoneSpliceLength;
		ar << m_nGeneralDeckCZoneSpliceWay;
		ar << m_nGeneralDeckCZoneSpliceLength;
		ar << m_nPCSlabCZoneSpliceWay;
		ar << m_nPCSlabCZoneSpliceLength;
		ar << m_nPCIMTSlabCZoneSpliceWay;
		ar << m_nPCIMTSlabCZoneSpliceLength;
		ar << m_nPCHCSSlabCZoneSpliceWay;
		ar << m_nPCHCSSlabCZoneSpliceLength;
		ar << m_nPCRPSSlabCZoneSpliceWay;
		ar << m_nPCRPSSlabCZoneSpliceLength;

		ar << m_bMatTopSpacerPlaceConditionSet;
		ar << m_nMatTopSpacerPlaceCondition;
		ar << m_bFlatTopSpacerPlaceConditionSet;
		ar << m_nFlatTopSpacerPlaceCondition;
		ar << m_bSlabTopSpacerPlaceConditionSet;
		ar << m_nSlabTopSpacerPlaceCondition;
		ar << m_bSOGTopSpacerPlaceConditionSet;
		ar << m_nSOGTopSpacerPlaceCondition;
		ar << m_bCircularRampTopSpacerPlaceConditionSet;
		ar << m_nCircularRampTopSpacerPlaceCondition;

		ar << m_bSlabOpeningAddBarDeductionCondition;
		ar << m_nSlabOpeningAddBarDeductionPercent;

		ar << (int)mm_MatThickToDiaIndex.size();
		for (auto it = mm_MatThickToDiaIndex.begin(); it != mm_MatThickToDiaIndex.end(); it++)
		{
			CString sThick = it->first;
			long nDiaIndex = it->second;
			ar << sThick;
			ar << nDiaIndex;
		}
		ar << (int)mm_FlatThickToDiaIndex.size();
		for (auto it = mm_FlatThickToDiaIndex.begin(); it != mm_FlatThickToDiaIndex.end(); it++)
		{
			CString sThick = it->first;
			long nDiaIndex = it->second;
			ar << sThick;
			ar << nDiaIndex;
		}
		ar << (int)mm_SlabThickToDiaIndex.size();
		for (auto it = mm_SlabThickToDiaIndex.begin(); it != mm_SlabThickToDiaIndex.end(); it++)
		{
			CString sThick = it->first;
			long nDiaIndex = it->second;
			ar << sThick;
			ar << nDiaIndex;
		}
		ar << (int)mm_SOGThickToDiaIndex.size();
		for (auto it = mm_SOGThickToDiaIndex.begin(); it != mm_SOGThickToDiaIndex.end(); it++)
		{
			CString sThick = it->first;
			long nDiaIndex = it->second;
			ar << sThick;
			ar << nDiaIndex;
		}
		ar << (int)mm_CircularRampThickToDiaIndex.size();
		for (auto it = mm_CircularRampThickToDiaIndex.begin(); it != mm_CircularRampThickToDiaIndex.end(); it++)
		{
			CString sThick = it->first;
			long nDiaIndex = it->second;
			ar << sThick;
			ar << nDiaIndex;
		}
	
		ar << m_bSlabLongEndPlaceAreaSet;
		ar << m_nSlabLongEndPlaceAreaOneWaySlabType;
		ar << m_nSlabLongEndPlaceAreaTwoWaySlabType;
		ar << m_bSlabBotAddBarLengthSet;
		ar << m_nSlabBotAddBarLengthType;
		ar << m_bSlabBotAddBarCutBarLenExclusionSet;

		ar << m_nSlabOpeningAddBarDevLengthType;
	}
	else
	{
		// MAT
		ar >> bDefaultValue;
		ar >> nDefaultValue;

		ar >> bDefaultValue;
		ar >> m_nMatIsoFootDevType;

		ar >> m_bMatLevelDifInfoSet;
		ar >> nDefaultValue;
		ar >> nDefaultValue;

		ar >> m_bMatEndInfoSet;
		ar >> m_nMatTopEndHookType;
		ar >> m_nMatBotEndHookType;

		ar >> bDefaultValue;
		if (MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> bDefaultValue;
			ar >> nDefaultValue;
		}
		ar >> nDefaultValue;
		ar >> bDefaultValue;
		ar >> bDefaultValue;
		if (MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> bDefaultValue;
			ar >> nDefaultValue;
		}
		ar >> nDefaultValue;
		ar >> bDefaultValue;
		ar >> bDefaultValue;

		ar >> m_bMatRebarAreaInfoSet;
		ar >> m_nMatRebarTolDist;
		ar >> m_nMatRebarTolLenDif;

		// FLAT
		ar >> bDefaultValue;
		ar >> nDefaultValue;

		ar >> bDefaultValue;
		ar >> m_nFlatIsoFootDevType;

		ar >> m_bFlatOpeningInfoSet;
		ar >> m_nFlatOpeningTopDevType;
		ar >> m_nFlatOpeningBotDevType;

		ar >> m_bFlatLevelDifInfoSet;
		ar >> nDefaultValue;
		ar >> nDefaultValue;

		ar >> m_bFlatEndInfoSet;
		ar >> m_nFlatTopEndHookType;
		ar >> m_nFlatBotEndHookType;

		ar >> bDefaultValue;
		if (MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> bDefaultValue;
			ar >> nDefaultValue;
		}
		ar >> nDefaultValue;
		ar >> bDefaultValue;
		ar >> bDefaultValue;
		if (MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> bDefaultValue;
			ar >> nDefaultValue;
		}
		ar >> nDefaultValue;
		ar >> bDefaultValue;
		ar >> bDefaultValue;
		if (MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> bDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
		}		

		ar >> m_bFlatRebarAreaInfoSet;
		ar >> m_nFlatRebarTolDist;
		ar >> m_nFlatRebarTolLenDif;

		// SLAB
		ar >> bDefaultValue;
		ar >> nDefaultValue;

		long nOldSlabTopEndHookType = 0, nOldSlabBotEndHookType = 0;
		long nOldPCSlabTopEndHookType = 0, nOldPCSlabBotEndHookType = 0;
		long nOldEmbeddedDeckTopEndHookType = 0, nOldEmbeddedDeckBotEndHookType = 0;
		long nOldGeneralDeckTopEndHookType = 0, nOldGeneralDeckBotEndHookType = 0;

		if (MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> bDefaultValue;
			ar >> nOldSlabTopEndHookType;
			ar >> nOldSlabBotEndHookType;
		}

		ar >> m_bSlabTopCutBarLenInfoSet;
		ar >> m_nSlabOneWayCutBarLenType;
		ar >> m_nSlabTwoWayCutBarLenType;

		// Mat
		ar >> m_nMatLevelDifPlaceShape;
		ar >> m_nMatLevelDifHaunchPlaceType;
		ar >> m_bMatMainBarInfoSet;
		ar >> m_nMatMaxDualHookLength;
		ar >> m_nMatSpliceWay;
		ar >> m_nMatSpliceType;
		if (MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> bDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
		}

		// Flat
		ar >> m_nFlatLevelDifPlaceShape;
		ar >> m_nFlatLevelDifHaunchPlaceType;
		ar >> m_bFlatMainBarInfoSet;
		ar >> m_nFlatMaxDualHookLength;
		ar >> m_nFlatSpliceWay;
		ar >> m_nFlatSpliceType;
		if (MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> bDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
		}

		// Slab
		ar >> m_bSlabMainBarInfoSet;
		ar >> m_nSlabMaxDualHookLength;
		ar >> m_nSlabSpliceWay;
		ar >> m_nSlabSpliceType;
		if (MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> bDefaultValue;
			ar >> nDefaultValue;
		}
		ar >> nDefaultValue;
		if (MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> bDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
		
			ar >> bDefaultValue;
			ar >> nDefaultValue;
		}
		ar >> nDefaultValue;

		ar >> bDefaultValue;
		ar >> m_nSOGSpliceDevSlabType;
		ar >> m_nFlatSpliceDevSlabType;
		ar >> m_nCircularRampSpliceDevSlabType;

		ar >> m_bSlabUnStructuredEndInfoSet;
		ar >> m_nSlabTopUnStructuredEndHookType;
		ar >> m_nSlabBotUnStructuredEndHookType;

		if(MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> nDefaultValue;
		}

		ar >> m_bSlabLevelDifInfoSet;
		ar >> m_nSlabLevelDifPlaceShape;
		ar >> m_nSlabLevelDifHaunchPlaceType;

		ar >> m_bSlabOpeningInfoSet;
		ar >> m_nSlabOpeningTopDevType;
		ar >> m_nSlabOpeningBotDevType;

		ar >> m_bSlabRebarAreaInfoSet;
		ar >> m_nSlabRebarTolDist;
		ar >> m_nSlabRebarTolLenDif;

		// SOG Slab
		ar >> m_bSOGMainBarInfoSet;
		ar >> m_nSOGMaxDualHookLength;
		ar >> m_nSOGSpliceWay;
		ar >> m_nSOGSpliceType;
		if (MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> bDefaultValue;
			ar >> nDefaultValue;
		}

		if (MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> bDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
		
			ar >> bDefaultValue;
			ar >> nDefaultValue;
		}

		ar >> m_bSOGEndInfoSet;
		ar >> m_nSOGTopEndHookType;
		ar >> m_nSOGBotEndHookType;
		ar >> m_nSOGIsoFootDevType;

		ar >> m_bSOGLevelDifInfoSet;
		ar >> m_nSOGLevelDifPlaceShape;
		ar >> m_nSOGLevelDifHaunchPlaceType;

		ar >> m_bSOGOpeningInfoSet;
		ar >> m_nSOGOpeningTopDevType;
		ar >> m_nSOGOpeningBotDevType;

		ar >> m_bSOGRebarAreaInfoSet;
		ar >> m_nSOGRebarTolDist;
		ar >> m_nSOGRebarTolLenDif;

		// CircularRamp
		ar >> m_bCircularRampMainBarInfoSet;
		ar >> m_nCircularRampMaxDualHookLength;
		ar >> m_nCircularRampSpliceWay;
		ar >> m_nCircularRampSpliceType;
		if (MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> bDefaultValue;
			ar >> nDefaultValue;
		
			ar >> bDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
		
			ar >> bDefaultValue;
			ar >> nDefaultValue;
		}

		ar >> m_bCircularRampEndInfoSet;
		ar >> m_nCircularRampTopEndHookType;
		ar >> m_nCircularRampBotEndHookType;
		ar >> m_nCircularRampIsoFootDevType;

		ar >> m_bCircularRampLevelDifInfoSet;
		ar >> m_nCircularRampLevelDifPlaceShape;
		ar >> m_nCircularRampLevelDifHaunchPlaceType;

		ar >> m_bCircularRampOpeningInfoSet;
		ar >> m_nCircularRampOpeningTopDevType;
		ar >> m_nCircularRampOpeningBotDevType;

		ar >> m_bCircularRampRebarAreaInfoSet;
		ar >> m_nCircularRampRebarTolDist;
		ar >> m_nCircularRampRebarTolLenDif;

		// PC Slab
		ar >> m_bPCSlabMainBarInfoSet;
		ar >> m_nPCSlabMaxDualHookLength;
		ar >> m_nPCSlabSpliceWay;
		ar >> m_nPCSlabSpliceType;
		if (MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> bDefaultValue;
			ar >> nDefaultValue;
		
			ar >> bDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
		
			ar >> bDefaultValue;
			ar >> nDefaultValue;
		
			ar >> bDefaultValue;
			ar >> nOldPCSlabTopEndHookType;
			ar >> nOldPCSlabBotEndHookType;

			ar >> bDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
		}
		ar >> m_bPCSlabUnStructuredEndInfoSet;
		ar >> m_nPCSlabTopUnStructuredEndHookType;
		ar >> m_nPCSlabBotUnStructuredEndHookType;
		if (MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> nDefaultValue;
		}

		ar >> m_bPCSlabLevelDifInfoSet;
		ar >> m_nPCSlabLevelDifPlaceShape;
		ar >> m_nPCSlabLevelDifHaunchPlaceType;

		ar >> m_bPCSlabOpeningInfoSet;
		ar >> m_nPCSlabOpeningTopDevType;
		ar >> m_nPCSlabOpeningBotDevType;

		ar >> m_bPCSlabRebarAreaInfoSet;
		ar >> m_nPCSlabRebarTolDist;
		ar >> m_nPCSlabRebarTolLenDif;

		// 슬래브 보강근
		ar >> m_nSlabRebarRebarTolDist;
		ar >> m_nSlabRebarRebarTolLenDif;

		// 철근 일체형 데크 슬래브
		ar >> m_bEmbeddedDeckMainBarInfoSet;
		ar >> m_nEmbeddedDeckMaxDualHookLength;
		ar >> m_nEmbeddedDeckSpliceWay;
		ar >> m_nEmbeddedDeckSpliceType;
		if (MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> bDefaultValue;
			ar >> nDefaultValue;

			ar >> bDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;

			ar >> bDefaultValue;
			ar >> nDefaultValue;


			ar >> bDefaultValue;
			ar >> nOldEmbeddedDeckTopEndHookType;
			ar >> nOldEmbeddedDeckBotEndHookType;
		}
		ar >> m_bEmbeddedDeckUnStructuredEndInfoSet;
		ar >> m_nEmbeddedDeckTopUnStructuredEndHookType;
		ar >> m_nEmbeddedDeckBotUnStructuredEndHookType;
		if (MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> nDefaultValue;
		}

		ar >> m_bEmbeddedDeckLevelDifInfoSet;
		ar >> m_nEmbeddedDeckLevelDifPlaceShape;
		ar >> m_nEmbeddedDeckLevelDifHaunchPlaceType;

		ar >> m_bEmbeddedDeckOpeningInfoSet;
		ar >> m_nEmbeddedDeckOpeningTopDevType;
		ar >> m_nEmbeddedDeckOpeningBotDevType;

		ar >> m_bEmbeddedDeckRebarAreaInfoSet;
		ar >> m_nEmbeddedDeckRebarTolDist;
		ar >> m_nEmbeddedDeckRebarTolLenDif;

		// 일반 데크 슬래브
		ar >> m_bGeneralDeckMainBarInfoSet;
		ar >> m_nGeneralDeckMaxDualHookLength;
		ar >> m_nGeneralDeckSpliceWay;
		ar >> m_nGeneralDeckSpliceType;
		if (MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> bDefaultValue;
			ar >> nDefaultValue;

			ar >> bDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;
			ar >> nDefaultValue;

			ar >> bDefaultValue;
			ar >> nDefaultValue;

			ar >> bDefaultValue;
			ar >> nOldGeneralDeckTopEndHookType;
			ar >> nOldGeneralDeckBotEndHookType;
		}
		ar >> m_bGeneralDeckUnStructuredEndInfoSet;
		ar >> m_nGeneralDeckTopUnStructuredEndHookType;
		ar >> m_nGeneralDeckBotUnStructuredEndHookType;
		if (MSVersionInfo::GetCurrentVersion() < 20230510)
		{
			ar >> nDefaultValue;
		}

		ar >> m_bGeneralDeckLevelDifInfoSet;
		ar >> m_nGeneralDeckLevelDifPlaceShape;
		ar >> m_nGeneralDeckLevelDifHaunchPlaceType;

		ar >> m_bGeneralDeckOpeningInfoSet;
		ar >> m_nGeneralDeckOpeningTopDevType;
		ar >> m_nGeneralDeckOpeningBotDevType;

		ar >> m_bGeneralDeckRebarAreaInfoSet;
		ar >> m_nGeneralDeckRebarTolDist;
		ar >> m_nGeneralDeckRebarTolLenDif;

		ar >> m_bMatTopSpacerInfoSet;
		ar >> m_bMatTopSpacerSet;
		ar >> m_nMatTopSpacerDiaType;
		ar >> nMatTopSpacerDiaIndex;
		ar >> m_nMatTopSpacerSpacingType;
		ar >> m_nMatTopSpacerSlabSpacing;
		ar >> m_nMatTopSpacerUserSpacing1;
		ar >> m_nMatTopSpacerUserSpacing2;
		ar >> m_nMatTopSpacerHeightType;
		ar >> m_nMatTopSpacerUserHeight;
		ar >> m_nMatTopSpacerHookLenAType;
		ar >> m_nMatTopSpacerMainBarHookLenA;
		ar >> m_nMatTopSpacerUserHookLenA;
		ar >> m_nMatTopSpacerHookLenBType;
		ar >> m_nMatTopSpacerMainBarHookLenB;
		ar >> m_nMatTopSpacerUserHookLenB;
		ar >> m_nMatTopSpacerWidth;

		ar >> m_bMatBotSpacerInfoSet;
		ar >> m_bMatBotSpacerSet;
		ar >> m_nMatBotSpacerDiaType;
		ar >> m_nMatBotSpacerDiaIndex;
		ar >> m_nMatBotSpacerSpacingType;
		ar >> m_nMatBotSpacerSlabSpacing;
		ar >> m_nMatBotSpacerUserSpacing1;
		ar >> m_nMatBotSpacerUserSpacing2;
		ar >> m_nMatBotSpacerHeightType;
		ar >> m_nMatBotSpacerUserHeight;
		ar >> m_nMatBotSpacerHookLenAType;
		ar >> m_nMatBotSpacerMainBarHookLenA;
		ar >> m_nMatBotSpacerUserHookLenA;
		ar >> m_nMatBotSpacerHookLenBType;
		ar >> m_nMatBotSpacerMainBarHookLenB;
		ar >> m_nMatBotSpacerUserHookLenB;

		ar >> m_bFlatTopSpacerInfoSet;
		ar >> m_bFlatTopSpacerSet;
		ar >> m_nFlatTopSpacerDiaType;
		ar >> nFlatTopSpacerDiaIndex;
		ar >> m_nFlatTopSpacerSpacingType;
		ar >> m_nFlatTopSpacerSlabSpacing;
		ar >> m_nFlatTopSpacerUserSpacing1;
		ar >> m_nFlatTopSpacerUserSpacing2;
		ar >> m_nFlatTopSpacerHeightType;
		ar >> m_nFlatTopSpacerUserHeight;
		ar >> m_nFlatTopSpacerHookLenAType;
		ar >> m_nFlatTopSpacerMainBarHookLenA;
		ar >> m_nFlatTopSpacerUserHookLenA;
		ar >> m_nFlatTopSpacerHookLenBType;
		ar >> m_nFlatTopSpacerMainBarHookLenB;
		ar >> m_nFlatTopSpacerUserHookLenB;
		ar >> m_nFlatTopSpacerWidth;

		ar >> m_bFlatBotSpacerInfoSet;
		ar >> m_bFlatBotSpacerSet;
		ar >> m_nFlatBotSpacerDiaType;
		ar >> m_nFlatBotSpacerDiaIndex;
		ar >> m_nFlatBotSpacerSpacingType;
		ar >> m_nFlatBotSpacerSlabSpacing;
		ar >> m_nFlatBotSpacerUserSpacing1;
		ar >> m_nFlatBotSpacerUserSpacing2;
		ar >> m_nFlatBotSpacerHeightType;
		ar >> m_nFlatBotSpacerUserHeight;
		ar >> m_nFlatBotSpacerHookLenAType;
		ar >> m_nFlatBotSpacerMainBarHookLenA;
		ar >> m_nFlatBotSpacerUserHookLenA;
		ar >> m_nFlatBotSpacerHookLenBType;
		ar >> m_nFlatBotSpacerMainBarHookLenB;
		ar >> m_nFlatBotSpacerUserHookLenB;

		ar >> m_bSlabTopSpacerInfoSet;
		ar >> m_bSlabTopSpacerSet;
		ar >> m_nSlabTopSpacerDiaType;
		ar >> nSlabTopSpacerDiaIndex;
		ar >> m_nSlabTopSpacerSpacingType;
		ar >> m_nSlabTopSpacerSlabSpacing;
		ar >> m_nSlabTopSpacerUserSpacing1;
		ar >> m_nSlabTopSpacerUserSpacing2;
		ar >> m_nSlabTopSpacerHeightType;
		ar >> m_nSlabTopSpacerUserHeight;
		ar >> m_nSlabTopSpacerHookLenAType;
		ar >> m_nSlabTopSpacerMainBarHookLenA;
		ar >> m_nSlabTopSpacerUserHookLenA;
		ar >> m_nSlabTopSpacerHookLenBType;
		ar >> m_nSlabTopSpacerMainBarHookLenB;
		ar >> m_nSlabTopSpacerUserHookLenB;
		ar >> m_nSlabTopSpacerWidth;

		ar >> m_bSlabBotSpacerInfoSet;
		ar >> m_bSlabBotSpacerSet;
		ar >> m_nSlabBotSpacerDiaType;
		ar >> m_nSlabBotSpacerDiaIndex;
		ar >> m_nSlabBotSpacerSpacingType;
		ar >> m_nSlabBotSpacerSlabSpacing;
		ar >> m_nSlabBotSpacerUserSpacing1;
		ar >> m_nSlabBotSpacerUserSpacing2;
		ar >> m_nSlabBotSpacerHeightType;
		ar >> m_nSlabBotSpacerUserHeight;
		ar >> m_nSlabBotSpacerHookLenAType;
		ar >> m_nSlabBotSpacerMainBarHookLenA;
		ar >> m_nSlabBotSpacerUserHookLenA;
		ar >> m_nSlabBotSpacerHookLenBType;
		ar >> m_nSlabBotSpacerMainBarHookLenB;
		ar >> m_nSlabBotSpacerUserHookLenB;

		ar >> m_bSOGTopSpacerInfoSet;
		ar >> m_bSOGTopSpacerSet;
		ar >> m_nSOGTopSpacerDiaType;
		ar >> nSOGTopSpacerDiaIndex;
		ar >> m_nSOGTopSpacerSpacingType;
		ar >> m_nSOGTopSpacerSlabSpacing;
		ar >> m_nSOGTopSpacerUserSpacing1;
		ar >> m_nSOGTopSpacerUserSpacing2;
		ar >> m_nSOGTopSpacerHeightType;
		ar >> m_nSOGTopSpacerUserHeight;
		ar >> m_nSOGTopSpacerHookLenAType;
		ar >> m_nSOGTopSpacerMainBarHookLenA;
		ar >> m_nSOGTopSpacerUserHookLenA;
		ar >> m_nSOGTopSpacerHookLenBType;
		ar >> m_nSOGTopSpacerMainBarHookLenB;
		ar >> m_nSOGTopSpacerUserHookLenB;
		ar >> m_nSOGTopSpacerWidth;

		ar >> m_bSOGBotSpacerInfoSet;
		ar >> m_bSOGBotSpacerSet;
		ar >> m_nSOGBotSpacerDiaType;
		ar >> m_nSOGBotSpacerDiaIndex;
		ar >> m_nSOGBotSpacerSpacingType;
		ar >> m_nSOGBotSpacerSlabSpacing;
		ar >> m_nSOGBotSpacerUserSpacing1;
		ar >> m_nSOGBotSpacerUserSpacing2;
		ar >> m_nSOGBotSpacerHeightType;
		ar >> m_nSOGBotSpacerUserHeight;
		ar >> m_nSOGBotSpacerHookLenAType;
		ar >> m_nSOGBotSpacerMainBarHookLenA;
		ar >> m_nSOGBotSpacerUserHookLenA;
		ar >> m_nSOGBotSpacerHookLenBType;
		ar >> m_nSOGBotSpacerMainBarHookLenB;
		ar >> m_nSOGBotSpacerUserHookLenB;

		ar >> m_bCircularRampTopSpacerInfoSet;
		ar >> m_bCircularRampTopSpacerSet;
		ar >> m_nCircularRampTopSpacerDiaType;
		ar >> nCircularRampTopSpacerDiaIndex;
		ar >> m_nCircularRampTopSpacerSpacingType;
		ar >> m_nCircularRampTopSpacerSlabSpacing;
		ar >> m_nCircularRampTopSpacerUserSpacing1;
		ar >> m_nCircularRampTopSpacerUserSpacing2;
		ar >> m_nCircularRampTopSpacerHeightType;
		ar >> m_nCircularRampTopSpacerUserHeight;
		ar >> m_nCircularRampTopSpacerHookLenAType;
		ar >> m_nCircularRampTopSpacerMainBarHookLenA;
		ar >> m_nCircularRampTopSpacerUserHookLenA;
		ar >> m_nCircularRampTopSpacerHookLenBType;
		ar >> m_nCircularRampTopSpacerMainBarHookLenB;
		ar >> m_nCircularRampTopSpacerUserHookLenB;
		ar >> m_nCircularRampTopSpacerWidth;

		ar >> m_bCircularRampBotSpacerInfoSet;
		ar >> m_bCircularRampBotSpacerSet;
		ar >> m_nCircularRampBotSpacerDiaType;
		ar >> m_nCircularRampBotSpacerDiaIndex;
		ar >> m_nCircularRampBotSpacerSpacingType;
		ar >> m_nCircularRampBotSpacerSlabSpacing;
		ar >> m_nCircularRampBotSpacerUserSpacing1;
		ar >> m_nCircularRampBotSpacerUserSpacing2;
		ar >> m_nCircularRampBotSpacerHeightType;
		ar >> m_nCircularRampBotSpacerUserHeight;
		ar >> m_nCircularRampBotSpacerHookLenAType;
		ar >> m_nCircularRampBotSpacerMainBarHookLenA;
		ar >> m_nCircularRampBotSpacerUserHookLenA;
		ar >> m_nCircularRampBotSpacerHookLenBType;
		ar >> m_nCircularRampBotSpacerMainBarHookLenB;
		ar >> m_nCircularRampBotSpacerUserHookLenB;

		ar >> m_bPCSlabTopSpacerInfoSet;
		ar >> m_bPCSlabTopSpacerSet;
		ar >> m_nPCSlabTopSpacerDiaType;
		ar >> m_nPCSlabTopSpacerDiaIndex;
		ar >> m_nPCSlabTopSpacerSpacingType;
		ar >> m_nPCSlabTopSpacerSlabSpacing;
		ar >> m_nPCSlabTopSpacerUserSpacing1;
		ar >> m_nPCSlabTopSpacerUserSpacing2;
		ar >> m_nPCSlabTopSpacerHeightType;
		ar >> m_nPCSlabTopSpacerUserHeight;
		ar >> m_nPCSlabTopSpacerHookLenAType;
		ar >> m_nPCSlabTopSpacerMainBarHookLenA;
		ar >> m_nPCSlabTopSpacerUserHookLenA;
		ar >> m_nPCSlabTopSpacerHookLenBType;
		ar >> m_nPCSlabTopSpacerMainBarHookLenB;
		ar >> m_nPCSlabTopSpacerUserHookLenB;
		ar >> m_nPCSlabTopSpacerWidth;

		ar >> m_bPCSlabBotSpacerInfoSet;
		ar >> m_bPCSlabBotSpacerSet;
		ar >> m_nPCSlabBotSpacerDiaType;
		ar >> m_nPCSlabBotSpacerDiaIndex;
		ar >> m_nPCSlabBotSpacerSpacingType;
		ar >> m_nPCSlabBotSpacerSlabSpacing;
		ar >> m_nPCSlabBotSpacerUserSpacing1;
		ar >> m_nPCSlabBotSpacerUserSpacing2;
		ar >> m_nPCSlabBotSpacerHeightType;
		ar >> m_nPCSlabBotSpacerUserHeight;
		ar >> m_nPCSlabBotSpacerHookLenAType;
		ar >> m_nPCSlabBotSpacerMainBarHookLenA;
		ar >> m_nPCSlabBotSpacerUserHookLenA;
		ar >> m_nPCSlabBotSpacerHookLenBType;
		ar >> m_nPCSlabBotSpacerMainBarHookLenB;
		ar >> m_nPCSlabBotSpacerUserHookLenB;

		if (MSVersionInfo::GetCurrentVersion() >= 20170621)
		{
			ar >> m_nMatBotSpacerWidth;
			ar >> m_nFlatBotSpacerWidth;
			ar >> m_nSlabBotSpacerWidth;
			ar >> m_nSOGBotSpacerWidth;
			ar >> m_nCircularRampBotSpacerWidth;
			ar >> m_nPCSlabBotSpacerWidth;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20170822)
		{
			ar >> m_bMatSpacerSupportBarsSet;
			ar >> m_bMatSpacerSupportBarsConditionSet;
			ar >> m_nMatSpacerSupportBarsCondition;
			ar >> m_nMatSpacerSupportBarsDiaType;
			ar >> m_nMatSpacerSupportBarsUserDiaIndex;
			ar >> m_nMatSpacerSupportBarsNumType;
			ar >> m_dMatSpacerSupportBarsTopSpacerNums;
			ar >> m_nMatSpacerSupportBarsUserNums;
			ar >> m_nMatSpacerSupportBarsLengthType;
			ar >> m_nMatSpacerSupportBarsUserLength;

			ar >> m_bFlatSpacerSupportBarsSet;
			ar >> m_bFlatSpacerSupportBarsConditionSet;
			ar >> m_nFlatSpacerSupportBarsCondition;
			ar >> m_nFlatSpacerSupportBarsDiaType;
			ar >> m_nFlatSpacerSupportBarsUserDiaIndex;
			ar >> m_nFlatSpacerSupportBarsNumType;
			ar >> m_dFlatSpacerSupportBarsTopSpacerNums;
			ar >> m_nFlatSpacerSupportBarsUserNums;
			ar >> m_nFlatSpacerSupportBarsLengthType;
			ar >> m_nFlatSpacerSupportBarsUserLength;

			ar >> m_bSlabSpacerSupportBarsSet;
			ar >> m_bSlabSpacerSupportBarsConditionSet;
			ar >> m_nSlabSpacerSupportBarsCondition;
			ar >> m_nSlabSpacerSupportBarsDiaType;
			ar >> m_nSlabSpacerSupportBarsUserDiaIndex;
			ar >> m_nSlabSpacerSupportBarsNumType;
			ar >> m_dSlabSpacerSupportBarsTopSpacerNums;
			ar >> m_nSlabSpacerSupportBarsUserNums;
			ar >> m_nSlabSpacerSupportBarsLengthType;
			ar >> m_nSlabSpacerSupportBarsUserLength;

			ar >> m_bSOGSpacerSupportBarsSet;
			ar >> m_bSOGSpacerSupportBarsConditionSet;
			ar >> m_nSOGSpacerSupportBarsCondition;
			ar >> m_nSOGSpacerSupportBarsDiaType;
			ar >> m_nSOGSpacerSupportBarsUserDiaIndex;
			ar >> m_nSOGSpacerSupportBarsNumType;
			ar >> m_dSOGSpacerSupportBarsTopSpacerNums;
			ar >> m_nSOGSpacerSupportBarsUserNums;
			ar >> m_nSOGSpacerSupportBarsLengthType;
			ar >> m_nSOGSpacerSupportBarsUserLength;

			ar >> m_bCircularRampSpacerSupportBarsSet;
			ar >> m_bCircularRampSpacerSupportBarsConditionSet;
			ar >> m_nCircularRampSpacerSupportBarsCondition;
			ar >> m_nCircularRampSpacerSupportBarsDiaType;
			ar >> m_nCircularRampSpacerSupportBarsUserDiaIndex;
			ar >> m_nCircularRampSpacerSupportBarsNumType;
			ar >> m_dCircularRampSpacerSupportBarsTopSpacerNums;
			ar >> m_nCircularRampSpacerSupportBarsUserNums;
			ar >> m_nCircularRampSpacerSupportBarsLengthType;
			ar >> m_nCircularRampSpacerSupportBarsUserLength;

			ar >> m_bMatSupportRebarsSet;
			ar >> m_nMatSupportRebarsDiaIndex;
			ar >> m_nMatSupportRebarsLengthType;
			ar >> m_dMatSupportRebarsRWallLengthRate;
			ar >> m_dMatSupportRebarsSlabLengthRate;
			ar >> m_nMatSupportRebarsOrderLength;
			ar >> m_nMatSupportRebarsSpliceLength;
			ar >> m_nMatSupportRebarsCorrectiveLength;

			if (MSVersionInfo::GetCurrentVersion() < 20230510)
			{
				ar >> bDefaultValue;
				ar >> nDefaultValue;
				ar >> nDefaultValue;
				ar >> dDefaultValue;
				ar >> dDefaultValue;
				ar >> nDefaultValue;
				ar >> nDefaultValue;
				ar >> nDefaultValue;

				ar >> bDefaultValue;
				ar >> nDefaultValue;
				ar >> nDefaultValue;
				ar >> dDefaultValue;
				ar >> dDefaultValue;
				ar >> nDefaultValue;
				ar >> nDefaultValue;
				ar >> nDefaultValue;
			}
			
			ar >> m_bSOGSupportRebarsSet;
			ar >> m_nSOGSupportRebarsDiaIndex;
			ar >> m_nSOGSupportRebarsLengthType;
			ar >> m_dSOGSupportRebarsRWallLengthRate;
			ar >> m_dSOGSupportRebarsSlabLengthRate;
			ar >> m_nSOGSupportRebarsOrderLength;
			ar >> m_nSOGSupportRebarsSpliceLength;
			ar >> m_nSOGSupportRebarsCorrectiveLength;

			if (MSVersionInfo::GetCurrentVersion() < 20230510)
			{
				ar >> bDefaultValue;
				ar >> nDefaultValue;
				ar >> nDefaultValue;
				ar >> dDefaultValue;
				ar >> dDefaultValue;
				ar >> nDefaultValue;
				ar >> nDefaultValue;
				ar >> nDefaultValue;
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20180717)
		{
			ar >> m_nMatTopSpacerSlabThick;
			ar >> m_nMatTopSpacerSlabThickOverDiaIndex;
			ar >> m_nMatTopSpacerSlabThickUnderDiaIndex;

			ar >> m_nFlatTopSpacerSlabThick;
			ar >> m_nFlatTopSpacerSlabThickOverDiaIndex;
			ar >> m_nFlatTopSpacerSlabThickUnderDiaIndex;

			ar >> m_nSlabTopSpacerSlabThick;
			ar >> m_nSlabTopSpacerSlabThickOverDiaIndex;
			ar >> m_nSlabTopSpacerSlabThickUnderDiaIndex;

			ar >> m_nSOGTopSpacerSlabThick;
			ar >> m_nSOGTopSpacerSlabThickOverDiaIndex;
			ar >> m_nSOGTopSpacerSlabThickUnderDiaIndex;

			ar >> m_nCircularRampTopSpacerSlabThick;
			ar >> m_nCircularRampTopSpacerSlabThickOverDiaIndex;
			ar >> m_nCircularRampTopSpacerSlabThickUnderDiaIndex;
		}
		else
		{
			if (m_nMatTopSpacerDiaType == 1)
				m_nMatTopSpacerSlabThickOverDiaIndex = nMatTopSpacerDiaIndex;

			if (m_nFlatTopSpacerDiaType == 1)
				m_nFlatTopSpacerSlabThickUnderDiaIndex = nFlatTopSpacerDiaIndex;

			if (m_nSlabTopSpacerDiaType == 1)
				m_nSlabTopSpacerSlabThickUnderDiaIndex = nSlabTopSpacerDiaIndex;

			if (m_nSOGTopSpacerDiaType == 1)
				m_nSOGTopSpacerSlabThickUnderDiaIndex = nSOGTopSpacerDiaIndex;

			if (m_nCircularRampTopSpacerDiaType == 1)
				m_nCircularRampTopSpacerSlabThickUnderDiaIndex = nCircularRampTopSpacerDiaIndex;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20190423)
		{
			ar >> m_bMatSpliceDevConditionSet;
			ar >> m_nMatSpliceDevConditionLength;
			ar >> m_bFlatSpliceDevConditionSet;
			ar >> m_nFlatSpliceDevConditionLength;
			ar >> m_bSlabSpliceDevConditionSet;
			ar >> m_nSlabSpliceDevConditionLength;
			ar >> m_bSOGSpliceDevConditionSet;
			ar >> m_nSOGSpliceDevConditionLength;
			ar >> m_bCircularRampSpliceDevConditionSet;
			ar >> m_nCircularRampSpliceDevConditionLength;
			ar >> m_bPCSlabSpliceDevConditionSet;
			ar >> m_nPCSlabSpliceDevConditionLength;
			ar >> m_bEmbeddedDeckSpliceDevConditionSet;
			ar >> m_nEmbeddedDeckSpliceDevConditionLength;
			ar >> m_bGeneralDeckSpliceDevConditionSet;
			ar >> m_nGeneralDeckSpliceDevConditionLength;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20190829)
		{
			// PC IMT Slab
			ar >> m_bPCIMTSlabSpliceDevConditionSet;
			ar >> m_nPCIMTSlabSpliceDevConditionLength;

			ar >> m_nPCIMTSlabMaxDualHookLength;
			ar >> m_nPCIMTSlabSpliceWay;
			ar >> m_nPCIMTSlabSpliceType;
			if (MSVersionInfo::GetCurrentVersion() < 20230510)
			{
				ar >> bDefaultValue;
				ar >> nDefaultValue;
			
				ar >> nDefaultValue;
				ar >> nDefaultValue;
				ar >> nDefaultValue;
			
				ar >> bDefaultValue;
				ar >> nDefaultValue;			

				ar >> nDefaultValue;
				ar >> nDefaultValue;
				ar >> nDefaultValue;
				ar >> nDefaultValue;
			}
			ar >> m_nPCIMTSlabRebarTolDist;
			ar >> m_nPCIMTSlabRebarTolLenDif;

			map<long, long> tmpLongLongMap;
			if (MSVersionInfo::GetCurrentVersion() < 20230510)
			{
				int NumObject;

				ar >> NumObject;
				if (NumObject > 0)
					tmpLongLongMap.clear();
				for (int index = 0; index < NumObject; index++)
				{
					long nDiaIndex = 0, hookLen = 0;

					ar >> nDiaIndex;
					ar >> hookLen;
					tmpLongLongMap.insert(make_pair(nDiaIndex, hookLen));
				}

				ar >> NumObject;
				if (NumObject > 0)
					tmpLongLongMap.clear();
				for (int index = 0; index < NumObject; index++)
				{
					long nDiaIndex = 0, hookLen = 0;

					ar >> nDiaIndex;
					ar >> hookLen;
					tmpLongLongMap.insert(make_pair(nDiaIndex, hookLen));
				}
			}
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20191001)
		{
			ar >> m_dMatSpacerSupportBarsUserRatio;
			ar >> m_dFlatSpacerSupportBarsUserRatio;
			ar >> m_dSlabSpacerSupportBarsUserRatio;
			ar >> m_dSOGSpacerSupportBarsUserRatio;
			ar >> m_dCircularRampSpacerSupportBarsUserRatio;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20191220)
		{
			ar >> m_bMatSpliceAddLengthSet;
			ar >> m_nMatSpliceAddLengthNumSplice;
			ar >> m_bFlatSpliceAddLengthSet;
			ar >> m_nFlatSpliceAddLengthNumSplice;
			ar >> m_bSOGSpliceAddLengthSet;
			ar >> m_nSOGSpliceAddLengthNumSplice;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20200702)
		{
			ar >> m_bSlabSpliceAddLengthSet;
			ar >> m_nSlabSpliceAddLengthNumSplice;
			ar >> m_bCircularRampSpliceAddLengthSet;
			ar >> m_nCircularRampSpliceAddLengthNumSplice;
			ar >> m_bEmbeddedDeckSpliceAddLengthSet;
			ar >> m_nEmbeddedDeckSpliceAddLengthNumSplice;
			ar >> m_bGeneralDeckSpliceAddLengthSet;
			ar >> m_nGeneralDeckSpliceAddLengthNumSplice;
			ar >> m_bPCSlabSpliceAddLengthSet;
			ar >> m_nPCSlabSpliceAddLengthNumSplice;
			ar >> m_bPCIMTSlabSpliceAddLengthSet;
			ar >> m_nPCIMTSlabSpliceAddLengthNumSplice;
		}
		
		if(MSVersionInfo::GetCurrentVersion() >= 20200716)
		{
			ar >> m_nPCIMTSlabLongSpliceBarType;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20200722)
		{
			m_pPCSlabHorSupportRebarsTemplate->Serialize(ar);
			m_pPCIMTSlabHorSupportRebarsTemplate->Serialize(ar);
			m_pGeneralDeckHorSupportRebarsTemplate->Serialize(ar);
			m_pEmbeddedDeckHorSupportRebarsTemplate->Serialize(ar);
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20210909)
		{
			if (MSVersionInfo::GetCurrentVersion() < 20230510)
			{
				ar >> nDefaultValue;
				ar >> nDefaultValue;
				ar >> nDefaultValue;
			}

			int NumObject = 0;

			ar >> NumObject;
			if (NumObject > 0)
				mm_EmbeddedDeckSpliceBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_EmbeddedDeckSpliceBarUserLen.insert(make_pair(nDiaIndex, len));
			}

			if (MSVersionInfo::GetCurrentVersion() < 20230510)
			{
				map<long, long> tmpLongLongMap;
				ar >> NumObject;
				if (NumObject > 0)
					tmpLongLongMap.clear();
				for (int index = 0; index < NumObject; index++)
				{
					long nDiaIndex = 0, len = 0;

					ar >> nDiaIndex;
					ar >> len;
					tmpLongLongMap.insert(make_pair(nDiaIndex, len));
				}
			}
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20221011)
		{
			ar >> m_bPCHCSSlabSpliceDevConditionSet;
			ar >> m_nPCHCSSlabSpliceDevConditionLength;
			ar >> m_bPCHCSSlabSpliceAddLengthSet;
			ar >> m_nPCHCSSlabSpliceAddLengthNumSplice;
			ar >> m_nPCHCSSlabMaxDualHookLength;	
			ar >> m_nPCHCSSlabSpliceWay;
			ar >> m_nPCHCSSlabSpliceType;
			if (MSVersionInfo::GetCurrentVersion() < 20230510)
			{
				ar >> bDefaultValue;
				ar >> nDefaultValue;
			
				ar >> nDefaultValue;
				ar >> nDefaultValue;
				ar >> nDefaultValue;
			
				ar >> bDefaultValue;
				ar >> nDefaultValue;
			

				ar >> nDefaultValue;
			}
			ar >> m_nPCHCSSlabTopEndAddBarLengthType;
			ar >> m_nPCHCSSlabDevSubBarLengthType;
			ar >> m_nPCHCSSlabShortCenSpliceBarLengthType;
			ar >> m_nPCHCSSlabLongCenSpliceBarLengthType;

			ar >> m_nPCHCSSlabRebarTolDist;
			ar >> m_nPCHCSSlabRebarTolLenDif;

			m_pPCHCSSlabHorSupportRebarsTemplate->Serialize(ar);

			int NumObject = 0;
			ar >> NumObject;
			if (NumObject > 0)
				mm_PCHCSSlabShortCenSpliceBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCHCSSlabShortCenSpliceBarUserLen.insert(make_pair(nDiaIndex, len));
			}

			ar >> NumObject;
			if (NumObject > 0)
				mm_PCHCSSlabLongCenSpliceBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCHCSSlabLongCenSpliceBarUserLen.insert(make_pair(nDiaIndex, len));
			}

			ar >> NumObject;
			if (NumObject > 0)
				mm_PCHCSSlabTopEndAddBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCHCSSlabTopEndAddBarUserLen.insert(make_pair(nDiaIndex, len));
			}

			ar >> NumObject;
			if (NumObject > 0)
				mm_PCHCSSlabDevSubBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCHCSSlabDevSubBarUserLen.insert(make_pair(nDiaIndex, len));
			}

			//RPS
			ar >> m_bPCRPSSlabSpliceDevConditionSet;
			ar >> m_nPCRPSSlabSpliceDevConditionLength;
			ar >> m_bPCRPSSlabSpliceAddLengthSet;
			ar >> m_nPCRPSSlabSpliceAddLengthNumSplice;
			ar >> m_nPCRPSSlabMaxDualHookLength;	
			ar >> m_nPCRPSSlabSpliceWay;	
			ar >> m_nPCRPSSlabSpliceType;
			if (MSVersionInfo::GetCurrentVersion() < 20230510)
			{
				ar >> bDefaultValue;
				ar >> nDefaultValue;
			
				ar >> nDefaultValue;
				ar >> nDefaultValue;
				ar >> nDefaultValue;
			
				ar >> bDefaultValue;
				ar >> nDefaultValue;			

				ar >> nDefaultValue;
			}
			ar >> m_nPCRPSSlabTopEndAddBarLengthType;
			ar >> m_nPCRPSSlabDevSubBarLengthType;
			ar >> m_nPCRPSSlabLongCenSpliceBarLengthType;

			ar >> m_nPCRPSSlabRebarTolDist;
			ar >> m_nPCRPSSlabRebarTolLenDif;

			m_pPCRPSSlabHorSupportRebarsTemplate->Serialize(ar);

			ar >> NumObject;
			if (NumObject > 0)
				mm_PCRPSSlabLongCenSpliceBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCRPSSlabLongCenSpliceBarUserLen.insert(make_pair(nDiaIndex, len));
			}

			ar >> NumObject;
			if (NumObject > 0)
				mm_PCRPSSlabTopEndAddBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCRPSSlabTopEndAddBarUserLen.insert(make_pair(nDiaIndex, len));
			}

			ar >> NumObject;
			if (NumObject > 0)
				mm_PCRPSSlabDevSubBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCRPSSlabDevSubBarUserLen.insert(make_pair(nDiaIndex, len));
			}
		}

		if(MSVersionInfo::GetCurrentVersion()  >= 20221216)
		{
			ar >> m_nPCHCSSlabTopEndAddBarLxLnxType;
			ar >> m_nPCRPSSlabTopEndAddBarLxLnxType;
		}
		else
		{
			m_nPCRPSSlabTopEndAddBarLengthType = m_nPCHCSSlabTopEndAddBarLengthType = 0;
		}

		//Q22-806
		if(MSVersionInfo::GetCurrentVersion()  >= 20221221)
		{
			int NumObject = 0;
			ar >>  m_bPCHCSSlabShortCenSpliceBarPlace;
			ar >>  m_bPCHCSSlabShortEndSpliceBarPlace;
			ar >>  m_bPCHCSSlabLongCenSpliceBarPlace;
			ar >>  m_bPCHCSSlabLongEndSpliceBarPlace;
			ar >> m_nPCHCSSlabShortEndSpliceBarLengthType;
			ar >> m_nPCHCSSlabLongEndSpliceBarLengthType;
			ar >> NumObject;
			if (NumObject > 0)
				mm_PCHCSSlabShortEndSpliceBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCHCSSlabShortEndSpliceBarUserLen.insert(make_pair(nDiaIndex, len));
			}

			ar >> NumObject;
			if (NumObject > 0)
				mm_PCHCSSlabLongEndSpliceBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCHCSSlabLongEndSpliceBarUserLen.insert(make_pair(nDiaIndex, len));
			}

			ar >> m_nPCRPSSlabLongEndSpliceBarLengthType;
			ar >>  m_bPCRPSSlabLongCenSpliceBarPlace;
			ar >>  m_bPCRPSSlabLongEndSpliceBarPlace;
			ar >> NumObject;
			if (NumObject > 0)
				mm_PCRPSSlabLongEndSpliceBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCRPSSlabLongEndSpliceBarUserLen.insert(make_pair(nDiaIndex, len));
			}
		}
		if(MSVersionInfo::GetCurrentVersion()  >= 20230224)
		{
			int NumObject = 0;

			ar >> m_nPCHCSSlabTopCenAddBarLxLnxType;
			ar >> m_nPCHCSSlabTopCenAddBarLengthType;
			ar >> NumObject;
			if (NumObject > 0)
				mm_PCHCSSlabTopCenAddBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCHCSSlabTopCenAddBarUserLen.insert(make_pair(nDiaIndex, len));
			}

			ar >> m_nPCRPSSlabTopCenAddBarLxLnxType;
			ar >> m_nPCRPSSlabTopCenAddBarLengthType;
			ar >> NumObject;
			if (NumObject > 0)
				mm_PCRPSSlabTopCenAddBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCRPSSlabTopCenAddBarUserLen.insert(make_pair(nDiaIndex, len));
			}

			ar >> m_nGeneralDeckTopEndAddBarLxLnxType;
			ar >> m_nGeneralDeckTopEndAddBarLengthType;
			ar >> m_nGeneralDeckTopCenAddBarLxLnxType;
			ar >> m_nGeneralDeckTopCenAddBarLengthType;

			ar >> NumObject;
			if (NumObject > 0)
				mm_GeneralDeckTopEndAddBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_GeneralDeckTopEndAddBarUserLen.insert(make_pair(nDiaIndex, len));
			}
			ar >> NumObject;
			if (NumObject > 0)
				mm_GeneralDeckTopCenAddBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_GeneralDeckTopCenAddBarUserLen.insert(make_pair(nDiaIndex, len));
			}
		}
		else
		{
			m_nPCHCSSlabTopCenAddBarLxLnxType = m_nPCHCSSlabTopEndAddBarLxLnxType;
			m_nPCHCSSlabTopCenAddBarLengthType = m_nPCHCSSlabTopEndAddBarLengthType;
			m_nPCRPSSlabTopCenAddBarLxLnxType = m_nPCRPSSlabTopEndAddBarLxLnxType;
			m_nPCRPSSlabTopCenAddBarLengthType = m_nPCRPSSlabTopEndAddBarLengthType;
			mm_PCHCSSlabTopCenAddBarUserLen = mm_PCHCSSlabTopEndAddBarUserLen;
			mm_PCRPSSlabTopCenAddBarUserLen = mm_PCRPSSlabTopEndAddBarUserLen;
		}
		if(MSVersionInfo::GetCurrentVersion()  >= 20230412)
		{
			ar >> m_nMatCZoneSpliceWay;
			ar >> m_nMatCZoneSpliceLength;
			ar >> m_nFlatCZoneSpliceWay;
			ar >> m_nFlatCZoneSpliceLength;
			ar >> m_nSlabCZoneSpliceWay;
			ar >> m_nSlabCZoneSpliceLength;
			ar >> m_nSOGCZoneSpliceWay;
			ar >> m_nSOGCZoneSpliceLength;
			ar >> m_nCircularRampCZoneSpliceWay;
			ar >> m_nCircularRampCZoneSpliceLength;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20230510)
		{
			ar >> m_nMatTopEndDevTypeUserLength;
			ar >> m_nMatBotEndDevTypeUserLength;
			ar >> m_nMatOpeningTopDevType;
			ar >> m_nMatOpeningBotDevType;

			ar >> m_nSOGTopEndDevTypeUserLength;
			ar >> m_nSOGBotEndDevTypeUserLength;

			ar >> m_nFlatTopEndDevTypeUserLength;
			ar >> m_nFlatBotEndDevTypeUserLength;

			ar >> m_nCircularRampTopEndDevTypeUserLength;
			ar >> m_nCircularRampBotEndDevTypeUserLength;

			ar >> m_nSlabTopEndDevTypeUserLength;
			ar >> m_nSlabBotEndDevTypeUserLength;
			
			ar >> m_nEmbeddedDeckTopEndAddBarLxLnxType;
			ar >> m_nEmbeddedDeckTopEndAddBarLengthType;
			ar >> m_nEmbeddedDeckTopCenAddBarLxLnxType;
			ar >> m_nEmbeddedDeckTopCenAddBarLengthType;

			ar >> m_bEmbeddedDeckSpliceBarPlace;
			ar >> m_nEmbeddedDeckSpliceBarLengthType;

			ar >> m_nEmbeddedDeckTopEndDevTypeUserLength;
			ar >> m_nEmbeddedDeckBotEndDevTypeUserLength;

			ar >> m_bGeneralDeckSpliceBarPlace;
			ar >> m_nGeneralDeckSpliceBarLengthType;
			ar >> m_nGeneralDeckTopEndDevTypeUserLength;
			ar >> m_nGeneralDeckBotEndDevTypeUserLength;

			ar >> m_nPCSlabTopEndAddBarLxLnxType;
			ar >> m_nPCSlabTopEndAddBarLengthType;
			ar >> m_nPCSlabTopCenAddBarLxLnxType;
			ar >> m_nPCSlabTopCenAddBarLengthType;

			ar >> m_nPCSlabTopEndDevTypeUserLength;
			ar >> m_nPCSlabBotEndDevTypeUserLength;

			ar >> m_bPCSlabShortCenSpliceBarPlace;
			ar >> m_nPCSlabShortCenSpliceBarLengthType;
			ar >> m_bPCSlabShortEndSpliceBarPlace;
			ar >> m_nPCSlabShortEndSpliceBarLengthType;
			ar >> m_bPCSlabLongCenSpliceBarPlace;
			ar >> m_nPCSlabLongCenSpliceBarLengthType;
			ar >> m_bPCSlabLongEndSpliceBarPlace;
			ar >> m_nPCSlabLongEndSpliceBarLengthType;

			int NumObject = 0;

			ar >> NumObject;
			if (NumObject > 0)
				mm_PCSlabTopEndAddBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCSlabTopEndAddBarUserLen.insert(make_pair(nDiaIndex, len));
			}
			ar >> NumObject;
			if (NumObject > 0)
				mm_PCSlabTopCenAddBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCSlabTopCenAddBarUserLen.insert(make_pair(nDiaIndex, len));
			}
			ar >> NumObject;
			if (NumObject > 0)
				mm_PCSlabShortCenSpliceBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCSlabShortCenSpliceBarUserLen.insert(make_pair(nDiaIndex, len));
			}
			ar >> NumObject;
			if (NumObject > 0)
				mm_PCSlabShortEndSpliceBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCSlabShortEndSpliceBarUserLen.insert(make_pair(nDiaIndex, len));
			}
			ar >> NumObject;
			if (NumObject > 0)
				mm_PCSlabLongCenSpliceBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCSlabLongCenSpliceBarUserLen.insert(make_pair(nDiaIndex, len));
			}
			ar >> NumObject;
			if (NumObject > 0)
				mm_PCSlabLongEndSpliceBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCSlabLongEndSpliceBarUserLen.insert(make_pair(nDiaIndex, len));
			}

			ar >> m_nPCHCSSlabTopHookType;
			ar >> m_nPCHCSSlabBotHookType;
			ar >> m_nPCHCSSlabTopEndDevTypeUserLength;
			ar >> m_nPCHCSSlabBotEndDevTypeUserLength;
			ar >> m_nPCHCSSlabOpeningTopDevType;
			ar >> m_nPCHCSSlabOpeningBotDevType;

			ar >> m_nPCRPSSlabTopHookType;
			ar >> m_nPCRPSSlabBotHookType;
			ar >> m_nPCRPSSlabTopEndDevTypeUserLength;
			ar >> m_nPCRPSSlabBotEndDevTypeUserLength;
			ar >> m_nPCRPSSlabOpeningTopDevType;
			ar >> m_nPCRPSSlabOpeningBotDevType;

			ar >> m_nPCIMTSlabTopHookType;
			ar >> m_nPCIMTSlabBotHookType;
			ar >> m_nPCIMTSlabTopEndDevTypeUserLength;
			ar >> m_nPCIMTSlabBotEndDevTypeUserLength;
			ar >> m_nPCIMTSlabOpeningTopDevType;
			ar >> m_nPCIMTSlabOpeningBotDevType;
			ar >> m_nPCIMTSlabTopEndAddBarLxLnxType;
			ar >> m_nPCIMTSlabTopEndAddBarLengthType;
			ar >> m_nPCIMTSlabTopCenAddBarLxLnxType;
			ar >> m_nPCIMTSlabTopCenAddBarLengthType;
			ar >> m_bPCIMTSlabShortSpliceBarPlace;
			ar >> m_nPCIMTSlabShortSpliceBarLengthType;
			ar >> m_bPCIMTSlabLongSpliceBarPlace;

			ar >> NumObject;
			if (NumObject > 0)
				mm_PCIMTSlabTopEndAddBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCIMTSlabTopEndAddBarUserLen.insert(make_pair(nDiaIndex, len));
			}
			ar >> NumObject;
			if (NumObject > 0)
				mm_PCIMTSlabTopCenAddBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCIMTSlabTopCenAddBarUserLen.insert(make_pair(nDiaIndex, len));
			}
			ar >> NumObject;
			if (NumObject > 0)
				mm_PCIMTSlabShortSpliceBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_PCIMTSlabShortSpliceBarUserLen.insert(make_pair(nDiaIndex, len));
			}

			ar >> NumObject;
			if (NumObject > 0)
				mm_EmbeddedDeckTopEndAddBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_EmbeddedDeckTopEndAddBarUserLen.insert(make_pair(nDiaIndex, len));
			}
			ar >> NumObject;
			if (NumObject > 0)
				mm_EmbeddedDeckTopCenAddBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_EmbeddedDeckTopCenAddBarUserLen.insert(make_pair(nDiaIndex, len));
			}
			ar >> NumObject;
			if (NumObject > 0)
				mm_GeneralDeckSpliceBarUserLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, len = 0;

				ar >> nDiaIndex;
				ar >> len;
				mm_GeneralDeckSpliceBarUserLen.insert(make_pair(nDiaIndex, len));
			}
		}
		else
		{
		//슬래브 이전 상하부근 값 변경 0정착 1 후크 -> 2,3
			if(m_nMatTopEndHookType == 1)
				m_nMatTopEndHookType = 2;
			if(m_nMatBotEndHookType == 1)
				m_nMatBotEndHookType = 3;

			if(m_nFlatTopEndHookType == 1)
				m_nFlatTopEndHookType = 2;
			if(m_nFlatBotEndHookType == 1)
				m_nFlatBotEndHookType = 3;

			if(nOldSlabTopEndHookType == 1)
				m_nSlabTopUnStructuredEndHookType = 2;
			else if(nOldSlabTopEndHookType == 0)
				m_nSlabTopUnStructuredEndHookType = 0;
			if(nOldSlabBotEndHookType == 1)
				m_nSlabBotUnStructuredEndHookType = 1;
			else if(nOldSlabBotEndHookType == 0)
				m_nSlabBotUnStructuredEndHookType = 0;

			if(m_nSOGTopEndHookType == 1)
				m_nSOGTopEndHookType = 2;
			if(m_nSOGBotEndHookType == 1)
				m_nSOGBotEndHookType = 3;


			if(m_nCircularRampTopEndHookType == 1)
				m_nCircularRampTopEndHookType = 2;
			if(m_nCircularRampBotEndHookType == 1)
				m_nCircularRampBotEndHookType = 3;

			/*if(nOldPCSlabTopEndHookType == 1)
				m_nPCSlabTopUnStructuredEndHookType = 2;
			else if(nOldPCSlabTopEndHookType == 0)
				m_nPCSlabTopUnStructuredEndHookType = 0;
			if(nOldPCSlabBotEndHookType == 1)
				m_nPCSlabBotUnStructuredEndHookType = 1;
			else if(nOldPCSlabBotEndHookType == 0)
				m_nPCSlabBotUnStructuredEndHookType = 0;

			if(nOldEmbeddedDeckTopEndHookType == 1)
				m_nEmbeddedDeckTopUnStructuredEndHookType = 2;
			else if(nOldEmbeddedDeckTopEndHookType == 1)
				m_nEmbeddedDeckTopUnStructuredEndHookType = 0;			

			if(nOldGeneralDeckTopEndHookType == 1)
				m_nGeneralDeckTopUnStructuredEndHookType = 2;
			else if(nOldGeneralDeckTopEndHookType == 1)
				m_nGeneralDeckTopUnStructuredEndHookType = 0;
			*/

			m_nPCSlabTopUnStructuredEndHookType = 2;			
			//m_nPCSlabBoTUnStructuredEndHookType = 3;
			m_nEmbeddedDeckTopUnStructuredEndHookType = 2;
			if(nOldEmbeddedDeckBotEndHookType == 1)
				m_nEmbeddedDeckBotUnStructuredEndHookType = 1;
			else if(nOldEmbeddedDeckBotEndHookType == 0)
				m_nEmbeddedDeckBotUnStructuredEndHookType = 0;
									
			m_nGeneralDeckTopUnStructuredEndHookType = 2;
			if(nOldGeneralDeckBotEndHookType == 1)
				m_nGeneralDeckBotUnStructuredEndHookType = 1;
			else if(nOldGeneralDeckBotEndHookType == 0)
				m_nGeneralDeckBotUnStructuredEndHookType = 0;

			m_nPCIMTSlabTopHookType = 2;
			m_nPCIMTSlabBotHookType = 3;
			m_nPCHCSSlabTopHookType = 2;
			m_nPCHCSSlabBotHookType = 3;
			m_nPCRPSSlabTopHookType = 2;
			m_nPCRPSSlabBotHookType = 3;

			m_nMatOpeningTopDevType = m_nFlatOpeningTopDevType;
			m_nMatOpeningBotDevType = m_nFlatOpeningBotDevType;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20230608)
		{
			m_pMatShearFrictionBarTemplate->Serialize(ar);
			m_pFlatShearFrictionBarTemplate->Serialize(ar);
		}
		if (MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231010)
		{
			ar >> m_bMatBotSpacerUserSteelGradeS;
			ar >> m_bMatTopSpacerUserSteelGradeS;
			ar >> m_bMatSpacerSupportBarUserSteelGradeS;
			ar >> m_bMatSupportRebarUserSteelGradeS;

			ar >> m_bFlatBotSpacerUserSteelGradeS;
			ar >> m_bFlatTopSpacerUserSteelGradeS;
			ar >> m_bFlatSpacerSupportBarUserSteelGradeS;

			ar >> m_bSlabBotSpacerUserSteelGradeS;
			ar >> m_bSlabTopSpacerUserSteelGradeS;
			ar >> m_bSlabSpacerSupportBarUserSteelGradeS;

			ar >> m_bSOGBotSpacerUserSteelGradeS;
			ar >> m_bSOGTopSpacerUserSteelGradeS;
			ar >> m_bSOGSpacerSupportBarUserSteelGradeS;
			ar >> m_bSOGSupportRebarUserSteelGradeS;

			ar >> m_bCircularRampBotSpacerUserSteelGradeS;
			ar >> m_bCircularRampTopSpacerUserSteelGradeS;
			ar >> m_bCircularRampSpacerSupportBarUserSteelGradeS;
		}
		if((MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231230) || MSVersionInfo::IsOldTotalVersion())
		{
			ar >> m_nEmbeddedDeckCZoneSpliceWay;
			ar >> m_nEmbeddedDeckCZoneSpliceLength;
			ar >> m_nGeneralDeckCZoneSpliceWay;
			ar >> m_nGeneralDeckCZoneSpliceLength;
			ar >> m_nPCSlabCZoneSpliceWay;
			ar >> m_nPCSlabCZoneSpliceLength;
			ar >> m_nPCIMTSlabCZoneSpliceWay;
			ar >> m_nPCIMTSlabCZoneSpliceLength;
			ar >> m_nPCHCSSlabCZoneSpliceWay;
			ar >> m_nPCHCSSlabCZoneSpliceLength;
			ar >> m_nPCRPSSlabCZoneSpliceWay;
			ar >> m_nPCRPSSlabCZoneSpliceLength;
		}
		if(MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20240306)
		{
			ar >> m_bMatTopSpacerPlaceConditionSet;
			ar >> m_nMatTopSpacerPlaceCondition;
			ar >> m_bFlatTopSpacerPlaceConditionSet;
			ar >> m_nFlatTopSpacerPlaceCondition;
			ar >> m_bSlabTopSpacerPlaceConditionSet;
			ar >> m_nSlabTopSpacerPlaceCondition;
			ar >> m_bSOGTopSpacerPlaceConditionSet;
			ar >> m_nSOGTopSpacerPlaceCondition;
			ar >> m_bCircularRampTopSpacerPlaceConditionSet;
			ar >> m_nCircularRampTopSpacerPlaceCondition;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20240416)
		{
			ar >> m_bSlabOpeningAddBarDeductionCondition;
			ar >> m_nSlabOpeningAddBarDeductionPercent;
		}

		if(MSVersionInfo::GetCurrentVersion() < 20240906)
		{
			if(m_nGeneralDeckTopEndAddBarLengthType == 2)
				m_nGeneralDeckTopEndAddBarLengthType = 3;
			if(m_nGeneralDeckTopCenAddBarLengthType == 2)
				m_nGeneralDeckTopCenAddBarLengthType = 3;
			if(m_nEmbeddedDeckTopEndAddBarLengthType == 2)
				m_nEmbeddedDeckTopEndAddBarLengthType = 3;
			if(m_nEmbeddedDeckTopCenAddBarLengthType == 2)
				m_nEmbeddedDeckTopCenAddBarLengthType = 3;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20241018)
		{
			int NumObject = 0;

			mm_MatThickToDiaIndex.clear();
			ar >> NumObject;
			if (NumObject > 0)
				mm_MatThickToDiaIndex.clear();
			for (int index = 0; index < NumObject; index++)
			{
				CString sThick = _T("");
				long nDiaIndex = 0;

				ar >> sThick;
				ar >> nDiaIndex;
				mm_MatThickToDiaIndex.insert(make_pair(sThick, nDiaIndex));
			}


			mm_FlatThickToDiaIndex.clear();
			ar >> NumObject;
			if (NumObject > 0)
				mm_FlatThickToDiaIndex.clear();
			for (int index = 0; index < NumObject; index++)
			{
				CString sThick = _T("");
				long nDiaIndex = 0;

				ar >> sThick;
				ar >> nDiaIndex;
				mm_FlatThickToDiaIndex.insert(make_pair(sThick, nDiaIndex));
			}

			mm_SlabThickToDiaIndex.clear();
			ar >> NumObject;
			if (NumObject > 0)
				mm_SlabThickToDiaIndex.clear();
			for (int index = 0; index < NumObject; index++)
			{
				CString sThick = _T("");
				long nDiaIndex = 0;

				ar >> sThick;
				ar >> nDiaIndex;
				mm_SlabThickToDiaIndex.insert(make_pair(sThick, nDiaIndex));
			}

			mm_SOGThickToDiaIndex.clear();
			ar >> NumObject;
			if (NumObject > 0)
				mm_SOGThickToDiaIndex.clear();
			for (int index = 0; index < NumObject; index++)
			{
				CString sThick = _T("");
				long nDiaIndex = 0;

				ar >> sThick;
				ar >> nDiaIndex;
				mm_SOGThickToDiaIndex.insert(make_pair(sThick, nDiaIndex));
			}

			mm_CircularRampThickToDiaIndex.clear();
			ar >> NumObject;
			if (NumObject > 0)
				mm_CircularRampThickToDiaIndex.clear();
			for (int index = 0; index < NumObject; index++)
			{
				CString sThick = _T("");
				long nDiaIndex = 0;

				ar >> sThick;
				ar >> nDiaIndex;
				mm_CircularRampThickToDiaIndex.insert(make_pair(sThick, nDiaIndex));
			}
		}
		else
		{	
			mm_MatThickToDiaIndex.clear();
			mm_FlatThickToDiaIndex.clear();
			mm_SlabThickToDiaIndex.clear();
			mm_SOGThickToDiaIndex.clear();
			mm_CircularRampThickToDiaIndex.clear();

			CString sThick;

			sThick.Format(_T("%ld"), m_nMatTopSpacerSlabThick - 1);
			mm_MatThickToDiaIndex.insert(make_pair(sThick, m_nMatTopSpacerSlabThickUnderDiaIndex));
			mm_MatThickToDiaIndex.insert(make_pair(_T("Default"), m_nMatTopSpacerSlabThickOverDiaIndex));

			sThick.Format(_T("%ld"), m_nFlatTopSpacerSlabThick - 1);
			mm_FlatThickToDiaIndex.insert(make_pair(sThick, m_nFlatTopSpacerSlabThickUnderDiaIndex));
			mm_FlatThickToDiaIndex.insert(make_pair(_T("Default"), m_nFlatTopSpacerSlabThickOverDiaIndex));

			sThick.Format(_T("%ld"), m_nSlabTopSpacerSlabThick - 1);
			mm_SlabThickToDiaIndex.insert(make_pair(sThick, m_nSlabTopSpacerSlabThickUnderDiaIndex));
			mm_SlabThickToDiaIndex.insert(make_pair(_T("Default"), m_nSlabTopSpacerSlabThickOverDiaIndex));

			sThick.Format(_T("%ld"), m_nSOGTopSpacerSlabThick - 1);
			mm_SOGThickToDiaIndex.insert(make_pair(sThick, m_nSOGTopSpacerSlabThickUnderDiaIndex));
			mm_SOGThickToDiaIndex.insert(make_pair(_T("Default"), m_nSOGTopSpacerSlabThickOverDiaIndex));

			sThick.Format(_T("%ld"), m_nCircularRampTopSpacerSlabThick - 1);
			mm_CircularRampThickToDiaIndex.insert(make_pair(sThick, m_nCircularRampTopSpacerSlabThickUnderDiaIndex));
			mm_CircularRampThickToDiaIndex.insert(make_pair(_T("Default"), m_nCircularRampTopSpacerSlabThickOverDiaIndex));
		}		

		if (MSVersionInfo::GetCurrentVersion() >= 20241115)
		{
			ar >> m_bSlabLongEndPlaceAreaSet;
			ar >> m_nSlabLongEndPlaceAreaOneWaySlabType;
			ar >> m_nSlabLongEndPlaceAreaTwoWaySlabType;
			ar >> m_bSlabBotAddBarLengthSet;
			ar >> m_nSlabBotAddBarLengthType;
			ar >> m_bSlabBotAddBarCutBarLenExclusionSet;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20241120)
			ar >> m_nSlabOpeningAddBarDevLengthType;
	}
}

void SlabRebarTemplate::DataInit()
{	
	// MAT Slab
	m_bMatMainBarInfoSet = false;
	m_nMatMaxDualHookLength = 2000;
	m_nMatSpliceWay = 0;
	m_nMatSpliceType = 0;

	m_bMatEndInfoSet = false;
	m_nMatTopEndHookType = 1;
	m_nMatBotEndHookType = 3;
	m_nMatIsoFootDevType = 1;

	m_nMatTopEndDevTypeUserLength = 0;
	m_nMatBotEndDevTypeUserLength = 0;
	m_nMatOpeningTopDevType = 1;
	m_nMatOpeningBotDevType = 1;

	m_bMatLevelDifInfoSet = false;
	m_nMatLevelDifPlaceShape = 0;
	m_nMatLevelDifHaunchPlaceType = 0;

	m_bMatSupportRebarsSet = false;
	m_nMatSupportRebarsDiaIndex = 0;
	m_nMatSupportRebarsLengthType = 1;
	m_dMatSupportRebarsRWallLengthRate = 1.0;
	m_dMatSupportRebarsSlabLengthRate = 1.0;
	m_nMatSupportRebarsOrderLength = 8000;
	m_nMatSupportRebarsSpliceLength = 300;
	m_nMatSupportRebarsCorrectiveLength = 10;

	m_bMatRebarAreaInfoSet = false;
	m_nMatRebarTolDist = 10;
	m_nMatRebarTolLenDif = 10;

	m_bMatSpliceDevConditionSet = false;
	m_nMatSpliceDevConditionLength = 340;

	m_bMatTopSpacerInfoSet = false;
	m_bMatTopSpacerSet = false;
	m_nMatTopSpacerDiaType = 0;
	m_nMatTopSpacerSlabThick = 700;
	m_nMatTopSpacerSlabThickOverDiaIndex = 2;
	m_nMatTopSpacerSlabThickUnderDiaIndex = 1;
	m_nMatTopSpacerSpacingType = 1;
	m_nMatTopSpacerSlabSpacing = 3;
	m_nMatTopSpacerUserSpacing1 = 1500;
	m_nMatTopSpacerUserSpacing2 = 1500;
	m_nMatTopSpacerHeightType = 0;
	m_nMatTopSpacerUserHeight = 300;
	m_nMatTopSpacerHookLenAType = 1;
	m_nMatTopSpacerMainBarHookLenA = 100;
	m_nMatTopSpacerUserHookLenA = 100;
	m_nMatTopSpacerHookLenBType = 1;
	m_nMatTopSpacerMainBarHookLenB = 0;
	m_nMatTopSpacerUserHookLenB = 100;
	m_nMatTopSpacerWidth = 200;

	m_bMatBotSpacerInfoSet = false;
	m_bMatBotSpacerSet = false;
	m_nMatBotSpacerDiaType = 0;
	m_nMatBotSpacerDiaIndex = 0;
	m_nMatBotSpacerSpacingType = 1;
	m_nMatBotSpacerSlabSpacing = 3;
	m_nMatBotSpacerUserSpacing1 = 1500;
	m_nMatBotSpacerUserSpacing2 = 1500;
	m_nMatBotSpacerHeightType = 0;
	m_nMatBotSpacerUserHeight = 300;
	m_nMatBotSpacerHookLenAType = 1;
	m_nMatBotSpacerMainBarHookLenA = 100;
	m_nMatBotSpacerUserHookLenA = 100;
	m_nMatBotSpacerHookLenBType = 1;
	m_nMatBotSpacerMainBarHookLenB = 0;
	m_nMatBotSpacerUserHookLenB = 100;
	m_nMatBotSpacerWidth = 200;

	m_bMatSpacerSupportBarsSet = false;
	m_bMatSpacerSupportBarsConditionSet = true;
	m_nMatSpacerSupportBarsCondition = 1000;
	m_nMatSpacerSupportBarsDiaType = 1;
	m_nMatSpacerSupportBarsUserDiaIndex = 0;
	m_nMatSpacerSupportBarsNumType = 0;
	m_dMatSpacerSupportBarsTopSpacerNums = 0.5;
	m_nMatSpacerSupportBarsUserNums = 10;
	m_nMatSpacerSupportBarsLengthType = 1;
	m_nMatSpacerSupportBarsUserLength = 1600;

	m_bMatSpliceAddLengthSet = true;
	m_nMatSpliceAddLengthNumSplice = 3;
	
	m_nMatCZoneSpliceWay = 0;
	m_nMatCZoneSpliceLength = 300;
	
	// FLAT Slab
	m_bFlatMainBarInfoSet = false;
	m_nFlatMaxDualHookLength = 2000;
	m_nFlatSpliceWay = 0;
	m_nFlatSpliceType = 0;

	m_bFlatEndInfoSet = false;
	m_nFlatTopEndHookType = 1;
	m_nFlatBotEndHookType = 3;
	m_nFlatIsoFootDevType = 1;
	m_nFlatSpliceDevSlabType = 1;

	m_nFlatTopEndDevTypeUserLength = 0;
	m_nFlatBotEndDevTypeUserLength = 0;
	m_nFlatOpeningTopDevType = 1;
	m_nFlatOpeningBotDevType = 1;

	m_bFlatLevelDifInfoSet = false;
	m_nFlatLevelDifPlaceShape = 0;
	m_nFlatLevelDifHaunchPlaceType = 0;
	
	m_bFlatOpeningInfoSet = false;
	
	m_bFlatRebarAreaInfoSet = false;
	m_nFlatRebarTolDist = 10;
	m_nFlatRebarTolLenDif = 10;

	m_bFlatSpliceDevConditionSet = false;
	m_nFlatSpliceDevConditionLength = 340;

	m_bFlatTopSpacerInfoSet = false;
	m_bFlatTopSpacerSet = false;
	m_nFlatTopSpacerDiaType = 0;
	m_nFlatTopSpacerSlabThick = 700;
	m_nFlatTopSpacerSlabThickOverDiaIndex = 2;
	m_nFlatTopSpacerSlabThickUnderDiaIndex = 1;
	m_nFlatTopSpacerSpacingType = 1;
	m_nFlatTopSpacerSlabSpacing = 3;
	m_nFlatTopSpacerUserSpacing1 = 1500;
	m_nFlatTopSpacerUserSpacing2 = 1500;
	m_nFlatTopSpacerHeightType = 0;
	m_nFlatTopSpacerUserHeight = 300;
	m_nFlatTopSpacerHookLenAType = 1;
	m_nFlatTopSpacerMainBarHookLenA = 100;
	m_nFlatTopSpacerUserHookLenA = 100;
	m_nFlatTopSpacerHookLenBType = 1;
	m_nFlatTopSpacerMainBarHookLenB = 0;
	m_nFlatTopSpacerUserHookLenB = 100;
	m_nFlatTopSpacerWidth = 200;

	m_bFlatBotSpacerInfoSet = false;
	m_bFlatBotSpacerSet = false;
	m_nFlatBotSpacerDiaType = 0;
	m_nFlatBotSpacerDiaIndex = 0;
	m_nFlatBotSpacerSpacingType = 1;
	m_nFlatBotSpacerSlabSpacing = 3;
	m_nFlatBotSpacerUserSpacing1 = 1500;
	m_nFlatBotSpacerUserSpacing2 = 1500;
	m_nFlatBotSpacerHeightType = 0;
	m_nFlatBotSpacerUserHeight = 300;
	m_nFlatBotSpacerHookLenAType = 1;
	m_nFlatBotSpacerMainBarHookLenA = 100;
	m_nFlatBotSpacerUserHookLenA = 100;
	m_nFlatBotSpacerHookLenBType = 1;
	m_nFlatBotSpacerMainBarHookLenB = 0;
	m_nFlatBotSpacerUserHookLenB = 100;
	m_nFlatBotSpacerWidth = 200;

	m_bFlatSpacerSupportBarsSet = false;
	m_bFlatSpacerSupportBarsConditionSet = true;
	m_nFlatSpacerSupportBarsCondition = 1000;
	m_nFlatSpacerSupportBarsDiaType = 1;
	m_nFlatSpacerSupportBarsUserDiaIndex = 0;
	m_nFlatSpacerSupportBarsNumType = 0;
	m_dFlatSpacerSupportBarsTopSpacerNums = 0.5;
	m_nFlatSpacerSupportBarsUserNums = 10;
	m_nFlatSpacerSupportBarsLengthType = 1;
	m_nFlatSpacerSupportBarsUserLength = 1600;

	m_bFlatSpliceAddLengthSet = true;
	m_nFlatSpliceAddLengthNumSplice = 3;
	
	m_nFlatCZoneSpliceWay = 0;
	m_nFlatCZoneSpliceLength = 300;
	
	// 사변지지 SLAB
	m_bSlabMainBarInfoSet = false;
	m_nSlabMaxDualHookLength = 2000;
	m_nSlabSpliceWay = 0;
	m_nSlabSpliceType = 0;
	
	m_bSlabTopCutBarLenInfoSet = false;
	m_nSlabOneWayCutBarLenType = 0;
	m_nSlabTwoWayCutBarLenType = 0;

	m_bSlabUnStructuredEndInfoSet = false;
	m_nSlabTopUnStructuredEndHookType = 0;
	m_nSlabBotUnStructuredEndHookType = 0;

	m_nSlabTopEndDevTypeUserLength = 0;
	m_nSlabBotEndDevTypeUserLength = 0;
	m_nSlabOpeningTopDevType = 1;
	m_nSlabOpeningBotDevType = 1;

	m_bSlabLevelDifInfoSet = false;
	m_nSlabLevelDifPlaceShape = 0;
	m_nSlabLevelDifHaunchPlaceType = 0;

	m_bSlabOpeningInfoSet = false;
	
	m_bSlabRebarAreaInfoSet = false;
	m_nSlabRebarTolDist = 10;
	m_nSlabRebarTolLenDif = 10;

	m_bSlabSpliceDevConditionSet = false;
	m_nSlabSpliceDevConditionLength = 340;

	m_bSlabSpliceAddLengthSet = true;
	m_nSlabSpliceAddLengthNumSplice = 3;

	m_bSlabTopSpacerInfoSet = false;
	m_bSlabTopSpacerSet = false;
	m_nSlabTopSpacerDiaType = 0;
	m_nSlabTopSpacerSlabThick = 700;
	m_nSlabTopSpacerSlabThickOverDiaIndex = 2;
	m_nSlabTopSpacerSlabThickUnderDiaIndex = 1;
	m_nSlabTopSpacerSpacingType = 1;
	m_nSlabTopSpacerSlabSpacing = 3;
	m_nSlabTopSpacerUserSpacing1 = 1500;
	m_nSlabTopSpacerUserSpacing2 = 1500;
	m_nSlabTopSpacerHeightType = 0;
	m_nSlabTopSpacerUserHeight = 300;
	m_nSlabTopSpacerHookLenAType = 1;
	m_nSlabTopSpacerMainBarHookLenA = 100;
	m_nSlabTopSpacerUserHookLenA = 100;
	m_nSlabTopSpacerHookLenBType = 1;
	m_nSlabTopSpacerMainBarHookLenB = 0;
	m_nSlabTopSpacerUserHookLenB = 100;
	m_nSlabTopSpacerWidth = 200;

	m_bSlabBotSpacerInfoSet = false;
	m_bSlabBotSpacerSet = false;
	m_nSlabBotSpacerDiaType = 0;
	m_nSlabBotSpacerDiaIndex = 0;
	m_nSlabBotSpacerSpacingType = 1;
	m_nSlabBotSpacerSlabSpacing = 3;
	m_nSlabBotSpacerUserSpacing1 = 1500;
	m_nSlabBotSpacerUserSpacing2 = 1500;
	m_nSlabBotSpacerHeightType = 0;
	m_nSlabBotSpacerUserHeight = 300;
	m_nSlabBotSpacerHookLenAType = 1;
	m_nSlabBotSpacerMainBarHookLenA = 100;
	m_nSlabBotSpacerUserHookLenA = 100;
	m_nSlabBotSpacerHookLenBType = 1;
	m_nSlabBotSpacerMainBarHookLenB = 0;
	m_nSlabBotSpacerUserHookLenB = 100;
	m_nSlabBotSpacerWidth = 200;

	m_bSlabSpacerSupportBarsSet = false;
	m_bSlabSpacerSupportBarsConditionSet = true;
	m_nSlabSpacerSupportBarsCondition = 1000;
	m_nSlabSpacerSupportBarsDiaType = 1;
	m_nSlabSpacerSupportBarsUserDiaIndex = 0;
	m_nSlabSpacerSupportBarsNumType = 0;
	m_dSlabSpacerSupportBarsTopSpacerNums = 0.5;
	m_nSlabSpacerSupportBarsUserNums = 10;
	m_nSlabSpacerSupportBarsLengthType = 1;
	m_nSlabSpacerSupportBarsUserLength = 1600;

	m_nSlabCZoneSpliceWay = 0;
	m_nSlabCZoneSpliceLength = 300;

	// SOG Slab
	m_bSOGMainBarInfoSet = false;
	m_nSOGMaxDualHookLength = 2000;
	m_nSOGSpliceWay = 0;
	m_nSOGSpliceType = 0;
	
	m_bSOGEndInfoSet = false;
	m_nSOGTopEndHookType = 1;
	m_nSOGBotEndHookType = 3;
	m_nSOGIsoFootDevType = 1;
	m_nSOGSpliceDevSlabType = 1;

	m_nSOGTopEndDevTypeUserLength = 0;
	m_nSOGBotEndDevTypeUserLength = 0;
	m_nSOGOpeningTopDevType = 1;
	m_nSOGOpeningBotDevType = 1;

	m_bSOGLevelDifInfoSet = false;
	m_nSOGLevelDifPlaceShape = 0;
	m_nSOGLevelDifHaunchPlaceType = 0;

	m_bSOGSupportRebarsSet = false;
	m_nSOGSupportRebarsDiaIndex = 0;
	m_nSOGSupportRebarsLengthType = 1;
	m_dSOGSupportRebarsRWallLengthRate = 1.0;
	m_dSOGSupportRebarsSlabLengthRate = 1.0;
	m_nSOGSupportRebarsOrderLength = 8000;
	m_nSOGSupportRebarsSpliceLength = 300;
	m_nSOGSupportRebarsCorrectiveLength = 10;

	m_bSOGOpeningInfoSet = false;
	
	m_bSOGRebarAreaInfoSet = false;
	m_nSOGRebarTolDist = 10;
	m_nSOGRebarTolLenDif = 10;
	
	m_bSOGSpliceDevConditionSet = false;
	m_nSOGSpliceDevConditionLength = 340;

	m_bSOGTopSpacerInfoSet = false;
	m_bSOGTopSpacerSet = false;
	m_nSOGTopSpacerDiaType = 0;
	m_nSOGTopSpacerSlabThick = 700;
	m_nSOGTopSpacerSlabThickOverDiaIndex = 2;
	m_nSOGTopSpacerSlabThickUnderDiaIndex = 1;
	m_nSOGTopSpacerSpacingType = 1;
	m_nSOGTopSpacerSlabSpacing = 3;
	m_nSOGTopSpacerUserSpacing1 = 1500;
	m_nSOGTopSpacerUserSpacing2 = 1500;
	m_nSOGTopSpacerHeightType = 0;
	m_nSOGTopSpacerUserHeight = 300;
	m_nSOGTopSpacerHookLenAType = 1;
	m_nSOGTopSpacerMainBarHookLenA = 100;
	m_nSOGTopSpacerUserHookLenA = 100;
	m_nSOGTopSpacerHookLenBType = 1;
	m_nSOGTopSpacerMainBarHookLenB = 0;
	m_nSOGTopSpacerUserHookLenB = 100;
	m_nSOGTopSpacerWidth = 200;

	m_bSOGBotSpacerInfoSet = false;
	m_bSOGBotSpacerSet = false;
	m_nSOGBotSpacerDiaType = 0;
	m_nSOGBotSpacerDiaIndex = 0;
	m_nSOGBotSpacerSpacingType = 1;
	m_nSOGBotSpacerSlabSpacing = 3;
	m_nSOGBotSpacerUserSpacing1 = 1500;
	m_nSOGBotSpacerUserSpacing2 = 1500;
	m_nSOGBotSpacerHeightType = 0;
	m_nSOGBotSpacerUserHeight = 300;
	m_nSOGBotSpacerHookLenAType = 1;
	m_nSOGBotSpacerMainBarHookLenA = 100;
	m_nSOGBotSpacerUserHookLenA = 100;
	m_nSOGBotSpacerHookLenBType = 1;
	m_nSOGBotSpacerMainBarHookLenB = 0;
	m_nSOGBotSpacerUserHookLenB = 100;
	m_nSOGBotSpacerWidth = 200;

	m_bSOGSpacerSupportBarsSet = false;
	m_bSOGSpacerSupportBarsConditionSet = true;
	m_nSOGSpacerSupportBarsCondition = 1000;
	m_nSOGSpacerSupportBarsDiaType = 1;
	m_nSOGSpacerSupportBarsUserDiaIndex = 0;
	m_nSOGSpacerSupportBarsNumType = 0;
	m_dSOGSpacerSupportBarsTopSpacerNums = 0.5;
	m_nSOGSpacerSupportBarsUserNums = 10;
	m_nSOGSpacerSupportBarsLengthType = 1;
	m_nSOGSpacerSupportBarsUserLength = 1600;

	m_bSOGSpliceAddLengthSet = true;
	m_nSOGSpliceAddLengthNumSplice = 3;

	
	m_nSOGCZoneSpliceWay = 0;
	m_nSOGCZoneSpliceLength = 300;

	// CircularRamp
	m_bCircularRampMainBarInfoSet = false;
	m_nCircularRampMaxDualHookLength = 2000;
	m_nCircularRampSpliceWay = 0;
	m_nCircularRampSpliceType = 0;
	
	m_bCircularRampEndInfoSet = false;
	m_nCircularRampTopEndHookType = 1;
	m_nCircularRampBotEndHookType = 3;
	m_nCircularRampIsoFootDevType = 1;
	m_nCircularRampSpliceDevSlabType = 1;

	m_nCircularRampTopEndDevTypeUserLength = 0;
	m_nCircularRampBotEndDevTypeUserLength = 0;
	m_nCircularRampOpeningTopDevType = 1;
	m_nCircularRampOpeningBotDevType = 1;

	m_bCircularRampLevelDifInfoSet = false;
	m_nCircularRampLevelDifPlaceShape = 0;
	m_nCircularRampLevelDifHaunchPlaceType = 0;
	
	m_bCircularRampOpeningInfoSet = false;
	
	m_bCircularRampRebarAreaInfoSet = false;
	m_nCircularRampRebarTolDist = 10;
	m_nCircularRampRebarTolLenDif = 10;

	m_bCircularRampSpliceDevConditionSet = false;
	m_nCircularRampSpliceDevConditionLength = 340;
	
	m_bCircularRampSpliceAddLengthSet = true;
	m_nCircularRampSpliceAddLengthNumSplice = 3;

	m_bCircularRampTopSpacerInfoSet = false;
	m_bCircularRampTopSpacerSet = false;
	m_nCircularRampTopSpacerDiaType = 0;
	m_nCircularRampTopSpacerSlabThick = 700;
	m_nCircularRampTopSpacerSlabThickOverDiaIndex = 2;
	m_nCircularRampTopSpacerSlabThickUnderDiaIndex = 1;
	m_nCircularRampTopSpacerSpacingType = 1;
	m_nCircularRampTopSpacerSlabSpacing = 3;
	m_nCircularRampTopSpacerUserSpacing1 = 1500;
	m_nCircularRampTopSpacerUserSpacing2 = 1500;
	m_nCircularRampTopSpacerHeightType = 0;
	m_nCircularRampTopSpacerUserHeight = 300;
	m_nCircularRampTopSpacerHookLenAType = 1;
	m_nCircularRampTopSpacerMainBarHookLenA = 100;
	m_nCircularRampTopSpacerUserHookLenA = 100;
	m_nCircularRampTopSpacerHookLenBType = 1;
	m_nCircularRampTopSpacerMainBarHookLenB = 0;
	m_nCircularRampTopSpacerUserHookLenB = 100;
	m_nCircularRampTopSpacerWidth = 200;

	m_bCircularRampBotSpacerInfoSet = false;
	m_bCircularRampBotSpacerSet = false;
	m_nCircularRampBotSpacerDiaType = 0;
	m_nCircularRampBotSpacerDiaIndex = 0;
	m_nCircularRampBotSpacerSpacingType = 1;
	m_nCircularRampBotSpacerSlabSpacing = 3;
	m_nCircularRampBotSpacerUserSpacing1 = 1500;
	m_nCircularRampBotSpacerUserSpacing2 = 1500;
	m_nCircularRampBotSpacerHeightType = 0;
	m_nCircularRampBotSpacerUserHeight = 300;
	m_nCircularRampBotSpacerHookLenAType = 1;
	m_nCircularRampBotSpacerMainBarHookLenA = 100;
	m_nCircularRampBotSpacerUserHookLenA = 100;
	m_nCircularRampBotSpacerHookLenBType = 1;
	m_nCircularRampBotSpacerMainBarHookLenB = 0;
	m_nCircularRampBotSpacerUserHookLenB = 100;
	m_nCircularRampBotSpacerWidth = 200;

	m_bCircularRampSpacerSupportBarsSet = false;
	m_bCircularRampSpacerSupportBarsConditionSet = true;
	m_nCircularRampSpacerSupportBarsCondition = 1000;
	m_nCircularRampSpacerSupportBarsDiaType = 1;
	m_nCircularRampSpacerSupportBarsUserDiaIndex = 0;
	m_nCircularRampSpacerSupportBarsNumType = 0;
	m_dCircularRampSpacerSupportBarsTopSpacerNums = 0.5;
	m_nCircularRampSpacerSupportBarsUserNums = 10;
	m_nCircularRampSpacerSupportBarsLengthType = 1;
	m_nCircularRampSpacerSupportBarsUserLength = 1600;

	m_nCircularRampCZoneSpliceWay = 0;
	m_nCircularRampCZoneSpliceLength = 300;

	// PC Slab
	m_bPCSlabMainBarInfoSet = false;
	m_nPCSlabMaxDualHookLength = 2000;
	m_nPCSlabSpliceWay = 0;
	m_nPCSlabSpliceType = 0;

	m_nPCSlabTopEndAddBarLxLnxType = 1;
	m_nPCSlabTopEndAddBarLengthType = 0;
	m_nPCSlabTopCenAddBarLxLnxType = 1;
	m_nPCSlabTopCenAddBarLengthType = 1;

	m_nPCSlabTopEndDevTypeUserLength = 0;
	m_nPCSlabBotEndDevTypeUserLength = 0;

	m_bPCSlabShortCenSpliceBarPlace = TRUE;
	m_nPCSlabShortCenSpliceBarLengthType = 1;
	m_bPCSlabShortEndSpliceBarPlace = TRUE;
	m_nPCSlabShortEndSpliceBarLengthType = 1;
	m_bPCSlabLongCenSpliceBarPlace = TRUE;
	m_nPCSlabLongCenSpliceBarLengthType = 1;
	m_bPCSlabLongEndSpliceBarPlace = TRUE;
	m_nPCSlabLongEndSpliceBarLengthType = 1;

	m_bPCSlabUnStructuredEndInfoSet = false;
	m_nPCSlabTopUnStructuredEndHookType = 2;
	m_nPCSlabBotUnStructuredEndHookType = 3;

	m_bPCSlabLevelDifInfoSet = false;
	m_nPCSlabLevelDifPlaceShape = 0;
	m_nPCSlabLevelDifHaunchPlaceType = 0;

	m_bPCSlabOpeningInfoSet = false;
	m_nPCSlabOpeningTopDevType = 1;
	m_nPCSlabOpeningBotDevType = 1;

	m_bPCSlabRebarAreaInfoSet = false;
	m_nPCSlabRebarTolDist = 10;
	m_nPCSlabRebarTolLenDif = 10;

	m_bPCSlabSpliceDevConditionSet = false;
	m_nPCSlabSpliceDevConditionLength = 340;

	m_bPCSlabSpliceAddLengthSet = true;
	m_nPCSlabSpliceAddLengthNumSplice = 3;

	InitUserLenMap(mm_PCSlabTopEndAddBarUserLen);
	InitUserLenMap(mm_PCSlabTopCenAddBarUserLen);
	InitUserLenMap(mm_PCSlabShortCenSpliceBarUserLen, 600);
	InitUserLenMap(mm_PCSlabShortEndSpliceBarUserLen, 600);
	InitUserLenMap(mm_PCSlabLongCenSpliceBarUserLen, 600);
	InitUserLenMap(mm_PCSlabLongEndSpliceBarUserLen, 600);


	// 슬래브 보강근
	m_bSlabRebarRebarAreaInfoSet = false;
	m_nSlabRebarRebarTolDist = 10;
	m_nSlabRebarRebarTolLenDif = 10;

	// 철근 일체형 데크 슬래브
	m_bEmbeddedDeckMainBarInfoSet = false;
	m_nEmbeddedDeckMaxDualHookLength = 2000;
	m_nEmbeddedDeckSpliceWay = 0;
	m_nEmbeddedDeckSpliceType = 0;
	
	m_bEmbeddedDeckUnStructuredEndInfoSet = false;
	m_nEmbeddedDeckTopUnStructuredEndHookType = 2;
	m_nEmbeddedDeckBotUnStructuredEndHookType = 3;

	m_bEmbeddedDeckLevelDifInfoSet = false;
	m_nEmbeddedDeckLevelDifPlaceShape = 0;
	m_nEmbeddedDeckLevelDifHaunchPlaceType = 0;

	m_bEmbeddedDeckOpeningInfoSet = false;
	m_nEmbeddedDeckOpeningTopDevType = 1;
	m_nEmbeddedDeckOpeningBotDevType = 1;

	m_nEmbeddedDeckTopEndAddBarLxLnxType = 1;
	m_nEmbeddedDeckTopEndAddBarLengthType = 0;
	m_nEmbeddedDeckTopCenAddBarLxLnxType = 1;
	m_nEmbeddedDeckTopCenAddBarLengthType = 1;

	m_bEmbeddedDeckSpliceBarPlace = TRUE;
	m_nEmbeddedDeckSpliceBarLengthType = 1;

	m_nEmbeddedDeckTopEndDevTypeUserLength = 0;
	m_nEmbeddedDeckBotEndDevTypeUserLength = 0;

	m_bEmbeddedDeckRebarAreaInfoSet = false;
	m_nEmbeddedDeckRebarTolDist = 10;
	m_nEmbeddedDeckRebarTolLenDif = 10;

	m_bEmbeddedDeckSpliceDevConditionSet = false;
	m_nEmbeddedDeckSpliceDevConditionLength = 340;

	m_bEmbeddedDeckSpliceAddLengthSet = true;
	m_nEmbeddedDeckSpliceAddLengthNumSplice = 3;

	InitUserLenMap(mm_EmbeddedDeckTopEndAddBarUserLen);
	InitUserLenMap(mm_EmbeddedDeckTopCenAddBarUserLen);
	InitUserLenMap(mm_EmbeddedDeckSpliceBarUserLen, 600);
	// 일반 데크 슬래브
	m_bGeneralDeckMainBarInfoSet = false;
	m_nGeneralDeckMaxDualHookLength = 2000;
	m_nGeneralDeckSpliceWay = 0;
	m_nGeneralDeckSpliceType = 0;
	
	m_bGeneralDeckUnStructuredEndInfoSet = false;
	m_nGeneralDeckTopUnStructuredEndHookType = 2;
	m_nGeneralDeckBotUnStructuredEndHookType = 3;

	m_bGeneralDeckLevelDifInfoSet = false;
	m_nGeneralDeckLevelDifPlaceShape = 0;
	m_nGeneralDeckLevelDifHaunchPlaceType = 0;

	m_bGeneralDeckOpeningInfoSet = false;
	m_nGeneralDeckOpeningTopDevType = 1;
	m_nGeneralDeckOpeningBotDevType = 1;

	m_nGeneralDeckTopEndAddBarLxLnxType = 1;
	m_nGeneralDeckTopEndAddBarLengthType = 0;
	m_nGeneralDeckTopCenAddBarLxLnxType = 1;
	m_nGeneralDeckTopCenAddBarLengthType = 1;

	m_bGeneralDeckSpliceBarPlace = TRUE;
	m_nGeneralDeckSpliceBarLengthType = 1;

	m_nGeneralDeckTopEndDevTypeUserLength = 0;
	m_nGeneralDeckBotEndDevTypeUserLength = 0;

	m_bGeneralDeckRebarAreaInfoSet = false;
	m_nGeneralDeckRebarTolDist = 10;
	m_nGeneralDeckRebarTolLenDif = 10;

	m_bGeneralDeckSpliceDevConditionSet = false;
	m_nGeneralDeckSpliceDevConditionLength = 340;

	m_bGeneralDeckSpliceAddLengthSet = true;
	m_nGeneralDeckSpliceAddLengthNumSplice = 3;

	InitUserLenMap(mm_GeneralDeckTopEndAddBarUserLen);
	InitUserLenMap(mm_GeneralDeckTopCenAddBarUserLen);
	InitUserLenMap(mm_GeneralDeckSpliceBarUserLen, 600);

	// PC IMT Slab
	m_bPCIMTSlabSpliceDevConditionSet = false;
	m_nPCIMTSlabSpliceDevConditionLength = 340;

	m_bPCIMTSlabSpliceAddLengthSet = true;
	m_nPCIMTSlabSpliceAddLengthNumSplice = 3;

	m_nPCIMTSlabMaxDualHookLength = 2000;
	m_nPCIMTSlabSpliceWay = 0;
	m_nPCIMTSlabSpliceType = 0;
					   
	m_nPCIMTSlabTopHookType = 2;
	m_nPCIMTSlabBotHookType = 3;
	m_nPCIMTSlabTopEndDevTypeUserLength = 0;
	m_nPCIMTSlabBotEndDevTypeUserLength = 0;
	m_nPCIMTSlabOpeningTopDevType = 1;
	m_nPCIMTSlabOpeningBotDevType = 1;

	m_nPCIMTSlabTopEndAddBarLxLnxType = 1;
	m_nPCIMTSlabTopEndAddBarLengthType = 0;
	m_nPCIMTSlabTopCenAddBarLxLnxType = 1;
	m_nPCIMTSlabTopCenAddBarLengthType = 1;

	m_bPCIMTSlabShortSpliceBarPlace = TRUE;
	m_nPCIMTSlabShortSpliceBarLengthType = 1;

	m_nPCIMTSlabRebarTolDist = 10;
	m_nPCIMTSlabRebarTolLenDif = 10;

	InitUserLenMap(mm_PCIMTSlabTopEndAddBarUserLen);
	InitUserLenMap(mm_PCIMTSlabTopCenAddBarUserLen);
	InitUserLenMap(mm_PCIMTSlabShortSpliceBarUserLen, 600);
	//SK 템플릿 추가
	m_dMatSpacerSupportBarsUserRatio = 1.4;
	m_dFlatSpacerSupportBarsUserRatio = 1.4;
	m_dSlabSpacerSupportBarsUserRatio = 1.4;
	m_dSOGSpacerSupportBarsUserRatio = 1.4;
	m_dCircularRampSpacerSupportBarsUserRatio = 1.4;

	m_bPCIMTSlabLongSpliceBarPlace = TRUE;
	m_nPCIMTSlabLongSpliceBarType = 0;

	//HCS
	m_bPCHCSSlabSpliceDevConditionSet = false;
	m_nPCHCSSlabSpliceDevConditionLength = 340;
	m_bPCHCSSlabSpliceAddLengthSet = true;
	m_nPCHCSSlabSpliceAddLengthNumSplice = 3;
	m_nPCHCSSlabMaxDualHookLength = 2000;
	m_nPCHCSSlabSpliceWay = 0;
	m_nPCHCSSlabSpliceType = 0;
	
	m_nPCHCSSlabTopHookType = 2;
	m_nPCHCSSlabBotHookType = 3;
	m_nPCHCSSlabTopEndDevTypeUserLength = 0;
	m_nPCHCSSlabBotEndDevTypeUserLength = 0;
	m_nPCHCSSlabOpeningTopDevType = 1;
	m_nPCHCSSlabOpeningBotDevType = 1;
	
	m_nPCHCSSlabTopEndAddBarLxLnxType = 1;
	m_nPCHCSSlabTopEndAddBarLengthType = 0;
	m_nPCHCSSlabTopCenAddBarLxLnxType = 1;
	m_nPCHCSSlabTopCenAddBarLengthType = 1;
	m_nPCHCSSlabDevSubBarLengthType = 0;
	m_nPCHCSSlabShortCenSpliceBarLengthType = 1;
	m_nPCHCSSlabShortEndSpliceBarLengthType = 1;
	m_nPCHCSSlabLongCenSpliceBarLengthType = 1;
	m_nPCHCSSlabLongEndSpliceBarLengthType = 1;

	m_nPCHCSSlabRebarTolDist = 10;
	m_nPCHCSSlabRebarTolLenDif = 10;

	m_bPCHCSSlabShortCenSpliceBarPlace = TRUE;
	m_bPCHCSSlabShortEndSpliceBarPlace = TRUE;
	m_bPCHCSSlabLongCenSpliceBarPlace = TRUE;
	m_bPCHCSSlabLongEndSpliceBarPlace = TRUE;

	InitUserLenMap(mm_PCHCSSlabTopEndAddBarUserLen);
	InitUserLenMap(mm_PCHCSSlabTopCenAddBarUserLen);
	InitUserLenMap(mm_PCHCSSlabDevSubBarUserLen);
	InitUserLenMap(mm_PCHCSSlabShortCenSpliceBarUserLen, 600);
	InitUserLenMap(mm_PCHCSSlabShortEndSpliceBarUserLen, 600);
	InitUserLenMap(mm_PCHCSSlabLongCenSpliceBarUserLen, 600);
	InitUserLenMap(mm_PCHCSSlabLongEndSpliceBarUserLen, 600);

	//RPS
	m_bPCRPSSlabSpliceDevConditionSet = false;
	m_nPCRPSSlabSpliceDevConditionLength = 340;
	m_bPCRPSSlabSpliceAddLengthSet = true;
	m_nPCRPSSlabSpliceAddLengthNumSplice = 3;
	m_nPCRPSSlabMaxDualHookLength = 2000;
	m_nPCRPSSlabSpliceWay = 0;
	m_nPCRPSSlabSpliceType = 0;
	
	m_nPCRPSSlabTopHookType = 2;
	m_nPCRPSSlabBotHookType = 3;
	m_nPCRPSSlabTopEndDevTypeUserLength = 0;
	m_nPCRPSSlabBotEndDevTypeUserLength = 0;
	m_nPCRPSSlabOpeningTopDevType = 1;
	m_nPCRPSSlabOpeningBotDevType = 1;
	
	m_nPCRPSSlabTopEndAddBarLxLnxType = 1;
	m_nPCRPSSlabTopEndAddBarLengthType = 0;
	m_nPCRPSSlabTopCenAddBarLxLnxType = 1;
	m_nPCRPSSlabTopCenAddBarLengthType = 1;
	m_nPCRPSSlabDevSubBarLengthType = 0;
	m_nPCRPSSlabLongCenSpliceBarLengthType = 1;
	m_nPCRPSSlabLongEndSpliceBarLengthType = 1;

	m_nPCRPSSlabRebarTolDist = 10;
	m_nPCRPSSlabRebarTolLenDif = 10;

	m_bPCRPSSlabLongCenSpliceBarPlace = TRUE;
	m_bPCRPSSlabLongEndSpliceBarPlace = TRUE;

	InitUserLenMap(mm_PCRPSSlabTopEndAddBarUserLen);
	InitUserLenMap(mm_PCRPSSlabTopCenAddBarUserLen);
	InitUserLenMap(mm_PCRPSSlabDevSubBarUserLen);	
	InitUserLenMap(mm_PCRPSSlabLongCenSpliceBarUserLen, 600);
	InitUserLenMap(mm_PCRPSSlabLongEndSpliceBarUserLen, 600);

	m_bMatBotSpacerUserSteelGradeS = FALSE;
	m_bMatTopSpacerUserSteelGradeS = FALSE;
	m_bMatSpacerSupportBarUserSteelGradeS = FALSE;
	m_bMatSupportRebarUserSteelGradeS = FALSE;
	m_bFlatBotSpacerUserSteelGradeS = FALSE;
	m_bFlatTopSpacerUserSteelGradeS = FALSE;
	m_bFlatSpacerSupportBarUserSteelGradeS = FALSE;
	m_bSlabBotSpacerUserSteelGradeS = FALSE;
	m_bSlabTopSpacerUserSteelGradeS = FALSE;
	m_bSlabSpacerSupportBarUserSteelGradeS = FALSE;
	m_bSOGBotSpacerUserSteelGradeS = FALSE;
	m_bSOGTopSpacerUserSteelGradeS = FALSE;
	m_bSOGSpacerSupportBarUserSteelGradeS = FALSE;
	m_bSOGSupportRebarUserSteelGradeS = FALSE;
	m_bCircularRampBotSpacerUserSteelGradeS = FALSE;
	m_bCircularRampTopSpacerUserSteelGradeS = FALSE;
	m_bCircularRampSpacerSupportBarUserSteelGradeS = FALSE;

	m_nEmbeddedDeckCZoneSpliceWay = 0;
	m_nEmbeddedDeckCZoneSpliceLength = 300;
	m_nGeneralDeckCZoneSpliceWay = 0;
	m_nGeneralDeckCZoneSpliceLength = 300;
	m_nPCSlabCZoneSpliceWay = 0;
	m_nPCSlabCZoneSpliceLength = 300;
	m_nPCIMTSlabCZoneSpliceWay = 0;
	m_nPCIMTSlabCZoneSpliceLength = 300;
	m_nPCHCSSlabCZoneSpliceWay = 0;
	m_nPCHCSSlabCZoneSpliceLength = 300;
	m_nPCRPSSlabCZoneSpliceWay = 0;
	m_nPCRPSSlabCZoneSpliceLength = 300;

	m_bMatTopSpacerPlaceConditionSet = FALSE;
	m_nMatTopSpacerPlaceCondition = 0;
	m_bFlatTopSpacerPlaceConditionSet = FALSE;
	m_nFlatTopSpacerPlaceCondition = 0;
	m_bSlabTopSpacerPlaceConditionSet = FALSE;
	m_nSlabTopSpacerPlaceCondition = 0;
	m_bSOGTopSpacerPlaceConditionSet = FALSE;
	m_nSOGTopSpacerPlaceCondition = 0;
	m_bCircularRampTopSpacerPlaceConditionSet = FALSE;
	m_nCircularRampTopSpacerPlaceCondition = 0;

	m_bSlabOpeningAddBarDeductionCondition = FALSE;
	m_nSlabOpeningAddBarDeductionPercent = 80;


	mm_MatThickToDiaIndex.insert(make_pair(_T("700"), 0));
	mm_MatThickToDiaIndex.insert(make_pair(_T("Default"), 1));

	mm_FlatThickToDiaIndex = mm_MatThickToDiaIndex;
	mm_SlabThickToDiaIndex = mm_MatThickToDiaIndex;
	mm_SOGThickToDiaIndex = mm_MatThickToDiaIndex;
	mm_CircularRampThickToDiaIndex = mm_MatThickToDiaIndex;

	m_bSlabLongEndPlaceAreaSet = FALSE;
	m_nSlabLongEndPlaceAreaOneWaySlabType = 2;
	m_nSlabLongEndPlaceAreaTwoWaySlabType = 1;
	m_bSlabBotAddBarLengthSet = FALSE;
	m_nSlabBotAddBarLengthType = 0;	
	m_bSlabBotAddBarCutBarLenExclusionSet = FALSE;

	m_nSlabOpeningAddBarDevLengthType = 0;
}
SlabHorSupportRebarsTemplate *SlabRebarTemplate::GetPCSlabHorSupportRebarsTemplate()
{
	return m_pPCSlabHorSupportRebarsTemplate;
}
SlabHorSupportRebarsTemplate *SlabRebarTemplate::GetPCIMTSlabHorSupportRebarsTemplate()
{
	return m_pPCIMTSlabHorSupportRebarsTemplate;
}
SlabHorSupportRebarsTemplate *SlabRebarTemplate::GetPCHCSSlabHorSupportRebarsTemplate()
{
	return m_pPCHCSSlabHorSupportRebarsTemplate;
}
SlabHorSupportRebarsTemplate *SlabRebarTemplate::GetPCRPSSlabHorSupportRebarsTemplate()
{
	return m_pPCRPSSlabHorSupportRebarsTemplate;
}
SlabHorSupportRebarsTemplate *SlabRebarTemplate::GetGeneralDeckHorSupportRebarsTemplate()
{
	return m_pGeneralDeckHorSupportRebarsTemplate;
}
SlabHorSupportRebarsTemplate *SlabRebarTemplate::GetEmbeddedDeckHorSupportRebarsTemplate()
{
	return m_pEmbeddedDeckHorSupportRebarsTemplate;
}
ShearFrictionBarTemplate *SlabRebarTemplate::GetMatShearFrictionBarTemplate()
{
	return m_pMatShearFrictionBarTemplate;
}
ShearFrictionBarTemplate *SlabRebarTemplate::GetFlatShearFrictionBarTemplate()
{
	return m_pFlatShearFrictionBarTemplate;
}
long SlabRebarTemplate::GetPCHCSSlabTopEndAddBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCHCSSlabTopEndAddBarUserLen.find(DiaIndex);
	if (it == mm_PCHCSSlabTopEndAddBarUserLen.end())
		return 0;
	return it->second;
}
long SlabRebarTemplate::GetPCHCSSlabTopCenAddBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCHCSSlabTopCenAddBarUserLen.find(DiaIndex);
	if (it == mm_PCHCSSlabTopCenAddBarUserLen.end())
		return 0;
	return it->second;
}
long SlabRebarTemplate::GetPCHCSSlabDevSubBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCHCSSlabDevSubBarUserLen.find(DiaIndex);
	if (it == mm_PCHCSSlabDevSubBarUserLen.end())
		return 0;
	return it->second;
}
long SlabRebarTemplate::GetPCHCSSlabShortCenSpliceBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCHCSSlabShortCenSpliceBarUserLen.find(DiaIndex);
	if (it == mm_PCHCSSlabShortCenSpliceBarUserLen.end())
		return 0;
	return it->second;
}
long SlabRebarTemplate::GetPCHCSSlabShortEndSpliceBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCHCSSlabShortEndSpliceBarUserLen.find(DiaIndex);
	if (it == mm_PCHCSSlabShortEndSpliceBarUserLen.end())
		return 0;
	return it->second;
}
long SlabRebarTemplate::GetPCHCSSlabLongCenSpliceBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCHCSSlabLongCenSpliceBarUserLen.find(DiaIndex);
	if (it == mm_PCHCSSlabLongCenSpliceBarUserLen.end())
		return 0;
	return it->second;
}
long SlabRebarTemplate::GetPCHCSSlabLongEndSpliceBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCHCSSlabLongEndSpliceBarUserLen.find(DiaIndex);
	if (it == mm_PCHCSSlabLongEndSpliceBarUserLen.end())
		return 0;
	return it->second;
}
long SlabRebarTemplate::GetPCRPSSlabTopEndAddBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCRPSSlabTopEndAddBarUserLen.find(DiaIndex);
	if (it == mm_PCRPSSlabTopEndAddBarUserLen.end())
		return 0;
	return it->second;
}
long SlabRebarTemplate::GetPCRPSSlabTopCenAddBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCRPSSlabTopCenAddBarUserLen.find(DiaIndex);
	if (it == mm_PCRPSSlabTopCenAddBarUserLen.end())
		return 0;
	return it->second;
}
long SlabRebarTemplate::GetPCRPSSlabDevSubBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCRPSSlabDevSubBarUserLen.find(DiaIndex);
	if (it == mm_PCRPSSlabDevSubBarUserLen.end())
		return 0;
	return it->second;
}
long SlabRebarTemplate::GetPCRPSSlabLongCenSpliceBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCRPSSlabLongCenSpliceBarUserLen.find(DiaIndex);
	if (it == mm_PCRPSSlabLongCenSpliceBarUserLen.end())
		return 0;
	return it->second;
}
long SlabRebarTemplate::GetPCRPSSlabLongEndSpliceBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCRPSSlabLongEndSpliceBarUserLen.find(DiaIndex);
	if (it == mm_PCRPSSlabLongEndSpliceBarUserLen.end())
		return 0;
	return it->second;
}

void SlabRebarTemplate::SetPCHCSSlabTopEndAddBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCHCSSlabTopEndAddBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCHCSSlabTopEndAddBarUserLen.insert(make_pair(it->first, it->second));
	}
}
void SlabRebarTemplate::SetPCHCSSlabTopCenAddBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCHCSSlabTopCenAddBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCHCSSlabTopCenAddBarUserLen.insert(make_pair(it->first, it->second));
	}
}
void SlabRebarTemplate::SetPCHCSSlabDevSubBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCHCSSlabDevSubBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCHCSSlabDevSubBarUserLen.insert(make_pair(it->first, it->second));
	}
}

void SlabRebarTemplate::SetPCHCSSlabShortCenSpliceBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCHCSSlabShortCenSpliceBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCHCSSlabShortCenSpliceBarUserLen.insert(make_pair(it->first, it->second));
	}
}
void SlabRebarTemplate::SetPCHCSSlabShortEndSpliceBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCHCSSlabShortEndSpliceBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCHCSSlabShortEndSpliceBarUserLen.insert(make_pair(it->first, it->second));
	}
}
void SlabRebarTemplate::SetPCHCSSlabLongCenSpliceBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCHCSSlabLongCenSpliceBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCHCSSlabLongCenSpliceBarUserLen.insert(make_pair(it->first, it->second));
	}
}
void SlabRebarTemplate::SetPCHCSSlabLongEndSpliceBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCHCSSlabLongEndSpliceBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCHCSSlabLongEndSpliceBarUserLen.insert(make_pair(it->first, it->second));
	}
}
void SlabRebarTemplate::SetPCRPSSlabTopEndAddBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCRPSSlabTopEndAddBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCRPSSlabTopEndAddBarUserLen.insert(make_pair(it->first, it->second));
	}
}
void SlabRebarTemplate::SetPCRPSSlabTopCenAddBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCRPSSlabTopCenAddBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCRPSSlabTopCenAddBarUserLen.insert(make_pair(it->first, it->second));
	}
}
void SlabRebarTemplate::SetPCRPSSlabDevSubBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCRPSSlabDevSubBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCRPSSlabDevSubBarUserLen.insert(make_pair(it->first, it->second));
	}
}

void SlabRebarTemplate::SetPCRPSSlabLongCenSpliceBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCRPSSlabLongCenSpliceBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCRPSSlabLongCenSpliceBarUserLen.insert(make_pair(it->first, it->second));
	}
}
void SlabRebarTemplate::SetPCRPSSlabLongEndSpliceBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCRPSSlabLongEndSpliceBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCRPSSlabLongEndSpliceBarUserLen.insert(make_pair(it->first, it->second));
	}
}
void SlabRebarTemplate::SetGeneralDeckTopEndAddBarUserLenMap(map<long, long> mapUserLen)
{
	mm_GeneralDeckTopEndAddBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_GeneralDeckTopEndAddBarUserLen.insert(make_pair(it->first, it->second));
	}
}
long SlabRebarTemplate::GetGeneralDeckTopEndAddBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_GeneralDeckTopEndAddBarUserLen.find(DiaIndex);
	if (it == mm_GeneralDeckTopEndAddBarUserLen.end())
		return 0;
	return it->second;
}
void SlabRebarTemplate::SetGeneralDeckTopCenAddBarUserLenMap(map<long, long> mapUserLen)
{
	mm_GeneralDeckTopCenAddBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_GeneralDeckTopCenAddBarUserLen.insert(make_pair(it->first, it->second));
	}
}
long SlabRebarTemplate::GetGeneralDeckTopCenAddBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_GeneralDeckTopCenAddBarUserLen.find(DiaIndex);
	if (it == mm_GeneralDeckTopCenAddBarUserLen.end())
		return 0;
	return it->second;
}
void SlabRebarTemplate::SetGeneralDeckSpliceBarUserLenMap(map<long, long> mapUserLen)
{
	mm_GeneralDeckSpliceBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_GeneralDeckSpliceBarUserLen.insert(make_pair(it->first, it->second));
	}
}
long SlabRebarTemplate::GetGeneralDeckSpliceBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_GeneralDeckSpliceBarUserLen.find(DiaIndex);
	if (it == mm_GeneralDeckSpliceBarUserLen.end())
		return 0;
	return it->second;
}


void SlabRebarTemplate::SetEmbeddedDeckTopEndAddBarUserLenMap(map<long, long> mapUserLen)
{
	mm_EmbeddedDeckTopEndAddBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_EmbeddedDeckTopEndAddBarUserLen.insert(make_pair(it->first, it->second));
	}
}
long SlabRebarTemplate::GetEmbeddedDeckTopEndAddBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_EmbeddedDeckTopEndAddBarUserLen.find(DiaIndex);
	if (it == mm_EmbeddedDeckTopEndAddBarUserLen.end())
		return 0;
	return it->second;
}
void SlabRebarTemplate::SetEmbeddedDeckTopCenAddBarUserLenMap(map<long, long> mapUserLen)
{
	mm_EmbeddedDeckTopCenAddBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_EmbeddedDeckTopCenAddBarUserLen.insert(make_pair(it->first, it->second));
	}
}
long SlabRebarTemplate::GetEmbeddedDeckTopCenAddBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_EmbeddedDeckTopCenAddBarUserLen.find(DiaIndex);
	if (it == mm_EmbeddedDeckTopCenAddBarUserLen.end())
		return 0;
	return it->second;
}
void SlabRebarTemplate::SetEmbeddedDeckSpliceBarUserLenMap(map<long, long> mapUserLen)
{
	mm_EmbeddedDeckSpliceBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_EmbeddedDeckSpliceBarUserLen.insert(make_pair(it->first, it->second));
	}
}
long SlabRebarTemplate::GetEmbeddedDeckSpliceBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_EmbeddedDeckSpliceBarUserLen.find(DiaIndex);
	if (it == mm_EmbeddedDeckSpliceBarUserLen.end())
		return 0;
	return it->second;
}


void SlabRebarTemplate::SetPCSlabTopEndAddBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCSlabTopEndAddBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCSlabTopEndAddBarUserLen.insert(make_pair(it->first, it->second));
	}
}
long SlabRebarTemplate::GetPCSlabTopEndAddBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCSlabTopEndAddBarUserLen.find(DiaIndex);
	if (it == mm_PCSlabTopEndAddBarUserLen.end())
		return 0;
	return it->second;
}
void SlabRebarTemplate::SetPCSlabTopCenAddBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCSlabTopCenAddBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCSlabTopCenAddBarUserLen.insert(make_pair(it->first, it->second));
	}
}
long SlabRebarTemplate::GetPCSlabTopCenAddBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCSlabTopCenAddBarUserLen.find(DiaIndex);
	if (it == mm_PCSlabTopCenAddBarUserLen.end())
		return 0;
	return it->second;
}
void SlabRebarTemplate::SetPCSlabShortCenSpliceBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCSlabShortCenSpliceBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCSlabShortCenSpliceBarUserLen.insert(make_pair(it->first, it->second));
	}
}
long SlabRebarTemplate::GetPCSlabShortCenSpliceBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCSlabShortCenSpliceBarUserLen.find(DiaIndex);
	if (it == mm_PCSlabShortCenSpliceBarUserLen.end())
		return 0;
	return it->second;
}
void SlabRebarTemplate::SetPCSlabShortEndSpliceBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCSlabShortEndSpliceBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCSlabShortEndSpliceBarUserLen.insert(make_pair(it->first, it->second));
	}
}
long SlabRebarTemplate::GetPCSlabShortEndSpliceBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCSlabShortEndSpliceBarUserLen.find(DiaIndex);
	if (it == mm_PCSlabShortEndSpliceBarUserLen.end())
		return 0;
	return it->second;
}

void SlabRebarTemplate::SetPCSlabLongCenSpliceBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCSlabLongCenSpliceBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCSlabLongCenSpliceBarUserLen.insert(make_pair(it->first, it->second));
	}
}
long SlabRebarTemplate::GetPCSlabLongCenSpliceBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCSlabLongCenSpliceBarUserLen.find(DiaIndex);
	if (it == mm_PCSlabLongCenSpliceBarUserLen.end())
		return 0;
	return it->second;
}
void SlabRebarTemplate::SetPCSlabLongEndSpliceBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCSlabLongEndSpliceBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCSlabLongEndSpliceBarUserLen.insert(make_pair(it->first, it->second));
	}
}
long SlabRebarTemplate::GetPCSlabLongEndSpliceBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCSlabLongEndSpliceBarUserLen.find(DiaIndex);
	if (it == mm_PCSlabLongEndSpliceBarUserLen.end())
		return 0;
	return it->second;
}

void SlabRebarTemplate::SetPCIMTSlabTopEndAddBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCIMTSlabTopEndAddBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCIMTSlabTopEndAddBarUserLen.insert(make_pair(it->first, it->second));
	}
}
long SlabRebarTemplate::GetPCIMTSlabTopEndAddBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCIMTSlabTopEndAddBarUserLen.find(DiaIndex);
	if (it == mm_PCIMTSlabTopEndAddBarUserLen.end())
		return 0;
	return it->second;
}
void SlabRebarTemplate::SetPCIMTSlabTopCenAddBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCIMTSlabTopCenAddBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCIMTSlabTopCenAddBarUserLen.insert(make_pair(it->first, it->second));
	}
}
long SlabRebarTemplate::GetPCIMTSlabTopCenAddBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCIMTSlabTopCenAddBarUserLen.find(DiaIndex);
	if (it == mm_PCIMTSlabTopCenAddBarUserLen.end())
		return 0;
	return it->second;
}
void SlabRebarTemplate::SetPCIMTSlabShortSpliceBarUserLenMap(map<long, long> mapUserLen)
{
	mm_PCIMTSlabShortSpliceBarUserLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserLen.begin(); it != mapUserLen.end(); it++)
	{
		mm_PCIMTSlabShortSpliceBarUserLen.insert(make_pair(it->first, it->second));
	}
}
long SlabRebarTemplate::GetPCIMTSlabShortSpliceBarUserLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);
	map<long, long>::iterator it = mm_PCIMTSlabShortSpliceBarUserLen.find(DiaIndex);
	if (it == mm_PCIMTSlabShortSpliceBarUserLen.end())
		return 0;
	return it->second;
}

void SlabRebarTemplate::SetMatThickToDiaIndex(map<CString, long> value)
{
	mm_MatThickToDiaIndex.clear();	
	mm_MatThickToDiaIndex.insert(value.begin(), value.end());
}
void SlabRebarTemplate::SetFlatThickToDiaIndex(map<CString, long> value)
{
	mm_FlatThickToDiaIndex.clear();	
	mm_FlatThickToDiaIndex.insert(value.begin(), value.end());
}
void SlabRebarTemplate::SetSlabThickToDiaIndex(map<CString, long> value)
{
	mm_SlabThickToDiaIndex.clear();	
	mm_SlabThickToDiaIndex.insert(value.begin(), value.end());
}
void SlabRebarTemplate::SetSOGThickToDiaIndex(map<CString, long> value)
{
	mm_SOGThickToDiaIndex.clear();	
	mm_SOGThickToDiaIndex.insert(value.begin(), value.end());
}
void SlabRebarTemplate::SetCircularRampThickToDiaIndex(map<CString, long> value)
{
	mm_CircularRampThickToDiaIndex.clear();	
	mm_CircularRampThickToDiaIndex.insert(value.begin(), value.end());
}
