#pragma once
#include "afx.h"

#include "mtreaderhelper.h"
class StructureModel_CLASS MTRWallMHelper : public MTReaderHelper
{
public:
	MTRWallMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTRWallMHelper(void);

	virtual CString GetGroupName() { return _T("RWall");};
	virtual MSCompoM* MakeData(CString Name, SRSectionBox* pSectionBox);

protected:
	long GetFromStory();
	long GetToStory();
	bool CheckStory();

	bool MakeStoryData(GMText* pText);
	bool MakeThickData();

	bool Make_ExtVertDatas();
	bool Make_IntVertDatas();
	bool Make_ExtHoriDatas();
	bool Make_IntHoriDatas();
	bool Make_ShearTopBotDatas();
	bool Make_ShearStartEndDatas();

	bool MakeVerticalMainBar(MTReader::CellType cellType, MSStirrupBar* pStirrupBar);
	bool MakeVerticalAddedBar(MTReader::CellType cellType, MSStirrupBar* pStirrupBar, long& stirrupBarLength);
	bool MakeHorizontalBar(MTReader::CellType cellType, MSStirrupBar* pStirrupBar);
	bool MakeShearBars(MTReader::CellType cellType, MSStirrupBar* pHStirrupBar, MSStirrupBar* pVStirrupBar, long& barLength);

	void MakeStirrupBar(CString rebarText, MSStirrupBar* pStirrupBar);
	void MakeAddedStirrupBar(MTReader::CellType cellType, CString rebarText, MSStirrupBar* pStirrupBar, long& stirrupBarLength);
	void MakeShearBars(MTReader::CellType cellType, CString rebaText, MSStirrupBar* pHStirrupBar, MSStirrupBar* pVStirrupBar, long& barLength);

	long Parsing_StirrupBarLength(CString rebarText);
	bool Parsing_ShearBars(MTReader::CellType cellType, CString rebaText, MSStirrupBar* pHStirrupBar, MSStirrupBar* pVStirrupBar, long& barLength);

	bool IsDuplicatedRebarInfo(MTReader::CellType cellType);
	bool IsRebarText(CString rebarText);
	CString GetRebarLocation(MTReader::CellType cellType);

	GMText* FindRebarText(vector<GMText*>& textArr, MTReader::CellType cellType);
	vector<CString> GetShearBarParsers(CString rebarText);

	void CreateErrorObjectByShearBar(MTReader::CellType cellType);
protected:
	SRSectionBox* mp_SectionBox;
};

