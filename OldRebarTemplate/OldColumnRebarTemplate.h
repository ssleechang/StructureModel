#pragma once
#include "OldBaseRebarTemplate.h"
#include "..\RebarTemplate.h"

class StructureModel_CLASS OldColumnRebarTemplate : public OldBaseRebarTemplate
{
public:
	OldColumnRebarTemplate();
	~OldColumnRebarTemplate(void);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	virtual void CopyFromMe(OldColumnRebarTemplate* pColumnRebarTemplate);

	DECLARE_SERIAL(OldColumnRebarTemplate)
	void Serialize(CArchive& ar);

	void DataInit();

	bool TopOptionDataCopy(OldColumnRebarTemplate* rebarTemplate);
	void DataCopy(OldColumnRebarTemplate* rebarTemplate);
	void OldDataCopy(ColumnRebarTemplate* rebarTemplate);
	void InfoSetCheckBoxChecked();

	//Get Property
	virtual BOOL GetColumnGeneralInfoSet() { return m_bColumnGeneralInfoSet; }
	virtual BOOL GetColumnConnectLengthSet() { return m_bColumnConnectLengthSet; }
	virtual long GetColumnConnectLength() { return m_nColumnConnectLength; }

	virtual BOOL GetColumnHoopBarInfoSet() { return m_bColumnHoopBarInfoSet; }
	virtual BOOL GetColumnHoopBarStartLengthSet() { return m_bColumnHoopBarStartLengthSet; }
	virtual long GetColumnHoopBarStartLength() { return m_nColumnHoopBarStartLength; }
	virtual BOOL GetColumnTieAddLengthSet() { return m_bColumnTieAddLengthSet; }
	virtual long GetColumnTieAddLength() { return m_nColumnTieAddLength; }
	virtual long GetColumnTopMemberCover() { return m_nColumnTopMemberCover; }
	virtual long GetColumnHoopBarShapeIdx() { return m_nColumnHoopBarShapeIdx; }
	virtual long GetColumnTieBarShapeIdx() { return m_nColumnTieBarShapeIdx; }
	virtual BOOL GetColumnEndHoopBarShapeSet() { return m_bColumnEndHoopBarShapeSet; }

	virtual BOOL GetColumnVerInfoSet() { return m_bColumnVerInfoSet; }
	virtual long GetColumnVerBarSpliceType() { return m_nColumnVerBarSpliceType; }
	virtual long GetColumnVerBarTopLenType() { return m_nColumnVerBarTopLenType; }
	virtual BOOL GetColumnVerBarAlterSpliceSet() { return m_bColumnVerBarAlterSpliceSet; }
	virtual long GetColumnVerBarAlterSpliceValue1() { return m_nColumnVerBarAlterSpliceValue1; }
	virtual long GetColumnVerBarAlterSpliceValue2() { return m_nColumnVerBarAlterSpliceValue2; }

	virtual BOOL GetColumnDowelInfoSet() { return m_bColumnDowelInfoSet; }
	virtual BOOL GetColumnDowelFootingCoverSet() { return m_bColumnDowelFootingCoverSet; }
	virtual long GetColumnDowelFootingCover() { return m_nColumnDowelFootingCover; }
	virtual long GetColumnDowelBotPlaceType() { return m_nColumnDowelBotPlaceType; }
	virtual long GetColumnDowelTopPlaceType() { return m_nColumnDowelTopPlaceType; }
	virtual long GetColumnDowelHoopRebarType() { return m_nColumnDowelHoopRebarType; }
	virtual long GetColumnDowelHoopBySpace() { return m_nColumnDowelHoopBySpace; }
	virtual long GetColumnDowelHoopByNum() { return m_nColumnDowelHoopByNum; }
	virtual long GetColumnDowelSupportBarDiaIndex() { return m_nColumnDowelSupportBarDiaIndex; }
	virtual long GetColumnDowelSupportBarLength() { return m_nColumnDowelSupportBarLength; }
	virtual long GetColumnDowelSupportBarNum() { return m_nColumnDowelSupportBarNum; }
	virtual long GetColumnDowelBarSpliceType() { return m_nColumnDowelBarSpliceType; }
	virtual BOOL GetColumnDowelBarAlterSpliceSet() { return m_bColumnDowelBarAlterSpliceSet; }
	virtual long GetColumnDowelBarAlterSpliceValue1() { return m_nColumnDowelBarAlterSpliceValue1; }
	virtual long GetColumnDowelBarAlterSpliceValue2() { return m_nColumnDowelBarAlterSpliceValue2; }
	virtual BOOL GetColumnDowelFootingPercentSet() { return m_bColumnDowelFootingPercentSet; }
	virtual long GetColumnDowelFootingPercent() { return m_nColumnDowelFootingPercent; }

