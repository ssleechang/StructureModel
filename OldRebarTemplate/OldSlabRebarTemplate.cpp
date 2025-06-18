#include "StdAfx.h"
#include "OldSlabRebarTemplate.h"
#include "..\MSDeformedBar.h"
#include "..\..\GMLib\MSVersionInfo.h"

IMPLEMENT_SERIAL(OldSlabRebarTemplate, OldBaseRebarTemplate, VERSIONABLE_SCHEMA | 1)
OldSlabRebarTemplate::OldSlabRebarTemplate() :OldBaseRebarTemplate()
{
	DataInit();
}

OldSlabRebarTemplate::~OldSlabRebarTemplate(void)
{
}

void OldSlabRebarTemplate::CopyFromMe(OldSlabRebarTemplate* pSlabRebarTemplate)
{

}

void OldSlabRebarTemplate::Dump(EFS::EFS_Buffer& buffer)
{
	// MAT Slab
	buffer << m_bMatLongMainBarInfoSet;
	buffer << m_nMatLongMaxDualHookLength;
	buffer << m_nMatLongSpliceWay;
	buffer << m_nMatLongSpliceType;
	buffer << m_bMatLongAlterSpliceSet;
	buffer << m_nMatLongAlterSpliceValue1;

	buffer << m_bMatShortMainBarInfoSet;
	buffer << m_nMatShortMaxDualHookLength;
	buffer << m_nMatShortSpliceWay;
	buffer << m_nMatShortSpliceType;
	buffer << m_bMatShortAlterSpliceSet;
	buffer << m_nMatShortAlterSpliceValue1;

	buffer << m_bMatEndInfoSet;
	buffer << m_nMatTopEndHookType;
	buffer << m_nMatBotEndHookType;
	buffer << m_nMatIsoFootDevType;

	buffer << m_bMatLevelDifInfoSet;
	buffer << m_nMatLevelDifPlaceShape;
	buffer << m_nMatLevelDifHaunchPlaceType;

	buffer << m_bMatRebarAreaInfoSet;
	buffer << m_nMatRebarTolDist;
	buffer << m_nMatRebarTolLenDif;

	buffer << m_bMatTopSpacerInfoSet;
	buffer << m_bMatTopSpacerSet;
	buffer << m_nMatTopSpacerDiaType;
	buffer << m_nMatTopSpacerDiaIndex;
	buffer << m_nMatTopSpacerSpacingType;
	buffer << m_nMatTopSpacerSlabSpacing;
	buffer << m_nMatTopSpacerUserSpacing1;
	buffer << m_nMatTopSpacerUserSpacing2;
	buffer << m_nMatTopSpacerHeightType;
	buffer << m_nMatTopSpacerUserHeight;
	buffer << m_nMatTopSpacerHookLenAType;
	buffer << m_nMatTopSpacerMainBarHookLenA;
	buffer << m_nMatTopSpacerUserHookLenA;
	buffer << m_nMatTopSpacerHookLenBType;
	buffer << m_nMatTopSpacerMainBarHookLenB;
	buffer << m_nMatTopSpacerUserHookLenB;
	buffer << m_nMatTopSpacerWidth;

	buffer << m_bMatBotSpacerInfoSet;
	buffer << m_bMatBotSpacerSet;
	buffer << m_nMatBotSpacerDiaType;
	buffer << m_nMatBotSpacerDiaIndex;
	buffer << m_nMatBotSpacerSpacingType;
	buffer << m_nMatBotSpacerSlabSpacing;
	buffer << m_nMatBotSpacerUserSpacing1;
	buffer << m_nMatBotSpacerUserSpacing2;
	buffer << m_nMatBotSpacerHeightType;
	buffer << m_nMatBotSpacerUserHeight;
	buffer << m_nMatBotSpacerHookLenAType;
	buffer << m_nMatBotSpacerMainBarHookLenA;
	buffer << m_nMatBotSpacerUserHookLenA;
	buffer << m_nMatBotSpacerHookLenBType;
	buffer << m_nMatBotSpacerMainBarHookLenB;
	buffer << m_nMatBotSpacerUserHookLenB;
	buffer << m_nMatBotSpacerWidth;

	// Flat Slab
	buffer << m_bFlatLongMainBarInfoSet;
	buffer << m_nFlatLongMaxDualHookLength;
	buffer << m_nFlatLongSpliceWay;
	buffer << m_nFlatLongSpliceType;
	buffer << m_bFlatLongAlterSpliceSet;
	buffer << m_nFlatLongAlterSpliceValue1;

	buffer << m_bFlatShortMainBarInfoSet;
	buffer << m_nFlatShortMaxDualHookLength;
	buffer << m_nFlatShortSpliceWay;
	buffer << m_nFlatShortSpliceType;
	buffer << m_bFlatShortAlterSpliceSet;
	buffer << m_nFlatShortAlterSpliceValue1;

	buffer << m_bFlatEndInfoSet;
	buffer << m_nFlatTopEndHookType;
	buffer << m_nFlatBotEndHookType;
	buffer << m_nFlatIsoFootDevType;
	buffer << m_nFlatSpliceDevSlabType;

	buffer << m_bFlatLevelDifInfoSet;
	buffer << m_nFlatLevelDifPlaceShape;
	buffer << m_nFlatLevelDifHaunchPlaceType;

	buffer << m_bFlatOpeningInfoSet;
	buffer << m_nFlatOpeningTopHookType;
	buffer << m_nFlatOpeningBotHookType;

	buffer << m_bFlatRebarAreaInfoSet;
	buffer << m_nFlatRebarTolDist;
	buffer << m_nFlatRebarTolLenDif;

	buffer << m_bFlatTopSpacerInfoSet;
	buffer << m_bFlatTopSpacerSet;
	buffer << m_nFlatTopSpacerDiaType;
	buffer << m_nFlatTopSpacerDiaIndex;
	buffer << m_nFlatTopSpacerSpacingType;
	buffer << m_nFlatTopSpacerSlabSpacing;
	buffer << m_nFlatTopSpacerUserSpacing1;
	buffer << m_nFlatTopSpacerUserSpacing2;
	buffer << m_nFlatTopSpacerHeightType;
	buffer << m_nFlatTopSpacerUserHeight;
	buffer << m_nFlatTopSpacerHookLenAType;
	buffer << m_nFlatTopSpacerMainBarHookLenA;
	buffer << m_nFlatTopSpacerUserHookLenA;
	buffer << m_nFlatTopSpacerHookLenBType;
	buffer << m_nFlatTopSpacerMainBarHookLenB;
	buffer << m_nFlatTopSpacerUserHookLenB;
	buffer << m_nFlatTopSpacerWidth;

	buffer << m_bFlatBotSpacerInfoSet;
	buffer << m_bFlatBotSpacerSet;
	buffer << m_nFlatBotSpacerDiaType;
	buffer << m_nFlatBotSpacerDiaIndex;
	buffer << m_nFlatBotSpacerSpacingType;
	buffer << m_nFlatBotSpacerSlabSpacing;
	buffer << m_nFlatBotSpacerUserSpacing1;
	buffer << m_nFlatBotSpacerUserSpacing2;
	buffer << m_nFlatBotSpacerHeightType;
	buffer << m_nFlatBotSpacerUserHeight;
	buffer << m_nFlatBotSpacerHookLenAType;
	buffer << m_nFlatBotSpacerMainBarHookLenA;
	buffer << m_nFlatBotSpacerUserHookLenA;
	buffer << m_nFlatBotSpacerHookLenBType;
	buffer << m_nFlatBotSpacerMainBarHookLenB;
	buffer << m_nFlatBotSpacerUserHookLenB;
	buffer << m_nFlatBotSpacerWidth;

	// 사변지지 SLAB
	buffer << m_bSlabLongMainBarInfoSet;
	buffer << m_nSlabLongMaxDualHookLength;
	buffer << m_nSlabLongSpliceWay;
	buffer << m_nSlabLongSpliceType;
	buffer << m_bSlabLongAlterSpliceSet;
	buffer << m_nSlabLongAlterSpliceValue1;

	buffer << m_bSlabShortMainBarInfoSet;
	buffer << m_nSlabShortMaxDualHookLength;
	buffer << m_nSlabShortSpliceWay;
	buffer << m_nSlabShortSpliceType;
	buffer << m_bSlabShortAlterSpliceSet;
	buffer << m_nSlabShortAlterSpliceValue1;

	buffer << m_bSlabStructuredEndInfoSet;
	buffer << m_nSlabTopStructuredEndHookType;
	buffer << m_nSlabBotStructuredEndHookType;

	buffer << m_bSlabTopCutBarLenInfoSet;
	buffer << m_nSlabOneWayCutBarLenType;
	buffer << m_nSlabTwoWayCutBarLenType;
	
	buffer << m_bSlabUnStructuredEndInfoSet;
	buffer << m_nSlabTopUnStructuredEndHookType;
	buffer << m_nSlabBotUnStructuredEndHookType;
	buffer << m_nSlabIsoFootUnStructuredDevType;

	buffer << m_bSlabLevelDifInfoSet;
	buffer << m_nSlabLevelDifPlaceShape;
	buffer << m_nSlabLevelDifHaunchPlaceType;

	buffer << m_bSlabOpeningInfoSet;
	buffer << m_nSlabOpeningTopHookType;
	buffer << m_nSlabOpeningBotHookType;

	buffer << m_bSlabRebarAreaInfoSet;
	buffer << m_nSlabRebarTolDist;
	buffer << m_nSlabRebarTolLenDif;

	buffer << m_bSlabTopSpacerInfoSet;
	buffer << m_bSlabTopSpacerSet;
	buffer << m_nSlabTopSpacerDiaType;
	buffer << m_nSlabTopSpacerDiaIndex;
	buffer << m_nSlabTopSpacerSpacingType;
	buffer << m_nSlabTopSpacerSlabSpacing;
	buffer << m_nSlabTopSpacerUserSpacing1;
	buffer << m_nSlabTopSpacerUserSpacing2;
	buffer << m_nSlabTopSpacerHeightType;
	buffer << m_nSlabTopSpacerUserHeight;
	buffer << m_nSlabTopSpacerHookLenAType;
	buffer << m_nSlabTopSpacerMainBarHookLenA;
	buffer << m_nSlabTopSpacerUserHookLenA;
	buffer << m_nSlabTopSpacerHookLenBType;
	buffer << m_nSlabTopSpacerMainBarHookLenB;
	buffer << m_nSlabTopSpacerUserHookLenB;
	buffer << m_nSlabTopSpacerWidth;

	buffer << m_bSlabBotSpacerInfoSet;
	buffer << m_bSlabBotSpacerSet;
	buffer << m_nSlabBotSpacerDiaType;
	buffer << m_nSlabBotSpacerDiaIndex;
	buffer << m_nSlabBotSpacerSpacingType;
	buffer << m_nSlabBotSpacerSlabSpacing;
	buffer << m_nSlabBotSpacerUserSpacing1;
	buffer << m_nSlabBotSpacerUserSpacing2;
	buffer << m_nSlabBotSpacerHeightType;
	buffer << m_nSlabBotSpacerUserHeight;
	buffer << m_nSlabBotSpacerHookLenAType;
	buffer << m_nSlabBotSpacerMainBarHookLenA;
	buffer << m_nSlabBotSpacerUserHookLenA;
	buffer << m_nSlabBotSpacerHookLenBType;
	buffer << m_nSlabBotSpacerMainBarHookLenB;
	buffer << m_nSlabBotSpacerUserHookLenB;
	buffer << m_nSlabBotSpacerWidth;

	// SOG Slab
	buffer << m_bSOGLongMainBarInfoSet;
	buffer << m_nSOGLongMaxDualHookLength;
	buffer << m_nSOGLongSpliceWay;
	buffer << m_nSOGLongSpliceType;
	buffer << m_bSOGLongAlterSpliceSet;
	buffer << m_nSOGLongAlterSpliceValue1;

	buffer << m_bSOGShortMainBarInfoSet;
	buffer << m_nSOGShortMaxDualHookLength;
	buffer << m_nSOGShortSpliceWay;
	buffer << m_nSOGShortSpliceType;
	buffer << m_bSOGShortAlterSpliceSet;
	buffer << m_nSOGShortAlterSpliceValue1;

	buffer << m_bSOGEndInfoSet;
	buffer << m_nSOGTopEndHookType;
	buffer << m_nSOGBotEndHookType;
	buffer << m_nSOGIsoFootDevType;
	buffer << m_nSOGSpliceDevSlabType;

	buffer << m_bSOGLevelDifInfoSet;
	buffer << m_nSOGLevelDifPlaceShape;
	buffer << m_nSOGLevelDifHaunchPlaceType;

	buffer << m_bSOGOpeningInfoSet;
	buffer << m_nSOGOpeningTopHookType;
	buffer << m_nSOGOpeningBotHookType;

	buffer << m_bSOGRebarAreaInfoSet;
	buffer << m_nSOGRebarTolDist;
	buffer << m_nSOGRebarTolLenDif;

	buffer << m_bSOGTopSpacerInfoSet;
	buffer << m_bSOGTopSpacerSet;
	buffer << m_nSOGTopSpacerDiaType;
	buffer << m_nSOGTopSpacerDiaIndex;
	buffer << m_nSOGTopSpacerSpacingType;
	buffer << m_nSOGTopSpacerSlabSpacing;
	buffer << m_nSOGTopSpacerUserSpacing1;
	buffer << m_nSOGTopSpacerUserSpacing2;
	buffer << m_nSOGTopSpacerHeightType;
	buffer << m_nSOGTopSpacerUserHeight;
	buffer << m_nSOGTopSpacerHookLenAType;
	buffer << m_nSOGTopSpacerMainBarHookLenA;
	buffer << m_nSOGTopSpacerUserHookLenA;
	buffer << m_nSOGTopSpacerHookLenBType;
	buffer << m_nSOGTopSpacerMainBarHookLenB;
	buffer << m_nSOGTopSpacerUserHookLenB;
	buffer << m_nSOGTopSpacerWidth;

	buffer << m_bSOGBotSpacerInfoSet;
	buffer << m_bSOGBotSpacerSet;
	buffer << m_nSOGBotSpacerDiaType;
	buffer << m_nSOGBotSpacerDiaIndex;
	buffer << m_nSOGBotSpacerSpacingType;
	buffer << m_nSOGBotSpacerSlabSpacing;
	buffer << m_nSOGBotSpacerUserSpacing1;
	buffer << m_nSOGBotSpacerUserSpacing2;
	buffer << m_nSOGBotSpacerHeightType;
	buffer << m_nSOGBotSpacerUserHeight;
	buffer << m_nSOGBotSpacerHookLenAType;
	buffer << m_nSOGBotSpacerMainBarHookLenA;
	buffer << m_nSOGBotSpacerUserHookLenA;
	buffer << m_nSOGBotSpacerHookLenBType;
	buffer << m_nSOGBotSpacerMainBarHookLenB;
	buffer << m_nSOGBotSpacerUserHookLenB;
	buffer << m_nSOGBotSpacerWidth;

	// Circular Ramp
	buffer << m_bCircularRampLongMainBarInfoSet;
	buffer << m_nCircularRampLongMaxDualHookLength;
	buffer << m_nCircularRampLongSpliceWay;
	buffer << m_nCircularRampLongSpliceType;
	buffer << m_bCircularRampLongAlterSpliceSet;
	buffer << m_nCircularRampLongAlterSpliceValue1;

	buffer << m_bCircularRampShortMainBarInfoSet;
	buffer << m_nCircularRampShortMaxDualHookLength;
	buffer << m_nCircularRampShortSpliceWay;
	buffer << m_nCircularRampShortSpliceType;
	buffer << m_bCircularRampShortAlterSpliceSet;
	buffer << m_nCircularRampShortAlterSpliceValue1;

	buffer << m_bCircularRampEndInfoSet;
	buffer << m_nCircularRampTopEndHookType;
	buffer << m_nCircularRampBotEndHookType;
	buffer << m_nCircularRampIsoFootDevType;
	buffer << m_nCircularRampSpliceDevSlabType;

	buffer << m_bCircularRampLevelDifInfoSet;
	buffer << m_nCircularRampLevelDifPlaceShape;
	buffer << m_nCircularRampLevelDifHaunchPlaceType;

	buffer << m_bCircularRampOpeningInfoSet;
	buffer << m_nCircularRampOpeningTopHookType;
	buffer << m_nCircularRampOpeningBotHookType;

	buffer << m_bCircularRampRebarAreaInfoSet;
	buffer << m_nCircularRampRebarTolDist;
	buffer << m_nCircularRampRebarTolLenDif;

	buffer << m_bCircularRampTopSpacerInfoSet;
	buffer << m_bCircularRampTopSpacerSet;
	buffer << m_nCircularRampTopSpacerDiaType;
	buffer << m_nCircularRampTopSpacerDiaIndex;
	buffer << m_nCircularRampTopSpacerSpacingType;
	buffer << m_nCircularRampTopSpacerSlabSpacing;
	buffer << m_nCircularRampTopSpacerUserSpacing1;
	buffer << m_nCircularRampTopSpacerUserSpacing2;
	buffer << m_nCircularRampTopSpacerHeightType;
	buffer << m_nCircularRampTopSpacerUserHeight;
	buffer << m_nCircularRampTopSpacerHookLenAType;
	buffer << m_nCircularRampTopSpacerMainBarHookLenA;
	buffer << m_nCircularRampTopSpacerUserHookLenA;
	buffer << m_nCircularRampTopSpacerHookLenBType;
	buffer << m_nCircularRampTopSpacerMainBarHookLenB;
	buffer << m_nCircularRampTopSpacerUserHookLenB;
	buffer << m_nCircularRampTopSpacerWidth;

	buffer << m_bCircularRampBotSpacerInfoSet;
	buffer << m_bCircularRampBotSpacerSet;
	buffer << m_nCircularRampBotSpacerDiaType;
	buffer << m_nCircularRampBotSpacerDiaIndex;
	buffer << m_nCircularRampBotSpacerSpacingType;
	buffer << m_nCircularRampBotSpacerSlabSpacing;
	buffer << m_nCircularRampBotSpacerUserSpacing1;
	buffer << m_nCircularRampBotSpacerUserSpacing2;
	buffer << m_nCircularRampBotSpacerHeightType;
	buffer << m_nCircularRampBotSpacerUserHeight;
	buffer << m_nCircularRampBotSpacerHookLenAType;
	buffer << m_nCircularRampBotSpacerMainBarHookLenA;
	buffer << m_nCircularRampBotSpacerUserHookLenA;
	buffer << m_nCircularRampBotSpacerHookLenBType;
	buffer << m_nCircularRampBotSpacerMainBarHookLenB;
	buffer << m_nCircularRampBotSpacerUserHookLenB;
	buffer << m_nCircularRampBotSpacerWidth;

	// PC Slab
	buffer << m_bPCSlabLongMainBarInfoSet;
	buffer << m_nPCSlabLongMaxDualHookLength;
	buffer << m_nPCSlabLongSpliceWay;
	buffer << m_nPCSlabLongSpliceType;
	buffer << m_bPCSlabLongAlterSpliceSet;
	buffer << m_nPCSlabLongAlterSpliceValue1;

	buffer << m_bPCSlabShortMainBarInfoSet;
	buffer << m_nPCSlabShortMaxDualHookLength;
	buffer << m_nPCSlabShortSpliceWay;
	buffer << m_nPCSlabShortSpliceType;
	buffer << m_bPCSlabShortAlterSpliceSet;
	buffer << m_nPCSlabShortAlterSpliceValue1;

	buffer << m_bPCSlabStructuredEndInfoSet;
	buffer << m_nPCSlabTopStructuredEndHookType;
	buffer << m_nPCSlabBotStructuredEndHookType;

	buffer << m_bPCSlabDeckInfoSet;
	buffer << m_nPCSlabDeckDifDiaConnectBarType;
	buffer << m_nPCSlabDeckAnchorType;
	buffer << m_nPCSlabDeckTopEndRatioType;
	buffer << m_nPCSlabDeckBotEndRatioType;

	buffer << m_bPCSlabUnStructuredEndInfoSet;
	buffer << m_nPCSlabTopUnStructuredEndHookType;
	buffer << m_nPCSlabBotUnStructuredEndHookType;
	buffer << m_nPCSlabIsoFootUnStructuredDevType;

	buffer << m_bPCSlabLevelDifInfoSet;
	buffer << m_nPCSlabLevelDifPlaceShape;
	buffer << m_nPCSlabLevelDifHaunchPlaceType;

	buffer << m_bPCSlabOpeningInfoSet;
	buffer << m_nPCSlabOpeningTopHookType;
	buffer << m_nPCSlabOpeningBotHookType;

	buffer << m_bPCSlabRebarAreaInfoSet;
	buffer << m_nPCSlabRebarTolDist;
	buffer << m_nPCSlabRebarTolLenDif;

	buffer << m_bPCSlabTopSpacerInfoSet;
	buffer << m_bPCSlabTopSpacerSet;
	buffer << m_nPCSlabTopSpacerDiaType;
	buffer << m_nPCSlabTopSpacerDiaIndex;
	buffer << m_nPCSlabTopSpacerSpacingType;
	buffer << m_nPCSlabTopSpacerSlabSpacing;
	buffer << m_nPCSlabTopSpacerUserSpacing1;
	buffer << m_nPCSlabTopSpacerUserSpacing2;
	buffer << m_nPCSlabTopSpacerHeightType;
	buffer << m_nPCSlabTopSpacerUserHeight;
	buffer << m_nPCSlabTopSpacerHookLenAType;
	buffer << m_nPCSlabTopSpacerMainBarHookLenA;
	buffer << m_nPCSlabTopSpacerUserHookLenA;
	buffer << m_nPCSlabTopSpacerHookLenBType;
	buffer << m_nPCSlabTopSpacerMainBarHookLenB;
	buffer << m_nPCSlabTopSpacerUserHookLenB;
	buffer << m_nPCSlabTopSpacerWidth;

	buffer << m_bPCSlabBotSpacerInfoSet;
	buffer << m_bPCSlabBotSpacerSet;
	buffer << m_nPCSlabBotSpacerDiaType;
	buffer << m_nPCSlabBotSpacerDiaIndex;
	buffer << m_nPCSlabBotSpacerSpacingType;
	buffer << m_nPCSlabBotSpacerSlabSpacing;
	buffer << m_nPCSlabBotSpacerUserSpacing1;
	buffer << m_nPCSlabBotSpacerUserSpacing2;
	buffer << m_nPCSlabBotSpacerHeightType;
	buffer << m_nPCSlabBotSpacerUserHeight;
	buffer << m_nPCSlabBotSpacerHookLenAType;
	buffer << m_nPCSlabBotSpacerMainBarHookLenA;
	buffer << m_nPCSlabBotSpacerUserHookLenA;
	buffer << m_nPCSlabBotSpacerHookLenBType;
	buffer << m_nPCSlabBotSpacerMainBarHookLenB;
	buffer << m_nPCSlabBotSpacerUserHookLenB;
	buffer << m_nPCSlabBotSpacerWidth;

	// 철근 일체형 데크 슬래브
	buffer << m_bEmbeddedDeckLongMainBarInfoSet;
	buffer << m_nEmbeddedDeckLongMaxDualHookLength;
	buffer << m_nEmbeddedDeckLongSpliceWay;
	buffer << m_nEmbeddedDeckLongSpliceType;
	buffer << m_bEmbeddedDeckLongAlterSpliceSet;
	buffer << m_nEmbeddedDeckLongAlterSpliceValue1;

	buffer << m_bEmbeddedDeckShortMainBarInfoSet;
	buffer << m_nEmbeddedDeckShortMaxDualHookLength;
	buffer << m_nEmbeddedDeckShortSpliceWay;
	buffer << m_nEmbeddedDeckShortSpliceType;
	buffer << m_bEmbeddedDeckShortAlterSpliceSet;
	buffer << m_nEmbeddedDeckShortAlterSpliceValue1;

	buffer << m_bEmbeddedDeckStructuredEndInfoSet;
	buffer << m_nEmbeddedDeckTopStructuredEndHookType;
	buffer << m_nEmbeddedDeckBotStructuredEndHookType;

	buffer << m_bEmbeddedDeckInfoSet;
	buffer << m_nEmbeddedDeckDifDiaConnectBarType;
	buffer << m_nEmbeddedDeckAnchorType;
	buffer << m_nEmbeddedDeckTopEndRatioType;
	buffer << m_nEmbeddedDeckBotEndRatioType;

	buffer << m_bEmbeddedDeckUnStructuredEndInfoSet;
	buffer << m_nEmbeddedDeckTopUnStructuredEndHookType;
	buffer << m_nEmbeddedDeckBotUnStructuredEndHookType;
	buffer << m_nEmbeddedDeckIsoFootUnStructuredDevType;

	buffer << m_bEmbeddedDeckLevelDifInfoSet;
	buffer << m_nEmbeddedDeckLevelDifPlaceShape;
	buffer << m_nEmbeddedDeckLevelDifHaunchPlaceType;

	buffer << m_bEmbeddedDeckOpeningInfoSet;
	buffer << m_nEmbeddedDeckOpeningTopHookType;
	buffer << m_nEmbeddedDeckOpeningBotHookType;

	buffer << m_bEmbeddedDeckRebarAreaInfoSet;
	buffer << m_nEmbeddedDeckRebarTolDist;
	buffer << m_nEmbeddedDeckRebarTolLenDif;

	// 일반 데크 슬래브
	buffer << m_bGeneralDeckLongMainBarInfoSet;
	buffer << m_nGeneralDeckLongMaxDualHookLength;
	buffer << m_nGeneralDeckLongSpliceWay;
	buffer << m_nGeneralDeckLongSpliceType;
	buffer << m_bGeneralDeckLongAlterSpliceSet;
	buffer << m_nGeneralDeckLongAlterSpliceValue1;

	buffer << m_bGeneralDeckShortMainBarInfoSet;
	buffer << m_nGeneralDeckShortMaxDualHookLength;
	buffer << m_nGeneralDeckShortSpliceWay;
	buffer << m_nGeneralDeckShortSpliceType;
	buffer << m_bGeneralDeckShortAlterSpliceSet;
	buffer << m_nGeneralDeckShortAlterSpliceValue1;

	buffer << m_bGeneralDeckStructuredEndInfoSet;
	buffer << m_nGeneralDeckTopStructuredEndHookType;
	buffer << m_nGeneralDeckBotStructuredEndHookType;

	buffer << m_bGeneralDeckUnStructuredEndInfoSet;
	buffer << m_nGeneralDeckTopUnStructuredEndHookType;
	buffer << m_nGeneralDeckBotUnStructuredEndHookType;
	buffer << m_nGeneralDeckIsoFootUnStructuredDevType;

	buffer << m_bGeneralDeckLevelDifInfoSet;
	buffer << m_nGeneralDeckLevelDifPlaceShape;
	buffer << m_nGeneralDeckLevelDifHaunchPlaceType;

	buffer << m_bGeneralDeckOpeningInfoSet;
	buffer << m_nGeneralDeckOpeningTopHookType;
	buffer << m_nGeneralDeckOpeningBotHookType;

	buffer << m_bGeneralDeckRebarAreaInfoSet;
	buffer << m_nGeneralDeckRebarTolDist;
	buffer << m_nGeneralDeckRebarTolLenDif;
}

