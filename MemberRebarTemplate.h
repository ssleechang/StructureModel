#pragma once
#include "CommonRebarTemplate.h"


class StructureModel_CLASS MemberRebarTemplate : public CommonRebarTemplate
{
public:
	MemberRebarTemplate();
	~MemberRebarTemplate(void);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	DECLARE_SERIAL(MemberRebarTemplate)
	void Serialize(CArchive& ar);

	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);

	void DataInit();
	void TopOptionDataCopy(CommonRebarTemplate* commonRebarTemplate);
	void CreateMemberAndDataCopy(CommonRebarTemplate* commonRebarTemplate);
	void InfoSetCheckBoxChecked();

	CString m_sName;
	long m_nMemberType;
	long m_nID;

	long m_nTopOptionID;
};

