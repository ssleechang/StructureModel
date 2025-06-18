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
	BOOL m_bSupportRebarsSet;					//���� ������ ����
	long m_nSupportRebarsDiaIndex;				//����
	long m_nSupportRebarsLengthType;			//���� Ÿ��(Lx, Ly)
	double m_dSupportRebarsTotalLengthRate;	//���� ����
	long m_nSupportRebarsOrderLength;			//�ֹ�����
	long m_nSupportRebarsSpliceLength;			//��������
	long m_nSupportRebarsCorrectiveLength;	//��������
	BOOL m_bUserSteelGradeS;
};

