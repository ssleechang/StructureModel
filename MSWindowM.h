#pragma once
#include "msopeningm.h"
class StructureModel_CLASS MSWindowM : public MSOpeningM
{
public:
	MSWindowM(void);
	~MSWindowM(void);

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSWindowM)
	virtual void CopyFromMe(MSCompoM* pSource);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 
};

