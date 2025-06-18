#include "StdAfx.h"
#include "MSRcSlabM.h"
#include "GMLib/MSVersionInfo.h"
#include "Radfcore_CLI/AppGlobalOptionWrapper.h"
#include "MSObject.h"

IMPLEMENT_SERIAL(MSRcSlabM, MDMember, VERSIONABLE_SCHEMA | 1)
MSRcSlabM::MSRcSlabM(void)
{
	m_nSlabMType = GENERAL_TYPE;
	m_bIsMat = FALSE;
	m_IsSpacer = TRUE;
	m_BotSpacerType = 0;
	m_dCvrConc = 20.0;

	m_IsShort1 = true;
	m_IsShort2 = true;
	m_IsShort3 = true;
	m_IsShort4	= true;
	m_IsShort5 = true;
	m_IsShort6 = true;
	m_IsShort7 = true;
	m_IsShort8 = true;
	m_IsShort9 = true;
	m_IsShort10 = true;

	m_IsLong1 = true;
	m_IsLong2 = true;
	m_IsLong3 = true;
	m_IsLong4 = true;
	m_IsLong5 = true;
	m_IsLong6 = true;
	m_IsLong7 = true;
	m_IsLong8 = true;
	m_IsLong9 = true;
	m_IsLong10 = true;
			
	CString sBar = 	MSDeformedBar::GetStirrupBarStr(1000);
	m_SpacerBars.InitData(sBar);
 	m_pTopSpacerShape = new SpacerShape();
 
 	m_pBotSpacerShape = new SpacerShape();
	m_IsUsedColumnStripLxQuarter = false;

	m_IsNumPlaceDeckBar2 = true;	
	if (MSDeformedBar::GetRebarCode() == MSDeformedBar::REBAR_MATERIAL_KS)
	{
		m_DeckBar2.InitData(MSDeformedBar::GetMainBarStr(4, 13));
		m_DeckBar3.InitData(MSDeformedBar::GetMainBarStr(1, 16));
		m_DeckBar4.InitData(MSDeformedBar::GetMainBarStr(1, 16));
		m_DeckStirrupBar2.InitData(MSDeformedBar::GetStirrupBarStr(300));
	}
	else
	{
		m_DeckBar2.InitData(MSDeformedBar::GetMainBarStr(4));	
		m_DeckBar3.InitData(MSDeformedBar::GetMainBarStr(1));
		m_DeckBar4.InitData(MSDeformedBar::GetMainBarStr(1));
		m_DeckStirrupBar2.InitData(MSDeformedBar::GetStirrupBarStr(300));
	}
	
	CheckShortnLongBarFlag();
}

MSRcSlabM::~MSRcSlabM(void)
{
	if(m_pTopSpacerShape != NULL) delete m_pTopSpacerShape;
	if(m_pBotSpacerShape	!= NULL) delete m_pBotSpacerShape;
}


void MSRcSlabM::CopyFromMe(MDMember* pSource)
{
	if(pSource==NULL)	return;
	MSRcSlabM* pSourceSlabM = (MSRcSlabM*)pSource;
	m_bIsMat = pSourceSlabM->m_bIsMat;
	m_nSlabMType = pSourceSlabM->m_nSlabMType;
	m_IsSpacer = pSourceSlabM->m_IsSpacer;
	m_BotSpacerType = pSourceSlabM->m_BotSpacerType;

	m_ShortStrip.CopyFromMe(&pSourceSlabM->m_ShortStrip);
	m_LongStrip.CopyFromMe(&pSourceSlabM->m_LongStrip);
	m_SpacerBars.CopyFromMe(&pSourceSlabM->m_SpacerBars);

	m_IsShort1 = pSourceSlabM->m_IsShort1;
	m_IsShort2	= pSourceSlabM->m_IsShort2;
	m_IsShort3 = pSourceSlabM->m_IsShort3;
	m_IsShort4	= pSourceSlabM->m_IsShort4;
	m_IsShort5 = pSourceSlabM->m_IsShort5;
	m_IsShort6	= pSourceSlabM->m_IsShort6;
	m_IsShort7 = pSourceSlabM->m_IsShort7;
	m_IsShort8 = pSourceSlabM->m_IsShort8;
	m_IsShort9 = pSourceSlabM->m_IsShort9;
	m_IsShort10 = pSourceSlabM->m_IsShort10;

	m_IsLong1 = pSourceSlabM->m_IsLong1;
	m_IsLong2	= pSourceSlabM->m_IsLong2;
	m_IsLong3 = pSourceSlabM->m_IsLong3;
	m_IsLong4	= pSourceSlabM->m_IsLong4;
	m_IsLong5 = pSourceSlabM->m_IsLong5;
	m_IsLong6	= pSourceSlabM->m_IsLong6;
	m_IsLong7	= pSourceSlabM->m_IsLong7;
	m_IsLong8	= pSourceSlabM->m_IsLong8;
	m_IsLong9	= pSourceSlabM->m_IsLong9;
	m_IsLong10	= pSourceSlabM->m_IsLong10;

	m_IsUsedColumnStripLxQuarter = pSourceSlabM->m_IsUsedColumnStripLxQuarter;

	m_DeckBar2.CopyFromMe(&pSourceSlabM->m_DeckBar2);
	m_DeckBar3.CopyFromMe(&pSourceSlabM->m_DeckBar3);
	m_DeckBar4.CopyFromMe(&pSourceSlabM->m_DeckBar4);

	m_IsNumPlaceDeckBar2 = pSourceSlabM->m_IsNumPlaceDeckBar2;
	m_DeckStirrupBar2.CopyFromMe(&pSourceSlabM->m_DeckStirrupBar2);

	MDMember::CopyFromMe(pSource);
}

void MSRcSlabM::Dump(EFS::EFS_Buffer& buffer)
{
	MDMember::Dump(buffer);
	
	buffer << m_bIsMat << m_nSlabMType << m_IsSpacer << m_BotSpacerType;
// m_pTopSpacerShape과 m_pBotSpacerShape 사용 안함 -> G로 이동
//	buffer << &m_pTopSpacerShape << &m_pBotSpacerShape;
	m_ShortStrip.Dump(buffer);
	m_LongStrip.Dump(buffer);
	m_SpacerBars.Dump(buffer);

	buffer << m_IsShort1 << m_IsShort2 << m_IsShort3 << m_IsShort4 << m_IsShort5 << m_IsShort6 << m_IsShort7 << m_IsShort8 << m_IsShort9 << m_IsShort9;
	buffer << m_IsLong1 << m_IsLong2 << m_IsLong3 << m_IsLong4 << m_IsLong5 << m_IsLong6 << m_IsLong7 << m_IsLong8 << m_IsLong9 << m_IsLong10;
	buffer << m_IsUsedColumnStripLxQuarter;

	m_DeckBar2.Dump(buffer);
	m_DeckBar3.Dump(buffer);
	m_DeckBar4.Dump(buffer);

	buffer << m_IsNumPlaceDeckBar2;
	m_DeckStirrupBar2.Dump(buffer);
}

void MSRcSlabM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDMember::Recovery(buffer);

	buffer >> m_bIsMat >> m_nSlabMType >> m_IsSpacer >> m_BotSpacerType;
// m_pTopSpacerShape과 m_pBotSpacerShape 사용 안함 -> G로 이동
//	buffer >> &m_pTopSpacerShape >> &m_pBotSpacerShape;
	m_ShortStrip.Recovery(buffer);
	m_LongStrip.Recovery(buffer);
	m_SpacerBars.Recovery(buffer);

	buffer >> m_IsShort1 >> m_IsShort2 >> m_IsShort3 >> m_IsShort4 >> m_IsShort5 >> m_IsShort6 >> m_IsShort7 >> m_IsShort8 >> m_IsShort9 >> m_IsShort10;
	buffer >> m_IsLong1 >> m_IsLong2 >> m_IsLong3 >> m_IsLong4 >> m_IsLong5 >> m_IsLong6 >> m_IsLong7 >> m_IsLong8 >> m_IsLong9 >> m_IsLong10;
	buffer >> m_IsUsedColumnStripLxQuarter;

	m_DeckBar2.Recovery(buffer);
	m_DeckBar3.Recovery(buffer);
	m_DeckBar4.Recovery(buffer);

	buffer >> m_IsNumPlaceDeckBar2;
	m_DeckStirrupBar2.Recovery(buffer);
}

void MSRcSlabM::Serialize(CArchive &ar)
{	
	MDMember::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_bIsMat;
		ar << m_nSlabMType;
		ar << m_IsSpacer;
		ar << m_BotSpacerType;
		m_ShortStrip.Serialize(ar);
		m_LongStrip.Serialize(ar);

		m_SpacerBars.Serialize(ar);
		m_pTopSpacerShape->Serialize(ar);
		m_pBotSpacerShape->Serialize(ar);

		ar << m_IsShort2;
		ar << m_IsShort4;
		ar << m_IsLong3;
		ar << m_IsLong4;
		ar << m_IsLong5;
		ar << m_IsLong6;

		ar << m_IsShort1;
		ar << m_IsShort3;
		ar << m_IsLong1;
		ar << m_IsLong2;

		ar << m_IsShort5;
		ar << m_IsShort6;

		ar << m_IsUsedColumnStripLxQuarter;

		ar << m_IsShort7;
		ar << m_IsShort8;
		ar << m_IsShort9;
		ar << m_IsShort10;

		ar << m_IsLong7;
		ar << m_IsLong8;
		ar << m_IsLong9;
		ar << m_IsLong10;

		m_DeckBar2.Serialize(ar);
		m_DeckBar3.Serialize(ar);
		m_DeckBar4.Serialize(ar);

		ar << m_IsNumPlaceDeckBar2;
		m_DeckStirrupBar2.Serialize(ar);
	}
	else
	{
		ar >> m_bIsMat;
		ar >> m_nSlabMType;
		ar >> m_IsSpacer;
		ar >> m_BotSpacerType;
		m_ShortStrip.Serialize(ar);
		m_LongStrip.Serialize(ar);

		m_SpacerBars.Serialize(ar);
		m_pTopSpacerShape->Serialize(ar);
		m_pBotSpacerShape->Serialize(ar);

		if(MSVersionInfo::GetCurrentVersion() >= 20140801)
		{
			ar >> m_IsShort2;
			ar >> m_IsShort4;
			ar >> m_IsLong3;
			ar >> m_IsLong4;
			ar >> m_IsLong5;
			ar >> m_IsLong6;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20140930)
		{
			ar >> m_IsShort1;
			ar >> m_IsShort3;
			ar >> m_IsLong1;
			ar >> m_IsLong2;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20141209)
		{
			ar >> m_IsShort5;
			ar >> m_IsShort6;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20150422)
		{
			ar >> m_IsUsedColumnStripLxQuarter;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20150501)
		{
			ar >> m_IsShort7;
			ar >> m_IsShort8;
			ar >> m_IsShort9;
			ar >> m_IsShort10;

			ar >> m_IsLong7;
			ar >> m_IsLong8;
			ar >> m_IsLong9;
			ar >> m_IsLong10;
		}

		if (MSVersionInfo::GetCurrentVersion() < 20150521)
		{
			if (m_nSlabMType != GENERAL_TYPE) return;

			if (m_IsShort3 && (m_ShortStrip.m_StripType == MSRcStrip::TYPE_C || m_ShortStrip.m_StripType == MSRcStrip::TYPE_D || m_ShortStrip.m_StripType == MSRcStrip::TYPE_I))
			{
				m_ShortStrip.m_Bar7.CopyFromMe(&m_ShortStrip.m_Bar3);
				m_IsShort7 = true;
				m_IsShort3 = false;
			}

			if (m_IsLong3 && (m_LongStrip.m_StripType == MSRcStrip::TYPE_C || m_LongStrip.m_StripType == MSRcStrip::TYPE_D || m_LongStrip.m_StripType == MSRcStrip::TYPE_I))
			{
				m_LongStrip.m_Bar7.CopyFromMe(&m_LongStrip.m_Bar3);
				m_IsLong7 = true;
				m_IsLong3 = false;
			}
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20160428)
		{
			m_DeckBar2.Serialize(ar);
			m_DeckBar3.Serialize(ar);
			m_DeckBar4.Serialize(ar);
		}
		if (MSVersionInfo::GetCurrentVersion() < 20151028)
			CheckShortnLongBarFlag();

		if (m_nSlabMType == MAT_TYPE || m_nSlabMType == FLAT_TYPE)
		{
			if (m_ShortStrip.m_StripType == MSRcStrip::TYPE_NONE)
				m_ShortStrip.m_StripType = MSRcStrip::TYPE_ALL;

			if (m_LongStrip.m_StripType == MSRcStrip::TYPE_NONE)
				m_LongStrip.m_StripType = MSRcStrip::TYPE_ALL;
		}

		if (m_nSlabMType == DECK_TYPE)
		{
			m_IsLong5 = false;
			m_IsLong6 = false;
		}

		// 20190822에 변수가 추가 되었다가 20221011에 변수가 제거됨
		// m_PastDataL1 ~ m_PastDataL4 정보는 MSRcFaceMemberM으로 이동하였습니다.
		//if(MSVersionInfo::GetCurrentVersion() >= 20190822 && MSVersionInfo::GetCurrentVersion() < 20221011)
		if ((MSObject::IsBHQorBHCEMode() && (MSVersionInfo::GetCurrentVersion() >= 20190822 && MSVersionInfo::GetCurrentVersion() < 20221011))
			|| (MSObject::IsBHForBHAMode() && (MSVersionInfo::GetCurrentVersion() >= 20220825 && MSVersionInfo::GetCurrentVersion() < 20240307)))
		{
			ar >> m_PastDataL1;
			ar >> m_PastDataL2;
			ar >> m_PastDataL3;
			ar >> m_PastDataH1;
			ar >> m_PastDataH2;
			ar >> m_PastDataH3;
		}
		if (MSVersionInfo::GetCurrentVersion() < 20180828)
			ShortnLongBarNotUsedFlagFalseSetting();

		//if(MSVersionInfo::GetCurrentVersion() >= 20231026 && MSVersionInfo::IsOldTotalVersion() == false)
		if ((MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231026))
		{
			ar >> m_IsNumPlaceDeckBar2;
			m_DeckStirrupBar2.Serialize(ar);
		}
	}
}

