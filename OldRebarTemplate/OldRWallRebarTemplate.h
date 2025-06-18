#pragma once
#include "OldBaseRebarTemplate.h"
#include "..\RWallRebarTemplate.h"

class StructureModel_CLASS OldRWallRebarTemplate : public OldBaseRebarTemplate
{
public:
	OldRWallRebarTemplate();
	~OldRWallRebarTemplate(void);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	DECLARE_SERIAL(OldRWallRebarTemplate)
	void Serialize(CArchive& ar);

	virtual void CopyFromMe(OldRWallRebarTemplate* pRWallRebarTemplate);

	void DataInit();

	bool TopOptionDataCopy(OldRWallRebarTemplate* rebarTemplate);
	void DataCopy(OldRWallRebarTemplate* rebarTemplate);
	void OldDataCopy(RWallRebarTemplate* rebarTemplate);
	void InfoSetCheckBoxChecked();

	//Get Property
	virtual BOOL GetRWallHorInfoSet() { return m_bRWallHorInfoSet; }
	virtual BOOL GetRWallHorBarStartSet() { return m_bRWallHorBarStartSet; }
	virtual long GetRWallHorBarStartLength() { return m_nRWallHorBarStartLength; }
	virtual long GetRWallEndUbar() { return m_nRWallEndUbar; }
	virtual BOOL GetRWallDefaultUbarLen() { return m_bRWallDefaultUbarLen; }

	virtual BOOL GetRWallSpacerInfoSet() { return m_bRWallSpacerInfoSet; }
	virtual long GetRWallSpacerDiaType() { return m_nRWallSpacerDiaType; }
	virtual long GetRWallSpacerDiaIndex() { return m_nRWallSpacerDiaIndex; }
	virtual long GetRWallSpacerHorSpacing() { return m_nRWallSpacerHorSpacing; }
	virtual long GetRWallSpacerVerSpacingType() { return m_nRWallSpacerVerSpacingType; }
	virtual long GetRWallSpacerVerSpacing() { return m_nRWallSpacerVerSpacing; }
	virtual long GetRWallSpacerVerSpacingRate() { return m_nRWallSpacerVerSpacingRate; }
	virtual long GetRWallSpacerVerSpacingCount() { return m_nRWallSpacerVerSpacingCount; }
	virtual long GetRWallDowelSpacerVerSpacingCount() { return m_nRWallDowelSpacerVerSpacingCount; }
	virtual long GetRWallSpacerLenType() { return m_nRWallSpacerLenType; }
	virtual long GetRWallSpacerLenCalWay() { return m_nRWallSpacerLenCalWay; }
	virtual long GetRWallSpacerLen1() { return m_nRWallSpacerLen1; }
	virtual long GetRWallSpacerLen2() { return m_nRWallSpacerLen2; }
	virtual long GetRWallSpacerShape() { return m_nRWallSpacerShape; }
	virtual BOOL GetRWallSpacerRebarPlaceSet() { return m_bRWallSpacerRebarPlaceSet; }
	virtual long GetRWallSpacerRebarPlaceCondition() { return m_nRWallSpacerRebarPlaceCondition; }

	virtual BOOL GetRWallVerInfoSet() { return m_bRWallVerInfoSet; }
	virtual long GetRWallVerBarTopLenType() { return m_nRWallVerBarTopLenType; }
	virtual long GetRWallVerBarSpliceWay() { return m_nRWallVerBarSpliceWay; }
	virtual BOOL GetRWallVerBarAlterSpliceSet() { return m_bRWallVerBarAlterSpliceSet; }
	virtual long GetRWallVerBarAlterSpliceValue1() { return m_nRWallVerBarAlterSpliceValue1; }
	virtual long GetRWallVerBarAlterSpliceValue2() { return m_nRWallVerBarAlterSpliceValue2; }

