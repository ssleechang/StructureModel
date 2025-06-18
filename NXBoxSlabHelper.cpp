#include "stdafx.h"
#include "NXBoxSlabHelper.h"

#include "MSRebarBuilding.h"
#include "MSCompoM.h"
#include "MSSlabG.h"
#include "MSRcLineMemberM.h"
#include "MSRcSlabM.h"
#include "MSPolylineObject.h"
#include "CogReader.h"

///////////////////////////////// NXBoxSlabData ////////////////////////////////////////////////////////
NXBoxSlabData::NXBoxSlabData() : NXBoxData()
{
	m_Offset = 0;
	mp_Boundary = nullptr;
}

NXBoxSlabData::~NXBoxSlabData()
{

}

///////////////////////////////// NXBoxSlabHelper ////////////////////////////////////////////////////////

NXBoxSlabHelper::NXBoxSlabHelper(CogReader* pReader) : NXBoxHelper(pReader)
{
}


NXBoxSlabHelper::~NXBoxSlabHelper()
{
}

MSCompoG*	NXBoxSlabHelper::MakeData(NXBoxData* pData)
{
	mp_Data = pData;
	NXBoxSlabData* pBoxData = (NXBoxSlabData*)mp_Data;

	double dT = 200;
	MSSlabG* pCompoG = (MSSlabG*)GetBuilding()->CreateNewCompoG(MSCompoG::msSlabG);
	pCompoG->mp_CompoM = FindCompoM(pBoxData->GetName(), MSCompoM::msSlabM);
	if (pCompoG->mp_CompoM == nullptr)
	{
		pCompoG->mp_CompoM = CreateNewSlabM(dT, MakeDefaultName());
		MSRcSlabM* pRebarM = (MSRcSlabM*)pCompoG->GetCompoM()->GetRebarM();
		pRebarM->m_nSlabMType = MSRcSlabM::FLAT_TYPE;
	}
	else
		CheckCompoM(pCompoG->mp_CompoM);

	pCompoG->SetProfile(pBoxData->GetBoundary());
	pCompoG->m_dOffset = pBoxData->GetOffset();

	if (!m_IsMaked)
	{
		delete pCompoG;
		return nullptr;
	}

	return pCompoG;
}

MSCompoM* NXBoxSlabHelper::CreateNewSlabM(double dT, CString TheName)
{
	CString defName;
	double dH = 700;
	defName.Format(_T("%s(%ld)"), TheName, (long)dT, (long)dH);

	CString msg;
	msg.Format(_T("%s %s"), TheName, WARNING_NAME_NONE);
	CreateErrorObject(mp_Data->GetFigureID(), W_CODE_NAME_NONE, WARNING_LEVEL, mp_Reader->GetGroupName(), msg);
	return mp_Reader->CreateNewSlabM(dT, defName);
}

bool NXBoxSlabHelper::CheckCompoM(MSCompoM* pCompoM)
{
	NXBoxSlabData * pBoxData = (NXBoxSlabData*)mp_Data;

	return m_IsMaked;
}
