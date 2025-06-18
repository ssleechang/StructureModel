#pragma once
#include "mrhbarpoint.h"

class StructureModel_CLASS MRHBarPointByVBar :	public MRHBarPoint
{
public:
	MRHBarPointByVBar(void);
	~MRHBarPointByVBar(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MRHBarPointByVBar)

	int m_EdgeIdx;
	int m_VBarIdx;
	int m_BarNum;

};
