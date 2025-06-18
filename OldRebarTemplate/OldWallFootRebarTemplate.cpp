#include "StdAfx.h"
#include "OldWallFootRebarTemplate.h"
#include "..\MSDeformedBar.h"
#include "..\..\GMLib\MSVersionInfo.h"

IMPLEMENT_SERIAL(OldWallFootRebarTemplate, OldBaseRebarTemplate, VERSIONABLE_SCHEMA | 1)
OldWallFootRebarTemplate::OldWallFootRebarTemplate() :OldBaseRebarTemplate()
{
	DataInit();
}

OldWallFootRebarTemplate::~OldWallFootRebarTemplate(void)
{
}

void OldWallFootRebarTemplate::CopyFromMe(OldWallFootRebarTemplate* pWallFootRebarTemplate)
{

}

void OldWallFootRebarTemplate::Dump(EFS::EFS_Buffer& buffer)
{
	buffer << m_bWallFootTopSpacerInfoSet;
	buffer << m_bWallFootTopSpacerSet;
	buffer << m_nWallFootTopSpacerDiaType;
	buffer << m_nWallFootTopSpacerDiaIndex;
	buffer << m_nWallFootTopSpacerSpacingType;
	buffer << m_nWallFootTopSpacerSlabSpacing;
	buffer << m_nWallFootTopSpacerUserSpacing1;
	buffer << m_nWallFootTopSpacerUserSpacing2;
	buffer << m_nWallFootTopSpacerHeightType;
	buffer << m_nWallFootTopSpacerUserHeight;
	buffer << m_nWallFootTopSpacerHookLenAType;
	buffer << m_nWallFootTopSpacerMainBarHookLenA;
	buffer << m_nWallFootTopSpacerUserHookLenA;
	buffer << m_nWallFootTopSpacerHookLenBType;
	buffer << m_nWallFootTopSpacerMainBarHookLenB;
	buffer << m_nWallFootTopSpacerUserHookLenB;
	buffer << m_nWallFootTopSpacerWidth;

	buffer << m_bWallFootBotSpacerInfoSet;
	buffer << m_bWallFootBotSpacerSet;
	buffer << m_nWallFootBotSpacerDiaType;
	buffer << m_nWallFootBotSpacerDiaIndex;
	buffer << m_nWallFootBotSpacerSpacingType;
	buffer << m_nWallFootBotSpacerSlabSpacing;
	buffer << m_nWallFootBotSpacerUserSpacing1;
	buffer << m_nWallFootBotSpacerUserSpacing2;
	buffer << m_nWallFootBotSpacerHeightType;
	buffer << m_nWallFootBotSpacerUserHeight;
	buffer << m_nWallFootBotSpacerHookLenAType;
	buffer << m_nWallFootBotSpacerMainBarHookLenA;
	buffer << m_nWallFootBotSpacerUserHookLenA;
	buffer << m_nWallFootBotSpacerHookLenBType;
	buffer << m_nWallFootBotSpacerMainBarHookLenB;
	buffer << m_nWallFootBotSpacerUserHookLenB;
	buffer << m_nWallFootBotSpacerWidth;
}

