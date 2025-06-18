#pragma once
#include "mtreaderhelper.h"

class SRSectionTable;
class MSRcBeamSec;
class MREdge;
class MSMainBarSet;
class MSSubMainBar;
class StructureModel_CLASS MTRcBeamMHelper : public MTReaderHelper
{
public:
	MTRcBeamMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTRcBeamMHelper(void);

	virtual CString GetGroupName() { return _T("Beam");};
	virtual MSCompoM* MakeData(CString Name, SRSectionTable* pTable);

	enum eSideBarType { NONE = 0, MAINBAR = 1, STIRRUPBAR};

protected:
	long GetFromStory();
	long GetToStory();

	void MakeSections();
	MSRcBeamSec* GetBeamSection(int SectionIdx);
	MTResultItem::eItemType GetBeamSectionType(int SectionIdx);

	bool MakeTopBars(SRSectionBox* pSectionBox, MSMainBarSet* pBoxBar, MSMainBarSet* pMainBar, MTResultItem::eItemType itemType);
	bool MakeBotBars(SRSectionBox* pSectionBox, MSMainBarSet* pBoxBar, MSMainBarSet* pMainBar, MTResultItem::eItemType itemType);

	bool MakeStirrupBars(SRSectionBox* pSectionBox, MSStirrupBar* pBoxBar, MSStirrupBar* pStirrupBar, long& StirrupCount, MTResultItem::eItemType itemType);
	bool MakeSideBars(SRSectionBox* pSectionBox, MSMainBar* pBoxBar, MSMainBar* pSideBar);
	bool MakeSideBars(SRSectionBox* pSectionBox, MSStirrupBar* pBoxBar, MSStirrupBar* pSideBar);
	bool MakeSubMainBars(SRSectionBox* pSectionBox, MSMainBar* pBoxBar, MSSubMainBar* pSubMainBar);


	void CopySideBars();
	bool CheckSideBars();
	void MakeSideBarError();
	vector<int> GetSideBarFigureIDs();
	eSideBarType GetSideBarType();
	void CopySideMainBars();
	void CopySideStirrupBars();
	bool CheckMainBar(MSMainBarSet* pMainBar, MREdge* pEdge);

	virtual MSRcBeamSec* GetLargeSection();
	virtual MSRcBeamSec* GetMidSection();
	virtual MSRcBeamSec* GetSmallerSection();
protected:
	SRSectionTable* mp_Table;
};

