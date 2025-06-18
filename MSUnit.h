#pragma once
#include "msobject.h"

#define D_UNIT_FORCE_KGF    0
#define D_UNIT_FORCE_TONF   1
#define D_UNIT_FORCE_N      2
#define D_UNIT_FORCE_KN     3
#define D_UNIT_FORCE_LBF    4
#define D_UNIT_FORCE_KIP    5

#define D_UNIT_LENGTH_MM    0
#define D_UNIT_LENGTH_CM    1
#define D_UNIT_LENGTH_M     2
#define D_UNIT_LENGTH_IN    3
#define D_UNIT_LENGTH_FT    4

class StructureModel_CLASS MSUnit :
	public MSObject
{
public:
	MSUnit();
	MSUnit(long ForceUnit, long LengthUnit);
	~MSUnit();
private:
	long m_nForceUnit;  // 0 : kgf, 1 : tonf, N, kN, lbf, kip
	long m_nLengthUnit; // 0 : mm, 1 : cm, m, in, ft

	static long ms_nDefaultForceUnit;
	static long ms_nDefaultLengthUnit;
	static void SetDefaultUnits(long ForceUnit, long LengthUnit);
	static void GetDefaultUnits(long& ForceUnit, long& LengthUnit);

public:
	double m_dForceFactor;
	double m_dLengthFactor;
	double m_dAreaFactor;
	double m_dMomentFactor;
	double m_dStressFactor;
	double m_dDensityFactor;    // ton/m3, ...
	double m_dConcCostFactor;   //원/m3, ...
	double m_dSteelCostFactor;  //원/ton, ...
	double m_dFormCostFactor;   //원/m2, ...
	double m_dVelocityFactor; // m/sec, ...

	double m_dUnitForceFactor;

	double m_dSUnitLengthFactor;
	double m_dSUnitStressFactor;

	double m_dMUnitMomentFactor; // moment/m, moment/ft  
	double m_dMUnitForceFactor; // force/m, force/ft

	void SetUnit(long ForceUnit=-1, long LengthUnit=-1);
	void GetUnit(long& ForceUnit, long& LengthUnit);
	void Copy(MSUnit& TheUnit);

	CString GetForceStr();
	CString GetMomentStr();
	CString GetStressStr();

	CString GetLengthStr();
	CString GetLength2Str();
	CString GetLength3Str();
	CString GetLength4Str();

	CString GetDensityStr();
	CString GetConcCostStr();
	CString GetSteelCostStr();
	CString GetFormCostStr();

	CString GetVelocityStr();

	CString GetSUnitStressStr(); //kg/cm2/mm, kg/m2/mm, ..., kip/ft2/in, kip/in2/in
	CString GetSUnitLengthStr(); //(m, cm, mm -> mm), (ft, in -> in)

	CString GetUnitForceStr();  //kg/m, kg/cm, ...

	CString GetMUnitForceStr();  //  /m, /ft
	CString GetMUnitMomentStr(); //  /m, /ft

	long GetForceUnitIndex(CString& strForceUnit);
	long GetLengthUnitIndex(CString& strLengthUnit);
};
