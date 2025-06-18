#include "stdafx.h"
#include "MSFormWorkDesignUnit.h"
#include "MSFormWorkFloor.h"
#include "MSFormWorkBuilding.h"

IMPLEMENT_SERIAL(MSFormWorkDesignUnit, MSElement, VERSIONABLE_SCHEMA | 1)
MSFormWorkDesignUnit::MSFormWorkDesignUnit(double dHeight)
{
	m_pBuilding = nullptr;
	m_Name = _T("");
	m_pTopFloor = new MSFormWorkFloor();
	m_pWorkingFloor = new MSFormWorkFloor();
	m_pBottomFloor = new MSFormWorkFloor();

	m_pTopFloor->SetName(_T("Top Floor"));
	m_pWorkingFloor->SetName(_T("Working Floor"));
	m_pBottomFloor->SetName(_T("Bottom Floor"));

	m_pTopFloor->SetOwnerUnit(this);
	m_pWorkingFloor->SetOwnerUnit(this);
	m_pBottomFloor->SetOwnerUnit(this);

	m_pTopFloor->SetFloorNum(3);
	m_pWorkingFloor->SetFloorNum(2);
	m_pBottomFloor->SetFloorNum(1);

    m_pTopFloor->SetHeight(dHeight);
    m_pWorkingFloor->SetHeight(dHeight);
    m_pBottomFloor->SetHeight(dHeight);

	m_pTopFloor->GetID();
	m_pWorkingFloor->GetID();
	m_pBottomFloor->GetID();
}


MSFormWorkDesignUnit::~MSFormWorkDesignUnit()
{
	DeleteFloors();
}

void MSFormWorkDesignUnit::DeleteFloors()
{
	delete m_pTopFloor;
	delete m_pWorkingFloor;
	delete m_pBottomFloor;

	m_pTopFloor = nullptr;
	m_pWorkingFloor = nullptr;
	m_pBottomFloor = nullptr;
}

void MSFormWorkDesignUnit::SetBuilding(MSFormWorkBuilding* pBuilding)
{
	m_pBuilding = pBuilding;
 	m_pTopFloor->SetOwner(pBuilding);
 	m_pWorkingFloor->SetOwner(pBuilding);
 	m_pBottomFloor->SetOwner(pBuilding);
}
void MSFormWorkDesignUnit::Dump(EFS::EFS_Buffer& buffer)
{
	MSElement::Dump(buffer);
	buffer << m_Name;
	m_pTopFloor->Dump(buffer);
	m_pWorkingFloor->Dump(buffer);
	m_pBottomFloor->Dump(buffer);
}

void MSFormWorkDesignUnit::Recovery(EFS::EFS_Buffer& buffer)
{
	MSElement::Recovery(buffer);
	buffer >> m_Name;
	m_pTopFloor->Recovery(buffer);
	m_pWorkingFloor->Recovery(buffer);
	m_pBottomFloor->Recovery(buffer);
}

MSObject* MSFormWorkDesignUnit::Copy(bool bNewID /*= true*/)
{
	MSFormWorkDesignUnit* pNewObj = new MSFormWorkDesignUnit();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSFormWorkDesignUnit::CopyFromMe(MSFormWorkDesignUnit* pSource, bool bNewID/*=true*/)
{
	if (pSource == NULL) return;
	m_Name = pSource->m_Name;
	m_pTopFloor->CopyFromMe(pSource->GetTopFloor());
	m_pWorkingFloor->CopyFromMe(pSource->GetWorkingFloor());
	m_pBottomFloor->CopyFromMe(pSource->GetBottomFloor());
	MSElement::CopyFromMe(pSource, bNewID);
}
void MSFormWorkDesignUnit::Serialize(CArchive &ar)
{
	MSElement::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_Name;
		m_pTopFloor->Serialize(ar);
		m_pWorkingFloor->Serialize(ar);
		m_pBottomFloor->Serialize(ar);
	}
	else
	{
		ar >> m_Name;
		m_pTopFloor->Serialize(ar);
		m_pWorkingFloor->Serialize(ar);
		m_pBottomFloor->Serialize(ar);

		m_pBottomFloor->RecoverPointerAfterSerialize();
		m_pWorkingFloor->RecoverPointerAfterSerialize();
		m_pTopFloor->RecoverPointerAfterSerialize();
	}
}

MSFormWorkFloor* MSFormWorkDesignUnit::GetFloorByID(long nID)
{
	if (m_pTopFloor->m_ID == nID)
		return m_pTopFloor;
	if (m_pWorkingFloor->m_ID == nID)
		return m_pWorkingFloor;
	if (m_pBottomFloor->m_ID == nID)
		return m_pBottomFloor;

	return nullptr;
}

void MSFormWorkDesignUnit::SetFloorIDs()
{
    m_pTopFloor->GetID();
    m_pWorkingFloor->GetID();
    m_pBottomFloor->GetID();
}

double MSFormWorkDesignUnit::GetFloorLevel(MSFormWorkFloor* pFloor)
{
	// Floor가 1~3층까지 만 존재한다.
	// 기준 층고는 0으로 설정 
	// MSFloor::GetFloorLevel() 함수 호출하면 안된다.

	double dGroundLevel = 0;
	double dTotalHeight = dGroundLevel;

	if (m_pBottomFloor == pFloor)
		return dTotalHeight;
	else if (m_pWorkingFloor == pFloor)
	{
		dTotalHeight += m_pBottomFloor->GetHeight();
		return dTotalHeight;
	}
	else if (m_pTopFloor == pFloor)
	{
		dTotalHeight += m_pBottomFloor->GetHeight();
		dTotalHeight += m_pWorkingFloor->GetHeight();
		return dTotalHeight;
	}

	return dTotalHeight;
}

vector<MSFloor*> MSFormWorkDesignUnit::GetFloorsByLevel(double dStartLevel, double dEndLevel)
{
	vector<MSFloor*> FloorArr;

	vector<MSFloor*> tempFloorArr;
	tempFloorArr.push_back(m_pBottomFloor);
	tempFloorArr.push_back(m_pWorkingFloor);
	tempFloorArr.push_back(m_pTopFloor);

	map<long, MSFloor*>::iterator itFloor;
	for (int iFloor = 0 ; iFloor < tempFloorArr.size(); iFloor++)
	{
		MSFloor* pFloor = tempFloorArr[iFloor];
		double dFloorBotLevel = pFloor->GetBotCompoGZLevel();
		double dFloorTopLevel = pFloor->GetTopCompoGZLevel();
		if (dStartLevel >= dFloorBotLevel && dStartLevel <= dFloorTopLevel)
			FloorArr.push_back(pFloor);
		else if (dEndLevel >= dFloorBotLevel && dEndLevel <= dFloorTopLevel)
			FloorArr.push_back(pFloor);
	}

	return FloorArr;
}
