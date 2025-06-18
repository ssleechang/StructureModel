#include "StdAfx.h"
#include "MSBaseBuilding.h"

#include "GMLib\MSVersionInfo.h"
#include "MRRcSection.h"

#include "MSRcLineMemberM.h"
#include "MSRcHandRailM.h"
#include "MSRcFaceMemberM.h"
#include "MSStairM.h"
#include "MSRcIsoFootM.h"
#include "MSWindowM.h"
#include "MSDoorM.h"
#include "MSOpeningM.h"
#include "MSPcLineMemberM.h"
#include "MSOpeningReinforceM.h"
#include "MSWallOpeningExtraM.h"
#include "MSPcColumnM.h"
#include "MSSteelBeamM.h"
#include "MSSteelColumnM.h"
#include "MSSrcBeamM.h"
#include "MSSrcColumnM.h"
#include "MSColumnHeaderM.h"
#include "MSCustomColumnM.h"
#include "MSCustomWallColumnM.h"
#include "MSCustomBeamM.h"
#include "MSCustomColumnSection.h"
#include "MSCustomWallColumnSection.h"
#include "MSCustomBeamSection.h"
#include "MSRcHaunchM.h"

#include "MSConcrete.h"
#include "MSSteel.h"
#include "MSFormWorkMaterial.h"

#include "MSRibTypeM.h"
#include "MSStandardRibTypeM.h"
#include "MSWallEndRebarLine.h"
#include "MSSpacerShape.h"

#include "MSBuildingUCS.h"
#include "MSBuildingInfoManager.h"
#include "MSFormWorkTypeManager.h"
#include "MSQuantityOption.h"
#include "MSFloor.h"

#include "Radfcore_CLI/AppOptionsWrapper.h"
#include "Radfcore_CLI/AppGlobalOptionWrapper.h"
#include "ActivateLib_CLI/Activate.h"
#include "ActivateLib_CLI/AppLicenseWrapper.h"

#include "GMLib\TypeUidFactory.h"

IMPLEMENT_SERIAL(MSBaseBuilding, MSContainer, VERSIONABLE_SCHEMA | 1)

MSBaseBuilding* MSBaseBuilding::ms_ActiveBuilding = nullptr;

MSBaseBuilding::MSBaseBuilding(void)
{
	CString language(AppOptionsWrapper::Instance()->GetLanguage());

#ifndef _DEBUG 
	AppLicenseWrapper::Instance()->Restore();
	const wchar_t* cLKey = AppLicenseWrapper::Instance()->GetLicenseKey();
	CString strLKey = (CString)cLKey;

	const wchar_t* cAKey = AppLicenseWrapper::Instance()->GetActivationKey();
	CString strAKey = (CString)cAKey;

	const wchar_t* cName = AppLicenseWrapper::Instance()->GetCompanyName();
	CString strName = (CString)cName;

	int nVer = 0;
	if (language.MakeLower() == "ko-kr")
		Activate::Instance()->SetLanguageType(eLanguageType::ko_KR);
	else
		Activate::Instance()->SetLanguageType(eLanguageType::en_US);
	Activate::Instance()->SetAboutErrorMsg(true);
	int nErrorCode = Activate::Instance()->CheckLicense(strLKey, strAKey, strName, (CString)AppLicenseWrapper::Instance()->GetActUserName(), false);
	if (nErrorCode != 0)
	{
		Activate::Instance()->SetAboutErrorMsg(false);
		throw exception("License Error");
		return;
	}
	Activate::Instance()->SetAboutErrorMsg(false);
#endif

	if (language.MakeLower() == "ko-kr")
	{
		g_multilang.Create(LANG_KOREAN_PATH, LANG_ENGLISH_PATH);
	}
	/*else if (language.MakeLower() == "jp-JP")
	{
	g_multilang.Create(LANG_JAPANESE_PATH, LANG_ENGLISH_PATH);
	}*/
	else
		g_multilang.Create(LANG_ENGLISH_PATH, LANG_ENGLISH_PATH);

	StatusReporterWrapper::OutputMessage(CFormatMsg(LANG(TEXT_MSG_LANGUAGE_SET), language));
	MSElement::ms_LastID = 1;

	CoCreateGuid(&m_GUID);

	m_bIsNeedRegenAfterLoad = false;
	m_nUpdateModelVersion = MSVersionInfo::GetLatestVersion();
	m_strFilePath = _T("");

	mp_UCS = new MSBuildingUCS();
	mp_UCS_import = new MSBuildingUCS();
	m_pTypeUIDFactory = new TypeUIDFactory();
	m_nLicenseVersion = 0;

	mp_FormManager = new MSFormWorkTypeManager();
}

