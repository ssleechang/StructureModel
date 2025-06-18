#include "StdAfx.h"
#include "MSLineMemberM.h"


IMPLEMENT_SERIAL(MSLineMemberM, MSCompoM, VERSIONABLE_SCHEMA | 1)
MSLineMemberM::MSLineMemberM(void)
{
}

MSLineMemberM::~MSLineMemberM(void)
{
}
void MSLineMemberM::CopyFromMe(MSCompoM* pSource)
{
	if(pSource==NULL)	return;
	MSLineMemberM* pSourceLineM = (MSLineMemberM*)pSource;
	MSCompoM::CopyFromMe(pSource);
}

void MSLineMemberM::Serialize(CArchive &ar)
{
	MSCompoM::Serialize(ar);
	if(ar.IsStoring())
	{
	}
	else
	{
	}
}


CString MSLineMemberM::ToString()
{
	auto str = MSCompoM::ToString();
	return str;
}
