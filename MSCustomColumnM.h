#pragma once
#include "MSCustomLineM.h"

class StructureModel_CLASS MSCustomColumnM : public MSCustomLineM
{
public:
	MSCustomColumnM();
	~MSCustomColumnM();
	
	DECLARE_SERIAL(MSCustomColumnM)
	void Serialize(CArchive& ar);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	virtual void CopyFromMe(MSCompoM* pSource);
};