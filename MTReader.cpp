#include "StdAfx.h"
#include "MTReader.h"
#include "SRSectionTable.h"
#include "SRSectionBox.h"

#include "GMLib/GMText.h"
#include "GMLib/GM2DLineVector.h"
#include "GMLib/GM2DRectangle.h"
#include "GMLib/XErrorObject.h"
#include "StructureModel/MSRebarBuilding.h"
#include "StructureModel/MSFloor.h"
#include "StructureModel/MSConcrete.h"
#include "StructureModel/MRCompoM.h"
#include "StructureModel/MSRcLineMemberM.h"
#include "StructureModel/MSRcFaceMemberM.h"
#include "StructureModel/MSRcColumnM.h"
#include "StructureModel/MRRcSection.h"
#include "StructureModel/MSRcBeamM.h"
#include "StructureModel/MSRcBWallM.h"
#include "StructureModel/MSRcBeamSec.h"
#include "StructureModel/MDRcButtressM.h"
#include "StructureModel/MDRcButtressSec.h"
#include "StructureModel/MSRcSlabM.h"
#include "StructureModel/MSRcStrip.h"
#include "StructureModel/MREdge.h"
#include "StructureModel/MRHBarByPoint.h"
#include "StructureModel/MRHBarPointByLoc.h"
#include "StructureModel/MRVBar.h"
#include "MSDeformedBar.h"
#include "StructureModel/MSSpecificTieBar.h"
#include "StructureModel/MSSteelUtility.h"
#include "StructureModel/MSStoryMaterial.h"

#include "StructureModel/MSSteelBeamM.h"
#include "StructureModel/MSSteelColumnM.h"
#include "StructureModel/MSSrcBeamM.h"
#include "StructureModel/MSSrcColumnM.h"
#include "StructureModel/MDSrcBeamM.h"
#include "StructureModel/MDSrcColumnM.h"

#include "MTRcColumnMHelper.h"
#include "MTSteelColumnMHelper.h"
#include "MTSrcColumnMHelper.h"
#include "MTSrcBeamMHelper.h"
#include "MTSteelBeamMHelper.h"
#include "MTRWallMHelper.h"
#include "MTButtressMHelper.h"

#include "MTDisplayRcColumnMHelper.h"
#include "MTDisplaySteelColumnMHelper.h"
#include "MTDisplaySrcColumnMHelper.h"

#include "MTDisplayRcBeamMHelper.h"
#include "MTDisplaySteelBeamMHelper.h"
#include "MTDisplaySrcBeamMHelper.h"
#include "MTRcSlabMHelper.h"
#include "MTBWallMHelper.h"

#include "MTDisplayWallMHelper.h"
#include "MTDisplaySlabMHelper.h"
#include "MTDisplayButtressMHelper.h"
#include "MTDisplayRWallMHelper.h"
#include "MTDisplayWallMHelper_LH.h"

int MTError::AddCopyArea(GM2DRectangle* pSource)
{
	if (pSource)
	{
		GM2DRectangle* pRect = new GM2DRectangle;
		pRect->m_Origin.SetXY(pSource->m_Origin.m_X, pSource->m_Origin.m_Y);
		pRect->m_Corner.SetXY(pSource->m_Corner.m_X, pSource->m_Corner.m_Y);
		return ma_Rect.Add(pRect);
	}
	return -1;
}

int MTOverlapArea::AddCopyArea(GM2DRectangle* pSource)
{
	if (pSource)
	{
		GM2DRectangle* pRect = new GM2DRectangle();
		pRect->CopyFrom(pSource);
		return ma_Area.Add(pRect);
	}
	return -1;
}

#pragma region MTRecognizeTextFilter
void MTRecognizeTextFilter::Initialize()
{
	m_CellType = MTReader::ctNone;
	m_eFilterType = eFilterType::Contain;
}

int MTRecognizeTextFilter::AnalyizeText(CString text)
{
	for (int i = 0; i < ma_TextList.size(); i++)
	{
		CString filterText = ma_TextList[i];
		switch (m_eFilterType)
		{
		case MTRecognizeTextFilter::Contain:
		{
			if (text.Find(filterText) != -1)	return m_CellType;
			break;
		}
		case MTRecognizeTextFilter::Prefix:
		{
			auto prefix = text.Left(filterText.GetLength());
			if (prefix == filterText)	return m_CellType;
			break;
		}
		case MTRecognizeTextFilter::Suffix:
			auto suffix = text.Right(filterText.GetLength());
			if (suffix == filterText)	return m_CellType;
			break;
		}
	}
	return 0;
}

#pragma endregion


MTReader::MTReader(void)
{
	mp_Bldg = NULL;
	m_Group = GROUP_COLUMN;
	m_Type = TABLE_1;
	m_bHorType = TRUE;
	m_dTol = 10;
	m_nMaterialID = 0;
	m_IsUsedStoryMaterial = true;
	m_IsBoxOnly = true;
	m_ReplceType = KEEP_AND_REPLACE;
	m_IsUsedStoryMaterial = true;
	m_MaterialType = MSMaterial::msConcrete;
	m_IsMakeBuiltUp = false;
	m_eRecognizeEndHoopType = MTReader::CenEndHoop;
}

MTReader::~MTReader(void)
{
	DeleteTableArr();
	DeleteGMTextArr();
	DeleteCellTypeGMTextMap();
	DeleteCellTypeTextFilter();
	DeleteRecognizeBoxs();
	DeleteDisplaySectionRects();
	DeleteOverlapAreas();
	DeletePolylineArr();
	DeleteRecognizeSectionArr();
	DeleteMTErrorArr();
	DeleteResultMArr();
}
void MTReader::DeleteCellTypeTextFilter()
{
	for (auto it = ma_TextFilter.begin(); it != ma_TextFilter.end(); it++)
	{
		delete* it;
	}
	ma_TextFilter.clear();
}
void MTReader::DeleteResultMArr()
{
	for (int i = 0; i < ma_ResultM.size(); i++)
		delete ma_ResultM[i];
	ma_ResultM.clear();
}
void MTReader::DeleteMTErrorArr()
{
	int NumError = ma_Error.GetSize();
	for (int iError = 0; iError < NumError; iError++)
		delete ma_Error[iError];
	ma_Error.RemoveAll();
}

void MTReader::DeleteRecognizeSectionArr()
{
	map<GM2DRectangle*, MTRecognizeSection*>::iterator it;
	for (it = mm_RectToSection.begin(); it != mm_RectToSection.end(); it++)
	{
		delete it->second;
	}
	mm_RectToSection.clear();
}

void MTReader::DeletePolylineArr()
{
	int NumObj = ma_Polyline.size();
	for (int iObj = 0; iObj < NumObj; iObj++)
		delete ma_Polyline[iObj];
	ma_Polyline.clear();
}
void MTReader::DeleteGMTextArr()
{
	INT_PTR  NumObj = ma_GMText.GetSize();
	for (int iObj = 0; iObj < NumObj; iObj++)
		if (ma_GMText[iObj])
			delete ma_GMText[iObj];
	ma_GMText.RemoveAll();
}

void MTReader::DeleteTableArr()
{
	INT_PTR NumObj = ma_Table.GetSize();
	for (int iObj = 0; iObj < NumObj; iObj++)
		if (ma_Table[iObj])
			delete ma_Table[iObj];
	ma_Table.RemoveAll();
}

void MTReader::DeleteCellTypeGMTextMap()
{
	map<long, vector<GMText*>>::iterator it;
	for (it = mm_CellTypeGMTexts.begin(); it != mm_CellTypeGMTexts.end(); it++)
	{
		vector<GMText*> TextArr = it->second;
		int textCount = TextArr.size();
		for (int i = 0; i < textCount; i++)
			delete TextArr[i];
		TextArr.clear();
	}
	mm_CellTypeGMTexts.clear();
}

void MTReader::DeleteRecognizeBoxs()
{
	for (auto it = mm_RecognizeBox.begin(); it != mm_RecognizeBox.end(); it++)
	{
		GM2DRectangle* Rect = it->second;
		delete Rect;
	}
	mm_RecognizeBox.clear();
}

void MTReader::DeleteDisplaySectionRects()
{
	for (auto it = ma_DisplaySectionRect.begin(); it != ma_DisplaySectionRect.end(); it++)
	{
		delete* it;
	}
	ma_DisplaySectionRect.clear();
}

void MTReader::DeleteOverlapAreas()
{
	map<CString, MTOverlapArea*>::iterator it;
	for (it = mm_Overlap.begin(); it != mm_Overlap.end(); it++)
		delete it->second;
	mm_Overlap.clear();
}

void MTReader::Init()
{
	DeleteTableArr();
	DeleteGMTextArr();
	DeleteCellTypeGMTextMap();
	DeleteCellTypeTextFilter();
	DeleteRecognizeBoxs();
	DeleteDisplaySectionRects();
	DeleteOverlapAreas();
	DeletePolylineArr();
	DeleteMTErrorArr();
	SetMaterialMap();
	mm_LayerNameToCellTypes.clear();

	ma_CreateMID.RemoveAll();
	ma_ReplaceMID.RemoveAll();
	ma_DeletedName.RemoveAll();
	//	ma_ErrorInfo.RemoveAll();
	ma_NewM.RemoveAll();

	ma_NameSeperate.clear();
	ma_StorySeperate.clear();
	ma_FlatSlabType.clear();
	m_RemoveNameOption = 0;

	m_SlabX3Y3Type = SLAB_X3Y3_TYPE_BENT;		// 0 : Bent, 1:Cut
	m_nReadType = eReadType::General;

	m_VMark_LH = _T("V");
	m_HMark_LH = _T("H");
	m_DefaultWallThick_LH = 0;

	m_eBeamSectionType = eBeamSectionType::Small_Mid_Large;
	m_eSizeType = eSizetype::Width_X_Depth;
	m_eRecognizeSizeType = eRecognizeSizeType::NONE;
	m_eRecognizeStoryType = eRecognizeStoryType::FloorNum;
	m_eRecognizeEndHoopType = eRecognizeEndHoopType::CenEndHoop;
}
void MTReader::SetFlatSlabTypeOption(vector<CString> textList)
{
	ma_FlatSlabType.clear();
	ma_FlatSlabType = textList;
}
bool MTReader::IsFlatSlabType(CString text)
{
	for (int i = 0; i < ma_FlatSlabType.size(); i++)
	{
		if (ma_FlatSlabType[i] == text)
			return true;
	}
	return false;
}

void MTReader::AddSubBeamFlag(CString sFlag)
{
	CString sData = sFlag.MakeUpper();
	ma_SubBeamFlag.push_back(sData);
}

void MTReader::AddSubBeamExcludeFlag(CString sFlag)
{
	CString sData = sFlag.MakeUpper();
	ma_SubBeamExcludeFlag.push_back(sData);
}

long MTReader::GetCellTypebyDictionary(CString strKey)
{
	CString strCell = strKey.MakeUpper();
	strCell.Replace(_T(" "), _T(""));
	map<CString, long>::iterator it = mm_CellTypeDictionary.find(strCell);
	if (it == mm_CellTypeDictionary.end())	return 0;
	else
		return it->second;
}
void MTReader::SetCellTypebyDictionary(CString strKey, int cellType)
{
	mm_CellTypeDictionary.insert(make_pair(strKey, cellType));
}

void MTReader::setCellTypeTextFilter(int cellType, int filterType, vector<CString> textList)
{
	MTRecognizeTextFilter* textFilter = new MTRecognizeTextFilter();
	textFilter->SetCellType(cellType);
	textFilter->SetFilterType(filterType);
	textFilter->SetTextList(textList);

	ma_TextFilter.push_back(textFilter);
}

CString MTReader::GetStringByCellType(CellType celltype)
{
	CString str = _T("");
	map<CString, long>::iterator it;
	for (it = mm_CellTypeDictionary.begin(); it != mm_CellTypeDictionary.end(); it++)
	{
		if (it->second == (int)celltype)
		{
			str = it->first;
			return str;
		}
	}
	return str;
}

long MTReader::GetCellType(GMText* theText, bool isFlatSlab)
{
	MTReader::CellType cellType = GetCellTypeByLayer(theText);
	if (cellType != MTReader::ctNone)	return cellType;

	cellType = GetCellTypeByLocation(theText, isFlatSlab);
	if (cellType != MTReader::ctNone)	return cellType;

	return GetCellTypeByTextFilter(theText);
}
GMText* MTReader::GetCellTypeGMText(int cellType)
{
	auto it = mm_CellTypeGMTexts.find(cellType);
	if (it != mm_CellTypeGMTexts.end())
	{
		if (it->second.size() > 0)
			return it->second[0];
	}
	return nullptr;
}

MTReader::CellType MTReader::GetCellTypeByLocation(GMText* theText, bool isFlatSlab)
{
	double dVal = 0.;
	GM2DVector TheVec = theText->m_Vec;

	bool bCalcX = false;
	if (m_Group == GROUP_WALL || m_Group == GROUP_SLAB)
		bCalcX = true;
	if (m_MaterialType == MSMaterial::msSteel)
		bCalcX = true;
	double dMinDist = 10e10;
	long nRtnType = 0;
	long nCellType = 0;
	double dLoc = 0.;
	double minValue = theText->m_Origin.m_Y;
	double maxValue = theText->m_Corner.m_Y;
	if (bCalcX)
	{
		minValue = theText->m_Origin.m_X;
		maxValue = theText->m_Corner.m_X;
	}
	double midValue = minValue + (maxValue - minValue) / 2;

	double cellMinValue = 0, cellMaxValue = 0;

	CMap<double, double, long, long>* locCellTypeMap;
	if (isFlatSlab)
		locCellTypeMap = &m_LocCellTypeMapFlatSlab;
	else
		locCellTypeMap = &m_LocCellTypeMap;

	POSITION Pos = locCellTypeMap->GetStartPosition();
	while (Pos)
	{
		locCellTypeMap->GetNextAssoc(Pos, dLoc, nCellType);
		GMText* pCellText = NULL;

		map<long, vector<GMText*>>::iterator it = mm_CellTypeGMTexts.find(nCellType);
		if (it == mm_CellTypeGMTexts.end())
			continue;

		vector<GMText*> TextArr = it->second;

		int textCount = TextArr.size();
		for (int i = 0; i < textCount; i++)
		{
			pCellText = TextArr[i];
			cellMinValue = pCellText->m_Origin.m_Y;
			cellMaxValue = pCellText->m_Corner.m_Y;
			if (bCalcX)
			{
				cellMinValue = pCellText->m_Origin.m_X;
				cellMaxValue = pCellText->m_Corner.m_X;
			}
			double cellMidValue = cellMinValue + (cellMaxValue - cellMinValue) / 2;
			bool bFind = false;
			if (cellMinValue <= minValue && minValue <= cellMaxValue)
				bFind = true;
			else if (cellMinValue <= maxValue && maxValue <= cellMaxValue)
				bFind = true;
			else if (minValue <= cellMinValue && cellMinValue <= maxValue)
				bFind = true;
			else if (minValue <= cellMaxValue && cellMaxValue <= maxValue)
				bFind = true;

			if (bFind)
			{
				double dist = fabs(midValue - cellMidValue);
				if (dist < dMinDist)
				{
					dMinDist = dist;
					nRtnType = nCellType;
				}
			}
		}
	}
	return MTReader::CellType(nRtnType);
}

MTReader::CellType MTReader::GetCellTypeByTextFilter(GMText* theText)
{
	for (int i = 0; i < ma_TextFilter.size(); i++)
	{
		MTReader::CellType cellType = (MTReader::CellType)ma_TextFilter[i]->AnalyizeText(theText->m_strText);
		if (cellType != MTReader::ctNone)	return cellType;
	}
	return MTReader::ctNone;
}

