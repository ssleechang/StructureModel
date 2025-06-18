#include "StdAfx.h"
#include "MSSteelBraceM.h"

#include "MSRebarBuilding.h"
#include "MSSteelUtility.h"
#include "MSSteelSection.h"

IMPLEMENT_SERIAL(MSSteelBraceM, MSLineMemberM, VERSIONABLE_SCHEMA | 1)
MSSteelBraceM::MSSteelBraceM(void)
{
	m_nType = msBraceM;
	m_pSection = NULL;
	m_nSecType = SECTION_H_TYPE;
	m_bDoubleSection = FALSE;
}

MSSteelBraceM::~MSSteelBraceM(void)
{
}

void MSSteelBraceM::CopyFromMe(MSCompoM* pSource)
{
	MSSteelBraceM* pSourceBraceM = (MSSteelBraceM*)pSource;
	m_pSection = pSourceBraceM->m_pSection;
	m_nSecType = pSourceBraceM->m_nSecType;
	m_bDoubleSection = pSourceBraceM->m_bDoubleSection;

	MSLineMemberM::CopyFromMe(pSource);
}

bool MSSteelBraceM::IsGeometryDifferent(MSCompoM* compoM)
{
	if (MSLineMemberM::IsGeometryDifferent(compoM)) return true;

	MSSteelBraceM* srcM = dynamic_cast<MSSteelBraceM*>(compoM);
	if (srcM == NULL) return true;

	if (GetSteelSection()->IsGeometryDifferent(srcM->GetSteelSection())) return true;

	return false;
}

void MSSteelBraceM::Serialize(CArchive& ar)
{
	MSLineMemberM::Serialize(ar);
	long nSectionID = 0;
	if(ar.IsStoring())
	{
		if(m_pSection)	nSectionID = m_pSection->m_ID;
		ar << nSectionID;
		ar << m_nSecType;
		ar << m_bDoubleSection;
	}
	else
	{
		ar >> nSectionID;
		ar >> m_nSecType;
		ar >> m_bDoubleSection;

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