#include "StdAfx.h"
#include "MSMemberLevelDif.h"
#include "GMLib/MSVersionInfo.h"
#include "MSFloor.h"
#include "MSStoryMaterial.h"
#include "MSRebarBuilding.h"


MSMemberLevelDif::MSMemberLevelDif(void)
{
	m_Loc = 0;
	m_LLoc = 0;
	m_RLoc = 0;
	m_StartLevel = 0;
	m_EndLevel = 0;
	m_InfillThick = 0;
	mp_LineLevelDifG = NULL;
	mp_OwnerCompoG = NULL;
	m_dHaunchAngle = 0;
	m_dHaunchY = 0;
	m_bUseHaunchMaterial = false;
	m_nHaunchMaterialID = 0;
	m_IsUsedStoryMaterial = false;
}


MSMemberLevelDif::~MSMemberLevelDif(void)
{
}

void MSMemberLevelDif::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_Loc << m_LLoc << m_RLoc << m_StartLevel << m_EndLevel << m_InfillThick;
		ar << m_dHaunchAngle << m_dHaunchY;
		ar << m_bUseHaunchMaterial << m_nHaunchMaterialID;
		ar << m_IsUsedStoryMaterial;
	}
	else
	{
		ar >> m_Loc >> m_LLoc >> m_RLoc >> m_StartLevel >> m_EndLevel >> m_InfillThick;
		if (MSVersionInfo::GetCurrentVersion() >= 20150621)
		{
			ar >> m_dHaunchAngle >> m_dHaunchY;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20150630)
		{
			ar >> m_bUseHaunchMaterial >> m_nHaunchMaterialID;
		}
		mp_LineLevelDifG = NULL;
		mp_OwnerCompoG = NULL;
		if (MSVersionInfo::GetCurrentVersion() >= 20160310)
		{
			ar >> m_IsUsedStoryMaterial;
		}
	}
}

void MSMemberLevelDif::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	buffer << m_Loc << m_LLoc << m_RLoc << m_StartLevel << m_EndLevel << m_InfillThick << &mp_LineLevelDifG;
	buffer << &mp_OwnerCompoG;
	buffer << m_dHaunchAngle << m_dHaunchY;
	buffer << m_bUseHaunchMaterial << m_nHaunchMaterialID << m_IsUsedStoryMaterial;
}

void MSMemberLevelDif::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	buffer >> m_Loc >> m_LLoc >> m_RLoc >> m_StartLevel >> m_EndLevel >> m_InfillThick >> &mp_LineLevelDifG;
	buffer >> &mp_OwnerCompoG;
	buffer >> m_dHaunchAngle >> m_dHaunchY;
	buffer >> m_bUseHaunchMaterial >> m_nHaunchMaterialID >> m_IsUsedStoryMaterial;
}

MSObject* MSMemberLevelDif::Copy(bool bNewID /*= true*/)
{
	MSMemberLevelDif* pNewObj;

	pNewObj = new MSMemberLevelDif();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSMemberLevelDif::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;
	MSMemberLevelDif* pSource = dynamic_cast<MSMemberLevelDif*>(pSource_in);
	if (pSource == nullptr) return;
	MSObject::CopyFromMe(pSource_in, bNewID);

	if (pSource)
	{
		m_Loc = pSource->m_Loc;
		m_LLoc = pSource->m_LLoc;
		m_RLoc = pSource->m_RLoc;
		m_StartLevel = pSource->m_StartLevel;
		m_EndLevel = pSource->m_EndLevel;
		m_InfillThick = pSource->m_InfillThick;
		mp_LineLevelDifG = pSource->mp_LineLevelDifG;
		mp_OwnerCompoG = pSource->mp_OwnerCompoG;
		m_dHaunchAngle = pSource->m_dHaunchAngle;
		m_dHaunchY = pSource->m_dHaunchY;
		m_bUseHaunchMaterial = pSource->m_bUseHaunchMaterial;
		m_nHaunchMaterialID = pSource->m_nHaunchMaterialID;
		m_IsUsedStoryMaterial = pSource->m_IsUsedStoryMaterial;
	}
}

