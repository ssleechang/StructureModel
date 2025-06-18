#pragma once
#include "OldBaseRebarTemplate.h"
#include "..\BWallRebarTemplate.h"

class StructureModel_CLASS OldBWallRebarTemplate : public OldBaseRebarTemplate
{
public:
	OldBWallRebarTemplate();
	~OldBWallRebarTemplate(void);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	DECLARE_SERIAL(OldBWallRebarTemplate)
	void Serialize(CArchive& ar);
	virtual void CopyFromMe(OldBWallRebarTemplate* pBWallRebarTemplate);

	void DataInit();

	bool TopOptionDataCopy(OldBWallRebarTemplate* rebarTemplate);
	void DataCopy(OldBWallRebarTemplate* rebarTemplate);
	void OldDataCopy(BWallRebarTemplate* rebarTemplate);
	void InfoSetCheckBoxChecked();

	//Get Property
	virtual BOOL GetBWallVerInfoSet() { return m_bBWallVerInfoSet; }
	virtual BOOL GetBWallPlaceEndBarSet() { return m_bBWallPlaceEndBarSet; }
	virtual long GetBWallEndVerAddBarCount() { return m_nBWallEndVerAddBarCount; }
	virtual long GetBWallEndVerAddBarDist() { return m_nBWallEndVerAddBarDist; }
	virtual long GetBWallVerBarTopLenType() { return m_nBWallVerBarTopLenType; }
	virtual long GetBWallCrossBarEAType() { return m_nBWallCrossBarEAType; }
	virtual long GetBWallVerBarSpliceWay() { return m_nBWallVerBarSpliceWay; }
	virtual BOOL GetBWallVerBarAlterSpliceSet() { return m_bBWallVerBarAlterSpliceSet; }
	virtual long GetBWallVerBarAlterSpliceValue1() { return m_nBWallVerBarAlterSpliceValue1; }
	virtual long GetBWallVerBarAlterSpliceValue2() { return m_nBWallVerBarAlterSpliceValue2; }
	virtual long GetBWallVerNumBarCrossBarIncludeSet() { return m_nBWallVerNumBarCrossBarIncludeSet; }

	virtual BOOL GetBWallHorInfoSet() { return m_bBWallHorInfoSet; }
	virtual BOOL GetBWallPlaceUBar4ShortWall() { return m_bBWallPlaceUBar4ShortWall; }
	virtual long GetBWallMaxUbarLegLength() { return m_nBWallMaxUbarLegLength; }
	virtual BOOL GetBWallPlaceStirrup4ShortWall() { return m_bBWallPlaceStirrup4ShortWall; }
	virtual long GetBWallHBarMaxStirrupLen() { return m_nBWallHBarMaxStirrupLen; }
	virtual BOOL GetBWallHorBarStartSet() { return m_bBWallHorBarStartSet; }
	virtual long GetBWallHorBarStartLength() { return m_nBWallHorBarStartLength; }
	virtual BOOL GetBWallStraightEndUBarPlaceSet() { return m_bBWallStraightEndUBarPlaceSet; }
	virtual BOOL GetBWallDefaultUbarLen() { return m_bBWallDefaultUbarLen; }
	virtual BOOL GetBWallCrossBarUBarHalfPlace() { return m_bBWallCrossBarUBarHalfPlace; }

