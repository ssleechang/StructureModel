#pragma once
#include "NXBoxHelper.h"

class StructureModel_CLASS NXBoxWallOpeningData
{
public:
	NXBoxWallOpeningData();
	~NXBoxWallOpeningData();

	void SetName(CString name) { m_Name = name; };
	void SetBotLevel(int botLevel) { m_BotLevel = botLevel; };
	void SetHeight(int height) { m_Height = height; };
	void SetStartVec(GM2DVector sVec) { m_sVec = sVec; };
	void SetEndVec(GM2DVector eVec) { m_eVec = eVec; };
	void SetExtSize(int extSize) { m_ExtSize = extSize; };

	CString GetName() { return m_Name; };
	int GetBotLevel() { return m_BotLevel; };
	int GetHeight() { return m_Height; };
	int GetExtSize() { return m_ExtSize; };
	GM2DVector GetStartVec() { return m_sVec; };
	GM2DVector GetEndVec() { return m_eVec; };

	CString MakeName();
	double GetLength();
protected:
	CString m_Name;
	GM2DVector m_sVec, m_eVec;
	int m_BotLevel;
	int m_Height;
	int m_ExtSize;
};

class StructureModel_CLASS NXBoxWallData : public NXBoxData
{
public:
	NXBoxWallData();
	~NXBoxWallData();

	void SetThick(double thick) { m_Thick = thick; };
	void SetOffset(int topOffset, int botOffset) { m_TopOffset = topOffset, m_BotOffset = botOffset; };
	void SetStartVec(GM2DVector sVec) { m_sVec = sVec; };
	void SetEndVec(GM2DVector eVec) { m_eVec = eVec; };

	NXBoxWallOpeningData* AddOpeningData(CString name, GM2DVector sVec, GM2DVector eVec, int level, int height, int extSize);
	vector<NXBoxWallOpeningData*> ma_Opening;

	double GetThick() { return m_Thick; };
	int GetTopOffset() { return m_TopOffset; };
	int GetBotOffset() { return m_BotOffset; };
	GM2DVector GetSVec() { return m_sVec; };
	GM2DVector GetEVec() { return m_eVec; };

protected:
	void DeleteOpeningDatas();

protected:
	double m_Thick;
	GM2DVector m_sVec, m_eVec;
	int m_TopOffset;
	int m_BotOffset;
};

class StructureModel_CLASS NXBoxWallHelper :public NXBoxHelper
{
public:
	NXBoxWallHelper(CogReader* pReader);
	~NXBoxWallHelper();

	MSCompoG* MakeData(NXBoxData* pData) override;
protected:
	void MakeWallOpenings(MSCompoG* pWallG);
	MSCompoM* FindOrCreateDefaultReinforceM();

	MSCompoM* CreateNewWallM(double dT, CString TheName);
	bool CheckCompoM(MSCompoM* pCompoM);
	virtual CString MakeDefaultName() override { return _T("DEFW1"); };
};

