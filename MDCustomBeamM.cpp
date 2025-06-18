#include "stdafx.h"
#include "MDCustomBeamM.h"
#include "MDCustomSection.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////     MDCustomBeamM     //////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(MDCustomBeamM, MDCustomM, VERSIONABLE_SCHEMA | 1)
MDCustomBeamM::MDCustomBeamM()
{
	mp_SmallSec = nullptr;
	mp_MidSec = nullptr;
	mp_LargeSec = nullptr;
	CreateSection();
}


MDCustomBeamM::~MDCustomBeamM()
{
	DeleteSection();
}

void MDCustomBeamM::Serialize(CArchive& ar)
{
	MDCustomM::Serialize(ar);
	
	mp_SmallSec->Serialize(ar);
	mp_MidSec->Serialize(ar);
	mp_LargeSec->Serialize(ar);
}

void MDCustomBeamM::CopyFromMe(MDMember* pSource)
{
	if (pSource == NULL)	return;
	MDCustomM::CopyFromMe(pSource);

	MDCustomBeamM* pSourceM = (MDCustomBeamM*)pSource;
	mp_SmallSec->CopyFromMe(pSourceM->GetMDCustomSection(Beam_Small));
	mp_MidSec->CopyFromMe(pSourceM->GetMDCustomSection(Beam_Mid));
	mp_LargeSec->CopyFromMe(pSourceM->GetMDCustomSection(Beam_Large));
}

void MDCustomBeamM::Dump(EFS::EFS_Buffer& buffer)
{
	MDCustomM::Dump(buffer);
	mp_SmallSec->Dump(buffer);
	mp_MidSec->Dump(buffer);
	mp_LargeSec->Dump(buffer);
}

void MDCustomBeamM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDCustomM::Recovery(buffer);
	mp_SmallSec->Recovery(buffer);
	mp_MidSec->Recovery(buffer);
	mp_LargeSec->Recovery(buffer);
}

MDCustomSection* MDCustomBeamM::GetMDCustomSection(eCustomM_SectionType eType)
{
	if (eType == Beam_Small)
		return mp_SmallSec;
	else if (eType == Beam_Mid)
		return mp_MidSec;
	else if (eType == Beam_Large)
		return mp_LargeSec;

	return nullptr;
}

void MDCustomBeamM::CreateSection()
{
	DeleteSection();
	mp_SmallSec = new MDCustomSection();
	mp_MidSec = new MDCustomSection();
	mp_LargeSec = new MDCustomSection();
}

void MDCustomBeamM::DeleteSection()
{
	delete mp_SmallSec;
	delete mp_MidSec;
	delete mp_LargeSec;
	mp_SmallSec = mp_MidSec = mp_LargeSec = nullptr;
}