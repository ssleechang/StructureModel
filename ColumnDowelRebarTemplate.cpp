#include "stdafx.h"
#include "ColumnDowelRebarTemplate.h"
#include "MSDeformedBar.h"
#include "BaseRebarTemplate.h"
#include "MSObject.h"
#include "..\GMLib\MSVersionInfo.h"

ColumnDowelRebarTemplate::ColumnDowelRebarTemplate()
{
	DataInit();
}


ColumnDowelRebarTemplate::~ColumnDowelRebarTemplate()
{
}

void ColumnDowelRebarTemplate::DataInit()
{
	m_nBarSpliceType = 0;
	m_nTopLenType = 0;
	m_nConstructionHeight = 300;
	m_bAlterSpliceSet = FALSE;
	m_nAlterSpliceDist = 400;
	m_nHoopRebarType = 1;
	m_nHoopBySpace = 1000;
	m_nHoopByNum = 3;
	m_nSupportBarDiaIndex = 2;
	m_nSupportBarLengthType = 2;
	m_nSupportBarLength = 1600;
	m_nSupportBarCorrectiveLength = 10;
	m_nSupportBarNum = 4;
	m_nBotPlaceType = 1;
	m_bFootingPercentSet = FALSE;
	m_nFootingPercent = 50;
	m_bFootingCoverSet = FALSE;
	m_nFootingCover = 100;
	m_bUserHookLenSet = FALSE;
	m_dSupportBarFootingThickMultiplicationValue = 2;
	m_bSupportBarUserSteelGradeS = FALSE;
	m_nAlterSpliceType = 0;
}
void ColumnDowelRebarTemplate::CopyFromMe(ColumnDowelRebarTemplate* pColumnRebarTemplate)
{
	m_nBarSpliceType = pColumnRebarTemplate->GetBarSpliceType();
	m_nTopLenType = pColumnRebarTemplate->GetTopLenType();
	m_nConstructionHeight = pColumnRebarTemplate->GetConstructionHeight();
	m_bAlterSpliceSet = pColumnRebarTemplate->GetAlterSpliceSet();
	m_nAlterSpliceDist = pColumnRebarTemplate->GetAlterSpliceDist();
	m_nHoopRebarType = pColumnRebarTemplate->GetHoopRebarType();
	m_nHoopBySpace = pColumnRebarTemplate->GetHoopBySpace();
	m_nHoopByNum = pColumnRebarTemplate->GetHoopByNum();
	m_nSupportBarDiaIndex = pColumnRebarTemplate->GetSupportBarDiaIndex();
	m_nSupportBarLengthType = pColumnRebarTemplate->GetSupportBarLengthType();
	m_nSupportBarLength = pColumnRebarTemplate->GetSupportBarLength();
	m_nSupportBarCorrectiveLength = pColumnRebarTemplate->GetSupportBarCorrectiveLength();
	m_nSupportBarNum = pColumnRebarTemplate->GetSupportBarNum();
	m_nBotPlaceType = pColumnRebarTemplate->GetBotPlaceType();
	m_bFootingPercentSet = pColumnRebarTemplate->GetFootingPercentSet();
	m_nFootingPercent = pColumnRebarTemplate->GetFootingPercent();
	m_bFootingCoverSet = pColumnRebarTemplate->GetFootingCoverSet();
	m_nFootingCover = pColumnRebarTemplate->GetFootingCover();
	m_bUserHookLenSet = pColumnRebarTemplate->GetUserHookLenSet();
	m_dSupportBarFootingThickMultiplicationValue = pColumnRebarTemplate->GetSupportBarFootingThickMultiplicationValue();
	m_bSupportBarUserSteelGradeS = pColumnRebarTemplate->GetSupportBarUserSteelGradeS();
	m_nAlterSpliceType = pColumnRebarTemplate->GetAlterSpliceType();
}
void ColumnDowelRebarTemplate::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_nBarSpliceType;
		ar << m_nTopLenType;
		ar << m_nConstructionHeight;
		ar << m_bAlterSpliceSet;
		ar << m_nAlterSpliceDist;
		ar << m_nHoopRebarType;
		ar << m_nHoopBySpace;
		ar << m_nHoopByNum;
		ar << m_nSupportBarDiaIndex;
		ar << m_nSupportBarLengthType;
		ar << m_nSupportBarLength;
		ar << m_nSupportBarCorrectiveLength;
		ar << m_nSupportBarNum;
		ar << m_nBotPlaceType;
		ar << m_bFootingPercentSet;
		ar << m_nFootingPercent;
		ar << m_bFootingCoverSet;
		ar << m_nFootingCover;
		ar << m_bUserHookLenSet;
		ar << m_dSupportBarFootingThickMultiplicationValue;
		ar << m_bSupportBarUserSteelGradeS;
		ar << m_nAlterSpliceType;
	}
	else
	{
		ar >> m_nBarSpliceType;
		ar >> m_nTopLenType;
		ar >> m_nConstructionHeight;
		ar >> m_bAlterSpliceSet;
		ar >> m_nAlterSpliceDist;
		ar >> m_nHoopRebarType;
		ar >> m_nHoopBySpace;
		ar >> m_nHoopByNum;
		ar >> m_nSupportBarDiaIndex;
		ar >> m_nSupportBarLengthType;
		ar >> m_nSupportBarLength;
		ar >> m_nSupportBarCorrectiveLength;
		ar >> m_nSupportBarNum;
		ar >> m_nBotPlaceType;
		ar >> m_bFootingPercentSet;
		ar >> m_nFootingPercent;
		ar >> m_bFootingCoverSet;
		ar >> m_nFootingCover;
		ar >> m_bUserHookLenSet;
		if(m_nFootingPercent > 100)
			m_nFootingPercent = 100;

		if (MSVersionInfo::GetCurrentVersion() >= 20230419)
		{
			if(MSVersionInfo::GetCurrentVersion() >= 20240905)
			{
				ar >> m_dSupportBarFootingThickMultiplicationValue;
			}
			else
			{
				long nSupportBarFootingThickMultiplicationValue = 0;
				ar >> nSupportBarFootingThickMultiplicationValue;
				m_dSupportBarFootingThickMultiplicationValue = nSupportBarFootingThickMultiplicationValue;
			}			
		}

		if(MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231010)
			ar >> m_bSupportBarUserSteelGradeS;


		if (MSVersionInfo::GetCurrentVersion() >= 20250219)
			ar >> m_nAlterSpliceType;
		else
			m_nAlterSpliceType = 1;
	}
}

