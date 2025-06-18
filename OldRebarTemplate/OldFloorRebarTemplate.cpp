#include "StdAfx.h"
#include "OldFloorRebarTemplate.h"
#include "OldRebarTemplateManager.h"
#include "..\..\GMLib\MSVersionInfo.h"

IMPLEMENT_SERIAL(OldFloorRebarTemplate, OldCommonRebarTemplate, VERSIONABLE_SCHEMA | 1)
OldFloorRebarTemplate::OldFloorRebarTemplate() : OldCommonRebarTemplate()
{
	DataInit();
}

OldFloorRebarTemplate::~OldFloorRebarTemplate(void)
{
}

void OldFloorRebarTemplate::Dump(EFS::EFS_Buffer& buffer)
{
	OldCommonRebarTemplate::Dump(buffer);
	buffer << m_sName;
	buffer << m_nFromStory;
	buffer << m_nToStory;

	m_ColumnRebarTemplate->Dump(buffer);
	m_BeamRebarTemplate->Dump(buffer);
	m_BWallRebarTemplate->Dump(buffer);
	m_RWallRebarTemplate->Dump(buffer);
	m_PitRebarTemplate->Dump(buffer);
	m_SlabRebarTemplate->Dump(buffer);
	m_IsoFootRebarTemplate->Dump(buffer);
	m_WallFootRebarTemplate->Dump(buffer);
}

void OldFloorRebarTemplate::Recovery(EFS::EFS_Buffer& buffer)
{
	OldCommonRebarTemplate::Recovery(buffer);
	buffer >> m_sName;
	buffer >> m_nFromStory;
	buffer >> m_nToStory;

	m_ColumnRebarTemplate->Recovery(buffer);
	m_BeamRebarTemplate->Recovery(buffer);
	m_BWallRebarTemplate->Recovery(buffer);
	m_RWallRebarTemplate->Recovery(buffer);
	m_PitRebarTemplate->Recovery(buffer);
	m_SlabRebarTemplate->Recovery(buffer);
	m_IsoFootRebarTemplate->Recovery(buffer);
	m_WallFootRebarTemplate->Recovery(buffer);
}

void OldFloorRebarTemplate::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;

	OldFloorRebarTemplate* Source = (OldFloorRebarTemplate*)pSource_in;

	m_sName = Source->m_sName;
	m_nFromStory = Source->m_nFromStory;
	m_nToStory = Source->m_nToStory;

	OldCommonRebarTemplate::CopyFromMe(pSource_in);
}

void OldFloorRebarTemplate::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_sName;
		ar << m_nFromStory;
		ar << m_nToStory;
		ar << m_nID;

		m_ColumnRebarTemplate->Serialize(ar);
		m_BeamRebarTemplate->Serialize(ar);
		m_BWallRebarTemplate->Serialize(ar);
		m_RWallRebarTemplate->Serialize(ar);
		m_PitRebarTemplate->Serialize(ar);
		m_SlabRebarTemplate->Serialize(ar);
		m_IsoFootRebarTemplate->Serialize(ar);
		m_WallFootRebarTemplate->Serialize(ar);
	}
	else
	{
		ar >> m_sName;
		ar >> m_nFromStory;
		ar >> m_nToStory;
		ar >> m_nID;

		m_ColumnRebarTemplate =(OldColumnRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldColumn);
		m_ColumnRebarTemplate->Serialize(ar);

		m_BeamRebarTemplate = (OldBeamRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldBeam);
		m_BeamRebarTemplate->Serialize(ar);

		m_BWallRebarTemplate = (OldBWallRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldBWall);
		m_BWallRebarTemplate->Serialize(ar);

		m_RWallRebarTemplate = (OldRWallRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldRWall);
		m_RWallRebarTemplate->Serialize(ar);

		m_PitRebarTemplate = (OldPitRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldPit);
		m_PitRebarTemplate->Serialize(ar);

		m_SlabRebarTemplate = (OldSlabRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldSlab);
		m_SlabRebarTemplate->Serialize(ar);

		m_IsoFootRebarTemplate = (OldIsoFootRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldIsoFoot);
		m_WallFootRebarTemplate = (OldWallFootRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldWallFoot);

		if (MSVersionInfo::GetCurrentVersion() >= 20170621)
		{
			m_IsoFootRebarTemplate->Serialize(ar);
			m_WallFootRebarTemplate->Serialize(ar);
		}

		OldRebarTemplateManager::SetLastID(m_nID);
	}
}

void OldFloorRebarTemplate::DataInit()
{
	m_sName = _T("");
	m_nFromStory = 0;
	m_nToStory = 0;
	m_nID = -1;
}

bool OldFloorRebarTemplate::TopOptionDataCopy(OldCommonRebarTemplate* commonRebarTemplate)
{
	bool bDataCopy = false;

	bDataCopy = m_ColumnRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_ColumnRebarTemplate);
	bDataCopy = m_BeamRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_BeamRebarTemplate);
	bDataCopy = m_BWallRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_BWallRebarTemplate);
	bDataCopy = m_RWallRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_RWallRebarTemplate);
	bDataCopy = m_PitRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_PitRebarTemplate);
	bDataCopy = m_SlabRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_SlabRebarTemplate);
	bDataCopy = m_IsoFootRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_IsoFootRebarTemplate);
	bDataCopy = m_WallFootRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_WallFootRebarTemplate);

	return bDataCopy;
}

void OldFloorRebarTemplate::CreateMemberAndDataCopy(OldCommonRebarTemplate* commonRebarTemplate)
{
	m_ColumnRebarTemplate = (OldColumnRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldColumn);
	m_ColumnRebarTemplate->DataCopy(commonRebarTemplate->m_ColumnRebarTemplate);

	m_BeamRebarTemplate = (OldBeamRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldBeam);
	m_BeamRebarTemplate->DataCopy(commonRebarTemplate->m_BeamRebarTemplate);

	m_BWallRebarTemplate = (OldBWallRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldBWall);
	m_BWallRebarTemplate->DataCopy(commonRebarTemplate->m_BWallRebarTemplate);

	m_RWallRebarTemplate = (OldRWallRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldRWall);
	m_RWallRebarTemplate->DataCopy(commonRebarTemplate->m_RWallRebarTemplate);

	m_PitRebarTemplate = (OldPitRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldPit);
	m_PitRebarTemplate->DataCopy(commonRebarTemplate->m_PitRebarTemplate);

	m_SlabRebarTemplate = (OldSlabRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldSlab);
	m_SlabRebarTemplate->DataCopy(commonRebarTemplate->m_SlabRebarTemplate);

	m_IsoFootRebarTemplate = (OldIsoFootRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldIsoFoot);
	m_IsoFootRebarTemplate->DataCopy(commonRebarTemplate->m_IsoFootRebarTemplate);

	m_WallFootRebarTemplate = (OldWallFootRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldWallFoot);
	m_WallFootRebarTemplate->DataCopy(commonRebarTemplate->m_WallFootRebarTemplate);
}

