#include "stdafx.h"
#include "CogReader.h"

#include "NXBoxColumnHelper.h"
#include "NXBoxBeamHelper.h"
#include "NXBoxWallHelper.h"
#include "NXBoxRWallHelper.h"
#include "NXBoxSlabHelper.h"
#include "NXBoxIosFootHelper.h"
#include "NXBoxWallFootHelper.h"
#include "NXBoxPitHelper.h"
#include "NXBoxLowWallHelper.h"

void CogReader::DeleteBoxDatas()
{
	for (int i = 0; i < ma_BoxData.size(); i++)
		delete ma_BoxData[i];
	ma_BoxData.clear();
}


NXBoxData* CogReader::AddColumnBox(int version, int figureID, CString sName, GM2DVector cenVec, double width, double depth, int topLevel, int botLevel, double rotate)
{
	if (mp_MSFloor->IsRoof())	return nullptr;

	NXBoxColumnData* pData = (NXBoxColumnData*)NXBoxHelper::CreateNXBoxData(DG_COLUMN);
	pData->SetVersion(version);
	pData->SetFigureID(figureID);
	pData->SetName(sName);
	pData->SetShape(1);
	pData->SetWidth(width);
	pData->SetDepth(depth);
	pData->SetOffset(topLevel, botLevel);
	pData->SetCenterPoint(cenVec);
	pData->SetRotate(rotate);
	ma_BoxData.push_back(pData);
	return pData;
}
NXBoxData* CogReader::AddColumnBox(int version, int figureID, CString sName, GM2DVector cenVec, double radius, int topLevel, int botLevel)
{
	if (mp_MSFloor->IsRoof())	return nullptr;

	NXBoxColumnData* pData = (NXBoxColumnData*)NXBoxHelper::CreateNXBoxData(DG_COLUMN);
	pData->SetVersion(version);
	pData->SetFigureID(figureID);
	pData->SetName(sName);
	pData->SetShape(2);
	pData->SetRadius(radius);
	pData->SetOffset(topLevel, botLevel);
	pData->SetCenterPoint(cenVec);
	ma_BoxData.push_back(pData);
	return pData;
}

NXBoxData* CogReader::AddBeamBox(int version, int figureID, CString sName, GM2DVector sVec, GM2DVector eVec, double width, int nLevel)
{
	NXBoxBeamData* pData = (NXBoxBeamData*)NXBoxHelper::CreateNXBoxData(DG_BEAM);
	pData->SetVersion(version);
	pData->SetFigureID(figureID);
	pData->SetName(sName);
	pData->SetWidth(width);
	pData->SetOffset(nLevel);
	pData->SetStartVec(sVec);
	pData->SetEndVec(eVec);
	ma_BoxData.push_back(pData);
	return pData;
}

NXBoxData* CogReader::AddWallBox(int version, int figureID, CString sName, GM2DVector sVec, GM2DVector eVec, double thick, int topLevel, int botLevel)
{
	if (mp_MSFloor->IsRoof())	return nullptr;

	NXBoxWallData* pData = (NXBoxWallData*)NXBoxHelper::CreateNXBoxData(DG_WALL);
	pData->SetVersion(version);
	pData->SetFigureID(figureID);
	pData->SetName(sName);
	pData->SetThick(thick);
	pData->SetOffset(topLevel, botLevel);
	pData->SetStartVec(sVec);
	pData->SetEndVec(eVec);
	ma_BoxData.push_back(pData);
	return pData;
}

NXBoxData* CogReader::AddRWallBox(int version, int figureID, CString sName, GM2DVector sVec, GM2DVector eVec, double thick, int topLevel, int botLevel, bool IsLeftInside)
{
	if (mp_MSFloor->IsRoof())	return nullptr;

	NXBoxRWallData* pData = (NXBoxRWallData*)NXBoxHelper::CreateNXBoxData(DG_RWALL);
	pData->SetVersion(version);
	pData->SetFigureID(figureID);
	pData->SetName(sName);
	pData->SetThick(thick);
	pData->SetOffset(topLevel, botLevel);
	pData->SetLeftInside(IsLeftInside);
	pData->SetStartVec(sVec);
	pData->SetEndVec(eVec);
	ma_BoxData.push_back(pData);
	return pData;
}

NXBoxData* CogReader::AddLowWallBox(int version, int figureID, CString sName, GM2DVector sVec, GM2DVector eVec, double thick, int level)
{
	NXBoxLowWallData* pData = (NXBoxLowWallData*)NXBoxHelper::CreateNXBoxData(DG_LOWWALL);
	pData->SetVersion(version);
	pData->SetFigureID(figureID);
	pData->SetName(sName);
	pData->SetThick(thick);
	pData->SetOffset(level);
	pData->SetStartVec(sVec);
	pData->SetEndVec(eVec);
	ma_BoxData.push_back(pData);
	return pData;
}

NXBoxData* CogReader::AddWallFootBox(int version, int figureID, CString sName, GM2DVector sVec, GM2DVector eVec, double width, int level)
{
	NXBoxWallFootData* pData = (NXBoxWallFootData*)NXBoxHelper::CreateNXBoxData(DG_WFOOT);
	pData->SetVersion(version);
	pData->SetFigureID(figureID);
	pData->SetName(sName);
	pData->SetWidth(width);
	pData->SetOffset(level);
	pData->SetStartVec(sVec);
	pData->SetEndVec(eVec);
	ma_BoxData.push_back(pData);
	return pData;
}

