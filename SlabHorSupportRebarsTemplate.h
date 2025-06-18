#pragma once
class StructureModel_CLASS SlabHorSupportRebarsTemplate
{
public:
	SlabHorSupportRebarsTemplate();
	~SlabHorSupportRebarsTemplate();

	void DataInit();
	void CopyFromMe(SlabHorSupportRebarsTemplate* pRebarTemplate);
	void Serialize(CArchive& ar);

	BOOL GetSupportRebarsSet();
	long GetSupportRebarsDiaIndex();
	long GetSupportRebarsLengthType();
	double GetSupportRebarsTotalLengthRate();
	long GetSupportRebarsOrderLength();
	long GetSupportRebarsSpliceLength();
	long GetSupportRebarsCorrectiveLength();
	BOOL GetUserSteelGradeS();

	void SetSupportRebarsSet(BOOL value);
	void SetSupportRebarsDiaIndex(long value);
	void SetSupportRebarsLengthType(long value);
	void SetSupportRebarsTotalLengthRate(double value);
	void SetSupportRebarsOrderLength(long value);
	void SetSupportRebarsSpliceLength(long value);
	void SetSupportRebarsCorrectiveLength(long value);
	void SetUserSteelGradeS(BOOL value);
private:
	BOOL m_bSupportRebarsSet;					//수평 도리근 설정
	long m_nSupportRebarsDiaIndex;				//직경
	long m_nSupportRebarsLengthType;			//길이 타입(Lx, Ly)
	double m_dSupportRebarsTotalLengthRate;	//길이 비율
	long m_nSupportRebarsOrderLength;			//주문길이
	long m_nSupportRebarsSpliceLength;			//이음길이
	long m_nSupportRebarsCorrectiveLength;	//보정길이
	BOOL m_bUserSteelGradeS;
};

