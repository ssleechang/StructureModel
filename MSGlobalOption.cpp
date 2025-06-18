#include "StdAfx.h"
#include "MSGlobalOption.h"
//#include "MSAMBuilding.h"
#include "MSConcrete.h"
#include "MSSteel.h"
#include "MSReinforceBar.h"
#include "ReinforceBarSetting.h"
#include "MSSrcMaterial.h"


#include "GMLib/MSVersionInfo.h"
#include "MSDeformedBar.h"

#include "MSFormWorkMaterial.h"
#include "MSRcFWColumnM.h"
#include "MSRcFWBeamM.h"
#include "MSRcFWBWallM.h"
#include "MSRcFWSlabM.h"
#include "MSFWOpeningM.h"
#include "MSFWShadeM.h"
#include "MSFWSwitchBoxM.h"
#include "MSFWSoundAbsorptionM.h"
#include "MSFWSurfaceFoldingM.h"
#include "MSFWTrenchM.h"


#include "MSRcBeamM.h"
#include "MSRcBraceM.h"
#include "MSRcBWallM.h"
#include "MSRcColumnM.h"
#include "MSRcIsoFootM.h"
#include "MSRcRampM.h"
#include "MSRcRWallM.h"
#include "MSRcSlabM.h"
#include "MSRcStairM.h"
#include "MSRcWallFoot.h"
#include "MSRcPitM.h"
#include "MSStairM.h"
#include "MSRcLevelDifM.h"
#include "MSRcHandRailM.h"
#include "MSWindowM.h"
#include "MSDoorM.h"
#include "MSPcLineMemberM.h"
#include "MSPcColumnM.h"
#include "MRRcSection.h"
#include "MDRcHandRailM.h"
#include "MDRcIsoFootM.h"
#include "MSOpeningReinforceM.h"
#include "MSWallOpeningExtraM.h"
#include "MSSteelBeamM.h"
#include "MSSteelColumnM.h"
#include "MSSrcBeamM.h"
#include "MSSrcColumnM.h"
#include "MSColumnHeaderM.h"
#include "MSCustomColumnM.h"
#include "MSCustomWallColumnM.h"
#include "MSCustomBeamM.h"
#include "MSRcHaunchM.h"

IMPLEMENT_SERIAL(MSGlobalOption, MSObject, VERSIONABLE_SCHEMA | 1)

MSGlobalOption* MSGlobalOption::m_instance = NULL;
MSGlobalOption* MSGlobalOption::GetInstance()
{
	if(m_instance == NULL)
	{
		m_instance = new MSGlobalOption();		
	}
	return m_instance;
}

MSGlobalOption::MSGlobalOption(void)
{
	m_pSGradeManager = new MSSteelGradeManager();
}

MSGlobalOption::~MSGlobalOption(void)
{
	DeleteMaterials();
	DeleteReinforceBars();
	DeleteReinforceBarSettings();
	DeleteFormWorkShapeDatas();
	delete m_pSGradeManager;
}

bool MSGlobalOption::WriteFileBinary(CString strFilePath)
{
	CFile file;
	CFileException e;
	if(file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite, &e)==FALSE)
	{
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		}
	}
	CArchive ar(&file, CArchive::store);
	Serialize(ar);
	return true;
}

bool MSGlobalOption::WriteFileBinary_FormWorkMaterial(CString strFilePath)
{
	CFile file;
	CFileException e;
	if (file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite, &e) == FALSE)
	{
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		}
	}
	CArchive ar(&file, CArchive::store);
	Serialize_FormWorkMaterial(ar);
	return true;

}
bool MSGlobalOption::WriteFileBinary_FormWorkShape(CString strFilePath)
{
	CFile file;
	CFileException e;
	if (file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite, &e) == FALSE)
	{
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		}
	}
	CArchive ar(&file, CArchive::store);
	Serialize_FormWorkShape(ar);
	return true;

}

int MSGlobalOption::ReadFileBinary(CString strFilePath)
{
	CFile file;
	CFileException e;
	try
	{
		if (file.Open(strFilePath, CFile::modeRead, &e) == FALSE)
		{
			if (e.m_cause == CFileException::fileNotFound)
				return e.m_cause;
			else
			{
#ifdef _DEBUG
				afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
			}
		}
		if (file.GetLength() <= 0)	return e.m_cause;
		CArchive ar(&file, CArchive::load);
		Serialize(ar);
	}
	catch (...)
	{
		throw new exception("Unknown exception occurred in MSGlobalOption ReadFileBinary");
	}

	return e.m_cause;
}
int MSGlobalOption::ReadFileBinary_FormWorkMaterial(CString strFilePath)
{
	CFile file;
	CFileException e;
	try
	{
		if (file.Open(strFilePath, CFile::modeRead, &e) == FALSE)
		{
			if (e.m_cause == CFileException::fileNotFound)
				return e.m_cause;
			else
			{
#ifdef _DEBUG
				afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
			}
		}
		if (file.GetLength() <= 0)	return e.m_cause;
		CArchive ar(&file, CArchive::load);
		Serialize_FormWorkMaterial(ar);
	}
	catch (...)
	{
		throw new exception("Unknown exception occurred in MSGlobalOption ReadFileBinary_FormWorkMaterial");
	}

	return e.m_cause;
}

int MSGlobalOption::ReadFileBinary_FormWorkShape(CString strFilePath)
{
	CFile file;
	CFileException e;
	try
	{
		if (file.Open(strFilePath, CFile::modeRead, &e) == FALSE)
		{
			if (e.m_cause == CFileException::fileNotFound)
				return e.m_cause;
			else
			{
#ifdef _DEBUG
				afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
			}
		}
		if (file.GetLength() <= 0)	return e.m_cause;
		CArchive ar(&file, CArchive::load);
		Serialize_FormWorkShape(ar);
	}
	catch (...)
	{
		throw new exception("Unknown exception occurred in MSGlobalOption ReadFileBinary_FormWorkShape");
	}

	return e.m_cause;
}

void MSGlobalOption::DeleteMaterials()
{
	map<long, MSMaterial*>::iterator itMaterial;
	for(itMaterial=mm_Material.begin(); itMaterial != mm_Material.end(); itMaterial++)
		delete itMaterial->second;
	mm_Material.clear();
}

void MSGlobalOption::DeleteReinforceBars()
{
	map<long, MSReinforceBar*>::iterator it;
	for( it = mm_ReinforceBar.begin(); it != mm_ReinforceBar.end(); it++)
		delete it->second;
	mm_ReinforceBar.clear();
}

void MSGlobalOption::DeleteReinforceBarSettings()
{
	map<long, MSReinforceBarSetting*>::iterator it;
	for( it = mm_ReinforceBarSetting.begin(); it != mm_ReinforceBarSetting.end(); it++)
		delete it->second;
	mm_ReinforceBarSetting.clear();
}

bool MSGlobalOption::AddMaterial(MSMaterial* pMaterial)
{
	pair<map<long, MSMaterial*>::iterator, bool> bPair;
	bPair = mm_Material.insert(make_pair(pMaterial->m_GlobalID, pMaterial));
	return bPair.second;
}

bool MSGlobalOption::AddReinforceBar(MSReinforceBar* pReinforceBar)
{
	pair<map<long, MSReinforceBar*>::iterator, bool> bPair;
	bPair = mm_ReinforceBar.insert(make_pair(pReinforceBar->m_GlobalID, pReinforceBar));
	return bPair.second;
}

bool MSGlobalOption::AddReinforceBarSetting(MSReinforceBarSetting* pReinforceBarSetting)
{
	pair<map<long, MSReinforceBarSetting*>::iterator, bool> bPair;
	bPair = mm_ReinforceBarSetting.insert(make_pair(pReinforceBarSetting->m_GlobalID, pReinforceBarSetting));
	return bPair.second;
}

bool MSGlobalOption::DeleteMaterial(MSMaterial* pMaterial)
{
	map<long, MSMaterial*>::iterator itMaterial = mm_Material.find(pMaterial->m_GlobalID);
	if(itMaterial == mm_Material.end())	return false;
	mm_Material.erase(itMaterial);

	delete pMaterial;

	return true;
}

bool MSGlobalOption::DeleteReinforceBar(MSReinforceBar* pReinforceBar)
{
	map<long, MSReinforceBar*>::iterator it = mm_ReinforceBar.find(pReinforceBar->m_GlobalID);
	if(it == mm_ReinforceBar.end())	return false;
	mm_ReinforceBar.erase(it);

	delete pReinforceBar;

	return true;
}

bool MSGlobalOption::DeleteReinforceBarSetting(MSReinforceBarSetting* pReinforceBarSetting)
{
	map<long, MSReinforceBarSetting*>::iterator it = mm_ReinforceBarSetting.find(pReinforceBarSetting->m_GlobalID);
	if(it == mm_ReinforceBarSetting.end())	return false;
	mm_ReinforceBarSetting.erase(it);

	delete pReinforceBarSetting;

	return true;
}

