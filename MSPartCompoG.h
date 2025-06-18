#pragma once
#include "MSCompoG.h"

class StructureModel_CLASS MSPartCompoG :
	public MSCompoG
{
public:
	MSPartCompoG();
	~MSPartCompoG(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSPartCompoG)

	virtual void Init(bool bInConstructor=false);

};
