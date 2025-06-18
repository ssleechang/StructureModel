#include "StdAfx.h"
#include "MSSteelBeamM.h"

#include "MSRebarBuilding.h"
#include "MSSteelUtility.h"
#include "MSSteelSection.h"

IMPLEMENT_SERIAL(MSSteelBeamM, MSLineMemberM, VERSIONABLE_SCHEMA | 1)
MSSteelBeamM::MSSteelBeamM(void)
{
	m_nType = msSteelBeamM;
	m_pSection = NULL;
	m_nSecType = SECTION_H_TYPE;
}

MSSteelBeamM::~MSSteelBeamM(void)
{
}

void MSSteelBeamM::CopyFromMe(MSCompoM* pSource)
{
	MSSteelBeamM* pSourceBeamM = (MSSteelBeamM*)pSource;
	m_pSection = pSourceBeamM->m_pSection;
	m_nSecType = pSourceBeamM->m_nSecType;

	MSLineMemberM::CopyFromMe(pSource);
}

void MSSteelBeamM::Dump(EFS::EFS_Buffer& buffer)
{
	MSLineMemberM::Dump(buffer);
	buffer << &m_pSection;
	buffer << m_nSecType;
}

void MSSteelBeamM::Recovery(EFS::EFS_Buffer& buffer)
{
	MSLineMemberM::Recovery(buffer);

	buffer >> &m_pSection;
	buffer >> m_nSecType;
}

void MSSteelBeamM::Serialize(CArchive& ar )
{
	MSLineMemberM::Serialize(ar);
	long nSectionID = 0;
	if(ar.IsStoring())
	{
		if(m_pSection)	nSectionID = m_pSection->m_ID;
		ar << nSectionID;
		ar << m_nSecType;
	}
	else
	{
		ar >> nSectionID;
		ar >> m_nSecType;

		MSSteelUtility* pUtil = mp_Owner->GetSteelUtili();
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

double MSSteelBeamM::GetWidth()
{
	return m_pSection->GetB();
}

double MSSteelBeamM::GetDepth()
{
	return m_pSection->GetH();
}

double MSSteelBeamM::GetThick()
{
	return m_pSection->GetWThk();
}

void MSSteelBeamM::GetSecPolyline(GM2DPolyline& TheGon)
{
	m_pSection->GetSecPolyline(TheGon);
}

bool MSSteelBeamM::IsGeometryDifferent(MSCompoM* compoM)
{
	if (MSLineMemberM::IsGeometryDifferent(compoM)) return true;

	MSSteelBeamM* srcM = dynamic_cast<MSSteelBeamM*>(compoM);
	if (srcM == NULL) return true;

	if (GetSteelSection()->IsGeometryDifferent(srcM->GetSteelSection())) return true;

	return false;
}
