#include "StdAfx.h"
#include "OldIsoFootRebarTemplate.h"
#include "..\MSDeformedBar.h"
#include "..\..\GMLib\MSVersionInfo.h"

IMPLEMENT_SERIAL(OldIsoFootRebarTemplate, OldBaseRebarTemplate, VERSIONABLE_SCHEMA | 1)
OldIsoFootRebarTemplate::OldIsoFootRebarTemplate() :OldBaseRebarTemplate()
{
	DataInit();
}

OldIsoFootRebarTemplate::~OldIsoFootRebarTemplate(void)
{
}

void OldIsoFootRebarTemplate::CopyFromMe(OldIsoFootRebarTemplate* pIsoFootRebarTemplate)
{

}

void OldIsoFootRebarTemplate::Dump(EFS::EFS_Buffer& buffer)
{
	buffer << m_bIsoFootTopSpacerInfoSet;
	buffer << m_bIsoFootTopSpacerSet;
	buffer << m_nIsoFootTopSpacerDiaType;
	buffer << m_nIsoFootTopSpacerDiaIndex;
	buffer << m_nIsoFootTopSpacerSpacingType;
	buffer << m_nIsoFootTopSpacerSlabSpacing;
	buffer << m_nIsoFootTopSpacerUserSpacing1;
	buffer << m_nIsoFootTopSpacerUserSpacing2;
	buffer << m_nIsoFootTopSpacerHeightType;
	buffer << m_nIsoFootTopSpacerUserHeight;
	buffer << m_nIsoFootTopSpacerHookLenAType;
	buffer << m_nIsoFootTopSpacerMainBarHookLenA;
	buffer << m_nIsoFootTopSpacerUserHookLenA;
	buffer << m_nIsoFootTopSpacerHookLenBType;
	buffer << m_nIsoFootTopSpacerMainBarHookLenB;
	buffer << m_nIsoFootTopSpacerUserHookLenB;
	buffer << m_nIsoFootTopSpacerWidth;

	buffer << m_bIsoFootBotSpacerInfoSet;
	buffer << m_bIsoFootBotSpacerSet;
	buffer << m_nIsoFootBotSpacerDiaType;
	buffer << m_nIsoFootBotSpacerDiaIndex;
	buffer << m_nIsoFootBotSpacerSpacingType;
	buffer << m_nIsoFootBotSpacerSlabSpacing;
	buffer << m_nIsoFootBotSpacerUserSpacing1;
	buffer << m_nIsoFootBotSpacerUserSpacing2;
	buffer << m_nIsoFootBotSpacerHeightType;
	buffer << m_nIsoFootBotSpacerUserHeight;
	buffer << m_nIsoFootBotSpacerHookLenAType;
	buffer << m_nIsoFootBotSpacerMainBarHookLenA;
	buffer << m_nIsoFootBotSpacerUserHookLenA;
	buffer << m_nIsoFootBotSpacerHookLenBType;
	buffer << m_nIsoFootBotSpacerMainBarHookLenB;
	buffer << m_nIsoFootBotSpacerUserHookLenB;
	buffer << m_nIsoFootBotSpacerWidth;
}

