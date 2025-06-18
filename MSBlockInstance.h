#pragma once
#include "mselement.h"

class MSBlock;
class MSCompoG;
class MSLevelZone;
class MSSlabRebar;

class StructureModel_CLASS MSBlockInstance :
	public MSElement
{
public:
	MSBlockInstance(void);
	~MSBlockInstance(void);

	MSBlock* mp_Block;
	vector<MSCompoG*> ma_CompoG;
	vector<MSLevelZone*> ma_LevelZone;
	vector<MSSlabRebar*> ma_SlabRebar;

	void InitMemberArr();
};

