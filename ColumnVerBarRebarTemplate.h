#pragma once
class StructureModel_CLASS ColumnVerBarRebarTemplate
{
public:
	ColumnVerBarRebarTemplate();
	~ColumnVerBarRebarTemplate();

	void DataInit();
	void CopyFromMe(ColumnVerBarRebarTemplate* pColumnRebarTemplate);
	void Serialize(CArchive& ar);

	long GetBarSpliceType();
	void SetBarSpliceType(long value);
	long GetTopLenType();
	void SetTopLenType(long value);
	long GetTopFloorDevType();
	void SetTopFloorDevType(long value);
	long GetConstructionHeight();
	void SetConstructionHeight(long value);
	BOOL GetAlterSpliceSet();
	void SetAlterSpliceSet(BOOL value);
	long GetAlterSpliceDist();
	void SetAlterSpliceDist(long value);
	long GetAlterSpliceType();
	void SetAlterSpliceType(long value);
private:
	long m_nBarSpliceType;			//이음방식 0:겹침이음 1:커플러이음 2:압접이음
	long m_nTopLenType;			//겹침이음 길이 0:인장이음 1:압축이음
	long m_nTopFloorDevType;	//최상층 정착 타입 0:정착 1:피복일자 2:피복후크
	long m_nConstructionHeight;	//시공높이
	BOOL m_bAlterSpliceSet;		//엇이음 설정
	long m_nAlterSpliceType; // 0 이음길이 1 사용자입력
	long m_nAlterSpliceDist;	//엇갈림 거리
};

