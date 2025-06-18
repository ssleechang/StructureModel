#pragma once
#include "mdmember.h"
#include "MSStirrupBar.h"
#include "MSMainBar.h"

class StructureModel_CLASS MDRcHandRailM :
	public MDMember
{
public:
	MDRcHandRailM(void);
	~MDRcHandRailM(void);
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MDRcHandRailM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	virtual void SetLRCover(double dLRCover){m_dLRCvrConc=dLRCover;};	// by gracias

	//벽 두께 m_dWidth
	double m_dLRCvrConc;	// 좌우 피복 (m_dCvrConc 상하피복으로 사용)
	BOOL m_bIsSinglePlaceBar;				//배근 타입 // 디폴트 = 1 // 0 = 복배근 1 = 단배근(벽배근)

	//수직근 - 배근방식
	BOOL m_bIsNumPlaceVBar;		//배근 방식 // 디폴트 = 1 // 0 = 간격 1 = 갯수	
	//갯수일경우
	MSMainBar m_MainVBar;
	//간격일경우
	MSStirrupBar m_VBars;	
	//수직근배근정보
	long m_nVerBarType;//0;벽타입 1:보(스터럽)타입
	//벽타입일경우
	BOOL m_bPlaceTopUBar;//상부유바마무리 
	long m_nBotDevType;//하부정착타입 0:일자정착 1: 표준후크정착

	//보타입
	long m_nStrpShapeType;//스트럽철근형상 0:유바형, 1:스트럽형

	//수평근 - 배근방식
	BOOL m_bIsNumPlaceHBar;		//배근 방식 // 디폴트 = 1 // 0 = 간격 1 = 갯수	
	//갯수일경우
	MSMainBar m_MainHBar;					
	//간격일경우
	MSStirrupBar m_HBars;	

	// 보강근 추가
	BOOL m_bTopAddBar;
	MSMainBar m_TopAddBar;
	BOOL m_bBotAddBar;
	MSMainBar m_BotAddBar;

	// 2013.09.24 - by Gracias
	BOOL m_bMidAddBar;
	MSMainBar m_MidAddBar;

	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetStirrupBarDiaAllLayer();

	double getVBarMaxDeformedBarDiameter();
	double getHBarMaxDeformedBarDiameter();
};