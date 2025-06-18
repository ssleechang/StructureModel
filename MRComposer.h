#pragma once
#include "msobject.h"

class MRSectionZone;

class StructureModel_CLASS MRComposer :
	public MSObject
{
public:
	static int ReadComposer();
	static int WriteComposer(); 
	static CTypedPtrArray<CObArray, MRComposer*> sm_Composer;
public:
	MRComposer(void);
	~MRComposer(void);
 
	CTypedPtrArray<CObArray, MRSectionZone*> ma_Zone;
};
  