void MTReader::setDefaultMaterial(CString defaultMaterialName)
{
	MSMaterial* pMaterial = mp_Bldg->GetMaterial(defaultMaterialName);
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

bool MTReader::IsPlainConcreteReinforceBar()
{
	int fromStory = -(mp_Bldg->GetNumUnderStory());
	if (IsVerticalMember())	fromStory++;
	if (fromStory == 0)	fromStory++;
	int materialID = m_nMaterialID;
	if (m_IsUsedStoryMaterial)
	{
		MSStoryMaterial* pStoryMaterial = mp_Bldg->GetStoryMaterialByFloorNum(fromStory);
		if (pStoryMaterial == nullptr) return false;

		if (IsVerticalMember())
			materialID = pStoryMaterial->GetRcVerticalMaterialID();
		else
			materialID = pStoryMaterial->GetRcHorizontalMaterialID();

	}
	MSMaterial* pMaterial = mp_Bldg->GetMaterial(materialID);
	if (pMaterial == nullptr)	return false;
	return pMaterial->IsPlainConcreteReinforceBar();
}
void MTReader::SetMaterialMap()
{
	mm_Material.RemoveAll();
	map<long, MSMaterial*>::iterator it;
	for (it = mp_Bldg->GetMaterialMap()->begin(); it != mp_Bldg->GetMaterialMap()->end(); it++)
	{
		MSMaterial* pMSMate = it->second;
		if (pMSMate->GetType() != MSMaterial::msConcrete)	continue;
		MSConcrete* pMSConc = (MSConcrete*)pMSMate;
		long nKind = GetMaterialKind(pMSConc->m_ConcKind);
		mm_Material.SetAt(nKind, pMSConc->m_ID);
	}
}
long MTReader::GetMaterialKind(CString strKind)
{
	CString strTemp;
	strTemp = strKind.Mid(1, strKind.GetLength() - 1);
	return _ttol(strTemp);
}

SRSectionTable* MTReader::FindSectionTable(GM2DVector* pVec1, GM2DVector* pVec2)
{
	long nTable = ma_Table.GetSize();
	for (long iTable = 0; iTable < nTable; iTable++)
	{
		SRSectionTable* pTable = ma_Table[iTable];
		if (pTable->Contains(pVec1, pVec2))
			return pTable;
	}
	return NULL;
}
void MTReader::FindStoryInfo(SRSectionBox* pSectionBox)
{
	double dMinYc = pSectionBox->mp_Rect->m_Origin.m_Y;
	double dMaxYc = pSectionBox->mp_Rect->m_Corner.m_Y;
	long nText = ma_GMText.GetSize();

	for (long iText = 0; iText < nText; iText++)
	{
		GM2DVector TextVec = ma_GMText[iText]->m_Vec;
		if (dMinYc < TextVec.m_Y && TextVec.m_Y < dMaxYc)
		{
			long nFrom = 0, nTo = 0;
			Parsing_strStory(ma_GMText[iText]->m_strText, nFrom, nTo);
			if (nFrom != 0 && nTo != 0)
			{
				pSectionBox->m_nFromStory = nFrom;
				pSectionBox->m_nToStory = nTo;
				break;
			}
		}
	}
}

GMText* MTReader::GetParentSectionName(SRSectionTable* pTable)
{
	GM2DVector MyCenVec;
	pTable->mp_Rect->GetCenter(MyCenVec);
	double dCompareVal = 0.0;
	if (m_Type == 0)	dCompareVal = MyCenVec.m_X;
	else				dCompareVal = MyCenVec.m_Y;
	long nTable = ma_Table.GetSize();
	for (long iTable = 0; iTable < nTable; iTable++)
	{
		SRSectionTable* TheTable = ma_Table[iTable];
		if (TheTable == pTable)	continue;
		if (TheTable->mp_Name == NULL)	continue;

		double dMinVal = 0.0, dMaxVal = 0.0;
		if (m_Type == 0)
		{
			dMinVal = TheTable->mp_Rect->m_Origin.m_X;
			dMaxVal = TheTable->mp_Rect->m_Corner.m_X;
		}
		else
		{
			dMinVal = TheTable->mp_Rect->m_Origin.m_Y;
			dMaxVal = TheTable->mp_Rect->m_Corner.m_Y;
		}
		if (dMinVal <= dCompareVal && dCompareVal <= dMaxVal)
			return TheTable->mp_Name;
	}
	return NULL;
}
CString MTReader::GetBoxLayerName()
{
	return DG_STR_BOX;
}

void MTReader::MakeWallInfoArr(CTypedPtrArray<CObArray, SRWallInfo*>& WallInfoArr)
{
	int numText = ma_GMText.GetSize();
	for (int iText = 0; iText < numText; iText++)
	{
		GMText* pText = ma_GMText[iText];
		int nCellType = GetCellType(pText);
		if (nCellType == MTReader::ctStory)
		{
			SRWallInfo* pWallInfo = MakeWallInfo(pText, nCellType);
			if (pWallInfo)	WallInfoArr.Add(pWallInfo);
		}
	}
}

SRWallInfo* MTReader::MakeWallInfo(GMText* pText, long nCellType)
{
	CString strData = pText->m_strText;
	//		enum CellType{ctName=1, ctStory, ctSize, ctMaterial, ctThick, ctWidth, ctDepth, ctMainBar, ctVBar, ctHBar, ctSection, ctTBar, ctBBar, ctStirrup, ctSBar1, ctSBar2, ctLoc, ctTieBar};

	double dLoc = 0;
	double dTol = pText->m_dHeight;

	long fromStory = 0, toStory = 0;
	Parsing_strStory(strData, fromStory, toStory);
	if (fromStory == 0 || toStory == 0)
		return NULL;

	SRWallInfo* pWallInfo = new SRWallInfo();
	dLoc = pText->m_Vec.m_Y;

	pWallInfo->SetStartLocation(pText->m_Vec.m_Y - dTol / 2);
	pWallInfo->SetEndLocation(pText->m_Vec.m_Y + dTol / 2);
	pWallInfo->SetFromStory(fromStory);
	pWallInfo->SetToStory(toStory);
	return pWallInfo;
}

bool MTReader::AddPolyline(GM2DPolyline* polyline, CString layerName)
{
	int numVec = polyline->ma_Point.GetSize();
	if (numVec < 2)
	{
		delete polyline;
		return false;
	}
	GM2DVector* SVec = polyline->ma_Point[0];
	GM2DVector* EVec = polyline->ma_Point[numVec - 1];

	SRSectionTable* pTable = FindSectionTable(SVec, EVec);
	if (pTable == NULL)
	{
		delete polyline;
		return false;
	}

	pTable->AddPolyline(polyline);
	MTReader::CellType cellType = GetCellTypeByLayer(layerName);
	if (cellType != MTReader::ctNone)
		pTable->AddCellTypeToGeometry(cellType, polyline);

	return true;
}

bool MTReader::AddTable(double cenX, double cenY, double width, double height, int VdId)
{
	// LH벽체
	if (m_Group == GROUP_WALL && m_nReadType == eReadType::LH_WALL)
	{
		AddTable_LH(GM2DVector(cenX, cenY), width, height, VdId);
		return true;
	}
	double tolerance = 10;
	GM2DRectangle* pRect = new GM2DRectangle(cenX - width / 2, cenY - height / 2, cenX + width / 2, cenY + height / 2);
	int NumTable = ma_Table.GetSize();
	for (int iTable = 0; iTable < NumTable; iTable++)
	{
		GM2DRectangle* pOldRect = ma_Table[iTable]->mp_Rect;
		if (pOldRect->m_Origin.SameVector4DTOL(pRect->m_Origin, tolerance) &&
			pOldRect->m_Corner.SameVector4DTOL(pRect->m_Corner, tolerance))
		{
			delete pRect;
			return false;
		}
	}

	SRSectionTable* pTable = new SRSectionTable(this);
	pTable->mp_Rect = pRect;
	pTable->m_VdId = VdId;
	ma_Table.Add(pTable);
	return true;
}
bool MTReader::AddLine(double x1, double y1, double x2, double y2, CString layerName)
{
	GM2DVector SVec(x1, y1);
	GM2DVector EVec(x2, y2);

	double extendLength = 10.0;
	GM2DLineVector aExtendLine(SVec, EVec);

	if (m_Group == GROUP_COLUMN || m_Group == GROUP_BEAM || m_Group == GROUP_BUTTRESS)
	{
		aExtendLine.ExtendLineVector(TRUE, extendLength, FALSE);
		aExtendLine.ExtendLineVector(FALSE, extendLength, TRUE);
	}

	SRSectionTable* pTable = FindSectionTable(&aExtendLine.m_P0, &aExtendLine.m_P1);
	if (pTable)
	{
		int RtnVal = -1;
		GM2DLineVector* pLineVec = new GM2DLineVector(SVec, EVec);
		double dDegree = pLineVec->GetDegree();
		if (dDegree > 90 && dDegree <= 270)
			pLineVec->ReverseDirection();

		GM2DPolyline poly;
		pTable->mp_Rect->GetBoundPolyline(poly);
		GM2DCurve* onLineCurve = poly.GetOnLineCurve(pLineVec, 1.0);
		if (onLineCurve != nullptr)
		{
			delete pLineVec;
			delete onLineCurve;
			return false;
		}

		if (layerName == DG_OUT_LINE)
			RtnVal = pTable->AddLine(pLineVec, OUTLINE_TYPE);
		else if (layerName == DG_STRP_BAR)
			RtnVal = pTable->AddLine(pLineVec, STRPLINE_TYPE);
		else if (layerName == DG_TIE_BAR)
			RtnVal = pTable->AddLine(pLineVec, TIELINE_TYPE);
		else
			RtnVal = pTable->AddLine(pLineVec, CURVE_TYPE);

		CellType cellType = GetCellTypeByLayer(layerName);
		if (cellType != MTReader::ctNone)
			pTable->AddCellTypeToGeometry(cellType, pLineVec);
		if (RtnVal == -1)
		{
			delete pLineVec;
			return false;
		}
		return true;
	}
	return false;
}

bool MTReader::AddCircle(double cenX, double cenY, double radius, CString layerName)
{
	GM2DVector CenVec(cenX, cenY);

	SRSectionTable* pTable = FindSectionTable(&CenVec, NULL);
	if (pTable)
	{
		GMCircle* pCircle = pTable->FindGMCircle(CenVec, radius);
		if (pCircle == NULL)
		{
			pCircle = new GMCircle(radius, cenX, cenY);
			if (layerName == DG_SUBMAINBAR)
				pTable->ma_SubMainBarCircle.Add(pCircle);
			else
				pTable->ma_Circle.Add(pCircle);

			MTReader::CellType cellType = GetCellTypeByLayer(layerName);
			if (cellType != MTReader::ctNone)
				pTable->AddCellTypeToGeometry(cellType, pCircle);

			return true;
		}
	}
	return false;
}
bool MTReader::AddArc(double cenX, double cenY, double radius, double sAngle, double eAngle, CString layerName, bool IsFromPolyline)
{
	GM2DVector CenVec(cenX, cenY);

	SRSectionTable* pTable = FindSectionTable(&CenVec, NULL);
	if (pTable)
	{
		if (pTable->mp_Rect->Width() < radius * 2 || pTable->mp_Rect->Height() < radius * 2)
			return false;
		GMCircle* pCircle = pTable->FindGMCircle(CenVec, radius);
		if (pCircle == NULL)
		{
			if (fabs(sAngle - eAngle) < DTOL1)
			{
				pCircle = new GMCircle(radius, cenX, cenY);
				if (layerName == DG_SUBMAINBAR)
					pTable->ma_SubMainBarCircle.Add(pCircle);
				else
					pTable->ma_Circle.Add(pCircle);
			}
			else
			{
				pCircle = new GMArc(radius, cenX, cenY, sAngle, eAngle);
				pCircle->m_Tag = 1;
				pTable->ma_Curve.Add(pCircle);
			}
			return true;
		}
	}

	return false;
}
bool MTReader::AddText(GM2DVector insertVec, CString text, double degree, double height, GM2DVector origin, GM2DVector corner, CString layerName, int figureID)
{
	// LH벽체
	if (m_Group == GROUP_WALL && m_nReadType == eReadType::LH_WALL)
	{
		AddText_LH(insertVec, text, origin, corner, figureID);
		return true;
	}

	if (m_Group == GROUP_SLAB)
		return AddTextBySlab(insertVec, text, degree, height, origin, corner, layerName, figureID);

	GM2DVector	txtVec = insertVec;
	CString sText = text.MakeUpper();
	if (sText == "" || sText == "-")	// 공백이나 "-"이면 무시
		return false;

	layerName.Replace(_T(" "), _T(""));
	layerName = layerName.MakeUpper();
	long nCellType = GetCellTypebyDictionary(sText);
	if (nCellType != 0)
	{
		double dValue = 0.;
		if (m_Group == GROUP_WALL || m_Group == GROUP_SLAB)
			dValue = txtVec.m_X;
		else
			dValue = txtVec.m_Y;

		if (m_MaterialType == MSMaterial::msSteel)
			dValue = txtVec.m_X;

		m_LocCellTypeMap.SetAt(dValue, nCellType);

		GMText* pText = new GMText;
		pText->m_strText = sText;
		pText->m_Vec = txtVec;
		pText->m_dHeight = height;
		pText->m_Origin = origin;
		pText->m_Corner = corner;

		map<long, vector<GMText*>>::iterator it = mm_CellTypeGMTexts.find(nCellType);
		vector<GMText*> TextArr;
		if (it == mm_CellTypeGMTexts.end())
		{
			TextArr.push_back(pText);
			mm_CellTypeGMTexts.insert(make_pair(nCellType, TextArr));
		}
		else
		{
			TextArr = it->second;
			TextArr.push_back(pText);
			mm_CellTypeGMTexts.erase(it);
			mm_CellTypeGMTexts.insert(make_pair(nCellType, TextArr));
		}
		return true;
	}

	GMText* pText = new GMText;
	pText->m_strText = sText;
	pText->m_Vec = txtVec;
	pText->m_dHeight = height;
	pText->m_Origin = origin;
	pText->m_Corner = corner;
	if (ma_GMText.GetSize() == 0)	m_dTol = pText->m_dHeight;
	SRSectionTable* pTable = FindSectionTable(&txtVec, NULL);
	if (pTable)
	{
		if (layerName == DG_STR_TEXT && pTable->mp_Name == NULL)
			pTable->mp_Name = pText;
		pTable->ma_Text.Add(pText);
		AddTextByLayerName(pTable, layerName, pText);
		mm_TextToFigureID.insert(make_pair(pText, figureID));

		AddDictionaryTextToLayer(pText, layerName);
		return true;
	}
	else if (m_Group == GROUP_COLUMN || m_Group == GROUP_SLAB)
	{
		ma_GMText.Add(pText);
		mm_TextToFigureID.insert(make_pair(pText, figureID));
	}
	else if (m_Group == GROUP_WALL)
	{
		ma_GMText.Add(pText);
		mm_TextToFigureID.insert(make_pair(pText, figureID));
	}
	else	delete pText;

	return true;
}

bool MTReader::AddDimension(double defPointX1, double defPointY1, double defPointX2, double defPointY2, double linePointX, double linePointY, double txtX, double txtY, CString text, CString layerName)
{
	GM2DVector TextVec(txtX, txtY);
	SRSectionTable* pTable = FindSectionTable(&TextVec, NULL);
	if (pTable)
	{
		GM2DVector defVec1(defPointX1, defPointY1);
		GM2DVector defVec2(defPointX2, defPointY2);
		GM2DVector lineVec(linePointX, linePointY);

		SRDimension* pSRDim = new SRDimension(pTable);

		if (text.IsEmpty())
		{
			GM2DLineVector aLineVec(defVec2, lineVec);
			GM2DVector OrthoVec = aLineVec.OrthoBaseVector(defVec1);

			text.Format(_T("%.1f"), defVec1.Distance(OrthoVec));
		}
		bool Result = pSRDim->SetLength(text);
		if (!Result)
		{
			delete pSRDim;	return false;
		}

		Result = pSRDim->SetLVec(defVec1, defVec2, lineVec);
		if (!Result)
		{
			delete pSRDim;	return false;
		}
		pTable->AddDimension(pSRDim);
		AddDimensionByLayerName(pTable, layerName, pSRDim);
		return true;
	}
	return false;
}

void MTReader::MakeSectionDatas()
{
#ifdef _DEBUG
	TestMsg();
#endif

	// LH벽체
	if (m_Group == GROUP_WALL && m_nReadType == eReadType::LH_WALL)
	{
		MakeWallM_LH();
		return;
	}


	long nTable = ma_Table.GetSize();

	if (nTable == 0 && m_Group == GROUP_SLAB)
		nTable = MakeSlabTable();
	for (long iTable = 0; iTable < nTable; iTable++)
		ma_Table[iTable]->MakeData();

}

void MTReader::MakeData_Excel()
{
	long nTable = ma_Table.GetSize();
	for (long iTable = 0; iTable < nTable; iTable++)
		ma_Table[iTable]->MakeData_Excel();
}

int MTReader::MakeSlabTable()
{
	CTypedPtrArray<CObArray, GMText*> NameArr;

	// 1. LocCellTypeMap을 사용해서, MinX, MaxX를 구한후 Width설정
	double dMinX = 10e10;
	double dMaxX = -10e10;

	double dLoc = 0;
	long nCellType;
	int CellTypeCount = m_LocCellTypeMap.GetCount();
	if (CellTypeCount == 0)
		return 0;

	double dSlabLx1 = 0, dSlabLy1 = 0;
	POSITION pos = m_LocCellTypeMap.GetStartPosition();
	while (pos)
	{
		m_LocCellTypeMap.GetNextAssoc(pos, dLoc, nCellType);
		if (dLoc < dMinX)
			dMinX = dLoc;
		if (dLoc > dMaxX)
			dMaxX = dLoc;

		if (nCellType == ctSlabLx1)
			dSlabLx1 = dLoc;
		if (nCellType == ctSlabLy1)
			dSlabLy1 = dLoc;
	}
	if (fabs(dSlabLx1 - dSlabLy1) < 10.)
		CellTypeCount -= 6;
	// 1.1 각 Cell간의 평균거리를 구해서 MinX와 MaxX에 각각 빼고 더해준다.
	double CellDistance = (dMaxX - dMinX) / CellTypeCount;
	dMinX -= CellDistance;
	dMaxX += CellDistance;
	double cenX = dMinX + (dMaxX - dMinX) / 2;
	double width = (dMaxX - dMinX);
	// 1.2 컬럼헤더 텍스트와의 톨러런스는 Cell간격의 1/4로 설정
	m_dTol = CellDistance / 4;

	// 2. CellType이 ctName인 것만 찾아서 Height설정
	double TableHeight = 100;
	int NumText = ma_GMText.GetSize();
	for (int iText = 0; iText < NumText; iText++)
	{
		GMText* pText = ma_GMText[iText];
		long nCellType;
		nCellType = GetCellType(pText);
		if (nCellType == ctName)
		{
			NameArr.Add(pText);
			TableHeight = pText->m_dHeight * 1.5;
		}
	}
	// 2.1 NameArr 정렬
	int NumName = NameArr.GetSize();
	for (iText = 0; iText < NumName; iText++)
	{
		for (int jText = iText + 1; jText < NumName; jText++)
		{
			GMText* pIText = NameArr[iText];
			GMText* pJText = NameArr[jText];
			if (pIText->m_Vec.m_Y < pJText->m_Vec.m_Y)
			{
				GMText* pTemp = pIText;
				NameArr[iText] = pJText;
				NameArr[jText] = pTemp;
			}
		}
	}
	// 2.2 NameArr가 두개 이상이면 정렬된 첫번째와 두번째의 차만큼을 TableHeight로 설정
	//        아니라면, 1개뿐이므로 텍스트크기의 1.5*10 정도의 큰 크기로 설정
	if (NumName >= 2)
	{
		GMText* pFirstText = NameArr[0];
		GMText* pSecondText = NameArr[1];
		TableHeight = fabs(pSecondText->m_Vec.m_Y - pFirstText->m_Vec.m_Y);
	}
	else
		TableHeight = TableHeight * 10;

	// 3. CellType이 ctName인 것들로 Table생성
	for (int iName = 0; iName < NumName; iName++)
	{
		GMText* pName = NameArr[iName];
		double cenY = pName->m_Vec.m_Y;

		AddTable(cenX, cenY, width, TableHeight, 0);
	}

	// 4. ma_GMText를 해당 Table에 넣어준다.
	for (iText = 0; iText < NumText; iText++)
	{
		GMText* pText = ma_GMText[iText];
		long nCellType;
		nCellType = GetCellType(pText);
		if (nCellType == ctNone)	continue;

		SRSectionTable* pTable = FindSectionTable(&pText->m_Vec, NULL);
		if (pTable)
			pTable->ma_Text.Add(pText);
	}
	return ma_Table.GetSize();
}

CString MTReader::GetGroupName()
{
	CString GroupName = _T("");
	switch (m_Group)
	{
	case GROUP_COLUMN:	GroupName = _T("Column");	break;
	case GROUP_BEAM:			GroupName = _T("Beam");		break;
	case GROUP_WALL:			GroupName = _T("Wall");		break;
	case GROUP_SLAB:			GroupName = _T("Slab");		break;
	case GROUP_BUTTRESS:	GroupName = _T("Buttress");	break;
	case GROUP_RWALL:		GroupName = _T("RWall");		break;
	default:
		ASSERT(0);
	}
	return GroupName;
}

long MTReader::ExecuteSectionDatas(/*CTypedPtrArray<CObArray, MSCompoM*>& NewCompoMArr*/)
{
	long nMakeData = 0;
	long nTable = ma_Table.GetSize();

	if (m_Group == GROUP_WALL && m_nReadType == 2)
		return 1;

	if (m_Group == GROUP_COLUMN || m_Group == GROUP_WALL || m_Group == GROUP_BUTTRESS || m_Group == GROUP_RWALL)
		CheckOverlapStory();
	else
		CheckOverlapName();

	if (m_Group == GROUP_WALL)
		CheckWallThick();
	CString GroupName = GetGroupName();

	DeleteResultMArr();
	for (long iTable = 0; iTable < nTable; iTable++)
	{
		SRSectionTable* pTable = ma_Table[iTable];
		pTable->MakeErrorObject(iTable + 1);

		if (m_Group == GROUP_COLUMN)				MakeColumnSection(pTable);
		else if (m_Group == GROUP_BEAM)			MakeBeamSection(pTable);
		else if (m_Group == GROUP_WALL)			MakeWallSection(pTable);
		else if (m_Group == GROUP_SLAB)				MakeSlabSection(pTable);
		else if (m_Group == GROUP_BUTTRESS)	MakeButtressSection(pTable);
		else if (m_Group == GROUP_RWALL)			MakeRWallSection(pTable);
		nMakeData++;
	}

	int Level = -1;
	map<CString, MTOverlapArea*>::iterator it;
	for (it = mm_Overlap.begin(); it != mm_Overlap.end(); it++)
	{
		CString Name = it->first;
		int errorCode = it->second->m_ErrorCode;
		CreateErrorObject(GroupName, Level, errorCode, Name, it->second->ma_Area);
	}

	return nMakeData;
}

bool MTReader::CheckOverlapStory()
{
	DeleteOverlapAreas();
	long nTable = ma_Table.GetSize();
	if (m_MaterialType == MSMaterial::msSteel && m_Group == GROUP_COLUMN)
	{
		return CheckOverlapStory_SteelColumn();
	}
	int LimitFrom = -(mp_Bldg->GetNumUnderStory() - 1);
	int LimitTo = mp_Bldg->GetNumAboveStory() - 1;
	for (long iTable = 0; iTable < nTable; iTable++)
	{
		SRSectionTable* pITable = ma_Table[iTable];
		CStringArray iNameArr, jNameArr;

		long nIName = pITable->GetSectionName(iNameArr);
		if (nIName == 0)
			nIName = FindSectionName(pITable, iNameArr);

		long iFrom = pITable->m_nFromStory;
		long iTo = pITable->m_nToStory;
		for (long iName = 0; iName < nIName; iName++)
		{
			CString IName = iNameArr[iName];
			for (long jTable = iTable + 1; jTable < nTable; jTable++)
			{
				SRSectionTable* pJTable = ma_Table[jTable];
				jNameArr.RemoveAll();
				long nJName = pJTable->GetSectionName(jNameArr);
				if (nJName == 0)
					nJName = FindSectionName(pJTable, jNameArr);
				for (long jName = 0; jName < nJName; jName++)
				{
					CString JName = jNameArr[jName];
					if (IName == JName)
					{
						long jFrom = pJTable->m_nFromStory;
						long jTo = pJTable->m_nToStory;
						bool overlapStory = false;
						if (iFrom <= jFrom && jFrom <= iTo)
							overlapStory = true;
						if (iFrom <= jTo && jTo <= iTo)
							overlapStory = true;
						if (jFrom <= iFrom && iFrom <= jTo)
							overlapStory = true;
						if (jFrom <= iTo && iTo <= jTo)
							overlapStory = true;

						if (overlapStory)
						{
							map<CString, MTOverlapArea*>::iterator it = mm_Overlap.find(IName);
							if (it == mm_Overlap.end())
							{
								CTypedPtrArray<CObArray, GM2DRectangle*> Areas;
								Areas.Add(pITable->mp_Rect);
								Areas.Add(pJTable->mp_Rect);
								MTOverlapArea* pOverlapArea = MakeOverlapArea(IName, STORY_OVERLAP, Areas);
								mm_Overlap.insert(make_pair(IName, pOverlapArea));
							}
							else
							{
								MTOverlapArea* pOverlapArea = it->second;
								pOverlapArea->AddCopyArea(pJTable->mp_Rect);
							}
						}
					}
				}
			}
			if (iFrom < LimitFrom || iTo > LimitTo)
			{
				map<CString, MTOverlapArea*>::iterator it = mm_Overlap.find(IName);
				if (it == mm_Overlap.end())
				{
					CTypedPtrArray<CObArray, GM2DRectangle*> Areas;
					Areas.Add(pITable->mp_Rect);
					MTOverlapArea* pOverlapArea = MakeOverlapArea(IName, STORY_OUT_OF_RANGE, Areas);
					mm_Overlap.insert(make_pair(IName, pOverlapArea));
				}
				else
				{
					it->second->AddCopyArea(pITable->mp_Rect);
				}
			}
		}
	}
	if (m_Group == GROUP_WALL)
		CheckOverlapStory_Wall();
	return true;
}

bool MTReader::CheckOverlapStory_Wall()
{
	long nTable = ma_Table.GetSize();
	int LimitFrom = -mp_Bldg->GetNumUnderStory() - 1;
	int LimitTo = mp_Bldg->GetNumAboveStory() - 1;
	for (long iTable = 0; iTable < nTable; iTable++)
	{
		SRSectionTable* pTable = ma_Table[iTable];
		double sXc = pTable->mp_Rect->m_Origin.m_X;
		double eXc = pTable->mp_Rect->m_Corner.m_X;
		if (pTable->ma_Box.GetSize() == 0)	continue;
		SRSectionBox* pBox = pTable->ma_Box[0];
		long nWall = pBox->ma_WallInfo.GetSize();
		for (long iWall = 0; iWall < nWall; iWall++)
		{
			if (pBox->ma_WallInfo[iWall]->GetIsMerged())	continue;
			long iFrom = pBox->ma_WallInfo[iWall]->GetFromStory();
			long iTo = pBox->ma_WallInfo[iWall]->GetToStory();
			for (long jWall = iWall + 1; jWall < nWall; jWall++)
			{
				if (pBox->ma_WallInfo[jWall]->GetIsMerged())	continue;
				long jFrom = pBox->ma_WallInfo[jWall]->GetFromStory();
				long jTo = pBox->ma_WallInfo[jWall]->GetToStory();
				bool overlapStory = false;
				if (iFrom <= jFrom && jFrom <= iTo)
					overlapStory = true;
				if (iFrom <= jTo && jTo <= iTo)
					overlapStory = true;
				if (jFrom <= iFrom && iFrom <= jTo)
					overlapStory = true;
				if (jFrom <= iTo && iTo <= jTo)
					overlapStory = true;

				if (overlapStory)
				{
					SRWallInfo* pIWall = pBox->ma_WallInfo[iWall];
					SRWallInfo* pJWall = pBox->ma_WallInfo[jWall];
					GM2DRectangle iRect(sXc, pIWall->GetStartLocation(), eXc, pIWall->GetEndLocation());
					GM2DRectangle jRect(sXc, pJWall->GetStartLocation(), eXc, pJWall->GetEndLocation());

					CStringArray NameArr;
					long nName = pTable->GetSectionName(NameArr);
					for (long iName = 0; iName < nName; iName++)
					{
						CString Name = NameArr[iName];
						map<CString, MTOverlapArea*>::iterator it = mm_Overlap.find(Name);
						if (it == mm_Overlap.end())
						{
							CTypedPtrArray<CObArray, GM2DRectangle*> Areas;

							Areas.Add(&iRect);
							Areas.Add(&jRect);
							MTOverlapArea* pOverlapArea = MakeOverlapArea(Name, STORY_OVERLAP, Areas);
							mm_Overlap.insert(make_pair(Name, pOverlapArea));
						}
						else
						{
							MTOverlapArea* pOverlapArea = it->second;
							pOverlapArea->AddCopyArea(&iRect);
						}
					}
				}
			}
			if (iFrom < LimitFrom || iTo > LimitTo)
			{
				SRWallInfo* pIWall = pBox->ma_WallInfo[iWall];
				GM2DRectangle iRect(sXc, pIWall->GetStartLocation(), eXc, pIWall->GetEndLocation());

				CStringArray NameArr;
				long nName = pTable->GetSectionName(NameArr);
				for (long iName = 0; iName < nName; iName++)
				{
					CString Name = NameArr[iName];
					map<CString, MTOverlapArea*>::iterator it = mm_Overlap.find(Name);
					if (it == mm_Overlap.end())
					{
						CTypedPtrArray<CObArray, GM2DRectangle*> Areas;
						Areas.Add(&iRect);
						MTOverlapArea* pOverlapArea = MakeOverlapArea(Name, STORY_OUT_OF_RANGE, Areas);
						mm_Overlap.insert(make_pair(Name, pOverlapArea));
					}
					else
					{
						it->second->AddCopyArea(&iRect);
					}
				}
			}
		}
	}

	return true;
}

bool MTReader::CheckOverlapStory_SteelColumn()
{
	return true;
}

bool MTReader::CheckOverlapName_SteelBeam()
{
	map<CString, bool> NameMap;
	long nTable = ma_Table.GetSize();
	for (long iTable = 0; iTable < nTable; iTable++)
	{
		SRSectionTable* pITable = ma_Table[iTable];
		int nBox = pITable->ma_Box.GetSize();
		for (int iBox = 0; iBox < nBox; iBox++)
		{
			SRSectionBox* pBox = pITable->ma_Box[iBox];
			int nInfo = pBox->ma_SteelInfo.GetSize();
			for (int iInfo = 0; iInfo < nInfo; iInfo++)
			{
				SRSteelInfo* pInfo = pBox->ma_SteelInfo[iInfo];
				int nName = pInfo->ma_Name.size();
				for (int iName = 0; iName < nName; iName++)
				{
					CString name = pInfo->ma_Name[iName];
					if (NameMap.find(name) != NameMap.end())
					{
						map<CString, MTOverlapArea*>::iterator it = mm_Overlap.find(name);
						if (it == mm_Overlap.end())
						{
							CTypedPtrArray<CObArray, GM2DRectangle*> Areas;
							Areas.Add(pITable->mp_Rect);
							MTOverlapArea* pOverlapArea = MakeOverlapArea(name, ERROR_NAME_OVERLAP, Areas);
							mm_Overlap.insert(make_pair(name, pOverlapArea));
						}
						else
						{
							MTOverlapArea* pOverlapArea = it->second;
							pOverlapArea->AddCopyArea(pITable->mp_Rect);
						}
					}
				}
			}
		}
	}
	return true;
}

bool MTReader::CheckOverlapName()
{
	DeleteOverlapAreas();
	if (m_MaterialType == MSMaterial::msSteel && m_Group == GROUP_BEAM)
	{
		return CheckOverlapName_SteelBeam();
	}
	long nTable = ma_Table.GetSize();
	for (long iTable = 0; iTable < nTable; iTable++)
	{
		for (long jTable = iTable + 1; jTable < nTable; jTable++)
		{
			SRSectionTable* pITable = ma_Table[iTable];
			SRSectionTable* pJTable = ma_Table[jTable];
			CStringArray iNameArr, jNameArr;
			long nIName = pITable->GetSectionName(iNameArr);
			long nJName = pJTable->GetSectionName(jNameArr);
			for (long iName = 0; iName < nIName; iName++)
			{
				CString IName = iNameArr[iName];
				for (long jName = 0; jName < nJName; jName++)
				{
					CString JName = jNameArr[jName];
					if (IName == JName)
					{
						map<CString, MTOverlapArea*>::iterator it = mm_Overlap.find(IName);
						if (it == mm_Overlap.end())
						{
							CTypedPtrArray<CObArray, GM2DRectangle*> Areas;
							Areas.Add(pITable->mp_Rect);
							Areas.Add(pJTable->mp_Rect);
							MTOverlapArea* pOverlapArea = MakeOverlapArea(IName, ERROR_NAME_OVERLAP, Areas);
							mm_Overlap.insert(make_pair(IName, pOverlapArea));
						}
						else
						{
							MTOverlapArea* pOverlapArea = it->second;
							pOverlapArea->AddCopyArea(pJTable->mp_Rect);
						}
					}
				}
			}
		}
	}
	return true;
}

bool MTReader::CheckWallThick()
{
	int Level = -1;
	long nTable = ma_Table.GetSize();
	for (long iTable = 0; iTable < nTable; iTable++)
	{
		SRSectionTable* pTable = ma_Table[iTable];
		CStringArray NameArr;
		long nName = pTable->GetSectionName(NameArr);
		if (nName == 0)	continue;

		CString Name = NameArr[0];
		long nBox = pTable->ma_Box.GetSize();
		bool IsError = false;
		for (long iBox = 0; iBox < nBox; iBox++)
		{
			SRSectionBox* pBox = pTable->ma_Box[iBox];
			long nWallInfo = pBox->ma_WallInfo.GetSize();
			for (long iInfo = 0; iInfo < nWallInfo; iInfo++)
			{
				SRWallInfo* pInfo = pBox->ma_WallInfo[iInfo];
				if (pInfo->GetThick() < DTOL1)
				{
					IsError = true;
					break;
				}
			}
			if (IsError)
				break;
		}
		if (IsError)
		{
			map<CString, MTOverlapArea*>::iterator it = mm_Overlap.find(Name);
			if (it == mm_Overlap.end())
			{
				CTypedPtrArray<CObArray, GM2DRectangle*> Ares;
				Ares.Add(pTable->mp_Rect);
				MTOverlapArea* pOverlap = MakeOverlapArea(Name, ERROR_THICK, Ares);
				mm_Overlap.insert(make_pair(Name, pOverlap));
			}
		}
	}
	return true;
}

MTOverlapArea* MTReader::MakeOverlapArea(CString sKey, int errorCode, CTypedPtrArray<CObArray, GM2DRectangle*>& Areas)
{
	MTOverlapArea* pOverlap = new MTOverlapArea();
	pOverlap->m_sKey = sKey;
	pOverlap->m_ErrorCode = errorCode;

	int numArea = Areas.GetSize();
	for (int iArea = 0; iArea < numArea; iArea++)
	{
		GM2DRectangle* pRect = Areas[iArea];
		pOverlap->AddCopyArea(pRect);
	}
	return pOverlap;
}

int MTReader::CreateErrorObject(CString GroupName, int Level, int ErrorCode, CString Message, CTypedPtrArray<CObArray, GM2DRectangle*>& Areas)
{
	MTError* pError = new MTError();
	pError->m_GroupName = GroupName;
	pError->m_Level = Level;
	pError->m_ErrorCode = ErrorCode;
	pError->m_Message = Message;
	int numArea = Areas.GetSize();
	for (int iArea = 0; iArea < numArea; iArea++)
	{
		GM2DRectangle* pRect = Areas[iArea];
		pError->AddCopyArea(pRect);
	}
	return ma_Error.Add(pError);
}

int MTReader::CreateErrorObject(CString GroupName, int Level, int ErrorCode, CString Message, GM2DRectangle* pArea)
{
	MTError* pError = new MTError();
	pError->m_GroupName = GroupName;
	pError->m_Level = Level;
	pError->m_ErrorCode = ErrorCode;
	pError->m_Message = Message;
	if (pArea != NULL)
		pError->AddCopyArea(pArea);
	return ma_Error.Add(pError);
}

long MTReader::FindSectionName(SRSectionTable* pTable, CStringArray& NameArr)
{
	GM2DVector cenVec;
	double X1 = pTable->mp_Rect->Left();
	double X2 = pTable->mp_Rect->Right();
	double Y1 = pTable->mp_Rect->Bottom();
	double Y2 = pTable->mp_Rect->Top();

	SRSectionTable* pLeftTable = NULL, * pTopTable = NULL;
	int nTable = ma_Table.GetSize();
	for (int iTable = 0; iTable < nTable; iTable++)
	{
		SRSectionTable* iSectionTable = ma_Table[iTable];
		if (iSectionTable == pTable)	continue;

		GM2DVector cenVec;
		iSectionTable->mp_Rect->GetCenter(cenVec);
		if (X1 <= cenVec.m_X && cenVec.m_X <= X2)	// 같은 col 인지 판단
		{
			if (Y2 < cenVec.m_Y)												// row가 윗쪽인지 판단
			{
				if (pTopTable == NULL)
					pTopTable = iSectionTable;
				else
				{
					if (pTopTable->mp_Rect->Top() < cenVec.m_Y)
						pTopTable = iSectionTable;
				}
			}
		}
		if (Y1 <= cenVec.m_Y && cenVec.m_Y <= Y2)	// 같은 row인지 판단
		{
			if (X1 > cenVec.m_X)
			{
				if (pLeftTable == NULL)
					pLeftTable = iSectionTable;
				else
				{
					if (pLeftTable->mp_Rect->Left() > cenVec.m_X)
						pLeftTable = iSectionTable;
				}
			}
		}
	}
	int nName = 0;
	if (pTopTable != NULL)
		nName = pTopTable->GetSectionName(NameArr);
	if (nName == 0 && pLeftTable != NULL)
		nName = pLeftTable->GetSectionName(NameArr);
	return nName;
}

void MTReader::MakeColumnSection(SRSectionTable* pTable)
{
	if (m_MaterialType == MSMaterial::msConcrete)
		MakeRcColumnSection(pTable);
	else if (m_MaterialType == MSMaterial::msSteel)
		MakeSteelColumnSection(pTable);
	else if (m_MaterialType == MSMaterial::msSRC)
		MakeSrcColumnSection(pTable);

}
void MTReader::MakeRcColumnSection(SRSectionTable* pTable)
{
	long nBox = pTable->ma_Box.GetSize();
	if (nBox == 0)	return;
	SRSectionBox* pSectionBox = pTable->ma_Box[0];
	if (pSectionBox == NULL)	return;
	// 	if(pSectionBox->mp_Section == NULL)	return;
	CStringArray NameArr;
	long nName = pTable->GetSectionName(NameArr);
	if (nName == 0)
	{
		nName = FindSectionName(pTable, NameArr);
		if (nName == 0)	NameArr.Add(_T(""));
	}
	// 	if(pTable->GetErrorCode() > 0)	return;

	vector<MSCompoM*> addCompoMs;
	MSMaterial* pDefMaterial = mp_Bldg->GetMaterial(m_nMaterialID);
	if (pDefMaterial == NULL)	pDefMaterial = mp_Bldg->GetDefMaterial(MSMaterial::msConcrete);
	int oldCount = ma_NewM.GetSize();
	MTResultElement* firstResult = nullptr;
	for (long iName = 0; iName < NameArr.GetSize(); iName++)
	{
		CString Name = NameArr[iName];

		// 		map<CString, MTOverlapArea*>::iterator it = mm_Overlap.find(Name);
		// 		if(it != mm_Overlap.end())
		// 			continue;

		MTRcColumnMHelper theHelper(mp_Bldg, this);
		MSCompoM* pColumnM = theHelper.MakeData(Name, pSectionBox);
		if (pColumnM == NULL)	continue;
		ma_NewM.Add(pColumnM);
		addCompoMs.push_back(pColumnM);

		ma_ResultM.push_back(theHelper.GetResultCompoM());
		if (firstResult == nullptr)
			firstResult = theHelper.GetResultCompoM();
	}
	GM2DRectangle* pRect = MakeRecognizeBox(firstResult, pSectionBox->mp_Rect);
	if (pRect != nullptr)    MakeRecognizeSection(pRect, pTable, addCompoMs);
}

void MTReader::MakeSteelColumnSection(SRSectionTable* pTable)
{
	long nBox = pTable->ma_Box.GetSize();
	if (nBox == 0)	return;
	// 	if(pTable->GetErrorCode() > 0)
	// 		return;

	int oldCount = ma_NewM.GetSize();

	MSSteelUtility* pUtil = mp_Bldg->GetSteelUtili();
	vector<MSCompoM*> addCompoMs;
	map<SRSteelInfo*, MSCompoM*> steelInfoToCompoMMap;
	MTResultElement* firstResult = nullptr;
	for (int iBox = 0; iBox < nBox; iBox++)
	{
		SRSectionBox* pBox = pTable->ma_Box[iBox];
		int nSteelInfo = pBox->ma_SteelInfo.GetSize();
		for (int iInfo = 0; iInfo < nSteelInfo; iInfo++)
		{
			SRSteelInfo* pSteelInfo = pBox->ma_SteelInfo[iInfo];

			MTSteelColumnMHelper theHelper(mp_Bldg, this);
			MSCompoM* pCompoM = theHelper.MakeData(pTable, pSteelInfo);
			if (pCompoM == NULL)	continue;

			ma_NewM.Add(pCompoM);
			addCompoMs.push_back(pCompoM);
			steelInfoToCompoMMap.insert(make_pair(pSteelInfo, pCompoM));

			ma_ResultM.push_back(theHelper.GetResultCompoM());
			if (firstResult == nullptr)
				firstResult = theHelper.GetResultCompoM();
		}
	}
	GM2DRectangle* pRect = MakeRecognizeBox(firstResult, pTable->mp_Rect);
	if (pRect != nullptr)
	{
		MTRecognizeSection* pRecSection = MakeRecognizeSection(pRect, pTable, addCompoMs);
		pRecSection->AddSteelInfoToCompoMMap(steelInfoToCompoMMap);
	}
}

void MTReader::MakeSrcColumnSection(SRSectionTable* pTable)
{
	long nBox = pTable->ma_Box.GetSize();
	if (nBox == 0)	return;
	SRSectionBox* pSectionBox = pTable->ma_Box[0];
	if (pSectionBox == NULL)	return;
	// 	if(pSectionBox->mp_Section == NULL)	return;
	CStringArray NameArr;
	long nName = pTable->GetSectionName(NameArr);
	if (nName == 0)
	{
		nName = FindSectionName(pTable, NameArr);
		if (nName == 0)	NameArr.Add(_T(""));
	}
	long nType = pSectionBox->GetSectionType();
	long nFrom = -(mp_Bldg->GetNumUnderStory() - 1);
	long nTo = mp_Bldg->GetNumAboveStory() - 1;
	// 
	// 	if(pTable->GetErrorCode() > 0)
	// 		return;

	int oldCount = ma_NewM.GetSize();
	vector<MSCompoM*> addCompoMs;
	MTResultElement* firstResult = nullptr;
	for (long iName = 0; iName < NameArr.GetSize(); iName++)
	{
		CString Name = NameArr[iName];

		// 		map<CString, MTOverlapArea*>::iterator it = mm_Overlap.find(Name);
		// 		if(it != mm_Overlap.end())
		// 			continue;

		MTSrcColumnMHelper theHelper(mp_Bldg, this);
		MSCompoM* pCompoM = theHelper.MakeData(Name, pSectionBox);
		if (pCompoM == NULL)	continue;
		ma_NewM.Add(pCompoM);
		addCompoMs.push_back(pCompoM);

		ma_ResultM.push_back(theHelper.GetResultCompoM());
		if (firstResult == nullptr)
			firstResult = theHelper.GetResultCompoM();
	}
	GM2DRectangle* pRect = MakeRecognizeBox(firstResult, pSectionBox->mp_Rect);
	if (pRect != nullptr)    MakeRecognizeSection(pRect, pTable, addCompoMs);
}

void MTReader::MakeWallSection(SRSectionTable* pTable)
{
	// 	MTReader* pDoc = (MTReader*)m_pView->GetDocument();
	long nBox = pTable->ma_Box.GetSize();
	if (nBox == 0)	return;
	SRSectionBox* pSectionBox = pTable->ma_Box[0];
	CStringArray NameArr;
	long nName = pTable->GetSectionName(NameArr);
	if (nName == 0)	 NameArr.Add(_T(""));

	long nFrom = -(mp_Bldg->GetNumUnderStory() - 1);
	int nTo = mp_Bldg->GetNumAboveStory() - 1;
	MSMaterial* pDefMaterial = mp_Bldg->GetMaterial(m_nMaterialID);
	if (pDefMaterial == NULL)	pDefMaterial = mp_Bldg->GetDefMaterial(MSMaterial::msConcrete);

	CString GroupName = _T("Wall");
	int Level = -1;
	int Waning = 0;

	double sXc = pTable->mp_Rect->m_Origin.m_X;
	double eXc = pTable->mp_Rect->m_Corner.m_X;
	int oldCount = ma_NewM.GetSize();

	vector<MSCompoM*> addCompoMs;
	map<SRWallInfo*, MSCompoM*> wallInfoToCompoMMap;

	MTResultElement* firstResult = nullptr;
	for (long iName = 0; iName < NameArr.GetSize(); iName++)
	{
		CString Name = NameArr[iName];

		// 		map<CString, MTOverlapArea*>::iterator it = mm_Overlap.find(Name);
		// 		if (it != mm_Overlap.end())
		// 			continue;

		long NumWallInfo = pSectionBox->ma_WallInfo.GetSize();
		for (long iInfo = 0; iInfo < NumWallInfo; iInfo++)
		{
			SRWallInfo* pWallInfo = pSectionBox->ma_WallInfo[iInfo];
			if (pWallInfo->GetIsMerged())	continue;
			MTBWallMHelper theHelper(mp_Bldg, this);
			MSCompoM* pCompoM = theHelper.MakeData(Name, pSectionBox, pWallInfo);
			if (pCompoM == NULL)	continue;
			ma_NewM.Add(pCompoM);
			addCompoMs.push_back(pCompoM);
			wallInfoToCompoMMap.insert(make_pair(pWallInfo, pCompoM));
			ma_ResultM.push_back(theHelper.GetResultCompoM());
			if (firstResult == nullptr)
				firstResult = theHelper.GetResultCompoM();
		}
	}

	GM2DRectangle* pRect = MakeRecognizeBox(firstResult, pSectionBox->mp_Rect);
	if (pRect != nullptr)
	{
		MTRecognizeSection* pRecSection = MakeRecognizeSection(pRect, pTable, addCompoMs);
		pRecSection->AddWallInfoToCompoMMap(wallInfoToCompoMMap);
	}

	return;
}

void MTReader::MakeRWallSection(SRSectionTable* pTable)
{
	long nBox = pTable->ma_Box.GetSize();
	if (nBox == 0)	return;
	SRSectionBox* pSectionBox = pTable->ma_Box[0];
	if (pSectionBox == NULL)	return;
	CStringArray NameArr;
	long nName = pTable->GetSectionName(NameArr);
	if (nName == 0)
	{
		nName = FindSectionName(pTable, NameArr);
		if (nName == 0)	NameArr.Add(_T(""));
	}
	if (pTable->GetErrorCode() > 0)	return;

	vector<MSCompoM*> addCompoMs;
	MSMaterial* pDefMaterial = mp_Bldg->GetMaterial(m_nMaterialID);
	if (pDefMaterial == NULL)	pDefMaterial = mp_Bldg->GetDefMaterial(MSMaterial::msConcrete);
	int oldCount = ma_NewM.GetSize();
	MTResultElement* firstResult = nullptr;
	for (long iName = 0; iName < NameArr.GetSize(); iName++)
	{
		CString Name = NameArr[iName];

		// 		map<CString, MTOverlapArea*>::iterator it = mm_Overlap.find(Name);
		// 		if(it != mm_Overlap.end())
		// 			continue;

		MTRWallMHelper theHelper(mp_Bldg, this);
		MSCompoM* pCompoM = theHelper.MakeData(Name, pSectionBox);
		if (pCompoM == NULL)	continue;
		ma_NewM.Add(pCompoM);
		addCompoMs.push_back(pCompoM);

		ma_ResultM.push_back(theHelper.GetResultCompoM());
		if (firstResult == nullptr)
			firstResult = theHelper.GetResultCompoM();
	}
	GM2DRectangle* pRect = MakeRecognizeBox(firstResult, pSectionBox->mp_Rect);
	if (pRect != nullptr)    MakeRecognizeSection(pRect, pTable, addCompoMs);
}

void MTReader::MakeBeamSection(SRSectionTable* pTable)
{
	if (m_MaterialType == MSMaterial::msConcrete)
		MakeRcBeamSection(pTable);
	else if (m_MaterialType == MSMaterial::msSteel)
		MakeSteelBeamSection(pTable);
	else if (m_MaterialType == MSMaterial::msSRC)
		MakeSrcBeamSection(pTable);
}

void MTReader::MakeRcBeamSection(SRSectionTable* pTable)
{
	long nBox = pTable->ma_Box.GetSize();
	if (nBox == 0)	return;
	// 	if(pTable->GetErrorCode() > 0)
	// 		return;

	CStringArray NameArr;
	long nName = pTable->GetSectionName(NameArr);
	if (nName == 0)	 NameArr.Add(_T(""));

	int oldCount = ma_NewM.GetSize();
	vector<MSCompoM*> addCompoMs;
	MTResultElement* firstResult = nullptr;
	for (long iName = 0; iName < NameArr.GetSize(); iName++)
	{
		CString Name = NameArr[iName];
		map<CString, MTOverlapArea*>::iterator it = mm_Overlap.find(Name);
		if (it != mm_Overlap.end())
			continue;

		MTRcBeamMHelper theHelper(mp_Bldg, this);
		MSCompoM* pCompoM = theHelper.MakeData(Name, pTable);
		if (pCompoM == NULL)	continue;

		ma_NewM.Add(pCompoM);
		addCompoMs.push_back(pCompoM);

		ma_ResultM.push_back(theHelper.GetResultCompoM());
		if (firstResult == nullptr)
			firstResult = theHelper.GetResultCompoM();
	}
	GM2DRectangle* pRect = MakeRecognizeBox(firstResult, pTable->mp_Rect);
	if (pRect != nullptr)    MakeRecognizeSection(pRect, pTable, addCompoMs);
}

void MTReader::MakeSteelBeamSection(SRSectionTable* pTable)
{
	long nBox = pTable->ma_Box.GetSize();
	if (nBox == 0)	return;
	if (pTable->GetErrorCode() > 0)
		return;

	int oldCount = ma_NewM.GetSize();

	vector<MSCompoM*> addCompoMs;
	map<SRSteelInfo*, MSCompoM*> steelInfoToCompoMMap;
	MTResultElement* firstResult = nullptr;

	for (int iBox = 0; iBox < nBox; iBox++)
	{
		SRSectionBox* pBox = pTable->ma_Box[iBox];
		int nSteelInfo = pBox->ma_SteelInfo.GetSize();
		for (int iInfo = 0; iInfo < nSteelInfo; iInfo++)
		{
			SRSteelInfo* pSteelInfo = pBox->ma_SteelInfo[iInfo];
			if (pSteelInfo->CheckData() == false)	continue;

			int nName = pSteelInfo->ma_Name.size();
			for (int iName = 0; iName < nName; iName++)
			{
				CString name = pSteelInfo->ma_Name[iName];
				map<CString, MTOverlapArea*>::iterator it = mm_Overlap.find(name);
				if (it != mm_Overlap.end())
					continue;

				MTSteelBeamMHelper theHelper(mp_Bldg, this);
				MSCompoM* pCompoM = theHelper.MakeData(pTable, pSteelInfo);
				if (pCompoM == NULL)		continue;

				ma_NewM.Add(pCompoM);
				addCompoMs.push_back(pCompoM);
				steelInfoToCompoMMap.insert(make_pair(pSteelInfo, pCompoM));

				ma_ResultM.push_back(theHelper.GetResultCompoM());
				if (firstResult == nullptr)
					firstResult = theHelper.GetResultCompoM();

			}
		}
	}
	GM2DRectangle* pRect = MakeRecognizeBox(firstResult, pTable->mp_Rect);
	if (pRect != nullptr)
	{
		MTRecognizeSection* pRecSection = MakeRecognizeSection(pRect, pTable, addCompoMs);
		pRecSection->AddSteelInfoToCompoMMap(steelInfoToCompoMMap);
	}
}

void MTReader::MakeSrcBeamSection(SRSectionTable* pTable)
{
	long nBox = pTable->ma_Box.GetSize();
	if (nBox == 0)	return;
	if (pTable->GetErrorCode() > 0)
		return;

	CStringArray NameArr;
	long nName = pTable->GetSectionName(NameArr);
	if (nName == 0)	return;

	int oldCount = ma_NewM.GetSize();
	vector<MSCompoM*> addCompoMs;
	MTResultElement* firstResult = nullptr;
	for (long iName = 0; iName < nName; iName++)
	{
		CString Name = NameArr[iName];
		map<CString, MTOverlapArea*>::iterator it = mm_Overlap.find(Name);
		if (it != mm_Overlap.end())
			continue;

		MTSrcBeamMHelper theHelper(mp_Bldg, this);
		MSCompoM* pCompoM = theHelper.MakeData(Name, pTable);
		if (pCompoM == NULL)	continue;

		ma_NewM.Add(pCompoM);
		addCompoMs.push_back(pCompoM);

		ma_ResultM.push_back(theHelper.GetResultCompoM());
		if (firstResult == nullptr)
			firstResult = theHelper.GetResultCompoM();

	}
	GM2DRectangle* pRect = MakeRecognizeBox(firstResult, pTable->mp_Rect);
	if (pRect != nullptr)    MakeRecognizeSection(pRect, pTable, addCompoMs);
}

bool MTReader::IsSubBeam(CString name)
{
	int nIgnoreFlag = ma_SubBeamExcludeFlag.size();
	for (int iFlag = 0; iFlag < nIgnoreFlag; iFlag++)
	{
		CString sFlage = ma_SubBeamExcludeFlag[iFlag];
		if (sFlage == _T(""))	continue;
		if (name.Find(sFlage) >= 0)
			return false;
	}

	int nSubFlag = ma_SubBeamFlag.size();
	for (int iFlag = 0; iFlag < nSubFlag; iFlag++)
	{
		CString sFlage = ma_SubBeamFlag[iFlag];
		if (sFlage == _T(""))	continue;
		if (name.Find(sFlage) >= 0)
			return true;
	}
	return false;
}
void MTReader::MakeSlabSection(SRSectionTable* pTable)
{
	// 	MTReader* pDoc = (MTReader*)m_pView->GetDocument();
	long nBox = pTable->ma_Box.GetSize();
	if (nBox == 0)	return;
	SRSectionBox* pSectionBox = pTable->ma_Box[0];
	CStringArray NameArr;
	long nName = pTable->GetSectionName(NameArr);
	if (nName == 0)	NameArr.Add(_T(""));

	CString GroupName = _T("Slab");
	int Level = -1;
	long nFromStory = -mp_Bldg->GetNumUnderStory();
	long nToStory = mp_Bldg->GetNumAboveStory() - 1;
	MSMaterial* pDefMaterial = mp_Bldg->GetMaterial(m_nMaterialID);
	if (pDefMaterial == NULL)	pDefMaterial = mp_Bldg->GetDefMaterial(MSMaterial::msConcrete);

	int oldCount = ma_NewM.GetSize();
	vector<MSCompoM*> addCompoMs;
	map<SRSlabInfo*, MSCompoM*> slabInfoToCompoMMap;

	MTResultElement* firstResult = nullptr;
	for (long iName = 0; iName < NameArr.GetSize(); iName++)
	{
		if (pSectionBox->mp_SlabInfo == NULL)
			continue;

		CString Name = NameArr[iName];
		// 
		// 		map<CString, MTOverlapArea*>::iterator it = mm_Overlap.find(Name);
		// 		if (it != mm_Overlap.end())
		// 			continue;

		MTRcSlabMHelper theHelper(mp_Bldg, this);
		MSCompoM* pCompoM = theHelper.MakeData(Name, pTable);
		if (pCompoM == NULL)	continue;
		ma_NewM.Add(pCompoM);
		addCompoMs.push_back(pCompoM);
		SRSlabInfo* pSlabInfo = pSectionBox->mp_SlabInfo;
		slabInfoToCompoMMap.insert(make_pair(pSlabInfo, pCompoM));

		ma_ResultM.push_back(theHelper.GetResultCompoM());
		if (firstResult == nullptr)
			firstResult = theHelper.GetResultCompoM();

	}
	GM2DRectangle* pRect = MakeRecognizeBox(firstResult, pSectionBox->mp_Rect);
	if (pRect != nullptr)
	{
		MTRecognizeSection* pRecSection = MakeRecognizeSection(pRect, pTable, addCompoMs);
		pRecSection->AddSlabInfoToCompoMMap(slabInfoToCompoMMap);
	}
}

void MTReader::MakeButtressSection(SRSectionTable* pTable)
{
	long nBox = pTable->ma_Box.GetSize();
	if (nBox == 0)	return;
	if (pTable->GetErrorCode() > 0)
		return;
	SRSectionBox* pSectionBox = pTable->ma_Box[0];
	if (pSectionBox == NULL)	return;
	// 	if(pSectionBox->mp_Section == NULL)	return;

	CStringArray NameArr;
	long nName = pTable->GetSectionName(NameArr);
	if (nName == 0)	 NameArr.Add(_T(""));

	int oldCount = ma_NewM.GetSize();

	vector<MSCompoM*> addCompoMs;
	MTResultElement* firstResult = nullptr;
	for (int iName = 0; iName < NameArr.GetSize(); iName++)
	{
		CString Name = NameArr[iName];
		MTButtressMHelper theHelper(mp_Bldg, this);
		MSCompoM* pCompoM = theHelper.MakeData(Name, pTable);
		if (pCompoM == NULL)	continue;

		ma_NewM.Add(pCompoM);
		addCompoMs.push_back(pCompoM);

		ma_ResultM.push_back(theHelper.GetResultCompoM());
		if (firstResult == nullptr)
			firstResult = theHelper.GetResultCompoM();

	}
	GM2DRectangle* pRect = MakeRecognizeBox(firstResult, pSectionBox->mp_Rect);
	if (pRect != nullptr)    MakeRecognizeSection(pRect, pTable, addCompoMs);

	// 	long nType = MSRcLineMemberM::RECT;
	// 	double Width = pSectionBox->m_Width;
	// 	double Depth = pSectionBox->m_Depth;
	// 	long nSectionCombination = 0;
	// 	if(nBox==1)				nSectionCombination = 0;
	// 	else if(nBox==2)		nSectionCombination = 1;
	// 	else if(nBox==3)		nSectionCombination = 2;
	// 	else return;
	// 
	// 	long nFrom = -(mp_Bldg->GetNumUnderStory()-1);
	// 	long nTo = mp_Bldg->GetNumAboveStory()-1;
	// 
	// 	MSMaterial* pDefMaterial = mp_Bldg->GetMaterial(m_nMaterialID);
	// 	if (pDefMaterial == NULL)	pDefMaterial = mp_Bldg->GetDefMaterial(MSMaterial::msConcrete);
	// 	MDRcButtressSec* pButtressSec = NULL;
	// 
	// 	CString GroupName = _T("Buttress");
	// 	int Level = -1;
	// 
	//
	// 	for(long iName=0; iName<nName; iName++)
	// 	{
	// 		CString Name = NameArr[iName];
	// 		map<CString, MTOverlapArea*>::iterator it = mm_Overlap.find(Name);
	// 		if(it != mm_Overlap.end())
	// 			continue;
	// 
	// 		if(MSCompoM::IsExistedSpacialCharacters(Name))
	// 		{
	// 			CString errorMessage;
	// 			errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1의 이름에 사용할 수 없는 특수 기호가 존재합니다."), _T(" SectionRecognize")), Name));
	// 			CreateErrorObject(GroupName, Level, ERROR_INFO, errorMessage, pTable->mp_Rect);
	// 			continue;
	// 		}
	// 
	// 		bool bMakeData = true;
	// 		MSRcLineMemberM* pButtressM = (MSRcLineMemberM*)mp_Bldg->CreateNewCompoM(MSCompoM::msButtressM);
	// 		MDRcButtressM* pRebarM = new MDRcButtressM;
	// 
	// 		pButtressM->m_Name = NameArr[iName];
	// 		pButtressM->SetMaterial(pDefMaterial);
	// 		pButtressM->SetIsUsedStoryMaterial(m_IsUsedStoryMaterial);
	// 
	// 		pSectionBox->GetFromToStory(pButtressM->m_nFromStory, pButtressM->m_nToStory);
	// 		if(pButtressM->m_nFromStory < nFrom || pButtressM->m_nToStory > nTo	)
	// 		{
	// 			CString errorMessage;
	// 			errorMessage.Format(_T("%s(%ld~%ld)"), pButtressM->m_Name, pButtressM->m_nFromStory, pButtressM->m_nToStory);
	// 			CreateErrorObject(GroupName, Level, STORY_OUT_OF_RANGE, errorMessage, pTable->mp_Rect);
	// 
	// 			//			ma_ErrorInfo.Add(errorMessage);
	// 			delete pButtressM;
	// 			continue;
	// 		}
	// 		if(pButtressM->m_nFromStory==0 || pButtressM->m_nToStory==0)
	// 		{
	// 			if(nFrom==0)	nFrom = 1;
	// 			if(nTo==0)	nTo = -1;
	// 			pButtressM->m_nFromStory = nFrom;
	// 			pButtressM->m_nToStory = nTo;
	// 			CreateErrorObject(GroupName, 1, STORY_ERROR, Name, pTable->mp_Rect);
	// 
	// 		}
	// 		pButtressM->SetWidth(Width);
	// 		pButtressM->SetDepth(Depth);
	// 		for(long iBox=0; iBox<nBox; iBox++)
	// 		{
	// 			SRSectionBox* pSectionBox = pTable->ma_Box[iBox];
	// 			if(pSectionBox->GetErrorCode())
	// 			{
	// 				bMakeData = false;
	// 				continue;
	// 			}
	// 			MRRcSection* pSection = pSectionBox->mp_Section;
	// 			if(pSection == NULL || pSection->ma_Edge.GetSize() != 4)
	// 			{
	// 				bMakeData = false;
	// 				continue;
	// 			}
	// 
	// 			if(nBox==1)		pButtressSec = pRebarM->mp_CenSec;
	// 			else if(nBox==2)
	// 			{
	// 				if(iBox==0)	pButtressSec = pRebarM->mp_TopSec;
	// 				else					pButtressSec = pRebarM->mp_CenSec;
	// 			}
	// 			else if(nBox==3)
	// 			{
	// 				if(m_eBeamSectionType == Small_Mid_Large)
	// 				{
	// 					if(iBox==0)			pButtressSec = pRebarM->mp_TopSec;
	// 					else if(iBox==1)  pButtressSec = pRebarM->mp_CenSec;
	// 					else							pButtressSec = pRebarM->mp_BotSec;
	// 				}
	// 				else
	// 				{
	// 					if(iBox==0)			pButtressSec = pRebarM->mp_BotSec;
	// 					else if(iBox==1)    pButtressSec = pRebarM->mp_CenSec;
	// 					else					pButtressSec = pRebarM->mp_TopSec;
	// 				}
	// 			}
	// 			if(pButtressSec==NULL)
	// 			{
	// 				ASSERT(0);
	// 				continue;
	// 			}
	// 			if(pSectionBox->mp_MainBar1)
	// 			{
	// 				pButtressSec->m_TBars.RemoveAll();
	// 				CString sDiaName = pSectionBox->mp_MainBar1->GetBarDiameterName();
	// 				double dDia = _ttof(sDiaName);
	// 				if(CheckDia(dDia) == FALSE)
	// 					bMakeData = false;
	// 				MREdge* pTopEdge = pSection->ma_Edge[2];
	// 				int NumLayer = pTopEdge->ma_VBarSet.GetSize();
	// 				if(NumLayer)
	// 				{
	// 					for(long iLayer=0; iLayer < NumLayer; iLayer++)
	// 					{
	// 						MRVBar* pVBar = pTopEdge->ma_VBarSet[iLayer];
	// 
	// 						long NumVBar = pVBar->ma_VBarLoc.GetSize();
	// 						CString theBar;
	// 						theBar.Format(_T("%ld-HD%s"), NumVBar, sDiaName);
	// 						pButtressSec->m_TBars.Add(theBar);
	// 					}
	// 				}
	// 				else
	// 				{
	// 					pButtressSec->m_TBars.SetDataByOldVersion(pSectionBox->mp_MainBar1);
	// 				}
	// 			}
	// 			if(pSectionBox->mp_MainBar2)
	// 			{
	// 				pButtressSec->m_BBars.RemoveAll();
	// 				CString sDiaName = pSectionBox->mp_MainBar2->GetBarDiameterName();
	// 				double dDia = _ttof(sDiaName);
	// 				if(CheckDia(dDia) == FALSE)
	// 					bMakeData = false;
	// 				MREdge* pBotEdge = pSection->ma_Edge[0];
	// 				int NumLayer = pBotEdge->ma_VBarSet.GetSize();
	// 				if(NumLayer)
	// 				{
	// 					for(long iLayer=0; iLayer < NumLayer; iLayer++)
	// 					{
	// 						MRVBar* pVBar = pBotEdge->ma_VBarSet[iLayer];
	// 
	// 						long NumVBar = pVBar->ma_VBarLoc.GetSize();
	// 						CString theBar;
	// 						theBar.Format(_T("%ld-HD%s"), NumVBar, sDiaName);
	// 						pButtressSec->m_BBars.Add(theBar);
	// 					}
	// 				}
	// 				else
	// 					pButtressSec->m_BBars.SetDataByOldVersion(pSectionBox->mp_MainBar2);
	// 			}
	// 			if(pSectionBox->mp_StrpBar1)
	// 			{
	// 				int NumLayer = pSectionBox->mp_StrpBar1->GetNumLayers();
	// 				for( int iLayer = 0; iLayer < NumLayer; iLayer++)
	// 				{
	// 					CString sDiaName = pSectionBox->mp_StrpBar1->GetBarDiameterName(iLayer);
	// 					double dDia = _ttof(sDiaName);
	// 					if(CheckDia(dDia) == FALSE)
	// 						bMakeData = false;
	// 				}
	// 
	// 				pButtressSec->m_SBars.ma_strBar.RemoveAll();
	// 				pButtressSec->m_SBars.ma_strBar.Copy(pSectionBox->mp_StrpBar1->ma_strBar);
	// 				pButtressSec->m_SBarNum = pSectionBox->m_nStrp;
	// 			}
	// 			if (pSectionBox->mp_SideBar != nullptr)
	// 			{
	// 				if (MakeSideBars(pSectionBox, pSectionBox->mp_SideBar, &pRebarM->m_SideMainBars, Name))
	// 					pRebarM->m_IsNumPlaceSideBar = true;
	// 				else
	// 					bMakeData = false;
	// 			}
	// 			else if (pSectionBox->m_SideStirrupBar)
	// 			{
	// 				if (MakeSideBars(pSectionBox, pSectionBox->m_SideStirrupBar, &pRebarM->m_SideStirrupBars, Name))
	// 					pRebarM->m_IsNumPlaceSideBar = false;
	// 				else
	// 					bMakeData = false;
	// 			}
	// 		}
	// 
	// 		if(nBox==1)
	// 		{
	// 			pRebarM->mp_TopSec->CopyFromMe(pRebarM->mp_CenSec);
	// 			pRebarM->mp_TopSec->CopyFromMe(pRebarM->mp_CenSec);
	// 		}
	// 		else if(nBox==2)
	// 		{
	// 			pRebarM->mp_BotSec->CopyFromMe(pRebarM->mp_TopSec);
	// 		}
	// 		if(bMakeData == false)
	// 		{
	// 			// 			CString errorMessage;
	// 			// 			errorMessage.Format(_T("%s"), pBeamM->m_Name);
	// 			// 			CreateErrorObject(GroupName, Level, ERROR_REBAR, errorMessage);
	// 
	// 			delete pButtressM;
	// 			delete pRebarM;
	// 			continue;
	// 		}
	// 		pButtressM->mp_RebarM = pRebarM;
	// 		ma_NewM.Add(pButtressM);
	// 		addCompoMs.push_back(pButtressM);
	// 	}
}

bool MTReader::MakeSideBars(SRSectionBox* pSectionBox, MSMainBar* pBoxBar, MSMainBar* pSideBar, CString name)
{
	CString sDiaName = pBoxBar->GetBarDiameterName();
	double dDia = _ttof(sDiaName);
	if (MTReaderHelper::CheckDia(dDia) == FALSE)
	{
		CString errorMessage;
		errorMessage.Format(_T("%s %s"), name, LocalDic::GetLocal(_T("측면근 직경"), _T("SectionRecognize")));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage, pSectionBox->GetOwnerRect());
		AddErrorFigureIDs(pSectionBox->GetTextFigureIDs(MTReader::ctSBar2));
		return false;
	}

	pSideBar->ma_strBar.RemoveAll();
	pSideBar->ma_strBar.Copy(pBoxBar->ma_strBar);

	return true;
}

