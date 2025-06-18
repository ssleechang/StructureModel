#include "stdafx.h"
#include "MDCustomWallColumnM.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////     MDCustomWallColumnM     //////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(MDCustomWallColumnM, MDCustomLineM, VERSIONABLE_SCHEMA | 1)
MDCustomWallColumnM::MDCustomWallColumnM()
{

}


MDCustomWallColumnM::~MDCustomWallColumnM()
{

}

void MDCustomWallColumnM::Serialize(CArchive& ar)
{
	MDCustomLineM::Serialize(ar);
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

void MDCustomWallColumnM::CopyFromMe(MDMember* pSource)
{
	if (pSource == NULL)	return;
	MDCustomWallColumnM* pSourceRebar = (MDCustomWallColumnM*)pSource;


	MDCustomLineM::CopyFromMe(pSource);
}

void MDCustomWallColumnM::Dump(EFS::EFS_Buffer& buffer)
{
	MDCustomLineM::Dump(buffer);
}

void MDCustomWallColumnM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDCustomLineM::Recovery(buffer);

}