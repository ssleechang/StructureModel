#include "StdAfx.h"
#include "MSStandardRibTypeM.h"

IMPLEMENT_SERIAL(MSStandardRibTypeM, MSRibTypeM, VERSIONABLE_SCHEMA | 1)
MSStandardRibTypeM::MSStandardRibTypeM(void)
{
	m_RibType = Type_MSRib::Rib_Standard;
	m_ParamA = 100;
	m_ParamB = 100;
	m_ParamC = 70;
}

MSStandardRibTypeM::~MSStandardRibTypeM(void)
{
}

void MSStandardRibTypeM::CopyFromMe(MSStandardRibTypeM* pSource)
{
	if(pSource==NULL)	return;
	m_ParamA = pSource->GetParamA();
	m_ParamB = pSource->GetParamB();
	m_ParamC = pSource->GetParamC();
}

void MSStandardRibTypeM::Dump( EFS::EFS_Buffer& buffer )
{
	MSRibTypeM::Dump(buffer);
	buffer << m_ParamA << m_ParamB << m_ParamC;
}

void MSStandardRibTypeM::Recovery( EFS::EFS_Buffer& buffer )
{
	MSRibTypeM::Recovery(buffer);
	buffer >> m_ParamA >> m_ParamB >> m_ParamC;
}
void MSStandardRibTypeM::Serialize(CArchive &ar)
{
	MSRibTypeM::Serialize(ar);
	if(ar.IsStoring())
	{
		ar >> m_ParamA;
		ar >> m_ParamB;
		ar >> m_ParamC;
	}
	else
	{
		ar << m_ParamA;
		ar << m_ParamB;
		ar << m_ParamC;

		SetLastID(m_ID);
	}
}

