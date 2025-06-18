#pragma once
#include "OldBaseRebarTemplate.h"
#include "..\SlabRebarTemplate.h"

class StructureModel_CLASS OldSlabRebarTemplate : public OldBaseRebarTemplate
{
public:
	OldSlabRebarTemplate();
	~OldSlabRebarTemplate(void);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	DECLARE_SERIAL(OldSlabRebarTemplate)
	void Serialize(CArchive& ar);

	virtual void CopyFromMe(OldSlabRebarTemplate* pSlabRebarTemplate);

	void DataInit();

	bool TopOptionDataCopy(OldSlabRebarTemplate* rebarTemplate);
	void DataCopy(OldSlabRebarTemplate* rebarTemplate);
	void OldDataCopy(SlabRebarTemplate* rebarTemplate);
	void InfoSetCheckBoxChecked();

	// MAT Slab
	virtual BOOL GetMatLongMainBarInfoSet() { return m_bMatLongMainBarInfoSet; }
	virtual long GetMatLongMaxDualHookLength() { return m_nMatLongMaxDualHookLength; }
	virtual long GetMatLongSpliceWay() { return m_nMatLongSpliceWay; }
	virtual long GetMatLongSpliceType() { return m_nMatLongSpliceType; }
	virtual BOOL GetMatLongAlterSpliceSet() { return m_bMatLongAlterSpliceSet; }
	virtual long GetMatLongAlterSpliceValue1() { return m_nMatLongAlterSpliceValue1; }

	virtual BOOL GetMatShortMainBarInfoSet() { return m_bMatShortMainBarInfoSet; }
	virtual long GetMatShortMaxDualHookLength() { return m_nMatShortMaxDualHookLength; }
	virtual long GetMatShortSpliceWay() { return m_nMatShortSpliceWay; }
	virtual long GetMatShortSpliceType() { return m_nMatShortSpliceType; }
	virtual BOOL GetMatShortAlterSpliceSet() { return m_bMatShortAlterSpliceSet; }
	virtual long GetMatShortAlterSpliceValue1() { return m_nMatShortAlterSpliceValue1; }

	virtual BOOL GetMatEndInfoSet() { return m_bMatEndInfoSet; }
	virtual long GetMatTopEndHookType() { return m_nMatTopEndHookType; }
	virtual long GetMatBotEndHookType() { return m_nMatBotEndHookType; }
	virtual long GetMatIsoFootDevType() { return m_nMatIsoFootDevType; }

	virtual BOOL GetMatLevelDifInfoSet() { return m_bMatLevelDifInfoSet; }
	virtual long GetMatLevelDifPlaceShape() { return m_nMatLevelDifPlaceShape; }
	virtual long GetMatLevelDifHaunchPlaceType() { return m_nMatLevelDifHaunchPlaceType; }

	virtual BOOL GetMatRebarAreaInfoSet() { return m_bMatRebarAreaInfoSet; }
	virtual long GetMatRebarTolDist() { return m_nMatRebarTolDist; }
	virtual long GetMatRebarTolLenDif() { return m_nMatRebarTolLenDif; }

	virtual BOOL GetMatTopSpacerInfoSet() { return m_bMatTopSpacerInfoSet; }
	virtual BOOL GetMatTopSpacerSet() { return m_bMatTopSpacerSet; }
	virtual long GetMatTopSpacerDiaType() { return m_nMatTopSpacerDiaType; }
	virtual long GetMatTopSpacerDiaIndex() { return m_nMatTopSpacerDiaIndex; }
	virtual long GetMatTopSpacerSpacingType() { return m_nMatTopSpacerSpacingType; }
	virtual long GetMatTopSpacerSlabSpacing() { return m_nMatTopSpacerSlabSpacing; }
	virtual long GetMatTopSpacerUserSpacing1() { return m_nMatTopSpacerUserSpacing1; }
	virtual long GetMatTopSpacerUserSpacing2() { return m_nMatTopSpacerUserSpacing2; }
	virtual long GetMatTopSpacerHeightType() { return m_nMatTopSpacerHeightType; }
	virtual long GetMatTopSpacerUserHeight() { return m_nMatTopSpacerUserHeight; }
	virtual long GetMatTopSpacerHookLenAType() { return m_nMatTopSpacerHookLenAType; }
	virtual long GetMatTopSpacerMainBarHookLenA() { return m_nMatTopSpacerMainBarHookLenA; }
	virtual long GetMatTopSpacerUserHookLenA() { return m_nMatTopSpacerUserHookLenA; }
	virtual long GetMatTopSpacerHookLenBType() { return m_nMatTopSpacerHookLenBType; }
	virtual long GetMatTopSpacerMainBarHookLenB() { return m_nMatTopSpacerMainBarHookLenB; }
	virtual long GetMatTopSpacerUserHookLenB() { return m_nMatTopSpacerUserHookLenB; }
	virtual long GetMatTopSpacerWidth() { return m_nMatTopSpacerWidth; }

	virtual BOOL GetMatBotSpacerInfoSet() { return m_bMatBotSpacerInfoSet; }
	virtual BOOL GetMatBotSpacerSet() { return m_bMatBotSpacerSet; }
	virtual long GetMatBotSpacerDiaType() { return m_nMatBotSpacerDiaType; }
	virtual long GetMatBotSpacerDiaIndex() { return m_nMatBotSpacerDiaIndex; }
	virtual long GetMatBotSpacerSpacingType() { return m_nMatBotSpacerSpacingType; }
	virtual long GetMatBotSpacerSlabSpacing() { return m_nMatBotSpacerSlabSpacing; }
	virtual long GetMatBotSpacerUserSpacing1() { return m_nMatBotSpacerUserSpacing1; }
	virtual long GetMatBotSpacerUserSpacing2() { return m_nMatBotSpacerUserSpacing2; }
	virtual long GetMatBotSpacerHeightType() { return m_nMatBotSpacerHeightType; }
	virtual long GetMatBotSpacerUserHeight() { return m_nMatBotSpacerUserHeight; }
	virtual long GetMatBotSpacerHookLenAType() { return m_nMatBotSpacerHookLenAType; }
	virtual long GetMatBotSpacerMainBarHookLenA() { return m_nMatBotSpacerMainBarHookLenA; }
	virtual long GetMatBotSpacerUserHookLenA() { return m_nMatBotSpacerUserHookLenA; }
	virtual long GetMatBotSpacerHookLenBType() { return m_nMatBotSpacerHookLenBType; }
	virtual long GetMatBotSpacerMainBarHookLenB() { return m_nMatBotSpacerMainBarHookLenB; }
	virtual long GetMatBotSpacerUserHookLenB() { return m_nMatBotSpacerUserHookLenB; }
	virtual long GetMatBotSpacerWidth() { return m_nMatBotSpacerWidth; }

	virtual void SetMatLongMainBarInfoSet(BOOL value) { m_bMatLongMainBarInfoSet = value; }
	virtual void SetMatLongMaxDualHookLength(long value) { m_nMatLongMaxDualHookLength = value; }
	virtual void SetMatLongSpliceWay(long value) { m_nMatLongSpliceWay = value; }
	virtual void SetMatLongSpliceType(long value) { m_nMatLongSpliceType = value; }
	virtual void SetMatLongAlterSpliceSet(BOOL value) { m_bMatLongAlterSpliceSet = value; }
	virtual void SetMatLongAlterSpliceValue1(long value) { m_nMatLongAlterSpliceValue1 = value; }

	virtual void SetMatShortMainBarInfoSet(BOOL value) { m_bMatShortMainBarInfoSet = value; }
	virtual void SetMatShortMaxDualHookLength(long value) { m_nMatShortMaxDualHookLength = value; }
	virtual void SetMatShortSpliceWay(long value) { m_nMatShortSpliceWay = value; }
	virtual void SetMatShortSpliceType(long value) { m_nMatShortSpliceType = value; }
	virtual void SetMatShortAlterSpliceSet(BOOL value) { m_bMatShortAlterSpliceSet = value; }
	virtual void SetMatShortAlterSpliceValue1(long value) { m_nMatShortAlterSpliceValue1 = value; }

	virtual void SetMatEndInfoSet(BOOL value) { m_bMatEndInfoSet = value; }
	virtual void SetMatTopEndHookType(long value) { m_nMatTopEndHookType = value; }
	virtual void SetMatBotEndHookType(long value) { m_nMatBotEndHookType = value; }
	virtual void SetMatIsoFootDevType(long value) { m_nMatIsoFootDevType = value; }

	virtual void SetMatLevelDifInfoSet(BOOL value) { m_bMatLevelDifInfoSet = value; }
	virtual void SetMatLevelDifPlaceShape(long value) { m_nMatLevelDifPlaceShape = value; }
	virtual void SetMatLevelDifHaunchPlaceType(long value) { m_nMatLevelDifHaunchPlaceType = value; }

	virtual void SetMatRebarAreaInfoSet(BOOL value) { m_bMatRebarAreaInfoSet = value; }
	virtual void SetMatRebarTolDist(long value) { m_nMatRebarTolDist = value; }
	virtual void SetMatRebarTolLenDif(long value) { m_nMatRebarTolLenDif = value; }

	virtual void SetMatTopSpacerInfoSet(BOOL value) { m_bMatTopSpacerInfoSet = value; }
	virtual void SetMatTopSpacerSet(BOOL value) { m_bMatTopSpacerSet = value; }
	virtual void SetMatTopSpacerDiaType(long value) { m_nMatTopSpacerDiaType = value; }
	virtual void SetMatTopSpacerDiaIndex(long value) { m_nMatTopSpacerDiaIndex = value; }
	virtual void SetMatTopSpacerSpacingType(long value) { m_nMatTopSpacerSpacingType = value; }
	virtual void SetMatTopSpacerSlabSpacing(long value) { m_nMatTopSpacerSlabSpacing = value; }
	virtual void SetMatTopSpacerUserSpacing1(long value) { m_nMatTopSpacerUserSpacing1 = value; }
	virtual void SetMatTopSpacerUserSpacing2(long value) { m_nMatTopSpacerUserSpacing2 = value; }
	virtual void SetMatTopSpacerHeightType(long value) { m_nMatTopSpacerHeightType = value; }
	virtual void SetMatTopSpacerUserHeight(long value) { m_nMatTopSpacerUserHeight = value; }
	virtual void SetMatTopSpacerHookLenAType(long value) { m_nMatTopSpacerHookLenAType = value; }
	virtual void SetMatTopSpacerMainBarHookLenA(long value) { m_nMatTopSpacerMainBarHookLenA = value; }
	virtual void SetMatTopSpacerUserHookLenA(long value) { m_nMatTopSpacerUserHookLenA = value; }
	virtual void SetMatTopSpacerHookLenBType(long value) { m_nMatTopSpacerHookLenBType = value; }
	virtual void SetMatTopSpacerMainBarHookLenB(long value) { m_nMatTopSpacerMainBarHookLenB = value; }
	virtual void SetMatTopSpacerUserHookLenB(long value) { m_nMatTopSpacerUserHookLenB = value; }
	virtual void SetMatTopSpacerWidth(long value) { m_nMatTopSpacerWidth = value; }

	virtual void SetMatBotSpacerInfoSet(BOOL value) { m_bMatBotSpacerInfoSet = value; }
	virtual void SetMatBotSpacerSet(BOOL value) { m_bMatBotSpacerSet = value; }
	virtual void SetMatBotSpacerDiaType(long value) { m_nMatBotSpacerDiaType = value; }
	virtual void SetMatBotSpacerDiaIndex(long value) { m_nMatBotSpacerDiaIndex = value; }
	virtual void SetMatBotSpacerSpacingType(long value) { m_nMatBotSpacerSpacingType = value; }
	virtual void SetMatBotSpacerSlabSpacing(long value) { m_nMatBotSpacerSlabSpacing = value; }
	virtual void SetMatBotSpacerUserSpacing1(long value) { m_nMatBotSpacerUserSpacing1 = value; }
	virtual void SetMatBotSpacerUserSpacing2(long value) { m_nMatBotSpacerUserSpacing2 = value; }
	virtual void SetMatBotSpacerHeightType(long value) { m_nMatBotSpacerHeightType = value; }
	virtual void SetMatBotSpacerUserHeight(long value) { m_nMatBotSpacerUserHeight = value; }
	virtual void SetMatBotSpacerHookLenAType(long value) { m_nMatBotSpacerHookLenAType = value; }
	virtual void SetMatBotSpacerMainBarHookLenA(long value) { m_nMatBotSpacerMainBarHookLenA = value; }
	virtual void SetMatBotSpacerUserHookLenA(long value) { m_nMatBotSpacerUserHookLenA = value; }
	virtual void SetMatBotSpacerHookLenBType(long value) { m_nMatBotSpacerHookLenBType = value; }
	virtual void SetMatBotSpacerMainBarHookLenB(long value) { m_nMatBotSpacerMainBarHookLenB = value; }
	virtual void SetMatBotSpacerUserHookLenB(long value) { m_nMatBotSpacerUserHookLenB = value; }
	virtual void SetMatBotSpacerWidth(long value) { m_nMatBotSpacerWidth = value; }

	//FLAT Slab
	virtual BOOL GetFlatLongMainBarInfoSet() { return m_bFlatLongMainBarInfoSet; }
	virtual long GetFlatLongMaxDualHookLength() { return m_nFlatLongMaxDualHookLength; }
	virtual long GetFlatLongSpliceWay() { return m_nFlatLongSpliceWay; }
	virtual long GetFlatLongSpliceType() { return m_nFlatLongSpliceType; }
	virtual BOOL GetFlatLongAlterSpliceSet() { return m_bFlatLongAlterSpliceSet; }
	virtual long GetFlatLongAlterSpliceValue1() { return m_nFlatLongAlterSpliceValue1; }

	virtual BOOL GetFlatShortMainBarInfoSet() { return m_bFlatShortMainBarInfoSet; }
	virtual long GetFlatShortMaxDualHookLength() { return m_nFlatShortMaxDualHookLength; }
	virtual long GetFlatShortSpliceWay() { return m_nFlatShortSpliceWay; }
	virtual long GetFlatShortSpliceType() { return m_nFlatShortSpliceType; }
	virtual BOOL GetFlatShortAlterSpliceSet() { return m_bFlatShortAlterSpliceSet; }
	virtual long GetFlatShortAlterSpliceValue1() { return m_nFlatShortAlterSpliceValue1; }

	virtual BOOL GetFlatEndInfoSet() { return m_bFlatEndInfoSet; }
	virtual long GetFlatTopEndHookType() { return m_nFlatTopEndHookType; }
	virtual long GetFlatBotEndHookType() { return m_nFlatBotEndHookType; }
	virtual long GetFlatIsoFootDevType() { return m_nFlatIsoFootDevType; }
	virtual long GetFlatSpliceDevSlabType() { return m_nFlatSpliceDevSlabType; }

	virtual BOOL GetFlatLevelDifInfoSet() { return m_bFlatLevelDifInfoSet; }
	virtual long GetFlatLevelDifPlaceShape() { return m_nFlatLevelDifPlaceShape; }
	virtual long GetFlatLevelDifHaunchPlaceType() { return m_nFlatLevelDifHaunchPlaceType; }

	virtual BOOL GetFlatOpeningInfoSet() { return m_bFlatOpeningInfoSet; }
	virtual long GetFlatOpeningTopHookType() { return m_nFlatOpeningTopHookType; }
	virtual long GetFlatOpeningBotHookType() { return m_nFlatOpeningBotHookType; }

	virtual BOOL GetFlatRebarAreaInfoSet() { return m_bFlatRebarAreaInfoSet; }
	virtual long GetFlatRebarTolDist() { return m_nFlatRebarTolDist; }
	virtual long GetFlatRebarTolLenDif() { return m_nFlatRebarTolLenDif; }

