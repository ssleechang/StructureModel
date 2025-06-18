#include "StdAfx.h"
#include "MSCompoM.h"
#include "MSRebarBuilding.h"
#include "MSPolySection.h"
#include "MRCompoM.h" //by LSS Temp

#include "MSRcBeamM.h"
#include "MSRcBraceM.h"
#include "MSRcColumnM.h"
#include "MSRcWallFoot.h"
#include "MSRcLevelDifM.h"
#include "MDRcHandRailM.h"
#include "MSRcRampM.h"
#include "MSRcBWallM.h"
#include "MSRcRWallM.h"
#include "MSRcSlabM.h"
#include "MSRcStairM.h"
#include "MSRcPitM.h"
#include "MDRcIsoFootM.h"
#include "MDRcStairLandingM.h"
#include "MDRcStairStepM.h"
#include "MDRcButtressM.h"
#include "MDCustomColumnM.h"
#include "MDCustomWallColumnM.h"
#include "MDCustomBeamM.h"

#include "GMLib/MSVersionInfo.h"

#include "MDSrcColumnM.h"
#include "MDSrcBeamM.h"
#include "MSRcLineMemberM.h"
#include "MSOpeningReinforceM.h"
#include "MDCustomLineM.h"

#include "MSModelChecker.h"
#include "ClassificationNameManager.h"

#ifndef YEAR_START
#define YEAR_START 2000
#endif

IMPLEMENT_SERIAL(MSCompoM, MSObject, VERSIONABLE_SCHEMA | 1)
long MSCompoM::ms_LastID = 0;
MSCompoM::MSCompoM(void)
{
	m_Name = _T("");
	m_ClassificationName = _T("");
	m_ShapeName = _T("");
	mp_Owner = NULL;
	mp_Material = NULL;
	m_nFromStory = 0;
	m_nToStory = 0;
	m_bDummy = FALSE;

	mp_PSec = NULL;
	mp_MRM = NULL;
    m_ID = 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// start of addition by ironbell
	//this->ProfileInstance = 0;
	//this->AggrBldgEleInstance = NULL;
	//// end of addition by ironbell
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_Inserted_date = CTime(YEAR_START,1,1,0,0,0);
	mp_RebarM = NULL;
	m_IsUsedStoryMaterial = true;
	m_IsDefault = false;
	m_nClassificationID = 0;
}

MSCompoM::~MSCompoM(void)
{
	if(mp_PSec)
		delete mp_PSec;
	if(mp_RebarM)
		delete mp_RebarM;
}

// m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
long MSCompoM::GetID()
{
	if(m_ID == 0) // 
	{
		m_ID = MSBaseBuilding::GetUID(UIDType::msCompoM);
		ms_LastID = m_ID;
		return m_ID;
	}

	return m_ID;
}

void MSCompoM::SetName(CString name)
{
    m_Name=name;
}

CString MSCompoM::GetName()
{
    return m_Name;
}


void MSCompoM::SetLastID( long NewID )
{
	if(NewID > ms_LastID)
		ms_LastID = NewID;
}

MSObject* MSCompoM::Copy(bool bNewID)
{
	if (mp_Owner == NULL)
		return NULL;

	MSCompoM* pNewCompoM = mp_Owner->CreateNewCompoM((long)GetType());
	pNewCompoM->CopyFromMe(this);
	return pNewCompoM;
}

void MSCompoM::CopyFromMe(MSCompoM* pSource)
{
	if(pSource==NULL)	return;
	m_Name				=	pSource->m_Name;
	m_ClassificationName = pSource->GetClassificationName();
	m_ShapeName = pSource->m_ShapeName;
	m_nType				= pSource->m_nType;
	mp_Owner			=	pSource->mp_Owner;
	mp_Material		=	pSource->mp_Material;
	m_nFromStory	=	pSource->m_nFromStory;
	m_nToStory		= pSource->m_nToStory;
	m_bDummy = pSource->m_bDummy;
	m_Inserted_date = pSource->m_Inserted_date;
//	mp_RebarM = pSource->mp_RebarM;
	if(pSource->mp_RebarM)
	{
		if(mp_RebarM == NULL)
			mp_RebarM = CreateRebarM(GetType());

		mp_RebarM->CopyFromMe(pSource->mp_RebarM);
	}
	m_IsUsedStoryMaterial = pSource->m_IsUsedStoryMaterial;
	m_nClassificationID = pSource->m_nClassificationID;
}

