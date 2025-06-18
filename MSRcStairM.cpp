#include "StdAfx.h"
#include "MSRcStairM.h"


IMPLEMENT_SERIAL(MSRcStairM, MDMember, VERSIONABLE_SCHEMA | 1)
MSRcStairM::MSRcStairM(void)
{
    m_nStairType = msLanding;

	m_bOneWay	= FALSE;
	m_bStairAllSame = FALSE;
	m_dLandingThick = 150.0;			
	m_dStairThick = 150.0;				
	m_dTopLandingLength = 1200.0;	
	m_dBotLandingLength = 1200.0;	
	m_dRightWallThick = 200.0;		
	m_dLeftWallThick = 200.0;	
	m_dCvrConc = 20.0;
	m_nWallCvrConc = 20;			
	m_nStairNum = 10;					

	CString sStirrupBar = MSDeformedBar::GetStirrupBarStr(200, 13);
	CString sAddBar = MSDeformedBar::GetMainBarStr(8, 13);

	m_SXTopEndBar.InitData(sStirrupBar);
	m_SXBotEndBar.InitData(sStirrupBar);
	m_SYTopEndBar.InitData(sStirrupBar);
	m_SYBotEndBar.InitData(sStirrupBar);
	m_SXTopCenBar.InitData(sStirrupBar);
	m_SXBotCenBar.InitData(sStirrupBar);
	m_SYTopCenBar.InitData(sStirrupBar);
	m_SYBotCenBar.InitData(sStirrupBar);

	m_LXTopBar.InitData(sStirrupBar);
	m_LYTopBar.InitData(sStirrupBar);
	m_LXBotBar.InitData(sStirrupBar);
	m_LYBotBar.InitData(sStirrupBar);

	m_TopAddBar.InitData(sAddBar);	
	m_BotAddBar.InitData(sAddBar);	
	m_dRailWidth = 100;
}

MSRcStairM::~MSRcStairM(void)
{
}

void MSRcStairM::CopyFromMe(MDMember* pSource)
{
	if(pSource==NULL)	return; 
	MSRcStairM* pSourceStairM = (MSRcStairM*)pSource;
    m_nStairType = pSourceStairM->m_nStairType;

	m_bOneWay	= pSourceStairM->m_bOneWay;				
	m_dLandingThick = pSourceStairM->m_dLandingThick;			
	m_dStairThick = pSourceStairM->m_dStairThick;				
	m_dTopLandingLength = pSourceStairM->m_dTopLandingLength;	
	m_dBotLandingLength = pSourceStairM->m_dBotLandingLength;
	m_dRightWallThick = pSourceStairM->m_dRightWallThick;
	m_dLeftWallThick = pSourceStairM->m_dLeftWallThick;
	m_dCvrConc = pSourceStairM->m_dCvrConc;
	m_nWallCvrConc = pSourceStairM->m_nWallCvrConc;
	m_nStairNum = pSourceStairM->m_nStairNum;
	m_bStairAllSame = pSourceStairM->m_bStairAllSame;

	m_SXTopEndBar.CopyFromMe(&pSourceStairM->m_SXTopEndBar);
	m_SXBotEndBar.CopyFromMe(&pSourceStairM->m_SXBotEndBar);
	m_SYTopEndBar.CopyFromMe(&pSourceStairM->m_SYTopEndBar);
	m_SYBotEndBar.CopyFromMe(&pSourceStairM->m_SYBotEndBar);

	m_SXTopCenBar.CopyFromMe(&pSourceStairM->m_SXTopCenBar);
	m_SXBotCenBar.CopyFromMe(&pSourceStairM->m_SXBotCenBar);
	m_SYTopCenBar.CopyFromMe(&pSourceStairM->m_SYTopCenBar);
	m_SYBotCenBar.CopyFromMe(&pSourceStairM->m_SYBotCenBar);

	m_LYTopBar.CopyFromMe(&pSourceStairM->m_LYTopBar);
	m_LYBotBar.CopyFromMe(&pSourceStairM->m_LYBotBar);
	m_LXTopBar.CopyFromMe(&pSourceStairM->m_LXTopBar);
	m_LXBotBar.CopyFromMe(&pSourceStairM->m_LXBotBar);

	m_TopAddBar.CopyFromMe(&pSourceStairM->m_TopAddBar);
	m_BotAddBar.CopyFromMe(&pSourceStairM->m_BotAddBar);

	MDMember::CopyFromMe(pSource);
}

