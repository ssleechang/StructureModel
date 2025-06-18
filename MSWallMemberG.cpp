#include "StdAfx.h"
#include "MSWallMemberG.h"

#include "MSLinkMemberG.h"
#include "MSContainer.h"
#include "MSFloor.h"
#include "MSFloorQT.h"
#include "MSRebarBuilding.h"
#include "MSRcFaceMemberM.h"
#include "MSLineLevelDifG.h"
#include "MSMemberLevelDif.h"
#include "MSLevelZone.h"
#include "MSConstructionZone.h"
#include "GMLib/MSVersionInfo.h"
#include "MSDoorG.h"
#include "MSWindowG.h"
#include "MSFWSwitchBoxG.h"
#include "MSOpeningM.h"
#include "GMLib/GM2DRectangle.h"
#include "GMLib\AcisGeomHelper.h"
#include "GMLib\GM2DBound.h"
#include "MSPolylineObject.h"
#include "MSConnection.h"
#include "MSWallOpeningExtraG.h"
#include "MSDoorM.h"
#include "MSModelChecker.h"
#include "MSWallEndRebarLine.h"
#include "MSWallEndRebarM.h"
#include "MSRcBWallM.h"
#include "MSFWShadeG.h"

#include <math.h>

#include "GMLib/MSVersionInfo.h"
#include "Radfcore_CLI/AppGlobalOptionWrapper.h"

IMPLEMENT_SERIAL(MSWallMemberG, MSLinkMemberG,  VERSIONABLE_SCHEMA | 1)

MSWallMemberG::MSWallMemberG()
{
	Init(true);
}

MSWallMemberG::~MSWallMemberG(void)
{
	DeleteLowMemberLevelDifG();
    DeleteOpenings();
	DeleteShadeGs();
}

MSObject* MSWallMemberG::Copy( bool bNewID /*= true*/ )
{
	MSWallMemberG* pNewObj;

	pNewObj = new MSWallMemberG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSWallMemberG::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;
	if( !pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() != msWallG) { ASSERT(0); return; }

	MSLinkMemberG::CopyFromMe(pSource_in, bNewID);

	MSWallMemberG* wall = (MSWallMemberG*)pSource_in;
	DeleteOpenings();
	for(map<long, MSWallOpeningG*>::iterator itr = wall->mm_WallOpening.begin() ; itr != wall->mm_WallOpening.end() ; ++itr)
	{
		MSWallOpeningG* copy_opening = (MSWallOpeningG*)(itr->second->Copy(false));
////		copy_opening->m_ID = itr->second->m_ID;
		copy_opening->SetOwnerCompoG(this);
		mm_WallOpening.insert(make_pair(copy_opening->GetID(), copy_opening));
	}
	m_IsLeftInside = wall->m_IsLeftInside;
	
	m_SOffsetZ_End = wall->m_SOffsetZ_End;
	m_EOffsetZ_End = wall->m_EOffsetZ_End;

	m_IsStartEndAddedBar = wall->m_IsStartEndAddedBar;
	m_IsEndEndAddedBar = wall->m_IsEndEndAddedBar;
	m_StartEndRebarLineID = wall->m_StartEndRebarLineID;
	m_EndEndRebarLineID = wall->m_EndEndRebarLineID;

	DeleteShadeGs();
	for (map<long, MSFWShadeG*>::iterator itr = wall->mm_ShadeG.begin(); itr != wall->mm_ShadeG.end(); ++itr)
	{
		MSFWShadeG* copy_shade = (MSFWShadeG*)(itr->second->Copy(false));
		//copy_shade->m_ID = itr->second->m_ID;
		//copy_shade->GetID();
		copy_shade->SetOwnerCompoG(this);
		Add(copy_shade);
	}
}
//
void MSWallMemberG::Serialize(CArchive &ar)
{
	MSLinkMemberG::Serialize(ar);
	MSBaseBuilding* pBldg = GetBuilding();
	bool bEachCompoM = false;
	if(ar.IsStoring())
	{
		if(mp_CompoM)
		{
			if(mp_CompoM->m_ID == 0)
				bEachCompoM = true;

			ar << bEachCompoM;
			if(bEachCompoM == true)
				Serialize_EachCompoM(ar);
		}
        int nOpening = mm_WallOpening.size();
        ar << nOpening;
        map<long, MSWallOpeningG*>::iterator it;
        for(it = mm_WallOpening.begin();it!=mm_WallOpening.end();it++)
         {
			 MSWallOpeningG* pWallOpeningG = it->second;
			 int openingType = pWallOpeningG->GetType();
			 ar << openingType;
     		 it->second->Serialize(ar);
		}
		ar << m_IsLeftInside;
		
		ar << m_SOffsetZ_End;
		ar << m_EOffsetZ_End;

		int count = ma_MemberLevelDifLow.GetSize();
		ar << count;
		for (int i=0 ; i<count ; ++i)
			ma_MemberLevelDifLow[i]->Serialize(ar);

		ar << m_IsStartEndAddedBar;
		ar << m_IsEndEndAddedBar;
		ar << m_StartEndRebarLineID;
		ar << m_EndEndRebarLineID;

		count = mm_ShadeG.size();
		ar << count;
		for (auto it = mm_ShadeG.begin(); it != mm_ShadeG.end(); it++)
		{
			it->second->Serialize(ar);
		}
	}
	else
	{
		ar >> bEachCompoM;
		if(bEachCompoM)
			Serialize_EachCompoM(ar);

        int nOpening = 0;
        ar >> nOpening;
        for (int i = 0; i < nOpening; i++)
        {
			int openingType = 0;
			ar >> openingType;
            MSWallOpeningG* pOpening = (MSWallOpeningG*)pBldg->CreateNewCompoG(openingType);
			pOpening->mp_Owner = mp_Owner;
			pOpening->SetOwnerCompoG(this);
            pOpening->Serialize(ar);
			MSOpeningM* pOpeningM = (MSOpeningM*)pOpening->GetCompoM();
			// Serialize를 사용하는 경우, 1.파일 오픈, 2.클립보드 복사 및 붙이기
			// 클립 보드 붙이기시 pOpeningM정보를 나중에 셋팅하기 때문에 이단계에서 검토하여 삭제 하면 안된다.
			if (pOpeningM == nullptr && (mp_Owner != nullptr && mp_Owner->m_bIsSerializing))
			{
				CString strGroup = LocalDic::GetLocal(L"모델링 오류", L"StructureModel");
				CString strErrMsg;
				strErrMsg.Format(LocalDic::GetLocal(L"Wall(%%d) 손상된 Opening을 삭제 하였습니다.", L"StructureModel"));
				MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 1, MESSAGE_LEVEL_WARNING, MESSAGE_WARNING_WALLOPENINGLOC, this);

				delete pOpeningM;
			}
			else
				mm_WallOpening.insert(make_pair(pOpening->m_ID, pOpening));
        }
		if (MSVersionInfo::GetCurrentVersion() >= 20140504)
			ar >> m_IsLeftInside;
		else
			m_IsLeftInside = true;
		if ((IsBHQorBHCEMode() && (MSVersionInfo::GetCurrentVersion() >= 20140731 && MSVersionInfo::GetCurrentVersion() < 20180906))
			|| (IsBHForBHAMode() && MSVersionInfo::GetCurrentVersion() < 20240307))
		{
			double dThickSetback = 0;
			ar >> dThickSetback;
			SetWidthSetback(dThickSetback);
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20150404)
		{
			ar >> m_SOffsetZ_End;
			ar >> m_EOffsetZ_End;
		}
		else
		{
			m_SOffsetZ_End = m_SOffsetZ;
			m_EOffsetZ_End = m_EOffsetZ;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20141006)
		{
			int count = 0;
			ar >> count;
			for (int i=0 ; i<count ; ++i)
			{
// 				if (MSConnectivity::JOINT_SERIALIZE())
// 				{
// 					MSMemberLevelDif* diff = new MSMemberLevelDif();
// 					diff->Serialize(ar);
// 					ma_MemberLevelDifLow.Add(diff);
// 				}
// 				else
				{
					MSMemberLevelDif tmp;
					tmp.Serialize(ar);
				}
			}
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20151211)
		{
			ar >> m_IsStartEndAddedBar;
			ar >> m_IsEndEndAddedBar;
			ar >> m_StartEndRebarLineID;
			ar >> m_EndEndRebarLineID;
		}

		if (MSVersionInfo::GetCurrentVersion() < 20181127)
		{
			SubMemberMaterialValidCheck();
		}

		// KF3D 추가 DB
		if (IsMVAddToBHQ())
		{
			int count = 0;
			ar >> count;
			for (int i = 0; i < count; i++)
			{
				MSFWShadeG* pShade = new MSFWShadeG();
				pShade->mp_Owner = mp_Owner;
				pShade->SetOwnerCompoG(this);
				pShade->Serialize(ar);
				Add(pShade);
			}
		}
	}
}

void MSWallMemberG::Serialize_EachCompoM_Rebar(CArchive& ar)
{
	if (ar.IsStoring())
	{
		MSRcFaceMemberM* pFaceM = (MSRcFaceMemberM*)mp_CompoM;
		long MType = pFaceM->m_nType;

		ar << MType;
		mp_CompoM->Serialize(ar);
		//ar << pFaceM->GetMaterial()->m_ID;
		//ar << pFaceM->m_Name;
		//ar << pFaceM->GetThick();
	}
	else
	{
		MSBaseBuilding* pBldg = GetBuilding();
		if(MSVersionInfo::GetCurrentVersion() < 20220321)
		{
			long MType = 0;
			ar >> MType;

			MSRcFaceMemberM* pFaceM = (MSRcFaceMemberM*)pBldg->CreateNewCompoM(MType);
			pFaceM->m_nType = (MSCompoM::Type_MSCompoM)MType;

			long nMaterialID = 0;
			ar >> nMaterialID;
			pFaceM->SetMaterial(pBldg->GetMaterial(nMaterialID));
			ar >> pFaceM->m_Name;

			double dThick = 0;
			ar >> dThick;
			pFaceM->SetThick(dThick);
			pFaceM->mp_Owner = pBldg;
			mp_CompoM = pFaceM;
		}
		else
		{
			long nType = 0;
			ar >> nType;
			mp_CompoM = pBldg->CreateNewCompoM(nType);
			mp_CompoM->Serialize(ar);
		}
	}
}