void OldIsoFootRebarTemplate::Recovery(EFS::EFS_Buffer& buffer)
{
	buffer >> m_bIsoFootTopSpacerInfoSet;
	buffer >> m_bIsoFootTopSpacerSet;
	buffer >> m_nIsoFootTopSpacerDiaType;
	buffer >> m_nIsoFootTopSpacerDiaIndex;
	buffer >> m_nIsoFootTopSpacerSpacingType;
	buffer >> m_nIsoFootTopSpacerSlabSpacing;
	buffer >> m_nIsoFootTopSpacerUserSpacing1;
	buffer >> m_nIsoFootTopSpacerUserSpacing2;
	buffer >> m_nIsoFootTopSpacerHeightType;
	buffer >> m_nIsoFootTopSpacerUserHeight;
	buffer >> m_nIsoFootTopSpacerHookLenAType;
	buffer >> m_nIsoFootTopSpacerMainBarHookLenA;
	buffer >> m_nIsoFootTopSpacerUserHookLenA;
	buffer >> m_nIsoFootTopSpacerHookLenBType;
	buffer >> m_nIsoFootTopSpacerMainBarHookLenB;
	buffer >> m_nIsoFootTopSpacerUserHookLenB;
	buffer >> m_nIsoFootTopSpacerWidth;

	buffer >> m_bIsoFootBotSpacerInfoSet;
	buffer >> m_bIsoFootBotSpacerSet;
	buffer >> m_nIsoFootBotSpacerDiaType;
	buffer >> m_nIsoFootBotSpacerDiaIndex;
	buffer >> m_nIsoFootBotSpacerSpacingType;
	buffer >> m_nIsoFootBotSpacerSlabSpacing;
	buffer >> m_nIsoFootBotSpacerUserSpacing1;
	buffer >> m_nIsoFootBotSpacerUserSpacing2;
	buffer >> m_nIsoFootBotSpacerHeightType;
	buffer >> m_nIsoFootBotSpacerUserHeight;
	buffer >> m_nIsoFootBotSpacerHookLenAType;
	buffer >> m_nIsoFootBotSpacerMainBarHookLenA;
	buffer >> m_nIsoFootBotSpacerUserHookLenA;
	buffer >> m_nIsoFootBotSpacerHookLenBType;
	buffer >> m_nIsoFootBotSpacerMainBarHookLenB;
	buffer >> m_nIsoFootBotSpacerUserHookLenB;
	buffer >> m_nIsoFootBotSpacerWidth;
}

void OldIsoFootRebarTemplate::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_bIsoFootTopSpacerInfoSet;
		ar << m_bIsoFootTopSpacerSet;
		ar << m_nIsoFootTopSpacerDiaType;
		ar << m_nIsoFootTopSpacerDiaIndex;
		ar << m_nIsoFootTopSpacerSpacingType;
		ar << m_nIsoFootTopSpacerSlabSpacing;
		ar << m_nIsoFootTopSpacerUserSpacing1;
		ar << m_nIsoFootTopSpacerUserSpacing2;
		ar << m_nIsoFootTopSpacerHeightType;
		ar << m_nIsoFootTopSpacerUserHeight;
		ar << m_nIsoFootTopSpacerHookLenAType;
		ar << m_nIsoFootTopSpacerMainBarHookLenA;
		ar << m_nIsoFootTopSpacerUserHookLenA;
		ar << m_nIsoFootTopSpacerHookLenBType;
		ar << m_nIsoFootTopSpacerMainBarHookLenB;
		ar << m_nIsoFootTopSpacerUserHookLenB;
		ar << m_nIsoFootTopSpacerWidth;

		ar << m_bIsoFootBotSpacerInfoSet;
		ar << m_bIsoFootBotSpacerSet;
		ar << m_nIsoFootBotSpacerDiaType;
		ar << m_nIsoFootBotSpacerDiaIndex;
		ar << m_nIsoFootBotSpacerSpacingType;
		ar << m_nIsoFootBotSpacerSlabSpacing;
		ar << m_nIsoFootBotSpacerUserSpacing1;
		ar << m_nIsoFootBotSpacerUserSpacing2;
		ar << m_nIsoFootBotSpacerHeightType;
		ar << m_nIsoFootBotSpacerUserHeight;
		ar << m_nIsoFootBotSpacerHookLenAType;
		ar << m_nIsoFootBotSpacerMainBarHookLenA;
		ar << m_nIsoFootBotSpacerUserHookLenA;
		ar << m_nIsoFootBotSpacerHookLenBType;
		ar << m_nIsoFootBotSpacerMainBarHookLenB;
		ar << m_nIsoFootBotSpacerUserHookLenB;
		ar << m_nIsoFootBotSpacerWidth;
	}
	else
	{
		ar >> m_bIsoFootTopSpacerInfoSet;
		ar >> m_bIsoFootTopSpacerSet;
		ar >> m_nIsoFootTopSpacerDiaType;
		ar >> m_nIsoFootTopSpacerDiaIndex;
		ar >> m_nIsoFootTopSpacerSpacingType;
		ar >> m_nIsoFootTopSpacerSlabSpacing;
		ar >> m_nIsoFootTopSpacerUserSpacing1;
		ar >> m_nIsoFootTopSpacerUserSpacing2;
		ar >> m_nIsoFootTopSpacerHeightType;
		ar >> m_nIsoFootTopSpacerUserHeight;
		ar >> m_nIsoFootTopSpacerHookLenAType;
		ar >> m_nIsoFootTopSpacerMainBarHookLenA;
		ar >> m_nIsoFootTopSpacerUserHookLenA;
		ar >> m_nIsoFootTopSpacerHookLenBType;
		ar >> m_nIsoFootTopSpacerMainBarHookLenB;
		ar >> m_nIsoFootTopSpacerUserHookLenB;
		ar >> m_nIsoFootTopSpacerWidth;

		ar >> m_bIsoFootBotSpacerInfoSet;
		ar >> m_bIsoFootBotSpacerSet;
		ar >> m_nIsoFootBotSpacerDiaType;
		ar >> m_nIsoFootBotSpacerDiaIndex;
		ar >> m_nIsoFootBotSpacerSpacingType;
		ar >> m_nIsoFootBotSpacerSlabSpacing;
		ar >> m_nIsoFootBotSpacerUserSpacing1;
		ar >> m_nIsoFootBotSpacerUserSpacing2;
		ar >> m_nIsoFootBotSpacerHeightType;
		ar >> m_nIsoFootBotSpacerUserHeight;
		ar >> m_nIsoFootBotSpacerHookLenAType;
		ar >> m_nIsoFootBotSpacerMainBarHookLenA;
		ar >> m_nIsoFootBotSpacerUserHookLenA;
		ar >> m_nIsoFootBotSpacerHookLenBType;
		ar >> m_nIsoFootBotSpacerMainBarHookLenB;
		ar >> m_nIsoFootBotSpacerUserHookLenB;
		ar >> m_nIsoFootBotSpacerWidth;
	}
}

