#pragma once
#include "MDMember.h"

#include "MSStirrupBar.h"

class StructureModel_CLASS MSRcRWallM :
	public MDMember
{
public:
	MSRcRWallM(void);
	~MSRcRWallM(void);
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSRcRWallM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 


	//////////////////////////////////////////////////////////////////////////
	//0 : 일반배근		 - 주근
	//1 : 상부보강		 - 주근 + 상부
	//2 : 하부보강		 - 주근 + 하부
	//3 : 상하부 보강 - 주근 + 상부 + 하부
	//////////////////////////////////////////////////////////////////////////
	long m_nExtVBarType;
	BOOL m_bUse2VMidBar;		//2단배근
	long m_nOutdCvrConc;

	//수직근 - 외부
	MSStirrupBar m_VOutTopBar;					//상부 보강근					//외부 : Out
	MSStirrupBar m_VOutMidBar;					//주근									//외부
	MSStirrupBar m_VOutBotBar;					//하부 보강근					//외부
	MSStirrupBar m_VOutMidAddBar;				//중앙 보강근					//외부
	BOOL m_bVOutTopAddBar;						//상부 보강근 유무				//외부
	BOOL m_bVOutMidAddBar;						//중앙 보강근 유무				//외부
	BOOL m_bVOutBotAddBar;						//하부 보강근 유무				//외부
	long m_nVTopBarLength;
	long m_nVBotBarLength;
	long m_nVMidAddBarLength;
	//수직근 - 내부
	MSStirrupBar m_2VMidBar;						//2단배근 보강근 - 현재 사용안함
	MSStirrupBar m_VInMidBar;						//주근								//내부
	long m_nVInBarType;
	MSStirrupBar m_VInTopBar;						//상부 보강근					20121031 추가
	MSStirrupBar m_VInBotBar;						//하부 보강근					20121031 추가
	MSStirrupBar m_VInMidAddBar;					//중앙 보강근
	BOOL m_bVInTopAddBar;							//상부 보강근 유무
	BOOL m_bVInMidAddBar;							//중앙 보강근 유무
	BOOL m_bVInBotAddBar;							//하부 보강근 유무
	long m_nVInTopBarLength;						// 20121031추가
	long m_nVInBotBarLength;						// 20121031추가
	long m_nVInMidAddBarLength;

	//수평근
	BOOL m_HSameCheck;
	BOOL m_HInOutCheck;
	MSStirrupBar m_HInTopBar;						//상부 보강근					//내부 : In	
	MSStirrupBar m_HInMidBar;						//주근									//내부
	MSStirrupBar m_HInBotBar;						//하부 보강근					//내부
	MSStirrupBar m_HInStartAddBar;					//시작 보강근					//내부
	MSStirrupBar m_HInMidAddBar;					//중앙 보강근					//내부
	MSStirrupBar m_HInEndAddBar;					//끝단 보강근					//내부
	BOOL m_bHInStartAddBar;							//시작 보강근 유무				//내부
	BOOL m_bHInMidAddBar;							//시작 보강근 유무				//내부
	BOOL m_bHInEndAddBar;							//시작 보강근 유무				//내부
	long m_nHInStartAddBarLength;
	long m_nHInMidAddBarLength;
	long m_nHInEndAddBarLength;

	MSStirrupBar m_HOutTopBar;					//상부 보강근					//외부 : Out	
	MSStirrupBar m_HOutMidBar;					//주근									//외부	
	MSStirrupBar m_HOutBotBar;					//하부 보강근					//외부
	MSStirrupBar m_HOutStartAddBar;				//시작 보강근					//외부
	MSStirrupBar m_HOutMidAddBar;				//중앙 보강근					//외부
	MSStirrupBar m_HOutEndAddBar;				//끝단 보강근					//외부
	BOOL m_bHOutStartAddBar;						//시작 보강근 유무				//외부
	BOOL m_bHOutMidAddBar;						//시작 보강근 유무				//외부
	BOOL m_bHOutEndAddBar;						//시작 보강근 유무				//외부
	long m_nHOutStartAddBarLength;
	long m_nHOutMidAddBarLength;
	long m_nHOutEndAddBarLength;

	BOOL m_bUseAllStirrup;
	BOOL m_bUseTopStirrup;								//전단보강근 상부근 디폴트 : 0 
	BOOL m_bUseBotStirrup;								//전단보강근 하부근 디폴트 : 0
	double m_dTopSRegion;									//상부 전단보강구간 길이
	double m_dBotSRegion;									//하부 전단보강구간 길이
	MSStirrupBar m_HTopStirrup;						
	MSStirrupBar m_VTopStirrup;
	MSStirrupBar m_HBotStirrup;
	MSStirrupBar m_VBotStirrup;

	// Add Code - by Gracias(2013.09.10)
	BOOL m_bStartShearAddBar;					//시작근 유무
	BOOL m_bEndShearAddBar;					//끝근 유무
	MSStirrupBar m_StartShearHBars;
	MSStirrupBar m_StartShearVBars;
	MSStirrupBar m_EndShearHBars;
	MSStirrupBar m_EndShearVBars;
	long m_nStartShearAddBarLength;
	long m_nEndShearAddBarLength;
	// End(2013.09.10)

	BOOL m_bAllShearAddBar;
	MSStirrupBar m_AllShearHBars;
	MSStirrupBar m_AllShearVBars;

	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetVerBarDiaAllLayer();
	virtual vector<double> GetHorBarDiaAllLayer();
	virtual vector<double> GetStirrupBarDiaAllLayer();

	double GetMaxDia_HorInner();
	double GetMaxDia_HorOutter();	
	double GetMaxDia_VerInner();
	double GetMaxDia_VerOutter();	

	double GetMinDia_VerInnerMainBar();
	double GetMinDia_VerOutterMainBar();	

	double GetMaxDia();
	double GetMaxVerBarDia();
	double GetMinVerBarDia();

	double GetMaxDia_VerInnerMainBar();
	double GetMaxDia_VerOutterMainBar();

	double GetSpacing_VerInnerMainBar();
	double GetSpacing_VerOutterMainBar();

	bool GetVerBarUserSteelGradeS();

	double GetMaxHorBarSpacing();
	double GetMaxVerBarSpacing();
};