void MSWallMemberG::Dump(EFS::EFS_Buffer& buffer)
{
	MSLinkMemberG::Dump(buffer);

	int nOpening = mm_WallOpening.size();
	buffer << nOpening;
	map<long, MSWallOpeningG*>::iterator it;
	for(it = mm_WallOpening.begin();it!=mm_WallOpening.end();it++)
	{
		int openingType = it->second->GetType();
		buffer << openingType;
		it->second->Dump(buffer);
	}
	buffer << m_IsLeftInside;
	buffer << m_SOffsetZ_End;
	buffer << m_EOffsetZ_End;

	buffer << m_IsStartEndAddedBar;
	buffer << m_IsEndEndAddedBar;
	buffer << m_StartEndRebarLineID;
	buffer << m_EndEndRebarLineID;

	MSWallEndRebarLine* pStartWallEndRebarLine = GetWallEndRebarLine(m_StartEndRebarLineID);
	if(pStartWallEndRebarLine != NULL)	pStartWallEndRebarLine->Dump(buffer);

	MSWallEndRebarLine* pEndWallEndRebarLine = GetWallEndRebarLine(m_EndEndRebarLineID);
	if(pEndWallEndRebarLine != NULL)	pEndWallEndRebarLine->Dump(buffer);

	int nShadeG = mm_ShadeG.size();
	buffer << nShadeG;
	for (auto it = mm_ShadeG.begin(); it != mm_ShadeG.end(); it++)
	{
		it->second->Dump(buffer);
	}
}

void MSWallMemberG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSLinkMemberG::Recovery(buffer);

	DeleteOpenings();
	//mm_WallOpening.clear();
	int nOpening = 0;
	buffer >> nOpening;
	for (int i = 0; i < nOpening; i++)
	{
		int type = 0;
		buffer >> type;
		MSWallOpeningG* pOpening;
		if (type == msDoorG)
			pOpening = new MSDoorG();
		else if (type == msWindowG)
			pOpening = new MSWindowG();
		else if (type == msFWSwitchBoxG)
			pOpening = new MSFWSwitchBoxG();
		else
			pOpening = new MSWallOpeningExtraG;

		pOpening->mp_Owner = mp_Owner;
		pOpening->SetOwnerCompoG(this);
		pOpening->Recovery(buffer);
		mm_WallOpening.insert(make_pair(pOpening->m_ID, pOpening));
	}
	buffer >> m_IsLeftInside;
	buffer >> m_SOffsetZ_End;
	buffer >> m_EOffsetZ_End;

	buffer >> m_IsStartEndAddedBar;
	buffer >> m_IsEndEndAddedBar;
	buffer >> m_StartEndRebarLineID;
	buffer >> m_EndEndRebarLineID;

	MSWallEndRebarLine* pStartWallEndRebarLine = GetWallEndRebarLine(m_StartEndRebarLineID);
	if(pStartWallEndRebarLine != NULL)	pStartWallEndRebarLine->Recovery(buffer);

	MSWallEndRebarLine* pEndWallEndRebarLine = GetWallEndRebarLine(m_EndEndRebarLineID);
	if(pEndWallEndRebarLine != NULL)	pEndWallEndRebarLine->Recovery(buffer);

	DeleteShadeGs();
	int nShade = 0;
	buffer >> nShade;
	for (int i = 0; i < nShade; i++)
	{
		MSFWShadeG* pShadeG = new MSFWShadeG();
		pShadeG->mp_Owner = mp_Owner;
		pShadeG->SetOwnerCompoG(this);
		pShadeG->Recovery(buffer);
		Add(pShadeG);
	}
}

void MSWallMemberG::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSLinkMemberG::Init(bInConstructor);

	mp_Surface = NULL;
	//mp_SJointUp = NULL;
	//mp_EJointUp = NULL;
	//mp_LineUp = NULL;
	m_IsLeftInside = true;
	m_SOffsetZ_End = 0.0;
	m_EOffsetZ_End = 0.0;

	m_IsStartEndAddedBar = false;
	m_IsEndEndAddedBar = false;
	m_StartEndRebarLineID = 0;
	m_EndEndRebarLineID = 0;
}

bool MSWallMemberG::IsBearingWall()
{
	if(mp_CompoM==NULL)
		return false;

	if(mp_CompoM->GetType() == MSCompoM::msBWallM)
		return true;

	return false;
}

bool MSWallMemberG::IsRetainingWall()
{
	if(mp_CompoM==NULL)
		return false;

	if(mp_CompoM->GetType() == MSCompoM::msRWallM)
		return true;

	return false;
}

bool MSWallMemberG::GetZLevels( double& TopLevel, double& BottomLevel, bool GetBigVal, bool bCheckLevelZone )
{
	if(bCheckLevelZone)
	{
		if(GetBigVal)
		{
			BottomLevel = GetStartLevel();
			TopLevel = GetEndLevel();
		}
		else
		{
			BottomLevel = GetStartLevel(true);
			TopLevel = GetEndLevel(false);
		}
	}
	else
	{
		BottomLevel = GetSlabLevel() + min(m_SOffsetZ, m_SOffsetZ_End);
		TopLevel = GetNextSlabLevel() + max(m_EOffsetZ, m_EOffsetZ_End);
	}

	if (BottomLevel > TopLevel)
	{
		double dTmp = BottomLevel;
		BottomLevel = TopLevel;
		TopLevel = dTmp;
	}

	return true;
}

/*
bool MSWallMemberG::GetZLevelsbyPos(double& TopLevel, double& BottomLevel, char cPos)
{
	if(cPos != 'S' && cPos != 'E') return false;

	GM2DVector* pVec;
	if(cPos == 'S')
		pVec = GetSVec();
	else if(cPos == 'E')
		pVec = GetEVec();

	double dOffSet = 0.;//GetOffSetbyJoint(pJoint);
	double dLowLevelZoneZ = 0;
	double dCurLevelZoneZ = 0;

	if(mp_Owner && mp_Owner->IsFloor())
	{
		MSFloor* pCurFloor = (MSFloor*)mp_Owner;
		MSFloor* pLowFloor = pCurFloor->GetLowerFloor();
		if(pLowFloor != NULL && ma_MemberLevelDifLow.GetSize() > 0) // 조인트의 X,Y 좌표가 래벨존에 포함되나 MemberLevelDif를 생성안하는 경우가 존재함 --> 평행할경우
			dLowLevelZoneZ = pLowFloor->GetLevel4LevelZone(pVec->m_X, pVec->m_Y);
		else
			dLowLevelZoneZ = pLowFloor->GetLevel4LevelZone(this);

		if(pCurFloor != NULL && ma_MemberLevelDif.GetSize() > 0)
			dCurLevelZoneZ = pCurFloor->GetLevel4LevelZone(pVec->m_X, pVec->m_Y);
		else
			dCurLevelZoneZ = pCurFloor->GetLevel4LevelZone(this);

		BottomLevel = GetFloorLevel() + dOffSet + dLowLevelZoneZ + m_SOffsetZ;
		TopLevel = GetNextFloorLevel() + dOffSet + dCurLevelZoneZ + m_EOffsetZ;
	}
	else
	{
		BottomLevel = GetFloorLevel() + dOffSet + dLowLevelZoneZ + m_SOffsetZ;
		TopLevel = GetNextFloorLevel() + dOffSet + dCurLevelZoneZ + m_EOffsetZ;
	}

	return true;
}
*/

bool MSWallMemberG::GetZLevelsbyLoc(double& TopLevel, double& BottomLevel, double dLoc)
{
	shared_ptr<GM2DCurve> pCenCurve = GetCenCurve();
	GM2DVector TheVec = pCenCurve->PointAtParam(dLoc);

	double dTopOffSet = GetOffSetbyLoc(dLoc);
	double dBotOffSet = GetStartOffSetbyLoc(dLoc);

	pCenCurve = NULL;

	double dLowLevelZoneZ = 0;
	double dCurLevelZoneZ = 0;

	if(mp_Owner && mp_Owner->IsFloor())
	{
		MSFloor* pCurFloor = (MSFloor*)mp_Owner;
		MSFloor* pLowFloor = pCurFloor->GetLowerFloor();
		if (pLowFloor != NULL)
		{
			if(ma_MemberLevelDifLow.GetSize() > 0) // 조인트의 X,Y 좌표가 래벨존에 포함되나 MemberLevelDif를 생성안하는 경우가 존재함 --> 평행할경우
				dLowLevelZoneZ = pLowFloor->GetLevel4LevelZone(TheVec.m_X, TheVec.m_Y);
			else
				dLowLevelZoneZ = pLowFloor->GetLevel4LevelZone(this);
		}

		if(pCurFloor != NULL && ma_MemberLevelDif.GetSize() > 0)
			dCurLevelZoneZ = pCurFloor->GetLevel4LevelZone(TheVec.m_X, TheVec.m_Y);
		else
			dCurLevelZoneZ = pCurFloor->GetLevel4LevelZone(this);

		BottomLevel = GetSlabLevel() + dLowLevelZoneZ + dBotOffSet;
		TopLevel = GetNextSlabLevel() + dCurLevelZoneZ + dTopOffSet;
	}
	else
	{
		BottomLevel = GetSlabLevel() + dLowLevelZoneZ + dBotOffSet;
		TopLevel = GetNextSlabLevel() + dCurLevelZoneZ + dTopOffSet;
	}

	return true;
}

