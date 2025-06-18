#include "StdAfx.h"
#include "MSSolidCompoM.h"

MSSolidCompoM::MSSolidCompoM(void)
{
}

MSSolidCompoM::~MSSolidCompoM(void)
{
}

void MSSolidCompoM::CopyFromMe(MSCompoM* pSource)
{
	if(pSource==NULL)	return;
	MSCompoM::CopyFromMe(pSource);
}
