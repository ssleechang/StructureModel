#include "StdAfx.h"
#include "MSContainer.h"

#include "MSCompoG.h"
#include "MSConnection.h"
#include "MSCompoM.h"
#include "MSConcrete.h"
#include "MSSteel.h"
#include "MSGridLine.h"

#include "MSBeamG.h"
#include "MSBraceG.h"
#include "MSColumnG.h"
#include "MSWallColumnG.h"
#include "MSIsoFootG.h"
#include "MSSlabG.h"
#include "MSPitG.h"
#include "MSRampG.h"
#include "MSWallFootG.h"
#include "MSGenStairG.h"
#include "MSWallMemberG.h"
#include "MSLineLevelDifG.h"
#include "MSHandRailG.h"
#include "MSWindowG.h"
#include "MSDoorG.h"
#include "MSFloor.h"
#include "MSWallOpeningG.h"
#include "MSLevelZone.h"
#include "MSSectionView.h"
#include "MSConstructionZone.h"
#include "MSBlock.h"
#include "MSWell.h"
#include "MSTrench.h"
#include "MSOpeningG.h"
#include "MSWallOpeningExtraG.h"
#include "GMLib/MSVersionInfo.h"
#include "MSBuiltUpBeamG.h"
#include "XMSRowM.h"
#include "MSPolylineObject.h"
#include "MSHaunchG.h"
#include "MSFWSurfaceFoldingG.h"
#include "MSFWShadeG.h"
#include "MSFWSwitchBoxG.h"
#include "MSFWSoundAbsorptionG.h"
#include "MSHaunchG.h"

#include "MSModelChecker.h"

#include <math.h>
#include "../RADFCore_CLI/RadfContextWrapper.h"
#include "../Radfcore_CLI/AppGlobalOptionWrapper.h"

#include "MSBaseBuilding.h"
IMPLEMENT_SERIAL( MSContainer, MSElement, VERSIONABLE_SCHEMA | 1 )
MSContainer::MSContainer()
{
	Init(true);
}

MSContainer::~MSContainer(void)
{
}

template<template <typename> class P = std::less >
struct compare_pair_second {

	template<class T1, class T2> bool operator()(const std::pair<T1,T2>&left, const std::pair<T1,T2>&right) {
		MSCompoG* pCompoG1 = left.second;
		MSCompoG* pCompoG2 = right.second;
		return pCompoG1->m_nCmdCnt < pCompoG2->m_nCmdCnt;
	}
};


