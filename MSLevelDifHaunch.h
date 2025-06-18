#pragma once
#include "msobject.h"
class MSLevelDifHaunch :
    public MSObject
{
public:
    MSLevelDifHaunch(void);
    ~MSLevelDifHaunch(void);

	void CopyFrom(MSLevelDifHaunch* pSource);
	MSLevelDifHaunch* Copy();
    double m_TopLength;
    double m_BotLength;
};

