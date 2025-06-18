#include "StdAfx.h"
#include "CogReader.h"

#include "GMLib/GMObject.h"
#include "GMLib/GM2DLineVector.h"
#include "GMLib/GMQuadTree.h"
#include "GMLib/GMText.h"

#include "StructureModel\MSRebarBuilding.h"
#include "StructureModel\MSFormWorkBuilding.h"
#include "StructureModel\MSGridLine.h"
#include "StructureModel\MSLinkMemberG.h"
#include "StructureModel\MSCompoG.h"
#include "StructureModel\MSModelChecker.h"
#include "StructureModel\MSCompoGUtil.h"

#include "DG2DLine.h"
#include "DG2DArc.h"

#include "StructureModel\MSRcFaceMemberM.h"
#include "StructureModel\MSRcIsoFootM.h"
#include "StructureModel\MSRcLineMemberM.h"
#include "StructureModel\MSRcBeamM.h"
#include "StructureModel\MSPcLineMemberM.h"
#include "StructureModel\MSSrcBeamM.h"
#include "StructureModel\MSSteelBeamM.h"
#include "StructureModel\MSSteelColumnM.h"
#include "StructureModel\MSSrcColumnM.h"
#include "StructureModel\MSSteelUtility.h"
#include "StructureModel\MSWallEndRebarLine.h"
#include "RADFCore_CLI/ProgressReporterWrapper.h"
#include "StructureModel/MSRcFWColumnM.h"
#include "StructureModel/MSRcFWBeamM.h"
#include "StructureModel/MSRcFWSlabM.h"
#include "StructureModel/MSRcFWBWallM.h"
#include "StructureModel/MSFormWorkMaterial.h"

#include "CogReaderWallOpeningHelper.h"
#include "Radfcore_CLI\AppGlobalOptionWrapper.h"

#include "MTReader.h"
#include "SlabRebarOption.h"
#include <math.h>


double CogReader::m_ARC_ANGLE = 5.;
double CogReader::m_DTOL_COLUMN_RESION = 90;
double CogReader::m_DTOL_WALL_RESION = 50;

CogReader::CogReader(void) : m_BasePnt(0., 0.)
{
	m_bBreakCircleFig = true;
	m_pQT = NULL;
	m_pBldg = NULL;
	mp_MSFloor = NULL;
	m_nMemberType = DG_BEAM;
	m_MaterialType = MSMaterial::msConcrete;
	m_IsPairLine = false;
	mp_SlabRebarOption = new SlabRebarOption();
	Init();
}

CogReader::~CogReader(void)
{
	DeleteDGMembers();
    DeleteSlabRebarDWG();
	DeleteSlabRebarInfoArr();
	DeleteGridTextInfos();
	DeleteSteelPolylines();
	DeleteDimensionArr();
	DeleteSlabRebarAreaArr();
	DeleteWallEndRebarInfos();
	DeleteBoxDatas();
	DeleteSlabPolylines();
	if (mp_DrawingHistoryMSFloor != nullptr)
		delete mp_DrawingHistoryMSFloor;
	delete mp_SlabRebarOption;
	if (mp_SlabHelper != nullptr)	delete mp_SlabHelper;
	mp_SlabHelper = nullptr;
}

void CogReader::DeleteWallEndRebarInfos()
{
	map<MSWallMemberG*, MSWallEndRebarInfo*>::iterator it;
	for(it = mm_WallEndRebarInfo.begin(); it != mm_WallEndRebarInfo.end(); it++)
		delete it->second;
	mm_WallEndRebarInfo.clear();
}
void CogReader::DeleteSlabRebarAreaArr()
{
	int NumArea = ma_SlabRebarArea.GetSize();
	for(int i = 0; i < NumArea; i++)
		delete ma_SlabRebarArea[i];
	ma_SlabRebarArea.RemoveAll();
}

void CogReader::DeleteSteelPolylines()
{
	int nData = ma_SteelPolyline.GetSize();
	for(int iData = 0; iData < nData; iData++)
	{
		delete ma_SteelPolyline[iData];
	}
	ma_SteelPolyline.RemoveAll();
}

void CogReader::DeleteSlabPolylines()
{
	int nData = ma_SlabPolyline.GetSize();
	for (int iData = 0; iData < nData; iData++)
	{
		delete ma_SlabPolyline[iData];
	}
	ma_SlabPolyline.RemoveAll();
}

void CogReader::DeleteSlabRebarInfoArr()
{
	int numInfo = ma_SlabRebarInfo.size();
	for(int iInfo = 0; iInfo < numInfo; iInfo++)
		delete ma_SlabRebarInfo[iInfo];
	ma_SlabRebarInfo.clear();
}

void CogReader::DeleteDimensionArr()
{
	int numInfo = ma_Dimension.size();
	for(int iInfo = 0; iInfo < numInfo; iInfo++)
		delete ma_Dimension[iInfo];
	ma_Dimension.clear();
}

void CogReader::DeleteGridTextInfos()
{
	int nInfo = m_GridTextInfo.size();
	for(int i = 0; i < nInfo; i++)
	{
		delete m_GridTextInfo[i];
	}
	m_GridTextInfo.clear();
}

void CogReader::Init()
{
	InitCogReaderData();

	mp_MessageContainer = NULL;

	m_BasePointKeep = TRUE;
	m_MaxSizeOfQTLeaf = 10;
	m_ExtractSeq = 0;
	m_DTOL_DIST_LINKTEST = 15.;

	m_MIN_BEAM_WIDTH = 200; 
	m_MAX_BEAM_WIDTH = 1200; 
	m_MIN_BEAM_LENGTH = m_MAX_BEAM_WIDTH;

	m_MIN_WALL_THICK = 50;
	m_MAX_WALL_THICK = 1200;
	m_MAX_WALL_ECC = 500.;
	m_MAX_GAPPED_STEP = 10;
	m_MAX_GAP_LENGTH = m_MAX_BEAM_WIDTH > m_MAX_WALL_THICK ? m_MAX_BEAM_WIDTH : m_MAX_WALL_THICK;
	m_MIN_WALL_LENGTH = m_MAX_WALL_THICK;


	m_MIN_RWALL_LENGTH = 200; 
	m_MIN_RWALL_THICK = 50; 
	m_MAX_RWALL_THICK = 300; 
	m_MAX_RWALL_ECC = 300;

	m_DTOL_PAIR_DIST_ERRRATIO	= 0.1;

	m_DTOL_DIST_COLINEAR = 2;
	m_DTOL_DEGREE_COLINEAR = 1.;
	m_DTOL_PARALLEL_DEGREE = 5.;
	m_DTOL_NODE = 100;

	m_DTOL_COLUMN = 500;

	m_MIN_WFOOT_WIDTH = 200; 
	m_MAX_WFOOT_WIDTH = 2000; 
	m_MIN_WFOOT_LENGTH = 1000;

	m_ARC_ANGLE = 5;

	m_DTOL_QT_MARGIN = m_MAX_WALL_THICK > m_MAX_BEAM_WIDTH ? m_MAX_WALL_THICK : m_MAX_BEAM_WIDTH;

	//	m_MAX_WEDGE_LENGTH = 150.;
	m_MAX_WEDGE_LENGTH = 80.;
	m_bMergedLineSegment = TRUE;
	m_SEARCH_LINEAR_WALL_FOR_NAME = false;

	m_DTOL_TEXT_DIST = 50;

	m_nGridLineOption = 1;
	m_dGridLineMinLength = 30;
	m_dGridLineExtendLength = 300;

	m_nDocID = 0;

	m_IsTopBar_for_FloorRebar = true;
	m_IsBotBar_for_FloorRebar = true;
	m_IsBasicGrid_for_FloorRebar = true;

	m_Prefix = _T("");
	m_Suffix = _T("");
	m_IsSaveM = true;

	m_nMaterialID = 0;

	m_GridTextHeight = 120;
	m_defaultDepth = 700;

	m_WallOpeningMType = MSCompoM::msDoorM;
	m_IsWallOpeningAutoM = true;
	m_DefaultWallOpeningMID = 0;
	m_FromFloorID = m_ToFloorID = 0;
	m_WallOpeningLevelType = 0;
	m_WallOpeningBottomLevel = 0;

	mp_CreateReinforceM = nullptr;
	mp_DrawingHistoryMSFloor = nullptr;
	m_DrawingHistoryLayerName = _T("");
	m_DrawingHistoryDistTolerance = 0.0;
	m_DrawingHistoryDistTolerancePercent = 1.0;
	m_DrawingHistoryDegreeTolerance = 0.0;

	m_BotOffset = 0.0;
	m_TopOffset = 0.0;
	m_IsBrickWall = false;
	m_IsReverseBeam = false;
	
	m_IsSaveM = true;
	m_defaultDepth = 700;
	m_IsAddBottomSlabLevel = false;

	m_IsFindMName = true;
	m_DefaultMName= _T("");
	m_SteelBeamLineOption = 0;
	mp_SlabRebarOption->Init();

	DeleteSteelPolylines();
	DeleteSlabPolylines();
	mm_SlabPolylineToFigureID.clear();

	mp_SlabHelper = new CogReaderSlabHelper();
}

void CogReader::InitCogReaderData()
{
	if (m_pQT)
		delete m_pQT;

	m_pQT = new GMQuadTree(20);
	m_pQT->m_IsRoot = TRUE;

	m_MinX = DBL_MAX;
	m_MaxX = -DBL_MAX;
	m_MinY = DBL_MAX;
	m_MaxY = -DBL_MAX;

	mm_GeomToFigureIDs.clear();
	mm_ErrorToMSElements.clear();
	mm_CompoGtoFigureIDs.clear();
	mm_UsedTextToCompoGs.clear();
	m_RepeatedNamesToFigIds.clear();
	ma_BoxData.clear();
	mm_IDtoLineType.clear();
	m_MatchingDictionary.clear();

	DeleteGMs();

	ma_NewM.RemoveAll();
	ma_CompoM.clear();
	m_RowMMap.RemoveAll();

	ma_RebarLine.clear();
	m_GMFigureMap.RemoveAll();
	m_GMs->RemoveAll();

	RemoveAllCompoG();

	DeleteDGMembers();
	DeleteSlabRebarDWG();
	DeleteSlabRebarInfoArr();
	DeleteGridTextInfos();
	DeleteSteelPolylines();
	DeleteDimensionArr();
	DeleteSlabRebarAreaArr();
	DeleteWallEndRebarInfos();
	DeleteBoxDatas();
	DeleteErrorRanges();
	DeleteSteelColumnDatas();
	DeleteSteelBeamDatas();
}


void CogReader::RemoveAllCompoG()
{
	ma_BeamG.RemoveAll();
	ma_WallG.RemoveAll();
	ma_ColumnG.RemoveAll();
	ma_IsoFootG.RemoveAll();
	ma_WallFootG.RemoveAll();
	ma_SlabG.RemoveAll();
	ma_PitG.RemoveAll();
	ma_LowWallG.RemoveAll();
	ma_GridLine.RemoveAll();
	ma_SlabRebar.RemoveAll();
	mm_WalltoOpeningG.clear();
}

void CogReader::InitializeSlabHelper()
{
	mp_SlabHelper->Initialize();
}
MSFloor* CogReader::GetDrawingHistoryFloor()
{
	if (mp_DrawingHistoryMSFloor == nullptr)
	{
		mp_DrawingHistoryMSFloor = new MSFloor;
		mp_DrawingHistoryMSFloor->SetOwner(m_pBldg);
// 		int floorNum = mp_MSFloor->GetFloorNum();
// 		if (!IsVerticalRecognize())	floorNum--;
// 		if (floorNum == 0)	floorNum = -1;
		mp_DrawingHistoryMSFloor->mp_QT->m_Rect.CopyFrom(&m_pBldg->m_SiteRect);
		int floorNum = 0;
		mp_DrawingHistoryMSFloor->SetFloorNum(floorNum);
	}

	return mp_DrawingHistoryMSFloor;
}

bool CogReader::IsVerticalRecognize()
{
	switch (m_nMemberType)
	{
	case DG_COLUMN:
	case DG_WALL:
	case DG_RWALL:
		return true;
	}
	return false;
}

void CogReader::AddGeometryToFigureIDs(GM2DGeometry* pGeometry, long figureID)
{
	vector<long> figureIDs;
	map<GM2DGeometry*, vector<long>>::iterator it = mm_GeomToFigureIDs.find(pGeometry);
	if(it != mm_GeomToFigureIDs.end())
	{
		figureIDs = it->second;
		mm_GeomToFigureIDs.erase(it);
	}
	figureIDs.push_back(figureID);
	mm_GeomToFigureIDs.insert(make_pair(pGeometry, figureIDs));
}

vector<long> CogReader::FindGeometyToFigureIDs(GM2DGeometry* pGeometry)
{
	vector<long> figureIDs;
	map<GM2DGeometry*, vector<long>>::iterator it = mm_GeomToFigureIDs.find(pGeometry);
	if(it != mm_GeomToFigureIDs.end())
		return it->second;
	return figureIDs;
}
void CogReader::SetOptions(int IsMultiline, double MinLength, double MaxThick, double MinThick, bool IsCenterLine, double extendLength)
{
	if(IsMultiline == 0)
		m_bMergedLineSegment = TRUE;
	else
		m_bMergedLineSegment = FALSE;

	m_MIN_RWALL_LENGTH = m_MIN_WALL_LENGTH = m_MIN_BEAM_LENGTH = m_MIN_WFOOT_LENGTH = MinLength;
 	m_MAX_RWALL_THICK = m_MAX_WALL_THICK = m_MAX_BEAM_WIDTH = m_MAX_WFOOT_WIDTH = MaxThick;
 	m_MIN_RWALL_THICK = m_MIN_WALL_THICK = m_MIN_BEAM_WIDTH = m_MIN_WFOOT_WIDTH = MinThick;

// 	m_MAX_GAPPED_STEP = 10
// 	m_DTOL_NODE = m_pDoc->m_ReadPlanOption.m_dNodeTol;
// 	m_bCreateNode = m_pDoc->m_ReadPlanOption.m_bCreateNode;
// 	m_bMergedLineSegment = m_pDoc->m_ReadPlanOption.m_bMergeLine;
// 	m_MIN_RWALL_LENGTH = m_MIN_WALL_LENGTH = m_MIN_BEAM_LENGTH = m_MIN_WFOOT_LENGTH = m_pDoc->m_ReadPlanOption.m_dMinLength;
// 	m_MAX_RWALL_THICK = m_MAX_WALL_THICK = m_MAX_BEAM_WIDTH = m_MAX_WFOOT_WIDTH = m_pDoc->m_ReadPlanOption.m_dMaxThick;
// 	m_MIN_RWALL_THICK = m_MIN_WALL_THICK = m_MIN_BEAM_WIDTH = m_MIN_WFOOT_WIDTH = m_pDoc->m_ReadPlanOption.m_dMinThick;
// 	m_nGridLineOption = m_pDoc->m_ReadPlanOption.m_nGridLineOption;
// 	m_dGridLineMinLength = m_pDoc->m_ReadPlanOption.m_dGridLineMinLength;
// 	m_dGridLineExtendLength = m_pDoc->m_ReadPlanOption.m_dGridLineExtendLength;
	if(IsCenterLine)
		m_nMemberType = DG_GRIDLINE;
	m_dGridLineExtendLength = extendLength;
}

void CogReader::SetFloorRebarOption(bool IsTopBar, bool IsBotBar, bool IsBasicGrid)
{
	m_IsTopBar_for_FloorRebar = IsTopBar;
	m_IsBotBar_for_FloorRebar = IsBotBar;
	m_IsBasicGrid_for_FloorRebar = IsBasicGrid;
}

void CogReader::SetFloorRebarOptions(int materialID, double offset)
{
	m_nMaterialID = materialID;
	m_BotOffset = offset;
}

void CogReader::SetPrefixSuffix(CString prefix, CString suffix)
{
	m_Prefix = prefix.MakeUpper();
	m_Suffix = suffix.MakeUpper();
}

void CogReader::DeleteGMs()
{
	for(long iGMs=0; iGMs<DG_ENUM_END; iGMs++)
	{
		CTypedPtrArray<CObArray, GM2DGeometry*>& aGMLayer = m_GMs[iGMs];
		long nGM = aGMLayer.GetSize();
		for(long iGM=0; iGM<nGM; iGM++)
		{
			if(aGMLayer[iGM])
				delete aGMLayer[iGM];
		}
		aGMLayer.RemoveAll();
	}
}

void CogReader::DeleteSlabRebarDWG()
{

}

CString CogReader::GetGroupName()
{
	CString GroupName = _T("");
	switch(m_nMemberType)
	{
	case DG_BEAM:	GroupName = _T("Beam");	break;
	case DG_COLUMN: GroupName = _T("Column");	break;
	case  DG_WALL:	GroupName = _T("Wall");	break;
	case DG_RWALL:	GroupName = _T("RWall");	break;
	case DG_ISOFOOT:	GroupName = _T("IsoFoot");	break;
	case DG_WFOOT:	GroupName = _T("WallFoot");	break;
	case DG_REBARZONE: GroupName = _T("Rebar");	break;
	case DG_OPENING: GroupName = _T("Opening");	break;
	case DG_WALL_ENDBARS:	GroupName = _T("Wall Endbar");	break;
	case DG_LOWWALL: GroupName = _T("LowWall"); break;
	case DG_SLAB:	GroupName = _T("Slab");	break;
	case DG_PIT:	GroupName = _T("Pit");	break;
	}
	return GroupName;
}

