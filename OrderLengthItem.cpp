#include "StdAfx.h"
#include "OrderLengthItem.h"

OrderLengthItem::OrderLengthItem(CString DiaName, CString FyName, int orderLength)
{
	this->DiaName = DiaName;
	this->FyName = FyName;
	this->orderLength = orderLength;
}


OrderLengthItem::~OrderLengthItem(void)
{
}

void OrderLengthItem::Serialize( CArchive& ar )
{
	if(ar.IsStoring())
	{
		ar << DiaName;
		ar << FyName;
		ar << orderLength;
	}
	else
	{
		
		ar >> DiaName;
		ar >> FyName;
		ar >> orderLength;
		
		
	}
}

vector<CString> OrderLengthItem::GetFyNames()
{
	vector<CString> FyNames;
	FyNames.reserve(3);
	FyNames.push_back(L"SD40");
	FyNames.push_back(L"SD50");
	FyNames.push_back(L"SD60");
	return FyNames;
}

void OrderLengthItem::Copy(OrderLengthItem desOrderLengthItem)
{
	DiaName = desOrderLengthItem.DiaName;
	FyName = desOrderLengthItem.FyName;
	orderLength = desOrderLengthItem.orderLength;
}
