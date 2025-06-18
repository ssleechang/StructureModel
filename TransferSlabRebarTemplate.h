#pragma once
#include "BaseRebarTemplate.h"

class StructureModel_CLASS TransferSlabRebarTemplate : public BaseRebarTemplate
{
public:
	TransferSlabRebarTemplate();
	~TransferSlabRebarTemplate(void);

	void Serialize(CArchive& ar);

	virtual void CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate) override;

	void DataInit();
		
	//TransferSlab Slab
	virtual BOOL GetTransferSlabMainBarInfoSet() { return m_bTransferSlabMainBarInfoSet; }
	virtual long GetTransferSlabMaxDualHookLength() { return m_nTransferSlabMaxDualHookLength; }
	virtual long GetTransferSlabSpliceWay() { return m_nTransferSlabSpliceWay; }
	virtual long GetTransferSlabSpliceType() { return m_nTransferSlabSpliceType; }

	virtual BOOL GetTransferSlabEndInfoSet() { return m_bTransferSlabEndInfoSet; }
	virtual long GetTransferSlabTopEndHookType() { return m_nTransferSlabTopEndHookType; }
	virtual long GetTransferSlabBotEndHookType() { return m_nTransferSlabBotEndHookType; }
	virtual long GetTransferSlabIsoFootDevType() { return m_nTransferSlabIsoFootDevType; }
	virtual long GetTransferSlabSpliceDevSlabType() { return m_nTransferSlabSpliceDevSlabType; }

	virtual long GetTransferSlabTopEndDevTypeUserLength() { return m_nTransferSlabTopEndDevTypeUserLength; }
	virtual long GetTransferSlabBotEndDevTypeUserLength() { return m_nTransferSlabBotEndDevTypeUserLength; }
	virtual long GetTransferSlabOpeningTopDevType() { return m_nTransferSlabOpeningTopDevType; }
	virtual long GetTransferSlabOpeningBotDevType() { return m_nTransferSlabOpeningBotDevType; }

	virtual BOOL GetTransferSlabLevelDifInfoSet() { return m_bTransferSlabLevelDifInfoSet; }
	virtual long GetTransferSlabLevelDifPlaceShape() { return m_nTransferSlabLevelDifPlaceShape; }
	virtual long GetTransferSlabLevelDifHaunchPlaceType() { return m_nTransferSlabLevelDifHaunchPlaceType; }


	virtual BOOL GetTransferSlabOpeningInfoSet() { return m_bTransferSlabOpeningInfoSet; }
	
	virtual BOOL GetTransferSlabRebarAreaInfoSet() { return m_bTransferSlabRebarAreaInfoSet; }
	virtual long GetTransferSlabRebarTolDist() { return m_nTransferSlabRebarTolDist; }
	virtual long GetTransferSlabRebarTolLenDif() { return m_nTransferSlabRebarTolLenDif; }

	virtual BOOL GetTransferSlabSpliceDevConditionSet() { return m_bTransferSlabSpliceDevConditionSet; }
	virtual long GetTransferSlabSpliceDevConditionLength() { return m_nTransferSlabSpliceDevConditionLength; }

	virtual BOOL GetTransferSlabSpliceAddLengthSet() { return m_bTransferSlabSpliceAddLengthSet; }
	virtual long GetTransferSlabSpliceAddLengthNumSplice() { return m_nTransferSlabSpliceAddLengthNumSplice; }

	virtual BOOL GetTransferSlabTopSpacerInfoSet() { return m_bTransferSlabTopSpacerInfoSet; }
	virtual BOOL GetTransferSlabTopSpacerSet() { return m_bTransferSlabTopSpacerSet; }
	virtual long GetTransferSlabTopSpacerDiaType() { return m_nTransferSlabTopSpacerDiaType; }
	virtual long GetTransferSlabTopSpacerSlabThick() { return m_nTransferSlabTopSpacerSlabThick; }
	virtual long GetTransferSlabTopSpacerSlabThickOverDiaIndex() { return m_nTransferSlabTopSpacerSlabThickOverDiaIndex; }
	virtual long GetTransferSlabTopSpacerSlabThickUnderDiaIndex() { return m_nTransferSlabTopSpacerSlabThickUnderDiaIndex; }
	virtual long GetTransferSlabTopSpacerSpacingType() { return m_nTransferSlabTopSpacerSpacingType; }
	virtual long GetTransferSlabTopSpacerSlabSpacing() { return m_nTransferSlabTopSpacerSlabSpacing; }
	virtual long GetTransferSlabTopSpacerUserSpacing1() { return m_nTransferSlabTopSpacerUserSpacing1; }
	virtual long GetTransferSlabTopSpacerUserSpacing2() { return m_nTransferSlabTopSpacerUserSpacing2; }
	virtual long GetTransferSlabTopSpacerHeightType() { return m_nTransferSlabTopSpacerHeightType; }
	virtual long GetTransferSlabTopSpacerUserHeight() { return m_nTransferSlabTopSpacerUserHeight; }
	virtual long GetTransferSlabTopSpacerHookLenAType() { return m_nTransferSlabTopSpacerHookLenAType; }
	virtual long GetTransferSlabTopSpacerMainBarHookLenA() { return m_nTransferSlabTopSpacerMainBarHookLenA; }
	virtual long GetTransferSlabTopSpacerUserHookLenA() { return m_nTransferSlabTopSpacerUserHookLenA; }
	virtual long GetTransferSlabTopSpacerHookLenBType() { return m_nTransferSlabTopSpacerHookLenBType; }
	virtual long GetTransferSlabTopSpacerMainBarHookLenB() { return m_nTransferSlabTopSpacerMainBarHookLenB; }
	virtual long GetTransferSlabTopSpacerUserHookLenB() { return m_nTransferSlabTopSpacerUserHookLenB; }
	virtual long GetTransferSlabTopSpacerWidth() { return m_nTransferSlabTopSpacerWidth; }

	virtual BOOL GetTransferSlabBotSpacerInfoSet() { return m_bTransferSlabBotSpacerInfoSet; }
	virtual BOOL GetTransferSlabBotSpacerSet() { return m_bTransferSlabBotSpacerSet; }
	virtual long GetTransferSlabBotSpacerDiaType() { return m_nTransferSlabBotSpacerDiaType; }
	virtual long GetTransferSlabBotSpacerDiaIndex() { return m_nTransferSlabBotSpacerDiaIndex; }
	virtual long GetTransferSlabBotSpacerSpacingType() { return m_nTransferSlabBotSpacerSpacingType; }
	virtual long GetTransferSlabBotSpacerSlabSpacing() { return m_nTransferSlabBotSpacerSlabSpacing; }
	virtual long GetTransferSlabBotSpacerUserSpacing1() { return m_nTransferSlabBotSpacerUserSpacing1; }
	virtual long GetTransferSlabBotSpacerUserSpacing2() { return m_nTransferSlabBotSpacerUserSpacing2; }
	virtual long GetTransferSlabBotSpacerHeightType() { return m_nTransferSlabBotSpacerHeightType; }
	virtual long GetTransferSlabBotSpacerUserHeight() { return m_nTransferSlabBotSpacerUserHeight; }
	virtual long GetTransferSlabBotSpacerHookLenAType() { return m_nTransferSlabBotSpacerHookLenAType; }
	virtual long GetTransferSlabBotSpacerMainBarHookLenA() { return m_nTransferSlabBotSpacerMainBarHookLenA; }
	virtual long GetTransferSlabBotSpacerUserHookLenA() { return m_nTransferSlabBotSpacerUserHookLenA; }
	virtual long GetTransferSlabBotSpacerHookLenBType() { return m_nTransferSlabBotSpacerHookLenBType; }
	virtual long GetTransferSlabBotSpacerMainBarHookLenB() { return m_nTransferSlabBotSpacerMainBarHookLenB; }
	virtual long GetTransferSlabBotSpacerUserHookLenB() { return m_nTransferSlabBotSpacerUserHookLenB; }
	virtual long GetTransferSlabBotSpacerWidth() { return m_nTransferSlabBotSpacerWidth; }

	virtual BOOL GetTransferSlabSpacerSupportBarsSet() { return m_bTransferSlabSpacerSupportBarsSet; }
	virtual BOOL GetTransferSlabSpacerSupportBarsConditionSet() { return m_bTransferSlabSpacerSupportBarsConditionSet; }
	virtual long GetTransferSlabSpacerSupportBarsCondition() { return m_nTransferSlabSpacerSupportBarsCondition; }
	virtual long GetTransferSlabSpacerSupportBarsDiaType() { return m_nTransferSlabSpacerSupportBarsDiaType; }
	virtual long GetTransferSlabSpacerSupportBarsUserDiaIndex() { return m_nTransferSlabSpacerSupportBarsUserDiaIndex; }
	virtual long GetTransferSlabSpacerSupportBarsNumType() { return m_nTransferSlabSpacerSupportBarsNumType; }
	virtual double GetTransferSlabSpacerSupportBarsTopSpacerNums() { return m_dTransferSlabSpacerSupportBarsTopSpacerNums; }
	virtual long GetTransferSlabSpacerSupportBarsUserNums() { return m_nTransferSlabSpacerSupportBarsUserNums; }
	virtual long GetTransferSlabSpacerSupportBarsLengthType() { return m_nTransferSlabSpacerSupportBarsLengthType; }
	virtual double GetTransferSlabSpacerSupportBarsUserRatio() { return m_dTransferSlabSpacerSupportBarsUserRatio; }
	virtual long GetTransferSlabSpacerSupportBarsUserLength() { return m_nTransferSlabSpacerSupportBarsUserLength; }

	virtual long GetTransferSlabCZoneSpliceWay() { return m_nTransferSlabCZoneSpliceWay; }
	virtual long GetTransferSlabCZoneSpliceLength() { return m_nTransferSlabCZoneSpliceLength; }

	virtual void SetTransferSlabMainBarInfoSet(BOOL value) { m_bTransferSlabMainBarInfoSet = value; }
	virtual void SetTransferSlabMaxDualHookLength(long value) { m_nTransferSlabMaxDualHookLength = value; }
	virtual void SetTransferSlabSpliceWay(long value) { m_nTransferSlabSpliceWay = value; }
	virtual void SetTransferSlabSpliceType(long value) { m_nTransferSlabSpliceType = value; }

	virtual void SetTransferSlabEndInfoSet(BOOL value) { m_bTransferSlabEndInfoSet = value; }
	virtual void SetTransferSlabTopEndHookType(long value) { m_nTransferSlabTopEndHookType = value; }
	virtual void SetTransferSlabBotEndHookType(long value) { m_nTransferSlabBotEndHookType = value; }
	virtual void SetTransferSlabIsoFootDevType(long value) { m_nTransferSlabIsoFootDevType = value; }
	virtual void SetTransferSlabSpliceDevSlabType(long value) { m_nTransferSlabSpliceDevSlabType = value; }

	virtual void SetTransferSlabTopEndDevTypeUserLength(long value) { m_nTransferSlabTopEndDevTypeUserLength = value; }
	virtual void SetTransferSlabBotEndDevTypeUserLength(long value) { m_nTransferSlabBotEndDevTypeUserLength = value; }
	virtual void SetTransferSlabOpeningTopDevType(long value) { m_nTransferSlabOpeningTopDevType = value; }
	virtual void SetTransferSlabOpeningBotDevType(long value) { m_nTransferSlabOpeningBotDevType = value; }

	virtual void SetTransferSlabLevelDifInfoSet(BOOL value) { m_bTransferSlabLevelDifInfoSet = value; }
	virtual void SetTransferSlabLevelDifPlaceShape(long value) { m_nTransferSlabLevelDifPlaceShape = value; }
	virtual void SetTransferSlabLevelDifHaunchPlaceType(long value) { m_nTransferSlabLevelDifHaunchPlaceType = value; }


	virtual void SetTransferSlabOpeningInfoSet(BOOL value) { m_bTransferSlabOpeningInfoSet = value; }
	
	virtual void SetTransferSlabRebarAreaInfoSet(BOOL value) { m_bTransferSlabRebarAreaInfoSet = value; }
	virtual void SetTransferSlabRebarTolDist(long value) { m_nTransferSlabRebarTolDist = value; }
	virtual void SetTransferSlabRebarTolLenDif(long value) { m_nTransferSlabRebarTolLenDif = value; }

	virtual void SetTransferSlabSpliceDevConditionSet(BOOL value) { m_bTransferSlabSpliceDevConditionSet = value; }
	virtual void SetTransferSlabSpliceDevConditionLength(long value) { m_nTransferSlabSpliceDevConditionLength = value; }

	virtual void SetTransferSlabSpliceAddLengthSet(BOOL value) { m_bTransferSlabSpliceAddLengthSet = value; }
	virtual void SetTransferSlabSpliceAddLengthNumSplice(long value) { m_nTransferSlabSpliceAddLengthNumSplice = value; }

	virtual void SetTransferSlabTopSpacerInfoSet(BOOL value) { m_bTransferSlabTopSpacerInfoSet = value; }
	virtual void SetTransferSlabTopSpacerSet(BOOL value) { m_bTransferSlabTopSpacerSet = value; }
	virtual void SetTransferSlabTopSpacerDiaType(long value) { m_nTransferSlabTopSpacerDiaType = value; }
	virtual void SetTransferSlabTopSpacerSlabThick(long value) { m_nTransferSlabTopSpacerSlabThick = value; }
	virtual void SetTransferSlabTopSpacerSlabThickOverDiaIndex(long value) { m_nTransferSlabTopSpacerSlabThickOverDiaIndex = value; }
	virtual void SetTransferSlabTopSpacerSlabThickUnderDiaIndex(long value) { m_nTransferSlabTopSpacerSlabThickUnderDiaIndex = value; }
	virtual void SetTransferSlabTopSpacerSpacingType(long value) { m_nTransferSlabTopSpacerSpacingType = value; }
	virtual void SetTransferSlabTopSpacerSlabSpacing(long value) { m_nTransferSlabTopSpacerSlabSpacing = value; }
	virtual void SetTransferSlabTopSpacerUserSpacing1(long value) { m_nTransferSlabTopSpacerUserSpacing1 = value; }
	virtual void SetTransferSlabTopSpacerUserSpacing2(long value) { m_nTransferSlabTopSpacerUserSpacing2 = value; }
	virtual void SetTransferSlabTopSpacerHeightType(long value) { m_nTransferSlabTopSpacerHeightType = value; }
	virtual void SetTransferSlabTopSpacerUserHeight(long value) { m_nTransferSlabTopSpacerUserHeight = value; }
	virtual void SetTransferSlabTopSpacerHookLenAType(long value) { m_nTransferSlabTopSpacerHookLenAType = value; }
	virtual void SetTransferSlabTopSpacerMainBarHookLenA(long value) { m_nTransferSlabTopSpacerMainBarHookLenA = value; }
	virtual void SetTransferSlabTopSpacerUserHookLenA(long value) { m_nTransferSlabTopSpacerUserHookLenA = value; }
	virtual void SetTransferSlabTopSpacerHookLenBType(long value) { m_nTransferSlabTopSpacerHookLenBType = value; }
	virtual void SetTransferSlabTopSpacerMainBarHookLenB(long value) { m_nTransferSlabTopSpacerMainBarHookLenB = value; }
	virtual void SetTransferSlabTopSpacerUserHookLenB(long value) { m_nTransferSlabTopSpacerUserHookLenB = value; }
	virtual void SetTransferSlabTopSpacerWidth(long value) { m_nTransferSlabTopSpacerWidth = value; }

	virtual void SetTransferSlabBotSpacerInfoSet(BOOL value) { m_bTransferSlabBotSpacerInfoSet = value; }
	virtual void SetTransferSlabBotSpacerSet(BOOL value) { m_bTransferSlabBotSpacerSet = value; }
	virtual void SetTransferSlabBotSpacerDiaType(long value) { m_nTransferSlabBotSpacerDiaType = value; }
	virtual void SetTransferSlabBotSpacerDiaIndex(long value) { m_nTransferSlabBotSpacerDiaIndex = value; }
	virtual void SetTransferSlabBotSpacerSpacingType(long value) { m_nTransferSlabBotSpacerSpacingType = value; }
	virtual void SetTransferSlabBotSpacerSlabSpacing(long value) { m_nTransferSlabBotSpacerSlabSpacing = value; }
	virtual void SetTransferSlabBotSpacerUserSpacing1(long value) { m_nTransferSlabBotSpacerUserSpacing1 = value; }
	virtual void SetTransferSlabBotSpacerUserSpacing2(long value) { m_nTransferSlabBotSpacerUserSpacing2 = value; }
	virtual void SetTransferSlabBotSpacerHeightType(long value) { m_nTransferSlabBotSpacerHeightType = value; }
	virtual void SetTransferSlabBotSpacerUserHeight(long value) { m_nTransferSlabBotSpacerUserHeight = value; }
	virtual void SetTransferSlabBotSpacerHookLenAType(long value) { m_nTransferSlabBotSpacerHookLenAType = value; }
	virtual void SetTransferSlabBotSpacerMainBarHookLenA(long value) { m_nTransferSlabBotSpacerMainBarHookLenA = value; }
	virtual void SetTransferSlabBotSpacerUserHookLenA(long value) { m_nTransferSlabBotSpacerUserHookLenA = value; }
	virtual void SetTransferSlabBotSpacerHookLenBType(long value) { m_nTransferSlabBotSpacerHookLenBType = value; }
	virtual void SetTransferSlabBotSpacerMainBarHookLenB(long value) { m_nTransferSlabBotSpacerMainBarHookLenB = value; }
	virtual void SetTransferSlabBotSpacerUserHookLenB(long value) { m_nTransferSlabBotSpacerUserHookLenB = value; }
	virtual void SetTransferSlabBotSpacerWidth(long value) { m_nTransferSlabBotSpacerWidth = value; }

	virtual void SetTransferSlabSpacerSupportBarsSet(BOOL value) { m_bTransferSlabSpacerSupportBarsSet = value; }
	virtual void SetTransferSlabSpacerSupportBarsConditionSet(BOOL value) { m_bTransferSlabSpacerSupportBarsConditionSet = value; }
	virtual void SetTransferSlabSpacerSupportBarsCondition(long value) { m_nTransferSlabSpacerSupportBarsCondition = value; }
	virtual void SetTransferSlabSpacerSupportBarsDiaType(long value) { m_nTransferSlabSpacerSupportBarsDiaType = value; }
	virtual void SetTransferSlabSpacerSupportBarsUserDiaIndex(long value) { m_nTransferSlabSpacerSupportBarsUserDiaIndex = value; }
	virtual void SetTransferSlabSpacerSupportBarsNumType(long value) { m_nTransferSlabSpacerSupportBarsNumType = value; }
	virtual void SetTransferSlabSpacerSupportBarsTopSpacerNums(double value) { m_dTransferSlabSpacerSupportBarsTopSpacerNums = value; }
	virtual void SetTransferSlabSpacerSupportBarsUserNums(long value) { m_nTransferSlabSpacerSupportBarsUserNums = value; }
	virtual void SetTransferSlabSpacerSupportBarsLengthType(long value) { m_nTransferSlabSpacerSupportBarsLengthType = value; }
	virtual void SetTransferSlabSpacerSupportBarsUserRatio(double value) { m_dTransferSlabSpacerSupportBarsUserRatio = value; }
	virtual void SetTransferSlabSpacerSupportBarsUserLength(long value) { m_nTransferSlabSpacerSupportBarsUserLength = value; }

	virtual void SetTransferSlabCZoneSpliceWay(long value) { m_nTransferSlabCZoneSpliceWay = value; }
	virtual void SetTransferSlabCZoneSpliceLength(long value) { m_nTransferSlabCZoneSpliceLength = value; }
			
	virtual ShearFrictionBarTemplate *GetTransferSlabShearFrictionBarTemplate();
	
	virtual BOOL GetTransferSlabBotSpacerUserSteelGradeS() { return m_bTransferSlabBotSpacerUserSteelGradeS; }
	virtual BOOL GetTransferSlabTopSpacerUserSteelGradeS() { return m_bTransferSlabTopSpacerUserSteelGradeS; }
	virtual BOOL GetTransferSlabSpacerSupportBarUserSteelGradeS() { return m_bTransferSlabSpacerSupportBarUserSteelGradeS; }

	virtual void SetTransferSlabBotSpacerUserSteelGradeS(BOOL value) { m_bTransferSlabBotSpacerUserSteelGradeS = value; }
	virtual void SetTransferSlabTopSpacerUserSteelGradeS(BOOL value) { m_bTransferSlabTopSpacerUserSteelGradeS = value; }
	virtual void SetTransferSlabSpacerSupportBarUserSteelGradeS(BOOL value) { m_bTransferSlabSpacerSupportBarUserSteelGradeS = value; }
	
	virtual BOOL GetTransferSlabTopSpacerPlaceConditionSet() { return m_bTransferSlabTopSpacerPlaceConditionSet; }
	virtual long GetTransferSlabTopSpacerPlaceCondition() { return m_nTransferSlabTopSpacerPlaceCondition; }
	virtual void SetTransferSlabTopSpacerPlaceConditionSet(BOOL value) { m_bTransferSlabTopSpacerPlaceConditionSet = value; }
	virtual void SetTransferSlabTopSpacerPlaceCondition(long value) { m_nTransferSlabTopSpacerPlaceCondition = value; }

	virtual BOOL GetTransferSlabEndAddBarSet() { return m_bTransferSlabEndAddBarSet; }
	virtual long GetTransferSlabEndVerAddBarDiaIndex() { return m_nTransferSlabEndVerAddBarDiaIndex; }
	virtual long GetTransferSlabEndVerAddBarSpacing() { return m_nTransferSlabEndVerAddBarSpacing; }
	virtual long GetTransferSlabEndHorAddBarPlaceCondition() { return m_nTransferSlabEndHorAddBarPlaceCondition; }
	virtual long GetTransferSlabEndHorAddBarDiaIndex() { return m_nTransferSlabEndHorAddBarDiaIndex; }
	virtual long GetTransferSlabEndHorAddBarSpacing() { return m_nTransferSlabEndHorAddBarSpacing; }

	virtual void SetTransferSlabEndAddBarSet(BOOL value) { m_bTransferSlabEndAddBarSet = value; }
	virtual void SetTransferSlabEndVerAddBarDiaIndex(long value) { m_nTransferSlabEndVerAddBarDiaIndex = value; }
	virtual void SetTransferSlabEndVerAddBarSpacing(long value) { m_nTransferSlabEndVerAddBarSpacing = value; }
	virtual void SetTransferSlabEndHorAddBarPlaceCondition(long value) { m_nTransferSlabEndHorAddBarPlaceCondition = value; }
	virtual void SetTransferSlabEndHorAddBarDiaIndex(long value) { m_nTransferSlabEndHorAddBarDiaIndex = value; }
	virtual void SetTransferSlabEndHorAddBarSpacing(long value) { m_nTransferSlabEndHorAddBarSpacing = value; }