bool MSCompoM::IsGeometryDifferent(MSCompoM* compoM)
{
	bool ret = false;

	if (GetDepth() != compoM->GetDepth()) ret = true;
	else if (GetWidth() != compoM->GetWidth()) ret = true;
	else if (GetThick() != compoM->GetThick()) ret = true;
	else if (GetHeight() != compoM->GetHeight()) ret = true;
	else if (GetName() != compoM->GetName()) ret = true;

	MSRcLineMemberM* pMyRcLineMemberM = dynamic_cast<MSRcLineMemberM*>(this);
	MSRcLineMemberM* pUrRcLineMemberM = dynamic_cast<MSRcLineMemberM*>(compoM);

	if (pMyRcLineMemberM != nullptr && pUrRcLineMemberM != nullptr)
	{
		if (pMyRcLineMemberM->GetShape() != pUrRcLineMemberM->GetShape())
			ret = true;
	}

	// BH-4632 개구부 단면정보 중 콘크리트 물량산출 변경 시 슬래브 개구부형상 변경하기 위함
	MSOpeningReinforceM* pMyOpeningM = dynamic_cast<MSOpeningReinforceM*>(this);
	MSOpeningReinforceM* pUrOpeningM = dynamic_cast<MSOpeningReinforceM*>(compoM);
	if (pMyOpeningM != nullptr && pUrOpeningM != nullptr)
	{
		if (pMyOpeningM->GetIsCalcConcVolumn() != pUrOpeningM->GetIsCalcConcVolumn())
			ret = true;
	}
	
	return ret;
}

CTime MSCompoM::GetInsertedDate()
{
	return m_Inserted_date;
}
void MSCompoM::SetInsertedDate(CTime& time)
{
	m_Inserted_date = time;
}

void MSCompoM::Dump( EFS::EFS_Buffer& buffer )
{
	MSObject::Dump(buffer);
	buffer << m_Name << &mp_Material << m_nFromStory << m_nToStory << m_bDummy << m_Inserted_date << &mp_MRM;
	int nOne = 1, nZero = 0;
	if(mp_RebarM)
	{
		buffer << nOne;
		mp_RebarM->Dump(buffer);
	}
	else
		buffer << nZero;

	buffer << m_IsUsedStoryMaterial;
	buffer << m_IsDefault;
	buffer << m_ClassificationName;
	buffer << m_nClassificationID;
	buffer << m_ShapeName;
}

void MSCompoM::Recovery( EFS::EFS_Buffer& buffer )
{
	MSObject::Recovery(buffer);
	buffer >> m_Name >> &mp_Material >> m_nFromStory >> m_nToStory >> m_bDummy >> m_Inserted_date >> &mp_MRM;
	int nValue = 0;
	buffer >> nValue;
	if(nValue)
	{
		if(mp_RebarM == NULL)
			mp_RebarM = CreateRebarM(GetType());
		mp_RebarM->Recovery(buffer);
	}

	buffer >> m_IsUsedStoryMaterial;
	buffer >> m_IsDefault;
	buffer >> m_ClassificationName;
	buffer >> m_nClassificationID;
	buffer >> m_ShapeName;
}


