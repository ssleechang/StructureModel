#include "StdAfx.h"
#include "MSUnit.h"

long MSUnit::ms_nDefaultForceUnit = D_UNIT_FORCE_KN;
long MSUnit::ms_nDefaultLengthUnit = D_UNIT_LENGTH_M;

MSUnit::MSUnit()
{
	SetUnit(D_UNIT_FORCE_KN, D_UNIT_LENGTH_M);
}

MSUnit::MSUnit(long ForceUnit, long LengthUnit)
{
	SetUnit(ForceUnit, LengthUnit);
}

MSUnit::~MSUnit()
{
}

void MSUnit::SetUnit(long ForceUnit, long LengthUnit)
{
	if(ForceUnit==-1) m_nForceUnit = ms_nDefaultForceUnit;
	else              m_nForceUnit = ForceUnit;

	if(LengthUnit==-1) m_nLengthUnit = ms_nDefaultLengthUnit;
	else                m_nLengthUnit = LengthUnit;

	m_dForceFactor = 1.0;
	if(m_nForceUnit==D_UNIT_FORCE_TONF)      m_dForceFactor = 0.001; //kgf->tonf
	else if(m_nForceUnit==D_UNIT_FORCE_N  )  m_dForceFactor = 9.806; //kgf->N
	else if(m_nForceUnit==D_UNIT_FORCE_KN )  m_dForceFactor = 9.806*0.001; //kgf->kN
	else if(m_nForceUnit==D_UNIT_FORCE_LBF)  m_dForceFactor = (1/4.448)*9.806; //kgf->lbf
	else if(m_nForceUnit==D_UNIT_FORCE_KIP)  m_dForceFactor = (1/4.448)*9.806*0.001; //kgf->kip

	m_dLengthFactor = 1.0;
	if(m_nLengthUnit==D_UNIT_LENGTH_CM)       m_dLengthFactor = 0.1;    //mm->cm
	else if(m_nLengthUnit==D_UNIT_LENGTH_M )  m_dLengthFactor = 0.001;  //mm->m
	else if(m_nLengthUnit==D_UNIT_LENGTH_IN)  m_dLengthFactor = 1/25.4;  //mm->in
	else if(m_nLengthUnit==D_UNIT_LENGTH_FT)  m_dLengthFactor = 1/(12*25.4);  //mm->ft

	m_dAreaFactor = m_dLengthFactor * m_dLengthFactor;
	m_dMomentFactor = m_dForceFactor*m_dLengthFactor;
	m_dStressFactor = m_dForceFactor/(m_dLengthFactor*m_dLengthFactor);

	m_dDensityFactor = m_dForceFactor/(m_dLengthFactor*m_dLengthFactor*m_dLengthFactor);    // ton/m3, ...

	m_dVelocityFactor = m_dLengthFactor; // m/sec, ...
	if(m_nLengthUnit==D_UNIT_LENGTH_IN || m_nLengthUnit==D_UNIT_LENGTH_FT)  m_dVelocityFactor = 3600./1609344.;  //mm/s -> mph(mile/hour)
	m_dUnitForceFactor = m_dForceFactor/m_dLengthFactor;


	double dLengthFactor = 1.0;
	if(m_nLengthUnit==D_UNIT_LENGTH_MM || m_nLengthUnit==D_UNIT_LENGTH_CM || m_nLengthUnit==D_UNIT_LENGTH_M)
		dLengthFactor = 0.001;  //mm->m
	else
		dLengthFactor = 1/(12*25.4);  //mm->ft
	m_dConcCostFactor = 1.0/(dLengthFactor*dLengthFactor*dLengthFactor);
	m_dFormCostFactor = 1.0/(dLengthFactor*dLengthFactor);
	m_dMUnitMomentFactor = m_dMomentFactor / dLengthFactor;
	m_dMUnitForceFactor = m_dForceFactor / dLengthFactor;

	double dForceFactor = 1.0;
	if(m_nForceUnit==D_UNIT_FORCE_KGF || m_nForceUnit==D_UNIT_FORCE_TONF)
		dForceFactor = 0.001; //kgf->tonf
	else if(m_nForceUnit==D_UNIT_FORCE_N || m_nForceUnit==D_UNIT_FORCE_KN)
		dForceFactor = 9.806*0.001; //kgf->kN
	else
		dForceFactor = (1/4.448)*9.806*0.001; //kgf->kip
	m_dSteelCostFactor = 1.0/dForceFactor;

	m_dSUnitLengthFactor = 1.0;
	if(m_nLengthUnit==D_UNIT_LENGTH_MM || m_nLengthUnit==D_UNIT_LENGTH_CM || m_nLengthUnit==D_UNIT_LENGTH_M)
		m_dSUnitLengthFactor = 1.0; //mm->mm
	else
		m_dSUnitLengthFactor = 1/(25.4);  //mm->in
	m_dSUnitStressFactor = m_dStressFactor/m_dSUnitLengthFactor;
}

