#pragma once
#include "mtdisplaycompomhelper.h"

class MTRecognizeSection;
class MTRecognizeData;
class SRSectionTable;
class SRSectionBox;
class MSCompoM;
class MSRcBeamM;
class MSRcBeamSec;
class StructureModel_CLASS MTDisplayRcBeamMHelper : public MTDisplayCompoMHelper
{
public:
	MTDisplayRcBeamMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTDisplayRcBeamMHelper(void);

	bool MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);

protected:
	void MakeSectionDatas(MSCompoM* pCompoM, SRSectionTable* pTable, vector<MTRecognizeData*>& DataArr);
	MSRcBeamSec* GetBeamSection(MSRcBeamM* pRebarM, int nSectionCount, int SectionIdx);
	void MakeSectionDatas(MSRcBeamSec* pBeamSec, SRSectionBox* pSectionBox, vector<MTRecognizeData*>& DataArr);

};

