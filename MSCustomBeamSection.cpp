#include "stdafx.h"
#include "MSCustomBeamSection.h"

IMPLEMENT_SERIAL(MSCustomBeamSection, MSCustomLineSection, VERSIONABLE_SCHEMA | 1)

MSCustomBeamSection::MSCustomBeamSection(MSBaseBuilding* pBuilding) : MSCustomLineSection(pBuilding)
{
	m_eSectionType = eSectionType::CUSTOM_BEAM;
}

MSCustomBeamSection::~MSCustomBeamSection(void)
{

}

void MSCustomBeamSection::CopyFromMe(MSCustomBeamSection* pSource, bool bNewID /*= true*/)
{
	MSCustomLineSection::CopyFromMe(pSource);
	MSCustomBeamSection* pSourceM = (MSCustomBeamSection*)pSource;
}

double MSCustomBeamSection::GetFloorLevelOffSet()
{
	double dOffSet = 0;
	GM2DPolyline* pPoly = GetProfile();
	double dXMin, dYMin, dXMax, dYMax;
	pPoly->GetXYMinMax(dXMin, dYMin, dXMax, dYMax);
	dOffSet = dYMax;

	return dYMax;
}