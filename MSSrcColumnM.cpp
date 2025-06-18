#include "StdAfx.h"
#include "MSSrcColumnM.h"

#include "MSRebarBuilding.h"
#include "MSSteelUtility.h"
#include "MSSteelSection.h"
#include "MSSrcMaterial.h"

#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSSrcColumnM, MSRcLineMemberM, VERSIONABLE_SCHEMA | 1)
MSSrcColumnM::MSSrcColumnM(void)
{
	m_nType = msSrcColumnM;
	
	m_pSection = NULL;
	m_nSecType = SECTION_H_TYPE;
	m_NumStudBoltBars = 4;
	m_StudBoltBars.InitData(MSDeformedBar::GetStirrupBarStr(300, 19));	
	m_StudBoltLength = 120;

	m_IsUsedStoryInnerConcMaterial = true;
	mp_InnerConcMaterial = NULL;
}

MSSrcColumnM::~MSSrcColumnM(void)
{
}

void MSSrcColumnM::CopyFromMe(MSCompoM* pSource)
{
	MSSrcColumnM* pSourceColumnM = (MSSrcColumnM*)pSource;
	m_pSection = pSourceColumnM->m_pSection;
	m_nSecType = pSourceColumnM->m_nSecType;

	m_NumStudBoltBars = pSourceColumnM->m_NumStudBoltBars;
	m_StudBoltBars.CopyFromMe(&pSourceColumnM->m_StudBoltBars);
	m_StudBoltLength = pSourceColumnM->m_StudBoltLength;
	MSRcLineMemberM::CopyFromMe(pSource);

	m_IsUsedStoryInnerConcMaterial = pSourceColumnM->m_IsUsedStoryInnerConcMaterial;
	mp_InnerConcMaterial = pSourceColumnM->mp_InnerConcMaterial;
}


void MSSrcColumnM::Dump(EFS::EFS_Buffer& buffer)
{
	MSRcLineMemberM::Dump(buffer);
	buffer << &m_pSection;
	buffer << m_nSecType;

	buffer << m_NumStudBoltBars;
	m_StudBoltBars.Dump(buffer);
	buffer << m_StudBoltLength;

	buffer << m_IsUsedStoryInnerConcMaterial;
	buffer << &mp_InnerConcMaterial;
}

void MSSrcColumnM::Recovery(EFS::EFS_Buffer& buffer)
{
	MSRcLineMemberM::Recovery(buffer);

	buffer >> &m_pSection;
	buffer >> m_nSecType;

	buffer >> m_NumStudBoltBars;
	m_StudBoltBars.Recovery(buffer);
	buffer >> m_StudBoltLength;

	buffer >> m_IsUsedStoryInnerConcMaterial;
	buffer >> &mp_InnerConcMaterial;
}

