#include "StdAfx.h"
#include "OldBuildingRebarTemplate.h"
#include "..\..\GMLib\MSVersionInfo.h"
#include "..\MSDeformedBar.h"
#include "OldRebarTemplateManager.h"
#include "..\ColumnRebarTemplate.h"

IMPLEMENT_SERIAL(OldBuildingRebarTemplate, OldCommonRebarTemplate, VERSIONABLE_SCHEMA | 1)
OldBuildingRebarTemplate::OldBuildingRebarTemplate() : OldCommonRebarTemplate()
{
	DataInit();
}

OldBuildingRebarTemplate::~OldBuildingRebarTemplate(void)
{
}

void OldBuildingRebarTemplate::Dump(EFS::EFS_Buffer& buffer)
{
	OldCommonRebarTemplate::Dump(buffer);
	buffer << m_sName;
}

void OldBuildingRebarTemplate::Recovery(EFS::EFS_Buffer& buffer)
{
	OldCommonRebarTemplate::Recovery(buffer);
	buffer >> m_sName;
}

void OldBuildingRebarTemplate::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		ar << m_sName;
		ar << m_nID;

		m_ColumnRebarTemplate->Serialize(ar);
		m_BeamRebarTemplate->Serialize(ar);
		m_BWallRebarTemplate->Serialize(ar);
		m_RWallRebarTemplate->Serialize(ar);
		m_PitRebarTemplate->Serialize(ar);
		m_SlabRebarTemplate->Serialize(ar);
		m_IsoFootRebarTemplate->Serialize(ar);
		m_WallFootRebarTemplate->Serialize(ar);
	}
	else
	{
		ar >> m_sName;
		ar >> m_nID;

		m_ColumnRebarTemplate = (OldColumnRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldColumn);
		m_ColumnRebarTemplate->Serialize(ar);

		m_BeamRebarTemplate = (OldBeamRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldBeam);
		m_BeamRebarTemplate->Serialize(ar);

		m_BWallRebarTemplate = (OldBWallRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldBWall);
		m_BWallRebarTemplate->Serialize(ar);

		m_RWallRebarTemplate = (OldRWallRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldRWall);;
		m_RWallRebarTemplate->Serialize(ar);

		m_PitRebarTemplate = (OldPitRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldPit);
		m_PitRebarTemplate->Serialize(ar);

		m_SlabRebarTemplate = (OldSlabRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldSlab);
		m_SlabRebarTemplate->Serialize(ar);

		m_IsoFootRebarTemplate = (OldIsoFootRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldIsoFoot);
		m_WallFootRebarTemplate = (OldWallFootRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldWallFoot);

		if (MSVersionInfo::GetCurrentVersion() >= 20170621)
		{
			m_IsoFootRebarTemplate->Serialize(ar);
			m_WallFootRebarTemplate->Serialize(ar);
		}

		OldRebarTemplateManager::SetLastID(m_nID);
	}
}

