#include "StdAfx.h"
#include "OldCommonRebarTemplate.h"

OldCommonRebarTemplate::OldCommonRebarTemplate()
{
	DataInit();
}

OldCommonRebarTemplate::~OldCommonRebarTemplate(void)
{
	if(m_ColumnRebarTemplate)	delete m_ColumnRebarTemplate;
	if(m_BeamRebarTemplate)		delete m_BeamRebarTemplate;
	if(m_BWallRebarTemplate)		delete m_BWallRebarTemplate;
	if(m_RWallRebarTemplate)		delete m_RWallRebarTemplate;
	if(m_PitRebarTemplate)			delete m_PitRebarTemplate;
	if(m_SlabRebarTemplate)		delete m_SlabRebarTemplate;
	if(m_IsoFootRebarTemplate)		delete m_IsoFootRebarTemplate;
	if(m_WallFootRebarTemplate)		delete m_WallFootRebarTemplate;
}

void OldCommonRebarTemplate::DataInit()
{
	m_ColumnRebarTemplate = NULL;
	m_BeamRebarTemplate = NULL;
	m_BWallRebarTemplate = NULL;
	m_RWallRebarTemplate = NULL;
	m_PitRebarTemplate = NULL;
	m_SlabRebarTemplate = NULL;
	m_IsoFootRebarTemplate = NULL;
	m_WallFootRebarTemplate = NULL;
}

