#pragma once
#include "OldCommonRebarTemplate.h"


class StructureModel_CLASS OldMemberRebarTemplate : public OldCommonRebarTemplate
{
public:
	OldMemberRebarTemplate();
	~OldMemberRebarTemplate(void);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	DECLARE_SERIAL(OldMemberRebarTemplate)
	void Serialize(CArchive& ar);

	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);

	void DataInit();
	void TopOptionDataCopy(OldCommonRebarTemplate* commonRebarTemplate);
	void CreateMemberAndDataCopy(OldCommonRebarTemplate* commonRebarTemplate);
	void InfoSetCheckBoxChecked();

	CString m_sName;
	long m_nMemberType;
	long m_nID;

	long m_nTopOptionID;
};

