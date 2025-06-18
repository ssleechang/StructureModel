#include "stdafx.h"
#include "NXBoxIosFootHelper.h"

#include "MSRebarBuilding.h"
#include "MSCompoM.h"
#include "MSIsoFootG.h"
#include "MSRcLineMemberM.h"

#include "CogReader.h"
///////////////////////////////// NXBoxIsoFootData ////////////////////////////////////////////////////////
NXBoxIsoFootData::NXBoxIsoFootData() : NXBoxData()
{
	m_Width = 500;
	m_Depth = 600;
	m_Offset = 0;
	m_Rotate = 0.0;
}

NXBoxIsoFootData::~NXBoxIsoFootData()
{

}

///////////////////////////////// NXBoxIosFootHelper ////////////////////////////////////////////////////////
NXBoxIosFootHelper::NXBoxIosFootHelper(CogReader* pReader) : NXBoxHelper(pReader)
{
}


NXBoxIosFootHelper::~NXBoxIosFootHelper()
{
}

MSCompoG*	NXBoxIosFootHelper::MakeData(NXBoxData* pData)
{
	mp_Data = pData;
	NXBoxIsoFootData* pBoxData = (NXBoxIsoFootData*)mp_Data;

	MSIsoFootG* pCompoG = (MSIsoFootG*)GetBuilding()->CreateNewCompoG(MSCompoG::msIsoFootG);
	pCompoG->mp_CompoM = FindCompoM(pBoxData->GetName(), MSCompoM::msIsoFootM);
	if (pCompoG->mp_CompoM == nullptr)
	{
		int shape = 1;
		pCompoG->mp_CompoM = CreateNewIsoFootM(pBoxData->GetWidth(), pBoxData->GetDepth(), shape, MakeDefaultName());
	}
	else
		CheckCompoM(pCompoG->mp_CompoM);

	pCompoG->ReplaceVec(0, new GM2DVector(pBoxData->GetCenterPoint().m_X, pBoxData->GetCenterPoint().m_Y));

	pCompoG->m_SOffsetZ = pBoxData->GetOffset();
	pCompoG->m_dAxisRotation = pBoxData->GetRotate();
	pCompoG->SetPointAlignType(COLUMNALIGN_CENTER);

	if (!m_IsMaked)
	{
		delete pCompoG;
		return nullptr;
	}

	return pCompoG;
}

MSCompoM* NXBoxIosFootHelper::CreateNewIsoFootM(double dB, double dH, long shape, CString TheName)
{
	CString defName;
	defName.Format(_T("%s(%ld x %ld)"), TheName, (long)dB, (long)dH);

	CString msg;
	msg.Format(_T("%s %s"), TheName, WARNING_NAME_NONE);
	CreateErrorObject(mp_Data->GetFigureID(), W_CODE_NAME_NONE, WARNING_LEVEL, mp_Reader->GetGroupName(), msg);
	return mp_Reader->CreateNewColumnM(dB, dH, shape, defName);
}

bool NXBoxIosFootHelper::CheckCompoM(MSCompoM* pCompoM)
{
	NXBoxIsoFootData* pBoxData = (NXBoxIsoFootData*)mp_Data;
	MSRcLineMemberM* pColumnM = (MSRcLineMemberM*)pCompoM;

	CString message;
	if (fabs(pColumnM->GetWidth() - pBoxData->GetWidth()) > 1.0 || fabs(pColumnM->GetDepth() - pBoxData->GetDepth()) > 1.0)
	{
		message.Format(_T("%s - 단면정보(%ldx%ld)와 평면크기(%ldx%ld)가 다릅니다."), pCompoM->m_Name,
			(long)pCompoM->GetWidth(), (long)pCompoM->GetDepth(), (long)pBoxData->GetWidth(), (long)pBoxData->GetDepth());
		CreateErrorObject(pBoxData->GetFigureID(), W_CODE_SIZE_DIFF, ERROR_LEVEL, mp_Reader->GetGroupName(), message);
	}
	return m_IsMaked;
}
