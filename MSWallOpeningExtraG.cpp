#include "StdAfx.h"
#include "MSWallOpeningExtraG.h"
#include "MSRebarBuilding.h"
#include "MSWallOpeningExtraM.h"

IMPLEMENT_SERIAL(MSWallOpeningExtraG, MSWallOpeningG, VERSIONABLE_SCHEMA | 1)

	MSWallOpeningExtraG::MSWallOpeningExtraG(void)
{
}

MSWallOpeningExtraG::~MSWallOpeningExtraG(void)
{
}

void MSWallOpeningExtraG::Dump(EFS::EFS_Buffer& buffer)
{
	MSWallOpeningG::Dump(buffer);
}

void MSWallOpeningExtraG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSWallOpeningG::Recovery(buffer);
}
void MSWallOpeningExtraG::Serialize(CArchive &ar)
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
			MSWallOpeningExtraM* pExtraM = (MSWallOpeningExtraM*)mp_CompoM;
			long MType = pExtraM->m_nType;

			ar << MType;

			ar << pExtraM->GetMaterial()->m_ID;
			ar << pExtraM->m_Name;
			ar << pExtraM->GetWidth();
			ar << pExtraM->GetHeight();

			ar << pExtraM->GetOpeningReinforceMID();
			ar << pExtraM->GetExtraSize();
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

				MSWallOpeningExtraM* pExtraM = (MSWallOpeningExtraM*)pBldg->CreateNewCompoM(MType);
				pExtraM->m_nType = (MSCompoM::Type_MSCompoM)MType;

				long nMaterialID = 0;
				ar >> nMaterialID;
				pExtraM->SetMaterial(pBldg->GetMaterial(nMaterialID));
				ar >> pExtraM->m_Name;
				double dWidth = 0, dHeight = 0;
				ar >> dWidth;
				ar >> dHeight;
				pExtraM->SetWidth(dWidth);
				pExtraM->SetHeight(dHeight);
				pExtraM->mp_Owner = pBldg;

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

				pExtraM->SetOpeningReinforceMID(nReinforceMID);
				pExtraM->SetExtraSize(nExtraSize);

				mp_CompoM = pExtraM;
				if(mp_CompoM->m_Name == "")	mp_CompoM->m_Name = _T("NONAME");
			}
		}
	}
}

MSObject* MSWallOpeningExtraG::Copy( bool bNewID /*= true*/ )
{
	MSWallOpeningExtraG* pNewObj = new MSWallOpeningExtraG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSWallOpeningExtraG::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;
	if( !pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() != msWallOpeningExtraG) { ASSERT(0); return; }

	MSWallOpeningG::CopyFromMe(pSource_in, bNewID);
}
