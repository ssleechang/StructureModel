#pragma once
#include "CorrectiveLengthItem.h"
class StructureModel_CLASS CorrectiveLengthSet
{
public:
	CorrectiveLengthSet(void);
	CorrectiveLengthSet(CString name);
	~CorrectiveLengthSet(void);
	
	CorrectiveLengthItem* GetCorrectiveLengthOptByType(RebarOption::CorLen_Type type);
	void Serialize(CArchive& ar);
	void Initialize();
	CString m_sName;
	vector<CorrectiveLengthItem> m_aCorrectiveLengths;

	void Copy(shared_ptr<CorrectiveLengthSet> desCorrectiveLengthSet);
};

