#pragma once
#include "MRVBar.h"

class StructureModel_CLASS MRVBarByNum :
	public MRVBar
{
public:
	MRVBarByNum(void);
	~MRVBarByNum(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MRVBarByNum)

	virtual int GetBarLocArr(vector<double>&LocArr);
	int GetNumBar(){return m_NumBar;};
	void SetNumBar(int nVal){m_NumBar = nVal;};
	bool Get_bHasCornerBar(){return m_bHasCornerBar;};
	void Set_bHasCornerBar(bool bVal){m_bHasCornerBar = bVal;};

private:
	bool m_bHasCornerBar;
	int m_NumBar;
};
