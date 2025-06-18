#pragma once
#include "MRHBar.h"

class StructureModel_CLASS MRHBarByEdge :	public MRHBar
{
public:
	MRHBarByEdge(void);
	~MRHBarByEdge(void);

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MRHBarByEdge)
	CArray<int, int> ma_EdgeIdx;
	
	double m_dSLoc;
	double m_dELoc;

	AnchorType m_StartAnchorType;
	AnchorType m_EndAnchorType;

	void AddEdgeIdx(int nVal);


};