MSBaseBuilding::~MSBaseBuilding(void)
{
	DeleteRibTypes();

	TypeUIDFactory* pActTypeUIDFactory = TypeUIDFactory::GetActiveTypeUIDFactory();
	if(pActTypeUIDFactory == m_pTypeUIDFactory)
		TypeUIDFactory::SetActiveTypeUIDFactory(nullptr);

	delete mp_UCS;
	delete m_pTypeUIDFactory;
	delete mp_FormManager;
}

MRSection* MSBaseBuilding::CreateNewMRSection(long nType)
{
	MRSection* pSection = NULL;
	if(nType==0)	pSection = new MRRcSection();
	return pSection;
}
void MSBaseBuilding::Serialize(CArchive &ar)
{
	MSContainer::Serialize(ar);
	if(ar.IsStoring())
	{
		m_pTypeUIDFactory->Serialize(ar);
	}
	else
	{
// 		MSCompoM* pTestM = CreateNewCompoM(MSCompoM::msColumnHeaderM);
// 		pTestM->SetName(_T("CH1"));
// 		pTestM->SetOwner(this);
// 		pTestM->m_nFromStory = -this->GetNumUnderStory();
// 		pTestM->m_nToStory = this->GetNumAboveStory()-1;
// 		pTestM->SetMaterial(GetDefMaterial(MSMaterial::msConcrete));
// 		pTestM->GetID();
// 		Add(pTestM);

		//if (AppGlobalOptionWrapper::Instance()->GetLoadBldgMode() == AppGlobalOptionWrapper::CE
			//|| AppGlobalOptionWrapper::Instance()->GetLoadBldgMode() == AppGlobalOptionWrapper::Rebar && MSVersionInfo::GetCurrentVersion() >= 20240328)
		if(IsMVAddToBHQ())
			m_pTypeUIDFactory->Serialize(ar);
	}

}
CString MSBaseBuilding::GetGUIDString()
{
	wchar_t wszCLSID[129];
	StringFromGUID2(m_GUID, wszCLSID, 128);
	CString sGUID;
	sGUID.Format(_T("%s"), wszCLSID);
	return sGUID;
}

void MSBaseBuilding::SetID(int id)
{
	m_ID = id;
}