void MSSrcColumnM::Serialize(CArchive& ar )
{
	MSRcLineMemberM::Serialize(ar);
	long nSectionID = 0;
	if(ar.IsStoring())
	{
		if(m_pSection)	nSectionID = m_pSection->m_ID;
		ar << nSectionID;
		ar << m_nSecType;

		ar << m_NumStudBoltBars;
		m_StudBoltBars.Serialize(ar);
		ar << m_StudBoltLength;

		ar << m_IsUsedStoryInnerConcMaterial;
		
		int nInnerConcMaterialID = 0;
		if (mp_InnerConcMaterial != nullptr)
			nInnerConcMaterialID = mp_InnerConcMaterial->m_ID;
		ar << nInnerConcMaterialID;
	}
	else
	{
		ar >> nSectionID;
		ar >> m_nSecType;

		ar >> m_NumStudBoltBars;
		m_StudBoltBars.Serialize(ar);
		ar >> m_StudBoltLength;

		MSSteelUtility* pUtil = mp_Owner->GetSteelUtili();
		m_pSection = pUtil->GetSection(m_nSecType, nSectionID);
		if(m_pSection == NULL)
		{
			m_pSection = pUtil->GetDefaultHSection();
			CString Msg;	Msg.Format(_T("%s : %s"), ERROR_STEEL_SECTION_FILE, m_Name);
			StatusReporterWrapper::OutputWarning(Msg);

			MSSteelUtility::CreateErrorObject(MESSAGE_WARNING_CHANGED_DEFAULT_STEEL_SECTION, WARNING_LEVEL, Msg, XErrorObject::MSCompoMIDSelect);
		}

		if (IsMVAddToBHA())
		{
			// [BH-5832] SRC기둥 내 철골닫힌구간(ㅁ, O) 영역의 콘크리트 별도 설정
			int curVersion = MSVersionInfo::GetCurrentVersion();
			if (curVersion >= 20201124)
			{
				ar >> m_IsUsedStoryInnerConcMaterial;

				if (curVersion >= 20240925)
				{
					int nInnerConcMaterialID = 0;
					ar >> nInnerConcMaterialID;

					if (mp_Owner != nullptr)
						mp_InnerConcMaterial = dynamic_cast<MSSrcMaterial*>(mp_Owner->GetMaterial(nInnerConcMaterialID));

					if(mp_InnerConcMaterial == nullptr)
						mp_InnerConcMaterial = dynamic_cast<MSSrcMaterial*>(mp_Owner->GetDefMaterial(GetMaterialType()));
				}
				else
				{
					bool isUsedInnerConc = false;
					if (curVersion >= 20201210)
						ar >> isUsedInnerConc;

					//if (mp_InnerConcMaterial != nullptr)
					if (isUsedInnerConc)
					{
						mp_InnerConcMaterial = new MSSrcMaterial();
						mp_InnerConcMaterial->Serialize(ar);

						MSSrcMaterial* pFindMSSrcMaterial = dynamic_cast<MSSrcMaterial*>(mp_Owner->GetMaterialbyName(MSMaterial::Type_MSMaterial::msSRC, mp_InnerConcMaterial->GetName()));
						if (pFindMSSrcMaterial != nullptr)
						{
							delete mp_InnerConcMaterial;
							mp_InnerConcMaterial = pFindMSSrcMaterial;
						}
						else
						{
							mp_InnerConcMaterial->SetBuilding(mp_Owner);
							mp_InnerConcMaterial->SetBuildingMaterialBySerializeID();
							mp_Owner->Add(mp_InnerConcMaterial);
						}
					}
				}
			}
			else
			{
				m_IsUsedStoryInnerConcMaterial = true;
				mp_InnerConcMaterial = nullptr;
			}
		}


	}
}

bool MSSrcColumnM::IsGeometryDifferent(MSCompoM* compoM)
{
	if (MSRcLineMemberM::IsGeometryDifferent(compoM)) return true;

	MSSrcColumnM* srcM = dynamic_cast<MSSrcColumnM*>(compoM);
	if (srcM == NULL) return true;

	if (GetSteelSection()->IsGeometryDifferent(srcM->GetSteelSection())) return true;

	return false;
}

void MSSrcColumnM::GetSteelSecPolyline(GM2DPolyline& TheGon)
{
	m_pSection->GetSecPolyline(TheGon);
}

double MSSrcColumnM::GetSpecificTieHeight(CString sInitial)
{
	double dLen = 0;
	double dCover = GetCover();

	double dDist = 20;//Steel 과 rebar 사이간격
	double dWebThick = m_pSection->GetWThk();
	//차후 편심고려시켜야함.
	if (sInitial == _T("T"))
	{
		dLen = GetDepth() / 2 - dCover - m_pSection->GetH() / 2 - dDist;
	}
	else if (sInitial == _T("B"))
	{
		dLen = GetDepth() / 2 - dCover - m_pSection->GetH() / 2 - dDist;
	}
	else if (sInitial == _T("L"))
	{
		dLen = GetWidth() / 2 - dCover - dWebThick / 2 - dDist;
	}
	else if (sInitial == _T("R"))
	{
		dLen = GetWidth() / 2 - dCover - dWebThick / 2 - dDist;
	}

	return dLen;
}

void MSSrcColumnM::SetInnerConcMaterial(MSSrcMaterial* pMaterial)
{
	mp_InnerConcMaterial = pMaterial; 

	if (mp_InnerConcMaterial != nullptr)
	{
		if (mp_InnerConcMaterial->GetSerializeRcID() == 0 || mp_InnerConcMaterial->GetSerializeSteelID() == 0)
			mp_InnerConcMaterial = dynamic_cast<MSSrcMaterial*>(mp_Owner->GetMaterialbyName(MSMaterial::Type_MSMaterial::msSRC, mp_InnerConcMaterial->GetName()));
		else
		{
			mp_InnerConcMaterial->SetBuilding(mp_Owner);
			mp_InnerConcMaterial->SetBuildingMaterialBySerializeID();
		}
	}
}
