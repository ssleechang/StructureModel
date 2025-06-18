#pragma once
#include "afx.h"

#ifndef _MSSteelAngleKr_HeaderFile
#define _MSSteelAngleKr_HeaderFile

#include "MSSteelSection.h"
class StructureModel_CLASS MSSteelAngleKr : public MSSteelSection
{
public:
	MSSteelAngleKr(void);
	~MSSteelAngleKr(void);

	virtual void		Init();
	virtual long		GetSectionType()	{return SECTION_ANGLE_TYPE;};
	virtual CString  GetSectionTypeName()	{return _T("Angle-Type");};
	virtual void GetSecPolyline(GM2DPolyline& TheGon);

	double m_H, m_B;
	double m_WThk, m_FThk;
	double m_r1, m_r2;
	double m_Cx, m_Cy;
	double m_IIx, m_IIy, m_IIu, m_IIv;
	double m_ix,  m_iy,  m_iu,  m_iv;
	double m_Sx, m_Sy, m_Zx,  m_Zy;

	virtual double GetH()			{ return m_H;};
	virtual double GetB()			{ return m_B;};
	virtual double GetMajorI()   { return m_IIx; };
	virtual double GetMinorI()   { return m_IIy; };
	virtual double GetMajorAe() { return 5.0/6.0*m_H*m_WThk;};
	virtual double GetMinorAe(){ return 5.0/6.0*m_B*m_FThk;};
	virtual double GetKt();
	virtual double GetCw()		 { return 0.0;};
	virtual double GetFThk()      {  return m_FThk;  };
	virtual double GetWThk()     {  return m_WThk;  };
	virtual double Getr1()		{ return m_r1; };
	virtual double Getr2()		{ return m_r2; };

	void SetH(double val)			{ m_H = val; };
	void SetB(double val)			{ m_B = val; };
	void SetWThk(double val)    { m_WThk = val; };
	void SetFThk(double val)    { m_FThk = val; };

};

#endif