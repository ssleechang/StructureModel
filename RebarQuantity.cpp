#include "stdafx.h"
#include "MSRcSlabM.h"
#include "RebarQuantity.h"
#include "GMLib/MSVersionInfo.h"
#include "MSDeformedBar.h"
#include "MSCompoG.h"
RebarQuantity::RebarQuantity(void)
{
	_GCode = 0;
	_SCode = 0;
	_Strength = _T("None");
	_BarSetType = _T("");
	_DiaName = _T("None");
	_Length = 0;
	_NumBar = 0;
	_Location = _T("");
	_QuantityType = RebarQuantity::Etc;
	_MemberIDs.clear();
	_MemberNamesForView = _T("");
	_MemberIDsForView = _T("");

	_Sector = _T("");
	_OriginBuilding = _T("");
	_OriginFloor = _T("");
	_DrawingName = _T("");
	_Member = _T("");
	_SeparateType = eSeparateType::New;
	_BuildingID = -1;
	_FloorNum = 0;
	_CZoneID = -1;
	_BarTypeID = -1;
	_BarSetRootType = -1;

	_BuildingName = _T("");
	_FloorName = _T("");
	_CZoneName = _T("");
	_BarTypeName = _T("");

	_QTMemberName = _T("");

	_CustomWeight = -1;
	_ActualCZoneName = _T("");
	_IsLowerFloorMatchingMember = false;
}

RebarQuantity::~RebarQuantity(void)
{
}

CString RebarQuantity::GetLocation()
{
	CString sLocation = _T("");;
	sLocation.Format(_T("%ls\\%ls\\%ls"), _BuildingName, _FloorName, _CZoneName);

	return sLocation;
}

CString RebarQuantity::GetLocationForRebarSummary()
{
	CString sLocation = _T("");;
	sLocation.Format(_T("Building\\%ls\\%ls\\%ls"), _FloorName, _CZoneName, _QTMemberName);

	return sLocation;
}

void RebarQuantity::SetGCode(int gCode)
{
	_GCode = gCode;
	
}
void RebarQuantity::SetSCode(int sCode)
{
	
	_SCode = sCode;
}
void RebarQuantity::SetCode(int gCode, int sCode)
{
	_GCode = gCode;
	_SCode = sCode;
}

void RebarQuantity::SetStrength(CString strength)
{
	_Strength = strength;
}
void RebarQuantity::SetDiaName(CString sDiaName)
{
	_DiaName = sDiaName;
}
void RebarQuantity::SetNumBar(int numBar)
{
	_NumBar = numBar;
}

void RebarQuantity::SetLength(int length)
{
	_Length = length;
}

void RebarQuantity::SetQuantityType(RebarQuantity::eQuantityMemberType type)
{
	_QuantityType = type;
}

void RebarQuantity::SetLParams(vector<int> lParams)
{
	_BarShapeLParams.resize((int)lParams.size());
	std::copy(lParams.begin(), lParams.end(), _BarShapeLParams.begin());
}

void RebarQuantity::SetRParams(vector<int> rParams)
{
	_BarShapeRParams.resize((int)rParams.size());
	std::copy(rParams.begin(), rParams.end(), _BarShapeRParams.begin());
}

void RebarQuantity::ParsingData(CString data)
{
	int numLength = data.GetLength();
	int firstPlus = data.Find('+');
	int shapeIdx = data.Find('e');

	int RebarCode = _ttol(data.Mid(shapeIdx + 1, (firstPlus - shapeIdx)));
	int GCode = RebarCode / 1000;
	int SCode = RebarCode % 1000;

	SetCode(GCode, SCode);

	CString sLengthData = data.Mid(firstPlus + 1, numLength - (firstPlus + 1));
	vector<CString> dataArr;
	int numData = ParsingTextData(sLengthData, dataArr, '+');//TODO: CString Tokenize로 대체할 것

	int numLParam = RebarQuantityManager::Instance.GetNumLParam(_GCode, _SCode);
	for (int iData = 0; iData < numData; iData++)
	{
		int param = _ttoi(dataArr[iData]);
		if (iData<numLParam)
			_BarShapeLParams.push_back(param);
		else
			_BarShapeRParams.push_back(param);
	}
}

CString RebarQuantity::GetBarSetType()
{
	return _BarSetType;
}
void RebarQuantity::SetBarSetType(CString type)
{
	_BarSetType = type;

}

vector<long> RebarQuantity::GetMemberIDs()
{
	return _MemberIDs;
}

void RebarQuantity::SetMemberIDs(vector<long>& memberIDList)
{
	_MemberIDs.resize((int)memberIDList.size());
	std::copy(memberIDList.begin(), memberIDList.end(), _MemberIDs.begin());
}

CString RebarQuantity::GetMemberNamesForView()
{
	return _MemberNamesForView;
}

void RebarQuantity::SetMemberNamesForView(CString memberNamesForView)
{
	_MemberNamesForView = memberNamesForView;
}

CString RebarQuantity::GetMemberIDsForView()
{
	return _MemberIDsForView;
}

void RebarQuantity::SetMemberIDsForView(CString memberIDsForView)
{
	_MemberIDsForView = memberIDsForView;
}

//TODO: CString Tokenize로 대체할 것
int RebarQuantity::ParsingTextData(CString text, vector<CString>& dataArr, char token)
{
	int length = text.GetLength();
	if(length == 0)	return 0;
	int startIdx = 0;
	for (int i = 0; i < length; i++)
	{
		if (text.GetAt(i) == token)
		{
			CString data = text.Mid(startIdx, (i - startIdx));
			dataArr.push_back(data);
			startIdx = i + 1;
		}
	}
	CString data = text.Mid(startIdx, (length - startIdx));
	dataArr.push_back(data);
	return (int)dataArr.size();
}

