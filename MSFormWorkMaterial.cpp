#include "stdafx.h"
#include "MSFormWorkMaterial.h"

IMPLEMENT_SERIAL(MSFormWorkMaterial, MSMaterial, VERSIONABLE_SCHEMA | 1)
MSFormWorkMaterial::MSFormWorkMaterial(void) : MSMaterial()
{
	m_nType = MSMaterial::msFormWork;
	m_eType = eFWMaterialType::Concrete;
}

MSFormWorkMaterial::~MSFormWorkMaterial(void)
{
}

MSObject* MSFormWorkMaterial::Copy(bool bNewID /*= true*/)
{
	MSFormWorkMaterial* pNewObj;

	pNewObj = new MSFormWorkMaterial();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;

}

void MSFormWorkMaterial::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;

	MSFormWorkMaterial* pSource = (MSFormWorkMaterial*)pSource_in;
	m_eType = pSource->GetFWMaterialType();
	MSMaterial::CopyFromMe(pSource, bNewID);
}

void MSFormWorkMaterial::Dump(EFS::EFS_Buffer& buffer)
{
	MSMaterial::Dump(buffer);
	int type = (int)m_eType;
	buffer << type;
}

void MSFormWorkMaterial::Recovery(EFS::EFS_Buffer& buffer)
{
	MSMaterial::Recovery(buffer);
	int type = 0;
	buffer >> type;
	m_eType = (eFWMaterialType)type;
}

void MSFormWorkMaterial::Serialize(CArchive &ar)
{
	MSMaterial::Serialize(ar);

	int type = 0;
	if (ar.IsStoring())
	{
		type = (int)m_eType;
		ar << type;
	}
	else
	{
		ar >> type;
		m_eType = (eFWMaterialType)type;
	}
}

void MSFormWorkMaterial::GlobalOptionSerialize(CArchive &ar)
{
	MSMaterial::GlobalOptionSerialize(ar);

	int type = 0;
	if (ar.IsStoring())
	{
		type = (int)m_eType;
		ar << type;
	}
	else
	{
		ar >> type;
		m_eType = (eFWMaterialType)type;
	}
}