#include "StdAfx.h"
#include "MSWallOpeningExtraM.h"


IMPLEMENT_SERIAL(MSWallOpeningExtraM, MSOpeningM, VERSIONABLE_SCHEMA | 1)
MSWallOpeningExtraM::MSWallOpeningExtraM(void)
{
}


MSWallOpeningExtraM::~MSWallOpeningExtraM(void)
{
}

void MSWallOpeningExtraM::CopyFromMe(MSCompoM* pSource)
{
	if(pSource==NULL)	return;
	MSOpeningM::CopyFromMe(pSource);

	MSWallOpeningExtraM* pSourceM = (MSWallOpeningExtraM*)pSource;

}

void MSWallOpeningExtraM::Dump( EFS::EFS_Buffer& buffer )
{
	MSOpeningM::Dump(buffer);

}

void MSWallOpeningExtraM::Recovery( EFS::EFS_Buffer& buffer )
{
	MSOpeningM::Recovery(buffer);

}
void MSWallOpeningExtraM::Serialize(CArchive &ar)
{
	MSOpeningM::Serialize(ar);

	if(ar.IsStoring())
	{
	}
	else
	{
	}
}