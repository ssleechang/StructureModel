#include "StdAfx.h"
#include "OrderLengthSettingSet.h"
#include "OrderLengthSet.h"

using namespace RebarOption;
OrderLengthSettingSet::OrderLengthSettingSet()
{
	this->m_sName = "";
}

OrderLengthSettingSet::OrderLengthSettingSet(CString Name)
{
	this->m_sName = Name;
}


OrderLengthSettingSet::~OrderLengthSettingSet(void)
{
}

void OrderLengthSettingSet::Serialize( CArchive& ar )
{

	if(ar.IsStoring())
	{
		ar << m_sName;
		ar << (int) orderLengthSettings.size();
		for(auto it = orderLengthSettings.begin(); it != orderLengthSettings.end(); it++)
		{
			(*it).Serialize(ar);
		}
		ar << m_sDefaultOrderLengthSetName;
	}
	else
	{

		ar >> m_sName;
		int size;
		ar >> size;
		orderLengthSettings.clear();
		orderLengthSettings.reserve(size);
		for( int i=0; i< size; i++)
		{
			OrderLengthSettingItem item;
			item.Serialize(ar);
			orderLengthSettings.push_back(item);
		}
		ar >> m_sDefaultOrderLengthSetName;
	}
}

void OrderLengthSettingSet::Initialize()
{
	
}

CString OrderLengthSettingSet::GetOrderLength(long nFloorNum ) const
{
	long nFromFloor = 0;
	long nToFloor = 0;

	vector<OrderLengthSettingItem>::const_iterator it;
	for(it = orderLengthSettings.begin() ; it != orderLengthSettings.end() ;it++)
	{
		nFromFloor = _ttol(it->m_sFromFloor);
		nToFloor = _ttol(it->m_sToFloor);
		if(nFromFloor<=nFloorNum && nToFloor>=nFloorNum)
		{
			if(it->m_eMemberType == RebarOption::OL_ALL)
				return it->m_sOrderLengthSetName;

		}

	}
	if(m_pDefaultOrderLengthSet)
		return m_pDefaultOrderLengthSet->m_sName;
	return m_sDefaultOrderLengthSetName;

}

void OrderLengthSettingSet::SetDefaultOrderLengthSet(shared_ptr<OrderLengthSet> pOrderLengthSet)
{
	m_sDefaultOrderLengthSetName = pOrderLengthSet->m_sName;
	m_pDefaultOrderLengthSet = pOrderLengthSet;
}

void OrderLengthSettingSet::Copy(shared_ptr<OrderLengthSettingSet> desOrderLengthSettingSet)
{
	m_sName = desOrderLengthSettingSet->m_sName;

	int size = desOrderLengthSettingSet->orderLengthSettings.size();
	orderLengthSettings.clear();
	orderLengthSettings.reserve(size);
	for (int i = 0; i < size; i++)
	{
		OrderLengthSettingItem item;
		item.Copy(desOrderLengthSettingSet->orderLengthSettings[i]);
		orderLengthSettings.push_back(item);
	}
	m_sDefaultOrderLengthSetName = desOrderLengthSettingSet->m_sDefaultOrderLengthSetName;
}