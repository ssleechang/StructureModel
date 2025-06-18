#include "StdAfx.h"
#include "MSObject.h"
#include "LowWallRebarTemplate.h"
#include "MSDeformedBar.h"
#include "..\GMLib\MSVersionInfo.h"

LowWallRebarTemplate::LowWallRebarTemplate() :BaseRebarTemplate()
{
	DataInit();
}

LowWallRebarTemplate::~LowWallRebarTemplate(void)
{
}

void LowWallRebarTemplate::DataInit()
{
	m_nLowWallVerBarTopLenType = 0;

	m_bLowWallHorBarStartSet = false;
	m_nLowWallHorBarStartLength = 50;

	m_bLowWallSupportRebarsSet = false;
	m_nLowWallSupportRebarsDiaIndex = 0;
	m_dLowWallSupportRebarsTotalLengthRate = 2.0;
	m_nLowWallSupportRebarsOrderLength = 8000;
	m_nLowWallSupportRebarsSpliceLength = 300;
	m_nLowWallSupportRebarsCorrectiveLength = 10;

	m_bLowWallAddBarsSet = false;
	m_nLowWallAddBarsDiaIndex = 0;
	m_nLowWallAddBarsNumber = 4;
	m_nLowWallAddBarsLengthType = 0;
	m_nLowWallAddBarsTensionLengthType = 0;
	m_nLowWallAddBarsUserLength = 1600;

	m_bLowWallVerBarStirrup = false;
	m_bLowWallVerBarDoubleUbar = false;
	m_bLowWallVerBarDowelIntegrated = false;
	m_nLowWallVerBarStirrupMaxLength = 300;
	m_nLowWallVerBarDoubleUbarMaxLength=1000;
	m_nLowWallVerBarDowelIntegratedLength = 500;

	m_bLowWallSupportRebarUserSteelGradeS = FALSE;
	m_bLowWallAddBarUserSteelGradeS = FALSE;

	m_bLowWallSpliceAddLengthSet = TRUE;
	m_nLowWallSpliceAddLengthNumSplice = 3;

	m_bLowWallVerBarStartSet = FALSE;
	m_nLowWallVerBarStartLength = 50;
}

void LowWallRebarTemplate::CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate)
{
	LowWallRebarTemplate* pLowWallRebarTemplate = dynamic_cast<LowWallRebarTemplate*>(pBaseRebarTemplate);
	if (pLowWallRebarTemplate == nullptr) return;

	m_nLowWallVerBarTopLenType = pLowWallRebarTemplate->m_nLowWallVerBarTopLenType;

	m_bLowWallHorBarStartSet = pLowWallRebarTemplate->m_bLowWallHorBarStartSet;
	m_nLowWallHorBarStartLength = pLowWallRebarTemplate->m_nLowWallHorBarStartLength;

	m_bLowWallSupportRebarsSet = pLowWallRebarTemplate->m_bLowWallSupportRebarsSet;
	m_nLowWallSupportRebarsDiaIndex = pLowWallRebarTemplate->m_nLowWallSupportRebarsDiaIndex;
	m_dLowWallSupportRebarsTotalLengthRate = pLowWallRebarTemplate->m_dLowWallSupportRebarsTotalLengthRate;
	m_nLowWallSupportRebarsOrderLength = pLowWallRebarTemplate->m_nLowWallSupportRebarsOrderLength;
	m_nLowWallSupportRebarsSpliceLength = pLowWallRebarTemplate->m_nLowWallSupportRebarsSpliceLength;
	m_nLowWallSupportRebarsCorrectiveLength = pLowWallRebarTemplate->m_nLowWallSupportRebarsCorrectiveLength;

	m_bLowWallAddBarsSet = pLowWallRebarTemplate->m_bLowWallAddBarsSet;
	m_nLowWallAddBarsDiaIndex = pLowWallRebarTemplate->m_nLowWallAddBarsDiaIndex;
	m_nLowWallAddBarsNumber = pLowWallRebarTemplate->m_nLowWallAddBarsNumber;
	m_nLowWallAddBarsLengthType = pLowWallRebarTemplate->m_nLowWallAddBarsLengthType;
	m_nLowWallAddBarsTensionLengthType = pLowWallRebarTemplate->m_nLowWallAddBarsTensionLengthType;
	m_nLowWallAddBarsUserLength = pLowWallRebarTemplate->m_nLowWallAddBarsUserLength;

	m_bLowWallVerBarStirrup = pLowWallRebarTemplate->m_bLowWallVerBarStirrup;
	m_bLowWallVerBarDoubleUbar = pLowWallRebarTemplate->m_bLowWallVerBarDoubleUbar;
	m_bLowWallVerBarDowelIntegrated = pLowWallRebarTemplate->m_bLowWallVerBarDowelIntegrated;
	m_nLowWallVerBarStirrupMaxLength = pLowWallRebarTemplate->m_nLowWallVerBarStirrupMaxLength;
	m_nLowWallVerBarDoubleUbarMaxLength = pLowWallRebarTemplate->m_nLowWallVerBarDoubleUbarMaxLength;
	m_nLowWallVerBarDowelIntegratedLength = pLowWallRebarTemplate->m_nLowWallVerBarDowelIntegratedLength;

	m_bLowWallSupportRebarUserSteelGradeS = pLowWallRebarTemplate->GetLowWallSupportRebarUserSteelGradeS();
	m_bLowWallAddBarUserSteelGradeS = pLowWallRebarTemplate->GetLowWallAddBarUserSteelGradeS();

	m_bLowWallSpliceAddLengthSet = pLowWallRebarTemplate->GetLowWallSpliceAddLengthSet();
	m_nLowWallSpliceAddLengthNumSplice = pLowWallRebarTemplate->GetLowWallSpliceAddLengthNumSplice();

	m_bLowWallVerBarStartSet = pLowWallRebarTemplate->GetLowWallVerBarStartSet();
	m_nLowWallVerBarStartLength = pLowWallRebarTemplate->GetLowWallVerBarStartLength();
}

