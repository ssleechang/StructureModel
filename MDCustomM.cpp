#include "stdafx.h"
#include "MDCustomM.h"
#include "MDCustomSection.h"

IMPLEMENT_SERIAL(MDCustomM, MDMember, VERSIONABLE_SCHEMA | 1)
MDCustomM::MDCustomM()
{
}


MDCustomM::~MDCustomM()
{
}

void MDCustomM::Serialize(CArchive& ar)
{
	MDMember::Serialize(ar);
}

void MDCustomM::CopyFromMe(MDMember* pSource)
{

	MDMember::CopyFromMe(pSource);
}

void MDCustomM::Dump(EFS::EFS_Buffer& buffer)
{
	MDMember::Dump(buffer);
}

void MDCustomM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDMember::Recovery(buffer);
}

MDCustomSection* MDCustomM::GetMDCustomSection(eCustomM_SectionType eType)
{
	return nullptr;
}

