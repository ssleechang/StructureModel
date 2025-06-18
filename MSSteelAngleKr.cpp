#include "StdAfx.h"
#include "MSSteelAngleKr.h"

MSSteelAngleKr::MSSteelAngleKr(void)
{
	Init();
}
void MSSteelAngleKr::Init()
{
	m_H = m_B = 0.0;
	m_WThk = m_FThk = 0.0;
	m_r1 = m_r2 = 0.0;
	m_Cx = m_Cy = 0.0;
	m_IIx = m_IIy = m_IIu = m_IIv = 0.0;
	m_ix =  m_iy =  m_iu =  m_iv = 0.0;
	m_Sx = m_Sy = m_Zx =  m_Zy = 0.0;
	m_nSecType = SECTION_ANGLE_TYPE;
}
MSSteelAngleKr::~MSSteelAngleKr(void)
{
}

double MSSteelAngleKr::GetKt()
{
	double Kt, I1, I2, Alpha, D;
	double a, b, c, d, h;
	Kt = I1 = I2 = Alpha = D = a = b = c = d = h = 0.0;
	if(m_DataType == 0)
	{
		a = m_B;
		b = m_FThk;
		c = m_H - m_FThk;
		d = m_WThk;

		I1 = a*pow(b,3)*(1.0/3.0 - 0.21*b/a*(1 - pow(b,4)/12/pow(a,4)));
		I2 = c*pow(d,3)*(1.0/3.0 - 0.105*d/c*(1 - pow(d,4)/192/pow(c,4)));
		Alpha = d/b*(0.07 + 0.076*m_r1/b);
		D = 2*(d+b+3*m_r1 - sqrt(2*(2*m_r1+b)*(2*m_r1+d)));
		Kt = I1 + I2 + Alpha*pow(D,4);
	}
	else
	{
		h = m_H - m_FThk/2.0;
		b = m_B - m_WThk/2.0;
		Kt = (h*pow(m_WThk,3) + b*pow(m_FThk,3))/3.0;
	}
	return Kt;
}

void MSSteelAngleKr::GetSecPolyline(GM2DPolyline& TheGon)
{
	double dB = GetB();
	double dH = GetH();
	double dFThk = GetFThk();
	double dWThk = GetWThk();

	TheGon.AddPointAndBulge(new GM2DVector(-dB / 2, dH / 2)); // 좌측 최상단
	TheGon.AddPointAndBulge(new GM2DVector(-dB / 2, -dH / 2));//좌측최하단
	TheGon.AddPointAndBulge(new GM2DVector(dB / 2, -dH / 2));
	TheGon.AddPointAndBulge(new GM2DVector(dB / 2, -dH / 2 + dFThk));
	TheGon.AddPointAndBulge(new GM2DVector(-dB / 2 + dWThk, -dH / 2 + dFThk));
	TheGon.AddPointAndBulge(new GM2DVector(-dB / 2 + dWThk, dH / 2));
	TheGon.MakeClosedPolyline(DTOL_GM);
	TheGon.MakeCCW();
}