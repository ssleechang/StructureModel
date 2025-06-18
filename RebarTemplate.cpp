#include "StdAfx.h"
#include "RebarTemplate.h"
#include "RebarTemplateManager.h"
#include "..\GMLib\MSVersionInfo.h"

RebarTemplate::RebarTemplate()
{
	DataInit();
}

RebarTemplate::~RebarTemplate(void)
{
	if (m_ColumnRebarTemplate)	delete m_ColumnRebarTemplate;
	if (m_BWallRebarTemplate)	delete m_BWallRebarTemplate;
	if (m_RWallRebarTemplate)	delete m_RWallRebarTemplate;
	if (m_LowWallRebarTemplate)	delete m_LowWallRebarTemplate;
	if (m_BeamRebarTemplate)	delete m_BeamRebarTemplate;
	if (m_SlabRebarTemplate)	delete m_SlabRebarTemplate;
	if (m_PitRebarTemplate)	delete m_PitRebarTemplate;
	if (m_IsoFootRebarTemplate)	delete m_IsoFootRebarTemplate;
	if (m_WallFootRebarTemplate)	delete m_WallFootRebarTemplate;
	if (m_TransferColumnRebarTemplate) delete m_TransferColumnRebarTemplate;
	if (m_TransferBeamRebarTemplate) delete m_TransferBeamRebarTemplate;
	if (m_TransferSlabRebarTemplate) delete m_TransferSlabRebarTemplate;
	if (m_DeckBeamRebarTemplate) delete m_DeckBeamRebarTemplate;
}

void RebarTemplate::CopyFromMe(RebarTemplate* pSource_in)
{
	if (pSource_in == nullptr) return;

	RebarTemplate* Source = (RebarTemplate*)pSource_in;

	m_sName = Source->m_sName;

	m_ColumnRebarTemplate->CopyFromMe(&*Source->m_ColumnRebarTemplate);
	m_BWallRebarTemplate->CopyFromMe(&*Source->m_BWallRebarTemplate);
	m_RWallRebarTemplate->CopyFromMe(&*Source->m_RWallRebarTemplate);
	m_LowWallRebarTemplate->CopyFromMe(&*Source->m_LowWallRebarTemplate);
	m_BeamRebarTemplate->CopyFromMe(&*Source->m_BeamRebarTemplate);
	m_SlabRebarTemplate->CopyFromMe(&*Source->m_SlabRebarTemplate);
	m_PitRebarTemplate->CopyFromMe(&*Source->m_PitRebarTemplate);
	m_IsoFootRebarTemplate->CopyFromMe(&*Source->m_IsoFootRebarTemplate);
	m_WallFootRebarTemplate->CopyFromMe(&*Source->m_WallFootRebarTemplate);
	m_TransferColumnRebarTemplate->CopyFromMe(&*Source->m_TransferColumnRebarTemplate);
	m_TransferBeamRebarTemplate->CopyFromMe(&*Source->m_TransferBeamRebarTemplate);
	m_TransferSlabRebarTemplate->CopyFromMe(&*Source->m_TransferSlabRebarTemplate);
	m_DeckBeamRebarTemplate->CopyFromMe(&*Source->m_DeckBeamRebarTemplate);
}

void RebarTemplate::DataInit()
{
	m_sName = _T("");
	m_nID = -1;

	m_ColumnRebarTemplate = new ColumnRebarTemplate();
	m_BWallRebarTemplate = new BWallRebarTemplate();
	m_RWallRebarTemplate = new RWallRebarTemplate();
	m_LowWallRebarTemplate = new LowWallRebarTemplate();
	m_BeamRebarTemplate = new BeamRebarTemplate();
	m_SlabRebarTemplate = new SlabRebarTemplate();
	m_PitRebarTemplate = new PitRebarTemplate();
	m_IsoFootRebarTemplate = new IsoFootRebarTemplate();
	m_WallFootRebarTemplate = new WallFootRebarTemplate();
	m_TransferColumnRebarTemplate = new TransferColumnRebarTemplate();
	m_TransferBeamRebarTemplate = new TransferBeamRebarTemplate();
	m_TransferSlabRebarTemplate = new TransferSlabRebarTemplate();
	m_DeckBeamRebarTemplate = new DeckBeamRebarTemplate();
}

void RebarTemplate::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_sName;
		ar << m_nID;

		m_ColumnRebarTemplate->Serialize(ar);
		m_BeamRebarTemplate->Serialize(ar);
		m_BWallRebarTemplate->Serialize(ar);
		m_RWallRebarTemplate->Serialize(ar);
		m_PitRebarTemplate->Serialize(ar);
		m_SlabRebarTemplate->Serialize(ar);
		m_IsoFootRebarTemplate->Serialize(ar);
		m_WallFootRebarTemplate->Serialize(ar);
		m_LowWallRebarTemplate->Serialize(ar);

		m_TransferColumnRebarTemplate->Serialize(ar);
		m_TransferBeamRebarTemplate->Serialize(ar);
		m_TransferSlabRebarTemplate->Serialize(ar);

		m_DeckBeamRebarTemplate->Serialize(ar);
	}
	else
	{
		ar >> m_sName;
		ar >> m_nID;

		m_ColumnRebarTemplate->Serialize(ar);
		m_BeamRebarTemplate->Serialize(ar);
		m_BWallRebarTemplate->Serialize(ar);
		m_RWallRebarTemplate->Serialize(ar);
		m_PitRebarTemplate->Serialize(ar);
		m_SlabRebarTemplate->Serialize(ar);
		m_IsoFootRebarTemplate->Serialize(ar);
		m_WallFootRebarTemplate->Serialize(ar);

		if (MSVersionInfo::GetCurrentVersion() >= 20171019)
		{
			m_LowWallRebarTemplate->Serialize(ar);
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20240626)
		{
			m_TransferColumnRebarTemplate->Serialize(ar);
			m_TransferBeamRebarTemplate->Serialize(ar);
			m_TransferSlabRebarTemplate->Serialize(ar);
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20241018)
		{
			m_DeckBeamRebarTemplate->Serialize(ar);
		}

		RebarTemplateManager::SetLastID(m_nID);

	}
}
