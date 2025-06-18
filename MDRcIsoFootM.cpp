#include "StdAfx.h"
#include "MDRcIsoFootM.h"

IMPLEMENT_SERIAL(MDRcIsoFootM, MDMember, VERSIONABLE_SCHEMA | 1)

MDRcIsoFootM::MDRcIsoFootM(void)
{
	m_dCvrConc = 80;

	m_PileLayout = 0;

	m_bIsPileFooting = TRUE;
	m_bIsLxTopBar = FALSE;			
	m_bIsLyTopBar = FALSE;

	CString sBar = _T("");

	sBar = MSDeformedBar::GetStirrupBarStr(300);
	m_ShortBotBars.InitData(sBar);
	m_LongBotBars.InitData(sBar);
	m_ShortTopBars.InitData(sBar);
	m_LongTopBars.InitData(sBar);
	sBar = MSDeformedBar::GetStirrupBarStr(1000);
	m_SpacerBars.InitData(sBar);
	m_bShortBotHookType = TRUE;
	m_bLongBotHookType = TRUE;
	m_bSpacerBars = TRUE;

	m_nNumPiles = 0;
	m_nPileIdx = 1;
	m_dPileSpacing = 0;
	m_dPileCompCapa = 0;
	m_dPileTenCapa = 0;

	m_BotSpacerType = 0;

	m_pTopSpacerShape = new SpacerShape();
	m_pBotSpacerShape = new SpacerShape();
}


MDRcIsoFootM::~MDRcIsoFootM(void)
{
	delete m_pTopSpacerShape;
	delete m_pBotSpacerShape;
}

void MDRcIsoFootM::Dump(EFS::EFS_Buffer& buffer)
{
	MDMember::Dump(buffer);

	buffer << m_PileLayout << m_bIsPileFooting << m_bIsLxTopBar << m_bIsLyTopBar;
	buffer << m_bShortBotHookType << m_bLongBotHookType << m_bSpacerBars << m_nNumPiles;
	buffer << m_nPileIdx << m_dPileSpacing << m_dPileCompCapa << m_dPileTenCapa << m_BotSpacerType;

	m_ShortBotBars.Dump(buffer);
	m_LongBotBars.Dump(buffer);
	m_ShortTopBars.Dump(buffer);
	m_LongTopBars.Dump(buffer);
	m_SpacerBars.Dump(buffer);
}

void MDRcIsoFootM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDMember::Recovery(buffer);
	buffer >> m_PileLayout >> m_bIsPileFooting >> m_bIsLxTopBar >> m_bIsLyTopBar;
	buffer >> m_bShortBotHookType >> m_bLongBotHookType >> m_bSpacerBars >> m_nNumPiles;
	buffer >> m_nPileIdx >> m_dPileSpacing >> m_dPileCompCapa >> m_dPileTenCapa >> m_BotSpacerType;

	m_ShortBotBars.Recovery(buffer);
	m_LongBotBars.Recovery(buffer);
	m_ShortTopBars.Recovery(buffer);
	m_LongTopBars.Recovery(buffer);
	m_SpacerBars.Recovery(buffer);
}

void MDRcIsoFootM::Serialize( CArchive& ar )
{
	MDMember::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_PileLayout;
		ar << m_bIsPileFooting;
		ar << m_bIsLxTopBar;
		ar << m_bIsLyTopBar;

		m_ShortBotBars.Serialize(ar);
		m_LongBotBars.Serialize(ar);
		m_ShortTopBars.Serialize(ar);
		m_LongTopBars.Serialize(ar);
		m_SpacerBars.Serialize(ar);
		ar << m_bShortBotHookType;
		ar << m_bLongBotHookType;
		ar << m_bSpacerBars;

		ar << m_nNumPiles;
		ar << m_nPileIdx;
		ar << m_dPileSpacing;
		ar << m_dPileCompCapa;
		ar << m_dPileTenCapa;

		ar << m_BotSpacerType;

		m_pTopSpacerShape->Serialize(ar);
		m_pBotSpacerShape->Serialize(ar);
	}
	else
	{
		ar >> m_PileLayout;
		ar >> m_bIsPileFooting;
		ar >> m_bIsLxTopBar;
		ar >> m_bIsLyTopBar;

		m_ShortBotBars.Serialize(ar);
		m_LongBotBars.Serialize(ar);
		m_ShortTopBars.Serialize(ar);
		m_LongTopBars.Serialize(ar);
		m_SpacerBars.Serialize(ar);
		ar >> m_bShortBotHookType;
		ar >> m_bLongBotHookType;
		ar >> m_bSpacerBars;

		ar >> m_nNumPiles;
		ar >> m_nPileIdx;
		ar >> m_dPileSpacing;
		ar >> m_dPileCompCapa;
		ar >> m_dPileTenCapa;

		ar >> m_BotSpacerType;

		m_pTopSpacerShape->Serialize(ar);
		m_pBotSpacerShape->Serialize(ar);
	}
}

