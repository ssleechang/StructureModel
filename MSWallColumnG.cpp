#include "StdAfx.h"
#include "MSWallColumnG.h"
#include "MSRcLineMemberM.h"
#include "MSRebarBuilding.h"
#include "MSFloor.h"
#include "MSPitG.h"
#include "MSIsoFootG.h"
#include "MSLinkMemberG.h"
#include "MSSlabG.h"
#include "MSRcColumnM.h"
#include "MDSrcColumnM.h"
#include "MSSrcColumnM.h"
#include "MSSteelColumnM.h"
#include "MSColumnHeaderM.h"
#include "MSCustomWallColumnM.h"

#include "MSSteelUtility.h"
#include "MSSteelSection.h"

#include <math.h>
IMPLEMENT_SERIAL(MSWallColumnG, MSVerticalLineMemberG, VERSIONABLE_SCHEMA | 1)
MSWallColumnG::MSWallColumnG()
{
	Init(true);
}

MSWallColumnG::~MSWallColumnG(void)
{

}

void MSWallColumnG::Dump(EFS::EFS_Buffer& buffer)
{
	MSVerticalLineMemberG::Dump(buffer);
}

void MSWallColumnG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSVerticalLineMemberG::Recovery(buffer);
}

void MSWallColumnG::Serialize(CArchive &ar)
{
	MSVerticalLineMemberG::Serialize(ar);
	bool bEachCompoM = false;
	if (ar.IsStoring())
	{
		if (mp_CompoM)
		{
			if (mp_CompoM->m_ID == 0)
				bEachCompoM = true;

			ar << bEachCompoM;
			if (bEachCompoM == true)
			{
				Serialize_EachCompoM(ar);
			}
		}
	}
	else
	{
		ar >> bEachCompoM;
		if (bEachCompoM)
		{
			Serialize_EachCompoM(ar);
		}
	}
}

void MSWallColumnG::Serialize_EachCompoM_Rebar(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ASSERT(0);
		CString strMsg, strErrMsg;
		strMsg = LocalDic::GetLocal(L"오류: 사용자 벽기둥 단면이 아닙니다.");
		strErrMsg.Format(strMsg + L" GID:", m_ID);
		StatusReporterWrapper::OutputError(strErrMsg);
	}
	else
	{
		ASSERT(0);
		CString strMsg, strErrMsg;
		strMsg = LocalDic::GetLocal(L"오류: 사용자 벽기둥 단면이 아닙니다.");
		strErrMsg.Format(strMsg + L" GID:", m_ID);
		StatusReporterWrapper::OutputError(strErrMsg);
	}
}

MSObject* MSWallColumnG::Copy(bool bNewID /*= true*/)
{
	MSWallColumnG* pNewObj;

	pNewObj = new MSWallColumnG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSWallColumnG::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;
	if (!pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() != msWallColumnG) { ASSERT(0); return; }

	MSVerticalLineMemberG::CopyFromMe(pSource_in, bNewID);
}

void MSWallColumnG::Init(bool bInConstructor/*=false*/)
{
	if (!bInConstructor)
		MSVerticalLineMemberG::Init(bInConstructor);
}

bool MSWallColumnG::CheckData()
{
	if (MSElement::CheckData() == false)
		return false;

	// Height Check
	if (!CheckHeight())
		return false;

	// 부재 중첩 체크 (각도 돌린 경우는 여기서 체크해야만 함)
	MSFloor* floor = GetFloor();
	if (floor != NULL && floor->CheckCompoGOverlap_WithUpDownFloor(this, this->m_ID))
		return true;
	//return false;

	// Check add...

	return true;
}