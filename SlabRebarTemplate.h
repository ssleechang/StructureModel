#pragma once
#include "BaseRebarTemplate.h"

class StructureModel_CLASS SlabRebarTemplate : public BaseRebarTemplate
{
public:
	SlabRebarTemplate();
	~SlabRebarTemplate(void);

	void Serialize(CArchive& ar);

	virtual void CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate) override;

	void DataInit();

	// MAT Slab
	virtual BOOL GetMatMainBarInfoSet() { return m_bMatMainBarInfoSet; }
	virtual long GetMatMaxDualHookLength() { return m_nMatMaxDualHookLength; }
	virtual long GetMatSpliceWay() { return m_nMatSpliceWay; }
	virtual long GetMatSpliceType() { return m_nMatSpliceType; }

	virtual BOOL GetMatEndInfoSet() { return m_bMatEndInfoSet; }
	virtual long GetMatTopEndHookType() { return m_nMatTopEndHookType; }
	virtual long GetMatBotEndHookType() { return m_nMatBotEndHookType; }
	virtual long GetMatIsoFootDevType() { return m_nMatIsoFootDevType; }

	virtual long GetMatTopEndDevTypeUserLength() { return m_nMatTopEndDevTypeUserLength; }
	virtual long GetMatBotEndDevTypeUserLength() { return m_nMatBotEndDevTypeUserLength; }
	virtual long GetMatOpeningTopDevType() { return m_nMatOpeningTopDevType; }
	virtual long GetMatOpeningBotDevType() { return m_nMatOpeningBotDevType; }

	virtual BOOL GetMatLevelDifInfoSet() { return m_bMatLevelDifInfoSet; }
	virtual long GetMatLevelDifPlaceShape() { return m_nMatLevelDifPlaceShape; }
	virtual long GetMatLevelDifHaunchPlaceType() { return m_nMatLevelDifHaunchPlaceType; }

	virtual BOOL GetMatSupportRebarsSet() { return m_bMatSupportRebarsSet; }
	virtual long GetMatSupportRebarsDiaIndex() { return m_nMatSupportRebarsDiaIndex; }
	virtual long GetMatSupportRebarsLengthType() { return m_nMatSupportRebarsLengthType; }
	virtual double GetMatSupportRebarsRWallLengthRate() { return m_dMatSupportRebarsRWallLengthRate; }
	virtual double GetMatSupportRebarsSlabLengthRate() { return m_dMatSupportRebarsSlabLengthRate; }
	virtual long GetMatSupportRebarsOrderLength() { return m_nMatSupportRebarsOrderLength; }
	virtual long GetMatSupportRebarsSpliceLength() { return m_nMatSupportRebarsSpliceLength; }
	virtual long GetMatSupportRebarsCorrectiveLength() { return m_nMatSupportRebarsCorrectiveLength; }

	virtual BOOL GetMatRebarAreaInfoSet() { return m_bMatRebarAreaInfoSet; }
	virtual long GetMatRebarTolDist() { return m_nMatRebarTolDist; }
	virtual long GetMatRebarTolLenDif() { return m_nMatRebarTolLenDif; }

	virtual BOOL GetMatSpliceDevConditionSet() { return m_bMatSpliceDevConditionSet; }
	virtual long GetMatSpliceDevConditionLength() { return m_nMatSpliceDevConditionLength; }

	virtual BOOL GetMatSpliceAddLengthSet() { return m_bMatSpliceAddLengthSet; }
	virtual long GetMatSpliceAddLengthNumSplice() { return m_nMatSpliceAddLengthNumSplice; }

	virtual BOOL GetMatTopSpacerInfoSet() { return m_bMatTopSpacerInfoSet; }
	virtual BOOL GetMatTopSpacerSet() { return m_bMatTopSpacerSet; }
	virtual long GetMatTopSpacerDiaType() { return m_nMatTopSpacerDiaType; }
	virtual long GetMatTopSpacerSlabThick() { return m_nMatTopSpacerSlabThick; }
	virtual long GetMatTopSpacerSlabThickOverDiaIndex() { return m_nMatTopSpacerSlabThickOverDiaIndex; }
	virtual long GetMatTopSpacerSlabThickUnderDiaIndex() { return m_nMatTopSpacerSlabThickUnderDiaIndex; }
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

	virtual BOOL GetMatSpacerSupportBarsSet() { return m_bMatSpacerSupportBarsSet; }
	virtual BOOL GetMatSpacerSupportBarsConditionSet() { return m_bMatSpacerSupportBarsConditionSet; }
	virtual long GetMatSpacerSupportBarsCondition() { return m_nMatSpacerSupportBarsCondition; }
	virtual long GetMatSpacerSupportBarsDiaType() { return m_nMatSpacerSupportBarsDiaType; }
	virtual long GetMatSpacerSupportBarsUserDiaIndex() { return m_nMatSpacerSupportBarsUserDiaIndex; }
	virtual long GetMatSpacerSupportBarsNumType() { return m_nMatSpacerSupportBarsNumType; }
	virtual double GetMatSpacerSupportBarsTopSpacerNums() { return m_dMatSpacerSupportBarsTopSpacerNums; }
	virtual long GetMatSpacerSupportBarsUserNums() { return m_nMatSpacerSupportBarsUserNums; }
	virtual long GetMatSpacerSupportBarsLengthType() { return m_nMatSpacerSupportBarsLengthType; }
	virtual double GetMatSpacerSupportBarsUserRatio() { return m_dMatSpacerSupportBarsUserRatio; }
	virtual long GetMatSpacerSupportBarsUserLength() { return m_nMatSpacerSupportBarsUserLength; }

	virtual long GetMatCZoneSpliceWay() { return m_nMatCZoneSpliceWay; }
	virtual long GetMatCZoneSpliceLength() { return m_nMatCZoneSpliceLength; }

	virtual void SetMatMainBarInfoSet(BOOL value) { m_bMatMainBarInfoSet = value; }
	virtual void SetMatMaxDualHookLength(long value) { m_nMatMaxDualHookLength = value; }
	virtual void SetMatSpliceWay(long value) { m_nMatSpliceWay = value; }
	virtual void SetMatSpliceType(long value) { m_nMatSpliceType = value; }

	virtual void SetMatEndInfoSet(BOOL value) { m_bMatEndInfoSet = value; }
	virtual void SetMatTopEndHookType(long value) { m_nMatTopEndHookType = value; }
	virtual void SetMatBotEndHookType(long value) { m_nMatBotEndHookType = value; }
	virtual void SetMatIsoFootDevType(long value) { m_nMatIsoFootDevType = value; }

	virtual void SetMatTopEndDevTypeUserLength(long value) { m_nMatTopEndDevTypeUserLength = value; }
	virtual void SetMatBotEndDevTypeUserLength(long value) { m_nMatBotEndDevTypeUserLength = value; }
	virtual void SetMatOpeningTopDevType(long value) { m_nMatOpeningTopDevType = value; }
	virtual void SetMatOpeningBotDevType(long value) { m_nMatOpeningBotDevType = value; }

	virtual void SetMatLevelDifInfoSet(BOOL value) { m_bMatLevelDifInfoSet = value; }
	virtual void SetMatLevelDifPlaceShape(long value) { m_nMatLevelDifPlaceShape = value; }
	virtual void SetMatLevelDifHaunchPlaceType(long value) { m_nMatLevelDifHaunchPlaceType = value; }

	virtual void SetMatSupportRebarsSet(BOOL value) { m_bMatSupportRebarsSet = value; }
	virtual void SetMatSupportRebarsDiaIndex(long value) { m_nMatSupportRebarsDiaIndex = value; }
	virtual void SetMatSupportRebarsLengthType(long value) { m_nMatSupportRebarsLengthType = value; }
	virtual void SetMatSupportRebarsRWallLengthRate(double value) { m_dMatSupportRebarsRWallLengthRate = value; }
	virtual void SetMatSupportRebarsSlabLengthRate(double value) { m_dMatSupportRebarsSlabLengthRate = value; }
	virtual void SetMatSupportRebarsOrderLength(long value) { m_nMatSupportRebarsOrderLength = value; }
	virtual void SetMatSupportRebarsSpliceLength(long value) { m_nMatSupportRebarsSpliceLength = value; }
	virtual void SetMatSupportRebarsCorrectiveLength(long value) { m_nMatSupportRebarsCorrectiveLength = value; }

	virtual void SetMatRebarAreaInfoSet(BOOL value) { m_bMatRebarAreaInfoSet = value; }
	virtual void SetMatRebarTolDist(long value) { m_nMatRebarTolDist = value; }
	virtual void SetMatRebarTolLenDif(long value) { m_nMatRebarTolLenDif = value; }

	virtual void SetMatSpliceDevConditionSet(BOOL value) { m_bMatSpliceDevConditionSet = value; }
	virtual void SetMatSpliceDevConditionLength(long value) { m_nMatSpliceDevConditionLength = value; }

	virtual void SetMatSpliceAddLengthSet(BOOL value) { m_bMatSpliceAddLengthSet = value; }
	virtual void SetMatSpliceAddLengthNumSplice(long value) { m_nMatSpliceAddLengthNumSplice = value; }

	virtual void SetMatTopSpacerInfoSet(BOOL value) { m_bMatTopSpacerInfoSet = value; }
	virtual void SetMatTopSpacerSet(BOOL value) { m_bMatTopSpacerSet = value; }
	virtual void SetMatTopSpacerDiaType(long value) { m_nMatTopSpacerDiaType = value; }
	virtual void SetMatTopSpacerSlabThick(long value) { m_nMatTopSpacerSlabThick = value; }
	virtual void SetMatTopSpacerSlabThickOverDiaIndex(long value) { m_nMatTopSpacerSlabThickOverDiaIndex = value; }
	virtual void SetMatTopSpacerSlabThickUnderDiaIndex(long value) { m_nMatTopSpacerSlabThickUnderDiaIndex = value; }
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

	virtual void SetMatSpacerSupportBarsSet(BOOL value) { m_bMatSpacerSupportBarsSet = value; }
	virtual void SetMatSpacerSupportBarsConditionSet(BOOL value) { m_bMatSpacerSupportBarsConditionSet = value; }
	virtual void SetMatSpacerSupportBarsCondition(long value) { m_nMatSpacerSupportBarsCondition = value; }
	virtual void SetMatSpacerSupportBarsDiaType(long value) { m_nMatSpacerSupportBarsDiaType = value; }
	virtual void SetMatSpacerSupportBarsUserDiaIndex(long value) { m_nMatSpacerSupportBarsUserDiaIndex = value; }
	virtual void SetMatSpacerSupportBarsNumType(long value) { m_nMatSpacerSupportBarsNumType = value; }
	virtual void SetMatSpacerSupportBarsTopSpacerNums(double value) { m_dMatSpacerSupportBarsTopSpacerNums = value; }
	virtual void SetMatSpacerSupportBarsUserNums(long value) { m_nMatSpacerSupportBarsUserNums = value; }
	virtual void SetMatSpacerSupportBarsLengthType(long value) { m_nMatSpacerSupportBarsLengthType = value; }
	virtual void SetMatSpacerSupportBarsUserRatio(double value) { m_dMatSpacerSupportBarsUserRatio = value; }
	virtual void SetMatSpacerSupportBarsUserLength(long value) { m_nMatSpacerSupportBarsUserLength = value; }

	virtual void SetMatCZoneSpliceWay(long value) { m_nMatCZoneSpliceWay = value; }
	virtual void SetMatCZoneSpliceLength(long value) { m_nMatCZoneSpliceLength = value; }

	//FLAT Slab
	virtual BOOL GetFlatMainBarInfoSet() { return m_bFlatMainBarInfoSet; }
	virtual long GetFlatMaxDualHookLength() { return m_nFlatMaxDualHookLength; }
	virtual long GetFlatSpliceWay() { return m_nFlatSpliceWay; }
	virtual long GetFlatSpliceType() { return m_nFlatSpliceType; }

	virtual BOOL GetFlatEndInfoSet() { return m_bFlatEndInfoSet; }
	virtual long GetFlatTopEndHookType() { return m_nFlatTopEndHookType; }
	virtual long GetFlatBotEndHookType() { return m_nFlatBotEndHookType; }
	virtual long GetFlatIsoFootDevType() { return m_nFlatIsoFootDevType; }
	virtual long GetFlatSpliceDevSlabType() { return m_nFlatSpliceDevSlabType; }

	virtual long GetFlatTopEndDevTypeUserLength() { return m_nFlatTopEndDevTypeUserLength; }
	virtual long GetFlatBotEndDevTypeUserLength() { return m_nFlatBotEndDevTypeUserLength; }
	virtual long GetFlatOpeningTopDevType() { return m_nFlatOpeningTopDevType; }
	virtual long GetFlatOpeningBotDevType() { return m_nFlatOpeningBotDevType; }

	virtual BOOL GetFlatLevelDifInfoSet() { return m_bFlatLevelDifInfoSet; }
	virtual long GetFlatLevelDifPlaceShape() { return m_nFlatLevelDifPlaceShape; }
	virtual long GetFlatLevelDifHaunchPlaceType() { return m_nFlatLevelDifHaunchPlaceType; }


	virtual BOOL GetFlatOpeningInfoSet() { return m_bFlatOpeningInfoSet; }
	
	virtual BOOL GetFlatRebarAreaInfoSet() { return m_bFlatRebarAreaInfoSet; }
	virtual long GetFlatRebarTolDist() { return m_nFlatRebarTolDist; }
	virtual long GetFlatRebarTolLenDif() { return m_nFlatRebarTolLenDif; }

	virtual BOOL GetFlatSpliceDevConditionSet() { return m_bFlatSpliceDevConditionSet; }
	virtual long GetFlatSpliceDevConditionLength() { return m_nFlatSpliceDevConditionLength; }

	virtual BOOL GetFlatSpliceAddLengthSet() { return m_bFlatSpliceAddLengthSet; }
	virtual long GetFlatSpliceAddLengthNumSplice() { return m_nFlatSpliceAddLengthNumSplice; }

	virtual BOOL GetFlatTopSpacerInfoSet() { return m_bFlatTopSpacerInfoSet; }
	virtual BOOL GetFlatTopSpacerSet() { return m_bFlatTopSpacerSet; }
	virtual long GetFlatTopSpacerDiaType() { return m_nFlatTopSpacerDiaType; }
	virtual long GetFlatTopSpacerSlabThick() { return m_nFlatTopSpacerSlabThick; }
	virtual long GetFlatTopSpacerSlabThickOverDiaIndex() { return m_nFlatTopSpacerSlabThickOverDiaIndex; }
	virtual long GetFlatTopSpacerSlabThickUnderDiaIndex() { return m_nFlatTopSpacerSlabThickUnderDiaIndex; }
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

	virtual BOOL GetFlatSpacerSupportBarsSet() { return m_bFlatSpacerSupportBarsSet; }
	virtual BOOL GetFlatSpacerSupportBarsConditionSet() { return m_bFlatSpacerSupportBarsConditionSet; }
	virtual long GetFlatSpacerSupportBarsCondition() { return m_nFlatSpacerSupportBarsCondition; }
	virtual long GetFlatSpacerSupportBarsDiaType() { return m_nFlatSpacerSupportBarsDiaType; }
	virtual long GetFlatSpacerSupportBarsUserDiaIndex() { return m_nFlatSpacerSupportBarsUserDiaIndex; }
	virtual long GetFlatSpacerSupportBarsNumType() { return m_nFlatSpacerSupportBarsNumType; }
	virtual double GetFlatSpacerSupportBarsTopSpacerNums() { return m_dFlatSpacerSupportBarsTopSpacerNums; }
	virtual long GetFlatSpacerSupportBarsUserNums() { return m_nFlatSpacerSupportBarsUserNums; }
	virtual long GetFlatSpacerSupportBarsLengthType() { return m_nFlatSpacerSupportBarsLengthType; }
	virtual double GetFlatSpacerSupportBarsUserRatio() { return m_dFlatSpacerSupportBarsUserRatio; }
	virtual long GetFlatSpacerSupportBarsUserLength() { return m_nFlatSpacerSupportBarsUserLength; }

	virtual long GetFlatCZoneSpliceWay() { return m_nFlatCZoneSpliceWay; }
	virtual long GetFlatCZoneSpliceLength() { return m_nFlatCZoneSpliceLength; }

	virtual void SetFlatMainBarInfoSet(BOOL value) { m_bFlatMainBarInfoSet = value; }
	virtual void SetFlatMaxDualHookLength(long value) { m_nFlatMaxDualHookLength = value; }
	virtual void SetFlatSpliceWay(long value) { m_nFlatSpliceWay = value; }
	virtual void SetFlatSpliceType(long value) { m_nFlatSpliceType = value; }

	virtual void SetFlatEndInfoSet(BOOL value) { m_bFlatEndInfoSet = value; }
	virtual void SetFlatTopEndHookType(long value) { m_nFlatTopEndHookType = value; }
	virtual void SetFlatBotEndHookType(long value) { m_nFlatBotEndHookType = value; }
	virtual void SetFlatIsoFootDevType(long value) { m_nFlatIsoFootDevType = value; }
	virtual void SetFlatSpliceDevSlabType(long value) { m_nFlatSpliceDevSlabType = value; }

	virtual void SetFlatTopEndDevTypeUserLength(long value) { m_nFlatTopEndDevTypeUserLength = value; }
	virtual void SetFlatBotEndDevTypeUserLength(long value) { m_nFlatBotEndDevTypeUserLength = value; }
	virtual void SetFlatOpeningTopDevType(long value) { m_nFlatOpeningTopDevType = value; }
	virtual void SetFlatOpeningBotDevType(long value) { m_nFlatOpeningBotDevType = value; }

	virtual void SetFlatLevelDifInfoSet(BOOL value) { m_bFlatLevelDifInfoSet = value; }
	virtual void SetFlatLevelDifPlaceShape(long value) { m_nFlatLevelDifPlaceShape = value; }
	virtual void SetFlatLevelDifHaunchPlaceType(long value) { m_nFlatLevelDifHaunchPlaceType = value; }


	virtual void SetFlatOpeningInfoSet(BOOL value) { m_bFlatOpeningInfoSet = value; }
	
	virtual void SetFlatRebarAreaInfoSet(BOOL value) { m_bFlatRebarAreaInfoSet = value; }
	virtual void SetFlatRebarTolDist(long value) { m_nFlatRebarTolDist = value; }
	virtual void SetFlatRebarTolLenDif(long value) { m_nFlatRebarTolLenDif = value; }

	virtual void SetFlatSpliceDevConditionSet(BOOL value) { m_bFlatSpliceDevConditionSet = value; }
	virtual void SetFlatSpliceDevConditionLength(long value) { m_nFlatSpliceDevConditionLength = value; }

	virtual void SetFlatSpliceAddLengthSet(BOOL value) { m_bFlatSpliceAddLengthSet = value; }
	virtual void SetFlatSpliceAddLengthNumSplice(long value) { m_nFlatSpliceAddLengthNumSplice = value; }

	virtual void SetFlatTopSpacerInfoSet(BOOL value) { m_bFlatTopSpacerInfoSet = value; }
	virtual void SetFlatTopSpacerSet(BOOL value) { m_bFlatTopSpacerSet = value; }
	virtual void SetFlatTopSpacerDiaType(long value) { m_nFlatTopSpacerDiaType = value; }
	virtual void SetFlatTopSpacerSlabThick(long value) { m_nFlatTopSpacerSlabThick = value; }
	virtual void SetFlatTopSpacerSlabThickOverDiaIndex(long value) { m_nFlatTopSpacerSlabThickOverDiaIndex = value; }
	virtual void SetFlatTopSpacerSlabThickUnderDiaIndex(long value) { m_nFlatTopSpacerSlabThickUnderDiaIndex = value; }
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

	virtual void SetFlatSpacerSupportBarsSet(BOOL value) { m_bFlatSpacerSupportBarsSet = value; }
	virtual void SetFlatSpacerSupportBarsConditionSet(BOOL value) { m_bFlatSpacerSupportBarsConditionSet = value; }
	virtual void SetFlatSpacerSupportBarsCondition(long value) { m_nFlatSpacerSupportBarsCondition = value; }
	virtual void SetFlatSpacerSupportBarsDiaType(long value) { m_nFlatSpacerSupportBarsDiaType = value; }
	virtual void SetFlatSpacerSupportBarsUserDiaIndex(long value) { m_nFlatSpacerSupportBarsUserDiaIndex = value; }
	virtual void SetFlatSpacerSupportBarsNumType(long value) { m_nFlatSpacerSupportBarsNumType = value; }
	virtual void SetFlatSpacerSupportBarsTopSpacerNums(double value) { m_dFlatSpacerSupportBarsTopSpacerNums = value; }
	virtual void SetFlatSpacerSupportBarsUserNums(long value) { m_nFlatSpacerSupportBarsUserNums = value; }
	virtual void SetFlatSpacerSupportBarsLengthType(long value) { m_nFlatSpacerSupportBarsLengthType = value; }
	virtual void SetFlatSpacerSupportBarsUserRatio(double value) { m_dFlatSpacerSupportBarsUserRatio = value; }
	virtual void SetFlatSpacerSupportBarsUserLength(long value) { m_nFlatSpacerSupportBarsUserLength = value; }

	virtual void SetFlatCZoneSpliceWay(long value) { m_nFlatCZoneSpliceWay = value; }
	virtual void SetFlatCZoneSpliceLength(long value) { m_nFlatCZoneSpliceLength = value; }

	// 사변지지 SLAB
	virtual BOOL GetSlabMainBarInfoSet() { return m_bSlabMainBarInfoSet; }
	virtual long GetSlabMaxDualHookLength() { return m_nSlabMaxDualHookLength; }
	virtual long GetSlabSpliceWay() { return m_nSlabSpliceWay; }
	virtual long GetSlabSpliceType() { return m_nSlabSpliceType; }

	virtual BOOL GetSlabTopCutBarLenInfoSet() { return m_bSlabTopCutBarLenInfoSet; }
	virtual long GetSlabOneWayCutBarLenType() { return m_nSlabOneWayCutBarLenType; }
	virtual long GetSlabTwoWayCutBarLenType() { return m_nSlabTwoWayCutBarLenType; }

	virtual BOOL GetSlabUnStructuredEndInfoSet() { return m_bSlabUnStructuredEndInfoSet; }
	virtual long GetSlabTopUnStructuredEndHookType() { return m_nSlabTopUnStructuredEndHookType; }
	virtual long GetSlabBotUnStructuredEndHookType() { return m_nSlabBotUnStructuredEndHookType; }

	virtual long GetSlabTopEndDevTypeUserLength() { return m_nSlabTopEndDevTypeUserLength; }
	virtual long GetSlabBotEndDevTypeUserLength() { return m_nSlabBotEndDevTypeUserLength; }
	virtual long GetSlabOpeningTopDevType() { return m_nSlabOpeningTopDevType; }
	virtual long GetSlabOpeningBotDevType() { return m_nSlabOpeningBotDevType; }


	virtual BOOL GetSlabLevelDifInfoSet() { return m_bSlabLevelDifInfoSet; }
	virtual long GetSlabLevelDifPlaceShape() { return m_nSlabLevelDifPlaceShape; }
	virtual long GetSlabLevelDifHaunchPlaceType() { return m_nSlabLevelDifHaunchPlaceType; }
	
	virtual BOOL GetSlabOpeningInfoSet() { return m_bSlabOpeningInfoSet; }
	
	virtual BOOL GetSlabRebarAreaInfoSet() { return m_bSlabRebarAreaInfoSet; }
	virtual long GetSlabRebarTolDist() { return m_nSlabRebarTolDist; }
	virtual long GetSlabRebarTolLenDif() { return m_nSlabRebarTolLenDif; }

	virtual BOOL GetSlabSpliceDevConditionSet() { return m_bSlabSpliceDevConditionSet; }
	virtual long GetSlabSpliceDevConditionLength() { return m_nSlabSpliceDevConditionLength; }

	virtual BOOL GetSlabSpliceAddLengthSet() { return m_bSlabSpliceAddLengthSet; }
	virtual long GetSlabSpliceAddLengthNumSplice() { return m_nSlabSpliceAddLengthNumSplice; }

	virtual BOOL GetSlabTopSpacerInfoSet() { return m_bSlabTopSpacerInfoSet; }
	virtual BOOL GetSlabTopSpacerSet() { return m_bSlabTopSpacerSet; }
	virtual long GetSlabTopSpacerDiaType() { return m_nSlabTopSpacerDiaType; }
	virtual long GetSlabTopSpacerSlabThick() { return m_nSlabTopSpacerSlabThick; }
	virtual long GetSlabTopSpacerSlabThickOverDiaIndex() { return m_nSlabTopSpacerSlabThickOverDiaIndex; }
	virtual long GetSlabTopSpacerSlabThickUnderDiaIndex() { return m_nSlabTopSpacerSlabThickUnderDiaIndex; }
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

	virtual BOOL GetSlabSpacerSupportBarsSet() { return m_bSlabSpacerSupportBarsSet; }
	virtual BOOL GetSlabSpacerSupportBarsConditionSet() { return m_bSlabSpacerSupportBarsConditionSet; }
	virtual long GetSlabSpacerSupportBarsCondition() { return m_nSlabSpacerSupportBarsCondition; }
	virtual long GetSlabSpacerSupportBarsDiaType() { return m_nSlabSpacerSupportBarsDiaType; }
	virtual long GetSlabSpacerSupportBarsUserDiaIndex() { return m_nSlabSpacerSupportBarsUserDiaIndex; }
	virtual long GetSlabSpacerSupportBarsNumType() { return m_nSlabSpacerSupportBarsNumType; }
	virtual double GetSlabSpacerSupportBarsTopSpacerNums() { return m_dSlabSpacerSupportBarsTopSpacerNums; }
	virtual long GetSlabSpacerSupportBarsUserNums() { return m_nSlabSpacerSupportBarsUserNums; }
	virtual long GetSlabSpacerSupportBarsLengthType() { return m_nSlabSpacerSupportBarsLengthType; }
	virtual double GetSlabSpacerSupportBarsUserRatio() { return m_dSlabSpacerSupportBarsUserRatio; }
	virtual long GetSlabSpacerSupportBarsUserLength() { return m_nSlabSpacerSupportBarsUserLength; }

	virtual long GetSlabCZoneSpliceWay() { return m_nSlabCZoneSpliceWay; }
	virtual long GetSlabCZoneSpliceLength() { return m_nSlabCZoneSpliceLength; }

	virtual void SetSlabMainBarInfoSet(BOOL value) { m_bSlabMainBarInfoSet = value; }
	virtual void SetSlabMaxDualHookLength(long value) { m_nSlabMaxDualHookLength = value; }
	virtual void SetSlabSpliceWay(long value) { m_nSlabSpliceWay = value; }
	virtual void SetSlabSpliceType(long value) { m_nSlabSpliceType = value; }
			
	virtual void SetSlabTopCutBarLenInfoSet(BOOL value) { m_bSlabTopCutBarLenInfoSet = value; }
	virtual void SetSlabOneWayCutBarLenType(long value) { m_nSlabOneWayCutBarLenType = value; }
	virtual void SetSlabTwoWayCutBarLenType(long value) { m_nSlabTwoWayCutBarLenType = value; }

	virtual void SetSlabUnStructuredEndInfoSet(BOOL value) { m_bSlabUnStructuredEndInfoSet = value; }
	virtual void SetSlabTopUnStructuredEndHookType(long value) { m_nSlabTopUnStructuredEndHookType = value; }
	virtual void SetSlabBotUnStructuredEndHookType(long value) { m_nSlabBotUnStructuredEndHookType = value; }

	virtual void SetSlabTopEndDevTypeUserLength(long value) { m_nSlabTopEndDevTypeUserLength = value;	}
	virtual void SetSlabBotEndDevTypeUserLength(long value) { m_nSlabBotEndDevTypeUserLength = value;	}
	virtual void SetSlabOpeningTopDevType(long value) { m_nSlabOpeningTopDevType = value;	}
	virtual void SetSlabOpeningBotDevType(long value) { m_nSlabOpeningBotDevType = value;	}

	virtual void SetSlabLevelDifInfoSet(BOOL value) { m_bSlabLevelDifInfoSet = value; }
	virtual void SetSlabLevelDifPlaceShape(long value) { m_nSlabLevelDifPlaceShape = value; }
	virtual void SetSlabLevelDifHaunchPlaceType(long value) { m_nSlabLevelDifHaunchPlaceType = value; }
		
	virtual void SetSlabOpeningInfoSet(BOOL value) { m_bSlabOpeningInfoSet = value; }
	
	virtual void SetSlabRebarAreaInfoSet(BOOL value) { m_bSlabRebarAreaInfoSet = value; }
	virtual void SetSlabRebarTolDist(long value) { m_nSlabRebarTolDist = value; }
	virtual void SetSlabRebarTolLenDif(long value) { m_nSlabRebarTolLenDif = value; }

	virtual void SetSlabSpliceDevConditionSet(BOOL value) { m_bSlabSpliceDevConditionSet = value; }
	virtual void SetSlabSpliceDevConditionLength(long value) { m_nSlabSpliceDevConditionLength = value; }

	virtual void SetSlabSpliceAddLengthSet(BOOL value) { m_bSlabSpliceAddLengthSet = value; }
	virtual void SetSlabSpliceAddLengthNumSplice(long value) { m_nSlabSpliceAddLengthNumSplice = value; }

	virtual void SetSlabTopSpacerInfoSet(BOOL value) { m_bSlabTopSpacerInfoSet = value; }
	virtual void SetSlabTopSpacerSet(BOOL value) { m_bSlabTopSpacerSet = value; }
	virtual void SetSlabTopSpacerDiaType(long value) { m_nSlabTopSpacerDiaType = value; }
	virtual void SetSlabTopSpacerSlabThick(long value) { m_nSlabTopSpacerSlabThick = value; }
	virtual void SetSlabTopSpacerSlabThickOverDiaIndex(long value) { m_nSlabTopSpacerSlabThickOverDiaIndex = value; }
	virtual void SetSlabTopSpacerSlabThickUnderDiaIndex(long value) { m_nSlabTopSpacerSlabThickUnderDiaIndex = value; }
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

	virtual void SetSlabSpacerSupportBarsSet(BOOL value) { m_bSlabSpacerSupportBarsSet = value; }
	virtual void SetSlabSpacerSupportBarsConditionSet(BOOL value) { m_bSlabSpacerSupportBarsConditionSet = value; }
	virtual void SetSlabSpacerSupportBarsCondition(long value) { m_nSlabSpacerSupportBarsCondition = value; }
	virtual void SetSlabSpacerSupportBarsDiaType(long value) { m_nSlabSpacerSupportBarsDiaType = value; }
	virtual void SetSlabSpacerSupportBarsUserDiaIndex(long value) { m_nSlabSpacerSupportBarsUserDiaIndex = value; }
	virtual void SetSlabSpacerSupportBarsNumType(long value) { m_nSlabSpacerSupportBarsNumType = value; }
	virtual void SetSlabSpacerSupportBarsTopSpacerNums(double value) { m_dSlabSpacerSupportBarsTopSpacerNums = value; }
	virtual void SetSlabSpacerSupportBarsUserNums(long value) { m_nSlabSpacerSupportBarsUserNums = value; }
	virtual void SetSlabSpacerSupportBarsLengthType(long value) { m_nSlabSpacerSupportBarsLengthType = value; }
	virtual void SetSlabSpacerSupportBarsUserRatio(double value) { m_dSlabSpacerSupportBarsUserRatio = value; }
	virtual void SetSlabSpacerSupportBarsUserLength(long value) { m_nSlabSpacerSupportBarsUserLength = value; }

	virtual void SetSlabCZoneSpliceWay(long value) { m_nSlabCZoneSpliceWay = value; }
	virtual void SetSlabCZoneSpliceLength(long value) { m_nSlabCZoneSpliceLength = value; }

	// SOG Slab
	virtual BOOL GetSOGMainBarInfoSet() { return m_bSOGMainBarInfoSet; }
	virtual long GetSOGMaxDualHookLength() { return m_nSOGMaxDualHookLength; }
	virtual long GetSOGSpliceWay() { return m_nSOGSpliceWay; }
	virtual long GetSOGSpliceType() { return m_nSOGSpliceType; }
		
	virtual BOOL GetSOGEndInfoSet() { return m_bSOGEndInfoSet; }
	virtual long GetSOGTopEndHookType() { return m_nSOGTopEndHookType; }
	virtual long GetSOGBotEndHookType() { return m_nSOGBotEndHookType; }
	virtual long GetSOGIsoFootDevType() { return m_nSOGIsoFootDevType; }
	virtual long GetSOGSpliceDevSlabType() { return m_nSOGSpliceDevSlabType; }

	virtual long GetSOGTopEndDevTypeUserLength() { return m_nSOGTopEndDevTypeUserLength; }
	virtual long GetSOGBotEndDevTypeUserLength() { return m_nSOGBotEndDevTypeUserLength; }
	virtual long GetSOGOpeningTopDevType() { return m_nSOGOpeningTopDevType; }
	virtual long GetSOGOpeningBotDevType() { return m_nSOGOpeningBotDevType; }

	virtual BOOL GetSOGLevelDifInfoSet() { return m_bSOGLevelDifInfoSet; }
	virtual long GetSOGLevelDifPlaceShape() { return m_nSOGLevelDifPlaceShape; }
	virtual long GetSOGLevelDifHaunchPlaceType() { return m_nSOGLevelDifHaunchPlaceType; }

	virtual BOOL GetSOGSupportRebarsSet() { return m_bSOGSupportRebarsSet; }
	virtual long GetSOGSupportRebarsDiaIndex() { return m_nSOGSupportRebarsDiaIndex; }
	virtual long GetSOGSupportRebarsLengthType() { return m_nSOGSupportRebarsLengthType; }
	virtual double GetSOGSupportRebarsRWallLengthRate() { return m_dSOGSupportRebarsRWallLengthRate; }
	virtual double GetSOGSupportRebarsSlabLengthRate() { return m_dSOGSupportRebarsSlabLengthRate; }
	virtual long GetSOGSupportRebarsOrderLength() { return m_nSOGSupportRebarsOrderLength; }
	virtual long GetSOGSupportRebarsSpliceLength() { return m_nSOGSupportRebarsSpliceLength; }
	virtual long GetSOGSupportRebarsCorrectiveLength() { return m_nSOGSupportRebarsCorrectiveLength; }

	virtual BOOL GetSOGOpeningInfoSet() { return m_bSOGOpeningInfoSet; }
	
	virtual BOOL GetSOGRebarAreaInfoSet() { return m_bSOGRebarAreaInfoSet; }
	virtual long GetSOGRebarTolDist() { return m_nSOGRebarTolDist; }
	virtual long GetSOGRebarTolLenDif() { return m_nSOGRebarTolLenDif; }

	virtual BOOL GetSOGSpliceDevConditionSet() { return m_bSOGSpliceDevConditionSet; }
	virtual long GetSOGSpliceDevConditionLength() { return m_nSOGSpliceDevConditionLength; }

	virtual BOOL GetSOGSpliceAddLengthSet() { return m_bSOGSpliceAddLengthSet; }
	virtual long GetSOGSpliceAddLengthNumSplice() { return m_nSOGSpliceAddLengthNumSplice; }

	virtual BOOL GetSOGTopSpacerInfoSet() { return m_bSOGTopSpacerInfoSet; }
	virtual BOOL GetSOGTopSpacerSet() { return m_bSOGTopSpacerSet; }
	virtual long GetSOGTopSpacerDiaType() { return m_nSOGTopSpacerDiaType; }
	virtual long GetSOGTopSpacerSlabThick() { return m_nSOGTopSpacerSlabThick; }
	virtual long GetSOGTopSpacerSlabThickOverDiaIndex() { return m_nSOGTopSpacerSlabThickOverDiaIndex; }
	virtual long GetSOGTopSpacerSlabThickUnderDiaIndex() { return m_nSOGTopSpacerSlabThickUnderDiaIndex; }
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

	virtual BOOL GetSOGSpacerSupportBarsSet() { return m_bSOGSpacerSupportBarsSet; }
	virtual BOOL GetSOGSpacerSupportBarsConditionSet() { return m_bSOGSpacerSupportBarsConditionSet; }
	virtual long GetSOGSpacerSupportBarsCondition() { return m_nSOGSpacerSupportBarsCondition; }
	virtual long GetSOGSpacerSupportBarsDiaType() { return m_nSOGSpacerSupportBarsDiaType; }
	virtual long GetSOGSpacerSupportBarsUserDiaIndex() { return m_nSOGSpacerSupportBarsUserDiaIndex; }
	virtual long GetSOGSpacerSupportBarsNumType() { return m_nSOGSpacerSupportBarsNumType; }
	virtual double GetSOGSpacerSupportBarsTopSpacerNums() { return m_dSOGSpacerSupportBarsTopSpacerNums; }
	virtual long GetSOGSpacerSupportBarsUserNums() { return m_nSOGSpacerSupportBarsUserNums; }
	virtual long GetSOGSpacerSupportBarsLengthType() { return m_nSOGSpacerSupportBarsLengthType; }
	virtual double GetSOGSpacerSupportBarsUserRatio() { return m_dSOGSpacerSupportBarsUserRatio; }
	virtual long GetSOGSpacerSupportBarsUserLength() { return m_nSOGSpacerSupportBarsUserLength; }

	virtual long GetSOGCZoneSpliceWay() { return m_nSOGCZoneSpliceWay; }
	virtual long GetSOGCZoneSpliceLength() { return m_nSOGCZoneSpliceLength; }

	virtual void SetSOGMainBarInfoSet(BOOL value) { m_bSOGMainBarInfoSet = value; }
	virtual void SetSOGMaxDualHookLength(long value) { m_nSOGMaxDualHookLength = value; }
	virtual void SetSOGSpliceWay(long value) { m_nSOGSpliceWay = value; }
	virtual void SetSOGSpliceType(long value) { m_nSOGSpliceType = value; }
		
	virtual void SetSOGEndInfoSet(BOOL value) { m_bSOGEndInfoSet = value; }
	virtual void SetSOGTopEndHookType(long value) { m_nSOGTopEndHookType = value; }
	virtual void SetSOGBotEndHookType(long value) { m_nSOGBotEndHookType = value; }
	virtual void SetSOGIsoFootDevType(long value) { m_nSOGIsoFootDevType = value; }
	virtual void SetSOGSpliceDevSlabType(long value) { m_nSOGSpliceDevSlabType = value; }

	virtual void SetSOGTopEndDevTypeUserLength(long value) { m_nSOGTopEndDevTypeUserLength = value; }
	virtual void SetSOGBotEndDevTypeUserLength(long value) { m_nSOGBotEndDevTypeUserLength = value; }
	virtual void SetSOGOpeningTopDevType(long value) { m_nSOGOpeningTopDevType = value; }
	virtual void SetSOGOpeningBotDevType(long value) { m_nSOGOpeningBotDevType = value; }

	virtual void SetSOGLevelDifInfoSet(BOOL value) { m_bSOGLevelDifInfoSet = value; }
	virtual void SetSOGLevelDifPlaceShape(long value) { m_nSOGLevelDifPlaceShape = value; }
	virtual void SetSOGLevelDifHaunchPlaceType(long value) { m_nSOGLevelDifHaunchPlaceType = value; }

	virtual void SetSOGSupportRebarsSet(BOOL value) { m_bSOGSupportRebarsSet = value; }
	virtual void SetSOGSupportRebarsDiaIndex(long value) { m_nSOGSupportRebarsDiaIndex = value; }
	virtual void SetSOGSupportRebarsLengthType(long value) { m_nSOGSupportRebarsLengthType = value; }
	virtual void SetSOGSupportRebarsRWallLengthRate(double value) { m_dSOGSupportRebarsRWallLengthRate = value; }
	virtual void SetSOGSupportRebarsSlabLengthRate(double value) { m_dSOGSupportRebarsSlabLengthRate = value; }
	virtual void SetSOGSupportRebarsOrderLength(long value) { m_nSOGSupportRebarsOrderLength = value; }
	virtual void SetSOGSupportRebarsSpliceLength(long value) { m_nSOGSupportRebarsSpliceLength = value; }
	virtual void SetSOGSupportRebarsCorrectiveLength(long value) { m_nSOGSupportRebarsCorrectiveLength = value; }

	virtual void SetSOGOpeningInfoSet(BOOL value) { m_bSOGOpeningInfoSet = value; }
	
	virtual void SetSOGRebarAreaInfoSet(BOOL value) { m_bSOGRebarAreaInfoSet = value; }
	virtual void SetSOGRebarTolDist(long value) { m_nSOGRebarTolDist = value; }
	virtual void SetSOGRebarTolLenDif(long value) { m_nSOGRebarTolLenDif = value; }
	
	virtual void SetSOGSpliceDevConditionSet(BOOL value) { m_bSOGSpliceDevConditionSet = value; }
	virtual void SetSOGSpliceDevConditionLength(long value) { m_nSOGSpliceDevConditionLength = value; }

	virtual void SetSOGSpliceAddLengthSet(BOOL value) { m_bSOGSpliceAddLengthSet = value; }
	virtual void SetSOGSpliceAddLengthNumSplice(long value) { m_nSOGSpliceAddLengthNumSplice = value; }

	virtual void SetSOGTopSpacerInfoSet(BOOL value) { m_bSOGTopSpacerInfoSet = value; }
	virtual void SetSOGTopSpacerSet(BOOL value) { m_bSOGTopSpacerSet = value; }
	virtual void SetSOGTopSpacerDiaType(long value) { m_nSOGTopSpacerDiaType = value; }
	virtual void SetSOGTopSpacerSlabThick(long value) { m_nSOGTopSpacerSlabThick = value; }
	virtual void SetSOGTopSpacerSlabThickOverDiaIndex(long value) { m_nSOGTopSpacerSlabThickOverDiaIndex = value; }
	virtual void SetSOGTopSpacerSlabThickUnderDiaIndex(long value) { m_nSOGTopSpacerSlabThickUnderDiaIndex = value; }
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

	virtual void SetSOGSpacerSupportBarsSet(BOOL value) { m_bSOGSpacerSupportBarsSet = value; }
	virtual void SetSOGSpacerSupportBarsConditionSet(BOOL value) { m_bSOGSpacerSupportBarsConditionSet = value; }
	virtual void SetSOGSpacerSupportBarsCondition(long value) { m_nSOGSpacerSupportBarsCondition = value; }
	virtual void SetSOGSpacerSupportBarsDiaType(long value) { m_nSOGSpacerSupportBarsDiaType = value; }
	virtual void SetSOGSpacerSupportBarsUserDiaIndex(long value) { m_nSOGSpacerSupportBarsUserDiaIndex = value; }
	virtual void SetSOGSpacerSupportBarsNumType(long value) { m_nSOGSpacerSupportBarsNumType = value; }
	virtual void SetSOGSpacerSupportBarsTopSpacerNums(double value) { m_dSOGSpacerSupportBarsTopSpacerNums = value; }
	virtual void SetSOGSpacerSupportBarsUserNums(long value) { m_nSOGSpacerSupportBarsUserNums = value; }
	virtual void SetSOGSpacerSupportBarsLengthType(long value) { m_nSOGSpacerSupportBarsLengthType = value; }
	virtual void SetSOGSpacerSupportBarsUserRatio(double value) { m_dSOGSpacerSupportBarsUserRatio = value; }
	virtual void SetSOGSpacerSupportBarsUserLength(long value) { m_nSOGSpacerSupportBarsUserLength = value; }

	virtual void SetSOGCZoneSpliceWay(long value) { m_nSOGCZoneSpliceWay = value; }
	virtual void SetSOGCZoneSpliceLength(long value) { m_nSOGCZoneSpliceLength = value; }

	// CircularRamp
	virtual BOOL GetCircularRampMainBarInfoSet() { return m_bCircularRampMainBarInfoSet; }
	virtual long GetCircularRampMaxDualHookLength() { return m_nCircularRampMaxDualHookLength; }
	virtual long GetCircularRampSpliceWay() { return m_nCircularRampSpliceWay; }
	virtual long GetCircularRampSpliceType() { return m_nCircularRampSpliceType; }
		
	virtual BOOL GetCircularRampEndInfoSet() { return m_bCircularRampEndInfoSet; }
	virtual long GetCircularRampTopEndHookType() { return m_nCircularRampTopEndHookType; }
	virtual long GetCircularRampBotEndHookType() { return m_nCircularRampBotEndHookType; }
	virtual long GetCircularRampIsoFootDevType() { return m_nCircularRampIsoFootDevType; }
	virtual long GetCircularRampSpliceDevSlabType() { return m_nCircularRampSpliceDevSlabType; }

	virtual long GetCircularRampTopEndDevTypeUserLength() { return m_nCircularRampTopEndDevTypeUserLength; }
	virtual long GetCircularRampBotEndDevTypeUserLength() { return m_nCircularRampBotEndDevTypeUserLength; }
	virtual long GetCircularRampOpeningTopDevType() { return m_nCircularRampOpeningTopDevType; }
	virtual long GetCircularRampOpeningBotDevType() { return m_nCircularRampOpeningBotDevType; }

	virtual BOOL GetCircularRampLevelDifInfoSet() { return m_bCircularRampLevelDifInfoSet; }
	virtual long GetCircularRampLevelDifPlaceShape() { return m_nCircularRampLevelDifPlaceShape; }
	virtual long GetCircularRampLevelDifHaunchPlaceType() { return m_nCircularRampLevelDifHaunchPlaceType; }
	
	virtual BOOL GetCircularRampOpeningInfoSet() { return m_bCircularRampOpeningInfoSet; }
	
	virtual BOOL GetCircularRampRebarAreaInfoSet() { return m_bCircularRampRebarAreaInfoSet; }
	virtual long GetCircularRampRebarTolDist() { return m_nCircularRampRebarTolDist; }
	virtual long GetCircularRampRebarTolLenDif() { return m_nCircularRampRebarTolLenDif; }

	virtual BOOL GetCircularRampSpliceDevConditionSet() { return m_bCircularRampSpliceDevConditionSet; }
	virtual long GetCircularRampSpliceDevConditionLength() { return m_nCircularRampSpliceDevConditionLength; }

	virtual BOOL GetCircularRampSpliceAddLengthSet() { return m_bCircularRampSpliceAddLengthSet; }
	virtual long GetCircularRampSpliceAddLengthNumSplice() { return m_nCircularRampSpliceAddLengthNumSplice; }

	virtual BOOL GetCircularRampTopSpacerInfoSet() { return m_bCircularRampTopSpacerInfoSet; }
	virtual BOOL GetCircularRampTopSpacerSet() { return m_bCircularRampTopSpacerSet; }
	virtual long GetCircularRampTopSpacerDiaType() { return m_nCircularRampTopSpacerDiaType; }
	virtual long GetCircularRampTopSpacerSlabThick() { return m_nCircularRampTopSpacerSlabThick; }
	virtual long GetCircularRampTopSpacerSlabThickOverDiaIndex() { return m_nCircularRampTopSpacerSlabThickOverDiaIndex; }
	virtual long GetCircularRampTopSpacerSlabThickUnderDiaIndex() { return m_nCircularRampTopSpacerSlabThickUnderDiaIndex; }
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

	virtual BOOL GetCircularRampSpacerSupportBarsSet() { return m_bCircularRampSpacerSupportBarsSet; }
	virtual BOOL GetCircularRampSpacerSupportBarsConditionSet() { return m_bCircularRampSpacerSupportBarsConditionSet; }
	virtual long GetCircularRampSpacerSupportBarsCondition() { return m_nCircularRampSpacerSupportBarsCondition; }
	virtual long GetCircularRampSpacerSupportBarsDiaType() { return m_nCircularRampSpacerSupportBarsDiaType; }
	virtual long GetCircularRampSpacerSupportBarsUserDiaIndex() { return m_nCircularRampSpacerSupportBarsUserDiaIndex; }
	virtual long GetCircularRampSpacerSupportBarsNumType() { return m_nCircularRampSpacerSupportBarsNumType; }
	virtual double GetCircularRampSpacerSupportBarsTopSpacerNums() { return m_dCircularRampSpacerSupportBarsTopSpacerNums; }
	virtual long GetCircularRampSpacerSupportBarsUserNums() { return m_nCircularRampSpacerSupportBarsUserNums; }
	virtual long GetCircularRampSpacerSupportBarsLengthType() { return m_nCircularRampSpacerSupportBarsLengthType; }
	virtual double GetCircularRampSpacerSupportBarsUserRatio() { return m_dCircularRampSpacerSupportBarsUserRatio; }
	virtual long GetCircularRampSpacerSupportBarsUserLength() { return m_nCircularRampSpacerSupportBarsUserLength; }

	virtual long GetCircularRampCZoneSpliceWay() { return m_nCircularRampCZoneSpliceWay; }
	virtual long GetCircularRampCZoneSpliceLength() { return m_nCircularRampCZoneSpliceLength; }

	virtual void SetCircularRampMainBarInfoSet(BOOL value) { m_bCircularRampMainBarInfoSet = value; }
	virtual void SetCircularRampMaxDualHookLength(long value) { m_nCircularRampMaxDualHookLength = value; }
	virtual void SetCircularRampSpliceWay(long value) { m_nCircularRampSpliceWay = value; }
	virtual void SetCircularRampSpliceType(long value) { m_nCircularRampSpliceType = value; }
		
	virtual void SetCircularRampEndInfoSet(BOOL value) { m_bCircularRampEndInfoSet = value; }
	virtual void SetCircularRampTopEndHookType(long value) { m_nCircularRampTopEndHookType = value; }
	virtual void SetCircularRampBotEndHookType(long value) { m_nCircularRampBotEndHookType = value; }
	virtual void SetCircularRampIsoFootDevType(long value) { m_nCircularRampIsoFootDevType = value; }
	virtual void SetCircularRampSpliceDevSlabType(long value) { m_nCircularRampSpliceDevSlabType = value; }

	virtual void SetCircularRampTopEndDevTypeUserLength(long value) { m_nCircularRampTopEndDevTypeUserLength = value; }
	virtual void SetCircularRampBotEndDevTypeUserLength(long value) { m_nCircularRampBotEndDevTypeUserLength = value; }
	virtual void SetCircularRampOpeningTopDevType(long value) { m_nCircularRampOpeningTopDevType = value; }
	virtual void SetCircularRampOpeningBotDevType(long value) { m_nCircularRampOpeningBotDevType = value; }

	virtual void SetCircularRampLevelDifInfoSet(BOOL value) { m_bCircularRampLevelDifInfoSet = value; }
	virtual void SetCircularRampLevelDifPlaceShape(long value) { m_nCircularRampLevelDifPlaceShape = value; }
	virtual void SetCircularRampLevelDifHaunchPlaceType(long value) { m_nCircularRampLevelDifHaunchPlaceType = value; }
		
	virtual void SetCircularRampOpeningInfoSet(BOOL value) { m_bCircularRampOpeningInfoSet = value; }
	
	virtual void SetCircularRampRebarAreaInfoSet(BOOL value) { m_bCircularRampRebarAreaInfoSet = value; }
	virtual void SetCircularRampRebarTolDist(long value) { m_nCircularRampRebarTolDist = value; }
	virtual void SetCircularRampRebarTolLenDif(long value) { m_nCircularRampRebarTolLenDif = value; }

	virtual void SetCircularRampSpliceDevConditionSet(BOOL value) { m_bCircularRampSpliceDevConditionSet = value; }
	virtual void SetCircularRampSpliceDevConditionLength(long value) { m_nCircularRampSpliceDevConditionLength = value; }

	virtual void SetCircularRampSpliceAddLengthSet(BOOL value) { m_bCircularRampSpliceAddLengthSet = value; }
	virtual void SetCircularRampSpliceAddLengthNumSplice(long value) { m_nCircularRampSpliceAddLengthNumSplice = value; }

	virtual void SetCircularRampTopSpacerInfoSet(BOOL value) { m_bCircularRampTopSpacerInfoSet = value; }
	virtual void SetCircularRampTopSpacerSet(BOOL value) { m_bCircularRampTopSpacerSet = value; }
	virtual void SetCircularRampTopSpacerDiaType(long value) { m_nCircularRampTopSpacerDiaType = value; }
	virtual void SetCircularRampTopSpacerSlabThick(long value) { m_nCircularRampTopSpacerSlabThick = value; }
	virtual void SetCircularRampTopSpacerSlabThickOverDiaIndex(long value) { m_nCircularRampTopSpacerSlabThickOverDiaIndex = value; }
	virtual void SetCircularRampTopSpacerSlabThickUnderDiaIndex(long value) { m_nCircularRampTopSpacerSlabThickUnderDiaIndex = value; }
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

	virtual void SetCircularRampSpacerSupportBarsSet(BOOL value) { m_bCircularRampSpacerSupportBarsSet = value; }
	virtual void SetCircularRampSpacerSupportBarsConditionSet(BOOL value) { m_bCircularRampSpacerSupportBarsConditionSet = value; }
	virtual void SetCircularRampSpacerSupportBarsCondition(long value) { m_nCircularRampSpacerSupportBarsCondition = value; }
	virtual void SetCircularRampSpacerSupportBarsDiaType(long value) { m_nCircularRampSpacerSupportBarsDiaType = value; }
	virtual void SetCircularRampSpacerSupportBarsUserDiaIndex(long value) { m_nCircularRampSpacerSupportBarsUserDiaIndex = value; }
	virtual void SetCircularRampSpacerSupportBarsNumType(long value) { m_nCircularRampSpacerSupportBarsNumType = value; }
	virtual void SetCircularRampSpacerSupportBarsTopSpacerNums(double value) { m_dCircularRampSpacerSupportBarsTopSpacerNums = value; }
	virtual void SetCircularRampSpacerSupportBarsUserNums(long value) { m_nCircularRampSpacerSupportBarsUserNums = value; }
	virtual void SetCircularRampSpacerSupportBarsLengthType(long value) { m_nCircularRampSpacerSupportBarsLengthType = value; }
	virtual void SetCircularRampSpacerSupportBarsUserRatio(double value) { m_dCircularRampSpacerSupportBarsUserRatio = value; }
	virtual void SetCircularRampSpacerSupportBarsUserLength(long value) { m_nCircularRampSpacerSupportBarsUserLength = value; }

	virtual void SetCircularRampCZoneSpliceWay(long value) { m_nCircularRampCZoneSpliceWay = value; }
	virtual void SetCircularRampCZoneSpliceLength(long value) { m_nCircularRampCZoneSpliceLength = value; }
	// PC Slab
	virtual BOOL GetPCSlabMainBarInfoSet() { return m_bPCSlabMainBarInfoSet; }
	virtual long GetPCSlabMaxDualHookLength() { return m_nPCSlabMaxDualHookLength; }
	virtual long GetPCSlabSpliceWay() { return m_nPCSlabSpliceWay; }
	virtual long GetPCSlabSpliceType() { return m_nPCSlabSpliceType; }
	
	virtual long GetPCSlabTopEndAddBarLxLnxType() { return  m_nPCSlabTopEndAddBarLxLnxType; }
	virtual long GetPCSlabTopEndAddBarLengthType() { return  m_nPCSlabTopEndAddBarLengthType; }
	virtual long GetPCSlabTopCenAddBarLxLnxType() { return  m_nPCSlabTopCenAddBarLxLnxType; }
	virtual long GetPCSlabTopCenAddBarLengthType() { return  m_nPCSlabTopCenAddBarLengthType; }

	virtual long GetPCSlabTopEndDevTypeUserLength() { return m_nPCSlabTopEndDevTypeUserLength; }
	virtual long GetPCSlabBotEndDevTypeUserLength() { return m_nPCSlabBotEndDevTypeUserLength; }

	virtual BOOL GetPCSlabShortCenSpliceBarPlace() { return  m_bPCSlabShortCenSpliceBarPlace; }
	virtual long GetPCSlabShortCenSpliceBarLengthType() { return  m_nPCSlabShortCenSpliceBarLengthType; }
	virtual BOOL GetPCSlabShortEndSpliceBarPlace() { return  m_bPCSlabShortEndSpliceBarPlace; }
	virtual long GetPCSlabShortEndSpliceBarLengthType() { return  m_nPCSlabShortEndSpliceBarLengthType; }
	virtual BOOL GetPCSlabLongCenSpliceBarPlace() { return  m_bPCSlabLongCenSpliceBarPlace; }
	virtual long GetPCSlabLongCenSpliceBarLengthType() { return  m_nPCSlabLongCenSpliceBarLengthType; }
	virtual BOOL GetPCSlabLongEndSpliceBarPlace() { return  m_bPCSlabLongEndSpliceBarPlace; }
	virtual long GetPCSlabLongEndSpliceBarLengthType() { return  m_nPCSlabLongEndSpliceBarLengthType; }

	virtual BOOL GetPCSlabUnStructuredEndInfoSet() { return m_bPCSlabUnStructuredEndInfoSet; }
	virtual long GetPCSlabTopUnStructuredEndHookType() { return m_nPCSlabTopUnStructuredEndHookType; }
	virtual long GetPCSlabBotUnStructuredEndHookType() { return m_nPCSlabBotUnStructuredEndHookType; }
	
	virtual BOOL GetPCSlabLevelDifInfoSet() { return m_bPCSlabLevelDifInfoSet; }
	virtual long GetPCSlabLevelDifPlaceShape() { return m_nPCSlabLevelDifPlaceShape; }
	virtual long GetPCSlabLevelDifHaunchPlaceType() { return m_nPCSlabLevelDifHaunchPlaceType; }

	virtual BOOL GetPCSlabOpeningInfoSet() { return m_bPCSlabOpeningInfoSet; }
	virtual long GetPCSlabOpeningTopDevType() { return m_nPCSlabOpeningTopDevType; }
	virtual long GetPCSlabOpeningBotDevType() { return m_nPCSlabOpeningBotDevType; }

	virtual BOOL GetPCSlabRebarAreaInfoSet() { return m_bPCSlabRebarAreaInfoSet; }
	virtual long GetPCSlabRebarTolDist() { return m_nPCSlabRebarTolDist; }
	virtual long GetPCSlabRebarTolLenDif() { return m_nPCSlabRebarTolLenDif; }

	virtual BOOL GetPCSlabSpliceDevConditionSet() { return m_bPCSlabSpliceDevConditionSet; }
	virtual long GetPCSlabSpliceDevConditionLength() { return m_nPCSlabSpliceDevConditionLength; }

	virtual BOOL GetPCSlabSpliceAddLengthSet() { return m_bPCSlabSpliceAddLengthSet; }
	virtual long GetPCSlabSpliceAddLengthNumSplice() { return m_nPCSlabSpliceAddLengthNumSplice; }

	virtual void SetPCSlabMainBarInfoSet(BOOL value) { m_bPCSlabMainBarInfoSet = value; }
	virtual void SetPCSlabMaxDualHookLength(long value) { m_nPCSlabMaxDualHookLength = value; }
	virtual void SetPCSlabSpliceWay(long value) { m_nPCSlabSpliceWay = value; }
	virtual void SetPCSlabSpliceType(long value) { m_nPCSlabSpliceType = value; }
		
	virtual void SetPCSlabTopEndAddBarLxLnxType(long value) { m_nPCSlabTopEndAddBarLxLnxType = value; }
	virtual void SetPCSlabTopEndAddBarLengthType(long value) { m_nPCSlabTopEndAddBarLengthType = value; }
	virtual void SetPCSlabTopCenAddBarLxLnxType(long value) { m_nPCSlabTopCenAddBarLxLnxType = value; }
	virtual void SetPCSlabTopCenAddBarLengthType(long value) { m_nPCSlabTopCenAddBarLengthType = value; }

	virtual void SetPCSlabTopEndDevTypeUserLength(long value) { m_nPCSlabTopEndDevTypeUserLength = value; }
	virtual void SetPCSlabBotEndDevTypeUserLength(long value) { m_nPCSlabBotEndDevTypeUserLength = value; }

	virtual void SetPCSlabShortCenSpliceBarPlace(BOOL value) { m_bPCSlabShortCenSpliceBarPlace = value; }
	virtual void SetPCSlabShortCenSpliceBarLengthType(long value) { m_nPCSlabShortCenSpliceBarLengthType = value; }
	virtual void SetPCSlabShortEndSpliceBarPlace(BOOL value) { m_bPCSlabShortEndSpliceBarPlace = value; }
	virtual void SetPCSlabShortEndSpliceBarLengthType(long value) { m_nPCSlabShortEndSpliceBarLengthType = value; }
	virtual void SetPCSlabLongCenSpliceBarPlace(BOOL value) { m_bPCSlabLongCenSpliceBarPlace = value; }
	virtual void SetPCSlabLongCenSpliceBarLengthType(long value) { m_nPCSlabLongCenSpliceBarLengthType = value; }
	virtual void SetPCSlabLongEndSpliceBarPlace(BOOL value) { m_bPCSlabLongEndSpliceBarPlace = value; }
	virtual void SetPCSlabLongEndSpliceBarLengthType(long value) { m_nPCSlabLongEndSpliceBarLengthType = value; }
		
	virtual void SetPCSlabUnStructuredEndInfoSet(BOOL value) { m_bPCSlabUnStructuredEndInfoSet = value; }
	virtual void SetPCSlabTopUnStructuredEndHookType(long value) { m_nPCSlabTopUnStructuredEndHookType = value; }
	virtual void SetPCSlabBotUnStructuredEndHookType(long value) { m_nPCSlabBotUnStructuredEndHookType = value; }
	
	virtual void SetPCSlabLevelDifInfoSet(BOOL value) { m_bPCSlabLevelDifInfoSet = value; }
	virtual void SetPCSlabLevelDifPlaceShape(long value) { m_nPCSlabLevelDifPlaceShape = value; }
	virtual void SetPCSlabLevelDifHaunchPlaceType(long value) { m_nPCSlabLevelDifHaunchPlaceType = value; }

	virtual void SetPCSlabOpeningInfoSet(BOOL value) { m_bPCSlabOpeningInfoSet = value; }
	virtual void SetPCSlabOpeningTopDevType(long value) { m_nPCSlabOpeningTopDevType = value; }
	virtual void SetPCSlabOpeningBotDevType(long value) { m_nPCSlabOpeningBotDevType = value; }

	virtual void SetPCSlabRebarAreaInfoSet(BOOL value) { m_bPCSlabRebarAreaInfoSet = value; }
	virtual void SetPCSlabRebarTolDist(long value) { m_nPCSlabRebarTolDist = value; }
	virtual void SetPCSlabRebarTolLenDif(long value) { m_nPCSlabRebarTolLenDif = value; }

	virtual void SetPCSlabSpliceDevConditionSet(BOOL value) { m_bPCSlabSpliceDevConditionSet = value; }
	virtual void SetPCSlabSpliceDevConditionLength(long value) { m_nPCSlabSpliceDevConditionLength = value; }

	virtual void SetPCSlabSpliceAddLengthSet(BOOL value) { m_bPCSlabSpliceAddLengthSet = value; }
	virtual void SetPCSlabSpliceAddLengthNumSplice(long value) { m_nPCSlabSpliceAddLengthNumSplice = value; }

	virtual map<long, long> GetPCSlabTopEndAddBarUserLenMap() { return mm_PCSlabTopEndAddBarUserLen; }
	virtual void SetPCSlabTopEndAddBarUserLenMap(map<long, long> mapUserLen);
	virtual long GetPCSlabTopEndAddBarUserLen(double dDia);
	virtual void SetPCSlabTopEndAddBarUserLen(long key, long value) { mm_PCSlabTopEndAddBarUserLen[key] = value; }

	virtual map<long, long> GetPCSlabTopCenAddBarUserLenMap() { return mm_PCSlabTopCenAddBarUserLen; }
	virtual void SetPCSlabTopCenAddBarUserLenMap(map<long, long> mapUserLen);
	virtual long GetPCSlabTopCenAddBarUserLen(double dDia);
	virtual void SetPCSlabTopCenAddBarUserLen(long key, long value) { mm_PCSlabTopCenAddBarUserLen[key] = value; }

	virtual map<long, long> GetPCSlabShortCenSpliceBarUserLenMap() { return mm_PCSlabShortCenSpliceBarUserLen; }
	virtual void SetPCSlabShortCenSpliceBarUserLenMap(map<long, long> mapUserLen);
	virtual long GetPCSlabShortCenSpliceBarUserLen(double dDia);
	virtual void SetPCSlabShortCenSpliceBarUserLen(long key, long value) { mm_PCSlabShortCenSpliceBarUserLen[key] = value; }

	virtual map<long, long> GetPCSlabShortEndSpliceBarUserLenMap() { return mm_PCSlabShortEndSpliceBarUserLen; }
	virtual void SetPCSlabShortEndSpliceBarUserLenMap(map<long, long> mapUserLen);
	virtual long GetPCSlabShortEndSpliceBarUserLen(double dDia);
	virtual void SetPCSlabShortEndSpliceBarUserLen(long key, long value) { mm_PCSlabShortEndSpliceBarUserLen[key] = value; }

	virtual map<long, long> GetPCSlabLongCenSpliceBarUserLenMap() { return mm_PCSlabLongCenSpliceBarUserLen; }
	virtual void SetPCSlabLongCenSpliceBarUserLenMap(map<long, long> mapUserLen);
	virtual long GetPCSlabLongCenSpliceBarUserLen(double dDia);
	virtual void SetPCSlabLongCenSpliceBarUserLen(long key, long value) { mm_PCSlabLongCenSpliceBarUserLen[key] = value; }

	virtual map<long, long> GetPCSlabLongEndSpliceBarUserLenMap() { return mm_PCSlabLongEndSpliceBarUserLen; }
	virtual void SetPCSlabLongEndSpliceBarUserLenMap(map<long, long> mapUserLen);
	virtual long GetPCSlabLongEndSpliceBarUserLen(double dDia);
	virtual void SetPCSlabLongEndSpliceBarUserLen(long key, long value) { mm_PCSlabLongEndSpliceBarUserLen[key] = value; }


	// 슬래브 보강근
	virtual BOOL GetSlabRebarRebarAreaInfoSet() { return m_bSlabRebarRebarAreaInfoSet; }
	virtual long GetSlabRebarRebarTolDist() { return m_nSlabRebarRebarTolDist; }
	virtual long GetSlabRebarRebarTolLenDif() { return m_nSlabRebarRebarTolLenDif; }

	virtual void SetSlabRebarRebarAreaInfoSet(BOOL value) { m_bSlabRebarRebarAreaInfoSet = value; }
	virtual void SetSlabRebarRebarTolDist(long value) { m_nSlabRebarRebarTolDist = value; }
	virtual void SetSlabRebarRebarTolLenDif(long value) { m_nSlabRebarRebarTolLenDif = value; }

	// 철근 일체형 데크 슬래브
	virtual BOOL GetEmbeddedDeckMainBarInfoSet() { return m_bEmbeddedDeckMainBarInfoSet; }
	virtual long GetEmbeddedDeckMaxDualHookLength() { return m_nEmbeddedDeckMaxDualHookLength; }
	virtual long GetEmbeddedDeckSpliceWay() { return m_nEmbeddedDeckSpliceWay; }
	virtual long GetEmbeddedDeckSpliceType() { return m_nEmbeddedDeckSpliceType; }
				
	virtual long GetEmbeddedDeckTopEndAddBarLxLnxType() { return m_nEmbeddedDeckTopEndAddBarLxLnxType; }
	virtual long GetEmbeddedDeckTopEndAddBarLengthType() { return m_nEmbeddedDeckTopEndAddBarLengthType; }
	virtual long GetEmbeddedDeckTopCenAddBarLxLnxType() { return m_nEmbeddedDeckTopCenAddBarLxLnxType; }
	virtual long GetEmbeddedDeckTopCenAddBarLengthType() { return m_nEmbeddedDeckTopCenAddBarLengthType; }

	virtual BOOL GetEmbeddedDeckSpliceBarPlace() { return m_bEmbeddedDeckSpliceBarPlace; }
	virtual long GetEmbeddedDeckSpliceBarLengthType() { return m_nEmbeddedDeckSpliceBarLengthType; }

	virtual long GetEmbeddedDeckTopEndDevTypeUserLength() { return m_nEmbeddedDeckTopEndDevTypeUserLength; }
	virtual long GetEmbeddedDeckBotEndDevTypeUserLength() { return m_nEmbeddedDeckBotEndDevTypeUserLength; }

	virtual BOOL GetEmbeddedDeckUnStructuredEndInfoSet() { return m_bEmbeddedDeckUnStructuredEndInfoSet; }
	virtual long GetEmbeddedDeckTopUnStructuredEndHookType() { return m_nEmbeddedDeckTopUnStructuredEndHookType; }
	virtual long GetEmbeddedDeckBotUnStructuredEndHookType() { return m_nEmbeddedDeckBotUnStructuredEndHookType; }

	virtual BOOL GetEmbeddedDeckLevelDifInfoSet() { return m_bEmbeddedDeckLevelDifInfoSet; }
	virtual long GetEmbeddedDeckLevelDifPlaceShape() { return m_nEmbeddedDeckLevelDifPlaceShape; }
	virtual long GetEmbeddedDeckLevelDifHaunchPlaceType() { return m_nEmbeddedDeckLevelDifHaunchPlaceType; }

	virtual BOOL GetEmbeddedDeckOpeningInfoSet() { return m_bEmbeddedDeckOpeningInfoSet; }
	virtual long GetEmbeddedDeckOpeningTopDevType() { return m_nEmbeddedDeckOpeningTopDevType; }
	virtual long GetEmbeddedDeckOpeningBotDevType() { return m_nEmbeddedDeckOpeningBotDevType; }

	virtual BOOL GetEmbeddedDeckRebarAreaInfoSet() { return m_bEmbeddedDeckRebarAreaInfoSet; }
	virtual long GetEmbeddedDeckRebarTolDist() { return m_nEmbeddedDeckRebarTolDist; }
	virtual long GetEmbeddedDeckRebarTolLenDif() { return m_nEmbeddedDeckRebarTolLenDif; }

	virtual BOOL GetEmbeddedDeckSpliceDevConditionSet() { return m_bEmbeddedDeckSpliceDevConditionSet; }
	virtual long GetEmbeddedDeckSpliceDevConditionLength() { return m_nEmbeddedDeckSpliceDevConditionLength; }

	virtual BOOL GetEmbeddedDeckSpliceAddLengthSet() {	return m_bEmbeddedDeckSpliceAddLengthSet;	}
	virtual long GetEmbeddedDeckSpliceAddLengthNumSplice() { return m_nEmbeddedDeckSpliceAddLengthNumSplice; }

	virtual void SetEmbeddedDeckMainBarInfoSet(BOOL value) { m_bEmbeddedDeckMainBarInfoSet = value; }
	virtual void SetEmbeddedDeckMaxDualHookLength(long value) { m_nEmbeddedDeckMaxDualHookLength = value; }
	virtual void SetEmbeddedDeckSpliceWay(long value) { m_nEmbeddedDeckSpliceWay = value; }
	virtual void SetEmbeddedDeckSpliceType(long value) { m_nEmbeddedDeckSpliceType = value; }
			
	virtual void SetEmbeddedDeckTopEndAddBarLxLnxType(long value) { m_nEmbeddedDeckTopEndAddBarLxLnxType = value; }
	virtual void SetEmbeddedDeckTopEndAddBarLengthType(long value) { m_nEmbeddedDeckTopEndAddBarLengthType = value; }
	virtual void SetEmbeddedDeckTopCenAddBarLxLnxType(long value) { m_nEmbeddedDeckTopCenAddBarLxLnxType = value; }
	virtual void SetEmbeddedDeckTopCenAddBarLengthType(long value) { m_nEmbeddedDeckTopCenAddBarLengthType = value; }

	virtual void SetEmbeddedDeckSpliceBarPlace(BOOL value) { m_bEmbeddedDeckSpliceBarPlace = value; }
	virtual void SetEmbeddedDeckSpliceBarLengthType(long value) { m_nEmbeddedDeckSpliceBarLengthType = value; }

	virtual void SetEmbeddedDeckTopEndDevTypeUserLength(long value) { m_nEmbeddedDeckTopEndDevTypeUserLength = value; }
	virtual void SetEmbeddedDeckBotEndDevTypeUserLength(long value) { m_nEmbeddedDeckBotEndDevTypeUserLength = value; }

	virtual void SetEmbeddedDeckUnStructuredEndInfoSet(BOOL value) { m_bEmbeddedDeckUnStructuredEndInfoSet = value; }
	virtual void SetEmbeddedDeckTopUnStructuredEndHookType(long value) { m_nEmbeddedDeckTopUnStructuredEndHookType = value; }
	virtual void SetEmbeddedDeckBotUnStructuredEndHookType(long value) { m_nEmbeddedDeckBotUnStructuredEndHookType = value; }

	virtual void SetEmbeddedDeckLevelDifInfoSet(BOOL value) { m_bEmbeddedDeckLevelDifInfoSet = value; }
	virtual void SetEmbeddedDeckLevelDifPlaceShape(long value) { m_nEmbeddedDeckLevelDifPlaceShape = value; }
	virtual void SetEmbeddedDeckLevelDifHaunchPlaceType(long value) { m_nEmbeddedDeckLevelDifHaunchPlaceType = value; }

	virtual void SetEmbeddedDeckOpeningInfoSet(BOOL value) { m_bEmbeddedDeckOpeningInfoSet = value; }
	virtual void SetEmbeddedDeckOpeningTopDevType(long value) { m_nEmbeddedDeckOpeningTopDevType = value; }
	virtual void SetEmbeddedDeckOpeningBotDevType(long value) { m_nEmbeddedDeckOpeningBotDevType = value; }

	virtual void SetEmbeddedDeckRebarAreaInfoSet(BOOL value) { m_bEmbeddedDeckRebarAreaInfoSet = value; }
	virtual void SetEmbeddedDeckRebarTolDist(long value) { m_nEmbeddedDeckRebarTolDist = value; }
	virtual void SetEmbeddedDeckRebarTolLenDif(long value) { m_nEmbeddedDeckRebarTolLenDif = value; }

	virtual void SetEmbeddedDeckSpliceDevConditionSet(BOOL value) { m_bEmbeddedDeckSpliceDevConditionSet = value; }
	virtual void SetEmbeddedDeckSpliceDevConditionLength(long value) { m_nEmbeddedDeckSpliceDevConditionLength = value; }

	virtual void SetEmbeddedDeckSpliceAddLengthSet(BOOL value) {	m_bEmbeddedDeckSpliceAddLengthSet = value; }
	virtual void SetEmbeddedDeckSpliceAddLengthNumSplice(long value) { m_nEmbeddedDeckSpliceAddLengthNumSplice = value; }

	virtual map<long, long> GetEmbeddedDeckTopEndAddBarUserLenMap() { return mm_EmbeddedDeckTopEndAddBarUserLen; }
	virtual void SetEmbeddedDeckTopEndAddBarUserLenMap(map<long, long> mapUserLen);
	virtual long GetEmbeddedDeckTopEndAddBarUserLen(double dDia);
	virtual void SetEmbeddedDeckTopEndAddBarUserLen(long key, long value) { mm_EmbeddedDeckTopEndAddBarUserLen[key] = value; }

	virtual map<long, long> GetEmbeddedDeckTopCenAddBarUserLenMap() { return mm_EmbeddedDeckTopCenAddBarUserLen; }
	virtual void SetEmbeddedDeckTopCenAddBarUserLenMap(map<long, long> mapUserLen);
	virtual long GetEmbeddedDeckTopCenAddBarUserLen(double dDia);
	virtual void SetEmbeddedDeckTopCenAddBarUserLen(long key, long value) { mm_EmbeddedDeckTopCenAddBarUserLen[key] = value; }

	virtual map<long, long> GetEmbeddedDeckSpliceBarUserLenMap() { return mm_EmbeddedDeckSpliceBarUserLen; }
	virtual void SetEmbeddedDeckSpliceBarUserLenMap(map<long, long> mapUserLen);
	virtual long GetEmbeddedDeckSpliceBarUserLen(double dDia);
	virtual void SetEmbeddedDeckSpliceBarUserLen(long key, long value) { mm_EmbeddedDeckSpliceBarUserLen[key] = value; }


	// 일반 데크 슬래브
	virtual BOOL GetGeneralDeckMainBarInfoSet() { return m_bGeneralDeckMainBarInfoSet; }
	virtual long GetGeneralDeckMaxDualHookLength() { return m_nGeneralDeckMaxDualHookLength; }
	virtual long GetGeneralDeckSpliceWay() { return m_nGeneralDeckSpliceWay; }
	virtual long GetGeneralDeckSpliceType() { return m_nGeneralDeckSpliceType; }
				
	virtual long GetGeneralDeckTopEndAddBarLxLnxType() { return m_nGeneralDeckTopEndAddBarLxLnxType; }
	virtual long GetGeneralDeckTopEndAddBarLengthType() { return m_nGeneralDeckTopEndAddBarLengthType; }
	virtual long GetGeneralDeckTopCenAddBarLxLnxType() { return m_nGeneralDeckTopCenAddBarLxLnxType; }
	virtual long GetGeneralDeckTopCenAddBarLengthType() { return m_nGeneralDeckTopCenAddBarLengthType; }

	virtual BOOL GetGeneralDeckSpliceBarPlace() { return m_bGeneralDeckSpliceBarPlace; }
	virtual long GetGeneralDeckSpliceBarLengthType() { return m_nGeneralDeckSpliceBarLengthType; }

	virtual long GetGeneralDeckTopEndDevTypeUserLength() { return m_nGeneralDeckTopEndDevTypeUserLength; }
	virtual long GetGeneralDeckBotEndDevTypeUserLength() { return m_nGeneralDeckBotEndDevTypeUserLength; }

	virtual BOOL GetGeneralDeckUnStructuredEndInfoSet() { return m_bGeneralDeckUnStructuredEndInfoSet; }
	virtual long GetGeneralDeckTopUnStructuredEndHookType() { return m_nGeneralDeckTopUnStructuredEndHookType; }
	virtual long GetGeneralDeckBotUnStructuredEndHookType() { return m_nGeneralDeckBotUnStructuredEndHookType; }

	virtual BOOL GetGeneralDeckLevelDifInfoSet() { return m_bGeneralDeckLevelDifInfoSet; }
	virtual long GetGeneralDeckLevelDifPlaceShape() { return m_nGeneralDeckLevelDifPlaceShape; }
	virtual long GetGeneralDeckLevelDifHaunchPlaceType() { return m_nGeneralDeckLevelDifHaunchPlaceType; }

	virtual BOOL GetGeneralDeckOpeningInfoSet() { return m_bGeneralDeckOpeningInfoSet; }
	virtual long GetGeneralDeckOpeningTopDevType() { return m_nGeneralDeckOpeningTopDevType; }
	virtual long GetGeneralDeckOpeningBotDevType() { return m_nGeneralDeckOpeningBotDevType; }

	virtual BOOL GetGeneralDeckRebarAreaInfoSet() { return m_bGeneralDeckRebarAreaInfoSet; }
	virtual long GetGeneralDeckRebarTolDist() { return m_nGeneralDeckRebarTolDist; }
	virtual long GetGeneralDeckRebarTolLenDif() { return m_nGeneralDeckRebarTolLenDif; }

	virtual BOOL GetGeneralDeckSpliceDevConditionSet() { return m_bGeneralDeckSpliceDevConditionSet; }
	virtual long GetGeneralDeckSpliceDevConditionLength() { return m_nGeneralDeckSpliceDevConditionLength; }

	virtual BOOL GetGeneralDeckSpliceAddLengthSet() {	return m_bGeneralDeckSpliceAddLengthSet;	}
	virtual long GetGeneralDeckSpliceAddLengthNumSplice() { return m_nGeneralDeckSpliceAddLengthNumSplice; }

	virtual void SetGeneralDeckMainBarInfoSet(BOOL value) { m_bGeneralDeckMainBarInfoSet = value; }
	virtual void SetGeneralDeckMaxDualHookLength(long value) { m_nGeneralDeckMaxDualHookLength = value; }
	virtual void SetGeneralDeckSpliceWay(long value) { m_nGeneralDeckSpliceWay = value; }
	virtual void SetGeneralDeckSpliceType(long value) { m_nGeneralDeckSpliceType = value; }
			
	virtual void SetGeneralDeckTopEndAddBarLxLnxType(long value) { m_nGeneralDeckTopEndAddBarLxLnxType = value; }
	virtual void SetGeneralDeckTopEndAddBarLengthType(long value) { m_nGeneralDeckTopEndAddBarLengthType = value; }
	virtual void SetGeneralDeckTopCenAddBarLxLnxType(long value) { m_nGeneralDeckTopCenAddBarLxLnxType = value; }
	virtual void SetGeneralDeckTopCenAddBarLengthType(long value) { m_nGeneralDeckTopCenAddBarLengthType = value; }

	virtual void SetGeneralDeckSpliceBarPlace(BOOL value) { m_bGeneralDeckSpliceBarPlace = value; }
	virtual void SetGeneralDeckSpliceBarLengthType(long value) { m_nGeneralDeckSpliceBarLengthType = value; }

	virtual void SetGeneralDeckTopEndDevTypeUserLength(long value) { m_nGeneralDeckTopEndDevTypeUserLength = value; }
	virtual void SetGeneralDeckBotEndDevTypeUserLength(long value) { m_nGeneralDeckBotEndDevTypeUserLength = value; }

	virtual void SetGeneralDeckUnStructuredEndInfoSet(BOOL value) { m_bGeneralDeckUnStructuredEndInfoSet = value; }
	virtual void SetGeneralDeckTopUnStructuredEndHookType(long value) { m_nGeneralDeckTopUnStructuredEndHookType = value; }
	virtual void SetGeneralDeckBotUnStructuredEndHookType(long value) { m_nGeneralDeckBotUnStructuredEndHookType = value; }

	virtual void SetGeneralDeckLevelDifInfoSet(BOOL value) { m_bGeneralDeckLevelDifInfoSet = value; }
	virtual void SetGeneralDeckLevelDifPlaceShape(long value) { m_nGeneralDeckLevelDifPlaceShape = value; }
	virtual void SetGeneralDeckLevelDifHaunchPlaceType(long value) { m_nGeneralDeckLevelDifHaunchPlaceType = value; }

	virtual void SetGeneralDeckOpeningInfoSet(BOOL value) { m_bGeneralDeckOpeningInfoSet = value; }
	virtual void SetGeneralDeckOpeningTopDevType(long value) { m_nGeneralDeckOpeningTopDevType = value; }
	virtual void SetGeneralDeckOpeningBotDevType(long value) { m_nGeneralDeckOpeningBotDevType = value; }

	virtual void SetGeneralDeckRebarAreaInfoSet(BOOL value) { m_bGeneralDeckRebarAreaInfoSet = value; }
	virtual void SetGeneralDeckRebarTolDist(long value) { m_nGeneralDeckRebarTolDist = value; }
	virtual void SetGeneralDeckRebarTolLenDif(long value) { m_nGeneralDeckRebarTolLenDif = value; }

	virtual void SetGeneralDeckSpliceDevConditionSet(BOOL value) { m_bGeneralDeckSpliceDevConditionSet = value; }
	virtual void SetGeneralDeckSpliceDevConditionLength(long value) { m_nGeneralDeckSpliceDevConditionLength = value; }

	virtual void SetGeneralDeckSpliceAddLengthSet(BOOL value) {	m_bGeneralDeckSpliceAddLengthSet = value; }
	virtual void SetGeneralDeckSpliceAddLengthNumSplice(long value) { m_nGeneralDeckSpliceAddLengthNumSplice = value; }

	virtual map<long, long> GetGeneralDeckTopEndAddBarUserLenMap() { return mm_GeneralDeckTopEndAddBarUserLen; }
	virtual void SetGeneralDeckTopEndAddBarUserLenMap(map<long, long> mapUserLen);
	virtual long GetGeneralDeckTopEndAddBarUserLen(double dDia);
	virtual void SetGeneralDeckTopEndAddBarUserLen(long key, long value) { mm_GeneralDeckTopEndAddBarUserLen[key] = value; }

	virtual map<long, long> GetGeneralDeckTopCenAddBarUserLenMap() { return mm_GeneralDeckTopCenAddBarUserLen; }
	virtual void SetGeneralDeckTopCenAddBarUserLenMap(map<long, long> mapUserLen);
	virtual long GetGeneralDeckTopCenAddBarUserLen(double dDia);
	virtual void SetGeneralDeckTopCenAddBarUserLen(long key, long value) { mm_GeneralDeckTopCenAddBarUserLen[key] = value; }

	virtual map<long, long> GetGeneralDeckSpliceBarUserLenMap() { return mm_GeneralDeckSpliceBarUserLen; }
	virtual void SetGeneralDeckSpliceBarUserLenMap(map<long, long> mapUserLen);
	virtual long GetGeneralDeckSpliceBarUserLen(double dDia);
	virtual void SetGeneralDeckSpliceBarUserLen(long key, long value) { mm_GeneralDeckSpliceBarUserLen[key] = value; }

	// PC IMT Slab
	virtual BOOL GetPCIMTSlabSpliceDevConditionSet() { return m_bPCIMTSlabSpliceDevConditionSet; }//
	virtual long GetPCIMTSlabSpliceDevConditionLength() { return m_nPCIMTSlabSpliceDevConditionLength; }//

	virtual BOOL GetPCIMTSlabSpliceAddLengthSet() { return m_bPCIMTSlabSpliceAddLengthSet; }//
	virtual long GetPCIMTSlabSpliceAddLengthNumSplice() { return m_nPCIMTSlabSpliceAddLengthNumSplice; };//

	virtual long GetPCIMTSlabMaxDualHookLength() { return m_nPCIMTSlabMaxDualHookLength; }//
	virtual long GetPCIMTSlabSpliceWay() { return m_nPCIMTSlabSpliceWay; }//
	virtual long GetPCIMTSlabSpliceType() { return m_nPCIMTSlabSpliceType; }//
	
	virtual long GetPCIMTSlabTopHookType() { return m_nPCIMTSlabTopHookType; }
	virtual long GetPCIMTSlabBotHookType() { return m_nPCIMTSlabBotHookType; }
	virtual long GetPCIMTSlabTopEndDevTypeUserLength() { return m_nPCIMTSlabTopEndDevTypeUserLength; }
	virtual long GetPCIMTSlabBotEndDevTypeUserLength() { return m_nPCIMTSlabBotEndDevTypeUserLength; }
	virtual long GetPCIMTSlabOpeningTopDevType() { return m_nPCIMTSlabOpeningTopDevType; }
	virtual long GetPCIMTSlabOpeningBotDevType() { return m_nPCIMTSlabOpeningBotDevType; }
	virtual long GetPCIMTSlabTopEndAddBarLxLnxType() { return m_nPCIMTSlabTopEndAddBarLxLnxType; }
	virtual long GetPCIMTSlabTopEndAddBarLengthType() { return m_nPCIMTSlabTopEndAddBarLengthType; }
	virtual long GetPCIMTSlabTopCenAddBarLxLnxType() { return m_nPCIMTSlabTopCenAddBarLxLnxType; }
	virtual long GetPCIMTSlabTopCenAddBarLengthType() { return m_nPCIMTSlabTopCenAddBarLengthType; }
	virtual BOOL GetPCIMTSlabShortSpliceBarPlace() { return m_bPCIMTSlabShortSpliceBarPlace; }
	virtual long GetPCIMTSlabShortSpliceBarLengthType() { return m_nPCIMTSlabShortSpliceBarLengthType; }
	virtual BOOL GetPCIMTSlabLongSpliceBarPlace() { return m_bPCIMTSlabLongSpliceBarPlace; }

	virtual long GetPCIMTSlabRebarTolDist() { return m_nPCIMTSlabRebarTolDist; }
	virtual long GetPCIMTSlabRebarTolLenDif() { return m_nPCIMTSlabRebarTolLenDif; }
	virtual long GetPCIMTSlabLongSpliceBarType() { return m_nPCIMTSlabLongSpliceBarType; }

	virtual void SetPCIMTSlabSpliceDevConditionSet(BOOL value) { m_bPCIMTSlabSpliceDevConditionSet = value; }
	virtual void SetPCIMTSlabSpliceDevConditionLength(long value) { m_nPCIMTSlabSpliceDevConditionLength = value; }

	virtual void SetPCIMTSlabSpliceAddLengthSet(BOOL value) { m_bPCIMTSlabSpliceAddLengthSet = value; }
	virtual void SetPCIMTSlabSpliceAddLengthNumSplice(long value) { m_nPCIMTSlabSpliceAddLengthNumSplice = value; }

	virtual void SetPCIMTSlabMaxDualHookLength(long value) { m_nPCIMTSlabMaxDualHookLength = value; }
	virtual void SetPCIMTSlabSpliceWay(long value) { m_nPCIMTSlabSpliceWay = value; }
	virtual void SetPCIMTSlabSpliceType(long value) { m_nPCIMTSlabSpliceType = value; }
	
	virtual void SetPCIMTSlabTopHookType(long value) { m_nPCIMTSlabTopHookType = value; }
	virtual void SetPCIMTSlabBotHookType(long value) { m_nPCIMTSlabBotHookType = value; }
	virtual void SetPCIMTSlabTopEndDevTypeUserLength(long value) { m_nPCIMTSlabTopEndDevTypeUserLength = value; }
	virtual void SetPCIMTSlabBotEndDevTypeUserLength(long value) { m_nPCIMTSlabBotEndDevTypeUserLength = value; }
	virtual void SetPCIMTSlabOpeningTopDevType(long value) { m_nPCIMTSlabOpeningTopDevType = value; }
	virtual void SetPCIMTSlabOpeningBotDevType(long value) { m_nPCIMTSlabOpeningBotDevType = value; }
	virtual void SetPCIMTSlabTopEndAddBarLxLnxType(long value) { m_nPCIMTSlabTopEndAddBarLxLnxType = value; }
	virtual void SetPCIMTSlabTopEndAddBarLengthType(long value) { m_nPCIMTSlabTopEndAddBarLengthType = value; }
	virtual void SetPCIMTSlabTopCenAddBarLxLnxType(long value) { m_nPCIMTSlabTopCenAddBarLxLnxType = value; }
	virtual void SetPCIMTSlabTopCenAddBarLengthType(long value) { m_nPCIMTSlabTopCenAddBarLengthType = value; }
	virtual void SetPCIMTSlabShortSpliceBarPlace(BOOL value) { m_bPCIMTSlabShortSpliceBarPlace = value; }
	virtual void SetPCIMTSlabShortSpliceBarLengthType(long value) { m_nPCIMTSlabShortSpliceBarLengthType = value; }
	virtual void SetPCIMTSlabLongSpliceBarPlace(BOOL value) { m_bPCIMTSlabLongSpliceBarPlace = value; }

	virtual void SetPCIMTSlabRebarTolDist(long value) { m_nPCIMTSlabRebarTolDist = value; }
	virtual void SetPCIMTSlabRebarTolLenDif(long value) { m_nPCIMTSlabRebarTolLenDif = value; }
	virtual void SetPCIMTSlabLongSpliceBarType(long value) { m_nPCIMTSlabLongSpliceBarType = value; }

			
	//Function
	virtual map<long, long> GetPCIMTSlabTopEndAddBarUserLenMap() { return mm_PCIMTSlabTopEndAddBarUserLen; }
	virtual map<long, long> GetPCIMTSlabTopCenAddBarUserLenMap() { return mm_PCIMTSlabTopCenAddBarUserLen; }
	virtual map<long, long> GetPCIMTSlabShortSpliceBarUserLenMap() { return mm_PCIMTSlabShortSpliceBarUserLen; }

	virtual void SetPCIMTSlabTopEndAddBarUserLenMap(map<long, long> mapUserLen);
	virtual void SetPCIMTSlabTopCenAddBarUserLenMap(map<long, long> mapUserLen);
	virtual void SetPCIMTSlabShortSpliceBarUserLenMap(map<long, long> mapUserLen);

	virtual long GetPCIMTSlabTopEndAddBarUserLen(double dDia);
	virtual long GetPCIMTSlabTopCenAddBarUserLen(double dDia);
	virtual long GetPCIMTSlabShortSpliceBarUserLen(double dDia);

	virtual void SetPCIMTSlabTopEndAddBarUserLen(long key, long value) { mm_PCIMTSlabTopEndAddBarUserLen[key] = value; }
	virtual void SetPCIMTSlabTopCenAddBarUserLen(long key, long value) { mm_PCIMTSlabTopCenAddBarUserLen[key] = value; }
	virtual void SetPCIMTSlabShortSpliceBarUserLen(long key, long value) { mm_PCIMTSlabShortSpliceBarUserLen[key] = value; }

	//PC HCS Slab
	virtual BOOL GetPCHCSSlabSpliceDevConditionSet() { return m_bPCHCSSlabSpliceDevConditionSet; }////
	virtual long GetPCHCSSlabSpliceDevConditionLength() { return m_nPCHCSSlabSpliceDevConditionLength; }////

	virtual BOOL GetPCHCSSlabSpliceAddLengthSet() { return m_bPCHCSSlabSpliceAddLengthSet; }////
	virtual long GetPCHCSSlabSpliceAddLengthNumSplice() { return m_nPCHCSSlabSpliceAddLengthNumSplice; };////

	virtual long GetPCHCSSlabMaxDualHookLength() { return m_nPCHCSSlabMaxDualHookLength; }////
	virtual long GetPCHCSSlabSpliceWay() { return m_nPCHCSSlabSpliceWay; }////
	virtual long GetPCHCSSlabSpliceType() { return m_nPCHCSSlabSpliceType; }////
	
	virtual long GetPCHCSSlabTopHookType() { return m_nPCHCSSlabTopHookType; }
	virtual long GetPCHCSSlabBotHookType() { return m_nPCHCSSlabBotHookType; }
	virtual long GetPCHCSSlabTopEndDevTypeUserLength() { return m_nPCHCSSlabTopEndDevTypeUserLength; }
	virtual long GetPCHCSSlabBotEndDevTypeUserLength() { return m_nPCHCSSlabBotEndDevTypeUserLength; }
	virtual long GetPCHCSSlabOpeningTopDevType() { return m_nPCHCSSlabOpeningTopDevType; }
	virtual long GetPCHCSSlabOpeningBotDevType() { return m_nPCHCSSlabOpeningBotDevType; }
	
	virtual long GetPCHCSSlabTopEndAddBarLxLnxType() { return m_nPCHCSSlabTopEndAddBarLxLnxType; }
	virtual long GetPCHCSSlabTopEndAddBarLengthType() { return m_nPCHCSSlabTopEndAddBarLengthType; }
	virtual long GetPCHCSSlabTopCenAddBarLxLnxType() { return m_nPCHCSSlabTopCenAddBarLxLnxType; }
	virtual long GetPCHCSSlabTopCenAddBarLengthType() { return m_nPCHCSSlabTopCenAddBarLengthType; }
	virtual long GetPCHCSSlabDevSubBarLengthType() { return m_nPCHCSSlabDevSubBarLengthType; }
	virtual long GetPCHCSSlabShortCenSpliceBarLengthType() { return m_nPCHCSSlabShortCenSpliceBarLengthType; }
	virtual long GetPCHCSSlabShortEndSpliceBarLengthType() { return m_nPCHCSSlabShortEndSpliceBarLengthType; }
	
	virtual long GetPCHCSSlabLongCenSpliceBarLengthType() { return m_nPCHCSSlabLongCenSpliceBarLengthType; }
	virtual long GetPCHCSSlabLongEndSpliceBarLengthType() { return m_nPCHCSSlabLongEndSpliceBarLengthType; }

	virtual long GetPCHCSSlabRebarTolDist() { return m_nPCHCSSlabRebarTolDist; }////
	virtual long GetPCHCSSlabRebarTolLenDif() { return m_nPCHCSSlabRebarTolLenDif; }////

	virtual BOOL GetPCHCSSlabShortCenSpliceBarPlace() { return m_bPCHCSSlabShortCenSpliceBarPlace; }
	virtual BOOL GetPCHCSSlabShortEndSpliceBarPlace() { return m_bPCHCSSlabShortEndSpliceBarPlace; }
	virtual BOOL GetPCHCSSlabLongCenSpliceBarPlace() { return m_bPCHCSSlabLongCenSpliceBarPlace; }
	virtual BOOL GetPCHCSSlabLongEndSpliceBarPlace() { return m_bPCHCSSlabLongEndSpliceBarPlace; }

	virtual void SetPCHCSSlabSpliceDevConditionSet(BOOL value) { m_bPCHCSSlabSpliceDevConditionSet = value; }
	virtual void SetPCHCSSlabSpliceDevConditionLength(long value) { m_nPCHCSSlabSpliceDevConditionLength = value; }

	virtual void SetPCHCSSlabSpliceAddLengthSet(BOOL value) { m_bPCHCSSlabSpliceAddLengthSet = value; }
	virtual void SetPCHCSSlabSpliceAddLengthNumSplice(long value) { m_nPCHCSSlabSpliceAddLengthNumSplice = value; }

	virtual void SetPCHCSSlabMaxDualHookLength(long value) { m_nPCHCSSlabMaxDualHookLength = value; }
	virtual void SetPCHCSSlabSpliceWay(long value) { m_nPCHCSSlabSpliceWay = value; }
	virtual void SetPCHCSSlabSpliceType(long value) { m_nPCHCSSlabSpliceType = value; }
	
	virtual void SetPCHCSSlabTopHookType(long value) { m_nPCHCSSlabTopHookType = value; }
	virtual void SetPCHCSSlabBotHookType(long value) { m_nPCHCSSlabBotHookType = value; }
	virtual void SetPCHCSSlabTopEndDevTypeUserLength(long value) { m_nPCHCSSlabTopEndDevTypeUserLength = value; }
	virtual void SetPCHCSSlabBotEndDevTypeUserLength(long value) { m_nPCHCSSlabBotEndDevTypeUserLength = value; }
	virtual void SetPCHCSSlabOpeningTopDevType(long value) { m_nPCHCSSlabOpeningTopDevType = value; }
	virtual void SetPCHCSSlabOpeningBotDevType(long value) { m_nPCHCSSlabOpeningBotDevType = value; }

	
	virtual void SetPCHCSSlabTopEndAddBarLxLnxType(long value) { m_nPCHCSSlabTopEndAddBarLxLnxType = value; }
	virtual void SetPCHCSSlabTopEndAddBarLengthType(long value) { m_nPCHCSSlabTopEndAddBarLengthType = value; }
	virtual void SetPCHCSSlabTopCenAddBarLxLnxType(long value) { m_nPCHCSSlabTopCenAddBarLxLnxType = value; }
	virtual void SetPCHCSSlabTopCenAddBarLengthType(long value) { m_nPCHCSSlabTopCenAddBarLengthType = value; }
	virtual void SetPCHCSSlabDevSubBarLengthType(long value) { m_nPCHCSSlabDevSubBarLengthType = value; }
	virtual void SetPCHCSSlabShortCenSpliceBarLengthType(long value) { m_nPCHCSSlabShortCenSpliceBarLengthType = value; }
	virtual void SetPCHCSSlabShortEndSpliceBarLengthType(long value) { m_nPCHCSSlabShortEndSpliceBarLengthType = value; }

	virtual void SetPCHCSSlabLongCenSpliceBarLengthType(long value) { m_nPCHCSSlabLongCenSpliceBarLengthType = value; }
	virtual void SetPCHCSSlabLongEndSpliceBarLengthType(long value) { m_nPCHCSSlabLongEndSpliceBarLengthType = value; }
		
	virtual void SetPCHCSSlabRebarTolDist(long value) { m_nPCHCSSlabRebarTolDist = value; }
	virtual void SetPCHCSSlabRebarTolLenDif(long value) { m_nPCHCSSlabRebarTolLenDif = value; }

	virtual void SetPCHCSSlabShortCenSpliceBarPlace(BOOL value) { m_bPCHCSSlabShortCenSpliceBarPlace = value;}
	virtual void SetPCHCSSlabShortEndSpliceBarPlace(BOOL value) { m_bPCHCSSlabShortEndSpliceBarPlace = value; }
	virtual void SetPCHCSSlabLongCenSpliceBarPlace(BOOL value) { m_bPCHCSSlabLongCenSpliceBarPlace = value; }
	virtual void SetPCHCSSlabLongEndSpliceBarPlace(BOOL value) { m_bPCHCSSlabLongEndSpliceBarPlace = value; }
	
	virtual map<long, long> GetPCHCSSlabTopEndAddBarUserLenMap() { return mm_PCHCSSlabTopEndAddBarUserLen; }
	virtual map<long, long> GetPCHCSSlabTopCenAddBarUserLenMap() { return mm_PCHCSSlabTopCenAddBarUserLen; }
	virtual map<long, long> GetPCHCSSlabDevSubBarUserLenMap() { return mm_PCHCSSlabDevSubBarUserLen; }
	virtual map<long, long> GetPCHCSSlabShortCenSpliceBarUserLenMap() { return mm_PCHCSSlabShortCenSpliceBarUserLen; }
	virtual map<long, long> GetPCHCSSlabShortEndSpliceBarUserLenMap() { return mm_PCHCSSlabShortEndSpliceBarUserLen; }
	virtual map<long, long> GetPCHCSSlabLongCenSpliceBarUserLenMap() { return mm_PCHCSSlabLongCenSpliceBarUserLen; }
	virtual map<long, long> GetPCHCSSlabLongEndSpliceBarUserLenMap() { return mm_PCHCSSlabLongEndSpliceBarUserLen; }

	virtual void SetPCHCSSlabTopEndAddBarUserLenMap(map<long, long> mapUserLen);
	virtual void SetPCHCSSlabTopCenAddBarUserLenMap(map<long, long> mapUserLen);
	virtual void SetPCHCSSlabDevSubBarUserLenMap(map<long, long> mapUserLen);
	virtual void SetPCHCSSlabShortCenSpliceBarUserLenMap(map<long, long> mapUserLen);
	virtual void SetPCHCSSlabShortEndSpliceBarUserLenMap(map<long, long> mapUserLen);
	virtual void SetPCHCSSlabLongCenSpliceBarUserLenMap(map<long, long> mapUserLen);
	virtual void SetPCHCSSlabLongEndSpliceBarUserLenMap(map<long, long> mapUserLen);

	virtual long GetPCHCSSlabTopEndAddBarUserLen(double dDia);
	virtual long GetPCHCSSlabTopCenAddBarUserLen(double dDia);
	virtual long GetPCHCSSlabDevSubBarUserLen(double dDia);
	virtual long GetPCHCSSlabShortCenSpliceBarUserLen(double dDia);
	virtual long GetPCHCSSlabShortEndSpliceBarUserLen(double dDia);
	virtual long GetPCHCSSlabLongCenSpliceBarUserLen(double dDia);
	virtual long GetPCHCSSlabLongEndSpliceBarUserLen(double dDia);

	virtual void SetPCHCSSlabTopEndAddBarUserLen(long key, long value) { mm_PCHCSSlabTopEndAddBarUserLen[key] = value; }
	virtual void SetPCHCSSlabTopCenAddBarUserLen(long key, long value) { mm_PCHCSSlabTopCenAddBarUserLen[key] = value; }
	virtual void SetPCHCSSlabDevSubBarUserLen(long key, long value) { mm_PCHCSSlabDevSubBarUserLen[key] = value; }
	virtual void SetPCHCSSlabShortCenSpliceBarUserLen(long key, long value) { mm_PCHCSSlabShortCenSpliceBarUserLen[key] = value; }
	virtual void SetPCHCSSlabShortEndSpliceBarUserLen(long key, long value) { mm_PCHCSSlabShortEndSpliceBarUserLen[key] = value; }
	virtual void SetPCHCSSlabLongCenSpliceBarUserLen(long key, long value) { mm_PCHCSSlabLongCenSpliceBarUserLen[key] = value; }
	virtual void SetPCHCSSlabLongEndSpliceBarUserLen(long key, long value) { mm_PCHCSSlabLongEndSpliceBarUserLen[key] = value; }

	
	//PC RPS Slab
	virtual BOOL GetPCRPSSlabSpliceDevConditionSet() { return m_bPCRPSSlabSpliceDevConditionSet; }//
	virtual long GetPCRPSSlabSpliceDevConditionLength() { return m_nPCRPSSlabSpliceDevConditionLength; }//

	virtual BOOL GetPCRPSSlabSpliceAddLengthSet() { return m_bPCRPSSlabSpliceAddLengthSet; }//
	virtual long GetPCRPSSlabSpliceAddLengthNumSplice() { return m_nPCRPSSlabSpliceAddLengthNumSplice; };//

	virtual long GetPCRPSSlabMaxDualHookLength() { return m_nPCRPSSlabMaxDualHookLength; }//
	virtual long GetPCRPSSlabSpliceWay() { return m_nPCRPSSlabSpliceWay; }//
	virtual long GetPCRPSSlabSpliceType() { return m_nPCRPSSlabSpliceType; }//
		
	virtual long GetPCRPSSlabTopHookType() { return m_nPCRPSSlabTopHookType; }
	virtual long GetPCRPSSlabBotHookType() { return m_nPCRPSSlabBotHookType; }
	virtual long GetPCRPSSlabTopEndDevTypeUserLength() { return m_nPCRPSSlabTopEndDevTypeUserLength; }
	virtual long GetPCRPSSlabBotEndDevTypeUserLength() { return m_nPCRPSSlabBotEndDevTypeUserLength; }
	virtual long GetPCRPSSlabOpeningTopDevType() { return m_nPCRPSSlabOpeningTopDevType; }
	virtual long GetPCRPSSlabOpeningBotDevType() { return m_nPCRPSSlabOpeningBotDevType; }
		
	virtual long GetPCRPSSlabTopEndAddBarLxLnxType() { return m_nPCRPSSlabTopEndAddBarLxLnxType; }
	virtual long GetPCRPSSlabTopEndAddBarLengthType() { return m_nPCRPSSlabTopEndAddBarLengthType; }
	virtual long GetPCRPSSlabTopCenAddBarLxLnxType() { return m_nPCRPSSlabTopCenAddBarLxLnxType; }
	virtual long GetPCRPSSlabTopCenAddBarLengthType() { return m_nPCRPSSlabTopCenAddBarLengthType; }
	virtual long GetPCRPSSlabDevSubBarLengthType() { return m_nPCRPSSlabDevSubBarLengthType; }
	virtual long GetPCRPSSlabLongCenSpliceBarLengthType() { return m_nPCRPSSlabLongCenSpliceBarLengthType; }
	virtual long GetPCRPSSlabLongEndSpliceBarLengthType() { return m_nPCRPSSlabLongEndSpliceBarLengthType; }

	virtual long GetPCRPSSlabRebarTolDist() { return m_nPCRPSSlabRebarTolDist; }//
	virtual long GetPCRPSSlabRebarTolLenDif() { return m_nPCRPSSlabRebarTolLenDif; }//

	virtual BOOL GetPCRPSSlabLongCenSpliceBarPlace() { return m_bPCRPSSlabLongCenSpliceBarPlace; }
	virtual BOOL GetPCRPSSlabLongEndSpliceBarPlace() { return m_bPCRPSSlabLongEndSpliceBarPlace; }

	virtual void SetPCRPSSlabSpliceDevConditionSet(BOOL value) { m_bPCRPSSlabSpliceDevConditionSet = value; }
	virtual void SetPCRPSSlabSpliceDevConditionLength(long value) { m_nPCRPSSlabSpliceDevConditionLength = value; }

	virtual void SetPCRPSSlabSpliceAddLengthSet(BOOL value) { m_bPCRPSSlabSpliceAddLengthSet = value; }
	virtual void SetPCRPSSlabSpliceAddLengthNumSplice(long value) { m_nPCRPSSlabSpliceAddLengthNumSplice = value; }

	virtual void SetPCRPSSlabMaxDualHookLength(long value) { m_nPCRPSSlabMaxDualHookLength = value; }
	virtual void SetPCRPSSlabSpliceWay(long value) { m_nPCRPSSlabSpliceWay = value; }
	virtual void SetPCRPSSlabSpliceType(long value) { m_nPCRPSSlabSpliceType = value; }
		
	virtual void SetPCRPSSlabTopHookType(long value) { m_nPCRPSSlabTopHookType = value; }
	virtual void SetPCRPSSlabBotHookType(long value) { m_nPCRPSSlabBotHookType = value; }
	virtual void SetPCRPSSlabTopEndDevTypeUserLength(long value) { m_nPCRPSSlabTopEndDevTypeUserLength = value; }
	virtual void SetPCRPSSlabBotEndDevTypeUserLength(long value) { m_nPCRPSSlabBotEndDevTypeUserLength = value; }
	virtual void SetPCRPSSlabOpeningTopDevType(long value) { m_nPCRPSSlabOpeningTopDevType = value; }
	virtual void SetPCRPSSlabOpeningBotDevType(long value) { m_nPCRPSSlabOpeningBotDevType = value; }

	virtual void SetPCRPSSlabTopEndAddBarLxLnxType(long value) { m_nPCRPSSlabTopEndAddBarLxLnxType = value; }
	virtual void SetPCRPSSlabTopEndAddBarLengthType(long value) { m_nPCRPSSlabTopEndAddBarLengthType = value; }
	virtual void SetPCRPSSlabTopCenAddBarLxLnxType(long value) { m_nPCRPSSlabTopCenAddBarLxLnxType = value; }
	virtual void SetPCRPSSlabTopCenAddBarLengthType(long value) { m_nPCRPSSlabTopCenAddBarLengthType = value; }
	virtual void SetPCRPSSlabDevSubBarLengthType(long value) { m_nPCRPSSlabDevSubBarLengthType = value; }
	virtual void SetPCRPSSlabLongCenSpliceBarLengthType(long value) { m_nPCRPSSlabLongCenSpliceBarLengthType = value; }
	virtual void SetPCRPSSlabLongEndSpliceBarLengthType(long value) { m_nPCRPSSlabLongEndSpliceBarLengthType = value; }
		
	virtual void SetPCRPSSlabRebarTolDist(long value) { m_nPCRPSSlabRebarTolDist = value; }
	virtual void SetPCRPSSlabRebarTolLenDif(long value) { m_nPCRPSSlabRebarTolLenDif = value; }

	virtual void SetPCRPSSlabLongCenSpliceBarPlace(BOOL value) { m_bPCRPSSlabLongCenSpliceBarPlace = value; }
	virtual void SetPCRPSSlabLongEndSpliceBarPlace(BOOL value) { m_bPCRPSSlabLongEndSpliceBarPlace = value; }

	virtual map<long, long> GetPCRPSSlabTopEndAddBarUserLenMap() { return mm_PCRPSSlabTopEndAddBarUserLen; }
	virtual map<long, long> GetPCRPSSlabTopCenAddBarUserLenMap() { return mm_PCRPSSlabTopCenAddBarUserLen; }
	virtual map<long, long> GetPCRPSSlabDevSubBarUserLenMap() { return mm_PCRPSSlabDevSubBarUserLen; }
	virtual map<long, long> GetPCRPSSlabLongCenSpliceBarUserLenMap() { return mm_PCRPSSlabLongCenSpliceBarUserLen; }
	virtual map<long, long> GetPCRPSSlabLongEndSpliceBarUserLenMap() { return mm_PCRPSSlabLongEndSpliceBarUserLen; }

	virtual void SetPCRPSSlabTopEndAddBarUserLenMap(map<long, long> mapUserLen);
	virtual void SetPCRPSSlabTopCenAddBarUserLenMap(map<long, long> mapUserLen);
	virtual void SetPCRPSSlabDevSubBarUserLenMap(map<long, long> mapUserLen);
	virtual void SetPCRPSSlabLongCenSpliceBarUserLenMap(map<long, long> mapUserLen);
	virtual void SetPCRPSSlabLongEndSpliceBarUserLenMap(map<long, long> mapUserLen);

	virtual long GetPCRPSSlabTopEndAddBarUserLen(double dDia);
	virtual long GetPCRPSSlabTopCenAddBarUserLen(double dDia);
	virtual long GetPCRPSSlabDevSubBarUserLen(double dDia);
	virtual long GetPCRPSSlabLongCenSpliceBarUserLen(double dDia);
	virtual long GetPCRPSSlabLongEndSpliceBarUserLen(double dDia);

	virtual void SetPCRPSSlabTopEndAddBarUserLen(long key, long value) { mm_PCRPSSlabTopEndAddBarUserLen[key] = value; }
	virtual void SetPCRPSSlabTopCenAddBarUserLen(long key, long value) { mm_PCRPSSlabTopCenAddBarUserLen[key] = value; }
	virtual void SetPCRPSSlabDevSubBarUserLen(long key, long value) { mm_PCRPSSlabDevSubBarUserLen[key] = value; }
	virtual void SetPCRPSSlabLongCenSpliceBarUserLen(long key, long value) { mm_PCRPSSlabLongCenSpliceBarUserLen[key] = value; }
	virtual void SetPCRPSSlabLongEndSpliceBarUserLen(long key, long value) { mm_PCRPSSlabLongEndSpliceBarUserLen[key] = value; }

	// [BH-4851] PC, 데크슬래브 판판연결근 도리근 추가
	virtual SlabHorSupportRebarsTemplate *GetPCSlabHorSupportRebarsTemplate();
	virtual SlabHorSupportRebarsTemplate *GetPCIMTSlabHorSupportRebarsTemplate();
	virtual SlabHorSupportRebarsTemplate *GetPCHCSSlabHorSupportRebarsTemplate();
	virtual SlabHorSupportRebarsTemplate *GetPCRPSSlabHorSupportRebarsTemplate();
	virtual SlabHorSupportRebarsTemplate *GetGeneralDeckHorSupportRebarsTemplate();
	virtual SlabHorSupportRebarsTemplate *GetEmbeddedDeckHorSupportRebarsTemplate();

	virtual ShearFrictionBarTemplate *GetMatShearFrictionBarTemplate();
	virtual ShearFrictionBarTemplate *GetFlatShearFrictionBarTemplate();

	virtual BOOL GetMatBotSpacerUserSteelGradeS() { return m_bMatBotSpacerUserSteelGradeS; }
	virtual BOOL GetMatTopSpacerUserSteelGradeS() { return m_bMatTopSpacerUserSteelGradeS; }
	virtual BOOL GetMatSpacerSupportBarUserSteelGradeS() { return m_bMatSpacerSupportBarUserSteelGradeS; }
	virtual BOOL GetMatSupportRebarUserSteelGradeS() { return m_bMatSupportRebarUserSteelGradeS; }

	virtual BOOL GetFlatBotSpacerUserSteelGradeS() { return m_bFlatBotSpacerUserSteelGradeS; }
	virtual BOOL GetFlatTopSpacerUserSteelGradeS() { return m_bFlatTopSpacerUserSteelGradeS; }
	virtual BOOL GetFlatSpacerSupportBarUserSteelGradeS() { return m_bFlatSpacerSupportBarUserSteelGradeS; }

	virtual BOOL GetSlabBotSpacerUserSteelGradeS() { return m_bSlabBotSpacerUserSteelGradeS; }
	virtual BOOL GetSlabTopSpacerUserSteelGradeS() { return m_bSlabTopSpacerUserSteelGradeS; }
	virtual BOOL GetSlabSpacerSupportBarUserSteelGradeS() { return m_bSlabSpacerSupportBarUserSteelGradeS; }

	virtual BOOL GetSOGBotSpacerUserSteelGradeS() { return m_bSOGBotSpacerUserSteelGradeS; }
	virtual BOOL GetSOGTopSpacerUserSteelGradeS() { return m_bSOGTopSpacerUserSteelGradeS; }
	virtual BOOL GetSOGSpacerSupportBarUserSteelGradeS() { return m_bSOGSpacerSupportBarUserSteelGradeS; }
	virtual BOOL GetSOGSupportRebarUserSteelGradeS() { return m_bSOGSupportRebarUserSteelGradeS; }

	virtual BOOL GetCircularRampBotSpacerUserSteelGradeS() { return m_bCircularRampBotSpacerUserSteelGradeS; }
	virtual BOOL GetCircularRampTopSpacerUserSteelGradeS() { return m_bCircularRampTopSpacerUserSteelGradeS; }
	virtual BOOL GetCircularRampSpacerSupportBarUserSteelGradeS() { return m_bCircularRampSpacerSupportBarUserSteelGradeS; }

	//
	virtual void SetMatBotSpacerUserSteelGradeS(BOOL value) { m_bMatBotSpacerUserSteelGradeS = value; }
	virtual void SetMatTopSpacerUserSteelGradeS(BOOL value) { m_bMatTopSpacerUserSteelGradeS = value; }
	virtual void SetMatSpacerSupportBarUserSteelGradeS(BOOL value) { m_bMatSpacerSupportBarUserSteelGradeS = value; }
	virtual void SetMatSupportRebarUserSteelGradeS(BOOL value) { m_bMatSupportRebarUserSteelGradeS = value; }

	virtual void SetFlatBotSpacerUserSteelGradeS(BOOL value) { m_bFlatBotSpacerUserSteelGradeS = value; }
	virtual void SetFlatTopSpacerUserSteelGradeS(BOOL value) { m_bFlatTopSpacerUserSteelGradeS = value; }
	virtual void SetFlatSpacerSupportBarUserSteelGradeS(BOOL value) { m_bFlatSpacerSupportBarUserSteelGradeS = value; }

	virtual void SetSlabBotSpacerUserSteelGradeS(BOOL value) { m_bSlabBotSpacerUserSteelGradeS = value; }
	virtual void SetSlabTopSpacerUserSteelGradeS(BOOL value) { m_bSlabTopSpacerUserSteelGradeS = value; }
	virtual void SetSlabSpacerSupportBarUserSteelGradeS(BOOL value) { m_bSlabSpacerSupportBarUserSteelGradeS = value; }

	virtual void SetSOGBotSpacerUserSteelGradeS(BOOL value) { m_bSOGBotSpacerUserSteelGradeS = value; }
	virtual void SetSOGTopSpacerUserSteelGradeS(BOOL value) { m_bSOGTopSpacerUserSteelGradeS = value; }
	virtual void SetSOGSpacerSupportBarUserSteelGradeS(BOOL value) { m_bSOGSpacerSupportBarUserSteelGradeS = value; }
	virtual void SetSOGSupportRebarUserSteelGradeS(BOOL value) { m_bSOGSupportRebarUserSteelGradeS = value; }

	virtual void SetCircularRampBotSpacerUserSteelGradeS(BOOL value) { m_bCircularRampBotSpacerUserSteelGradeS = value; }
	virtual void SetCircularRampTopSpacerUserSteelGradeS(BOOL value) { m_bCircularRampTopSpacerUserSteelGradeS = value; }
	virtual void SetCircularRampSpacerSupportBarUserSteelGradeS(BOOL value) { m_bCircularRampSpacerSupportBarUserSteelGradeS = value; }
	
	virtual long GetEmbeddedDeckCZoneSpliceWay() { return m_nEmbeddedDeckCZoneSpliceWay; }
	virtual long GetEmbeddedDeckCZoneSpliceLength() { return m_nEmbeddedDeckCZoneSpliceLength; }
	virtual long GetGeneralDeckCZoneSpliceWay() { return m_nGeneralDeckCZoneSpliceWay; }
	virtual long GetGeneralDeckCZoneSpliceLength() { return m_nGeneralDeckCZoneSpliceLength; }
	virtual long GetPCSlabCZoneSpliceWay() { return m_nPCSlabCZoneSpliceWay; }
	virtual long GetPCSlabCZoneSpliceLength() { return m_nPCSlabCZoneSpliceLength; }
	virtual long GetPCIMTSlabCZoneSpliceWay() { return m_nPCIMTSlabCZoneSpliceWay; }
	virtual long GetPCIMTSlabCZoneSpliceLength() { return m_nPCIMTSlabCZoneSpliceLength; }
	virtual long GetPCHCSSlabCZoneSpliceWay() { return m_nPCHCSSlabCZoneSpliceWay; }
	virtual long GetPCHCSSlabCZoneSpliceLength() { return m_nPCHCSSlabCZoneSpliceLength; }
	virtual long GetPCRPSSlabCZoneSpliceWay() { return m_nPCRPSSlabCZoneSpliceWay; }
	virtual long GetPCRPSSlabCZoneSpliceLength() { return m_nPCRPSSlabCZoneSpliceLength; }

	virtual void SetEmbeddedDeckCZoneSpliceWay(long value) { m_nEmbeddedDeckCZoneSpliceWay = value; }
	virtual void SetEmbeddedDeckCZoneSpliceLength(long value) { m_nEmbeddedDeckCZoneSpliceLength = value; }
	virtual void SetGeneralDeckCZoneSpliceWay(long value) { m_nGeneralDeckCZoneSpliceWay = value; }
	virtual void SetGeneralDeckCZoneSpliceLength(long value) { m_nGeneralDeckCZoneSpliceLength = value; }
	virtual void SetPCSlabCZoneSpliceWay(long value) { m_nPCSlabCZoneSpliceWay = value; }
	virtual void SetPCSlabCZoneSpliceLength(long value) { m_nPCSlabCZoneSpliceLength = value; }
	virtual void SetPCIMTSlabCZoneSpliceWay(long value) { m_nPCIMTSlabCZoneSpliceWay = value; }
	virtual void SetPCIMTSlabCZoneSpliceLength(long value) { m_nPCIMTSlabCZoneSpliceLength = value; }
	virtual void SetPCHCSSlabCZoneSpliceWay(long value) { m_nPCHCSSlabCZoneSpliceWay = value; }
	virtual void SetPCHCSSlabCZoneSpliceLength(long value) { m_nPCHCSSlabCZoneSpliceLength = value; }
	virtual void SetPCRPSSlabCZoneSpliceWay(long value) { m_nPCRPSSlabCZoneSpliceWay = value; }
	virtual void SetPCRPSSlabCZoneSpliceLength(long value) { m_nPCRPSSlabCZoneSpliceLength = value; }

	virtual BOOL GetMatTopSpacerPlaceConditionSet() { return m_bMatTopSpacerPlaceConditionSet; }
	virtual long GetMatTopSpacerPlaceCondition() { return m_nMatTopSpacerPlaceCondition; }
	virtual void SetMatTopSpacerPlaceConditionSet(BOOL value) { m_bMatTopSpacerPlaceConditionSet = value; }
	virtual void SetMatTopSpacerPlaceCondition(long value) { m_nMatTopSpacerPlaceCondition = value; }

	virtual BOOL GetFlatTopSpacerPlaceConditionSet() { return m_bFlatTopSpacerPlaceConditionSet; }
	virtual long GetFlatTopSpacerPlaceCondition() { return m_nFlatTopSpacerPlaceCondition; }
	virtual void SetFlatTopSpacerPlaceConditionSet(BOOL value) { m_bFlatTopSpacerPlaceConditionSet = value; }
	virtual void SetFlatTopSpacerPlaceCondition(long value) { m_nFlatTopSpacerPlaceCondition = value; }

	virtual BOOL GetSlabTopSpacerPlaceConditionSet() { return m_bSlabTopSpacerPlaceConditionSet; }
	virtual long GetSlabTopSpacerPlaceCondition() { return m_nSlabTopSpacerPlaceCondition; }
	virtual void SetSlabTopSpacerPlaceConditionSet(BOOL value) { m_bSlabTopSpacerPlaceConditionSet = value; }
	virtual void SetSlabTopSpacerPlaceCondition(long value) { m_nSlabTopSpacerPlaceCondition = value; }

	virtual BOOL GetSOGTopSpacerPlaceConditionSet() { return m_bSOGTopSpacerPlaceConditionSet; }
	virtual long GetSOGTopSpacerPlaceCondition() { return m_nSOGTopSpacerPlaceCondition; }
	virtual void SetSOGTopSpacerPlaceConditionSet(BOOL value) { m_bSOGTopSpacerPlaceConditionSet = value; }
	virtual void SetSOGTopSpacerPlaceCondition(long value) { m_nSOGTopSpacerPlaceCondition = value; }

	virtual BOOL GetCircularRampTopSpacerPlaceConditionSet() { return m_bCircularRampTopSpacerPlaceConditionSet; }
	virtual long GetCircularRampTopSpacerPlaceCondition() { return m_nCircularRampTopSpacerPlaceCondition; }
	virtual void SetCircularRampTopSpacerPlaceConditionSet(BOOL value) { m_bCircularRampTopSpacerPlaceConditionSet = value; }
	virtual void SetCircularRampTopSpacerPlaceCondition(long value) { m_nCircularRampTopSpacerPlaceCondition = value; }


	virtual BOOL GetSlabOpeningAddBarDeductionCondition() { return m_bSlabOpeningAddBarDeductionCondition; }
	virtual void SetSlabOpeningAddBarDeductionCondition(BOOL value) { m_bSlabOpeningAddBarDeductionCondition = value; }
	virtual long GetSlabOpeningAddBarDeductionPercent() { return m_nSlabOpeningAddBarDeductionPercent; }
	virtual void SetSlabOpeningAddBarDeductionPercent(long value) { m_nSlabOpeningAddBarDeductionPercent = value; }
	
	virtual BOOL GetSlabLongEndPlaceAreaSet() { return m_bSlabLongEndPlaceAreaSet; }
	virtual long GetSlabLongEndPlaceAreaOneWaySlabType() { return m_nSlabLongEndPlaceAreaOneWaySlabType; }
	virtual long GetSlabLongEndPlaceAreaTwoWaySlabType() { return m_nSlabLongEndPlaceAreaTwoWaySlabType; }
	virtual BOOL GetSlabBotAddBarLengthSet() { return m_bSlabBotAddBarLengthSet; }
	virtual long GetSlabBotAddBarLengthType() { return m_nSlabBotAddBarLengthType; }
	virtual BOOL GetSlabBotAddBarCutBarLenExclusionSet() { return m_bSlabBotAddBarCutBarLenExclusionSet; }

	virtual void  SetSlabLongEndPlaceAreaSet(BOOL value) { m_bSlabLongEndPlaceAreaSet = value; }
	virtual void  SetSlabLongEndPlaceAreaOneWaySlabType(long value) { m_nSlabLongEndPlaceAreaOneWaySlabType = value; }
	virtual void  SetSlabLongEndPlaceAreaTwoWaySlabType(long value) { m_nSlabLongEndPlaceAreaTwoWaySlabType = value; }
	virtual void  SetSlabBotAddBarLengthSet(BOOL value) { m_bSlabBotAddBarLengthSet = value; }
	virtual void  SetSlabBotAddBarLengthType(long value) { m_nSlabBotAddBarLengthType = value; }
	virtual void  SetSlabBotAddBarCutBarLenExclusionSet(BOOL value) { m_bSlabBotAddBarCutBarLenExclusionSet = value; }