int RebarQuantity::GetGCode()
{
	return _GCode;
}
int RebarQuantity::GetSCode()
{
	return _SCode;
}


vector<int> RebarQuantity::GetLParams()
{
	int numLParam = RebarQuantityManager::Instance.GetNumLParam(_GCode, _SCode);

	return _BarShapeLParams;
}

vector<int> RebarQuantity::GetRParams()
{
	return _BarShapeRParams;
}
CString RebarQuantity::GetStrength()
{
	return _Strength;
}
CString RebarQuantity::GetDiaName()
{
	return _DiaName;
}
int RebarQuantity::GetNumBar()
{
	return _NumBar;
}

int RebarQuantity::GetLength()
{
	return _Length;
}

RebarQuantity::eQuantityMemberType RebarQuantity::GetQuantityType()
{
	return _QuantityType;
}

CString RebarQuantity::GetSector()
{
	return _Sector;
}
void RebarQuantity::SetSector(CString sector)
{
	_Sector = sector;
}
CString RebarQuantity::GetOriginBuilding()
{
	return _OriginBuilding;
}
void RebarQuantity::SetOriginBuilding(CString originBuilding)
{
	_OriginBuilding = originBuilding;
}
CString RebarQuantity::GetOriginFloor()
{
	return _OriginFloor;
}
void RebarQuantity::SetOriginFloor(CString originFloor)
{
	_OriginFloor = originFloor;
}
CString RebarQuantity::GetDrawingName()
{
	return _DrawingName;
}
void RebarQuantity::SetDrawingName(CString drawingName)
{
	_DrawingName = drawingName;
}
CString RebarQuantity::GetMember()
{
	return _Member;
}
void RebarQuantity::SetMember(CString member)
{
	_Member = member;
}
RebarQuantity::eSeparateType RebarQuantity::GetSeparateType()
{
	return _SeparateType;
}
void RebarQuantity::SetSeparateType(eSeparateType separateType)
{
	_SeparateType = separateType;
}
void RebarQuantity::SetBuilding(int buildingID, CString buildingName)
{
	_BuildingID = buildingID;
	_BuildingName = buildingName;
}
int RebarQuantity::GetBuildingID()
{
	return _BuildingID;
}
CString RebarQuantity::GetBuildingName()
{
	return _BuildingName;
}
void RebarQuantity::SetFloor(int floorNum, CString floorName)
{
	_FloorNum = floorNum;
	_FloorName = floorName;
}
int RebarQuantity::GetFloorNum()
{
	return _FloorNum;
}
CString RebarQuantity::GetFloorName()
{
	return _FloorName;
}
void RebarQuantity::SetCZone(int cZoneID, CString cZoneName)
{
	_CZoneID = cZoneID;
	_CZoneName = cZoneName;
}
int RebarQuantity::GetCZoneID()
{
	return _CZoneID;
}
CString RebarQuantity::GetCZoneName()
{
	return _CZoneName;
}
void RebarQuantity::SetBarType(int barTypeID, CString barTypeName)
{
	_BarTypeID = barTypeID;
	_BarTypeName = barTypeName;
}
int RebarQuantity::GetBarTypeID()
{
	return _BarTypeID;
}
CString RebarQuantity::GetBarTypeName()
{
	return _BarTypeName;
}
int RebarQuantity::GetBarSetRootType()
{
	return _BarSetRootType;
}
void RebarQuantity::SetBarSetRootType(int barSetRootType)
{
	_BarSetRootType = barSetRootType;
}
Type_RebarOwnerClassification RebarQuantity::GetRebarOwnerClassification()
{
	return _RebarOwnerClassification;
}
void RebarQuantity::SetRebarOwnerClassification(Type_RebarOwnerClassification value)
{
	_RebarOwnerClassification = value;
}
CString RebarQuantity::GetQTMemberName()
{
	return _QTMemberName;
}
void RebarQuantity::SetQTMemberName(CString QTMemberName)
{
	_QTMemberName = QTMemberName;
}

void RebarQuantity::SetCustomWeight(double customWeight)
{
	_CustomWeight = customWeight;
}

CString RebarQuantity::GetActualCZoneName()
{
	return _ActualCZoneName;
}
void RebarQuantity::SetActualCZoneName(CString actualCZoneName)
{
	_ActualCZoneName = actualCZoneName;
}

bool RebarQuantity::GetIsLowerFloorMatchingMember()
{
	return _IsLowerFloorMatchingMember;
}
void RebarQuantity::SetIsLowerFloorMatchingMember(bool IsLowerFloorMatchingMember)
{
	_IsLowerFloorMatchingMember = IsLowerFloorMatchingMember;
}

RebarQuantity::eQuantityMemberType RebarQuantity::GetQuantityType(CString layerName)
{
	if(layerName == STR_NX_TABLE)
		return RebarQuantity::Etc;
	else if(layerName == STR_NX_TABLE_ELEV_PIT)
		return RebarQuantity::Elev_Pit;
	else if(layerName == STR_NX_TABLE_COLUMN)
		return RebarQuantity::Column;
	else if(layerName == STR_NX_TABLE_COLUMN_DOWEL)
		return RebarQuantity::Column_Dowel;
	else if(layerName == STR_NX_TABLE_WALL)
		return RebarQuantity::Wall;
	else if(layerName == STR_NX_TABLE_WALL_DOWEL)
		return RebarQuantity::Wall_Dowel;
	else if(layerName == STR_NX_TABLE_SLAB)
		return RebarQuantity::Slab;
	else if(layerName == STR_NX_TABLE_STAIRS)
		return RebarQuantity::Stairs;
	else if(layerName == STR_NX_TABLE_FOUNDATION)
		return RebarQuantity::Foundation;
	else if (layerName == STR_NX_TABLE_GIRDER)
		return RebarQuantity::Girder;
	else
	{
		ASSERT(0);
		return RebarQuantity::Etc;
	}
}