double MSWallMemberG::GetOffSetbyLoc(double dLoc)
{
	return (m_EOffsetZ_End - m_EOffsetZ)* dLoc + m_EOffsetZ;
}

double MSWallMemberG::GetStartOffSetbyLoc(double dLoc)
{
	return (m_SOffsetZ_End - m_SOffsetZ)* dLoc + m_SOffsetZ;
}

void MSWallMemberG::SetStartOffSet(double dOffSet)
{
	m_SOffsetZ = m_SOffsetZ_End = dOffSet;
}

void MSWallMemberG::SetEndOffSet(double dOffSet)
{
	m_EOffsetZ = m_EOffsetZ_End = dOffSet;
}

double MSWallMemberG::GetStartOffSet()
{
	return m_SOffsetZ;
}

double MSWallMemberG::GetEndOffSet()
{
	return m_EOffsetZ;
}


double MSWallMemberG::GetHeight()
{
	return GetEndLevel()-GetStartLevel();
}
double MSWallMemberG::GetStartLevel(bool isMax)
{
	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;
	return GetSlabLevel()+GetRelativeStartLevel(isMax);

}
double MSWallMemberG::GetEndLevel(bool isMax)
{

	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;
	return GetNextSlabLevel()+GetRelativeEndLevel(isMax);

}
double MSWallMemberG::GetRelativeEndLevel(bool isMax)
{
	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;

	double dOffSet = 0;
	if(isMax)
		dOffSet = max(m_EOffsetZ, m_EOffsetZ_End);
	else
		dOffSet = min(m_EOffsetZ, m_EOffsetZ_End);

	MSFloor* pFloor = (MSFloor*)mp_Owner;
	if(pFloor == NULL)
		return dOffSet;

	return pFloor->GetLevel4LevelZone(this, isMax) + dOffSet;
}
double MSWallMemberG::GetRelativeStartLevel(bool isMax)
{
	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;

	double dOffSet = 0;
	if(isMax)
		dOffSet = max(m_SOffsetZ, m_SOffsetZ_End);
	else
		dOffSet = min(m_SOffsetZ, m_SOffsetZ_End);

	MSFloor* pFloor = (MSFloor*)mp_Owner;
	if(pFloor == NULL)
		return dOffSet;
	MSFloor* pLowFloor = pFloor->GetLowerFloor();
	if(pLowFloor == NULL)
		return dOffSet;

	return pLowFloor->GetLevel4LevelZone(this, isMax) + dOffSet;
}

double MSWallMemberG::GetRelativeEndLevelbyLoc(double dLocVal)
{
	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;

	double dOffSet = GetOffSetbyLoc(dLocVal);

	MSFloor* pFloor = (MSFloor*)mp_Owner;
	if(pFloor == NULL)
		return dOffSet;

	shared_ptr<GM2DCurve> pCenCurve = GetCenCurve();
	GM2DVector TheVec = pCenCurve->PointAtParam(dLocVal);

	double dZLevel = pFloor->GetLevel4LevelZone(TheVec.m_X, TheVec.m_Y) + dOffSet;

	return dZLevel;
}

double MSWallMemberG::GetRelativeStartLevelbyLoc(double dLocVal)
{
	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;

	double dOffSet = GetStartOffSetbyLoc(dLocVal);

	MSFloor* pFloor = (MSFloor*)mp_Owner;
	if(pFloor == NULL)
		return dOffSet;
	MSFloor* pLowFloor = pFloor->GetLowerFloor();
	if(pLowFloor == NULL)
		return dOffSet;

	shared_ptr<GM2DCurve> pCenCurve = GetCenCurve();
	GM2DVector TheVec = pCenCurve->PointAtParam(dLocVal);

	double dZLevel = pLowFloor->GetLevel4LevelZone(TheVec.m_X, TheVec.m_Y) + dOffSet;

	return dZLevel;
}

bool MSWallMemberG::Add( MSWallOpeningG* pOpening )
{
	if (pOpening == NULL)
		return false;
	pOpening->GetID();
	pOpening->SetOwnerCompoG(this);
    mm_WallOpening.insert(make_pair(pOpening->m_ID, pOpening));
    return true;
}

bool MSWallMemberG::Remove( MSWallOpeningG* pOpening )
{
	if (pOpening == NULL)
		return false;

    map<long, MSWallOpeningG*>::iterator it = mm_WallOpening.find(pOpening->m_ID);
    if(it != mm_WallOpening.end())
        mm_WallOpening.erase(it);
    return true;
}

bool MSWallMemberG::Delete( MSWallOpeningG* pOpening )
{
    Remove(pOpening);
    delete pOpening;
    return true;
}

void MSWallMemberG::DeleteOpenings()
{
    map<long, MSWallOpeningG*>::iterator it;
    for(it = mm_WallOpening.begin(); it != mm_WallOpening.end(); it++)
        delete it->second;
    mm_WallOpening.clear();
}

bool MSWallMemberG::HasOpening()
{
    if (mm_WallOpening.size()>0)
        return true;
    return false;
}

MSWallOpeningG* MSWallMemberG::GetOpening( long ID )
{
    map<long, MSWallOpeningG*>::iterator itr = mm_WallOpening.find(ID);
    if(itr != mm_WallOpening.end())
        return itr->second;
    return NULL;
}

void MSWallMemberG::ModifyOpeningWhenModifyWall(MSWallMemberG* org, MSWallMemberG* tgt)
{
	if (org->isArc())
	{
		shared_ptr<GM2DCurve> cv1 = org->GetCenCurve();
		shared_ptr<GM2DCurve> cv2 = tgt->GetCenCurve();
		GM2DArc* arc1 = (GM2DArc*)(&*cv1);
		GM2DArc* arc2 = (GM2DArc*)(&*cv2);
		
		if (fabs(arc1->Length() - arc2->Length()) < DTOL_GM) return;
		for (map<long, MSWallOpeningG*>::iterator itr = org->mm_WallOpening.begin(); itr != org->mm_WallOpening.end(); ++itr)
		{
			MSWallOpeningG* org_opening = itr->second;
			if (tgt->mm_WallOpening.count(org_opening->m_ID) == 0) continue;
			MSWallOpeningG* tgt_opening = tgt->mm_WallOpening.find(org_opening->m_ID)->second;
			if (tgt_opening == NULL) continue;

			MSOpeningM* openingM = (MSOpeningM*)tgt_opening->GetCompoM();
			double width = openingM->GetWidth();
			if (arc1->GetStartPoint().Distance(arc2->GetStartPoint()) > DTOL_GM)
			{
				double end_dist = arc1->Length() - org_opening->m_BaseOffset.m_X;
				double new_leng = arc2->Length();
				double new_x = new_leng - end_dist;
				if (new_x < 0) new_x = 0;
				if (new_x > new_leng - width) new_x = new_leng - width;
				tgt_opening->m_BaseOffset.m_X = new_x;
			}
			if (arc1->GetEndPoint().Distance(arc2->GetEndPoint()) > DTOL_GM)
			{
				double new_leng = arc2->Length();
				if (tgt_opening->m_BaseOffset.m_X > new_leng - width)
					tgt_opening->m_BaseOffset.m_X = new_leng - width;
			}
		}
		return;
	}

	GMLineVector org_vec(org->GetSVec()->m_X, org->GetEVec()->m_X, org->GetSVec()->m_Y, org->GetEVec()->m_Y);
	double org_leng = org_vec.Length();
	GMLineVector tgt_vec(tgt->GetSVec()->m_X, tgt->GetEVec()->m_X, tgt->GetSVec()->m_Y, tgt->GetEVec()->m_Y);
	double tgt_leng = tgt_vec.Length();

	bool bMove = false;
	if (fabs(org_leng - tgt_leng) < DTOL_GM)
		bMove = true;

	if (bMove)
		return;

	for (map<long, MSWallOpeningG*>::iterator itr = org->mm_WallOpening.begin() ; itr != org->mm_WallOpening.end() ; ++itr)
	{
		MSWallOpeningG* org_opening = itr->second;
		if (tgt->mm_WallOpening.count( org_opening->m_ID ) == 0) continue;
		MSWallOpeningG* tgt_opening = tgt->mm_WallOpening.find( org_opening->m_ID )->second;
		if ( tgt_opening == NULL ) continue;

		MSOpeningM* openingM = (MSOpeningM*)tgt_opening->GetCompoM();
		double width = openingM->GetWidth();
		double width_ratio = width / tgt_leng;

		// org ratio, 위치 구하기
		double org_ratio = org_opening->m_BaseOffset.m_X / org_leng;
		if (org_opening->m_Reversed) org_ratio = 1.0 - org_ratio;
		double x = org->GetSVec()->m_X + org_ratio * (org->GetEVec()->m_X - org->GetSVec()->m_X);
		double y = org->GetSVec()->m_Y + org_ratio * (org->GetEVec()->m_Y - org->GetSVec()->m_Y);

		// new ratio, 위치 구하기
		double new_ratio = tgt_vec.OrthoBaseLocValue(GM2DVector(x, y));
		if (tgt_opening->m_Reversed) new_ratio = 1.0 - new_ratio;

		if (new_ratio < 0.0 || new_ratio > 1.0 - width_ratio)
		{
			if ( fabs(org_leng - tgt_leng) > DTOL_GM) // 길이 변경시만 끝으로 이동, 변경 없는경우는 그대로
			{
				if (new_ratio < 0.0) new_ratio = 0.0;
				if (new_ratio > 1.0 - width_ratio) new_ratio = 1.0 - width_ratio;
				tgt_opening->m_BaseOffset.m_X = tgt_leng * new_ratio;
			}
		}
		else
		{
			tgt_opening->m_BaseOffset.m_X = tgt_leng * new_ratio;
		}
	}
	
}

