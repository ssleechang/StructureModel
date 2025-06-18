#include "StdAfx.h"
#include "MSLinkMemberG.h"

#include "MSContainer.h"
#include "GMLib/GM2DLineVector.h"
#include "GMLib/GM2DArc.h"
#include "MSColumnG.h"
#include "MSFloor.h"
#include "MSIsoFootG.h"
#include "Logger.h"
#include "MSFloorQT.h"
//#include "GMLib/MSVersionInfo.h"
#include "MSRebarBuilding.h"

#include "MSLineLevelDifG.h"
#include "MSMemberLevelDif.h"
#include "MSLevelZone.h"
#include "MSConstructionZone.h"
#include "GMLib/MSVersionInfo.h"

#include "MSConnectivityDummy.h"
#include "MSConnection.h"
#include "MSModelChecker.h"
#include "MSCompoGUtil.h"

#include "CmdProcess\CmdNewModifyVariable.h"
#include <stdint.h>

IMPLEMENT_SERIAL(MSLinkMemberG, MSCompoG, VERSIONABLE_SCHEMA | 1)

MSColineLinkMemberGs* MSLinkMemberG::MakeColineLinkMemberG(MSLinkMemberG* pFindLinkG, FindType_MSElement eType, map<long, MSLinkMemberG*>& UsedLinkMap)
{
	MSColineLinkMemberGs* pLink = nullptr;
	pLink = new MSColineLinkMemberGs();
	UsedLinkMap.insert(make_pair(pFindLinkG->m_ID, pFindLinkG));

	pLink->Add((MSLinkMemberG*)pFindLinkG);

	MSLinkMemberG* pNextLink = nullptr;

	vector<MSCompoG*> NextLinkArr = pFindLinkG->FindColinerLinkMember(CON_START, eType, 0, MSCheckModelOption::GetInstance()->m_dColinearDegreeTol, pFindLinkG->GetWidth() / 2, pFindLinkG->GetLength() * 0.2);
	if (NextLinkArr.size() > 0)
		pNextLink = (MSBeamG*)NextLinkArr[0];
	long nRval = pLink->Add((MSLinkMemberG*)pNextLink);
	if (nRval != 0)
		UsedLinkMap.insert(make_pair(pNextLink->m_ID, pNextLink));

	while (pNextLink != nullptr)
	{
		MSLinkMemberG* pSLink = nullptr;
		MSLinkMemberG* pELink = nullptr;

		vector<MSCompoG*> SNextLinkArr = pNextLink->FindColinerLinkMember(CON_START, eType, 0, MSCheckModelOption::GetInstance()->m_dColinearDegreeTol, pFindLinkG->GetWidth() / 2, pFindLinkG->GetLength() * 0.2);
		if (SNextLinkArr.size() > 0)
			pSLink = (MSLinkMemberG*)SNextLinkArr[0];

		vector<MSCompoG*> ENextLinkArr = pNextLink->FindColinerLinkMember(CON_END, eType, 0, MSCheckModelOption::GetInstance()->m_dColinearDegreeTol, pFindLinkG->GetWidth() / 2, pFindLinkG->GetLength() * 0.2);
		if (ENextLinkArr.size() > 0)
			pELink = (MSLinkMemberG*)ENextLinkArr[0];

		long nSRval = pLink->Add((MSLinkMemberG*)pSLink);
		long nERval = pLink->Add((MSLinkMemberG*)pELink);

		if (nSRval != 0)
		{
			UsedLinkMap.insert(make_pair(pSLink->m_ID, pSLink));
			pNextLink = pSLink;
		}
		if (nERval != 0)
		{
			UsedLinkMap.insert(make_pair(pELink->m_ID, pELink));
			pNextLink = pELink;
		}
		if (nSRval == 0 && nERval == 0)
			pNextLink = nullptr;

		if (nSRval != 0 && nERval != 0)
		{
			pNextLink = nullptr;
			ASSERT(0);
		}
	}

	NextLinkArr = pFindLinkG->FindColinerLinkMember(CON_END, eType, 0, MSCheckModelOption::GetInstance()->m_dColinearDegreeTol, pFindLinkG->GetWidth() / 2, pFindLinkG->GetLength() * 0.2);
	if (NextLinkArr.size() > 0)
		pNextLink = (MSLinkMemberG*)NextLinkArr[0];

	nRval = pLink->Add((MSLinkMemberG*)pNextLink);
	if (nRval != 0)
		UsedLinkMap.insert(make_pair(pNextLink->m_ID, pNextLink));

	while (pNextLink != nullptr)
	{
		MSLinkMemberG* pSLink = nullptr;
		MSLinkMemberG* pELink = nullptr;

		vector<MSCompoG*> SNextLinkArr = pNextLink->FindColinerLinkMember(CON_START, eType, 0, MSCheckModelOption::GetInstance()->m_dColinearDegreeTol, pFindLinkG->GetWidth() / 2, pFindLinkG->GetLength() * 0.2);
		if (SNextLinkArr.size() > 0)
			pSLink = (MSBeamG*)SNextLinkArr[0];

		vector<MSCompoG*> ENextLinkArr = pNextLink->FindColinerLinkMember(CON_END, eType, 0, MSCheckModelOption::GetInstance()->m_dColinearDegreeTol, pFindLinkG->GetWidth() / 2, pFindLinkG->GetLength() * 0.2);
		if (ENextLinkArr.size() > 0)
			pELink = (MSBeamG*)ENextLinkArr[0];

		long nSRval = pLink->Add((MSLinkMemberG*)pSLink);
		long nERval = pLink->Add((MSLinkMemberG*)pELink);

		if (nSRval != 0)
		{
			UsedLinkMap.insert(make_pair(pSLink->m_ID, pSLink));
			pNextLink = pSLink;
		}
		if (nERval != 0)
		{
			UsedLinkMap.insert(make_pair(pELink->m_ID, pELink));
			pNextLink = pELink;
		}
		if (nSRval == 0 && nERval == 0)
			pNextLink = nullptr;

		if (nSRval != 0 && nERval != 0)
		{
			ASSERT(0);
			pNextLink = nullptr;

		}
	}

	return pLink;
}

MSLinkMemberG::MSLinkMemberG()
{
	Init(true);
}

MSLinkMemberG::~MSLinkMemberG(void)
{
	//ClearConnectMembers();
	if(mp_Curve)		delete mp_Curve;
	if(mp_LLine)		delete mp_LLine;
	if(mp_RLine)		delete mp_RLine;
	if(mp_LLineTR)	delete mp_LLineTR;
	if(mp_RLineTR)	delete mp_RLineTR;
	DeleteMemberLevelDifG();
}

MSObject* MSLinkMemberG::Copy( bool bNewID /*= true*/ )
{
	MSLinkMemberG* pNewObj;

	pNewObj = new MSLinkMemberG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSLinkMemberG::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/ )
{
	if(pSource_in == nullptr) return;
	MSCompoG::CopyFromMe(pSource_in, bNewID);

	if( !pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() == msNULL) { ASSERT(0); return; }
	MSLinkMemberG* pSource = (MSLinkMemberG*)pSource_in;

	if(pSource)
	{
		delete mp_Curve;
		delete mp_LLine;
		delete mp_RLine;
		delete mp_LLineTR;
		delete mp_RLineTR;
		mp_Curve = nullptr;
		mp_LLine = nullptr;
		mp_RLine = nullptr;
		mp_LLineTR = nullptr;
		mp_RLineTR = nullptr;

		if(pSource->mp_Curve)
			mp_Curve =  pSource->mp_Curve->Copy();
		if(pSource->mp_LLine)
			mp_LLine = pSource->mp_LLine->Copy();
		if(pSource->mp_RLine)
			mp_RLine = pSource->mp_RLine->Copy();
		if(pSource->mp_LLineTR)
			mp_LLineTR = pSource->mp_LLineTR->Copy();
		if(pSource->mp_RLineTR)
			mp_RLineTR = pSource->mp_RLineTR->Copy();

		m_SOffsetZ = pSource->m_SOffsetZ;
		m_EOffsetZ = pSource->m_EOffsetZ;

		m_dWidthSetback = pSource->m_dWidthSetback;
		/*
		ma_MemberLevelDif.RemoveAll();
		for (int i = 0; i < pSource->ma_MemberLevelDif.GetSize(); ++i)
		{
			MSMemberLevelDif* diff = new MSMemberLevelDif();
			diff->CopyFromMe(pSource->ma_MemberLevelDif[i]);
			ma_MemberLevelDif.Add(diff);
		}
		*/
		
		m_eLineAlign = pSource->m_eLineAlign;
		/*
		long numMidJoint = pSource->ma_MidJoint.size();
		for(long iMid = 0;iMid<numMidJoint;iMid++)
			ma_MidJoint.push_back(pSource->ma_MidJoint[iMid]);
			*/
// 		DeleteMemPartAll();
// 		long numMemPart = pSource->ma_MemPart.size();
// 		for(long iMemPart = 0;iMemPart<numMemPart;iMemPart++)
// 		{
// 			MSLinkMemPart* pMemPart = (MSLinkMemPart*)pSource->ma_MemPart[iMemPart]->Copy();
// 			ma_MemPart.push_back(pMemPart);
// 		}
	}
}

void MSLinkMemberG::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSCompoG::Init(bInConstructor);

	ma_Vec.push_back(new GM2DVector());
	ma_Vec.push_back(new GM2DVector());

	ma_Vec_Input.push_back(new GM2DVector());
	ma_Vec_Input.push_back(new GM2DVector());
	// 위 ma_vec, ma_Vec_Input 상태 InvalidVec() 이다.

	m_SOffsetZ = 0.;
	m_EOffsetZ = 0.;
	mp_LLine = nullptr;
	mp_RLine = nullptr;
	mp_LLineTR = nullptr;
	mp_RLineTR = nullptr;
	mp_Curve = nullptr;
	m_isBuriedLinkMember = false;
	m_hasDelPart = false;
	m_eLineAlign = LINEALIGN_CENTER;
	m_dWidthSetback = 0;
	InitMemPart();
}

bool MSLinkMemberG::IsInvalidVecInput()
{
	if (ma_Vec_Input.size() != 2)
		return true;
	if (ma_Vec_Input[0]->SameVector4DTOL(*ma_Vec_Input[1]))
		return true;

	return false;
}

void MSLinkMemberG::InitMemPart()
{
	
}


//bool MSLinkMemberG::SetJoints( long SJointID, long EJointID )
//{
//	if(mp_FloorG==nullptr)
//		return FALSE;
//
//	GetSConn() = mp_FloorG->GetJoint(SJointID);
//	if(GetSConn() == nullptr)
//		return false;
//
//	GetEConn() = mp_FloorG->GetJoint(EJointID);
//	if(GetEConn() == nullptr)
//		return false;
//
//	return true;
//}
GM2DLineVector MSLinkMemberG::GetCenLVec()
{
	double dWidth = GetWidth();
	GM2DLineVector CenLine(*ma_Vec[0],*ma_Vec[1]);
	
 	return CenLine;

}

void MSLinkMemberG::SetTrimmedLines()
{
	shared_ptr<GM2DCurve> pLCurve, pRCurve;
	GetTrimmedLRLines(pLCurve, pRCurve);

	if(mp_LLineTR != nullptr)
		delete mp_LLineTR;
	mp_LLineTR = pLCurve->Copy();

	if(mp_RLineTR != nullptr)
		delete mp_RLineTR;
	mp_RLineTR = pRCurve->Copy();

	if (isArc())
		FixTrimmedLine();
}

void MSLinkMemberG::SetLines()
{
	// 입력 라인 기준으로 LR라인 생성
	// => Center Line 기준으로 생성
	double dWidth = GetWidth();
	if (dWidth <= 0)
	{
		dWidth = 5;
		CString msg;
		msg.Format(L"(ID:%d) SetLines시 라인부재의 두께가 0보다 작습니다.", m_ID);
		LoggerWrapper::Log(GetGroupName() + msg);
		ASSERT(0);

	}

	// User 입력점 계산
	UpdateCurveInputByCenCurve();

	shared_ptr<GM2DCurve> pCurveCenter = GetCenCurve();

	if(mp_LLine) delete mp_LLine;
	if(mp_RLine) delete mp_RLine;

	mp_LLine = pCurveCenter->Copy();
	mp_RLine = pCurveCenter->Copy();

	/*
	if(m_eLineAlign == LINEALIGN_CENTER)
	{
		mp_LLine->NormalTranslateMe(dWidth/2);
		mp_RLine->NormalTranslateMe(-dWidth/2);
	}
	else if(m_eLineAlign == LINEALIGN_LEFT)
	{
		mp_RLine->NormalTranslateMe(-dWidth);
	}
	else if(m_eLineAlign == LINEALIGN_RIGHT)
	{
		mp_LLine->NormalTranslateMe(dWidth);
	}
	*/

	mp_LLine->NormalTranslateMe(dWidth/2);
	mp_RLine->NormalTranslateMe(-dWidth/2);

	if (mp_LLineTR != nullptr)
	{
		delete mp_LLineTR;
		mp_LLineTR = nullptr;
	}
	if (mp_RLineTR != nullptr)
	{
		delete mp_RLineTR;
		mp_RLineTR = nullptr;
	}	
	
	m_DoUpdateTrimmedProfile = true;
	/*
	// 센터라인기준으로 LR라인 생성
	double dWidth = GetWidth();
	if(isArc() == false)
	{
		GM2DLineVector LLVec = GetCenLVec().NormalTranslate(dWidth/2.);
		if(mp_LLine) delete mp_LLine;
		if(mp_RLine) delete mp_RLine;

		if(IS_USE_NATIVE_CMD)
			CmdNewModifyVariable<GM2DCurve*>::Set(&mp_LLine, new GM2DLineVector(LLVec));
		else
			mp_LLine = new GM2DLineVector(LLVec);
		GM2DLineVector RLVec = GetCenLVec().NormalTranslate(-dWidth/2.);
		if(IS_USE_NATIVE_CMD)
			CmdNewModifyVariable<GM2DCurve*>::Set(&mp_RLine, new GM2DLineVector(RLVec));
		else
			mp_RLine = new GM2DLineVector(RLVec);//by LSS

	}
	else
	{
		GM2DArc* pArc = (GM2DArc*)mp_Curve;
		mp_Curve->SetP0(pArc->GetStart2DVec());
		mp_Curve->SetP1(pArc->GetEnd2DVec());

		GMVector CenVec = pArc->m_Center;
		double dSAngle = pArc->m_Theta1;
		double dEAngle = pArc->m_Theta2;
		double Radius = pArc->m_A;
		GM2DArc* LArc = new GM2DArc(Radius+dWidth/2, CenVec.m_X, CenVec.m_Y, dSAngle, dEAngle);
        if(mp_RLine) delete mp_RLine;
		if(IS_USE_NATIVE_CMD)
		{
			LArc->SetP0(LArc->GetStart2DVec());
			LArc->SetP1(LArc->GetEnd2DVec());
			CmdNewModifyVariable<GM2DCurve*>::Set(&mp_RLine, LArc);
		}
		else
		{
			mp_RLine = LArc;
			mp_RLine->SetP0(LArc->GetStart2DVec());
			mp_RLine->SetP1(LArc->GetEnd2DVec());
		}
		GM2DArc* RArc = new GM2DArc(Radius-dWidth/2, CenVec.m_X, CenVec.m_Y, dSAngle, dEAngle);
        if(mp_LLine) delete mp_LLine;
		if(IS_USE_NATIVE_CMD)
		{
			RArc->SetP0(RArc->GetStart2DVec());
			RArc->SetP1(RArc->GetEnd2DVec());
			CmdNewModifyVariable<GM2DCurve*>::Set(&mp_LLine, RArc);
		}
		else
		{
			mp_LLine = RArc;
			mp_LLine->SetP0(RArc->GetStart2DVec());
			mp_LLine->SetP1(RArc->GetEnd2DVec());
		}
	}

	if (mp_LLineTR != nullptr)
	{
		delete mp_LLineTR;
		mp_LLineTR = nullptr;
	}
	if (mp_RLineTR != nullptr)
	{
		delete mp_RLineTR;
		mp_RLineTR = nullptr;
	}
	*/
}



void MSLinkMemberG::Dump(EFS::EFS_Buffer& buffer)
{
	MSCompoG::Dump(buffer);
	buffer << m_SOffsetZ << m_EOffsetZ;
	GM2DCurve::Dump(buffer, mp_Curve);
	GM2DCurve::Dump(buffer, mp_LLine);
	GM2DCurve::Dump(buffer, mp_RLine);
	GM2DCurve::Dump(buffer, mp_LLineTR);
	GM2DCurve::Dump(buffer, mp_RLineTR);

	int count = ma_MemberLevelDif.GetSize();
	buffer << count;
	for (int i = 0; i<count; ++i)
		ma_MemberLevelDif[i]->Dump(buffer);

	buffer << m_eLineAlign;
	buffer << m_dWidthSetback;
}

void MSLinkMemberG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSCompoG::Recovery(buffer);
	buffer >> m_SOffsetZ >> m_EOffsetZ;
	GM2DCurve::Recovery(buffer, mp_Curve);
	GM2DCurve::Recovery(buffer, mp_LLine);
	GM2DCurve::Recovery(buffer, mp_RLine);
	GM2DCurve::Recovery(buffer, mp_LLineTR);
	GM2DCurve::Recovery(buffer, mp_RLineTR);

	int count = 0;
	buffer >> count;
	ma_MemberLevelDif.RemoveAll();
	for (int i = 0; i < count; ++i)
	{
		MSMemberLevelDif* diff = new MSMemberLevelDif();
		diff->Recovery(buffer);
		ma_MemberLevelDif.Add(diff);
	}

	buffer >> m_eLineAlign;
	buffer >> m_dWidthSetback;
}

