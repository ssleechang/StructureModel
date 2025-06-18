#include "StdAfx.h"
#include "MSSrcBraceM.h"

#include "StructureModelEnum.h"
#include "MSRebarBuilding.h"
#include "MSSteelUtility.h"
#include "MSSteelSection.h"

IMPLEMENT_SERIAL(MSSrcBraceM, MSLineMemberM, VERSIONABLE_SCHEMA | 1)
MSSrcBraceM::MSSrcBraceM(void)
{
	m_nType = msBraceM;
	m_pSection = NULL;
	m_nSecType =SECTION_H_TYPE;
}

MSSrcBraceM::~MSSrcBraceM(void)
{
}

void MSSrcBraceM::CopyFromMe(MSCompoM* pSource)
{
	MSSrcBraceM* pSourceBraceM = (MSSrcBraceM*)pSource;
	m_pSection = pSourceBraceM->m_pSection;
	m_nSecType = pSourceBraceM->m_nSecType;

	MSLineMemberM::CopyFromMe(pSource);
}

bool MSSrcBraceM::IsGeometryDifferent(MSCompoM* compoM)
{
	if (MSLineMemberM::IsGeometryDifferent(compoM)) return true;

	MSSrcBraceM* srcM = dynamic_cast<MSSrcBraceM*>(compoM);
	if (srcM == NULL) return true;

	if (GetSteelSection()->IsGeometryDifferent(srcM->GetSteelSection())) return true;

	return false;
}

void MSSrcBraceM::Serialize(CArchive& ar)
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