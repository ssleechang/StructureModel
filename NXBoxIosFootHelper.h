#pragma once
#include "NXBoxHelper.h"

class StructureModel_CLASS NXBoxIsoFootData : public NXBoxData
{
public:
	NXBoxIsoFootData();
	~NXBoxIsoFootData();

	void SetWidth(double width) { m_Width = width; };
	void SetDepth(double depth) { m_Depth = depth; };
	void SetOffset(int offset)		 { m_Offset = offset; };
	void SetCenterPoint(GM2DVector center) { m_Center = center; };
	void SetRotate(double rotate) { m_Rotate = rotate; };

	double GetWidth() { return m_Width; };
	double GetDepth() { return m_Depth; };
	int GetOffset() { return m_Offset; };
	GM2DVector GetCenterPoint() { return m_Center; };
	double GetRotate() { return m_Rotate; };

protected:
	double m_Width;
	double m_Depth;
	GM2DVector m_Center;
	int m_Offset;
	double m_Rotate;
};
class StructureModel_CLASS NXBoxIosFootHelper :public NXBoxHelper
{
public:
	NXBoxIosFootHelper(CogReader* pReader);
	~NXBoxIosFootHelper();

	MSCompoG* MakeData(NXBoxData* pData) override;
protected:
	MSCompoM* CreateNewIsoFootM(double dB, double dH, long nShape, CString TheName);
	bool CheckCompoM(MSCompoM* pCompoM);
	virtual CString MakeDefaultName() override { return _T("DEFF1"); };
};