CString RebarQuantity::GetQuantityTypeString() {
	switch (this->_QuantityType)
	{
		case RebarQuantity::Elev_Pit:
			return L"Elev_Pit";
		case RebarQuantity::Foundation:
			return L"Foundation";
		case RebarQuantity::Column:
			return L"Column";
		case RebarQuantity::Column_Dowel:
			return L"Column_Dowel";
		case RebarQuantity::Wall:
			return L"Wall";
		case RebarQuantity::Wall_Dowel:
			return L"Wall_Dowel";
		case RebarQuantity::Slab:
			return L"Slab";
		case RebarQuantity::Girder:
			return L"Girder";
		case RebarQuantity::Stairs:
			return L"Stairs";
		default:
			return L"Etc";
	};
}

CString RebarQuantity::GetQuantityTypeStringByRAPCompoGType(MSCompoG* compoG)
{
	MSElement::Type_MSCompoG compoGType = compoG->GetType();
	switch (compoGType)
	{
	case MSElement::Type_MSCompoG::msBeamG:
	case MSElement::Type_MSCompoG::msBraceG:
		return L"Beam";

	case MSElement::Type_MSCompoG::msWallG:
	{
		if (compoG->IsRetainingWall())
			return L"RetainingWall";
		else
			return L"Wall";
	}
	case MSElement::Type_MSCompoG::msColumnG:
		return L"Column";
	case MSElement::Type_MSCompoG::msWallColumnG:
		return L"Wall";
	case MSElement::Type_MSCompoG::msIsoFootG:
	{
		if (compoG->IsDropPanel()) return L"Slab";
		else return L"Footing";
	}
	case MSElement::Type_MSCompoG::msWallFootG:
	case MSElement::Type_MSCompoG::msPitG:
		return L"Footing";

	case MSElement::Type_MSCompoG::msRampG:
	case MSElement::Type_MSCompoG::msSlabG:
	case MSElement::Type_MSCompoG::msLineLevelDifG:
	{
		MSRcSlabM *pSlabM = (MSRcSlabM*)compoG->GetRebarM();
		if(pSlabM != nullptr)
		{
			if(pSlabM->m_nSlabMType == MSRcSlabM::MAT_TYPE || pSlabM->m_nSlabMType == MSRcSlabM::SOG)
				return L"Footing";
		}
		return L"Slab";
	}

	case MSElement::Type_MSCompoG::msStairG:
		return L"Stair";

	case MSElement::Type_MSCompoG::msHandRailG:
		return L"LowWall";


	default:
	{
		ASSERT(0);
		return L"Etc";
	}
	}

}

CString RebarQuantity::GetQuantityTypeStringByQuantityType(eQuantityMemberType quantityType, CString floorName, bool DowelFoundationClassfy)
{
	//enum eQuantityMemberType { Etc = 0, Elev_Pit, Foundation, Column, Column_Dowel, Wall, Wall_Dowel, Slab, Girder, Stairs };
	switch (quantityType)
	{
		case RebarQuantity::Etc:	return L"Etc";
		case RebarQuantity::Foundation: 		return L"Footing";
		case RebarQuantity::Column:	return L"Column";
		case RebarQuantity::Column_Dowel:
		{
			CString type = L"Column";
			if(DowelFoundationClassfy == true)
				type = L"Footing";

			return type;
		}
		case RebarQuantity::Wall: 		return L"Wall";
		case RebarQuantity::Wall_Dowel:
		{
			CString type = L"Wall";
			if (DowelFoundationClassfy == true)
				type = L"Footing";

			return type;
		}
		case RebarQuantity::Slab: 		return L"Slab";
		case RebarQuantity::Girder: 		return L"Beam";
		case RebarQuantity::Stairs:	return L"Stair";
		case RebarQuantity::Elev_Pit:
		{
			//CString type = L"Slab";
			//if(floorName == L"BOTTOM")//if floor is BOTTOM
			//	type = L"Footing";

			//return type;
			return L"Footing";
		}
		default:
		{
			ASSERT(0);
			return L"Etc";
		}
	}
}

bool RebarQuantity::IsNotContainBottomFloorByLayerName(CString layerName)
{
	if (layerName == STR_NX_TABLE_COLUMN)
		return true;
	if (layerName == STR_NX_TABLE_WALL)
		return true;
	if (layerName == STR_NX_TABLE_STAIRS)
		return true;
	if (layerName == STR_NX_TABLE)
		return true;
	return false;
}

bool RebarQuantity::IsVerticalRebarQuantityByLayerName(CString layerName)
{
	if (layerName == STR_NX_TABLE_COLUMN || layerName == STR_NX_TABLE_COLUMN_DOWEL)
		return true;
	if (layerName == STR_NX_TABLE_WALL || layerName == STR_NX_TABLE_WALL_DOWEL)
		return true;
	if (layerName == STR_NX_TABLE_STAIRS)
		return true;
	if (layerName == STR_NX_TABLE)
		return true;
	return false;
}

bool RebarQuantity::IsHorizontalRebarQuantityByLayerName(CString layerName)
{
	if (layerName == STR_NX_TABLE_SLAB || layerName == STR_NX_TABLE_GIRDER)
		return true;
	if (layerName == STR_NX_TABLE_FOUNDATION || layerName == STR_NX_TABLE_ELEV_PIT)
		return true;
	if (layerName == STR_NX_TABLE)
		return true;

	return false;
}

