#pragma once
#include "MRHBar.h"

class MRHBarPointByLoc;

class StructureModel_CLASS MRHBarByPoint :
	public MRHBar
{
public:
	MRHBarByPoint(void);
	~MRHBarByPoint(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MRHBarByPoint)

	CTypedPtrArray<CObArray,MRHBarPointByLoc*> ma_Point;
	void DeleteHBarPoints();

	AnchorType m_StartAnchorType;
	AnchorType m_EndAnchorType;
};

