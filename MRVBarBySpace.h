#pragma once
#include "MRVBar.h"

class StructureModel_CLASS MRVBarBySpace :
	public MRVBar
{
public:
	MRVBarBySpace(void);
	~MRVBarBySpace(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MRVBarBySpace)

	virtual int GetBarLocArr(vector<double>&LocArr);


	double m_Space;
};