CString CogReader::GetProcessingMessage()
{
	CString Message = _T("Members");
	switch (m_nMemberType)
	{
	case DG_BEAM:	Message = _T("Girders");	break;
	case DG_COLUMN: Message = _T("Columns");	break;
	case  DG_WALL:	Message = _T("Walls");	break;
	case DG_RWALL:	Message = _T("RWalls");	break;
	case DG_ISOFOOT:	Message = _T("IsoFoots");	break;
	case DG_WFOOT:	Message = _T("WallFoots");	break;
	case DG_OPENING:	Message = _T("Openings");	break;
	case DG_LOWWALL:	Message = _T("LowWalls");	break;
	case DG_SLAB:	Message = _T("Slabs");	break;
	case DG_PIT:	Message = _T("Pits");	break;
	}

	CString ProcessingMessage;
	ProcessingMessage.Format(_T("Processing Extract %s"), Message);
	return ProcessingMessage;
}

bool CogReader::AddLine(int figureID, double x1, double y1, double x2, double y2, CString layerName, CString lineType)
{
	GM2DVector SVec(x1-m_BasePnt.m_X, y1-m_BasePnt.m_Y);
	GM2DVector EVec(x2-m_BasePnt.m_X, y2-m_BasePnt.m_Y);

	GM2DLineVector* pLineVec = new GM2DLineVector(SVec, EVec);
	double dDegree = pLineVec->GetDegree();
	if(dDegree > 90 && dDegree <= 270)
		pLineVec->ReverseDirection();

	if(m_nMemberType == DG_REBARZONE)
	{
		if (mp_SlabRebarOption->IsRebarLayer(layerName))
		{
			m_pQT->ma_GMs[DG_REBARLINE].Add(pLineVec);
			mp_SlabRebarOption->AddRebarLine(pLineVec, layerName, lineType);
		}
		else if (mp_SlabRebarOption->GetInfoLayer() == layerName)
		{
			m_pQT->ma_GMs[DG_INFOLINE].Add(pLineVec);
		}
		else
			m_pQT->ma_GMs[DG_REBARZONE].Add(pLineVec);
	}
	else
		m_pQT->ma_GMs[m_nMemberType].Add(pLineVec);

	m_GMFigureMap.SetAt(pLineVec, figureID);
	AddGeometryToFigureIDs(pLineVec, figureID);

	m_MinX = min(SVec.m_X, m_MinX);
	m_MaxX = max(SVec.m_X, m_MaxX);
	m_MinY = min(SVec.m_Y, m_MinY);
	m_MaxY = max(SVec.m_Y, m_MaxY);

	m_MinX = min(EVec.m_X, m_MinX);
	m_MaxX = max(EVec.m_X, m_MaxX);
	m_MinY = min(EVec.m_Y, m_MinY);
	m_MaxY = max(EVec.m_Y, m_MaxY);
	return true;
}

bool CogReader::AddPolyline(int figureID, GM2DPolyline* pPoly, CString layerName)
{
	if(m_nMemberType == DG_REBARZONE)
	{
		if(layerName.MakeUpper() == GetSlabRebarOption()->GetRangeLayer().MakeUpper())
		{
			ma_SlabRebarArea.Add(pPoly);
			return true;

			CR_SlabRebarInfo* pRebarInfo = new CR_SlabRebarInfo();
			pRebarInfo->SetBoundary(pPoly);
			pRebarInfo->SetFigureID(figureID);
			ma_SlabRebarInfo.push_back(pRebarInfo);
			return true;
		}
	}
	else if(m_nMemberType == DG_COLUMN/* && m_MaterialType == MSMaterial::msSteel*/)
	{
		ma_SteelPolyline.Add(pPoly);
	}
	else if (m_nMemberType == DG_SLAB)
	{
		mm_SlabPolylineToFigureID.insert(make_pair(pPoly, figureID));
		ma_SlabPolyline.Add(pPoly);
	}
	return false;
}

bool CogReader::AddDimension(double defPointX1, double defPointY1, double defPointX2, double defPointY2, double linePointX, double linePointY, double txtX, double txtY, CString text, long figureID)
{
	GM2DVector TextVec(txtX, txtY);
	GM2DVector defVec1(defPointX1, defPointY1);
	GM2DVector defVec2(defPointX2, defPointY2);
	GM2DVector lineVec(linePointX, linePointY);

	MSDimensionInfo* pDimension = new MSDimensionInfo();
	pDimension->m_DefPoint1.SetXY(defPointX1, defPointY1);
	pDimension->m_DefPoint2.SetXY(defPointX2, defPointY2);
	pDimension->m_LinePosition.SetXY(linePointX, linePointY);
	pDimension->m_Text = text;
	pDimension->m_FigureID = figureID;

	if (MergeDimension(pDimension) == true)
	{
		delete pDimension;
		return false;
	}
	
	ma_Dimension.push_back(pDimension);
	return true;
}

bool CogReader::MergeDimension(MSDimensionInfo* pDimension)
{
	double dDistTol = 1.0;
	for (int iDim = 0; iDim < ma_Dimension.size(); iDim++)
	{
		if (ma_Dimension[iDim]->MergeDimensionInfo(pDimension, m_DTOL_DEGREE_COLINEAR, dDistTol, m_DTOL_DIST_COLINEAR))
			return true;
	}

	return false;
}

bool CogReader::AddCircle(int figureID, double cenX, double cenY, double radius, CString layerName)
{
	GM2DVector CenVec(cenX-m_BasePnt.m_X, cenY-m_BasePnt.m_Y);

	GM2DCircle* pCircle = new GM2DCircle(radius, cenX, cenY);

	if(m_nMemberType == DG_REBARZONE)
	{
		if(IsExistedCircle(pCircle, (long)DG_REBARZONE))	
			return false;
		m_pQT->ma_GMs[DG_REBARZONE].Add(pCircle);
	}
	else
		m_pQT->ma_GMs[m_nMemberType].Add(pCircle);

	m_GMFigureMap.SetAt(pCircle, figureID);

	m_MinX = min(CenVec.m_X, m_MinX);
	m_MaxX = max(CenVec.m_X, m_MaxX);
	m_MinY = min(CenVec.m_Y, m_MinY);
	m_MaxY = max(CenVec.m_Y, m_MaxY);
	return true;
}

bool CogReader::IsExistedCircle(GM2DCircle* pCircle, long layerNum)
{
	double tolerance = 1.0;
	CTypedPtrArray<CObArray, GM2DGeometry*>& Geoms = GetGeometry(layerNum);
	int NumGeom = Geoms.GetSize();
	for(int i = 0; i < NumGeom; i++)
	{
		GM2DGeometry* pGeom = Geoms[i];
		if(pGeom->m_ObjType != GT_CIRCLE)	continue;

		GM2DCircle* pOldCircle = (GM2DCircle*)pGeom;
		if(pCircle->m_Center.SameVector4DTOL(pOldCircle->m_Center, tolerance) && fabs(pCircle->m_A - pOldCircle->m_A) < tolerance)
			return true;
	}
	return false;
}

bool CogReader::AddArc(int figureID, double cenX, double cenY, double radius, double sAngle, double eAngle, CString layerName)
{
	GM2DVector CenVec(cenX-m_BasePnt.m_X, cenY-m_BasePnt.m_Y);

	GM2DCircle* pCircle = nullptr;
	if(fabs(sAngle-eAngle) < DTOL1)
	{
		pCircle = new GM2DCircle(radius, CenVec.m_X, CenVec.m_Y);
		if(m_nMemberType >= 0)
		{
			if(m_nMemberType == DG_REBARZONE)
			{
				m_pQT->ma_GMs[DG_REBARZONE].Add(pCircle);
			}
			else
			{
				if (m_nMemberType == DG_WALL)
					m_pQT->ma_GMs[DG_WALL_ARC].Add(pCircle);
				else if (m_nMemberType == DG_BEAM)
					m_pQT->ma_GMs[DG_BEAM_ARC].Add(pCircle);
				else if (m_nMemberType == DG_WFOOT)
					m_pQT->ma_GMs[DG_WFOOT_ARC].Add(pCircle);
				else if (m_nMemberType == DG_RWALL)
					m_pQT->ma_GMs[DG_RWALL_ARC].Add(pCircle);
				else if (m_nMemberType == DG_LOWWALL)
					m_pQT->ma_GMs[DG_LOWWALL_ARC].Add(pCircle);
			}
		}
	}
	else
	{
		if(m_nMemberType >= 0)
		{
			if(m_nMemberType == DG_REBARZONE)
			{
				pCircle = NULL;
				CTypedPtrArray<CObArray, GM2DGeometry*>& REBARZONE = m_pQT->ma_GMs[DG_REBARZONE];
				long NumCircle = REBARZONE.GetSize();
				for(long iCircle=0; iCircle<NumCircle; iCircle++)
				{
					if(REBARZONE[iCircle]->m_ObjType != GT_CIRCLE)
						continue;
					GM2DCircle* pUrCircle = (GM2DCircle*)REBARZONE[iCircle];
					if(fabs(pUrCircle->m_A-radius) > DTOL1)
						continue;
					if(fabs(pUrCircle->m_Center.m_X-CenVec.m_X) > DTOL1 || fabs(pUrCircle->m_Center.m_Y-CenVec.m_Y) > DTOL1)
						continue;
					pCircle = pUrCircle;
					break;
				}
				if(pCircle==NULL)
				{
					pCircle = new GM2DCircle(radius, CenVec.m_X, CenVec.m_Y);
					REBARZONE.Add(pCircle);
				}
			}
			else
			{
				pCircle = new GM2DArc(radius, CenVec.m_X, CenVec.m_Y, sAngle, eAngle);
				if(m_nMemberType==DG_WALL)
					m_pQT->ma_GMs[DG_WALL_ARC].Add(pCircle);
				else if(m_nMemberType==DG_BEAM)
					m_pQT->ma_GMs[DG_BEAM_ARC].Add(pCircle);
				else if(m_nMemberType==DG_WFOOT)
					m_pQT->ma_GMs[DG_WFOOT_ARC].Add(pCircle);
				else if(m_nMemberType==DG_RWALL)
					m_pQT->ma_GMs[DG_RWALL_ARC].Add(pCircle);
				// RH-1520 버그수정
				else if(m_nMemberType==DG_COLUMN)
					m_pQT->ma_GMs[DG_COLUMN].Add(pCircle);
				else if (m_nMemberType == DG_LOWWALL)
					m_pQT->ma_GMs[DG_LOWWALL_ARC].Add(pCircle);
				// End
				else
				{
					int numGM = m_pQT->ma_GMs[m_nMemberType].GetSize();
					bool bExist = false;
					for(long iGM = 0; iGM < numGM; iGM++)
					{
						GM2DGeometry* pGeo = m_pQT->ma_GMs[m_nMemberType][iGM];
						if(pGeo->m_ObjType != GT_ARC)
							continue;

						GM2DCircle* pUrCircle = (GM2DCircle*)pGeo;
						if(fabs(pUrCircle->m_A-radius) > DTOL1)
							continue;
						if(fabs(pUrCircle->m_Center.m_X-CenVec.m_X) > DTOL1 || fabs(pUrCircle->m_Center.m_Y-CenVec.m_Y) > DTOL1)
							continue;
						bExist = true;
						break;
					}
					if(bExist)
						delete pCircle;
					else
						m_pQT->ma_GMs[m_nMemberType].Add(pCircle);
				}

			}
		}

	}
	if(pCircle != nullptr)
		m_GMFigureMap.SetAt(pCircle, figureID);

	m_MinX = min(CenVec.m_X, m_MinX);
	m_MaxX = max(CenVec.m_X, m_MaxX);
	m_MinY = min(CenVec.m_Y, m_MinY);
	m_MaxY = max(CenVec.m_Y, m_MaxY);
	return true;
}

bool CogReader::AddText(int figureID, double cenX, double cenY, double insertX, double insertY, CString text, double degree, double height, 
						double leftBottomX, double leftBottomY, double rightBottomX, double rightBottomY, CString layerName)
{
	GM2DVector txtVec(cenX-m_BasePnt.m_X, cenY-m_BasePnt.m_Y);
	GM2DVector insertVec(insertX-m_BasePnt.m_X, insertY-m_BasePnt.m_Y);
	text.Replace(_T(" "), _T(""));
	text = text.MakeUpper();
	GMText* pText = new GMText;
	pText->m_strText = text;
	pText->m_Vec = txtVec;
	pText->m_InsertVec = insertVec;
	pText->m_dHeight = height;
	pText->m_dDegree = degree*Rad2Deg;

	pText->m_Origin.SetXY(leftBottomX, leftBottomY);
	pText->m_Corner.SetXY(rightBottomX, rightBottomY);

	m_pQT->ma_GMs[DG_TEXT].Add(pText);

	m_pQT->ma_GMs[m_nMemberType].Add(pText);
	m_GMFigureMap.SetAt(pText, figureID);

	m_MinX = min(txtVec.m_X, m_MinX);
	m_MaxX = max(txtVec.m_X, m_MaxX);
	m_MinY = min(txtVec.m_Y, m_MinY);
	m_MaxY = max(txtVec.m_Y, m_MaxY);
	return true;
}

int CogReader::makeData()
{
	ExtractMembers();
	return true;
}
MSCompoM* CogReader::CreateNewColumnM(double dB, double dH, long nShape, CString TheName)
{
	MSCompoM* pCompoM = NULL;

	long MType = GetMSCompoMType(DG_COLUMN);

	if (GetRecognizeMode() == eRecognizeMode::Single)
	{
		pCompoM = GetCompoM(TheName.MakeUpper(), MType);
		if (pCompoM)	return pCompoM;
	}
	else if (GetRecognizeMode() == eRecognizeMode::Formwork)
	{
		MSRcFWColumnM* pColumnM = (MSRcFWColumnM*)m_pBldg->CreateNewCompoM(MType);
		pColumnM->m_nFromStory = 1;
		pColumnM->m_nToStory = 3;
		pColumnM->SetW(dB);
		pColumnM->SetD(dH);
		pColumnM->SetMaterial(m_pBldg->GetDefMaterial());

		MSRcFWColumnM::eFWColumnType fwColumnType = MSRcFWColumnM::ToFWColumnType((MSRcLineMemberM::SECT_SHAPE)nShape);
		pColumnM->SetFWColumnType(fwColumnType);
		pColumnM->SetShapeName(MSRcFWColumnM::ToShapeName(fwColumnType));
		pColumnM->MakeProfile();
		return pColumnM;
	}

	long nFrom = -(m_pBldg->GetNumUnderStory()-1);
	long nTo   =  m_pBldg->GetNumAboveStory()-1;
	if (nFrom == 0)	nFrom = 1;
	if(nTo==0)	nTo = -1;

	int width = ROUND(dB);
	int depth = ROUND(dH);

	MSCompoM* pColumnM = m_pBldg->CreateNewCompoM(MType);
	pColumnM->m_Name = TheName.MakeUpper();
	pColumnM->SetMaterial(m_pBldg->GetDefMaterial(m_MaterialType));
	if(m_MaterialType == MSMaterial::msConcrete)
	{
		((MSRcLineMemberM*)pColumnM)->SetWidth(width);
		((MSRcLineMemberM*)pColumnM)->SetDepth(depth);
		((MSRcLineMemberM*)pColumnM)->SetShape((MSRcLineMemberM::SECT_SHAPE)nShape);
	}
	else if(m_MaterialType == MSMaterial::msSteel)
	{
		MSRebarBuilding* pRebarBuilding = dynamic_cast<MSRebarBuilding*>(m_pBldg);
		if (pRebarBuilding == nullptr)	return nullptr;
		((MSSteelColumnM*)pColumnM)->m_pSection = pRebarBuilding->GetSteelUtili()->GetSection(SECTION_H_TYPE, 1);
	}
	else if(m_MaterialType == MSMaterial::msSRC)
	{
		MSRebarBuilding* pRebarBuilding = dynamic_cast<MSRebarBuilding*>(m_pBldg);
		if (pRebarBuilding == nullptr)	return nullptr;
		((MSSrcColumnM*)pColumnM)->SetWidth(width);
		((MSSrcColumnM*)pColumnM)->SetDepth(depth);
		((MSSrcColumnM*)pColumnM)->SetShape((MSRcLineMemberM::SECT_SHAPE)nShape);
		((MSSrcColumnM*)pColumnM)->m_pSection = pRebarBuilding->GetSteelUtili()->GetSection(SECTION_H_TYPE, 1);
	}
	pColumnM->m_nFromStory = nFrom;
	pColumnM->m_nToStory = nTo;
	MSMaterial* pDefMaterial = m_pBldg->GetDefMaterial(m_MaterialType);
	MSMaterial* pMaterial = m_pBldg->GetMaterial(m_nMaterialID);
	if (pMaterial == nullptr)
	{
		pColumnM->SetMaterial(pDefMaterial);
		pColumnM->SetIsUsedStoryMaterial(true);
	}
	else
	{
		pColumnM->SetMaterial(pMaterial);
		pColumnM->SetIsUsedStoryMaterial(false);
	}

	ma_NewM.Add(pColumnM);
// 	pColumnM->GetID();
// 	m_pBldg->Add(pColumnM);

// 	XMSRowM aRowM;	aRowM.Initialize();
// 	if(m_RowMMap.Lookup(pColumnM->m_Name, aRowM))
// 		aRowM.AddMID(pColumnM->m_ID);
// 	else
// 	{
// 		aRowM.AddMID(pColumnM->m_ID);
// 		aRowM.m_strName = pColumnM->m_Name;
// 		aRowM.m_nRepresentMID = pColumnM->m_ID;
// 		aRowM.SetOwner(m_pBldg);
// 	}
// 	m_RowMMap.SetAt(pColumnM->m_Name, aRowM);

	return pColumnM;
}