	virtual BOOL GetRWallDowelInfoSet() { return m_bRWallDowelInfoSet; }
	virtual BOOL GetRWallUserDowelHookLen() { return m_bRWallUserDowelHookLen; }
	virtual long GetRWallDowelBarAnchorType() { return m_nRWallDowelBarAnchorType; }
	virtual long GetRWallDowelBarTopLenType() { return m_nRWallDowelBarTopLenType; }
	virtual long GetRWallDowelSupportBarDiaIndex() { return m_nRWallDowelSupportBarDiaIndex; }
	virtual long GetRWallDowelSupportBarNum() { return m_nRWallDowelSupportBarNum; }
	virtual BOOL GetRWallDowelSupportPlaceType() { return m_bRWallDowelSupportPlaceType; }
	virtual long GetRWallDowelBarSpliceWay() { return m_nRWallDowelBarSpliceWay; }
	virtual BOOL GetRWallDowelBarAlterSpliceSet() { return m_bRWallDowelBarAlterSpliceSet; }
	virtual long GetRWallDowelBarAlterSpliceValue1() { return m_nRWallDowelBarAlterSpliceValue1; }
	virtual long GetRWallDowelBarAlterSpliceValue2() { return m_nRWallDowelBarAlterSpliceValue2; }
	virtual BOOL GetRWallDowelHookAnchorPlaceSpacingSet() { return m_bRWallDowelHookAnchorPlaceSpacingSet; }
	virtual long GetRWallDowelHookAnchorPlaceSpacing() { return m_nRWallDowelHookAnchorPlaceSpacing; }
	virtual BOOL GetRWallDowelBotAddBarIntegratedPlaceSet() { return m_bRWallDowelBotAddBarIntegratedPlaceSet; }

	//Set Property
	virtual void SetRWallHorInfoSet(BOOL value) { m_bRWallHorInfoSet = value; }
	virtual void SetRWallHorBarStartSet(BOOL value) { m_bRWallHorBarStartSet = value; }
	virtual void SetRWallHorBarStartLength(long value) { m_nRWallHorBarStartLength = value; }
	virtual void SetRWallEndUbar(long value) { m_nRWallEndUbar = value; }
	virtual void SetRWallDefaultUbarLen(BOOL value) { m_bRWallDefaultUbarLen = value; }

	virtual void SetRWallSpacerInfoSet(BOOL value) { m_bRWallSpacerInfoSet = value; }
	virtual void SetRWallSpacerDiaType(long value) { m_nRWallSpacerDiaType = value; }
	virtual void SetRWallSpacerDiaIndex(long value) { m_nRWallSpacerDiaIndex = value; }
	virtual void SetRWallSpacerHorSpacing(long value) { m_nRWallSpacerHorSpacing = value; }
	virtual void SetRWallSpacerVerSpacingType(long value) { m_nRWallSpacerVerSpacingType = value; }
	virtual void SetRWallSpacerVerSpacing(long value) { m_nRWallSpacerVerSpacing = value; }
	virtual void SetRWallSpacerVerSpacingRate(long value) { m_nRWallSpacerVerSpacingRate = value; }
	virtual void SetRWallSpacerVerSpacingCount(long value) { m_nRWallSpacerVerSpacingCount = value; }
	virtual void SetRWallDowelSpacerVerSpacingCount(long value) { m_nRWallDowelSpacerVerSpacingCount = value; }
	virtual void SetRWallSpacerLenType(long value) { m_nRWallSpacerLenType = value; }
	virtual void SetRWallSpacerLenCalWay(long value) { m_nRWallSpacerLenCalWay = value; }
	virtual void SetRWallSpacerLen1(long value) { m_nRWallSpacerLen1 = value; }
	virtual void SetRWallSpacerLen2(long value) { m_nRWallSpacerLen2 = value; }
	virtual void SetRWallSpacerShape(long value) { m_nRWallSpacerShape = value; }
	virtual void SetRWallSpacerRebarPlaceSet(BOOL value) { m_bRWallSpacerRebarPlaceSet = value; }
	virtual void SetRWallSpacerRebarPlaceCondition(long value) { m_nRWallSpacerRebarPlaceCondition = value; }
	