void MSContainer::Serialize(CArchive &ar)
{
	MSElement::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_Name;
		int CompoGCount = mm_CompoG.size();
		ar << CompoGCount;
        long nCount=0;
		
		vector<std::pair<long, MSCompoG*> > CompoGArr(mm_CompoG.begin(), mm_CompoG.end());
		sort(CompoGArr.begin(), CompoGArr.end(), compare_pair_second<std::less>());

		vector<std::pair<long, MSCompoG*> > ::iterator it;
		for(it = CompoGArr.begin() ; it != CompoGArr.end() ; it++)
		{
			MSCompoG* pCompoG = it->second;
			
			int type = pCompoG->GetType();
			ar << type;
			pCompoG->Serialize(ar);
			if(pCompoG->mp_CompoM->m_ID == 0 && pCompoG->mp_CompoM->m_Name == "")
				pCompoG->mp_CompoM->m_Name = _T("NONAME");
		}
// 		map<long, MSCompoG*>::iterator itCompoG;
// 		for( itCompoG = mm_CompoG.begin(); itCompoG != mm_CompoG.end(); itCompoG++)
// 		{
//             nCount++;
// 			MSCompoG* pCompoG = itCompoG->second;
// 			int type = pCompoG->GetType();
// 			ar << type;
// 			pCompoG->Serialize(ar);
// 		}

		int nGIDToLinkedCompoGsCount = mm_GIDToLinkedCompoGIDArr.size();
		ar << nGIDToLinkedCompoGsCount;

		for (auto itGIDToLinked = mm_GIDToLinkedCompoGIDArr.begin(); itGIDToLinked != mm_GIDToLinkedCompoGIDArr.end(); itGIDToLinked++)
		{
			int nGID = itGIDToLinked->first;
			ar << nGID;

			int nLinkedCompoGCount = itGIDToLinked->second.size();
			ar << nLinkedCompoGCount;

			for (int idx = 0; idx < itGIDToLinked->second.size(); idx++)
			{
				int nLinkedGID = itGIDToLinked->second[idx];
				ar << nLinkedGID;

			}
		}
	}
	else
	{
		ar >> m_Name;
		int CompoGCount = 0;
		ar >> CompoGCount;
		for( int iCompoG = 0; iCompoG < CompoGCount; iCompoG++)
		{
			int type;
			ar >> type;

			MSBaseBuilding* pBuilding = GetBuilding();
			MSCompoG* pCompoG = CreateNewCompoG(type);
			pCompoG->SetOwner(this);
			pCompoG->Serialize(ar);

			if (MSVersionInfo::GetCurrentVersion() < 20180416) //데크슬래브에 Sx,SY 추가
			{
				if (pCompoG->GetType() == Type_MSCompoG::msSlabG && pCompoG->IsRebarEmbeddedDeckSlab())
				{
					pCompoG->m_VolumeAreaEMS.ma_Param.push_back(0);
					pCompoG->m_VolumeAreaEMS.ma_Param.push_back(0);
				}
			}

			if (MSVersionInfo::GetCurrentVersion() < 20180511) // BH-2518
			{
				if (pCompoG->GetType() == Type_MSCompoG::msSlabG && !(pCompoG->IsGeneralDeckSlab() || pCompoG->IsSlopeMember()))
				{
					pCompoG->m_VolumeAreaEMS.ma_Param.push_back(0);
					pCompoG->m_VolumeAreaEMS.ma_Param.push_back(0);
					pCompoG->m_VolumeAreaEMS.ma_Param.push_back(0);
				}
			}

			if (MSVersionInfo::GetCurrentVersion() < 20170808)
			{
				MSColumnG* pColumnG = dynamic_cast<MSColumnG*>(pCompoG);
				if (pColumnG != nullptr && pColumnG->IsCustomM())
				{
					
					CString groupName = _T("Custom Section");
					int errorCode = -10000;
					CString strMessage;
					strMessage.Format(_T("%s : 기존에 입력된 부재(%ld)가 삭제되었습니다"), m_Name, pCompoG->m_ID);
					MSModelChecker::MakeErrorObject(groupName, strMessage, 0, MESSAGE_LEVEL_ERROR, errorCode, pCompoG);
					delete pCompoG;
					continue;
				}
			}

			if(pCompoG->mp_CompoM->m_ID == 0 && pCompoG->mp_CompoM->m_Name == "")
				pCompoG->mp_CompoM->m_Name = _T("NONAME");
			if(pCompoG->IsLinkMember())
			{
				MSLinkMemberG* pLinkG = (MSLinkMemberG*)pCompoG;
				if(pLinkG->mp_LLine == NULL || pLinkG->mp_RLine == NULL)
					pLinkG->SetLines();
			}
			else if(pCompoG->GetType() == MSElement::msSlabG)
			{
				MSSlabG* pSlabG = (MSSlabG*)pCompoG;
				//[BH-2518] ������ ���� �۾�
				if (MSVersionInfo::GetCurrentVersion() < 20180511)
					pSlabG->SetProfile(pSlabG->mp_PolylineInfo->GetProfile());

				GM2DPolyline* pPoly = pSlabG->GetProfile();

				if(pPoly == NULL || pPoly->GetNumLines() < 2)
				{
					CString msg;
					msg.Format(L"[Floor Name: %S] delete SlabG GID:%d",  GetName(), pSlabG->m_ID);
					LoggerWrapper::Log(msg);
					delete pCompoG;
					continue;
				}
			}
			
			if (RadfContextWrapper::Instance()->GetDummyObjectImportIncludeCheck() == false)
			{
				if (pCompoG->GetDummy() == true)
				{
					pBuilding->AddDummyCompoGID(pCompoG->m_ID);

					/*delete pCompoG;
					continue;*/
				}
			}

			pCompoG->MakeBoundary();

			if (pCompoG->IsLinkMember())
			{
				MSLinkMemberG* pLinkG = (MSLinkMemberG*)pCompoG;
				if (pLinkG->mp_LLineTR == NULL || pLinkG->mp_RLineTR == NULL)
					pLinkG->SetTrimmedLines();
			}

			if (MSConnection::CONNECTION_SERIALIZE() && MSVersionInfo::GetCurrentVersion() >= 20141217)
			{
				mm_CompoG[pCompoG->m_ID] = pCompoG;
			}
			else
			{
				Add(pCompoG);
			}
		}

		mm_GIDToLinkedCompoGIDArr.clear();
		int nGIDToLinkedCompoGsCount = 0;
		
		//if (AppGlobalOptionWrapper::Instance()->GetLoadBldgMode() == AppGlobalOptionWrapper::CE
			//|| AppGlobalOptionWrapper::Instance()->GetLoadBldgMode() == AppGlobalOptionWrapper::Rebar && MSVersionInfo::GetCurrentVersion() >= 20240328)
		if(AppGlobalOptionWrapper::Instance()->GetLoadBldgMode() == AppGlobalOptionWrapper::CE
		  || IsMVAddToBHQ())
		{
			ar >> nGIDToLinkedCompoGsCount;

			for (int i = 0; i < nGIDToLinkedCompoGsCount; i++)
			{
				vector<long> LinkedGIDArr;
				int nGID = 0;
				ar >> nGID;

				int nLinkedCompoGIDCount = 0;
				ar >> nLinkedCompoGIDCount;

				for (int iLinkedCompoGIDIdx = 0; iLinkedCompoGIDIdx < nLinkedCompoGIDCount; iLinkedCompoGIDIdx++)
				{
					int nLinkedGID = 0;
					ar >> nLinkedGID;
					LinkedGIDArr.push_back(nLinkedGID);
				}

				if (nLinkedCompoGIDCount > 0)
					mm_GIDToLinkedCompoGIDArr.insert(make_pair(nGID, LinkedGIDArr));
			}
		}
	}
}

