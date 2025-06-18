#include "StdAfx.h"
#include "MSSlantWallMemberG.h"

IMPLEMENT_SERIAL(MSSlantWallMemberG, MSWallMemberG, VERSIONABLE_SCHEMA | 1)

MSSlantWallMemberG::MSSlantWallMemberG()
{
	Init(true);
}


MSSlantWallMemberG::~MSSlantWallMemberG(void)
{
}

void MSSlantWallMemberG::Serialize( CArchive& archive )
{

}

MSObject* MSSlantWallMemberG::Copy( bool bNewID /*= true*/ )
{
	MSSlantWallMemberG* pNewObj;

	pNewObj = new MSSlantWallMemberG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSSlantWallMemberG::CopyFromMe( MSSlantWallMemberG* pSource, bool bNewID/*=true*/)
{

}

void MSSlantWallMemberG::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSWallMemberG::Init(bInConstructor);
}
