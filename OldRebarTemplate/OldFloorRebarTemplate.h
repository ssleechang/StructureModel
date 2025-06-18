#pragma once
#include "OldCommonRebarTemplate.h"

class StructureModel_CLASS OldFloorRebarTemplate : public OldCommonRebarTemplate
{
public:
	OldFloorRebarTemplate();
	~OldFloorRebarTemplate(void);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	DECLARE_SERIAL(OldFloorRebarTemplate)
	void Serialize(CArchive& ar);

	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);

	void DataInit();
	bool TopOptionDataCopy(OldCommonRebarTemplate* commonRebarTemplate);
	void CreateMemberAndDataCopy(OldCommonRebarTemplate* commonRebarTemplate);

	CString m_sName;
	long m_nFromStory;
	long m_nToStory;

	long m_nID;
};

