#pragma once
#include "mslinkmemberg.h"

class StructureModel_CLASS MSSlabLineG :	public MSLinkMemberG
{
public:
	MSSlabLineG();
	~MSSlabLineG(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSSlabLineG)

    virtual MSObject* Copy(bool bNewID = true);
    void CopyFromMe(MSSlabLineG* pSource, bool bNewID=true);
    virtual void Init(bool bInConstructor=false);

    virtual Type_MSCompoG GetType(){return msSlabLineG;};

    virtual double GetThick();


};
