#include "StdAfx.h"
#include "MSDoorG.h"
#include "MSRebarBuilding.h"
#include "MSDoorM.h"
#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSDoorG, MSWallOpeningG, VERSIONABLE_SCHEMA | 1)

MSDoorG::MSDoorG(void)
{
}

MSDoorG::~MSDoorG(void)
{
}

void MSDoorG::Dump(EFS::EFS_Buffer& buffer)
{
	MSWallOpeningG::Dump(buffer);
}

void MSDoorG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSWallOpeningG::Recovery(buffer);
}
void MSDoorG::Serialize(CArchive &ar)
{
	MSWallOpeningG::Serialize(ar);
	bool bEachCompoM = false;
    if(ar.IsStoring())
    {
		if(mp_CompoM->m_ID == 0)
			bEachCompoM = true;

		ar << bEachCompoM;
		if(bEachCompoM == true)
		{
			MSDoorM* pDoorM = (MSDoorM*)mp_CompoM;
			long MType = pDoorM->m_nType;

			ar << MType;

			ar << pDoorM->GetMaterial()->m_ID;
			ar << pDoorM->m_Name;
			ar << pDoorM->GetWidth();
			ar << pDoorM->GetHeight();

			ar << pDoorM->GetOpeningReinforceMID();
			ar << pDoorM->GetExtraSize();
		}
	}
    else
    {
		if(MSVersionInfo::GetCurrentVersion() >= 20140228)
		{
			ar >> bEachCompoM;
			if(bEachCompoM)
			{
				MSBaseBuilding* pBldg = GetBuilding();
				long MType = 0;
				ar >> MType;

				MSDoorM* pDoorM = (MSDoorM*)pBldg->CreateNewCompoM(MType);
				pDoorM->m_nType = (MSCompoM::Type_MSCompoM)MType;

				long nMaterialID = 0;
				ar >> nMaterialID;
				pDoorM->SetMaterial(pBldg->GetMaterial(nMaterialID));
				ar >> pDoorM->m_Name;
				double dWidth = 0, dHeight = 0;
				ar >> dWidth;
				ar >> dHeight;
				pDoorM->SetWidth(dWidth);
				pDoorM->SetHeight(dHeight);
				pDoorM->mp_Owner = pBldg;

				long nReinforceMID = 0;
				long nExtraSize = 0;
				if(MSVersionInfo::GetCurrentVersion() >= 20150417)
				{
					ar >> nReinforceMID;
					ar >> nExtraSize;
				}
				MSCompoM* pDefReinforceM = pBldg->GetCompoM(nReinforceMID);
				if (pDefReinforceM == nullptr)
				{
					pDefReinforceM = pBldg->GetDefaultOpeningReinforceM();
					if (pDefReinforceM != nullptr)	nReinforceMID = pDefReinforceM->m_ID;
				}
				pDoorM->SetOpeningReinforceMID(nReinforceMID);
				pDoorM->SetExtraSize(nExtraSize);
				mp_CompoM = pDoorM;
				if(mp_CompoM->m_Name == "")	mp_CompoM->m_Name = _T("NONAME");
			}
		}
    }
}

MSObject* MSDoorG::Copy( bool bNewID /*= true*/ )
{
	MSDoorG* pNewObj = new MSDoorG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSDoorG::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;
	if( !pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() != msDoorG) { ASSERT(0); return; }

	MSWallOpeningG::CopyFromMe(pSource_in, bNewID);
}
