#include "stdafx.h"
#include "MSFormWorkBuilding.h"
#include "MSFormWorkDesignUnit.h"
#include "MSFormWorkMaterial.h"

#include "MSRcFWBeamM.h"
#include "MSRcFWColumnM.h"
#include "MSRcFWBWallM.h"
#include "MSRcFWSlabM.h"
#include "MSFWOpeningM.h"
#include "MSFWShadeM.h"
#include "MSFWSwitchBoxM.h"
#include "MSFWSoundAbsorptionM.h"
#include "MSFWSurfaceFoldingM.h"
#include "MSFWTrenchM.h"
#include "MSBuildingUCS.h"

#include "GMlib/TypeUIDFactory.h"
#include "GMlib/UIDFactory.h"

IMPLEMENT_SERIAL(MSFormWorkBuilding, MSBaseBuilding, VERSIONABLE_SCHEMA | 1)
MSFormWorkBuilding::MSFormWorkBuilding()
{
	LoggerWrapper::Log("MSFormWorkBuilding initialized");
}


MSFormWorkBuilding::~MSFormWorkBuilding()
{
	DeleteDesignUnits();
	DeleteMaterials();
	DeleteShapeTemplates();
}

void MSFormWorkBuilding::Serialize(CArchive &ar)
{
	MSBaseBuilding::SetActiveBuilding(this);
	int count = 0;
	if (ar.IsStoring())
	{
		ar << MSVersionInfo::GetLatestVersion();
		MSBaseBuilding::Serialize(ar);

		ar << m_Name;

		count = mm_Material.size();
		ar << count;
		for (auto it = mm_Material.begin(); it != mm_Material.end(); it++)
		{
			int materialType = (int)it->second->GetType();
			ar << materialType;
			it->second->Serialize(ar);
		}

		count = mm_ShapeTemplate.size();
		ar << count;
		for (auto it = mm_ShapeTemplate.begin(); it != mm_ShapeTemplate.end(); it++)
		{
			int mType = (int)it->second->GetType();
			ar << mType;
			it->second->Serialize(ar);
		}

		count = mm_DesignUnit.size();
		ar << count;
		for (auto it = mm_DesignUnit.begin(); it != mm_DesignUnit.end(); it++)
			it->second->Serialize(ar);

		mp_UCS->Serialize(ar);
	}
	else
	{
		int version;
		ar >> version;
		MSVersionInfo::PushCurrentVersion(version);
		MSBaseBuilding::Serialize(ar);

		ar >> m_Name;
		ar >> count;
		for (int i = 0; i < count; i++)
		{
			int materialType = 0;
			ar >> materialType;
			MSMaterial* pMaterial = CreateNewMaterial(materialType);
			if (pMaterial == nullptr)	continue;
			pMaterial->SetBuilding(this);
			pMaterial->Serialize(ar);
			Add(pMaterial);
		}

		ar >> count;
		for (int i = 0; i < count; i++)
		{
			int mType = 0;
			ar >> mType;
			if (IsBHFMode() && MSVersionInfo::GetCurrentVersion() < 20240425)
			{
				if (mType == MSCompoM::msBeamM)
					mType = MSCompoM::msFWBeamM;
				else if (mType == MSCompoM::msColumnM)
					mType = MSCompoM::msFWColumnM;
				else if (mType == MSCompoM::msBWallM)
					mType = MSCompoM::msFWBWallM;
				else if (mType == MSCompoM::msSlabM)
					mType = MSCompoM::msFWSlabM;
			}

			MSCompoM* shapeTemplate = CreateNewCompoM(mType);
			if (shapeTemplate == nullptr)	continue;
			shapeTemplate->SetOwner(this);
			shapeTemplate->Serialize(ar);

			// F22-285 거푸집 객체 프로퍼티 정리
			// 사용자 기둥 삭제 사용하지 않는다. 20220803 객체 프로퍼티 정리
			if (shapeTemplate->GetShapeName() == MSRcFWColumnM::ToShapeName(MSRcFWColumnM::Custom))
			{
				delete shapeTemplate;
				continue;
			}
			AddShapeTemplate(shapeTemplate);
		}

		ar >> count;
		for (int i = 0; i < count; i++)
		{
			MSFormWorkDesignUnit* pDesignUnit = CreateDesignUnit();
			pDesignUnit->Serialize(ar);
			Add(pDesignUnit);
		}

		// QT 갱신하는 로직이 필요함
		vector<MSFloor*> floorArr = GetAllFloors();
		for (int i = 0; i < floorArr.size(); i++)
		{
			vector<MSCompoG*> compoGArr;
			MSFloor* pFloor = floorArr[i];
			for (auto itCompoG = pFloor->mm_CompoG.begin(); itCompoG != pFloor->mm_CompoG.end(); itCompoG++)
				compoGArr.push_back(itCompoG->second);
			pFloor->CheckFloorQT(compoGArr);
		}

		if (MSVersionInfo::GetCurrentVersion() < 20191024)
		{
			MSCompoM* shapeTemplate = CreateNewCompoM(MSCompoM::msColumnM);
			if (shapeTemplate != nullptr)
			{
				MSRcFWColumnM* rcFWColumnM = dynamic_cast<MSRcFWColumnM*>(shapeTemplate);
				if (rcFWColumnM != nullptr)
				{
					double w = 600, d = 600;
					rcFWColumnM->SetFWColumnType(MSRcFWColumnM::Custom);
					rcFWColumnM->SetShapeName(rcFWColumnM->ToShapeName(MSRcFWColumnM::Custom));
					rcFWColumnM->SetName(_T("NONAME"));
					rcFWColumnM->SetW(w);
					rcFWColumnM->SetD(d);
					AddShapeTemplate(shapeTemplate);
				}
				else
				{
					delete shapeTemplate;
				}
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20200507)
		{
			mp_UCS->Serialize(ar);
		}

		// MSBaseBuilding::Serialize() 에서 저장 처리
		// 이 코드는 TypeUIDFactory 복구 하는 코드이다,
		if (MSVersionInfo::GetCurrentVersion() < 20200703)
		{
			RecoverTypeUIDFactoryByMSObjectLastID();
		}
	}
}

void MSFormWorkBuilding::RecoverTypeUIDFactoryByMSObjectLastID()
{
	if (m_pTypeUIDFactory == nullptr)
		m_pTypeUIDFactory = new TypeUIDFactory();
	m_pTypeUIDFactory->Clear();

	/*
	enum UIDType
	msElement = 0, msCompoM,
	xErrorObject, ropObject, msMaterial, msReinforceBar, msReinforceBarSetting, msMarkM, msCustomBarUnit, msCustomReBarLine, msCustomSection, msRibTypeM,
	msWallEndRebarLine, msWallEndRebarM, msBarSet, msRebarShape, dtStirrup, rebarTemplateManager
	위 클래스 초기화
	*/

	UIDFactory* pUIDFactory = m_pTypeUIDFactory->GetUIDFactory(UIDType::msElement);
	pUIDFactory->SetCurrentID(MSElement::ms_LastID);

	pUIDFactory = m_pTypeUIDFactory->GetUIDFactory(UIDType::msCompoM);
	pUIDFactory->SetCurrentID(MSCompoM::ms_LastID);

	pUIDFactory = m_pTypeUIDFactory->GetUIDFactory(UIDType::xErrorObject);
	pUIDFactory->SetCurrentID(XErrorObject::ms_LastID);
	
	pUIDFactory = m_pTypeUIDFactory->GetUIDFactory(UIDType::msMaterial);
	pUIDFactory->SetCurrentID(MSMaterial::ms_LastID);
}

#pragma region override
bool MSFormWorkBuilding::WriteFileBinary()
{
	// TODO : ?뺤옣???뺥븯湲?
	CString strFilter = _T("*.fwb");
	if (m_strFilePath == _T(""))
	{
		CFileDialog FileDlg(FALSE, _T(""), strFilter);
		if (FileDlg.DoModal() == IDCANCEL)
			return false;

		m_strFilePath = FileDlg.GetPathName();
	}
	return WriteFileBinary(m_strFilePath);
}

bool MSFormWorkBuilding::WriteFileBinary(CString strFilePath)
{
	CFile file;
	CFileException e;
	try
	{
		if (file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite, &e) == FALSE)
		{
			{
#ifdef _DEBUG
				afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
			}
		}
		CArchive ar(&file, CArchive::store);
		Serialize(ar);
	}
	catch (...)
	{
		throw new exception("Unknown exception occurred in FormwokBuilding WriteFileBinary");
	}

	return true;
}