	virtual BOOL GetBWallSpacerInfoSet() { return m_bBWallSpacerInfoSet; }
	virtual long GetBWallSpacerDiaType() { return m_nBWallSpacerDiaType; }
	virtual long GetBWallSpacerDiaIndex() { return m_nBWallSpacerDiaIndex; }
	virtual long GetBWallSpacerHorSpacing() { return m_nBWallSpacerHorSpacing; }
	virtual long GetBWallSpacerVerSpacingType() { return m_nBWallSpacerVerSpacingType; }
	virtual long GetBWallSpacerVerSpacing() { return m_nBWallSpacerVerSpacing; }
	virtual long GetBWallSpacerVerSpacingRate() { return m_nBWallSpacerVerSpacingRate; }
	virtual long GetBWallSpacerVerSpacingCount() { return m_nBWallSpacerVerSpacingCount; }
	virtual long GetBWallDowelSpacerVerSpacingCount() { return m_nBWallDowelSpacerVerSpacingCount; }
	virtual long GetBWallSpacerLenType() { return m_nBWallSpacerLenType; }
	virtual long GetBWallSpacerLenCalWay() { return m_nBWallSpacerLenCalWay; }
	virtual long GetBWallSpacerLen1() { return m_nBWallSpacerLen1; }
	virtual long GetBWallSpacerLen2() { return m_nBWallSpacerLen2; }
	virtual long GetBWallSpacerShape() { return m_nBWallSpacerShape; }
	virtual BOOL GetBWallSpacerRebarPlaceSet() { return m_bBWallSpacerRebarPlaceSet; }
	virtual long GetBWallSpacerRebarPlaceCondition() { return m_nBWallSpacerRebarPlaceCondition; }

	virtual BOOL GetBWallDowelInfoSet() { return m_bBWallDowelInfoSet; }
	virtual BOOL GetBWallUserDowelHookLen() { return m_bBWallUserDowelHookLen; }
	virtual long GetBWallDowelBarAnchorType() { return m_nBWallDowelBarAnchorType; }
	virtual long GetBWallDowelBarTopLenType() { return m_nBWallDowelBarTopLenType; }
	virtual long GetBWallDowelSupportBarDiaIndex() { return m_nBWallDowelSupportBarDiaIndex; }
	virtual long GetBWallDowelSupportBarNum() { return m_nBWallDowelSupportBarNum; }
	virtual BOOL GetBWallDowelSupportPlaceType() { return m_bBWallDowelSupportPlaceType; }
	virtual long GetBWallDowelBarSpliceWay() { return m_nBWallDowelBarSpliceWay; }
	virtual BOOL GetBWallDowelBarAlterSpliceSet() { return m_bBWallDowelBarAlterSpliceSet; }
	virtual long GetBWallDowelBarAlterSpliceValue1() { return m_nBWallDowelBarAlterSpliceValue1; }
	virtual long GetBWallDowelBarAlterSpliceValue2() { return m_nBWallDowelBarAlterSpliceValue2; }
	virtual BOOL GetBWallDowelHookAnchorPlaceSpacingSet() { return m_bBWallDowelHookAnchorPlaceSpacingSet; }
	virtual long GetBWallDowelHookAnchorPlaceSpacing() { return m_nBWallDowelHookAnchorPlaceSpacing; }

	//Set Property
	virtual void SetBWallVerInfoSet(BOOL value) { m_bBWallVerInfoSet = value; }
	virtual void SetBWallPlaceEndBarSet(BOOL value) { m_bBWallPlaceEndBarSet = value; }
	virtual void SetBWallEndVerAddBarCount(long value) { m_nBWallEndVerAddBarCount = value; }
	virtual void SetBWallEndVerAddBarDist(long value) { m_nBWallEndVerAddBarDist = value; }
	virtual void SetBWallVerBarTopLenType(long value) { m_nBWallVerBarTopLenType = value; }
	virtual void SetBWallCrossBarEAType(long value) { m_nBWallCrossBarEAType = value; }
	virtual void SetBWallVerBarSpliceWay(long value) { m_nBWallVerBarSpliceWay = value; }
	virtual void SetBWallVerBarAlterSpliceSet(BOOL value) { m_bBWallVerBarAlterSpliceSet = value; }
	virtual void SetBWallVerBarAlterSpliceValue1(long value) { m_nBWallVerBarAlterSpliceValue1 = value; }
	virtual void SetBWallVerBarAlterSpliceValue2(long value) { m_nBWallVerBarAlterSpliceValue2 = value; }
	virtual void SetBWallVerNumBarCrossBarIncludeSet(long value) { m_nBWallVerNumBarCrossBarIncludeSet = value; }

