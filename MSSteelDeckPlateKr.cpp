#include "StdAfx.h"
#include "MSSteelDeckPlateKr.h"

MSSteelDeckPlateKr::MSSteelDeckPlateKr(void)
{
	Init();
}
void MSSteelDeckPlateKr::Init()
{
	m_H = m_B = 0.0;
	m_Lw = m_Uw = 0.0;
	m_Thk = 0.0;
	m_Area = m_W = m_y = m_I = m_Zp = m_Zn = m_ht = 0.0;
	m_nSecType = SECTION_DECK_TYPE;
}
MSSteelDeckPlateKr::~MSSteelDeckPlateKr(void)
{
}
