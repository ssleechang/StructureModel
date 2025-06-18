#pragma once
#include "BaseRebarTemplate.h"

class StructureModel_CLASS DeckBeamRebarTemplate : public BaseRebarTemplate
{
public:
	DeckBeamRebarTemplate();
	~DeckBeamRebarTemplate(void);

	void Serialize(CArchive& ar);

	virtual void CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate) override;

	void DataInit();

	//Get Property
	virtual BOOL GetDeckBeamGeneralInfoSet() { return m_bDeckBeamGeneralInfoSet; }
	virtual long GetDeckBeamTopMaxLen2HookAnchorBar() { return m_nDeckBeamTopMaxLen2HookAnchorBar; }
	virtual long GetDeckBeamBotMaxLen2HookAnchorBar() { return m_nDeckBeamBotMaxLen2HookAnchorBar; }
	virtual long GetDeckBeamLineContinuityHorEcc() { return m_nDeckBeamLineContinuityHorEcc; }
	virtual long GetDeckBeamLineContinuityVerEcc() { return m_nDeckBeamLineContinuityVerEcc; }
	virtual long GetDeckBeamBarLayerGap() { return m_nDeckBeamBarLayerGap; }
	virtual long GetDeckBeamMainBarConnectionTolerance() { return m_nDeckBeamMainBarConnectionTolerance; }
	
	virtual BOOL GetDeckBeamBottomBarEnd150Dev() { return  m_bDeckBeamBottomBarEnd150Dev; }
	virtual BOOL GetDeckBeamBottomBarLink150Dev() { return  m_bDeckBeamBottomBarLink150Dev; }

	virtual BOOL GetDeckBeamSubBeamBottomBarEnd150Dev() { return  m_bDeckBeamSubBeamBottomBarEnd150Dev; }
	virtual BOOL GetDeckBeamSubBeamBottomBarLink150Dev() { return  m_bDeckBeamSubBeamBottomBarLink150Dev; }

	virtual BOOL GetDeckBeamSpliceDevConditionSet() { return m_bDeckBeamSpliceDevConditionSet; }
	virtual long GetDeckBeamSpliceDevConditionLength() { return m_nDeckBeamSpliceDevConditionLength; }

	virtual BOOL GetDeckBeamStirrupInfoSet() { return m_bDeckBeamStirrupInfoSet; }
	virtual long GetDeckBeamShearBarType() { return m_nDeckBeamShearBarType; }
	virtual long GetDeckBeamHorShearBarType() { return m_nDeckBeamHorShearBarType; }
	virtual long GetDeckBeamCapbarRatio() { return m_nDeckBeamCapbarRatio; }
	virtual long GetDeckBeamStirrupType() { return m_nDeckBeamStirrupType; }
	virtual long GetDeckBeamCoverBarType() { return m_nDeckBeamCoverBarType; }
	
	virtual long GetDeckBeamStartSpacingType() { return m_nDeckBeamStartSpacingType; }

	virtual BOOL GetDeckBeamSubBeamInfoSet() { return m_bDeckBeamSubBeamInfoSet; }
	virtual long GetDeckBeamSubBeamStirrupType() { return m_nDeckBeamSubBeamStirrupType; }
	virtual long GetDeckBeamSubBeamCapbarRatio() { return m_nDeckBeamSubBeamCapbarRatio; }
	virtual long GetDeckBeamSubBeamStirrupShape() { return m_nDeckBeamSubBeamStirrupShape; }
	virtual BOOL GetDeckBeamSubBeamOnlyCenterStirrupPlace() { return m_bDeckBeamSubBeamOnlyCenterStirrupPlace; }
	virtual long GetDeckBeamSubBeamStartSpacingType() { return m_nDeckBeamSubBeamStartSpacingType; }

	virtual BOOL GetDeckBeamSubBarInfoSet() { return m_bDeckBeamSubBarInfoSet; }
	virtual long GetDeckBeamTopSubStirrupBarShape() { return m_nDeckBeamTopSubStirrupBarShape; }
	virtual long GetDeckBeamBotSubStirrupBarShape() { return m_nDeckBeamBotSubStirrupBarShape; }
	virtual long GetDeckBeamSubStirrupStartSpacing() { return m_nDeckBeamSubStirrupStartSpacing; }
	virtual long GetDeckBeamSubTopStirrupType() { return m_nDeckBeamSubTopStirrupType; }
	virtual long GetDeckBeamSubTopStirrupNum() { return m_nDeckBeamSubTopStirrupNum; }
	virtual long GetDeckBeamSubTopStirrupSpacing() { return m_nDeckBeamSubTopStirrupSpacing; }
	virtual long GetDeckBeamSubBotStirrupType() { return m_nDeckBeamSubBotStirrupType; }
	virtual long GetDeckBeamSubBotStirrupNum() { return m_nDeckBeamSubBotStirrupNum; }
	virtual long GetDeckBeamSubBotStirrupSpacing() { return m_nDeckBeamSubBotStirrupSpacing; }
	virtual long GetDeckBeamSubStirrupTopLen() { return m_nDeckBeamSubStirrupTopLen; }
	virtual long GetDeckBeamSubStirrupBotLen() { return m_nDeckBeamSubStirrupBotLen; }
	virtual BOOL GetDeckBeamSubStirrupUserHookLen() { return m_bDeckBeamSubStirrupUserHookLen; }
	virtual long GetDeckBeamSubStirrupBarType() { return m_nDeckBeamSubStirrupBarType; }

	virtual long GetDeckBeamCapBarPlaceType(){ return m_nDeckBeamCapBarPlaceType; }
	virtual long GetDeckBeamCapBarNum(){ return m_nDeckBeamCapBarNum; }

	virtual long GetDeckBeamSubBeamCapBarPlaceType(){ return m_nDeckBeamSubBeamCapBarPlaceType; }
	virtual long GetDeckBeamSubBeamCapBarNum(){ return m_nDeckBeamSubBeamCapBarNum; }
	
	//Set Property
	virtual void SetDeckBeamGeneralInfoSet(BOOL value) { m_bDeckBeamGeneralInfoSet = value; }
	virtual void SetDeckBeamTopMaxLen2HookAnchorBar(long value) { m_nDeckBeamTopMaxLen2HookAnchorBar = value; }
	virtual void SetDeckBeamBotMaxLen2HookAnchorBar(long value) { m_nDeckBeamBotMaxLen2HookAnchorBar = value; }
	virtual void SetDeckBeamLineContinuityHorEcc(long value) { m_nDeckBeamLineContinuityHorEcc = value; }
	virtual void SetDeckBeamLineContinuityVerEcc(long value) { m_nDeckBeamLineContinuityVerEcc = value; }
	virtual void SetDeckBeamBarLayerGap(long value) { m_nDeckBeamBarLayerGap = value; }
	virtual void SetDeckBeamMainBarConnectionTolerance(long value) { m_nDeckBeamMainBarConnectionTolerance = value; }
	
	virtual void SetDeckBeamBottomBarEnd150Dev(BOOL value) { m_bDeckBeamBottomBarEnd150Dev = value; }
	virtual void SetDeckBeamBottomBarLink150Dev(BOOL value) { m_bDeckBeamBottomBarLink150Dev = value; }

	virtual void SetDeckBeamSubBeamBottomBarEnd150Dev(BOOL value) { m_bDeckBeamSubBeamBottomBarEnd150Dev = value; }
	virtual void SetDeckBeamSubBeamBottomBarLink150Dev(BOOL value) { m_bDeckBeamSubBeamBottomBarLink150Dev = value; }

	virtual void SetDeckBeamSpliceDevConditionSet(BOOL value) { m_bDeckBeamSpliceDevConditionSet = value; }
	virtual void SetDeckBeamSpliceDevConditionLength(long value) { m_nDeckBeamSpliceDevConditionLength = value; }

	virtual void SetDeckBeamStirrupInfoSet(BOOL value) { m_bDeckBeamStirrupInfoSet = value; }
	virtual void SetDeckBeamShearBarType(long value) { m_nDeckBeamShearBarType = value; }
	virtual void SetDeckBeamHorShearBarType(long value) { m_nDeckBeamHorShearBarType = value; }
	virtual void SetDeckBeamCapbarRatio(long value) { m_nDeckBeamCapbarRatio = value; }
	virtual void SetDeckBeamStirrupType(long value) { m_nDeckBeamStirrupType = value; }
	virtual void SetDeckBeamCoverBarType(long value) { m_nDeckBeamCoverBarType = value; }
	
	virtual void SetDeckBeamStartSpacingType(long value) { m_nDeckBeamStartSpacingType = value; }

	virtual void SetDeckBeamSubBeamInfoSet(BOOL value) { m_bDeckBeamSubBeamInfoSet = value; }
	virtual void SetDeckBeamSubBeamStirrupType(long value) { m_nDeckBeamSubBeamStirrupType = value; }
	virtual void SetDeckBeamSubBeamCapbarRatio(long value) { m_nDeckBeamSubBeamCapbarRatio = value; }
	virtual void SetDeckBeamSubBeamStirrupShape(long value) { m_nDeckBeamSubBeamStirrupShape = value; }
	virtual void SetDeckBeamSubBeamOnlyCenterStirrupPlace(BOOL value) { m_bDeckBeamSubBeamOnlyCenterStirrupPlace = value; }
	virtual void SetDeckBeamSubBeamStartSpacingType(long value) { m_nDeckBeamSubBeamStartSpacingType = value; }

	virtual void SetDeckBeamSubBarInfoSet(BOOL value) { m_bDeckBeamSubBarInfoSet = value; }
	virtual void SetDeckBeamTopSubStirrupBarShape(long value) { m_nDeckBeamTopSubStirrupBarShape = value; }
	virtual void SetDeckBeamBotSubStirrupBarShape(long value) { m_nDeckBeamBotSubStirrupBarShape = value; }
	virtual void SetDeckBeamSubStirrupStartSpacing(long value) { m_nDeckBeamSubStirrupStartSpacing = value; }
	virtual void SetDeckBeamSubTopStirrupType(long value) { m_nDeckBeamSubTopStirrupType = value; }
	virtual void SetDeckBeamSubTopStirrupNum(long value) { m_nDeckBeamSubTopStirrupNum = value; }
	virtual void SetDeckBeamSubTopStirrupSpacing(long value) { m_nDeckBeamSubTopStirrupSpacing = value; }
	virtual void SetDeckBeamSubBotStirrupType(long value) { m_nDeckBeamSubBotStirrupType = value; }
	virtual void SetDeckBeamSubBotStirrupNum(long value) { m_nDeckBeamSubBotStirrupNum = value; }
	virtual void SetDeckBeamSubBotStirrupSpacing(long value) { m_nDeckBeamSubBotStirrupSpacing = value; }
	virtual void SetDeckBeamSubStirrupTopLen(long value) { m_nDeckBeamSubStirrupTopLen = value; }
	virtual void SetDeckBeamSubStirrupBotLen(long value) { m_nDeckBeamSubStirrupBotLen = value; }
	virtual void SetDeckBeamSubStirrupUserHookLen(BOOL value) { m_bDeckBeamSubStirrupUserHookLen = value; }
	virtual void SetDeckBeamSubStirrupBarType(long value) { m_nDeckBeamSubStirrupBarType = value; }

	virtual void SetDeckBeamCapBarPlaceType(long value){ m_nDeckBeamCapBarPlaceType = value; }
	virtual void SetDeckBeamCapBarNum(long value){ m_nDeckBeamCapBarNum = value; }

	virtual void SetDeckBeamSubBeamCapBarPlaceType(long value){ m_nDeckBeamSubBeamCapBarPlaceType = value; }
	virtual void SetDeckBeamSubBeamCapBarNum(long value){ m_nDeckBeamSubBeamCapBarNum = value; }
	
	//Function
	virtual map<long, long> GetDeckBeamTopSubStirrupHookLenMap() { return mm_DeckBeamTopSubStirrupHookLen; }
	virtual map<long, long> GetDeckBeamBotSubStirrupHookLenMap() { return mm_DeckBeamBotSubStirrupHookLen; }
	virtual map<long, long> GetDeckBeamShearBarHookLenAMap() { return mm_DeckBeamShearBarHookLenA; }
	virtual map<long, long> GetDeckBeamShearBarHookLenBMap() { return mm_DeckBeamShearBarHookLenB; }
	virtual map<long, long> GetDeckBeamTopSubStirrupDiaIndexMap() { return mm_DeckBeamTopSubStirrupDiaIndex; }
	virtual map<long, long> GetDeckBeamBotSubStirrupDiaIndexMap() { return mm_DeckBeamBotSubStirrupDiaIndex; }

	virtual void SetDeckBeamTopSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen);
	virtual void SetDeckBeamBotSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen);
	virtual void SetDeckBeamShearBarHookLenAMap(map<long, long> mapShearBarHookLenA);
	virtual void SetDeckBeamShearBarHookLenBMap(map<long, long> mapShearBarHookLenB);
	virtual void SetDeckBeamTopSubStirrupDiaIndexMap(map<long, long> mapSubStirrupDiaIndex);
	virtual void SetDeckBeamBotSubStirrupDiaIndexMap(map<long, long> mapSubStirrupDiaIndex);

	virtual double GetDeckBeamTopSubStirrupHookLen(double dDia);
	virtual double GetDeckBeamBotSubStirrupHookLen(double dDia);
	virtual double GetDeckBeamShearBarHookLenA(double dDia);
	virtual double GetDeckBeamShearBarHookLenB(double dDia);
	virtual double GetDeckBeamTopSubStirrupDia(double dDia);
	virtual double GetDeckBeamBotSubStirrupDia(double dDia);

	virtual void SetDeckBeamTopSubStirrupHookLen(long key, long value) { mm_DeckBeamTopSubStirrupHookLen[key] = value; }
	virtual void SetDeckBeamBotSubStirrupHookLen(long key, long value) { mm_DeckBeamBotSubStirrupHookLen[key] = value; }
	virtual void SetDeckBeamShearBarHookLenA(long key, long value) { mm_DeckBeamShearBarHookLenA[key] = value; }
	virtual void SetDeckBeamShearBarHookLenB(long key, long value) { mm_DeckBeamShearBarHookLenB[key] = value; }
	virtual void SetDeckBeamTopSubStirrupDiaIndex(long key, long value) { mm_DeckBeamTopSubStirrupDiaIndex[key] = value; }
	virtual void SetDeckBeamBotSubStirrupDiaIndex(long key, long value) { mm_DeckBeamBotSubStirrupDiaIndex[key] = value; }
	
	virtual long GetDeckBeamPlaceTypeDiaIndex() { return m_nDeckBeamPlaceTypeDiaIndex; }
	virtual void SetDeckBeamPlaceTypeDiaIndex(long value) { m_nDeckBeamPlaceTypeDiaIndex = value; }

	virtual long GetDeckBeamSpliceTypeBelowDia() { return m_nDeckBeamSpliceTypeBelowDia; }
	virtual long GetDeckBeamSpliceWayBelowDia() { return m_nDeckBeamSpliceWayBelowDia; }
	virtual long GetDeckBeamCZoneSpliceWayBelowDia() { return m_nDeckBeamCZoneSpliceWayBelowDia; }
	virtual long GetDeckBeamCZoneSpliceLengthBelowDia() { return m_nDeckBeamCZoneSpliceLengthBelowDia; }

	virtual long GetDeckBeamSpliceTypeExcessDia() { return m_nDeckBeamSpliceTypeExcessDia; }
	virtual long GetDeckBeamSpliceWayExcessDia() { return m_nDeckBeamSpliceWayExcessDia; }
	virtual long GetDeckBeamCZoneSpliceWayExcessDia() { return m_nDeckBeamCZoneSpliceWayExcessDia; }
	virtual long GetDeckBeamCZoneSpliceLengthExcessDia() { return m_nDeckBeamCZoneSpliceLengthExcessDia; }

	virtual void SetDeckBeamSpliceTypeBelowDia(long value) { m_nDeckBeamSpliceTypeBelowDia = value; }
	virtual void SetDeckBeamSpliceWayBelowDia(long value) { m_nDeckBeamSpliceWayBelowDia = value; }
	virtual void SetDeckBeamCZoneSpliceWayBelowDia(long value) { m_nDeckBeamCZoneSpliceWayBelowDia = value; }
	virtual void SetDeckBeamCZoneSpliceLengthBelowDia(long value) { m_nDeckBeamCZoneSpliceLengthBelowDia = value; }

	virtual void SetDeckBeamSpliceTypeExcessDia(long value) { m_nDeckBeamSpliceTypeExcessDia = value; }
	virtual void SetDeckBeamSpliceWayExcessDia(long value) { m_nDeckBeamSpliceWayExcessDia = value; }
	virtual void SetDeckBeamCZoneSpliceWayExcessDia(long value) { m_nDeckBeamCZoneSpliceWayExcessDia = value; }
	virtual void SetDeckBeamCZoneSpliceLengthExcessDia(long value) { m_nDeckBeamCZoneSpliceLengthExcessDia = value; }

	virtual long GetDeckBeamSpliceType(double dDia);
	virtual long GetDeckBeamSpliceWay(double dDia);
	virtual long GetDeckBeamCZoneSpliceWay(double dDia);
	virtual long GetDeckBeamCZoneSpliceLength(double dDia);

	virtual BOOL GetDeckBeamSpliceAddLengthSet() { return m_bDeckBeamSpliceAddLengthSet; }
	virtual void SetDeckBeamSpliceAddLengthSet(BOOL value) { m_bDeckBeamSpliceAddLengthSet = value; }
	virtual long GetDeckBeamSpliceAddLengthNumSplice() { return m_nDeckBeamSpliceAddLengthNumSplice; }
	virtual void SetDeckBeamSpliceAddLengthNumSplice(long value) { m_nDeckBeamSpliceAddLengthNumSplice = value; }

	virtual long GetDeckBeamSidePressureBarDiaIndex() { return m_nDeckBeamSidePressureBarDiaIndex; }	
	virtual long GetDeckBeamSidePressureBarNum() { return m_nDeckBeamSidePressureBarNum; }
	virtual long GetDeckBeamSidePressureBarSpacing() { return m_nDeckBeamSidePressureBarSpacing; }

	virtual void SetDeckBeamSidePressureBarDiaIndex(long value) { m_nDeckBeamSidePressureBarDiaIndex = value; }	
	virtual void SetDeckBeamSidePressureBarNum(long value) { m_nDeckBeamSidePressureBarNum = value; }
	virtual void SetDeckBeamSidePressureBarSpacing(long value) { m_nDeckBeamSidePressureBarSpacing = value; }
