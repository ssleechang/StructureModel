#pragma once
#include "afx.h"

#include "MTDisplayCompoMHelper.h"

class MTRecognizeSection;
class MTRecognizeData;

class StructureModel_CLASS MTDisplaySteelColumnMHelper : public MTDisplayCompoMHelper
{
public:
	MTDisplaySteelColumnMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTDisplaySteelColumnMHelper(void);

	bool MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);
};

