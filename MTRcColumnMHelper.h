#pragma once
#include "mtreaderhelper.h"

class SRSectionBox;
class MSSpecificTieBar;
class MSSubMainBar;
class MSMainBarSet;
class StructureModel_CLASS MTRcColumnMHelper : public MTReaderHelper
{
public:
	MTRcColumnMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTRcColumnMHelper(void);

	virtual CString GetGroupName() { return _T("Column");};

	virtual MSCompoM* MakeData(CString Name, SRSectionBox* pSectionBox);

protected:
	long GetFromStory();
	long GetToStory();
	bool CheckStory();
	// Rect Column
	void MakeRect_Data();
	bool MakeRect_MainBars(MSMainBarSet* pTBar, MSMainBarSet* pBBar, MSMainBarSet* pLBar, MSMainBarSet* pRBar, double CvrConc);
	bool MakeRect_TieBarCount(long& rowTie, long& colTie, long nTopBars, long nLeftBars);
	bool MakeRect_TieBars(MSStirrupBar* pRowCenBar, MSStirrupBar* pColCenBar, MSStirrupBar* pRowEndBar, MSStirrupBar* pColEndBar, MSStirrupBar* pCenHoop, MSStirrupBar* pEndHoop);

	bool CheckRect_MainBars();
	bool CheckRect_MainBar2Layer(MSMainBarSet* pTBar, MSMainBarSet* pLBar);
	bool CheckRect_MainBarMinSpacing(MSMainBarSet* pTBar, MSMainBarSet* pLBar, double CvrConc);

	// Round Column
	void MakeRound_Data();
	bool MakeRound_MainBars(MSMainBarSet* pTBar);
	bool MakeRound_TieBars(long& rowTieBar, long& colTieBar, MSMainBarSet* pTBar);
	bool CheckRound_MainBars();

	bool MakeStirrupBars(MSStirrupBar* pCenBar, MSStirrupBar* pEndBar);
	bool MakeDoubleHoop(BOOL& IsDoubleHoop);
	bool MakeSpecificTieBars(vector<MSSpecificTieBar*>& SpecificTieBarArr);
	bool MakeSubMainBars(MSSubMainBar* pSubMainBar);

	void MakePolygon_Data();

	bool CheckHoopBarMinSpacing(MSMainBarSet* pTBar, MSStirrupBar* pCenHoopBar);
	void ParsingHoopBars(MSStirrupBar* pStirrupBar, CString& cenHoop, CString& endHoop);

	CString GetRectangleText() { return _T("Rectangle"); };
	CString GetCircleText() { return _T("Circle"); };

	bool ParsingHoopBars(SRSectionBox* pBox, CString& cenHoop, CString& endHoop);
	bool AnalyizeStirrupBar(vector<CString> textList, CString& cenHoop, CString& endHoop);
	void AnalyizeText(CString sBarText, CString& cenHoop, CString& endHoop);


protected:
	SRSectionBox* mp_SectionBox;
};