bool MSFormWorkBuilding::ReadFileBinary(CString strFilePath)
{
	CFile file;
	CFileException e;
	try
	{
		if (file.Open(strFilePath, CFile::modeRead, &e) == FALSE)
		{
			{
#ifdef _DEBUG
				afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
			}
		}
		if (file.GetLength() <= 0)	return false;
		CArchive ar(&file, CArchive::load);
		Serialize(ar);
	}
	catch (...)
	{
		throw new exception("Unknown exception occurred in MSBuilding ReadFileBinary");
	}
	return true;

}

MSCompoM* MSFormWorkBuilding::CreateNewCompoM(long nType)
{
	MSCompoM* pCompoM = NULL;
	switch (nType)
	{
		//Line
	case MSCompoM::msFWBeamM:	pCompoM = new MSRcFWBeamM();	break;
	case MSCompoM::msFWColumnM: pCompoM = new MSRcFWColumnM();	break;
	case MSCompoM::msFWBWallM:	pCompoM = new MSRcFWBWallM();	break;
	case MSCompoM::msFWSlabM:	pCompoM = new MSRcFWSlabM();	break;
	case MSCompoM::msFWOpeningM:	pCompoM = new MSFWOpeningM();	break;
	case MSCompoM::msFWShadeM: pCompoM = new MSFWShadeM();	break;
	case MSCompoM::msFWSwitchBoxM: pCompoM = new MSFWSwitchBoxM();	break;
	case MSCompoM::msFWSoundAbsorptionM: pCompoM = new MSFWSoundAbsorptionM();	break;
	case MSCompoM::msFWSurfaceFoldingM:	pCompoM = new MSFWSurfaceFoldingM();	break;
	case MSCompoM::msFWTrenchM:	pCompoM = new MSFWTrenchM();	break;
	}
	if (pCompoM == NULL)	return MSBaseBuilding::CreateNewCompoM(nType);

	pCompoM->m_nType = (MSCompoM::Type_MSCompoM)nType;
	pCompoM->mp_Owner = this;
	return pCompoM;
}

