#include "StdAfx.h"
#include "MSRibTypeM.h"

IMPLEMENT_SERIAL(MSRibTypeM, MSObject, VERSIONABLE_SCHEMA | 1)
long MSRibTypeM::ms_LastID = 0;
MSRibTypeM::MSRibTypeM(void)
{
	m_ID = 0;
	m_Name = _T("");
}


MSRibTypeM::~MSRibTypeM(void)
{
}

long MSRibTypeM::GetID()
{
	if(m_ID == 0) // 
	{
		m_ID = ms_LastID + 1;
		ms_LastID = m_ID;
		return m_ID;
	}

	return m_ID;
}

void MSRibTypeM::SetLastID( long NewID )
{
	if(NewID > ms_LastID)
		ms_LastID = NewID;
}

void MSRibTypeM::CopyFromMe(MSRibTypeM* pSource)
{
	if(pSource==NULL)	return;
	m_Name				=	pSource->m_Name;
	m_RibType = pSource->m_RibType;
}

void MSRibTypeM::Dump( EFS::EFS_Buffer& buffer )
{
	MSObject::Dump(buffer);
	buffer << m_Name;
	int RibType = (int)m_RibType;
	buffer << m_RibType;

}

void MSRibTypeM::Recovery( EFS::EFS_Buffer& buffer )
{
	MSObject::Recovery(buffer);
	buffer >> m_Name;
	int RibType = 0;
	buffer >> RibType;
	m_RibType = (Type_MSRib)RibType;
}

void MSRibTypeM::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	int RibType = 0;
	if(ar.IsStoring())
	{
		ar >> m_ID;
		ar >> m_Name;
		RibType = (int)m_RibType;
		ar >> RibType;
	}
	else
	{
		ar << m_ID;
		ar << m_Name;
		ar << RibType;
		m_RibType = (Type_MSRib)RibType;

		SetLastID(m_ID);
	}
}

