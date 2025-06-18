#include "stdafx.h"
#include "ClassificationNameData.h"
#include "ClassificationNameManager.h"

IMPLEMENT_SERIAL(ClassificationNameData, MSObject, VERSIONABLE_SCHEMA | 1)

ClassificationNameData::ClassificationNameData(void)
{
}

ClassificationNameData::~ClassificationNameData(void)
{
}

MSObject* ClassificationNameData::Copy(bool bNewID /*= true*/)
{
	ClassificationNameData* pNewObj;

	pNewObj = new ClassificationNameData();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;

}

void ClassificationNameData::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;

	ClassificationNameData* Source = (ClassificationNameData*)pSource_in;

	m_nID = Source->m_nID;
	m_sName = Source->m_sName;

	MSObject::CopyFromMe(Source, bNewID);
}

void ClassificationNameData::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	buffer << m_nID;
	buffer << m_sName;
}

void ClassificationNameData::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);

	buffer >> m_nID;
	buffer >> m_sName;
}

void ClassificationNameData::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_nID;
		ar << m_sName;
		ar << (int)compoMType;
		ar << (int)materialType;
	}
	else
	{
		ar >> m_nID;
		ar >> m_sName;
		int type = 0;
		ar >> type;
		compoMType = (MSCompoM::Type_MSCompoM)type;
		ar >> type;
		materialType = (MSMaterial::Type_MSMaterial)type;
		ClassificationNameManager::SetLastID(m_nID);
	}
}
