#pragma once
#include "MTReaderHelper.h"

class SRSlabInfo;

class StructureModel_CLASS MTRcSlabMHelper : public MTReaderHelper
{
public:
	MTRcSlabMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTRcSlabMHelper();

	virtual CString GetGroupName() { return _T("Slab"); };
	virtual MSCompoM* MakeData(CString Name, SRSectionTable* pTable);

protected:
	long GetFromStory();
	long GetToStory();

	void MakeSlabRebars(bool IsShortBar, SRSlabInfo* pSlabInfo);

	CString GetRebarTextFromSlabInfo(int BarNum, bool IsShortBar, SRSlabInfo* pSlabInfo);
	void MakeStirrupBar(CString rebarText, MSStirrupBar* pStirrupBar, long BarNum, bool IsShort);

	void MakeRebarTextDataArr();
	void SetIsUsedStirrupBar(long BarNum, bool IsShort, bool IsUsed);
	MTResultItem* GetSubResultItemByBarNum(MTResultItem::eItemType stripType, int barNum);
	CString GetStripTypeName(int eStripType);

protected:
	SRSectionTable* mp_Table;


	vector<CString> ma_ShortRebarText, ma_LongRebarText;
};

