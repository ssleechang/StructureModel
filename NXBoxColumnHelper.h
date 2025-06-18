#pragma once
#include "NXBoxHelper.h"

class StructureModel_CLASS NXBoxColumnData : public NXBoxData
{
public:
	NXBoxColumnData();
	~NXBoxColumnData();

//	enum SECT_SHAPE { RECT = 1, CIRCLE, POLYGON, POLYGONINPLAN };
	void SetShape(int shape) { m_Shape = shape; };
	void SetWidth(double width) { m_Width = width; };
	void SetDepth(double depth) { m_Depth = depth; };
	void SetRadius(double radius) { m_Width = radius * 2, m_Depth = 0; };
	void SetOffset(int topOffset, int botOffset) { m_TopOffset = topOffset, m_BotOffset = botOffset; };
	void SetCenterPoint(GM2DVector center) { m_Center = center; };
	void SetRotate(double rotate) { m_Rotate = rotate; };

	int GetShape() { return m_Shape; };
	double GetWidth() { return m_Width; };
	double GetDepth() { return m_Depth; };
	double GetRadius() { return m_Width / 2.0; };
	int GetTopOffset(){ return m_TopOffset; };
	int GetBotOffset() { return m_BotOffset; }
	GM2DVector GetCenterPoint()	{ return m_Center; };
	double GetRotate() { return m_Rotate; };

protected:
	int m_Shape;
	double m_Width;
	double m_Depth;
	GM2DVector m_Center;
	int m_TopOffset;
	int m_BotOffset;
	double m_Rotate;
};

class StructureModel_CLASS NXBoxColumnHelper : public NXBoxHelper
{
public:
	NXBoxColumnHelper(CogReader* pReader);
	~NXBoxColumnHelper();

	MSCompoG* MakeData(NXBoxData* pData) override;
protected:
	MSCompoM* CreateNewColumnM(double dB, double dH, long nShape, CString TheName);
	bool CheckCompoM(MSCompoM* pCompoM);
	virtual CString MakeDefaultName() override { return _T("DEFC1"); };
};

