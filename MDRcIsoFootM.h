#pragma once
#include "mdmember.h"

#include "MSStirrupBar.h"

class StructureModel_CLASS MDRcIsoFootM :
	public MDMember
{
public:
	MDRcIsoFootM(void);
	~MDRcIsoFootM(void);
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MDRcIsoFootM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	long m_PileLayout;

	BOOL m_bIsPileFooting;	 //long m_bIsPileFooting;  // 0 : 직접기초, 1 : 말뚝기초
	BOOL m_bIsLxTopBar;				//상부근 체크
	BOOL m_bIsLyTopBar;

	MSStirrupBar m_ShortBotBars;	//MSStirrupBar m_HBars;		//x
	MSStirrupBar m_LongBotBars;		//MSStirrupBar m_VBars;		//y
	MSStirrupBar m_ShortTopBars;	//MSStirrupBar m_HTopBars;
	MSStirrupBar m_LongTopBars;	//MSStirrupBar m_VTopBars;
	MSStirrupBar m_SpacerBars;	//우마철근
	BOOL m_bShortBotHookType;  //long m_bHasHBarHook;  // 휨보강근 단부 후크 여부; //0 직선 // 1 후크
	BOOL m_bLongBotHookType; //long m_bHasVBarHook;  // 휨보강근 단부 후크 여부;		//0 직선 // 1 후크
	BOOL m_bSpacerBars;

	long m_nNumPiles;  // 말뚝 갯수
	long m_nPileIdx;
	double m_dPileSpacing;    // 말뚝간 간격  <- 기초 설계 옵션에서 가져오기
	double m_dPileCompCapa;  // 말뚝 압축내력 <- 기초 설계 옵션에서 가져오기
	double m_dPileTenCapa;  // 말뚝 인장내력 <- 기초 설계 옵션에서 가져오기

	BOOL m_BotSpacerType;  // 0 : 스페이서, 1 : 하부우마철근

	virtual void InitValue();

	virtual BOOL GetBotSpacerType(){return m_BotSpacerType;}
	virtual SpacerShape * GetTopSpacerShape(){return m_pTopSpacerShape;}
	virtual SpacerShape * GetBotSpacerShape(){return m_pBotSpacerShape;}
	virtual void AutoCalcTopSpacerShape(double dMemberThick,double dTopCover, double dBotCover);
	virtual void AutoCalcBotSpacerShape(double dPileCover=150);

	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetStirrupBarDiaAllLayer();

private:
// m_pTopSpacerShape과 m_pBotSpacerShape 사용 안함 -> G로 이동, Serialize때문에 남겨둠
	SpacerShape * m_pTopSpacerShape;	//상부 우마근
	SpacerShape * m_pBotSpacerShape;	//하부 우마근
};

