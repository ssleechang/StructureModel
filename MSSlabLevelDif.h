#pragma once
#include "MSMemberLevelDif.h"

class MSLineLevelDifG;
class MSLevelDifHaunch;
class MSLevelZone;
class GM2DCurve;

class StructureModel_CLASS MSSlabLevelDif :
    public MSMemberLevelDif
{
public:
    MSSlabLevelDif(void);
    ~MSSlabLevelDif(void);

	MSLevelZone* mp_LevelZone;
	GM2DCurve* mp_Curve;
	double m_LeftLevel;
	double m_RightLevel;
	double m_InfillThick;

	MSLevelDifHaunch* mp_Haunch;
};

