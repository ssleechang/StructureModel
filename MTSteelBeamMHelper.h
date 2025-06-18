#pragma once
#include "mtrcbeammhelper.h"

class SRSectionTable;
class SRSteelInfo;
class StructureModel_CLASS MTSteelBeamMHelper : public MTRcBeamMHelper
{
public:
	MTSteelBeamMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTSteelBeamMHelper(void);

	MSCompoM* MakeData(SRSectionTable* pTable, SRSteelInfo* pSteelInfo);
};

