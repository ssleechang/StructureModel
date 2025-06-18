#pragma once
#include "OldBaseRebarTemplate.h"
#include "..\PitRebarTemplate.h"

class StructureModel_CLASS OldPitRebarTemplate : public OldBaseRebarTemplate
{
public:
	OldPitRebarTemplate();
	~OldPitRebarTemplate(void);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	DECLARE_SERIAL(OldPitRebarTemplate)
	void Serialize(CArchive& ar);

	virtual void CopyFromMe(OldPitRebarTemplate* pPitRebarTemplate);

	void DataInit();

	bool TopOptionDataCopy(OldPitRebarTemplate* rebarTemplate);
	void DataCopy(OldPitRebarTemplate* rebarTemplate);
	void OldDataCopy(PitRebarTemplate* rebarTemplate);
	void InfoSetCheckBoxChecked();

	virtual BOOL GetPitGeneralInfoSet() { return m_bPitGeneralInfoSet; }
	virtual long GetPitMaxLen2HookAnchorBar() { return m_nPitMaxLen2HookAnchorBar; }

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
	virtual long GetPitTopSpacerDiaIndex() { return m_nPitTopSpacerDiaIndex; }
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

	virtual void SetPitGeneralInfoSet(BOOL value) { m_bPitGeneralInfoSet = value; }
	virtual void SetPitMaxLen2HookAnchorBar(long value) { m_nPitMaxLen2HookAnchorBar = value; }

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
	virtual void SetPitTopSpacerDiaIndex(long value) { m_nPitTopSpacerDiaIndex = value; }
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

private:
	BOOL m_bPitGeneralInfoSet;
	long m_nPitMaxLen2HookAnchorBar;	//수평근 최대 양후크 길이

	BOOL m_bPitVerInfoSet;
	long m_nPitVerOutTopHookType;	//수직근 설정 외부 상부
	long m_nPitVerOutBotHookType;	//수직근 설정 외부 하부
	long m_nPitVerInTopHookType;	//수직근 설정 내부 상부
	long m_nPitVerInBotHookType;	//수직근 설정 내부 하부

	BOOL m_bPitHorInfoSet;
	long m_nPitHorOutHookType;		//수평근 설정 외부
	long m_nPitHorInHookType;		//수평근 설정 내부

	BOOL m_bPitTopSpacerInfoSet;
	BOOL m_bPitTopSpacerSet;			//상부 우마철근 설정
	long m_nPitTopSpacerDiaType;		//직경 0:해당기초직경 1:사용자직경
	long m_nPitTopSpacerDiaIndex;		//직경 : 사용자 직경 입력
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
};

