#pragma once
#include "MRVBar.h"

class StructureModel_CLASS MRVBarByLoc : 
	public MRVBar
{
public:
	MRVBarByLoc(void);
	~MRVBarByLoc(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MRVBarByLoc)

	virtual int GetBarLocArr(vector<double>&LocArr);


	double m_Loc;
};
