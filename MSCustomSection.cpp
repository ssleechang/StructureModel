#include "stdafx.h"
#include "MSCustomSection.h"
#include "MSCustomPointBarUnit.h"
#include "MSCustomLineBarUnit.h"
#include "MSCustomReBarLine.h"
#include "MSRebarBuilding.h"

#include "GMLib/GM2DLineVector.h"
#include "GMLib/GM2DArc.h"
long MSCustomSection::ms_LastID = 0;

IMPLEMENT_SERIAL(MSCustomSection, MSObject, VERSIONABLE_SCHEMA | 1)

MSCustomSection::MSCustomSection(MSBaseBuilding* pBuilding)
{
	m_ID = 0;
	mp_Profile = new GM2DPolyline();
	m_eSectionType = CUSTOM_NONE;
	mp_Builing = pBuilding;
}


MSCustomSection::~MSCustomSection(void)
{
	delete mp_Profile;
	DeleteMSReBarLines();
}

long MSCustomSection::GetID()
{
	if (m_ID == 0) // 
	{
		m_ID = ms_LastID + 1;
		ms_LastID = m_ID;
		return m_ID;
	}

	return m_ID;
}

void MSCustomSection::SetLastID(long NewID)
{
	if (NewID > ms_LastID)
		ms_LastID = NewID;
}

MSObject* MSCustomSection::Copy(bool bNewID)
{
	MSCustomSection* pNewObj;

	pNewObj = GetBuilding()->CreateNewCustomSection(m_eSectionType);
	if (pNewObj == nullptr)	return nullptr;
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSCustomSection::CopyFromMe(MSCustomSection* pSource, bool bNewID)
{
	m_Name = pSource->GetName();
	m_eSectionType = pSource->GetSectionType();
	SetProfile(pSource->GetProfile());

	DeleteMSReBarLines();
	for (int iRebarLine = 0; iRebarLine < pSource->ma_RebarLine.size(); iRebarLine++)
	{
		MSCustomReBarLine* pBarLine = new MSCustomReBarLine(this);
		pBarLine->CopyFromMe(pSource->ma_RebarLine[iRebarLine]);
		pBarLine->SetCustomSection(this);
		Add(pBarLine);
	}
}

void MSCustomSection::CopyFromMe_Info(MSCustomSection* pSource)
{
	m_Name = pSource->GetName();
	m_eSectionType = pSource->GetSectionType();
	SetProfile(pSource->GetProfile());

	CopyFromMe_CustomBarLine(pSource);
}

void MSCustomSection::CopyFromMe_CustomBarLine(MSCustomSection* pSource)
{
	DeleteMSReBarLines();
	for (int iRebarLine = 0; iRebarLine < pSource->ma_RebarLine.size(); iRebarLine++)
	{
		MSCustomReBarLine* pBarLine = new MSCustomReBarLine(this);
		pBarLine->CopyFromMe(pSource->ma_RebarLine[iRebarLine]);
		pBarLine->m_ID = pSource->ma_RebarLine[iRebarLine]->GetID();
		Add(pBarLine);
	}
}

void MSCustomSection::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	buffer << m_Name;
	mp_Profile->Dump(buffer);

	int NumRebarLine = ma_RebarLine.size();
	buffer << NumRebarLine;
	for (int iRebarLine = 0; iRebarLine < ma_RebarLine.size(); iRebarLine++)
		ma_RebarLine[iRebarLine]->Dump(buffer);
}

void MSCustomSection::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	buffer >> m_Name;
	mp_Profile->Recovery(buffer);

	DeleteMSReBarLines();
	int NumRebarLine = 0;
	buffer >> NumRebarLine;
	for (int iRebarLine = 0; iRebarLine < NumRebarLine; iRebarLine++)
	{
		MSCustomReBarLine* pBarLine = new MSCustomReBarLine(this);
		pBarLine->SetCustomSection(this);
		pBarLine->Recovery(buffer);
		Add(pBarLine);
	}
}

void MSCustomSection::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	int count = 0;
	if (ar.IsStoring())
	{
		ar << m_ID;
		ar << m_Name;
		mp_Profile->Serialize(ar);

		count = ma_RebarLine.size();
		ar << count;
		for (int iRebarLine = 0; iRebarLine < count; iRebarLine++)
		{
			ma_RebarLine[iRebarLine]->Serialize(ar);
		}
	}
	else
	{
		ar >> m_ID;
		ar >> m_Name;
		mp_Profile->Serialize(ar);

		ar >> count;
		for (int i = 0; i < count; i++)
		{
			MSCustomReBarLine* pRebarLine = new MSCustomReBarLine(this);
			pRebarLine->SetCustomSection(this);
			pRebarLine->Serialize(ar);
			Add(pRebarLine);
		}
		SetLastID(m_ID);
	}
}