CString MSContainer::GetName()
{
	return m_Name;
}
void MSContainer::SetName(CString name)
{
	m_Name = name;
}
MSObject* MSContainer::Copy( bool bNewID /*= true*/ )
{
	MSContainer* pNewObj;

	pNewObj = new MSContainer();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSContainer::CopyFromMe( MSContainer* pSource, bool bNewID/*=true*/ )
{
	MSElement::CopyFromMe(pSource, bNewID);

	mm_GIDToLinkedCompoGIDArr.clear();
	for (auto itGIDToLinked = pSource->mm_GIDToLinkedCompoGIDArr.begin(); itGIDToLinked != pSource->mm_GIDToLinkedCompoGIDArr.end(); itGIDToLinked++)
		mm_GIDToLinkedCompoGIDArr.insert(make_pair(itGIDToLinked->first, itGIDToLinked->second));
// 	m_Name = pSource->GetName();
}


void MSContainer::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSElement::Init(bInConstructor);
	m_bIsSerializing = false;

	m_Name = _T("");
}

void MSContainer::Add( MSCompoG* pCompoG, bool bChangeOwner /*= false*/, bool isDoit /*= true*/, bool bUpdateTrimmedBoundary /*= true*/ )
{
	if(bChangeOwner)
		pCompoG->SetOwner(this);

	pair<map<long, MSCompoG*>::iterator, bool> bPair;
	bPair = mm_CompoG.insert(make_pair(pCompoG->m_ID, pCompoG));
}

