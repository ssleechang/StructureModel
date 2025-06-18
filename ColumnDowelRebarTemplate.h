#pragma once
class StructureModel_CLASS ColumnDowelRebarTemplate
{
public:
	ColumnDowelRebarTemplate();
	~ColumnDowelRebarTemplate();

	void DataInit();
	void CopyFromMe(ColumnDowelRebarTemplate* pColumnRebarTemplate);
	void Serialize(CArchive& ar);

	long GetBarSpliceType();
	void SetBarSpliceType(long value);
	long GetTopLenType();
	void SetTopLenType(long value);
	long GetConstructionHeight();
	void SetConstructionHeight(long value);
	BOOL GetAlterSpliceSet();
	void SetAlterSpliceSet(BOOL value);
	long GetAlterSpliceDist();
	void SetAlterSpliceDist(long value);
	long GetHoopRebarType();
	void SetHoopRebarType(long value);
	long GetHoopBySpace();
	void SetHoopBySpace(long value);
	long GetHoopByNum();
	void SetHoopByNum(long value);
	long GetSupportBarDiaIndex();
	void SetSupportBarDiaIndex(long value);
	long GetSupportBarLengthType();
	void SetSupportBarLengthType(long value);
	long GetSupportBarLength();
	void SetSupportBarLength(long value);
	double GetSupportBarFootingThickMultiplicationValue();
	void SetSupportBarFootingThickMultiplicationValue(double value);
	long GetSupportBarCorrectiveLength();
	void SetSupportBarCorrectiveLength(long value);
	long GetSupportBarNum();
	void SetSupportBarNum(long value);
	long GetBotPlaceType();
	void SetBotPlaceType(long value);
	BOOL GetFootingPercentSet();
	void SetFootingPercentSet(BOOL value);
	long GetFootingPercent();
	void SetFootingPercent(long value);
	BOOL GetFootingCoverSet();
	void SetFootingCoverSet(BOOL value);
	long GetFootingCover();
	void SetFootingCover(long value);
	BOOL GetUserHookLenSet();
	void SetUserHookLenSet(BOOL value);
	BOOL GetSupportBarUserSteelGradeS();
	void SetSupportBarUserSteelGradeS(BOOL value);
	long GetAlterSpliceType();
	void SetAlterSpliceType(long value);
private:
	long m_nBarSpliceType;			//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nTopLenType;			//겹침이음 길이 0:인장이음 1:압축이음 2:사용자설정
	long m_nConstructionHeight;		//시공높이
	BOOL m_bAlterSpliceSet;			//엇이음 설정
	long m_nAlterSpliceType;
	long m_nAlterSpliceDist;			//엇갈림 거리
	long m_nHoopRebarType;			//다월 대근 개수 산정
	long m_nHoopBySpace;				//다월대근 간격
	long m_nHoopByNum;				//다월대근 개수
	long m_nSupportBarDiaIndex;		//지지근 직경
	BOOL m_bSupportBarUserSteelGradeS;
	long m_nSupportBarLengthType;	//지지근 길이타입
	long m_nSupportBarLength;		//지지근 길이
	double m_dSupportBarFootingThickMultiplicationValue;
	long m_nSupportBarCorrectiveLength;		//보정길이
	long m_nSupportBarNum;			//지지근 개수
	long m_nBotPlaceType;			//정착 방식 0:일자 정착 1:후크 2:코너 부분에만 후크 적용
	BOOL m_bFootingPercentSet;		//다월 적용비율 설정
	long m_nFootingPercent;			//다월 적용비율
	BOOL m_bFootingCoverSet;			//다월 하부 피복두께 설정
	long m_nFootingCover;			//다월 하부 피복두께
	BOOL m_bUserHookLenSet;		//다월후크길이설정
};

