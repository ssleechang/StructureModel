#include "stdafx.h"
#include "NXBoxBeamHelper.h"

#include "MSRebarBuilding.h"
#include "MSCompoM.h"
#include "MSBeamG.h"
#include "MSRcLineMemberM.h"

#include "CogReader.h"

///////////////////////////////// NXBoxBeamData ////////////////////////////////////////////////////////
NXBoxBeamData::NXBoxBeamData() : NXBoxData()
{
	m_Width = 500;
	m_Offset = 0;
}

NXBoxBeamData::~NXBoxBeamData()
{

}

///////////////////////////////// NXBoxBeamHelper ////////////////////////////////////////////////////////

NXBoxBeamHelper::NXBoxBeamHelper(CogReader* pReader) : NXBoxHelper(pReader)
{
}


NXBoxBeamHelper::~NXBoxBeamHelper()
{
}

MSCompoG*	NXBoxBeamHelper::MakeData(NXBoxData* pData)
{
	mp_Data = pData;
	NXBoxBeamData* pBoxData = (NXBoxBeamData*)mp_Data;

	MSBeamG* pCompoG = (MSBeamG*)GetBuilding()->CreateNewCompoG(MSCompoG::msBeamG);
	pCompoG->mp_CompoM = FindCompoM(pBoxData->GetName(), MSCompoM::msBeamM);
	if (pCompoG->mp_CompoM == nullptr)
		pCompoG->mp_CompoM = CreateNewBeamM(pBoxData->GetWidth(), MakeDefaultName());
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

MSCompoM* NXBoxBeamHelper::CreateNewBeamM(double dB, CString TheName)
{
	CString defName;
	double dH = 700;
	defName.Format(_T("%s(%ld x %ld)"), TheName, (long)dB, (long)dH);

	CString msg;
	msg.Format(_T("%s %s"), TheName, WARNING_NAME_NONE);
	CreateErrorObject(mp_Data->GetFigureID(), W_CODE_NAME_NONE, WARNING_LEVEL, mp_Reader->GetGroupName(), msg);
	return mp_Reader->CreateNewBeamM(dB, defName);
}

bool NXBoxBeamHelper::CheckCompoM(MSCompoM* pCompoM)
{
	NXBoxBeamData * pBoxData = (NXBoxBeamData*)mp_Data;

	CString message;
	if (fabs(pCompoM->GetWidth() - pBoxData->GetWidth()) > 1.0)
	{
		message.Format(_T("%s - 단면정보(%ldx%ld)와 평면크기(%ldx%ld)가 다릅니다."), pCompoM->m_Name,
			(long)pCompoM->GetWidth(), (long)pCompoM->GetDepth(), (long)pBoxData->GetWidth(), (long)pCompoM->GetDepth());
		CreateErrorObject(pBoxData->GetFigureID(), W_CODE_SIZE_DIFF, ERROR_LEVEL, mp_Reader->GetGroupName(), message);
	}
	return m_IsMaked;
}