void OldWallFootRebarTemplate::Recovery(EFS::EFS_Buffer& buffer)
{
	buffer >> m_bWallFootTopSpacerInfoSet;
	buffer >> m_bWallFootTopSpacerSet;
	buffer >> m_nWallFootTopSpacerDiaType;
	buffer >> m_nWallFootTopSpacerDiaIndex;
	buffer >> m_nWallFootTopSpacerSpacingType;
	buffer >> m_nWallFootTopSpacerSlabSpacing;
	buffer >> m_nWallFootTopSpacerUserSpacing1;
	buffer >> m_nWallFootTopSpacerUserSpacing2;
	buffer >> m_nWallFootTopSpacerHeightType;
	buffer >> m_nWallFootTopSpacerUserHeight;
	buffer >> m_nWallFootTopSpacerHookLenAType;
	buffer >> m_nWallFootTopSpacerMainBarHookLenA;
	buffer >> m_nWallFootTopSpacerUserHookLenA;
	buffer >> m_nWallFootTopSpacerHookLenBType;
	buffer >> m_nWallFootTopSpacerMainBarHookLenB;
	buffer >> m_nWallFootTopSpacerUserHookLenB;
	buffer >> m_nWallFootTopSpacerWidth;

	buffer >> m_bWallFootBotSpacerInfoSet;
	buffer >> m_bWallFootBotSpacerSet;
	buffer >> m_nWallFootBotSpacerDiaType;
	buffer >> m_nWallFootBotSpacerDiaIndex;
	buffer >> m_nWallFootBotSpacerSpacingType;
	buffer >> m_nWallFootBotSpacerSlabSpacing;
	buffer >> m_nWallFootBotSpacerUserSpacing1;
	buffer >> m_nWallFootBotSpacerUserSpacing2;
	buffer >> m_nWallFootBotSpacerHeightType;
	buffer >> m_nWallFootBotSpacerUserHeight;
	buffer >> m_nWallFootBotSpacerHookLenAType;
	buffer >> m_nWallFootBotSpacerMainBarHookLenA;
	buffer >> m_nWallFootBotSpacerUserHookLenA;
	buffer >> m_nWallFootBotSpacerHookLenBType;
	buffer >> m_nWallFootBotSpacerMainBarHookLenB;
	buffer >> m_nWallFootBotSpacerUserHookLenB;
	buffer >> m_nWallFootBotSpacerWidth;
}

void OldWallFootRebarTemplate::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_bWallFootTopSpacerInfoSet;
		ar << m_bWallFootTopSpacerSet;
		ar << m_nWallFootTopSpacerDiaType;
		ar << m_nWallFootTopSpacerDiaIndex;
		ar << m_nWallFootTopSpacerSpacingType;
		ar << m_nWallFootTopSpacerSlabSpacing;
		ar << m_nWallFootTopSpacerUserSpacing1;
		ar << m_nWallFootTopSpacerUserSpacing2;
		ar << m_nWallFootTopSpacerHeightType;
		ar << m_nWallFootTopSpacerUserHeight;
		ar << m_nWallFootTopSpacerHookLenAType;
		ar << m_nWallFootTopSpacerMainBarHookLenA;
		ar << m_nWallFootTopSpacerUserHookLenA;
		ar << m_nWallFootTopSpacerHookLenBType;
		ar << m_nWallFootTopSpacerMainBarHookLenB;
		ar << m_nWallFootTopSpacerUserHookLenB;
		ar << m_nWallFootTopSpacerWidth;

		ar << m_bWallFootBotSpacerInfoSet;
		ar << m_bWallFootBotSpacerSet;
		ar << m_nWallFootBotSpacerDiaType;
		ar << m_nWallFootBotSpacerDiaIndex;
		ar << m_nWallFootBotSpacerSpacingType;
		ar << m_nWallFootBotSpacerSlabSpacing;
		ar << m_nWallFootBotSpacerUserSpacing1;
		ar << m_nWallFootBotSpacerUserSpacing2;
		ar << m_nWallFootBotSpacerHeightType;
		ar << m_nWallFootBotSpacerUserHeight;
		ar << m_nWallFootBotSpacerHookLenAType;
		ar << m_nWallFootBotSpacerMainBarHookLenA;
		ar << m_nWallFootBotSpacerUserHookLenA;
		ar << m_nWallFootBotSpacerHookLenBType;
		ar << m_nWallFootBotSpacerMainBarHookLenB;
		ar << m_nWallFootBotSpacerUserHookLenB;
		ar << m_nWallFootBotSpacerWidth;
	}
	else
	{
		ar >> m_bWallFootTopSpacerInfoSet;
		ar >> m_bWallFootTopSpacerSet;
		ar >> m_nWallFootTopSpacerDiaType;
		ar >> m_nWallFootTopSpacerDiaIndex;
		ar >> m_nWallFootTopSpacerSpacingType;
		ar >> m_nWallFootTopSpacerSlabSpacing;
		ar >> m_nWallFootTopSpacerUserSpacing1;
		ar >> m_nWallFootTopSpacerUserSpacing2;
		ar >> m_nWallFootTopSpacerHeightType;
		ar >> m_nWallFootTopSpacerUserHeight;
		ar >> m_nWallFootTopSpacerHookLenAType;
		ar >> m_nWallFootTopSpacerMainBarHookLenA;
		ar >> m_nWallFootTopSpacerUserHookLenA;
		ar >> m_nWallFootTopSpacerHookLenBType;
		ar >> m_nWallFootTopSpacerMainBarHookLenB;
		ar >> m_nWallFootTopSpacerUserHookLenB;
		ar >> m_nWallFootTopSpacerWidth;

		ar >> m_bWallFootBotSpacerInfoSet;
		ar >> m_bWallFootBotSpacerSet;
		ar >> m_nWallFootBotSpacerDiaType;
		ar >> m_nWallFootBotSpacerDiaIndex;
		ar >> m_nWallFootBotSpacerSpacingType;
		ar >> m_nWallFootBotSpacerSlabSpacing;
		ar >> m_nWallFootBotSpacerUserSpacing1;
		ar >> m_nWallFootBotSpacerUserSpacing2;
		ar >> m_nWallFootBotSpacerHeightType;
		ar >> m_nWallFootBotSpacerUserHeight;
		ar >> m_nWallFootBotSpacerHookLenAType;
		ar >> m_nWallFootBotSpacerMainBarHookLenA;
		ar >> m_nWallFootBotSpacerUserHookLenA;
		ar >> m_nWallFootBotSpacerHookLenBType;
		ar >> m_nWallFootBotSpacerMainBarHookLenB;
		ar >> m_nWallFootBotSpacerUserHookLenB;
		ar >> m_nWallFootBotSpacerWidth;
	}
}

