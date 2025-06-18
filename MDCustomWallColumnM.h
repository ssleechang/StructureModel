#pragma once

#include "MDCustomLineM.h"

class StructureModel_CLASS MDCustomWallColumnM : public MDCustomLineM
{
public:
	MDCustomWallColumnM();
	~MDCustomWallColumnM();
	void Serialize(CArchive& ar);
	DECLARE_SERIAL(MDCustomWallColumnM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
};

