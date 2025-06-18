#pragma once
#include "MRObject.h"

class StructureModel_CLASS MRHBarPointByLoc :	public MRObject
{
public:
	MRHBarPointByLoc(void);
	~MRHBarPointByLoc(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MRHBarPointByLoc)

	int m_EdgeIdx;
	int m_LayerIdx;
	double m_Loc;

};
