#include "stdafx.h"
#include "MSBentRebar.h"
#include "MSAddRebarAreaBent.h"

IMPLEMENT_SERIAL(MSBentRebar, MSLineRebar, VERSIONABLE_SCHEMA | 1)

MSBentRebar::MSBentRebar()
{
	m_eBentType = eBentType::OneBent;
	m_eStartBentLocation = eStartBentLocation::Top;
	m_pAddRebarAreaBent = nullptr;
}


MSBentRebar::~MSBentRebar()
{
}

void MSBentRebar::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;
	MSLineRebar::CopyFromMe(pSource_in, bNewID);

	MSBentRebar* pSourceRebar = dynamic_cast<MSBentRebar*>(pSource_in);
	if (pSourceRebar == nullptr)	return;

	m_eBentType = pSourceRebar->GetBentType();
	m_eStartBentLocation = pSourceRebar->GetStartBentLocation();

	ma_BentLocValue = pSourceRebar->ma_BentLocValue;
	m_RebarZone = pSourceRebar->m_RebarZone;
	m_RebarLine = pSourceRebar->m_RebarLine;
	m_BentLine.CopyFrom(&pSourceRebar->m_BentLine);
}

void MSBentRebar::Dump(EFS::EFS_Buffer& ar)
{
	MSLineRebar::Dump(ar);
	int bentType = (int)m_eBentType;
	int startBentLocation = (int)m_eStartBentLocation;
	
	ar << bentType << startBentLocation;

	int nBentLoc = ma_BentLocValue.size();
	ar << nBentLoc;
	for (int iBentLoc = 0; iBentLoc < nBentLoc; iBentLoc++)
		ar << ma_BentLocValue[iBentLoc];
	
	GM2DLineVector* pLineVec = &m_RebarZone;
	GM2DLineVector::Dump(ar, pLineVec);
	pLineVec = &m_RebarLine;
	GM2DLineVector::Dump(ar, pLineVec);
	m_BentLine.Dump(ar);
}

void MSBentRebar::Recovery(EFS::EFS_Buffer& ar)
{
	MSLineRebar::Recovery(ar);
	int bentType = 0, startBentLocation = 0;
	ar >> bentType >> startBentLocation;

	m_eBentType = (eBentType)bentType;
	m_eStartBentLocation = (eStartBentLocation)startBentLocation;
	
	int nBentLoc = 0;
	double Loc = 0;
	ma_BentLocValue.clear();
	ar >> nBentLoc;
	for (int iBentLoc = 0; iBentLoc < nBentLoc; iBentLoc++)
	{
		ar >> Loc;
		ma_BentLocValue.push_back(Loc);
	}

	GM2DLineVector* pLineVec = &m_RebarZone;
	GM2DLineVector::Recovery(ar, pLineVec);
	pLineVec = &m_RebarLine;
	GM2DLineVector::Recovery(ar, pLineVec);
	m_BentLine.Recovery(ar);
}

void MSBentRebar::Serialize(CArchive &ar)
{
	MSLineRebar::Serialize(ar);
	int bentType = 0, startBentLocation = 0;
	if (ar.IsStoring())
	{
		bentType = (int)m_eBentType;
		startBentLocation = (int)m_eStartBentLocation;

		ar << bentType;
		ar << startBentLocation;

		int nBentLoc = ma_BentLocValue.size();
		double Loc = 0;
		ar << nBentLoc;
		for (int iBentLoc = 0; iBentLoc < nBentLoc; iBentLoc++)
		{
			Loc = ma_BentLocValue[iBentLoc];
			ar << Loc;
		}

		m_RebarZone.Serialize(ar);
		m_RebarLine.Serialize(ar);
		m_BentLine.Serialize(ar);
	}
	else
	{
		ar >> bentType;
		ar >> startBentLocation;

		m_eBentType = (eBentType)bentType;
		m_eStartBentLocation = (eStartBentLocation)startBentLocation;

		int nBentLoc = 0;
		double Loc = 0;
		ma_BentLocValue.clear();
		ar >> nBentLoc;
		for (int iBentLoc = 0; iBentLoc < nBentLoc; iBentLoc++)
		{
			ar >> Loc;
			ma_BentLocValue.push_back(Loc);
		}

		m_RebarZone.Serialize(ar);
		m_RebarLine.Serialize(ar);
		m_BentLine.Serialize(ar);
	}
}

void MSBentRebar::SetStartBentLocation(eStartBentLocation startBentLocation)
{
	m_eStartBentLocation = startBentLocation;
}

vector<double> MSBentRebar::GetBentLocValue()
{
	return ma_BentLocValue;
}

void MSBentRebar::SetBentLocValue(vector<double> Locs)
{
	ma_BentLocValue.clear();
	ma_BentLocValue = Locs;
}

vector<GM2DVector> MSBentRebar::GetBentPoints()
{
	vector<GM2DVector> bentPoints;
	GM2DLineVector offsetRebarLine = m_RebarLine.NormalTranslate(-_BENT_OFFSET / 2);
	double length = offsetRebarLine.Length();
	long nLoc = ma_BentLocValue.size();
	for (long iLoc = 0; iLoc < nLoc; iLoc++)
	{
		double bentLength = ma_BentLocValue[iLoc];
		GM2DVector locVec = offsetRebarLine.PointVector(bentLength / length);
		bentPoints.push_back(locVec);
	}
	return bentPoints;
}