	//Set Property
	virtual void SetColumnGeneralInfoSet(BOOL value) { m_bColumnGeneralInfoSet = value; }
	virtual void SetColumnConnectLengthSet(BOOL value) { m_bColumnConnectLengthSet = value; }
	virtual void SetColumnConnectLength(long value) { m_nColumnConnectLength = value; }

	virtual void SetColumnHoopBarInfoSet(BOOL value) { m_bColumnHoopBarInfoSet = value; }
	virtual void SetColumnHoopBarStartLengthSet(BOOL value) { m_bColumnHoopBarStartLengthSet = value; }
	virtual void SetColumnHoopBarStartLength(long value) { m_nColumnHoopBarStartLength = value; }
	virtual void SetColumnTieAddLengthSet(BOOL value) { m_bColumnTieAddLengthSet = value; }
	virtual void SetColumnTieAddLength(long value) { m_nColumnTieAddLength = value; }
	virtual void SetColumnTopMemberCover(long value) { m_nColumnTopMemberCover = value; }
	virtual void SetColumnHoopBarShapeIdx(long value) { m_nColumnHoopBarShapeIdx = value; }
	virtual void SetColumnTieBarShapeIdx(long value) { m_nColumnTieBarShapeIdx = value; }
	virtual void SetColumnEndHoopBarShapeSet(BOOL value) { m_bColumnEndHoopBarShapeSet = value; }

	virtual void SetColumnVerInfoSet(BOOL value) { m_bColumnVerInfoSet = value; }
	virtual void SetColumnVerBarSpliceType(long value) { m_nColumnVerBarSpliceType = value; }
	virtual void SetColumnVerBarTopLenType(long value) { m_nColumnVerBarTopLenType = value; }
	virtual void SetColumnVerBarAlterSpliceSet(BOOL value) { m_bColumnVerBarAlterSpliceSet = value; }
	virtual void SetColumnVerBarAlterSpliceValue1(long value) { m_nColumnVerBarAlterSpliceValue1 = value; }
	virtual void SetColumnVerBarAlterSpliceValue2(long value) { m_nColumnVerBarAlterSpliceValue2 = value; }

	virtual void SetColumnDowelInfoSet(BOOL value) { m_bColumnDowelInfoSet = value; }
	virtual void SetColumnDowelFootingCoverSet(BOOL value) { m_bColumnDowelFootingCoverSet = value; }
	virtual void SetColumnDowelFootingCover(long value) { m_nColumnDowelFootingCover = value; }
	virtual void SetColumnDowelBotPlaceType(long value) { m_nColumnDowelBotPlaceType = value; }
	virtual void SetColumnDowelTopPlaceType(long value) { m_nColumnDowelTopPlaceType = value; }
	virtual void SetColumnDowelHoopRebarType(long value) { m_nColumnDowelHoopRebarType = value; }
	virtual void SetColumnDowelHoopBySpace(long value) { m_nColumnDowelHoopBySpace = value; }
	virtual void SetColumnDowelHoopByNum(long value) { m_nColumnDowelHoopByNum = value; }
	virtual void SetColumnDowelSupportBarDiaIndex(long value) { m_nColumnDowelSupportBarDiaIndex = value; }
	virtual void SetColumnDowelSupportBarLength(long value) { m_nColumnDowelSupportBarLength = value; }
	virtual void SetColumnDowelSupportBarNum(long value) { m_nColumnDowelSupportBarNum = value; }
	virtual void SetColumnDowelBarSpliceType(long value) { m_nColumnDowelBarSpliceType = value; }
	virtual void SetColumnDowelBarAlterSpliceSet(BOOL value) { m_bColumnDowelBarAlterSpliceSet = value; }
	virtual void SetColumnDowelBarAlterSpliceValue1(long value) { m_nColumnDowelBarAlterSpliceValue1 = value; }
	virtual void SetColumnDowelBarAlterSpliceValue2(long value) { m_nColumnDowelBarAlterSpliceValue2 = value; }
	virtual void SetColumnDowelFootingPercentSet(BOOL value) { m_bColumnDowelFootingPercentSet = value; }
	virtual void SetColumnDowelFootingPercent(long value) { m_nColumnDowelFootingPercent = value; }

