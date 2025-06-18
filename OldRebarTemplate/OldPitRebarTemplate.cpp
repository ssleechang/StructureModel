#include "StdAfx.h"
#include "OldPitRebarTemplate.h"
#include "..\MSDeformedBar.h"
#include "..\..\GMLib\MSVersionInfo.h"

IMPLEMENT_SERIAL(OldPitRebarTemplate, OldBaseRebarTemplate, VERSIONABLE_SCHEMA | 1)
OldPitRebarTemplate::OldPitRebarTemplate() :OldBaseRebarTemplate()
{
	DataInit();
}

OldPitRebarTemplate::~OldPitRebarTemplate(void)
{
}

void OldPitRebarTemplate::CopyFromMe(OldPitRebarTemplate* pPitRebarTemplate)
{

}

void OldPitRebarTemplate::Dump(EFS::EFS_Buffer& buffer)
{
	buffer << m_bPitGeneralInfoSet;
	buffer << m_nPitMaxLen2HookAnchorBar;

	buffer << m_bPitVerInfoSet;
	buffer << m_nPitVerOutTopHookType;
	buffer << m_nPitVerOutBotHookType;
	buffer << m_nPitVerInTopHookType;
	buffer << m_nPitVerInBotHookType;

	buffer << m_bPitHorInfoSet;
	buffer << m_nPitHorOutHookType;
	buffer << m_nPitHorInHookType;

	buffer << m_bPitTopSpacerInfoSet;
	buffer << m_bPitTopSpacerSet;
	buffer << m_nPitTopSpacerDiaType;
	buffer << m_nPitTopSpacerDiaIndex;
	buffer << m_nPitTopSpacerSpacingType;
	buffer << m_nPitTopSpacerSlabSpacing;
	buffer << m_nPitTopSpacerUserSpacing1;
	buffer << m_nPitTopSpacerUserSpacing2;
	buffer << m_nPitTopSpacerHeightType;
	buffer << m_nPitTopSpacerUserHeight;
	buffer << m_nPitTopSpacerHookLenAType;
	buffer << m_nPitTopSpacerMainBarHookLenA;
	buffer << m_nPitTopSpacerUserHookLenA;
	buffer << m_nPitTopSpacerHookLenBType;
	buffer << m_nPitTopSpacerMainBarHookLenB;
	buffer << m_nPitTopSpacerUserHookLenB;
	buffer << m_nPitTopSpacerWidth;

	buffer << m_bPitBotSpacerInfoSet;
	buffer << m_bPitBotSpacerSet;
	buffer << m_nPitBotSpacerDiaType;
	buffer << m_nPitBotSpacerDiaIndex;
	buffer << m_nPitBotSpacerSpacingType;
	buffer << m_nPitBotSpacerSlabSpacing;
	buffer << m_nPitBotSpacerUserSpacing1;
	buffer << m_nPitBotSpacerUserSpacing2;
	buffer << m_nPitBotSpacerHeightType;
	buffer << m_nPitBotSpacerUserHeight;
	buffer << m_nPitBotSpacerHookLenAType;
	buffer << m_nPitBotSpacerMainBarHookLenA;
	buffer << m_nPitBotSpacerUserHookLenA;
	buffer << m_nPitBotSpacerHookLenBType;
	buffer << m_nPitBotSpacerMainBarHookLenB;
	buffer << m_nPitBotSpacerUserHookLenB;
	buffer << m_nPitBotSpacerWidth;
}

