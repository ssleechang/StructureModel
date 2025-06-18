#pragma once
#include "OldBaseRebarTemplate.h"
#include "..\IsoFootRebarTemplate.h"

class StructureModel_CLASS OldIsoFootRebarTemplate : public OldBaseRebarTemplate
{
public:
	OldIsoFootRebarTemplate();
	~OldIsoFootRebarTemplate(void);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	DECLARE_SERIAL(OldIsoFootRebarTemplate)
	void Serialize(CArchive& ar);

	virtual void CopyFromMe(OldIsoFootRebarTemplate* pIsoFootRebarTemplate);

	void DataInit();

	bool TopOptionDataCopy(OldIsoFootRebarTemplate* rebarTemplate);
	void DataCopy(OldIsoFootRebarTemplate* rebarTemplate);
	void OldDataCopy(IsoFootRebarTemplate* rebarTemplate);
	void InfoSetCheckBoxChecked();

	virtual BOOL GetIsoFootTopSpacerInfoSet() { return m_bIsoFootTopSpacerInfoSet; }
	virtual BOOL GetIsoFootTopSpacerSet() { return m_bIsoFootTopSpacerSet; }
	virtual long GetIsoFootTopSpacerDiaType() { return m_nIsoFootTopSpacerDiaType; }
	virtual long GetIsoFootTopSpacerDiaIndex() { return m_nIsoFootTopSpacerDiaIndex; }
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

	virtual void SetIsoFootTopSpacerInfoSet(BOOL value) { m_bIsoFootTopSpacerInfoSet = value; }
	virtual void SetIsoFootTopSpacerSet(BOOL value) { m_bIsoFootTopSpacerSet = value; }
	virtual void SetIsoFootTopSpacerDiaType(long value) { m_nIsoFootTopSpacerDiaType = value; }
	virtual void SetIsoFootTopSpacerDiaIndex(long value) { m_nIsoFootTopSpacerDiaIndex = value; }
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

private:
	BOOL m_bIsoFootTopSpacerInfoSet;
	BOOL m_bIsoFootTopSpacerSet;			//상부 우마철근 설정
	long m_nIsoFootTopSpacerDiaType;		//직경 0:해당기초직경 1:사용자직경
	long m_nIsoFootTopSpacerDiaIndex;		//직경 : 사용자 직경 입력
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
};

