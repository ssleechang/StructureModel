#pragma once
#include "MTReaderHelper.h"
class StructureModel_CLASS MTBWallMHelper : public MTReaderHelper
{
public:
	MTBWallMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTBWallMHelper();

	virtual CString GetGroupName() { return _T("BWall"); };
	virtual MSCompoM* MakeData(CString Name, SRSectionBox* pSectionBox, SRWallInfo* pWallInfo);

protected:
	long GetFromStory();
	long GetToStory();
	
	void MakeWallData();
	void MakeVBars(CString rebarText);
	void MakeHBars(CString rebarText);
	void MakeShearRatio(CString shearText);
	void MakeWallHBars(CString rebarText, MSMainBar* pMainBar, MTResultItem::eItemType itemType);

	void MakeMainBars(CString rebarText, MSMainBar* pMainBar, MTResultItem::eItemType itemType);
	void MakeStirrupBars(CString rebarText, MSStirrupBar* pStirrupBar, int cellType, MTResultItem::eItemType itemType);

protected:
	SRSectionBox* mp_SectionBox;
	SRWallInfo* mp_WallInfo;
};