MSCompoM* CogReader::CreateNewIsoFootM(double dB, double dH, long nShape, CString TheName)
{
	if (GetRecognizeMode() == eRecognizeMode::Single)
	{
		MSCompoM* pCompoM = GetCompoM(TheName, MSCompoM::msIsoFootM);
		if (pCompoM)	return pCompoM;
	}
	int width = ROUND(dB);
	int depth = ROUND(dH);

 	long nFrom = -m_pBldg->GetNumUnderStory();
 	long nTo   =  m_pBldg->GetNumAboveStory()-1;
	if (nFrom == 0)	nFrom = 1;
	if (nTo == 0)	nTo = -1;
 	MSRcIsoFootM* pIsoFootM = (MSRcIsoFootM*)m_pBldg->CreateNewCompoM(MSCompoM::msIsoFootM);;
 	pIsoFootM->m_Name = TheName.MakeUpper();
 	pIsoFootM->SetWidth(width);
 	pIsoFootM->SetDepth(depth);
	pIsoFootM->SetThick(m_defaultDepth);
 	pIsoFootM->m_nFromStory = nFrom;
 	pIsoFootM->m_nToStory = nTo;
	MSMaterial* pDefMaterial = m_pBldg->GetDefMaterial(MSMaterial::msConcrete);
	MSMaterial* pMaterial = m_pBldg->GetMaterial(m_nMaterialID);
	if (pMaterial == nullptr)
	{
		pIsoFootM->SetMaterial(pDefMaterial);
		pIsoFootM->SetIsUsedStoryMaterial(true);
	}
	else
	{
		pIsoFootM->SetMaterial(pMaterial);
		pIsoFootM->SetIsUsedStoryMaterial(false);
	}

	ma_NewM.Add(pIsoFootM);
// 	pIsoFootM->GetID();
//  	m_pBldg->Add(pIsoFootM);
 
// 	ma_CompoM.push_back(pIsoFootM);
 
 	return pIsoFootM;
}

MSCompoM* CogReader::CreateNewWallM(double dT, CString TheName, BOOL bWall)
{
 	long nType = MSCompoM::msBWallM;
 	if(bWall)	nType = MSCompoM::msBWallM;
 	else			nType = MSCompoM::msRWallM;

	int thick = ROUND(dT);

	if (GetRecognizeMode() == eRecognizeMode::Single)
	{
		MSCompoM* pCompoM = GetCompoM(TheName.MakeUpper(), nType);
		if (pCompoM)	return pCompoM;
	}
	else if (GetRecognizeMode() == eRecognizeMode::Formwork)
	{
		MSRcFWBWallM* pWallM = (MSRcFWBWallM*)m_pBldg->CreateNewCompoM(MSCompoM::msBWallM);
		pWallM->m_nFromStory = 1;
		pWallM->m_nToStory = 3;
		pWallM->SetName(_T("NONAME"));
		pWallM->SetT(thick);

		MSFormWorkMaterial::eFWMaterialType materialType = MSFormWorkMaterial::Concrete;
		MSRcFWBWallM::eFWBWallType fwWallType = MSRcFWBWallM::General;
		if (GetIsBrickWall())
		{
			fwWallType = MSRcFWBWallM::Brick;
			materialType = MSFormWorkMaterial::Brick;
		}
		pWallM->SetFWBWallType(fwWallType);
		pWallM->SetShapeName(MSRcFWBWallM::ToShapeName(fwWallType));
		pWallM->SetMaterial(((MSFormWorkBuilding*)m_pBldg)->GetDefMaterialByFWMaterialType(materialType));

		return pWallM;
	}

	long nFrom = -(m_pBldg->GetNumUnderStory()-1);
 	long nTo   =  m_pBldg->GetNumAboveStory()-1;
	if (nFrom == 0)	nFrom = 1;
 	if(nTo==0)	nTo = -1;
 	MSRcFaceMemberM* pWallM = (MSRcFaceMemberM*)m_pBldg->CreateNewCompoM(nType);
 	pWallM->m_Name = TheName.MakeUpper();
 	pWallM->SetThick(thick);
 	pWallM->m_nFromStory = nFrom;
 	pWallM->m_nToStory = nTo;
	MSMaterial* pDefMaterial = m_pBldg->GetDefMaterial(MSMaterial::msConcrete);
	MSMaterial* pMaterial = m_pBldg->GetMaterial(m_nMaterialID);
	if (pMaterial == nullptr)
	{
		pWallM->SetMaterial(pDefMaterial);
		pWallM->SetIsUsedStoryMaterial(true);
	}
	else
	{
		pWallM->SetMaterial(pMaterial);
		pWallM->SetIsUsedStoryMaterial(false);
	}

	ma_NewM.Add(pWallM);
 	return pWallM;
}

MSCompoM* CogReader::CreateNewWallFootM(double dB, CString TheName)
{
	if (GetRecognizeMode() == eRecognizeMode::Single)
	{
		MSCompoM* pCompoM = GetCompoM(TheName.MakeUpper(), MSCompoM::msWallFootM);
		if (pCompoM)	return pCompoM;
	}
	int width = ROUND(dB);

 	long nFrom = -m_pBldg->GetNumUnderStory();
 	long nTo   =  m_pBldg->GetNumAboveStory()-1;
	if (nFrom == 0)	nFrom = 1;
 	if(nTo==0)	nTo = -1;
 	MSRcLineMemberM* pWallFootM = (MSRcLineMemberM*)m_pBldg->CreateNewCompoM(MSCompoM::msWallFootM);;
 	pWallFootM->m_Name = TheName.MakeUpper();
 	pWallFootM->SetWidth(width);
	pWallFootM->SetDepth(m_defaultDepth);
 	pWallFootM->m_nFromStory = nFrom;
 	pWallFootM->m_nToStory = nTo;
	MSMaterial* pDefMaterial = m_pBldg->GetDefMaterial(MSMaterial::msConcrete);
	MSMaterial* pMaterial = m_pBldg->GetMaterial(m_nMaterialID);
	if (pMaterial == nullptr)
	{
		pWallFootM->SetMaterial(pDefMaterial);
		pWallFootM->SetIsUsedStoryMaterial(true);
	}
	else
	{
		pWallFootM->SetMaterial(pMaterial);
		pWallFootM->SetIsUsedStoryMaterial(false);
	}

	ma_NewM.Add(pWallFootM);
//  	pWallFootM->GetID();
//  	m_pBldg->Add(pWallFootM);
 
// 	ma_CompoM.push_back(pWallFootM);
 
 	return pWallFootM;
}

MSCompoM* CogReader::CreateNewHandRailM(double dB, CString TheName)
{
	if (GetRecognizeMode() == eRecognizeMode::Single)
	{
		MSCompoM* pCompoM = GetCompoM(TheName.MakeUpper(), MSCompoM::msHandRailM);
		if (pCompoM)	return pCompoM;
	}
	int width = ROUND(dB);

	long nFrom = -m_pBldg->GetNumUnderStory();
	long nTo = m_pBldg->GetNumAboveStory() - 1;
	if (nFrom == 0)	nFrom = 1;
	if (nTo == 0)	nTo = -1;
	MSRcLineMemberM* pHandRailM = (MSRcLineMemberM*)m_pBldg->CreateNewCompoM(MSCompoM::msHandRailM);;
	pHandRailM->m_Name = TheName.MakeUpper();
	pHandRailM->SetMaterial(m_pBldg->GetDefMaterial());
	pHandRailM->SetWidth(width);
	pHandRailM->m_nFromStory = nFrom;
	pHandRailM->m_nToStory = nTo;
	MSMaterial* pDefMaterial = m_pBldg->GetDefMaterial(m_MaterialType);
	MSMaterial* pMaterial = m_pBldg->GetMaterial(m_nMaterialID);
	if (pMaterial == nullptr)
	{
		pHandRailM->SetMaterial(pDefMaterial);
		pHandRailM->SetIsUsedStoryMaterial(true);
	}
	else
	{
		pHandRailM->SetMaterial(pMaterial);
		pHandRailM->SetIsUsedStoryMaterial(false);
	}

	ma_NewM.Add(pHandRailM);
	//  	pWallFootM->GetID();
	//  	m_pBldg->Add(pWallFootM);

	// 	ma_CompoM.push_back(pWallFootM);

	return pHandRailM;
}

MSCompoM* CogReader::CreateNewBeamM(double dB, CString TheName)
{
 	MSCompoM* pCompoM = NULL;
	int width = ROUND(dB);

	long MType = GetMSCompoMType(DG_BEAM);
	if (GetRecognizeMode() == eRecognizeMode::Single)
	{
		pCompoM = GetCompoM(TheName.MakeUpper(), MType);
		if (pCompoM)	return pCompoM;
	}
	else if (GetRecognizeMode() == eRecognizeMode::Formwork)
	{
		MSRcFWBeamM* pBeamM = (MSRcFWBeamM*)m_pBldg->CreateNewCompoM(MType);
		pBeamM->m_nFromStory = 1;
		pBeamM->m_nToStory = 3;
		pBeamM->SetName(_T("NONAME"));
		pBeamM->SetW(width);

		MSRcFWBeamM::eFWBeamType fwBeamType = MSRcFWBeamM::Type1;
		if (GetIsReverseBeam())
		{
			fwBeamType = MSRcFWBeamM::Type5;
			pBeamM->SetH(320);
		}
		
		pBeamM->SetFWBeamType(fwBeamType);
		pBeamM->SetShapeName(MSRcFWBeamM::ToShapeName(fwBeamType));
		pBeamM->SetMaterial(m_pBldg->GetDefMaterial());

		return pBeamM;
	}

 	long nFrom = -m_pBldg->GetNumUnderStory();
 	long nTo   =  m_pBldg->GetNumAboveStory()-1;
	if (nFrom == 0)	nFrom = 1;
	if (nTo == 0)	nTo = -1;
 	MSCompoM* pBeamM = m_pBldg->CreateNewCompoM(MType);
 	pBeamM->m_Name = TheName.MakeUpper();

	if(MType == MSCompoM::msBeamM)
	{
		((MSRcLineMemberM*)pBeamM)->SetWidth(width);
		((MSRcLineMemberM*)pBeamM)->SetDepth(m_defaultDepth);
	}
	else if(MType == MSCompoM::msPcBeamM)
	{
		((MSPcLineMemberM*)pBeamM)->SetWidth(width);
		((MSPcLineMemberM*)pBeamM)->SetPcWidth(width);
		((MSPcLineMemberM*)pBeamM)->SetPcDepth(m_defaultDepth);
	}
	else if(MType == MSCompoM::msSteelBeamM)
	{
		MSRebarBuilding* pRebarBuilding = dynamic_cast<MSRebarBuilding*>(m_pBldg);
		if (pRebarBuilding == nullptr)	return nullptr;
		((MSSteelBeamM*)pBeamM)->m_pSection = pRebarBuilding->GetSteelUtili()->GetSection(SECTION_H_TYPE, 1);
	}
	else if(MType == MSCompoM::msSrcBeamM)
	{
		MSRebarBuilding* pRebarBuilding = dynamic_cast<MSRebarBuilding*>(m_pBldg);
		if (pRebarBuilding == nullptr)	return nullptr;
		((MSSrcBeamM*)pBeamM)->SetWidth(width);
		((MSSrcBeamM*)pBeamM)->SetDepth(m_defaultDepth);
		((MSSrcBeamM*)pBeamM)->m_pSection = pRebarBuilding->GetSteelUtili()->GetSection(SECTION_H_TYPE, 1);
	}
	MSMaterial* pDefMaterial = m_pBldg->GetDefMaterial(m_MaterialType);
	MSMaterial* pMaterial = m_pBldg->GetMaterial(m_nMaterialID);
	bool isStoryMaterial = true;
	if (pMaterial == nullptr)
	{
		pBeamM->SetMaterial(pDefMaterial);
		pBeamM->SetIsUsedStoryMaterial(true);
	}
	else
	{
		pBeamM->SetMaterial(pMaterial);
		pBeamM->SetIsUsedStoryMaterial(false);
	}

 	pBeamM->m_nFromStory = nFrom;
 	pBeamM->m_nToStory = nTo;
	ma_NewM.Add(pBeamM);
 
 	return pBeamM;
}

MSCompoM* CogReader::CreateNewSlabM(double dT, CString TheName)
{
	int thick = ROUND(dT);
	if (GetRecognizeMode() == eRecognizeMode::Single)
	{
		MSCompoM* pCompoM = GetCompoM(TheName.MakeUpper(), MSCompoM::msSlabM);
		if (pCompoM)	return pCompoM;
	}
	else if (GetRecognizeMode() == eRecognizeMode::Formwork)
	{
		MSRcFWSlabM* pSlabM = (MSRcFWSlabM*)m_pBldg->CreateNewCompoM(MSCompoM::msSlabM);
		pSlabM->m_nFromStory = 1;
		pSlabM->m_nToStory = 3;
		pSlabM->SetName(_T("NONAME"));
		pSlabM->SetT(thick);

		MSRcFWSlabM::eFWSlabType fwSlabType = MSRcFWSlabM::General;
		// @@@역보인지 판단하는 코드 추가해야 한다.
		pSlabM->SetFWSlabType(fwSlabType);
		pSlabM->SetShapeName(MSRcFWSlabM::ToShapeName(fwSlabType));
		pSlabM->SetMaterial(m_pBldg->GetDefMaterial());

		return pSlabM;
	}

    long nFrom = -m_pBldg->GetNumUnderStory();
    long nTo   =  m_pBldg->GetNumAboveStory();
	if (nFrom == 0)	nFrom = 1;
	if (nTo == 0)	nTo = -1;
    MSRcFaceMemberM* pSlabM = (MSRcFaceMemberM*)m_pBldg->CreateNewCompoM(MSCompoM::msSlabM);
    pSlabM->m_Name = TheName.MakeUpper();
    pSlabM->SetThick(thick);
    pSlabM->m_nFromStory = nFrom;
    pSlabM->m_nToStory = nTo;
	MSMaterial* pDefMaterial = m_pBldg->GetDefMaterial(m_MaterialType);
	MSMaterial* pMaterial = m_pBldg->GetMaterial(m_nMaterialID);
	if (pMaterial == nullptr)
	{
		pSlabM->SetMaterial(pDefMaterial);
		pSlabM->SetIsUsedStoryMaterial(true);
	}
	else
	{
		pSlabM->SetMaterial(pMaterial);
		pSlabM->SetIsUsedStoryMaterial(false);
	}

    ma_NewM.Add(pSlabM);
//    ma_CompoM.push_back(pSlabM);

    return pSlabM;
}

MSCompoM* CogReader::FindSlabM(CString TheName)
{
	CString Name;
	Name.Format(_T("%s%s%s"), m_Prefix, TheName, m_Suffix);
	// M이 정의 되어 있지 않는 객체는 "Def"라는 키워드를 넣어주기 위해, M이 없을때는 skip
    MSCompoM* pCompoM = GetCompoM(Name.MakeUpper(), MSCompoM::msSlabM);
    if(pCompoM)	return pCompoM;
    return NULL;
}


void CogReader::MakeCompoMData()
{
	long nMSType = GetMSCompoMType(m_nMemberType);
	m_RowMMap.RemoveAll();
	ma_CompoM.clear();

	vector<long> CompoMIDArr = m_pBldg->GetCompoMs(nMSType);
	vector<long>::iterator it;
	for(it=CompoMIDArr.begin(); it != CompoMIDArr.end(); it++)
		ma_CompoM.push_back(m_pBldg->GetCompoM(*it));

	if(nMSType == MSCompoM::msColumnM)
	{
		CompoMIDArr = m_pBldg->GetCompoMs(MSCompoM::msPcColumnM);
		for(it=CompoMIDArr.begin(); it != CompoMIDArr.end(); it++)
			ma_CompoM.push_back(m_pBldg->GetCompoM(*it));

		CompoMIDArr = m_pBldg->GetCompoMs(MSCompoM::msButtressM);
		for(it=CompoMIDArr.begin(); it != CompoMIDArr.end(); it++)
			ma_CompoM.push_back(m_pBldg->GetCompoM(*it));
	}
	else if (nMSType == MSCompoM::msBeamM)
	{
		CompoMIDArr = m_pBldg->GetCompoMs(MSCompoM::msPcBeamM);
		for (it = CompoMIDArr.begin(); it != CompoMIDArr.end(); it++)
			ma_CompoM.push_back(m_pBldg->GetCompoM(*it));

	}
	else if (m_nMemberType == DG_WALL)
	{
		CompoMIDArr = m_pBldg->GetCompoMs(MSCompoM::msDoorM);
		for (it = CompoMIDArr.begin(); it != CompoMIDArr.end(); it++)
			ma_CompoM.push_back(m_pBldg->GetCompoM(*it));
	}

// 	if(nMSType==MSCompoM::msColumnM || nMSType==MSCompoM::msBWallM || nMSType==MSCompoM::msRWallM)
// 		m_pBldg->GetRowCompoMs(nMSType, m_RowMMap);
// 	else
// 	{
// 		vector<long> CompoMIDArr = m_pBldg->GetCompoMs(nMSType);
// 		vector<long>::iterator it;
// 		for(it=CompoMIDArr.begin(); it != CompoMIDArr.end(); it++)
// 			ma_CompoM.push_back(m_pBldg->GetCompoM(*it));
// 	}
}