void MSCompoM::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	long nOwnerID = 0;
	long nMaterialID = 0;
	long nMRCompoMID = 0;
	int nOne = 1;
	int nZero = 0;
	if(ar.IsStoring())
	{
		if(mp_Owner)	nOwnerID = mp_Owner->m_ID;
		if(mp_Material)	nMaterialID = mp_Material->m_ID;
		ar << (long)m_nType;
		ar << m_ID;
		ar << m_Name;
		ar << nOwnerID;
		ar << nMaterialID;
		ar << m_nFromStory;
		ar << m_nToStory;
		ar << m_bDummy;
		if(mp_MRM)	
		{
			ar << mp_MRM->m_ID;
			mp_MRM->Serialize(ar);
		}
		else
			ar << nMRCompoMID;

		ar << m_Inserted_date;
		if(mp_RebarM)
		{
			ar << nOne;
			mp_RebarM->Serialize(ar);
		}
		else
			ar << nZero;

		ar << m_IsUsedStoryMaterial;
		ar << m_IsDefault;

		ar << m_ClassificationName;
		ar << m_nClassificationID;
		ar << m_ShapeName;
	}
	else
	{
		long nType;
		ar >> nType;
//msBeamM=100, msBraceM, msBWallM, msColumnM, msIsoFootM, msRampM, msRWallM, msSlabM, msStairM, msWallFootM};
// 		if(nType==100)			m_nType = msBeamM;
// 		else if(nType==101)	m_nType = msBraceM;
// 		else if(nType==102)	m_nType = msBWallM;
// 		else if(nType==103)	m_nType = msColumnM;
// 		else if(nType==104)	m_nType = msIsoFootM;
// 		else if(nType==105)	m_nType = msRampM;
// 		else if(nType==106)	m_nType = msRWallM;
// 		else if(nType==107)	m_nType = msSlabM;
// 		else if(nType==108)	m_nType = msStairM;
// 		else if(nType==109)	m_nType = msWallFootM;
		m_nType = (MSCompoM::Type_MSCompoM)nType;
		
		ar >> m_ID;
		ar >> m_Name;
		ar >> nOwnerID;
		ar >> nMaterialID;
		if (mp_Owner)
		{
			mp_Material = mp_Owner->GetMaterial(nMaterialID);
			if (mp_Material == NULL)
				mp_Material = mp_Owner->GetDefMaterial(GetMaterialType());
		}
		ar >> m_nFromStory;
		ar >> m_nToStory;

		ar >> m_bDummy;
		ar >> nMRCompoMID;
		if(nMRCompoMID != 0)
		{
			mp_MRM = new MRCompoM;
			mp_MRM->mp_MSM = this;
			mp_MRM->Serialize(ar);
		}
		ar >> m_Inserted_date;

		int tempData = 0;
		ar >> tempData;
		if( tempData != 0)
		{
			mp_RebarM = CreateRebarM(m_nType);
			mp_RebarM->Serialize(ar);
		}

		// default : m_IsUsedStoryMaterial는 true 이전 데이터는 false로 설정
		if(MSVersionInfo::GetCurrentVersion() >= 20150717)
		{
			ar >> m_IsUsedStoryMaterial;
		}
		else
			m_IsUsedStoryMaterial = false;

		if (MSVersionInfo::GetCurrentVersion() < 20161214)
		{
			if (m_nType == msColumnHeaderM && m_Name == _T("NONE"))
				m_Name = _T("_NONE");
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20220321)
		{
			ar >> m_IsDefault;
		}
		else
			m_IsDefault = false;

		SetLastID(m_ID);

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20230217)
		{
			ar >> m_ClassificationName;
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231207 && MSVersionInfo::GetCurrentVersion() != 20231217)
		{
			ar >> m_nClassificationID;
		}
		
		if (IsMVAddToBHQ())
		{
			ar >> m_ShapeName;
		}
	}
}


bool MSCompoM::IsRC()
{
	if (m_nType >= msBeamM && m_nType < msSteelBeamM ||
		m_nType >= msCustomColumnM && m_nType <= msCustomButtressM)
		return true;

	return false;
}

bool MSCompoM::IsStoryIncluded(long nStory)
{
	if(m_nFromStory <= nStory && nStory <= m_nToStory)
		return TRUE;
	return FALSE;
}

bool MSCompoM::IsVerticalMember()
{
	switch (GetType())
	{
	case msBWallM:
	case msRWallM:
	case msButtressM:
	case msColumnM:
	case msPcColumnM:
	case msSteelColumnM:
	case msSrcColumnM:

	case msStairLandingM:
	case msStairStepM:
		return true;
	default:
		return false;
	}
}

MDMember* MSCompoM::CreateRebarM(Type_MSCompoM nType)
{
	MDMember* pRebarM = NULL;
	switch(nType)
	{
	case MSCompoM::msBeamM:	pRebarM = new MSRcBeamM();	break;
	case MSCompoM::msBraceM:	pRebarM = new MSRcBraceM();	break;
	case MSCompoM::msColumnM:	pRebarM = new MSRcColumnM();	break;
	case MSCompoM::msButtressM:	pRebarM = new MDRcButtressM();	break;
	case MSCompoM::msWallFootM:	pRebarM = new MSRcWallFootM();	break;
	case MSCompoM::msLevelDifM:	pRebarM = new MSRcLevelDifM();		break;
	case MSCompoM::msHandRailM:	pRebarM = new MDRcHandRailM();	break;
	case MSCompoM::msRampM:		pRebarM = new MSRcRampM();	break;
	case MSCompoM::msBWallM:		pRebarM = new MSRcBWallM();	break;
	case MSCompoM::msRWallM:		pRebarM = new MSRcRWallM();	break;
	case MSCompoM::msSlabM:			pRebarM = new MSRcSlabM();	break;
	case MSCompoM::msStairLandingM:	pRebarM = new MDRcStairLandingM();	break;
	case MSCompoM::msStairStepM:			pRebarM = new MDRcStairStepM();		break;
	case MSCompoM::msPitM:				pRebarM = new MSRcPitM();		break;
	case MSCompoM::msIsoFootM:	pRebarM = new MDRcIsoFootM();	break;
	case MSCompoM::msPcColumnM:	pRebarM = new MSRcColumnM();	break;	// TODO : 나중에 변경되면 수정해야 함
	case MSCompoM::msPcBeamM:	pRebarM = new MSRcBeamM();	break;
	case MSCompoM::msSrcColumnM: pRebarM = new MDSrcColumnM();	break;
	case MSCompoM::msSrcBeamM: pRebarM = new MDSrcBeamM();	break;
	//case MSCompoM::msSteelColumnM: pRebarM = new MDSteelBeamM();	break;
	//case MSCompoM::msSteelBeamM: pRebarM = new MDSrcColumnM();	break;
	case MSCompoM::msCustomColumnM:	pRebarM = new MDCustomColumnM();	break;
	case MSCompoM::msCustomWallColumnM:	pRebarM = new MDCustomWallColumnM();	break;
	case MSCompoM::msCustomBeamM:	pRebarM = new MDCustomBeamM();	break;
	case MSCompoM::msFWBeamM:	pRebarM = new MSRcBeamM();	break;
	case MSCompoM::msFWColumnM:	pRebarM = new MSRcColumnM();	break;
	case MSCompoM::msFWBWallM:		pRebarM = new MSRcBWallM();	break;
	case MSCompoM::msFWSlabM:			pRebarM = new MSRcSlabM();	break;
	}
	return pRebarM;
}

