#include "stdafx.h"
#include "NXBoxWallHelper.h"

#include "MSRebarBuilding.h"
#include "MSCompoM.h"
#include "MSWallMemberG.h"
#include "MSRcLineMemberM.h"
#include "MSDoorM.h"
#include "MSDoorG.h"
#include "MSModelChecker.h"
#include "CogReader.h"

///////////////////////////////// NXBoxWallOpeningData ////////////////////////////////////////////////////////
NXBoxWallOpeningData::NXBoxWallOpeningData()
{
	m_BotLevel = 0;
	m_Height = 500;
	m_ExtSize = 0;
}

NXBoxWallOpeningData::~NXBoxWallOpeningData()
{

}

double NXBoxWallOpeningData::GetLength()
{
	return m_sVec.Distance(m_eVec);
}

CString NXBoxWallOpeningData::MakeName()
{
	CString name;
	name.Format(_T("%s-%ldx%ld(%ld)"), m_Name, (long)GetLength(), GetHeight(), GetExtSize());
	return name;
}
///////////////////////////////// NXBoxWallData ////////////////////////////////////////////////////////
NXBoxWallData::NXBoxWallData() : NXBoxData()
{
	m_Thick = 500;
	m_TopOffset = 0;
	m_BotOffset = 0;
}

NXBoxWallData::~NXBoxWallData()
{
	DeleteOpeningDatas();
}

void NXBoxWallData::DeleteOpeningDatas()
{
	for (int i = 0; i < ma_Opening.size(); i++)
		delete ma_Opening[i];
	ma_Opening.clear();
}

NXBoxWallOpeningData* NXBoxWallData::AddOpeningData(CString name, GM2DVector sVec, GM2DVector eVec, int level, int height, int extSize)
{
	NXBoxWallOpeningData* pOpening = new NXBoxWallOpeningData();
	pOpening->SetName(name);
	pOpening->SetStartVec(sVec);
	pOpening->SetEndVec(eVec);
	pOpening->SetBotLevel(level);
	pOpening->SetHeight(height);
	pOpening->SetExtSize(extSize);
	ma_Opening.push_back(pOpening);
	return pOpening;
}

///////////////////////////////// NXBoxWallHelper ////////////////////////////////////////////////////////

NXBoxWallHelper::NXBoxWallHelper(CogReader* pReader) : NXBoxHelper(pReader)
{
}


NXBoxWallHelper::~NXBoxWallHelper()
{
}

MSCompoG*	NXBoxWallHelper::MakeData(NXBoxData* pData)
{
	mp_Data = pData;
	NXBoxWallData* pBoxData = (NXBoxWallData*)mp_Data;

	MSWallMemberG* pCompoG = (MSWallMemberG*)GetBuilding()->CreateNewCompoG(MSCompoG::msWallG);
	pCompoG->mp_CompoM = FindCompoM(pBoxData->GetName(), MSCompoM::msBWallM);
	if (pCompoG->mp_CompoM == nullptr)
		pCompoG->mp_CompoM = CreateNewWallM(pBoxData->GetThick(), MakeDefaultName());
	else
		CheckCompoM(pCompoG->mp_CompoM);

	pCompoG->ReplaceVec(0, new GM2DVector(pBoxData->GetSVec().m_X, pBoxData->GetSVec().m_Y));
	pCompoG->ReplaceVec(1, new GM2DVector(pBoxData->GetEVec().m_X, pBoxData->GetEVec().m_Y));

	pCompoG->m_SOffsetZ = pBoxData->GetBotOffset();
	pCompoG->m_SOffsetZ_End = pBoxData->GetBotOffset();
	pCompoG->m_EOffsetZ = pBoxData->GetTopOffset();
	pCompoG->m_EOffsetZ_End = pBoxData->GetTopOffset();
	pCompoG->SetOwner(GetFloor());

	MakeWallOpenings(pCompoG);
	if (!m_IsMaked)
	{
		delete pCompoG;
		return nullptr;
	}

	return pCompoG;
}