void MSLinkMemberG::Serialize(CArchive &ar)
{
	MSCompoG::Serialize(ar);
	int nZero = 0;
	if(ar.IsStoring())
	{
		ar << m_SOffsetZ;
		ar << m_EOffsetZ;

		if(mp_Curve)
		{
			ar << mp_Curve->m_ObjType;
			mp_Curve->Serialize(ar);
		}
		else
			ar << nZero;

		// JOINT_SERIALIZE
		{
			long ID = 0;
// 			int count = ma_MidJoint.size();
// 			ar << count;
// 			for (int i=0 ; i<count ; ++i)
// 			{
// 				ID = (ma_MidJoint[i] == nullptr ? 0 : ma_MidJoint[i]->m_ID);
// 				ar << ID;
// 			}

			GM2DCurve::SerializeWithNullFlag(ar, mp_LLine);
			GM2DCurve::SerializeWithNullFlag(ar, mp_RLine);

			GM2DCurve::SerializeWithNullFlag(ar, mp_LLineTR);
			GM2DCurve::SerializeWithNullFlag(ar, mp_RLineTR);

// 			int count = ma_MemPart.size();
// 			ar << count;
// 			for (int i=0 ; i<count ; ++i)
// 				ma_MemPart[i]->Serialize(ar);

//			ar << m_hasDelPart;
			ar << m_isBuriedLinkMember;
			
			int count = ma_MemberLevelDif.GetSize();
			ar << count;
			for (int i=0 ; i<count ; ++i)
				ma_MemberLevelDif[i]->Serialize(ar);
			ar << (int)m_eLineAlign;

			ar << m_dWidthSetback;
		}
	}
	else
	{
		int val = 0;

		ar >> m_SOffsetZ;
		ar >> m_EOffsetZ;

		int nObjType=0;
		ar >> nObjType;
		if (nObjType == 0) {}
		else if(nObjType==GT_ARC)
		{
			mp_Curve = new GM2DArc();
			mp_Curve->Serialize(ar);
		}
		else if(nObjType==GT_LINE)
		{
			mp_Curve = new GM2DLineVector();
			mp_Curve->Serialize(ar);
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20141003)
		{
			long ID = 0;
			int count = 0;
			if (MSConnectivityDummy::JOINT_SERIALIZE())
			{

				if(MSVersionInfo::GetCurrentVersion() < 20150107)
				{
					ar >> count;
					//ma_MidJoint.clear();
					for (int i=0 ; i<count ; ++i)
					{
						ar >> ID;
						//ma_MidJoint.push_back((MSConnectivityDummy*)ID);
					}
					GM2DCurve::SerializeWithNullFlag(ar, mp_LLine);
					GM2DCurve::SerializeWithNullFlag(ar, mp_RLine);
				}
				if(MSVersionInfo::GetCurrentVersion() >= 20150108)
				{
					GM2DCurve::SerializeWithNullFlag(ar, mp_LLineTR);
					GM2DCurve::SerializeWithNullFlag(ar, mp_RLineTR);
				}

				if(MSVersionInfo::GetCurrentVersion() < 20150113)
				{
					ar >> count;
				//ma_MemPart.clear();
				
				
					for (int i=0 ; i<count ; ++i)
					{
						MSLinkMemPartDummy* part = new MSLinkMemPartDummy();
						part->Serialize(ar);
						delete part;
						//ma_MemPart.push_back(part);
					}

					ar >> m_hasDelPart;
				}
				ar >> m_isBuriedLinkMember;
				

				if(MSVersionInfo::GetCurrentVersion() >= 20141006)
				{
					int count = 0;
					ar >> count;
					for (int i=0 ; i<count ; ++i)
					{
						MSMemberLevelDif* diff = new MSMemberLevelDif();
						diff->Serialize(ar);
						ma_MemberLevelDif.Add(diff);
						//delete diff;
					}
				}
			}
			else
			{
				// MidJoint ID
				if(MSVersionInfo::GetCurrentVersion() < 20150107)
				{
					ar >> count;
					for (int i=0 ; i<count ; ++i)
					{
						ar >> ID;
					}
				}

				GM2DCurve* dummy = nullptr;
				GM2DCurve::SerializeWithNullFlag(ar, mp_LLine);
				GM2DCurve::SerializeWithNullFlag(ar, mp_RLine);
				if(MSVersionInfo::GetCurrentVersion() >= 20150108)
				{
					GM2DCurve::SerializeWithNullFlag(ar, mp_LLineTR);
					GM2DCurve::SerializeWithNullFlag(ar, mp_RLineTR);
				}
				
				if(MSVersionInfo::GetCurrentVersion() < 20150113)
				{
					ar >> count;
					for (int i=0 ; i<count ; ++i)
					{
						MSLinkMemPartDummy part_tmp;
						part_tmp.Serialize(ar);
					}
					bool hasDelPart;
					ar >> hasDelPart;
				}
				bool isBuriedLinkMember;
				ar >> isBuriedLinkMember;
				
				if(MSVersionInfo::GetCurrentVersion() >= 20141006)
				{
					int count = 0;
					ar >> count;
					for (int i=0 ; i<count ; ++i)
					{
						MSMemberLevelDif* diff = new MSMemberLevelDif();
						diff->Serialize(ar);
						ma_MemberLevelDif.Add(diff);
						//delete diff;
					}
				}
			}
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20150430)
		{
			int eLineAlign;
			ar >> eLineAlign;
			m_eLineAlign = (eLINEALIGN_TYPE)eLineAlign;
		}
		else
			m_eLineAlign = eLINEALIGN_TYPE::LINEALIGN_CENTER;

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20180906)
		{
			double dThickSetback = 0;
			ar >> m_dWidthSetback;
		}

		if (isArc())
			FixTrimmedLine();
	}
}

void MSLinkMemberG::FixTrimmedLine()
{
	// GM2DArc->MakeCCW()가 작동하지 않아서 mp_LLine과 mp_RLine이 반시계방향이라고 가정하고 계산한다.
	if (mp_LLineTR != nullptr)
	{
		GM2DVector lTRVec = mp_LLineTR->PointAtParam(0.5);
		if (mp_LLine->HasOnLine(lTRVec, DTOL_GM) == false)
		{
			GM2DArc* pLArc = dynamic_cast<GM2DArc*>(mp_LLineTR);
			if (pLArc != nullptr)
			{
				pLArc->FixTheta(pLArc->m_Theta1);
				pLArc->FixTheta(pLArc->m_Theta2);

				swap(pLArc->m_Theta1, pLArc->m_Theta2);
				pLArc->m_P0 = pLArc->GetStart2DVec();
				pLArc->m_P1 = pLArc->GetEnd2DVec();
			}
		}
	}

	if (mp_RLineTR != nullptr)
	{
		GM2DVector rTRVec = mp_RLineTR->PointAtParam(0.5);
		if (mp_RLine->HasOnLine(rTRVec, DTOL_GM) == false)
		{
			GM2DArc* pRArc = dynamic_cast<GM2DArc*>(mp_RLineTR);
			if (pRArc != nullptr)
			{
				pRArc->FixTheta(pRArc->m_Theta1);
				pRArc->FixTheta(pRArc->m_Theta2);

				swap(pRArc->m_Theta1, pRArc->m_Theta2);
				pRArc->m_P0 = pRArc->GetStart2DVec();
				pRArc->m_P1 = pRArc->GetEnd2DVec();
			}
		}
	}
}

double MSLinkMemberG::GetCompoMFloorLevelOffSet()
{
	MSCompoM* pCompoM = GetCompoM();
	double dFloorLevelOffSet = 0;
	if(pCompoM == nullptr)
		return dFloorLevelOffSet;

	MSCustomM* pMSCustomM = dynamic_cast<MSCustomM*>(pCompoM);
	if (pMSCustomM != nullptr)
		dFloorLevelOffSet = pMSCustomM->GetFloorLevelOffSet();
	
	return dFloorLevelOffSet;
}


double MSLinkMemberG::GetRelativeStartLevel()
{
	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;
	MSFloor* pFloor = (MSFloor*)mp_Owner;
	//MSFloor* pNextFloor = GetBuilding()->GetFloor(pFloor->m_nFloorNum+1);

	return pFloor->GetLevel4LevelZone(ma_Vec[0]->m_X, ma_Vec[0]->m_Y)+m_SOffsetZ;
}
double MSLinkMemberG::GetRelativeEndLevel()
{
	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;
	MSFloor* pFloor = (MSFloor*)mp_Owner;
	//MSFloor* pNextFloor = GetBuilding()->GetFloor(pFloor->m_nFloorNum+1);

	return pFloor->GetLevel4LevelZone(ma_Vec[1]->m_X, ma_Vec[1]->m_Y)+m_EOffsetZ;
}

double MSLinkMemberG::GetRelativeLevelbyLoc(double dLocVal)
{
	MSFloor* pFloor = (MSFloor*)mp_Owner;
	if(pFloor == nullptr)
		return 0;

	shared_ptr<GM2DCurve> pCenCurve = GetCenCurve();
	GM2DVector TheVec = pCenCurve->PointAtParam(dLocVal);

	double dOffSet = GetOffSetbyLoc(dLocVal);
	double dZLevel = pFloor->GetLevel4LevelZone(TheVec.m_X, TheVec.m_Y)+ dOffSet;

	return dZLevel;
}

/*
double MSLinkMemberG::GetOffSetbyLoc(double dLoc)
{
	double dOffSet = 0;

	if(dLoc <= 0)
		return m_SOffsetZ;
	else if(dLoc >= 1)
		return m_EOffsetZ;
	else
	{
		double dLenH = fabs(m_SOffsetZ - m_EOffsetZ);
		double dTmp = dLenH* dLoc;
		dOffSet = max(m_SOffsetZ, m_EOffsetZ) - dTmp;
	}

	return dOffSet;
}


bool MSLinkMemberG::GetZLevelsbyLoc(double& TopLevel, double& BottomLevel, double dLoc)
{
	double dOffSet = GetOffSetbyLoc(dLoc);
	double dLevelZoneZ = 0;

	if(mp_Owner && mp_Owner->IsFloor())
	{
		MSFloor* pCurFloor = (MSFloor*)mp_Owner;
		GM2DVector TheVec = nullptr;
		GM2DCurve* pCenCurve = GetCenLineVec();
		if(dLoc <= 0)
			TheVec = pCenCurve->PointAtParam(0);
		else if(dLoc >= 1)
			TheVec = pCenCurve->PointAtParam(1);
		else
			TheVec = pCenCurve->PointAtParam(dLoc);

		delete pCenCurve;

		if(GetType() == MSElement::msLineLevelDifG)
			dLevelZoneZ = pCurFloor->GetLevel4LevelZone(TheVec.m_X, TheVec.m_Y);
		else
		{
			long NumDif = ma_MemberLevelDif.GetSize();
			if(NumDif > 0)
				dLevelZoneZ = pCurFloor->GetLevel4LevelZone(TheVec.m_X, TheVec.m_Y);
			else // 레벨존 안에 완전히 포함 또는 레벨존에 포함 되지 않음
				dLevelZoneZ = GetFloor()->GetLevel4LevelZone(this);
		}

		TopLevel = GetFloorLevel()+((MSFloor*)mp_Owner)->GetHeight() + dOffSet + dLevelZoneZ;
	}
	else
		TopLevel = GetFloorLevel()+dOffSet;
	BottomLevel = TopLevel-GetDepth();
	return true;
}*/

bool MSLinkMemberG::HasParallelLinkMember(bool CheckSameMemberType)
{
	bool hasParalle = false;
	for(long iConn = 0 ; iConn < ma_Connections.size() ; iConn++)
	{
		MSConnection* pConn = ma_Connections[iConn];
		if(CheckSameMemberType == true && (pConn->GetFirstCompoG()->GetType() != pConn->GetSecondCompoG()->GetType()) )
			continue;
		if(pConn->IsParallel())
		{
			hasParalle = true;
			break;
		}
	}

	return hasParalle;
}

GM2DCurve* MSLinkMemberG::GetLongLVec(bool bExtendToOrthoPart)
{
// 	shared_ptr<GM2DCurve> pLCurve, pRCurve;
// 	GetTrimmedLRLines(pLCurve, pRCurve);
	GM2DCurve* pLCurve = mp_LLineTR;
	GM2DCurve* pRCurve = mp_RLineTR;

	if(pLCurve == nullptr || pRCurve == nullptr)
		return nullptr;

	GM2DCurve* pLongCurve = nullptr;
	if(isArc())
	{
		pLongCurve = mp_Curve->Copy();
		double LLoc = mp_Curve->OrthoBaseLocValue(pLCurve->GetP0());
		double RLoc = mp_Curve->OrthoBaseLocValue(pRCurve->GetP0());
		pLongCurve->SetP0(mp_Curve->PointAtParam(min(LLoc,RLoc)));
		LLoc = mp_Curve->OrthoBaseLocValue(pLCurve->GetP1());
		RLoc = mp_Curve->OrthoBaseLocValue(pRCurve->GetP1());
		pLongCurve->SetP1(mp_Curve->PointAtParam(max(LLoc,RLoc)));
	}
	else
	{
		pLongCurve = new GM2DLineVector();
		GM2DLineVector TheLine = GetCenLVec();
		if(bExtendToOrthoPart == true/* && HasParallelLinkMember() == false*/)
		{
			GM2DLineVector SLine = GetEndLine4Joint2_ExtendToOrthoPart(0, FALSE);
			GM2DLineVector ELine = GetEndLine4Joint2_ExtendToOrthoPart(1, FALSE);
			pLongCurve->m_P0 = TheLine.OrthoBaseVector(SLine.m_P0);
			pLongCurve->m_P1 = TheLine.OrthoBaseVector(ELine.m_P0);
		}
		else
		{
			double LLoc = TheLine.OrthoBaseLocValue(pLCurve->GetP0());
			double RLoc = TheLine.OrthoBaseLocValue(pRCurve->GetP0());
			pLongCurve->SetP0(TheLine.PointAtParam(min(LLoc,RLoc)));
			LLoc = TheLine.OrthoBaseLocValue(pLCurve->GetP1());
			RLoc = TheLine.OrthoBaseLocValue(pRCurve->GetP1());
			pLongCurve->SetP1(TheLine.PointAtParam(max(LLoc,RLoc)));
		}
		double dDeg = TheLine.GetDegree();	
		double dCurveDeg = pLongCurve->GetDegree();
		if(fabs(dDeg - dCurveDeg) > DTOL_GM)
			pLongCurve->ReverseDirection();
	}

	return pLongCurve;
}
GM2DCurve* MSLinkMemberG::GetShortLVec()
{
// 	shared_ptr<GM2DCurve> pLCurve, pRCurve;
// 	GetTrimmedLRLines(pLCurve, pRCurve);
	GM2DCurve* pLCurve = mp_LLineTR;
	GM2DCurve* pRCurve = mp_RLineTR;

	if(pLCurve == nullptr || pRCurve == nullptr)
		return nullptr;

	GM2DCurve* pLongCurve = nullptr;
	if(isArc())
	{
		pLongCurve = mp_Curve->Copy();
		double SLLoc = mp_Curve->OrthoBaseLocValue(pLCurve->GetP0());
		double SRLoc = mp_Curve->OrthoBaseLocValue(pRCurve->GetP0());
		double ELLoc = mp_Curve->OrthoBaseLocValue(pLCurve->GetP1());
		double ERLoc = mp_Curve->OrthoBaseLocValue(pRCurve->GetP1());
		
		double dSLoc = max(SLLoc, SRLoc);
		double dELoc = min(ELLoc, ERLoc);

		pLongCurve->SetP0(mp_Curve->PointAtParam(min(dSLoc, dELoc)));
		pLongCurve->SetP1(mp_Curve->PointAtParam(max(dSLoc, dELoc)));
	}
	else
	{
		pLongCurve = new GM2DLineVector();
		GM2DLineVector LVec = GetCenLVec();
		double LLoc = LVec.OrthoBaseLocValue(pLCurve->GetP0());
		double RLoc = LVec.OrthoBaseLocValue(pRCurve->GetP0());
		pLongCurve->SetP0(LVec.PointAtParam(max(LLoc,RLoc)));
		LLoc = LVec.OrthoBaseLocValue(pLCurve->GetP1());
		RLoc = LVec.OrthoBaseLocValue(pRCurve->GetP1());
		pLongCurve->SetP1(LVec.PointAtParam(min(LLoc,RLoc)));
		double dDeg = LVec.GetDegree();
		double dCurveDeg = pLongCurve->GetDegree();
		if(fabs(dDeg - dCurveDeg) > DTOL_GM)
			pLongCurve->ReverseDirection();
	}


	return pLongCurve;
}
//void MSLinkMemberG::GetEccJoint( GM2DVector& SJoint, GM2DVector& EJoint )
//{
//	// ckkim1308
//	SJoint = m_SVec;
//	EJoint = m_EVec;
////	if(mp_SJoint)
////	{
////		SJoint.m_X = mp_SJoint->m_dXc + m_SEcc.m_X;
////		SJoint.m_Y = mp_SJoint->m_dYc + m_SEcc.m_Y;
////	}
////	if(mp_EJoint)
////	{
////		EJoint.m_X = mp_EJoint->m_dXc + m_EEcc.m_X;
////		EJoint.m_Y = mp_EJoint->m_dYc + m_EEcc.m_Y;
////	}
//
//}
//

// GM2DCurve* MSLinkMemberG::GetCenLineVec()
// {
// 	GM2DCurve* pCenCurve = nullptr;
// 	if(isArc() == false)
// 	{
// 		pCenCurve = new GM2DLineVector(*GetSVec(), *GetEVec());
// 
// 	}
// 	else
// 		pCenCurve = mp_Curve->Copy();
// 	// LevelDif 비율은 모델링된 시점의 센터라인으로 생성됨 LLine,RLine중 긴선으로 판단하면 안된다.
// 		//pCenCurve = GetLongLVec();
// 	return pCenCurve;
// }

shared_ptr<GM2DCurve> MSLinkMemberG::GetCenCurve()
{
	shared_ptr<GM2DCurve> CenCurve = nullptr;
	if(isArc() == false)
	{
		if(ma_Vec.size() < 2)
			return CenCurve;
		CenCurve = shared_ptr<GM2DCurve>(new GM2DLineVector(*ma_Vec[0], *ma_Vec[1]));
	}
	else
		CenCurve = shared_ptr<GM2DCurve>(mp_Curve->Copy());
	// LevelDif 비율은 모델링된 시점의 센터라인으로 생성됨 LLine,RLine중 긴선으로 판단하면 안된다.
	//pCenCurve = GetLongLVec();
	return CenCurve;
}

double MSLinkMemberG::GetLength()
{
	if(isArc())
		return mp_Curve->Length();
	else
		return GetSVec()->Distance(*GetEVec());
}

bool MSLinkMemberG::HitTestWithVector(GM2DVector& TheVec, double DistTol, bool bOnlyInLine)
{
	long Side2LLine = mp_LLine->WhichSide(TheVec, DistTol);
	long Side2RLine = mp_RLine->WhichSide(TheVec, DistTol);

	if(Side2RLine*Side2LLine == 1)
		return false;

    if(!bOnlyInLine) return true; // jissi / Arc 일 때 만들기

	double LocValue = mp_LLine->OrthoBaseLocValue(TheVec);
	if(LocValue<0. || LocValue>1.)
		return false;
	LocValue = mp_RLine->OrthoBaseLocValue(TheVec);
	if(LocValue<0. || LocValue>1.)
		return false;
	return true;
}

