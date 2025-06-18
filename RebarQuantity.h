#pragma once

#include <string>
#include "Structuremodelenum.h"
using std::string;
#include "EFS_Buffer.h"
class MSCompoG;
class StructureModel_CLASS RebarQuantityManager
{
public:
	
	static RebarQuantityManager Instance;
	int GetNumLParam(int GCode,int SCode);
private:
	RebarQuantityManager(void);
	
	void AddRebarShape(int code, int numLparam, int numRparam = 0);
	void InitRebarShapeArr();
	map<int, int> mapCodeToNumLParam;
};

enum eRebarQuantityType  { Actual, Planned };

class StructureModel_CLASS RebarQuantity :public CObject
{

public:
	RebarQuantity(void);
	virtual ~RebarQuantity(void);

	RebarQuantity(const RebarQuantity& src);
	RebarQuantity& operator=(const RebarQuantity& src);
public:

	
	//여기서 정의하는 타입은 2DShopPro에서 세팅하는 타입들이다.
	enum eQuantityMemberType { Etc = 0, Elev_Pit, Foundation, Column, Column_Dowel, Wall, Wall_Dowel, Slab, Girder, Stairs };
	enum eSeparateType { New = 0, Preexistence, Exclusion };

	void SetGCode(int gCode);
	void SetSCode(int sCode);
	void SetCode(int gCode, int sCode);
	
	void SetStrength(CString strength);
	void SetDiaName(CString sDiaName);
	void SetNumBar(int numBar);
	void SetLength(int length);
	void SetQuantityType(eQuantityMemberType type);
	void SetLParams(vector<int> lParams);
	void SetRParams(vector<int> rParams);

	void ParsingData(CString data);
	int ParsingTextData(CString text, vector<CString>& dataArr, char parser);

	int GetGCode();
	int GetSCode();

	CString GetBarSetType();
	void SetBarSetType(CString type);
	
	vector<int> GetLParams();
	vector<int> GetRParams();
	CString GetStrength();
	CString GetDiaName();
	int GetNumBar();
	int GetLength();
	double GetWeight();//Ton
	eQuantityMemberType GetQuantityType();
	CString GetQuantityTypeString();
	CString GetLocation();
	CString RebarQuantity::GetLocationForRebarSummary();

	eQuantityMemberType GetQuantityType(CString layerName);
	//CString GetQuantityTypeString();
	CString GetQuantityTypeLayerName();
	
	vector<long> GetMemberIDs();
	void SetMemberIDs(vector<long>& memberIDs);
	CString GetMemberNamesForView();
	void SetMemberNamesForView(CString memberNames);
	CString GetMemberIDsForView();
	void SetMemberIDsForView(CString memberIDs);

	
	void Serialize(CArchive& ar);
	void Dump(EFS::EFS_Buffer& ar);
	void Recovery(EFS::EFS_Buffer& ar);
	
	// 이 함수에서 나오는 타입은 Foler Tree생성시 만드는 타입이름들이다.
	static CString GetQuantityTypeStringByRAPCompoGType(MSCompoG* compoG);
	static CString GetQuantityTypeStringByQuantityType(eQuantityMemberType quantityType, CString floorName, bool DowelFoundationClassfy);
	static bool IsNotContainBottomFloorByLayerName(CString layerName);
	static bool IsVerticalRebarQuantityByLayerName(CString layerName);
	static bool IsHorizontalRebarQuantityByLayerName(CString layerName);
	static bool IsNotWallRebarQuantityByLayerName(CString layerName);

	CString GetSector();
	void SetSector(CString sector);
	CString GetOriginBuilding();
	void SetOriginBuilding(CString originBuilding);
	CString GetOriginFloor();
	void SetOriginFloor(CString originFloor);
	CString GetDrawingName();
	void SetDrawingName(CString drawingName);
	CString GetMember();
	void SetMember(CString member);
	eSeparateType GetSeparateType();
	void SetSeparateType(eSeparateType separateType);

	void SetBuilding(int buildingID, CString buildingName);
	int GetBuildingID();
	CString GetBuildingName();

	void SetFloor(int floorNum, CString floorName);
	int GetFloorNum();
	CString GetFloorName();