MSReinforceBar* MSGlobalOption::GetReinforceBar(long ID)
{
	map<long, MSReinforceBar*>::iterator it = mm_ReinforceBar.find(ID);
	if(it == mm_ReinforceBar.end())
		return NULL;

	return it->second;
}

MSReinforceBar* MSGlobalOption::GetReinforceBar(CString Name)
{
	map<long, MSReinforceBar*>::iterator it;
	for (it = mm_ReinforceBar.begin(); it != mm_ReinforceBar.end(); it++)
	{
		if (it->second->m_Name == Name)
			return it->second;
	}

	return NULL;
}

MSReinforceBar* MSGlobalOption::GetFirstReinforceBar()
{
	map<long, MSReinforceBar*>::iterator it;
	for (it = mm_ReinforceBar.begin(); it != mm_ReinforceBar.end(); it++)
	{
		return it->second;
	}

	return nullptr;
}

bool MSGlobalOption::IsValidReinforceBar()
{
	return IsValidReinforceBar(GetFirstReinforceBar());
}

bool MSGlobalOption::IsValidReinforceBar(MSReinforceBar* pFirstReinforceBar)
{
	if (pFirstReinforceBar == nullptr)
		return false;

	CString name = pFirstReinforceBar->m_Name;

	int nRebarCode = MSDeformedBar::GetRebarCode();
	if (nRebarCode == MSDeformedBar::REBAR_MATERIAL_KS || nRebarCode == MSDeformedBar::REBAR_MATERIAL_ASTM ||
		nRebarCode == MSDeformedBar::REBAR_MATERIAL_BS || nRebarCode == MSDeformedBar::REBAR_MATERIAL_VIETNAM)
	{
		if (name == STR_SD300 || name == STR_SD350 || name == STR_SD400 || name == STR_SD400S || name == STR_SD400W || 
			name == STR_SD500 || name == STR_SD500S || name == STR_SD500W || name == STR_SD600 || name == STR_SD600S || name == STR_SD700)
			return true;
	}
	else if (nRebarCode == MSDeformedBar::REBAR_MATERIAL_GB)
	{
		if (name == STR_HPB235 || name == STR_HPB300 || name == STR_HRB335 || name == STR_HRBF335 || name == STR_HRB400 || name == STR_HRBF400 || 
			name == STR_RRB400 || name == STR_HRB500 || name == STR_HRBF500)
			return true;
	}
	else if (nRebarCode == MSDeformedBar::REBAR_MATERIAL_INDIA)
	{
		if (name == STR_Fe415 || name == STR_Fe415D || name == STR_Fe500 || name == STR_Fe500D || 
			name == STR_Fe550 || name == STR_Fe550D || name == STR_Fe600 || name == STR_CRS)
			return true;
	}
	else if (nRebarCode == MSDeformedBar::REBAR_MATERIAL_SINGAPORE)
	{
		if (name == STR_B485B || name == STR_B500B || name == STR_B600A || name == STR_B600B)
			return true;
	}
	else if (nRebarCode == MSDeformedBar::REBAR_MATERIAL_JIS)
	{
		if (name == STR_SD295 || name == STR_SD345 || name == STR_SD390 || name == STR_SD490)
			return true;
	}

	return false;
}

bool MSGlobalOption::IsValidReinforceBarSetting()
{
	for (auto it = mm_ReinforceBarSetting.begin(); it != mm_ReinforceBarSetting.end(); it++)
	{
		bool isValid = IsValidReinforceBarSetting(it->second);
		if(isValid == false)
			return false;
	}

	return true;
}

bool MSGlobalOption::IsValidReinforceBarSetting(MSReinforceBarSetting* pMSReinforceBarSetting)
{
	if (pMSReinforceBarSetting == nullptr)
		return false;

	if (pMSReinforceBarSetting->IsPlainConc())
		return true;

	if (pMSReinforceBarSetting->mm_IndexToReinforceBar.size() == 0)
		return false;

	auto it2 = pMSReinforceBarSetting->mm_IndexToReinforceBar.begin();
	if (it2 != pMSReinforceBarSetting->mm_IndexToReinforceBar.end())
	{
		// 처음 데이타만 체크한다.. 문제 발생시 전부 체크하는 로직으로 변경할것
		bool isValid = IsValidReinforceBar(it2->second);
		if (isValid == false)
			return false;
	}

	return true;
}

MSReinforceBarSetting* MSGlobalOption::GetReinforceBarSetting(long ID)
{
	map<long, MSReinforceBarSetting*>::iterator it = mm_ReinforceBarSetting.find(ID);
	if(it == mm_ReinforceBarSetting.end())
		return NULL;

	return it->second;
}

MSMaterial* MSGlobalOption::GetMaterial( long MaterialID)
{
	map<long, MSMaterial*>::iterator it = mm_Material.find(MaterialID);
	if(it == mm_Material.end())
		return NULL;

	return it->second;
}

MSMaterial* MSGlobalOption::CreateNewMaterial(long nType)
{
	MSMaterial* pMaterial = NULL;
	switch(nType)
	{
	case MSMaterial::msConcrete:
		pMaterial = new MSConcrete;	break;
	case MSMaterial::msSteel:
		pMaterial = new MSSteel;	break;
	case MSMaterial::msSRC:
		pMaterial = new MSSrcMaterial; break;
	default:
		ASSERT(0);
	}
	return pMaterial;
}

MSConcrete* MSGlobalOption::CreateConcrete(CString Name, int Strength)
{
	MSConcrete* pConcrete = new MSConcrete;
	pConcrete->SetName(Name);
	pConcrete->m_Name = Name;
	pConcrete->m_ConcKind = Name;
	pConcrete->m_ConcStrength = Strength;

	return pConcrete;
}

MSSteel* MSGlobalOption::CreateSteel(CString Name, int Strength)
{
	MSSteel* pSteelMaterial = new MSSteel();
	pSteelMaterial->m_Name = Name;
	pSteelMaterial->m_Fy = Strength;

	return pSteelMaterial;
}

MSSrcMaterial* MSGlobalOption::CreateSrc(MSConcrete* pConcrete, MSSteel* pSteel)
{
	MSSrcMaterial* pSrcMaterial = new MSSrcMaterial;
	pSrcMaterial->m_Name.Format(_T("%s/%s"), pConcrete->m_Name, pSteel->m_Name);
	pSrcMaterial->mp_RcMaterial = pConcrete;
	pSrcMaterial->mp_SteelMaterial = pSteel;

	return pSrcMaterial;
}