GM2DVector MSBentRebar::GetRebarOrigin()
{
	GM2DVector RebarOrigin;
	if (m_pAddRebarAreaBent == nullptr)
		return RebarOrigin;
	GM2DPolyline* pProfile = m_pAddRebarAreaBent->GetProfile();
	GM2DVector CenVec = pProfile->MassCenter();
	double dDeg = GetDegree();
	
	GM2DLineVector TheLine = pProfile->GetLineVectorOnXY(dDeg, CenVec);
	RebarOrigin = TheLine.m_P0;
	return RebarOrigin;
}

void MSBentRebar::CalculateBentLine()
{
	if (m_pAddRebarAreaBent == nullptr)
		return;

	GM2DPolyline* pProfile = m_pAddRebarAreaBent->GetProfile();
	m_BentLine.ResetPolyline();
	GM2DVector NormalVec = GetNormal();
	GM2DVector normalUnitVec = NormalVec.UnitVector();

	GM2DVector StartVec = GetRebarOrigin();

	if(m_eStartBentLocation == eStartBentLocation::Bot)
 		normalUnitVec.ReverseDirection();
	GM2DVector* pRebarOrigin = new GM2DVector(StartVec);
	m_BentLine.Add(pRebarOrigin);
	m_BentLine.ma_Bulge.Add(0.0); ////

	GM2DVector DirectionVec = GetDirection();

	double previous = 0.0;
	double offset = _BENT_OFFSET; //
	for (int i = 0; i < ma_BentLocValue.size(); i++)
	{
		double location = ma_BentLocValue[i];
		double length = location - previous - (offset / 2);
		m_BentLine.Add(new GM2DVector(*(m_BentLine.GetAt(m_BentLine.GetSize() - 1)) + DirectionVec *length));
		m_BentLine.ma_Bulge.Add(0.0); ////
		previous = location + (offset / 2);

		normalUnitVec.ReverseDirection();
		m_BentLine.Add(new GM2DVector(*(m_BentLine.GetAt(m_BentLine.GetSize() - 1)) + (DirectionVec + normalUnitVec) *offset));
		m_BentLine.ma_Bulge.Add(0.0); ////
	}

	// find end point
	GM2DVector* pCrossPt = pProfile->GetFirstIntersectedPoint(*(m_BentLine.GetAt(m_BentLine.GetSize() - 1)), DirectionVec);
	if (pCrossPt != NULL)
		m_BentLine.Add(pCrossPt);

	// rebar line
	pCrossPt = pProfile->GetFirstIntersectedPoint(StartVec, DirectionVec);
	if (pCrossPt != NULL)
	{
		m_RebarLine.m_P0 = StartVec;
		m_RebarLine.m_P1 = *pCrossPt;
		delete pCrossPt;
	}

	// info line
	GM2DVector* pCrossPt1 = NULL;
	GM2DVector* pCrossPt2 = NULL;
	GM2DVector midPt = m_RebarLine.PointAtParam(0.5);
	pCrossPt1 = pProfile->GetFirstIntersectedPoint(midPt, NormalVec);
	pCrossPt2 = pProfile->GetFirstIntersectedPoint(midPt, NormalVec*(-1.0));

	m_RebarZone.m_P0 = *pCrossPt1;
	m_RebarZone.m_P1 = *pCrossPt2;
	delete pCrossPt1;
	delete pCrossPt2;
}

GM2DLineVector MSBentRebar::GetRebarCenterLine()
{
	GM2DLineVector cenline = m_RebarLine.NormalTranslate(-_BENT_OFFSET / 2);
	return cenline;
}

void MSBentRebar::SetOwner(MSAddRebarAreaBent* pOwner)
{
	m_pAddRebarAreaBent = pOwner;
}

MSAddRebarAreaBent* MSBentRebar::GetOwner()
{
	return m_pAddRebarAreaBent;
}

double MSBentRebar::GetDegree()
{
	if (m_pAddRebarAreaBent != nullptr)
	{
		if (m_pAddRebarAreaBent->GetMainBar() == this)
			return m_pAddRebarAreaBent->GetPlacingRebarDegree();
		else if (m_pAddRebarAreaBent->GetSubBar() == this)
			return m_pAddRebarAreaBent->GetPlacingRebarDegree() + 90;
	}

	ASSERT(0);
	return 0;
}

GM2DVector MSBentRebar::GetNormal()
{
	return GM2DVector(GM2DVector(0, 0), 1.0, Deg2Rad*(90 + GetDegree()));
}

GM2DVector MSBentRebar::GetDirection()
{
	return GM2DVector(GM2DVector(0, 0), 1.0, Deg2Rad*GetDegree());
}

bool MSBentRebar::CheckBentLocation(GM2DPolyline* profile)
{
	double offset = _BENT_OFFSET; //
	GM2DPolyline offsetProfile;
	profile->OffSet_Acis(offset, false, offsetProfile);

	GM2DVector StartVec = GetRebarOrigin();

	GM2DVector DirectionVec = GetDirection();

	bool IsError = false;
	for (int i = 0; i < ma_BentLocValue.size(); i++)
	{
		double location = ma_BentLocValue[i];
		GM2DVector* bentLoc = new GM2DVector(StartVec + DirectionVec *location);
		if (offsetProfile.Contains(bentLoc) > 0)
			return false;
	}
	return true;
}

