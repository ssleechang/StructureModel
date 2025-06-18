#pragma once
#include "msobject.h"

class StructureModel_CLASS MSRcObject :
	public MSObject
{
public:
	MSRcObject(void);
	~MSRcObject(void);

	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetStirrupBarDiaAllLayer();
};