void CogReader::ExtractMembers()
{
	ProgressReporterWrapper::GetProgressReporterWithBeginWaiting(_T("Processing Extract Members"));

	try
	{
		ProgressReporterWrapper::Instance()->SetProgressMaxValue(100);

		BuildTree();

		if(m_nMemberType == DG_REBARZONE)
		{
			ProgressReporterWrapper::Instance()->AddProgress(100);
			ProgressReporterWrapper::Instance()->AddStatus(_T("Processing Extract FloorRebar"));
			ExtractFloorRebar();
		}
		else if(m_nMemberType==DG_GRIDLINE)
		{
			if(m_nGridLineOption==0)
			{
				ProgressReporterWrapper::Instance()->AddProgress(100);
				ProgressReporterWrapper::Instance()->AddStatus(_T("Processing Extract GridEntities"));
				ExtractGridEntities();
			}
			else												
			{
				ProgressReporterWrapper::Instance()->AddProgress(100);
				ProgressReporterWrapper::Instance()->AddStatus(_T("Processing Extract GridLines"));
				ExtractGridLines();
			}
		}
		else if(m_nMemberType == DG_OPENING)
		{
			ProgressReporterWrapper::Instance()->AddProgress(100);
			ProgressReporterWrapper::Instance()->AddStatus(_T("Processing Extract Openings"));
			ExtractOpenings();
		}
		else if(m_nMemberType == DG_WALL_ENDBARS)
		{
			ProgressReporterWrapper::Instance()->AddProgress(100);
			ProgressReporterWrapper::Instance()->AddStatus(_T("Processing Extract End Rebars"));
		}
		else
		{
			ProgressReporterWrapper::Instance()->AddProgress(15);
			ProgressReporterWrapper::Instance()->AddStatus(_T("Processing Extract CompoM"));
			MakeCompoMData();

			CString ProcessingMessage = GetProcessingMessage();
			ProgressReporterWrapper::Instance()->AddProgress(100);
			ProgressReporterWrapper::Instance()->AddStatus(ProcessingMessage);

			if (GetRecognizeMode() == eRecognizeMode::Single || GetRecognizeMode() == eRecognizeMode::History 
				|| GetRecognizeMode() == eRecognizeMode::Formwork)
			{
				if (GetRecognizeMode() == eRecognizeMode::Formwork)
				{
					ExtractWalls();
					ExtractRWalls();
					ExtractColumns();
				}
				else if (!mp_MSFloor->IsRoof())
				{
					ExtractWalls();
					ExtractRWalls();
					ExtractColumns();
				}
				ExtractIsoFoots();
				ExtractWallFoots();
				ExtractGirders();
				ExtractHandRails();
				if (GetRecognizeMode() == eRecognizeMode::History)
					ExtractPits();
				else if (GetRecognizeMode() == eRecognizeMode::Formwork)
				{
					ExtractSlabs();
				}

				MakeSpliteGirders();
			} 
			else if (GetRecognizeMode() == eRecognizeMode::Box)
			{
				if (!mp_MSFloor->IsRoof())
				{
					ExtractColumnBoxs();
					ExtractWallBoxs();
					ExtractRWallBoxs();
				}
				ExtractLowWallBoxs();
				ExtractSlabBoxs();
				ExtractBeamBoxs();
				ExtractPitBoxs();
				ExtractIsoFootBoxs();
				ExtractWallFootBoxs();
			}
			AddErrorObject_RepeatedName();
		}
		Sleep(100);
		ProgressReporterWrapper::DisposeProgressReporter();
	}
	catch (CException* e)
	{
		ProgressReporterWrapper::DisposeProgressReporter();
	}
	
}

void CogReader::BuildTree()  //XCadView 에서 호출 
{ 
	m_pQT->Build(m_DTOL_QT_MARGIN);
}
void CogReader::ExtractGridLines()
{
	m_bMergedLineSegment = TRUE;
	CTypedPtrArray<CObArray, DG2DLine*> DGLines;
	MergeLineSegments(DGLines, DG_GRIDLINE);

	MakeGridTextInfos(DGLines);
	MakeGridLines(DGLines);

	int nDGLines = DGLines.GetSize();
	for(int i = 0; i < nDGLines; i++)
		delete DGLines[i];
}

void CogReader::MakeGridTextInfos(CTypedPtrArray<CObArray, DG2DLine*>& DGLines)
{
	CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = m_pQT->ma_GMs[DG_TEXT];
	CTypedPtrArray<CObArray, GM2DGeometry*>& GridGMs = m_pQT->ma_GMs[DG_GRIDLINE];

	int NumGM = GridGMs.GetSize();
	int NumText = TxtGMs.GetSize();

	vector<GM2DCircle*> CircleArr;
	vector<GM2DLineVector*> LineArr;
	for(int iGM = 0; iGM < NumGM; iGM++)
	{
		GM2DGeometry* pGeom = GridGMs[iGM];
		pGeom->m_Tag = 0;
		if(pGeom->m_ObjType == GT_CIRCLE)
			CircleArr.push_back((GM2DCircle*)pGeom);
	}

	int NumDGLine = DGLines.GetSize();
	for(int i = 0; i < NumDGLine; i++)
	{
		GM2DCurve* pCurve = DGLines[i]->m_pLine;
		if(pCurve->m_ObjType == GT_LINE)
		{
			LineArr.push_back((GM2DLineVector*)pCurve);
			pCurve->m_Tag = 0;
		}
	}

	bool IsSettingGridTextHeight = false;
	int nCircle = CircleArr.size();
	for(int iCircle = 0; iCircle < nCircle; iCircle++)
	{
		GM2DCircle* pCircle = CircleArr[iCircle];
		GMText* pText = FindGridLineText(pCircle);
		if(pText == NULL)	continue;

		bool IsStart = true;
		GM2DLineVector* pGuideLine = FindGuideLine(pCircle, LineArr);
		if(pGuideLine == NULL)	continue;
		pGuideLine->m_Tag = 1;

		MSGridTextInfo* pGridInfo = new MSGridTextInfo;
		pGridInfo->m_pTextCircle = pCircle;
		pGridInfo->m_pText = pText;
		pGridInfo->m_pGuideLine = pGuideLine;
		FindConnectedGuideLine(pGridInfo, LineArr);

		m_GridTextInfo.push_back(pGridInfo);
		if(IsSettingGridTextHeight == false)
		{
			m_GridTextHeight = (int)pText->m_dHeight;
			IsSettingGridTextHeight = true;
		}
	}
}

GMText* CogReader::FindGridLineText(GM2DCircle* pCircle)
{
	GMText* pMinText = NULL;

	long Tolerance = 100;
	long MinDist = 10e10;
	CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = m_pQT->ma_GMs[DG_TEXT];
	long NumText = TxtGMs.GetSize();
	for(int iGMs = 0; iGMs < NumText; iGMs++)
	{
		GMText* TheGM = (GMText*)TxtGMs[iGMs];
		if(pCircle->Contains(TheGM->m_Vec) <= 0)
			return TheGM;
	}
	return NULL;
}

GM2DLineVector* CogReader::FindGuideLine(GM2DCircle* pCircle, vector<GM2DLineVector*>& LineVecArr)
{
	GM2DLineVector* pGuideLine = NULL;
	double dTol = 5;
	int nLine = LineVecArr.size();
	for(int iLine = 0; iLine < nLine; iLine++)
	{
		GM2DLineVector* pLineVec = LineVecArr[iLine];
		if(pCircle->Contains(pLineVec->m_P0) <= 0)
			return pLineVec;
		if(pCircle->Contains(pLineVec->m_P1) <= 0)
			return pLineVec;
	}
	return NULL;
}

void CogReader::FindConnectedGuideLine(MSGridTextInfo* pGridInfo, vector<GM2DLineVector*>& LineVecArr)
{
	GM2DLineVector* pGuideLine = pGridInfo->m_pGuideLine;
	GM2DLineVector* pLineVec = FindConnectedLine(pGuideLine, LineVecArr);
	if(pGuideLine != pLineVec)
		pGridInfo->m_pConnectedGuideLine = pLineVec;
}

GM2DLineVector* CogReader::FindConnectedLine(GM2DLineVector* pMyLine, vector<GM2DLineVector*>& LineVecArr)
{
	int nLine = LineVecArr.size();
	double distTolerance = 1.0;
	for(int iLine = 0; iLine < nLine; iLine++)
	{
		GM2DLineVector* pUrLine = LineVecArr[iLine];
		if(pUrLine == pMyLine)	continue;
		if(pUrLine->m_Tag == 1)	continue;

		bool bConnected = false;
		if(pMyLine->m_P0.SameVector4DTOL(pUrLine->m_P0, distTolerance) || pMyLine->m_P0.SameVector4DTOL(pUrLine->m_P1, distTolerance))
			bConnected = true;
		if(pMyLine->m_P1.SameVector4DTOL(pUrLine->m_P0, distTolerance) || pMyLine->m_P1.SameVector4DTOL(pUrLine->m_P1, distTolerance))
			bConnected = true;

		if(bConnected)
		{
			pUrLine->m_Tag = 1;
			return FindConnectedLine(pUrLine, LineVecArr);
		}
	}
	return pMyLine;
}

void CogReader::MakeGridLines(CTypedPtrArray<CObArray, DG2DLine*>& DGLines)
{
	double MyLoc=0, YourLoc=0;
	double dTol = 10;

	long NumGrid = DGLines.GetSize();
	// 1. Sort 조건 : Horizontal : 0~44, 316~364
	// 2. X Dir -> YDir
	// 3. 작은 순서 -> 큰순서
	vector<GM2DLineVector*> VertArr, HoriArr;
	for(long iGrid=0; iGrid<NumGrid; iGrid++)
	{
		DG2DLine* pDGLine = DGLines[iGrid];
		GM2DCurve* iObj = pDGLine->m_pLine;
		if(iObj == NULL)  continue;
		if(iObj->m_ObjType!=GT_LINE)	continue;

		GM2DLineVector* pLineVec = (GM2DLineVector*)iObj;
		if(pLineVec->m_Tag == 1)	continue;		// GuideLine은 Tag = 1로 설정
		double dDeg = pLineVec->GetDegree();
		if(dDeg >= 0 && dDeg <= 45 || dDeg >= 135 && dDeg <= 225 || dDeg >= 315 && dDeg <= 360)
			HoriArr.push_back(pLineVec);
		else
			VertArr.push_back(pLineVec);
	}

	// Vert Sort
	long NumVert = VertArr.size();
	for( iGrid = 0; iGrid < NumVert; iGrid++)
	{
		for(long jGrid = iGrid+1; jGrid < NumVert; jGrid++)
		{
			GM2DLineVector* pILVec = VertArr[iGrid];
			GM2DLineVector* pJLVec = VertArr[jGrid];
			double dICenX = (pILVec->m_P0.m_X+pILVec->m_P1.m_X)/2;
			double dJCenX = (pJLVec->m_P0.m_X+pJLVec->m_P1.m_X)/2;
			if(dICenX > dJCenX)
			{
				VertArr[iGrid] = pJLVec;
				VertArr[jGrid]= pILVec;
			}
		}
	}

	// Hori Sort
	long NumHori = HoriArr.size();
	for( iGrid = 0; iGrid < NumHori; iGrid++)
	{
		for(long jGrid = iGrid+1; jGrid < NumHori; jGrid++)
		{
			GM2DLineVector* pILVec = HoriArr[iGrid];
			GM2DLineVector* pJLVec = HoriArr[jGrid];
			double dICenY = (pILVec->m_P0.m_Y+pILVec->m_P1.m_Y)/2;
			double dJCenY = (pJLVec->m_P0.m_Y+pJLVec->m_P1.m_Y)/2;
			if(dICenY > dJCenY)
			{
				HoriArr[iGrid] = pJLVec;
				HoriArr[jGrid]= pILVec;
			}
		}
	}

	MakeGridLines(VertArr);
	MakeGridLines(HoriArr);
}

void CogReader::MakeGridLines(vector<GM2DLineVector*>& LineArr)
{
	int nLine = LineArr.size();
	for(int i = 0; i < nLine; i++)
	{
		GM2DLineVector* pLineVec = LineArr[i];

		GM2DVector StartConn, EndConn;
		MSGridTextInfo* pStartGridInfo = FindGridTextInfo(pLineVec, StartConn, true);
		MSGridTextInfo* pEndGridInfo = FindGridTextInfo(pLineVec, EndConn, false);

		MSGridLine* pGridLine = new MSGridLine();
		pGridLine->m_pOwner = mp_MSFloor;
		pGridLine->m_HasBubble1 = 0;
		pGridLine->m_HasBubble2 = 0;
		pGridLine->m_X1 = pLineVec->m_P0.m_X;
		pGridLine->m_Y1 = pLineVec->m_P0.m_Y;
		pGridLine->m_X2 = pLineVec->m_P1.m_X;
		pGridLine->m_Y2 = pLineVec->m_P1.m_Y;

		if(pStartGridInfo)
		{
			pGridLine->m_X1 = StartConn.m_X;
			pGridLine->m_Y1 = StartConn.m_Y;
			if(pStartGridInfo->m_pText != nullptr)
				pGridLine->m_Name = pStartGridInfo->m_pText->m_strText;
			pGridLine->m_HasBubble1 = TRUE;
			if(pStartGridInfo->m_pTextCircle != nullptr)
				pGridLine->m_BubbleDia = pStartGridInfo->m_pTextCircle->GetRadius()*2;
			pGridLine->m_Type = pGridLine->AnalyzeGridLineType();
		}
		if(pEndGridInfo)
		{
			pGridLine->m_X2 = EndConn.m_X;
			pGridLine->m_Y2 = EndConn.m_Y;
			if(pEndGridInfo->m_pText != nullptr)
				pGridLine->m_Name = pEndGridInfo->m_pText->m_strText;
			pGridLine->m_HasBubble2 = TRUE;
			if(pEndGridInfo->m_pTextCircle != nullptr)
				pGridLine->m_BubbleDia = pEndGridInfo->m_pTextCircle->GetRadius()*2;
			pGridLine->m_Type = pGridLine->AnalyzeGridLineType();
		}

		if(pStartGridInfo && pEndGridInfo)
		{
			if (pStartGridInfo->m_pText != nullptr && pEndGridInfo->m_pText != nullptr)
			{
				if (pStartGridInfo->m_pText->m_strText != pEndGridInfo->m_pText->m_strText)
				{
					CString sMsg;
					sMsg.Format(_T("Gridline의 양끝단 버블 텍스트가 다릅니다."));
					vector<MSElement*> elements;	elements.push_back(pGridLine);
					AddErrorObject(elements, W_CODE_NAME_NONE, WARNING_LEVEL, _T("GridLine"), sMsg);
				}
			}
		}

		ma_GridLine.Add(pGridLine);
	}
}

MSGridTextInfo* CogReader::FindGridTextInfo(GM2DLineVector* pLineVec, GM2DVector &ConnVec, bool IsStart)
{
	double degreeTolerance = 1.0;
	int nInfo = m_GridTextInfo.size();
	for(int i = 0; i < nInfo; i++)
	{
		MSGridTextInfo* pInfo = m_GridTextInfo[i];
 		if(pInfo->m_pGuideLine->IsColinear4DTol(*pLineVec, degreeTolerance))
		{
			GM2DVector CheckVec = pInfo->m_pGuideLine->m_P0;

			double LocValue = pLineVec->OrthoBaseLocValue(CheckVec);
			if(IsStart && LocValue < 0)
			{
				ConnVec = pInfo->m_pGuideLine->m_P0;
				return pInfo;
			}
			else if(IsStart == false && LocValue > 0)
			{
				ConnVec = pInfo->m_pGuideLine->m_P1;
				return pInfo;
			}
		}
		else if(pInfo->m_pConnectedGuideLine != NULL)
		{
			if(pInfo->m_pConnectedGuideLine->IsColinear4DTol(*pLineVec))
			{
				GM2DVector CheckVec = pInfo->m_pGuideLine->m_P0;

				double LocValue = pLineVec->OrthoBaseLocValue(CheckVec);
				if(IsStart && LocValue < 0)
				{
					ConnVec = pLineVec->OrthoBaseVector(pInfo->m_pGuideLine->m_P0);
					return pInfo;
				}
				else if(IsStart == false && LocValue > 0)
				{
					ConnVec = pLineVec->OrthoBaseVector(pInfo->m_pGuideLine->m_P1);
					return pInfo;
				}
			}
		}
	}
	return NULL;
}

GMText* CogReader::FindGridLineText(GM2DLineVector* pLVec)
{
	GMText* pMinText = NULL;

	long Tolerance = 100;
	long MinDist = 10e10;
	CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = m_pQT->ma_GMs[DG_TEXT];
	long NumText = TxtGMs.GetSize();
	for(int iGMs = 0; iGMs < NumText; iGMs++)
	{
		GMText* TheGM = (GMText*)TxtGMs[iGMs];
		if(iGMs == 0)
			Tolerance = TheGM->m_dHeight/2;

		GM2DVector OrthoVec = pLVec->OrthoBaseVector(TheGM->m_Vec);
		double dDist = TheGM->m_Vec.Distance(OrthoVec);
		if(dDist < Tolerance && dDist < MinDist)
		{
			MinDist = dDist;
			pMinText = TheGM;
		}
	}
	return pMinText;
}

CTypedPtrArray<CObArray, GM2DGeometry*>& CogReader::GetGeometry(long geomIndex)
{
	 return m_pQT->ma_GMs[geomIndex];
}

vector<GMQuadTree*> CogReader::FindCrossQts(GM2DGeometry* pGeom)
{
	return m_pQT->FindCrossQts(pGeom);
}

