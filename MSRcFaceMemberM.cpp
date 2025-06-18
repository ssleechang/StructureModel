#include "StdAfx.h"
#include "MSRcFaceMemberM.h"
#include "MSRebarBuilding.h"
#include "MSRcSlabM.h"
#include "MSRibTypeM.h"
#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSRcFaceMemberM, MSFaceMemberM, VERSIONABLE_SCHEMA | 1)
MSRcFaceMemberM::MSRcFaceMemberM(void)
{
	mp_RibM = NULL;
	m_RibSpacing = 800;
	m_dPcThick = 500;

	m_DeckFormThick = 250;
	m_DeckFormTopWidth = 200;
	m_DeckFormBotWidth = 100;
	m_DeckFormSpace = 600;

	m_IMT_L1 = 225;
	m_IMT_L2 = 150;
	m_IMT_L3 = 450;
	m_IMT_H1 = 60;
	m_IMT_H2 = 120;
	m_IMT_H3 = 150;

	m_HCS_L1 = 250;
	m_HCS_L2 = 150;
	m_HCS_L3 = 0;
	m_HCS_H1 = 300;
	m_HCS_H2 = 100;

	m_RPS_L1 = 140;
	m_RPS_L2 = 500;
	m_RPS_L3 = 440;
	m_RPS_H1 = 150;
	m_RPS_H2 = 250;
	m_RPS_H3 = 150;

	m_HoleType = 0;
	m_HoleNum = 4;
	m_FillHoleNum = 2;
	m_FillHoleLength = 1200;

}

MSRcFaceMemberM::~MSRcFaceMemberM(void)
{
}

void MSRcFaceMemberM::CopyFromMe(MSCompoM* pSource)
{
	if(pSource==NULL)	return;
	MSRcFaceMemberM* pSourceLineM = (MSRcFaceMemberM*)pSource;

	if(pSourceLineM->mp_RibM)
		mp_RibM = pSourceLineM->mp_RibM;
	m_RibSpacing = pSourceLineM->m_RibSpacing;
	m_dPcThick = pSourceLineM->m_dPcThick;

	m_DeckFormThick = pSourceLineM->m_DeckFormThick;
	m_DeckFormTopWidth = pSourceLineM->m_DeckFormTopWidth;
	m_DeckFormBotWidth = pSourceLineM->m_DeckFormBotWidth;
	m_DeckFormSpace = pSourceLineM->m_DeckFormSpace;

	m_IMT_L1 = pSourceLineM->GetIMT_L1();
	m_IMT_L2 = pSourceLineM->GetIMT_L2();
	m_IMT_L3 = pSourceLineM->GetIMT_L3();
	m_IMT_H1 = pSourceLineM->GetIMT_H1();
	m_IMT_H2 = pSourceLineM->GetIMT_H2();
	m_IMT_H3 = pSourceLineM->GetIMT_H3();
	m_HoleType = pSourceLineM->GetHoleType();
	m_HoleNum = pSourceLineM->GetHoleNum();
	m_FillHoleNum = pSourceLineM->GetFillHoleNum();
	m_FillHoleLength = pSourceLineM->GetFillHoleLength();

	m_HCS_L1 = pSourceLineM->GetHCS_L1();
	m_HCS_L2 = pSourceLineM->GetHCS_L2();
	m_HCS_L3 = pSourceLineM->GetHCS_L3();
	m_HCS_H1 = pSourceLineM->GetHCS_H1();
	m_HCS_H2 = pSourceLineM->GetHCS_H2();

	m_RPS_L1 = pSourceLineM->GetRPS_L1();
	m_RPS_L2 = pSourceLineM->GetRPS_L2();
	m_RPS_L3 = pSourceLineM->GetRPS_L3();
	m_RPS_H1 = pSourceLineM->GetRPS_H1();
	m_RPS_H2 = pSourceLineM->GetRPS_H2();
	m_RPS_H3 = pSourceLineM->GetRPS_H3();

	MSFaceMemberM::CopyFromMe(pSource);
}

