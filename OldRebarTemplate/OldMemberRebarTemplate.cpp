#include "StdAfx.h"
#include "OldMemberRebarTemplate.h"
#include "OldRebarTemplateManager.h"
#include "..\MSObject.h"

IMPLEMENT_SERIAL(OldMemberRebarTemplate, OldCommonRebarTemplate, VERSIONABLE_SCHEMA | 1)
OldMemberRebarTemplate::OldMemberRebarTemplate() : OldCommonRebarTemplate()
{
	DataInit();
}

OldMemberRebarTemplate::~OldMemberRebarTemplate(void)
{
}

void OldMemberRebarTemplate::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;

	OldMemberRebarTemplate* Source = (OldMemberRebarTemplate*)pSource_in;

	m_sName = Source->m_sName;
	m_nMemberType = Source->m_nMemberType;
	m_nTopOptionID = Source->m_nTopOptionID;

	if (m_nMemberType == RT_OldColumn)
		m_ColumnRebarTemplate->CopyFromMe(&*Source->m_ColumnRebarTemplate);
	else if (m_nMemberType == RT_OldBeam)
		m_BeamRebarTemplate->CopyFromMe(&*Source->m_BeamRebarTemplate);
	else if (m_nMemberType == RT_OldBWall)
		m_BWallRebarTemplate->CopyFromMe(&*Source->m_BWallRebarTemplate);
	else if (m_nMemberType == RT_OldRWall)
		m_RWallRebarTemplate->CopyFromMe(&*Source->m_RWallRebarTemplate);
	else if (m_nMemberType == RT_OldPit)
		m_PitRebarTemplate->CopyFromMe(&*Source->m_PitRebarTemplate);
	else if (m_nMemberType == RT_OldSlab)
		m_SlabRebarTemplate->CopyFromMe(&*Source->m_SlabRebarTemplate);
	else if (m_nMemberType == RT_OldIsoFoot)
		m_IsoFootRebarTemplate->CopyFromMe(&*Source->m_IsoFootRebarTemplate);
	else //if (m_nMemberType == RT_OldWallFoot)
		m_WallFootRebarTemplate->CopyFromMe(&*Source->m_WallFootRebarTemplate);
}

void OldMemberRebarTemplate::Dump(EFS::EFS_Buffer& buffer)
{
	OldCommonRebarTemplate::Dump(buffer);
	buffer << m_sName;
	buffer << m_nMemberType;
	buffer << m_nTopOptionID;

	if (m_nMemberType == RT_OldColumn)
		m_ColumnRebarTemplate->Dump(buffer);
	else if (m_nMemberType == RT_OldBeam)
		m_BeamRebarTemplate->Dump(buffer);
	else if (m_nMemberType == RT_OldBWall)
		m_BWallRebarTemplate->Dump(buffer);
	else if (m_nMemberType == RT_OldRWall)
		m_RWallRebarTemplate->Dump(buffer);
	else if (m_nMemberType == RT_OldPit)
		m_PitRebarTemplate->Dump(buffer);
	else if (m_nMemberType == RT_OldSlab)
		m_SlabRebarTemplate->Dump(buffer);
	else if (m_nMemberType == RT_OldIsoFoot)
		m_IsoFootRebarTemplate->Dump(buffer);
	else //if (m_nMemberType == RT_OldWallFoot)
		m_WallFootRebarTemplate->Dump(buffer);
}

void OldMemberRebarTemplate::Recovery(EFS::EFS_Buffer& buffer)
{
	OldCommonRebarTemplate::Recovery(buffer);
	buffer >> m_sName;
	buffer >> m_nMemberType;
	buffer >> m_nTopOptionID;

	if (m_nMemberType == RT_OldColumn)
		m_ColumnRebarTemplate->Recovery(buffer);
	else if (m_nMemberType == RT_OldBeam)
		m_BeamRebarTemplate->Recovery(buffer);
	else if (m_nMemberType == RT_OldBWall)
		m_BWallRebarTemplate->Recovery(buffer);
	else if (m_nMemberType == RT_OldRWall)
		m_RWallRebarTemplate->Recovery(buffer);
	else if (m_nMemberType == RT_OldPit)
		m_PitRebarTemplate->Recovery(buffer);
	else if (m_nMemberType == RT_OldSlab)
		m_SlabRebarTemplate->Recovery(buffer);
	else if (m_nMemberType == RT_OldIsoFoot)
		m_IsoFootRebarTemplate->Recovery(buffer);
	else //if (m_nMemberType == RT_OldWallFoot)
		m_WallFootRebarTemplate->Recovery(buffer);
}

