#include "StdAfx.h"
#include "MemberRebarTemplate.h"
#include "RebarTemplateManager.h"
#include "MSObject.h"

IMPLEMENT_SERIAL(MemberRebarTemplate, CommonRebarTemplate, VERSIONABLE_SCHEMA | 1)
MemberRebarTemplate::MemberRebarTemplate() : CommonRebarTemplate()
{
	DataInit();
}

MemberRebarTemplate::~MemberRebarTemplate(void)
{
}

void MemberRebarTemplate::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;

	MemberRebarTemplate* Source = (MemberRebarTemplate*)pSource_in;

	m_sName = Source->m_sName;
	m_nMemberType = Source->m_nMemberType;
	m_nTopOptionID = Source->m_nTopOptionID;

	if (m_nMemberType == RT_Column)
		m_ColumnRebarTemplate->CopyFromMe(&*Source->m_ColumnRebarTemplate);
	else if (m_nMemberType == RT_Beam)
		m_BeamRebarTemplate->CopyFromMe(&*Source->m_BeamRebarTemplate);
	else if (m_nMemberType == RT_BWall)
		m_BWallRebarTemplate->CopyFromMe(&*Source->m_BWallRebarTemplate);
	else if (m_nMemberType == RT_RWall)
		m_RWallRebarTemplate->CopyFromMe(&*Source->m_RWallRebarTemplate);
	else if (m_nMemberType == RT_Pit)
		m_PitRebarTemplate->CopyFromMe(&*Source->m_PitRebarTemplate);
	else if (m_nMemberType == RT_Slab)
		m_SlabRebarTemplate->CopyFromMe(&*Source->m_SlabRebarTemplate);
	else if (m_nMemberType == RT_IsoFoot)
		m_IsoFootRebarTemplate->CopyFromMe(&*Source->m_IsoFootRebarTemplate);
	else //if (m_nMemberType == RT_WallFoot)
		m_WallFootRebarTemplate->CopyFromMe(&*Source->m_WallFootRebarTemplate);
}

void MemberRebarTemplate::Dump(EFS::EFS_Buffer& buffer)
{
	CommonRebarTemplate::Dump(buffer);
	buffer << m_sName;
	buffer << m_nMemberType;
	buffer << m_nTopOptionID;

	if (m_nMemberType == RT_Column)
		m_ColumnRebarTemplate->Dump(buffer);
	else if (m_nMemberType == RT_Beam)
		m_BeamRebarTemplate->Dump(buffer);
	else if (m_nMemberType == RT_BWall)
		m_BWallRebarTemplate->Dump(buffer);
	else if (m_nMemberType == RT_RWall)
		m_RWallRebarTemplate->Dump(buffer);
	else if (m_nMemberType == RT_Pit)
		m_PitRebarTemplate->Dump(buffer);
	else if (m_nMemberType == RT_Slab)
		m_SlabRebarTemplate->Dump(buffer);
	else if (m_nMemberType == RT_IsoFoot)
		m_IsoFootRebarTemplate->Dump(buffer);
	else //if (m_nMemberType == RT_WallFoot)
		m_WallFootRebarTemplate->Dump(buffer);
}

void MemberRebarTemplate::Recovery(EFS::EFS_Buffer& buffer)
{
	CommonRebarTemplate::Recovery(buffer);
	buffer >> m_sName;
	buffer >> m_nMemberType;
	buffer >> m_nTopOptionID;

	if (m_nMemberType == RT_Column)
		m_ColumnRebarTemplate->Recovery(buffer);
	else if (m_nMemberType == RT_Beam)
		m_BeamRebarTemplate->Recovery(buffer);
	else if (m_nMemberType == RT_BWall)
		m_BWallRebarTemplate->Recovery(buffer);
	else if (m_nMemberType == RT_RWall)
		m_RWallRebarTemplate->Recovery(buffer);
	else if (m_nMemberType == RT_Pit)
		m_PitRebarTemplate->Recovery(buffer);
	else if (m_nMemberType == RT_Slab)
		m_SlabRebarTemplate->Recovery(buffer);
	else if (m_nMemberType == RT_IsoFoot)
		m_IsoFootRebarTemplate->Recovery(buffer);
	else //if (m_nMemberType == RT_WallFoot)
		m_WallFootRebarTemplate->Recovery(buffer);
}

