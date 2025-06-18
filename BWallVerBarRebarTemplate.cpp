#include "stdafx.h"
#include "BWallVerBarRebarTemplate.h"
#include "..\GMLib\MSVersionInfo.h"


BWallVerBarRebarTemplate::BWallVerBarRebarTemplate()
{
	DataInit();
}


BWallVerBarRebarTemplate::~BWallVerBarRebarTemplate()
{
}

void BWallVerBarRebarTemplate::DataInit()
{
	m_nBarSpliceType = 0;
	m_nTopLenType = 0;
	m_nTopFloorDevType = 2;
	m_nConstructionHeight = 300;
	m_bAlterSpliceSet = false;
	m_nAlterSpliceDist = 400;
	m_nAlterSpliceType = 0;
}
void BWallVerBarRebarTemplate::CopyFromMe(BWallVerBarRebarTemplate* pRebarTemplate)
{
	m_nBarSpliceType = pRebarTemplate->GetBarSpliceType();
	m_nTopLenType = pRebarTemplate->GetTopLenType();
	m_nTopFloorDevType = pRebarTemplate->GetTopFloorDevType();
	m_nConstructionHeight = pRebarTemplate->GetConstructionHeight();
	m_bAlterSpliceSet = pRebarTemplate->GetAlterSpliceSet();
	m_nAlterSpliceDist = pRebarTemplate->GetAlterSpliceDist();
	m_nAlterSpliceType = pRebarTemplate->GetAlterSpliceType();
}
void BWallVerBarRebarTemplate::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_nBarSpliceType;
		ar << m_nTopLenType;
		ar << m_nTopFloorDevType;
		ar << m_nConstructionHeight;
		ar << m_bAlterSpliceSet;
		ar << m_nAlterSpliceDist;
		ar << m_nAlterSpliceType;
	}
	else
	{
		ar >> m_nBarSpliceType;
		ar >> m_nTopLenType;
		ar >> m_nTopFloorDevType;
		ar >> m_nConstructionHeight;
		ar >> m_bAlterSpliceSet;
		ar >> m_nAlterSpliceDist;
		if (MSVersionInfo::GetCurrentVersion() >= 20250219)
			ar >> m_nAlterSpliceType;
		else
			m_nAlterSpliceType = 1;
	}
}

long BWallVerBarRebarTemplate::GetBarSpliceType()
{
	return m_nBarSpliceType;
}

void BWallVerBarRebarTemplate::SetBarSpliceType(long value)
{
	m_nBarSpliceType = value;
}

long BWallVerBarRebarTemplate::GetTopLenType()
{
	return m_nTopLenType;
}

void BWallVerBarRebarTemplate::SetTopLenType(long value)
{
	m_nTopLenType = value;
}

long BWallVerBarRebarTemplate::GetTopFloorDevType()
{
	return m_nTopFloorDevType;
}

void BWallVerBarRebarTemplate::SetTopFloorDevType(long value)
{
	m_nTopFloorDevType = value;
}

long BWallVerBarRebarTemplate::GetConstructionHeight()
{
	return m_nConstructionHeight;
}

void BWallVerBarRebarTemplate::SetConstructionHeight(long value)
{
	m_nConstructionHeight = value;
}

BOOL BWallVerBarRebarTemplate::GetAlterSpliceSet()
{
	return m_bAlterSpliceSet;
}

void BWallVerBarRebarTemplate::SetAlterSpliceSet(BOOL value)
{
	m_bAlterSpliceSet = value;
}

long BWallVerBarRebarTemplate::GetAlterSpliceDist()
{
	return m_nAlterSpliceDist;
}

void BWallVerBarRebarTemplate::SetAlterSpliceDist(long value)
{
	m_nAlterSpliceDist = value;
}

long BWallVerBarRebarTemplate::GetAlterSpliceType()
{
	return m_nAlterSpliceType;
}

void BWallVerBarRebarTemplate::SetAlterSpliceType(long value)
{
	m_nAlterSpliceType = value;
}