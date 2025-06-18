#include "stdafx.h"
#include "MSCustomLineBarUnit.h"
#include "MSCustomLocation.h"
#include "MSCustomLineBarLocation.h"
#include "MSCustomReBarLine.h"
#include "MSCustomRebar.h"

#include "GMLib/GM2DCurve.h"
#include "GMLib/GM2DLineVector.h"
#include "../GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSCustomLineBarUnit, MSCustomBarUnit, VERSIONABLE_SCHEMA | 1)

MSCustomLineBarUnit::MSCustomLineBarUnit(void)
{
	m_eLineBarType = eLineBarType::Hoop;
	m_eSectionSpacingType = eSectionSpacingType::SectionEqual;
	m_eExtrudeSpacingType = eExtrudeSpacingType::ExtrudeEqual;
	m_ShapeCode = eShapeCode::UNKNOWN;

	m_nStartCover = 0;
	m_nStartHookAngleType = 7;
	m_nStartHookUserAngle = 0;
	m_nStartHookLengthType = 0;
	m_nStartHookUserLength = 0;
	m_nEndCover = 0;
	m_nEndHookAngleType = 7;
	m_nEndHookUserAngle = 0;
	m_nEndHookLengthType = 0;
	m_nEndHookUserLength = 0;
}

MSCustomLineBarUnit::~MSCustomLineBarUnit(void)
{
	ClearLocations();
}

void MSCustomLineBarUnit::ClearLocations()
{
	ma_Location.clear();
}

void MSCustomLineBarUnit::CopyFromMe(MSCustomBarUnit* pSource)
{
	if (pSource == NULL)	return;

	MSCustomBarUnit::CopyFromMe(pSource);
	MSCustomLineBarUnit* pSourceUnit = (MSCustomLineBarUnit*)pSource;
	m_eLineBarType = pSourceUnit->GetLineBarType();
	m_eSectionSpacingType = pSourceUnit->GetSectionSpacingType();
	m_eExtrudeSpacingType = pSourceUnit->GetExtrudeSpacingType();
	m_ShapeCode = pSourceUnit->GetShapeCode();

	ClearLocations();
	vector<MSCustomLocation> urLocations = pSourceUnit->GetLocations();
	for (int i = 0; i < urLocations.size(); i++)
	{
		MSCustomLocation myLoc;
		myLoc.CopyFromMe(&urLocations[i]);
		ma_Location.push_back(myLoc);
	}

	m_DistLocation.CopyFromMe(&pSourceUnit->GetDistLocation());

	m_nStartCover = pSourceUnit->m_nStartCover;
	m_nStartHookAngleType = pSourceUnit->m_nStartHookAngleType;
	m_nStartHookUserAngle = pSourceUnit->m_nStartHookUserAngle;
	m_nStartHookLengthType = pSourceUnit->m_nStartHookLengthType;
	m_nStartHookUserLength = pSourceUnit->m_nStartHookUserLength;
	m_nEndCover = pSourceUnit->m_nEndCover;
	m_nEndHookAngleType = pSourceUnit->m_nEndHookAngleType;
	m_nEndHookUserAngle = pSourceUnit->m_nEndHookUserAngle;
	m_nEndHookLengthType = pSourceUnit->m_nEndHookLengthType;
	m_nEndHookUserLength = pSourceUnit->m_nEndHookUserLength;
}

void MSCustomLineBarUnit::Dump(EFS::EFS_Buffer& buffer)
{
	MSCustomBarUnit::Dump(buffer);

	int lineBarType = (int)GetLineBarType();
	int sectionSpacingType = (int)GetSectionSpacingType();
	int extrudeSpacingType = (int)GetExtrudeSpacingType();

	buffer << lineBarType << sectionSpacingType << extrudeSpacingType;
	buffer << m_ShapeCode;

	int count = ma_Location.size();
	buffer << count;
	for (int i = 0; i < ma_Location.size(); i++)
	{
		ma_Location[i].Dump(buffer);
	}

	m_DistLocation.Dump(buffer);

	buffer << m_nStartCover << m_nStartHookAngleType << m_nStartHookUserAngle << m_nStartHookLengthType << m_nStartHookUserLength;
	buffer << m_nEndCover << m_nEndHookAngleType << m_nEndHookUserAngle << m_nEndHookLengthType << m_nEndHookUserLength;
}

