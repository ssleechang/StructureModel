#pragma once
#include "BaseRebarTemplate.h"

class StructureModel_CLASS PitRebarTemplate : public BaseRebarTemplate
{
public:
	PitRebarTemplate();
	~PitRebarTemplate(void);

	void Serialize(CArchive& ar);

	virtual void CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate) override;

	void DataInit();

	virtual BOOL GetPitGeneralInfoSet() { return m_bPitGeneralInfoSet; }
	virtual long GetPitMaxLen2HookAnchorBar() { return m_nPitMaxLen2HookAnchorBar; }

	virtual BOOL GetPitSpliceDevConditionSet() { return m_bPitSpliceDevConditionSet; }
	virtual long GetPitSpliceDevConditionLength() { return m_nPitSpliceDevConditionLength; }

	virtual BOOL GetPitVerInfoSet() { return m_bPitVerInfoSet; }
	virtual long GetPitVerOutTopHookType() { return m_nPitVerOutTopHookType; }
	virtual long GetPitVerOutBotHookType() { return m_nPitVerOutBotHookType; }
	virtual long GetPitVerInTopHookType() { return m_nPitVerInTopHookType; }
	virtual long GetPitVerInBotHookType() { return m_nPitVerInBotHookType; }

	virtual BOOL GetPitHorInfoSet() { return m_bPitHorInfoSet; }
	virtual long GetPitHorOutHookType() { return m_nPitHorOutHookType; }
	virtual long GetPitHorInHookType() { return m_nPitHorInHookType; }

	virtual BOOL GetPitTopSpacerInfoSet() { return m_bPitTopSpacerInfoSet; }
	virtual BOOL GetPitTopSpacerSet() { return m_bPitTopSpacerSet; }
	virtual long GetPitTopSpacerDiaType() { return m_nPitTopSpacerDiaType; }
	virtual long GetPitTopSpacerSlabThick() { return m_nPitTopSpacerSlabThick; }
	virtual long GetPitTopSpacerSlabThickOverDiaIndex() { return m_nPitTopSpacerSlabThickOverDiaIndex; }
	virtual long GetPitTopSpacerSlabThickUnderDiaIndex() { return m_nPitTopSpacerSlabThickUnderDiaIndex; }
	virtual long GetPitTopSpacerSpacingType() { return m_nPitTopSpacerSpacingType; }
	virtual long GetPitTopSpacerSlabSpacing() { return m_nPitTopSpacerSlabSpacing; }
	virtual long GetPitTopSpacerUserSpacing1() { return m_nPitTopSpacerUserSpacing1; }
	virtual long GetPitTopSpacerUserSpacing2() { return m_nPitTopSpacerUserSpacing2; }
	virtual long GetPitTopSpacerHeightType() { return m_nPitTopSpacerHeightType; }
	virtual long GetPitTopSpacerUserHeight() { return m_nPitTopSpacerUserHeight; }
	virtual long GetPitTopSpacerHookLenAType() { return m_nPitTopSpacerHookLenAType; }
	virtual long GetPitTopSpacerMainBarHookLenA() { return m_nPitTopSpacerMainBarHookLenA; }
	virtual long GetPitTopSpacerUserHookLenA() { return m_nPitTopSpacerUserHookLenA; }
	virtual long GetPitTopSpacerHookLenBType() { return m_nPitTopSpacerHookLenBType; }
	virtual long GetPitTopSpacerMainBarHookLenB() { return m_nPitTopSpacerMainBarHookLenB; }
	virtual long GetPitTopSpacerUserHookLenB() { return m_nPitTopSpacerUserHookLenB; }
	virtual long GetPitTopSpacerWidth() { return m_nPitTopSpacerWidth; }

	virtual BOOL GetPitBotSpacerInfoSet() { return m_bPitBotSpacerInfoSet; }
	virtual BOOL GetPitBotSpacerSet() { return m_bPitBotSpacerSet; }
	virtual long GetPitBotSpacerDiaType() { return m_nPitBotSpacerDiaType; }
	virtual long GetPitBotSpacerDiaIndex() { return m_nPitBotSpacerDiaIndex; }
	virtual long GetPitBotSpacerSpacingType() { return m_nPitBotSpacerSpacingType; }
	virtual long GetPitBotSpacerSlabSpacing() { return m_nPitBotSpacerSlabSpacing; }
	virtual long GetPitBotSpacerUserSpacing1() { return m_nPitBotSpacerUserSpacing1; }
	virtual long GetPitBotSpacerUserSpacing2() { return m_nPitBotSpacerUserSpacing2; }
	virtual long GetPitBotSpacerHeightType() { return m_nPitBotSpacerHeightType; }
	virtual long GetPitBotSpacerUserHeight() { return m_nPitBotSpacerUserHeight; }
	virtual long GetPitBotSpacerHookLenAType() { return m_nPitBotSpacerHookLenAType; }
	virtual long GetPitBotSpacerMainBarHookLenA() { return m_nPitBotSpacerMainBarHookLenA; }
	virtual long GetPitBotSpacerUserHookLenA() { return m_nPitBotSpacerUserHookLenA; }
	virtual long GetPitBotSpacerHookLenBType() { return m_nPitBotSpacerHookLenBType; }
	virtual long GetPitBotSpacerMainBarHookLenB() { return m_nPitBotSpacerMainBarHookLenB; }
	virtual long GetPitBotSpacerUserHookLenB() { return m_nPitBotSpacerUserHookLenB; }
	virtual long GetPitBotSpacerWidth() { return m_nPitBotSpacerWidth; }

	virtual BOOL GetPitSpacerSupportBarsSet() { return m_bPitSpacerSupportBarsSet; }
	virtual BOOL GetPitSpacerSupportBarsConditionSet() { return m_bPitSpacerSupportBarsConditionSet; }
	virtual long GetPitSpacerSupportBarsCondition() { return m_nPitSpacerSupportBarsCondition; }
	virtual long GetPitSpacerSupportBarsDiaType() { return m_nPitSpacerSupportBarsDiaType; }
	virtual long GetPitSpacerSupportBarsUserDiaIndex() { return m_nPitSpacerSupportBarsUserDiaIndex; }
	virtual long GetPitSpacerSupportBarsNumType() { return m_nPitSpacerSupportBarsNumType; }
	virtual double GetPitSpacerSupportBarsTopSpacerNums() { return m_dPitSpacerSupportBarsTopSpacerNums; }
	virtual long GetPitSpacerSupportBarsUserNums() { return m_nPitSpacerSupportBarsUserNums; }
	virtual long GetPitSpacerSupportBarsLengthType() { return m_nPitSpacerSupportBarsLengthType; }
	virtual long GetPitSpacerSupportBarsUserLength() { return m_nPitSpacerSupportBarsUserLength; }
	virtual double GetPitSpacerSupportBarsUserRatio() { return m_dPitSpacerSupportBarsUserRatio; }

	virtual BOOL GetPitSupportRebarsSet() { return m_bPitSupportRebarsSet; }
	virtual long GetPitSupportRebarsDiaIndex() { return m_nPitSupportRebarsDiaIndex; }
	virtual double GetPitSupportRebarsSlabLengthRate() { return m_dPitSupportRebarsSlabLengthRate; }
	virtual long GetPitSupportRebarsOrderLength() { return m_nPitSupportRebarsOrderLength; }
	virtual long GetPitSupportRebarsSpliceLength() { return m_nPitSupportRebarsSpliceLength; }
	virtual long GetPitSupportRebarsCorrectiveLength() { return m_nPitSupportRebarsCorrectiveLength; }


	virtual void SetPitGeneralInfoSet(BOOL value) { m_bPitGeneralInfoSet = value; }
	virtual void SetPitMaxLen2HookAnchorBar(long value) { m_nPitMaxLen2HookAnchorBar = value; }

	virtual void SetPitSpliceDevConditionSet(BOOL value) { m_bPitSpliceDevConditionSet = value; }
	virtual void SetPitSpliceDevConditionLength(long value) { m_nPitSpliceDevConditionLength = value; }

	virtual void SetPitVerInfoSet(BOOL value) { m_bPitVerInfoSet = value; }
	virtual void SetPitVerOutTopHookType(long value) { m_nPitVerOutTopHookType = value; }
	virtual void SetPitVerOutBotHookType(long value) { m_nPitVerOutBotHookType = value; }
	virtual void SetPitVerInTopHookType(long value) { m_nPitVerInTopHookType = value; }
	virtual void SetPitVerInBotHookType(long value) { m_nPitVerInBotHookType = value; }

	virtual void SetPitHorInfoSet(BOOL value) { m_bPitHorInfoSet = value; }
	virtual void SetPitHorOutHookType(long value) { m_nPitHorOutHookType = value; }
	virtual void SetPitHorInHookType(long value) { m_nPitHorInHookType = value; }

	virtual void SetPitTopSpacerInfoSet(BOOL value) { m_bPitTopSpacerInfoSet = value; }
	virtual void SetPitTopSpacerSet(BOOL value) { m_bPitTopSpacerSet = value; }
	virtual void SetPitTopSpacerDiaType(long value) { m_nPitTopSpacerDiaType = value; }
	virtual void SetPitTopSpacerSlabThick(long value) { m_nPitTopSpacerSlabThick = value; }
	virtual void SetPitTopSpacerSlabThickOverDiaIndex(long value) { m_nPitTopSpacerSlabThickOverDiaIndex = value; }
	virtual void SetPitTopSpacerSlabThickUnderDiaIndex(long value) { m_nPitTopSpacerSlabThickUnderDiaIndex = value; }
	virtual void SetPitTopSpacerSpacingType(long value) { m_nPitTopSpacerSpacingType = value; }
	virtual void SetPitTopSpacerSlabSpacing(long value) { m_nPitTopSpacerSlabSpacing = value; }
	virtual void SetPitTopSpacerUserSpacing1(long value) { m_nPitTopSpacerUserSpacing1 = value; }
	virtual void SetPitTopSpacerUserSpacing2(long value) { m_nPitTopSpacerUserSpacing2 = value; }
	virtual void SetPitTopSpacerHeightType(long value) { m_nPitTopSpacerHeightType = value; }
	virtual void SetPitTopSpacerUserHeight(long value) { m_nPitTopSpacerUserHeight = value; }
	virtual void SetPitTopSpacerHookLenAType(long value) { m_nPitTopSpacerHookLenAType = value; }
	virtual void SetPitTopSpacerMainBarHookLenA(long value) { m_nPitTopSpacerMainBarHookLenA = value; }
	virtual void SetPitTopSpacerUserHookLenA(long value) { m_nPitTopSpacerUserHookLenA = value; }
	virtual void SetPitTopSpacerHookLenBType(long value) { m_nPitTopSpacerHookLenBType = value; }
	virtual void SetPitTopSpacerMainBarHookLenB(long value) { m_nPitTopSpacerMainBarHookLenB = value; }
	virtual void SetPitTopSpacerUserHookLenB(long value) { m_nPitTopSpacerUserHookLenB = value; }
	virtual void SetPitTopSpacerWidth(long value) { m_nPitTopSpacerWidth = value; }

	virtual void SetPitBotSpacerInfoSet(BOOL value) { m_bPitBotSpacerInfoSet = value; }
	virtual void SetPitBotSpacerSet(BOOL value) { m_bPitBotSpacerSet = value; }
	virtual void SetPitBotSpacerDiaType(long value) { m_nPitBotSpacerDiaType = value; }
	virtual void SetPitBotSpacerDiaIndex(long value) { m_nPitBotSpacerDiaIndex = value; }
	virtual void SetPitBotSpacerSpacingType(long value) { m_nPitBotSpacerSpacingType = value; }
	virtual void SetPitBotSpacerSlabSpacing(long value) { m_nPitBotSpacerSlabSpacing = value; }
	virtual void SetPitBotSpacerUserSpacing1(long value) { m_nPitBotSpacerUserSpacing1 = value; }
	virtual void SetPitBotSpacerUserSpacing2(long value) { m_nPitBotSpacerUserSpacing2 = value; }
	virtual void SetPitBotSpacerHeightType(long value) { m_nPitBotSpacerHeightType = value; }
	virtual void SetPitBotSpacerUserHeight(long value) { m_nPitBotSpacerUserHeight = value; }
	virtual void SetPitBotSpacerHookLenAType(long value) { m_nPitBotSpacerHookLenAType = value; }
	virtual void SetPitBotSpacerMainBarHookLenA(long value) { m_nPitBotSpacerMainBarHookLenA = value; }
	virtual void SetPitBotSpacerUserHookLenA(long value) { m_nPitBotSpacerUserHookLenA = value; }
	virtual void SetPitBotSpacerHookLenBType(long value) { m_nPitBotSpacerHookLenBType = value; }
	virtual void SetPitBotSpacerMainBarHookLenB(long value) { m_nPitBotSpacerMainBarHookLenB = value; }
	virtual void SetPitBotSpacerUserHookLenB(long value) { m_nPitBotSpacerUserHookLenB = value; }
	virtual void SetPitBotSpacerWidth(long value) { m_nPitBotSpacerWidth = value; }

	virtual void SetPitSpacerSupportBarsSet(BOOL value) { m_bPitSpacerSupportBarsSet = value; }
	virtual void SetPitSpacerSupportBarsConditionSet(BOOL value) { m_bPitSpacerSupportBarsConditionSet = value; }
	virtual void SetPitSpacerSupportBarsCondition(long value) { m_nPitSpacerSupportBarsCondition = value; }
	virtual void SetPitSpacerSupportBarsDiaType(long value) { m_nPitSpacerSupportBarsDiaType = value; }
	virtual void SetPitSpacerSupportBarsUserDiaIndex(long value) { m_nPitSpacerSupportBarsUserDiaIndex = value; }
	virtual void SetPitSpacerSupportBarsNumType(long value) { m_nPitSpacerSupportBarsNumType = value; }
	virtual void SetPitSpacerSupportBarsTopSpacerNums(double value) { m_dPitSpacerSupportBarsTopSpacerNums = value; }
	virtual void SetPitSpacerSupportBarsUserNums(long value) { m_nPitSpacerSupportBarsUserNums = value; }
	virtual void SetPitSpacerSupportBarsLengthType(long value) { m_nPitSpacerSupportBarsLengthType = value; }
	virtual void SetPitSpacerSupportBarsUserLength(long value) { m_nPitSpacerSupportBarsUserLength = value; }
	virtual void SetPitSpacerSupportBarsUserRatio(double value) { m_dPitSpacerSupportBarsUserRatio = value; }

	virtual void SetPitSupportRebarsSet(BOOL value) { m_bPitSupportRebarsSet = value; }
	virtual void SetPitSupportRebarsDiaIndex(long value) { m_nPitSupportRebarsDiaIndex = value; }
	virtual void SetPitSupportRebarsSlabLengthRate(double value) { m_dPitSupportRebarsSlabLengthRate = value; }
	virtual void SetPitSupportRebarsOrderLength(long value) { m_nPitSupportRebarsOrderLength = value; }
	virtual void SetPitSupportRebarsSpliceLength(long value) { m_nPitSupportRebarsSpliceLength = value; }
	virtual void SetPitSupportRebarsCorrectiveLength(long value) { m_nPitSupportRebarsCorrectiveLength = value; }

	virtual long GetPitHorOutHookLengthType(){ return m_nPitHorOutHookLengthType; }
	virtual void SetPitHorOutHookLengthType(long value) { m_nPitHorOutHookLengthType = value; }
	virtual map<long, long> GetPitUserOutHorBarHookLenMap(){ return mm_PitUserOutHorBarHookLen; }
	virtual void SetPitUserOutHorBarHookLenMap(map<long, long> newVal) { mm_PitUserOutHorBarHookLen = newVal; }

	virtual long GetPitUserOutHorBarHookLen(double dDia);
	virtual void SetPitUserOutHorBarHookLen(long key, long value) { mm_PitUserOutHorBarHookLen[key] = value; }

	virtual BOOL GetPitBotSpacerUserSteelGradeS() { return m_bPitBotSpacerUserSteelGradeS; }
	virtual BOOL GetPitTopSpacerUserSteelGradeS() { return m_bPitTopSpacerUserSteelGradeS; }
	virtual BOOL GetPitSpacerSupportBarUserSteelGradeS() { return m_bPitSpacerSupportBarUserSteelGradeS; }
	virtual BOOL GetPitSupportRebarUserSteelGradeS() { return m_bPitSupportRebarUserSteelGradeS; }
	
	virtual void SetPitBotSpacerUserSteelGradeS(BOOL value) { m_bPitBotSpacerUserSteelGradeS = value; }
	virtual void SetPitTopSpacerUserSteelGradeS(BOOL value) { m_bPitTopSpacerUserSteelGradeS = value; }
	virtual void SetPitSpacerSupportBarUserSteelGradeS(BOOL value) { m_bPitSpacerSupportBarUserSteelGradeS = value; }
	virtual void SetPitSupportRebarUserSteelGradeS(BOOL value) { m_bPitSupportRebarUserSteelGradeS = value; }

	virtual BOOL GetPitWellAddBarSet() { return m_bPitWellAddBarSet; }
	virtual long GetPitWellAddBarDiaIndex() { return m_nPitWellAddBarDiaIndex; }
	virtual BOOL GetPitWellAddBarUserSteelGradeS() { return m_bPitWellAddBarUserSteelGradeS; }
	virtual long GetPitWellAddBarNum() { return m_nPitWellAddBarNum; }
	virtual long GetPitWellAddBarDevType() { return m_nPitWellAddBarDevType; }
	virtual long GetPitWellAddBarDevUserLength() { return m_nPitWellAddBarDevUserLength; }

	virtual void SetPitWellAddBarSet(BOOL value) { m_bPitWellAddBarSet = value; }
	virtual void SetPitWellAddBarDiaIndex(long value) { m_nPitWellAddBarDiaIndex = value; }
	virtual void SetPitWellAddBarUserSteelGradeS(BOOL value) { m_bPitWellAddBarUserSteelGradeS = value; }
	virtual void SetPitWellAddBarNum(long value) { m_nPitWellAddBarNum = value; }
	virtual void SetPitWellAddBarDevType(long value) { m_nPitWellAddBarDevType = value; }
	virtual void SetPitWellAddBarDevUserLength(long value) { m_nPitWellAddBarDevUserLength = value; }


	virtual BOOL GetPitWallSpacerInfoSet() { return m_bPitWallSpacerInfoSet; }
	virtual long GetPitWallSpacerDiaType() { return m_nPitWallSpacerDiaType; }
	virtual long GetPitWallSpacerDiaIndex() { return m_nPitWallSpacerDiaIndex; }
	virtual BOOL GetPitWallSpacerUserSteelGradeS() { return m_bPitWallSpacerUserSteelGradeS; }
	virtual long GetPitWallSpacerHorSpacing() { return m_nPitWallSpacerHorSpacing; }
	virtual long GetPitWallSpacerVerSpacingType() { return m_nPitWallSpacerVerSpacingType; }
	virtual long GetPitWallSpacerVerSpacing() { return m_nPitWallSpacerVerSpacing; }
	virtual long GetPitWallSpacerVerSpacingRate() { return m_nPitWallSpacerVerSpacingRate; }
	virtual long GetPitWallSpacerVerSpacingCount() { return m_nPitWallSpacerVerSpacingCount; }
	virtual long GetPitWallSpacerLenType() { return m_nPitWallSpacerLenType; }
	virtual long GetPitWallSpacerLenCalWayA() { return m_nPitWallSpacerLenCalWayA; }
	virtual long GetPitWallSpacerLenCalWayB() { return m_nPitWallSpacerLenCalWayB; }
	virtual long GetPitWallSpacerLenA() { return m_nPitWallSpacerLenA; }
	virtual long GetPitWallSpacerLenB() { return m_nPitWallSpacerLenB; }
	virtual long GetPitWallSpacerShape() { return m_nPitWallSpacerShape; }

	virtual void SetPitWallSpacerInfoSet(BOOL value) { m_bPitWallSpacerInfoSet = value; }
	virtual void SetPitWallSpacerDiaType(long value) { m_nPitWallSpacerDiaType = value; }
	virtual void SetPitWallSpacerDiaIndex(long value) { m_nPitWallSpacerDiaIndex = value; }
	virtual void SetPitWallSpacerUserSteelGradeS(BOOL value) { m_bPitWallSpacerUserSteelGradeS = value; }
	virtual void SetPitWallSpacerHorSpacing(long value) { m_nPitWallSpacerHorSpacing = value; }
	virtual void SetPitWallSpacerVerSpacingType(long value) { m_nPitWallSpacerVerSpacingType = value; }
	virtual void SetPitWallSpacerVerSpacing(long value) { m_nPitWallSpacerVerSpacing = value; }
	virtual void SetPitWallSpacerVerSpacingRate(long value) { m_nPitWallSpacerVerSpacingRate = value; }
	virtual void SetPitWallSpacerVerSpacingCount(long value) { m_nPitWallSpacerVerSpacingCount = value; }
	virtual void SetPitWallSpacerLenType(long value) { m_nPitWallSpacerLenType = value; }
	virtual void SetPitWallSpacerLenCalWayA(long value) { m_nPitWallSpacerLenCalWayA = value; }
	virtual void SetPitWallSpacerLenCalWayB(long value) { m_nPitWallSpacerLenCalWayB = value; }
	virtual void SetPitWallSpacerLenA(long value) { m_nPitWallSpacerLenA = value; }
	virtual void SetPitWallSpacerLenB(long value) { m_nPitWallSpacerLenB = value; }
	virtual void SetPitWallSpacerShape(long value) { m_nPitWallSpacerShape = value; }

	virtual BOOL GetPitTopSpacerPlaceConditionSet() { return m_bPitTopSpacerPlaceConditionSet; }
	virtual long GetPitTopSpacerPlaceCondition() { return m_nPitTopSpacerPlaceCondition; }

	virtual void SetPitTopSpacerPlaceConditionSet(BOOL value) { m_bPitTopSpacerPlaceConditionSet = value; }
	virtual void SetPitTopSpacerPlaceCondition(long value) { m_nPitTopSpacerPlaceCondition = value; }

	virtual BOOL GetPitPlaceBraceSupportBar() { return m_bPitPlaceBraceSupportBar; }
	virtual long GetPitBraceSupportBarDiaIndex() { return m_nPitBraceSupportBarDiaIndex; }
	virtual BOOL GetPitBraceSupportBarUserSteelGradeS() { return m_bPitBraceSupportBarUserSteelGradeS; }
	virtual long GetPitBraceSupportBarNumIndex() { return m_nPitBraceSupportBarNumIndex; }

	virtual void SetPitPlaceBraceSupportBar(BOOL value) { m_bPitPlaceBraceSupportBar = value; }
	virtual void SetPitBraceSupportBarDiaIndex(long value) { m_nPitBraceSupportBarDiaIndex = value; }
	virtual void SetPitBraceSupportBarUserSteelGradeS(BOOL value) { m_bPitBraceSupportBarUserSteelGradeS = value; }
	virtual void SetPitBraceSupportBarNumIndex(long value) { m_nPitBraceSupportBarNumIndex = value; }

	virtual BOOL GetPitSpliceAddLengthSet() { return m_bPitSpliceAddLengthSet; }
	virtual void SetPitSpliceAddLengthSet(BOOL value) { m_bPitSpliceAddLengthSet = value; }
	virtual long GetPitSpliceAddLengthNumSplice() { return m_nPitSpliceAddLengthNumSplice; }
	virtual void SetPitSpliceAddLengthNumSplice(long value) { m_nPitSpliceAddLengthNumSplice = value; }
