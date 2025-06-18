#include "stdafx.h"
#include "MSArea.h"

IMPLEMENT_SERIAL(MSArea, MSElement, VERSIONABLE_SCHEMA | 1)
MSArea::MSArea(void)
{
	m_eType = MSArea::Added;
	m_FloorNum = 0;
	m_Name = _T("");
	m_pProfile = new GM2DPolyline();
}

MSArea::~MSArea(void)
{
	delete m_pProfile;
}

MSObject* MSArea::Copy(bool bNewID /*= true*/)
{
	MSArea* pNewObj = new MSArea();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSArea::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;
	MSArea* pSource = dynamic_cast<MSArea*>(pSource_in);
	if (pSource == nullptr)	return;

	m_eType = pSource->GetAreaType();
	m_FloorNum = pSource->GetFloorNum();
	m_Name = pSource->GetName();
	m_pProfile->ResetPolyline();
	m_pProfile->CopyFrom(pSource->GetProfile());
	MSElement::CopyFromMe(pSource);
}

void MSArea::Serialize(CArchive &ar)
{
	MSElement::Serialize(ar);
	int areaType = 0;
	if (ar.IsStoring())
	{
		areaType = (int)m_eType;
		ar << areaType;
		ar << m_FloorNum;
		ar << m_Name;
		m_pProfile->Serialize(ar);
	}
	else
	{
		ar >> areaType;
		m_eType = (MSArea::eAreaType)areaType;
		ar >> m_FloorNum;
		ar >> m_Name;
		m_pProfile->Serialize(ar);
	}
}
void MSArea::Dump(EFS::EFS_Buffer& buffer)
{
	MSElement::Dump(buffer);
	int areaType = (int)m_eType;
	buffer << areaType;
	buffer << m_FloorNum;
	buffer << m_Name;
	m_pProfile->Dump(buffer);
}

void MSArea::Recovery(EFS::EFS_Buffer& buffer)
{
	MSElement::Recovery(buffer);
	int areaType = 0;
	buffer >> areaType;
	m_eType = (MSArea::eAreaType)areaType;
	buffer >> m_FloorNum;
	buffer >> m_Name;
	m_pProfile->Recovery(buffer);
}

void MSArea::SetProfile(GM2DPolyline* profile)
{
	m_pProfile->ResetPolyline();
	m_pProfile->CopyFrom(profile);
}
double MSArea::GetArea_ACIS()
{
	return m_pProfile->Area_ACIS();
}