void OldBuildingRebarTemplate::SerializeReadForBefore20160122(CArchive &ar, int firstData)
{
	int NumObject;
	BOOL boolValue, boolDummyValue;
	long longValue, longDummyValue;
	double doubleValue, doubleDummyValue;

	m_ColumnRebarTemplate = (OldColumnRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldColumn);
	m_BeamRebarTemplate = (OldBeamRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldBeam);
	m_BWallRebarTemplate = (OldBWallRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldBWall);
	m_RWallRebarTemplate = (OldRWallRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldRWall);
	m_PitRebarTemplate = (OldPitRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldPit);
	m_SlabRebarTemplate = (OldSlabRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldSlab);

	//기둥
	boolValue = firstData;	m_ColumnRebarTemplate->SetColumnConnectLengthSet(boolValue);
	ar >> longValue;	m_ColumnRebarTemplate->SetColumnConnectLength(longValue);

	ar >> boolValue;	m_ColumnRebarTemplate->SetColumnHoopBarStartLengthSet(boolValue);
	ar >> longValue;	m_ColumnRebarTemplate->SetColumnHoopBarStartLength(longValue);
	ar >> boolValue;	m_ColumnRebarTemplate->SetColumnTieAddLengthSet(boolValue);
	ar >> longValue;	m_ColumnRebarTemplate->SetColumnTieAddLength(longValue);
	ar >> longValue;	m_ColumnRebarTemplate->SetColumnHoopBarShapeIdx(longValue);
	ar >> longValue;	m_ColumnRebarTemplate->SetColumnTieBarShapeIdx(longValue);

	ar >> longValue;	m_ColumnRebarTemplate->SetColumnVerBarTopLenType(longValue);

	ar >> boolValue;	m_ColumnRebarTemplate->SetColumnDowelFootingCoverSet(boolValue);
	ar >> longValue;	m_ColumnRebarTemplate->SetColumnDowelFootingCover(longValue);
	ar >> longValue;	m_ColumnRebarTemplate->SetColumnDowelBotPlaceType(longValue);
	ar >> longValue;	m_ColumnRebarTemplate->SetColumnDowelTopPlaceType(longValue);
	ar >> longValue;	m_ColumnRebarTemplate->SetColumnDowelHoopRebarType(longValue);
	ar >> longValue;	m_ColumnRebarTemplate->SetColumnDowelHoopBySpace(longValue);
	ar >> longValue;	m_ColumnRebarTemplate->SetColumnDowelHoopByNum(longValue);
	ar >> longValue;	m_ColumnRebarTemplate->SetColumnDowelSupportBarDiaIndex(longValue);
	ar >> longValue;	m_ColumnRebarTemplate->SetColumnDowelSupportBarLength(longValue);
	ar >> longValue;	m_ColumnRebarTemplate->SetColumnDowelSupportBarNum(longValue);
	
	map<long, long> mapColumnUserDowelSpliceLen;
	mapColumnUserDowelSpliceLen.clear();
	ar >> NumObject;
	for (int index = 0; index < NumObject; index++)
	{
		long nDiaIndex = 0, spliceLen = 0;

		ar >> nDiaIndex;
		ar >> spliceLen;
		mapColumnUserDowelSpliceLen.insert(make_pair(nDiaIndex, spliceLen));
	}

	//보
	ar >> longValue;	m_BeamRebarTemplate->SetBeamTopMaxLen2HookAnchorBar(longValue);
	ar >> longValue;	m_BeamRebarTemplate->SetBeamTopSpliceType(longValue);

	ar >> longValue;	m_BeamRebarTemplate->SetBeamShearBarType(longValue);
	ar >> boolDummyValue; //m_bBeamUserStirrupShape;
	ar >> longValue;	m_BeamRebarTemplate->SetBeamStirrupType(longValue);
	ar >> longValue;	m_BeamRebarTemplate->SetBeamCoverBarType(longValue);

	//벽
	ar >> boolDummyValue; //m_bBWallArcWallDeg;

	ar >> boolValue;	m_BWallRebarTemplate->SetBWallPlaceEndBarSet(boolValue);
	ar >> longValue;	m_BWallRebarTemplate->SetBWallEndVerAddBarDist(longValue);
	ar >> longValue;	m_BWallRebarTemplate->SetBWallVerBarTopLenType(longValue);
	ar >> longValue;	m_BWallRebarTemplate->SetBWallCrossBarEAType(longValue);

	ar >> boolValue;	m_BWallRebarTemplate->SetBWallPlaceUBar4ShortWall(boolValue);
	ar >> longValue;	m_BWallRebarTemplate->SetBWallMaxUbarLegLength(longValue);
	ar >> boolValue;	m_BWallRebarTemplate->SetBWallPlaceStirrup4ShortWall(boolValue);
	ar >> longValue;	m_BWallRebarTemplate->SetBWallHBarMaxStirrupLen(longValue);
	ar >> boolValue;	m_BWallRebarTemplate->SetBWallHorBarStartSet(boolValue);
	ar >> longValue;	m_BWallRebarTemplate->SetBWallHorBarStartLength(longValue);

	ar >> longValue;	
	ar >> longValue;	
	ar >> longValue;	
	ar >> longValue;	
	ar >> longValue;	
	ar >> longValue;	
	ar >> longValue;	
	ar >> longValue;	
	ar >> boolValue;	m_BWallRebarTemplate->SetBWallDefaultUbarLen(boolValue);

	ar >> boolValue;	m_BWallRebarTemplate->SetBWallUserDowelHookLen(boolValue);
	ar >> longValue;	m_BWallRebarTemplate->SetBWallDowelBarAnchorType(longValue);
	ar >> longValue;	m_BWallRebarTemplate->SetBWallDowelBarTopLenType(longValue);
	ar >> longValue;	m_BWallRebarTemplate->SetBWallDowelSupportBarDiaIndex(longValue);
	ar >> longValue;	m_BWallRebarTemplate->SetBWallDowelSupportBarNum(longValue);
	ar >> boolValue;	m_BWallRebarTemplate->SetBWallDowelSupportPlaceType(boolValue);

	map<long, long> mapBWallAddBarDiaIndex;
	mapBWallAddBarDiaIndex.clear();
	ar >> NumObject;
	for (int index = 0; index < NumObject; index++)
	{
		long nDiaIndex1 = 0, nDiaIndex2 = 0;

		ar >> nDiaIndex1;
		ar >> nDiaIndex2;
		mapBWallAddBarDiaIndex.insert(make_pair(nDiaIndex1, nDiaIndex2));
	}

	map<long, long> mapBWallDefaultUbarLen;
	mapBWallDefaultUbarLen.clear();
	ar >> NumObject;
	for (int index = 0; index < NumObject; index++)
	{
		long nDiaIndex = 0, uBarLen = 0;

		ar >> nDiaIndex;
		ar >> uBarLen;
		mapBWallDefaultUbarLen.insert(make_pair(nDiaIndex, uBarLen));
	}

	map<long, long> mapBWallUserDowelHookLen;
	mapBWallUserDowelHookLen.clear();
	ar >> NumObject;
	for (int index = 0; index < NumObject; index++)
	{
		long nDiaIndex = 0, hookLen = 0;

		ar >> nDiaIndex;
		ar >> hookLen;
		mapBWallUserDowelHookLen.insert(make_pair(nDiaIndex, hookLen));
	}

	map<long, long> mapBWallUserDowelSpliceLen;
	mapBWallUserDowelSpliceLen.clear();
	ar >> NumObject;
	for (int index = 0; index < NumObject; index++)
	{
		long nDiaIndex = 0, spliceLen = 0;

		ar >> nDiaIndex;
		ar >> spliceLen;
		mapBWallUserDowelSpliceLen.insert(make_pair(nDiaIndex, spliceLen));
	}

	//지하외벽
	ar >> longDummyValue; //m_nRWallArcWallDeg;

	ar >> boolValue;	m_RWallRebarTemplate->SetRWallHorBarStartSet(boolValue);
	ar >> longValue;	m_RWallRebarTemplate->SetRWallHorBarStartLength(longValue);

	ar >> longValue;	
	ar >> longValue;	
	ar >> longValue;	
	ar >> longValue;	
	ar >> longValue;	m_RWallRebarTemplate->SetRWallEndUbar(longValue);
	ar >> boolValue;	m_RWallRebarTemplate->SetRWallDefaultUbarLen(boolValue);

	ar >> longValue;	m_RWallRebarTemplate->SetRWallVerBarTopLenType(longValue);

	ar >> boolValue;	m_RWallRebarTemplate->SetRWallUserDowelHookLen(boolValue);
	ar >> longValue;	m_RWallRebarTemplate->SetRWallDowelBarAnchorType(longValue);
	ar >> longValue;	m_RWallRebarTemplate->SetRWallDowelBarTopLenType(longValue);
	ar >> longValue;	m_RWallRebarTemplate->SetRWallDowelSupportBarDiaIndex(longValue);
	ar >> longValue;	m_RWallRebarTemplate->SetRWallDowelSupportBarNum(longValue);
	ar >> boolValue;	m_RWallRebarTemplate->SetRWallDowelSupportPlaceType(boolValue);

	map<long, long> mapRWallDefaultUbarLen;
	mapRWallDefaultUbarLen.clear();
	ar >> NumObject;
	for (int index = 0; index < NumObject; index++)
	{
		long nDiaIndex = 0, uBarLen = 0;

		ar >> nDiaIndex;
		ar >> uBarLen;
		mapRWallDefaultUbarLen.insert(make_pair(nDiaIndex, uBarLen));
	}

	map<long, long> mapRWallUserDowelHookLen;
	mapRWallUserDowelHookLen.clear();
	ar >> NumObject;
	for (int index = 0; index < NumObject; index++)
	{
		long nDiaIndex = 0, hookLen = 0;

		ar >> nDiaIndex;
		ar >> hookLen;
		mapRWallUserDowelHookLen.insert(make_pair(nDiaIndex, hookLen));
	}
	
	map<long, long> mapRWallUserDowelSpliceLen;
	mapRWallUserDowelSpliceLen.clear();
	ar >> NumObject;
	for (int index = 0; index < NumObject; index++)
	{
		long nDiaIndex = 0, spliceLen = 0;

		ar >> nDiaIndex;
		ar >> spliceLen;
		mapRWallUserDowelSpliceLen.insert(make_pair(nDiaIndex, spliceLen));
	}

	//PIT
	ar >> longValue;	m_PitRebarTemplate->SetPitMaxLen2HookAnchorBar(longValue);

	ar >> longValue;	m_PitRebarTemplate->SetPitVerOutTopHookType(longValue);
	ar >> longValue;	m_PitRebarTemplate->SetPitVerOutBotHookType(longValue);
	ar >> longValue;	m_PitRebarTemplate->SetPitVerInTopHookType(longValue);
	ar >> longValue;	m_PitRebarTemplate->SetPitVerInBotHookType(longValue);
	ar >> longValue;	m_PitRebarTemplate->SetPitHorOutHookType(longValue);
	ar >> longValue;	m_PitRebarTemplate->SetPitHorInHookType(longValue);

	//매트(기초)
	ar >> longValue;	

	ar >> longValue;	
	ar >> longValue;	

	ar >> longValue;	m_SlabRebarTemplate->SetMatTopEndHookType(longValue);
	ar >> longValue;	m_SlabRebarTemplate->SetMatBotEndHookType(longValue);

	ar >> longValue;	m_SlabRebarTemplate->SetMatIsoFootDevType(longValue);

	if (MSVersionInfo::GetCurrentVersion() >= 20150410)
	{
		ar >> longValue;	m_SlabRebarTemplate->SetMatRebarTolDist(longValue);
		ar >> longValue;	m_SlabRebarTemplate->SetMatRebarTolLenDif(longValue);
	}

	//슬래브
	ar >> longValue;	

	ar >> longValue;	m_SlabRebarTemplate->SetFlatOpeningTopHookType(longValue);
	ar >> longValue;	m_SlabRebarTemplate->SetFlatOpeningBotHookType(longValue);
	ar >> longValue;	m_SlabRebarTemplate->SetSlabTopStructuredEndHookType(longValue);
	ar >> longValue;	m_SlabRebarTemplate->SetSlabBotStructuredEndHookType(longValue);

	if (MSVersionInfo::GetCurrentVersion() >= 20140803)
	{
		ar >> doubleDummyValue; //m_dDeckEndRatio;
		ar >> longDummyValue; //m_nDeckConnectionBarLengthType;
		ar >> longDummyValue; //m_nDeckConnectionBarLength;

		ar >> longValue;	m_SlabRebarTemplate->SetEmbeddedDeckDifDiaConnectBarType(longValue);
		ar >> longValue;	m_SlabRebarTemplate->SetEmbeddedDeckAnchorType(longValue);
		ar >> longValue;	m_SlabRebarTemplate->SetEmbeddedDeckTopEndRatioType(longValue);
		ar >> longValue;	m_SlabRebarTemplate->SetEmbeddedDeckBotEndRatioType(longValue);
	}

	map<long, long> mapBeamSubStirrupHookLen;	mapBeamSubStirrupHookLen.clear();
	map<long, long> mapBeamShearBarHookLenA;	mapBeamShearBarHookLenA.clear();
	map<long, long> mapBeamShearBarHookLenB;	mapBeamShearBarHookLenB.clear();

	if (MSVersionInfo::GetCurrentVersion() >= 20140902)
	{
		ar >> longValue; m_BeamRebarTemplate->SetBeamTopSubStirrupBarShape(longValue);
		ar >> longValue; m_BeamRebarTemplate->SetBeamBotSubStirrupBarShape(longValue);
		ar >> longValue; m_BeamRebarTemplate->SetBeamSubTopStirrupNum(longValue);
		ar >> longValue; m_BeamRebarTemplate->SetBeamSubBotStirrupNum(longValue);
		ar >> longValue; m_BeamRebarTemplate->SetBeamSubStirrupTopLen(longValue);
		ar >> longValue; m_BeamRebarTemplate->SetBeamSubStirrupBotLen(longValue);
		ar >> boolValue; m_BeamRebarTemplate->SetBeamSubStirrupUserHookLen(boolValue);

		ar >> NumObject;
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mapBeamSubStirrupHookLen.insert(make_pair(nDiaIndex, hookLen));
		}

		ar >> NumObject;
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mapBeamShearBarHookLenA.insert(make_pair(nDiaIndex, hookLen));
		}
		
		ar >> NumObject;
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mapBeamShearBarHookLenB.insert(make_pair(nDiaIndex, hookLen));
		}
	}

	if (MSVersionInfo::GetCurrentVersion() >= 20141017)
	{
		ar >> longValue;	m_BeamRebarTemplate->SetBeamBotMaxLen2HookAnchorBar(longValue);
		ar >> longValue;	m_BeamRebarTemplate->SetBeamCapbarRatio(longValue);
	}

	if (MSVersionInfo::GetCurrentVersion() >= 20141209)
		ar >> longValue;	m_ColumnRebarTemplate->SetColumnTopMemberCover(longValue);

	//슬래브 사변지지 상부 CutBar 길이 옵션 설정
	if (MSVersionInfo::GetCurrentVersion() >= 20150326)
	{
		ar >> longValue;	m_SlabRebarTemplate->SetSlabOneWayCutBarLenType(longValue);
		ar >> longValue;	m_SlabRebarTemplate->SetSlabTwoWayCutBarLenType(longValue);
	}

	if (MSVersionInfo::GetCurrentVersion() >= 20150410)
	{
		long nSlabRebarTolDist = 0;
		long nSlabRebarTolLenDif = 0;

		ar >> nSlabRebarTolDist;
		ar >> nSlabRebarTolLenDif;
	}

	if (MSVersionInfo::GetCurrentVersion() >= 20150421)
	{
		ar >> longValue;	m_BeamRebarTemplate->SetSubBeamStirrupType(longValue);
		ar >> boolDummyValue; //m_bSubBeamUserStirrupShape;
		ar >> longValue;	m_BeamRebarTemplate->SetSubBeamCapbarRatio(longValue);
		ar >> longValue;	m_BeamRebarTemplate->SetSubBeamStirrupShape(longValue);
	}

	if (MSVersionInfo::GetCurrentVersion() >= 20150423)
	{
		ar >> boolValue;	m_BeamRebarTemplate->SetSubBeamOnlyCenterStirrupPlace(boolValue);
	}

	if (MSVersionInfo::GetCurrentVersion() >= 20150522)
	{
		//Mat
		ar >> boolValue;	m_SlabRebarTemplate->SetMatLongAlterSpliceSet(boolValue);
		ar >> longValue;	m_SlabRebarTemplate->SetMatLongAlterSpliceValue1(longValue);
		ar >> longValue;
		ar >> boolValue;	
		ar >> boolValue;	
		ar >> boolValue;	m_SlabRebarTemplate->SetMatShortAlterSpliceSet(boolValue);
		ar >> longValue;	m_SlabRebarTemplate->SetMatShortAlterSpliceValue1(longValue);
		ar >> longValue;
		ar >> boolValue;	
		ar >> boolValue;	

		//Flat
		ar >> longValue;	

		ar >> longValue;	
		ar >> longValue;	

		ar >> longValue;	m_SlabRebarTemplate->SetFlatTopEndHookType(longValue);
		ar >> longValue;	m_SlabRebarTemplate->SetFlatBotEndHookType(longValue);

		ar >> longValue;	m_SlabRebarTemplate->SetFlatIsoFootDevType(longValue);

		ar >> boolValue;	m_SlabRebarTemplate->SetFlatLongAlterSpliceSet(boolValue);
		ar >> longValue;	m_SlabRebarTemplate->SetFlatLongAlterSpliceValue1(longValue);
		ar >> longValue;
		ar >> boolValue;	
		ar >> boolValue;	
		ar >> boolValue;	m_SlabRebarTemplate->SetFlatShortAlterSpliceSet(boolValue);
		ar >> longValue;	m_SlabRebarTemplate->SetFlatShortAlterSpliceValue1(longValue);
		ar >> longValue;
		ar >> boolValue;	
		ar >> boolValue;	

		ar >> longValue;	m_SlabRebarTemplate->SetFlatRebarTolDist(longValue);
		ar >> longValue;	m_SlabRebarTemplate->SetFlatRebarTolLenDif(longValue);

		//Column
		ar >> longValue;	m_ColumnRebarTemplate->SetColumnVerBarSpliceType(longValue);
		ar >> boolValue;	m_ColumnRebarTemplate->SetColumnVerBarAlterSpliceSet(boolValue);
		ar >> longValue;	m_ColumnRebarTemplate->SetColumnVerBarAlterSpliceValue1(longValue);
		ar >> longValue;	m_ColumnRebarTemplate->SetColumnVerBarAlterSpliceValue2(longValue);
	}

	if (MSVersionInfo::GetCurrentVersion() < 20150611)
	{
		if (m_SlabRebarTemplate->GetFlatBotEndHookType() == 1)
			m_SlabRebarTemplate->SetFlatBotEndHookType(2);
	}

	if (MSVersionInfo::GetCurrentVersion() < 20160106)
	{
		Patch_20160106_OldDiaLengthData(mapColumnUserDowelSpliceLen);

		if (MSVersionInfo::GetCurrentVersion() >= 20140902)
		{
			Patch_20160106_OldDiaLengthData(mapBeamSubStirrupHookLen);
			Patch_20160106_OldDiaLengthData(mapBeamShearBarHookLenA);
			Patch_20160106_OldDiaLengthData(mapBeamShearBarHookLenB);
		}

		Patch_20160106_OldDiaMapData(mapBWallAddBarDiaIndex);
		Patch_20160106_OldDiaLengthData(mapBWallDefaultUbarLen);
		Patch_20160106_OldDiaLengthData(mapBWallUserDowelHookLen);
		Patch_20160106_OldDiaLengthData(mapBWallUserDowelSpliceLen);

		Patch_20160106_OldDiaLengthData(mapRWallDefaultUbarLen);
		Patch_20160106_OldDiaLengthData(mapRWallUserDowelHookLen);
		Patch_20160106_OldDiaLengthData(mapRWallUserDowelSpliceLen);

		longValue = m_ColumnRebarTemplate->GetColumnDowelSupportBarDiaIndex();
		Patch_20160106_OldDiaData(longValue);
		m_ColumnRebarTemplate->SetColumnDowelSupportBarDiaIndex(longValue);

		longValue = m_BWallRebarTemplate->GetBWallDowelSupportBarDiaIndex();
		Patch_20160106_OldDiaData(longValue);
		m_BWallRebarTemplate->SetBWallDowelSupportBarDiaIndex(longValue);

		longValue = m_RWallRebarTemplate->GetRWallSpacerDiaIndex();
		Patch_20160106_OldDiaData(longValue);
		m_RWallRebarTemplate->SetRWallSpacerDiaIndex(longValue);

		longValue = m_RWallRebarTemplate->GetRWallDowelSupportBarDiaIndex();
		Patch_20160106_OldDiaData(longValue);
		m_RWallRebarTemplate->SetRWallDowelSupportBarDiaIndex(longValue);
	}

	m_ColumnRebarTemplate->SetColumnUserDowelSpliceLenMap(mapColumnUserDowelSpliceLen);

	if (MSVersionInfo::GetCurrentVersion() >= 20140902)
	{
		m_BeamRebarTemplate->SetBeamSubStirrupHookLenMap(mapBeamSubStirrupHookLen);
		m_BeamRebarTemplate->SetBeamShearBarHookLenAMap(mapBeamShearBarHookLenA);
		m_BeamRebarTemplate->SetBeamShearBarHookLenBMap(mapBeamShearBarHookLenB);
	}

	map<long, long> mapBWallEndBarDiaIndex;
	mapBWallEndBarDiaIndex.clear();

	if (MSVersionInfo::GetCurrentVersion() >= 20160803)
	{
		ar >> longValue;	m_BWallRebarTemplate->SetBWallEndVerAddBarCount(longValue);

		ar >> NumObject;
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex1 = 0, nDiaIndex2 = 0;

			ar >> nDiaIndex1;
			ar >> nDiaIndex2;
			mapBWallEndBarDiaIndex.insert(make_pair(nDiaIndex1, nDiaIndex2));
		}
	}
	else
	{
		map<long, long>::iterator it;
		for (it = mapBWallAddBarDiaIndex.begin(); it != mapBWallAddBarDiaIndex.end(); it++)
		{
			mapBWallEndBarDiaIndex.insert(make_pair(it->first, it->second));
		}

		if (MSVersionInfo::GetCurrentVersion() < 20160106)
		{
			Patch_20160106_OldDiaMapData(mapBWallEndBarDiaIndex);
		}
	}

	m_BWallRebarTemplate->SetBWallAddBarDiaIndexMap(mapBWallAddBarDiaIndex);
	m_BWallRebarTemplate->SetBWallEndBarDiaIndexMap(mapBWallEndBarDiaIndex);
	m_BWallRebarTemplate->SetBWallDefaultUbarLenMap(mapBWallDefaultUbarLen);
	m_BWallRebarTemplate->SetBWallUserDowelHookLenMap(mapBWallUserDowelHookLen);
	m_BWallRebarTemplate->SetBWallUserDowelSpliceLenMap(mapBWallUserDowelSpliceLen);

	m_RWallRebarTemplate->SetRWallDefaultUbarLenMap(mapRWallDefaultUbarLen);
	m_RWallRebarTemplate->SetRWallUserDowelHookLenMap(mapRWallUserDowelHookLen);
	m_RWallRebarTemplate->SetRWallUserDowelSpliceLenMap(mapRWallUserDowelSpliceLen);
}

