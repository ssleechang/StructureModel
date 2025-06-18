#pragma once
#include "NXBoxHelper.h"

class StructureModel_CLASS NXBoxWallFootData : public NXBoxData
{
public:
	NXBoxWallFootData();
	~NXBoxWallFootData();

	void SetWidth(double width) { m_Width = width; };
	void SetOffset(int offset) { m_Offset = offset; };
	void SetStartVec(GM2DVector sVec) { m_sVec = sVec; };
	void SetEndVec(GM2DVector eVec) { m_eVec = eVec; };

	double GetWidth() { return m_Width; };
	int GetOffset() { return m_Offset; };
	GM2DVector GetSVec() { return m_sVec; };
	GM2DVector GetEVec() { return m_eVec; };

protected:
	double m_Width;
	GM2DVector m_sVec, m_eVec;
	int m_Offset;
};

class StructureModel_CLASS NXBoxWallFootHelper :public NXBoxHelper
{
public:
	NXBoxWallFootHelper(CogReader* pReader);
	~NXBoxWallFootHelper();

	MSCompoG* MakeData(NXBoxData* pData) override;
protected:
	MSCompoM* CreateNewWallFootM(double dB, CString TheName);
	bool CheckCompoM(MSCompoM* pCompoM);
	virtual CString MakeDefaultName() override { return _T("DEFWF1"); };
};

