#include "StdAfx.h"
#include "MSSteelDAngleKr.h"

MSSteelDAngleKr::MSSteelDAngleKr(void)
{
	Init();
}
void MSSteelDAngleKr::Init()
{
	m_H = m_B = 0.0;
	m_WThk = m_FThk = 0.0;
	m_r1 = m_r2 = 0.0;
	m_Cx = 0.0;
	m_ix_A =  m_iy_A = 0.0;
	m_IIx = m_IIy = m_ix = m_iy = 0.0;
	m_Sx = m_Sy = m_Zx =  m_Zy = 0.0;
	m_C = 0.0;
	m_nSecType = SECTION_DANGLE_TYPE;
}
MSSteelDAngleKr::~MSSteelDAngleKr(void)
{
}

double MSSteelDAngleKr::GetKt()
{
	double Kt, I1, I2, Alpha, D;
	double a, b, c, d, h;
	if(m_DataType == 0)
	{
		a = m_B;
		b = m_FThk;
		c = m_H - m_FThk;
		d = m_WThk;

		I1 = a*pow(b,3)*(1./3. - 0.21*b/a*(1 - pow(b,4)/12/pow(a,4)));
		I2 = c*pow(d,3)*(1./3. - 0.105*d/c*(1 - pow(d,4)/192/pow(c,4)));
		Alpha = d/b*(0.07 + 0.076*m_r1/b);
		D = 2*(d+b+3*m_r1 - sqrt(2*(2*m_r1+b)*(2*m_r1+d)));
		Kt = (I1 + I2 + Alpha*pow(D,4))*2;
	}
	else
	{
		h = m_H - m_FThk/2.;
		b = m_B - m_WThk/2;
		Kt = ((h*pow(m_WThk,3) + b*pow(m_FThk,3))/3.) * 2;
	}
	return Kt;
}
void MSSteelDAngleKr::CalcZ()
{
	double dh = m_H - m_FThk;
	double db = m_B;
	double dtw = m_WThk;
	double dtf = m_FThk;
	double dC = m_C;

	double dy = 0.0;
	if(dtw*dh >= db*dtf)
	{
		dy = (db*dtf/dtw + dh)/2.;
		m_Zx = 2 * (db*dtf*(dh-dy+dtf/2.) + dtw*(pow(dh-dy,2) + pow(dy,2))/2.);
	}
	else
	{
		dy = (2*dh+dtf - dtw*dh/db)/2.;
		m_Zx = 2 * ((db*pow(dh+dtf-dy,2) + pow(dy-dh,2))/2. + dtw*dh*(dy-dh/2.));
	}

	m_Zy = dh*dtw*(dtw+dC) + db*dtf*(db+dC);
}