void OldIsoFootRebarTemplate::DataInit()
{
	m_bIsoFootTopSpacerInfoSet = false;
	m_bIsoFootTopSpacerSet = false;
	m_nIsoFootTopSpacerDiaType = 0;
	m_nIsoFootTopSpacerDiaIndex = 0;
	m_nIsoFootTopSpacerSpacingType = 1;
	m_nIsoFootTopSpacerSlabSpacing = 3;
	m_nIsoFootTopSpacerUserSpacing1 = 1500;
	m_nIsoFootTopSpacerUserSpacing2 = 1500;
	m_nIsoFootTopSpacerHeightType = 0;
	m_nIsoFootTopSpacerUserHeight = 300;
	m_nIsoFootTopSpacerHookLenAType = 1;
	m_nIsoFootTopSpacerMainBarHookLenA = 100;
	m_nIsoFootTopSpacerUserHookLenA = 100;
	m_nIsoFootTopSpacerHookLenBType = 1;
	m_nIsoFootTopSpacerMainBarHookLenB = 0;
	m_nIsoFootTopSpacerUserHookLenB = 100;
	m_nIsoFootTopSpacerWidth = 200;

	m_bIsoFootBotSpacerInfoSet = false;
	m_bIsoFootBotSpacerSet = false;
	m_nIsoFootBotSpacerDiaType = 0;
	m_nIsoFootBotSpacerDiaIndex = 0;
	m_nIsoFootBotSpacerSpacingType = 1;
	m_nIsoFootBotSpacerSlabSpacing = 3;
	m_nIsoFootBotSpacerUserSpacing1 = 1500;
	m_nIsoFootBotSpacerUserSpacing2 = 1500;
	m_nIsoFootBotSpacerHeightType = 0;
	m_nIsoFootBotSpacerUserHeight = 300;
	m_nIsoFootBotSpacerHookLenAType = 1;
	m_nIsoFootBotSpacerMainBarHookLenA = 100;
	m_nIsoFootBotSpacerUserHookLenA = 100;
	m_nIsoFootBotSpacerHookLenBType = 1;
	m_nIsoFootBotSpacerMainBarHookLenB = 0;
	m_nIsoFootBotSpacerUserHookLenB = 100;
	m_nIsoFootBotSpacerWidth = 200;
}

