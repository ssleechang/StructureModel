#include "stdafx.h"
#include "BWallDowelRebarTemplate.h"
#include "MSObject.h"
#include "..\GMLib\MSVersionInfo.h"


BWallDowelRebarTemplate::BWallDowelRebarTemplate()
{
	DataInit();
}

BWallDowelRebarTemplate::~BWallDowelRebarTemplate()
{
}

void BWallDowelRebarTemplate::DataInit()
{
	m_nBarSpliceType = 0;
	m_nTopLenType = 0;
	m_nConstructionHeight = 300;
	m_bAlterSpliceSet = FALSE;
	m_nAlterSpliceDist = 400;
	m_nSupportBarDiaIndex = 0;
	m_nSupportBarNum = 3;
	m_nBarAnchorType = 1;
	m_bHookAnchorPlaceSpacingSet = FALSE;
	m_nHookAnchorPlaceSpacing = 500;
	m_bUserHookLenSet = FALSE;
	m_nDowelShape = 0;
	m_bSupportBarUserSteelGradeS = FALSE;

	m_nSpacerDiaIndex = 0;
	m_nSpacerSpacing = 1200;
	m_bSpacerUserSteelGradeS = FALSE;
	m_nSpacerHeightType = 0;
	m_nSpacerUserHeight = 300;
	m_nSpacerLengthA = 100;
	m_nSpacerLengthB = 100;
	m_nSpacerWidthType = 0;
	m_nSpacerUserWidth = 200;
	m_nSpacerHeightSpliceAddLength = 50;
	m_bPlaceSpacer = FALSE;
	m_nAlterSpliceType = 0;
}
void BWallDowelRebarTemplate::CopyFromMe(BWallDowelRebarTemplate* pRebarTemplate)
{
	m_nBarSpliceType = pRebarTemplate->GetBarSpliceType();
	m_nTopLenType = pRebarTemplate->GetTopLenType();
	m_nConstructionHeight = pRebarTemplate->GetConstructionHeight();
	m_bAlterSpliceSet = pRebarTemplate->GetAlterSpliceSet();
	m_nAlterSpliceDist = pRebarTemplate->GetAlterSpliceDist();
	m_nSupportBarDiaIndex = pRebarTemplate->GetSupportBarDiaIndex();
	m_nSupportBarNum = pRebarTemplate->GetSupportBarNum();
	m_nBarAnchorType = pRebarTemplate->GetBarAnchorType();
	m_bHookAnchorPlaceSpacingSet = pRebarTemplate->GetHookAnchorPlaceSpacingSet();
	m_nHookAnchorPlaceSpacing = pRebarTemplate->GetHookAnchorPlaceSpacing();
	m_bUserHookLenSet = pRebarTemplate->GetUserHookLenSet();
	m_nDowelShape = pRebarTemplate->GetDowelShape();
	m_bSupportBarUserSteelGradeS = pRebarTemplate->GetSupportBarUserSteelGradeS();

	m_bPlaceSpacer = pRebarTemplate->GetPlaceSpacer();
	m_nSpacerDiaIndex = pRebarTemplate->GetSpacerDiaIndex();
	m_nSpacerSpacing = pRebarTemplate->GetSpacerSpacing();
	m_bSpacerUserSteelGradeS = pRebarTemplate->GetSpacerUserSteelGradeS();
	m_nSpacerHeightType = pRebarTemplate->GetSpacerHeightType();
	m_nSpacerUserHeight = pRebarTemplate->GetSpacerUserHeight();
	m_nSpacerLengthA = pRebarTemplate->GetSpacerLengthA();
	m_nSpacerLengthB = pRebarTemplate->GetSpacerLengthB();
	m_nSpacerWidthType = pRebarTemplate->GetSpacerWidthType();
	m_nSpacerUserWidth = pRebarTemplate->GetSpacerUserWidth();
	m_nSpacerHeightSpliceAddLength = pRebarTemplate->GetSpacerHeightSpliceAddLength();
	m_nAlterSpliceType = pRebarTemplate->GetAlterSpliceType();
}
void BWallDowelRebarTemplate::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_nBarSpliceType;
		ar << m_nTopLenType;
		ar << m_nConstructionHeight;
		ar << m_bAlterSpliceSet;
		ar << m_nAlterSpliceDist;
		ar << m_nSupportBarDiaIndex;
		ar << m_nSupportBarNum;
		ar << m_nBarAnchorType;
		ar << m_bHookAnchorPlaceSpacingSet;
		ar << m_nHookAnchorPlaceSpacing;
		ar << m_bUserHookLenSet;
		ar << m_nDowelShape;
		ar << m_bSupportBarUserSteelGradeS;

		ar << m_nSpacerDiaIndex;
		ar << m_nSpacerSpacing;
		ar << m_bSpacerUserSteelGradeS;
		ar << m_nSpacerHeightType;
		ar << m_nSpacerUserHeight;
		ar << m_nSpacerLengthA;
		ar << m_nSpacerLengthB;
		ar << m_nSpacerWidthType;
		ar << m_nSpacerUserWidth;
		ar << m_nSpacerHeightSpliceAddLength;
		ar << m_bPlaceSpacer;
		ar << m_nAlterSpliceType;
	}
	else
	{
		ar >> m_nBarSpliceType;
		ar >> m_nTopLenType;
		ar >> m_nConstructionHeight;
		ar >> m_bAlterSpliceSet;
		ar >> m_nAlterSpliceDist;
		ar >> m_nSupportBarDiaIndex;
		ar >> m_nSupportBarNum;
		ar >> m_nBarAnchorType;
		ar >> m_bHookAnchorPlaceSpacingSet;
		ar >> m_nHookAnchorPlaceSpacing;
		ar >> m_bUserHookLenSet;
		if (MSVersionInfo::GetCurrentVersion() >= 20230503)
		{
			ar >> m_nDowelShape;
		}
		if(MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231010)
			ar >> m_bSupportBarUserSteelGradeS;

		if(MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20240325)
		{
			ar >>m_nSpacerDiaIndex;
			ar >>m_nSpacerSpacing;
			ar >>m_bSpacerUserSteelGradeS;
			ar >>m_nSpacerHeightType;
			ar >>m_nSpacerUserHeight;
			ar >>m_nSpacerLengthA;
			ar >>m_nSpacerLengthB;
			ar >>m_nSpacerWidthType;
			ar >>m_nSpacerUserWidth;
			ar >> m_nSpacerHeightSpliceAddLength;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20240614)
			ar >> m_bPlaceSpacer;
		else if(m_nDowelShape == 1)
			m_bPlaceSpacer = TRUE;


		if (MSVersionInfo::GetCurrentVersion() >= 20250219)
			ar >> m_nAlterSpliceType;
		else
			m_nAlterSpliceType = 1;
	}
}