void MSWallMemberG::RemoveLevelDifLow( MSLineLevelDifG* pLineLevelDifg )
{
	long nLevelDif = ma_MemberLevelDifLow.GetSize();
	for(long iLevelDif=nLevelDif-1; iLevelDif>=0; iLevelDif--)
	{
		if(ma_MemberLevelDifLow[iLevelDif]->mp_LineLevelDifG ==pLineLevelDifg)
		{
			delete ma_MemberLevelDifLow[iLevelDif];
			ma_MemberLevelDifLow.RemoveAt(iLevelDif);
		}
	}
}
void MSWallMemberG::DeleteLowMemberLevelDifG()
{
	long nLevelDif = ma_MemberLevelDifLow.GetSize();
	for(long iLevelDif= 0 ; iLevelDif < nLevelDif; iLevelDif++)
	{
		MSMemberLevelDif* pBeamDif = ma_MemberLevelDifLow[iLevelDif];
		delete pBeamDif;
	}
	ma_MemberLevelDifLow.RemoveAll();
}

void MSWallMemberG::InitializeParameter()
{
	MSLinkMemberG::InitializeParameter();
	DeleteLowMemberLevelDifG();
}

bool MSWallMemberG::CheckHeight(bool IsUseGroupName)
{
	// Height Check
	if (GetFloor() == NULL || GetFloor()->IsBlock()) { } // check skip
	else
	{
		CString groupName(_T("Wall"));
		if (IsRetainingWall()) groupName = _T("Retaining Wall");
		/*
		if (m_SOffsetZ < 0)
		{
			CString msg(_T("Error : Bottom Offset >= 0"));
			mp_MsgObject = NULL;
			AddErrorObject(0, -1, groupName, msg);
			return false;
		}
		if (m_EOffsetZ > 0)
		{
			CString msg(_T("Error : Top Offset <= 0"));
			mp_MsgObject = NULL;
			AddErrorObject(0, -1, groupName, msg);
			return false;
		}
		*/
		MSFloor* pCurFloor = GetFloor();
		MSFloor* pLowerFloor = GetFloor()->GetLowerFloor();
		double bot = 0, top = 0;
		if (IsSlopeMember())
		{
			double Bot1 = GetSlabLevel() + m_SOffsetZ;
			double Bot2 = GetSlabLevel() + m_SOffsetZ_End;
			double Top1 = GetNextSlabLevel() + m_EOffsetZ;
			double Top2 = GetNextSlabLevel() + m_EOffsetZ_End;
			bot = __max(Bot1, Bot2);
			top = __min(Top1, Top2);
		}
		else
		{
			double Bot1 = GetSlabLevel() + m_SOffsetZ;
			double Bot2 = GetSlabLevel() + m_SOffsetZ_End;
			if (pLowerFloor != nullptr)
			{
				Bot1 += pLowerFloor->GetLevel4LevelZone(ma_Vec[0]->m_X, ma_Vec[0]->m_Y);
				Bot2 += pLowerFloor->GetLevel4LevelZone(ma_Vec[1]->m_X, ma_Vec[1]->m_Y);
			}

			double Top1 = GetNextSlabLevel() + pCurFloor->GetLevel4LevelZone(ma_Vec[0]->m_X, ma_Vec[0]->m_Y) + m_EOffsetZ;
			double Top2 = GetNextSlabLevel() + pCurFloor->GetLevel4LevelZone(ma_Vec[1]->m_X, ma_Vec[1]->m_Y) + m_EOffsetZ_End;
			bot = __max(Bot1, Bot2);
			top = __min(Top1, Top2);
		}
		
		if (bot >= top)
		{
			CString strGroupName = _T("");
			CString msg = LocalDic::GetLocal(_T("오프셋과 레벨존을 적용한 수직부재의 높이가 0보다 작습니다."), _T("CheckData"));
			if (IsUseGroupName == true)
			{
				if (m_ID != 0)
					strGroupName.Format(_T("[%s_%s(ID:%d)]: "), GetFloor()->GetName(), GetGroupName(), m_ID);
			}
			
			mp_MsgObject = NULL;
			AddErrorObject(0, -1, groupName, strGroupName + msg);
			return false;
		}
	}
	return true;
}

bool MSWallMemberG::CheckData()
{
	if (MSElement::CheckData() == false)
		return false;

	// Height Check
	if (!CheckHeight())
		return false;

	if (IsSpecialBoundaryM())
	{
		CString strGroup = LocalDic::GetLocal(L"모델링 오류", L"StructureModel");
		CString strErrMsg;
		MSFloor* pFloor = GetFloor();
		bool rVal = true;
		if (mm_WallOpening.size() > 0)
		{
			strErrMsg.Format(LocalDic::GetLocal(L"오프닝이 존재하는 벽은(%%d)은 특수 전단 정보를 입력할수 없습니다.", L"StructureModel"));
			MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 1, MESSAGE_LEVEL_WARNING, MESSAGE_WARNING_WALLOPENINGLOC, this);
			rVal = false;
		}
		if (IsSlopeMember())
		{
			strErrMsg.Format(LocalDic::GetLocal(L"경사벽(%%d)은 특수 전단 정보를 입력할수 없습니다.", L"StructureModel"));
			MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 1, MESSAGE_LEVEL_WARNING, MESSAGE_WARNING_WALLOPENINGLOC, this);
			rVal = false;
		}
		if (isArc())
		{
			strErrMsg.Format(LocalDic::GetLocal(L"아크 벽(%%d)은 특수 전단 정보를 입력할수 없습니다.", L"StructureModel"));
			MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 1, MESSAGE_LEVEL_WARNING, MESSAGE_WARNING_WALLOPENINGLOC, this);
			rVal = false;
		}
		if (rVal == false)
			return rVal;
	}

	//오프닝이 벽체 안에 있는지 체크
	map<long, MSWallOpeningG*>::iterator it;

	if (MSModelChecker::CheckModel_WallOpening(this) == false)
		return true;

	if(CheckShadeOverlap() == true)
		return false;
	// 부재 중첩 체크
	MSFloor* floor = GetFloor();
	if (floor != NULL && floor->CheckCompoGOverlap_WithUpDownFloor(this, this->m_ID))
		return true;
		//return false;

	// Check add...

	return true;
}
bool MSWallMemberG::CheckShadeOverlap(MSFWShadeG *pShade)
{
	CString strGroup = _T(""), strErrMsg = _T("");			
	if (pShade->CheckData(true) == false)
	{		
		return true;
	}
	strGroup = LocalDic::GetLocal(L"벽체 차양 중첩", L"StructureModel");	
	strErrMsg.Format(LocalDic::GetLocal(L"%s : 입력된 벽체 차양이 기존 차양과 중첩됩니다.", L"StructureModel"), GetFloor()->GetName());
	for(auto itShade = mm_ShadeG.begin() ; itShade != mm_ShadeG.end() ; itShade++)
	{
		MSFWShadeG *pCompareShade = itShade->second;
		if(pCompareShade->Overlap(pShade) || pShade->Overlap(pCompareShade))
		{
			MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pCompareShade);
			return true;
		}
	}
	return false;
}
bool MSWallMemberG::CheckShadeOverlap()
{	
	for (auto itShade = mm_ShadeG.begin(); itShade != mm_ShadeG.end(); itShade++)
	{
		MSFWShadeG* pCheckShade = itShade->second;

		if(pCheckShade->CheckData(true) == false)
			return true;
		
		for (auto itShade2 = mm_ShadeG.begin(); itShade2 != mm_ShadeG.end(); itShade2++)
		{
			MSFWShadeG* pShade = itShade2->second;
			if (pCheckShade == pShade)
				continue;
			
			if (pShade->Overlap(pCheckShade) || pCheckShade->Overlap(pShade))
			{
				CString strGroup = LocalDic::GetLocal(L"벽체 차양 중첩", L"StructureModel");								
				CString strErrMsgOverlapShade;
				strErrMsgOverlapShade.Format(LocalDic::GetLocal(L"%s : 수정된 차양이 기존 차양과 중첩됩니다.", L"StructureModel"), GetFloor()->GetName());
				MSModelChecker::MakeErrorObject(strGroup, strErrMsgOverlapShade, 0, MESSAGE_LEVEL_ERROR, MESSAGE_ERROR_OVERLAP, pShade);
				return true;
			}
		}
	}
	return false;
}
bool MSWallMemberG::GetNextMemberLevelDif(double dLoc,double& dNextLoc, double& dBotLevelDif, double& dTopLevelDif)
{
	dBotLevelDif = 0;
	dTopLevelDif = 0;

	bool bFind = false;
	dNextLoc = 1;
	double dTol = 0.01;
	long numBotLevelDif = ma_MemberLevelDifLow.GetSize();
	long numTopLevelDif = ma_MemberLevelDif.GetSize();
	for(long iBotLD = 0;iBotLD<numBotLevelDif;iBotLD++)
	{
		double dBotLoc = ma_MemberLevelDifLow[iBotLD]->m_Loc;
		if(fabs(dBotLoc-dLoc) > dTol && dLoc+dTol < dBotLoc && dNextLoc-dBotLoc > dTol)
		{
			dNextLoc = dBotLoc;
			dBotLevelDif = ma_MemberLevelDifLow[iBotLD]->m_EndLevel - ma_MemberLevelDifLow[iBotLD]->m_StartLevel;
			bFind = true;
		}
	}
	for(long iTopLD = 0;iTopLD<numTopLevelDif;iTopLD++)
	{
		double dTopLoc = ma_MemberLevelDif[iTopLD]->m_Loc;
		if(bFind)
		{
			if(fabs(dNextLoc-dTopLoc) < dTol)
			{
				dTopLevelDif = ma_MemberLevelDif[iTopLD]->m_EndLevel - ma_MemberLevelDif[iTopLD]->m_StartLevel;
				dNextLoc = min(dNextLoc,dTopLoc);
				bFind = true;
			}
			else if(dNextLoc > dTopLoc && dLoc+dTol < dTopLoc)
			{
				dNextLoc = dTopLoc;
				dBotLevelDif = 0;
				dTopLevelDif = ma_MemberLevelDif[iTopLD]->m_EndLevel - ma_MemberLevelDif[iTopLD]->m_StartLevel;
				bFind = true;
			}
		}
		else
		{
			if(dLoc+dTol < dTopLoc && dNextLoc > dTopLoc)
			{
				dTopLevelDif = ma_MemberLevelDif[iTopLD]->m_EndLevel - ma_MemberLevelDif[iTopLD]->m_StartLevel;
				dNextLoc = dTopLoc;
				bFind = true;
			}
		}
	}
	return bFind;
}
long MSWallMemberG::GetLevelDifWalls( vector<LevelDifWall>& LevelDifWalls )
{
	double dBotLevel = 0;
	double dTopLevel = 0;

	vector<double> LocArr;
	LocArr.push_back(0);
	LocArr.push_back(1);

	shared_ptr<GM2DCurve> OrgCenCurve = GetCenCurve();

	double dLen = OrgCenCurve->Length();
	double dMegeLen = 200; // 기본 길이 값은 100
	double dTol = 0.01; // 머지 할 Tol 기본은 1%
	double dWidth = GetWidth();

	if (dLen > dMegeLen && dTol > dMegeLen / dLen)
	{
		dTol = dMegeLen / dLen;
		dMegeLen = dTol* dLen;
	}

	long NumTopLevelDif = ma_MemberLevelDif.GetSize();
	for (long iDif = 0; iDif < NumTopLevelDif; iDif++)
	{
		MSMemberLevelDif* pDif = ma_MemberLevelDif[iDif];
		if (pDif == NULL) 
			continue;
		LocArr.push_back(pDif->m_Loc);
	}

	long NumBotLevelDif = ma_MemberLevelDifLow.GetSize();
	for (long iDif = 0; iDif < NumBotLevelDif; iDif++)
	{
		MSMemberLevelDif* pDif = ma_MemberLevelDifLow[iDif];
		if (pDif == NULL)
			continue;
		LocArr.push_back(pDif->m_Loc);
	}

	sort(LocArr.begin(), LocArr.end(), [](const double& a, const double& b)->bool {return a < b; });
	vector<double>::iterator new_end;
	new_end = unique(LocArr.begin(), LocArr.end());
	LocArr.erase(new_end, LocArr.end());

	for (long iLoc = LocArr.size() - 1; iLoc >= 0; iLoc--)
	{
		if (LocArr[iLoc] < 0 || LocArr[iLoc] > 1)
			LocArr.erase(LocArr.begin() + iLoc);
	}

	for (long iLoc = LocArr.size() - 2; iLoc >= 0; iLoc--)
	{
		if (fabs(LocArr[iLoc + 1] - LocArr[iLoc]) < dTol)
		{
			if (iLoc + 1 == LocArr.size() - 1)
				LocArr.erase(LocArr.begin() + iLoc);
			else
				LocArr.erase(LocArr.begin() + iLoc + 1);
		}
	}


	long NumLoc = LocArr.size();
	for (long iLoc = 0; iLoc < NumLoc - 1; iLoc++)
	{
		double dCheckLoc = dCheckLoc = (LocArr[iLoc] + LocArr[iLoc + 1]) / 2;
		GetZLevelsbyLoc(dTopLevel, dBotLevel, dCheckLoc);

		LevelDifWall levelDifWall;
		levelDifWall.SetBotLevel(dBotLevel);
		levelDifWall.SetTopLevel(dTopLevel);
		levelDifWall.SetSLoc(LocArr[iLoc]);
		levelDifWall.SetELoc(LocArr[iLoc + 1]);
		
		LevelDifWalls.push_back(levelDifWall);
	}

	return LevelDifWalls.size();
}