void LowWallRebarTemplate::Serialize(CArchive& ar)
{
	long nDefaultValue = 0;

	if (ar.IsStoring())
	{
		ar << m_nLowWallVerBarTopLenType;

		ar << m_bLowWallHorBarStartSet;
		ar << m_nLowWallHorBarStartLength;

		ar << m_bLowWallAddBarsSet;
		ar << m_nLowWallAddBarsDiaIndex;
		ar << m_nLowWallAddBarsNumber;
		ar << m_nLowWallAddBarsLengthType;
		ar << m_nLowWallAddBarsTensionLengthType;
		ar << m_nLowWallAddBarsUserLength;

		ar << m_bLowWallSupportRebarsSet;
		ar << m_nLowWallSupportRebarsDiaIndex;
		ar << m_dLowWallSupportRebarsTotalLengthRate;
		ar << m_nLowWallSupportRebarsOrderLength;
		ar << m_nLowWallSupportRebarsSpliceLength;
		ar << m_nLowWallSupportRebarsCorrectiveLength;

		ar << m_bLowWallSupportRebarUserSteelGradeS;
		ar << m_bLowWallAddBarUserSteelGradeS;

		ar << m_bLowWallVerBarStirrup;
		ar << m_bLowWallVerBarDoubleUbar;
		ar << m_bLowWallVerBarDowelIntegrated;
		ar << m_nLowWallVerBarStirrupMaxLength;
		ar << m_nLowWallVerBarDoubleUbarMaxLength;
		ar << m_nLowWallVerBarDowelIntegratedLength;

		ar << m_bLowWallSpliceAddLengthSet;
		ar << m_nLowWallSpliceAddLengthNumSplice;

		ar << m_bLowWallVerBarStartSet;
		ar << m_nLowWallVerBarStartLength;
	}
	else
	{
		ar >> m_nLowWallVerBarTopLenType;

		ar >> m_bLowWallHorBarStartSet;
		ar >> m_nLowWallHorBarStartLength;

		ar >> m_bLowWallAddBarsSet;
		ar >> m_nLowWallAddBarsDiaIndex;
		ar >> m_nLowWallAddBarsNumber;
		ar >> m_nLowWallAddBarsLengthType;
		ar >> m_nLowWallAddBarsTensionLengthType;
		ar >> m_nLowWallAddBarsUserLength;

		if (MSVersionInfo::GetCurrentVersion() >= 20171027)
		{
			ar >> m_bLowWallSupportRebarsSet;
			ar >> m_nLowWallSupportRebarsDiaIndex;
			ar >> m_dLowWallSupportRebarsTotalLengthRate;
			ar >> m_nLowWallSupportRebarsOrderLength;
			ar >> m_nLowWallSupportRebarsSpliceLength;
			ar >> m_nLowWallSupportRebarsCorrectiveLength;
		}
		if (MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231010)
		{
			ar >> m_bLowWallSupportRebarUserSteelGradeS;
			ar >> m_bLowWallAddBarUserSteelGradeS;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20231220)
		{
			ar >> m_bLowWallVerBarStirrup;
			ar >> m_bLowWallVerBarDoubleUbar;
			ar >> m_bLowWallVerBarDowelIntegrated;
			ar >> m_nLowWallVerBarStirrupMaxLength;
			ar >> m_nLowWallVerBarDoubleUbarMaxLength;
			ar >> m_nLowWallVerBarDowelIntegratedLength;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20241018)
		{
			ar >> m_bLowWallSpliceAddLengthSet;
			ar >> m_nLowWallSpliceAddLengthNumSplice;
		}
		else
		{
			m_bLowWallSpliceAddLengthSet = FALSE;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20241115)
		{
			ar >> m_bLowWallVerBarStartSet;
			ar >> m_nLowWallVerBarStartLength;
		}
	}
}