void MSGlobalOption::Serialize(CArchive &ar)
{
	if(ar.IsStoring())
	{
		ar << MSVersionInfo::GetLatestVersion();

		ar << m_PlainConcreteReinforceBarSettingID;
		ar << m_DefaultReinforceBarSettingID;
		ar << (int)mm_ReinforceBar.size();
		map<long, MSReinforceBar*>::iterator itReinforceBar;
		for( itReinforceBar = mm_ReinforceBar.begin() ; itReinforceBar != mm_ReinforceBar.end(); itReinforceBar++)
		{
			MSReinforceBar* pReinforceBar = itReinforceBar->second;
			pReinforceBar->GlobalOptionSerialize(ar);
		}

		ar << (int)mm_ReinforceBarSetting.size();
		map<long, MSReinforceBarSetting*>::iterator itReinforceBarSetting;
		for( itReinforceBarSetting = mm_ReinforceBarSetting.begin() ; itReinforceBarSetting != mm_ReinforceBarSetting.end(); itReinforceBarSetting++)
		{
			MSReinforceBarSetting* pReinforceBarSetting = itReinforceBarSetting->second;
			pReinforceBarSetting->GlobalOptionSerialize(ar);
		}

		ar << (int)mm_Material.size();
		map<long, MSMaterial*>::iterator itMaterial;
		for( itMaterial = mm_Material.begin(); itMaterial != mm_Material.end(); itMaterial++)
		{
			MSMaterial* pMaterial = itMaterial->second;
			int Type = pMaterial->GetType();
			ar << Type;
			pMaterial->GlobalOptionSerialize(ar);
		}

		m_pSGradeManager->GlobalOptionSerialize(ar);
	}
	else
	{
		DeleteMaterials();
		DeleteReinforceBars();
		DeleteReinforceBarSettings();
		m_pSGradeManager->ClearSGradePrefixMap();

		long firstVal;
		int version;

		ar >> firstVal;

		if(firstVal < 20150323)
		{
			version = 0;
			MSVersionInfo::PushCurrentVersion(version);

			m_PlainConcreteReinforceBarSettingID = firstVal;
		}
		else
		{
			version = firstVal;
			MSVersionInfo::PushCurrentVersion(version);

			ar >> m_PlainConcreteReinforceBarSettingID;
		}

		ar >> m_DefaultReinforceBarSettingID;
		int NumObject;

		vector<MSReinforceBar*> ReinforceBarList;

		ar >> NumObject;
		for(int iReinforceBar = 0; iReinforceBar < NumObject; iReinforceBar++)
		{
			MSReinforceBar* pReinforceBar = new MSReinforceBar();
			pReinforceBar->GlobalOptionSerialize(ar);

			mm_ReinforceBar.insert(make_pair(pReinforceBar->m_GlobalID, pReinforceBar));
		}

		if (firstVal < 20200112)
		{
			MSReinforceBar* pSD600SReinforcebar = MSReinforceBar::CreateSD600SReinforceBar(true);
			mm_ReinforceBar.insert(make_pair(pSD600SReinforcebar->m_GlobalID, pSD600SReinforcebar));
		}

		if(firstVal < 20150323)
			CreateNewReinforceBars();

		ar >> NumObject;
		for(int iReinforceBarSetting = 0; iReinforceBarSetting < NumObject; iReinforceBarSetting++)
		{
			MSReinforceBarSetting* pReinforceBarSetting = new MSReinforceBarSetting();
			pReinforceBarSetting->GlobalOptionSerialize(ar);
			mm_ReinforceBarSetting.insert(make_pair(pReinforceBarSetting->m_GlobalID, pReinforceBarSetting));
		}
		if (MSVersionInfo::GetCurrentVersion() < 20171210 && MSDeformedBar::GetRebarCode() == MSDeformedBar::REBAR_MATERIAL_GB)
		{
			UpdateReinforceBarGB_BH3374();
		}

		ar >> NumObject;
		for(int iMaterial = 0; iMaterial < NumObject; iMaterial++)
		{
			int nType = 0;
			ar >> nType;
			MSMaterial* pMaterial  = CreateNewMaterial(nType);
			if(pMaterial==NULL)
			{
				ASSERT(0);
				continue;
			}
			pMaterial->GlobalOptionSerialize(ar);
			mm_Material.insert(make_pair(pMaterial->m_GlobalID, pMaterial));
		} 

		map<long, MSMaterial*>::iterator itMaterial;
		for (itMaterial = mm_Material.begin(); itMaterial != mm_Material.end(); itMaterial++)
		{
			MSMaterial* pMaterial = itMaterial->second;
			int Type = pMaterial->GetType();
			if (Type == MSMaterial::msSRC)
				((MSSrcMaterial*)pMaterial)->SetGlobalOptionMaterialBySerializeID();
		}

		if(firstVal >= 20150323)
			m_pSGradeManager->GlobalOptionSerialize(ar);
		else
			m_pSGradeManager->MakeDefaultDataByRebarCode(MSDeformedBar::GetRebarCode());

		if(firstVal < 20150721)
		{
			Patch_Material_Version20150701();
		}
		MSVersionInfo::PopCurrentVersion();

		SetReinforceBarManager();

		PlainConcreteMaterialAdd();

	}
}

bool MSGlobalOption::MaterialNameExistCheck(CString name)
{
	map<long, MSMaterial*>::iterator it;
	for (it = mm_Material.begin(); it != mm_Material.end(); it++)
	{
		MSMaterial* pMate = it->second;
		if (pMate == NULL)	continue;
		if (pMate->m_Name == name.MakeUpper() && pMate->GetType() == MSMaterial::msConcrete)
			return true;
	}
	return false;
}

CString MSGlobalOption::NewMaterialName(double dConcStrength)
{
	CString sMaterialName;
	sMaterialName.Format(LocalDic::GetLocal(_T("C%d_무근")), (int)dConcStrength);
	int nIndex = 0;

	while (true)
	{
		if (MaterialNameExistCheck(sMaterialName) == false)
			return sMaterialName;

		sMaterialName.Format(LocalDic::GetLocal(_T("C%d_무근(%d)")), (int)dConcStrength, ++nIndex);

		if (nIndex == 50) break;
	}

	return _T("");
}

void MSGlobalOption::PlainConcreteMaterialAdd()
{
	double dConcStrength = -1;

	map<long, MSMaterial*>::iterator itMaterial;
	for (itMaterial = mm_Material.begin(); itMaterial != mm_Material.end(); itMaterial++)
	{
		MSMaterial* pMaterial = itMaterial->second;
		if (pMaterial->GetType() != MSMaterial::msConcrete) continue;
		
		if (dConcStrength == -1 || dConcStrength > pMaterial->GetConcStrength())
			dConcStrength = pMaterial->GetConcStrength();
		
		if (pMaterial->GetReinforceBarSetting()->m_GlobalID == GetPlainConcreteReinforceBarSettingID())
			return;
	}

	MSReinforceBarSetting* pPlainConcreteReinforceBarSetting = GetReinforceBarSetting(GetPlainConcreteReinforceBarSettingID());
	CString sMaterialName = NewMaterialName(dConcStrength);

	MSConcrete* ConcretePlain = CreateConcrete(sMaterialName, dConcStrength);
	ConcretePlain->GetGlobalID();
	ConcretePlain->m_pReinforceBarSetting = pPlainConcreteReinforceBarSetting;
	AddMaterial(ConcretePlain);

	// Src
	vector<MSMaterial*> steelMaterials;
	for (auto it = mm_Material.begin(); it != mm_Material.end(); it++)
	{
		if (it->second->GetType() == MSMaterial::msSteel)
			steelMaterials.push_back(it->second);
	}

	int nSteel = steelMaterials.size();
	for (int iStl = 0; iStl < nSteel; iStl++)
	{
		MSMaterial* pSteelMaterial = steelMaterials[iStl];
		MSSrcMaterial* pSRCMaterial = (MSSrcMaterial*)CreateNewMaterial(MSMaterial::msSRC);
		pSRCMaterial->mp_RcMaterial = (MSConcrete*)ConcretePlain;
		pSRCMaterial->mp_SteelMaterial = (MSSteel*)pSteelMaterial;
		pSRCMaterial->m_Name.Format(_T("%s/%s"), ConcretePlain->m_Name, pSteelMaterial->m_Name);
		pSRCMaterial->GetGlobalID();
		AddMaterial(pSRCMaterial);
	}
}

void MSGlobalOption::Serialize_FormWorkMaterial(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar << MSVersionInfo::GetLatestVersion();

		ar << (int)mm_Material.size();
		map<long, MSMaterial*>::iterator itMaterial;
		for (itMaterial = mm_Material.begin(); itMaterial != mm_Material.end(); itMaterial++)
		{
			MSFormWorkMaterial* pMaterial = (MSFormWorkMaterial*)itMaterial->second;
			pMaterial->GlobalOptionSerialize(ar);
		}
	}
	else
	{
		int version;

		ar >> version;
		MSVersionInfo::PushCurrentVersion(version);

		DeleteMaterials();

		int NumObject;

		ar >> NumObject;
		for (int iMaterial = 0; iMaterial < NumObject; iMaterial++)
		{
			MSFormWorkMaterial* pMaterial = new MSFormWorkMaterial();
			if (pMaterial == NULL)
			{
				ASSERT(0);
				continue;
			}
			pMaterial->GlobalOptionSerialize(ar);
			mm_Material.insert(make_pair(pMaterial->m_GlobalID, pMaterial));
		}

		MSVersionInfo::PopCurrentVersion();
	}
}


void MSGlobalOption::Serialize_FormWorkShape(CArchive &ar)
{
	long mType = 0;
	if (ar.IsStoring())
	{
		ar << MSVersionInfo::GetLatestVersion();

		ar << (int)ma_FormWorkShape.size();
		for (int i = 0; i < ma_FormWorkShape.size(); i++)
		{
			MSCompoM* pShape = ma_FormWorkShape[i];
			mType = pShape->GetType();
			ar << mType;
			pShape->Serialize(ar);
		}
	}
	else
	{
		int version;

		ar >> version;
		MSVersionInfo::PushCurrentVersion(version);

		DeleteFormWorkShapeDatas();

		int NumObject;

		ar >> NumObject;
		for (int i = 0; i < NumObject; i++)
		{
			long mType = 0;
			ar >> mType;
			MSCompoM* compoM = CreateNewCompoM(mType);
			if (compoM == nullptr)	continue;
			compoM->Serialize(ar);

			// F22-285 거푸집 객체 프로퍼티 정리
			// 사용자 기둥 삭제 사용하지 않는다. 20220803 객체 프로퍼티 정리
			if (compoM->GetShapeName() == MSRcFWColumnM::ToShapeName(MSRcFWColumnM::Custom))
			{
				delete compoM;
				continue;
			}
			AddFormWorkShape(compoM);
		}
		// 사용자 기둥 추가
// 		if (MSVersionInfo::GetCurrentVersion() < 20191024)
// 		{
// 			double w = 600, d = 600;
// 			MSRcFWColumnM* column = new MSRcFWColumnM();
// 			column->SetFWColumnType(MSRcFWColumnM::Custom);
// 			column->SetShapeName(column->ToShapeName(MSRcFWColumnM::Custom));
// 			column->SetName(_T("NONAME"));
// 			column->SetW(w);
// 			column->SetD(d);
// 			AddFormWorkShape(column);
// 		}
		MSVersionInfo::PopCurrentVersion();
	}
}