	virtual void SetRWallVerInfoSet(BOOL value) { m_bRWallVerInfoSet = value; }
	virtual void SetRWallVerBarTopLenType(long value) { m_nRWallVerBarTopLenType = value; }
	virtual void SetRWallVerBarSpliceWay(long value) { m_nRWallVerBarSpliceWay = value; }
	virtual void SetRWallVerBarAlterSpliceSet(BOOL value) { m_bRWallVerBarAlterSpliceSet = value; }
	virtual void SetRWallVerBarAlterSpliceValue1(long value) { m_nRWallVerBarAlterSpliceValue1 = value; }
	virtual void SetRWallVerBarAlterSpliceValue2(long value) { m_nRWallVerBarAlterSpliceValue2 = value; }

	virtual void SetRWallDowelInfoSet(BOOL value) { m_bRWallDowelInfoSet = value; }
	virtual void SetRWallUserDowelHookLen(BOOL value) { m_bRWallUserDowelHookLen = value; }
	virtual void SetRWallDowelBarAnchorType(long value) { m_nRWallDowelBarAnchorType = value; }
	virtual void SetRWallDowelBarTopLenType(long value) { m_nRWallDowelBarTopLenType = value; }
	virtual void SetRWallDowelSupportBarDiaIndex(long value) { m_nRWallDowelSupportBarDiaIndex = value; }
	virtual void SetRWallDowelSupportBarNum(long value) { m_nRWallDowelSupportBarNum = value; }
	virtual void SetRWallDowelSupportPlaceType(BOOL value) { m_bRWallDowelSupportPlaceType = value; }
	virtual void SetRWallDowelBarSpliceWay(long value) { m_nRWallDowelBarSpliceWay = value; }
	virtual void SetRWallDowelBarAlterSpliceSet(BOOL value) { m_bRWallDowelBarAlterSpliceSet = value; }
	virtual void SetRWallDowelBarAlterSpliceValue1(long value) { m_nRWallDowelBarAlterSpliceValue1 = value; }
	virtual void SetRWallDowelBarAlterSpliceValue2(long value) { m_nRWallDowelBarAlterSpliceValue2 = value; }
	virtual void SetRWallDowelHookAnchorPlaceSpacingSet(BOOL value) { m_bRWallDowelHookAnchorPlaceSpacingSet = value; }
	virtual void SetRWallDowelHookAnchorPlaceSpacing(long value) { m_nRWallDowelHookAnchorPlaceSpacing = value; }
	virtual void SetRWallDowelBotAddBarIntegratedPlaceSet(BOOL value) { m_bRWallDowelBotAddBarIntegratedPlaceSet = value; }

	//Function
	virtual map<long, long> GetRWallDefaultUbarLenMap() { return mm_RWallDefaultUbarLen; }
	virtual map<long, long> GetRWallUserDowelHookLenMap() { return mm_RWallUserDowelHookLen; }
	virtual map<long, long> GetRWallUserDowelSpliceLenMap() { return mm_RWallUserDowelSpliceLen; }

	virtual void SetRWallDefaultUbarLenMap(map<long, long> mapDefaultUbarLen);
	virtual void SetRWallUserDowelHookLenMap(map<long, long> mapUserDowelHookLen);
	virtual void SetRWallUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen);

	virtual double GetRWallUserDowelHookLen(double dDia);
	virtual double GetRWallUserDowelSpliceLen(double dDia);

	virtual void SetRWallDefaultUbarLen(long key, long value) { mm_RWallDefaultUbarLen[key] = value; }
	virtual void SetRWallUserDowelHookLen(long key, long value) { mm_RWallUserDowelHookLen[key] = value; }
	virtual void SetRWallUserDowelSpliceLen(long key, long value) { mm_RWallUserDowelSpliceLen[key] = value; }
	
