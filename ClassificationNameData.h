#pragma once
#include "MSObject.h"
#include "MSCompoM.h"
#include "StructureModel/MSElement.h"
class StructureModel_CLASS ClassificationNameData : public MSObject
{
public:

	ClassificationNameData(void);
	~ClassificationNameData(void);
	void Serialize(CArchive& archive);
	DECLARE_SERIAL(ClassificationNameData)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);
	
	long m_nID;
	CString m_sName;
	MSCompoM::Type_MSCompoM compoMType;
	MSMaterial::Type_MSMaterial materialType;
};