void OldPitRebarTemplate::Recovery(EFS::EFS_Buffer& buffer)
{
	buffer >> m_bPitGeneralInfoSet;
	buffer >> m_nPitMaxLen2HookAnchorBar;

	buffer >> m_bPitVerInfoSet;
	buffer >> m_nPitVerOutTopHookType;
	buffer >> m_nPitVerOutBotHookType;
	buffer >> m_nPitVerInTopHookType;
	buffer >> m_nPitVerInBotHookType;

	buffer >> m_bPitHorInfoSet;
	buffer >> m_nPitHorOutHookType;
	buffer >> m_nPitHorInHookType;

	buffer >> m_bPitTopSpacerInfoSet;
	buffer >> m_bPitTopSpacerSet;
	buffer >> m_nPitTopSpacerDiaType;
	buffer >> m_nPitTopSpacerDiaIndex;
	buffer >> m_nPitTopSpacerSpacingType;
	buffer >> m_nPitTopSpacerSlabSpacing;
	buffer >> m_nPitTopSpacerUserSpacing1;
	buffer >> m_nPitTopSpacerUserSpacing2;
	buffer >> m_nPitTopSpacerHeightType;
	buffer >> m_nPitTopSpacerUserHeight;
	buffer >> m_nPitTopSpacerHookLenAType;
	buffer >> m_nPitTopSpacerMainBarHookLenA;
	buffer >> m_nPitTopSpacerUserHookLenA;
	buffer >> m_nPitTopSpacerHookLenBType;
	buffer >> m_nPitTopSpacerMainBarHookLenB;
	buffer >> m_nPitTopSpacerUserHookLenB;
	buffer >> m_nPitTopSpacerWidth;

	buffer >> m_bPitBotSpacerInfoSet;
	buffer >> m_bPitBotSpacerSet;
	buffer >> m_nPitBotSpacerDiaType;
	buffer >> m_nPitBotSpacerDiaIndex;
	buffer >> m_nPitBotSpacerSpacingType;
	buffer >> m_nPitBotSpacerSlabSpacing;
	buffer >> m_nPitBotSpacerUserSpacing1;
	buffer >> m_nPitBotSpacerUserSpacing2;
	buffer >> m_nPitBotSpacerHeightType;
	buffer >> m_nPitBotSpacerUserHeight;
	buffer >> m_nPitBotSpacerHookLenAType;
	buffer >> m_nPitBotSpacerMainBarHookLenA;
	buffer >> m_nPitBotSpacerUserHookLenA;
	buffer >> m_nPitBotSpacerHookLenBType;
	buffer >> m_nPitBotSpacerMainBarHookLenB;
	buffer >> m_nPitBotSpacerUserHookLenB;
	buffer >> m_nPitBotSpacerWidth;
}

