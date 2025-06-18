#pragma once
#include "mslinememberm.h"

#ifndef _MSSteelBraceM_HeaderFile
#define _MSSteelBraceM_HeaderFile

class MSSteelSection;
class StructureModel_CLASS MSSteelBraceM :
	public MSLineMemberM
{
public:
	MSSteelBraceM(void);
	~MSSteelBraceM(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSSteelBraceM)
	virtual void CopyFromMe(MSCompoM* pSource);
	virtual bool IsGeometryDifferent(MSCompoM* compoM);

	MSSteelSection* m_pSection;
	long m_nSecType;
	BOOL m_bDoubleSection;
};

#endif
