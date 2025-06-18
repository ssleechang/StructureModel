#pragma once
#include "afx.h"

class MSBaseBuilding;
class MSCompoM;
class MTReader;
class MSMainBar;
class MSStirrupBar;
class GMText;
class MTRecognizeData;
class MSMainBarSet;
class SRSectionBox;
class StructureModel_CLASS MTDisplayCompoMHelper : public CObject
{
public:
	MTDisplayCompoMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTDisplayCompoMHelper(void);
		
	bool MakeRecognizeData(GMText* pText, CString sData, vector<MTRecognizeData*>& DataArr);
	bool MakeRecognizeData(vector<int> figureIDs, CString sData, vector<MTRecognizeData*>& DataArr);
	bool MakeRecognizeData(int figureId, CString sData, vector<MTRecognizeData*>& DataArr);

	CString GetName(vector<MSCompoM*>& compoMs);
	CString GetMainBarString(MSMainBar* pMainBar);
	CString GetMainBarSetString(MSMainBarSet* pMainBar);
	CString GetStirrupBarString(MSStirrupBar* pStirrupBar);
	CString GetShearBarString(MSStirrupBar* pHStirrupBar, MSStirrupBar* pVStirrupBar);

protected:
	void MakeSectionRect(SRSectionBox* pSectionBox);

protected:
	MSBaseBuilding* mp_Bldg;
	MTReader*   mp_Reader;
};