void OldWallFootRebarTemplate::DataInit()
{
	m_bWallFootTopSpacerInfoSet = false;
	m_bWallFootTopSpacerSet = false;
	m_nWallFootTopSpacerDiaType = 0;
	m_nWallFootTopSpacerDiaIndex = 0;
	m_nWallFootTopSpacerSpacingType = 1;
	m_nWallFootTopSpacerSlabSpacing = 3;
	m_nWallFootTopSpacerUserSpacing1 = 1500;
	m_nWallFootTopSpacerUserSpacing2 = 1500;
	m_nWallFootTopSpacerHeightType = 0;
	m_nWallFootTopSpacerUserHeight = 300;
	m_nWallFootTopSpacerHookLenAType = 1;
	m_nWallFootTopSpacerMainBarHookLenA = 100;
	m_nWallFootTopSpacerUserHookLenA = 100;
	m_nWallFootTopSpacerHookLenBType = 1;
	m_nWallFootTopSpacerMainBarHookLenB = 0;
	m_nWallFootTopSpacerUserHookLenB = 100;
	m_nWallFootTopSpacerWidth = 200;

	m_bWallFootBotSpacerInfoSet = false;
	m_bWallFootBotSpacerSet = false;
	m_nWallFootBotSpacerDiaType = 0;
	m_nWallFootBotSpacerDiaIndex = 0;
	m_nWallFootBotSpacerSpacingType = 1;
	m_nWallFootBotSpacerSlabSpacing = 3;
	m_nWallFootBotSpacerUserSpacing1 = 1500;
	m_nWallFootBotSpacerUserSpacing2 = 1500;
	m_nWallFootBotSpacerHeightType = 0;
	m_nWallFootBotSpacerUserHeight = 300;
	m_nWallFootBotSpacerHookLenAType = 1;
	m_nWallFootBotSpacerMainBarHookLenA = 100;
	m_nWallFootBotSpacerUserHookLenA = 100;
	m_nWallFootBotSpacerHookLenBType = 1;
	m_nWallFootBotSpacerMainBarHookLenB = 0;
	m_nWallFootBotSpacerUserHookLenB = 100;
	m_nWallFootBotSpacerWidth = 200;
}