void OldSlabRebarTemplate::Recovery(EFS::EFS_Buffer& buffer)
{
	// MAT Slab
	buffer >> m_bMatLongMainBarInfoSet;
	buffer >> m_nMatLongMaxDualHookLength;
	buffer >> m_nMatLongSpliceWay;
	buffer >> m_nMatLongSpliceType;
	buffer >> m_bMatLongAlterSpliceSet;
	buffer >> m_nMatLongAlterSpliceValue1;

	buffer >> m_bMatShortMainBarInfoSet;
	buffer >> m_nMatShortMaxDualHookLength;
	buffer >> m_nMatShortSpliceWay;
	buffer >> m_nMatShortSpliceType;
	buffer >> m_bMatShortAlterSpliceSet;
	buffer >> m_nMatShortAlterSpliceValue1;

	buffer >> m_bMatEndInfoSet;
	buffer >> m_nMatTopEndHookType;
	buffer >> m_nMatBotEndHookType;
	buffer >> m_nMatIsoFootDevType;

	buffer >> m_bMatLevelDifInfoSet;
	buffer >> m_nMatLevelDifPlaceShape;
	buffer >> m_nMatLevelDifHaunchPlaceType;

	buffer >> m_bMatRebarAreaInfoSet;
	buffer >> m_nMatRebarTolDist;
	buffer >> m_nMatRebarTolLenDif;

	buffer >> m_bMatTopSpacerInfoSet;
	buffer >> m_bMatTopSpacerSet;
	buffer >> m_nMatTopSpacerDiaType;
	buffer >> m_nMatTopSpacerDiaIndex;
	buffer >> m_nMatTopSpacerSpacingType;
	buffer >> m_nMatTopSpacerSlabSpacing;
	buffer >> m_nMatTopSpacerUserSpacing1;
	buffer >> m_nMatTopSpacerUserSpacing2;
	buffer >> m_nMatTopSpacerHeightType;
	buffer >> m_nMatTopSpacerUserHeight;
	buffer >> m_nMatTopSpacerHookLenAType;
	buffer >> m_nMatTopSpacerMainBarHookLenA;
	buffer >> m_nMatTopSpacerUserHookLenA;
	buffer >> m_nMatTopSpacerHookLenBType;
	buffer >> m_nMatTopSpacerMainBarHookLenB;
	buffer >> m_nMatTopSpacerUserHookLenB;
	buffer >> m_nMatTopSpacerWidth;

	buffer >> m_bMatBotSpacerInfoSet;
	buffer >> m_bMatBotSpacerSet;
	buffer >> m_nMatBotSpacerDiaType;
	buffer >> m_nMatBotSpacerDiaIndex;
	buffer >> m_nMatBotSpacerSpacingType;
	buffer >> m_nMatBotSpacerSlabSpacing;
	buffer >> m_nMatBotSpacerUserSpacing1;
	buffer >> m_nMatBotSpacerUserSpacing2;
	buffer >> m_nMatBotSpacerHeightType;
	buffer >> m_nMatBotSpacerUserHeight;
	buffer >> m_nMatBotSpacerHookLenAType;
	buffer >> m_nMatBotSpacerMainBarHookLenA;
	buffer >> m_nMatBotSpacerUserHookLenA;
	buffer >> m_nMatBotSpacerHookLenBType;
	buffer >> m_nMatBotSpacerMainBarHookLenB;
	buffer >> m_nMatBotSpacerUserHookLenB;
	buffer >> m_nMatBotSpacerWidth;

	// Flat Slab
	buffer >> m_bFlatLongMainBarInfoSet;
	buffer >> m_nFlatLongMaxDualHookLength;
	buffer >> m_nFlatLongSpliceWay;
	buffer >> m_nFlatLongSpliceType;
	buffer >> m_bFlatLongAlterSpliceSet;
	buffer >> m_nFlatLongAlterSpliceValue1;

	buffer >> m_bFlatShortMainBarInfoSet;
	buffer >> m_nFlatShortMaxDualHookLength;
	buffer >> m_nFlatShortSpliceWay;
	buffer >> m_nFlatShortSpliceType;
	buffer >> m_bFlatShortAlterSpliceSet;
	buffer >> m_nFlatShortAlterSpliceValue1;

	buffer >> m_bFlatEndInfoSet;
	buffer >> m_nFlatTopEndHookType;
	buffer >> m_nFlatBotEndHookType;
	buffer >> m_nFlatIsoFootDevType;
	buffer >> m_nFlatSpliceDevSlabType;

	buffer >> m_bFlatLevelDifInfoSet;
	buffer >> m_nFlatLevelDifPlaceShape;
	buffer >> m_nFlatLevelDifHaunchPlaceType;

	buffer >> m_bFlatOpeningInfoSet;
	buffer >> m_nFlatOpeningTopHookType;
	buffer >> m_nFlatOpeningBotHookType;

	buffer >> m_bFlatRebarAreaInfoSet;
	buffer >> m_nFlatRebarTolDist;
	buffer >> m_nFlatRebarTolLenDif;

	buffer >> m_bFlatTopSpacerInfoSet;
	buffer >> m_bFlatTopSpacerSet;
	buffer >> m_nFlatTopSpacerDiaType;
	buffer >> m_nFlatTopSpacerDiaIndex;
	buffer >> m_nFlatTopSpacerSpacingType;
	buffer >> m_nFlatTopSpacerSlabSpacing;
	buffer >> m_nFlatTopSpacerUserSpacing1;
	buffer >> m_nFlatTopSpacerUserSpacing2;
	buffer >> m_nFlatTopSpacerHeightType;
	buffer >> m_nFlatTopSpacerUserHeight;
	buffer >> m_nFlatTopSpacerHookLenAType;
	buffer >> m_nFlatTopSpacerMainBarHookLenA;
	buffer >> m_nFlatTopSpacerUserHookLenA;
	buffer >> m_nFlatTopSpacerHookLenBType;
	buffer >> m_nFlatTopSpacerMainBarHookLenB;
	buffer >> m_nFlatTopSpacerUserHookLenB;
	buffer >> m_nFlatTopSpacerWidth;

	buffer >> m_bFlatBotSpacerInfoSet;
	buffer >> m_bFlatBotSpacerSet;
	buffer >> m_nFlatBotSpacerDiaType;
	buffer >> m_nFlatBotSpacerDiaIndex;
	buffer >> m_nFlatBotSpacerSpacingType;
	buffer >> m_nFlatBotSpacerSlabSpacing;
	buffer >> m_nFlatBotSpacerUserSpacing1;
	buffer >> m_nFlatBotSpacerUserSpacing2;
	buffer >> m_nFlatBotSpacerHeightType;
	buffer >> m_nFlatBotSpacerUserHeight;
	buffer >> m_nFlatBotSpacerHookLenAType;
	buffer >> m_nFlatBotSpacerMainBarHookLenA;
	buffer >> m_nFlatBotSpacerUserHookLenA;
	buffer >> m_nFlatBotSpacerHookLenBType;
	buffer >> m_nFlatBotSpacerMainBarHookLenB;
	buffer >> m_nFlatBotSpacerUserHookLenB;
	buffer >> m_nFlatBotSpacerWidth;

	// 사변지지 SLAB
	buffer >> m_bSlabLongMainBarInfoSet;
	buffer >> m_nSlabLongMaxDualHookLength;
	buffer >> m_nSlabLongSpliceWay;
	buffer >> m_nSlabLongSpliceType;
	buffer >> m_bSlabLongAlterSpliceSet;
	buffer >> m_nSlabLongAlterSpliceValue1;

	buffer >> m_bSlabShortMainBarInfoSet;
	buffer >> m_nSlabShortMaxDualHookLength;
	buffer >> m_nSlabShortSpliceWay;
	buffer >> m_nSlabShortSpliceType;
	buffer >> m_bSlabShortAlterSpliceSet;
	buffer >> m_nSlabShortAlterSpliceValue1;

	buffer >> m_bSlabStructuredEndInfoSet;
	buffer >> m_nSlabTopStructuredEndHookType;
	buffer >> m_nSlabBotStructuredEndHookType;

	buffer >> m_bSlabTopCutBarLenInfoSet;
	buffer >> m_nSlabOneWayCutBarLenType;
	buffer >> m_nSlabTwoWayCutBarLenType;

	buffer >> m_bSlabUnStructuredEndInfoSet;
	buffer >> m_nSlabTopUnStructuredEndHookType;
	buffer >> m_nSlabBotUnStructuredEndHookType;
	buffer >> m_nSlabIsoFootUnStructuredDevType;

	buffer >> m_bSlabLevelDifInfoSet;
	buffer >> m_nSlabLevelDifPlaceShape;
	buffer >> m_nSlabLevelDifHaunchPlaceType;

	buffer >> m_bSlabOpeningInfoSet;
	buffer >> m_nSlabOpeningTopHookType;
	buffer >> m_nSlabOpeningBotHookType;

	buffer >> m_bSlabRebarAreaInfoSet;
	buffer >> m_nSlabRebarTolDist;
	buffer >> m_nSlabRebarTolLenDif;

	buffer >> m_bSlabTopSpacerInfoSet;
	buffer >> m_bSlabTopSpacerSet;
	buffer >> m_nSlabTopSpacerDiaType;
	buffer >> m_nSlabTopSpacerDiaIndex;
	buffer >> m_nSlabTopSpacerSpacingType;
	buffer >> m_nSlabTopSpacerSlabSpacing;
	buffer >> m_nSlabTopSpacerUserSpacing1;
	buffer >> m_nSlabTopSpacerUserSpacing2;
	buffer >> m_nSlabTopSpacerHeightType;
	buffer >> m_nSlabTopSpacerUserHeight;
	buffer >> m_nSlabTopSpacerHookLenAType;
	buffer >> m_nSlabTopSpacerMainBarHookLenA;
	buffer >> m_nSlabTopSpacerUserHookLenA;
	buffer >> m_nSlabTopSpacerHookLenBType;
	buffer >> m_nSlabTopSpacerMainBarHookLenB;
	buffer >> m_nSlabTopSpacerUserHookLenB;
	buffer >> m_nSlabTopSpacerWidth;

	buffer >> m_bSlabBotSpacerInfoSet;
	buffer >> m_bSlabBotSpacerSet;
	buffer >> m_nSlabBotSpacerDiaType;
	buffer >> m_nSlabBotSpacerDiaIndex;
	buffer >> m_nSlabBotSpacerSpacingType;
	buffer >> m_nSlabBotSpacerSlabSpacing;
	buffer >> m_nSlabBotSpacerUserSpacing1;
	buffer >> m_nSlabBotSpacerUserSpacing2;
	buffer >> m_nSlabBotSpacerHeightType;
	buffer >> m_nSlabBotSpacerUserHeight;
	buffer >> m_nSlabBotSpacerHookLenAType;
	buffer >> m_nSlabBotSpacerMainBarHookLenA;
	buffer >> m_nSlabBotSpacerUserHookLenA;
	buffer >> m_nSlabBotSpacerHookLenBType;
	buffer >> m_nSlabBotSpacerMainBarHookLenB;
	buffer >> m_nSlabBotSpacerUserHookLenB;
	buffer >> m_nSlabBotSpacerWidth;

	// SOG Slab
	buffer >> m_bSOGLongMainBarInfoSet;
	buffer >> m_nSOGLongMaxDualHookLength;
	buffer >> m_nSOGLongSpliceWay;
	buffer >> m_nSOGLongSpliceType;
	buffer >> m_bSOGLongAlterSpliceSet;
	buffer >> m_nSOGLongAlterSpliceValue1;

	buffer >> m_bSOGShortMainBarInfoSet;
	buffer >> m_nSOGShortMaxDualHookLength;
	buffer >> m_nSOGShortSpliceWay;
	buffer >> m_nSOGShortSpliceType;
	buffer >> m_bSOGShortAlterSpliceSet;
	buffer >> m_nSOGShortAlterSpliceValue1;

	buffer >> m_bSOGEndInfoSet;
	buffer >> m_nSOGTopEndHookType;
	buffer >> m_nSOGBotEndHookType;
	buffer >> m_nSOGIsoFootDevType;
	buffer >> m_nSOGSpliceDevSlabType;

	buffer >> m_bSOGLevelDifInfoSet;
	buffer >> m_nSOGLevelDifPlaceShape;
	buffer >> m_nSOGLevelDifHaunchPlaceType;

	buffer >> m_bSOGOpeningInfoSet;
	buffer >> m_nSOGOpeningTopHookType;
	buffer >> m_nSOGOpeningBotHookType;

	buffer >> m_bSOGRebarAreaInfoSet;
	buffer >> m_nSOGRebarTolDist;
	buffer >> m_nSOGRebarTolLenDif;
	
	buffer >> m_bSOGTopSpacerInfoSet;
	buffer >> m_bSOGTopSpacerSet;
	buffer >> m_nSOGTopSpacerDiaType;
	buffer >> m_nSOGTopSpacerDiaIndex;
	buffer >> m_nSOGTopSpacerSpacingType;
	buffer >> m_nSOGTopSpacerSlabSpacing;
	buffer >> m_nSOGTopSpacerUserSpacing1;
	buffer >> m_nSOGTopSpacerUserSpacing2;
	buffer >> m_nSOGTopSpacerHeightType;
	buffer >> m_nSOGTopSpacerUserHeight;
	buffer >> m_nSOGTopSpacerHookLenAType;
	buffer >> m_nSOGTopSpacerMainBarHookLenA;
	buffer >> m_nSOGTopSpacerUserHookLenA;
	buffer >> m_nSOGTopSpacerHookLenBType;
	buffer >> m_nSOGTopSpacerMainBarHookLenB;
	buffer >> m_nSOGTopSpacerUserHookLenB;
	buffer >> m_nSOGTopSpacerWidth;

	buffer >> m_bSOGBotSpacerInfoSet;
	buffer >> m_bSOGBotSpacerSet;
	buffer >> m_nSOGBotSpacerDiaType;
	buffer >> m_nSOGBotSpacerDiaIndex;
	buffer >> m_nSOGBotSpacerSpacingType;
	buffer >> m_nSOGBotSpacerSlabSpacing;
	buffer >> m_nSOGBotSpacerUserSpacing1;
	buffer >> m_nSOGBotSpacerUserSpacing2;
	buffer >> m_nSOGBotSpacerHeightType;
	buffer >> m_nSOGBotSpacerUserHeight;
	buffer >> m_nSOGBotSpacerHookLenAType;
	buffer >> m_nSOGBotSpacerMainBarHookLenA;
	buffer >> m_nSOGBotSpacerUserHookLenA;
	buffer >> m_nSOGBotSpacerHookLenBType;
	buffer >> m_nSOGBotSpacerMainBarHookLenB;
	buffer >> m_nSOGBotSpacerUserHookLenB;
	buffer >> m_nSOGBotSpacerWidth;

	// Circular Ramp
	buffer >> m_bCircularRampLongMainBarInfoSet;
	buffer >> m_nCircularRampLongMaxDualHookLength;
	buffer >> m_nCircularRampLongSpliceWay;
	buffer >> m_nCircularRampLongSpliceType;
	buffer >> m_bCircularRampLongAlterSpliceSet;
	buffer >> m_nCircularRampLongAlterSpliceValue1;

	buffer >> m_bCircularRampShortMainBarInfoSet;
	buffer >> m_nCircularRampShortMaxDualHookLength;
	buffer >> m_nCircularRampShortSpliceWay;
	buffer >> m_nCircularRampShortSpliceType;
	buffer >> m_bCircularRampShortAlterSpliceSet;
	buffer >> m_nCircularRampShortAlterSpliceValue1;

	buffer >> m_bCircularRampEndInfoSet;
	buffer >> m_nCircularRampTopEndHookType;
	buffer >> m_nCircularRampBotEndHookType;
	buffer >> m_nCircularRampIsoFootDevType;
	buffer >> m_nCircularRampSpliceDevSlabType;

	buffer >> m_bCircularRampLevelDifInfoSet;
	buffer >> m_nCircularRampLevelDifPlaceShape;
	buffer >> m_nCircularRampLevelDifHaunchPlaceType;

	buffer >> m_bCircularRampOpeningInfoSet;
	buffer >> m_nCircularRampOpeningTopHookType;
	buffer >> m_nCircularRampOpeningBotHookType;

	buffer >> m_bCircularRampRebarAreaInfoSet;
	buffer >> m_nCircularRampRebarTolDist;
	buffer >> m_nCircularRampRebarTolLenDif;

	buffer >> m_bCircularRampTopSpacerInfoSet;
	buffer >> m_bCircularRampTopSpacerSet;
	buffer >> m_nCircularRampTopSpacerDiaType;
	buffer >> m_nCircularRampTopSpacerDiaIndex;
	buffer >> m_nCircularRampTopSpacerSpacingType;
	buffer >> m_nCircularRampTopSpacerSlabSpacing;
	buffer >> m_nCircularRampTopSpacerUserSpacing1;
	buffer >> m_nCircularRampTopSpacerUserSpacing2;
	buffer >> m_nCircularRampTopSpacerHeightType;
	buffer >> m_nCircularRampTopSpacerUserHeight;
	buffer >> m_nCircularRampTopSpacerHookLenAType;
	buffer >> m_nCircularRampTopSpacerMainBarHookLenA;
	buffer >> m_nCircularRampTopSpacerUserHookLenA;
	buffer >> m_nCircularRampTopSpacerHookLenBType;
	buffer >> m_nCircularRampTopSpacerMainBarHookLenB;
	buffer >> m_nCircularRampTopSpacerUserHookLenB;
	buffer >> m_nCircularRampTopSpacerWidth;

	buffer >> m_bCircularRampBotSpacerInfoSet;
	buffer >> m_bCircularRampBotSpacerSet;
	buffer >> m_nCircularRampBotSpacerDiaType;
	buffer >> m_nCircularRampBotSpacerDiaIndex;
	buffer >> m_nCircularRampBotSpacerSpacingType;
	buffer >> m_nCircularRampBotSpacerSlabSpacing;
	buffer >> m_nCircularRampBotSpacerUserSpacing1;
	buffer >> m_nCircularRampBotSpacerUserSpacing2;
	buffer >> m_nCircularRampBotSpacerHeightType;
	buffer >> m_nCircularRampBotSpacerUserHeight;
	buffer >> m_nCircularRampBotSpacerHookLenAType;
	buffer >> m_nCircularRampBotSpacerMainBarHookLenA;
	buffer >> m_nCircularRampBotSpacerUserHookLenA;
	buffer >> m_nCircularRampBotSpacerHookLenBType;
	buffer >> m_nCircularRampBotSpacerMainBarHookLenB;
	buffer >> m_nCircularRampBotSpacerUserHookLenB;
	buffer >> m_nCircularRampBotSpacerWidth;

	// PC Slab
	buffer >> m_bPCSlabLongMainBarInfoSet;
	buffer >> m_nPCSlabLongMaxDualHookLength;
	buffer >> m_nPCSlabLongSpliceWay;
	buffer >> m_nPCSlabLongSpliceType;
	buffer >> m_bPCSlabLongAlterSpliceSet;
	buffer >> m_nPCSlabLongAlterSpliceValue1;

	buffer >> m_bPCSlabShortMainBarInfoSet;
	buffer >> m_nPCSlabShortMaxDualHookLength;
	buffer >> m_nPCSlabShortSpliceWay;
	buffer >> m_nPCSlabShortSpliceType;
	buffer >> m_bPCSlabShortAlterSpliceSet;
	buffer >> m_nPCSlabShortAlterSpliceValue1;

	buffer >> m_bPCSlabStructuredEndInfoSet;
	buffer >> m_nPCSlabTopStructuredEndHookType;
	buffer >> m_nPCSlabBotStructuredEndHookType;

	buffer >> m_bPCSlabDeckInfoSet;
	buffer >> m_nPCSlabDeckDifDiaConnectBarType;
	buffer >> m_nPCSlabDeckAnchorType;
	buffer >> m_nPCSlabDeckTopEndRatioType;
	buffer >> m_nPCSlabDeckBotEndRatioType;

	buffer >> m_bPCSlabUnStructuredEndInfoSet;
	buffer >> m_nPCSlabTopUnStructuredEndHookType;
	buffer >> m_nPCSlabBotUnStructuredEndHookType;
	buffer >> m_nPCSlabIsoFootUnStructuredDevType;

	buffer >> m_bPCSlabLevelDifInfoSet;
	buffer >> m_nPCSlabLevelDifPlaceShape;
	buffer >> m_nPCSlabLevelDifHaunchPlaceType;

	buffer >> m_bPCSlabOpeningInfoSet;
	buffer >> m_nPCSlabOpeningTopHookType;
	buffer >> m_nPCSlabOpeningBotHookType;

	buffer >> m_bPCSlabRebarAreaInfoSet;
	buffer >> m_nPCSlabRebarTolDist;
	buffer >> m_nPCSlabRebarTolLenDif;

	buffer >> m_bPCSlabTopSpacerInfoSet;
	buffer >> m_bPCSlabTopSpacerSet;
	buffer >> m_nPCSlabTopSpacerDiaType;
	buffer >> m_nPCSlabTopSpacerDiaIndex;
	buffer >> m_nPCSlabTopSpacerSpacingType;
	buffer >> m_nPCSlabTopSpacerSlabSpacing;
	buffer >> m_nPCSlabTopSpacerUserSpacing1;
	buffer >> m_nPCSlabTopSpacerUserSpacing2;
	buffer >> m_nPCSlabTopSpacerHeightType;
	buffer >> m_nPCSlabTopSpacerUserHeight;
	buffer >> m_nPCSlabTopSpacerHookLenAType;
	buffer >> m_nPCSlabTopSpacerMainBarHookLenA;
	buffer >> m_nPCSlabTopSpacerUserHookLenA;
	buffer >> m_nPCSlabTopSpacerHookLenBType;
	buffer >> m_nPCSlabTopSpacerMainBarHookLenB;
	buffer >> m_nPCSlabTopSpacerUserHookLenB;
	buffer >> m_nPCSlabTopSpacerWidth;

	buffer >> m_bPCSlabBotSpacerInfoSet;
	buffer >> m_bPCSlabBotSpacerSet;
	buffer >> m_nPCSlabBotSpacerDiaType;
	buffer >> m_nPCSlabBotSpacerDiaIndex;
	buffer >> m_nPCSlabBotSpacerSpacingType;
	buffer >> m_nPCSlabBotSpacerSlabSpacing;
	buffer >> m_nPCSlabBotSpacerUserSpacing1;
	buffer >> m_nPCSlabBotSpacerUserSpacing2;
	buffer >> m_nPCSlabBotSpacerHeightType;
	buffer >> m_nPCSlabBotSpacerUserHeight;
	buffer >> m_nPCSlabBotSpacerHookLenAType;
	buffer >> m_nPCSlabBotSpacerMainBarHookLenA;
	buffer >> m_nPCSlabBotSpacerUserHookLenA;
	buffer >> m_nPCSlabBotSpacerHookLenBType;
	buffer >> m_nPCSlabBotSpacerMainBarHookLenB;
	buffer >> m_nPCSlabBotSpacerUserHookLenB;
	buffer >> m_nPCSlabBotSpacerWidth;

	// 철근 일체형 데크 슬래브
	buffer >> m_bEmbeddedDeckLongMainBarInfoSet;
	buffer >> m_nEmbeddedDeckLongMaxDualHookLength;
	buffer >> m_nEmbeddedDeckLongSpliceWay;
	buffer >> m_nEmbeddedDeckLongSpliceType;
	buffer >> m_bEmbeddedDeckLongAlterSpliceSet;
	buffer >> m_nEmbeddedDeckLongAlterSpliceValue1;

	buffer >> m_bEmbeddedDeckShortMainBarInfoSet;
	buffer >> m_nEmbeddedDeckShortMaxDualHookLength;
	buffer >> m_nEmbeddedDeckShortSpliceWay;
	buffer >> m_nEmbeddedDeckShortSpliceType;
	buffer >> m_bEmbeddedDeckShortAlterSpliceSet;
	buffer >> m_nEmbeddedDeckShortAlterSpliceValue1;

	buffer >> m_bEmbeddedDeckStructuredEndInfoSet;
	buffer >> m_nEmbeddedDeckTopStructuredEndHookType;
	buffer >> m_nEmbeddedDeckBotStructuredEndHookType;

	buffer >> m_bEmbeddedDeckInfoSet;
	buffer >> m_nEmbeddedDeckDifDiaConnectBarType;
	buffer >> m_nEmbeddedDeckAnchorType;
	buffer >> m_nEmbeddedDeckTopEndRatioType;
	buffer >> m_nEmbeddedDeckBotEndRatioType;

	buffer >> m_bEmbeddedDeckUnStructuredEndInfoSet;
	buffer >> m_nEmbeddedDeckTopUnStructuredEndHookType;
	buffer >> m_nEmbeddedDeckBotUnStructuredEndHookType;
	buffer >> m_nEmbeddedDeckIsoFootUnStructuredDevType;

	buffer >> m_bEmbeddedDeckLevelDifInfoSet;
	buffer >> m_nEmbeddedDeckLevelDifPlaceShape;
	buffer >> m_nEmbeddedDeckLevelDifHaunchPlaceType;

	buffer >> m_bEmbeddedDeckOpeningInfoSet;
	buffer >> m_nEmbeddedDeckOpeningTopHookType;
	buffer >> m_nEmbeddedDeckOpeningBotHookType;

	buffer >> m_bEmbeddedDeckRebarAreaInfoSet;
	buffer >> m_nEmbeddedDeckRebarTolDist;
	buffer >> m_nEmbeddedDeckRebarTolLenDif;

	// 일반 데크 슬래브
	buffer >> m_bGeneralDeckLongMainBarInfoSet;
	buffer >> m_nGeneralDeckLongMaxDualHookLength;
	buffer >> m_nGeneralDeckLongSpliceWay;
	buffer >> m_nGeneralDeckLongSpliceType;
	buffer >> m_bGeneralDeckLongAlterSpliceSet;
	buffer >> m_nGeneralDeckLongAlterSpliceValue1;

	buffer >> m_bGeneralDeckShortMainBarInfoSet;
	buffer >> m_nGeneralDeckShortMaxDualHookLength;
	buffer >> m_nGeneralDeckShortSpliceWay;
	buffer >> m_nGeneralDeckShortSpliceType;
	buffer >> m_bGeneralDeckShortAlterSpliceSet;
	buffer >> m_nGeneralDeckShortAlterSpliceValue1;

	buffer >> m_bGeneralDeckStructuredEndInfoSet;
	buffer >> m_nGeneralDeckTopStructuredEndHookType;
	buffer >> m_nGeneralDeckBotStructuredEndHookType;

	buffer >> m_bGeneralDeckUnStructuredEndInfoSet;
	buffer >> m_nGeneralDeckTopUnStructuredEndHookType;
	buffer >> m_nGeneralDeckBotUnStructuredEndHookType;
	buffer >> m_nGeneralDeckIsoFootUnStructuredDevType;

	buffer >> m_bGeneralDeckLevelDifInfoSet;
	buffer >> m_nGeneralDeckLevelDifPlaceShape;
	buffer >> m_nGeneralDeckLevelDifHaunchPlaceType;

	buffer >> m_bGeneralDeckOpeningInfoSet;
	buffer >> m_nGeneralDeckOpeningTopHookType;
	buffer >> m_nGeneralDeckOpeningBotHookType;

	buffer >> m_bGeneralDeckRebarAreaInfoSet;
	buffer >> m_nGeneralDeckRebarTolDist;
	buffer >> m_nGeneralDeckRebarTolLenDif;
}

