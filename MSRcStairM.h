#pragma once
#include "MDMember.h"

#include "MSStirrupBar.h"
#include "MSMainBar.h"

class StructureModel_CLASS MSRcStairM :
	public MDMember
{
public:
	MSRcStairM(void);
	~MSRcStairM(void);
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSRcStairM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

    enum Type_Stair {msLanding=0, msStep};
    Type_Stair m_nStairType;

	BOOL m_bOneWay;			// TRUE : 일방향계단 FALSE: 이방향계단 디폴트 = 2
	BOOL m_bStairAllSame;
	double m_dRailWidth;//난간폭

	long m_nStairNum;					//계단 갯수
	double m_dLandingThick;			//계단 참 두께
	double m_dStairThick;				//계단 두께
	double m_dTopLandingLength;	//상부 계단 참길이
	double m_dBotLandingLength;	//하부 계단 참길이
	
	
	double m_dRightWallThick;		//오른쪽 벽 두께
	double m_dLeftWallThick;		//왼쪽 벽 두께
	
	long m_nWallCvrConc;			//벽 피복 두께

	MSStirrupBar m_SXTopEndBar;			//계단 : 단부 x 방향
	MSStirrupBar m_SXBotEndBar;			//계단 : 단부 x 방향
	MSStirrupBar m_SYTopEndBar;			//계단 : 단부 y 방향
	MSStirrupBar m_SYBotEndBar;			//계단 : 단부 y 방향
	MSStirrupBar m_SXTopCenBar;		//계단 : 중앙부 x 방향
	MSStirrupBar m_SXBotCenBar;		//계단 : 중앙부 x 방향
	MSStirrupBar m_SYTopCenBar;		//계단 : 중앙부 y 방향
	MSStirrupBar m_SYBotCenBar;		//계단 : 중앙부 y 방향


	MSStirrupBar m_LXTopBar;	//참 : 상부 x방향
	MSStirrupBar m_LYTopBar;	//참 : 상부 y방향
	MSStirrupBar m_LXBotBar;	//참 : 하부 x방향
	MSStirrupBar m_LYBotBar;	//참 : 하부 y방향	

	MSMainBar   m_TopAddBar;			//상부참 보강근
	MSMainBar   m_BotAddBar;			//하부참 보강근


// 	double m_dUThick;
// 	double m_dLThick;
// 	double m_dULength;
// 	double m_dLLength;

// 	MSStirrupBar m_UYTopBar;
// 	MSStirrupBar m_UYBotBar;
// 	MSStirrupBar m_UXTopBar;
// 	MSStirrupBar m_UXBotBar;


 	virtual double GetStairThick(){ return m_dStairThick;}//by LSS for Stair
 	virtual double GetLandingThick(){ return m_dLandingThick;}//by LSS for Stair
 	virtual void SetStairThick(double dThick){m_dStairThick = dThick;};	// by gracias for Stair
 	virtual void SetLandingThick(double dThick){m_dLandingThick = dThick;};	// by gracias for Stair;
	virtual double GetTopLandLength(){return m_dTopLandingLength;};	// by gracias for stair
	virtual double GetBotLandLength(){return m_dBotLandingLength;};	// by gracias for stair
};