void MSRcSlabM::ShortnLongBarNotUsedFlagFalseSetting()
{
	if (m_nSlabMType == MAT_TYPE)
	{
		m_IsShort2 = false;	m_IsShort3 = false;	m_IsShort5 = false;	m_IsShort6 = false;
		m_IsShort7 = false;	m_IsShort8 = false;	m_IsShort9 = false;	m_IsShort10 = false;

		m_IsLong2 = false;	m_IsLong3 = false;	m_IsLong5 = false;	m_IsLong6 = false;
		m_IsLong7 = false;	m_IsLong8 = false;	m_IsLong9 = false;	m_IsLong10 = false;
	}
	else if (m_nSlabMType == FLAT_TYPE)
	{
		if (m_ShortStrip.m_StripType == MSRcStrip::TYPE_B || m_LongStrip.m_StripType == MSRcStrip::TYPE_B)
			return;

		m_IsShort2 = false;	m_IsShort3 = false;	m_IsShort5 = false;	m_IsShort6 = false;
		m_IsShort7 = false;	m_IsShort8 = false;	m_IsShort9 = false;	m_IsShort10 = false;

		m_IsLong2 = false;	m_IsLong3 = false;	m_IsLong5 = false;	m_IsLong6 = false;
		m_IsLong7 = false;	m_IsLong8 = false;	m_IsLong9 = false;	m_IsLong10 = false;
	}
	else if (m_nSlabMType == GENERAL_TYPE)
	{
		if (m_ShortStrip.m_StripType != MSRcStrip::TYPE_ALL)
		{	
			m_IsShort8 = false;	m_IsShort9 = false;	m_IsShort10 = false;	
		}

		if (m_ShortStrip.m_StripType == MSRcStrip::TYPE_A)
		{	
			m_IsShort2 = false; m_IsShort3 = false;	m_IsShort7 = false;		
		}
		else if (m_ShortStrip.m_StripType == MSRcStrip::TYPE_B)
		{
			m_IsShort1 = false;	m_IsShort3 = false;	m_IsShort7 = false;
		}
		else if (m_ShortStrip.m_StripType == MSRcStrip::TYPE_C)
		{
			m_IsShort1 = false;	m_IsShort3 = false;
		}
		else if (m_ShortStrip.m_StripType == MSRcStrip::TYPE_D)
		{
			m_IsShort3 = false;
		}
		else if (m_ShortStrip.m_StripType == MSRcStrip::TYPE_E)
		{
			m_IsShort3 = false;	m_IsShort7 = false;
		}
		else if (m_ShortStrip.m_StripType == MSRcStrip::TYPE_F)
		{
			m_IsShort1 = false;	m_IsShort2 = false;	m_IsShort7 = false;
		}
		else if (m_ShortStrip.m_StripType == MSRcStrip::TYPE_G)
		{
			m_IsShort2 = false;	m_IsShort7 = false;
		}
		else if (m_ShortStrip.m_StripType == MSRcStrip::TYPE_H)
		{
			m_IsShort1 = false;	m_IsShort7 = false;
		}
		else if (m_ShortStrip.m_StripType == MSRcStrip::TYPE_I)
		{
			m_IsShort2 = false;	m_IsShort3 = false;
		}

		if (m_LongStrip.m_StripType != MSRcStrip::TYPE_ALL)
		{
			m_IsLong8 = false;	m_IsLong9 = false;	m_IsLong10 = false;
		}

		if (m_LongStrip.m_StripType == MSRcStrip::TYPE_A)
		{
			m_IsLong2 = false; m_IsLong3 = false;	m_IsLong7 = false;
		}
		else if (m_LongStrip.m_StripType == MSRcStrip::TYPE_B)
		{
			m_IsLong1 = false;	m_IsLong3 = false;	m_IsLong7 = false;
		}
		else if (m_LongStrip.m_StripType == MSRcStrip::TYPE_C)
		{
			m_IsLong1 = false;	m_IsLong3 = false;
		}
		else if (m_LongStrip.m_StripType == MSRcStrip::TYPE_D)
		{
			m_IsLong3 = false;
		}
		else if (m_LongStrip.m_StripType == MSRcStrip::TYPE_E)
		{
			m_IsLong3 = false;	m_IsLong7 = false;
		}
		else if (m_LongStrip.m_StripType == MSRcStrip::TYPE_F)
		{
			m_IsLong1 = false;	m_IsLong2 = false;	m_IsLong7 = false;
		}
		else if (m_LongStrip.m_StripType == MSRcStrip::TYPE_G)
		{
			m_IsLong2 = false;	m_IsLong7 = false;
		}
		else if (m_LongStrip.m_StripType == MSRcStrip::TYPE_H)
		{
			m_IsLong1 = false;	m_IsLong7 = false;
		}
		else if (m_LongStrip.m_StripType == MSRcStrip::TYPE_I)
		{
			m_IsLong2 = false;	m_IsLong3 = false;
		}
	}
	else if (m_nSlabMType == SOG || m_nSlabMType == CIRCULAR_RAMP)
	{
		m_IsShort2 = false;	m_IsShort3 = false;	m_IsShort5 = false;	m_IsShort6 = false;
		m_IsShort7 = false;	m_IsShort8 = false;	m_IsShort9 = false;	m_IsShort10 = false;

		if (m_ShortStrip.m_StripType == MSRcStrip::TYPE_NONE)
		{
			m_IsShort1 = false;	m_IsShort4 = false;
		}

		m_IsLong2 = false;	m_IsLong3 = false;	m_IsLong5 = false;	m_IsLong6 = false;
		m_IsLong7 = false;	m_IsLong8 = false;	m_IsLong9 = false;	m_IsLong10 = false;

		if (m_LongStrip.m_StripType == MSRcStrip::TYPE_NONE)
		{
			m_IsLong1 = false;	m_IsLong4 = false;
		}
	}
	else if (m_nSlabMType == PC_TYPE)
	{
		m_IsShort6 = false;	m_IsShort7 = false;	m_IsShort8 = false;	m_IsShort9 = false;	m_IsShort10 = false;

		m_IsLong6 = false;	m_IsLong7 = false;	m_IsLong8 = false;	m_IsLong9 = false;	m_IsLong10 = false;
	}
	else if (m_nSlabMType == DECK_TYPE)
	{
		m_IsShort5 = false;	m_IsShort6 = false;	m_IsShort7 = false;	
		m_IsShort8 = false;	m_IsShort9 = false;	m_IsShort10 = false;

		m_IsLong5 = false;	m_IsLong6 = false;	m_IsLong7 = false;
		m_IsLong8 = false;	m_IsLong9 = false;	m_IsLong10 = false;
	}
	else if (m_nSlabMType == DECK_GENERAL)
	{
		m_IsShort7 = false;	m_IsShort8 = false;	m_IsShort9 = false;	m_IsShort10 = false;

		m_IsLong3 = false;	m_IsLong4 = false; m_IsLong5 = false; m_IsLong6 = false; 
		m_IsLong7 = false;	m_IsLong8 = false;	m_IsLong9 = false;	m_IsLong10 = false;
	}
	else if (m_nSlabMType == PC_IMT)
	{
		m_IsShort7 = m_IsShort8 = m_IsShort9 = m_IsShort10 = false;
		m_IsLong4 = m_IsLong5 = m_IsLong6 = m_IsLong7 = m_IsLong8 = m_IsLong9 = m_IsLong10 = false;
	}
	else if(m_nSlabMType == PC_RPS)
	{
		m_IsShort4 = m_IsShort5 = m_IsShort6 = m_IsShort7 = m_IsShort8 = m_IsShort9 = m_IsShort10 = false;	
		m_IsLong4 = m_IsLong5 = m_IsLong6 = m_IsLong7 = m_IsLong8 = m_IsLong9 = m_IsLong10 = false;
	}
	else if (m_nSlabMType == PC_HCS)
	{
		m_IsShort6 = m_IsShort7 = m_IsShort8 = m_IsShort9 = m_IsShort10 = false;
		m_IsLong4 = m_IsLong5 = m_IsLong6 = m_IsLong7 = m_IsLong8 = m_IsLong9 = m_IsLong10 = false;
	}
	else
		ASSERT(0);
}

long MSRcSlabM::GetSlabMType()
{
	return m_nSlabMType;
}

BOOL MSRcSlabM::IsPC()
{
	// 테크는 PC가 아니다
	if (m_nSlabMType == PC_TYPE)	return TRUE;
	if (m_nSlabMType == PC_IMT || m_nSlabMType == PC_RPS || m_nSlabMType == PC_HCS )	return TRUE;
	return FALSE;
}

BOOL MSRcSlabM::IsHalfPCSlab()
{
	if (m_nSlabMType == PC_TYPE)
		return TRUE;
	return FALSE;
}

BOOL MSRcSlabM::IsIMTPCSlab()
{
	if (m_nSlabMType == PC_IMT)
		return TRUE;
	return FALSE;
}
BOOL MSRcSlabM::IsHCSPCSlab()
{
	if (m_nSlabMType == PC_HCS)
		return TRUE;
	return FALSE;
}
BOOL MSRcSlabM::IsRPSPCSlab()
{
	if (m_nSlabMType == PC_RPS)
		return TRUE;
	return FALSE;
}

bool MSRcSlabM::IsTransfer()
{
	if (m_nSlabMType == TRANSFER)
		return TRUE;
	return FALSE;
}

BOOL MSRcSlabM::IsGeneralSlab()
{
	if (IsDeckSlab() == false && IsPCSlab() == false) return TRUE;
	return FALSE;
}

