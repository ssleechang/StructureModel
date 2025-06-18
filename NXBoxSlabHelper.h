#pragma once
#include "NXBoxHelper.h"

class StructureModel_CLASS NXBoxSlabData: public NXBoxData
{
public:
	NXBoxSlabData();
	~NXBoxSlabData();

	void SetOffset(int offset) { m_Offset = offset; };
	void SetBoundary(GM2DPolyline* pBoundary) { mp_Boundary = pBoundary;};

	int GetOffset() { return m_Offset; };
	GM2DPolyline* GetBoundary() { return mp_Boundary; };

protected:
	int m_Offset;
	GM2DPolyline* mp_Boundary;
};
class StructureModel_CLASS NXBoxSlabHelper :public NXBoxHelper
{
public:
	NXBoxSlabHelper(CogReader* pReader);
	~NXBoxSlabHelper();

	MSCompoG* MakeData(NXBoxData* pData) override;
protected:
	MSCompoM* CreateNewSlabM(double dT, CString TheName);
	bool CheckCompoM(MSCompoM* pCompoM);
	virtual CString MakeDefaultName() override { return _T("DEFS1"); };
};

