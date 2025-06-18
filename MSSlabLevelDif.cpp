#include "StdAfx.h"
#include "MSSlabLevelDif.h"

#include "MSLevelDifHaunch.h"
#include "GMLib/GM2DCurve.h"

MSSlabLevelDif::MSSlabLevelDif(void)
{
	mp_Haunch = NULL;
	mp_Curve = NULL;
}


MSSlabLevelDif::~MSSlabLevelDif(void)
{
	if(mp_Haunch)
		delete mp_Haunch;
	if(mp_Curve)
		delete mp_Curve;
}
