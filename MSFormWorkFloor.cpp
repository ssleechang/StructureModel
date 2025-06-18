#include "stdafx.h"
#include "MSFormWorkFloor.h"
#include "MSFormWorkDesignUnit.h"

IMPLEMENT_SERIAL(MSFormWorkFloor, MSFloor, VERSIONABLE_SCHEMA | 1)
MSFormWorkFloor::MSFormWorkFloor()
{
	m_pOwnerUnit = nullptr;
	m_nFloorCount = 1;
}


MSFormWorkFloor::~MSFormWorkFloor()
{
	m_pOwnerUnit = nullptr;
}

void MSFormWorkFloor::SetOwnerUnit(MSFormWorkDesignUnit* pOwnerUnit)
{
	m_pOwnerUnit = pOwnerUnit;
}
void MSFormWorkFloor::Dump(EFS::EFS_Buffer& buffer)
{
	MSFloor::Dump(buffer);
	buffer << m_nFloorCount;
}

void MSFormWorkFloor::Recovery(EFS::EFS_Buffer& buffer)
{
	MSFloor::Recovery(buffer);
	buffer >> m_nFloorCount;
}

MSFloor* MSFormWorkFloor::GetUpperFloor()
{
	if (GetOwnerUnit()->GetWorkingFloor() == this)
		return GetOwnerUnit()->GetTopFloor();
	else if(GetOwnerUnit()->GetBottomFloor() == this)
		return GetOwnerUnit()->GetWorkingFloor();
	
	return nullptr;
}

MSFloor* MSFormWorkFloor::GetLowerFloor()
{
	if (GetOwnerUnit()->GetWorkingFloor() == this)
		return GetOwnerUnit()->GetBottomFloor();
	else if (GetOwnerUnit()->GetTopFloor() == this)
		return GetOwnerUnit()->GetWorkingFloor();

	return nullptr;
}

MSObject* MSFormWorkFloor::Copy(bool bNewID /*= true*/)
{
	MSFormWorkFloor* pNewObj = new MSFormWorkFloor();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSFormWorkFloor::CopyFromMe(MSFormWorkFloor* pSource, bool bNewID/*=true*/)
{
	if (pSource == NULL) return;
	m_pOwnerUnit = pSource->m_pOwnerUnit;
	m_nFloorCount = pSource->m_nFloorCount;
	MSFloor::CopyFromMe(pSource, bNewID);
}
void MSFormWorkFloor::Serialize(CArchive &ar)
{
	MSFloor::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_nFloorCount;
	}
	else
	{
		if (MSVersionInfo::GetCurrentVersion() >= 20200917)
			ar >> m_nFloorCount;

		InitQT();
		ResetFloorQTLeaves();
	}
}

double MSFormWorkFloor::GetFloorLevel()
{
	MSFormWorkDesignUnit* pFWDU = GetOwnerUnit();
	if (pFWDU == nullptr)
		return 0;

	return pFWDU->GetFloorLevel(this);
}

double MSFormWorkFloor::GetSlabLevel()
{
	return GetFloorLevel() + GetSlabLevelOffset();
}

vector<MSFloor*> MSFormWorkFloor::GetFloorsByLevel(double dStartLevel, double dEndLevel)
{
	MSFormWorkDesignUnit* pFWDU = GetOwnerUnit();
	if (pFWDU == nullptr)
		return MSFloor::GetFloorsByLevel(dStartLevel, dEndLevel);

	return pFWDU->GetFloorsByLevel(dStartLevel, dEndLevel);
}

int MSFormWorkFloor::GetFloorCount()
{
	return m_nFloorCount;
}

void MSFormWorkFloor::SetFloorCount(int floorCount)
{
	m_nFloorCount = floorCount;
}