bool RebarQuantity::IsNotWallRebarQuantityByLayerName(CString layerName)
{
	if (layerName == STR_NX_TABLE_COLUMN || layerName == STR_NX_TABLE_COLUMN_DOWEL)
		return false;
	if (layerName == STR_NX_TABLE_STAIRS)
		return false;
	if (layerName == STR_NX_TABLE_SLAB || layerName == STR_NX_TABLE_GIRDER)
		return false;
	if (layerName == STR_NX_TABLE_FOUNDATION || layerName == STR_NX_TABLE_ELEV_PIT)
		return false;

	return true;
}

//CString RebarQuantity::GetQuantityTypeString()
//{
//	switch (_QuantityType)
//	{
//	case RebarQuantity::Etc:			return L"Etc";
//	case RebarQuantity::Elev_Pit:		return L"Footing";
//	case RebarQuantity::Column:			return L"Column";
//	case RebarQuantity::Column_Dowel:	
//		//return L"Column Dowel";
//		return L"Column";
//	case RebarQuantity::Wall:			return L"Wall";
//	case RebarQuantity::Wall_Dowel:		
//		return L"Wall";
//		//return L"Wall Dowel";
//	case RebarQuantity::Slab:			return L"Slab";
//	case RebarQuantity::Girder:			return L"Beam";
//	case RebarQuantity::Stairs:			return L"Stair";
//	case RebarQuantity::Foundation:			return L"Footing";
//	default:
//		{
//			ASSERT(0);
//			return L"Undefined";
//		}
//	}
//	
//}

CString RebarQuantity::GetQuantityTypeLayerName()
{
	switch (_QuantityType)
	{
	case RebarQuantity::Etc:							return STR_NX_TABLE;
	case RebarQuantity::Elev_Pit:					return STR_NX_TABLE_ELEV_PIT;
	case RebarQuantity::Column:					return STR_NX_TABLE_COLUMN;
	case RebarQuantity::Column_Dowel:	return STR_NX_TABLE_COLUMN_DOWEL;
	case RebarQuantity::Wall:						return STR_NX_TABLE_WALL;
	case RebarQuantity::Wall_Dowel:			return STR_NX_TABLE_WALL_DOWEL;
	case RebarQuantity::Slab:						return STR_NX_TABLE_SLAB;
	case RebarQuantity::Girder:					return STR_NX_TABLE_GIRDER;
	case RebarQuantity::Stairs:						return STR_NX_TABLE_STAIRS;
	case RebarQuantity::Foundation:			return STR_NX_TABLE_FOUNDATION;
	default:
	{
		ASSERT(0);
	}
	}
	return STR_NX_TABLE;
}


void RebarQuantity::Dump(EFS::EFS_Buffer& ar)
{
	ar << _GCode;
	ar << _SCode;
	//ar << (int)barShapeParams.size(); // EFS_Buffer << operator가 복사 방식의 인자(size()리턴값은 lvalue가 아님)를 받는 것이 없으므로 이런식으로 쓸 수 없다. (참조/포인터 만 받는다)
	int size = _BarShapeLParams.size();
	ar << size;
	for (auto it = _BarShapeLParams.begin(); it != _BarShapeLParams.end(); it++)
		ar << *it;

	size = _BarShapeRParams.size();
	ar << size;
	for (auto it = _BarShapeRParams.begin(); it != _BarShapeRParams.end(); it++)
		ar << *it;
	ar << _Strength;
	ar << _DiaName;
	ar << _Length;
	ar << _NumBar;
	ar << _Location;
	int type = (int)_QuantityType;
	ar << type;
	ar << _BarSetType;
	
	size = _MemberIDs.size();
	ar << size;
	for (auto it = _MemberIDs.begin(); it != _MemberIDs.end(); it++)
		ar << *it;
	ar << _MemberNamesForView;
	ar << _MemberIDsForView;

	ar << _Sector;
	ar << _OriginBuilding;
	ar << _OriginFloor;
	ar << _DrawingName;
	ar << _Member;
	int nSeparateType = (int)_SeparateType;
	ar << nSeparateType;
	ar << _BuildingID;
	ar << _FloorNum;
	ar << _CZoneID;
	ar << _BarTypeID;
	ar << _BarSetRootType;

	ar << _BuildingName;
	ar << _FloorName;
	ar << _CZoneName;
	ar << _BarTypeName;

	ar << _QTMemberName;

	int nRebarOwnerClassification = (int)_RebarOwnerClassification;
	ar << nRebarOwnerClassification;

	ar << _CustomWeight;
	ar << _ActualCZoneName;
	ar << _IsLowerFloorMatchingMember;
}

