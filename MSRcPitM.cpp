#include "StdAfx.h"
#include "MSRcPitM.h"


IMPLEMENT_SERIAL(MSRcPitM, MDMember, VERSIONABLE_SCHEMA | 1)

MSRcPitM::MSRcPitM(void)
{
	m_dCvrConc = 80;
	m_dPitDepth = 500;

	CString sBar = MSDeformedBar::GetStirrupBarStr(300);
	m_TopXBar.InitData(sBar);	
	m_TopYBar.InitData(sBar);	
	m_BotXBar.InitData(sBar);	
	m_BotYBar.InitData(sBar);	
	m_InHorBar.InitData(sBar);	
	m_InVerBar.InitData(sBar);	
	m_OutHorBar.InitData(sBar);	
	m_OutVerBar.InitData(sBar);
	m_SpacerBars.InitData(MSDeformedBar::GetStirrupBarStr(1000));

	m_bSpacerBars = TRUE;
	m_pTopSpacerShape = new SpacerShape();

	m_pBotSpacerShape = new SpacerShape();

}

MSRcPitM::~MSRcPitM(void)
{
	if(m_pTopSpacerShape) delete m_pTopSpacerShape;
	if(m_pBotSpacerShape) delete m_pBotSpacerShape;

}


void MSRcPitM::CopyFromMe(MDMember* pSource)
{
	if(pSource==NULL)	return;
	MSRcPitM* pSourcePitM = (MSRcPitM*)pSource;
	m_dPitDepth = pSourcePitM->m_dPitDepth;
	m_TopXBar.CopyFromMe(&pSourcePitM->m_TopXBar);
	m_TopYBar.CopyFromMe(&pSourcePitM->m_TopYBar);
	m_BotXBar.CopyFromMe(&pSourcePitM->m_BotXBar);
	m_BotYBar.CopyFromMe(&pSourcePitM->m_BotYBar);

	m_InHorBar.CopyFromMe(&pSourcePitM->m_InHorBar);
	m_InVerBar.CopyFromMe(&pSourcePitM->m_InVerBar);
	m_OutHorBar.CopyFromMe(&pSourcePitM->m_OutHorBar);
	m_OutVerBar.CopyFromMe(&pSourcePitM->m_OutVerBar);

	m_SpacerBars.CopyFromMe(&pSourcePitM->m_SpacerBars);
	m_bSpacerBars = pSourcePitM->m_bSpacerBars;

	MDMember::CopyFromMe(pSource);
}

void MSRcPitM::Dump(EFS::EFS_Buffer& buffer)
{
	MDMember::Dump(buffer);

	buffer << m_bSpacerBars << m_dPitDepth;

	m_TopXBar.Dump(buffer);
	m_TopYBar.Dump(buffer);
	m_BotXBar.Dump(buffer);
	m_BotYBar.Dump(buffer);
	m_InHorBar.Dump(buffer);
	m_InVerBar.Dump(buffer);
	m_OutHorBar.Dump(buffer);
	m_OutVerBar.Dump(buffer);
	m_SpacerBars.Dump(buffer);
}

void MSRcPitM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDMember::Recovery(buffer);
	buffer >> m_bSpacerBars >> m_dPitDepth;
	
	m_TopXBar.Recovery(buffer);
	m_TopYBar.Recovery(buffer);
	m_BotXBar.Recovery(buffer);
	m_BotYBar.Recovery(buffer);
	m_InHorBar.Recovery(buffer);
	m_InVerBar.Recovery(buffer);
	m_OutHorBar.Recovery(buffer);
	m_OutVerBar.Recovery(buffer);
	m_SpacerBars.Recovery(buffer);
}

