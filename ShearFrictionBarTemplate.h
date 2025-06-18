#pragma once
#include "MSStirrupBar.h"

class CrackPreventionShearBarTemplate;

class StructureModel_CLASS ShearFrictionBarRebarInfoByThick
{
public:
	ShearFrictionBarRebarInfoByThick();
	ShearFrictionBarRebarInfoByThick(ShearFrictionBarRebarInfoByThick *pData);
	~ShearFrictionBarRebarInfoByThick();
	void Serialize(CArchive& ar);

	long GetTotalThick();
	void SetTotalThick(long value);
	long GetNumBar();
	void SetNumBar(long value);
	BOOL GetIsNumSpacingType();
	void SetIsNumSpacingType(BOOL value);

	MSStirrupBar m_HBars;
	MSStirrupBar m_VBars;
private:
	BOOL m_isNumSpacingType;
	long m_nTotalThick;
	long m_nNumBar;
};
class StructureModel_CLASS ShearFrictionBarPlacementNumberByThick
{
public:
	ShearFrictionBarPlacementNumberByThick();
	ShearFrictionBarPlacementNumberByThick(ShearFrictionBarPlacementNumberByThick *pData);
	~ShearFrictionBarPlacementNumberByThick();
	void Serialize(CArchive& ar);

	long GetTotalThick();
	long GetPlacementNumber();
	long GetFirstPlacementThick();
	void SetTotalThick(long value);
	void SetPlacementNumber(long value);
	void SetFirstPlacementThick(long value);
private:
	long m_nTotalThick;
	long m_nPlacementNumber;
	long m_nFirstPlacementThick;
};

class StructureModel_CLASS ShearFrictionBarTemplate
{
public:
	ShearFrictionBarTemplate();
	~ShearFrictionBarTemplate();

	void DataInit();
	void CopyFromMe(ShearFrictionBarTemplate* pRebarTemplate);
	void Serialize(CArchive& ar);

	BOOL GetPlaceBar();
	long GetPlaceConditionThick();
	long GetTopDevType();
	long GetTopDevUserLength();
	long GetBotDevType();
	long GetBotDevUserLength();
	vector<ShearFrictionBarRebarInfoByThick*> GetShearFrictionBarRebarInfoByThickArr();
	vector<ShearFrictionBarPlacementNumberByThick*> GetShearFrictionBarPlacementNumberByThickArr();

	void SetPlaceBar(BOOL value);
	void SetPlaceConditionThick(long value);
	void SetTopDevType(long value);
	void SetTopDevUserLength(long value);
	void SetBotDevType(long value);
	void SetBotDevUserLength(long value);
	void SetShearFrictionBarRebarInfoByThickArr(vector<ShearFrictionBarRebarInfoByThick*> arr);
	void SetShearFrictionBarPlacementNumberByThickArr(vector<ShearFrictionBarPlacementNumberByThick*> arr);

	ShearFrictionBarRebarInfoByThick *FindShearFrictionBarRebarInfoByThick(long nThick);
	ShearFrictionBarPlacementNumberByThick *FindShearFrictionBarPlacementNumberByThick(long nThick);

	CrackPreventionShearBarTemplate *GetCrackPreventionShearBarTemplate();
private:
	BOOL m_bPlaceBar;
	long m_nPlaceConditionThick;
	long m_nTopDevType;
	long m_nTopDevUserLength;
	long m_nBotDevType;
	long m_nBotDevUserLength;
	vector<ShearFrictionBarRebarInfoByThick*> m_ShearFrictionBarRebarInfoByThickArr;
	vector<ShearFrictionBarPlacementNumberByThick*> m_ShearFrictionBarPlacementNumberByThickArr;

	CrackPreventionShearBarTemplate *m_pCrackPreventionShearBarTemplate;
	

	void DeleteShearFrictionBarRebarInfoByThickArr();
	void DeleteShearFrictionBarPlacementNumberByThickArr();


};