void MSRcFaceMemberM::Dump(EFS::EFS_Buffer& buffer)
{
	MSFaceMemberM::Dump(buffer);
	buffer << &mp_RibM;
	buffer << m_RibSpacing << m_dPcThick;

	buffer << m_DeckFormThick << m_DeckFormTopWidth << m_DeckFormBotWidth << m_DeckFormSpace;

	buffer << m_IMT_L1 << m_IMT_L2 << m_IMT_L3 << m_IMT_H1 << m_IMT_H2 << m_IMT_H3;
	buffer << m_HoleType << m_HoleNum << m_FillHoleNum << m_FillHoleLength;

	buffer << m_HCS_L1 << m_HCS_L2 << m_HCS_L3;
	buffer << m_HCS_H1 << m_HCS_H2;

	buffer << m_RPS_L1 << m_RPS_L2 << m_RPS_L3;
	buffer << m_RPS_H1 << m_RPS_H2 << m_RPS_H3;
}

void MSRcFaceMemberM::Recovery(EFS::EFS_Buffer& buffer)
{
	MSFaceMemberM::Recovery(buffer);
	buffer >> &mp_RibM;
	buffer >> m_RibSpacing >> m_dPcThick;

	buffer >> m_DeckFormThick >> m_DeckFormTopWidth >> m_DeckFormBotWidth >> m_DeckFormSpace;

	buffer >> m_IMT_L1 >> m_IMT_L2 >> m_IMT_L3 >> m_IMT_H1 >> m_IMT_H2 >> m_IMT_H3;
	buffer >> m_HoleType >> m_HoleNum >> m_FillHoleNum >> m_FillHoleLength;

	buffer >> m_HCS_L1 >> m_HCS_L2 >> m_HCS_L3;
	buffer >> m_HCS_H1 >> m_HCS_H2;

	buffer >> m_RPS_L1 >> m_RPS_L2 >> m_RPS_L3;
	buffer >> m_RPS_H1 >> m_RPS_H2 >> m_RPS_H3;
}

void MSRcFaceMemberM::Serialize(CArchive &ar)
{
	MSFaceMemberM::Serialize(ar);
	long RibMID = 0;
	if(ar.IsStoring())
	{
		if(mp_RibM)
			RibMID = mp_RibM->m_ID;
		ar << RibMID;
		ar << m_RibSpacing;
		ar << m_dPcThick;

		ar << m_DeckFormThick;
		ar << m_DeckFormTopWidth;
		ar << m_DeckFormBotWidth;
		ar << m_DeckFormSpace;

		ar << m_IMT_L1;
		ar << m_IMT_L2;
		ar << m_IMT_L3;
		ar << m_IMT_H1;
		ar << m_IMT_H2;
		ar << m_IMT_H3;
		ar << m_HoleNum;
		ar << m_FillHoleNum;
		ar << m_FillHoleLength;

		ar << m_HCS_L1;
		ar << m_HCS_L2;
		ar << m_HCS_H1;
		ar << m_HCS_H2;
		ar << m_RPS_L1;
		ar << m_RPS_L2;
		ar << m_RPS_L3;
		ar << m_RPS_H1;
		ar << m_RPS_H2;
		ar << m_RPS_H3;

		ar << m_HCS_L3;
		ar << m_HoleType;
	}
	else
	{
		if(MSVersionInfo::GetCurrentVersion() >= 20140901)
		{
			ar >> RibMID;
			if(mp_Owner != nullptr)
				mp_RibM = mp_Owner->GetRibM(RibMID);
			ar >> m_RibSpacing;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20140915)
			ar >> m_dPcThick;

		if(MSVersionInfo::GetCurrentVersion() >= 20160428)
		{
			ar >> m_DeckFormThick;
			ar >> m_DeckFormTopWidth;
			ar >> m_DeckFormBotWidth;
			ar >> m_DeckFormSpace;
		}
		
		if(IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20221011)
		{
			ar >> m_IMT_L1;
			ar >> m_IMT_L2;
			ar >> m_IMT_L3;
			ar >> m_IMT_H1;
			ar >> m_IMT_H2;
			ar >> m_IMT_H3;
			ar >> m_HoleNum;
			ar >> m_FillHoleNum;
			ar >> m_FillHoleLength;
		}
		else
		{
			MSRcSlabM* pRcSlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
			if (pRcSlabM != nullptr)
			{
				m_IMT_L1 = pRcSlabM->GetPastDataL1();
				m_IMT_L2 = pRcSlabM->GetPastDataL2();
				m_IMT_L3 = pRcSlabM->GetPastDataL3();
				m_IMT_H1 = pRcSlabM->GetPastDataH1();
				m_IMT_H2 = pRcSlabM->GetPastDataH2();
				m_IMT_H3 = pRcSlabM->GetPastDataH3();
			}
		}

		//if(MSVersionInfo::GetCurrentVersion() >= 20231024 && MSVersionInfo::IsOldTotalVersion() == false)
		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231024)
		{
			ar >> m_HCS_L1;
			ar >> m_HCS_L2;
			ar >> m_HCS_H1;
			ar >> m_HCS_H2;
			ar >> m_RPS_L1;
			ar >> m_RPS_L2;
			ar >> m_RPS_L3;
			ar >> m_RPS_H1;
			ar >> m_RPS_H2;
			ar >> m_RPS_H3;
		}
		else
		{
			if(GetSlabMType() == MSRcSlabM::PC_HCS)
			{
				m_HCS_L1 = m_IMT_L1;
				m_HCS_L2 = m_IMT_L2;
				m_HCS_H1 = m_IMT_H1;
				m_HCS_H2 = m_IMT_H2;
			}
			if(GetSlabMType() == MSRcSlabM::PC_RPS)
			{
				m_RPS_L1 = m_IMT_L1;
				m_RPS_L2 = m_IMT_L2;
				m_RPS_L3 = m_IMT_L3;
				m_RPS_H1 = m_IMT_H1;
				m_RPS_H2 = m_IMT_H2;
				m_RPS_H3 = m_IMT_H3;
			}
		}
		if ((IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231225) || MSVersionInfo::IsOldTotalVersion())
		{
			ar >> m_HCS_L3;
			ar >> m_HoleType;
		}
		else
		{
			m_HCS_L3 = 0;
			m_HoleType = 0;
		}
	}
}