void MSCustomLineBarUnit::Recovery(EFS::EFS_Buffer& buffer)
{
	MSCustomBarUnit::Recovery(buffer);

	int lineBarType = 0, sectionSpacingType = 0, extrudeSpacingType = 0;

	buffer >> lineBarType >> sectionSpacingType >> extrudeSpacingType;
	buffer >> m_ShapeCode;

	m_eLineBarType = (eLineBarType)lineBarType;
	m_eSectionSpacingType = (eSectionSpacingType)sectionSpacingType;
	m_eExtrudeSpacingType = (eExtrudeSpacingType)extrudeSpacingType;

	int count = 0;
	buffer >> count;
	ClearLocations();
	for (int i = 0; i < count; i++)
	{
		MSCustomLocation myLoc;
		myLoc.Recovery(buffer);
		ma_Location.push_back(myLoc);
	}

	m_DistLocation.Recovery(buffer);

	buffer >> m_nStartCover >> m_nStartHookAngleType >> m_nStartHookUserAngle >> m_nStartHookLengthType >> m_nStartHookUserLength;
	buffer >> m_nEndCover >> m_nEndHookAngleType >> m_nEndHookUserAngle >> m_nEndHookLengthType >> m_nEndHookUserLength;
}

void MSCustomLineBarUnit::Serialize(CArchive &ar)
{
	MSCustomBarUnit::Serialize(ar);
	int lineBarType = 0, sectionSpacingType = 0, extrudeSpacingType = 0;
	if (ar.IsStoring())
	{
		int lineBarType = (int)GetLineBarType();
		int sectionSpacingType = (int)GetSectionSpacingType();
		int extrudeSpacingType = (int)GetExtrudeSpacingType();

		ar << lineBarType;
		ar << sectionSpacingType;
		ar << m_eExtrudeSpacingType;
		ar << (int)m_ShapeCode;

		int count = ma_Location.size();
		ar << count;
		for (int i = 0; i < ma_Location.size(); i++)
		{
			ma_Location[i].Serialize(ar);
		}

		m_DistLocation.Serialize(ar);

		ar << m_nStartCover;
		ar << m_nStartHookAngleType;
		ar << m_nStartHookUserAngle;
		ar << m_nStartHookLengthType;
		ar << m_nStartHookUserLength;
		ar << m_nEndCover;
		ar << m_nEndHookAngleType;
		ar << m_nEndHookUserAngle;
		ar << m_nEndHookLengthType;
		ar << m_nEndHookUserLength;
	}
	else
	{
		int lineBarType = 0, sectionSpacingType = 0, extrudeSpacingType = 0, shapeCode = 0;

		ar >> lineBarType;
		ar >> sectionSpacingType;
		ar >> extrudeSpacingType;
		ar >> shapeCode;

		m_eLineBarType = (eLineBarType)lineBarType;
		m_eSectionSpacingType = (eSectionSpacingType)sectionSpacingType;
		m_eExtrudeSpacingType = (eExtrudeSpacingType)extrudeSpacingType;
		m_ShapeCode = (eShapeCode)shapeCode;

		int count = 0;
		ar >> count;
		ClearLocations();
		for (int i = 0; i < count; i++)
		{
			MSCustomLocation myLoc;
			myLoc.Serialize(ar);
			ma_Location.push_back(myLoc);
		}

		m_DistLocation.Serialize(ar);

		if (MSVersionInfo::GetCurrentVersion() >= 20190723)
		{
			ar >> m_nStartCover;
			ar >> m_nStartHookAngleType;
			ar >> m_nStartHookUserAngle;
			ar >> m_nStartHookLengthType;
			ar >> m_nStartHookUserLength;
			ar >> m_nEndCover;
			ar >> m_nEndHookAngleType;
			ar >> m_nEndHookUserAngle;
			ar >> m_nEndHookLengthType;
			ar >> m_nEndHookUserLength;
		}

		m_ShapeCode = eShapeCode::UNKNOWN;
	}
}

void MSCustomLineBarUnit::AddLocation(long rebarLineID1, long rebarLineID2)
{
	MSCustomLocation loc;
	loc.SetRebarLineID1(rebarLineID1);
	loc.SetRebarLineID2(rebarLineID2);
	ma_Location.push_back(loc);
}
void MSCustomLineBarUnit::SetDistLocation(long rebarLineID1, long rebarLineID2)
{
	m_DistLocation.SetRebarLineID1(rebarLineID1);
	m_DistLocation.SetRebarLineID2(rebarLineID2);
}

