#pragma once
#include "mscompom.h"

class StructureModel_CLASS MSSolidCompoM :
	public MSCompoM
{
public:
	MSSolidCompoM(void);
	~MSSolidCompoM(void);
	virtual void CopyFromMe(MSCompoM* pSource);
};
