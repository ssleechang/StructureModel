#pragma once
#include "NXBoxHelper.h"

class StructureModel_CLASS NXBoxBeamData : public NXBoxData
{
public:
	NXBoxBeamData();
	~NXBoxBeamData();

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

class StructureModel_CLASS NXBoxBeamHelper : public NXBoxHelper
{
public:
	NXBoxBeamHelper(CogReader* pReader);
	~NXBoxBeamHelper();

	MSCompoG* MakeData(NXBoxData* pData) override;
protected:
	virtual CString MakeDefaultName() override { return _T("DEFG1"); };
	MSCompoM* CreateNewBeamM(double dB, CString TheName);
	bool CheckCompoM(MSCompoM* pCompoM);
};

