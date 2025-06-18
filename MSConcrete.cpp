#include "StdAfx.h"
#include "MSConcrete.h"
#include "ReinforceBarSetting.h"
#include "MSRebarBuilding.h"
#include "MSGlobalOption.h"
#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSConcrete, MSMaterial, VERSIONABLE_SCHEMA | 1)
MSConcrete::MSConcrete(void) : MSMaterial()
{
	m_nType = msConcrete;
	m_ConcStrength = 0.;
	m_Es = 0.;
	m_ConcKind = _T("");
	m_UnitCostConc = 0.;
	m_UnitCostReBar = 0.;
	m_Aggregate = 25.;
	m_Slump = 150;
	m_pReinforceBarSetting = NULL;
}

MSConcrete::~MSConcrete(void)
{
}

MSObject* MSConcrete::Copy( bool bNewID /*= true*/ )
{
	MSConcrete* pNewObj;

	pNewObj = new MSConcrete();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;

}

void MSConcrete::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;

	MSConcrete* pSource = (MSConcrete*)pSource_in;
	m_ConcStrength = pSource->m_ConcStrength;
	m_ConcKind = pSource->m_ConcKind;
	m_UnitCostConc = pSource->m_UnitCostConc;
	m_UnitCostReBar = pSource->m_UnitCostReBar;
	m_Aggregate = pSource->m_Aggregate;
	m_Slump = pSource->m_Slump;
	m_pReinforceBarSetting = pSource->m_pReinforceBarSetting;

	MSMaterial::CopyFromMe(pSource, bNewID);
}

void MSConcrete::Dump(EFS::EFS_Buffer& buffer)
{
	MSMaterial::Dump(buffer);
	buffer << m_ConcStrength << m_Es << m_ConcKind << m_UnitCostConc << m_UnitCostReBar;
	buffer << m_Aggregate << m_Slump << &m_pReinforceBarSetting;
}

void MSConcrete::Recovery(EFS::EFS_Buffer& buffer)
{
	MSMaterial::Recovery(buffer);
	buffer >> m_ConcStrength >> m_Es >> m_ConcKind >> m_UnitCostConc >> m_UnitCostReBar;
	buffer >> m_Aggregate >> m_Slump >> &m_pReinforceBarSetting;
}

void MSConcrete::Serialize(CArchive &ar)
{
	MSMaterial::Serialize(ar);

	int ReinforceBarSettingID = 0;
	if(ar.IsStoring())
	{
		if(m_pReinforceBarSetting)
			ReinforceBarSettingID = m_pReinforceBarSetting->m_ID;
		ar << ReinforceBarSettingID;
		ar << m_ConcStrength;
		ar << m_Es;
		ar << m_ConcKind;
		ar << m_UnitCostConc;
		ar << m_UnitCostReBar;
		ar << m_Aggregate;
		ar << m_Slump;
	}
	else
	{
		ar >> ReinforceBarSettingID;
		m_pReinforceBarSetting = m_pBldg->GetReinforceBarSetting(ReinforceBarSettingID);
		if(m_pReinforceBarSetting == NULL)
			m_pReinforceBarSetting = m_pBldg->GetReinforceBarSetting(m_pBldg->GetDefaultReinforceBarSettingID());
		ar >> m_ConcStrength;
		ar >> m_Es;
		ar >> m_ConcKind;
		ar >> m_UnitCostConc;
		ar >> m_UnitCostReBar;
		ar >> m_Aggregate;
		ar >> m_Slump;
	}
}

void MSConcrete::GlobalOptionSerialize(CArchive &ar)
{
	MSMaterial::GlobalOptionSerialize(ar);

	int ReinforceBarSettingID = 0;
	if(ar.IsStoring())
	{
		if(m_pReinforceBarSetting)
			ReinforceBarSettingID = m_pReinforceBarSetting->m_GlobalID;
		ar << ReinforceBarSettingID;
		ar << m_ConcStrength;
		ar << m_Es;
		ar << m_ConcKind;
		ar << m_UnitCostConc;
		ar << m_UnitCostReBar;
		ar << m_Aggregate;
		ar << m_Slump;
	}
	else
	{
		ar >> ReinforceBarSettingID;
		m_pReinforceBarSetting = MSGlobalOption::GetInstance()->GetReinforceBarSetting(ReinforceBarSettingID);
		ar >> m_ConcStrength;
		ar >> m_Es;
		ar >> m_ConcKind;
		ar >> m_UnitCostConc;
		ar >> m_UnitCostReBar;
		ar >> m_Aggregate;
		ar >> m_Slump;

		// BH25-43 슬럼프 기본값 변경 15 -> 150, 글로벌 옵션만 강제로 변경하도록 처리
		if (MSVersionInfo::GetCurrentVersion() < 20250428)
		{
			m_Slump = 150;
		}
	}
}

CString MSConcrete::GetConcKind()
{
	return m_ConcKind;
}

CString MSConcrete::GetConcKindAggregateSlump()
{
	CString str;
	str.Format(_T("%g - C%d - %g"), m_Aggregate, (int)m_ConcStrength, m_Slump);
	return str;
}

double MSConcrete::GetFck()
{
	return m_ConcStrength;
}
CString MSConcrete::GetFckName()
{
	CString strFckName;
	strFckName.Format(_T("C%.0f"), m_ConcStrength);
	return strFckName;
}

MSReinforceBarSetting* MSConcrete::GetReinforceBarSetting()
{
	return m_pReinforceBarSetting;
}

bool MSConcrete::IsPlainConcreteReinforceBar()
{
	if (m_pReinforceBarSetting->m_ID == m_pBldg->GetPlainConcreteReinforceBarSettingID())
		return true;
	return false;
}
