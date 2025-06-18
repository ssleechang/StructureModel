#pragma once
#include "mssteelsection.h"

#ifndef _MSSteelDeckPlateKr_HeaderFile
#define _MSSteelDeckPlateKr_HeaderFile

class StructureModel_CLASS MSSteelDeckPlateKr : public MSSteelSection
{
public:
	MSSteelDeckPlateKr(void);
	~MSSteelDeckPlateKr(void);

	virtual void		Init();
	virtual long		GetSectionType()	{return SECTION_DECK_TYPE;};
	virtual CString  GetSectionTypeName()	{return _T("Deck-Type");};

	double m_H, m_B;
	double m_Lw, m_Uw;
	double m_Thk;
	double m_Area, m_W, m_y, m_I, m_Zp, m_Zn, m_ht;

	virtual double GetH()			{ return m_H;};
	virtual double GetB()			{ return m_B;};
	virtual double GetLw()		{ return m_Lw; };
	virtual double GetUw()		{ return m_Uw; };
	virtual double GetFThk()     {  return m_Thk; };
	virtual double GetArea()		{ return m_Area; };
	virtual double GetW()			{ return m_W; };
	virtual double Gety()			{ return m_y; };
	virtual double GetI()			{ return m_I; };
	virtual double GetZp()		{ return m_Zp; };
	virtual double GetZn()		{ return m_Zn; };
	virtual double Getht()			{ return m_ht; };
};

#endif