#pragma once
#include "MSStirrupBar.h"

class StructureModel_CLASS CrackPreventionShearBarRebarInfo
{
public:
	CrackPreventionShearBarRebarInfo();
	CrackPreventionShearBarRebarInfo(CrackPreventionShearBarRebarInfo *pInfo);
	~CrackPreventionShearBarRebarInfo();
	void Serialize(CArchive& ar);

	long GetStartPlacementNumber();
	void SetStartPlacementNumber(long value);
	long GetEndPlacementNumber();
	void SetEndPlacementNumber(long value);

	MSStirrupBar m_HBars;
	MSStirrupBar m_VBars;
private:
	long m_nStartPlacementNumber;
	long m_nEndPlacementNumber;
};
class StructureModel_CLASS CrackPreventionShearBarTemplate
{
public:
	CrackPreventionShearBarTemplate();
	CrackPreventionShearBarTemplate(CrackPreventionShearBarTemplate *pTemplate);
	~CrackPreventionShearBarTemplate();
	void Serialize(CArchive& ar);

	void DataInit();
	void CopyFromMe(CrackPreventionShearBarTemplate* pRebarTemplate);

	vector<CrackPreventionShearBarRebarInfo*> GetCrackPreventionShearBarRebarInfoArr();
	void SetCrackPreventionShearBarRebarInfoArr(vector<CrackPreventionShearBarRebarInfo*> arr);

	BOOL GetPlaceBar();	
	long GetDiaType();
	long GetLxDiaIndex();
	long GetLyDiaIndex();
	BOOL GetLxUserSteelGradeS();
	BOOL GetLyUserSteelGradeS();
	long GetSpacingType();
	long GetLxSpacing();
	long GetLySpacing();
	BOOL GetPlaceSpacer();
	long GetSpacerHookLenAType();
	long GetSpacerHookLenA();
	long GetSpacerHookLenAUser();
	long GetSpacerHookLenBType();
	long GetSpacerHookLenB();
	long GetSpacerHookLenBUser();
	long GetSpacerWidth();

	void SetPlaceBar(BOOL value);
	void SetDiaType(long value);
	void SetLxDiaIndex(long value);
	void SetLyDiaIndex(long value);
	void SetLxUserSteelGradeS(BOOL value);
	void SetLyUserSteelGradeS(BOOL value);
	void SetSpacingType(long value);
	void SetLxSpacing(long value);
	void SetLySpacing(long value);
	void SetPlaceSpacer(BOOL value);
	void SetSpacerHookLenAType(long value);
	void SetSpacerHookLenA(long value);
	void SetSpacerHookLenAUser(long value);
	void SetSpacerHookLenBType(long value);
	void SetSpacerHookLenB(long value);
	void SetSpacerHookLenBUser(long value);
	void SetSpacerWidth(long value);
private:
	BOOL m_bPlaceBar;
	long m_nDiaType;
	long m_nLxDiaIndex;
	long m_nLyDiaIndex;
	BOOL m_bLxUserSteelGradeS;
	BOOL m_bLyUserSteelGradeS;
	long m_nSpacingType;
	long m_nLxSpacing;
	long m_nLySpacing;

	BOOL m_bPlaceSpacer;
	long m_nSpacerHookLenAType;
	long m_nSpacerHookLenA;
	long m_nSpacerHookLenAUser;
	long m_nSpacerHookLenBType;
	long m_nSpacerHookLenB;
	long m_nSpacerHookLenBUser;
	long m_nSpacerWidth;

	vector<CrackPreventionShearBarRebarInfo*> m_CrackPreventionShearBarRebarInfoArr;

	void DeleteCrackPreventionShearBarRebarInfoArr();
};