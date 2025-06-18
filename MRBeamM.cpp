#include "StdAfx.h"
#include "MRBeamM.h"
#include "MRRcSection.h"

MRBeamM::MRBeamM(void)
{
	mp_SmallSec = NULL;
	mp_MidSec = NULL;
	mp_LargeSec = NULL;
}

MRBeamM::~MRBeamM(void)
{
	if(mp_SmallSec)	delete mp_SmallSec;
	if(mp_MidSec)	delete mp_MidSec;
	if(mp_SmallSec)	delete mp_SmallSec;
}
