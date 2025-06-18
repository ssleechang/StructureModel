#include "StdAfx.h"
#include "MSStoryMaterial.h"
#include "MSRebarBuilding.h"


IMPLEMENT_SERIAL(MSStoryMaterial, MSElement, VERSIONABLE_SCHEMA | 1)
MSStoryMaterial::MSStoryMaterial(void)
{
	m_pBldg = NULL;
	m_FloorNum = 0;
	m_RcHMaterialID = m_RcVMaterialID = 0;
	m_SteelHMaterialID = m_SteelVMaterialID = 0;
	m_SrcHMaterialID = m_SrcVMaterialID = 0;
}

MSStoryMaterial::~MSStoryMaterial(void)
{
}

MSObject* MSStoryMaterial::Copy( bool bNewID /*= true*/ )
{
	MSStoryMaterial* pNewObj;

	pNewObj = new MSStoryMaterial();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;

}

void MSStoryMaterial::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;

	MSStoryMaterial* pSource = (MSStoryMaterial*)pSource_in;
	m_FloorNum = pSource->m_FloorNum;
	m_RcHMaterialID = pSource->m_RcHMaterialID;
	m_RcVMaterialID = pSource->m_RcVMaterialID;
	m_SteelHMaterialID = pSource->m_SteelHMaterialID;
	m_SteelVMaterialID = pSource->m_SteelVMaterialID;
	m_SrcHMaterialID = pSource->m_SrcHMaterialID;
	m_SrcVMaterialID = pSource->m_SrcVMaterialID;

	MSElement::CopyFromMe(pSource, bNewID);
}

void MSStoryMaterial::Dump(EFS::EFS_Buffer& buffer)
{
	MSElement::Dump(buffer);
	buffer << m_FloorNum;
	buffer << m_RcHMaterialID << m_RcVMaterialID ;
	buffer << m_SteelHMaterialID << m_SteelVMaterialID ;
	buffer << m_SrcHMaterialID << m_SrcVMaterialID ;
}

void MSStoryMaterial::Recovery(EFS::EFS_Buffer& buffer)
{
	MSElement::Recovery(buffer);
	buffer >> m_FloorNum;
	buffer >> m_RcHMaterialID >> m_RcVMaterialID ;
	buffer >> m_SteelHMaterialID >> m_SteelVMaterialID ;
	buffer >> m_SrcHMaterialID >> m_SrcVMaterialID ;
}

void MSStoryMaterial::Serialize(CArchive &ar)
{
	MSElement::Serialize(ar);

	CString temp = _T("");
	if(ar.IsStoring())
	{
		ar <<		m_FloorNum;
		ar <<       m_RcHMaterialID;
		ar <<		m_RcVMaterialID;
		ar <<		m_SteelHMaterialID;
		ar <<		m_SteelVMaterialID;
		ar <<		m_SrcHMaterialID;
		ar <<		m_SrcVMaterialID;
	}
	else
	{
		ar >>		m_FloorNum;
		ar >>       m_RcHMaterialID;
		ar >>		m_RcVMaterialID;
		ar >>		m_SteelHMaterialID;
		ar >>		m_SteelVMaterialID;
		ar >>		m_SrcHMaterialID;
		ar >>		m_SrcVMaterialID;
	}
}

MSMaterial* MSStoryMaterial::GetMaterial(MSMaterial::Type_MSMaterial matType, bool IsVerticalMember)
{
	int materialID = m_RcHMaterialID;
	if(matType == MSMaterial::msConcrete)
		materialID = GetRcMaterialID(IsVerticalMember);
	else if(matType == MSMaterial::msSteel)
		materialID = GetSteelMaterialID(IsVerticalMember);
	else if(matType == MSMaterial::msSRC)
		materialID = GetSrcMaterialID(IsVerticalMember);
	
	return m_pBldg->GetMaterial(materialID);
}

int MSStoryMaterial::GetRcMaterialID(bool IsVerticalMember)
{
	if(IsVerticalMember)	return m_RcVMaterialID;
	else                                return m_RcHMaterialID;
}

int MSStoryMaterial::GetSteelMaterialID(bool IsVerticalMember)
{
	if(IsVerticalMember)	return m_SteelVMaterialID;
	else                                return m_SteelHMaterialID;
}

int MSStoryMaterial::GetSrcMaterialID(bool IsVerticalMember)
{
	if(IsVerticalMember)	return m_SrcVMaterialID;
	else                                return m_SrcHMaterialID;
}

bool MSStoryMaterial::IsUsedMaterial(int materialID)
{
	if(m_RcHMaterialID == materialID || m_RcVMaterialID == materialID)
		return true;
	if(m_SteelHMaterialID == materialID || m_SteelVMaterialID == materialID)
		return true;
	if(m_SrcHMaterialID == materialID || m_SrcVMaterialID == materialID)
		return true;
	return false;
}


