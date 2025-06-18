#include "stdafx.h"
#include "MDCustomPointUnitRebar.h"
#include "MDMember.h"
#include "MSDeformedBar.h"
#include "MDCustomLineM.h"
#include "MSCustomPointBarUnit.h"

#include "GMLib/GM2DLineVector.h"

IMPLEMENT_SERIAL(MDCustomPointUnitRebar, MSRcObject, VERSIONABLE_SCHEMA | 1)
MDCustomPointUnitRebar::MDCustomPointUnitRebar()
{
	CString sBar = MSDeformedBar::GetMainBarStr(4);

	m_PointUnitID = 0;
	m_MainBarSet.InitData(sBar);

	mp_RebarM = nullptr;
}

MDCustomPointUnitRebar::~MDCustomPointUnitRebar()
{

}

void MDCustomPointUnitRebar::Serialize(CArchive& ar)
{
	MSRcObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_PointUnitID;
		m_MainBarSet.Serialize(ar);
	}
	else
	{
		ar >> m_PointUnitID;
		m_MainBarSet.Serialize(ar);
	}
}

void MDCustomPointUnitRebar::CopyFromMe(MSRcObject* pSource)
{
	if (pSource == NULL)	return;
	MDCustomPointUnitRebar* pSourceRebar = (MDCustomPointUnitRebar*)pSource;

	mp_RebarM = pSourceRebar->GetMDMemberM();

	m_PointUnitID = pSourceRebar->GetUnitID();
	m_MainBarSet.CopyFromMe(&pSourceRebar->GetMainBarSet());

	MSRcObject::CopyFromMe(pSource);
}

void MDCustomPointUnitRebar::Dump(EFS::EFS_Buffer& buffer)
{
	MSRcObject::Dump(buffer);

	buffer << m_PointUnitID;
	m_MainBarSet.Dump(buffer);

}

void MDCustomPointUnitRebar::Recovery(EFS::EFS_Buffer& buffer)
{
	MSRcObject::Recovery(buffer);

	buffer >> m_PointUnitID;
	m_MainBarSet.Recovery(buffer);
}

bool MDCustomPointUnitRebar::SetMainBarSet(CString fullData)
{
	fullData.Replace(_T(" "), _T(""));
	int numSet = MSMainBarSet::GetNumSets(fullData);
	if (numSet != 1)	return false;

	if (!MSMainBarSet::CheckMainBarSet(fullData))
		return false;

	m_MainBarSet.SetBarString(fullData);
	return true;
}

CString MDCustomPointUnitRebar::GetMainBarText()
{
	return m_MainBarSet.GetBarString();
}

vector<GM2DVector> MDCustomPointUnitRebar::CalculatorPointBarLocation(GM2DLineVector lineVec, MSCustomPointBarUnit::eSpacingType spacingType)
{
	vector<GM2DVector> locArr;
	if (spacingType == MSCustomPointBarUnit::Equal)
		return CalculatorPointBarLocationByEqual(lineVec);
	else if (spacingType == MSCustomPointBarUnit::EffectiveSpacing)
		return CalculatorPointBarLocationByEffectiveSpacing(lineVec);

	return locArr;
}

vector<GM2DVector> MDCustomPointUnitRebar::CalculatorPointBarLocationByEqual(GM2DLineVector lineVec)
{
	vector<GM2DVector> locArr;
	double Length = lineVec.Length();
	int NumBars = m_MainBarSet.GetTotalBars();
	if (NumBars == 0)	return locArr;
	if (NumBars == 1)
	{
		locArr.push_back(lineVec.PointAtParam(0.5));
		return locArr;
	}

	double spacing = Length / (NumBars - 1);
	double spacingRatio = spacing / lineVec.Length();
	for (int i = 0; i < NumBars; i++)
	{
		locArr.push_back(lineVec.PointAtParam(spacingRatio * i));
	}
	return locArr;
}

vector<GM2DVector> MDCustomPointUnitRebar::CalculatorPointBarLocationByEffectiveSpacing(GM2DLineVector lineVec)
{
	vector<GM2DVector> locArr;
	double Length = lineVec.Length();
	int NumBars = m_MainBarSet.GetTotalBars();
	if (NumBars == 0)	return locArr;
	if (NumBars == 1)
	{
		locArr.push_back(lineVec.PointAtParam(0.5));
		return locArr;
	}

	if (NumBars == -1)	return locArr;
	double spacing = Length / (NumBars + 1);
	double spacingRatio = spacing / lineVec.Length();
	for (int i = 0; i < NumBars; i++)
	{
		locArr.push_back(lineVec.PointAtParam(spacingRatio * (i + 1)));
	}
	return locArr;
}

double MDCustomPointUnitRebar::GetMaxDeformedMainBarDiameter()
{
	return m_MainBarSet.GetMaxDeformedBarDiameter();
}