void MSUnit::GetUnit(long& ForceUnit, long& LengthUnit)
{
	ForceUnit = m_nForceUnit;
	LengthUnit = m_nLengthUnit;
}

void MSUnit::Copy(MSUnit& TheUnit)
{
	long ForceUnit = 0, LengthUnit = 0;
	TheUnit.GetUnit(ForceUnit, LengthUnit);
	SetUnit(ForceUnit, LengthUnit);
}

CString MSUnit::GetForceStr()
{
	CString strUnit = _T("tonf");

	switch(m_nForceUnit)
	{
	case D_UNIT_FORCE_KGF : return _T("kgf");
	case D_UNIT_FORCE_TONF: return _T("tonf");
	case D_UNIT_FORCE_N   : return _T("N");
	case D_UNIT_FORCE_KN  : return _T("kN");
	case D_UNIT_FORCE_LBF : return _T("lbf");
	case D_UNIT_FORCE_KIP : return _T("kip");
	}
	return strUnit;
}

CString MSUnit::GetMomentStr()
{
	CString strForceUnit = GetForceStr();
	CString strLengthUnit = GetLengthStr();

	CString strUnit=_T("");
	strUnit.Format(_T("%s·%s"), strForceUnit, strLengthUnit);

	return strUnit;
}

CString MSUnit::GetStressStr()
{
	CString strForceUnit = GetForceStr();
	CString strAreaUnit = GetLength2Str();

	CString strUnit=_T("");

	if(m_nForceUnit==D_UNIT_FORCE_N && m_nLengthUnit==D_UNIT_LENGTH_MM)
		strUnit = _T("MPa");
	else if(m_nForceUnit==D_UNIT_FORCE_LBF && m_nLengthUnit==D_UNIT_LENGTH_IN)
		strUnit = _T("psi");
	else if(m_nForceUnit==D_UNIT_FORCE_LBF && m_nLengthUnit==D_UNIT_LENGTH_FT)
		strUnit = _T("psf");
	else if(m_nForceUnit==D_UNIT_FORCE_KIP && m_nLengthUnit==D_UNIT_LENGTH_IN)
		strUnit = _T("ksi");
	else if(m_nForceUnit==D_UNIT_FORCE_KIP && m_nLengthUnit==D_UNIT_LENGTH_FT)
		strUnit = _T("ksf");
	else strUnit.Format(_T("%s/%s"), strForceUnit, strAreaUnit);

	return strUnit;
}

CString MSUnit::GetLengthStr()
{
	CString strUnit = _T("cm");

	switch(m_nLengthUnit)
	{
	case D_UNIT_LENGTH_MM: return _T("mm");
	case D_UNIT_LENGTH_CM: return _T("cm");
	case D_UNIT_LENGTH_M : return _T("m");
	case D_UNIT_LENGTH_IN: return _T("in");
	case D_UNIT_LENGTH_FT: return _T("ft");
	}
	return strUnit;
}

CString MSUnit::GetLength2Str()
{
	CString strLengthUnit = GetLengthStr();
	CString strUnit=_T("");
	strUnit.Format(_T("%s²"), strLengthUnit);
	return strUnit;
}

CString MSUnit::GetLength3Str()
{
	CString strLengthUnit = GetLengthStr();
	CString strUnit=_T("");
	strUnit.Format(_T("%s³"), strLengthUnit);
	return strUnit;
}

CString MSUnit::GetLength4Str()
{
	CString strLengthUnit = GetLengthStr();
	CString strUnit=_T("");
	strUnit.Format(_T("%s⁴"), strLengthUnit);
	return strUnit;
}

CString MSUnit::GetDensityStr()
{
	CString strForceUnit = GetForceStr();
	CString strLengthUnit = GetLengthStr();
	CString strUnit=_T("");
	strUnit.Format(_T("%s/%s³"), strForceUnit, strLengthUnit);
	return strUnit;
}

CString MSUnit::GetConcCostStr()
{
	CString strLengthUnit = _T("");
	if(m_nLengthUnit==D_UNIT_LENGTH_MM || m_nLengthUnit==D_UNIT_LENGTH_CM || m_nLengthUnit==D_UNIT_LENGTH_M)
		strLengthUnit = _T("m");
	else
		strLengthUnit = _T("ft");

	CString strUnit=_T("");
	strUnit.Format(_T("Won/%s³"), strLengthUnit);
	return strUnit;
}

CString MSUnit::GetSteelCostStr()
{
	CString strForceUnit = _T("");

	if(m_nForceUnit==D_UNIT_FORCE_KGF || m_nForceUnit==D_UNIT_FORCE_TONF)
		strForceUnit = _T("tonf");
	else if(m_nForceUnit==D_UNIT_FORCE_N || m_nForceUnit==D_UNIT_FORCE_KN)
		strForceUnit = _T("kN");
	else
		strForceUnit = _T("kip");

	CString strUnit=_T("");
	strUnit.Format(_T("Won/%s"), strForceUnit);
	return strUnit;
}