void MSCustomSection::SetProfile(GM2DPolyline* profile)
{
	mp_Profile->CopyFrom(profile);
	mp_Profile->MakeCCW();
}

GM2DPolyline* MSCustomSection::GetProfile()
{
	return mp_Profile;
}

void MSCustomSection::GetSecPolyline(GM2DPolyline& APGon)
{
	// GetSecPolygon -> GetSecPolyline으로 변경하고 구현해야 함
	//ASSERT(0);

	APGon.CopyFrom(mp_Profile);
}

void MSCustomSection::Add(MSCustomReBarLine* pRebarLine)
{
	ma_RebarLine.push_back(pRebarLine);
}

void MSCustomSection::DeleteMSReBarLines()
{
	for (int iRebarLine = 0; iRebarLine < ma_RebarLine.size(); iRebarLine++)
		delete ma_RebarLine[iRebarLine];
	ma_RebarLine.clear();
}

MSCustomReBarLine* MSCustomSection::GetMSRebarLine(long nID)
{
	for (int iRebarLine = 0; iRebarLine < ma_RebarLine.size(); iRebarLine++)
	{
		if (ma_RebarLine[iRebarLine]->m_ID == nID)
			return ma_RebarLine[iRebarLine];
	}
	return nullptr;
}

GM2DCurve* MSCustomSection::GetRebarLineGeometry(long RebarLineID)
{
	for (int iRebarLine = 0; iRebarLine < ma_RebarLine.size(); iRebarLine++)
		if (ma_RebarLine[iRebarLine]->m_ID == RebarLineID)
			return ma_RebarLine[iRebarLine]->GetCurve();

	return nullptr;
}

bool MSCustomSection::DeleteMSReBarLine(long RebarLineID)
{
	for (int iRebarLine = ma_RebarLine.size() - 1; iRebarLine >= 0; iRebarLine--)
	{
		if (ma_RebarLine[iRebarLine]->m_ID == RebarLineID)
		{
			delete ma_RebarLine[iRebarLine];
			ma_RebarLine.erase(ma_RebarLine.begin() + iRebarLine);
			return true;
		}
	}
	return false;
}

MSCustomReBarLine* MSCustomSection::CreateMSCustomBarLine()
{
	return new MSCustomReBarLine();
}

MSCustomPointBarUnit* MSCustomSection::CreateMSPointBarUnit()
{
	return new MSCustomPointBarUnit();
}

MSCustomLineBarUnit* MSCustomSection::CreateMSLineBarUnit()
{
	return new MSCustomLineBarUnit();
}

bool MSCustomSection::IsExistRebarLine(int edgeIndex, double offset)
{
	double offsetTolerance = 10;
	for (int iRebarLine = 0; iRebarLine < ma_RebarLine.size(); iRebarLine++)
	{
		MSCustomReBarLine* pBarLine = ma_RebarLine[iRebarLine];
		if (pBarLine->GetEdgeIndex() == edgeIndex && fabs(offset - pBarLine->GetUserOffset()) < offsetTolerance)
			return true;
	}
	return false;
}

bool MSCustomSection::IsContainRebarLine(MSCustomReBarLine* pRebarLine)
{
	GM2DCurve* pCurve = pRebarLine->GetCurve();
	if (pCurve == nullptr)	return false;

	vector<GM2DCurve*> NewCurves;
	mp_Profile->GetCurveInPolygon(pCurve, NewCurves);
	if (NewCurves.size() == 0)	return false;

	GMObject::DeleteGMObject(NewCurves);

	return true;
}

bool MSCustomSection::MakeMSRebarLine(GM2DCurve* pCurve, int offsetType, double offset, int coverType)
{
	int edgeIndex = FindEdgeIndex(pCurve);
	if (edgeIndex < 0 || edgeIndex > mp_Profile->GetNumLines())
		return false;

	if (IsExistRebarLine(edgeIndex, offset))	return false;

	MSCustomReBarLine* pRebarLine = new MSCustomReBarLine(this);
	pRebarLine->SetCustomSection(this);
	pRebarLine->SetEdgeIndex(edgeIndex);
	pRebarLine->SetOffsetType(offsetType);
	pRebarLine->SetUserOffset(offset);
	pRebarLine->SetCoverType((eCoverLengthFace)coverType);

	if (!IsContainRebarLine(pRebarLine))
	{
		delete pRebarLine;
		return false;
	}

	pRebarLine->GetID();
	Add(pRebarLine);
	
	return true;
}

