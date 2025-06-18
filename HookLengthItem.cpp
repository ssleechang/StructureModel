#include "StdAfx.h"
#include "HookLengthItem.h"


HookLengthItem::HookLengthItem(void)
{
	Initialize();
}


HookLengthItem::~HookLengthItem(void)
{
}

void HookLengthItem::Serialize( CArchive& ar )
{
	if(ar.IsStoring())
	{
		ar << m_dMainbarHookLength90;
		ar << m_dMainbarHookLength180;
		ar << m_dStirrupHookLength90;
		ar << m_dStirrupHookLength135;
	}
	else
	{
		ar >> m_dMainbarHookLength90;
		ar >> m_dMainbarHookLength180;
		ar >> m_dStirrupHookLength90;
		ar >> m_dStirrupHookLength135;
	}

}

void HookLengthItem::Initialize()
{
	m_dMainbarHookLength90 = 0.;
	m_dMainbarHookLength180 = 0.;
	m_dStirrupHookLength90 = 0.;
	m_dStirrupHookLength135 = 0.;
}

double HookLengthItem::GetHookLength( bool isStirrup /*= false*/, double dDegree /*= 90*/ ) const
{
	if(isStirrup)
	{
		if(fabs(dDegree-135) < DTOL1)
			return m_dStirrupHookLength135;
		else
			return m_dStirrupHookLength90;

	}
	else
	{
		if(fabs(dDegree-180) < DTOL1)
			return m_dMainbarHookLength180;
		else
			return m_dMainbarHookLength90;

	}
	return m_dMainbarHookLength90;

}

void HookLengthItem::Copy(HookLengthItem desHookLengthItem)
{
	m_dMainbarHookLength90 = desHookLengthItem.m_dMainbarHookLength90;
	m_dMainbarHookLength180 = desHookLengthItem.m_dMainbarHookLength180;
	m_dStirrupHookLength90 = desHookLengthItem.m_dStirrupHookLength90;
	m_dStirrupHookLength135 = desHookLengthItem.m_dStirrupHookLength135;
}