void MSGlobalOption::UpdateReinforceBarGB_BH3374()
{
	DeleteReinforceBars();
	for (auto it = mm_ReinforceBarSetting.begin(); it != mm_ReinforceBarSetting.end(); it++)
	{
		it->second->mm_IndexToReinforceBar.clear();
	}

	CStringArray Names;
	Names.Add(STR_HPB235);	Names.Add(STR_HPB300);	Names.Add(STR_HRB335);	Names.Add(STR_HRBF335);
	Names.Add(STR_HRB400);	Names.Add(STR_HRBF400);	Names.Add(STR_RRB400);	Names.Add(STR_HRB500);
	Names.Add(STR_HRBF500);

	CArray<double, double> Strengths;
	Strengths.Add(235);		Strengths.Add(300);		Strengths.Add(335);		Strengths.Add(335);
	Strengths.Add(400);		Strengths.Add(400);		Strengths.Add(400);		Strengths.Add(500);
	Strengths.Add(500);

	CStringArray PrintNames;
	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);
	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);
	PrintNames.Add(STR_BARTYPE_HD);

	CArray<MSSteelGradeManager::eSteelGrade> eSGrades;
	eSGrades.Add(MSSteelGradeManager::HPB235); eSGrades.Add(MSSteelGradeManager::HPB300);	eSGrades.Add(MSSteelGradeManager::HRB335); eSGrades.Add(MSSteelGradeManager::HRBF335);
	eSGrades.Add(MSSteelGradeManager::HRB400); eSGrades.Add(MSSteelGradeManager::HRBF400);	eSGrades.Add(MSSteelGradeManager::RRB400); eSGrades.Add(MSSteelGradeManager::HRB500);
	eSGrades.Add(MSSteelGradeManager::HRBF500);

	MSReinforceBarSetting* pReinforceBarSetting = GetReinforceBarSetting(GetDefaultReinforceBarSettingID());
	MSReinforceBar::ms_GlobalLastID = 0;
	int numData = Names.GetSize();
	CStringArray diaNameList;
	MSReinforceBar* pDefaultReinforceBar = nullptr;
	for (int iData = 0; iData < numData; iData++)
	{
		MSReinforceBar* pReinforcebar = new MSReinforceBar;
		if (pDefaultReinforceBar == nullptr)	pDefaultReinforceBar = pReinforcebar;
		pReinforcebar->m_Name = Names[iData];
		pReinforcebar->m_dBarStrength = Strengths[iData];
		//		pReinforcebar->m_PrintName = PrintNames[iData];
		pReinforcebar->GetGlobalID();
		AddReinforceBar(pReinforcebar);
	}

	int NumDia = MSDeformedBar::GetRebarNameList(diaNameList);
	for (int iDia = 0; iDia < NumDia; iDia++)
	{
		CString sDiaName = diaNameList[iDia];
		int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
		pReinforceBarSetting->mm_IndexToReinforceBar.insert(make_pair(DiaIndex, pDefaultReinforceBar));
	}
}

void MSGlobalOption::Patch_Material_Version20150701()
{
	vector<MSMaterial*> steelMaterials;
	vector<MSMaterial*> srcMaterials;
	vector<MSMaterial*> rcMaterial;
	map<long, MSMaterial*>::iterator it;
	for(it = mm_Material.begin(); it != mm_Material.end(); it++)
	{
		MSMaterial* pMaterial = it->second;
		if(pMaterial->GetType() == MSMaterial::msConcrete)
			rcMaterial.push_back(pMaterial);
		else if(pMaterial->GetType() == MSMaterial::msSteel)
			steelMaterials.push_back(pMaterial);
		else if(pMaterial->GetType() == MSMaterial::msSRC)
			srcMaterials.push_back(pMaterial);
	}

	int nSteel = steelMaterials.size();
	for(int i = 0; i < nSteel; i++)
		DeleteMaterial(steelMaterials[i]);
	int nSRC = srcMaterials.size();
	for(int i  = 0; i < nSRC; i++)
		DeleteMaterial(srcMaterials[i]);

	steelMaterials.clear();
	// RH-639 Steel/SRC Modeling기획 참조
	MSSteel* pSteelMaterial = (MSSteel*)CreateNewMaterial(MSMaterial::msSteel);
	pSteelMaterial->m_Name = _T("SS400");
	pSteelMaterial->m_Code = MSSteel::STL_CODE_KS;
	pSteelMaterial->m_Grade = MSSteel::STL_GRADE_SS400;
	pSteelMaterial->m_Density = 7850;
	pSteelMaterial->GetGlobalID();
	AddMaterial(pSteelMaterial);
	steelMaterials.push_back(pSteelMaterial);

	int nRC = rcMaterial.size();
	nSteel = steelMaterials.size();
	for(int iRc = 0; iRc < nRC; iRc++)
	{
		MSMaterial* pRcMaterial = rcMaterial[iRc];
		for(int iStl = 0; iStl < nSteel; iStl++)
		{
			MSMaterial* pSteelMaterial = steelMaterials[iStl];
			MSSrcMaterial* pSRCMaterial = (MSSrcMaterial*)CreateNewMaterial(MSMaterial::msSRC);
			pSRCMaterial->mp_RcMaterial = (MSConcrete*)pRcMaterial;
			pSRCMaterial->mp_SteelMaterial = (MSSteel*)pSteelMaterial;
			pSRCMaterial->m_Name.Format(_T("%s/%s"), pRcMaterial->m_Name, pSteelMaterial->m_Name);
			pSRCMaterial->GetGlobalID();
			AddMaterial(pSRCMaterial);
		}
	}
}

void MSGlobalOption::SetReinforceBarManager()
{
	map<long, MSReinforceBar*>::iterator it;
	for(it = mm_ReinforceBar.begin(); it != mm_ReinforceBar.end(); it++)
	{
		MSReinforceBar* pReinforceBar = it->second;
		pReinforceBar->m_pSGradeManager = m_pSGradeManager;
	}
}



void MSGlobalOption::CreateMaterialData()
{
	DeleteMaterials();
	DeleteReinforceBars();
	DeleteReinforceBarSettings();
	m_pSGradeManager->ClearSGradePrefixMap();

	int rebarCode = MSDeformedBar::sm_nRebarCode;

	MakeReinforceBarsByRebarCode();
	MakeReinforceBarSettingAndMaterialDatasByRebarCode();

}


void MSGlobalOption::CreateNewReinforceBars()
{
	DeleteReinforceBars();
	MSReinforceBar::ms_LastID = 0;

	CStringArray Names;
	Names.Add(_T("SD350"));	Names.Add(_T("SD400S"));	Names.Add(_T("SD400W"));	Names.Add(_T("SD500S"));	Names.Add(_T("SD500W"));	Names.Add(_T("SD700"));
	CArray<double, double> Strengths;
	Strengths.Add(350);	Strengths.Add(400);	Strengths.Add(400);	Strengths.Add(500);	Strengths.Add(500);	Strengths.Add(700);
	CStringArray PrintNames;
	PrintNames.Add(_T("D"));	PrintNames.Add(_T("HD"));	PrintNames.Add(_T("HD"));	PrintNames.Add(_T("SHD"));	PrintNames.Add(_T("SHD"));	PrintNames.Add(_T("UHD"));
	CArray<MSSteelGradeManager::eSteelGrade> eSGrades;
	eSGrades.Add(MSSteelGradeManager::SD350);	eSGrades.Add(MSSteelGradeManager::SD400S);	eSGrades.Add(MSSteelGradeManager::SD400W);
	eSGrades.Add(MSSteelGradeManager::SD500S);	eSGrades.Add(MSSteelGradeManager::SD500W);	eSGrades.Add(MSSteelGradeManager::SD700);

	int numData = 6;
	for (int iData = 0; iData < numData; iData++)
	{
		MSReinforceBar* pReinforcebar = new MSReinforceBar;
		pReinforcebar->m_Name = Names[iData];
		pReinforcebar->m_dBarStrength = Strengths[iData];
		//		pReinforcebar->m_PrintName = PrintNames[iData];
		pReinforcebar->m_eSGrade = eSGrades[iData];
		pReinforcebar->m_pSGradeManager = m_pSGradeManager;
		pReinforcebar->GetGlobalID();
		AddReinforceBar(pReinforcebar);
	}
}

