#pragma once
#include "afx.h"

#ifndef _MSSteelSection_HeaderFile
#define _MSSteelSection_HeaderFile

class StructureModel_CLASS MSSteelFileInfo : public CObject
{
public:
	MSSteelFileInfo(){ m_Data.RemoveAll();};
	~MSSteelFileInfo(){};
	CStringArray m_Data;
	long Add(CString data)
	{
		return m_Data.Add(data);
	}
};
class StructureModel_CLASS MSSteelSection : public CObject
{
public:
	MSSteelSection(void);
	~MSSteelSection(void);

	long m_ID;
	CString m_Name;
	long m_nSecType;
	double m_A;
	double m_w;
	long m_DataType;	// 0 : DB 1: Built-In, 2 : Section Manager

	virtual void		Init();
	virtual long	   GetSectionType()	{return m_nSecType;};
	virtual CString GetSectionTypeName()	{return _T("");};
	virtual CString GetName()			{return m_Name;};
	virtual double  GetArea()				{return m_A;};
	virtual double  Getw()					{return m_w;};
	virtual double  GetH()					{return 0.0;};
	virtual double  GetB()					{return 0.0;};	// max(B1, B2)
	virtual double  GetB1()					{return 0.0;};
	virtual double  GetB2()					{return 0.0;};
	virtual double  GetWThk()			{return 0.0;};	// max(WThk1, WThk2)
	virtual double  GetWThk1()			{return 0.0;};
	virtual double  GetWThk2()			{return 0.0;};
	virtual double  GetFThk()				{return 0.0;};	// max(FThk1, FThk2)
	virtual double  GetFThk1()			{return 0.0;};
	virtual double  GetFThk2()			{return 0.0;};
	virtual double  Getr()					{return 0.0;}; //max(r1,r2)
	virtual double  Getr1()					{ return 0.0; };
	virtual double  Getr2()					{ return 0.0; };
	virtual double GetSpaceing()		{return 0.0;};
	virtual double GetPerimeter();

	virtual bool IsGeometryDifferent(MSSteelSection* section);

	virtual void GetSecPolyline(GM2DPolyline& APGon);
	static bool IsSameSectionName(CString oldName, CString compareName);
	static CString GetDataPath();
	static long ReadFileData(CString strFile, vector<MSSteelFileInfo*>& DataArr);
	static long ParsingData(CString TextLine, CStringArray& ValueArr);
	static long GetCommandType(CString TextLine);
	static CString MakeName(vector<double>& params);
	static double _STEEL_DENSITY;
};

#endif