#include "StdAfx.h"
#include "MSSrcBeamM.h"
#include "MSRebarBuilding.h"
#include "MSSteelUtility.h"
#include "MSSteelSection.h"

IMPLEMENT_SERIAL(MSSrcBeamM, MSRcLineMemberM, VERSIONABLE_SCHEMA | 1)
MSSrcBeamM::MSSrcBeamM(void)
{
	m_nType = msSrcBeamM;

	m_pSection = NULL;
	m_nSecType = SECTION_H_TYPE;
	m_NumStudBoltBars = 4;
	m_StudBoltBars.InitData(MSDeformedBar::GetStirrupBarStr(300, 19));	
	m_StudBoltLength = 120;
	m_h = 200;
}

MSSrcBeamM::~MSSrcBeamM(void)
{
}

void MSSrcBeamM::CopyFromMe(MSCompoM* pSource)
{
	MSSrcBeamM* pSourceBeamM = (MSSrcBeamM*)pSource;


	m_pSection = pSourceBeamM->m_pSection;
	m_nSecType = pSourceBeamM->m_nSecType;
	m_NumStudBoltBars = pSourceBeamM->m_NumStudBoltBars;
	m_StudBoltBars.CopyFromMe(&pSourceBeamM->m_StudBoltBars);
	m_StudBoltLength = pSourceBeamM->m_StudBoltLength;
	m_h = pSourceBeamM->m_h;
	MSRcLineMemberM::CopyFromMe(pSource);
}


void MSSrcBeamM::Dump(EFS::EFS_Buffer& buffer)
{
	MSRcLineMemberM::Dump(buffer);
	buffer << &m_pSection;
	buffer << m_nSecType;

	buffer << m_NumStudBoltBars;
	m_StudBoltBars.Dump(buffer);
	buffer << m_StudBoltLength;
	buffer << m_h;
}

void MSSrcBeamM::Recovery(EFS::EFS_Buffer& buffer)
{
	MSRcLineMemberM::Recovery(buffer);

	buffer >> &m_pSection;
	buffer >> m_nSecType;

	buffer >> m_NumStudBoltBars;
	m_StudBoltBars.Recovery(buffer);
	buffer >> m_StudBoltLength;
	buffer >> m_h;
}

void MSSrcBeamM::Serialize(CArchive& ar )
{
	MSRcLineMemberM::Serialize(ar);
	long nSectionID = 0;
	if(ar.IsStoring())
	{

		if(m_pSection)	nSectionID = m_pSection->m_ID;
		ar << nSectionID;
		ar << m_nSecType;

		ar << m_NumStudBoltBars;
		m_StudBoltBars.Serialize(ar);
		ar << m_StudBoltLength;
		ar << m_h;
	}
	else
	{

		ar >> nSectionID;
		ar >> m_nSecType;

		ar >> m_NumStudBoltBars;
		m_StudBoltBars.Serialize(ar);
		ar >> m_StudBoltLength;
		ar >> m_h;

		MSSteelUtility* pUtil = mp_Owner->GetSteelUtili();
		if (pUtil == nullptr)	return;
		m_pSection = pUtil->GetSection(m_nSecType, nSectionID);
		if(m_pSection == NULL)
		{
			m_pSection = pUtil->GetDefaultHSection();
			CString Msg;	Msg.Format(_T("%s : %s"), ERROR_STEEL_SECTION_FILE, m_Name);
			StatusReporterWrapper::OutputWarning(Msg);

			MSSteelUtility::CreateErrorObject(MESSAGE_WARNING_CHANGED_DEFAULT_STEEL_SECTION, WARNING_LEVEL, Msg, XErrorObject::MSCompoMIDSelect);
		}
	}
}

bool MSSrcBeamM::IsGeometryDifferent(MSCompoM* compoM)
{
	if (MSRcLineMemberM::IsGeometryDifferent(compoM)) return true;

	MSSrcBeamM* srcM = dynamic_cast<MSSrcBeamM*>(compoM);
	if (srcM == NULL) return true;

	if (GetSteelSection()->IsGeometryDifferent(srcM->GetSteelSection())) return true;

	if ( fabs(m_h - srcM->m_h) > DTOL_GM) return true;

	return false;
}

double MSSrcBeamM::GetWidth()
{
	double width = MSRcLineMemberM::GetWidth();
	if (m_pSection != NULL)
	{
		if (m_pSection->GetB() > width) width = m_pSection->GetB();
	}
	return width;
}