bool MSContainer::Remove( MSCompoG* pCompoG )
{
	long nID = pCompoG->m_ID;
	map<long, MSCompoG*>::iterator it = mm_CompoG.find(nID);
	if(it == mm_CompoG.end())	return false;

	mm_CompoG.erase(it);
	return true;
}

bool MSContainer::Delete( MSCompoG* pCompoG )
{
	if(!Remove(pCompoG))
		return false;
	delete pCompoG;
	return true;
}

bool MSContainer::Modify( MSCompoG* pOldCompoG, MSCompoG* pNewCompoG )
{
	pOldCompoG->CopyFromMe(pNewCompoG);

	return true;
}

MSCompoG* MSContainer::CreateNewCompoG(long nType)
{
	MSCompoG* pCompoG = NULL;
	if(nType==MSCompoG::msBeamG)					pCompoG = new MSBeamG;
	else if(nType==MSCompoG::msBraceG)		pCompoG = new MSBraceG;
	else if(nType==MSCompoG::msWallG)		pCompoG = new MSWallMemberG;
	else if(nType==MSCompoG::msColumnG)		pCompoG = new MSColumnG;
	else if (nType == MSCompoG::msWallColumnG)		pCompoG = new MSWallColumnG;
	else if(nType==MSCompoG::msIsoFootG)	pCompoG = new MSIsoFootG;
	else if(nType==MSCompoG::msRampG)			pCompoG = new MSRampG;
	else if(nType==MSCompoG::msSlabG)			pCompoG = new MSSlabG;
	else if(nType==MSCompoG::msStairG)		pCompoG = new MSGenStairG;
	else if(nType==MSCompoG::msPitG)			pCompoG = new MSPitG;
	else if(nType==MSCompoG::msWallFootG)	pCompoG = new MSWallFootG;
// 	else if(nType==MSCompoG::msLineLevelDifG)	pCompoG = new MSLineLevelDifG;
	else if(nType==MSCompoG::msHandRailG)	pCompoG = new MSHandRailG;
	else if(nType==MSCompoG::msWindowG)	pCompoG = new MSWindowG;
	else if(nType==MSCompoG::msDoorG)			pCompoG = new MSDoorG;
	else if(nType==MSCompoG::msStairPart)			pCompoG = new MSStairPart;
	else if(nType == MSCompoG::msOpeningG)	pCompoG = new MSOpeningG;
	else if(nType == MSCompoG::msWallOpeningExtraG)	pCompoG = new MSWallOpeningExtraG;
	else if (nType == MSCompoG::msBuiltUpBeam)	pCompoG = new MSBuiltUpBeamG;
	else if (nType == MSCompoG::msHaunchG)	pCompoG = new MSHaunchG();
	else if (nType == MSCompoG::msFWSurfaceFoldingG)	pCompoG = new MSFWSurfaceFoldingG;
	else if (nType == MSCompoG::msFWShadeG)	pCompoG = new MSFWShadeG;
	else if (nType == MSCompoG::msFWSwitchBoxG)	pCompoG = new MSFWSwitchBoxG;
	else if (nType == MSCompoG::msFWSoundAbsorptionG)	pCompoG = new MSFWSoundAbsorptionG;
	else if (nType == MSCompoG::msHaunchG)	pCompoG = new MSHaunchG;
	else
		ASSERT(0);

	return pCompoG;
}



void MSContainer::ForEachCompoG( MSActionFunc Action, void* pParameters )
{
	map<long, MSCompoG*>::iterator itCompoG;
	for( itCompoG = mm_CompoG.begin(); itCompoG != mm_CompoG.end(); itCompoG++)
	{
		MSCompoG* pCompoG = itCompoG->second;
		Action(pCompoG, pParameters);
	}
}

