#pragma once
#include "mssteelsection.h"

#ifndef _MSSteelRectPipeKr_HeaderFile
#define _MSSteelRectPipeKr_HeaderFile

class StructureModel_CLASS MSSteelRectPipeKr : public MSSteelSection
{
public:
	MSSteelRectPipeKr(void);
	~MSSteelRectPipeKr(void);

	virtual void		Init();
	virtual long		GetSectionType()	{return SECTION_RECT_TYPE;};
	virtual CString  GetSectionTypeName()	{return _T("Rect-Type");};

	double m_H, m_B;
	double m_FThk1, m_FThk2;
	double m_WThk1, m_WThk2;

	virtual double  GetH()					{return m_H;};
	virtual double  GetB()					{return m_B;};	// max(B1, B2)
	virtual double  GetWThk()			{return max(m_WThk1, m_WThk2);};	// max(WThk1, WThk2)
	virtual double  GetWThk1()			{return m_WThk1;};
	virtual double  GetWThk2()			{return m_WThk2;};
	virtual double  GetFThk()				{return max(m_FThk1, m_FThk2);};	// max(FThk1, FThk2)
	virtual double  GetFThk1()			{return m_FThk1;};
	virtual double  GetFThk2()			{return m_FThk2;};
	virtual void GetSecPolyline(GM2DPolyline& APGon);

	void SetH(double val)			{ m_H = val; };
	void SetB(double val)			{ m_B = val; };
	void SetWThk1(double val)    { m_WThk1 = val; };
	void SetWThk2(double val)    { m_WThk2 = val; };
	void SetFThk1(double val)    { m_FThk1 = val; };
	void SetFThk2(double val)    { m_FThk2 = val; };

};

#endif