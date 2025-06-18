#pragma once
#include "BaseRebarTemplate.h"

class StructureModel_CLASS IsoFootRebarTemplate : public BaseRebarTemplate
{
public:
	IsoFootRebarTemplate();
	~IsoFootRebarTemplate(void);

	void Serialize(CArchive& ar);

	virtual void CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate) override;

	void DataInit();

	virtual BOOL GetIsoFootSpliceDevConditionSet() { return m_bIsoFootSpliceDevConditionSet; }
	virtual long GetIsoFootSpliceDevConditionLength() { return m_nIsoFootSpliceDevConditionLength; }

	virtual BOOL GetIsoFootTopSpacerInfoSet() { return m_bIsoFootTopSpacerInfoSet; }
	virtual BOOL GetIsoFootTopSpacerSet() { return m_bIsoFootTopSpacerSet; }
	virtual long GetIsoFootTopSpacerDiaType() { return m_nIsoFootTopSpacerDiaType; }
	virtual long GetIsoFootTopSpacerSlabThick() { return m_nIsoFootTopSpacerSlabThick; }
	virtual long GetIsoFootTopSpacerSlabThickOverDiaIndex() { return m_nIsoFootTopSpacerSlabThickOverDiaIndex; }
	virtual long GetIsoFootTopSpacerSlabThickUnderDiaIndex() { return m_nIsoFootTopSpacerSlabThickUnderDiaIndex; }
	virtual long GetIsoFootTopSpacerSpacingType() { return m_nIsoFootTopSpacerSpacingType; }
	virtual long GetIsoFootTopSpacerSlabSpacing() { return m_nIsoFootTopSpacerSlabSpacing; }
	virtual long GetIsoFootTopSpacerUserSpacing1() { return m_nIsoFootTopSpacerUserSpacing1; }
	virtual long GetIsoFootTopSpacerUserSpacing2() { return m_nIsoFootTopSpacerUserSpacing2; }
	virtual long GetIsoFootTopSpacerHeightType() { return m_nIsoFootTopSpacerHeightType; }
	virtual long GetIsoFootTopSpacerUserHeight() { return m_nIsoFootTopSpacerUserHeight; }
	virtual long GetIsoFootTopSpacerHookLenAType() { return m_nIsoFootTopSpacerHookLenAType; }
	virtual long GetIsoFootTopSpacerMainBarHookLenA() { return m_nIsoFootTopSpacerMainBarHookLenA; }
	virtual long GetIsoFootTopSpacerUserHookLenA() { return m_nIsoFootTopSpacerUserHookLenA; }
	virtual long GetIsoFootTopSpacerHookLenBType() { return m_nIsoFootTopSpacerHookLenBType; }
	virtual long GetIsoFootTopSpacerMainBarHookLenB() { return m_nIsoFootTopSpacerMainBarHookLenB; }
	virtual long GetIsoFootTopSpacerUserHookLenB() { return m_nIsoFootTopSpacerUserHookLenB; }
	virtual long GetIsoFootTopSpacerWidth() { return m_nIsoFootTopSpacerWidth; }

	virtual BOOL GetIsoFootBotSpacerInfoSet() { return m_bIsoFootBotSpacerInfoSet; }
	virtual BOOL GetIsoFootBotSpacerSet() { return m_bIsoFootBotSpacerSet; }
	virtual long GetIsoFootBotSpacerDiaType() { return m_nIsoFootBotSpacerDiaType; }
	virtual long GetIsoFootBotSpacerDiaIndex() { return m_nIsoFootBotSpacerDiaIndex; }
	virtual long GetIsoFootBotSpacerSpacingType() { return m_nIsoFootBotSpacerSpacingType; }
	virtual long GetIsoFootBotSpacerSlabSpacing() { return m_nIsoFootBotSpacerSlabSpacing; }
	virtual long GetIsoFootBotSpacerUserSpacing1() { return m_nIsoFootBotSpacerUserSpacing1; }
	virtual long GetIsoFootBotSpacerUserSpacing2() { return m_nIsoFootBotSpacerUserSpacing2; }
	virtual long GetIsoFootBotSpacerHeightType() { return m_nIsoFootBotSpacerHeightType; }
	virtual long GetIsoFootBotSpacerUserHeight() { return m_nIsoFootBotSpacerUserHeight; }
	virtual long GetIsoFootBotSpacerHookLenAType() { return m_nIsoFootBotSpacerHookLenAType; }
	virtual long GetIsoFootBotSpacerMainBarHookLenA() { return m_nIsoFootBotSpacerMainBarHookLenA; }
	virtual long GetIsoFootBotSpacerUserHookLenA() { return m_nIsoFootBotSpacerUserHookLenA; }
	virtual long GetIsoFootBotSpacerHookLenBType() { return m_nIsoFootBotSpacerHookLenBType; }
	virtual long GetIsoFootBotSpacerMainBarHookLenB() { return m_nIsoFootBotSpacerMainBarHookLenB; }
	virtual long GetIsoFootBotSpacerUserHookLenB() { return m_nIsoFootBotSpacerUserHookLenB; }
	virtual long GetIsoFootBotSpacerWidth() { return m_nIsoFootBotSpacerWidth; }

	virtual BOOL GetIsoFootSpacerSupportBarsSet() { return m_bIsoFootSpacerSupportBarsSet; }
	virtual BOOL GetIsoFootSpacerSupportBarsConditionSet() { return m_bIsoFootSpacerSupportBarsConditionSet; }
	virtual long GetIsoFootSpacerSupportBarsCondition() { return m_nIsoFootSpacerSupportBarsCondition; }
	virtual long GetIsoFootSpacerSupportBarsDiaType() { return m_nIsoFootSpacerSupportBarsDiaType; }
	virtual long GetIsoFootSpacerSupportBarsUserDiaIndex() { return m_nIsoFootSpacerSupportBarsUserDiaIndex; }
	virtual long GetIsoFootSpacerSupportBarsNumType() { return m_nIsoFootSpacerSupportBarsNumType; }
	virtual double GetIsoFootSpacerSupportBarsTopSpacerNums() { return m_dIsoFootSpacerSupportBarsTopSpacerNums; }
	virtual long GetIsoFootSpacerSupportBarsUserNums() { return m_nIsoFootSpacerSupportBarsUserNums; }
	virtual long GetIsoFootSpacerSupportBarsLengthType() { return m_nIsoFootSpacerSupportBarsLengthType; }
	virtual long GetIsoFootSpacerSupportBarsUserLength() { return m_nIsoFootSpacerSupportBarsUserLength; }
	virtual double GetIsoFootSpacerSupportBarsUserRatio() { return m_dIsoFootSpacerSupportBarsUserRatio; }

	virtual BOOL GetIsoFootSupportRebarsSet() { return m_bIsoFootSupportRebarsSet; }
	virtual long GetIsoFootSupportRebarsDiaIndex() { return m_nIsoFootSupportRebarsDiaIndex; }
	virtual double GetIsoFootSupportRebarsSlabLengthRate() { return m_dIsoFootSupportRebarsSlabLengthRate; }
	virtual long GetIsoFootSupportRebarsOrderLength() { return m_nIsoFootSupportRebarsOrderLength; }
	virtual long GetIsoFootSupportRebarsSpliceLength() { return m_nIsoFootSupportRebarsSpliceLength; }
	virtual long GetIsoFootSupportRebarsCorrectiveLength() { return m_nIsoFootSupportRebarsCorrectiveLength; }


	virtual void SetIsoFootSpliceDevConditionSet(BOOL value) { m_bIsoFootSpliceDevConditionSet = value; }
	virtual void SetIsoFootSpliceDevConditionLength(long value) { m_nIsoFootSpliceDevConditionLength = value; }

	virtual void SetIsoFootTopSpacerInfoSet(BOOL value) { m_bIsoFootTopSpacerInfoSet = value; }
	virtual void SetIsoFootTopSpacerSet(BOOL value) { m_bIsoFootTopSpacerSet = value; }
	virtual void SetIsoFootTopSpacerDiaType(long value) { m_nIsoFootTopSpacerDiaType = value; }
	virtual void SetIsoFootTopSpacerSlabThick(long value) { m_nIsoFootTopSpacerSlabThick = value; }
	virtual void SetIsoFootTopSpacerSlabThickOverDiaIndex(long value) { m_nIsoFootTopSpacerSlabThickOverDiaIndex = value; }
	virtual void SetIsoFootTopSpacerSlabThickUnderDiaIndex(long value) { m_nIsoFootTopSpacerSlabThickUnderDiaIndex = value; }
	virtual void SetIsoFootTopSpacerSpacingType(long value) { m_nIsoFootTopSpacerSpacingType = value; }
	virtual void SetIsoFootTopSpacerSlabSpacing(long value) { m_nIsoFootTopSpacerSlabSpacing = value; }
	virtual void SetIsoFootTopSpacerUserSpacing1(long value) { m_nIsoFootTopSpacerUserSpacing1 = value; }
	virtual void SetIsoFootTopSpacerUserSpacing2(long value) { m_nIsoFootTopSpacerUserSpacing2 = value; }
	virtual void SetIsoFootTopSpacerHeightType(long value) { m_nIsoFootTopSpacerHeightType = value; }
	virtual void SetIsoFootTopSpacerUserHeight(long value) { m_nIsoFootTopSpacerUserHeight = value; }
	virtual void SetIsoFootTopSpacerHookLenAType(long value) { m_nIsoFootTopSpacerHookLenAType = value; }
	virtual void SetIsoFootTopSpacerMainBarHookLenA(long value) { m_nIsoFootTopSpacerMainBarHookLenA = value; }
	virtual void SetIsoFootTopSpacerUserHookLenA(long value) { m_nIsoFootTopSpacerUserHookLenA = value; }
	virtual void SetIsoFootTopSpacerHookLenBType(long value) { m_nIsoFootTopSpacerHookLenBType = value; }
	virtual void SetIsoFootTopSpacerMainBarHookLenB(long value) { m_nIsoFootTopSpacerMainBarHookLenB = value; }
	virtual void SetIsoFootTopSpacerUserHookLenB(long value) { m_nIsoFootTopSpacerUserHookLenB = value; }
	virtual void SetIsoFootTopSpacerWidth(long value) { m_nIsoFootTopSpacerWidth = value; }

	virtual void SetIsoFootBotSpacerInfoSet(BOOL value) { m_bIsoFootBotSpacerInfoSet = value; }
	virtual void SetIsoFootBotSpacerSet(BOOL value) { m_bIsoFootBotSpacerSet = value; }
	virtual void SetIsoFootBotSpacerDiaType(long value) { m_nIsoFootBotSpacerDiaType = value; }
	virtual void SetIsoFootBotSpacerDiaIndex(long value) { m_nIsoFootBotSpacerDiaIndex = value; }
	virtual void SetIsoFootBotSpacerSpacingType(long value) { m_nIsoFootBotSpacerSpacingType = value; }
	virtual void SetIsoFootBotSpacerSlabSpacing(long value) { m_nIsoFootBotSpacerSlabSpacing = value; }
	virtual void SetIsoFootBotSpacerUserSpacing1(long value) { m_nIsoFootBotSpacerUserSpacing1 = value; }
	virtual void SetIsoFootBotSpacerUserSpacing2(long value) { m_nIsoFootBotSpacerUserSpacing2 = value; }
	virtual void SetIsoFootBotSpacerHeightType(long value) { m_nIsoFootBotSpacerHeightType = value; }
	virtual void SetIsoFootBotSpacerUserHeight(long value) { m_nIsoFootBotSpacerUserHeight = value; }
	virtual void SetIsoFootBotSpacerHookLenAType(long value) { m_nIsoFootBotSpacerHookLenAType = value; }
	virtual void SetIsoFootBotSpacerMainBarHookLenA(long value) { m_nIsoFootBotSpacerMainBarHookLenA = value; }
	virtual void SetIsoFootBotSpacerUserHookLenA(long value) { m_nIsoFootBotSpacerUserHookLenA = value; }
	virtual void SetIsoFootBotSpacerHookLenBType(long value) { m_nIsoFootBotSpacerHookLenBType = value; }
	virtual void SetIsoFootBotSpacerMainBarHookLenB(long value) { m_nIsoFootBotSpacerMainBarHookLenB = value; }
	virtual void SetIsoFootBotSpacerUserHookLenB(long value) { m_nIsoFootBotSpacerUserHookLenB = value; }
	virtual void SetIsoFootBotSpacerWidth(long value) { m_nIsoFootBotSpacerWidth = value; }

	virtual void SetIsoFootSpacerSupportBarsSet(BOOL value) { m_bIsoFootSpacerSupportBarsSet = value; }
	virtual void SetIsoFootSpacerSupportBarsConditionSet(BOOL value) { m_bIsoFootSpacerSupportBarsConditionSet = value; }
	virtual void SetIsoFootSpacerSupportBarsCondition(long value) { m_nIsoFootSpacerSupportBarsCondition = value; }
	virtual void SetIsoFootSpacerSupportBarsDiaType(long value) { m_nIsoFootSpacerSupportBarsDiaType = value; }
	virtual void SetIsoFootSpacerSupportBarsUserDiaIndex(long value) { m_nIsoFootSpacerSupportBarsUserDiaIndex = value; }
	virtual void SetIsoFootSpacerSupportBarsNumType(long value) { m_nIsoFootSpacerSupportBarsNumType = value; }
	virtual void SetIsoFootSpacerSupportBarsTopSpacerNums(double value) { m_dIsoFootSpacerSupportBarsTopSpacerNums = value; }
	virtual void SetIsoFootSpacerSupportBarsUserNums(long value) { m_nIsoFootSpacerSupportBarsUserNums = value; }
	virtual void SetIsoFootSpacerSupportBarsLengthType(long value) { m_nIsoFootSpacerSupportBarsLengthType = value; }
	virtual void SetIsoFootSpacerSupportBarsUserRatio(double value) { m_dIsoFootSpacerSupportBarsUserRatio = value; }
	virtual void SetIsoFootSpacerSupportBarsUserLength(long value) { m_nIsoFootSpacerSupportBarsUserLength = value; }

	virtual void SetIsoFootSupportRebarsSet(BOOL value) { m_bIsoFootSupportRebarsSet = value; }
	virtual void SetIsoFootSupportRebarsDiaIndex(long value) { m_nIsoFootSupportRebarsDiaIndex = value; }
	virtual void SetIsoFootSupportRebarsSlabLengthRate(double value) { m_dIsoFootSupportRebarsSlabLengthRate = value; }
	virtual void SetIsoFootSupportRebarsOrderLength(long value) { m_nIsoFootSupportRebarsOrderLength = value; }
	virtual void SetIsoFootSupportRebarsSpliceLength(long value) { m_nIsoFootSupportRebarsSpliceLength = value; }
	virtual void SetIsoFootSupportRebarsCorrectiveLength(long value) { m_nIsoFootSupportRebarsCorrectiveLength = value; }

	virtual long GetIsoFootDevType() { return m_nIsoFootDevType; }	
	virtual void SetIsoFootDevType(long value) { m_nIsoFootDevType = value; }	

	virtual BOOL GetIsoFootBotSpacerUserSteelGradeS() { return m_bIsoFootBotSpacerUserSteelGradeS; }
	virtual BOOL GetIsoFootTopSpacerUserSteelGradeS() { return m_bIsoFootTopSpacerUserSteelGradeS; }
	virtual BOOL GetIsoFootSpacerSupportBarUserSteelGradeS() { return m_bIsoFootSpacerSupportBarUserSteelGradeS; }
	virtual BOOL GetIsoFootSupportRebarUserSteelGradeS() { return m_bIsoFootSupportRebarUserSteelGradeS; }
	
	virtual void SetIsoFootBotSpacerUserSteelGradeS(BOOL value) { m_bIsoFootBotSpacerUserSteelGradeS = value; }
	virtual void SetIsoFootTopSpacerUserSteelGradeS(BOOL value) { m_bIsoFootTopSpacerUserSteelGradeS = value; }
	virtual void SetIsoFootSpacerSupportBarUserSteelGradeS(BOOL value) { m_bIsoFootSpacerSupportBarUserSteelGradeS = value; }
	virtual void SetIsoFootSupportRebarUserSteelGradeS(BOOL value) { m_bIsoFootSupportRebarUserSteelGradeS = value; }
	
	virtual BOOL GetIsoFootTopSpacerPlaceConditionSet() { return m_bIsoFootTopSpacerPlaceConditionSet; }
	virtual long GetIsoFootTopSpacerPlaceCondition() { return m_nIsoFootTopSpacerPlaceCondition; }

	virtual void SetIsoFootTopSpacerPlaceConditionSet(BOOL value) { m_bIsoFootTopSpacerPlaceConditionSet = value; }
	virtual void SetIsoFootTopSpacerPlaceCondition(long value) { m_nIsoFootTopSpacerPlaceCondition = value; }

	virtual BOOL GetIsoFootSpliceAddLengthSet() { return m_bIsoFootSpliceAddLengthSet; }
	virtual void SetIsoFootSpliceAddLengthSet(BOOL value) { m_bIsoFootSpliceAddLengthSet = value; }
	virtual long GetIsoFootSpliceAddLengthNumSplice() { return m_nIsoFootSpliceAddLengthNumSplice; }
	virtual void SetIsoFootSpliceAddLengthNumSplice(long value) { m_nIsoFootSpliceAddLengthNumSplice = value; }