bool OldWallFootRebarTemplate::TopOptionDataCopy(OldWallFootRebarTemplate* rebarTemplate)
{
	bool bDataCopy = false;

	if (!m_bWallFootTopSpacerInfoSet)
	{
		m_bWallFootTopSpacerSet = rebarTemplate->m_bWallFootTopSpacerSet;
		m_nWallFootTopSpacerDiaType = rebarTemplate->m_nWallFootTopSpacerDiaType;
		m_nWallFootTopSpacerDiaIndex = rebarTemplate->m_nWallFootTopSpacerDiaIndex;
		m_nWallFootTopSpacerSpacingType = rebarTemplate->m_nWallFootTopSpacerSpacingType;
		m_nWallFootTopSpacerSlabSpacing = rebarTemplate->m_nWallFootTopSpacerSlabSpacing;
		m_nWallFootTopSpacerUserSpacing1 = rebarTemplate->m_nWallFootTopSpacerUserSpacing1;
		m_nWallFootTopSpacerUserSpacing2 = rebarTemplate->m_nWallFootTopSpacerUserSpacing2;
		m_nWallFootTopSpacerHeightType = rebarTemplate->m_nWallFootTopSpacerHeightType;
		m_nWallFootTopSpacerUserHeight = rebarTemplate->m_nWallFootTopSpacerUserHeight;
		m_nWallFootTopSpacerHookLenAType = rebarTemplate->m_nWallFootTopSpacerHookLenAType;
		m_nWallFootTopSpacerMainBarHookLenA = rebarTemplate->m_nWallFootTopSpacerMainBarHookLenA;
		m_nWallFootTopSpacerUserHookLenA = rebarTemplate->m_nWallFootTopSpacerUserHookLenA;
		m_nWallFootTopSpacerHookLenBType = rebarTemplate->m_nWallFootTopSpacerHookLenBType;
		m_nWallFootTopSpacerMainBarHookLenB = rebarTemplate->m_nWallFootTopSpacerMainBarHookLenB;
		m_nWallFootTopSpacerUserHookLenB = rebarTemplate->m_nWallFootTopSpacerUserHookLenB;
		m_nWallFootTopSpacerWidth = rebarTemplate->m_nWallFootTopSpacerWidth;
	}

	if (!m_bWallFootBotSpacerInfoSet)
	{
		m_bWallFootBotSpacerSet = rebarTemplate->m_bWallFootBotSpacerSet;
		m_nWallFootBotSpacerDiaType = rebarTemplate->m_nWallFootBotSpacerDiaType;
		m_nWallFootBotSpacerDiaIndex = rebarTemplate->m_nWallFootBotSpacerDiaIndex;
		m_nWallFootBotSpacerSpacingType = rebarTemplate->m_nWallFootBotSpacerSpacingType;
		m_nWallFootBotSpacerSlabSpacing = rebarTemplate->m_nWallFootBotSpacerSlabSpacing;
		m_nWallFootBotSpacerUserSpacing1 = rebarTemplate->m_nWallFootBotSpacerUserSpacing1;
		m_nWallFootBotSpacerUserSpacing2 = rebarTemplate->m_nWallFootBotSpacerUserSpacing2;
		m_nWallFootBotSpacerHeightType = rebarTemplate->m_nWallFootBotSpacerHeightType;
		m_nWallFootBotSpacerUserHeight = rebarTemplate->m_nWallFootBotSpacerUserHeight;
		m_nWallFootBotSpacerHookLenAType = rebarTemplate->m_nWallFootBotSpacerHookLenAType;
		m_nWallFootBotSpacerMainBarHookLenA = rebarTemplate->m_nWallFootBotSpacerMainBarHookLenA;
		m_nWallFootBotSpacerUserHookLenA = rebarTemplate->m_nWallFootBotSpacerUserHookLenA;
		m_nWallFootBotSpacerHookLenBType = rebarTemplate->m_nWallFootBotSpacerHookLenBType;
		m_nWallFootBotSpacerMainBarHookLenB = rebarTemplate->m_nWallFootBotSpacerMainBarHookLenB;
		m_nWallFootBotSpacerUserHookLenB = rebarTemplate->m_nWallFootBotSpacerUserHookLenB;
		m_nWallFootBotSpacerWidth = rebarTemplate->m_nWallFootBotSpacerWidth;
	}

	return bDataCopy;
}