	virtual BOOL GetFlatTopSpacerInfoSet() { return m_bFlatTopSpacerInfoSet; }
	virtual BOOL GetFlatTopSpacerSet() { return m_bFlatTopSpacerSet; }
	virtual long GetFlatTopSpacerDiaType() { return m_nFlatTopSpacerDiaType; }
	virtual long GetFlatTopSpacerDiaIndex() { return m_nFlatTopSpacerDiaIndex; }
	virtual long GetFlatTopSpacerSpacingType() { return m_nFlatTopSpacerSpacingType; }
	virtual long GetFlatTopSpacerSlabSpacing() { return m_nFlatTopSpacerSlabSpacing; }
	virtual long GetFlatTopSpacerUserSpacing1() { return m_nFlatTopSpacerUserSpacing1; }
	virtual long GetFlatTopSpacerUserSpacing2() { return m_nFlatTopSpacerUserSpacing2; }
	virtual long GetFlatTopSpacerHeightType() { return m_nFlatTopSpacerHeightType; }
	virtual long GetFlatTopSpacerUserHeight() { return m_nFlatTopSpacerUserHeight; }
	virtual long GetFlatTopSpacerHookLenAType() { return m_nFlatTopSpacerHookLenAType; }
	virtual long GetFlatTopSpacerMainBarHookLenA() { return m_nFlatTopSpacerMainBarHookLenA; }
	virtual long GetFlatTopSpacerUserHookLenA() { return m_nFlatTopSpacerUserHookLenA; }
	virtual long GetFlatTopSpacerHookLenBType() { return m_nFlatTopSpacerHookLenBType; }
	virtual long GetFlatTopSpacerMainBarHookLenB() { return m_nFlatTopSpacerMainBarHookLenB; }
	virtual long GetFlatTopSpacerUserHookLenB() { return m_nFlatTopSpacerUserHookLenB; }
	virtual long GetFlatTopSpacerWidth() { return m_nFlatTopSpacerWidth; }

	virtual BOOL GetFlatBotSpacerInfoSet() { return m_bFlatBotSpacerInfoSet; }
	virtual BOOL GetFlatBotSpacerSet() { return m_bFlatBotSpacerSet; }
	virtual long GetFlatBotSpacerDiaType() { return m_nFlatBotSpacerDiaType; }
	virtual long GetFlatBotSpacerDiaIndex() { return m_nFlatBotSpacerDiaIndex; }
	virtual long GetFlatBotSpacerSpacingType() { return m_nFlatBotSpacerSpacingType; }
	virtual long GetFlatBotSpacerSlabSpacing() { return m_nFlatBotSpacerSlabSpacing; }
	virtual long GetFlatBotSpacerUserSpacing1() { return m_nFlatBotSpacerUserSpacing1; }
	virtual long GetFlatBotSpacerUserSpacing2() { return m_nFlatBotSpacerUserSpacing2; }
	virtual long GetFlatBotSpacerHeightType() { return m_nFlatBotSpacerHeightType; }
	virtual long GetFlatBotSpacerUserHeight() { return m_nFlatBotSpacerUserHeight; }
	virtual long GetFlatBotSpacerHookLenAType() { return m_nFlatBotSpacerHookLenAType; }
	virtual long GetFlatBotSpacerMainBarHookLenA() { return m_nFlatBotSpacerMainBarHookLenA; }
	virtual long GetFlatBotSpacerUserHookLenA() { return m_nFlatBotSpacerUserHookLenA; }
	virtual long GetFlatBotSpacerHookLenBType() { return m_nFlatBotSpacerHookLenBType; }
	virtual long GetFlatBotSpacerMainBarHookLenB() { return m_nFlatBotSpacerMainBarHookLenB; }
	virtual long GetFlatBotSpacerUserHookLenB() { return m_nFlatBotSpacerUserHookLenB; }
	virtual long GetFlatBotSpacerWidth() { return m_nFlatBotSpacerWidth; }

	virtual void SetFlatLongMainBarInfoSet(BOOL value) { m_bFlatLongMainBarInfoSet = value; }
	virtual void SetFlatLongMaxDualHookLength(long value) { m_nFlatLongMaxDualHookLength = value; }
	virtual void SetFlatLongSpliceWay(long value) { m_nFlatLongSpliceWay = value; }
	virtual void SetFlatLongSpliceType(long value) { m_nFlatLongSpliceType = value; }
	virtual void SetFlatLongAlterSpliceSet(BOOL value) { m_bFlatLongAlterSpliceSet = value; }
	virtual void SetFlatLongAlterSpliceValue1(long value) { m_nFlatLongAlterSpliceValue1 = value; }

	virtual void SetFlatShortMainBarInfoSet(BOOL value) { m_bFlatShortMainBarInfoSet = value; }
	virtual void SetFlatShortMaxDualHookLength(long value) { m_nFlatShortMaxDualHookLength = value; }
	virtual void SetFlatShortSpliceWay(long value) { m_nFlatShortSpliceWay = value; }
	virtual void SetFlatShortSpliceType(long value) { m_nFlatShortSpliceType = value; }
	virtual void SetFlatShortAlterSpliceSet(BOOL value) { m_bFlatShortAlterSpliceSet = value; }
	virtual void SetFlatShortAlterSpliceValue1(long value) { m_nFlatShortAlterSpliceValue1 = value; }

	virtual void SetFlatEndInfoSet(BOOL value) { m_bFlatEndInfoSet = value; }
	virtual void SetFlatTopEndHookType(long value) { m_nFlatTopEndHookType = value; }
	virtual void SetFlatBotEndHookType(long value) { m_nFlatBotEndHookType = value; }
	virtual void SetFlatIsoFootDevType(long value) { m_nFlatIsoFootDevType = value; }
	virtual void SetFlatSpliceDevSlabType(long value) { m_nFlatSpliceDevSlabType = value; }

	virtual void SetFlatLevelDifInfoSet(BOOL value) { m_bFlatLevelDifInfoSet = value; }
	virtual void SetFlatLevelDifPlaceShape(long value) { m_nFlatLevelDifPlaceShape = value; }
	virtual void SetFlatLevelDifHaunchPlaceType(long value) { m_nFlatLevelDifHaunchPlaceType = value; }

	virtual void SetFlatOpeningInfoSet(BOOL value) { m_bFlatOpeningInfoSet = value; }
	virtual void SetFlatOpeningTopHookType(long value) { m_nFlatOpeningTopHookType = value; }
	virtual void SetFlatOpeningBotHookType(long value) { m_nFlatOpeningBotHookType = value; }

	virtual void SetFlatRebarAreaInfoSet(BOOL value) { m_bFlatRebarAreaInfoSet = value; }
	virtual void SetFlatRebarTolDist(long value) { m_nFlatRebarTolDist = value; }
	virtual void SetFlatRebarTolLenDif(long value) { m_nFlatRebarTolLenDif = value; }

	virtual void SetFlatTopSpacerInfoSet(BOOL value) { m_bFlatTopSpacerInfoSet = value; }
	virtual void SetFlatTopSpacerSet(BOOL value) { m_bFlatTopSpacerSet = value; }
	virtual void SetFlatTopSpacerDiaType(long value) { m_nFlatTopSpacerDiaType = value; }
	virtual void SetFlatTopSpacerDiaIndex(long value) { m_nFlatTopSpacerDiaIndex = value; }
	virtual void SetFlatTopSpacerSpacingType(long value) { m_nFlatTopSpacerSpacingType = value; }
	virtual void SetFlatTopSpacerSlabSpacing(long value) { m_nFlatTopSpacerSlabSpacing = value; }
	virtual void SetFlatTopSpacerUserSpacing1(long value) { m_nFlatTopSpacerUserSpacing1 = value; }
	virtual void SetFlatTopSpacerUserSpacing2(long value) { m_nFlatTopSpacerUserSpacing2 = value; }
	virtual void SetFlatTopSpacerHeightType(long value) { m_nFlatTopSpacerHeightType = value; }
	virtual void SetFlatTopSpacerUserHeight(long value) { m_nFlatTopSpacerUserHeight = value; }
	virtual void SetFlatTopSpacerHookLenAType(long value) { m_nFlatTopSpacerHookLenAType = value; }
	virtual void SetFlatTopSpacerMainBarHookLenA(long value) { m_nFlatTopSpacerMainBarHookLenA = value; }
	virtual void SetFlatTopSpacerUserHookLenA(long value) { m_nFlatTopSpacerUserHookLenA = value; }
	virtual void SetFlatTopSpacerHookLenBType(long value) { m_nFlatTopSpacerHookLenBType = value; }
	virtual void SetFlatTopSpacerMainBarHookLenB(long value) { m_nFlatTopSpacerMainBarHookLenB = value; }
	virtual void SetFlatTopSpacerUserHookLenB(long value) { m_nFlatTopSpacerUserHookLenB = value; }
	virtual void SetFlatTopSpacerWidth(long value) { m_nFlatTopSpacerWidth = value; }

	virtual void SetFlatBotSpacerInfoSet(BOOL value) { m_bFlatBotSpacerInfoSet = value; }
	virtual void SetFlatBotSpacerSet(BOOL value) { m_bFlatBotSpacerSet = value; }
	virtual void SetFlatBotSpacerDiaType(long value) { m_nFlatBotSpacerDiaType = value; }
	virtual void SetFlatBotSpacerDiaIndex(long value) { m_nFlatBotSpacerDiaIndex = value; }
	virtual void SetFlatBotSpacerSpacingType(long value) { m_nFlatBotSpacerSpacingType = value; }
	virtual void SetFlatBotSpacerSlabSpacing(long value) { m_nFlatBotSpacerSlabSpacing = value; }
	virtual void SetFlatBotSpacerUserSpacing1(long value) { m_nFlatBotSpacerUserSpacing1 = value; }
	virtual void SetFlatBotSpacerUserSpacing2(long value) { m_nFlatBotSpacerUserSpacing2 = value; }
	virtual void SetFlatBotSpacerHeightType(long value) { m_nFlatBotSpacerHeightType = value; }
	virtual void SetFlatBotSpacerUserHeight(long value) { m_nFlatBotSpacerUserHeight = value; }
	virtual void SetFlatBotSpacerHookLenAType(long value) { m_nFlatBotSpacerHookLenAType = value; }
	virtual void SetFlatBotSpacerMainBarHookLenA(long value) { m_nFlatBotSpacerMainBarHookLenA = value; }
	virtual void SetFlatBotSpacerUserHookLenA(long value) { m_nFlatBotSpacerUserHookLenA = value; }
	virtual void SetFlatBotSpacerHookLenBType(long value) { m_nFlatBotSpacerHookLenBType = value; }
	virtual void SetFlatBotSpacerMainBarHookLenB(long value) { m_nFlatBotSpacerMainBarHookLenB = value; }
	virtual void SetFlatBotSpacerUserHookLenB(long value) { m_nFlatBotSpacerUserHookLenB = value; }
	virtual void SetFlatBotSpacerWidth(long value) { m_nFlatBotSpacerWidth = value; }

	// 사변지지 SLAB
	virtual BOOL GetSlabLongMainBarInfoSet() { return m_bSlabLongMainBarInfoSet; }
	virtual long GetSlabLongMaxDualHookLength() { return m_nSlabLongMaxDualHookLength; }
	virtual long GetSlabLongSpliceWay() { return m_nSlabLongSpliceWay; }
	virtual long GetSlabLongSpliceType() { return m_nSlabLongSpliceType; }
	virtual BOOL GetSlabLongAlterSpliceSet() { return m_bSlabLongAlterSpliceSet; }
	virtual long GetSlabLongAlterSpliceValue1() { return m_nSlabLongAlterSpliceValue1; }

	virtual BOOL GetSlabShortMainBarInfoSet() { return m_bSlabShortMainBarInfoSet; }
	virtual long GetSlabShortMaxDualHookLength() { return m_nSlabShortMaxDualHookLength; }
	virtual long GetSlabShortSpliceWay() { return m_nSlabShortSpliceWay; }
	virtual long GetSlabShortSpliceType() { return m_nSlabShortSpliceType; }
	virtual BOOL GetSlabShortAlterSpliceSet() { return m_bSlabShortAlterSpliceSet; }
	virtual long GetSlabShortAlterSpliceValue1() { return m_nSlabShortAlterSpliceValue1; }

	virtual BOOL GetSlabStructuredEndInfoSet() { return m_bSlabStructuredEndInfoSet; }
	virtual long GetSlabTopStructuredEndHookType() { return m_nSlabTopStructuredEndHookType; }
	virtual long GetSlabBotStructuredEndHookType() { return m_nSlabBotStructuredEndHookType; }

	virtual BOOL GetSlabTopCutBarLenInfoSet() { return m_bSlabTopCutBarLenInfoSet; }
	virtual long GetSlabOneWayCutBarLenType() { return m_nSlabOneWayCutBarLenType; }
	virtual long GetSlabTwoWayCutBarLenType() { return m_nSlabTwoWayCutBarLenType; }

	virtual BOOL GetSlabUnStructuredEndInfoSet() { return m_bSlabUnStructuredEndInfoSet; }
	virtual long GetSlabTopUnStructuredEndHookType() { return m_nSlabTopUnStructuredEndHookType; }
	virtual long GetSlabBotUnStructuredEndHookType() { return m_nSlabBotUnStructuredEndHookType; }
	virtual long GetSlabIsoFootUnStructuredDevType() { return m_nSlabIsoFootUnStructuredDevType; }

	virtual BOOL GetSlabLevelDifInfoSet() { return m_bSlabLevelDifInfoSet; }
	virtual long GetSlabLevelDifPlaceShape() { return m_nSlabLevelDifPlaceShape; }
	virtual long GetSlabLevelDifHaunchPlaceType() { return m_nSlabLevelDifHaunchPlaceType; }

	virtual BOOL GetSlabOpeningInfoSet() { return m_bSlabOpeningInfoSet; }
	virtual long GetSlabOpeningTopHookType() { return m_nSlabOpeningTopHookType; }
	virtual long GetSlabOpeningBotHookType() { return m_nSlabOpeningBotHookType; }

	virtual BOOL GetSlabRebarAreaInfoSet() { return m_bSlabRebarAreaInfoSet; }
	virtual long GetSlabRebarTolDist() { return m_nSlabRebarTolDist; }
	virtual long GetSlabRebarTolLenDif() { return m_nSlabRebarTolLenDif; }

	virtual BOOL GetSlabTopSpacerInfoSet() { return m_bSlabTopSpacerInfoSet; }
	virtual BOOL GetSlabTopSpacerSet() { return m_bSlabTopSpacerSet; }
	virtual long GetSlabTopSpacerDiaType() { return m_nSlabTopSpacerDiaType; }
	virtual long GetSlabTopSpacerDiaIndex() { return m_nSlabTopSpacerDiaIndex; }
	virtual long GetSlabTopSpacerSpacingType() { return m_nSlabTopSpacerSpacingType; }
	virtual long GetSlabTopSpacerSlabSpacing() { return m_nSlabTopSpacerSlabSpacing; }
	virtual long GetSlabTopSpacerUserSpacing1() { return m_nSlabTopSpacerUserSpacing1; }
	virtual long GetSlabTopSpacerUserSpacing2() { return m_nSlabTopSpacerUserSpacing2; }
	virtual long GetSlabTopSpacerHeightType() { return m_nSlabTopSpacerHeightType; }
	virtual long GetSlabTopSpacerUserHeight() { return m_nSlabTopSpacerUserHeight; }
	virtual long GetSlabTopSpacerHookLenAType() { return m_nSlabTopSpacerHookLenAType; }
	virtual long GetSlabTopSpacerMainBarHookLenA() { return m_nSlabTopSpacerMainBarHookLenA; }
	virtual long GetSlabTopSpacerUserHookLenA() { return m_nSlabTopSpacerUserHookLenA; }
	virtual long GetSlabTopSpacerHookLenBType() { return m_nSlabTopSpacerHookLenBType; }
	virtual long GetSlabTopSpacerMainBarHookLenB() { return m_nSlabTopSpacerMainBarHookLenB; }
	virtual long GetSlabTopSpacerUserHookLenB() { return m_nSlabTopSpacerUserHookLenB; }
	virtual long GetSlabTopSpacerWidth() { return m_nSlabTopSpacerWidth; }

	virtual BOOL GetSlabBotSpacerInfoSet() { return m_bSlabBotSpacerInfoSet; }
	virtual BOOL GetSlabBotSpacerSet() { return m_bSlabBotSpacerSet; }
	virtual long GetSlabBotSpacerDiaType() { return m_nSlabBotSpacerDiaType; }
	virtual long GetSlabBotSpacerDiaIndex() { return m_nSlabBotSpacerDiaIndex; }
	virtual long GetSlabBotSpacerSpacingType() { return m_nSlabBotSpacerSpacingType; }
	virtual long GetSlabBotSpacerSlabSpacing() { return m_nSlabBotSpacerSlabSpacing; }
	virtual long GetSlabBotSpacerUserSpacing1() { return m_nSlabBotSpacerUserSpacing1; }
	virtual long GetSlabBotSpacerUserSpacing2() { return m_nSlabBotSpacerUserSpacing2; }
	virtual long GetSlabBotSpacerHeightType() { return m_nSlabBotSpacerHeightType; }
	virtual long GetSlabBotSpacerUserHeight() { return m_nSlabBotSpacerUserHeight; }
	virtual long GetSlabBotSpacerHookLenAType() { return m_nSlabBotSpacerHookLenAType; }
	virtual long GetSlabBotSpacerMainBarHookLenA() { return m_nSlabBotSpacerMainBarHookLenA; }
	virtual long GetSlabBotSpacerUserHookLenA() { return m_nSlabBotSpacerUserHookLenA; }
	virtual long GetSlabBotSpacerHookLenBType() { return m_nSlabBotSpacerHookLenBType; }
	virtual long GetSlabBotSpacerMainBarHookLenB() { return m_nSlabBotSpacerMainBarHookLenB; }
	virtual long GetSlabBotSpacerUserHookLenB() { return m_nSlabBotSpacerUserHookLenB; }
	virtual long GetSlabBotSpacerWidth() { return m_nSlabBotSpacerWidth; }

