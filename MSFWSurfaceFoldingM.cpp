#include "stdafx.h"
#include "GMLib/MSVersionInfo.h"
#include "MSFWSurfaceFoldingM.h"


IMPLEMENT_SERIAL(MSFWSurfaceFoldingM, MSCompoM, VERSIONABLE_SCHEMA | 1)
MSFWSurfaceFoldingM::MSFWSurfaceFoldingM()
{
	m_nType = MSCompoM::msFWSurfaceFoldingM;
	
	m_TriangleDegree = 90;
	m_L1 = 150;
	m_L2 = 150;
	m_pProfile = new GM2DPolyline();
	m_eSurfaceFoldingType = eSurfaceFoldingMType::Triangle;
	MakeProfile();
}

MSFWSurfaceFoldingM::~MSFWSurfaceFoldingM()
{
	delete m_pProfile;
}
void MSFWSurfaceFoldingM::CopyFromMe(MSCompoM* pSource)
{
	if (pSource == NULL)	return;

	MSFWSurfaceFoldingM* pSourceM = (MSFWSurfaceFoldingM*)pSource;
	m_TriangleDegree = pSourceM->GetTriangleDegree();
	m_L1 = pSourceM->GetL1();
	m_L2 = pSourceM->GetL2();
	m_pProfile->CopyFrom(pSourceM->GetProfile());
	m_eSurfaceFoldingType = pSourceM->m_eSurfaceFoldingType;
	MSCompoM::CopyFromMe(pSource);
	//MakeProfile();
}

void MSFWSurfaceFoldingM::Dump(EFS::EFS_Buffer& buffer)
{
	buffer << m_TriangleDegree << m_L1 << m_L2;
	MSCompoM::Dump(buffer);
	m_pProfile->Dump(buffer);

	buffer << m_eSurfaceFoldingType;
}

void MSFWSurfaceFoldingM::Recovery(EFS::EFS_Buffer& buffer)
{
	buffer >> m_TriangleDegree >> m_L1 >> m_L2;
	MSCompoM::Recovery(buffer);
	m_pProfile->Recovery(buffer);

	buffer >> m_eSurfaceFoldingType;
}

void MSFWSurfaceFoldingM::Serialize(CArchive &ar)
{
	MSCompoM::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_TriangleDegree;
		ar << m_L1;
		ar << m_L2;
		GM2DPolyline::SerializeWithNullFlag(ar, m_pProfile);

		ar << m_eSurfaceFoldingType;
	}
	else
	{
		ar >> m_TriangleDegree;
		ar >> m_L1;
		ar >> m_L2;
		GM2DPolyline::SerializeWithNullFlag(ar, m_pProfile);
		//MakeProfile();
		
		m_pProfile->MakeCCW();

		if (MSVersionInfo::GetCurrentVersion() >= 20230217)
		{
			int nType = 0;
			ar >> nType;
			m_eSurfaceFoldingType = (eSurfaceFoldingMType)nType;
		}
	}
}

void MSFWSurfaceFoldingM::SetTriangleDegree(double param)
{
	m_TriangleDegree = param;
	MakeProfile();
}

void MSFWSurfaceFoldingM::SetL1(double param)
{
	m_L1 = param;
	MakeProfile();
}
void MSFWSurfaceFoldingM::SetL2(double param)
{
	m_L2 = param;
	MakeProfile();
}

void MSFWSurfaceFoldingM::MakeProfile()
{
	if (m_eSurfaceFoldingType == eSurfaceFoldingMType::PolyLine)
		return;

	m_pProfile->ResetPolyline();

	GM2DVector* tri0 = new GM2DVector(0, 0);
	GM2DVector* tri1 = new GM2DVector(0, -m_L1);
	GM2DVector* tri2 = new GM2DVector(m_L2, 0);

	double tolerance = 1.0;
	m_pProfile->AddPointAndBulge(tri0);
	m_pProfile->AddPointAndBulge(tri1);
	m_pProfile->AddPointAndBulge(tri2);
	m_pProfile->AddPointAndBulge((GM2DVector*)tri0->Copy());

	m_pProfile->MakeCCW();
}

GM2DPolyline* MSFWSurfaceFoldingM::GetProfile()
{
	return m_pProfile; 
}

void MSFWSurfaceFoldingM::SetProfile(GM2DPolyline* pProfile)
{
	m_pProfile->CopyFrom(pProfile);

	m_eSurfaceFoldingType = eSurfaceFoldingMType::PolyLine;
}

CString MSFWSurfaceFoldingM::ToShapeName()
{
	return LocalDic::GetLocal(_T("면접기"));
}

CString MSFWSurfaceFoldingM::GetShapeName()
{
	return ToShapeName();
}

void MSFWSurfaceFoldingM::GetSecPolyline(GM2DPolyline& APGon)
{
	APGon.CopyFrom(m_pProfile);
}