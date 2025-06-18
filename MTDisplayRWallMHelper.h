#pragma once
#include "mtdisplaycompomhelper.h"

class MTRecognizeSection;
class MTRecognizeData;

class StructureModel_CLASS MTDisplayRWallMHelper :	public MTDisplayCompoMHelper
{
public:
	MTDisplayRWallMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTDisplayRWallMHelper(void);

	bool MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);
};

