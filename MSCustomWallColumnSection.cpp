#include "stdafx.h"
#include "MSCustomWallColumnSection.h"

IMPLEMENT_SERIAL(MSCustomWallColumnSection, MSCustomLineSection, VERSIONABLE_SCHEMA | 1)

MSCustomWallColumnSection::MSCustomWallColumnSection(MSBaseBuilding* pBuilding) : MSCustomLineSection(pBuilding)
{
	m_eSectionType = eSectionType::CUSTOM_WALLCOLUMN;
}

MSCustomWallColumnSection::~MSCustomWallColumnSection(void)
{

}

void MSCustomWallColumnSection::CopyFromMe(MSCustomSection* pSource, bool bNewID /*= true*/)
{
	MSCustomLineSection::CopyFromMe(pSource);
	MSCustomWallColumnSection* pSourceM = (MSCustomWallColumnSection*)pSource;
}
