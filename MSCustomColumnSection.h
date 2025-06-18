#pragma once
#include "MSCustomLineSection.h"

class MSBaseBuilding;
class StructureModel_CLASS MSCustomColumnSection : public MSCustomLineSection
{
public:
	MSCustomColumnSection(MSBaseBuilding* pBuilding = nullptr);
	~MSCustomColumnSection();

	DECLARE_SERIAL(MSCustomColumnSection)
	virtual void CopyFromMe(MSCustomColumnSection* pSource, bool bNewID = true);
};