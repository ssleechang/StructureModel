#include "stdafx.h"
#include "ViewDisplaySetting.h"
IMPLEMENT_SERIAL(ViewDisplaySetting, MSObject, VERSIONABLE_SCHEMA | 1)

ViewDisplaySetting::ViewDisplaySetting()
{
	m_bReassignView2D = false;
	m_LineColor2D = RGB(255, 255, 255);
	m_LineTypeName2D = _T("SOLID");
	m_LineWeight2D = 0;
	m_HatchPatternName2D = _T("INVISIBLE");
	m_HatchColor2D = RGB(255, 255, 255);
	m_HatchScale2D = 0;
	m_AlphaBlending2D = 100;

	m_bReassignView3D = false;
	m_PatternColor3D = RGB(255, 255, 255);
	m_AlphaBlending3D = 100;
}

ViewDisplaySetting::~ViewDisplaySetting()
{
}

MSObject* ViewDisplaySetting::Copy(bool bNewID /*= true*/)
{
	ViewDisplaySetting* pNewObj;

	pNewObj = new ViewDisplaySetting();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void ViewDisplaySetting::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;

	ViewDisplaySetting* pSource = (ViewDisplaySetting*)pSource_in;

	m_bReassignView2D = pSource->m_bReassignView2D;
	m_LineColor2D = pSource->m_LineColor2D;
	m_LineTypeName2D = pSource->m_LineTypeName2D;
	m_LineWeight2D = pSource->m_LineWeight2D;
	m_HatchPatternName2D = pSource->m_HatchPatternName2D;
	m_HatchColor2D = pSource->m_HatchColor2D;
	m_HatchScale2D = pSource->m_HatchScale2D;
	m_AlphaBlending2D = pSource->m_AlphaBlending2D;

	m_bReassignView3D = pSource->m_bReassignView3D;
	m_PatternColor3D = pSource->m_PatternColor3D;
	m_AlphaBlending3D = pSource->m_AlphaBlending3D;

	MSObject::CopyFromMe(pSource, bNewID);
}
void ViewDisplaySetting::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	buffer << m_bReassignView2D << m_LineColor2D << m_LineTypeName2D << m_LineWeight2D << m_HatchPatternName2D << m_HatchColor2D << m_HatchScale2D << m_AlphaBlending2D;
	buffer << m_bReassignView3D << m_PatternColor3D << m_AlphaBlending3D;
}

void ViewDisplaySetting::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	buffer >> m_bReassignView2D >> m_LineColor2D >> m_LineTypeName2D >> m_LineWeight2D >> m_HatchPatternName2D >> m_HatchColor2D >> m_HatchScale2D >> m_AlphaBlending2D;
	buffer >> m_bReassignView3D >> m_PatternColor3D >> m_AlphaBlending3D;
}

void ViewDisplaySetting::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_bReassignView2D;
		ar << m_LineColor2D;
		ar << m_LineTypeName2D;
		ar << m_LineWeight2D;
		ar << m_HatchPatternName2D;
        ar << m_HatchColor2D;
        ar << m_HatchScale2D;
        ar << m_AlphaBlending2D;
		ar << m_bReassignView3D;
		ar << m_PatternColor3D;
		ar << m_AlphaBlending3D;
	}
	else
	{
		ar >> m_bReassignView2D;
		ar >> m_LineColor2D;
		ar >> m_LineTypeName2D;
		ar >> m_LineWeight2D;
		ar >> m_HatchPatternName2D;
		ar >> m_HatchColor2D;
		ar >> m_HatchScale2D;
		ar >> m_AlphaBlending2D;
		ar >> m_bReassignView3D;
		ar >> m_PatternColor3D;
		ar >> m_AlphaBlending3D;
	}
}

bool ViewDisplaySetting::GetReassignView2D()
{
	return m_bReassignView2D;
}
void ViewDisplaySetting::SetReassignView2D(bool bReassignView2D)
{
	m_bReassignView2D = bReassignView2D;
}

void ViewDisplaySetting::GetLineColor2D(int& red, int& green, int&blue)
{
	red = GetRValue(m_LineColor2D);
	green = GetGValue(m_LineColor2D);
	blue = GetBValue(m_LineColor2D);
}
void ViewDisplaySetting::SetLineColor2D(int red, int green, int blue)
{
	m_LineColor2D = RGB(red, green, blue);
}

CString ViewDisplaySetting::GetLineTypeName2D()
{
	return m_LineTypeName2D;
}
void ViewDisplaySetting::SetLineTypeName2D(CString lineTypeName2D)
{
	m_LineTypeName2D = lineTypeName2D;
}

int ViewDisplaySetting::GetLineWeight2D()
{
	return m_LineWeight2D;
}
void ViewDisplaySetting::SetLineWeight2D(int lineWeight2D)
{
	m_LineWeight2D = lineWeight2D;
}

CString ViewDisplaySetting::GetHatchPatternName2D()
{
	return m_HatchPatternName2D;
}
void ViewDisplaySetting::SetHatchPatternName2D(CString hatchPatternName2D)
{
	m_HatchPatternName2D = hatchPatternName2D;
}

void ViewDisplaySetting::GetHatchColor2D(int& red, int& green, int& blue)
{
	red = GetRValue(m_HatchColor2D);
	green = GetGValue(m_HatchColor2D);
	blue = GetBValue(m_HatchColor2D);
}
void ViewDisplaySetting::SetHatchColor2D(int red, int green, int blue)
{
	m_HatchColor2D = RGB(red, green, blue);
}

double ViewDisplaySetting::GetHatchScale2D()
{
	return m_HatchScale2D;
}
void ViewDisplaySetting::SetHatchScale2D(double hatchScale2D)
{
	m_HatchScale2D = hatchScale2D;
}

double ViewDisplaySetting::GetAlphaBlending2D()
{
	return m_AlphaBlending2D;
}
void ViewDisplaySetting::SetAlphaBlending2D(double alphaBlending2D)
{
	m_AlphaBlending2D = alphaBlending2D;
}

bool ViewDisplaySetting::GetReassignView3D()
{
	return m_bReassignView3D;
}
void ViewDisplaySetting::SetReassignView3D(bool bReassignView3D)
{
	m_bReassignView3D = bReassignView3D;
}

void ViewDisplaySetting::GetPatternColor3D(int& red, int& green, int& blue)
{
	red = GetRValue(m_PatternColor3D);
	green = GetGValue(m_PatternColor3D);
	blue = GetBValue(m_PatternColor3D);
}
void ViewDisplaySetting::SetPatternColor3D(int red, int green, int blue)
{
	m_PatternColor3D = RGB(red, green, blue);
}

double ViewDisplaySetting::GetAlphaBlending3D()
{
	return m_AlphaBlending3D;
}
void ViewDisplaySetting::SetAlphaBlending3D(double alphaBlending3D)
{
	m_AlphaBlending3D = alphaBlending3D;
}