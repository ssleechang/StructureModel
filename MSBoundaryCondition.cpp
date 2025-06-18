#include "StdAfx.h"
#include "MSBoundaryCondition.h"


IMPLEMENT_SERIAL(MSBoundaryCondition, MSObject, VERSIONABLE_SCHEMA | 1)
MSBoundaryCondition::MSBoundaryCondition(void)
{
	m_Condition = msShear;
}

MSBoundaryCondition::~MSBoundaryCondition(void)
{
}

MSObject* MSBoundaryCondition::Copy( bool bNewID /*= true*/ )
{
	MSBoundaryCondition* pNewObj = new MSBoundaryCondition();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSBoundaryCondition::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/ )
{
	if(pSource_in == NULL)	return;
	MSBoundaryCondition* TheSource = (MSBoundaryCondition*)pSource_in;

	m_Condition = TheSource->m_Condition;
}

void MSBoundaryCondition::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);

	int condition = (int)m_Condition;
	buffer << condition;
}

void MSBoundaryCondition::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);

	int condition = 0;
	buffer >> condition;
	m_Condition = (Type_Condition)condition;
}

void MSBoundaryCondition::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	int condition = 0;
	if(ar.IsStoring())
	{
		condition = (int)m_Condition;
		ar << condition;
	}
	else
	{
		ar >> condition;
		m_Condition = (Type_Condition)condition;
	}
}