BOOL MSRcSlabM::IsDeckSlab()
{
	if (m_nSlabMType == DECK_TYPE || m_nSlabMType == DECK_GENERAL) return TRUE;
	return FALSE;
}

BOOL MSRcSlabM::IsPCSlab()
{
	if (m_nSlabMType == PC_TYPE || m_nSlabMType == PC_IMT || m_nSlabMType == PC_RPS || m_nSlabMType == PC_HCS)
		return TRUE;
	return FALSE;
}

void MSRcSlabM::AutoCalcTopSpacerShape(double dMemberThick, double dTopCover, double dBotCover )
{
	if(m_pTopSpacerShape ==NULL)
		return;
	double dSpacing =max(m_ShortStrip.m_Bar4.GetSpacing(), m_LongStrip.m_Bar4.GetSpacing());
	double dTopDia = max(m_ShortStrip.m_Bar1.GetDeformedBarDiameter(),m_LongStrip.m_Bar1.GetDeformedBarDiameter());
	double dBotDia = max(m_ShortStrip.m_Bar4.GetDeformedBarDiameter(),m_LongStrip.m_Bar4.GetDeformedBarDiameter());
	m_pTopSpacerShape->AutoCalc(dSpacing,dMemberThick,dTopCover,dBotCover,m_SpacerBars.GetDeformedBarDiameter(),dTopDia,dBotDia);

}

void MSRcSlabM::AutoCalcBotSpacerShape( double dPileCover/*=150*/ )
{
	if(m_pBotSpacerShape ==NULL)
		return;
	if(dPileCover<GetCover())
		return;
	double dSpacing =max(m_ShortStrip.m_Bar4.GetSpacing(), m_LongStrip.m_Bar4.GetSpacing());
	m_pBotSpacerShape->AutoCalc(dSpacing,dPileCover,0,0,m_SpacerBars.GetDeformedBarDiameter(),0,0);
}

void MSRcSlabM::GetMainBarDiaAllLayer(MSStirrupBar* pStirrupBar, vector<double>& DiaArr)
{
	vector<double> ShortStripTopBar1DiaArr = pStirrupBar->GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), ShortStripTopBar1DiaArr.begin(), ShortStripTopBar1DiaArr.end());
}

