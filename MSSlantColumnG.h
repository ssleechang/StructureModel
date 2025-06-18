#pragma once
#include "mscolumng.h"


class StructureModel_CLASS MSSlantColumnG :	public MSColumnG
{
public:
	MSSlantColumnG(void);
	~MSSlantColumnG(void);

	
	GM2DVector m_EVec;

};
