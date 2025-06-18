#include "StdAfx.h"
#include "MSRebarBuilding.h"
#include "MSGlobalOption.h"
#include "MSCompoM.h"
#include "MSFloor.h"
#include "MSGroup.h"
#include "MSGridLine.h"
#include "GMLib/MSVersionInfo.h"
#include "MSConcrete.h"
#include "MSSteel.h"
#include "XMSRowM.h"
#include "MSFaceMemberM.h"
#include "MSMarkM.h"
#include "MSMarkG.h"
#include "MSReinforceBar.h"
#include "ReinforceBarSetting.h"
#include "MSSrcMaterial.h"
#include "MSBlock.h"
#include "MSRibTypeM.h"
#include "MSStandardRibTypeM.h"
#include "MSSpacerShape.h"
#include "GMLib/XErrorContainer.h"
#include "GMLib/XErrorObject.h"
#include "GMLib\GM2DBound.h"
#include "MSConstructionZone.h"
#include "MSConnection.h"
#include "MSCompoGUtil.h"

#include "RADFCore_CLI/LoggerWrapper.h"
#include "RADFCore_CLI/StatusReporterWrapper.h"

#include "RADFCore_CLI/AppGlobalOptionWrapper.h"
#include "RADFCore_CLI/ProgressReporterWrapper.h"
#include "Radfcore_CLI/AppOptionsWrapper.h"
#include "ActivateLib_CLI/Activate.h"
#include "ActivateLib_CLI/AppLicenseWrapper.h"
#include "MSModelChecker.h"
#include "MSOpeningM.h"
#include "MSStoryMaterial.h"
#include "MSWallEndRebarLine.h"
#include "MSSteelUtility.h"
#include "MSRcLineMemberM.h"
#include "MSRcSlabM.h"
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
#include "MSCustomM.h"
#include "MSCustomSection.h"
#include "MSCustomLineSection.h"
#include "RebarTemplateManager.h"
#include "RADFCore_CLI/RadfContextWrapper.h"
#include "MSRibTypeM.h"
#include "MSStandardRibTypeM.h"
#include "MSBuildingUCS.h"
#include "MSSectionView.h"
#include "GMlib/TypeUIDFactory.h"
#include "GMlib/UIDFactory.h"

//#include "../MSRAP/ROPObject.h"
#include "MSCustomReBarLine.h"
#include "MSBarSet.h"
#include "MSWallEndRebarM.h"
#include "DTStirrup.h"

#include "FormWorkTemplateManager.h"
#include "NayukTemplateManager.h"
#include "MSCustomColumnSection.h"
#include "MSCustomWallColumnSection.h"
#include "MSCustomBeamSection.h"
#include "MSBuildingInfo.h"


IMPLEMENT_SERIAL(MSRebarBuilding, MSBaseBuilding, VERSIONABLE_SCHEMA | 1)


double MSRebarBuilding::ms_CMColinearDegreeTol = 5.0;
double MSRebarBuilding::ms_CMColumnRatio = 95.0;
double MSRebarBuilding::ms_CMWallRatio = 95.0;
double MSRebarBuilding::ms_CMWallFootRatio = 5.0;
double MSRebarBuilding::ms_CMSlabRatio = 95.0;

MSRebarBuilding::MSRebarBuilding(void)
{
	m_pSteelUtil = NULL;

	Init(true);

	m_pAreaTemplateManager = new MSAreaTemplateManager();
	m_pBuildingInfoManager = new MSBuildingInfoManager();

	DummyCompoGIDListClear();

	LoggerWrapper::Log("MSBuilding initialized");
}

MSRebarBuilding::~MSRebarBuilding(void)
{
	try
	{
		LoggerWrapper::Log("Deleting MSRebarBuilding....");
		DeleteGroups();//반드시 Group을 먼저 삭제해야 한다. Floor에는 compoG들이 있기 때문
		DeleteFloors();
		DeleteCompoMs();
		DeleteMaterials();
		DeleteAssemblys();
		DeleteGridLines();
		DeleteCompoGs();
		DeleteReinforceBars();
		DeleteReinforceBarSettings();
		DeleteMarkMs();
		DeleteMarkGs();
		DeleteRibTypes();
		DeleteSpacerShapes();
		DeleteStoryMaterials();
		DeleteWallEndRebarLines();
		if (m_pSGradeManager)
			delete m_pSGradeManager;

		DeleteCustomSections();
		DeleteSteelUtil();

		delete m_pAreaTemplateManager;
		delete m_pBuildingInfoManager;
		LoggerWrapper::Log("MSBuilding Deleted");
	}
	catch (const std::exception& ex)
	{
		CString errorMsg;
		errorMsg.Format(L"Standard exception occurred in Building Destructor: %S", ex.what());
		StatusReporterWrapper::OutputError(errorMsg);
	}
#ifndef _DEBUG
	catch (...)
	{
		StatusReporterWrapper::OutputError(L"Unexpected exception occurred in Building destructor!");
	}
#endif
}

// 
// CString MSBuilding::GetGUIDString()
// {
// 	wchar_t wszCLSID[129];
// 	StringFromGUID2(m_GUID, wszCLSID, 128);
// 	CString sGUID;
// 	sGUID.Format(_T("%s"), wszCLSID);
// 	return sGUID;
// }

MSSteelUtility* MSRebarBuilding::GetSteelUtili()
{
	if (m_pSteelUtil)	return m_pSteelUtil;
	else
		m_pSteelUtil = new MSSteelUtility;
	return m_pSteelUtil;
}

void MSRebarBuilding::DeleteSteelUtil()
{
	if (m_pSteelUtil != NULL)
		delete m_pSteelUtil;

	m_pSteelUtil = NULL;
}

void MSRebarBuilding::SetCMColinearDegreeTol(double dTol)
{
	MSRebarBuilding::ms_CMColinearDegreeTol = dTol;
}

void MSRebarBuilding::SetCMColumnRatio(double dRatio)
{
	MSRebarBuilding::ms_CMColumnRatio = dRatio;
}

void MSRebarBuilding::SetCMWallRatio(double dRatio)
{
	MSRebarBuilding::ms_CMWallRatio = dRatio;
}

void MSRebarBuilding::SetCMWallFootRatio(double dRatio)
{
	MSRebarBuilding::ms_CMWallFootRatio = dRatio;
}

void MSRebarBuilding::SetCMSlabRatio(double dRatio)
{
	MSRebarBuilding::ms_CMSlabRatio = dRatio;
}


MSGridLine* MSRebarBuilding::GetGLine(long GLineID)
{
	map<long, MSGridLine*>::iterator it = mm_GridLine.find(GLineID);
	if (it == mm_GridLine.end())
		return NULL;

	return it->second;
}

void MSRebarBuilding::GetCompoMFloorName(long nMID, CString& strFrom, CString& strTo)
{
	CString strStroySection = _T("");
	MSCompoM* pCompoM = GetCompoM(nMID);
	if (pCompoM == NULL)	return;
	long nFromStory = pCompoM->m_nFromStory;
	long nTostory = pCompoM->m_nToStory;
	strFrom = GetFloorName(nFromStory);
	strTo = GetFloorName(nTostory);
}

CString MSRebarBuilding::GetFloorName(long nStroy)
{
	MSFloor* pMSFloor = GetFloor(nStroy);
	if (pMSFloor == NULL) return _T("");

	CString pFloorName = _T("");
	pFloorName = pMSFloor->GetName();

	return pFloorName;
}

bool MSRebarBuilding::WriteFileBinary()
{
	CString strFilter = _T("*.bldg");
	if (m_strFilePath == _T(""))
	{
		CFileDialog FileDlg(FALSE, _T(""), strFilter);
		if (FileDlg.DoModal() == IDCANCEL)
			return false;

		m_strFilePath = FileDlg.GetPathName();
	}
	return WriteFileBinary(m_strFilePath);
}

bool MSRebarBuilding::WriteFileBinary(CString strFilePath)
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
	catch (exception ex)
	{
		throw new exception("Unknown exception occurred in MSBuilding WriteFileBinary");
	}

	return true;
}

bool MSRebarBuilding::ReadFileBinary(CString strFilePath)
{
	CFile file;
	CFileException e;
	CFileStatus status;
	if (!CFile::GetStatus(strFilePath, status))
		return false;
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

		long nCount = 0;

		map<long, MSFloor*>::iterator itFloor;
		for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
		{
			MSFloor* pFloor = itFloor->second;
			map<long, long>::iterator itFloor;
			for (itFloor = pFloor->tmep_CloneMap.begin(); itFloor != pFloor->tmep_CloneMap.end(); itFloor++)
			{
				long FloorID = itFloor->first;
				pFloor->ma_Clone.push_back(GetFloorByID(FloorID));
			}

			if (pFloor->tmep_SourceID)
				pFloor->mp_Source = GetFloorByID(pFloor->tmep_SourceID);

			map<long, MSCompoG*>::iterator itCompoG;
			for (itCompoG = pFloor->mm_CompoG.begin(); itCompoG != pFloor->mm_CompoG.end(); itCompoG++)
			{
				MSCompoG* pCompoG = itCompoG->second;
				map<long, long>::iterator itClone;
				for (itClone = pCompoG->tmep_CloneMap.begin(); itClone != pCompoG->tmep_CloneMap.end(); itClone++)
				{
					MSFloor* pCloneOwner = GetFloorByID(itClone->first);
					if (pCloneOwner)
					{
						MSCompoG* pCloneCompoG = pCloneOwner->GetCompoG(itClone->second);
						if (pCloneCompoG)	pCompoG->ma_Clone.push_back(pCloneCompoG);
					}
				}
			}
		}
	}
	catch (exception ex)
	{
		throw new exception("Unknown exception occurred in MSBuilding ReadFileBinary");
	}
	// mylee - RegenList Clear after Open File
	ClearRegenList();

	// 	map<long, MSMaterial*>::iterator itMaterial;
	// 	for(itMaterial=mm_Material.begin(); itMaterial != mm_Material.end(); itMaterial++)
	// 	{
	// 		MSMaterial* pMate = itMaterial->second;
	// 		nCount++;
	// 	}
	// 	nCount = 0;
	// 	map<long, MSCompoM*>::iterator itCompoM;
	// 	for(itCompoM=mm_CompoM.begin(); itCompoM != mm_CompoM.end(); itCompoM++)
	// 	{
	// 		MSCompoM* pCompoM = itCompoM->second;
	// 		nCount++;
	// 	}
	// 
	// 	nCount = 0;
	// 	map<long, MSJoint*>::iterator itJoint;
	// 	for(itJoint=mm_Joint.begin(); itJoint != mm_Joint.end(); itJoint++)
	// 	{
	// 		MSJoint* pJoint = itJoint->second;
	// 		nCount++;
	// 	}

		//CheckMidNodes();
		//nCount = 0;
		//map<long, MSFloor*>::iterator itFloor;
		//for(itFloor=mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
		//{
		//	MSFloor* pFloor = itFloor->second;
		//	pFloor->ShowMembers();
		//}

	// 	// Test Steel Section
	// 	// 아래 함수가 호출되면, 해당국가코드의 Steel Section이 메모리에 올라온다.
	// 	// 현재 한국코드만 존재
	MSSteelUtility::m_nSteelCode = STEEL_CODE_KOREAN;
	MSSteelUtility* pUtil = GetSteelUtili();


	// 	// 읽혔는지 테스트
	// 	MSSteelSection* pHSection = pUtil->GetSection(SECTION_H_TYPE, 1);
	// 	MSSteelSection* pRectSection = pUtil->GetSection(SECTION_RECT_TYPE, 1);
	// 	MSSteelSection* pRoundSection = pUtil->GetSection(SECTION_ROUND_TYPE, 1);
	// 	MSSteelSection* pTSection = pUtil->GetSection(SECTION_T_TYPE, 1);
	// 	MSSteelSection* pCSection = pUtil->GetSection(SECTION_C_TYPE, 1);
	// 	MSSteelSection* pDAngleSection = pUtil->GetSection(SECTION_DANGLE_TYPE, 1);
	// 	MSSteelSection* pAngleSection = pUtil->GetSection(SECTION_ANGLE_TYPE, 1);
	// 	MSSteelSection* pDeckSection = pUtil->GetSection(SECTION_DECK_TYPE, 1);

		// m_QuantityOption.m_strMaterial 값이 없으면 RcMaterial 처음값 셋팅
	InitQuantityOptionMaterial();

	//	WriteFile_4KangjiHawan();
	return true;

}

void MSRebarBuilding::DeleteFloors()
{
	map<long, MSFloor*>::iterator itFloor;
	for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
		delete itFloor->second;
	mm_Floor.clear();
}

// by Gracias
// CmdAddFloor, CmdDelFloor 를 통해서 AddFloor, Remove가 호출된다는 것은, 다른 Floor의 m_FloorNum이 변경되었을 수도 있다.
// 따라서 변경된 MSFloor::m_nFloorNum으로 mm_Floor를 다시 만들어준다.
void MSRebarBuilding::ReMakeFloorMap(long deleteID)
{
	vector<MSFloor*> floors;
	map<long, MSFloor*>::iterator it;
	for (it = mm_Floor.begin(); it != mm_Floor.end(); it++)
	{
		floors.push_back(it->second);
	}

	mm_Floor.clear();
	int nFloor = floors.size();
	for (int iFloor = 0; iFloor < nFloor; iFloor++)
	{
		MSFloor* pFloor = floors[iFloor];
		if (pFloor->m_ID == deleteID)
			continue;
		mm_Floor.insert(make_pair(pFloor->GetFloorNum(), pFloor));
	}

	//층이 삭제되는 경우 compoG->SetOverlapFloor()를 호출해서  오버랩되는 Floor를 다시 계산해야 한다.
	for (it = mm_Floor.begin(); it != mm_Floor.end(); it++)
	{
		for (auto compoGIt = it->second->mm_CompoG.begin(); compoGIt != it->second->mm_CompoG.end(); compoGIt++)
			compoGIt->second->SetOverlapFloor();
	}
}

void MSRebarBuilding::AddFloor(MSFloor* floor)
{
	floor->GetID();
	floor->SetOwner(this);

	floor->SetSite(&m_SiteRect);
	
	CString strFloorClassify = LocalDic::GetLocal(_T("지상층"));
	if (floor->GetFloorNum() < 0)
		strFloorClassify = LocalDic::GetLocal(_T("지하층"));
	floor->SetFloorClassification(strFloorClassify);

	ReMakeFloorMap();

	pair<map<long, MSFloor*>::iterator, bool> bPair;
	bPair = mm_Floor.insert(make_pair(floor->GetFloorNum(), floor));
}

MSFloor* MSRebarBuilding::AddNewFloor(long FloorNum)
{
	MSFloor* pNewFloor = new MSFloor;
	//	pNewFloor->m_ID = FloorNum;
	pNewFloor->GetID();
	pNewFloor->SetFloorNum(FloorNum);

	pNewFloor->SetOwner(this);

	pair<map<long, MSFloor*>::iterator, bool> bPair;
	bPair = mm_Floor.insert(make_pair(FloorNum, pNewFloor));

	if (bPair.second)
		return pNewFloor;
	else
		return NULL;
}

MSFloor* MSRebarBuilding::GetFloor(long FloorNum)
{
	mm_Floor.find(FloorNum);

	map<long, MSFloor*>::iterator it = mm_Floor.find(FloorNum);
	if (it == mm_Floor.end())
		return NULL;

	return it->second;
}

MSFloor* MSRebarBuilding::GetFloorByID(long FloorID)
{
	map<long, MSFloor*>::iterator it;
	for (it = mm_Floor.begin(); it != mm_Floor.end(); it++)
	{
		MSFloor* pFloor = it->second;
		if (pFloor == NULL)	continue;
		if (pFloor->m_ID == FloorID)	return pFloor;
	}
	return GetBlock(FloorID);

	return NULL;
}

long MSRebarBuilding::Remove(MSFloor* pFloor)
{
	long nID = pFloor->m_ID;
	long nFloorNum = pFloor->GetFloorNum();

	ReMakeFloorMap(nID);

	return nID;
}

long MSRebarBuilding::Delete(MSFloor* pFloor)
{
	long nID = Remove(pFloor);
	delete pFloor;
	return nID;
}

long MSRebarBuilding::GetNumAboveStory()
{
	long NumAboveStory = 0;
	map<long, MSFloor*>::iterator it;
	for (it = mm_Floor.begin(); it != mm_Floor.end(); it++)
	{
		MSFloor* pFloor = it->second;
		if (pFloor == NULL)	continue;
		if (pFloor->GetFloorNum() > 0)	NumAboveStory++;
	}
	return NumAboveStory;
}

long MSRebarBuilding::GetNumUnderStory()
{
	long NumUnderStory = 0;
	map<long, MSFloor*>::iterator it;
	for (it = mm_Floor.begin(); it != mm_Floor.end(); it++)
	{
		MSFloor* pFloor = it->second;
		if (pFloor == NULL)	continue;
		if (pFloor->GetFloorNum() < 0)	NumUnderStory++;
	}
	return NumUnderStory;
}

bool MSRebarBuilding::IsRoofFloor(long FloorNum)
{
	MSFloor* pFloor = GetFloor(FloorNum);
	if (pFloor == nullptr)	return false;
	if (pFloor->IsRoof())	return true;
	return false;
}

bool MSRebarBuilding::IsBottomFloor(long FloorNum)
{
	if (mm_Floor.begin()->first == FloorNum)
		return true;
	else
		return false;
}

void MSRebarBuilding::SetGroundLevel(double dGroundLevel)
{
	m_dGroundLevel = dGroundLevel;
	ClearAllCashFloorLevel();
}

double MSRebarBuilding::GetFloorLevel(long FloorNum)
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

double MSRebarBuilding::GetSlabLevel(long FloorNum)
{
	MSFloor* pFloor = GetFloor(FloorNum);
	if (pFloor == NULL)
		return GetFloorLevel(FloorNum);
	return GetFloorLevel(FloorNum) + pFloor->GetSlabLevelOffset();
}

vector<MSFloor*> MSRebarBuilding::GetFloorsByLevel(double dStartLevel, double dEndLevel)
{
	vector<MSFloor*> FloorArr;

	map<long, MSFloor*>::iterator itFloor;
	for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->second;
		double dFloorBotLevel = pFloor->GetBotCompoGZLevel();
		double dFloorTopLevel = pFloor->GetTopCompoGZLevel();
		if (dStartLevel >= dFloorBotLevel && dStartLevel <= dFloorTopLevel)
			FloorArr.push_back(pFloor);
		else if (dEndLevel >= dFloorBotLevel && dEndLevel <= dFloorTopLevel)
			FloorArr.push_back(pFloor);
	}

	return FloorArr;
}

void MSRebarBuilding::DeleteCompoGs()
{
	map<long, MSCompoG*>::iterator itCompoG;
	for (itCompoG = mm_CompoG.begin(); itCompoG != mm_CompoG.end(); itCompoG++)
		delete itCompoG->second;
	mm_CompoG.clear();
}

MSCompoG* MSRebarBuilding::GetCompoG(long CompoGID)
{
	map<long, MSCompoG*>::iterator it = mm_CompoG.find(CompoGID);
	if (it == mm_CompoG.end())
		return NULL;

	return it->second;
}

MSCompoG* MSRebarBuilding::GetCompoGByFloors(long CompoGID)
{
	for (auto itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->second;
		auto itCompoG = pFloor->mm_CompoG.find(CompoGID);
		if (pFloor->mm_CompoG.end() != itCompoG)
			return itCompoG->second;
	}
	return NULL;
}

void MSRebarBuilding::DeleteMaterials()
{
	map<long, MSMaterial*>::iterator itMaterial;
	for (itMaterial = mm_Material.begin(); itMaterial != mm_Material.end(); itMaterial++)
		delete itMaterial->second;
	mm_Material.clear();
}

MSMaterial* MSRebarBuilding::CreateNewMaterial(long nType)
{
	return MSBaseBuilding::CreateNewMaterial(nType);
}

MSMaterial* MSRebarBuilding::GetMaterialbyName(MSMaterial::Type_MSMaterial materialType, CString name)
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

bool MSRebarBuilding::Add(MSMaterial* pMaterial)
{
	pair<map<long, MSMaterial*>::iterator, bool> bPair;
	bPair = mm_Material.insert(make_pair(pMaterial->m_ID, pMaterial));
	return bPair.second;
}

MSMaterial* MSRebarBuilding::GetMaterial(long MaterialID)
{
	map<long, MSMaterial*>::iterator it = mm_Material.find(MaterialID);
	if (it == mm_Material.end())
		return NULL;

	return it->second;
}

MSMaterial* MSRebarBuilding::GetMaterial(CString strMateName)
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

long MSRebarBuilding::Remove(MSMaterial* pMaterial)
{
	long nID = pMaterial->m_ID;
	map<long, MSMaterial*>::iterator itMaterial = mm_Material.find(nID);
	if (itMaterial == mm_Material.end())	return 0;
	mm_Material.erase(itMaterial);
	return nID;
}

long MSRebarBuilding::Delete(MSMaterial* pMaterial)
{
	long nID = Remove(pMaterial);
	delete pMaterial;
	return nID;
}

void MSRebarBuilding::DeleteAssemblys()
{
	map<long, MSAssembly*>::iterator itAssembly;
	for (itAssembly = mm_Assembly.begin(); itAssembly != mm_Assembly.end(); itAssembly++)
		delete itAssembly->second;
	mm_Assembly.clear();
}

bool MSRebarBuilding::Add(MSAssembly* pAssembly)
{
	pair<map<long, MSAssembly*>::iterator, bool> bPair;
	bPair = mm_Assembly.insert(make_pair(pAssembly->m_ID, pAssembly));
	return bPair.second;
}

MSAssembly* MSRebarBuilding::GetAssembly(long AssemblyID)
{
	map<long, MSAssembly*>::iterator it = mm_Assembly.find(AssemblyID);
	if (it == mm_Assembly.end())
		return NULL;

	return it->second;
}

long MSRebarBuilding::Remove(MSAssembly* pAssembly)
{
	long nID = pAssembly->m_ID;
	map<long, MSAssembly*>::iterator itAssembly = mm_Assembly.find(nID);
	if (itAssembly == mm_Assembly.end())	return 0;
	mm_Assembly.erase(itAssembly);
	return nID;
}

long MSRebarBuilding::Delete(MSAssembly* pAssembly)
{
	long nID = Remove(pAssembly);
	delete pAssembly;
	return nID;
}