void OldPitRebarTemplate::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_bPitGeneralInfoSet;
		ar << m_nPitMaxLen2HookAnchorBar;

		ar << m_bPitVerInfoSet;
		ar << m_nPitVerOutTopHookType;
		ar << m_nPitVerOutBotHookType;
		ar << m_nPitVerInTopHookType;
		ar << m_nPitVerInBotHookType;

		ar << m_bPitHorInfoSet;
		ar << m_nPitHorOutHookType;
		ar << m_nPitHorInHookType;

		ar << m_bPitTopSpacerInfoSet;
		ar << m_bPitTopSpacerSet;
		ar << m_nPitTopSpacerDiaType;
		ar << m_nPitTopSpacerDiaIndex;
		ar << m_nPitTopSpacerSpacingType;
		ar << m_nPitTopSpacerSlabSpacing;
		ar << m_nPitTopSpacerUserSpacing1;
		ar << m_nPitTopSpacerUserSpacing2;
		ar << m_nPitTopSpacerHeightType;
		ar << m_nPitTopSpacerUserHeight;
		ar << m_nPitTopSpacerHookLenAType;
		ar << m_nPitTopSpacerMainBarHookLenA;
		ar << m_nPitTopSpacerUserHookLenA;
		ar << m_nPitTopSpacerHookLenBType;
		ar << m_nPitTopSpacerMainBarHookLenB;
		ar << m_nPitTopSpacerUserHookLenB;
		ar << m_nPitTopSpacerWidth;

		ar << m_bPitBotSpacerInfoSet;
		ar << m_bPitBotSpacerSet;
		ar << m_nPitBotSpacerDiaType;
		ar << m_nPitBotSpacerDiaIndex;
		ar << m_nPitBotSpacerSpacingType;
		ar << m_nPitBotSpacerSlabSpacing;
		ar << m_nPitBotSpacerUserSpacing1;
		ar << m_nPitBotSpacerUserSpacing2;
		ar << m_nPitBotSpacerHeightType;
		ar << m_nPitBotSpacerUserHeight;
		ar << m_nPitBotSpacerHookLenAType;
		ar << m_nPitBotSpacerMainBarHookLenA;
		ar << m_nPitBotSpacerUserHookLenA;
		ar << m_nPitBotSpacerHookLenBType;
		ar << m_nPitBotSpacerMainBarHookLenB;
		ar << m_nPitBotSpacerUserHookLenB;
		ar << m_nPitBotSpacerWidth;
	}
	else
	{
		ar >> m_bPitGeneralInfoSet;
		ar >> m_nPitMaxLen2HookAnchorBar;

		ar >> m_bPitVerInfoSet;
		ar >> m_nPitVerOutTopHookType;
		ar >> m_nPitVerOutBotHookType;
		ar >> m_nPitVerInTopHookType;
		ar >> m_nPitVerInBotHookType;

		ar >> m_bPitHorInfoSet;
		ar >> m_nPitHorOutHookType;
		ar >> m_nPitHorInHookType;

		if (MSVersionInfo::GetCurrentVersion() >= 20170621)
		{
			ar >> m_bPitTopSpacerInfoSet;
			ar >> m_bPitTopSpacerSet;
			ar >> m_nPitTopSpacerDiaType;
			ar >> m_nPitTopSpacerDiaIndex;
			ar >> m_nPitTopSpacerSpacingType;
			ar >> m_nPitTopSpacerSlabSpacing;
			ar >> m_nPitTopSpacerUserSpacing1;
			ar >> m_nPitTopSpacerUserSpacing2;
			ar >> m_nPitTopSpacerHeightType;
			ar >> m_nPitTopSpacerUserHeight;
			ar >> m_nPitTopSpacerHookLenAType;
			ar >> m_nPitTopSpacerMainBarHookLenA;
			ar >> m_nPitTopSpacerUserHookLenA;
			ar >> m_nPitTopSpacerHookLenBType;
			ar >> m_nPitTopSpacerMainBarHookLenB;
			ar >> m_nPitTopSpacerUserHookLenB;
			ar >> m_nPitTopSpacerWidth;

			ar >> m_bPitBotSpacerInfoSet;
			ar >> m_bPitBotSpacerSet;
			ar >> m_nPitBotSpacerDiaType;
			ar >> m_nPitBotSpacerDiaIndex;
			ar >> m_nPitBotSpacerSpacingType;
			ar >> m_nPitBotSpacerSlabSpacing;
			ar >> m_nPitBotSpacerUserSpacing1;
			ar >> m_nPitBotSpacerUserSpacing2;
			ar >> m_nPitBotSpacerHeightType;
			ar >> m_nPitBotSpacerUserHeight;
			ar >> m_nPitBotSpacerHookLenAType;
			ar >> m_nPitBotSpacerMainBarHookLenA;
			ar >> m_nPitBotSpacerUserHookLenA;
			ar >> m_nPitBotSpacerHookLenBType;
			ar >> m_nPitBotSpacerMainBarHookLenB;
			ar >> m_nPitBotSpacerUserHookLenB;
			ar >> m_nPitBotSpacerWidth;
		}
	}
}