void CogReader::ExtractWalls()
{
	m_pQT->ResetGMTag(DG_WALL);
	MergeLineSegments(ma_DGWallLines, DG_WALL);
	MergeArcSegments(ma_DGWallArcs, DG_WALL_ARC);
	SortDGLines(ma_DGWallLines);
	SortDGArcs(ma_DGWallArcs);
	SortPairLines(ma_DGWallLines);

	ResetTag(ma_DGWallLines);
	ResetTag(ma_DGWallArcs);

	m_pQT->ResetGMTag(DG_RWALL);

	MergeLineSegments(ma_DGRWallLines, DG_RWALL);
	MergeArcSegments(ma_DGRWallArcs, DG_RWALL_ARC);
	SortDGLines(ma_DGRWallLines);
	SortDGArcs(ma_DGRWallArcs);
	SortPairLines(ma_DGRWallLines);
	ResetTag(ma_DGRWallLines);
	ResetTag(ma_DGRWallArcs);
	PutDGLinesIntoQT(ma_DGRWallLines);
	PutDGArcsIntoQT(ma_DGRWallArcs);

	long NumObj = ma_DGWallLines.GetSize();
	long NumRWallObj = ma_DGRWallLines.GetSize();

	for(long ObjNum=0; ObjNum<NumRWallObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGRWallLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindLinkedLine(AObj, DG_RWALL);
	}
	ResetTag(ma_DGRWallLines);
	for(ObjNum=0; ObjNum<NumRWallObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGRWallLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		
		vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(AObj);
		AObj->m_Tag = 1;
		SearchParallelLine(AObj, m_MIN_WALL_THICK, m_MAX_WALL_THICK, CrossQTs, DG_WALL);
	}
	m_pQT->ResetSearchSpace();
	PutDGLinesIntoQT(ma_DGWallLines);
	PutDGArcsIntoQT(ma_DGWallArcs);
	PutDGLinesIntoQT(ma_DGRWallLines);
	PutDGArcsIntoQT(ma_DGRWallArcs);

	for(long ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGWallLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindLinkedLine(AObj, DG_WALL);
	}

	NumObj = ma_DGWallLines.GetSize();
	for(long ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGWallLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = RemoveWedgeLink(AObj, DG_WALL);
	}

	ResetTag(ma_DGWallLines);
	ResetTag(ma_DGWallArcs);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGWallLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindGappedLine(AObj, DG_WALL);
	}

	ResetTag(ma_DGWallLines);
	ResetTag(ma_DGWallArcs);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGWallLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindStaggeredLine(AObj, DG_WALL);
	}

	ResetTag(ma_DGWallLines);
	ResetTag(ma_DGWallArcs);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGWallLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(AObj);
		AObj->m_Tag = 1;
		SearchParallelLine(AObj, m_MIN_WALL_THICK, m_MAX_WALL_THICK, CrossQTs, DG_WALL);
	}

	ResetTag(ma_DGWallLines);
	ResetTag(ma_DGWallArcs);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGWallLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindLinkedColLine(AObj, DG_WALL);
	}

	ResetTag(ma_DGWallLines);
	ResetTag(ma_DGWallArcs);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGWallLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		PrecheckWallShape(AObj);
	}

	// 마이라인 페어가 한개이고, 링크끼리 페어라 문제가 있지만
	// 원인을 못찾아 두 관계 모두 해제
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* MyLine = ma_DGWallLines[ObjNum];
		if(MyLine == NULL)  
			continue;
		long nPair = MyLine->ma_Pair.GetSize();
		if(MyLine->ma_Pair.GetSize() != 1)
			continue;

		DG2DLine* MyLink0 = MyLine->m_Linked0;
		DG2DLine* MyLink1 = MyLine->m_Linked1;

		if(!MyLink0 || !MyLink1 || !MyLink0->IsPair(MyLink1))
			continue;

		ReleasePairs(MyLine, MyLine->ma_Pair[0]);
		ReleasePairs(MyLink0, MyLink1);
	}

	ResetTag(ma_DGWallLines);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGWallLines[ObjNum];
		if(AObj == NULL)  continue;
		long NumMembers = CutEachMembers(AObj, DG_WALL);
	}
	CutEachWallArcs(TRUE);
}

void CogReader::ExtractRWalls()
{
	ResetTag(ma_DGRWallLines);
	ResetTag(ma_DGRWallArcs);

	long NumObj = ma_DGRWallLines.GetSize();
	for(long ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGRWallLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindLinkedColLine(AObj, DG_RWALL);
	}

	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGRWallLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindGappedLine(AObj, DG_RWALL);
	}
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGRWallLines[ObjNum];
		if(AObj == NULL || AObj->m_Tag != 0)  continue;
		long Result = FindStaggeredLine(AObj, DG_RWALL);
	}
	ResetTag(ma_DGRWallLines);
	for(ObjNum=0; ObjNum<NumObj; ObjNum++)
	{
		DG2DLine* AObj = ma_DGRWallLines[ObjNum];
		if(AObj == NULL)  continue;
		long NumMembers = CutEachMembers(AObj, DG_RWALL);
	}

 	CutEachWallArcs(FALSE);
}
BOOL CogReader::SearchPolygonInColumnLayer(GM2DLineVector* Geo)
{
	GM2DPolyline ThePLine(TRUE);
	CTypedPtrArray<CObArray, GM2DLineVector*> TheSelLines;
	CTypedPtrArray<CObArray, GMQuadTree*> RootQT;
	RootQT.Add(m_pQT);

	ThePLine.Add(new GM2DVector(Geo->m_P0));
	ThePLine.Add(new GM2DVector(Geo->m_P1));
	TheSelLines.Add(Geo);

	vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(Geo);;
	Geo->m_Tag = 1;

	BOOL bPolygon = SearchLinkedLine(TheSelLines, ThePLine, CrossQTs, DG_COLUMN);
	if(ThePLine.GetSize() == 2)
		return FALSE;		

	if(bPolygon == FALSE)
		bPolygon = CompletePolygonInWallLayer(TheSelLines, ThePLine, CrossQTs, DG_WALL);
	if(bPolygon == FALSE)
		bPolygon = CompletePolygonInWallLayer(TheSelLines, ThePLine, CrossQTs, DG_RWALL);

	if(bPolygon == FALSE)
	{
		long RtnVal = CompleteGMPolygon(ThePLine);
		if(RtnVal == 0)
			return FALSE;
	}

	GMPolygon ThePGon(TRUE);
	ThePGon = ThePLine;
	long SecShape = MSRcLineMemberM::RECT;
	if(ThePGon.IsRectangle() == FALSE)
		SecShape = MSRcLineMemberM::POLYGON;

	GMVector Cen3DVec = ThePGon.MassCenter();
	GM2DVector Cen2DVec(Cen3DVec.m_X, Cen3DVec.m_Y);
	ThePGon.Translate(Cen3DVec);
	double dB = ThePGon.GetLine(0).Length();
	double dH = ThePGon.GetLine(1).Length();

	MSCompoM* pCompoM=NULL;

	GM2DPolyline* pOffsetPolyline = MakeOffsetPolyline(&ThePLine, m_DTOL_TEXT_DIST, TRUE);
	GMText* pGMTxt = GetGMText_ContainPolyline(pOffsetPolyline, DG_COLUMN, m_pQT->FindCrossQts(Geo));
	delete pOffsetPolyline;

	CString strDefName;
	strDefName.Format(_T("%sC1"), m_DefNamePrefix);
	if(pGMTxt)
	{
		pCompoM = GetCompoM(pGMTxt->m_strText, GetMSCompoMType(DG_COLUMN));
		strDefName = pGMTxt->m_strText;
	}
	else
		strDefName.Format(_T("%sC1(%ld x %ld)"), m_DefNamePrefix, long(dB), long(dH));
	if(pCompoM==NULL)
	{
		pCompoM = GetCompoM(strDefName, MSCompoM::msColumnM);
		if(pCompoM==NULL)
		{
			if (!m_IsSaveM)
			{
				if (pGMTxt)	strDefName += _T("(No Section)");
			}
			pCompoM = CreateNewColumnM(dB, dH, SecShape, strDefName);
		}
	}

	double dRotation = 0.;
	BOOL bDifSection = FALSE;
	if(SecShape == MSRcLineMemberM::RECT)
	{
		double dWidth = pCompoM->GetWidth();
		double dDepth = pCompoM->GetDepth();

		GM2DLineVector FirstLine = ThePLine.GetLine(0);
		GM2DLineVector SecondLine = ThePLine.GetLine(1);
		double dFirstLength = FirstLine.Length();
		double dSecondLength = SecondLine.Length();

		if(fabs(dWidth - dFirstLength) < 1.)
		{
			dRotation = FirstLine.GetDegree();
			if(fabs(dDepth - dSecondLength) > 1.)
				bDifSection = TRUE;
		}
		else if(fabs(dWidth - dSecondLength) < 1.)
		{
			dRotation = SecondLine.GetDegree();
			if(fabs(dDepth - dFirstLength) > 1.)
				bDifSection = TRUE;
		}
		else if(fabs(dDepth - dFirstLength) < 1.)
		{
			dRotation = SecondLine.GetDegree();
			if(fabs(dWidth - dSecondLength) > 1.)
				bDifSection = TRUE;
		}
		else if(fabs(dDepth - dSecondLength) < 1.)
		{
			dRotation = FirstLine.GetDegree();
			if(fabs(dWidth - dFirstLength) > 1.)
				bDifSection = TRUE;
		}
		else
		{
			dRotation = 0.;
			bDifSection = TRUE;
		}
		// 			if(bDifSection)
		// 				CreateErrorObject(new XErrDifColSection(30007, TheSelLines, dFirstLength, dSecondLength, dWidth, dDepth));
	}
	else if(SecShape==MSRcLineMemberM::POLYGON)
	{
		double dRotate;
		BOOL bReverseLoop;
		GMPolygon SecPGon(true);
		BOOL Result = ThePGon.Compare(SecPGon, dRotate, bReverseLoop);
		if(Result == FALSE)
		{
			// 				CreateErrorObject(new XErrDifColSection(30007, TheSelLines, ThePGon.GetLine(0).Length(), ThePGon.GetLine(1).Length(), 0., 0.));
			dRotation = 0.;
		}
		else
			dRotation = dRotate;
	}
	else
		dRotation = 0.;

	MSColumnG* pColumnG = (MSColumnG*)m_pBldg->CreateNewCompoG(MSCompoG::msColumnG);
	pColumnG->ReplaceVec(0, new GM2DVector(Cen2DVec.m_X, Cen2DVec.m_Y));
	pColumnG->mp_CompoM = pCompoM;
	pColumnG->m_SOffsetZ = m_BotOffset;
	pColumnG->m_EOffsetZ = m_TopOffset;
	pColumnG->SetPointAlignType(COLUMNALIGN_CENTER);
	ma_ColumnG.Add(pColumnG);

	return TRUE;
}

long CogReader::CompleteGMPolygon(GM2DPolyline& ThePLine)
{
	long NumPoint = ThePLine.ma_Point.GetSize();
	if(NumPoint == 3)
	{
		GM2DVector NewCornerVec = *ThePLine[2] + (*ThePLine[0]-*ThePLine[1]);
		GM2DVector* pDiagonalVec = new GM2DVector(NewCornerVec);
		ThePLine.Add(pDiagonalVec);
		return 1;
	}

	GM2DVector LastPnt = ThePLine.LastPnt();
	GM2DVector FirstPnt = ThePLine.FirstPnt();

	GM2DLineVector FirstLine(*ThePLine.ma_Point[1], *ThePLine.ma_Point[0]);
	GM2DLineVector LastLine(*ThePLine.ma_Point[NumPoint-2], *ThePLine.ma_Point[NumPoint-1]);

	long Result = FirstLine.IsParallel4DTol(LastLine, m_DTOL_PARALLEL_DEGREE);
	if(Result)
	{
		double Dir = FirstLine.DirVector() * LastLine.DirVector();
		if(Dir)
		{
			double LocVal = FirstLine.OrthoBaseLocValue(LastLine.m_P1);
			if(LocVal < 1.)
				*ThePLine[NumPoint-1] = LastLine.OrthoBaseVector(FirstLine.m_P1);
			else
				*ThePLine[0] = FirstLine.OrthoBaseVector(LastLine.m_P1);
		}
		return 2;
	}
	else
	{
		GM2DVector CrossVec;
		double MyLoc, UrLoc;
		Result = FirstLine.Intersect(LastLine, CrossVec, &MyLoc, &UrLoc);

		// case 1
		if(Result == -5) // 시작선, 끝선이 평행하지 않고 연장선 상에서 만나는 경우
		{
			if(fabs(MyLoc-1.)*FirstLine.Length() < m_DTOL_DIST_LINKTEST)
// 				ThePLine.ma_Point[NumPoint-1]->SetXY(CrossVec.m_X, CrossVec.m_Y);
				ThePLine.ma_Point.RemoveAt(NumPoint-1);
			else if(fabs(UrLoc-1.)*LastLine.Length() < m_DTOL_DIST_LINKTEST)
			{
				ThePLine.ma_Point[0]->SetXY(CrossVec.m_X, CrossVec.m_Y);
				ThePLine.ma_Point.RemoveAt(NumPoint-1);
			}
			return 3;
		}
	}
	return 0;
}


BOOL CogReader::SearchGappedLine(CTypedPtrArray<CObArray, GM2DLineVector*>& TheSelLines, GM2DPolyline& ThePLine, 
																	vector<GMQuadTree*>& CrossQTs, long LayerNum)
{
	GM2DVector LastPnt = ThePLine.LastPnt();
	GM2DVector FirstPnt = ThePLine.FirstPnt();
	long NumPoint = ThePLine.ma_Point.GetSize();

	GM2DLineVector FirstLine(*ThePLine.ma_Point[1], *ThePLine.ma_Point[0]);
	GM2DLineVector LastLine(*ThePLine.ma_Point[NumPoint-2], *ThePLine.ma_Point[NumPoint-1]);

	GM2DVector CrossVec;
	double MyLoc, UrLoc;
	long Result = FirstLine.Intersect(LastLine, CrossVec, &MyLoc, &UrLoc);
	if(Result == -5)
	{

	}

	long NumQT = CrossQTs.size();
	BOOL RtnVal = FALSE;
	for(long QTNum=0; QTNum<NumQT; QTNum++)
	{
		GMQuadTree* CurQT = CrossQTs[QTNum];
		CTypedPtrArray<CObArray, GM2DGeometry*>& ColLayer = CurQT->ma_GMs[LayerNum];
		long NumGM = ColLayer.GetSize();
		for(long GMNum=0; GMNum<NumGM; GMNum++)
		{
			GM2DGeometry* AGeom = ColLayer[GMNum];
			if(AGeom == NULL || AGeom->m_Tag != 0 || AGeom->m_ObjType != GT_LINE)
				continue;

			GM2DLineVector* ALine = (GM2DLineVector*)AGeom;
			double Dist0 = LastLine.m_P1.Distance(ALine->m_P0);
			double Dist1 = LastLine.m_P1.Distance(ALine->m_P1);
			double Dist;
			GM2DVector* NearPnt, *FarPnt;
			if(Dist0 < Dist1)
			{
				NearPnt = &(ALine->m_P0);
				FarPnt = &(ALine->m_P1);
				Dist = Dist0;
			}
			else
			{
				NearPnt = &(ALine->m_P1);
				FarPnt = &(ALine->m_P0);
				Dist = Dist1;
			}
			if(Dist > DTOL_GM && Dist < m_MAX_GAP_LENGTH && LastLine.IsColinear4DTol(*ALine, m_DTOL_DEGREE_COLINEAR, m_DTOL_DIST_COLINEAR))
			{
				ALine->m_Tag = 1;
				TheSelLines.Add(ALine);
				if(FarPnt->Distance(FirstPnt) < m_DTOL_DIST_LINKTEST)
					return TRUE;
				if(ThePLine.ma_Point[NumPoint-1] != NULL)
					delete ThePLine.ma_Point[NumPoint-1];
				ThePLine.ma_Point[NumPoint-1] = new GM2DVector(*FarPnt);
				LastPnt = *FarPnt;
				LastLine.m_P0 = *NearPnt;
				LastLine.m_P1 = *FarPnt;
				continue;
			}

			Dist0 = FirstLine.m_P1.Distance(ALine->m_P0);
			Dist1 = FirstLine.m_P1.Distance(ALine->m_P1);
			if(Dist0 < Dist1)
			{
				NearPnt = &(ALine->m_P0);
				FarPnt = &(ALine->m_P1);
				Dist = Dist0;
			}
			else
			{
				NearPnt = &(ALine->m_P1);
				FarPnt = &(ALine->m_P0);
				Dist = Dist1;
			}
			if(Dist > DTOL_GM && Dist < m_MAX_GAP_LENGTH && FirstLine.IsColinear4DTol(*ALine, m_DTOL_DEGREE_COLINEAR, m_DTOL_DIST_COLINEAR))
			{
				ALine->m_Tag = 1;
				TheSelLines.InsertAt(0, ALine, 1);
				if(FarPnt->Distance(LastPnt) < m_DTOL_DIST_LINKTEST)
					return TRUE;
				ThePLine.InsertAt(0, new GM2DVector(*FarPnt), 1);
				FirstPnt = *FarPnt;
				FirstLine.m_P0 = *NearPnt;
				FirstLine.m_P1 = *FarPnt;
			}
		}
	}
	return FALSE;
}