	virtual void SetSlabLongMainBarInfoSet(BOOL value) { m_bSlabLongMainBarInfoSet = value; }
	virtual void SetSlabLongMaxDualHookLength(long value) { m_nSlabLongMaxDualHookLength = value; }
	virtual void SetSlabLongSpliceWay(long value) { m_nSlabLongSpliceWay = value; }
	virtual void SetSlabLongSpliceType(long value) { m_nSlabLongSpliceType = value; }
	virtual void SetSlabLongAlterSpliceSet(BOOL value) { m_bSlabLongAlterSpliceSet = value; }
	virtual void SetSlabLongAlterSpliceValue1(long value) { m_nSlabLongAlterSpliceValue1 = value; }

	virtual void SetSlabShortMainBarInfoSet(BOOL value) { m_bSlabShortMainBarInfoSet = value; }
	virtual void SetSlabShortMaxDualHookLength(long value) { m_nSlabShortMaxDualHookLength = value; }
	virtual void SetSlabShortSpliceWay(long value) { m_nSlabShortSpliceWay = value; }
	virtual void SetSlabShortSpliceType(long value) { m_nSlabShortSpliceType = value; }
	virtual void SetSlabShortAlterSpliceSet(BOOL value) { m_bSlabShortAlterSpliceSet = value; }
	virtual void SetSlabShortAlterSpliceValue1(long value) { m_nSlabShortAlterSpliceValue1 = value; }

	virtual void SetSlabStructuredEndInfoSet(BOOL value) { m_bSlabStructuredEndInfoSet = value; }
	virtual void SetSlabTopStructuredEndHookType(long value) { m_nSlabTopStructuredEndHookType = value; }
	virtual void SetSlabBotStructuredEndHookType(long value) { m_nSlabBotStructuredEndHookType = value; }

	virtual void SetSlabTopCutBarLenInfoSet(BOOL value) { m_bSlabTopCutBarLenInfoSet = value; }
	virtual void SetSlabOneWayCutBarLenType(long value) { m_nSlabOneWayCutBarLenType = value; }
	virtual void SetSlabTwoWayCutBarLenType(long value) { m_nSlabTwoWayCutBarLenType = value; }

	virtual void SetSlabUnStructuredEndInfoSet(BOOL value) { m_bSlabUnStructuredEndInfoSet = value; }
	virtual void SetSlabTopUnStructuredEndHookType(long value) { m_nSlabTopUnStructuredEndHookType = value; }
	virtual void SetSlabBotUnStructuredEndHookType(long value) { m_nSlabBotUnStructuredEndHookType = value; }
	virtual void SetSlabIsoFootUnStructuredDevType(long value) { m_nSlabIsoFootUnStructuredDevType = value; }

	virtual void SetSlabLevelDifInfoSet(BOOL value) { m_bSlabLevelDifInfoSet = value; }
	virtual void SetSlabLevelDifPlaceShape(long value) { m_nSlabLevelDifPlaceShape = value; }
	virtual void SetSlabLevelDifHaunchPlaceType(long value) { m_nSlabLevelDifHaunchPlaceType = value; }

	virtual void SetSlabOpeningInfoSet(BOOL value) { m_bSlabOpeningInfoSet = value; }
	virtual void SetSlabOpeningTopHookType(long value) { m_nSlabOpeningTopHookType = value; }
	virtual void SetSlabOpeningBotHookType(long value) { m_nSlabOpeningBotHookType = value; }

	virtual void SetSlabRebarAreaInfoSet(BOOL value) { m_bSlabRebarAreaInfoSet = value; }
	virtual void SetSlabRebarTolDist(long value) { m_nSlabRebarTolDist = value; }
	virtual void SetSlabRebarTolLenDif(long value) { m_nSlabRebarTolLenDif = value; }

	virtual void SetSlabTopSpacerInfoSet(BOOL value) { m_bSlabTopSpacerInfoSet = value; }
	virtual void SetSlabTopSpacerSet(BOOL value) { m_bSlabTopSpacerSet = value; }
	virtual void SetSlabTopSpacerDiaType(long value) { m_nSlabTopSpacerDiaType = value; }
	virtual void SetSlabTopSpacerDiaIndex(long value) { m_nSlabTopSpacerDiaIndex = value; }
	virtual void SetSlabTopSpacerSpacingType(long value) { m_nSlabTopSpacerSpacingType = value; }
	virtual void SetSlabTopSpacerSlabSpacing(long value) { m_nSlabTopSpacerSlabSpacing = value; }
	virtual void SetSlabTopSpacerUserSpacing1(long value) { m_nSlabTopSpacerUserSpacing1 = value; }
	virtual void SetSlabTopSpacerUserSpacing2(long value) { m_nSlabTopSpacerUserSpacing2 = value; }
	virtual void SetSlabTopSpacerHeightType(long value) { m_nSlabTopSpacerHeightType = value; }
	virtual void SetSlabTopSpacerUserHeight(long value) { m_nSlabTopSpacerUserHeight = value; }
	virtual void SetSlabTopSpacerHookLenAType(long value) { m_nSlabTopSpacerHookLenAType = value; }
	virtual void SetSlabTopSpacerMainBarHookLenA(long value) { m_nSlabTopSpacerMainBarHookLenA = value; }
	virtual void SetSlabTopSpacerUserHookLenA(long value) { m_nSlabTopSpacerUserHookLenA = value; }
	virtual void SetSlabTopSpacerHookLenBType(long value) { m_nSlabTopSpacerHookLenBType = value; }
	virtual void SetSlabTopSpacerMainBarHookLenB(long value) { m_nSlabTopSpacerMainBarHookLenB = value; }
	virtual void SetSlabTopSpacerUserHookLenB(long value) { m_nSlabTopSpacerUserHookLenB = value; }
	virtual void SetSlabTopSpacerWidth(long value) { m_nSlabTopSpacerWidth = value; }

	virtual void SetSlabBotSpacerInfoSet(BOOL value) { m_bSlabBotSpacerInfoSet = value; }
	virtual void SetSlabBotSpacerSet(BOOL value) { m_bSlabBotSpacerSet = value; }
	virtual void SetSlabBotSpacerDiaType(long value) { m_nSlabBotSpacerDiaType = value; }
	virtual void SetSlabBotSpacerDiaIndex(long value) { m_nSlabBotSpacerDiaIndex = value; }
	virtual void SetSlabBotSpacerSpacingType(long value) { m_nSlabBotSpacerSpacingType = value; }
	virtual void SetSlabBotSpacerSlabSpacing(long value) { m_nSlabBotSpacerSlabSpacing = value; }
	virtual void SetSlabBotSpacerUserSpacing1(long value) { m_nSlabBotSpacerUserSpacing1 = value; }
	virtual void SetSlabBotSpacerUserSpacing2(long value) { m_nSlabBotSpacerUserSpacing2 = value; }
	virtual void SetSlabBotSpacerHeightType(long value) { m_nSlabBotSpacerHeightType = value; }
	virtual void SetSlabBotSpacerUserHeight(long value) { m_nSlabBotSpacerUserHeight = value; }
	virtual void SetSlabBotSpacerHookLenAType(long value) { m_nSlabBotSpacerHookLenAType = value; }
	virtual void SetSlabBotSpacerMainBarHookLenA(long value) { m_nSlabBotSpacerMainBarHookLenA = value; }
	virtual void SetSlabBotSpacerUserHookLenA(long value) { m_nSlabBotSpacerUserHookLenA = value; }
	virtual void SetSlabBotSpacerHookLenBType(long value) { m_nSlabBotSpacerHookLenBType = value; }
	virtual void SetSlabBotSpacerMainBarHookLenB(long value) { m_nSlabBotSpacerMainBarHookLenB = value; }
	virtual void SetSlabBotSpacerUserHookLenB(long value) { m_nSlabBotSpacerUserHookLenB = value; }
	virtual void SetSlabBotSpacerWidth(long value) { m_nSlabBotSpacerWidth = value; }

	// SOG Slab
	virtual BOOL GetSOGLongMainBarInfoSet() { return m_bSOGLongMainBarInfoSet; }
	virtual long GetSOGLongMaxDualHookLength() { return m_nSOGLongMaxDualHookLength; }
	virtual long GetSOGLongSpliceWay() { return m_nSOGLongSpliceWay; }
	virtual long GetSOGLongSpliceType() { return m_nSOGLongSpliceType; }
	virtual BOOL GetSOGLongAlterSpliceSet() { return m_bSOGLongAlterSpliceSet; }
	virtual long GetSOGLongAlterSpliceValue1() { return m_nSOGLongAlterSpliceValue1; }

	virtual BOOL GetSOGShortMainBarInfoSet() { return m_bSOGShortMainBarInfoSet; }
	virtual long GetSOGShortMaxDualHookLength() { return m_nSOGShortMaxDualHookLength; }
	virtual long GetSOGShortSpliceWay() { return m_nSOGShortSpliceWay; }
	virtual long GetSOGShortSpliceType() { return m_nSOGShortSpliceType; }
	virtual BOOL GetSOGShortAlterSpliceSet() { return m_bSOGShortAlterSpliceSet; }
	virtual long GetSOGShortAlterSpliceValue1() { return m_nSOGShortAlterSpliceValue1; }

	virtual BOOL GetSOGEndInfoSet() { return m_bSOGEndInfoSet; }
	virtual long GetSOGTopEndHookType() { return m_nSOGTopEndHookType; }
	virtual long GetSOGBotEndHookType() { return m_nSOGBotEndHookType; }
	virtual long GetSOGIsoFootDevType() { return m_nSOGIsoFootDevType; }
	virtual long GetSOGSpliceDevSlabType() { return m_nSOGSpliceDevSlabType; }

	virtual BOOL GetSOGLevelDifInfoSet() { return m_bSOGLevelDifInfoSet; }
	virtual long GetSOGLevelDifPlaceShape() { return m_nSOGLevelDifPlaceShape; }
	virtual long GetSOGLevelDifHaunchPlaceType() { return m_nSOGLevelDifHaunchPlaceType; }

	virtual BOOL GetSOGOpeningInfoSet() { return m_bSOGOpeningInfoSet; }
	virtual long GetSOGOpeningTopHookType() { return m_nSOGOpeningTopHookType; }
	virtual long GetSOGOpeningBotHookType() { return m_nSOGOpeningBotHookType; }

	virtual BOOL GetSOGRebarAreaInfoSet() { return m_bSOGRebarAreaInfoSet; }
	virtual long GetSOGRebarTolDist() { return m_nSOGRebarTolDist; }
	virtual long GetSOGRebarTolLenDif() { return m_nSOGRebarTolLenDif; }

	virtual BOOL GetSOGTopSpacerInfoSet() { return m_bSOGTopSpacerInfoSet; }
	virtual BOOL GetSOGTopSpacerSet() { return m_bSOGTopSpacerSet; }
	virtual long GetSOGTopSpacerDiaType() { return m_nSOGTopSpacerDiaType; }
	virtual long GetSOGTopSpacerDiaIndex() { return m_nSOGTopSpacerDiaIndex; }
	virtual long GetSOGTopSpacerSpacingType() { return m_nSOGTopSpacerSpacingType; }
	virtual long GetSOGTopSpacerSlabSpacing() { return m_nSOGTopSpacerSlabSpacing; }
	virtual long GetSOGTopSpacerUserSpacing1() { return m_nSOGTopSpacerUserSpacing1; }
	virtual long GetSOGTopSpacerUserSpacing2() { return m_nSOGTopSpacerUserSpacing2; }
	virtual long GetSOGTopSpacerHeightType() { return m_nSOGTopSpacerHeightType; }
	virtual long GetSOGTopSpacerUserHeight() { return m_nSOGTopSpacerUserHeight; }
	virtual long GetSOGTopSpacerHookLenAType() { return m_nSOGTopSpacerHookLenAType; }
	virtual long GetSOGTopSpacerMainBarHookLenA() { return m_nSOGTopSpacerMainBarHookLenA; }
	virtual long GetSOGTopSpacerUserHookLenA() { return m_nSOGTopSpacerUserHookLenA; }
	virtual long GetSOGTopSpacerHookLenBType() { return m_nSOGTopSpacerHookLenBType; }
	virtual long GetSOGTopSpacerMainBarHookLenB() { return m_nSOGTopSpacerMainBarHookLenB; }
	virtual long GetSOGTopSpacerUserHookLenB() { return m_nSOGTopSpacerUserHookLenB; }
	virtual long GetSOGTopSpacerWidth() { return m_nSOGTopSpacerWidth; }

	virtual BOOL GetSOGBotSpacerInfoSet() { return m_bSOGBotSpacerInfoSet; }
	virtual BOOL GetSOGBotSpacerSet() { return m_bSOGBotSpacerSet; }
	virtual long GetSOGBotSpacerDiaType() { return m_nSOGBotSpacerDiaType; }
	virtual long GetSOGBotSpacerDiaIndex() { return m_nSOGBotSpacerDiaIndex; }
	virtual long GetSOGBotSpacerSpacingType() { return m_nSOGBotSpacerSpacingType; }
	virtual long GetSOGBotSpacerSlabSpacing() { return m_nSOGBotSpacerSlabSpacing; }
	virtual long GetSOGBotSpacerUserSpacing1() { return m_nSOGBotSpacerUserSpacing1; }
	virtual long GetSOGBotSpacerUserSpacing2() { return m_nSOGBotSpacerUserSpacing2; }
	virtual long GetSOGBotSpacerHeightType() { return m_nSOGBotSpacerHeightType; }
	virtual long GetSOGBotSpacerUserHeight() { return m_nSOGBotSpacerUserHeight; }
	virtual long GetSOGBotSpacerHookLenAType() { return m_nSOGBotSpacerHookLenAType; }
	virtual long GetSOGBotSpacerMainBarHookLenA() { return m_nSOGBotSpacerMainBarHookLenA; }
	virtual long GetSOGBotSpacerUserHookLenA() { return m_nSOGBotSpacerUserHookLenA; }
	virtual long GetSOGBotSpacerHookLenBType() { return m_nSOGBotSpacerHookLenBType; }
	virtual long GetSOGBotSpacerMainBarHookLenB() { return m_nSOGBotSpacerMainBarHookLenB; }
	virtual long GetSOGBotSpacerUserHookLenB() { return m_nSOGBotSpacerUserHookLenB; }
	virtual long GetSOGBotSpacerWidth() { return m_nSOGBotSpacerWidth; }

	virtual void SetSOGLongMainBarInfoSet(BOOL value) { m_bSOGLongMainBarInfoSet = value; }
	virtual void SetSOGLongMaxDualHookLength(long value) { m_nSOGLongMaxDualHookLength = value; }
	virtual void SetSOGLongSpliceWay(long value) { m_nSOGLongSpliceWay = value; }
	virtual void SetSOGLongSpliceType(long value) { m_nSOGLongSpliceType = value; }
	virtual void SetSOGLongAlterSpliceSet(BOOL value) { m_bSOGLongAlterSpliceSet = value; }
	virtual void SetSOGLongAlterSpliceValue1(long value) { m_nSOGLongAlterSpliceValue1 = value; }

	virtual void SetSOGShortMainBarInfoSet(BOOL value) { m_bSOGShortMainBarInfoSet = value; }
	virtual void SetSOGShortMaxDualHookLength(long value) { m_nSOGShortMaxDualHookLength = value; }
	virtual void SetSOGShortSpliceWay(long value) { m_nSOGShortSpliceWay = value; }
	virtual void SetSOGShortSpliceType(long value) { m_nSOGShortSpliceType = value; }
	virtual void SetSOGShortAlterSpliceSet(BOOL value) { m_bSOGShortAlterSpliceSet = value; }
	virtual void SetSOGShortAlterSpliceValue1(long value) { m_nSOGShortAlterSpliceValue1 = value; }