void OldPitRebarTemplate::DataInit()
{
	m_bPitGeneralInfoSet = false;
	m_nPitMaxLen2HookAnchorBar = 6000;

	m_bPitVerInfoSet = false;
	m_nPitVerOutTopHookType = 1;
	m_nPitVerOutBotHookType = 1;
	m_nPitVerInTopHookType = 1;
	m_nPitVerInBotHookType = 0;

	m_bPitHorInfoSet = false;
	m_nPitHorOutHookType = 0;
	m_nPitHorInHookType = 0;

	m_bPitTopSpacerInfoSet = false;
	m_bPitTopSpacerSet = false;
	m_nPitTopSpacerDiaType = 0;
	m_nPitTopSpacerDiaIndex = 0;
	m_nPitTopSpacerSpacingType = 1;
	m_nPitTopSpacerSlabSpacing = 3;
	m_nPitTopSpacerUserSpacing1 = 1500;
	m_nPitTopSpacerUserSpacing2 = 1500;
	m_nPitTopSpacerHeightType = 0;
	m_nPitTopSpacerUserHeight = 300;
	m_nPitTopSpacerHookLenAType = 1;
	m_nPitTopSpacerMainBarHookLenA = 100;
	m_nPitTopSpacerUserHookLenA = 100;
	m_nPitTopSpacerHookLenBType = 1;
	m_nPitTopSpacerMainBarHookLenB = 0;
	m_nPitTopSpacerUserHookLenB = 100;
	m_nPitTopSpacerWidth = 200;

	m_bPitBotSpacerInfoSet = false;
	m_bPitBotSpacerSet = false;
	m_nPitBotSpacerDiaType = 0;
	m_nPitBotSpacerDiaIndex = 0;
	m_nPitBotSpacerSpacingType = 1;
	m_nPitBotSpacerSlabSpacing = 3;
	m_nPitBotSpacerUserSpacing1 = 1500;
	m_nPitBotSpacerUserSpacing2 = 1500;
	m_nPitBotSpacerHeightType = 0;
	m_nPitBotSpacerUserHeight = 300;
	m_nPitBotSpacerHookLenAType = 1;
	m_nPitBotSpacerMainBarHookLenA = 100;
	m_nPitBotSpacerUserHookLenA = 100;
	m_nPitBotSpacerHookLenBType = 1;
	m_nPitBotSpacerMainBarHookLenB = 0;
	m_nPitBotSpacerUserHookLenB = 100;
	m_nPitBotSpacerWidth = 200;
}

