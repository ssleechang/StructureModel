#include "stdafx.h"
#include "MDCustomSection.h"

#include "MDCustomPointUnitRebar.h"
#include "MDCustomLineUnitRebar.h"
#include "MSCustomSection.h"
#include "MSCustomPointBarUnit.h"
#include "MSCustomLineBarUnit.h"
#include "MSCustomPointBarUnit.h"
#include "..\GMLib\MSVersionInfo.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////     MDCustomSection //////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(MDCustomSection, MDMember, VERSIONABLE_SCHEMA | 1)
MDCustomSection::MDCustomSection()
{
}


MDCustomSection::~MDCustomSection()
{
	DeleteCustomMDPointUnitRebars();
	DeleteCustomMDLineUnitRebars();
	DeleteSpecificTieBars();
}

void MDCustomSection::DeleteCustomMDPointUnitRebars()
{
	for (int i = 0; i < ma_PointUnit.size(); i++)
		delete ma_PointUnit[i];
	ma_PointUnit.clear();
}

void MDCustomSection::DeleteCustomMDLineUnitRebars()
{
	for (int i = 0; i < ma_LineUnit.size(); i++)
		delete ma_LineUnit[i];
	ma_LineUnit.clear();
}

void MDCustomSection::Serialize(CArchive& ar)
{
	MDMember::Serialize(ar);
	if (ar.IsStoring())
	{
		int count = ma_PointUnit.size();
		ar << count;
		for (int i = 0; i < count; i++)
			ma_PointUnit[i]->Serialize(ar);

		count = ma_LineUnit.size();
		ar << count;
		for (int i = 0; i < count; i++)
			ma_LineUnit[i]->Serialize(ar);

		int nSTieBar = ma_SpecificTieBar.size();
		ar << nSTieBar;

		for (int i = 0; i < nSTieBar; i++)
		{
			MSSpecificTieBar* pTieBar = ma_SpecificTieBar[i];
			pTieBar->Serialize(ar);
		}
	}
	else
	{
		int count = 0;
		ar >> count;

		for (int i = 0; i < count; i++)
		{
			MDCustomPointUnitRebar* pUnit = new MDCustomPointUnitRebar;
			pUnit->SetMDMemberM(this);
			pUnit->Serialize(ar);
			ma_PointUnit.push_back(pUnit);
		}

		ar >> count;
		for (int i = 0; i < count; i++)
		{
			MDCustomLineUnitRebar* pUnit = new MDCustomLineUnitRebar;
			pUnit->SetMDMemberM(this);
			pUnit->Serialize(ar);
			ma_LineUnit.push_back(pUnit);
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20190530)
		{
			int nSTieBar = 0;
			ar >> nSTieBar;
			for (int i = 0; i < nSTieBar; i++)
			{
				MSSpecificTieBar* pTieBar = new MSSpecificTieBar;
				pTieBar->Serialize(ar);
				ma_SpecificTieBar.push_back(pTieBar);
			}
		}
	}
}

void MDCustomSection::CopyFromMe(MDMember* pSource)
{
	if (pSource == NULL)	return;
	MDCustomSection* pSourceRebar = (MDCustomSection*)pSource;

	DeleteCustomMDPointUnitRebars();
	DeleteCustomMDLineUnitRebars();

	vector<MDCustomPointUnitRebar*> PointUnits = pSourceRebar->GetMDPointUnitRebars();
	for (int i = 0; i < PointUnits.size(); i++)
	{
		MDCustomPointUnitRebar* pUnit = new MDCustomPointUnitRebar;
		pUnit->CopyFromMe(PointUnits[i]);
		pUnit->SetMDMemberM(this);
		ma_PointUnit.push_back(pUnit);
	}

	vector<MDCustomLineUnitRebar*> LineUnits = pSourceRebar->GetMDLineUnitRebars();
	for (int i = 0; i < LineUnits.size(); i++)
	{
		MDCustomLineUnitRebar* pUnit = new MDCustomLineUnitRebar;
		pUnit->CopyFromMe(LineUnits[i]);
		pUnit->SetMDMemberM(this);
		ma_LineUnit.push_back(pUnit);
	}

	DeleteSpecificTieBars();
	int numTieBars = pSourceRebar->ma_SpecificTieBar.size();
	for (int i = 0; i < numTieBars; i++)
	{
		MSSpecificTieBar* pUrTieBar = pSourceRebar->ma_SpecificTieBar[i];
		MSSpecificTieBar* pMyTieBar = new MSSpecificTieBar();
		pMyTieBar->CopyFromMe(pUrTieBar);
		ma_SpecificTieBar.push_back(pMyTieBar);
	}

	MDMember::CopyFromMe(pSource);
}

