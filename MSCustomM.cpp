#include "stdafx.h"
#include "MSCustomM.h"
#include "MSRebarBuilding.h"
#include "MSCustomSection.h"
#include "MSCustomPointBarUnit.h"
#include "MSCustomLineBarUnit.h"
#include "MDCustomSection.h"
#include "MDCustomPointUnitRebar.h"
#include "MDCustomLineUnitRebar.h"
#include "MDCustomM.h"

IMPLEMENT_SERIAL(MSCustomM, MSCompoM, VERSIONABLE_SCHEMA | 1)

MSCustomM::MSCustomM(void)
{
	mp_Section = nullptr;
	m_nSectionID = 0;
}

MSCustomM::~MSCustomM(void)
{
// mp_Section 은 사용자 정의 단면에서 삭제 한다.
// 	if (mp_Section != nullptr)
// 		delete mp_Section;
}

void MSCustomM::CopyFromMe(MSCompoM* pSource)
{
	if (pSource == NULL)	return;
	MSCustomM* pSourceM = (MSCustomM*)pSource;

	mp_Section = pSourceM->GetMSCustomSection();
	m_nSectionID = pSourceM->m_nSectionID;
	MSCompoM::CopyFromMe(pSource);
}

void MSCustomM::Dump(EFS::EFS_Buffer& buffer)
{
	MSCompoM::Dump(buffer);

//	mp_Section->Dump(buffer);
	buffer << m_nSectionID;
}

void MSCustomM::Recovery(EFS::EFS_Buffer& buffer)
{
	MSCompoM::Recovery(buffer);

//	mp_Section->Recovery(buffer);
	buffer >> m_nSectionID;
}

void MSCustomM::Serialize(CArchive &ar)
{
	MSBaseBuilding* pBuilding = GetBuilding();
	MSCompoM::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_nSectionID;
	}
	else
	{
		ar >> m_nSectionID;
		mp_Section = pBuilding->GetCustomSection(m_nSectionID);
	}
}

bool MSCustomM::IsGeometryDifferent(MSCompoM* compoM)
{
	if (MSCompoM::IsGeometryDifferent(compoM))
		return true;

	MSCustomM* urCustomM = dynamic_cast<MSCustomM*>(compoM);
	if (urCustomM == nullptr)	return false;

	if (m_nSectionID != urCustomM->GetSectionID())	return true;
	return false;
}
void MSCustomM::GetSecPolyline(GM2DPolyline& APGon)
{
	if (mp_Section == nullptr)	return;
	mp_Section->GetSecPolyline(APGon);
}

void MSCustomM::SetMSCustomSection(MSCustomSection* customSection)
{
	if (customSection == nullptr)
		return;
	m_nSectionID = customSection->GetID();
	mp_Section = customSection;
}

MSCustomSection* MSCustomM::GetMSCustomSection()
{
	return mp_Section;
}

MDCustomSection* MSCustomM::GetMDCustomSection(eCustomM_SectionType eType)
{
	MDMember* pMDMember = GetRebarM();
	if (pMDMember == nullptr)
		return nullptr;
	MDCustomM* pMDCustomM = dynamic_cast<MDCustomM*>(pMDMember);

	if (pMDCustomM == nullptr)
		return nullptr;
	
	return pMDCustomM->GetMDCustomSection(eType);
}

vector<GM2DVector> MSCustomM::CalculatorPointBarLocation(int msPointUnitID, eCustomM_SectionType eType)
{
	vector<GM2DVector> locArr;
	MSCustomLineSection* pSection = dynamic_cast<MSCustomLineSection*>(GetMSCustomSection());
	if (pSection == nullptr)	return locArr;

	MSCustomPointBarUnit* pUnit = pSection->GetMSPointUnit(msPointUnitID);
	if (pUnit == nullptr)	return locArr;

	MDCustomSection* pMDCustomSection = nullptr;
	MDCustomPointUnitRebar* pPointRebar = nullptr;

	pMDCustomSection = GetMDCustomSection(eType);
	if (pMDCustomSection != nullptr)
	{
		pPointRebar = pMDCustomSection->GetMDPointUnitRebar(msPointUnitID);
		if (pPointRebar != nullptr)
			return pPointRebar->CalculatorPointBarLocation(pUnit->Get2DLineVector(), pUnit->GetSpacingType());
	}

	return locArr;
}

