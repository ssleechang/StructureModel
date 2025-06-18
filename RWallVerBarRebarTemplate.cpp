#include "stdafx.h"
#include "RWallVerBarRebarTemplate.h"
#include "..\GMLib\MSVersionInfo.h"

RWallVerBarRebarTemplate::RWallVerBarRebarTemplate()
{
	DataInit();
}


RWallVerBarRebarTemplate::~RWallVerBarRebarTemplate()
{
}

void RWallVerBarRebarTemplate::DataInit()
{
	m_nBarSpliceType = 0;
	m_nTopLenType = 0;
	m_nTopFloorDevType = 2;
	m_nConstructionHeight = 300;
	m_bAlterSpliceSet = FALSE;
	m_nAlterSpliceDist = 400;
	m_nAlterSpliceType = 0;
}
void RWallVerBarRebarTemplate::CopyFromMe(RWallVerBarRebarTemplate* pRebarTemplate)
{
	m_nBarSpliceType = pRebarTemplate->GetBarSpliceType();
	m_nTopLenType = pRebarTemplate->GetTopLenType();
	m_nTopFloorDevType = pRebarTemplate->GetTopFloorDevType();
	m_nConstructionHeight = pRebarTemplate->GetConstructionHeight();
	m_bAlterSpliceSet = pRebarTemplate->GetAlterSpliceSet();
	m_nAlterSpliceDist = pRebarTemplate->GetAlterSpliceDist();
	m_nAlterSpliceType = pRebarTemplate->GetAlterSpliceType();
}
void RWallVerBarRebarTemplate::Serialize(CArchive& ar)
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

long RWallVerBarRebarTemplate::GetBarSpliceType()
{
	return m_nBarSpliceType;
}

void RWallVerBarRebarTemplate::SetBarSpliceType(long value)
{
	m_nBarSpliceType = value;
}

long RWallVerBarRebarTemplate::GetTopLenType()
{
	return m_nTopLenType;
}

void RWallVerBarRebarTemplate::SetTopLenType(long value)
{
	m_nTopLenType = value;
}

long RWallVerBarRebarTemplate::GetTopFloorDevType()
{
	return m_nTopFloorDevType;
}

void RWallVerBarRebarTemplate::SetTopFloorDevType(long value)
{
	m_nTopFloorDevType = value;
}

long RWallVerBarRebarTemplate::GetConstructionHeight()
{
	return m_nConstructionHeight;
}

void RWallVerBarRebarTemplate::SetConstructionHeight(long value)
{
	m_nConstructionHeight = value;
}

BOOL RWallVerBarRebarTemplate::GetAlterSpliceSet()
{
	return m_bAlterSpliceSet;
}

void RWallVerBarRebarTemplate::SetAlterSpliceSet(BOOL value)
{
	m_bAlterSpliceSet = value;
}

long RWallVerBarRebarTemplate::GetAlterSpliceDist()
{
	return m_nAlterSpliceDist;
}

void RWallVerBarRebarTemplate::SetAlterSpliceDist(long value)
{
	m_nAlterSpliceDist = value;
}


long RWallVerBarRebarTemplate::GetAlterSpliceType()
{
	return m_nAlterSpliceType;
}

void RWallVerBarRebarTemplate::SetAlterSpliceType(long value)
{
	m_nAlterSpliceType = value;
}