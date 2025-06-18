#include "stdafx.h"
#include "MSCustomLineM.h"
#include "MSCustomLineSection.h"
#include "MDCustomLineM.h"
#include "MSCustomPointBarUnit.h"
#include "MSCustomLineBarUnit.h"
#include "MDCustomPointUnitRebar.h"
#include "MDCustomLineUnitRebar.h"
#include "MDCustomSection.h"

#include "GMLib\GM2DLineVector.h"

IMPLEMENT_SERIAL(MSCustomLineM, MSCustomM, VERSIONABLE_SCHEMA | 1)

MSCustomLineM::MSCustomLineM(void)
{
	
}

MSCustomLineM::~MSCustomLineM(void)
{
}

void MSCustomLineM::CopyFromMe(MSCompoM* pSource)
{
	if (pSource == NULL)	return;
	MSCustomLineM* pSourceM = (MSCustomLineM*)pSource;


	MSCustomM::CopyFromMe(pSource);
}

void MSCustomLineM::Dump(EFS::EFS_Buffer& buffer)
{
	MSCustomM::Dump(buffer);
}

void MSCustomLineM::Recovery(EFS::EFS_Buffer& buffer)
{
	MSCustomM::Recovery(buffer);
}

void MSCustomLineM::Serialize(CArchive &ar)
{
	MSCustomM::Serialize(ar);
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

void MSCustomLineM::MakeDefaultRebars()
{
	MSCustomLineSection* pSection = dynamic_cast<MSCustomLineSection*>(GetMSCustomSection());
	if (pSection == nullptr)	return;

	MDCustomSection* pMDCustomSection = GetMDCustomSection();
	if (pMDCustomSection == nullptr)
		return;

	pMDCustomSection->DeleteCustomMDPointUnitRebars();
	pMDCustomSection->DeleteCustomMDLineUnitRebars();

	vector<MSCustomPointBarUnit*> pointUnitList = pSection->GetMSPointUnits();
	for (int i = 0; i < pointUnitList.size(); i++)
	{
		pMDCustomSection->MakeDefaultMDPointUnitRebar(pointUnitList[i]);
	}

	vector<MSCustomLineBarUnit*> lineUnitList = pSection->GetMSLineUnits();
	for (int i = 0; i < lineUnitList.size(); i++)
	{
		pMDCustomSection->MakeDefaultMDLineUnitRebar(lineUnitList[i]);
	}
}

void MSCustomLineM::UpdateCustomRebarsID(map<long, long> oldUnitIDtoNewUnitID)
{
	MDCustomSection* pMDCustomSection = GetMDCustomSection();
	if (pMDCustomSection == nullptr)
		return;

	MSCustomLineSection* pSection = dynamic_cast<MSCustomLineSection*>(GetMSCustomSection());
	pMDCustomSection->UpdateCustomRebarsID(oldUnitIDtoNewUnitID);
}

void MSCustomLineM::UpdateCustomCompoM()
{
	MDCustomSection* pMDCustomSection = GetMDCustomSection();
	if (pMDCustomSection == nullptr)
		return;

	MSCustomLineSection* pSection = dynamic_cast<MSCustomLineSection*>(GetMSCustomSection());
	pMDCustomSection->UpdateCustomRebars(pSection);
}

MDCustomSection* MSCustomLineM::GetMDCustomSection()
{
	MDMember* pMDMember = GetRebarM();
	if (pMDMember == nullptr)
		return nullptr;
	MDCustomLineM* pMDCustomLineM = dynamic_cast<MDCustomLineM*>(pMDMember);
	if (pMDCustomLineM == nullptr)
		return nullptr;

	return pMDCustomLineM->GetMDCustomSection();
}