void MSRebarBuilding::DeleteMarkMs()
{
	map<long, MSMarkM*>::iterator itMarker;
	for (itMarker = mm_MarkM.begin(); itMarker != mm_MarkM.end(); itMarker++)
		delete itMarker->second;
	mm_MarkM.clear();
}

MSMarkM* MSRebarBuilding::GetMarkM(long MarkerID)
{
	map<long, MSMarkM*>::iterator it = mm_MarkM.find(MarkerID);
	if (it == mm_MarkM.end())
		return NULL;

	return it->second;

}
bool MSRebarBuilding::Add(MSMarkM* pMarker)
{
	pair<map<long, MSMarkM*>::iterator, bool> bPair;
	bPair = mm_MarkM.insert(make_pair(pMarker->m_ID, pMarker));
	return bPair.second;
}

long MSRebarBuilding::Remove(MSMarkM* pMarker)
{
	long nID = pMarker->m_ID;
	map<long, MSMarkM*>::iterator itMarker = mm_MarkM.find(nID);
	if (itMarker == mm_MarkM.end())	return 0;
	mm_MarkM.erase(itMarker);
	return nID;
}
long MSRebarBuilding::Delete(MSMarkM* pMarker)
{
	long nID = Remove(pMarker);
	delete pMarker;
	return nID;
}

int MSRebarBuilding::GetNextTagNum(int type)
{
	int MaxTagNum = 0;
	map<long, MSMarkM*>::iterator itMarker;
	for (itMarker = mm_MarkM.begin(); itMarker != mm_MarkM.end(); ++itMarker)
	{
		MSMarkM* pMarkM = itMarker->second;
		if (pMarkM == NULL)	continue;
		if (pMarkM->m_nType != (MSMarkM::Type_MSMark)type)	continue;
		if (pMarkM->m_nTagNum >= MaxTagNum)
			MaxTagNum = pMarkM->m_nTagNum;
	}
	return MaxTagNum + 1;
}

MSMarkM* MSRebarBuilding::GetMarkMbyErrorCode(int type, int errorCode)
{
	map<long, MSMarkM*>::iterator itMarker;
	for (itMarker = mm_MarkM.begin(); itMarker != mm_MarkM.end(); ++itMarker)
	{
		MSMarkM* pMarkM = itMarker->second;
		if (pMarkM == NULL)	continue;
		if (pMarkM->m_nType != (MSMarkM::Type_MSMark)type)	continue;
		if (pMarkM->m_nErrorCode == errorCode)
			return pMarkM;
	}
	return NULL;
}

MSMarkM* MSRebarBuilding::GetMarkM(int type, CString comment)
{
	map<long, MSMarkM*>::iterator itM;
	for (itM = mm_MarkM.begin(); itM != mm_MarkM.end(); itM++)
	{
		MSMarkM* pMarkM = itM->second;
		if (pMarkM->m_nType == (MSMarkM::Type_MSMark)type && pMarkM->m_Comment == comment)
			return pMarkM;
	}
	return NULL;
}

void MSRebarBuilding::DeleteMarkGs()
{
	map<long, MSMarkG*>::iterator itMarker;
	for (itMarker = mm_MarkG.begin(); itMarker != mm_MarkG.end(); itMarker++)
		delete itMarker->second;
	mm_MarkG.clear();
}

vector<MSMarkG*> MSRebarBuilding::GetMarkGsfromMID(long MID)
{
	vector<MSMarkG*> markGs;
	map<long, MSMarkG*>::iterator itMarker;
	for (itMarker = mm_MarkG.begin(); itMarker != mm_MarkG.end(); itMarker++)
	{
		MSMarkG* pMarkG = itMarker->second;
		if (pMarkG->mp_MarkerM->m_ID == MID)
			markGs.push_back(pMarkG);
	}
	return markGs;
}

MSMarkG* MSRebarBuilding::GetMarkG(long MarkerID)
{
	map<long, MSMarkG*>::iterator it = mm_MarkG.find(MarkerID);
	if (it == mm_MarkG.end())
		return NULL;

	return it->second;

}
bool MSRebarBuilding::Add(MSMarkG* pMarker)
{
	pair<map<long, MSMarkG*>::iterator, bool> bPair;
	bPair = mm_MarkG.insert(make_pair(pMarker->m_ID, pMarker));
	return bPair.second;
}

long MSRebarBuilding::Remove(MSMarkG* pMarker)
{
	long nID = pMarker->m_ID;
	map<long, MSMarkG*>::iterator itMarker = mm_MarkG.find(nID);
	if (itMarker == mm_MarkG.end())	return 0;
	mm_MarkG.erase(itMarker);
	return nID;
}
long MSRebarBuilding::Delete(MSMarkG* pMarker)
{
	long nID = Remove(pMarker);
	delete pMarker;
	return nID;
}

void MSRebarBuilding::DeleteBlocks()
{
	map<long, MSBlock*>::iterator itBlock;
	for (itBlock = mm_Block.begin(); itBlock != mm_Block.end(); itBlock++)
		delete itBlock->second;
	mm_Block.clear();
}

MSBlock* MSRebarBuilding::GetBlock(long blockID)
{
	map<long, MSBlock*>::iterator it = mm_Block.find(blockID);
	if (it == mm_Block.end())
		return NULL;

	return it->second;

}
bool MSRebarBuilding::Add(MSBlock* pBlock)
{
	pair<map<long, MSBlock*>::iterator, bool> bPair;
	bPair = mm_Block.insert(make_pair(pBlock->m_ID, pBlock));
	return bPair.second;
}

long MSRebarBuilding::Remove(MSBlock* pBlock)
{
	long nID = pBlock->m_ID;
	map<long, MSBlock*>::iterator itBlock = mm_Block.find(nID);
	if (itBlock == mm_Block.end())	return 0;
	mm_Block.erase(itBlock);
	return nID;
}
long MSRebarBuilding::Delete(MSBlock* pBlock)
{
	long nID = Remove(pBlock);
	delete pBlock;
	return nID;
}

void MSRebarBuilding::DeleteGridLines()
{
	map<long, MSGridLine*>::iterator itGridLine;
	for (itGridLine = mm_GridLine.begin(); itGridLine != mm_GridLine.end(); itGridLine++)
		delete itGridLine->second;
	mm_GridLine.clear();
}

bool MSRebarBuilding::Add(MSGridLine* pGridLine)
{
	pair<map<long, MSGridLine*>::iterator, bool> bPair;
	bPair = mm_GridLine.insert(make_pair(pGridLine->m_ID, pGridLine));
	return bPair.second;
}

long MSRebarBuilding::Remove(MSGridLine* pGridLine)
{
	long nID = pGridLine->m_ID;
	map<long, MSGridLine*>::iterator itGridLine = mm_GridLine.find(nID);
	if (itGridLine == mm_GridLine.end())	return 0;
	mm_GridLine.erase(itGridLine);
	return nID;
}

long MSRebarBuilding::Delete(MSGridLine* pGridLine)
{
	long nID = Remove(pGridLine);
	delete pGridLine;
	return nID;
}

void MSRebarBuilding::DeleteReinforceBars()
{
	map<long, MSReinforceBar*>::iterator it;
	for (it = mm_ReinforceBar.begin(); it != mm_ReinforceBar.end(); it++)
		delete it->second;
	mm_ReinforceBar.clear();

	for (auto it = mm_ReinforceBarSetting.begin(); it != mm_ReinforceBarSetting.end(); it++)
		it->second->mm_IndexToReinforceBar.clear();

	MSReinforceBar::ms_LastID = 0;
}

MSReinforceBar* MSRebarBuilding::GetReinforceBar(long ID)
{
	map<long, MSReinforceBar*>::iterator it = mm_ReinforceBar.find(ID);
	if (it == mm_ReinforceBar.end())
		return NULL;

	return it->second;
}

MSReinforceBar* MSRebarBuilding::GetReinforceBar(CString Name)
{
	map<long, MSReinforceBar*>::iterator it;
	for (it = mm_ReinforceBar.begin(); it != mm_ReinforceBar.end(); it++)
	{
		if (it->second->m_Name == Name)
			return it->second;
	}

	return NULL;
}

bool MSRebarBuilding::Add(MSReinforceBar* pReinforceBar)
{
	pair<map<long, MSReinforceBar*>::iterator, bool> bPair;
	bPair = mm_ReinforceBar.insert(make_pair(pReinforceBar->m_ID, pReinforceBar));
	return bPair.second;
}

long MSRebarBuilding::Remove(MSReinforceBar* pReinforceBar)
{
	long nID = pReinforceBar->m_ID;
	map<long, MSReinforceBar*>::iterator it = mm_ReinforceBar.find(nID);
	if (it == mm_ReinforceBar.end())	return 0;
	mm_ReinforceBar.erase(it);
	return nID;
}

long MSRebarBuilding::Delete(MSReinforceBar* pReinforceBar)
{
	long nID = Remove(pReinforceBar);
	delete pReinforceBar;
	return nID;
}

void MSRebarBuilding::DeleteReinforceBarSettings()
{
	map<long, MSReinforceBarSetting*>::iterator it;
	for (it = mm_ReinforceBarSetting.begin(); it != mm_ReinforceBarSetting.end(); it++)
		delete it->second;
	mm_ReinforceBarSetting.clear();
}

MSReinforceBarSetting* MSRebarBuilding::GetReinforceBarSetting(long ID)
{
	map<long, MSReinforceBarSetting*>::iterator it = mm_ReinforceBarSetting.find(ID);
	if (it == mm_ReinforceBarSetting.end())
		return NULL;

	return it->second;
}

MSReinforceBarSetting* MSRebarBuilding::GetReinforceBarSetting(CString name)
{
	map<long, MSReinforceBarSetting*>::iterator it;
	for (it = mm_ReinforceBarSetting.begin(); it != mm_ReinforceBarSetting.end(); it++)
	{
		MSReinforceBarSetting* pSetting = it->second;
		if (pSetting->m_Name.MakeUpper() == name.MakeUpper())
			return pSetting;
	}
	return NULL;
}


bool MSRebarBuilding::Add(MSReinforceBarSetting* pReinforceBarSetting)
{
	pair<map<long, MSReinforceBarSetting*>::iterator, bool> bPair;
	bPair = mm_ReinforceBarSetting.insert(make_pair(pReinforceBarSetting->m_ID, pReinforceBarSetting));
	return bPair.second;
}

long MSRebarBuilding::Remove(MSReinforceBarSetting* pReinforceBarSetting)
{
	long nID = pReinforceBarSetting->m_ID;
	map<long, MSReinforceBarSetting*>::iterator it = mm_ReinforceBarSetting.find(nID);
	if (it == mm_ReinforceBarSetting.end())	return 0;
	mm_ReinforceBarSetting.erase(it);
	return nID;
}
long MSRebarBuilding::Delete(MSReinforceBarSetting* pReinforceBarSetting)
{
	long nID = Remove(pReinforceBarSetting);
	delete pReinforceBarSetting;
	return nID;
}

MSWallEndRebarLine* MSRebarBuilding::CreateNewWallEndRebarLine()
{
	MSWallEndRebarLine* pWallEndRebarLine = new MSWallEndRebarLine();
	return pWallEndRebarLine;
}

void MSRebarBuilding::DeleteWallEndRebarLines()
{
	map<int, MSWallEndRebarLine*>::iterator it;
	for (it = mm_WallEndRebarLine.begin(); it != mm_WallEndRebarLine.end(); it++)
		delete it->second;
	mm_WallEndRebarLine.clear();
}

MSWallEndRebarLine* MSRebarBuilding::GetWallEndRebarLine(long ID)
{
	map<int, MSWallEndRebarLine*>::iterator it = mm_WallEndRebarLine.find(ID);
	if (it == mm_WallEndRebarLine.end())
		return NULL;
	return it->second;
}

MSWallEndRebarLine* MSRebarBuilding::FindSameWallEndRebarLine(MSWallEndRebarLine* urEndRebarLine)
{
	for (auto it = mm_WallEndRebarLine.begin(); it != mm_WallEndRebarLine.end(); it++)
	{
		MSWallEndRebarLine* myEndRebarLine = it->second;
		if (myEndRebarLine->IsSameData(urEndRebarLine))
			return myEndRebarLine;
	}
	return nullptr;
}

bool MSRebarBuilding::Add(MSWallEndRebarLine* pWallEndRebarLine)
{
	pair<map<int, MSWallEndRebarLine*>::iterator, bool> bPair;
	bPair = mm_WallEndRebarLine.insert(make_pair(pWallEndRebarLine->m_ID, pWallEndRebarLine));
	pWallEndRebarLine->SetBuilding(this);
	return bPair.second;
}

long MSRebarBuilding::Remove(MSWallEndRebarLine* pWallEndRebarLine)
{
	long nID = pWallEndRebarLine->m_ID;
	map<int, MSWallEndRebarLine*>::iterator it = mm_WallEndRebarLine.find(nID);
	if (it == mm_WallEndRebarLine.end())	return 0;
	mm_WallEndRebarLine.erase(it);
	return nID;
}

long MSRebarBuilding::Delete(MSWallEndRebarLine* pWallEndRebarLine)
{
	long nID = Remove(pWallEndRebarLine);
	delete pWallEndRebarLine;
	return nID;
}

void MSRebarBuilding::DeleteRibTypes()
{
	map<long, MSRibTypeM*>::iterator it;
	for (it = mm_RibM.begin(); it != mm_RibM.end(); ++it)
		delete it->second;
	mm_RibM.clear();
}

void MSRebarBuilding::Add(MSRibTypeM* pRibType)
{
	mm_RibM.insert(make_pair(pRibType->m_ID, pRibType));
}

MSRibTypeM* MSRebarBuilding::GetRibM(long ID)
{
	map<long, MSRibTypeM*>::iterator it = mm_RibM.find(ID);
	if (it == mm_RibM.end())
		return NULL;

	return it->second;
}

void MSRebarBuilding::Remove(MSRibTypeM* pRibType)
{
	long nID = pRibType->m_ID;
	map<long, MSRibTypeM*>::iterator itRibType = mm_RibM.find(nID);
	if (itRibType == mm_RibM.end())	return;
	mm_RibM.erase(itRibType);
}

void MSRebarBuilding::Delete(MSRibTypeM* pRibType)
{
	Remove(pRibType);
	delete pRibType;
}

void MSRebarBuilding::DeleteSpacerShapes()
{
	map<long, MSSpacerShape*>::iterator it;
	for (it = mm_Spacer.begin(); it != mm_Spacer.end(); ++it)
		delete it->second;
	mm_Spacer.clear();
}

void MSRebarBuilding::Add(MSSpacerShape* pSpacerShape)
{
	mm_Spacer.insert(make_pair(pSpacerShape->m_ID, pSpacerShape));
}

MSSpacerShape* MSRebarBuilding::GetSpacerShape(long ID)
{
	map<long, MSSpacerShape*>::iterator it = mm_Spacer.find(ID);
	if (it == mm_Spacer.end())
		return NULL;

	return it->second;
}

long MSRebarBuilding::GetSpacerShape(CString name)
{
	map<long, MSSpacerShape*>::iterator it;
	for (it = mm_Spacer.begin(); it != mm_Spacer.end(); ++it)
	{
		MSSpacerShape* spacerShape = it->second;
		if (spacerShape->GetName() == name)
			return it->first;
	}

	return 0;
}

void MSRebarBuilding::Remove(MSSpacerShape* pSpacerShape)
{
	long nID = pSpacerShape->m_ID;
	map<long, MSSpacerShape*>::iterator itSpacerShape = mm_Spacer.find(nID);
	if (itSpacerShape == mm_Spacer.end())	return;
	mm_Spacer.erase(itSpacerShape);
}

void MSRebarBuilding::Delete(MSSpacerShape* pSpacerShape)
{
	Remove(pSpacerShape);
	delete pSpacerShape;
}

MSStoryMaterial* MSRebarBuilding::CreateNewStoryMaterial()
{
	MSStoryMaterial* pStoryMaterial = new MSStoryMaterial();
	return pStoryMaterial;
}

void MSRebarBuilding::DeleteStoryMaterials()
{
	map<int, MSStoryMaterial*>::iterator it;
	for (it = mm_StoryMaterial.begin(); it != mm_StoryMaterial.end(); ++it)
		delete it->second;
	mm_StoryMaterial.clear();
}

MSStoryMaterial* MSRebarBuilding::GetStoryMaterialByID(long nID)
{
	map<int, MSStoryMaterial*>::iterator it = mm_StoryMaterial.find(nID);
	if (it == mm_StoryMaterial.end())
		return NULL;

	return it->second;
}

MSStoryMaterial* MSRebarBuilding::GetStoryMaterialByFloorNum(long floorNum)
{
	map<int, MSStoryMaterial*>::iterator it;
	for (it = mm_StoryMaterial.begin(); it != mm_StoryMaterial.end(); ++it)
	{
		MSStoryMaterial* pStoryMaterial = it->second;
		if (pStoryMaterial->GetFloorNum() == floorNum)
			return pStoryMaterial;
	}
	return NULL;
}

vector<MSStoryMaterial*> MSRebarBuilding::GetStoryMaterialsOrderbyFloorNum()
{
	vector<MSStoryMaterial*> SMArr;

	map<long, MSFloor*>::iterator itFloor;
	for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->second;

		MSStoryMaterial* pSM = GetStoryMaterialByFloorNum(pFloor->GetFloorNum());
		if (pSM == NULL)	continue;
		SMArr.push_back(pSM);
	}

	return SMArr;
}

void MSRebarBuilding::Add(MSStoryMaterial* pStoryMaterial)
{
	mm_StoryMaterial.insert(make_pair(pStoryMaterial->m_ID, pStoryMaterial));
}
void MSRebarBuilding::Remove(MSStoryMaterial* pStoryMaterial)
{
	long nID = pStoryMaterial->m_ID;
	map<int, MSStoryMaterial*>::iterator it = mm_StoryMaterial.find(nID);
	if (it == mm_StoryMaterial.end())	return;
	mm_StoryMaterial.erase(it);
}

void MSRebarBuilding::Delete(MSStoryMaterial* pStoryMaterial)
{
	Remove(pStoryMaterial);
	delete pStoryMaterial;
}

long MSRebarBuilding::GetFloorNumForUserbyStoryMaterial(long nFloorNum, MSMaterial::Type_MSMaterial matType, bool isVerMember, bool bFindLowFloor)
{
	// FloorNum을 입력 받으면 Material 정보가 같은 낮은층 Num을 리턴함
	vector<MSStoryMaterial*> SMArr = GetStoryMaterialsOrderbyFloorNum();

	if (isVerMember == false)
	{
		nFloorNum--;
		if (nFloorNum == 0)
			nFloorNum--;
	}

	MSStoryMaterial* pCurSM = GetStoryMaterialByFloorNum(nFloorNum);
	MSMaterial* pCurMaterial = pCurSM->GetMaterial(matType, isVerMember);

	MSStoryMaterial* pRSM = pCurSM;
	MSStoryMaterial* pPreSM = pCurSM;

	if (bFindLowFloor)
	{
		for (long i = SMArr.size() - 1; i >= 0; i--)
		{
			MSStoryMaterial* pSM = SMArr[i];
			if (pSM == NULL)		continue;
			if (pSM->GetFloorNum() > nFloorNum)
				continue;

			MSMaterial* pMaterial = pSM->GetMaterial(matType, isVerMember);

			if (pCurMaterial->m_ID != pMaterial->m_ID)
			{
				pRSM = pPreSM;
				break;
			}

			if (i == 0)
				pRSM = pSM;
			pPreSM = pSM;
		}
	}
	else
	{
		for (long i = 0; i < SMArr.size(); i++)
		{
			MSStoryMaterial* pSM = SMArr[i];
			if (pSM == NULL)		continue;
			if (pSM->GetFloorNum() < nFloorNum)
				continue;

			MSMaterial* pMaterial = pSM->GetMaterial(matType, isVerMember);

			if (pCurMaterial->m_ID != pMaterial->m_ID)
			{
				pRSM = pPreSM;
				break;
			}

			if (i == SMArr.size() - 1)
				pRSM = pSM;

			pPreSM = pSM;
		}
	}

	long nRvalFloorNum = pRSM->GetFloorNum();
	if (isVerMember == false)
	{
		nRvalFloorNum++;
		if (nRvalFloorNum == 0)
			nRvalFloorNum++;
	}

	return nRvalFloorNum;
}