BOOL CogReader::SearchLinkedLine(CTypedPtrArray<CObArray, GM2DLineVector*>& TheSelLines, GM2DPolyline& ThePLine, 
																	vector<GMQuadTree*>& EndLineCrossQTs, long LayerNum, BOOL bSearchGappedLine)
{
	if(bSearchGappedLine)
	{
		if(SearchGappedLine(TheSelLines, ThePLine, EndLineCrossQTs, LayerNum)==TRUE)
			return TRUE;
	}

	GM2DVector LastPnt = ThePLine.LastPnt();
	GM2DVector FirstPnt = ThePLine.FirstPnt();
	long NumPoint = ThePLine.ma_Point.GetSize();

	GM2DLineVector FirstLine(*ThePLine.ma_Point[1], *ThePLine.ma_Point[0]);
	GM2DLineVector LastLine(*ThePLine.ma_Point[NumPoint-2], *ThePLine.ma_Point[NumPoint-1]);

	GM2DLineVector* pLastSelLine = TheSelLines[TheSelLines.GetSize()-1];
	vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(pLastSelLine);
	long NumQT = CrossQTs.size();
	BOOL RtnVal = FALSE;
	for(long QTNum=0; QTNum<NumQT; QTNum++)
	{
		GMQuadTree* CurQT = CrossQTs[QTNum];
		CTypedPtrArray<CObArray, GM2DGeometry*>& ColLayer = CurQT->ma_GMs[LayerNum];
		long NumGM = ColLayer.GetSize();
		for(long GMNum=0; GMNum<NumGM; GMNum++)
		{
			GM2DGeometry* AGeom = ColLayer[GMNum];
			if(AGeom == NULL || AGeom->m_Tag != 0 || AGeom->m_ObjType != GT_LINE)
				continue;

			GM2DLineVector* ALine = (GM2DLineVector*)AGeom;
			if(LastPnt.Distance(ALine->m_P0) < m_DTOL_DIST_LINKTEST)
			{
				ALine->m_Tag = 1;
				TheSelLines.Add(ALine);
				if(ALine->m_P1.Distance(FirstPnt) < m_DTOL_DIST_LINKTEST)
					return TRUE;
				ThePLine.Add(new GM2DVector(ALine->m_P1));
				RtnVal = SearchLinkedLine(TheSelLines, ThePLine, m_pQT->FindCrossQts(ALine), LayerNum, bSearchGappedLine);
			}
			else if(LastPnt.Distance(ALine->m_P1) < m_DTOL_DIST_LINKTEST)
			{
				ALine->m_Tag = 1;
				TheSelLines.Add(ALine);
				if(ALine->m_P0.Distance(FirstPnt) < m_DTOL_DIST_LINKTEST)
					return TRUE;
				ThePLine.Add(new GM2DVector(ALine->m_P0));
				RtnVal = SearchLinkedLine(TheSelLines, ThePLine, m_pQT->FindCrossQts(ALine), LayerNum, bSearchGappedLine);
			}
			if(RtnVal == TRUE)
				return TRUE;
		}
	}

	GM2DLineVector* pFirstSelLine = TheSelLines[0];
	vector<GMQuadTree*> CrossQTsFirst = m_pQT->FindCrossQts(pFirstSelLine);
	NumQT = CrossQTsFirst.size();
	RtnVal = FALSE;
	for(QTNum=0; QTNum<NumQT; QTNum++)
	{
		GMQuadTree* CurQT = CrossQTsFirst[QTNum];
		CTypedPtrArray<CObArray, GM2DGeometry*>& ColLayer = CurQT->ma_GMs[LayerNum];
		long NumGM = ColLayer.GetSize();
		for(long GMNum=0; GMNum<NumGM; GMNum++)
		{
			GM2DGeometry* AGeom = ColLayer[GMNum];
			if(AGeom == NULL || AGeom->m_Tag != 0 || AGeom->m_ObjType != GT_LINE)
				continue;

			GM2DLineVector* ALine = (GM2DLineVector*)AGeom;
			if(FirstPnt.Distance(ALine->m_P0) < m_DTOL_DIST_LINKTEST)
			{
				ALine->m_Tag = 1;
				TheSelLines.InsertAt(0, ALine, 1);
				if(ALine->m_P1.Distance(LastPnt) < m_DTOL_DIST_LINKTEST)
					return TRUE;
				ThePLine.InsertAt(0, new GM2DVector(ALine->m_P1), 1);
				RtnVal = SearchLinkedLine(TheSelLines, ThePLine, m_pQT->FindCrossQts(ALine), LayerNum, bSearchGappedLine);
			}
			else if(FirstPnt.Distance(ALine->m_P1) < m_DTOL_DIST_LINKTEST)
			{
				ALine->m_Tag = 1;
				TheSelLines.InsertAt(0, ALine, 1);
				if(ALine->m_P0.Distance(LastPnt) < m_DTOL_DIST_LINKTEST)
					return TRUE;
				ThePLine.InsertAt(0, new GM2DVector(ALine->m_P0), 1);
				RtnVal = SearchLinkedLine(TheSelLines, ThePLine, m_pQT->FindCrossQts(ALine), LayerNum, bSearchGappedLine);
			}
			if(RtnVal == TRUE)
				return TRUE;
		}
	}

	return FALSE;
}

BOOL CogReader::CompletePolygonInWallLayer(CTypedPtrArray<CObArray, GM2DLineVector*>& TheSelLines, GM2DPolyline& ThePLine, 
																						vector<GMQuadTree*>& CrossQTs, long LayerNum)
{
	// 	long NumNewPoint = NewPoints.GetSize();
	// 	for(long PntNum=0; PntNum<NumNewPoint; PntNum++)
	// 		delete NewPoints[PntNum];
	// 	NewPoints.RemoveAll();

	GM2DVector LastPnt = ThePLine.LastPnt();
	GM2DVector FirstPnt = ThePLine.FirstPnt();
	long NumPLine = ThePLine.ma_Point.GetSize();

	GM2DLineVector* pLastLn = TheSelLines[TheSelLines.GetSize()-1];
	GM2DLineVector* pFirstLn = TheSelLines[0];

	DG2DLine* pWallLine1 = (DG2DLine*)(pFirstLn->m_Link0 != NULL ? pFirstLn->m_Link0 : pFirstLn->m_Link1);
	if(pWallLine1 == NULL)
		return FALSE;
	// 	if(pWallLine->m_P0.Distance(FirstPnt) > m_DTOL_DIST_LINKTEST &&
	// 		pWallLine->m_P1.Distance(FirstPnt) > m_DTOL_DIST_LINKTEST)
	// 		return FALSE;

	DG2DLine* pWallPairLine1 = pWallLine1->GetAPairLine(FirstPnt);
	if(pWallPairLine1 == NULL)
		return FALSE;

	DG2DLine* pWallLine2 = (DG2DLine*)(pLastLn->m_Link0 != NULL ? pLastLn->m_Link0 : pLastLn->m_Link1);
	if(pWallLine2 == NULL)
		return FALSE;
	// 	if(pWallLine2->m_P0.Distance(LastPnt) > m_DTOL_DIST_LINKTEST &&
	// 		pWallLine2->m_P1.Distance(LastPnt) > m_DTOL_DIST_LINKTEST)
	// 		return FALSE;

	DG2DLine* pWallPairLine2 = pWallLine2->GetAPairLine(LastPnt);
	if(pWallPairLine2 == NULL)
		return FALSE;

	if(pWallLine2 == pWallPairLine1)
	{
		if(pWallLine1->m_pLine->m_ObjType != GT_LINE ||
			 pWallLine2->m_pLine->m_ObjType != GT_LINE)
			return FALSE;

		GM2DLineVector* CrossLine1 = (GM2DLineVector*)pWallLine1->m_pLine;
		double Dist1 = CrossLine1->Distance(*ThePLine.ma_Point[1]);
		GM2DLineVector* CrossLine2 = (GM2DLineVector*)pWallLine2->m_pLine;
		double Dist2 = CrossLine2->Distance(*ThePLine.ma_Point[1]);
		GM2DVector CrossVec;
		double MyLoc, UrLoc;

		if(Dist1 < Dist2)
		{
			long Result = pFirstLn->Intersect(*CrossLine2, CrossVec, &MyLoc, &UrLoc);
			if(Result != -5)
				return FALSE;

			if(ThePLine.ma_Point[0] != NULL)
				delete ThePLine.ma_Point[0];
			ThePLine.ma_Point[0] = new GM2DVector(CrossVec);
			// 			NewPoints.Add(ThePLine.ma_Point[0]);

			TheSelLines.Add(CrossLine1);

			return TRUE;
		}

		else
		{
			long Result = pLastLn->Intersect(*CrossLine1, CrossVec, &MyLoc, &UrLoc);
			if(Result != -5)
				return FALSE;

			if(ThePLine.ma_Point[NumPLine-1] != NULL)
				delete ThePLine.ma_Point[NumPLine-1];
			ThePLine.ma_Point[NumPLine-1] = new GM2DVector(CrossVec);
			// 			NewPoints.Add(ThePLine.ma_Point[NumPLine-1]);

			TheSelLines.Add(CrossLine1);

			return TRUE;
		}
	}

	if(pWallLine1->m_pLine->m_ObjType != GT_LINE ||
		 pWallLine2->m_pLine->m_ObjType != GT_LINE)
		return FALSE;

	GM2DLineVector* CrossLine1 = (GM2DLineVector*)pWallPairLine1->m_pLine;
	GM2DVector CrossVec1;
	double MyLoc, UrLoc;
	long Result = pFirstLn->Intersect(*CrossLine1, CrossVec1, &MyLoc, &UrLoc);
	if(Result != -5)
		return FALSE;

	GM2DLineVector* CrossLine2 = (GM2DLineVector*)pWallPairLine2->m_pLine;
	GM2DVector CrossVec2;
	Result = pLastLn->Intersect(*CrossLine2, CrossVec2, &MyLoc, &UrLoc);
	if(Result != -5)
		return FALSE;

	if(CrossLine1->IsParallel4DTol(*CrossLine2, m_DTOL_DEGREE_COLINEAR))
	{
		if(CrossLine1->IsColinear4DTol(*CrossLine2, m_DTOL_DEGREE_COLINEAR, m_DTOL_DIST_LINKTEST))
		{
			if(ThePLine.ma_Point[0] != NULL)
				delete ThePLine.ma_Point[0];
			ThePLine.ma_Point[0] = new GM2DVector(CrossVec1);
			// 			NewPoints.Add(ThePLine.ma_Point[0]);

			if(ThePLine.ma_Point[NumPLine-1] != NULL)
				delete ThePLine.ma_Point[NumPLine-1];
			ThePLine.ma_Point[NumPLine-1] = new GM2DVector(CrossVec2);
			// 			NewPoints.Add(ThePLine.ma_Point[NumPLine-1]);

			TheSelLines.Add(CrossLine1);
		}
	}
	else
	{
		GM2DVector CrossVec;
		double Loc1, Loc2;
		long Result = CrossLine1->Intersect(*CrossLine2, CrossVec, &Loc1, &Loc2);
		if(Result != 0 && Result != -5)
			return FALSE;
		if(ThePLine.ma_Point[0] != NULL)
			delete ThePLine.ma_Point[0];
		ThePLine.ma_Point[0] = new GM2DVector(CrossVec1);
		// 		NewPoints.Add(ThePLine.ma_Point[0]);
		ThePLine.ma_Point[NumPLine-1] = new GM2DVector(CrossVec2);
		// 		if(ThePLine.ma_Point[NumPLine-1] != NULL)
		// 			delete ThePLine.ma_Point[NumPLine-1];
		// 		NewPoints.Add(ThePLine.ma_Point[NumPLine-1]);
		ThePLine.Add(new GM2DVector(CrossVec));
		// 		NewPoints.Add(ThePLine.ma_Point[ThePLine.GetSize()-1]);
		TheSelLines.Add(CrossLine2);
		TheSelLines.Add(CrossLine1);
	}
	return TRUE;

}

GM2DPolyline* CogReader::MakeOffsetPolyline(GM2DPolyline* pPolyline, double dDist, BOOL bExtDir)
{
	if(pPolyline == NULL || pPolyline->ma_Point.GetSize()==0)
		return NULL;

	GM2DVector* pFirstVec = pPolyline->ma_Point[0];
	GM2DPolyline copyPolyline(*pPolyline);
	if(copyPolyline.IsClosed() == false)
	{
		copyPolyline.ma_Point.Add(new GM2DVector(*pFirstVec));
		copyPolyline.ma_Bulge.Add(0);
	}
	if(pPolyline->ma_Bulge.GetSize() == 0)
	{
		for(int iBulge = 0; iBulge < pPolyline->ma_Point.GetSize()-1; iBulge++)
			copyPolyline.ma_Bulge.Add(0);
	}

	GM2DPolyline* offsetPolyline = new GM2DPolyline(TRUE);
	copyPolyline.OffSet_Acis(dDist, bExtDir, *offsetPolyline);
	
	return offsetPolyline;
}

void CogReader::AddWallOpeningG(MSWallMemberG* pWallG, MSWallOpeningG* pOpeningG)
{
	map<MSWallMemberG*, vector<MSWallOpeningG*>>::iterator it = mm_WalltoOpeningG.find(pWallG);
	if(it == mm_WalltoOpeningG.end())
	{
		vector<MSWallOpeningG*> openingArr;
		openingArr.push_back(pOpeningG);
		mm_WalltoOpeningG.insert(make_pair(pWallG, openingArr));
	}
	else
		it->second.push_back(pOpeningG);
}

GM2DPolyline* CogReader::GetSteelPolyline_ContainPolyline(GM2DPolyline* pPolyline)
{
	int nSteel = ma_SteelPolyline.GetSize();
	for(int i = 0; i < nSteel; i++)
	{
		GM2DPolyline* pSteelLine = ma_SteelPolyline[i];
		if(pPolyline->Contains(pSteelLine, true))
			return pSteelLine;
	}
	return NULL;
}

GMText* CogReader::GetGMText_ContainPolyline(GM2DPolyline* pPolyline, long nDGType, vector<GMQuadTree*>& CrossQTs)
{
	GMText* pRtnGM = NULL;

	long MemType = GetMSCompoMType(nDGType);

	double dMinDist = m_DTOL_TEXT_DIST;
	bool bFirstFind = false;
	GM2DVector CenVec = pPolyline->MassCenter();
	long NumCrossQT = CrossQTs.size();
	if(NumCrossQT == 0)
	{
		CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = m_pQT->ma_GMs[DG_TEXT];	
		long NumGMs = TxtGMs.GetSize();
		for(long iGMs=0; iGMs<NumGMs; iGMs++)
		{
			GMText* TheGM = (GMText*)TxtGMs[iGMs];
			if(TheGM==NULL) continue;

			bool IsContain = false;
			if(pPolyline->Contains(&TheGM->m_Vec) != 1 )	// 1:폴리곤 밖
				IsContain = true;
			if(IsContain == false)
			{
				if(pPolyline->Contains(&TheGM->m_InsertVec) != 1)
					IsContain = true;
			}

			if(IsContain == false)
				continue;

			if (GetRecognizeMode() == eRecognizeMode::Single && !m_IsSaveM)
			{
				CString Name;
				Name.Format(_T("%s%s%s"), m_Prefix, TheGM->m_strText, m_Suffix);
				MSCompoM* pCompoM = GetCompoM(Name, MemType);
				if (pCompoM == NULL)	continue;
			}
			double dCenDist = CenVec.Distance(TheGM->m_Vec);
			double dInsertDist = CenVec.Distance(TheGM->m_InsertVec);
			double TextDist = min(dCenDist, dInsertDist);
			if(bFirstFind == false)
			{
				bFirstFind = true;	dMinDist = TextDist;	pRtnGM = TheGM;	continue;
			}

			if(TextDist < dMinDist)
			{
				dMinDist = TextDist;
				pRtnGM = TheGM;
			}
		}
	}
	else
	{
		for(long iCrossQT=0; iCrossQT<NumCrossQT; iCrossQT++) 
		{
			GMQuadTree* pQT = CrossQTs[iCrossQT];
			CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = pQT->ma_GMs[DG_TEXT];
			long NumGMs = TxtGMs.GetSize();
			for(long iGMs=0; iGMs<NumGMs; iGMs++)
			{
				GMText* TheGM = (GMText*)TxtGMs[iGMs];
				if(TheGM==NULL) continue;

				bool IsContain = false;
				if(pPolyline->Contains(&TheGM->m_Vec) != 1 )	// 1:폴리곤 밖
					IsContain = true;
				if(IsContain == false)
				{
					if(pPolyline->Contains(&TheGM->m_InsertVec) != 1)
						IsContain = true;
				}

				if(IsContain == false)
					continue;

				if (GetRecognizeMode() == eRecognizeMode::Single && !m_IsSaveM)
				{
					CString Name;
					Name.Format(_T("%s%s%s"), m_Prefix, TheGM->m_strText, m_Suffix);
					MSCompoM* pCompoM = GetCompoM(Name, MemType);
					if (pCompoM == NULL)	continue;
				}

				double dCenDist = CenVec.Distance(TheGM->m_Vec);
				double dInsertDist = CenVec.Distance(TheGM->m_InsertVec);
				double TextDist = min(dCenDist, dInsertDist);
				if(bFirstFind == false)
				{
					bFirstFind = true;	dMinDist = TextDist;	pRtnGM = TheGM;	continue;
				}

				if(TextDist < dMinDist)
				{
					dMinDist = TextDist;
					pRtnGM = TheGM;
				}
			}
		}
	}
	return pRtnGM;
}

GMText* CogReader::GetGMText_ContainCircle(GM2DCircle* pCircle, long nDGType, vector<GMQuadTree*>& CrossQTs)
{
	GMText* pRtnGM = NULL;

	long MemType = MSCompoM::msColumnM;
	switch(nDGType)
	{
	case DG_COLUMN:
		MemType = MSCompoM::msColumnM;
		break;
	case DG_ISOFOOT	:
		MemType = MSCompoM::msIsoFootM;
		break;
	}
	double dMinDist = m_DTOL_TEXT_DIST;
	bool bFirstFind = false;

	GM2DVector CenVec = pCircle->m_Center;
	long NumCrossQT = CrossQTs.size();
	for(long iCrossQT=0; iCrossQT<NumCrossQT; iCrossQT++) 
	{
		GMQuadTree* pQT = CrossQTs[iCrossQT];
		CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = pQT->ma_GMs[DG_TEXT];
		long NumGMs = TxtGMs.GetSize();
		for(long iGMs=0; iGMs<NumGMs; iGMs++)
		{
			GMText* TheGM = (GMText*)TxtGMs[iGMs];
			if(TheGM==NULL) continue;

			bool IsContain = false;
			if(pCircle->Contains(TheGM->m_Vec) != 1 )	// 1:폴리곤 밖
				IsContain = true;
			if(IsContain == false)
			{
				if(pCircle->Contains(TheGM->m_InsertVec) != 1)
					IsContain = true;
			}
			if(IsContain == false)
				continue;

			if (GetRecognizeMode() == eRecognizeMode::Single && !m_IsSaveM)
			{
				CString Name;
				Name.Format(_T("%s%s%s"), m_Prefix, TheGM->m_strText, m_Suffix);
				MSCompoM* pCompoM = GetCompoM(Name, MemType);
				if (pCompoM == NULL)	continue;
			}
			double dCenDist = CenVec.Distance(TheGM->m_Vec);
			double dInsertDist = CenVec.Distance(TheGM->m_InsertVec);
			double TextDist = min(dCenDist, dInsertDist);
			if(bFirstFind == false)
			{
				bFirstFind = true;	dMinDist = TextDist;	pRtnGM = TheGM;	continue;
			}

			if(TextDist < dMinDist)
			{
				dMinDist = TextDist;
				pRtnGM = TheGM;
			}
		}
	}
	return pRtnGM;
}

