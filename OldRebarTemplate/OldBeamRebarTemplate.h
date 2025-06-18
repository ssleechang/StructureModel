#pragma once
#include "OldBaseRebarTemplate.h"
#include "..\BeamRebarTemplate.h"

class StructureModel_CLASS OldBeamRebarTemplate : public OldBaseRebarTemplate
{
public:
	OldBeamRebarTemplate();
	~OldBeamRebarTemplate(void);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	DECLARE_SERIAL(OldBeamRebarTemplate)
	void Serialize(CArchive& ar);

	virtual void CopyFromMe(OldBeamRebarTemplate* pBeamRebarTemplate);

	void DataInit();

	bool TopOptionDataCopy(OldBeamRebarTemplate* rebarTemplate);
	void DataCopy(OldBeamRebarTemplate* rebarTemplate);
	void OldDataCopy(BeamRebarTemplate* rebarTemplate);
	void InfoSetCheckBoxChecked();

	//Get Property
	virtual BOOL GetBeamGeneralInfoSet() { return m_bBeamGeneralInfoSet; }
	virtual long GetBeamTopMaxLen2HookAnchorBar() { return m_nBeamTopMaxLen2HookAnchorBar; }
	virtual long GetBeamBotMaxLen2HookAnchorBar() { return m_nBeamBotMaxLen2HookAnchorBar; }
	virtual long GetBeamTopSpliceType() { return m_nBeamTopSpliceType; }
	virtual long GetBeamTopSpliceWay() { return m_nBeamTopSpliceWay; }

	virtual BOOL GetBeamStirrupInfoSet() { return m_bBeamStirrupInfoSet; }
	virtual long GetBeamShearBarType() { return m_nBeamShearBarType; }
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
	virtual long GetBeamSubTopStirrupNum() { return m_nBeamSubTopStirrupNum; }
	virtual long GetBeamSubBotStirrupNum() { return m_nBeamSubBotStirrupNum; }
	virtual long GetBeamSubStirrupTopLen() { return m_nBeamSubStirrupTopLen; }
	virtual long GetBeamSubStirrupBotLen() { return m_nBeamSubStirrupBotLen; }
	virtual BOOL GetBeamSubStirrupUserHookLen() { return m_bBeamSubStirrupUserHookLen; }
	virtual long GetBeamSubStirrupBarType() { return m_nBeamSubStirrupBarType; }

	virtual BOOL GetBuiltUpBeamInfoSet() { return m_bBuiltUpBeamInfoSet; }
	virtual long GetBuiltUpBeamPlacerType() { return m_nBuiltUpBeamPlacerType; }

	//Set Property
	virtual void SetBeamGeneralInfoSet(BOOL value) { m_bBeamGeneralInfoSet = value; }
	virtual void SetBeamTopMaxLen2HookAnchorBar(long value) { m_nBeamTopMaxLen2HookAnchorBar = value; }
	virtual void SetBeamBotMaxLen2HookAnchorBar(long value) { m_nBeamBotMaxLen2HookAnchorBar = value; }
	virtual void SetBeamTopSpliceType(long value) { m_nBeamTopSpliceType = value; }
	virtual void SetBeamTopSpliceWay(long value) { m_nBeamTopSpliceWay = value; }

	virtual void SetBeamStirrupInfoSet(BOOL value) { m_bBeamStirrupInfoSet = value; }
	virtual void SetBeamShearBarType(long value) { m_nBeamShearBarType = value; }
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
	virtual void SetBeamSubTopStirrupNum(long value) { m_nBeamSubTopStirrupNum = value; }
	virtual void SetBeamSubBotStirrupNum(long value) { m_nBeamSubBotStirrupNum = value; }
	virtual void SetBeamSubStirrupTopLen(long value) { m_nBeamSubStirrupTopLen = value; }
	virtual void SetBeamSubStirrupBotLen(long value) { m_nBeamSubStirrupBotLen = value; }
	virtual void SetBeamSubStirrupUserHookLen(BOOL value) { m_bBeamSubStirrupUserHookLen = value; }
	virtual void SetBeamSubStirrupBarType(long value) { m_nBeamSubStirrupBarType = value; }

	virtual void SetBuiltUpBeamInfoSet(BOOL value) { m_bBuiltUpBeamInfoSet = value; }
	virtual void SetBuiltUpBeamPlacerType(long value) { m_nBuiltUpBeamPlacerType = value; }

	//Function
	virtual map<long, long> GetBeamSubStirrupHookLenMap() { return mm_BeamSubStirrupHookLen; }
	virtual map<long, long> GetBeamShearBarHookLenAMap() { return mm_BeamShearBarHookLenA; }
	virtual map<long, long> GetBeamShearBarHookLenBMap() { return mm_BeamShearBarHookLenB; }
	virtual map<long, long> GetBeamSubStirrupDiaIndexMap() { return mm_BeamSubStirrupDiaIndex; }

