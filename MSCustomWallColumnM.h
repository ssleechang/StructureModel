#pragma once
#include "MSCustomLineM.h"

class StructureModel_CLASS MSCustomWallColumnM : public MSCustomLineM
{
public:
	MSCustomWallColumnM();
	~MSCustomWallColumnM();

	DECLARE_SERIAL(MSCustomWallColumnM)
	void Serialize(CArchive& ar);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	virtual void CopyFromMe(MSCompoM* pSource);
};