MSCompoM* MSBaseBuilding::CreateNewCompoM(long nType)
{
	MSCompoM* pCompoM = NULL;
	switch (nType)
	{
		//Line
	case MSCompoM::msBeamM:
	case MSCompoM::msBraceM:
	case MSCompoM::msColumnM:
	case MSCompoM::msButtressM:
	case MSCompoM::msWallFootM:
	case MSCompoM::msLevelDifM:
		pCompoM = new MSRcLineMemberM(/*nType*/);
		//		((MSRcLineMemberM*)pCompoM)->SetColumnHeaderMID(1);
		break;
	case MSCompoM::msHandRailM:
		pCompoM = new MSRcHandRailM();
		break;
		//face
	case MSCompoM::msRampM:
	case MSCompoM::msBWallM:
	case MSCompoM::msRWallM:
	case MSCompoM::msSlabM:
	case MSCompoM::msPitM:
		pCompoM = new MSRcFaceMemberM(/*nType*/);
		break;
	case MSCompoM::msStairLandingM:
	case MSCompoM::msStairStepM:
		pCompoM = new MSStairM;
		break;
	case MSCompoM::msIsoFootM:
		pCompoM = new MSRcIsoFootM();
		break;
	case MSCompoM::msWindowM:
		pCompoM = new MSWindowM();
		break;
	case MSCompoM::msDoorM:
		pCompoM = new MSDoorM();
		break;
	case MSCompoM::msWallOpeningM:
		pCompoM = new MSOpeningM();
		break;
	case MSCompoM::msPcBeamM:
		pCompoM = new MSPcLineMemberM();
		break;
	case MSCompoM::msOpeningReinforceM:
		pCompoM = new MSOpeningReinforceM();
		break;
	case MSCompoM::msOpeningExtraM:
		pCompoM = new MSWallOpeningExtraM();
		break;
	case MSCompoM::msPcColumnM:
		pCompoM = new MSPcColumnM();
		break;
	case MSCompoM::msSteelBeamM:
		pCompoM = new MSSteelBeamM();
		break;
	case MSCompoM::msSteelColumnM:
		pCompoM = new MSSteelColumnM();
		break;
	case MSCompoM::msSrcBeamM:
		pCompoM = new MSSrcBeamM();
		break;
	case MSCompoM::msSrcColumnM:
		pCompoM = new MSSrcColumnM();
		break;
	case MSCompoM::msColumnHeaderM:
		pCompoM = new MSColumnHeaderM();
		break;
	case MSCompoM::msCustomColumnM:
		pCompoM = new MSCustomColumnM();
		break;
	case MSCompoM::msCustomWallColumnM:
		pCompoM = new MSCustomWallColumnM();
		break;
	case MSCompoM::msCustomBeamM:
		pCompoM = new MSCustomBeamM();
		break;
	case MSCompoM::msHaunchM:
		pCompoM = new MSRcHaunchM();
		break;
	}
	if (pCompoM == NULL)	return NULL;

	pCompoM->m_nType = (MSCompoM::Type_MSCompoM)nType;
	pCompoM->mp_Owner = this;
	return pCompoM;
}

MSCustomSection* MSBaseBuilding::CreateNewCustomSection(int sectionType)
{
	MSCustomSection* customSection = nullptr;
	switch (sectionType)
	{
	case MSCustomSection::CUSTOM_COLUMN:
		customSection = new MSCustomColumnSection(this);
		break;
	case MSCustomSection::CUSTOM_WALLCOLUMN:
		customSection = new MSCustomWallColumnSection(this);
		break;
	case MSCustomSection::CUSTOM_BEAM:
		customSection = new MSCustomBeamSection(this);
		break;
	}

	return customSection;
}

vector<long> MSBaseBuilding::GetCompoMs(long nType)
{
	vector<long> idArr;
	return idArr;
}
vector<long> MSBaseBuilding::GetCompoMs()
{
	vector<long> idArr;
	return idArr;
}
vector<long> MSBaseBuilding::GetCompoGs(long nMID)
{
	vector<long> idArr;
	return idArr;
}

vector<MSCustomM*> MSBaseBuilding::GetCompoMsHasCustomSectionID(int sectionID)
{
	vector<MSCustomM*> compoMList; 
	return compoMList;
}

vector<MSCompoM*> MSBaseBuilding::GetAllColumnMsWidthColumnHeaderMID(long headerMID)
{
	vector<MSCompoM*> compoMList;
	return compoMList;
}

bool MSBaseBuilding::VerticalMNameAndStoryDuplicateCheck(MSCompoM* SrcCompoM, MSCompoM* DesCompoM, bool IsOnlyNameCheck)
{
	CString SrcName = SrcCompoM->GetName();
	CString DesName = DesCompoM->GetName();

	if (SrcName != DesName) return false;

	if (IsOnlyNameCheck == true) return true;

	int SrcFromStory = SrcCompoM->m_nFromStory;
	int SrcToStory = SrcCompoM->m_nToStory;
	int DesFromStory = DesCompoM->m_nFromStory;
	int DesToStory = DesCompoM->m_nToStory;

	if (DesFromStory <= SrcFromStory && SrcFromStory <= DesToStory) return true;
	if (DesFromStory <= SrcToStory && SrcToStory <= DesToStory) return true;
	if (SrcFromStory <= DesFromStory && DesFromStory <= SrcToStory) return true;
	if (SrcFromStory <= DesToStory && DesToStory <= SrcToStory) return true;

	if (SrcFromStory > SrcToStory || SrcFromStory == 0 || SrcToStory == 0) return true;

	int underStory = -(GetNumUnderStory() - 1);
	int aboveStory = GetNumAboveStory() - 1;

	if (SrcFromStory < underStory) return true;
	if (SrcToStory > aboveStory) return true;

	return false;
}

