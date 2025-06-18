#pragma once
#include "mtdisplaycompomhelper.h"

class MTRecognizeSection;
class MTRecognizeData;
class StructureModel_CLASS MTDisplaySrcColumnMHelper : public MTDisplayCompoMHelper
{
public:
	MTDisplaySrcColumnMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTDisplaySrcColumnMHelper(void);

	bool MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);
};