void MDRcIsoFootM::CopyFromMe( MDMember* pSource )
{
	MDRcIsoFootM* pSourceIsoFootM = (MDRcIsoFootM*)pSource;
	m_PileLayout = pSourceIsoFootM->m_PileLayout;

	m_bShortBotHookType = pSourceIsoFootM->m_bShortBotHookType;
	m_bLongBotHookType = pSourceIsoFootM->m_bLongBotHookType;

	m_bIsPileFooting = pSourceIsoFootM->m_bIsPileFooting;
	m_nNumPiles = pSourceIsoFootM->m_nNumPiles;
	m_nPileIdx = pSourceIsoFootM->m_nPileIdx;
	m_dPileSpacing = pSourceIsoFootM->m_dPileSpacing;
	m_dPileCompCapa = pSourceIsoFootM->m_dPileCompCapa;
	m_dPileTenCapa = pSourceIsoFootM->m_dPileTenCapa;
	m_BotSpacerType = pSourceIsoFootM->m_BotSpacerType;
	m_bSpacerBars = m_bSpacerBars;

	m_ShortBotBars.CopyFromMe(&pSourceIsoFootM->m_ShortBotBars);
	m_LongBotBars.CopyFromMe(&pSourceIsoFootM->m_LongBotBars);

	m_ShortTopBars.CopyFromMe(&pSourceIsoFootM->m_ShortTopBars);
	m_LongTopBars.CopyFromMe(&pSourceIsoFootM->m_LongTopBars);;

	m_SpacerBars.CopyFromMe(&pSourceIsoFootM->m_SpacerBars);
	MDMember::CopyFromMe(pSource);
}

void MDRcIsoFootM::InitValue()
{

}


void MDRcIsoFootM::AutoCalcTopSpacerShape( double dMemberThick,double dTopCover, double dBotCover)
{
	if(m_pTopSpacerShape ==NULL)
		return;
	double dSpacing = max(m_ShortBotBars.GetSpacing(), m_LongBotBars.GetSpacing());
	double dBotDia = max(m_ShortBotBars.GetDeformedBarDiameter(),m_LongBotBars.GetDeformedBarDiameter());
	m_pTopSpacerShape->AutoCalc(dSpacing,dMemberThick,dTopCover,dBotCover, m_SpacerBars.GetDeformedBarDiameter(),dBotDia,dBotDia);

}

void MDRcIsoFootM::AutoCalcBotSpacerShape( double dPileCover/*=150*/ )
{
	if(m_pBotSpacerShape ==NULL)
		return;
	if(dPileCover<GetCover())
		return;
	double dSpacing = max(m_ShortBotBars.GetSpacing(), m_LongBotBars.GetSpacing());
	m_pBotSpacerShape->AutoCalc(dSpacing,dPileCover,0,0,m_SpacerBars.GetDeformedBarDiameter(),0,0);
}

vector<double> MDRcIsoFootM::GetMainBarDiaAllLayer()
{
	vector<double> DiaArr;

	vector<double> ShortBotBarDiaArr = m_ShortBotBars.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), ShortBotBarDiaArr.begin(), ShortBotBarDiaArr.end());

	vector<double> LongTopBarDiaArr = m_LongBotBars.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), LongTopBarDiaArr.begin(), LongTopBarDiaArr.end());

	if(m_bIsLxTopBar)				//상부근 체크
	{
		vector<double> ShortTopBarDiaArr = m_ShortTopBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), ShortTopBarDiaArr.begin(), ShortTopBarDiaArr.end());
	}

	if(m_bIsLyTopBar)
	{
		vector<double> LongTopBarDiaArr = m_LongTopBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), LongTopBarDiaArr.begin(), LongTopBarDiaArr.end());
	}
	
	sort(DiaArr.begin(), DiaArr.end(), 
		[](const long& a, const long& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MDRcIsoFootM::GetStirrupBarDiaAllLayer()
{
	vector<double> DiaArr;

	// 현재 사용되지 않고 있음 ==> 추가 작업이 필요함, 셋팅 하는 다이얼로그 없음
	if(m_bSpacerBars) // 
	{
		vector<double> SpacerBarDiaArr = m_SpacerBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), SpacerBarDiaArr.begin(), SpacerBarDiaArr.end());
	}

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const long& a, const long& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}
