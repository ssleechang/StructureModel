#include "StdAfx.h"
#include "MSPolySection.h"

MSPolySection::MSPolySection(void)
{
}

MSPolySection::~MSPolySection(void)
{
}

void MSPolySection::GetSecPolygon( GMPolyline& APGon )
{
	APGon.DeleteVecArr();
	APGon = m_SecPGon;	
}

void MSPolySection::SetSecPolygon( GMPolyline& APGon )
{
	m_SecPGon.DeleteVecArr();
	m_SecPGon = APGon;
}