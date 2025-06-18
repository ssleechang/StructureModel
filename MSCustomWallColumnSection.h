#pragma once
#include "MSCustomLineSection.h"

class MSBaseBuilding;
class StructureModel_CLASS MSCustomWallColumnSection : public MSCustomLineSection
{
public:
	MSCustomWallColumnSection(MSBaseBuilding* pBuilding = nullptr);
	~MSCustomWallColumnSection();

	DECLARE_SERIAL(MSCustomWallColumnSection)
	virtual void CopyFromMe(MSCustomSection* pSource, bool bNewID = true);
};