void MSRcPitM::Serialize(CArchive &ar)
{
	MDMember::Serialize(ar);
	if(ar.IsStoring())
	{
		m_TopXBar.Serialize(ar);
		m_TopYBar.Serialize(ar);
		m_BotXBar.Serialize(ar);
		m_BotYBar.Serialize(ar);


		m_InHorBar.Serialize(ar);
		m_InVerBar.Serialize(ar);
		m_OutHorBar.Serialize(ar);
		m_OutVerBar.Serialize(ar);

		m_SpacerBars.Serialize(ar);

		m_pTopSpacerShape->Serialize(ar);
		m_pBotSpacerShape->Serialize(ar);

		// 20120921
		ar << m_bSpacerBars;

		// 20121005
		ar << m_dPitDepth;
	}
	else
	{
		m_TopXBar.Serialize(ar);
		m_TopYBar.Serialize(ar);
		m_BotXBar.Serialize(ar);
		m_BotYBar.Serialize(ar);

		m_InHorBar.Serialize(ar);
		m_InVerBar.Serialize(ar);
		m_OutHorBar.Serialize(ar);
		m_OutVerBar.Serialize(ar);

		m_SpacerBars.Serialize(ar);

		m_pTopSpacerShape->Serialize(ar);
		m_pBotSpacerShape->Serialize(ar);

		ar >> m_bSpacerBars;
		ar >> m_dPitDepth;

		//if(m_dThick < DTOL1)
		//{
		//	m_dThick = 1000.0;
		//	AutoCalcTopSpacerShape();
		//	AutoCalcBotSpacerShape();
		//}
	}
}

vector<double> MSRcPitM::GetMainBarDiaAllLayer()
{
	vector<double> DiaArr;

	vector<double> TopXBarDiaArr = m_TopXBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), TopXBarDiaArr.begin(), TopXBarDiaArr.end());

	vector<double> TopYBarDiaArr = m_TopYBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), TopYBarDiaArr.begin(), TopYBarDiaArr.end());

	vector<double> BotXBarDiaArr = m_BotXBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), BotXBarDiaArr.begin(), BotXBarDiaArr.end());

	vector<double> BotYBarDiaArr = m_BotYBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), BotYBarDiaArr.begin(), BotYBarDiaArr.end());

	vector<double> InHorBarDiaArr = m_InHorBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), InHorBarDiaArr.begin(), InHorBarDiaArr.end());

	vector<double> InVerBarDiaArr = m_InVerBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), InVerBarDiaArr.begin(), InVerBarDiaArr.end());

	vector<double> OutHorBarDiaArr = m_OutHorBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), OutHorBarDiaArr.begin(), OutHorBarDiaArr.end());

	vector<double> OutVerBarDiaArr = m_OutVerBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), OutVerBarDiaArr.begin(), OutVerBarDiaArr.end());

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const double& a, const double& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MSRcPitM::GetStirrupBarDiaAllLayer()
{
	vector<double> DiaArr;

	if(m_bSpacerBars)
	{
		vector<double> SpacerBarDiaArr = m_SpacerBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), SpacerBarDiaArr.begin(), SpacerBarDiaArr.end());
	}

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const double& a, const double& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());


	return DiaArr;
}

//
//void MSRcPitM::AutoCalcTopSpacerShape(double dPileCover/*=0*/ )
//{
//	if(m_pTopSpacerShape ==NULL)
//		return;
//	double dBotCover = GetCover();//하부피복
//	if(dPileCover>GetCover())
//		dBotCover = dPileCover;
//	double dSpacing = max(m_BotYBar.GetSpacing(), m_BotXBar.GetSpacing());
//	long nTopDia = (long)max(m_TopXBar.GetBarDia(),m_TopYBar.GetBarDia());
//	long nBotDia = (long)max(m_BotXBar.GetBarDia(),m_BotYBar.GetBarDia());
//	m_pTopSpacerShape->AutoCalc(dSpacing,GetThick(),GetCover(),dBotCover,(long)m_SpacerBars.GetBarDia(),nTopDia,nBotDia);
//}
//
//void MSRcPitM::AutoCalcBotSpacerShape( double dPileCover/*=150*/ )
//{
//	if(m_pBotSpacerShape ==NULL)
//		return;
//	if(dPileCover<GetCover())
//		return;
//	double dSpacing = max(m_BotYBar.GetSpacing(), m_BotXBar.GetSpacing());
//	m_pBotSpacerShape->AutoCalc(dSpacing,dPileCover,0,0,(long)m_SpacerBars.GetBarDia(),0,0);
//}
