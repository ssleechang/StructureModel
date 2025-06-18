#pragma once

namespace RebarOption
{
	enum CorLen_Type
	{
		MAT, COLUMN, WALL_VER, WALL_HOR, RWALL_VER, RWALL_HOR, BEAM, SLAB,//Corrective Length Types
	};
	enum CorLen_ROUND
	{
		UP, ROUND, OFF//Corrective Length Rounding Methods
	};
}

class StructureModel_CLASS CorrectiveLengthItem
{
public:
	CorrectiveLengthItem(void) {
		m_sText= _T("");
		m_bCorrective = false;
		m_bHalf50 = false;
		m_nSpliceAddLen = 0;
	};
	CorrectiveLengthItem(RebarOption::CorLen_Type type, bool IsCorrective, CString text, bool half50Check, int spliceAddLen);
	~CorrectiveLengthItem(void);
	void Serialize(CArchive& ar);

	int ParsingCorLenData(vector<long>& ValueArr) const;
	int GetUnitbyText();
	CString GetTextLine() const;
	long Get50UpLength(long nLength) const;
	long Get50UpDownLength(long nLength) const;
	long GetSpliceAddLen();

	RebarOption::CorLen_Type m_eType;
	int m_nUnit;
	RebarOption::CorLen_ROUND m_eRoundMethod;
	CString m_sText;
	bool m_bCorrective;
	bool m_bHalf50;
	int m_nSpliceAddLen;

	void Copy(CorrectiveLengthItem desCorrectiveLengthItem);
};

