#pragma once
#include "MSRcLineMemberM.h"
#include "MSStirrupBar.h"
class MSSteelSection;
class StructureModel_CLASS MSSrcBeamM : public MSRcLineMemberM
{
public:
	MSSrcBeamM(void);
	~MSSrcBeamM(void);

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSSrcBeamM)
	virtual void CopyFromMe(MSCompoM* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 
	virtual bool IsSRC(){ return true; };
	virtual MSSteelSection* GetSteelSection() { return m_pSection; };

	virtual bool IsGeometryDifferent(MSCompoM* compoM);

	virtual double GetWidth();

	MSSteelSection* m_pSection;
	long m_nSecType;
	int m_NumStudBoltBars;
	MSStirrupBar m_StudBoltBars;
	double m_StudBoltLength;
	double m_h;
};