bool OldPitRebarTemplate::TopOptionDataCopy(OldPitRebarTemplate* rebarTemplate)
{
	bool bDataCopy = false;

	if (!m_bPitGeneralInfoSet)
	{
		m_nPitMaxLen2HookAnchorBar = rebarTemplate->m_nPitMaxLen2HookAnchorBar;
		bDataCopy = true;
	}

	if (!m_bPitVerInfoSet)
	{
		m_nPitVerOutTopHookType = rebarTemplate->m_nPitVerOutTopHookType;
		m_nPitVerOutBotHookType = rebarTemplate->m_nPitVerOutBotHookType;
		m_nPitVerInTopHookType = rebarTemplate->m_nPitVerInTopHookType;
		m_nPitVerInBotHookType = rebarTemplate->m_nPitVerInBotHookType;
		bDataCopy = true;
	}

	if (!m_bPitHorInfoSet)
	{
		m_nPitHorOutHookType = rebarTemplate->m_nPitHorOutHookType;
		m_nPitHorInHookType = rebarTemplate->m_nPitHorInHookType;
		bDataCopy = true;
	}

	if (!m_bPitTopSpacerInfoSet)
	{
		m_bPitTopSpacerSet = rebarTemplate->m_bPitTopSpacerSet;
		m_nPitTopSpacerDiaType = rebarTemplate->m_nPitTopSpacerDiaType;
		m_nPitTopSpacerDiaIndex = rebarTemplate->m_nPitTopSpacerDiaIndex;
		m_nPitTopSpacerSpacingType = rebarTemplate->m_nPitTopSpacerSpacingType;
		m_nPitTopSpacerSlabSpacing = rebarTemplate->m_nPitTopSpacerSlabSpacing;
		m_nPitTopSpacerUserSpacing1 = rebarTemplate->m_nPitTopSpacerUserSpacing1;
		m_nPitTopSpacerUserSpacing2 = rebarTemplate->m_nPitTopSpacerUserSpacing2;
		m_nPitTopSpacerHeightType = rebarTemplate->m_nPitTopSpacerHeightType;
		m_nPitTopSpacerUserHeight = rebarTemplate->m_nPitTopSpacerUserHeight;
		m_nPitTopSpacerHookLenAType = rebarTemplate->m_nPitTopSpacerHookLenAType;
		m_nPitTopSpacerMainBarHookLenA = rebarTemplate->m_nPitTopSpacerMainBarHookLenA;
		m_nPitTopSpacerUserHookLenA = rebarTemplate->m_nPitTopSpacerUserHookLenA;
		m_nPitTopSpacerHookLenBType = rebarTemplate->m_nPitTopSpacerHookLenBType;
		m_nPitTopSpacerMainBarHookLenB = rebarTemplate->m_nPitTopSpacerMainBarHookLenB;
		m_nPitTopSpacerUserHookLenB = rebarTemplate->m_nPitTopSpacerUserHookLenB;
		m_nPitTopSpacerWidth = rebarTemplate->m_nPitTopSpacerWidth;
	}

	if (!m_bPitBotSpacerInfoSet)
	{
		m_bPitBotSpacerSet = rebarTemplate->m_bPitBotSpacerSet;
		m_nPitBotSpacerDiaType = rebarTemplate->m_nPitBotSpacerDiaType;
		m_nPitBotSpacerDiaIndex = rebarTemplate->m_nPitBotSpacerDiaIndex;
		m_nPitBotSpacerSpacingType = rebarTemplate->m_nPitBotSpacerSpacingType;
		m_nPitBotSpacerSlabSpacing = rebarTemplate->m_nPitBotSpacerSlabSpacing;
		m_nPitBotSpacerUserSpacing1 = rebarTemplate->m_nPitBotSpacerUserSpacing1;
		m_nPitBotSpacerUserSpacing2 = rebarTemplate->m_nPitBotSpacerUserSpacing2;
		m_nPitBotSpacerHeightType = rebarTemplate->m_nPitBotSpacerHeightType;
		m_nPitBotSpacerUserHeight = rebarTemplate->m_nPitBotSpacerUserHeight;
		m_nPitBotSpacerHookLenAType = rebarTemplate->m_nPitBotSpacerHookLenAType;
		m_nPitBotSpacerMainBarHookLenA = rebarTemplate->m_nPitBotSpacerMainBarHookLenA;
		m_nPitBotSpacerUserHookLenA = rebarTemplate->m_nPitBotSpacerUserHookLenA;
		m_nPitBotSpacerHookLenBType = rebarTemplate->m_nPitBotSpacerHookLenBType;
		m_nPitBotSpacerMainBarHookLenB = rebarTemplate->m_nPitBotSpacerMainBarHookLenB;
		m_nPitBotSpacerUserHookLenB = rebarTemplate->m_nPitBotSpacerUserHookLenB;
		m_nPitBotSpacerWidth = rebarTemplate->m_nPitBotSpacerWidth;
	}

	return bDataCopy;
}

