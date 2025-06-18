#pragma once
#include "mrobject.h"

class StructureModel_CLASS MRSection :
	public MRObject
{
public:
	MRSection(void);
	~MRSection(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MRSection)

};