	//Function
	virtual map<long, long> GetColumnUserDowelSpliceLenMap() { return mm_ColumnUserDowelSpliceLen; }
	virtual void SetColumnUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen);

	virtual long GetColumnUserDowelSpliceLen(double dDia);
	virtual void SetColumnUserDowelSpliceLen(long key, long value) { mm_ColumnUserDowelSpliceLen[key] = value; }

private:
	BOOL m_bColumnGeneralInfoSet;
	BOOL m_bColumnConnectLengthSet;	//단면치수 변경시 굽힘 치수 설정
	long m_nColumnConnectLength;	//단면치수 변경시 굽힘 치수

	BOOL m_bColumnHoopBarInfoSet;
	BOOL m_bColumnHoopBarStartLengthSet;	//대근 시작간격 설정
	long m_nColumnHoopBarStartLength;		//대근 시작간격
	BOOL m_bColumnTieAddLengthSet;	//보조대근 추가길이 설정
	long m_nColumnTieAddLength;		//보조대근 추가길이
	long m_nColumnTopMemberCover;	//상부부재 하부피복
	long m_nColumnHoopBarShapeIdx;		//대근형상
	long m_nColumnTieBarShapeIdx;		//보조대근 형상
	BOOL m_bColumnEndHoopBarShapeSet;	//단부 대근후크형상 135+135 적용

	BOOL m_bColumnVerInfoSet;
	long m_nColumnVerBarSpliceType;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nColumnVerBarTopLenType;	//이음타입 0:인장이음 1:압축이음
	BOOL m_bColumnVerBarAlterSpliceSet;		//엇이음 설정
	long m_nColumnVerBarAlterSpliceValue1;	//엇이음 설정 H1 Value
	long m_nColumnVerBarAlterSpliceValue2;	//엇이음 설정 H2 Value

	BOOL m_bColumnDowelInfoSet;
	BOOL m_bColumnDowelFootingCoverSet;	//하부기초 피복두께 설정
	long m_nColumnDowelFootingCover;		//하부기초 피복두께
	long m_nColumnDowelBotPlaceType;	//정착타입 0:정착 1:후크정착 2:코너만 후크
	long m_nColumnDowelTopPlaceType;   //이음타입 0:인장이음 1:압축이음 2:사용자설정
	long m_nColumnDowelHoopRebarType;  //다월대근 개수산정
	long m_nColumnDowelHoopBySpace;	//다월대근 간격
	long m_nColumnDowelHoopByNum;		//다월대근 개수
	long m_nColumnDowelSupportBarDiaIndex;    //지지근 직경
	long m_nColumnDowelSupportBarLength; //지지근 길이
	long m_nColumnDowelSupportBarNum;    //지지근 개수
	long m_nColumnDowelBarSpliceType;	//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	BOOL m_bColumnDowelBarAlterSpliceSet;		//엇이음 설정
	long m_nColumnDowelBarAlterSpliceValue1;	//엇이음 설정 H1 Value
	long m_nColumnDowelBarAlterSpliceValue2;	//엇이음 설정 H2 Value
	BOOL m_bColumnDowelFootingPercentSet;	//기초다월 적용수량 설정
	long m_nColumnDowelFootingPercent;	//기초다월 적용수량
	map<long, long> mm_ColumnUserDowelSpliceLen;	//DiaIndex별 이음타입 사용자설정 값
};

