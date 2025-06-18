#pragma once
#include "MRSectionBar.h"

class MREdge;

class StructureModel_CLASS MRVBar :	public MRSectionBar
{
public:
	MRVBar(void);
	~MRVBar(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MRVBar)

	virtual int GetBarLocArr(vector<double>&LocArr){return 0;};

	GM2DGeometry* mp_StrpCurve;
	CArray<double, double> ma_VBarLoc;
	double m_LayerDistFromEdge;

public:
	MREdge* mp_Edge;
	CTypedPtrArray<CObArray, GM2DVector*> ma_VBarVec;

};