bool MSLinkMemberG::VisibleInRect( GM2DRectangle* pRect)
{
	GM2DCurve* pCenCurve = nullptr;
	GM2DLineVector CenLine;
	if(isArc() == false)
	{
		CenLine =GetCenLVec();
		pCenCurve = &CenLine;
	}
	else
		pCenCurve = mp_Curve;
	
	if(pCenCurve->VisibleInRect(*pRect))
		return true;
	return false;
/*
	if(mp_LLine->VisibleInRect(*pRect))
		return TRUE;
	if(mp_RLine->VisibleInRect(*pRect))
		return TRUE;

	long NumPart = ma_MemPart.size();
	for(long iPart = 0 ; iPart < NumPart ; iPart++)
	{
		MSLinkMemPart* pPart = ma_MemPart[iPart];
		if(pPart->mp_LLine != nullptr && pPart->mp_LLine->VisibleInRect(*pRect))
			return TRUE;
		if(pPart->mp_RLine != nullptr && pPart->mp_RLine->VisibleInRect(*pRect))
			return TRUE;
	}

	return FALSE;
*/
}


GM2DCurve* MSLinkMemberG::FindFarLine( vector<GM2DCurve*> vCurves, long StartOrEnd, GM2DCurve* pCenCurve )
{
	GM2DCurve* ret = nullptr;

	if(StartOrEnd==0)
	{
		double locMin = 0.0;
		for (int i=0 ; i<vCurves.size() ; ++i)
		{
			GM2DCurve* pCurve = vCurves[i];
			double loc = pCenCurve->OrthoBaseLocValue(pCurve->GetP0());
			if (i==0 || loc < locMin) { locMin = loc; ret = pCurve; }
		}
	}
	else
	{
		double locMax = 0.0;
		for (int i=0 ; i<vCurves.size() ; ++i)
		{
			GM2DCurve* pCurve = vCurves[i];
			double loc = pCenCurve->OrthoBaseLocValue(pCurve->GetP0());
			if (i==0 || loc > locMax) { locMax = loc; ret = pCurve; }
		}
	}

	return ret;
}

GM2DLineVector MSLinkMemberG::GetStartEndLine()
{
	GM2DLineVector StartEndLine(mp_LLine->m_P0, mp_RLine->m_P0);
	return StartEndLine;
}

GM2DLineVector MSLinkMemberG::GetEndEndLine()
{
	GM2DLineVector EndEndLine(mp_LLine->m_P1, mp_RLine->m_P1);
	return EndEndLine;
}

GM2DLineVector MSLinkMemberG::GetEndLineByParam(double param)
{
	GM2DVector lPnt = mp_LLine->PointAtParam(param);
	GM2DVector rPnt = mp_RLine->PointAtParam(param);

	GM2DLineVector EndEndLine(rPnt, lPnt);
	return EndEndLine;
}


GM2DLineVector MSLinkMemberG::GetEndLine4Joint2_ExtendToOrthoPart( long StartOrEnd, BOOL bShort /*= TRUE*/ )
{
	GM2DLineVector EndLine;
	shared_ptr<GM2DCurve> pCenCurve = GetCenCurve();
// 	shared_ptr<GM2DCurve> pLCurve, pRCurve;
// 	GetTrimmedLRLines(pLCurve, pRCurve);
	GM2DCurve* pLCurve = mp_LLineTR;
	GM2DCurve* pRCurve = mp_RLineTR;

	GM2DVector LVec, RVec;
	double LLoc,RLoc;
	CON_POS eConPos = CON_UNKNOWN;
	if(StartOrEnd == 0)
		eConPos = CON_START;
	else if(StartOrEnd == 1)
		eConPos = CON_END;

    if(isArc() == false && (pLCurve != nullptr && pRCurve != nullptr))
    {
		//vector<MSLinkMemPart*> rParts;
		vector<GM2DCurve*> vCurve;
		vector<MSConnection*> ConnArr = GetConnectionByPos(eConPos);
		bool bHasPointMember = false;
		for(long iConn = 0 ; iConn < ConnArr.size() ; iConn++)
		{
			MSConnection* pConn = ConnArr[iConn];
			double dInnerDeg = pConn->GetInnerDeg();
			if ( fabs(dInnerDeg - 90.0) < 1.0 || fabs(dInnerDeg - 270.0) < 1.0)
			{
				MSLinkMemberG* pUrLinkG = (MSLinkMemberG*)pConn->GetPartner(this);
				if(pUrLinkG == nullptr || GetType() != pUrLinkG->GetType())
					continue;
				vCurve.push_back(pUrLinkG->mp_LLine);
				vCurve.push_back(pUrLinkG->mp_RLine);
			}
			MSCompoG* pCompoG = pConn->GetPartner(this);
			if(pCompoG == nullptr) continue;
			if(bHasPointMember == false && pCompoG->IsPointMember())
				bHasPointMember = true;
		}

		GM2DCurve* farLine = FindFarLine(vCurve, StartOrEnd, &*pCenCurve );
		if (farLine != nullptr && bHasPointMember == false)
		{
			if(StartOrEnd==0)
			{
				EndLine.m_P0 = farLine->OrthoBaseVector(pLCurve->GetP0());
				EndLine.m_P1 = farLine->OrthoBaseVector(pRCurve->GetP0());
			}
			else
			{
				EndLine.m_P0 = farLine->OrthoBaseVector(pLCurve->GetP1());
				EndLine.m_P1 = farLine->OrthoBaseVector(pRCurve->GetP1());
			}

			return EndLine; ////////////////////////////////////////////// return in this CASE
		}

        if(StartOrEnd==0)
        {
            LLoc = pCenCurve->OrthoBaseLocValue(pLCurve->GetP0());
            RLoc = pCenCurve->OrthoBaseLocValue(pRCurve->GetP0());

			
			if((LLoc>RLoc && bShort) || (LLoc<RLoc && !bShort))
            {
                EndLine.m_P0 = pLCurve->GetP0();
                EndLine.m_P1 = pRCurve->OrthoBaseVector(pLCurve->GetP0());
            }
            else
            {
                EndLine.m_P0 = pLCurve->OrthoBaseVector(pRCurve->GetP0());
                EndLine.m_P1 = pRCurve->GetP0();
            }
			
        }
        else
        {
            LLoc = pCenCurve->OrthoBaseLocValue(pLCurve->GetP1());
            RLoc = pCenCurve->OrthoBaseLocValue(pRCurve->GetP1());

			
			if((LLoc<RLoc && bShort) || (LLoc>RLoc && !bShort))
            {
				EndLine.m_P0 = pLCurve->GetP1();
				EndLine.m_P1 = pRCurve->OrthoBaseVector(pLCurve->GetP1());
			}
			else
			{
				EndLine.m_P0 = pLCurve->OrthoBaseVector(pRCurve->GetP1());
				EndLine.m_P1 = pRCurve->GetP1();
			}
			
        }
    }
    else
    {
        if(StartOrEnd==0)
        {
            EndLine.m_P0 = mp_LLine->GetP0();
            EndLine.m_P1 = mp_RLine->GetP0();
        }
        else
        {
            EndLine.m_P0 = mp_LLine->GetP1();
            EndLine.m_P1 = mp_RLine->GetP1();
        }
    }

	return EndLine;
}

GM2DLineVector MSLinkMemberG::GetEndLine4Joint2( long StartOrEnd, BOOL bShort /*= TRUE*/ )
{
	GM2DLineVector EndLine;
	GM2DLineVector CenLine = GetCenLVec();
	GM2DVector LVec, RVec;
	double LLoc,RLoc;
    if(isArc() == false)
    {
        if(StartOrEnd==0)
        {
            LLoc = CenLine.OrthoBaseLocValue(mp_LLineTR->GetP0());
            RLoc = CenLine.OrthoBaseLocValue(mp_RLineTR->GetP0());

			
			if((LLoc>RLoc && bShort) || (LLoc<RLoc && !bShort))
            {
                EndLine.m_P0 = mp_LLineTR->GetP0();
                EndLine.m_P1 = mp_RLineTR->OrthoBaseVector(mp_LLineTR->GetP0());
            }
            else
            {
				EndLine.m_P0 = mp_LLineTR->OrthoBaseVector(mp_RLineTR->GetP0());
				EndLine.m_P1 = mp_RLineTR->GetP0();
            }
			
        }
        else
        {
            LLoc = CenLine.OrthoBaseLocValue(mp_LLineTR->GetP1());
            RLoc = CenLine.OrthoBaseLocValue(mp_RLineTR->GetP1());

			
			if((LLoc<RLoc && bShort) || (LLoc>RLoc && !bShort))
            {
				EndLine.m_P0 = mp_LLineTR->GetP1();
				EndLine.m_P1 = mp_RLineTR->OrthoBaseVector(mp_LLineTR->GetP1());
			}
			else
			{
				EndLine.m_P0 = mp_LLineTR->OrthoBaseVector(mp_RLineTR->GetP1());
				EndLine.m_P1 = mp_RLineTR->GetP1();
			}
			
        }
    }
    else
    {
        if(StartOrEnd==0)
        {
            EndLine.m_P0 = mp_LLine->GetP0();
            EndLine.m_P1 = mp_RLine->GetP0();
        }
        else
        {
            EndLine.m_P0 = mp_LLine->GetP1();
            EndLine.m_P1 = mp_RLine->GetP1();
        }
    }
	return EndLine;
}

double MSLinkMemberG::GetWidth()
{
	MSCompoM* pCompoM = GetCompoM();
	if(pCompoM)
		return pCompoM->GetWidth() + GetWidthSetback();
	else
		return 0;
}

double MSLinkMemberG::GetDepth()
{
	MSCompoM* pCompoM = GetCompoM();
	if(pCompoM)
		return pCompoM->GetDepth();
	else
		return 0;
}

double MSLinkMemberG::GetThick()
{
	MSCompoM* pCompoM = GetCompoM();
	if(pCompoM)
		return pCompoM->GetThick();
	else
		return 0;
}

bool MSLinkMemberG::GetZLevels( double& TopLevel, double& BottomLevel, bool GetBigVal, bool bCheckLevelZone )
{
	double dTopOffSetZ = 0, dBotOffSetZ = 0;
	dTopOffSetZ = max(m_SOffsetZ, m_EOffsetZ);
	dBotOffSetZ = min(m_SOffsetZ, m_EOffsetZ);

	double dFloorLevelOffSet = 0;
	MSCompoM* pCompoM = GetCompoM();
	if (pCompoM != nullptr && pCompoM->IsCustomM())
	{
		MSCustomM* pMSCustomM = dynamic_cast<MSCustomM*>(pCompoM);
		if(pMSCustomM != nullptr)
			dFloorLevelOffSet = pMSCustomM->GetFloorLevelOffSet();
	}

	dTopOffSetZ += dFloorLevelOffSet;
	dBotOffSetZ += dFloorLevelOffSet;

	double dTopZLevelDif = 0;
	double dBotZLevelDif = 0;
	if(bCheckLevelZone)
	{
		dTopZLevelDif = GetLevelbyLevelDif(true);
		dBotZLevelDif = GetLevelbyLevelDif(false);
	}

	if(mp_Owner && mp_Owner->IsFloor())
	{
		MSFloor* pCurFloor = (MSFloor*)mp_Owner;
		TopLevel = GetSlabLevel() + ((MSFloor*)mp_Owner)->GetSlabHeight() + dTopOffSetZ + dTopZLevelDif;
	}
	else
		TopLevel = GetSlabLevel() + dTopOffSetZ;
	BottomLevel = GetSlabLevel() + ((MSFloor*)mp_Owner)->GetSlabHeight() + dBotOffSetZ + dBotZLevelDif - GetDepth();
	return true;
}

double MSLinkMemberG::GetOffSetbyLoc(double dLoc)
{
	MSCompoM* pCompoM = GetCompoM();
	return (m_EOffsetZ - m_SOffsetZ)* dLoc + m_SOffsetZ + GetCompoMFloorLevelOffSet();
}

double MSLinkMemberG::GetStartOffSetbyLoc(double dLoc)
{
	return GetOffSetbyLoc(dLoc);
}

bool MSLinkMemberG::GetZLevelsbyLoc(double& TopLevel, double& BottomLevel, double dLoc)
{
	double dOffSet = GetOffSetbyLoc(dLoc);
	double dLevelZoneZ = 0;

	if(mp_Owner && mp_Owner->IsFloor())
	{
		MSFloor* pCurFloor = (MSFloor*)mp_Owner;
		GM2DVector TheVec;
		shared_ptr<GM2DCurve> pCenCurve = GetCenCurve();
		if(dLoc <= 0)
			TheVec = pCenCurve->PointAtParam(0);
		else if(dLoc >= 1)
			TheVec = pCenCurve->PointAtParam(1);
		else
			TheVec = pCenCurve->PointAtParam(dLoc);

		if(GetType() == MSElement::msLineLevelDifG)
			dLevelZoneZ = pCurFloor->GetLevel4LevelZone(TheVec.m_X, TheVec.m_Y);
		else
		{
			long NumDif = ma_MemberLevelDif.GetSize();
			if(NumDif > 0)
				dLevelZoneZ = pCurFloor->GetLevel4LevelZone(TheVec.m_X, TheVec.m_Y);
			else // 레벨존 안에 완전히 포함 또는 레벨존에 포함 되지 않음
				dLevelZoneZ = GetFloor()->GetLevel4LevelZone(this);
		}

		TopLevel = GetSlabLevel() + ((MSFloor*)mp_Owner)->GetSlabHeight() + dOffSet + dLevelZoneZ;
	}
	else
		TopLevel = GetSlabLevel()+dOffSet;
	BottomLevel = TopLevel-GetDepth();
	return true;
}

bool MSLinkMemberG::GetOverLapPolyLineCenLoc(GM2DPolyline* pPoly, vector<GM2DCurve*>& NewCurveArr)
{
	shared_ptr<GM2DCurve> pCenCurve = GetCenCurve();

	//vector<GM2DCurve*> NewCurveArr;
	pPoly->GetCurveInPolygon(&*pCenCurve, NewCurveArr);
	
	if(NewCurveArr.size() == 0)
	{
		GM2DVector CenPolyVec = pPoly->MassCenter();
		if(pCenCurve->WhichSide(CenPolyVec) == 1)
		{
			pPoly->GetCurveInPolygon(mp_LLine, NewCurveArr);
		}
		else
		{
			pPoly->GetCurveInPolygon(mp_RLine, NewCurveArr);
		}
	}

	if (NewCurveArr.size() > 0)
		return true;

	return false;

// 	long NumNewCurve = NewCurveArr.size();
// 
// 	if(NewCurveArr.size() > 0)
// 	{
// 		GM2DCurve* pCenCurve = NewCurveArr[0];
// 		CenVec = pCenCurve->PointAtParam(0.5);
// 	}
// 	
// 	for(long i = 0 ; i < NewCurveArr.size() ; i++)
// 		delete NewCurveArr[i];
// 	NewCurveArr.clear();
// 
// 	dLoc = pCenCurve->OrthoBaseLocValue(CenVec);
// 
// 	if(NumNewCurve > 0 &&
// 		(dLoc > 0 - DTOL_GM && dLoc < 1 + DTOL_GM))
// 
// 		return true;
// 
// 	dLoc = 0;
// 	return false;
}

bool MSLinkMemberG::MakeBoundary()
{
	/*
	ma_BoundaryPoints.clear();
	ma_BoundaryPoints.push_back(mp_LLine->m_P0);
	ma_BoundaryPoints.push_back(mp_LLine->m_P1);
	ma_BoundaryPoints.push_back(mp_RLine->m_P1);
	ma_BoundaryPoints.push_back(mp_RLine->m_P0);
	*/
	if (mp_RLine == nullptr || mp_LLine == nullptr)
		SetLines();
	GM2DPolyline* pBoundary = mp_Boundary;
	pBoundary->ResetPolyline();
	if(isArc() == false)
	{
		pBoundary->AddCopy(mp_RLine->m_P0);
		pBoundary->ma_Bulge.Add(0);
		pBoundary->AddCopy(mp_RLine->m_P1);
		pBoundary->ma_Bulge.Add(0);
		pBoundary->AddCopy(mp_LLine->m_P1);
		pBoundary->ma_Bulge.Add(0);
		pBoundary->AddCopy(mp_LLine->m_P0);
		pBoundary->ma_Bulge.Add(0);
		pBoundary->AddCopy(mp_RLine->m_P0);
	}
	else
	{
		pBoundary->AddCopy(mp_RLine->m_P0);
		pBoundary->ma_Bulge.Add(mp_RLine->GetBulge());
		pBoundary->AddCopy(mp_RLine->m_P1);
		pBoundary->ma_Bulge.Add(0);
		pBoundary->AddCopy(mp_LLine->m_P1);
		pBoundary->ma_Bulge.Add(mp_LLine->GetBulge() * -1);
		pBoundary->AddCopy(mp_LLine->m_P0);
		pBoundary->ma_Bulge.Add(0);
		pBoundary->AddCopy(mp_RLine->m_P0);
	}
	pBoundary->MakeCCW();

	MakeSubMemberBoundary();
	return true;
}

bool MSLinkMemberG::MakeSubMemberBoundary()
{
	return true;
}

bool MSLinkMemberG::IsColinear4DTol(MSLinkMemberG* pUrLink, double DegreeTol, double DistTol)
{
	if (isArc() == false && pUrLink->isArc() == false)
	{
		GM2DLineVector MyLVec = GetCenLVec();
		GM2DLineVector UrLVec = pUrLink->GetCenLVec();

		if (MyLVec.IsColinear4DTol(UrLVec, DegreeTol, DistTol))
			return true;
	}
	//아크 일때?
	return false;
}
bool MSLinkMemberG::IsParallel( MSLinkMemberG* pUrLink, double dDegTol)
{
	if(isArc() == false && pUrLink->isArc() == false)
	{
		GM2DLineVector MyLVec = GetCenLVec();
		GM2DLineVector UrLVec = pUrLink->GetCenLVec();

		if(MyLVec.IsParallel4DTol(UrLVec, dDegTol))
			return true;
	}
	else if(isArc() && pUrLink->isArc())
	{
		GM2DArc* pMyArc = (GM2DArc*)mp_Curve;
		GM2DArc* pUrArc = (GM2DArc*)pUrLink->mp_Curve;

		if(pMyArc->m_Center.Distance(pUrArc->m_Center) < MSFloor::ms_DefLineHitDistTol)
		   return true;
	}

	return false;
}

