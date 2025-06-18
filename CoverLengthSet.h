#pragma once
#include "CoverLengthItem.h"
class StructureModel_CLASS CoverLengthSet
{
public:
	CoverLengthSet(void);
	CoverLengthSet(CString name);
	~CoverLengthSet(void);
	
	CoverLengthItem* GetCoverLengthOptByTypeAndDia(RebarOption::eCoverLengthType type, CString sDiaName);
	vector<CoverLengthItem>& GetCoverLengthOptByType(RebarOption::eCoverLengthType type);//복사본
	void SetCoverLengthOptByType(RebarOption::eCoverLengthType type, vector<CoverLengthItem>& coverLengths);
	void Serialize(CArchive& ar);
	void Initialize();

	int GetCoverLengthByTypeAndFaceAndDia(RebarOption::eCoverLengthType type, RebarOption::eCoverLengthFace eFaceType, CString sDiaName) const;
	CString m_sName;
	
	map<RebarOption::eCoverLengthType, vector<CoverLengthItem>> m_mapTypeToCoverLengths;

	void Copy(shared_ptr<CoverLengthSet> desCoverLengthSet);

private:
	vector<int> GetTypeToDefaultValue(RebarOption::eCoverLengthType type);
	vector<CoverLengthItem> GetTypeToDefaultData(RebarOption::eCoverLengthType type);
	CString m_sMaxDiaName;
};