	void SetCZone(int cZoneID, CString cZoneName);
	int GetCZoneID();
	CString GetCZoneName();
	
	void SetBarType(int barTypeID, CString barTypeName);
	int GetBarTypeID();
	CString GetBarTypeName();

	CString GetQTMemberName();
	void SetQTMemberName(CString QTMemberName);

	int GetBarSetRootType();
	void SetBarSetRootType(int barSetRootType);

	Type_RebarOwnerClassification GetRebarOwnerClassification();
	void SetRebarOwnerClassification(Type_RebarOwnerClassification value);

	void SetCustomWeight(double customWeight);

	CString GetActualCZoneName();
	void SetActualCZoneName(CString actualCZoneName);

	bool GetIsLowerFloorMatchingMember();
	void SetIsLowerFloorMatchingMember(bool IsLowerFloorMatchingMember);

protected:
	int _GCode;
	int _SCode;
	vector<int> _BarShapeLParams;
	vector<int> _BarShapeRParams;
	CString _Location; // BuildingName\FloorName\ZoneName\TypeName\ID, e.g.> Building,   Building\1F,   Building\1F\Zone A\Beam 
	CString _Strength;
	CString _BarSetType;
	CString _DiaName;
	int _Length;
	int _NumBar;
	eQuantityMemberType _QuantityType;
	vector<long> _MemberIDs;//quantity에 해당되는 모든 부재 ID
	CString _MemberNamesForView;
	CString _MemberIDsForView;

	CString _Sector;
	CString _OriginBuilding;
	CString _OriginFloor;
	CString _DrawingName;
	CString _Member;

	CString _BuildingName;
	CString _FloorName;
	CString _CZoneName;
	CString _BarTypeName;

	CString _QTMemberName;

	eSeparateType _SeparateType;
	int _BuildingID;
	int _FloorNum;
	int _CZoneID;
	int _BarTypeID;
	int _BarSetRootType;

	double _CustomWeight;
	CString _ActualCZoneName;
	bool _IsLowerFloorMatchingMember;

	Type_RebarOwnerClassification _RebarOwnerClassification;
};

enum ePlanFieldType { PlanBuilding = 0, PlanFloor, CZone, BarType };
enum eActualFieldType { ActualBuilding = 0, ActualFloor, DrawingName, Member };
enum eConditionFieldType { Equal = 0, Contains};

class StructureModel_CLASS RebarQuantityAutoMatchingSet
{
public:
	RebarQuantityAutoMatchingSet(void);
	~RebarQuantityAutoMatchingSet(void);

	RebarQuantityAutoMatchingSet& operator=(const RebarQuantityAutoMatchingSet& src);

	ePlanFieldType GetPlanFieldType();
	void SetPlanFieldType(ePlanFieldType planFieldType);
	eActualFieldType GetActualFieldType();
	void SetActualFieldType(eActualFieldType actualFieldType);
	eConditionFieldType GetConditionFieldType();
	void SetConditionFieldType(eConditionFieldType conditionFieldType);

	void Dump(EFS::EFS_Buffer& ar);
	void Recovery(EFS::EFS_Buffer& ar);
	void Serialize(CArchive& ar);

protected:
	ePlanFieldType _PlanFieldType;
	eActualFieldType _ActualFieldType;
	eConditionFieldType _ConditionFieldType;
};

class StructureModel_CLASS RebarQuantityInfo
{
public:
	vector<RebarQuantity>& GetRebarQuantities(eRebarQuantityType type)
	{
		if (type == Actual)
			return m_aActualQuantity;
		else
			return m_aPlannedQuantity;
	}

	vector<RebarQuantity> m_aPlannedQuantity;
	vector<RebarQuantity> m_aActualQuantity;

	vector<RebarQuantityAutoMatchingSet> m_aAutoMatchingSets;

	void Serialize(CArchive& ar);

	void Dump(EFS::EFS_Buffer& ar);

	void Recovery(EFS::EFS_Buffer& ar);

	void ClearActualQuantity();
	void ClearPlannedQuantity();
	void ClearAllQuantity();
	void ClearAutoMatchingSets();
};

