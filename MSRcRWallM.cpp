#include "StdAfx.h"
#include "MSRcRWallM.h"
#include "..\GMLib\MSVersionInfo.h"


IMPLEMENT_SERIAL(MSRcRWallM, MDMember, VERSIONABLE_SCHEMA | 1)
MSRcRWallM::MSRcRWallM(void)
{
	m_nExtVBarType = 3;
	m_bUse2VMidBar = 0;	
	m_dTopSRegion = 1000;								
	m_dBotSRegion = 1000;	
	m_nOutdCvrConc = 40;
	m_dCvrConc = 20;
	m_nVTopBarLength = 1000;
	m_nVBotBarLength = 1000;

	m_HSameCheck = TRUE;
	m_bUseAllStirrup = FALSE;	
	m_bUseTopStirrup = FALSE;						
	m_bUseBotStirrup = FALSE;						
	m_HInOutCheck = FALSE;

	CString sBar = MSDeformedBar::GetStirrupBarStr(300);
	m_VOutTopBar.InitData(sBar);	
	m_VInMidBar.InitData(sBar);
	m_VOutMidBar.InitData(sBar);
	m_2VMidBar.InitData(sBar);
	m_VOutBotBar.InitData(sBar);

	m_HInTopBar.InitData(sBar);
	m_HOutTopBar.InitData(sBar);
	m_HInMidBar.InitData(sBar);
	m_HOutMidBar.InitData(sBar);
	m_HInBotBar.InitData(sBar);
	m_HOutBotBar.InitData(sBar);
							
	m_HTopStirrup.InitData(sBar);
	m_VTopStirrup.InitData(sBar);
	m_HBotStirrup.InitData(sBar);
	m_VBotStirrup.InitData(sBar);

	m_nVInBarType = 3;
	m_VInTopBar.InitData(sBar);
	m_VInBotBar.InitData(sBar);
	m_nVInTopBarLength = 1000;
	m_nVInBotBarLength = 1000;

	m_bStartShearAddBar = FALSE;
	m_bEndShearAddBar = FALSE;
	m_StartShearHBars.InitData(sBar);
	m_StartShearVBars.InitData(sBar);
	m_EndShearHBars.InitData(sBar);
	m_EndShearVBars.InitData(sBar);
	m_nStartShearAddBarLength = 1000;
	m_nEndShearAddBarLength = 1000;

	m_bAllShearAddBar = FALSE;
	m_AllShearHBars.InitData(sBar);
	m_AllShearVBars.InitData(sBar);

	m_VOutMidAddBar.InitData(sBar);
	m_VInMidAddBar.InitData(sBar);
	m_bVOutMidAddBar = FALSE;
	m_bVInMidAddBar = FALSE;
	m_nVMidAddBarLength = 1000;
	m_nVInMidAddBarLength = 1000;

	m_HInStartAddBar.InitData(sBar);
	m_HInMidAddBar.InitData(sBar);
	m_HInEndAddBar.InitData(sBar);
	m_bHInStartAddBar = FALSE;
	m_bHInMidAddBar = FALSE;
	m_bHInEndAddBar = FALSE;
	m_nHInStartAddBarLength = 1000;
	m_nHInMidAddBarLength = 1000;
	m_nHInEndAddBarLength = 1000;

	m_HOutStartAddBar.InitData(sBar);
	m_HOutMidAddBar.InitData(sBar);
	m_HOutEndAddBar.InitData(sBar);
	m_bHOutStartAddBar = FALSE;
	m_bHOutMidAddBar = FALSE;
	m_bHOutEndAddBar = FALSE;
	m_nHOutStartAddBarLength = 1000;
	m_nHOutMidAddBarLength = 1000;
	m_nHOutEndAddBarLength = 1000;

	m_bVOutTopAddBar = TRUE;
	m_bVOutBotAddBar = TRUE;
	m_bVInTopAddBar = TRUE;
	m_bVInBotAddBar = TRUE;
}

MSRcRWallM::~MSRcRWallM(void)
{
}

