#pragma once
class StructureModel_CLASS RWallVerBarRebarTemplate
{
public:
	RWallVerBarRebarTemplate();
	~RWallVerBarRebarTemplate();

	void DataInit();
	void CopyFromMe(RWallVerBarRebarTemplate* pColumnRebarTemplate);
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
	long m_nBarSpliceType;
	long m_nTopLenType;
	long m_nTopFloorDevType;
	long m_nConstructionHeight;
	BOOL m_bAlterSpliceSet;
	long m_nAlterSpliceType;
	long m_nAlterSpliceDist;
};

