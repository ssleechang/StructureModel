#include "stdafx.h"
#include "MSCustomColumnSection.h"

IMPLEMENT_SERIAL(MSCustomColumnSection, MSCustomLineSection, VERSIONABLE_SCHEMA | 1)

MSCustomColumnSection::MSCustomColumnSection(MSBaseBuilding* pBuilding) : MSCustomLineSection(pBuilding)
{
	m_eSectionType = eSectionType::CUSTOM_COLUMN;
}

MSCustomColumnSection::~MSCustomColumnSection(void)
{

}

void MSCustomColumnSection::CopyFromMe(MSCustomColumnSection* pSource, bool bNewID /*= true*/)
{
	MSCustomLineSection::CopyFromMe(pSource);
	MSCustomColumnSection* pSourceM = (MSCustomColumnSection*)pSource;
}
