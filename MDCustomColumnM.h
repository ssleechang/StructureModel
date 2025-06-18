#pragma once

#include "MDCustomLineM.h"

class StructureModel_CLASS MDCustomColumnM : public MDCustomLineM
{
public:
	MDCustomColumnM();
	~MDCustomColumnM();
	void Serialize(CArchive& ar);
	DECLARE_SERIAL(MDCustomColumnM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
};