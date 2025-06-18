#include "stdafx.h"
#include "MSCustomReBarLine.h"
#include "MSCustomSection.h"
#include "GMLib/GM2DCurve.h"
#include "GMLib/MSVersionInfo.h"


long MSCustomReBarLine::ms_LastID = 0;

IMPLEMENT_SERIAL(MSCustomReBarLine, MSRcObject, VERSIONABLE_SCHEMA | 1)

MSCustomReBarLine::MSCustomReBarLine()
{
	m_ID = 0;
	m_EdgeIndex = -1;
	m_OffsetType = MSCustomReBarLine::Cover;
	m_UserOffset = 0;
	m_pCustomSection = nullptr;
	m_eCoverType = eCoverLengthFace::CL_SIDE_FACE;
}

MSCustomReBarLine::MSCustomReBarLine(MSCustomSection* pCustomSection)
{
	m_ID = 0;
	m_EdgeIndex = -1;
	m_OffsetType = MSCustomReBarLine::Cover;
	m_UserOffset = 0;
	m_pCustomSection = pCustomSection;
	m_eCoverType = eCoverLengthFace::CL_SIDE_FACE;
}

MSCustomReBarLine::~MSCustomReBarLine()
{
}

long MSCustomReBarLine::GetID()
{
	if (m_ID == 0) // 
	{
		m_ID = ms_LastID + 1;
		ms_LastID = m_ID;
		return m_ID;
	}

	return m_ID;
}

void MSCustomReBarLine::SetLastID(long NewID)
{
	if (NewID > ms_LastID)
		ms_LastID = NewID;
}

void MSCustomReBarLine::CopyFromMe(MSCustomReBarLine* pSource)
{
	if (pSource == NULL)	return;

	m_EdgeIndex = pSource->GetEdgeIndex();
	m_OffsetType = pSource->GetOffsetType();
	m_UserOffset = pSource->GetUserOffset();
	m_eCoverType = pSource->GetCoverType();
	MSRcObject::CopyFromMe(pSource);
}

void MSCustomReBarLine::Dump(EFS::EFS_Buffer& buffer)
{
	MSRcObject::Dump(buffer);
	buffer << m_ID;
	int offsetType = (int)m_OffsetType;
	buffer << m_EdgeIndex << offsetType << m_UserOffset;

	int coverType = (int)m_eCoverType;
	buffer << coverType;
}

void MSCustomReBarLine::Recovery(EFS::EFS_Buffer& buffer)
{
	MSRcObject::Recovery(buffer);
	buffer >> m_ID;
	int offsetType = 0;
	buffer >> m_EdgeIndex >> offsetType >> m_UserOffset;
	m_OffsetType = (eOffsetType)offsetType;

	int coverType = 0;
	buffer >> coverType;
	m_eCoverType = (eCoverLengthFace)coverType;
}

void MSCustomReBarLine::Serialize(CArchive &ar)
{
	MSRcObject::Serialize(ar);
	int offsetType = 0, coverType = 0;
	if (ar.IsStoring())
	{
		offsetType = (int)m_OffsetType;
		ar << m_ID;
		ar << m_EdgeIndex;
		ar << offsetType;
		ar << m_UserOffset;
		coverType = (int)m_eCoverType;
		ar << coverType;
	}
	else
	{
		ar >> m_ID;
		ar >> m_EdgeIndex;
		ar >> offsetType;
		ar >> m_UserOffset;
		m_OffsetType = (eOffsetType)offsetType;

		if (MSVersionInfo::GetCurrentVersion() >= 20171212)
		{
			ar >> coverType;
			m_eCoverType = (eCoverLengthFace)coverType;
		}

		SetLastID(m_ID);
	}
}

double MSCustomReBarLine::GetDefCover()
{
	return 40.0;
}

double MSCustomReBarLine::GetOffSet()
{
	if (m_OffsetType == MSCustomReBarLine::User)
		return m_UserOffset;

	return GetDefCover();
}

GM2DCurve* MSCustomReBarLine::GetCurve()
{
	if (m_pCustomSection == nullptr)
		return nullptr;

	GM2DPolyline* pPoly = m_pCustomSection->GetProfile();
	if (pPoly == nullptr || pPoly->GetNumLines() <= m_EdgeIndex)
		return nullptr;

	GM2DCurve* pCurve = pPoly->GetCurve(m_EdgeIndex);

	pCurve->NormalTranslateMe(GetOffSet());
	
	// pCurve가 기울어져서 Profile과 한점만 맞닿은 경우, 성공하지 못한다.
	GM2DVector MidVec = pCurve->PointAtParam(0.5);
	double extendLength = 100000;
	pCurve->ExtendLineVector(TRUE, extendLength , FALSE);
	pCurve->ExtendLineVector(FALSE, extendLength, TRUE);

	//pPoly->GetCurveInPolygon(pCurve);
	//return pCurve;
	vector<GM2DCurve*> NewCurves;
	pPoly->GetCurveInPolygon(pCurve, NewCurves);
	if (NewCurves.size() == 0)
		return nullptr;

	double dMinDist = pCurve->Length();
	int nMinIdx = 0;
	for (int i = 0; i < NewCurves.size(); i++)
	{
		double dDist = NewCurves[i]->PointAtParam(0.5).Distance(MidVec);
		if (dDist < dMinDist)
		{
			dMinDist = dDist;
			nMinIdx = i;
		}
	}

	pCurve->CopyFrom(NewCurves[nMinIdx]);

	GMObject::DeleteGMObject(NewCurves);

	return pCurve;
}

void MSCustomReBarLine::SetCustomSection(MSCustomSection* pCustomSection)
{
	m_pCustomSection = pCustomSection;
}

CString MSCustomReBarLine::GetConstraint()
{
	char BaseConstraint = 'A';
	char CurConstraint = BaseConstraint + m_EdgeIndex;
	CString strConstraint;
	strConstraint.Format(_T("%c"), CurConstraint);
	
	return strConstraint;
}

CString MSCustomReBarLine::GetName()
{
	CString name;
	name.Format(_T("%s-%ld"), GetConstraint(), ROUND(GetOffSet()));
	return name;
}

MSBaseBuilding* MSCustomReBarLine::GetBuilding()
{
	return GetCustomSection()->GetBuilding(); 
}
