#include "StdAfx.h"
#include "MSLevelDifHaunch.h"


MSLevelDifHaunch::MSLevelDifHaunch(void)
{
}


MSLevelDifHaunch::~MSLevelDifHaunch(void)
{
}

MSLevelDifHaunch* MSLevelDifHaunch::Copy()
{
	MSLevelDifHaunch* pNewObj = new MSLevelDifHaunch();
	pNewObj->CopyFrom(this);
	return pNewObj;
}

void MSLevelDifHaunch::CopyFrom( MSLevelDifHaunch* pSource)
{
	m_TopLength = pSource->m_TopLength;
	m_BotLength = pSource->m_BotLength;
}