void MemberRebarTemplate::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_sName;
		ar << m_nMemberType;
		ar << m_nID;
		ar << m_nTopOptionID;

		if (m_nMemberType == RT_Column)
			m_ColumnRebarTemplate->Serialize(ar);
		else if (m_nMemberType == RT_Beam)
			m_BeamRebarTemplate->Serialize(ar);
		else if (m_nMemberType == RT_BWall)
			m_BWallRebarTemplate->Serialize(ar);
		else if (m_nMemberType == RT_RWall)
			m_RWallRebarTemplate->Serialize(ar);
		else if (m_nMemberType == RT_Pit)
			m_PitRebarTemplate->Serialize(ar);
		else if (m_nMemberType == RT_Slab)
			m_SlabRebarTemplate->Serialize(ar);
		else if (m_nMemberType == RT_IsoFoot)
			m_IsoFootRebarTemplate->Serialize(ar);
		else //if (m_nMemberType == RT_WallFoot)
			m_WallFootRebarTemplate->Serialize(ar);
	}
	else
	{
		ar >> m_sName;
		ar >> m_nMemberType;
		ar >> m_nID;
		ar >> m_nTopOptionID;

		if (m_nMemberType == RT_Column)
		{
			m_ColumnRebarTemplate = (ColumnRebarTemplate*)RebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_Column);
			m_ColumnRebarTemplate->Serialize(ar);
		}
		else if (m_nMemberType == RT_Beam)
		{
			m_BeamRebarTemplate = (BeamRebarTemplate*)RebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_Beam);
			m_BeamRebarTemplate->Serialize(ar);
		}
		else if (m_nMemberType == RT_BWall)
		{
			m_BWallRebarTemplate = (BWallRebarTemplate*)RebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_BWall);
			m_BWallRebarTemplate->Serialize(ar);
		}
		else if (m_nMemberType == RT_RWall)
		{
			m_RWallRebarTemplate = (RWallRebarTemplate*)RebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_RWall);
			m_RWallRebarTemplate->Serialize(ar);
		}
		else if (m_nMemberType == RT_Pit)
		{
			m_PitRebarTemplate = (PitRebarTemplate*)RebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_Pit);
			m_PitRebarTemplate->Serialize(ar);
		}
		else if (m_nMemberType == RT_Slab)
		{
			m_SlabRebarTemplate = (SlabRebarTemplate*)RebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_Slab);
			m_SlabRebarTemplate->Serialize(ar);
		}
		else if (m_nMemberType == RT_IsoFoot)
		{
			m_IsoFootRebarTemplate = (IsoFootRebarTemplate*)RebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_IsoFoot);
			m_IsoFootRebarTemplate->Serialize(ar);
		}
		else //if (m_nMemberType == RT_WallFoot)
		{
			m_WallFootRebarTemplate = (WallFootRebarTemplate*)RebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_WallFoot);
			m_WallFootRebarTemplate->Serialize(ar);
		}

		RebarTemplateManager::SetLastID(m_nID);
	}
}

void MemberRebarTemplate::DataInit()
{
	m_sName = _T("");
	m_nMemberType = -1;
	m_nID = -1;
	m_nTopOptionID = -1;
}

void MemberRebarTemplate::TopOptionDataCopy(CommonRebarTemplate* commonRebarTemplate)
{
	if (m_nMemberType == RT_Column)
	{
		m_ColumnRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_ColumnRebarTemplate);
	}
	else if (m_nMemberType == RT_Beam)
	{
		m_BeamRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_BeamRebarTemplate);
	}
	else if (m_nMemberType == RT_BWall)
	{
		m_BWallRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_BWallRebarTemplate);
	}
	else if (m_nMemberType == RT_RWall)
	{
		m_RWallRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_RWallRebarTemplate);
	}
	else if (m_nMemberType == RT_Pit)
	{
		m_PitRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_PitRebarTemplate);
	}
	else if (m_nMemberType == RT_Slab)
	{
		m_SlabRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_SlabRebarTemplate);
	}
	else if (m_nMemberType == RT_IsoFoot)
	{
		m_IsoFootRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_IsoFootRebarTemplate);
	}
	else //if (m_nMemberType == RT_WallFoot)
	{
		m_WallFootRebarTemplate->TopOptionDataCopy(commonRebarTemplate->m_WallFootRebarTemplate);
	}
}

