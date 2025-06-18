#pragma once
#include "MTReaderHelper.h"

class MDRcButtressSec;
class MSMainBarSet;
class MSStirrupBar;
class MREdge;
class StructureModel_CLASS MTButtressMHelper :	public MTReaderHelper
{
public:
	MTButtressMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTButtressMHelper();

	virtual CString GetGroupName() { return _T("Buttress"); };
	virtual MSCompoM* MakeData(CString Name, SRSectionTable* pTable);

	enum eSideBarType { NONE = 0, MAINBAR = 1, STIRRUPBAR };

protected:
	long GetFromStory();
	long GetToStory();
	bool CheckStory();

	void MakeSections();
	MDRcButtressSec* GetButtressSection(int SectionIdx);
	MTResultItem::eItemType GetButtressSectionType(int SectionIdx);

	bool MakeTopBars(SRSectionBox* pSectionBox, MSMainBarSet* pBoxBar, MSMainBarSet* pMainBar, MTResultItem::eItemType itemType);
	bool MakeBotBars(SRSectionBox* pSectionBox, MSMainBarSet* pBoxBar, MSMainBarSet* pMainBar, MTResultItem::eItemType itemType);

	bool MakeStirrupBars(SRSectionBox* pSectionBox, MSStirrupBar* pBoxBar, MSStirrupBar* pStirrupBar, int& StirrupCount, MTResultItem::eItemType itemType);
	bool MakeSideBars(SRSectionBox* pSectionBox, MSMainBar* pBoxBar, MSMainBar* pSideBar);
	bool MakeSideBars(SRSectionBox* pSectionBox, MSStirrupBar* pBoxBar, MSStirrupBar* pSideBar);
	void CopySideBars();
	bool CheckSideBars();
	void MakeSideBarError();
	vector<int> GetSideBarFigureIDs();
	eSideBarType GetSideBarType();
	void CopySideMainBars();
	void CopySideStirrupBars();
	bool CheckMainBar(MSMainBarSet* pMainBar, MREdge* pEdge);

protected:
	SRSectionTable* mp_Table;
};

