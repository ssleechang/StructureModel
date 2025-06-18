#include "stdafx.h"
#include "MDCustomLineUnitRebar.h"
#include "MDMember.h"
#include "MSDeformedBar.h"
#include "MDCustomLineM.h"
#include "MSCustomReBarLine.h"
#include "GMLib/GM2DLineVector.h"

IMPLEMENT_SERIAL(MDCustomLineUnitRebar, MSRcObject, VERSIONABLE_SCHEMA | 1)
MDCustomLineUnitRebar::MDCustomLineUnitRebar()
{
	CString sBar = MSDeformedBar::GetStirrupBarStr(300);
	m_LineUnitID = 0;
	m_NumBars = 1;
	m_StirrupBar.InitData(sBar);

	mp_RebarM = nullptr;

}

MDCustomLineUnitRebar::~MDCustomLineUnitRebar()
{

}

void MDCustomLineUnitRebar::Serialize(CArchive& ar)
{
	MSRcObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_LineUnitID;
		ar << m_NumBars;
		m_StirrupBar.Serialize(ar);
	}
	else
	{
		ar >> m_LineUnitID;
		ar >> m_NumBars;
		m_StirrupBar.Serialize(ar);
	}
}

void MDCustomLineUnitRebar::CopyFromMe(MSRcObject* pSource)
{
	if (pSource == NULL)	return;
	MDCustomLineUnitRebar* pSourceRebar = (MDCustomLineUnitRebar*)pSource;

	mp_RebarM = pSourceRebar->GetMDMemberM();

	m_LineUnitID = pSourceRebar->GetUnitID();
	m_NumBars = pSourceRebar->GetNumBars();
	m_StirrupBar.CopyFromMe(&pSourceRebar->GetStirrupBar());

	MSRcObject::CopyFromMe(pSource);
}

void MDCustomLineUnitRebar::Dump(EFS::EFS_Buffer& buffer)
{
	MSRcObject::Dump(buffer);

	buffer << m_LineUnitID;
	buffer << m_NumBars;
	m_StirrupBar.Dump(buffer);

}

void MDCustomLineUnitRebar::Recovery(EFS::EFS_Buffer& buffer)
{
	MSRcObject::Recovery(buffer);

	buffer >> m_LineUnitID;
	buffer >> m_NumBars;
	m_StirrupBar.Recovery(buffer);
}

CString MDCustomLineUnitRebar::GetStirrupBarText()
{
	return m_StirrupBar.GetStirrupBarString();
}

void MDCustomLineUnitRebar::SetStirrupBar(CString rebarText)
{
	m_StirrupBar.InitData(rebarText);
}

vector<GM2DPolyline*> MDCustomLineUnitRebar::CalculatorLineBarLocation(MSCustomLineBarUnit* pUnit)
{
	vector<GM2DPolyline*> locArr;
	bool isNumAndSpacingType = MSCustomLineBarUnit::IsNumAndSpacingBarType(pUnit->GetLineBarType());
	if (!isNumAndSpacingType)
		return CalculatorLineBarLocationByHoop(pUnit);
	else
		return CalculatorLineBarLocationByTie(pUnit);
	return locArr;
}

vector<GM2DPolyline*> MDCustomLineUnitRebar::CalculatorLineBarLocationByHoop(MSCustomLineBarUnit* pUnit)
{
	vector<GM2DPolyline*> locArr;
//	if (GetNumBars() == 0)	return locArr;

	GM2DPolyline* sourcePolyline = pUnit->GetProfile();
	if (sourcePolyline == nullptr)	return locArr;
	GM2DPolyline* pProfile = new GM2DPolyline();
	pProfile->CopyFrom(sourcePolyline);
	locArr.push_back(pProfile);

	delete sourcePolyline;
	return locArr;
}

