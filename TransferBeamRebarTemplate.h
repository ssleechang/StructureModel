#pragma once
#include "BaseRebarTemplate.h"
class ShearFrictionBarTemplate;

class StructureModel_CLASS TransferBeamRebarTemplate : public BaseRebarTemplate
{
public:
	TransferBeamRebarTemplate();
	~TransferBeamRebarTemplate(void);

	void Serialize(CArchive& ar);

	virtual void CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate) override;

	void DataInit();

	//Get Property
	virtual BOOL GetTransferBeamGeneralInfoSet() { return m_bTransferBeamGeneralInfoSet; }
	virtual long GetTransferBeamTopMaxLen2HookAnchorBar() { return m_nTransferBeamTopMaxLen2HookAnchorBar; }
	virtual long GetTransferBeamBotMaxLen2HookAnchorBar() { return m_nTransferBeamBotMaxLen2HookAnchorBar; }
	virtual long GetTransferBeamLineContinuityHorEcc() { return m_nTransferBeamLineContinuityHorEcc; }
	virtual long GetTransferBeamLineContinuityVerEcc() { return m_nTransferBeamLineContinuityVerEcc; }
	virtual long GetTransferBeamBarLayerGap() { return m_nTransferBeamBarLayerGap; }
	virtual long GetTransferBeamMainBarConnectionTolerance() { return m_nTransferBeamMainBarConnectionTolerance; }
	
	virtual BOOL GetTransferBeamBottomBarEnd150Dev() { return  m_bTransferBeamBottomBarEnd150Dev; }
	virtual BOOL GetTransferBeamBottomBarLink150Dev() { return  m_bTransferBeamBottomBarLink150Dev; }

	virtual BOOL GetTransferBeamSubBeamBottomBarEnd150Dev() { return  m_bTransferBeamSubBeamBottomBarEnd150Dev; }
	virtual BOOL GetTransferBeamSubBeamBottomBarLink150Dev() { return  m_bTransferBeamSubBeamBottomBarLink150Dev; }

	virtual BOOL GetTransferBeamSpliceDevConditionSet() { return m_bTransferBeamSpliceDevConditionSet; }
	virtual long GetTransferBeamSpliceDevConditionLength() { return m_nTransferBeamSpliceDevConditionLength; }

	virtual BOOL GetTransferBeamStirrupInfoSet() { return m_bTransferBeamStirrupInfoSet; }
	virtual long GetTransferBeamShearBarType() { return m_nTransferBeamShearBarType; }
	virtual long GetTransferBeamHorShearBarType() { return m_nTransferBeamHorShearBarType; }
	virtual long GetTransferBeamCapbarRatio() { return m_nTransferBeamCapbarRatio; }
	virtual long GetTransferBeamStirrupType() { return m_nTransferBeamStirrupType; }
	virtual long GetTransferBeamCoverBarType() { return m_nTransferBeamCoverBarType; }
	virtual BOOL GetTransferBeamDeepDualUBARPlaceSet() { return m_bTransferBeamDeepDualUBARPlaceSet; }
	virtual long GetTransferBeamDeepDepth() { return m_nTransferBeamDeepDepth; }
	virtual long GetTransferBeamDeepSpliceType() { return m_nTransferBeamDeepSpliceType; }
	virtual long GetTransferBeamStartSpacingType() { return m_nTransferBeamStartSpacingType; }

	virtual BOOL GetTransferBeamSubBeamInfoSet() { return m_bTransferBeamSubBeamInfoSet; }
	virtual long GetTransferBeamSubBeamStirrupType() { return m_nTransferBeamSubBeamStirrupType; }
	virtual long GetTransferBeamSubBeamCapbarRatio() { return m_nTransferBeamSubBeamCapbarRatio; }
	virtual long GetTransferBeamSubBeamStirrupShape() { return m_nTransferBeamSubBeamStirrupShape; }
	virtual BOOL GetTransferBeamSubBeamOnlyCenterStirrupPlace() { return m_bTransferBeamSubBeamOnlyCenterStirrupPlace; }
	virtual long GetTransferBeamSubBeamStartSpacingType() { return m_nTransferBeamSubBeamStartSpacingType; }

	virtual BOOL GetTransferBeamSubBarInfoSet() { return m_bTransferBeamSubBarInfoSet; }
	virtual long GetTransferBeamTopSubStirrupBarShape() { return m_nTransferBeamTopSubStirrupBarShape; }
	virtual long GetTransferBeamBotSubStirrupBarShape() { return m_nTransferBeamBotSubStirrupBarShape; }
	virtual long GetTransferBeamSubStirrupStartSpacing() { return m_nTransferBeamSubStirrupStartSpacing; }
	virtual long GetTransferBeamSubTopStirrupType() { return m_nTransferBeamSubTopStirrupType; }
	virtual long GetTransferBeamSubTopStirrupNum() { return m_nTransferBeamSubTopStirrupNum; }
	virtual long GetTransferBeamSubTopStirrupSpacing() { return m_nTransferBeamSubTopStirrupSpacing; }
	virtual long GetTransferBeamSubBotStirrupType() { return m_nTransferBeamSubBotStirrupType; }
	virtual long GetTransferBeamSubBotStirrupNum() { return m_nTransferBeamSubBotStirrupNum; }
	virtual long GetTransferBeamSubBotStirrupSpacing() { return m_nTransferBeamSubBotStirrupSpacing; }
	virtual long GetTransferBeamSubStirrupTopLen() { return m_nTransferBeamSubStirrupTopLen; }
	virtual long GetTransferBeamSubStirrupBotLen() { return m_nTransferBeamSubStirrupBotLen; }
	virtual BOOL GetTransferBeamSubStirrupUserHookLen() { return m_bTransferBeamSubStirrupUserHookLen; }
	virtual long GetTransferBeamSubStirrupBarType() { return m_nTransferBeamSubStirrupBarType; }

	virtual BOOL GetTransferBeamSupportRebarsSet() { return m_bTransferBeamSupportRebarsSet; }
	virtual long GetTransferBeamSupportRebarsDiaIndex() { return m_nTransferBeamSupportRebarsDiaIndex; }
	virtual double GetTransferBeamSupportRebarsTotalLengthRate() { return m_dTransferBeamSupportRebarsTotalLengthRate; }
	virtual long GetTransferBeamSupportRebarsOrderLength() { return m_nTransferBeamSupportRebarsOrderLength; }
	virtual long GetTransferBeamSupportRebarsSpliceLength() { return m_nTransferBeamSupportRebarsSpliceLength; }
	virtual long GetTransferBeamSupportRebarsCorrectiveLength() { return m_nTransferBeamSupportRebarsCorrectiveLength; }
	
	virtual long GetTransferBeamCapBarPlaceType(){ return m_nTransferBeamCapBarPlaceType; }
	virtual long GetTransferBeamCapBarNum(){ return m_nTransferBeamCapBarNum; }

	virtual long GetTransferBeamSubBeamCapBarPlaceType(){ return m_nTransferBeamSubBeamCapBarPlaceType; }
	virtual long GetTransferBeamSubBeamCapBarNum(){ return m_nTransferBeamSubBeamCapBarNum; }
	virtual BOOL GetTransferBeamSupportRebarUserSteelGradeS() { return m_bTransferBeamSupportRebarUserSteelGradeS; }

	//Set Property
	virtual void SetTransferBeamGeneralInfoSet(BOOL value) { m_bTransferBeamGeneralInfoSet = value; }
	virtual void SetTransferBeamTopMaxLen2HookAnchorBar(long value) { m_nTransferBeamTopMaxLen2HookAnchorBar = value; }
	virtual void SetTransferBeamBotMaxLen2HookAnchorBar(long value) { m_nTransferBeamBotMaxLen2HookAnchorBar = value; }
	virtual void SetTransferBeamLineContinuityHorEcc(long value) { m_nTransferBeamLineContinuityHorEcc = value; }
	virtual void SetTransferBeamLineContinuityVerEcc(long value) { m_nTransferBeamLineContinuityVerEcc = value; }
	virtual void SetTransferBeamBarLayerGap(long value) { m_nTransferBeamBarLayerGap = value; }
	virtual void SetTransferBeamMainBarConnectionTolerance(long value) { m_nTransferBeamMainBarConnectionTolerance = value; }
	
	virtual void SetTransferBeamBottomBarEnd150Dev(BOOL value) { m_bTransferBeamBottomBarEnd150Dev = value; }
	virtual void SetTransferBeamBottomBarLink150Dev(BOOL value) { m_bTransferBeamBottomBarLink150Dev = value; }

	virtual void SetTransferBeamSubBeamBottomBarEnd150Dev(BOOL value) { m_bTransferBeamSubBeamBottomBarEnd150Dev = value; }
	virtual void SetTransferBeamSubBeamBottomBarLink150Dev(BOOL value) { m_bTransferBeamSubBeamBottomBarLink150Dev = value; }

	virtual void SetTransferBeamSpliceDevConditionSet(BOOL value) { m_bTransferBeamSpliceDevConditionSet = value; }
	virtual void SetTransferBeamSpliceDevConditionLength(long value) { m_nTransferBeamSpliceDevConditionLength = value; }

	virtual void SetTransferBeamStirrupInfoSet(BOOL value) { m_bTransferBeamStirrupInfoSet = value; }
	virtual void SetTransferBeamShearBarType(long value) { m_nTransferBeamShearBarType = value; }
	virtual void SetTransferBeamHorShearBarType(long value) { m_nTransferBeamHorShearBarType = value; }
	virtual void SetTransferBeamCapbarRatio(long value) { m_nTransferBeamCapbarRatio = value; }
	virtual void SetTransferBeamStirrupType(long value) { m_nTransferBeamStirrupType = value; }
	virtual void SetTransferBeamCoverBarType(long value) { m_nTransferBeamCoverBarType = value; }
	virtual void SetTransferBeamDeepDualUBARPlaceSet(BOOL value) { m_bTransferBeamDeepDualUBARPlaceSet = value; }
	virtual void SetTransferBeamDeepDepth(long value) { m_nTransferBeamDeepDepth = value; }
	virtual void SetTransferBeamDeepSpliceType(long value) { m_nTransferBeamDeepSpliceType = value; }
	virtual void SetTransferBeamStartSpacingType(long value) { m_nTransferBeamStartSpacingType = value; }

	virtual void SetTransferBeamSubBeamInfoSet(BOOL value) { m_bTransferBeamSubBeamInfoSet = value; }
	virtual void SetTransferBeamSubBeamStirrupType(long value) { m_nTransferBeamSubBeamStirrupType = value; }
	virtual void SetTransferBeamSubBeamCapbarRatio(long value) { m_nTransferBeamSubBeamCapbarRatio = value; }
	virtual void SetTransferBeamSubBeamStirrupShape(long value) { m_nTransferBeamSubBeamStirrupShape = value; }
	virtual void SetTransferBeamSubBeamOnlyCenterStirrupPlace(BOOL value) { m_bTransferBeamSubBeamOnlyCenterStirrupPlace = value; }
	virtual void SetTransferBeamSubBeamStartSpacingType(long value) { m_nTransferBeamSubBeamStartSpacingType = value; }

	virtual void SetTransferBeamSubBarInfoSet(BOOL value) { m_bTransferBeamSubBarInfoSet = value; }
	virtual void SetTransferBeamTopSubStirrupBarShape(long value) { m_nTransferBeamTopSubStirrupBarShape = value; }
	virtual void SetTransferBeamBotSubStirrupBarShape(long value) { m_nTransferBeamBotSubStirrupBarShape = value; }
	virtual void SetTransferBeamSubStirrupStartSpacing(long value) { m_nTransferBeamSubStirrupStartSpacing = value; }
	virtual void SetTransferBeamSubTopStirrupType(long value) { m_nTransferBeamSubTopStirrupType = value; }
	virtual void SetTransferBeamSubTopStirrupNum(long value) { m_nTransferBeamSubTopStirrupNum = value; }
	virtual void SetTransferBeamSubTopStirrupSpacing(long value) { m_nTransferBeamSubTopStirrupSpacing = value; }
	virtual void SetTransferBeamSubBotStirrupType(long value) { m_nTransferBeamSubBotStirrupType = value; }
	virtual void SetTransferBeamSubBotStirrupNum(long value) { m_nTransferBeamSubBotStirrupNum = value; }
	virtual void SetTransferBeamSubBotStirrupSpacing(long value) { m_nTransferBeamSubBotStirrupSpacing = value; }
	virtual void SetTransferBeamSubStirrupTopLen(long value) { m_nTransferBeamSubStirrupTopLen = value; }
	virtual void SetTransferBeamSubStirrupBotLen(long value) { m_nTransferBeamSubStirrupBotLen = value; }
	virtual void SetTransferBeamSubStirrupUserHookLen(BOOL value) { m_bTransferBeamSubStirrupUserHookLen = value; }
	virtual void SetTransferBeamSubStirrupBarType(long value) { m_nTransferBeamSubStirrupBarType = value; }

	virtual void SetTransferBeamSupportRebarsSet(BOOL value) { m_bTransferBeamSupportRebarsSet = value; }
	virtual void SetTransferBeamSupportRebarsDiaIndex(long value) { m_nTransferBeamSupportRebarsDiaIndex = value; }
	virtual void SetTransferBeamSupportRebarsTotalLengthRate(double value) { m_dTransferBeamSupportRebarsTotalLengthRate = value; }
	virtual void SetTransferBeamSupportRebarsOrderLength(long value) { m_nTransferBeamSupportRebarsOrderLength = value; }
	virtual void SetTransferBeamSupportRebarsSpliceLength(long value) { m_nTransferBeamSupportRebarsSpliceLength = value; }
	virtual void SetTransferBeamSupportRebarsCorrectiveLength(long value) { m_nTransferBeamSupportRebarsCorrectiveLength = value; }
	
	virtual void SetTransferBeamCapBarPlaceType(long value){ m_nTransferBeamCapBarPlaceType = value; }
	virtual void SetTransferBeamCapBarNum(long value){ m_nTransferBeamCapBarNum = value; }

	virtual void SetTransferBeamSubBeamCapBarPlaceType(long value){ m_nTransferBeamSubBeamCapBarPlaceType = value; }
	virtual void SetTransferBeamSubBeamCapBarNum(long value){ m_nTransferBeamSubBeamCapBarNum = value; }
	virtual void SetTransferBeamSupportRebarUserSteelGradeS(BOOL value) { m_bTransferBeamSupportRebarUserSteelGradeS = value; }
	//Function
	virtual map<long, long> GetTransferBeamTopSubStirrupHookLenMap() { return mm_TransferBeamTopSubStirrupHookLen; }
	virtual map<long, long> GetTransferBeamBotSubStirrupHookLenMap() { return mm_TransferBeamBotSubStirrupHookLen; }
	virtual map<long, long> GetTransferBeamShearBarHookLenAMap() { return mm_TransferBeamShearBarHookLenA; }
	virtual map<long, long> GetTransferBeamShearBarHookLenBMap() { return mm_TransferBeamShearBarHookLenB; }
	virtual map<long, long> GetTransferBeamTopSubStirrupDiaIndexMap() { return mm_TransferBeamTopSubStirrupDiaIndex; }
	virtual map<long, long> GetTransferBeamBotSubStirrupDiaIndexMap() { return mm_TransferBeamBotSubStirrupDiaIndex; }

	virtual void SetTransferBeamTopSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen);
	virtual void SetTransferBeamBotSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen);
	virtual void SetTransferBeamShearBarHookLenAMap(map<long, long> mapShearBarHookLenA);
	virtual void SetTransferBeamShearBarHookLenBMap(map<long, long> mapShearBarHookLenB);
	virtual void SetTransferBeamTopSubStirrupDiaIndexMap(map<long, long> mapSubStirrupDiaIndex);
	virtual void SetTransferBeamBotSubStirrupDiaIndexMap(map<long, long> mapSubStirrupDiaIndex);

	virtual double GetTransferBeamTopSubStirrupHookLen(double dDia);
	virtual double GetTransferBeamBotSubStirrupHookLen(double dDia);
	virtual double GetTransferBeamShearBarHookLenA(double dDia);
	virtual double GetTransferBeamShearBarHookLenB(double dDia);
	virtual double GetTransferBeamTopSubStirrupDia(double dDia);
	virtual double GetTransferBeamBotSubStirrupDia(double dDia);

	virtual void SetTransferBeamTopSubStirrupHookLen(long key, long value) { mm_TransferBeamTopSubStirrupHookLen[key] = value; }
	virtual void SetTransferBeamBotSubStirrupHookLen(long key, long value) { mm_TransferBeamBotSubStirrupHookLen[key] = value; }
	virtual void SetTransferBeamShearBarHookLenA(long key, long value) { mm_TransferBeamShearBarHookLenA[key] = value; }
	virtual void SetTransferBeamShearBarHookLenB(long key, long value) { mm_TransferBeamShearBarHookLenB[key] = value; }
	virtual void SetTransferBeamTopSubStirrupDiaIndex(long key, long value) { mm_TransferBeamTopSubStirrupDiaIndex[key] = value; }
	virtual void SetTransferBeamBotSubStirrupDiaIndex(long key, long value) { mm_TransferBeamBotSubStirrupDiaIndex[key] = value; }
	
	virtual ShearFrictionBarTemplate *GetTransferBeamShearFrictionBarTemplate();

	virtual BOOL GetTransferBeamSupportRebarsPlaceConditionSet() { return m_bTransferBeamSupportRebarsPlaceConditionSet; }
	virtual long GetTransferBeamSupportRebarsPlaceCondition() { return m_nTransferBeamSupportRebarsPlaceCondition; }
	virtual void SetTransferBeamSupportRebarsPlaceConditionSet(BOOL value) { m_bTransferBeamSupportRebarsPlaceConditionSet = value; }
	virtual void SetTransferBeamSupportRebarsPlaceCondition(long value) { m_nTransferBeamSupportRebarsPlaceCondition = value; }

	virtual long GetTransferBeamPlaceTypeDiaIndex() { return m_nTransferBeamPlaceTypeDiaIndex; }
	virtual void SetTransferBeamPlaceTypeDiaIndex(long value) { m_nTransferBeamPlaceTypeDiaIndex = value; }

	virtual long GetTransferBeamSpliceTypeBelowDia() { return m_nTransferBeamSpliceTypeBelowDia; }
	virtual long GetTransferBeamSpliceWayBelowDia() { return m_nTransferBeamSpliceWayBelowDia; }
	virtual long GetTransferBeamCZoneSpliceWayBelowDia() { return m_nTransferBeamCZoneSpliceWayBelowDia; }
	virtual long GetTransferBeamCZoneSpliceLengthBelowDia() { return m_nTransferBeamCZoneSpliceLengthBelowDia; }

	virtual long GetTransferBeamSpliceTypeExcessDia() { return m_nTransferBeamSpliceTypeExcessDia; }
	virtual long GetTransferBeamSpliceWayExcessDia() { return m_nTransferBeamSpliceWayExcessDia; }
	virtual long GetTransferBeamCZoneSpliceWayExcessDia() { return m_nTransferBeamCZoneSpliceWayExcessDia; }
	virtual long GetTransferBeamCZoneSpliceLengthExcessDia() { return m_nTransferBeamCZoneSpliceLengthExcessDia; }

	virtual void SetTransferBeamSpliceTypeBelowDia(long value) { m_nTransferBeamSpliceTypeBelowDia = value; }
	virtual void SetTransferBeamSpliceWayBelowDia(long value) { m_nTransferBeamSpliceWayBelowDia = value; }
	virtual void SetTransferBeamCZoneSpliceWayBelowDia(long value) { m_nTransferBeamCZoneSpliceWayBelowDia = value; }
	virtual void SetTransferBeamCZoneSpliceLengthBelowDia(long value) { m_nTransferBeamCZoneSpliceLengthBelowDia = value; }

	virtual void SetTransferBeamSpliceTypeExcessDia(long value) { m_nTransferBeamSpliceTypeExcessDia = value; }
	virtual void SetTransferBeamSpliceWayExcessDia(long value) { m_nTransferBeamSpliceWayExcessDia = value; }
	virtual void SetTransferBeamCZoneSpliceWayExcessDia(long value) { m_nTransferBeamCZoneSpliceWayExcessDia = value; }
	virtual void SetTransferBeamCZoneSpliceLengthExcessDia(long value) { m_nTransferBeamCZoneSpliceLengthExcessDia = value; }

	virtual long GetTransferBeamSpliceType(double dDia);
	virtual long GetTransferBeamSpliceWay(double dDia);
	virtual long GetTransferBeamCZoneSpliceWay(double dDia);
	virtual long GetTransferBeamCZoneSpliceLength(double dDia);

	virtual BOOL GetTransferBeamSpliceAddLengthSet() { return m_bTransferBeamSpliceAddLengthSet; }
	virtual void SetTransferBeamSpliceAddLengthSet(BOOL value) { m_bTransferBeamSpliceAddLengthSet = value; }
	virtual long GetTransferBeamSpliceAddLengthNumSplice() { return m_nTransferBeamSpliceAddLengthNumSplice; }
	virtual void SetTransferBeamSpliceAddLengthNumSplice(long value) { m_nTransferBeamSpliceAddLengthNumSplice = value; }

	virtual BOOL GetTransferBeamPlaceHorShearBar() { return m_bTransferBeamPlaceHorShearBar; }
	virtual long GetTransferBeamHorShearBarMainDirDiaIndex() { return m_nTransferBeamHorShearBarMainDirDiaIndex; }
	virtual long GetTransferBeamHorShearBarMainDirPlaceType() { return m_nTransferBeamHorShearBarMainDirPlaceType; }
	virtual long GetTransferBeamHorShearBarMainDirSpacing() { return m_nTransferBeamHorShearBarMainDirSpacing; }
	virtual long GetTransferBeamHorShearBarMainDirNumBar() { return m_nTransferBeamHorShearBarMainDirNumBar; }
	virtual long GetTransferBeamHorShearBarStirrupDirDiaIndex() { return m_nTransferBeamHorShearBarStirrupDirDiaIndex; }
	virtual long GetTransferBeamHorShearBarStirrupDirSpacing() { return m_nTransferBeamHorShearBarStirrupDirSpacing; }
	virtual BOOL GetTransferBeamHorShearBarMainDirUserSteelGradeS() { return m_bTransferBeamHorShearBarMainDirUserSteelGradeS; }
	virtual BOOL GetTransferBeamHorShearBarStirrupDirUserSteelGradeS() { return m_bTransferBeamHorShearBarStirrupDirUserSteelGradeS; }

	virtual void SetTransferBeamPlaceHorShearBar(BOOL value) { m_bTransferBeamPlaceHorShearBar = value; }
	virtual void SetTransferBeamHorShearBarMainDirDiaIndex(long value) { m_nTransferBeamHorShearBarMainDirDiaIndex = value; }
	virtual void SetTransferBeamHorShearBarMainDirPlaceType(long value) { m_nTransferBeamHorShearBarMainDirPlaceType = value; }
	virtual void SetTransferBeamHorShearBarMainDirSpacing(long value) { m_nTransferBeamHorShearBarMainDirSpacing = value; }
	virtual void SetTransferBeamHorShearBarMainDirNumBar(long value) { m_nTransferBeamHorShearBarMainDirNumBar = value; }
	virtual void SetTransferBeamHorShearBarStirrupDirDiaIndex(long value) { m_nTransferBeamHorShearBarStirrupDirDiaIndex = value; }
	virtual void SetTransferBeamHorShearBarStirrupDirSpacing(long value) { m_nTransferBeamHorShearBarStirrupDirSpacing = value; }
	virtual void SetTransferBeamHorShearBarMainDirUserSteelGradeS(BOOL value) { m_bTransferBeamHorShearBarMainDirUserSteelGradeS = value; }
	virtual void SetTransferBeamHorShearBarStirrupDirUserSteelGradeS(BOOL value) { m_bTransferBeamHorShearBarStirrupDirUserSteelGradeS = value; }
