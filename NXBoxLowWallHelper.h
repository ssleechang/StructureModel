#pragma once
#include "NXBoxHelper.h"

class StructureModel_CLASS NXBoxLowWallData : public NXBoxData
{
public:
	NXBoxLowWallData();
	~NXBoxLowWallData();

	void SetThick(double thick) { m_Thick = thick; };
	void SetOffset(int offset) { m_Offset = offset; };
	void SetStartVec(GM2DVector sVec) { m_sVec = sVec; };
	void SetEndVec(GM2DVector eVec) { m_eVec = eVec; };

	double GetThick() { return m_Thick; };
	int GetOffset() { return m_Offset; };
	GM2DVector GetSVec() { return m_sVec; };
	GM2DVector GetEVec() { return m_eVec; };

protected:
	double m_Thick;
	double m_TopDepth, m_BotDepth;
	GM2DVector m_sVec, m_eVec;
	int m_Offset;
};

class StructureModel_CLASS NXBoxLowWallHelper :public NXBoxHelper
{
public:
	NXBoxLowWallHelper(CogReader* pReader);
	~NXBoxLowWallHelper();

	MSCompoG* MakeData(NXBoxData* pData) override;
protected:
	MSCompoM* CreateNewLowWallM(double thick, CString TheName);
	bool CheckCompoM(MSCompoM* pCompoM);
	virtual CString MakeDefaultName() override { return _T("DEFLW1"); };
};

