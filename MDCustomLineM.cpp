#include "stdafx.h"
#include "MDCustomLineM.h"

#include "MDCustomSection.h"
#include "MDCustomPointUnitRebar.h"
#include "MDCustomLineUnitRebar.h"
#include "MSCustomPointBarUnit.h"
#include "MSCustomLineBarUnit.h"
#include "MSCustomLineSection.h"
#include "MSCustomPointBarUnit.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////     MDCustomColumnM //////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(MDCustomLineM, MDCustomM, VERSIONABLE_SCHEMA | 1)
MDCustomLineM::MDCustomLineM()
{
	m_pMDCustomSection = new MDCustomSection();
}


MDCustomLineM::~MDCustomLineM()
{
	delete m_pMDCustomSection;
}

void MDCustomLineM::Serialize(CArchive& ar)
{
	MDCustomM::Serialize(ar);
	m_pMDCustomSection->Serialize(ar);
}

void MDCustomLineM::CopyFromMe(MDMember* pSource)
{
	if (pSource == NULL)	return;
	MDCustomM::CopyFromMe(pSource);

	MDCustomLineM* pSourceMDCustomLineM = (MDCustomLineM*)pSource;
	m_pMDCustomSection->CopyFromMe(pSourceMDCustomLineM->GetMDCustomSection());
}

void MDCustomLineM::Dump(EFS::EFS_Buffer& buffer)
{
	MDCustomM::Dump(buffer);
	m_pMDCustomSection->Dump(buffer);
}

void MDCustomLineM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDCustomM::Recovery(buffer);
	m_pMDCustomSection->Recovery(buffer);
}

MDCustomSection* MDCustomLineM::GetMDCustomSection(eCustomM_SectionType eType)
{
	if (eType == Column || eType == WallColumn)
		return m_pMDCustomSection;

	return nullptr;
}
