#pragma once
#include "OldBaseRebarTemplate.h"
#include "..\WallFootRebarTemplate.h"

class StructureModel_CLASS OldWallFootRebarTemplate : public OldBaseRebarTemplate
{
public:
	OldWallFootRebarTemplate();
	~OldWallFootRebarTemplate(void);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	DECLARE_SERIAL(OldWallFootRebarTemplate)
	void Serialize(CArchive& ar);

	virtual void CopyFromMe(OldWallFootRebarTemplate* pWallFootRebarTemplate);

	void DataInit();

	bool TopOptionDataCopy(OldWallFootRebarTemplate* rebarTemplate);
	void DataCopy(OldWallFootRebarTemplate* rebarTemplate);
	void OldDataCopy(WallFootRebarTemplate* rebarTemplate);
	void InfoSetCheckBoxChecked();

	virtual BOOL GetWallFootTopSpacerInfoSet() { return m_bWallFootTopSpacerInfoSet; }
	virtual BOOL GetWallFootTopSpacerSet() { return m_bWallFootTopSpacerSet; }
	virtual long GetWallFootTopSpacerDiaType() { return m_nWallFootTopSpacerDiaType; }
	virtual long GetWallFootTopSpacerDiaIndex() { return m_nWallFootTopSpacerDiaIndex; }
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

	virtual void SetWallFootTopSpacerInfoSet(BOOL value) { m_bWallFootTopSpacerInfoSet = value; }
	virtual void SetWallFootTopSpacerSet(BOOL value) { m_bWallFootTopSpacerSet = value; }
	virtual void SetWallFootTopSpacerDiaType(long value) { m_nWallFootTopSpacerDiaType = value; }
	virtual void SetWallFootTopSpacerDiaIndex(long value) { m_nWallFootTopSpacerDiaIndex = value; }
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

private:
	BOOL m_bWallFootTopSpacerInfoSet;
	BOOL m_bWallFootTopSpacerSet;			//상부 우마철근 설정
	long m_nWallFootTopSpacerDiaType;		//직경 0:해당기초직경 1:사용자직경
	long m_nWallFootTopSpacerDiaIndex;		//직경 : 사용자 직경 입력
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
};