void OldSlabRebarTemplate::Serialize(CArchive& ar)
{
	BOOL bDefaultValue = true;
	long nDefaultValue = 0;

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
		ar << m_bMatLongAlterSpliceSet;
		ar << m_nMatLongAlterSpliceValue1;
		ar << nDefaultValue;
		ar << bDefaultValue;
		ar << bDefaultValue;
		ar << m_bMatShortAlterSpliceSet;
		ar << m_nMatShortAlterSpliceValue1;
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
		ar << m_nFlatOpeningTopHookType;
		ar << m_nFlatOpeningBotHookType;

		ar << m_bFlatLevelDifInfoSet;
		ar << nDefaultValue;
		ar << nDefaultValue;

		ar << m_bFlatEndInfoSet;
		ar << m_nFlatTopEndHookType;
		ar << m_nFlatBotEndHookType;

		ar << bDefaultValue;
		ar << m_bFlatLongAlterSpliceSet;
		ar << m_nFlatLongAlterSpliceValue1;
		ar << nDefaultValue;
		ar << bDefaultValue;
		ar << bDefaultValue;
		ar << m_bFlatShortAlterSpliceSet;
		ar << m_nFlatShortAlterSpliceValue1;
		ar << nDefaultValue;
		ar << bDefaultValue;
		ar << bDefaultValue;

		ar << m_bEmbeddedDeckInfoSet;
		ar << m_nEmbeddedDeckDifDiaConnectBarType;
		ar << m_nEmbeddedDeckAnchorType;
		ar << m_nEmbeddedDeckTopEndRatioType;
		ar << m_nEmbeddedDeckBotEndRatioType;

		ar << m_bFlatRebarAreaInfoSet;
		ar << m_nFlatRebarTolDist;
		ar << m_nFlatRebarTolLenDif;

		// SLAB
		ar << bDefaultValue;
		ar << nDefaultValue;

		ar << m_bSlabStructuredEndInfoSet;
		ar << m_nSlabTopStructuredEndHookType;
		ar << m_nSlabBotStructuredEndHookType;

		ar << m_bSlabTopCutBarLenInfoSet;
		ar << m_nSlabOneWayCutBarLenType;
		ar << m_nSlabTwoWayCutBarLenType;

		// Mat
		ar << m_nMatLevelDifPlaceShape;
		ar << m_nMatLevelDifHaunchPlaceType;
		ar << m_bMatLongMainBarInfoSet;
		ar << m_nMatLongMaxDualHookLength;
		ar << m_nMatLongSpliceWay;
		ar << m_nMatLongSpliceType;
		ar << m_bMatShortMainBarInfoSet;
		ar << m_nMatShortMaxDualHookLength;
		ar << m_nMatShortSpliceWay;
		ar << m_nMatShortSpliceType;

		// Flat
		ar << m_nFlatLevelDifPlaceShape;
		ar << m_nFlatLevelDifHaunchPlaceType;
		ar << m_bFlatLongMainBarInfoSet;
		ar << m_nFlatLongMaxDualHookLength;
		ar << m_nFlatLongSpliceWay;
		ar << m_nFlatLongSpliceType;
		ar << m_bFlatShortMainBarInfoSet;
		ar << m_nFlatShortMaxDualHookLength;
		ar << m_nFlatShortSpliceWay;
		ar << m_nFlatShortSpliceType;

		// Slab
		ar << m_bSlabLongMainBarInfoSet;
		ar << m_nSlabLongMaxDualHookLength;
		ar << m_nSlabLongSpliceWay;
		ar << m_nSlabLongSpliceType;
		ar << m_bSlabLongAlterSpliceSet;
		ar << m_nSlabLongAlterSpliceValue1;
		ar << nDefaultValue;
		ar << m_bSlabShortMainBarInfoSet;
		ar << m_nSlabShortMaxDualHookLength;
		ar << m_nSlabShortSpliceWay;
		ar << m_nSlabShortSpliceType;
		ar << m_bSlabShortAlterSpliceSet;
		ar << m_nSlabShortAlterSpliceValue1;
		ar << nDefaultValue;

		ar << bDefaultValue;
		ar << m_nSOGSpliceDevSlabType;
		ar << m_nFlatSpliceDevSlabType;
		ar << m_nCircularRampSpliceDevSlabType;

		ar << m_bSlabUnStructuredEndInfoSet;
		ar << m_nSlabTopUnStructuredEndHookType;
		ar << m_nSlabBotUnStructuredEndHookType;
		ar << m_nSlabIsoFootUnStructuredDevType;

		ar << m_bSlabLevelDifInfoSet;
		ar << m_nSlabLevelDifPlaceShape;
		ar << m_nSlabLevelDifHaunchPlaceType;

		ar << m_bSlabOpeningInfoSet;
		ar << m_nSlabOpeningTopHookType;
		ar << m_nSlabOpeningBotHookType;

		ar << m_bSlabRebarAreaInfoSet;
		ar << m_nSlabRebarTolDist;
		ar << m_nSlabRebarTolLenDif;

		// SOG Slab
		ar << m_bSOGLongMainBarInfoSet;
		ar << m_nSOGLongMaxDualHookLength;
		ar << m_nSOGLongSpliceWay;
		ar << m_nSOGLongSpliceType;
		ar << m_bSOGLongAlterSpliceSet;
		ar << m_nSOGLongAlterSpliceValue1;

		ar << m_bSOGShortMainBarInfoSet;
		ar << m_nSOGShortMaxDualHookLength;
		ar << m_nSOGShortSpliceWay;
		ar << m_nSOGShortSpliceType;
		ar << m_bSOGShortAlterSpliceSet;
		ar << m_nSOGShortAlterSpliceValue1;

		ar << m_bSOGEndInfoSet;
		ar << m_nSOGTopEndHookType;
		ar << m_nSOGBotEndHookType;
		ar << m_nSOGIsoFootDevType;

		ar << m_bSOGLevelDifInfoSet;
		ar << m_nSOGLevelDifPlaceShape;
		ar << m_nSOGLevelDifHaunchPlaceType;

		ar << m_bSOGOpeningInfoSet;
		ar << m_nSOGOpeningTopHookType;
		ar << m_nSOGOpeningBotHookType;

		ar << m_bSOGRebarAreaInfoSet;
		ar << m_nSOGRebarTolDist;
		ar << m_nSOGRebarTolLenDif;

		// CircularRamp
		ar << m_bCircularRampLongMainBarInfoSet;
		ar << m_nCircularRampLongMaxDualHookLength;
		ar << m_nCircularRampLongSpliceWay;
		ar << m_nCircularRampLongSpliceType;
		ar << m_bCircularRampLongAlterSpliceSet;
		ar << m_nCircularRampLongAlterSpliceValue1;

		ar << m_bCircularRampShortMainBarInfoSet;
		ar << m_nCircularRampShortMaxDualHookLength;
		ar << m_nCircularRampShortSpliceWay;
		ar << m_nCircularRampShortSpliceType;
		ar << m_bCircularRampShortAlterSpliceSet;
		ar << m_nCircularRampShortAlterSpliceValue1;

		ar << m_bCircularRampEndInfoSet;
		ar << m_nCircularRampTopEndHookType;
		ar << m_nCircularRampBotEndHookType;
		ar << m_nCircularRampIsoFootDevType;

		ar << m_bCircularRampLevelDifInfoSet;
		ar << m_nCircularRampLevelDifPlaceShape;
		ar << m_nCircularRampLevelDifHaunchPlaceType;

		ar << m_bCircularRampOpeningInfoSet;
		ar << m_nCircularRampOpeningTopHookType;
		ar << m_nCircularRampOpeningBotHookType;

		ar << m_bCircularRampRebarAreaInfoSet;
		ar << m_nCircularRampRebarTolDist;
		ar << m_nCircularRampRebarTolLenDif;

		// PC Slab
		ar << m_bPCSlabLongMainBarInfoSet;
		ar << m_nPCSlabLongMaxDualHookLength;
		ar << m_nPCSlabLongSpliceWay;
		ar << m_nPCSlabLongSpliceType;
		ar << m_bPCSlabLongAlterSpliceSet;
		ar << m_nPCSlabLongAlterSpliceValue1;

		ar << m_bPCSlabShortMainBarInfoSet;
		ar << m_nPCSlabShortMaxDualHookLength;
		ar << m_nPCSlabShortSpliceWay;
		ar << m_nPCSlabShortSpliceType;
		ar << m_bPCSlabShortAlterSpliceSet;
		ar << m_nPCSlabShortAlterSpliceValue1;

		ar << m_bPCSlabStructuredEndInfoSet;
		ar << m_nPCSlabTopStructuredEndHookType;
		ar << m_nPCSlabBotStructuredEndHookType;

		ar << m_bPCSlabDeckInfoSet;
		ar << m_nPCSlabDeckDifDiaConnectBarType;
		ar << m_nPCSlabDeckAnchorType;
		ar << m_nPCSlabDeckTopEndRatioType;
		ar << m_nPCSlabDeckBotEndRatioType;

		ar << m_bPCSlabUnStructuredEndInfoSet;
		ar << m_nPCSlabTopUnStructuredEndHookType;
		ar << m_nPCSlabBotUnStructuredEndHookType;
		ar << m_nPCSlabIsoFootUnStructuredDevType;

		ar << m_bPCSlabLevelDifInfoSet;
		ar << m_nPCSlabLevelDifPlaceShape;
		ar << m_nPCSlabLevelDifHaunchPlaceType;

		ar << m_bPCSlabOpeningInfoSet;
		ar << m_nPCSlabOpeningTopHookType;
		ar << m_nPCSlabOpeningBotHookType;

		ar << m_bPCSlabRebarAreaInfoSet;
		ar << m_nPCSlabRebarTolDist;
		ar << m_nPCSlabRebarTolLenDif;

		// 철근 일체형 데크 슬래브
		ar << m_bEmbeddedDeckLongMainBarInfoSet;
		ar << m_nEmbeddedDeckLongMaxDualHookLength;
		ar << m_nEmbeddedDeckLongSpliceWay;
		ar << m_nEmbeddedDeckLongSpliceType;
		ar << m_bEmbeddedDeckLongAlterSpliceSet;
		ar << m_nEmbeddedDeckLongAlterSpliceValue1;

		ar << m_bEmbeddedDeckShortMainBarInfoSet;
		ar << m_nEmbeddedDeckShortMaxDualHookLength;
		ar << m_nEmbeddedDeckShortSpliceWay;
		ar << m_nEmbeddedDeckShortSpliceType;
		ar << m_bEmbeddedDeckShortAlterSpliceSet;
		ar << m_nEmbeddedDeckShortAlterSpliceValue1;

		ar << m_bEmbeddedDeckStructuredEndInfoSet;
		ar << m_nEmbeddedDeckTopStructuredEndHookType;
		ar << m_nEmbeddedDeckBotStructuredEndHookType;

		ar << m_bEmbeddedDeckUnStructuredEndInfoSet;
		ar << m_nEmbeddedDeckTopUnStructuredEndHookType;
		ar << m_nEmbeddedDeckBotUnStructuredEndHookType;
		ar << m_nEmbeddedDeckIsoFootUnStructuredDevType;

		ar << m_bEmbeddedDeckLevelDifInfoSet;
		ar << m_nEmbeddedDeckLevelDifPlaceShape;
		ar << m_nEmbeddedDeckLevelDifHaunchPlaceType;

		ar << m_bEmbeddedDeckOpeningInfoSet;
		ar << m_nEmbeddedDeckOpeningTopHookType;
		ar << m_nEmbeddedDeckOpeningBotHookType;

		ar << m_bEmbeddedDeckRebarAreaInfoSet;
		ar << m_nEmbeddedDeckRebarTolDist;
		ar << m_nEmbeddedDeckRebarTolLenDif;

		// 일반 데크 슬래브
		ar << m_bGeneralDeckLongMainBarInfoSet;
		ar << m_nGeneralDeckLongMaxDualHookLength;
		ar << m_nGeneralDeckLongSpliceWay;
		ar << m_nGeneralDeckLongSpliceType;
		ar << m_bGeneralDeckLongAlterSpliceSet;
		ar << m_nGeneralDeckLongAlterSpliceValue1;

		ar << m_bGeneralDeckShortMainBarInfoSet;
		ar << m_nGeneralDeckShortMaxDualHookLength;
		ar << m_nGeneralDeckShortSpliceWay;
		ar << m_nGeneralDeckShortSpliceType;
		ar << m_bGeneralDeckShortAlterSpliceSet;
		ar << m_nGeneralDeckShortAlterSpliceValue1;

		ar << m_bGeneralDeckStructuredEndInfoSet;
		ar << m_nGeneralDeckTopStructuredEndHookType;
		ar << m_nGeneralDeckBotStructuredEndHookType;

		ar << m_bGeneralDeckUnStructuredEndInfoSet;
		ar << m_nGeneralDeckTopUnStructuredEndHookType;
		ar << m_nGeneralDeckBotUnStructuredEndHookType;
		ar << m_nGeneralDeckIsoFootUnStructuredDevType;

		ar << m_bGeneralDeckLevelDifInfoSet;
		ar << m_nGeneralDeckLevelDifPlaceShape;
		ar << m_nGeneralDeckLevelDifHaunchPlaceType;

		ar << m_bGeneralDeckOpeningInfoSet;
		ar << m_nGeneralDeckOpeningTopHookType;
		ar << m_nGeneralDeckOpeningBotHookType;

		ar << m_bGeneralDeckRebarAreaInfoSet;
		ar << m_nGeneralDeckRebarTolDist;
		ar << m_nGeneralDeckRebarTolLenDif;

		ar << m_bMatTopSpacerInfoSet;
		ar << m_bMatTopSpacerSet;
		ar << m_nMatTopSpacerDiaType;
		ar << m_nMatTopSpacerDiaIndex;
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
		ar << m_nFlatTopSpacerDiaIndex;
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
		ar << m_nSlabTopSpacerDiaIndex;
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
		ar << m_nSOGTopSpacerDiaIndex;
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
		ar << m_nCircularRampTopSpacerDiaIndex;
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
		ar >> m_bMatLongAlterSpliceSet;
		ar >> m_nMatLongAlterSpliceValue1;
		ar >> nDefaultValue;
		ar >> bDefaultValue;
		ar >> bDefaultValue;
		ar >> m_bMatShortAlterSpliceSet;
		ar >> m_nMatShortAlterSpliceValue1;
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
		ar >> m_nFlatOpeningTopHookType;
		ar >> m_nFlatOpeningBotHookType;

		ar >> m_bFlatLevelDifInfoSet;
		ar >> nDefaultValue;
		ar >> nDefaultValue;

		ar >> m_bFlatEndInfoSet;
		ar >> m_nFlatTopEndHookType;
		ar >> m_nFlatBotEndHookType;

		ar >> bDefaultValue;
		ar >> m_bFlatLongAlterSpliceSet;
		ar >> m_nFlatLongAlterSpliceValue1;
		ar >> nDefaultValue;
		ar >> bDefaultValue;
		ar >> bDefaultValue;
		ar >> m_bFlatShortAlterSpliceSet;
		ar >> m_nFlatShortAlterSpliceValue1;
		ar >> nDefaultValue;
		ar >> bDefaultValue;
		ar >> bDefaultValue;

		ar >> m_bEmbeddedDeckInfoSet;
		ar >> m_nEmbeddedDeckDifDiaConnectBarType;
		ar >> m_nEmbeddedDeckAnchorType;
		ar >> m_nEmbeddedDeckTopEndRatioType;
		ar >> m_nEmbeddedDeckBotEndRatioType;

		ar >> m_bFlatRebarAreaInfoSet;
		ar >> m_nFlatRebarTolDist;
		ar >> m_nFlatRebarTolLenDif;

		// SLAB
		ar >> bDefaultValue;
		ar >> nDefaultValue;

		ar >> m_bSlabStructuredEndInfoSet;
		ar >> m_nSlabTopStructuredEndHookType;
		ar >> m_nSlabBotStructuredEndHookType;

		ar >> m_bSlabTopCutBarLenInfoSet;
		ar >> m_nSlabOneWayCutBarLenType;
		ar >> m_nSlabTwoWayCutBarLenType;

		if (MSVersionInfo::GetCurrentVersion() >= 20160218)
		{
			// Mat
			ar >> m_nMatLevelDifPlaceShape;
			ar >> m_nMatLevelDifHaunchPlaceType;
			ar >> m_bMatLongMainBarInfoSet;
			ar >> m_nMatLongMaxDualHookLength;
			ar >> m_nMatLongSpliceWay;
			ar >> m_nMatLongSpliceType;
			ar >> m_bMatShortMainBarInfoSet;
			ar >> m_nMatShortMaxDualHookLength;
			ar >> m_nMatShortSpliceWay;
			ar >> m_nMatShortSpliceType;

			// Flat
			ar >> m_nFlatLevelDifPlaceShape;
			ar >> m_nFlatLevelDifHaunchPlaceType;
			ar >> m_bFlatLongMainBarInfoSet;
			ar >> m_nFlatLongMaxDualHookLength;
			ar >> m_nFlatLongSpliceWay;
			ar >> m_nFlatLongSpliceType;
			ar >> m_bFlatShortMainBarInfoSet;
			ar >> m_nFlatShortMaxDualHookLength;
			ar >> m_nFlatShortSpliceWay;
			ar >> m_nFlatShortSpliceType;

			// Slab
			ar >> m_bSlabLongMainBarInfoSet;
			ar >> m_nSlabLongMaxDualHookLength;
			ar >> m_nSlabLongSpliceWay;
			ar >> m_nSlabLongSpliceType;
			ar >> m_bSlabLongAlterSpliceSet;
			ar >> m_nSlabLongAlterSpliceValue1;
			ar >> nDefaultValue;
			ar >> m_bSlabShortMainBarInfoSet;
			ar >> m_nSlabShortMaxDualHookLength;
			ar >> m_nSlabShortSpliceWay;
			ar >> m_nSlabShortSpliceType;
			ar >> m_bSlabShortAlterSpliceSet;
			ar >> m_nSlabShortAlterSpliceValue1;
			ar >> nDefaultValue;
		}

		if (m_nFlatBotEndHookType == 2) //Flat 신규기획안에서 일자(150)이 없어지면서 Index 조정
			m_nFlatBotEndHookType = 1;

		if (MSVersionInfo::GetCurrentVersion() >= 20160419)
		{
			ar >> bDefaultValue;
			ar >> m_nSOGSpliceDevSlabType;
			ar >> m_nFlatSpliceDevSlabType;
			ar >> m_nCircularRampSpliceDevSlabType;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20170403)
		{
			ar >> m_bSlabUnStructuredEndInfoSet;
			ar >> m_nSlabTopUnStructuredEndHookType;
			ar >> m_nSlabBotUnStructuredEndHookType;
			ar >> m_nSlabIsoFootUnStructuredDevType;

			ar >> m_bSlabLevelDifInfoSet;
			ar >> m_nSlabLevelDifPlaceShape;
			ar >> m_nSlabLevelDifHaunchPlaceType;

			ar >> m_bSlabOpeningInfoSet;
			ar >> m_nSlabOpeningTopHookType;
			ar >> m_nSlabOpeningBotHookType;

			ar >> m_bSlabRebarAreaInfoSet;
			ar >> m_nSlabRebarTolDist;
			ar >> m_nSlabRebarTolLenDif;

			// SOG Slab
			ar >> m_bSOGLongMainBarInfoSet;
			ar >> m_nSOGLongMaxDualHookLength;
			ar >> m_nSOGLongSpliceWay;
			ar >> m_nSOGLongSpliceType;
			ar >> m_bSOGLongAlterSpliceSet;
			ar >> m_nSOGLongAlterSpliceValue1;

			ar >> m_bSOGShortMainBarInfoSet;
			ar >> m_nSOGShortMaxDualHookLength;
			ar >> m_nSOGShortSpliceWay;
			ar >> m_nSOGShortSpliceType;
			ar >> m_bSOGShortAlterSpliceSet;
			ar >> m_nSOGShortAlterSpliceValue1;

			ar >> m_bSOGEndInfoSet;
			ar >> m_nSOGTopEndHookType;
			ar >> m_nSOGBotEndHookType;
			ar >> m_nSOGIsoFootDevType;

			ar >> m_bSOGLevelDifInfoSet;
			ar >> m_nSOGLevelDifPlaceShape;
			ar >> m_nSOGLevelDifHaunchPlaceType;

			ar >> m_bSOGOpeningInfoSet;
			ar >> m_nSOGOpeningTopHookType;
			ar >> m_nSOGOpeningBotHookType;

			ar >> m_bSOGRebarAreaInfoSet;
			ar >> m_nSOGRebarTolDist;
			ar >> m_nSOGRebarTolLenDif;

			// CircularRamp
			ar >> m_bCircularRampLongMainBarInfoSet;
			ar >> m_nCircularRampLongMaxDualHookLength;
			ar >> m_nCircularRampLongSpliceWay;
			ar >> m_nCircularRampLongSpliceType;
			ar >> m_bCircularRampLongAlterSpliceSet;
			ar >> m_nCircularRampLongAlterSpliceValue1;

			ar >> m_bCircularRampShortMainBarInfoSet;
			ar >> m_nCircularRampShortMaxDualHookLength;
			ar >> m_nCircularRampShortSpliceWay;
			ar >> m_nCircularRampShortSpliceType;
			ar >> m_bCircularRampShortAlterSpliceSet;
			ar >> m_nCircularRampShortAlterSpliceValue1;

			ar >> m_bCircularRampEndInfoSet;
			ar >> m_nCircularRampTopEndHookType;
			ar >> m_nCircularRampBotEndHookType;
			ar >> m_nCircularRampIsoFootDevType;

			ar >> m_bCircularRampLevelDifInfoSet;
			ar >> m_nCircularRampLevelDifPlaceShape;
			ar >> m_nCircularRampLevelDifHaunchPlaceType;

			ar >> m_bCircularRampOpeningInfoSet;
			ar >> m_nCircularRampOpeningTopHookType;
			ar >> m_nCircularRampOpeningBotHookType;

			ar >> m_bCircularRampRebarAreaInfoSet;
			ar >> m_nCircularRampRebarTolDist;
			ar >> m_nCircularRampRebarTolLenDif;

			// PC Slab
			ar >> m_bPCSlabLongMainBarInfoSet;
			ar >> m_nPCSlabLongMaxDualHookLength;
			ar >> m_nPCSlabLongSpliceWay;
			ar >> m_nPCSlabLongSpliceType;
			ar >> m_bPCSlabLongAlterSpliceSet;
			ar >> m_nPCSlabLongAlterSpliceValue1;

			ar >> m_bPCSlabShortMainBarInfoSet;
			ar >> m_nPCSlabShortMaxDualHookLength;
			ar >> m_nPCSlabShortSpliceWay;
			ar >> m_nPCSlabShortSpliceType;
			ar >> m_bPCSlabShortAlterSpliceSet;
			ar >> m_nPCSlabShortAlterSpliceValue1;

			ar >> m_bPCSlabStructuredEndInfoSet;
			ar >> m_nPCSlabTopStructuredEndHookType;
			ar >> m_nPCSlabBotStructuredEndHookType;

			ar >> m_bPCSlabDeckInfoSet;
			ar >> m_nPCSlabDeckDifDiaConnectBarType;
			ar >> m_nPCSlabDeckAnchorType;
			ar >> m_nPCSlabDeckTopEndRatioType;
			ar >> m_nPCSlabDeckBotEndRatioType;

			ar >> m_bPCSlabUnStructuredEndInfoSet;
			ar >> m_nPCSlabTopUnStructuredEndHookType;
			ar >> m_nPCSlabBotUnStructuredEndHookType;
			ar >> m_nPCSlabIsoFootUnStructuredDevType;

			ar >> m_bPCSlabLevelDifInfoSet;
			ar >> m_nPCSlabLevelDifPlaceShape;
			ar >> m_nPCSlabLevelDifHaunchPlaceType;

			ar >> m_bPCSlabOpeningInfoSet;
			ar >> m_nPCSlabOpeningTopHookType;
			ar >> m_nPCSlabOpeningBotHookType;

			ar >> m_bPCSlabRebarAreaInfoSet;
			ar >> m_nPCSlabRebarTolDist;
			ar >> m_nPCSlabRebarTolLenDif;

			// 철근 일체형 데크 슬래브
			ar >> m_bEmbeddedDeckLongMainBarInfoSet;
			ar >> m_nEmbeddedDeckLongMaxDualHookLength;
			ar >> m_nEmbeddedDeckLongSpliceWay;
			ar >> m_nEmbeddedDeckLongSpliceType;
			ar >> m_bEmbeddedDeckLongAlterSpliceSet;
			ar >> m_nEmbeddedDeckLongAlterSpliceValue1;

			ar >> m_bEmbeddedDeckShortMainBarInfoSet;
			ar >> m_nEmbeddedDeckShortMaxDualHookLength;
			ar >> m_nEmbeddedDeckShortSpliceWay;
			ar >> m_nEmbeddedDeckShortSpliceType;
			ar >> m_bEmbeddedDeckShortAlterSpliceSet;
			ar >> m_nEmbeddedDeckShortAlterSpliceValue1;

			ar >> m_bEmbeddedDeckStructuredEndInfoSet;
			ar >> m_nEmbeddedDeckTopStructuredEndHookType;
			ar >> m_nEmbeddedDeckBotStructuredEndHookType;

			ar >> m_bEmbeddedDeckUnStructuredEndInfoSet;
			ar >> m_nEmbeddedDeckTopUnStructuredEndHookType;
			ar >> m_nEmbeddedDeckBotUnStructuredEndHookType;
			ar >> m_nEmbeddedDeckIsoFootUnStructuredDevType;

			ar >> m_bEmbeddedDeckLevelDifInfoSet;
			ar >> m_nEmbeddedDeckLevelDifPlaceShape;
			ar >> m_nEmbeddedDeckLevelDifHaunchPlaceType;

			ar >> m_bEmbeddedDeckOpeningInfoSet;
			ar >> m_nEmbeddedDeckOpeningTopHookType;
			ar >> m_nEmbeddedDeckOpeningBotHookType;

			ar >> m_bEmbeddedDeckRebarAreaInfoSet;
			ar >> m_nEmbeddedDeckRebarTolDist;
			ar >> m_nEmbeddedDeckRebarTolLenDif;

			// 일반 데크 슬래브
			ar >> m_bGeneralDeckLongMainBarInfoSet;
			ar >> m_nGeneralDeckLongMaxDualHookLength;
			ar >> m_nGeneralDeckLongSpliceWay;
			ar >> m_nGeneralDeckLongSpliceType;
			ar >> m_bGeneralDeckLongAlterSpliceSet;
			ar >> m_nGeneralDeckLongAlterSpliceValue1;

			ar >> m_bGeneralDeckShortMainBarInfoSet;
			ar >> m_nGeneralDeckShortMaxDualHookLength;
			ar >> m_nGeneralDeckShortSpliceWay;
			ar >> m_nGeneralDeckShortSpliceType;
			ar >> m_bGeneralDeckShortAlterSpliceSet;
			ar >> m_nGeneralDeckShortAlterSpliceValue1;

			ar >> m_bGeneralDeckStructuredEndInfoSet;
			ar >> m_nGeneralDeckTopStructuredEndHookType;
			ar >> m_nGeneralDeckBotStructuredEndHookType;

			ar >> m_bGeneralDeckUnStructuredEndInfoSet;
			ar >> m_nGeneralDeckTopUnStructuredEndHookType;
			ar >> m_nGeneralDeckBotUnStructuredEndHookType;
			ar >> m_nGeneralDeckIsoFootUnStructuredDevType;

			ar >> m_bGeneralDeckLevelDifInfoSet;
			ar >> m_nGeneralDeckLevelDifPlaceShape;
			ar >> m_nGeneralDeckLevelDifHaunchPlaceType;

			ar >> m_bGeneralDeckOpeningInfoSet;
			ar >> m_nGeneralDeckOpeningTopHookType;
			ar >> m_nGeneralDeckOpeningBotHookType;

			ar >> m_bGeneralDeckRebarAreaInfoSet;
			ar >> m_nGeneralDeckRebarTolDist;
			ar >> m_nGeneralDeckRebarTolLenDif;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20170602)
		{
			ar >> m_bMatTopSpacerInfoSet;
			ar >> m_bMatTopSpacerSet;
			ar >> m_nMatTopSpacerDiaType;
			ar >> m_nMatTopSpacerDiaIndex;
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
			ar >> m_nFlatTopSpacerDiaIndex;
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
			ar >> m_nSlabTopSpacerDiaIndex;
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
			ar >> m_nSOGTopSpacerDiaIndex;
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
			ar >> m_nCircularRampTopSpacerDiaIndex;
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
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20170621)
		{
			ar >> m_nMatBotSpacerWidth;
			ar >> m_nFlatBotSpacerWidth;
			ar >> m_nSlabBotSpacerWidth;
			ar >> m_nSOGBotSpacerWidth;
			ar >> m_nCircularRampBotSpacerWidth;
			ar >> m_nPCSlabBotSpacerWidth;
		}
	}
}

