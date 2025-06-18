#include "StdAfx.h"
#include "MSSteelRectPipeKr.h"
#include "MSSteelUtility.h"

MSSteelRectPipeKr::MSSteelRectPipeKr(void)
{
	Init();
}
void MSSteelRectPipeKr::Init()
{
	m_H = m_B = 0.0;
	m_FThk1 = m_FThk2 = 0.0;
	m_WThk1 = m_WThk2 = 0.0;
	m_nSecType = SECTION_RECT_TYPE;
}
MSSteelRectPipeKr::~MSSteelRectPipeKr(void)
{
}

void MSSteelRectPipeKr::GetSecPolyline(GM2DPolyline& TheGon)
{
	double b = GetB();
	double h = GetH();
	double tw1= GetWThk1();
	double tw2 = GetWThk2();
	double tf1 = GetFThk1();
	double tf2 = GetFThk2();
	
	TheGon.AddPointAndBulge(new GM2DVector(b / 2, h / 2)); // 우측 최상단
	TheGon.AddPointAndBulge(new GM2DVector(-b / 2, h / 2));
	TheGon.AddPointAndBulge(new GM2DVector(-b / 2, -h / 2));
	TheGon.AddPointAndBulge(new GM2DVector(b / 2, -h / 2));
	
	GM2DPolyline* inner = new GM2DPolyline(TRUE);
	{
		inner->AddPointAndBulge(new GM2DVector(b / 2 - tw2, h / 2 - tf1)); // 우측 최상단
		inner->AddPointAndBulge(new GM2DVector(-b / 2 + tw1, h / 2 - tf1));
		inner->AddPointAndBulge(new GM2DVector(-b / 2 + tw1, -h / 2 + tf2));
		inner->AddPointAndBulge(new GM2DVector(b / 2 - tw2, -h / 2 + tf2));
		inner->MakeClosedPolyline(DTOL_GM);
		inner->MakeCCW();
		TheGon.ma_InnerOpening.push_back(inner);
	}
	TheGon.MakeClosedPolyline(DTOL_GM);
	TheGon.MakeCCW();
}
