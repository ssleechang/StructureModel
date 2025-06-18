#include "StdAfx.h"
#include "MSBlockInstance.h"


MSBlockInstance::MSBlockInstance(void)
{
	mp_Block = NULL;
}


MSBlockInstance::~MSBlockInstance(void)
{
	InitMemberArr();
}

void MSBlockInstance::InitMemberArr()
{
	ma_CompoG.clear();
	ma_LevelZone.clear();
	ma_SlabRebar.clear();
}