void OldSlabRebarTemplate::DataInit()
{
	// MAT Slab
	m_bMatLongMainBarInfoSet = false;
	m_nMatLongMaxDualHookLength = 2000;
	m_nMatLongSpliceWay = 0;
	m_nMatLongSpliceType = 0;
	m_bMatLongAlterSpliceSet = false;
	m_nMatLongAlterSpliceValue1 = 300;

	m_bMatShortMainBarInfoSet = false;
	m_nMatShortMaxDualHookLength = 2000;
	m_nMatShortSpliceWay = 0;
	m_nMatShortSpliceType = 0;
	m_bMatShortAlterSpliceSet = false;
	m_nMatShortAlterSpliceValue1 = 300;

	m_bMatEndInfoSet = false;
	m_nMatTopEndHookType = 0;
	m_nMatBotEndHookType = 1;
	m_nMatIsoFootDevType = 1;

	m_bMatLevelDifInfoSet = false;
	m_nMatLevelDifPlaceShape = 0;
	m_nMatLevelDifHaunchPlaceType = 0;

	m_bMatRebarAreaInfoSet = false;
	m_nMatRebarTolDist = 10;
	m_nMatRebarTolLenDif = 10;

	m_bMatTopSpacerInfoSet = false;
	m_bMatTopSpacerSet = false;
	m_nMatTopSpacerDiaType = 0;
	m_nMatTopSpacerDiaIndex = 0;
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

	// FLAT Slab
	m_bFlatLongMainBarInfoSet = false;
	m_nFlatLongMaxDualHookLength = 2000;
	m_nFlatLongSpliceWay = 0;
	m_nFlatLongSpliceType = 0;
	m_bFlatLongAlterSpliceSet = false;
	m_nFlatLongAlterSpliceValue1 = 300;

	m_bFlatShortMainBarInfoSet = false;
	m_nFlatShortMaxDualHookLength = 2000;
	m_nFlatShortSpliceWay = 0;
	m_nFlatShortSpliceType = 0;
	m_bFlatShortAlterSpliceSet = false;
	m_nFlatShortAlterSpliceValue1 = 300;

	m_bFlatEndInfoSet = false;
	m_nFlatTopEndHookType = 0;
	m_nFlatBotEndHookType = 1;
	m_nFlatIsoFootDevType = 1;
	m_nFlatSpliceDevSlabType = 1;

	m_bFlatLevelDifInfoSet = false;
	m_nFlatLevelDifPlaceShape = 0;
	m_nFlatLevelDifHaunchPlaceType = 0;

	m_bFlatOpeningInfoSet = false;
	m_nFlatOpeningTopHookType = 1;
	m_nFlatOpeningBotHookType = 1;

	m_bFlatRebarAreaInfoSet = false;
	m_nFlatRebarTolDist = 10;
	m_nFlatRebarTolLenDif = 10;

	m_bFlatTopSpacerInfoSet = false;
	m_bFlatTopSpacerSet = false;
	m_nFlatTopSpacerDiaType = 0;
	m_nFlatTopSpacerDiaIndex = 0;
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

	// 사변지지 SLAB
	m_bSlabLongMainBarInfoSet = false;
	m_nSlabLongMaxDualHookLength = 2000;
	m_nSlabLongSpliceWay = 0;
	m_nSlabLongSpliceType = 0;
	m_bSlabLongAlterSpliceSet = false;
	m_nSlabLongAlterSpliceValue1 = 300;

	m_bSlabShortMainBarInfoSet = false;
	m_nSlabShortMaxDualHookLength = 2000;
	m_nSlabShortSpliceWay = 0;
	m_nSlabShortSpliceType = 0;
	m_bSlabShortAlterSpliceSet = false;
	m_nSlabShortAlterSpliceValue1 = 300;

	m_bSlabStructuredEndInfoSet = false;
	m_nSlabTopStructuredEndHookType = 0;
	m_nSlabBotStructuredEndHookType = 0;

	m_bSlabTopCutBarLenInfoSet = false;
	m_nSlabOneWayCutBarLenType = 0;
	m_nSlabTwoWayCutBarLenType = 0;

	m_bSlabUnStructuredEndInfoSet = false;
	m_nSlabTopUnStructuredEndHookType = 0;
	m_nSlabBotUnStructuredEndHookType = 1;
	m_nSlabIsoFootUnStructuredDevType = 1;

	m_bSlabLevelDifInfoSet = false;
	m_nSlabLevelDifPlaceShape = 0;
	m_nSlabLevelDifHaunchPlaceType = 0;

	m_bSlabOpeningInfoSet = false;
	m_nSlabOpeningTopHookType = 1;
	m_nSlabOpeningBotHookType = 1;

	m_bSlabRebarAreaInfoSet = false;
	m_nSlabRebarTolDist = 10;
	m_nSlabRebarTolLenDif = 10;

	m_bSlabTopSpacerInfoSet = false;
	m_bSlabTopSpacerSet = false;
	m_nSlabTopSpacerDiaType = 0;
	m_nSlabTopSpacerDiaIndex = 0;
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

	// SOG Slab
	m_bSOGLongMainBarInfoSet = false;
	m_nSOGLongMaxDualHookLength = 2000;
	m_nSOGLongSpliceWay = 0;
	m_nSOGLongSpliceType = 0;
	m_bSOGLongAlterSpliceSet = false;
	m_nSOGLongAlterSpliceValue1 = 300;

	m_bSOGShortMainBarInfoSet = false;
	m_nSOGShortMaxDualHookLength = 2000;
	m_nSOGShortSpliceWay = 0;
	m_nSOGShortSpliceType = 0;
	m_bSOGShortAlterSpliceSet = false;
	m_nSOGShortAlterSpliceValue1 = 300;

	m_bSOGEndInfoSet = false;
	m_nSOGTopEndHookType = 0;
	m_nSOGBotEndHookType = 1;
	m_nSOGIsoFootDevType = 1;
	m_nSOGSpliceDevSlabType = 1;

	m_bSOGLevelDifInfoSet = false;
	m_nSOGLevelDifPlaceShape = 0;
	m_nSOGLevelDifHaunchPlaceType = 0;

	m_bSOGOpeningInfoSet = false;
	m_nSOGOpeningTopHookType = 1;
	m_nSOGOpeningBotHookType = 1;

	m_bSOGRebarAreaInfoSet = false;
	m_nSOGRebarTolDist = 10;
	m_nSOGRebarTolLenDif = 10;
	
	m_bSOGTopSpacerInfoSet = false;
	m_bSOGTopSpacerSet = false;
	m_nSOGTopSpacerDiaType = 0;
	m_nSOGTopSpacerDiaIndex = 0;
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

	// CircularRamp
	m_bCircularRampLongMainBarInfoSet = false;
	m_nCircularRampLongMaxDualHookLength = 2000;
	m_nCircularRampLongSpliceWay = 0;
	m_nCircularRampLongSpliceType = 0;
	m_bCircularRampLongAlterSpliceSet = false;
	m_nCircularRampLongAlterSpliceValue1 = 300;

	m_bCircularRampShortMainBarInfoSet = false;
	m_nCircularRampShortMaxDualHookLength = 2000;
	m_nCircularRampShortSpliceWay = 0;
	m_nCircularRampShortSpliceType = 0;
	m_bCircularRampShortAlterSpliceSet = false;
	m_nCircularRampShortAlterSpliceValue1 = 300;

	m_bCircularRampEndInfoSet = false;
	m_nCircularRampTopEndHookType = 0;
	m_nCircularRampBotEndHookType = 1;
	m_nCircularRampIsoFootDevType = 1;
	m_nCircularRampSpliceDevSlabType = 1;

	m_bCircularRampLevelDifInfoSet = false;
	m_nCircularRampLevelDifPlaceShape = 0;
	m_nCircularRampLevelDifHaunchPlaceType = 0;

	m_bCircularRampOpeningInfoSet = false;
	m_nCircularRampOpeningTopHookType = 1;
	m_nCircularRampOpeningBotHookType = 1;

	m_bCircularRampRebarAreaInfoSet = false;
	m_nCircularRampRebarTolDist = 10;
	m_nCircularRampRebarTolLenDif = 10;

	m_bCircularRampTopSpacerInfoSet = false;
	m_bCircularRampTopSpacerSet = false;
	m_nCircularRampTopSpacerDiaType = 0;
	m_nCircularRampTopSpacerDiaIndex = 0;
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

	// PC Slab
	m_bPCSlabLongMainBarInfoSet = false;
	m_nPCSlabLongMaxDualHookLength = 2000;
	m_nPCSlabLongSpliceWay = 0;
	m_nPCSlabLongSpliceType = 0;
	m_bPCSlabLongAlterSpliceSet = false;
	m_nPCSlabLongAlterSpliceValue1 = 300;

	m_bPCSlabShortMainBarInfoSet = false;
	m_nPCSlabShortMaxDualHookLength = 2000;
	m_nPCSlabShortSpliceWay = 0;
	m_nPCSlabShortSpliceType = 0;
	m_bPCSlabShortAlterSpliceSet = false;
	m_nPCSlabShortAlterSpliceValue1 = 300;

	m_bPCSlabStructuredEndInfoSet = false;
	m_nPCSlabTopStructuredEndHookType = 0;
	m_nPCSlabBotStructuredEndHookType = 0;

	m_bPCSlabDeckInfoSet = false;
	m_nPCSlabDeckDifDiaConnectBarType = 0;
	m_nPCSlabDeckAnchorType = 0;
	m_nPCSlabDeckTopEndRatioType = 0;
	m_nPCSlabDeckBotEndRatioType = 0;

	m_bPCSlabUnStructuredEndInfoSet = false;
	m_nPCSlabTopUnStructuredEndHookType = 0;
	m_nPCSlabBotUnStructuredEndHookType = 1;
	m_nPCSlabIsoFootUnStructuredDevType = 1;

	m_bPCSlabLevelDifInfoSet = false;
	m_nPCSlabLevelDifPlaceShape = 0;
	m_nPCSlabLevelDifHaunchPlaceType = 0;

	m_bPCSlabOpeningInfoSet = false;
	m_nPCSlabOpeningTopHookType = 1;
	m_nPCSlabOpeningBotHookType = 1;

	m_bPCSlabRebarAreaInfoSet = false;
	m_nPCSlabRebarTolDist = 10;
	m_nPCSlabRebarTolLenDif = 10;

	m_bPCSlabTopSpacerInfoSet = false;
	m_bPCSlabTopSpacerSet = false;
	m_nPCSlabTopSpacerDiaType = 0;
	m_nPCSlabTopSpacerDiaIndex = 0;
	m_nPCSlabTopSpacerSpacingType = 1;
	m_nPCSlabTopSpacerSlabSpacing = 3;
	m_nPCSlabTopSpacerUserSpacing1 = 1500;
	m_nPCSlabTopSpacerUserSpacing2 = 1500;
	m_nPCSlabTopSpacerHeightType = 0;
	m_nPCSlabTopSpacerUserHeight = 300;
	m_nPCSlabTopSpacerHookLenAType = 1;
	m_nPCSlabTopSpacerMainBarHookLenA = 100;
	m_nPCSlabTopSpacerUserHookLenA = 100;
	m_nPCSlabTopSpacerHookLenBType = 1;
	m_nPCSlabTopSpacerMainBarHookLenB = 0;
	m_nPCSlabTopSpacerUserHookLenB = 100;
	m_nPCSlabTopSpacerWidth = 200;

	m_bPCSlabBotSpacerInfoSet = false;
	m_bPCSlabBotSpacerSet = false;
	m_nPCSlabBotSpacerDiaType = 0;
	m_nPCSlabBotSpacerDiaIndex = 0;
	m_nPCSlabBotSpacerSpacingType = 1;
	m_nPCSlabBotSpacerSlabSpacing = 3;
	m_nPCSlabBotSpacerUserSpacing1 = 1500;
	m_nPCSlabBotSpacerUserSpacing2 = 1500;
	m_nPCSlabBotSpacerHeightType = 0;
	m_nPCSlabBotSpacerUserHeight = 300;
	m_nPCSlabBotSpacerHookLenAType = 1;
	m_nPCSlabBotSpacerMainBarHookLenA = 100;
	m_nPCSlabBotSpacerUserHookLenA = 100;
	m_nPCSlabBotSpacerHookLenBType = 1;
	m_nPCSlabBotSpacerMainBarHookLenB = 0;
	m_nPCSlabBotSpacerUserHookLenB = 100;
	m_nPCSlabBotSpacerWidth = 200;

	// 철근 일체형 데크 슬래브
	m_bEmbeddedDeckLongMainBarInfoSet = false;
	m_nEmbeddedDeckLongMaxDualHookLength = 2000;
	m_nEmbeddedDeckLongSpliceWay = 0;
	m_nEmbeddedDeckLongSpliceType = 0;
	m_bEmbeddedDeckLongAlterSpliceSet = false;
	m_nEmbeddedDeckLongAlterSpliceValue1 = 300;

	m_bEmbeddedDeckShortMainBarInfoSet = false;
	m_nEmbeddedDeckShortMaxDualHookLength = 2000;
	m_nEmbeddedDeckShortSpliceWay = 0;
	m_nEmbeddedDeckShortSpliceType = 0;
	m_bEmbeddedDeckShortAlterSpliceSet = false;
	m_nEmbeddedDeckShortAlterSpliceValue1 = 300;

	m_bEmbeddedDeckStructuredEndInfoSet = false;
	m_nEmbeddedDeckTopStructuredEndHookType = 0;
	m_nEmbeddedDeckBotStructuredEndHookType = 0;

	m_bEmbeddedDeckInfoSet = false;
	m_nEmbeddedDeckDifDiaConnectBarType = 0;
	m_nEmbeddedDeckAnchorType = 0;
	m_nEmbeddedDeckTopEndRatioType = 0;
	m_nEmbeddedDeckBotEndRatioType = 0;

	m_bEmbeddedDeckUnStructuredEndInfoSet = false;
	m_nEmbeddedDeckTopUnStructuredEndHookType = 0;
	m_nEmbeddedDeckBotUnStructuredEndHookType = 1;
	m_nEmbeddedDeckIsoFootUnStructuredDevType = 1;

	m_bEmbeddedDeckLevelDifInfoSet = false;
	m_nEmbeddedDeckLevelDifPlaceShape = 0;
	m_nEmbeddedDeckLevelDifHaunchPlaceType = 0;

	m_bEmbeddedDeckOpeningInfoSet = false;
	m_nEmbeddedDeckOpeningTopHookType = 1;
	m_nEmbeddedDeckOpeningBotHookType = 1;

	m_bEmbeddedDeckRebarAreaInfoSet = false;
	m_nEmbeddedDeckRebarTolDist = 10;
	m_nEmbeddedDeckRebarTolLenDif = 10;

	// 일반 데크 슬래브
	m_bGeneralDeckLongMainBarInfoSet = false;
	m_nGeneralDeckLongMaxDualHookLength = 2000;
	m_nGeneralDeckLongSpliceWay = 0;
	m_nGeneralDeckLongSpliceType = 0;
	m_bGeneralDeckLongAlterSpliceSet = false;
	m_nGeneralDeckLongAlterSpliceValue1 = 300;

	m_bGeneralDeckShortMainBarInfoSet = false;
	m_nGeneralDeckShortMaxDualHookLength = 2000;
	m_nGeneralDeckShortSpliceWay = 0;
	m_nGeneralDeckShortSpliceType = 0;
	m_bGeneralDeckShortAlterSpliceSet = false;
	m_nGeneralDeckShortAlterSpliceValue1 = 300;

	m_bGeneralDeckStructuredEndInfoSet = false;
	m_nGeneralDeckTopStructuredEndHookType = 0;
	m_nGeneralDeckBotStructuredEndHookType = 0;

	m_bGeneralDeckUnStructuredEndInfoSet = false;
	m_nGeneralDeckTopUnStructuredEndHookType = 0;
	m_nGeneralDeckBotUnStructuredEndHookType = 1;
	m_nGeneralDeckIsoFootUnStructuredDevType = 1;

	m_bGeneralDeckLevelDifInfoSet = false;
	m_nGeneralDeckLevelDifPlaceShape = 0;
	m_nGeneralDeckLevelDifHaunchPlaceType = 0;

	m_bGeneralDeckOpeningInfoSet = false;
	m_nGeneralDeckOpeningTopHookType = 1;
	m_nGeneralDeckOpeningBotHookType = 1;

	m_bGeneralDeckRebarAreaInfoSet = false;
	m_nGeneralDeckRebarTolDist = 10;
	m_nGeneralDeckRebarTolLenDif = 10;
}