	virtual void SetBWallHorInfoSet(BOOL value) { m_bBWallHorInfoSet = value; }
	virtual void SetBWallPlaceUBar4ShortWall(BOOL value) { m_bBWallPlaceUBar4ShortWall = value; }
	virtual void SetBWallMaxUbarLegLength(long value) { m_nBWallMaxUbarLegLength = value; }
	virtual void SetBWallPlaceStirrup4ShortWall(BOOL value) { m_bBWallPlaceStirrup4ShortWall = value; }
	virtual void SetBWallHBarMaxStirrupLen(long value) { m_nBWallHBarMaxStirrupLen = value; }
	virtual void SetBWallHorBarStartSet(BOOL value) { m_bBWallHorBarStartSet = value; }
	virtual void SetBWallHorBarStartLength(long value) { m_nBWallHorBarStartLength = value; }
	virtual void SetBWallStraightEndUBarPlaceSet(BOOL value) { m_bBWallStraightEndUBarPlaceSet = value; }
	virtual void SetBWallDefaultUbarLen(BOOL value) { m_bBWallDefaultUbarLen = value; }
	virtual void SetBWallCrossBarUBarHalfPlace(BOOL value) { m_bBWallCrossBarUBarHalfPlace = value; }

	virtual void SetBWallSpacerInfoSet(BOOL value) { m_bBWallSpacerInfoSet = value; }
	virtual void SetBWallSpacerDiaType(long value) { m_nBWallSpacerDiaType = value; }
	virtual void SetBWallSpacerDiaIndex(long value) { m_nBWallSpacerDiaIndex = value; }
	virtual void SetBWallSpacerHorSpacing(long value) { m_nBWallSpacerHorSpacing = value; }
	virtual void SetBWallSpacerVerSpacingType(long value) { m_nBWallSpacerVerSpacingType = value; }
	virtual void SetBWallSpacerVerSpacing(long value) { m_nBWallSpacerVerSpacing = value; }
	virtual void SetBWallSpacerVerSpacingRate(long value) { m_nBWallSpacerVerSpacingRate = value; }
	virtual void SetBWallSpacerVerSpacingCount(long value) { m_nBWallSpacerVerSpacingCount = value; }
	virtual void SetBWallDowelSpacerVerSpacingCount(long value) { m_nBWallDowelSpacerVerSpacingCount = value; }
	virtual void SetBWallSpacerLenType(long value) { m_nBWallSpacerLenType = value; }
	virtual void SetBWallSpacerLenCalWay(long value) { m_nBWallSpacerLenCalWay = value; }
	virtual void SetBWallSpacerLen1(long value) { m_nBWallSpacerLen1 = value; }
	virtual void SetBWallSpacerLen2(long value) { m_nBWallSpacerLen2 = value; }
	virtual void SetBWallSpacerShape(long value) { m_nBWallSpacerShape = value; }
	virtual void SetBWallSpacerRebarPlaceSet(BOOL value) { m_bBWallSpacerRebarPlaceSet = value; }
	virtual void SetBWallSpacerRebarPlaceCondition(long value) { m_nBWallSpacerRebarPlaceCondition = value; }