void RebarQuantity::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << _GCode;
		ar << _SCode;
		ar << (int)_BarShapeLParams.size();;
		for (auto it = _BarShapeLParams.begin(); it != _BarShapeLParams.end(); it++)
			ar << *it;
		ar << (int)_BarShapeRParams.size();;
		for (auto it = _BarShapeRParams.begin(); it != _BarShapeRParams.end(); it++)
			ar << *it;
		ar << _Strength;
		ar << _DiaName;
		ar << _Length;
		ar << _NumBar;
		ar << _Location;
		int type = (int)_QuantityType;
		ar << type;
		ar<< _BarSetType;

		ar << (int)_MemberIDs.size();
		for (auto it = _MemberIDs.begin(); it != _MemberIDs.end(); it++)
			ar << *it;
		ar << _MemberNamesForView;
		ar << _MemberIDsForView;

		ar << _Sector;
		ar << _OriginBuilding;
		ar << _OriginFloor;
		ar << _DrawingName;
		ar << _Member;
		int nSeparateType = (int)_SeparateType;
		ar << nSeparateType;
		ar << _BuildingID;
		ar << _FloorNum;
		ar << _CZoneID;
		ar << _BarTypeID;
		ar << _BarSetRootType;

		ar << _BuildingName;
		ar << _FloorName;
		ar << _CZoneName;
		ar << _BarTypeName;

		ar << _QTMemberName;

		int nRebarOwnerClassification = (int)_RebarOwnerClassification;
		ar << nRebarOwnerClassification;

		ar << _CustomWeight;
		ar << _ActualCZoneName;
		ar << _IsLowerFloorMatchingMember;
	}
	else
	{
		_BarShapeLParams.clear();
		_BarShapeRParams.clear();
		ar >> _GCode;
		ar >> _SCode;
		int size = 0;
		ar >> size;
		_BarShapeLParams.resize(size);
		for (int i = 0; i < size; i++)
			ar >> _BarShapeLParams[i];
		ar >> size;
		_BarShapeRParams.resize(size);
		for (int i = 0; i < size; i++)
			ar >> _BarShapeRParams[i];
		ar >> _Strength;

		if(MSVersionInfo::GetCurrentVersion() < 20160106)
		{
			int oldData = 0;
			ar >> oldData;
			_DiaName.Format(_T("%ld"), oldData);
		}
		else
		{
			ar >> _DiaName;
		}
		ar >> _Length;
		ar >> _NumBar;

		
		if(MSVersionInfo::GetCurrentVersion() >= 20150318)
		{
			ar >> _Location;
			int type = 0;
			ar >> type;
			_QuantityType = (eQuantityMemberType)type;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20150520)
		{
			ar >> _BarSetType;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20160317)
		{
			_MemberIDs.clear();
			ar >> size;
			_MemberIDs.resize(size);
			for (int i = 0; i < size; i++)
				ar >> _MemberIDs[i];
			ar >> _MemberNamesForView;
			ar >> _MemberIDsForView;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20200728)
		{
			ar >> _Sector;
			ar >> _OriginBuilding;
			ar >> _OriginFloor;
			ar >> _DrawingName;
			ar >> _Member;
			int nSeparateType = 0;
			ar >> nSeparateType;
			_SeparateType = (eSeparateType)nSeparateType;
			ar >> _BuildingID;
			ar >> _FloorNum;
			ar >> _CZoneID;
			ar >> _BarTypeID;
			ar >> _BarSetRootType;

			ar >> _BuildingName;
			ar >> _FloorName;
			ar >> _CZoneName;
			ar >> _BarTypeName;

			ar >> _QTMemberName;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20200729)
		{
			int nRebarOwnerClassification = 0;
			ar >> nRebarOwnerClassification;
			_RebarOwnerClassification = (Type_RebarOwnerClassification)nRebarOwnerClassification;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20200731)
		{
			ar >> _CustomWeight;
			ar >> _ActualCZoneName;
			ar >> _IsLowerFloorMatchingMember;
		}
	}
}

void RebarQuantity::Recovery(EFS::EFS_Buffer& ar)
{
	_BarShapeLParams.clear();
	_BarShapeRParams.clear();
	ar >> _GCode;
	ar >> _SCode;
	int size = 0;
	ar >> size;
	_BarShapeLParams.resize(size);
	for (int i = 0; i < size; i++)
		ar >> _BarShapeLParams[i];
	ar >> size;
	_BarShapeRParams.resize(size);
	for (int i = 0; i < size; i++)
		ar >> _BarShapeRParams[i];
	ar >> _Strength;
	ar >> _DiaName;
	ar >> _Length;
	ar >> _NumBar;
	ar >> _Location;
	int type = 0;
	ar >> type;
	_QuantityType = (eQuantityMemberType)type;
	ar >> _BarSetType;
	
	_MemberIDs.clear();
	ar >> size;
	_MemberIDs.resize(size);
	for (int i = 0; i < size; i++)
		ar >> _MemberIDs[i];
	ar >> _MemberNamesForView;
	ar >> _MemberIDsForView;

	ar >> _Sector;
	ar >> _OriginBuilding;
	ar >> _OriginFloor;
	ar >> _DrawingName;
	ar >> _Member;
	int nSeparateType = 0;
	ar >> nSeparateType;
	_SeparateType = (eSeparateType)nSeparateType;
	ar >> _BuildingID;
	ar >> _FloorNum;
	ar >> _CZoneID;
	ar >> _BarTypeID;
	ar >> _BarSetRootType;

	ar >> _BuildingName;
	ar >> _FloorName;
	ar >> _CZoneName;
	ar >> _BarTypeName;

	ar >> _QTMemberName;

	int nRebarOwnerClassification = 0;
	ar >> nRebarOwnerClassification;
	_RebarOwnerClassification = (Type_RebarOwnerClassification)nRebarOwnerClassification;

	ar >> _CustomWeight;
	ar >> _ActualCZoneName;
	ar >> _IsLowerFloorMatchingMember;
}


