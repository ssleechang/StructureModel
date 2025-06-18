#pragma once

class StructureModel_CLASS OrderLengthItem
{
public:
	OrderLengthItem(){};
	OrderLengthItem(CString DiaName, CString Fy, int orderLength);
	~OrderLengthItem(void);

	CString DiaName;
	CString FyName;
	int orderLength;

	static vector<CString> GetFyNames();

	void Serialize(CArchive& ar);

	void Copy(OrderLengthItem desOrderLengthItem);
};