bool MTReader::MakeSideBars(SRSectionBox* pSectionBox, MSStirrupBar* pBoxBar, MSStirrupBar* pSideBar, CString name)
{
	bool IsOK = true;
	int NumLayer = pBoxBar->GetNumLayers();
	for (int iLayer = 0; iLayer < NumLayer; iLayer++)
	{
		CString sDiaName = pBoxBar->GetBarDiameterName(iLayer);
		double dDia = _ttof(sDiaName);
		if (MTReaderHelper::CheckDia(dDia) == FALSE)
		{
			CString errorMessage;
			errorMessage.Format(_T("%s %s"), name, LocalDic::GetLocal(_T("측면근 직경"), _T("SectionRecognize")));
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage, pSectionBox->GetOwnerRect());
			AddErrorFigureIDs(pSectionBox->GetTextFigureIDs(MTReader::ctSBar2));
			IsOK = false;
		}
	}
	pSideBar->ma_strBar.RemoveAll();
	pSideBar->ma_strBar.Copy(pBoxBar->ma_strBar);
	return true;
}
void MTReader::CreateSlabErrorObject(SRSectionTable* pTable, SRSlabInfo* pSlabInfo, CString errorBar, int cellType)
{
	CString GroupName;	GroupName = _T("Slab");
	int Level = -1;
	CString errorMessage = _T("");
	errorMessage.Format(_T("%s  %s"), pSlabInfo->m_Name, errorBar);
	CreateErrorObject(GroupName, Level, ERROR_REBAR, errorMessage, pTable->mp_Rect);
	AddErrorFigureIDs(pSlabInfo->GetTextFigureIDs(cellType));
}