GMText* CogReader::GetNearestGMText(GM2DGeometry* AObj, long nDGType, GM2DVector TheVec)
{
	//  CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = m_pQT->ma_GMs[DG_TEXT];
	double dMinDist = m_DTOL_TEXT_DIST;

	CString KeyWord =_T("");
	BOOL bUseKeyWord = FALSE;
	switch(nDGType)
	{
	case DG_COLUMN:
		KeyWord = _T("C");
		bUseKeyWord = FALSE;
		break;
	case DG_WALL: 
		KeyWord = _T("W");
		bUseKeyWord = TRUE;
		break;
	case DG_RWALL:
		KeyWord = _T("R");
		bUseKeyWord = TRUE;
		break;
	case DG_ISOFOOT	:
	case DG_WFOOT:
		KeyWord = _T("F");
		bUseKeyWord = TRUE;
		break;
	}
	GMText* pRtnGM = NULL;
	vector<GMQuadTree*> CrossQTs = m_pQT->FindCrossQts(AObj);
	long NumCrossQT = CrossQTs.size();
	for(long iCrossQT=0; iCrossQT<NumCrossQT; iCrossQT++) 
	{
		GMQuadTree* pQT = CrossQTs[iCrossQT];
		CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = pQT->ma_GMs[DG_TEXT];
		long NumGMs = TxtGMs.GetSize();
		for(long iGMs=0; iGMs<NumGMs; iGMs++)
		{
			GMText* TheGM = (GMText*)TxtGMs[iGMs];
			if(TheGM==NULL) continue;

// 			if(bUseKeyWord)
// 			{
// 				if(TheGM->m_strText.Find(KeyWord) < 0)
// 					continue;
// 			}

			long MemType;
			switch(nDGType)
			{
			case DG_COLUMN:
				MemType = MSCompoM::msColumnM;
				break;
			case DG_WALL: 
				MemType = MSCompoM::msBWallM;
				break;
			case DG_RWALL:
				MemType = MSCompoM::msRWallM;
				break;
			case DG_ISOFOOT	:
				MemType = MSCompoM::msIsoFootM;
				break;
			case DG_WFOOT:
				MemType = MSCompoM::msWallFootM;
				break;
			}

			if (GetRecognizeMode() == eRecognizeMode::Single)
			{
				CString Name;
				Name.Format(_T("%s%s%s"), m_Prefix, TheGM->m_strText, m_Suffix);
				MSCompoM* pCompoM = GetCompoM(Name, MemType);
				if (pCompoM == NULL)	continue;
			}

			double dDist = TheVec.Distance(TheGM->m_Vec);
			if(dDist < dMinDist)
			{
				dMinDist = dDist;
				pRtnGM = TheGM;
			}
		}
	}
	return pRtnGM;
}

MSCompoM* CogReader::FindGMTextInLinearWall( long MyLineDir, DG2DLine* MyLine, DG2DLine* UrLine, long Type )
{
	DG2DLine* MyStaggered, *UrStaggered;
	GMText* pGMTxt = NULL;
	MSCompoM* pCompoM=NULL;

	if(MyLineDir == 0)
	{
		MyStaggered = MyLine->m_Staggered0;
		UrStaggered = MyLine->IsSameDir(UrLine) ? UrLine->m_Staggered0 : UrLine->m_Staggered1;
	}
	else
	{
		MyStaggered = MyLine->m_Staggered1;
		UrStaggered = MyLine->IsSameDir(UrLine) ? UrLine->m_Staggered1 : UrLine->m_Staggered0;
	}

	if(MyStaggered==NULL || UrStaggered==NULL )
		return NULL;

	if(MyStaggered->IsPair(UrStaggered)==FALSE)
		return FindGMTextInLinearWall(MyLineDir, MyLine, UrStaggered, Type);

	pGMTxt = FindGMText(MyStaggered, UrStaggered, Type);
	if(pGMTxt)
	{
		CString Name;
		Name.Format(_T("%s%s%s"), m_Prefix, pGMTxt->m_strText, m_Suffix);
		pCompoM = GetCompoM(Name, GetMSCompoMType(Type));
		if(pCompoM)
			return pCompoM;
	}

	long MyStaggeredDir = MyLine->m_SharePntMask & (MyLineDir==0 ? NM_GAPPED0 : NM_GAPPED1);
	return FindGMTextInLinearWall( (MyStaggeredDir==0 ? 1 : 0), MyStaggered, UrStaggered, Type);
}

MSCompoM* CogReader::GetLinearWallCompoM( DG2DLine* MyLine, DG2DLine* UrLine, long Type )
{
	MSCompoM* pCompoM;
	pCompoM = FindGMTextInLinearWall(0, MyLine, UrLine, Type);
	if(pCompoM==NULL)
		pCompoM = FindGMTextInLinearWall(1, MyLine, UrLine, Type);

	return pCompoM;
}

MSCompoM* CogReader::GetCompoM( CString Name, long Type )
{
 	if(m_pBldg==NULL)	return NULL;
 	CString TheName = Name.MakeUpper();

	bool IsVertical = false;
	int FloorNum = mp_MSFloor->GetFloorNum();
	if(Type == MSCompoM::msColumnM || Type == MSCompoM::msBWallM || Type == MSCompoM::msRWallM)
		IsVertical = true;
	if(Type == MSCompoM::msSteelColumnM || Type == MSCompoM::msSrcColumnM)
		IsVertical = true;

	if(IsVertical == false)
		FloorNum--;
	if(FloorNum == 0)
		FloorNum = -1;
	vector<MSCompoM*>::iterator it;
	for(it=ma_CompoM.begin(); it != ma_CompoM.end(); it++)
	{
		MSCompoM* pCompoM = *it;
		CString CompoMName = pCompoM->m_Name.MakeUpper();
		if(CompoMName==TheName)
		{
			if(pCompoM->m_nFromStory <= FloorNum && FloorNum <= pCompoM->m_nToStory)
				return pCompoM;
		}
	}
	if (m_IsSaveM)
	{
		for (int i = 0; i < ma_NewM.GetSize(); i++)
		{
			if (ma_NewM[i]->m_Name == TheName)
				return ma_NewM[i];
		}
	}
//  	if(Type==MSCompoM::msColumnM || Type==MSCompoM::msBWallM || Type==MSCompoM::msRWallM)
//  	{
//  		XMSRowM aRowM;	aRowM.Initialize();
//  		if(m_RowMMap.Lookup(TheName, aRowM))
//  		{
//  			long nFloorNum = mp_MSFloor->m_nFloorNum;
//  			aRowM.SetOwner(m_pBldg);
//  			long nMID = aRowM.GetIncludeMIDbyFloorNum(nFloorNum);
//  			return m_pBldg->GetCompoM(nMID);
//  		}
//  		return NULL;
//  	}
//  	else
//  	{
//  		vector<MSCompoM*>::iterator it;
//  		for(it=ma_CompoM.begin(); it != ma_CompoM.end(); it++)
//  		{
//  			MSCompoM* pCompoM = *it;
//  			CString CompoMName = pCompoM->m_Name.MakeUpper();
//  			if(CompoMName==TheName)
//  				return pCompoM;
//  		}
//  	}
	return NULL;
}

CString CogReader::GetstrMemberType()
{
	CString strType  = _T("");
	switch(m_nMemberType)
	{
	case DG_GRIDLINE:	return _T("GRIDLINE");
	case DG_COLUMN:		return _T("COLUMN");
	case DG_WALL	:		return _T("BWALL");
	case DG_RWALL	:		return _T("RWALL");
	case DG_BEAM	:		return _T("BEAM");
	case DG_ISOFOOT:	return _T("ISOFOOT");
	case DG_WFOOT:		return _T("WALLFOOT");
	case DG_SLAB:			return _T("SLAB");
	case DG_REBARZONE:	return _T("REBAR");
	case DG_WALL_ENDBARS: return _T("End Rebar");
	case DG_LOWWALL:	return _T("LOWWALL");
	case DG_PIT:	return _T("PIT");
// 	case MSCompoM::msStairM:		return _T("STAIR")
	default:
		ASSERT(0);
	}
	return strType;
}

long CogReader::GetMemberType()
{
	switch(m_nMemberType)
	{
	case DG_COLUMN:		return MSCompoG::msColumnG;
	case DG_WALL	:		return MSCompoG::msWallG;
	case DG_RWALL	:		return MSCompoG::msWallG;
	case DG_BEAM	:		return MSCompoG::msBeamG;
	case DG_ISOFOOT:	return MSCompoG::msIsoFootG;
	case DG_WFOOT:		return MSCompoG::msWallFootG;
	case DG_SLAB:			return MSCompoG::msSlabG;
		// 	case MSCompoM::msStairM:		return _T("STAIR")
	case DG_GRIDLINE:	return DG_GRIDLINE;
	case DG_REBARZONE:	return DG_REBARZONE;
	case DG_OPENING:	return DG_OPENING;
	case DG_WALL_ENDBARS:	return DG_WALL_ENDBARS;
	case DG_LOWWALL: return MSCompoG::msHandRailG;
	case DG_PIT:	return MSCompoG::msPitG;
	default:
		ASSERT(0);
	}
	return -1;
}

void CogReader::DeleteDGMembers()
{
	for(long i=0; i<ma_DGWallLines.GetSize(); i++)
		delete ma_DGWallLines[i];
	ma_DGWallLines.RemoveAll();

	for (long i = 0; i < ma_DGColumnLines.GetSize(); i++)
		delete ma_DGColumnLines[i];
	ma_DGColumnLines.RemoveAll();

	for(i=0; i<ma_DGRWallLines.GetSize(); i++)
		delete ma_DGRWallLines[i];
	ma_DGRWallLines.RemoveAll();

	for(i=0; i<ma_DGBeamLines.GetSize(); i++)
		delete ma_DGBeamLines[i];
	ma_DGBeamLines.RemoveAll();

	for(i=0; i<ma_DGWFootLines.GetSize(); i++)
		delete ma_DGWFootLines[i];
	ma_DGWFootLines.RemoveAll();

// 	for(i=0; i<ma_DGSlabLine.GetSize(); i++)
// 		delete ma_DGSlabLine[i];
// 	ma_DGSlabLine.RemoveAll();

	for(i=0; i<ma_DGWallArcs.GetSize(); i++)
		delete ma_DGWallArcs[i];
	ma_DGWallArcs.RemoveAll();

	for(i=0; i<ma_DGRWallArcs.GetSize(); i++)
		delete ma_DGRWallArcs[i];
	ma_DGRWallArcs.RemoveAll();

	for(i=0; i<ma_DGBeamArcs.GetSize(); i++)
		delete ma_DGBeamArcs[i];
	ma_DGBeamArcs.RemoveAll();

	for(i=0; i<ma_DGWFootArcs.GetSize(); i++)
		delete ma_DGWFootArcs[i];
	ma_DGWFootArcs.RemoveAll();

	for(i=0; i<ma_GridEntity.GetSize(); i++)
		delete ma_GridEntity[i];
	ma_GridEntity.RemoveAll();
}

void CogReader::DeleteSteelColumnDatas()
{
	for (auto it = mm_SteelColumnData.begin(); it != mm_SteelColumnData.end(); it++)
	{
		delete it->first;
	}
	mm_SteelColumnData.clear();
}
void CogReader::DeleteSteelBeamDatas()
{
	for (auto it = ma_SteelBeamData.begin(); it != ma_SteelBeamData.end(); it++)
		delete *it;
	ma_SteelBeamData.clear();
}
/*
void CogReader::ExtractSlab()
{
    CTypedPtrArray<CObArray, MSSlabPolygon*> PGonArr;
    MSFloor* pLowerFloor = mp_MSFloor->GetLowerFloor();
    if(pLowerFloor==NULL)
        return;
    pLowerFloor->FindSlabPolygons(PGonArr, true);
    long nSlab = PGonArr.GetSize();
    for(long iSlab=0; iSlab<nSlab; iSlab++)
    {
        MSSlabPolygon* pGon = PGonArr[iSlab];
        long nConn = pGon->ma_SptNode.GetSize();
        if(nConn<2)
        {
			delete pGon;
			continue;
		}
        MSSlabG* pSlab = new MSSlabG();

		GM2DPolyline* pPoly = pSlab->GetProfile();
		pGon->GetPolygon(*pPoly);		// pPoly에 pGon profile을 넣어줌
		

        MSRcFaceMemberM* pSlabM = NULL;
        CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs =  m_pQT->ma_GMs[DG_TEXT];
        long nText = TxtGMs.GetSize();
        for(long iText = 0; iText < nText; iText++)
        {
            GMText* pText = (GMText*)TxtGMs[iText];
            GM2DVector Vec(pText->m_Vec.m_X, pText->m_Vec.m_Y);
            if(pPoly->Contains(&Vec) < 0)
            {
                pSlabM = (MSRcFaceMemberM*)FindSlabM(pText->m_strText);
                break;
            }
        }
        if(pSlabM == NULL)
        {
            pSlabM = (MSRcFaceMemberM*)m_pBldg->CreateNewCompoM(MSCompoM::msSlabM);
			pSlabM->m_nType = MSCompoM::msSlabM;
            pSlabM->mp_Material = m_pBldg->GetDefMaterial();
			pSlabM->m_Name = _T("NONAME");
            pSlabM->SetThick(150.);
        }
		pSlab->SetOwner(pLowerFloor);
        pSlab->mp_CompoM = pSlabM;

		pSlab->MakeOuterProfileFromFindSlab();

        ma_SlabG.Add(pSlab);

		delete pGon;
    }
	PGonArr.RemoveAll();
}
*/
//void CogReader::ExtractSlab()
//{
//	CTypedPtrArray<CObArray, MSSlabPolygon*> PGonArr;
//	MSFloor* pLowerFloor = mp_MSFloor->GetLowerFloor();
//	if(pLowerFloor==NULL)
//		return;
//	pLowerFloor->FindSlabPolygons(PGonArr, true);
//	long nSlab = PGonArr.GetSize();
//	for(long iSlab=0; iSlab<nSlab; iSlab++)
//	{
//		MSSlabPolygon* pGon = PGonArr[iSlab];
//		long nConn = pGon->ma_SptNode.GetSize();
//		if(nConn<2)
//		{
//			delete pGon;
//			continue;
//		}
//
//		MSSlabG* pSlab = new MSSlabG();
//
//		GM2DPolyline* pPoly = pSlab->GetProfile();
//		pGon->GetPolygon(*pPoly);		// pPoly에 pGon profile을 넣어줌
//		delete pGon;
//
//		MSRcFaceMemberM* pSlabM = NULL;
//		CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs =  m_pQT->ma_GMs[DG_TEXT];
//		long nText = TxtGMs.GetSize();
//		for(long iText = 0; iText < nText; iText++)
//		{
//			GMText* pText = (GMText*)TxtGMs[iText];
//			GM2DVector Vec(pText->m_Vec.m_X, pText->m_Vec.m_Y);
//			if(pPoly->Contains(&Vec) < 0)
//			{
//				pSlabM = (MSRcFaceMemberM*)FindSlabM(pText->m_strText);
//				break;
//			}
//		}
//		if(pSlabM == NULL)
//		{
//			delete pSlab;
//			continue;
//		}
//		pSlab->SetOwner(pLowerFloor);
//		pSlab->mp_CompoM = pSlabM;
//
//		pSlab->MakeOuterProfileFromFindSlab();
//
//		ma_SlabG.Add(pSlab);
//	}
//	PGonArr.RemoveAll();
//}
//
void CogReader::SetFloorQT()
{
	// 도면 인식에 사용된 객체를 사용하여 X,Y 값을 계산하기 때문에 위험한 실재로 입력된 부재보다 크게 쿼드 트리가 생성된다
	// 사용하지 않는것을 권장함
// 	if(m_GMFigureMap.GetSize() > 0)
// 		m_pBldg->ChangFloorQT(m_MinX, m_MinY, m_MaxX, m_MaxY);
}

void CogReader::AddUsedTextByCompoG(GMText* pGMTxt, MSElement* pCompoG)
{
	map<GMText*, vector<MSElement*>>::iterator it = mm_UsedTextToCompoGs.find(pGMTxt);

	vector<MSElement*> compoGs;
	if(it != mm_UsedTextToCompoGs.end())
	{
		it->second.push_back(pCompoG);
	}
	else
	{
		compoGs.push_back(pCompoG);
		mm_UsedTextToCompoGs.insert(make_pair(pGMTxt, compoGs));
	}
}

void CogReader::AddErrorObject_RepeatedName()
{
	map<GMText*, vector<MSElement*>>::iterator it;
	for(it = mm_UsedTextToCompoGs.begin(); it != mm_UsedTextToCompoGs.end(); it++)
	{
		int count = it->second.size();
		if(count > 1)
		{
			AddErrorObject(it->second, W_CODE_REPEAT_NAME, WARNING_LEVEL, GetGroupName(), WARNING_REPEAT_NAME);
			long errFigId;
			m_GMFigureMap.Lookup(it->first, errFigId);
			m_RepeatedNamesToFigIds.push_back(errFigId);
		}
	}
}

void CogReader::ExtractOpenings()
{
	MSFloor* pFromFloor = m_pBldg->GetFloorByID(m_FromFloorID);
	MSFloor* pToFloor = m_pBldg->GetFloorByID(m_ToFloorID);

	CogReaderWallOpeningHelper theHelper(m_pBldg, this, pFromFloor, pToFloor);
	theHelper.MakeData();
}