MSMemberLevelDif& MSMemberLevelDif::operator = (const MSMemberLevelDif& src)
{
	m_Loc = src.m_Loc;
	m_LLoc = src.m_LLoc;
	m_RLoc = src.m_RLoc;
	m_StartLevel = src.m_StartLevel;
	m_EndLevel = src.m_EndLevel;
	m_InfillThick = src.m_InfillThick;
	mp_LineLevelDifG = src.mp_LineLevelDifG;
	mp_OwnerCompoG = src.mp_OwnerCompoG;
	m_dHaunchAngle = src.m_dHaunchAngle;
	m_dHaunchY = src.m_dHaunchY;
	m_bUseHaunchMaterial = src.m_bUseHaunchMaterial;
	m_nHaunchMaterialID = src.m_nHaunchMaterialID;
	m_IsUsedStoryMaterial = src.m_IsUsedStoryMaterial;

	return *this;
}

MSMemberLevelDif::MSMemberLevelDif(const MSMemberLevelDif& src)
{
	(*this) = src;
}

void MSMemberLevelDif::ReverseData()
{
	m_Loc = 1 - m_Loc;
	m_LLoc = 1 - m_LLoc;
	m_RLoc = 1 - m_RLoc;
	double dTmp = m_StartLevel;
	m_StartLevel = m_EndLevel;
	m_EndLevel = dTmp;
}

double MSMemberLevelDif::GetLevelDist()
{
	return abs(m_StartLevel - m_EndLevel);
}

long MSMemberLevelDif::GetHaunchMaterialID()
{
	if (m_IsUsedStoryMaterial)
	{
		MSFloor* pFloor = mp_LineLevelDifG->GetFloor();

		if (pFloor == NULL)
		{
			ASSERT(0);
			return m_nHaunchMaterialID;
		}
		else
		{
			MSBaseBuilding* pBldg = pFloor->GetBuilding();
			MSStoryMaterial* pStoryMaterial = pBldg->GetStoryMaterialByFloorNum(pFloor->GetFloorNum());
			if (pStoryMaterial == NULL)
			{
				ASSERT(0);
				return m_nHaunchMaterialID;
			}

			MSMaterial* pMaterial = pStoryMaterial->GetMaterial(MSMaterial::msConcrete, false);
			if (pMaterial)	return pMaterial->m_ID;
		}
	}
	return m_nHaunchMaterialID;
}

GM2DVector MSMemberLevelDif::GetStartVec()
{
	MSLinkMemberG* pLinkG = (MSLinkMemberG*)mp_OwnerCompoG;
	shared_ptr<GM2DCurve> CenCurve = pLinkG->GetCenCurve();

	return CenCurve->PointAtParam(GetStartLoc());
}

GM2DVector MSMemberLevelDif::GetEndVec()
{
	MSLinkMemberG* pLinkG = (MSLinkMemberG*)mp_OwnerCompoG;
	shared_ptr<GM2DCurve> CenCurve = pLinkG->GetCenCurve();

	return CenCurve->PointAtParam(GetEndLoc());
}

GM2DVector MSMemberLevelDif::GetMidVec()
{
	MSLinkMemberG* pLinkG = (MSLinkMemberG*)mp_OwnerCompoG;
	double dLen = pLinkG->GetLength();
	shared_ptr<GM2DCurve> CenCurve = pLinkG->GetCenCurve();

	double dLoc = 0;
	if (m_StartLevel < m_EndLevel)
		dLoc = m_Loc + ((m_InfillThick / dLen)/2);
	else
		dLoc = m_Loc - ((m_InfillThick / dLen)/2);

	return CenCurve->PointAtParam(dLoc);
}

double MSMemberLevelDif::GetStartLoc()
{
	MSLinkMemberG* pLinkG = (MSLinkMemberG*)mp_OwnerCompoG;
	double dLen = pLinkG->GetLength();
	shared_ptr<GM2DCurve> CenCurve = pLinkG->GetCenCurve();

	double dLoc = 0;
	if (m_StartLevel < m_EndLevel)
		dLoc = m_Loc;
	else
		dLoc = m_Loc - (m_InfillThick / dLen);
	
	return dLoc;
}

double MSMemberLevelDif::GetEndLoc()
{
	MSLinkMemberG* pLinkG = (MSLinkMemberG*)mp_OwnerCompoG;
	double dLen = pLinkG->GetLength();
	shared_ptr<GM2DCurve> CenCurve = pLinkG->GetCenCurve();

	double dLoc = 0;
	if (m_StartLevel < m_EndLevel)
		dLoc = m_Loc + (m_InfillThick / dLen);
	else
		dLoc = m_Loc;

	return dLoc;
}