	virtual void SetSOGEndInfoSet(BOOL value) { m_bSOGEndInfoSet = value; }
	virtual void SetSOGTopEndHookType(long value) { m_nSOGTopEndHookType = value; }
	virtual void SetSOGBotEndHookType(long value) { m_nSOGBotEndHookType = value; }
	virtual void SetSOGIsoFootDevType(long value) { m_nSOGIsoFootDevType = value; }
	virtual void SetSOGSpliceDevSlabType(long value) { m_nSOGSpliceDevSlabType = value; }

	virtual void SetSOGLevelDifInfoSet(BOOL value) { m_bSOGLevelDifInfoSet = value; }
	virtual void SetSOGLevelDifPlaceShape(long value) { m_nSOGLevelDifPlaceShape = value; }
	virtual void SetSOGLevelDifHaunchPlaceType(long value) { m_nSOGLevelDifHaunchPlaceType = value; }

	virtual void SetSOGOpeningInfoSet(BOOL value) { m_bSOGOpeningInfoSet = value; }
	virtual void SetSOGOpeningTopHookType(long value) { m_nSOGOpeningTopHookType = value; }
	virtual void SetSOGOpeningBotHookType(long value) { m_nSOGOpeningBotHookType = value; }

	virtual void SetSOGRebarAreaInfoSet(BOOL value) { m_bSOGRebarAreaInfoSet = value; }
	virtual void SetSOGRebarTolDist(long value) { m_nSOGRebarTolDist = value; }
	virtual void SetSOGRebarTolLenDif(long value) { m_nSOGRebarTolLenDif = value; }
	
	virtual void SetSOGTopSpacerInfoSet(BOOL value) { m_bSOGTopSpacerInfoSet = value; }
	virtual void SetSOGTopSpacerSet(BOOL value) { m_bSOGTopSpacerSet = value; }
	virtual void SetSOGTopSpacerDiaType(long value) { m_nSOGTopSpacerDiaType = value; }
	virtual void SetSOGTopSpacerDiaIndex(long value) { m_nSOGTopSpacerDiaIndex = value; }
	virtual void SetSOGTopSpacerSpacingType(long value) { m_nSOGTopSpacerSpacingType = value; }
	virtual void SetSOGTopSpacerSlabSpacing(long value) { m_nSOGTopSpacerSlabSpacing = value; }
	virtual void SetSOGTopSpacerUserSpacing1(long value) { m_nSOGTopSpacerUserSpacing1 = value; }
	virtual void SetSOGTopSpacerUserSpacing2(long value) { m_nSOGTopSpacerUserSpacing2 = value; }
	virtual void SetSOGTopSpacerHeightType(long value) { m_nSOGTopSpacerHeightType = value; }
	virtual void SetSOGTopSpacerUserHeight(long value) { m_nSOGTopSpacerUserHeight = value; }
	virtual void SetSOGTopSpacerHookLenAType(long value) { m_nSOGTopSpacerHookLenAType = value; }
	virtual void SetSOGTopSpacerMainBarHookLenA(long value) { m_nSOGTopSpacerMainBarHookLenA = value; }
	virtual void SetSOGTopSpacerUserHookLenA(long value) { m_nSOGTopSpacerUserHookLenA = value; }
	virtual void SetSOGTopSpacerHookLenBType(long value) { m_nSOGTopSpacerHookLenBType = value; }
	virtual void SetSOGTopSpacerMainBarHookLenB(long value) { m_nSOGTopSpacerMainBarHookLenB = value; }
	virtual void SetSOGTopSpacerUserHookLenB(long value) { m_nSOGTopSpacerUserHookLenB = value; }
	virtual void SetSOGTopSpacerWidth(long value) { m_nSOGTopSpacerWidth = value; }

	virtual void SetSOGBotSpacerInfoSet(BOOL value) { m_bSOGBotSpacerInfoSet = value; }
	virtual void SetSOGBotSpacerSet(BOOL value) { m_bSOGBotSpacerSet = value; }
	virtual void SetSOGBotSpacerDiaType(long value) { m_nSOGBotSpacerDiaType = value; }
	virtual void SetSOGBotSpacerDiaIndex(long value) { m_nSOGBotSpacerDiaIndex = value; }
	virtual void SetSOGBotSpacerSpacingType(long value) { m_nSOGBotSpacerSpacingType = value; }
	virtual void SetSOGBotSpacerSlabSpacing(long value) { m_nSOGBotSpacerSlabSpacing = value; }
	virtual void SetSOGBotSpacerUserSpacing1(long value) { m_nSOGBotSpacerUserSpacing1 = value; }
	virtual void SetSOGBotSpacerUserSpacing2(long value) { m_nSOGBotSpacerUserSpacing2 = value; }
	virtual void SetSOGBotSpacerHeightType(long value) { m_nSOGBotSpacerHeightType = value; }
	virtual void SetSOGBotSpacerUserHeight(long value) { m_nSOGBotSpacerUserHeight = value; }
	virtual void SetSOGBotSpacerHookLenAType(long value) { m_nSOGBotSpacerHookLenAType = value; }
	virtual void SetSOGBotSpacerMainBarHookLenA(long value) { m_nSOGBotSpacerMainBarHookLenA = value; }
	virtual void SetSOGBotSpacerUserHookLenA(long value) { m_nSOGBotSpacerUserHookLenA = value; }
	virtual void SetSOGBotSpacerHookLenBType(long value) { m_nSOGBotSpacerHookLenBType = value; }
	virtual void SetSOGBotSpacerMainBarHookLenB(long value) { m_nSOGBotSpacerMainBarHookLenB = value; }
	virtual void SetSOGBotSpacerUserHookLenB(long value) { m_nSOGBotSpacerUserHookLenB = value; }
	virtual void SetSOGBotSpacerWidth(long value) { m_nSOGBotSpacerWidth = value; }

	// CircularRamp
	virtual BOOL GetCircularRampLongMainBarInfoSet() { return m_bCircularRampLongMainBarInfoSet; }
	virtual long GetCircularRampLongMaxDualHookLength() { return m_nCircularRampLongMaxDualHookLength; }
	virtual long GetCircularRampLongSpliceWay() { return m_nCircularRampLongSpliceWay; }
	virtual long GetCircularRampLongSpliceType() { return m_nCircularRampLongSpliceType; }
	virtual BOOL GetCircularRampLongAlterSpliceSet() { return m_bCircularRampLongAlterSpliceSet; }
	virtual long GetCircularRampLongAlterSpliceValue1() { return m_nCircularRampLongAlterSpliceValue1; }

	virtual BOOL GetCircularRampShortMainBarInfoSet() { return m_bCircularRampShortMainBarInfoSet; }
	virtual long GetCircularRampShortMaxDualHookLength() { return m_nCircularRampShortMaxDualHookLength; }
	virtual long GetCircularRampShortSpliceWay() { return m_nCircularRampShortSpliceWay; }
	virtual long GetCircularRampShortSpliceType() { return m_nCircularRampShortSpliceType; }
	virtual BOOL GetCircularRampShortAlterSpliceSet() { return m_bCircularRampShortAlterSpliceSet; }
	virtual long GetCircularRampShortAlterSpliceValue1() { return m_nCircularRampShortAlterSpliceValue1; }

	virtual BOOL GetCircularRampEndInfoSet() { return m_bCircularRampEndInfoSet; }
	virtual long GetCircularRampTopEndHookType() { return m_nCircularRampTopEndHookType; }
	virtual long GetCircularRampBotEndHookType() { return m_nCircularRampBotEndHookType; }
	virtual long GetCircularRampIsoFootDevType() { return m_nCircularRampIsoFootDevType; }
	virtual long GetCircularRampSpliceDevSlabType() { return m_nCircularRampSpliceDevSlabType; }

	virtual BOOL GetCircularRampLevelDifInfoSet() { return m_bCircularRampLevelDifInfoSet; }
	virtual long GetCircularRampLevelDifPlaceShape() { return m_nCircularRampLevelDifPlaceShape; }
	virtual long GetCircularRampLevelDifHaunchPlaceType() { return m_nCircularRampLevelDifHaunchPlaceType; }

	virtual BOOL GetCircularRampOpeningInfoSet() { return m_bCircularRampOpeningInfoSet; }
	virtual long GetCircularRampOpeningTopHookType() { return m_nCircularRampOpeningTopHookType; }
	virtual long GetCircularRampOpeningBotHookType() { return m_nCircularRampOpeningBotHookType; }

	virtual BOOL GetCircularRampRebarAreaInfoSet() { return m_bCircularRampRebarAreaInfoSet; }
	virtual long GetCircularRampRebarTolDist() { return m_nCircularRampRebarTolDist; }
	virtual long GetCircularRampRebarTolLenDif() { return m_nCircularRampRebarTolLenDif; }

	virtual BOOL GetCircularRampTopSpacerInfoSet() { return m_bCircularRampTopSpacerInfoSet; }
	virtual BOOL GetCircularRampTopSpacerSet() { return m_bCircularRampTopSpacerSet; }
	virtual long GetCircularRampTopSpacerDiaType() { return m_nCircularRampTopSpacerDiaType; }
	virtual long GetCircularRampTopSpacerDiaIndex() { return m_nCircularRampTopSpacerDiaIndex; }
	virtual long GetCircularRampTopSpacerSpacingType() { return m_nCircularRampTopSpacerSpacingType; }
	virtual long GetCircularRampTopSpacerSlabSpacing() { return m_nCircularRampTopSpacerSlabSpacing; }
	virtual long GetCircularRampTopSpacerUserSpacing1() { return m_nCircularRampTopSpacerUserSpacing1; }
	virtual long GetCircularRampTopSpacerUserSpacing2() { return m_nCircularRampTopSpacerUserSpacing2; }
	virtual long GetCircularRampTopSpacerHeightType() { return m_nCircularRampTopSpacerHeightType; }
	virtual long GetCircularRampTopSpacerUserHeight() { return m_nCircularRampTopSpacerUserHeight; }
	virtual long GetCircularRampTopSpacerHookLenAType() { return m_nCircularRampTopSpacerHookLenAType; }
	virtual long GetCircularRampTopSpacerMainBarHookLenA() { return m_nCircularRampTopSpacerMainBarHookLenA; }
	virtual long GetCircularRampTopSpacerUserHookLenA() { return m_nCircularRampTopSpacerUserHookLenA; }
	virtual long GetCircularRampTopSpacerHookLenBType() { return m_nCircularRampTopSpacerHookLenBType; }
	virtual long GetCircularRampTopSpacerMainBarHookLenB() { return m_nCircularRampTopSpacerMainBarHookLenB; }
	virtual long GetCircularRampTopSpacerUserHookLenB() { return m_nCircularRampTopSpacerUserHookLenB; }
	virtual long GetCircularRampTopSpacerWidth() { return m_nCircularRampTopSpacerWidth; }

	virtual BOOL GetCircularRampBotSpacerInfoSet() { return m_bCircularRampBotSpacerInfoSet; }
	virtual BOOL GetCircularRampBotSpacerSet() { return m_bCircularRampBotSpacerSet; }
	virtual long GetCircularRampBotSpacerDiaType() { return m_nCircularRampBotSpacerDiaType; }
	virtual long GetCircularRampBotSpacerDiaIndex() { return m_nCircularRampBotSpacerDiaIndex; }
	virtual long GetCircularRampBotSpacerSpacingType() { return m_nCircularRampBotSpacerSpacingType; }
	virtual long GetCircularRampBotSpacerSlabSpacing() { return m_nCircularRampBotSpacerSlabSpacing; }
	virtual long GetCircularRampBotSpacerUserSpacing1() { return m_nCircularRampBotSpacerUserSpacing1; }
	virtual long GetCircularRampBotSpacerUserSpacing2() { return m_nCircularRampBotSpacerUserSpacing2; }
	virtual long GetCircularRampBotSpacerHeightType() { return m_nCircularRampBotSpacerHeightType; }
	virtual long GetCircularRampBotSpacerUserHeight() { return m_nCircularRampBotSpacerUserHeight; }
	virtual long GetCircularRampBotSpacerHookLenAType() { return m_nCircularRampBotSpacerHookLenAType; }
	virtual long GetCircularRampBotSpacerMainBarHookLenA() { return m_nCircularRampBotSpacerMainBarHookLenA; }
	virtual long GetCircularRampBotSpacerUserHookLenA() { return m_nCircularRampBotSpacerUserHookLenA; }
	virtual long GetCircularRampBotSpacerHookLenBType() { return m_nCircularRampBotSpacerHookLenBType; }
	virtual long GetCircularRampBotSpacerMainBarHookLenB() { return m_nCircularRampBotSpacerMainBarHookLenB; }
	virtual long GetCircularRampBotSpacerUserHookLenB() { return m_nCircularRampBotSpacerUserHookLenB; }
	virtual long GetCircularRampBotSpacerWidth() { return m_nCircularRampBotSpacerWidth; }

	virtual void SetCircularRampLongMainBarInfoSet(BOOL value) { m_bCircularRampLongMainBarInfoSet = value; }
	virtual void SetCircularRampLongMaxDualHookLength(long value) { m_nCircularRampLongMaxDualHookLength = value; }
	virtual void SetCircularRampLongSpliceWay(long value) { m_nCircularRampLongSpliceWay = value; }
	virtual void SetCircularRampLongSpliceType(long value) { m_nCircularRampLongSpliceType = value; }
	virtual void SetCircularRampLongAlterSpliceSet(BOOL value) { m_bCircularRampLongAlterSpliceSet = value; }
	virtual void SetCircularRampLongAlterSpliceValue1(long value) { m_nCircularRampLongAlterSpliceValue1 = value; }

	virtual void SetCircularRampShortMainBarInfoSet(BOOL value) { m_bCircularRampShortMainBarInfoSet = value; }
	virtual void SetCircularRampShortMaxDualHookLength(long value) { m_nCircularRampShortMaxDualHookLength = value; }
	virtual void SetCircularRampShortSpliceWay(long value) { m_nCircularRampShortSpliceWay = value; }
	virtual void SetCircularRampShortSpliceType(long value) { m_nCircularRampShortSpliceType = value; }
	virtual void SetCircularRampShortAlterSpliceSet(BOOL value) { m_bCircularRampShortAlterSpliceSet = value; }
	virtual void SetCircularRampShortAlterSpliceValue1(long value) { m_nCircularRampShortAlterSpliceValue1 = value; }

	virtual void SetCircularRampEndInfoSet(BOOL value) { m_bCircularRampEndInfoSet = value; }
	virtual void SetCircularRampTopEndHookType(long value) { m_nCircularRampTopEndHookType = value; }
	virtual void SetCircularRampBotEndHookType(long value) { m_nCircularRampBotEndHookType = value; }
	virtual void SetCircularRampIsoFootDevType(long value) { m_nCircularRampIsoFootDevType = value; }
	virtual void SetCircularRampSpliceDevSlabType(long value) { m_nCircularRampSpliceDevSlabType = value; }

	virtual void SetCircularRampLevelDifInfoSet(BOOL value) { m_bCircularRampLevelDifInfoSet = value; }
	virtual void SetCircularRampLevelDifPlaceShape(long value) { m_nCircularRampLevelDifPlaceShape = value; }
	virtual void SetCircularRampLevelDifHaunchPlaceType(long value) { m_nCircularRampLevelDifHaunchPlaceType = value; }

	virtual void SetCircularRampOpeningInfoSet(BOOL value) { m_bCircularRampOpeningInfoSet = value; }
	virtual void SetCircularRampOpeningTopHookType(long value) { m_nCircularRampOpeningTopHookType = value; }
	virtual void SetCircularRampOpeningBotHookType(long value) { m_nCircularRampOpeningBotHookType = value; }

	virtual void SetCircularRampRebarAreaInfoSet(BOOL value) { m_bCircularRampRebarAreaInfoSet = value; }
	virtual void SetCircularRampRebarTolDist(long value) { m_nCircularRampRebarTolDist = value; }
	virtual void SetCircularRampRebarTolLenDif(long value) { m_nCircularRampRebarTolLenDif = value; }