void OldWallFootRebarTemplate::DataCopy(OldWallFootRebarTemplate* rebarTemplate)
{
	m_bWallFootTopSpacerSet = rebarTemplate->m_bWallFootTopSpacerSet;
	m_nWallFootTopSpacerDiaType = rebarTemplate->m_nWallFootTopSpacerDiaType;
	m_nWallFootTopSpacerDiaIndex = rebarTemplate->m_nWallFootTopSpacerDiaIndex;
	m_nWallFootTopSpacerSpacingType = rebarTemplate->m_nWallFootTopSpacerSpacingType;
	m_nWallFootTopSpacerSlabSpacing = rebarTemplate->m_nWallFootTopSpacerSlabSpacing;
	m_nWallFootTopSpacerUserSpacing1 = rebarTemplate->m_nWallFootTopSpacerUserSpacing1;
	m_nWallFootTopSpacerUserSpacing2 = rebarTemplate->m_nWallFootTopSpacerUserSpacing2;
	m_nWallFootTopSpacerHeightType = rebarTemplate->m_nWallFootTopSpacerHeightType;
	m_nWallFootTopSpacerUserHeight = rebarTemplate->m_nWallFootTopSpacerUserHeight;
	m_nWallFootTopSpacerHookLenAType = rebarTemplate->m_nWallFootTopSpacerHookLenAType;
	m_nWallFootTopSpacerMainBarHookLenA = rebarTemplate->m_nWallFootTopSpacerMainBarHookLenA;
	m_nWallFootTopSpacerUserHookLenA = rebarTemplate->m_nWallFootTopSpacerUserHookLenA;
	m_nWallFootTopSpacerHookLenBType = rebarTemplate->m_nWallFootTopSpacerHookLenBType;
	m_nWallFootTopSpacerMainBarHookLenB = rebarTemplate->m_nWallFootTopSpacerMainBarHookLenB;
	m_nWallFootTopSpacerUserHookLenB = rebarTemplate->m_nWallFootTopSpacerUserHookLenB;
	m_nWallFootTopSpacerWidth = rebarTemplate->m_nWallFootTopSpacerWidth;

	m_bWallFootBotSpacerSet = rebarTemplate->m_bWallFootBotSpacerSet;
	m_nWallFootBotSpacerDiaType = rebarTemplate->m_nWallFootBotSpacerDiaType;
	m_nWallFootBotSpacerDiaIndex = rebarTemplate->m_nWallFootBotSpacerDiaIndex;
	m_nWallFootBotSpacerSpacingType = rebarTemplate->m_nWallFootBotSpacerSpacingType;
	m_nWallFootBotSpacerSlabSpacing = rebarTemplate->m_nWallFootBotSpacerSlabSpacing;
	m_nWallFootBotSpacerUserSpacing1 = rebarTemplate->m_nWallFootBotSpacerUserSpacing1;
	m_nWallFootBotSpacerUserSpacing2 = rebarTemplate->m_nWallFootBotSpacerUserSpacing2;
	m_nWallFootBotSpacerHeightType = rebarTemplate->m_nWallFootBotSpacerHeightType;
	m_nWallFootBotSpacerUserHeight = rebarTemplate->m_nWallFootBotSpacerUserHeight;
	m_nWallFootBotSpacerHookLenAType = rebarTemplate->m_nWallFootBotSpacerHookLenAType;
	m_nWallFootBotSpacerMainBarHookLenA = rebarTemplate->m_nWallFootBotSpacerMainBarHookLenA;
	m_nWallFootBotSpacerUserHookLenA = rebarTemplate->m_nWallFootBotSpacerUserHookLenA;
	m_nWallFootBotSpacerHookLenBType = rebarTemplate->m_nWallFootBotSpacerHookLenBType;
	m_nWallFootBotSpacerMainBarHookLenB = rebarTemplate->m_nWallFootBotSpacerMainBarHookLenB;
	m_nWallFootBotSpacerUserHookLenB = rebarTemplate->m_nWallFootBotSpacerUserHookLenB;
	m_nWallFootBotSpacerWidth = rebarTemplate->m_nWallFootBotSpacerWidth;
}

