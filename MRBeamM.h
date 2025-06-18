#pragma once
#include "MRLineMemberM.h"

class MRSection;

class MRBeamM :
	public MRLineMemberM
{
public:
	MRBeamM(void);
	~MRBeamM(void);

	MRSection* mp_SmallSec;
	MRSection* mp_MidSec;
	MRSection* mp_LargeSec;

};
