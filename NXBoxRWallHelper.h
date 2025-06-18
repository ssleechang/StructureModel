#pragma once
#include "NXBoxHelper.h"

class StructureModel_CLASS NXBoxRWallData : public NXBoxData
{
public:
	NXBoxRWallData();
	~NXBoxRWallData();

	void SetThick(double thick) { m_Thick = thick; };
	void SetLeftInside(bool IsInside) { m_IsLeftInside = IsInside; };
	void SetOffset(int topOffset, int botOffset) { m_TopOffset = topOffset, m_BotOffset = botOffset; };
	void SetStartVec(GM2DVector sVec) { m_sVec = sVec; };
	void SetEndVec(GM2DVector eVec) { m_eVec = eVec; };

	double GetThick() { return m_Thick; };
	bool GetIsLeftInside() { return m_IsLeftInside; };
	int GetTopOffset() { return m_TopOffset; };
	int GetBotOffset() { return m_BotOffset; };
	GM2DVector GetSVec() { return m_sVec; };
	GM2DVector GetEVec() { return m_eVec; };

protected:
	double m_Thick;
	bool m_IsLeftInside;
	GM2DVector m_sVec, m_eVec;
	int m_TopOffset, m_BotOffset;
};

class StructureModel_CLASS NXBoxRWallHelper :public NXBoxHelper
{
public:
	NXBoxRWallHelper(CogReader* pReader);
	~NXBoxRWallHelper();

	MSCompoG* MakeData(NXBoxData* pData) override;
protected:
	MSCompoM* CreateNewRWallM(double dT, CString TheName);
	bool CheckCompoM(MSCompoM* pCompoM);
	virtual CString MakeDefaultName() override { return _T("DEFRW1"); };
};