void MSRebarBuilding::Serialize(CArchive &ar)
{
	MSBaseBuilding::SetActiveBuilding(this);
	if (ar.IsStoring())
	{
		CString startStr;
		startStr.Format(L"MSRebarBuilding %s was serialized start => Model Version : %d", this->m_Name, MSVersionInfo::GetLatestVersion());
		LoggerWrapper::Log(startStr);

		ar << MSVersionInfo::GetLatestVersion();
		ar << m_nUpdateModelVersion;

		MSBaseBuilding::Serialize(ar);

		ar << m_dGroundLevel;
		ar << m_DefaultReinforceBarSettingID;
		ar << m_Worker;
		ar << m_PlainConcreteReinforceBarSettingID;
		m_SiteRect.Serialize(ar);

		// add code - by gracias(2014, 01, 21)
		ar << (int)mm_ReinforceBar.size();
		map<long, MSReinforceBar*>::iterator itReinforceBar;
		for (itReinforceBar = mm_ReinforceBar.begin(); itReinforceBar != mm_ReinforceBar.end(); itReinforceBar++)
		{
			MSReinforceBar* pReinforceBar = itReinforceBar->second;
			pReinforceBar->Serialize(ar);
		}

		ar << (int)mm_ReinforceBarSetting.size();
		map<long, MSReinforceBarSetting*>::iterator itReinforceBarSetting;
		for (itReinforceBarSetting = mm_ReinforceBarSetting.begin(); itReinforceBarSetting != mm_ReinforceBarSetting.end(); itReinforceBarSetting++)
		{
			MSReinforceBarSetting* pReinforceBarSetting = itReinforceBarSetting->second;
			pReinforceBarSetting->Serialize(ar);
		}

		ar << (int)mm_Material.size();
		map<long, MSMaterial*>::iterator itMaterial;
		for (itMaterial = mm_Material.begin(); itMaterial != mm_Material.end(); itMaterial++)
		{
			MSMaterial* pMaterial = itMaterial->second;
			int Type = pMaterial->GetType();
			ar << Type;
			pMaterial->Serialize(ar);
		}

		int customCount = mm_CustomSection.size();
		ar << customCount;
		for (auto itCustom = mm_CustomSection.begin(); itCustom != mm_CustomSection.end(); itCustom++)
		{
			MSCustomSection* pCustomSection = itCustom->second;
			int Type = (int)pCustomSection->GetSectionType();
			ar << Type;

			pCustomSection->Serialize(ar);
		}

		ar << (int)mm_CompoM.size();
		map<long, MSCompoM*>::iterator itCompoM;
		for (itCompoM = mm_CompoM.begin(); itCompoM != mm_CompoM.end(); itCompoM++)
		{
			MSCompoM* pCompoM = itCompoM->second;
			int Type = pCompoM->GetType();
			ar << Type;
			pCompoM->Serialize(ar);
		}

		ar << (int)mm_Floor.size();
		map<long, MSFloor*>::iterator itFloor;
		for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
		{
			MSFloor* pFloor = itFloor->second;
			// 자료형 주의 (int, long)
			long nFloorNum = pFloor->GetFloorNum();
			ar << nFloorNum;
			ar << pFloor->GetHeight();
		}
		for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
		{
			MSFloor* pFloor = itFloor->second;
			pFloor->Serialize(ar);
		}

		ar << (int)mm_GridLine.size();
		map<long, MSGridLine*>::iterator itGridLine;
		for (itGridLine = mm_GridLine.begin(); itGridLine != mm_GridLine.end(); itGridLine++)
		{
			MSGridLine* pGridLine = itGridLine->second;
			pGridLine->Serialize(ar);
		}

		ar << (int)mm_Assembly.size();
		map<long, MSAssembly*>::iterator itAssem;
		for (itAssem = mm_Assembly.begin(); itAssem != mm_Assembly.end(); itAssem++)
		{
			MSAssembly* pAssem = itAssem->second;
			pAssem->Serialize(ar);
		}

		// add code - by gracias(2014, 01, 07)
		ar << (int)mm_MarkM.size();
		map<long, MSMarkM*>::iterator itMarker;
		for (itMarker = mm_MarkM.begin(); itMarker != mm_MarkM.end(); itMarker++)
		{
			MSMarkM* pMarker = itMarker->second;
			pMarker->Serialize(ar);
		}
		ar << bCalculationPrepared;

		m_rebarOptionManager.Serialize(ar);

		// add code - by gracias(2014, 03, 19)
		ar << (int)mm_MarkG.size();
		map<long, MSMarkG*>::iterator itMarkerG;
		for (itMarkerG = mm_MarkG.begin(); itMarkerG != mm_MarkG.end(); itMarkerG++)
		{
			MSMarkG* pMarker = itMarkerG->second;
			pMarker->Serialize(ar);
		}

		ar << (int)mm_Block.size();
		map<long, MSBlock*>::iterator itBlock;
		for (itBlock = mm_Block.begin(); itBlock != mm_Block.end(); itBlock++)
		{
			MSBlock* pBlock = itBlock->second;
			pBlock->Serialize(ar);
		}

		m_QuantityOption.Serialize(ar);

		int RibCount = mm_RibM.size();
		ar << RibCount;
		map<long, MSRibTypeM*>::iterator itRibM;
		for (itRibM = mm_RibM.begin(); itRibM != mm_RibM.end(); itRibM++)
		{
			MSRibTypeM* pRibM = itRibM->second;
			int RibType = (int)pRibM->GetRibType();
			ar << RibType;
			pRibM->Serialize(ar);
		}

		int SpacerShape = mm_Spacer.size();
		ar << SpacerShape;
		map<long, MSSpacerShape*>::iterator itSpacerShape;
		for (itSpacerShape = mm_Spacer.begin(); itSpacerShape != mm_Spacer.end(); itSpacerShape++)
		{
			MSSpacerShape* pSpacer = itSpacerShape->second;
			pSpacer->Serialize(ar);
		}

		m_rebarQuantityInfo.Serialize(ar);
		m_pSGradeManager->Serialize(ar);

		// add code - by gracias(2015, 07, 14)
		int StoryMaterialCount = mm_StoryMaterial.size();
		ar << StoryMaterialCount;
		map<int, MSStoryMaterial*>::iterator itStoryMaterial;
		for (itStoryMaterial = mm_StoryMaterial.begin(); itStoryMaterial != mm_StoryMaterial.end(); itStoryMaterial++)
		{
			MSStoryMaterial* pStoryMaterial = itStoryMaterial->second;
			pStoryMaterial->Serialize(ar);
		}

		int WallEndRebarCount = mm_WallEndRebarLine.size();
		ar << WallEndRebarCount;
		map<int, MSWallEndRebarLine*>::iterator itWallEndRebarLine;
		for (itWallEndRebarLine = mm_WallEndRebarLine.begin(); itWallEndRebarLine != mm_WallEndRebarLine.end(); itWallEndRebarLine++)
		{
			MSWallEndRebarLine* pWallEndRebarLine = itWallEndRebarLine->second;
			pWallEndRebarLine->Serialize(ar);
		}

		mp_FormManager->Serialize(ar);

		CString str;
		str.Format(L"MSBuilding %s was serialized successfully", this->m_Name);
		LoggerWrapper::Log(str);

		LoggingCompoGsTypeGeo();

		ar << m_nRebarTemplateID;

		mp_UCS->Serialize(ar);

		// 추가된 부분
		ar << m_nFormWorkTemplateID;

		int NoteCount = mm_Note.size();
		ar << NoteCount;

		int sNoteID = 0;
		CString sNote = _T("");

		map<int, CString>::iterator itNote;
		for (itNote = mm_Note.begin(); itNote != mm_Note.end(); itNote++)
		{
			sNoteID = itNote->first;
			sNote = itNote->second;

			ar << sNoteID;
			ar << sNote;
		}

		ar << m_nLicenseVersion;

		m_pAreaTemplateManager->Serialize(ar);

		m_pBuildingInfoManager->Serialize(ar);
		ar << m_nBuildingInfoID;

		ar << facilitySeparationType;
		ar << m_nNayukTemplateID;

		ar << (int)mm_DefaultCompoM.size();
		for (auto Iter = mm_DefaultCompoM.begin(); Iter != mm_DefaultCompoM.end(); Iter++)
		{
			MSCompoM *pCompoM = Iter->second;
			ar << Iter->first;
			pCompoM->Serialize(ar);
		}
		ar << m_bTopDownPlace;
		ar << m_nTopDownStartFloor;

		CString endStr;
		endStr.Format(L"MSRebarBuilding %s was serialized successfully", this->m_Name);
		LoggerWrapper::Log(endStr);
	}
	else
	{
		int version;
		ar >> version;
		MSVersionInfo::PushCurrentVersion(version);

		CString startStr;
		startStr.Format(L"MSRebarBuilding %s was deserialized start => Model Version : %d", this->m_Name, version);
		LoggerWrapper::Log(startStr);

		if (MSVersionInfo::GetCurrentVersion() >= 20170118)
			ar >> m_nUpdateModelVersion;
		else
			m_nUpdateModelVersion = 0;

		MSBaseBuilding::Serialize(ar);

		ar >> m_dGroundLevel;
		ar >> m_DefaultReinforceBarSettingID;
		ar >> m_Worker;
		ar >> m_PlainConcreteReinforceBarSettingID;
		m_SiteRect.Serialize(ar);

		// 		m_SiteRect.m_Origin->SetXY(0, 0);
		// 		m_SiteRect.m_Corner->SetXY(0, 0);

		int NumObject;

		// add code - by gracias(2014, 01, 21)
		ar >> NumObject;
		for (int iReinforceBar = 0; iReinforceBar < NumObject; iReinforceBar++)
		{
			MSReinforceBar* pReinforceBar = new MSReinforceBar();
			pReinforceBar->Serialize(ar);
			mm_ReinforceBar.insert(make_pair(pReinforceBar->m_ID, pReinforceBar));
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() < 20200112)
		{
			MSReinforceBar* pSD600SReinforcebar = MSReinforceBar::CreateSD600SReinforceBar(false);
			mm_ReinforceBar.insert(make_pair(pSD600SReinforcebar->m_ID, pSD600SReinforcebar));
		}

		if (version < 20150323)
			CreateNewReinforceBars();

		ar >> NumObject;
		for (int iReinforceBarSetting = 0; iReinforceBarSetting < NumObject; iReinforceBarSetting++)
		{
			MSReinforceBarSetting* pReinforceBarSetting = new MSReinforceBarSetting();
			pReinforceBarSetting->SetBuilding(this);
			pReinforceBarSetting->Serialize(ar);
			mm_ReinforceBarSetting.insert(make_pair(pReinforceBarSetting->m_ID, pReinforceBarSetting));
		}
		if (MSVersionInfo::GetCurrentVersion() < 20171210 && MSDeformedBar::GetRebarCode() == MSDeformedBar::REBAR_MATERIAL_GB)
		{
			UpdateReinforceBarGB_BH3374();
		}

		ar >> NumObject;
		for (int iMaterial = 0; iMaterial < NumObject; iMaterial++)
		{
			int nType = 0;
			ar >> nType;
			MSMaterial* pMaterial = CreateNewMaterial(nType);
			if (pMaterial == NULL)
			{
				ASSERT(0);
				continue;
			}
			pMaterial->SetBuilding(this);
			pMaterial->Serialize(ar);
			mm_Material.insert(make_pair(pMaterial->m_ID, pMaterial));
		}

		map<long, MSMaterial*>::iterator itMaterial;
		for (itMaterial = mm_Material.begin(); itMaterial != mm_Material.end(); itMaterial++)
		{
			MSMaterial* pMaterial = itMaterial->second;
			int Type = pMaterial->GetType();
			if (Type == MSMaterial::msSRC)
				((MSSrcMaterial*)pMaterial)->SetBuildingMaterialBySerializeID();
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20170412)
		{
			ar >> NumObject;
			for (int i = 0; i < NumObject; i++)
			{
				int type = 0;
				ar >> type;
				MSCustomSection* pCustomSection = CreateNewCustomSection((MSCustomSection::eSectionType)type);
				pCustomSection->Serialize(ar);

				Add(pCustomSection);
			}
		}

		ar >> NumObject;
		for (int iCompoM = 0; iCompoM < NumObject; iCompoM++)
		{
			int nType = 0;
			ar >> nType;
			MSCompoM* pCompoM = CreateNewCompoM(nType);
			if (pCompoM == NULL)
			{
				ASSERT(0);
				continue;
			}
			pCompoM->SetOwner(this);
			pCompoM->Serialize(ar);
			mm_CompoM.insert(make_pair(pCompoM->m_ID, pCompoM));
		}

		if (MSVersionInfo::GetCurrentVersion() < 20150403)
		{
			CreateDefaultOpeningReinforceM();
			map<long, MSCompoM*>::iterator it;

			for (it = mm_CompoM.begin(); it != mm_CompoM.end(); it++)
			{
				MSCompoM* pCompoM = it->second;
				MSCompoM::Type_MSCompoM eType = pCompoM->GetType();
				if (eType == MSCompoM::msDoorM || eType == MSCompoM::msWindowM || eType == MSCompoM::msOpeningExtraM)
				{
					MSOpeningM* pOpeningM = (MSOpeningM*)pCompoM;
					pOpeningM->SetOpeningReinforceMID(GetDefaultOpeningReinforceM()->m_ID);
				}
			}
		}

		ar >> NumObject;
		for (int iFloor = 0; iFloor < NumObject; iFloor++)
		{
			MSFloor* pFloor = new MSFloor;
			pFloor->SetOwner(this);
			pFloor->SetSite();
			long nFloorNum = 0;
			ar >> nFloorNum;
			pFloor->SetFloorNum(nFloorNum);
			double dHeight = 0;
			ar >> dHeight;

			//MSFloor::Serialize()에서 Height 처리하지만 ReSetCompoGZLevel 계산하기 위해 사용
			pFloor->SetHeight(dHeight, false);
			mm_Floor.insert(make_pair(pFloor->GetFloorNum(), pFloor));
		}

		MSFloor* bottomFloor = nullptr;
		map<long, MSFloor*>::iterator itFloor;
		for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
		{
			MSFloor* pFloor = itFloor->second;
			pFloor->Serialize(ar);

			if (bottomFloor == nullptr)
				bottomFloor = pFloor;
			else
			{
				if (bottomFloor->GetFloorNum() > pFloor->GetFloorNum())
					bottomFloor = pFloor;
			}
		}

		// 모든 Floor Serialize 끝난후 ZLevel을 계산해준다
		for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
		{
			MSFloor* pFloor = itFloor->second;
			pFloor->ReSetCompoGZLevel();
		}

		//기본 시공존에서 수직, 수평 시공존으로 나뉘어지면서 
		//
		CheckFloorQT();

		for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
		{
			MSFloor* pFloor = itFloor->second;
			pFloor->RecoverPointerAfterSerialize();
		}

		CalcUpdateType();

		if (bottomFloor != nullptr && bottomFloor->GetName() != _T("BOTTOM"))
			bottomFloor->SetName(_T("BOTTOM"));

		AddLevelZoneAfterSerialize();

		ar >> NumObject;
		for (int iGridLine = 0; iGridLine < NumObject; iGridLine++)
		{
			MSGridLine* pGridLine = new MSGridLine;
			pGridLine->Serialize(ar);
			pGridLine->m_pOwner = this;
			mm_GridLine.insert(make_pair(pGridLine->m_ID, pGridLine));
		}

		ar >> NumObject;
		for (int iAssem = 0; iAssem < NumObject; iAssem++)
		{
			MSAssembly* pAssem = new MSAssembly;
			pAssem->Serialize(ar);
			mm_Assembly.insert(make_pair(pAssem->m_ID, pAssem));
		}

		ar >> NumObject;
		for (int iMarkerM = 0; iMarkerM < NumObject; iMarkerM++)
		{
			MSMarkM* pMarkM = new MSMarkM;
			pMarkM->SetBuilding(this);
			pMarkM->Serialize(ar);
			mm_MarkM.insert(make_pair(pMarkM->m_ID, pMarkM));
		}

		ar >> bCalculationPrepared;

		m_rebarOptionManager.Serialize(ar);

		if (MSVersionInfo::GetCurrentVersion() >= 20140319)
		{
			ar >> NumObject;
			for (int iMarkG = 0; iMarkG < NumObject; iMarkG++)
			{
				MSMarkG* pMarkG = new MSMarkG;
				pMarkG->mp_Bldg = this;
				pMarkG->Serialize(ar);
				mm_MarkG.insert(make_pair(pMarkG->m_ID, pMarkG));
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20140604)
		{
			ar >> NumObject;
			for (int iBlock = 0; iBlock < NumObject; iBlock++)
			{
				MSBlock* pBlock = new MSBlock;
				pBlock->SetOwner(this);
				pBlock->Serialize(ar);
				mm_Block.insert(make_pair(pBlock->m_ID, pBlock));
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20140605)
		{
			m_QuantityOption.SetBuilding(this);
			m_QuantityOption.Serialize(ar);
		}

		//MSVersionInfo::PopCurrentVersion(); // mylee TEMP - Project 에서 Import 시에 Version 정보 사용하기 위함

		// MSConstructionZone, MSGridLine, MSMarkG를 MSElement에서 파생시켜서 m_ID를 하나로 관리한다.
		// 이전 버전과의 호환성을 위해, Serialize가 끝난 후 위 세개의 객체에 대해서 GetID()를 다시 호출하여 새로운 m_ID를 부여한다.
		if (MSVersionInfo::GetCurrentVersion() < 20140729)
			Patch_Version20140729();

		if (MSVersionInfo::GetCurrentVersion() >= 20140901)
		{
			int RibCount = 0;
			int RibType = 0;
			ar >> RibCount;
			for (int iRib = 0; iRib < RibCount; iRib++)
			{
				ar >> RibType;
				MSRibTypeM* pRibM = CreateNewRibM(RibType);
				pRibM->Serialize(ar);
				mm_RibM.insert(make_pair(pRibM->m_ID, pRibM));
			}
		}

		// 		if(MSVersionInfo::GetCurrentVersion() >= 20140918)
		// 		{
		// 			for(itFloor=mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
		// 			{
		// 				MSFloor* pFloor = itFloor->second;
		// 				CheckCompoM(pFloor);
		// 			}
		// 		}

		if (MSVersionInfo::GetCurrentVersion() >= 20141103)
		{
			int SpacerCount = 0;
			ar >> SpacerCount;
			for (int iSpacer = 0; iSpacer < SpacerCount; iSpacer++)
			{
				MSSpacerShape* pSpacer = CreateNewSpacerShape();
				pSpacer->Serialize(ar);
				mm_Spacer.insert(make_pair(pSpacer->m_ID, pSpacer));
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20150312)
		{
			m_rebarQuantityInfo.Serialize(ar);
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20150323)
		{
			m_pSGradeManager->Serialize(ar);
		}
		else
		{
			// Global옵션도 추가시켜줘야 한다.
//			MakeDefaultReinforceBars();	// 추가된 Reinforcebar를 생성한다(SD350, SD300W, SD400S, SD500W, SD500S, SD700)
			m_pSGradeManager->MakeDefaultDataByRebarCode(MSDeformedBar::GetRebarCode());
		}

		if (MSVersionInfo::GetCurrentVersion() < 20150402)
		{
			long defMaterialID = GetDefMaterial()->m_ID;
			map<long, MSFloor*>::iterator itFloor;
			for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
			{
				MSFloor* pFloor = itFloor->second;
				map<long, MSSlabRebar*>::iterator itRebar;
				for (itRebar = pFloor->mm_Rebar.begin(); itRebar != pFloor->mm_Rebar.end(); itRebar++)
				{
					MSSlabRebar* pRebar = itRebar->second;
					MSMaterial* pMaterial = GetMaterial(pRebar->GetMaterialID());
					if (pMaterial == NULL)
						pRebar->SetMaterialID(defMaterialID);
				}
			}
		}

		SetReinforceBarManager();

		if (MSVersionInfo::GetCurrentVersion() < 20150408)
		{
			StatusReporterWrapper::UpdateStatus_MsgOnly(_T("Converting Opening Information..."));
			for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
			{
				MSFloor* pFloor = itFloor->second;
				pFloor->ConverterOpeningG();
			}
			StatusReporterWrapper::UpdateStatus_MsgOnly(_T("Converting Finished."));
		}

		CString str;
		str.Format(L"MSBuilding %s was deserialized successfully", this->m_Name);
		LoggerWrapper::Log(str);

		//Serialize()끝나고 ReadFileBinary()함수에서 처리 BuildingUpdate() 함수호출

		if (MSVersionInfo::GetCurrentVersion() < 20150502)
		{
			map<long, MSCompoM*>::iterator it;
			map<long, MSMaterial*>::iterator Matit;
			for (it = mm_CompoM.begin(); it != mm_CompoM.end(); it++)
			{
				MSCompoM* pCompoM = it->second;

				if (pCompoM->GetMaterial()->GetType() == MSMaterial::msConcrete) continue;

				for (Matit = mm_Material.begin(); Matit != mm_Material.end(); Matit++)
				{
					MSMaterial* pMaterial = Matit->second;
					if (pMaterial->GetType() == MSMaterial::msConcrete && pCompoM->GetMaterial()->GetName() == pMaterial->GetName())
						pCompoM->SetMaterial(pMaterial);
				}
			}
		}

		if (MSVersionInfo::GetCurrentVersion() < 20150717)
		{
			Patch_Material_Version20150701();
			MakeDefaultStoryMaterials();
		}
		else if (20150717 <= MSVersionInfo::GetCurrentVersion() && MSVersionInfo::GetCurrentVersion() < 20150721)
		{
			int StoryMaterialCount = 0;
			ar >> StoryMaterialCount;
			for (int iStoryMaterial = 0; iStoryMaterial < StoryMaterialCount; iStoryMaterial++)
			{
				MSStoryMaterial* pStoryMaterial = CreateNewStoryMaterial();
				pStoryMaterial->SetBuilding(this);
				pStoryMaterial->Serialize(ar);
				Add(pStoryMaterial);
			}
			Patch_Material_Version20150701();
			MakeDefaultStoryMaterials();
		}
		else
		{
			int StoryMaterialCount = 0;
			ar >> StoryMaterialCount;
			for (int iStoryMaterial = 0; iStoryMaterial < StoryMaterialCount; iStoryMaterial++)
			{
				MSStoryMaterial* pStoryMaterial = CreateNewStoryMaterial();
				pStoryMaterial->SetBuilding(this);
				pStoryMaterial->Serialize(ar);
				Add(pStoryMaterial);
			}
		}

		for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
		{
			MSFloor* pFloor = itFloor->second;
			pFloor->CheckRebarLine();
		}

		//CheckGeometryModel(false, true);
		if (MSVersionInfo::GetCurrentVersion() >= 20151211)
		{
			int WallEndRebarLineCount = 0;
			ar >> WallEndRebarLineCount;
			for (int iRebarLine = 0; iRebarLine < WallEndRebarLineCount; iRebarLine++)
			{
				MSWallEndRebarLine* pWallEndRebarLine = CreateNewWallEndRebarLine();
				pWallEndRebarLine->SetBuilding(this);
				pWallEndRebarLine->Serialize(ar);
				Add(pWallEndRebarLine);
			}
		}

		if (MSVersionInfo::GetCurrentVersion() < 20160510)
		{
			RemoveSpacialCharacters_InCompoM();
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20160517 && MSVersionInfo::GetCurrentVersion() < 20160620)
			FixBoundaryCompoGs();
		CheckInValidElements();

		if (MSVersionInfo::GetCurrentVersion() >= 20160905)
		{
			mp_FormManager->DeleteFormWorkArr();
			mp_FormManager->Serialize(ar);
		}
		else
		{
			//이전버전은 한국어로 무조건 초기화
			mp_FormManager->DeleteFormWorkArr();
			mp_FormManager->CreateKorDefaultData();
		}

		// 		RecoverCustomProfile();
		// 
		// 		for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
		// 		{
		// 			MSFloor* pFloor = itFloor->second;
		// 			pFloor->RecoverEachCompoMCustomProfile();
		// 		}

		if (!IsCorrectedStoryMaterials())
			RepairStoryMaterials();

		//if (MSVersionInfo::GetCurrentVersion() <= 20170106)
		{
			AllCompoGUsedFormWorkExistCheck();
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20170715)
		{
			ar >> m_nRebarTemplateID;
		}

		if (AppGlobalOptionWrapper::Instance()->GetLoadBldgMode() == AppGlobalOptionWrapper::Rebar
			&& MSVersionInfo::GetCurrentVersion() >= 20180131 && MSVersionInfo::GetCurrentVersion() < 20190731)
		{
			double overGrossFloorArea = 0;
			double underGrossFloorArea = 0;
			CString buildingNameForExcel = _T("");

			ar >> overGrossFloorArea;
			ar >> underGrossFloorArea;
			ar >> buildingNameForExcel;
		}

		if (((AppGlobalOptionWrapper::Instance()->GetLoadBldgMode() == AppGlobalOptionWrapper::Rebar
			|| AppGlobalOptionWrapper::Instance()->GetLoadBldgMode() == AppGlobalOptionWrapper::CE)
			&& MSVersionInfo::GetCurrentVersion() >= 20180620)
			|| ((AppGlobalOptionWrapper::Instance()->GetLoadBldgMode() == AppGlobalOptionWrapper::Formwork
				|| AppGlobalOptionWrapper::Instance()->GetLoadBldgMode() == AppGlobalOptionWrapper::Finish)
				&& MSVersionInfo::GetCurrentVersion() >= 20200507)
			)
		{
			mp_UCS->Serialize(ar);
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20180622)
		{
			ar >> m_nFormWorkTemplateID;
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() < 20180912)
		{
			TotalColumnMNameDuplicateCheckAndNameModify();
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20181023)
		{
			NoteClear();

			int nNoteCount = 0;
			ar >> nNoteCount;

			int sNoteID = 0;
			CString sNote = _T("");
			for (int index = 0; index < nNoteCount; index++)
			{
				ar >> sNoteID;
				ar >> sNote;

				mm_Note.insert(make_pair(sNoteID, sNote));
			}
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20190122)
		{
			ar >> m_nLicenseVersion;
			if (IsBHQorBHCEMode() && MSVersionInfo::GetCurrentVersion() < 20240425)
			{
				if (m_nLicenseVersion == -1)
					m_nLicenseVersion = 0;
				else if (m_nLicenseVersion = 0)
					m_nLicenseVersion = 1;
				else if (m_nLicenseVersion = 1)
					m_nLicenseVersion = 2;
				else if (m_nLicenseVersion = 2)
					m_nLicenseVersion = 4;
				else if (m_nLicenseVersion = 3)
					m_nLicenseVersion = 8;
				else if (m_nLicenseVersion = 4)
					m_nLicenseVersion = 16;
				else if (m_nLicenseVersion = 5)
					m_nLicenseVersion = 32;
				else if (m_nLicenseVersion = 6)
					m_nLicenseVersion = 64;
			}
		}
		else
			m_nLicenseVersion = 2;

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20190417)
			m_pAreaTemplateManager->Serialize(ar);

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20190731)
		{
			m_pBuildingInfoManager->Serialize(ar);
			ar >> m_nBuildingInfoID;
		}
		else
		{
			CString buildingNameForExcel = this->m_Name;
			buildingNameForExcel.Replace(_T(" "), _T(""));
			m_pBuildingInfoManager->CreateDefaultBuildingInfoAndAdd(this->m_Name, buildingNameForExcel);
			m_nBuildingInfoID = -1;
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20190801)
		{
			ar >> facilitySeparationType;
			ar >> m_nNayukTemplateID;
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() < 20200727)
		{
			makeVerticalConstructionZones();
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20220321)
		{
			long nType = 0;
			ar >> NumObject;
			for (int iCompoM = 0; iCompoM < NumObject; iCompoM++)
			{
				ar >> nType;
				MSCompoM* pCompoM = CreateNewCompoM(nType);
				pCompoM->Serialize(ar);
				mm_DefaultCompoM.insert(make_pair(nType, pCompoM));
			}
		}
		if(IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231010)
		{
			ar >> m_bTopDownPlace;
			ar >> m_nTopDownStartFloor;
		}

		CString endStr;
		endStr.Format(L"MSRebarBuilding %s was deserialized successfully", this->m_Name);
		LoggerWrapper::Log(endStr);

		// MSBaseBuilding::Serialize() 에서 저장 처리
		// 이 코드는 TypeUIDFactory 복구 하는 코드이다,
		/*
		if ((IsBHQorBHCEMode() && (MSVersionInfo::GetCurrentVersion() < 20240425 || MSVersionInfo::IsOldTotalVersion() == false))
			|| (IsBHForBHAMode() && MSVersionInfo::GetCurrentVersion() < 20200703)
			)
			*/
		if ((IsBHQorBHCEMode() && MSVersionInfo::GetCurrentVersion() < 20240425)
			|| (IsBHForBHAMode() && MSVersionInfo::GetCurrentVersion() < 20200703))
		{
			RecoverTypeUIDFactoryByMSObjectLastID();
		}

		if (((AppGlobalOptionWrapper::Instance()->GetLoadBldgMode() == AppGlobalOptionWrapper::Formwork
				|| AppGlobalOptionWrapper::Instance()->GetLoadBldgMode() == AppGlobalOptionWrapper::Finish)
				&& MSVersionInfo::GetCurrentVersion() < 20210706)
			)
		{
			ConvertFloor_UType();
			// 			m_eUpdateType = BuildingUpdateType_ALL;
		}

		m_pTypeUIDFactory->Restore(UIDType::msElement, MSElement::ms_LastID);
		m_pTypeUIDFactory->Restore(UIDType::msCompoM, MSCompoM::ms_LastID);
		m_pTypeUIDFactory->Restore(UIDType::msMaterial, MSMaterial::ms_LastID);
	}

	LoggingCompoGsTypeGeo();
	LogningConnections();
}
void MSRebarBuilding::ConvertFloor_UType()
{
	for (auto it = mm_Floor.begin(); it != mm_Floor.end(); it++)
	{
		it->second->Convert_UType_Floor();
	}
}
void MSRebarBuilding::RecoverTypeUIDFactoryByMSObjectLastID()
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

	// 	pUIDFactory = m_pTypeUIDFactory->GetUIDFactory(UIDType::ropObject);
	// 	pUIDFactory->SetCurrentID(ROPObject::ms_LastID);

	pUIDFactory = m_pTypeUIDFactory->GetUIDFactory(UIDType::msMaterial);
	pUIDFactory->SetCurrentID(MSMaterial::ms_LastID);

	pUIDFactory = m_pTypeUIDFactory->GetUIDFactory(UIDType::msReinforceBar);
	pUIDFactory->SetCurrentID(MSReinforceBar::ms_LastID);

	pUIDFactory = m_pTypeUIDFactory->GetUIDFactory(UIDType::msReinforceBarSetting);
	pUIDFactory->SetCurrentID(MSReinforceBarSetting::ms_LastID);

	pUIDFactory = m_pTypeUIDFactory->GetUIDFactory(UIDType::msMarkM);
	pUIDFactory->SetCurrentID(MSMarkM::ms_LastID);

	pUIDFactory = m_pTypeUIDFactory->GetUIDFactory(UIDType::msCustomBarUnit);
	pUIDFactory->SetCurrentID(MSCustomBarUnit::ms_LastID);

	pUIDFactory = m_pTypeUIDFactory->GetUIDFactory(UIDType::msCustomReBarLine);
	pUIDFactory->SetCurrentID(MSCustomReBarLine::ms_LastID);

	pUIDFactory = m_pTypeUIDFactory->GetUIDFactory(UIDType::msCustomSection);
	pUIDFactory->SetCurrentID(MSCustomSection::ms_LastID);

	pUIDFactory = m_pTypeUIDFactory->GetUIDFactory(UIDType::msRibTypeM);
	pUIDFactory->SetCurrentID(MSRibTypeM::ms_LastID);

	pUIDFactory = m_pTypeUIDFactory->GetUIDFactory(UIDType::msWallEndRebarLine);
	pUIDFactory->SetCurrentID(MSWallEndRebarLine::ms_LastID);

	pUIDFactory = m_pTypeUIDFactory->GetUIDFactory(UIDType::msWallEndRebarM);
	pUIDFactory->SetCurrentID(MSWallEndRebarM::ms_LastID);

	pUIDFactory = m_pTypeUIDFactory->GetUIDFactory(UIDType::msBarSet);
	pUIDFactory->SetCurrentID(MSBarSet::ms_LastID);

	pUIDFactory = m_pTypeUIDFactory->GetUIDFactory(UIDType::dtStirrup);
	pUIDFactory->SetCurrentID(DTStirrup::ms_LastID);

	pUIDFactory = m_pTypeUIDFactory->GetUIDFactory(UIDType::rebarTemplateManager);
	pUIDFactory->SetCurrentID(RebarTemplateManager::ms_LastID);
}


