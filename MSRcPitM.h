#pragma once
#include "MDMember.h"
#include "MSStirrupBar.h"

class StructureModel_CLASS MSRcPitM :
	public MDMember
{
public:
	MSRcPitM(void);
	~MSRcPitM(void);

	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSRcPitM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	MSStirrupBar m_TopXBar;
	MSStirrupBar m_TopYBar;
	MSStirrupBar m_BotXBar;
	MSStirrupBar m_BotYBar;

	MSStirrupBar m_InHorBar;
	MSStirrupBar m_InVerBar;
	MSStirrupBar m_OutHorBar;
	MSStirrupBar m_OutVerBar;

	BOOL m_bSpacerBars;
	MSStirrupBar m_SpacerBars;	//우마철근
	double m_dPitDepth;

	BOOL m_BotSpacerType;   //1 : 하부우마철근 0 : 스페이서

	virtual BOOL GetBotSpacerType(){return m_BotSpacerType;}
	virtual SpacerShape * GetTopSpacerShape(){return m_pTopSpacerShape;}
	virtual SpacerShape * GetBotSpacerShape(){return m_pBotSpacerShape;}
	virtual double GetDepth(){return m_dPitDepth;};
	//virtual void AutoCalcTopSpacerShape(double dPileCover=0);//by hana
	//virtual void AutoCalcBotSpacerShape(double dPileCover=150);//by hana

	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetStirrupBarDiaAllLayer();
private:
// m_pTopSpacerShape과 m_pBotSpacerShape 사용 안함 -> G로 이동, Serialize때문에 남겨둠
	SpacerShape * m_pTopSpacerShape;	//상부 우마근
	SpacerShape * m_pBotSpacerShape;	//하부 우마근



};
