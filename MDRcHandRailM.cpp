#include "StdAfx.h"
#include "MDRcHandRailM.h"

IMPLEMENT_SERIAL(MDRcHandRailM, MDMember, VERSIONABLE_SCHEMA | 1)

MDRcHandRailM::MDRcHandRailM(void)
{
	CString sBar = _T("");

	//벽 두께 m_dWidth
	m_dCvrConc = 50;
	m_dLRCvrConc = 50;

	m_bIsSinglePlaceBar = 0;				//배근 타입 // 디폴트 = 1 // 0 = 복배근 1 = 단배근(벽배근)

	//수직근 배근방식
	m_bIsNumPlaceVBar = 0;		//배근 방식 // 디폴트 = 1 // 0 = 간격 1 = 갯수	
	sBar = MSDeformedBar::GetMainBarStr(4);
	m_MainVBar.InitData(sBar);
	sBar = MSDeformedBar::GetStirrupBarStr(300);
	m_VBars.InitData(sBar);
	m_nVerBarType = 1;//0;벽타입 1:보(스터럽)타입
	m_bPlaceTopUBar = FALSE;//상부유바마무리 
	m_nBotDevType = 1;//하부정착타입 0:일자정착 1: 표준후크정착
	m_nStrpShapeType = 0;//스트럽철근형상 0:유바형, 1:스트럽형

	m_bIsNumPlaceHBar = 0;
	sBar = MSDeformedBar::GetMainBarStr(4);
	m_MainHBar.InitData(sBar);
	sBar = MSDeformedBar::GetStirrupBarStr(300);
	m_HBars.InitData(sBar);

	sBar = MSDeformedBar::GetMainBarStr(4);
	m_bTopAddBar = FALSE;
	m_TopAddBar.InitData(sBar);
	m_bBotAddBar = FALSE;
	m_BotAddBar.InitData(sBar);

	m_bMidAddBar = FALSE;
	m_MidAddBar.InitData(sBar);
}


MDRcHandRailM::~MDRcHandRailM(void)
{
}

void MDRcHandRailM::Dump(EFS::EFS_Buffer& buffer)
{
	MDMember::Dump(buffer);
	buffer << m_dLRCvrConc << m_bIsSinglePlaceBar << m_bIsNumPlaceVBar;
	buffer << m_nVerBarType << m_bPlaceTopUBar << m_nBotDevType << m_nStrpShapeType << m_bIsNumPlaceHBar;
	buffer << m_bTopAddBar << &m_TopAddBar;
	buffer << m_bBotAddBar << &m_BotAddBar;
	buffer << m_bMidAddBar << &m_MidAddBar;

	m_MainVBar.Dump(buffer);
	m_VBars.Dump(buffer);
	m_MainHBar.Dump(buffer);
	m_HBars.Dump(buffer);
}

void MDRcHandRailM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDMember::Recovery(buffer);
	buffer >> m_dLRCvrConc >> m_bIsSinglePlaceBar >> m_bIsNumPlaceVBar;
	buffer >> m_nVerBarType >> m_bPlaceTopUBar >> m_nBotDevType >> m_nStrpShapeType >> m_bIsNumPlaceHBar;
	buffer >> m_bTopAddBar >> &m_TopAddBar;
	buffer >> m_bBotAddBar >> &m_BotAddBar;
	buffer >> m_bMidAddBar >> &m_MidAddBar;

	m_MainVBar.Recovery(buffer);
	m_VBars.Recovery(buffer);
	m_MainHBar.Recovery(buffer);
	m_HBars.Recovery(buffer);
}

void MDRcHandRailM::Serialize( CArchive& ar )
{
	MDMember::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_dLRCvrConc;
		ar << m_bIsSinglePlaceBar;
		ar << m_bIsNumPlaceVBar;
		m_MainVBar.Serialize(ar);
		m_VBars.Serialize(ar);
		
		ar << m_nVerBarType;
		ar << m_bPlaceTopUBar;
		ar << m_nBotDevType;

		ar << m_nStrpShapeType;
		ar << m_bIsNumPlaceHBar;
		
		m_MainHBar.Serialize(ar);					
		m_HBars.Serialize(ar);	

		ar << m_bTopAddBar;
		m_TopAddBar.Serialize(ar);
		ar << m_bBotAddBar;
		m_BotAddBar.Serialize(ar);

		ar << m_bMidAddBar;
		m_MidAddBar.Serialize(ar);
	}
	else
	{
		ar >> m_dLRCvrConc;
		ar >> m_bIsSinglePlaceBar;
		ar >> m_bIsNumPlaceVBar;
		m_MainVBar.Serialize(ar);
		m_VBars.Serialize(ar);

		ar >> m_nVerBarType;
		ar >> m_bPlaceTopUBar;
		ar >> m_nBotDevType;

		ar >> m_nStrpShapeType;
		ar >> m_bIsNumPlaceHBar;

		m_MainHBar.Serialize(ar);					
		m_HBars.Serialize(ar);	

		ar >> m_bTopAddBar;
		m_TopAddBar.Serialize(ar);
		ar >> m_bBotAddBar;
		m_BotAddBar.Serialize(ar);

		ar >> m_bMidAddBar;
		m_MidAddBar.Serialize(ar);
	}
}