void MSCustomLineBarUnit::SetLocation(int index, int rebarLineID1, int rebarLineID2)
{
	if (index >= ma_Location.size()) return;

	ma_Location[index].SetRebarLineID1(rebarLineID1);
	ma_Location[index].SetRebarLineID2(rebarLineID2);
}

bool MSCustomLineBarUnit::HasRebarLineID(long rebarLineID)
{
	if (m_DistLocation.HasRebarLineID(rebarLineID))	return true;
	for (int i = 0; i < ma_Location.size(); i++)
	{
		if (ma_Location[i].HasRebarLineID(rebarLineID))
			return true;
	}
	return false;
}

GM2DPolyline* MSCustomLineBarUnit::GetProfile()
{
	vector<MSCustomLocation> locations = GetLocations();
	if (locations.size() == 0)	return nullptr;

	MSCustomLineSection* pCustomLineSection = dynamic_cast<MSCustomLineSection*>(GetCustomSection());
	if (pCustomLineSection == nullptr)	return nullptr;

	GM2DPolyline* pProfile = new GM2DPolyline();
	for (int i = 0; i < locations.size(); i++)
	{
		GM2DVector* pCrossVec = GetCustomSection()->GetCrossPointByRebarLineIDs(locations[i].GetRebarLineID1(), locations[i].GetRebarLineID2());
		if (pCrossVec != nullptr)
		{
			pProfile->Add(pCrossVec);
			if (i + 1 != locations.size())	pProfile->ma_Bulge.Add(0.0);
		}
	}


	return pProfile;
}

bool MSCustomLineBarUnit::GetTieLineVector(GM2DLineVector& lineVec)
{
	vector<MSCustomLocation> locations = GetLocations();
	if (locations.size() == 0)	return false;

	MSCustomLineSection* pCustomLineSection = dynamic_cast<MSCustomLineSection*>(GetCustomSection());
	if (pCustomLineSection == nullptr)	return false;

	if (locations.size() != 2)	return false;
	GM2DVector* pCrossVec1 = GetCustomSection()->GetCrossPointByRebarLineIDs(locations[0].GetRebarLineID1(), locations[0].GetRebarLineID2());
	GM2DVector* pCrossVec2 = GetCustomSection()->GetCrossPointByRebarLineIDs(locations[1].GetRebarLineID1(), locations[1].GetRebarLineID2());
	if (pCrossVec1 != nullptr && pCrossVec2 != nullptr)
	{
		lineVec.m_P0 = *pCrossVec1;
		lineVec.m_P1 = *pCrossVec2;
	}

	delete pCrossVec1;
	delete pCrossVec2;
	return true;
}

vector<CString> MSCustomLineBarUnit::GetShapeCodeNameList(MSCustomSection::eSectionType eSecType, eLineBarType barType)
{
	vector<CString> shapeNameList;

	if (eSecType == MSCustomSection::CUSTOM_COLUMN || eSecType == MSCustomSection::CUSTOM_WALLCOLUMN)
	{
		if (barType == eLineBarType::Tie)
			shapeNameList.push_back(_T("L01_00_10"));
		else if (barType == eLineBarType::Hoop)
		{
			shapeNameList.push_back(_T("L01_00_00"));
			shapeNameList.push_back(_T("L01_00_10"));
			shapeNameList.push_back(_T("L03_00_00"));
			shapeNameList.push_back(_T("L03_00_10"));
			shapeNameList.push_back(_T("L04_00_00"));
			shapeNameList.push_back(_T("L04_00_10"));
			shapeNameList.push_back(_T("L04_01_00"));
			shapeNameList.push_back(_T("L04_01_10"));
			shapeNameList.push_back(_T("L04_01_11"));
			shapeNameList.push_back(_T("L04_02_00"));
			shapeNameList.push_back(_T("L05_01_10"));
			shapeNameList.push_back(_T("L05_02_10"));
			shapeNameList.push_back(_T("L06_01_10"));
		}
	}
	else if (eSecType == MSCustomSection::CUSTOM_BEAM)
	{
		if (barType == eLineBarType::Stirrup)
		{
			shapeNameList.push_back(_T("L04_00_10"));
			shapeNameList.push_back(_T("L03_00_10"));
		}
		else if (barType == eLineBarType::Shear||
			barType == eLineBarType::CapTie)
		{
			shapeNameList.push_back(_T("L01_00_10"));
		}
	}

	return shapeNameList;
}