bool OldSlabRebarTemplate::TopOptionDataCopy(OldSlabRebarTemplate* rebarTemplate)
{
	bool bDataCopy = false;

	// MAT Slab
	if (!m_bMatLongMainBarInfoSet)
	{
		m_nMatLongMaxDualHookLength = rebarTemplate->m_nMatLongMaxDualHookLength;
		m_nMatLongSpliceWay = rebarTemplate->m_nMatLongSpliceWay;
		m_nMatLongSpliceType = rebarTemplate->m_nMatLongSpliceType;
		m_bMatLongAlterSpliceSet = rebarTemplate->m_bMatLongAlterSpliceSet;
		m_nMatLongAlterSpliceValue1 = rebarTemplate->m_nMatLongAlterSpliceValue1;
		bDataCopy = true;
	}

	if (!m_bMatShortMainBarInfoSet)
	{
		m_nMatShortMaxDualHookLength = rebarTemplate->m_nMatShortMaxDualHookLength;
		m_nMatShortSpliceWay = rebarTemplate->m_nMatShortSpliceWay;
		m_nMatShortSpliceType = rebarTemplate->m_nMatShortSpliceType;
		m_bMatShortAlterSpliceSet = rebarTemplate->m_bMatShortAlterSpliceSet;
		m_nMatShortAlterSpliceValue1 = rebarTemplate->m_nMatShortAlterSpliceValue1;
		bDataCopy = true;
	}

	if (!m_bMatEndInfoSet)
	{
		m_nMatTopEndHookType = rebarTemplate->m_nMatTopEndHookType;
		m_nMatBotEndHookType = rebarTemplate->m_nMatBotEndHookType;
		m_nMatIsoFootDevType = rebarTemplate->m_nMatIsoFootDevType;
		bDataCopy = true;
	}

	if (!m_bMatLevelDifInfoSet)
	{
		m_nMatLevelDifPlaceShape = rebarTemplate->m_nMatLevelDifPlaceShape;
		m_nMatLevelDifHaunchPlaceType = rebarTemplate->m_nMatLevelDifHaunchPlaceType;
		bDataCopy = true;
	}

	if (!m_bMatRebarAreaInfoSet)
	{
		m_nMatRebarTolDist = rebarTemplate->m_nMatRebarTolDist;
		m_nMatRebarTolLenDif = rebarTemplate->m_nMatRebarTolLenDif;
		bDataCopy = true;
	}

	if (!m_bMatTopSpacerInfoSet)
	{
		m_bMatTopSpacerSet = rebarTemplate->m_bMatTopSpacerSet;
		m_nMatTopSpacerDiaType = rebarTemplate->m_nMatTopSpacerDiaType;
		m_nMatTopSpacerDiaIndex = rebarTemplate->m_nMatTopSpacerDiaIndex;
		m_nMatTopSpacerSpacingType = rebarTemplate->m_nMatTopSpacerSpacingType;
		m_nMatTopSpacerSlabSpacing = rebarTemplate->m_nMatTopSpacerSlabSpacing;
		m_nMatTopSpacerUserSpacing1 = rebarTemplate->m_nMatTopSpacerUserSpacing1;
		m_nMatTopSpacerUserSpacing2 = rebarTemplate->m_nMatTopSpacerUserSpacing2;
		m_nMatTopSpacerHeightType = rebarTemplate->m_nMatTopSpacerHeightType;
		m_nMatTopSpacerUserHeight = rebarTemplate->m_nMatTopSpacerUserHeight;
		m_nMatTopSpacerHookLenAType = rebarTemplate->m_nMatTopSpacerHookLenAType;
		m_nMatTopSpacerMainBarHookLenA = rebarTemplate->m_nMatTopSpacerMainBarHookLenA;
		m_nMatTopSpacerUserHookLenA = rebarTemplate->m_nMatTopSpacerUserHookLenA;
		m_nMatTopSpacerHookLenBType = rebarTemplate->m_nMatTopSpacerHookLenBType;
		m_nMatTopSpacerMainBarHookLenB = rebarTemplate->m_nMatTopSpacerMainBarHookLenB;
		m_nMatTopSpacerUserHookLenB = rebarTemplate->m_nMatTopSpacerUserHookLenB;
		m_nMatTopSpacerWidth = rebarTemplate->m_nMatTopSpacerWidth;
	}
	
	if (!m_bMatBotSpacerInfoSet)
	{
		m_bMatBotSpacerSet = rebarTemplate->m_bMatBotSpacerSet;
		m_nMatBotSpacerDiaType = rebarTemplate->m_nMatBotSpacerDiaType;
		m_nMatBotSpacerDiaIndex = rebarTemplate->m_nMatBotSpacerDiaIndex;
		m_nMatBotSpacerSpacingType = rebarTemplate->m_nMatBotSpacerSpacingType;
		m_nMatBotSpacerSlabSpacing = rebarTemplate->m_nMatBotSpacerSlabSpacing;
		m_nMatBotSpacerUserSpacing1 = rebarTemplate->m_nMatBotSpacerUserSpacing1;
		m_nMatBotSpacerUserSpacing2 = rebarTemplate->m_nMatBotSpacerUserSpacing2;
		m_nMatBotSpacerHeightType = rebarTemplate->m_nMatBotSpacerHeightType;
		m_nMatBotSpacerUserHeight = rebarTemplate->m_nMatBotSpacerUserHeight;
		m_nMatBotSpacerHookLenAType = rebarTemplate->m_nMatBotSpacerHookLenAType;
		m_nMatBotSpacerMainBarHookLenA = rebarTemplate->m_nMatBotSpacerMainBarHookLenA;
		m_nMatBotSpacerUserHookLenA = rebarTemplate->m_nMatBotSpacerUserHookLenA;
		m_nMatBotSpacerHookLenBType = rebarTemplate->m_nMatBotSpacerHookLenBType;
		m_nMatBotSpacerMainBarHookLenB = rebarTemplate->m_nMatBotSpacerMainBarHookLenB;
		m_nMatBotSpacerUserHookLenB = rebarTemplate->m_nMatBotSpacerUserHookLenB;
		m_nMatBotSpacerWidth = rebarTemplate->m_nMatBotSpacerWidth;
	}

	// FLAT Slab
	if (!m_bFlatLongMainBarInfoSet)
	{
		m_nFlatLongMaxDualHookLength = rebarTemplate->m_nFlatLongMaxDualHookLength;
		m_nFlatLongSpliceWay = rebarTemplate->m_nFlatLongSpliceWay;
		m_nFlatLongSpliceType = rebarTemplate->m_nFlatLongSpliceType;
		m_bFlatLongAlterSpliceSet = rebarTemplate->m_bFlatLongAlterSpliceSet;
		m_nFlatLongAlterSpliceValue1 = rebarTemplate->m_nFlatLongAlterSpliceValue1;
		bDataCopy = true;
	}

	if (!m_bFlatShortMainBarInfoSet)
	{
		m_nFlatShortMaxDualHookLength = rebarTemplate->m_nFlatShortMaxDualHookLength;
		m_nFlatShortSpliceWay = rebarTemplate->m_nFlatShortSpliceWay;
		m_nFlatShortSpliceType = rebarTemplate->m_nFlatShortSpliceType;
		m_bFlatShortAlterSpliceSet = rebarTemplate->m_bFlatShortAlterSpliceSet;
		m_nFlatShortAlterSpliceValue1 = rebarTemplate->m_nFlatShortAlterSpliceValue1;
		bDataCopy = true;
	}

	if (!m_bFlatEndInfoSet)
	{
		m_nFlatTopEndHookType = rebarTemplate->m_nFlatTopEndHookType;
		m_nFlatBotEndHookType = rebarTemplate->m_nFlatBotEndHookType;
		m_nFlatIsoFootDevType = rebarTemplate->m_nFlatIsoFootDevType;
		m_nFlatSpliceDevSlabType = rebarTemplate->m_nFlatSpliceDevSlabType;
		bDataCopy = true;
	}

	if (!m_bFlatLevelDifInfoSet)
	{
		m_nFlatLevelDifPlaceShape = rebarTemplate->m_nFlatLevelDifPlaceShape;
		m_nFlatLevelDifHaunchPlaceType = rebarTemplate->m_nFlatLevelDifHaunchPlaceType;
		bDataCopy = true;
	}

	if (!m_bFlatOpeningInfoSet)
	{
		m_nFlatOpeningTopHookType = rebarTemplate->m_nFlatOpeningTopHookType;
		m_nFlatOpeningBotHookType = rebarTemplate->m_nFlatOpeningBotHookType;
		bDataCopy = true;
	}

	if (!m_bFlatRebarAreaInfoSet)
	{
		m_nFlatRebarTolDist = rebarTemplate->m_nFlatRebarTolDist;
		m_nFlatRebarTolLenDif = rebarTemplate->m_nFlatRebarTolLenDif;
		bDataCopy = true;
	}

	if (!m_bFlatTopSpacerInfoSet)
	{
		m_bFlatTopSpacerSet = rebarTemplate->m_bFlatTopSpacerSet;
		m_nFlatTopSpacerDiaType = rebarTemplate->m_nFlatTopSpacerDiaType;
		m_nFlatTopSpacerDiaIndex = rebarTemplate->m_nFlatTopSpacerDiaIndex;
		m_nFlatTopSpacerSpacingType = rebarTemplate->m_nFlatTopSpacerSpacingType;
		m_nFlatTopSpacerSlabSpacing = rebarTemplate->m_nFlatTopSpacerSlabSpacing;
		m_nFlatTopSpacerUserSpacing1 = rebarTemplate->m_nFlatTopSpacerUserSpacing1;
		m_nFlatTopSpacerUserSpacing2 = rebarTemplate->m_nFlatTopSpacerUserSpacing2;
		m_nFlatTopSpacerHeightType = rebarTemplate->m_nFlatTopSpacerHeightType;
		m_nFlatTopSpacerUserHeight = rebarTemplate->m_nFlatTopSpacerUserHeight;
		m_nFlatTopSpacerHookLenAType = rebarTemplate->m_nFlatTopSpacerHookLenAType;
		m_nFlatTopSpacerMainBarHookLenA = rebarTemplate->m_nFlatTopSpacerMainBarHookLenA;
		m_nFlatTopSpacerUserHookLenA = rebarTemplate->m_nFlatTopSpacerUserHookLenA;
		m_nFlatTopSpacerHookLenBType = rebarTemplate->m_nFlatTopSpacerHookLenBType;
		m_nFlatTopSpacerMainBarHookLenB = rebarTemplate->m_nFlatTopSpacerMainBarHookLenB;
		m_nFlatTopSpacerUserHookLenB = rebarTemplate->m_nFlatTopSpacerUserHookLenB;
		m_nFlatTopSpacerWidth = rebarTemplate->m_nFlatTopSpacerWidth;
	}

	if (!m_bFlatBotSpacerInfoSet)
	{
		m_bFlatBotSpacerSet = rebarTemplate->m_bFlatBotSpacerSet;
		m_nFlatBotSpacerDiaType = rebarTemplate->m_nFlatBotSpacerDiaType;
		m_nFlatBotSpacerDiaIndex = rebarTemplate->m_nFlatBotSpacerDiaIndex;
		m_nFlatBotSpacerSpacingType = rebarTemplate->m_nFlatBotSpacerSpacingType;
		m_nFlatBotSpacerSlabSpacing = rebarTemplate->m_nFlatBotSpacerSlabSpacing;
		m_nFlatBotSpacerUserSpacing1 = rebarTemplate->m_nFlatBotSpacerUserSpacing1;
		m_nFlatBotSpacerUserSpacing2 = rebarTemplate->m_nFlatBotSpacerUserSpacing2;
		m_nFlatBotSpacerHeightType = rebarTemplate->m_nFlatBotSpacerHeightType;
		m_nFlatBotSpacerUserHeight = rebarTemplate->m_nFlatBotSpacerUserHeight;
		m_nFlatBotSpacerHookLenAType = rebarTemplate->m_nFlatBotSpacerHookLenAType;
		m_nFlatBotSpacerMainBarHookLenA = rebarTemplate->m_nFlatBotSpacerMainBarHookLenA;
		m_nFlatBotSpacerUserHookLenA = rebarTemplate->m_nFlatBotSpacerUserHookLenA;
		m_nFlatBotSpacerHookLenBType = rebarTemplate->m_nFlatBotSpacerHookLenBType;
		m_nFlatBotSpacerMainBarHookLenB = rebarTemplate->m_nFlatBotSpacerMainBarHookLenB;
		m_nFlatBotSpacerUserHookLenB = rebarTemplate->m_nFlatBotSpacerUserHookLenB;
		m_nFlatBotSpacerWidth = rebarTemplate->m_nFlatBotSpacerWidth;
	}

	// 사변지지 SLAB
	if (!m_bSlabLongMainBarInfoSet)
	{
		m_nSlabLongMaxDualHookLength = rebarTemplate->m_nSlabLongMaxDualHookLength;
		m_nSlabLongSpliceWay = rebarTemplate->m_nSlabLongSpliceWay;
		m_nSlabLongSpliceType = rebarTemplate->m_nSlabLongSpliceType;
		m_bSlabLongAlterSpliceSet = rebarTemplate->m_bSlabLongAlterSpliceSet;
		m_nSlabLongAlterSpliceValue1 = rebarTemplate->m_nSlabLongAlterSpliceValue1;
		bDataCopy = true;
	}

	if (!m_bSlabShortMainBarInfoSet)
	{
		m_nSlabShortMaxDualHookLength = rebarTemplate->m_nSlabShortMaxDualHookLength;
		m_nSlabShortSpliceWay = rebarTemplate->m_nSlabShortSpliceWay;
		m_nSlabShortSpliceType = rebarTemplate->m_nSlabShortSpliceType;
		m_bSlabShortAlterSpliceSet = rebarTemplate->m_bSlabShortAlterSpliceSet;
		m_nSlabShortAlterSpliceValue1 = rebarTemplate->m_nSlabShortAlterSpliceValue1;
		bDataCopy = true;
	}

	if (!m_bSlabStructuredEndInfoSet)
	{
		m_nSlabTopStructuredEndHookType = rebarTemplate->m_nSlabTopStructuredEndHookType;
		m_nSlabBotStructuredEndHookType = rebarTemplate->m_nSlabBotStructuredEndHookType;
		bDataCopy = true;
	}

	if (!m_bSlabTopCutBarLenInfoSet)
	{
		m_nSlabOneWayCutBarLenType = rebarTemplate->m_nSlabOneWayCutBarLenType;
		m_nSlabTwoWayCutBarLenType = rebarTemplate->m_nSlabTwoWayCutBarLenType;
		bDataCopy = true;
	}

	if (!m_bSlabUnStructuredEndInfoSet)
	{
		m_nSlabTopUnStructuredEndHookType = rebarTemplate->m_nSlabTopUnStructuredEndHookType;
		m_nSlabBotUnStructuredEndHookType = rebarTemplate->m_nSlabBotUnStructuredEndHookType;
		m_nSlabIsoFootUnStructuredDevType = rebarTemplate->m_nSlabIsoFootUnStructuredDevType;
		bDataCopy = true;
	}

	if (!m_bSlabLevelDifInfoSet)
	{
		m_nSlabLevelDifPlaceShape = rebarTemplate->m_nSlabLevelDifPlaceShape;
		m_nSlabLevelDifHaunchPlaceType = rebarTemplate->m_nSlabLevelDifHaunchPlaceType;
		bDataCopy = true;
	}

	if (!m_bSlabOpeningInfoSet)
	{
		m_nSlabOpeningTopHookType = rebarTemplate->m_nSlabOpeningTopHookType;
		m_nSlabOpeningBotHookType = rebarTemplate->m_nSlabOpeningBotHookType;
		bDataCopy = true;
	}

	if (!m_bSlabRebarAreaInfoSet)
	{
		m_nSlabRebarTolDist = rebarTemplate->m_nSlabRebarTolDist;
		m_nSlabRebarTolLenDif = rebarTemplate->m_nSlabRebarTolLenDif;
		bDataCopy = true;
	}

	if (!m_bSlabTopSpacerInfoSet)
	{
		m_bSlabTopSpacerSet = rebarTemplate->m_bSlabTopSpacerSet;
		m_nSlabTopSpacerDiaType = rebarTemplate->m_nSlabTopSpacerDiaType;
		m_nSlabTopSpacerDiaIndex = rebarTemplate->m_nSlabTopSpacerDiaIndex;
		m_nSlabTopSpacerSpacingType = rebarTemplate->m_nSlabTopSpacerSpacingType;
		m_nSlabTopSpacerSlabSpacing = rebarTemplate->m_nSlabTopSpacerSlabSpacing;
		m_nSlabTopSpacerUserSpacing1 = rebarTemplate->m_nSlabTopSpacerUserSpacing1;
		m_nSlabTopSpacerUserSpacing2 = rebarTemplate->m_nSlabTopSpacerUserSpacing2;
		m_nSlabTopSpacerHeightType = rebarTemplate->m_nSlabTopSpacerHeightType;
		m_nSlabTopSpacerUserHeight = rebarTemplate->m_nSlabTopSpacerUserHeight;
		m_nSlabTopSpacerHookLenAType = rebarTemplate->m_nSlabTopSpacerHookLenAType;
		m_nSlabTopSpacerMainBarHookLenA = rebarTemplate->m_nSlabTopSpacerMainBarHookLenA;
		m_nSlabTopSpacerUserHookLenA = rebarTemplate->m_nSlabTopSpacerUserHookLenA;
		m_nSlabTopSpacerHookLenBType = rebarTemplate->m_nSlabTopSpacerHookLenBType;
		m_nSlabTopSpacerMainBarHookLenB = rebarTemplate->m_nSlabTopSpacerMainBarHookLenB;
		m_nSlabTopSpacerUserHookLenB = rebarTemplate->m_nSlabTopSpacerUserHookLenB;
		m_nSlabTopSpacerWidth = rebarTemplate->m_nSlabTopSpacerWidth;
	}

	if (!m_bSlabBotSpacerInfoSet)
	{
		m_bSlabBotSpacerSet = rebarTemplate->m_bSlabBotSpacerSet;
		m_nSlabBotSpacerDiaType = rebarTemplate->m_nSlabBotSpacerDiaType;
		m_nSlabBotSpacerDiaIndex = rebarTemplate->m_nSlabBotSpacerDiaIndex;
		m_nSlabBotSpacerSpacingType = rebarTemplate->m_nSlabBotSpacerSpacingType;
		m_nSlabBotSpacerSlabSpacing = rebarTemplate->m_nSlabBotSpacerSlabSpacing;
		m_nSlabBotSpacerUserSpacing1 = rebarTemplate->m_nSlabBotSpacerUserSpacing1;
		m_nSlabBotSpacerUserSpacing2 = rebarTemplate->m_nSlabBotSpacerUserSpacing2;
		m_nSlabBotSpacerHeightType = rebarTemplate->m_nSlabBotSpacerHeightType;
		m_nSlabBotSpacerUserHeight = rebarTemplate->m_nSlabBotSpacerUserHeight;
		m_nSlabBotSpacerHookLenAType = rebarTemplate->m_nSlabBotSpacerHookLenAType;
		m_nSlabBotSpacerMainBarHookLenA = rebarTemplate->m_nSlabBotSpacerMainBarHookLenA;
		m_nSlabBotSpacerUserHookLenA = rebarTemplate->m_nSlabBotSpacerUserHookLenA;
		m_nSlabBotSpacerHookLenBType = rebarTemplate->m_nSlabBotSpacerHookLenBType;
		m_nSlabBotSpacerMainBarHookLenB = rebarTemplate->m_nSlabBotSpacerMainBarHookLenB;
		m_nSlabBotSpacerUserHookLenB = rebarTemplate->m_nSlabBotSpacerUserHookLenB;
		m_nSlabBotSpacerWidth = rebarTemplate->m_nSlabBotSpacerWidth;
	}

	// SOG Slab
	if (!m_bSOGLongMainBarInfoSet)
	{
		m_nSOGLongMaxDualHookLength = rebarTemplate->m_nSOGLongMaxDualHookLength;
		m_nSOGLongSpliceWay = rebarTemplate->m_nSOGLongSpliceWay;
		m_nSOGLongSpliceType = rebarTemplate->m_nSOGLongSpliceType;
		m_bSOGLongAlterSpliceSet = rebarTemplate->m_bSOGLongAlterSpliceSet;
		m_nSOGLongAlterSpliceValue1 = rebarTemplate->m_nSOGLongAlterSpliceValue1;
		bDataCopy = true;
	}

	if (!m_bSOGShortMainBarInfoSet)
	{
		m_nSOGShortMaxDualHookLength = rebarTemplate->m_nSOGShortMaxDualHookLength;
		m_nSOGShortSpliceWay = rebarTemplate->m_nSOGShortSpliceWay;
		m_nSOGShortSpliceType = rebarTemplate->m_nSOGShortSpliceType;
		m_bSOGShortAlterSpliceSet = rebarTemplate->m_bSOGShortAlterSpliceSet;
		m_nSOGShortAlterSpliceValue1 = rebarTemplate->m_nSOGShortAlterSpliceValue1;
		bDataCopy = true;
	}

	if (!m_bSOGEndInfoSet)
	{
		m_nSOGTopEndHookType = rebarTemplate->m_nSOGTopEndHookType;
		m_nSOGBotEndHookType = rebarTemplate->m_nSOGBotEndHookType;
		m_nSOGIsoFootDevType = rebarTemplate->m_nSOGIsoFootDevType;
		m_nSOGSpliceDevSlabType = rebarTemplate->m_nSOGSpliceDevSlabType;
		bDataCopy = true;
	}

	if (!m_bSOGLevelDifInfoSet)
	{
		m_nSOGLevelDifPlaceShape = rebarTemplate->m_nSOGLevelDifPlaceShape;
		m_nSOGLevelDifHaunchPlaceType = rebarTemplate->m_nSOGLevelDifHaunchPlaceType;
		bDataCopy = true;
	}

	if (!m_bSOGOpeningInfoSet)
	{
		m_nSOGOpeningTopHookType = rebarTemplate->m_nSOGOpeningTopHookType;
		m_nSOGOpeningBotHookType = rebarTemplate->m_nSOGOpeningBotHookType;
		bDataCopy = true;
	}

	if (!m_bSOGRebarAreaInfoSet)
	{
		m_nSOGRebarTolDist = rebarTemplate->m_nSOGRebarTolDist;
		m_nSOGRebarTolLenDif = rebarTemplate->m_nSOGRebarTolLenDif;
		bDataCopy = true;
	}

	if (!m_bSOGTopSpacerInfoSet)
	{
		m_bSOGTopSpacerSet = rebarTemplate->m_bSOGTopSpacerSet;
		m_nSOGTopSpacerDiaType = rebarTemplate->m_nSOGTopSpacerDiaType;
		m_nSOGTopSpacerDiaIndex = rebarTemplate->m_nSOGTopSpacerDiaIndex;
		m_nSOGTopSpacerSpacingType = rebarTemplate->m_nSOGTopSpacerSpacingType;
		m_nSOGTopSpacerSlabSpacing = rebarTemplate->m_nSOGTopSpacerSlabSpacing;
		m_nSOGTopSpacerUserSpacing1 = rebarTemplate->m_nSOGTopSpacerUserSpacing1;
		m_nSOGTopSpacerUserSpacing2 = rebarTemplate->m_nSOGTopSpacerUserSpacing2;
		m_nSOGTopSpacerHeightType = rebarTemplate->m_nSOGTopSpacerHeightType;
		m_nSOGTopSpacerUserHeight = rebarTemplate->m_nSOGTopSpacerUserHeight;
		m_nSOGTopSpacerHookLenAType = rebarTemplate->m_nSOGTopSpacerHookLenAType;
		m_nSOGTopSpacerMainBarHookLenA = rebarTemplate->m_nSOGTopSpacerMainBarHookLenA;
		m_nSOGTopSpacerUserHookLenA = rebarTemplate->m_nSOGTopSpacerUserHookLenA;
		m_nSOGTopSpacerHookLenBType = rebarTemplate->m_nSOGTopSpacerHookLenBType;
		m_nSOGTopSpacerMainBarHookLenB = rebarTemplate->m_nSOGTopSpacerMainBarHookLenB;
		m_nSOGTopSpacerUserHookLenB = rebarTemplate->m_nSOGTopSpacerUserHookLenB;
		m_nSOGTopSpacerWidth = rebarTemplate->m_nSOGTopSpacerWidth;
	}

	if (!m_bSOGBotSpacerInfoSet)
	{
		m_bSOGBotSpacerSet = rebarTemplate->m_bSOGBotSpacerSet;
		m_nSOGBotSpacerDiaType = rebarTemplate->m_nSOGBotSpacerDiaType;
		m_nSOGBotSpacerDiaIndex = rebarTemplate->m_nSOGBotSpacerDiaIndex;
		m_nSOGBotSpacerSpacingType = rebarTemplate->m_nSOGBotSpacerSpacingType;
		m_nSOGBotSpacerSlabSpacing = rebarTemplate->m_nSOGBotSpacerSlabSpacing;
		m_nSOGBotSpacerUserSpacing1 = rebarTemplate->m_nSOGBotSpacerUserSpacing1;
		m_nSOGBotSpacerUserSpacing2 = rebarTemplate->m_nSOGBotSpacerUserSpacing2;
		m_nSOGBotSpacerHeightType = rebarTemplate->m_nSOGBotSpacerHeightType;
		m_nSOGBotSpacerUserHeight = rebarTemplate->m_nSOGBotSpacerUserHeight;
		m_nSOGBotSpacerHookLenAType = rebarTemplate->m_nSOGBotSpacerHookLenAType;
		m_nSOGBotSpacerMainBarHookLenA = rebarTemplate->m_nSOGBotSpacerMainBarHookLenA;
		m_nSOGBotSpacerUserHookLenA = rebarTemplate->m_nSOGBotSpacerUserHookLenA;
		m_nSOGBotSpacerHookLenBType = rebarTemplate->m_nSOGBotSpacerHookLenBType;
		m_nSOGBotSpacerMainBarHookLenB = rebarTemplate->m_nSOGBotSpacerMainBarHookLenB;
		m_nSOGBotSpacerUserHookLenB = rebarTemplate->m_nSOGBotSpacerUserHookLenB;
		m_nSOGBotSpacerWidth = rebarTemplate->m_nSOGBotSpacerWidth;
	}

	// CircularRamp
	if (!m_bCircularRampLongMainBarInfoSet)
	{
		m_nCircularRampLongMaxDualHookLength = rebarTemplate->m_nCircularRampLongMaxDualHookLength;
		m_nCircularRampLongSpliceWay = rebarTemplate->m_nCircularRampLongSpliceWay;
		m_nCircularRampLongSpliceType = rebarTemplate->m_nCircularRampLongSpliceType;
		m_bCircularRampLongAlterSpliceSet = rebarTemplate->m_bCircularRampLongAlterSpliceSet;
		m_nCircularRampLongAlterSpliceValue1 = rebarTemplate->m_nCircularRampLongAlterSpliceValue1;
		bDataCopy = true;
	}

	if (!m_bCircularRampShortMainBarInfoSet)
	{
		m_nCircularRampShortMaxDualHookLength = rebarTemplate->m_nCircularRampShortMaxDualHookLength;
		m_nCircularRampShortSpliceWay = rebarTemplate->m_nCircularRampShortSpliceWay;
		m_nCircularRampShortSpliceType = rebarTemplate->m_nCircularRampShortSpliceType;
		m_bCircularRampShortAlterSpliceSet = rebarTemplate->m_bCircularRampShortAlterSpliceSet;
		m_nCircularRampShortAlterSpliceValue1 = rebarTemplate->m_nCircularRampShortAlterSpliceValue1;
		bDataCopy = true;
	}

	if (!m_bCircularRampEndInfoSet)
	{
		m_nCircularRampTopEndHookType = rebarTemplate->m_nCircularRampTopEndHookType;
		m_nCircularRampBotEndHookType = rebarTemplate->m_nCircularRampBotEndHookType;
		m_nCircularRampIsoFootDevType = rebarTemplate->m_nCircularRampIsoFootDevType;
		m_nCircularRampSpliceDevSlabType = rebarTemplate->m_nCircularRampSpliceDevSlabType;
		bDataCopy = true;
	}

	if (!m_bCircularRampLevelDifInfoSet)
	{
		m_nCircularRampLevelDifPlaceShape = rebarTemplate->m_nCircularRampLevelDifPlaceShape;
		m_nCircularRampLevelDifHaunchPlaceType = rebarTemplate->m_nCircularRampLevelDifHaunchPlaceType;
		bDataCopy = true;
	}

	if (!m_bCircularRampOpeningInfoSet)
	{
		m_nCircularRampOpeningTopHookType = rebarTemplate->m_nCircularRampOpeningTopHookType;
		m_nCircularRampOpeningBotHookType = rebarTemplate->m_nCircularRampOpeningBotHookType;
		bDataCopy = true;
	}

	if (!m_bCircularRampRebarAreaInfoSet)
	{
		m_nCircularRampRebarTolDist = rebarTemplate->m_nCircularRampRebarTolDist;
		m_nCircularRampRebarTolLenDif = rebarTemplate->m_nCircularRampRebarTolLenDif;
		bDataCopy = true;
	}

	if (!m_bCircularRampTopSpacerInfoSet)
	{
		m_bCircularRampTopSpacerSet = rebarTemplate->m_bCircularRampTopSpacerSet;
		m_nCircularRampTopSpacerDiaType = rebarTemplate->m_nCircularRampTopSpacerDiaType;
		m_nCircularRampTopSpacerDiaIndex = rebarTemplate->m_nCircularRampTopSpacerDiaIndex;
		m_nCircularRampTopSpacerSpacingType = rebarTemplate->m_nCircularRampTopSpacerSpacingType;
		m_nCircularRampTopSpacerSlabSpacing = rebarTemplate->m_nCircularRampTopSpacerSlabSpacing;
		m_nCircularRampTopSpacerUserSpacing1 = rebarTemplate->m_nCircularRampTopSpacerUserSpacing1;
		m_nCircularRampTopSpacerUserSpacing2 = rebarTemplate->m_nCircularRampTopSpacerUserSpacing2;
		m_nCircularRampTopSpacerHeightType = rebarTemplate->m_nCircularRampTopSpacerHeightType;
		m_nCircularRampTopSpacerUserHeight = rebarTemplate->m_nCircularRampTopSpacerUserHeight;
		m_nCircularRampTopSpacerHookLenAType = rebarTemplate->m_nCircularRampTopSpacerHookLenAType;
		m_nCircularRampTopSpacerMainBarHookLenA = rebarTemplate->m_nCircularRampTopSpacerMainBarHookLenA;
		m_nCircularRampTopSpacerUserHookLenA = rebarTemplate->m_nCircularRampTopSpacerUserHookLenA;
		m_nCircularRampTopSpacerHookLenBType = rebarTemplate->m_nCircularRampTopSpacerHookLenBType;
		m_nCircularRampTopSpacerMainBarHookLenB = rebarTemplate->m_nCircularRampTopSpacerMainBarHookLenB;
		m_nCircularRampTopSpacerUserHookLenB = rebarTemplate->m_nCircularRampTopSpacerUserHookLenB;
		m_nCircularRampTopSpacerWidth = rebarTemplate->m_nCircularRampTopSpacerWidth;
	}

	if (!m_bCircularRampBotSpacerInfoSet)
	{
		m_bCircularRampBotSpacerSet = rebarTemplate->m_bCircularRampBotSpacerSet;
		m_nCircularRampBotSpacerDiaType = rebarTemplate->m_nCircularRampBotSpacerDiaType;
		m_nCircularRampBotSpacerDiaIndex = rebarTemplate->m_nCircularRampBotSpacerDiaIndex;
		m_nCircularRampBotSpacerSpacingType = rebarTemplate->m_nCircularRampBotSpacerSpacingType;
		m_nCircularRampBotSpacerSlabSpacing = rebarTemplate->m_nCircularRampBotSpacerSlabSpacing;
		m_nCircularRampBotSpacerUserSpacing1 = rebarTemplate->m_nCircularRampBotSpacerUserSpacing1;
		m_nCircularRampBotSpacerUserSpacing2 = rebarTemplate->m_nCircularRampBotSpacerUserSpacing2;
		m_nCircularRampBotSpacerHeightType = rebarTemplate->m_nCircularRampBotSpacerHeightType;
		m_nCircularRampBotSpacerUserHeight = rebarTemplate->m_nCircularRampBotSpacerUserHeight;
		m_nCircularRampBotSpacerHookLenAType = rebarTemplate->m_nCircularRampBotSpacerHookLenAType;
		m_nCircularRampBotSpacerMainBarHookLenA = rebarTemplate->m_nCircularRampBotSpacerMainBarHookLenA;
		m_nCircularRampBotSpacerUserHookLenA = rebarTemplate->m_nCircularRampBotSpacerUserHookLenA;
		m_nCircularRampBotSpacerHookLenBType = rebarTemplate->m_nCircularRampBotSpacerHookLenBType;
		m_nCircularRampBotSpacerMainBarHookLenB = rebarTemplate->m_nCircularRampBotSpacerMainBarHookLenB;
		m_nCircularRampBotSpacerUserHookLenB = rebarTemplate->m_nCircularRampBotSpacerUserHookLenB;
		m_nCircularRampBotSpacerWidth = rebarTemplate->m_nCircularRampBotSpacerWidth;
	}

	// PC Slab
	if (!m_bPCSlabLongMainBarInfoSet)
	{
		m_nPCSlabLongMaxDualHookLength = rebarTemplate->m_nPCSlabLongMaxDualHookLength;
		m_nPCSlabLongSpliceWay = rebarTemplate->m_nPCSlabLongSpliceWay;
		m_nPCSlabLongSpliceType = rebarTemplate->m_nPCSlabLongSpliceType;
		m_bPCSlabLongAlterSpliceSet = rebarTemplate->m_bPCSlabLongAlterSpliceSet;
		m_nPCSlabLongAlterSpliceValue1 = rebarTemplate->m_nPCSlabLongAlterSpliceValue1;
		bDataCopy = true;
	}

	if (!m_bPCSlabShortMainBarInfoSet)
	{
		m_nPCSlabShortMaxDualHookLength = rebarTemplate->m_nPCSlabShortMaxDualHookLength;
		m_nPCSlabShortSpliceWay = rebarTemplate->m_nPCSlabShortSpliceWay;
		m_nPCSlabShortSpliceType = rebarTemplate->m_nPCSlabShortSpliceType;
		m_bPCSlabShortAlterSpliceSet = rebarTemplate->m_bPCSlabShortAlterSpliceSet;
		m_nPCSlabShortAlterSpliceValue1 = rebarTemplate->m_nPCSlabShortAlterSpliceValue1;
		bDataCopy = true;
	}

	if (!m_bPCSlabStructuredEndInfoSet)
	{
		m_nPCSlabTopStructuredEndHookType = rebarTemplate->m_nPCSlabTopStructuredEndHookType;
		m_nPCSlabBotStructuredEndHookType = rebarTemplate->m_nPCSlabBotStructuredEndHookType;
		bDataCopy = true;
	}

	if (!m_bPCSlabDeckInfoSet)
	{
		m_nPCSlabDeckDifDiaConnectBarType = rebarTemplate->m_nPCSlabDeckDifDiaConnectBarType;
		m_nPCSlabDeckAnchorType = rebarTemplate->m_nPCSlabDeckAnchorType;
		m_nPCSlabDeckTopEndRatioType = rebarTemplate->m_nPCSlabDeckTopEndRatioType;
		m_nPCSlabDeckBotEndRatioType = rebarTemplate->m_nPCSlabDeckBotEndRatioType;
		bDataCopy = true;
	}

	if (!m_bPCSlabUnStructuredEndInfoSet)
	{
		m_nPCSlabTopUnStructuredEndHookType = rebarTemplate->m_nPCSlabTopUnStructuredEndHookType;
		m_nPCSlabBotUnStructuredEndHookType = rebarTemplate->m_nPCSlabBotUnStructuredEndHookType;
		m_nPCSlabIsoFootUnStructuredDevType = rebarTemplate->m_nPCSlabIsoFootUnStructuredDevType;
		bDataCopy = true;
	}

	if (!m_bPCSlabLevelDifInfoSet)
	{
		m_nPCSlabLevelDifPlaceShape = rebarTemplate->m_nPCSlabLevelDifPlaceShape;
		m_nPCSlabLevelDifHaunchPlaceType = rebarTemplate->m_nPCSlabLevelDifHaunchPlaceType;
		bDataCopy = true;
	}

	if (!m_bPCSlabOpeningInfoSet)
	{
		m_nPCSlabOpeningTopHookType = rebarTemplate->m_nPCSlabOpeningTopHookType;
		m_nPCSlabOpeningBotHookType = rebarTemplate->m_nPCSlabOpeningBotHookType;
		bDataCopy = true;
	}

	if (!m_bPCSlabRebarAreaInfoSet)
	{
		m_nPCSlabRebarTolDist = rebarTemplate->m_nPCSlabRebarTolDist;
		m_nPCSlabRebarTolLenDif = rebarTemplate->m_nPCSlabRebarTolLenDif;
		bDataCopy = true;
	}

	if (!m_bPCSlabTopSpacerInfoSet)
	{
		m_bPCSlabTopSpacerSet = rebarTemplate->m_bPCSlabTopSpacerSet;
		m_nPCSlabTopSpacerDiaType = rebarTemplate->m_nPCSlabTopSpacerDiaType;
		m_nPCSlabTopSpacerDiaIndex = rebarTemplate->m_nPCSlabTopSpacerDiaIndex;
		m_nPCSlabTopSpacerSpacingType = rebarTemplate->m_nPCSlabTopSpacerSpacingType;
		m_nPCSlabTopSpacerSlabSpacing = rebarTemplate->m_nPCSlabTopSpacerSlabSpacing;
		m_nPCSlabTopSpacerUserSpacing1 = rebarTemplate->m_nPCSlabTopSpacerUserSpacing1;
		m_nPCSlabTopSpacerUserSpacing2 = rebarTemplate->m_nPCSlabTopSpacerUserSpacing2;
		m_nPCSlabTopSpacerHeightType = rebarTemplate->m_nPCSlabTopSpacerHeightType;
		m_nPCSlabTopSpacerUserHeight = rebarTemplate->m_nPCSlabTopSpacerUserHeight;
		m_nPCSlabTopSpacerHookLenAType = rebarTemplate->m_nPCSlabTopSpacerHookLenAType;
		m_nPCSlabTopSpacerMainBarHookLenA = rebarTemplate->m_nPCSlabTopSpacerMainBarHookLenA;
		m_nPCSlabTopSpacerUserHookLenA = rebarTemplate->m_nPCSlabTopSpacerUserHookLenA;
		m_nPCSlabTopSpacerHookLenBType = rebarTemplate->m_nPCSlabTopSpacerHookLenBType;
		m_nPCSlabTopSpacerMainBarHookLenB = rebarTemplate->m_nPCSlabTopSpacerMainBarHookLenB;
		m_nPCSlabTopSpacerUserHookLenB = rebarTemplate->m_nPCSlabTopSpacerUserHookLenB;
		m_nPCSlabTopSpacerWidth = rebarTemplate->m_nPCSlabTopSpacerWidth;
	}

	if (!m_bPCSlabBotSpacerInfoSet)
	{
		m_bPCSlabBotSpacerSet = rebarTemplate->m_bPCSlabBotSpacerSet;
		m_nPCSlabBotSpacerDiaType = rebarTemplate->m_nPCSlabBotSpacerDiaType;
		m_nPCSlabBotSpacerDiaIndex = rebarTemplate->m_nPCSlabBotSpacerDiaIndex;
		m_nPCSlabBotSpacerSpacingType = rebarTemplate->m_nPCSlabBotSpacerSpacingType;
		m_nPCSlabBotSpacerSlabSpacing = rebarTemplate->m_nPCSlabBotSpacerSlabSpacing;
		m_nPCSlabBotSpacerUserSpacing1 = rebarTemplate->m_nPCSlabBotSpacerUserSpacing1;
		m_nPCSlabBotSpacerUserSpacing2 = rebarTemplate->m_nPCSlabBotSpacerUserSpacing2;
		m_nPCSlabBotSpacerHeightType = rebarTemplate->m_nPCSlabBotSpacerHeightType;
		m_nPCSlabBotSpacerUserHeight = rebarTemplate->m_nPCSlabBotSpacerUserHeight;
		m_nPCSlabBotSpacerHookLenAType = rebarTemplate->m_nPCSlabBotSpacerHookLenAType;
		m_nPCSlabBotSpacerMainBarHookLenA = rebarTemplate->m_nPCSlabBotSpacerMainBarHookLenA;
		m_nPCSlabBotSpacerUserHookLenA = rebarTemplate->m_nPCSlabBotSpacerUserHookLenA;
		m_nPCSlabBotSpacerHookLenBType = rebarTemplate->m_nPCSlabBotSpacerHookLenBType;
		m_nPCSlabBotSpacerMainBarHookLenB = rebarTemplate->m_nPCSlabBotSpacerMainBarHookLenB;
		m_nPCSlabBotSpacerUserHookLenB = rebarTemplate->m_nPCSlabBotSpacerUserHookLenB;
		m_nPCSlabBotSpacerWidth = rebarTemplate->m_nPCSlabBotSpacerWidth;
	}

	// 철근 일체형 데크 슬래브
	if (!m_bEmbeddedDeckLongMainBarInfoSet)
	{
		m_nEmbeddedDeckLongMaxDualHookLength = rebarTemplate->m_nEmbeddedDeckLongMaxDualHookLength;
		m_nEmbeddedDeckLongSpliceWay = rebarTemplate->m_nEmbeddedDeckLongSpliceWay;
		m_nEmbeddedDeckLongSpliceType = rebarTemplate->m_nEmbeddedDeckLongSpliceType;
		m_bEmbeddedDeckLongAlterSpliceSet = rebarTemplate->m_bEmbeddedDeckLongAlterSpliceSet;
		m_nEmbeddedDeckLongAlterSpliceValue1 = rebarTemplate->m_nEmbeddedDeckLongAlterSpliceValue1;
		bDataCopy = true;
	}

	if (!m_bEmbeddedDeckShortMainBarInfoSet)
	{
		m_nEmbeddedDeckShortMaxDualHookLength = rebarTemplate->m_nEmbeddedDeckShortMaxDualHookLength;
		m_nEmbeddedDeckShortSpliceWay = rebarTemplate->m_nEmbeddedDeckShortSpliceWay;
		m_nEmbeddedDeckShortSpliceType = rebarTemplate->m_nEmbeddedDeckShortSpliceType;
		m_bEmbeddedDeckShortAlterSpliceSet = rebarTemplate->m_bEmbeddedDeckShortAlterSpliceSet;
		m_nEmbeddedDeckShortAlterSpliceValue1 = rebarTemplate->m_nEmbeddedDeckShortAlterSpliceValue1;
		bDataCopy = true;
	}

	if (!m_bEmbeddedDeckStructuredEndInfoSet)
	{
		m_nEmbeddedDeckTopStructuredEndHookType = rebarTemplate->m_nEmbeddedDeckTopStructuredEndHookType;
		m_nEmbeddedDeckBotStructuredEndHookType = rebarTemplate->m_nEmbeddedDeckBotStructuredEndHookType;
		bDataCopy = true;
	}

	if (!m_bEmbeddedDeckInfoSet)
	{
		m_nEmbeddedDeckDifDiaConnectBarType = rebarTemplate->m_nEmbeddedDeckDifDiaConnectBarType;
		m_nEmbeddedDeckAnchorType = rebarTemplate->m_nEmbeddedDeckAnchorType;
		m_nEmbeddedDeckTopEndRatioType = rebarTemplate->m_nEmbeddedDeckTopEndRatioType;
		m_nEmbeddedDeckBotEndRatioType = rebarTemplate->m_nEmbeddedDeckBotEndRatioType;
		bDataCopy = true;
	}

	if (!m_bEmbeddedDeckUnStructuredEndInfoSet)
	{
		m_nEmbeddedDeckTopUnStructuredEndHookType = rebarTemplate->m_nEmbeddedDeckTopUnStructuredEndHookType;
		m_nEmbeddedDeckBotUnStructuredEndHookType = rebarTemplate->m_nEmbeddedDeckBotUnStructuredEndHookType;
		m_nEmbeddedDeckIsoFootUnStructuredDevType = rebarTemplate->m_nEmbeddedDeckIsoFootUnStructuredDevType;
		bDataCopy = true;
	}

	if (!m_bEmbeddedDeckLevelDifInfoSet)
	{
		m_nEmbeddedDeckLevelDifPlaceShape = rebarTemplate->m_nEmbeddedDeckLevelDifPlaceShape;
		m_nEmbeddedDeckLevelDifHaunchPlaceType = rebarTemplate->m_nEmbeddedDeckLevelDifHaunchPlaceType;
		bDataCopy = true;
	}

	if (!m_bEmbeddedDeckOpeningInfoSet)
	{
		m_nEmbeddedDeckOpeningTopHookType = rebarTemplate->m_nEmbeddedDeckOpeningTopHookType;
		m_nEmbeddedDeckOpeningBotHookType = rebarTemplate->m_nEmbeddedDeckOpeningBotHookType;
		bDataCopy = true;
	}

	if (!m_bEmbeddedDeckRebarAreaInfoSet)
	{
		m_nEmbeddedDeckRebarTolDist = rebarTemplate->m_nEmbeddedDeckRebarTolDist;
		m_nEmbeddedDeckRebarTolLenDif = rebarTemplate->m_nEmbeddedDeckRebarTolLenDif;
		bDataCopy = true;
	}

	// 일반 데크 슬래브
	if (!m_bGeneralDeckLongMainBarInfoSet)
	{
		m_nGeneralDeckLongMaxDualHookLength = rebarTemplate->m_nGeneralDeckLongMaxDualHookLength;
		m_nGeneralDeckLongSpliceWay = rebarTemplate->m_nGeneralDeckLongSpliceWay;
		m_nGeneralDeckLongSpliceType = rebarTemplate->m_nGeneralDeckLongSpliceType;
		m_bGeneralDeckLongAlterSpliceSet = rebarTemplate->m_bGeneralDeckLongAlterSpliceSet;
		m_nGeneralDeckLongAlterSpliceValue1 = rebarTemplate->m_nGeneralDeckLongAlterSpliceValue1;
		bDataCopy = true;
	}

	if (!m_bGeneralDeckShortMainBarInfoSet)
	{
		m_nGeneralDeckShortMaxDualHookLength = rebarTemplate->m_nGeneralDeckShortMaxDualHookLength;
		m_nGeneralDeckShortSpliceWay = rebarTemplate->m_nGeneralDeckShortSpliceWay;
		m_nGeneralDeckShortSpliceType = rebarTemplate->m_nGeneralDeckShortSpliceType;
		m_bGeneralDeckShortAlterSpliceSet = rebarTemplate->m_bGeneralDeckShortAlterSpliceSet;
		m_nGeneralDeckShortAlterSpliceValue1 = rebarTemplate->m_nGeneralDeckShortAlterSpliceValue1;
		bDataCopy = true;
	}

	if (!m_bGeneralDeckStructuredEndInfoSet)
	{
		m_nGeneralDeckTopStructuredEndHookType = rebarTemplate->m_nGeneralDeckTopStructuredEndHookType;
		m_nGeneralDeckBotStructuredEndHookType = rebarTemplate->m_nGeneralDeckBotStructuredEndHookType;
		bDataCopy = true;
	}

	if (!m_bGeneralDeckUnStructuredEndInfoSet)
	{
		m_nGeneralDeckTopUnStructuredEndHookType = rebarTemplate->m_nGeneralDeckTopUnStructuredEndHookType;
		m_nGeneralDeckBotUnStructuredEndHookType = rebarTemplate->m_nGeneralDeckBotUnStructuredEndHookType;
		m_nGeneralDeckIsoFootUnStructuredDevType = rebarTemplate->m_nGeneralDeckIsoFootUnStructuredDevType;
		bDataCopy = true;
	}

	if (!m_bGeneralDeckLevelDifInfoSet)
	{
		m_nGeneralDeckLevelDifPlaceShape = rebarTemplate->m_nGeneralDeckLevelDifPlaceShape;
		m_nGeneralDeckLevelDifHaunchPlaceType = rebarTemplate->m_nGeneralDeckLevelDifHaunchPlaceType;
		bDataCopy = true;
	}

	if (!m_bGeneralDeckOpeningInfoSet)
	{
		m_nGeneralDeckOpeningTopHookType = rebarTemplate->m_nGeneralDeckOpeningTopHookType;
		m_nGeneralDeckOpeningBotHookType = rebarTemplate->m_nGeneralDeckOpeningBotHookType;
		bDataCopy = true;
	}

	if (!m_bGeneralDeckRebarAreaInfoSet)
	{
		m_nGeneralDeckRebarTolDist = rebarTemplate->m_nGeneralDeckRebarTolDist;
		m_nGeneralDeckRebarTolLenDif = rebarTemplate->m_nGeneralDeckRebarTolLenDif;
		bDataCopy = true;
	}

	return bDataCopy;
}

