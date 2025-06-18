#include "StdAfx.h"
#include "OrderLengthSettingItem.h"
using namespace RebarOption;

OrderLengthSettingItem::OrderLengthSettingItem(CString FromFloor, CString ToFloor, MemberType memberType, CString OrderLengthSetName)
{
	this->m_sFromFloor = FromFloor;
	this->m_sToFloor = ToFloor;
	this->m_eMemberType = memberType;
	this->m_sOrderLengthSetName = OrderLengthSetName;
	
}


OrderLengthSettingItem::~OrderLengthSettingItem(void)
{
}

void OrderLengthSettingItem::Serialize( CArchive& ar )
{
	if(ar.IsStoring())
	{
		ar << m_sFromFloor;
		ar << m_sToFloor;
		ar << (int)m_eMemberType;
		ar << m_sOrderLengthSetName;
	}
	else
	{
	
		ar >> m_sFromFloor;
		ar >> m_sToFloor;
		int type;
		ar >> type;
		m_eMemberType = (MemberType)type;
		ar >> m_sOrderLengthSetName;

	
	}
}

void OrderLengthSettingItem::Copy(OrderLengthSettingItem desOrderLengthSettingItem)
{
	m_sFromFloor = desOrderLengthSettingItem.m_sFromFloor;
	m_sToFloor = desOrderLengthSettingItem.m_sToFloor;
	m_eMemberType = desOrderLengthSettingItem.m_eMemberType;
	m_sOrderLengthSetName = desOrderLengthSettingItem.m_sOrderLengthSetName;
}
