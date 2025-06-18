#include "StdAfx.h"
#include "MSSteelColumnM.h"

#include "MSRebarBuilding.h"
#include "MSSteelUtility.h"
#include "MSSteelSection.h"

#include "GMLib/GM2DRectangle.h"

IMPLEMENT_SERIAL(MSSteelColumnM, MSLineMemberM, VERSIONABLE_SCHEMA | 1)
MSSteelColumnM::MSSteelColumnM(void)
{
	m_nType = msSteelColumnM;
	m_pSection = NULL;
	m_nSecType = SECTION_H_TYPE;
}

MSSteelColumnM::~MSSteelColumnM(void)
{
}

void MSSteelColumnM::CopyFromMe(MSCompoM* pSource)
{
	MSSteelColumnM* pSourceColumnM = (MSSteelColumnM*)pSource;
	m_pSection = pSourceColumnM->m_pSection;
	m_nSecType = pSourceColumnM->m_nSecType;

	MSLineMemberM::CopyFromMe(pSource);
}

void MSSteelColumnM::Dump(EFS::EFS_Buffer& buffer)
{
	MSLineMemberM::Dump(buffer);
	buffer << &m_pSection;
	buffer << m_nSecType;
}

void MSSteelColumnM::Recovery(EFS::EFS_Buffer& buffer)
{
	MSLineMemberM::Recovery(buffer);

	buffer >> &m_pSection;
	buffer >> m_nSecType;
}

void MSSteelColumnM::Serialize(CArchive& ar )
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

double MSSteelColumnM::GetWidth()
{
	return m_pSection->GetB();
}

double MSSteelColumnM::GetDepth()
{
	return m_pSection->GetH();
}

double MSSteelColumnM::GetThick()
{
	return m_pSection->GetWThk();
}


void MSSteelColumnM::GetSecPolyline(GM2DPolyline& TheGon)
{

	m_pSection->GetSecPolyline(TheGon);

	
}

bool MSSteelColumnM::IsGeometryDifferent(MSCompoM* compoM)
{
	if (MSLineMemberM::IsGeometryDifferent(compoM)) return true;

	MSSteelColumnM* srcM = dynamic_cast<MSSteelColumnM*>(compoM);
	if (srcM == NULL) return true;

	if (GetSteelSection()->IsGeometryDifferent(srcM->GetSteelSection())) return true;

	return false;
}
