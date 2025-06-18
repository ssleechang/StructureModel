#pragma once
#include "MSRcLineMemberM.h"

class StructureModel_CLASS MSPcLineMemberM : public MSRcLineMemberM
{
public:
	MSPcLineMemberM(void);
	~MSPcLineMemberM(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSPcLineMemberM)
	virtual void CopyFromMe(MSCompoM* pSource);

	virtual double GetPcWidth()	{ return m_dPcWidth;};
	virtual double GetPcDepth()	{ return m_dPcDepth;};
	void SetPcWidth(double Width){m_dPcWidth = Width;};
	void SetPcDepth(double Depth){m_dPcDepth = Depth;};

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	virtual bool IsGeometryDifferent(MSCompoM* compoM);
protected:
	double m_dPcWidth;
	double m_dPcDepth;
};
