#include "StdAfx.h"
#include "MSRcHandRailM.h"


IMPLEMENT_SERIAL(MSRcHandRailM, MSRcLineMemberM, VERSIONABLE_SCHEMA | 1)
MSRcHandRailM::MSRcHandRailM(void)
{
	m_nType = msHandRailM;
	//벽 두께 m_dWidth
	m_dWidth = 300;

	m_nHandRailType = 0;//0:상부벽만 있음, 1:하부벽도 있음.
	m_dTopDepth = 500;//상부벽 높이
	m_dBotDepth = 500;//하부벽 높이

	m_nParapetType = 0;//0:없음, 1:Defalt parapet
	m_dParapetLengthA = 300;
	m_dParapetLengthB = 300;
	m_dParapetLengthC = 300;
	m_dParapetLengthD = 300;


}

MSRcHandRailM::~MSRcHandRailM(void)
{
}

void MSRcHandRailM::CopyFromMe(MSCompoM* pSource)
{
	if(pSource==NULL)	return;
	MSRcHandRailM* pSourceHandRailM = (MSRcHandRailM*)pSource;

	m_nHandRailType = pSourceHandRailM->m_nHandRailType;
	m_dTopDepth = pSourceHandRailM->m_dTopDepth;
	m_dBotDepth = pSourceHandRailM->m_dBotDepth;

	m_nParapetType = pSourceHandRailM->m_nParapetType;
	m_dParapetLengthA = pSourceHandRailM->m_dParapetLengthA;
	m_dParapetLengthB = pSourceHandRailM->m_dParapetLengthB;
	m_dParapetLengthC = pSourceHandRailM->m_dParapetLengthC;
	m_dParapetLengthD = pSourceHandRailM->m_dParapetLengthD;

	MSRcLineMemberM::CopyFromMe(pSource);
}

void MSRcHandRailM::Dump( EFS::EFS_Buffer& buffer )
{
	MSRcLineMemberM::Dump(buffer);
	buffer << m_nHandRailType << m_dTopDepth << m_dBotDepth << m_nParapetType << m_dParapetLengthA << m_dParapetLengthB << m_dParapetLengthC << m_dParapetLengthD;
}

void MSRcHandRailM::Recovery( EFS::EFS_Buffer& buffer )
{
	MSRcLineMemberM::Recovery(buffer);
	buffer >> m_nHandRailType >> m_dTopDepth >> m_dBotDepth >> m_nParapetType >> m_dParapetLengthA >> m_dParapetLengthB >> m_dParapetLengthC >> m_dParapetLengthD;
}

void MSRcHandRailM::Serialize(CArchive &ar)
{
	MSRcLineMemberM::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_nHandRailType;//0:상부벽만 있음, 1:하부벽도 있음.
		ar << m_dTopDepth;//상부벽 높이
		ar << m_dBotDepth;//하부벽 높이
		ar << m_nParapetType;//0:없음, 1:Defalt parapet
		ar << m_dParapetLengthA;
		ar << m_dParapetLengthB;
		ar << m_dParapetLengthC;
		ar << m_dParapetLengthD;
	}
	else
	{
		ar >> m_nHandRailType;//0:상부벽만 있음, 1:하부벽도 있음.
		ar >> m_dTopDepth;//상부벽 높이
		ar >> m_dBotDepth;//하부벽 높이
		ar >> m_nParapetType;//0:없음, 1:Defalt parapet
		ar >> m_dParapetLengthA;
		ar >> m_dParapetLengthB;
		ar >> m_dParapetLengthC;
		ar >> m_dParapetLengthD;
	}
}

