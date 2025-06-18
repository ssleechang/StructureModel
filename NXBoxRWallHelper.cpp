#include "stdafx.h"
#include "NXBoxRWallHelper.h"

#include "MSRebarBuilding.h"
#include "MSCompoM.h"
#include "MSWallMemberG.h"
#include "MSRcFaceMemberM.h"

#include "CogReader.h"

///////////////////////////////// NXBoxRWallData ////////////////////////////////////////////////////////
NXBoxRWallData::NXBoxRWallData() : NXBoxData()
{
	m_Thick = 200;
	m_IsLeftInside = true;
	m_TopOffset = 0;
	m_BotOffset = 0;
}

NXBoxRWallData::~NXBoxRWallData()
{

}

///////////////////////////////// NXBoxRWallHelper ////////////////////////////////////////////////////////

NXBoxRWallHelper::NXBoxRWallHelper(CogReader* pReader) : NXBoxHelper(pReader)
{
}


NXBoxRWallHelper::~NXBoxRWallHelper()
{
}

MSCompoG*	NXBoxRWallHelper::MakeData(NXBoxData* pData)
{
	mp_Data = pData;
	NXBoxRWallData* pBoxData = (NXBoxRWallData*)mp_Data;

	MSWallMemberG* pCompoG = (MSWallMemberG*)GetBuilding()->CreateNewCompoG(MSCompoG::msWallG);
	pCompoG->mp_CompoM = FindCompoM(pBoxData->GetName(), MSCompoM::msRWallM);
	if (pCompoG->mp_CompoM == nullptr)
		pCompoG->mp_CompoM = CreateNewRWallM(pBoxData->GetThick(), MakeDefaultName());
	else
		CheckCompoM(pCompoG->mp_CompoM);

	pCompoG->ReplaceVec(0, new GM2DVector(pBoxData->GetSVec().m_X, pBoxData->GetSVec().m_Y));
	pCompoG->ReplaceVec(1, new GM2DVector(pBoxData->GetEVec().m_X, pBoxData->GetEVec().m_Y));

	pCompoG->m_SOffsetZ = pBoxData->GetBotOffset();
	pCompoG->m_SOffsetZ_End = pBoxData->GetBotOffset();
	pCompoG->m_EOffsetZ = pBoxData->GetTopOffset();
	pCompoG->m_EOffsetZ_End = pBoxData->GetTopOffset();
	pCompoG->SetLeftInside(pBoxData->GetIsLeftInside());
	if (!m_IsMaked)
	{
		delete pCompoG;
		return nullptr;
	}

	return pCompoG;
}

MSCompoM* NXBoxRWallHelper::CreateNewRWallM(double dT, CString TheName)
{
	CString defName;
	defName.Format(_T("%s(%ld)"), TheName, (long)dT);

	CString msg;
	msg.Format(_T("%s %s"), TheName, WARNING_NAME_NONE);
	CreateErrorObject(mp_Data->GetFigureID(), W_CODE_NAME_NONE, WARNING_LEVEL, mp_Reader->GetGroupName(), msg);
	return mp_Reader->CreateNewWallM(dT, defName, false);
}

bool NXBoxRWallHelper::CheckCompoM(MSCompoM* pCompoM)
{
	NXBoxRWallData * pBoxData = (NXBoxRWallData*)mp_Data;

	CString message;
	if (fabs(pCompoM->GetThick() - pBoxData->GetThick()) > 1.0)
	{
		message.Format(_T("%s - 단면정보(%ld)와 평면크기(%ld)가 다릅니다."), pCompoM->m_Name, (long)pCompoM->GetThick(), (long)pBoxData->GetThick());
		CreateErrorObject(pBoxData->GetFigureID(), W_CODE_SIZE_DIFF, ERROR_LEVEL, mp_Reader->GetGroupName(), message);
	}
	return m_IsMaked;
}