	virtual void SetCircularRampTopSpacerInfoSet(BOOL value) { m_bCircularRampTopSpacerInfoSet = value; }
	virtual void SetCircularRampTopSpacerSet(BOOL value) { m_bCircularRampTopSpacerSet = value; }
	virtual void SetCircularRampTopSpacerDiaType(long value) { m_nCircularRampTopSpacerDiaType = value; }
	virtual void SetCircularRampTopSpacerDiaIndex(long value) { m_nCircularRampTopSpacerDiaIndex = value; }
	virtual void SetCircularRampTopSpacerSpacingType(long value) { m_nCircularRampTopSpacerSpacingType = value; }
	virtual void SetCircularRampTopSpacerSlabSpacing(long value) { m_nCircularRampTopSpacerSlabSpacing = value; }
	virtual void SetCircularRampTopSpacerUserSpacing1(long value) { m_nCircularRampTopSpacerUserSpacing1 = value; }
	virtual void SetCircularRampTopSpacerUserSpacing2(long value) { m_nCircularRampTopSpacerUserSpacing2 = value; }
	virtual void SetCircularRampTopSpacerHeightType(long value) { m_nCircularRampTopSpacerHeightType = value; }
	virtual void SetCircularRampTopSpacerUserHeight(long value) { m_nCircularRampTopSpacerUserHeight = value; }
	virtual void SetCircularRampTopSpacerHookLenAType(long value) { m_nCircularRampTopSpacerHookLenAType = value; }
	virtual void SetCircularRampTopSpacerMainBarHookLenA(long value) { m_nCircularRampTopSpacerMainBarHookLenA = value; }
	virtual void SetCircularRampTopSpacerUserHookLenA(long value) { m_nCircularRampTopSpacerUserHookLenA = value; }
	virtual void SetCircularRampTopSpacerHookLenBType(long value) { m_nCircularRampTopSpacerHookLenBType = value; }
	virtual void SetCircularRampTopSpacerMainBarHookLenB(long value) { m_nCircularRampTopSpacerMainBarHookLenB = value; }
	virtual void SetCircularRampTopSpacerUserHookLenB(long value) { m_nCircularRampTopSpacerUserHookLenB = value; }
	virtual void SetCircularRampTopSpacerWidth(long value) { m_nCircularRampTopSpacerWidth = value; }

	virtual void SetCircularRampBotSpacerInfoSet(BOOL value) { m_bCircularRampBotSpacerInfoSet = value; }
	virtual void SetCircularRampBotSpacerSet(BOOL value) { m_bCircularRampBotSpacerSet = value; }
	virtual void SetCircularRampBotSpacerDiaType(long value) { m_nCircularRampBotSpacerDiaType = value; }
	virtual void SetCircularRampBotSpacerDiaIndex(long value) { m_nCircularRampBotSpacerDiaIndex = value; }
	virtual void SetCircularRampBotSpacerSpacingType(long value) { m_nCircularRampBotSpacerSpacingType = value; }
	virtual void SetCircularRampBotSpacerSlabSpacing(long value) { m_nCircularRampBotSpacerSlabSpacing = value; }
	virtual void SetCircularRampBotSpacerUserSpacing1(long value) { m_nCircularRampBotSpacerUserSpacing1 = value; }
	virtual void SetCircularRampBotSpacerUserSpacing2(long value) { m_nCircularRampBotSpacerUserSpacing2 = value; }
	virtual void SetCircularRampBotSpacerHeightType(long value) { m_nCircularRampBotSpacerHeightType = value; }
	virtual void SetCircularRampBotSpacerUserHeight(long value) { m_nCircularRampBotSpacerUserHeight = value; }
	virtual void SetCircularRampBotSpacerHookLenAType(long value) { m_nCircularRampBotSpacerHookLenAType = value; }
	virtual void SetCircularRampBotSpacerMainBarHookLenA(long value) { m_nCircularRampBotSpacerMainBarHookLenA = value; }
	virtual void SetCircularRampBotSpacerUserHookLenA(long value) { m_nCircularRampBotSpacerUserHookLenA = value; }
	virtual void SetCircularRampBotSpacerHookLenBType(long value) { m_nCircularRampBotSpacerHookLenBType = value; }
	virtual void SetCircularRampBotSpacerMainBarHookLenB(long value) { m_nCircularRampBotSpacerMainBarHookLenB = value; }
	virtual void SetCircularRampBotSpacerUserHookLenB(long value) { m_nCircularRampBotSpacerUserHookLenB = value; }
	virtual void SetCircularRampBotSpacerWidth(long value) { m_nCircularRampBotSpacerWidth = value; }

	// PC Slab
	virtual BOOL GetPCSlabLongMainBarInfoSet() { return m_bPCSlabLongMainBarInfoSet; }
	virtual long GetPCSlabLongMaxDualHookLength() { return m_nPCSlabLongMaxDualHookLength; }
	virtual long GetPCSlabLongSpliceWay() { return m_nPCSlabLongSpliceWay; }
	virtual long GetPCSlabLongSpliceType() { return m_nPCSlabLongSpliceType; }
	virtual BOOL GetPCSlabLongAlterSpliceSet() { return m_bPCSlabLongAlterSpliceSet; }
	virtual long GetPCSlabLongAlterSpliceValue1() { return m_nPCSlabLongAlterSpliceValue1; }

	virtual BOOL GetPCSlabShortMainBarInfoSet() { return m_bPCSlabShortMainBarInfoSet; }
	virtual long GetPCSlabShortMaxDualHookLength() { return m_nPCSlabShortMaxDualHookLength; }
	virtual long GetPCSlabShortSpliceWay() { return m_nPCSlabShortSpliceWay; }
	virtual long GetPCSlabShortSpliceType() { return m_nPCSlabShortSpliceType; }
	virtual BOOL GetPCSlabShortAlterSpliceSet() { return m_bPCSlabShortAlterSpliceSet; }
	virtual long GetPCSlabShortAlterSpliceValue1() { return m_nPCSlabShortAlterSpliceValue1; }

	virtual BOOL GetPCSlabStructuredEndInfoSet() { return m_bPCSlabStructuredEndInfoSet; }
	virtual long GetPCSlabTopStructuredEndHookType() { return m_nPCSlabTopStructuredEndHookType; }
	virtual long GetPCSlabBotStructuredEndHookType() { return m_nPCSlabBotStructuredEndHookType; }

	virtual BOOL GetPCSlabDeckInfoSet() { return m_bPCSlabDeckInfoSet; }
	virtual long GetPCSlabDeckDifDiaConnectBarType() { return m_nPCSlabDeckDifDiaConnectBarType; }
	virtual long GetPCSlabDeckAnchorType() { return m_nPCSlabDeckAnchorType; }
	virtual long GetPCSlabDeckTopEndRatioType() { return m_nPCSlabDeckTopEndRatioType; }
	virtual long GetPCSlabDeckBotEndRatioType() { return m_nPCSlabDeckBotEndRatioType; }

	virtual BOOL GetPCSlabUnStructuredEndInfoSet() { return m_bPCSlabUnStructuredEndInfoSet; }
	virtual long GetPCSlabTopUnStructuredEndHookType() { return m_nPCSlabTopUnStructuredEndHookType; }
	virtual long GetPCSlabBotUnStructuredEndHookType() { return m_nPCSlabBotUnStructuredEndHookType; }
	virtual long GetPCSlabIsoFootUnStructuredDevType() { return m_nPCSlabIsoFootUnStructuredDevType; }

	virtual BOOL GetPCSlabLevelDifInfoSet() { return m_bPCSlabLevelDifInfoSet; }
	virtual long GetPCSlabLevelDifPlaceShape() { return m_nPCSlabLevelDifPlaceShape; }
	virtual long GetPCSlabLevelDifHaunchPlaceType() { return m_nPCSlabLevelDifHaunchPlaceType; }

	virtual BOOL GetPCSlabOpeningInfoSet() { return m_bPCSlabOpeningInfoSet; }
	virtual long GetPCSlabOpeningTopHookType() { return m_nPCSlabOpeningTopHookType; }
	virtual long GetPCSlabOpeningBotHookType() { return m_nPCSlabOpeningBotHookType; }

	virtual BOOL GetPCSlabRebarAreaInfoSet() { return m_bPCSlabRebarAreaInfoSet; }
	virtual long GetPCSlabRebarTolDist() { return m_nPCSlabRebarTolDist; }
	virtual long GetPCSlabRebarTolLenDif() { return m_nPCSlabRebarTolLenDif; }

	virtual BOOL GetPCSlabTopSpacerInfoSet() { return m_bPCSlabTopSpacerInfoSet; }
	virtual BOOL GetPCSlabTopSpacerSet() { return m_bPCSlabTopSpacerSet; }
	virtual long GetPCSlabTopSpacerDiaType() { return m_nPCSlabTopSpacerDiaType; }
	virtual long GetPCSlabTopSpacerDiaIndex() { return m_nPCSlabTopSpacerDiaIndex; }
	virtual long GetPCSlabTopSpacerSpacingType() { return m_nPCSlabTopSpacerSpacingType; }
	virtual long GetPCSlabTopSpacerSlabSpacing() { return m_nPCSlabTopSpacerSlabSpacing; }
	virtual long GetPCSlabTopSpacerUserSpacing1() { return m_nPCSlabTopSpacerUserSpacing1; }
	virtual long GetPCSlabTopSpacerUserSpacing2() { return m_nPCSlabTopSpacerUserSpacing2; }
	virtual long GetPCSlabTopSpacerHeightType() { return m_nPCSlabTopSpacerHeightType; }
	virtual long GetPCSlabTopSpacerUserHeight() { return m_nPCSlabTopSpacerUserHeight; }
	virtual long GetPCSlabTopSpacerHookLenAType() { return m_nPCSlabTopSpacerHookLenAType; }
	virtual long GetPCSlabTopSpacerMainBarHookLenA() { return m_nPCSlabTopSpacerMainBarHookLenA; }
	virtual long GetPCSlabTopSpacerUserHookLenA() { return m_nPCSlabTopSpacerUserHookLenA; }
	virtual long GetPCSlabTopSpacerHookLenBType() { return m_nPCSlabTopSpacerHookLenBType; }
	virtual long GetPCSlabTopSpacerMainBarHookLenB() { return m_nPCSlabTopSpacerMainBarHookLenB; }
	virtual long GetPCSlabTopSpacerUserHookLenB() { return m_nPCSlabTopSpacerUserHookLenB; }
	virtual long GetPCSlabTopSpacerWidth() { return m_nPCSlabTopSpacerWidth; }

	virtual BOOL GetPCSlabBotSpacerInfoSet() { return m_bPCSlabBotSpacerInfoSet; }
	virtual BOOL GetPCSlabBotSpacerSet() { return m_bPCSlabBotSpacerSet; }
	virtual long GetPCSlabBotSpacerDiaType() { return m_nPCSlabBotSpacerDiaType; }
	virtual long GetPCSlabBotSpacerDiaIndex() { return m_nPCSlabBotSpacerDiaIndex; }
	virtual long GetPCSlabBotSpacerSpacingType() { return m_nPCSlabBotSpacerSpacingType; }
	virtual long GetPCSlabBotSpacerSlabSpacing() { return m_nPCSlabBotSpacerSlabSpacing; }
	virtual long GetPCSlabBotSpacerUserSpacing1() { return m_nPCSlabBotSpacerUserSpacing1; }
	virtual long GetPCSlabBotSpacerUserSpacing2() { return m_nPCSlabBotSpacerUserSpacing2; }
	virtual long GetPCSlabBotSpacerHeightType() { return m_nPCSlabBotSpacerHeightType; }
	virtual long GetPCSlabBotSpacerUserHeight() { return m_nPCSlabBotSpacerUserHeight; }
	virtual long GetPCSlabBotSpacerHookLenAType() { return m_nPCSlabBotSpacerHookLenAType; }
	virtual long GetPCSlabBotSpacerMainBarHookLenA() { return m_nPCSlabBotSpacerMainBarHookLenA; }
	virtual long GetPCSlabBotSpacerUserHookLenA() { return m_nPCSlabBotSpacerUserHookLenA; }
	virtual long GetPCSlabBotSpacerHookLenBType() { return m_nPCSlabBotSpacerHookLenBType; }
	virtual long GetPCSlabBotSpacerMainBarHookLenB() { return m_nPCSlabBotSpacerMainBarHookLenB; }
	virtual long GetPCSlabBotSpacerUserHookLenB() { return m_nPCSlabBotSpacerUserHookLenB; }
	virtual long GetPCSlabBotSpacerWidth() { return m_nPCSlabBotSpacerWidth; }

	virtual void SetPCSlabLongMainBarInfoSet(BOOL value) { m_bPCSlabLongMainBarInfoSet = value; }
	virtual void SetPCSlabLongMaxDualHookLength(long value) { m_nPCSlabLongMaxDualHookLength = value; }
	virtual void SetPCSlabLongSpliceWay(long value) { m_nPCSlabLongSpliceWay = value; }
	virtual void SetPCSlabLongSpliceType(long value) { m_nPCSlabLongSpliceType = value; }
	virtual void SetPCSlabLongAlterSpliceSet(BOOL value) { m_bPCSlabLongAlterSpliceSet = value; }
	virtual void SetPCSlabLongAlterSpliceValue1(long value) { m_nPCSlabLongAlterSpliceValue1 = value; }

	virtual void SetPCSlabShortMainBarInfoSet(BOOL value) { m_bPCSlabShortMainBarInfoSet = value; }
	virtual void SetPCSlabShortMaxDualHookLength(long value) { m_nPCSlabShortMaxDualHookLength = value; }
	virtual void SetPCSlabShortSpliceWay(long value) { m_nPCSlabShortSpliceWay = value; }
	virtual void SetPCSlabShortSpliceType(long value) { m_nPCSlabShortSpliceType = value; }
	virtual void SetPCSlabShortAlterSpliceSet(BOOL value) { m_bPCSlabShortAlterSpliceSet = value; }
	virtual void SetPCSlabShortAlterSpliceValue1(long value) { m_nPCSlabShortAlterSpliceValue1 = value; }

	virtual void SetPCSlabStructuredEndInfoSet(BOOL value) { m_bPCSlabStructuredEndInfoSet = value; }
	virtual void SetPCSlabTopStructuredEndHookType(long value) { m_nPCSlabTopStructuredEndHookType = value; }
	virtual void SetPCSlabBotStructuredEndHookType(long value) { m_nPCSlabBotStructuredEndHookType = value; }

	virtual void SetPCSlabDeckInfoSet(BOOL value) { m_bPCSlabDeckInfoSet = value; }
	virtual void SetPCSlabDeckDifDiaConnectBarType(long value) { m_nPCSlabDeckDifDiaConnectBarType = value; }
	virtual void SetPCSlabDeckAnchorType(long value) { m_nPCSlabDeckAnchorType = value; }
	virtual void SetPCSlabDeckTopEndRatioType(long value) { m_nPCSlabDeckTopEndRatioType = value; }
	virtual void SetPCSlabDeckBotEndRatioType(long value) { m_nPCSlabDeckBotEndRatioType = value; }

	virtual void SetPCSlabUnStructuredEndInfoSet(BOOL value) { m_bPCSlabUnStructuredEndInfoSet = value; }
	virtual void SetPCSlabTopUnStructuredEndHookType(long value) { m_nPCSlabTopUnStructuredEndHookType = value; }
	virtual void SetPCSlabBotUnStructuredEndHookType(long value) { m_nPCSlabBotUnStructuredEndHookType = value; }
	virtual void SetPCSlabIsoFootUnStructuredDevType(long value) { m_nPCSlabIsoFootUnStructuredDevType = value; }

	virtual void SetPCSlabLevelDifInfoSet(BOOL value) { m_bPCSlabLevelDifInfoSet = value; }
	virtual void SetPCSlabLevelDifPlaceShape(long value) { m_nPCSlabLevelDifPlaceShape = value; }
	virtual void SetPCSlabLevelDifHaunchPlaceType(long value) { m_nPCSlabLevelDifHaunchPlaceType = value; }

	virtual void SetPCSlabOpeningInfoSet(BOOL value) { m_bPCSlabOpeningInfoSet = value; }
	virtual void SetPCSlabOpeningTopHookType(long value) { m_nPCSlabOpeningTopHookType = value; }
	virtual void SetPCSlabOpeningBotHookType(long value) { m_nPCSlabOpeningBotHookType = value; }

	virtual void SetPCSlabRebarAreaInfoSet(BOOL value) { m_bPCSlabRebarAreaInfoSet = value; }
	virtual void SetPCSlabRebarTolDist(long value) { m_nPCSlabRebarTolDist = value; }
	virtual void SetPCSlabRebarTolLenDif(long value) { m_nPCSlabRebarTolLenDif = value; }

