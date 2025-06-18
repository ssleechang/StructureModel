#include "StdAfx.h"
#include "MSSpecificTieBar.h"
#include "..\GMLib\MSVersionInfo.h"


IMPLEMENT_SERIAL(MSSpecificTieBar, MSRcObject, VERSIONABLE_SCHEMA | 1)
MSSpecificTieBar::MSSpecificTieBar(void)
{
	m_sLoc = _T("");
	m_dDia = 0;
	m_bUserSteelGradeS = false;
}

MSSpecificTieBar::~MSSpecificTieBar(void)
{
}

void MSSpecificTieBar::CopyFromMe(MSSpecificTieBar* pSource)
{
	if(pSource==NULL)	return; 

	m_sLoc = pSource->m_sLoc;
	m_dDia = pSource->m_dDia;
	m_bUserSteelGradeS = pSource->GetUserSteelGradeS();
}

void MSSpecificTieBar::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);

	buffer << m_sLoc;
	buffer << m_dDia;
	buffer << m_bUserSteelGradeS;
}

void MSSpecificTieBar::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);

	buffer >> m_sLoc;
	buffer >> m_dDia;
	buffer >> m_bUserSteelGradeS;
}

void MSSpecificTieBar::Serialize(CArchive &ar)
{
	MSRcObject::Serialize(ar);
	CString strBar;
	if(ar.IsStoring())
	{
		ar << m_sLoc;
		ar << m_dDia;
		ar << m_bUserSteelGradeS;
	}
	else
	{
		ar >> m_sLoc;

		if (MSVersionInfo::GetCurrentVersion() >= 20200904)
		{
			ar >> m_dDia;
		}
		if(IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231010)
		{
			ar >> m_bUserSteelGradeS;
		}

	}
}

// Edge는 반시계 방향으로, Bottom -> Right -> Top -> Left순서
bool MSSpecificTieBar::AnalyizeTieBarByRecoginze(int StartEdge, int StartRebar, int EndEdge, int EndRebar)
{
	if(StartEdge == EndEdge && StartRebar == EndRebar)	return false;
	
	CString sStartKey = GetEdgeKeyName(StartEdge);
	CString sEndKey = GetEdgeKeyName(EndEdge);
	if(sStartKey ==_T("") || sEndKey == _T(""))	return false;

	m_sLoc.Format(_T("%s%ld-%s%ld"), sStartKey, StartRebar, sEndKey, EndRebar);
	return true;
}

CString MSSpecificTieBar::GetEdgeKeyName(int EdgeIndex)
{
	switch((MSSpecificTieBar::eEdge)EdgeIndex)
	{
	case MSSpecificTieBar::Bottom:	return _T("B");
	case MSSpecificTieBar::Right:		return _T("R");
	case MSSpecificTieBar::Top:			return _T("T");
	case MSSpecificTieBar::Left:			return _T("L");
	case MSSpecificTieBar::Circle:		return _T("C");
	}
	return _T("");
}

bool MSSpecificTieBar::GetUserSteelGradeS()
{
	return m_bUserSteelGradeS;
}
void MSSpecificTieBar::SetUserSteelGradeS(bool value)
{
	m_bUserSteelGradeS = value;
}