#include "stdafx.h"
#include "MSCustomColumnM.h"

IMPLEMENT_SERIAL(MSCustomColumnM, MSCustomLineM, VERSIONABLE_SCHEMA | 1)

MSCustomColumnM::MSCustomColumnM()
{
	m_nType = MSCompoM::msCustomColumnM;
}


MSCustomColumnM::~MSCustomColumnM()
{

}

void MSCustomColumnM::Serialize(CArchive& ar)
{
	MSCustomLineM::Serialize(ar);
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

void MSCustomColumnM::Dump(EFS::EFS_Buffer& buffer)
{
	MSCustomLineM::Dump(buffer);
}

void MSCustomColumnM::Recovery(EFS::EFS_Buffer& buffer)
{
	MSCustomLineM::Recovery(buffer);
}

void MSCustomColumnM::CopyFromMe(MSCompoM* pSource)
{
	if (pSource == NULL)	return;
	MSCustomColumnM* pSourceM = (MSCustomColumnM*)pSource;


	MSCustomLineM::CopyFromMe(pSource);
}
