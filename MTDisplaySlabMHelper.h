#pragma once
#include "mtdisplaycompomhelper.h"

class MTRecognizeSection;
class MTRecognizeData;
class StructureModel_CLASS MTDisplaySlabMHelper : public MTDisplayCompoMHelper
{
public:
	MTDisplaySlabMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTDisplaySlabMHelper(void);

	bool MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);
protected:

};

