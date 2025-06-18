#include "stdafx.h"
#include "NXBoxPitHelper.h"

#include "MSRebarBuilding.h"
#include "MSCompoM.h"
#include "MSPitG.h"
#include "MSRcLineMemberM.h"

#include "CogReader.h"


///////////////////////////////// NXBoxPitData ////////////////////////////////////////////////////////
NXBoxPitData::NXBoxPitData() : NXBoxData()
{
	m_PitThick = 500;
	m_PitDepth = 2000;
	m_WellThick = 300;
	m_IsPlaceWell = false;
	m_OutLine = nullptr;
	m_InnerLine = nullptr;
	m_WellLine = nullptr;
}

NXBoxPitData::~NXBoxPitData()
{

}

///////////////////////////////// NXBoxPitHelper ////////////////////////////////////////////////////////
NXBoxPitHelper::NXBoxPitHelper(CogReader* pReader) : NXBoxHelper(pReader)
{
}


NXBoxPitHelper::~NXBoxPitHelper()
{
}

MSCompoG*	NXBoxPitHelper::MakeData(NXBoxData* pData)
{
	mp_Data = pData;
	NXBoxPitData* pBoxData = (NXBoxPitData*)mp_Data;

	MSPitG* pCompoG = (MSPitG*)GetBuilding()->CreateNewCompoG(MSCompoG::msPitG);
	pCompoG->mp_CompoM = FindCompoM(pBoxData->GetName(), MSCompoM::msPitM);
	if (pCompoG->mp_CompoM == nullptr)
		pCompoG->mp_CompoM = CreateNewPitM(MakeDefaultName());
	else
		CheckCompoM(pCompoG->mp_CompoM);

	pCompoG->m_dThick = pBoxData->GetPitThick();
	pCompoG->m_dPitDepth = pBoxData->GetPitDepth();
	pCompoG->m_dInnerDepth = pBoxData->GetWellThick();

	if (!pBoxData->GetOutLine()->IsClosed())		pBoxData->GetOutLine()->MakeClosedPolyline(DTOL_GM);
	if (!pBoxData->GetInnerLine()->IsClosed())	pBoxData->GetInnerLine()->MakeClosedPolyline(DTOL_GM);
	if (pBoxData->GetIsPlaceWell() && !pBoxData->GetWellLine()->IsClosed())		pBoxData->GetWellLine()->MakeClosedPolyline(DTOL_GM);

	if (pCompoG->mp_OutPoly != nullptr)	delete pCompoG->mp_OutPoly;
	pCompoG->mp_OutPoly = pBoxData->GetOutLine();
	
	pCompoG->AddInPoly(pBoxData->GetInnerLine());
	if (pBoxData->GetIsPlaceWell())
		pCompoG->AddWellPoly(pBoxData->GetWellLine());
	else
	{
		delete pBoxData->GetWellLine();
		pBoxData->SetWellLine(nullptr);
	}

	if (!m_IsMaked)
	{
		delete pCompoG;
		return nullptr;
	}

	return pCompoG;
}

MSCompoM* NXBoxPitHelper::CreateNewPitM(CString TheName)
{
	CString msg;
	msg.Format(_T("%s %s"), TheName, WARNING_NAME_NONE);
	CreateErrorObject(mp_Data->GetFigureID(), W_CODE_NAME_NONE, WARNING_LEVEL, mp_Reader->GetGroupName(), msg);
	MSCompoM* pPitM = GetBuilding()->CreateNewCompoM(MSCompoM::msPitM);
	pPitM->SetName(TheName);
	return pPitM;
}

bool NXBoxPitHelper::CheckCompoM(MSCompoM* pCompoM)
{
	NXBoxPitData * pBoxData = (NXBoxPitData*)mp_Data;
	// Pit의 Thick, PitThick, WellThick은 G의 정보

	return m_IsMaked;
}