void MSRebarBuilding::UpdateReinforceBarGB_BH3374()
{
	DeleteReinforceBars();

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
	MSReinforceBar::ms_LastID = 0;
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
		pReinforcebar->GetID();
		Add(pReinforcebar);
	}

	int NumDia = MSDeformedBar::GetRebarNameList(diaNameList);
	for (int iDia = 0; iDia < NumDia; iDia++)
	{
		CString sDiaName = diaNameList[iDia];
		int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
		pReinforceBarSetting->mm_IndexToReinforceBar.insert(make_pair(DiaIndex, pDefaultReinforceBar));
	}
}

void MSRebarBuilding::RecoverCustomProfile()
{
	for (auto it = mm_CompoM.begin(); it != mm_CompoM.end(); it++)
	{
		MSCompoM* pCompoM = it->second;
		if (pCompoM->IsCustomM() == false)
			continue;

		MSCustomM* pCustomM = (MSCustomM*)pCompoM;
		MSCustomSection* pSection = GetCustomSection(pCustomM->GetSectionID());
		pCustomM->SetMSCustomSection(pSection);
	}
}

void MSRebarBuilding::AllCompoGUsedFormWorkExistCheck()
{
	CString sChangeFormWorkTypeName = mp_FormManager->GetNoneFormWorkTypeName();
	vector<CString> formWorkTypeNameListAll = mp_FormManager->GetFormWorkTypeNameListAll();

	vector<MSCompoG*> compoGs = GetAllCompoGs();
	for (int index = 0; index < compoGs.size(); index++)
	{
		MSCompoG* compoG = compoGs[index];

		if (compoG->GetType() == MSElement::msSlabG && compoG->GetCompoM()->GetType() == MSCompoM::msSlabM)
		{
			MSRcSlabM* rcSlabM = (MSRcSlabM*)compoG->GetRebarM();

			if (rcSlabM != NULL && rcSlabM->IsDeckSlab() == TRUE)
			{
				sChangeFormWorkTypeName = mp_FormManager->GetDeckFormWorkTypeName();
			}
		}

		if (compoG->ma_VolumeInfo.size() > 0)
		{
			for (int i = 0; i < compoG->ma_VolumeInfo.size(); i++)
			{
				CString formWorkName = compoG->ma_VolumeInfo[i].formWorkName;
				if (formWorkName == _T("")) continue;;

				vector<CString>::iterator it = std::find(formWorkTypeNameListAll.begin(), formWorkTypeNameListAll.end(), formWorkName);

				if (it == formWorkTypeNameListAll.end()) {
					compoG->ma_VolumeInfo[i].formWorkName = sChangeFormWorkTypeName;
					StatusReporterWrapper::OutputError(CFormatMsg(LANG2(SFORMWORKTYPECHANGE, TEXT), formWorkName, sChangeFormWorkTypeName));
				}
			}
		}

		if (compoG->ma_AreaInfo.size() > 0)
		{
			for (int i = 0; i < compoG->ma_AreaInfo.size(); i++)
			{
				CString formWorkName = compoG->ma_AreaInfo[i].formWorkName;
				if (formWorkName == _T("")) continue;;

				vector<CString>::iterator it = std::find(formWorkTypeNameListAll.begin(), formWorkTypeNameListAll.end(), formWorkName);

				if (it == formWorkTypeNameListAll.end()) {
					if (compoG->ma_AreaInfo[i].use_user_input == false)
					{
						compoG->ma_AreaInfo[i].formWorkName = sChangeFormWorkTypeName;
						StatusReporterWrapper::OutputError(CFormatMsg(LANG2(SFORMWORKTYPECHANGE, TEXT), formWorkName, sChangeFormWorkTypeName));
					}
					else
					{
						compoG->ma_AreaInfo[i].formWorkName = formWorkTypeNameListAll[0];
						StatusReporterWrapper::OutputError(CFormatMsg(LANG2(SFORMWORKTYPECHANGE, TEXT), formWorkName, formWorkTypeNameListAll[0]));
					}
				}
			}
		}

		if (compoG->ma_UserFormWork.size() > 0)
		{
			for (int i = 0; i < compoG->ma_UserFormWork.size(); i++)
			{
				CString formWorkName = compoG->ma_UserFormWork[i].formWorkName;
				if (formWorkName == _T("")) continue;;

				vector<CString>::iterator it = std::find(formWorkTypeNameListAll.begin(), formWorkTypeNameListAll.end(), formWorkName);

				if (it == formWorkTypeNameListAll.end()) {
					compoG->ma_UserFormWork[i].formWorkName = sChangeFormWorkTypeName;
					StatusReporterWrapper::OutputError(CFormatMsg(LANG2(SFORMWORKTYPECHANGE, TEXT), formWorkName, sChangeFormWorkTypeName));
				}
			}
		}
	}
}

void MSRebarBuilding::CalcUpdateType()
{
	if (m_eUpdateType != BuildingUpdateType_3DView)
		m_eUpdateType = BuildingUpdateType_NONE;

	if (m_nUpdateModelVersion < 20170303) //[BH-2161] MSRebarBuilding::m_nUpdateModelVersion 추가
		m_eUpdateType = BuildingUpdateType_ALL;

	if (m_nUpdateModelVersion < 20200911)
		m_eUpdateType = BuildingUpdateType_3DView;

	if (m_nUpdateModelVersion < 20241125)
		m_eUpdateType = BuildingUpdateType_3DView;
	//if ((MSVersionInfo::GetCurrentVersion() >= 20150209 && MSVersionInfo::GetCurrentVersion() < 20150513) ||
	//	MSVersionInfo::GetCurrentVersion() < 20161207 ||	// [BH-1620]MSConstructionZone 데이타 변경 mp_PolylineInfo 사용
	//	MSVersionInfo::GetCurrentVersion() < 20161223 ||	//[BH-1987] GM2DPolyline::SegmentstoMe 버그 수정으로 인한 모델 업데이트 추가
	//	MSVersionInfo::GetCurrentVersion() < 20170102)		//[BH-1906] 슬래브에 포함된 부재의 ID 저장
	//	m_eUpdateType = BuildingUpdateType_ALL;
	//// 이 경우는 3D 만 Regen
	//else if (MSVersionInfo::GetCurrentVersion() < 20150820 || MSVersionInfo::GetCurrentVersion() < 20151124 ||
	//	MSVersionInfo::GetCurrentVersion() < 20161229) // 3d original shape 추가
	//	m_eUpdateType = BuildingUpdateType_3DView;
}

bool MSRebarBuilding::IsCorrectedStoryMaterials()
{
	// 1. 크기가 다르다
	if (mm_StoryMaterial.size() != mm_Floor.size())	return false;

	// 2. StoryMaterial이 중복된 층을 갖고 있다.
	map<long, MSStoryMaterial*> floorNumToStoryMaterialMap;
	for (auto it = mm_StoryMaterial.begin(); it != mm_StoryMaterial.end(); it++)
	{
		long floorNum = it->second->GetFloorNum();
		auto itMap = floorNumToStoryMaterialMap.find(floorNum);
		if (itMap == floorNumToStoryMaterialMap.end())
			floorNumToStoryMaterialMap.insert(make_pair(floorNum, it->second));
		else
			return false;
	}

	// 3. FloorNum이 빠진 층을 갖고 있다.
	for (auto itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		long floorNum = itFloor->second->GetFloorNum();
		if (floorNumToStoryMaterialMap.find(floorNum) == floorNumToStoryMaterialMap.end())
			return false;
	}
	return true;
}

void MSRebarBuilding::RepairStoryMaterials()
{
	map<long, MSStoryMaterial*> floorNumToStoryMaterialMap;
	for (auto it = mm_StoryMaterial.begin(); it != mm_StoryMaterial.end(); it++)
	{
		long floorNum = it->second->GetFloorNum();
		auto itMap = floorNumToStoryMaterialMap.find(floorNum);
		if (itMap == floorNumToStoryMaterialMap.end())
			floorNumToStoryMaterialMap.insert(make_pair(floorNum, it->second));
		else
			delete it->second;
	}

	mm_StoryMaterial.clear();
	if (floorNumToStoryMaterialMap.size() == 0)
	{
		MakeDefaultStoryMaterials();
		return;
	}
	MSStoryMaterial* pDefaultStoryMaterial = floorNumToStoryMaterialMap.begin()->second;

	for (auto it = mm_Floor.begin(); it != mm_Floor.end(); it++)
	{
		long story = it->second->GetFloorNum();
		auto itMap = floorNumToStoryMaterialMap.find(story);
		if (itMap != floorNumToStoryMaterialMap.end())
			Add(itMap->second);
		else
		{
			long preStory = story - 1;
			if (preStory == 0)	preStory = -1;
			itMap = floorNumToStoryMaterialMap.find(preStory);
			MSStoryMaterial* pStoryMaterial = CreateNewStoryMaterial();
			pStoryMaterial->SetBuilding(this);
			if (itMap != floorNumToStoryMaterialMap.end())
				pStoryMaterial->CopyFromMe(itMap->second);
			else
				pStoryMaterial->CopyFromMe(pDefaultStoryMaterial);

			pStoryMaterial->SetFloorNum(story);
			pStoryMaterial->GetID();
			Add(pStoryMaterial);
		}
	}
}

void MSRebarBuilding::RemoveSpacialCharacters_InCompoM()
{
	map<long, MSCompoM*>::iterator it;
	for (it = mm_CompoM.begin(); it != mm_CompoM.end(); it++)
	{
		MSCompoM* pCompoM = it->second;
		if (pCompoM == NULL)		continue;

		if (MSCompoM::IsExistedSpacialCharacters(pCompoM->m_Name))
		{
			CString newName = pCompoM->m_Name;
			char replacer = ' ';
			newName = MSCompoM::ReplaceSpacialCharactersName(newName, replacer);
			pCompoM->m_Name.Format(_T("%s(%ld)"), newName, pCompoM->m_ID);
		}
	}
}

void MSRebarBuilding::MakeDefaultStoryMaterials()
{
	DeleteStoryMaterials();
	MSMaterial* pDefaultRcMaterial = GetDefMaterial(MSMaterial::msConcrete);
	MSMaterial* pDefaultSteelMaterial = GetDefMaterial(MSMaterial::msSteel);
	MSMaterial* pDefaultSrcMaterial = GetDefMaterial(MSMaterial::msSRC);
	if (pDefaultRcMaterial == NULL || pDefaultSteelMaterial == NULL || pDefaultSrcMaterial == NULL)
		return;

	int rcMaterialID = pDefaultRcMaterial->m_ID;
	int steelMaterialID = pDefaultSteelMaterial->m_ID;
	int srcMaterialID = pDefaultSrcMaterial->m_ID;
	int nFloor = mm_Floor.size();
	map<long, MSFloor*>::iterator itFloor;
	for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		int floorNum = itFloor->first;
		MSStoryMaterial* pStoryMaterial = CreateNewStoryMaterial();
		pStoryMaterial->SetBuilding(this);
		pStoryMaterial->SetFloorNum(floorNum);
		pStoryMaterial->SetRcHorizontalMaterialID(rcMaterialID);
		pStoryMaterial->SetRcVerticalMaterialID(rcMaterialID);
		pStoryMaterial->SetSteelHorizontalMaterialID(steelMaterialID);
		pStoryMaterial->SetSteelVerticalMaterialID(steelMaterialID);
		pStoryMaterial->SetSrcHorizontalMaterialID(srcMaterialID);
		pStoryMaterial->SetSrcVerticalMaterialID(srcMaterialID);
		pStoryMaterial->GetID();
		Add(pStoryMaterial);
	}
}

void MSRebarBuilding::CreateDefaultOpeningReinforceM()
{
	MSCompoM* pDefOpeningReinforceM = CreateNewCompoM(MSCompoM::msOpeningReinforceM);
	pDefOpeningReinforceM->GetID();
	pDefOpeningReinforceM->m_Name = _T("DEFAULTNAME");
	pDefOpeningReinforceM->SetMaterial(GetDefMaterial(MSMaterial::msConcrete));
	pDefOpeningReinforceM->SetInsertedDate(CTime::GetCurrentTime());
	Add(pDefOpeningReinforceM);
}

MSCompoM* MSRebarBuilding::GetDefaultOpeningReinforceM()
{
	map<long, MSCompoM*>::iterator it;
	for (it = mm_CompoM.begin(); it != mm_CompoM.end(); it++)
	{
		MSCompoM* pCompoM = it->second;

		if (pCompoM->GetType() == MSCompoM::msOpeningReinforceM)
			return pCompoM;
	}

	return NULL;
}

void MSRebarBuilding::SetReinforceBarManager()
{
	map<long, MSReinforceBar*>::iterator it;
	for (it = mm_ReinforceBar.begin(); it != mm_ReinforceBar.end(); it++)
	{
		MSReinforceBar* pReinforceBar = it->second;
		pReinforceBar->m_pSGradeManager = m_pSGradeManager;
	}
}

