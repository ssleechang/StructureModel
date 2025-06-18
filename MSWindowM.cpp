#include "StdAfx.h"
#include "MSWindowM.h"


IMPLEMENT_SERIAL(MSWindowM, MSOpeningM, VERSIONABLE_SCHEMA | 1)

MSWindowM::MSWindowM(void)
{
	m_nType = msWindowM;
}

MSWindowM::~MSWindowM(void)
{
}

void MSWindowM::CopyFromMe(MSCompoM* pSource)
{
	if(pSource==NULL)	return;

	MSOpeningM::CopyFromMe(pSource);
}

void MSWindowM::Dump( EFS::EFS_Buffer& buffer )
{
	MSOpeningM::Dump(buffer);
}

void MSWindowM::Recovery( EFS::EFS_Buffer& buffer )
{
	MSOpeningM::Recovery(buffer);
}

void MSWindowM::Serialize(CArchive &ar)
{
	MSOpeningM::Serialize(ar);
	if(ar.IsStoring())
	{
	}
	else
	{
	}
}

