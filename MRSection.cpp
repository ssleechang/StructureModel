#include "StdAfx.h"
#include "MRSection.h"

IMPLEMENT_SERIAL(MRSection, MRObject, VERSIONABLE_SCHEMA | 1)
MRSection::MRSection(void)
{
}

MRSection::~MRSection(void)
{
}
void MRSection::Serialize(CArchive &ar)
{
	MRObject::Serialize(ar);
}