bool MSRcFaceMemberM::IsMatSlab()
{
	MSRcSlabM* pRcSlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (pRcSlabM == nullptr)
		return false;

	if(pRcSlabM->m_nSlabMType == MSRcSlabM::MAT_TYPE)
		return true;

	return false;
}

bool MSRcFaceMemberM::IsSOGSlab()
{
	MSRcSlabM* pRcSlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (pRcSlabM == nullptr)
		return false;

	if (pRcSlabM->m_nSlabMType == MSRcSlabM::SOG)
		return true;

	return false;
}

//enum SLAB_TYPE {MAT_TYPE=0, FLAT_TYPE, GENERAL_TYPE, PC_TYPE, DECK_TYPE};
long MSRcFaceMemberM::GetSlabMType()
{
	MSRcSlabM* pRcSlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (pRcSlabM == nullptr)
		return MSRcSlabM::GENERAL_TYPE;

	return pRcSlabM->m_nSlabMType;
}

bool MSRcFaceMemberM::IsDeckSlab()
{
	MSRcSlabM* pRcSlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (pRcSlabM == nullptr)
		return false;

	if(pRcSlabM->m_nSlabMType == MSRcSlabM::DECK_TYPE || pRcSlabM->m_nSlabMType == MSRcSlabM::DECK_GENERAL)
		return true;
	return false;
}

