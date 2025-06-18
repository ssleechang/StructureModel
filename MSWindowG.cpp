#include "StdAfx.h"
#include "MSWindowG.h"
#include "MSRebarBuilding.h"
#include "MSWindowM.h"
#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSWindowG, MSWallOpeningG, VERSIONABLE_SCHEMA | 1)
MSWindowG::MSWindowG(void)
{
}

MSWindowG::~MSWindowG(void)
{
}

void MSWindowG::Dump(EFS::EFS_Buffer& buffer)
{
	MSWallOpeningG::Dump(buffer);
}

void MSWindowG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSWallOpeningG::Recovery(buffer);
}

void MSWindowG::Serialize(CArchive &ar)
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
			MSWindowM* pWindowM = (MSWindowM*)mp_CompoM;
			long MType = pWindowM->m_nType;

			ar << MType;

			ar << pWindowM->GetMaterial()->m_ID;
			ar << pWindowM->m_Name;
			ar << pWindowM->GetWidth();
			ar << pWindowM->GetHeight();

			ar << pWindowM->GetOpeningReinforceMID();
			ar << pWindowM->GetExtraSize();
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

				MSWindowM* pWindowM = (MSWindowM*)pBldg->CreateNewCompoM(MType);
				pWindowM->m_nType = (MSCompoM::Type_MSCompoM)MType;

				long nMaterialID = 0;
				ar >> nMaterialID;
				pWindowM->SetMaterial(pBldg->GetMaterial(nMaterialID));
				ar >> pWindowM->m_Name;
				double dWidth = 0, dHeight = 0;
				ar >> dWidth;
				ar >> dHeight;
				pWindowM->SetWidth(dWidth);
				pWindowM->SetHeight(dHeight);
				pWindowM->mp_Owner = pBldg;

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
				pWindowM->SetOpeningReinforceMID(nReinforceMID);
				pWindowM->SetExtraSize(nExtraSize);

				mp_CompoM = pWindowM;
				if(mp_CompoM->m_Name == "")	mp_CompoM->m_Name = _T("NONAME");
			}
		}
	}
}
MSObject* MSWindowG::Copy( bool bNewID /*= true*/ )
{
	MSWindowG* pNewObj = new MSWindowG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSWindowG::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;
	if( !pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() != msWindowG) { ASSERT(0); return; }

	MSWallOpeningG::CopyFromMe(pSource_in, bNewID);
}