void OldSlabRebarTemplate::DataCopy(OldSlabRebarTemplate* rebarTemplate)
{
	// MAT Slab
	m_nMatLongMaxDualHookLength = rebarTemplate->m_nMatLongMaxDualHookLength;
	m_nMatLongSpliceWay = rebarTemplate->m_nMatLongSpliceWay;
	m_nMatLongSpliceType = rebarTemplate->m_nMatLongSpliceType;
	m_bMatLongAlterSpliceSet = rebarTemplate->m_bMatLongAlterSpliceSet;
	m_nMatLongAlterSpliceValue1 = rebarTemplate->m_nMatLongAlterSpliceValue1;

	m_nMatShortMaxDualHookLength = rebarTemplate->m_nMatShortMaxDualHookLength;
	m_nMatShortSpliceWay = rebarTemplate->m_nMatShortSpliceWay;
	m_nMatShortSpliceType = rebarTemplate->m_nMatShortSpliceType;
	m_bMatShortAlterSpliceSet = rebarTemplate->m_bMatShortAlterSpliceSet;
	m_nMatShortAlterSpliceValue1 = rebarTemplate->m_nMatShortAlterSpliceValue1;

	m_nMatTopEndHookType = rebarTemplate->m_nMatTopEndHookType;
	m_nMatBotEndHookType = rebarTemplate->m_nMatBotEndHookType;
	m_nMatIsoFootDevType = rebarTemplate->m_nMatIsoFootDevType;

	m_nMatLevelDifPlaceShape = rebarTemplate->m_nMatLevelDifPlaceShape;
	m_nMatLevelDifHaunchPlaceType = rebarTemplate->m_nMatLevelDifHaunchPlaceType;

	m_nMatRebarTolDist = rebarTemplate->m_nMatRebarTolDist;
	m_nMatRebarTolLenDif = rebarTemplate->m_nMatRebarTolLenDif;

	m_bMatTopSpacerSet = rebarTemplate->m_bMatTopSpacerSet;
	m_nMatTopSpacerDiaType = rebarTemplate->m_nMatTopSpacerDiaType;
	m_nMatTopSpacerDiaIndex = rebarTemplate->m_nMatTopSpacerDiaIndex;
	m_nMatTopSpacerSpacingType = rebarTemplate->m_nMatTopSpacerSpacingType;
	m_nMatTopSpacerSlabSpacing = rebarTemplate->m_nMatTopSpacerSlabSpacing;
	m_nMatTopSpacerUserSpacing1 = rebarTemplate->m_nMatTopSpacerUserSpacing1;
	m_nMatTopSpacerUserSpacing2 = rebarTemplate->m_nMatTopSpacerUserSpacing2;
	m_nMatTopSpacerHeightType = rebarTemplate->m_nMatTopSpacerHeightType;
	m_nMatTopSpacerUserHeight = rebarTemplate->m_nMatTopSpacerUserHeight;
	m_nMatTopSpacerHookLenAType = rebarTemplate->m_nMatTopSpacerHookLenAType;
	m_nMatTopSpacerMainBarHookLenA = rebarTemplate->m_nMatTopSpacerMainBarHookLenA;
	m_nMatTopSpacerUserHookLenA = rebarTemplate->m_nMatTopSpacerUserHookLenA;
	m_nMatTopSpacerHookLenBType = rebarTemplate->m_nMatTopSpacerHookLenBType;
	m_nMatTopSpacerMainBarHookLenB = rebarTemplate->m_nMatTopSpacerMainBarHookLenB;
	m_nMatTopSpacerUserHookLenB = rebarTemplate->m_nMatTopSpacerUserHookLenB;
	m_nMatTopSpacerWidth = rebarTemplate->m_nMatTopSpacerWidth;

	m_bMatBotSpacerSet = rebarTemplate->m_bMatBotSpacerSet;
	m_nMatBotSpacerDiaType = rebarTemplate->m_nMatBotSpacerDiaType;
	m_nMatBotSpacerDiaIndex = rebarTemplate->m_nMatBotSpacerDiaIndex;
	m_nMatBotSpacerSpacingType = rebarTemplate->m_nMatBotSpacerSpacingType;
	m_nMatBotSpacerSlabSpacing = rebarTemplate->m_nMatBotSpacerSlabSpacing;
	m_nMatBotSpacerUserSpacing1 = rebarTemplate->m_nMatBotSpacerUserSpacing1;
	m_nMatBotSpacerUserSpacing2 = rebarTemplate->m_nMatBotSpacerUserSpacing2;
	m_nMatBotSpacerHeightType = rebarTemplate->m_nMatBotSpacerHeightType;
	m_nMatBotSpacerUserHeight = rebarTemplate->m_nMatBotSpacerUserHeight;
	m_nMatBotSpacerHookLenAType = rebarTemplate->m_nMatBotSpacerHookLenAType;
	m_nMatBotSpacerMainBarHookLenA = rebarTemplate->m_nMatBotSpacerMainBarHookLenA;
	m_nMatBotSpacerUserHookLenA = rebarTemplate->m_nMatBotSpacerUserHookLenA;
	m_nMatBotSpacerHookLenBType = rebarTemplate->m_nMatBotSpacerHookLenBType;
	m_nMatBotSpacerMainBarHookLenB = rebarTemplate->m_nMatBotSpacerMainBarHookLenB;
	m_nMatBotSpacerUserHookLenB = rebarTemplate->m_nMatBotSpacerUserHookLenB;
	m_nMatBotSpacerWidth = rebarTemplate->m_nMatBotSpacerWidth;

	// FLAT Slab
	m_nFlatLongMaxDualHookLength = rebarTemplate->m_nFlatLongMaxDualHookLength;
	m_nFlatLongSpliceWay = rebarTemplate->m_nFlatLongSpliceWay;
	m_nFlatLongSpliceType = rebarTemplate->m_nFlatLongSpliceType;
	m_bFlatLongAlterSpliceSet = rebarTemplate->m_bFlatLongAlterSpliceSet;
	m_nFlatLongAlterSpliceValue1 = rebarTemplate->m_nFlatLongAlterSpliceValue1;

	m_nFlatShortMaxDualHookLength = rebarTemplate->m_nFlatShortMaxDualHookLength;
	m_nFlatShortSpliceWay = rebarTemplate->m_nFlatShortSpliceWay;
	m_nFlatShortSpliceType = rebarTemplate->m_nFlatShortSpliceType;
	m_bFlatShortAlterSpliceSet = rebarTemplate->m_bFlatShortAlterSpliceSet;
	m_nFlatShortAlterSpliceValue1 = rebarTemplate->m_nFlatShortAlterSpliceValue1;

	m_nFlatTopEndHookType = rebarTemplate->m_nFlatTopEndHookType;
	m_nFlatBotEndHookType = rebarTemplate->m_nFlatBotEndHookType;
	m_nFlatIsoFootDevType = rebarTemplate->m_nFlatIsoFootDevType;
	m_nFlatSpliceDevSlabType = rebarTemplate->m_nFlatSpliceDevSlabType;

	m_nFlatLevelDifPlaceShape = rebarTemplate->m_nFlatLevelDifPlaceShape;
	m_nFlatLevelDifHaunchPlaceType = rebarTemplate->m_nFlatLevelDifHaunchPlaceType;

	m_nFlatOpeningTopHookType = rebarTemplate->m_nFlatOpeningTopHookType;
	m_nFlatOpeningBotHookType = rebarTemplate->m_nFlatOpeningBotHookType;

	m_nFlatRebarTolDist = rebarTemplate->m_nFlatRebarTolDist;
	m_nFlatRebarTolLenDif = rebarTemplate->m_nFlatRebarTolLenDif;

	m_bFlatTopSpacerSet = rebarTemplate->m_bFlatTopSpacerSet;
	m_nFlatTopSpacerDiaType = rebarTemplate->m_nFlatTopSpacerDiaType;
	m_nFlatTopSpacerDiaIndex = rebarTemplate->m_nFlatTopSpacerDiaIndex;
	m_nFlatTopSpacerSpacingType = rebarTemplate->m_nFlatTopSpacerSpacingType;
	m_nFlatTopSpacerSlabSpacing = rebarTemplate->m_nFlatTopSpacerSlabSpacing;
	m_nFlatTopSpacerUserSpacing1 = rebarTemplate->m_nFlatTopSpacerUserSpacing1;
	m_nFlatTopSpacerUserSpacing2 = rebarTemplate->m_nFlatTopSpacerUserSpacing2;
	m_nFlatTopSpacerHeightType = rebarTemplate->m_nFlatTopSpacerHeightType;
	m_nFlatTopSpacerUserHeight = rebarTemplate->m_nFlatTopSpacerUserHeight;
	m_nFlatTopSpacerHookLenAType = rebarTemplate->m_nFlatTopSpacerHookLenAType;
	m_nFlatTopSpacerMainBarHookLenA = rebarTemplate->m_nFlatTopSpacerMainBarHookLenA;
	m_nFlatTopSpacerUserHookLenA = rebarTemplate->m_nFlatTopSpacerUserHookLenA;
	m_nFlatTopSpacerHookLenBType = rebarTemplate->m_nFlatTopSpacerHookLenBType;
	m_nFlatTopSpacerMainBarHookLenB = rebarTemplate->m_nFlatTopSpacerMainBarHookLenB;
	m_nFlatTopSpacerUserHookLenB = rebarTemplate->m_nFlatTopSpacerUserHookLenB;
	m_nFlatTopSpacerWidth = rebarTemplate->m_nFlatTopSpacerWidth;

	m_bFlatBotSpacerSet = rebarTemplate->m_bFlatBotSpacerSet;
	m_nFlatBotSpacerDiaType = rebarTemplate->m_nFlatBotSpacerDiaType;
	m_nFlatBotSpacerDiaIndex = rebarTemplate->m_nFlatBotSpacerDiaIndex;
	m_nFlatBotSpacerSpacingType = rebarTemplate->m_nFlatBotSpacerSpacingType;
	m_nFlatBotSpacerSlabSpacing = rebarTemplate->m_nFlatBotSpacerSlabSpacing;
	m_nFlatBotSpacerUserSpacing1 = rebarTemplate->m_nFlatBotSpacerUserSpacing1;
	m_nFlatBotSpacerUserSpacing2 = rebarTemplate->m_nFlatBotSpacerUserSpacing2;
	m_nFlatBotSpacerHeightType = rebarTemplate->m_nFlatBotSpacerHeightType;
	m_nFlatBotSpacerUserHeight = rebarTemplate->m_nFlatBotSpacerUserHeight;
	m_nFlatBotSpacerHookLenAType = rebarTemplate->m_nFlatBotSpacerHookLenAType;
	m_nFlatBotSpacerMainBarHookLenA = rebarTemplate->m_nFlatBotSpacerMainBarHookLenA;
	m_nFlatBotSpacerUserHookLenA = rebarTemplate->m_nFlatBotSpacerUserHookLenA;
	m_nFlatBotSpacerHookLenBType = rebarTemplate->m_nFlatBotSpacerHookLenBType;
	m_nFlatBotSpacerMainBarHookLenB = rebarTemplate->m_nFlatBotSpacerMainBarHookLenB;
	m_nFlatBotSpacerUserHookLenB = rebarTemplate->m_nFlatBotSpacerUserHookLenB;
	m_nFlatBotSpacerWidth = rebarTemplate->m_nFlatBotSpacerWidth;

	// 사변지지 SLAB
	m_nSlabLongMaxDualHookLength = rebarTemplate->m_nSlabLongMaxDualHookLength;
	m_nSlabLongSpliceWay = rebarTemplate->m_nSlabLongSpliceWay;
	m_nSlabLongSpliceType = rebarTemplate->m_nSlabLongSpliceType;
	m_bSlabLongAlterSpliceSet = rebarTemplate->m_bSlabLongAlterSpliceSet;
	m_nSlabLongAlterSpliceValue1 = rebarTemplate->m_nSlabLongAlterSpliceValue1;

	m_nSlabShortMaxDualHookLength = rebarTemplate->m_nSlabShortMaxDualHookLength;
	m_nSlabShortSpliceWay = rebarTemplate->m_nSlabShortSpliceWay;
	m_nSlabShortSpliceType = rebarTemplate->m_nSlabShortSpliceType;
	m_bSlabShortAlterSpliceSet = rebarTemplate->m_bSlabShortAlterSpliceSet;
	m_nSlabShortAlterSpliceValue1 = rebarTemplate->m_nSlabShortAlterSpliceValue1;

	m_nSlabTopStructuredEndHookType = rebarTemplate->m_nSlabTopStructuredEndHookType;
	m_nSlabBotStructuredEndHookType = rebarTemplate->m_nSlabBotStructuredEndHookType;

	m_nSlabOneWayCutBarLenType = rebarTemplate->m_nSlabOneWayCutBarLenType;
	m_nSlabTwoWayCutBarLenType = rebarTemplate->m_nSlabTwoWayCutBarLenType;

	m_nSlabTopUnStructuredEndHookType = rebarTemplate->m_nSlabTopUnStructuredEndHookType;
	m_nSlabBotUnStructuredEndHookType = rebarTemplate->m_nSlabBotUnStructuredEndHookType;
	m_nSlabIsoFootUnStructuredDevType = rebarTemplate->m_nSlabIsoFootUnStructuredDevType;

	m_nSlabLevelDifPlaceShape = rebarTemplate->m_nSlabLevelDifPlaceShape;
	m_nSlabLevelDifHaunchPlaceType = rebarTemplate->m_nSlabLevelDifHaunchPlaceType;

	m_nSlabOpeningTopHookType = rebarTemplate->m_nSlabOpeningTopHookType;
	m_nSlabOpeningBotHookType = rebarTemplate->m_nSlabOpeningBotHookType;

	m_nSlabRebarTolDist = rebarTemplate->m_nSlabRebarTolDist;
	m_nSlabRebarTolLenDif = rebarTemplate->m_nSlabRebarTolLenDif;

	m_bSlabTopSpacerSet = rebarTemplate->m_bSlabTopSpacerSet;
	m_nSlabTopSpacerDiaType = rebarTemplate->m_nSlabTopSpacerDiaType;
	m_nSlabTopSpacerDiaIndex = rebarTemplate->m_nSlabTopSpacerDiaIndex;
	m_nSlabTopSpacerSpacingType = rebarTemplate->m_nSlabTopSpacerSpacingType;
	m_nSlabTopSpacerSlabSpacing = rebarTemplate->m_nSlabTopSpacerSlabSpacing;
	m_nSlabTopSpacerUserSpacing1 = rebarTemplate->m_nSlabTopSpacerUserSpacing1;
	m_nSlabTopSpacerUserSpacing2 = rebarTemplate->m_nSlabTopSpacerUserSpacing2;
	m_nSlabTopSpacerHeightType = rebarTemplate->m_nSlabTopSpacerHeightType;
	m_nSlabTopSpacerUserHeight = rebarTemplate->m_nSlabTopSpacerUserHeight;
	m_nSlabTopSpacerHookLenAType = rebarTemplate->m_nSlabTopSpacerHookLenAType;
	m_nSlabTopSpacerMainBarHookLenA = rebarTemplate->m_nSlabTopSpacerMainBarHookLenA;
	m_nSlabTopSpacerUserHookLenA = rebarTemplate->m_nSlabTopSpacerUserHookLenA;
	m_nSlabTopSpacerHookLenBType = rebarTemplate->m_nSlabTopSpacerHookLenBType;
	m_nSlabTopSpacerMainBarHookLenB = rebarTemplate->m_nSlabTopSpacerMainBarHookLenB;
	m_nSlabTopSpacerUserHookLenB = rebarTemplate->m_nSlabTopSpacerUserHookLenB;
	m_nSlabTopSpacerWidth = rebarTemplate->m_nSlabTopSpacerWidth;

	m_bSlabBotSpacerSet = rebarTemplate->m_bSlabBotSpacerSet;
	m_nSlabBotSpacerDiaType = rebarTemplate->m_nSlabBotSpacerDiaType;
	m_nSlabBotSpacerDiaIndex = rebarTemplate->m_nSlabBotSpacerDiaIndex;
	m_nSlabBotSpacerSpacingType = rebarTemplate->m_nSlabBotSpacerSpacingType;
	m_nSlabBotSpacerSlabSpacing = rebarTemplate->m_nSlabBotSpacerSlabSpacing;
	m_nSlabBotSpacerUserSpacing1 = rebarTemplate->m_nSlabBotSpacerUserSpacing1;
	m_nSlabBotSpacerUserSpacing2 = rebarTemplate->m_nSlabBotSpacerUserSpacing2;
	m_nSlabBotSpacerHeightType = rebarTemplate->m_nSlabBotSpacerHeightType;
	m_nSlabBotSpacerUserHeight = rebarTemplate->m_nSlabBotSpacerUserHeight;
	m_nSlabBotSpacerHookLenAType = rebarTemplate->m_nSlabBotSpacerHookLenAType;
	m_nSlabBotSpacerMainBarHookLenA = rebarTemplate->m_nSlabBotSpacerMainBarHookLenA;
	m_nSlabBotSpacerUserHookLenA = rebarTemplate->m_nSlabBotSpacerUserHookLenA;
	m_nSlabBotSpacerHookLenBType = rebarTemplate->m_nSlabBotSpacerHookLenBType;
	m_nSlabBotSpacerMainBarHookLenB = rebarTemplate->m_nSlabBotSpacerMainBarHookLenB;
	m_nSlabBotSpacerUserHookLenB = rebarTemplate->m_nSlabBotSpacerUserHookLenB;
	m_nSlabBotSpacerWidth = rebarTemplate->m_nSlabBotSpacerWidth;

	// SOG Slab
	m_nSOGLongMaxDualHookLength = rebarTemplate->m_nSOGLongMaxDualHookLength;
	m_nSOGLongSpliceWay = rebarTemplate->m_nSOGLongSpliceWay;
	m_nSOGLongSpliceType = rebarTemplate->m_nSOGLongSpliceType;
	m_bSOGLongAlterSpliceSet = rebarTemplate->m_bSOGLongAlterSpliceSet;
	m_nSOGLongAlterSpliceValue1 = rebarTemplate->m_nSOGLongAlterSpliceValue1;

	m_nSOGShortMaxDualHookLength = rebarTemplate->m_nSOGShortMaxDualHookLength;
	m_nSOGShortSpliceWay = rebarTemplate->m_nSOGShortSpliceWay;
	m_nSOGShortSpliceType = rebarTemplate->m_nSOGShortSpliceType;
	m_bSOGShortAlterSpliceSet = rebarTemplate->m_bSOGShortAlterSpliceSet;
	m_nSOGShortAlterSpliceValue1 = rebarTemplate->m_nSOGShortAlterSpliceValue1;

	m_nSOGTopEndHookType = rebarTemplate->m_nSOGTopEndHookType;
	m_nSOGBotEndHookType = rebarTemplate->m_nSOGBotEndHookType;
	m_nSOGIsoFootDevType = rebarTemplate->m_nSOGIsoFootDevType;
	m_nSOGSpliceDevSlabType = rebarTemplate->m_nSOGSpliceDevSlabType;

	m_nSOGLevelDifPlaceShape = rebarTemplate->m_nSOGLevelDifPlaceShape;
	m_nSOGLevelDifHaunchPlaceType = rebarTemplate->m_nSOGLevelDifHaunchPlaceType;

	m_nSOGOpeningTopHookType = rebarTemplate->m_nSOGOpeningTopHookType;
	m_nSOGOpeningBotHookType = rebarTemplate->m_nSOGOpeningBotHookType;

	m_nSOGRebarTolDist = rebarTemplate->m_nSOGRebarTolDist;
	m_nSOGRebarTolLenDif = rebarTemplate->m_nSOGRebarTolLenDif;

	m_bSOGTopSpacerSet = rebarTemplate->m_bSOGTopSpacerSet;
	m_nSOGTopSpacerDiaType = rebarTemplate->m_nSOGTopSpacerDiaType;
	m_nSOGTopSpacerDiaIndex = rebarTemplate->m_nSOGTopSpacerDiaIndex;
	m_nSOGTopSpacerSpacingType = rebarTemplate->m_nSOGTopSpacerSpacingType;
	m_nSOGTopSpacerSlabSpacing = rebarTemplate->m_nSOGTopSpacerSlabSpacing;
	m_nSOGTopSpacerUserSpacing1 = rebarTemplate->m_nSOGTopSpacerUserSpacing1;
	m_nSOGTopSpacerUserSpacing2 = rebarTemplate->m_nSOGTopSpacerUserSpacing2;
	m_nSOGTopSpacerHeightType = rebarTemplate->m_nSOGTopSpacerHeightType;
	m_nSOGTopSpacerUserHeight = rebarTemplate->m_nSOGTopSpacerUserHeight;
	m_nSOGTopSpacerHookLenAType = rebarTemplate->m_nSOGTopSpacerHookLenAType;
	m_nSOGTopSpacerMainBarHookLenA = rebarTemplate->m_nSOGTopSpacerMainBarHookLenA;
	m_nSOGTopSpacerUserHookLenA = rebarTemplate->m_nSOGTopSpacerUserHookLenA;
	m_nSOGTopSpacerHookLenBType = rebarTemplate->m_nSOGTopSpacerHookLenBType;
	m_nSOGTopSpacerMainBarHookLenB = rebarTemplate->m_nSOGTopSpacerMainBarHookLenB;
	m_nSOGTopSpacerUserHookLenB = rebarTemplate->m_nSOGTopSpacerUserHookLenB;
	m_nSOGTopSpacerWidth = rebarTemplate->m_nSOGTopSpacerWidth;

	m_bSOGBotSpacerSet = rebarTemplate->m_bSOGBotSpacerSet;
	m_nSOGBotSpacerDiaType = rebarTemplate->m_nSOGBotSpacerDiaType;
	m_nSOGBotSpacerDiaIndex = rebarTemplate->m_nSOGBotSpacerDiaIndex;
	m_nSOGBotSpacerSpacingType = rebarTemplate->m_nSOGBotSpacerSpacingType;
	m_nSOGBotSpacerSlabSpacing = rebarTemplate->m_nSOGBotSpacerSlabSpacing;
	m_nSOGBotSpacerUserSpacing1 = rebarTemplate->m_nSOGBotSpacerUserSpacing1;
	m_nSOGBotSpacerUserSpacing2 = rebarTemplate->m_nSOGBotSpacerUserSpacing2;
	m_nSOGBotSpacerHeightType = rebarTemplate->m_nSOGBotSpacerHeightType;
	m_nSOGBotSpacerUserHeight = rebarTemplate->m_nSOGBotSpacerUserHeight;
	m_nSOGBotSpacerHookLenAType = rebarTemplate->m_nSOGBotSpacerHookLenAType;
	m_nSOGBotSpacerMainBarHookLenA = rebarTemplate->m_nSOGBotSpacerMainBarHookLenA;
	m_nSOGBotSpacerUserHookLenA = rebarTemplate->m_nSOGBotSpacerUserHookLenA;
	m_nSOGBotSpacerHookLenBType = rebarTemplate->m_nSOGBotSpacerHookLenBType;
	m_nSOGBotSpacerMainBarHookLenB = rebarTemplate->m_nSOGBotSpacerMainBarHookLenB;
	m_nSOGBotSpacerUserHookLenB = rebarTemplate->m_nSOGBotSpacerUserHookLenB;
	m_nSOGBotSpacerWidth = rebarTemplate->m_nSOGBotSpacerWidth;

	// Circular Ramp
	m_nCircularRampLongMaxDualHookLength = rebarTemplate->m_nCircularRampLongMaxDualHookLength;
	m_nCircularRampLongSpliceWay = rebarTemplate->m_nCircularRampLongSpliceWay;
	m_nCircularRampLongSpliceType = rebarTemplate->m_nCircularRampLongSpliceType;
	m_bCircularRampLongAlterSpliceSet = rebarTemplate->m_bCircularRampLongAlterSpliceSet;
	m_nCircularRampLongAlterSpliceValue1 = rebarTemplate->m_nCircularRampLongAlterSpliceValue1;

	m_nCircularRampShortMaxDualHookLength = rebarTemplate->m_nCircularRampShortMaxDualHookLength;
	m_nCircularRampShortSpliceWay = rebarTemplate->m_nCircularRampShortSpliceWay;
	m_nCircularRampShortSpliceType = rebarTemplate->m_nCircularRampShortSpliceType;
	m_bCircularRampShortAlterSpliceSet = rebarTemplate->m_bCircularRampShortAlterSpliceSet;
	m_nCircularRampShortAlterSpliceValue1 = rebarTemplate->m_nCircularRampShortAlterSpliceValue1;

	m_nCircularRampTopEndHookType = rebarTemplate->m_nCircularRampTopEndHookType;
	m_nCircularRampBotEndHookType = rebarTemplate->m_nCircularRampBotEndHookType;
	m_nCircularRampIsoFootDevType = rebarTemplate->m_nCircularRampIsoFootDevType;
	m_nCircularRampSpliceDevSlabType = rebarTemplate->m_nCircularRampSpliceDevSlabType;

	m_nCircularRampLevelDifPlaceShape = rebarTemplate->m_nCircularRampLevelDifPlaceShape;
	m_nCircularRampLevelDifHaunchPlaceType = rebarTemplate->m_nCircularRampLevelDifHaunchPlaceType;

	m_nCircularRampOpeningTopHookType = rebarTemplate->m_nCircularRampOpeningTopHookType;
	m_nCircularRampOpeningBotHookType = rebarTemplate->m_nCircularRampOpeningBotHookType;

	m_nCircularRampRebarTolDist = rebarTemplate->m_nCircularRampRebarTolDist;
	m_nCircularRampRebarTolLenDif = rebarTemplate->m_nCircularRampRebarTolLenDif;

	m_bCircularRampTopSpacerSet = rebarTemplate->m_bCircularRampTopSpacerSet;
	m_nCircularRampTopSpacerDiaType = rebarTemplate->m_nCircularRampTopSpacerDiaType;
	m_nCircularRampTopSpacerDiaIndex = rebarTemplate->m_nCircularRampTopSpacerDiaIndex;
	m_nCircularRampTopSpacerSpacingType = rebarTemplate->m_nCircularRampTopSpacerSpacingType;
	m_nCircularRampTopSpacerSlabSpacing = rebarTemplate->m_nCircularRampTopSpacerSlabSpacing;
	m_nCircularRampTopSpacerUserSpacing1 = rebarTemplate->m_nCircularRampTopSpacerUserSpacing1;
	m_nCircularRampTopSpacerUserSpacing2 = rebarTemplate->m_nCircularRampTopSpacerUserSpacing2;
	m_nCircularRampTopSpacerHeightType = rebarTemplate->m_nCircularRampTopSpacerHeightType;
	m_nCircularRampTopSpacerUserHeight = rebarTemplate->m_nCircularRampTopSpacerUserHeight;
	m_nCircularRampTopSpacerHookLenAType = rebarTemplate->m_nCircularRampTopSpacerHookLenAType;
	m_nCircularRampTopSpacerMainBarHookLenA = rebarTemplate->m_nCircularRampTopSpacerMainBarHookLenA;
	m_nCircularRampTopSpacerUserHookLenA = rebarTemplate->m_nCircularRampTopSpacerUserHookLenA;
	m_nCircularRampTopSpacerHookLenBType = rebarTemplate->m_nCircularRampTopSpacerHookLenBType;
	m_nCircularRampTopSpacerMainBarHookLenB = rebarTemplate->m_nCircularRampTopSpacerMainBarHookLenB;
	m_nCircularRampTopSpacerUserHookLenB = rebarTemplate->m_nCircularRampTopSpacerUserHookLenB;
	m_nCircularRampTopSpacerWidth = rebarTemplate->m_nCircularRampTopSpacerWidth;

	m_bCircularRampBotSpacerSet = rebarTemplate->m_bCircularRampBotSpacerSet;
	m_nCircularRampBotSpacerDiaType = rebarTemplate->m_nCircularRampBotSpacerDiaType;
	m_nCircularRampBotSpacerDiaIndex = rebarTemplate->m_nCircularRampBotSpacerDiaIndex;
	m_nCircularRampBotSpacerSpacingType = rebarTemplate->m_nCircularRampBotSpacerSpacingType;
	m_nCircularRampBotSpacerSlabSpacing = rebarTemplate->m_nCircularRampBotSpacerSlabSpacing;
	m_nCircularRampBotSpacerUserSpacing1 = rebarTemplate->m_nCircularRampBotSpacerUserSpacing1;
	m_nCircularRampBotSpacerUserSpacing2 = rebarTemplate->m_nCircularRampBotSpacerUserSpacing2;
	m_nCircularRampBotSpacerHeightType = rebarTemplate->m_nCircularRampBotSpacerHeightType;
	m_nCircularRampBotSpacerUserHeight = rebarTemplate->m_nCircularRampBotSpacerUserHeight;
	m_nCircularRampBotSpacerHookLenAType = rebarTemplate->m_nCircularRampBotSpacerHookLenAType;
	m_nCircularRampBotSpacerMainBarHookLenA = rebarTemplate->m_nCircularRampBotSpacerMainBarHookLenA;
	m_nCircularRampBotSpacerUserHookLenA = rebarTemplate->m_nCircularRampBotSpacerUserHookLenA;
	m_nCircularRampBotSpacerHookLenBType = rebarTemplate->m_nCircularRampBotSpacerHookLenBType;
	m_nCircularRampBotSpacerMainBarHookLenB = rebarTemplate->m_nCircularRampBotSpacerMainBarHookLenB;
	m_nCircularRampBotSpacerUserHookLenB = rebarTemplate->m_nCircularRampBotSpacerUserHookLenB;
	m_nCircularRampBotSpacerWidth = rebarTemplate->m_nCircularRampBotSpacerWidth;

	// PC Slab
	m_nPCSlabLongMaxDualHookLength = rebarTemplate->m_nPCSlabLongMaxDualHookLength;
	m_nPCSlabLongSpliceWay = rebarTemplate->m_nPCSlabLongSpliceWay;
	m_nPCSlabLongSpliceType = rebarTemplate->m_nPCSlabLongSpliceType;
	m_bPCSlabLongAlterSpliceSet = rebarTemplate->m_bPCSlabLongAlterSpliceSet;
	m_nPCSlabLongAlterSpliceValue1 = rebarTemplate->m_nPCSlabLongAlterSpliceValue1;

	m_nPCSlabShortMaxDualHookLength = rebarTemplate->m_nPCSlabShortMaxDualHookLength;
	m_nPCSlabShortSpliceWay = rebarTemplate->m_nPCSlabShortSpliceWay;
	m_nPCSlabShortSpliceType = rebarTemplate->m_nPCSlabShortSpliceType;
	m_bPCSlabShortAlterSpliceSet = rebarTemplate->m_bPCSlabShortAlterSpliceSet;
	m_nPCSlabShortAlterSpliceValue1 = rebarTemplate->m_nPCSlabShortAlterSpliceValue1;

	m_nPCSlabTopStructuredEndHookType = rebarTemplate->m_nPCSlabTopStructuredEndHookType;
	m_nPCSlabBotStructuredEndHookType = rebarTemplate->m_nPCSlabBotStructuredEndHookType;

	m_nPCSlabDeckDifDiaConnectBarType = rebarTemplate->m_nPCSlabDeckDifDiaConnectBarType;
	m_nPCSlabDeckAnchorType = rebarTemplate->m_nPCSlabDeckAnchorType;
	m_nPCSlabDeckTopEndRatioType = rebarTemplate->m_nPCSlabDeckTopEndRatioType;
	m_nPCSlabDeckBotEndRatioType = rebarTemplate->m_nPCSlabDeckBotEndRatioType;

	m_nPCSlabTopUnStructuredEndHookType = rebarTemplate->m_nPCSlabTopUnStructuredEndHookType;
	m_nPCSlabBotUnStructuredEndHookType = rebarTemplate->m_nPCSlabBotUnStructuredEndHookType;
	m_nPCSlabIsoFootUnStructuredDevType = rebarTemplate->m_nPCSlabIsoFootUnStructuredDevType;

	m_nPCSlabLevelDifPlaceShape = rebarTemplate->m_nPCSlabLevelDifPlaceShape;
	m_nPCSlabLevelDifHaunchPlaceType = rebarTemplate->m_nPCSlabLevelDifHaunchPlaceType;

	m_nPCSlabOpeningTopHookType = rebarTemplate->m_nPCSlabOpeningTopHookType;
	m_nPCSlabOpeningBotHookType = rebarTemplate->m_nPCSlabOpeningBotHookType;

	m_nPCSlabRebarTolDist = rebarTemplate->m_nPCSlabRebarTolDist;
	m_nPCSlabRebarTolLenDif = rebarTemplate->m_nPCSlabRebarTolLenDif;

	m_bPCSlabTopSpacerSet = rebarTemplate->m_bPCSlabTopSpacerSet;
	m_nPCSlabTopSpacerDiaType = rebarTemplate->m_nPCSlabTopSpacerDiaType;
	m_nPCSlabTopSpacerDiaIndex = rebarTemplate->m_nPCSlabTopSpacerDiaIndex;
	m_nPCSlabTopSpacerSpacingType = rebarTemplate->m_nPCSlabTopSpacerSpacingType;
	m_nPCSlabTopSpacerSlabSpacing = rebarTemplate->m_nPCSlabTopSpacerSlabSpacing;
	m_nPCSlabTopSpacerUserSpacing1 = rebarTemplate->m_nPCSlabTopSpacerUserSpacing1;
	m_nPCSlabTopSpacerUserSpacing2 = rebarTemplate->m_nPCSlabTopSpacerUserSpacing2;
	m_nPCSlabTopSpacerHeightType = rebarTemplate->m_nPCSlabTopSpacerHeightType;
	m_nPCSlabTopSpacerUserHeight = rebarTemplate->m_nPCSlabTopSpacerUserHeight;
	m_nPCSlabTopSpacerHookLenAType = rebarTemplate->m_nPCSlabTopSpacerHookLenAType;
	m_nPCSlabTopSpacerMainBarHookLenA = rebarTemplate->m_nPCSlabTopSpacerMainBarHookLenA;
	m_nPCSlabTopSpacerUserHookLenA = rebarTemplate->m_nPCSlabTopSpacerUserHookLenA;
	m_nPCSlabTopSpacerHookLenBType = rebarTemplate->m_nPCSlabTopSpacerHookLenBType;
	m_nPCSlabTopSpacerMainBarHookLenB = rebarTemplate->m_nPCSlabTopSpacerMainBarHookLenB;
	m_nPCSlabTopSpacerUserHookLenB = rebarTemplate->m_nPCSlabTopSpacerUserHookLenB;
	m_nPCSlabTopSpacerWidth = rebarTemplate->m_nPCSlabTopSpacerWidth;

	m_bPCSlabBotSpacerSet = rebarTemplate->m_bPCSlabBotSpacerSet;
	m_nPCSlabBotSpacerDiaType = rebarTemplate->m_nPCSlabBotSpacerDiaType;
	m_nPCSlabBotSpacerDiaIndex = rebarTemplate->m_nPCSlabBotSpacerDiaIndex;
	m_nPCSlabBotSpacerSpacingType = rebarTemplate->m_nPCSlabBotSpacerSpacingType;
	m_nPCSlabBotSpacerSlabSpacing = rebarTemplate->m_nPCSlabBotSpacerSlabSpacing;
	m_nPCSlabBotSpacerUserSpacing1 = rebarTemplate->m_nPCSlabBotSpacerUserSpacing1;
	m_nPCSlabBotSpacerUserSpacing2 = rebarTemplate->m_nPCSlabBotSpacerUserSpacing2;
	m_nPCSlabBotSpacerHeightType = rebarTemplate->m_nPCSlabBotSpacerHeightType;
	m_nPCSlabBotSpacerUserHeight = rebarTemplate->m_nPCSlabBotSpacerUserHeight;
	m_nPCSlabBotSpacerHookLenAType = rebarTemplate->m_nPCSlabBotSpacerHookLenAType;
	m_nPCSlabBotSpacerMainBarHookLenA = rebarTemplate->m_nPCSlabBotSpacerMainBarHookLenA;
	m_nPCSlabBotSpacerUserHookLenA = rebarTemplate->m_nPCSlabBotSpacerUserHookLenA;
	m_nPCSlabBotSpacerHookLenBType = rebarTemplate->m_nPCSlabBotSpacerHookLenBType;
	m_nPCSlabBotSpacerMainBarHookLenB = rebarTemplate->m_nPCSlabBotSpacerMainBarHookLenB;
	m_nPCSlabBotSpacerUserHookLenB = rebarTemplate->m_nPCSlabBotSpacerUserHookLenB;
	m_nPCSlabBotSpacerWidth = rebarTemplate->m_nPCSlabBotSpacerWidth;

	// 철근 일체형 데크 슬래브
	m_nEmbeddedDeckLongMaxDualHookLength = rebarTemplate->m_nEmbeddedDeckLongMaxDualHookLength;
	m_nEmbeddedDeckLongSpliceWay = rebarTemplate->m_nEmbeddedDeckLongSpliceWay;
	m_nEmbeddedDeckLongSpliceType = rebarTemplate->m_nEmbeddedDeckLongSpliceType;
	m_bEmbeddedDeckLongAlterSpliceSet = rebarTemplate->m_bEmbeddedDeckLongAlterSpliceSet;
	m_nEmbeddedDeckLongAlterSpliceValue1 = rebarTemplate->m_nEmbeddedDeckLongAlterSpliceValue1;

	m_nEmbeddedDeckShortMaxDualHookLength = rebarTemplate->m_nEmbeddedDeckShortMaxDualHookLength;
	m_nEmbeddedDeckShortSpliceWay = rebarTemplate->m_nEmbeddedDeckShortSpliceWay;
	m_nEmbeddedDeckShortSpliceType = rebarTemplate->m_nEmbeddedDeckShortSpliceType;
	m_bEmbeddedDeckShortAlterSpliceSet = rebarTemplate->m_bEmbeddedDeckShortAlterSpliceSet;
	m_nEmbeddedDeckShortAlterSpliceValue1 = rebarTemplate->m_nEmbeddedDeckShortAlterSpliceValue1;

	m_nEmbeddedDeckTopStructuredEndHookType = rebarTemplate->m_nEmbeddedDeckTopStructuredEndHookType;
	m_nEmbeddedDeckBotStructuredEndHookType = rebarTemplate->m_nEmbeddedDeckBotStructuredEndHookType;

	m_nEmbeddedDeckDifDiaConnectBarType = rebarTemplate->m_nEmbeddedDeckDifDiaConnectBarType;
	m_nEmbeddedDeckAnchorType = rebarTemplate->m_nEmbeddedDeckAnchorType;
	m_nEmbeddedDeckTopEndRatioType = rebarTemplate->m_nEmbeddedDeckTopEndRatioType;
	m_nEmbeddedDeckBotEndRatioType = rebarTemplate->m_nEmbeddedDeckBotEndRatioType;

	m_nEmbeddedDeckTopUnStructuredEndHookType = rebarTemplate->m_nEmbeddedDeckTopUnStructuredEndHookType;
	m_nEmbeddedDeckBotUnStructuredEndHookType = rebarTemplate->m_nEmbeddedDeckBotUnStructuredEndHookType;
	m_nEmbeddedDeckIsoFootUnStructuredDevType = rebarTemplate->m_nEmbeddedDeckIsoFootUnStructuredDevType;

	m_nEmbeddedDeckLevelDifPlaceShape = rebarTemplate->m_nEmbeddedDeckLevelDifPlaceShape;
	m_nEmbeddedDeckLevelDifHaunchPlaceType = rebarTemplate->m_nEmbeddedDeckLevelDifHaunchPlaceType;

	m_nEmbeddedDeckOpeningTopHookType = rebarTemplate->m_nEmbeddedDeckOpeningTopHookType;
	m_nEmbeddedDeckOpeningBotHookType = rebarTemplate->m_nEmbeddedDeckOpeningBotHookType;

	m_nEmbeddedDeckRebarTolDist = rebarTemplate->m_nEmbeddedDeckRebarTolDist;
	m_nEmbeddedDeckRebarTolLenDif = rebarTemplate->m_nEmbeddedDeckRebarTolLenDif;

	// 일반 데크 슬래브
	m_nGeneralDeckLongMaxDualHookLength = rebarTemplate->m_nGeneralDeckLongMaxDualHookLength;
	m_nGeneralDeckLongSpliceWay = rebarTemplate->m_nGeneralDeckLongSpliceWay;
	m_nGeneralDeckLongSpliceType = rebarTemplate->m_nGeneralDeckLongSpliceType;
	m_bGeneralDeckLongAlterSpliceSet = rebarTemplate->m_bGeneralDeckLongAlterSpliceSet;
	m_nGeneralDeckLongAlterSpliceValue1 = rebarTemplate->m_nGeneralDeckLongAlterSpliceValue1;

	m_nGeneralDeckShortMaxDualHookLength = rebarTemplate->m_nGeneralDeckShortMaxDualHookLength;
	m_nGeneralDeckShortSpliceWay = rebarTemplate->m_nGeneralDeckShortSpliceWay;
	m_nGeneralDeckShortSpliceType = rebarTemplate->m_nGeneralDeckShortSpliceType;
	m_bGeneralDeckShortAlterSpliceSet = rebarTemplate->m_bGeneralDeckShortAlterSpliceSet;
	m_nGeneralDeckShortAlterSpliceValue1 = rebarTemplate->m_nGeneralDeckShortAlterSpliceValue1;

	m_nGeneralDeckTopStructuredEndHookType = rebarTemplate->m_nGeneralDeckTopStructuredEndHookType;
	m_nGeneralDeckBotStructuredEndHookType = rebarTemplate->m_nGeneralDeckBotStructuredEndHookType;

	m_nGeneralDeckTopUnStructuredEndHookType = rebarTemplate->m_nGeneralDeckTopUnStructuredEndHookType;
	m_nGeneralDeckBotUnStructuredEndHookType = rebarTemplate->m_nGeneralDeckBotUnStructuredEndHookType;
	m_nGeneralDeckIsoFootUnStructuredDevType = rebarTemplate->m_nGeneralDeckIsoFootUnStructuredDevType;

	m_nGeneralDeckLevelDifPlaceShape = rebarTemplate->m_nGeneralDeckLevelDifPlaceShape;
	m_nGeneralDeckLevelDifHaunchPlaceType = rebarTemplate->m_nGeneralDeckLevelDifHaunchPlaceType;

	m_nGeneralDeckOpeningTopHookType = rebarTemplate->m_nGeneralDeckOpeningTopHookType;
	m_nGeneralDeckOpeningBotHookType = rebarTemplate->m_nGeneralDeckOpeningBotHookType;

	m_nGeneralDeckRebarTolDist = rebarTemplate->m_nGeneralDeckRebarTolDist;
	m_nGeneralDeckRebarTolLenDif = rebarTemplate->m_nGeneralDeckRebarTolLenDif;
}