void MemberRebarTemplate::CreateMemberAndDataCopy(CommonRebarTemplate* commonRebarTemplate)
{
	if (m_nMemberType == RT_Column)
	{
		m_ColumnRebarTemplate = (ColumnRebarTemplate*)RebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_Column);
		m_ColumnRebarTemplate->DataCopy(commonRebarTemplate->m_ColumnRebarTemplate);
	}
	else if (m_nMemberType == RT_Beam)
	{
		m_BeamRebarTemplate = (BeamRebarTemplate*)RebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_Beam);
		m_BeamRebarTemplate->DataCopy(commonRebarTemplate->m_BeamRebarTemplate);
	}
	else if (m_nMemberType == RT_BWall)
	{
		m_BWallRebarTemplate =(BWallRebarTemplate*)RebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_BWall);
		m_BWallRebarTemplate->DataCopy(commonRebarTemplate->m_BWallRebarTemplate);
	}
	else if (m_nMemberType == RT_RWall)
	{
		m_RWallRebarTemplate = (RWallRebarTemplate*)RebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_RWall);
		m_RWallRebarTemplate->DataCopy(commonRebarTemplate->m_RWallRebarTemplate);
	}
	else if (m_nMemberType == RT_Pit)
	{
		m_PitRebarTemplate = (PitRebarTemplate*)RebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_Pit);
		m_PitRebarTemplate->DataCopy(commonRebarTemplate->m_PitRebarTemplate);
	}
	else if (m_nMemberType == RT_Slab)
	{
		m_SlabRebarTemplate = (SlabRebarTemplate*)RebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_Slab);
		m_SlabRebarTemplate->DataCopy(commonRebarTemplate->m_SlabRebarTemplate);
	}
	else if (m_nMemberType == RT_IsoFoot)
	{
		m_IsoFootRebarTemplate = (IsoFootRebarTemplate*)RebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_IsoFoot);
		m_IsoFootRebarTemplate->DataCopy(commonRebarTemplate->m_IsoFootRebarTemplate);
	}
	else //if (m_nMemberType == RT_WallFoot)
	{
		m_WallFootRebarTemplate = (WallFootRebarTemplate*)RebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_WallFoot);
		m_WallFootRebarTemplate->DataCopy(commonRebarTemplate->m_WallFootRebarTemplate);
	}
}

void MemberRebarTemplate::InfoSetCheckBoxChecked()
{
	if (m_nMemberType == RT_Column)
	{
		m_ColumnRebarTemplate->InfoSetCheckBoxChecked();
	}
	else if (m_nMemberType == RT_Beam)
	{
		m_BeamRebarTemplate->InfoSetCheckBoxChecked();
	}
	else if (m_nMemberType == RT_BWall)
	{
		m_BWallRebarTemplate->InfoSetCheckBoxChecked();
	}
	else if (m_nMemberType == RT_RWall)
	{
		m_RWallRebarTemplate->InfoSetCheckBoxChecked();
	}
	else if (m_nMemberType == RT_Pit)
	{
		m_PitRebarTemplate->InfoSetCheckBoxChecked();
	}
	else if (m_nMemberType == RT_Slab)
	{
		m_SlabRebarTemplate->InfoSetCheckBoxChecked();
	}
	else if (m_nMemberType == RT_IsoFoot)
	{
		m_IsoFootRebarTemplate->InfoSetCheckBoxChecked();
	}
	else //if (m_nMemberType == RT_WallFoot)
	{
		m_WallFootRebarTemplate->InfoSetCheckBoxChecked();
	}
}