void MSRcRWallM::CopyFromMe(MDMember* pSource)
{
	if(pSource==NULL)	return;
	MSRcRWallM* pSourceRWallM = (MSRcRWallM*)pSource;	
	m_nExtVBarType = pSourceRWallM->m_nExtVBarType;
	m_bUse2VMidBar = pSourceRWallM->m_bUse2VMidBar;
	m_bUseAllStirrup = pSourceRWallM->m_bUseAllStirrup;
	m_bUseTopStirrup = pSourceRWallM->m_bUseTopStirrup;
	m_bUseBotStirrup = pSourceRWallM->m_bUseBotStirrup;
	m_dTopSRegion = pSourceRWallM->m_dTopSRegion;
	m_dBotSRegion = pSourceRWallM->m_dBotSRegion;
	m_HSameCheck = pSourceRWallM->m_HSameCheck;
	m_HInOutCheck = pSourceRWallM->m_HInOutCheck;
	m_nOutdCvrConc = pSourceRWallM->m_nOutdCvrConc;
	m_nVTopBarLength = pSourceRWallM->m_nVTopBarLength;
	m_nVBotBarLength = pSourceRWallM->m_nVBotBarLength;

	m_VOutTopBar.CopyFromMe(&pSourceRWallM->m_VOutTopBar);
	m_VInMidBar.CopyFromMe(&pSourceRWallM->m_VInMidBar);
	m_VOutMidBar.CopyFromMe(&pSourceRWallM->m_VOutMidBar);
	m_2VMidBar.CopyFromMe(&pSourceRWallM->m_2VMidBar);
	m_VOutBotBar.CopyFromMe(&pSourceRWallM->m_VOutBotBar);

	m_HInTopBar.CopyFromMe(&pSourceRWallM->m_HInTopBar);
	m_HOutTopBar.CopyFromMe(&pSourceRWallM->m_HOutTopBar);
	m_HInMidBar.CopyFromMe(&pSourceRWallM->m_HInMidBar);
	m_HOutMidBar.CopyFromMe(&pSourceRWallM->m_HOutMidBar);
	m_HInBotBar.CopyFromMe(&pSourceRWallM->m_HInBotBar);
	m_HOutBotBar.CopyFromMe(&pSourceRWallM->m_HOutBotBar);

	m_HTopStirrup.CopyFromMe(&pSourceRWallM->m_HTopStirrup);
	m_VTopStirrup.CopyFromMe(&pSourceRWallM->m_VTopStirrup);
	m_HBotStirrup.CopyFromMe(&pSourceRWallM->m_HBotStirrup);
	m_VBotStirrup.CopyFromMe(&pSourceRWallM->m_VBotStirrup);

	m_nVInBarType = pSourceRWallM->m_nVInBarType;
	m_VInTopBar.CopyFromMe(&pSourceRWallM->m_VInTopBar);
	m_VInBotBar.CopyFromMe(&pSourceRWallM->m_VInBotBar);
	m_nVInTopBarLength = pSourceRWallM->m_nVInTopBarLength;
	m_nVInBotBarLength = pSourceRWallM->m_nVInBotBarLength;

	m_bStartShearAddBar = pSourceRWallM->m_bStartShearAddBar;
	m_bEndShearAddBar = pSourceRWallM->m_bEndShearAddBar;
	m_StartShearHBars.CopyFromMe(&pSourceRWallM->m_StartShearHBars);
	m_StartShearVBars.CopyFromMe(&pSourceRWallM->m_StartShearVBars);
	m_EndShearHBars.CopyFromMe(&pSourceRWallM->m_EndShearHBars);
	m_EndShearVBars.CopyFromMe(&pSourceRWallM->m_EndShearVBars);
	m_nStartShearAddBarLength =  pSourceRWallM->m_nStartShearAddBarLength;
	m_nEndShearAddBarLength =  pSourceRWallM->m_nEndShearAddBarLength;

	m_bAllShearAddBar = pSourceRWallM->m_bAllShearAddBar;
	m_AllShearHBars.CopyFromMe(&pSourceRWallM->m_AllShearHBars);
	m_AllShearVBars.CopyFromMe(&pSourceRWallM->m_AllShearVBars);

	m_VOutMidAddBar.CopyFromMe(&pSourceRWallM->m_VOutMidAddBar);
	m_VInMidAddBar.CopyFromMe(&pSourceRWallM->m_VInMidAddBar);
	m_bVOutMidAddBar = pSourceRWallM->m_bVOutMidAddBar;
	m_bVInMidAddBar = pSourceRWallM->m_bVInMidAddBar;
	m_nVMidAddBarLength = pSourceRWallM->m_nVMidAddBarLength;
	m_nVInMidAddBarLength = pSourceRWallM->m_nVInMidAddBarLength;

	m_HInStartAddBar.CopyFromMe(&pSourceRWallM->m_HInStartAddBar);
	m_HInMidAddBar.CopyFromMe(&pSourceRWallM->m_HInMidAddBar);
	m_HInEndAddBar.CopyFromMe(&pSourceRWallM->m_HInEndAddBar);
	m_bHInStartAddBar = pSourceRWallM->m_bHInStartAddBar;
	m_bHInMidAddBar = pSourceRWallM->m_bHInMidAddBar;
	m_bHInEndAddBar = pSourceRWallM->m_bHInEndAddBar;
	m_nHInStartAddBarLength = pSourceRWallM->m_nHInStartAddBarLength;
	m_nHInMidAddBarLength = pSourceRWallM->m_nHInMidAddBarLength;
	m_nHInEndAddBarLength = pSourceRWallM->m_nHInEndAddBarLength;

	m_HOutStartAddBar.CopyFromMe(&pSourceRWallM->m_HOutStartAddBar);
	m_HOutMidAddBar.CopyFromMe(&pSourceRWallM->m_HOutMidAddBar);
	m_HOutEndAddBar.CopyFromMe(&pSourceRWallM->m_HOutEndAddBar);
	m_bHOutStartAddBar = pSourceRWallM->m_bHOutStartAddBar;
	m_bHOutMidAddBar = pSourceRWallM->m_bHOutMidAddBar;
	m_bHOutEndAddBar = pSourceRWallM->m_bHOutEndAddBar;
	m_nHOutStartAddBarLength = pSourceRWallM->m_nHOutStartAddBarLength;
	m_nHOutMidAddBarLength = pSourceRWallM->m_nHOutMidAddBarLength;
	m_nHOutEndAddBarLength = pSourceRWallM->m_nHOutEndAddBarLength;

	m_bVOutTopAddBar = pSourceRWallM->m_bVOutTopAddBar;
	m_bVOutBotAddBar = pSourceRWallM->m_bVOutBotAddBar;
	m_bVInTopAddBar = pSourceRWallM->m_bVInTopAddBar;
	m_bVInBotAddBar = pSourceRWallM->m_bVInBotAddBar;

	MDMember::CopyFromMe(pSource);
}

