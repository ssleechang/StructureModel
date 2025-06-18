#pragma once
#include "mtdisplaycompomhelper.h"

class MTRecognizeSection;
class MTRecognizeData;
class MSCompoM;
class MDSrcBeamM;
class MSRcBeamSec;
class SRSectionTable;
class SRSectionBox;
class StructureModel_CLASS MTDisplaySrcBeamMHelper : public MTDisplayCompoMHelper
{
public:
	MTDisplaySrcBeamMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTDisplaySrcBeamMHelper(void);

	bool MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);

protected:
	void MakeSectionDatas(MSCompoM* pCompoM, SRSectionTable* pTable, vector<MTRecognizeData*>& DataArr);
	MSRcBeamSec* GetBeamSection(MDSrcBeamM* pRebarM, int nSectionCount, int SectionIdx);
	void MakeSectionDatas(MSRcBeamSec* pBeamSec, SRSectionBox* pSectionBox, vector<MTRecognizeData*>& DataArr);

};

