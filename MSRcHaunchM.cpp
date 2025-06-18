#include "stdafx.h"
#include "MSRcHaunchM.h"


IMPLEMENT_SERIAL(MSRcHaunchM, MSCompoM, VERSIONABLE_SCHEMA | 1)
MSRcHaunchM::MSRcHaunchM()
{
	m_Degree = 45;
	m_Depth = 300;
}


MSRcHaunchM::~MSRcHaunchM()
{
}
void MSRcHaunchM::CopyFromMe(MSCompoM* pSource)
{
	if (pSource == NULL)	return;
	MSRcHaunchM* pSourceM = (MSRcHaunchM*)pSource;
	m_Degree = pSourceM->GetDegree();
	m_Depth = pSourceM->GetDepth();
	MSCompoM::CopyFromMe(pSource);
}

bool MSRcHaunchM::IsGeometryDifferent(MSCompoM* compoM)
{
	MSRcHaunchM* pUrCompoM = dynamic_cast<MSRcHaunchM*>(compoM);
	if (pUrCompoM != NULL)
	{
		if (fabs(m_Degree - pUrCompoM->GetDegree()) > DTOL1)	return true;
		if (fabs(m_Depth - pUrCompoM->GetDepth()) > DTOL1)	return true;
	}
	return MSCompoM::IsGeometryDifferent(compoM);
}

void MSRcHaunchM::GetSecPolyline(GM2DPolyline& APGon)
{
	GetSecPolyline(APGon, 0, 0);
}

void MSRcHaunchM::GetSecPolyline(GM2DPolyline& APGon, double dTopOffSet, double dBottomOffSet)
{
	vector<GM2DVector> vectors = MakeSectionShape(this, dTopOffSet, dBottomOffSet);
	for (int i = 0; i<int(vectors.size()); i++)
		APGon.AddPointAndBulge(new GM2DVector(vectors[i].m_X, vectors[i].m_Y));
	APGon.MakeClosedPolyline(DTOL_GM);
	APGon.MakeCCW();
}

vector<GM2DVector> MSRcHaunchM::MakeSectionShape(MSRcHaunchM* pRcHaunchM, double dTopOffSet, double dBottomOffSet)
{
	vector<GM2DVector> sections;

	double depth = pRcHaunchM->m_Depth + dTopOffSet - dBottomOffSet;
	if (depth > DTOL_GM)
	{
		double width = pRcHaunchM->GetCalcWidth(depth, pRcHaunchM->m_Degree);
		// xOffset 이 필요 없는거 같음 by 규완
		double xOffset = 0; //pRcHaunchM->GetCalcWidth(dTopOffSet - dBottomOffSet, pRcHaunchM->m_Degree);

		sections.push_back(GM2DVector(-width / 2 + xOffset / 2, dTopOffSet));
		sections.push_back(GM2DVector(-width / 2 + xOffset / 2, -depth + dTopOffSet));
		sections.push_back(GM2DVector(width / 2 + xOffset / 2, dTopOffSet));
	}
	return sections;
}

void MSRcHaunchM::Dump(EFS::EFS_Buffer& buffer)
{
	MSCompoM::Dump(buffer);
	buffer << m_Degree;
	buffer << m_Depth;
}

void MSRcHaunchM::Recovery(EFS::EFS_Buffer& buffer)
{
	MSCompoM::Recovery(buffer);
	buffer >> m_Degree;
	buffer >> m_Depth;
}

void MSRcHaunchM::Serialize(CArchive &ar)
{
	MSCompoM::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_Degree;
		ar << m_Depth;
	}
	else
	{
		ar >> m_Degree;
		ar >> m_Depth;
	}
}

double MSRcHaunchM::GetWidth()
{
	return GetCalcWidth();
}

double MSRcHaunchM::GetCalcWidth()
{
	double radian = (90-m_Degree)* Deg2Rad;
	return m_Depth * tan(radian);
}

double MSRcHaunchM::GetCalcWidth(double Depth, double Degree)
{
	double radian = (90 - Degree)* Deg2Rad;
	return Depth * tan(radian);
}