long BWallDowelRebarTemplate::GetBarSpliceType()
{
	return m_nBarSpliceType;
}

void BWallDowelRebarTemplate::SetBarSpliceType(long value)
{
	m_nBarSpliceType = value;
}

long BWallDowelRebarTemplate::GetTopLenType()
{
	return m_nTopLenType;
}

void BWallDowelRebarTemplate::SetTopLenType(long value)
{
	m_nTopLenType = value;
}

long BWallDowelRebarTemplate::GetConstructionHeight()
{
	return m_nConstructionHeight;
}

void BWallDowelRebarTemplate::SetConstructionHeight(long value)
{
	m_nConstructionHeight = value;
}

BOOL BWallDowelRebarTemplate::GetAlterSpliceSet()
{
	return m_bAlterSpliceSet;
}

void BWallDowelRebarTemplate::SetAlterSpliceSet(BOOL value)
{
	m_bAlterSpliceSet = value;
}

long BWallDowelRebarTemplate::GetAlterSpliceDist()
{
	return m_nAlterSpliceDist;
}

void BWallDowelRebarTemplate::SetAlterSpliceDist(long value)
{
	m_nAlterSpliceDist = value;
}

long BWallDowelRebarTemplate::GetSupportBarDiaIndex()
{
	return m_nSupportBarDiaIndex;
}

void BWallDowelRebarTemplate::SetSupportBarDiaIndex(long value)
{
	m_nSupportBarDiaIndex = value;
}

long BWallDowelRebarTemplate::GetSupportBarNum()
{
	return m_nSupportBarNum;
}

void BWallDowelRebarTemplate::SetSupportBarNum(long value)
{
	m_nSupportBarNum = value;
}

long BWallDowelRebarTemplate::GetBarAnchorType()
{
	return m_nBarAnchorType;
}

void BWallDowelRebarTemplate::SetBarAnchorType(long value)
{
	m_nBarAnchorType = value;
}

BOOL BWallDowelRebarTemplate::GetHookAnchorPlaceSpacingSet()
{
	return m_bHookAnchorPlaceSpacingSet;
}