bool MSLinkMemberG::IsSameInfo(MSLinkMemberG* pUrLink)
{
	if (GetType() != pUrLink->GetType())
		return false;
	if (IsEachCompoM() != pUrLink->IsEachCompoM())
		return false;
	if (IsEachCompoM())
	{
		if ( (GetWidth() != pUrLink->GetWidth()) ||
			(GetDepth() != pUrLink->GetDepth()) ||
			(GetThick() != pUrLink->GetThick()) ||
			(GetPcWidth() != pUrLink->GetPcWidth()) ||
			(GetPcDepth() != pUrLink->GetPcDepth()) ||
			(GetPcThick() != pUrLink->GetPcThick()) ||
			(GetTopSpacerID() != pUrLink->GetTopSpacerID()) ||
			(GetBotSpacerID() != pUrLink->GetBotSpacerID()) )
			return false;
	}
	else
	{
		if (GetCompoM() != pUrLink->GetCompoM())
			return false;
	}

	return true;
}

double MSLinkMemberG::GetMaxPartnerWidth()
{
	double dMaxWidth = GetWidth();
	long NumConn = ma_Connections.size();
	for(long iConn = 0 ; iConn < NumConn ; iConn++)
	{
		MSConnection* pConn = ma_Connections[iConn];
		MSCompoG* pParther = pConn->GetPartner(this);
		if(pParther == nullptr)
			continue;
		double dWidth = pParther->GetWidth();
		if (pParther->IsPointMember() && pParther->IsButtress() == false)
			dWidth = max(dWidth, pParther->GetDepth());
		dMaxWidth = max(dMaxWidth, dWidth);
	}
	return dMaxWidth;
}

bool MSLinkMemberG::isArc()
{
	if(mp_Curve != nullptr && mp_Curve->m_ObjType == GT_ARC)
		return true;	
	return false;
}

void MSLinkMemberG::SetCenCurve(GM2DCurve* pCurve)
{
	if (pCurve == nullptr)
		return;

	if (mp_Curve != nullptr && mp_Curve != pCurve)
	{
		delete mp_Curve;
		mp_Curve = nullptr;
	}

	if (pCurve->m_ObjType == GT_ARC)
	{
		GM2DVector SVec = pCurve->m_P0;
		GM2DVector EVec = pCurve->m_P1;
		GM2DVector CVec = pCurve->PointAtParam(0.5);
		GM2DArc TmpArc;
		if (TmpArc.Set3P(SVec, CVec, EVec) == false)
		{
			delete pCurve;
			pCurve = new GM2DLineVector(SVec, EVec);
		}
	}
	
	if(pCurve->m_ObjType == GT_ARC)
	{
		GM2DArc* pArc = (GM2DArc*)pCurve;
		pCurve->SetP0(pArc->GetStart2DVec());
		pCurve->SetP1(pArc->GetEnd2DVec());
		mp_Curve = pCurve;
		ReplaceVec(0, (GM2DVector*)pCurve->m_P0.Copy());
		ReplaceVec(1, (GM2DVector*)pCurve->m_P1.Copy());
	}
	else
	{
		ReplaceVec(0, (GM2DVector*)pCurve->m_P0.Copy());
		ReplaceVec(1, (GM2DVector*)pCurve->m_P1.Copy());
		delete pCurve;
	}
}

void MSLinkMemberG::SetCurve_Input(GM2DCurve* pCurve)
{
	DeleteVecInput();

	ma_Vec_Input.push_back((GM2DVector*)pCurve->m_P0.Copy());
	ma_Vec_Input.push_back((GM2DVector*)pCurve->m_P1.Copy());

}

shared_ptr<GM2DCurve> MSLinkMemberG::GetCurve_Input()
{
	shared_ptr<GM2DCurve> CenCurve = nullptr;
	if (IsInvalidVecInput())
		UpdateCurveInputByCenCurve();

	if(isArc() == false)
	{
		CenCurve = shared_ptr<GM2DCurve>(new GM2DLineVector(*ma_Vec_Input[0], *ma_Vec_Input[1]));
	}
	else
	{
		//double dWidth = GetWidth();
		CenCurve = shared_ptr<GM2DCurve>(mp_Curve->Copy());

		long nSide = mp_Curve->WhichSide(*ma_Vec_Input[0]);

		double dist = fabs( mp_Curve->m_P0.Distance(*ma_Vec_Input[0]) );

		if(nSide == 1)
			CenCurve->NormalTranslateMe(dist);
		else if(nSide == -1)
			CenCurve->NormalTranslateMe(-dist);
	}
	return CenCurve;
}

void MSLinkMemberG::InitializeParameter()
{
	MSFloor* pFloor	= GetFloor();
	
	m_isBuriedLinkMember = false;
	MSCompoG::InitializeParameter();

	//DeleteMemberLevelDifG();
	for (int i = 0; i < ma_MemberLevelDif.GetSize(); ++i)
		ma_DeletedDif.push_back(ma_MemberLevelDif[i]);
	ma_MemberLevelDif.RemoveAll();

	InitMemPart();
}

GM2DVector MSLinkMemberG::GetCenVec()
{
	shared_ptr<GM2DCurve> pCurve = GetCenCurve();
	GM2DVector vec = pCurve->PointAtParam(0.5);
	return vec;
}

void MSLinkMemberG::SetBuriedLinkMember()
{
	m_isBuriedLinkMember = true;
	//InitMemPart();
}

void MSLinkMemberG::RemoveLevelDif( MSLineLevelDifG* pLineLevelDifg )
{
	long nLevelDif = ma_MemberLevelDif.GetSize();
	for(long iLevelDif=nLevelDif-1; iLevelDif>=0; iLevelDif--)
	{
		if(ma_MemberLevelDif[iLevelDif]->mp_LineLevelDifG ==pLineLevelDifg)
		{
			//delete ma_MemberLevelDif[iLevelDif];
			ma_DeletedDif.push_back(ma_MemberLevelDif[iLevelDif]);
			ma_MemberLevelDif.RemoveAt(iLevelDif);
		}
	}
}
double MSLinkMemberG::GetLevelbyLevelDif(bool isMax)
{
	double dRval = 0;
	double dMax = 0;
	
	if(isMax)
		dRval = INT32_MIN;
	else
		dRval = INT32_MAX;

	long nCnt = 0;
	long NumDifG = ma_MemberLevelDif.GetCount();
	for(long iDifG = 0 ; iDifG < NumDifG ; iDifG++)
	{
		if(ma_MemberLevelDif[iDifG]->m_EndLevel != 0 )
		{
			dMax = ma_MemberLevelDif[iDifG]->m_EndLevel;
			nCnt++;
		}
		if(ma_MemberLevelDif[iDifG]->m_StartLevel != 0)
		{
			dMax = ma_MemberLevelDif[iDifG]->m_StartLevel;
			nCnt++;
		}

		if(isMax)
			dRval = max(dMax , dRval);
		else
			dRval = min(dMax , dRval);
	}

	if(nCnt == 0)
	{
		return GetFloor()->GetLevel4LevelZone(this, isMax);
	}
	return dRval;
}


vector<double> MSLinkMemberG::GetLevelDifLevel()
{
	vector<double> LevelArr;
	long nLevelDif = ma_MemberLevelDif.GetSize();
	if(nLevelDif == 0)
		return LevelArr;
	MSMemberLevelDif* pMemberLevelDif= nullptr;
	for(long iLevelDif= 0 ; iLevelDif < nLevelDif; iLevelDif++)
	{
		pMemberLevelDif = ma_MemberLevelDif[iLevelDif];
		if(pMemberLevelDif != nullptr)
			LevelArr.push_back(pMemberLevelDif->m_StartLevel);
	}
	if(pMemberLevelDif != nullptr)
		LevelArr.push_back(pMemberLevelDif->m_EndLevel);

	return LevelArr;
}

long MSLinkMemberG::CheckCrossThenAddLevelDif( MSLineLevelDifG* pLineLevelDifg, CTypedPtrArray<CObArray, MSMemberLevelDif*>& MemberLevelDifArr, MSFloor* pFloor)
{
	if(IsSlopeMember() || IsCustomM())
		return 0;

	GM2DCurve* pLevelDifCurve = pLineLevelDifg->mp_Curve;
	GM2DCurve* pMyCurve;
	GM2DLineVector MyLine;
	long nCross=0;

	if(isArc() == false)
	{
		MyLine = GetCenLVec();
		pMyCurve = &MyLine;
	}
	else
		pMyCurve = mp_Curve;

	GM2DVector LevelDifCrossVec = pLevelDifCurve->m_P0;
	GM2DVector MyLCrossVec = mp_LLine->m_P0;
	GM2DVector MyRCrossVec = mp_RLine->m_P0;
	double dLLoc = -1, dRLoc = -1;
	double MyLoc = -1;
	GM2DVector MyCrossVec = pMyCurve->m_P0;

	vector<GM2DVector> CrossVecArr;
	vector<double> LevelDifLocArr, CurveLocArr;
	vector<long> ResultArr;
	long idx = -1;

	long Result = pLevelDifCurve->IntersectEnd2(pMyCurve, CrossVecArr, LevelDifLocArr, CurveLocArr, ResultArr, false);
	if(Result%10==1)  
	{
		for(long i = 0 ; i < ResultArr.size() ; i++)
		{
			if(ResultArr[i] == 1)
				idx = i;
		}
		if (idx != -1)
			dLLoc = dRLoc = MyLoc = CurveLocArr[idx];
        
		if(MyLoc < 1.-DTOL_GM && MyLoc > DTOL_GM)	
		{
			idx = -1;
            pLevelDifCurve->IntersectEnd2(mp_LLine, CrossVecArr, LevelDifLocArr, CurveLocArr, ResultArr, false);
            for(long i = 0 ; i < ResultArr.size() ; i++)
            {
                if(ResultArr[i] == 1)
                    idx = i;
            }
			if (idx != -1)
				dLLoc = CurveLocArr[idx];

			idx = -1;
            pLevelDifCurve->IntersectEnd2(mp_RLine, CrossVecArr, LevelDifLocArr, CurveLocArr, ResultArr, false);
            for(long i = 0 ; i < ResultArr.size() ; i++)
            {
                if(ResultArr[i] == 1)
                    idx = i;
            }
			if (idx != -1)
				dRLoc = CurveLocArr[idx];

			if(AddLevelDif(MyLoc, dLLoc, dRLoc, pLineLevelDifg, MemberLevelDifArr, pFloor))
				nCross++;
		}

		return nCross;
	}
	else if(Result == 2)
	{
		for(long i = 0 ; i < 2 ; i++)
		{
			MyLoc = CurveLocArr[i];

			if(MyLoc < 1.-DTOL_GM && MyLoc > DTOL_GM)	
			{
                vector<GM2DVector> CrossVecArr2;
                vector<double> LevelDifLocArr2, CurveLocArr2;
                vector<long> ResultArr2;

                pLevelDifCurve->IntersectEnd2(mp_LLine, CrossVecArr2, LevelDifLocArr2, CurveLocArr2, ResultArr2, false);

                if(CurveLocArr2.size() == 2)
                    dLLoc = CurveLocArr[i];
                else
                    dLLoc = CurveLocArr[0];

                pLevelDifCurve->IntersectEnd2(mp_RLine, CrossVecArr2, LevelDifLocArr2, CurveLocArr2, ResultArr2, false);

                if(CurveLocArr2.size() == 2)
                    dRLoc = CurveLocArr[i];
                else
                    dRLoc = CurveLocArr[0];

				if(AddLevelDif(MyLoc, dLLoc, dRLoc, pLineLevelDifg, MemberLevelDifArr, pFloor))
					nCross++;
			}

		}
	}

	return nCross;

/*

	long LResult = pLevelDifCurve->IntersectEnd(mp_LLine, LevelDifCrossVec, MyLCrossVec, &dLLevelDifLoc, &dLLoc false);
	long RResult = pLevelDifCurve->IntersectEnd(mp_RLine, LevelDifCrossVec, MyRCrossVec, &dRLevelDifLoc, &dRLoc, false);

	//GM2DVector LevelDifCrossVec = pLevelDifCurve->m_P0;
	GM2DVector MyCrossVec = pMyCurve->m_P0;
	double LevelDifLoc, MyLoc;
	long Result = pLevelDifCurve->IntersectEnd(pMyCurve, LevelDifCrossVec, MyCrossVec, &LevelDifLoc, &MyLoc, false);
	// return : -1 : Colinear 
	//          -2 : Parallel
	//           1 : Intersect at 1 point
	//           2 : not Parallel and not intersection, 외부 교차
	//           0 : 길이 제로 라인벡터 포함 에러
	if(Result!=1)  
		return false;

	if(MyLoc < 1.-DTOL_GM && MyLoc > DTOL_GM)	
	{
		if(AddLevelDif(MyLoc, dLLoc, dRLoc, pLineLevelDifg))
			nCross++;
	}

	if(mp_Curve==nullptr && pMyCurve->m_ObjType==GT_LINE)
		return nCross;

	double FirstUrLoc = MyLoc;

	LevelDifCrossVec = pLevelDifCurve->m_P1;
	MyCrossVec = pMyCurve->m_P1;
	Result = pLevelDifCurve->IntersectEnd(pMyCurve, LevelDifCrossVec, MyCrossVec, &LevelDifLoc, &MyLoc, true);
	if(Result!=1 || fabs(MyLoc-FirstUrLoc) < DTOL_GM)  
		return nCross;

	if(MyLoc < 1.-DTOL_GM && MyLoc > DTOL_GM)
	{
		if(AddLevelDif(MyLoc, dLLoc, dRLoc, pLineLevelDifg))
			nCross++;
	}

	return nCross;*/
}

bool MSLinkMemberG::AddLevelDif( double Loc, double dLLoc, double dRLoc, MSLineLevelDifG* pLineLevelDifg, CTypedPtrArray<CObArray, MSMemberLevelDif*>& MemberLevelDifArr, MSFloor* pFloor)
{
	GM2DLineVector MyLVec;
	GM2DCurve* pMyCurve;
	if(isArc() == false)
	{
		MyLVec = GetCenLVec();
		pMyCurve = &MyLVec;
	}
	else
		pMyCurve = mp_Curve;

    // jissi / 용도와 1.5의 의미
	//Loc 기준으로 전후가 부재 Depth의 1.5배 이하이면 Dif정보를 생성하지 않는다.
	double dCurveLen = pMyCurve->Length();
	//double InfillThickValue = GetDepth()*1.5 / dCurveLen;
	double InfillThickValue = GetDepth() / dCurveLen;
	if (GetType() == MSElement::msWallG)
		InfillThickValue = 0;

	double dNextLocVal = MSFloor::ms_DefLineHitDistTol / dCurveLen;

	bool hasSameMemberLevelDif = false;
	MSMemberLevelDif* pMemberLevelDif = nullptr;

	long nLevelDif = MemberLevelDifArr.GetSize();
	for(long iLevelDif=0; iLevelDif<nLevelDif; iLevelDif++)
	{
		pMemberLevelDif = MemberLevelDifArr[iLevelDif];
		// mylee : mp_LineLevelDifG 가 아닌 위치 정보로만 중복 체크한다 (Serialize Add 시에 nullptr 인 상태로 들어온다)
		//if (pMemberLevelDif->mp_LineLevelDifG == nullptr) continue; // mylee temp
		// 이미 존재하면 Dif정보를 생성하지 않는다.
		//if(pMemberLevelDif->mp_LineLevelDifG == pLineLevelDifg)
		{
			if (fabs(pMemberLevelDif->m_Loc - Loc) < DTOL_GM)
			{
				hasSameMemberLevelDif = true;
				break; // 20150901 LevelZone Bug Fix
			}
		}

		// InfillThickValue 값 변경으로 Serialize시 InfillThickValue값 사이에 래벨Dif가 존재하면 값을 변경여 죽는 문제가 발생
		// 이 경우 일단 LevelDif 값을 다시 만든다. 
		if (pMemberLevelDif->mp_LineLevelDifG == nullptr)
			continue;

		// 버그 가능성 있음 //kkw
		if(fabs(pMemberLevelDif->m_Loc-Loc) < InfillThickValue)
		{
			//ma_BeamLevelDif[iLevelDif]->mp_LineLevelDifG와 pLineLevelDif의 방향이 같으면오류

			//다르면
			double dLevelDif = pMemberLevelDif->mp_LineLevelDifG->mp_LeftLevelZone->m_Level;
			if(dLevelDif == pMemberLevelDif->m_StartLevel)
				pMemberLevelDif->m_EndLevel=pLineLevelDifg->mp_LeftLevelZone->m_Level;
			else if(dLevelDif == pMemberLevelDif->m_EndLevel)
				pMemberLevelDif->m_StartLevel=pLineLevelDifg->mp_LeftLevelZone->m_Level;
			return false;
		}
	}

	// iLevelDif 값을 Insert 위치에 활용
	for(long iLevelDif=0; iLevelDif<nLevelDif; iLevelDif++)
	{
		if(Loc < MemberLevelDifArr[iLevelDif]->m_Loc + DTOL10)
			break;
	}

	if(hasSameMemberLevelDif == false)
	{
		//  Deleted 에서 한번 더 검색하여 존재하면 사용
		int idx_same = -1;
		for (int i = 0; i < ma_DeletedDif.size(); ++i)
		{
			if (fabs(ma_DeletedDif[i]->m_Loc - Loc) < DTOL_GM) { idx_same = i; break; }
		}
		if (idx_same != -1)
		{
			pMemberLevelDif = ma_DeletedDif[i];
			ma_DeletedDif.erase(ma_DeletedDif.begin() + idx_same);
		}
		else
			pMemberLevelDif = new MSMemberLevelDif();
	}

	if(mp_Owner->m_bIsSerializing) // Serialize Add 시에 대한 처리
		pLineLevelDifg->ma_LinkMemberGID.push_back(GetID());

	// 20150901 LevelZone Bug Fix
	// ma_LinkMemberGID 에 현재 Link 의 ID 가 없는 경우만 추가해 주도록
	bool exist = false;
	for (int i = 0; i < pLineLevelDifg->ma_LinkMemberGID.size(); ++i)
	{
		if (pLineLevelDifg->ma_LinkMemberGID[i] == GetID()) { exist = true; break; }
	}
	if (!exist)
		pLineLevelDifg->ma_LinkMemberGID.push_back(GetID());

	pMemberLevelDif->m_Loc = Loc;
	pMemberLevelDif->m_LLoc = dLLoc;
	pMemberLevelDif->m_RLoc = dRLoc;
	pMemberLevelDif->mp_LineLevelDifG = pLineLevelDifg;
	pMemberLevelDif->mp_OwnerCompoG = this;
	
	GM2DVector CrossNextVec;
	CrossNextVec = pMyCurve->PointAtParam(Loc + dNextLocVal);

	// 20150901 LevelZone Bug Fix
	GM2DVector CrossPreVec = pMyCurve->PointAtParam(Loc - dNextLocVal);
	pMemberLevelDif->m_StartLevel = pFloor->GetLevel4LevelZone(CrossPreVec.m_X, CrossPreVec.m_Y);
	pMemberLevelDif->m_EndLevel = pFloor->GetLevel4LevelZone(CrossNextVec.m_X, CrossNextVec.m_Y);

	/*
	if(pLineLevelDifg->mp_Curve->WhichSide(CrossNextVec)==1)
	{
		pMemberLevelDif->m_EndLevel = pLineLevelDifg->mp_LeftLevelZone->m_Level;
		if(iLevelDif==0 || hasSameMemberLevelDif)
		{
			GM2DVector CrossPreVec;
			CrossPreVec = pMyCurve->PointAtParam(Loc - dNextLocVal);
			pMemberLevelDif->m_StartLevel = pFloor->GetLevel4LevelZone(CrossPreVec.m_X,CrossPreVec.m_Y);//GetRelativeStartLevel();
		}
		else
			pMemberLevelDif->m_StartLevel = MemberLevelDifArr[iLevelDif-1]->m_EndLevel;//->m_Loc;
	}
	else
	{
		pMemberLevelDif->m_StartLevel = pLineLevelDifg->mp_LeftLevelZone->m_Level;
		if(iLevelDif==nLevelDif || hasSameMemberLevelDif)
			pMemberLevelDif->m_EndLevel = pFloor->GetLevel4LevelZone(CrossNextVec.m_X,CrossNextVec.m_Y);//GetRelativeEndLevel();
		else
			pMemberLevelDif->m_EndLevel = MemberLevelDifArr[iLevelDif]->m_StartLevel;//->m_Loc;
	}
	*/

	if (!mp_Owner->m_bIsSerializing)
	{
		if (pMemberLevelDif->m_InfillThick == 0.0)
			pMemberLevelDif->m_InfillThick = GetDepth();
	}

	if (GetType() == MSElement::msWallG)
		pMemberLevelDif->m_InfillThick = 0;

	if (pMemberLevelDif->m_StartLevel < pMemberLevelDif->m_EndLevel)
	{
		if (Loc + InfillThickValue > 1)
		{
			InfillThickValue = 1 - Loc;
			pMemberLevelDif->m_InfillThick = InfillThickValue * dCurveLen;

// 			delete pMemberLevelDif;
// 			return false;
		}
	}
	else
	{ 
		if (Loc - InfillThickValue < 0)
		{
			InfillThickValue = Loc;
			pMemberLevelDif->m_InfillThick = InfillThickValue * dCurveLen;

// 			delete pMemberLevelDif;
// 			return false;
		}
	}

	if (pMemberLevelDif->m_StartLevel == pMemberLevelDif->m_EndLevel)
	{
		if (hasSameMemberLevelDif == true)
		{
			for (int iDif = 0; iDif < MemberLevelDifArr.GetSize(); iDif++)
			{
				MSMemberLevelDif* pDif = MemberLevelDifArr[iDif];
				if (pDif == pMemberLevelDif)
				{
					MemberLevelDifArr.RemoveAt(iDif);
					break;
				}
			}
		}
		delete pMemberLevelDif;
		return false;
	}

	double dMinLoc = Loc;
	double dMaxLoc = Loc;
	if (pMemberLevelDif->m_StartLevel < pMemberLevelDif->m_EndLevel)
		dMaxLoc += InfillThickValue;
	else
		dMinLoc -= InfillThickValue;

	if (hasSameMemberLevelDif == false)
	{
		for (int iDif = 0; iDif < MemberLevelDifArr.GetSize(); iDif++)
		{
			MSMemberLevelDif* pDif = MemberLevelDifArr[iDif];
			if (pDif->m_StartLevel < pDif->m_EndLevel)
			{
				if ((pDif->m_Loc < dMinLoc && pDif->m_Loc + InfillThickValue > dMinLoc) ||
					(pDif->m_Loc < dMaxLoc && pDif->m_Loc + InfillThickValue > dMaxLoc))
				{
					delete pMemberLevelDif;
					return false;
				}
			}
			else
			{
				if ((pDif->m_Loc - InfillThickValue < dMinLoc && pDif->m_Loc > dMinLoc) ||
					(pDif->m_Loc - InfillThickValue < dMaxLoc && pDif->m_Loc > dMaxLoc))
				{
					delete pMemberLevelDif;
					return false;
				}
			}
		}
	}

	if(hasSameMemberLevelDif == false)
		MemberLevelDifArr.InsertAt(iLevelDif,pMemberLevelDif);

	MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(m_ID);

	return true;
}


