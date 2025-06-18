#include "stdafx.h"
#include "MSObject.h"
#include "SlabHorSupportRebarsTemplate.h"
#include "GMLib/MSVersionInfo.h"

SlabHorSupportRebarsTemplate::SlabHorSupportRebarsTemplate()
{
	DataInit();
}


SlabHorSupportRebarsTemplate::~SlabHorSupportRebarsTemplate()
{
}
void SlabHorSupportRebarsTemplate::DataInit()
{
	m_bSupportRebarsSet = false;
	m_nSupportRebarsDiaIndex = 0;
	m_nSupportRebarsLengthType = 0;
	m_dSupportRebarsTotalLengthRate = 2.0;
	m_nSupportRebarsOrderLength = 8000;
	m_nSupportRebarsSpliceLength = 300;
	m_nSupportRebarsCorrectiveLength = 10;
	m_bUserSteelGradeS = false;
}
void SlabHorSupportRebarsTemplate::CopyFromMe(SlabHorSupportRebarsTemplate* pRebarTemplate)
{
	m_bSupportRebarsSet = pRebarTemplate->GetSupportRebarsSet();
	m_nSupportRebarsDiaIndex = pRebarTemplate->GetSupportRebarsDiaIndex();
	m_nSupportRebarsLengthType = pRebarTemplate->GetSupportRebarsLengthType();
	m_dSupportRebarsTotalLengthRate = pRebarTemplate->GetSupportRebarsTotalLengthRate();
	m_nSupportRebarsOrderLength = pRebarTemplate->GetSupportRebarsOrderLength();
	m_nSupportRebarsSpliceLength = pRebarTemplate->GetSupportRebarsSpliceLength();
	m_nSupportRebarsCorrectiveLength = pRebarTemplate->GetSupportRebarsCorrectiveLength();
	m_bUserSteelGradeS = pRebarTemplate->GetUserSteelGradeS();
}
void SlabHorSupportRebarsTemplate::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_bSupportRebarsSet;
		ar << m_nSupportRebarsDiaIndex;
		ar << m_nSupportRebarsLengthType;
		ar << m_dSupportRebarsTotalLengthRate;
		ar << m_nSupportRebarsOrderLength;
		ar << m_nSupportRebarsSpliceLength;
		ar << m_nSupportRebarsCorrectiveLength;
		ar << m_bUserSteelGradeS;
	}
	else
	{
		ar >> m_bSupportRebarsSet;
		ar >> m_nSupportRebarsDiaIndex;
		ar >> m_nSupportRebarsLengthType;
		ar >> m_dSupportRebarsTotalLengthRate;
		ar >> m_nSupportRebarsOrderLength;
		ar >> m_nSupportRebarsSpliceLength;
		ar >> m_nSupportRebarsCorrectiveLength;
		if (MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231010)
			ar >> m_bUserSteelGradeS;
	}
}
BOOL SlabHorSupportRebarsTemplate::GetSupportRebarsSet()
{
	return m_bSupportRebarsSet;
}
long SlabHorSupportRebarsTemplate::GetSupportRebarsDiaIndex()
{
	return m_nSupportRebarsDiaIndex;
}
long SlabHorSupportRebarsTemplate::GetSupportRebarsLengthType()
{
	return m_nSupportRebarsLengthType;
}
double SlabHorSupportRebarsTemplate::GetSupportRebarsTotalLengthRate()
{
	return m_dSupportRebarsTotalLengthRate;
}
long SlabHorSupportRebarsTemplate::GetSupportRebarsOrderLength()
{
	return m_nSupportRebarsOrderLength;
}
long SlabHorSupportRebarsTemplate::GetSupportRebarsSpliceLength()
{
	return m_nSupportRebarsSpliceLength;
}
long SlabHorSupportRebarsTemplate::GetSupportRebarsCorrectiveLength()
{
	return m_nSupportRebarsCorrectiveLength;
}
BOOL SlabHorSupportRebarsTemplate::GetUserSteelGradeS()
{
	return m_bUserSteelGradeS;
}

void SlabHorSupportRebarsTemplate::SetSupportRebarsSet(BOOL value)
{
	m_bSupportRebarsSet = value;
}
void SlabHorSupportRebarsTemplate::SetSupportRebarsDiaIndex(long value)
{
	m_nSupportRebarsDiaIndex = value;
}
void SlabHorSupportRebarsTemplate::SetSupportRebarsLengthType(long value)
{
	m_nSupportRebarsLengthType = value;
}
void SlabHorSupportRebarsTemplate::SetSupportRebarsTotalLengthRate(double value)
{
	m_dSupportRebarsTotalLengthRate = value;
}
void SlabHorSupportRebarsTemplate::SetSupportRebarsOrderLength(long value)
{
	m_nSupportRebarsOrderLength = value;
}
void SlabHorSupportRebarsTemplate::SetSupportRebarsSpliceLength(long value)
{
	m_nSupportRebarsSpliceLength = value;
}
void SlabHorSupportRebarsTemplate::SetSupportRebarsCorrectiveLength(long value)
{
	m_nSupportRebarsCorrectiveLength = value;
}
void SlabHorSupportRebarsTemplate::SetUserSteelGradeS(BOOL value)
{
	m_bUserSteelGradeS = value;
}