void BWallDowelRebarTemplate::SetHookAnchorPlaceSpacingSet(BOOL value)
{
	m_bHookAnchorPlaceSpacingSet = value;
}

long BWallDowelRebarTemplate::GetHookAnchorPlaceSpacing()
{
	return m_nHookAnchorPlaceSpacing;
}

void BWallDowelRebarTemplate::SetHookAnchorPlaceSpacing(long value)
{
	m_nHookAnchorPlaceSpacing = value;
}

BOOL BWallDowelRebarTemplate::GetUserHookLenSet()
{
	return m_bUserHookLenSet;
}

void BWallDowelRebarTemplate::SetUserHookLenSet(BOOL value)
{
	m_bUserHookLenSet = value;
}
long BWallDowelRebarTemplate::GetDowelShape()
{
	return m_nDowelShape;
}
void BWallDowelRebarTemplate::SetDowelShape(long value)
{
	m_nDowelShape = value;
}
BOOL BWallDowelRebarTemplate::GetSupportBarUserSteelGradeS()
{
	return m_bSupportBarUserSteelGradeS;
}
void BWallDowelRebarTemplate::SetSupportBarUserSteelGradeS(BOOL value)
{
	m_bSupportBarUserSteelGradeS = value;
}

long BWallDowelRebarTemplate::GetSpacerDiaIndex() 
{ 
	return m_nSpacerDiaIndex; 
}
long BWallDowelRebarTemplate::GetSpacerSpacing()
{ 
	return m_nSpacerSpacing; 
}
BOOL BWallDowelRebarTemplate::GetSpacerUserSteelGradeS()
{ 
	return m_bSpacerUserSteelGradeS; 
}
long BWallDowelRebarTemplate::GetSpacerHeightType() 
{ 
	return m_nSpacerHeightType; 
}
long BWallDowelRebarTemplate::GetSpacerHeightSpliceAddLength()
{
	return m_nSpacerHeightSpliceAddLength;
}
long BWallDowelRebarTemplate::GetSpacerUserHeight() 
{ 
	return m_nSpacerUserHeight; 
}
long BWallDowelRebarTemplate::GetSpacerLengthA() 
{ 
	return m_nSpacerLengthA; 
}
long BWallDowelRebarTemplate::GetSpacerLengthB() 
{ 
	return m_nSpacerLengthB; 
}
long BWallDowelRebarTemplate::GetSpacerWidthType()
{ 
	return m_nSpacerWidthType;
}
long BWallDowelRebarTemplate::GetSpacerUserWidth() 
{ 
	return m_nSpacerUserWidth;
}
BOOL BWallDowelRebarTemplate::GetPlaceSpacer()
{
	return m_bPlaceSpacer;
}
void BWallDowelRebarTemplate::SetPlaceSpacer(BOOL value)
{
	m_bPlaceSpacer = value;
}
void BWallDowelRebarTemplate::SetSpacerDiaIndex(long value)
{ 
	m_nSpacerDiaIndex = value;
}
void BWallDowelRebarTemplate::SetSpacerSpacing(long value) 
{ 
	m_nSpacerSpacing = value; 
}
void BWallDowelRebarTemplate::SetSpacerUserSteelGradeS(BOOL value)
{ 
	m_bSpacerUserSteelGradeS = value; 
}
void BWallDowelRebarTemplate::SetSpacerHeightType(long value)
{ 
	m_nSpacerHeightType = value;
}
void BWallDowelRebarTemplate::SetSpacerHeightSpliceAddLength(long value)
{
	m_nSpacerHeightSpliceAddLength = value;
}
void BWallDowelRebarTemplate::SetSpacerUserHeight(long value)
{ 
	m_nSpacerUserHeight = value;
}
void BWallDowelRebarTemplate::SetSpacerLengthA(long value) 
{ 
	m_nSpacerLengthA = value; 
}
void BWallDowelRebarTemplate::SetSpacerLengthB(long value)
{ 
	m_nSpacerLengthB = value;
}
void BWallDowelRebarTemplate::SetSpacerWidthType(long value) 
{ 
	m_nSpacerWidthType = value; 
}
void BWallDowelRebarTemplate::SetSpacerUserWidth(long value)
{ 
	m_nSpacerUserWidth = value; 
}
long BWallDowelRebarTemplate::GetAlterSpliceType()
{
	return m_nAlterSpliceType;
}

void BWallDowelRebarTemplate::SetAlterSpliceType(long value)
{
	m_nAlterSpliceType = value;
}