int MSCustomSection::FindEdgeIndex(GM2DCurve* pCurve)
{
	return mp_Profile->FindEdgeIndex(pCurve);
}

void MSCustomSection::MakeRebarLineIDs()
{
	for (int iRebarLine = 0; iRebarLine < ma_RebarLine.size(); iRebarLine++)
	{
		ma_RebarLine[iRebarLine]->m_ID = 0;
		ma_RebarLine[iRebarLine]->GetID();
	}
}

int MSCustomSection::CheckData()
{
	bool isValidProfile = true;
	if (mp_Profile->CheckPolygon(false) == FALSE)	isValidProfile = false;
	if (isValidProfile == false)
		return 1;

	for (int i = 0; i < ma_RebarLine.size(); i++)
	{
		MSCustomReBarLine* piCustomBarLine = ma_RebarLine[i];
		if (fabs(piCustomBarLine->GetOffSet()) < 1)
			return 2;

		GM2DCurve* pCurve = piCustomBarLine->GetCurve();
		if (pCurve == nullptr)
			return 2;
		delete pCurve;

		for (int j = i+1 ; j < ma_RebarLine.size(); j++)
		{
			MSCustomReBarLine* pjCustomBarLine = ma_RebarLine[j];
			if (piCustomBarLine->GetEdgeIndex() == pjCustomBarLine->GetEdgeIndex() &&
				fabs(piCustomBarLine->GetOffSet() - pjCustomBarLine->GetOffSet()) < DTOL_GM)
				return 2;
		}
	}

	return 0;
}

void MSCustomSection::ResetProfile()
{
	GetProfile()->ResetPolyline();
	DeleteMSReBarLines();
}

GM2DVector*MSCustomSection::GetCrossPointByRebarLineIDs(long rebarLineID1, long rebarLineID2)
{
	MSCustomReBarLine* pRebarLine1 = GetMSRebarLine(rebarLineID1);
	MSCustomReBarLine* pRebarLine2 = GetMSRebarLine(rebarLineID2);
	if (pRebarLine1 == nullptr || pRebarLine2 == nullptr)	return nullptr;

	GM2DCurve* pCurve1 = pRebarLine1->GetCurve();
	GM2DCurve* pCurve2 = pRebarLine2->GetCurve();
	if (pCurve1 == nullptr || pCurve2 == nullptr)	return nullptr;

	if (pCurve1->m_ObjType == GT_LINE && pCurve2->m_ObjType == GT_LINE)
	{
		GM2DLineVector* pLine1 = (GM2DLineVector*)pCurve1;
		GM2DLineVector* pLine2 = (GM2DLineVector*)pCurve2;
		GM2DVector CrossVec;
		double MyLoc = 0.0, UrLoc = 0.0;
		if (pLine1->IntersectIn(*pLine2, CrossVec, &MyLoc, &UrLoc))
			return new GM2DVector(CrossVec.m_X, CrossVec.m_Y);
	}
	return nullptr;
}
vector<GM2DVector> MSCustomSection::CalculatorPointBarLocation(int pointUnitID)
{
	vector<GM2DVector> locArr;
	return locArr;
}

vector<MSCustomPointBarUnit*> MSCustomSection::GetMSPointUnits()
{
	vector<MSCustomPointBarUnit*> PointUnitArr;
	return PointUnitArr;
}

vector<MSCustomLineBarUnit*> MSCustomSection::GetMSLineUnits()
{
	vector<MSCustomLineBarUnit*> LineUnitArr;
	return LineUnitArr;
}

vector<MSCustomReBarLine*> MSCustomSection::GetRebarLineByEdgeIdx(int EdgeIdx)
{
	vector<MSCustomReBarLine*> allRebarLines = ma_RebarLine;

	vector<MSCustomReBarLine*> returnLines;
	for each(auto rebarLine in allRebarLines)
	{
		if (rebarLine->GetEdgeIndex() == EdgeIdx)
		{
			returnLines.push_back(rebarLine);
		}
	}

	//offset 순으로 정렬
	int size = returnLines.size();
	if (size > 0)
	{
		for (int i = 0; i < size - 1; i++)
		{
			int min = i;
			for (int j = i + 1; j < size; j++)
			{
				double jOffset = returnLines[j]->GetOffSet();
				double minOffset = returnLines[min]->GetOffSet();
				if (jOffset < minOffset) min = j;
			}

			auto temp = returnLines[i];
			returnLines[i] = returnLines[min];
			returnLines[min] = temp;			
		}
	}
	return returnLines;
}