void MSRebarBuilding::CreateNewReinforceBars()
{
	DeleteReinforceBars();
	MSReinforceBar::ms_LastID = 0;

	CStringArray Names;
	Names.Add(STR_SD300);	Names.Add(STR_SD350);	Names.Add(STR_SD400);	Names.Add(STR_SD400S);	Names.Add(STR_SD400W);
	Names.Add(STR_SD500);	Names.Add(STR_SD500S);  Names.Add(STR_SD500W);  Names.Add(STR_SD600);	Names.Add(STR_SD600S);  Names.Add(STR_SD700);

	CArray<double, double> Strengths;
	Strengths.Add(300);		Strengths.Add(350);		Strengths.Add(400);		Strengths.Add(400);		Strengths.Add(400);
	Strengths.Add(500);		Strengths.Add(500);		Strengths.Add(500);		Strengths.Add(600);		Strengths.Add(600);	Strengths.Add(700);

	CStringArray PrintNames;
	PrintNames.Add(STR_BARTYPE_D);	PrintNames.Add(STR_BARTYPE_D);	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);	PrintNames.Add(STR_BARTYPE_HD);
	PrintNames.Add(STR_BARTYPE_SHD);  PrintNames.Add(STR_BARTYPE_SHD);  PrintNames.Add(STR_BARTYPE_SHD);  PrintNames.Add(STR_BARTYPE_UHD); PrintNames.Add(STR_BARTYPE_UHD);  PrintNames.Add(STR_BARTYPE_UHD);

	CArray<MSSteelGradeManager::eSteelGrade> eSGrades;
	eSGrades.Add(MSSteelGradeManager::SD300); eSGrades.Add(MSSteelGradeManager::SD350);	eSGrades.Add(MSSteelGradeManager::SD400); eSGrades.Add(MSSteelGradeManager::SD400S);	eSGrades.Add(MSSteelGradeManager::SD400W);
	eSGrades.Add(MSSteelGradeManager::SD500); eSGrades.Add(MSSteelGradeManager::SD500S);	eSGrades.Add(MSSteelGradeManager::SD500W); eSGrades.Add(MSSteelGradeManager::SD600); eSGrades.Add(MSSteelGradeManager::SD600S);	eSGrades.Add(MSSteelGradeManager::SD700);

	int numData = 6;
	for (int iData = 0; iData < numData; iData++)
	{
		MSReinforceBar* pReinforcebar = new MSReinforceBar;
		pReinforcebar->m_Name = Names[iData];
		pReinforcebar->m_dBarStrength = Strengths[iData];
		//		pReinforcebar->m_PrintName = PrintNames[iData];
		pReinforcebar->m_eSGrade = eSGrades[iData];
		pReinforcebar->GetID();
		mm_ReinforceBar.insert(make_pair(pReinforcebar->m_ID, pReinforcebar));
	}
}
// MSConstructionZone, MSGridLine, MSMarkG를 MSElement에서 파생시켜서 m_ID를 하나로 관리한다.
// 이전 버전과의 호환성을 위해, Serialize가 끝난 후 위 세개의 객체에 대해서 GetID()를 다시 호출하여 새로운 m_ID를 부여한다. - by Gracias
void MSRebarBuilding::Patch_Version20140729()
{
	// Floor안의 ConstructionZone::ResetGetID();
	map<long, MSFloor*>::iterator itFloor;
	for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); ++itFloor)
	{
		MSFloor* pFloor = itFloor->second;

		map<long, MSConstructionZone*> mapCZone(pFloor->mm_CZone.begin(), pFloor->mm_CZone.end());
		pFloor->mm_CZone.clear();
		map<long, MSConstructionZone*>::iterator itCZone;
		for (itCZone = mapCZone.begin(); itCZone != mapCZone.end(); ++itCZone)
		{
			MSConstructionZone* pCZone = itCZone->second;
			pCZone->ResetGetID();
			pFloor->mm_CZone.insert(make_pair(pCZone->m_ID, pCZone));
		}
	}

	// Building안의 GridLine::ResetGetID()
	map<long, MSGridLine*> mapGridLine(mm_GridLine.begin(), mm_GridLine.end());
	mm_GridLine.clear();

	map<long, MSGridLine*>::iterator itGrid;
	for (itGrid = mapGridLine.begin(); itGrid != mapGridLine.end(); ++itGrid)
	{
		MSGridLine* pGrid = itGrid->second;
		pGrid->ResetGetID();
		mm_GridLine.insert(make_pair(pGrid->m_ID, pGrid));
	}

	// Building안의 MarkG::ResetGetID()
	map<long, MSMarkG*> mapMarkG(mm_MarkG.begin(), mm_MarkG.end());
	mm_MarkG.clear();

	map<long, MSMarkG*>::iterator itMark;
	for (itMark = mapMarkG.begin(); itMark != mapMarkG.end(); ++itMark)
	{
		MSMarkG* pMark = itMark->second;
		pMark->ResetGetID();
		mm_MarkG.insert(make_pair(pMark->m_ID, pMark));
	}
}

void MSRebarBuilding::Patch_Material_Version20150701()
{
	vector<MSMaterial*> steelMaterials;
	vector<MSMaterial*> srcMaterials;
	vector<MSMaterial*> rcMaterial;
	map<long, MSMaterial*>::iterator it;
	for (it = mm_Material.begin(); it != mm_Material.end(); it++)
	{
		MSMaterial* pMaterial = it->second;
		if (pMaterial->GetType() == MSMaterial::msConcrete)
			rcMaterial.push_back(pMaterial);
		else if (pMaterial->GetType() == MSMaterial::msSteel)
			steelMaterials.push_back(pMaterial);
		else if (pMaterial->GetType() == MSMaterial::msSRC)
			srcMaterials.push_back(pMaterial);
	}

	int nSteel = steelMaterials.size();
	for (int i = 0; i < nSteel; i++)
		Delete(steelMaterials[i]);
	int nSRC = srcMaterials.size();
	for (int i = 0; i < nSRC; i++)
		Delete(srcMaterials[i]);

	steelMaterials.clear();
	// RH-639 Steel/SRC Modeling기획 참조
	MSSteel* pSteelMaterial = (MSSteel*)CreateNewMaterial(MSMaterial::msSteel);
	pSteelMaterial->m_Name = _T("SS400");
	pSteelMaterial->m_Code = MSSteel::STL_CODE_KS;
	pSteelMaterial->m_Grade = MSSteel::STL_GRADE_SS400;
	pSteelMaterial->m_Density = 7850;
	pSteelMaterial->GetID();
	Add(pSteelMaterial);
	steelMaterials.push_back(pSteelMaterial);

	int nRC = rcMaterial.size();
	nSteel = steelMaterials.size();
	for (int iRc = 0; iRc < nRC; iRc++)
	{
		MSMaterial* pRcMaterial = rcMaterial[iRc];
		for (int iStl = 0; iStl < nSteel; iStl++)
		{
			MSMaterial* pSteelMaterial = steelMaterials[iStl];
			MSSrcMaterial* pSRCMaterial = (MSSrcMaterial*)CreateNewMaterial(MSMaterial::msSRC);
			pSRCMaterial->mp_RcMaterial = (MSConcrete*)pRcMaterial;
			pSRCMaterial->mp_SteelMaterial = (MSSteel*)pSteelMaterial;
			pSRCMaterial->m_Name.Format(_T("%s/%s"), pRcMaterial->m_Name, pSteelMaterial->m_Name);
			pSRCMaterial->GetID();
			Add(pSRCMaterial);
		}
	}
}

void MSRebarBuilding::AddLevelZoneAfterSerialize()
{
	// Serializing 사용함
	map<long, MSFloor*>::iterator itFloor;
	for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->second;
		pFloor->m_bIsSerializing = true;
		vector<MSLevelZone*> LevelZoneArr = pFloor->ma_LevelZone;
		pFloor->ma_LevelZone.clear();
		long NumLevelZone = LevelZoneArr.size();
		for (long iLevelZone = 0; iLevelZone < NumLevelZone; iLevelZone++)
		{
			MSLevelZone* pLevelZone = LevelZoneArr[iLevelZone];
			pFloor->Add(pLevelZone);
		}
		pFloor->m_bIsSerializing = false;
	}
}

void MSRebarBuilding::DeleteGroups()
{
	map<long, MSGroup*>::iterator itGroup;
	for (itGroup = mm_Group.begin(); itGroup != mm_Group.end(); itGroup++)
	{
		itGroup->second->DeConvertBarSet4Member();//by LSS NX
		delete itGroup->second;
	}
	mm_Group.clear();
}

MSGroup* MSRebarBuilding::AddNewGroup(MSGroup* pNewGroup)
{
	pNewGroup->SetOwner(this);

	pair<map<long, MSGroup*>::iterator, bool> bPair;
	bPair = mm_Group.insert(make_pair(pNewGroup->m_ID, pNewGroup));

	if (bPair.second)
		return pNewGroup;
	else
		return NULL;
}

MSGroup* MSRebarBuilding::GetGroup(long GroupNum)
{
	mm_Group.find(GroupNum);

	map<long, MSGroup*>::iterator it = mm_Group.find(GroupNum);
	if (it == mm_Group.end())
		return NULL;

	return it->second;
}

bool MSRebarBuilding::DeleteGroup(long GroupNum)
{
	map<long, MSGroup*>::iterator it = mm_Group.find(GroupNum);
	if (it == mm_Group.end())
		return false;

	delete it->second;
	mm_Group.erase(it);

	return true;
}

bool MSRebarBuilding::DeleteGroup(CString GroupName)
{
	map<long, MSGroup*>::iterator itGroup;
	for (itGroup = mm_Group.begin(); itGroup != mm_Group.end(); itGroup++)
	{
		MSGroup* pGroup = itGroup->second;
		if (pGroup->GetName() == GroupName)
		{
			delete itGroup->second;
			mm_Group.erase(itGroup);
			return true;
		}
	}
	return false;
}

double MSRebarBuilding::GetTotalLength()
{
	double dTotalLength = 0.;
	map<long, MSGroup*>::iterator itGroup;
	for (itGroup = mm_Group.begin(); itGroup != mm_Group.end(); itGroup++)
	{
		MSGroup* pGroup = itGroup->second;
		dTotalLength += pGroup->GetTotalLength();
	}
	return dTotalLength;
}

//void MSRebarBuilding::GetRebarTotalLengthByDia( map<double,double>& mapLengthByDia )
//{
//	map<long, MSGroup*>::iterator itGroup;
//	for(itGroup=mm_Group.begin(); itGroup != mm_Group.end(); itGroup++)
//	{
//		MSGroup* pGroup = itGroup->second;
//		pGroup->GetRebarTotalLengthByDia(mapLengthByDia);
//	}	
//}

//by LSS beam/wall component의 volume및 formwork area계산을 위한 기본 준비/ 각 부재의 trim boundary를 만든다.
//void MSRebarBuilding::MakeCompoBoundary()
//{
//	if(bCalculationPrepared) return;
//
//	map<long, MSCompoG*>::iterator itMemberG;
//	for(itMemberG=mm_CompoG.begin(); itMemberG != mm_CompoG.end(); itMemberG++)
//	{
//		MSCompoG* pCompoG = itMemberG->second;
//		
//		pCompoG->MakeBoundary();			
//	}
//
//	map<long, MSFloor*>::iterator itFloor;
//	for(itFloor=mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
//	{
//		MSFloor* pFloor = itFloor->second;
//		CFloorManipulator floorManip(pFloor);
//		floorManip.TrimAllCompoG();//아래층과 연동해서 작동한다.
//	}
//
//
//	for(itFloor=mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
//	{
//		MSFloor* pFloor = itFloor->second;
//		CFloorManipulator floorManip(pFloor);
//		floorManip.MakeBoundary();//자신에게만 작동한다.
//		pFloor->MakeSlabEndContinuityInfo();// by LSS Slab 연결성 정보 작성
//	}
//
//	bCalculationPrepared = true;
//}

// 수직부재 M정보 변경시 같은위치, 같은이름으로 되었는 MSCompoG들중 변경될 MSCompoM이 해당층이 정의 안될때 삭제되어야 할 CompoGArr를 StoryMap으로 채워보낸다
BOOL MSRebarBuilding::GetDeletedCompoGs_byChangeMID(vector<MSCompoG*> CompoGArr, long nNewMID, long nMType, CMap<long, long, vector<MSCompoG*>, vector<MSCompoG*>>& StoryToCompoGArrMap)
{
	MSCompoM* pCompoM = GetCompoM(nNewMID);
	if (pCompoM == NULL)	return FALSE;
	FindType_MSElement eGType = FindType_MSElement::msNULL;
	if (nMType == MSCompoM::msColumnM)			eGType = FindType_MSElement::msColumnG;
	else if (nMType == MSCompoM::msButtressM)	eGType = FindType_MSElement::msColumnG;
	else if (nMType == MSCompoM::msPcColumnM)	eGType = FindType_MSElement::msColumnG;
	else if (nMType == MSCompoM::msBWallM)	eGType = FindType_MSElement::msWallG;
	else if (nMType == MSCompoM::msRWallM)	eGType = FindType_MSElement::msWallG;
	else if (nMType == MSCompoM::msCustomColumnM) eGType = FindType_MSElement::msColumnG;
	else if (nMType == MSCompoM::msCustomWallColumnM) eGType = FindType_MSElement::msWallColumnG;
	else return FALSE;

	CMap<CString, LPCTSTR, XMSRowM, XMSRowM> RowMMap;
	GetRowCompoMs(nMType, RowMMap);
	XMSRowM aRowM;
	if (RowMMap.Lookup(pCompoM->m_Name, aRowM) == FALSE)
		return FALSE;
	aRowM.SetOwner(this);

	long nFrom = -(GetNumUnderStory() - 1);
	long nTo = GetNumAboveStory() - 1;
	vector<MSCompoG*>::iterator it;
	for (it = CompoGArr.begin(); it != CompoGArr.end(); it++)
	{
		MSCompoG* pCompoG = *it;
		MSLinkMemberG* pLinkG = (MSLinkMemberG*)pCompoG;
		MSCompoM* pCompoM = pLinkG->GetCompoM();
		shared_ptr<GM2DCurve> pCenCurve = pLinkG->GetCenCurve();
		for (long iStory = nFrom; iStory <= nTo; iStory++)
		{
			if (iStory == 0)	continue;
			MSFloor* pFloor = GetFloor(iStory);
			if (pFloor == NULL)	continue;

			vector<MSCompoG*> CompoGArr = MSCompoGUtil::FindCompoG(pFloor, &*pCenCurve, eGType);
			for (long iFindG = 0; iFindG < CompoGArr.size(); iFindG++)
			{
				MSLinkMemberG* pOtherLinkG = (MSLinkMemberG*)CompoGArr[iFindG];
				if (pOtherLinkG == NULL)	continue;
				MSCompoM* pOtherCompoM = pOtherLinkG->GetCompoM();
				if (pOtherCompoM == NULL)	continue;
				if (pOtherCompoM->m_Name != pCompoM->m_Name)	continue;	// 같은 위치의 다른M이면 skip
				long nCurMID = aRowM.GetIncludeMIDbyFloorNum(iStory);
				MSCompoM* pCurCompoM = GetCompoM(nCurMID);
				if (pCurCompoM == NULL)
				{
					vector<MSCompoG*> EdtCompoGArr;
					StoryToCompoGArrMap.Lookup(iStory, EdtCompoGArr);
					EdtCompoGArr.push_back(pOtherLinkG);
					StoryToCompoGArrMap.SetAt(iStory, EdtCompoGArr);
				}
			}
		}
	}
	return TRUE;
}
// 수직부재 M정보 변경시 같은위치, 같은이름으로 되었는 MSCompoG들중 MID가 변경되어야 할 CompoGArr를 StoryMap으로 채워보낸다
BOOL MSRebarBuilding::GetEditedCompoGs_byChangeMID(vector<MSCompoG*> CompoGArr, long nNewMID, long nMType, CMap<long, long, vector<MSCompoG*>, vector<MSCompoG*>>& StoryToCompoGArrMap)
{
	MSCompoM* pCompoM = GetCompoM(nNewMID);
	if (pCompoM == NULL)	return FALSE;
	FindType_MSElement eGType = FindType_MSElement::msNULL;
	if (nMType == MSCompoM::msColumnM)			eGType = FindType_MSElement::msColumnG;
	else if (nMType == MSCompoM::msButtressM)	eGType = FindType_MSElement::msColumnG;
	else if (nMType == MSCompoM::msPcColumnM)	eGType = FindType_MSElement::msColumnG;
	else if (nMType == MSCompoM::msBWallM)	eGType = FindType_MSElement::msWallG;
	else if (nMType == MSCompoM::msRWallM)	eGType = FindType_MSElement::msWallG;
	else if (nMType == MSCompoM::msCustomColumnM) eGType = FindType_MSElement::msColumnG;
	else if (nMType == MSCompoM::msCustomWallColumnM) eGType = FindType_MSElement::msWallColumnG;
	else return FALSE;

	CMap<CString, LPCTSTR, XMSRowM, XMSRowM> RowMMap;
	GetRowCompoMs(nMType, RowMMap);
	XMSRowM aRowM;
	if (RowMMap.Lookup(pCompoM->m_Name, aRowM) == FALSE)
		return FALSE;
	aRowM.SetOwner(this);

	long nFrom = -(GetNumUnderStory() - 1);
	long nTo = GetNumAboveStory() - 1;
	vector<MSCompoG*>::iterator it;
	for (it = CompoGArr.begin(); it != CompoGArr.end(); it++)
	{
		MSCompoG* pCompoG = *it;
		MSLinkMemberG* pLinkG = (MSLinkMemberG*)pCompoG;
		MSCompoM* pCompoM = pLinkG->GetCompoM();
		shared_ptr<GM2DCurve> pCenCurve = pLinkG->GetCenCurve();
		for (long iStory = nFrom; iStory <= nTo; iStory++)
		{
			if (iStory == 0)	continue;
			MSFloor* pFloor = GetFloor(iStory);
			if (pFloor == NULL)	continue;
			vector<MSCompoG*> CompoGArr = MSCompoGUtil::FindCompoG(pFloor, &*pCenCurve, eGType);
			for (long iFindG = 0; iFindG < CompoGArr.size(); iFindG++)
			{
				MSLinkMemberG* pOtherLinkG = (MSLinkMemberG*)CompoGArr[iFindG];
				if (pOtherLinkG == NULL)	continue;
				MSCompoM* pOtherCompoM = pOtherLinkG->GetCompoM();
				if (pOtherCompoM == NULL)	continue;
				if (pOtherCompoM->m_Name != pCompoM->m_Name)	continue;	// 같은 위치의 다른M이면 skip
				long nCurMID = aRowM.GetIncludeMIDbyFloorNum(iStory);
				MSCompoM* pCurCompoM = GetCompoM(nCurMID);
				if (pCurCompoM)
				{
					vector<MSCompoG*> EdtCompoGArr;
					StoryToCompoGArrMap.Lookup(iStory, EdtCompoGArr);
					EdtCompoGArr.push_back(pOtherLinkG);
					StoryToCompoGArrMap.SetAt(iStory, EdtCompoGArr);
				}
			}
		}
	}
	return TRUE;
}

long MSRebarBuilding::ChangeCompoG_CompoMs(MSLinkMemberG* pLinkG, long nNewMID)
{
	if (pLinkG == NULL)	return FALSE;
	FindType_MSElement eType = pLinkG->GetFindType();
	//long nMType = 0;
	//if(nType==MSCompoG::msColumnG)			nMType = MSCompoM::msColumnM;
	//else if(nType==MSCompoG::msBWallG)	nMType = MSCompoM::msRWallM;
	//else if(nType==MSCompoG::msRWallG)	nMType = MSCompoM::msBWallM;
	//else	return FALSE;

	if (pLinkG->GetCompoM() == NULL)
		return FALSE;
	if (pLinkG->IsVerticalMember() == FALSE)
		return FALSE;

	long nMType = pLinkG->GetCompoM()->GetType();

	MSCompoM* pCompoM = GetCompoM(nNewMID);
	if (pCompoM == NULL)	return FALSE;

	CMap<CString, LPCTSTR, XMSRowM, XMSRowM> RowMMap;
	GetRowCompoMs(nMType, RowMMap);

	XMSRowM aRowM;
	if (RowMMap.Lookup(pCompoM->m_Name, aRowM) == FALSE)
		return FALSE;
	aRowM.SetOwner(this);

	shared_ptr<GM2DCurve> pCenCurve = pLinkG->GetCenCurve();

	long NumCount = 0;
	long nFrom = -(GetNumUnderStory() - 1);
	long nTo = GetNumAboveStory() - 1;
	for (long iStory = nFrom; iStory <= nTo; iStory++)
	{
		if (iStory == 0)	continue;
		MSFloor* pFloor = GetFloor(iStory);
		if (pFloor == NULL)	continue;
		vector<MSCompoG*> CompoGArr = MSCompoGUtil::FindCompoG(pFloor, &*pCenCurve, eType);
		for (long iFindG = 0; iFindG < CompoGArr.size(); iFindG++)
		{
			MSLinkMemberG* pOtherLinkG = (MSLinkMemberG*)CompoGArr[iFindG];
			if (pOtherLinkG == NULL)	continue;
			long nCurMID = aRowM.GetIncludeMIDbyFloorNum(iStory);
			MSCompoM* pCurCompoM = GetCompoM(nCurMID);
			if (pCurCompoM == NULL)	continue;
			pOtherLinkG->SetCompoM(nCurMID);
			NumCount++;
		}
	}
	return NumCount;
}

vector<long> MSRebarBuilding::GetCompoGs_InAllFloor(long nMID)
{
	vector<long> CompoGIDArr = GetCompoGs(nMID);

	map<long, MSCompoG*>::iterator itMemberG;
	map<long, MSFloor*>::iterator itFloor;
	for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->second;
		for (itMemberG = pFloor->mm_CompoG.begin(); itMemberG != pFloor->mm_CompoG.end(); itMemberG++)
		{
			MSCompoG* pCompoG = itMemberG->second;
			if (pCompoG == NULL)	continue;
			if (pCompoG->GetType() == MSCompoG::msStairG)
			{
				MSGenStairG* pStairG = (MSGenStairG*)pCompoG;
				if (pStairG->IsUsedMID(nMID))
					CompoGIDArr.push_back(pCompoG->m_ID);
				continue;
			}
			MSCompoM* pCompoM = pCompoG->GetCompoM();
			if (pCompoM == NULL)	continue;
			if (pCompoM->m_ID == nMID)
				CompoGIDArr.push_back(pCompoG->m_ID);
		}
	}
	return CompoGIDArr;
}

bool MSRebarBuilding::IsUsedReinforceM(long reinforceMID)
{
	map<long, MSCompoM*>::iterator it;
	for (it = mm_CompoM.begin(); it != mm_CompoM.end(); it++)
	{
		MSCompoM* pCompoM = it->second;
		if (pCompoM->GetType() == MSCompoM::msWindowM || pCompoM->GetType() == MSCompoM::msDoorM || pCompoM->GetType() == MSCompoM::msOpeningExtraM)
		{
			MSOpeningM* pOpeningM = (MSOpeningM*)pCompoM;
			if (pOpeningM->GetOpeningReinforceMID() == reinforceMID)
				return true;
		}
	}
	map<long, MSCompoG*>::iterator itMemberG;
	map<long, MSFloor*>::iterator itFloor;
	for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->second;
		for (itMemberG = pFloor->mm_CompoG.begin(); itMemberG != pFloor->mm_CompoG.end(); itMemberG++)
		{
			MSCompoG* pCompoG = itMemberG->second;
			if (pCompoG == NULL)	continue;
			if (pCompoG->GetType() == MSCompoG::msOpeningG)
			{
				MSOpeningM* pOpeningM = (MSOpeningM*)pCompoG->GetCompoM();
				if (pOpeningM->m_ID == reinforceMID)
					return true;
			}
			else if (pCompoG->GetType() == MSCompoG::msWallG)
			{
				MSWallMemberG* pWallMemberG = (MSWallMemberG*)pCompoG;

				map<long, MSWallOpeningG*>::iterator it;
				for (it = pWallMemberG->mm_WallOpening.begin(); it != pWallMemberG->mm_WallOpening.end(); it++)
				{
					MSWallOpeningG* pWallOpeningG = it->second;

					if (pWallOpeningG->mp_CompoM->m_ID == 0)
					{
						if (pWallOpeningG->mp_CompoM->GetType() == MSCompoM::msWindowM || pWallOpeningG->mp_CompoM->GetType() == MSCompoM::msDoorM || pWallOpeningG->mp_CompoM->GetType() == MSCompoM::msOpeningExtraM)
						{
							MSOpeningM* pOpeningM = (MSOpeningM*)pWallOpeningG->mp_CompoM;
							if (pOpeningM->GetOpeningReinforceMID() == reinforceMID)
								return true;
						}
					}
				}
			}
		}
	}

	return false;
}

