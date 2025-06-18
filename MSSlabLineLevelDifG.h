#pragma once
#include "MSLineLevelDifG.h"

class MSSlabG;
class StructureModel_CLASS MSSlabLineLevelDifG : public MSLineLevelDifG
{
public:
	MSSlabLineLevelDifG(MSLineLevelDifG* pDif, MSSlabG* pSlab);
	~MSSlabLineLevelDifG(void);

	

	MSLineLevelDifG* mp_LineLevelDifG;
	MSSlabG* m_pSlab;

	void CopyFromMe(MSSlabLineLevelDifG* pSource, bool bNewID=true);

	void SetLoc(double dSLoc, double dELoc);
	bool isSameDif(MSSlabLineLevelDifG* pDif);


protected:
	double m_dSLoc;
	double m_dELoc;
};