vector<double> MSRcSlabM::GetMainBarDiaAllLayer()
{
	vector<double> DiaArr;

	long nSlabMType = GetSlabMType();
	if (nSlabMType == MAT_TYPE || nSlabMType == FLAT_TYPE)
	{
		// ShortStrip Bar 1,4
		// LongStrip Bar 1,4

		vector<double> ShortStripTopBar1DiaArr = m_ShortStrip.m_Bar1.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), ShortStripTopBar1DiaArr.begin(), ShortStripTopBar1DiaArr.end());

		vector<double> ShortStripBotBar4DiaArr = m_ShortStrip.m_Bar4.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), ShortStripBotBar4DiaArr.begin(), ShortStripBotBar4DiaArr.end());

		vector<double> LongStripTopBar1DiaArr = m_LongStrip.m_Bar1.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), LongStripTopBar1DiaArr.begin(), LongStripTopBar1DiaArr.end());

		vector<double> LongStripTopBar4DiaArr = m_LongStrip.m_Bar4.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), LongStripTopBar4DiaArr.begin(), LongStripTopBar4DiaArr.end());
	}
	else if (nSlabMType == GENERAL_TYPE)
	{
		if (m_IsShort1)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar1, DiaArr);
		if (m_IsShort2)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar2, DiaArr);
		if (m_IsShort3)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar3, DiaArr);
		if (m_IsShort4)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar4, DiaArr);
		if (m_IsShort5)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar5, DiaArr);
		if (m_IsShort6)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar6, DiaArr);
		if (m_IsShort7)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar7, DiaArr);
		if (m_IsShort8)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar8, DiaArr);
		if (m_IsShort9)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar9, DiaArr);
		if (m_IsShort10)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar10, DiaArr);

		if (m_IsLong1)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar1, DiaArr);
		if (m_IsLong2)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar2, DiaArr);
		if (m_IsLong3)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar3, DiaArr);
		if (m_IsLong4)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar4, DiaArr);
		if (m_IsLong5)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar5, DiaArr);
		if (m_IsLong6)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar6, DiaArr);
		if (m_IsLong7)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar7, DiaArr);
		if (m_IsLong8)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar8, DiaArr);
		if (m_IsLong9)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar9, DiaArr);
		if (m_IsLong10)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar10, DiaArr);
	}
	/*
		else if(nSlabMType == PC_TYPE)
		{
			// ShortStrip Bar 1~5
			// LongStrip Bar 1~5

			vector<long> ShortStripTopBar1DiaArr = m_ShortStrip.m_TopBar1.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripTopBar1DiaArr.begin(), ShortStripTopBar1DiaArr.end());

			vector<long> ShortStripTopBar2DiaArr = m_ShortStrip.m_TopBar2.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripTopBar2DiaArr.begin(), ShortStripTopBar2DiaArr.end());

			vector<long> ShortStripBotBar3DiaArr = m_ShortStrip.m_BotBar3.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripBotBar3DiaArr.begin(), ShortStripBotBar3DiaArr.end());

			vector<long> ShortStripBotBar4DiaArr = m_ShortStrip.m_BotBar4.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripBotBar4DiaArr.begin(), ShortStripBotBar4DiaArr.end());

			vector<long> ShortStripTopBar5DiaArr = m_ShortStrip.m_TopBar5.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripTopBar5DiaArr.begin(), ShortStripTopBar5DiaArr.end());


			vector<long> LongStripTopBar1DiaArr = m_LongStrip.m_TopBar1.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripTopBar1DiaArr.begin(), LongStripTopBar1DiaArr.end());

			vector<long> LongStripTopBar2DiaArr = m_LongStrip.m_TopBar2.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripTopBar2DiaArr.begin(), LongStripTopBar2DiaArr.end());

			vector<long> LongStripBotBar3DiaArr = m_LongStrip.m_BotBar3.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripBotBar3DiaArr.begin(), LongStripBotBar3DiaArr.end());

			vector<long> LongStripBotBar4DiaArr = m_LongStrip.m_BotBar4.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripBotBar4DiaArr.begin(), LongStripBotBar4DiaArr.end());

			vector<long> LongStripTopBar5DiaArr = m_LongStrip.m_TopBar5.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripTopBar5DiaArr.begin(), LongStripTopBar5DiaArr.end());
		}
	*/
	else if (nSlabMType == DECK_TYPE || nSlabMType == PC_TYPE)
	{
		// DECK : ShortStrip Bar 1~4,  LongStrip Bar 1~6
		// PC	    : ShortStripBar 1~5, LongStripBar 1~5
		if (nSlabMType == DECK_TYPE)
		{
			m_IsShort5 = false;
			m_IsShort6 = false;
		}
		else if (nSlabMType == PC_TYPE)
		{
			m_IsShort6 = false;
			m_IsLong6 = false;
		}
		if (m_IsShort1)
		{
			vector<double> ShortStripTopBar1DiaArr = m_ShortStrip.m_Bar1.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripTopBar1DiaArr.begin(), ShortStripTopBar1DiaArr.end());
		}

		if (m_IsShort2)
		{
			vector<double> ShortStripTopBar2DiaArr = m_ShortStrip.m_Bar2.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripTopBar2DiaArr.begin(), ShortStripTopBar2DiaArr.end());
		}

		if (m_IsShort3)
		{
			vector<double> ShortStripBotBar3DiaArr = m_ShortStrip.m_Bar3.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripBotBar3DiaArr.begin(), ShortStripBotBar3DiaArr.end());
		}

		if (m_IsShort4)
		{
			vector<double> ShortStripBotBar4DiaArr = m_ShortStrip.m_Bar4.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripBotBar4DiaArr.begin(), ShortStripBotBar4DiaArr.end());
		}
		if (m_IsShort5)
		{
			vector<double> ShortStripTopBar5DiaArr = m_ShortStrip.m_Bar5.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripTopBar5DiaArr.begin(), ShortStripTopBar5DiaArr.end());
		}
		if (m_IsShort6)
		{
			vector<double> ShortStripBotBar6DiaArr = m_ShortStrip.m_Bar6.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripBotBar6DiaArr.begin(), ShortStripBotBar6DiaArr.end());
		}
		// 		if(m_IsShort7)
		// 		{
		// 			vector<long> ShortStripBotBar7DiaArr = m_ShortStrip.m_Bar7.GetBarDiaAllLayer();
		// 			DiaArr.insert(DiaArr.end(), ShortStripBotBar7DiaArr.begin(), ShortStripBotBar7DiaArr.end());
		// 		}
		// 		if(m_IsShort8)
		// 		{
		// 			vector<long> ShortStripBotBar8DiaArr = m_ShortStrip.m_Bar8.GetBarDiaAllLayer();
		// 			DiaArr.insert(DiaArr.end(), ShortStripBotBar8DiaArr.begin(), ShortStripBotBar8DiaArr.end());
		// 		}
		// 		if(m_IsShort9)
		// 		{
		// 			vector<long> ShortStripBotBar9DiaArr = m_ShortStrip.m_Bar9.GetBarDiaAllLayer();
		// 			DiaArr.insert(DiaArr.end(), ShortStripBotBar9DiaArr.begin(), ShortStripBotBar9DiaArr.end());
		// 		}
		// 		if(m_IsShort10)
		// 		{
		// 			vector<long> ShortStripBotBar10DiaArr = m_ShortStrip.m_Bar10.GetBarDiaAllLayer();
		// 			DiaArr.insert(DiaArr.end(), ShortStripBotBar10DiaArr.begin(), ShortStripBotBar10DiaArr.end());
		// 		}
		// 
		if (m_IsLong1)
		{
			vector<double> LongStripTopBar1DiaArr = m_LongStrip.m_Bar1.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripTopBar1DiaArr.begin(), LongStripTopBar1DiaArr.end());
		}

		if (m_IsLong2)
		{
			vector<double> LongStripTopBar2DiaArr = m_LongStrip.m_Bar2.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripTopBar2DiaArr.begin(), LongStripTopBar2DiaArr.end());
		}

		if (m_IsLong3)
		{
			vector<double> LongStripBotBar3DiaArr = m_LongStrip.m_Bar3.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripBotBar3DiaArr.begin(), LongStripBotBar3DiaArr.end());
		}

		if (m_IsLong4)
		{
			vector<double> LongStripBotBar4DiaArr = m_LongStrip.m_Bar4.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripBotBar4DiaArr.begin(), LongStripBotBar4DiaArr.end());
		}

		if (m_IsLong5)
		{
			vector<double> LongStripTopBar5DiaArr = m_LongStrip.m_Bar5.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripTopBar5DiaArr.begin(), LongStripTopBar5DiaArr.end());
		}

		if (m_IsLong6)
		{
			vector<double> LongStripBotBar6DiaArr = m_LongStrip.m_Bar6.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripBotBar6DiaArr.begin(), LongStripBotBar6DiaArr.end());
		}
		// 		if(m_IsLong7)
		// 		{
		// 			vector<long> LongStripBotBar7DiaArr = m_LongStrip.m_Bar7.GetBarDiaAllLayer();
		// 			DiaArr.insert(DiaArr.end(), LongStripBotBar7DiaArr.begin(), LongStripBotBar7DiaArr.end());
		// 		}
		// 		if(m_IsLong8)
		// 		{
		// 			vector<long> LongStripBotBar8DiaArr = m_LongStrip.m_Bar8.GetBarDiaAllLayer();
		// 			DiaArr.insert(DiaArr.end(), LongStripBotBar8DiaArr.begin(), LongStripBotBar8DiaArr.end());
		// 		}
		// 		if(m_IsLong9)
		// 		{
		// 			vector<long> LongStripBotBar9DiaArr = m_LongStrip.m_Bar9.GetBarDiaAllLayer();
		// 			DiaArr.insert(DiaArr.end(), LongStripBotBar9DiaArr.begin(), LongStripBotBar9DiaArr.end());
		// 		}
		// 		if(m_IsLong10)
		// 		{
		// 			vector<long> LongStripBotBar10DiaArr = m_LongStrip.m_Bar10.GetBarDiaAllLayer();
		// 			DiaArr.insert(DiaArr.end(), LongStripBotBar10DiaArr.begin(), LongStripBotBar10DiaArr.end());
		// 		}

	}
	else if (nSlabMType == SOG || nSlabMType == CIRCULAR_RAMP)
	{
		if (m_IsShort1)
		{
			vector<double> ShortStripTopBar1DiaArr = m_ShortStrip.m_Bar1.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripTopBar1DiaArr.begin(), ShortStripTopBar1DiaArr.end());
		}
		if (m_IsShort4)
		{
			vector<double> ShortStripBotBar4DiaArr = m_ShortStrip.m_Bar4.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripBotBar4DiaArr.begin(), ShortStripBotBar4DiaArr.end());
		}

		if (m_IsLong1)
		{
			vector<double> LongStripTopBar1DiaArr = m_LongStrip.m_Bar1.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripTopBar1DiaArr.begin(), LongStripTopBar1DiaArr.end());
		}

		if (m_IsLong4)
		{
			vector<double> LongStripBotBar4DiaArr = m_LongStrip.m_Bar4.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripBotBar4DiaArr.begin(), LongStripBotBar4DiaArr.end());
		}
	}
	else if (nSlabMType == DECK_GENERAL)
	{
		vector<double> LongStripBotBar4DiaArr = m_LongStrip.m_Bar7.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), LongStripBotBar4DiaArr.begin(), LongStripBotBar4DiaArr.end());

		if(m_IsNumPlaceDeckBar2)
		{
			vector<double> DeckBar2DiaArr = m_DeckBar2.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), DeckBar2DiaArr.begin(), DeckBar2DiaArr.end());
		}
		else
		{
			vector<double> DeckBar2DiaArr = m_DeckStirrupBar2.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), DeckBar2DiaArr.begin(), DeckBar2DiaArr.end());
		}

		vector<double> DeckBar3DiaArr = m_DeckBar3.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), DeckBar3DiaArr.begin(), DeckBar3DiaArr.end());

		vector<double> DeckBar4DiaArr = m_DeckBar4.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), DeckBar4DiaArr.begin(), DeckBar4DiaArr.end());
	}
	else if (m_nSlabMType == PC_IMT)
	{
		vector<double> barDiaArr;
		barDiaArr = m_ShortStrip.m_Bar1.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_ShortStrip.m_Bar2.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_DeckBar3.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_DeckBar4.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_ShortStrip.m_Bar5.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_ShortStrip.m_Bar6.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_LongStrip.m_Bar1.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_LongStrip.m_Bar2.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_LongStrip.m_Bar3.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());
	}
	else if(m_nSlabMType == PC_RPS)
	{
		vector<double> barDiaArr;
		barDiaArr = m_ShortStrip.m_Bar1.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_ShortStrip.m_Bar2.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_ShortStrip.m_MainBar3.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_LongStrip.m_Bar1.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_LongStrip.m_Bar2.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_LongStrip.m_Bar3.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());
	}
	else if (m_nSlabMType == PC_HCS)
	{
		vector<double> barDiaArr;
		barDiaArr = m_ShortStrip.m_Bar1.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_ShortStrip.m_Bar2.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_ShortStrip.m_MainBar3.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		//전단보강근 미정
		//barDiaArr = m_DeckBar4.GetBarDiaAllLayer();
		//DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_ShortStrip.m_Bar5.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_LongStrip.m_Bar1.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_LongStrip.m_Bar2.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_LongStrip.m_Bar3.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());
	}
	else
		ASSERT(0);

	sort(DiaArr.begin(), DiaArr.end(),
		[](const double& a, const double& b)->bool {return a < b; }
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MSRcSlabM::GetShortBarDiaAllLayer()
{
	vector<double> DiaArr;

	long nSlabMType = GetSlabMType();
	if (nSlabMType == MAT_TYPE || nSlabMType == FLAT_TYPE)
	{
		// ShortStrip Bar 1,4
		vector<double> ShortStripTopBar1DiaArr = m_ShortStrip.m_Bar1.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), ShortStripTopBar1DiaArr.begin(), ShortStripTopBar1DiaArr.end());

		vector<double> ShortStripBotBar4DiaArr = m_ShortStrip.m_Bar4.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), ShortStripBotBar4DiaArr.begin(), ShortStripBotBar4DiaArr.end());
	}
	else if (nSlabMType == GENERAL_TYPE)
	{
		if (m_IsShort1)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar1, DiaArr);
		if (m_IsShort2)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar2, DiaArr);
		if (m_IsShort3)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar3, DiaArr);
		if (m_IsShort4)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar4, DiaArr);
		if (m_IsShort5)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar5, DiaArr);
		if (m_IsShort6)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar6, DiaArr);
		if (m_IsShort7)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar7, DiaArr);
		if (m_IsShort8)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar8, DiaArr);
		if (m_IsShort9)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar9, DiaArr);
		if (m_IsShort10)	GetMainBarDiaAllLayer(&m_ShortStrip.m_Bar10, DiaArr);
	}
	else if (nSlabMType == DECK_TYPE || nSlabMType == PC_TYPE)
	{
		// DECK : ShortStrip Bar 1~4
		// PC	    : ShortStripBar 1~5
		if (nSlabMType == DECK_TYPE)
		{
			m_IsShort5 = false;
			m_IsShort6 = false;
		}
		else if (nSlabMType == PC_TYPE)
		{
			m_IsShort6 = false;
		}
		if (m_IsShort1)
		{
			vector<double> ShortStripTopBar1DiaArr = m_ShortStrip.m_Bar1.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripTopBar1DiaArr.begin(), ShortStripTopBar1DiaArr.end());
		}

		if (m_IsShort2)
		{
			vector<double> ShortStripTopBar2DiaArr = m_ShortStrip.m_Bar2.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripTopBar2DiaArr.begin(), ShortStripTopBar2DiaArr.end());
		}

		if (m_IsShort3)
		{
			vector<double> ShortStripBotBar3DiaArr = m_ShortStrip.m_Bar3.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripBotBar3DiaArr.begin(), ShortStripBotBar3DiaArr.end());
		}

		if (m_IsShort4)
		{
			vector<double> ShortStripBotBar4DiaArr = m_ShortStrip.m_Bar4.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripBotBar4DiaArr.begin(), ShortStripBotBar4DiaArr.end());
		}
		if (m_IsShort5)
		{
			vector<double> ShortStripTopBar5DiaArr = m_ShortStrip.m_Bar5.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripTopBar5DiaArr.begin(), ShortStripTopBar5DiaArr.end());
		}
		if (m_IsShort6)
		{
			vector<double> ShortStripBotBar6DiaArr = m_ShortStrip.m_Bar6.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripBotBar6DiaArr.begin(), ShortStripBotBar6DiaArr.end());
		}
		// 		if(m_IsShort7)
		// 		{
		// 			vector<long> ShortStripBotBar7DiaArr = m_ShortStrip.m_Bar7.GetBarDiaAllLayer();
		// 			DiaArr.insert(DiaArr.end(), ShortStripBotBar7DiaArr.begin(), ShortStripBotBar7DiaArr.end());
		// 		}
		// 		if(m_IsShort8)
		// 		{
		// 			vector<long> ShortStripBotBar8DiaArr = m_ShortStrip.m_Bar8.GetBarDiaAllLayer();
		// 			DiaArr.insert(DiaArr.end(), ShortStripBotBar8DiaArr.begin(), ShortStripBotBar8DiaArr.end());
		// 		}
		// 		if(m_IsShort9)
		// 		{
		// 			vector<long> ShortStripBotBar9DiaArr = m_ShortStrip.m_Bar9.GetBarDiaAllLayer();
		// 			DiaArr.insert(DiaArr.end(), ShortStripBotBar9DiaArr.begin(), ShortStripBotBar9DiaArr.end());
		// 		}
		// 		if(m_IsShort10)
		// 		{
		// 			vector<long> ShortStripBotBar10DiaArr = m_ShortStrip.m_Bar10.GetBarDiaAllLayer();
		// 			DiaArr.insert(DiaArr.end(), ShortStripBotBar10DiaArr.begin(), ShortStripBotBar10DiaArr.end());
		// 		}
		// 
	}
	else if (nSlabMType == SOG || nSlabMType == CIRCULAR_RAMP)
	{
		if (m_IsShort1)
		{
			vector<double> ShortStripTopBar1DiaArr = m_ShortStrip.m_Bar1.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripTopBar1DiaArr.begin(), ShortStripTopBar1DiaArr.end());
		}
		if (m_IsShort4)
		{
			vector<double> ShortStripBotBar4DiaArr = m_ShortStrip.m_Bar4.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShortStripBotBar4DiaArr.begin(), ShortStripBotBar4DiaArr.end());
		}
	}
	else if (nSlabMType == DECK_GENERAL)
	{
		if(m_IsNumPlaceDeckBar2)
		{
			vector<double> DeckBar2DiaArr = m_DeckBar2.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), DeckBar2DiaArr.begin(), DeckBar2DiaArr.end());
		}
		else
		{
			vector<double> DeckBar2DiaArr = m_DeckStirrupBar2.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), DeckBar2DiaArr.begin(), DeckBar2DiaArr.end());
		}

		vector<double> DeckBar3DiaArr = m_DeckBar3.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), DeckBar3DiaArr.begin(), DeckBar3DiaArr.end());

		vector<double> DeckBar4DiaArr = m_DeckBar4.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), DeckBar4DiaArr.begin(), DeckBar4DiaArr.end());
	}
	else if (m_nSlabMType == PC_IMT)
	{
		vector<double> barDiaArr;
		barDiaArr = m_ShortStrip.m_Bar1.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_ShortStrip.m_Bar2.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_DeckBar3.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_DeckBar4.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_ShortStrip.m_Bar5.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_ShortStrip.m_Bar6.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());
	}
	else if(m_nSlabMType == PC_RPS)
	{
		vector<double> barDiaArr;
		barDiaArr = m_ShortStrip.m_Bar1.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_ShortStrip.m_Bar2.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_ShortStrip.m_MainBar3.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());
	}
	else if (m_nSlabMType == PC_HCS)
	{
		vector<double> barDiaArr;
		barDiaArr = m_ShortStrip.m_Bar1.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_ShortStrip.m_Bar2.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_ShortStrip.m_MainBar3.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		//barDiaArr = m_DeckBar4.GetBarDiaAllLayer();
		//DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_ShortStrip.m_Bar5.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());
	}
	else
		ASSERT(0);

	sort(DiaArr.begin(), DiaArr.end(),
		[](const double& a, const double& b)->bool {return a < b; }
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MSRcSlabM::GetLongBarDiaAllLayer()
{
	vector<double> DiaArr;

	long nSlabMType = GetSlabMType();
	if (nSlabMType == MAT_TYPE || nSlabMType == FLAT_TYPE)
	{
		// LongStrip Bar 1,4

		vector<double> LongStripTopBar1DiaArr = m_LongStrip.m_Bar1.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), LongStripTopBar1DiaArr.begin(), LongStripTopBar1DiaArr.end());

		vector<double> LongStripTopBar4DiaArr = m_LongStrip.m_Bar4.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), LongStripTopBar4DiaArr.begin(), LongStripTopBar4DiaArr.end());
	}
	else if (nSlabMType == GENERAL_TYPE)
	{
		if (m_IsLong1)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar1, DiaArr);
		if (m_IsLong2)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar2, DiaArr);
		if (m_IsLong3)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar3, DiaArr);
		if (m_IsLong4)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar4, DiaArr);
		if (m_IsLong5)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar5, DiaArr);
		if (m_IsLong6)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar6, DiaArr);
		if (m_IsLong7)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar7, DiaArr);
		if (m_IsLong8)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar8, DiaArr);
		if (m_IsLong9)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar9, DiaArr);
		if (m_IsLong10)	GetMainBarDiaAllLayer(&m_LongStrip.m_Bar10, DiaArr);
	}
	else if (nSlabMType == DECK_TYPE || nSlabMType == PC_TYPE)
	{
		// DECK : LongStrip Bar 1~6
		// PC	    : LongStripBar 1~5
		if (nSlabMType == PC_TYPE)
		{
			m_IsLong6 = false;
		}
		if (m_IsLong1)
		{
			vector<double> LongStripTopBar1DiaArr = m_LongStrip.m_Bar1.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripTopBar1DiaArr.begin(), LongStripTopBar1DiaArr.end());
		}

		if (m_IsLong2)
		{
			vector<double> LongStripTopBar2DiaArr = m_LongStrip.m_Bar2.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripTopBar2DiaArr.begin(), LongStripTopBar2DiaArr.end());
		}

		if (m_IsLong3)
		{
			vector<double> LongStripBotBar3DiaArr = m_LongStrip.m_Bar3.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripBotBar3DiaArr.begin(), LongStripBotBar3DiaArr.end());
		}

		if (m_IsLong4)
		{
			vector<double> LongStripBotBar4DiaArr = m_LongStrip.m_Bar4.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripBotBar4DiaArr.begin(), LongStripBotBar4DiaArr.end());
		}

		if (m_IsLong5)
		{
			vector<double> LongStripTopBar5DiaArr = m_LongStrip.m_Bar5.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripTopBar5DiaArr.begin(), LongStripTopBar5DiaArr.end());
		}

		if (m_IsLong6)
		{
			vector<double> LongStripBotBar6DiaArr = m_LongStrip.m_Bar6.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripBotBar6DiaArr.begin(), LongStripBotBar6DiaArr.end());
		}
		// 		if(m_IsLong7)
		// 		{
		// 			vector<long> LongStripBotBar7DiaArr = m_LongStrip.m_Bar7.GetBarDiaAllLayer();
		// 			DiaArr.insert(DiaArr.end(), LongStripBotBar7DiaArr.begin(), LongStripBotBar7DiaArr.end());
		// 		}
		// 		if(m_IsLong8)
		// 		{
		// 			vector<long> LongStripBotBar8DiaArr = m_LongStrip.m_Bar8.GetBarDiaAllLayer();
		// 			DiaArr.insert(DiaArr.end(), LongStripBotBar8DiaArr.begin(), LongStripBotBar8DiaArr.end());
		// 		}
		// 		if(m_IsLong9)
		// 		{
		// 			vector<long> LongStripBotBar9DiaArr = m_LongStrip.m_Bar9.GetBarDiaAllLayer();
		// 			DiaArr.insert(DiaArr.end(), LongStripBotBar9DiaArr.begin(), LongStripBotBar9DiaArr.end());
		// 		}
		// 		if(m_IsLong10)
		// 		{
		// 			vector<long> LongStripBotBar10DiaArr = m_LongStrip.m_Bar10.GetBarDiaAllLayer();
		// 			DiaArr.insert(DiaArr.end(), LongStripBotBar10DiaArr.begin(), LongStripBotBar10DiaArr.end());
		// 		}

	}
	else if (nSlabMType == SOG || nSlabMType == CIRCULAR_RAMP)
	{
		if (m_IsLong1)
		{
			vector<double> LongStripTopBar1DiaArr = m_LongStrip.m_Bar1.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripTopBar1DiaArr.begin(), LongStripTopBar1DiaArr.end());
		}

		if (m_IsLong4)
		{
			vector<double> LongStripBotBar4DiaArr = m_LongStrip.m_Bar4.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), LongStripBotBar4DiaArr.begin(), LongStripBotBar4DiaArr.end());
		}
	}
	else if (nSlabMType == DECK_GENERAL)
	{
		vector<double> LongStripBotBar4DiaArr = m_LongStrip.m_Bar7.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), LongStripBotBar4DiaArr.begin(), LongStripBotBar4DiaArr.end());
	}
	else if (m_nSlabMType == PC_IMT)
	{
		vector<double> barDiaArr;
		barDiaArr = m_LongStrip.m_Bar1.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_LongStrip.m_Bar2.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_LongStrip.m_Bar3.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());
	}
	else if(m_nSlabMType == PC_RPS)
	{
		vector<double> barDiaArr;
		barDiaArr = m_LongStrip.m_Bar1.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_LongStrip.m_Bar2.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_LongStrip.m_Bar3.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());
	}
	else if (m_nSlabMType == PC_HCS)
	{
		vector<double> barDiaArr;
		barDiaArr = m_LongStrip.m_Bar1.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_LongStrip.m_Bar2.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());

		barDiaArr = m_LongStrip.m_Bar3.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), barDiaArr.begin(), barDiaArr.end());
	}
	else
		ASSERT(0);

	sort(DiaArr.begin(), DiaArr.end(),
		[](const double& a, const double& b)->bool {return a < b; }
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MSRcSlabM::GetStirrupBarDiaAllLayer()
{
	vector<double> DiaArr;

	if(m_BotSpacerType)   // 0 : 스페이서, 1 : 하부우마철근
	{
		vector<double> SpacerBarDiaArr = m_SpacerBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), SpacerBarDiaArr.begin(), SpacerBarDiaArr.end());
	}
	else
	{
		
	}
	
	sort(DiaArr.begin(), DiaArr.end(), 
		[](const double& a, const double& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

double MSRcSlabM::GetSpacerDia(bool isTop)
{
	double dDia = MSDeformedBar::GetMaxDia();;
	if (isTop)
	{
		if (!m_IsShort1 && !m_IsLong1)
			return 0;
		if (m_IsShort1)	dDia = min(m_ShortStrip.m_Bar1.GetMinBarDia(),dDia);
		if (m_IsLong1)	dDia = min(m_LongStrip.m_Bar1.GetMinBarDia(), dDia);
	}
	else
	{
		if (m_nSlabMType == MSRcSlabM::FLAT_TYPE &&//FlatB
			(m_ShortStrip.m_StripType == MSRcStrip::TYPE_B ||
				m_LongStrip.m_StripType == MSRcStrip::TYPE_B))
		{
			if (!m_IsShort2 && !m_IsLong2)
				return 0;
			if (m_IsShort2)	dDia = min(m_ShortStrip.m_Bar2.GetMinBarDia(), dDia);
			if (m_IsLong2)	dDia = min(m_LongStrip.m_Bar2.GetMinBarDia(), dDia);

		}
		else
		{
			if (!m_IsShort4 && !m_IsLong4)
				return 0;
			if (m_IsShort4)	dDia = min(m_ShortStrip.m_Bar4.GetMinBarDia(), dDia);
			if (m_IsLong4)	dDia = min(m_LongStrip.m_Bar4.GetMinBarDia(), dDia);
		}
	}
	return dDia;
}
double MSRcSlabM::GetMaxDia_ShortTop()
{
	double dDia = 0.;

	if (m_IsShort1) GetMaxDia(m_ShortStrip.m_Bar1, dDia);
	if (m_IsShort2) GetMaxDia(m_ShortStrip.m_Bar2, dDia);
	if (m_IsShort3) GetMaxDia(m_ShortStrip.m_Bar3, dDia);
	if (m_IsShort5) GetMaxDia(m_ShortStrip.m_Bar5, dDia);
	if (m_IsShort8) GetMaxDia(m_ShortStrip.m_Bar8, dDia);
	if (m_IsShort9) GetMaxDia(m_ShortStrip.m_Bar9, dDia);

	return dDia;
}

double MSRcSlabM::GetMinSpacing_ShortTop()
{
	double dSpacing = 9999;

	if (m_IsShort1)	dSpacing = min(m_ShortStrip.m_Bar1.GetSpacing(), dSpacing);
	if (m_IsShort2) dSpacing = min(m_ShortStrip.m_Bar2.GetSpacing(), dSpacing);
	if (m_IsShort3) dSpacing = min(m_ShortStrip.m_Bar3.GetSpacing(), dSpacing);
	if (m_IsShort5) dSpacing = min(m_ShortStrip.m_Bar5.GetSpacing(), dSpacing);
	if (m_IsShort8) dSpacing = min(m_ShortStrip.m_Bar8.GetSpacing(), dSpacing);
	if (m_IsShort9) dSpacing = min(m_ShortStrip.m_Bar9.GetSpacing(), dSpacing);

	return dSpacing;
}
double MSRcSlabM::GetMaxSpacing_ShortTop()
{
	double dSpacing = 0.;

	if (m_IsShort1)	dSpacing = max(m_ShortStrip.m_Bar1.GetSpacing(), dSpacing);
	if (m_IsShort2) dSpacing = max(m_ShortStrip.m_Bar2.GetSpacing(), dSpacing);
	if (m_IsShort3) dSpacing = max(m_ShortStrip.m_Bar3.GetSpacing(), dSpacing);
	if (m_IsShort5) dSpacing = max(m_ShortStrip.m_Bar5.GetSpacing(), dSpacing);
	if (m_IsShort8) dSpacing = max(m_ShortStrip.m_Bar8.GetSpacing(), dSpacing);
	if (m_IsShort9) dSpacing = max(m_ShortStrip.m_Bar9.GetSpacing(), dSpacing);

	return dSpacing;
}
double MSRcSlabM::GetMaxDia_LongTop()
{
	double dDia = 0.;

	if (m_IsLong1) GetMaxDia(m_LongStrip.m_Bar1, dDia);
	if (m_IsLong2) GetMaxDia(m_LongStrip.m_Bar2, dDia);
	if (m_IsLong3) GetMaxDia(m_LongStrip.m_Bar3, dDia);
	if (m_IsLong5) GetMaxDia(m_LongStrip.m_Bar5, dDia);
	if (m_IsLong8) GetMaxDia(m_LongStrip.m_Bar8, dDia);
	if (m_IsLong9) GetMaxDia(m_LongStrip.m_Bar9, dDia);

	return dDia;
}
double MSRcSlabM::GetMinSpacing_LongTop()
{
	double dSpacing = 9999;

	if (m_IsLong1)	dSpacing = min(m_LongStrip.m_Bar1.GetSpacing(), dSpacing);
	if (m_IsLong2) dSpacing = min(m_LongStrip.m_Bar2.GetSpacing(), dSpacing);
	if (m_IsLong3) dSpacing = min(m_LongStrip.m_Bar3.GetSpacing(), dSpacing);
	if (m_IsLong5) dSpacing = min(m_LongStrip.m_Bar5.GetSpacing(), dSpacing);
	if (m_IsLong8) dSpacing = min(m_LongStrip.m_Bar8.GetSpacing(), dSpacing);
	if (m_IsLong9) dSpacing = min(m_LongStrip.m_Bar9.GetSpacing(), dSpacing);

	return dSpacing;
}
double MSRcSlabM::GetMaxSpacing_LongTop()
{
	double dSpacing = 0.;

	if (m_IsLong1)	dSpacing = max(m_LongStrip.m_Bar1.GetSpacing(), dSpacing);
	if (m_IsLong2) dSpacing = max(m_LongStrip.m_Bar2.GetSpacing(), dSpacing);
	if (m_IsLong3) dSpacing = max(m_LongStrip.m_Bar3.GetSpacing(), dSpacing);
	if (m_IsLong5) dSpacing = max(m_LongStrip.m_Bar5.GetSpacing(), dSpacing);
	if (m_IsLong8) dSpacing = max(m_LongStrip.m_Bar8.GetSpacing(), dSpacing);
	if (m_IsLong9) dSpacing = max(m_LongStrip.m_Bar9.GetSpacing(), dSpacing);

	return dSpacing;
}
double MSRcSlabM::GetMaxDia_ShortBot()
{
	double dDia = 0.;

	if (m_IsShort4) GetMaxDia(m_ShortStrip.m_Bar4, dDia);
	if (m_IsShort7) GetMaxDia(m_ShortStrip.m_Bar7, dDia);
	if (m_IsShort6) GetMaxDia(m_ShortStrip.m_Bar6, dDia);
	if (m_IsShort10) GetMaxDia(m_ShortStrip.m_Bar10, dDia);

	return dDia;
}
double MSRcSlabM::GetMinSpacing_ShortBot()
{
	double dSpacing = 9999;

	if (m_IsShort4)	dSpacing = min(m_ShortStrip.m_Bar4.GetSpacing(), dSpacing);
	if (m_IsShort7) dSpacing = min(m_ShortStrip.m_Bar7.GetSpacing(), dSpacing);
	if (m_IsShort6) dSpacing = min(m_ShortStrip.m_Bar6.GetSpacing(), dSpacing);
	if (m_IsShort10) dSpacing = min(m_ShortStrip.m_Bar10.GetSpacing(), dSpacing);

	return dSpacing;
}
double MSRcSlabM::GetMaxSpacing_ShortBot()
{
	double dSpacing = 0.;

	if (m_IsShort4)	dSpacing = max(m_ShortStrip.m_Bar4.GetSpacing(), dSpacing);
	if (m_IsShort7) dSpacing = max(m_ShortStrip.m_Bar7.GetSpacing(), dSpacing);
	if (m_IsShort6) dSpacing = max(m_ShortStrip.m_Bar6.GetSpacing(), dSpacing);
	if (m_IsShort10) dSpacing = max(m_ShortStrip.m_Bar10.GetSpacing(), dSpacing);

	return dSpacing;
}

double MSRcSlabM::GetMaxDia_LongBot()
{
	double dDia = 0.;

	if (m_IsLong4) GetMaxDia(m_LongStrip.m_Bar4, dDia);
	if (m_IsLong7) GetMaxDia(m_LongStrip.m_Bar7, dDia);
	if (m_IsLong6) GetMaxDia(m_LongStrip.m_Bar6, dDia);
	if (m_IsLong10) GetMaxDia(m_LongStrip.m_Bar10, dDia);

	return dDia;
}
double MSRcSlabM::GetMinSpacing_LongBot()
{
	double dSpacing = 9999;

	if (m_IsLong4)	dSpacing = min(m_LongStrip.m_Bar4.GetSpacing(), dSpacing);
	if (m_IsLong7) dSpacing = min(m_LongStrip.m_Bar7.GetSpacing(), dSpacing);
	if (m_IsLong6) dSpacing = min(m_LongStrip.m_Bar6.GetSpacing(), dSpacing);
	if (m_IsLong10) dSpacing = min(m_LongStrip.m_Bar10.GetSpacing(), dSpacing);

	return dSpacing;
}
double MSRcSlabM::GetMaxSpacing_LongBot()
{
	double dSpacing = 0.;

	if (m_IsLong4)	dSpacing = max(m_LongStrip.m_Bar4.GetSpacing(), dSpacing);
	if (m_IsLong7) dSpacing = max(m_LongStrip.m_Bar7.GetSpacing(), dSpacing);
	if (m_IsLong6) dSpacing = max(m_LongStrip.m_Bar6.GetSpacing(), dSpacing);
	if (m_IsLong10) dSpacing = max(m_LongStrip.m_Bar10.GetSpacing(), dSpacing);

	return dSpacing;
}
void MSRcSlabM::GetMaxDia(MSStirrupBar bar, double& dCurMaxDia)
{
	dCurMaxDia = max(bar.GetMaxBarDia(), dCurMaxDia);
}

void MSRcSlabM::GetLxMaxDiaInfo(double &dDia, bool &bUserSteelGradeS)
{
	if (m_IsShort4 && m_ShortStrip.m_Bar4.GetMaxBarDia() > dDia)
	{
		dDia = m_ShortStrip.m_Bar4.GetMaxBarDia();
		bUserSteelGradeS = m_ShortStrip.m_Bar4.GetUserSteelGradeS();	
	}
	if (m_IsShort7 && m_ShortStrip.m_Bar7.GetMaxBarDia() > dDia)
	{
		dDia = m_ShortStrip.m_Bar7.GetMaxBarDia();
		bUserSteelGradeS = m_ShortStrip.m_Bar7.GetUserSteelGradeS();	
	}
	if (m_IsShort6 && m_ShortStrip.m_Bar6.GetMaxBarDia() >  dDia)
	{
		dDia = m_ShortStrip.m_Bar6.GetMaxBarDia();
		bUserSteelGradeS = m_ShortStrip.m_Bar6.GetUserSteelGradeS();	
	}
	if (m_IsShort10 && m_ShortStrip.m_Bar10.GetMaxBarDia() > dDia)
	{
		dDia = m_ShortStrip.m_Bar10.GetMaxBarDia();
		bUserSteelGradeS = m_ShortStrip.m_Bar10.GetUserSteelGradeS();	
	}
	if (m_IsShort1&& m_ShortStrip.m_Bar1.GetMaxBarDia() > dDia)
	{
		dDia = m_ShortStrip.m_Bar1.GetMaxBarDia();
		bUserSteelGradeS = m_ShortStrip.m_Bar1.GetUserSteelGradeS();	
	}
	if (m_IsShort2&& m_ShortStrip.m_Bar2.GetMaxBarDia() > dDia)
	{
		dDia = m_ShortStrip.m_Bar2.GetMaxBarDia();
		bUserSteelGradeS = m_ShortStrip.m_Bar2.GetUserSteelGradeS();	
	}
	if (m_IsShort3&& m_ShortStrip.m_Bar3.GetMaxBarDia() > dDia)
	{
		dDia = m_ShortStrip.m_Bar3.GetMaxBarDia();
		bUserSteelGradeS = m_ShortStrip.m_Bar3.GetUserSteelGradeS();	
	}
	if (m_IsShort5&& m_ShortStrip.m_Bar5.GetMaxBarDia() > dDia)
	{
		dDia = m_ShortStrip.m_Bar5.GetMaxBarDia();
		bUserSteelGradeS = m_ShortStrip.m_Bar5.GetUserSteelGradeS();	
	}
	if (m_IsShort8&& m_ShortStrip.m_Bar8.GetMaxBarDia() > dDia)
	{
		dDia = m_ShortStrip.m_Bar8.GetMaxBarDia();
		bUserSteelGradeS = m_ShortStrip.m_Bar8.GetUserSteelGradeS();	
	}
	if (m_IsShort9&& m_ShortStrip.m_Bar9.GetMaxBarDia() > dDia)
	{
		dDia = m_ShortStrip.m_Bar9.GetMaxBarDia();
		bUserSteelGradeS = m_ShortStrip.m_Bar9.GetUserSteelGradeS();	
	}
}
void MSRcSlabM::GetLyMaxDiaInfo(double &dDia, bool &bUserSteelGradeS)
{
	if (m_IsLong4 && m_LongStrip.m_Bar4.GetMaxBarDia() > dDia)
	{
		dDia = m_LongStrip.m_Bar4.GetMaxBarDia();
		bUserSteelGradeS = m_LongStrip.m_Bar4.GetUserSteelGradeS();
	}
	if (m_IsLong7 && m_LongStrip.m_Bar7.GetMaxBarDia() > dDia)
	{
		dDia = m_LongStrip.m_Bar7.GetMaxBarDia();
		bUserSteelGradeS = m_LongStrip.m_Bar7.GetUserSteelGradeS();
	}
	if (m_IsLong6 && m_LongStrip.m_Bar6.GetMaxBarDia() > dDia)
	{
		dDia = m_LongStrip.m_Bar6.GetMaxBarDia();
		bUserSteelGradeS = m_LongStrip.m_Bar6.GetUserSteelGradeS();
	}
	if (m_IsLong10 && m_LongStrip.m_Bar10.GetMaxBarDia() > dDia)
	{
		dDia = m_LongStrip.m_Bar10.GetMaxBarDia();
		bUserSteelGradeS = m_LongStrip.m_Bar10.GetUserSteelGradeS();
	}
	if (m_IsLong1 && m_LongStrip.m_Bar1.GetMaxBarDia() > dDia)
	{
		dDia = m_LongStrip.m_Bar1.GetMaxBarDia();
		bUserSteelGradeS = m_LongStrip.m_Bar1.GetUserSteelGradeS();
	}
	if (m_IsLong2 && m_LongStrip.m_Bar2.GetMaxBarDia() > dDia)
	{
		dDia = m_LongStrip.m_Bar2.GetMaxBarDia();
		bUserSteelGradeS = m_LongStrip.m_Bar2.GetUserSteelGradeS();
	}
	if (m_IsLong3 && m_LongStrip.m_Bar3.GetMaxBarDia() > dDia)
	{
		dDia = m_LongStrip.m_Bar3.GetMaxBarDia();
		bUserSteelGradeS = m_LongStrip.m_Bar3.GetUserSteelGradeS();
	}
	if (m_IsLong5 && m_LongStrip.m_Bar5.GetMaxBarDia() > dDia)
	{
		dDia = m_LongStrip.m_Bar5.GetMaxBarDia();
		bUserSteelGradeS = m_LongStrip.m_Bar5.GetUserSteelGradeS();
	}
	if (m_IsLong8 && m_LongStrip.m_Bar8.GetMaxBarDia() > dDia)
	{
		dDia = m_LongStrip.m_Bar8.GetMaxBarDia();
		bUserSteelGradeS = m_LongStrip.m_Bar8.GetUserSteelGradeS();
	}
	if (m_IsLong9 && m_LongStrip.m_Bar9.GetMaxBarDia() > dDia)
	{
		dDia = m_LongStrip.m_Bar9.GetMaxBarDia();
		bUserSteelGradeS = m_LongStrip.m_Bar9.GetUserSteelGradeS();
	}
}


MSStirrupBar* MSRcSlabM::GetMaxDiaStirrupBar_ShortTop()
{
	vector<MSStirrupBar*> CheckStirrupBarArr;
	MSStirrupBar* pMaxDiaStirrupBar = NULL;
	double dMaxDia = 0;

	if (m_IsShort1)
		CheckStirrupBarArr.push_back(&m_ShortStrip.m_Bar1);

	if (m_IsShort3)
		CheckStirrupBarArr.push_back(&m_ShortStrip.m_Bar3); 
	
	if (m_IsShort2)
		CheckStirrupBarArr.push_back(&m_ShortStrip.m_Bar2);

	if (m_IsShort5)
		CheckStirrupBarArr.push_back(&m_ShortStrip.m_Bar5);

	if (m_IsShort8)
		CheckStirrupBarArr.push_back(&m_ShortStrip.m_Bar8);

	if (m_IsShort9)
		CheckStirrupBarArr.push_back(&m_ShortStrip.m_Bar9);

	for (long iStirrup = 0; iStirrup < CheckStirrupBarArr.size(); iStirrup++)
	{
		MSStirrupBar* pCheckStirrupBar = CheckStirrupBarArr[iStirrup];
		double dDia = pCheckStirrupBar->GetMaxBarDia();
		if (dMaxDia < dDia || pMaxDiaStirrupBar == NULL)
		{
			pMaxDiaStirrupBar = pCheckStirrupBar;
			dMaxDia = dDia;
		}
	}

	return pMaxDiaStirrupBar;
}

MSStirrupBar* MSRcSlabM::GetMaxDiaStirrupBar_LongTop()
{
	vector<MSStirrupBar*> CheckStirrupBarArr;
	MSStirrupBar* pMaxDiaStirrupBar = NULL;
	double dMaxDia = 0;

	if (m_IsShort1)
		CheckStirrupBarArr.push_back(&m_LongStrip.m_Bar1);

	if (m_IsShort3)
		CheckStirrupBarArr.push_back(&m_LongStrip.m_Bar3);

	if (m_IsShort2)
		CheckStirrupBarArr.push_back(&m_LongStrip.m_Bar2);

	if (m_IsShort5)
		CheckStirrupBarArr.push_back(&m_LongStrip.m_Bar5);

	if (m_IsShort8)
		CheckStirrupBarArr.push_back(&m_LongStrip.m_Bar8);

	if (m_IsShort9)
		CheckStirrupBarArr.push_back(&m_LongStrip.m_Bar9);

	for (long iStirrup = 0; iStirrup < CheckStirrupBarArr.size(); iStirrup++)
	{
		MSStirrupBar* pCheckStirrupBar = CheckStirrupBarArr[iStirrup];
		double dDia = pCheckStirrupBar->GetMaxBarDia();
		if (dMaxDia < dDia || pMaxDiaStirrupBar == NULL)
		{
			pMaxDiaStirrupBar = pCheckStirrupBar;
			dMaxDia = dDia;
		}
	}

	return pMaxDiaStirrupBar;
}

MSStirrupBar* MSRcSlabM::GetMaxDiaStirrupBar_ShortBot()
{
	vector<MSStirrupBar*> CheckStirrupBarArr;
	MSStirrupBar* pMaxDiaStirrupBar = NULL;
	double dMaxDia = 0;

	if (m_IsShort4)
		CheckStirrupBarArr.push_back(&m_ShortStrip.m_Bar4);

	if (m_IsShort7)
		CheckStirrupBarArr.push_back(&m_ShortStrip.m_Bar7);

	if (m_IsShort6)
		CheckStirrupBarArr.push_back(&m_ShortStrip.m_Bar6);

	if (m_IsShort10)
		CheckStirrupBarArr.push_back(&m_ShortStrip.m_Bar10);

	for (long iStirrup = 0; iStirrup < CheckStirrupBarArr.size(); iStirrup++)
	{
		MSStirrupBar* pCheckStirrupBar = CheckStirrupBarArr[iStirrup];
		double dDia = pCheckStirrupBar->GetMaxBarDia();
		if (dMaxDia < dDia || pMaxDiaStirrupBar == NULL)
		{
			pMaxDiaStirrupBar = pCheckStirrupBar;
			dMaxDia = dDia;
		}
	}

	return pMaxDiaStirrupBar;
}

MSStirrupBar* MSRcSlabM::GetMaxDiaStirrupBar_LongBot()
{
	vector<MSStirrupBar*> CheckStirrupBarArr;
	MSStirrupBar* pMaxDiaStirrupBar = NULL;
	double dMaxDia = 0;

	if (m_IsShort4)
		CheckStirrupBarArr.push_back(&m_LongStrip.m_Bar4);

	if (m_IsShort7)
		CheckStirrupBarArr.push_back(&m_LongStrip.m_Bar7);

	if (m_IsShort6)
		CheckStirrupBarArr.push_back(&m_LongStrip.m_Bar6);

	if (m_IsShort10)
		CheckStirrupBarArr.push_back(&m_LongStrip.m_Bar10);

	for (long iStirrup = 0; iStirrup < CheckStirrupBarArr.size(); iStirrup++)
	{
		MSStirrupBar* pCheckStirrupBar = CheckStirrupBarArr[iStirrup];
		double dDia = pCheckStirrupBar->GetMaxBarDia();
		if (dMaxDia < dDia || pMaxDiaStirrupBar == NULL)
		{
			pMaxDiaStirrupBar = pCheckStirrupBar;
			dMaxDia = dDia;
		}
	}

	return pMaxDiaStirrupBar;
}

void MSRcSlabM::SetIsLongBarFlag(bool bFlag)
{
	m_IsLong1 = bFlag;
	m_IsLong2 = bFlag;
	m_IsLong3 = bFlag;
	m_IsLong4 = bFlag;
	m_IsLong5 = bFlag;
	m_IsLong6 = bFlag;
	m_IsLong7 = bFlag;
	m_IsLong8 = bFlag;
	m_IsLong9 = bFlag;
	m_IsLong10 = bFlag;
}

void MSRcSlabM::SetIsShortBarFlag(bool bFlag)
{
	m_IsShort1 = bFlag;
	m_IsShort2 = bFlag;
	m_IsShort3 = bFlag;
	m_IsShort4 = bFlag;
	m_IsShort5 = bFlag;
	m_IsShort6 = bFlag;
	m_IsShort7 = bFlag;
	m_IsShort8 = bFlag;
	m_IsShort9 = bFlag;
	m_IsShort10 = bFlag;
}

void MSRcSlabM::CheckShortnLongBarFlag()
{
	SetIsLongBarFlag(false);
	SetIsShortBarFlag(false);

	if (m_nSlabMType == MAT_TYPE )
	{
		m_IsShort1 = true;
		m_IsShort4 = true;

		m_IsLong1 = true;
		m_IsLong4 = true;
	}
	else if (m_nSlabMType == FLAT_TYPE)
	{
		m_IsShort1 = true;
		m_IsShort4 = true;

		m_IsLong1 = true;
		m_IsLong4 = true;

		if (m_ShortStrip.m_StripType == MSRcStrip::TYPE_B)
		{
			m_IsShort2 = true;
			m_IsShort3 = true;
			m_IsShort5 = true;
			m_IsShort6 = true;
			m_IsShort7 = true;
			m_IsShort8 = true;
			m_IsShort9 = true;
		}

		if (m_LongStrip.m_StripType == MSRcStrip::TYPE_B)
		{
			m_IsLong2 = true;
			m_IsLong3 = true;
			m_IsLong5 = true;
			m_IsLong6 = true;
			m_IsLong7 = true;
			m_IsLong8 = true;
			m_IsLong9 = true;
		}
	}
	else if (m_nSlabMType == SOG || m_nSlabMType == CIRCULAR_RAMP)
	{
		if (m_ShortStrip.m_StripType != MSRcStrip::TYPE_NONE)
		{
			m_IsShort1 = true;
			m_IsShort4 = true;
		}
		if (m_LongStrip.m_StripType != MSRcStrip::TYPE_NONE)
		{
			m_IsLong1 = true;
			m_IsLong4 = true;
		}
	}
	else if (m_nSlabMType == GENERAL_TYPE)
	{
		m_IsShort4 = true;
		m_IsShort5 = true;
		m_IsShort6 = true;

		if (m_ShortStrip.m_StripType == MSRcStrip::TYPE_ALL || m_ShortStrip.m_StripType == MSRcStrip::TYPE_A || m_ShortStrip.m_StripType == MSRcStrip::TYPE_D || m_ShortStrip.m_StripType == MSRcStrip::TYPE_E || m_ShortStrip.m_StripType == MSRcStrip::TYPE_G || m_ShortStrip.m_StripType == MSRcStrip::TYPE_I)
			m_IsShort1 = true;
		if (m_ShortStrip.m_StripType == MSRcStrip::TYPE_ALL || m_ShortStrip.m_StripType == MSRcStrip::TYPE_B || m_ShortStrip.m_StripType == MSRcStrip::TYPE_C || m_ShortStrip.m_StripType == MSRcStrip::TYPE_D || m_ShortStrip.m_StripType == MSRcStrip::TYPE_E || m_ShortStrip.m_StripType == MSRcStrip::TYPE_H)
			m_IsShort2 = true;
		if (m_ShortStrip.m_StripType == MSRcStrip::TYPE_ALL || m_ShortStrip.m_StripType == MSRcStrip::TYPE_F || m_ShortStrip.m_StripType == MSRcStrip::TYPE_G || m_ShortStrip.m_StripType == MSRcStrip::TYPE_H)
			m_IsShort3 = true;
		if (m_ShortStrip.m_StripType == MSRcStrip::TYPE_ALL || m_ShortStrip.m_StripType == MSRcStrip::TYPE_C || m_ShortStrip.m_StripType == MSRcStrip::TYPE_D || m_ShortStrip.m_StripType == MSRcStrip::TYPE_I)
			m_IsShort7 = true;
		if (m_ShortStrip.m_StripType == MSRcStrip::TYPE_ALL)
		{
			m_IsShort8 = true;
			m_IsShort9 = true;
			m_IsShort10 = true;
		}
			
		m_IsLong4 = true;
		m_IsLong5 = true;
		m_IsLong6 = true;

		if (m_LongStrip.m_StripType == MSRcStrip::TYPE_ALL || m_LongStrip.m_StripType == MSRcStrip::TYPE_A || m_LongStrip.m_StripType == MSRcStrip::TYPE_D || m_LongStrip.m_StripType == MSRcStrip::TYPE_E || m_LongStrip.m_StripType == MSRcStrip::TYPE_G || m_LongStrip.m_StripType == MSRcStrip::TYPE_I)
			m_IsLong1 = true;
		if (m_LongStrip.m_StripType == MSRcStrip::TYPE_ALL || m_LongStrip.m_StripType == MSRcStrip::TYPE_B || m_LongStrip.m_StripType == MSRcStrip::TYPE_C || m_LongStrip.m_StripType == MSRcStrip::TYPE_D || m_LongStrip.m_StripType == MSRcStrip::TYPE_E || m_LongStrip.m_StripType == MSRcStrip::TYPE_H)
			m_IsLong2 = true;
		if (m_LongStrip.m_StripType == MSRcStrip::TYPE_ALL || m_LongStrip.m_StripType == MSRcStrip::TYPE_F || m_LongStrip.m_StripType == MSRcStrip::TYPE_G || m_LongStrip.m_StripType == MSRcStrip::TYPE_H)
			m_IsLong3 = true;
		if (m_LongStrip.m_StripType == MSRcStrip::TYPE_ALL || m_LongStrip.m_StripType == MSRcStrip::TYPE_C || m_LongStrip.m_StripType == MSRcStrip::TYPE_D || m_LongStrip.m_StripType == MSRcStrip::TYPE_I)
			m_IsLong7 = true;
		if (m_LongStrip.m_StripType == MSRcStrip::TYPE_ALL)
		{
			m_IsLong8 = true;
			m_IsLong9 = true;
			m_IsLong10 = true;
		}
	}
	else if (m_nSlabMType == PC_TYPE)
	{
		m_IsShort1 = true;
		m_IsShort2 = true;
		m_IsShort3 = true;
		m_IsShort4 = true;
		m_IsShort5 = true;

		m_IsLong1 = true;
		m_IsLong2 = true;
		m_IsLong3 = true;
		m_IsLong4 = true;
		m_IsLong5 = true;
	}
	else if (m_nSlabMType == DECK_TYPE)
	{
		m_IsShort1 = true;
		m_IsShort2 = true;
		m_IsShort3 = true;
		m_IsShort4 = true;

		m_IsLong1 = true;
		m_IsLong2 = true;
		m_IsLong3 = true;
		m_IsLong4 = true;
	}
	else if (m_nSlabMType == DECK_GENERAL)
	{
		m_IsShort1 = true;
		m_IsShort2 = true;
		m_IsShort3 = true;
		m_IsShort4 = true;
		m_IsShort5 = true;

		m_IsLong1 = true;
	}
	else if (m_nSlabMType == PC_IMT)
	{
		m_IsShort1 = m_IsShort2 = m_IsShort3 = m_IsShort4 = m_IsShort5 = m_IsShort6 = true;
		m_IsLong1 = m_IsLong2 = m_IsLong3 = true;
	}
	else if(m_nSlabMType == PC_RPS)
	{
		m_IsShort1 = m_IsShort2 = m_IsShort3 = true;
		m_IsLong1 = m_IsLong2 = m_IsLong3 = true;
	}
	else if (m_nSlabMType == PC_HCS)
	{
		m_IsShort1 = m_IsShort2 = m_IsShort3 = m_IsShort4 = m_IsShort5 = true;
		m_IsLong1 = m_IsLong2 = m_IsLong3 = true;
	}
	else
		ASSERT(0);
}

double MSRcSlabM::GetShortBarMaxDeformedBarDiameter()
{
	double dDia = 0;

	if (m_IsShort1)	dDia = max(dDia, m_ShortStrip.m_Bar1.GetMaxBarDia());
	if (m_IsShort2)
	{
		if(m_nSlabMType == DECK_GENERAL)
		{
			if(m_IsNumPlaceDeckBar2)
				dDia = max(dDia, m_DeckBar2.GetDeformedBarDiameter());
			else
				dDia = max(dDia, m_DeckStirrupBar2.GetMaxBarDia());
		}
		else
			dDia = max(dDia, m_ShortStrip.m_Bar2.GetMaxBarDia());
	}
	if (m_IsShort3)
	{
		if (m_nSlabMType == DECK_GENERAL)
			dDia = max(dDia, m_DeckBar3.GetDeformedBarDiameter());
		else
			dDia = max(dDia, m_ShortStrip.m_Bar3.GetMaxBarDia());
	}
	if (m_IsShort4)
	{
		if (m_nSlabMType == DECK_GENERAL)
			dDia = max(dDia, m_DeckBar4.GetDeformedBarDiameter());
		else
			dDia = max(dDia, m_ShortStrip.m_Bar4.GetMaxBarDia());
	}
	if (m_IsShort5)	dDia = max(dDia, m_ShortStrip.m_Bar5.GetMaxBarDia());
	if (m_IsShort6)	dDia = max(dDia, m_ShortStrip.m_Bar6.GetMaxBarDia());
	if (m_IsShort7)	dDia = max(dDia, m_ShortStrip.m_Bar7.GetMaxBarDia());
	if (m_IsShort8)	dDia = max(dDia, m_ShortStrip.m_Bar8.GetMaxBarDia());
	if (m_IsShort9)	dDia = max(dDia, m_ShortStrip.m_Bar9.GetMaxBarDia());
	if (m_IsShort10) dDia = max(dDia, m_ShortStrip.m_Bar10.GetMaxBarDia());

	return dDia;
}

double MSRcSlabM::GetLongBarMaxDeformedBarDiameter()
{
	double dDia = 0;

	if (m_IsLong1)	dDia = max(dDia, m_LongStrip.m_Bar1.GetMaxBarDia());
	if (m_IsLong2)	dDia = max(dDia, m_LongStrip.m_Bar2.GetMaxBarDia());
	if (m_IsLong3)	dDia = max(dDia, m_LongStrip.m_Bar3.GetMaxBarDia());
	if (m_IsLong4)	dDia = max(dDia, m_LongStrip.m_Bar4.GetMaxBarDia());
	if (m_IsLong5)	dDia = max(dDia, m_LongStrip.m_Bar5.GetMaxBarDia());
	if (m_IsLong6)	dDia = max(dDia, m_LongStrip.m_Bar6.GetMaxBarDia());
	if (m_IsLong7)	dDia = max(dDia, m_LongStrip.m_Bar7.GetMaxBarDia());
	if (m_IsLong8)	dDia = max(dDia, m_LongStrip.m_Bar8.GetMaxBarDia());
	if (m_IsLong9)	dDia = max(dDia, m_LongStrip.m_Bar9.GetMaxBarDia());
	if (m_IsLong10) dDia = max(dDia, m_LongStrip.m_Bar10.GetMaxBarDia());

	return dDia;
}

double MSRcSlabM::GetPastDataL1()
{
	return m_PastDataL1;
}
double MSRcSlabM::GetPastDataL2()
{
	return m_PastDataL2;
}
double MSRcSlabM::GetPastDataL3()
{
	return m_PastDataL3;
}
double MSRcSlabM::GetPastDataH1()
{
	return m_PastDataH1;
}
double MSRcSlabM::GetPastDataH2()
{
	return m_PastDataH2;
}
double MSRcSlabM::GetPastDataH3()
{
	return m_PastDataH3;
}


void MSRcSlabM::GetLxTopMaxDiaInfo(double &dDia, bool &bUserSteelGradeS)
{
	dDia = MSDeformedBar::GetMinDia();
	bUserSteelGradeS = false;
	if(IsPCSlab())
	{
	}
	else if(IsDeckSlab())
	{
	}
	else
		GetRcSlabTopMaxDiaInfo(dDia, bUserSteelGradeS, true);
}
void MSRcSlabM::GetLyTopMaxDiaInfo(double &dDia, bool &bUserSteelGradeS)
{
	dDia = MSDeformedBar::GetMinDia();
	bUserSteelGradeS = false;
	if(IsPCSlab())
	{
	}
	else if(IsDeckSlab())
	{
	}
	else
		GetRcSlabTopMaxDiaInfo(dDia, bUserSteelGradeS, false);
}

void MSRcSlabM::GetLxBotMaxDiaInfo(double &dDia, bool &bUserSteelGradeS)
{
	dDia = MSDeformedBar::GetMinDia();
	bUserSteelGradeS = false;
	if(IsPCSlab())
	{
	}
	else if(IsDeckSlab())
	{
	}
	else
		GetRcSlabBotMaxDiaInfo(dDia, bUserSteelGradeS, true);
}
void MSRcSlabM::GetLyBotMaxDiaInfo(double &dDia, bool &bUserSteelGradeS)
{	
	dDia = MSDeformedBar::GetMinDia();
	bUserSteelGradeS = false;
	if(IsPCSlab())
	{
	}
	else if(IsDeckSlab())
	{
	}
	else
		GetRcSlabBotMaxDiaInfo(dDia, bUserSteelGradeS, false);
}

void MSRcSlabM::GetRcSlabTopMaxDiaInfo(double &dDia, bool &bUserSteelGradeS, bool isLx)
{
	if (m_nSlabMType == MSRcSlabM::FLAT_TYPE && (m_ShortStrip.m_StripType == MSRcStrip::TYPE_B || m_LongStrip.m_StripType == MSRcStrip::TYPE_B))//FlatB
	{
		MSRcStrip &curStrip = isLx ? m_ShortStrip : m_LongStrip;
		if (m_IsShort1&& curStrip.m_Bar1.GetMaxBarDia() > dDia)
		{
			dDia = curStrip.m_Bar1.GetMaxBarDia();
			bUserSteelGradeS = curStrip.m_Bar1.GetUserSteelGradeS();	
		}
		if (m_IsShort3&& curStrip.m_Bar3.GetMaxBarDia() > dDia)
		{
			dDia = curStrip.m_Bar3.GetMaxBarDia();
			bUserSteelGradeS = curStrip.m_Bar3.GetUserSteelGradeS();	
		}
		if (m_IsShort5&& curStrip.m_Bar5.GetMaxBarDia() > dDia)
		{
			dDia = curStrip.m_Bar5.GetMaxBarDia();
			bUserSteelGradeS = curStrip.m_Bar5.GetUserSteelGradeS();	
		}
		if (m_IsShort7 && curStrip.m_Bar7.GetMaxBarDia() > dDia)
		{
			dDia = curStrip.m_Bar7.GetMaxBarDia();
			bUserSteelGradeS = curStrip.m_Bar7.GetUserSteelGradeS();	
		}		
		if (m_IsShort8&& curStrip.m_Bar8.GetMaxBarDia() > dDia)
		{
			dDia = curStrip.m_Bar8.GetMaxBarDia();
			bUserSteelGradeS = curStrip.m_Bar8.GetUserSteelGradeS();	
		}
	}
	else
	{
		MSRcStrip &curStrip = isLx ? m_ShortStrip : m_LongStrip;
		if (m_IsShort1&& curStrip.m_Bar1.GetMaxBarDia() > dDia)
		{
			dDia = curStrip.m_Bar1.GetMaxBarDia();
			bUserSteelGradeS = curStrip.m_Bar1.GetUserSteelGradeS();	
		}
		if (m_IsShort2&& curStrip.m_Bar2.GetMaxBarDia() > dDia)
		{
			dDia = curStrip.m_Bar2.GetMaxBarDia();
			bUserSteelGradeS = curStrip.m_Bar2.GetUserSteelGradeS();	
		}
		if (m_IsShort3&& curStrip.m_Bar3.GetMaxBarDia() > dDia)
		{
			dDia = curStrip.m_Bar3.GetMaxBarDia();
			bUserSteelGradeS = curStrip.m_Bar3.GetUserSteelGradeS();	
		}
		if (m_IsShort5&& curStrip.m_Bar5.GetMaxBarDia() > dDia)
		{
			dDia = curStrip.m_Bar5.GetMaxBarDia();
			bUserSteelGradeS = curStrip.m_Bar5.GetUserSteelGradeS();	
		}
		if (m_IsShort8&& curStrip.m_Bar8.GetMaxBarDia() > dDia)
		{
			dDia = curStrip.m_Bar8.GetMaxBarDia();
			bUserSteelGradeS = curStrip.m_Bar8.GetUserSteelGradeS();	
		}
		if (m_IsShort9&& curStrip.m_Bar9.GetMaxBarDia() > dDia)
		{
			dDia = curStrip.m_Bar9.GetMaxBarDia();
			bUserSteelGradeS = curStrip.m_Bar9.GetUserSteelGradeS();	
		}
	}
}
void MSRcSlabM::GetRcSlabBotMaxDiaInfo(double &dDia, bool &bUserSteelGradeS, bool isLx)
{
	if (m_nSlabMType == MSRcSlabM::FLAT_TYPE && (m_ShortStrip.m_StripType == MSRcStrip::TYPE_B || m_LongStrip.m_StripType == MSRcStrip::TYPE_B))//FlatB
	{
		MSRcStrip &curStrip = isLx ? m_ShortStrip : m_LongStrip;
		if (m_IsShort2&& curStrip.m_Bar2.GetMaxBarDia() > dDia)
		{
			dDia = curStrip.m_Bar2.GetMaxBarDia();
			bUserSteelGradeS = curStrip.m_Bar2.GetUserSteelGradeS();	
		}
		if (m_IsShort4 && curStrip.m_Bar4.GetMaxBarDia() > dDia)
		{
			dDia = curStrip.m_Bar4.GetMaxBarDia();
			bUserSteelGradeS = curStrip.m_Bar4.GetUserSteelGradeS();	
		}
		if (m_IsShort6 && curStrip.m_Bar6.GetMaxBarDia() >  dDia)
		{
			dDia = curStrip.m_Bar6.GetMaxBarDia();
			bUserSteelGradeS = curStrip.m_Bar6.GetUserSteelGradeS();	
		}
		if (m_IsShort9&& curStrip.m_Bar9.GetMaxBarDia() > dDia)
		{
			dDia = curStrip.m_Bar9.GetMaxBarDia();
			bUserSteelGradeS = curStrip.m_Bar9.GetUserSteelGradeS();	
		}		
	}
	else
	{
		MSRcStrip &curStrip = isLx ? m_ShortStrip : m_LongStrip;
		if (m_IsShort4 && curStrip.m_Bar4.GetMaxBarDia() > dDia)
		{
			dDia = curStrip.m_Bar4.GetMaxBarDia();
			bUserSteelGradeS = curStrip.m_Bar4.GetUserSteelGradeS();	
		}
		if (m_IsShort6 && curStrip.m_Bar6.GetMaxBarDia() >  dDia)
		{
			dDia = curStrip.m_Bar6.GetMaxBarDia();
			bUserSteelGradeS = curStrip.m_Bar6.GetUserSteelGradeS();	
		}
		if (m_IsShort7 && curStrip.m_Bar7.GetMaxBarDia() > dDia)
		{
			dDia = curStrip.m_Bar7.GetMaxBarDia();
			bUserSteelGradeS = curStrip.m_Bar7.GetUserSteelGradeS();	
		}
		if (m_IsShort10 && curStrip.m_Bar10.GetMaxBarDia() > dDia)
		{
			dDia = curStrip.m_Bar10.GetMaxBarDia();
			bUserSteelGradeS = curStrip.m_Bar10.GetUserSteelGradeS();	
		}
	}
}

bool MSRcSlabM::IsDeck()
{
	return IsDeckSlab();
}