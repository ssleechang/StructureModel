#pragma once
#include "MRSectionBar.h"


class StructureModel_CLASS MRHBar :	public MRSectionBar
{
public:
	MRHBar(void);
	~MRHBar(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MRHBar)

	virtual bool ParsingBarString();

	double m_Space; 
	int m_NumBar;

};

