#include "StdAfx.h"
#include "MSSteelHSectionKr.h"
#include "GMLib/GM2DRectangle.h"
#include "MSSteelUtility.h"

MSSteelHSectionKr::MSSteelHSectionKr(void)
{
	Init();
}
void MSSteelHSectionKr::Init()
{
	m_H = m_B1 = m_B2 = 0.0;
	m_WThk = m_FThk1 = m_FThk2 = 0.0;
	m_r =0.0;
	m_nSecType = SECTION_H_TYPE;
}
MSSteelHSectionKr::~MSSteelHSectionKr(void)
{
}

void MSSteelHSectionKr::GetSecPolyline(GM2DPolyline& TheGon)
{
	double b1 = GetB1();
	double b2 = GetB2();
	double h = GetH();
	double tw = GetWThk();
	double tf1 = GetFThk1();
	double tf2 = GetFThk2();
	
	TheGon.AddPointAndBulge(new GM2DVector(b1 / 2, h / 2)); // 우측 최상단
	TheGon.AddPointAndBulge(new GM2DVector(-b1 / 2, h / 2));
	TheGon.AddPointAndBulge(new GM2DVector(-b1 / 2, h / 2 - tf1));
	TheGon.AddPointAndBulge(new GM2DVector(-tw / 2, h / 2 - tf1));
	TheGon.AddPointAndBulge(new GM2DVector(-tw / 2, -h / 2 + tf2));
	TheGon.AddPointAndBulge(new GM2DVector(-b2 / 2, -h / 2 + tf2));
	TheGon.AddPointAndBulge(new GM2DVector(-b2 / 2, -h / 2)); // 좌측 최하단
	TheGon.AddPointAndBulge(new GM2DVector(b2 / 2, -h / 2)); // 우측 최하단
	TheGon.AddPointAndBulge(new GM2DVector(b2 / 2, -h / 2 + tf2));
	TheGon.AddPointAndBulge(new GM2DVector(tw / 2, -h / 2 + tf2));
	TheGon.AddPointAndBulge(new GM2DVector(tw / 2, h / 2 - tf1));
	TheGon.AddPointAndBulge(new GM2DVector(b1 / 2, h / 2 - tf1));
	TheGon.MakeClosedPolyline(DTOL_GM);
	TheGon.MakeCCW();
}