private:
	BOOL m_bPitGeneralInfoSet;
	long m_nPitMaxLen2HookAnchorBar;	//수평근 최대 양후크 길이

	BOOL m_bPitSpliceDevConditionSet;
	long m_nPitSpliceDevConditionLength;

	BOOL m_bPitVerInfoSet;
	long m_nPitVerOutTopHookType;	//수직근 설정 외부 상부
	long m_nPitVerOutBotHookType;	//수직근 설정 외부 하부
	long m_nPitVerInTopHookType;	//수직근 설정 내부 상부
	long m_nPitVerInBotHookType;	//수직근 설정 내부 하부

	BOOL m_bPitHorInfoSet;
	long m_nPitHorOutHookType;		//수평근 설정 외부
	long m_nPitHorOutHookLengthType;
	long m_nPitHorInHookType;		//수평근 설정 내부

	BOOL m_bPitTopSpacerInfoSet;
	BOOL m_bPitTopSpacerSet;			//상부 우마철근 설정
	long m_nPitTopSpacerDiaType;		//직경 0:해당기초직경 1:슬래브 두께
	long m_nPitTopSpacerSlabThick;		//슬래브 두께
	long m_nPitTopSpacerSlabThickOverDiaIndex;	//슬래브 두께 이상 직경
	long m_nPitTopSpacerSlabThickUnderDiaIndex;	//슬래브 두께 미만 직경
	long m_nPitTopSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nPitTopSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nPitTopSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nPitTopSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nPitTopSpacerHeightType;		//높이
	long m_nPitTopSpacerUserHeight;		//높이 : 사용자 입력
	long m_nPitTopSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nPitTopSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nPitTopSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nPitTopSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nPitTopSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nPitTopSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nPitTopSpacerWidth;			//폭

	BOOL m_bPitBotSpacerInfoSet;
	BOOL m_bPitBotSpacerSet;			//상부 우마철근 설정
	long m_nPitBotSpacerDiaType;		//직경 0:해당기초직경 1:사용자직경
	long m_nPitBotSpacerDiaIndex;		//직경 : 사용자 직경 입력
	long m_nPitBotSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nPitBotSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nPitBotSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nPitBotSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nPitBotSpacerHeightType;		//높이
	long m_nPitBotSpacerUserHeight;		//높이 : 사용자 입력
	long m_nPitBotSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nPitBotSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nPitBotSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nPitBotSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nPitBotSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nPitBotSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nPitBotSpacerWidth;			//폭

	BOOL m_bPitSpacerSupportBarsSet;			//우마지지근 설정
	BOOL m_bPitSpacerSupportBarsConditionSet;	//우마지지근 적용 조건 설정
	long m_nPitSpacerSupportBarsCondition;		//우마지지근 적용 조건
	long m_nPitSpacerSupportBarsDiaType;		//직경 0:상부 우마직경 1:사용자 지정
	long m_nPitSpacerSupportBarsUserDiaIndex;		//사용자 지정 입력
	long m_nPitSpacerSupportBarsNumType;			//개수 0:상부 우마 개수 1:사용자 지정
	double m_dPitSpacerSupportBarsTopSpacerNums;	//상부 우마 개수 입력
	long m_nPitSpacerSupportBarsUserNums;			//사용자 지정 입력
	long m_nPitSpacerSupportBarsLengthType;		//길이 0:해당 슬래브 높이x비율 1:사용자 지정
	double m_dPitSpacerSupportBarsUserRatio;		//사용자 지정 비율
	long m_nPitSpacerSupportBarsUserLength;		//사용자 지정 입력

	BOOL m_bPitSupportRebarsSet;				//도리근 설정
	long m_nPitSupportRebarsDiaIndex;			//도리근 직경
	double m_dPitSupportRebarsSlabLengthRate;	//해당 슬래브의 둘레길이 비율
	long m_nPitSupportRebarsOrderLength;		//도리근 주문길이
	long m_nPitSupportRebarsSpliceLength;		//도리근 이음길이
	long m_nPitSupportRebarsCorrectiveLength;	//도리근 보정길이

	map<long, long> mm_PitUserOutHorBarHookLen;

	BOOL m_bPitBotSpacerUserSteelGradeS;
	BOOL m_bPitTopSpacerUserSteelGradeS;
	BOOL m_bPitSpacerSupportBarUserSteelGradeS;
	BOOL m_bPitSupportRebarUserSteelGradeS;

	BOOL m_bPitWellAddBarSet;
	long m_nPitWellAddBarDiaIndex;
	BOOL m_bPitWellAddBarUserSteelGradeS;
	long m_nPitWellAddBarNum;

	long m_nPitWellAddBarDevType;
	long m_nPitWellAddBarDevUserLength;


	BOOL m_bPitWallSpacerInfoSet;
	long m_nPitWallSpacerDiaType;				//직경설정	0:수평근 직경에 따름 1:직경
	long m_nPitWallSpacerDiaIndex;			//직경Index
	BOOL m_bPitWallSpacerUserSteelGradeS;
	long m_nPitWallSpacerHorSpacing;			//수평간격
	long m_nPitWallSpacerVerSpacingType;		//폭 고정근 수직간격 타입	0:간격 1:비율 2:개수
	long m_nPitWallSpacerVerSpacing;			//수직간격
	long m_nPitWallSpacerVerSpacingRate;		//수직비율
	long m_nPitWallSpacerVerSpacingCount;		//수직갯수	
	long m_nPitWallSpacerLenType;				//발길이 타입	0:인장이음 1:후크
	long m_nPitWallSpacerLenCalWayA;			//발길이 계산 방법	0:표준 1:사용자 정의
	long m_nPitWallSpacerLenCalWayB;			//발길이 계산 방법	0:표준 1:사용자 정의
	long m_nPitWallSpacerLenA;				//사용자 정의 길이A
	long m_nPitWallSpacerLenB;				//사용자 정의 길이B
	long m_nPitWallSpacerShape;				//형상

	BOOL m_bPitTopSpacerPlaceConditionSet;
	long m_nPitTopSpacerPlaceCondition;


	BOOL m_bPitPlaceBraceSupportBar;
	long m_nPitBraceSupportBarDiaIndex;
	BOOL m_bPitBraceSupportBarUserSteelGradeS;
	long m_nPitBraceSupportBarNumIndex;

	BOOL m_bPitSpliceAddLengthSet;
	long m_nPitSpliceAddLengthNumSplice;

	//BOOL m_bBWallBraceSupportNotPlaceConditionDiaSet;
	//long m_nBWallBraceSupportNotPlaceConditionDiaIndex;
	//BOOL m_bBWallBraceSupportNotPlaceConditionLengthSet;
	//long m_nBWallBraceSupportNotPlaceConditionLength;
	//BOOL m_bBWallBraceSupportNotPlaceConditionBottomDowel;
	//BOOL m_bBWallBraceSupportNotPlaceConditionHeightSet;
	//long m_nBWallBraceSupportNotPlaceConditionHeight;
};