RebarQuantity& RebarQuantity::operator=(const RebarQuantity& src)
{
	_Strength = src._Strength;
	_DiaName = src._DiaName;
	_Length = src._Length;
	_NumBar = src._NumBar;
	_GCode = src._GCode;
	_SCode = src._SCode;
	_Location = src._Location;
	_BarShapeLParams.resize((int)src._BarShapeLParams.size());
	std::copy(src._BarShapeLParams.begin(), src._BarShapeLParams.end(), _BarShapeLParams.begin());
	_BarShapeRParams.resize((int)src._BarShapeRParams.size());
	std::copy(src._BarShapeRParams.begin(), src._BarShapeRParams.end(), _BarShapeRParams.begin());
	_QuantityType = src._QuantityType;
	_BarSetType = src._BarSetType;
	
	_MemberIDs.resize((int)src._MemberIDs.size());
	std::copy(src._MemberIDs.begin(), src._MemberIDs.end(), _MemberIDs.begin());
	_MemberNamesForView = src._MemberNamesForView;
	_MemberIDsForView = src._MemberIDsForView;

	_Sector = src._Sector;
	_OriginBuilding = src._OriginBuilding;
	_OriginFloor = src._OriginFloor;
	_DrawingName = src._DrawingName;
	_Member = src._Member;
	_SeparateType = src._SeparateType;
	_BuildingID = src._BuildingID;
	_FloorNum = src._FloorNum;
	_CZoneID = src._CZoneID;
	_BarTypeID = src._BarTypeID;
	_BarSetRootType = src._BarSetRootType;

	_BuildingName = src._BuildingName;
	_FloorName = src._FloorName;
	_CZoneName = src._CZoneName;
	_BarTypeName = src._BarTypeName;

	_QTMemberName = src._QTMemberName;
	_RebarOwnerClassification = src._RebarOwnerClassification;

	_CustomWeight = src._CustomWeight;
	_ActualCZoneName = src._ActualCZoneName;
	_IsLowerFloorMatchingMember = src._IsLowerFloorMatchingMember;

	return (*this);
}

RebarQuantity::RebarQuantity(const RebarQuantity& src)
{
	(*this) = src;
}

double RebarQuantity::GetWeight()
{
	if (_CustomWeight > -1)
		return _CustomWeight;

	MSDeformedBar bar(_DiaName);
	
	return _Length*bar.m_Weight / 1000.0;
}


void RebarQuantityInfo::Dump(EFS::EFS_Buffer& ar)
{
	int size = m_aPlannedQuantity.size();;
	ar << size;

	for (auto it = m_aPlannedQuantity.begin(); it != m_aPlannedQuantity.end(); it++)
		it->Dump(ar);

	size = m_aActualQuantity.size();;
	ar << size;

	for (auto it = m_aActualQuantity.begin(); it != m_aActualQuantity.end(); it++)
		it->Dump(ar);

	size = m_aAutoMatchingSets.size();
	ar << size;

	for (auto it = m_aAutoMatchingSets.begin(); it != m_aAutoMatchingSets.end(); it++)
		it->Dump(ar);
}

void RebarQuantityInfo::Recovery(EFS::EFS_Buffer& ar)
{
	m_aPlannedQuantity.clear();
	int size = 0;
	ar >> size;
	m_aPlannedQuantity.resize(size);
	for (int i = 0; i < size; i++)
	{
		RebarQuantity rebarQuant;
		rebarQuant.Recovery(ar);
		m_aPlannedQuantity[i] = rebarQuant;
	}

	m_aActualQuantity.clear();
	size = 0;
	ar >> size;
	m_aActualQuantity.resize(size);
	for (int i = 0; i < size; i++)
	{
		RebarQuantity rebarQuant;
		rebarQuant.Recovery(ar);
		m_aActualQuantity[i] = rebarQuant;
	}

	m_aAutoMatchingSets.clear();
	size = 0;
	ar >> size;
	m_aAutoMatchingSets.resize(size);
	for (int i = 0; i < size; i++)
	{
		RebarQuantityAutoMatchingSet rebarQuantAutoMatchingSet;
		rebarQuantAutoMatchingSet.Recovery(ar);
		m_aAutoMatchingSets[i] = rebarQuantAutoMatchingSet;
	}
}

void RebarQuantityInfo::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << (int)m_aPlannedQuantity.size();

		for (auto it = m_aPlannedQuantity.begin(); it != m_aPlannedQuantity.end(); it++)
			it->Serialize(ar);

		ar << (int)m_aActualQuantity.size();

		for (auto it = m_aActualQuantity.begin(); it != m_aActualQuantity.end(); it++)
			it->Serialize(ar);

		ar << (int)m_aAutoMatchingSets.size();

		for (auto it = m_aAutoMatchingSets.begin(); it != m_aAutoMatchingSets.end(); it++)
			it->Serialize(ar);
	}
	else
	{
		m_aPlannedQuantity.clear();
		int size = 0;
		ar >> size;
		m_aPlannedQuantity.resize(size);
		for (int i = 0; i < size; i++)
		{
			RebarQuantity rebarQuant;
			rebarQuant.Serialize(ar);
			m_aPlannedQuantity[i] = rebarQuant;
		}

		m_aActualQuantity.clear();
		size = 0;
		ar >> size;
		m_aActualQuantity.resize(size);
		for (int i = 0; i < size; i++)
		{
			RebarQuantity rebarQuant;
			rebarQuant.Serialize(ar);
			m_aActualQuantity[i] = rebarQuant;
		}

		if (MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20200728)
		{
			m_aAutoMatchingSets.clear();
			size = 0;
			ar >> size;
			m_aAutoMatchingSets.resize(size);
			for (int i = 0; i < size; i++)
			{
				RebarQuantityAutoMatchingSet rebarQuantAutoMatchingSet;
				rebarQuantAutoMatchingSet.Serialize(ar);
				m_aAutoMatchingSets[i] = rebarQuantAutoMatchingSet;
			}
		}
	}
}

void RebarQuantityInfo::ClearActualQuantity()
{
	m_aActualQuantity.clear();
}

void RebarQuantityInfo::ClearPlannedQuantity()
{
	m_aPlannedQuantity.clear();
}

void RebarQuantityInfo::ClearAllQuantity()
{
	ClearActualQuantity();
	ClearPlannedQuantity();
}