void MSGlobalOption::MakeReinforceBarsByRebarCode()
{
	int rebarCode = MSDeformedBar::GetRebarCode();

	switch (rebarCode)
	{
	case MSDeformedBar::REBAR_MATERIAL_KS:					MakeReinforceBarsByRebarCode_KR();	break;
	case MSDeformedBar::REBAR_MATERIAL_ASTM:				MakeReinforceBarsByRebarCode_ASTM();	break;
	case MSDeformedBar::REBAR_MATERIAL_GB:					MakeReinforceBarsByRebarCode_GB();	break;
	case MSDeformedBar::REBAR_MATERIAL_BS:					MakeReinforceBarsByRebarCode_BS();	break;
	case MSDeformedBar::REBAR_MATERIAL_VIETNAM:				MakeReinforceBarsByRebarCode_VIETNAM();	break;
	case MSDeformedBar::REBAR_MATERIAL_INDIA:				MakeReinforceBarsByRebarCode_INDIA();	break;
	case MSDeformedBar::REBAR_MATERIAL_SINGAPORE:			MakeReinforceBarsByRebarCode_SINGAPORE();	break;
	case MSDeformedBar::REBAR_MATERIAL_JIS:					MakeReinforceBarsByRebarCode_JIS();	break;
	default:
		MakeReinforceBarsByRebarCode_KR();
	}
}

void MSGlobalOption::MakeReinforceBarsByRebarCode_KR()
{
	CStringArray Names;
	Names.Add(STR_SD300);	Names.Add(STR_SD350);		
	Names.Add(STR_SD400);	Names.Add(STR_SD400S);	Names.Add(STR_SD400W);	
	Names.Add(STR_SD500);	Names.Add(STR_SD500S);	Names.Add(STR_SD500W);	
	Names.Add(STR_SD600);	Names.Add(STR_SD600S);	Names.Add(STR_SD700);

	CArray<double, double> Strengths;
	Strengths.Add(300);	Strengths.Add(350);
	Strengths.Add(400);	Strengths.Add(400);	Strengths.Add(400);
	Strengths.Add(500);	Strengths.Add(500);	Strengths.Add(500);
	Strengths.Add(600);	Strengths.Add(600);	Strengths.Add(700);

	CStringArray PrintNames;
	PrintNames.Add(_T("D"));	PrintNames.Add(_T("D"));
	PrintNames.Add(_T("HD"));	PrintNames.Add(_T("HD"));	PrintNames.Add(_T("HD"));
	PrintNames.Add(_T("SHD"));	PrintNames.Add(_T("SHD"));	PrintNames.Add(_T("SHD"));
	PrintNames.Add(_T("UHD"));	PrintNames.Add(_T("UHD"));	PrintNames.Add(_T("UHD"));

	CArray<MSSteelGradeManager::eSteelGrade> eSGrades;
	eSGrades.Add(MSSteelGradeManager::SD300);	eSGrades.Add(MSSteelGradeManager::SD350);
	eSGrades.Add(MSSteelGradeManager::SD400);	eSGrades.Add(MSSteelGradeManager::SD400S);	eSGrades.Add(MSSteelGradeManager::SD400W);
	eSGrades.Add(MSSteelGradeManager::SD500);	eSGrades.Add(MSSteelGradeManager::SD500S);	eSGrades.Add(MSSteelGradeManager::SD500W);
	eSGrades.Add(MSSteelGradeManager::SD600);	eSGrades.Add(MSSteelGradeManager::SD600S);	eSGrades.Add(MSSteelGradeManager::SD700);

	vector<MSReinforceBar*> reinforceBarArr;
	int numData = Names.GetSize();
	for (int iData = 0; iData < numData; iData++)
	{
		MSReinforceBar* pReinforcebar = new MSReinforceBar;
		pReinforcebar->m_Name = Names[iData];
		pReinforcebar->m_dBarStrength = Strengths[iData];
		//		pReinforcebar->m_PrintName = PrintNames[iData];
		pReinforcebar->m_eSGrade = eSGrades[iData];
		pReinforcebar->GetGlobalID();
		AddReinforceBar(pReinforcebar);
	}
}

void MSGlobalOption::MakeReinforceBarsByRebarCode_ASTM()
{
	MakeReinforceBarsByRebarCode_KR();
}
void MSGlobalOption::MakeReinforceBarsByRebarCode_GB()
{
	CStringArray Names;
	Names.Add(STR_HPB235);	Names.Add(STR_HPB300);	Names.Add(STR_HRB335);	Names.Add(STR_HRBF335);
	Names.Add(STR_HRB400);	Names.Add(STR_HRBF400);	Names.Add(STR_RRB400);	Names.Add(STR_HRB500);
	Names.Add(STR_HRBF500);

	CArray<double, double> Strengths;
	Strengths.Add(235);		Strengths.Add(300);		Strengths.Add(335);		Strengths.Add(335);
	Strengths.Add(400);		Strengths.Add(400);		Strengths.Add(400);		Strengths.Add(500);
	Strengths.Add(500);

	CStringArray PrintNames;
	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);
	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);
	PrintNames.Add(STR_BARTYPE_HD);

	CArray<MSSteelGradeManager::eSteelGrade> eSGrades;
	eSGrades.Add(MSSteelGradeManager::HPB235); eSGrades.Add(MSSteelGradeManager::HPB300);	eSGrades.Add(MSSteelGradeManager::HRB335); eSGrades.Add(MSSteelGradeManager::HRBF335);
	eSGrades.Add(MSSteelGradeManager::HRB400); eSGrades.Add(MSSteelGradeManager::HRBF400);	eSGrades.Add(MSSteelGradeManager::RRB400); eSGrades.Add(MSSteelGradeManager::HRB500);
	eSGrades.Add(MSSteelGradeManager::HRBF500);

	int numData = Names.GetSize();
	for (int iData = 0; iData < numData; iData++)
	{
		MSReinforceBar* pReinforcebar = new MSReinforceBar;
		pReinforcebar->m_Name = Names[iData];
		pReinforcebar->m_dBarStrength = Strengths[iData];
		//		pReinforcebar->m_PrintName = PrintNames[iData];
		pReinforcebar->GetGlobalID();
		AddReinforceBar(pReinforcebar);
	}
}
void MSGlobalOption::MakeReinforceBarsByRebarCode_BS()
{
	MakeReinforceBarsByRebarCode_KR();
}
void MSGlobalOption::MakeReinforceBarsByRebarCode_VIETNAM()
{
	MakeReinforceBarsByRebarCode_KR();
}
void MSGlobalOption::MakeReinforceBarsByRebarCode_INDIA()
{
	CStringArray Names;
	Names.Add(STR_Fe415);	Names.Add(STR_Fe415D);	Names.Add(STR_Fe500);	Names.Add(STR_Fe500D);
	Names.Add(STR_Fe550);	Names.Add(STR_Fe550D);  Names.Add(STR_Fe600);  Names.Add(STR_CRS);

	CArray<double, double> Strengths;
	Strengths.Add(415);		Strengths.Add(415);		Strengths.Add(500);		Strengths.Add(500);
	Strengths.Add(550);		Strengths.Add(550);		Strengths.Add(600);		Strengths.Add(600);

	CStringArray PrintNames;
	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);
	PrintNames.Add(STR_BARTYPE_HD);  PrintNames.Add(STR_BARTYPE_HD);  PrintNames.Add(STR_BARTYPE_HD);  PrintNames.Add(STR_BARTYPE_HD);

	CArray<MSSteelGradeManager::eSteelGrade> eSGrades;
	eSGrades.Add(MSSteelGradeManager::Fe415); eSGrades.Add(MSSteelGradeManager::Fe415D);	eSGrades.Add(MSSteelGradeManager::Fe500); eSGrades.Add(MSSteelGradeManager::Fe500D);
	eSGrades.Add(MSSteelGradeManager::Fe550); eSGrades.Add(MSSteelGradeManager::Fe550D);	eSGrades.Add(MSSteelGradeManager::Fe600); eSGrades.Add(MSSteelGradeManager::CRS);

	int numData = Names.GetSize();
	for (int iData = 0; iData < numData; iData++)
	{
		MSReinforceBar* pReinforcebar = new MSReinforceBar;
		pReinforcebar->m_Name = Names[iData];
		pReinforcebar->m_dBarStrength = Strengths[iData];
		//		pReinforcebar->m_PrintName = PrintNames[iData];
		pReinforcebar->m_eSGrade = eSGrades[iData];
		pReinforcebar->GetGlobalID();
		AddReinforceBar(pReinforcebar);
	}
}
void MSGlobalOption::MakeReinforceBarsByRebarCode_SINGAPORE()
{
	CStringArray Names;
	Names.Add(STR_B485B);	Names.Add(STR_B500B);	Names.Add(STR_B600A);	Names.Add(STR_B600B);

	CArray<double, double> Strengths;
	Strengths.Add(485);		Strengths.Add(500);		Strengths.Add(600);		Strengths.Add(600);

	CStringArray PrintNames;
	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);

	CArray<MSSteelGradeManager::eSteelGrade> eSGrades;
	eSGrades.Add(MSSteelGradeManager::B485B); eSGrades.Add(MSSteelGradeManager::B500B);	eSGrades.Add(MSSteelGradeManager::B600A); eSGrades.Add(MSSteelGradeManager::B600B);

	int numData = Names.GetSize();
	for (int iData = 0; iData < numData; iData++)
	{
		MSReinforceBar* pReinforcebar = new MSReinforceBar;
		pReinforcebar->m_Name = Names[iData];
		pReinforcebar->m_dBarStrength = Strengths[iData];
		//		pReinforcebar->m_PrintName = PrintNames[iData];
		pReinforcebar->GetGlobalID();
		AddReinforceBar(pReinforcebar);
	}
}

