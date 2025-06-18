#include "StdAfx.h"
#include "MSSteelCSectionKr.h"
#include "MSSteelUtility.h"

MSSteelCSectionKr::MSSteelCSectionKr(void)
{
	Init();
}
void MSSteelCSectionKr::Init()
{
	m_H = m_B1 = m_B2 = 0.0;
	m_WThk = m_FThk1 = m_FThk2 = 0.0;
	m_r1 = m_r2 = 0.0;
	m_nSecType = SECTION_C_TYPE;

}
MSSteelCSectionKr::~MSSteelCSectionKr(void)
{
}

void MSSteelCSectionKr::GetSecPolyline(GM2DPolyline& TheGon)
{
	double b1 = GetB1();
	double b2 = GetB2();
	double b = max(b1, b2);
	double h = GetH();
	double tw = GetWThk();
	double tf1 = GetFThk1();
	double tf2 = GetFThk2();

	TheGon.AddPointAndBulge(new GM2DVector(-b / 2, h / 2)); // 좌측 최상단
	TheGon.AddPointAndBulge(new GM2DVector(-b / 2, -h / 2));//좌측최하단
	TheGon.AddPointAndBulge(new GM2DVector(-b / 2 + b2, -h / 2));//우측최하단
	TheGon.AddPointAndBulge(new GM2DVector(-b / 2 + b2, -h / 2 + tf2));
	TheGon.AddPointAndBulge(new GM2DVector(-b / 2 + tw, -h / 2 + tf2));
	TheGon.AddPointAndBulge(new GM2DVector(-b / 2 + tw, h / 2 - tf1));
	TheGon.AddPointAndBulge(new GM2DVector(-b / 2 + b1, h / 2 - tf1)); // 
	TheGon.AddPointAndBulge(new GM2DVector(-b / 2 + b1, h / 2)); // 우측 최상단

	TheGon.MakeClosedPolyline(DTOL_GM);
	TheGon.MakeCCW();
}
