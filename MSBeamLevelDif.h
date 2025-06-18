#pragma once
#include "MSMemberLevelDif.h"


class MSLevelDifHaunch;

class StructureModel_CLASS MSBeamLevelDif :
    public MSMemberLevelDif
{
public:
    MSBeamLevelDif(void);
    ~MSBeamLevelDif(void);

 

    MSLevelDifHaunch* mp_Haunch;
};

