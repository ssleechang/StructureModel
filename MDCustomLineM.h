#pragma once

#include "MDCustomM.h"

class MDCustomSection;
class MDCustomPointUnitRebar;
class MDCustomLineUnitRebar;
class MSCustomPointBarUnit;
class MSCustomLineBarUnit;
class MSCustomLineSection;
class StructureModel_CLASS MDCustomLineM :public MDCustomM
{
public:
	MDCustomLineM();
	~MDCustomLineM();
	void Serialize(CArchive& ar);
	DECLARE_SERIAL(MDCustomLineM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MDCustomSection* GetMDCustomSection(eCustomM_SectionType eType);

	MDCustomSection* GetMDCustomSection() { return m_pMDCustomSection; };
protected:
	MDCustomSection* m_pMDCustomSection;
};