vector<GM2DCurve*> MSLinkMemberG::GetLevelDifCurves()
{
	vector<GM2DCurve*> CurveArr;
	long nLevelDif = ma_MemberLevelDif.GetSize();
	if(nLevelDif == 0)
		return CurveArr;

	shared_ptr<GM2DCurve> pOrgMyCurve = GetCenCurve();
	shared_ptr<GM2DCurve> pMyCurve = GetCenCurvebyConntion();

	GM2DVector SVec, EVec;

	SVec = pMyCurve->m_P0;

	for(long iLevelDif= 0 ; iLevelDif < nLevelDif; iLevelDif++)
	{
		MSMemberLevelDif* pLevelDif = ma_MemberLevelDif[iLevelDif];
		EVec = pOrgMyCurve->PointAtParam(pLevelDif->m_Loc);
		GM2DCurve* pCurCurve = pMyCurve->Copy();

		pCurCurve->SetP0(SVec);
		pCurCurve->SetP1(EVec);

		CurveArr.push_back(pCurCurve);
		SVec = EVec;
	}

	GM2DCurve* pCurCurve = pMyCurve->Copy();
	pCurCurve->SetP0(SVec);
	pCurCurve->SetP1(pMyCurve->m_P1);
	
	CurveArr.push_back(pCurCurve);

	return CurveArr;
}

vector<int> MSLinkMemberG::GetCZoneIDsByCenCurve()
{
	MSFloor* pFloor = GetFloor();
	vector<GM2DCurve*> CenCurveArr = pFloor->GetCenCurveByCZone(this);

	vector<int> CZoneIDList;
	bool IsHorizontalZone = IsVerticalMember() == true ? false : true;
	long NumCenCurve = CenCurveArr.size();
	for(long iCurve = 0 ; iCurve < NumCenCurve ; iCurve++)
	{
		GM2DCurve* pCurve = CenCurveArr[iCurve];
		GM2DVector MidVec = pCurve->PointAtParam(0.5);
		long CZoneID = pFloor->GetOverlapCZoneID(MidVec.m_X, MidVec.m_Y, IsHorizontalZone);
		CZoneIDList.push_back(CZoneID);
		delete pCurve;
	}
	CenCurveArr.clear();

	return CZoneIDList;
}

vector<double> MSLinkMemberG::GetLevelDifLevelByCZone(int idx, bool isTop)
{
	// GetCenCurveByCZoneIDs(); 결과로 나온 배열의 idx 값을 사용하여 Level 값을 구한다.

	vector<double> LevelArr;

	MSFloor* pFloor = GetFloor();
	MSFloor* pCheckFloor = nullptr;
	if(isTop)
		pCheckFloor = pFloor->GetUpperFloor();
	else
		pCheckFloor = pFloor;

	if(pFloor == nullptr || pCheckFloor == nullptr)
		return LevelArr;

	vector<int> CZoneIDArr = GetCZoneIDsByCenCurve();
	int CZoneID = CZoneIDArr[idx];
	MSConstructionZone* pCZone = pFloor->GetCZone(CZoneID);

	vector<GM2DCurve*> CurveArr = GetLevelDifCurves();
	vector<GM2DCurve*> CZoneCurveArrAll;

	long NumDifCurve = CurveArr.size();
	bool IsHorizontalZone = !IsVerticalMember();
	for(long iCurve = 0 ; iCurve < NumDifCurve ; iCurve++)
	{
		GM2DCurve* pCurve = CurveArr[iCurve];
		vector<GM2DCurve*> CZoneCurveArr = pFloor->GetCurveByCZone(pCurve, IsHorizontalZone);
		for(long i = 0 ; i < CZoneCurveArr.size(); i++)
		{
			GM2DCurve* pCZoneCurve = CZoneCurveArr[i];
			GM2DVector MidVec = pCZoneCurve->PointAtParam(0.5);
			if(!pCZone->Contains(MidVec.m_X, MidVec.m_Y)) continue;

			CZoneCurveArrAll.push_back(CZoneCurveArr[i]);
		}
	}

	long NumDifCurveAll = CZoneCurveArrAll.size();

	for(long iCurve = 0 ; iCurve < NumDifCurveAll ; iCurve++)
	{
		GM2DCurve* pCurve = CZoneCurveArrAll[iCurve];
		GM2DVector MidVec = pCurve->PointAtParam(0.5);
		double dZlevel = pCheckFloor->GetZLevel(MidVec.m_X, MidVec.m_Y);
		LevelArr.push_back(dZlevel);
		delete pCurve;
	}

	for(long iCurve = 0 ; iCurve < NumDifCurve ; iCurve++)
		delete CurveArr[iCurve];
	CurveArr.clear();

	return LevelArr;
}

vector<GM2DCurve*> MSLinkMemberG::GetLevelDifCurvesByCZone(int idx)
{
	// GetCenCurveByCZoneIDs(); 결과로 나온 배열의 idx 값을 사용하여 GM2DCurve를 구한다.

	vector<GM2DCurve*> CurveArr = GetLevelDifCurves();
	
	MSFloor* pFloor = GetFloor();
	if(pFloor == nullptr)
		return CurveArr;

	vector<int> CZoneIDArr = GetCZoneIDsByCenCurve();
	int CZoneID = CZoneIDArr[idx];
	MSConstructionZone* pCZone = pFloor->GetCZone(CZoneID);
	
	vector<GM2DCurve*> CZoneCurveArrAll;

	long NumDifCurve = CurveArr.size();
	bool IsHorizontalZone = !IsVerticalMember();
	for(long iCurve = 0 ; iCurve < NumDifCurve ; iCurve++)
	{
		GM2DCurve* pCurve = CurveArr[iCurve];
		vector<GM2DCurve*> CZoneCurveArr = pFloor->GetCurveByCZone(pCurve, IsHorizontalZone);

		for(long i = 0 ; i < CZoneCurveArr.size(); i++)
		{
			GM2DCurve* pCZoneCurve = CZoneCurveArr[i];
			GM2DVector MidVec = pCZoneCurve->PointAtParam(0.5);
			if(!pCZone->Contains(MidVec.m_X, MidVec.m_Y)) continue;

			CZoneCurveArrAll.push_back(CZoneCurveArr[i]);
		}
	}

	return CZoneCurveArrAll;
}

void MSLinkMemberG::DeleteMemberLevelDifG()
{
	long nLevelDif = ma_MemberLevelDif.GetSize();
	for(long iLevelDif= 0 ; iLevelDif < nLevelDif; iLevelDif++)
	{
		MSMemberLevelDif* pBeamDif = ma_MemberLevelDif[iLevelDif];
		delete pBeamDif;
	}
	ma_MemberLevelDif.RemoveAll();
}

void MSLinkMemberG::MoveToDeleted_MemberLevelDifG()
{
	for (int i = 0; i < ma_MemberLevelDif.GetSize(); ++i)
		ma_DeletedDif.push_back(ma_MemberLevelDif[i]);
	ma_MemberLevelDif.RemoveAll();
}

void MSLinkMemberG::Translate( GM2DVector &pDelta )
{
	MSCompoG::Translate(pDelta);
	if(isArc())
	{
		GM2DArc* pMemArc = (GM2DArc*)mp_Curve;
		pMemArc->Translate(pDelta);
	}
}

void MSLinkMemberG::MirrorByAxis( GM2DLineVector& pAxis )
{
	MSCompoG::MirrorByAxis(pAxis);
	if(isArc())
	{
		GM2DArc* pMemArc = (GM2DArc*)mp_Curve;
		pMemArc->MirrorByAxis(pAxis);
	}
}

void MSLinkMemberG::RotateZ( double theta, GM2DVector *pCenter )
{
	MSCompoG::RotateZ(theta, pCenter);
	if(isArc())
	{
		GM2DArc* pMemArc = (GM2DArc*)mp_Curve;
		pMemArc->RotateZ(theta, pCenter);
	}
}

bool MSLinkMemberG::IsPointOnLRLines(GM2DVector Pt, bool bCheckDeg, double dDeg)
{
	if(IsPointOnLLine(Pt, bCheckDeg, dDeg) || IsPointOnRLine(Pt, bCheckDeg, dDeg))
		return true;
	else
		return false;
}

bool MSLinkMemberG::IsPointOnLLine(GM2DVector Pt, bool bCheckDeg, double dDeg)
{
	double dTol = DTOL_MODELING;

	if(mp_LLine == nullptr)
		return false;

	if(mp_LLine->m_ObjType == GT_ARC)
		return mp_LLine->HasOnLine(Pt, dTol);

	double dTolDeg = 1;
	bool isOnLine = false;
	double dDist = mp_LLine->Distance(Pt);
	double dMyDeg = mp_LLine->GetDegree();
	double dInnerDeg = fabs(dMyDeg - dDeg);
	if(bCheckDeg == true)
	{
		if(fabs(dDist) < dTol && (dInnerDeg < dTolDeg || fabs(dInnerDeg - 180) < dTolDeg || fabs(dInnerDeg - 360) < dTolDeg)  )
			isOnLine = true;
	}
	else
	{
		if(fabs(dDist) < dTol )
			isOnLine = true;
	}

	return isOnLine;
}

bool MSLinkMemberG::IsPointOnRLine(GM2DVector Pt, bool bCheckDeg, double dDeg)
{
	double dTol = DTOL_MODELING;

	if(mp_RLine == nullptr)
		return false;

	if (mp_RLine->m_ObjType == GT_ARC)
		return mp_RLine->HasOnLine(Pt, dTol);

	double dTolDeg = 1;
	
	bool isOnLine = false;
	double dDist = mp_RLine->Distance(Pt);
	double dMyDeg = mp_RLine->GetDegree();
	double dInnerDeg = fabs(dMyDeg - dDeg);
	if(bCheckDeg == true)
	{
		if(fabs(dDist) < dTol && (dInnerDeg < dTolDeg || fabs(dInnerDeg - 180) < dTolDeg || fabs(dInnerDeg - 360) < dTolDeg)  )
			isOnLine = true;
	}
	else
	{
		if(fabs(dDist) < dTol )
			isOnLine = true;
	}
	
		
	return isOnLine;
}

long MSLinkMemberG::WhichSide( GM2DVector vec )
{
	double dTol = DTOL1;
	shared_ptr<GM2DCurve> pCurve = GetCenCurve();
	long side = pCurve->WhichSide(vec, dTol);

	return side;
}

bool MSLinkMemberG::IsHorizontalityBarDirX()
{
	shared_ptr<GM2DCurve> pCurve = GetCenCurve();
	double dDeg = pCurve->GetDegree();
	
	if(dDeg >= 0 && dDeg <= 45 || dDeg >= 135 && dDeg <= 225 || dDeg >= 315 && dDeg <= 360)
		return true;
	else 
		return false;
}

double MSLinkMemberG::GetMemberDegree()
{
	shared_ptr<GM2DCurve> pCurve = GetCenCurve();
	double dDeg = pCurve->GetDegree();
	return dDeg;
}

void MSLinkMemberG::ReverseLevelDif()
{
	long NumDif = ma_MemberLevelDif.GetSize();
	for(long iDif = 0 ; iDif < NumDif ; iDif++)
	{
		MSMemberLevelDif* pDif = ma_MemberLevelDif[iDif];
		pDif->ReverseData();
	}
	for(long iDif = 0 ; iDif < NumDif/2 ; iDif++)
	{
		MSMemberLevelDif* pDif = ma_MemberLevelDif[iDif];
		MSMemberLevelDif* pDif2 = ma_MemberLevelDif[NumDif -(1+iDif) ];
		ma_MemberLevelDif[iDif] = pDif2;
		ma_MemberLevelDif[NumDif -(1+iDif) ] = pDif;
	}
}

GM2DPolyline* MSLinkMemberG::GetLinkMemberBoundary(bool bShort)
{
	GM2DPolyline* pPoly = new GM2DPolyline(TRUE);
	GM2DLineVector SLine, ELine;

	GM2DCurve* pLLine = mp_LLine->Copy();
	GM2DCurve* pRLine = mp_RLine->Copy();

	SLine = GetEndLine4Joint2_ExtendToOrthoPart(0, bShort);
	ELine = GetEndLine4Joint2_ExtendToOrthoPart(1, bShort);

	pRLine->SetP0(SLine.m_P1);
	pRLine->SetP1(ELine.m_P1);
	pLLine->SetP0(SLine.m_P0);
	pLLine->SetP1(ELine.m_P0);

	pPoly->AddCopy(SLine.m_P1);
	pPoly->ma_Bulge.Add(pRLine->GetBulge());
	pPoly->AddCopy(ELine.m_P1);
	pPoly->ma_Bulge.Add(0.0);
	pPoly->AddCopy(ELine.m_P0);
	pPoly->ma_Bulge.Add(pLLine->GetBulge() * -1);
	pPoly->AddCopy(SLine.m_P0);
	pPoly->ma_Bulge.Add(0.0);
	pPoly->AddCopy(SLine.m_P1);

	pPoly->MakeCCW();

	delete pLLine;
	delete pRLine;

	return pPoly;
}

GM2DPolyline* MSLinkMemberG::GetLinkMemberTRBoundary()
{
	if (mp_LLineTR == nullptr || mp_RLineTR == nullptr)
		SetTrimmedLines();
	
	GM2DPolyline* pPoly = new GM2DPolyline(TRUE);
	pPoly->AddCopy(mp_RLineTR->m_P0);
	pPoly->ma_Bulge.Add(mp_RLineTR->GetBulge());
	pPoly->AddCopy(mp_RLineTR->m_P1);
	pPoly->ma_Bulge.Add(0.0);
	pPoly->AddCopy(mp_LLineTR->m_P1);
	pPoly->ma_Bulge.Add(mp_LLineTR->GetBulge() * -1);
	pPoly->AddCopy(mp_LLineTR->m_P0);
	pPoly->ma_Bulge.Add(0.0);
	pPoly->AddCopy(mp_RLineTR->m_P0);
	pPoly->MakeCCW();

	return pPoly;
}