bool MTReader::CheckSlabBars(bool bShortStrip, SRSlabInfo* pSlabInfo, SRSectionTable* pTable)
{
	bool IsBent = false;
	if (m_SlabX3Y3Type == SLAB_X3Y3_TYPE_BENT)
		IsBent = true;
	MSRcStrip::STRIP_TYPE StripType = pSlabInfo->GetStripType(bShortStrip, IsBent);
	CString GroupName = _T("Slab");
	int Level = -1;
	CString errorMessage;
	CString Bar1, Bar2, Bar3, Bar4, Bar5, Bar6, Bar7, Bar8, Bar9, Bar10;
	if (StripType == MSRcStrip::TYPE_NONE)
	{
		StripType = MSRcStrip::TYPE_ALL;
		return true;
	}

	// 	if(StripType == MSRcStrip::TYPE_NONE)
	// 	{
	// 		if(bShortStrip)	errorMessage.Format(_T("%s  단변방향"), pSlabInfo->m_Name);
	// 		else						errorMessage.Format(_T("%s  장변방향"), pSlabInfo->m_Name);
	// 
	// 		CreateErrorObject(GroupName, Level, ERROR_REBAR, errorMessage, pTable->mp_Rect);
	// 		return false;
	// 	}
	if (bShortStrip)
	{
		Bar1 = pSlabInfo->m_X1;	Bar2 = pSlabInfo->m_X2;	Bar3 = pSlabInfo->m_X3;	Bar4 = pSlabInfo->m_X4;	Bar5 = pSlabInfo->m_X5;	Bar6 = pSlabInfo->m_X6;	Bar7 = pSlabInfo->m_X7;
	}
	else
	{
		Bar1 = pSlabInfo->m_Y1;	Bar2 = pSlabInfo->m_Y2;	Bar3 = pSlabInfo->m_Y3;	Bar4 = pSlabInfo->m_Y4;	Bar5 = pSlabInfo->m_Y5;	Bar6 = pSlabInfo->m_Y6;Bar7 = pSlabInfo->m_Y7;
	}

	int cellType = 0;
	CString errorBar = _T("");
	bool bRtnVal = true;
	if (!CheckBars(Bar5))
	{
		if (bShortStrip) { errorBar = _T("X5");	cellType = ctSlabLx5; }
		else { errorBar = _T("Y5");	cellType = ctSlabLy5; }
		bRtnVal = false;
		CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
	}
	if (!CheckBars(Bar6))
	{
		if (bShortStrip) { errorBar = _T("X6");	cellType = ctSlabLx6; }
		else { errorBar = _T("Y6");	cellType = ctSlabLy6; }
		bRtnVal = false;
		CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
	}

	switch (StripType)
	{
	case MSRcStrip::TYPE_A:
	{
		if (!CheckBars(Bar1))
		{
			if (bShortStrip) { errorBar = _T("X1");	cellType = ctSlabLx1; }
			else { errorBar = _T("Y1");	cellType = ctSlabLy1; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		if (!CheckBars(Bar4))
		{
			if (bShortStrip) { errorBar = _T("X4");	cellType = ctSlabLx4; }
			else { errorBar = _T("Y4");	cellType = ctSlabLy4; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		break;
	}
	case MSRcStrip::TYPE_B:
	{
		if (!CheckBars(Bar2))
		{
			if (bShortStrip) { errorBar = _T("X2");	cellType = ctSlabLx2; }
			else { errorBar = _T("Y2");	cellType = ctSlabLy2; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		if (!CheckBars(Bar4))
		{
			if (bShortStrip) { errorBar = _T("X4");	cellType = ctSlabLx4; }
			else { errorBar = _T("Y4");	cellType = ctSlabLy4; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		break;
	}
	case MSRcStrip::TYPE_C:
	{
		if (!CheckBars(Bar2))
		{
			if (bShortStrip) { errorBar = _T("X2");	cellType = ctSlabLx2; }
			else { errorBar = _T("Y2");	cellType = ctSlabLy2; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		if (!CheckBars(Bar4))
		{
			if (bShortStrip) { errorBar = _T("X4");	cellType = ctSlabLx4; }
			else { errorBar = _T("Y4");	cellType = ctSlabLy4; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		if (!CheckBars(Bar7))
		{
			if (bShortStrip) { errorBar = _T("X7");	cellType = ctSlabLx7; }
			else { errorBar = _T("Y7");	cellType = ctSlabLy7; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		break;
	}
	case MSRcStrip::TYPE_D:
	{
		if (!CheckBars(Bar1))
		{
			if (bShortStrip) { errorBar = _T("X1");	cellType = ctSlabLx1; }
			else { errorBar = _T("Y1");	cellType = ctSlabLy1; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		if (!CheckBars(Bar2))
		{
			if (bShortStrip) { errorBar = _T("X2");	cellType = ctSlabLx2; }
			else { errorBar = _T("Y2");	cellType = ctSlabLy2; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		if (!CheckBars(Bar4))
		{
			if (bShortStrip) { errorBar = _T("X4");	cellType = ctSlabLx4; }
			else { errorBar = _T("Y4");	cellType = ctSlabLy4; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		if (!CheckBars(Bar7))
		{
			if (bShortStrip) { errorBar = _T("X7");	cellType = ctSlabLx7; }
			else { errorBar = _T("Y7");	cellType = ctSlabLy7; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		break;
	}
	case MSRcStrip::TYPE_E:
	{
		if (!CheckBars(Bar1))
		{
			if (bShortStrip) { errorBar = _T("X1");	cellType = ctSlabLx1; }
			else { errorBar = _T("Y1");	cellType = ctSlabLy1; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		if (!CheckBars(Bar2))
		{
			if (bShortStrip) { errorBar = _T("X2");	cellType = ctSlabLx2; }
			else { errorBar = _T("Y2");	cellType = ctSlabLy2; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		if (!CheckBars(Bar4))
		{
			if (bShortStrip) { errorBar = _T("X4");	cellType = ctSlabLx4; }
			else { errorBar = _T("Y4");	cellType = ctSlabLy4; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		break;
	}
	case MSRcStrip::TYPE_F:
	{
		if (!CheckBars(Bar3))
		{
			if (bShortStrip) { errorBar = _T("X3");	cellType = ctSlabLx3; }
			else { errorBar = _T("Y3");	cellType = ctSlabLy3; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		if (!CheckBars(Bar4))
		{
			if (bShortStrip) { errorBar = _T("X4");	cellType = ctSlabLx4; }
			else { errorBar = _T("Y4");	cellType = ctSlabLy4; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		break;
	}
	case MSRcStrip::TYPE_G:
	{
		if (!CheckBars(Bar1))
		{
			if (bShortStrip) { errorBar = _T("X1");	cellType = ctSlabLx1; }
			else { errorBar = _T("Y1");	cellType = ctSlabLy1; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		if (!CheckBars(Bar3))
		{
			if (bShortStrip) { errorBar = _T("X3");	cellType = ctSlabLx3; }
			else { errorBar = _T("Y3");	cellType = ctSlabLy3; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		if (!CheckBars(Bar4))
		{
			if (bShortStrip) { errorBar = _T("X4");	cellType = ctSlabLx4; }
			else { errorBar = _T("Y4");	cellType = ctSlabLy4; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		break;
	}
	case MSRcStrip::TYPE_H:
	{
		if (!CheckBars(Bar2))
		{
			if (bShortStrip) { errorBar = _T("X2");	cellType = ctSlabLx2; }
			else { errorBar = _T("Y2");	cellType = ctSlabLy2; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}

		if (!CheckBars(Bar3))
		{
			if (bShortStrip) { errorBar = _T("X3");	cellType = ctSlabLx3; }
			else { errorBar = _T("Y3");	cellType = ctSlabLy3; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		if (!CheckBars(Bar4))
		{
			if (bShortStrip) { errorBar = _T("X4");	cellType = ctSlabLx4; }
			else { errorBar = _T("Y4");	cellType = ctSlabLy4; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		break;
	}
	case MSRcStrip::TYPE_I:
	{
		if (!CheckBars(Bar1))
		{
			if (bShortStrip) { errorBar = _T("X1");	cellType = ctSlabLx1; }
			else { errorBar = _T("Y1");	cellType = ctSlabLy1; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		if (!CheckBars(Bar4))
		{
			if (bShortStrip) { errorBar = _T("X4");	cellType = ctSlabLx4; }
			else { errorBar = _T("Y4");	cellType = ctSlabLy4; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		if (!CheckBars(Bar7))
		{
			if (bShortStrip) { errorBar = _T("X7");	cellType = ctSlabLx7; }
			else { errorBar = _T("Y7");	cellType = ctSlabLy7; }
			bRtnVal = false;
			CreateSlabErrorObject(pTable, pSlabInfo, errorBar, cellType);
		}
		break;
	}
	}
	return bRtnVal;
}

void MTReader::SetStripInfo(bool bShortStrip, MSRcSlabM* pSlabM, SRSlabInfo* pSlabInfo)
{
	bool IsBent = false;
	if (m_SlabX3Y3Type == SLAB_X3Y3_TYPE_BENT)
		IsBent = true;
	MSRcStrip::STRIP_TYPE StripType = pSlabInfo->GetStripType(bShortStrip, IsBent);
	CString Bar1, Bar2, Bar3, Bar4, Bar5, Bar6, Bar7, Bar8, Bar9, Bar10;

	MSRcStrip* pStrip = &pSlabM->m_ShortStrip;
	if (bShortStrip == false)
		pStrip = &pSlabM->m_LongStrip;

	if (StripType == MSRcStrip::TYPE_NONE)
	{
		pStrip->m_StripType = MSRcStrip::TYPE_ALL;
		if (bShortStrip)
		{
			Bar1 = pSlabInfo->m_X1;	Bar2 = pSlabInfo->m_X2;	Bar3 = pSlabInfo->m_X3;	Bar4 = pSlabInfo->m_X4;	Bar5 = pSlabInfo->m_X5;
			Bar6 = pSlabInfo->m_X6;	Bar7 = pSlabInfo->m_X7;	Bar8 = pSlabInfo->m_X8;	Bar9 = pSlabInfo->m_X9;	Bar10 = pSlabInfo->m_X10;
		}
		else
		{
			Bar1 = pSlabInfo->m_Y1;	Bar2 = pSlabInfo->m_Y2;	Bar3 = pSlabInfo->m_Y3;	Bar4 = pSlabInfo->m_Y4;	Bar5 = pSlabInfo->m_Y5;
			Bar6 = pSlabInfo->m_Y6;	Bar7 = pSlabInfo->m_Y7;	Bar8 = pSlabInfo->m_Y8;	Bar9 = pSlabInfo->m_Y9;	Bar10 = pSlabInfo->m_Y10;
		}
		bool IsUsed1 = Convert_MSStirrupBar(Bar1, pStrip->m_Bar1);
		bool IsUsed2 = Convert_MSStirrupBar(Bar2, pStrip->m_Bar2);
		bool IsUsed3 = Convert_MSStirrupBar(Bar3, pStrip->m_Bar3);
		bool IsUsed4 = Convert_MSStirrupBar(Bar4, pStrip->m_Bar4);
		bool IsUsed5 = Convert_MSStirrupBar(Bar5, pStrip->m_Bar5);
		bool IsUsed6 = Convert_MSStirrupBar(Bar6, pStrip->m_Bar6);
		bool IsUsed7 = Convert_MSStirrupBar(Bar7, pStrip->m_Bar7);
		bool IsUsed8 = Convert_MSStirrupBar(Bar8, pStrip->m_Bar8);
		bool IsUsed9 = Convert_MSStirrupBar(Bar9, pStrip->m_Bar9);
		bool IsUsed10 = Convert_MSStirrupBar(Bar10, pStrip->m_Bar10);

		if (bShortStrip)
		{
			pSlabM->m_IsShort1 = IsUsed1;	pSlabM->m_IsShort2 = IsUsed2;	pSlabM->m_IsShort3 = IsUsed3;	pSlabM->m_IsShort4 = IsUsed4;	pSlabM->m_IsShort5 = IsUsed5;
			pSlabM->m_IsShort6 = IsUsed6;	pSlabM->m_IsShort7 = IsUsed7;	pSlabM->m_IsShort8 = IsUsed8;	pSlabM->m_IsShort9 = IsUsed9;	pSlabM->m_IsShort10 = IsUsed10;
		}
		else
		{
			pSlabM->m_IsLong1 = IsUsed1;	pSlabM->m_IsLong2 = IsUsed2;	pSlabM->m_IsLong3 = IsUsed3;	pSlabM->m_IsLong4 = IsUsed4;	pSlabM->m_IsLong5 = IsUsed5;
			pSlabM->m_IsLong6 = IsUsed6;	pSlabM->m_IsLong7 = IsUsed7;	pSlabM->m_IsLong8 = IsUsed8;	pSlabM->m_IsLong9 = IsUsed9;	pSlabM->m_IsLong10 = IsUsed10;
		}
		return;
	}

	if (bShortStrip)
	{
		Bar1 = pSlabInfo->m_X1;	Bar2 = pSlabInfo->m_X2;	Bar3 = pSlabInfo->m_X3;	Bar4 = pSlabInfo->m_X4;	Bar5 = pSlabInfo->m_X5;	Bar6 = pSlabInfo->m_X6;
		Bar7 = pSlabInfo->m_X7;
		pSlabM->SetIsShortBarFlag(false);
	}
	else
	{
		Bar1 = pSlabInfo->m_Y1;	Bar2 = pSlabInfo->m_Y2;	Bar3 = pSlabInfo->m_Y3;	Bar4 = pSlabInfo->m_Y4;	Bar5 = pSlabInfo->m_Y5;	Bar6 = pSlabInfo->m_Y6;
		Bar7 = pSlabInfo->m_Y7;
		pSlabM->SetIsLongBarFlag(false);
	}
	pStrip->m_StripType = StripType;
	Convert_MSStirrupBar(Bar5, pStrip->m_Bar5);
	Convert_MSStirrupBar(Bar6, pStrip->m_Bar6);
	switch (StripType)
	{
	case MSRcStrip::TYPE_A:
		Convert_MSStirrupBar(Bar1, pStrip->m_Bar1);
		Convert_MSStirrupBar(Bar4, pStrip->m_Bar4);
		break;
	case MSRcStrip::TYPE_B:
		Convert_MSStirrupBar(Bar2, pStrip->m_Bar2);
		Convert_MSStirrupBar(Bar4, pStrip->m_Bar4);
		break;
	case MSRcStrip::TYPE_C:
		Convert_MSStirrupBar(Bar2, pStrip->m_Bar2);
		Convert_MSStirrupBar(Bar7, pStrip->m_Bar7);
		Convert_MSStirrupBar(Bar4, pStrip->m_Bar4);
		break;
	case MSRcStrip::TYPE_D:
		Convert_MSStirrupBar(Bar1, pStrip->m_Bar1);
		Convert_MSStirrupBar(Bar2, pStrip->m_Bar2);
		Convert_MSStirrupBar(Bar7, pStrip->m_Bar7);
		Convert_MSStirrupBar(Bar4, pStrip->m_Bar4);
		break;
	case MSRcStrip::TYPE_E:
		Convert_MSStirrupBar(Bar1, pStrip->m_Bar1);
		Convert_MSStirrupBar(Bar2, pStrip->m_Bar2);
		Convert_MSStirrupBar(Bar4, pStrip->m_Bar4);
		break;
	case MSRcStrip::TYPE_F:
		Convert_MSStirrupBar(Bar3, pStrip->m_Bar3);
		Convert_MSStirrupBar(Bar4, pStrip->m_Bar4);
		break;
	case MSRcStrip::TYPE_G:
		Convert_MSStirrupBar(Bar1, pStrip->m_Bar1);
		Convert_MSStirrupBar(Bar3, pStrip->m_Bar3);
		Convert_MSStirrupBar(Bar4, pStrip->m_Bar4);
		break;
	case MSRcStrip::TYPE_H:
		Convert_MSStirrupBar(Bar2, pStrip->m_Bar2);
		Convert_MSStirrupBar(Bar3, pStrip->m_Bar3);
		Convert_MSStirrupBar(Bar4, pStrip->m_Bar4);
		break;
	case MSRcStrip::TYPE_I:
		Convert_MSStirrupBar(Bar1, pStrip->m_Bar1);
		Convert_MSStirrupBar(Bar7, pStrip->m_Bar7);
		Convert_MSStirrupBar(Bar4, pStrip->m_Bar4);
		break;
	case MSRcStrip::TYPE_ALL:
		Convert_MSStirrupBar(Bar1, pStrip->m_Bar1);
		Convert_MSStirrupBar(Bar2, pStrip->m_Bar2);
		Convert_MSStirrupBar(Bar3, pStrip->m_Bar3);
		Convert_MSStirrupBar(Bar4, pStrip->m_Bar4);
		Convert_MSStirrupBar(Bar7, pStrip->m_Bar7);
		Convert_MSStirrupBar(Bar8, pStrip->m_Bar8);
		Convert_MSStirrupBar(Bar9, pStrip->m_Bar9);
		Convert_MSStirrupBar(Bar10, pStrip->m_Bar10);
		break;
	}
	pSlabM->CheckShortnLongBarFlag();
}

BOOL MTReader::CheckMainBarDia(CString strTopBar, CString strLeftBar)
{
	CString sTopDiaName = GetDia2MainBar(strTopBar);
	CString sLeftDiaName = GetDia2MainBar(strLeftBar);
	if (sTopDiaName != sLeftDiaName)	return FALSE;
	return TRUE;
}
BOOL MTReader::CheckStrpBarDia(CString strTopBar, CString strCenBar)
{
	CString sTopDiaName = GetDia2StrpBar(strTopBar);
	CString sCenDiaName = GetDia2StrpBar(strCenBar);
	if (sTopDiaName != sCenDiaName)	return FALSE;
	return TRUE;
}
long MTReader::GetBarNumArrbyString(CArray<long, long>& BarNumArr, CString strData)
{
	BarNumArr.RemoveAll();

	long iStartIdx = 0;
	long nDashIndex = strData.Find('-');
	if (nDashIndex == -1)	return 0;

	for (long ichar = 0; ichar < nDashIndex; ichar++)
	{
		char aChar = strData.GetAt(ichar);
		if (aChar == '/')
		{
			if (ichar == 0)	return 0;
			CString strBar = strData.Mid(iStartIdx, ichar - iStartIdx);
			long NumBar = _ttol(strBar);
			if (NumBar <= 0)	return 0;
			BarNumArr.Add(NumBar);
			iStartIdx = ichar + 1;
			continue;
		}
		if (ichar + 1 == nDashIndex)
		{
			CString strBar = strData.Mid(iStartIdx, ichar + 1 - iStartIdx);
			long NumBar = _ttol(strBar);
			if (NumBar <= 0)	return 0;
			BarNumArr.Add(NumBar);
		}
	}

	return BarNumArr.GetSize();
}

BOOL MTReader::CheckMainBars(CString strBar)
{
	CString sRebarText = strBar.MakeUpper();
	sRebarText.Replace(_T(" "), _T(""));
	long nSlashIndex = sRebarText.Find('/');
	CString CheckString;
	if (nSlashIndex == -1)
	{
		CString sDiaName = GetDia2MainBar(sRebarText);
		if (CheckDia(_ttof(sDiaName)) == FALSE)	return FALSE;

		int indexDash = sRebarText.Find('-');
		if (indexDash == -1)
			return FALSE;
		int numBar = _ttol(sRebarText.Mid(0, indexDash));
		if (numBar < 0)
			return FALSE;
		return TRUE;
	}
	else
	{

		long nLength = sRebarText.GetLength();
		long NumBar1 = _ttol(sRebarText.Mid(0, nSlashIndex));
		long nDashIndex = sRebarText.Find('-');
		if (nDashIndex == -1)	return FALSE;
		if (NumBar1 == 0)	return FALSE;

		CString strTemp = sRebarText.Mid(nDashIndex, nLength - nDashIndex);
		CString strBar1, strBar2;
		strBar1.Format(_T("%ld%s"), NumBar1, strTemp);
		CString sDiaName = GetDia2MainBar(strBar1);
		if (CheckDia(_ttof(sDiaName)) == FALSE)	return FALSE;

		long NumBar2 = _ttol(sRebarText.Mid(nSlashIndex + 1, nDashIndex - (nSlashIndex + 1)));
		if (NumBar2 == 0)	return TRUE;
		strBar2.Format(_T("%ld%s"), NumBar2, strTemp);
		sDiaName = GetDia2MainBar(strBar2);
		if (CheckDia(_ttof(sDiaName)) == FALSE)	return FALSE;

		if (NumBar2 > NumBar1)	return FALSE;

		return TRUE;
	}
}
BOOL MTReader::CheckTieBar(double dLength, long NumMainBars, long NumTieBars)
{
	double dSpacing = dLength / (NumMainBars - 1);
	if (dSpacing - 150 > DTOL_GM)
	{
		if (NumTieBars != NumMainBars - 2)
			return FALSE;
	}
	else
	{
		if (NumTieBars != (NumMainBars - 2) / 2)
			return FALSE;
	}
	return TRUE;
}
BOOL MTReader::GetNumBars(CString strBar)
{
	long nSlashIndex = strBar.Find('/');
	CString CheckString;
	if (nSlashIndex == -1)
	{
		long nDashIndex = strBar.Find('-');
		if (nDashIndex == -1)	return 0;
		return _ttol(strBar.Mid(0, nDashIndex));
	}
	else
		return _ttol(strBar.Mid(0, nSlashIndex));
}

BOOL MTReader::CheckBars(CString strBar)
{
	long nAtIndex = strBar.Find('@');
	if (nAtIndex == -1)	return CheckMainBars(strBar);
	else							return CheckStrpBars(strBar);
}
BOOL MTReader::CheckStrpBars(CString strBar)
{
	CString sDiaName = GetDia2StrpBar(strBar);
	if (CheckDia(_ttof(sDiaName)) == FALSE)	return FALSE;
	long nPlusIndex = strBar.Find('+');
	long nAtIndex = strBar.Find('@');
	if (nPlusIndex != -1 && nAtIndex != -1)
	{
		sDiaName = strBar.Mid(nPlusIndex + 1, nAtIndex - (nPlusIndex + 1));
		if (CheckDia(_ttof(sDiaName)) == FALSE)	return FALSE;
	}
	if (nAtIndex == -1)
		return FALSE;
	int nSpacing = 0;
	long nPLIndex = strBar.Find('(');
	if (nPLIndex == -1)
		nSpacing = _ttol(strBar.Mid(nAtIndex + 1, strBar.GetLength() - (nAtIndex + 1)));
	else
	{
		nSpacing = _ttol(strBar.Mid(nAtIndex + 1, (nPLIndex - 1) - nAtIndex));
	}
	if (nSpacing <= 0)
		return FALSE;

	return TRUE;
}
BOOL MTReader::CheckShearAddBar(CString strBar, long nShearAddLength)
{
	if (strBar == "" && nShearAddLength < DTOL1)	return TRUE;
	else if (strBar == "" && nShearAddLength > DTOL1)	return FALSE;
	else if (strBar != "" && nShearAddLength < DTOL1)	return FALSE;

	CString sDiaName = GetDia2StrpBar(strBar);
	if (CheckDia(_ttof(sDiaName)) == FALSE)	return FALSE;

	long nLength = strBar.GetLength();
	long nAtIndex1 = strBar.Find('@');
	long nAtIndex2 = strBar.ReverseFind('@');
	if (nAtIndex1 == nLength - (nAtIndex2 + 1))	return FALSE;

	return TRUE;
}

CString MTReader::GetDia2MainBar(CString strBar)
{
	CString sRebarText = strBar.MakeUpper();
	sRebarText.Replace(_T(" "), _T(""));
	long nLength = sRebarText.GetLength();
	long nIndex = sRebarText.Find('D');
	if (nIndex == -1)	return _T("");
	return sRebarText.Mid(nIndex + 1, nLength - (nIndex + 1));
}

CString MTReader::GetDia2StrpBar(CString strBar)
{
	CString sRebarText = strBar.MakeUpper();
	sRebarText.Replace(_T(" "), _T(""));
	long nLength = sRebarText.GetLength();
	long nDIndex = sRebarText.Find('D');
	long nAtIndex = sRebarText.Find('@');
	if (nDIndex == -1 || nAtIndex == -1)	return _T("");

	return sRebarText.Mid(nDIndex + 1, (nAtIndex - (nDIndex + 1)));
}

BOOL MTReader::CheckDia(double dDia)
{
	CString sDiaName;
	long nDia = (long)dDia;
	if (fabs(dDia - nDia) < 0.1)
		sDiaName.Format(_T("%ld"), nDia);
	else
		sDiaName.Format(_T("%.1f"), dDia);

	if (MSDeformedBar::FindBarDiameterIndexByName(sDiaName) == -1)	return FALSE;
	return TRUE;
}

bool MTReader::Convert_MSMainBar(CString strBar, MSMainBar& aMainBar)
{
	aMainBar.ma_strBar.RemoveAll();
	long indexSemiColon = strBar.Find(';');
	if (indexSemiColon != -1)
	{
		int nTokenPos = 0;
		CString strToken = strBar.Tokenize(_T(";"), nTokenPos);

		aMainBar.InitData(strToken);

		while (!strToken.IsEmpty())
		{
			strToken = strBar.Tokenize(_T(";"), nTokenPos);
			if (!strToken.IsEmpty())
				aMainBar.Add(strToken);
		}
		return true;
	}
	long nSlashIndex = strBar.Find('/');
	CString CheckString;
	if (nSlashIndex == -1)
	{
		long nDashIndex = strBar.Find('-');
		long nDIndex = strBar.Find('D');
		if (nDashIndex == -1)	return false;

		long NumBars = _ttol(strBar.Mid(0, nDashIndex));
		//		CString strType = strBar.Mid(nDashIndex+1, nDIndex-nDashIndex);
		CString strType = MSDeformedBar::m_sDefaultBarType;
		CString sDiaName = GetDia2MainBar(strBar);

		CString strNewBar;
		strNewBar.Format(_T("%ld-%s%s"), NumBars, strType, sDiaName);
		aMainBar.ma_strBar.Add(strNewBar);
		return TRUE;
	}
	else
	{
		long nLength = strBar.GetLength();
		long NumBar1 = _ttol(strBar.Mid(0, nSlashIndex));
		long nDIndex = strBar.Find('D');
		if (nDIndex == -1)	return false;

		CString strTemp = strBar.Mid(nDIndex, nLength - nDIndex);
		CString strBar1, strBar2;
		strBar1.Format(_T("%ld-%s%s"), NumBar1, MSDeformedBar::m_sDefaultBarType, strTemp);

		aMainBar.ma_strBar.Add(strBar1);

		int nDashIndex = strBar.Find('-');
		if (nDashIndex == -1)	return false;
		long NumBar2 = _ttol(strBar.Mid(nSlashIndex + 1, nDashIndex - (nSlashIndex + 1)));
		if (NumBar2 == 0)	return TRUE;
		strBar2.Format(_T("%ld%s"), NumBar2, strTemp);

		aMainBar.ma_strBar.Add(strBar2);

		return TRUE;
	}
}
bool MTReader::Convert_MSStirrupBar(CString strBar, MSStirrupBar& aStirrupBar)
{
	if (strBar == _T(""))
		return false;

	long iStartIdx = 0;
	long nDIndex = strBar.Find('D');
	long nAtIndex = strBar.Find('@');
	long nPlusIndex = strBar.Find('+');

	if (nDIndex == -1 || nAtIndex == -1)	return false;

	if (nPlusIndex == -1)
	{
		MSStirrupBar tempBar;
		tempBar.InitData(strBar);
		CString sDiaName = tempBar.GetBarDiameterName();
		int nSpacing = (int)tempBar.GetSpacing();
		CString theBar;
		theBar.Format(_T("%s%s@%ld"), MSDeformedBar::m_sDefaultBarType, sDiaName, nSpacing);
		aStirrupBar = theBar;
	}
	else
	{
		long NumLength = strBar.GetLength();
		CString strBarType = MSDeformedBar::m_sDefaultBarType;
		CString strSpacing = strBar.Mid(nAtIndex + 1, NumLength - nAtIndex - 1);
		CString strFirstDia = strBar.Mid(nDIndex + 1, nPlusIndex - nDIndex - 1);
		CString strSecondDia = strBar.Mid(nPlusIndex + 1, nAtIndex - nPlusIndex - 1);

		CString strData;

		strData.Format(_T("%s%s@%ld"), strBarType, strFirstDia, _ttol(strSpacing));
		aStirrupBar.InitData(strData);
		strData.Format(_T("%s%s@%ld"), strBarType, strSecondDia, _ttol(strSpacing));
		aStirrupBar.Add(strData);
		aStirrupBar.SetAlternate(TRUE);
	}
	return true;
}

MTRecognizeSection* MTReader::MakeRecognizeSection(GM2DRectangle* pRect, SRSectionTable* pTable, vector<MSCompoM*>& CompoMs)
{
	MTRecognizeSection* pRecognize = new MTRecognizeSection();
	pRecognize->SetSectionTable(pTable);
	pRecognize->AddCompoMs(CompoMs);
	mm_RectToSection.insert(make_pair(pRect, pRecognize));
	return pRecognize;
}


GM2DRectangle* MTReader::MakeRecognizeBox(MTResultElement* firstResult, GM2DRectangle* pBoxRect)
{
	if (firstResult == nullptr)   return nullptr;

	GM2DRectangle* pRecognizeBox = new GM2DRectangle();
	pRecognizeBox->m_Origin.SetXY(pBoxRect->m_Origin.m_X, pBoxRect->m_Origin.m_Y);
	pRecognizeBox->m_Corner.SetXY(pBoxRect->m_Corner.m_X, pBoxRect->m_Corner.m_Y);

	mm_RecognizeBox.insert(make_pair(firstResult, pRecognizeBox));
	return pRecognizeBox;
}

int MTReader::CreateSectionTable(CString Name)
{
	SRSectionTable* pTable = new SRSectionTable(this);;
	pTable->mp_Name = new GMText();
	pTable->mp_Name->m_strText = Name;
	pTable->mp_Rect = new GM2DRectangle();

	SRSectionBox* pBox = new SRSectionBox(pTable);
	pBox->mp_Rect = new GM2DRectangle();
	pTable->ma_Box.Add(pBox);
	return ma_Table.Add(pTable);
}

int MTReader::AddSectionBoxData(int tableIndex, CellType type, int row, CString data)
{
	int NumTable = ma_Table.GetSize();
	if (tableIndex < 0 || tableIndex >= NumTable)
		return -1;

	SRSectionTable* pTable = ma_Table[tableIndex];
	return pTable->AddBoxData(type, row, data);
}

int MTReader::FindTextFigureID(GMText* pText)
{
	if (pText == NULL)	return 0;
	map<GMText*, int>::iterator it = mm_TextToFigureID.find(pText);
	if (it == mm_TextToFigureID.end())
		return 0;

	return it->second;
}

void MTReader::AddErrorFigureID(int figureID)
{
	if (figureID == 0)
		return;
	ma_ErrorFigureID.push_back(figureID);
}
void MTReader::AddErrorFigureIDs(vector<int> figureIDs)
{
	for (int i = 0; i < figureIDs.size(); i++)
		ma_ErrorFigureID.push_back(figureIDs[i]);
}

bool MTReader::IsTopStory(CString strStory)
{
	CStringArray topStorys;
	topStorys.Add(_T("최상층"));		topStorys.Add(_T("ROOF"));		topStorys.Add(_T("RF"));

	int nCount = topStorys.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		if (topStorys[i] == strStory)
			return true;
	}

	return false;
}

bool MTReader::IsBotStory(CString strStory)
{
	CStringArray topStorys;
	topStorys.Add(_T("최하층"));

	int nCount = topStorys.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		if (topStorys[i] == strStory)
			return true;
	}

	return false;
}

bool MTReader::IsUnderStory(CString strStory)
{
	CStringArray botStorys;
	botStorys.Add(_T("B"));	botStorys.Add(_T("지하"));	botStorys.Add(_T("-"));

	int nCount = botStorys.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		if (strStory.Find(botStorys[i]) != -1)
			return true;
	}

	return false;
}

bool MTReader::IsAboveStory(CString strStory)
{
	CStringArray topStorys;
	topStorys.Add(_T("지상"));

	int nCount = topStorys.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		if (strStory.Find(topStorys[i]) != -1)
			return true;
	}

	return false;
}

void MTReader::Parsing_strStory(CString strStory, long& nFrom, long& nTo)
{
	if (m_eRecognizeStoryType == eRecognizeStoryType::FloorNum)
		Parsing_StroyByFloorNum(strStory, nFrom, nTo);
	else if (m_eRecognizeStoryType == eRecognizeStoryType::FloorName)
		Parsing_StoryByFloorName(strStory, nFrom, nTo);
}

void MTReader::Parsing_StoryByFloorName(CString strStory, long& nFrom, long& nTo)
{
	nFrom = nTo = 0;

	MSBaseBuilding* pBldg = mp_Bldg;
	if (pBldg == nullptr)	return;

	if (strStory == _T(""))	return;

	CString TempVal = strStory;
	TempVal.Replace(_T(" "), _T(""));
	TempVal.MakeUpper();

	int nLength = TempVal.GetLength();

	int Idx = -1;
	int nStorySeperate = ma_StorySeperate.size();
	for (int iSeperate = 0; iSeperate < nStorySeperate; iSeperate++)
	{
		CString sKey = ma_StorySeperate[iSeperate];
		Idx = TempVal.Find(sKey);
		if (Idx >= 0)
			break;
	}
	if (Idx >= 0)
	{
		CString strFrom = TempVal.Left(Idx);
		CString strTo = TempVal.Right(nLength - Idx - 1);
		nFrom = FindFloorNumByName(strFrom);
		nTo = FindFloorNumByName(strTo);
	}
	else
	{
		nFrom = nTo = FindFloorNumByName(TempVal);
	}
	if (nFrom != 0 && nTo != 0)
	{
		if (nFrom > nTo)
		{
			long nTemp = nFrom;
			nFrom = nTo;
			nTo = nTemp;
		}
	}
}

long MTReader::FindFloorNumByName(CString name)
{
	if (mp_Bldg == nullptr)	return 0;
	CString upperName = name.MakeUpper();
	vector<MSFloor*> floorArr = mp_Bldg->GetAllFloors();
	for (int idx = 0; idx < floorArr.size(); idx++)
	{
		MSFloor* pFloor = floorArr[idx];
		if (pFloor == nullptr) continue;;
		if (pFloor->GetName().MakeUpper() == upperName)
			return pFloor->GetFloorNum();
	}

	return 0;
}
void MTReader::Parsing_StroyByFloorNum(CString strStory, long& nFrom, long& nTo)
{
	nFrom = nTo = 0;

	MSBaseBuilding* pBldg = mp_Bldg;
	if (pBldg == NULL)	return;

	long nLimitFromStory = -(pBldg->GetNumUnderStory() - 1);
	if (nLimitFromStory == 0)	nLimitFromStory = 1;
	long nLimitToStory = pBldg->GetNumAboveStory() - 1;
	if (nLimitToStory == 0)	nLimitToStory = -1;
	if (nLimitToStory < nLimitFromStory)	return;

	if (strStory == _T(""))	return;

	CString TempVal = strStory;
	TempVal.Replace(_T(" "), _T(""));
	TempVal.MakeUpper();

	int nLength = TempVal.GetLength();

	int Idx = -1;
	int nStorySeperate = ma_StorySeperate.size();
	for (int iSeperate = 0; iSeperate < nStorySeperate; iSeperate++)
	{
		CString sKey = ma_StorySeperate[iSeperate];
		Idx = TempVal.Find(sKey);
		if (Idx >= 0)
			break;
	}
	if (Idx >= 0)
	{
		CString strFrom = TempVal.Left(Idx);
		CString strTo = TempVal.Right(nLength - Idx - 1);
		int nSubIdx = 0;

		if (IsBotStory(strFrom))
			nFrom = nLimitFromStory;
		else if (IsTopStory(strFrom))
			nFrom = nLimitToStory;
		else
		{
			nFrom = FindNumberData(0, strFrom);
			if (IsUnderStory(strFrom))	nFrom *= -1;
		}

		if (IsBotStory(strTo))
			nTo = nLimitFromStory;
		else if (IsTopStory(strTo))
			nTo = nLimitToStory;
		else
		{
			nTo = FindNumberData(0, strTo);
			if (IsUnderStory(strTo))	nTo *= -1;
		}
	}
	else  // 찾지 못했다면 하나의 층으로 이루어진 것이다.
	{
		if (IsBotStory(TempVal))
		{
			nFrom = nTo = nLimitFromStory;
			return;
		}
		if (IsTopStory(TempVal))
		{
			nFrom = nTo = nLimitToStory;
			return;
		}

		if (TempVal == "지하층")
		{
			nFrom = nLimitFromStory;
			nTo = -1;
			return;
		}
		else if (TempVal == "지상층")
		{
			nFrom = 1;
			nTo = nLimitToStory;
			return;
		}
		else
		{
			nFrom = nTo = FindNumberData(0, TempVal);
			if (IsUnderStory(TempVal))
			{
				nFrom = nTo = -1 * nFrom;
			}
		}
	}
	if (nFrom != 0 && nTo != 0)
	{
		if (nFrom > nTo)
		{
			long nTemp = nFrom;
			nFrom = nTo;
			nTo = nTemp;
		}
	}
}

void MTReader::AddErrorName(CString Name)
{
	ma_ErrorName.push_back(Name);
}

void MTReader::AddRegisterM(MSCompoM* pCompoM)
{
	ma_RegisterM.push_back(pCompoM);
}

bool MTReader::MakeDisplayCompoMDatas(GM2DRectangle* pRect, vector<MTRecognizeData*>& DataArr)
{
	map<GM2DRectangle*, MTRecognizeSection*>::iterator it = mm_RectToSection.find(pRect);
	if (it == mm_RectToSection.end())
		return false;

	MTRecognizeSection* pSection = it->second;
	if (m_Group == GROUP_COLUMN)
		return MakeDisplayColumnMDatas(pSection, DataArr);
	else if (m_Group == GROUP_BEAM)
		return MakeDisplayBeamMDatas(pSection, DataArr);
	else if (m_Group == GROUP_WALL)
		return MakeDisplayWallMDatas(pSection, DataArr);
	else if (m_Group == GROUP_SLAB)
		return MakeDisplaySlabMDatas(pSection, DataArr);
	else if (m_Group == GROUP_BUTTRESS)
		return MakeDisplayButtressMDatas(pSection, DataArr);
	else if (m_Group == GROUP_RWALL)
		return MakeDisplayRWallMDatas(pSection, DataArr);
	return true;
}

bool MTReader::MakeDisplayColumnMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	if (m_MaterialType == MSMaterial::msConcrete)
		return MakeDisplayRcColumnMDatas(pSection, DataArr);
	else if (m_MaterialType == MSMaterial::msSteel)
		return MakeDisplaySteelColumnMDatas(pSection, DataArr);
	else if (m_MaterialType == MSMaterial::msSRC)
		return MakeDisplaySrcColumnMDatas(pSection, DataArr);
	return false;
}

bool MTReader::MakeDisplayRcColumnMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	MTDisplayRcColumnMHelper aHelper(mp_Bldg, this);
	return aHelper.MakeData(pSection, DataArr);
}

bool MTReader::MakeDisplaySteelColumnMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	MTDisplaySteelColumnMHelper aHelper(mp_Bldg, this);
	return aHelper.MakeData(pSection, DataArr);
}

bool MTReader::MakeDisplaySrcColumnMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	MTDisplaySrcColumnMHelper aHelper(mp_Bldg, this);
	return aHelper.MakeData(pSection, DataArr);
}

bool MTReader::MakeDisplayBeamMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	if (m_MaterialType == MSMaterial::msConcrete)
		return MakeDisplayRcBeamMDatas(pSection, DataArr);
	else if (m_MaterialType == MSMaterial::msSteel)
		return MakeDisplaySteelBeamMDatas(pSection, DataArr);
	else if (m_MaterialType == MSMaterial::msSRC)
		return MakeDisplaySrcBeamMDatas(pSection, DataArr);
	return false;
}

bool MTReader::MakeDisplayRcBeamMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	MTDisplayRcBeamMHelper aHelper(mp_Bldg, this);
	return aHelper.MakeData(pSection, DataArr);
}

bool MTReader::MakeDisplaySteelBeamMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	MTDisplaySteelBeamMHelper aHelper(mp_Bldg, this);
	return aHelper.MakeData(pSection, DataArr);
}

bool MTReader::MakeDisplaySrcBeamMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	MTDisplaySrcBeamMHelper aHelper(mp_Bldg, this);
	return aHelper.MakeData(pSection, DataArr);
}

bool MTReader::MakeDisplayWallMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	if (m_nReadType == eReadType::General)
	{
		MTDisplayWallMHelper aHelper(mp_Bldg, this);
		return aHelper.MakeData(pSection, DataArr);
	}
	else if (m_nReadType == eReadType::LH_WALL)
	{
		MTDisplayWallMHelper_LH aHelper(mp_Bldg, this);
		return aHelper.MakeData(pSection, DataArr);
	}
	return false;
}

bool MTReader::MakeDisplaySlabMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	MTDisplaySlabMHelper aHelper(mp_Bldg, this);
	return aHelper.MakeData(pSection, DataArr);
}

bool MTReader::MakeDisplayButtressMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	MTDisplayButtressMHelper aHelper(mp_Bldg, this);
	return aHelper.MakeData(pSection, DataArr);
}

bool MTReader::MakeDisplayRWallMDatas(MTRecognizeSection* pSection, vector<MTRecognizeData*>& DataArr)
{
	MTDisplayRWallMHelper aHelper(mp_Bldg, this);
	return aHelper.MakeData(pSection, DataArr);
}

void MTReader::SetLayerNameToCellTypesMap(CString layerName, int cellType)
{
	layerName = layerName.MakeUpper();
	layerName.Replace(_T(" "), _T(""));
	map<CString, vector<int>>::iterator it = mm_LayerNameToCellTypes.find(layerName);
	if (it == mm_LayerNameToCellTypes.end())
	{
		vector<int> cellTypes;	cellTypes.push_back(cellType);
		mm_LayerNameToCellTypes.insert(make_pair(layerName, cellTypes));
	}
	else
	{
		it->second.push_back(cellType);
	}

	for (it = mm_LayerNameToCellTypes.begin(); it != mm_LayerNameToCellTypes.end(); it++)
	{
		CString layer = it->first;
		vector<int> types = it->second;
		for (int i = 0; i < types.size(); i++)
		{
			int type = types[i];
		}
	}
}

vector<int> MTReader::GetCellTypesByLayerName(CString layerName)
{
	vector<int> cellTypes;
	map<CString, vector<int>>::iterator it = mm_LayerNameToCellTypes.find(layerName);
	if (it != mm_LayerNameToCellTypes.end())
		return it->second;
	return cellTypes;
}

void MTReader::AddTextByLayerName(SRSectionTable* pTable, CString layerName, GMText* pText)
{
	pTable->AddTextByLayerName(layerName, pText);
}

void MTReader::AddDimensionByLayerName(SRSectionTable* pTable, CString layerName, SRDimension* pDimension)
{
	pTable->AddDimensionByLayerName(layerName, pDimension);
}

bool MTReader::IsVerticalMember()
{
	switch (m_Group)
	{
	case GROUP_COLUMN:
	case GROUP_WALL:
	case GROUP_BUTTRESS:
	case GROUP_RWALL:
		return true;
	}
	return false;
}

int MTReader::FindFirstNumberIndex(CString sData)
{
	int nLength = sData.GetLength();
	for (int i = 0; i < nLength; i++)
	{
		char aChr = sData[i];
		if (aChr >= '0' && aChr <= '9')
			return i;
	}
	return -1;
}

long MTReader::FindNumberData(int startIndex, CString sData)
{
	double dNumber = _ttof(sData);
	if (dNumber > 0)	return ROUND(dNumber);

	int start = -1, end = -1;
	int nLength = sData.GetLength();
	for (int i = 0; i < nLength; i++)
	{
		if (i < startIndex)	continue;
		char aChr = sData[i];
		if (aChr >= '0' && aChr <= '9')
		{
			if (start == -1)
			{
				start = i;
				end = i;
			}
			else
				end = i;
		}
		else
		{
			if (start != -1)
				break;
		}
	}

	return ROUND(_ttof(sData.Mid(start, end - (start - 1))));
}

bool MTReader::CorrectiveStirrupBar(CString& sData)
{
	sData = sData.MakeUpper();
	sData.Replace(_T(" "), _T(""));

	int indexD = sData.Find('D');
	int indexAt = sData.Find('@');
	if (indexD == -1 || indexAt == -1)	return false;

	CString sDiaName = sData.Mid(indexD + 1, (indexAt - (indexD + 1)));
	CString preData = sData.Left(indexAt + 1);
	long Spacing = FindNumberData(indexAt + 1, sData);

	sData.Format(_T("%s%ld"), preData, Spacing);
	return true;
}

bool MTReader::CorrectiveMainBarSet(CString& sData)
{
	sData = sData.MakeUpper();
	sData.Replace(_T(" "), _T(""));

	RemoveParenthesesInString(sData);
	return true;
}

bool MTReader::CorrectiveMainBar(CString& sData)
{
	sData = sData.MakeUpper();
	sData.Replace(_T(" "), _T(""));

	RemoveParenthesesInString(sData);

	int indexDash = sData.Find('-');
	int indexD = sData.Find('D');
	if (indexDash == -1 || indexD == -1)	return false;

	int NumBars = _ttol(sData.Mid(0, indexDash));

	int start = -1, end = -1;
	int nLength = sData.GetLength();
	for (int i = indexD + 1; i < nLength; i++)
	{
		char aChr = sData[i];
		if ((aChr >= '0' && aChr <= '9') || aChr == '.')
		{
			if (start == -1)
			{
				start = i;
				end = i;
			}
			else
				end = i;
		}
		else
		{
			if (start != -1)
				break;
		}
	}

	CString sDiaName = sData.Mid(start, end - (start - 1));

	sData.Format(_T("%ld-%s%s"), NumBars, MSDeformedBar::m_sDefaultBarType, sDiaName);
	return true;
}

bool MTReader::RemoveParenthesesInString(CString& sData)
{
	while (true)
	{
		int index = sData.Find('(');
		if (index == -1)	break;
		RemoveParenthesesInString(index, sData);
	}

	return true;
}

void MTReader::RemoveParenthesesInString(int startIndex, CString& sData)
{
	int endIndex = sData.Find(')');
	if (endIndex == -1)	return;

	sData.Delete(startIndex, (endIndex - startIndex) + 1);
}

void MTReader::TestMsg()
{
	CString msg;

	msg = CFormatMsg(LocalDic::GetLocal(_T("메시지"), _T("FeatureRebarPlacer")));  StatusReporterWrapper::OutputMessage(msg);
	msg.Format(LocalDic::GetLocal(_T("메시지"), _T("FeatureRebarPlacer")));  StatusReporterWrapper::OutputMessage(msg);
	msg = CFormatMsg(LocalDic::GetLocal(_T("%1로의 언어설정이 성공했습니다!"), _T(" TEXT")), _T("AA"));  StatusReporterWrapper::OutputMessage(msg);
	msg.Format(CFormatMsg(LocalDic::GetLocal(_T("%1로의 언어설정이 성공했습니다!"), _T(" TEXT")), _T("AA"))); StatusReporterWrapper::OutputMessage(msg);
	msg = CFormatMsg(LocalDic::GetLocal(_T("%1 부재의 %2 정보가 올바르지 않습니다."), _T(" SectionRecognize")), _T("AA"), _T("BB"));  StatusReporterWrapper::OutputMessage(msg);
	msg = CFormatMsg(LocalDic::GetLocal(_T("A-평면오류-%1!d!"), _T(" TEXT")), 20);  StatusReporterWrapper::OutputMessage(msg);
	long aa = 20; int bb = 30; double cc = 25.5;
	//double은 표시가 안되는데 가능한 방법을 알면 방법 추가해주세요 https://msdn.microsoft.com/en-us/library/windows/desktop/ms647550(v=vs.85).aspx
	//https://msdn.microsoft.com/en-us/library/aa314330(v=vs.60).aspx
	msg = CFormatMsg(LocalDic::GetLocal(_T("%1!ld! 과 %2!d! 와 %3!g!"), _T(" TEXT")), aa, bb, cc);  StatusReporterWrapper::OutputMessage(msg);
}

#pragma region BH-5356 단면인식 개선 - 레이이 기반
bool MTReader::AddDictionaryTextToLayer(GMText* text, CString layer)
{
	auto itLayer = mm_LayerNameToCellTypes.find(layer);
	if (itLayer == mm_LayerNameToCellTypes.end())	return false;
	auto it = mm_TextToLayer.find(text);
	if (it != mm_TextToLayer.end())	return false;
	mm_TextToLayer.insert(make_pair(text, layer));
	return true;
}

MTReader::CellType MTReader::GetCellTypeByLayer(GMText* text)
{
	auto itLayer = mm_TextToLayer.find(text);
	if (itLayer != mm_TextToLayer.end())
	{
		CString layerName = itLayer->second;
		auto itCell = mm_LayerNameToCellTypes.find(layerName);
		if (itCell != mm_LayerNameToCellTypes.end())
			if (itCell->second.size() > 0)		return (MTReader::CellType)itCell->second[0];
	}
	return MTReader::ctNone;
}

MTReader::CellType MTReader::GetCellTypeByLayer(CString layerName)
{
	auto itCell = mm_LayerNameToCellTypes.find(layerName);
	if (itCell != mm_LayerNameToCellTypes.end())
	{
		if (itCell->second.size() > 0)
			return (MTReader::CellType)itCell->second[0];
	}

	return MTReader::ctNone;
}

void MTReader::InitailizeLayerToCellTypeDictionary()
{
	mm_LayerNameToCellTypes.clear();
}

MSCompoM::Type_MSCompoM MTReader::GetCompoMType()
{
	if (m_Group == GROUP_COLUMN)
	{
		switch (m_MaterialType)
		{
		case MSMaterial::msConcrete:					return MSCompoM::msColumnM;
		case MSMaterial::msPC:								return MSCompoM::msColumnM;
		case MSMaterial::msSRC:							return MSCompoM::msSrcColumnM;
		case MSMaterial::msSteel:							return MSCompoM::msSteelColumnM;
		}
	}
	else if (m_Group == GROUP_WALL)
	{
		return MSCompoM::msBWallM;
	}
	else if (m_Group == GROUP_BEAM)
	{
		switch (m_MaterialType)
		{
		case MSMaterial::msConcrete:					return MSCompoM::msBeamM;
		case MSMaterial::msPC:								return MSCompoM::msBeamM;
		case MSMaterial::msSRC:							return MSCompoM::msSrcBeamM;
		case MSMaterial::msSteel:							return MSCompoM::msSteelBeamM;
		}
	}
	else if (m_Group == GROUP_SLAB)
	{
		return MSCompoM::msSlabM;

	}
	else if (m_Group == GROUP_RWALL)
	{
		return MSCompoM::msRWallM;
	}
	else if (m_Group == GROUP_BRACE)
	{
		return MSCompoM::msBraceM;
	}
	else if (m_Group == GROUP_BUTTRESS)
	{
		return MSCompoM::msButtressM;
	}
	return MSCompoM::msNONE;
}
#pragma endregion

#pragma region  Slab
GMText* MTReader::MakeGMText(GM2DVector insertVec, CString text, double height, GM2DVector origin, GM2DVector corner)
{
	GMText* pText = new GMText;
	pText->m_strText = text;
	pText->m_Vec = insertVec;
	pText->m_dHeight = height;
	pText->m_Origin = origin;
	pText->m_Corner = corner;
	return pText;
}
void MTReader::AddCellTypeToGMText(int nCellType, GMText* pText)
{
	auto it = mm_CellTypeGMTexts.find(nCellType);
	vector<GMText*> TextArr;
	if (it == mm_CellTypeGMTexts.end())
	{
		TextArr.push_back(pText);
		mm_CellTypeGMTexts.insert(make_pair(nCellType, TextArr));
	}
	else
	{
		it->second.push_back(pText);
	}
}

bool MTReader::AddTextBySlab(GM2DVector insertVec, CString text, double degree, double height, GM2DVector origin, GM2DVector corner, CString layerName, int figureID)
{
	GM2DVector	txtVec = insertVec;
	CString sText = text.MakeUpper();
	if (sText == "" || sText == "-")	// 공백이나 "-"이면 무시
		return false;

	layerName.Replace(_T(" "), _T(""));
	layerName = layerName.MakeUpper();

	bool isFlatSlabTypeText = false;
	if (IsFlatSlabType(sText))
	{
		SRSectionTable* pTable = FindSectionTable(&txtVec, NULL);
		if (pTable != nullptr)  isFlatSlabTypeText = true;
	}

	long nSlabCellType = GetCellTypebyDictionary(sText);
	long nFlatCellType = GetCellTypebyDictionaryFlatSlab(sText);

	if (isFlatSlabTypeText == false)
	{
		if (nSlabCellType != MTReader::ctNone || nFlatCellType != MTReader::ctNone)
		{
			double dValue = txtVec.m_X;

			if (nSlabCellType != MTReader::ctNone)
			{
				m_LocCellTypeMap.SetAt(dValue, nSlabCellType);
				GMText* pText = MakeGMText(txtVec, sText, height, origin, corner);
				AddCellTypeToGMText(nSlabCellType, pText);
			}
			if (nFlatCellType != MTReader::ctNone)
			{
				m_LocCellTypeMapFlatSlab.SetAt(dValue, nFlatCellType);
				GMText* pText = MakeGMText(txtVec, sText, height, origin, corner);
				AddCellTypeToGMText(nFlatCellType, pText);
			}

			// 공통CellType도 m_locCellTypeMapFlatSlab에 넣어두고 FlatType일때도 찾을수 있도록 하자
			if (nSlabCellType != MTReader::ctNone && nFlatCellType == MTReader::ctNone)
			{
				if (nSlabCellType == MTReader::ctName || nSlabCellType == MTReader::ctThick || nSlabCellType == MTReader::ctSlabFlatType)
				{
					m_LocCellTypeMapFlatSlab.SetAt(dValue, nSlabCellType);
					GMText* pText = MakeGMText(txtVec, sText, height, origin, corner);
					AddCellTypeToGMText(nSlabCellType, pText);
				}
			}

			return true;
		}
	}
	SRSectionTable* pTable = FindSectionTable(&txtVec, NULL);
	if (pTable)
	{
		GMText* pText = MakeGMText(txtVec, sText, height, origin, corner);

		if (layerName == DG_STR_TEXT && pTable->mp_Name == NULL)
			pTable->mp_Name = pText;
		pTable->ma_Text.Add(pText);
		AddTextByLayerName(pTable, layerName, pText);
		mm_TextToFigureID.insert(make_pair(pText, figureID));

		AddDictionaryTextToLayer(pText, layerName);
		return true;
	}
	return false;
}

vector<MTResultElement*> MTReader::GetResultMList()
{
	return ma_ResultM;
}
#pragma endregion

#pragma region FlatSlab용
long MTReader::GetCellTypebyDictionaryFlatSlab(CString strKey)
{
	CString strCell = strKey.MakeUpper();
	strCell.Replace(_T(" "), _T(""));
	map<CString, long>::iterator it = mm_CellTypeDictionaryFlatSlab.find(strCell);
	if (it == mm_CellTypeDictionaryFlatSlab.end())	return 0;
	else
		return it->second;
}
void MTReader::SetCellTypebyDictionaryFlatSlab(CString strKey, int cellType)
{
	mm_CellTypeDictionaryFlatSlab.insert(make_pair(strKey, cellType));
}

#pragma endregion