void RebarQuantityInfo::ClearAutoMatchingSets()
{
	m_aAutoMatchingSets.clear();
}

RebarQuantityManager RebarQuantityManager::Instance;


RebarQuantityManager::RebarQuantityManager(void)
{
	InitRebarShapeArr();
}

void RebarQuantityManager::AddRebarShape(int code, int numLparam, int numRparam)
{
	mapCodeToNumLParam.insert(make_pair(code, numLparam));
}
void RebarQuantityManager::InitRebarShapeArr()
{
	AddRebarShape(0000, 1);//Undefined
	//───────────────0───────────────
	AddRebarShape(1000, 1);
	AddRebarShape(1001, 2);
	AddRebarShape(1002, 2, 1);//e
	AddRebarShape(1003, 2);
	AddRebarShape(1004, 3);
	AddRebarShape(1005, 3, 2);//e
	AddRebarShape(1006, 3);
	AddRebarShape(1007, 3, 1);//e
	AddRebarShape(1008, 3);
	AddRebarShape(1009, 3, 1);//e
	AddRebarShape(1010, 3);
	AddRebarShape(1011, 3, 2);//e
	AddRebarShape(1012, 3);
	AddRebarShape(1013, 3, 1);//e
	AddRebarShape(1014, 3);
	AddRebarShape(1015, 3, 1);//e

	//──────────────1───────────────
	AddRebarShape(2000, 2);
	AddRebarShape(2001, 3);
	AddRebarShape(2002, 3, 1);//e
	AddRebarShape(2003, 3);
	AddRebarShape(2004, 3, 1);//e
	AddRebarShape(2005, 4);
	AddRebarShape(2006, 4, 2);//e
	AddRebarShape(2007, 4);
	AddRebarShape(2008, 4, 1);//e
	AddRebarShape(2009, 4);
	AddRebarShape(2010, 4, 1);//e
	AddRebarShape(2011, 4);
	AddRebarShape(2012, 2, 1);
	AddRebarShape(2013, 2, 1);
	AddRebarShape(2014, 3, 1);
	AddRebarShape(2015, 3, 1);

	//──────────────2──
	AddRebarShape(3000, 2);
	AddRebarShape(3001, 5);
	AddRebarShape(3002, 5, 2);//e
	AddRebarShape(3003, 5);
	AddRebarShape(3004, 5, 1);//e
	AddRebarShape(3005, 5);
	AddRebarShape(3006, 5, 1);//e
	AddRebarShape(3007, 5);
	AddRebarShape(3008, 5, 2);
	AddRebarShape(3009, 5);
	AddRebarShape(3010, 5);
	AddRebarShape(3011, 5, 2);//e
	AddRebarShape(3012, 5);
	AddRebarShape(3013, 5, 2);
	AddRebarShape(3014, 5, 3);//e
	AddRebarShape(3015, 5, 2);
	AddRebarShape(3016, 5, 3);	//e
	AddRebarShape(3017, 5, 2);
	AddRebarShape(3019, 3, 2);
	AddRebarShape(3022, 3);
	//New 140428
	AddRebarShape(3023, 5, 2);

	//──────────────3──
	AddRebarShape(4000, 6);
	AddRebarShape(4001, 6, 2);	//e
	AddRebarShape(4002, 6);
	AddRebarShape(4003, 6, 1);//e
	AddRebarShape(4004, 5);
	AddRebarShape(4005, 2, 1);
	AddRebarShape(4006, 1, 1);
	AddRebarShape(4007, 6, 1);
	AddRebarShape(4012, 7, 3);
	AddRebarShape(4013, 6, 3);	//e
	AddRebarShape(4014, 6, 2);
	AddRebarShape(4015, 7, 2);
	AddRebarShape(4016, 6, 2);
	AddRebarShape(4017, 7, 2);
	AddRebarShape(4018, 7, 2);
	//New 140428
	AddRebarShape(4019, 6, 2);
	AddRebarShape(4020, 6, 3);
	//New 141023
	AddRebarShape(4021, 3);

	//──────────────4──
	AddRebarShape(5000, 3);
	AddRebarShape(5003, 3, 2);
	AddRebarShape(5004, 3, 1);
	AddRebarShape(5005, 3, 1);
	AddRebarShape(5006, 3, 1);
	AddRebarShape(5007, 3, 2);
	AddRebarShape(5009, 4, 1);
	AddRebarShape(5012, 5, 1);
	AddRebarShape(5013, 5, 1);
	AddRebarShape(5014, 3, 2);
	AddRebarShape(5015, 5, 4);
	AddRebarShape(5019, 7, 2);
	AddRebarShape(5020, 5, 2);
	AddRebarShape(5021, 4, 1);
	AddRebarShape(5022, 4, 1);
	AddRebarShape(5028, 4);
	AddRebarShape(5029, 5, 4);
	AddRebarShape(5030, 5, 2);
	AddRebarShape(5037, 5);
	//AddRebarShape5003, 5);
	AddRebarShape(5039, 4);
	AddRebarShape(5040, 5);
	AddRebarShape(5041, 5);
	//New 140428
	AddRebarShape(5042, 5);
	//New 140722
	AddRebarShape(5043, 6);
	AddRebarShape(5044, 7);
	AddRebarShape(5045, 5, 1);

	//──────────────5──
	AddRebarShape(9000, 1, 1);
	AddRebarShape(9001, 2, 1);
	AddRebarShape(9002, 3, 1);
	AddRebarShape(9003, 3, 2);//e
	AddRebarShape(9004, 3, 2);//e
	AddRebarShape(9005, 3, 2);
	AddRebarShape(9006, 3, 1);
	AddRebarShape(6006, 5);
	AddRebarShape(2016, 4);
	AddRebarShape(2017, 4, 1);//e
	AddRebarShape(6023, 4);
	AddRebarShape(6024, 4, 1);//e
	//New 140428
	AddRebarShape(9007, 5, 1);
	AddRebarShape(9008, 2, 1);
	AddRebarShape(9009, 2, 1);
	//New 140722
	AddRebarShape(9010, 3, 1);
	AddRebarShape(9011, 3, 1);
	AddRebarShape(9012, 3, 1);
	AddRebarShape(9013, 4, 1);
	AddRebarShape(9014, 3);
	//New 141022
	AddRebarShape(9015, 2, 1);
	AddRebarShape(9016, 3, 1);
	AddRebarShape(9017, 3, 1);
	AddRebarShape(9018, 3, 1);
	AddRebarShape(9019, 4, 1);

	//──────────────6──
	AddRebarShape(7000, 1);
	AddRebarShape(7001, 1);
	AddRebarShape(7002, 1);
	AddRebarShape(7003, 1);
	AddRebarShape(7004, 1);
	AddRebarShape(7005, 2);
	AddRebarShape(7006, 2);
	AddRebarShape(7007, 3, 1);
	AddRebarShape(7008, 3, 1);
	AddRebarShape(7009, 3, 1);
	AddRebarShape(7010, 3, 1);
	AddRebarShape(7011, 3, 1);
	//New 140428
	AddRebarShape(7012, 2, 1);
	AddRebarShape(7013, 2, 1);
	//New 140722
	AddRebarShape(7014, 3, 1);
	AddRebarShape(7015, 3, 1);
	AddRebarShape(7016, 3, 1);
	AddRebarShape(7017, 3, 1);
	//New 141022
	AddRebarShape(7018, 3, 1);
	AddRebarShape(7019, 3, 1);
	AddRebarShape(7020, 3, 1);
	AddRebarShape(7021, 3, 1);

	//──────────────7──
	AddRebarShape(8000, 2, 7);
	AddRebarShape(8001, 2, 5);

}

