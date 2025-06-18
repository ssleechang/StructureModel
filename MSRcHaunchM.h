#pragma once
#include "MSCompoM.h"
class StructureModel_CLASS MSRcHaunchM :public MSCompoM
{
public:
	MSRcHaunchM();
	~MSRcHaunchM();

	void Serialize(CArchive& ar);
	DECLARE_SERIAL(MSRcHaunchM)
	virtual void CopyFromMe(MSCompoM* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual bool IsGeometryDifferent(MSCompoM* compoM);

	virtual void GetSecPolyline(GM2DPolyline& APGon);
	void GetSecPolyline(GM2DPolyline& APGon, double dTopOffSet, double dBottomOffSet);

private:
	static vector<GM2DVector> MakeSectionShape(MSRcHaunchM* pRcHaunchM, double dTopOffSet, double dBottomOffSet);
public:

	double GetDegree() { return m_Degree; };
	double GetDepth() { return m_Depth; };
	virtual double GetWidth() override;

	void SetDegree(double degree) { m_Degree = degree; };
	void SetDepth(double depth) { m_Depth = depth; };

	static double GetCalcWidth(double Depth, double Degree);

private:
	double GetCalcWidth();
private:
	double m_Degree;
	double m_Depth;
};