private:
	BOOL m_bRWallHorInfoSet;
	BOOL m_bRWallHorBarStartSet;    //시작간격 설정
	long m_nRWallHorBarStartLength; //시작간격
	long m_nRWallEndUbar;			//단부 배근형상 0:후크 1:U-BAR
	BOOL m_bRWallDefaultUbarLen;	//마구리U-BAR 발길이 설정
	map<long, long> mm_RWallDefaultUbarLen;		//DiaIndex별 마구리U-BAR 발길이 설정 값

	BOOL m_bRWallSpacerInfoSet;
	long m_nRWallSpacerDiaType;				//직경설정	0:수평근 직경에 따름 1:직경
	long m_nRWallSpacerDiaIndex;			//직경Index
	long m_nRWallSpacerHorSpacing;			//수평간격
	long m_nRWallSpacerVerSpacingType;		//폭 고정근 수직간격 타입	0:간격 1:비율 2:개수
	long m_nRWallSpacerVerSpacing;			//수직간격
	long m_nRWallSpacerVerSpacingRate;		//수직비율
	long m_nRWallSpacerVerSpacingCount;		//수직갯수
	long m_nRWallDowelSpacerVerSpacingCount;		//다월 폭 고정근 수직갯수
	long m_nRWallSpacerLenType;				//발길이 타입	0:인장이음 1:후크
	long m_nRWallSpacerLenCalWay;			//발길이 계산 방법	0:표준 1:사용자 정의
	long m_nRWallSpacerLen1;				//사용자 정의 길이1
	long m_nRWallSpacerLen2;				//사용자 정의 길이2
	long m_nRWallSpacerShape;				//형상
	BOOL m_bRWallSpacerRebarPlaceSet;		//배근 조건 설정
	long m_nRWallSpacerRebarPlaceCondition;	//배근 조건

	BOOL m_bRWallVerInfoSet;
	long m_nRWallVerBarTopLenType;	//이음타입 0:인장이음 1:압축이음 2:인장+압축
	long m_nRWallVerBarSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	BOOL m_bRWallVerBarAlterSpliceSet;		//엇이음 설정
	long m_nRWallVerBarAlterSpliceValue1;	//엇이음 설정 H1 Value
	long m_nRWallVerBarAlterSpliceValue2;	//엇이음 설정 H2 Value

	BOOL m_bRWallDowelInfoSet;
	BOOL m_bRWallUserDowelHookLen;		//다월후크길이설정
	long m_nRWallDowelBarAnchorType;	//정착타입 0:정착 1:후크정착
	long m_nRWallDowelBarTopLenType;	//이음타입 0:인장이음 1:압축이음 2:인장+압축 3:사용자설정
	long m_nRWallDowelSupportBarDiaIndex;	//지지근 지름
	long m_nRWallDowelSupportBarNum;	//지지근 단수
	BOOL m_bRWallDowelSupportPlaceType;	//장대산출여부
	long m_nRWallDowelBarSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	BOOL m_bRWallDowelBarAlterSpliceSet;		//엇이음 설정
	long m_nRWallDowelBarAlterSpliceValue1;	//엇이음 설정 H1 Value
	long m_nRWallDowelBarAlterSpliceValue2;	//엇이음 설정 H2 Value
	BOOL m_bRWallDowelHookAnchorPlaceSpacingSet;	//후크정착 배치간격 설정
	long m_nRWallDowelHookAnchorPlaceSpacing;	//후크정착 배치간격
	BOOL m_bRWallDowelBotAddBarIntegratedPlaceSet;	//하부 보강근과 다월 일체 배근 설정
	map<long, long> mm_RWallUserDowelHookLen;	//DiaIndex별 다월후크길이설정 값
	map<long, long> mm_RWallUserDowelSpliceLen;	//DiaIndex별 이음타입 사용자설정 값
};

