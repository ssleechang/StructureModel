#include "stdafx.h"
#include "NXBoxLowWallHelper.h"

#include "MSRebarBuilding.h"
#include "MSCompoM.h"
#include "MSHandRailG.h"
#include "MSRcHandRailM.h"

#include "CogReader.h"
///////////////////////////////// NXBoxLowWallData ////////////////////////////////////////////////////////
NXBoxLowWallData::NXBoxLowWallData() : NXBoxData()
{
	m_Thick = 500;
	m_Offset = 0;
	m_TopDepth = 300;
	m_BotDepth = 0;
}

NXBoxLowWallData::~NXBoxLowWallData()
{

}

///////////////////////////////// NXBoxLowWallHelper ////////////////////////////////////////////////////////

NXBoxLowWallHelper::NXBoxLowWallHelper(CogReader* pReader) : NXBoxHelper(pReader)
{
}


NXBoxLowWallHelper::~NXBoxLowWallHelper()
{
}

MSCompoG*	NXBoxLowWallHelper::MakeData(NXBoxData* pData)
{
	mp_Data = pData;
	NXBoxLowWallData* pBoxData = (NXBoxLowWallData*)mp_Data;

	MSHandRailG* pCompoG = (MSHandRailG*)GetBuilding()->CreateNewCompoG(MSCompoG::msHandRailG);
	pCompoG->mp_CompoM = FindCompoM(pBoxData->GetName(), MSCompoM::msHandRailM);
	if (pCompoG->mp_CompoM == nullptr)
		pCompoG->mp_CompoM = CreateNewLowWallM(pBoxData->GetThick(), MakeDefaultName());
	else
		CheckCompoM(pCompoG->mp_CompoM);

	pCompoG->ReplaceVec(0, new GM2DVector(pBoxData->GetSVec().m_X, pBoxData->GetSVec().m_Y));
	pCompoG->ReplaceVec(1, new GM2DVector(pBoxData->GetEVec().m_X, pBoxData->GetEVec().m_Y));

	pCompoG->m_SOffsetZ = pBoxData->GetOffset();
	pCompoG->m_EOffsetZ = pBoxData->GetOffset();

	if (!m_IsMaked)
	{
		delete pCompoG;
		return nullptr;
	}

	return pCompoG;
}

MSCompoM* NXBoxLowWallHelper::CreateNewLowWallM(double dT, CString TheName)
{
	CString defName;
	defName.Format(_T("%s(%ld)"), TheName, (long)dT);

	CString msg;
	msg.Format(_T("%s %s"), TheName, WARNING_NAME_NONE);
	CreateErrorObject(mp_Data->GetFigureID(), W_CODE_NAME_NONE, WARNING_LEVEL, mp_Reader->GetGroupName(), msg);
	return mp_Reader->CreateNewHandRailM(dT, defName);
}

bool NXBoxLowWallHelper::CheckCompoM(MSCompoM* pCompoM)
{
	NXBoxLowWallData * pBoxData = (NXBoxLowWallData*)mp_Data;

	CString message;
	if (fabs(pCompoM->GetThick() - pBoxData->GetThick()) > 1.0)
	{
		message.Format(_T("%s - 단면정보(%ld)와 평면크기(%ld)가 다릅니다."), pCompoM->m_Name, (long)pCompoM->GetThick(), (long)pBoxData->GetThick());
		CreateErrorObject(pBoxData->GetFigureID(), W_CODE_SIZE_DIFF, ERROR_LEVEL, mp_Reader->GetGroupName(), message);
	}
	return m_IsMaked;
}
