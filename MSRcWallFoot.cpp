#include "StdAfx.h"
#include "MSRcWallFoot.h"

IMPLEMENT_SERIAL(MSRcWallFootM, MDMember, VERSIONABLE_SCHEMA | 1)
	MSRcWallFootM::MSRcWallFootM(void)
{
	m_dCvrConc = 80;

	m_bIsPileFooting = TRUE;
	m_Haunch = HAUNCH_TYPE_RECTANGLE;
	m_bBotHookType = TRUE;		
	m_bIsLxTopBar = FALSE;			
	m_bIsLyTopBar = FALSE;
	m_BotSpacerType = 0;   
	m_bAddSpacer = TRUE;

	CString sBar = _T("");

	sBar = MSDeformedBar::GetStirrupBarStr(300);
	m_ShortBotBars.InitData(sBar);
	m_LongBotBars.InitData(sBar);
	m_ShortTopBars.InitData(sBar);
	m_LongTopBars.InitData(sBar);	
	sBar = MSDeformedBar::GetStirrupBarStr(1000);
	m_SpacerBars.InitData(sBar);

	m_pTopSpacerShape = new SpacerShape();
	m_pBotSpacerShape = new SpacerShape();
}

MSRcWallFootM::~MSRcWallFootM(void)
{
	delete m_pTopSpacerShape;
	delete m_pBotSpacerShape;
}

void MSRcWallFootM::CopyFromMe(MDMember* pSource)
{
	if(pSource==NULL)	return;
	MSRcWallFootM* pSourceWFootM = (MSRcWallFootM*)pSource;
	m_ShortBotBars.CopyFromMe(&pSourceWFootM->m_ShortBotBars);
	m_LongBotBars.CopyFromMe(&pSourceWFootM->m_LongBotBars);
	m_ShortTopBars.CopyFromMe(&pSourceWFootM->m_ShortTopBars);
	m_LongTopBars.CopyFromMe(&pSourceWFootM->m_LongTopBars);

	m_bIsPileFooting = pSourceWFootM->m_bIsPileFooting;
	m_Haunch = pSourceWFootM->m_Haunch;
	m_bAddSpacer = pSourceWFootM->m_bAddSpacer;

	m_bBotHookType = pSourceWFootM->m_bBotHookType;		
	m_bIsLxTopBar = pSourceWFootM->m_bIsLxTopBar;
	m_bIsLyTopBar = pSourceWFootM->m_bIsLyTopBar;
	m_BotSpacerType = pSourceWFootM->m_BotSpacerType;   

	MDMember::CopyFromMe(pSource);
}

void MSRcWallFootM::Dump(EFS::EFS_Buffer& buffer)
{
	MDMember::Dump(buffer);

	buffer << m_bIsPileFooting << m_Haunch << m_bBotHookType << m_bAddSpacer;
	buffer << m_bIsLxTopBar << m_bIsLyTopBar;

	m_ShortBotBars.Dump(buffer); 
	m_LongBotBars.Dump(buffer);
	m_ShortTopBars.Dump(buffer);
	m_LongTopBars.Dump(buffer);
	m_SpacerBars.Dump(buffer);
}

void MSRcWallFootM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDMember::Recovery(buffer);

	buffer >> m_bIsPileFooting >> m_Haunch >> m_bBotHookType >> m_bAddSpacer;
	buffer >> m_bIsLxTopBar >> m_bIsLyTopBar;

	m_ShortBotBars.Recovery(buffer); 
	m_LongBotBars.Recovery(buffer);
	m_ShortTopBars.Recovery(buffer);
	m_LongTopBars.Recovery(buffer);
	m_SpacerBars.Recovery(buffer);
}

