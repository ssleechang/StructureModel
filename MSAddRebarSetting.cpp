#include "stdafx.h"
#include "MSAddRebarSetting.h"


IMPLEMENT_SERIAL(MSAddRebarSetting, MSObject, VERSIONABLE_SCHEMA | 1)
MSAddRebarSetting::MSAddRebarSetting()
{
	m_eDevType  = eDevType::Straight;
	m_eSpacingType = eSpacingType::UserSpacing;
	m_UserSpacingLength = 0;
	m_eSectionType = eSectionType::Section_User;
	m_UserSectionLength = 0;
}


MSAddRebarSetting::~MSAddRebarSetting()
{
}

void MSAddRebarSetting::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;
	MSObject::CopyFromMe(pSource_in, bNewID);

	MSAddRebarSetting* pSource = dynamic_cast<MSAddRebarSetting*>(pSource_in);
	if (pSource == nullptr)	return;

	m_eDevType = pSource->GetDevType();
	m_eSpacingType = pSource->GetSpacingType();
	m_UserSpacingLength = pSource->GetUserSpacingLength();
	m_eSectionType = pSource->GetSectionType();
	m_UserSectionLength = pSource->GetUserSectionLength();
}

void MSAddRebarSetting::Dump(EFS::EFS_Buffer& ar)
{
	MSObject::Dump(ar);
	int devType = (int)m_eDevType;
	int spacingType = (int)m_eSpacingType;
	int sectionType = (int)m_eSectionType;

	ar << devType << spacingType << sectionType;
	ar << m_UserSpacingLength << m_UserSectionLength;
}

void MSAddRebarSetting::Recovery(EFS::EFS_Buffer& ar)
{
	MSObject::Recovery(ar);
	int devType = 0, spacingType = 0, sectionType = 0;
	ar >> devType >> spacingType >> sectionType;
	ar >> m_UserSpacingLength >> m_UserSectionLength;

	m_eDevType = (eDevType)devType;
	m_eSpacingType = (eSpacingType)spacingType;
	m_eSectionType = (eSectionType)sectionType;
}

void MSAddRebarSetting::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	int devType = 0, spacingType = 0, sectionType = 0;
	if (ar.IsStoring())
	{
		devType = (int)m_eDevType;
		spacingType = (int)m_eSpacingType;
		sectionType = (int)m_eSectionType;

		ar << devType;
		ar << spacingType;
		ar << sectionType;
		ar << m_UserSpacingLength;
		ar << m_UserSectionLength;
	}
	else
	{
		ar >> devType;
		ar >> spacingType;
		ar >> sectionType;
		ar >> m_UserSpacingLength;
		ar >> m_UserSectionLength;

		m_eDevType = (eDevType)devType;
		m_eSpacingType = (eSpacingType)spacingType;
		m_eSectionType = (eSectionType)sectionType;

	}
}