	virtual void SetBWallDowelInfoSet(BOOL value) { m_bBWallDowelInfoSet = value; }
	virtual void SetBWallUserDowelHookLen(BOOL value) { m_bBWallUserDowelHookLen = value; }
	virtual void SetBWallDowelBarAnchorType(long value) { m_nBWallDowelBarAnchorType = value; }
	virtual void SetBWallDowelBarTopLenType(long value) { m_nBWallDowelBarTopLenType = value; }
	virtual void SetBWallDowelSupportBarDiaIndex(long value) { m_nBWallDowelSupportBarDiaIndex = value; }
	virtual void SetBWallDowelSupportBarNum(long value) { m_nBWallDowelSupportBarNum = value; }
	virtual void SetBWallDowelSupportPlaceType(BOOL value) { m_bBWallDowelSupportPlaceType = value; }
	virtual void SetBWallDowelBarSpliceWay(long value) { m_nBWallDowelBarSpliceWay = value; }
	virtual void SetBWallDowelBarAlterSpliceSet(BOOL value) { m_bBWallDowelBarAlterSpliceSet = value; }
	virtual void SetBWallDowelBarAlterSpliceValue1(long value) { m_nBWallDowelBarAlterSpliceValue1 = value; }
	virtual void SetBWallDowelBarAlterSpliceValue2(long value) { m_nBWallDowelBarAlterSpliceValue2 = value; }
	virtual void SetBWallDowelHookAnchorPlaceSpacingSet(BOOL value) { m_bBWallDowelHookAnchorPlaceSpacingSet = value; }
	virtual void SetBWallDowelHookAnchorPlaceSpacing(long value) { m_nBWallDowelHookAnchorPlaceSpacing = value; }

	//Function
	virtual map<long, long> GetBWallAddBarDiaIndexMap() { return mm_BWallAddBarDiaIndex; }
	virtual map<long, long> GetBWallEndBarDiaIndexMap() { return mm_BWallEndBarDiaIndex; }
	virtual map<long, long> GetBWallDefaultUbarLenMap() { return mm_BWallDefaultUbarLen; }
	virtual map<long, long> GetBWallUserDowelHookLenMap() { return mm_BWallUserDowelHookLen; }
	virtual map<long, long> GetBWallUserDowelSpliceLenMap() { return mm_BWallUserDowelSpliceLen; }

	virtual void SetBWallAddBarDiaIndexMap(map<long, long> mapAddBarDiaIndex);
	virtual void SetBWallEndBarDiaIndexMap(map<long, long> mapEndBarDiaIndex);
	virtual void SetBWallDefaultUbarLenMap(map<long, long> mapDefaultUbarLen);
	virtual void SetBWallUserDowelHookLenMap(map<long, long> mapUserDowelHookLen);
	virtual void SetBWallUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen);

	virtual double GetBWallAddBarDia(double dDia);
	virtual double GetBWallEndBarDia(double dDia);
	virtual double GetBWallDefaultUbarLen(double dDia);
	virtual double GetBWallUserDowelHookLen(double dDia);
	virtual double GetBWallUserDowelSpliceLen(double dDia);

	virtual void SetBWallAddBarDiaIndex(long key, long value) { mm_BWallAddBarDiaIndex[key] = value; }
	virtual void SetBWallEndBarDiaIndex(long key, long value) { mm_BWallEndBarDiaIndex[key] = value; }
	virtual void SetBWallDefaultUbarLen(long key, long value) { mm_BWallDefaultUbarLen[key] = value; }
	virtual void SetBWallUserDowelHookLen(long key, long value) { mm_BWallUserDowelHookLen[key] = value; }
	virtual void SetBWallUserDowelSpliceLen(long key, long value) { mm_BWallUserDowelSpliceLen[key] = value; }

