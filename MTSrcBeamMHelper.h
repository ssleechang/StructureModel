#pragma once
#include "mtrcbeammhelper.h"

class MSSubMainBar;
class StructureModel_CLASS MTSrcBeamMHelper : public MTRcBeamMHelper
{
public:
	MTSrcBeamMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTSrcBeamMHelper(void);

	MSCompoM* MakeData(CString Name, SRSectionTable* pTable);

protected:
	MSRcBeamSec* GetLargeSection() override;
	MSRcBeamSec* GetMidSection() override;
	MSRcBeamSec* GetSmallerSection() override;

};

