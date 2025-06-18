#include "stdafx.h"
#include "MSBuildingInfoColorData.h"

IMPLEMENT_SERIAL(MSBuildingInfoColorData, MSElement, VERSIONABLE_SCHEMA | 1)

MSBuildingInfoColorData::MSBuildingInfoColorData(void)
{
	m_Alpha = 255;
	m_R = 255;
	m_G = 255;
	m_B = 255;
	m_bShow = true;
}

MSBuildingInfoColorData::MSBuildingInfoColorData(int alpha, int r, int g, int b, bool isShow)
{
	m_Alpha = alpha;
	m_R = r;
	m_G = g;
	m_B = b;
	m_bShow = true;
}

MSBuildingInfoColorData::~MSBuildingInfoColorData(void)
{
}

MSObject* MSBuildingInfoColorData::Copy(bool bNewID /*= true*/)
{
	MSBuildingInfoColorData* pNewObj = new MSBuildingInfoColorData();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSBuildingInfoColorData::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;

	MSBuildingInfoColorData* pSource = dynamic_cast<MSBuildingInfoColorData*>(pSource_in);
	if (pSource == nullptr)	return;

	m_Alpha = pSource->m_Alpha;
	m_R = pSource->m_R;
	m_G = pSource->m_G;
	m_B = pSource->m_B;
	m_bShow = pSource->m_bShow;
}

void MSBuildingInfoColorData::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_Alpha;
		ar << m_R;
		ar << m_G;
		ar << m_B;
		ar << m_bShow;
	}
	else
	{
		ar >> m_Alpha;
		ar >> m_R;
		ar >> m_G;
		ar >> m_B;
		ar >> m_bShow;
	}
}
void MSBuildingInfoColorData::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	buffer << m_Alpha;
	buffer << m_R;
	buffer << m_G;
	buffer << m_B;
	buffer << m_bShow;
}

void MSBuildingInfoColorData::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	buffer >> m_Alpha;
	buffer >> m_R;
	buffer >> m_G;
	buffer >> m_B;
	buffer >> m_bShow;
}

void MSBuildingInfoColorData::GetColor(int& alpha, int& red, int& green, int&blue)
{
	alpha = m_Alpha;
	red = m_R;
	green = m_G;
	blue = m_B;
}
void MSBuildingInfoColorData::SetColor(int alpha, int red, int green, int blue)
{
	m_Alpha = alpha;
	m_R = red;
	m_G = green;
	m_B = blue;
}

bool MSBuildingInfoColorData::GetShow()
{
	return this->m_bShow;
}
void MSBuildingInfoColorData::SetShow(bool bShow)
{
	this->m_bShow = bShow;
}