void MDCustomSection::Dump(EFS::EFS_Buffer& buffer)
{
	MDMember::Dump(buffer);

	int count = 0;
	count = ma_PointUnit.size();

	buffer << count;
	for (int i = 0; i < count; i++)
		ma_PointUnit[i]->Dump(buffer);

	count = ma_LineUnit.size();
	buffer << count;
	for (int i = 0; i < count; i++)
		ma_LineUnit[i]->Dump(buffer);

	int nSTieBar = ma_SpecificTieBar.size();
	buffer << nSTieBar;
	for (int i = 0; i < nSTieBar; i++)
	{
		MSSpecificTieBar* pTieBar = ma_SpecificTieBar[i];
		pTieBar->Dump(buffer);
	}
}

void MDCustomSection::Recovery(EFS::EFS_Buffer& buffer)
{
	MDMember::Recovery(buffer);

	DeleteCustomMDPointUnitRebars();
	DeleteCustomMDLineUnitRebars();
	int count = 0;

	buffer >> count;
	for (int i = 0; i < count; i++)
	{
		MDCustomPointUnitRebar* pUnit = new MDCustomPointUnitRebar;
		pUnit->Recovery(buffer);
		ma_PointUnit.push_back(pUnit);
	}

	buffer >> count;
	for (int i = 0; i < count; i++)
	{
		MDCustomLineUnitRebar* pUnit = new MDCustomLineUnitRebar;
		pUnit->Recovery(buffer);
		ma_LineUnit.push_back(pUnit);
	}

	DeleteSpecificTieBars();
	int nSTieBar = 0;
	buffer >> nSTieBar;
	for (int i = 0; i < nSTieBar; i++)
	{
		MSSpecificTieBar* pTieBar = new MSSpecificTieBar();
		pTieBar->Recovery(buffer);
		ma_SpecificTieBar.push_back(pTieBar);
	}
}

MDCustomPointUnitRebar* MDCustomSection::GetMDPointUnitRebar(int unitID)
{
	for (int i = 0; i < ma_PointUnit.size(); i++)
	{
		if (ma_PointUnit[i]->GetUnitID() == unitID)
			return ma_PointUnit[i];
	}
	return nullptr;
}

MDCustomLineUnitRebar* MDCustomSection::GetMDLineUnitRebar(int unitID)
{
	for (int i = 0; i < ma_LineUnit.size(); i++)
	{
		if (ma_LineUnit[i]->GetUnitID() == unitID)
			return ma_LineUnit[i];
	}
	return nullptr;
}

void MDCustomSection::MakeDefaultMDPointUnitRebar(MSCustomPointBarUnit* pMSPointUnit)
{
	CString sBar = MSDeformedBar::GetMainBarStr(4);

	MDCustomPointUnitRebar* pRebar = new MDCustomPointUnitRebar();
	pRebar->SetMDMemberM(this);
	pRebar->SetUnitID(pMSPointUnit->GetID());
	pRebar->SetMainBarSet(sBar);
	ma_PointUnit.push_back(pRebar);
}

void MDCustomSection::MakeDefaultMDLineUnitRebar(MSCustomLineBarUnit* pMSLineUnit)
{	
	int numTieBar = 2;	
	CString sBar = MSDeformedBar::GetStirrupBarStr(300);
	MDCustomLineUnitRebar* pRebar = new MDCustomLineUnitRebar();
	pRebar->SetMDMemberM(this);
	pRebar->SetUnitID(pMSLineUnit->GetID());
	bool isNumAndSpacingType = MSCustomLineBarUnit::IsNumAndSpacingBarType(pMSLineUnit->GetLineBarType());
	if(!isNumAndSpacingType)
		numTieBar = 0;
	pRebar->SetNumBars(numTieBar);
	pRebar->SetStirrupBar(sBar);
	ma_LineUnit.push_back(pRebar);
}

