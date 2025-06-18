#pragma once
#include "BaseRebarTemplate.h"
#include "SlabHorSupportRebarsTemplate.h"

class StructureModel_CLASS WallFootRebarTemplate : public BaseRebarTemplate
{
public:
	WallFootRebarTemplate();
	~WallFootRebarTemplate(void);

	void Serialize(CArchive& ar);

	virtual void CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate) override;

	void DataInit();

	virtual BOOL GetWallFootSpliceDevConditionSet() { return m_bWallFootSpliceDevConditionSet; }
	virtual long GetWallFootSpliceDevConditionLength() { return m_nWallFootSpliceDevConditionLength; }
	
	virtual BOOL GetWallFootTopSpacerInfoSet() { return m_bWallFootTopSpacerInfoSet; }
	virtual BOOL GetWallFootTopSpacerSet() { return m_bWallFootTopSpacerSet; }
	virtual long GetWallFootTopSpacerDiaType() { return m_nWallFootTopSpacerDiaType; }
	virtual long GetWallFootTopSpacerSlabThick() { return m_nWallFootTopSpacerSlabThick; }
	virtual long GetWallFootTopSpacerSlabThickOverDiaIndex() { return m_nWallFootTopSpacerSlabThickOverDiaIndex; }
	virtual long GetWallFootTopSpacerSlabThickUnderDiaIndex() { return m_nWallFootTopSpacerSlabThickUnderDiaIndex; }
	virtual long GetWallFootTopSpacerSpacingType() { return m_nWallFootTopSpacerSpacingType; }
	virtual long GetWallFootTopSpacerSlabSpacing() { return m_nWallFootTopSpacerSlabSpacing; }
	virtual long GetWallFootTopSpacerUserSpacing1() { return m_nWallFootTopSpacerUserSpacing1; }
	virtual long GetWallFootTopSpacerUserSpacing2() { return m_nWallFootTopSpacerUserSpacing2; }
	virtual long GetWallFootTopSpacerHeightType() { return m_nWallFootTopSpacerHeightType; }
	virtual long GetWallFootTopSpacerUserHeight() { return m_nWallFootTopSpacerUserHeight; }
	virtual long GetWallFootTopSpacerHookLenAType() { return m_nWallFootTopSpacerHookLenAType; }
	virtual long GetWallFootTopSpacerMainBarHookLenA() { return m_nWallFootTopSpacerMainBarHookLenA; }
	virtual long GetWallFootTopSpacerUserHookLenA() { return m_nWallFootTopSpacerUserHookLenA; }
	virtual long GetWallFootTopSpacerHookLenBType() { return m_nWallFootTopSpacerHookLenBType; }
	virtual long GetWallFootTopSpacerMainBarHookLenB() { return m_nWallFootTopSpacerMainBarHookLenB; }
	virtual long GetWallFootTopSpacerUserHookLenB() { return m_nWallFootTopSpacerUserHookLenB; }
	virtual long GetWallFootTopSpacerWidth() { return m_nWallFootTopSpacerWidth; }

	virtual BOOL GetWallFootBotSpacerInfoSet() { return m_bWallFootBotSpacerInfoSet; }
	virtual BOOL GetWallFootBotSpacerSet() { return m_bWallFootBotSpacerSet; }
	virtual long GetWallFootBotSpacerDiaType() { return m_nWallFootBotSpacerDiaType; }
	virtual long GetWallFootBotSpacerDiaIndex() { return m_nWallFootBotSpacerDiaIndex; }
	virtual long GetWallFootBotSpacerSpacingType() { return m_nWallFootBotSpacerSpacingType; }
	virtual long GetWallFootBotSpacerSlabSpacing() { return m_nWallFootBotSpacerSlabSpacing; }
	virtual long GetWallFootBotSpacerUserSpacing1() { return m_nWallFootBotSpacerUserSpacing1; }
	virtual long GetWallFootBotSpacerUserSpacing2() { return m_nWallFootBotSpacerUserSpacing2; }
	virtual long GetWallFootBotSpacerHeightType() { return m_nWallFootBotSpacerHeightType; }
	virtual long GetWallFootBotSpacerUserHeight() { return m_nWallFootBotSpacerUserHeight; }
	virtual long GetWallFootBotSpacerHookLenAType() { return m_nWallFootBotSpacerHookLenAType; }
	virtual long GetWallFootBotSpacerMainBarHookLenA() { return m_nWallFootBotSpacerMainBarHookLenA; }
	virtual long GetWallFootBotSpacerUserHookLenA() { return m_nWallFootBotSpacerUserHookLenA; }
	virtual long GetWallFootBotSpacerHookLenBType() { return m_nWallFootBotSpacerHookLenBType; }
	virtual long GetWallFootBotSpacerMainBarHookLenB() { return m_nWallFootBotSpacerMainBarHookLenB; }
	virtual long GetWallFootBotSpacerUserHookLenB() { return m_nWallFootBotSpacerUserHookLenB; }
	virtual long GetWallFootBotSpacerWidth() { return m_nWallFootBotSpacerWidth; }

	virtual BOOL GetWallFootSpacerSupportBarsSet() { return m_bWallFootSpacerSupportBarsSet; }
	virtual BOOL GetWallFootSpacerSupportBarsConditionSet() { return m_bWallFootSpacerSupportBarsConditionSet; }
	virtual long GetWallFootSpacerSupportBarsCondition() { return m_nWallFootSpacerSupportBarsCondition; }
	virtual long GetWallFootSpacerSupportBarsDiaType() { return m_nWallFootSpacerSupportBarsDiaType; }
	virtual long GetWallFootSpacerSupportBarsUserDiaIndex() { return m_nWallFootSpacerSupportBarsUserDiaIndex; }
	virtual long GetWallFootSpacerSupportBarsNumType() { return m_nWallFootSpacerSupportBarsNumType; }
	virtual double GetWallFootSpacerSupportBarsTopSpacerNums() { return m_dWallFootSpacerSupportBarsTopSpacerNums; }
	virtual long GetWallFootSpacerSupportBarsUserNums() { return m_nWallFootSpacerSupportBarsUserNums; }
	virtual long GetWallFootSpacerSupportBarsLengthType() { return m_nWallFootSpacerSupportBarsLengthType; }
	virtual long GetWallFootSpacerSupportBarsUserLength() { return m_nWallFootSpacerSupportBarsUserLength; }
	virtual double GetWallFootSpacerSupportBarsUserRatio() { return m_dWallFootSpacerSupportBarsUserRatio; }

	virtual void SetWallFootSpliceDevConditionSet(BOOL value) { m_bWallFootSpliceDevConditionSet = value; }
	virtual void SetWallFootSpliceDevConditionLength(long value) { m_nWallFootSpliceDevConditionLength = value; }

	virtual void SetWallFootTopSpacerInfoSet(BOOL value) { m_bWallFootTopSpacerInfoSet = value; }
	virtual void SetWallFootTopSpacerSet(BOOL value) { m_bWallFootTopSpacerSet = value; }
	virtual void SetWallFootTopSpacerDiaType(long value) { m_nWallFootTopSpacerDiaType = value; }
	virtual void SetWallFootTopSpacerSlabThick(long value) { m_nWallFootTopSpacerSlabThick = value; }
	virtual void SetWallFootTopSpacerSlabThickOverDiaIndex(long value) { m_nWallFootTopSpacerSlabThickOverDiaIndex = value; }
	virtual void SetWallFootTopSpacerSlabThickUnderDiaIndex(long value) { m_nWallFootTopSpacerSlabThickUnderDiaIndex = value; }
	virtual void SetWallFootTopSpacerSpacingType(long value) { m_nWallFootTopSpacerSpacingType = value; }
	virtual void SetWallFootTopSpacerSlabSpacing(long value) { m_nWallFootTopSpacerSlabSpacing = value; }
	virtual void SetWallFootTopSpacerUserSpacing1(long value) { m_nWallFootTopSpacerUserSpacing1 = value; }
	virtual void SetWallFootTopSpacerUserSpacing2(long value) { m_nWallFootTopSpacerUserSpacing2 = value; }
	virtual void SetWallFootTopSpacerHeightType(long value) { m_nWallFootTopSpacerHeightType = value; }
	virtual void SetWallFootTopSpacerUserHeight(long value) { m_nWallFootTopSpacerUserHeight = value; }
	virtual void SetWallFootTopSpacerHookLenAType(long value) { m_nWallFootTopSpacerHookLenAType = value; }
	virtual void SetWallFootTopSpacerMainBarHookLenA(long value) { m_nWallFootTopSpacerMainBarHookLenA = value; }
	virtual void SetWallFootTopSpacerUserHookLenA(long value) { m_nWallFootTopSpacerUserHookLenA = value; }
	virtual void SetWallFootTopSpacerHookLenBType(long value) { m_nWallFootTopSpacerHookLenBType = value; }
	virtual void SetWallFootTopSpacerMainBarHookLenB(long value) { m_nWallFootTopSpacerMainBarHookLenB = value; }
	virtual void SetWallFootTopSpacerUserHookLenB(long value) { m_nWallFootTopSpacerUserHookLenB = value; }
	virtual void SetWallFootTopSpacerWidth(long value) { m_nWallFootTopSpacerWidth = value; }

	virtual void SetWallFootBotSpacerInfoSet(BOOL value) { m_bWallFootBotSpacerInfoSet = value; }
	virtual void SetWallFootBotSpacerSet(BOOL value) { m_bWallFootBotSpacerSet = value; }
	virtual void SetWallFootBotSpacerDiaType(long value) { m_nWallFootBotSpacerDiaType = value; }
	virtual void SetWallFootBotSpacerDiaIndex(long value) { m_nWallFootBotSpacerDiaIndex = value; }
	virtual void SetWallFootBotSpacerSpacingType(long value) { m_nWallFootBotSpacerSpacingType = value; }
	virtual void SetWallFootBotSpacerSlabSpacing(long value) { m_nWallFootBotSpacerSlabSpacing = value; }
	virtual void SetWallFootBotSpacerUserSpacing1(long value) { m_nWallFootBotSpacerUserSpacing1 = value; }
	virtual void SetWallFootBotSpacerUserSpacing2(long value) { m_nWallFootBotSpacerUserSpacing2 = value; }
	virtual void SetWallFootBotSpacerHeightType(long value) { m_nWallFootBotSpacerHeightType = value; }
	virtual void SetWallFootBotSpacerUserHeight(long value) { m_nWallFootBotSpacerUserHeight = value; }
	virtual void SetWallFootBotSpacerHookLenAType(long value) { m_nWallFootBotSpacerHookLenAType = value; }
	virtual void SetWallFootBotSpacerMainBarHookLenA(long value) { m_nWallFootBotSpacerMainBarHookLenA = value; }
	virtual void SetWallFootBotSpacerUserHookLenA(long value) { m_nWallFootBotSpacerUserHookLenA = value; }
	virtual void SetWallFootBotSpacerHookLenBType(long value) { m_nWallFootBotSpacerHookLenBType = value; }
	virtual void SetWallFootBotSpacerMainBarHookLenB(long value) { m_nWallFootBotSpacerMainBarHookLenB = value; }
	virtual void SetWallFootBotSpacerUserHookLenB(long value) { m_nWallFootBotSpacerUserHookLenB = value; }
	virtual void SetWallFootBotSpacerWidth(long value) { m_nWallFootBotSpacerWidth = value; }

	virtual void SetWallFootSpacerSupportBarsSet(BOOL value) { m_bWallFootSpacerSupportBarsSet = value; }
	virtual void SetWallFootSpacerSupportBarsConditionSet(BOOL value) { m_bWallFootSpacerSupportBarsConditionSet = value; }
	virtual void SetWallFootSpacerSupportBarsCondition(long value) { m_nWallFootSpacerSupportBarsCondition = value; }
	virtual void SetWallFootSpacerSupportBarsDiaType(long value) { m_nWallFootSpacerSupportBarsDiaType = value; }
	virtual void SetWallFootSpacerSupportBarsUserDiaIndex(long value) { m_nWallFootSpacerSupportBarsUserDiaIndex = value; }
	virtual void SetWallFootSpacerSupportBarsNumType(long value) { m_nWallFootSpacerSupportBarsNumType = value; }
	virtual void SetWallFootSpacerSupportBarsTopSpacerNums(double value) { m_dWallFootSpacerSupportBarsTopSpacerNums = value; }
	virtual void SetWallFootSpacerSupportBarsUserNums(long value) { m_nWallFootSpacerSupportBarsUserNums = value; }
	virtual void SetWallFootSpacerSupportBarsLengthType(long value) { m_nWallFootSpacerSupportBarsLengthType = value; }
	virtual void SetWallFootSpacerSupportBarsUserRatio(double value) { m_dWallFootSpacerSupportBarsUserRatio = value; }
	virtual void SetWallFootSpacerSupportBarsUserLength(long value) { m_nWallFootSpacerSupportBarsUserLength = value; }
	
	virtual long GetWallFootDevType() { return m_nWallFootDevType; }

	virtual void SetWallFootDevType(long value) { m_nWallFootDevType = value; }
	
	virtual SlabHorSupportRebarsTemplate *GetWallFootSupportRebarsTemplate();

	virtual BOOL GetWallFootBotSpacerUserSteelGradeS() { return m_bWallFootBotSpacerUserSteelGradeS; }
	virtual BOOL GetWallFootTopSpacerUserSteelGradeS() { return m_bWallFootTopSpacerUserSteelGradeS; }
	virtual BOOL GetWallFootSpacerSupportBarUserSteelGradeS() { return m_bWallFootSpacerSupportBarUserSteelGradeS; }
	
	virtual void SetWallFootBotSpacerUserSteelGradeS(BOOL value) { m_bWallFootBotSpacerUserSteelGradeS = value; }
	virtual void SetWallFootTopSpacerUserSteelGradeS(BOOL value) { m_bWallFootTopSpacerUserSteelGradeS = value; }
	virtual void SetWallFootSpacerSupportBarUserSteelGradeS(BOOL value) { m_bWallFootSpacerSupportBarUserSteelGradeS = value; }

	virtual BOOL GetWallFootTopSpacerPlaceConditionSet() { return m_bWallFootTopSpacerPlaceConditionSet; }
	virtual long GetWallFootTopSpacerPlaceCondition() { return m_nWallFootTopSpacerPlaceCondition; }

	virtual void SetWallFootTopSpacerPlaceConditionSet(BOOL value) { m_bWallFootTopSpacerPlaceConditionSet = value; }
	virtual void SetWallFootTopSpacerPlaceCondition(long value) { m_nWallFootTopSpacerPlaceCondition = value; }

	virtual BOOL GetWallFootSpliceAddLengthSet() { return m_bWallFootSpliceAddLengthSet; }
	virtual void SetWallFootSpliceAddLengthSet(BOOL value) { m_bWallFootSpliceAddLengthSet = value; }
	virtual long GetWallFootSpliceAddLengthNumSplice() { return m_nWallFootSpliceAddLengthNumSplice; }
	virtual void SetWallFootSpliceAddLengthNumSplice(long value) { m_nWallFootSpliceAddLengthNumSplice = value; }
