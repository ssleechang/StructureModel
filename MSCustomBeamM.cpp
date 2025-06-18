#include "stdafx.h"
#include "MSCustomBeamM.h"
#include "MSCustomLineSection.h"
#include "MSCustomBeamSection.h"
#include "MDCustomLineM.h"
#include "MSCustomPointBarUnit.h"
#include "MSCustomLineBarUnit.h"
#include "MDCustomPointUnitRebar.h"
#include "MDCustomLineUnitRebar.h"
#include "MDCustomSection.h"
#include "MDCustomBeamM.h"
#include "GMLib/GM2DLineVector.h"
#include "GMLib/AcisGeomHelper.h"

IMPLEMENT_SERIAL(MSCustomBeamM, MSCustomM, VERSIONABLE_SCHEMA | 1)

MSCustomBeamM::MSCustomBeamM()
{
	m_nType = MSCompoM::msCustomBeamM;
}


MSCustomBeamM::~MSCustomBeamM()
{

}

void MSCustomBeamM::Serialize(CArchive& ar)
{
	int nExterior = 0;
	int nInterior = 0;
	MSCustomM::Serialize(ar);
	if (ar.IsStoring())
	{
		nExterior = ma_LinkedExterior.size();
		ar << nExterior;
		for (int i = 0; i < nExterior; i++)
			ar << ma_LinkedExterior[i];

		nInterior = ma_LinkedInterior.size();
		ar << nInterior;
		for (int i = 0; i < nInterior; i++)
			ar << ma_LinkedInterior[i];
	}
	else
	{
		CString sData;
		ar >> nExterior;
		for (int i = 0; i < nExterior; i++)
		{
			ar >> sData;
			ma_LinkedExterior.push_back(sData);
		}
		ar >> nInterior;
		for (int i = 0; i < nInterior; i++)
		{
			ar >> sData;
			ma_LinkedInterior.push_back(sData);
		}
	}
}

void MSCustomBeamM::Dump(EFS::EFS_Buffer& buffer)
{
	MSCustomM::Dump(buffer);

	int nExterior = ma_LinkedExterior.size();
	buffer << nExterior;
	for (int i = 0; i < nExterior; i++)
		buffer << ma_LinkedExterior[i];

	int nInterior = ma_LinkedInterior.size();
	buffer << nInterior;
	for (int i = 0; i < nInterior; i++)
		buffer << ma_LinkedInterior[i];
}

void MSCustomBeamM::Recovery(EFS::EFS_Buffer& buffer)
{
	MSCustomM::Recovery(buffer);

	CString sData;
	int nExterior = 0;
	buffer >> nExterior;
	ma_LinkedExterior.clear();
	for (int i = 0; i < nExterior; i++)
	{
		buffer >> sData;
		ma_LinkedExterior.push_back(sData);
	}

	int nInterior = 0;
	buffer >> nInterior;
	ma_LinkedInterior.clear();
	for (int i = 0; i < nInterior; i++)
	{
		buffer >> sData;
		ma_LinkedInterior.push_back(sData);
	}
}

void MSCustomBeamM::CopyFromMe(MSCompoM* pSource)
{
	if (pSource == NULL)	return;
	MSCustomBeamM* pSourceBeamM = (MSCustomBeamM*)pSource;

	ma_LinkedExterior.resize((int)pSourceBeamM->ma_LinkedExterior.size());
	std::copy(pSourceBeamM->ma_LinkedExterior.begin(), pSourceBeamM->ma_LinkedExterior.end(), ma_LinkedExterior.begin());

	ma_LinkedInterior.resize((int)pSourceBeamM->ma_LinkedInterior.size());
	std::copy(pSourceBeamM->ma_LinkedInterior.begin(), pSourceBeamM->ma_LinkedInterior.end(), ma_LinkedInterior.begin());

	MSCustomM::CopyFromMe(pSource);
}

void MSCustomBeamM::MakeDefaultRebars()
{
	MakeDefaultRebars(GetMDCustomSection(Beam_Small));
	MakeDefaultRebars(GetMDCustomSection(Beam_Mid));
	MakeDefaultRebars(GetMDCustomSection(Beam_Large));
}

void MSCustomBeamM::MakeDefaultRebars(MDCustomSection* pMDCustomSection)
{
	MSCustomLineSection* pSection = dynamic_cast<MSCustomLineSection*>(GetMSCustomSection());
	if (pSection == nullptr)
		return;

	if (pMDCustomSection == nullptr)
		return;
	
	pMDCustomSection->DeleteCustomMDPointUnitRebars();
	pMDCustomSection->DeleteCustomMDLineUnitRebars();

	vector<MSCustomPointBarUnit*> pointUnitList = pSection->GetMSPointUnits();
	for (int i = 0; i < pointUnitList.size(); i++)
		pMDCustomSection->MakeDefaultMDPointUnitRebar(pointUnitList[i]);

	vector<MSCustomLineBarUnit*> lineUnitList = pSection->GetMSLineUnits();
	for (int i = 0; i < lineUnitList.size(); i++)
		pMDCustomSection->MakeDefaultMDLineUnitRebar(lineUnitList[i]);
}