vector<CString> MSCustomLineBarUnit::GetBarTypeNameList(MSCustomSection::eSectionType eType)
{
	vector<CString> typeNameList;
	if (eType == MSCustomSection::CUSTOM_COLUMN || eType == MSCustomSection::CUSTOM_WALLCOLUMN)
	{
		typeNameList.push_back(LocalDic::GetLocal(L"대근", L"MSDBManager"));
		typeNameList.push_back(LocalDic::GetLocal(L"보조 대근", L"Chang.RebarHUBCommon"));
	}
	else if (eType == MSCustomSection::CUSTOM_BEAM)
	{
		typeNameList.push_back(LocalDic::GetLocal(L"스터럽", L"Chang.RebarHUBCommon"));
		typeNameList.push_back(LocalDic::GetLocal(L"전단 보강근", L"MSDBManager"));
		typeNameList.push_back(LocalDic::GetLocal(L"덮개철근", L"FeatureRebarPlacer"));
	}
	return typeNameList;
}

vector<CString> MSCustomLineBarUnit::GetSectionSpacingNameList(MSCustomLineBarUnit::eLineBarType barType)
{
	vector<CString> spacingNameList;
	spacingNameList.push_back(LocalDic::GetLocal(L"등분포", L"ExtraMethods"));
	if(barType == eLineBarType::Tie || barType == eLineBarType::Shear)
		spacingNameList.push_back(LocalDic::GetLocal(L"등분포 - 유효간격", L"ExtraMethods"));
	return spacingNameList;
}

vector<CString> MSCustomLineBarUnit::GetExtrudeSpacingNameList()
{
	vector<CString> spacingNameList;
	spacingNameList.push_back(LocalDic::GetLocal(L"등분포", L"ExtraMethods"));
	return spacingNameList;
}

int MSCustomLineBarUnit::GetInputPointCount(eShapeCode shapeCode, eLineBarType lineBarType)
{
	if (lineBarType == CapTie)
	{
		switch (shapeCode)
		{
		case eShapeCode::L01_00_10:	return 2;
		case eShapeCode::L04_00_10:	return 4;
		case eShapeCode::L03_00_10:	return 4;
		}
	}
	else if (lineBarType == Hoop)
	{
		switch (shapeCode)
		{
		case eShapeCode::L01_00_00:
		case eShapeCode::L01_00_10:	return 2;
		case eShapeCode::L03_00_00:
		case eShapeCode::L03_00_10:
		case eShapeCode::L04_00_00:
		case eShapeCode::L04_00_10:
		case eShapeCode::L04_01_00:
		case eShapeCode::L04_01_10:
		case eShapeCode::L04_01_11:
		case eShapeCode::L04_02_00: return 4;
		case eShapeCode::L05_01_10:
		case eShapeCode::L05_02_10: return 5;
		case eShapeCode::L06_01_10: return 6;
		}
	}
	else
	{
		switch (shapeCode)
		{
		case eShapeCode::L01_00_10:	return 3;
		case eShapeCode::L04_00_10:	return 4;
		case eShapeCode::L03_00_10:	return 4;
		}
	}
	return -1;
}

CString MSCustomLineBarUnit::GetDefaultLineRebar(MSCustomLineBarUnit::eShapeCode shapeCode, MSCustomLineBarUnit::eLineBarType barType)
{
	CString sBar = _T("");
	CString sDefDiaName = MSDeformedBar::GetDefaultRebarName();
	if (!IsNumAndSpacingBarType(barType))
		sBar = MSDeformedBar::GetStirrupBarStr(300);		
	else
	{
		int numTieBar = 2;
		if (shapeCode == L01_00_10 && barType == CapTie)
			numTieBar = 1;
		sBar.Format(_T("%ld-%s%s@300"), numTieBar, MSDeformedBar::m_sDefaultBarType, sDefDiaName);
	}
	return sBar;
}

bool MSCustomLineBarUnit::IsNumAndSpacingBarType(eLineBarType eBarType)
{
	//구속조건에따라결정
	//Constraint Line o
	//Constraint DIST o
	//	-- > 개수 + 간격
	
	//Constraint Line o
	//Constraint DIST x
	//-- > 간격

	switch (eBarType)
	{
	case eLineBarType::Hoop:
	case eLineBarType::Stirrup:
		return false;
		break;
	case eLineBarType::Tie:
	case eLineBarType::Shear:
	case eLineBarType::CapTie:
		return false;
		break;
	default:
		break;
	}

	return false;
}