bool MSRcFaceMemberM::IsGeneralDeckSlab()
{
	MSRcSlabM* pRcSlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (pRcSlabM == nullptr)
		return false;

	if (pRcSlabM->m_nSlabMType == MSRcSlabM::DECK_GENERAL)
		return true;
	return false;
}
double MSRcFaceMemberM::GetThick()
{	
	long nSlabMType = GetSlabMType();
	if (nSlabMType == MSRcSlabM::PC_IMT)
		return m_IMT_H2 + m_IMT_H3;
	else if (nSlabMType == MSRcSlabM::PC_HCS)
		return m_HCS_H2;
	else if (nSlabMType == MSRcSlabM::PC_RPS)
		return m_RPS_H3;
	return m_dThick;
}
double MSRcFaceMemberM::GetThickXMLExport()
{
	long nSlabMType = GetSlabMType();
	if (nSlabMType == MSRcSlabM::PC_IMT)
		return m_IMT_H1 + m_IMT_H2 + m_IMT_H3;
	else if(nSlabMType == MSRcSlabM::PC_HCS)
		return m_HCS_H1 + m_HCS_H2;
	else if(nSlabMType == MSRcSlabM::PC_RPS)
		return m_RPS_H1 + m_RPS_H2 + m_RPS_H3;

	return m_dThick;
}
double MSRcFaceMemberM::GetIMT_L1() 
{ 
	return m_IMT_L1; 
}
double MSRcFaceMemberM::GetIMT_L2() 
{ 
	return m_IMT_L2; 
}
double MSRcFaceMemberM::GetIMT_L3()
{ 
	return m_IMT_L3; 
}
double MSRcFaceMemberM::GetIMT_H1()
{ 
	return m_IMT_H1; 
}
double MSRcFaceMemberM::GetIMT_H2() 
{ 
	return m_IMT_H2; 
}
double MSRcFaceMemberM::GetIMT_H3() 
{ 
	return m_IMT_H3; 
}
void MSRcFaceMemberM::SetIMT_L1(double param) 
{ 
	m_IMT_L1 = param;
}
void MSRcFaceMemberM::SetIMT_L2(double param) 
{ 
	m_IMT_L2 = param; 
}
void MSRcFaceMemberM::SetIMT_L3(double param) 
{ 
	m_IMT_L3 = param; 
}
void MSRcFaceMemberM::SetIMT_H1(double param) 
{ 
	m_IMT_H1 = param; 
}
void MSRcFaceMemberM::SetIMT_H2(double param) 
{ 
	m_IMT_H2 = param; 
}
void MSRcFaceMemberM::SetIMT_H3(double param) 
{ 
	m_IMT_H3 = param;
}
int MSRcFaceMemberM::GetHoleType()
{
	return m_HoleType;
}
void MSRcFaceMemberM::SetHoleType(int val)
{
	m_HoleType = val;
}
long MSRcFaceMemberM::GetHoleNum()
{
	return m_HoleNum;
}
void MSRcFaceMemberM::SetHoleNum(long val)
{
	m_HoleNum = val;
}
long MSRcFaceMemberM::GetFillHoleNum()
{
	return m_FillHoleNum;
}
void MSRcFaceMemberM::SetFillHoleNum(long val)
{
	m_FillHoleNum = val;
}
double MSRcFaceMemberM::GetFillHoleLength()
{
	return m_FillHoleLength;
}
void MSRcFaceMemberM::SetFillHoleLength(double val)
{
	m_FillHoleLength = val;
}
bool MSRcFaceMemberM::IsGeometryDifferent(MSCompoM* pCompoM)
{	
	if (GetThick() != pCompoM->GetThick())
		return true;
	if (GetWidth() != pCompoM->GetWidth())
		return true;
	if (GetDepth() != pCompoM->GetDepth())
		return true;
	if (GetHeight() != pCompoM->GetHeight())
		return true;
	else if (GetName() != pCompoM->GetName())
		return true;

	MSRcFaceMemberM* pUrCompoM = dynamic_cast<MSRcFaceMemberM*>(pCompoM);
	if(pUrCompoM == nullptr)
		return false;

	long nSlabMType = GetSlabMType();
	if (nSlabMType == MSRcSlabM::PC_TYPE)
	{
		if (fabs(this->GetPcThick() - pUrCompoM->GetPcThick()) > DTOL_GM)
			return true;
	}
	else if(nSlabMType == MSRcSlabM::PC_IMT)
	{
		if (fabs(this->GetIMT_H1() - pUrCompoM->GetIMT_H1()) > DTOL_GM
			|| fabs(this->GetIMT_H2() - pUrCompoM->GetIMT_H2()) > DTOL_GM
			|| fabs(this->GetIMT_H3() - pUrCompoM->GetIMT_H3()) > DTOL_GM
			|| fabs(this->GetIMT_L1() - pUrCompoM->GetIMT_L1()) > DTOL_GM
			|| fabs(this->GetIMT_L2() - pUrCompoM->GetIMT_L2()) > DTOL_GM
			|| fabs(this->GetIMT_L3() - pUrCompoM->GetIMT_L3()) > DTOL_GM)
			return true;
	}
	else if(nSlabMType == MSRcSlabM::PC_RPS)
	{
		if (fabs(this->GetRPS_H1() - pUrCompoM->GetRPS_H1()) > DTOL_GM
			|| fabs(this->GetRPS_H2() - pUrCompoM->GetRPS_H2()) > DTOL_GM
			|| fabs(this->GetRPS_H3() - pUrCompoM->GetRPS_H3()) > DTOL_GM
			|| fabs(this->GetRPS_L1() - pUrCompoM->GetRPS_L1()) > DTOL_GM
			|| fabs(this->GetRPS_L2() - pUrCompoM->GetRPS_L2()) > DTOL_GM
			|| fabs(this->GetRPS_L3() - pUrCompoM->GetRPS_L3()) > DTOL_GM
			|| fabs(this->GetFillHoleLength() - pUrCompoM->GetFillHoleLength()) > DTOL_GM)
			return true;
	}
	else if(nSlabMType == MSRcSlabM::PC_HCS)
	{
		if (fabs(this->GetHCS_H1() - pUrCompoM->GetHCS_H1()) > DTOL_GM
			|| fabs(this->GetHCS_H2() - pUrCompoM->GetHCS_H2()) > DTOL_GM
			|| fabs(this->GetHCS_L1() - pUrCompoM->GetHCS_L1()) > DTOL_GM
			|| fabs(this->GetHCS_L2() - pUrCompoM->GetHCS_L2()) > DTOL_GM
			|| fabs(this->GetHCS_L3() - pUrCompoM->GetHCS_L3()) > DTOL_GM
			|| this->GetHoleType() != pUrCompoM->GetHoleType()
			|| this->GetHoleNum() != pUrCompoM->GetHoleNum()
			|| this->GetFillHoleNum() != pUrCompoM->GetFillHoleNum()
			|| fabs(this->GetFillHoleLength() - pUrCompoM->GetFillHoleLength()) > DTOL_GM)
			return true;
	}
	return false;
}

