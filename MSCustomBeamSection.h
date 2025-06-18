#pragma once
#include "MSCustomLineSection.h"

class MSBaseBuilding;
class StructureModel_CLASS MSCustomBeamSection : public MSCustomLineSection
{
public:
	MSCustomBeamSection(MSBaseBuilding* pBuilding = nullptr);
	~MSCustomBeamSection();

	DECLARE_SERIAL(MSCustomBeamSection)
	virtual void CopyFromMe(MSCustomBeamSection* pSource, bool bNewID = true);

	virtual double GetFloorLevelOffSet();
};