void NXBoxWallHelper::MakeWallOpenings(MSCompoG* pCompoG)
{
	NXBoxWallData* pBoxData = (NXBoxWallData*)mp_Data;
	MSWallMemberG* pWallG = (MSWallMemberG*)pCompoG;
	GM2DLineVector wallLine(pBoxData->GetSVec(), pBoxData->GetEVec());

	CString DoorMName;
	MSCompoM* pDefaultReinforceM = FindOrCreateDefaultReinforceM();
	for (int i = 0; i < pBoxData->ma_Opening.size(); i++)
	{
		NXBoxWallOpeningData* pOpeningData = pBoxData->ma_Opening[i];
		GM2DVector sVec = pOpeningData->GetStartVec();
		GM2DVector eVec = pOpeningData->GetEndVec();

		DoorMName = pOpeningData->MakeName();

		MSDoorM* pDoorM= (MSDoorM*)mp_Reader->GetCompoM(DoorMName, MSCompoM::msDoorM);
		if (pDoorM == nullptr)
		{
			pDoorM = (MSDoorM*)GetBuilding()->CreateNewCompoM(MSCompoM::msDoorM);
			pDoorM ->SetName(pOpeningData->MakeName());
			pDoorM->SetWidth(pOpeningData->GetLength());
			pDoorM->SetHeight(pOpeningData->GetHeight());
			pDoorM->SetExtraSize(pOpeningData->GetExtSize());
			MSCompoM* pReinforceM = GetBuilding()->GetCompoM(pOpeningData->GetName(), MSCompoM::msOpeningReinforceM);
			if (pReinforceM == nullptr)	pReinforceM = pDefaultReinforceM;

			pDoorM->SetOpeningReinforceMID(pReinforceM->GetID());
		}

		MSDoorG* pDoorG = new MSDoorG();
		pDoorG->GetID();
		pDoorG->SetCompoM(pDoorM);
		pDoorG->SetOwnerCompoG(pWallG);
		pDoorG->SetOwner(GetFloor());

		GM2DVector offset;
		offset.m_X = fabs(pBoxData->GetSVec().m_X - pOpeningData->GetStartVec().m_X);
		offset.m_Y = pBoxData->GetBotOffset() + pOpeningData->GetBotLevel();
		pDoorG->m_BaseOffset = offset;
		
		if (MSModelChecker::CheckModel_WallOpeningStep2(pWallG, pDoorG) == true)
			pWallG->Add(pDoorG);
		else
			delete pDoorG; 
	}
}

MSCompoM* NXBoxWallHelper::FindOrCreateDefaultReinforceM()
{
	MSCompoM* ReinforceM = GetBuilding()->GetDefaultOpeningReinforceM();
 	if (ReinforceM != nullptr)	return ReinforceM;
 
	ReinforceM = GetBuilding()->CreateNewCompoM(MSCompoM::msOpeningReinforceM);
	ReinforceM->SetName(_T("DEFAULTNAME"));
	ReinforceM->SetMaterial(GetBuilding()->GetDefMaterial(MSMaterial::msConcrete));
	mp_Reader->SetCreateReinforceM(ReinforceM);
	return ReinforceM;
}

MSCompoM* NXBoxWallHelper::CreateNewWallM(double dT, CString TheName)
{
	CString defName;
	defName.Format(_T("%s(%ld)"), TheName, (long)dT);

	CString msg;
	msg.Format(_T("%s %s"), TheName, WARNING_NAME_NONE);
	CreateErrorObject(mp_Data->GetFigureID(), W_CODE_NAME_NONE, WARNING_LEVEL, mp_Reader->GetGroupName(), msg);
	return mp_Reader->CreateNewWallM(dT, defName, true);
}

bool NXBoxWallHelper::CheckCompoM(MSCompoM* pCompoM)
{
	NXBoxWallData * pBoxData = (NXBoxWallData*)mp_Data;

	CString message;
	if (fabs(pCompoM->GetThick() - pBoxData->GetThick()) > 1.0)
	{
		message.Format(_T("%s - 단면정보(%ld)와 평면크기(%ld)가 다릅니다."), pCompoM->m_Name, (long)pCompoM->GetThick(), (long)pBoxData->GetThick());
		CreateErrorObject(pBoxData->GetFigureID(), W_CODE_SIZE_DIFF, ERROR_LEVEL, mp_Reader->GetGroupName(), message);
	}
	return m_IsMaked;
}
