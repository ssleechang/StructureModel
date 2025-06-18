#include "StdAfx.h"
#include "MSSrcMaterial.h"
#include "MSRebarBuilding.h"
#include "MSConcrete.h"
#include "MSSteel.h"
#include "MSGlobalOption.h"
IMPLEMENT_SERIAL(MSSrcMaterial, MSMaterial, VERSIONABLE_SCHEMA | 1)

MSSrcMaterial::MSSrcMaterial(void)
{
	m_nType = MSMaterial::msSRC;
	mp_RcMaterial = NULL;
	mp_SteelMaterial = NULL;
	m_nSerializeRcID = 0;
	m_nSerializeSteelID = 0;
}

MSSrcMaterial::~MSSrcMaterial(void)
{
}


MSObject* MSSrcMaterial::Copy( bool bNewID /*= true*/ )
{
	MSSrcMaterial* pNewObj;

	pNewObj = new MSSrcMaterial();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;

}

void MSSrcMaterial::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;

	MSSrcMaterial* pSource = (MSSrcMaterial*)pSource_in;
	mp_RcMaterial = pSource->mp_RcMaterial;
	mp_SteelMaterial = pSource->mp_SteelMaterial;

	MSMaterial::CopyFromMe(pSource, bNewID);
}
void MSSrcMaterial::Dump(EFS::EFS_Buffer& buffer)
{
	MSMaterial::Dump(buffer);
	buffer << mp_RcMaterial;
	buffer << mp_SteelMaterial;
}

void MSSrcMaterial::Recovery(EFS::EFS_Buffer& buffer)
{
	MSMaterial::Recovery(buffer);
	buffer >> mp_RcMaterial;
	buffer >> mp_SteelMaterial;
}

void MSSrcMaterial::Serialize(CArchive &ar)
{
	MSMaterial::Serialize(ar);

	if(ar.IsStoring())
	{
		long RcMateID = 0;
		long StlMateID = 0;

		if(mp_RcMaterial)
			RcMateID = mp_RcMaterial->m_ID;
		if(mp_SteelMaterial)
			StlMateID = mp_SteelMaterial->m_ID;
		ar << RcMateID;
		ar << StlMateID;
	}
	else
	{
		ar >> m_nSerializeRcID;
		ar >> m_nSerializeSteelID;
	}
}

void MSSrcMaterial::GlobalOptionSerialize(CArchive &ar)
{
	MSMaterial::GlobalOptionSerialize(ar);
	
	if(ar.IsStoring())
	{
		long RcMateID = 0;
		long StlMateID = 0;

		if(mp_RcMaterial)
			RcMateID = mp_RcMaterial->m_GlobalID;
		if(mp_SteelMaterial)
			StlMateID = mp_SteelMaterial->m_GlobalID;
		ar << RcMateID;
		ar << StlMateID;
	}
	else
	{
		ar >> m_nSerializeRcID;
		ar >> m_nSerializeSteelID;
	}
}

void MSSrcMaterial::SetBuildingMaterialBySerializeID()
{
	MSConcrete* msConcrete = dynamic_cast<MSConcrete*>(m_pBldg->GetMaterial(m_nSerializeRcID));
	MSSteel* msSteel = dynamic_cast<MSSteel*>(m_pBldg->GetMaterial(m_nSerializeSteelID));

	if (msConcrete == nullptr)
	{
		msConcrete = dynamic_cast<MSConcrete*>(m_pBldg->GetDefMaterial(MSMaterial::Type_MSMaterial::msConcrete));
		m_nSerializeRcID = msConcrete->GetID();
	}
	
	if (msSteel == nullptr)
	{
		msSteel = dynamic_cast<MSSteel*>(m_pBldg->GetDefMaterial(MSMaterial::Type_MSMaterial::msSteel));
		m_nSerializeSteelID = msSteel->GetID();
	}
	
	mp_RcMaterial = msConcrete;
	mp_SteelMaterial = msSteel;
}

void MSSrcMaterial::SetGlobalOptionMaterialBySerializeID()
{
	MSConcrete* msConcrete = dynamic_cast<MSConcrete*>(MSGlobalOption::GetInstance()->GetMaterial(m_nSerializeRcID));
	MSSteel* msSteel = dynamic_cast<MSSteel*>(MSGlobalOption::GetInstance()->GetMaterial(m_nSerializeSteelID));

	if (msConcrete == nullptr)
	{
		msConcrete = dynamic_cast<MSConcrete*>(m_pBldg->GetDefMaterial(MSMaterial::Type_MSMaterial::msConcrete));
		m_nSerializeRcID = msConcrete->GetID();
	}
	
	if (msSteel == nullptr)
	{
		msSteel = dynamic_cast<MSSteel*>(m_pBldg->GetDefMaterial(MSMaterial::Type_MSMaterial::msSteel));
		m_nSerializeSteelID = msSteel->GetID();
	}

	mp_RcMaterial = msConcrete;
	mp_SteelMaterial = msSteel;
}

double MSSrcMaterial::GetFy()
{
	if(mp_SteelMaterial)
		return mp_SteelMaterial->m_Fy;
	return 0.0;
}
void MSSrcMaterial::SetFy(double fy)
{
	if(mp_SteelMaterial)
		mp_SteelMaterial->m_Fy = fy;
}

double MSSrcMaterial::GetFck()
{
	return mp_RcMaterial->GetFck();
}
CString MSSrcMaterial::GetFckName()
{
	return mp_RcMaterial->GetFckName();
}

double MSSrcMaterial::GetConcStrength()
{
	return mp_RcMaterial->m_ConcStrength;
}

MSReinforceBarSetting* MSSrcMaterial::GetReinforceBarSetting()
{
	return mp_RcMaterial->GetReinforceBarSetting();
}
bool MSSrcMaterial::IsPlainConcreteReinforceBar()
{
	return mp_RcMaterial->IsPlainConcreteReinforceBar();
}

long MSSrcMaterial::GetSerializeRcID()
{
	return m_nSerializeRcID;
}

long MSSrcMaterial::GetSerializeSteelID()
{
	return m_nSerializeSteelID;
}

void MSSrcMaterial::SetSerializeRcID(long RcID)
{
	m_nSerializeRcID = RcID;
}

void MSSrcMaterial::SetSerializeSteelID(long SteelID)
{
	m_nSerializeSteelID = SteelID;
}
