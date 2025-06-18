#pragma once
#include "MRSection.h"

class MREdge;
class MRHBarByPoint;
class MRHBar;

class StructureModel_CLASS MRRcSection :
	public MRSection
{
public:
	MRRcSection(void);
	~MRRcSection(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MRRcSection)

	void AddEdge(MREdge* pEdge);
	void SetStirrupTempletID(long nID);
	long GetStirrupTempletID(){return m_nStirrupTempletID;};
	void SetBarLine4Edge(double dCover, double dHBarDia, double dVBarDia);

	void DeleteEdges();
	void DeleteTieBars();

	CTypedPtrArray<CObArray, MREdge*> ma_Edge;
	CTypedPtrArray<CObArray, MRHBarByPoint*> ma_TieBar;

	double m_StirrupSpace;
	int m_StirrupNum;
	double m_TieBarSpace;
	int m_TieBarNum;

protected:
	long m_nStirrupTempletID;

};
