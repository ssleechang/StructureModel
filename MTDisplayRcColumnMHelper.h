#pragma once
#include "MTDisplayCompoMHelper.h"

class MTRecognizeSection;
class MTRecognizeData;
class StructureModel_CLASS MTDisplayRcColumnMHelper : public MTDisplayCompoMHelper
{
public:
	MTDisplayRcColumnMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTDisplayRcColumnMHelper(void);

	bool MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);
};