void OldBuildingRebarTemplate::Patch_20160106_OldDiaData(long& convertData)
{
	CString sDiaName;	sDiaName.Format(_T("%ld"), convertData);
	convertData = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
}

void OldBuildingRebarTemplate::Patch_20160106_OldDiaMapData(map<long, long>& convertMapData)
{
	int NumData = convertMapData.size();
	map<long, long> oldDataMap;
	map<long, long>::iterator it;
	for (it = convertMapData.begin(); it != convertMapData.end(); it++)
		oldDataMap.insert(make_pair(it->first, it->second));

	CString sDiaName1, sDiaName2;
	convertMapData.clear();
	for (it = oldDataMap.begin(); it != oldDataMap.end(); it++)
	{
		sDiaName1.Format(_T("%ld"), it->first);
		sDiaName2.Format(_T("%ld"), it->second);

		int index1 = MSDeformedBar::FindBarDiameterIndexByName(sDiaName1);
		int index2 = MSDeformedBar::FindBarDiameterIndexByName(sDiaName2);

		convertMapData.insert(make_pair(index1, index2));
	}
}

void OldBuildingRebarTemplate::Patch_20160106_OldDiaLengthData(map<long, long>& convertMapData)
{
	int NumData = convertMapData.size();
	map<long, long> oldDataMap;
	map<long, long>::iterator it;
	for (it = convertMapData.begin(); it != convertMapData.end(); it++)
		oldDataMap.insert(make_pair(it->first, it->second));

	CString sDiaName;
	convertMapData.clear();
	for (it = oldDataMap.begin(); it != oldDataMap.end(); it++)
	{
		sDiaName.Format(_T("%ld"), it->first);
		long length = it->second;
		int index = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
		convertMapData.insert(make_pair(index, length));
	}
}

