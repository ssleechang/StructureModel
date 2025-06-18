#include "stdafx.h"
#include "NXBoxWallFootHelper.h"
#include "MSRebarBuilding.h"
#include "MSCompoM.h"
#include "MSWallFootG.h"
#include "MSRcLineMemberM.h"

#include "CogReader.h"
///////////////////////////////// NXBoxWallFootData ////////////////////////////////////////////////////////
NXBoxWallFootData::NXBoxWallFootData() : NXBoxData()
{
	m_Width = 500;
	m_Offset = 0;
}

NXBoxWallFootData::~NXBoxWallFootData()
{

}

///////////////////////////////// NXBoxWallFootHelper ////////////////////////////////////////////////////////

NXBoxWallFootHelper::NXBoxWallFootHelper(CogReader* pReader) : NXBoxHelper(pReader)
{
}


NXBoxWallFootHelper::~NXBoxWallFootHelper()
{
}

MSCompoG*	NXBoxWallFootHelper::MakeData(NXBoxData* pData)
{
	mp_Data = pData;
	NXBoxWallFootData* pBoxData = (NXBoxWallFootData*)mp_Data;

	MSWallFootG* pCompoG = (MSWallFootG*)GetBuilding()->CreateNewCompoG(MSCompoG::msWallFootG);
	pCompoG->mp_CompoM = FindCompoM(pBoxData->GetName(), MSCompoM::msWallFootM);
	if (pCompoG->mp_CompoM == nullptr)
		pCompoG->mp_CompoM = CreateNewWallFootM(pBoxData->GetWidth(), MakeDefaultName());
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

MSCompoM* NXBoxWallFootHelper::CreateNewWallFootM(double dB, CString TheName)
{
	CString defName;
	double dH = 500;
	defName.Format(_T("%s(%ld x %ld)"), TheName, (long)dB, (long)dH);

	CString msg;
	msg.Format(_T("%s %s"), TheName, WARNING_NAME_NONE);
	CreateErrorObject(mp_Data->GetFigureID(), W_CODE_NAME_NONE, WARNING_LEVEL, mp_Reader->GetGroupName(), msg);
	return mp_Reader->CreateNewWallFootM(dB, defName);
}

bool NXBoxWallFootHelper::CheckCompoM(MSCompoM* pCompoM)
{
	NXBoxWallFootData * pBoxData = (NXBoxWallFootData*)mp_Data;

	CString message;
	if (fabs(pCompoM->GetWidth() - pBoxData->GetWidth()) > 1.0)
	{
		message.Format(_T("%s - 단면정보(%ldx%ld)와 평면크기(%ldx%ld)가 다릅니다."), pCompoM->m_Name,
			(long)pCompoM->GetWidth(), (long)pCompoM->GetDepth(), (long)pBoxData->GetWidth(), (long)pCompoM->GetDepth());
		CreateErrorObject(pBoxData->GetFigureID(), W_CODE_SIZE_DIFF, ERROR_LEVEL, mp_Reader->GetGroupName(), message);
	}
	return m_IsMaked;
}
