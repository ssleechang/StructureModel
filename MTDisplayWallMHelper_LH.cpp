#include "stdafx.h"
#include "MTDisplayWallMHelper_LH.h"

#include "MSRebarBuilding.h"
#include "MSFloor.h"
#include "MSCompoM.h"
#include "MSRcBWallM.h"
#include "MTReader.h"
#include "SRSectionTable.h"

MTDisplayWallMHelper_LH::MTDisplayWallMHelper_LH(MSBaseBuilding* pBldg, MTReader* pReader) : MTDisplayCompoMHelper(pBldg, pReader)
{
}


MTDisplayWallMHelper_LH::~MTDisplayWallMHelper_LH()
{
}

bool MTDisplayWallMHelper_LH::MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	for (auto it = pSection->mm_FigureIDToString_LH.begin(); it != pSection->mm_FigureIDToString_LH.end(); it++)
	{
		MakeRecognizeData(it->first, it->second, DataArr);
	}

	return true;
}