void OldWallFootRebarTemplate::OldDataCopy(WallFootRebarTemplate* rebarTemplate)
{
	rebarTemplate->SetWallFootTopSpacerSet(m_bWallFootTopSpacerSet);
	rebarTemplate->SetWallFootTopSpacerDiaType(m_nWallFootTopSpacerDiaType);
	rebarTemplate->SetWallFootTopSpacerSlabThickOverDiaIndex(m_nWallFootTopSpacerDiaIndex);
	rebarTemplate->SetWallFootTopSpacerSpacingType(m_nWallFootTopSpacerSpacingType);
	rebarTemplate->SetWallFootTopSpacerSlabSpacing(m_nWallFootTopSpacerSlabSpacing);
	rebarTemplate->SetWallFootTopSpacerUserSpacing1(m_nWallFootTopSpacerUserSpacing1);
	rebarTemplate->SetWallFootTopSpacerUserSpacing2(m_nWallFootTopSpacerUserSpacing2);
	rebarTemplate->SetWallFootTopSpacerHeightType(m_nWallFootTopSpacerHeightType);
	rebarTemplate->SetWallFootTopSpacerUserHeight(m_nWallFootTopSpacerUserHeight);
	rebarTemplate->SetWallFootTopSpacerHookLenAType(m_nWallFootTopSpacerHookLenAType);
	rebarTemplate->SetWallFootTopSpacerMainBarHookLenA(m_nWallFootTopSpacerMainBarHookLenA);
	rebarTemplate->SetWallFootTopSpacerUserHookLenA(m_nWallFootTopSpacerUserHookLenA);
	rebarTemplate->SetWallFootTopSpacerHookLenBType(m_nWallFootTopSpacerHookLenBType);
	rebarTemplate->SetWallFootTopSpacerMainBarHookLenB(m_nWallFootTopSpacerMainBarHookLenB);
	rebarTemplate->SetWallFootTopSpacerUserHookLenB(m_nWallFootTopSpacerUserHookLenB);
	rebarTemplate->SetWallFootTopSpacerWidth(m_nWallFootTopSpacerWidth);

	rebarTemplate->SetWallFootBotSpacerSet(m_bWallFootBotSpacerSet);
	rebarTemplate->SetWallFootBotSpacerDiaType(m_nWallFootBotSpacerDiaType);
	rebarTemplate->SetWallFootBotSpacerDiaIndex(m_nWallFootBotSpacerDiaIndex);
	rebarTemplate->SetWallFootBotSpacerSpacingType(m_nWallFootBotSpacerSpacingType);
	rebarTemplate->SetWallFootBotSpacerSlabSpacing(m_nWallFootBotSpacerSlabSpacing);
	rebarTemplate->SetWallFootBotSpacerUserSpacing1(m_nWallFootBotSpacerUserSpacing1);
	rebarTemplate->SetWallFootBotSpacerUserSpacing2(m_nWallFootBotSpacerUserSpacing2);
	rebarTemplate->SetWallFootBotSpacerHeightType(m_nWallFootBotSpacerHeightType);
	rebarTemplate->SetWallFootBotSpacerUserHeight(m_nWallFootBotSpacerUserHeight);
	rebarTemplate->SetWallFootBotSpacerHookLenAType(m_nWallFootBotSpacerHookLenAType);
	rebarTemplate->SetWallFootBotSpacerMainBarHookLenA(m_nWallFootBotSpacerMainBarHookLenA);
	rebarTemplate->SetWallFootBotSpacerUserHookLenA(m_nWallFootBotSpacerUserHookLenA);
	rebarTemplate->SetWallFootBotSpacerHookLenBType(m_nWallFootBotSpacerHookLenBType);
	rebarTemplate->SetWallFootBotSpacerMainBarHookLenB(m_nWallFootBotSpacerMainBarHookLenB);
	rebarTemplate->SetWallFootBotSpacerUserHookLenB(m_nWallFootBotSpacerUserHookLenB);
	rebarTemplate->SetWallFootBotSpacerWidth(m_nWallFootBotSpacerWidth);
}

void OldWallFootRebarTemplate::InfoSetCheckBoxChecked()
{
	m_bWallFootTopSpacerInfoSet = true;
	m_bWallFootBotSpacerInfoSet = true;
}
