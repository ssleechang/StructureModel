#pragma once
#include "mtrccolumnmhelper.h"
class StructureModel_CLASS MTSrcColumnMHelper : public MTRcColumnMHelper
{
public:
	MTSrcColumnMHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTSrcColumnMHelper(void);

	virtual MSCompoM* MakeData(CString Name, SRSectionBox* pSectionBox);

protected:
	void MakeSrcRect_Data();
	void MakeSrcRound_Data();
	void MakeSrcPolygon_Data();

};

