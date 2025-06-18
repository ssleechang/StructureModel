#include "stdafx.h"
#include "MSObject.h"
#include "RWallDowelRebarTemplate.h"
#include "..\GMLib\MSVersionInfo.h"


RWallDowelRebarTemplate::RWallDowelRebarTemplate()
{
	DataInit();
}

void RWallDowelRebarTemplate::DataInit()
{
	m_nBarSpliceType = 0;
	m_nTopLenType = 0;
	m_nConstructionHeight = 300;
	m_bAlterSpliceSet = false;
	m_nAlterSpliceDist = 400;
	m_nSupportBarDiaIndex = 0;
	m_nSupportBarNum = 3;
	m_nBarAnchorType = 1;
	m_bHookAnchorPlaceSpacingSet = false;
	m_nHookAnchorPlaceSpacing = 500;
	m_bUserHookLenSet = false;
	m_bBotAddBarIntegratedPlaceSet = false;
	dowelAdjustRatioSet = false;
	dowelAdjustRatio = 50;
	m_nDowelShape = 0;
	m_bSupportBarUserSteelGradeS = FALSE;
	m_nBotAddBarIntegratedPlaceLengthConditionType = 0;
	m_nBotAddBarIntegratedPlaceLengthCondition = 1500;

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
void RWallDowelRebarTemplate::CopyFromMe(RWallDowelRebarTemplate* pRebarTemplate)
{
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
	m_bBotAddBarIntegratedPlaceSet = pRebarTemplate->GetBotAddBarIntegratedPlaceSet();
	dowelAdjustRatioSet = pRebarTemplate->GetDowelAdjustRatioSet();
	dowelAdjustRatio = pRebarTemplate->GetDowelAdjustRatio();
	m_nDowelShape = pRebarTemplate->GetDowelShape();
	m_bSupportBarUserSteelGradeS = pRebarTemplate->GetSupportBarUserSteelGradeS();
	m_nBotAddBarIntegratedPlaceLengthConditionType = pRebarTemplate->GetBotAddBarIntegratedPlaceLengthConditionType();
	m_nBotAddBarIntegratedPlaceLengthCondition = pRebarTemplate->GetBotAddBarIntegratedPlaceLengthCondition();

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
void RWallDowelRebarTemplate::Serialize(CArchive& ar)
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
		ar << m_bBotAddBarIntegratedPlaceSet;
		ar << dowelAdjustRatioSet;
		ar << dowelAdjustRatio;
		ar << m_nDowelShape;
		ar << m_bSupportBarUserSteelGradeS;
		ar << m_nBotAddBarIntegratedPlaceLengthConditionType;
		ar << m_nBotAddBarIntegratedPlaceLengthCondition;

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
		ar >> m_bBotAddBarIntegratedPlaceSet;

		if(MSVersionInfo::GetCurrentVersion() >= 20230306)
		{
			ar >> dowelAdjustRatioSet;
			ar >> dowelAdjustRatio;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20230503)
		{
			ar >> m_nDowelShape;
		}
		if (MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231010)
			ar >> m_bSupportBarUserSteelGradeS;

		if((MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231228) || MSVersionInfo::IsOldTotalVersion())
		{
			ar >> m_nBotAddBarIntegratedPlaceLengthConditionType;
			ar >> m_nBotAddBarIntegratedPlaceLengthCondition;
		}

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

RWallDowelRebarTemplate::~RWallDowelRebarTemplate()
{
}

long RWallDowelRebarTemplate::GetBarSpliceType()
{
	return m_nBarSpliceType;
}

void RWallDowelRebarTemplate::SetBarSpliceType(long value)
{
	m_nBarSpliceType = value;
}

long RWallDowelRebarTemplate::GetTopLenType()
{
	return m_nTopLenType;
}

void RWallDowelRebarTemplate::SetTopLenType(long value)
{
	m_nTopLenType = value;
}

long RWallDowelRebarTemplate::GetConstructionHeight()
{
	return m_nConstructionHeight;
}

void RWallDowelRebarTemplate::SetConstructionHeight(long value)
{
	m_nConstructionHeight = value;
}

BOOL RWallDowelRebarTemplate::GetAlterSpliceSet()
{
	return m_bAlterSpliceSet;
}

void RWallDowelRebarTemplate::SetAlterSpliceSet(BOOL value)
{
	m_bAlterSpliceSet = value;
}

long RWallDowelRebarTemplate::GetAlterSpliceDist()
{
	return m_nAlterSpliceDist;
}

void RWallDowelRebarTemplate::SetAlterSpliceDist(long value)
{
	m_nAlterSpliceDist = value;
}

long RWallDowelRebarTemplate::GetSupportBarDiaIndex()
{
	return m_nSupportBarDiaIndex;
}

void RWallDowelRebarTemplate::SetSupportBarDiaIndex(long value)
{
	m_nSupportBarDiaIndex = value;
}

long RWallDowelRebarTemplate::GetSupportBarNum()
{
	return m_nSupportBarNum;
}

void RWallDowelRebarTemplate::SetSupportBarNum(long value)
{
	m_nSupportBarNum = value;
}

long RWallDowelRebarTemplate::GetBarAnchorType()
{
	return m_nBarAnchorType;
}

void RWallDowelRebarTemplate::SetBarAnchorType(long value)
{
	m_nBarAnchorType = value;
}

BOOL RWallDowelRebarTemplate::GetHookAnchorPlaceSpacingSet()
{
	return m_bHookAnchorPlaceSpacingSet;
}

void RWallDowelRebarTemplate::SetHookAnchorPlaceSpacingSet(BOOL value)
{
	m_bHookAnchorPlaceSpacingSet = value;
}

long RWallDowelRebarTemplate::GetHookAnchorPlaceSpacing()
{
	return m_nHookAnchorPlaceSpacing;
}

void RWallDowelRebarTemplate::SetHookAnchorPlaceSpacing(long value)
{
	m_nHookAnchorPlaceSpacing = value;
}

BOOL RWallDowelRebarTemplate::GetUserHookLenSet()
{
	return m_bUserHookLenSet;
}

void RWallDowelRebarTemplate::SetUserHookLenSet(BOOL value)
{
	m_bUserHookLenSet = value;
}

BOOL RWallDowelRebarTemplate::GetBotAddBarIntegratedPlaceSet()
{
	return m_bBotAddBarIntegratedPlaceSet;
}

void RWallDowelRebarTemplate::SetBotAddBarIntegratedPlaceSet(BOOL value)
{
	m_bBotAddBarIntegratedPlaceSet = value;
}

BOOL RWallDowelRebarTemplate::GetDowelAdjustRatioSet()
{
	return dowelAdjustRatioSet;
}

void RWallDowelRebarTemplate::SetDowelAdjustRatioSet(BOOL value)
{
	dowelAdjustRatioSet = value;
}

int RWallDowelRebarTemplate::GetDowelAdjustRatio()
{
	return dowelAdjustRatio;
}

void RWallDowelRebarTemplate::SetDowelAdjustRatio(int value)
{
	dowelAdjustRatio = value;
}

long RWallDowelRebarTemplate::GetDowelShape()
{
	return m_nDowelShape;
}
void RWallDowelRebarTemplate::SetDowelShape(long value)
{
	m_nDowelShape = value;
}
BOOL RWallDowelRebarTemplate::GetSupportBarUserSteelGradeS()
{
	return m_bSupportBarUserSteelGradeS;
}
void RWallDowelRebarTemplate::SetSupportBarUserSteelGradeS(BOOL value)
{
	m_bSupportBarUserSteelGradeS = value;
}
long RWallDowelRebarTemplate::GetBotAddBarIntegratedPlaceLengthConditionType()
{
	return m_nBotAddBarIntegratedPlaceLengthConditionType;
}
void RWallDowelRebarTemplate::SetBotAddBarIntegratedPlaceLengthConditionType(long value)
{
	m_nBotAddBarIntegratedPlaceLengthConditionType = value;
}
long RWallDowelRebarTemplate::GetBotAddBarIntegratedPlaceLengthCondition()
{
	return m_nBotAddBarIntegratedPlaceLengthCondition;
}
void RWallDowelRebarTemplate::SetBotAddBarIntegratedPlaceLengthCondition(long value)
{
	m_nBotAddBarIntegratedPlaceLengthCondition = value;
}

long RWallDowelRebarTemplate::GetSpacerDiaIndex() 
{ 
	return m_nSpacerDiaIndex; 
}
long RWallDowelRebarTemplate::GetSpacerSpacing()
{ 
	return m_nSpacerSpacing; 
}
BOOL RWallDowelRebarTemplate::GetSpacerUserSteelGradeS()
{ 
	return m_bSpacerUserSteelGradeS; 
}
long RWallDowelRebarTemplate::GetSpacerHeightType() 
{ 
	return m_nSpacerHeightType; 
}
long RWallDowelRebarTemplate::GetSpacerUserHeight() 
{ 
	return m_nSpacerUserHeight; 
}
long RWallDowelRebarTemplate::GetSpacerHeightSpliceAddLength()
{
	return m_nSpacerHeightSpliceAddLength;
}
long RWallDowelRebarTemplate::GetSpacerLengthA() 
{ 
	return m_nSpacerLengthA; 
}
long RWallDowelRebarTemplate::GetSpacerLengthB() 
{ 
	return m_nSpacerLengthB; 
}
long RWallDowelRebarTemplate::GetSpacerWidthType()
{ 
	return m_nSpacerWidthType;
}
long RWallDowelRebarTemplate::GetSpacerUserWidth() 
{ 
	return m_nSpacerUserWidth;
}
BOOL RWallDowelRebarTemplate::GetPlaceSpacer()
{
	return m_bPlaceSpacer;
}
void RWallDowelRebarTemplate::SetPlaceSpacer(BOOL value)
{
	m_bPlaceSpacer = value;
}
void RWallDowelRebarTemplate::SetSpacerDiaIndex(long value)
{ 
	m_nSpacerDiaIndex = value;
}
void RWallDowelRebarTemplate::SetSpacerSpacing(long value) 
{ 
	m_nSpacerSpacing = value; 
}
void RWallDowelRebarTemplate::SetSpacerUserSteelGradeS(BOOL value)
{ 
	m_bSpacerUserSteelGradeS = value; 
}
void RWallDowelRebarTemplate::SetSpacerHeightType(long value)
{ 
	m_nSpacerHeightType = value;
}
void RWallDowelRebarTemplate::SetSpacerHeightSpliceAddLength(long value)
{
	m_nSpacerHeightSpliceAddLength = value;
}
void RWallDowelRebarTemplate::SetSpacerUserHeight(long value)
{ 
	m_nSpacerUserHeight = value;
}
void RWallDowelRebarTemplate::SetSpacerLengthA(long value) 
{ 
	m_nSpacerLengthA = value; 
}
void RWallDowelRebarTemplate::SetSpacerLengthB(long value)
{ 
	m_nSpacerLengthB = value;
}
void RWallDowelRebarTemplate::SetSpacerWidthType(long value) 
{ 
	m_nSpacerWidthType = value; 
}
void RWallDowelRebarTemplate::SetSpacerUserWidth(long value)
{ 
	m_nSpacerUserWidth = value; 
}
long RWallDowelRebarTemplate::GetAlterSpliceType()
{
	return m_nAlterSpliceType;
}

void RWallDowelRebarTemplate::SetAlterSpliceType(long value)
{
	m_nAlterSpliceType = value;
}