void OldMemberRebarTemplate::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_sName;
		ar << m_nMemberType;
		ar << m_nID;
		ar << m_nTopOptionID;

		if (m_nMemberType == RT_OldColumn)
			m_ColumnRebarTemplate->Serialize(ar);
		else if (m_nMemberType == RT_OldBeam)
			m_BeamRebarTemplate->Serialize(ar);
		else if (m_nMemberType == RT_OldBWall)
			m_BWallRebarTemplate->Serialize(ar);
		else if (m_nMemberType == RT_OldRWall)
			m_RWallRebarTemplate->Serialize(ar);
		else if (m_nMemberType == RT_OldPit)
			m_PitRebarTemplate->Serialize(ar);
		else if (m_nMemberType == RT_OldSlab)
			m_SlabRebarTemplate->Serialize(ar);
		else if (m_nMemberType == RT_OldIsoFoot)
			m_IsoFootRebarTemplate->Serialize(ar);
		else //if (m_nMemberType == RT_OldWallFoot)
			m_WallFootRebarTemplate->Serialize(ar);
	}
	else
	{
		ar >> m_sName;
		ar >> m_nMemberType;
		ar >> m_nID;
		ar >> m_nTopOptionID;

		if (m_nMemberType == RT_OldColumn)
		{
			m_ColumnRebarTemplate = (OldColumnRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldColumn);
			m_ColumnRebarTemplate->Serialize(ar);
		}
		else if (m_nMemberType == RT_OldBeam)
		{
			m_BeamRebarTemplate = (OldBeamRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldBeam);
			m_BeamRebarTemplate->Serialize(ar);
		}
		else if (m_nMemberType == RT_OldBWall)
		{
			m_BWallRebarTemplate = (OldBWallRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldBWall);
			m_BWallRebarTemplate->Serialize(ar);
		}
		else if (m_nMemberType == RT_OldRWall)
		{
			m_RWallRebarTemplate = (OldRWallRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldRWall);
			m_RWallRebarTemplate->Serialize(ar);
		}
		else if (m_nMemberType == RT_OldPit)
		{
			m_PitRebarTemplate = (OldPitRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldPit);
			m_PitRebarTemplate->Serialize(ar);
		}
		else if (m_nMemberType == RT_OldSlab)
		{
			m_SlabRebarTemplate = (OldSlabRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldSlab);
			m_SlabRebarTemplate->Serialize(ar);
		}
		else if (m_nMemberType == RT_OldIsoFoot)
		{
			m_IsoFootRebarTemplate = (OldIsoFootRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldIsoFoot);
			m_IsoFootRebarTemplate->Serialize(ar);
		}
		else //if (m_nMemberType == RT_OldWallFoot)
		{
			m_WallFootRebarTemplate = (OldWallFootRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldWallFoot);
			m_WallFootRebarTemplate->Serialize(ar);
		}

		OldRebarTemplateManager::SetLastID(m_nID);
	}
}

void OldMemberRebarTemplate::DataInit()
{
	m_sName = _T("");
	m_nMemberType = -1;
	m_nID = -1;
	m_nTopOptionID = -1;
}