	virtual void SetPCSlabTopSpacerInfoSet(BOOL value) { m_bPCSlabTopSpacerInfoSet = value; }
	virtual void SetPCSlabTopSpacerSet(BOOL value) { m_bPCSlabTopSpacerSet = value; }
	virtual void SetPCSlabTopSpacerDiaType(long value) { m_nPCSlabTopSpacerDiaType = value; }
	virtual void SetPCSlabTopSpacerDiaIndex(long value) { m_nPCSlabTopSpacerDiaIndex = value; }
	virtual void SetPCSlabTopSpacerSpacingType(long value) { m_nPCSlabTopSpacerSpacingType = value; }
	virtual void SetPCSlabTopSpacerSlabSpacing(long value) { m_nPCSlabTopSpacerSlabSpacing = value; }
	virtual void SetPCSlabTopSpacerUserSpacing1(long value) { m_nPCSlabTopSpacerUserSpacing1 = value; }
	virtual void SetPCSlabTopSpacerUserSpacing2(long value) { m_nPCSlabTopSpacerUserSpacing2 = value; }
	virtual void SetPCSlabTopSpacerHeightType(long value) { m_nPCSlabTopSpacerHeightType = value; }
	virtual void SetPCSlabTopSpacerUserHeight(long value) { m_nPCSlabTopSpacerUserHeight = value; }
	virtual void SetPCSlabTopSpacerHookLenAType(long value) { m_nPCSlabTopSpacerHookLenAType = value; }
	virtual void SetPCSlabTopSpacerMainBarHookLenA(long value) { m_nPCSlabTopSpacerMainBarHookLenA = value; }
	virtual void SetPCSlabTopSpacerUserHookLenA(long value) { m_nPCSlabTopSpacerUserHookLenA = value; }
	virtual void SetPCSlabTopSpacerHookLenBType(long value) { m_nPCSlabTopSpacerHookLenBType = value; }
	virtual void SetPCSlabTopSpacerMainBarHookLenB(long value) { m_nPCSlabTopSpacerMainBarHookLenB = value; }
	virtual void SetPCSlabTopSpacerUserHookLenB(long value) { m_nPCSlabTopSpacerUserHookLenB = value; }
	virtual void SetPCSlabTopSpacerWidth(long value) { m_nPCSlabTopSpacerWidth = value; }

	virtual void SetPCSlabBotSpacerInfoSet(BOOL value) { m_bPCSlabBotSpacerInfoSet = value; }
	virtual void SetPCSlabBotSpacerSet(BOOL value) { m_bPCSlabBotSpacerSet = value; }
	virtual void SetPCSlabBotSpacerDiaType(long value) { m_nPCSlabBotSpacerDiaType = value; }
	virtual void SetPCSlabBotSpacerDiaIndex(long value) { m_nPCSlabBotSpacerDiaIndex = value; }
	virtual void SetPCSlabBotSpacerSpacingType(long value) { m_nPCSlabBotSpacerSpacingType = value; }
	virtual void SetPCSlabBotSpacerSlabSpacing(long value) { m_nPCSlabBotSpacerSlabSpacing = value; }
	virtual void SetPCSlabBotSpacerUserSpacing1(long value) { m_nPCSlabBotSpacerUserSpacing1 = value; }
	virtual void SetPCSlabBotSpacerUserSpacing2(long value) { m_nPCSlabBotSpacerUserSpacing2 = value; }
	virtual void SetPCSlabBotSpacerHeightType(long value) { m_nPCSlabBotSpacerHeightType = value; }
	virtual void SetPCSlabBotSpacerUserHeight(long value) { m_nPCSlabBotSpacerUserHeight = value; }
	virtual void SetPCSlabBotSpacerHookLenAType(long value) { m_nPCSlabBotSpacerHookLenAType = value; }
	virtual void SetPCSlabBotSpacerMainBarHookLenA(long value) { m_nPCSlabBotSpacerMainBarHookLenA = value; }
	virtual void SetPCSlabBotSpacerUserHookLenA(long value) { m_nPCSlabBotSpacerUserHookLenA = value; }
	virtual void SetPCSlabBotSpacerHookLenBType(long value) { m_nPCSlabBotSpacerHookLenBType = value; }
	virtual void SetPCSlabBotSpacerMainBarHookLenB(long value) { m_nPCSlabBotSpacerMainBarHookLenB = value; }
	virtual void SetPCSlabBotSpacerUserHookLenB(long value) { m_nPCSlabBotSpacerUserHookLenB = value; }
	virtual void SetPCSlabBotSpacerWidth(long value) { m_nPCSlabBotSpacerWidth = value; }

	// 철근 일체형 데크 슬래브
	virtual BOOL GetEmbeddedDeckLongMainBarInfoSet() { return m_bEmbeddedDeckLongMainBarInfoSet; }
	virtual long GetEmbeddedDeckLongMaxDualHookLength() { return m_nEmbeddedDeckLongMaxDualHookLength; }
	virtual long GetEmbeddedDeckLongSpliceWay() { return m_nEmbeddedDeckLongSpliceWay; }
	virtual long GetEmbeddedDeckLongSpliceType() { return m_nEmbeddedDeckLongSpliceType; }
	virtual BOOL GetEmbeddedDeckLongAlterSpliceSet() { return m_bEmbeddedDeckLongAlterSpliceSet; }
	virtual long GetEmbeddedDeckLongAlterSpliceValue1() { return m_nEmbeddedDeckLongAlterSpliceValue1; }

	virtual BOOL GetEmbeddedDeckShortMainBarInfoSet() { return m_bEmbeddedDeckShortMainBarInfoSet; }
	virtual long GetEmbeddedDeckShortMaxDualHookLength() { return m_nEmbeddedDeckShortMaxDualHookLength; }
	virtual long GetEmbeddedDeckShortSpliceWay() { return m_nEmbeddedDeckShortSpliceWay; }
	virtual long GetEmbeddedDeckShortSpliceType() { return m_nEmbeddedDeckShortSpliceType; }
	virtual BOOL GetEmbeddedDeckShortAlterSpliceSet() { return m_bEmbeddedDeckShortAlterSpliceSet; }
	virtual long GetEmbeddedDeckShortAlterSpliceValue1() { return m_nEmbeddedDeckShortAlterSpliceValue1; }

	virtual BOOL GetEmbeddedDeckStructuredEndInfoSet() { return m_bEmbeddedDeckStructuredEndInfoSet; }
	virtual long GetEmbeddedDeckTopStructuredEndHookType() { return m_nEmbeddedDeckTopStructuredEndHookType; }
	virtual long GetEmbeddedDeckBotStructuredEndHookType() { return m_nEmbeddedDeckBotStructuredEndHookType; }

	virtual BOOL GetEmbeddedDeckInfoSet() { return m_bEmbeddedDeckInfoSet; }
	virtual long GetEmbeddedDeckDifDiaConnectBarType() { return m_nEmbeddedDeckDifDiaConnectBarType; }
	virtual long GetEmbeddedDeckAnchorType() { return m_nEmbeddedDeckAnchorType; }
	virtual long GetEmbeddedDeckTopEndRatioType() { return m_nEmbeddedDeckTopEndRatioType; }
	virtual long GetEmbeddedDeckBotEndRatioType() { return m_nEmbeddedDeckBotEndRatioType; }

	virtual BOOL GetEmbeddedDeckUnStructuredEndInfoSet() { return m_bEmbeddedDeckUnStructuredEndInfoSet; }
	virtual long GetEmbeddedDeckTopUnStructuredEndHookType() { return m_nEmbeddedDeckTopUnStructuredEndHookType; }
	virtual long GetEmbeddedDeckBotUnStructuredEndHookType() { return m_nEmbeddedDeckBotUnStructuredEndHookType; }
	virtual long GetEmbeddedDeckIsoFootUnStructuredDevType() { return m_nEmbeddedDeckIsoFootUnStructuredDevType; }

	virtual BOOL GetEmbeddedDeckLevelDifInfoSet() { return m_bEmbeddedDeckLevelDifInfoSet; }
	virtual long GetEmbeddedDeckLevelDifPlaceShape() { return m_nEmbeddedDeckLevelDifPlaceShape; }
	virtual long GetEmbeddedDeckLevelDifHaunchPlaceType() { return m_nEmbeddedDeckLevelDifHaunchPlaceType; }

	virtual BOOL GetEmbeddedDeckOpeningInfoSet() { return m_bEmbeddedDeckOpeningInfoSet; }
	virtual long GetEmbeddedDeckOpeningTopHookType() { return m_nEmbeddedDeckOpeningTopHookType; }
	virtual long GetEmbeddedDeckOpeningBotHookType() { return m_nEmbeddedDeckOpeningBotHookType; }

	virtual BOOL GetEmbeddedDeckRebarAreaInfoSet() { return m_bEmbeddedDeckRebarAreaInfoSet; }
	virtual long GetEmbeddedDeckRebarTolDist() { return m_nEmbeddedDeckRebarTolDist; }
	virtual long GetEmbeddedDeckRebarTolLenDif() { return m_nEmbeddedDeckRebarTolLenDif; }

	virtual void SetEmbeddedDeckLongMainBarInfoSet(BOOL value) { m_bEmbeddedDeckLongMainBarInfoSet = value; }
	virtual void SetEmbeddedDeckLongMaxDualHookLength(long value) { m_nEmbeddedDeckLongMaxDualHookLength = value; }
	virtual void SetEmbeddedDeckLongSpliceWay(long value) { m_nEmbeddedDeckLongSpliceWay = value; }
	virtual void SetEmbeddedDeckLongSpliceType(long value) { m_nEmbeddedDeckLongSpliceType = value; }
	virtual void SetEmbeddedDeckLongAlterSpliceSet(BOOL value) { m_bEmbeddedDeckLongAlterSpliceSet = value; }
	virtual void SetEmbeddedDeckLongAlterSpliceValue1(long value) { m_nEmbeddedDeckLongAlterSpliceValue1 = value; }

	virtual void SetEmbeddedDeckShortMainBarInfoSet(BOOL value) { m_bEmbeddedDeckShortMainBarInfoSet = value; }
	virtual void SetEmbeddedDeckShortMaxDualHookLength(long value) { m_nEmbeddedDeckShortMaxDualHookLength = value; }
	virtual void SetEmbeddedDeckShortSpliceWay(long value) { m_nEmbeddedDeckShortSpliceWay = value; }
	virtual void SetEmbeddedDeckShortSpliceType(long value) { m_nEmbeddedDeckShortSpliceType = value; }
	virtual void SetEmbeddedDeckShortAlterSpliceSet(BOOL value) { m_bEmbeddedDeckShortAlterSpliceSet = value; }
	virtual void SetEmbeddedDeckShortAlterSpliceValue1(long value) { m_nEmbeddedDeckShortAlterSpliceValue1 = value; }

	virtual void SetEmbeddedDeckStructuredEndInfoSet(BOOL value) { m_bEmbeddedDeckStructuredEndInfoSet = value; }
	virtual void SetEmbeddedDeckTopStructuredEndHookType(long value) { m_nEmbeddedDeckTopStructuredEndHookType = value; }
	virtual void SetEmbeddedDeckBotStructuredEndHookType(long value) { m_nEmbeddedDeckBotStructuredEndHookType = value; }

	virtual void SetEmbeddedDeckInfoSet(BOOL value) { m_bEmbeddedDeckInfoSet = value; }
	virtual void SetEmbeddedDeckDifDiaConnectBarType(long value) { m_nEmbeddedDeckDifDiaConnectBarType = value; }
	virtual void SetEmbeddedDeckAnchorType(long value) { m_nEmbeddedDeckAnchorType = value; }
	virtual void SetEmbeddedDeckTopEndRatioType(long value) { m_nEmbeddedDeckTopEndRatioType = value; }
	virtual void SetEmbeddedDeckBotEndRatioType(long value) { m_nEmbeddedDeckBotEndRatioType = value; }

	virtual void SetEmbeddedDeckUnStructuredEndInfoSet(BOOL value) { m_bEmbeddedDeckUnStructuredEndInfoSet = value; }
	virtual void SetEmbeddedDeckTopUnStructuredEndHookType(long value) { m_nEmbeddedDeckTopUnStructuredEndHookType = value; }
	virtual void SetEmbeddedDeckBotUnStructuredEndHookType(long value) { m_nEmbeddedDeckBotUnStructuredEndHookType = value; }
	virtual void SetEmbeddedDeckIsoFootUnStructuredDevType(long value) { m_nEmbeddedDeckIsoFootUnStructuredDevType = value; }

	virtual void SetEmbeddedDeckLevelDifInfoSet(BOOL value) { m_bEmbeddedDeckLevelDifInfoSet = value; }
	virtual void SetEmbeddedDeckLevelDifPlaceShape(long value) { m_nEmbeddedDeckLevelDifPlaceShape = value; }
	virtual void SetEmbeddedDeckLevelDifHaunchPlaceType(long value) { m_nEmbeddedDeckLevelDifHaunchPlaceType = value; }

	virtual void SetEmbeddedDeckOpeningInfoSet(BOOL value) { m_bEmbeddedDeckOpeningInfoSet = value; }
	virtual void SetEmbeddedDeckOpeningTopHookType(long value) { m_nEmbeddedDeckOpeningTopHookType = value; }
	virtual void SetEmbeddedDeckOpeningBotHookType(long value) { m_nEmbeddedDeckOpeningBotHookType = value; }

	virtual void SetEmbeddedDeckRebarAreaInfoSet(BOOL value) { m_bEmbeddedDeckRebarAreaInfoSet = value; }
	virtual void SetEmbeddedDeckRebarTolDist(long value) { m_nEmbeddedDeckRebarTolDist = value; }
	virtual void SetEmbeddedDeckRebarTolLenDif(long value) { m_nEmbeddedDeckRebarTolLenDif = value; }

	// 일반 데크 슬래브
	virtual BOOL GetGeneralDeckLongMainBarInfoSet() { return m_bGeneralDeckLongMainBarInfoSet; }
	virtual long GetGeneralDeckLongMaxDualHookLength() { return m_nGeneralDeckLongMaxDualHookLength; }
	virtual long GetGeneralDeckLongSpliceWay() { return m_nGeneralDeckLongSpliceWay; }
	virtual long GetGeneralDeckLongSpliceType() { return m_nGeneralDeckLongSpliceType; }
	virtual BOOL GetGeneralDeckLongAlterSpliceSet() { return m_bGeneralDeckLongAlterSpliceSet; }
	virtual long GetGeneralDeckLongAlterSpliceValue1() { return m_nGeneralDeckLongAlterSpliceValue1; }

	virtual BOOL GetGeneralDeckShortMainBarInfoSet() { return m_bGeneralDeckShortMainBarInfoSet; }
	virtual long GetGeneralDeckShortMaxDualHookLength() { return m_nGeneralDeckShortMaxDualHookLength; }
	virtual long GetGeneralDeckShortSpliceWay() { return m_nGeneralDeckShortSpliceWay; }
	virtual long GetGeneralDeckShortSpliceType() { return m_nGeneralDeckShortSpliceType; }
	virtual BOOL GetGeneralDeckShortAlterSpliceSet() { return m_bGeneralDeckShortAlterSpliceSet; }
	virtual long GetGeneralDeckShortAlterSpliceValue1() { return m_nGeneralDeckShortAlterSpliceValue1; }

	virtual BOOL GetGeneralDeckStructuredEndInfoSet() { return m_bGeneralDeckStructuredEndInfoSet; }
	virtual long GetGeneralDeckTopStructuredEndHookType() { return m_nGeneralDeckTopStructuredEndHookType; }
	virtual long GetGeneralDeckBotStructuredEndHookType() { return m_nGeneralDeckBotStructuredEndHookType; }

	virtual BOOL GetGeneralDeckUnStructuredEndInfoSet() { return m_bGeneralDeckUnStructuredEndInfoSet; }
	virtual long GetGeneralDeckTopUnStructuredEndHookType() { return m_nGeneralDeckTopUnStructuredEndHookType; }
	virtual long GetGeneralDeckBotUnStructuredEndHookType() { return m_nGeneralDeckBotUnStructuredEndHookType; }
	virtual long GetGeneralDeckIsoFootUnStructuredDevType() { return m_nGeneralDeckIsoFootUnStructuredDevType; }

	virtual BOOL GetGeneralDeckLevelDifInfoSet() { return m_bGeneralDeckLevelDifInfoSet; }
	virtual long GetGeneralDeckLevelDifPlaceShape() { return m_nGeneralDeckLevelDifPlaceShape; }
	virtual long GetGeneralDeckLevelDifHaunchPlaceType() { return m_nGeneralDeckLevelDifHaunchPlaceType; }

	virtual BOOL GetGeneralDeckOpeningInfoSet() { return m_bGeneralDeckOpeningInfoSet; }
	virtual long GetGeneralDeckOpeningTopHookType() { return m_nGeneralDeckOpeningTopHookType; }
	virtual long GetGeneralDeckOpeningBotHookType() { return m_nGeneralDeckOpeningBotHookType; }