void MSGlobalOption::MakeReinforceBarsByRebarCode_JIS()
{
	CStringArray Names;
	Names.Add(STR_SD295);	Names.Add(STR_SD345);
	Names.Add(STR_SD390);	Names.Add(STR_SD490);

	CArray<double, double> Strengths;
	Strengths.Add(295);	Strengths.Add(345);
	Strengths.Add(390);	Strengths.Add(490);
	
	CStringArray PrintNames;
	PrintNames.Add(_T("D"));	PrintNames.Add(_T("D"));
	PrintNames.Add(_T("D"));	PrintNames.Add(_T("D"));

	CArray<MSSteelGradeManager::eSteelGrade> eSGrades;
	eSGrades.Add(MSSteelGradeManager::SD295);	eSGrades.Add(MSSteelGradeManager::SD345);
	eSGrades.Add(MSSteelGradeManager::SD390);	eSGrades.Add(MSSteelGradeManager::SD490);

	vector<MSReinforceBar*> reinforceBarArr;
	int numData = Names.GetSize();
	for (int iData = 0; iData < numData; iData++)
	{
		MSReinforceBar* pReinforcebar = new MSReinforceBar;
		pReinforcebar->m_Name = Names[iData];
		pReinforcebar->m_dBarStrength = Strengths[iData];
		//		pReinforcebar->m_PrintName = PrintNames[iData];
		pReinforcebar->m_eSGrade = eSGrades[iData];
		pReinforcebar->GetGlobalID();
		AddReinforceBar(pReinforcebar);
	}
}

void MSGlobalOption::MakeReinforceBarSettingAndMaterialDatasByRebarCode()
{
	int rebarCode = MSDeformedBar::sm_nRebarCode;

	switch (rebarCode)
	{
	case MSDeformedBar::REBAR_MATERIAL_KS:					MakeReinforceBarSettingAndMaterialDatasByRebarCode_KR();	break;
	case MSDeformedBar::REBAR_MATERIAL_ASTM:				MakeReinforceBarSettingAndMaterialDatasByRebarCode_ASTM();	break;
	case MSDeformedBar::REBAR_MATERIAL_GB:					MakeReinforceBarSettingAndMaterialDatasByRebarCode_GB();	break;
	case MSDeformedBar::REBAR_MATERIAL_BS:					MakeReinforceBarSettingAndMaterialDatasByRebarCode_BS();	break;
	case MSDeformedBar::REBAR_MATERIAL_VIETNAM:				MakeReinforceBarSettingAndMaterialDatasByRebarCode_VIETNAM();	break;
	case MSDeformedBar::REBAR_MATERIAL_INDIA:				MakeReinforceBarSettingAndMaterialDatasByRebarCode_INDIA();	break;
	case MSDeformedBar::REBAR_MATERIAL_SINGAPORE:			MakeReinforceBarSettingAndMaterialDatasByRebarCode_SINGAPORE();	break;
	case MSDeformedBar::REBAR_MATERIAL_JIS:					MakeReinforceBarSettingAndMaterialDatasByRebarCode_JIS();	break;
	default:
		MakeReinforceBarSettingAndMaterialDatasByRebarCode_KR();
	}
}
void MSGlobalOption::MakeReinforceBarSettingAndMaterialDatasByRebarCode_KR()
{
	int numData = mm_ReinforceBar.size();
	MSReinforceBar* pSD400Reinforcebar = NULL;
	MSReinforceBar* pSD500Reinforcebar = NULL;

	m_pSGradeManager->MakeDefaultDataByRebarCode(MSDeformedBar::GetRebarCode());

	MakeDefaultReinforceBarSettings();
	MakeRcMaterialsByRebarCode();
	MakeDefaultSteelMaterials();
	MakeDefaultSrcMaterials();
}

void MSGlobalOption::MakeReinforceBarSettingAndMaterialDatasByRebarCode_ASTM()
{
	MakeReinforceBarSettingAndMaterialDatasByRebarCode_KR();
}
void MSGlobalOption::MakeReinforceBarSettingAndMaterialDatasByRebarCode_GB()
{
	MakeReinforceBarSettingAndMaterialDatasByRebarCode_KR();
}
void MSGlobalOption::MakeReinforceBarSettingAndMaterialDatasByRebarCode_BS()
{
	MakeReinforceBarSettingAndMaterialDatasByRebarCode_KR();
}
void MSGlobalOption::MakeReinforceBarSettingAndMaterialDatasByRebarCode_VIETNAM()
{
	MakeReinforceBarSettingAndMaterialDatasByRebarCode_KR();
}									  
void MSGlobalOption::MakeReinforceBarSettingAndMaterialDatasByRebarCode_INDIA()
{
	MakeReinforceBarSettingAndMaterialDatasByRebarCode_KR();
}
void MSGlobalOption::MakeReinforceBarSettingAndMaterialDatasByRebarCode_SINGAPORE()
{
	MakeReinforceBarSettingAndMaterialDatasByRebarCode_KR();
}

void MSGlobalOption::MakeReinforceBarSettingAndMaterialDatasByRebarCode_JIS()
{
	m_pSGradeManager->MakeDefaultDataByRebarCode(MSDeformedBar::GetRebarCode());

	MakeDefaultReinforceBarSettings();
	MakeRcMaterialsByRebarCode();
	MakeDefaultSteelMaterials();
	MakeDefaultSrcMaterials();
}

void MSGlobalOption::MakeDefaultReinforceBarSettings()
{
	MSReinforceBarSetting* pPlainConcreteReinforceBarSetting = new MSReinforceBarSetting;
	pPlainConcreteReinforceBarSetting->m_Name = LocalDic::GetLocal(L"무근콘크리트");
	pPlainConcreteReinforceBarSetting->GetGlobalID();
	AddReinforceBarSetting(pPlainConcreteReinforceBarSetting);
	SetPlainConcreteReinforceBarSettingID(pPlainConcreteReinforceBarSetting->GetGlobalID());

	CStringArray DiaNameList;
	long NumDia = MSDeformedBar::GetRebarNameList(DiaNameList);
	MSReinforceBarSetting* pReinforceBarSetting = new MSReinforceBarSetting;
	pReinforceBarSetting->m_Name = _T("Default");

	MSReinforceBar* pSD400ReinforceBar = nullptr;
	MSReinforceBar* pSD500ReinforceBar = nullptr;
	MSReinforceBar* pDefaultReinforceBar = nullptr;
	bool IsKr = false;
	if (MSDeformedBar::GetRebarCode() == MSDeformedBar::REBAR_MATERIAL_KS)
	{
		int i = 0;
		for (auto it = mm_ReinforceBar.begin(); it != mm_ReinforceBar.end(); it++)
		{
			MSReinforceBar* pReinforcebar = it->second;
			if (i == 2)
				pSD400ReinforceBar = pReinforcebar;
			else if (i == 5)
				pSD500ReinforceBar = pReinforcebar;
			i++;
		}
		IsKr = true;
	}
	else if (mm_ReinforceBar.size() > 0)
		pDefaultReinforceBar = mm_ReinforceBar.begin()->second;
	else
		return;


	for (int iDia = 0; iDia < NumDia; iDia++)
	{
		CString sDiaName = DiaNameList[iDia];
		int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
		double dDia = MSDeformedBar::FindDeformedBarDiameterByIndex(DiaIndex);

		// TODO(김신웅) : 아래 조건문은 한국규준에만 맞춘것이다. 다른 규준에도 적용할 수 있도록 수정되어야 한다.
		if (IsKr && pSD400ReinforceBar != nullptr && pSD500ReinforceBar != nullptr)
		{
			if (dDia <= 13)
				pReinforceBarSetting->mm_IndexToReinforceBar.insert(make_pair(DiaIndex, pSD400ReinforceBar));
			else
				pReinforceBarSetting->mm_IndexToReinforceBar.insert(make_pair(DiaIndex, pSD500ReinforceBar));
		}
		else if(pDefaultReinforceBar != nullptr)
			pReinforceBarSetting->mm_IndexToReinforceBar.insert(make_pair(DiaIndex, pDefaultReinforceBar));
	}

	pReinforceBarSetting->GetGlobalID();
	AddReinforceBarSetting(pReinforceBarSetting);
	SetDefaultReinforceBarSettingID(pReinforceBarSetting->GetGlobalID());
}

