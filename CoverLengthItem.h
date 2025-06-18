#pragma once

namespace RebarOption
{
	enum eCoverLengthFace
	{
		CL_SIDE_FACE, CL_TOP_FACE, CL_INNER_FACE, CL_OUTTER_FACE, CL_END_FACE, CL_UP_FACE, CL_DOWN_FACE, CL_UPDOWN_FACE, CL_RWALL_INNER_FACE // CoverLengthFace
		//측면(사면), 최상층상부, 내측,        외측,        끝단,    윗면,     아랫면
	};
	enum eCoverLengthType
	{
		CL_COLUMN, CL_WALL, CL_BEAM, CL_SLAB, CL_FOUNDATION, CL_DECKSLAB//Cover Length Member Type
	};
}


class StructureModel_CLASS CoverLengthItem
{
public:
	CoverLengthItem(void){};
	CoverLengthItem(RebarOption::eCoverLengthType type, CString diaName, vector<int>& coverLengths);
	~CoverLengthItem(void);
	void Serialize(CArchive& ar);
	void PrepareCoverLengthMap();
	
	CString  m_sDiaName;
	RebarOption::eCoverLengthType m_eType;
	void SetCoverLengths( vector<int>& coverLengths);
	vector<int> GetCoverLengths();//복사본이 반환된다.

	int GetCoverLength(RebarOption::eCoverLengthFace face) const;

	vector<int> m_aCoverLengths;
	
	void Copy(CoverLengthItem desCoverLengthItem);
private:
	map<RebarOption::eCoverLengthFace,int> m_mapCoverLength;
};