void CogReader::AddSlabRebar(MSSlabRebar* pRebar)
{
	ma_SlabRebar.Add(pRebar);
}

void CogReader::setDefaultMaterial(CString defaultMaterialName)
{
	MSMaterial* pMaterial = m_pBldg->GetMaterial(defaultMaterialName);
	if (pMaterial)
	{
		m_nMaterialID = pMaterial->m_ID;
		m_IsUsedStoryMaterial = false;
	}
	else
	{
		m_nMaterialID = 0;
		m_IsUsedStoryMaterial = true;
	}
}

bool CogReader::MakeWallEndRebar(CString name, GM2DVector sVec, GM2DVector eVec, vector<CString>& dataList, bool IsStart, int figureID)
{
	int NumData = dataList.size();
	if(NumData == 0)	return false;
	double findTolerance = 250;
	GM2DLineVector aLineVec(sVec, eVec);
	vector<MSCompoG*> compoGArr = MSCompoGUtil::FindCompoG(mp_MSFloor, &aLineVec, FindType_MSElement::msWallG, true, findTolerance);
	int NumCompoG = compoGArr.size();
	vector<MSWallMemberG*> wallGArr;
	for(int iCompoG = 0; iCompoG < NumCompoG; iCompoG++)
	{
		MSCompoG* pCompoG = compoGArr[iCompoG];
		MSWallMemberG* pWallG = dynamic_cast<MSWallMemberG*>(pCompoG);
		if(pWallG != NULL)
		{
			shared_ptr<GM2DCurve> pCurve = pWallG->GetCenCurve();
			GM2DLineVector* pLineVec = dynamic_cast<GM2DLineVector*>(&*pCurve);
			if(pLineVec != NULL)
			{
				if(pLineVec->IsSameLineVector(aLineVec, findTolerance))
					wallGArr.push_back(pWallG);
			}
		}
	}
	int NumWallG = wallGArr.size();
	if(NumWallG == 0)	return false;
	if(!CheckStory(dataList))
	{
		vector<long> figureIDs;	figureIDs.push_back(figureID);
		CString strMsg;	strMsg.Format(_T("베이스도면의 층 범위가 모델을 벗어납니다."));
		AddErrorObject(figureIDs, STORY_OUT_OF_RANGE, ERROR_LEVEL, GetGroupName(), strMsg);
		return false;
	}
	MSRebarBuilding* pRebarBuilding = dynamic_cast<MSRebarBuilding*>(m_pBldg);
	if (pRebarBuilding == nullptr) return false;

	MSWallEndRebarLine* pWallEndRebarLine = pRebarBuilding->CreateNewWallEndRebarLine();

	CString sStory, sRebar;
	MTReader aReader;
	aReader.SetBuilding(m_pBldg);
	aReader.ma_StorySeperate.push_back(_T("~"));
	for(int iData = 0; iData < NumData; iData++)
	{
		CString strData = dataList[iData];
		if(!Parsing_WallData(strData, sStory, sRebar))	continue;

		long nFrom, nTo;
		aReader.Parsing_strStory(sStory, nFrom, nTo);

		MSWallEndRebarM* pEndRebarM = pWallEndRebarLine->CreateWallEndRebarM(nFrom, nTo, sRebar);
		pWallEndRebarLine->Add(pEndRebarM);
	}

	double DistanceP0 = 0, DistanceP1 = 0;
	for(int iWallG = 0; iWallG < NumWallG; iWallG++)
	{
		MSWallMemberG* pWallG = wallGArr[iWallG];
		if(pWallG != NULL)
		{
			CString MName = pWallG->GetCompoM()->GetName();
			if(MName != name)
			{
				vector<long> figureIDs;	figureIDs.push_back(figureID);
				CString strMsg;	strMsg.Format(_T("베이스도면과 모델링 정보가 다릅니다.(%s, %s)"), name, MName);
				AddErrorObject(figureIDs, STORY_OUT_OF_RANGE, ERROR_INFO, GetGroupName(), strMsg);
				continue;
			}
			GM2DVector* pWallGSVec = pWallG->GetSVec();
			GM2DVector* pWallGEVec = pWallG->GetEVec();
			if(IsStart)
			{
				DistanceP0 = sVec.Distance(*pWallGSVec);
				DistanceP1 = sVec.Distance(*pWallGEVec);
			}
			else
			{
				DistanceP0 = eVec.Distance(*pWallGSVec);
				DistanceP1 = eVec.Distance(*pWallGEVec);
			}

			map<MSWallMemberG*, MSWallEndRebarInfo*>::iterator it = mm_WallEndRebarInfo.find(pWallG);
			if(it != mm_WallEndRebarInfo.end())
			{
				if(DistanceP0 < DistanceP1)
					it->second->mp_StartEndRebarLine = pWallEndRebarLine;
				else
					it->second->mp_EndEndRebarLine = pWallEndRebarLine;
			}
			else
			{
				MSWallEndRebarInfo* pInfo = new MSWallEndRebarInfo;
				if(DistanceP0 < DistanceP1)
					pInfo->mp_StartEndRebarLine = pWallEndRebarLine;
				else
					pInfo->mp_EndEndRebarLine = pWallEndRebarLine;
				mm_WallEndRebarInfo.insert(make_pair(pWallG, pInfo));
			}
		}
	}

	return true;
}

bool CogReader::CheckStory(vector<CString>& DataList)
{
	int LimitFrom = -(m_pBldg->GetNumUnderStory()-1);
	int LimitTo = m_pBldg->GetNumAboveStory()-1;

	MTReader aReader;
	aReader.SetBuilding(m_pBldg);
	aReader.ma_StorySeperate.push_back(_T("~"));
	CString sStroy, sRebar;
	int nData = DataList.size();
	for(int iData = 0; iData < nData; iData++)
	{
		CString strData = DataList[iData];
		if(!Parsing_WallData(strData, sStroy, sRebar))	return false;
		
		long nFrom, nTo;
		aReader.Parsing_strStory(sStroy, nFrom, nTo);
		if(nFrom == 0 || nTo == 0)	return false;
//		if(nFrom < LimitFrom || nTo	 > LimitTo)		return false;
	}
	return true;
}

bool CogReader::Parsing_WallData(CString strData, CString& sStory, CString& sRebar)
{
	sStory = sRebar = _T("");
	int indexColon = strData.Find(_T("="));
	if(indexColon == -1)	return false;

	sStory = strData.Mid(0, indexColon);
	sRebar = strData.Mid(indexColon+1, strData.GetLength() - (indexColon+1));
	return true;
}

void CogReader::ExtractPits()
{

}

CString CogReader::MakeDefaultName(CString text)
{
	if (m_IsFindMName == false && m_DefaultMName != _T(""))
		return m_DefaultMName;

	CString defaultName = _T("");
	if (m_eRecognizeMode == CogReader::Formwork)
		return _T("NONAME");

	defaultName.Format(_T("%s%s%s"), m_Prefix, text, m_Suffix);
	return defaultName;
}

CString CogReader::MakeDefaultName(long width, long depth, long thick)
{
	if (m_IsFindMName == false && m_DefaultMName != _T(""))
		return m_DefaultMName;

	CString defaultName = _T("");
	if (m_eRecognizeMode == CogReader::Formwork)
		return _T("NONAME");

	switch (m_nMemberType)
	{
	case DG_BEAM:			defaultName.Format(_T("%sG1(%ld X %ld)"), m_DefNamePrefix, width, depth);	break;
	case DG_COLUMN:
	{
		if (depth == 0)
			defaultName.Format(_T("%sC1(%ld)"), m_DefNamePrefix, width);
		else
			defaultName.Format(_T("%sC1(%ld X %ld)"), m_DefNamePrefix, width, depth);
		break;
	}
	case DG_WALL:			defaultName.Format(_T("%sW1(%ld)"), m_DefNamePrefix, width);						break;
	case DG_RWALL:		defaultName.Format(_T("%sRW1(%ld)"), m_DefNamePrefix, width);						break;
	case DG_ISOFOOT:		defaultName.Format(_T("%sF1(%ld x %ld x %ld)"), m_DefNamePrefix, width, depth, thick);	break;
	case DG_WFOOT:		defaultName.Format(_T("%sWF1(%ld x %ld)"), m_DefNamePrefix, width, depth);						break;
	case DG_LOWWALL:	defaultName.Format(_T("%sHR1(%ld)"), m_DefNamePrefix, width);						break;
	case DG_SLAB:			defaultName.Format(_T("%sS1(%ld)"), m_DefNamePrefix, depth);						break;
	}
	
	defaultName.MakeUpper();
	defaultName.Replace(_T(" "), NULL);

	return defaultName;
}
void CogReader::MakeErrorRange(GM2DLineVector* pRebarLine, eSlabError errorType, double extendDist)
{
	GM2DLineVector centerLineVec;
	centerLineVec.SetP0(pRebarLine->m_P0);
	centerLineVec.SetP1(pRebarLine->m_P1);

	centerLineVec.ExtendLineVector(true, extendDist, false);
	centerLineVec.ExtendLineVector(false, extendDist, true);

	GM2DLineVector bottomLineVec = centerLineVec.NormalTranslate(-extendDist);
	GM2DLineVector topLineVec = centerLineVec.NormalTranslate(extendDist);

	GM2DPolyline* pPolyline = new GM2DPolyline();
	GM2DVector* pLBVec = new GM2DVector(bottomLineVec.m_P0);
	GM2DVector* pRBVec = new GM2DVector(bottomLineVec.m_P1);
	GM2DVector* pRTVec = new GM2DVector(topLineVec.m_P1);
	GM2DVector* pLTVec = new GM2DVector(topLineVec.m_P0);

	pPolyline->Add(pLBVec);
	pPolyline->Add(pRBVec);
	pPolyline->Add(pLTVec);
	pPolyline->Add(pRTVec);

	pPolyline->ma_Bulge.Add(0);
	pPolyline->ma_Bulge.Add(0);
	pPolyline->ma_Bulge.Add(0);

	mm_SlabRebarError.insert(make_pair(pPolyline, errorType));
}

void CogReader::DeleteErrorRanges()
{
	map<GM2DPolyline*, eSlabError>::iterator it;
	for (it = mm_SlabRebarError.begin(); it != mm_SlabRebarError.end(); it++)
		delete it->first;
	mm_SlabRebarError.clear();
}

void CogReader::SetSlabRebarOption_LineFilter(CString rebarLayer, CString infoLayer, CString rangeLayer, int leaderDist)
{
	mp_SlabRebarOption->SetRebarLayer(rebarLayer);
	mp_SlabRebarOption->SetInfoLayer(infoLayer);
	mp_SlabRebarOption->SetRangeLayer(rangeLayer);
	mp_SlabRebarOption->SetLeaderDist(leaderDist);
}

void CogReader::SetSlabRebarOption_TopBottomFilter(int topBottomFilter, CString topBarLineType, CString botBarLineType,
	CString topBarText, CString botBarText, CString botBarLayer)
{
	mp_SlabRebarOption->SetTopBottomFilter((SlabRebarOption::eTopBottomFilter)topBottomFilter);
	mp_SlabRebarOption->SetTopLineType(topBarLineType);
	mp_SlabRebarOption->SetBotLineType(botBarLineType);
	mp_SlabRebarOption->SetTopBarTextInfo(topBarText);
	mp_SlabRebarOption->SetBotBarTextInfo(botBarText);
	mp_SlabRebarOption->SetBotRebarLayer(botBarLayer);
}
void CogReader::SetSlabRebarOption_TextFilter(int textDist, bool isUsedDefaultBar, CString defaultBar)
{
	mp_SlabRebarOption->SetLeaderDistToText(textDist);
	mp_SlabRebarOption->SetIsUsedDefaultBar(isUsedDefaultBar);
	mp_SlabRebarOption->SetDefaultBar(defaultBar);
}
void CogReader::SetSlabRebarOption_MatchingFilter(bool isUsedMatchingText, map<CString, CString>& dicMatchingText)
{
	mp_SlabRebarOption->SetIsUsedMatchingText(isUsedMatchingText);
	for (auto it = dicMatchingText.begin(); it != dicMatchingText.end(); it++)
		mp_SlabRebarOption->SetMatchingText(it->first, it->second);
}

MSCompoM::Type_MSCompoM CogReader::GetMSCompoMType()
{
	return (MSCompoM::Type_MSCompoM)GetMSCompoMType(m_nMemberType);
}

void CogReader::SetNameOptions(bool isFindMName, CString defaultMName)
{
	m_IsFindMName = isFindMName;
	m_DefaultMName = defaultMName;
	if (m_DefaultMName == _T(""))	m_IsFindMName = false;
}
MSCompoM* CogReader::GetDefaultCompoM()
{
	if (m_IsFindMName)	return nullptr;
	return GetCompoM(m_DefaultMName, GetMSCompoMType());
}

void CogReader::ExtractSlabs()
{
// 	// Test Code
// 	mp_SlabHelper->SetIsUsedThickOption(true);
// 	mp_SlabHelper->AddSlabThickOptionText(_T("EXCEPT"), CogReaderTextArray::Except);
// 	mp_SlabHelper->AddSlabThickOptionText(_T("THK:"), CogReaderTextArray::Same);
// 	mp_SlabHelper->AddSlabThickOptionText(_T("INCLUDE"), CogReaderTextArray::Include);
// 
// 	mp_SlabHelper->SetIsUsedOffsetOption(true);
// 	mp_SlabHelper->AddSlabOffsetOptionText(_T("OFFSET:"), CogReaderTextArray::Same);
// 	// 

	long errorCode = E_CODE_POLYLINE_NOT_CLOSED;
	long level = ERROR_LEVEL;
	bool isThick = true;
	bool isOffset = false;
	CString groupName = GetGroupName();
	CString msg;
	msg.Format(_T("%s"), LocalDic::GetLocal(_T("슬래브 생성에 실패 하였습니다.")));
	for (int i = 0; i < ma_SlabPolyline.GetSize(); i++)
	{
		GM2DPolyline* pPoly = ma_SlabPolyline[i];
		if (!pPoly->IsClosed())
		{
			auto it = mm_SlabPolylineToFigureID.find(pPoly);
			if (it != mm_SlabPolylineToFigureID.end())
			{
				vector<long> figureIDs;	figureIDs.push_back(it->second);
				AddErrorObject(figureIDs, errorCode, level, groupName, msg);
			}
			continue;
		}
		double thick = 0.0, offset = 0.0;

		if(!IsFindText(pPoly, thick, isThick))
			thick = mp_SlabHelper->GetDefaultSlabThick();
		if(!IsFindText(pPoly, offset, isOffset))
			offset = mp_SlabHelper->GetDefaultSlabOffset();

		CString defName = MakeDefaultName(thick);
		MSCompoM* pSlabM = CreateNewSlabM(thick, defName);

		MSSlabG* pSlabG = (MSSlabG*)m_pBldg->CreateNewCompoG(MSElement::msSlabG);
		pSlabG->SetOwner(mp_MSFloor);
		pSlabG->SetCompoM(pSlabM);
		pSlabG->SetProfile(pPoly);
		pSlabG->m_dOffset = offset;
		ma_SlabG.Add(pSlabG);
	}
}

bool CogReader::IsFindText(GM2DPolyline* pPoly, double& outData, bool isThick)
{
	bool IsFind = false;
	CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = m_pQT->ma_GMs[DG_TEXT];
	for (int i = 0; i < TxtGMs.GetSize(); i++)
	{
		GMText* pText = dynamic_cast<GMText*>(TxtGMs[i]);
		if (pText == nullptr)		continue;
		if (!pPoly->Contains(pText))	continue;

		if (!mp_SlabHelper->AnalyzeText(pText->m_strText, outData, isThick))	continue;
		if (IsFind == false)	IsFind = true;
		else
		{
			outData = 0.0;
			return false;
		}
	}
	return IsFind;
}

void CogReader::SetDefaultSlabThick(double thick)
{
	mp_SlabHelper->SetDefaultSlabThick(thick);
}
void CogReader::SetDefaultSlabOffset(double offset)
{
	mp_SlabHelper->SetDefaultSlabOffset(offset);
}

void CogReader::SetIsUsedSlabThickOption(bool isUsed)
{
	mp_SlabHelper->SetIsUsedThickOption(isUsed);
}
void CogReader::SetIsUsedSlabOffsetOption(bool isUsed)
{
	mp_SlabHelper->SetIsUsedOffsetOption(isUsed);
}
void CogReader::AddSlabThickOptionText(CString text, CogReaderTextArray::eTextType textType)
{
	mp_SlabHelper->AddSlabThickOptionText(text, textType);
}
void CogReader::AddSlabOffsetOptionText(CString text, CogReaderTextArray::eTextType textType)
{
	mp_SlabHelper->AddSlabOffsetOptionText(text, textType);
}

///////////////////////////////////// MSGridEntity//////////////////////////////////////////////
MSGridEntity::MSGridEntity( void )
{
	mp_LVec = NULL;
}

MSGridEntity::~MSGridEntity( void )
{
	if(mp_LVec)
		delete mp_LVec;
}

///////////////////////////////////// MSGridTextInfo //////////////////////////////////////////////
MSGridTextInfo::MSGridTextInfo()
{
	m_pText = NULL;
	m_pTextCircle = NULL;
	m_pGuideLine = NULL;
	m_pConnectedGuideLine = NULL;
}

MSGridTextInfo::~MSGridTextInfo()
{

}

//////////////////////////////////// MSWallEndRebarInfo ///////////////////////////////////////////
MSWallEndRebarInfo::MSWallEndRebarInfo()
{
	mp_StartEndRebarLine = NULL;
	mp_EndEndRebarLine = NULL;
}

MSWallEndRebarInfo::~MSWallEndRebarInfo()
{

}
