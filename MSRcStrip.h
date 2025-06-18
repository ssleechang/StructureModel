#pragma once
#include "MSRcObject.h"

#include "MSStirrupBar.h"
#include "MSMainBar.h"
#include "StructureModelEnum.h"

class StructureModel_CLASS MSRcStrip :
	public MSRcObject
{
public:
	MSRcStrip(void);
	~MSRcStrip(void);
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSRcStrip)
	virtual void CopyFromMe(MSRcStrip* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	long m_Direction;   // 0 : 단변, 1 : 장변.
	long m_IsMiddle;    // 0 : No,   1 : Yes.
	double m_FromRatio;
	double m_ToRatio;
	// T-m_TBars
	// (T =  (B : Bent = 0, C : Cutoff = 1, S : Straight = 2,
	// B+C : Bent+Cutoff = 3, B+S : Bent+Straight = 4, C+S : Cutoff+Straight = 5
	// Co : Connect = 6, Co+C : Connect+Cutoff = 7)
	long m_CenBarType;		//long m_TBarShape; 
	long m_CenBarTypeBot;		//하부
	
	BOOL m_IsEndBar;

	MSStirrupBar m_Bar1;  //General : 상부중앙부직선, Deck : Short(상부연결근) Long(상부중앙배력근), PC : 중앙부 상부 직선근
	MSStirrupBar m_Bar2;  //General : 상부중앙부Cut,  Deck : Short(상부보강근) Long(상부단부배력근), PC : 단부 상부 직선근
	MSStirrupBar m_Bar3;  //General : 상부중앙부Bent, Deck : Short(하부연결근) Long(하부중앙배력근), PC : 중앙부 상부 보강근
	MSStirrupBar m_Bar4;  //General : 하부중앙부직선, Deck : Short(하부보강근) Long(하부단부배력근), PC : 단부 상부 보강근
	MSStirrupBar m_Bar5;  //General : 상부단부직선,   Deck : Short(None)       Long(상부보강근),     PC : 상부 연결근
	MSStirrupBar m_Bar6;  //General : 하부단부직선,   Deck : Short(None)       Long(하부보강근),     PC : None
	MSStirrupBar m_Bar7;  //General : 하부중앙부Cut,  Deck : None,                                   PC : None
	MSStirrupBar m_Bar8;  //General : 상부단부Bent,   Deck : None,                                   PC : None
	MSStirrupBar m_Bar9;  //General : 상부단부Cut,    Deck : None,                                   PC : None
	MSStirrupBar m_Bar10; //General : 하부단부Cut,    Deck : None,                                   PC : None
		
	long m_nUseColumnStrip; // 0: 주열대,중간대 배근 동일  1: 주열대,중간대 배근 다름

	bool m_IsNumPlaceBar4;
	MSMainBar m_MainBar3;
	MSMainBar m_MainBar4;

	//* ADD Gracias 131121
	enum STRIP_TYPE {TYPE_NONE = -1, TYPE_A = 0, TYPE_B, TYPE_C, TYPE_D, TYPE_E, TYPE_F, TYPE_G, TYPE_H, TYPE_I, TYPE_ALL};

	STRIP_TYPE m_StripType;

	virtual vector<long> GetBarDiaAllLayer();

	bool GetRebarInfobyFlatSlabType(msBarSetType eBarSetType, vector<double>& DiaArr, double& dSpacing);

};