bool MSWallMemberG::GetLeftInside()
{
	return m_IsLeftInside;
}

void MSWallMemberG::SetLeftInside(bool isLeftInside)
{
	m_IsLeftInside = isLeftInside;
}

bool WallLevelDifStructCompare(WallLevelDifStruct &a, WallLevelDifStruct &b)
{
	return a.pLevelDif->m_Loc < b.pLevelDif->m_Loc;
}


vector<WallLevelDifStruct> MSWallMemberG::GetWallLevelDifSt()
{
	vector<WallLevelDifStruct> LevelDifArr;

	long nLevelDif = ma_MemberLevelDif.GetSize();
	long NumLowLevelDif = ma_MemberLevelDifLow.GetSize();

	MSMemberLevelDif* pMemberLevelDif= NULL;
	for(long iLevelDif= 0 ; iLevelDif < nLevelDif; iLevelDif++)
	{
		pMemberLevelDif = ma_MemberLevelDif[iLevelDif];
		if(pMemberLevelDif == NULL) continue;

		WallLevelDifStruct WallLevelDifSt;
		WallLevelDifSt.isTop = 1;
		WallLevelDifSt.pLevelDif = pMemberLevelDif;
		LevelDifArr.push_back(WallLevelDifSt);
	}

	for(long iLevelDif= 0 ; iLevelDif < NumLowLevelDif; iLevelDif++)
	{
		pMemberLevelDif = ma_MemberLevelDifLow[iLevelDif];
		if(pMemberLevelDif == NULL) continue;

		WallLevelDifStruct WallLevelDifSt;
		WallLevelDifSt.isTop = 2;
		WallLevelDifSt.pLevelDif = pMemberLevelDif;
		LevelDifArr.push_back(WallLevelDifSt);
	}
	sort(LevelDifArr.begin(), LevelDifArr.end(), WallLevelDifStructCompare);

	long NumLevelDif = LevelDifArr.size();
	for(long iLevelDif = NumLevelDif-1 ; iLevelDif >= 1 ; iLevelDif--)
	{
		if(fabs(LevelDifArr[iLevelDif].pLevelDif->m_Loc - LevelDifArr[iLevelDif-1].pLevelDif->m_Loc) < DTOL_GM)
		{
			LevelDifArr[iLevelDif-1].isTop = 0;
			LevelDifArr.erase(LevelDifArr.begin() + iLevelDif);
		}
	}

	return LevelDifArr;
}

vector<double> MSWallMemberG::GetWallLevelDifLevel(bool isTop)
{
	vector<double> LevelArr;
	vector<WallLevelDifStruct> LevelDifArr = GetWallLevelDifSt();
	long NumDif = LevelDifArr.size();
	if(NumDif == 0)
		return LevelArr;

	MSFloor* pFloor = GetFloor();
	MSFloor* pLowFloor = pFloor->GetLowerFloor();

	GM2DCurve* pCurve;
	GM2DLineVector TheLine;

	 if(mp_Curve == NULL)
	 {
		 TheLine = GetCenLVec();
		 pCurve = &TheLine;
	 }
	 else
		pCurve = mp_Curve;
	
	GM2DVector SVec = pCurve->m_P0;
	GM2DVector EVec = pCurve->m_P0;
	WallLevelDifStruct WallLevelDifSt;
	for(long iDif = 0 ; iDif < NumDif ; iDif++)
	{
		WallLevelDifSt = LevelDifArr[iDif];
		GM2DCurve*pLevelDifCurve = pCurve->Copy();
		EVec = pCurve->PointAtParam(WallLevelDifSt.pLevelDif->m_Loc);
		pLevelDifCurve->SetP0(SVec);
		pLevelDifCurve->SetP1(EVec);

		GM2DVector MidVec = pLevelDifCurve->PointAtParam(0.5);
		double dZLevel = 0;
		if(isTop)
			dZLevel = pFloor->GetLevel4LevelZone(MidVec.m_X, MidVec.m_Y);
		else if(pLowFloor != nullptr)
			dZLevel = pLowFloor->GetLevel4LevelZone(MidVec.m_X, MidVec.m_Y);
		LevelArr.push_back(dZLevel);

		SVec = EVec;
		delete pLevelDifCurve;
	}


	GM2DCurve*pLevelDifCurve = pCurve->Copy();
	pLevelDifCurve->SetP0(EVec);
	GM2DVector MidVec = pLevelDifCurve->PointAtParam(0.5);

	double dZLevel = 0;
	if(isTop)
		dZLevel = pFloor->GetLevel4LevelZone(MidVec.m_X, MidVec.m_Y);
	else
		dZLevel = pLowFloor->GetLevel4LevelZone(MidVec.m_X, MidVec.m_Y);
	LevelArr.push_back(dZLevel);



// 	double dTopLevel = GetRelativeEndLevel();
// 	double dBotLevel = GetRelativeStartLevel();
// 
// 	WallLevelDifStruct WallLevelDifSt;
// 	for(long iDif = 0 ; iDif < NumDif ; iDif++)
// 	{
// 		 WallLevelDifSt = LevelDifArr[iDif];
// 
// 		if(WallLevelDifSt.isTop == 0 || WallLevelDifSt.isTop == 1)
// 			dTopLevel = WallLevelDifSt.pLevelDif->m_StartLevel;
// 		if(WallLevelDifSt.isTop == 0 || WallLevelDifSt.isTop == 2)
// 			dBotLevel = WallLevelDifSt.pLevelDif->m_StartLevel;
// 
// 		if(isTop)
// 			LevelArr.push_back(dTopLevel);
// 		else 
// 			LevelArr.push_back(dBotLevel);
// 
// 		if(WallLevelDifSt.isTop == 0 || WallLevelDifSt.isTop == 1)
// 			dTopLevel = WallLevelDifSt.pLevelDif->m_EndLevel;
// 		if(WallLevelDifSt.isTop == 0 || WallLevelDifSt.isTop == 2)
// 			dBotLevel = WallLevelDifSt.pLevelDif->m_EndLevel;
// 	}
// 
// 	if(isTop)
// 		LevelArr.push_back(dTopLevel);
// 	else
// 		LevelArr.push_back(dBotLevel);

	return LevelArr;
}

