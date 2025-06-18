#pragma once
#include "mtdisplaycompomhelper.h"

class MTRecognizeSection;
class MTRecognizeData;
class StructureModel_CLASS MTDisplayWallMHelper : public MTDisplayCompoMHelper
{
public:
	MTDisplayWallMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTDisplayWallMHelper(void);

	bool MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);
};

