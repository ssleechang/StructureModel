#include "StdAfx.h"
#include "MSLineMemberG.h"

#include "MSLinkMemberG.h"
#include "MSLineMemberM.h"
#include "MSContainer.h"
#include "MSFloor.h"
#include "MSRebarBuilding.h"

MSLineMemberG::MSLineMemberG()
{
	m_dAxisRotation = 0.0; // mylee
}

MSLineMemberG::~MSLineMemberG(void)
{
}

// bool MSLineMemberG::GetZLevels( double& TopLevel, double& BottomLevel, bool GetBigVal )
// {
// 	double dTopOffSetZ = 0, dBotOffSetZ = 0;
// 	dTopOffSetZ = max(m_SOffsetZ, m_EOffsetZ);
// 	dBotOffSetZ = min(m_SOffsetZ, m_EOffsetZ);
// 
//     if(mp_Owner && mp_Owner->IsFloor())
//     {
//         MSFloor* pCurFloor = (MSFloor*)mp_Owner;
// 		TopLevel = GetFloorLevel()+((MSFloor*)mp_Owner)->GetHeight() + dTopOffSetZ + GetLevelbyLevelDif(true);
//     }
//     else
// 	    TopLevel = GetFloorLevel() + dTopOffSetZ;
// 	BottomLevel = GetFloorLevel()+((MSFloor*)mp_Owner)->GetHeight() + dTopOffSetZ + GetLevelbyLevelDif(false) - GetDepth();
// 	return true;
// }

// double MSLineMemberG::GetOffSetbyPos(char cPos)
// {
// 	Type_MSCompoG MyType = GetType();
// 	if(MyType == MSElement::msWallFootG || MyType == MSElement::msHandRailG)
// 		return m_SOffsetZ;
// 
// 	if(cPos == 'S')
// 		return m_SOffsetZ;
// 	else if(cPos == 'E')
// 		return m_EOffsetZ;
// 	else
// 	{
// 		// 추가 작업 해야함
// 		ASSERT(0);
// 	}
// 	return 0;
// }
// 
// bool MSLineMemberG::GetZLevelsbyPos(double& TopLevel, double& BottomLevel, char cPos)
// {
// 	double dOffSet = GetOffSetbyPos(cPos);
// 	double dLevelZoneZ = 0;
// 
// 	if(mp_Owner && mp_Owner->IsFloor())
// 	{
// 		MSFloor* pCurFloor = (MSFloor*)mp_Owner;
// 		GM2DVector* pVec = NULL;
// 		if(cPos == 'S')
// 			pVec = GetSVec();
// 		else if(cPos == 'E')
// 			pVec = GetEVec();
// 		else
// 			return false;
// 
// 		if(ma_MemberLevelDif.GetSize() > 0) // 조인트의 X,Y 좌표가 래벨존에 포함되나 MemberLevelDif를 생성안하는 경우가 존재함 --> 평행할경우
// 			dLevelZoneZ = pCurFloor->GetLevel4LevelZone(pVec->m_X, pVec->m_Y);
// 		else
// 			dLevelZoneZ = pCurFloor->GetLevel4LevelZone(this);
// 
// 		TopLevel = GetFloorLevel()+((MSFloor*)mp_Owner)->GetHeight() + dOffSet + dLevelZoneZ;
// 	}
// 	else
// 		TopLevel = GetFloorLevel()+m_SOffsetZ;
// 	BottomLevel = TopLevel-GetDepth();
// 	return true;
// }

// double MSLineMemberG::GetOffSetbyLoc(double dLoc)
// {
// 	double dOffSet = 0;
// 
// 	if(dLoc <= 0)
// 		return m_SOffsetZ;
// 	else if(dLoc >= 1)
// 		return m_EOffsetZ;
// 	else
// 		dOffSet = (m_EOffsetZ - m_SOffsetZ)* dLoc + m_SOffsetZ;
// 
// 	return dOffSet;
// }
// 
// 
// bool MSLineMemberG::GetZLevelsbyLoc(double& TopLevel, double& BottomLevel, double dLoc)
// {
// 	double dOffSet = GetOffSetbyLoc(dLoc);
// 	double dLevelZoneZ = 0;
// 
// 	if(mp_Owner && mp_Owner->IsFloor())
// 	{
// 		MSFloor* pCurFloor = (MSFloor*)mp_Owner;
// 		GM2DVector TheVec = NULL;
// 		GM2DCurve* pCenCurve = GetCenLineVec();
// 		if(dLoc <= 0)
// 			TheVec = pCenCurve->PointAtParam(0);
// 		else if(dLoc >= 1)
// 			TheVec = pCenCurve->PointAtParam(1);
// 		else
// 			TheVec = pCenCurve->PointAtParam(dLoc);
// 
// 		delete pCenCurve;
// 
// 		if(GetType() == MSElement::msLineLevelDifG)
// 			dLevelZoneZ = pCurFloor->GetLevel4LevelZone(TheVec.m_X, TheVec.m_Y);
// 		else
// 		{
// 			long NumDif = ma_MemberLevelDif.GetSize();
// 			if(NumDif > 0)
// 				dLevelZoneZ = pCurFloor->GetLevel4LevelZone(TheVec.m_X, TheVec.m_Y);
// 			else // 레벨존 안에 완전히 포함 또는 레벨존에 포함 되지 않음
// 				dLevelZoneZ = GetFloor()->GetLevel4LevelZone(this);
// 		}
// 
// 		TopLevel = GetFloorLevel()+((MSFloor*)mp_Owner)->GetHeight() + dOffSet + dLevelZoneZ;
// 	}
// 	else
// 		TopLevel = GetFloorLevel()+dOffSet;
// 	BottomLevel = TopLevel-GetDepth();
// 	return true;
// }

void MSLineMemberG::MirrorByAxis( GM2DLineVector& pAxis )
{
	MSLinkMemberG::MirrorByAxis(pAxis);
	m_dAxisRotation *= -1.;
}