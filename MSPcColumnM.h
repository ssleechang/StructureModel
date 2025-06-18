#pragma once
#include "msrclinememberm.h"

class StructureModel_CLASS MSPcColumnM : public MSRcLineMemberM
{
public:
	MSPcColumnM(void);
	~MSPcColumnM(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSPcColumnM)
	virtual void CopyFromMe(MSCompoM* pSource);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	virtual bool IsGeometryDifferent(MSCompoM* compoM);
};
