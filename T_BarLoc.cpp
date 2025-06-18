#include "stdafx.h"
#include "T_BarLoc.h"


T_BarLoc::T_BarLoc(GM2DVector LocVec, double dBarDeg, double dDia, bool bUserSteelGradeS)
{
	m_dBarDeg = dBarDeg;
	m_Loc = LocVec;
	m_BottomLoc = LocVec;
	m_nTopDevType = 0;
	m_IsCornerBar = FALSE;
	m_dDia = dDia;
	m_BarName = _T("");
	m_isValue1 = TRUE;
	m_needDowel = FALSE;
	m_nBotDevType = 0;
	m_nBotSpliceType = 0;
	m_nTopSpliceType = 0;
	m_nRootType = COLUMN_MAIN;
	m_nTopLength = 0;
	m_nBotLength = 0;
	m_nTopAlterDist = 0;
	m_nBotAlterDist = 0;
	m_bUserSteelGradeS = bUserSteelGradeS;
}
T_BarLoc* T_BarLoc::Copy()
{
	T_BarLoc* newBarLoc = new T_BarLoc(m_Loc, m_dBarDeg, m_dDia, m_bUserSteelGradeS);
	newBarLoc->m_nTopDevType = m_nTopDevType;
	newBarLoc->m_IsCornerBar = m_IsCornerBar;
	newBarLoc->m_isValue1 = m_isValue1;
	newBarLoc->m_needDowel = m_needDowel;
	newBarLoc->m_nBotDevType = m_nBotDevType;
	newBarLoc->m_nBotSpliceType = m_nBotSpliceType;
	newBarLoc->m_nTopSpliceType = m_nTopSpliceType;
	newBarLoc->m_nRootType = m_nRootType;
	newBarLoc->m_nTopLength = m_nTopLength;
	newBarLoc->m_nBotLength = m_nBotLength;
	newBarLoc->m_nTopAlterDist = m_nTopAlterDist;
	newBarLoc->m_nBotAlterDist = m_nBotAlterDist;
	newBarLoc->m_BottomLoc = m_BottomLoc;
	//		newBarLoc->m_BarName = m_BarName;//철근의 위치정보는 복사하지 않음.
	return newBarLoc;
}


T_BarLoc::~T_BarLoc()
{
}

