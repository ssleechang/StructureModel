#pragma once
#include "BaseRebarTemplate.h"
class ShearFrictionBarTemplate;

class StructureModel_CLASS BeamRebarTemplate : public BaseRebarTemplate
{
public:
	BeamRebarTemplate();
	~BeamRebarTemplate(void);

	void Serialize(CArchive& ar);

	virtual void CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate) override;

	void DataInit();

	//Get Property
	virtual BOOL GetBeamGeneralInfoSet() { return m_bBeamGeneralInfoSet; }
	virtual long GetBeamTopMaxLen2HookAnchorBar() { return m_nBeamTopMaxLen2HookAnchorBar; }
	virtual long GetBeamBotMaxLen2HookAnchorBar() { return m_nBeamBotMaxLen2HookAnchorBar; }
	virtual long GetBeamLineContinuityHorEcc() { return m_nBeamLineContinuityHorEcc; }
	virtual long GetBeamLineContinuityVerEcc() { return m_nBeamLineContinuityVerEcc; }
	virtual long GetBeamBarLayerGap() { return m_nBeamBarLayerGap; }
	virtual long GetBeamMainBarConnectionTolerance() { return m_nBeamMainBarConnectionTolerance; }	
	virtual BOOL GetBeamBottomBarEnd150Dev() { return  m_bBeamBottomBarEnd150Dev; }
	virtual BOOL GetBeamBottomBarLink150Dev() { return  m_bBeamBottomBarLink150Dev; }

	virtual BOOL GetBeamSubBeamBottomBarEnd150Dev() { return  m_bBeamSubBeamBottomBarEnd150Dev; }
	virtual BOOL GetBeamSubBeamBottomBarLink150Dev() { return  m_bBeamSubBeamBottomBarLink150Dev; }

	virtual BOOL GetBeamSpliceDevConditionSet() { return m_bBeamSpliceDevConditionSet; }
	virtual long GetBeamSpliceDevConditionLength() { return m_nBeamSpliceDevConditionLength; }

	virtual BOOL GetBeamStirrupInfoSet() { return m_bBeamStirrupInfoSet; }
	virtual long GetBeamShearBarType() { return m_nBeamShearBarType; }
	virtual long GetBeamHorShearBarType() { return m_nBeamHorShearBarType; }
	virtual long GetBeamCapbarRatio() { return m_nBeamCapbarRatio; }
	virtual long GetBeamStirrupType() { return m_nBeamStirrupType; }
	virtual long GetBeamCoverBarType() { return m_nBeamCoverBarType; }
	virtual BOOL GetBeamDeepDualUBARPlaceSet() { return m_bBeamDeepDualUBARPlaceSet; }
	virtual long GetBeamDeepDepth() { return m_nBeamDeepDepth; }
	virtual long GetBeamDeepSpliceType() { return m_nBeamDeepSpliceType; }
	virtual long GetBeamStartSpacingType() { return m_nBeamStartSpacingType; }

	virtual BOOL GetBeamSubBeamInfoSet() { return m_bBeamSubBeamInfoSet; }
	virtual long GetSubBeamStirrupType() { return m_nSubBeamStirrupType; }
	virtual long GetSubBeamCapbarRatio() { return m_nSubBeamCapbarRatio; }
	virtual long GetSubBeamStirrupShape() { return m_nSubBeamStirrupShape; }
	virtual BOOL GetSubBeamOnlyCenterStirrupPlace() { return m_bSubBeamOnlyCenterStirrupPlace; }
	virtual long GetSubBeamStartSpacingType() { return m_nSubBeamStartSpacingType; }

	virtual BOOL GetBeamSubBarInfoSet() { return m_bBeamSubBarInfoSet; }
	virtual long GetBeamTopSubStirrupBarShape() { return m_nBeamTopSubStirrupBarShape; }
	virtual long GetBeamBotSubStirrupBarShape() { return m_nBeamBotSubStirrupBarShape; }
	virtual long GetBeamSubStirrupStartSpacing() { return m_nBeamSubStirrupStartSpacing; }
	virtual long GetBeamSubTopStirrupType() { return m_nBeamSubTopStirrupType; }
	virtual long GetBeamSubTopStirrupNum() { return m_nBeamSubTopStirrupNum; }
	virtual long GetBeamSubTopStirrupSpacing() { return m_nBeamSubTopStirrupSpacing; }
	virtual long GetBeamSubBotStirrupType() { return m_nBeamSubBotStirrupType; }
	virtual long GetBeamSubBotStirrupNum() { return m_nBeamSubBotStirrupNum; }
	virtual long GetBeamSubBotStirrupSpacing() { return m_nBeamSubBotStirrupSpacing; }
	virtual long GetBeamSubStirrupTopLen() { return m_nBeamSubStirrupTopLen; }
	virtual long GetBeamSubStirrupBotLen() { return m_nBeamSubStirrupBotLen; }
	virtual BOOL GetBeamSubStirrupUserHookLen() { return m_bBeamSubStirrupUserHookLen; }
	virtual long GetBeamSubStirrupBarType() { return m_nBeamSubStirrupBarType; }

	virtual BOOL GetBeamSupportRebarsSet() { return m_bBeamSupportRebarsSet; }
	virtual long GetBeamSupportRebarsDiaIndex() { return m_nBeamSupportRebarsDiaIndex; }
	virtual double GetBeamSupportRebarsTotalLengthRate() { return m_dBeamSupportRebarsTotalLengthRate; }
	virtual long GetBeamSupportRebarsOrderLength() { return m_nBeamSupportRebarsOrderLength; }
	virtual long GetBeamSupportRebarsSpliceLength() { return m_nBeamSupportRebarsSpliceLength; }
	virtual long GetBeamSupportRebarsCorrectiveLength() { return m_nBeamSupportRebarsCorrectiveLength; }
	
	virtual long GetBeamCapBarPlaceType(){ return m_nBeamCapBarPlaceType; }
	virtual long GetBeamCapBarNum(){ return m_nBeamCapBarNum; }

	virtual long GetSubBeamCapBarPlaceType(){ return m_nSubBeamCapBarPlaceType; }
	virtual long GetSubBeamCapBarNum(){ return m_nSubBeamCapBarNum; }
	virtual BOOL GetBeamSupportRebarUserSteelGradeS() { return m_bBeamSupportRebarUserSteelGradeS; }

	//Set Property
	virtual void SetBeamGeneralInfoSet(BOOL value) { m_bBeamGeneralInfoSet = value; }
	virtual void SetBeamTopMaxLen2HookAnchorBar(long value) { m_nBeamTopMaxLen2HookAnchorBar = value; }
	virtual void SetBeamBotMaxLen2HookAnchorBar(long value) { m_nBeamBotMaxLen2HookAnchorBar = value; }
	virtual void SetBeamLineContinuityHorEcc(long value) { m_nBeamLineContinuityHorEcc = value; }
	virtual void SetBeamLineContinuityVerEcc(long value) { m_nBeamLineContinuityVerEcc = value; }
	virtual void SetBeamBarLayerGap(long value) { m_nBeamBarLayerGap = value; }
	virtual void SetBeamMainBarConnectionTolerance(long value) { m_nBeamMainBarConnectionTolerance = value; }
	
	virtual void SetBeamBottomBarEnd150Dev(BOOL value) { m_bBeamBottomBarEnd150Dev = value; }
	virtual void SetBeamBottomBarLink150Dev(BOOL value) { m_bBeamBottomBarLink150Dev = value; }

	virtual void SetBeamSubBeamBottomBarEnd150Dev(BOOL value) { m_bBeamSubBeamBottomBarEnd150Dev = value; }
	virtual void SetBeamSubBeamBottomBarLink150Dev(BOOL value) { m_bBeamSubBeamBottomBarLink150Dev = value; }

	virtual void SetBeamSpliceDevConditionSet(BOOL value) { m_bBeamSpliceDevConditionSet = value; }
	virtual void SetBeamSpliceDevConditionLength(long value) { m_nBeamSpliceDevConditionLength = value; }

	virtual void SetBeamStirrupInfoSet(BOOL value) { m_bBeamStirrupInfoSet = value; }
	virtual void SetBeamShearBarType(long value) { m_nBeamShearBarType = value; }
	virtual void SetBeamHorShearBarType(long value) { m_nBeamHorShearBarType = value; }
	virtual void SetBeamCapbarRatio(long value) { m_nBeamCapbarRatio = value; }
	virtual void SetBeamStirrupType(long value) { m_nBeamStirrupType = value; }
	virtual void SetBeamCoverBarType(long value) { m_nBeamCoverBarType = value; }
	virtual void SetBeamDeepDualUBARPlaceSet(BOOL value) { m_bBeamDeepDualUBARPlaceSet = value; }
	virtual void SetBeamDeepDepth(long value) { m_nBeamDeepDepth = value; }
	virtual void SetBeamDeepSpliceType(long value) { m_nBeamDeepSpliceType = value; }
	virtual void SetBeamStartSpacingType(long value) { m_nBeamStartSpacingType = value; }

	virtual void SetBeamSubBeamInfoSet(BOOL value) { m_bBeamSubBeamInfoSet = value; }
	virtual void SetSubBeamStirrupType(long value) { m_nSubBeamStirrupType = value; }
	virtual void SetSubBeamCapbarRatio(long value) { m_nSubBeamCapbarRatio = value; }
	virtual void SetSubBeamStirrupShape(long value) { m_nSubBeamStirrupShape = value; }
	virtual void SetSubBeamOnlyCenterStirrupPlace(BOOL value) { m_bSubBeamOnlyCenterStirrupPlace = value; }
	virtual void SetSubBeamStartSpacingType(long value) { m_nSubBeamStartSpacingType = value; }

	virtual void SetBeamSubBarInfoSet(BOOL value) { m_bBeamSubBarInfoSet = value; }
	virtual void SetBeamTopSubStirrupBarShape(long value) { m_nBeamTopSubStirrupBarShape = value; }
	virtual void SetBeamBotSubStirrupBarShape(long value) { m_nBeamBotSubStirrupBarShape = value; }
	virtual void SetBeamSubStirrupStartSpacing(long value) { m_nBeamSubStirrupStartSpacing = value; }
	virtual void SetBeamSubTopStirrupType(long value) { m_nBeamSubTopStirrupType = value; }
	virtual void SetBeamSubTopStirrupNum(long value) { m_nBeamSubTopStirrupNum = value; }
	virtual void SetBeamSubTopStirrupSpacing(long value) { m_nBeamSubTopStirrupSpacing = value; }
	virtual void SetBeamSubBotStirrupType(long value) { m_nBeamSubBotStirrupType = value; }
	virtual void SetBeamSubBotStirrupNum(long value) { m_nBeamSubBotStirrupNum = value; }
	virtual void SetBeamSubBotStirrupSpacing(long value) { m_nBeamSubBotStirrupSpacing = value; }
	virtual void SetBeamSubStirrupTopLen(long value) { m_nBeamSubStirrupTopLen = value; }
	virtual void SetBeamSubStirrupBotLen(long value) { m_nBeamSubStirrupBotLen = value; }
	virtual void SetBeamSubStirrupUserHookLen(BOOL value) { m_bBeamSubStirrupUserHookLen = value; }
	virtual void SetBeamSubStirrupBarType(long value) { m_nBeamSubStirrupBarType = value; }

	virtual void SetBeamSupportRebarsSet(BOOL value) { m_bBeamSupportRebarsSet = value; }
	virtual void SetBeamSupportRebarsDiaIndex(long value) { m_nBeamSupportRebarsDiaIndex = value; }
	virtual void SetBeamSupportRebarsTotalLengthRate(double value) { m_dBeamSupportRebarsTotalLengthRate = value; }
	virtual void SetBeamSupportRebarsOrderLength(long value) { m_nBeamSupportRebarsOrderLength = value; }
	virtual void SetBeamSupportRebarsSpliceLength(long value) { m_nBeamSupportRebarsSpliceLength = value; }
	virtual void SetBeamSupportRebarsCorrectiveLength(long value) { m_nBeamSupportRebarsCorrectiveLength = value; }
	
	virtual void SetBeamCapBarPlaceType(long value){ m_nBeamCapBarPlaceType = value; }
	virtual void SetBeamCapBarNum(long value){ m_nBeamCapBarNum = value; }

	virtual void SetSubBeamCapBarPlaceType(long value){ m_nSubBeamCapBarPlaceType = value; }
	virtual void SetSubBeamCapBarNum(long value){ m_nSubBeamCapBarNum = value; }
	virtual void SetBeamSupportRebarUserSteelGradeS(BOOL value) { m_bBeamSupportRebarUserSteelGradeS = value; }
		
	//Function
	virtual map<long, long> GetBeamTopSubStirrupHookLenMap() { return mm_BeamTopSubStirrupHookLen; }
	virtual map<long, long> GetBeamBotSubStirrupHookLenMap() { return mm_BeamBotSubStirrupHookLen; }
	virtual map<long, long> GetBeamShearBarHookLenAMap() { return mm_BeamShearBarHookLenA; }
	virtual map<long, long> GetBeamShearBarHookLenBMap() { return mm_BeamShearBarHookLenB; }
	virtual map<long, long> GetBeamTopSubStirrupDiaIndexMap() { return mm_BeamTopSubStirrupDiaIndex; }
	virtual map<long, long> GetBeamBotSubStirrupDiaIndexMap() { return mm_BeamBotSubStirrupDiaIndex; }

	virtual void SetBeamTopSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen);
	virtual void SetBeamBotSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen);
	virtual void SetBeamShearBarHookLenAMap(map<long, long> mapShearBarHookLenA);
	virtual void SetBeamShearBarHookLenBMap(map<long, long> mapShearBarHookLenB);
	virtual void SetBeamTopSubStirrupDiaIndexMap(map<long, long> mapSubStirrupDiaIndex);
	virtual void SetBeamBotSubStirrupDiaIndexMap(map<long, long> mapSubStirrupDiaIndex);

	virtual double GetBeamTopSubStirrupHookLen(double dDia);
	virtual double GetBeamBotSubStirrupHookLen(double dDia);
	virtual double GetBeamShearBarHookLenA(double dDia);
	virtual double GetBeamShearBarHookLenB(double dDia);
	virtual double GetBeamTopSubStirrupDia(double dDia);
	virtual double GetBeamBotSubStirrupDia(double dDia);

	virtual void SetBeamTopSubStirrupHookLen(long key, long value) { mm_BeamTopSubStirrupHookLen[key] = value; }
	virtual void SetBeamBotSubStirrupHookLen(long key, long value) { mm_BeamBotSubStirrupHookLen[key] = value; }
	virtual void SetBeamShearBarHookLenA(long key, long value) { mm_BeamShearBarHookLenA[key] = value; }
	virtual void SetBeamShearBarHookLenB(long key, long value) { mm_BeamShearBarHookLenB[key] = value; }
	virtual void SetBeamTopSubStirrupDiaIndex(long key, long value) { mm_BeamTopSubStirrupDiaIndex[key] = value; }
	virtual void SetBeamBotSubStirrupDiaIndex(long key, long value) { mm_BeamBotSubStirrupDiaIndex[key] = value; }
	
	virtual ShearFrictionBarTemplate *GetBeamShearFrictionBarTemplate();

	virtual BOOL GetBeamSupportRebarsPlaceConditionSet() { return m_bBeamSupportRebarsPlaceConditionSet; }
	virtual long GetBeamSupportRebarsPlaceCondition() { return m_nBeamSupportRebarsPlaceCondition; }
	virtual void SetBeamSupportRebarsPlaceConditionSet(BOOL value) { m_bBeamSupportRebarsPlaceConditionSet = value; }
	virtual void SetBeamSupportRebarsPlaceCondition(long value) { m_nBeamSupportRebarsPlaceCondition = value; }

	virtual long GetBeamPlaceTypeDiaIndex() { return m_nBeamPlaceTypeDiaIndex; }
	virtual void SetBeamPlaceTypeDiaIndex(long value) { m_nBeamPlaceTypeDiaIndex = value; }

	virtual long GetBeamSpliceTypeBelowDia() { return m_nBeamSpliceTypeBelowDia; }
	virtual long GetBeamSpliceWayBelowDia() { return m_nBeamSpliceWayBelowDia; }
	virtual long GetBeamCZoneSpliceWayBelowDia() { return m_nBeamCZoneSpliceWayBelowDia; }
	virtual long GetBeamCZoneSpliceLengthBelowDia() { return m_nBeamCZoneSpliceLengthBelowDia; }

	virtual long GetBeamSpliceTypeExcessDia() { return m_nBeamSpliceTypeExcessDia; }
	virtual long GetBeamSpliceWayExcessDia() { return m_nBeamSpliceWayExcessDia; }
	virtual long GetBeamCZoneSpliceWayExcessDia() { return m_nBeamCZoneSpliceWayExcessDia; }
	virtual long GetBeamCZoneSpliceLengthExcessDia() { return m_nBeamCZoneSpliceLengthExcessDia; }

	virtual void SetBeamSpliceTypeBelowDia(long value) { m_nBeamSpliceTypeBelowDia = value; }
	virtual void SetBeamSpliceWayBelowDia(long value) { m_nBeamSpliceWayBelowDia = value; }
	virtual void SetBeamCZoneSpliceWayBelowDia(long value) { m_nBeamCZoneSpliceWayBelowDia = value; }
	virtual void SetBeamCZoneSpliceLengthBelowDia(long value) { m_nBeamCZoneSpliceLengthBelowDia = value; }

	virtual void SetBeamSpliceTypeExcessDia(long value) { m_nBeamSpliceTypeExcessDia = value; }
	virtual void SetBeamSpliceWayExcessDia(long value) { m_nBeamSpliceWayExcessDia = value; }
	virtual void SetBeamCZoneSpliceWayExcessDia(long value) { m_nBeamCZoneSpliceWayExcessDia = value; }
	virtual void SetBeamCZoneSpliceLengthExcessDia(long value) { m_nBeamCZoneSpliceLengthExcessDia = value; }

	virtual long GetBeamSpliceType(double dDia);
	virtual long GetBeamSpliceWay(double dDia);
	virtual long GetBeamCZoneSpliceWay(double dDia);
	virtual long GetBeamCZoneSpliceLength(double dDia);

	virtual BOOL GetBeamSpliceAddLengthSet() { return m_bBeamSpliceAddLengthSet; }
	virtual void SetBeamSpliceAddLengthSet(BOOL value) { m_bBeamSpliceAddLengthSet = value; }
	virtual long GetBeamSpliceAddLengthNumSplice() { return m_nBeamSpliceAddLengthNumSplice; }
	virtual void SetBeamSpliceAddLengthNumSplice(long value) { m_nBeamSpliceAddLengthNumSplice = value; }

	virtual BOOL GetBeamPlaceHorShearBar() { return m_bBeamPlaceHorShearBar; }
	virtual long GetBeamHorShearBarMainDirDiaIndex() { return m_nBeamHorShearBarMainDirDiaIndex; }
	virtual long GetBeamHorShearBarMainDirPlaceType() { return m_nBeamHorShearBarMainDirPlaceType; }
	virtual long GetBeamHorShearBarMainDirSpacing() { return m_nBeamHorShearBarMainDirSpacing; }
	virtual long GetBeamHorShearBarMainDirNumBar() { return m_nBeamHorShearBarMainDirNumBar; }
	virtual long GetBeamHorShearBarStirrupDirDiaIndex() { return m_nBeamHorShearBarStirrupDirDiaIndex; }
	virtual long GetBeamHorShearBarStirrupDirSpacing() { return m_nBeamHorShearBarStirrupDirSpacing; }
	virtual BOOL GetBeamHorShearBarMainDirUserSteelGradeS() { return m_bBeamHorShearBarMainDirUserSteelGradeS; }
	virtual BOOL GetBeamHorShearBarStirrupDirUserSteelGradeS() { return m_bBeamHorShearBarStirrupDirUserSteelGradeS; }
	

	virtual void SetBeamPlaceHorShearBar(BOOL value) { m_bBeamPlaceHorShearBar = value; }
	virtual void SetBeamHorShearBarMainDirDiaIndex(long value) { m_nBeamHorShearBarMainDirDiaIndex = value;	}
	virtual void SetBeamHorShearBarMainDirPlaceType(long value) { m_nBeamHorShearBarMainDirPlaceType = value; }
	virtual void SetBeamHorShearBarMainDirSpacing(long value) { m_nBeamHorShearBarMainDirSpacing = value; }
	virtual void SetBeamHorShearBarMainDirNumBar(long value) { m_nBeamHorShearBarMainDirNumBar = value; }
	virtual void SetBeamHorShearBarStirrupDirDiaIndex(long value) { m_nBeamHorShearBarStirrupDirDiaIndex = value; }
	virtual void SetBeamHorShearBarStirrupDirSpacing(long value) { m_nBeamHorShearBarStirrupDirSpacing = value; }
	virtual void SetBeamHorShearBarMainDirUserSteelGradeS(BOOL value) { m_bBeamHorShearBarMainDirUserSteelGradeS = value; }
	virtual void SetBeamHorShearBarStirrupDirUserSteelGradeS(BOOL value) { m_bBeamHorShearBarStirrupDirUserSteelGradeS = value; }
