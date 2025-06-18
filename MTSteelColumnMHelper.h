#pragma once
#include "MTRcColumnMHelper.h"

class SRSectionTable;
class SRSteelInfo;
class StructureModel_CLASS MTSteelColumnMHelper : public MTRcColumnMHelper
{
public:
	MTSteelColumnMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTSteelColumnMHelper(void);

	MSCompoM* MakeData(SRSectionTable* pTable, SRSteelInfo* pSteelInfo);
protected:

};