double MSRcFaceMemberM::GetPcThick()
{
	long nSlabMType = GetSlabMType();
	if (nSlabMType == MSRcSlabM::PC_IMT)
		return m_IMT_H1;
	else if(nSlabMType == MSRcSlabM::PC_HCS)
		return m_HCS_H1;
	else if(nSlabMType == MSRcSlabM::PC_RPS)
		return m_RPS_H1;

	return m_dPcThick; 
}
void MSRcFaceMemberM::SetPcThick(double thick) 
{
	m_dPcThick = thick;
}
double MSRcFaceMemberM::GetHCS_L1()
{
	return m_HCS_L1;
}
double MSRcFaceMemberM::GetHCS_L2()
{
	return m_HCS_L2;
}
double MSRcFaceMemberM::GetHCS_L3()
{
	if (m_HoleType == 0)
		m_HCS_L3 = 0;

	return m_HCS_L3;
}
double MSRcFaceMemberM::GetHCS_H1()
{
	return m_HCS_H1;
}
double MSRcFaceMemberM::GetHCS_H2()
{
	return m_HCS_H2;
}
void MSRcFaceMemberM::SetHCS_L1(double value)
{
	m_HCS_L1 = value;
}
void MSRcFaceMemberM::SetHCS_L2(double value)
{
	m_HCS_L2 = value;
}
void MSRcFaceMemberM::SetHCS_L3(double value)
{
	m_HCS_L3 = value;
}
void MSRcFaceMemberM::SetHCS_H1(double value)
{
	m_HCS_H1 = value;
}
void MSRcFaceMemberM::SetHCS_H2(double value)
{
	m_HCS_H2 = value;
}
	
double MSRcFaceMemberM::GetRPS_L1()
{
	return m_RPS_L1;
}
double MSRcFaceMemberM::GetRPS_L2()
{
	return m_RPS_L2;
}
double MSRcFaceMemberM::GetRPS_L3()
{
	return m_RPS_L3;
}
double MSRcFaceMemberM::GetRPS_H1()
{
	return m_RPS_H1;
}
double MSRcFaceMemberM::GetRPS_H2()
{
	return m_RPS_H2;
}
double MSRcFaceMemberM::GetRPS_H3()
{
	return m_RPS_H3;
}
void MSRcFaceMemberM::SetRPS_L1(double value)
{
	m_RPS_L1 = value;
}
void MSRcFaceMemberM::SetRPS_L2(double value)
{
	m_RPS_L2 = value;
}
void MSRcFaceMemberM::SetRPS_L3(double value)
{
	m_RPS_L3 = value;
}
void MSRcFaceMemberM::SetRPS_H1(double value)
{
	m_RPS_H1 = value;
}
void MSRcFaceMemberM::SetRPS_H2(double value)
{
	m_RPS_H2 = value;
}
void MSRcFaceMemberM::SetRPS_H3(double value)
{
	m_RPS_H3 = value;
}