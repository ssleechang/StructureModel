#include "StdAfx.h"
#include "MSDoorM.h"

#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSDoorM, MSOpeningM, VERSIONABLE_SCHEMA | 1)

MSDoorM::MSDoorM(void)
{
	m_nType = msDoorM;
	m_IsOpenToWallBottom = false;
}

MSDoorM::~MSDoorM(void)
{
}

void MSDoorM::CopyFromMe(MSCompoM* pSource)
{
	if(pSource==NULL)	return;

	MSDoorM* pSourceDoorM = (MSDoorM*)pSource;
	m_IsOpenToWallBottom = pSourceDoorM->m_IsOpenToWallBottom;
	MSOpeningM::CopyFromMe(pSource);
}

bool MSDoorM::IsGeometryDifferent(MSCompoM* compoM)
{
	if (MSCompoM::IsGeometryDifferent(compoM))
		return true;

	MSDoorM* other = dynamic_cast<MSDoorM*>(compoM);
	if (other == NULL) return true;

	if (GetIsOpenToWallBottom() != other->GetIsOpenToWallBottom()) return true;

	return false;
}

void MSDoorM::Dump( EFS::EFS_Buffer& buffer )
{
	buffer << m_IsOpenToWallBottom;
	MSOpeningM::Dump(buffer);
}

void MSDoorM::Recovery( EFS::EFS_Buffer& buffer )
{
	buffer >> m_IsOpenToWallBottom;
	MSOpeningM::Recovery(buffer);
}

void MSDoorM::Serialize(CArchive &ar)
{
	MSOpeningM::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_IsOpenToWallBottom;
	}
	else
	{
		if(MSVersionInfo::GetCurrentVersion() >= 20150908)
			ar >> m_IsOpenToWallBottom;
	}
}

double MSDoorM::GetHeightBySlabLevel(double dSlabLevel)
{
	/*
	if (m_IsOpenToSlabLevel == false)
		return m_dHeight;

	return m_dHeight - dSlabLevel;
	*/
	return m_dHeight;
}