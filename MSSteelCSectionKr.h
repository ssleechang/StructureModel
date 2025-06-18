#pragma once
#include "afx.h"
#include "MSSteelUtility.h"

#ifndef _MSSteelCSectionKr_HeaderFile
#define _MSSteelCSectionKr_HeaderFile

#include "MSSteelSection.h"
class StructureModel_CLASS MSSteelCSectionKr : public MSSteelSection
{
public:
	MSSteelCSectionKr(void);
	~MSSteelCSectionKr(void);

	virtual void		Init();
	virtual long		GeCSectionType()	{return SECTION_C_TYPE;};
	virtual CString  GeCSectionTypeName()	{return _T("C-Type");};

	double m_H, m_B1, m_B2;
	double m_WThk, m_FThk1, m_FThk2;
	double m_r1, m_r2;

	virtual double  GetH()					{return m_H;};
	virtual double  GetB()					{return max(m_B1, m_B2);};	// max(B1, B2)
	virtual double  GetB1()					{return m_B1;};
	virtual double  GetB2()					{return m_B2;};
	virtual double  GetWThk()			{return m_WThk;};	// max(WThk1, WThk2)
	virtual double  GetFThk()				{return max(m_FThk1, m_FThk2);};	// max(FThk1, FThk2)
	virtual double  GetFThk1()			{return m_FThk1;};
	virtual double  GetFThk2()			{return m_FThk2;};
	virtual double  Getr()					{return max(m_r1, m_r2);};
	virtual double  Getr1()					{return m_r1;};
	virtual double  Getr2()					{return m_r2;};
	virtual void GetSecPolyline(GM2DPolyline& APGon);

	void SetH(double val)			{ m_H = val; };
	void SetB1(double val)			{ m_B1 = val; };
	void SetB2(double val)			{ m_B2 = val; };
	void SetWThk(double val)    { m_WThk = val; };
	void SetFThk1(double val)    { m_FThk1 = val; };
	void SetFThk2(double val)    { m_FThk2 = val; };

};

#endif