private:
	SlabHorSupportRebarsTemplate *m_pSupportRebarsTemplate;

	BOOL m_bWallFootSpliceDevConditionSet;
	long m_nWallFootSpliceDevConditionLength;

	long m_nWallFootDevType;

	BOOL m_bWallFootTopSpacerInfoSet;
	BOOL m_bWallFootTopSpacerSet;			//상부 우마철근 설정
	long m_nWallFootTopSpacerDiaType;		//직경 0:해당기초직경 1:슬래브 두께
	long m_nWallFootTopSpacerSlabThick;		//슬래브 두께
	long m_nWallFootTopSpacerSlabThickOverDiaIndex;	//슬래브 두께 이상 직경
	long m_nWallFootTopSpacerSlabThickUnderDiaIndex;	//슬래브 두께 미만 직경
	long m_nWallFootTopSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nWallFootTopSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nWallFootTopSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nWallFootTopSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nWallFootTopSpacerHeightType;		//높이
	long m_nWallFootTopSpacerUserHeight;		//높이 : 사용자 입력
	long m_nWallFootTopSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nWallFootTopSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nWallFootTopSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nWallFootTopSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nWallFootTopSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nWallFootTopSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nWallFootTopSpacerWidth;			//폭

	BOOL m_bWallFootBotSpacerInfoSet;
	BOOL m_bWallFootBotSpacerSet;			//상부 우마철근 설정
	long m_nWallFootBotSpacerDiaType;		//직경 0:해당기초직경 1:사용자직경
	long m_nWallFootBotSpacerDiaIndex;		//직경 : 사용자 직경 입력
	long m_nWallFootBotSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nWallFootBotSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nWallFootBotSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nWallFootBotSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nWallFootBotSpacerHeightType;		//높이
	long m_nWallFootBotSpacerUserHeight;		//높이 : 사용자 입력
	long m_nWallFootBotSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nWallFootBotSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nWallFootBotSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nWallFootBotSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nWallFootBotSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nWallFootBotSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nWallFootBotSpacerWidth;			//폭

	BOOL m_bWallFootSpacerSupportBarsSet;			//우마지지근 설정
	BOOL m_bWallFootSpacerSupportBarsConditionSet;	//우마지지근 적용 조건 설정
	long m_nWallFootSpacerSupportBarsCondition;		//우마지지근 적용 조건
	long m_nWallFootSpacerSupportBarsDiaType;		//직경 0:상부 우마직경 1:사용자 지정
	long m_nWallFootSpacerSupportBarsUserDiaIndex;		//사용자 지정 입력
	long m_nWallFootSpacerSupportBarsNumType;			//개수 0:상부 우마 개수 1:사용자 지정
	double m_dWallFootSpacerSupportBarsTopSpacerNums;	//상부 우마 개수 입력
	long m_nWallFootSpacerSupportBarsUserNums;			//사용자 지정 입력
	long m_nWallFootSpacerSupportBarsLengthType;		//길이 0:해당 슬래브 높이X비율 1:사용자 지정
	double m_dWallFootSpacerSupportBarsUserRatio;		//사용자 지정 비율
	long m_nWallFootSpacerSupportBarsUserLength;		//사용자 지정 입력

	BOOL m_bWallFootBotSpacerUserSteelGradeS;
	BOOL m_bWallFootTopSpacerUserSteelGradeS;
	BOOL m_bWallFootSpacerSupportBarUserSteelGradeS;

	BOOL m_bWallFootTopSpacerPlaceConditionSet;
	long m_nWallFootTopSpacerPlaceCondition;

	BOOL m_bWallFootSpliceAddLengthSet;
	long m_nWallFootSpliceAddLengthNumSplice;
};