MSCompoG* MSFormWorkBuilding::GetCompoG(long CompoGID)
{
	return GetCompoGByFloors(CompoGID);
}

MSCompoG* MSFormWorkBuilding::GetCompoGByFloors(long CompoGID)
{
	vector<MSFloor*> FloorList = GetAllFloors();
	for (int iFloor = 0; iFloor < FloorList.size(); iFloor++)
	{
		MSFloor* pFloor = FloorList[iFloor];
		if (pFloor == nullptr)
			continue;
		MSCompoG* pFindCompoG = pFloor->GetCompoG(CompoGID);
		if (pFindCompoG != nullptr)
			return pFindCompoG;
	}

	return nullptr;
}


#pragma endregion

#pragma region ShapeTemplate
void MSFormWorkBuilding::DeleteShapeTemplates()
{
	for (auto it = mm_ShapeTemplate.begin(); it != mm_ShapeTemplate.end(); it++)
		delete it->second;
	mm_ShapeTemplate.clear();
}
void MSFormWorkBuilding::AddShapeTemplate(MSCompoM* shapeTemplate)
{
	shapeTemplate->SetOwner(this);
	mm_ShapeTemplate.insert(make_pair(shapeTemplate->GetID(), shapeTemplate));
}
MSCompoM* MSFormWorkBuilding::GetShapeTemplate(long shapeTemplateID)
{
	auto it = mm_ShapeTemplate.find(shapeTemplateID);
	if (it != mm_ShapeTemplate.end())	return it->second;
	return nullptr;
}
vector<MSCompoM*> MSFormWorkBuilding::GetShapeTemplates()
{
	vector<MSCompoM*> templateList;
	for (auto it = mm_ShapeTemplate.begin(); it != mm_ShapeTemplate.end(); it++)
		templateList.push_back(it->second);
	return templateList;
}