private:	
	ShearFrictionBarTemplate *m_pTransferSlabShearFrictionBarTemplate;
	
	// TransferSlab Slab
	BOOL m_bTransferSlabMainBarInfoSet;
	long m_nTransferSlabMaxDualHookLength;	//최대 양후크길이
	long m_nTransferSlabSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nTransferSlabSpliceType;	//이음타입 0:인장이음 1:압축이음
			
	BOOL m_bTransferSlabEndInfoSet;
	long m_nTransferSlabTopEndHookType;		//상부근 단부 0:정착 1:후크 
	long m_nTransferSlabBotEndHookType;		//하부근 단부 0:정착 1:후크 
	long m_nTransferSlabIsoFootDevType;		//독립기초 & Panel 접합부위 하부근 0:Passing 1:인장정착
	long m_nTransferSlabSpliceDevSlabType;	//이음/정착 적용	0:기초	1:슬래브

	long m_nTransferSlabTopEndDevTypeUserLength;
	long m_nTransferSlabBotEndDevTypeUserLength;
	long m_nTransferSlabOpeningTopDevType;
	long m_nTransferSlabOpeningBotDevType;

	BOOL m_bTransferSlabLevelDifInfoSet;
	long m_nTransferSlabLevelDifPlaceShape;		//배근 형상
	long m_nTransferSlabLevelDifHaunchPlaceType;	//헌치 배근타입
	
	BOOL m_bTransferSlabOpeningInfoSet;
	
	BOOL m_bTransferSlabRebarAreaInfoSet;
	long m_nTransferSlabRebarTolDist;		//앞뒤 철근의 길이차
	long m_nTransferSlabRebarTolLenDif;		//가장 긴 철근과 짧은 철근이 길이차가 m_nTransferSlabRebarTolLenDif이상이 되면 barset을 나눈다.

	BOOL m_bTransferSlabSpliceDevConditionSet;
	long m_nTransferSlabSpliceDevConditionLength;

	BOOL m_bTransferSlabSpliceAddLengthSet;
	long m_nTransferSlabSpliceAddLengthNumSplice;

	BOOL m_bTransferSlabTopSpacerInfoSet;
	BOOL m_bTransferSlabTopSpacerSet;			//상부 우마철근 설정
	long m_nTransferSlabTopSpacerDiaType;		//직경 0:해당기초직경 1:슬래브 두께
	long m_nTransferSlabTopSpacerSlabThick;		//슬래브 두께
	long m_nTransferSlabTopSpacerSlabThickOverDiaIndex;	//슬래브 두께 이상 직경
	long m_nTransferSlabTopSpacerSlabThickUnderDiaIndex;	//슬래브 두께 미만 직경
	long m_nTransferSlabTopSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nTransferSlabTopSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nTransferSlabTopSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nTransferSlabTopSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nTransferSlabTopSpacerHeightType;		//높이
	long m_nTransferSlabTopSpacerUserHeight;		//높이 : 사용자 입력
	long m_nTransferSlabTopSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nTransferSlabTopSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nTransferSlabTopSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nTransferSlabTopSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nTransferSlabTopSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nTransferSlabTopSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nTransferSlabTopSpacerWidth;			//폭

	BOOL m_bTransferSlabBotSpacerInfoSet;
	BOOL m_bTransferSlabBotSpacerSet;			//상부 우마철근 설정
	long m_nTransferSlabBotSpacerDiaType;		//직경 0:해당기초직경 1:사용자직경
	long m_nTransferSlabBotSpacerDiaIndex;		//직경 : 사용자 직경 입력
	long m_nTransferSlabBotSpacerSpacingType;	//간격 0:해당 슬래브 간격 1:사용자
	long m_nTransferSlabBotSpacerSlabSpacing;	//간격 : 해당 슬래브 간격 입력
	long m_nTransferSlabBotSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nTransferSlabBotSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nTransferSlabBotSpacerHeightType;		//높이
	long m_nTransferSlabBotSpacerUserHeight;		//높이 : 사용자 입력
	long m_nTransferSlabBotSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nTransferSlabBotSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nTransferSlabBotSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nTransferSlabBotSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nTransferSlabBotSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nTransferSlabBotSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nTransferSlabBotSpacerWidth;			//폭

	BOOL m_bTransferSlabSpacerSupportBarsSet;			//우마지지근 설정
	BOOL m_bTransferSlabSpacerSupportBarsConditionSet;	//우마지지근 적용 조건 설정
	long m_nTransferSlabSpacerSupportBarsCondition;		//우마지지근 적용 조건
	long m_nTransferSlabSpacerSupportBarsDiaType;		//직경 0:상부 우마직경 1:사용자 지정
	long m_nTransferSlabSpacerSupportBarsUserDiaIndex;		//사용자 지정 입력
	long m_nTransferSlabSpacerSupportBarsNumType;			//개수 0:상부 우마 개수 1:사용자 지정
	double m_dTransferSlabSpacerSupportBarsTopSpacerNums;	//상부 우마 개수 입력
	long m_nTransferSlabSpacerSupportBarsUserNums;			//사용자 지정 입력
	long m_nTransferSlabSpacerSupportBarsLengthType;		//길이 0:해당 슬래브 높이X1.4 1:사용자 지정
	double m_dTransferSlabSpacerSupportBarsUserRatio;
	long m_nTransferSlabSpacerSupportBarsUserLength;		//사용자 지정 입력

	long m_nTransferSlabCZoneSpliceWay;
	long m_nTransferSlabCZoneSpliceLength;

	BOOL m_bTransferSlabBotSpacerUserSteelGradeS;
	BOOL m_bTransferSlabTopSpacerUserSteelGradeS;
	BOOL m_bTransferSlabSpacerSupportBarUserSteelGradeS;

	BOOL m_bTransferSlabTopSpacerPlaceConditionSet;
	long m_nTransferSlabTopSpacerPlaceCondition;

	BOOL m_bTransferSlabEndAddBarSet;
	long m_nTransferSlabEndVerAddBarDiaIndex;
	long m_nTransferSlabEndVerAddBarSpacing;
	long m_nTransferSlabEndHorAddBarPlaceCondition;
	long m_nTransferSlabEndHorAddBarDiaIndex;
	long m_nTransferSlabEndHorAddBarSpacing;

	map<CString, long> mm_TransferSlabThickToDiaIndex;
	
	virtual map<CString, long> GetTransferSlabThickToDiaIndex() { return mm_TransferSlabThickToDiaIndex; }
	
	virtual void SetTransferSlabThickToDiaIndex(map<CString, long> value);
};