private:

	//주문길이보다 짧고, 양단 후크인 철근의 최대길이
	BOOL m_bDeckBeamGeneralInfoSet;
	long m_nDeckBeamTopMaxLen2HookAnchorBar;	//상부근 최대 양후크 길이
	long m_nDeckBeamBotMaxLen2HookAnchorBar;	//하부근 최대 양후크 길이
	long m_nDeckBeamLineContinuityHorEcc;//보연속성판단기준 수평편심
	long m_nDeckBeamLineContinuityVerEcc;//보연속성판단기준 수직편심
	long m_nDeckBeamBarLayerGap;			//단사이간격
	long m_nDeckBeamMainBarConnectionTolerance;//주근 연결 허용길이
	
	long m_nDeckBeamPlaceTypeDiaIndex;
	long m_nDeckBeamSpliceTypeBelowDia;	//이음 타입 0:인장이음 1:압축이음
	long m_nDeckBeamSpliceWayBelowDia;	//이음 방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nDeckBeamCZoneSpliceWayBelowDia;
	long m_nDeckBeamCZoneSpliceLengthBelowDia;

	long m_nDeckBeamSpliceTypeExcessDia;	//이음 타입 0:인장이음 1:압축이음
	long m_nDeckBeamSpliceWayExcessDia;	//이음 방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nDeckBeamCZoneSpliceWayExcessDia;
	long m_nDeckBeamCZoneSpliceLengthExcessDia;

	BOOL m_bDeckBeamBottomBarEnd150Dev;
	BOOL m_bDeckBeamBottomBarLink150Dev;
	BOOL m_bDeckBeamSubBeamBottomBarEnd150Dev;
	BOOL m_bDeckBeamSubBeamBottomBarLink150Dev;


	BOOL m_bDeckBeamSpliceDevConditionSet;
	long m_nDeckBeamSpliceDevConditionLength;

	BOOL m_bDeckBeamStirrupInfoSet;
	long m_nDeckBeamShearBarType;	//전단근 형상
	long m_nDeckBeamHorShearBarType;	//수평 전단근 형상
	long m_nDeckBeamCapBarPlaceType;//캡바타입
	long m_nDeckBeamCapBarNum;//캡바개수
	long m_nDeckBeamCapbarRatio;	//캡바 비율
	
	long m_nDeckBeamStirrupType;	//스트럽 형상
	long m_nDeckBeamCoverBarType;	//상부CAP 형상

	

	long m_nDeckBeamStartSpacingType;		//시작간격 설정 0:S/2 1:50

	BOOL m_bDeckBeamSubBeamInfoSet;
	long m_nDeckBeamSubBeamStirrupType;			//0:개방형 스트럽배근 1:폐쇄형 스트럽배근
	long m_nDeckBeamSubBeamCapBarPlaceType;//캡바타입
	long m_nDeckBeamSubBeamCapBarNum;//캡바개수
	long m_nDeckBeamSubBeamCapbarRatio;			//캡바 비율
	long m_nDeckBeamSubBeamStirrupShape;		//0:90*90(내측) 1:90*90(외측)
	BOOL m_bDeckBeamSubBeamOnlyCenterStirrupPlace;	//중앙부만 개방형 스트럽배근
	long m_nDeckBeamSubBeamStartSpacingType;		//시작간격 설정 0:S/2 1:50

	//이단받침철근(SubStirrupBar)옵션 추가-20140902
	//-2DShopPro와 변수명 동일하게
	BOOL m_bDeckBeamSubBarInfoSet;
	long m_nDeckBeamTopSubStirrupBarShape;//상부받침철근형상
	long m_nDeckBeamBotSubStirrupBarShape;//하부받침철근형상
	long m_nDeckBeamSubTopStirrupNum;//상부받침철근 개수
	long m_nDeckBeamSubBotStirrupNum;//하부받침철근 개수
	long m_nDeckBeamSubStirrupTopLen;//상부스트럽높이
	long m_nDeckBeamSubStirrupBotLen;//하부스트럽높이 = (스트럽높이-m_dBeamSubStirrupBotLen)
	BOOL m_bDeckBeamSubStirrupUserHookLen;//true : 받침철근 발길이 및 전단보강근 A,B길이 설정을 사용자 설정으로
	long m_nDeckBeamSubStirrupBarType;	//0:주근 1:스터럽
	map<long, long> mm_DeckBeamTopSubStirrupHookLen;		//DiaIndex별 받침철근 발길이
	map<long, long> mm_DeckBeamBotSubStirrupHookLen;		//DiaIndex별 받침철근 발길이
	map<long, long> mm_DeckBeamShearBarHookLenA;		//DiaIndex별 전단보강근 발길이A
	map<long, long> mm_DeckBeamShearBarHookLenB;		//DiaIndex별 전단보강근 발길이B
	map<long, long> mm_DeckBeamTopSubStirrupDiaIndex;		//DiaIndex별 받침철근 직경설정 값
	map<long, long> mm_DeckBeamBotSubStirrupDiaIndex;		//DiaIndex별 받침철근 직경설정 값

	//BH-3617 보 2단 받침철근 간격배근 지원 추가
	long m_nDeckBeamSubStirrupStartSpacing; //받침철근 시작 간격
	long m_nDeckBeamSubTopStirrupType; //상부받침철근 타입
	long m_nDeckBeamSubTopStirrupSpacing;//상부받침철근 간격
	long m_nDeckBeamSubBotStirrupType; //하부받침철근 타입
	long m_nDeckBeamSubBotStirrupSpacing;//상부받침철근 간격

	BOOL m_bDeckBeamSpliceAddLengthSet;
	long m_nDeckBeamSpliceAddLengthNumSplice;

	long m_nDeckBeamSidePressureBarDiaIndex;
	long m_nDeckBeamSidePressureBarNum;
	long m_nDeckBeamSidePressureBarSpacing;

};