MSCustomReBarLine* MSCustomLineBarUnit::GetReferenceRebarLine_InTieBarP0()
{
	if (IsNumAndSpacingBarType(m_eLineBarType) == false)		return nullptr;
	if (ma_Location.size() != 2)	return nullptr;

	int myID1 = ma_Location[0].GetRebarLineID1();
	int myID2 = ma_Location[0].GetRebarLineID2();

	vector<int> urIDs;
	urIDs.push_back(ma_Location[1].GetRebarLineID1());
	urIDs.push_back(ma_Location[1].GetRebarLineID2());

	bool hasID1 = false, hasID2 = false;
	if (HasRebarLineInRebarLineIDs(urIDs, myID1))	hasID1 = true;
	if (HasRebarLineInRebarLineIDs(urIDs, myID2))	hasID2 = true;

	if (hasID1 && hasID2)	return nullptr;
	if (!hasID1 && !hasID2)	return nullptr;

	if (hasID1)	return GetCustomSection()->GetMSRebarLine(myID2);
	if (hasID2)	return GetCustomSection()->GetMSRebarLine(myID1);

	return nullptr;
}

MSCustomReBarLine* MSCustomLineBarUnit::GetReferenceRebarLine_InTieBarP1()
{
	if (IsNumAndSpacingBarType(m_eLineBarType) == false)		return nullptr;
	if (ma_Location.size() != 2)	return nullptr;

	int myID1 = ma_Location[1].GetRebarLineID1();
	int myID2 = ma_Location[1].GetRebarLineID2();

	vector<int> urIDs;
	urIDs.push_back(ma_Location[0].GetRebarLineID1());
	urIDs.push_back(ma_Location[0].GetRebarLineID2());

	bool hasID1 = false, hasID2 = false;
	if (HasRebarLineInRebarLineIDs(urIDs, myID1))	hasID1 = true;
	if (HasRebarLineInRebarLineIDs(urIDs, myID2))	hasID2 = true;

	if (hasID1 && hasID2)	return nullptr;
	if (!hasID1 && !hasID2)	return nullptr;

	if (hasID1)	return GetCustomSection()->GetMSRebarLine(myID2);
	if (hasID2)	return GetCustomSection()->GetMSRebarLine(myID1);

	return nullptr;
}

bool MSCustomLineBarUnit::HasRebarLineInRebarLineIDs(vector<int>& rebarLineIDs, int rebarLineID)
{
	for (int i = 0; i < rebarLineIDs.size(); i++)
	{
		if (rebarLineIDs[i] == rebarLineID)	return true;
	}
	return false;
}

bool MSCustomLineBarUnit::PolyLineClosedCheckByShapeCode()
{
	bool IsClosed = true;

	if (m_ShapeCode == eShapeCode::L01_00_00 || m_ShapeCode == eShapeCode::L01_00_10 || m_ShapeCode == eShapeCode::L03_00_00
		|| m_ShapeCode == eShapeCode::L03_00_10 || m_ShapeCode == eShapeCode::L04_00_00 || m_ShapeCode == eShapeCode::L04_01_00
		|| m_ShapeCode == eShapeCode::L04_02_00)
	{
		IsClosed = false;
	}

	return IsClosed;
}

vector<CString> MSCustomLineBarUnit::GetHookAngleTypeList()
{
	vector<CString> hookAngleTypeList;

	hookAngleTypeList.push_back(_T("None"));
	hookAngleTypeList.push_back(_T("90"));
	hookAngleTypeList.push_back(_T("135"));
	hookAngleTypeList.push_back(_T("180"));
	hookAngleTypeList.push_back(_T("-90"));
	hookAngleTypeList.push_back(_T("-135"));
	hookAngleTypeList.push_back(_T("-180"));
	hookAngleTypeList.push_back(_T("RebarTemplate"));
	hookAngleTypeList.push_back(_T("User"));

	return hookAngleTypeList;
}

vector<CString> MSCustomLineBarUnit::GetHookLengthTypeList()
{
	vector<CString> hookLengthTypeList;

	hookLengthTypeList.push_back(_T("표준 후크 길이"));
	hookLengthTypeList.push_back(_T("User"));

	return hookLengthTypeList;
}
long MSCustomLineBarUnit::GetHookDegree(bool isStart)
{
	long nHookAngleType = isStart ? m_nStartHookAngleType : m_nEndHookAngleType;
	switch(nHookAngleType)
	{
	case 1:			return 90;
	case 2:			return 135;
	case 3:			return 180;
	case 4:			return -90;
	case 5:			return -135;
	case 6:			return -180;
	case 8:			return isStart ? m_nStartHookUserAngle : m_nEndHookUserAngle;
	default:			return 0;
	}
}