void OldBuildingRebarTemplate::DataInit()
{
	m_sName = _T("");
	m_nID = -1;
}

void OldBuildingRebarTemplate::CreateData()
{
	m_sName = _T("Building Rebar Template");

	m_ColumnRebarTemplate = (OldColumnRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldColumn);
	m_BeamRebarTemplate = (OldBeamRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldBeam);
	m_BWallRebarTemplate = (OldBWallRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldBWall);
	m_RWallRebarTemplate = (OldRWallRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldRWall);
	m_PitRebarTemplate = (OldPitRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldPit);
	m_SlabRebarTemplate = (OldSlabRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldSlab);
	m_IsoFootRebarTemplate = (OldIsoFootRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldIsoFoot);
	m_WallFootRebarTemplate = (OldWallFootRebarTemplate*)OldRebarTemplateManager::GetInstance()->CreateRebarTemplate(RT_OldWallFoot);
}

void OldBuildingRebarTemplate::OldBuildingDataCopy(RebarTemplate* rebarTemplate)
{
	m_ColumnRebarTemplate->OldDataCopy((ColumnRebarTemplate*)rebarTemplate->GetColumnRebarTemplate());
	m_BeamRebarTemplate->OldDataCopy((BeamRebarTemplate*)rebarTemplate->GetBeamRebarTemplate());
	m_BWallRebarTemplate->OldDataCopy((BWallRebarTemplate*)rebarTemplate->GetBWallRebarTemplate());
	m_RWallRebarTemplate->OldDataCopy((RWallRebarTemplate*)rebarTemplate->GetRWallRebarTemplate());
	m_PitRebarTemplate->OldDataCopy((PitRebarTemplate*)rebarTemplate->GetPitRebarTemplate());
	m_SlabRebarTemplate->OldDataCopy((SlabRebarTemplate*)rebarTemplate->GetSlabRebarTemplate());
	if (m_IsoFootRebarTemplate != nullptr)
		m_IsoFootRebarTemplate->OldDataCopy((IsoFootRebarTemplate*)rebarTemplate->GetIsoFootRebarTemplate());
	if (m_WallFootRebarTemplate != nullptr)
		m_WallFootRebarTemplate->OldDataCopy((WallFootRebarTemplate*)rebarTemplate->GetWallFootRebarTemplate());
}