void MSContainer::ChooseCompoGThat( MSCondFunc Test, MSContainer& ChoosedCompoGs, void* pParameters )
{
	map<long, MSCompoG*>::iterator itCompoG;
	for( itCompoG = mm_CompoG.begin(); itCompoG != mm_CompoG.end(); itCompoG++)
	{
		MSCompoG* pCompoG = itCompoG->second;
		if(Test(pCompoG, pParameters))
			ChoosedCompoGs.Add(pCompoG);
	}
}

MSCompoG* MSContainer::FirstThat( MSCondFunc Test, void* pParamList )
{
	map<long, MSCompoG*>::iterator itCompoG;
	for( itCompoG = mm_CompoG.begin(); itCompoG != mm_CompoG.end(); itCompoG++)
	{
		MSCompoG* pCompoG = itCompoG->second;
		if(Test(pCompoG, pParamList))
			return pCompoG;
	}
	return NULL;
}

MSCompoG* MSContainer::NextThat( MSCompoG* pCurCompoG, MSCondFunc Test, void* pParamList )
{
	map<long, MSCompoG*>::iterator itCompoG = mm_CompoG.find(pCurCompoG->m_ID);
	if(itCompoG ==mm_CompoG.end())
		return NULL;

	for( itCompoG++ ; itCompoG != mm_CompoG.end(); itCompoG++)
	{
		MSCompoG* pCompoG = itCompoG->second;
		if(Test(pCompoG, pParamList))
			return pCompoG;
	}
	return NULL;
}

void MSContainer::Dump( EFS::EFS_Buffer& buffer )
{
	MSElement::Dump(buffer);
	buffer << m_Name;
}

void MSContainer::Recovery( EFS::EFS_Buffer& buffer )
{
	MSElement::Recovery(buffer);
	buffer >> m_Name;
}

MSLevelZone* MSContainer::CreateNewLevelZone()
{
    MSLevelZone* pTemp = new MSLevelZone();
    return pTemp;
}

MSSectionView* MSContainer::CreateNewSectionView()
{
	MSSectionView* pTemp = new MSSectionView();
	return pTemp;
}

MSConstructionZone* MSContainer::CreateNewConstructionZone()
{
    MSConstructionZone* pTemp = new MSConstructionZone();
    return pTemp;
}

MSOpening* MSContainer::CreateNewOpening()
{
    MSOpening* pTemp = new MSOpening();
    return pTemp;
}

MSWallOpeningG* MSContainer::CreateNewWallOpening()
{
	ASSERT(FALSE);//Abstract Type
    return NULL;
}

MSBlock* MSContainer::CreateNewBlock()
{
	MSBlock* pTemp = new MSBlock();
	return pTemp;
}

MSSlabRebar* MSContainer::CreateNewSlabRebar()
{
    MSSlabRebar* pTemp = new MSSlabRebar();
    return pTemp;
}

MSGridLine* MSContainer::CreateNewGridLine()
{
	MSGridLine* pTemp = new MSGridLine();
	return pTemp;
}

MSWell* MSContainer::CreateNewWell()
{
	MSWell* pTemp = new MSWell();
	return pTemp;
}

MSTrench* MSContainer::CreateNewTrench()
{
	MSTrench* pTemp = new MSTrench();
	return pTemp;
}
MSBuiltUpBeamG* MSContainer::CreateNewBuiltUpBeamG()
{
	MSBuiltUpBeamG* pTemp = new MSBuiltUpBeamG();
	return pTemp;
}

vector<long> MSContainer::GetLinkedCompoGIDArr(long nGID)
{
	map<long, vector<long>>::iterator it = mm_GIDToLinkedCompoGIDArr.find(nGID);

	if (it == mm_GIDToLinkedCompoGIDArr.end())
	{
		vector<long> tempCompoGs;
		return tempCompoGs;
	}

	return it->second;
}