private:
	// [BH-4851] PC, 데크슬래브 판판연결근 도리근 추가
	SlabHorSupportRebarsTemplate *m_pPCSlabHorSupportRebarsTemplate;
	SlabHorSupportRebarsTemplate *m_pPCIMTSlabHorSupportRebarsTemplate;
	SlabHorSupportRebarsTemplate *m_pPCHCSSlabHorSupportRebarsTemplate;
	SlabHorSupportRebarsTemplate *m_pPCRPSSlabHorSupportRebarsTemplate;
	SlabHorSupportRebarsTemplate *m_pGeneralDeckHorSupportRebarsTemplate;
	SlabHorSupportRebarsTemplate *m_pEmbeddedDeckHorSupportRebarsTemplate;
	//ShearFrictionBarTemplate	
	ShearFrictionBarTemplate *m_pMatShearFrictionBarTemplate;
	ShearFrictionBarTemplate *m_pFlatShearFrictionBarTemplate;
	// MAT Slab
	BOOL m_bMatMainBarInfoSet;
	long m_nMatMaxDualHookLength;	//최대 양후크길이
	long m_nMatSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nMatSpliceType;	//이음타입 0:인장이음 1:압축이음
		
	BOOL m_bMatEndInfoSet;
	long m_nMatTopEndHookType;		//상부근 단부 0:정착 1:후크 
	long m_nMatBotEndHookType;		//하부근 단부 0:일자 1:후크	
	long m_nMatIsoFootDevType;		//독립기초 & Panel 접합부위 하부근 0:Passing 1:인장정착

	long m_nMatTopEndDevTypeUserLength;
	long m_nMatBotEndDevTypeUserLength;
	long m_nMatOpeningTopDevType;
	long m_nMatOpeningBotDevType;

	BOOL m_bMatLevelDifInfoSet;
	long m_nMatLevelDifPlaceShape;		//배근 형상
	long m_nMatLevelDifHaunchPlaceType;	//헌치 배근타입

	BOOL m_bMatSupportRebarsSet;				//도리근 설정
	long m_nMatSupportRebarsDiaIndex;			//도리근 직경
	long m_nMatSupportRebarsLengthType;			//도리근 길이 타입
	double m_dMatSupportRebarsRWallLengthRate;	//지하외벽 길이 비율
	double m_dMatSupportRebarsSlabLengthRate;	//해당 슬래브의 둘레길이 비율
	long m_nMatSupportRebarsOrderLength;		//도리근 주문길이
	long m_nMatSupportRebarsSpliceLength;		//도리근 이음길이
	long m_nMatSupportRebarsCorrectiveLength;	//도리근 보정길이

	BOOL m_bMatRebarAreaInfoSet;
	long m_nMatRebarTolDist;		//앞뒤 철근의 길이차
	long m_nMatRebarTolLenDif;		//가장 긴 철근과 짧은 철근이 길이차가 m_nMatRebarTolLenDif이상이 되면 barset을 나눈다.

	BOOL m_bMatSpliceDevConditionSet;
	long m_nMatSpliceDevConditionLength;

	BOOL m_bMatSpliceAddLengthSet;
	long m_nMatSpliceAddLengthNumSplice;

	BOOL m_bMatTopSpacerInfoSet;
	BOOL m_bMatTopSpacerSet;			//상부 우마철근 설정
	long m_nMatTopSpacerDiaType;		//직경 0:해당기초직경 1:슬래브 두께
	long m_nMatTopSpacerSlabThick;		//슬래브 두께
	long m_nMatTopSpacerSlabThickOverDiaIndex;	//슬래브 두께 이상 직경
	long m_nMatTopSpacerSlabThickUnderDiaIndex;	//슬래브 두께 미만 직경
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

	BOOL m_bMatSpacerSupportBarsSet;			//우마지지근 설정
	BOOL m_bMatSpacerSupportBarsConditionSet;	//우마지지근 적용 조건 설정
	long m_nMatSpacerSupportBarsCondition;		//우마지지근 적용 조건
	long m_nMatSpacerSupportBarsDiaType;		//직경 0:상부 우마직경 1:사용자 지정
	long m_nMatSpacerSupportBarsUserDiaIndex;		//사용자 지정 입력
	long m_nMatSpacerSupportBarsNumType;			//개수 0:상부 우마 개수 1:사용자 지정
	double m_dMatSpacerSupportBarsTopSpacerNums;	//상부 우마 개수 입력
	long m_nMatSpacerSupportBarsUserNums;			//사용자 지정 입력
	long m_nMatSpacerSupportBarsLengthType;		//길이 0:해당 슬래브 높이X1.4 1:사용자 지정
	double m_dMatSpacerSupportBarsUserRatio;
	long m_nMatSpacerSupportBarsUserLength;		//사용자 지정 입력

	long m_nMatCZoneSpliceWay;
	long m_nMatCZoneSpliceLength;

	// Flat Slab
	BOOL m_bFlatMainBarInfoSet;
	long m_nFlatMaxDualHookLength;	//최대 양후크길이
	long m_nFlatSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nFlatSpliceType;	//이음타입 0:인장이음 1:압축이음
			
	BOOL m_bFlatEndInfoSet;
	long m_nFlatTopEndHookType;		//상부근 단부 0:정착 1:후크 
	long m_nFlatBotEndHookType;		//하부근 단부 0:정착 1:후크 
	long m_nFlatIsoFootDevType;		//독립기초 & Panel 접합부위 하부근 0:Passing 1:인장정착
	long m_nFlatSpliceDevSlabType;	//이음/정착 적용	0:기초	1:슬래브

	long m_nFlatTopEndDevTypeUserLength;
	long m_nFlatBotEndDevTypeUserLength;
	long m_nFlatOpeningTopDevType;
	long m_nFlatOpeningBotDevType;

	BOOL m_bFlatLevelDifInfoSet;
	long m_nFlatLevelDifPlaceShape;		//배근 형상
	long m_nFlatLevelDifHaunchPlaceType;	//헌치 배근타입
	
	BOOL m_bFlatOpeningInfoSet;
	
	BOOL m_bFlatRebarAreaInfoSet;
	long m_nFlatRebarTolDist;		//앞뒤 철근의 길이차
	long m_nFlatRebarTolLenDif;		//가장 긴 철근과 짧은 철근이 길이차가 m_nFlatRebarTolLenDif이상이 되면 barset을 나눈다.

	BOOL m_bFlatSpliceDevConditionSet;
	long m_nFlatSpliceDevConditionLength;

	BOOL m_bFlatSpliceAddLengthSet;
	long m_nFlatSpliceAddLengthNumSplice;

	BOOL m_bFlatTopSpacerInfoSet;
	BOOL m_bFlatTopSpacerSet;			//상부 우마철근 설정
	long m_nFlatTopSpacerDiaType;		//직경 0:해당기초직경 1:슬래브 두께
	long m_nFlatTopSpacerSlabThick;		//슬래브 두께
	long m_nFlatTopSpacerSlabThickOverDiaIndex;	//슬래브 두께 이상 직경
	long m_nFlatTopSpacerSlabThickUnderDiaIndex;	//슬래브 두께 미만 직경
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

	BOOL m_bFlatSpacerSupportBarsSet;			//우마지지근 설정
	BOOL m_bFlatSpacerSupportBarsConditionSet;	//우마지지근 적용 조건 설정
	long m_nFlatSpacerSupportBarsCondition;		//우마지지근 적용 조건
	long m_nFlatSpacerSupportBarsDiaType;		//직경 0:상부 우마직경 1:사용자 지정
	long m_nFlatSpacerSupportBarsUserDiaIndex;		//사용자 지정 입력
	long m_nFlatSpacerSupportBarsNumType;			//개수 0:상부 우마 개수 1:사용자 지정
	double m_dFlatSpacerSupportBarsTopSpacerNums;	//상부 우마 개수 입력
	long m_nFlatSpacerSupportBarsUserNums;			//사용자 지정 입력
	long m_nFlatSpacerSupportBarsLengthType;		//길이 0:해당 슬래브 높이X1.4 1:사용자 지정
	double m_dFlatSpacerSupportBarsUserRatio;
	long m_nFlatSpacerSupportBarsUserLength;		//사용자 지정 입력

	long m_nFlatCZoneSpliceWay;
	long m_nFlatCZoneSpliceLength;

	// 사변지지 SLAB
	BOOL m_bSlabMainBarInfoSet;
	long m_nSlabMaxDualHookLength;	//최대 양후크길이
	long m_nSlabSpliceType;	//이음타입 0:인장이음 1:압축이음
	long m_nSlabSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	
	BOOL m_bSlabTopCutBarLenInfoSet;
	long m_nSlabOneWayCutBarLenType;	//0:묻힘길이 = max(d, 12db) 1:묻힘길이 = max(d, 12db, In/16) 2:CutBar길이 = 0.3 * 경간길이(Ln)
	long m_nSlabTwoWayCutBarLenType;	//0:묻힘길이 = max(d, 12db) 1:묻힘길이 = max(d, 12db, In/16) 2:CutBar길이 = 0.3 * 경간길이(Ln)

	BOOL m_bSlabUnStructuredEndInfoSet;
	long m_nSlabTopUnStructuredEndHookType;		//상부근 단부 0:정착 1:후크 
	long m_nSlabBotUnStructuredEndHookType;		//하부근 단부 0:정착 1:후크 

	long m_nSlabTopEndDevTypeUserLength;
	long m_nSlabBotEndDevTypeUserLength;
	long m_nSlabOpeningTopDevType;
	long m_nSlabOpeningBotDevType;

	BOOL m_bSlabLevelDifInfoSet;
	long m_nSlabLevelDifPlaceShape;		//배근 형상
	long m_nSlabLevelDifHaunchPlaceType;	//헌치 배근타입
		
	BOOL m_bSlabOpeningInfoSet;
	
	BOOL m_bSlabRebarAreaInfoSet;
	long m_nSlabRebarTolDist;		//앞뒤 철근의 길이차
	long m_nSlabRebarTolLenDif;		//가장 긴 철근과 짧은 철근이 길이차가 m_nSlabRebarTolLenDif이상이 되면 barset을 나눈다.

	BOOL m_bSlabSpliceDevConditionSet;
	long m_nSlabSpliceDevConditionLength;

	BOOL m_bSlabSpliceAddLengthSet;
	long m_nSlabSpliceAddLengthNumSplice;

	BOOL m_bSlabTopSpacerInfoSet;
	BOOL m_bSlabTopSpacerSet;			//상부 우마철근 설정
	long m_nSlabTopSpacerDiaType;		//직경 0:해당기초직경 1:슬래브 두께
	long m_nSlabTopSpacerSlabThick;		//슬래브 두께
	long m_nSlabTopSpacerSlabThickOverDiaIndex;	//슬래브 두께 이상 직경
	long m_nSlabTopSpacerSlabThickUnderDiaIndex;	//슬래브 두께 미만 직경
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

	BOOL m_bSlabSpacerSupportBarsSet;			//우마지지근 설정
	BOOL m_bSlabSpacerSupportBarsConditionSet;	//우마지지근 적용 조건 설정
	long m_nSlabSpacerSupportBarsCondition;		//우마지지근 적용 조건
	long m_nSlabSpacerSupportBarsDiaType;		//직경 0:상부 우마직경 1:사용자 지정
	long m_nSlabSpacerSupportBarsUserDiaIndex;		//사용자 지정 입력
	long m_nSlabSpacerSupportBarsNumType;			//개수 0:상부 우마 개수 1:사용자 지정
	double m_dSlabSpacerSupportBarsTopSpacerNums;	//상부 우마 개수 입력
	long m_nSlabSpacerSupportBarsUserNums;			//사용자 지정 입력
	long m_nSlabSpacerSupportBarsLengthType;		//길이 0:해당 슬래브 높이X1.4 1:사용자 지정
	double m_dSlabSpacerSupportBarsUserRatio;
	long m_nSlabSpacerSupportBarsUserLength;		//사용자 지정 입력

	long m_nSlabCZoneSpliceWay;
	long m_nSlabCZoneSpliceLength;
		
	// SOG Slab
	BOOL m_bSOGMainBarInfoSet;
	long m_nSOGMaxDualHookLength;	//최대 양후크길이
	long m_nSOGSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nSOGSpliceType;	//이음타입 0:인장이음 1:압축이음
		
	BOOL m_bSOGEndInfoSet;
	long m_nSOGTopEndHookType;		//상부근 단부 0:정착 1:후크 
	long m_nSOGBotEndHookType;		//하부근 단부 0:정착 1:후크 
	long m_nSOGIsoFootDevType;		//독립기초 & Panel 접합부위 하부근 0:Passing 1:인장정착
	long m_nSOGSpliceDevSlabType;	//이음/정착 적용	0:기초	1:슬래브

	long m_nSOGTopEndDevTypeUserLength;
	long m_nSOGBotEndDevTypeUserLength;
	long m_nSOGOpeningTopDevType;
	long m_nSOGOpeningBotDevType;

	BOOL m_bSOGLevelDifInfoSet;
	long m_nSOGLevelDifPlaceShape;		//배근 형상
	long m_nSOGLevelDifHaunchPlaceType;	//헌치 배근타입

	BOOL m_bSOGSupportRebarsSet;				//도리근 설정
	long m_nSOGSupportRebarsDiaIndex;			//도리근 직경
	long m_nSOGSupportRebarsLengthType;			//도리근 길이 타입
	double m_dSOGSupportRebarsRWallLengthRate;	//지하외벽 길이 비율
	double m_dSOGSupportRebarsSlabLengthRate;	//해당 슬래브의 둘레길이 비율
	long m_nSOGSupportRebarsOrderLength;		//도리근 주문길이
	long m_nSOGSupportRebarsSpliceLength;		//도리근 이음길이
	long m_nSOGSupportRebarsCorrectiveLength;	//도리근 보정길이

	BOOL m_bSOGOpeningInfoSet;
	
	BOOL m_bSOGRebarAreaInfoSet;
	long m_nSOGRebarTolDist;		//앞뒤 철근의 길이차
	long m_nSOGRebarTolLenDif;		//가장 긴 철근과 짧은 철근이 길이차가 m_nSOGRebarTolLenDif이상이 되면 barset을 나눈다.

	BOOL m_bSOGSpliceDevConditionSet;
	long m_nSOGSpliceDevConditionLength;

	BOOL m_bSOGSpliceAddLengthSet;
	long m_nSOGSpliceAddLengthNumSplice;

	BOOL m_bSOGTopSpacerInfoSet;
	BOOL m_bSOGTopSpacerSet;			//상부 우마철근 설정
	long m_nSOGTopSpacerDiaType;		//직경 0:해당기초직경 1:슬래브 두께
	long m_nSOGTopSpacerSlabThick;		//슬래브 두께
	long m_nSOGTopSpacerSlabThickOverDiaIndex;	//슬래브 두께 이상 직경
	long m_nSOGTopSpacerSlabThickUnderDiaIndex;	//슬래브 두께 미만 직경
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

	BOOL m_bSOGSpacerSupportBarsSet;			//우마지지근 설정
	BOOL m_bSOGSpacerSupportBarsConditionSet;	//우마지지근 적용 조건 설정
	long m_nSOGSpacerSupportBarsCondition;		//우마지지근 적용 조건
	long m_nSOGSpacerSupportBarsDiaType;		//직경 0:상부 우마직경 1:사용자 지정
	long m_nSOGSpacerSupportBarsUserDiaIndex;		//사용자 지정 입력
	long m_nSOGSpacerSupportBarsNumType;			//개수 0:상부 우마 개수 1:사용자 지정
	double m_dSOGSpacerSupportBarsTopSpacerNums;	//상부 우마 개수 입력
	long m_nSOGSpacerSupportBarsUserNums;			//사용자 지정 입력
	long m_nSOGSpacerSupportBarsLengthType;		//길이 0:해당 슬래브 높이X1.4 1:사용자 지정
	double m_dSOGSpacerSupportBarsUserRatio;
	long m_nSOGSpacerSupportBarsUserLength;		//사용자 지정 입력

	long m_nSOGCZoneSpliceWay;
	long m_nSOGCZoneSpliceLength;
	// CircularRamp
	BOOL m_bCircularRampMainBarInfoSet;
	long m_nCircularRampMaxDualHookLength;	//최대 양후크길이
	long m_nCircularRampSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nCircularRampSpliceType;	//이음타입 0:인장이음 1:압축이음
		
	BOOL m_bCircularRampEndInfoSet;
	long m_nCircularRampTopEndHookType;		//상부근 단부 0:정착 1:후크 
	long m_nCircularRampBotEndHookType;		//하부근 단부 0:정착 1:후크 
	long m_nCircularRampIsoFootDevType;		//독립기초 & Panel 접합부위 하부근 0:Passing 1:인장정착
	long m_nCircularRampSpliceDevSlabType;	//이음/정착 적용	0:기초	1:슬래브

	long m_nCircularRampTopEndDevTypeUserLength;
	long m_nCircularRampBotEndDevTypeUserLength;
	long m_nCircularRampOpeningTopDevType;
	long m_nCircularRampOpeningBotDevType;

	BOOL m_bCircularRampLevelDifInfoSet;
	long m_nCircularRampLevelDifPlaceShape;		//배근 형상
	long m_nCircularRampLevelDifHaunchPlaceType;	//헌치 배근타입
		
	BOOL m_bCircularRampOpeningInfoSet;
	
	BOOL m_bCircularRampRebarAreaInfoSet;
	long m_nCircularRampRebarTolDist;		//앞뒤 철근의 길이차
	long m_nCircularRampRebarTolLenDif;		//가장 긴 철근과 짧은 철근이 길이차가 m_nCircularRampRebarTolLenDif이상이 되면 barset을 나눈다.

	BOOL m_bCircularRampSpliceDevConditionSet;
	long m_nCircularRampSpliceDevConditionLength;

	BOOL m_bCircularRampSpliceAddLengthSet;
	long m_nCircularRampSpliceAddLengthNumSplice;

	BOOL m_bCircularRampTopSpacerInfoSet;
	BOOL m_bCircularRampTopSpacerSet;			//상부 우마철근 설정
	long m_nCircularRampTopSpacerDiaType;		//직경 0:해당기초직경 1:슬래브 두께
	long m_nCircularRampTopSpacerSlabThick;		//슬래브 두께
	long m_nCircularRampTopSpacerSlabThickOverDiaIndex;	//슬래브 두께 이상 직경
	long m_nCircularRampTopSpacerSlabThickUnderDiaIndex;	//슬래브 두께 미만 직경
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

	BOOL m_bCircularRampSpacerSupportBarsSet;			//우마지지근 설정
	BOOL m_bCircularRampSpacerSupportBarsConditionSet;	//우마지지근 적용 조건 설정
	long m_nCircularRampSpacerSupportBarsCondition;		//우마지지근 적용 조건
	long m_nCircularRampSpacerSupportBarsDiaType;		//직경 0:상부 우마직경 1:사용자 지정
	long m_nCircularRampSpacerSupportBarsUserDiaIndex;		//사용자 지정 입력
	long m_nCircularRampSpacerSupportBarsNumType;			//개수 0:상부 우마 개수 1:사용자 지정
	double m_dCircularRampSpacerSupportBarsTopSpacerNums;	//상부 우마 개수 입력
	long m_nCircularRampSpacerSupportBarsUserNums;			//사용자 지정 입력
	long m_nCircularRampSpacerSupportBarsLengthType;		//길이 0:해당 슬래브 높이X1.4 1:사용자 지정
	double m_dCircularRampSpacerSupportBarsUserRatio;
	long m_nCircularRampSpacerSupportBarsUserLength;		//사용자 지정 입력

	long m_nCircularRampCZoneSpliceWay;
	long m_nCircularRampCZoneSpliceLength;

	// PC Slab
	BOOL m_bPCSlabMainBarInfoSet;
	long m_nPCSlabMaxDualHookLength;
	long m_nPCSlabSpliceWay;
	long m_nPCSlabSpliceType;
		
	long m_nPCSlabTopEndAddBarLxLnxType;
	long m_nPCSlabTopEndAddBarLengthType;
	long m_nPCSlabTopCenAddBarLxLnxType;
	long m_nPCSlabTopCenAddBarLengthType;

	long m_nPCSlabTopEndDevTypeUserLength;
	long m_nPCSlabBotEndDevTypeUserLength;

	BOOL m_bPCSlabShortCenSpliceBarPlace;
	long m_nPCSlabShortCenSpliceBarLengthType;
	BOOL m_bPCSlabShortEndSpliceBarPlace;
	long m_nPCSlabShortEndSpliceBarLengthType;
	BOOL m_bPCSlabLongCenSpliceBarPlace;
	long m_nPCSlabLongCenSpliceBarLengthType;
	BOOL m_bPCSlabLongEndSpliceBarPlace;
	long m_nPCSlabLongEndSpliceBarLengthType;

	BOOL m_bPCSlabUnStructuredEndInfoSet;
	long m_nPCSlabTopUnStructuredEndHookType;
	long m_nPCSlabBotUnStructuredEndHookType;

	BOOL m_bPCSlabLevelDifInfoSet;
	long m_nPCSlabLevelDifPlaceShape;
	long m_nPCSlabLevelDifHaunchPlaceType;

	BOOL m_bPCSlabOpeningInfoSet;
	long m_nPCSlabOpeningTopDevType;
	long m_nPCSlabOpeningBotDevType;

	BOOL m_bPCSlabRebarAreaInfoSet;
	long m_nPCSlabRebarTolDist;
	long m_nPCSlabRebarTolLenDif;

	BOOL m_bPCSlabSpliceDevConditionSet;
	long m_nPCSlabSpliceDevConditionLength;

	BOOL m_bPCSlabSpliceAddLengthSet;
	long m_nPCSlabSpliceAddLengthNumSplice;

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

	map<long, long> mm_PCSlabTopEndAddBarUserLen, mm_PCSlabTopCenAddBarUserLen,
		mm_PCSlabShortCenSpliceBarUserLen, mm_PCSlabShortEndSpliceBarUserLen, mm_PCSlabLongCenSpliceBarUserLen, mm_PCSlabLongEndSpliceBarUserLen;

	// 슬래브 보강근
	BOOL m_bSlabRebarRebarAreaInfoSet;
	long m_nSlabRebarRebarTolDist;		//앞뒤 철근의 길이차
	long m_nSlabRebarRebarTolLenDif;		//가장 긴 철근과 짧은 철근이 길이차가 m_nMatRebarTolLenDif이상이 되면 barset을 나눈다.

	// 철근 일체형 데크 슬래브
	BOOL m_bEmbeddedDeckMainBarInfoSet;
	long m_nEmbeddedDeckMaxDualHookLength;	//최대 양후크길이
	long m_nEmbeddedDeckSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nEmbeddedDeckSpliceType;	//이음타입 0:인장이음 1:압축이음
		
	long m_nEmbeddedDeckTopEndAddBarLxLnxType;
	long m_nEmbeddedDeckTopEndAddBarLengthType;
	long m_nEmbeddedDeckTopCenAddBarLxLnxType;
	long m_nEmbeddedDeckTopCenAddBarLengthType;
		
	BOOL m_bEmbeddedDeckSpliceBarPlace;
	long m_nEmbeddedDeckSpliceBarLengthType;

	BOOL m_bEmbeddedDeckUnStructuredEndInfoSet;
	long m_nEmbeddedDeckTopUnStructuredEndHookType;		//상부근 단부 0:정착 1:후크 
	long m_nEmbeddedDeckBotUnStructuredEndHookType;		//하부근 단부 0:정착 1:후크 
	
	long m_nEmbeddedDeckTopEndDevTypeUserLength;
	long m_nEmbeddedDeckBotEndDevTypeUserLength;
	long m_nEmbeddedDeckOpeningTopDevType;
	long m_nEmbeddedDeckOpeningBotDevType;

	BOOL m_bEmbeddedDeckLevelDifInfoSet;
	long m_nEmbeddedDeckLevelDifPlaceShape;		//배근 형상
	long m_nEmbeddedDeckLevelDifHaunchPlaceType;	//헌치 배근타입

	BOOL m_bEmbeddedDeckOpeningInfoSet;
	
	BOOL m_bEmbeddedDeckRebarAreaInfoSet;
	long m_nEmbeddedDeckRebarTolDist;		//앞뒤 철근의 길이차
	long m_nEmbeddedDeckRebarTolLenDif;		//가장 긴 철근과 짧은 철근이 길이차가 m_nMatRebarTolLenDif이상이 되면 barset을 나눈다.

	BOOL m_bEmbeddedDeckSpliceDevConditionSet;
	long m_nEmbeddedDeckSpliceDevConditionLength;

	BOOL m_bEmbeddedDeckSpliceAddLengthSet;
	long m_nEmbeddedDeckSpliceAddLengthNumSplice;
	

	map<long, long> mm_EmbeddedDeckTopEndAddBarUserLen, mm_EmbeddedDeckTopCenAddBarUserLen,
		mm_EmbeddedDeckSpliceBarUserLen;

	// 일반 데크 슬래브
	BOOL m_bGeneralDeckMainBarInfoSet;
	long m_nGeneralDeckMaxDualHookLength;	//최대 양후크길이
	long m_nGeneralDeckSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nGeneralDeckSpliceType;	//이음타입 0:인장이음 1:압축이음
		
	long m_nGeneralDeckTopEndAddBarLxLnxType;
	long m_nGeneralDeckTopEndAddBarLengthType;
	long m_nGeneralDeckTopCenAddBarLxLnxType;
	long m_nGeneralDeckTopCenAddBarLengthType;
		
	BOOL m_bGeneralDeckSpliceBarPlace;
	long m_nGeneralDeckSpliceBarLengthType;

	BOOL m_bGeneralDeckUnStructuredEndInfoSet;
	long m_nGeneralDeckTopUnStructuredEndHookType;		//상부근 단부 0:정착 1:후크 
	long m_nGeneralDeckBotUnStructuredEndHookType;		//하부근 단부 0:정착 1:후크 
	
	long m_nGeneralDeckTopEndDevTypeUserLength;
	long m_nGeneralDeckBotEndDevTypeUserLength;
	long m_nGeneralDeckOpeningTopDevType;
	long m_nGeneralDeckOpeningBotDevType;

	BOOL m_bGeneralDeckLevelDifInfoSet;
	long m_nGeneralDeckLevelDifPlaceShape;		//배근 형상
	long m_nGeneralDeckLevelDifHaunchPlaceType;	//헌치 배근타입

	BOOL m_bGeneralDeckOpeningInfoSet;
	
	BOOL m_bGeneralDeckRebarAreaInfoSet;
	long m_nGeneralDeckRebarTolDist;		//앞뒤 철근의 길이차
	long m_nGeneralDeckRebarTolLenDif;		//가장 긴 철근과 짧은 철근이 길이차가 m_nMatRebarTolLenDif이상이 되면 barset을 나눈다.

	BOOL m_bGeneralDeckSpliceDevConditionSet;
	long m_nGeneralDeckSpliceDevConditionLength;

	BOOL m_bGeneralDeckSpliceAddLengthSet;
	long m_nGeneralDeckSpliceAddLengthNumSplice;
	

	map<long, long> mm_GeneralDeckTopEndAddBarUserLen, mm_GeneralDeckTopCenAddBarUserLen,
		mm_GeneralDeckSpliceBarUserLen;

	// PC IMT Slab
	BOOL m_bPCIMTSlabSpliceDevConditionSet;
	long m_nPCIMTSlabSpliceDevConditionLength;

	BOOL m_bPCIMTSlabSpliceAddLengthSet;
	long m_nPCIMTSlabSpliceAddLengthNumSplice;

	long m_nPCIMTSlabMaxDualHookLength;	
	long m_nPCIMTSlabSpliceWay;	
	long m_nPCIMTSlabSpliceType;
	
	long m_nPCIMTSlabTopHookType;
	long m_nPCIMTSlabBotHookType;
	long m_nPCIMTSlabTopEndDevTypeUserLength;
	long m_nPCIMTSlabBotEndDevTypeUserLength;
	long m_nPCIMTSlabOpeningTopDevType;
	long m_nPCIMTSlabOpeningBotDevType;

	long m_nPCIMTSlabTopEndAddBarLxLnxType;
	long m_nPCIMTSlabTopEndAddBarLengthType;
	long m_nPCIMTSlabTopCenAddBarLxLnxType;
	long m_nPCIMTSlabTopCenAddBarLengthType;

	BOOL m_bPCIMTSlabShortSpliceBarPlace;
	long m_nPCIMTSlabShortSpliceBarLengthType;
	BOOL m_bPCIMTSlabLongSpliceBarPlace;

	long m_nPCIMTSlabRebarTolDist;
	long m_nPCIMTSlabRebarTolLenDif;

	long m_nPCIMTSlabLongSpliceBarType;
	
	map<long, long> mm_PCIMTSlabTopEndAddBarUserLen, mm_PCIMTSlabTopCenAddBarUserLen;
	map<long, long> mm_PCIMTSlabShortSpliceBarUserLen;
	
	
	// PC HCS Slab
	BOOL m_bPCHCSSlabSpliceDevConditionSet;
	long m_nPCHCSSlabSpliceDevConditionLength;

	BOOL m_bPCHCSSlabSpliceAddLengthSet;
	long m_nPCHCSSlabSpliceAddLengthNumSplice;

	long m_nPCHCSSlabMaxDualHookLength;	
	long m_nPCHCSSlabSpliceWay;	
	long m_nPCHCSSlabSpliceType;
	
	long m_nPCHCSSlabTopHookType;
	long m_nPCHCSSlabBotHookType;
	long m_nPCHCSSlabTopEndDevTypeUserLength;
	long m_nPCHCSSlabBotEndDevTypeUserLength;
	long m_nPCHCSSlabOpeningTopDevType;
	long m_nPCHCSSlabOpeningBotDevType;

	long m_nPCHCSSlabTopEndAddBarLxLnxType;
	long m_nPCHCSSlabTopEndAddBarLengthType;
	long m_nPCHCSSlabTopCenAddBarLxLnxType;
	long m_nPCHCSSlabTopCenAddBarLengthType;
	long m_nPCHCSSlabDevSubBarLengthType;

	long m_nPCHCSSlabShortCenSpliceBarLengthType;
	long m_nPCHCSSlabShortEndSpliceBarLengthType;

	long m_nPCHCSSlabLongCenSpliceBarLengthType;
	long m_nPCHCSSlabLongEndSpliceBarLengthType;

	long m_nPCHCSSlabRebarTolDist;
	long m_nPCHCSSlabRebarTolLenDif;
	
	BOOL m_bPCHCSSlabShortCenSpliceBarPlace;
	BOOL m_bPCHCSSlabShortEndSpliceBarPlace;
	BOOL m_bPCHCSSlabLongCenSpliceBarPlace;
	BOOL m_bPCHCSSlabLongEndSpliceBarPlace;

	map<long, long> mm_PCHCSSlabTopEndAddBarUserLen, mm_PCHCSSlabTopCenAddBarUserLen, mm_PCHCSSlabDevSubBarUserLen;
	map<long, long> mm_PCHCSSlabShortCenSpliceBarUserLen, mm_PCHCSSlabShortEndSpliceBarUserLen;
	map<long, long> mm_PCHCSSlabLongCenSpliceBarUserLen, mm_PCHCSSlabLongEndSpliceBarUserLen;

	//PC RPS Slab
	BOOL m_bPCRPSSlabSpliceDevConditionSet;
	long m_nPCRPSSlabSpliceDevConditionLength;

	BOOL m_bPCRPSSlabSpliceAddLengthSet;
	long m_nPCRPSSlabSpliceAddLengthNumSplice;

	long m_nPCRPSSlabMaxDualHookLength;	
	long m_nPCRPSSlabSpliceWay;	
	long m_nPCRPSSlabSpliceType;
		
	long m_nPCRPSSlabTopHookType;
	long m_nPCRPSSlabBotHookType;
	long m_nPCRPSSlabTopEndDevTypeUserLength;
	long m_nPCRPSSlabBotEndDevTypeUserLength;
	long m_nPCRPSSlabOpeningTopDevType;
	long m_nPCRPSSlabOpeningBotDevType;
		
	long m_nPCRPSSlabTopEndAddBarLxLnxType;
	long m_nPCRPSSlabTopEndAddBarLengthType;
	long m_nPCRPSSlabTopCenAddBarLxLnxType;
	long m_nPCRPSSlabTopCenAddBarLengthType;
	long m_nPCRPSSlabDevSubBarLengthType;

	long m_nPCRPSSlabLongCenSpliceBarLengthType;
	long m_nPCRPSSlabLongEndSpliceBarLengthType;

	long m_nPCRPSSlabRebarTolDist;
	long m_nPCRPSSlabRebarTolLenDif;

	BOOL m_bPCRPSSlabLongCenSpliceBarPlace;
	BOOL m_bPCRPSSlabLongEndSpliceBarPlace;

	map<long, long> mm_PCRPSSlabTopEndAddBarUserLen, mm_PCRPSSlabTopCenAddBarUserLen, 
		mm_PCRPSSlabDevSubBarUserLen, mm_PCRPSSlabLongCenSpliceBarUserLen, mm_PCRPSSlabLongEndSpliceBarUserLen;

	BOOL m_bMatBotSpacerUserSteelGradeS;
	BOOL m_bMatTopSpacerUserSteelGradeS;
	BOOL m_bMatSpacerSupportBarUserSteelGradeS;
	BOOL m_bMatSupportRebarUserSteelGradeS;

	BOOL m_bFlatBotSpacerUserSteelGradeS;
	BOOL m_bFlatTopSpacerUserSteelGradeS;
	BOOL m_bFlatSpacerSupportBarUserSteelGradeS;

	BOOL m_bSlabBotSpacerUserSteelGradeS;
	BOOL m_bSlabTopSpacerUserSteelGradeS;
	BOOL m_bSlabSpacerSupportBarUserSteelGradeS;

	BOOL m_bSOGBotSpacerUserSteelGradeS;
	BOOL m_bSOGTopSpacerUserSteelGradeS;
	BOOL m_bSOGSpacerSupportBarUserSteelGradeS;
	BOOL m_bSOGSupportRebarUserSteelGradeS;

	BOOL m_bCircularRampBotSpacerUserSteelGradeS;
	BOOL m_bCircularRampTopSpacerUserSteelGradeS;
	BOOL m_bCircularRampSpacerSupportBarUserSteelGradeS;

	long m_nEmbeddedDeckCZoneSpliceWay;
	long m_nEmbeddedDeckCZoneSpliceLength;
	long m_nGeneralDeckCZoneSpliceWay;
	long m_nGeneralDeckCZoneSpliceLength;
	long m_nPCSlabCZoneSpliceWay;
	long m_nPCSlabCZoneSpliceLength;
	long m_nPCIMTSlabCZoneSpliceWay;
	long m_nPCIMTSlabCZoneSpliceLength;
	long m_nPCHCSSlabCZoneSpliceWay;
	long m_nPCHCSSlabCZoneSpliceLength;
	long m_nPCRPSSlabCZoneSpliceWay;
	long m_nPCRPSSlabCZoneSpliceLength;


	BOOL m_bMatTopSpacerPlaceConditionSet;
	long m_nMatTopSpacerPlaceCondition;
	BOOL m_bFlatTopSpacerPlaceConditionSet;
	long m_nFlatTopSpacerPlaceCondition;
	BOOL m_bSlabTopSpacerPlaceConditionSet;
	long m_nSlabTopSpacerPlaceCondition;
	BOOL m_bSOGTopSpacerPlaceConditionSet;
	long m_nSOGTopSpacerPlaceCondition;
	BOOL m_bCircularRampTopSpacerPlaceConditionSet;
	long m_nCircularRampTopSpacerPlaceCondition;


	BOOL m_bSlabOpeningAddBarDeductionCondition;
	long m_nSlabOpeningAddBarDeductionPercent;

	map<CString, long> mm_MatThickToDiaIndex;
	map<CString, long> mm_FlatThickToDiaIndex;
	map<CString, long> mm_SlabThickToDiaIndex;
	map<CString, long> mm_SOGThickToDiaIndex;
	map<CString, long> mm_CircularRampThickToDiaIndex;

	virtual map<CString, long> GetMatThickToDiaIndex() { return mm_MatThickToDiaIndex; }
	virtual map<CString, long> GetFlatThickToDiaIndex() { return mm_FlatThickToDiaIndex; }
	virtual map<CString, long> GetSlabThickToDiaIndex() { return mm_SlabThickToDiaIndex; }
	virtual map<CString, long> GetSOGThickToDiaIndex() { return mm_SOGThickToDiaIndex; }
	virtual map<CString, long> GetCircularRampThickToDiaIndex() { return mm_CircularRampThickToDiaIndex; }

	virtual void SetMatThickToDiaIndex(map<CString, long> value);
	virtual void SetFlatThickToDiaIndex(map<CString, long> value);
	virtual void SetSlabThickToDiaIndex(map<CString, long> value);
	virtual void SetSOGThickToDiaIndex(map<CString, long> value);
	virtual void SetCircularRampThickToDiaIndex(map<CString, long> value);

	BOOL m_bSlabLongEndPlaceAreaSet;
	long m_nSlabLongEndPlaceAreaOneWaySlabType;
	long m_nSlabLongEndPlaceAreaTwoWaySlabType;
	BOOL m_bSlabBotAddBarLengthSet;
	long m_nSlabBotAddBarLengthType;
	BOOL m_bSlabBotAddBarCutBarLenExclusionSet;

	long m_nSlabOpeningAddBarDevLengthType;


	virtual long GetSlabOpeningAddBarDevLengthType() { return m_nSlabOpeningAddBarDevLengthType; }
	virtual void SetSlabOpeningAddBarDevLengthType(long value) { m_nSlabOpeningAddBarDevLengthType = value; }
};

