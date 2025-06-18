#pragma once
#include "OrderLengthSettingItem.h"

using namespace std;
class OrderLengthSet;

class StructureModel_CLASS OrderLengthSettingSet
{
public:
	OrderLengthSettingSet();
	OrderLengthSettingSet(CString Name);
	~OrderLengthSettingSet(void);
	void Initialize();
	void Serialize(CArchive& ar);

	CString GetOrderLength(long nFloorNum) const;
	void SetDefaultOrderLengthSet(shared_ptr<OrderLengthSet> pOrderLengthSet);
	
	CString m_sName;
	CString m_sDefaultOrderLengthSetName;
	shared_ptr<OrderLengthSet> m_pDefaultOrderLengthSet;
	vector<OrderLengthSettingItem> orderLengthSettings;

	void Copy(shared_ptr<OrderLengthSettingSet> desOrderLengthSettingSet);
};