CString MSUnit::GetFormCostStr()
{
	CString strLengthUnit = _T("");
	if(m_nLengthUnit==D_UNIT_LENGTH_MM || m_nLengthUnit==D_UNIT_LENGTH_CM || m_nLengthUnit==D_UNIT_LENGTH_M)
		strLengthUnit = _T("m");
	else
		strLengthUnit = _T("ft");

	CString strUnit=_T("");
	strUnit.Format(_T("Won/%s²"), strLengthUnit);
	return strUnit;
}

CString MSUnit::GetSUnitStressStr()
{
	CString strUnit=_T("");
	strUnit.Format(_T("%s/%s"), GetStressStr(), GetSUnitLengthStr());
	return strUnit;
}

CString MSUnit::GetSUnitLengthStr()
{
	CString strLengthUnit = _T("");
	if(m_nLengthUnit==D_UNIT_LENGTH_MM || m_nLengthUnit==D_UNIT_LENGTH_CM || m_nLengthUnit==D_UNIT_LENGTH_M)
		strLengthUnit = _T("mm");
	else
		strLengthUnit = _T("in");

	return strLengthUnit;
}

CString MSUnit::GetVelocityStr()
{
	CString strUnit = _T("");

	if(m_nLengthUnit==D_UNIT_LENGTH_MM || m_nLengthUnit==D_UNIT_LENGTH_CM || m_nLengthUnit==D_UNIT_LENGTH_M)
	{
		CString strLengthUnit = GetLengthStr();
		strUnit.Format(_T("%s/sec"), strLengthUnit);
	}
	else
	{
		strUnit.Format(_T("mph"));
	}

	return strUnit;
}

CString MSUnit::GetUnitForceStr()
{
	CString strForceUnit = GetForceStr();
	CString strLengthUnit = GetLengthStr();
	CString strUnit=_T("");
	strUnit.Format(_T("%s/%s"), strForceUnit, strLengthUnit);
	return strUnit;
}

CString MSUnit::GetMUnitForceStr()
{
	CString strForceUnit = GetForceStr();
	CString strLengthUnit = _T("");
	if(m_nLengthUnit==D_UNIT_LENGTH_MM || m_nLengthUnit==D_UNIT_LENGTH_CM || m_nLengthUnit==D_UNIT_LENGTH_M)
		strLengthUnit = _T("m");
	else
		strLengthUnit = _T("ft");

	CString strUnit=_T("");
	strUnit.Format(_T("%s/%s"), strForceUnit, strLengthUnit);
	return strUnit;
}

CString MSUnit::GetMUnitMomentStr()
{
	CString strMomentUnit = GetMomentStr();
	CString strLengthUnit = _T("");
	if(m_nLengthUnit==D_UNIT_LENGTH_MM || m_nLengthUnit==D_UNIT_LENGTH_CM || m_nLengthUnit==D_UNIT_LENGTH_M)
		strLengthUnit = _T("m");
	else
		strLengthUnit = _T("ft");

	CString strUnit=_T("");
	strUnit.Format(_T("%s/%s"), strMomentUnit, strLengthUnit);
	return strUnit;
}


long MSUnit::GetForceUnitIndex(CString& strForceUnit)
{
	long nForceUnit = -1;
	strForceUnit.MakeUpper();

	if(strForceUnit==_T("KGF")) nForceUnit = D_UNIT_FORCE_KGF;
	else if(strForceUnit==_T("TONF")) nForceUnit = D_UNIT_FORCE_TONF;
	else if(strForceUnit==_T("N")) nForceUnit = D_UNIT_FORCE_N;
	else if(strForceUnit==_T("KN")) nForceUnit = D_UNIT_FORCE_KN;
	else if(strForceUnit==_T("LBF")) nForceUnit = D_UNIT_FORCE_LBF;
	else if(strForceUnit==_T("KIP")) nForceUnit = D_UNIT_FORCE_KIP;
	else ASSERT(0);

	return nForceUnit;
}

long MSUnit::GetLengthUnitIndex(CString& strLengthUnit)
{
	long nLengthUnit = -1;
	strLengthUnit.MakeUpper();

	if(strLengthUnit==_T("MM")) nLengthUnit = D_UNIT_LENGTH_MM;
	else if(strLengthUnit==_T("CM")) nLengthUnit = D_UNIT_LENGTH_CM;
	else if(strLengthUnit==_T("M")) nLengthUnit = D_UNIT_LENGTH_M;
	else if(strLengthUnit==_T("IN")) nLengthUnit = D_UNIT_LENGTH_IN;
	else if(strLengthUnit==_T("FT")) nLengthUnit = D_UNIT_LENGTH_FT;
	else ASSERT(0);

	return nLengthUnit;
}

void MSUnit::SetDefaultUnits( long ForceUnit, long LengthUnit )
{
	ms_nDefaultForceUnit = ForceUnit;
	ms_nDefaultLengthUnit = LengthUnit;
}

void MSUnit::GetDefaultUnits( long& ForceUnit, long& LengthUnit )
{
	ForceUnit = ms_nDefaultForceUnit;
	LengthUnit = ms_nDefaultLengthUnit;
}