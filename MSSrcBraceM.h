#pragma once
#include "mslinememberm.h"

#ifndef _MSSrcBraceM_HeaderFile
#define _MSSrcBraceM_HeaderFile

class MSSteelSection;
class StructureModel_CLASS MSSrcBraceM :
	public MSLineMemberM
{
public:
	MSSrcBraceM(void);
	~MSSrcBraceM(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSSrcBraceM)
	virtual void CopyFromMe(MSCompoM* pSource);
	virtual bool IsGeometryDifferent(MSCompoM* compoM);
// 	double m_dCvrConc;
// 	MSMainBar m_TBars;
// 	MSMainBar m_BBars;
// 	MSMainBar m_LBars;
// 	MSMainBar m_RBars;
// 	MSStirrupBar m_Bars;
// 	long m_nStirrupType;

	MSSteelSection* m_pSection;
	long m_nSecType;
};

#endif