void MDRcHandRailM::CopyFromMe( MDMember* pSource )
{
	MDRcHandRailM* pSourceHandRailM = (MDRcHandRailM*)pSource;
	m_dLRCvrConc = pSourceHandRailM->m_dLRCvrConc;

	m_bIsSinglePlaceBar = pSourceHandRailM->m_bIsSinglePlaceBar;				//배근 타입 // 디폴트 = 1 // 0 = 복배근 1 = 단배근(벽배근)

	//수직근 배근방식
	m_bIsNumPlaceVBar = pSourceHandRailM->m_bIsNumPlaceVBar;		//배근 방식 // 디폴트 = 1 // 0 = 간격 1 = 갯수	
	m_MainVBar.CopyFromMe(&pSourceHandRailM->m_MainVBar);
	m_VBars.CopyFromMe(&pSourceHandRailM->m_VBars);
	m_nVerBarType = pSourceHandRailM->m_nVerBarType;
	m_bPlaceTopUBar = pSourceHandRailM->m_bPlaceTopUBar;
	m_nBotDevType = pSourceHandRailM->m_nBotDevType;
	m_nStrpShapeType = pSourceHandRailM->m_nStrpShapeType;

	m_bIsNumPlaceHBar = pSourceHandRailM->m_bIsNumPlaceHBar;
	m_MainHBar.CopyFromMe(&pSourceHandRailM->m_MainHBar);
	m_HBars.CopyFromMe(&pSourceHandRailM->m_HBars);

	m_bTopAddBar = pSourceHandRailM->m_bTopAddBar;
	m_TopAddBar.CopyFromMe(&pSourceHandRailM->m_TopAddBar);
	m_bBotAddBar = pSourceHandRailM->m_bBotAddBar;
	m_BotAddBar.CopyFromMe(&pSourceHandRailM->m_BotAddBar);

	m_bMidAddBar = pSourceHandRailM->m_bMidAddBar;
	m_MidAddBar.CopyFromMe(&pSourceHandRailM->m_MidAddBar);

	MDMember::CopyFromMe(pSource);
}

vector<double> MDRcHandRailM::GetMainBarDiaAllLayer()
{
	vector<double> DiaArr;

	//수직근 - 배근방식
	if(m_bIsNumPlaceVBar == 0) //배근 방식 // 디폴트 = 1 // 0 = 간격 1 = 갯수	
	{
		vector<double> VBarDiaArr = m_VBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VBarDiaArr.begin(), VBarDiaArr.end());
	}
	else if(m_bIsNumPlaceVBar == 1)
	{
		vector<double> MainVBarDiaArr = m_MainVBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), MainVBarDiaArr.begin(), MainVBarDiaArr.end());
	}
	else
		ASSERT(0);

	//수평근 - 배근방식
	if(m_bIsNumPlaceHBar == 0) //배근 방식 // 디폴트 = 1 // 0 = 간격 1 = 갯수	
	{
		vector<double> HBarDiaArr = m_HBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), HBarDiaArr.begin(), HBarDiaArr.end());
	}
	else if(m_bIsNumPlaceHBar == 1)
	{
		vector<double> MainHBarDiaArr = m_MainHBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), MainHBarDiaArr.begin(), MainHBarDiaArr.end());
	}
	else
		ASSERT(0);

	// 보강근 추가
	if(m_bTopAddBar)
	{
		vector<double> TopBarDiaArr = m_TopAddBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), TopBarDiaArr.begin(), TopBarDiaArr.end());
	}
	
	if(m_bBotAddBar)
	{
		vector<double> BotBarDiaArr = m_BotAddBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), BotBarDiaArr.begin(), BotBarDiaArr.end());
	}
	
	if(m_bMidAddBar)
	{
		vector<double> MidAddBarDiaArr = m_MidAddBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), MidAddBarDiaArr.begin(), MidAddBarDiaArr.end());
	}

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const long& a, const long& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MDRcHandRailM::GetStirrupBarDiaAllLayer()
{
	vector<double> DiaArr;

	return DiaArr;
}

double MDRcHandRailM::getVBarMaxDeformedBarDiameter()
{
	vector<double> DiaArr;

	//수직근 - 배근방식
	if (m_bIsNumPlaceVBar == 0) //배근 방식 // 디폴트 = 1 // 0 = 간격 1 = 갯수	
	{
		vector<double> VBarDiaArr = m_VBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VBarDiaArr.begin(), VBarDiaArr.end());
	}
	else if (m_bIsNumPlaceVBar == 1)
	{
		vector<double> MainVBarDiaArr = m_MainVBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), MainVBarDiaArr.begin(), MainVBarDiaArr.end());
	}
	else
		ASSERT(0);

	sort(DiaArr.begin(), DiaArr.end(),
		[](const double& a, const double& b)->bool {return a < b; }
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	long nDiaSize = DiaArr.size();
	if (nDiaSize == 0) return 0;

	return DiaArr[nDiaSize - 1];
}

double MDRcHandRailM::getHBarMaxDeformedBarDiameter()
{
	vector<double> DiaArr;

	if (m_bIsNumPlaceHBar == 0)						//배근 방식 // 디폴트 = 1 // 0 = 간격 1 = 갯수	
	{
		vector<double> HBarDiaArr = m_HBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), HBarDiaArr.begin(), HBarDiaArr.end());
	}
	else if (m_bIsNumPlaceHBar == 1)
	{
		vector<double> MainHBarDiaArr = m_MainHBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), MainHBarDiaArr.begin(), MainHBarDiaArr.end());
	}
	else
		ASSERT(0);

	sort(DiaArr.begin(), DiaArr.end(),
		[](const long& a, const long& b)->bool {return a < b; }
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	long nDiaSize = DiaArr.size();
	if (nDiaSize == 0) return 0;

	return DiaArr[nDiaSize - 1];
}