MDMember* MSCompoM::GetRebarM()
{
	if(mp_RebarM ==NULL)
		mp_RebarM = MSCompoM::CreateRebarM(GetType());
	return mp_RebarM;
}

MSMaterial* MSCompoM::GetMaterial()
{
	if (mp_Material == nullptr)
	{
		MSBaseBuilding* pbuilding = GetBuilding();
		if (pbuilding == nullptr)
			return nullptr;

		return pbuilding->GetDefMaterial();
	}

	return mp_Material;
}

MSMaterial::Type_MSMaterial MSCompoM::GetMaterialType()
{
	if (IsSteel())
		return MSMaterial::msSteel;
	else if (IsSRC())
		return MSMaterial::msSRC;
	else
	{
		MSRebarBuilding* pRebarBuilding = dynamic_cast<MSRebarBuilding*>(GetBuilding());
		if (pRebarBuilding != nullptr)
			return MSMaterial::msConcrete;
		else
			return MSMaterial::msFormWork;
	}
}

bool MSCompoM::IsExistedSpacialCharacters(CString name)
{
	vector<char> spacialCharacters;
	spacialCharacters.push_back('\\');
	spacialCharacters.push_back('/');
	spacialCharacters.push_back(':');
	spacialCharacters.push_back('*');
	spacialCharacters.push_back('?');
	spacialCharacters.push_back('"');
	spacialCharacters.push_back('<');
	spacialCharacters.push_back('>');
	spacialCharacters.push_back('|');

	for(int i = 0; i < spacialCharacters.size(); i++)
	{
		if(name.Find(spacialCharacters[i]) != -1)
			return true;
	}

	return false;
}

CString MSCompoM::ReplaceSpacialCharactersName(CString name, char replacer)
{
	//윈도우 파일에서 사용 할 수 없는 특수 문자(\/:*?"<>|) 치환
	name.Replace('\\',replacer);
	name.Replace('/',replacer);
	name.Replace(':',replacer);
	name.Replace('*',replacer);
	name.Replace('?',replacer);
	name.Replace('"',replacer);
	name.Replace('<',replacer);
	name.Replace('>',replacer);
	name.Replace('|',replacer);

	name.Replace(_T(" "), _T(""));
	return name;
}

CString MSCompoM::ToString()
{
	CString str;
	str.Format(L"Name:%s ID:%d, MType:%d, ", GetName(), m_ID, GetType());

	return str;
}
CString MSCompoM::GetClassificationName()
{
	ClassificationNameData* data = ClassificationNameManager::GetInstance()->GetClassificationNameDataByID(m_nClassificationID);
	if (data != nullptr)
		m_ClassificationName = data->m_sName;
	else
		m_ClassificationName = "";

	return m_ClassificationName;
}
//void MSCompoM::SetClassificationName(CString value)
//{
//	m_ClassificationName = value;
//}

void MSCompoM::SetClassificationName(long nId)
{
	m_nClassificationID = nId;
}

bool MSCompoM::CheckData()
{
	return MSModelChecker::CheckModel_MSCompoM(this);
}

bool MSCompoM::IsMainCompoM(Type_MSCompoM type)
{
	if (type == MSCompoM::msFWOpeningM)	return false;
	return true;
}

bool MSCompoM::IsTransfer()
{
	MDMember* pMDMember = GetRebarM();
	if (pMDMember == nullptr)
		return false;
	return pMDMember->IsTransfer();
}