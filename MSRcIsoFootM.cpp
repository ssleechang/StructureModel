#include "StdAfx.h"
#include "MSRcIsoFootM.h"
#include "GMLib/GM2DRectangle.h"
#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSRcIsoFootM, MSRcFaceMemberM, VERSIONABLE_SCHEMA | 1)
	MSRcIsoFootM::MSRcIsoFootM(void)
{
	InitValue();
}

MSRcIsoFootM::~MSRcIsoFootM(void)
{
}

void MSRcIsoFootM::InitValue()
{
	m_nType = msIsoFootM;
	m_Haunch = 0;
	m_nShape = 0;
	m_dDepth = 3000.;
	m_dWidth = 3000.;
	m_nIsoFootType = ISOFOOT_ISOFOOT;
}

void MSRcIsoFootM::CopyFromMe(MSCompoM* pSource)
{
	if(pSource==NULL)	return;
	MSRcIsoFootM* pSourceIsoFootM = (MSRcIsoFootM*)pSource;
	m_Haunch = pSourceIsoFootM->m_Haunch;
	m_nShape = pSourceIsoFootM->m_nShape;
	m_dDepth = pSourceIsoFootM->m_dDepth;
	m_dWidth = pSourceIsoFootM->m_dWidth;
	m_nIsoFootType = pSourceIsoFootM->m_nIsoFootType;
	MSRcFaceMemberM::CopyFromMe(pSource);
}

void MSRcIsoFootM::Dump( EFS::EFS_Buffer& buffer )
{
	MSRcFaceMemberM::Dump(buffer);
	buffer << m_nShape << m_Haunch << m_dDepth << m_dWidth << m_nIsoFootType;
}

void MSRcIsoFootM::Recovery( EFS::EFS_Buffer& buffer )
{
	MSRcFaceMemberM::Recovery(buffer);
	buffer >> m_nShape >> m_Haunch >> m_dDepth >> m_dWidth >> m_nIsoFootType;
}

void MSRcIsoFootM::Serialize(CArchive &ar)
{
	MSRcFaceMemberM::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_nShape;
		ar << m_Haunch;
		ar << m_dWidth;
		ar << m_dDepth;
		ar << m_nIsoFootType;
	}
	else
	{
		ar >> m_nShape;
		ar >> m_Haunch;
		ar >> m_dWidth;
		ar >> m_dDepth;
		if(MSVersionInfo::GetCurrentVersion() >= 20150515)
			ar >> m_nIsoFootType;
	}
}

void MSRcIsoFootM::GetSecPolyline( GM2DPolyline& TheGon )
{

	GM2DRectangle TheRect;

	double dXc1 = -m_dWidth/2;
	double dYc1 = -m_dDepth/2;
	double dXc2 = +m_dWidth/2;
	double dYc2 = +m_dDepth/2;
	GM2DVector* pVec1 = new GM2DVector(dXc1, dYc1);
	GM2DVector* pVec2 = new GM2DVector(dXc2, dYc1);
	GM2DVector* pVec3 = new GM2DVector(dXc2, dYc2);
	GM2DVector* pVec4 = new GM2DVector(dXc1, dYc2);
	GM2DVector* pVec5 = new GM2DVector(dXc1, dYc1);
	
	TheGon.AddPointAndBulge(pVec1);
	TheGon.AddPointAndBulge(pVec2);
	TheGon.AddPointAndBulge(pVec3);
	TheGon.AddPointAndBulge(pVec4);
	TheGon.AddPointAndBulge(pVec5);
	TheGon.MakeClosedPolyline(DTOL_GM);
	TheGon.MakeCCW();
}