bool MSRebarBuilding::IsUsedColumnHeadM(long columnHeadMID)
{
	map<long, MSCompoM*>::iterator it;
	for (it = mm_CompoM.begin(); it != mm_CompoM.end(); it++)
	{
		MSCompoM* pCompoM = it->second;
		if (pCompoM->GetType() == MSCompoM::msColumnM)
		{
			MSRcLineMemberM* pcolumnHeadM = (MSRcLineMemberM*)pCompoM;
			if (pcolumnHeadM->GetColumnHeaderMID() == columnHeadMID)
				return true;
		}
	}

	return false;
}

BOOL MSRebarBuilding::IsDeleteCompoM(long nMID)
{
	vector<long> CompoGIDArr = GetCompoGs_InAllFloor(nMID);
	if (CompoGIDArr.size() > 0)
		return FALSE;
	else
		return TRUE;
}

map<long, MSCompoG*> MSRebarBuilding::GetAllCompoGs(long nFloorNum, long nGType)
{
	map<long, MSCompoG*> RtnCompoGs;
	MSFloor* pFloor = GetFloor(nFloorNum);
	if (pFloor == NULL)	return RtnCompoGs;

	map<long, MSCompoG*>::iterator itMemberG;
	for (itMemberG = pFloor->mm_CompoG.begin(); itMemberG != pFloor->mm_CompoG.end(); itMemberG++)
	{
		MSCompoG* pCompoG = itMemberG->second;
		if (pCompoG == NULL)	continue;
		MSCompoM* pCompoM = pCompoG->GetCompoM();
		if (pCompoM == NULL)	continue;
		if (pCompoM->m_bDummy)	continue;
		if (pCompoG->GetType() == nGType)
			RtnCompoGs.insert(make_pair(pCompoG->m_ID, pCompoG));
	}
	return RtnCompoGs;
}

map<long, MSCompoG*> MSRebarBuilding::GetAllCompoGs(long nGType)
{
	map<long, MSCompoG*> RtnCompoGs;
	map<long, MSCompoG*>::iterator itMemberG;
	map<long, MSFloor*>::iterator itFloor;
	for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->second;
		for (itMemberG = pFloor->mm_CompoG.begin(); itMemberG != pFloor->mm_CompoG.end(); itMemberG++)
		{
			MSCompoG* pCompoG = itMemberG->second;
			if (pCompoG == NULL)	continue;
			if (pCompoG->GetType() == nGType)
				RtnCompoGs.insert(make_pair(pCompoG->m_ID, pCompoG));
		}
	}
	return RtnCompoGs;
}

map<long, MSCompoG*> MSRebarBuilding::GetAllCompoGs_SlabG(long nFloorNum, long nSlabType)
{
	map<long, MSCompoG*> RtnCompoGs;
	MSFloor* pFloor = GetFloor(nFloorNum);
	if (pFloor == NULL)	return RtnCompoGs;

	map<long, MSCompoG*>::iterator itMemberG;
	for (itMemberG = pFloor->mm_CompoG.begin(); itMemberG != pFloor->mm_CompoG.end(); itMemberG++)
	{
		MSCompoG* pCompoG = itMemberG->second;
		if (pCompoG == NULL)	continue;
		if (pCompoG->GetType() != MSCompoG::msSlabG)	continue;
		MSFaceMemberM* pCompoM = (MSFaceMemberM*)pCompoG->GetCompoM();
		if (pCompoM == NULL)	continue;
		if (pCompoM->m_bDummy)	continue;
		if (nSlabType == 3)
		{
			if (pCompoM->GetSlabMType() == 3 || pCompoM->GetSlabMType() == 4)
				RtnCompoGs.insert(make_pair(pCompoG->m_ID, pCompoG));
		}
		else
		{
			if (pCompoM->GetSlabMType() == nSlabType)
				RtnCompoGs.insert(make_pair(pCompoG->m_ID, pCompoG));
		}
	}
	return RtnCompoGs;
}

map<long, MSCompoG*> MSRebarBuilding::GetAllCompoGs_BeamG(long nFloorNum, bool IsPC)
{
	map<long, MSCompoG*> RtnCompoGs;
	MSFloor* pFloor = GetFloor(nFloorNum);
	if (pFloor == NULL)	return RtnCompoGs;

	map<long, MSCompoG*>::iterator itMemberG;
	for (itMemberG = pFloor->mm_CompoG.begin(); itMemberG != pFloor->mm_CompoG.end(); itMemberG++)
	{
		MSCompoG* pCompoG = itMemberG->second;
		if (pCompoG == NULL)	continue;
		if (pCompoG->GetType() != MSCompoG::msBeamG)	continue;
		MSFaceMemberM* pCompoM = (MSFaceMemberM*)pCompoG->GetCompoM();
		if (pCompoM == NULL)	continue;
		if (pCompoM->m_bDummy)	continue;
		if (pCompoM->IsPC() == IsPC)
			RtnCompoGs.insert(make_pair(pCompoG->m_ID, pCompoG));
	}
	return RtnCompoGs;
}

void MSRebarBuilding::GetAllCompoGsWithCompoM(MSCompoM* compoM, vector<MSCompoG*>& list)
{
	list.clear();
	map<long, MSCompoG*>::iterator itMemberG;
	map<long, MSFloor*>::iterator itFloor;
	for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->second;
		for (itMemberG = pFloor->mm_CompoG.begin(); itMemberG != pFloor->mm_CompoG.end(); itMemberG++)
		{
			MSCompoG* pCompoG = itMemberG->second;
			if (pCompoG == NULL)	continue;
			if (pCompoG->mp_CompoM == compoM)
				list.push_back(pCompoG);
			// 부재의 서부 부재도 검토할 필요가 있으면 추가 해준다.
			else if (pCompoG->GetType() == MSElement::msWallG)
			{
				MSWallMemberG* pWall = (MSWallMemberG*)pCompoG;
				map<long, MSWallOpeningG*>::iterator it;
				for (it = pWall->mm_WallOpening.begin(); it != pWall->mm_WallOpening.end(); it++)
				{
					MSWallOpeningG* pWallOpening = it->second;
					if (pWallOpening->mp_CompoM == compoM)
						list.push_back(pCompoG);
				}
			}
		}
	}
}

void MSRebarBuilding::Trim()
{
	//map<long, MSFloor*>::iterator itFloor;
	//for(itFloor=mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	//{
	//	MSFloor* pFloor = itFloor->second;
	//	CFloorManipulator floorManip(pFloor);
	//	floorManip.TrimAllCompoG();//아래층과 연동해서 작동한다.
	//}

}
//
//void MSRebarBuilding::UpdateJointZc()
//{
//	long nFrom = -GetNumUnderStory();
//	long nTo   = GetNumAboveStory();
//
//	map<long, MSConnectivity*>::iterator it;
//	for(long iStory=nFrom; iStory<=nTo; iStory++)
//	{
//		if(iStory==0)	continue;
//		MSFloor* pFloor = GetFloor(iStory);
//		if(pFloor==NULL)	continue;
//		
//		double dZc = GetFloorLevel(iStory);
//		for(it=pFloor->mm_Joint.begin(); it != pFloor->mm_Joint.end(); it++)
//		{
//			MSConnectivity* pJoint = it->second;
//			pJoint->m_dZc = dZc;
//		}
//	}
//}

// Floor삭제시 Roof층이 되는 MSFloor에 속해 있는 수직부재를 삭제한다.
void MSRebarBuilding::RemoveRootFloor_VMembers()
{
	//long nRoofStory = GetNumAboveStory();
	//MSFloor* pRoofFloor = GetFloor(nRoofStory);
	//if(pRoofFloor==NULL)	return;

	//map<long, MSCompoG*>::iterator it;
	//for(it = pRoofFloor->mm_CompoG.begin(); it != pRoofFloor->mm_CompoG.end(); it++)
	//{
	//	MSCompoG* pCompoG = it->second;
	//	if(pCompoG==NULL)	continue;

	//	//long nGType = pCompoG->GetType();
	//	//if(nGType==MSCompoG::msColumnG || nGType==MSCompoG::msBWallG || nGType==MSCompoG::msRWallG)
	//	//	pRoofFloor->Remove(pCompoG);
	//	if(pCompoG->IsVerticalMember())
	//		pRoofFloor->Remove(pCompoG);
	//}
}

long MSRebarBuilding::MakePlaceStoryMap(map<long, vector<long>>& PlaceStoryMap)
{
	map<long, vector<long>>::iterator it;
	long nFrom = -GetNumUnderStory();
	long nTo = GetNumAboveStory();
	for (long iStory = nFrom; iStory <= nTo; iStory++)
	{
		if (iStory == 0)	continue;
		MSFloor* pFloor = GetFloor(iStory);
		if (pFloor == NULL)	continue;

		if (pFloor->m_nPlaceStory == 0)	continue;
		MSFloor* pPlaceFloor = GetFloor(pFloor->m_nPlaceStory);
		if (pPlaceFloor == NULL)	continue;

		it = PlaceStoryMap.find(pFloor->m_nPlaceStory);
		if (it == PlaceStoryMap.end())
		{
			vector<long> aStoryArr;
			aStoryArr.push_back(pFloor->GetFloorNum());
			PlaceStoryMap.insert(make_pair(pFloor->m_nPlaceStory, aStoryArr));
		}
		else
		{
			vector<long> aStoryArr = it->second;
			aStoryArr.push_back(pFloor->GetFloorNum());
			PlaceStoryMap.erase(it);
			PlaceStoryMap.insert(make_pair(pFloor->m_nPlaceStory, aStoryArr));
		}
	}
	return (long)PlaceStoryMap.size();
}

//bool MSRebarBuilding::CheckMidNodes()
//{
//	map<long, MSCompoG*>::iterator itMemberG;
//	for(itMemberG=mm_CompoG.begin(); itMemberG != mm_CompoG.end(); itMemberG++)
//	{
//		MSCompoG* pCompoG = itMemberG->second;
//		pCompoG->CheckMidNodes(false);
//	}
//	map<long, MSFloor*>::iterator itFloor;
//	for(itFloor=mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
//	{
//		MSFloor* pFloor = itFloor->second;
//		map<long, MSCompoG*>::iterator itMemberG;
//		for(itMemberG=pFloor->mm_CompoG.begin(); itMemberG != pFloor->mm_CompoG.end(); itMemberG++)
//		{
//			MSCompoG* pCompoG = itMemberG->second;
//			pCompoG->CheckMidNodes(false);
//		}
//	}
//	return true;
//}

//by LSS Export All CompoGs in this Building
vector<MSCompoG*> MSRebarBuilding::GetAllCompoGs()
{
	vector<MSCompoG*> CompoGArr;
	map<long, MSCompoG*>::iterator itMemberG;
	for (itMemberG = mm_CompoG.begin(); itMemberG != mm_CompoG.end(); itMemberG++)
	{
		MSCompoG* pCompoG = itMemberG->second;
		if (pCompoG)	CompoGArr.push_back(pCompoG);
	}

	map<long, MSFloor*>::iterator itFloor;
	for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->second;
		for (itMemberG = pFloor->mm_CompoG.begin(); itMemberG != pFloor->mm_CompoG.end(); itMemberG++)
		{
			MSCompoG* pCompoG = itMemberG->second;
			if (pCompoG)	CompoGArr.push_back(pCompoG);
		}
	}
	return CompoGArr;
}

vector<MSFloor*> MSRebarBuilding::GetAllFloors()
{
	vector<MSFloor*> floors;
	floors.reserve(mm_Floor.size());
	for (map<long, MSFloor*>::iterator itr = mm_Floor.begin(); itr != mm_Floor.end(); ++itr)
		floors.push_back(itr->second);

	return floors;
}

/*
void MSRebarBuilding::GetAllFloors(vector<MSFloor*>& floors)
{
	floors.clear();
	floors.reserve(mm_Floor.size());
	for(map<long, MSFloor*>::iterator itr = mm_Floor.begin() ; itr != mm_Floor.end() ; ++itr)
		floors.push_back(itr->second);
}
*/

void MSRebarBuilding::ClearAllCashFloorLevel()
{
	for (map<long, MSFloor*>::iterator itr = mm_Floor.begin(); itr != mm_Floor.end(); ++itr)
		itr->second->ClearCashFloorLevel();
}

void MSRebarBuilding::DeleteCompoMs()
{
	map<long, MSCompoM*>::iterator it;
	for (it = mm_CompoM.begin(); it != mm_CompoM.end(); it++)
		delete it->second;
	mm_CompoM.clear();
}
MSCompoM* MSRebarBuilding::GetCompoM(long CompoMID)
{
	map<long, MSCompoM*>::iterator it = mm_CompoM.find(CompoMID);
	if (it == mm_CompoM.end())
	{
		return NULL;
	}

	return it->second;
}

MSCompoM* MSRebarBuilding::GetCompoM(CString strName, long nType, long nStory)
{
	if (IsVerticalCompoM(nType))
	{
		CMap<CString, LPCTSTR, XMSRowM, XMSRowM> RowMMap;
		GetRowCompoMs(nType, RowMMap);
		POSITION Pos = RowMMap.GetStartPosition();
		CString strKeyName;
		while (Pos)
		{
			XMSRowM aRowM;	aRowM.Initialize();
			RowMMap.GetNextAssoc(Pos, strKeyName, aRowM);
			if (strKeyName != strName)	continue;
			long nMID = aRowM.GetIncludeMIDbyFloorNum(nStory);
			MSCompoM* pCompoM = GetCompoM(nMID);
			if (pCompoM == NULL)	continue;
			if (pCompoM->m_Name == strName)
				return pCompoM;
		}
	}
	else
	{
		vector<long> CompoMIDArr = GetCompoMs(nType);
		vector<long>::iterator it;
		for (it = CompoMIDArr.begin(); it != CompoMIDArr.end(); it++)
		{
			long nMID = *it;
			MSCompoM* pCompoM = GetCompoM(nMID);
			if (pCompoM == NULL)	continue;
			if (pCompoM->m_Name == strName)
				return pCompoM;
		}
	}
	return NULL;
}

// MSCompoM* MSRebarBuilding::FindCompoM(CString& strName, long nFloorNum)
// {
// 	MSCompoM* ret = NULL;
// 	map<long, MSCompoM*>::iterator itCompoM;
// 	for(itCompoM=mm_CompoM.begin(); itCompoM != mm_CompoM.end(); itCompoM++)
// 	{
// 		MSCompoM* cur = itCompoM->second;
// 		if (cur->m_Name == strName && cur->IsStoryIncluded(nFloorNum))
// 		{
// 			ret = cur;
// 			break;
// 		}
// 	}
// 	return ret;
// }

bool MSRebarBuilding::ExistCompoM(CString& strName, long nFloorNum)
{
	if (strName == "NONAME") return true;

	bool ret = false;
	map<long, MSCompoM*>::iterator itCompoM;
	for (itCompoM = mm_CompoM.begin(); itCompoM != mm_CompoM.end(); itCompoM++)
	{
		MSCompoM* cur = itCompoM->second;
		if (cur->m_Name == strName && cur->IsStoryIncluded(nFloorNum))
		{
			ret = true;
			break;
		}
	}
	return ret;
}

vector<long> MSRebarBuilding::GetCompoMs()
{
	vector<long> itID;
	map<long, MSCompoM*>::iterator itCompoM;
	for (itCompoM = mm_CompoM.begin(); itCompoM != mm_CompoM.end(); itCompoM++)
	{
		long nCompoMID = itCompoM->first;
		itID.push_back(nCompoMID);
	}
	return itID;

}

vector<long> MSRebarBuilding::GetCompoMs(long nType)
{
	CArray<long, long> MIDArr;
	vector<long> itID;
	map<long, MSCompoM*>::iterator itCompoM;
	for (itCompoM = mm_CompoM.begin(); itCompoM != mm_CompoM.end(); itCompoM++)
	{
		MSCompoM* pCompoM = itCompoM->second;
		if (pCompoM == NULL)	continue;
		if (pCompoM->GetType() == nType)
		{
			MIDArr.Add(pCompoM->m_ID);
		}
	}
	long NumMID = MIDArr.GetSize();
	for (long iMID = 0; iMID < NumMID; iMID++)
	{
		for (long jMID = iMID + 1; jMID < NumMID; jMID++)
		{
			if (GetCompoM(MIDArr[iMID])->m_Name > GetCompoM(MIDArr[jMID])->m_Name)
			{
				long nTempID = MIDArr[iMID];
				MIDArr[iMID] = MIDArr[jMID];
				MIDArr[jMID] = nTempID;
			}
		}
	}

	for (iMID = 0; iMID < NumMID; iMID++)
	{
		itID.push_back(MIDArr[iMID]);
	}

	return itID;
}

long MSRebarBuilding::GetRowCompoMs(long nType, map<CString, vector<MSCompoM*>>& CompoMMap)
{
	map<long, MSCompoM*>::iterator it;
	for (it = mm_CompoM.begin(); it != mm_CompoM.end(); ++it)
	{
		MSCompoM* pCompoM = it->second;
		if (pCompoM->GetType() != nType)	continue;
		vector<MSCompoM*> compoMArr;
		map<CString, vector<MSCompoM*>>::iterator itName = CompoMMap.find(pCompoM->m_Name);
		if (itName == CompoMMap.end())
			compoMArr.push_back(pCompoM);
		else
		{
			compoMArr = itName->second;
			compoMArr.push_back(pCompoM);
			CompoMMap.erase(itName);
		}
		CompoMMap.insert(make_pair(pCompoM->m_Name, compoMArr));
	}
	return (long)CompoMMap.size();
}

long MSRebarBuilding::GetRowCompoMs(long nType, CMap<CString, LPCTSTR, XMSRowM, XMSRowM>& RowMMap)
{
	if (IsVerticalCompoM(nType))
	{
		vector<long> CompoMIDArr = GetCompoMs(nType);
		vector<long>::iterator it;
		for (it = CompoMIDArr.begin(); it != CompoMIDArr.end(); it++)
		{
			long nMID = *it;
			MSCompoM* pCompoM = GetCompoM(nMID);
			if (pCompoM == NULL)	continue;

			XMSRowM aRowM;	aRowM.Initialize();
			if (RowMMap.Lookup(pCompoM->m_Name, aRowM))
				aRowM.AddMID(nMID);
			else
			{
				aRowM.AddMID(nMID);
				aRowM.m_strName = pCompoM->m_Name;
				aRowM.m_nRepresentMID = nMID;
				aRowM.SetOwner(this);
			}
			RowMMap.SetAt(pCompoM->m_Name, aRowM);
		}
		return RowMMap.GetCount();
	}
	else
		ASSERT(0);
	return 0;
}

long MSRebarBuilding::GetRowCompoM(long nType, long nRowMID, XMSRowM& aRowM)
{
	MSCompoM* pRowCompoM = GetCompoM(nRowMID);
	if (pRowCompoM == NULL)	return 0;

	aRowM.AddMID(nRowMID);
	if (IsVerticalCompoM(nType))
	{
		vector<long> CompoMIDArr = GetCompoMs(nType);
		vector<long>::iterator it;
		for (it = CompoMIDArr.begin(); it != CompoMIDArr.end(); it++)
		{
			long nMID = *it;
			MSCompoM* pCompoM = GetCompoM(nMID);
			if (pCompoM == NULL)	continue;
			if (pCompoM->m_ID == nRowMID)	continue;
			if (pCompoM->GetType() != nType)	continue;
			if (pCompoM->m_Name != pRowCompoM->m_Name)	continue;

			aRowM.AddMID(nMID);
			aRowM.m_strName = pCompoM->m_Name;
			aRowM.m_nRepresentMID = nMID;
		}
		return TRUE;
	}
	else
		ASSERT(0);
	return 0;
}

bool MSRebarBuilding::Add(MSCompoM* pCompoM)
{
	pair<map<long, MSCompoM*>::iterator, bool> bPair;
	bPair = mm_CompoM.insert(make_pair(pCompoM->m_ID, pCompoM));
	return bPair.second;
}

bool MSRebarBuilding::Add(vector<MSCompoM*> CompoMArr)
{
	bool rVal = true;
	for (int iCompoM = 0; iCompoM < CompoMArr.size(); iCompoM++)
	{
		MSCompoM* pCompoM = CompoMArr[iCompoM];
		if (pCompoM == nullptr) continue;

		bool isAdd = Add(CompoMArr[iCompoM]);
		if (isAdd == false)
			rVal = false;
	}

	return rVal;
}

long MSRebarBuilding::Remove(MSCompoM* pCompoM)
{
	long nID = pCompoM->m_ID;
	map<long, MSCompoM*>::iterator it = mm_CompoM.find(nID);
	if (it == mm_CompoM.end())	return 0;
	mm_CompoM.erase(it);
	return nID;
}

bool MSRebarBuilding::Remove(vector<MSCompoM*> CompoMArr)
{
	bool rVal = true;
	for (int iCompoM = 0; iCompoM < CompoMArr.size(); iCompoM++)
	{
		MSCompoM* pCompoM = CompoMArr[iCompoM];
		if (pCompoM == nullptr) continue;

		long nMID = Remove(CompoMArr[iCompoM]);
		if (nMID == 0)
			rVal = false;
	}

	return rVal;
}

long MSRebarBuilding::Delete(MSCompoM* pCompoM)
{
	long nID = Remove(pCompoM);
	delete pCompoM;
	return nID;
}

vector<long> MSRebarBuilding::GetCompoGs(long nMID)
{
	vector<long> itID;
	map<long, MSCompoG*>::iterator itCompoG;
	for (itCompoG = mm_CompoG.begin(); itCompoG != mm_CompoG.end(); itCompoG++)
	{
		MSCompoG* pCompoG = itCompoG->second;
		if (pCompoG == NULL)	continue;
		MSCompoM* pCompoM = pCompoG->GetCompoM();
		if (pCompoM == NULL)	continue;
		if (pCompoM->m_ID == nMID)
			itID.push_back(pCompoG->m_ID);
	}
	return itID;
}

