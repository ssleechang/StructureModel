#include "StdAfx.h"
#include "MSBeamLevelDif.h"

#include "MSLevelDifHaunch.h"

MSBeamLevelDif::MSBeamLevelDif(void)
{
    mp_Haunch = NULL;
}


MSBeamLevelDif::~MSBeamLevelDif(void)
{
    if(mp_Haunch)
        delete mp_Haunch;
}
