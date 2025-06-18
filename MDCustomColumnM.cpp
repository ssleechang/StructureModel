#include "stdafx.h"
#include "MDCustomColumnM.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////     MDCustomColumnM //////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(MDCustomColumnM, MDCustomLineM, VERSIONABLE_SCHEMA | 1)
MDCustomColumnM::MDCustomColumnM()
{

}


MDCustomColumnM::~MDCustomColumnM()
{

}

void MDCustomColumnM::Serialize(CArchive& ar)
{
	MDCustomLineM::Serialize(ar);
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

void MDCustomColumnM::CopyFromMe(MDMember* pSource)
{
	if (pSource == NULL)	return;
	MDCustomColumnM* pSourceRebar = (MDCustomColumnM*)pSource;
	
	MDCustomLineM::CopyFromMe(pSource);
}

void MDCustomColumnM::Dump(EFS::EFS_Buffer& buffer)
{
	MDCustomLineM::Dump(buffer);
}

void MDCustomColumnM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDCustomLineM::Recovery(buffer);

	
}