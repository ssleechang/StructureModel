#pragma once
#include "mssteelsection.h"

#ifndef _MSSteelHSectionKr_HeaderFile
#define _MSSteelHSectionKr_HeaderFile

#include "MSSteelSection.h"
class StructureModel_CLASS MSSteelHSectionKr : public MSSteelSection
{
public:
	MSSteelHSectionKr(void);
	~MSSteelHSectionKr(void);

	virtual void		Init();
	virtual long		GetSectionType()	{return SECTION_H_TYPE;};
	virtual CString  GetSectionTypeName()	{return _T("H-Type");};

	double m_H, m_B1, m_B2;
	double m_WThk, m_FThk1, m_FThk2;
	double m_r;

	virtual double GetH()			{ return m_H;};
	virtual double GetB()				{ return max(m_B1, m_B2);};
	virtual double GetB1()			{ return m_B1;};
	virtual double GetB2()			{ return m_B2;};
	virtual double GetWThk()    { return m_WThk;  };
	virtual double GetFThk()		{ return max(m_FThk1, m_FThk2);};
	virtual double GetFThk1()    { return m_FThk1;  };
	virtual double GetFThk2()    { return m_FThk2;  };
	virtual double Getr()				{ return m_r;};
	virtual void GetSecPolyline(GM2DPolyline& APGon);

	void SetH(double val)			{ m_H = val; };
	void SetB1(double val)			{ m_B1=val; };
	void SetB2(double val)			{ m_B2=val;};
	void SetWThk(double val)    { m_WThk=val;};
	void SetFThk1(double val)    { m_FThk1=val;};
	void SetFThk2(double val)    { m_FThk2=val;};

};

#endif
