#include "StdAfx.h"
#include "MDRcButtressM.h"
#include "MDRcButtressSec.h"
#include "..\GMLib\MSVersionInfo.h"

IMPLEMENT_SERIAL(MDRcButtressM, MDMember, VERSIONABLE_SCHEMA | 1)
	MDRcButtressM::MDRcButtressM(void)
{
	mp_TopSec = NULL;
	mp_CenSec = NULL;
	mp_BotSec = NULL;

	CString sBar = _T("");
	sBar = MSDeformedBar::GetMainBarStr(0);
	m_SideMainBars.InitData(sBar);
	sBar = MSDeformedBar::GetStirrupBarStr(200);
	m_SideStirrupBars.InitData(sBar);
	m_IsNumPlaceSideBar = true;

	m_CoverShapeCode = MST1_09;
	m_StirrupShapeCode = MST3_00;

	CreateSections();

	m_bIsNumPlaceShearBar = TRUE;
	sBar = MSDeformedBar::GetMainBarStr(0);
	m_ShearBars.InitData(sBar);
	m_nShearBarRate = 1;
}

MDRcButtressM::~MDRcButtressM(void)
{
	if(mp_TopSec)		delete mp_TopSec;
	if(mp_CenSec)	delete mp_CenSec;
	if(mp_BotSec)		delete mp_BotSec;
}
void MDRcButtressM::CopyFromMe(MDMember* pSource)
{
	if(pSource==NULL)	return;
	MDRcButtressM * pSourceButtressM = (MDRcButtressM*)pSource;

	CreateSections();

	mp_TopSec->CopyFromMe(pSourceButtressM->mp_TopSec);
	mp_CenSec->CopyFromMe(pSourceButtressM->mp_CenSec);
	mp_BotSec->CopyFromMe(pSourceButtressM->mp_BotSec);

	m_IsNumPlaceSideBar = pSourceButtressM->m_IsNumPlaceSideBar;	
	m_SideMainBars.CopyFromMe(&pSourceButtressM->m_SideMainBars);
	m_SideStirrupBars.CopyFromMe(&pSourceButtressM->m_SideStirrupBars);

	m_CoverShapeCode = pSourceButtressM->m_CoverShapeCode;
	m_StirrupShapeCode = pSourceButtressM->m_StirrupShapeCode;

	m_bIsNumPlaceShearBar = pSourceButtressM->m_bIsNumPlaceShearBar;
	m_ShearBars.CopyFromMe(&pSourceButtressM->m_ShearBars);
	m_nShearBarRate = pSourceButtressM->m_nShearBarRate;

	MDMember::CopyFromMe(pSource);
}

void MDRcButtressM::Dump(EFS::EFS_Buffer& buffer)
{
	MDMember::Dump(buffer);

	mp_TopSec->Dump(buffer);
	mp_CenSec->Dump(buffer);
	mp_BotSec->Dump(buffer);
	buffer << m_IsNumPlaceSideBar;
	m_SideMainBars.Dump(buffer);
	m_SideStirrupBars.Dump(buffer);
	buffer << m_bIsNumPlaceShearBar << m_nShearBarRate;
	m_ShearBars.Dump(buffer);
}
void MDRcButtressM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDMember::Recovery(buffer);
	mp_TopSec->Recovery(buffer);
	mp_CenSec->Recovery(buffer);
	mp_BotSec->Recovery(buffer);
	buffer >> m_IsNumPlaceSideBar;
	m_SideMainBars.Recovery(buffer);
	m_SideStirrupBars.Recovery(buffer);
	buffer >> m_bIsNumPlaceShearBar >> m_nShearBarRate;
	m_ShearBars.Recovery(buffer);
}