	virtual BOOL GetGeneralDeckRebarAreaInfoSet() { return m_bGeneralDeckRebarAreaInfoSet; }
	virtual long GetGeneralDeckRebarTolDist() { return m_nGeneralDeckRebarTolDist; }
	virtual long GetGeneralDeckRebarTolLenDif() { return m_nGeneralDeckRebarTolLenDif; }

	virtual void SetGeneralDeckLongMainBarInfoSet(BOOL value) { m_bGeneralDeckLongMainBarInfoSet = value; }
	virtual void SetGeneralDeckLongMaxDualHookLength(long value) { m_nGeneralDeckLongMaxDualHookLength = value; }
	virtual void SetGeneralDeckLongSpliceWay(long value) { m_nGeneralDeckLongSpliceWay = value; }
	virtual void SetGeneralDeckLongSpliceType(long value) { m_nGeneralDeckLongSpliceType = value; }
	virtual void SetGeneralDeckLongAlterSpliceSet(BOOL value) { m_bGeneralDeckLongAlterSpliceSet = value; }
	virtual void SetGeneralDeckLongAlterSpliceValue1(long value) { m_nGeneralDeckLongAlterSpliceValue1 = value; }

	virtual void SetGeneralDeckShortMainBarInfoSet(BOOL value) { m_bGeneralDeckShortMainBarInfoSet = value; }
	virtual void SetGeneralDeckShortMaxDualHookLength(long value) { m_nGeneralDeckShortMaxDualHookLength = value; }
	virtual void SetGeneralDeckShortSpliceWay(long value) { m_nGeneralDeckShortSpliceWay = value; }
	virtual void SetGeneralDeckShortSpliceType(long value) { m_nGeneralDeckShortSpliceType = value; }
	virtual void SetGeneralDeckShortAlterSpliceSet(BOOL value) { m_bGeneralDeckShortAlterSpliceSet = value; }
	virtual void SetGeneralDeckShortAlterSpliceValue1(long value) { m_nGeneralDeckShortAlterSpliceValue1 = value; }

	virtual void SetGeneralDeckStructuredEndInfoSet(BOOL value) { m_bGeneralDeckStructuredEndInfoSet = value; }
	virtual void SetGeneralDeckTopStructuredEndHookType(long value) { m_nGeneralDeckTopStructuredEndHookType = value; }
	virtual void SetGeneralDeckBotStructuredEndHookType(long value) { m_nGeneralDeckBotStructuredEndHookType = value; }

	virtual void SetGeneralDeckUnStructuredEndInfoSet(BOOL value) { m_bGeneralDeckUnStructuredEndInfoSet = value; }
	virtual void SetGeneralDeckTopUnStructuredEndHookType(long value) { m_nGeneralDeckTopUnStructuredEndHookType = value; }
	virtual void SetGeneralDeckBotUnStructuredEndHookType(long value) { m_nGeneralDeckBotUnStructuredEndHookType = value; }
	virtual void SetGeneralDeckIsoFootUnStructuredDevType(long value) { m_nGeneralDeckIsoFootUnStructuredDevType = value; }

	virtual void SetGeneralDeckLevelDifInfoSet(BOOL value) { m_bGeneralDeckLevelDifInfoSet = value; }
	virtual void SetGeneralDeckLevelDifPlaceShape(long value) { m_nGeneralDeckLevelDifPlaceShape = value; }
	virtual void SetGeneralDeckLevelDifHaunchPlaceType(long value) { m_nGeneralDeckLevelDifHaunchPlaceType = value; }

	virtual void SetGeneralDeckOpeningInfoSet(BOOL value) { m_bGeneralDeckOpeningInfoSet = value; }
	virtual void SetGeneralDeckOpeningTopHookType(long value) { m_nGeneralDeckOpeningTopHookType = value; }
	virtual void SetGeneralDeckOpeningBotHookType(long value) { m_nGeneralDeckOpeningBotHookType = value; }

	virtual void SetGeneralDeckRebarAreaInfoSet(BOOL value) { m_bGeneralDeckRebarAreaInfoSet = value; }
	virtual void SetGeneralDeckRebarTolDist(long value) { m_nGeneralDeckRebarTolDist = value; }
	virtual void SetGeneralDeckRebarTolLenDif(long value) { m_nGeneralDeckRebarTolLenDif = value; }

private:
	// MAT Slab
	BOOL m_bMatLongMainBarInfoSet;
	long m_nMatLongMaxDualHookLength;	//최대 양후크길이
	long m_nMatLongSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nMatLongSpliceType;	//이음타입 0:인장이음 1:압축이음
	BOOL m_bMatLongAlterSpliceSet;		//엇이음 설정
	long m_nMatLongAlterSpliceValue1;	//엇이음 설정 H1 Value
	
	BOOL m_bMatShortMainBarInfoSet;
	long m_nMatShortMaxDualHookLength;	//최대 양후크길이
	long m_nMatShortSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nMatShortSpliceType;	//이음타입 0:인장이음 1:압축이음
	BOOL m_bMatShortAlterSpliceSet;		//엇이음 설정
	long m_nMatShortAlterSpliceValue1;	//엇이음 설정 H1 Value

	BOOL m_bMatEndInfoSet;
	long m_nMatTopEndHookType;		//상부근 단부 0:정착 1:후크 
	long m_nMatBotEndHookType;		//하부근 단부 0:일자 1:후크
	long m_nMatIsoFootDevType;		//독립기초 & Panel 접합부위 하부근 0:Passing 1:인장정착

	BOOL m_bMatLevelDifInfoSet;
	long m_nMatLevelDifPlaceShape;		//배근 형상
	long m_nMatLevelDifHaunchPlaceType;	//헌치 배근타입

	BOOL m_bMatRebarAreaInfoSet;
	long m_nMatRebarTolDist;		//앞뒤 철근의 길이차
	long m_nMatRebarTolLenDif;		//가장 긴 철근과 짧은 철근이 길이차가 m_nMatRebarTolLenDif이상이 되면 barset을 나눈다.

	BOOL m_bMatTopSpacerInfoSet;
	BOOL m_bMatTopSpacerSet;			//상부 우마철근 설정
	long m_nMatTopSpacerDiaType;		//직경 0:해당기초직경 1:사용자직경
	long m_nMatTopSpacerDiaIndex;		//직경 : 사용자 직경 입력
	long m_nMatTopSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nMatTopSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nMatTopSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nMatTopSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nMatTopSpacerHeightType;		//높이
	long m_nMatTopSpacerUserHeight;		//높이 : 사용자 입력
	long m_nMatTopSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nMatTopSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nMatTopSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nMatTopSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nMatTopSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nMatTopSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nMatTopSpacerWidth;			//폭

	BOOL m_bMatBotSpacerInfoSet;
	BOOL m_bMatBotSpacerSet;			//상부 우마철근 설정
	long m_nMatBotSpacerDiaType;		//직경 0:해당기초직경 1:사용자직경
	long m_nMatBotSpacerDiaIndex;		//직경 : 사용자 직경 입력
	long m_nMatBotSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nMatBotSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nMatBotSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nMatBotSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nMatBotSpacerHeightType;		//높이
	long m_nMatBotSpacerUserHeight;		//높이 : 사용자 입력
	long m_nMatBotSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nMatBotSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nMatBotSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nMatBotSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nMatBotSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nMatBotSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nMatBotSpacerWidth;			//폭

	// Flat Slab
	BOOL m_bFlatLongMainBarInfoSet;
	long m_nFlatLongMaxDualHookLength;	//최대 양후크길이
	long m_nFlatLongSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nFlatLongSpliceType;	//이음타입 0:인장이음 1:압축이음
	BOOL m_bFlatLongAlterSpliceSet;		//엇이음 설정
	long m_nFlatLongAlterSpliceValue1;	//엇이음 설정 H1 Value

	BOOL m_bFlatShortMainBarInfoSet;
	long m_nFlatShortMaxDualHookLength;	//최대 양후크길이
	long m_nFlatShortSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nFlatShortSpliceType;	//이음타입 0:인장이음 1:압축이음
	BOOL m_bFlatShortAlterSpliceSet;		//엇이음 설정
	long m_nFlatShortAlterSpliceValue1;	//엇이음 설정 H1 Value

	BOOL m_bFlatEndInfoSet;
	long m_nFlatTopEndHookType;		//상부근 단부 0:정착 1:후크 
	long m_nFlatBotEndHookType;		//하부근 단부 0:정착 1:후크 
	long m_nFlatIsoFootDevType;		//독립기초 & Panel 접합부위 하부근 0:Passing 1:인장정착
	long m_nFlatSpliceDevSlabType;	//이음/정착 적용	0:기초	1:슬래브

	BOOL m_bFlatLevelDifInfoSet;
	long m_nFlatLevelDifPlaceShape;		//배근 형상
	long m_nFlatLevelDifHaunchPlaceType;	//헌치 배근타입

	BOOL m_bFlatOpeningInfoSet;
	long m_nFlatOpeningTopHookType;		//Open 부위 철근 설정 상부0:일자정착 1:후크정착 
	long m_nFlatOpeningBotHookType;		//Open 부위 철근 설정 하부0:일자정착 1:후크정착

	BOOL m_bFlatRebarAreaInfoSet;
	long m_nFlatRebarTolDist;		//앞뒤 철근의 길이차
	long m_nFlatRebarTolLenDif;		//가장 긴 철근과 짧은 철근이 길이차가 m_nFlatRebarTolLenDif이상이 되면 barset을 나눈다.

	BOOL m_bFlatTopSpacerInfoSet;
	BOOL m_bFlatTopSpacerSet;			//상부 우마철근 설정
	long m_nFlatTopSpacerDiaType;		//직경 0:해당기초직경 1:사용자직경
	long m_nFlatTopSpacerDiaIndex;		//직경 : 사용자 직경 입력
	long m_nFlatTopSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nFlatTopSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nFlatTopSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nFlatTopSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nFlatTopSpacerHeightType;		//높이
	long m_nFlatTopSpacerUserHeight;		//높이 : 사용자 입력
	long m_nFlatTopSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nFlatTopSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nFlatTopSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nFlatTopSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nFlatTopSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nFlatTopSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nFlatTopSpacerWidth;			//폭

	BOOL m_bFlatBotSpacerInfoSet;
	BOOL m_bFlatBotSpacerSet;			//상부 우마철근 설정
	long m_nFlatBotSpacerDiaType;		//직경 0:해당기초직경 1:사용자직경
	long m_nFlatBotSpacerDiaIndex;		//직경 : 사용자 직경 입력
	long m_nFlatBotSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nFlatBotSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nFlatBotSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nFlatBotSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nFlatBotSpacerHeightType;		//높이
	long m_nFlatBotSpacerUserHeight;		//높이 : 사용자 입력
	long m_nFlatBotSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nFlatBotSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nFlatBotSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nFlatBotSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nFlatBotSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nFlatBotSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nFlatBotSpacerWidth;			//폭

	// 사변지지 SLAB
	BOOL m_bSlabLongMainBarInfoSet;
	long m_nSlabLongMaxDualHookLength;	//최대 양후크길이
	long m_nSlabLongSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nSlabLongSpliceType;	//이음타입 0:인장이음 1:압축이음
	BOOL m_bSlabLongAlterSpliceSet;		//엇이음 설정
	long m_nSlabLongAlterSpliceValue1;	//엇이음 설정 H1 Value

	BOOL m_bSlabShortMainBarInfoSet;
	long m_nSlabShortMaxDualHookLength;	//최대 양후크길이
	long m_nSlabShortSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nSlabShortSpliceType;	//이음타입 0:인장이음 1:압축이음
	BOOL m_bSlabShortAlterSpliceSet;		//엇이음 설정
	long m_nSlabShortAlterSpliceValue1;	//엇이음 설정 H1 Value

	BOOL m_bSlabStructuredEndInfoSet;
	long m_nSlabTopStructuredEndHookType;			//상부근 단부 0:정착 1:후크 
	long m_nSlabBotStructuredEndHookType;			//하부근 단부 0:정착 1:150

	BOOL m_bSlabTopCutBarLenInfoSet;
	long m_nSlabOneWayCutBarLenType;	//0:묻힘길이 = max(d, 12db) 1:묻힘길이 = max(d, 12db, In/16) 2:CutBar길이 = 0.3 * 경간길이(Ln)
	long m_nSlabTwoWayCutBarLenType;	//0:묻힘길이 = max(d, 12db) 1:묻힘길이 = max(d, 12db, In/16) 2:CutBar길이 = 0.3 * 경간길이(Ln)

	BOOL m_bSlabUnStructuredEndInfoSet;
	long m_nSlabTopUnStructuredEndHookType;		//상부근 단부 0:정착 1:후크 
	long m_nSlabBotUnStructuredEndHookType;		//하부근 단부 0:정착 1:후크 
	long m_nSlabIsoFootUnStructuredDevType;		//독립기초 & Panel 접합부위 하부근 0:Passing 1:인장정착

	BOOL m_bSlabLevelDifInfoSet;
	long m_nSlabLevelDifPlaceShape;		//배근 형상
	long m_nSlabLevelDifHaunchPlaceType;	//헌치 배근타입

	BOOL m_bSlabOpeningInfoSet;
	long m_nSlabOpeningTopHookType;		//Open 부위 철근 설정 상부0:일자정착 1:후크정착 
	long m_nSlabOpeningBotHookType;		//Open 부위 철근 설정 하부0:일자정착 1:후크정착

	BOOL m_bSlabRebarAreaInfoSet;
	long m_nSlabRebarTolDist;		//앞뒤 철근의 길이차
	long m_nSlabRebarTolLenDif;		//가장 긴 철근과 짧은 철근이 길이차가 m_nMatRebarTolLenDif이상이 되면 barset을 나눈다.

	BOOL m_bSlabTopSpacerInfoSet;
	BOOL m_bSlabTopSpacerSet;			//상부 우마철근 설정
	long m_nSlabTopSpacerDiaType;		//직경 0:해당기초직경 1:사용자직경
	long m_nSlabTopSpacerDiaIndex;		//직경 : 사용자 직경 입력
	long m_nSlabTopSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nSlabTopSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nSlabTopSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nSlabTopSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nSlabTopSpacerHeightType;		//높이
	long m_nSlabTopSpacerUserHeight;		//높이 : 사용자 입력
	long m_nSlabTopSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nSlabTopSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nSlabTopSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nSlabTopSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nSlabTopSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nSlabTopSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nSlabTopSpacerWidth;			//폭

	BOOL m_bSlabBotSpacerInfoSet;
	BOOL m_bSlabBotSpacerSet;			//상부 우마철근 설정
	long m_nSlabBotSpacerDiaType;		//직경 0:해당기초직경 1:사용자직경
	long m_nSlabBotSpacerDiaIndex;		//직경 : 사용자 직경 입력
	long m_nSlabBotSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nSlabBotSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nSlabBotSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nSlabBotSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nSlabBotSpacerHeightType;		//높이
	long m_nSlabBotSpacerUserHeight;		//높이 : 사용자 입력
	long m_nSlabBotSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nSlabBotSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nSlabBotSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nSlabBotSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nSlabBotSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nSlabBotSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nSlabBotSpacerWidth;			//폭

	// SOG Slab
	BOOL m_bSOGLongMainBarInfoSet;
	long m_nSOGLongMaxDualHookLength;	//최대 양후크길이
	long m_nSOGLongSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nSOGLongSpliceType;	//이음타입 0:인장이음 1:압축이음
	BOOL m_bSOGLongAlterSpliceSet;		//엇이음 설정
	long m_nSOGLongAlterSpliceValue1;	//엇이음 설정 H1 Value

	BOOL m_bSOGShortMainBarInfoSet;
	long m_nSOGShortMaxDualHookLength;	//최대 양후크길이
	long m_nSOGShortSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nSOGShortSpliceType;	//이음타입 0:인장이음 1:압축이음
	BOOL m_bSOGShortAlterSpliceSet;		//엇이음 설정
	long m_nSOGShortAlterSpliceValue1;	//엇이음 설정 H1 Value

	BOOL m_bSOGEndInfoSet;
	long m_nSOGTopEndHookType;		//상부근 단부 0:정착 1:후크 
	long m_nSOGBotEndHookType;		//하부근 단부 0:정착 1:후크 
	long m_nSOGIsoFootDevType;		//독립기초 & Panel 접합부위 하부근 0:Passing 1:인장정착
	long m_nSOGSpliceDevSlabType;	//이음/정착 적용	0:기초	1:슬래브