vector<GM2DPolyline*> MSLinkMemberG::GetLinkMemberBoundaryTrimmedByColumn()
{
	vector<GM2DPolyline*> polyTRList;
	polyTRList.push_back(GetTrimmedBoundary()->Copy());

	// Column으로 자른다.
	vector<MSCompoG*> columnList = FindConnectMember(FindType_MSElement::msColumnG, 0, DTOL_MODELING, true, false, false);
	vector<MSCompoG*> wallColumnList = FindConnectMember(FindType_MSElement::msWallColumnG, 0, DTOL_MODELING, true, false, false);
	for (MSCompoG* wallColumn : wallColumnList)
		columnList.push_back(wallColumn);

	for (int i = 0; i < columnList.size(); i++)
	{
		MSCompoG* pCompoG = columnList[i];
		if (pCompoG == nullptr)
			continue;
		if (IsLevelTouch(pCompoG))
			continue;

		GM2DPolyline* toolPoly = pCompoG->GetTrimmedBoundary();

		vector<GM2DPolyline*> newPolyList;
		for (int j = 0; j < polyTRList.size(); j++)
		{
			vector<GM2DPolyline*> subPolyList;
			polyTRList[j]->Subtraction_Acis(*toolPoly, subPolyList);
			for (GM2DPolyline* poly : subPolyList)
			{
				poly->DeleteParallelLine();
				newPolyList.push_back(poly);
			}
		}

		for (int j = 0; j < polyTRList.size(); j++)
			delete polyTRList[j];
		polyTRList.clear();
		for (int j = 0; j < newPolyList.size(); j++)
			polyTRList.push_back(newPolyList[j]);
	}

	return polyTRList;
}

// Trim 우선순위에 맞게 잘라낸 Trimmed Boundary
vector<GM2DPolyline*> MSLinkMemberG::GetLinkMemberBoundaryTrimmedByTRBoundary()
{
	vector<GM2DPolyline*> polyTRList = GetLinkMemberBoundaryTrimmedByColumn();

	shared_ptr<GM2DCurve> cenCurve = GetCenCurve();
	GM2DVector cenVec = cenCurve->PointAtParam(0.5);

	// Connection 기준으로 Trim로직에 맞게 Polyline을 자른다.
	// MSAcisShapeTrimmer::TrimWithConnections 참고
	for (int i = 0; i < ma_Connections.size(); i++)
	{
		MSConnection* pOverlap = ma_Connections[i];
		if (pOverlap->GetFirstCompoG() == nullptr) continue;
		if (pOverlap->GetSecondCompoG() == nullptr) continue;

		if (pOverlap->GetFirstCompoG()->GetType() != pOverlap->GetSecondCompoG()->GetType())
			continue;

		MSCompoG* toolCompoG = pOverlap->GetFirstCompoG();
		if (this == toolCompoG)
			continue;
		
		if (IsLevelTouch(toolCompoG))
			continue;

		MSLinkMemberG* pToolLinkG = dynamic_cast<MSLinkMemberG*>(toolCompoG);
		if (pToolLinkG != nullptr)
		{
			if ((isArc() || pToolLinkG->isArc()) || (IsSlopeMember() || pToolLinkG->IsSlopeMember()))
			{
				shared_ptr<GM2DCurve> toolCenCurve = pToolLinkG->GetCenCurve();
				double dToolLoc = toolCenCurve->OrthoBaseLocValue(cenVec);
				double dLoc = 0.5;

				if (isArc() || pToolLinkG->isArc())
				{
					GM2DVector tempVec = toolCenCurve->PointAtParam(dToolLoc);
					double pointDist = tempVec.Distance(cenVec);
					if (pointDist > max(GetWidth(), pToolLinkG->GetWidth()))
						continue;
				}
				else
				{
					if (cenCurve->IsParallel4DTol(&*toolCenCurve) == true)
						continue;
				}

				double dTopLevel = 0, dBotLevel = 0;
				double dToolTopLevel = 0, dToolBotLevel = 0;
				GetZLevelsbyLoc(dTopLevel, dBotLevel, dLoc);
				pToolLinkG->GetZLevelsbyLoc(dToolTopLevel, dToolBotLevel, dToolLoc);

				if (fabs(dTopLevel - dToolBotLevel) < DTOL_MODELING || fabs(dBotLevel - dToolTopLevel) < DTOL_MODELING)
					continue;
			}
		}

		GM2DPolyline* toolPoly = toolCompoG->GetTrimmedBoundary();

		vector<GM2DPolyline*> newPolyList;
		for (int j = 0; j < polyTRList.size(); j++)
		{
			vector<GM2DPolyline*> subPolyList;
			polyTRList[j]->Subtraction_Acis(*toolPoly, subPolyList);
			for (GM2DPolyline* poly : subPolyList)
			{
				poly->DeleteParallelLine();
				newPolyList.push_back(poly);
			}
		}
		
		for (int j = 0; j < polyTRList.size(); j++)
			delete polyTRList[j];
		polyTRList.clear();
		for (int j = 0; j < newPolyList.size(); j++)
			polyTRList.push_back(newPolyList[j]);
	}

	sort(polyTRList.begin(), polyTRList.end(),
		[](GM2DPolyline* a, GM2DPolyline* b)->bool {return a->Area_ACIS() > b->Area_ACIS(); }
	);

	return polyTRList;
}

shared_ptr<GM2DCurve> MSLinkMemberG::GetExtendLine(char cLineType)
{
	shared_ptr<GM2DCurve> pBaseCurve;
	if(cLineType == 'C')
		pBaseCurve = GetCenCurve();
	else if(cLineType == 'L')
		pBaseCurve = shared_ptr<GM2DCurve>(mp_LLine->Copy());
	else if(cLineType == 'R')
		pBaseCurve = shared_ptr<GM2DCurve>(mp_RLine->Copy());
	else
		return nullptr;

	vector<GM2DVector> CrossVecs;
	vector<double> MyLocArr, UrLocArr;
	vector<long> ResultArr;
	vector<MSCompoG*> LinkGArr;

	vector<MSConnection*> SConnArr = GetConnectionByPos(CON_START);
	vector<MSConnection*> EConnArr = GetConnectionByPos(CON_END);

	double dSLoc = 0;
	double dELoc = 1;

	for(long iConn = 0 ; iConn < SConnArr.size() ; iConn++)
	{
		MSConnection* pConn = SConnArr[iConn];
		MSCompoG* pPartner = pConn->GetPartner(this);
		if(pPartner == nullptr || pPartner->IsLinkMember() == FALSE)
			continue;
		
		MSLinkMemberG* pLinkG = (MSLinkMemberG*)pPartner;
		if(IsParallel(pLinkG))
			continue;

		long nRval = pBaseCurve->IntersectEnd2(pLinkG->mp_LLine, CrossVecs, MyLocArr, UrLocArr, ResultArr, true);
		if(abs(nRval%10)==1) // 내부 교차
		{
			for(long iLoc =0 ; iLoc < MyLocArr.size() ; iLoc++)
				dSLoc = min(dSLoc, MyLocArr[iLoc]);
		}

		nRval = pBaseCurve->IntersectEnd2(pLinkG->mp_RLine, CrossVecs, MyLocArr, UrLocArr, ResultArr, true);
		if(abs(nRval%10)==1) // 내부 교차
		{
			for(long iLoc =0 ; iLoc < MyLocArr.size() ; iLoc++)
				dSLoc = min(dSLoc, MyLocArr[iLoc]);
		}
	}

	for(long iConn = 0 ; iConn < EConnArr.size() ; iConn++)
	{
		MSConnection* pConn = EConnArr[iConn];
		MSCompoG* pPartner = pConn->GetPartner(this);
		if(pPartner->IsLinkMember() == FALSE)
			continue;

		MSLinkMemberG* pLinkG = (MSLinkMemberG*)pPartner;
		if(IsParallel(pLinkG))
			continue;

		long nRval = pBaseCurve->IntersectEnd2(pLinkG->mp_RLine, CrossVecs, MyLocArr, UrLocArr, ResultArr, true);
		if(abs(nRval%10)==1) // 내부 교차
		{
			for(long iLoc =0 ; iLoc < MyLocArr.size() ; iLoc++)
				dELoc = max(dELoc, MyLocArr[iLoc]);
		}

		nRval = pBaseCurve->IntersectEnd2(pLinkG->mp_RLine, CrossVecs, MyLocArr, UrLocArr, ResultArr, true);
		if(abs(nRval%10)==1) // 내부 교차
		{
			for(long iLoc =0 ; iLoc < MyLocArr.size() ; iLoc++)
				dELoc = max(dELoc, MyLocArr[iLoc]);
		}
	}

	GM2DVector SVec = pBaseCurve->PointAtParam(dSLoc);
	GM2DVector EVec = pBaseCurve->PointAtParam(dELoc);

	pBaseCurve->SetP0(SVec);
	pBaseCurve->SetP1(EVec);

	return pBaseCurve;
}

vector<MSCompoG*> MSLinkMemberG::FindConnectMember(FindType_MSElement memberType, long nFloorType, double dTol, bool bUseTrimedBoundry, bool bFindPolyMemberOnlyOnLine, bool bIncludeOnLine)
{
	return MSCompoGUtil::FindConnectMember(this, CON_ALL, memberType, nFloorType, false, dTol, bUseTrimedBoundry, bFindPolyMemberOnlyOnLine, bIncludeOnLine);
}

vector<MSCompoG*> MSLinkMemberG::FindConnectMember(char cLoc, FindType_MSElement memberType, long nFloorType, bool isReverse, double dTol, bool bUseTrimedBoundry, bool bFindPolyMemberOnlyOnLine)
{
	CON_POS eConPos = MSCompoGUtil::ToConPos(cLoc);
	return MSCompoGUtil::FindConnectMember(this, eConPos, memberType, nFloorType, isReverse, dTol, bUseTrimedBoundry, bFindPolyMemberOnlyOnLine);
}

vector<MSCompoG*> MSLinkMemberG::FindConnectMember(CON_POS eConPos, FindType_MSElement memberType, long nFloorType, bool isReverse, double dTol, bool bUseTrimedBoundry, bool bFindPolyMemberOnlyOnLine, bool bIncludeOnLine)
{
	return MSCompoGUtil::FindConnectMember(this, eConPos, memberType, nFloorType, isReverse, dTol, bUseTrimedBoundry, bFindPolyMemberOnlyOnLine, bIncludeOnLine);
}

vector<MSCompoG*> MSLinkMemberG::FindColinerLinkMember(char cLoc, FindType_MSElement memberType, long nFloorType, double dTolDeg, double DistTol, double LinkTestLength, bool isReverse, bool bUseTrimedBoundry)
{
	CON_POS eConPos = MSCompoGUtil::ToConPos(cLoc);	
	return MSCompoGUtil::FindColinerLinkMember(this, eConPos, memberType, nFloorType, dTolDeg, DistTol, LinkTestLength, isReverse, bUseTrimedBoundry);
}

vector<MSCompoG*> MSLinkMemberG::FindColinerLinkMember(CON_POS eConPos, FindType_MSElement memberType, long nFloorType, double dTolDeg, double DistTol, double LinkTestLength, bool isReverse, bool bUseTrimedBoundry)
{
	return MSCompoGUtil::FindColinerLinkMember(this, eConPos, memberType, nFloorType, dTolDeg, DistTol, LinkTestLength, isReverse, bUseTrimedBoundry);
}

vector<MSCompoG*> MSLinkMemberG::FindOverlapLinkMember(FindType_MSElement memberType, long nFloorType, double dTolDeg, double DistTol, double LinkTestLength, bool bCheckMemberWidth, bool bUseTrimedBoundry, bool bIncludeOnLine)
{
	return MSCompoGUtil::FindOverlapLinkMember(this, memberType, nFloorType, dTolDeg, DistTol, LinkTestLength, bCheckMemberWidth, bUseTrimedBoundry, bIncludeOnLine);
}

void MSLinkMemberG::GetPolyBoundary(double dSLoc, double dELoc, GM2DPolyline* pPoly)
{
	pPoly->ResetPolyline();

	GM2DVector SLVec = mp_LLine->PointAtParam(dSLoc);
	GM2DVector ELVec = mp_LLine->PointAtParam(dELoc);
	GM2DVector SRVec = mp_RLine->PointAtParam(dSLoc);
	GM2DVector ERVec = mp_RLine->PointAtParam(dELoc);
	if (isArc() == false)
	{
		pPoly->AddCopy(SRVec);
		pPoly->ma_Bulge.Add(0);
		pPoly->AddCopy(ERVec);
		pPoly->ma_Bulge.Add(0);
		pPoly->AddCopy(ELVec);
		pPoly->ma_Bulge.Add(0);
		pPoly->AddCopy(SLVec);
		pPoly->ma_Bulge.Add(0);
		pPoly->AddCopy(SRVec);
	}
	else
	{
		GM2DArc LArc, RArc;
		LArc.CopyFrom(mp_LLine);
		RArc.CopyFrom(mp_RLine);
		LArc.SetP0(SLVec);
		LArc.SetP1(ELVec);
		RArc.SetP0(SRVec);
		RArc.SetP1(ERVec);
		pPoly->AddCopy(SRVec);
		pPoly->ma_Bulge.Add(RArc.GetBulge());
		pPoly->AddCopy(ERVec);
		pPoly->ma_Bulge.Add(0);
		pPoly->AddCopy(ELVec);
		pPoly->ma_Bulge.Add(LArc.GetBulge() * -1);
		pPoly->AddCopy(SLVec);
		pPoly->ma_Bulge.Add(0);
		pPoly->AddCopy(SRVec);
	}
	pPoly->MakeCCW();
}