private:
	BOOL m_bBWallVerInfoSet;
	BOOL m_bBWallPlaceEndBarSet;	//단부보강근
	long m_nBWallEndVerAddBarCount;	//단부보강근 개수
	long m_nBWallEndVerAddBarDist;	//단부보강근 간격
	long m_nBWallVerBarTopLenType;  //이음 타입
	long m_nBWallCrossBarEAType;	//교차보강근 개수
	long m_nBWallVerBarSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	BOOL m_bBWallVerBarAlterSpliceSet;		//엇이음 설정
	long m_nBWallVerBarAlterSpliceValue1;	//엇이음 설정 H1 Value
	long m_nBWallVerBarAlterSpliceValue2;	//엇이음 설정 H2 Value
	long m_nBWallVerNumBarCrossBarIncludeSet;	//수직근 개수 산정 시 교차보강근 0:포함 1:미포함
	map<long, long> mm_BWallAddBarDiaIndex;		//DiaIndex별 교차보강근 직경설정 값
	map<long, long> mm_BWallEndBarDiaIndex;		//DiaIndex별 단부보강근 직경설정 값부

	BOOL m_bBWallHorInfoSet;
	BOOL m_bBWallPlaceUBar4ShortWall;		//짧은벽체 U-BAR 배근
	long m_nBWallMaxUbarLegLength;			//최대 벽체 길이
	BOOL m_bBWallPlaceStirrup4ShortWall;	//짧은벽체 스트럽 배근
	long m_nBWallHBarMaxStirrupLen;			//최대 벽체 길이
	BOOL m_bBWallHorBarStartSet;			//시작간격 설정
	long m_nBWallHorBarStartLength;			//시작간격
	BOOL m_bBWallStraightEndUBarPlaceSet;	//일자형벽체 단부 U_BAR 배근
	BOOL m_bBWallDefaultUbarLen;			//단부 U-BAR 발길이 설정
	BOOL m_bBWallCrossBarUBarHalfPlace;		//교차 벽체 마구리 유바 50% 체결
	map<long, long> mm_BWallDefaultUbarLen;		//DiaIndex별 단부 U-BAR 발길이 설정 설정 값

	BOOL m_bBWallSpacerInfoSet;
	long m_nBWallSpacerDiaType;				//직경설정	0:수평근 직경에 따름 1:직경
	long m_nBWallSpacerDiaIndex;			//직경Index
	long m_nBWallSpacerHorSpacing;			//수평간격
	long m_nBWallSpacerVerSpacingType;		//폭 고정근 수직간격 타입	0:간격 1:비율 2:개수
	long m_nBWallSpacerVerSpacing;			//수직간격
	long m_nBWallSpacerVerSpacingRate;		//수직비율
	long m_nBWallSpacerVerSpacingCount;		//수직갯수
	long m_nBWallDowelSpacerVerSpacingCount;		//다월 폭 고정근 수직갯수
	long m_nBWallSpacerLenType;				//발길이 타입	0:인장이음 1:후크
	long m_nBWallSpacerLenCalWay;			//발길이 계산 방법	0:표준 1:사용자 정의
	long m_nBWallSpacerLen1;				//사용자 정의 길이1
	long m_nBWallSpacerLen2;				//사용자 정의 길이2
	long m_nBWallSpacerShape;				//형상
	BOOL m_bBWallSpacerRebarPlaceSet;		//배근 조건 설정
	long m_nBWallSpacerRebarPlaceCondition;	//배근 조건
	
	BOOL m_bBWallDowelInfoSet;
	BOOL m_bBWallUserDowelHookLen;		//다월후크길이설정
	long m_nBWallDowelBarAnchorType;	//정착 타입
	long m_nBWallDowelBarTopLenType;	//이음 타입
	long m_nBWallDowelSupportBarDiaIndex;	//지지근 지름
	long m_nBWallDowelSupportBarNum;	//지지근 단수
	BOOL m_bBWallDowelSupportPlaceType;	//지지근 장대산출
	long m_nBWallDowelBarSpliceWay;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	BOOL m_bBWallDowelBarAlterSpliceSet;		//엇이음 설정
	long m_nBWallDowelBarAlterSpliceValue1;	//엇이음 설정 H1 Value
	long m_nBWallDowelBarAlterSpliceValue2;	//엇이음 설정 H2 Value
	BOOL m_bBWallDowelHookAnchorPlaceSpacingSet;	//후크정착 배치간격 설정
	long m_nBWallDowelHookAnchorPlaceSpacing;	//후크정착 배치간격
	map<long, long> mm_BWallUserDowelHookLen;	//DiaIndex별 다월후크길이설정 값
	map<long, long> mm_BWallUserDowelSpliceLen;	//DiaIndex별 이음타입 사용자설정 값
};

