#include "StdAfx.h"
#include "MSOpeningM.h"
#include "GMLib\MSVersionInfo.h"

IMPLEMENT_SERIAL(MSOpeningM, MSCompoM, VERSIONABLE_SCHEMA | 1)

MSOpeningM::MSOpeningM(void)
{
	m_dWidth = 300;
	m_dHeight = 1000;
	m_nOpeningReinforceMID = 0;
	m_ExtraSize = 0;
}

MSOpeningM::~MSOpeningM(void)
{
}

void MSOpeningM::CopyFromMe(MSCompoM* pSource)
{
	if(pSource==NULL)	return;
	MSOpeningM* pSourceOpenWindowM = (MSOpeningM*)pSource;

	m_dWidth = pSourceOpenWindowM->m_dWidth;
	m_dHeight = pSourceOpenWindowM->m_dHeight;

	m_nOpeningReinforceMID = pSourceOpenWindowM->m_nOpeningReinforceMID;
	m_ExtraSize = pSourceOpenWindowM->m_ExtraSize;
	MSCompoM::CopyFromMe(pSource);
}

void MSOpeningM::Dump( EFS::EFS_Buffer& buffer )
{
	MSCompoM::Dump(buffer);
	buffer << m_dWidth << m_dHeight;
	buffer << m_nOpeningReinforceMID << m_ExtraSize;
}

void MSOpeningM::Recovery( EFS::EFS_Buffer& buffer )
{
	MSCompoM::Recovery(buffer);
	buffer >> m_dWidth >> m_dHeight;
	buffer >> m_nOpeningReinforceMID >> m_ExtraSize;
}

void MSOpeningM::Serialize(CArchive &ar)
{
	MSCompoM::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_dWidth;
		ar << m_dHeight;

		ar << m_nOpeningReinforceMID;
		ar << m_ExtraSize;
	}
	else
	{
		ar >> m_dWidth;
		ar >> m_dHeight;

		if(MSVersionInfo::GetCurrentVersion() >= 20150403)
		{
			ar >> m_nOpeningReinforceMID;
			ar >> m_ExtraSize;
		}
	}
}