	virtual void SetBeamSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen);
	virtual void SetBeamShearBarHookLenAMap(map<long, long> mapShearBarHookLenA);
	virtual void SetBeamShearBarHookLenBMap(map<long, long> mapShearBarHookLenB);
	virtual void SetBeamSubStirrupDiaIndexMap(map<long, long> mapSubStirrupDiaIndex);

	virtual double GetBeamSubStirrupHookLen(double dDia);
	virtual double GetBeamShearBarHookLenA(double dDia);
	virtual double GetBeamShearBarHookLenB(double dDia);
	virtual double GetBeamSubStirrupDia(double dDia);

	virtual void SetBeamSubStirrupHookLen(long key, long value) { mm_BeamSubStirrupHookLen[key] = value; }
	virtual void SetBeamShearBarHookLenA(long key, long value) { mm_BeamShearBarHookLenA[key] = value; }
	virtual void SetBeamShearBarHookLenB(long key, long value) { mm_BeamShearBarHookLenB[key] = value; }
	virtual void SetBeamSubStirrupDiaIndex(long key, long value) { mm_BeamSubStirrupDiaIndex[key] = value; }
	
private:
	//주문길이보다 짧고, 양단 후크인 철근의 최대길이
	BOOL m_bBeamGeneralInfoSet;
	long m_nBeamTopMaxLen2HookAnchorBar;	//상부근 최대 양후크 길이
	long m_nBeamBotMaxLen2HookAnchorBar;	//하부근 최대 양후크 길이
	long m_nBeamTopSpliceType;	//이음 타입 0:인장이음 1:압축이음
	long m_nBeamTopSpliceWay;	//이음 방식 0:겹침이음 1:커플러이음 2:압접이음

	BOOL m_bBeamStirrupInfoSet;
	long m_nBeamShearBarType;	//전단근 형상
	long m_nBeamCapbarRatio;	//캡바 비율
	long m_nBeamStirrupType;	//스트럽 형상
	long m_nBeamCoverBarType;	//상부CAP 형상
	BOOL m_bBeamDeepDualUBARPlaceSet;	//깊은보 양U-BAR 배근
	long m_nBeamDeepDepth;		//깊은보 춤 이상
	long m_nBeamDeepSpliceType;		//이음 타입 0:인장이음 1:압축이음
	long m_nBeamStartSpacingType;		//시작간격 설정 0:S/2 1:50

	BOOL m_bBeamSubBeamInfoSet;
	long m_nSubBeamStirrupType;			//0:개방형 스트럽배근 1:폐쇄형 스트럽배근
	long m_nSubBeamCapbarRatio;			//캡바 비율
	long m_nSubBeamStirrupShape;		//0:90*90(내측) 1:90*90(외측)
	BOOL m_bSubBeamOnlyCenterStirrupPlace;	//중앙부만 개방형 스트럽배근
	long m_nSubBeamStartSpacingType;		//시작간격 설정 0:S/2 1:50

	//이단받침철근(SubStirrupBar)옵션 추가-20140902
	//-2DShopPro와 변수명 동일하게
	BOOL m_bBeamSubBarInfoSet;
	long m_nBeamTopSubStirrupBarShape;//상부받침철근형상
	long m_nBeamBotSubStirrupBarShape;//하부받침철근형상
	long m_nBeamSubTopStirrupNum;//상부받침철근 개수;
	long m_nBeamSubBotStirrupNum;//하부받침철근 개수
	long m_nBeamSubStirrupTopLen;//상부스트럽높이
	long m_nBeamSubStirrupBotLen;//하부스트럽높이 = (스트럽높이-m_dBeamSubStirrupBotLen)
	BOOL m_bBeamSubStirrupUserHookLen;//true : 받침철근 발길이 및 전단보강근 A,B길이 설정을 사용자 설정으로
	long m_nBeamSubStirrupBarType;	//0:주근 1:스터럽
	map<long, long> mm_BeamSubStirrupHookLen;		//DiaIndex별 받침철근 발길이
	map<long, long> mm_BeamShearBarHookLenA;		//DiaIndex별 전단보강근 발길이A
	map<long, long> mm_BeamShearBarHookLenB;		//DiaIndex별 전단보강근 발길이B
	map<long, long> mm_BeamSubStirrupDiaIndex;		//DiaIndex별 받침철근 직경설정 값

	BOOL m_bBuiltUpBeamInfoSet;
	long m_nBuiltUpBeamPlacerType;	//덧살보 배근
};