MSMaterial* MSRebarBuilding::GetDefMaterial()
{
	map<long, MSMaterial*>::iterator it;
	for (it = mm_Material.begin(); it != mm_Material.end(); it++)
	{
		MSMaterial* pMaterial = it->second;
		if (pMaterial->IsPlainConcreteReinforceBar() == true) continue;
		if (pMaterial == NULL || pMaterial->m_nType != MSMaterial::Type_MSMaterial::msConcrete) continue;

		return pMaterial;
	}
	return NULL;
}

MSMaterial* MSRebarBuilding::GetDefMaterial(MSMaterial::Type_MSMaterial materialType)
{
	map<long, MSMaterial*>::iterator it;
	for (it = mm_Material.begin(); it != mm_Material.end(); it++)
	{
		MSMaterial* pMaterial = it->second;
		if (pMaterial->IsPlainConcreteReinforceBar() == true) continue;
		if (pMaterial && pMaterial->m_nType != materialType) continue;

		return pMaterial;
	}
	return NULL;
}

void MSRebarBuilding::Init(bool bInConstructor/*=false*/)
{
	if (!bInConstructor)
		MSBaseBuilding::Init(bInConstructor);

	m_strFilePath = _T("");
	m_dGroundLevel = 0.0;
	m_bTopDownPlace = FALSE;
	m_nTopDownStartFloor = -1;
	bCalculationPrepared = false;
	//m_SiteRect.m_Origin->SetXY(-100000, -100000);
	//m_SiteRect.m_Corner->SetXY(100000,   100000);
	m_SiteRect.m_Origin.SetXY(0, 0);
	m_SiteRect.m_Corner.SetXY(0, 0);
	m_DefaultReinforceBarSettingID = 0;
	m_PlainConcreteReinforceBarSettingID = 0;
	//temporary Xerror Example by LSS
// 	XErrorObject* eo =XErrorContainer::GetMessageContainer()->CreateErrorObject(1,1,_T("From Native Building"));
// 	XErrorContainer::GetMessageContainer()->AddErrorObject(eo);

	m_pCheckModelOption = MSCheckModelOption::GetInstance();
	m_pSGradeManager = new MSSteelGradeManager();

	m_bIsNeedRegenAfterLoad = false;
	m_eUpdateType = BuildingUpdateType_NONE;

	m_nRebarTemplateID = -1;
	m_nFormWorkTemplateID = -1;

	m_nLicenseVersion = -1;

	m_nBuildingInfoID = -1;

	facilitySeparationType = 0;
	m_nNayukTemplateID = -1;

	NoteClear();

}

void MSRebarBuilding::Dump(EFS::EFS_Buffer& buffer)
{
	MSBaseBuilding::Dump(buffer);
	buffer << m_dGroundLevel << m_DefaultReinforceBarSettingID << m_Worker << m_PlainConcreteReinforceBarSettingID;
	buffer << m_nRebarTemplateID << m_nFormWorkTemplateID << m_nLicenseVersion << m_nNayukTemplateID;
	mp_UCS->Dump(buffer);

	buffer << m_bTopDownPlace << m_nTopDownStartFloor;
}

void MSRebarBuilding::Recovery(EFS::EFS_Buffer& buffer)
{
	MSBaseBuilding::Recovery(buffer);
	buffer >> m_dGroundLevel >> m_DefaultReinforceBarSettingID >> m_Worker >> m_PlainConcreteReinforceBarSettingID;
	buffer >> m_nRebarTemplateID >> m_nFormWorkTemplateID >> m_nLicenseVersion >> m_nNayukTemplateID;
	mp_UCS->Recovery(buffer);

	buffer >> m_bTopDownPlace >> m_nTopDownStartFloor;
}

CString MSRebarBuilding::GetWorkerA()
{
	return m_Worker;
}
void MSRebarBuilding::SetWorkerA(CString worker)
{
	m_Worker = worker;
}

void MSRebarBuilding::ChangFloorQT(double dMinX, double dMinY, double dMaxX, double dMaxY)
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

	map<long, MSFloor*>::iterator itFloor;
	for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->second;
		pFloor->SetSite();
		pFloor->ResetFloorQTLeaves();
	}
}

int MSRebarBuilding::CheckModel(bool bMakeErrorObject, bool bMakeOutputReport)
{

	MSModelChecker ModelChecker(this, bMakeErrorObject, bMakeOutputReport);
	int nErrorCnt = ModelChecker.CheckModel();

	return nErrorCnt;
}

int MSRebarBuilding::CheckGeometryModel(bool bMakeErrorObject, bool bMakeOutputReport)
{
	MSModelChecker ModelChecker(this, bMakeErrorObject, bMakeOutputReport);
	int nErrorCnt = ModelChecker.CheckGeometryModel();

	return nErrorCnt;
}

MSRibTypeM* MSRebarBuilding::CreateNewRibM(int RibType)
{
	MSRibTypeM* pRibM = NULL;
	switch (RibType)
	{
	case MSRibTypeM::Rib_Standard:
		return new MSStandardRibTypeM();
	}
	return pRibM;
}

void MSRebarBuilding::ReGenerateJoint()
{
	m_nUpdateModelVersion = MSVersionInfo::GetLatestVersion();
	if (mm_Floor.size() == 0)
		return;

	if (AppGlobalOptionWrapper::Instance()->GetAppMode() != AppGlobalOptionWrapper::Finish)
	{
		ProgressReporterWrapper::GetProgressReporterWithBeginWaiting(_T("Processing Update Story ..."));
		ProgressReporterWrapper::Instance()->SetProgressMaxValue(100);
	}
	else
	{
		ProgressReporterWrapper::AddStatus(_T("Processing Update MSConnection ..."));
		ProgressReporterWrapper::Instance()->ChangeProgress(0);
	}

	try
	{
		int nCnt = 0;
		int percent = (int)((1 * 100) / (mm_Floor.size()));
		map<long, MSFloor*>::iterator itFloor;
		for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
		{
			MSFloor* pFloor = itFloor->second;
			if (pFloor == NULL) continue;

			ProgressReporterWrapper::Instance()->AddProgress(percent);

			CString strMsg;
			strMsg.Format(_T("Processing Update Story (%s)"), pFloor->GetName());
			ProgressReporterWrapper::Instance()->AddStatus(strMsg);

			pFloor->UpdateCompoGs();
		}

		if (AppGlobalOptionWrapper::Instance()->GetAppMode() != AppGlobalOptionWrapper::Finish)
			ProgressReporterWrapper::DisposeProgressReporter();

		LoggingCompoGsTypeGeo();
		LogningConnections();
	}
	catch (CException* e)
	{
		if (AppGlobalOptionWrapper::Instance()->GetAppMode() != AppGlobalOptionWrapper::Finish)
			ProgressReporterWrapper::DisposeProgressReporter();
	}
}

void MSRebarBuilding::LoggingCompoGsTypeGeo()
{
	return;
#ifdef _DEBUG 
	double dTotalLineLen = 0;
	double dTotalBoundaryLen = 0;

	for (auto itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->second;
		if (pFloor == NULL) continue;

		map<MSElement::Type_MSCompoG, vector<MSCompoG*>> CompoGTypeMap = pFloor->GetCompGTypeListMap();
		map<CString, tuple<double, double>> CompoGGeoMapData = pFloor->GetCompoGsTypeGeoData(CompoGTypeMap);
		pFloor->LoggingCompoGsTypeGeo(CompoGGeoMapData);

		for (auto it = CompoGGeoMapData.begin(); it != CompoGGeoMapData.end(); it++)
		{
			dTotalLineLen += get<0>(it->second);
			dTotalBoundaryLen += get<1>(it->second);
		}
	}

	CString strLog;
	strLog.Format(_T("Total_BoundaryLen:%f, Total_LineLen:%f"), dTotalBoundaryLen, dTotalLineLen);
	StatusReporterWrapper::OutputMessage(strLog);
	LoggerWrapper::Log(strLog);
#endif
}

void MSRebarBuilding::LogningConnections()
{
	return;
#ifdef _DEBUG 
	for (auto itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->second;
		if (pFloor == NULL) continue;

		pFloor->LogningConnections();
	}
#endif
}

void MSRebarBuilding::FixCompoGInfoAfterSerialize()
{
	if (mm_Floor.size() == 0)
		return;

	// MSFloor::FixCompoGInfoAfterSerialize() 함수의 마지막 버전을 체크한다.
	if (MSVersionInfo::GetCurrentVersion() >= 20250412)
		return;

	ProgressReporterWrapper::GetProgressReporterWithBeginWaiting(_T("Processing Update Story ..."));
	try
	{
		ProgressReporterWrapper::Instance()->SetProgressMaxValue(100);

		int nCnt = 0;
		int percent = (int)((1 * 100) / (mm_Floor.size()));
		map<long, MSFloor*>::iterator itFloor;
		for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
		{
			MSFloor* pFloor = itFloor->second;
			if (pFloor == NULL) continue;

			ProgressReporterWrapper::Instance()->AddProgress(percent);

			CString strMsg;
			strMsg.Format(_T("Processing Update Story (%s)"), pFloor->GetName());
			ProgressReporterWrapper::Instance()->AddStatus(strMsg);

			pFloor->FixCompoGInfoAfterSerialize();
		}

		ProgressReporterWrapper::DisposeProgressReporter();
	}
	catch (CException* e)
	{
		ProgressReporterWrapper::DisposeProgressReporter();
	}

}

void MSRebarBuilding::FixBoundaryCompoGs()
{
	if (mm_Floor.size() == 0)
		return;

	if (m_eUpdateType == BuildingUpdateType_ALL)
		return;

	ProgressReporterWrapper::GetProgressReporterWithBeginWaiting(_T("Processing Update Story ..."));
	try
	{
		ProgressReporterWrapper::Instance()->SetProgressMaxValue(100);

		int nCnt = 0;
		int percent = (int)((1 * 100) / (mm_Floor.size()));
		map<long, MSFloor*>::iterator itFloor;
		for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
		{
			MSFloor* pFloor = itFloor->second;
			if (pFloor == NULL) continue;

			ProgressReporterWrapper::Instance()->AddProgress(percent);

			CString strMsg;
			strMsg.Format(_T("Processing Update Story (%s)"), pFloor->GetName());
			ProgressReporterWrapper::Instance()->AddStatus(strMsg);

			pFloor->FixBoundaryCompoGs();
		}

		ProgressReporterWrapper::DisposeProgressReporter();
	}
	catch (CException* e)
	{
		ProgressReporterWrapper::DisposeProgressReporter();
	}
}

void MSRebarBuilding::CheckInValidElements()
{
	map<long, MSFloor*>::iterator itFloor;
	for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->second;
		if (pFloor == NULL) continue;
		pFloor->CheckInValidElements();
	}
}

MSMaterial* MSRebarBuilding::GetRcMaterialbyName(CString name)
{
	return GetMaterial(MSMaterial::msConcrete, name);
}
MSMaterial* MSRebarBuilding::GetSteelMaterialbyName(CString name)
{
	return GetMaterial(MSMaterial::msSteel, name);
}
MSMaterial* MSRebarBuilding::GetSrcMaterialbyName(CString name)
{
	return GetMaterial(MSMaterial::msSRC, name);
}

MSMaterial* MSRebarBuilding::GetMaterial(MSMaterial::Type_MSMaterial type, CString name)
{
	map<long, MSMaterial*>::iterator it;
	for (it = mm_Material.begin(); it != mm_Material.end(); it++)
	{
		MSMaterial* pMaterial = it->second;
		if (pMaterial->GetType() == type && pMaterial->m_Name == LocalDic::GetLocal(name))
			return pMaterial;
	}
	return NULL;
}

vector<MSMaterial*> MSRebarBuilding::GetMaterialsByType(MSMaterial::Type_MSMaterial type)
{
	vector<MSMaterial*> materialArr;
	for (auto it = mm_Material.begin(); it != mm_Material.end(); it++)
	{
		if (it->second->GetType() == type)
			materialArr.push_back(it->second);
	}
	return materialArr;
}

long MSRebarBuilding::GetModelCheckElementCnt()
{
	long nElementCnt = 0;

	long AboveStory = GetNumAboveStory();
	long UnderStory = -(GetNumUnderStory() - 1);

	for (long iStory = AboveStory; iStory >= UnderStory; iStory--)
	{
		if (iStory == 0)	continue;
		MSFloor* pCurFloor = GetFloor(iStory);
		if (pCurFloor == NULL)	continue;

		nElementCnt += pCurFloor->mm_CompoG.size();
		nElementCnt += pCurFloor->ma_LevelZone.size();
		nElementCnt += pCurFloor->mm_CZone.size();
	}

	return nElementCnt;
}


void MSRebarBuilding::PopupCheckMsgNBuildingUpdate()
{
	// bool MSRebarBuildingCli::LoadFile( String^ FileName ) 에서 한번 호출 한다.

	bool isRunReGenerateJoint = false;

	if (m_eUpdateType == BuildingUpdateType_ALL)
	{
		if (RadfContextWrapper::Instance()->GetAutoTestCheck() == true || RadfContextWrapper::Instance()->GetAutoMultiQTSetCheck() == true)
		{
			// Model 과 3D Regen 실행
			ReGenerateJoint();
			m_bIsNeedRegenAfterLoad = true;
			isRunReGenerateJoint = true;
		}
		else
		{
			int nRVal = MessageBox(NULL, LANG2(ISOLDVERSIONALLUPDATE, CheckData), _T("ModelCheck"), MB_YESNO | MB_DEFBUTTON1 | MB_ICONWARNING | MB_TOPMOST);
			if (nRVal == IDYES)
			{
				// Model 과 3D Regen 실행
				ReGenerateJoint();
				m_bIsNeedRegenAfterLoad = true;
				isRunReGenerateJoint = true;
			}
			else
				m_eUpdateType = BuildingUpdateType_NONE;
		}
	}
	else
	{
		if (m_eUpdateType == BuildingUpdateType_3DView) // 이 경우는 3D 만 Regen
		{
			if (RadfContextWrapper::Instance()->GetAutoTestCheck() == true || RadfContextWrapper::Instance()->GetAutoMultiQTSetCheck() == true)
				m_bIsNeedRegenAfterLoad = true;
			else
			{
				int nRVal = MessageBox(NULL, LANG2(ISOLDVERSIONUPDATE, CheckData), _T("ModelCheck"), MB_YESNO | MB_DEFBUTTON1 | MB_ICONWARNING | MB_TOPMOST);
				if (nRVal == IDYES)
					m_bIsNeedRegenAfterLoad = true;
				else
					m_eUpdateType = BuildingUpdateType_NONE;
			}

			if(m_bIsNeedRegenAfterLoad == true)
				m_nUpdateModelVersion = MSVersionInfo::GetLatestVersion();
		}
	}

	if(isRunReGenerateJoint == false)
		FixCompoGInfoAfterSerialize();
	CheckInValidElements();
}

void MSRebarBuilding::UpdateWallGtoWallEndRebarLineID(int wallEndRebarLineID)
{
	/*map<long, MSFloor*>::iterator itFloor;
	for(itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); ++itFloor)
	{
		MSFloor* pFloor = itFloor->second;
		map<long, MSCompoG*>::iterator itCompoG;
		for(itCompoG = pFloor->mm_CompoG.begin(); itCompoG != pFloor->mm_CompoG.end(); ++itCompoG)
		{
			MSCompoG* pCompoG = itCompoG->second;
			MSWallMemberG* pWallG = dynamic_cast<MSWallMemberG*>(pCompoG);
			if(pWallG != NULL)
			{
				if(pWallG->GetStartEndRebarLineID() == wallEndRebarLineID)
					MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(pWallG->m_ID);
				else if(pWallG->GetEndEndRebarLineID() == wallEndRebarLineID)
					MSFloor::CompoGIDList_NeedRegen_CurTR().push_back(pWallG->m_ID);
			}
		}
	}*/
}

vector<MSCompoM*> MSRebarBuilding::GetAllColumnMsWidthColumnHeaderMID(long headerMID)
{
	vector<MSCompoM*> columnMArr;
	vector<long> ColumnMIDs = GetCompoMs(MSCompoM::msColumnM);
	vector<long> PCColumnMIDs = GetCompoMs(MSCompoM::msPcColumnM);
	for (int i = 0; i < ColumnMIDs.size(); i++)
	{
		MSRcLineMemberM* pColumnM = (MSRcLineMemberM*)GetCompoM(ColumnMIDs[i]);
		if (pColumnM == NULL)	 continue;

		if (pColumnM->GetColumnHeaderMID() == headerMID)
			columnMArr.push_back(pColumnM);
	}
	for (int i = 0; i < PCColumnMIDs.size(); i++)
	{
		MSRcLineMemberM* pColumnM = (MSRcLineMemberM*)GetCompoM(PCColumnMIDs[i]);
		if (pColumnM == NULL)	 continue;

		if (pColumnM->GetColumnHeaderMID() == headerMID)
			columnMArr.push_back(pColumnM);
	}

	return columnMArr;
}

int MSRebarBuilding::GetSectionViewCount()
{
	int count = 0;
	map<long, MSFloor*>::iterator it;
	for (it = mm_Floor.begin(); it != mm_Floor.end(); it++)
	{
		count += it->second->ma_SectionView.size();
	}
	return count;
}

void MSRebarBuilding::InitQuantityOptionMaterial()
{
	if (GetRcMaterialbyName(m_QuantityOption.m_strMaterial) != nullptr)
		return;

	for (auto it = mm_Material.begin(); it != mm_Material.end(); it++)
	{
		if (it->second->GetType() == MSMaterial::msConcrete)
		{
			m_QuantityOption.m_strMaterial = it->second->GetName();
			return;
		}
	}
}

MSCustomSection* MSRebarBuilding::CreateNewCustomSection(int sectionType)
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

void MSRebarBuilding::DeleteCustomSections()
{
	for (auto it = mm_CustomSection.begin(); it != mm_CustomSection.end(); it++)
		delete it->second;
	mm_CustomSection.clear();
}

MSCustomSection* MSRebarBuilding::GetCustomSection(long ID)
{
	auto it = mm_CustomSection.find(ID);
	if (it == mm_CustomSection.end())
		return NULL;

	return it->second;

}

bool MSRebarBuilding::Add(MSCustomSection* pCustomSection)
{
	auto bPair = mm_CustomSection.insert(make_pair(pCustomSection->m_ID, pCustomSection));
	return bPair.second;

}

long MSRebarBuilding::Remove(MSCustomSection* pCustomSection)
{
	long nID = pCustomSection->m_ID;
	auto it = mm_CustomSection.find(nID);
	if (it == mm_CustomSection.end())	return 0;
	mm_CustomSection.erase(it);
	return nID;

}

long MSRebarBuilding::Delete(MSCustomSection* pCustomSection)
{
	long nID = Remove(pCustomSection);
	delete pCustomSection;
	return nID;

}

bool MSRebarBuilding::IsCustomCompoM(MSCompoM* compoM)
{
	MSCompoM::Type_MSCompoM type = compoM->GetType();
	if (type == MSCompoM::msCustomColumnM || type == MSCompoM::msCustomWallColumnM || type == MSCompoM::msCustomBeamM || type == MSCompoM::msCustomButtressM)
		return true;
	return false;
}
bool MSRebarBuilding::IsUsedCustomSection(int sectionID)
{
	for (auto it = mm_CompoM.begin(); it != mm_CompoM.end(); it++)
	{
		MSCompoM* pCompoM = it->second;
		if (!IsCustomCompoM(pCompoM))	continue;

		MSCustomM* pCustomM = (MSCustomM*)pCompoM;
		MSCustomSection* pSection = pCustomM->GetMSCustomSection();
		if (pSection == nullptr)	continue;
		if (pSection->m_ID == sectionID)	return true;

	}
	return false;
}

vector<MSCustomM*> MSRebarBuilding::GetCompoMsHasCustomSectionID(int sectionID)
{
	vector<MSCustomM*> customMs;
	for (auto it = mm_CompoM.begin(); it != mm_CompoM.end(); it++)
	{
		MSCompoM* pCompoM = it->second;
		if (!IsCustomCompoM(pCompoM))	continue;

		MSCustomM* pCustomM = (MSCustomM*)pCompoM;
		MSCustomSection* pSection = pCustomM->GetMSCustomSection();
		if (pSection == nullptr)	continue;
		if (pSection->m_ID == sectionID)	customMs.push_back(pCustomM);
	}
	return customMs;
}

BaseRebarTemplate* MSRebarBuilding::GetRebarTemplate(RT_MemberType type)
{
	int nRebarTemplateID = GetRebarTemplateID();

	if (nRebarTemplateID == -1)
		m_nRebarTemplateID = RebarTemplateManager::GetInstance()->GetBuildingTemplateID();

	BaseRebarTemplate* rebarTemplate = RebarTemplateManager::GetInstance()->GetRebarTemplate(type, m_nRebarTemplateID);

	return rebarTemplate;
}

CString MSRebarBuilding::GetRebarTemplateName()
{
	int nRebarTemplateID = GetRebarTemplateID();

	if (nRebarTemplateID == -1)
		m_nRebarTemplateID = RebarTemplateManager::GetInstance()->GetBuildingTemplateID();

	CString sRebarTemplaeName = RebarTemplateManager::GetInstance()->GetNoneRebarTemplateName();

	RebarTemplate* pRebarTemplate = RebarTemplateManager::GetInstance()->GetRebarTemplateByID(m_nRebarTemplateID);
	if (pRebarTemplate != nullptr)
		sRebarTemplaeName = pRebarTemplate->m_sName;

	return sRebarTemplaeName;
}

void MSRebarBuilding::SetRebarTemplateName(CString sName)
{
	RebarTemplate* pRebarTemplate = RebarTemplateManager::GetInstance()->GetRebarTemplateByName(sName);
	if (pRebarTemplate == nullptr)
		return;

	RebarTemplateManager::GetInstance()->SetBuildingTemplateID(pRebarTemplate->m_nID);
	m_nRebarTemplateID = pRebarTemplate->m_nID;
}

int MSRebarBuilding::GetRebarTemplateID()
{
	RebarTemplate* rebarTemplate = RebarTemplateManager::GetInstance()->GetRebarTemplateByID(m_nRebarTemplateID);
	if (rebarTemplate == nullptr)
		m_nRebarTemplateID = -1;

	return m_nRebarTemplateID;
}

