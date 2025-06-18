#pragma once
#include "DG2DObject.h"
#include "GMLib\GM2DVector.h"

class GMArc;
class GMQuadTree;
class StructureModel_CLASS DG2DArc :	public DG2DObject
{
public:
	DG2DArc(GMQuadTree* pQuadTree);
	virtual ~DG2DArc();

	void Init();
	void SetArc(GM2DArc* pArc);
	void AddGMArc(GM2DArc* pNewArc);
	void AddQTs(GM2DArc* pNewArc);

	double m_dT;
	BOOL m_bOutArc;
	GM2DVector m_CenVec;
	GM2DArc* m_pArc;
	GMQuadTree* m_pQT;

	BOOL MergeWhenSameArc(GM2DArc* pNewArc, double DegreeTol, double DistTol);
};