bool MSRcHandRailM::IsGeometryDifferent(MSCompoM* compoM)
{
	if (MSCompoM::IsGeometryDifferent(compoM))
		return true;

	MSRcHandRailM* other = dynamic_cast<MSRcHandRailM*>(compoM);
	if (other == NULL) return true;

	if (m_nParapetType != other->m_nParapetType) return true;
	else if (m_dParapetLengthA != other->m_dParapetLengthA) return true;
	else if (m_dParapetLengthB != other->m_dParapetLengthB) return true;
	else if (m_dParapetLengthC != other->m_dParapetLengthC) return true;
	else if (m_dParapetLengthD != other->m_dParapetLengthD) return true;

	if (m_dTopDepth != other->m_dTopDepth) return true;
	if (m_dBotDepth != other->m_dBotDepth) return true;

	return false;
}

void MSRcHandRailM::GetSecPolyline(GM2DPolyline& APGon)
{
	vector<GM2DVector> vectors = GetSectionShape();
	for(int i=0; i<int(vectors.size()); i++)
		APGon.AddPointAndBulge(new GM2DVector(vectors[i].m_X, vectors[i].m_Y));
	APGon.MakeClosedPolyline(DTOL_GM);
	APGon.MakeCCW();
}

vector<GM2DVector> MSRcHandRailM::GetSectionShape()
{
	return MSRcHandRailM::MakeSectionShape(this, 0, 0);
}

vector<GM2DVector> MSRcHandRailM::MakeSectionShape(MSRcHandRailM* pRcHandRailM, double dTopOffSet, double dBottomOffSet)
{
	vector<GM2DVector> boundary;
	double dParapet_depth = 50;
	double dTopDepth = pRcHandRailM->m_dTopDepth + dTopOffSet;
	double dBotDepth = pRcHandRailM->m_dBotDepth + dBottomOffSet;
	if (dTopDepth <= 0) dTopDepth = 1.0;//minimum value를 준다.
	if (dBotDepth <= 0) dBotDepth = 1.0;//minimum value를 준다.

	boundary.push_back(GM2DVector(-pRcHandRailM->m_dWidth / 2.0, 0));
	if (pRcHandRailM->m_nParapetType == 0)//Parapet없음
	{
		boundary.push_back(GM2DVector(-pRcHandRailM->m_dWidth / 2.0, dTopDepth));
	}
	else
	{
		boundary.push_back(GM2DVector(-pRcHandRailM->m_dWidth / 2.0, dTopDepth - dParapet_depth));
		boundary.push_back(GM2DVector(-pRcHandRailM->m_dWidth / 2.0 - pRcHandRailM->m_dParapetLengthA, dTopDepth - dParapet_depth));
		boundary.push_back(GM2DVector(-pRcHandRailM->m_dWidth / 2.0 - pRcHandRailM->m_dParapetLengthA, dTopDepth));
	}

	boundary.push_back(GM2DVector(pRcHandRailM->m_dWidth / 2.0, dTopDepth));
	boundary.push_back(GM2DVector(pRcHandRailM->m_dWidth / 2.0, 0));

	if (pRcHandRailM->m_nHandRailType == 0)//상부벽만 있음
	{
		return boundary;
	}
	else //하부 추가
	{
		boundary.push_back(GM2DVector(pRcHandRailM->m_dWidth / 2.0, -dBotDepth));
		boundary.push_back(GM2DVector(-pRcHandRailM->m_dWidth / 2.0, -dBotDepth));
	}

	return boundary;
}

double MSRcHandRailM::GetParapetWidth()
{
	if(m_nParapetType == 0)
		return 0;
	return m_dParapetLengthA;
}

double MSRcHandRailM::GetDepth()
{
	return m_dTopDepth + m_dBotDepth;
}

double MSRcHandRailM::GetBotLevel()
{
	return m_dBotDepth; 
}; // by mylee - GetLevel() 대체 필요

double MSRcHandRailM::GetTopLevel()
{
	return m_dTopDepth; 
}; // by mylee - GetDepth() 대체 필요

void MSRcHandRailM::SetBotLevel(double val)
{ 
	m_dBotDepth = val;
}; // by mylee

void MSRcHandRailM::SetTopLevel(double val)
{ 
	m_dTopDepth = val;
}; // by mylee

void MSRcHandRailM::SetThick(double val)
{
	m_dWidth = val;
}; // by mylee