void OldPitRebarTemplate::DataCopy(OldPitRebarTemplate* rebarTemplate)
{
	m_nPitMaxLen2HookAnchorBar = rebarTemplate->m_nPitMaxLen2HookAnchorBar;

	m_nPitVerOutTopHookType = rebarTemplate->m_nPitVerOutTopHookType;
	m_nPitVerOutBotHookType = rebarTemplate->m_nPitVerOutBotHookType;
	m_nPitVerInTopHookType = rebarTemplate->m_nPitVerInTopHookType;
	m_nPitVerInBotHookType = rebarTemplate->m_nPitVerInBotHookType;

	m_nPitHorOutHookType = rebarTemplate->m_nPitHorOutHookType;
	m_nPitHorInHookType = rebarTemplate->m_nPitHorInHookType;

	m_bPitTopSpacerSet = rebarTemplate->m_bPitTopSpacerSet;
	m_nPitTopSpacerDiaType = rebarTemplate->m_nPitTopSpacerDiaType;
	m_nPitTopSpacerDiaIndex = rebarTemplate->m_nPitTopSpacerDiaIndex;
	m_nPitTopSpacerSpacingType = rebarTemplate->m_nPitTopSpacerSpacingType;
	m_nPitTopSpacerSlabSpacing = rebarTemplate->m_nPitTopSpacerSlabSpacing;
	m_nPitTopSpacerUserSpacing1 = rebarTemplate->m_nPitTopSpacerUserSpacing1;
	m_nPitTopSpacerUserSpacing2 = rebarTemplate->m_nPitTopSpacerUserSpacing2;
	m_nPitTopSpacerHeightType = rebarTemplate->m_nPitTopSpacerHeightType;
	m_nPitTopSpacerUserHeight = rebarTemplate->m_nPitTopSpacerUserHeight;
	m_nPitTopSpacerHookLenAType = rebarTemplate->m_nPitTopSpacerHookLenAType;
	m_nPitTopSpacerMainBarHookLenA = rebarTemplate->m_nPitTopSpacerMainBarHookLenA;
	m_nPitTopSpacerUserHookLenA = rebarTemplate->m_nPitTopSpacerUserHookLenA;
	m_nPitTopSpacerHookLenBType = rebarTemplate->m_nPitTopSpacerHookLenBType;
	m_nPitTopSpacerMainBarHookLenB = rebarTemplate->m_nPitTopSpacerMainBarHookLenB;
	m_nPitTopSpacerUserHookLenB = rebarTemplate->m_nPitTopSpacerUserHookLenB;
	m_nPitTopSpacerWidth = rebarTemplate->m_nPitTopSpacerWidth;

	m_bPitBotSpacerSet = rebarTemplate->m_bPitBotSpacerSet;
	m_nPitBotSpacerDiaType = rebarTemplate->m_nPitBotSpacerDiaType;
	m_nPitBotSpacerDiaIndex = rebarTemplate->m_nPitBotSpacerDiaIndex;
	m_nPitBotSpacerSpacingType = rebarTemplate->m_nPitBotSpacerSpacingType;
	m_nPitBotSpacerSlabSpacing = rebarTemplate->m_nPitBotSpacerSlabSpacing;
	m_nPitBotSpacerUserSpacing1 = rebarTemplate->m_nPitBotSpacerUserSpacing1;
	m_nPitBotSpacerUserSpacing2 = rebarTemplate->m_nPitBotSpacerUserSpacing2;
	m_nPitBotSpacerHeightType = rebarTemplate->m_nPitBotSpacerHeightType;
	m_nPitBotSpacerUserHeight = rebarTemplate->m_nPitBotSpacerUserHeight;
	m_nPitBotSpacerHookLenAType = rebarTemplate->m_nPitBotSpacerHookLenAType;
	m_nPitBotSpacerMainBarHookLenA = rebarTemplate->m_nPitBotSpacerMainBarHookLenA;
	m_nPitBotSpacerUserHookLenA = rebarTemplate->m_nPitBotSpacerUserHookLenA;
	m_nPitBotSpacerHookLenBType = rebarTemplate->m_nPitBotSpacerHookLenBType;
	m_nPitBotSpacerMainBarHookLenB = rebarTemplate->m_nPitBotSpacerMainBarHookLenB;
	m_nPitBotSpacerUserHookLenB = rebarTemplate->m_nPitBotSpacerUserHookLenB;
	m_nPitBotSpacerWidth = rebarTemplate->m_nPitBotSpacerWidth;
}

