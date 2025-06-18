#pragma once
#include "MSCustomM.h"
#include "MSElement.h"
#include "MSCustomLineBarUnit.h"
#include "MSCustomPointBarUnit.h"

class MDCustomSection;
class MDCustomPointUnitRebar;
class MDCustomLineUnitRebar;
class StructureModel_CLASS MSCustomLineM : public MSCustomM
{
public:
	MSCustomLineM();
	~MSCustomLineM();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSCustomLineM)
	virtual void CopyFromMe(MSCompoM* pSource);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	virtual void MakeDefaultRebars() override;

	virtual void UpdateCustomRebarsID(map<long, long> oldUnitIDtoNewUnitID) override;
	virtual void UpdateCustomCompoM() override;
	
	MDCustomSection* GetMDCustomSection();
};

