#pragma once
#include "OrderLengthItem.h"
using namespace std;
class StructureModel_CLASS OrderLengthSet
{
public:
	OrderLengthSet();
	OrderLengthSet(CString Name);
	~OrderLengthSet(void);
	
	const OrderLengthItem* GetOrderLengthOptByName(CString name);
	void Serialize(CArchive& ar);
	void Initialize();
	void OrderLengthSetDiaValidateCheck();
	CString m_sName;
	vector<OrderLengthItem> orderLengths;

	void Copy(shared_ptr<OrderLengthSet> desOrderLengthSet);
};

