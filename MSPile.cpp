#include "stdafx.h"
#include "MSPile.h"
#include "MSRebarBuilding.h"
#include "MSFloor.h"
#include "MSConstructionZone.h"

IMPLEMENT_SERIAL(MSPile, MSElement, VERSIONABLE_SCHEMA | 1)

MSPile::MSPile()
{
	m_ID = 0;
	Init(true);
}


MSPile::~MSPile()
{
}

void MSPile::Init(bool bInConstructor)
{
	mp_Floor = nullptr;
	mp_Profile = nullptr;
	m_nBuildingInfoID = -1;
	m_ePileType = MSPile::PHC;
	m_OutsideDiameter = 1400;
	m_InsideDiameter = 900;
	m_TopLevel = 63.50;
	m_BottomLevel = 60.00;
	m_DrillLength = 3.50;
	m_HeaderHeight = 1.00;
	m_MinLength = 5;
	m_OrderLegnth = 10;

	m_SpliceLength = 0;
	m_CementKgPerMeter  = 100;

	m_EquipmentEA = 3;
	m_EfficiencyPerEquipent  = 15;
	m_DayWorkdays  = 5;
}

MSBaseBuilding* MSPile::GetBuilding()
{
	if (mp_Floor == nullptr)
		return nullptr;
	return mp_Floor->GetBuilding();
}

GM2DPolyline* MSPile::GetProfile()
{
	if (mp_Profile == nullptr)
	{
		mp_Profile = new GM2DPolyline();
		int resolution = 24;
		GM2DVector Pnt;
		GMCircle SecCircle(m_OutsideDiameter / 2., 0., 0.);
		SecCircle.m_Center = m_Location;
		double RadStep = 2.0*PI / resolution;
		for (long PntNum = 0; PntNum < resolution; PntNum++)
		{
			SecCircle.GetGlobalXY(PntNum*RadStep, Pnt.m_X, Pnt.m_Y);
			mp_Profile->AddPointAndBulge((GM2DVector*)Pnt.Copy());
		}
		mp_Profile->MakeClosedPolyline(DTOL_GM);
	}
	return mp_Profile;
}

double MSPile::GetHeight()
{
	return fabs(m_TopLevel - m_BottomLevel);
}

MSObject* MSPile::Copy(bool bNewID /*= true*/)
{
	MSPile* pNewObj = new MSPile();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSPile::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == NULL)	return;
	MSPile* ThePile = (MSPile*)pSource_in;

	mp_Floor = ThePile->GetFloor();
	m_nBuildingInfoID = ThePile->GetBuildingInfoID();
	m_Location = ThePile->GetLocation();
	m_ePileType = ThePile->GetPileType();
	m_OutsideDiameter = ThePile->GetOutsideDiameter();
	m_InsideDiameter = ThePile->GetInsideDiameter();
	m_TopLevel = ThePile->GetTopLevel();
	m_BottomLevel = ThePile->GetBottomLevel();
	m_DrillLength = ThePile->GetDrillLength();
	m_HeaderHeight = ThePile->GetHeaderHeight();
	m_MinLength = ThePile->GetMinLength();
	m_OrderLegnth = ThePile->GetOrderLength();

	m_SpliceLength = ThePile->GetSpliceLength();
	m_CementKgPerMeter = ThePile->GetCementKgPerMeter();

	m_EquipmentEA = ThePile->GetEquipmentEA();
	m_EfficiencyPerEquipent = ThePile->GetEfficiencyPerEquipment();
	m_DayWorkdays = ThePile->GetDayWorkdays();

}

void MSPile::Dump(EFS::EFS_Buffer& buffer)
{
	MSElement::Dump(buffer);
	int type = (int)GetPileType();

	buffer << m_nBuildingInfoID << type << m_OutsideDiameter << m_InsideDiameter << m_TopLevel << m_BottomLevel;
	buffer << m_DrillLength << m_HeaderHeight << m_MinLength << m_OrderLegnth << m_SpliceLength << m_CementKgPerMeter;
	buffer << m_EquipmentEA << m_EfficiencyPerEquipent << m_DayWorkdays;
	m_Location.Dump(buffer);
}

