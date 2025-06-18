#pragma once
#include "MDMember.h"
#include "StructureModelEnum.h"

class MDCustomSection;
class StructureModel_CLASS MDCustomM :public MDMember
{
public:
	MDCustomM();
	~MDCustomM();
	void Serialize(CArchive& ar);
	DECLARE_SERIAL(MDCustomM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MDCustomSection* GetMDCustomSection(eCustomM_SectionType eType);
};


