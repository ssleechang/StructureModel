#pragma once

#include "MDCustomLineM.h"
class MDCustomSection;
class StructureModel_CLASS MDCustomBeamM : public MDCustomM
{
public:
	MDCustomBeamM();
	~MDCustomBeamM();
	void Serialize(CArchive& ar);
	DECLARE_SERIAL(MDCustomBeamM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MDCustomSection* GetMDCustomSection(eCustomM_SectionType eType);
private:
	void CreateSection();
	void DeleteSection();

	MDCustomSection* mp_SmallSec;
	MDCustomSection* mp_MidSec;
	MDCustomSection* mp_LargeSec;
};