void OldSlabRebarTemplate::OldDataCopy(SlabRebarTemplate* rebarTemplate)
{
	// MAT Slab
	rebarTemplate->SetMatMaxDualHookLength(m_nMatLongMaxDualHookLength);
	rebarTemplate->SetMatSpliceWay(m_nMatLongSpliceWay);
	rebarTemplate->SetMatSpliceType(m_nMatLongSpliceType);

	rebarTemplate->SetMatTopEndHookType(m_nMatTopEndHookType);
	rebarTemplate->SetMatBotEndHookType(m_nMatBotEndHookType);
	rebarTemplate->SetMatIsoFootDevType(m_nMatIsoFootDevType);

	rebarTemplate->SetMatLevelDifPlaceShape(m_nMatLevelDifPlaceShape);
	rebarTemplate->SetMatLevelDifHaunchPlaceType(m_nMatLevelDifHaunchPlaceType);

	rebarTemplate->SetMatRebarTolDist(m_nMatRebarTolDist);
	rebarTemplate->SetMatRebarTolLenDif(m_nMatRebarTolLenDif);

	rebarTemplate->SetMatTopSpacerSet(m_bMatTopSpacerSet);
	rebarTemplate->SetMatTopSpacerDiaType(m_nMatTopSpacerDiaType);
	rebarTemplate->SetMatTopSpacerSlabThickOverDiaIndex(m_nMatTopSpacerDiaIndex);
	rebarTemplate->SetMatTopSpacerSpacingType(m_nMatTopSpacerSpacingType);
	rebarTemplate->SetMatTopSpacerSlabSpacing(m_nMatTopSpacerSlabSpacing);
	rebarTemplate->SetMatTopSpacerUserSpacing1(m_nMatTopSpacerUserSpacing1);
	rebarTemplate->SetMatTopSpacerUserSpacing2(m_nMatTopSpacerUserSpacing2);
	rebarTemplate->SetMatTopSpacerHeightType(m_nMatTopSpacerHeightType);
	rebarTemplate->SetMatTopSpacerUserHeight(m_nMatTopSpacerUserHeight);
	rebarTemplate->SetMatTopSpacerHookLenAType(m_nMatTopSpacerHookLenAType);
	rebarTemplate->SetMatTopSpacerMainBarHookLenA(m_nMatTopSpacerMainBarHookLenA);
	rebarTemplate->SetMatTopSpacerUserHookLenA(m_nMatTopSpacerUserHookLenA);
	rebarTemplate->SetMatTopSpacerHookLenBType(m_nMatTopSpacerHookLenBType);
	rebarTemplate->SetMatTopSpacerMainBarHookLenB(m_nMatTopSpacerMainBarHookLenB);
	rebarTemplate->SetMatTopSpacerUserHookLenB(m_nMatTopSpacerUserHookLenB);
	rebarTemplate->SetMatTopSpacerWidth(m_nMatTopSpacerWidth);

	rebarTemplate->SetMatBotSpacerSet(m_bMatBotSpacerSet);
	rebarTemplate->SetMatBotSpacerDiaType(m_nMatBotSpacerDiaType);
	rebarTemplate->SetMatBotSpacerDiaIndex(m_nMatBotSpacerDiaIndex);
	rebarTemplate->SetMatBotSpacerSpacingType(m_nMatBotSpacerSpacingType);
	rebarTemplate->SetMatBotSpacerSlabSpacing(m_nMatBotSpacerSlabSpacing);
	rebarTemplate->SetMatBotSpacerUserSpacing1(m_nMatBotSpacerUserSpacing1);
	rebarTemplate->SetMatBotSpacerUserSpacing2(m_nMatBotSpacerUserSpacing2);
	rebarTemplate->SetMatBotSpacerHeightType(m_nMatBotSpacerHeightType);
	rebarTemplate->SetMatBotSpacerUserHeight(m_nMatBotSpacerUserHeight);
	rebarTemplate->SetMatBotSpacerHookLenAType(m_nMatBotSpacerHookLenAType);
	rebarTemplate->SetMatBotSpacerMainBarHookLenA(m_nMatBotSpacerMainBarHookLenA);
	rebarTemplate->SetMatBotSpacerUserHookLenA(m_nMatBotSpacerUserHookLenA);
	rebarTemplate->SetMatBotSpacerHookLenBType(m_nMatBotSpacerHookLenBType);
	rebarTemplate->SetMatBotSpacerMainBarHookLenB(m_nMatBotSpacerMainBarHookLenB);
	rebarTemplate->SetMatBotSpacerUserHookLenB(m_nMatBotSpacerUserHookLenB);
	rebarTemplate->SetMatBotSpacerWidth(m_nMatBotSpacerWidth);

	// FLAT Slab
	rebarTemplate->SetFlatMaxDualHookLength(m_nFlatLongMaxDualHookLength);
	rebarTemplate->SetFlatSpliceWay(m_nFlatLongSpliceWay);
	rebarTemplate->SetFlatSpliceType(m_nFlatLongSpliceType);
	
	rebarTemplate->SetFlatTopEndHookType(m_nFlatTopEndHookType);
	rebarTemplate->SetFlatBotEndHookType(m_nFlatBotEndHookType);
	rebarTemplate->SetFlatIsoFootDevType(m_nFlatIsoFootDevType);
	rebarTemplate->SetFlatSpliceDevSlabType(m_nFlatSpliceDevSlabType);

	rebarTemplate->SetFlatLevelDifPlaceShape(m_nFlatLevelDifPlaceShape);
	rebarTemplate->SetFlatLevelDifHaunchPlaceType(m_nFlatLevelDifHaunchPlaceType);
	
	rebarTemplate->SetFlatRebarTolDist(m_nFlatRebarTolDist);
	rebarTemplate->SetFlatRebarTolLenDif(m_nFlatRebarTolLenDif);

	rebarTemplate->SetFlatTopSpacerSet(m_bFlatTopSpacerSet);
	rebarTemplate->SetFlatTopSpacerDiaType(m_nFlatTopSpacerDiaType);
	rebarTemplate->SetFlatTopSpacerSlabThickUnderDiaIndex(m_nFlatTopSpacerDiaIndex);
	rebarTemplate->SetFlatTopSpacerSpacingType(m_nFlatTopSpacerSpacingType);
	rebarTemplate->SetFlatTopSpacerSlabSpacing(m_nFlatTopSpacerSlabSpacing);
	rebarTemplate->SetFlatTopSpacerUserSpacing1(m_nFlatTopSpacerUserSpacing1);
	rebarTemplate->SetFlatTopSpacerUserSpacing2(m_nFlatTopSpacerUserSpacing2);
	rebarTemplate->SetFlatTopSpacerHeightType(m_nFlatTopSpacerHeightType);
	rebarTemplate->SetFlatTopSpacerUserHeight(m_nFlatTopSpacerUserHeight);
	rebarTemplate->SetFlatTopSpacerHookLenAType(m_nFlatTopSpacerHookLenAType);
	rebarTemplate->SetFlatTopSpacerMainBarHookLenA(m_nFlatTopSpacerMainBarHookLenA);
	rebarTemplate->SetFlatTopSpacerUserHookLenA(m_nFlatTopSpacerUserHookLenA);
	rebarTemplate->SetFlatTopSpacerHookLenBType(m_nFlatTopSpacerHookLenBType);
	rebarTemplate->SetFlatTopSpacerMainBarHookLenB(m_nFlatTopSpacerMainBarHookLenB);
	rebarTemplate->SetFlatTopSpacerUserHookLenB(m_nFlatTopSpacerUserHookLenB);
	rebarTemplate->SetFlatTopSpacerWidth(m_nFlatTopSpacerWidth);

	rebarTemplate->SetFlatBotSpacerSet(m_bFlatBotSpacerSet);
	rebarTemplate->SetFlatBotSpacerDiaType(m_nFlatBotSpacerDiaType);
	rebarTemplate->SetFlatBotSpacerDiaIndex(m_nFlatBotSpacerDiaIndex);
	rebarTemplate->SetFlatBotSpacerSpacingType(m_nFlatBotSpacerSpacingType);
	rebarTemplate->SetFlatBotSpacerSlabSpacing(m_nFlatBotSpacerSlabSpacing);
	rebarTemplate->SetFlatBotSpacerUserSpacing1(m_nFlatBotSpacerUserSpacing1);
	rebarTemplate->SetFlatBotSpacerUserSpacing2(m_nFlatBotSpacerUserSpacing2);
	rebarTemplate->SetFlatBotSpacerHeightType(m_nFlatBotSpacerHeightType);
	rebarTemplate->SetFlatBotSpacerUserHeight(m_nFlatBotSpacerUserHeight);
	rebarTemplate->SetFlatBotSpacerHookLenAType(m_nFlatBotSpacerHookLenAType);
	rebarTemplate->SetFlatBotSpacerMainBarHookLenA(m_nFlatBotSpacerMainBarHookLenA);
	rebarTemplate->SetFlatBotSpacerUserHookLenA(m_nFlatBotSpacerUserHookLenA);
	rebarTemplate->SetFlatBotSpacerHookLenBType(m_nFlatBotSpacerHookLenBType);
	rebarTemplate->SetFlatBotSpacerMainBarHookLenB(m_nFlatBotSpacerMainBarHookLenB);
	rebarTemplate->SetFlatBotSpacerUserHookLenB(m_nFlatBotSpacerUserHookLenB);
	rebarTemplate->SetFlatBotSpacerWidth(m_nFlatBotSpacerWidth);

	// 사변지지 SLAB
	rebarTemplate->SetSlabMaxDualHookLength(m_nSlabLongMaxDualHookLength);
	rebarTemplate->SetSlabSpliceWay(m_nSlabLongSpliceWay);
	rebarTemplate->SetSlabSpliceType(m_nSlabLongSpliceType);
	

	rebarTemplate->SetSlabOneWayCutBarLenType(m_nSlabOneWayCutBarLenType);
	rebarTemplate->SetSlabTwoWayCutBarLenType(m_nSlabTwoWayCutBarLenType);

	rebarTemplate->SetSlabTopUnStructuredEndHookType(m_nSlabTopUnStructuredEndHookType);
	rebarTemplate->SetSlabBotUnStructuredEndHookType(m_nSlabBotUnStructuredEndHookType);

	rebarTemplate->SetSlabLevelDifPlaceShape(m_nSlabLevelDifPlaceShape);
	rebarTemplate->SetSlabLevelDifHaunchPlaceType(m_nSlabLevelDifHaunchPlaceType);
	
	rebarTemplate->SetSlabRebarTolDist(m_nSlabRebarTolDist);
	rebarTemplate->SetSlabRebarTolLenDif(m_nSlabRebarTolLenDif);

	rebarTemplate->SetSlabTopSpacerSet(m_bSlabTopSpacerSet);
	rebarTemplate->SetSlabTopSpacerDiaType(m_nSlabTopSpacerDiaType);
	rebarTemplate->SetSlabTopSpacerSlabThickUnderDiaIndex(m_nSlabTopSpacerDiaIndex);
	rebarTemplate->SetSlabTopSpacerSpacingType(m_nSlabTopSpacerSpacingType);
	rebarTemplate->SetSlabTopSpacerSlabSpacing(m_nSlabTopSpacerSlabSpacing);
	rebarTemplate->SetSlabTopSpacerUserSpacing1(m_nSlabTopSpacerUserSpacing1);
	rebarTemplate->SetSlabTopSpacerUserSpacing2(m_nSlabTopSpacerUserSpacing2);
	rebarTemplate->SetSlabTopSpacerHeightType(m_nSlabTopSpacerHeightType);
	rebarTemplate->SetSlabTopSpacerUserHeight(m_nSlabTopSpacerUserHeight);
	rebarTemplate->SetSlabTopSpacerHookLenAType(m_nSlabTopSpacerHookLenAType);
	rebarTemplate->SetSlabTopSpacerMainBarHookLenA(m_nSlabTopSpacerMainBarHookLenA);
	rebarTemplate->SetSlabTopSpacerUserHookLenA(m_nSlabTopSpacerUserHookLenA);
	rebarTemplate->SetSlabTopSpacerHookLenBType(m_nSlabTopSpacerHookLenBType);
	rebarTemplate->SetSlabTopSpacerMainBarHookLenB(m_nSlabTopSpacerMainBarHookLenB);
	rebarTemplate->SetSlabTopSpacerUserHookLenB(m_nSlabTopSpacerUserHookLenB);
	rebarTemplate->SetSlabTopSpacerWidth(m_nSlabTopSpacerWidth);

	rebarTemplate->SetSlabBotSpacerSet(m_bSlabBotSpacerSet);
	rebarTemplate->SetSlabBotSpacerDiaType(m_nSlabBotSpacerDiaType);
	rebarTemplate->SetSlabBotSpacerDiaIndex(m_nSlabBotSpacerDiaIndex);
	rebarTemplate->SetSlabBotSpacerSpacingType(m_nSlabBotSpacerSpacingType);
	rebarTemplate->SetSlabBotSpacerSlabSpacing(m_nSlabBotSpacerSlabSpacing);
	rebarTemplate->SetSlabBotSpacerUserSpacing1(m_nSlabBotSpacerUserSpacing1);
	rebarTemplate->SetSlabBotSpacerUserSpacing2(m_nSlabBotSpacerUserSpacing2);
	rebarTemplate->SetSlabBotSpacerHeightType(m_nSlabBotSpacerHeightType);
	rebarTemplate->SetSlabBotSpacerUserHeight(m_nSlabBotSpacerUserHeight);
	rebarTemplate->SetSlabBotSpacerHookLenAType(m_nSlabBotSpacerHookLenAType);
	rebarTemplate->SetSlabBotSpacerMainBarHookLenA(m_nSlabBotSpacerMainBarHookLenA);
	rebarTemplate->SetSlabBotSpacerUserHookLenA(m_nSlabBotSpacerUserHookLenA);
	rebarTemplate->SetSlabBotSpacerHookLenBType(m_nSlabBotSpacerHookLenBType);
	rebarTemplate->SetSlabBotSpacerMainBarHookLenB(m_nSlabBotSpacerMainBarHookLenB);
	rebarTemplate->SetSlabBotSpacerUserHookLenB(m_nSlabBotSpacerUserHookLenB);
	rebarTemplate->SetSlabBotSpacerWidth(m_nSlabBotSpacerWidth);

	// SOG Slab
	rebarTemplate->SetSOGMaxDualHookLength(m_nSOGLongMaxDualHookLength);
	rebarTemplate->SetSOGSpliceWay(m_nSOGLongSpliceWay);
	rebarTemplate->SetSOGSpliceType(m_nSOGLongSpliceType);
	
	rebarTemplate->SetSOGTopEndHookType(m_nSOGTopEndHookType);
	rebarTemplate->SetSOGBotEndHookType(m_nSOGBotEndHookType);
	rebarTemplate->SetSOGIsoFootDevType(m_nSOGIsoFootDevType);
	rebarTemplate->SetSOGSpliceDevSlabType(m_nSOGSpliceDevSlabType);

	rebarTemplate->SetSOGLevelDifPlaceShape(m_nSOGLevelDifPlaceShape);
	rebarTemplate->SetSOGLevelDifHaunchPlaceType(m_nSOGLevelDifHaunchPlaceType);

	rebarTemplate->SetSOGRebarTolDist(m_nSOGRebarTolDist);
	rebarTemplate->SetSOGRebarTolLenDif(m_nSOGRebarTolLenDif);

	rebarTemplate->SetSOGTopSpacerSet(m_bSOGTopSpacerSet);
	rebarTemplate->SetSOGTopSpacerDiaType(m_nSOGTopSpacerDiaType);
	rebarTemplate->SetSOGTopSpacerSlabThickUnderDiaIndex(m_nSOGTopSpacerDiaIndex);
	rebarTemplate->SetSOGTopSpacerSpacingType(m_nSOGTopSpacerSpacingType);
	rebarTemplate->SetSOGTopSpacerSlabSpacing(m_nSOGTopSpacerSlabSpacing);
	rebarTemplate->SetSOGTopSpacerUserSpacing1(m_nSOGTopSpacerUserSpacing1);
	rebarTemplate->SetSOGTopSpacerUserSpacing2(m_nSOGTopSpacerUserSpacing2);
	rebarTemplate->SetSOGTopSpacerHeightType(m_nSOGTopSpacerHeightType);
	rebarTemplate->SetSOGTopSpacerUserHeight(m_nSOGTopSpacerUserHeight);
	rebarTemplate->SetSOGTopSpacerHookLenAType(m_nSOGTopSpacerHookLenAType);
	rebarTemplate->SetSOGTopSpacerMainBarHookLenA(m_nSOGTopSpacerMainBarHookLenA);
	rebarTemplate->SetSOGTopSpacerUserHookLenA(m_nSOGTopSpacerUserHookLenA);
	rebarTemplate->SetSOGTopSpacerHookLenBType(m_nSOGTopSpacerHookLenBType);
	rebarTemplate->SetSOGTopSpacerMainBarHookLenB(m_nSOGTopSpacerMainBarHookLenB);
	rebarTemplate->SetSOGTopSpacerUserHookLenB(m_nSOGTopSpacerUserHookLenB);
	rebarTemplate->SetSOGTopSpacerWidth(m_nSOGTopSpacerWidth);

	rebarTemplate->SetSOGBotSpacerSet(m_bSOGBotSpacerSet);
	rebarTemplate->SetSOGBotSpacerDiaType(m_nSOGBotSpacerDiaType);
	rebarTemplate->SetSOGBotSpacerDiaIndex(m_nSOGBotSpacerDiaIndex);
	rebarTemplate->SetSOGBotSpacerSpacingType(m_nSOGBotSpacerSpacingType);
	rebarTemplate->SetSOGBotSpacerSlabSpacing(m_nSOGBotSpacerSlabSpacing);
	rebarTemplate->SetSOGBotSpacerUserSpacing1(m_nSOGBotSpacerUserSpacing1);
	rebarTemplate->SetSOGBotSpacerUserSpacing2(m_nSOGBotSpacerUserSpacing2);
	rebarTemplate->SetSOGBotSpacerHeightType(m_nSOGBotSpacerHeightType);
	rebarTemplate->SetSOGBotSpacerUserHeight(m_nSOGBotSpacerUserHeight);
	rebarTemplate->SetSOGBotSpacerHookLenAType(m_nSOGBotSpacerHookLenAType);
	rebarTemplate->SetSOGBotSpacerMainBarHookLenA(m_nSOGBotSpacerMainBarHookLenA);
	rebarTemplate->SetSOGBotSpacerUserHookLenA(m_nSOGBotSpacerUserHookLenA);
	rebarTemplate->SetSOGBotSpacerHookLenBType(m_nSOGBotSpacerHookLenBType);
	rebarTemplate->SetSOGBotSpacerMainBarHookLenB(m_nSOGBotSpacerMainBarHookLenB);
	rebarTemplate->SetSOGBotSpacerUserHookLenB(m_nSOGBotSpacerUserHookLenB);
	rebarTemplate->SetSOGBotSpacerWidth(m_nSOGBotSpacerWidth);

	// Circular Ramp
	rebarTemplate->SetCircularRampMaxDualHookLength(m_nCircularRampLongMaxDualHookLength);
	rebarTemplate->SetCircularRampSpliceWay(m_nCircularRampLongSpliceWay);
	rebarTemplate->SetCircularRampSpliceType(m_nCircularRampLongSpliceType);
	
	rebarTemplate->SetCircularRampTopEndHookType(m_nCircularRampTopEndHookType);
	rebarTemplate->SetCircularRampBotEndHookType(m_nCircularRampBotEndHookType);
	rebarTemplate->SetCircularRampIsoFootDevType(m_nCircularRampIsoFootDevType);
	rebarTemplate->SetCircularRampSpliceDevSlabType(m_nCircularRampSpliceDevSlabType);

	rebarTemplate->SetCircularRampLevelDifPlaceShape(m_nCircularRampLevelDifPlaceShape);
	rebarTemplate->SetCircularRampLevelDifHaunchPlaceType(m_nCircularRampLevelDifHaunchPlaceType);

	rebarTemplate->SetCircularRampRebarTolDist(m_nCircularRampRebarTolDist);
	rebarTemplate->SetCircularRampRebarTolLenDif(m_nCircularRampRebarTolLenDif);

	rebarTemplate->SetCircularRampTopSpacerSet(m_bCircularRampTopSpacerSet);
	rebarTemplate->SetCircularRampTopSpacerDiaType(m_nCircularRampTopSpacerDiaType);
	rebarTemplate->SetCircularRampTopSpacerSlabThickUnderDiaIndex(m_nCircularRampTopSpacerDiaIndex);
	rebarTemplate->SetCircularRampTopSpacerSpacingType(m_nCircularRampTopSpacerSpacingType);
	rebarTemplate->SetCircularRampTopSpacerSlabSpacing(m_nCircularRampTopSpacerSlabSpacing);
	rebarTemplate->SetCircularRampTopSpacerUserSpacing1(m_nCircularRampTopSpacerUserSpacing1);
	rebarTemplate->SetCircularRampTopSpacerUserSpacing2(m_nCircularRampTopSpacerUserSpacing2);
	rebarTemplate->SetCircularRampTopSpacerHeightType(m_nCircularRampTopSpacerHeightType);
	rebarTemplate->SetCircularRampTopSpacerUserHeight(m_nCircularRampTopSpacerUserHeight);
	rebarTemplate->SetCircularRampTopSpacerHookLenAType(m_nCircularRampTopSpacerHookLenAType);
	rebarTemplate->SetCircularRampTopSpacerMainBarHookLenA(m_nCircularRampTopSpacerMainBarHookLenA);
	rebarTemplate->SetCircularRampTopSpacerUserHookLenA(m_nCircularRampTopSpacerUserHookLenA);
	rebarTemplate->SetCircularRampTopSpacerHookLenBType(m_nCircularRampTopSpacerHookLenBType);
	rebarTemplate->SetCircularRampTopSpacerMainBarHookLenB(m_nCircularRampTopSpacerMainBarHookLenB);
	rebarTemplate->SetCircularRampTopSpacerUserHookLenB(m_nCircularRampTopSpacerUserHookLenB);
	rebarTemplate->SetCircularRampTopSpacerWidth(m_nCircularRampTopSpacerWidth);

	rebarTemplate->SetCircularRampBotSpacerSet(m_bCircularRampBotSpacerSet);
	rebarTemplate->SetCircularRampBotSpacerDiaType(m_nCircularRampBotSpacerDiaType);
	rebarTemplate->SetCircularRampBotSpacerDiaIndex(m_nCircularRampBotSpacerDiaIndex);
	rebarTemplate->SetCircularRampBotSpacerSpacingType(m_nCircularRampBotSpacerSpacingType);
	rebarTemplate->SetCircularRampBotSpacerSlabSpacing(m_nCircularRampBotSpacerSlabSpacing);
	rebarTemplate->SetCircularRampBotSpacerUserSpacing1(m_nCircularRampBotSpacerUserSpacing1);
	rebarTemplate->SetCircularRampBotSpacerUserSpacing2(m_nCircularRampBotSpacerUserSpacing2);
	rebarTemplate->SetCircularRampBotSpacerHeightType(m_nCircularRampBotSpacerHeightType);
	rebarTemplate->SetCircularRampBotSpacerUserHeight(m_nCircularRampBotSpacerUserHeight);
	rebarTemplate->SetCircularRampBotSpacerHookLenAType(m_nCircularRampBotSpacerHookLenAType);
	rebarTemplate->SetCircularRampBotSpacerMainBarHookLenA(m_nCircularRampBotSpacerMainBarHookLenA);
	rebarTemplate->SetCircularRampBotSpacerUserHookLenA(m_nCircularRampBotSpacerUserHookLenA);
	rebarTemplate->SetCircularRampBotSpacerHookLenBType(m_nCircularRampBotSpacerHookLenBType);
	rebarTemplate->SetCircularRampBotSpacerMainBarHookLenB(m_nCircularRampBotSpacerMainBarHookLenB);
	rebarTemplate->SetCircularRampBotSpacerUserHookLenB(m_nCircularRampBotSpacerUserHookLenB);
	rebarTemplate->SetCircularRampBotSpacerWidth(m_nCircularRampBotSpacerWidth);

	// PC Slab
	rebarTemplate->SetPCSlabMaxDualHookLength(m_nPCSlabLongMaxDualHookLength);
	rebarTemplate->SetPCSlabSpliceWay(m_nPCSlabLongSpliceWay);
	rebarTemplate->SetPCSlabSpliceType(m_nPCSlabLongSpliceType);
	
	rebarTemplate->SetPCSlabTopUnStructuredEndHookType(m_nPCSlabTopUnStructuredEndHookType);
	rebarTemplate->SetPCSlabBotUnStructuredEndHookType(m_nPCSlabBotUnStructuredEndHookType);
	
	rebarTemplate->SetPCSlabLevelDifPlaceShape(m_nPCSlabLevelDifPlaceShape);
	rebarTemplate->SetPCSlabLevelDifHaunchPlaceType(m_nPCSlabLevelDifHaunchPlaceType);

	rebarTemplate->SetPCSlabOpeningTopDevType(m_nPCSlabOpeningTopHookType);
	rebarTemplate->SetPCSlabOpeningBotDevType(m_nPCSlabOpeningBotHookType);

	rebarTemplate->SetPCSlabRebarTolDist(m_nPCSlabRebarTolDist);
	rebarTemplate->SetPCSlabRebarTolLenDif(m_nPCSlabRebarTolLenDif);

	// 철근 일체형 데크 슬래브
	rebarTemplate->SetEmbeddedDeckMaxDualHookLength(m_nEmbeddedDeckLongMaxDualHookLength);
	rebarTemplate->SetEmbeddedDeckSpliceWay(m_nEmbeddedDeckLongSpliceWay);
	rebarTemplate->SetEmbeddedDeckSpliceType(m_nEmbeddedDeckLongSpliceType);
		
	rebarTemplate->SetEmbeddedDeckTopUnStructuredEndHookType(m_nEmbeddedDeckTopUnStructuredEndHookType);
	rebarTemplate->SetEmbeddedDeckBotUnStructuredEndHookType(m_nEmbeddedDeckBotUnStructuredEndHookType);
	
	rebarTemplate->SetEmbeddedDeckLevelDifPlaceShape(m_nEmbeddedDeckLevelDifPlaceShape);
	rebarTemplate->SetEmbeddedDeckLevelDifHaunchPlaceType(m_nEmbeddedDeckLevelDifHaunchPlaceType);

	rebarTemplate->SetEmbeddedDeckOpeningTopDevType(m_nEmbeddedDeckOpeningTopHookType);
	rebarTemplate->SetEmbeddedDeckOpeningBotDevType(m_nEmbeddedDeckOpeningBotHookType);

	rebarTemplate->SetEmbeddedDeckRebarTolDist(m_nEmbeddedDeckRebarTolDist);
	rebarTemplate->SetEmbeddedDeckRebarTolLenDif(m_nEmbeddedDeckRebarTolLenDif);

	// 일반 데크 슬래브
	rebarTemplate->SetGeneralDeckMaxDualHookLength(m_nGeneralDeckLongMaxDualHookLength);
	rebarTemplate->SetGeneralDeckSpliceWay(m_nGeneralDeckLongSpliceWay);
	rebarTemplate->SetGeneralDeckSpliceType(m_nGeneralDeckLongSpliceType);
	
	rebarTemplate->SetGeneralDeckTopUnStructuredEndHookType(m_nGeneralDeckTopUnStructuredEndHookType);
	rebarTemplate->SetGeneralDeckBotUnStructuredEndHookType(m_nGeneralDeckBotUnStructuredEndHookType);
	
	rebarTemplate->SetGeneralDeckLevelDifPlaceShape(m_nGeneralDeckLevelDifPlaceShape);
	rebarTemplate->SetGeneralDeckLevelDifHaunchPlaceType(m_nGeneralDeckLevelDifHaunchPlaceType);

	rebarTemplate->SetGeneralDeckOpeningTopDevType(m_nGeneralDeckOpeningTopHookType);
	rebarTemplate->SetGeneralDeckOpeningBotDevType(m_nGeneralDeckOpeningBotHookType);

	rebarTemplate->SetGeneralDeckRebarTolDist(m_nGeneralDeckRebarTolDist);
	rebarTemplate->SetGeneralDeckRebarTolLenDif(m_nGeneralDeckRebarTolLenDif);
}

