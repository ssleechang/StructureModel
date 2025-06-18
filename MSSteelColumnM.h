#pragma once
#include "MSLineMemberM.h"

#ifndef _MSSteelColumnM_HeaderFile
#define _MSSteelColumnM_HeaderFile

class MSSteelSection;
class StructureModel_CLASS MSSteelColumnM : public MSLineMemberM
{
public:
	MSSteelColumnM(void);
	~MSSteelColumnM(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSSteelColumnM)
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
	long m_nSecType;
	MSSteelSection* m_pSection;

};

#endif