void MSRcStairM::Dump(EFS::EFS_Buffer& buffer)
{
	MDMember::Dump(buffer);

    int stairType = (int)m_nStairType;
    buffer << stairType;

	buffer << m_bOneWay << m_dLandingThick << m_dStairThick << m_dTopLandingLength << m_dBotLandingLength;
	buffer << m_dRightWallThick << m_dLeftWallThick << m_nWallCvrConc << m_nStairNum << m_bStairAllSame << m_dRailWidth;
	m_SXTopEndBar.Dump(buffer);
	m_SYTopEndBar.Dump(buffer);
	m_SXTopCenBar.Dump(buffer);
	m_SYTopCenBar.Dump(buffer);
	m_LYTopBar.Dump(buffer);
	m_LYBotBar.Dump(buffer);
	m_LXTopBar.Dump(buffer);
	m_LXBotBar.Dump(buffer);;
	m_TopAddBar.Dump(buffer);
	m_BotAddBar.Dump(buffer);
	m_SXBotCenBar.Dump(buffer);
	m_SYBotCenBar.Dump(buffer);
	m_SXTopCenBar.Dump(buffer);
	m_SYTopCenBar.Dump(buffer);
}

void MSRcStairM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDMember::Recovery(buffer);

    int stairType = 0;
    buffer >> stairType;
    m_nStairType = (Type_Stair)stairType;

	buffer >> m_bOneWay >> m_dLandingThick >> m_dStairThick >> m_dTopLandingLength >> m_dBotLandingLength;
	buffer >> m_dRightWallThick >> m_dLeftWallThick >> m_nWallCvrConc >> m_nStairNum >> m_bStairAllSame >> m_dRailWidth;
	m_SXTopEndBar.Recovery(buffer);
	m_SYTopEndBar.Recovery(buffer);
	m_SXTopCenBar.Recovery(buffer);
	m_SYTopCenBar.Recovery(buffer);
	m_LYTopBar.Recovery(buffer);
	m_LYBotBar.Recovery(buffer);
	m_LXTopBar.Recovery(buffer);
	m_LXBotBar.Recovery(buffer);;
	m_TopAddBar.Recovery(buffer);
	m_BotAddBar.Recovery(buffer);
	m_SXBotCenBar.Recovery(buffer);
	m_SYBotCenBar.Recovery(buffer);
	m_SXTopCenBar.Recovery(buffer);
	m_SYTopCenBar.Recovery(buffer);
}

void MSRcStairM::Serialize(CArchive &ar)
{
	MDMember::Serialize(ar);
    int stairType = 0;
	if(ar.IsStoring())
	{
        stairType = (int)m_nStairType;
        ar << stairType;

		ar << m_bOneWay;				
		ar << m_dLandingThick;			
		ar << m_dStairThick;				
		ar << m_dTopLandingLength;	
		ar << m_dBotLandingLength;	
		ar << m_dRightWallThick;		
		ar << m_dLeftWallThick;	
		ar << m_dCvrConc;
		ar << m_nWallCvrConc;			
		ar <<	m_nStairNum;					
		ar << m_bStairAllSame;

		m_SXTopEndBar.Serialize(ar);
		m_SYTopEndBar.Serialize(ar);
		m_SXTopCenBar.Serialize(ar);
		m_SYTopCenBar.Serialize(ar);

		m_LYTopBar.Serialize(ar);
		m_LYBotBar.Serialize(ar);
		m_LXTopBar.Serialize(ar);
		m_LXBotBar.Serialize(ar);

		m_TopAddBar.Serialize(ar);
		m_BotAddBar.Serialize(ar);

		m_SXBotCenBar.Serialize(ar);
		m_SYBotCenBar.Serialize(ar);
		m_SXTopCenBar.Serialize(ar);
		m_SYTopCenBar.Serialize(ar);

		ar << m_dRailWidth;
	}
	else
	{
        ar >> stairType;
        m_nStairType = (Type_Stair)stairType;
		ar >> m_bOneWay;				
		ar >> m_dLandingThick;			
		ar >> m_dStairThick;				
		ar >> m_dTopLandingLength;	
		ar >> m_dBotLandingLength;	
		ar >> m_dRightWallThick;		
		ar >> m_dLeftWallThick;	
		ar >> m_dCvrConc;
		ar >> m_nWallCvrConc;			
		ar >>	m_nStairNum;	
		ar >> m_bStairAllSame;

		m_SXTopEndBar.Serialize(ar);
		m_SYTopEndBar.Serialize(ar);
		m_SXTopCenBar.Serialize(ar);
		m_SYTopCenBar.Serialize(ar);

		m_LYTopBar.Serialize(ar);
		m_LYBotBar.Serialize(ar);
		m_LXTopBar.Serialize(ar);
		m_LXBotBar.Serialize(ar);

		m_TopAddBar.Serialize(ar);
		m_BotAddBar.Serialize(ar);

		m_SXBotCenBar.Serialize(ar);
		m_SYBotCenBar.Serialize(ar);
		m_SXTopCenBar.Serialize(ar);
		m_SYTopCenBar.Serialize(ar);

		ar >> m_dRailWidth;
	}
}