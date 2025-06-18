#include "StdAfx.h"
#include "MSSpacerShape.h"

#include "GMLib\MSVersionInfo.h"
#include "MSDeformedBar.h"

IMPLEMENT_SERIAL(MSSpacerShape, MSElement, VERSIONABLE_SCHEMA | 1)

MSSpacerShape::MSSpacerShape(void)
{
	Init();
}


MSSpacerShape::~MSSpacerShape(void)
{
}

void MSSpacerShape::Init()
{
	m_Name = _T("Default");
	m_G = 6; m_S = 6; 
	m_B = 300;
	m_C = 400;
	m_D = 200;
	m_E = 400;
	m_F = 300;
	m_DiaName = MSDeformedBar::GetDefaultRebarName();
	m_XSpacing = 1000;
	m_YSpacing = 1000;
}

MSObject* MSSpacerShape::Copy( bool bNewID /*= true*/ )
{
	MSSpacerShape* pNewObj;

	pNewObj = new MSSpacerShape();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSSpacerShape::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/ )
{
	if(pSource_in == nullptr) return;
	MSElement::CopyFromMe(pSource_in, bNewID);

	MSSpacerShape* Source = (MSSpacerShape*)pSource_in;
	m_B = Source->m_B;
	m_C = Source->m_C;
	m_D = Source->m_D;
	m_E = Source->m_E;
	m_F = Source->m_F;
	m_G = Source->m_G;
	m_S = Source->m_S;

	m_Name = Source->m_Name;
	m_DiaName = Source->m_DiaName;
	m_XSpacing = Source->m_XSpacing;
	m_YSpacing = Source->m_YSpacing;
}

void MSSpacerShape::Dump(EFS::EFS_Buffer& buffer)
{
	MSElement::Dump(buffer);
	buffer << m_Name << m_DiaName << m_XSpacing << m_YSpacing;
	buffer << m_B	 << m_C	 << m_D << m_E << m_F << m_G << m_S	;
}

void MSSpacerShape::Recovery(EFS::EFS_Buffer& buffer)
{
	MSElement::Recovery(buffer);
	buffer >> m_Name >> m_DiaName >> m_XSpacing >> m_YSpacing;
	buffer >> m_B	 >> m_C	 >> m_D >> m_E >> m_F >> m_G >> m_S	;
}

void MSSpacerShape::Serialize( CArchive& ar)
{
	MSElement::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_Name;
		ar << m_DiaName;
		ar << m_XSpacing;
		ar << m_YSpacing;
		ar << m_G;
		ar << m_S;
		ar << m_B;
		ar << m_C;
		ar << m_D;
		ar << m_E;
		ar << m_F;
	}
	else
	{
		ar >> m_Name;
		int nDia = 0;
		if(MSVersionInfo::GetCurrentVersion() < 20160108)
		{
			ar >> nDia;
			m_DiaName.Format(_T("%ld"), nDia);
		}
		else
			ar >> m_DiaName;
		ar >> m_XSpacing;
		ar >> m_YSpacing;
		ar >> m_G;
		ar >> m_S;
		ar >> m_B;
		ar >> m_C;
		ar >> m_D;
		ar >> m_E;
		ar >> m_F;

		if(m_DiaName == _T(""))
			m_DiaName = MSDeformedBar::GetDefaultRebarName();
	}
}