bool OldIsoFootRebarTemplate::TopOptionDataCopy(OldIsoFootRebarTemplate* rebarTemplate)
{
	bool bDataCopy = false;

	if (!m_bIsoFootTopSpacerInfoSet)
	{
		m_bIsoFootTopSpacerSet = rebarTemplate->m_bIsoFootTopSpacerSet;
		m_nIsoFootTopSpacerDiaType = rebarTemplate->m_nIsoFootTopSpacerDiaType;
		m_nIsoFootTopSpacerDiaIndex = rebarTemplate->m_nIsoFootTopSpacerDiaIndex;
		m_nIsoFootTopSpacerSpacingType = rebarTemplate->m_nIsoFootTopSpacerSpacingType;
		m_nIsoFootTopSpacerSlabSpacing = rebarTemplate->m_nIsoFootTopSpacerSlabSpacing;
		m_nIsoFootTopSpacerUserSpacing1 = rebarTemplate->m_nIsoFootTopSpacerUserSpacing1;
		m_nIsoFootTopSpacerUserSpacing2 = rebarTemplate->m_nIsoFootTopSpacerUserSpacing2;
		m_nIsoFootTopSpacerHeightType = rebarTemplate->m_nIsoFootTopSpacerHeightType;
		m_nIsoFootTopSpacerUserHeight = rebarTemplate->m_nIsoFootTopSpacerUserHeight;
		m_nIsoFootTopSpacerHookLenAType = rebarTemplate->m_nIsoFootTopSpacerHookLenAType;
		m_nIsoFootTopSpacerMainBarHookLenA = rebarTemplate->m_nIsoFootTopSpacerMainBarHookLenA;
		m_nIsoFootTopSpacerUserHookLenA = rebarTemplate->m_nIsoFootTopSpacerUserHookLenA;
		m_nIsoFootTopSpacerHookLenBType = rebarTemplate->m_nIsoFootTopSpacerHookLenBType;
		m_nIsoFootTopSpacerMainBarHookLenB = rebarTemplate->m_nIsoFootTopSpacerMainBarHookLenB;
		m_nIsoFootTopSpacerUserHookLenB = rebarTemplate->m_nIsoFootTopSpacerUserHookLenB;
		m_nIsoFootTopSpacerWidth = rebarTemplate->m_nIsoFootTopSpacerWidth;
	}

	if (!m_bIsoFootBotSpacerInfoSet)
	{
		m_bIsoFootBotSpacerSet = rebarTemplate->m_bIsoFootBotSpacerSet;
		m_nIsoFootBotSpacerDiaType = rebarTemplate->m_nIsoFootBotSpacerDiaType;
		m_nIsoFootBotSpacerDiaIndex = rebarTemplate->m_nIsoFootBotSpacerDiaIndex;
		m_nIsoFootBotSpacerSpacingType = rebarTemplate->m_nIsoFootBotSpacerSpacingType;
		m_nIsoFootBotSpacerSlabSpacing = rebarTemplate->m_nIsoFootBotSpacerSlabSpacing;
		m_nIsoFootBotSpacerUserSpacing1 = rebarTemplate->m_nIsoFootBotSpacerUserSpacing1;
		m_nIsoFootBotSpacerUserSpacing2 = rebarTemplate->m_nIsoFootBotSpacerUserSpacing2;
		m_nIsoFootBotSpacerHeightType = rebarTemplate->m_nIsoFootBotSpacerHeightType;
		m_nIsoFootBotSpacerUserHeight = rebarTemplate->m_nIsoFootBotSpacerUserHeight;
		m_nIsoFootBotSpacerHookLenAType = rebarTemplate->m_nIsoFootBotSpacerHookLenAType;
		m_nIsoFootBotSpacerMainBarHookLenA = rebarTemplate->m_nIsoFootBotSpacerMainBarHookLenA;
		m_nIsoFootBotSpacerUserHookLenA = rebarTemplate->m_nIsoFootBotSpacerUserHookLenA;
		m_nIsoFootBotSpacerHookLenBType = rebarTemplate->m_nIsoFootBotSpacerHookLenBType;
		m_nIsoFootBotSpacerMainBarHookLenB = rebarTemplate->m_nIsoFootBotSpacerMainBarHookLenB;
		m_nIsoFootBotSpacerUserHookLenB = rebarTemplate->m_nIsoFootBotSpacerUserHookLenB;
		m_nIsoFootBotSpacerWidth = rebarTemplate->m_nIsoFootBotSpacerWidth;
	}

	return bDataCopy;
}

