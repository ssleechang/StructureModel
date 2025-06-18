#include "StdAfx.h"
#include "MSVerticalLineMemberG.h"
#include "MSFloor.h"
#include "MSRebarBuilding.h"

IMPLEMENT_SERIAL(MSVerticalLineMemberG, MSPointMemberG, VERSIONABLE_SCHEMA | 1)

MSVerticalLineMemberG::MSVerticalLineMemberG()
{
	Init(true);
}

MSVerticalLineMemberG::~MSVerticalLineMemberG(void)
{
}


void MSVerticalLineMemberG::Dump(EFS::EFS_Buffer& buffer)
{
	MSPointMemberG::Dump(buffer);

	buffer << m_EOffsetZ;
}

void MSVerticalLineMemberG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSPointMemberG::Recovery(buffer);
	buffer >> m_EOffsetZ;
}


void MSVerticalLineMemberG::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;
	MSPointMemberG::CopyFromMe(pSource_in, bNewID);

	if( !pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() == msNULL) { ASSERT(0); return; }
	MSVerticalLineMemberG* pSource = (MSVerticalLineMemberG*)pSource_in;

    m_EOffsetZ = pSource->m_EOffsetZ;
}


void MSVerticalLineMemberG::Serialize( CArchive& ar)
{
	MSPointMemberG::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_EOffsetZ;
	}
	else
	{
		ar >> m_EOffsetZ;
	}
}

void MSVerticalLineMemberG::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSPointMemberG::Init(bInConstructor);

	m_EOffsetZ = 0;
}

bool MSVerticalLineMemberG::GetZLevels( double& TopLevel, double& BottomLevel, bool GetBigVal, bool bCheckLevelZone )
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
		double dThick = 0;
		if(mp_CompoM != NULL)
			dThick = mp_CompoM->GetThick();
		BottomLevel = GetSlabLevel() + m_SOffsetZ;

		TopLevel = GetNextSlabLevel() + m_EOffsetZ;

	}

	if (BottomLevel > TopLevel)
	{
		double dTmp = BottomLevel;
		BottomLevel = TopLevel;
		TopLevel = dTmp;
	}

	return true;
}

double MSVerticalLineMemberG::GetEndLevel(bool isMax)
{
	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;
	return GetNextSlabLevel()+GetRelativeEndLevel(isMax);

}
double MSVerticalLineMemberG::GetRelativeEndLevel(bool isMax)
{

	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;
	MSFloor* pFloor = (MSFloor*)mp_Owner;
	if(pFloor == NULL)
		return m_EOffsetZ;

	return pFloor->GetLevel4LevelZone(ma_Vec[0]->m_X, ma_Vec[0]->m_Y, isMax)+m_EOffsetZ;
}
double MSVerticalLineMemberG::GetStartLevel(bool isMax)
{
	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;
	return GetSlabLevel()+GetRelativeStartLevel(isMax);
}
double MSVerticalLineMemberG::GetRelativeStartLevel(bool isMax)
{
	if(mp_Owner->GetType()!=MSElement::msFloor)
		return 0.;
	MSFloor* pFloor = (MSFloor*)mp_Owner;
	if(pFloor == NULL)
		return m_SOffsetZ;
	MSFloor* pLowFloor = pFloor->GetLowerFloor();
	if (pLowFloor == NULL)
		return m_SOffsetZ;

	return pLowFloor->GetLevel4LevelZone(ma_Vec[0]->m_X, ma_Vec[0]->m_Y, isMax) + m_SOffsetZ;
}

double MSVerticalLineMemberG::GetHeight()
{
	return GetEndLevel() - GetStartLevel();
}