void MDRcButtressM::Serialize(CArchive &ar)
{
	MDMember::Serialize(ar);
	if(ar.IsStoring())
	{
		mp_TopSec->Serialize(ar);
		mp_CenSec->Serialize(ar);
		mp_BotSec->Serialize(ar);

		ar << m_IsNumPlaceSideBar;
		m_SideMainBars.Serialize(ar);
		m_SideStirrupBars.Serialize(ar);

		ar << m_CoverShapeCode;
		ar << m_StirrupShapeCode;

		ar << m_bIsNumPlaceShearBar;
		m_ShearBars.Serialize(ar);
		ar << m_nShearBarRate;
	}
	else
	{
		mp_TopSec->Serialize(ar);
		mp_CenSec->Serialize(ar);
		mp_BotSec->Serialize(ar);

		ar >> m_IsNumPlaceSideBar;
		m_SideMainBars.Serialize(ar);
		m_SideStirrupBars.Serialize(ar);

		long CoverShapeCode = 0, StirrupShapeCode = 0;

		ar >> CoverShapeCode;
		ar >> StirrupShapeCode;

		m_CoverShapeCode = (MSREBAR_SHAPECODE)CoverShapeCode;
		m_StirrupShapeCode = (MSREBAR_SHAPECODE)StirrupShapeCode;

		if (MSVersionInfo::GetCurrentVersion() >= 20200111)
		{
			ar >> m_bIsNumPlaceShearBar;
			m_ShearBars.Serialize(ar);
			ar >> m_nShearBarRate;
		}
	}
}

void MDRcButtressM::CreateSections()
{
	if(mp_TopSec ==NULL)	mp_TopSec = new MDRcButtressSec;
	if(mp_CenSec==NULL)		mp_CenSec = new MDRcButtressSec;
	if(mp_BotSec ==NULL)		mp_BotSec = new MDRcButtressSec;
}

vector<double> MDRcButtressM::GetMainBarDiaAllLayer()
{
	vector<double> DiaArr;

	if(mp_TopSec)
	{
		vector<double> TopSecBarDiaArr = mp_TopSec->GetMainBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), TopSecBarDiaArr.begin(), TopSecBarDiaArr.end());
	}

	if(mp_CenSec)
	{
		vector<double> CenSecBarDiaArr = mp_CenSec->GetMainBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), CenSecBarDiaArr.begin(), CenSecBarDiaArr.end());
	}

	if(mp_BotSec)
	{
		vector<double> BotSecBarDiaArr = mp_BotSec->GetMainBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), BotSecBarDiaArr.begin(), BotSecBarDiaArr.end());
	}

	if(m_IsNumPlaceSideBar)
	{
		vector<double> SideMainBarDiaArr = m_SideMainBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), SideMainBarDiaArr.begin(), SideMainBarDiaArr.end());
	}
	else
	{
		vector<double> SideStirrupBarDiaArr = m_SideStirrupBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), SideStirrupBarDiaArr.begin(), SideStirrupBarDiaArr.end());
	}

	if (m_bIsNumPlaceShearBar)
	{
		long nTotalBars = m_ShearBars.GetTotalBars();
		if (nTotalBars > 0)
		{
			vector<double> ShearBarDiaIdxArr = m_ShearBars.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShearBarDiaIdxArr.begin(), ShearBarDiaIdxArr.end());
		}
	}
	else
	{
		vector<double> ShearBarDiaIdxArr = m_ShearBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), ShearBarDiaIdxArr.begin(), ShearBarDiaIdxArr.end());
	}

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const long& a, const long& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MDRcButtressM::GetStirrupBarDiaAllLayer()
{
	vector<double> DiaArr;

	if(mp_TopSec)
	{
		vector<double> TopSecBarDiaArr = mp_TopSec->GetStirrupBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), TopSecBarDiaArr.begin(), TopSecBarDiaArr.end());
	}

	if(mp_CenSec)
	{
		vector<double> CenSecBarDiaArr = mp_CenSec->GetStirrupBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), CenSecBarDiaArr.begin(), CenSecBarDiaArr.end());
	}

	if(mp_BotSec)
	{
		vector<double> BotSecBarDiaArr = mp_BotSec->GetStirrupBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), BotSecBarDiaArr.begin(), BotSecBarDiaArr.end());
	}

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const double& a, const double& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

long MDRcButtressM::GetMaxTopLayer()
{
	long numLayers = mp_TopSec->m_TBars.GetNumSets();
	numLayers = max(numLayers, mp_CenSec->m_TBars.GetNumSets());
	numLayers = max(numLayers, mp_BotSec->m_TBars.GetNumSets());

	return numLayers;
}

