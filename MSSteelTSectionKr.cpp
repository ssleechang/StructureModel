#include "StdAfx.h"
#include "MSSteelTSectionKr.h"

MSSteelTSectionKr::MSSteelTSectionKr(void)
{
	Init();
}
void MSSteelTSectionKr::Init()
{
	m_H = m_B = 0.0;
	m_WThk = m_FThk = 0.0;
	m_r = 0.0;
	m_IIx = m_IIy = 0.0;
	m_ix =  m_iy = 0.0;
	m_Sx = m_Sy = 0.0;
	m_Zx =  m_Zy = 0.0;
	m_Cx = 0.0;
	m_nSecType = SECTION_T_TYPE;
}
MSSteelTSectionKr::~MSSteelTSectionKr(void)
{
}
double MSSteelTSectionKr::GetKt()
{
	double Kt, I1, I2, Alpha, D;
	double a, b, c, d, t, t1, h;
	if(m_DataType == 0)
	{
		a = m_B;
		b = m_FThk;
		c = m_H - m_FThk;
		d = m_WThk;
		if(b < d)
		{
			t = b;
			t1 = d;
		}
		else
		{
			t = d;
			t1 = b;
		}
		I1 = a * pow(b,3) * ( 1./3. - 0.21 * b / a * (1 - pow(b,4) / 12 / pow(a,4)));
		I2 = c * pow(d,3) * ( 1./3. - 0.105 * d / c * (1 - pow(d,4) / 192 / pow(c,4)));
		Alpha = t / t1 * (0.15 + 0.1 * m_r / b);
		D = ( pow(b+m_r,2) + m_r * d + pow(d,2) / 4 ) / (2*m_r + b);
		Kt = I1 + I2 + Alpha + pow(D,4);

	}
	else
	{
		h = m_H - m_FThk/2;
		b = m_B;
		Kt = ( h * pow(m_WThk,3) + b * pow(m_FThk,3)) / 3;
	}
	return Kt;
}
double MSSteelTSectionKr::GetCw()
{
	double dCw = 0.0;

	double db = m_B;
	double dh = m_H - m_FThk/2.;
	double dtf = m_FThk;
	double dtw = m_WThk;

	dCw = pow(dtf*db, 3)/144. + pow(dtw*dh,3)/36.;

	return dCw;
}
void MSSteelTSectionKr::CalcZ()
{
	double dh = m_H - m_FThk;
	double db = m_B;
	double dtw = m_WThk;
	double dtf = m_FThk;

	double dy = 0.0;
	if(dtw*dh >= db*dtf)
	{
		dy = (db*dtf/dtw + dh)/2.;
		m_Zx = db*dtf*(dh-dy+dtf/2.) + dtw*(pow(dh-dy,2) + pow(dy,2))/2.;
	}
	else
	{
		dy = (2*dh+dtf - dtw*dh/db)/2.;
		m_Zx = (db*pow(dh+dtf-dy,2) + pow(dy-dh,2))/2. + dtw*dh*(dy-dh/2.);
	}

	m_Zy = (dtf*pow(db,2) + dh*pow(dtw,2))/4.;
}

void MSSteelTSectionKr::GetSecPolyline(GM2DPolyline& TheGon)
{
	double dB = GetB();
	double dH = GetH();
	double dFThk = GetFThk();
	double dWThk = GetWThk();

	TheGon.AddPointAndBulge(new GM2DVector(-dB / 2, dH / 2)); // 좌측 최상단
	TheGon.AddPointAndBulge(new GM2DVector(-dB / 2, dH / 2 - dFThk));//좌측최하단
	TheGon.AddPointAndBulge(new GM2DVector(-dWThk / 2, dH / 2 - dFThk));
	TheGon.AddPointAndBulge(new GM2DVector(-dWThk / 2, -dH / 2));
	TheGon.AddPointAndBulge(new GM2DVector(dWThk / 2, -dH / 2));
	TheGon.AddPointAndBulge(new GM2DVector(dWThk / 2, dH / 2 - dFThk));
	TheGon.AddPointAndBulge(new GM2DVector(dB / 2, dH / 2 - dFThk));
	TheGon.AddPointAndBulge(new GM2DVector(dB / 2, dH / 2));
	TheGon.MakeClosedPolyline(DTOL_GM);
	TheGon.MakeCCW();
}