void OldMemberRebarTemplate::TopOptionDataCopy(OldCommonRebarTemplate* commonRebarTemplate)
{
	if (m_nMemberType == RT_OldColumn)
	{
		m_ColumnRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_ColumnRebarTemplate);
	}
	else if (m_nMemberType == RT_OldBeam)
	{
		m_BeamRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_BeamRebarTemplate);
	}
	else if (m_nMemberType == RT_OldBWall)
	{
		m_BWallRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_BWallRebarTemplate);
	}
	else if (m_nMemberType == RT_OldRWall)
	{
		m_RWallRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_RWallRebarTemplate);
	}
	else if (m_nMemberType == RT_OldPit)
	{
		m_PitRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_PitRebarTemplate);
	}
	else if (m_nMemberType == RT_OldSlab)
	{
		m_SlabRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_SlabRebarTemplate);
	}
	else if (m_nMemberType == RT_OldIsoFoot)
	{
		m_IsoFootRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_IsoFootRebarTemplate);
	}
	else //if (m_nMemberType == RT_OldWallFoot)
	{
		m_WallFootRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_WallFootRebarTemplate);
	}
}

void OldMemberRebarTemplate::CreateMemberAndDataCopy(OldCommonRebarTemplate* commonRebarTemplate)
{
	if (m_nMemberType == RT_OldColumn)
	{
		m_ColumnRebarTemplate = (OldColumnRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldColumn);
		m_ColumnRebarTemplate->DataCopy(commonRebarTemplate->m_ColumnRebarTemplate);
	}
	else if (m_nMemberType == RT_OldBeam)
	{
		m_BeamRebarTemplate = (OldBeamRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldBeam);
		m_BeamRebarTemplate->DataCopy(commonRebarTemplate->m_BeamRebarTemplate);
	}
	else if (m_nMemberType == RT_OldBWall)
	{
		m_BWallRebarTemplate =(OldBWallRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldBWall);
		m_BWallRebarTemplate->DataCopy(commonRebarTemplate->m_BWallRebarTemplate);
	}
	else if (m_nMemberType == RT_OldRWall)
	{
		m_RWallRebarTemplate = (OldRWallRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldRWall);
		m_RWallRebarTemplate->DataCopy(commonRebarTemplate->m_RWallRebarTemplate);
	}
	else if (m_nMemberType == RT_OldPit)
	{
		m_PitRebarTemplate = (OldPitRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldPit);
		m_PitRebarTemplate->DataCopy(commonRebarTemplate->m_PitRebarTemplate);
	}
	else if (m_nMemberType == RT_OldSlab)
	{
		m_SlabRebarTemplate = (OldSlabRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldSlab);
		m_SlabRebarTemplate->DataCopy(commonRebarTemplate->m_SlabRebarTemplate);
	}
	else if (m_nMemberType == RT_OldIsoFoot)
	{
		m_IsoFootRebarTemplate = (OldIsoFootRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldIsoFoot);
		m_IsoFootRebarTemplate->DataCopy(commonRebarTemplate->m_IsoFootRebarTemplate);
	}
	else //if (m_nMemberType == RT_OldWallFoot)
	{
		m_WallFootRebarTemplate = (OldWallFootRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldWallFoot);
		m_WallFootRebarTemplate->DataCopy(commonRebarTemplate->m_WallFootRebarTemplate);
	}
}

void OldMemberRebarTemplate::InfoSetCheckBoxChecked()
{
	if (m_nMemberType == RT_OldColumn)
	{
		m_ColumnRebarTemplate->InfoSetCheckBoxChecked();
	}
	else if (m_nMemberType == RT_OldBeam)
	{
		m_BeamRebarTemplate->InfoSetCheckBoxChecked();
	}
	else if (m_nMemberType == RT_OldBWall)
	{
		m_BWallRebarTemplate->InfoSetCheckBoxChecked();
	}
	else if (m_nMemberType == RT_OldRWall)
	{
		m_RWallRebarTemplate->InfoSetCheckBoxChecked();
	}
	else if (m_nMemberType == RT_OldPit)
	{
		m_PitRebarTemplate->InfoSetCheckBoxChecked();
	}
	else if (m_nMemberType == RT_OldSlab)
	{
		m_SlabRebarTemplate->InfoSetCheckBoxChecked();
	}
	else if (m_nMemberType == RT_OldIsoFoot)
	{
		m_IsoFootRebarTemplate->InfoSetCheckBoxChecked();
	}
	else //if (m_nMemberType == RT_OldWallFoot)
	{
		m_WallFootRebarTemplate->InfoSetCheckBoxChecked();
	}
}
