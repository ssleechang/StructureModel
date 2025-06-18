#pragma once
#include "NXBoxHelper.h"

class StructureModel_CLASS NXBoxPitData : public NXBoxData
{
public:
	NXBoxPitData();
	~NXBoxPitData();

	void SetPitThick(double thick) { m_PitThick = thick; };
	void SetPitDepth(double thick) { m_PitDepth = thick; };
	void SetWellThick(double thick) { m_WellThick = thick; };
	void SetPlaceWell(bool IsPlace) { m_IsPlaceWell = IsPlace; };

	void SetOutLine(GM2DPolyline* polyline) { m_OutLine = polyline; };
	void SetInnerLine(GM2DPolyline* polyline) { m_InnerLine = polyline; };
	void SetWellLine(GM2DPolyline* polyline) { m_WellLine = polyline; };

	double GetPitThick() { return m_PitThick; };
	double GetPitDepth() { return m_PitDepth; };
	double GetWellThick() { return m_WellThick; };
	bool GetIsPlaceWell() { return m_IsPlaceWell; };

	GM2DPolyline* GetOutLine() { return m_OutLine; };
	GM2DPolyline* GetInnerLine() { return m_InnerLine; };
	GM2DPolyline* GetWellLine() { return m_WellLine; };

protected:
	double m_PitThick;
	double m_PitDepth;
	double m_WellThick;
	bool m_IsPlaceWell;
	GM2DPolyline* m_OutLine;
	GM2DPolyline* m_InnerLine;
	GM2DPolyline* m_WellLine;
};

class StructureModel_CLASS NXBoxPitHelper :public NXBoxHelper
{
public:
	NXBoxPitHelper(CogReader* pReader);
	~NXBoxPitHelper();

	MSCompoG* MakeData(NXBoxData* pData) override;
protected:
	MSCompoM* CreateNewPitM(CString TheName);
	bool CheckCompoM(MSCompoM* pCompoM);
	virtual CString MakeDefaultName() override { return _T("DEFPT1"); };
};