MSCompoM* MSFormWorkBuilding::GetShapeTemplate(CString ShapeName)
{
	for (auto it = mm_ShapeTemplate.begin(); it != mm_ShapeTemplate.end(); it++)
	{
		if (it->second->GetShapeName() == ShapeName)
			return it->second;
	}

	return nullptr;
}

bool MSFormWorkBuilding::Add(MSCompoM* pCompoM)
{
	pair<map<long, MSCompoM*>::iterator, bool> bPair;
	bPair = mm_ShapeTemplate.insert(make_pair(pCompoM->m_ID, pCompoM));
	return bPair.second;
}

MSCompoM* MSFormWorkBuilding::GetCompoM(long CompoMID)
{
	map<long, MSCompoM*>::iterator it = mm_ShapeTemplate.find(CompoMID);
	if (it == mm_ShapeTemplate.end())
	{
		return NULL;
	}

	return it->second;
}

long MSFormWorkBuilding::Remove(MSCompoM* pCompoM)
{
	long nID = pCompoM->m_ID;
	map<long, MSCompoM*>::iterator it = mm_ShapeTemplate.find(nID);
	if (it == mm_ShapeTemplate.end())	return 0;
	mm_ShapeTemplate.erase(it);
	return nID;
}

#pragma endregion

#pragma region MSFormWorkDesignUnit
MSFormWorkDesignUnit* MSFormWorkBuilding::CreateDesignUnit(double dHeight)
{
	MSFormWorkDesignUnit* pDesignUnit = new MSFormWorkDesignUnit(dHeight);
	pDesignUnit->SetBuilding(this);
	return pDesignUnit;
}

void MSFormWorkBuilding::DeleteDesignUnits()
{
	for (auto it = mm_DesignUnit.begin(); it != mm_DesignUnit.end(); it++)
		delete it->second;
	mm_DesignUnit.clear();
}

void MSFormWorkBuilding::Add(MSFormWorkDesignUnit* pDesignUnit)
{
	pDesignUnit->SetFloorIDs();
	mm_DesignUnit.insert(make_pair(pDesignUnit->GetID(), pDesignUnit));
}

long MSFormWorkBuilding::Remove(MSFormWorkDesignUnit* pDesignUnit)
{
	long nID = pDesignUnit->m_ID;
	auto it = mm_DesignUnit.find(nID);
	if (it == mm_DesignUnit.end())	return 0;
	mm_DesignUnit.erase(it);
	return nID;
}

MSFormWorkDesignUnit* MSFormWorkBuilding::GetDesignUnit(long designUnitID)
{
	auto it = mm_DesignUnit.find(designUnitID);
	if (it == mm_DesignUnit.end())	return nullptr;
	return it->second;
}

vector<MSFormWorkDesignUnit*> MSFormWorkBuilding::GetDesignUnits()
{
	vector<MSFormWorkDesignUnit*> designUnitList;
	for (auto it = mm_DesignUnit.begin(); it != mm_DesignUnit.end(); it++)
		designUnitList.push_back(it->second);
	return designUnitList;
}
#pragma endregion

#pragma region MSMaterial

MSMaterial* MSFormWorkBuilding::CreateNewMaterial(long nType)
{
	return MSBaseBuilding::CreateNewMaterial(nType);
}

void MSFormWorkBuilding::DeleteMaterials()
{
	for (auto it = mm_Material.begin(); it != mm_Material.end(); it++)
		delete it->second;
	mm_Material.clear();
}

bool MSFormWorkBuilding::Add(MSMaterial* pMaterial)
{
	pair<map<long, MSMaterial*>::iterator, bool> bPair;
	bPair = mm_Material.insert(make_pair(pMaterial->GetID(), pMaterial));
	return bPair.second;
}

MSMaterial* MSFormWorkBuilding::GetMaterial(long MaterialID)
{
	auto it = mm_Material.find(MaterialID);
	if (it == mm_Material.end())	return nullptr;
	return it->second;
}

MSMaterial* MSFormWorkBuilding::GetMaterial(CString strMateName)
{
	map<long, MSMaterial*>::iterator it;
	for (it = mm_Material.begin(); it != mm_Material.end(); it++)
	{
		MSMaterial* pMate = it->second;
		if (pMate == NULL)	continue;
		if (pMate->m_Name == strMateName.MakeUpper())
			return pMate;
	}
	return NULL;
}