bool T_BarLoc::CheckGrouping(T_BarLoc* pVerBar)
{
	if (fabs(m_dDia - pVerBar->m_dDia) > DTOL1)
		return false;
	if (m_nTopDevType != pVerBar->m_nTopDevType)
		return false;
	if (m_isValue1 != pVerBar->m_isValue1)
		return false;
	if (m_needDowel != pVerBar->m_needDowel)
		return false;		
	if(m_nRootType!=pVerBar->m_nRootType)
		return false;
	if(fabs(m_dBarDeg-pVerBar->m_dBarDeg)>DTOL1)
		return false;
	if (m_nBotSpliceType != pVerBar->m_nBotSpliceType)
		return false;
	if(m_bUserSteelGradeS != pVerBar->m_bUserSteelGradeS)
		return false;

	return true;
}
bool T_BarLoc::CheckDowelGrouping(T_BarLoc* pVerBar)
{
	if (fabs(m_dDia - pVerBar->m_dDia) > DTOL1)
		return false;
	if (m_nBotDevType != pVerBar->m_nBotDevType)
		return false;
	if (m_isValue1 != pVerBar->m_isValue1)
		return false;
	if (m_needDowel != pVerBar->m_needDowel)
		return false;
	if (m_nBotSpliceType != pVerBar->m_nBotSpliceType)
		return false;	
	if(m_nRootType!=pVerBar->m_nRootType)
		return false;
	if(fabs(m_dBarDeg-pVerBar->m_dBarDeg)>DTOL1)
		return false;
	if(m_bUserSteelGradeS != pVerBar->m_bUserSteelGradeS)
		return false;
	
	return true;
}
bool T_BarLoc::CheckLinkDowelGrouping(T_BarLoc* pVerBar)
{
	if (fabs(m_dDia - pVerBar->m_dDia) > DTOL1)
		return false;
	if (m_nTopDevType != pVerBar->m_nTopDevType)
		return false;
	if (m_nBotDevType != pVerBar->m_nBotDevType)
		return false;
	if (m_isValue1 != pVerBar->m_isValue1)
		return false;
	if (m_needDowel != pVerBar->m_needDowel)
		return false;
	if(m_nRootType != pVerBar->m_nRootType)
		return false;
	if (m_nBotSpliceType != pVerBar->m_nBotSpliceType)
		return false;
	if (m_nTopSpliceType != pVerBar->m_nTopSpliceType)
		return false;
	if(pVerBar->m_nTopLength != m_nTopLength)
		return false;
	if(pVerBar->m_nBotLength != m_nBotLength)
		return false;	
	if(fabs(m_dBarDeg-pVerBar->m_dBarDeg)>DTOL1)
		return false;
	if(pVerBar->m_nTopAlterDist != m_nTopAlterDist)
		return false;
	if(pVerBar->m_nBotAlterDist != m_nBotAlterDist)
		return false;
	if(m_bUserSteelGradeS != pVerBar->m_bUserSteelGradeS)
		return false;

	return true;
}
bool T_BarLoc::CheckTopDownMainBarGrouping(T_BarLoc *pVerBar)
{
	if (fabs(m_dDia - pVerBar->m_dDia) > DTOL1)
		return false;
	if(fabs(m_dBarDeg-pVerBar->m_dBarDeg)>DTOL1)
		return false;
	if(pVerBar->m_nTopLength != m_nTopLength)
		return false;
	if(pVerBar->m_nTopAlterDist != m_nTopAlterDist)
		return false;
	if (m_nBotSpliceType != pVerBar->m_nBotSpliceType)
		return false;
	if (m_nTopSpliceType != pVerBar->m_nTopSpliceType)
		return false;
	if(m_nRootType != pVerBar->m_nRootType)
		return false;
	if (m_isValue1 != pVerBar->m_isValue1)
		return false;
	if(m_bUserSteelGradeS != pVerBar->m_bUserSteelGradeS)
		return false;

	return true;
}
double T_BarLoc::GetDia()
{
	return m_dDia;
}
void T_BarLoc::SetDia(double value)
{
	m_dDia = value;
}
double T_BarLoc::GetBarDeg()
{
	return m_dBarDeg;
}
void T_BarLoc::SetBarDeg(double value)
{
	m_dBarDeg = value;
}
long T_BarLoc::GetTopDevType()
{
	return m_nTopDevType;
}
void T_BarLoc::SetTopDevType(long value)
{
	m_nTopDevType = value;
}
GM2DVector T_BarLoc::GetLoc()
{
	return m_Loc;
}
void T_BarLoc::SetLoc(GM2DVector value)
{
	m_Loc = value;
}
GM2DVector T_BarLoc::GetBottomLoc()
{
	return m_BottomLoc;
}
void T_BarLoc::SetBottomLoc(GM2DVector value)
{
	m_BottomLoc = value;
}
BOOL T_BarLoc::GetIsCornerBar()
{
	return m_IsCornerBar;
}
void T_BarLoc::SetIsCornerBar(BOOL value)
{
	m_IsCornerBar= value;
}
CString T_BarLoc::GetBarName()
{
	return m_BarName;
}
void T_BarLoc::SetBarName(CString value)
{
	m_BarName = value;
}
BOOL T_BarLoc::GetIsValue1()
{
	return m_isValue1;
}
void T_BarLoc::SetIsValue1(BOOL value)
{
	m_isValue1 = value;
}
BOOL T_BarLoc::GetNeedDowel()
{
	return m_needDowel;
}
void T_BarLoc::SetNeedDowel(BOOL value)
{
	m_needDowel = value;
}
long T_BarLoc::GetBotDevType()
{
	return m_nBotDevType;
}
void T_BarLoc::SetBotDevType(long value)
{
	m_nBotDevType = value;
}
long T_BarLoc::GetBotSpliceType()
{
	return m_nBotSpliceType;
}
void T_BarLoc::SetBotSpliceType(long value)
{
	m_nBotSpliceType = value;
}
long T_BarLoc::GetTopSpliceType()
{
	return m_nTopSpliceType;
}
void T_BarLoc::SetTopSpliceType(long value)
{
	m_nTopSpliceType = value;
}
long T_BarLoc::GetRootType()
{
	return m_nRootType;
}
void T_BarLoc::SetRootType(long newVal)
{
	m_nRootType = newVal;
}
long T_BarLoc::GetTopLength()
{
	return m_nTopLength;
}
void T_BarLoc::SetTopLength(long newVal)
{
	m_nTopLength = newVal;
}
long T_BarLoc::GetBotLength()
{
	return m_nBotLength;
}
void T_BarLoc::SetBotLength(long newVal)
{
	m_nBotLength = newVal;
}
long T_BarLoc::GetTopAlterDist()
{
	return m_nTopAlterDist;
}
void T_BarLoc::SetTopAlterDist(long newVal)
{
	m_nTopAlterDist = newVal;
}
long T_BarLoc::GetBotAlterDist()
{
	return m_nBotAlterDist;
}
void T_BarLoc::SetBotAlterDist(long newVal)
{
	m_nBotAlterDist = newVal;
}
bool T_BarLoc::GetUserSteelGradeS()
{
	return m_bUserSteelGradeS;
}
void T_BarLoc::SetUserSteelGradeS(bool value)
{
	m_bUserSteelGradeS = value;
}