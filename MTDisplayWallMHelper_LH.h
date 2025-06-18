#pragma once
#include "MTDisplayCompoMHelper.h"
class MTRecognizeSection;
class MTRecognizeData;
class StructureModel_CLASS MTDisplayWallMHelper_LH :public MTDisplayCompoMHelper
{
public:
	MTDisplayWallMHelper_LH(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTDisplayWallMHelper_LH();

	bool MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);
};