long MSFormWorkBuilding::Remove(MSMaterial* pMaterial)
{
	long nID = pMaterial->m_ID;
	map<long, MSMaterial*>::iterator itMaterial = mm_Material.find(nID);
	if (itMaterial == mm_Material.end())	return 0;
	mm_Material.erase(itMaterial);
	return nID;
}

long MSFormWorkBuilding::Delete(MSMaterial* pMaterial)
{
	long nID = Remove(pMaterial);
	delete pMaterial;
	return nID;
}

vector<MSMaterial*> MSFormWorkBuilding::GetMaterias()
{
	vector<MSMaterial*> materialList;
	for (auto it = mm_Material.begin(); it != mm_Material.end(); it++)
		materialList.push_back(it->second);

	return materialList;
}

MSMaterial* MSFormWorkBuilding::GetDefMaterial()
{
	map<long, MSMaterial*>::iterator it;
	for (it = mm_Material.begin(); it != mm_Material.end(); it++)
	{
		MSMaterial* pMaterial = it->second;
		if (pMaterial == nullptr || pMaterial->m_nType != MSMaterial::Type_MSMaterial::msFormWork) continue;
		if (pMaterial)	return pMaterial;
	}
	return nullptr;
}

MSMaterial* MSFormWorkBuilding::GetDefMaterial(MSMaterial::Type_MSMaterial materialType)
{
	return GetDefMaterial();
}

MSMaterial* MSFormWorkBuilding::GetDefMaterialByFWMaterialType(MSFormWorkMaterial::eFWMaterialType eType)
{
	map<long, MSMaterial*>::iterator it;
	for (it = mm_Material.begin(); it != mm_Material.end(); it++)
	{
		MSMaterial* pMaterial = it->second;
		if (pMaterial == nullptr || pMaterial->m_nType != MSMaterial::Type_MSMaterial::msFormWork) continue;
		MSFormWorkMaterial* pMSFWMaterial = dynamic_cast<MSFormWorkMaterial*>(pMaterial);
		if (pMSFWMaterial == nullptr)
			continue;
		if (pMSFWMaterial->GetFWMaterialType() == eType)
			return pMSFWMaterial;
	}
	return nullptr;
}

MSMaterial* MSFormWorkBuilding::GetMaterialbyName(MSMaterial::Type_MSMaterial materialType, CString name)
{
	map<long, MSMaterial*>::iterator it;
	for (it = mm_Material.begin(); it != mm_Material.end(); it++)
	{
		MSMaterial* pMate = it->second;
		if (pMate == NULL)	continue;
		if (pMate->m_Name == name.MakeUpper() && pMate->GetType() == materialType)
			return pMate;
	}
	return NULL;
}

vector<MSMaterial*> MSFormWorkBuilding::GetMaterialsByType(MSMaterial::Type_MSMaterial type)
{
	vector<MSMaterial*> materialArr;
	for (auto it = mm_Material.begin(); it != mm_Material.end(); it++)
	{
		if (it->second->GetType() == type)
			materialArr.push_back(it->second);
	}
	return materialArr;
}

#pragma endregion

#pragma region MSFloor

MSFloor* MSFormWorkBuilding::GetFloorByID(long FloorID)
{
	for (auto it = mm_DesignUnit.begin(); it != mm_DesignUnit.end(); it++)
	{
		MSFloor* pFloor = it->second->GetFloorByID(FloorID);
		if (pFloor != nullptr)
			return pFloor;
	}
	return nullptr;
}

double MSFormWorkBuilding::GetFloorLevel(long FloorNum)
{
	if (FloorNum == 0)  // Block Object
		return 0.;

	long nFrom = -GetNumUnderStory();
	long nTo = GetNumAboveStory();
	double dTotalHeight = GetGroundLevel();

	if (FloorNum > 0)
	{
		for (long iStory = 1; iStory < FloorNum; iStory++)
		{
			MSFloor* pFloor = GetFloor(iStory);
			if (pFloor == NULL)	continue;
			dTotalHeight += pFloor->GetHeight();
		}
	}
	else
	{
		for (long iStory = -1; iStory >= FloorNum; iStory--)
		{
			MSFloor* pFloor = GetFloor(iStory);
			if (pFloor == NULL)	continue;
			dTotalHeight -= pFloor->GetHeight();
		}
	}
	return dTotalHeight;
}

