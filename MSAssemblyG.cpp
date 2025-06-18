#include "StdAfx.h"
#include "MSAssemblyG.h"

IMPLEMENT_SERIAL(MSAssemblyG, MSCompoG, VERSIONABLE_SCHEMA | 1)

MSAssemblyG::MSAssemblyG(void)
{
	Init(true);
}


MSAssemblyG::~MSAssemblyG(void)
{
}

void MSAssemblyG::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSCompoG::Init(bInConstructor);
}

void MSAssemblyG::Serialize( CArchive& archive )
{

}

MSObject* MSAssemblyG::Copy( bool bNewID /*= true*/ )
{
	MSAssemblyG* pNewObj;

	pNewObj = new MSAssemblyG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSAssemblyG::CopyFromMe( MSAssemblyG* pSource, bool bNewID/*=true*/ )
{

}
