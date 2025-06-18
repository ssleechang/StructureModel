#pragma once
#include "mssteelsection.h"

#ifndef _MSSteelRoundPipeKr_HeaderFile
#define _MSSteelRoundPipeKr_HeaderFile

class StructureModel_CLASS MSSteelRoundPipeKr : public MSSteelSection
{
public:
	MSSteelRoundPipeKr(void);
	~MSSteelRoundPipeKr(void);

	virtual void		Init();
	virtual long		GetSectionType()	{return SECTION_ROUND_TYPE;};
	virtual CString  GetSectionTypeName()	{return _T("Round-Type");};

	double m_D;
	double m_Thk;

	virtual double GetD()			{ return m_D;};
	virtual double GetH()			{ return GetD();};
	virtual double GetB()			{ return GetD();};
	virtual double GetFThk()     {  return m_Thk;  };
	virtual double GetWThk()    {  return m_Thk;  };
	virtual void GetSecPolyline(GM2DPolyline& APGon);

	void SetD(double val)			{ m_D = val; };
	void SetThk(double val)    { m_Thk = val; };
	//void SetH(double val)			{ m_D = val; };
	//void SetB(double val)			{ m_D = val; };
	//void SetFThk(double val)    { m_Thk = val; };
	//void SetWThk(double val)    { m_Thk = val; };
	
};

#endif