long MDRcButtressM::GetMaxBotLayer()
{
	long numLayers = mp_TopSec->m_BBars.GetNumSets();
	numLayers = max(numLayers, mp_CenSec->m_BBars.GetNumSets());
	numLayers = max(numLayers, mp_BotSec->m_BBars.GetNumSets());

	return numLayers;
}
double MDRcButtressM::GetTopBarDia(int layerNum)
{
	return max(max(mp_TopSec->GetTMaxBarDia(layerNum),mp_CenSec->GetTMaxBarDia(layerNum)),mp_BotSec->GetTMaxBarDia(layerNum));
}
double MDRcButtressM::GetBotBarDia(int layerNum)
{
	return max(max(mp_TopSec->GetBMaxBarDia(layerNum),mp_CenSec->GetBMaxBarDia(layerNum)),mp_BotSec->GetBMaxBarDia(layerNum));
}
double MDRcButtressM::GetMainBarDia()
{
	return max(max(mp_TopSec->GetMaxBarDia(), mp_CenSec->GetMaxBarDia()), mp_BotSec->GetMaxBarDia());
}
double MDRcButtressM::GetStrpBarDia()
{
	return max(max(mp_TopSec->GetStrpDia(),mp_CenSec->GetStrpDia()),mp_BotSec->GetStrpDia());
}
double MDRcButtressM::GetTopLayerDepth(long LayerNum, double dCover)
{
	double dStrpDia = GetStrpBarDia();
	double dMaxDia = GetTopBarDia(0);
	double dDepth = dCover + dStrpDia+ dMaxDia/2;//1단
	double barLayerGap = 25;
	for (long iLayer = 2; iLayer <= LayerNum; iLayer++)//2단 이상부터
	{
		dDepth += dMaxDia / 2;
		dMaxDia = GetTopBarDia(iLayer);
		dDepth += barLayerGap + dMaxDia / 2;
	}
	return dDepth;	
}
double MDRcButtressM::GetBotLayerDepth(long LayerNum, double dCover)
{
	double dStrpDia = GetStrpBarDia();
	double dMaxDia = GetBotBarDia(0);
	double dDepth = dCover + dStrpDia + dMaxDia / 2;//1단
	double barLayerGap = 25;
	
	for (long iLayer = 2; iLayer <= LayerNum; iLayer++)//2단이상부터
	{
		dDepth += dMaxDia / 2;
		dMaxDia = GetTopBarDia(iLayer);
		dDepth += barLayerGap + dMaxDia / 2;
	}
	return dDepth;
}
long MDRcButtressM::GetNumSideBar(double dDepth, double dCover)
{
	long numBar = 0;
	if(m_IsNumPlaceSideBar)
	{
		numBar = m_SideMainBars.GetTotalBars();
	}
	else
	{
		double dSpacing = m_SideStirrupBars.GetSpacing();
		if (dSpacing < DTOL_GM)
			return numBar;
				
		double dTopY = GetTopLayerDepth(GetMaxTopLayer(), dCover);
		double dBotY = GetBotLayerDepth(GetMaxBotLayer(), dCover);
		double dNumBar = (dDepth - fabs(dTopY) - fabs(dBotY))/dSpacing;
		numBar = (long)dNumBar;
		if (dNumBar - numBar < DTOL1)
			numBar --;
	}
	return numBar;
}
long MDRcButtressM::GetNumHorShearBar(double dDepth, double dCover)
{
	long numBar = 0;
	
	if(m_bIsNumPlaceShearBar)
		numBar = m_ShearBars.GetTotalBars();
	else if(m_nShearBarRate > 0)//비율배근
	{		
		long numSideBar = GetNumSideBar(dDepth, dCover);
		if(numSideBar > 0)
		{
			numBar = numSideBar/m_nShearBarRate + numSideBar%m_nShearBarRate;
			if(numBar == 0)
				numBar = 1;
		}
	}
	
	return numBar;
}
double MDRcButtressM::GetHorShearBarDia()
{	
	return m_ShearBars.GetDeformedBarDiameter();
}