void OldPitRebarTemplate::OldDataCopy(PitRebarTemplate* rebarTemplate)
{
	rebarTemplate->SetPitMaxLen2HookAnchorBar(m_nPitMaxLen2HookAnchorBar);

	rebarTemplate->SetPitVerOutTopHookType(m_nPitVerOutTopHookType);
	rebarTemplate->SetPitVerOutBotHookType(m_nPitVerOutBotHookType);
	rebarTemplate->SetPitVerInTopHookType(m_nPitVerInTopHookType);
	rebarTemplate->SetPitVerInBotHookType(m_nPitVerInBotHookType);

	rebarTemplate->SetPitHorOutHookType(m_nPitHorOutHookType);
	rebarTemplate->SetPitHorInHookType(m_nPitHorInHookType);

	rebarTemplate->SetPitTopSpacerSet(m_bPitTopSpacerSet);
	rebarTemplate->SetPitTopSpacerDiaType(m_nPitTopSpacerDiaType);
	rebarTemplate->SetPitTopSpacerSlabThickOverDiaIndex(m_nPitTopSpacerDiaIndex);
	rebarTemplate->SetPitTopSpacerSpacingType(m_nPitTopSpacerSpacingType);
	rebarTemplate->SetPitTopSpacerSlabSpacing(m_nPitTopSpacerSlabSpacing);
	rebarTemplate->SetPitTopSpacerUserSpacing1(m_nPitTopSpacerUserSpacing1);
	rebarTemplate->SetPitTopSpacerUserSpacing2(m_nPitTopSpacerUserSpacing2);
	rebarTemplate->SetPitTopSpacerHeightType(m_nPitTopSpacerHeightType);
	rebarTemplate->SetPitTopSpacerUserHeight(m_nPitTopSpacerUserHeight);
	rebarTemplate->SetPitTopSpacerHookLenAType(m_nPitTopSpacerHookLenAType);
	rebarTemplate->SetPitTopSpacerMainBarHookLenA(m_nPitTopSpacerMainBarHookLenA);
	rebarTemplate->SetPitTopSpacerUserHookLenA(m_nPitTopSpacerUserHookLenA);
	rebarTemplate->SetPitTopSpacerHookLenBType(m_nPitTopSpacerHookLenBType);
	rebarTemplate->SetPitTopSpacerMainBarHookLenB(m_nPitTopSpacerMainBarHookLenB);
	rebarTemplate->SetPitTopSpacerUserHookLenB(m_nPitTopSpacerUserHookLenB);
	rebarTemplate->SetPitTopSpacerWidth(m_nPitTopSpacerWidth);

	rebarTemplate->SetPitBotSpacerSet(m_bPitBotSpacerSet);
	rebarTemplate->SetPitBotSpacerDiaType(m_nPitBotSpacerDiaType);
	rebarTemplate->SetPitBotSpacerDiaIndex(m_nPitBotSpacerDiaIndex);
	rebarTemplate->SetPitBotSpacerSpacingType(m_nPitBotSpacerSpacingType);
	rebarTemplate->SetPitBotSpacerSlabSpacing(m_nPitBotSpacerSlabSpacing);
	rebarTemplate->SetPitBotSpacerUserSpacing1(m_nPitBotSpacerUserSpacing1);
	rebarTemplate->SetPitBotSpacerUserSpacing2(m_nPitBotSpacerUserSpacing2);
	rebarTemplate->SetPitBotSpacerHeightType(m_nPitBotSpacerHeightType);
	rebarTemplate->SetPitBotSpacerUserHeight(m_nPitBotSpacerUserHeight);
	rebarTemplate->SetPitBotSpacerHookLenAType(m_nPitBotSpacerHookLenAType);
	rebarTemplate->SetPitBotSpacerMainBarHookLenA(m_nPitBotSpacerMainBarHookLenA);
	rebarTemplate->SetPitBotSpacerUserHookLenA(m_nPitBotSpacerUserHookLenA);
	rebarTemplate->SetPitBotSpacerHookLenBType(m_nPitBotSpacerHookLenBType);
	rebarTemplate->SetPitBotSpacerMainBarHookLenB(m_nPitBotSpacerMainBarHookLenB);
	rebarTemplate->SetPitBotSpacerUserHookLenB(m_nPitBotSpacerUserHookLenB);
	rebarTemplate->SetPitBotSpacerWidth(m_nPitBotSpacerWidth);
}

void OldPitRebarTemplate::InfoSetCheckBoxChecked()
{
	m_bPitGeneralInfoSet = true;
	m_bPitVerInfoSet = true;
	m_bPitHorInfoSet = true;
	m_bPitTopSpacerInfoSet = true;
	m_bPitBotSpacerInfoSet = true;
}