void MSPile::Recovery(EFS::EFS_Buffer& buffer)
{
	MSElement::Recovery(buffer);
	int type = 0;

	buffer >> m_nBuildingInfoID >> type >> m_OutsideDiameter >> m_InsideDiameter >> m_TopLevel >> m_BottomLevel;
	buffer >> m_DrillLength >> m_HeaderHeight >>m_MinLength >> m_OrderLegnth >> m_SpliceLength >> m_CementKgPerMeter;
	buffer >> m_EquipmentEA >> m_EfficiencyPerEquipent >> m_DayWorkdays;

	m_ePileType = ePileType(type);
	m_Location.Recovery(buffer);
}
void MSPile::Serialize(CArchive &ar)
{
	MSElement::Serialize(ar);
	int type = 0;
	if (ar.IsStoring())
	{
		ar << m_nBuildingInfoID;
		type = (int)m_ePileType;
		ar << type;
		m_Location.Serialize(ar);
		ar << m_OutsideDiameter;
		ar << m_InsideDiameter;
		ar << m_TopLevel;
		ar << m_BottomLevel;
		ar << m_DrillLength;
		ar << m_HeaderHeight;
		ar << m_MinLength;
		ar << m_OrderLegnth;
		ar << m_SpliceLength;
		ar << m_CementKgPerMeter;
		ar << m_EquipmentEA;
		ar << m_EfficiencyPerEquipent;
		ar << m_DayWorkdays;
	}
	else
	{
		ar >> m_nBuildingInfoID;
		ar >> type;
		m_Location.Serialize(ar);
		ar >> m_OutsideDiameter;
		ar >> m_InsideDiameter;
		ar >> m_TopLevel;
		ar >> m_BottomLevel;
		ar >> m_DrillLength;
		ar >> m_HeaderHeight;
		ar >> m_MinLength;
		ar >> m_OrderLegnth;
		ar >> m_SpliceLength;
		ar >> m_CementKgPerMeter;
		ar >> m_EquipmentEA;
		ar >> m_EfficiencyPerEquipent;
		ar >> m_DayWorkdays;
		m_ePileType = ePileType(type);
	}
}

int MSPile::GetCZoneID()
{
 	for (auto it = mp_Floor->mm_CZone.begin(); it != mp_Floor->mm_CZone.end(); it++)
	{
		if(it->second->GetZoneType() != MSConstructionZone::Horizontal)
			continue;
		if (it->second->Contains(m_Location.m_X, m_Location.m_Y))
			return it->first;
	}
	return mp_Floor->GetCZoneDefault(true)->m_ID;
}

CString MSPile::GetCZoneName()
{
	return mp_Floor->GetCZone(GetCZoneID())->GetName();
	return _T("");
}

int MSPile::GetBuildingInfoID()
{
	if (m_nBuildingInfoID == -1)
	{
		MSRebarBuilding* pBldg = dynamic_cast<MSRebarBuilding*>(GetBuilding());
		if (pBldg == nullptr) return m_nBuildingInfoID;

		m_nBuildingInfoID = pBldg->GetBuildingInfoID();
	}

	return m_nBuildingInfoID;
}

MSBuildingInfo* MSPile::GetBuildingInfo()
{
	MSRebarBuilding* pBldg = dynamic_cast<MSRebarBuilding*>(GetBuilding());
	if (pBldg == nullptr) return nullptr;

	int buildingInfoID = GetBuildingInfoID();
	MSBuildingInfo* buildingInfo = pBldg->GetBuildingInfoManager()->GetBuildingInfoByID(buildingInfoID);
	if (buildingInfo == nullptr)
	{
		buildingInfo = pBldg->GetBuildingInfoManager()->GetDefaultBuildingInfo();
		m_nBuildingInfoID = buildingInfo->m_ID;
	}

	return buildingInfo;
}

CString MSPile::GetBuildingInfoClassification()
{
	MSBuildingInfo* buildingInfo = GetBuildingInfo();
	if (buildingInfo == nullptr) return _T("");

	return buildingInfo->GetBuildingInfoClassification();
}

CString MSPile::GetBuildingInfoType()
{
	MSBuildingInfo* buildingInfo = GetBuildingInfo();
	if (buildingInfo == nullptr) return _T("");

	return buildingInfo->GetBuildingInfoType();
}

CString MSPile::GetBuildingInfoName()
{
	MSBuildingInfo* buildingInfo = GetBuildingInfo();
	if (buildingInfo == nullptr) return _T("");

	return buildingInfo->GetName();
}
