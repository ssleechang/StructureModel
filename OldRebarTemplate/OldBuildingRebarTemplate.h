#pragma once
#include "OldCommonRebarTemplate.h"
#include "..\RebarTemplate.h"

class StructureModel_CLASS OldBuildingRebarTemplate : public OldCommonRebarTemplate
{
public:
	OldBuildingRebarTemplate();
	~OldBuildingRebarTemplate(void);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	DECLARE_SERIAL(OldBuildingRebarTemplate)
	void Serialize(CArchive& archive);

	void SerializeReadForBefore20160122(CArchive &ar, int firstData);

	void DataInit();
	void Patch_20160106_OldDiaLengthData(map<long, long>& convertMapData);
	void Patch_20160106_OldDiaMapData(map<long, long>& convertMapData);
	void Patch_20160106_OldDiaData(long& convertData);
	void CreateData();
	CString m_sName;

	void OldBuildingDataCopy(RebarTemplate* rebarTemplate);

	long m_nID;
};

