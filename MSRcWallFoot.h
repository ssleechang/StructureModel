#pragma once
#include "MDMember.h"

#include "MSStirrupBar.h"

const int HAUNCH_TYPE_RECTANGLE = 1;
const int HAUNCH_TYPE_TWO_WAY = 2;
const int HAUNCH_TYPE_ONE_WAY = 3;

class StructureModel_CLASS MSRcWallFootM :	public MDMember
{
public:
	MSRcWallFootM(void);
	~MSRcWallFootM(void);
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSRcWallFootM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	BOOL m_bIsPileFooting;	 // Pile 여부
	long m_Haunch; // 1 : 직사각형 2: 양헌치형 3: 일반향 헌치형
	BOOL m_bBotHookType;					//하부근 Hook 처리 0 직선 1 후크
	BOOL m_bIsLxTopBar;							//상부근 체크
	BOOL m_bIsLyTopBar;
	BOOL m_BotSpacerType;   // 0 : 스페이서, 1 : 하부우마철근
	BOOL m_bAddSpacer;			//우마철근 여부

	MSStirrupBar m_ShortBotBars;	//MSStirrupBar m_HBars;	//X
	MSStirrupBar m_LongBotBars;		//MSStirrupBar m_VBars;	//Y
	MSStirrupBar m_ShortTopBars;	//MSStirrupBar m_HTopBars;
	MSStirrupBar m_LongTopBars;		//MSStirrupBar m_VTopBars;
	MSStirrupBar m_SpacerBars;	//우마철근

	virtual BOOL GetBotSpacerType(){return m_BotSpacerType;}
	virtual SpacerShape * GetTopSpacerShape(){return m_pTopSpacerShape;}
	virtual SpacerShape * GetBotSpacerShape(){return m_pBotSpacerShape;}
	virtual long GetHaunchType(){return m_Haunch;}//by LSS 
	virtual void AutoCalcTopSpacerShape( double dMemberThick,double dTopCover,double dBotCover);
	virtual void AutoCalcBotSpacerShape(double dPileCover=150);//by hana

	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetStirrupBarDiaAllLayer();

	double GetBotMainBarMaxDeformedBarDiameter();
private:
// m_pTopSpacerShape과 m_pBotSpacerShape 사용 안함 -> G로 이동, Serialize때문에 남겨둠
	SpacerShape * m_pTopSpacerShape;	//상부 우마근
	SpacerShape * m_pBotSpacerShape;	//하부 우마근

//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	// start of addition by ironbell
//public:
//	virtual void ImportProfile(int pIfcId); // called by MSCompoG.cpp not by this->InnerImportIFC()
//	virtual int ExportProfile(double pWidth); // called by MSCompoG.cpp not by this->InnerExportIFC()
//	// end of addition by ironbell
//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};
