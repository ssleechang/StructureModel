#pragma once
#include "MSElement.h"

class MSBuildingInfo;
class StructureModel_CLASS MSPile :public MSElement
{
public:
	MSPile();
	~MSPile();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSPile)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);
	virtual void Init(bool bInConstructor = false);
	virtual Type_MSCompoG GetType() { return msPile; };

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	MSFloor* GetFloor() { return mp_Floor; };
	void SetFloor(MSFloor* pFloor) { mp_Floor = pFloor; };
	virtual GM2DPolyline* GetProfile() override;
	double GetHeight();
	int GetCZoneID();
	CString GetCZoneName();
	MSBuildingInfo* GetBuildingInfo();
	CString GetBuildingInfoClassification();
	CString GetBuildingInfoType();
	CString GetBuildingInfoName();

	enum ePileType { PHC = 0, SPC};

	virtual MSBaseBuilding* GetBuilding();
	int GetBuildingInfoID();
	ePileType GetPileType() { return m_ePileType; };
	GM2DVector GetLocation() { return m_Location; };
	double GetOutsideDiameter() { return m_OutsideDiameter; };
	double GetInsideDiameter() { return m_InsideDiameter; };
	double GetTopLevel() { return m_TopLevel; };
	double GetBottomLevel() { return m_BottomLevel; };
	double GetDrillLength() { return m_DrillLength; };
	double GetHeaderHeight() { return m_HeaderHeight; };
	double GetMinLength() { return m_MinLength; };
	double GetOrderLength() { return m_OrderLegnth; };
	double GetSpliceLength() { return m_SpliceLength; };
	double GetCementKgPerMeter() { return m_CementKgPerMeter; };

	int GetEquipmentEA() { return m_EquipmentEA; };
	double GetEfficiencyPerEquipment() { return m_EfficiencyPerEquipent;};
	double GetDayWorkdays() { return m_DayWorkdays; };

	void SetBuildingInfoID(int param) { m_nBuildingInfoID = param; };
	void SetPileType(ePileType type) { m_ePileType = type; };
	void SetLocation(GM2DVector locaction) { m_Location = locaction; };
	void SetOutsideDiameter(double param) { m_OutsideDiameter = param; };
	void SetInsideDiameter(double param) { m_InsideDiameter = param; };
	void SetTopLevel(double param) { m_TopLevel = param; };
	void SetBottomLevel(double param) { m_BottomLevel = param; };
	void SetDrillLength(double param) { m_DrillLength = param; };
	void SetHeaderHeight(double param) { m_HeaderHeight = param; };
	void SetMinLength(double param) { m_MinLength = param; };
	void SetOrderLength(double param) { m_OrderLegnth = param; };
	void SetSpliceLength(int param) { m_SpliceLength = param; };
	void SetCementKgPerMeter(double param) { m_CementKgPerMeter = param; };

	void SetEquipmentEA(int param) { m_EquipmentEA = param; };
	void SetEfficiencyPerEquipment(double param) { m_EfficiencyPerEquipent = param; };
	void SetDayWorkdays(double param) { m_DayWorkdays = param; };

private:
	int m_nBuildingInfoID;
	MSFloor* mp_Floor;
	GM2DVector m_Location;
	GM2DPolyline* mp_Profile;
	ePileType m_ePileType;
	double m_OutsideDiameter;		// 외경(mm)
	double m_InsideDiameter;			// 내경(mm)
	double m_TopLevel;					// 상부레벨(m)
	double m_BottomLevel;				// 하부레벨(m)
	double m_DrillLength;				// 천공길이(m)
	double m_HeaderHeight;			// 두부높이(m)
	double m_MinLength;				// 최소길이(m)
	double m_OrderLegnth;				// 주문길이(m)
	double m_SpliceLength;				// 파일이음개수 -> 길이로 변경(with 최선웅)
	double m_CementKgPerMeter;// 길이당 시멘트양(kg/m)

	int m_EquipmentEA;								// 장비투입개수
	double m_EfficiencyPerEquipent;			// 장비당 작업효율
	double m_DayWorkdays;						// 주간근무일수

};