private:
	ShearFrictionBarTemplate *m_pTransferBeamShearFrictionBarTemplate;
	//주문길이보다 짧고, 양단 후크인 철근의 최대길이
	BOOL m_bTransferBeamGeneralInfoSet;
	long m_nTransferBeamTopMaxLen2HookAnchorBar;	//상부근 최대 양후크 길이
	long m_nTransferBeamBotMaxLen2HookAnchorBar;	//하부근 최대 양후크 길이
	long m_nTransferBeamLineContinuityHorEcc;//보연속성판단기준 수평편심
	long m_nTransferBeamLineContinuityVerEcc;//보연속성판단기준 수직편심
	long m_nTransferBeamBarLayerGap;			//단사이간격
	long m_nTransferBeamMainBarConnectionTolerance;//주근 연결 허용길이
	
	long m_nTransferBeamPlaceTypeDiaIndex;
	long m_nTransferBeamSpliceTypeBelowDia;	//이음 타입 0:인장이음 1:압축이음
	long m_nTransferBeamSpliceWayBelowDia;	//이음 방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nTransferBeamCZoneSpliceWayBelowDia;
	long m_nTransferBeamCZoneSpliceLengthBelowDia;

	long m_nTransferBeamSpliceTypeExcessDia;	//이음 타입 0:인장이음 1:압축이음
	long m_nTransferBeamSpliceWayExcessDia;	//이음 방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nTransferBeamCZoneSpliceWayExcessDia;
	long m_nTransferBeamCZoneSpliceLengthExcessDia;

	BOOL m_bTransferBeamBottomBarEnd150Dev;
	BOOL m_bTransferBeamBottomBarLink150Dev;
	BOOL m_bTransferBeamSubBeamBottomBarEnd150Dev;
	BOOL m_bTransferBeamSubBeamBottomBarLink150Dev;


	BOOL m_bTransferBeamSpliceDevConditionSet;
	long m_nTransferBeamSpliceDevConditionLength;

	BOOL m_bTransferBeamStirrupInfoSet;
	long m_nTransferBeamShearBarType;	//전단근 형상
	long m_nTransferBeamHorShearBarType;	//수평 전단근 형상
	long m_nTransferBeamCapBarPlaceType;//캡바타입
	long m_nTransferBeamCapBarNum;//캡바개수
	long m_nTransferBeamCapbarRatio;	//캡바 비율
	
	long m_nTransferBeamStirrupType;	//스트럽 형상
	long m_nTransferBeamCoverBarType;	//상부CAP 형상
	BOOL m_bTransferBeamDeepDualUBARPlaceSet;	//깊은보 양U-BAR 배근
	long m_nTransferBeamDeepDepth;		//깊은보 춤 이상
	long m_nTransferBeamDeepSpliceType;		//이음 타입 0:인장이음 1:압축이음
	long m_nTransferBeamStartSpacingType;		//시작간격 설정 0:S/2 1:50

	BOOL m_bTransferBeamSubBeamInfoSet;
	long m_nTransferBeamSubBeamStirrupType;			//0:개방형 스트럽배근 1:폐쇄형 스트럽배근
	long m_nTransferBeamSubBeamCapBarPlaceType;//캡바타입
	long m_nTransferBeamSubBeamCapBarNum;//캡바개수
	long m_nTransferBeamSubBeamCapbarRatio;			//캡바 비율
	long m_nTransferBeamSubBeamStirrupShape;		//0:90*90(내측) 1:90*90(외측)
	BOOL m_bTransferBeamSubBeamOnlyCenterStirrupPlace;	//중앙부만 개방형 스트럽배근
	long m_nTransferBeamSubBeamStartSpacingType;		//시작간격 설정 0:S/2 1:50

	//이단받침철근(SubStirrupBar)옵션 추가-20140902
	//-2DShopPro와 변수명 동일하게
	BOOL m_bTransferBeamSubBarInfoSet;
	long m_nTransferBeamTopSubStirrupBarShape;//상부받침철근형상
	long m_nTransferBeamBotSubStirrupBarShape;//하부받침철근형상
	long m_nTransferBeamSubTopStirrupNum;//상부받침철근 개수
	long m_nTransferBeamSubBotStirrupNum;//하부받침철근 개수
	long m_nTransferBeamSubStirrupTopLen;//상부스트럽높이
	long m_nTransferBeamSubStirrupBotLen;//하부스트럽높이 = (스트럽높이-m_dBeamSubStirrupBotLen)
	BOOL m_bTransferBeamSubStirrupUserHookLen;//true : 받침철근 발길이 및 전단보강근 A,B길이 설정을 사용자 설정으로
	long m_nTransferBeamSubStirrupBarType;	//0:주근 1:스터럽
	map<long, long> mm_TransferBeamTopSubStirrupHookLen;		//DiaIndex별 받침철근 발길이
	map<long, long> mm_TransferBeamBotSubStirrupHookLen;		//DiaIndex별 받침철근 발길이
	map<long, long> mm_TransferBeamShearBarHookLenA;		//DiaIndex별 전단보강근 발길이A
	map<long, long> mm_TransferBeamShearBarHookLenB;		//DiaIndex별 전단보강근 발길이B
	map<long, long> mm_TransferBeamTopSubStirrupDiaIndex;		//DiaIndex별 받침철근 직경설정 값
	map<long, long> mm_TransferBeamBotSubStirrupDiaIndex;		//DiaIndex별 받침철근 직경설정 값

	//BH-3617 보 2단 받침철근 간격배근 지원 추가
	long m_nTransferBeamSubStirrupStartSpacing; //받침철근 시작 간격
	long m_nTransferBeamSubTopStirrupType; //상부받침철근 타입
	long m_nTransferBeamSubTopStirrupSpacing;//상부받침철근 간격
	long m_nTransferBeamSubBotStirrupType; //하부받침철근 타입
	long m_nTransferBeamSubBotStirrupSpacing;//상부받침철근 간격

	BOOL m_bTransferBeamSupportRebarsSet;					//하부 도리근 설정
	long m_nTransferBeamSupportRebarsDiaIndex;				//직경
	double m_dTransferBeamSupportRebarsTotalLengthRate;	//총 길이 비율
	long m_nTransferBeamSupportRebarsOrderLength;			//주문길이
	long m_nTransferBeamSupportRebarsSpliceLength;			//이음길이
	long m_nTransferBeamSupportRebarsCorrectiveLength;		//보정길이
	BOOL m_bTransferBeamSupportRebarUserSteelGradeS;
	BOOL m_bTransferBeamSupportRebarsPlaceConditionSet;
	long m_nTransferBeamSupportRebarsPlaceCondition;
	//BOOL m_bBuiltUpBeamInfoSet;
	//long m_nBuiltUpBeamPlacerType;	//덧살보 배근

	BOOL m_bTransferBeamSpliceAddLengthSet;
	long m_nTransferBeamSpliceAddLengthNumSplice;

	BOOL m_bTransferBeamPlaceHorShearBar;
	long m_nTransferBeamHorShearBarMainDirDiaIndex;
	long m_nTransferBeamHorShearBarMainDirPlaceType;
	long m_nTransferBeamHorShearBarMainDirSpacing;
	long m_nTransferBeamHorShearBarMainDirNumBar;
	long m_nTransferBeamHorShearBarStirrupDirDiaIndex;
	long m_nTransferBeamHorShearBarStirrupDirSpacing;
	BOOL m_bTransferBeamHorShearBarMainDirUserSteelGradeS;
	BOOL m_bTransferBeamHorShearBarStirrupDirUserSteelGradeS;
};