vector<GM2DPolyline*> MSLinkMemberG::GetLineLevelDifPolyLineArr()
{
	double dTol = DTOL1;
	vector<GM2DPolyline*> Joined_Polys;
	GM2DPolyline* pTrimmedPoly = GetTrimmedBoundary();
	GM2DPolyline* pPartLinkPoly = nullptr;
	vector<GM2DPolyline*> RValPolyArr;
	double dSLoc = -1.5;
	double dELoc = 1.5;

	long NumDif = ma_MemberLevelDif.GetSize();
	for (long iDif = 0; iDif < NumDif; iDif++)
	{
		MSMemberLevelDif* pMemberDif = ma_MemberLevelDif[iDif];
		if (pMemberDif == nullptr)
			continue;
		dELoc = pMemberDif->m_Loc;

		pPartLinkPoly = new GM2DPolyline();
		GetPolyBoundary(dSLoc, dELoc, pPartLinkPoly);
		pTrimmedPoly->BoolOperation('I', pPartLinkPoly, Joined_Polys, dTol);
		if (Joined_Polys.size() > 0)
		{
			RValPolyArr.push_back(Joined_Polys[0]);
			Joined_Polys.erase(Joined_Polys.begin());
			GMObject::DeleteGMObject(Joined_Polys);
		}
		delete pPartLinkPoly;

		dSLoc = dELoc;
	}
	dELoc = 1.5;

	pPartLinkPoly = new GM2DPolyline();
	GetPolyBoundary(dSLoc, dELoc, pPartLinkPoly);
	pTrimmedPoly->BoolOperation('I', pPartLinkPoly, Joined_Polys, dTol);
	if (Joined_Polys.size() > 0)
	{
		RValPolyArr.push_back(Joined_Polys[0]);
		Joined_Polys.erase(Joined_Polys.begin());
		GMObject::DeleteGMObject(Joined_Polys);
	}
	delete pPartLinkPoly;

	return RValPolyArr;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////

MSColineLinkMemberGs::MSColineLinkMemberGs()
{

}

MSColineLinkMemberGs::~MSColineLinkMemberGs()
{

}

int MSColineLinkMemberGs::Add(MSLinkMemberG* pLinkG)
{
	if(pLinkG == nullptr)
		return 0;

	if(Find(pLinkG))
		return 0;
	mm_LinkMem.insert(make_pair(pLinkG->m_ID, pLinkG));
	return mm_LinkMem.size();
}

bool MSColineLinkMemberGs::Find(MSLinkMemberG* pLinkG)
{
	if(pLinkG == nullptr)
		return false;

	if(mm_LinkMem.find(pLinkG->m_ID) != mm_LinkMem.end())
		return true;
	return false;
}

template<template <typename> class P = std::greater >
struct compare_pair_CompoMDataStr {

	template<class T1, class T2> bool operator()(const std::pair<T1,T2>&left, const std::pair<T1,T2>&right) {
		CompoMDataStr compoMDataStr1 = left.second;
		CompoMDataStr compoMDataStr2 = right.second;
		return compoMDataStr1.nCnt > compoMDataStr2.nCnt;
	}
};

double MSColineLinkMemberGs::GetMostlyWidth()
{
	map<double, CompoMDataStr> CompoMDataStrMap;

	map<long, MSLinkMemberG*>::iterator it;
	for(it = mm_LinkMem.begin() ; it != mm_LinkMem.end() ; it++)
	{
		MSLinkMemberG* pLinkMem = it->second;
		double dWidth = pLinkMem->GetWidth();

		map<double, CompoMDataStr>::iterator it;
		it = CompoMDataStrMap.find(dWidth);
		if(it == CompoMDataStrMap.end())
		{
			CompoMDataStr compoMData;
			compoMData.dData = dWidth;
			compoMData.nCnt = 1;
			CompoMDataStrMap.insert(make_pair(dWidth, compoMData));
		}
		else
			it->second.nCnt++;
	}

	vector<std::pair<double, CompoMDataStr> > CompoMDataStrArr(CompoMDataStrMap.begin(), CompoMDataStrMap.end());
	sort(CompoMDataStrArr.begin(), CompoMDataStrArr.end(), compare_pair_CompoMDataStr<std::greater>());

	vector<std::pair<double, CompoMDataStr> >::iterator itdata;

	for(itdata = CompoMDataStrArr.begin() ; itdata != CompoMDataStrArr.end() ; itdata++)
	{
		CompoMDataStr compoMData = itdata->second;
		return compoMData.dData;
	}
	
	return 0;
}

double MSColineLinkMemberGs::GetMostlyDepth()
{
	map<double, CompoMDataStr> CompoMDataStrMap;

	map<long, MSLinkMemberG*>::iterator it;
	for(it = mm_LinkMem.begin() ; it != mm_LinkMem.end() ; it++)
	{
		MSLinkMemberG* pLinkMem = it->second;
		double dDepth = pLinkMem->GetDepth();

		map<double, CompoMDataStr>::iterator it;
		it = CompoMDataStrMap.find(dDepth);
		if(it == CompoMDataStrMap.end())
		{
			CompoMDataStr compoMData;
			compoMData.dData = dDepth;
			compoMData.nCnt = 1;
			CompoMDataStrMap.insert(make_pair(dDepth, compoMData));
		}
		else
			it->second.nCnt++;
	}

	vector<std::pair<double, CompoMDataStr> > CompoMDataStrArr(CompoMDataStrMap.begin(), CompoMDataStrMap.end());
	sort(CompoMDataStrArr.begin(), CompoMDataStrArr.end(), compare_pair_CompoMDataStr<std::greater>());

	vector<std::pair<double, CompoMDataStr> >::iterator itdata;

	for(itdata = CompoMDataStrArr.begin() ; itdata != CompoMDataStrArr.end() ; itdata++)
	{
		CompoMDataStr compoMData = itdata->second;
		return compoMData.dData;
	}

	return 0;
}

double MSColineLinkMemberGs::GetMostlyZLevel()
{
	map<double, CompoMDataStr> CompoMDataStrMap;

	map<long, MSLinkMemberG*>::iterator it;
	for(it = mm_LinkMem.begin() ; it != mm_LinkMem.end() ; it++)
	{
		MSLinkMemberG* pLinkMem = it->second;
		double dTopLevel = 0;
		double dBotLevel = 0;
		map<double, CompoMDataStr>::iterator it;

		long NumDif = pLinkMem->ma_MemberLevelDif.GetSize();

		if(NumDif == 0)
		{
			//Get Start Joint ZLevel // Top만 검토 한다. Bot은 Depth 값 적용된 값이다.
			pLinkMem->GetZLevelsbyLoc(dTopLevel, dBotLevel, 0);

			it = CompoMDataStrMap.find(dTopLevel);
			if(it == CompoMDataStrMap.end())
			{
				CompoMDataStr compoMData(dTopLevel, 1);
				CompoMDataStrMap.insert(make_pair(dTopLevel, compoMData));
			}
			else
				it->second.nCnt++;

			//Get End Joint ZLevel // Top만 검토 한다. Bot은 Depth 값 적용된 값이다.
			pLinkMem->GetZLevelsbyLoc(dTopLevel, dBotLevel, 1);

			it = CompoMDataStrMap.find(dTopLevel);
			if(it == CompoMDataStrMap.end())
			{
				CompoMDataStr compoMData(dTopLevel, 1);
				CompoMDataStrMap.insert(make_pair(dTopLevel, compoMData));
			}
			else
				it->second.nCnt++;
		}
		else
		{
			for(long iDif = 0 ; iDif < NumDif ; iDif++)
			{
				MSMemberLevelDif* pDif = pLinkMem->ma_MemberLevelDif[iDif];
				double dOffSet = pLinkMem->GetOffSetbyLoc(pDif->m_Loc);
				it = CompoMDataStrMap.find(pDif->m_StartLevel + dOffSet + pLinkMem->GetNextSlabLevel());
				if(it == CompoMDataStrMap.end())
				{
					CompoMDataStr compoMData(pDif->m_StartLevel + dOffSet + pLinkMem->GetNextSlabLevel(), 1);
					CompoMDataStrMap.insert(make_pair(pDif->m_StartLevel + dOffSet + pLinkMem->GetNextSlabLevel(), compoMData));
				}
				else
					it->second.nCnt++;

				if(iDif == NumDif -1)
				{
					it = CompoMDataStrMap.find(pDif->m_EndLevel + dOffSet + pLinkMem->GetNextSlabLevel());
					if(it == CompoMDataStrMap.end())
					{
						CompoMDataStr compoMData(pDif->m_EndLevel + dOffSet + pLinkMem->GetNextSlabLevel(), 1);
						CompoMDataStrMap.insert(make_pair(pDif->m_EndLevel + dOffSet + pLinkMem->GetNextSlabLevel(), compoMData));
					}
					else
						it->second.nCnt++;
				}
			}

		}
	}

	vector<std::pair<double, CompoMDataStr> > CompoMDataStrArr(CompoMDataStrMap.begin(), CompoMDataStrMap.end());
	sort(CompoMDataStrArr.begin(), CompoMDataStrArr.end(), compare_pair_CompoMDataStr<std::greater>());

	vector<std::pair<double, CompoMDataStr> >::iterator itdata;

	for(itdata = CompoMDataStrArr.begin() ; itdata != CompoMDataStrArr.end() ; itdata++)
	{
		CompoMDataStr compoMData = itdata->second;
		return compoMData.dData;
	}

	return 0;
}

bool MSLinkMemberG::CheckExtendOverlap(MSCompoG* pUrCompoG, bool isUpdateGeometry)
{
	if(pUrCompoG->IsLinkMember() == FALSE)
		return false;

	MSLinkMemberG* pLinkG = this;
	MSLinkMemberG* pUrLinkG = (MSLinkMemberG*)pUrCompoG;

	double dSExtendDist = MSFloor::ms_LineHitDistTol;
	double dEExtendDist = MSFloor::ms_LineHitDistTol;
	bool IsSVecExtend = false;
	bool IsEVecExtend = false;
	bool bCheckDetail = true;

	GM2DLineVector SLine, ELine;
	SLine.m_P0 = pLinkG->mp_LLine->m_P0;
	SLine.m_P1 = pLinkG->mp_RLine->m_P0;
	ELine.m_P0 = pLinkG->mp_LLine->m_P1;
	ELine.m_P1 = pLinkG->mp_RLine->m_P1;

	long nExtendType = 0;
	GM2DPolyline MyExtBoundary(TRUE);
	pLinkG->MakeExtendedBoundary(MyExtBoundary, nExtendType, dSExtendDist);

	if (MyExtBoundary.GetNumLines() < 3)
		return false;

	GM2DCurve* pExSLine = MyExtBoundary.GetCurve(3);
	GM2DCurve* pExELine = MyExtBoundary.GetCurve(1);
	pExELine->ReverseDirection();

	GM2DPolyline MyStartExtBoundary2(TRUE);
	MyStartExtBoundary2.AddCopy(SLine.m_P0);
	MyStartExtBoundary2.AddCopy(SLine.m_P1);
	MyStartExtBoundary2.ma_Bulge.Add(0);
	MyStartExtBoundary2.AddCopy(pExSLine->m_P1);
	MyStartExtBoundary2.ma_Bulge.Add(0);
	MyStartExtBoundary2.AddCopy(pExSLine->m_P0);
	MyStartExtBoundary2.ma_Bulge.Add(0);
	MyStartExtBoundary2.AddCopy(SLine.m_P0);
	MyStartExtBoundary2.ma_Bulge.Add(0);

	GM2DPolyline MyEndExtBoundary2(TRUE);
	MyEndExtBoundary2.AddCopy(ELine.m_P0);
	MyEndExtBoundary2.AddCopy(ELine.m_P1);
	MyEndExtBoundary2.ma_Bulge.Add(0);
	MyEndExtBoundary2.AddCopy(pExELine->m_P1);
	MyEndExtBoundary2.ma_Bulge.Add(0);
	MyEndExtBoundary2.AddCopy(pExELine->m_P0);
	MyEndExtBoundary2.ma_Bulge.Add(0);
	MyEndExtBoundary2.AddCopy(ELine.m_P0);
	MyEndExtBoundary2.ma_Bulge.Add(0);

	delete pExSLine;
	delete pExELine;

	GM2DPolyline* pTrimedBoundry = pUrLinkG->GetTrimmedBoundary();

	if(pTrimedBoundry->OverlapEx(MyStartExtBoundary2))
		IsSVecExtend = true;
	if(pTrimedBoundry->OverlapEx(MyEndExtBoundary2))
		IsEVecExtend = true;

	/*
	GM2DLineVector SLine2 = SLine;
	GM2DLineVector ELine2 = ELine;

	GM2DLineVector CenLVec = GetCenLVec();
	GM2DVector DirVec = CenLVec.UnitVector();
	DirVec = DirVec * dSExtendDist;
	SLine2.m_P0 = SLine2.m_P0 - DirVec;
	SLine2.m_P1 = SLine2.m_P1 - DirVec;
	ELine2.m_P0 = SLine2.m_P0 + DirVec;
	ELine2.m_P1 = SLine2.m_P1 + DirVec;

	GM2DPolyline MyStartExtBoundary2(TRUE);
	MyStartExtBoundary2.AddCopy(SLine.m_P0);
	MyStartExtBoundary2.AddCopy(SLine.m_P1);
	MyStartExtBoundary2.ma_Bulge.Add(0);

	MyStartExtBoundary2.AddCopy(SLine2.m_P1);
	MyStartExtBoundary2.ma_Bulge.Add(0);
	MyStartExtBoundary2.AddCopy(SLine2.m_P0);
	MyStartExtBoundary2.ma_Bulge.Add(0);
	MyStartExtBoundary2.AddCopy(SLine.m_P0);
	MyStartExtBoundary2.ma_Bulge.Add(0);

	GM2DPolyline MyEndExtBoundary2(TRUE);
	MyEndExtBoundary2.AddCopy(ELine.m_P0);
	MyEndExtBoundary2.AddCopy(ELine.m_P1);
	MyEndExtBoundary2.ma_Bulge.Add(0);
	MyEndExtBoundary2.AddCopy(ELine2.m_P1);
	MyEndExtBoundary2.ma_Bulge.Add(0);
	MyEndExtBoundary2.AddCopy(ELine2.m_P0);
	MyEndExtBoundary2.ma_Bulge.Add(0);
	MyEndExtBoundary2.AddCopy(ELine.m_P0);
	MyEndExtBoundary2.ma_Bulge.Add(0);

	GM2DPolyline* pTrimedBoundry = pUrLinkG->GetTrimmedBoundary();

	if(pTrimedBoundry->OverlapEx(MyStartExtBoundary2))
	IsSVecExtend = true;
	if(pTrimedBoundry->OverlapEx(MyEndExtBoundary2))
	IsEVecExtend = true;
	*/

	shared_ptr<GM2DCurve> pCenCurve = pLinkG->GetCenCurve();

	if(isUpdateGeometry)
	{
		if(IsSVecExtend)
		{
			if(bCheckDetail)
			{
				double dMinDist = dSExtendDist;
				for(long i = 0 ; i < pTrimedBoundry->ma_Point.GetSize() -1 ; i++)
					dMinDist = min(dMinDist, SLine.Distance(*(pTrimedBoundry->ma_Point[i])));
				dSExtendDist = dMinDist;
			}

			pCenCurve->ExtendLineVector(TRUE, dSExtendDist + MSFloor::ms_DefLineHitDistTol, FALSE);
			pLinkG->ma_Vec[0]->SetXY(pCenCurve->m_P0.m_X, pCenCurve->m_P0.m_Y);
		}
		if(IsEVecExtend)
		{
			if(bCheckDetail)
			{
				double dMinDist = dEExtendDist;
				for(long i = 0 ; i < pTrimedBoundry->ma_Point.GetSize() -1 ; i++)
					dMinDist = min(dMinDist, ELine.Distance(*(pTrimedBoundry->ma_Point[i])));
				dEExtendDist = dMinDist;
			}

			pCenCurve->ExtendLineVector(FALSE, dEExtendDist + MSFloor::ms_DefLineHitDistTol, TRUE);
			pLinkG->ma_Vec[1]->SetXY(pCenCurve->m_P1.m_X, pCenCurve->m_P1.m_Y);
		}

		if(IsSVecExtend || IsEVecExtend)
		{
			pLinkG->SetLines();
			if(pLinkG->GetTrimmedBoundary() != nullptr)
				pLinkG->GetBoundary(*pLinkG->GetTrimmedBoundary());
			return true;
		}
	}
	return false;
}

eLINEALIGN_TYPE MSLinkMemberG::GetLineAlignType()
{
	return m_eLineAlign;
}
void MSLinkMemberG::SetLineAlignType(eLINEALIGN_TYPE eType, bool bUpdateCurveInput)
{
	m_eLineAlign = eType;
	
	/*
	if(bUpdateCurveInput)
		UpdateCurveInputByCenCurve();
	UpdateCenCurveByCurveInput();
	*/
}

void MSLinkMemberG::UpdateVecInput()
{
	UpdateCurveInputByCenCurve();
}

void MSLinkMemberG::UpdateVecCenter()
{
	UpdateCenCurveByCurveInput();
}

// Add / Modify 시 항상 불림 (Input 점을 계산해서 가지고 있기 위함)
void MSLinkMemberG::UpdateCurveInputByCenCurve()
{
	double dDist = 0;
	double dWidth = GetWidth();
	shared_ptr<GM2DCurve> pCurve = GetCenCurve();
	if(pCurve == nullptr)
		return;

	if(m_eLineAlign == LINEALIGN_LEFT)
	{
		dDist = dWidth/2;
		pCurve->NormalTranslateMe(dDist);
	}
	else if(m_eLineAlign == LINEALIGN_RIGHT)
	{
		dDist = -dWidth/2;
		pCurve->NormalTranslateMe(dDist);
	}

	GM2DCurve* pNewCurve = pCurve->Copy();
	SetCurve_Input(pNewCurve);

	delete pNewCurve;
}

// 특별한 상황에서만 불림 (Center 를 재계산 해야 하는 경우 ex> m 변경 등)
void MSLinkMemberG::UpdateCenCurveByCurveInput()
{
	double dDist = 0;
	double dWidth = GetWidth();
	shared_ptr<GM2DCurve> pCurve = GetCurve_Input();
	if(pCurve == nullptr)
		return;

	if(m_eLineAlign == LINEALIGN_LEFT)
	{
		dDist = -dWidth/2;
		pCurve->NormalTranslateMe(dDist);
	}
	else if(m_eLineAlign == LINEALIGN_RIGHT)
	{
		dDist = dWidth/2;
		pCurve->NormalTranslateMe(dDist);
	}

	GM2DCurve* pNewCurve = pCurve->Copy();
	SetCenCurve(pNewCurve);

	//delete pNewCurve;
}

bool MSLinkMemberG::SetCompoM(long MID, bool bUpdateVecInput)
{
	bool bRval = MSCompoG::SetCompoM(MID);
	if(bRval == false)
		return false;
	
	if(bUpdateVecInput)
	{
		UpdateCenCurveByCurveInput();
		SetLines();
	}
	return true;
}

bool MSLinkMemberG::SetCompoM(MSCompoM* pCompoM, bool bUpdateVecInput)
{
	if(pCompoM == nullptr)
		return false;

	bool bRval = MSCompoG::SetCompoM(pCompoM);
	if(bRval == false)
		return false;

	if(bUpdateVecInput)
	{
		UpdateCenCurveByCurveInput();
		SetLines();
	}
	return true;
}

double MSLinkMemberG::GetCenCurveLocbyConntion(bool isStart, bool isMaxLoc)
{
	double dRLoc = CalcCenCurveLocbyConntion(isStart, isMaxLoc);
	if (isStart == true && dRLoc > 1 - DTOL_GM)
	{
		CalculateConnectionsAngle();
		dRLoc = CalcCenCurveLocbyConntion(isStart, isMaxLoc);
	}
	else if(isStart == false && dRLoc < 0 + DTOL_GM)
	{
		CalculateConnectionsAngle();
		dRLoc = CalcCenCurveLocbyConntion(isStart, isMaxLoc);
	}
	return dRLoc;
}

double MSLinkMemberG::CalcCenCurveLocbyConntion(bool isStart, bool isMaxLoc)
{
	double dRLoc = 0;
	vector<MSConnection*> ConnArr;
	if (isStart == true)
	{
		dRLoc = 0;
		ConnArr = GetConnectionByPos(CON_START);
	}
	else
	{
		dRLoc = 1;
		ConnArr = GetConnectionByPos(CON_END);
	}

	long NumConn = ConnArr.size();
	if (NumConn == 0)
		return dRLoc;

	vector<MSConnection*> PointConnArr;
	for (long i = NumConn - 1; i >= 0; i--)
	{
		MSConnection* pConn = ConnArr[i];
		MSCompoG* pPartherG = pConn->GetPartner(this);
		if(pPartherG == nullptr)
			continue;
		if (pPartherG->IsPointMember() && pPartherG->IsLevelOverlap(this))
		{
			PointConnArr.push_back(pConn);
			ConnArr.erase(ConnArr.begin() + i);
		}
		else if(pConn->IsParallel())
			ConnArr.erase(ConnArr.begin() + i);
	}

	NumConn = ConnArr.size();
	for (long i = 0; i < NumConn; i++)
	{
		MSConnection* pConn = ConnArr[i];
		if (pConn->IsParallel())
		{
			if (PointConnArr.size() == 0)
				continue;
		}
		else
		{
			MSConnection::TRIM_OPTION MyTrimOpt = pConn->GetTrimOption(this);
			if (MyTrimOpt == MSConnection::TO_IGNORE || MyTrimOpt == MSConnection::TO_ORIGIN)
				continue;
		}

		double dMyLoc = pConn->GetLinkMemberLoc(this);
		if (i == 0)
			dRLoc = dMyLoc;

		if (isMaxLoc)
			dRLoc = max(dRLoc, dMyLoc);
		else
			dRLoc = min(dRLoc, dMyLoc);
	}

	// 라인 Connection이 없고 점 Connection만 있는 경우 점Connection 위치까지 계산한다.
// 	if (NumConn == 0)
// 	{
// 		for (long i = 0; i < PointConnArr.size(); i++)
// 		{
// 			MSConnection* pConn = PointConnArr[i];
// 			double dMyLoc = pConn->GetLinkMemberLoc(this);
// 			if (i == 0)
// 				dRLoc = dMyLoc;
// 
// 			if (isMaxLoc)
// 				dRLoc = max(dRLoc, dMyLoc);
// 			else
// 				dRLoc = min(dRLoc, dMyLoc);
// 		}
// 	}
	return dRLoc;
}

shared_ptr<GM2DCurve> MSLinkMemberG::GetLongCenCurvebyTrimmedBoundary()
{
	shared_ptr<GM2DCurve> CenCurve = GetCenCurve();
	if (IsSlopeMember())
		return CenCurve;

	double dSLoc = 0, dELoc = 1;
	GM2DPolyline* pPoly = GetTrimmedBoundary_Acis();
	pPoly->GetPointLocByCurve(&*CenCurve, dSLoc, dELoc);

	GM2DVector SVec = CenCurve->PointAtParam(dSLoc);
	GM2DVector EVec = CenCurve->PointAtParam(dELoc);

	if (SVec.Distance(EVec))
	{
		CenCurve->SetP0(SVec);
		CenCurve->SetP1(EVec);
	}
	else
		ASSERT(0);

	return CenCurve;
}

shared_ptr<GM2DCurve> MSLinkMemberG::GetLongCenCurveTrimmedByTRBoundary()
{
	// 초기값은 트림라인 기준 긴 Curve를 가지고 와서 이를 트림 Polyline으로 자른다.
	shared_ptr<GM2DCurve> cenCurve = GetLongCenCurvebyTrimmedBoundary();
	vector<GM2DPolyline*> polyTRLine = GetLinkMemberBoundaryTrimmedByTRBoundary();

	vector<double> sLocArr;
	vector<double> eLocArr;

	double cenLen = cenCurve->Length();
	if (cenLen == 0)
		return cenCurve;

	double minLen = 10;
	double minLoc = 10 / cenLen;
	// BH-4521 polyTRLine이 여러개인 경우에는 계산하지 않는다.
	// A24-39 이슈 처리하면서 AcisGeomHelper::DeleteSmallLumps()로직이 변경되었다 minAreaSum 값이 작아짐
	// polyTRLine이 여러게인 경우에도 루프 돌면서 처리하도록 수정함
	for (int i = 0; i < polyTRLine.size(); i++)
	{
		GM2DPolyline* pPoly = polyTRLine[i];
		// Trimmed Boundary 기준으로 Center Curve를 자른다.
		vector<GM2DVector> crossVecArr;
		vector<double> myLocArr, urLocArr;
		vector<double> locArr;
		vector<long> resultArr;
		double dCurSLoc = -1, dCurELoc = 2;
		int nLine = pPoly->GetNumLines();
		for (int i = 0; i < nLine; i++)
		{
			// 리턴값
			//	-1 : 외부 1점 교차
			//	-2 : 외부 2점 교차
			//	 1 : 1점 내부 교차
			//	 2 : 2점 내부 교차
			//  11 : 1점 내부, 1점 외부 교차
			GM2DCurve* polyCurve = pPoly->GetCurve(i);
			long result = cenCurve->IntersectEnd2(polyCurve, crossVecArr, myLocArr, urLocArr, resultArr, true);
			if ((result == 1 || result == 2 || result == 11) && myLocArr.size() == resultArr.size())
			{
				for (int j = 0; j < myLocArr.size(); j++)
				{
					// resultArr
					// -1 : Colinear 
					// -2 : Parallel
					//  1 : Intersect at 1 point, 내부 교차
					//  2 : not Parallel and not intersection, 외부 교차
					//  0 : 길이 제로 라인벡터 포함 에러
					if (resultArr[j] == 1)
						locArr.push_back(myLocArr[j]);
				}
			}
			delete polyCurve;
		}

		long nCnt = locArr.size();
		if (nCnt == 1)
		{
			double dLoc = locArr[0];
			if (dLoc < 0.5)
				sLocArr.push_back(dLoc);
			else
				eLocArr.push_back(dLoc);
		}
		else if (nCnt > 1)
		{
			sort(locArr.begin(), locArr.end(),
				[](const double& a, const double& b)->bool {return a < b; }
			);

			dCurSLoc = locArr[0];
			dCurELoc = locArr[nCnt-1];

			if (dCurELoc - dCurSLoc > minLoc)
			{
				sLocArr.push_back(dCurSLoc);
				eLocArr.push_back(dCurELoc);
			}
		}
	}

	double dSLoc = 0;
	if (sLocArr.size() > 0)
		dSLoc = sLocArr[0];
	for (int idx = 0; idx < sLocArr.size(); idx++)
		dSLoc = min(dSLoc, sLocArr[idx]);

	double dELoc = 1;
	if (eLocArr.size() > 0)
		dELoc = eLocArr[0];

	for (int idx = 0; idx < eLocArr.size(); idx++)
		dELoc = max(dELoc, eLocArr[idx]);

	if (dSLoc < 0)
		dSLoc = 0;
	if (dELoc > 1)
		dELoc = 1;
	if (dSLoc < dELoc)
	{
		GM2DVector SVec = cenCurve->PointAtParam(dSLoc);
		GM2DVector EVec = cenCurve->PointAtParam(dELoc);

		if (SVec.Distance(EVec) > 0)
		{
			cenCurve->SetP0(SVec);
			cenCurve->SetP1(EVec);
		}
	}

	for (int i = 0; i < polyTRLine.size(); i++)
		delete polyTRLine[i];

	return cenCurve;
}

shared_ptr<GM2DCurve> MSLinkMemberG::GetCenCurvebyConntion()
{
	shared_ptr<GM2DCurve> CenCurve = GetCenCurve();
	if (IsSlopeMember())
		return CenCurve;

	double dSLoc = GetCenCurveLocbyConntion(true, false);
	GM2DVector SVec = CenCurve->PointAtParam(dSLoc);
	double dELoc = GetCenCurveLocbyConntion(false, true);
	GM2DVector EVec = CenCurve->PointAtParam(dELoc);
	
	if (SVec.Distance(EVec))
	{
		CenCurve->SetP0(SVec);
		CenCurve->SetP1(EVec);
	}
	else
		ASSERT(0);

	return CenCurve;
}

vector<MSCompoG*> MSLinkMemberG::MakeSplitLinkMemberByColumn(double dMinSplitLen)
{
	MSFloor* pFloor = GetFloor();
	vector<MSCompoG*> LinkGArr;
	if (MSFloor::GetSplitLinkMember() == false || pFloor == nullptr)
	{
		LinkGArr.push_back(this);
		return LinkGArr;
	}
	
	//CheckAddbySpliteLinkMember 사용하면 반드시 CheckRemoveSpliteLinkMember 사용해야함
	pFloor->PreCheckAddbySpliteLinkMember(this);

	double dMinSplitLoc = dMinSplitLen / GetLength();
	vector<double> LocArr;
	vector<MSConnection*> ConnArr = GetConnectionByPos(CON_MID);
	for (long i = ConnArr.size() - 1; i >= 0; i--)
	{
		MSConnection* pConn = ConnArr[i];
		MSCompoG* pUrCompoG = pConn->GetPartner(this);
		if(pUrCompoG == nullptr)
			continue;
		if (pUrCompoG->GetType() != MSElement::msColumnG)
			ConnArr.erase(ConnArr.begin() + i);
	}
	// 기둥끼리 오버랩 될 경우 예외처리 추가

	MSCompoG* pPreCompoG = nullptr;
	for(long i = 0 ; i < ConnArr.size() ; i++)
	{
		MSConnection* pConn = ConnArr[i];
		MSCompoG* pUrCompoG = pConn->GetPartner(this);
		if(pUrCompoG == nullptr) continue;
		GM2DPolyline* pUrPoly = pUrCompoG->GetTrimmedBoundary();
		bool IsOverlap = false;

		if (pPreCompoG != nullptr)
		{
			GM2DPolyline* pUrPoly = pUrCompoG->GetTrimmedBoundary();
			GM2DPolyline* pPrePoly = pPreCompoG->GetTrimmedBoundary();
			if (pUrPoly->OverlapEx(*pPrePoly))
				IsOverlap = true;
		}

		if (pUrCompoG != nullptr && IsOverlap == false)
		{
			double dLoc = pConn->GetLinkMemberLoc(this);
			LocArr.push_back(dLoc);
		}

		if (IsOverlap == false)
			pPreCompoG = pUrCompoG;
	}

	sort(LocArr.begin(), LocArr.end(), 
		[](const double& a, const double& b)->bool {return a < b;}
	);

	double dPreLoc = 1;
	for (long iLoc = LocArr.size() -1; iLoc >= 0 ; iLoc--)
	{
		double dCurLoc = LocArr[iLoc];
		if (dPreLoc - dCurLoc < dMinSplitLoc)
			LocArr.erase(LocArr.begin() + iLoc);

		dPreLoc = dCurLoc;		
	}
	if (LocArr.size() > 0)
	{
		double dCurLoc = LocArr[0];
		if (dCurLoc < dMinSplitLoc)
			LocArr.erase(LocArr.begin());
	}

	LinkGArr.push_back(this);
	for(long i = 0 ; i < LocArr.size() ; i++)
	{
		MSCompoG* pNewCompoG = (MSCompoG*)Copy();
		LinkGArr.push_back(pNewCompoG);
	}
	
	long NumLinkG = LinkGArr.size();
	if (NumLinkG == 1)
	{
		pFloor->PreCheckRemoveSpliteLinkMember(this);
		return LinkGArr;
	}

	double dSLoc = 0;
	double dELoc = 1;
	double dFloorLevelOffSet = GetCompoMFloorLevelOffSet();
	shared_ptr<GM2DCurve> pOrgCenCurve = this->GetCenCurve();
	for (long iCompoG = NumLinkG -1 ; iCompoG >= 0; iCompoG--)
	{
		MSLinkMemberG* pLinkG = (MSLinkMemberG*)LinkGArr[iCompoG];
		shared_ptr<GM2DCurve> pCenCurve = pLinkG->GetCenCurve();

		if (iCompoG == 0)
			dSLoc = 0;
		else
			dSLoc = LocArr[iCompoG-1];

		GM2DVector SVec = pOrgCenCurve->PointAtParam(dSLoc);
		GM2DVector EVec = pOrgCenCurve->PointAtParam(dELoc);

		pCenCurve->SetP0(SVec);
		pCenCurve->SetP1(EVec);

		pLinkG->SetCenCurve(pCenCurve->Copy());
		
		double dSOffSet = GetStartOffSetbyLoc(dSLoc) - dFloorLevelOffSet;
		double dEOffSet = GetOffSetbyLoc(dELoc) - dFloorLevelOffSet;
		pLinkG->SetStartOffSet(dSOffSet);
		pLinkG->SetEndOffSet(dEOffSet);

		pLinkG->SetLines();
		pLinkG->InitTrimmedBoundary();
		dELoc = dSLoc;
	}


// 	shared_ptr<GM2DCurve> pOrgCenCurve = this->GetCenCurve();
// 	for(long iCompoG = 0 ; iCompoG < NumLinkG ; iCompoG++)
// 	{
// 		MSLinkMemberG* pLinkG = (MSLinkMemberG*)LinkGArr[iCompoG];
// 		shared_ptr<GM2DCurve> pCenCurve = pLinkG->GetCenCurve();
// 
// 		double dLoc = 1;
// 		if(iCompoG != NumLinkG -1)
// 			dLoc = LocArr[iCompoG];
// 
// 		dELoc = dLoc;
// 		
// 		GM2DVector SVec = pOrgCenCurve->PointAtParam(dSLoc);
// 		GM2DVector EVec = pOrgCenCurve->PointAtParam(dELoc);
// 
// 		pCenCurve->SetP0(SVec);
// 		pCenCurve->SetP1(EVec);
// 
// 		pLinkG->SetCenCurve(pCenCurve->Copy());
// 
// 		dSLoc = dELoc;
// 	}
	pFloor->PreCheckRemoveSpliteLinkMember(this);

	for (long iCompoG = LinkGArr.size() - 1; iCompoG >= 0; iCompoG--)
	{
		MSLinkMemberG* pLinkG = (MSLinkMemberG*)LinkGArr[iCompoG];
		MSObject* pRVal = MSModelChecker::CheckCompoGOverlap(pFloor, LinkGArr[iCompoG], 0, false);
		if (pRVal != nullptr)
		{
			LinkGArr.erase(LinkGArr.begin() + iCompoG);
			if (pLinkG != this)
				delete pLinkG;
		}
	}

	return LinkGArr;
}

bool MSLinkMemberG::IsSlopeMember()
{
	if(fabs(m_SOffsetZ - m_EOffsetZ) > DTOL_GM)
		return true;
	return false;
}

bool MSLinkMemberG::HasLevelDif()
{
	if(ma_MemberLevelDif.GetSize()>0)
		return true;

	return false;
}

CString MSLinkMemberG::GetKey(bool bUseID)
{
	// ID, FloorNum, Type, CompoM, 좌표, 옵셋
	CString strCompoMKey;
	if(mp_CompoM == nullptr || mp_CompoM->m_ID == 0)
		strCompoMKey.Format(_T("%d"), 0);
	else
		strCompoMKey.Format(_T("%d"), &mp_CompoM->m_ID);
	CString strGeo;
	strGeo.Format(_T("%d_%d_%d_%d"), (int)ma_Vec[0]->m_X, (int)ma_Vec[0]->m_Y, (int)ma_Vec[1]->m_X, (int)ma_Vec[1]->m_Y);
	CString strOffSet;
	strOffSet.Format(_T("%d_%d_%d"), (int)IsSlopeMember(), (int)m_SOffsetZ, (int)m_EOffsetZ);
	
	CString strKey;
	strKey.Format(_T("%d_%d_%s_%s_%s"), GetFloorNum(), (int)GetType(), strCompoMKey, strGeo, strOffSet);
	if (bUseID)
	{
		CString strID;
		strID.Format(_T("%d_"), m_ID);
		strKey = strID + strKey;
	}

	return strKey;
}

bool MSLinkMemberG::IsSameGeometry(MSCompoG* urCompoG, double distTolerance, double degreeTolerance)
{
	shared_ptr<GM2DCurve> myCurve = GetCenCurve();
	shared_ptr<GM2DCurve> urCurve = ((MSLinkMemberG*)urCompoG)->GetCenCurve();

	if (myCurve->m_ObjType != urCurve->m_ObjType)	return false;
	if (myCurve->m_ObjType == GT_LINE)
	{
		GM2DLineVector myLine = GetCenLVec();
		GM2DLineVector urLine = ((MSLinkMemberG*)urCompoG)->GetCenLVec();

		if (myLine.DirVector()*urLine.DirVector() < 0)
			urLine.ReverseDirection();

		if (!myLine.m_P0.SameVector4DTOLByCeil(urLine.m_P0, distTolerance))	return false;
		if (!myLine.m_P1.SameVector4DTOLByCeil(urLine.m_P1, distTolerance))	return false;

		if (!myLine.IsParallel4DTol(urLine, degreeTolerance))	return false;
	}
	else if(myCurve->m_ObjectType == GT_ARC)
	{
		GM2DArc* myArc = (GM2DArc*)&*myCurve;
		GM2DArc* urArc = (GM2DArc*)&*urCurve;

		if (!myArc->m_Center.SameVector4DTOLByCeil(urArc->m_Center, distTolerance))	return false;
		if (!myArc->GetStart2DVec().SameVector4DTOLByCeil(urArc->GetStart2DVec(), distTolerance))	return false;
		if (!myArc->GetEnd2DVec().SameVector4DTOLByCeil(urArc->GetEnd2DVec(), distTolerance))	return false;
	}

	return true;
}

// 직선인 경우만 판단
bool MSLinkMemberG::IsConnectedLinkMemberGs(vector<MSLinkMemberG*> urLinkGList)
{
	if (isArc())	return false;

	GM2DLineVector myLineVec;
	myLineVec.SetP0(*GetSVec());	myLineVec.SetP1(*GetEVec());
	vector<double> locValueArr;
	locValueArr.push_back(0.0), locValueArr.push_back(1.0);
	for (auto it = urLinkGList.begin(); it != urLinkGList.end(); it++)
	{
		MSLinkMemberG* pUrLinkG = *it;
		if (pUrLinkG->isArc())	return false;
		
		locValueArr.push_back(myLineVec.OrthoBaseLocValue(*pUrLinkG->GetSVec()));
		locValueArr.push_back(myLineVec.OrthoBaseLocValue(*pUrLinkG->GetEVec()));
	}

	std::sort(locValueArr.begin(), locValueArr.end());

	// 정렬된 LocValueArr의 처음과 마지막을 제외한 나머지 LocValue들이 두개씩 쌍으로 있어야 이어진것으로 판단
	int count = locValueArr.size();
	int fromLoc = 1, toLoc = count - 2;
	for (int index = fromLoc; index < toLoc; index += 2)
	{
		double pairLoc1 = locValueArr[index];
		double pairLoc2 = locValueArr[index + 1];
		if (fabs(pairLoc1 - pairLoc2) > DTOL_GM)	return false;
	}
	return true;
}

bool MSLinkMemberG::IsConnectedMember(MSLinkMemberG* pUrLinkG)
{
	MSLinkMemberG* pMyLinkG = this;
	MSConnection* linkConnection = pMyLinkG->GetConnection(pUrLinkG);
	if (linkConnection != nullptr)	return true;
	return false;
}

bool MSLinkMemberG::HasSubMember()
{
	if (HasLevelDif())	return true;
	return MSCompoG::HasSubMember();
}

void MSLinkMemberG::DeleteSubMembers()
{
	DeleteMemberLevelDifG();
	MSCompoG::DeleteSubMembers();
}

double MSLinkMemberG::GetWidthSetback()
{
	return m_dWidthSetback;
}

void MSLinkMemberG::SetWidthSetback(double thickSetback)
{
	m_dWidthSetback = thickSetback;
}

bool MSLinkMemberG::UsedMaterial(long materialID)
{
	long NumDifG = ma_MemberLevelDif.GetCount();
	for (long iDifG = 0; iDifG < NumDifG; iDifG++)
	{
		long nHaunchMaterialID = ma_MemberLevelDif[iDifG]->GetHaunchMaterialID();
		if (nHaunchMaterialID == materialID)
			return true;
	}

	return MSCompoG::UsedMaterial(materialID);
}

void MSLinkMemberG::SubMemberMaterialValidCheck()
{
	MSBaseBuilding* pBldg = GetBuilding();
	if (pBldg == nullptr) return;

	long defMaterialID = -1;

	long NumDifG = ma_MemberLevelDif.GetCount();
	for (long iDifG = 0; iDifG < NumDifG; iDifG++)
	{
		if (ma_MemberLevelDif[iDifG]->GetIsUsedStoryMaterial() == true) continue;;

		long nHaunchMaterialID = ma_MemberLevelDif[iDifG]->GetHaunchMaterialID();
		if (pBldg->GetMaterial(nHaunchMaterialID) != nullptr) continue;

		if (defMaterialID == -1)
		{
			MSMaterial* defMaterial = pBldg->GetDefMaterial();
			if (defMaterial == nullptr) return;
			defMaterialID = defMaterial->m_ID;
		}

		ma_MemberLevelDif[iDifG]->SetHaunchMaterialID(defMaterialID);
	}
}



CString MSLinkMemberG::ToString()
{
	auto str = MSCompoG::ToString();
	str.AppendFormat(L"Start:%s, End:%s, StartOffsetZ : %f, EndOffsetZ: %f, ", GetSVec()->ToString(), GetEVec()->ToString(), m_SOffsetZ, m_EOffsetZ);
	return str;
}

// Serialize이후 객체정보를 수정해야할 필요가 있는 경우 사용
void MSLinkMemberG::FixCompoGInfoAfterSerialize()
{
	//int MSVersionInfo::m_LatestVersion = 20250412;//BH25-303 슬래브 벽체 트림라인 버그 수정 업데이트
	// 25.0.0 ~ 25.0.0.1인 경우에만 업데이트처리
	if (MSVersionInfo::GetCurrentVersion() >= 20250319 && MSVersionInfo::GetCurrentVersion() < 20250412)
	{
		UpdateBoundaryVisibilities();
	}
}