double MSFormWorkBuilding::GetSlabLevel(long FloorNum)
{
	MSFloor* pFloor = GetFloor(FloorNum);
	if (pFloor == NULL)
		return GetFloorLevel(FloorNum);
	return GetFloorLevel(FloorNum) + pFloor->GetSlabLevelOffset();
}

vector<MSFloor*> MSFormWorkBuilding::GetAllFloors()
{
	vector<MSFloor*> floors;
	for (auto it = mm_DesignUnit.begin(); it != mm_DesignUnit.end(); it++)
	{
		floors.push_back(it->second->GetTopFloor());
		floors.push_back(it->second->GetWorkingFloor());
		floors.push_back(it->second->GetBottomFloor());
	}
	return floors;
}

void MSFormWorkBuilding::ClearAllCashFloorLevel()
{
	for (auto it = mm_DesignUnit.begin(); it != mm_DesignUnit.end(); it++)
	{
		it->second->GetTopFloor()->ClearCashFloorLevel();
		it->second->GetWorkingFloor()->ClearCashFloorLevel();
		it->second->GetBottomFloor()->ClearCashFloorLevel();
	}
}

#pragma endregion

#pragma region MSCompoGs
vector<MSCompoG*> MSFormWorkBuilding::GetAllCompoGs()
{
	vector<MSCompoG*> compoGArr;
	vector<MSFloor*> floorArr = GetAllFloors();
	for (int idx = 0; idx < floorArr.size(); idx++)
	{
		for (auto itr = floorArr[idx]->mm_CompoG.begin(); itr != floorArr[idx]->mm_CompoG.end(); ++itr)
			compoGArr.push_back(itr->second);
	}

	return compoGArr;
}

#pragma endregion

void MSFormWorkBuilding::ChangFloorQT(double dMinX, double dMinY, double dMaxX, double dMaxY)
{
	dMinX = min(m_SiteRect.m_Origin.m_X, dMinX);
	dMinY = min(m_SiteRect.m_Origin.m_Y, dMinY);
	dMaxX = max(m_SiteRect.m_Corner.m_X, dMaxX);
	dMaxY = max(m_SiteRect.m_Corner.m_Y, dMaxY);

	if (fabs(m_SiteRect.m_Origin.m_X - dMinX) < DTOL_GM &&
		fabs(m_SiteRect.m_Origin.m_Y - dMinY) < DTOL_GM &&
		fabs(m_SiteRect.m_Corner.m_X - dMaxX) < DTOL_GM &&
		fabs(m_SiteRect.m_Corner.m_Y - dMaxY) < DTOL_GM)
		return;


	m_SiteRect.m_Origin.SetXY(dMinX, dMinY);
	m_SiteRect.m_Corner.SetXY(dMaxX, dMaxY);

	vector<MSFloor*> floorArr = GetAllFloors();
	for (int iFloor = 0; iFloor < floorArr.size(); iFloor++)
	{
		MSFloor* pFloor = floorArr[iFloor];
		if (pFloor == nullptr)
			continue;

		pFloor->SetSite();
		pFloor->ResetFloorQTLeaves();
	}
}

bool MSFormWorkBuilding::IsVerticalCompoM(long nType)
{
	if (nType == MSCompoM::msColumnM || nType == MSCompoM::msBWallM || nType == MSCompoM::msRWallM || nType == MSCompoM::msPcColumnM || nType == MSCompoM::msCustomColumnM || nType == MSCompoM::msCustomWallColumnM)
		return true;
	else if (nType == MSCompoM::msBraceM || nType == MSCompoM::msButtressM)
		return true;
	else if (nType == MSCompoM::msSteelColumnM || nType == MSCompoM::msSrcColumnM)
		return true;
	return false;
}

map<long, MSMaterial*>* MSFormWorkBuilding::GetMaterialMap()
{
	return &mm_Material;
}