long ColumnDowelRebarTemplate::GetBarSpliceType()
{
	return m_nBarSpliceType;
}

void ColumnDowelRebarTemplate::SetBarSpliceType(long value)
{
	m_nBarSpliceType = value;
}

long ColumnDowelRebarTemplate::GetTopLenType()
{
	return m_nTopLenType;
}

void ColumnDowelRebarTemplate::SetTopLenType(long value)
{
	m_nTopLenType = value;
}

long ColumnDowelRebarTemplate::GetConstructionHeight()
{
	return m_nConstructionHeight;
}

void ColumnDowelRebarTemplate::SetConstructionHeight(long value)
{
	m_nConstructionHeight = value;
}

BOOL ColumnDowelRebarTemplate::GetAlterSpliceSet()
{
	return m_bAlterSpliceSet;
}

void ColumnDowelRebarTemplate::SetAlterSpliceSet(BOOL value)
{
	m_bAlterSpliceSet = value;
}

long ColumnDowelRebarTemplate::GetAlterSpliceDist()
{
	return m_nAlterSpliceDist;
}

void ColumnDowelRebarTemplate::SetAlterSpliceDist(long value)
{
	m_nAlterSpliceDist = value;
}

long ColumnDowelRebarTemplate::GetHoopRebarType()
{
	return m_nHoopRebarType;
}

void ColumnDowelRebarTemplate::SetHoopRebarType(long value)
{
	m_nHoopRebarType = value;
}

long ColumnDowelRebarTemplate::GetHoopBySpace()
{
	return m_nHoopBySpace;
}