void OldIsoFootRebarTemplate::DataCopy(OldIsoFootRebarTemplate* rebarTemplate)
{
	m_bIsoFootTopSpacerSet = rebarTemplate->m_bIsoFootTopSpacerSet;
	m_nIsoFootTopSpacerDiaType = rebarTemplate->m_nIsoFootTopSpacerDiaType;
	m_nIsoFootTopSpacerDiaIndex = rebarTemplate->m_nIsoFootTopSpacerDiaIndex;
	m_nIsoFootTopSpacerSpacingType = rebarTemplate->m_nIsoFootTopSpacerSpacingType;
	m_nIsoFootTopSpacerSlabSpacing = rebarTemplate->m_nIsoFootTopSpacerSlabSpacing;
	m_nIsoFootTopSpacerUserSpacing1 = rebarTemplate->m_nIsoFootTopSpacerUserSpacing1;
	m_nIsoFootTopSpacerUserSpacing2 = rebarTemplate->m_nIsoFootTopSpacerUserSpacing2;
	m_nIsoFootTopSpacerHeightType = rebarTemplate->m_nIsoFootTopSpacerHeightType;
	m_nIsoFootTopSpacerUserHeight = rebarTemplate->m_nIsoFootTopSpacerUserHeight;
	m_nIsoFootTopSpacerHookLenAType = rebarTemplate->m_nIsoFootTopSpacerHookLenAType;
	m_nIsoFootTopSpacerMainBarHookLenA = rebarTemplate->m_nIsoFootTopSpacerMainBarHookLenA;
	m_nIsoFootTopSpacerUserHookLenA = rebarTemplate->m_nIsoFootTopSpacerUserHookLenA;
	m_nIsoFootTopSpacerHookLenBType = rebarTemplate->m_nIsoFootTopSpacerHookLenBType;
	m_nIsoFootTopSpacerMainBarHookLenB = rebarTemplate->m_nIsoFootTopSpacerMainBarHookLenB;
	m_nIsoFootTopSpacerUserHookLenB = rebarTemplate->m_nIsoFootTopSpacerUserHookLenB;
	m_nIsoFootTopSpacerWidth = rebarTemplate->m_nIsoFootTopSpacerWidth;

	m_bIsoFootBotSpacerSet = rebarTemplate->m_bIsoFootBotSpacerSet;
	m_nIsoFootBotSpacerDiaType = rebarTemplate->m_nIsoFootBotSpacerDiaType;
	m_nIsoFootBotSpacerDiaIndex = rebarTemplate->m_nIsoFootBotSpacerDiaIndex;
	m_nIsoFootBotSpacerSpacingType = rebarTemplate->m_nIsoFootBotSpacerSpacingType;
	m_nIsoFootBotSpacerSlabSpacing = rebarTemplate->m_nIsoFootBotSpacerSlabSpacing;
	m_nIsoFootBotSpacerUserSpacing1 = rebarTemplate->m_nIsoFootBotSpacerUserSpacing1;
	m_nIsoFootBotSpacerUserSpacing2 = rebarTemplate->m_nIsoFootBotSpacerUserSpacing2;
	m_nIsoFootBotSpacerHeightType = rebarTemplate->m_nIsoFootBotSpacerHeightType;
	m_nIsoFootBotSpacerUserHeight = rebarTemplate->m_nIsoFootBotSpacerUserHeight;
	m_nIsoFootBotSpacerHookLenAType = rebarTemplate->m_nIsoFootBotSpacerHookLenAType;
	m_nIsoFootBotSpacerMainBarHookLenA = rebarTemplate->m_nIsoFootBotSpacerMainBarHookLenA;
	m_nIsoFootBotSpacerUserHookLenA = rebarTemplate->m_nIsoFootBotSpacerUserHookLenA;
	m_nIsoFootBotSpacerHookLenBType = rebarTemplate->m_nIsoFootBotSpacerHookLenBType;
	m_nIsoFootBotSpacerMainBarHookLenB = rebarTemplate->m_nIsoFootBotSpacerMainBarHookLenB;
	m_nIsoFootBotSpacerUserHookLenB = rebarTemplate->m_nIsoFootBotSpacerUserHookLenB;
	m_nIsoFootBotSpacerWidth = rebarTemplate->m_nIsoFootBotSpacerWidth;
}