void MDCustomSection::UpdateCustomRebarsID(map<long, long> oldUnitIDtoNewUnitID)
{
	for (int i = 0; i < ma_PointUnit.size(); i++)
	{
		int nUnitID = ma_PointUnit[i]->GetUnitID();

		map<long, long>::iterator it = oldUnitIDtoNewUnitID.find(nUnitID);
		if (it != oldUnitIDtoNewUnitID.end())
			ma_PointUnit[i]->SetUnitID(it->second);
	}

	for (int i = 0; i < ma_LineUnit.size(); i++)
	{
		int nUnitID = ma_LineUnit[i]->GetUnitID();

		map<long, long>::iterator it = oldUnitIDtoNewUnitID.find(nUnitID);
		if (it != oldUnitIDtoNewUnitID.end())
			ma_LineUnit[i]->SetUnitID(it->second);
	}
}

void MDCustomSection::UpdateCustomRebars(MSCustomSection* pMSSection)
{
	DeletePointRebarsByCustomSection(pMSSection);
	DeleteLineRebarsByCustomSection(pMSSection);

	AddedPointRebarsByCustomSection(pMSSection);
	AddedLineRebarsByCustomSection(pMSSection);
}

void MDCustomSection::DeletePointRebarsByCustomSection(MSCustomSection* pMSSection)
{
	for (int i = ma_PointUnit.size() - 1; i >= 0; i--)
	{
		MDCustomPointUnitRebar* pRebar = ma_PointUnit[i];
		MSCustomPointBarUnit* pUnit = pMSSection->GetMSPointUnit(pRebar->GetUnitID());
		if (pUnit == nullptr)
		{
			delete pRebar;
			ma_PointUnit.erase(ma_PointUnit.begin() + i);
		}
	}
}
void MDCustomSection::DeleteLineRebarsByCustomSection(MSCustomSection* pMSSection)
{
	for (int i = ma_LineUnit.size() - 1; i >= 0; i--)
	{
		MDCustomLineUnitRebar* pRebar = ma_LineUnit[i];
		MSCustomLineBarUnit* pUnit = pMSSection->GetMSLineUnit(pRebar->GetUnitID());
		if (pUnit == nullptr)
		{
			delete pRebar;
			ma_LineUnit.erase(ma_LineUnit.begin() + i);
		}
	}
}

void MDCustomSection::AddedPointRebarsByCustomSection(MSCustomSection* pMSSection)
{
	vector<MSCustomPointBarUnit*> pointUnits = pMSSection->GetMSPointUnits();
	for (int i = 0; i < pointUnits.size(); i++)
	{
		MSCustomPointBarUnit* pUnit = pointUnits[i];

		MDCustomPointUnitRebar* pRebar = GetMDPointUnitRebar(pUnit->GetID());
		if (pRebar != nullptr)	continue;

		MakeDefaultMDPointUnitRebar(pUnit);
	}
}
void MDCustomSection::AddedLineRebarsByCustomSection(MSCustomSection* pMSSection)
{
	vector<MSCustomLineBarUnit*> lineUnits = pMSSection->GetMSLineUnits();
	for (int i = 0; i < lineUnits.size(); i++)
	{
		MSCustomLineBarUnit* pUnit = lineUnits[i];

		MDCustomLineUnitRebar* pRebar = GetMDLineUnitRebar(pUnit->GetID());
		if (pRebar != nullptr)	continue;

		MakeDefaultMDLineUnitRebar(pUnit);
	}
}

double MDCustomSection::GetMaxDeformedBarDiameterByLineUnitRebar()
{
	double dMaxDiameter = 0;
	for (int iUnit = 0; iUnit < ma_PointUnit.size(); iUnit++)
		dMaxDiameter = max(dMaxDiameter, ma_PointUnit[iUnit]->GetMaxDeformedMainBarDiameter());

	return dMaxDiameter;
}

double MDCustomSection::GetMaxDeformedBarDiameterByPointUnitRebar()
{
	double dMaxDiameter = 0;
	for (int iUnit = 0; iUnit < ma_LineUnit.size(); iUnit++)
		dMaxDiameter = max(dMaxDiameter, ma_LineUnit[iUnit]->GetMaxDeformedStirrupBarDiameter());

	return dMaxDiameter;
}

void MDCustomSection::DeleteSpecificTieBars()
{
	int nSTieBar = ma_SpecificTieBar.size();
	for (int iTiebar = 0; iTiebar < nSTieBar; iTiebar++)
		delete ma_SpecificTieBar[iTiebar];
	ma_SpecificTieBar.clear();
}