void ColumnDowelRebarTemplate::SetHoopBySpace(long value)
{
	m_nHoopBySpace = value;
}

long ColumnDowelRebarTemplate::GetHoopByNum()
{
	return m_nHoopByNum;
}

void ColumnDowelRebarTemplate::SetHoopByNum(long value)
{
	m_nHoopByNum = value;
}

long ColumnDowelRebarTemplate::GetSupportBarDiaIndex()
{
	return m_nSupportBarDiaIndex;
}

void ColumnDowelRebarTemplate::SetSupportBarDiaIndex(long value)
{
	m_nSupportBarDiaIndex = value;
}

long ColumnDowelRebarTemplate::GetSupportBarLengthType()
{
	return m_nSupportBarLengthType;
}

void ColumnDowelRebarTemplate::SetSupportBarLengthType(long value)
{
	m_nSupportBarLengthType = value;
}

long ColumnDowelRebarTemplate::GetSupportBarLength()
{
	return m_nSupportBarLength;
}

void ColumnDowelRebarTemplate::SetSupportBarLength(long value)
{
	m_nSupportBarLength = value;
}
double ColumnDowelRebarTemplate::GetSupportBarFootingThickMultiplicationValue()
{
	return m_dSupportBarFootingThickMultiplicationValue;
}
void ColumnDowelRebarTemplate::SetSupportBarFootingThickMultiplicationValue(double value)
{
	m_dSupportBarFootingThickMultiplicationValue = value;
}
long ColumnDowelRebarTemplate::GetSupportBarCorrectiveLength()
{
	return m_nSupportBarCorrectiveLength;
}

void ColumnDowelRebarTemplate::SetSupportBarCorrectiveLength(long value)
{
	m_nSupportBarCorrectiveLength = value;
}

long ColumnDowelRebarTemplate::GetSupportBarNum()
{
	return m_nSupportBarNum;
}

void ColumnDowelRebarTemplate::SetSupportBarNum(long value)
{
	m_nSupportBarNum = value;
}

long ColumnDowelRebarTemplate::GetBotPlaceType()
{
	return m_nBotPlaceType;
}

void ColumnDowelRebarTemplate::SetBotPlaceType(long value)
{
	m_nBotPlaceType = value;
}

BOOL ColumnDowelRebarTemplate::GetFootingPercentSet()
{
	return m_bFootingPercentSet;
}

void ColumnDowelRebarTemplate::SetFootingPercentSet(BOOL value)
{
	m_bFootingPercentSet = value;
}

long ColumnDowelRebarTemplate::GetFootingPercent()
{
	return m_nFootingPercent;
}

void ColumnDowelRebarTemplate::SetFootingPercent(long value)
{
	m_nFootingPercent = value;
}

BOOL ColumnDowelRebarTemplate::GetFootingCoverSet()
{
	return m_bFootingCoverSet;
}

void ColumnDowelRebarTemplate::SetFootingCoverSet(BOOL value)
{
	m_bFootingCoverSet = value;
}

long ColumnDowelRebarTemplate::GetFootingCover()
{
	return m_nFootingCover;
}

void ColumnDowelRebarTemplate::SetFootingCover(long value)
{
	m_nFootingCover = value;
}

BOOL ColumnDowelRebarTemplate::GetUserHookLenSet()
{
	return m_bUserHookLenSet;
}

void ColumnDowelRebarTemplate::SetUserHookLenSet(BOOL value)
{
	m_bUserHookLenSet = value;
}

BOOL ColumnDowelRebarTemplate::GetSupportBarUserSteelGradeS()
{
	return m_bSupportBarUserSteelGradeS;
}
void ColumnDowelRebarTemplate::SetSupportBarUserSteelGradeS(BOOL value)
{
	m_bSupportBarUserSteelGradeS = value;
}
long ColumnDowelRebarTemplate::GetAlterSpliceType()
{
	return m_nAlterSpliceType;
}

void ColumnDowelRebarTemplate::SetAlterSpliceType(long value)
{
	m_nAlterSpliceType = value;
}