void MSRcWallFootM::Serialize(CArchive &ar)
{
	MDMember::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_bIsPileFooting;
		ar << m_Haunch;
		ar << m_bBotHookType;
		ar << m_bAddSpacer;

		m_ShortBotBars.Serialize(ar);
		m_LongBotBars.Serialize(ar);
		m_ShortTopBars.Serialize(ar);
		m_LongTopBars.Serialize(ar);
		m_SpacerBars.Serialize(ar);

		m_pTopSpacerShape->Serialize(ar);
		m_pBotSpacerShape->Serialize(ar);

		ar << m_bIsLxTopBar;
		ar << m_bIsLyTopBar;
	}
	else
	{
		ar >> m_bIsPileFooting;
		ar >> m_Haunch;
		ar >> m_bBotHookType;
		ar >> m_bAddSpacer;

		m_ShortBotBars.Serialize(ar);
		m_LongBotBars.Serialize(ar);
		m_ShortTopBars.Serialize(ar);
		m_LongTopBars.Serialize(ar);
		m_SpacerBars.Serialize(ar);

		m_pTopSpacerShape->Serialize(ar);
		m_pBotSpacerShape->Serialize(ar);

		ar >> m_bIsLxTopBar;
		ar >> m_bIsLyTopBar;

	}
}

void MSRcWallFootM::AutoCalcTopSpacerShape( double dMemberThick,double dTopCover,double dBotCover)
{
	if(m_pTopSpacerShape ==NULL)
		return;
	double dSpacing = max(m_ShortBotBars.GetSpacing(), m_LongBotBars.GetSpacing());
	double dBotDia = max(m_ShortBotBars.GetDeformedBarDiameter(),m_LongBotBars.GetDeformedBarDiameter());
	m_pTopSpacerShape->AutoCalc(dSpacing,dMemberThick,dTopCover,dBotCover,m_SpacerBars.GetDeformedBarDiameter(),dBotDia,dBotDia);

}

void MSRcWallFootM::AutoCalcBotSpacerShape( double dPileCover/*=150*/ )
{
	if(m_pBotSpacerShape ==NULL)
		return;
	if(dPileCover<GetCover())
		return;
	double dSpacing = max(m_ShortBotBars.GetSpacing(), m_LongBotBars.GetSpacing());
	m_pBotSpacerShape->AutoCalc(dSpacing,dPileCover,0,0,m_SpacerBars.GetDeformedBarDiameter(),0,0);
}

vector<double> MSRcWallFootM::GetMainBarDiaAllLayer()
{
	vector<double> DiaArr;

	vector<double> ShortBotBarDiaArr = m_ShortBotBars.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), ShortBotBarDiaArr.begin(), ShortBotBarDiaArr.end());

	vector<double> LongBotBarDiaArr = m_LongBotBars.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), LongBotBarDiaArr.begin(), LongBotBarDiaArr.end());

	if (m_bIsLxTopBar)
	{
		vector<double> ShortTopBarDiaArr = m_ShortTopBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), ShortTopBarDiaArr.begin(), ShortTopBarDiaArr.end());
	}

	if (m_bIsLyTopBar)
	{
		vector<double> LongTopBarDiaArr = m_LongTopBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), LongTopBarDiaArr.begin(), LongTopBarDiaArr.end());
	}

	//vector<double> SpacerBarDiaArr = m_SpacerBars.GetBarDiaAllLayer();
	//DiaArr.insert(DiaArr.end(), SpacerBarDiaArr.begin(), SpacerBarDiaArr.end());

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const double& a, const double& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MSRcWallFootM::GetStirrupBarDiaAllLayer()
{
	vector<double> DiaArr;

	return DiaArr;
}

double MSRcWallFootM::GetBotMainBarMaxDeformedBarDiameter()
{
	vector<double> DiaArr;

	vector<double> ShortBotBarDiaArr = m_ShortBotBars.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), ShortBotBarDiaArr.begin(), ShortBotBarDiaArr.end());

	vector<double> LongBotBarDiaArr = m_LongBotBars.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), LongBotBarDiaArr.begin(), LongBotBarDiaArr.end());

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
