#include "stdafx.h"

#include "MSCustomLineSection.h"

#include "MSCustomLineBarUnit.h"
#include "MSCustomPointBarUnit.h"
#include "MSCustomRebar.h"
#include "MSRebarShape.h"
#include "MSRebarBuilding.h"
#include "MSCustomReBarLine.h"

IMPLEMENT_SERIAL(MSCustomLineSection, MSCustomSection, VERSIONABLE_SCHEMA | 1)

MSCustomLineSection::MSCustomLineSection(MSBaseBuilding* pBuilding) : MSCustomSection(pBuilding)
{
	m_eSectionType = eSectionType::CUSTOM_NONE;
}

MSCustomLineSection::~MSCustomLineSection(void)
{
	DeleteMSLineUnits();
	DeleteMSPointUnits();
}

void MSCustomLineSection::DeleteMSLineUnits()
{
	for (int i = 0; i < ma_LineUnit.size(); i++)
		delete ma_LineUnit[i];
	ma_LineUnit.clear();
}

void MSCustomLineSection::DeleteMSPointUnits()
{
	for (int i = 0; i < ma_PointUnit.size(); i++)
		delete ma_PointUnit[i];
	ma_PointUnit.clear();
}

MSObject* MSCustomLineSection::Copy(bool bNewID)
{
	MSCustomLineSection* pNewObj;

	pNewObj = (MSCustomLineSection*)GetBuilding()->CreateNewCustomSection(m_eSectionType);
	if (pNewObj == nullptr)	return nullptr;
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSCustomLineSection::CopyFromMe(MSCustomSection* pSource, bool bNewID)
{
	MSCustomSection::CopyFromMe(pSource);
	MSCustomLineSection* pSourceM = (MSCustomLineSection*)pSource;
	DeleteMSPointUnits();
	auto pointUnits = pSourceM->GetMSPointUnits();
	for (auto it = pointUnits.begin(); it != pointUnits.end(); it++)
	{
		MSCustomPointBarUnit* pUnit = new MSCustomPointBarUnit();
		pUnit->CopyFromMe(*it);
		pUnit->SetCustomSection(this);
		AddMSPointUnit(pUnit);
	}

	DeleteMSLineUnits();
	auto lineUnits = pSourceM->GetMSLineUnits();
	for (auto it = lineUnits.begin(); it != lineUnits.end(); it++)
	{
		MSCustomLineBarUnit* pUnit = new MSCustomLineBarUnit();
		pUnit->CopyFromMe(*it);
		pUnit->SetCustomSection(this);

		AddMSLineUnit(pUnit);
	}

}

void MSCustomLineSection::CopyFromMe_Info(MSCustomSection* pSource)
{
	MSCustomSection::CopyFromMe_Info(pSource);
	CopyFromMe_MSCustomLineBarUnit(pSource);
	CopyFromMe_MSCustomPointBarUnit(pSource);
}

void MSCustomLineSection::CopyFromMe_MSCustomLineBarUnit(MSCustomSection* pSource)
{
	MSCustomLineSection* pSourceM = (MSCustomLineSection*)pSource;
	DeleteMSLineUnits();
	auto lineUnits = pSourceM->GetMSLineUnits();
	for (auto it = lineUnits.begin(); it != lineUnits.end(); it++)
	{
		MSCustomLineBarUnit* pUnit = new MSCustomLineBarUnit();
		pUnit->CopyFromMe(*it);
		pUnit->SetCustomSection(this);
		pUnit->m_ID = (*it)->GetID();
		AddMSLineUnit(pUnit);
	}
}

void MSCustomLineSection::CopyFromMe_MSCustomPointBarUnit(MSCustomSection* pSource)
{
	MSCustomLineSection* pSourceM = (MSCustomLineSection*)pSource;
	DeleteMSPointUnits();
	auto pointUnits = pSourceM->GetMSPointUnits();
	for (auto it = pointUnits.begin(); it != pointUnits.end(); it++)
	{
		MSCustomPointBarUnit* pUnit = new MSCustomPointBarUnit();
		pUnit->CopyFromMe(*it);
		pUnit->SetCustomSection(this);
		pUnit->m_ID = (*it)->GetID();
		AddMSPointUnit(pUnit);
	}
}
void MSCustomLineSection::Dump(EFS::EFS_Buffer& buffer)
{
	MSCustomSection::Dump(buffer);
	int count = ma_PointUnit.size();
	buffer << count;
	for (int i = 0; i < count; i++)
		ma_PointUnit[i]->Dump(buffer);

	count = ma_LineUnit.size();
	buffer << count;
	for (int i = 0; i < count; i++)
		ma_LineUnit[i]->Dump(buffer);

}

void MSCustomLineSection::Recovery(EFS::EFS_Buffer& buffer)
{
	MSCustomSection::Recovery(buffer);
	int count = 0;
	DeleteMSPointUnits();
	buffer >> count;
	for (int i = 0; i < count; i++)
	{
		MSCustomPointBarUnit* pUnit = new MSCustomPointBarUnit();
		pUnit->SetCustomSection(this);
		pUnit->Recovery(buffer);
		AddMSPointUnit(pUnit);
	}

	DeleteMSLineUnits();
	buffer >> count;
	for (int i = 0; i < count; i++)
	{
		MSCustomLineBarUnit* pUnit = new MSCustomLineBarUnit();
		pUnit->SetCustomSection(this);
		pUnit->Recovery(buffer);
		AddMSLineUnit(pUnit);
	}

}

void MSCustomLineSection::Serialize(CArchive &ar)
{
	MSCustomSection::Serialize(ar);

	int count = 0;
	if (ar.IsStoring())
	{
		count = ma_PointUnit.size();
		ar << count;
		for (int i = 0; i < count; i++)
			ma_PointUnit[i]->Serialize(ar);

		count = ma_LineUnit.size();
		ar << count;
		for (int i = 0; i < count; i++)
			ma_LineUnit[i]->Serialize(ar);
	}
	else
	{
		ar >> count;
		for (int i = 0; i < count; i++)
		{
			MSCustomPointBarUnit* pUnit = new MSCustomPointBarUnit();
			pUnit->SetCustomSection(this);
			pUnit->Serialize(ar);
			AddMSPointUnit(pUnit);
		}

		ar >> count;
		for (int i = 0; i < count; i++)
		{
			MSCustomLineBarUnit* pUnit = new MSCustomLineBarUnit();
			pUnit->SetCustomSection(this);
			pUnit->Serialize(ar);
			AddMSLineUnit(pUnit);
		}
	}
}

void MSCustomLineSection::AddMSLineUnit(MSCustomLineBarUnit* lineUnit)
{
	lineUnit->SetCustomSection(this);
	ma_LineUnit.push_back(lineUnit);
}

void MSCustomLineSection::AddMSPointUnit(MSCustomPointBarUnit* pointUnit)
{
	pointUnit->SetCustomSection(this);
	ma_PointUnit.push_back(pointUnit);
}

MSCustomPointBarUnit* MSCustomLineSection::GetMSPointUnit(int nID)
{
	for (int i = 0; i < ma_PointUnit.size(); i++)
	{
		if (ma_PointUnit[i]->m_ID == nID)
			return ma_PointUnit[i];
	}
	return nullptr;
}

MSCustomLineBarUnit* MSCustomLineSection::GetMSLineUnit(int nID)
{
	for (int i = 0; i < ma_LineUnit.size(); i++)
	{
		if (ma_LineUnit[i]->m_ID == nID)
			return ma_LineUnit[i];
	}
	return nullptr;
}

bool MSCustomLineSection::DeleteMSPointUnit(long nID)
{
	for (int i = ma_PointUnit.size() -1 ; i >= 0 ; i--)
	{
		if (ma_PointUnit[i]->GetID() == nID)
		{
			delete ma_PointUnit[i];
			ma_PointUnit.erase(ma_PointUnit.begin() + i);
			return true;
		}
	}
	return false;
}

bool MSCustomLineSection::DeleteLineUnit_HasRebarLineID(long RebarLineID)
{
	for (int i = ma_LineUnit.size() - 1; i >= 0; i--)
	{
		if (ma_LineUnit[i]->HasRebarLineID(RebarLineID))
		{
			delete ma_LineUnit[i];
			ma_LineUnit.erase(ma_LineUnit.begin() + i);
		}
	}

	return false;
}

bool MSCustomLineSection::DeleteMSLineBarUnit(long nID)
{
	for (int i = ma_LineUnit.size() - 1; i >= 0; i--)
	{
		if (ma_LineUnit[i]->GetID() == nID)
		{
			delete ma_LineUnit[i];
			ma_LineUnit.erase(ma_LineUnit.begin() + i);
			return true;
		}
	}
	return false;
}

// MSCustomRebar* MSCustomVerticalLineMemberSection::GetCustomRebar(int nPointUnitID, int nRebarIndex)
// {
// 	MSCustomPointBarUnit* pCustomPointBarUnit = GetPointUnit(nPointUnitID);
// 	if (pCustomPointBarUnit == nullptr)
// 		return nullptr;
// 
// 	return pCustomPointBarUnit->GetRebarByIndex(nRebarIndex);
// }

bool MSCustomLineSection::DeleteMSReBarLine(long UnitID)
{
	bool bVal = MSCustomSection::DeleteMSReBarLine(UnitID);
	DeletePointUnit_HasRebarLineID(UnitID);
	DeleteLineUnit_HasRebarLineID(UnitID);

	return bVal;
}

void MSCustomLineSection::DeleteMSReBarLines()
{
	MSCustomSection::DeleteMSReBarLines();
	DeleteMSPointUnits();
	DeleteMSLineUnits();
}

void MSCustomLineSection::GetRebarTypeToIDs(map<MSCustomSection::eRebarType, vector<long>>& RebarTypeToIDs)
{
	vector<long> rebarIDs;
	for (int iRebarLine = 0; iRebarLine < ma_RebarLine.size(); iRebarLine++)
		rebarIDs.push_back(ma_RebarLine[iRebarLine]->m_ID);

	RebarTypeToIDs.insert(make_pair(MSCustomSection::eRebarType::REBAR, rebarIDs));

	vector<long> pointIDs;
	for (int ipoint = 0; ipoint < ma_PointUnit.size(); ipoint++)
		pointIDs.push_back(ma_PointUnit[ipoint]->m_ID);

	RebarTypeToIDs.insert(make_pair(MSCustomSection::eRebarType::POINT, pointIDs));

	vector<long> lineIDs;
	for (int iline = 0; iline < ma_LineUnit.size(); iline++)
		lineIDs.push_back(ma_LineUnit[iline]->m_ID);

	RebarTypeToIDs.insert(make_pair(MSCustomSection::eRebarType::LINE, lineIDs));
}

void MSCustomLineSection::SetRebarTypeToIDs(map<MSCustomSection::eRebarType, vector<long>> RebarTypeToIDs)
{
	for (int iRebarLine = 0; iRebarLine < ma_RebarLine.size(); iRebarLine++)
	{
		if(iRebarLine < RebarTypeToIDs[MSCustomSection::eRebarType::REBAR].size())
			ma_RebarLine[iRebarLine]->m_ID = RebarTypeToIDs[MSCustomSection::eRebarType::REBAR][iRebarLine];
		else
			ma_RebarLine[iRebarLine]->GetID();
	}
		
	for (int ipoint = 0; ipoint < ma_PointUnit.size(); ipoint++)
	{
		if (ipoint < RebarTypeToIDs[MSCustomSection::eRebarType::POINT].size())
			ma_PointUnit[ipoint]->m_ID = RebarTypeToIDs[MSCustomSection::eRebarType::POINT][ipoint];
		else
			ma_PointUnit[ipoint]->GetID();
	}
		
	for (int iline = 0; iline < ma_LineUnit.size(); iline++)
	{
		if (iline < RebarTypeToIDs[MSCustomSection::eRebarType::LINE].size())
			ma_LineUnit[iline]->m_ID = RebarTypeToIDs[MSCustomSection::eRebarType::LINE][iline];
		else
			ma_LineUnit[iline]->GetID();
	}
}

bool MSCustomLineSection::DeletePointUnit_HasRebarLineID(long RebarLineID)
{
	for (int i = ma_PointUnit.size() - 1; i >= 0; i--)
	{
		if (ma_PointUnit[i]->HasRebarLineID(RebarLineID))
		{
			delete ma_PointUnit[i];
			ma_PointUnit.erase(ma_PointUnit.begin() + i);
		}
	}
	return false;
}

int MSCustomLineSection::GetInputNumberByMSPointUnitID(int nID)
{
	vector<MSCustomPointBarUnit*> pointUnits = GetMSPointUnits();
	for (int i = 0; i < pointUnits.size(); i++)
	{
		if (pointUnits[i]->GetID() == nID)
			return i + 1;
	}
	return 0;
}

int MSCustomLineSection::GetInputNumberByMSLineUnitID(int nID)
{
	vector<MSCustomLineBarUnit*> lineUnits = GetMSLineUnits();
	for (int i = 0; i < lineUnits.size(); i++)
	{
		if (lineUnits[i]->GetID() == nID)
			return i + 1;
	}
	return 0;

}

int FindNewIDbyOldID(map<int, int> oldIDtoNewID, int oldID)
{
	int nNewID = -1;

	map<int, int>::iterator it = oldIDtoNewID.find(oldID);
	if (it != oldIDtoNewID.end())
	{
		nNewID = it->second;
	}

	return nNewID;
}

void MSCustomLineSection::UpdateCustomSectionInfoID(MSCustomSection* pOldCustomSection, map<long, long>& oldUnitIDtoNewUnitID, bool NewID)
{
	vector<MSCustomPointBarUnit*> PointBarUnits = pOldCustomSection->GetMSPointUnits();
	vector<MSCustomPointBarUnit*> NewPointBarUnits = GetMSPointUnits();
	for (int index = 0; index < NewPointBarUnits.size(); index++)
	{
		if(NewID)
			NewPointBarUnits[index]->GetID();

		if(index <= PointBarUnits.size() - 1)
			oldUnitIDtoNewUnitID.insert(make_pair(PointBarUnits[index]->m_ID, NewPointBarUnits[index]->m_ID));
	}

	vector<MSCustomLineBarUnit*> LineBarUnits = pOldCustomSection->GetMSLineUnits();
	vector<MSCustomLineBarUnit*> NewLineBarUnits = GetMSLineUnits();
	for (int index = 0; index < NewLineBarUnits.size(); index++)
	{
		if (NewID)
			NewLineBarUnits[index]->GetID();

		if (index <= LineBarUnits.size() - 1)
			oldUnitIDtoNewUnitID.insert(make_pair(LineBarUnits[index]->m_ID, NewLineBarUnits[index]->m_ID));
	}

	map<int, int> oldIDtoNewID;
	vector<MSCustomReBarLine*> RebarLines = pOldCustomSection->GetMSRebarLines();
	vector<MSCustomReBarLine*> NewRebarLines = GetMSRebarLines();
	for (int index = 0; index < NewRebarLines.size(); index++)
	{
		if (NewID)
			NewRebarLines[index]->GetID();

		if (index <= RebarLines.size() - 1)
			oldIDtoNewID.insert(make_pair(RebarLines[index]->m_ID, NewRebarLines[index]->m_ID));
	}

	int rebarLineD1 = 0, rebarLineD2 = 0;

	for (int index = 0; index < NewPointBarUnits.size(); index++)
	{
		MSCustomPointBarUnit* NewPointBarUnit = NewPointBarUnits[index];

		MSCustomLocation Newloactions = NewPointBarUnit->GetCustomLocationP0();
		rebarLineD1 = FindNewIDbyOldID(oldIDtoNewID, Newloactions.GetRebarLineID1());
		if(rebarLineD1 == -1) continue;
		rebarLineD2 = FindNewIDbyOldID(oldIDtoNewID, Newloactions.GetRebarLineID2());
		if (rebarLineD2 == -1) continue;

		NewPointBarUnit->SetCustomLocationP0(rebarLineD1, rebarLineD2);

		Newloactions = NewPointBarUnit->GetCustomLocationP1();
		rebarLineD1 = FindNewIDbyOldID(oldIDtoNewID, Newloactions.GetRebarLineID1());
		if (rebarLineD1 == -1) continue;
		rebarLineD2 = FindNewIDbyOldID(oldIDtoNewID, Newloactions.GetRebarLineID2());
		if (rebarLineD2 == -1) continue;

		NewPointBarUnit->SetCustomLocationP1(rebarLineD1, rebarLineD2);
	}

	for (int index = 0; index < NewLineBarUnits.size(); index++)
	{
		MSCustomLineBarUnit* NewLineBarUnit = NewLineBarUnits[index];

		vector<MSCustomLocation> NewLoactions = NewLineBarUnit->GetLocations();
		for (int index_1 = 0; index_1 < NewLoactions.size(); index_1++)
		{
			rebarLineD1 = FindNewIDbyOldID(oldIDtoNewID, NewLoactions[index_1].GetRebarLineID1());
			if (rebarLineD1 == -1) continue;
			rebarLineD2 = FindNewIDbyOldID(oldIDtoNewID, NewLoactions[index_1].GetRebarLineID2());
			if (rebarLineD2 == -1) continue;

			NewLineBarUnit->SetLocation(index_1, rebarLineD1, rebarLineD2);
		}
	}
}