void MSRcRWallM::Dump(EFS::EFS_Buffer& buffer)
{
	MDMember::Dump(buffer);

	buffer << m_nExtVBarType << m_bUse2VMidBar << m_dTopSRegion << m_dBotSRegion;
	buffer << m_HSameCheck << m_bUseAllStirrup << m_bUseTopStirrup << m_bUseBotStirrup;
	buffer << m_HInOutCheck << m_nOutdCvrConc << m_nVTopBarLength << m_nVBotBarLength;
	buffer << m_nVInBarType << m_nVInTopBarLength << m_nVInBotBarLength;
	buffer << m_bStartShearAddBar << m_bEndShearAddBar;
	buffer << m_nStartShearAddBarLength << m_nEndShearAddBarLength;
	buffer << m_bAllShearAddBar;
	buffer << m_bVOutMidAddBar << m_bVInMidAddBar;
	buffer << m_nVMidAddBarLength << m_nVInMidAddBarLength;
	buffer << m_bHInStartAddBar << m_bHInMidAddBar << m_bHInEndAddBar;
	buffer << m_nHInStartAddBarLength << m_nHInMidAddBarLength << m_nHInEndAddBarLength;
	buffer << m_bHOutStartAddBar << m_bHOutMidAddBar << m_bHOutEndAddBar;
	buffer << m_nHOutStartAddBarLength << m_nHOutMidAddBarLength << m_nHOutEndAddBarLength;
	buffer << m_bVOutTopAddBar << m_bVOutBotAddBar << m_bVInTopAddBar << m_bVInBotAddBar;

	m_VOutTopBar.Dump(buffer);
	m_VInMidBar.Dump(buffer);
	m_VOutMidBar.Dump(buffer);
	m_2VMidBar.Dump(buffer);
	m_VOutBotBar.Dump(buffer);
	m_HInTopBar.Dump(buffer);
	m_HOutTopBar.Dump(buffer);
	m_HInMidBar.Dump(buffer);
	m_HOutMidBar.Dump(buffer);
	m_HInBotBar.Dump(buffer);
	m_HOutBotBar.Dump(buffer);;
	m_HTopStirrup.Dump(buffer);
	m_VTopStirrup.Dump(buffer);
	m_HBotStirrup.Dump(buffer);
	m_VBotStirrup.Dump(buffer);;
	m_VInTopBar.Dump(buffer);
	m_VInBotBar.Dump(buffer);;
	m_StartShearHBars.Dump(buffer);
	m_StartShearVBars.Dump(buffer);
	m_EndShearHBars.Dump(buffer);
	m_EndShearVBars.Dump(buffer);
	m_AllShearHBars.Dump(buffer);
	m_AllShearVBars.Dump(buffer);
	m_VOutMidAddBar.Dump(buffer);
	m_VInMidAddBar.Dump(buffer);
	m_HInStartAddBar.Dump(buffer);
	m_HInMidAddBar.Dump(buffer);
	m_HInEndAddBar.Dump(buffer);
	m_HOutStartAddBar.Dump(buffer);
	m_HOutMidAddBar.Dump(buffer);
	m_HOutEndAddBar.Dump(buffer);
}

void MSRcRWallM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDMember::Recovery(buffer);

	buffer >> m_nExtVBarType >> m_bUse2VMidBar >> m_dTopSRegion >> m_dBotSRegion;
	buffer >> m_HSameCheck >> m_bUseAllStirrup >> m_bUseTopStirrup >> m_bUseBotStirrup;
	buffer >> m_HInOutCheck >> m_nOutdCvrConc >> m_nVTopBarLength >> m_nVBotBarLength;
	buffer >> m_nVInBarType >> m_nVInTopBarLength >> m_nVInBotBarLength;
	buffer >> m_bStartShearAddBar >> m_bEndShearAddBar;
	buffer >> m_nStartShearAddBarLength >> m_nEndShearAddBarLength;
	buffer >> m_bAllShearAddBar;
	buffer >> m_bVOutMidAddBar >> m_bVInMidAddBar;
	buffer >> m_nVMidAddBarLength >> m_nVInMidAddBarLength;
	buffer >> m_bHInStartAddBar >> m_bHInMidAddBar >> m_bHInEndAddBar;
	buffer >> m_nHInStartAddBarLength >> m_nHInMidAddBarLength >> m_nHInEndAddBarLength;
	buffer >> m_bHOutStartAddBar >> m_bHOutMidAddBar >> m_bHOutEndAddBar;
	buffer >> m_nHOutStartAddBarLength >> m_nHOutMidAddBarLength >> m_nHOutEndAddBarLength;
	buffer >> m_bVOutTopAddBar >> m_bVOutBotAddBar >> m_bVInTopAddBar >> m_bVInBotAddBar;

	m_VOutTopBar.Recovery(buffer);
	m_VInMidBar.Recovery(buffer);
	m_VOutMidBar.Recovery(buffer);
	m_2VMidBar.Recovery(buffer);
	m_VOutBotBar.Recovery(buffer);
	m_HInTopBar.Recovery(buffer);
	m_HOutTopBar.Recovery(buffer);
	m_HInMidBar.Recovery(buffer);
	m_HOutMidBar.Recovery(buffer);
	m_HInBotBar.Recovery(buffer);
	m_HOutBotBar.Recovery(buffer);;
	m_HTopStirrup.Recovery(buffer);
	m_VTopStirrup.Recovery(buffer);
	m_HBotStirrup.Recovery(buffer);
	m_VBotStirrup.Recovery(buffer);;
	m_VInTopBar.Recovery(buffer);
	m_VInBotBar.Recovery(buffer);;
	m_StartShearHBars.Recovery(buffer);
	m_StartShearVBars.Recovery(buffer);
	m_EndShearHBars.Recovery(buffer);
	m_EndShearVBars.Recovery(buffer);
	m_AllShearHBars.Recovery(buffer);
	m_AllShearVBars.Recovery(buffer);
	m_VOutMidAddBar.Recovery(buffer);
	m_VInMidAddBar.Recovery(buffer);
	m_HInStartAddBar.Recovery(buffer);
	m_HInMidAddBar.Recovery(buffer);
	m_HInEndAddBar.Recovery(buffer);
	m_HOutStartAddBar.Recovery(buffer);
	m_HOutMidAddBar.Recovery(buffer);
	m_HOutEndAddBar.Recovery(buffer);
}

