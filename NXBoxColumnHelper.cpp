#include "stdafx.h"
#include "NXBoxColumnHelper.h"

#include "MSRebarBuilding.h"
#include "MSCompoM.h"
#include "MSColumnG.h"
#include "MSRcLineMemberM.h"

#include "CogReader.h"

///////////////////////////////// NXBoxColumnData ////////////////////////////////////////////////////////
NXBoxColumnData::NXBoxColumnData() : NXBoxData()
{
	m_Shape = 1;
	m_Width = 500;
	m_Depth = 600;
	m_TopOffset = 0;
	m_BotOffset = 0;
	m_Rotate = 0.0;
}

NXBoxColumnData::~NXBoxColumnData()
{

}

///////////////////////////////// NXBoxColumnHelper ////////////////////////////////////////////////////////
NXBoxColumnHelper::NXBoxColumnHelper(CogReader* pReader) : NXBoxHelper(pReader)
{
}


NXBoxColumnHelper::~NXBoxColumnHelper()
{
}

MSCompoG*	NXBoxColumnHelper::MakeData(NXBoxData* pData)
{
	mp_Data = pData;
	NXBoxColumnData* pBoxData = (NXBoxColumnData*)mp_Data;

	MSColumnG* pCompoG = (MSColumnG*)GetBuilding()->CreateNewCompoG(MSCompoG::msColumnG);
	pCompoG->mp_CompoM = FindCompoM(pBoxData->GetName(), MSCompoM::msColumnM);
	if (pCompoG->mp_CompoM == nullptr)
		pCompoG->mp_CompoM = CreateNewColumnM(pBoxData->GetWidth(), pBoxData->GetDepth(), pBoxData->GetShape(), MakeDefaultName());
	else
		CheckCompoM(pCompoG->mp_CompoM);

	pCompoG->ReplaceVec(0, new GM2DVector(pBoxData->GetCenterPoint().m_X, pBoxData->GetCenterPoint().m_Y));

	pCompoG->m_SOffsetZ = pBoxData->GetBotOffset();
	pCompoG->m_EOffsetZ = pBoxData->GetTopOffset();
	pCompoG->m_dAxisRotation = pBoxData->GetRotate();
	pCompoG->SetPointAlignType(COLUMNALIGN_CENTER);

	if (!m_IsMaked)
	{
		delete pCompoG;
		return nullptr;
	}

	return pCompoG;
}

MSCompoM* NXBoxColumnHelper::CreateNewColumnM(double dB, double dH, long nShape, CString TheName)
{
	CString defName;
	if(nShape == 1)
		defName.Format(_T("%s(%ld x %ld)"), TheName, (long)dB, (long)dH);
	else
		defName.Format(_T("%s(%ld)"), TheName, (long)dB);

	CString msg;
	msg.Format(_T("%s %s"), TheName, WARNING_NAME_NONE);
	CreateErrorObject(mp_Data->GetFigureID(), W_CODE_NAME_NONE, WARNING_LEVEL, mp_Reader->GetGroupName(), msg);
	return mp_Reader->CreateNewColumnM(dB, dH, nShape, defName);
}

bool NXBoxColumnHelper::CheckCompoM(MSCompoM* pCompoM)
{
	NXBoxColumnData* pBoxData = (NXBoxColumnData*)mp_Data;
	MSRcLineMemberM* pColumnM = (MSRcLineMemberM*)pCompoM;

	CString message;
	if (pBoxData->GetShape() != pColumnM->GetShape())
	{
		message.Format(_T("%s 부재의 형상이 단면과 다릅니다."));
		CreateErrorObject(pBoxData->GetFigureID(), W_CODE_SIZE_DIFF, ERROR_LEVEL, mp_Reader->GetGroupName(), message);
		m_IsMaked = false;
	}
	else
	{
		if (pBoxData->GetShape() == 1)
		{
			if (fabs(pColumnM->GetWidth() - pBoxData->GetWidth()) > 1.0 || fabs(pColumnM->GetDepth() - pBoxData->GetDepth()) > 1.0)
			{
				message.Format(_T("%s - 단면정보(%ldx%ld)와 평면크기(%ldx%ld)가 다릅니다."), pCompoM->m_Name,
					(long)pCompoM->GetWidth(), (long)pCompoM->GetDepth(), (long)pBoxData->GetWidth(), (long)pBoxData->GetDepth());
				CreateErrorObject(pBoxData->GetFigureID(), W_CODE_SIZE_DIFF, ERROR_LEVEL, mp_Reader->GetGroupName(), message);
			}
		}
		else if (pBoxData->GetShape() == 2)
		{
			if (fabs(pColumnM->GetWidth() - pBoxData->GetWidth()) > 1.0)
			{
				message.Format(_T("%s - 단면정보(%ld)와 평면크기(%ld)가 다릅니다."), pCompoM->m_Name, (long)pCompoM->GetWidth(), (long)pBoxData->GetWidth());
				CreateErrorObject(pBoxData->GetFigureID(), W_CODE_SIZE_DIFF, ERROR_LEVEL, mp_Reader->GetGroupName(), message);
			}
		}
	}
	return m_IsMaked;
}
