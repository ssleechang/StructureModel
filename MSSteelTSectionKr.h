#pragma once
#include "mssteelsection.h"

#ifndef _MSSteelTSectionKr_HeaderFile
#define _MSSteelTSectionKr_HeaderFile

#include "MSSteelSection.h"
class StructureModel_CLASS MSSteelTSectionKr : public MSSteelSection
{
public:
	MSSteelTSectionKr(void);
	~MSSteelTSectionKr(void);

	virtual void		Init();
	virtual long		GetSectionType()	{return SECTION_T_TYPE;};
	virtual CString  GetSectionTypeName()	{return _T("T-Type");};
	virtual void GetSecPolyline(GM2DPolyline& TheGon);

	double m_H, m_B;
	double m_WThk, m_FThk;
	double m_r;
	double m_IIx, m_IIy;
	double m_ix,  m_iy;
	double m_Sx, m_Sy;
	double m_Zx,  m_Zy;
	double m_Cx;

	virtual double GetH()			{ return m_H;};
	virtual double GetB()			{ return m_B;};
	virtual double GetFThk()     {  return m_FThk;  };
	virtual double GetWThk()    {  return m_WThk;  };
	virtual double Getr()		{ return m_r; };
	virtual double GetMajorI()   { return m_IIx; };
	virtual double GetMinorI()   { return m_IIy; };
	virtual double GetMajorAe() { return m_H*m_WThk;};
	virtual double GetMinorAe() { return 5.0/6.0*m_B*m_FThk;};
	virtual double GetKt();
	virtual double GetCw();
	virtual void CalcZ();

	void SetH(double val)			{ m_H = val; };
	void SetB(double val)			{ m_B = val; };
	void SetWThk(double val)    { m_WThk = val; };
	void SetFThk(double val)    { m_FThk = val; };
	
};

#endif
