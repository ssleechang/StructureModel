#include "stdafx.h"
#include "ColumnVerBarRebarTemplate.h"
#include "..\GMLib\MSVersionInfo.h"

ColumnVerBarRebarTemplate::ColumnVerBarRebarTemplate()
{
	DataInit();
}


ColumnVerBarRebarTemplate::~ColumnVerBarRebarTemplate()
{
}

void ColumnVerBarRebarTemplate::DataInit()
{
	m_nBarSpliceType = 0;
	m_nTopLenType = 0;
	m_nTopFloorDevType = 2;
	m_nConstructionHeight = 300;
	m_bAlterSpliceSet = FALSE;
	m_nAlterSpliceDist = 400;
	m_nAlterSpliceType = 0;
}
void ColumnVerBarRebarTemplate::CopyFromMe(ColumnVerBarRebarTemplate* pColumnRebarTemplate)
{
	m_nBarSpliceType = pColumnRebarTemplate->GetBarSpliceType();
	m_nTopLenType = pColumnRebarTemplate->GetTopLenType();
	m_nTopFloorDevType = pColumnRebarTemplate->GetTopFloorDevType();
	m_nConstructionHeight = pColumnRebarTemplate->GetConstructionHeight();
	m_bAlterSpliceSet = pColumnRebarTemplate->GetAlterSpliceSet();
	m_nAlterSpliceDist = pColumnRebarTemplate->GetAlterSpliceDist();
	m_nAlterSpliceType = pColumnRebarTemplate->GetAlterSpliceType();
}
void ColumnVerBarRebarTemplate::Serialize(CArchive& ar)
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

long ColumnVerBarRebarTemplate::GetBarSpliceType()
{
	return m_nBarSpliceType;
}

void ColumnVerBarRebarTemplate::SetBarSpliceType(long value)
{
	m_nBarSpliceType = value;
}

long ColumnVerBarRebarTemplate::GetTopLenType()
{
	return m_nTopLenType;
}

void ColumnVerBarRebarTemplate::SetTopLenType(long value)
{
	m_nTopLenType = value;
}

long ColumnVerBarRebarTemplate::GetTopFloorDevType()
{
	return m_nTopFloorDevType;
}

void ColumnVerBarRebarTemplate::SetTopFloorDevType(long value)
{
	m_nTopFloorDevType = value;
}

long ColumnVerBarRebarTemplate::GetConstructionHeight()
{
	return m_nConstructionHeight;
}

void ColumnVerBarRebarTemplate::SetConstructionHeight(long value)
{
	m_nConstructionHeight = value;
}

BOOL ColumnVerBarRebarTemplate::GetAlterSpliceSet()
{
	return m_bAlterSpliceSet;
}

void ColumnVerBarRebarTemplate::SetAlterSpliceSet(BOOL value)
{
	m_bAlterSpliceSet = value;
}

long ColumnVerBarRebarTemplate::GetAlterSpliceDist()
{
	return m_nAlterSpliceDist;
}

void ColumnVerBarRebarTemplate::SetAlterSpliceDist(long value)
{
	m_nAlterSpliceDist = value;
}

long ColumnVerBarRebarTemplate::GetAlterSpliceType()
{
	return m_nAlterSpliceType;
}

void ColumnVerBarRebarTemplate::SetAlterSpliceType(long value)
{
	m_nAlterSpliceType = value;
}