vector<GM2DPolyline*> MDCustomLineUnitRebar::CalculatorLineBarLocationByTie(MSCustomLineBarUnit* pUnit)
{
	vector<GM2DPolyline*> locArr;
	if (pUnit->GetSectionSpacingType() == MSCustomLineBarUnit::SectionEqual)
		return CalculatorTieLineBarLocationByEqual(pUnit);
	else if (pUnit->GetSectionSpacingType() == MSCustomLineBarUnit::EffectiveSpacing)
		return CalculatorTieLineBarLocationByEffectiveSpacing(pUnit);

	return locArr;
}

vector<GM2DPolyline*> MDCustomLineUnitRebar::CalculatorTieLineBarLocationByEqual(MSCustomLineBarUnit* pUnit)
{
	vector<GM2DPolyline*> locArr;
	if (GetNumBars() == 0)	return locArr;

	int rebarLineID1 = pUnit->GetDistLocation().GetRebarLineID1();
	int rebarLineID2 = pUnit->GetDistLocation().GetRebarLineID2();
	GM2DVector* pCrossVec = pUnit->GetCustomSection()->GetCrossPointByRebarLineIDs(rebarLineID1, rebarLineID2);
	if (pCrossVec == nullptr)	return locArr;

	GM2DLineVector lineVec;
	if (!pUnit->GetTieLineVector(lineVec))
	{
		delete pCrossVec;
		return locArr;
	}

	MSCustomReBarLine* pRefLine0 = pUnit->GetReferenceRebarLine_InTieBarP0();
	MSCustomReBarLine* pRefLine1 = pUnit->GetReferenceRebarLine_InTieBarP1();
	if (pRefLine0 == nullptr || pRefLine1 == nullptr)
	{
		delete pCrossVec;	return locArr;
	}

	GM2DLineVector* pRefLineVector0 = dynamic_cast<GM2DLineVector*>(pRefLine0->GetCurve());
	GM2DLineVector* pRefLineVector1 = dynamic_cast<GM2DLineVector*>(pRefLine1->GetCurve());
	if (pRefLineVector0 == nullptr || pRefLineVector1 == nullptr)
	{
		delete pRefLineVector0;	delete pRefLineVector1;delete pCrossVec;	return locArr;
	}

	GM2DVector crossVecP0, crossVecP1;
	double myLoc = 0.0, urLoc = 0.0;
	double distance = lineVec.Distance(*pCrossVec);
	if (GetNumBars() == 1)
	{
		GM2DLineVector locLineVec = lineVec.NormalTranslate(distance / 2, *pCrossVec);
		if (pRefLineVector0->IntersectEndWithLine(locLineVec, crossVecP0, &myLoc, &urLoc) > 0 && 
			pRefLineVector1->IntersectEndWithLine(locLineVec, crossVecP1, &myLoc, &urLoc) > 0)
		{
			locLineVec.SetP0(crossVecP0);	locLineVec.SetP1(crossVecP1);

			GM2DPolyline* locPolyline = ConvertLineToPolyline(&locLineVec);
			locArr.push_back(locPolyline);
		}
	}
	else
	{
		double spacing = distance / (GetNumBars() - 1);
		for (int i = 0; i < GetNumBars(); i++)
		{
			GM2DLineVector locLineVec = lineVec.NormalTranslate(spacing * i, *pCrossVec);
			if (pRefLineVector0->IntersectEndWithLine(locLineVec, crossVecP0, &myLoc, &urLoc) <= 0)	continue;
			if (pRefLineVector1->IntersectEndWithLine(locLineVec, crossVecP1, &myLoc, &urLoc) <= 0)	continue;
			locLineVec.SetP0(crossVecP0);	locLineVec.SetP1(crossVecP1);

			GM2DPolyline* locPolyline = ConvertLineToPolyline(&locLineVec);
			locArr.push_back(locPolyline);
		}
	}

	delete pRefLineVector0;	delete pRefLineVector1;
	delete pCrossVec;
	return locArr;
}

