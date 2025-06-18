#pragma once
#include "MRObject.h"
#include "GMLib/GM2DVector.h"

class GM2DGeometry;
class MRVBar;

class StructureModel_CLASS MREdge :
	public MRObject
{
public:
	MREdge(void);
	MREdge(GM2DGeometry* pCurve);
	~MREdge(void);

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MREdge)

	bool SetCurve(GM2DGeometry* pCurve);
	GM2DGeometry* mp_Curve;  // GMArc, GMCircle, GM2DLineVector
	CTypedPtrArray<CObArray, MRVBar*> ma_VBarSet;

	void AddVBarSet(MRVBar* pRebar);
	void AddVBarLayer(double LayerDist, double dTol);
	bool AddVBarLoc(GM2DVector& vVBar, double dTol);
	void AddVBarOnCircle(GMVector& vVBar, double dTol);
	bool SetVBarByCount(CString sDiaName, CString BarType);
	bool SetVBarByCount(long NumBar, CString sDiaName, CString BarType);
	long RemoveCornerVBar(bool bStart, bool bEnd, double scaleFactor);

	bool CalcVBarVectors();
	void DeleteVBarVec();
	long GetNumVBarOfEdge();

	void ReverseLocValues();
	void Reverse();
public:
	GM2DVector m_P0;  // 에지간의 연결성 찾을 때 임시 사용
	GM2DVector m_P1;

	MREdge* mp_Linked0;
	long m_Link0;
	MREdge* mp_Linked1;
	long m_Link1;

	GM2DGeometry* mp_StrpCurve;
	GM2DGeometry* mp_HBarCurve;
	GM2DGeometry* mp_VBarCurve;

	double m_VBarDistTol;
};