vector<GM2DPolyline*> MSCustomM::CalculatorLineBarLocation(int msLineUnitID, eCustomM_SectionType eType)
{
	vector<GM2DPolyline*> locArr;
	MSCustomLineSection* pSection = dynamic_cast<MSCustomLineSection*>(GetMSCustomSection());
	if (pSection == nullptr)	return locArr;

	MSCustomLineBarUnit* pUnit = pSection->GetMSLineUnit(msLineUnitID);
	if (pUnit == nullptr)	return locArr;

	MDCustomSection* pMDCustomSection = nullptr;
	MDCustomLineUnitRebar* pLineRebar = nullptr;

	pMDCustomSection = GetMDCustomSection(eType);
	if (pMDCustomSection != nullptr)
	{
		pLineRebar = pMDCustomSection->GetMDLineUnitRebar(msLineUnitID);
		if (pLineRebar != nullptr)
			return pLineRebar->CalculatorLineBarLocation(pUnit);
	}

	return locArr;
}

MDCustomPointUnitRebar* MSCustomM::GetMDPointUnitRebar(int unitID, eCustomM_SectionType eType)
{
	MDCustomSection* pMDCustomSection = GetMDCustomSection(eType);
	if (pMDCustomSection != nullptr)
		return pMDCustomSection->GetMDPointUnitRebar(unitID);

	return nullptr;
}

MDCustomLineUnitRebar* MSCustomM::GetMDLineUnitRebar(int unitID, eCustomM_SectionType eType)
{
	MDCustomSection* pMDCustomSection = GetMDCustomSection(eType);
	if (pMDCustomSection != nullptr)
		return pMDCustomSection->GetMDLineUnitRebar(unitID);

	return nullptr;
}

bool MSCustomM::IsCustomColumnSection()
{
	if (mp_Section->GetSectionType() == MSCustomSection::CUSTOM_COLUMN)
		return true;
	return false;
}

bool MSCustomM::IsCustomWallColumnSection()
{
	if (mp_Section->GetSectionType() == MSCustomSection::CUSTOM_WALLCOLUMN)
		return true;
	return false;
}

bool MSCustomM::IsCustomBeamSection()
{
	if (mp_Section->GetSectionType() == MSCustomSection::CUSTOM_BEAM)
		return true;
	return false;
}

double MSCustomM::GetWidth()
{
	if (mp_Section == nullptr || mp_Section->GetProfile() == nullptr)
		return 0;

	GM2DPolyline* pPoly = mp_Section->GetProfile();
	double dXMin = 0, dYMin = 0, dXMax = 0, dYMax = 0;
	pPoly->GetXYMinMax(dXMin, dYMin, dXMax, dYMax);
	return fabs(dXMax - dXMin);
}

double MSCustomM::GetDepth()
{
	if (mp_Section == nullptr || mp_Section->GetProfile() == nullptr)
		return 0;

	GM2DPolyline* pPoly = mp_Section->GetProfile();
	double dXMin = 0, dYMin = 0, dXMax = 0, dYMax = 0;
	pPoly->GetXYMinMax(dXMin, dYMin, dXMax, dYMax);
	return fabs(dYMax - dYMin);
}

double MSCustomM::GetFloorLevelOffSet()
{
	if (mp_Section == nullptr || mp_Section->GetProfile() == nullptr)
		return 0;

	return mp_Section->GetFloorLevelOffSet();
}

long MSCustomM::GetCustomSectionID()
{
	if (mp_Section == nullptr)
		return 0;

	return mp_Section->GetID();
}