private:
	ShearFrictionBarTemplate *m_pBeamShearFrictionBarTemplate;
	//주문길이보다 짧고, 양단 후크인 철근의 최대길이
	BOOL m_bBeamGeneralInfoSet;
	long m_nBeamTopMaxLen2HookAnchorBar;	//상부근 최대 양후크 길이
	long m_nBeamBotMaxLen2HookAnchorBar;	//하부근 최대 양후크 길이
	long m_nBeamLineContinuityHorEcc;//보연속성판단기준 수평편심
	long m_nBeamLineContinuityVerEcc;//보연속성판단기준 수직편심
	long m_nBeamBarLayerGap;			//단사이간격
	long m_nBeamMainBarConnectionTolerance;//주근 연결 허용길이
				
	long m_nBeamPlaceTypeDiaIndex;
	long m_nBeamSpliceTypeBelowDia;	//이음 타입 0:인장이음 1:압축이음
	long m_nBeamSpliceWayBelowDia;	//이음 방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nBeamCZoneSpliceWayBelowDia;
	long m_nBeamCZoneSpliceLengthBelowDia;

	long m_nBeamSpliceTypeExcessDia;	//이음 타입 0:인장이음 1:압축이음
	long m_nBeamSpliceWayExcessDia;	//이음 방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nBeamCZoneSpliceWayExcessDia;
	long m_nBeamCZoneSpliceLengthExcessDia;

	BOOL m_bBeamBottomBarEnd150Dev;
	BOOL m_bBeamBottomBarLink150Dev;
	BOOL m_bBeamSubBeamBottomBarEnd150Dev;
	BOOL m_bBeamSubBeamBottomBarLink150Dev;


	BOOL m_bBeamSpliceDevConditionSet;
	long m_nBeamSpliceDevConditionLength;

	BOOL m_bBeamStirrupInfoSet;
	long m_nBeamShearBarType;	//전단근 형상
	long m_nBeamHorShearBarType;	//수평 전단근 형상
	long m_nBeamCapBarPlaceType;//캡바타입
	long m_nBeamCapBarNum;//캡바개수
	long m_nBeamCapbarRatio;	//캡바 비율
	
	long m_nBeamStirrupType;	//스트럽 형상
	long m_nBeamCoverBarType;	//상부CAP 형상
	BOOL m_bBeamDeepDualUBARPlaceSet;	//깊은보 양U-BAR 배근
	long m_nBeamDeepDepth;		//깊은보 춤 이상
	long m_nBeamDeepSpliceType;		//이음 타입 0:인장이음 1:압축이음
	long m_nBeamStartSpacingType;		//시작간격 설정 0:S/2 1:50

	BOOL m_bBeamSubBeamInfoSet;
	long m_nSubBeamStirrupType;			//0:개방형 스트럽배근 1:폐쇄형 스트럽배근
	long m_nSubBeamCapBarPlaceType;//캡바타입
	long m_nSubBeamCapBarNum;//캡바개수
	long m_nSubBeamCapbarRatio;			//캡바 비율
	long m_nSubBeamStirrupShape;		//0:90*90(내측) 1:90*90(외측)
	BOOL m_bSubBeamOnlyCenterStirrupPlace;	//중앙부만 개방형 스트럽배근
	long m_nSubBeamStartSpacingType;		//시작간격 설정 0:S/2 1:50

	//이단받침철근(SubStirrupBar)옵션 추가-20140902
	//-2DShopPro와 변수명 동일하게
	BOOL m_bBeamSubBarInfoSet;
	long m_nBeamTopSubStirrupBarShape;//상부받침철근형상
	long m_nBeamBotSubStirrupBarShape;//하부받침철근형상
	long m_nBeamSubTopStirrupNum;//상부받침철근 개수
	long m_nBeamSubBotStirrupNum;//하부받침철근 개수
	long m_nBeamSubStirrupTopLen;//상부스트럽높이
	long m_nBeamSubStirrupBotLen;//하부스트럽높이 = (스트럽높이-m_dBeamSubStirrupBotLen)
	BOOL m_bBeamSubStirrupUserHookLen;//true : 받침철근 발길이 및 전단보강근 A,B길이 설정을 사용자 설정으로
	long m_nBeamSubStirrupBarType;	//0:주근 1:스터럽
	map<long, long> mm_BeamTopSubStirrupHookLen;		//DiaIndex별 받침철근 발길이
	map<long, long> mm_BeamBotSubStirrupHookLen;		//DiaIndex별 받침철근 발길이
	map<long, long> mm_BeamShearBarHookLenA;		//DiaIndex별 전단보강근 발길이A
	map<long, long> mm_BeamShearBarHookLenB;		//DiaIndex별 전단보강근 발길이B
	map<long, long> mm_BeamTopSubStirrupDiaIndex;		//DiaIndex별 받침철근 직경설정 값
	map<long, long> mm_BeamBotSubStirrupDiaIndex;		//DiaIndex별 받침철근 직경설정 값

	//BH-3617 보 2단 받침철근 간격배근 지원 추가
	long m_nBeamSubStirrupStartSpacing; //받침철근 시작 간격
	long m_nBeamSubTopStirrupType; //상부받침철근 타입
	long m_nBeamSubTopStirrupSpacing;//상부받침철근 간격
	long m_nBeamSubBotStirrupType; //하부받침철근 타입
	long m_nBeamSubBotStirrupSpacing;//상부받침철근 간격

	BOOL m_bBeamSupportRebarsSet;					//하부 도리근 설정
	long m_nBeamSupportRebarsDiaIndex;				//직경
	double m_dBeamSupportRebarsTotalLengthRate;	//총 길이 비율
	long m_nBeamSupportRebarsOrderLength;			//주문길이
	long m_nBeamSupportRebarsSpliceLength;			//이음길이
	long m_nBeamSupportRebarsCorrectiveLength;		//보정길이
	BOOL m_bBeamSupportRebarUserSteelGradeS;
	BOOL m_bBeamSupportRebarsPlaceConditionSet;
	long m_nBeamSupportRebarsPlaceCondition;
	//BOOL m_bBuiltUpBeamInfoSet;
	//long m_nBuiltUpBeamPlacerType;	//덧살보 배근
	BOOL m_bBeamSpliceAddLengthSet;
	long m_nBeamSpliceAddLengthNumSplice;

	BOOL m_bBeamPlaceHorShearBar;
	long m_nBeamHorShearBarMainDirDiaIndex;
	long m_nBeamHorShearBarMainDirPlaceType;
	long m_nBeamHorShearBarMainDirSpacing;
	long m_nBeamHorShearBarMainDirNumBar;
	long m_nBeamHorShearBarStirrupDirDiaIndex;	
	long m_nBeamHorShearBarStirrupDirSpacing;	
	BOOL m_bBeamHorShearBarMainDirUserSteelGradeS;
	BOOL m_bBeamHorShearBarStirrupDirUserSteelGradeS;
};