NXBoxData* CogReader::AddIsoFootBox(int version, int figureID, CString sName, GM2DVector cenVec, double width, double depth, int level, double rotate)
{
	NXBoxIsoFootData* pData = (NXBoxIsoFootData*)NXBoxHelper::CreateNXBoxData(DG_ISOFOOT);
	pData->SetVersion(version);
	pData->SetFigureID(figureID);
	pData->SetName(sName);
	pData->SetWidth(width);
	pData->SetDepth(depth);
	pData->SetOffset(level);
	pData->SetCenterPoint(cenVec);
	pData->SetRotate(rotate);
	ma_BoxData.push_back(pData);
	return pData;
}

NXBoxData* CogReader::AddPitBox(int version, int figureID, CString sName, int pitDepth, int pitThick, bool hasWell, int wellDepth, GM2DPolyline* outLine, GM2DPolyline* innerLine, GM2DPolyline* wellLine)
{
	NXBoxPitData* pData = (NXBoxPitData*)NXBoxHelper::CreateNXBoxData(DG_PIT);
	pData->SetVersion(version);
	pData->SetFigureID(figureID);
	pData->SetName(sName);
	pData->SetPitDepth(pitDepth);
	pData->SetPitThick(pitThick);
	pData->SetPlaceWell(hasWell);
	pData->SetWellThick(wellDepth);
	pData->SetOutLine(outLine);
	pData->SetInnerLine(innerLine);
	pData->SetWellLine(wellLine);
	ma_BoxData.push_back(pData);
	return pData;
}

NXBoxData* CogReader::AddSlabBox(int version, int figureID, CString sName, int level, GM2DPolyline* boundary)
{
	NXBoxSlabData* pData = (NXBoxSlabData*)NXBoxHelper::CreateNXBoxData(DG_SLAB);
	pData->SetVersion(version);
	pData->SetFigureID(figureID);
	pData->SetName(sName);
	pData->SetOffset(level);
	pData->SetBoundary(boundary);
	ma_BoxData.push_back(pData);
	return pData;
}

void CogReader::ExtractColumnBoxs()
{
	if (m_nMemberType != DG_COLUMN)	return;
	for (int i = 0; i < ma_BoxData.size(); i++)
	{
		NXBoxColumnHelper theHelper(this);
		MSCompoG* pCompoG = theHelper.MakeData(ma_BoxData[i]);
		if (pCompoG != nullptr)
			ma_ColumnG.Add(pCompoG);
	}
}

void CogReader::ExtractWallBoxs()
{
	if (m_nMemberType != DG_WALL)	return;
	for (int i = 0; i < ma_BoxData.size(); i++)
	{
		NXBoxWallHelper theHelper(this);
		MSCompoG* pCompoG = theHelper.MakeData(ma_BoxData[i]);
		if (pCompoG != nullptr)
			ma_WallG.Add(pCompoG);
	}
}

void CogReader::ExtractRWallBoxs()
{
	if (m_nMemberType != DG_RWALL)	return;
	for (int i = 0; i < ma_BoxData.size(); i++)
	{
		NXBoxRWallHelper theHelper(this);
		MSCompoG* pCompoG = theHelper.MakeData(ma_BoxData[i]);
		if (pCompoG != nullptr)
			ma_WallG.Add(pCompoG);
	}
}

void CogReader::ExtractLowWallBoxs()
{
	if (m_nMemberType != DG_LOWWALL)	return;
	for (int i = 0; i < ma_BoxData.size(); i++)
	{
		NXBoxLowWallHelper theHelper(this);
		MSCompoG* pCompoG = theHelper.MakeData(ma_BoxData[i]);
		if (pCompoG != nullptr)
			ma_LowWallG.Add(pCompoG);
	}
}

void CogReader::ExtractSlabBoxs()
{
	if (m_nMemberType != DG_SLAB)	return;
	for (int i = 0; i < ma_BoxData.size(); i++)
	{
		NXBoxSlabHelper theHelper(this);
		MSCompoG* pCompoG = theHelper.MakeData(ma_BoxData[i]);
		if (pCompoG != nullptr)
			ma_SlabG.Add(pCompoG);
	}
}

void CogReader::ExtractBeamBoxs()
{
	if (m_nMemberType != DG_BEAM)	return;
	for (int i = 0; i < ma_BoxData.size(); i++)
	{
		NXBoxBeamHelper theHelper(this);
		MSCompoG* pCompoG = theHelper.MakeData(ma_BoxData[i]);
		if (pCompoG != nullptr)
			ma_BeamG.Add(pCompoG);
	}
}

void CogReader::ExtractPitBoxs()
{
	if (m_nMemberType != DG_PIT)	return;
	for (int i = 0; i < ma_BoxData.size(); i++)
	{
		NXBoxPitHelper theHelper(this);
		MSCompoG* pCompoG = theHelper.MakeData(ma_BoxData[i]);
		if (pCompoG != nullptr)
			ma_PitG.Add(pCompoG);
	}
}

void CogReader::ExtractIsoFootBoxs()
{
	if (m_nMemberType != DG_ISOFOOT)	return;
	for (int i = 0; i < ma_BoxData.size(); i++)
	{
		NXBoxIosFootHelper theHelper(this);
		MSCompoG* pCompoG = theHelper.MakeData(ma_BoxData[i]);
		if (pCompoG != nullptr)
			ma_IsoFootG.Add(pCompoG);
	}
}

void CogReader::ExtractWallFootBoxs()
{
	if (m_nMemberType != DG_WFOOT)	return;
	for (int i = 0; i < ma_BoxData.size(); i++)
	{
		NXBoxWallFootHelper theHelper(this);
		MSCompoG* pCompoG = theHelper.MakeData(ma_BoxData[i]);
		if (pCompoG != nullptr)
			ma_WallFootG.Add(pCompoG);
	}
}