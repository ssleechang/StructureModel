#include "StdAfx.h"
#include "MDMember.h"

IMPLEMENT_SERIAL(MDMember, MSObject, VERSIONABLE_SCHEMA | 1)

MDMember::MDMember(void)
{
	m_dCvrConc = 40;
}


MDMember::~MDMember(void)
{
}

void MDMember::CopyFromMe( MDMember* pSource )
{
	m_dCvrConc = pSource->m_dCvrConc;
}

void MDMember::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	if(ar.IsStoring())
	{
		ar << m_dCvrConc;
	}
	else
	{
		ar >> m_dCvrConc;
	}
}

void MDMember::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	buffer << m_dCvrConc;
}
void MDMember::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	buffer >> m_dCvrConc;
}

vector<double> MDMember::GetMainBarDiaAllLayer()
{
	vector<double> DiaIdxArr;
	return DiaIdxArr;
}
vector<double> MDMember::GetStirrupBarDiaAllLayer()
{
	vector<double> DiaIdxArr;
	return DiaIdxArr;
}