void OldIsoFootRebarTemplate::OldDataCopy(IsoFootRebarTemplate* rebarTemplate)
{
	rebarTemplate->SetIsoFootTopSpacerSet(m_bIsoFootTopSpacerSet);
	rebarTemplate->SetIsoFootTopSpacerDiaType(m_nIsoFootTopSpacerDiaType);
	rebarTemplate->SetIsoFootTopSpacerSlabThickOverDiaIndex(m_nIsoFootTopSpacerDiaIndex);
	rebarTemplate->SetIsoFootTopSpacerSpacingType(m_nIsoFootTopSpacerSpacingType);
	rebarTemplate->SetIsoFootTopSpacerSlabSpacing(m_nIsoFootTopSpacerSlabSpacing);
	rebarTemplate->SetIsoFootTopSpacerUserSpacing1(m_nIsoFootTopSpacerUserSpacing1);
	rebarTemplate->SetIsoFootTopSpacerUserSpacing2(m_nIsoFootTopSpacerUserSpacing2);
	rebarTemplate->SetIsoFootTopSpacerHeightType(m_nIsoFootTopSpacerHeightType);
	rebarTemplate->SetIsoFootTopSpacerUserHeight(m_nIsoFootTopSpacerUserHeight);
	rebarTemplate->SetIsoFootTopSpacerHookLenAType(m_nIsoFootTopSpacerHookLenAType);
	rebarTemplate->SetIsoFootTopSpacerMainBarHookLenA(m_nIsoFootTopSpacerMainBarHookLenA);
	rebarTemplate->SetIsoFootTopSpacerUserHookLenA(m_nIsoFootTopSpacerUserHookLenA);
	rebarTemplate->SetIsoFootTopSpacerHookLenBType(m_nIsoFootTopSpacerHookLenBType);
	rebarTemplate->SetIsoFootTopSpacerMainBarHookLenB(m_nIsoFootTopSpacerMainBarHookLenB);
	rebarTemplate->SetIsoFootTopSpacerUserHookLenB(m_nIsoFootTopSpacerUserHookLenB);
	rebarTemplate->SetIsoFootTopSpacerWidth(m_nIsoFootTopSpacerWidth);

	rebarTemplate->SetIsoFootBotSpacerSet(m_bIsoFootBotSpacerSet);
	rebarTemplate->SetIsoFootBotSpacerDiaType(m_nIsoFootBotSpacerDiaType);
	rebarTemplate->SetIsoFootBotSpacerDiaIndex(m_nIsoFootBotSpacerDiaIndex);
	rebarTemplate->SetIsoFootBotSpacerSpacingType(m_nIsoFootBotSpacerSpacingType);
	rebarTemplate->SetIsoFootBotSpacerSlabSpacing(m_nIsoFootBotSpacerSlabSpacing);
	rebarTemplate->SetIsoFootBotSpacerUserSpacing1(m_nIsoFootBotSpacerUserSpacing1);
	rebarTemplate->SetIsoFootBotSpacerUserSpacing2(m_nIsoFootBotSpacerUserSpacing2);
	rebarTemplate->SetIsoFootBotSpacerHeightType(m_nIsoFootBotSpacerHeightType);
	rebarTemplate->SetIsoFootBotSpacerUserHeight(m_nIsoFootBotSpacerUserHeight);
	rebarTemplate->SetIsoFootBotSpacerHookLenAType(m_nIsoFootBotSpacerHookLenAType);
	rebarTemplate->SetIsoFootBotSpacerMainBarHookLenA(m_nIsoFootBotSpacerMainBarHookLenA);
	rebarTemplate->SetIsoFootBotSpacerUserHookLenA(m_nIsoFootBotSpacerUserHookLenA);
	rebarTemplate->SetIsoFootBotSpacerHookLenBType(m_nIsoFootBotSpacerHookLenBType);
	rebarTemplate->SetIsoFootBotSpacerMainBarHookLenB(m_nIsoFootBotSpacerMainBarHookLenB);
	rebarTemplate->SetIsoFootBotSpacerUserHookLenB(m_nIsoFootBotSpacerUserHookLenB);
	rebarTemplate->SetIsoFootBotSpacerWidth(m_nIsoFootBotSpacerWidth);
}

void OldIsoFootRebarTemplate::InfoSetCheckBoxChecked()
{
	m_bIsoFootTopSpacerInfoSet = true;
	m_bIsoFootBotSpacerInfoSet = true;
}
