#pragma once
#include "mtdisplaycompomhelper.h"

class MTRecognizeSection;
class MTRecognizeData;

class StructureModel_CLASS MTDisplaySteelBeamMHelper : public MTDisplayCompoMHelper
{
public:
	MTDisplaySteelBeamMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTDisplaySteelBeamMHelper(void);

	bool MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);

};