	BOOL m_bSOGLevelDifInfoSet;
	long m_nSOGLevelDifPlaceShape;		//배근 형상
	long m_nSOGLevelDifHaunchPlaceType;	//헌치 배근타입

	BOOL m_bSOGOpeningInfoSet;
	long m_nSOGOpeningTopHookType;		//Open 부위 철근 설정 상부0:일자정착 1:후크정착 
	long m_nSOGOpeningBotHookType;		//Open 부위 철근 설정 하부0:일자정착 1:후크정착

	BOOL m_bSOGRebarAreaInfoSet;
	long m_nSOGRebarTolDist;		//앞뒤 철근의 길이차
	long m_nSOGRebarTolLenDif;		//가장 긴 철근과 짧은 철근이 길이차가 m_nMatRebarTolLenDif이상이 되면 barset을 나눈다.

	BOOL m_bSOGTopSpacerInfoSet;
	BOOL m_bSOGTopSpacerSet;			//상부 우마철근 설정
	long m_nSOGTopSpacerDiaType;		//직경 0:해당기초직경 1:사용자직경
	long m_nSOGTopSpacerDiaIndex;		//직경 : 사용자 직경 입력
	long m_nSOGTopSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nSOGTopSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nSOGTopSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nSOGTopSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nSOGTopSpacerHeightType;		//높이
	long m_nSOGTopSpacerUserHeight;		//높이 : 사용자 입력
	long m_nSOGTopSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nSOGTopSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nSOGTopSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nSOGTopSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nSOGTopSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nSOGTopSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nSOGTopSpacerWidth;			//폭

	BOOL m_bSOGBotSpacerInfoSet;
	BOOL m_bSOGBotSpacerSet;			//상부 우마철근 설정
	long m_nSOGBotSpacerDiaType;		//직경 0:해당기초직경 1:사용자직경
	long m_nSOGBotSpacerDiaIndex;		//직경 : 사용자 직경 입력
	long m_nSOGBotSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nSOGBotSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nSOGBotSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nSOGBotSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nSOGBotSpacerHeightType;		//높이
	long m_nSOGBotSpacerUserHeight;		//높이 : 사용자 입력
	long m_nSOGBotSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nSOGBotSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nSOGBotSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nSOGBotSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nSOGBotSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nSOGBotSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nSOGBotSpacerWidth;			//폭

	// CircularRamp
	BOOL m_bCircularRampLongMainBarInfoSet;
	long m_nCircularRampLongMaxDualHookLength;	//최대 양후크길이
	long m_nCircularRampLongSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nCircularRampLongSpliceType;	//이음타입 0:인장이음 1:압축이음
	BOOL m_bCircularRampLongAlterSpliceSet;		//엇이음 설정
	long m_nCircularRampLongAlterSpliceValue1;	//엇이음 설정 H1 Value

	BOOL m_bCircularRampShortMainBarInfoSet;
	long m_nCircularRampShortMaxDualHookLength;	//최대 양후크길이
	long m_nCircularRampShortSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nCircularRampShortSpliceType;	//이음타입 0:인장이음 1:압축이음
	BOOL m_bCircularRampShortAlterSpliceSet;		//엇이음 설정
	long m_nCircularRampShortAlterSpliceValue1;	//엇이음 설정 H1 Value

	BOOL m_bCircularRampEndInfoSet;
	long m_nCircularRampTopEndHookType;		//상부근 단부 0:정착 1:후크 
	long m_nCircularRampBotEndHookType;		//하부근 단부 0:정착 1:후크 
	long m_nCircularRampIsoFootDevType;		//독립기초 & Panel 접합부위 하부근 0:Passing 1:인장정착
	long m_nCircularRampSpliceDevSlabType;	//이음/정착 적용	0:기초	1:슬래브

	BOOL m_bCircularRampLevelDifInfoSet;
	long m_nCircularRampLevelDifPlaceShape;		//배근 형상
	long m_nCircularRampLevelDifHaunchPlaceType;	//헌치 배근타입

	BOOL m_bCircularRampOpeningInfoSet;
	long m_nCircularRampOpeningTopHookType;		//Open 부위 철근 설정 상부0:일자정착 1:후크정착 
	long m_nCircularRampOpeningBotHookType;		//Open 부위 철근 설정 하부0:일자정착 1:후크정착

	BOOL m_bCircularRampRebarAreaInfoSet;
	long m_nCircularRampRebarTolDist;		//앞뒤 철근의 길이차
	long m_nCircularRampRebarTolLenDif;		//가장 긴 철근과 짧은 철근이 길이차가 m_nMatRebarTolLenDif이상이 되면 barset을 나눈다.

	BOOL m_bCircularRampTopSpacerInfoSet;
	BOOL m_bCircularRampTopSpacerSet;			//상부 우마철근 설정
	long m_nCircularRampTopSpacerDiaType;		//직경 0:해당기초직경 1:사용자직경
	long m_nCircularRampTopSpacerDiaIndex;		//직경 : 사용자 직경 입력
	long m_nCircularRampTopSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nCircularRampTopSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nCircularRampTopSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nCircularRampTopSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nCircularRampTopSpacerHeightType;		//높이
	long m_nCircularRampTopSpacerUserHeight;		//높이 : 사용자 입력
	long m_nCircularRampTopSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nCircularRampTopSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nCircularRampTopSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nCircularRampTopSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nCircularRampTopSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nCircularRampTopSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nCircularRampTopSpacerWidth;			//폭

	BOOL m_bCircularRampBotSpacerInfoSet;
	BOOL m_bCircularRampBotSpacerSet;			//상부 우마철근 설정
	long m_nCircularRampBotSpacerDiaType;		//직경 0:해당기초직경 1:사용자직경
	long m_nCircularRampBotSpacerDiaIndex;		//직경 : 사용자 직경 입력
	long m_nCircularRampBotSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nCircularRampBotSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nCircularRampBotSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nCircularRampBotSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nCircularRampBotSpacerHeightType;		//높이
	long m_nCircularRampBotSpacerUserHeight;		//높이 : 사용자 입력
	long m_nCircularRampBotSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nCircularRampBotSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nCircularRampBotSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nCircularRampBotSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nCircularRampBotSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nCircularRampBotSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nCircularRampBotSpacerWidth;			//폭

	// PC Slab
	BOOL m_bPCSlabLongMainBarInfoSet;
	long m_nPCSlabLongMaxDualHookLength;	//최대 양후크길이
	long m_nPCSlabLongSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nPCSlabLongSpliceType;	//이음타입 0:인장이음 1:압축이음
	BOOL m_bPCSlabLongAlterSpliceSet;		//엇이음 설정
	long m_nPCSlabLongAlterSpliceValue1;	//엇이음 설정 H1 Value

	BOOL m_bPCSlabShortMainBarInfoSet;
	long m_nPCSlabShortMaxDualHookLength;	//최대 양후크길이
	long m_nPCSlabShortSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nPCSlabShortSpliceType;	//이음타입 0:인장이음 1:압축이음
	BOOL m_bPCSlabShortAlterSpliceSet;		//엇이음 설정
	long m_nPCSlabShortAlterSpliceValue1;	//엇이음 설정 H1 Value

	BOOL m_bPCSlabStructuredEndInfoSet;
	long m_nPCSlabTopStructuredEndHookType;			//상부근 단부 0:정착 1:후크 
	long m_nPCSlabBotStructuredEndHookType;			//하부근 단부 0:정착 1:150

	BOOL m_bPCSlabDeckInfoSet;
	long m_nPCSlabDeckDifDiaConnectBarType;	//0:이음 1:정착
	long m_nPCSlabDeckAnchorType;				//0:피복+Hook정착 1:정착
	long m_nPCSlabDeckTopEndRatioType;		//0:0.35Lx 1:0.25Lx+15D
	long m_nPCSlabDeckBotEndRatioType;		//0:Lx/2+30D

	BOOL m_bPCSlabUnStructuredEndInfoSet;
	long m_nPCSlabTopUnStructuredEndHookType;		//상부근 단부 0:정착 1:후크 
	long m_nPCSlabBotUnStructuredEndHookType;		//하부근 단부 0:정착 1:후크 
	long m_nPCSlabIsoFootUnStructuredDevType;		//독립기초 & Panel 접합부위 하부근 0:Passing 1:인장정착

	BOOL m_bPCSlabLevelDifInfoSet;
	long m_nPCSlabLevelDifPlaceShape;		//배근 형상
	long m_nPCSlabLevelDifHaunchPlaceType;	//헌치 배근타입

	BOOL m_bPCSlabOpeningInfoSet;
	long m_nPCSlabOpeningTopHookType;		//Open 부위 철근 설정 상부0:일자정착 1:후크정착 
	long m_nPCSlabOpeningBotHookType;		//Open 부위 철근 설정 하부0:일자정착 1:후크정착

	BOOL m_bPCSlabRebarAreaInfoSet;
	long m_nPCSlabRebarTolDist;		//앞뒤 철근의 길이차
	long m_nPCSlabRebarTolLenDif;		//가장 긴 철근과 짧은 철근이 길이차가 m_nMatRebarTolLenDif이상이 되면 barset을 나눈다.

	BOOL m_bPCSlabTopSpacerInfoSet;
	BOOL m_bPCSlabTopSpacerSet;			//상부 우마철근 설정
	long m_nPCSlabTopSpacerDiaType;		//직경 0:해당기초직경 1:사용자직경
	long m_nPCSlabTopSpacerDiaIndex;		//직경 : 사용자 직경 입력
	long m_nPCSlabTopSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nPCSlabTopSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nPCSlabTopSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nPCSlabTopSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nPCSlabTopSpacerHeightType;		//높이
	long m_nPCSlabTopSpacerUserHeight;		//높이 : 사용자 입력
	long m_nPCSlabTopSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nPCSlabTopSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nPCSlabTopSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nPCSlabTopSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nPCSlabTopSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nPCSlabTopSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nPCSlabTopSpacerWidth;			//폭

	BOOL m_bPCSlabBotSpacerInfoSet;
	BOOL m_bPCSlabBotSpacerSet;			//상부 우마철근 설정
	long m_nPCSlabBotSpacerDiaType;		//직경 0:해당기초직경 1:사용자직경
	long m_nPCSlabBotSpacerDiaIndex;		//직경 : 사용자 직경 입력
	long m_nPCSlabBotSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nPCSlabBotSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nPCSlabBotSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nPCSlabBotSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nPCSlabBotSpacerHeightType;		//높이
	long m_nPCSlabBotSpacerUserHeight;		//높이 : 사용자 입력
	long m_nPCSlabBotSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nPCSlabBotSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nPCSlabBotSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nPCSlabBotSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nPCSlabBotSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nPCSlabBotSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nPCSlabBotSpacerWidth;			//폭

	// 철근 일체형 데크 슬래브
	BOOL m_bEmbeddedDeckLongMainBarInfoSet;
	long m_nEmbeddedDeckLongMaxDualHookLength;	//최대 양후크길이
	long m_nEmbeddedDeckLongSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nEmbeddedDeckLongSpliceType;	//이음타입 0:인장이음 1:압축이음
	BOOL m_bEmbeddedDeckLongAlterSpliceSet;		//엇이음 설정
	long m_nEmbeddedDeckLongAlterSpliceValue1;	//엇이음 설정 H1 Value

	BOOL m_bEmbeddedDeckShortMainBarInfoSet;
	long m_nEmbeddedDeckShortMaxDualHookLength;	//최대 양후크길이
	long m_nEmbeddedDeckShortSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nEmbeddedDeckShortSpliceType;	//이음타입 0:인장이음 1:압축이음
	BOOL m_bEmbeddedDeckShortAlterSpliceSet;		//엇이음 설정
	long m_nEmbeddedDeckShortAlterSpliceValue1;	//엇이음 설정 H1 Value

	BOOL m_bEmbeddedDeckStructuredEndInfoSet;
	long m_nEmbeddedDeckTopStructuredEndHookType;			//상부근 단부 0:정착 1:후크 
	long m_nEmbeddedDeckBotStructuredEndHookType;			//하부근 단부 0:정착 1:150

	BOOL m_bEmbeddedDeckInfoSet;
	long m_nEmbeddedDeckDifDiaConnectBarType;	//0:이음 1:정착
	long m_nEmbeddedDeckAnchorType;				//0:피복+Hook정착 1:정착
	long m_nEmbeddedDeckTopEndRatioType;		//0:0.35Lx 1:0.25Lx+15D
	long m_nEmbeddedDeckBotEndRatioType;		//0:Lx/2+30D

	BOOL m_bEmbeddedDeckUnStructuredEndInfoSet;
	long m_nEmbeddedDeckTopUnStructuredEndHookType;		//상부근 단부 0:정착 1:후크 
	long m_nEmbeddedDeckBotUnStructuredEndHookType;		//하부근 단부 0:정착 1:후크 
	long m_nEmbeddedDeckIsoFootUnStructuredDevType;		//독립기초 & Panel 접합부위 하부근 0:Passing 1:인장정착

	BOOL m_bEmbeddedDeckLevelDifInfoSet;
	long m_nEmbeddedDeckLevelDifPlaceShape;		//배근 형상
	long m_nEmbeddedDeckLevelDifHaunchPlaceType;	//헌치 배근타입

	BOOL m_bEmbeddedDeckOpeningInfoSet;
	long m_nEmbeddedDeckOpeningTopHookType;		//Open 부위 철근 설정 상부0:일자정착 1:후크정착 
	long m_nEmbeddedDeckOpeningBotHookType;		//Open 부위 철근 설정 하부0:일자정착 1:후크정착

	BOOL m_bEmbeddedDeckRebarAreaInfoSet;
	long m_nEmbeddedDeckRebarTolDist;		//앞뒤 철근의 길이차
	long m_nEmbeddedDeckRebarTolLenDif;		//가장 긴 철근과 짧은 철근이 길이차가 m_nMatRebarTolLenDif이상이 되면 barset을 나눈다.

	// 일반 데크 슬래브
	BOOL m_bGeneralDeckLongMainBarInfoSet;
	long m_nGeneralDeckLongMaxDualHookLength;	//최대 양후크길이
	long m_nGeneralDeckLongSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nGeneralDeckLongSpliceType;	//이음타입 0:인장이음 1:압축이음
	BOOL m_bGeneralDeckLongAlterSpliceSet;		//엇이음 설정
	long m_nGeneralDeckLongAlterSpliceValue1;	//엇이음 설정 H1 Value

	BOOL m_bGeneralDeckShortMainBarInfoSet;
	long m_nGeneralDeckShortMaxDualHookLength;	//최대 양후크길이
	long m_nGeneralDeckShortSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nGeneralDeckShortSpliceType;	//이음타입 0:인장이음 1:압축이음
	BOOL m_bGeneralDeckShortAlterSpliceSet;		//엇이음 설정
	long m_nGeneralDeckShortAlterSpliceValue1;	//엇이음 설정 H1 Value

	BOOL m_bGeneralDeckStructuredEndInfoSet;
	long m_nGeneralDeckTopStructuredEndHookType;			//상부근 단부 0:정착 1:후크 
	long m_nGeneralDeckBotStructuredEndHookType;			//하부근 단부 0:정착 1:150

	BOOL m_bGeneralDeckUnStructuredEndInfoSet;
	long m_nGeneralDeckTopUnStructuredEndHookType;		//상부근 단부 0:정착 1:후크 
	long m_nGeneralDeckBotUnStructuredEndHookType;		//하부근 단부 0:정착 1:후크 
	long m_nGeneralDeckIsoFootUnStructuredDevType;		//독립기초 & Panel 접합부위 하부근 0:Passing 1:인장정착

	BOOL m_bGeneralDeckLevelDifInfoSet;
	long m_nGeneralDeckLevelDifPlaceShape;		//배근 형상
	long m_nGeneralDeckLevelDifHaunchPlaceType;	//헌치 배근타입

	BOOL m_bGeneralDeckOpeningInfoSet;
	long m_nGeneralDeckOpeningTopHookType;		//Open 부위 철근 설정 상부0:일자정착 1:후크정착 
	long m_nGeneralDeckOpeningBotHookType;		//Open 부위 철근 설정 하부0:일자정착 1:후크정착

	BOOL m_bGeneralDeckRebarAreaInfoSet;
	long m_nGeneralDeckRebarTolDist;		//앞뒤 철근의 길이차
	long m_nGeneralDeckRebarTolLenDif;		//가장 긴 철근과 짧은 철근이 길이차가 m_nMatRebarTolLenDif이상이 되면 barset을 나눈다.
};

