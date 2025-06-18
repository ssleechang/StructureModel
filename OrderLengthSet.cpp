#include "StdAfx.h"
#include "OrderLengthSet.h"
#include "MSDeformedBar.h"

OrderLengthSet::OrderLengthSet()
{
	Initialize();	
}

OrderLengthSet::OrderLengthSet(CString Name)
{
	this->m_sName = Name;
	Initialize();
}


OrderLengthSet::~OrderLengthSet(void)
{
}

void OrderLengthSet::Serialize( CArchive& ar )
{

	if(ar.IsStoring())
	{
		ar << m_sName;
		ar << (int)orderLengths.size();
		for(auto it = orderLengths.begin(); it != orderLengths.end(); it++)
		{
			(*it).Serialize(ar);
		}
	}
	else
	{
		
		ar >> m_sName;
		int size;
		ar >> size;

		orderLengths.clear();
		orderLengths.reserve(size);
		for(int i=0; i< size; i++)
		{
			OrderLengthItem item;
			item.Serialize(ar);
			orderLengths.push_back(item);
		}

		if (size > 0)
			OrderLengthSetDiaValidateCheck();
	}
}

void OrderLengthSet::OrderLengthSetDiaValidateCheck()
{
	CString sDia, sFy;
	bool bDiaFind;

	CStringArray DiaNameList;
	long numDia = MSDeformedBar::GetRebarNameList(DiaNameList);

	int nOrderLengthSize = orderLengths.size();
	for (long index = 0; index < numDia; index++)
	{
		sDia = _T("D") + DiaNameList[index];

		bDiaFind = false;
		for (long orderLengthIndex = 0; orderLengthIndex < nOrderLengthSize; orderLengthIndex++)
		{
			OrderLengthItem item = orderLengths[orderLengthIndex];

			if (sDia == item.DiaName)
			{
				bDiaFind = true;
				break;
			}
		}

		if (bDiaFind == false)
		{
			if (sDia == _T("D10") || sDia == _T("D13"))
				sFy = _T("SD40");
			else
				sFy = _T("SD60");

			orderLengths.push_back(OrderLengthItem(sDia, sFy, 8000));
		}
	}
}

const OrderLengthItem* OrderLengthSet::GetOrderLengthOptByName( CString name )
{
	auto it = find_if(orderLengths.begin(), orderLengths.end(), [name](OrderLengthItem const& n){return n.DiaName == name;});
	if(it == orderLengths.end())
		return NULL;
	else
		return &(*it);
}

void OrderLengthSet::Initialize()
{
	orderLengths.clear();

	CStringArray DiaNameList;
	long numDia = MSDeformedBar::GetRebarNameList(DiaNameList);
	CString sDia, sFy;
	for (long index = 0; index < numDia; index++)
	{
		sDia = _T("D") + DiaNameList[index];

		if (sDia == _T("D10") || sDia == _T("D13"))
			sFy = _T("SD40");
		else
			sFy = _T("SD60");

		orderLengths.push_back(OrderLengthItem(sDia, sFy, 8000));
	}
}

void OrderLengthSet::Copy(shared_ptr<OrderLengthSet> desOrderLengthSet)
{
	m_sName = desOrderLengthSet->m_sName;

	int size = desOrderLengthSet->orderLengths.size();

	orderLengths.clear();
	orderLengths.reserve(size);
	for (int i = 0; i < size; i++)
	{
		OrderLengthItem item;
		item.Copy(desOrderLengthSet->orderLengths[i]);
		orderLengths.push_back(item);
	}

	if (size > 0)
		OrderLengthSetDiaValidateCheck();
}
