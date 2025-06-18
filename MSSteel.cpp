#include "StdAfx.h"
#include "MSSteel.h"
#include "GMLib\MSVersionInfo.h"

IMPLEMENT_SERIAL(MSSteel, MSMaterial, VERSIONABLE_SCHEMA | 1)
MSSteel::MSSteel(void)
{
	m_nType = msSteel;
	m_Fy = 0;
	m_Code = STL_CODE_KS;
	m_Grade = STL_GRADE_SS400;
}

MSSteel::~MSSteel(void)
{
}

MSObject* MSSteel::Copy( bool bNewID /*= true*/ )
{
	MSSteel* pNewObj;

	pNewObj = new MSSteel();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;

}

void MSSteel::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;

	MSSteel* pSource = (MSSteel*)pSource_in;
	m_Fy = pSource->m_Fy;
	m_Code = pSource->m_Code;
	m_Grade = pSource->m_Grade;

	MSMaterial::CopyFromMe(pSource, bNewID);
}

void MSSteel::Dump(EFS::EFS_Buffer& buffer)
{
	MSMaterial::Dump(buffer);
	buffer << m_Fy;
	int code = (int)m_Code;
	int grade = (int)m_Grade;
	buffer << code;
	buffer << grade;
}

void MSSteel::Recovery(EFS::EFS_Buffer& buffer)
{
	MSMaterial::Recovery(buffer);
	buffer >> m_Fy;
	int code = 0, grade = 0;
	buffer >> code;
	buffer >> grade;

	m_Code = (STEEL_CODE)code;
	m_Grade = (STEEL_GRADE)grade;
}

void MSSteel::Serialize(CArchive &ar)
{
	MSMaterial::Serialize(ar);
	int code = 0, grade = 0;
	if(ar.IsStoring())
	{
		ar <<		m_Fy;

		code = (int)m_Code;
		grade = (int)m_Grade;
		ar << code;
		ar << grade;
	}
	else
	{
		ar >>		m_Fy;

		if(MSVersionInfo::GetCurrentVersion() >= 20150721)
		{
			ar >> code;
			ar >> grade;

			m_Code = (STEEL_CODE)code;
			m_Grade = (STEEL_GRADE)grade;
		}
	}
}

void MSSteel::GlobalOptionSerialize(CArchive &ar)
{
	MSMaterial::GlobalOptionSerialize(ar);

	if(ar.IsStoring())
	{
		ar <<		m_Fy;
	}
	else
	{
		ar >>		m_Fy;
	}
}