int RebarQuantityManager::GetNumLParam(int GCode, int SCode)
{
	int code = GCode * 1000 + SCode;
	return mapCodeToNumLParam[code];
}

RebarQuantityAutoMatchingSet::RebarQuantityAutoMatchingSet(void)
{
	_PlanFieldType = ePlanFieldType::PlanBuilding;
	_ActualFieldType = eActualFieldType::ActualBuilding;
	_ConditionFieldType = eConditionFieldType::Equal;
}

RebarQuantityAutoMatchingSet::~RebarQuantityAutoMatchingSet(void)
{
}

RebarQuantityAutoMatchingSet& RebarQuantityAutoMatchingSet::operator=(const RebarQuantityAutoMatchingSet& src)
{
	_PlanFieldType = src._PlanFieldType;
	_ActualFieldType = src._ActualFieldType;
	_ConditionFieldType = src._ConditionFieldType;

	return (*this);
}

ePlanFieldType RebarQuantityAutoMatchingSet::GetPlanFieldType()
{
	return _PlanFieldType;
}
void RebarQuantityAutoMatchingSet::SetPlanFieldType(ePlanFieldType planFieldType)
{
	_PlanFieldType = planFieldType;
}
eActualFieldType RebarQuantityAutoMatchingSet::GetActualFieldType()
{
	return _ActualFieldType;
}
void RebarQuantityAutoMatchingSet::SetActualFieldType(eActualFieldType actualFieldType)
{
	_ActualFieldType = actualFieldType;
}
eConditionFieldType RebarQuantityAutoMatchingSet::GetConditionFieldType()
{
	return _ConditionFieldType;
}
void RebarQuantityAutoMatchingSet::SetConditionFieldType(eConditionFieldType conditionFieldType)
{
	_ConditionFieldType = conditionFieldType;
}

void RebarQuantityAutoMatchingSet::Dump(EFS::EFS_Buffer& ar)
{
	int nPlanFieldType = (int)_PlanFieldType;
	ar << nPlanFieldType;

	int nActualFieldType = (int)_ActualFieldType;
	ar << nActualFieldType;

	int nConditionFieldType = (int)_ConditionFieldType;
	ar << nConditionFieldType;
}

void RebarQuantityAutoMatchingSet::Recovery(EFS::EFS_Buffer& ar)
{
	int nPlanFieldType = 0;
	ar >> nPlanFieldType;
	_PlanFieldType = (ePlanFieldType)nPlanFieldType;

	int nActualFieldType = 0;
	ar >> nActualFieldType;
	_ActualFieldType = (eActualFieldType)nActualFieldType;

	int nConditionFieldType = 0;
	ar >> nConditionFieldType;
	_ConditionFieldType = (eConditionFieldType)nConditionFieldType;
}

void RebarQuantityAutoMatchingSet::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		int nPlanFieldType = (int)_PlanFieldType;
		ar << nPlanFieldType;

		int nActualFieldType = (int)_ActualFieldType;
		ar << nActualFieldType;

		int nConditionFieldType = (int)_ConditionFieldType;
		ar << nConditionFieldType;
	}
	else
	{
		int nPlanFieldType = 0;
		ar >> nPlanFieldType;
		_PlanFieldType = (ePlanFieldType)nPlanFieldType;

		int nActualFieldType = 0;
		ar >> nActualFieldType;
		_ActualFieldType = (eActualFieldType)nActualFieldType;

		int nConditionFieldType = 0;
		ar >> nConditionFieldType;
		_ConditionFieldType = (eConditionFieldType)nConditionFieldType;
	}
}