vector<int> MSBaseBuilding::GetAllWallGsWidthRebarLineID(int rebarLineID)
{
	vector<int> idArr;
	return idArr;
}

vector<long> MSBaseBuilding::GetCompoGs_InAllFloor(long nMID)
{
	vector<long> idArr;
	return idArr;
}

vector<MSCompoG*> MSBaseBuilding::GetAllCompoGs()
{
	vector<MSCompoG*> compoGArr;
	return compoGArr;
}

vector<MSFloor*> MSBaseBuilding::GetFloorsByLevel(double dStartLevel, double dEndLevel)
{
	vector<MSFloor*> floorArr;
	return floorArr;
}

vector<MSFloor*> MSBaseBuilding::GetAllFloors()
{
	vector<MSFloor*> floors;
	return floors;
}

void MSBaseBuilding::ClearAllCashFloorLevel()
{

}

vector<MSMaterial*> MSBaseBuilding::GetMaterialsByType(MSMaterial::Type_MSMaterial type)
{
	vector<MSMaterial*> materialArr;
	return materialArr;
}
MSRibTypeM* MSBaseBuilding::CreateNewRibM(int RibType)
{
	MSRibTypeM* pRibM = NULL;
	switch (RibType)
	{
	case MSRibTypeM::Rib_Standard:
		return new MSStandardRibTypeM();
	}
	return pRibM;
}

void MSBaseBuilding::DeleteRibTypes()
{
	map<long, MSRibTypeM*>::iterator it;
	for (it = mm_RibM.begin(); it != mm_RibM.end(); ++it)
		delete it->second;
	mm_RibM.clear();
}

void MSBaseBuilding::Add(MSRibTypeM* pRibType)
{
	mm_RibM.insert(make_pair(pRibType->m_ID, pRibType));
}

MSRibTypeM* MSBaseBuilding::GetRibM(long ID)
{
	map<long, MSRibTypeM*>::iterator it = mm_RibM.find(ID);
	if (it == mm_RibM.end())
		return NULL;

	return it->second;
}

void MSBaseBuilding::Remove(MSRibTypeM* pRibType)
{
	long nID = pRibType->m_ID;
	map<long, MSRibTypeM*>::iterator itRibType = mm_RibM.find(nID);
	if (itRibType == mm_RibM.end())	return;
	mm_RibM.erase(itRibType);
}

void MSBaseBuilding::Delete(MSRibTypeM* pRibType)
{
	Remove(pRibType);
	delete pRibType;
}

map<long, MSMaterial*>* MSBaseBuilding::GetMaterialMap()
{
	map<long, MSMaterial*> materialMap; 
	return &materialMap; 
}

MSWallEndRebarLine* MSBaseBuilding::CreateNewWallEndRebarLine()
{
	MSWallEndRebarLine* pWallEndRebarLine = new MSWallEndRebarLine();
	return pWallEndRebarLine;
}

MSSpacerShape* MSBaseBuilding::CreateNewSpacerShape()
{
	MSSpacerShape* pSpacer = new MSSpacerShape();
	return pSpacer;
}

bool MSBaseBuilding::IsVerticalCompoM(long nType)
{
	if (nType == MSCompoM::msColumnM || nType == MSCompoM::msBWallM || nType == MSCompoM::msRWallM || nType == MSCompoM::msPcColumnM || nType == MSCompoM::msCustomColumnM || nType == MSCompoM::msCustomWallColumnM)
		return true;
	else if (nType == MSCompoM::msBraceM || nType == MSCompoM::msButtressM)
		return true;
	else if (nType == MSCompoM::msSteelColumnM || nType == MSCompoM::msSrcColumnM)
		return true;
	return false;
}

