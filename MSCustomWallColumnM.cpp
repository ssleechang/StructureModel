#include "stdafx.h"
#include "MSCustomWallColumnM.h"

IMPLEMENT_SERIAL(MSCustomWallColumnM, MSCustomLineM, VERSIONABLE_SCHEMA | 1)

MSCustomWallColumnM::MSCustomWallColumnM()
{
	m_nType = MSCompoM::msCustomWallColumnM;
}


MSCustomWallColumnM::~MSCustomWallColumnM()
{

}

void MSCustomWallColumnM::Serialize(CArchive& ar)
{
	MSCustomLineM::Serialize(ar);
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

void MSCustomWallColumnM::Dump(EFS::EFS_Buffer& buffer)
{
	MSCustomLineM::Dump(buffer);
}

void MSCustomWallColumnM::Recovery(EFS::EFS_Buffer& buffer)
{
	MSCustomLineM::Recovery(buffer);
}

void MSCustomWallColumnM::CopyFromMe(MSCompoM* pSource)
{
	if (pSource == NULL)	return;
	MSCustomWallColumnM* pSourceM = (MSCustomWallColumnM*)pSource;


	MSCustomLineM::CopyFromMe(pSource);
}
