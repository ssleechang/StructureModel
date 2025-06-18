#pragma once
#include "afx.h"

#ifndef _MSSteelDAngleKr_HeaderFile
#define _MSSteelDAngleKr_HeaderFile

#include "MSSteelSection.h"
class StructureModel_CLASS MSSteelDAngleKr : public MSSteelSection
{
public:
	MSSteelDAngleKr(void);
	~MSSteelDAngleKr(void);

	virtual void		Init();
	virtual long		GetSectionType()	{return SECTION_DANGLE_TYPE;};
	virtual CString  GetSectionTypeName()	{return _T("DAngle-Type");};

	double m_H, m_B;
	double m_WThk, m_FThk;
	double m_r1, m_r2;
	double m_Cx;
	double m_ix_A,  m_iy_A;
	double m_IIx, m_IIy, m_ix, m_iy;
	double m_Sx, m_Sy, m_Zx,  m_Zy;
	double m_C;

	virtual double GetH()			{ return m_H;};
	virtual double GetB()			{ return 2*m_B+m_C;};
	virtual double GetMajorI()   { return m_IIx; };
	virtual double GetMinorI()   { return m_IIy; };
	virtual double GetMajorAe() { return 5.0/3.0*m_H*m_WThk;};
	virtual double GetMinorAe(){ return 5.0/3.0*m_B*m_FThk;};
	virtual double GetKt();
	virtual double GetCw()		 { return 0.0;};
	virtual double GetFThk()      {  return m_FThk;  };
	virtual double GetWThk()     {  return m_WThk;  };
	virtual double GetSpacingC() { return m_C;};
	virtual void CalcZ();

	void SetH(double val)			{ m_H = val; };
	void SetB(double val)			{ m_B = val; };
	void SetWThk(double val)    { m_WThk = val; };
	void SetFThk(double val)    { m_FThk = val; };

};

#endif