private:
	BOOL m_bIsoFootSpliceDevConditionSet;
	long m_nIsoFootSpliceDevConditionLength;

	long m_nIsoFootDevType;

	BOOL m_bIsoFootTopSpacerInfoSet;
	BOOL m_bIsoFootTopSpacerSet;			//상부 우마철근 설정
	long m_nIsoFootTopSpacerDiaType;		//직경 0:해당기초직경 1:슬래브 두께
	long m_nIsoFootTopSpacerSlabThick;		//슬래브 두께
	long m_nIsoFootTopSpacerSlabThickOverDiaIndex;	//슬래브 두께 이상 직경
	long m_nIsoFootTopSpacerSlabThickUnderDiaIndex;	//슬래브 두께 미만 직경
	long m_nIsoFootTopSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nIsoFootTopSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nIsoFootTopSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nIsoFootTopSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nIsoFootTopSpacerHeightType;		//높이
	long m_nIsoFootTopSpacerUserHeight;		//높이 : 사용자 입력
	long m_nIsoFootTopSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nIsoFootTopSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nIsoFootTopSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nIsoFootTopSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nIsoFootTopSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nIsoFootTopSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nIsoFootTopSpacerWidth;			//폭

	BOOL m_bIsoFootBotSpacerInfoSet;
	BOOL m_bIsoFootBotSpacerSet;			//상부 우마철근 설정
	long m_nIsoFootBotSpacerDiaType;		//직경 0:해당기초직경 1:사용자직경
	long m_nIsoFootBotSpacerDiaIndex;		//직경 : 사용자 직경 입력
	long m_nIsoFootBotSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nIsoFootBotSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nIsoFootBotSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nIsoFootBotSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nIsoFootBotSpacerHeightType;		//높이
	long m_nIsoFootBotSpacerUserHeight;		//높이 : 사용자 입력
	long m_nIsoFootBotSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nIsoFootBotSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nIsoFootBotSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nIsoFootBotSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nIsoFootBotSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nIsoFootBotSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nIsoFootBotSpacerWidth;			//폭

	BOOL m_bIsoFootSpacerSupportBarsSet;			//우마지지근 설정
	BOOL m_bIsoFootSpacerSupportBarsConditionSet;	//우마지지근 적용 조건 설정
	long m_nIsoFootSpacerSupportBarsCondition;		//우마지지근 적용 조건
	long m_nIsoFootSpacerSupportBarsDiaType;		//직경 0:상부 우마직경 1:사용자 지정
	long m_nIsoFootSpacerSupportBarsUserDiaIndex;		//사용자 지정 입력
	long m_nIsoFootSpacerSupportBarsNumType;			//개수 0:상부 우마 개수 1:사용자 지정
	double m_dIsoFootSpacerSupportBarsTopSpacerNums;	//상부 우마 개수 입력
	long m_nIsoFootSpacerSupportBarsUserNums;			//사용자 지정 입력
	long m_nIsoFootSpacerSupportBarsLengthType;		//길이 0:해당 슬래브 높이X비율 1:사용자 지정
	double m_dIsoFootSpacerSupportBarsUserRatio;		//사용자 지정 비율
	long m_nIsoFootSpacerSupportBarsUserLength;		//사용자 지정 입력


	BOOL m_bIsoFootSupportRebarsSet;				//도리근 설정
	long m_nIsoFootSupportRebarsDiaIndex;			//도리근 직경
	double m_dIsoFootSupportRebarsSlabLengthRate;	//해당 슬래브의 둘레길이 비율
	long m_nIsoFootSupportRebarsOrderLength;		//도리근 주문길이
	long m_nIsoFootSupportRebarsSpliceLength;		//도리근 이음길이
	long m_nIsoFootSupportRebarsCorrectiveLength;	//도리근 보정길이

	BOOL m_bIsoFootBotSpacerUserSteelGradeS;
	BOOL m_bIsoFootTopSpacerUserSteelGradeS;
	BOOL m_bIsoFootSpacerSupportBarUserSteelGradeS;
	BOOL m_bIsoFootSupportRebarUserSteelGradeS;

	BOOL m_bIsoFootTopSpacerPlaceConditionSet;
	long m_nIsoFootTopSpacerPlaceCondition;

	BOOL m_bIsoFootSpliceAddLengthSet;
	long m_nIsoFootSpliceAddLengthNumSplice;
};