vector<GM2DCurve*> MSWallMemberG::GetLevelDifCurves()
{
	vector<GM2DCurve*> CurveArr;

	vector<WallLevelDifStruct> LevelDifArr = GetWallLevelDifSt();

	long nLevelDif = LevelDifArr.size();
	if(nLevelDif == 0)
		return CurveArr;

	GM2DLineVector MyLVec;
	GM2DCurve* pMyCurve;
	if(mp_Curve==NULL)
	{
		MyLVec = GetCenLVec();
		pMyCurve = &MyLVec;
	}
	else
		pMyCurve = mp_Curve;

	GM2DVector SVec, EVec;

	SVec = pMyCurve->m_P0;

	for(long iLevelDif= 0 ; iLevelDif < nLevelDif; iLevelDif++)
	{
		MSMemberLevelDif* pLevelDif = LevelDifArr[iLevelDif].pLevelDif;
		EVec = pMyCurve->PointAtParam(pLevelDif->m_Loc);
		GM2DCurve* pCurCurve = pMyCurve->Copy();
		if(EVec.SameVector(SVec)) continue;

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

bool MSWallMemberG::IsContainSlabinLowFloor()
{
	MSFloor* pCurFloor = GetFloor();
	if(pCurFloor == NULL)
		return false;
	MSFloor* pLowFloor = pCurFloor->GetLowerFloor();
	if(pLowFloor == NULL)
		return false;
	return pLowFloor->IsContainSlab(this);
}

double MSWallMemberG::GetThick()
{
	MSCompoM* pCompoM = GetCompoM();
	if(pCompoM)
		return pCompoM->GetThick() + GetWidthSetback();
	else
		return 0.;
}

double MSWallMemberG::GetWidth()
{
	return GetThick();
}

void MSWallMemberG::SetThickOfCompoG( double dThick )
{
	MSCompoM* pCompoM = GetCompoM();
	if (pCompoM)
		SetWidthSetback(dThick - pCompoM->GetThick());
	else
		SetWidthSetback(0);
}

bool MSWallMemberG::CheckConnectionMember(CON_POS eConPos)
{
	vector<MSConnection*> EConnArr = GetConnectionByPos(eConPos);
	for(long iConn = 0 ; iConn < EConnArr.size() ; iConn++)
	{
		MSConnection* pConn = EConnArr[iConn];
		MSCompoG* pPartherG = pConn->GetPartner(this);
		if(pPartherG == nullptr)
			continue;
		Type_MSCompoG PartnerType = pPartherG->GetType();
		if(PartnerType == MSElement::msColumnG || PartnerType == MSElement::msWallG)
			return true;
	}
	return false;
}

void MSWallMemberG::ReverseLevelDifLow()
{
	long NumDif = ma_MemberLevelDifLow.GetSize();
	for(long iDif = 0 ; iDif < NumDif ; iDif++)
	{
		MSMemberLevelDif* pDif = ma_MemberLevelDifLow[iDif];
		pDif->ReverseData();
	}
	for(long iDif = 0 ; iDif < NumDif/2 ; iDif++)
	{
		MSMemberLevelDif* pDif = ma_MemberLevelDifLow[iDif];
		MSMemberLevelDif* pDif2 = ma_MemberLevelDifLow[NumDif -(1+iDif) ];
		ma_MemberLevelDifLow[iDif] = pDif2;
		ma_MemberLevelDifLow[NumDif -(1+iDif) ] = pDif;
	}
}

GM2DCurve* MSWallMemberG::GetLongLVecByTrimOpt()
{
	if(GetRigidMember() || HasTrimOption(MSConnection::TO_IGNORE, MSElement::msWallG))
		return GetLongLVec(false);
	else
	{
		vector<MSConnection*> SConnArr = GetConnectionByPos(CON_START);
		vector<MSConnection*> EConnArr = GetConnectionByPos(CON_END);

		bool HasStartPartnerIgnore = false;
		bool HasEndPartnerIgnore = false;
		MSLinkMemberG* pStartLinkG = NULL;
		MSLinkMemberG* pEndLinkG = NULL;
		CON_POS ePartnerStartPos = CON_UNKNOWN;
		CON_POS ePartnerEndPos = CON_UNKNOWN;
		double dStartInnerDeg = 0;
		double dEndInnerDeg = 0;
		for(long iConn = 0 ; iConn < SConnArr.size() ; iConn++)
		{
			MSConnection* pConn = SConnArr[iConn];
			MSCompoG* pPartnerG = pConn->GetPartner(this);
			if(pPartnerG == nullptr || pPartnerG->GetType() != MSElement::msWallG)
				continue;

			int nTrOpt = pConn->GetTrimOption(pPartnerG);
			if(pPartnerG->GetRigidMember() || nTrOpt == MSConnection::TO_IGNORE)
			{
				HasStartPartnerIgnore = true;
				break;
			}
		}

		for(long iConn = 0 ; iConn < EConnArr.size() ; iConn++)
		{
			MSConnection* pConn = EConnArr[iConn];
			MSCompoG* pPartnerG = pConn->GetPartner(this);
			if(pPartnerG == nullptr || pPartnerG->GetType() != MSElement::msWallG)
				continue;

			int nTrOpt = pConn->GetTrimOption(pPartnerG);
			if(pPartnerG->GetRigidMember() || nTrOpt == MSConnection::TO_IGNORE)
			{
				HasEndPartnerIgnore = true;
				break;
			}
		}
		
		bool bExtendToOrthoPart = true;
		if(HasStartPartnerIgnore || HasEndPartnerIgnore)
			bExtendToOrthoPart = false;
		GM2DCurve* pLongCenCurve = GetLongLVec(bExtendToOrthoPart);

		return pLongCenCurve;
	}

	return NULL;
}

CString MSWallMemberG::GetGroupName()
{
	// MSElement::GetGroupName() 함수도 같이 검토해야 한다.
	if(IsRWall())
		return _T("RWall");
	
	return _T("Wall");
}

bool MSWallMemberG::IsRWall()
{
	MSCompoM* pCompoM = GetCompoM();
	MSCompoM::Type_MSCompoM eMType = pCompoM->GetType();

	if(eMType == MSCompoM::msBWallM)
		return false;
	else if(eMType == MSCompoM::msRWallM)
		return true;
	else
		ASSERT(0);

	return false;
}

void MSWallMemberG::GetUrLinkMemberEcc(MSLinkMemberG* pLinkG, double& dLEcc,double& dREcc)
{
	dLEcc = 0;
	dREcc = 0;
	bool IsCenLineReverse = IsReverseDir();
	GM2DCurve* pCenCurve = GetShortLVec();
	if (pCenCurve == nullptr)
		return;

	if(IsCenLineReverse)
		pCenCurve->ReverseDirection();

	bool isRevers = false;

	GM2DVector MVec = pCenCurve->PointAtParam(0.5);
	GM2DVector LMVec, RMVec;

	if(IsCenLineReverse)
	{
		LMVec = mp_RLine->OrthoBaseVector(MVec);
		RMVec = mp_LLine->OrthoBaseVector(MVec);
	}
	else
	{
		LMVec = mp_LLine->OrthoBaseVector(MVec);
		RMVec = mp_RLine->OrthoBaseVector(MVec);
	}

	double dSLoc = pCenCurve->OrthoBaseLocValue(*pLinkG->GetSVec());
	double dELoc = pCenCurve->OrthoBaseLocValue(*pLinkG->GetEVec());
	delete pCenCurve;
	pCenCurve = NULL;

	GM2DVector WallLVec, WallRVec;

	if(dSLoc > dELoc) // reverse...
	{
		double dTmp = dSLoc;
		dSLoc = dELoc;
		dELoc = dTmp;
		isRevers = true;
	}

	if(isRevers)
	{
		WallLVec = pLinkG->mp_RLine->OrthoBaseVector(LMVec);
		WallRVec = pLinkG->mp_LLine->OrthoBaseVector(RMVec);
	}
	else
	{
		WallLVec = pLinkG->mp_LLine->OrthoBaseVector(LMVec);
		WallRVec = pLinkG->mp_RLine->OrthoBaseVector(RMVec);
	}

	// 내 부재 안쪽에 UrLink의 LR 라인이 있으면 양수 밖에 있으면 음수
	long nLSide = 0, nRSide = 0;
	if (IsCenLineReverse)
	{
		nLSide = mp_RLine->WhichSide(WallLVec); // 왼쪽이면 1 오른쪽이면 -1 선상이면 0
		nRSide = mp_LLine->WhichSide(WallRVec) * -1;
	}
	else
	{
		nLSide = mp_LLine->WhichSide(WallLVec)* -1; // 왼쪽이면 1 오른쪽이면 -1 선상이면 0
		nRSide = mp_RLine->WhichSide(WallRVec);
	}

	dLEcc = (double)(int)(LMVec.Distance(WallLVec) * (nLSide));
	dREcc = (double)(int)(RMVec.Distance(WallRVec) * (nRSide));
}

bool MSWallMemberG::IsSlopeMember()
{
	if(fabs(m_SOffsetZ - m_SOffsetZ_End) > DTOL_GM)
		return true;
	if(fabs(m_EOffsetZ - m_EOffsetZ_End) > DTOL_GM)
		return true;
	return false;
}

bool MSWallMemberG::HasLevelDif()
{
	if(MSLinkMemberG::HasLevelDif())
		return true;

	if(ma_MemberLevelDifLow.GetSize()> 0)
		return true;

	return false;
}

void MSWallMemberG::SetOwner(MSContainer* val)
{
	MSLinkMemberG::SetOwner(val);
	map<long, MSWallOpeningG*>::iterator it;
	for (it = mm_WallOpening.begin(); it != mm_WallOpening.end(); it++)
	{
		MSWallOpeningG* pOpening = it->second;
		pOpening->mp_Owner = mp_Owner;
		pOpening->SetOwnerCompoG(this);
	}
	for(auto it = mm_ShadeG.begin() ; it != mm_ShadeG.end() ; it++)
	{
		MSFWShadeG *pShade = it->second;
		pShade->mp_Owner = mp_Owner;
		pShade->SetOwnerCompoG(this);
	}
}

MSWallEndRebarLine* MSWallMemberG::GetWallEndRebarLine(int rebarLineID)
{
	MSRebarBuilding* pBuilding = dynamic_cast<MSRebarBuilding*>(GetBuilding());
	if(pBuilding == NULL)	return NULL;
	return pBuilding->GetWallEndRebarLine(rebarLineID);
}

MSWallEndRebarM* MSWallMemberG::GetWallEndRebarM(int rebarLineID, int story)
{
	MSWallEndRebarLine* pWallEndRebarLine = GetWallEndRebarLine(rebarLineID);
	if(pWallEndRebarLine == NULL)		return NULL;

	return pWallEndRebarLine->GetWallEndRebarM(story);
}

bool MSWallMemberG::IsUsedEndRebarLine()
{
	MSWallEndRebarLine* pStartEndRebarLine = GetWallEndRebarLine(m_StartEndRebarLineID);
	MSWallEndRebarLine* pEndEndRebarLine = GetWallEndRebarLine(m_EndEndRebarLineID);
	if(pStartEndRebarLine != NULL || pEndEndRebarLine != NULL)
		return true;
	return false;
}

void MSWallMemberG::ResetEndRebarLine()
{
	m_StartEndRebarLineID = 0;
	m_EndEndRebarLineID = 0;
	SetIsStartEndAddedBar(false);
	SetIsEndEndAddedBar(false);
}

int MSWallMemberG::GetStartEndRebarLineID()
{
	return m_StartEndRebarLineID;
}

int MSWallMemberG::GetEndEndRebarLineID()
{
	return m_EndEndRebarLineID;
}

bool MSWallMemberG::GetIsStartEndAddedBar()
{
	return m_IsStartEndAddedBar;
}

bool MSWallMemberG::GetIsEndEndAddedBar()
{
	return m_IsEndEndAddedBar;
}

void MSWallMemberG::SetStartEndRebarLineID(int ID)
{
	m_StartEndRebarLineID = ID;
}

void MSWallMemberG::SetEndEndRebarLineID(int ID)
{
	m_EndEndRebarLineID = ID;
}

void MSWallMemberG::SetIsStartEndAddedBar(bool IsAddedBar)
{
	m_IsStartEndAddedBar = IsAddedBar;
}

void MSWallMemberG::SetIsEndEndAddedBar(bool IsAddedBar)
{
	m_IsEndEndAddedBar = IsAddedBar;
}

MSMainBar MSWallMemberG::GetStartEndAddedBar()
{
	MSMainBar MainBar(_T(""));
	MSWallEndRebarM* pWallEndRebarM = GetWallEndRebarM(m_StartEndRebarLineID, GetFloorNum());
	if(pWallEndRebarM == NULL)	return MainBar;
	return pWallEndRebarM->GetMainBar();
}

MSMainBar MSWallMemberG::GetEndEndAddedBar()
{
	MSMainBar MainBar(_T(""));
	MSWallEndRebarM* pWallEndRebarM = GetWallEndRebarM(m_EndEndRebarLineID, GetFloorNum());
	if(pWallEndRebarM == NULL)	return MainBar;
	return pWallEndRebarM->GetMainBar();
}

int MSWallMemberG::GetStartEndAddedBarSpacing()
{
	MSWallEndRebarM* pWallEndRebarM = GetWallEndRebarM(m_StartEndRebarLineID, GetFloorNum());
	if(pWallEndRebarM == NULL)	return 0;
	return pWallEndRebarM->GetSpacing();
}

int MSWallMemberG::GetEndEndAddedBarSpacing()
{
	MSWallEndRebarM* pWallEndRebarM = GetWallEndRebarM(m_EndEndRebarLineID, GetFloorNum());
	if(pWallEndRebarM == NULL)	return 0;
	return pWallEndRebarM->GetSpacing();

}

void MSWallMemberG::SetStartEndAddedBar(CString sMainBar)
{
 	MSWallEndRebarLine* pWallEndRebarLine = GetWallEndRebarLine(m_StartEndRebarLineID);
	if(pWallEndRebarLine == NULL)		return;

	pWallEndRebarLine->SetWallEndRebarM(GetFloorNum(), sMainBar);
	pWallEndRebarLine->MergeRebarMArr();
}

void MSWallMemberG::SetEndEndAddedBar(CString sMainBar)
{
	MSWallEndRebarLine* pWallEndRebarLine = GetWallEndRebarLine(m_EndEndRebarLineID);
	if(pWallEndRebarLine == NULL)		return;

	pWallEndRebarLine->SetWallEndRebarM(GetFloorNum(), sMainBar);
	pWallEndRebarLine->MergeRebarMArr();
}

void MSWallMemberG::SetStartEndAddedBarSpacing(int spacing)
{

}

void MSWallMemberG::SetEndEndAddedBarSpacing(int spacing)
{

}

CString MSWallMemberG::GetKey(bool bUseID)
{
	// ID, FloorNum, Type, CompoM, 좌표, 옵셋
	CString strCompoMKey;
	if (mp_CompoM == NULL || mp_CompoM->m_ID == 0)
		strCompoMKey.Format(_T("%d"), 0);
	else
		strCompoMKey.Format(_T("%d"), &mp_CompoM->m_ID);
	CString strGeo;
	strGeo.Format(_T("%d_%d_%d_%d"), (int)ma_Vec[0]->m_X, (int)ma_Vec[0]->m_Y, (int)ma_Vec[1]->m_X, (int)ma_Vec[1]->m_Y);
	CString strOffSet;
	strOffSet.Format(_T("%d_%d_%d_%d_%d"), (int)IsSlopeMember(), (int)m_SOffsetZ, (int)m_SOffsetZ_End, (int)m_EOffsetZ, (int)m_EOffsetZ_End);

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

bool MSWallMemberG::IsSameBWallHorBar(MSWallMemberG* pUrWall)
{
	if (IsBearingWall() == false || pUrWall == nullptr || pUrWall->IsBearingWall() == false)
		return false;

	MSRcBWallM* pMyRcBWallM = (MSRcBWallM*)GetRebarM();
	MSRcBWallM* pUrRcBWallM = (MSRcBWallM*)pUrWall->GetRebarM();

	double dMyDia1 = pMyRcBWallM->m_HBars.GetDeformedBarDiameter(0);
	double dMyDia2 = pMyRcBWallM->m_HBars.GetDeformedBarDiameter(1);
	double dMySpace = pMyRcBWallM->m_HBars.GetSpacing();
	
	double dUrDia1 = pUrRcBWallM->m_HBars.GetDeformedBarDiameter(0);
	double dUrDia2 = pUrRcBWallM->m_HBars.GetDeformedBarDiameter(1);
	double dUrSpace = pUrRcBWallM->m_HBars.GetSpacing();

	if (fabs(dMyDia1 - dUrDia1) < DTOL_GM &&
		fabs(dMyDia2 - dUrDia2) < DTOL_GM &&
		fabs(dMySpace - dUrSpace) < DTOL_GM )
		return true;

	return false;
}

bool MSWallMemberG::IsSameInfo(MSLinkMemberG* pUrLink)
{
	if (MSLinkMemberG::IsSameInfo(pUrLink) == false)
		return false;

	MSWallMemberG* pUrWall = (MSWallMemberG*)pUrLink;
	if ( (m_IsStartEndAddedBar != pUrWall->m_IsStartEndAddedBar) ||
		(m_IsEndEndAddedBar != pUrWall->m_IsEndEndAddedBar)
		)
		return false;

	MSWallEndRebarM* pWallEndRebarM_S = GetWallEndRebarM(m_StartEndRebarLineID, GetFloorNum());
	MSWallEndRebarM* pUrWallEndRebarM_S = GetWallEndRebarM(pUrWall->m_StartEndRebarLineID, GetFloorNum());

	if (pWallEndRebarM_S != NULL || pUrWallEndRebarM_S != NULL)
	{
		if (pWallEndRebarM_S == NULL)
			return false;
		if (pUrWallEndRebarM_S == NULL)
			return false;
		if (pWallEndRebarM_S->IsSameData(pUrWallEndRebarM_S) == false)
			return false;
	}

	MSWallEndRebarM* pWallEndRebarM_E = GetWallEndRebarM(m_EndEndRebarLineID, GetFloorNum());
	MSWallEndRebarM* pUrWallEndRebarM_E = GetWallEndRebarM(pUrWall->m_EndEndRebarLineID, GetFloorNum());

	if (pWallEndRebarM_E != NULL || pUrWallEndRebarM_E != NULL)
	{
		if (pWallEndRebarM_E == NULL)
			return false;
		if (pUrWallEndRebarM_E == NULL)
			return false;
		if (pWallEndRebarM_E->IsSameData(pUrWallEndRebarM_E) == false)
			return false;
	}

	return true;
}

bool MSWallMemberG::HasSubMember()
{
	if (HasLevelDif())	return true;
	if (ma_Opening.size() > 0) return true;
	if (mm_WallOpening.size() > 0)	return true;
	if(mm_ShadeG.size() > 0)	return true;
	return MSLinkMemberG::HasSubMember();
}

void MSWallMemberG::DeleteSubMembers()
{
	DeleteLowMemberLevelDifG();
	DeleteOpenings();
	DeleteShadeGs();
	MSLinkMemberG::DeleteSubMembers();
}

vector<MSCompoG*> MSWallMemberG::GetSupportSlab()
{
	vector<MSCompoG*> SlabArr = FindConnectMember(FindType_MSElement::msSlabG, FIND_ALLFLOOR);
	double dWallTopZLevel = 0;
	double dWallBotZLevel = 0;
	GetZLevels(dWallTopZLevel, dWallBotZLevel);

	for (int iSlab = SlabArr.size() - 1; iSlab >= 0 ; iSlab--)
	{
		double dSlabTopZLevel = 0;
		double dSlabBotZLevel = 0;
		SlabArr[iSlab]->GetZLevels(dSlabTopZLevel, dSlabBotZLevel);
		// 벽체의 하부 래벨이 슬래브에 포함되지 않은 경우 제거
		// 슬래브에 벽체가 포함된 경우 제거
		if(dWallBotZLevel > dSlabTopZLevel || dWallBotZLevel < dSlabBotZLevel || (dSlabTopZLevel >= dWallTopZLevel && dSlabBotZLevel <= dWallBotZLevel))
			SlabArr.erase(SlabArr.begin() + iSlab);
	}
	return SlabArr;
}

void MSWallMemberG::GetSupportCurve(vector<GM2DCurve*>& LeftCurveArr, vector<GM2DCurve*>& RightCurveArr)
{
	vector<MSCompoG*> SlabArr = GetSupportSlab();
	vector<GM2DPolyline*> SlabPolyArr;
	vector<GM2DPolyline*> UnionSlabPolyArr;

	for (int iSlab = 0; iSlab < SlabArr.size(); iSlab++)
		SlabPolyArr.push_back(SlabArr[iSlab]->GetProfile());

	AcisGeomHelper::UnionPolylines(SlabPolyArr, UnionSlabPolyArr);

	for (int iPoly = 0; iPoly < UnionSlabPolyArr.size(); iPoly++)
	{
		vector<GM2DCurve*> SplitLCurveArr;
		UnionSlabPolyArr[iPoly]->GetCurveInPolygon(mp_LLineTR, SplitLCurveArr);
		LeftCurveArr.insert(LeftCurveArr.end(), SplitLCurveArr.begin(), SplitLCurveArr.end());

		vector<GM2DCurve*> SplitRCurveArr;
		UnionSlabPolyArr[iPoly]->GetCurveInPolygon(mp_RLineTR, SplitRCurveArr);
		RightCurveArr.insert(RightCurveArr.end(), SplitRCurveArr.begin(), SplitRCurveArr.end());
	}
}

bool MSWallMemberG::Add(MSFWShadeG* shadeG)
{
	shadeG->GetID();
	shadeG->SetOwnerCompoG(this);
	mm_ShadeG.insert(make_pair(shadeG->m_ID, shadeG));
	return true;
}
bool MSWallMemberG::Remove(MSFWShadeG* shadeG)
{
	map<long, MSFWShadeG*>::iterator it = mm_ShadeG.find(shadeG->m_ID);
	if (it != mm_ShadeG.end())
		mm_ShadeG.erase(it);
	return true;

}
bool MSWallMemberG::Delete(MSFWShadeG* shadeG)
{
	Remove(shadeG);
	delete shadeG;
	return true;
}
void MSWallMemberG::DeleteShadeGs()
{
	map<long, MSFWShadeG*>::iterator it;
	for (it = mm_ShadeG.begin(); it != mm_ShadeG.end(); it++)
		delete it->second;
	mm_ShadeG.clear();

}
bool MSWallMemberG::HasShadeG()
{
	if (mm_ShadeG.size() > 0)	return true;
	return false;
}
MSFWShadeG* MSWallMemberG::GetShadeG(long ID)
{
	map<long, MSFWShadeG*>::iterator it = mm_ShadeG.find(ID);
	if (it != mm_ShadeG.end())
		return it->second;
	return nullptr;
}

bool MSWallMemberG::MakeSubMemberBoundary()
{
	for (auto it = mm_WallOpening.begin(); it != mm_WallOpening.end(); it++)
	{
		MSWallOpeningG* pWallOpeningG = it->second;
		pWallOpeningG->MakeBoundary();
		pWallOpeningG->TrimInitialize();
	}
	for( auto it = mm_ShadeG.begin() ; it != mm_ShadeG.end() ; it++)
	{
		MSFWShadeG *pShadeG = it->second;
		pShadeG->MakeBoundary();
		pShadeG->TrimInitialize();
	}
	return true;
}


bool MSWallMemberG::RemoveSubMember(MSElement* element)
{
	switch (element->GetType())
	{
	case MSElement::msWallOpeningExtraG:	return Remove((MSWallOpeningG*)element);
	case MSElement::msWindowG:return Remove((MSWallOpeningG*)element);
	case MSElement::msDoorG:return Remove((MSWallOpeningG*)element);
	case MSElement::msFWShadeG:	return Remove((MSFWShadeG*)element);
	case MSElement::msFWSwitchBoxG:	return Remove((MSFWSwitchBoxG*)element);
	}
	return MSLinkMemberG::RemoveSubMember(element);
}
void MSWallMemberG::DeleteOnlyWallOpenings()
{
	for (auto it = mm_WallOpening.begin(); it != mm_WallOpening.end();)
	{
		if (it->second->GetType() != MSElement::msFWSwitchBoxG)
			mm_WallOpening.erase(it++);
		else
			++it;
	}
}
void MSWallMemberG::DeleteSwitchBoxs()
{
	for (auto it = mm_WallOpening.begin(); it != mm_WallOpening.end();)
	{
		if (it->second->GetType() == MSElement::msFWSwitchBoxG)
			mm_WallOpening.erase(it++);
		else
			++it;
	}
}
void MSWallMemberG::DeleteHorShadeGs()
{
	for (auto it = mm_ShadeG.begin(); it != mm_ShadeG.end();)
	{
		if (!it->second->IsVerticalShade() )
			mm_ShadeG.erase(it++);
		else
			++it;
	}
}
void MSWallMemberG::DeleteVerShadeGs()
{
	for (auto it = mm_ShadeG.begin(); it != mm_ShadeG.end();)
	{
		if (it->second->IsVerticalShade())
			mm_ShadeG.erase(it++);
		else
			++it;
	}
}

bool MSWallMemberG::UsedMaterial(long materialID)
{
	long nLevelDif = ma_MemberLevelDifLow.GetSize();
	for (long iLevelDif = 0; iLevelDif < nLevelDif; iLevelDif++)
	{
		long nHaunchMaterialID = ma_MemberLevelDifLow[iLevelDif]->GetHaunchMaterialID();
		if (nHaunchMaterialID == materialID)
			return true;
	}

	return MSLinkMemberG::UsedMaterial(materialID);
}

void MSWallMemberG::SubMemberMaterialValidCheck()
{
	MSBaseBuilding* pBldg = GetBuilding();
	if (pBldg == nullptr) return;

	MSLinkMemberG::SubMemberMaterialValidCheck();

	long defMaterialID = -1;

	long NumDifG = ma_MemberLevelDifLow.GetCount();
	for (long iDifG = 0; iDifG < NumDifG; iDifG++)
	{
		if (ma_MemberLevelDifLow[iDifG]->GetIsUsedStoryMaterial() == true) continue;

		long nHaunchMaterialID = ma_MemberLevelDifLow[iDifG]->GetHaunchMaterialID();
		if (pBldg->GetMaterial(nHaunchMaterialID) != nullptr) continue;

		if (defMaterialID == -1)
		{
			MSMaterial* defMaterial = pBldg->GetDefMaterial();
			if (defMaterial == nullptr) return;
			defMaterialID = defMaterial->m_ID;
		}

		ma_MemberLevelDifLow[iDifG]->SetHaunchMaterialID(defMaterialID);
	}
}

void MSWallMemberG::InitTrimmedBoundary(bool IsTopLevel)
{
	MSLinkMemberG::InitTrimmedBoundary(IsTopLevel);
	map<long, MSWallOpeningG*>::iterator it;
	for (it = mm_WallOpening.begin(); it != mm_WallOpening.end(); it++)
	{
		MSWallOpeningG* pWallOpeningG = it->second;
		pWallOpeningG->InitTrimmedBoundary(IsTopLevel);
	}
}

CString MSWallMemberG::ToString()
{
	auto str = MSLinkMemberG::ToString();
	str.AppendFormat(L"StartOffsetZ_End : %f, EndOffsetZ_End: %f, ", m_SOffsetZ_End, m_EOffsetZ_End);
	return str;
}

bool MSWallMemberG::IsSpecialBoundaryM()
{
	MSRcBWallM* pMyRcBWallM = (MSRcBWallM*)GetRebarM();

	if (pMyRcBWallM == nullptr)
		return false;

	return pMyRcBWallM->GetIsSpecialBoundary();
}

shared_ptr<GM2DCurve> MSWallMemberG::GetLongCenCurvebySpecialRebar()
{
	// 초기값은 트림라인 기준 긴 Curve를 가지고 와서 이를 트림 Polyline으로 자른다.
	shared_ptr<GM2DCurve> cenCurve = GetLongCenCurvebyTrimmedBoundary();
	vector<GM2DPolyline*> polyTRLine = GetLinkMemberBoundaryTrimmedByColumn();

	// 초기값은 약간 넓게 잡아준다.
	double dSLoc = -1, dELoc = 2;

	if (polyTRLine.size() == 1)
	{
		// BH-4521 polyTRLine이 여러개인 경우에는 계산하지 않는다.

		// Trimmed Boundary 기준으로 Center Curve를 자른다.
		vector<GM2DVector> crossVecArr;
		vector<double> myLocArr, urLocArr;
		vector<long> resultArr;
		int nLine = polyTRLine[0]->GetNumLines();
		for (int i = 0; i < nLine; i++)
		{
			// 리턴값
			//	-1 : 외부 1점 교차
			//	-2 : 외부 2점 교차
			//	 1 : 1점 내부 교차
			//	 2 : 2점 내부 교차
			//  11 : 1점 내부, 1점 외부 교차
			GM2DCurve* polyCurve = polyTRLine[0]->GetCurve(i);
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
					{
						// 내부교차하는 경우에만 계산한다. cenCurve가 Arc인 경우 고려하기 위함.
						double loc = myLocArr[j];
						if (dSLoc < 0 || loc < dSLoc)
							dSLoc = loc;
						if (dELoc > 1 || loc > dELoc)
							dELoc = loc;
					}
				}
			}
		}
	}

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