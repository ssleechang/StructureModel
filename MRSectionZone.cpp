#include "StdAfx.h"
#include "MRSectionZone.h"
IMPLEMENT_SERIAL(MRSectionZone, MRObject, VERSIONABLE_SCHEMA | 1)

MRSectionZone::MRSectionZone(void)
{
	m_SectionIdx = -1;
	mp_Section = NULL;
	m_SLoc = 0.0;
	m_ELoc = 1.0;
}

MRSectionZone::~MRSectionZone(void)
{
}

void MRSectionZone::Serialize( CArchive& archive )
{

}
