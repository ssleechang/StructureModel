#pragma once
#include "MSCustomBarUnit.h"
#include "MSStirrupBar.h"
#include "MSCustomLineSection.h"
#include "MSCustomLocation.h"

class MSCustomLineBarLocation;
class MSRebarShape;

class StructureModel_CLASS MSCustomLineBarUnit :public MSCustomBarUnit
{
public:
	enum eShapeCode
	{
		UNKNOWN = 0,
		L01_00_00,
		L01_00_10,
		L03_00_00,
		L03_00_10,
		L04_00_00,
		L04_00_10,
		L04_01_00,
		L04_01_10,
		L04_01_11,
		L04_02_00,
		L05_01_10,
		L05_02_10,
		L06_01_10
	};

	enum eLineBarType
	{
		Hoop,
		Tie,
		Stirrup,
		Shear,
		CapTie
	};

	enum eSectionSpacingType
	{
		SectionEqual,
		EffectiveSpacing
	};

	enum eExtrudeSpacingType
	{
		ExtrudeEqual,
	};

	MSCustomLineBarUnit();
	~MSCustomLineBarUnit();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSCustomLineBarUnit)
	virtual void CopyFromMe(MSCustomBarUnit* pSource);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	void SetLineBarType(eLineBarType barType) {m_eLineBarType = barType;};
	void SetSectionSpacingType(eSectionSpacingType spacingType) {m_eSectionSpacingType = spacingType;};
	void SetExtrudeSpacingType(eExtrudeSpacingType spacingType) { m_eExtrudeSpacingType = spacingType; };
	void SetShapeCode(eShapeCode shapeCode) {m_ShapeCode = shapeCode;};

	void AddLocation(long rebarLineID1, long rebarLineID2);
	void SetDistLocation(long rebarLineID1, long rebarLineID2);

	eLineBarType GetLineBarType() { return m_eLineBarType; };
	eSectionSpacingType GetSectionSpacingType() { return m_eSectionSpacingType; };
	eExtrudeSpacingType GetExtrudeSpacingType() { return m_eExtrudeSpacingType; };
	eShapeCode GetShapeCode() { return m_ShapeCode; };

	GM2DPolyline* GetProfile();
	bool GetTieLineVector(GM2DLineVector& lineVec);

	vector<MSCustomLocation> GetLocations() {return ma_Location;};
	MSCustomLocation GetDistLocation() { return m_DistLocation; };

	void SetLocation(int index, int rebarLineID1, int rebarLineID2);

	bool HasRebarLineID(long rebarLineID);

	void ClearLocations();
	void InitalizeDistLocation() { m_DistLocation.Initialize(); };

	bool PolyLineClosedCheckByShapeCode();

	MSCustomReBarLine* GetReferenceRebarLine_InTieBarP0();
	MSCustomReBarLine* GetReferenceRebarLine_InTieBarP1();
protected: 
	bool HasRebarLineInRebarLineIDs(vector<int>& rebarLineIDs, int rebarLineID);

public:
//******************************** string static function
	static vector<CString> GetShapeCodeNameList(MSCustomSection::eSectionType eSecType, eLineBarType barType);
	static vector<CString> GetBarTypeNameList(MSCustomSection::eSectionType eType);
	static vector<CString> GetSectionSpacingNameList(MSCustomLineBarUnit::eLineBarType barType);
	static vector<CString> GetExtrudeSpacingNameList();

	static int GetInputPointCount(eShapeCode shapeCode, eLineBarType lineBarType);
	static CString GetDefaultLineRebar(MSCustomLineBarUnit::eShapeCode shapeCode, MSCustomLineBarUnit::eLineBarType barType);
	static bool IsNumAndSpacingBarType(eLineBarType eBarType);

	static vector<CString> GetHookAngleTypeList();
	static vector<CString> GetHookLengthTypeList();
//********************************

	long GetStartCover() { return m_nStartCover; };
	void SetStartCover(long nStartCover) { m_nStartCover = nStartCover; };
	long GetStartHookAngleType() { return m_nStartHookAngleType; };
	void SetStartHookAngleType(long nStartHookAngleType) { m_nStartHookAngleType = nStartHookAngleType; };
	long GetStartHookUserAngle() { return m_nStartHookUserAngle; };
	void SetStartHookUserAngle(long nStartHookUserAngle) { m_nStartHookUserAngle = nStartHookUserAngle; };
	long GetStartHookLengthType() { return m_nStartHookLengthType; };
	void SetStartHookLengthType(long nStartHookLengthType) { m_nStartHookLengthType = nStartHookLengthType; };
	long GetStartHookUserLength() { return m_nStartHookUserLength; };
	void SetStartHookUserLength(long nStartHookUserLength) { m_nStartHookUserLength = nStartHookUserLength; };
	long GetEndCover() { return m_nEndCover; };
	void SetEndCover(long nEndCover) { m_nEndCover = nEndCover; };
	long GetEndHookAngleType() { return m_nEndHookAngleType; };
	void SetEndHookAngleType(long nEndHookAngleType) { m_nEndHookAngleType = nEndHookAngleType; };
	long GetEndHookUserAngle() { return m_nEndHookUserAngle; };
	void SetEndHookUserAngle(long nEndHookUserAngle) { m_nEndHookUserAngle = nEndHookUserAngle; };
	long GetEndHookLengthType() { return m_nEndHookLengthType; };
	void SetEndHookLengthType(long nEndHookLengthType) { m_nEndHookLengthType = nEndHookLengthType; };
	long GetEndHookUserLength() { return m_nEndHookUserLength; };
	void SetEndHookUserLength(long nEndHookUserLength) { m_nEndHookUserLength = nEndHookUserLength; };

	long GetHookDegree(bool isStart);	

protected:

	eLineBarType m_eLineBarType;
	eSectionSpacingType m_eSectionSpacingType;
	eExtrudeSpacingType m_eExtrudeSpacingType;
	eShapeCode m_ShapeCode;

	vector<MSCustomLocation> ma_Location;
	MSCustomLocation m_DistLocation;

	long m_nStartCover;
	long m_nStartHookAngleType;
	long m_nStartHookUserAngle;
	long m_nStartHookLengthType;
	long m_nStartHookUserLength;
	long m_nEndCover;
	long m_nEndHookAngleType;
	long m_nEndHookUserAngle;
	long m_nEndHookLengthType;
	long m_nEndHookUserLength;
};