void MSCustomBeamM::UpdateCustomRebarsID(map<long, long> oldUnitIDtoNewUnitID)
{
	MSCustomLineSection* pSection = dynamic_cast<MSCustomLineSection*>(GetMSCustomSection());

	MDCustomSection* pMDCustomSection = nullptr;
	pMDCustomSection = GetMDCustomSection(Beam_Small);
	if (pMDCustomSection != nullptr)
		pMDCustomSection->UpdateCustomRebarsID(oldUnitIDtoNewUnitID);

	pMDCustomSection = GetMDCustomSection(Beam_Mid);
	if (pMDCustomSection != nullptr)
		pMDCustomSection->UpdateCustomRebarsID(oldUnitIDtoNewUnitID);

	pMDCustomSection = GetMDCustomSection(Beam_Large);
	if (pMDCustomSection != nullptr)
		pMDCustomSection->UpdateCustomRebarsID(oldUnitIDtoNewUnitID);
}

void MSCustomBeamM::UpdateCustomCompoM()
{
	MSCustomLineSection* pSection = dynamic_cast<MSCustomLineSection*>(GetMSCustomSection());

	MDCustomSection* pMDCustomSection = nullptr;
	pMDCustomSection = GetMDCustomSection(Beam_Small);
	if (pMDCustomSection != nullptr)
		pMDCustomSection->UpdateCustomRebars(pSection);

	pMDCustomSection = GetMDCustomSection(Beam_Mid);
	if (pMDCustomSection != nullptr)
		pMDCustomSection->UpdateCustomRebars(pSection);

	pMDCustomSection = GetMDCustomSection(Beam_Large);
	if (pMDCustomSection != nullptr)
		pMDCustomSection->UpdateCustomRebars(pSection);
}

MDCustomBeamM* MSCustomBeamM::GetMDCustomBeamM()
{
	MDMember* pMDMember = GetRebarM();
	if (pMDMember == nullptr)
		return nullptr;
	return  dynamic_cast<MDCustomBeamM*>(pMDMember);
}

vector<CString> MSCustomBeamM::GetLinkedExteriors()
{
	return ma_LinkedExterior;
}

vector<CString> MSCustomBeamM::GetLinkedInteriors()
{
	return ma_LinkedInterior;
}

void MSCustomBeamM::SetLinkedExteriors(vector<CString>& linkeds)
{
	ma_LinkedExterior.resize((int)linkeds.size());
	std::copy(linkeds.begin(), linkeds.end(), ma_LinkedExterior.begin());
}

void MSCustomBeamM::SetLinkedInteriors(vector<CString>& linkeds)
{
	ma_LinkedInterior.resize((int)linkeds.size());
	std::copy(linkeds.begin(), linkeds.end(), ma_LinkedInterior.begin());
}

double MSCustomBeamM::GetSectionFloorLevelOffSet()
{
	MSCustomBeamSection* pSection = dynamic_cast<MSCustomBeamSection*>(GetMSCustomSection());
	if (pSection == nullptr)
		return 0;
	
	return pSection->GetFloorLevelOffSet();
}

// dFloorLevel은 0을 기준으로 생각한다.
vector<GM2DPolyline*> MSCustomBeamM::GetSplitSecPolylinebyFloorLevel(double dFloorLevel, bool IsUp)
{
	vector<GM2DPolyline*> DividePolylineList;

	GM2DPolyline SectionPoly;
	GetSecPolyline(SectionPoly);

	double dTmpDist = 10;
	double dMinDivideDist = DTOL_MODELING;

	double dMinX, dMinY, dMaxX, dMaxY;
	SectionPoly.GetXYMinMax(dMinX, dMinY, dMaxX, dMaxY);

	// SectionPoly의 높이 보다 작거나 크면 기존 SectionPoly을 리턴한다.
	if (dMinY + dMinDivideDist > dFloorLevel || dMaxY - dMinDivideDist < dFloorLevel)
	{
		DividePolylineList.push_back(SectionPoly.Copy());
		return DividePolylineList;
	}

	GM2DLineVector FloorLevelLine;
	FloorLevelLine.m_P0.SetXY(dMinX - dTmpDist, dFloorLevel);
	FloorLevelLine.m_P1.SetXY(dMaxX + dTmpDist, dFloorLevel);

	vector<GM2DCurve*> CuverList;
	CuverList.push_back(&FloorLevelLine);

	AcisGeomHelper::DividePolyline(CuverList, &SectionPoly, DividePolylineList);
	double dSectionFloorLevelOffSet = GetSectionFloorLevelOffSet();

	// 나뉘어진것이 없으면 기존 SectionPoly을 리턴한다.
	if (DividePolylineList.size() == 0 )
	{
		DividePolylineList.push_back(SectionPoly.Copy());
		return DividePolylineList;
	}

	double dTol = DTOL_GM;
	double dCheckDist = dTol * 2;
	if (IsUp == false)
		dCheckDist = dCheckDist*-1;

	GM2DLineVector CheckLine;
	CheckLine.m_P0.SetXY(dMinX - dTmpDist, dFloorLevel + dCheckDist);
	CheckLine.m_P1.SetXY(dMaxX + dTmpDist, dFloorLevel + dCheckDist);

	double dCheckY = dFloorLevel + dCheckDist;

	// 나눠진 SectionPoly의 MinY, MaxY값 사이에 dCheckY이 존재하는 폴리라인만 리턴한다.
	for (int i = DividePolylineList.size() - 1; i >= 0; i--)
	{
		GM2DPolyline* pDividePoly = DividePolylineList[i];
		double dDivMinX, dDivMinY, dDivMaxX, dDivMaxY;
		pDividePoly->GetXYMinMax(dDivMinX, dDivMinY, dDivMaxX, dDivMaxY);

		if (dDivMinY > dCheckY || dDivMaxY < dCheckY)
		{
			delete pDividePoly;
			DividePolylineList.erase(DividePolylineList.begin() + i);
		}
	}

	return DividePolylineList;
}