void MSRcRWallM::Serialize(CArchive &ar)
{
	MDMember::Serialize(ar);
	if(ar.IsStoring())
	{		
		ar << m_nExtVBarType;
		ar << m_bUse2VMidBar;	
		ar << m_dTopSRegion;								
		ar << m_dBotSRegion;	

		ar << m_HSameCheck;
		ar << m_bUseAllStirrup;	
		ar << m_bUseTopStirrup;						
		ar << m_bUseBotStirrup;						
		ar << m_HInOutCheck;
		ar << m_nOutdCvrConc;
		ar << m_nVTopBarLength;
		ar << m_nVBotBarLength;

		m_VOutTopBar.Serialize(ar);
		m_VInMidBar.Serialize(ar);
		m_VOutMidBar.Serialize(ar);
		m_2VMidBar.Serialize(ar);
		m_VOutBotBar.Serialize(ar);

		m_HInTopBar.Serialize(ar);
		m_HOutTopBar.Serialize(ar);
		m_HInMidBar.Serialize(ar);
		m_HOutMidBar.Serialize(ar);
		m_HInBotBar.Serialize(ar);
		m_HOutBotBar.Serialize(ar);
		
		m_HTopStirrup.Serialize(ar);
		m_VTopStirrup.Serialize(ar);
		m_HBotStirrup.Serialize(ar);
		m_VBotStirrup.Serialize(ar);

		ar << m_nVInBarType;
		ar << m_nVInTopBarLength;
		ar << m_nVInBotBarLength;
		m_VInTopBar.Serialize(ar);
		m_VInBotBar.Serialize(ar);

		// 2013.09.10
		ar << m_bStartShearAddBar;
		ar << m_bEndShearAddBar;
		m_StartShearHBars.Serialize(ar);
		m_StartShearVBars.Serialize(ar);
		m_EndShearHBars.Serialize(ar);
		m_EndShearVBars.Serialize(ar);
		ar << m_nStartShearAddBarLength;
		ar << m_nEndShearAddBarLength;

		ar << m_bAllShearAddBar;
		m_AllShearHBars.Serialize(ar);
		m_AllShearVBars.Serialize(ar);

		m_VOutMidAddBar.Serialize(ar);
		ar << m_bVOutMidAddBar;
		ar << m_nVMidAddBarLength;

		m_VInMidAddBar.Serialize(ar);
		ar << m_bVInMidAddBar;
		ar << m_nVInMidAddBarLength;

		m_HOutStartAddBar.Serialize(ar);
		m_HOutMidAddBar.Serialize(ar);
		m_HOutEndAddBar.Serialize(ar);
		ar << m_bHOutStartAddBar;
		ar << m_bHOutMidAddBar;
		ar << m_bHOutEndAddBar;
		ar << m_nHOutStartAddBarLength;
		ar << m_nHOutMidAddBarLength;
		ar << m_nHOutEndAddBarLength;

		m_HInStartAddBar.Serialize(ar);
		m_HInMidAddBar.Serialize(ar);
		m_HInEndAddBar.Serialize(ar);
		ar << m_bHInStartAddBar;
		ar << m_bHInMidAddBar;
		ar << m_bHInEndAddBar;
		ar << m_nHInStartAddBarLength;
		ar << m_nHInMidAddBarLength;
		ar << m_nHInEndAddBarLength;
		ar << m_bVOutTopAddBar;
		ar << m_bVOutBotAddBar;
		ar << m_bVInTopAddBar;
		ar << m_bVInBotAddBar;
	}
	else
	{		
		ar >> m_nExtVBarType;
		ar >> m_bUse2VMidBar;	
		ar >> m_dTopSRegion;								
		ar >> m_dBotSRegion;	

		ar >> m_HSameCheck;
		ar >> m_bUseAllStirrup;	
		ar >> m_bUseTopStirrup;						
		ar >> m_bUseBotStirrup;						
		ar >> m_HInOutCheck;
		ar >> m_nOutdCvrConc;
		ar >> m_nVTopBarLength;
		ar >> m_nVBotBarLength;

		m_VOutTopBar.Serialize(ar);
		m_VInMidBar.Serialize(ar);
		m_VOutMidBar.Serialize(ar);
		m_2VMidBar.Serialize(ar);
		m_VOutBotBar.Serialize(ar);

		m_HInTopBar.Serialize(ar);
		m_HOutTopBar.Serialize(ar);
		m_HInMidBar.Serialize(ar);
		m_HOutMidBar.Serialize(ar);
		m_HInBotBar.Serialize(ar);
		m_HOutBotBar.Serialize(ar);

		m_HTopStirrup.Serialize(ar);
		m_VTopStirrup.Serialize(ar);
		m_HBotStirrup.Serialize(ar);
		m_VBotStirrup.Serialize(ar);

		ar >> m_nVInBarType;
		ar >> m_nVInTopBarLength;
		ar >> m_nVInBotBarLength;
		m_VInTopBar.Serialize(ar);
		m_VInBotBar.Serialize(ar);
		ar >> m_bStartShearAddBar;
		ar >> m_bEndShearAddBar;
		m_StartShearHBars.Serialize(ar);
		m_StartShearVBars.Serialize(ar);
		m_EndShearHBars.Serialize(ar);
		m_EndShearVBars.Serialize(ar);
		ar >> m_nStartShearAddBarLength;
		ar >> m_nEndShearAddBarLength;

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20180718)
		{
			ar >> m_bAllShearAddBar;
			m_AllShearHBars.Serialize(ar);
			m_AllShearVBars.Serialize(ar);
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20190405)
		{
			m_VOutMidAddBar.Serialize(ar);
			ar >> m_bVOutMidAddBar;
			ar >> m_nVMidAddBarLength;

			m_VInMidAddBar.Serialize(ar);
			ar >> m_bVInMidAddBar;
			ar >> m_nVInMidAddBarLength;

			m_HOutStartAddBar.Serialize(ar);
			m_HOutMidAddBar.Serialize(ar);
			m_HOutEndAddBar.Serialize(ar);
			ar >> m_bHOutStartAddBar;
			ar >> m_bHOutMidAddBar;
			ar >> m_bHOutEndAddBar;
			ar >> m_nHOutStartAddBarLength;
			ar >> m_nHOutMidAddBarLength;
			ar >> m_nHOutEndAddBarLength;

			m_HInStartAddBar.Serialize(ar);
			m_HInMidAddBar.Serialize(ar);
			m_HInEndAddBar.Serialize(ar);
			ar >> m_bHInStartAddBar;
			ar >> m_bHInMidAddBar;
			ar >> m_bHInEndAddBar;
			ar >> m_nHInStartAddBarLength;
			ar >> m_nHInMidAddBarLength;
			ar >> m_nHInEndAddBarLength;

			ar >> m_bVOutTopAddBar;
			ar >> m_bVOutBotAddBar;
			ar >> m_bVInTopAddBar;
			ar >> m_bVInBotAddBar;
		}
	}
}

