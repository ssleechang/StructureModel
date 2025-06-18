#include "StdAfx.h"
#include "MSSlabLineG.h"

IMPLEMENT_SERIAL(MSSlabLineG, MSLinkMemberG,  VERSIONABLE_SCHEMA | 1)

MSSlabLineG::MSSlabLineG()
{

}

MSSlabLineG::~MSSlabLineG(void)
{
}
void MSSlabLineG::Serialize(CArchive &ar)
{
	MSLinkMemberG::Serialize(ar);
	if(ar.IsStoring())
	{
	}
	else
	{
	}
}

MSObject* MSSlabLineG::Copy( bool bNewID /*= true*/ )
{
    MSSlabLineG* pNewObj;

    pNewObj = new MSSlabLineG();
    pNewObj->CopyFromMe(this, bNewID);
    return pNewObj;
}

void MSSlabLineG::CopyFromMe( MSSlabLineG* pSource, bool bNewID/*=true*/ )
{
    MSLinkMemberG::CopyFromMe(pSource, bNewID);

    if(pSource)
    {
    }
}

void MSSlabLineG::Init( bool bInConstructor/*=false*/ )
{
    MSLinkMemberG::Init(bInConstructor);
}

double MSSlabLineG::GetThick()
{
    return 0.;
}
