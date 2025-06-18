#pragma once
namespace RebarOption
{
	enum MemberType
	{
		OL_ALL, OL_BEAM, OL_COLUMN, OL_WALL, OL_SLAB, //Order Length Member Type 
	};
}

class StructureModel_CLASS OrderLengthSettingItem
{
public:
	OrderLengthSettingItem(){};
	OrderLengthSettingItem(CString FromFloor, CString ToFloor, RebarOption::MemberType memberType, CString OrderLengthSetName);	
	~OrderLengthSettingItem(void);

	CString m_sFromFloor;
	CString m_sToFloor;
	RebarOption::MemberType m_eMemberType;	
	CString m_sOrderLengthSetName;
	
	void Serialize(CArchive& ar);

	void Copy(OrderLengthSettingItem desOrderLengthSettingItem);
};