void MSGlobalOption::MakeRcMaterialsByRebarCode()
{
	int rebarCode = MSDeformedBar::sm_nRebarCode;

	switch (rebarCode)
	{
	case MSDeformedBar::REBAR_MATERIAL_GB:					MakeRcMaterialsByRebarCode_GB();	break;
	default:
		MakeRcMaterialsByRebarCode_Kr();
	}
}

void MSGlobalOption::MakeRcMaterialsByRebarCode_Kr()
{
	MSReinforceBarSetting* pPlainConcreteReinforceBarSetting = GetReinforceBarSetting(GetPlainConcreteReinforceBarSettingID());

	MSConcrete* Concrete21Plain = CreateConcrete(LocalDic::GetLocal(_T("C21_무근")), 21);
	Concrete21Plain->GetGlobalID();
	Concrete21Plain->m_pReinforceBarSetting = pPlainConcreteReinforceBarSetting;
	AddMaterial(Concrete21Plain);

	MSReinforceBarSetting* pReinforceBarSetting = GetReinforceBarSetting(GetDefaultReinforceBarSettingID());

	MSConcrete* Concrete21 = CreateConcrete(_T("C21"), 21);
	Concrete21->GetGlobalID();
	Concrete21->m_pReinforceBarSetting = pReinforceBarSetting;
	AddMaterial(Concrete21);

	MSConcrete* Concrete24 = CreateConcrete(_T("C24"), 24);
	Concrete24->GetGlobalID();
	Concrete24->m_pReinforceBarSetting = pReinforceBarSetting;
	AddMaterial(Concrete24);

}

void MSGlobalOption::MakeRcMaterialsByRebarCode_GB()
{
	MSReinforceBarSetting* pPlainConcreteReinforceBarSetting = GetReinforceBarSetting(GetPlainConcreteReinforceBarSettingID());

	MSConcrete* Concrete20Plain = CreateConcrete(_T("C20_무근"), 20);
	Concrete20Plain->GetGlobalID();
	Concrete20Plain->m_pReinforceBarSetting = pPlainConcreteReinforceBarSetting;
	AddMaterial(Concrete20Plain);

	MSReinforceBarSetting* pReinforceBarSetting = GetReinforceBarSetting(GetDefaultReinforceBarSettingID());

	MSConcrete* Concrete20 = CreateConcrete(_T("C20"), 20);
	Concrete20->GetGlobalID();
	Concrete20->m_pReinforceBarSetting = pReinforceBarSetting;
	AddMaterial(Concrete20);

	MSConcrete* Concrete25 = CreateConcrete(_T("C25"), 25);
	Concrete25->GetGlobalID();
	Concrete25->m_pReinforceBarSetting = pReinforceBarSetting;
	AddMaterial(Concrete25);
}

void MSGlobalOption::MakeDefaultSteelMaterials()
{
	CString NameSS400 = _T("SS400");
	MSSteel* pSS400 = CreateSteel(NameSS400, 400);
	pSS400->m_Density = 7850;
	pSS400->GetGlobalID();
	AddMaterial(pSS400);
}
void MSGlobalOption::MakeDefaultSrcMaterials()
{
	// Src
	vector<MSMaterial*> rcMaterials, steelMaterials;
	for (auto it = mm_Material.begin(); it != mm_Material.end(); it++)
	{
		if (it->second->GetType() == MSMaterial::msConcrete)
			rcMaterials.push_back(it->second);
		else if (it->second->GetType() == MSMaterial::msSteel)
			steelMaterials.push_back(it->second);
	}
	int nRC = rcMaterials.size();
	int nSteel = steelMaterials.size();
	for (int iRc = 0; iRc < nRC; iRc++)
	{
		MSMaterial* pRcMaterial = rcMaterials[iRc];
		for (int iStl = 0; iStl < nSteel; iStl++)
		{
			MSMaterial* pSteelMaterial = steelMaterials[iStl];
			MSSrcMaterial* pSRCMaterial = (MSSrcMaterial*)CreateNewMaterial(MSMaterial::msSRC);
			pSRCMaterial->mp_RcMaterial = (MSConcrete*)pRcMaterial;
			pSRCMaterial->mp_SteelMaterial = (MSSteel*)pSteelMaterial;
			pSRCMaterial->m_Name.Format(_T("%s/%s"), pRcMaterial->m_Name, pSteelMaterial->m_Name);
			pSRCMaterial->GetGlobalID();
			AddMaterial(pSRCMaterial);
		}
	}
}

void MSGlobalOption::CreateFormWorkMaterialData()
{
	DeleteMaterials();
	CStringArray nameList;
	nameList.Add(_T("Concrete"));	nameList.Add(_T("PrecastConcrete"));
	nameList.Add(_T("Brick"));	nameList.Add(_T("Wood"));

	int fromIndex = MSFormWorkMaterial::Concrete;
	int toIndex = MSFormWorkMaterial::Wood;

	for (int i = fromIndex; i <= toIndex; i++)
	{
		MSFormWorkMaterial* pMaterial = new MSFormWorkMaterial();
		pMaterial->SetFWMaterialType((MSFormWorkMaterial::eFWMaterialType)i);
		pMaterial->SetName(nameList[i]);
		pMaterial->GetGlobalID();
		AddMaterial(pMaterial);
	}
}

void MSGlobalOption::DeleteFormWorkShapeDatas()
{
	for (int i = 0; i < ma_FormWorkShape.size(); i++)
		delete ma_FormWorkShape[i];
	ma_FormWorkShape.clear();
}

void MSGlobalOption::DeleteFormWorkShape(MSCompoM* pCompoM)
{
	int findIdx = -1;
	for (int i = 0; i < ma_FormWorkShape.size(); i++)
	{
		if (ma_FormWorkShape[i]->m_ID == pCompoM->m_ID)
		{
			findIdx = i;
			break;
		}
	}
	if (findIdx != -1)
	{
		ma_FormWorkShape.erase(ma_FormWorkShape.begin() + findIdx);
		delete pCompoM;
	}
}

void MSGlobalOption::CreateFormWorkShapeDatas()
{
	DeleteFormWorkShapeDatas();
	CreateFWMainCompoMs();
	CreateFWSubCompoMs();
}

void MSGlobalOption::AddFormWorkShape(MSCompoM* pCompoM)
{
	ma_FormWorkShape.push_back(pCompoM);
}



void MSGlobalOption::CreateFWMainCompoMs()
{
#pragma region --- FWColumnM
	int fromIndex = MSRcFWColumnM::Rect;
	//int toIndex = MSRcFWColumnM::Custom; // 사용자 단면은 사용하지 않는다,
	int toIndex = MSRcFWColumnM::Octagon_Outter; // 사용자 단면은 사용하지 않는다,

	double w = 500, d = 700;
	for (int i = fromIndex; i <= toIndex; i++)
	{
		if (i > 0) { w = d = 600; }
		MSRcFWColumnM* column = new MSRcFWColumnM();
		column->SetFWColumnType((MSRcFWColumnM::eFWColumnType)i);
        column->SetShapeName(column->ToShapeName((MSRcFWColumnM::eFWColumnType)i));
		column->SetName(_T("NONAME"));
		column->SetW(w);
		column->SetD(d);
		AddFormWorkShape(column);
	}
#pragma endregion

#pragma  region --- FWBWallM
	fromIndex = MSRcFWBWallM::General;
	toIndex = MSRcFWBWallM::Brick;
	double t = 200;
	for (int i = fromIndex; i <= toIndex; i++)
	{
		if (i > 0)	t = 90;
		MSRcFWBWallM* wall = new MSRcFWBWallM();
		wall->SetFWBWallType((MSRcFWBWallM::eFWBWallType)i);
        wall->SetShapeName(wall->ToShapeName((MSRcFWBWallM::eFWBWallType)i));
		wall->SetName(_T("NONAME"));
		wall->SetT(t);
		AddFormWorkShape(wall);
	}
#pragma endregion

#pragma region --- FWBeamM
	fromIndex = MSRcFWBeamM::Type1;
	toIndex = MSRcFWBeamM::Type9;
	double w1 = 0, w2 = 0;
	double h = 0, h1 = 0, h2 = 0;
	for (int i = fromIndex; i <= toIndex; i++)
	{
		w = w1 = w2 = h = h1 = h2 = 0;
		switch ((MSRcFWBeamM::eFWBeamType)i)
		{
		case MSRcFWBeamM::Type1:	w = 200;	h = 600;	break;
		case MSRcFWBeamM::Type2: w = 200; h = 400; h1 = 350;	break;
		case MSRcFWBeamM::Type3:	w = 400; w1 = 100; w2 = 200; h = 600; h1 = 200;	break;
		case MSRcFWBeamM::Type4:	w = 400; w1 = 100; w2 = 200; h = 600; h1 = 200;	break;
		case MSRcFWBeamM::Type5:	w = 200; h = 320; break;
		case MSRcFWBeamM::Type6:	w = 400; w1 = 100; w2 = 200; h = 600; h1 = 200;	break;
		case MSRcFWBeamM::Type7:	w = 400; w1 = 100; w2 = 200; h = 600; h1 = 200;	break;
		case MSRcFWBeamM::Type8:	w = 200; h = 320; h1 = 20;	break;
		case MSRcFWBeamM::Type9:	w = 150; w1 = 20; h = 600; h1 = 20;	break;
		}

		MSRcFWBeamM* beam = new MSRcFWBeamM();
		beam->SetFWBeamType((MSRcFWBeamM::eFWBeamType)i);
        beam->SetShapeName(beam->ToShapeName((MSRcFWBeamM::eFWBeamType)i));
		beam->SetName(_T("NONAME"));
		beam->SetW(w);
		beam->SetW1(w1);
		beam->SetW2(w2);
		beam->SetH(h);
		beam->SetH1(h1);
		beam->SetH2(h2);
		AddFormWorkShape(beam);
	}
#pragma endregion

#pragma region --- FWSlabM

	fromIndex = MSRcFWSlabM::General;
	toIndex = MSRcFWSlabM::TopSlope;
	double t1 = 0;
	for (int i = fromIndex; i <= toIndex; i++)
	{
		t = 0; t1 = 0;
		switch ((MSRcFWSlabM::eFWSlabType)i)
		{
		case MSRcFWSlabM::General:	t = 210;	break;
		case MSRcFWSlabM::BottomSlope:	t = 150; t1 = 125;	break;
		case MSRcFWSlabM::TopSlope: t = 150; t1 = 125;	break;
		}

		MSRcFWSlabM* slab = new MSRcFWSlabM();
		slab->SetFWSlabType((MSRcFWSlabM::eFWSlabType)i);
        slab->SetShapeName(slab->ToShapeName((MSRcFWSlabM::eFWSlabType)i));
		slab->SetName(_T("NONAME"));
		slab->SetT(t);
		slab->SetT1(t1);
		AddFormWorkShape(slab);
	}
#pragma endregion
}

