#pragma once
class StructureModel_CLASS CommonLengthItem
{
public:
	CommonLengthItem(void);
	~CommonLengthItem(void);

	void Serialize(CArchive& ar);
	void Initialize();

	long m_nCompLength;		// 압축이음
	long m_nDevLength4Hook;		// 표준후크를 갖는 정착길이

	void Copy(CommonLengthItem desCommonLengthItem);
};