vector<GM2DPolyline*> MDCustomLineUnitRebar::CalculatorTieLineBarLocationByEffectiveSpacing(MSCustomLineBarUnit* pUnit)
{
	vector<GM2DPolyline*> locArr;
	if (GetNumBars() == 0)	return locArr;

	int rebarLineID1 = pUnit->GetDistLocation().GetRebarLineID1();
	int rebarLineID2 = pUnit->GetDistLocation().GetRebarLineID2();
	GM2DVector* pCrossVec = pUnit->GetCustomSection()->GetCrossPointByRebarLineIDs(rebarLineID1, rebarLineID2);
	if (pCrossVec == nullptr)	return locArr;

	GM2DLineVector lineVec;
	if (!pUnit->GetTieLineVector(lineVec))	return locArr;

	MSCustomReBarLine* pRefLine0 = pUnit->GetReferenceRebarLine_InTieBarP0();
	MSCustomReBarLine* pRefLine1 = pUnit->GetReferenceRebarLine_InTieBarP1();
	if (pRefLine0 == nullptr || pRefLine1 == nullptr)
	{
		delete pCrossVec;	return locArr;
	}

	GM2DLineVector* pRefLineVector0 = dynamic_cast<GM2DLineVector*>(pRefLine0->GetCurve());
	GM2DLineVector* pRefLineVector1 = dynamic_cast<GM2DLineVector*>(pRefLine1->GetCurve());
	if (pRefLineVector0 == nullptr || pRefLineVector1 == nullptr)
	{
		delete pRefLineVector0;	delete pRefLineVector1;delete pCrossVec;	return locArr;
	}

	GM2DVector crossVecP0, crossVecP1;
	double myLoc = 0.0, urLoc = 0.0;
	double distance = lineVec.Distance(*pCrossVec);
	if (GetNumBars() == 1)
	{
		GM2DLineVector locLineVec = lineVec.NormalTranslate(distance / 2, *pCrossVec);
		if (pRefLineVector0->IntersectEndWithLine(locLineVec, crossVecP0, &myLoc, &urLoc) <= 0 &&
			pRefLineVector1->IntersectEndWithLine(locLineVec, crossVecP1, &myLoc, &urLoc) <= 0)
		{
			locLineVec.SetP0(crossVecP0);	locLineVec.SetP1(crossVecP1);

			GM2DPolyline* locPolyline = ConvertLineToPolyline(&locLineVec);
			locArr.push_back(locPolyline);
		}
	}
	else
	{
		double spacing = distance / (GetNumBars() + 1);
		for (int i = 0; i < GetNumBars(); i++)
		{
			GM2DLineVector locLineVec = lineVec.NormalTranslate(spacing * (i + 1), *pCrossVec);
			if (pRefLineVector0->IntersectEndWithLine(locLineVec, crossVecP0, &myLoc, &urLoc) <= 0)	continue;
			if (pRefLineVector1->IntersectEndWithLine(locLineVec, crossVecP1, &myLoc, &urLoc) <= 0)	continue;
			locLineVec.SetP0(crossVecP0);	locLineVec.SetP1(crossVecP1);

			GM2DPolyline* locPolyline = ConvertLineToPolyline(&locLineVec);
			locArr.push_back(locPolyline);
		}
	}

	delete pRefLineVector0;	delete pRefLineVector1;
	delete pCrossVec;
	return locArr;
}
GM2DPolyline* MDCustomLineUnitRebar::ConvertLineToPolyline(GM2DLineVector* pLineVec)
{
	GM2DPolyline* polyline = new GM2DPolyline();
	polyline->Add(new GM2DVector(pLineVec->m_P0));
	polyline->Add(new GM2DVector(pLineVec->m_P1));
	polyline->ma_Bulge.Add(0.0);
	return polyline;
}

double MDCustomLineUnitRebar::GetMaxDeformedStirrupBarDiameter()
{
	return m_StirrupBar.GetMaxBarDia();
}