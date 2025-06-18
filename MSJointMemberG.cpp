#include "StdAfx.h"
#include "MSJointMemberG.h"

IMPLEMENT_SERIAL(MSJointMemberG, MSVerticalLineMemberG, VERSIONABLE_SCHEMA | 1)
MSJointMemberG::MSJointMemberG()
{
	Init(true);
}

MSJointMemberG::~MSJointMemberG(void)
{
}

void MSJointMemberG::Serialize(CArchive &ar)
{
	MSVerticalLineMemberG::Serialize(ar);
	if(ar.IsStoring())
	{
	}
	else
	{
	}
}

MSObject* MSJointMemberG::Copy( bool bNewID /*= true*/ )
{
	MSJointMemberG* pNewObj;

	pNewObj = new MSJointMemberG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSJointMemberG::CopyFromMe( MSJointMemberG* pSource, bool bNewID/*=true*/)
{

}

void MSJointMemberG::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSVerticalLineMemberG::Init(bInConstructor);
}

void MSJointMemberG::RemoveJointMemParts()
{
	ma_JointMemPart.clear();
}

