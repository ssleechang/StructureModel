#pragma once
#include "mtdisplaycompomhelper.h"

class MDRcButtressM;
class MDRcButtressSec;
class MTRecognizeSection;
class MTRecognizeData;
class SRSectionTable;
class SRSectionBox;

class StructureModel_CLASS MTDisplayButtressMHelper : public MTDisplayCompoMHelper
{
public:
	MTDisplayButtressMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTDisplayButtressMHelper(void);

	bool MakeData(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr);
protected:
	void MakeSectionDatas(MSCompoM* pCompoM, SRSectionTable* pTable, vector<MTRecognizeData*>& DataArr);
	MDRcButtressSec* GetButtressSection(MDRcButtressM* pRebarM, int nSectionCount, int SectionIdx);
	void MakeSectionDatas(MDRcButtressSec* pButtressSec, SRSectionBox* pSectionBox, vector<MTRecognizeData*>& DataArr);

};

