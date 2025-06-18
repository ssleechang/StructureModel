#include "StdAfx.h"
#include "MSRevision.h"

IMPLEMENT_SERIAL( MSRevision, CObject, VERSIONABLE_SCHEMA | 1 )

MSRevision::MSRevision(void)
{
}


MSRevision::~MSRevision(void)
{
}

void MSRevision::SetTime()
{
	m_TimeStamp = CTime::GetCurrentTime();
}

void MSRevision::Serialize( CArchive& archive )
{

}
