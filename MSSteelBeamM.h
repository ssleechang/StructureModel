#pragma once
#include "mslinememberm.h"

#ifndef _MSSteelBeamM_HeaderFile
#define _MSSteelBeamM_HeaderFile

class MSSteelSection;
class StructureModel_CLASS MSSteelBeamM :
	public MSLineMemberM
{
public:
	MSSteelBeamM(void);
	~MSSteelBeamM(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSSteelBeamM)
	virtual void CopyFromMe(MSCompoM* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 
	virtual bool IsSteel(){ return true; };
	virtual MSSteelSection* GetSteelSection() { return m_pSection; };

	virtual double GetWidth();
	virtual double GetDepth();
	virtual double GetThick();

	virtual void GetSecPolyline(GM2DPolyline& APGon);
	virtual bool IsGeometryDifferent(MSCompoM* compoM);

	MSSteelSection* m_pSection;
	long m_nSecType;
};

#endif