void OldSlabRebarTemplate::InfoSetCheckBoxChecked()
{
	// MAT Slab
	m_bMatLongMainBarInfoSet = true;
	m_bMatShortMainBarInfoSet = true;
	m_bMatEndInfoSet = true;
	m_bMatLevelDifInfoSet = true;
	m_bMatRebarAreaInfoSet = true;
	m_bMatTopSpacerInfoSet = true;
	m_bMatBotSpacerInfoSet = true;

	// FLAT Slab
	m_bFlatLongMainBarInfoSet = true;
	m_bFlatShortMainBarInfoSet = true;
	m_bFlatEndInfoSet = true;
	m_bFlatLevelDifInfoSet = true;
	m_bFlatOpeningInfoSet = true;
	m_bFlatRebarAreaInfoSet = true;
	m_bFlatTopSpacerInfoSet = true;
	m_bFlatBotSpacerInfoSet = true;

	// 사변지지 SLAB
	m_bSlabLongMainBarInfoSet = true;
	m_bSlabShortMainBarInfoSet = true;
	m_bSlabStructuredEndInfoSet = true;
	m_bSlabTopCutBarLenInfoSet = true;
	m_bSlabUnStructuredEndInfoSet = true;
	m_bSlabLevelDifInfoSet = true;
	m_bSlabOpeningInfoSet = true;
	m_bSlabRebarAreaInfoSet = true;
	m_bSlabTopSpacerInfoSet = true;
	m_bSlabBotSpacerInfoSet = true;

	// SOG Slab
	m_bSOGLongMainBarInfoSet = true;
	m_bSOGShortMainBarInfoSet = true;
	m_bSOGEndInfoSet = true;
	m_bSOGLevelDifInfoSet = true;
	m_bSOGOpeningInfoSet = true;
	m_bSOGRebarAreaInfoSet = true;
	m_bSOGTopSpacerInfoSet = true;
	m_bSOGBotSpacerInfoSet = true;

	// Circular Ramp
	m_bCircularRampLongMainBarInfoSet = true;
	m_bCircularRampShortMainBarInfoSet = true;
	m_bCircularRampEndInfoSet = true;
	m_bCircularRampLevelDifInfoSet = true;
	m_bCircularRampOpeningInfoSet = true;
	m_bCircularRampRebarAreaInfoSet = true;
	m_bCircularRampTopSpacerInfoSet = true;
	m_bCircularRampBotSpacerInfoSet = true;

	// PC Slab
	m_bPCSlabLongMainBarInfoSet = true;
	m_bPCSlabShortMainBarInfoSet = true;
	m_bPCSlabStructuredEndInfoSet = true;
	m_bPCSlabDeckInfoSet = true;
	m_bPCSlabUnStructuredEndInfoSet = true;
	m_bPCSlabLevelDifInfoSet = true;
	m_bPCSlabOpeningInfoSet = true;
	m_bPCSlabRebarAreaInfoSet = true;
	m_bPCSlabTopSpacerInfoSet = true;
	m_bPCSlabBotSpacerInfoSet = true;

	// 철근 일체형 데크 슬래브
	m_bEmbeddedDeckLongMainBarInfoSet = true;
	m_bEmbeddedDeckShortMainBarInfoSet = true;
	m_bEmbeddedDeckStructuredEndInfoSet = true;
	m_bEmbeddedDeckInfoSet = true;
	m_bEmbeddedDeckUnStructuredEndInfoSet = true;
	m_bEmbeddedDeckLevelDifInfoSet = true;
	m_bEmbeddedDeckOpeningInfoSet = true;
	m_bEmbeddedDeckRebarAreaInfoSet = true;

	// 일반 데크 슬래브
	m_bGeneralDeckLongMainBarInfoSet = true;
	m_bGeneralDeckShortMainBarInfoSet = true;
	m_bGeneralDeckStructuredEndInfoSet = true;
	m_bGeneralDeckUnStructuredEndInfoSet = true;
	m_bGeneralDeckLevelDifInfoSet = true;
	m_bGeneralDeckOpeningInfoSet = true;
	m_bGeneralDeckRebarAreaInfoSet = true;
}