GM2DVector MSBaseBuilding::GetUCSOriginPoint()
{
	return mp_UCS->GetOriginPoint();
}
GM2DVector MSBaseBuilding::GetUCSXDir()
{
	return mp_UCS->GetXDir();
}
GM2DVector MSBaseBuilding::GetUCSYDir()
{
	return mp_UCS->GetYDir();
}
GM2DVector MSBaseBuilding::GetImportBldgUCSOriginPoint()
{
	return mp_UCS_import->GetOriginPoint();
}
GM2DVector MSBaseBuilding::GetImportBldgUCSXDir()
{
	return mp_UCS_import->GetXDir();
}
GM2DVector MSBaseBuilding::GetImportBldgUCSYDir()
{
	return mp_UCS_import->GetYDir();
}
double MSBaseBuilding::GetUCSz()
{
	return mp_UCS->GetUCSz();
}
void MSBaseBuilding::SetBuildingUCS(GM2DVector originPoint, GM2DVector xDir, GM2DVector yDir)
{
	mp_UCS->SetUCS(originPoint, xDir, yDir);
}
void MSBaseBuilding::SetBuildingUCSz(double z)
{
	mp_UCS->SetUCSz(z);
}
void MSBaseBuilding::SetImportBuildingUCS(GM2DVector originPoint, GM2DVector xDir, GM2DVector yDir)
{
	mp_UCS_import->SetUCS(originPoint, xDir, yDir);
}

MSBaseBuilding* MSBaseBuilding::GetActiveBuilding()
{
	return ms_ActiveBuilding;
}

void MSBaseBuilding::SetActiveBuilding(MSBaseBuilding* pActiveBuilding)
{
	ms_ActiveBuilding = pActiveBuilding;
	if(ms_ActiveBuilding != nullptr)
		TypeUIDFactory::SetActiveTypeUIDFactory(ms_ActiveBuilding->GetTypeUIDFactory());
}

// static 함수
long MSBaseBuilding::GetUID(int type)
{
	return GetActiveBuilding()->GetTypeUIDFactory()->GetUID(type);
	//return TypeUIDFactory::GetID(type);
}

bool MSBaseBuilding::IsSumSOGAsFooting()
{
	MSQuantityOption* pMSQuantityOption = GetQuantityOption();
	if (pMSQuantityOption != nullptr)
		return pMSQuantityOption->m_bSumSOGAsFooting;

	return false;
}

int MSBaseBuilding::GetLicenseVersion()
{
	return m_nLicenseVersion;
}

void MSBaseBuilding::SetLicenseVersion(int licenseVersion)
{
	m_nLicenseVersion = licenseVersion;
}

MSMaterial* MSBaseBuilding::CreateNewMaterial(long nType)
{
	MSMaterial* pMaterial = NULL;
	switch (nType)
	{
		//		enum Type_MSMaterial {msConcrete=1, msPC, msSRC, msSteel};
	case MSMaterial::msConcrete:
		pMaterial = new MSConcrete;	break;
	case MSMaterial::msSteel:
		pMaterial = new MSSteel;	break;
	case MSMaterial::msSRC:
		pMaterial = new MSSrcMaterial; break;
	case MSMaterial::msFormWork:
		pMaterial = new MSFormWorkMaterial; break;
	default:
		ASSERT(0);
	}
	return pMaterial;
}

void MSBaseBuilding::FixGeometry()
{
	vector<MSFloor*> floors = GetAllFloors();
	for (int idx = 0; idx < floors.size(); idx++)
	{
		MSFloor* pFloor = floors[idx];
		vector<MSCompoG*> compoGArr = pFloor->FixGeometry();
		for (int iG = 0; iG < compoGArr.size(); iG++)
		{
			MSCompoG* pCompoG = compoGArr[iG];
			pFloor->Modify(pCompoG);
		}

		/*
		map<long, MSCompoG*> compoGMap;
		for (int idx = 0; idx < compoGArr.size(); idx++)
		{
			MSCompoG* pCompoG = compoGArr[idx];
			compoGMap.insert(make_pair(pCompoG->m_ID, pCompoG));
		}
		
		for (auto it = pFloor->mm_CompoG.begin(); it != pFloor->mm_CompoG.end(); it++)
		{
			MSCompoG* pCompoG = it->second;
			pCompoG->DeleteTrimmedBoundary_Acis();
		}

		pFloor->UpdateCompoGs(compoGMap, false);
		*/
		//pFloor->FixGeometry();
	}
	//ReGenerateJoint();
}

void MSBaseBuilding::SetLastUpdateModelVersion()
{
	m_nUpdateModelVersion = MSVersionInfo::GetLatestVersion(); 
}