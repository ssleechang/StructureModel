#pragma once
#include "MSCompoM.h"
#include "StructureModelEnum.h"

class StructureModel_CLASS MSFWSurfaceFoldingM :public MSCompoM
{
public:
	MSFWSurfaceFoldingM();
	~MSFWSurfaceFoldingM();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSFWSurfaceFoldingM)
	virtual void CopyFromMe(MSCompoM* pSource);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	static CString ToShapeName();
	virtual CString GetShapeName();

	void MakeProfile();
	GM2DPolyline* GetProfile();
	void SetProfile(GM2DPolyline* pProfile);
	
	virtual bool IsPolygonSection() { return true; };
	virtual void GetSecPolyline(GM2DPolyline& APGon);

	eSurfaceFoldingMType GetSurfaceFoldingMType() { return m_eSurfaceFoldingType; };
	virtual double GetTriangleDegree() { return m_TriangleDegree; };
	virtual double GetL1() { return m_L1; };
	virtual double GetL2() { return m_L2; };

	void SetSurfaceFoldingMType(eSurfaceFoldingMType type) { m_eSurfaceFoldingType = type; };
	virtual void SetTriangleDegree(double param);
	virtual void SetL1(double param);
	virtual void SetL2(double param);


protected:
	GM2DPolyline* m_pProfile;
	eSurfaceFoldingMType m_eSurfaceFoldingType;
	double m_TriangleDegree;
	double m_L1;
	double m_L2;
};