FormWorkTemplate* MSRebarBuilding::GetFormWorkTemplate()
{
	int nFormWorkTemplateID = GetFormWorkTemplateID();

	if (nFormWorkTemplateID == -1)
		m_nFormWorkTemplateID = FormWorkTemplateManager::GetInstance()->GetBuildingTemplateID();

	FormWorkTemplate* formWorkTemplate = FormWorkTemplateManager::GetInstance()->GetFormWorkTemplateByID(m_nFormWorkTemplateID);

	return formWorkTemplate;
}

CString MSRebarBuilding::GetFormWorkTemplateName()
{
	int nFormWorkTemplateID = GetFormWorkTemplateID();

	if (nFormWorkTemplateID == -1)
		m_nFormWorkTemplateID = FormWorkTemplateManager::GetInstance()->GetBuildingTemplateID();

	FormWorkTemplate* pFormTemplate = FormWorkTemplateManager::GetInstance()->GetFormWorkTemplateByID(m_nFormWorkTemplateID);

	CString sFormTemplaeName = FormWorkTemplateManager::GetInstance()->GetNoneFormWorkTemplateName();

	if (pFormTemplate != nullptr)
		sFormTemplaeName = pFormTemplate->m_sName;

	return sFormTemplaeName;
}
void MSRebarBuilding::SetFormWorkTemplateByName(CString sName)
{
	FormWorkTemplate* pFormTemplate = FormWorkTemplateManager::GetInstance()->GetFormWorkTemplateByName(sName);
	FormWorkTemplateManager::GetInstance()->SetBuildingTemplateID(pFormTemplate->m_nID);
	m_nFormWorkTemplateID = pFormTemplate->m_nID;
}
int MSRebarBuilding::GetFormWorkTemplateID()
{
	FormWorkTemplate* formTemplate = FormWorkTemplateManager::GetInstance()->GetFormWorkTemplateByID(m_nFormWorkTemplateID);
	if (formTemplate == nullptr)
		m_nFormWorkTemplateID = -1;

	return m_nFormWorkTemplateID;
}

CString MSRebarBuilding::GetNayukTemplateName()
{
	int nNayukTemplateID = GetNayukTemplateID();

	if (nNayukTemplateID == -1)
		m_nNayukTemplateID = NayukTemplateManager::GetInstance()->GetBuildingTemplateID();

	NayukTemplate* pFormTemplate = NayukTemplateManager::GetInstance()->GetNayukTemplateByID(m_nNayukTemplateID);

	CString sFormTemplaeName = _T("");

	if (pFormTemplate != nullptr)
		sFormTemplaeName = pFormTemplate->m_sName;

	return sFormTemplaeName;
}
void MSRebarBuilding::SetNayukTemplateByName(CString sName)
{
	NayukTemplate* pFormTemplate = NayukTemplateManager::GetInstance()->GetNayukTemplateByName(sName);
	NayukTemplateManager::GetInstance()->SetBuildingTemplateID(pFormTemplate->m_nID);
	m_nNayukTemplateID = pFormTemplate->m_nID;
}
int MSRebarBuilding::GetNayukTemplateID()
{
	NayukTemplate* nayukTemplate = NayukTemplateManager::GetInstance()->GetNayukTemplateByID(m_nNayukTemplateID);
	if (nayukTemplate == nullptr)
		m_nNayukTemplateID = -1;

	return m_nNayukTemplateID;
}

void MSRebarBuilding::ClearRegenList()
{
	MSFloor::CompoGIDList_NeedRegen().clear();
	MSFloor::CompoGIDList_NeedRegen_CurTR().clear();
	MSFloor::CompoGIDList_NeedRegen_2DView().clear();
	MSFloor::CompoGIDList_NeedRegen_2DView_CurTR().clear();
}

vector<int> MSRebarBuilding::GetAllWallGsWidthRebarLineID(int rebarLineID)
{
	vector<int> list;
	map<long, MSCompoG*>::iterator itMemberG;
	map<long, MSFloor*>::iterator itFloor;
	for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->second;
		for (itMemberG = pFloor->mm_CompoG.begin(); itMemberG != pFloor->mm_CompoG.end(); itMemberG++)
		{
			MSCompoG* pCompoG = itMemberG->second;
			if (pCompoG == nullptr)	continue;
			if (pCompoG->GetType() != MSElement::msWallG)	continue;

			MSWallMemberG* pWallG = dynamic_cast<MSWallMemberG*>(pCompoG);
			if (pWallG == nullptr)	continue;

			if (pWallG->GetStartEndRebarLineID() == rebarLineID || pWallG->GetEndEndRebarLineID() == rebarLineID)
				list.push_back(pWallG->m_ID);
		}
	}
	return list;
}

void MSRebarBuilding::WriteFile_4KangjiHawan()
{
	int floorNum = -3;
	MSFloor* floor = GetFloor(floorNum);
	if (floor == nullptr)	return;

	vector<MSSlabG*> slabGArr;
	vector<MSIsoFootG*> isoFootGArr;
	for (auto it = floor->mm_CompoG.begin(); it != floor->mm_CompoG.end(); it++)
	{
		MSCompoG* compoG = it->second;
		if (compoG->GetType() == MSElement::msSlabG)
		{
			MSRcSlabM* pRcSlabM = (MSRcSlabM*)compoG->mp_CompoM->GetRebarM();
			if (pRcSlabM->GetSlabMType() == MSRcSlabM::MAT_TYPE)
				slabGArr.push_back((MSSlabG*)compoG);
		}
		else if (compoG->GetType() == MSElement::msIsoFootG)
		{
			isoFootGArr.push_back((MSIsoFootG*)compoG);
		}
	}

	CString filePath = _T("D:\\sample.txt");
	CStdioFile file(filePath, CFile::modeCreate | CFile::modeWrite);

	file.WriteString(_T("MAT SLAB\n"));
	file.WriteString(_T("ID		Offset		Depth\n"));
	CString data;
	for (int i = 0; i < slabGArr.size(); i++)
	{
		MSSlabG* slabG = slabGArr[i];
		data.Format(_T("%ld,		%ld,		%ld\n"), slabG->m_ID, ROUND(slabG->m_dOffset), ROUND(slabG->GetThick()));
		file.WriteString(data);
	}

	file.WriteString(_T("\n"));
	file.WriteString(_T("\n"));
	file.WriteString(_T("IsoFooT\n"));
	file.WriteString(_T("ID		Offset		Depth\n"));

	for (int i = 0; i < isoFootGArr.size(); i++)
	{
		MSIsoFootG* isoFootG = isoFootGArr[i];
		data.Format(_T("%ld,		%ld,		%ld\n"), isoFootG->m_ID, ROUND(isoFootG->m_SOffsetZ), ROUND(isoFootG->GetThick()));
		file.WriteString(data);
	}

	file.Close();

}

MSConstructionZone* MSRebarBuilding::GetCZone(int ID)
{
	map<long, MSFloor*>::iterator itFloor;
	for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->second;
		if (pFloor == nullptr) continue;
		MSConstructionZone* pCzone = pFloor->GetCZone(ID);
		if (pCzone != nullptr)
		{
			return pCzone;
		}
	}
	return nullptr;
}

GM2DVector MSRebarBuilding::GetUCSOriginPoint()
{
	return mp_UCS->GetOriginPoint();
}
GM2DVector MSRebarBuilding::GetUCSXDir()
{
	return mp_UCS->GetXDir();
}
GM2DVector MSRebarBuilding::GetUCSYDir()
{
	return mp_UCS->GetYDir();
}
GM2DVector MSRebarBuilding::GetImportBldgUCSOriginPoint()
{
	return mp_UCS_import->GetOriginPoint();
}
GM2DVector MSRebarBuilding::GetImportBldgUCSXDir()
{
	return mp_UCS_import->GetXDir();
}
GM2DVector MSRebarBuilding::GetImportBldgUCSYDir()
{
	return mp_UCS_import->GetYDir();
}
double MSRebarBuilding::GetUCSz()
{
	return mp_UCS->GetUCSz();
}
void MSRebarBuilding::SetBuildingUCS(GM2DVector originPoint, GM2DVector xDir, GM2DVector yDir)
{
	mp_UCS->SetUCS(originPoint, xDir, yDir);
}
void MSRebarBuilding::SetBuildingUCSz(double z)
{
	mp_UCS->SetUCSz(z);
}
void MSRebarBuilding::SetImportBuildingUCS(GM2DVector originPoint, GM2DVector xDir, GM2DVector yDir)
{
	mp_UCS_import->SetUCS(originPoint, xDir, yDir);
}

vector<long> MSRebarBuilding::GetDummyCompoGIDList()
{
	return DummyCompoGIDList;
}

void MSRebarBuilding::AddDummyCompoGID(long dummyCompoGID)
{
	DummyCompoGIDList.push_back(dummyCompoGID);
}

void MSRebarBuilding::DummyCompoGIDListClear()
{
	DummyCompoGIDList.clear();
}

vector<MSCompoM*> MSRebarBuilding::GetCompoMList(long nType)
{
	vector<MSCompoM*> compoMList;

	map<long, MSCompoM*>::iterator it;
	for (it = mm_CompoM.begin(); it != mm_CompoM.end(); ++it)
	{
		MSCompoM* pCompoM = it->second;
		if (pCompoM->GetType() != nType) continue;

		compoMList.push_back(pCompoM);
	}

	return compoMList;
}

void MSRebarBuilding::TotalColumnMNameDuplicateCheckAndNameModify()
{
	vector<MSCompoM*> columnMList = GetCompoMList((int)MSCompoM::msColumnM);
	vector<MSCompoM*> buttressColumnMList = GetCompoMList((int)MSCompoM::msButtressM);
	vector<MSCompoM*> pcColumnMList = GetCompoMList((int)MSCompoM::msPcColumnM);
	vector<MSCompoM*> srcColumnMList = GetCompoMList((int)MSCompoM::msSrcColumnM);
	vector<MSCompoM*> steelColumnMList = GetCompoMList((int)MSCompoM::msSteelColumnM);

	for (int BTIndex = 0; BTIndex < buttressColumnMList.size(); BTIndex++)
	{
		MSCompoM* SrcCompoM = buttressColumnMList[BTIndex];

		ColumnMNameDuplicateCheckAndNameModify(SrcCompoM, columnMList);
	}

	for (int PCIndex = 0; PCIndex < pcColumnMList.size(); PCIndex++)
	{
		MSCompoM* SrcCompoM = pcColumnMList[PCIndex];

		ColumnMNameDuplicateCheckAndNameModify(SrcCompoM, columnMList);
		ColumnMNameDuplicateCheckAndNameModify(SrcCompoM, buttressColumnMList);
	}

	for (int SRCIndex = 0; SRCIndex < srcColumnMList.size(); SRCIndex++)
	{
		MSCompoM* SrcCompoM = srcColumnMList[SRCIndex];

		ColumnMNameDuplicateCheckAndNameModify(SrcCompoM, columnMList);
		ColumnMNameDuplicateCheckAndNameModify(SrcCompoM, buttressColumnMList);
		ColumnMNameDuplicateCheckAndNameModify(SrcCompoM, pcColumnMList);
	}

	for (int SteelIndex = 0; SteelIndex < steelColumnMList.size(); SteelIndex++)
	{
		MSCompoM* SrcCompoM = steelColumnMList[SteelIndex];

		ColumnMNameDuplicateCheckAndNameModify(SrcCompoM, columnMList);
		ColumnMNameDuplicateCheckAndNameModify(SrcCompoM, buttressColumnMList);
		ColumnMNameDuplicateCheckAndNameModify(SrcCompoM, pcColumnMList);
		ColumnMNameDuplicateCheckAndNameModify(SrcCompoM, srcColumnMList);
	}
}

void MSRebarBuilding::ColumnMNameDuplicateCheckAndNameModify(MSCompoM* SrcCompoM, vector<MSCompoM*> desColumnMList)
{
	for (int index = 0; index < desColumnMList.size(); index++)
	{
		MSCompoM* DesCompoM = desColumnMList[index];

		if (VerticalMNameAndStoryDuplicateCheck(SrcCompoM, DesCompoM, true) == false) continue;

		ColumnNameModify(SrcCompoM);
	}
}

void MSRebarBuilding::ColumnNameModify(MSCompoM* SrcCompoM)
{
	CString sNewName;
	sNewName.Format(_T("%s_%d"), SrcCompoM->GetName(), SrcCompoM->m_ID);
	SrcCompoM->SetName(sNewName);
}

void MSRebarBuilding::AddNote(CString sNote)
{
	if (NoteExistCheck(sNote) == true) return;

	int nNewNoteID = NewNoteID();
	mm_Note.insert(make_pair(nNewNoteID, sNote));
}

bool MSRebarBuilding::NoteExistCheck(CString sNote)
{
	map<int, CString>::iterator itNote;
	for (itNote = mm_Note.begin(); itNote != mm_Note.end(); itNote++)
	{
		if (itNote->second != sNote) continue;

		return true;
	}

	return false;
}

void MSRebarBuilding::DelNote(int nNoteID)
{
	map<int, CString>::iterator itNote = mm_Note.find(nNoteID);
	if (itNote != mm_Note.end())
		mm_Note.erase(nNoteID);

	return;
}

void MSRebarBuilding::EditNote(CString sOldNote, CString sEditNote)
{
	map<int, CString>::iterator itNote;
	for (itNote = mm_Note.begin(); itNote != mm_Note.end(); itNote++)
	{
		if (itNote->second != sOldNote) continue;

		itNote->second = sEditNote;
	}
}

int MSRebarBuilding::NewNoteID()
{
	for (int index = 0; index < mm_Note.size(); index++)
	{
		map<int, CString>::iterator itNote = mm_Note.find(index + 1);
		if (itNote != mm_Note.end()) continue;

		return index + 1;
	}

	return mm_Note.size() + 1;
}

void MSRebarBuilding::NoteClear()
{
	mm_Note.clear();
}

int MSRebarBuilding::GetBuildingInfoID()
{
	MSBuildingInfo* buildingInfo = m_pBuildingInfoManager->GetBuildingInfoByID(m_nBuildingInfoID);
	if (buildingInfo == nullptr)
	{
		buildingInfo = m_pBuildingInfoManager->GetDefaultBuildingInfo();
		m_nBuildingInfoID = buildingInfo->m_ID;
	}

	return m_nBuildingInfoID;
}

MSBuildingInfo* MSRebarBuilding::GetBuildingInfo()
{
	int nBuildingInfoID = GetBuildingInfoID();

	MSBuildingInfo* buildingInfo = m_pBuildingInfoManager->GetBuildingInfoByID(nBuildingInfoID);
	return buildingInfo;
}

CString MSRebarBuilding::GetBuildingInfoType()
{
	MSBuildingInfo* pCurBuildingInfo = GetBuildingInfo();
	if (pCurBuildingInfo == nullptr)
		return _T("");

	return pCurBuildingInfo->GetBuildingInfoType();
}

vector<CString> MSRebarBuilding::GetBuildingInfoTypeList()
{
	return m_pBuildingInfoManager->GetBuildingTypeList();
}


// Serialize 에서만 사용해야한다.
void MSRebarBuilding::CheckFloorQT()
{
	GM2DBound PolyBound;
	bool isUpdate = false;

	if (m_SiteRect.m_Origin.m_X < INT_MIN || m_SiteRect.m_Origin.m_Y < INT_MIN ||
		m_SiteRect.m_Corner.m_X > INT_MAX || m_SiteRect.m_Corner.m_Y > INT_MAX)
		isUpdate = true;
	else
	{
		for (auto itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
		{
			MSFloor* pFloor = itFloor->second;
			MSFloorQT* pFloorQT = pFloor->mp_QT;
			if (pFloorQT->m_Rect.m_Origin.m_X < INT_MIN || pFloorQT->m_Rect.m_Origin.m_Y < INT_MIN ||
				pFloorQT->m_Rect.m_Corner.m_X > INT_MAX || pFloorQT->m_Rect.m_Corner.m_Y > INT_MAX)
			{
				isUpdate = true;
				break;
			}
		}
	}

	if (isUpdate == false)
		return;

	vector<MSCompoG*> compogArr;
	GM2DVector* pSVec = NULL;
	GM2DVector* pEVec = NULL;

	for (auto itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->second;

		for (auto itr = pFloor->mm_CompoG.begin(); itr != pFloor->mm_CompoG.end(); ++itr)
		{
			MSCompoG* pCompoG = itr->second;
			compogArr.push_back(pCompoG);
			if (pCompoG->IsPolyMember())
			{
				GM2DPolyline* pPoly = pCompoG->GetProfile();

				for (int iVec = 0; iVec < pPoly->ma_Point.GetSize(); iVec++)
				{
					pSVec = pPoly->ma_Point[iVec];
					if (pSVec != nullptr)
						PolyBound.AddVector(pSVec);
				}
			}
			else
			{
				pSVec = pCompoG->GetSVec();
				pEVec = pCompoG->GetEVec();
				if (pSVec != nullptr)
					PolyBound.AddVector(pSVec);
				if (pEVec != nullptr)
					PolyBound.AddVector(pEVec);
			}
		}
	}

	if (compogArr.size() == 0)
		return;

	GM2DVector OriginVec = *PolyBound.GetBottomLeft();
	GM2DVector CornerVec = *PolyBound.GetTopRight();

	double dMaxX = 0;
	double dMaxY = 0;
	double dMinX = 0;
	double dMinY = 0;
	double dMagin = 5000;

	dMinX = OriginVec.m_X - dMagin;
	dMinY = OriginVec.m_Y - dMagin;
	dMaxX = CornerVec.m_X + dMagin;
	dMaxY = CornerVec.m_Y + dMagin;

	m_SiteRect.m_Origin.SetXY(dMinX, dMinY);
	m_SiteRect.m_Corner.SetXY(dMaxX, dMaxY);

	for (auto itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->second;
		pFloor->mp_QT->m_Rect.m_Origin.SetXY(dMinX, dMinY);
		pFloor->mp_QT->m_Rect.m_Corner.SetXY(dMaxX, dMaxY);
		pFloor->m_SiteRect.m_Origin.SetXY(dMinX, dMinY);
		pFloor->m_SiteRect.m_Corner.SetXY(dMaxX, dMaxY);
	}

}

void MSRebarBuilding::makeVerticalConstructionZones()
{
	for (auto itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->second;

		if (pFloor->GetLowerFloor() == nullptr) continue;

		for (auto itCZone = pFloor->mm_CZone.begin(); itCZone != pFloor->mm_CZone.end(); itCZone++)
		{
			MSConstructionZone* HCZone = itCZone->second;
			if (HCZone->GetZoneType() != MSConstructionZone::Horizontal)	continue;
			if (HCZone->m_bDefaultCZone)	continue;	// MSFloor::Serizlize에서 예외처리 되어 있음
			MSConstructionZone* VCZone = dynamic_cast<MSConstructionZone*>(HCZone->Copy());
			if (VCZone == nullptr)	continue;
			VCZone->SetZoneType(MSConstructionZone::Vertical);
			pFloor->AddCZone(VCZone);
		}
		pFloor->AddDefaultCZone();
	}
}

void MSRebarBuilding::WriteLogInfo()
{
	long UnderStory = -(GetNumUnderStory() - 1);
	long AboveStory = GetNumAboveStory();

	CString msg;
	msg.Format(L"[MSRebarBuilding Info] \tBuilding_Name:%s \tMSMaterial_Count:%d \tMSCompoM_Count:%d \tFloor_Count:%d", m_Name, mm_Material.size(), mm_CompoM.size(), mm_Floor.size());
	LoggerWrapper::Log(msg);
	msg.Format(L"[MSRebarBuilding Info] \tUnderStory:%d \tAboveStory:%d", UnderStory, AboveStory);
	LoggerWrapper::Log(msg);

	map<long, MSFloor*>::iterator itFloor;
	for (itFloor = mm_Floor.begin(); itFloor != mm_Floor.end(); itFloor++)
	{
		MSFloor* pFloor = itFloor->second;
		pFloor->WriteLogInfo();
	}
}

MSCompoM *MSRebarBuilding::GetDefaultCompoM(long compoMType)
{
	auto Iter = mm_DefaultCompoM.find(compoMType);
	if (Iter != mm_DefaultCompoM.end())
		return Iter->second;

	return nullptr;
}

void MSRebarBuilding::SetDefaultCompoM(MSCompoM* defCompoM)
{
	auto Iter = mm_DefaultCompoM.find(defCompoM->GetType());
	if (Iter == mm_DefaultCompoM.end())
		mm_DefaultCompoM.insert(make_pair(defCompoM->GetType(), defCompoM));
	else
		Iter->second = defCompoM;
}
void MSRebarBuilding::RemoveDefaultCompoM(long compoMType)
{
	auto Iter = mm_DefaultCompoM.find(compoMType);
	if (Iter != mm_DefaultCompoM.end())
		mm_DefaultCompoM.erase(Iter);
}
BOOL MSRebarBuilding::GetTopDownPlace()
{
	return m_bTopDownPlace;
}
void MSRebarBuilding::SetTopDownPlace(BOOL value)
{
	m_bTopDownPlace = value;
}
long MSRebarBuilding::GetTopDownStartFloor()
{
	return m_nTopDownStartFloor;
}
void MSRebarBuilding::SetTopDownStartFloor(long value)
{
	m_nTopDownStartFloor = value;
}

map<long, MSMaterial*>* MSRebarBuilding::GetMaterialMap()
{
	return &mm_Material;
}

#pragma region A24-522 단면뷰 참조선 기능 개선
CString MSRebarBuilding::GetSectionViewNameByCopyToPaste(CString name)
{
	CString newName = name;
	vector<CString> allNames;
	for (auto it = mm_Floor.begin(); it != mm_Floor.end(); it++)
	{
		auto pFloor = it->second;
		for (int i = 0; i < pFloor->ma_SectionView.size(); i++)
		{
			auto pSectionView = pFloor->ma_SectionView[i];
			allNames.push_back(pSectionView->m_Name);
		}
	}

	// 중복된 이름이 있는지 확인
	if (std::find(allNames.begin(), allNames.end(), newName) != allNames.end()) {
		int next = 0;
		while (true) {
			next++;
			newName.Format(_T("%s-%d"), name.GetString(), next);  // 새로운 이름 생성
			if (std::find(allNames.begin(), allNames.end(), newName) == allNames.end()) {  // 중복이 아니면 탈출
				break;
			}
		}
	}

	return newName;  // 중복이 없는 고유한 이름 반환
}

#pragma endregion