vector<double> MSRcRWallM::GetMainBarDiaAllLayer()
{
	vector<double> DiaArr;

	// 수평근(Exterior)
	vector<double> HOutTopBarDiaArr = m_HOutTopBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), HOutTopBarDiaArr.begin(), HOutTopBarDiaArr.end());

	vector<double> HOutMidBarDiaArr = m_HOutMidBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), HOutMidBarDiaArr.begin(), HOutMidBarDiaArr.end());

	vector<double> HOutBotBarDiaArr = m_HOutBotBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), HOutBotBarDiaArr.begin(), HOutBotBarDiaArr.end());

	if (m_bHOutStartAddBar == TRUE)
	{
		vector<double> HOutStartAddBarDiaArr = m_HOutStartAddBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), HOutStartAddBarDiaArr.begin(), HOutStartAddBarDiaArr.end());
	}
	
	if (m_bHOutMidAddBar == TRUE)
	{
		vector<double> HOutMidAddBarDiaArr = m_HOutMidAddBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), HOutMidAddBarDiaArr.begin(), HOutMidAddBarDiaArr.end());
	}

	if (m_bHOutEndAddBar == TRUE)
	{
		vector<double> HOutEndAddBarDiaArr = m_HOutEndAddBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), HOutEndAddBarDiaArr.begin(), HOutEndAddBarDiaArr.end());
	}

	// 수평근(Interior)
	vector<double> HInTopBarDiaArr = m_HInTopBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), HInTopBarDiaArr.begin(), HInTopBarDiaArr.end());

	vector<double> HInMidBarDiaArr = m_HInMidBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), HInMidBarDiaArr.begin(), HInMidBarDiaArr.end());

	vector<double> HInBotBarDiaArr = m_HInBotBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), HInBotBarDiaArr.begin(), HInBotBarDiaArr.end());

	if (m_bHInStartAddBar == TRUE)
	{
		vector<double> HInStartAddBarDiaArr = m_HInStartAddBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), HInStartAddBarDiaArr.begin(), HInStartAddBarDiaArr.end());
	}

	if (m_bHInMidAddBar == TRUE)
	{
		vector<double> HInMidAddBarDiaArr = m_HInMidAddBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), HInMidAddBarDiaArr.begin(), HInMidAddBarDiaArr.end());
	}

	if (m_bHInEndAddBar == TRUE)
	{
		vector<double> HInEndAddBarDiaArr = m_HInEndAddBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), HInEndAddBarDiaArr.begin(), HInEndAddBarDiaArr.end());
	}

	// 수직근
	//////////////////////////////////////////////////////////////////////////
	//0 : 일반배근		 - 주근
	//1 : 상부보강		 - 주근 + 상부
	//2 : 하부보강		 - 주근 + 하부
	//3 : 상하부 보강 - 주근 + 상부 + 하부
	//////////////////////////////////////////////////////////////////////////

	// 수직근(Exterior)
	vector<double> VOutMidBarDiaArr = m_VOutMidBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), VOutMidBarDiaArr.begin(), VOutMidBarDiaArr.end());

	if(m_nExtVBarType == 1 || (m_nExtVBarType == 3 && m_bVOutTopAddBar == TRUE)) // 상부
	{
		vector<double> VOutTopBarDiaArr = m_VOutTopBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VOutTopBarDiaArr.begin(), VOutTopBarDiaArr.end());
	}

	if(m_nExtVBarType == 2 || (m_nExtVBarType == 3 && m_bVOutBotAddBar == TRUE)) // 하부
	{
		vector<double> VOutBotBarDiaArr = m_VOutBotBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VOutBotBarDiaArr.begin(), VOutBotBarDiaArr.end());
	}

	if (m_bVOutMidAddBar == TRUE) // 중앙
	{
		vector<double> VOutMidAddBarDiaArr = m_VOutMidAddBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VOutMidAddBarDiaArr.begin(), VOutMidAddBarDiaArr.end());
	}

	// 수직근(Interior)
	vector<double> VInMidBarDiaArr = m_VInMidBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), VInMidBarDiaArr.begin(), VInMidBarDiaArr.end());

	if (m_nVInBarType == 1 || (m_nVInBarType == 3 && m_bVInTopAddBar == TRUE)) // 상부
	{
		vector<double> VInTopBarDiaArr = m_VInTopBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VInTopBarDiaArr.begin(), VInTopBarDiaArr.end());
	}

	if (m_nVInBarType == 2 || (m_nVInBarType == 3 && m_bVInBotAddBar == TRUE)) // 하부
	{
		vector<double> VInBotBarDiaArr = m_VInBotBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VInBotBarDiaArr.begin(), VInBotBarDiaArr.end());
	}

	if (m_bVInMidAddBar == TRUE) // 중앙
	{
		vector<double> VInMidAddBarDiaArr = m_VInMidAddBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VInMidAddBarDiaArr.begin(), VInMidAddBarDiaArr.end());
	}

	//
	if(m_bUse2VMidBar) 
	{
		//2단배근 보강근 - 현재 사용안함
		//vector<long> 2VMidBarDiaArr = m_2VMidBar.GetBarDiaIdxAllLayer();
		//DiaIdxArr.insert(DiaIdxArr.end(), 2VMidBarDiaArr.begin(), 2VMidBarDiaArr.end());
	}

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const double& a, const double& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MSRcRWallM::GetVerBarDiaAllLayer()
{
	vector<double> DiaArr;

	// 수직근
	//////////////////////////////////////////////////////////////////////////
	//0 : 일반배근		 - 주근
	//1 : 상부보강		 - 주근 + 상부
	//2 : 하부보강		 - 주근 + 하부
	//3 : 상하부 보강 - 주근 + 상부 + 하부
	//////////////////////////////////////////////////////////////////////////

	// 수직근(Exterior)
	vector<double> VOutMidBarDiaArr = m_VOutMidBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), VOutMidBarDiaArr.begin(), VOutMidBarDiaArr.end());

	if (m_nExtVBarType == 1 || (m_nExtVBarType == 3 && m_bVOutTopAddBar == TRUE)) // 상부
	{
		vector<double> VOutTopBarDiaArr = m_VOutTopBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VOutTopBarDiaArr.begin(), VOutTopBarDiaArr.end());
	}

	if (m_nExtVBarType == 2 || (m_nExtVBarType == 3 && m_bVOutBotAddBar == TRUE)) // 하부
	{
		vector<double> VOutBotBarDiaArr = m_VOutBotBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VOutBotBarDiaArr.begin(), VOutBotBarDiaArr.end());
	}

	if (m_bVOutMidAddBar == TRUE) // 중앙
	{
		vector<double> VOutMidAddBarDiaArr = m_VOutMidAddBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VOutMidAddBarDiaArr.begin(), VOutMidAddBarDiaArr.end());
	}

	// 수직근(Interior)
	vector<double> VInMidBarDiaArr = m_VInMidBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), VInMidBarDiaArr.begin(), VInMidBarDiaArr.end());

	if (m_nVInBarType == 1 || (m_nVInBarType == 3 && m_bVInTopAddBar == TRUE)) // 상부
	{
		vector<double> VInTopBarDiaArr = m_VInTopBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VInTopBarDiaArr.begin(), VInTopBarDiaArr.end());
	}

	if (m_nVInBarType == 2 || (m_nVInBarType == 3 && m_bVInBotAddBar == TRUE)) // 하부
	{
		vector<double> VInBotBarDiaArr = m_VInBotBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VInBotBarDiaArr.begin(), VInBotBarDiaArr.end());
	}

	if (m_bVInMidAddBar == TRUE) // 중앙
	{
		vector<double> VInMidAddBarDiaArr = m_VInMidAddBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VInMidAddBarDiaArr.begin(), VInMidAddBarDiaArr.end());
	}

	//
	if (m_bUse2VMidBar)
	{
		//2단배근 보강근 - 현재 사용안함
		//vector<long> 2VMidBarDiaArr = m_2VMidBar.GetBarDiaIdxAllLayer();
		//DiaIdxArr.insert(DiaIdxArr.end(), 2VMidBarDiaArr.begin(), 2VMidBarDiaArr.end());
	}

	sort(DiaArr.begin(), DiaArr.end(),
		[](const double& a, const double& b)->bool {return a < b; }
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MSRcRWallM::GetHorBarDiaAllLayer()
{
	vector<double> DiaArr;

	// 수평근(Exterior)
	vector<double> HOutTopBarDiaArr = m_HOutTopBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), HOutTopBarDiaArr.begin(), HOutTopBarDiaArr.end());

	vector<double> HOutMidBarDiaArr = m_HOutMidBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), HOutMidBarDiaArr.begin(), HOutMidBarDiaArr.end());

	vector<double> HOutBotBarDiaArr = m_HOutBotBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), HOutBotBarDiaArr.begin(), HOutBotBarDiaArr.end());

	if (m_bHOutStartAddBar == TRUE)
	{
		vector<double> HOutStartAddBarDiaArr = m_HOutStartAddBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), HOutStartAddBarDiaArr.begin(), HOutStartAddBarDiaArr.end());
	}

	if (m_bHOutMidAddBar == TRUE)
	{
		vector<double> HOutMidAddBarDiaArr = m_HOutMidAddBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), HOutMidAddBarDiaArr.begin(), HOutMidAddBarDiaArr.end());
	}

	if (m_bHOutEndAddBar == TRUE)
	{
		vector<double> HOutEndAddBarDiaArr = m_HOutEndAddBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), HOutEndAddBarDiaArr.begin(), HOutEndAddBarDiaArr.end());
	}

	// 수평근(Interior)
	vector<double> HInTopBarDiaArr = m_HInTopBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), HInTopBarDiaArr.begin(), HInTopBarDiaArr.end());

	vector<double> HInMidBarDiaArr = m_HInMidBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), HInMidBarDiaArr.begin(), HInMidBarDiaArr.end());

	vector<double> HInBotBarDiaArr = m_HInBotBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), HInBotBarDiaArr.begin(), HInBotBarDiaArr.end());

	if (m_bHInStartAddBar == TRUE)
	{
		vector<double> HInStartAddBarDiaArr = m_HInStartAddBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), HInStartAddBarDiaArr.begin(), HInStartAddBarDiaArr.end());
	}

	if (m_bHInMidAddBar == TRUE)
	{
		vector<double> HInMidAddBarDiaArr = m_HInMidAddBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), HInMidAddBarDiaArr.begin(), HInMidAddBarDiaArr.end());
	}

	if (m_bHInEndAddBar == TRUE)
	{
		vector<double> HInEndAddBarDiaArr = m_HInEndAddBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), HInEndAddBarDiaArr.begin(), HInEndAddBarDiaArr.end());
	}

	sort(DiaArr.begin(), DiaArr.end(),
		[](const double& a, const double& b)->bool {return a < b; }
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MSRcRWallM::GetStirrupBarDiaAllLayer()
{
	vector<double> DiaArr;

	if (m_bAllShearAddBar)
	{
		vector<double> AllShearHBarDiaArr = m_AllShearHBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), AllShearHBarDiaArr.begin(), AllShearHBarDiaArr.end());

		vector<double> AllShearVBarDiaArr = m_AllShearVBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), AllShearVBarDiaArr.begin(), AllShearVBarDiaArr.end());
	}

	if(m_bUseTopStirrup || m_bUseAllStirrup )
	{
		vector<double> HTopStirrupDiaArr = m_HTopStirrup.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), HTopStirrupDiaArr.begin(), HTopStirrupDiaArr.end());

		vector<double> VTopStirrupDiaArr = m_VTopStirrup.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VTopStirrupDiaArr.begin(), VTopStirrupDiaArr.end());
	}

	if(m_bUseBotStirrup || m_bUseAllStirrup)
	{
		vector<double> HBotStirrupDiaArr = m_HBotStirrup.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), HBotStirrupDiaArr.begin(), HBotStirrupDiaArr.end());

		vector<double> VBotStirrupDiaArr = m_VBotStirrup.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VBotStirrupDiaArr.begin(), VBotStirrupDiaArr.end());
	}

	if(m_bStartShearAddBar)					//시작근 유무
	{
		vector<double> StartShearHBarDiaArr = m_StartShearHBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), StartShearHBarDiaArr.begin(), StartShearHBarDiaArr.end());

		vector<double> StartShearVBarDiaArr = m_StartShearVBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), StartShearVBarDiaArr.begin(), StartShearVBarDiaArr.end());
	}

	if(m_bEndShearAddBar)					//끝근 유무
	{
		vector<double> EndShearHBarDiaArr = m_EndShearHBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), EndShearHBarDiaArr.begin(), EndShearHBarDiaArr.end());

		vector<double> EndShearVBarDiaArr = m_EndShearVBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), EndShearVBarDiaArr.begin(), EndShearVBarDiaArr.end());
	}

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const double& a, const double& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}
double MSRcRWallM::GetMaxDia_HorInner()
{
	double dMaxDia = MSDeformedBar::GetMinDia();
	dMaxDia = max(dMaxDia, m_HInTopBar.GetDeformedBarDiameter(0));
	if (m_HInTopBar.GetNumLayers() >= 2)
		dMaxDia = max(dMaxDia, m_HInTopBar.GetDeformedBarDiameter(1));
	dMaxDia = max(dMaxDia, m_HInMidBar.GetDeformedBarDiameter(0));
	if (m_HInMidBar.GetNumLayers() >= 2)
		dMaxDia = max(dMaxDia, m_HInMidBar.GetDeformedBarDiameter(1));
	dMaxDia = max(dMaxDia, m_HInBotBar.GetDeformedBarDiameter(0));
	if (m_HInBotBar.GetNumLayers() >= 2)
		dMaxDia = max(dMaxDia, m_HInBotBar.GetDeformedBarDiameter(1));

	return dMaxDia;
}
double MSRcRWallM::GetMaxDia_HorOutter()
{
	double dMaxDia = MSDeformedBar::GetMinDia();
	dMaxDia = max(dMaxDia, m_HOutTopBar.GetDeformedBarDiameter(0));
	if (m_HOutTopBar.GetNumLayers() >= 2)
		dMaxDia = max(dMaxDia, m_HOutTopBar.GetDeformedBarDiameter(1));
	dMaxDia = max(dMaxDia, m_HOutMidBar.GetDeformedBarDiameter(0));
	if (m_HOutMidBar.GetNumLayers() >= 2)
		dMaxDia = max(dMaxDia, m_HOutMidBar.GetDeformedBarDiameter(1));
	dMaxDia = max(dMaxDia, m_HOutBotBar.GetDeformedBarDiameter(0));
	if (m_HOutBotBar.GetNumLayers() >= 2)
		dMaxDia = max(dMaxDia, m_HOutBotBar.GetDeformedBarDiameter(1));

	return dMaxDia;
}
double MSRcRWallM::GetMaxDia_VerInner()
{
	double dMaxDia = MSDeformedBar::GetMinDia();
	dMaxDia = max(dMaxDia, m_VInMidBar.GetDeformedBarDiameter(0));
	if (m_VInMidBar.GetNumLayers() >= 2)
		dMaxDia = max(dMaxDia, m_VInMidBar.GetDeformedBarDiameter(1));
	dMaxDia = max(dMaxDia, m_VInTopBar.GetDeformedBarDiameter(0));
	if (m_VInTopBar.GetNumLayers() >= 2)
		dMaxDia = max(dMaxDia, m_VInTopBar.GetDeformedBarDiameter(1));
	dMaxDia = max(dMaxDia, m_VInBotBar.GetDeformedBarDiameter(0));
	if (m_VInBotBar.GetNumLayers() >= 2)
		dMaxDia = max(dMaxDia, m_VInBotBar.GetDeformedBarDiameter(1));
	dMaxDia = max(dMaxDia, m_VInMidAddBar.GetDeformedBarDiameter(0));
	if(m_VInMidAddBar.GetNumLayers() >= 2)
		dMaxDia = max(dMaxDia, m_VInMidAddBar.GetDeformedBarDiameter(1));

	return dMaxDia;
}
double MSRcRWallM::GetMaxDia_VerOutter()
{
	double dMaxDia = MSDeformedBar::GetMinDia();
	dMaxDia = max(dMaxDia, m_VOutTopBar.GetDeformedBarDiameter(0));
	if (m_VOutTopBar.GetNumLayers() >= 2)
		dMaxDia = max(dMaxDia, m_VOutTopBar.GetDeformedBarDiameter(1));
	dMaxDia = max(dMaxDia, m_VOutMidBar.GetDeformedBarDiameter(0));
	if (m_VOutMidBar.GetNumLayers() >= 2)
		dMaxDia = max(dMaxDia, m_VOutMidBar.GetDeformedBarDiameter(1));
	dMaxDia = max(dMaxDia, m_VOutBotBar.GetDeformedBarDiameter(0));
	if (m_VOutBotBar.GetNumLayers() >= 2)
		dMaxDia = max(dMaxDia, m_VOutBotBar.GetDeformedBarDiameter(1));
	dMaxDia = max(dMaxDia, m_VOutMidAddBar.GetDeformedBarDiameter(0));
	if(m_VOutMidAddBar.GetNumLayers() >= 2)
		dMaxDia = max(dMaxDia, m_VOutMidAddBar.GetDeformedBarDiameter(1));

	return dMaxDia;
}
double MSRcRWallM::GetMaxDia()
{
	return max(GetMaxDia_VerOutter(), max(GetMaxDia_VerInner(), max(GetMaxDia_HorInner(), GetMaxDia_HorOutter())));
}
double MSRcRWallM::GetMaxVerBarDia()
{
	return max(GetMaxDia_VerInnerMainBar(), GetMinDia_VerOutterMainBar());
}
double MSRcRWallM::GetMinVerBarDia()
{
	return min(GetMinDia_VerInnerMainBar(), GetMinDia_VerOutterMainBar());
}
double MSRcRWallM::GetMinDia_VerInnerMainBar()
{
	double dMinDia = MSDeformedBar::GetMaxDia();
	dMinDia = min(dMinDia, m_VInMidBar.GetDeformedBarDiameter(0));
	if (m_VInMidBar.GetNumLayers() >= 2)
		dMinDia = min(dMinDia, m_VInMidBar.GetDeformedBarDiameter(1));

	return dMinDia;
}
double MSRcRWallM::GetMinDia_VerOutterMainBar()
{
	double dMinDia = MSDeformedBar::GetMaxDia();
	dMinDia = min(dMinDia, m_VOutMidBar.GetDeformedBarDiameter(0));
	if (m_VOutMidBar.GetNumLayers() >= 2)
		dMinDia = min(dMinDia, m_VOutMidBar.GetDeformedBarDiameter(1));

	return dMinDia;
}
double MSRcRWallM::GetMaxDia_VerInnerMainBar()
{
	double dMaxDia = MSDeformedBar::GetMinDia();
	dMaxDia = max(dMaxDia, m_VInMidBar.GetDeformedBarDiameter(0));
	if (m_VInMidBar.GetNumLayers() >= 2)
		dMaxDia = max(dMaxDia, m_VInMidBar.GetDeformedBarDiameter(1));

	return dMaxDia;
}
double MSRcRWallM::GetMaxDia_VerOutterMainBar()
{
	double dMaxDia = MSDeformedBar::GetMinDia();
	dMaxDia = max(dMaxDia, m_VOutMidBar.GetDeformedBarDiameter(0));
	if (m_VOutMidBar.GetNumLayers() >= 2)
		dMaxDia = max(dMaxDia, m_VOutMidBar.GetDeformedBarDiameter(1));

	return dMaxDia;
}
double MSRcRWallM::GetSpacing_VerInnerMainBar()
{
	return m_VInMidBar.GetSpacing();
}
double MSRcRWallM::GetSpacing_VerOutterMainBar()
{
	return m_VOutMidBar.GetSpacing();
}
bool MSRcRWallM::GetVerBarUserSteelGradeS()
{
	return m_VOutMidBar.GetUserSteelGradeS() || m_VInMidBar.GetUserSteelGradeS();
}
double MSRcRWallM::GetMaxHorBarSpacing()
{
	double dMaxSpacing = m_HInMidBar.GetSpacing();
	dMaxSpacing = max(dMaxSpacing, m_HOutMidBar.GetSpacing());
	return dMaxSpacing;
}
double MSRcRWallM::GetMaxVerBarSpacing()
{
	double dMaxSpacing = m_VInMidBar.GetSpacing();
	dMaxSpacing = max(dMaxSpacing, m_VOutMidBar.GetSpacing());
	return dMaxSpacing;
}