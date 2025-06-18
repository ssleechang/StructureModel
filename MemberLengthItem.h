#pragma once
class StructureModel_CLASS MemberLengthItem
{
public:
	MemberLengthItem(void);
	~MemberLengthItem(void);

	void Serialize(CArchive& ar);
	void Initialize();

	double m_dDevelopmentLength;
	double m_dSpliceLength;

	void Copy(MemberLengthItem memberLengthItem);
};

