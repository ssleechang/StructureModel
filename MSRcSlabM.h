#pragma once
#include "MDMember.h"

#include "MSRcStrip.h"
#include "MSMainBar.h"

class StructureModel_CLASS MSRcSlabM :
	public MDMember
{
public:
	MSRcSlabM(void);
	~MSRcSlabM(void);
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSRcSlabM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	enum SLAB_TYPE {MAT_TYPE=0, FLAT_TYPE, GENERAL_TYPE, PC_TYPE, DECK_TYPE, SOG, CIRCULAR_RAMP, DECK_GENERAL, PC_IMT, PC_RPS, PC_HCS, TRANSFER};

	long m_bIsMat;
	long m_nSlabMType;
	
	MSRcStrip m_ShortStrip;
	MSRcStrip m_LongStrip;

	BOOL m_IsSpacer;
	BOOL m_BotSpacerType;   // 0 : 스페이서, 1 : 하부우마철근 ==> 
	MSStirrupBar m_SpacerBars;	//우마철근
// m_pTopSpacerShape과 m_pBotSpacerShape 사용 안함 -> G로 이동, Serialize때문에 남겨둠
	SpacerShape * m_pTopSpacerShape;
	SpacerShape * m_pBotSpacerShape;

	// 20141209 Deck & PC 일때 각 항목 배근여부 판단 - by Gracias
	bool m_IsShort1, m_IsShort2, m_IsShort3, m_IsShort4, m_IsShort5, m_IsShort6;
	bool m_IsLong1, m_IsLong2, m_IsLong3, m_IsLong4, m_IsLong5, m_IsLong6;

	bool m_IsShort7, m_IsShort8, m_IsShort9, m_IsShort10;
	bool m_IsLong7, m_IsLong8, m_IsLong9, m_IsLong10;
		
	// 20150422 - by gracias
	bool m_IsUsedColumnStripLxQuarter;

	// Deck철근
	MSMainBar m_DeckBar2, m_DeckBar3, m_DeckBar4;
	bool m_IsNumPlaceDeckBar2;
	MSStirrupBar m_DeckStirrupBar2;

	void GetMainBarDiaAllLayer(MSStirrupBar* pStirrupBar, vector<double>& DiaArr);

	virtual long GetSlabMType();
	virtual SpacerShape * GetTopSpacerShape(){return m_pTopSpacerShape;}
	virtual SpacerShape * GetBotSpacerShape(){return m_pBotSpacerShape;}
	virtual BOOL GetBotSpacerType(){return m_BotSpacerType;}
	virtual BOOL IsPC();
	virtual BOOL IsHalfPCSlab();
	virtual BOOL IsIMTPCSlab();
	virtual BOOL IsHCSPCSlab();
	virtual BOOL IsRPSPCSlab();
	virtual bool IsTransfer() override;
	virtual void AutoCalcTopSpacerShape(double dMemberThick, double dTopCover, double dBotCover );
	virtual void AutoCalcBotSpacerShape(double dPileCover=150);

	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetShortBarDiaAllLayer();
	virtual vector<double> GetLongBarDiaAllLayer();
	virtual vector<double> GetStirrupBarDiaAllLayer();

	BOOL IsGeneralSlab();
	BOOL IsDeckSlab();
	BOOL IsPCSlab();

	bool GetIsUsedColumnStripLxQuarter()	{ return m_IsUsedColumnStripLxQuarter;};
	void SetIsUsedColumnStripLxQuarter(bool isUsedColumnStripLxQuarter)	{ m_IsUsedColumnStripLxQuarter = isUsedColumnStripLxQuarter;};

	double GetSpacerDia(bool isTop);
	double GetMaxDia_ShortTop();
	double GetMinSpacing_ShortTop();
	double GetMaxSpacing_ShortTop();
	double GetMaxDia_LongTop();
	double GetMinSpacing_LongTop();
	double GetMaxSpacing_LongTop();
	double GetMaxDia_ShortBot();
	double GetMinSpacing_ShortBot();
	double GetMaxSpacing_ShortBot();
	double GetMaxDia_LongBot();
	double GetMinSpacing_LongBot();
	double GetMaxSpacing_LongBot();

	void GetLxMaxDiaInfo(double &dDia, bool &bUserSteelGradeS);
	void GetLyMaxDiaInfo(double &dDia, bool &bUserSteelGradeS);
	//──────────────────────────────────
	void GetLxTopMaxDiaInfo(double &dDia, bool &bUserSteelGradeS);
	void GetLyTopMaxDiaInfo(double &dDia, bool &bUserSteelGradeS);

	void GetLxBotMaxDiaInfo(double &dDia, bool &bUserSteelGradeS);
	void GetLyBotMaxDiaInfo(double &dDia, bool &bUserSteelGradeS);

	void GetRcSlabTopMaxDiaInfo(double &dDia, bool &bUserSteelGradeS, bool isLx);
	void GetRcSlabBotMaxDiaInfo(double &dDia, bool &bUserSteelGradeS, bool isLx);
	//──────────────────────────────────
	MSStirrupBar* GetMaxDiaStirrupBar_ShortTop();
	MSStirrupBar* GetMaxDiaStirrupBar_LongTop();
	MSStirrupBar* GetMaxDiaStirrupBar_ShortBot();
	MSStirrupBar* GetMaxDiaStirrupBar_LongBot();

	void SetIsLongBarFlag(bool bFlag);
	void SetIsShortBarFlag(bool bFlag);

	void CheckShortnLongBarFlag();
	void GetMaxDia(MSStirrupBar bar, double& dCurMaxDia);

	double GetShortBarMaxDeformedBarDiameter();
	double GetLongBarMaxDeformedBarDiameter();

	void ShortnLongBarNotUsedFlagFalseSetting();
		

	// 20221011 이후 부터는 사용하지 않는 변수 - Start
	double GetPastDataL1();
	double GetPastDataL2();
	double GetPastDataL3();
	double GetPastDataH1();
	double GetPastDataH2();
	double GetPastDataH3();
private:
	double m_PastDataL1, m_PastDataL2, m_PastDataL3;
	double m_PastDataH1, m_PastDataH2, m_PastDataH3;
	// 20221011 이후 부터는 사용하지 않는 변수 - end


	virtual bool IsDeck() override;

};