void MSGlobalOption::CreateFWSubCompoMs()
{
#pragma region --- FWOpeningM
	int fromIndex = MSFWOpeningM::RECT;
	int toIndex = MSFWOpeningM::CIRCLE;
	double w = 0, h = 0;
	for (int i = fromIndex; i <= toIndex; i++)
	{
		w = 0; h = 0;
		switch ((MSFWOpeningM::eFWOpeningType)i)
		{
		case MSFWOpeningM::RECT:	w = 500; h = 500;	break;
		case MSFWOpeningM::CIRCLE:	 w = 200; h = 200;	break;
		}

		MSFWOpeningM* opening = new MSFWOpeningM();
		opening->SetFWOpeningType((MSFWOpeningM::eFWOpeningType)i);
        opening->SetShapeName(opening->ToShapeName((MSFWOpeningM::eFWOpeningType)i));
		opening->SetName(_T("NONAME"));
		opening->SetWidth(w);
		opening->SetHeight(h);
		AddFormWorkShape(opening);
	}
#pragma endregion

#pragma region --- FWShadeM
	fromIndex = MSFWShadeM::Horizontal_Type1;
	toIndex = MSFWShadeM::Vertical_Type3;
	w = 0, h = 0;
	double w1 = 0, h1 = 0;
	for (int i = fromIndex; i <= toIndex; i++)
	{
		w = 0; h = 0; w1 = 0; h1 = 0;
		switch ((MSFWShadeM::eFWShadeType)i)
		{
		case MSFWShadeM::Horizontal_Type1:	w = 100; h = 100;	break;
		case MSFWShadeM::Horizontal_Type2:	 w = 200; h = 200; w1 = 10;	h1 = 10; break;
		case MSFWShadeM::Vertical_Type1:	 w = 100; h = 100; break;
		case MSFWShadeM::Vertical_Type2:	 w = 200; h = 200; w1 = 10;	h1 = 10; break;
		case MSFWShadeM::Vertical_Type3:	 w = 200; h = 200; w1 = 10;	h1 = 10; break;
		default:
			continue;
		}

		MSFWShadeM* shade = new MSFWShadeM();
		shade->SetFWShadeType((MSFWShadeM::eFWShadeType)i);
        shade->SetShapeName(shade->ToShapeName((MSFWShadeM::eFWShadeType)i));
		shade->SetName(_T("NONAME"));
		shade->SetW(w);
		shade->SetH(h);
		shade->SetW1(w1);
		shade->SetH1(h1);
		AddFormWorkShape(shade);
	}
#pragma endregion

#pragma region --- FWSwitchBoxM
	fromIndex = MSFWSwitchBoxM::General;
	toIndex = MSFWSwitchBoxM::General;
	w = 0, h = 0;
	double l = 0;
	for (int i = fromIndex; i <= toIndex; i++)
	{
		w = 0; h = 0; l = 0;
		switch ((MSFWSwitchBoxM::eFWSwitchBoxType)i)
		{
		case MSFWSwitchBoxM::General:	w = 500; h = 500;	l = 25;  break;
		}

		MSFWSwitchBoxM* switchBox = new MSFWSwitchBoxM();
		switchBox->SetFWSwitchBoxType((MSFWSwitchBoxM::eFWSwitchBoxType)i);
        switchBox->SetShapeName(switchBox->ToShapeName((MSFWSwitchBoxM::eFWSwitchBoxType)i));
		switchBox->SetName(_T("NONAME"));
		switchBox->SetW(w);
		switchBox->SetH(h);
		switchBox->SetL(l);
		AddFormWorkShape(switchBox);
	}
#pragma endregion

#pragma region --- FWSoundAbsorptionM
	fromIndex = MSFWSoundAbsorptionM::General;
	toIndex = MSFWSoundAbsorptionM::General;
	double t = 0;
	for (int i = fromIndex; i <= toIndex; i++)
	{
		t = 0;
		switch ((MSFWSoundAbsorptionM::eFWSoundAbsorptionType)i)
		{
		case MSFWSoundAbsorptionM::General:	t = 30;	break;
		}

		MSFWSoundAbsorptionM* soundAbsorption = new MSFWSoundAbsorptionM();
		soundAbsorption->SetFWSoundAbsorptionType((MSFWSoundAbsorptionM::eFWSoundAbsorptionType)i);
        soundAbsorption->SetShapeName(soundAbsorption->ToShapeName((MSFWSoundAbsorptionM::eFWSoundAbsorptionType)i));
		soundAbsorption->SetName(_T("NONAME"));
		soundAbsorption->SetT(t);
		AddFormWorkShape(soundAbsorption);
	}
#pragma endregion

#pragma region --- FWSurfaceFoldingM

	double l1 = 150, l2 = 150;
	MSFWSurfaceFoldingM* surfaceFolding = new MSFWSurfaceFoldingM();
    surfaceFolding->SetShapeName(surfaceFolding->ToShapeName());
	surfaceFolding->SetName(_T("NONAME"));
	surfaceFolding->SetL1(l1);
	surfaceFolding->SetL2(l2);
	AddFormWorkShape(surfaceFolding);

#pragma endregion

#pragma region --- FWTrenchM

	w = 150, t = 50;
	MSFWTrenchM* trenchM = new MSFWTrenchM();
    trenchM->SetShapeName(trenchM->ToShapeName(MSFWTrenchM::eFWTrenchType::General));
	trenchM->SetName(_T("NONAME"));
	trenchM->SetW(w);
	trenchM->SetT(t);
	AddFormWorkShape(trenchM);

#pragma endregion
}

MSCompoM* MSGlobalOption::CreateNewCompoM(long nType)
{
	MSCompoM* pCompoM = nullptr;
	switch (nType)
	{
		//Line
	case MSCompoM::msBeamM:	pCompoM = new MSRcFWBeamM();	break;
	case MSCompoM::msColumnM: pCompoM = new MSRcFWColumnM();	break;
	case MSCompoM::msBWallM:	pCompoM = new MSRcFWBWallM();	break;
	case MSCompoM::msSlabM:	pCompoM = new MSRcFWSlabM();	break;
	case MSCompoM::msFWOpeningM: pCompoM = new MSFWOpeningM();	break;
	case MSCompoM::msFWShadeM: pCompoM = new MSFWShadeM();	break;
	case MSCompoM::msFWSwitchBoxM: pCompoM = new MSFWSwitchBoxM();	break;
	case MSCompoM::msFWSoundAbsorptionM: pCompoM = new MSFWSoundAbsorptionM();	break;
	case MSCompoM::msFWSurfaceFoldingM:	pCompoM = new MSFWSurfaceFoldingM();	break;
	case MSCompoM::msFWTrenchM:	pCompoM = new MSFWTrenchM();	break;
	}
	if (pCompoM == nullptr)	return nullptr;

	pCompoM->m_nType = (MSCompoM::Type_MSCompoM)nType;
	return pCompoM;
}