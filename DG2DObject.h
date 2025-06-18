#pragma once
#include "afx.h"
#include "GMLib/GM2DCurve.h"
#include "GMLib/GM2DGeometry.h"

class StructureModel_CLASS DG2DObject : public GM2DCurve
{
public:
	DG2DObject(void);
	~DG2DObject(void);

	long m_DGID;
	CTypedPtrArray<CObArray, GM2DGeometry*> ma_GMs;  // colinear test를 통하여 본 DGLine에 포함된 GM2DLineVector들
};
