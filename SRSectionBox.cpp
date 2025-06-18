#include "StdAfx.h"
#include "SRSectionBox.h"
#include "SRSectionTable.h"
#include "MTReader.h"

#include "GMLib/GMText.h"
#include "GMLib/GM2DLineVector.h"

#include "StructureModel\MSRcLineMemberM.h"
#include "StructureModel\MRRcSection.h"
#include "StructureModel\MREdge.h"
#include "StructureModel\MRVBar.h"
#include "StructureModel\MRHBarByPoint.h"
#include "StructureModel\MRHBarPointByLoc.h"
#include "StructureModel\MSMainBar.h"
#include "StructureModel\MSStirrupBar.h"
#include "StructureModel\MSSpecificTieBar.h"
#include "StructureModel\MSMainBarSet.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// SRDimension ////////////////////////////////////////////////////////
void SRDimension::DeleteObjects()
{
	if(mp_Text)	delete mp_Text;
	if(mp_LVec)	delete mp_LVec;
	mp_EdgeLVec = NULL;
}

bool SRDimension::SetLength( CString strText )
{
	strText.Replace(_T(","), _T(""));
	m_Length = MTReader::FindNumberData(0, strText);
	if(m_Length < DTOL_GM)
		return false;
	return true;
}

bool SRDimension::SetLVec( GM2DVector& defVec1, GM2DVector& defVec2, GM2DVector& LineVec )
{
	GM2DLineVector aLineVec(defVec2, LineVec);
	GM2DVector orthoVec = aLineVec.OrthoBaseVector(defVec1);


	GM2DLineVector aLVec(defVec1, orthoVec);
	m_EdgePosition = aLVec.WhichSide(LineVec)*-1;

	if(aLVec.Length()<DTOL_GM || m_EdgePosition==0)
	{
		// lineVec이 defVec2와 같은 위치를 찍었을 경우
		m_EdgePosition = -1;
		if(!mp_LVec)
			mp_LVec = new GM2DLineVector;
		mp_LVec->m_P0 = defVec1;
		mp_LVec->m_P1 = defVec2;
		double dDegree = mp_LVec->GetDegree();
		if(dDegree > 90 && dDegree <= 270)
			mp_LVec->ReverseDirection();
	}
	else
	{
		double dDist = orthoVec.Distance(LineVec);
		GM2DLineVector dimLine(defVec1, orthoVec);
		GM2DLineVector aDimLVec = dimLine.NormalTranslate(dDist, LineVec);

		if(!mp_LVec)
			mp_LVec = new GM2DLineVector;
		mp_LVec->m_P0 = aDimLVec.m_P0;
		mp_LVec->m_P1 = aDimLVec.m_P1;
	}

	if(fabs(mp_LVec->Length()/m_Length-1.) < DTOL_GM)
		m_NonScale = false;
	else
	{
		if (m_Length < DTOL_GM || mp_LVec->Length() < DTOL_GM)
		{
			m_NonScale = false;
			m_Length = 1.0;
		}
		else
		{
			m_NonScale = true;
			mp_Owner->m_dFactor = m_Length / mp_LVec->Length();
		}
	}

	m_Radian = mp_LVec->GetRadian();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// SRWallInfo.cpp /////////////////////////////////////////////////////
SRWallInfo::SRWallInfo()
{
	Initialize();
}
SRWallInfo::~SRWallInfo()
{

}
void SRWallInfo::Initialize()
{
	m_IsMerged = false;

	m_StartLoc = 0;
	m_EndLoc = 0;

	m_nFromStory = 0;
	m_nToStory = 0;
	m_nMaterialID = 0;
	m_nThick = 0;
	m_VBar = "";
	m_HBar = "";
	m_ShearRatio = "";
	m_nRow = 0;
}
void SRWallInfo::SetFromStory(long story)
{
    m_nFromStory = story; 
}
void SRWallInfo::SetToStory(long story)
{
    m_nToStory = story; 
}

bool SRWallInfo::CheckData()
{
// 	if(m_nMaterialID == 0)
// 		return false;
	if(m_nThick < DTOL1)
		return false;
	if(m_VBar == "" || m_HBar == "")
		return false;

	return true;
}

bool SRWallInfo::CheckStory()
{
	if (m_nFromStory == 0 || m_nToStory == 0)
		return false;
	return true;
}

bool SRWallInfo::IsSameData(SRWallInfo* urWallInfo)
{
	if (m_nMaterialID != urWallInfo->m_nMaterialID)	return false;
	if (m_nThick != urWallInfo->m_nThick)	return false;
	if (m_VBar != urWallInfo->m_VBar)	return false;
	if (m_HBar != urWallInfo->m_HBar)	return false;
	if (m_ShearRatio != urWallInfo->m_ShearRatio)	return false;

	return true;
}

vector<int> SRWallInfo::GetTextFigureIDs(int nCellType)
{
	auto it = mm_CellTypeToFigureIDs.find(nCellType);
	if(it != mm_CellTypeToFigureIDs.end())
		return it->second;
	vector<int> Ids;
	return Ids;
}

void SRWallInfo::AddTextFigureID(int nCellType, int nFigureID)
{
	auto it = mm_CellTypeToFigureIDs.find(nCellType);
	if(it != mm_CellTypeToFigureIDs.end())	
	{
		it->second.push_back(nFigureID);
		return;
	}
	vector<int> figureIds;
	figureIds.push_back(nFigureID);
	mm_CellTypeToFigureIDs.insert(make_pair(nCellType, figureIds));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// SRSteelInfo.cpp //////////////////////////////////////////////////////////////

SRSteelInfo::SRSteelInfo()
{
	Initialize();
}

SRSteelInfo::~SRSteelInfo()
{

}

void SRSteelInfo::Initialize()
{
	m_StartLoc = m_EndLoc = 0;
	m_nMaterialID = 0;
}

bool SRSteelInfo::CheckData()
{
	if(m_Name == _T("") || m_SectionName == _T(""))
		return false;
	return true;
}

vector<int> SRSteelInfo::GetTextFigureIDs(int nCellType)
{
	vector<int> Ids;
	auto it = mm_CellTypeToFigureIDs.find(nCellType);
	if(it != mm_CellTypeToFigureIDs.end())
		return it->second;
	return Ids;
}

void SRSteelInfo::AddTextFigureID(int nCellType, int nFigureID)
{
	auto it = mm_CellTypeToFigureIDs.find(nCellType);
	if(it != mm_CellTypeToFigureIDs.end())	
	{
		it->second.push_back(nFigureID);
		return;
	}
	vector<int> Ids;
	Ids.push_back(nFigureID);
	mm_CellTypeToFigureIDs.insert(make_pair(nCellType, Ids));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// SRSlabInfo.cpp////////////////////////////////////////////////////////////////

SRSlabInfo::SRSlabInfo()
{
	Initialize();
}
SRSlabInfo::~SRSlabInfo()
{

}
void SRSlabInfo::Initialize()
{
	m_Name = _T("");
	isFlatSlabType = false;
	m_nThick = 0;
	m_X1 =  m_X2 =  m_X3 =  m_X4 =  m_X5 =  m_X6 = m_X7 = m_X8 = m_X9 = m_X10 = _T("");
	m_Y1 =  m_Y2 =  m_Y3 =  m_Y4 =  m_Y5 =  m_Y6 = m_Y7 = m_Y8 = m_Y9 = m_Y10 = _T("");
	m_CellTypeLocMap.clear();
}

bool SRSlabInfo::GetIsFlatSlabType()
{
	return isFlatSlabType;
}
void SRSlabInfo::SetIsFlatSlabType(bool isFlat)
{
	isFlatSlabType = isFlat;
}
vector<int> SRSlabInfo::GetTextFigureIDs(int nCellType)
{
	auto it = mm_CellTypeToFigureIDs.find(nCellType);
	if(it != mm_CellTypeToFigureIDs.end())
		return it->second;
	vector<int> Ids;
	return Ids;
}

void SRSlabInfo::AddTextFigureID(int nCellType, int nFigureID)
{
	auto it = mm_CellTypeToFigureIDs.find(nCellType);
	if(it != mm_CellTypeToFigureIDs.end())	
	{
		it->second.push_back(nFigureID);
		return;
	}
	vector<int> Ids;
	Ids.push_back(nFigureID);
	mm_CellTypeToFigureIDs.insert(make_pair(nCellType, Ids));
}

void SRSlabInfo::RemoveCellType(int nCellType)
{
	auto it = mm_CellTypeToFigureIDs.find(nCellType);
	if(it != mm_CellTypeToFigureIDs.end())	
		mm_CellTypeToFigureIDs.erase(it);
}
void SRSlabInfo::AnalyzeRebars(MTReader* pReader)
{
	AnalyzeRebars(pReader, MTReader::ctSlabLx1, MTReader::ctSlabLy1);
	AnalyzeRebars(pReader, MTReader::ctSlabLx2, MTReader::ctSlabLy2);
	AnalyzeRebars(pReader, MTReader::ctSlabLx3, MTReader::ctSlabLy3);
	AnalyzeRebars(pReader, MTReader::ctSlabLx4, MTReader::ctSlabLy4);
	AnalyzeRebars(pReader, MTReader::ctSlabLx5, MTReader::ctSlabLy5);
	AnalyzeRebars(pReader, MTReader::ctSlabLx6, MTReader::ctSlabLy6);
	AnalyzeRebars(pReader, MTReader::ctSlabLx7, MTReader::ctSlabLy7);
	AnalyzeRebars(pReader, MTReader::ctSlabLx8, MTReader::ctSlabLy8);
	AnalyzeRebars(pReader, MTReader::ctSlabLx9, MTReader::ctSlabLy9);
	AnalyzeRebars(pReader, MTReader::ctSlabLx10, MTReader::ctSlabLy10);

	AnalyzeRebars(pReader, MTReader::ctFlatLx1, MTReader::ctFlatLy1);
	AnalyzeRebars(pReader, MTReader::ctFlatLx2, MTReader::ctFlatLy2);
}

void SRSlabInfo::AnalyzeRebars(MTReader* pReader, int cellTypeLx, int cellTypeLy)
{
	auto lxTextList = GetTextList(cellTypeLx);
	auto lyTextList = GetTextList(cellTypeLy);

	if (lxTextList.size() > 0 && lyTextList.size() > 0)
	{	// 둘다 값이 있는 경우
		auto resultLx = AnalyzeRebar(lxTextList);
		SetSlabRebar(cellTypeLx, resultLx);
		auto resultLy = AnalyzeRebar(lyTextList);
		SetSlabRebar(cellTypeLy, resultLy);
	}
	else if (lxTextList.size() > 0 || lyTextList.size() > 0)
	{	// 둘중에 하나만 값이 있는 경우

        bool isOverlapCell = IsOverlapCellTypeLocation(pReader, cellTypeLx, cellTypeLy);

		CString resultLx = _T("");
		CString resultLy = _T("");
		if (lxTextList.size() > 0)
		{
			AnalyzeRebar(lxTextList, resultLx, resultLy);
            SetSlabRebar(cellTypeLx, resultLx);
            if (isOverlapCell)
            {
                if(resultLy == _T(""))
                    SetSlabRebar(cellTypeLy, resultLx);
                else
                    SetSlabRebar(cellTypeLy, resultLy);
            }
        }
		else if (lyTextList.size() > 0)
		{
			AnalyzeRebar(lyTextList, resultLx, resultLy);
            if(resultLy == _T(""))
                SetSlabRebar(cellTypeLy, resultLx);
            else
                SetSlabRebar(cellTypeLy, resultLy);

            if(isOverlapCell)
                SetSlabRebar(cellTypeLx, resultLx);
        }
	}
}
bool SRSlabInfo::IsOverlapCellTypeLocation(MTReader* pReader, int cellTypeLx, int cellTypeLy)
{
    auto myText = pReader->GetCellTypeGMText(cellTypeLx);
    auto urText = pReader->GetCellTypeGMText(cellTypeLy);
    if (myText == nullptr || urText == nullptr)  return false;

    auto myMin = myText->m_Origin.m_X;
    auto myMax = myText->m_Corner.m_X;
    auto urMin = urText->m_Origin.m_X;
    auto urMax = urText->m_Corner.m_X;

    if (urMin <= myMin && myMin <= urMax)
        return true;
    if (urMin <= myMax && myMax <= urMax)
        return true;
    if (myMin <= urMin && urMin <= myMax)
        return true;
    if (myMin <= urMax && urMax <= myMax)
        return true;
    return false;
}
vector<GMText*> SRSlabInfo::GetTextList(int nCellType)
{
	vector<GMText*> textList;
	auto it = m_CellTypeLocMap.find(nCellType);
	if (it != m_CellTypeLocMap.end())	return it->second;
	return textList;
}
void SRSlabInfo::AnalyzeRebar(vector<GMText*> textList, CString& result1, CString& result2)
{
	AlignTextList(textList);
	CString resultD1 = _T("");
	CString resultAt1 = _T("");
	CString resultD2 = _T("");
	CString resultAt2 = _T("");

	for (int i = 0; i < textList.size(); i++)
	{
		auto strData = textList[i]->m_strText;
		int indexD = strData.Find(_T("D"));
		int indexAt = strData.Find(_T("@"));
		if (indexD != -1 && indexAt != -1)
		{
			if (result1 == _T(""))
				result1 = strData;
			else
				result2 = strData;
		}
		else if (indexD != -1)
		{
			if (result1 == _T(""))
				resultD1 = strData;
			else
				resultD2 = strData;
		}
		else if (indexAt != -1)
		{
			if (result1 == _T(""))
				resultAt1 = strData;
			else
				resultAt2 = strData;
		}
		if (resultD1 != _T("") && resultAt1 != _T(""))
			result1 = resultD1 + resultAt1;
		if (resultD2 != _T("") && resultAt2 != _T(""))
			result2 = resultD2 + resultAt2;

		if (result1 != _T("") && result2 != _T(""))
			break;
	}
}

CString SRSlabInfo::AnalyzeRebar(vector<GMText*> textList)
{
	AlignTextList(textList);
	CString resultD = _T("");
	CString resultAt = _T("");
	for (int i = 0; i < textList.size(); i++)
	{
		auto strData = textList[i]->m_strText;
		int indexD = strData.Find(_T("D"));
		int indexAt = strData.Find(_T("@"));
		if (indexD != -1 && indexAt != -1)
		{
			return strData;
		}
		if (indexD != -1)	resultD = strData;
		if (indexAt != -1)	resultAt = strData;
		if (resultD != _T("") && resultAt != _T(""))
			break;
	}
	return resultD + resultAt;
}
void SRSlabInfo::SetSlabRebar(int cellType, CString strData)
{
	switch (cellType)
	{
	case MTReader::ctSlabLx1:	m_X1 = strData; break;
	case MTReader::ctSlabLx2:	m_X2 = strData; break;
	case MTReader::ctSlabLx3:	m_X3 = strData; break;
	case MTReader::ctSlabLx4:	m_X4 = strData; break;
	case MTReader::ctSlabLx5:	m_X5 = strData; break;
	case MTReader::ctSlabLx6:	m_X6 = strData; break;
	case MTReader::ctSlabLx7:	m_X7 = strData; break;
	case MTReader::ctSlabLx8:	m_X8 = strData; break;
	case MTReader::ctSlabLx9:	m_X9 = strData; break;
	case MTReader::ctSlabLx10:	m_X10 = strData; break;

	case MTReader::ctSlabLy1:	m_Y1 = strData; break;
	case MTReader::ctSlabLy2:	m_Y2 = strData; break;
	case MTReader::ctSlabLy3:	m_Y3 = strData; break;
	case MTReader::ctSlabLy4:	m_Y4 = strData; break;
	case MTReader::ctSlabLy5:	m_Y5 = strData; break;
	case MTReader::ctSlabLy6:	m_Y6 = strData; break;
	case MTReader::ctSlabLy7:	m_Y7 = strData; break;
	case MTReader::ctSlabLy8:	m_Y8 = strData; break;
	case MTReader::ctSlabLy9:	m_Y9 = strData; break;
	case MTReader::ctSlabLy10:	m_Y10 = strData; break;

	case MTReader::ctFlatLx1: m_X1 = strData; break;
	case MTReader::ctFlatLx2: m_X4 = strData; break;
	case MTReader::ctFlatLy1: m_Y1 = strData; break;
	case MTReader::ctFlatLy2: m_Y4 = strData; break;
	}
}
void SRSlabInfo::AlignTextList(vector<GMText*>& textList)
{
	int numText = textList.size();
	for (int iText = 0; iText < numText; iText++)
	{
		for (int jText = iText + 1; jText < numText; jText++)
		{
			GMText* pIText = textList[iText];
			GMText* pJText = textList[jText];
			if (pIText->m_Vec.m_Y < pJText->m_Vec.m_Y)
			{
				textList[iText] = pJText;
				textList[jText] = pIText;
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// SRSectionBox.cpp ////////////////////////////////////////////////////////

SRSectionBox::SRSectionBox(SRSectionTable* pOwner)
{
	mp_Owner = pOwner;
	mp_Reader = pOwner->mp_Reader;
	mp_Rect = NULL;
	mp_Name = NULL;
	mp_Story = NULL;
	mp_Size = NULL;
	m_VdId = 0;
	m_LinkTol = 1.;
	mp_Section = NULL;
	m_dFactor = 1.0;
	m_Shape = 0;

	m_nFromStory = m_nToStory = 0;
	m_Width = m_Depth = m_Thick = 0;
	m_DimensionWidth = m_DimensionDepth = 0;
	m_nMaterialID = 0;
	mp_MainBarSet1 = mp_MainBarSet2 = NULL;
	mp_MainBar1 = mp_MainBar2 = NULL;
	mp_SideBar = NULL;
	mp_StrpBar1 = NULL;
	mp_StrpBar2 = NULL;
	m_SideStirrupBar = NULL;
	mp_SubMainBar = NULL;
	m_nStrp = 0;
	m_nVBar = 0;

	m_nErrorCode = 0;
	m_nWarningCode = 0;
	m_nBeamSection = 1;

	mp_SlabInfo = NULL;
	m_BoxNumber = 1;

	m_nSpliteDistTol = 50;
	m_EdgeToStrpDist = 25;

	m_SteelSectionName = _T("");
	m_StudBolt = _T("");
	mp_SectCircle = NULL;
	mp_StrpCircle = NULL;

	_sectionBoxID = 0;
	m_IsMainBarShapeByLayer = false;
	m_IsTieBarShapeByLayer = false;
	m_IsSubMainBarShapeByLayer = false;
	m_hasDimensionSize = false;

	m_sWidth = m_sDepth = m_sThick = _T("");
	m_sTopBar = m_sBotBar = m_sSideBar = _T("");
	m_sStirrupBar1 = m_sStirrupBar2 = _T("");
	m_sSubMainBar = _T("0-HD10");

	ma_StirrupBar1.clear();
	ma_StirrupBar2.clear();
}

SRSectionBox::~SRSectionBox(void)
{
	DeleteObjectArr();
}

void SRSectionBox::DeleteSubMainBars()
{
	if(mp_Reader->m_Group == GROUP_BEAM)
	{
		ma_SubMainBar.RemoveAll();
		return;
	}

	int nData = ma_SubMainBar.GetSize();
	for(int iData = 0; iData < nData; iData++)
		delete ma_SubMainBar[iData];
	ma_SubMainBar.RemoveAll();
}
void SRSectionBox::DeleteObjectArr()
{
	if(mp_Rect)		delete mp_Rect;
	if (mp_MainBarSet1)	delete mp_MainBarSet1;
	if (mp_MainBarSet2)	delete mp_MainBarSet2;
	if(mp_MainBar1)	delete mp_MainBar1;
	if(mp_MainBar2)	delete mp_MainBar2;
	if(mp_SideBar)	delete mp_SideBar;
	if(mp_StrpBar1)	delete mp_StrpBar1;
	if(mp_StrpBar2)	delete mp_StrpBar2;
	if(m_SideStirrupBar)	delete m_SideStirrupBar;
	if(mp_SlabInfo)	delete mp_SlabInfo;
	if(mp_SubMainBar)	delete mp_SubMainBar;
	DeleteWallInfoArr();
	DeleteSubMainBars();
	DeleteSteelInfoArr();
	DeleteSpecificTieBar();
}
void SRSectionBox::DeleteWallInfoArr()
{
	long nInfo = ma_WallInfo.GetSize();
	for(long iInfo=0; iInfo < nInfo; iInfo++)
		delete ma_WallInfo[iInfo];
	ma_WallInfo.RemoveAll();
}

void SRSectionBox::DeleteSteelInfoArr()
{
	long nInfo = ma_SteelInfo.GetSize();
	for(long iInfo=0; iInfo < nInfo; iInfo++)
		delete ma_SteelInfo[iInfo];
	ma_SteelInfo.RemoveAll();
}

void SRSectionBox::DeleteSpecificTieBar()
{
	for (int i = 0; i < ma_SpecificTieBar.size(); i++)
		delete ma_SpecificTieBar[i];
	ma_SpecificTieBar.clear();
}

GM2DRectangle* SRSectionBox::GetOwnerRect()
{
	if(mp_Owner)
		return mp_Owner->mp_Rect;
	return NULL;
}

bool SRSectionBox::Contains(GM2DVector* pVec1, GM2DVector* pVec2)
{
	if(mp_Rect==NULL)	return false;
	GM2DVector* TestVec;
	if(pVec1)
	{
		TestVec = pVec1;
		BOOL RtnVal = mp_Rect->Contains(TestVec, m_nSpliteDistTol);
		if(RtnVal == FALSE)	return false;
	}
	if(pVec2)
	{
		TestVec = pVec2;
		BOOL RtnVal = mp_Rect->Contains(TestVec, m_nSpliteDistTol);
		if(RtnVal == FALSE)	return false;
	}
	return true;
}
GMCircle* SRSectionBox::FindGMCircle(GM2DVector CenPnt, double dRadius)
{
	double dTol = DTOL1;
	long NumCircle = ma_Circle.GetSize();
	for(long iCircle=0; iCircle<NumCircle; iCircle++)
	{
		GMCircle* TheCir = ma_Circle[iCircle];
		dTol = min(TheCir->m_A, dRadius) * 2.5;
		if (TheCir->Contains(CenPnt) <= 0 && fabs(TheCir->m_A - dRadius) < dTol) // 원주상 or 원안
			return TheCir;
// 		if(fabs(TheCir->m_Center.m_X-CenPnt.m_X) < dTol && fabs(TheCir->m_Center.m_Y-CenPnt.m_Y) < dTol)
// 		{
// 			return TheCir;
// 		}
	}
	return NULL;
}

void SRSectionBox::MakeData()
{
	long nGroup = mp_Reader->m_Group;
	long nType = mp_Reader->m_Type;

	MakeCirclebyArc();
	if(nGroup==GROUP_COLUMN)
		MakeData_Column();
	else if(nGroup==GROUP_WALL)
		MakeData_Wall();
	else if(nGroup==GROUP_BEAM)
		MakeData_Beam();
	else if(nGroup == GROUP_SLAB)
		MakeData_Slab();
	else if(nGroup == GROUP_BUTTRESS)
		MakeData_Buttress();
	else if(nGroup == GROUP_RWALL)
		MakeData_RWall();
}
void SRSectionBox::MakeCirclebyArc()
{
	double dTol = 25;
	int nCircle = ma_Circle.GetSize();
	if (nCircle > 0)
	{
		GMCircle* pCircle = ma_Circle[0];
		dTol = ma_Circle[0]->m_A;
	}

	for(long iCurve=0; iCurve < ma_Curve.GetSize(); iCurve++)
	{
		GM2DGeometry* pIObj = ma_Curve[iCurve];
		if(pIObj==NULL)	continue;
		if(pIObj->m_ObjType != GT_ARC)	continue;
		GMArc* pIArc = (GMArc*)pIObj;
		for(long jCurve=iCurve+1; jCurve < ma_Curve.GetSize(); jCurve++)
		{
			GM2DGeometry* pJObj = ma_Curve[jCurve];
			if(pJObj==NULL)	continue;
			if(pJObj->m_ObjType != GT_ARC)	continue;
			GMArc* pJArc = (GMArc*)pJObj;
			if(pIArc->m_Center.SameVector4DTOL(pJArc->m_Center, DTOL1))
			{
				if(fabs(pIArc->m_A - pJArc->m_A) < DTOL1)
				{
					double arcLength = pIArc->GetArcLength()+pJArc->GetArcLength();
					double circleLength = 2*PI*pIArc->m_A;
					if(fabs(arcLength - circleLength) > DTOL1)	continue;

					GM2DVector aVec(pIArc->m_Center.m_X, pIArc->m_Center.m_Y);
					GMCircle* pCircle = FindGMCircle(aVec, pIArc->m_A);
					if(pCircle==NULL)
					{
						pCircle = new GMCircle(pIArc->m_A, pIArc->m_Center.m_X, pIArc->m_Center.m_Y);
						pCircle->m_Tag = 1;
						ma_Circle.Add(pCircle);
					}
					ma_Curve.RemoveAt(jCurve);
					ma_Curve.RemoveAt(iCurve);
					iCurve--;
					break;
				}
			}
		}
	}
}

bool SRSectionBox::MakeData_Column()
{
	MSMaterial::Type_MSMaterial materialType = mp_Reader->m_MaterialType;
	if(materialType == MSMaterial::msConcrete)
		return MakeData_RcColumn();
	else if(materialType == MSMaterial::msSteel)
		return MakeData_SteelColumn();
	else
		return MakeData_SrcColumn();
}

bool SRSectionBox::MakeData_SteelColumn()
{
	MakeSteelDataInfo_fromName();

	CStringArray strNames;
	int nSteelInfo = ma_SteelInfo.GetSize();
	for(int iInfo = 0; iInfo < nSteelInfo; iInfo++)
	{
		SRSteelInfo* pInfo = ma_SteelInfo[iInfo];
		long nNames = mp_Owner->GetName(pInfo->m_Name, strNames);
		for(int iName = 0; iName < nNames; iName++)
		{
			pInfo->ma_Name.push_back(strNames[iName]);
		}
		if(pInfo->m_nMaterialID == 0)
			pInfo->m_nMaterialID = mp_Reader->m_nMaterialID;
	}

	return true;
}

bool SRSectionBox::MakeData_SrcColumn()
{
	m_nErrorCode = 0;
	m_nWarningCode = 0;

	AnalyzeTexts_Column();
	CurveGrouping();

	if(FindDimensions() == FALSE)
	{
		m_nErrorCode = 1;
		return false;
	}
	m_Shape = FindEdges_ColumnH();
	if(m_Shape==0)								return false;
	AnalyzeCircles();
	FindMainBars_ColumnH(1);  // 콩이 제일 많은 주근
	FindMainBars_ColumnH(2);  // 콩이 두번째 많은 주근
	FindMainBars_ColumnH(3);  // X 표시된 보조근
	FindStrpBars_ColumnH();
	FindTieBars_ColumnH();

	CalcVBarLocOfEdges();
	CalcVBarVecOfEdges();
	FindSpecificTieBars_Column();
	FindSupportMainBar();
	AnalyzeSectionText();
	return true;
}

bool SRSectionBox::MakeData_RcColumn()
{
	m_nErrorCode = 0;
	m_nWarningCode = 0;

	AnalyzeTexts_Column();
	CurveGrouping();

	if(FindDimensions() == FALSE)
	{
		m_nErrorCode = 1;
		return false;
	}
	m_Shape = FindEdges_ColumnH();
	if(m_Shape==0)								return false;
	AnalyzeCircles();
	FindMainBars_ColumnH(1);  // 콩이 제일 많은 주근
	FindMainBars_ColumnH(2);  // 콩이 두번째 많은 주근
	FindMainBars_ColumnH(3);  // X 표시된 보조근
	FindStrpBars_ColumnH();
	FindTieBars_ColumnH();

	CalcVBarLocOfEdges();
	CalcVBarVecOfEdges();
	FindSpecificTieBars_Column();
	FindSupportMainBar();
	AnalyzeSectionText();

	return true;
}

static bool IsRebarByCount(CString strBar, long& nBar, CString& strBarType, CString& strDiaName) 
{
	strBar.Trim();
	int Idx = strBar.Find('-');
	if(Idx==-1)
		return false;
	CString strToken = strBar.Left(Idx);
	nBar = _ttol(strToken);
	if(nBar==0)
		return false;
	strBar = strBar.Mid(Idx+1);
	Idx = strBar.Find('D');
	if(Idx==-1)
		return false;
	strToken = strBar.Left(Idx);
	strToken.Trim();
	if(strToken=="")
		strBarType = "D";
	else if(strToken=="H")
		strBarType = "HD";
	else
		return false;

	strToken = strBar.Mid(Idx+1);
	strDiaName = strToken;

	return true;
}

static bool IsRebarBySpace(CString strBar, CString& strBarType, CString& sDiaName, double& Space) 
{
	strBar.Trim();
	int Idx = strBar.Find('D');
	if(Idx==0)
		strBarType = "D";
	else if(Idx==1)
	{
		if(strBar[0]=='H')
			strBarType = "HD";
		else
			return false;
	}
	else
		return false;

	strBar = strBar.Mid(Idx+1);
	Idx = strBar.Find('@');
	CString strToken = strBar.Left(Idx);
	strToken.Trim();
	sDiaName = strToken;
	strToken = strBar.Mid(Idx+1);
	Space = _ttol(strToken);
	if(Space==-1 || Space==0)
		return false;

	return true;
}

static long GetNumVBarOfEdge(MREdge* pEdge)
{
	long nVBar=0;
	long nLayer = pEdge->ma_VBarSet.GetSize();
	for(long iLayer=0; iLayer<nLayer; iLayer++)
		nVBar += pEdge->ma_VBarSet[iLayer]->ma_VBarLoc.GetSize();
	return nVBar;
}

bool SRSectionBox::SetEdgeVBarProperty(long EdgeNum, long NumBarByText, CString BarType, CString sDiaName)
{
	MREdge* pEdge = mp_Section->ma_Edge[EdgeNum];
	long nLayer = pEdge->ma_VBarSet.GetSize();
	double LocLimit = pEdge->ma_VBarSet[nLayer-1]->m_LayerDistFromEdge;
	long NumBarOnEdge = pEdge->GetNumVBarOfEdge(); 
	if(NumBarByText!=NumBarOnEdge)
		return false;

	pEdge->SetVBarByCount(sDiaName, BarType);

	// 	if(EdgeNum==0 && pText->m_Vec.m_Y < LocLimit)
	// 		pEdge->SetVBarByCount(BarDia, BarType);
	// 	else if(EdgeNum==2 && pText->m_Vec.m_Y > LocLimit)
	// 		pEdge->SetVBarByCount(BarDia, BarType);
	// 	else if(EdgeNum==1 && pText->m_Vec.m_X > LocLimit)
	// 		pEdge->SetVBarByCount(BarDia, BarType);
	// 	else if(EdgeNum==3 && pText->m_Vec.m_X < LocLimit)
	// 		pEdge->SetVBarByCount(BarDia, BarType);

	return true;
}

// 현재 Edge line : X 오른쪽 증가, Y : 위로 증가인 상태이다.
// MREdge의 자료 구조를 : X는 오른쪽 증가, Y : 아래로 감소로 하기로 했기에, locValue값을 다시 설정한다.
void SRSectionBox::CalcVBarLocOfEdges()
{
	int nEdge = mp_Section->ma_Edge.GetSize();
	if(nEdge != 4)	return;

	for(int iEdge = 0; iEdge < nEdge; iEdge++)
	{
		MREdge* pEdge = mp_Section->ma_Edge[iEdge];
		GM2DLineVector EdgeLineVec(pEdge->m_P0, pEdge->m_P1);
		if(IsCorrectiveDirection(&EdgeLineVec) == false)
		{
			pEdge->ReverseLocValues();
			pEdge->Reverse();
		}
	}
}

bool SRSectionBox::IsCorrectiveDirection(GM2DLineVector* lineVec)
{
	double dx = fabs(lineVec->m_P1.m_X-lineVec->m_P0.m_X	);
	double dy = fabs(lineVec->m_P1.m_Y-lineVec->m_P0.m_Y);

	if(dx > dy)
	{
			// 왼쪽 --> 오른쪽
		if(lineVec->m_P0.m_X	 < lineVec->m_P1.m_X)
			return true;
	}
	else
	{
		// 위 --> 아래
		if(lineVec->m_P0.m_Y	 > lineVec->m_P1.m_Y)
			return true;
	}
	return false;
}

void SRSectionBox::CalcVBarVecOfEdges()
{
	long nEdge = mp_Section->ma_Edge.GetSize();
	for(long iEdge=0; iEdge<nEdge; iEdge++)
		mp_Section->ma_Edge[iEdge]->CalcVBarVectors();
}

bool SRSectionBox::AnalyzeSectionText()
{
	// 	if(m_Shape!=MSLineMemberM::RECT)
	// 		;

	long NumBarByText;
	double Space;
	CString BarType;
	CTypedPtrArray<CObArray, GMText*> CountBarArr; 
	CTypedPtrArray<CObArray, GMText*> SpaceBarArr;

	long nText = ma_GMText.GetSize();
	for(long iText=0; iText<nText; iText++)
	{
		GMText* pText = ma_GMText[iText];
		long nCellType = mp_Reader->GetCellType(pText);
		if(nCellType!=0 && nCellType!=MTReader::ctSection)	
			continue;

		CString sDiaName;
		if(IsRebarByCount(pText->m_strText, NumBarByText, BarType, sDiaName))
		{
			long nBar = CountBarArr.GetSize();
			for(long iBar=0; iBar<nBar; iBar++)
			{
				if(CountBarArr[iBar]->m_Vec.m_Y > pText->m_Vec.m_Y)
				{
					CountBarArr.InsertAt(iBar, pText);
					break;
				}
			}
			if(iBar==nBar)
				CountBarArr.Add(pText);
		}
		else if(IsRebarBySpace(pText->m_strText, BarType, sDiaName, Space))
		{
			long nBar = SpaceBarArr.GetSize();
			for(long iBar=0; iBar<nBar; iBar++)
			{
				if(SpaceBarArr[iBar]->m_Vec.m_Y > pText->m_Vec.m_Y)
				{
					SpaceBarArr.InsertAt(iBar, pText);
					break;
				}
			}
			if(iBar==nBar)
				SpaceBarArr.Add(pText);
		}
	}

	long nBarText = CountBarArr.GetSize();
	if(mp_MainBar1==NULL)
		return false;

	CString GroupName = _T("");
	if(mp_Reader->m_Group == GROUP_COLUMN)
		GroupName = _T("Column");
	else if(mp_Reader->m_Group == GROUP_WALL)
		GroupName = _T("Wall");
	else if(mp_Reader->m_Group == GROUP_BEAM)
		GroupName = _T("Beam");
	else if(mp_Reader->m_Group == GROUP_SLAB)
		GroupName = _T("Slab");
	else if(mp_Reader->m_Group == GROUP_BUTTRESS)
		GroupName = _T("Buttress");
	else
		return false;
	int Level = -1;

	long nVBar = mp_MainBar1->GetTotalBars();
	if(mp_MainBar2)
	  nVBar += mp_MainBar2->GetTotalBars();
	double scaleFactor = GetScaleFactorFromDimension();

 	if(m_Shape==MSRcLineMemberM::RECT)
	{
		mp_Section->ma_Edge[1]->RemoveCornerVBar(true, true, scaleFactor);
		mp_Section->ma_Edge[3]->RemoveCornerVBar(true, true, scaleFactor);

		CString sDiaName = mp_MainBar1->GetBarDiameterName(0);
		BarType = mp_MainBar1->GetBarType();

		long nBotBar = mp_Section->ma_Edge[0]->GetNumVBarOfEdge();
		long nRightBar = mp_Section->ma_Edge[1]->GetNumVBarOfEdge();
		long nTopBar = mp_Section->ma_Edge[2]->GetNumVBarOfEdge();
		long nLeftBar = mp_Section->ma_Edge[3]->GetNumVBarOfEdge();

		if(nTopBar+nBotBar == nVBar)
		{
			CString SideBarType;
			CString SideBarDia;
			mp_Section->ma_Edge[2]->SetVBarByCount(sDiaName, BarType);
			mp_Section->ma_Edge[0]->SetVBarByCount(sDiaName, BarType);

			if(nLeftBar==0 && nRightBar==0)
				return true;

			if(nBarText==1)
				IsRebarByCount(CountBarArr[0]->m_strText, NumBarByText, SideBarType, SideBarDia);
			else if(nBarText==3)
				IsRebarByCount(CountBarArr[1]->m_strText, NumBarByText, SideBarType, SideBarDia);
			else
			{
				mp_Section->ma_Edge[1]->SetVBarByCount(sDiaName, BarType);
				mp_Section->ma_Edge[3]->SetVBarByCount(sDiaName, BarType);
				ASSERT(0); // Warning : 양측 사이드철근 유형과 지름을 주근과 동일하게 가정함
				return true;
			}

			mp_Section->ma_Edge[1]->SetVBarByCount(SideBarDia, SideBarType);
			mp_Section->ma_Edge[3]->SetVBarByCount(SideBarDia, SideBarType);

			if(!(nLeftBar+nRightBar==NumBarByText || (nLeftBar==nRightBar && nLeftBar==NumBarByText)))
				ASSERT(0); // Warning : 양측 사이드철근의 갯수가 일치하지 않음

			return true;
		}	
		else if(nTopBar+nBotBar+nRightBar+nLeftBar == nVBar)
		{
			mp_Section->ma_Edge[2]->SetVBarByCount(sDiaName, BarType);
			mp_Section->ma_Edge[0]->SetVBarByCount(sDiaName, BarType);
			mp_Section->ma_Edge[1]->SetVBarByCount(sDiaName, BarType);
			mp_Section->ma_Edge[3]->SetVBarByCount(sDiaName, BarType);
			return true;
		}

		if(nBarText<1)
		{
			mp_Section->ma_Edge[2]->SetVBarByCount(sDiaName, BarType);
			mp_Section->ma_Edge[0]->SetVBarByCount(sDiaName, BarType);
			mp_Section->ma_Edge[1]->SetVBarByCount(sDiaName, BarType);
			mp_Section->ma_Edge[3]->SetVBarByCount(sDiaName, BarType);
			{
				CString errorMessage;
				if(mp_Name)
					errorMessage.Format(_T("%s"), mp_Name);
				else
				{
					if(mp_Owner->mp_Name)
						errorMessage.Format(_T("%s"), mp_Owner->mp_Name->m_strText);
					else
						errorMessage = _T("");
				}
// 				mp_Reader->CreateErrorObject(GroupName, Level, ERROR_MAINBAR_NUM, errorMessage);
// 				mp_Owner->SetErrorCode(1);
				return false;
			}
			return true;
		}
		else if(nBarText==2)
		{
			IsRebarByCount(CountBarArr[0]->m_strText, NumBarByText, BarType, sDiaName);
			mp_Section->ma_Edge[0]->SetVBarByCount(NumBarByText, sDiaName, BarType);
			IsRebarByCount(CountBarArr[1]->m_strText, NumBarByText, BarType, sDiaName);
			mp_Section->ma_Edge[2]->SetVBarByCount(NumBarByText, sDiaName, BarType);
		}
		else if(nBarText==3)
		{
			IsRebarByCount(CountBarArr[0]->m_strText, NumBarByText, BarType, sDiaName);
			mp_Section->ma_Edge[0]->SetVBarByCount(NumBarByText, sDiaName, BarType);
			if(nBotBar!=NumBarByText)
				ASSERT(0); // Warning : 하단 주근의 갯수가 일치하지 않습니다.
			IsRebarByCount(CountBarArr[2]->m_strText, NumBarByText, BarType, sDiaName);
 			mp_Section->ma_Edge[2]->SetVBarByCount(NumBarByText, sDiaName, BarType);
			if(nTopBar!=NumBarByText)
				ASSERT(0); // Warning : 상단 주근의 갯수가 일치하지 않습니다.
			IsRebarByCount(CountBarArr[1]->m_strText, NumBarByText, BarType, sDiaName);
			if(nLeftBar+nRightBar!=NumBarByText || NumBarByText%2!=0)
				ASSERT(0); // Warning : 좌우측 주근의 갯수 설정이 불확실합니다.
			if(NumBarByText%2==0)
			{
				mp_Section->ma_Edge[1]->SetVBarByCount(NumBarByText/2, sDiaName, BarType);
				mp_Section->ma_Edge[3]->SetVBarByCount(NumBarByText/2, sDiaName, BarType);
			}
			else
			{
				mp_Section->ma_Edge[1]->SetVBarByCount(sDiaName, BarType);
				mp_Section->ma_Edge[3]->SetVBarByCount(sDiaName, BarType);
			}
		}
	}
	else if(m_Shape==MSRcLineMemberM::POLYGON)
	{
		long nTotalBar=0;
		long nEdge = mp_Section->ma_Edge.GetSize();
		for(long iEdge=0; iEdge<nEdge; iEdge++)
		{
			mp_Section->ma_Edge[iEdge]->RemoveCornerVBar(false, true, scaleFactor);
			nTotalBar += mp_Section->ma_Edge[iEdge]->GetNumVBarOfEdge();
		}	
		if(nTotalBar == nVBar)
		{
			CString sDiaName = mp_MainBar1->GetBarDiameterName(0);
			BarType = mp_MainBar1->GetBarType();
			for(long iEdge=0; iEdge<nEdge; iEdge++)
				mp_Section->ma_Edge[iEdge]->SetVBarByCount(sDiaName, BarType);
			return true;
		}
	}
	return false;
}

void SRSectionBox::MakeData_Wall()
{
	m_nErrorCode = 0;
	m_nWarningCode = 0;

	CurveGrouping_4Wall();
	MakeWallDataInfo_fromFloor();

	SetWallDataByColumns();
	MergeSameWallInfo();
	//	AddWallInfoTextFigureIDs();
}

void SRSectionBox::MergeSameWallInfo()
{
	for (int iInfo = 0; iInfo < ma_WallInfo.GetSize(); iInfo++)
	{
		SRWallInfo* pIInfo = ma_WallInfo[iInfo];
		if (pIInfo->GetIsMerged())	continue;
		for (int jInfo = iInfo + 1; jInfo < ma_WallInfo.GetSize(); jInfo++)
		{
			SRWallInfo* pJInfo = ma_WallInfo[jInfo];
			if (pJInfo->GetIsMerged())	continue;
			if (ma_WallInfo[iInfo] == ma_WallInfo[jInfo])	continue;
			if (pIInfo->IsSameData(pJInfo))
			{
                auto myToStory = ma_WallInfo[iInfo]->GetToStory();
                auto urToStory = ma_WallInfo[jInfo]->GetToStory();
                if(urToStory != 0 && myToStory < urToStory)
				    ma_WallInfo[iInfo]->SetToStory(ma_WallInfo[jInfo]->GetToStory());
				ma_WallInfo[jInfo]->SetIsMerged(true);
			}
			else
				break;
		}
	}
}
void SRSectionBox::AddWallInfoTextFigureIDs()
{
	long nText = ma_GMText.GetSize();
	for(long iText=0; iText<nText; iText++)
	{
		GMText* pText = ma_GMText[iText];
		int nCellType = mp_Reader->GetCellType(pText);
		if(nCellType <= 2)	continue;
		SRWallInfo* pWallInfo = FindWallInfo(pText->m_Vec.m_Y);
		if(pWallInfo == NULL)	continue;

		pWallInfo->AddTextFigureID(nCellType, FindTextFigureID(pText));
	}
}

void SRSectionBox::CurveGrouping_4Wall()
{
	mp_Rect->GetEdges(ma_BoxLVec);

	GM2DLineVector* pBotLVec = ma_BoxLVec[0];
	GM2DLineVector* pRightLVec = ma_BoxLVec[1];

	CTypedPtrArray<CObArray, GM2DLineVector*> MergeLineArr;
	long nCurve = ma_Curve.GetSize();
	for(long iLine=0; iLine<nCurve; iLine++)
	{
		GM2DGeometry* pCurve = ma_Curve[iLine];
		if(pCurve->m_ObjType != GT_LINE)	continue;

		GM2DLineVector* pLine = (GM2DLineVector*)pCurve;
		if(pLine->IsHorizontalLine() == FALSE)	continue;

		if (fabs(mp_Rect->m_Origin.m_Y-pLine->m_P0.m_Y)<DTOL1) // 표 경계에 있는 선은 제외
			continue;
		if (fabs(mp_Rect->m_Corner.m_Y-pLine->m_P0.m_Y)<DTOL1) // 표 경계에 있는 선은 제외
			continue;

		long nLine = ma_HoriLVec.GetSize();
		for(long jLine=0; jLine<nLine; jLine++)
		{
			if(pLine->m_P0.m_Y < ma_HoriLVec[jLine]->m_P0.m_Y)
			{
				if(pLine->m_P0.m_X>pLine->m_P1.m_X)
					pLine->ReverseDirection();
				ma_HoriLVec.InsertAt(jLine, pLine);
				break;
			}
		}
		if(jLine==nLine)
		{
			if(pLine->m_P0.m_X>pLine->m_P1.m_X)
				pLine->ReverseDirection();
			ma_HoriLVec.Add(pLine);
		}
	}
}

void SRSectionBox::SetWallDataByColumns()
{
	SetWallDataByColumn(ma_Fck, MTReader::ctMaterial);
	SetWallDataByColumn(ma_Thick, MTReader::ctThick);
	SetWallDataByColumn(ma_HBar, MTReader::ctHBar);
	SetWallDataByColumn(ma_VBar, MTReader::ctVBar);

	SetWallData4ShearRatio();
}

void SRSectionBox::SetWallData4ShearRatio()
{
	double tolerance = mp_Rect->m_Corner.m_Y - mp_Rect->m_Origin.m_Y;
	if (ma_WallInfo.GetSize() > 1)
		tolerance = (ma_WallInfo[1]->GetStartLocation() - ma_WallInfo[0]->GetStartLocation()) *0.5; // 하나만 있을 때는 정리

	for (int i = 0; i < ma_WallInfo.GetSize(); i++)
	{
		SRWallInfo* pCurWall = ma_WallInfo[i];
		double wallYc = pCurWall->GetStartLocation();
		for (int j = 0; j < ma_ShearRatio.size(); j++)
		{
			double textY = ma_ShearRatio[j]->m_Vec.m_Y;
			if (fabs(wallYc - textY) < tolerance)
			{
				pCurWall->SetShearRatio(ma_ShearRatio[j]->m_strText);
				pCurWall->AddTextFigureID(MTReader::ctShearRatio, FindTextFigureID(ma_ShearRatio[j]));
				break;
			}
		}
	}
}
void SRSectionBox::SetWallDataByColumn(vector<GMText*>& TextArr, long cellType)
{
	if (ma_WallInfo.GetSize()==0) return;

	CArray<int, int> indexArr;
	int index = 0;

	CArray<double, double> intersectYArr;

	int txtCount = TextArr.size();
	if(txtCount==0) return;
	double xCoord = TextArr[0]->m_Vec.m_X;
	CalculateYArr(xCoord, intersectYArr);

	int yCount = intersectYArr.GetSize();
//	if (yCount==0) return;

	int curTxtIdx = 0;
	int iLine = 0;
	double tolerance = mp_Rect->m_Corner.m_Y-mp_Rect->m_Origin.m_Y;
	if(ma_WallInfo.GetSize() > 1)
		tolerance = (ma_WallInfo[1]->GetStartLocation() - ma_WallInfo[0]->GetStartLocation()) *0.5; // 하나만 있을 때는 정리

	for(int i=0; i<ma_WallInfo.GetSize();i++)
	{
		SRWallInfo* pCurWall = ma_WallInfo[i];
		double currentY = pCurWall->GetStartLocation();
		double textY = TextArr[curTxtIdx]->m_Vec.m_Y;

		if (textY<currentY-tolerance/2)	// Info보다 위에 있는 Text와 비교하기 위한 보정
		{
			if (curTxtIdx < txtCount-1)
			{
				curTxtIdx++;
				textY = TextArr[curTxtIdx]->m_Vec.m_Y;
			}
			else
			{
				SetWallInfoData(ma_WallInfo[i], cellType, TextArr[curTxtIdx]);
				continue;
			}
		}

		if (textY<currentY+tolerance)
		{
			SetWallInfoData(ma_WallInfo[i], cellType, TextArr[curTxtIdx]); // 사이에 있으면 현재 텍스트 사용
			if (curTxtIdx < txtCount-1)
				curTxtIdx++;
		}
		else
		{
			if (FindLine(currentY, textY, intersectYArr, &iLine))
			{
				if (curTxtIdx > 0)
					SetWallInfoData(ma_WallInfo[i], cellType, TextArr[curTxtIdx-1]);  // 바로 위에 선이 있으면 아래 텍스트 사용
				else
					SetWallInfoData(ma_WallInfo[i], cellType, TextArr[curTxtIdx]);  // 첫번째 텍스트보다 밑에 있는 WallInfo를 첫번째 텍스트정보로 설정
			}
			else
			{
				SetWallInfoData(ma_WallInfo[i], cellType, TextArr[curTxtIdx]); // 선보다 텍스트가 먼저이면 현재 텍스트 사용
			}
		}
	}

}

bool SRSectionBox::FindLine(double infoY, double textY, CArray<double, double>& intersectYArr, int* iLine)
{
	for(int i= (*iLine); i<intersectYArr.GetSize();i++)
	{
		double current = intersectYArr[i];
		if (infoY<current && current<textY)
		{
			(*iLine) = i;
			return true;
		}
	}
	return false;
}

void SRSectionBox::SetWallInfoData(SRWallInfo* pWall, long cellType, GMText* pText)
{
	CString strData = pText->m_strText;
	if (cellType == MTReader::ctMaterial)
	{
		MSMaterial* pMate = mp_Reader->mp_Bldg->GetMaterial(strData);
		if (pMate)	pWall->SetMaterialID(pMate->m_ID);
		else
		{
			CString mateName;	mateName.Format(_T("C%s"), strData);
			pMate = mp_Reader->mp_Bldg->GetMaterial(mateName);
			if (pMate)	pWall->SetMaterialID(pMate->m_ID);
		}
	}
	else if (cellType == MTReader::ctThick)
		pWall->SetThick(_ttof(strData));
	else if (cellType == MTReader::ctHBar)
		pWall->SetHBar(strData);
	else if (cellType == MTReader::ctVBar)
		pWall->SetVBar(strData);
	else if (cellType == MTReader::ctShearRatio)
		pWall->SetShearRatio(strData);

// 	pWall->AddTextFigureID(cellType, FindTextFigureID(pText));
}

void SRSectionBox::CalculateYArr( double xCoord, CArray<double, double>& intersectYArr )
{
	int count = ma_HoriLVec.GetSize();
	for (int i=0; i<count;i++)
	{
		GM2DLineVector* currentVec = ma_HoriLVec[i];
		if (currentVec->m_P0.m_X< xCoord && xCoord < currentVec->m_P1.m_X)
			intersectYArr.Add(currentVec->m_P0.m_Y);
	}
}

void SRSectionBox::MakeWallDataInfo_fromFloor()
{
	long nText = ma_GMText.GetSize();
	for(long iText=0; iText<nText; iText++)
	{
		GMText* pText = ma_GMText[iText];
		long nCellType;
		if(pText == mp_Name || pText == mp_Owner->mp_Name)	continue;
		nCellType = mp_Reader->GetCellType(pText);
		if(nCellType==0)	continue;

		double dLoc = pText->m_Vec.m_Y;
		SRWallInfo* pWallInfo = FindWallInfo(pText->m_Vec.m_Y);
		pWallInfo = MakeWallInfo(pWallInfo, pText, nCellType);

		if (nCellType == MTReader::ctMaterial)
		 	ma_Fck.push_back(pText);
		else if (nCellType == MTReader::ctThick)
		 	ma_Thick.push_back(pText);
		else if (nCellType == MTReader::ctHBar)
		 	ma_HBar.push_back(pText);
		else if (nCellType == MTReader::ctVBar)
		 	ma_VBar.push_back(pText);
		else if (nCellType == MTReader::ctShearRatio)
		 	ma_ShearRatio.push_back(pText);

// 		if (nCellType == MTReader::ctStory)
// 		{
// 			double dLoc = 0.0;
// 			dLoc = pText->m_Vec.m_Y;
// 			SRWallInfo* pWallInfo = FindWallInfo(pText->m_Vec.m_Y);
// 			pWallInfo = MakeWallInfo(pWallInfo, pText, nCellType);
// 		}
// 		else if (nCellType == MTReader::ctMaterial)
// 			ma_Fck.push_back(pText);
// 		else if (nCellType == MTReader::ctThick)
// 			ma_Thick.push_back(pText);
// 		else if (nCellType == MTReader::ctHBar)
// 			ma_HBar.push_back(pText);
// 		else if (nCellType == MTReader::ctVBar)
// 			ma_VBar.push_back(pText);
// 		else if (nCellType == MTReader::ctShearRatio)
// 			ma_ShearRatio.push_back(pText);
	}

	if (ma_WallInfo.GetSize() == 0)
	{
		GMText* pNameText = nullptr;
		if (mp_Name != nullptr)	pNameText = mp_Name;
		else if (mp_Owner->mp_Name != nullptr)	pNameText = mp_Owner->mp_Name;

		if (pNameText != nullptr)
		{
			MakeWallInfo(nullptr, pNameText, MTReader::ctName);
		}
		else
			mp_Reader->MakeWallInfoArr(ma_WallInfo);
	}
	//if (ma_WallInfo.GetSize() == 1)
	//{
	//	SRWallInfo* pWallInfo = ma_WallInfo[0];
	//	if (!pWallInfo->CheckStory())
	//	{
	//		auto fromStory = -(mp_Reader->mp_Bldg->GetNumUnderStory() - 1);
	//		auto toStory = mp_Reader-> mp_Bldg->GetNumAboveStory() - 1;
	//		if (fromStory == 0)	fromStory = 1;
	//		if (toStory == 0)	toStory = -1;

	//		pWallInfo->SetFromStory(fromStory);
	//		pWallInfo->SetToStory(toStory);
	//	}
	//}

	SortWallInfo_byYCoord();
	SortTextArray_byYCoord(ma_Fck);
	SortTextArray_byYCoord(ma_Thick);
	SortTextArray_byYCoord(ma_HBar);
	SortTextArray_byYCoord(ma_VBar);
	SortTextArray_byYCoord(ma_ShearRatio);
}

void SRSectionBox::SortTextArray_byYCoord(vector<GMText*>& textArr)
{
	int NumInfo = textArr.size();
	for(int iInfo = 0; iInfo < NumInfo; iInfo++)
	{
		for(int jInfo = iInfo+1; jInfo < NumInfo; jInfo++)
		{
			GMText* pIInfo = textArr[iInfo];
			GMText* pJInfo = textArr[jInfo];
			if(pIInfo->m_Vec.m_Y > pJInfo->m_Vec.m_Y )
			{
				textArr[iInfo] = pJInfo;
				textArr[jInfo] = pIInfo;
			}
		}
	}
}

void SRSectionBox::SortWallInfo_byYCoord()
{
	int NumInfo = ma_WallInfo.GetSize();
	for(int iInfo = 0; iInfo < NumInfo; iInfo++)
	{
		for(int jInfo = iInfo+1; jInfo < NumInfo; jInfo++)
		{
			SRWallInfo* pIInfo = ma_WallInfo[iInfo];
			SRWallInfo* pJInfo = ma_WallInfo[jInfo];
			if(pIInfo->GetStartLocation() > pJInfo->GetStartLocation())
			{
				ma_WallInfo[iInfo] = pJInfo;
				ma_WallInfo[jInfo] = pIInfo;
			}
		}
	}
}

void SRSectionBox::MakeData_Excel()
{
	m_nErrorCode = 0;
	m_nWarningCode = 0;
	Analyze_WallInfo();
	CheckWallData();
}

bool SRSectionBox::MakeData_Beam()
{
	MSMaterial::Type_MSMaterial materialType = mp_Reader->m_MaterialType;
	if(materialType == MSMaterial::msConcrete)
		return MakeData_RcBeam();
	else if(materialType == MSMaterial::msSteel)
		return MakeData_SteelBeam();
	else
		return MakeData_SrcBeam();
}

bool SRSectionBox::MakeData_SteelBeam()
{
	MakeSteelDataInfo_fromName();
	ReNameByFloorNum();
	return true;
}

bool SRSectionBox::MakeData_SrcBeam()
{
	m_nErrorCode = 0;
	m_nWarningCode = 0;

	AnalyzeTexts_Beam();
	CurveGrouping();

	BOOL bFind = FindDimensions();
	if(bFind == FALSE)
	{
		m_nErrorCode = 1;
		return false;
	}
	m_Shape = FindEdges_ColumnH();
	if(m_Shape==0)								return false;
	AnalyzeCircles();
	FindMainBars_Beam();
	FindStrpBars_ColumnH();
	FindTieBars_ColumnH();
	FindSupportMainBar();
	CalcVBarVecOfEdges();
//	m_nErrorCode = !AnalyzeSectionText();
	return true;
}

bool SRSectionBox::MakeData_RcBeam()
{
	m_nErrorCode = 0;
	m_nWarningCode = 0;

	AnalyzeTexts_Beam();
	CurveGrouping();

	BOOL bFind = FindDimensions();
	if(bFind == FALSE)
	{
		m_nErrorCode = 1;
		return false;
	}
	m_Shape = FindEdges_ColumnH();
	if(m_Shape==0)								return false;
	AnalyzeCircles();
	FindMainBars_Beam();
	FindStrpBars_ColumnH();
	FindTieBars_ColumnH();

	CalcVBarVecOfEdges();
	if(m_nErrorCode != 0)	return false;
//	m_nErrorCode = !AnalyzeSectionText();

	return true;
}

bool SRSectionBox::MakeData_Slab()
{
	m_nErrorCode = 0;
	m_nWarningCode = 0;

	AnalyzeTexts_Slab();
	return true;
}

bool SRSectionBox::MakeData_Buttress()
{
	// test;
	m_nErrorCode = 0;
	m_nWarningCode = 0;

	AnalyzeTexts_Buttress();
	CurveGrouping();

	// 	if(!FindDimensions_ColumnH())	return false;
	// 	m_Shape = FindEdges_ColumnH();
	// 	if(m_Shape==0)								return false;
	// 	if(!AnalyzeCircles())					return false;
	// 	if(!FindMainBars_ColumnH())		return false;
	// 	if(!FindStrpBars_ColumnH())		return false;
	// 	if(!FindTieBars_ColumnH())		return false;

	if(FindDimensions() == FALSE)
	{
		m_nErrorCode = 1;
		return false;
	}
	m_Shape = FindEdges_ColumnH();
	if(m_Shape==0)								return false;
	AnalyzeCircles();
	FindMainBars_Buttress();
	FindStrpBars_ColumnH();
	FindTieBars_ColumnH();

	CalcVBarVecOfEdges();
	m_nErrorCode = !AnalyzeSectionText();

	return true;
}

static bool IsNumeric(CString& strText)
{
	strText.Replace(_T(","), _T(""));
	long nChar = strText.GetLength();
	for(long iChar=0; iChar<nChar; iChar++)
	{
		char aChr = strText[iChar];
		if(aChr==' ' || aChr==',')
			continue;
		if(aChr < '0' || aChr > '9')
			return false;
	}
	return true;
}

long SRSectionBox::MakeDimensionFromCircle()
{
	long Diameter = 0;
	for (int i = 0; i < ma_GMText.GetSize(); i++)
	{
		long cellType = mp_Reader->GetCellType(ma_GMText[i]);
		if (cellType == (long)MTReader::ctSize)
		{
			CString strData = ma_GMText[i]->m_strText;
			strData.Replace(_T(","), _T(""));
			CStringArray strArr;
			long nData = ParsingSize(strData, strArr);
			if (nData != 1)	return 0;

			Diameter = _ttol(strArr[0]);
			break;
		}
	}
	if (Diameter == 0)	return 0;

	GMCircle* pEdgeCircle = nullptr;
	long nCircle = ma_Circle.GetSize();
	for (long iCircle = 0; iCircle < nCircle; iCircle++)
	{
		GMCircle* pCircle = ma_Circle[iCircle];
		if (fabs(Diameter - pCircle->m_A * 2) < 1.0)
		{
			pEdgeCircle = pCircle;
			break;
		}
	}
	if (pEdgeCircle == nullptr)	return 0;
	

	CString sLength;
	sLength.Format(_T("%ld"), Diameter);
	SRDimension* pDim = MakeDimensionFromCircle(pEdgeCircle);
	if(pDim != nullptr)
		ma_SRDim.Add(pDim);
	return ma_SRDim.GetSize();
}

SRDimension* SRSectionBox::MakeDimensionFromLineVector(GM2DLineVector* pLineVec)
{
	if (pLineVec == nullptr)	return nullptr;
	CString length;
	length.Format(_T("%ld"), ROUND(pLineVec->Length()));

	SRDimension* pDim = new SRDimension(mp_Owner);
	pDim->SetLength(length);
	pDim->mp_LVec = new GM2DLineVector();
	pDim->mp_LVec->m_P0 = pLineVec->m_P0;
	pDim->mp_LVec->m_P1 = pLineVec->m_P1;
	pDim->m_Radian = pLineVec->GetRadian();

	return pDim;
}

SRDimension* SRSectionBox::MakeDimensionFromCircle(GMCircle* pCircle)
{
	if (pCircle == nullptr)	return nullptr;
	CString length;
	length.Format(_T("%ld"), ROUND(pCircle->m_A*2));

	SRDimension* pDim = new SRDimension(mp_Owner);
	pDim->SetLength(length);

	GMVectors points;
	pCircle->GetPoints(2, points);
	if (points.GetSize() != 2)	 return 0;

	GM2DLineVector* pDimLine = new GM2DLineVector();
	pDimLine->m_P0.SetXY(points[0]->m_X, points[0]->m_Y);
	pDimLine->m_P1.SetXY(points[1]->m_X, points[1]->m_Y);
	pDim->mp_LVec = pDimLine;
	pDim->m_Radian = pDimLine->GetRadian();

	return pDim;
}

long SRSectionBox::MakeDimensionFromSection()
{
	if (mp_Section == nullptr)	return ma_SRDim.GetSize();

	if (m_Shape == MSRcLineMemberM::RECT && mp_Section->ma_Edge.GetSize() == 4)
	{
		MREdge* pBottomEdge = mp_Section->ma_Edge[0];
		MREdge* pRightEdge = mp_Section->ma_Edge[1];

		GM2DLineVector* pWidthLine = dynamic_cast<GM2DLineVector*>(pBottomEdge->mp_Curve);
		GM2DLineVector* pDepthLine = dynamic_cast<GM2DLineVector*>(pBottomEdge->mp_Curve);

		SRDimension* pWidthDim = MakeDimensionFromLineVector(pWidthLine);
		SRDimension* pDepthDim = MakeDimensionFromLineVector(pDepthLine);
		if (pWidthDim == nullptr || pDepthDim == nullptr)	return ma_SRDim.GetSize();

		ma_SRDim.Add(pWidthDim);
		ma_SRDim.Add(pDepthDim);
	}
	else if (m_Shape == MSRcLineMemberM::CIRCLE && mp_SectCircle != nullptr)
	{
		SRDimension* pDim = MakeDimensionFromCircle(dynamic_cast<GMCircle*>(mp_SectCircle));
		if (pDim != nullptr)
			ma_SRDim.Add(pDim);

	}
	return ma_SRDim.GetSize();
}

long SRSectionBox::MakeDimensionFromLine()
{
	if (mp_Section != nullptr)
	{
		return MakeDimensionFromSection();
	}
	if (mp_Reader->m_Group == GROUP_COLUMN)
	{
		long numDim = MakeDimensionFromCircle();
		if (numDim == 1)	return numDim;
	}
	CArray<double, double> ValueArr;
	CTypedPtrArray<CObArray, GMText*> TextArr;
	long nText = ma_GMText.GetSize();
	for(long iText=nText-1; iText>=0; iText--)
	{
		GMText* pText = ma_GMText[iText];
		CString strText = pText->m_strText;
		if(!IsNumeric(strText))
			continue;

		long nLine = ma_LVec.GetSize();
		for(long iLine=0; iLine<nLine; iLine++)
		{
			GM2DLineVector* pDimLine = ma_LVec[iLine];
			CTypedPtrArray<CObArray, GM2DLineVector*> AuxLineArr;
			double DimAngle = pDimLine->GetDegree();
			double Angle = fabs(DimAngle - pText->m_dDegree);
			if( !(Angle < 1. || fabs(Angle-180.)<1.) )
				continue;
			if(pDimLine->Distance(pText->m_Vec) > pText->m_dHeight*2.)
				continue;
			double dLoc = pDimLine->OrthoBaseLocValue(pText->m_Vec);
			if(dLoc<0.25 || dLoc>0.75)
				continue;

			for(long jLine=0; jLine<nLine; jLine++)
			{
				GM2DLineVector* pAuxLine = ma_LVec[jLine];
				if(pDimLine==pAuxLine)
					continue;
				double AngleBetween = fabs(DimAngle - pAuxLine->GetDegree());
				if( !(fabs(AngleBetween-90.) < 1. || fabs(AngleBetween-270.)<1.) )
					continue;
				double MyLoc, UrLoc;
				GM2DVector CrossVec;
				long Result = pDimLine->Intersect(*pAuxLine, CrossVec, &MyLoc, &UrLoc);
				if(Result!=0 && Result!=-5)
					continue;
				if(MyLoc < -0.2 || MyLoc > 1.2)
					continue;
				if(UrLoc < -0.2 || UrLoc > 1.2)
					continue;
				long nAux = AuxLineArr.GetSize();
				for(long iAux=0; iAux<nAux; iAux++)
				{
					if(AuxLineArr[iAux]->IsColinear(*pAuxLine)!=0)
					{
						if(pAuxLine->Length() > AuxLineArr[iAux]->Length())
							AuxLineArr[iAux] = pAuxLine;
						break;
					}
				}
				if(iAux==nAux)
					AuxLineArr.Add(pAuxLine);
			}
			if(AuxLineArr.GetSize()==2)
			{
				SRDimension* pDim = new SRDimension(mp_Owner);
				ma_SRDim.Add(pDim);
				pDim->mp_Text = pText;
				ma_GMText.RemoveAt(iText);
				double MyLoc0 = 0.0, UrLoc0 = 0.0;
				GM2DVector CrossVec0;
				double MyLoc1 = 0.0, UrLoc1 = 0.0;
				GM2DVector CrossVec1;
				pDimLine->Intersect(*AuxLineArr[0], CrossVec0, &MyLoc0, &UrLoc0);
				pDimLine->Intersect(*AuxLineArr[1], CrossVec1, &MyLoc1, &UrLoc1);
				pDimLine->m_P0 = CrossVec0;
				pDimLine->m_P1 = CrossVec1;
				pDim->mp_LVec = pDimLine;
				pDim->m_Radian = pDimLine->GetRadian();
				pDim->m_Length = _ttol(strText);
				if (fabs(pDim->m_Length - 1) > DTOL_GM)
				{
					if (fabs((pDimLine->Length() / pDim->m_Length) - 1.) < DTOL_GM)
						pDim->m_NonScale = false;
					else
					{
						pDim->m_NonScale = true;
						m_dFactor = pDim->m_Length / pDimLine->Length();
					}
				}
				pDim->m_EdgePosition = UrLoc0<0.5 ? pDimLine->WhichSide(AuxLineArr[0]->m_P1) : pDimLine->WhichSide(AuxLineArr[0]->m_P0);
				break;
			}
		}
	}

	return ma_SRDim.GetSize();
}


void SRSectionBox::CurveGrouping()
{
	mp_Rect->GetEdges(ma_BoxLVec);

	GM2DLineVector* pBotLVec = ma_BoxLVec[0];
	GM2DLineVector* pRightLVec = ma_BoxLVec[1];

	long nCurve = ma_Curve.GetSize();
	for(long iLine=0; iLine<nCurve; iLine++)
	{
		GM2DGeometry* pCurve = ma_Curve[iLine];
		if(pCurve->m_ObjType == GT_ARC) 
		{
			ma_Arc.Add((GMArc*)pCurve);
			continue;
		}
		GM2DLineVector* pLine = (GM2DLineVector*)pCurve;
		ma_LVec.Add(pLine);
		if(pLine->IsHorizontalLine())
		{
			long nLine = ma_HoriLVec.GetSize();
			for(long jLine=0; jLine<nLine; jLine++)
			{
				if(pLine->m_P0.m_Y < ma_HoriLVec[jLine]->m_P0.m_Y)
				{
					ma_HoriLVec.InsertAt(jLine, pLine);
					break;
				}
			}
			if(jLine==nLine)
				ma_HoriLVec.Add(pLine);
		}
		else if(pLine->IsVerticalLine())
		{
			long nLine = ma_VertLVec.GetSize();
			for(long jLine=0; jLine<nLine; jLine++)
			{
				if(pLine->m_P0.m_X < ma_VertLVec[jLine]->m_P0.m_X)
				{
					ma_VertLVec.InsertAt(jLine, pLine);
					break;
				}
			}
			if(jLine==nLine)
				ma_VertLVec.Add(pLine);
		}
	}
}


BOOL SRSectionBox::HasSizeCell()
{
	if(m_Width>0. && m_Depth>0.)
		return TRUE;
	else 
		return FALSE;
}
void SRSectionBox::AnalyzeTexts_Column()
{
	long nText = ma_GMText.GetSize();
	for(long iText=0; iText<nText; iText++)
	{
		GMText* pText = ma_GMText[iText];
		long nCellType;
 		nCellType = mp_Reader->GetCellType(pText);
		if(nCellType==0)	continue;
//		enum CellType{ctName=1, ctStory, ctSize, ctMaterial, ctThick, ctWidth, ctDepth, ctMainBar, ctVBar, ctHBar, ctSection, ctTBar, ctBBar, ctStirrup, ctSBar1, ctSBar2, ctLoc, ctTieBar};
		CString strData = pText->m_strText;
//		strData.Replace(_T(","), _T(""));

		switch (nCellType)
		{
		case MTReader::ctName:	mp_Name = pText;	break;
		case MTReader::ctStory:	mp_Reader->Parsing_strStory(strData, m_nFromStory, m_nToStory);	mp_Story = pText;		mp_Story = pText;break;
		case MTReader::ctSize:
			{
				CStringArray strArr;
				long nData = ParsingSize(strData, strArr);
				if (nData == 1)
				{
					m_Width = m_Depth = _ttol(strArr[0]);
					m_sWidth = m_sDepth = strArr[0];
				}
				else if(nData==2)
				{
					m_Width = _ttol(strArr[0]);
					m_Depth = _ttol(strArr[1]);
					m_sWidth = strArr[0];
					m_sDepth = strArr[1];
// 					if(m_Width <= DTOL1 || m_Depth <= DTOL1)
// 					{
// 						mp_Reader->CreateErrorObject(_T("Column"), ERROR_LEVEL,ERROR_SIZE_INFO , mp_Owner->mp_Name->m_strText, mp_Owner->mp_Rect);
// 						mp_Owner->SetErrorCode(1);
// 					}
				}
				mp_Size = pText;
				break;
			}
		case MTReader::ctMaterial:
			{
				MSMaterial* pMate = mp_Reader->mp_Bldg->GetMaterial(strData);
				if(pMate)	m_nMaterialID = pMate->m_ID;
				break;
			}
		case MTReader::ctMainBar:
			{
// 				if (!MTReader::CorrectiveMainBar(strData))	break;

				if(mp_MainBar1)		ASSERT(0);
				else					mp_MainBar1 = new MSMainBar();
				mp_MainBar1->InitData(strData);
				break;
			}
		case MTReader::ctSBar1:
			{
 				//if (!MTReader::CorrectiveStirrupBar(strData))	break;
				if(mp_StrpBar1 == nullptr)
					mp_StrpBar1 = new MSStirrupBar(CString(strData));

				if (m_sStirrupBar1 != _T("")) m_sStirrupBar1 += MSMainBarSet::Parser_SLASH;
				m_sStirrupBar1 += strData;
				if(strData.Find(L"@") != -1 && strData.Find(L"D") != -1)
					ma_StirrupBar1.push_back(strData);

				break;
			}
		case MTReader::ctSBar2:
			{
 				//if (!MTReader::CorrectiveStirrupBar(strData))	break;
				if (mp_StrpBar2 == nullptr)
					mp_StrpBar2 = new MSStirrupBar(CString(strData));
				if (m_sStirrupBar2 != _T("")) m_sStirrupBar2 += MSMainBarSet::Parser_SLASH;
				m_sStirrupBar2 += strData;

				if (strData.Find(L"@") != -1 && strData.Find(L"D") != -1)
					ma_StirrupBar2.push_back(strData);

				break;
			}
		case MTReader::ctSubMainBar:
			{
			m_sSubMainBar = strData;
			if (mp_SubMainBar)	ASSERT(0);
				else
				{
					CString strTemp = strData;
					int indexD = strTemp.Find('D');
					int indexDash = strTemp.Find('-');
					if(indexD != -1 && indexDash != -1)
					{
						int indexColon = strTemp.Find(':');
						if(indexColon != -1)
						{
							int nBars = _ttol(strTemp.Mid(indexColon+1, strTemp.GetLength()-(indexDash-indexColon+1)));
							CString sDia = strTemp.Mid(indexD+1, strTemp.GetLength()-(indexD+1));
							strData.Format(_T("%ld-%s%s"), nBars,MSDeformedBar::m_sDefaultBarType,sDia);
							if(mp_SubMainBar==NULL)	mp_SubMainBar = new MSMainBar(CString(strData));
							else															mp_SubMainBar->Add(strData);
						}
						else
						{
							if (!MTReader::CorrectiveMainBar(strData))	break;
							if(mp_SubMainBar==NULL)		mp_SubMainBar = new MSMainBar(CString(strData));
							else																mp_SubMainBar->Add(strData);
						}
					}
				}
				break;
			}
		case MTReader::ctSectionName:
			{
				m_SteelSectionName = strData;
				break;
			}
		case MTReader::ctStudBolt:
			{
				if(strData.Find('@') == -1)
					continue;
				m_StudBolt = strData;
				break;
			}
		default:
			continue;
		}

		vector<int> figureIds;
		figureIds.push_back(FindTextFigureID(pText));
		mm_CellTypeToFigureIDs.insert(make_pair(nCellType, figureIds));
	}
	if (mp_Name == nullptr && mp_Owner->mp_Name != nullptr)
	{
		mp_Name = mp_Owner->mp_Name;
		vector<int> figureIds;
		figureIds.push_back(FindTextFigureID(mp_Name));
		mm_CellTypeToFigureIDs.insert(make_pair(MTReader::ctName, figureIds));
	}
// 	if(m_nFromStory==0 || m_nToStory == 0)
// 		mp_Reader->FindStoryInfo(this);
}
void SRSectionBox::AnalyzeTexts_Beam()
{
	long nText = ma_GMText.GetSize();
	for(long iText=0; iText<nText; iText++)
	{
		GMText* pText = ma_GMText[iText];
		long nCellType;
		nCellType = mp_Reader->GetCellType(pText);
		if(nCellType==0)	continue;
		//		enum CellType{ctName=1, ctStory, ctSize, ctMaterial, ctThick, ctWidth, ctDepth, ctMainBar, ctVBar, ctHBar, ctSection, ctTBar, ctBBar, ctStirrup, ctSBar1, ctSBar2, ctLoc};
		CString strData = pText->m_strText;
//		strData.Replace(_T(","), _T(""));

		switch (nCellType)
		{
		case MTReader::ctName:	mp_Name = pText;	break;
		case MTReader::ctStory:	mp_Reader->Parsing_strStory(strData, m_nFromStory, m_nToStory);	mp_Story = pText;	break;
		case MTReader::ctSize:
		{
			CStringArray strArr;
			long nData = ParsingSize(strData, strArr);
			if(nData==1)
			{
				m_Width = m_Depth = _ttol(strArr[0]);
				m_sWidth = m_sDepth = strArr[0];
			}
			else if(nData==2)
			{
				m_Width = _ttol(strArr[0]);
				m_Depth = _ttol(strArr[1]);
				m_sWidth = strArr[0];
				m_sDepth = strArr[1];
// 				if(m_Width <= DTOL1 || m_Depth <= DTOL1)
// 				{
// 					CString errorMsg = _T("Size Error!!!");
// 					if (mp_Owner->mp_Name != nullptr)	errorMsg = mp_Owner->mp_Name->m_strText;
// 					mp_Reader->CreateErrorObject(_T("Beam"), ERROR_LEVEL,ERROR_SIZE_INFO , errorMsg, mp_Owner->mp_Rect);
// 					mp_Owner->SetErrorCode(1);
// 				}
			}
			mp_Owner->m_Width = m_Width;
			mp_Owner->m_Depth = m_Depth;
			mp_Size = pText;
			break;
		}
		case MTReader::ctMaterial:
		{
			MSMaterial* pMate = mp_Reader->mp_Bldg->GetMaterial(strData);
			if(pMate)	m_nMaterialID = pMate->m_ID;
			break;
		}
		case MTReader::ctTBar:
		{
			m_sTopBar = strData;
			if (!MTReader::CorrectiveMainBarSet(strData))	continue;
			if (!MSMainBarSet::CheckMainBarSet(strData))	continue;
			if (mp_MainBarSet1 == NULL)	mp_MainBarSet1 = new MSMainBarSet();
			mp_MainBarSet1->SetBarString(strData);
			break;
		}
		case MTReader::ctBBar:
		{
			m_sBotBar = strData;
			if (!MTReader::CorrectiveMainBarSet(strData))	continue;
			if (!MSMainBarSet::CheckMainBarSet(strData))	continue;
			if (mp_MainBarSet2 == NULL)	mp_MainBarSet2 = new MSMainBarSet();
			mp_MainBarSet2->SetBarString(strData);
			break;
		}
		case MTReader::ctStirrup:
		{
			m_sStirrupBar1 = strData;
			CString strTemp = strData;
			long nIndex = strTemp.Find(_T("-"));
			if(nIndex < 0)	m_nStrp = 2;
			else	
			{
				m_nStrp = _ttol(strTemp.Mid(0, nIndex));
				if(m_nStrp < 2)	m_nStrp = 2;
				strData = strTemp.Mid(nIndex+1, strTemp.GetLength()-nIndex-1);
			}
			if (!MTReader::CorrectiveStirrupBar(strData))	break;

			if (mp_StrpBar1 == NULL)
			{
				mp_StrpBar1 = new MSStirrupBar();
				mp_StrpBar1->ma_strBar.RemoveAll();
			}
			mp_StrpBar1->Add(strData);
			break;
		}
		case MTReader::ctSBar2:
		{
			m_sSideBar = strData;
			CString strTemp = strData;
			int indexAt = strTemp.Find('@');
			int indexD = strTemp.Find('D');

			if(indexAt == -1)
			{
				if(strTemp.Find(_T("-")) < 0)	strData = _T("2-") + strTemp;

				if(mp_SideBar==NULL)		mp_SideBar = new MSMainBar(CString(strData));
				else											mp_SideBar->Add(strData);
			}
			else
			{
				if(indexD != -1)
					strData.Format(_T("%s%s"), MSDeformedBar::m_sDefaultBarType, strTemp.Mid(indexD+1, strTemp.GetLength()-(indexD+1)));

				if (!MTReader::CorrectiveStirrupBar(strData))	break;

				if(m_SideStirrupBar == NULL)
					m_SideStirrupBar = new MSStirrupBar(CString(strData));
				else
					m_SideStirrupBar->Add(strData);
			}
			break;
		}
		case MTReader::ctSpliteDistTol:
			m_nSpliteDistTol = _ttoi(strData);
			break;
		case MTReader::ctSectionName:
			m_SteelSectionName = strData;
			break;
		case MTReader::ctStudBolt:
			{
				if(strData.Find('@') == -1)
					continue;
				m_StudBolt = strData;
			}
			break;
		case MTReader::ctSubMainBar:
			{
				m_sSubMainBar = strData;
				if(mp_SubMainBar==NULL)	mp_SubMainBar = new MSMainBar(CString(strData));
				else							mp_SubMainBar->Add(strData);
				break;
			}
		default:
			break;
		}
		vector<int> figureIds;
		figureIds.push_back(FindTextFigureID(pText));
		mm_CellTypeToFigureIDs.insert(make_pair(nCellType, figureIds));
	}
	if(mp_MainBar1)	
		m_nBeamSection = mp_MainBar1->ma_strBar.GetSize();

	if(m_Width < DTOL1 || m_Depth < DTOL1)
	{
		m_Width = mp_Owner->m_Width;
		m_Depth = mp_Owner->m_Depth;
	}

	if(m_SteelSectionName != _T(""))
		mp_Owner->m_SteelSectionName = m_SteelSectionName;
	if(m_StudBolt != _T(""))
		mp_Owner->m_StudBolt = m_StudBolt;

}

void SRSectionBox::AnalyzeTexts_Buttress()
{
	long nText = ma_GMText.GetSize();
	for(long iText=0; iText<nText; iText++)
	{
		GMText* pText = ma_GMText[iText];
		long nCellType;
		nCellType = mp_Reader->GetCellType(pText);
		if(nCellType==0)	continue;
		//		enum CellType{ctName=1, ctStory, ctSize, ctMaterial, ctThick, ctWidth, ctDepth, ctMainBar, ctVBar, ctHBar, ctSection, ctTBar, ctBBar, ctStirrup, ctSBar1, ctSBar2, ctLoc};
		CString strData = pText->m_strText;
//		strData.Replace(_T(","), _T(""));

		switch (nCellType)
		{
		case MTReader::ctName:	mp_Name = pText;	break;
		case MTReader::ctStory:	mp_Reader->Parsing_strStory(strData, m_nFromStory, m_nToStory);	mp_Story = pText;	break;
		case MTReader::ctSize:
			{
				CStringArray strArr;
				long nData = ParsingSize(strData, strArr);
				if (nData == 1)
				{
					m_Width = m_Depth = _ttol(strArr[0]);
					m_sWidth = m_sDepth = strArr[0];
				}
				else if(nData==2)
				{
					m_Width = _ttol(strArr[0]);
					m_Depth = _ttol(strArr[1]);
					m_sWidth = strArr[0];
					m_sDepth = strArr[1];

// 					if(m_Width <= DTOL1 || m_Depth <= DTOL1)
// 					{
// 						mp_Reader->CreateErrorObject(_T("Buttress"), ERROR_LEVEL,ERROR_SIZE_INFO , mp_Owner->mp_Name->m_strText, mp_Owner->mp_Rect);
// 						mp_Owner->SetErrorCode(1);
// 					}
				}
				mp_Size = pText;
				break;
			}
		case MTReader::ctMaterial:
			{
				MSMaterial* pMate = mp_Reader->mp_Bldg->GetMaterial(strData);
				if(pMate)	m_nMaterialID = pMate->m_ID;
				break;
			}
		case MTReader::ctTBar:
			{
			m_sTopBar = strData;
			if (mp_MainBarSet1 == NULL)	mp_MainBarSet1 = new MSMainBarSet();
				mp_MainBarSet1->SetBarString(strData);
				break;
			}
		case MTReader::ctBBar:
			{
			m_sBotBar = strData;
			if (mp_MainBarSet2 == NULL)	mp_MainBarSet2 = new MSMainBarSet();
			mp_MainBarSet2->SetBarString(strData);
			break;
			}
		case MTReader::ctStirrup:
			{
			m_sStirrupBar1 = strData;
				CString strTemp = strData;
				long nIndex = strTemp.Find(_T("-"));
				if(nIndex < 0)	m_nStrp = 2;
				else	
				{
					m_nStrp = _ttol(strTemp.Mid(0, nIndex));
					strData = strTemp.Mid(nIndex+1, strTemp.GetLength()-nIndex-1);
				}

				if (!MTReader::CorrectiveStirrupBar(strData))	break;
				if (mp_StrpBar1 == NULL)
				{
					mp_StrpBar1 = new MSStirrupBar();
					mp_StrpBar1->ma_strBar.RemoveAll();
				}

				mp_StrpBar1->Add(strData);
				break;
			}
		case MTReader::ctSBar2:
			{
				m_sSideBar = strData;
				CString strTemp = strData;
				int indexAt = strTemp.Find('@');
				if(indexAt == -1)
				{
					if(strTemp.Find(_T("-")) < 0)	strData = _T("2-") + strTemp;

					if(mp_SideBar==NULL)		mp_SideBar = new MSMainBar(CString(strData));
					else											mp_SideBar->Add(strData);
				}
				else
				{
					if (!MTReader::CorrectiveStirrupBar(strData))	break;
					if(m_SideStirrupBar == NULL)
						m_SideStirrupBar = new MSStirrupBar(CString(strData));
					else
						m_SideStirrupBar->Add(strData);
				}
				break;
			}
		default:
			continue;
		}

		vector<int> figureIds;
		figureIds.push_back(FindTextFigureID(pText));
		mm_CellTypeToFigureIDs.insert(make_pair(nCellType, figureIds));
	}
	if(mp_MainBar1)	
		m_nBeamSection = mp_MainBar1->ma_strBar.GetSize();
}

void SRSectionBox::AnalyzeTexts_Wall()
{
	long nText = ma_GMText.GetSize();
	for(long iText=0; iText<nText; iText++)
	{
		GMText* pText = ma_GMText[iText];
		long nCellType;
		nCellType = mp_Reader->GetCellType(pText);
		if(nCellType==0)	continue;

		if(nCellType==MTReader::ctName)
		{
			mp_Name = pText;
//			mm_CellTypeToFigureID.insert(make_pair(nCellType, FindTextFigureID(pText)));
		}
		else if(mp_Owner->mp_Name == pText)
		{
			mp_Name = pText;
//			mm_CellTypeToFigureID.insert(make_pair(nCellType, FindTextFigureID(pText)));
		}
		else
		{
			double dLoc = 0.0;
			dLoc = pText->m_Vec.m_Y;

// 			if(mp_Reader->m_Type == TABLE_1)
// 				dLoc = pText->m_Vec.m_Y;
// 			else
// 				dLoc = pText->m_Vec.m_X;
			SRWallInfo* pWallInfo = FindWallInfo(pText->m_Vec.m_Y);
			pWallInfo = MakeWallInfo(pWallInfo, pText, nCellType);
		}
	}
}

SRWallInfo* SRSectionBox::FindWallInfo(double dLoc)
{
	long NumInfo = ma_WallInfo.GetSize();
	for(long iInfo=0; iInfo < NumInfo; iInfo++)
	{
		SRWallInfo* pInfo = ma_WallInfo[iInfo];
		if(pInfo->GetStartLocation() <= dLoc && dLoc <= pInfo->GetEndLocation())
			return pInfo;
	}
	return NULL;
}

SRWallInfo* SRSectionBox::MakeWallInfo(SRWallInfo* pWallInfo, GMText* pText, long nCellType)
{
	CString strData = pText->m_strText;
//	strData.Replace(_T(","), _T(""));
	//		enum CellType{ctName=1, ctStory, ctSize, ctMaterial, ctThick, ctWidth, ctDepth, ctMainBar, ctVBar, ctHBar, ctSection, ctTBar, ctBBar, ctStirrup, ctSBar1, ctSBar2, ctLoc, ctTieBar};

	double dLoc = 0;
	double dTol = pText->m_dHeight;

	if(pWallInfo==NULL)
	{
		pWallInfo = new SRWallInfo();
		dLoc = pText->m_Vec.m_Y;
		
		pWallInfo->SetStartLocation(pText->m_Vec.m_Y-dTol/2);
		pWallInfo->SetEndLocation(pText->m_Vec.m_Y+dTol/2);
		ma_WallInfo.Add(pWallInfo);
	}
	long fromStory = 0;
	long toStory = 0;
	switch(nCellType)
	{

	case MTReader::ctStory:	mp_Reader->Parsing_strStory(strData, fromStory, toStory);
	{
		if (fromStory == 0 || toStory == 0)
		{
			mp_Reader->CreateErrorObject(_T("Wall"), ERROR_LEVEL, STORY_ERROR, pText->m_strText, mp_Owner->mp_Rect);
		}
		pWallInfo->SetFromStory(fromStory);
		pWallInfo->SetToStory(toStory);
		mp_Story = pText;	break;
	}
	case MTReader::ctThick:	pWallInfo->SetThick(_ttol(strData));	break;
	case MTReader::ctMaterial:
		{
			MSMaterial* pMate = mp_Reader->mp_Bldg->GetMaterial(strData);
			if(pMate)	pWallInfo->SetMaterialID(pMate->m_ID);
			break;
		}
	case MTReader::ctVBar:	pWallInfo->SetVBar(strData);	break;
	case MTReader::ctHBar:	pWallInfo->SetHBar(strData);	break;
	case MTReader::ctShearRatio: pWallInfo->SetShearRatio(strData);	break;
	default:
		return pWallInfo;
	}

	pWallInfo->AddTextFigureID(nCellType, FindTextFigureID(pText));
	return pWallInfo;
}

SRWallInfo* SRSectionBox::MakeWallInfo(int nCellType, int Row, CString strData)
{
	SRWallInfo* pWallInfo = NULL;
	int NumInfo = ma_WallInfo.GetSize();
	for(int iInfo = 0; iInfo < NumInfo; iInfo++)
	{
		if(ma_WallInfo[iInfo]->GetRow() == Row)
		{
			pWallInfo = ma_WallInfo[iInfo];
			break;
		}
	}

	if(pWallInfo==NULL)
	{
		pWallInfo = new SRWallInfo();
		pWallInfo->SetRow(Row);
		ma_WallInfo.Add(pWallInfo);
	}
//	strData.Replace(_T(","), _T(""));

	long fromStory = 0;
	long toStory = 0;
	switch (nCellType)
	{
	case MTReader::ctStory:	mp_Reader->Parsing_strStory(strData, fromStory, toStory);
	{
		pWallInfo->SetFromStory(fromStory);
		pWallInfo->SetToStory(toStory);
		break;
	}
	case MTReader::ctThick:	pWallInfo->SetThick(_ttol(strData));	break;
	case MTReader::ctMaterial:
		{
			MSMaterial* pMate = mp_Reader->mp_Bldg->GetMaterial(strData);
			if(pMate == NULL)
			{
				CString MateName;
				MateName.Format(_T("C%s"), strData);
				pMate = mp_Reader->mp_Bldg->GetMaterial(MateName);
			}
			if(pMate)	pWallInfo->SetMaterialID(pMate->m_ID);
			break;
		}
	case MTReader::ctVBar:	pWallInfo->SetVBar(strData);	break;
	case MTReader::ctHBar:	pWallInfo->SetHBar(strData);	break;
	case MTReader::ctShearRatio: pWallInfo->SetShearRatio(strData);	break;
	default:
		return pWallInfo;
	}

	return pWallInfo;
}

 void SRSectionBox::Analyze_WallInfo()
 {
	 SortWallInfo();

	 SRWallInfo* pPrevInfo = NULL;
	 int NumInfo = ma_WallInfo.GetSize();
	 for(int iInfo = 0; iInfo < NumInfo; iInfo++)
	 {
		 SRWallInfo* pWallInfo = ma_WallInfo[iInfo];
		 if(pWallInfo->CheckData())
  		 {
			 pPrevInfo = pWallInfo;
			continue;
		 }

		 if(pPrevInfo == NULL)
			 continue;
		 if(pWallInfo->GetMaterialID() == 0)
			 pWallInfo->SetMaterialID(pPrevInfo->GetMaterialID());
		 if(pWallInfo->GetThick() < DTOL1)
			 pWallInfo->SetThick(pPrevInfo->GetThick());
		 if(pWallInfo->GetVBar() == "")
			 pWallInfo->SetVBar(pPrevInfo->GetVBar());
		 if(pWallInfo->GetHBar() == "")
			 pWallInfo->SetHBar(pPrevInfo->GetHBar());
		 if (pWallInfo->GetShearRatio() == "")
			 pWallInfo->SetShearRatio(pPrevInfo->GetShearRatio());
		 pPrevInfo = pWallInfo;
	 }
 }

void SRSectionBox::SortWallInfo()
{
	int NumInfo = ma_WallInfo.GetSize();
	for(int iInfo = 0; iInfo < NumInfo; iInfo++)
	{
		for(int jInfo = iInfo+1; jInfo < NumInfo; jInfo++)
		{
			SRWallInfo* pIInfo = ma_WallInfo[iInfo];
			SRWallInfo* pJInfo = ma_WallInfo[jInfo];
			if(pIInfo->GetFromStory() > pJInfo->GetFromStory())
			{
				ma_WallInfo[iInfo] = pJInfo;
				ma_WallInfo[jInfo] = pIInfo;
			}
		}
	}
}

void SRSectionBox::CheckWallData()
{
	int NumInfo = ma_WallInfo.GetSize();
	for(int iInfo = NumInfo-1; iInfo >= 0; iInfo--)	
	{
		SRWallInfo* pWallInfo = ma_WallInfo[iInfo];
		if(pWallInfo->CheckData()==false)
		{
			ma_WallInfo.RemoveAt(iInfo);
			delete pWallInfo;
		}
	}
}
bool SRSectionBox::IsFlatSlabType()
{
	long nText = ma_GMText.GetSize();
	for (long iText = 0; iText < nText; iText++)
	{
		GMText* pText = ma_GMText[iText];
		long nCellType;
		nCellType = mp_Reader->GetCellType(pText);
		if (nCellType != MTReader::ctSlabFlatType)	continue;

		return mp_Reader->IsFlatSlabType(pText->m_strText);
	}
	return false;
}
void SRSectionBox::AnalyzeTexts_Slab()
{
	long nText = ma_GMText.GetSize();
	bool isFlatSlab = IsFlatSlabType();

	for(long iText=0; iText<nText; iText++)
	{
		GMText* pText = ma_GMText[iText];
		long nCellType = mp_Reader->GetCellType(pText, isFlatSlab);
		if(nCellType==0)		continue;
		double dLoc = 0.0;
		dLoc = pText->m_Vec.m_Y;
		MakeSlabInfo(pText, nCellType);
	}
	if (mp_SlabInfo != nullptr)
	{
		mp_SlabInfo->SetIsFlatSlabType(isFlatSlab);
		mp_SlabInfo->AnalyzeRebars(mp_Reader);
	}
}

void SRSectionBox::MakeSlabInfo(GMText* pText, long nCellType)
{
	CString strData = pText->m_strText;
//	strData.Replace(_T(","), _T(""));

	if(mp_SlabInfo==NULL)
		mp_SlabInfo = new SRSlabInfo();

	double dLoc = pText->m_Vec.m_Y;
	auto it = mp_SlabInfo->m_CellTypeLocMap.find(nCellType);
	if (it == mp_SlabInfo->m_CellTypeLocMap.end())
	{
		vector<GMText*> textList;
		textList.push_back(pText);
		if (nCellType == MTReader::ctThick)
		{
			int Thick = _ttol(strData);
			if (Thick < DTOL1)	return;
		}
		mp_SlabInfo->m_CellTypeLocMap.insert(make_pair(nCellType, textList));

		switch (nCellType)
		{
		case MTReader::ctName:	mp_SlabInfo->m_Name = strData;	break;
		case MTReader::ctThick:	mp_SlabInfo->m_nThick = _ttol(strData);	break;
		default:
			return;
		}
	}
	else
	{
		it->second.push_back(pText);
	}
	mp_SlabInfo->AddTextFigureID(nCellType, FindTextFigureID(pText));
}
MSRcStrip::STRIP_TYPE SRSlabInfo::GetStripType(bool bShort, bool IsBent)
{
	MSRcStrip::STRIP_TYPE type = MSRcStrip::TYPE_NONE;
	CString Bar1, Bar2, Bar3, Bar4, Bar5, Bar6, Bar7;
	if(bShort)
	{
		Bar1 = m_X1;	Bar2 = m_X2;	Bar3 = m_X3;	Bar4 = m_X4;	Bar5 = m_X5;	Bar6 = m_X6;	Bar7 = m_X7;
	}
	else
	{
		Bar1 = m_Y1;	Bar2 = m_Y2;	Bar3 = m_Y3;	Bar4 = m_Y4;	Bar5 = m_Y5;	Bar6 = m_Y6;	Bar7 = m_Y7;
	}

	CString Empty = _T("");
	if (isFlatSlabType)
	{
		return MSRcStrip::TYPE_A;
	}
	if((Bar1 != Empty && Bar4 != Empty) && (Bar2 == Empty && Bar3 == Empty && Bar7 == Empty))
	{
		if(Bar5 == Empty)
		{
			Bar5 = Bar1;
			if(bShort)	m_X5 = m_X1;
			else             m_Y5 = m_Y1;
		}
		if(Bar6 == Empty)
		{
			Bar6 = Bar2;
			if(bShort)	m_X6 = m_X4;
			else             m_Y6 = m_Y4;
		}
		return MSRcStrip::TYPE_A;
	}

	if(Bar5 == Empty || Bar6 == Empty)
		return MSRcStrip::TYPE_NONE;

	else if((Bar2 != Empty && Bar4 != Empty)  && (Bar1 == Empty && Bar3 == Empty && Bar7 == Empty))
		return MSRcStrip::TYPE_B;
	else if((Bar2 != Empty && Bar4 != Empty && Bar7 != Empty) && (Bar1 == Empty && Bar3 == Empty))
		return MSRcStrip::TYPE_C;
	else if((Bar1 != Empty && Bar2 != Empty && Bar4 != Empty && Bar7 != Empty) && Bar3 == Empty)
		return MSRcStrip::TYPE_D;
	else if((Bar1 != Empty && Bar2 != Empty && Bar4 != Empty)  && (Bar3 == Empty && Bar7 == Empty))
		return MSRcStrip::TYPE_E;
	else if((Bar3 != Empty && Bar4 != Empty)  && (Bar1 == Empty && Bar2 == Empty && Bar7 == Empty))
		return MSRcStrip::TYPE_F;
	else if((Bar2 != Empty && Bar3 != Empty && Bar4 != Empty)  && Bar1 == Empty && Bar7 == Empty)
		return MSRcStrip::TYPE_H;
	else if((Bar1 != Empty && Bar3 != Empty && Bar4 != Empty)  && (Bar2 == Empty  && Bar7 == Empty))
		return MSRcStrip::TYPE_G;
	else if((Bar1 != Empty && Bar4 != Empty && Bar7 != Empty) && (Bar2 == Empty && Bar3 == Empty))
		return MSRcStrip::TYPE_I;

	return type;
}
long SRSectionBox::ParsingSize(CString strData, CStringArray& ValueArr)
{
	ValueArr.RemoveAll();

    strData.Replace(_T(","), _T(""));
	long nSeperator = strData.Find(_T("X"));
	if(nSeperator == -1)
	{
		nSeperator = strData.Find(_T("×"));	// 특수문자 ×
	}
	if(nSeperator==-1)
		ValueArr.Add(strData);
	else 
	{
		long nLen = strData.GetLength();
		CString strData1 = strData.Mid(0, nSeperator);
		CString strData2 = strData.Mid(nSeperator+1, nLen-(nSeperator+1));
		if (mp_Reader->m_eSizeType == MTReader::Width_X_Depth)
		{
			ValueArr.Add(strData1);
			ValueArr.Add(strData2);
		}
		else
		{
			ValueArr.Add(strData2);
			ValueArr.Add(strData1);
		}
	}
	return ValueArr.GetSize();
}

bool SRSectionBox::SetDimensionSize(double dimWidth, double dimDepth)
{
    auto fixWidth = ROUND(dimWidth * 0.1, 1) * 10;
    auto fixDepth = ROUND(dimDepth *0.1, 1) * 10;
    m_DimensionWidth = fixWidth;
	m_DimensionDepth = fixDepth;
	m_hasDimensionSize = true;

// 	if (HasSizeCell() && (fabs(m_Width - dimWidth) > DTOL1 || fabs(m_Depth - dimDepth) > DTOL1))
// 	{
// 		if (mp_Reader->m_eRecognizeSizeType == MTReader::NONE)
// 		{
// 			CString groupName = _T("Beam");
// 			if (mp_Reader->m_Group == GROUP_COLUMN)
// 				groupName = _T("Column");
// 			else if (mp_Reader->m_Group == GROUP_BUTTRESS)
// 				groupName = _T("Buttress");
// 
// 			CString Name = _T("");
// 			if (mp_Owner->mp_Name)	Name = mp_Owner->mp_Name->m_strText;
// 			mp_Reader->CreateErrorObject(groupName, ERROR_LEVEL, ERROR_SIZE_INFO, Name, mp_Owner->mp_Rect);
// 			return false;
// 		}
// 		else if (mp_Reader->m_eRecognizeSizeType == MTReader::Dimension)
// 		{
// 			m_DimensionWidth = dimWidth;
// 			m_DimensionDepth = dimDepth;
// 			mp_Owner->m_Width = m_Width = dimWidth;
// 			mp_Owner->m_Depth = m_Depth = dimDepth;
// 		}
// 	}
// 	else
// 	{
// 		m_DimensionWidth = m_Width = dimWidth;
// 		m_DimensionDepth = m_Depth = dimDepth;
// 		mp_Owner->m_Width = m_Width;
// 		mp_Owner->m_Depth = m_Depth;
// 	}
	return true;
}
BOOL SRSectionBox::FindDimensions()
{
	CString GroupName = _T("Beam");
	if(mp_Reader->m_Group == GROUP_COLUMN)
		GroupName = _T("Column");
	else if(mp_Reader->m_Group == GROUP_BUTTRESS)
		GroupName = _T("Buttress");

	long nDim = ma_SRDim.GetSize();
	if(nDim==1)	
	{
		CString Name = _T("");
		if(mp_Owner->mp_Name)	Name = mp_Owner->mp_Name->m_strText;
		if(mp_Reader->m_Group == GROUP_BEAM)
		{
			mp_Reader->CreateErrorObject(GroupName, ERROR_LEVEL,ERROR_SIZE_INFO , Name, mp_Owner->mp_Rect);
			return FALSE;
		}
		double width = ma_SRDim[0]->m_Length*m_dFactor;
		double depth = width;
		if (!SetDimensionSize(width, depth))	return false;
	}
	else if(nDim==2)
	{
		SRDimension* pHoriDim, *pVertDim;
		if(fabs(sin(ma_SRDim[0]->m_Radian)) < DTOL_GM)
		{
			pHoriDim = ma_SRDim[0];
			pVertDim = ma_SRDim[1];
		}
		else
		{
			pHoriDim = ma_SRDim[1];
			pVertDim = ma_SRDim[0];
		}

		double width = 0, depth = 0;
		width = pHoriDim->m_Length*m_dFactor;
		depth = pVertDim->m_Length*m_dFactor;
		if (!SetDimensionSize(width, depth))	return false;
	}
	if(nDim <= 2)	m_bStandardSection = TRUE;

	if(nDim > 2 && mp_Reader->m_Group == GROUP_BEAM)
	{
		SRDimension* pHoriDim = NULL;
		SRDimension* pVertDim = NULL;
		for(int i = 0; i < nDim; i++)
		{
			SRDimension* pSRDim = ma_SRDim[i];
			bool IsHorizontalDimension = true;
			double dDegree = fabs(pSRDim->mp_LVec->GetDegree());
			if(45. <= dDegree && dDegree < 135)
				IsHorizontalDimension = false;
			else if(225. <= dDegree && dDegree < 315)
				IsHorizontalDimension = false;
			if(IsHorizontalDimension == true && pHoriDim == NULL)
				pHoriDim = pSRDim;
			else if(IsHorizontalDimension == false)
			{
				if(pVertDim == NULL)
					pVertDim = pSRDim;
				else
				{
					if(pVertDim->mp_LVec->m_P0.m_X > pSRDim->mp_LVec->m_P0.m_X)
						pVertDim = pSRDim;
				}
			}
		}
		if(pHoriDim && pVertDim)
		{
			ma_SRDim.RemoveAll();
			ma_SRDim.Add(pHoriDim);
			ma_SRDim.Add(pVertDim);

			double width = 0, depth = 0;
			width = pHoriDim->m_Length*m_dFactor;
			depth = pVertDim->m_Length*m_dFactor;
			if (!SetDimensionSize(width, depth))	return false;
		}
	}
	if(nDim > 0)	return TRUE;

	nDim = MakeDimensionFromLine();
	if(nDim == 1)
	{
		m_Width = m_Depth = ma_SRDim[0]->m_Length*m_dFactor;
		m_bStandardSection = TRUE;
		mp_Owner->m_Width = m_Width;
		mp_Owner->m_Depth = m_Depth;
		return TRUE;
	}
	else if(nDim==2)
	{
		SRDimension* pHoriDim, *pVertDim;
		if(fabs(sin(ma_SRDim[0]->m_Radian)) < DTOL_GM)
		{
			pHoriDim = ma_SRDim[0];
			pVertDim = ma_SRDim[1];
		}
		else
		{
			pHoriDim = ma_SRDim[1];
			pVertDim = ma_SRDim[0];
		}
		m_Width = pHoriDim->m_Length*m_dFactor;
		m_Depth = pVertDim->m_Length*m_dFactor;
		m_bStandardSection = TRUE;
		mp_Owner->m_Width = m_Width;
		mp_Owner->m_Depth = m_Depth;
		return TRUE;
	}
	if(nDim > 0)
		return TRUE;

	if(HasSizeCell()==FALSE)
	{
		if(m_BoxNumber == 1)	// Box가 여러개(보)인 경우 에러 메시지는 1번만 표시
		{
			if(mp_Owner->mp_Name)
				mp_Reader->CreateErrorObject(GroupName, ERROR_LEVEL, ERROR_SIZE_INFO , mp_Owner->mp_Name->m_strText, mp_Owner->mp_Rect);
			else
				mp_Reader->CreateErrorObject(GroupName, ERROR_LEVEL,ERROR_SIZE_INFO , _T("NONE"), mp_Owner->mp_Rect);
		}
		return FALSE;  // 이 섹션박스에는 단면 크기 정보가 없으므로 인식 못하고 스킾
	}
		
	m_bStandardSection = TRUE;
	return TRUE;
}

BOOL SRSectionBox::SetSectionShape()  
{
	
	return TRUE;
}

GM2DLineVector* SRSectionBox::MatchEdgeWithStirrupLine(MREdge* pEdge)
{
// 	GM2DGeometry* pEdgeGeom = pEdge->mp_Curve;
// 	if(pEdgeGeom->GetObjectType()=="GM2DLineVector")
// 	{
// 		GM2DLineVector* pEdgeLVec = (GM2DLineVector*)pEdgeGeom;
// 		long nLine = ma_GMLine.GetSize();
// 		for(long iLine=0; iLine<nLine; iLine++)
// 		{
// 			GM2DLineVector* pLine = ma_GMLine[iLine];
// 			if(pLine->m_Tag==1) continue;
// 			if(pLine->IsParallel4DTol(*pEdgeLVec) == 0) continue;
// 
// 
// // 			pDim->mp_EdgeLVec = pLine;
// // 			pLine->m_Tag = 1;
// 			return pLine;
// 		}
// 	}

	return NULL;
}

void SRSectionBox::FindLinkedSectionLine(GM2DVector* pEndVec, bool IsStartPoint)
{
	GM2DVector SVec, EVec;
	GM2DVector PrevEndVec = *pEndVec;
	long nCurve = ma_Curve.GetSize();
	for(long iCurve=0; iCurve<nCurve; iCurve++)
	{
		GM2DGeometry* pCurve = ma_Curve[iCurve];
		if(pCurve->m_ObjType == GT_LINE)
		{
			GM2DLineVector* pLVec = (GM2DLineVector*)pCurve;
			SVec = pLVec->m_P0;
			EVec = pLVec->m_P1;
		}
		else
		{
			GMArc* pArc = (GMArc*)pCurve;
			SVec = pArc->GetStart2DVec();
			EVec = pArc->GetEnd2DVec();
		}

		if(PrevEndVec.Distance(SVec) < m_LinkTol)
		{
			if(IsStartPoint && pCurve->m_ObjType == GT_LINE)
			{
				GM2DLineVector* pLVec = (GM2DLineVector*)pCurve;
				pLVec->ReverseDirection();
			}	

			AddSectionLine(pCurve);
			nCurve--;
			PrevEndVec = EVec;
		}
		else if(PrevEndVec.Distance(EVec) < m_LinkTol)
		{
			if(!IsStartPoint && pCurve->m_ObjType == GT_LINE)
			{
				GM2DLineVector* pLVec = (GM2DLineVector*)pCurve;
				pLVec->ReverseDirection();
			}	
			AddSectionLine(pCurve);
			nCurve--;
			PrevEndVec = SVec;
		}
	}
}

BOOL SRSectionBox::MakeSectionPolyline()
{
/*

	GM2DPolyline ThePLine(TRUE);
	CTypedPtrArray<CObArray, GM2DLineVector*> TheSelLines;

	ThePLine.Add(new GM2DVector(pLine->m_P0));
	ThePLine.Add(new GM2DVector(pLine->m_P1));
	TheSelLines.Add(pLine);

	pLine->m_Tag = 1;

	double LinkTol = pLine/1000.;
	BOOL bPolygon = SearchLinkedSectionLine(TheSelLines, ThePLine, LinkTol);
	if(ThePLine.GetSize() == 2)
		return FALSE;		

	if(bPolygon == FALSE)
		bPolygon = CompletePolygonInWallLayer(TheSelLines, ThePLine, LinkTol);
	if(bPolygon == FALSE)
		bPolygon = CompletePolygonInWallLayer(TheSelLines, ThePLine, LinkTol);

	if(bPolygon == FALSE)
	{
		long RtnVal = CompleteGMPolygon(ThePLine);
		if(RtnVal == 0)
			return FALSE;
	}
*/
	return TRUE;
}

long SRSectionBox::CompleteGMPolygon(GM2DPolyline& ThePLine)
{
/*

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
			if(fabs(MyLoc-1.)*FirstLine.Length() < LinkTol)
				// 				ThePLine.ma_Point[NumPoint-1]->SetXY(CrossVec.m_X, CrossVec.m_Y);
				ThePLine.ma_Point.RemoveAt(NumPoint-1);
			else if(fabs(UrLoc-1.)*LastLine.Length() < LinkTol)
			{
				ThePLine.ma_Point[0]->SetXY(CrossVec.m_X, CrossVec.m_Y);
				ThePLine.ma_Point.RemoveAt(NumPoint-1);
			}
			return 3;
		}
	}
*/
	return 0;
}

BOOL SRSectionBox::FindEdgesOn0Dim()
{
	if(ma_VertLVec.GetSize()<2 || ma_HoriLVec.GetSize()<2)
		return FALSE;

	if(m_Width < 100. || m_Depth < 100.)
		return FALSE;

	double MinX = ma_VertLVec[0]->m_P0.m_X;
	double MaxX = ma_VertLVec[ma_VertLVec.GetSize()-1]->m_P0.m_X;
	double MinY = ma_HoriLVec[0]->m_P0.m_Y;
	double MaxY = ma_HoriLVec[ma_HoriLVec.GetSize()-1]->m_P0.m_Y;
	double DeltaX = MaxX - MinX;
	double DeltaY = MaxY - MinY;

	if(!(m_Width > 100. && m_Depth > 100.))
		return FALSE;

	CString GroupName = _T("");
	int WarningCode;
	switch(mp_Reader->m_Group)
	{
	case GROUP_COLUMN:	GroupName = _T("Column");		WarningCode = WARNING_SIZE_DIFF;	break;
	case GROUP_BEAM:			GroupName = _T("Beam");			WarningCode = WARNING_SIZE_DIFF;	break;
	case GROUP_BUTTRESS:	GroupName = _T("Buttress");		WarningCode = WARNING_SIZE_DIFF;	break;
	default:
		ASSERT(0);
	}

	// Dimension이 하나도 없을때는 사이즈 정보로 생성하기를 원함 - 김은석 과장
	GM2DLineVector TempLVec(MinX, MinY, MaxX, MinY);
	GM2DGeometry* pCurve = FindAndAddEdgeOnLVec(&TempLVec);
	TempLVec.m_P0.SetXY(MaxX, MinY);
	TempLVec.m_P1.SetXY(MaxX, MaxY);
	pCurve = FindAndAddEdgeOnLVec(&TempLVec);
	TempLVec.m_P0.SetXY(MaxX, MaxY);
	TempLVec.m_P1.SetXY(MinX, MaxY);
	pCurve = FindAndAddEdgeOnLVec(&TempLVec);
	TempLVec.m_P0.SetXY(MinX, MaxY);
	TempLVec.m_P1.SetXY(MinX, MinY);
	pCurve = FindAndAddEdgeOnLVec(&TempLVec);
	if(fabs(DeltaX-m_Width) < m_LinkTol && fabs(DeltaY-m_Depth) < m_LinkTol)
		return TRUE;
	else
	{
		if(mp_Owner->mp_Name)
			mp_Reader->CreateErrorObject(GroupName, WARNING_LEVEL, WarningCode, mp_Owner->mp_Name->m_strText, mp_Owner->mp_Rect);
		return TRUE;
	}
// 	if(fabs(DeltaX-m_Width) < m_LinkTol && fabs(DeltaY-m_Depth) < m_LinkTol)
// 	{
// 		GM2DLineVector TempLVec(MinX, MinY, MaxX, MinY);
// 		GM2DGeometry* pCurve = FindAndAddEdgeOnLVec(&TempLVec);
// 		TempLVec.m_P0.SetXY(MaxX, MinY);
// 		TempLVec.m_P1.SetXY(MaxX, MaxY);
// 		pCurve = FindAndAddEdgeOnLVec(&TempLVec);
// 		TempLVec.m_P0.SetXY(MaxX, MaxY);
// 		TempLVec.m_P1.SetXY(MinX, MaxY);
// 		pCurve = FindAndAddEdgeOnLVec(&TempLVec);
// 		TempLVec.m_P0.SetXY(MinX, MaxY);
// 		TempLVec.m_P1.SetXY(MinX, MinY);
// 		pCurve = FindAndAddEdgeOnLVec(&TempLVec);
// 
// 		return TRUE;
// 	}
// 
// 	return FALSE;
}

/*
BOOL SRSectionBox::FindEdgesOn0Dim()
{
	CTypedPtrArray<CObArray, GM2DLineVector*> BoxLVecArr;
	mp_Rect->GetEdges(BoxLVecArr);

	GM2DLineVector* pBotLVec = BoxLVecArr[0];
	GM2DLineVector* pRightLVec = BoxLVecArr[1];

	CTypedPtrArray<CObArray, GM2DLineVector*> HoriLVecArr;
	CTypedPtrArray<CObArray, GM2DLineVector*> VertLVecArr;
	CTypedPtrArray<CObArray, GMArc*> ArcArr;

	long nCurve = ma_Curve.GetSize();
	for(long iLine=0; iLine<nCurve; iLine++)
	{
		GM2DGeometry* pCurve = ma_Curve[iLine];
		if(pCurve->m_ObjType != GT_LINE) 
		{
			ArcArr.Add((GMArc*)pCurve);
			continue;
		}
		GM2DLineVector* pLine = (GM2DLineVector*)pCurve;
		if(pLine->IsHorizontalLine())
		{
			long nLine = HoriLVecArr.GetSize();
			for(long jLine=0; jLine<nLine; jLine++)
			{
				if(pLine->m_P0.m_Y < HoriLVecArr[jLine]->m_P0.m_Y)
				{
					HoriLVecArr.InsertAt(jLine, pLine);
					break;
				}
			}
			if(jLine==nLine)
				HoriLVecArr.Add(pLine);
		}
		else if(pLine->IsVerticalLine())
		{
			long nLine = VertLVecArr.GetSize();
			for(long jLine=0; jLine<nLine; jLine++)
			{
				if(pLine->m_P0.m_X < VertLVecArr[jLine]->m_P0.m_X)
				{
					VertLVecArr.InsertAt(jLine, pLine);
					break;
				}
			}
			if(jLine==nLine)
				VertLVecArr.Add(pLine);
		}
	}
	if(VertLVecArr.GetSize()==0 || HoriLVecArr.GetSize()==0)
		return FALSE;
	double MinX = VertLVecArr[0]->m_P0.m_X;
	double MaxX = VertLVecArr[VertLVecArr.GetSize()-1]->m_P0.m_X;
	double MinY = HoriLVecArr[0]->m_P0.m_Y;
	double MaxY = HoriLVecArr[HoriLVecArr.GetSize()-1]->m_P0.m_Y;
	double DeltaX = MaxX - MinX;
	double DeltaY = MaxY - MinY;

 	if(!(m_Width > 100. && m_Depth > 100.))
 		return FALSE;

	if(fabs(DeltaX-m_Width) < m_LinkTol && fabs(DeltaY-m_Depth) < m_LinkTol)
	{
		GM2DLineVector TempLVec(MinX, MinY, MaxX, MinY);
		GM2DGeometry* pCurve = FindAndAddEdgeOnLVec(&TempLVec);
		TempLVec.m_P0.SetXY(MaxX, MinY);
		TempLVec.m_P1.SetXY(MaxX, MaxY);
		pCurve = FindAndAddEdgeOnLVec(&TempLVec);
		TempLVec.m_P0.SetXY(MaxX, MaxY);
		TempLVec.m_P1.SetXY(MinX, MaxY);
		pCurve = FindAndAddEdgeOnLVec(&TempLVec);
		TempLVec.m_P0.SetXY(MinX, MaxY);
		TempLVec.m_P1.SetXY(MinX, MinY);
		pCurve = FindAndAddEdgeOnLVec(&TempLVec);

		return true;
	}

	return false;
}



*/
BOOL SRSectionBox::FindEdgesOn2Dim()
{
	SRDimension* pHoriDim=NULL, *pVertDim=NULL;

	SRDimension* pFirstDim = ma_SRDim[0];
	SRDimension* pSecondDimm = ma_SRDim[1];
	auto dirVec1 = ma_SRDim[0]->mp_LVec->DirVector();
	auto dirVec2 = ma_SRDim[1]->mp_LVec->DirVector();
 	double DotProduct = ma_SRDim[0]->mp_LVec->DirVector() * ma_SRDim[1]->mp_LVec->DirVector();
 	if(fabs(DotProduct) > (1 * Deg2Rad))  // 수평/수직 디맨전 2개 아닌 경우 실패
 		return FALSE;
	
	if(fabs(sin(ma_SRDim[0]->m_Radian)) < DTOL_GM)
	{
		pHoriDim = ma_SRDim[0];
		pVertDim = ma_SRDim[1];
	}
	else
	{
		pHoriDim = ma_SRDim[1];
		pVertDim = ma_SRDim[0];
	}

	double MinX = min(pHoriDim->mp_LVec->m_P0.m_X, pHoriDim->mp_LVec->m_P1.m_X);
	double MaxX = max(pHoriDim->mp_LVec->m_P0.m_X, pHoriDim->mp_LVec->m_P1.m_X);
	double MinY = min(pVertDim->mp_LVec->m_P0.m_Y, pVertDim->mp_LVec->m_P1.m_Y);
	double MaxY = max(pVertDim->mp_LVec->m_P0.m_Y, pVertDim->mp_LVec->m_P1.m_Y);
	GM2DLineVector TempLVec(MinX, MinY, MaxX, MinY);
	GM2DGeometry* pCurve = FindAndAddEdgeOnLVec(&TempLVec);
	TempLVec.m_P0.SetXY(MaxX, MinY);
	TempLVec.m_P1.SetXY(MaxX, MaxY);
	pCurve = FindAndAddEdgeOnLVec(&TempLVec);
	TempLVec.m_P0.SetXY(MaxX, MaxY);
	TempLVec.m_P1.SetXY(MinX, MaxY);
	pCurve = FindAndAddEdgeOnLVec(&TempLVec);
	TempLVec.m_P0.SetXY(MinX, MaxY);
	TempLVec.m_P1.SetXY(MinX, MinY);
	pCurve = FindAndAddEdgeOnLVec(&TempLVec);

	return TRUE;
}

GM2DGeometry* SRSectionBox::FindAndAddEdgeOnLVec(GM2DLineVector* pLVec)
{
	GMArc* pArc;
	GM2DLineVector* pLine=NULL;
	long nCurve = ma_Curve.GetSize();
	double dTol = m_LinkTol;
	double minLength = 10;
	dTol = 15;	// 김은석 과장 요청
	dTol = dTol / GetScaleFactorFromDimension();	// BH-3778
	for(long iLine=0; iLine<nCurve; iLine++)
	{
		GM2DGeometry* pCurve = ma_Curve[iLine];
		if(pCurve->m_ObjType == GT_LINE) 
		{
			pLine = (GM2DLineVector*)pCurve;
			if (pLine->Length() < minLength)	continue;
			if(pLine->IsColinear4DTol(*pLVec, 1., dTol) == 0) continue;

			double UrLoc0 = pLVec->OrthoBaseLocValue(pLine->m_P0);
			double UrLoc1 = pLVec->OrthoBaseLocValue(pLine->m_P1);
			long Result = pLVec->ProjectGroup(UrLoc0, UrLoc1, 0.01);
			if(Result==1 || Result==2 || Result==6 || Result==7 || Result==19 || Result==20 || Result==24 || Result==25)
				continue;

			pLine->m_P0 = pLVec->m_P0;
			pLine->m_P1 = pLVec->m_P1;
			
			AddSectionLine(pLine);
			return pLine;
		}
		else
		{
			pArc = (GMArc*)pCurve;
			GM2DVector P0 = pArc->GetStart2DVec();
			double Dist = pLVec->Distance(P0);
			if(Dist > m_LinkTol) continue;
			GM2DVector P1 = pArc->GetEnd2DVec();
			Dist = pLVec->Distance(P1);
			if(Dist > m_LinkTol) continue;

			double UrLoc0 = pLVec->OrthoBaseLocValue(P0);
			double UrLoc1 = pLVec->OrthoBaseLocValue(P1);
			long Result = pLVec->ProjectGroup(UrLoc0, UrLoc1, 0.01);
			if(!(Result==9 || Result==17))
				continue;

			AddSectionLine(pArc);
			return pArc;
		}
	}

	CTypedPtrArray<CObArray, GM2DLineVector*>* LineArr;
	if(pLVec->IsHorizontalLine())
		LineArr = &ma_HoriLVec;
	else
		LineArr = &ma_VertLVec;

	GM2DLineVector* pMinDistLVec=NULL;
	double MinDist = 1.E10;
	long nLine = LineArr->GetSize();
	for(long iLine=0; iLine<nLine; iLine++)
	{
		GM2DLineVector* pLine = LineArr->GetAt(iLine);
		double Dist = pLine->Distance(pLVec->m_P0);
		if(Dist<MinDist)
		{
			pMinDistLVec = pLine;
			MinDist = Dist;
		}
	}
	if(MinDist < pLVec->Length()/10. && pMinDistLVec)
	{
		pMinDistLVec->m_P0 = pLVec->m_P0;
		pMinDistLVec->m_P1 = pLVec->m_P1;
//        ASSERT(0);
		AddSectionLine(pMinDistLVec);
		return pMinDistLVec;
	}

	return NULL;
 }

bool SRSectionBox::FindDimEdgeOnPoint(SRDimension* pDim, GM2DVector& OnVec)
{
	return false;
}

bool SRSectionBox::FindEdgesBy3MoreDim()
{
	long nDim = ma_SRDim.GetSize();
	for(long iDim=0; iDim<nDim; iDim++)
	{
		SRDimension* pDim = ma_SRDim[iDim];
		if(!FindEdgesByDim(pDim))
			return false;
	}

	long nSectionLine=ma_OutLine.GetSize();
	for(long iLine=0; iLine<nSectionLine; iLine++)
	{
		GM2DLineVector* pSectionLine = (GM2DLineVector*)ma_OutLine[iLine];
		FindLinkedSectionLine(&pSectionLine->m_P0, true);
		FindLinkedSectionLine(&pSectionLine->m_P1, false);
	}

	return true;
}

bool SRSectionBox::FindEdgesByDim(SRDimension* pDim)
{
	pDim->mp_EdgeLVec = NULL;
	GM2DLineVector* pDimLVec = pDim->mp_LVec;
	double MinDist2Edge = 1.E10;
	GM2DLineVector* pMinDistLVec = NULL;

	long nCurve = ma_Curve.GetSize();
	for(long iLine=0; iLine<nCurve; iLine++)
	{
		GM2DGeometry* pCurve = ma_Curve[iLine];
		if(pCurve->m_ObjType != GT_LINE) 
			continue;
		GM2DLineVector* pLine = (GM2DLineVector*)pCurve;
		if(pLine->IsParallel4DTol(*pDimLVec) == 0) continue;
		if( pDimLVec->WhichSide(pLine->m_P0) * pDim->m_EdgePosition == -1) continue;

		double UrLoc0 = pDim->mp_LVec->OrthoBaseLocValue(pLine->m_P0);
		double UrLoc1 = pDim->mp_LVec->OrthoBaseLocValue(pLine->m_P1);
		long Result = pDim->mp_LVec->ProjectGroup(UrLoc0, UrLoc1, 0.1);
// 		if(Result==1 || Result==2 || Result==6 || Result==7 || Result==19 || Result==20 || Result==24 || Result==25)
// 			continue;
		if(!(Result==9 || Result==17))
			continue;

		double Dist = pDimLVec->Distance(pLine->m_P0);
		if(Dist<MinDist2Edge)
		{
			pMinDistLVec = pLine;
			MinDist2Edge = Dist;
		}
	}

	if(pMinDistLVec)
	{
		if(pMinDistLVec->WhichSide(pDimLVec->m_P0)==1)
			pMinDistLVec->ReverseDirection();
		bool bAdded = AddSectionLine(pMinDistLVec);
		pDim->mp_EdgeLVec = pMinDistLVec;
	}

	if(pDim->mp_EdgeLVec)
		return true;

	return false;
} 


/*
bool SRSectionBox::FindEdgesByDim(SRDimension* pDim)
{
	pDim->mp_EdgeLVec = NULL;
	GM2DLineVector* pDimLVec = pDim->mp_LVec;
	double MinDist2Edge = 1.E10;
	GM2DLineVector* pMinDistLVec = NULL;
	double MaxDist2Edge = 0.;
	GM2DLineVector* pMaxDistLVec = NULL;
	double MinLoc0, MinLoc1, MaxLoc0, MaxLoc1;
	long MinResult, MaxResult;

	long nCurve = ma_Curve.GetSize();
	for(long iLine=0; iLine<nCurve; iLine++)
	{
		GM2DGeometry* pCurve = ma_Curve[iLine];
		if(pCurve->m_ObjType != GT_LINE) 
			continue;
		GM2DLineVector* pLine = (GM2DLineVector*)pCurve;
		if(pLine->IsParallel4DTol(*pDimLVec) == 0) continue;
		if(pDimLVec->WhichSide(pLine->m_P0) * pDim->m_EdgePosition == -1) continue;

		double UrLoc0 = pDim->mp_LVec->OrthoBaseLocValue(pLine->m_P0);
		double UrLoc1 = pDim->mp_LVec->OrthoBaseLocValue(pLine->m_P1);
		long Result = pDim->mp_LVec->ProjectGroup(UrLoc0, UrLoc1, 0.01);
		if(Result==1 || Result==2 || Result==6 || Result==7 || Result==19 || Result==20 || Result==24 || Result==25)
			continue;

		double Dist = pDimLVec->Distance(pLine->m_P0);
		if(Dist<MinDist2Edge)
		{
			pMinDistLVec = pLine;
			MinDist2Edge = Dist;
			MinLoc0 = UrLoc0;
			MinLoc1 = UrLoc1;
			MinResult = Result;
		}
		if(Dist>MaxDist2Edge)
		{
			pMaxDistLVec = pLine;
			MaxDist2Edge = Dist;
			MaxLoc0 = UrLoc0;
			MaxLoc1 = UrLoc1;
			MaxResult = Result;
		}
	}

	if(pMinDistLVec)
	{
		bool bAdded = AddSectionLine(pMinDistLVec);
		if(MinResult==9 || MinResult==17 || !(MaxResult==9 || MaxResult==17))
			pDim->mp_EdgeLVec = pMinDistLVec;
	}

	if(pMaxDistLVec)
	{
		AddSectionLine(pMaxDistLVec);
		if((MaxResult==9 || MaxResult==17) && pDim->mp_EdgeLVec==NULL)
			pDim->mp_EdgeLVec = pMaxDistLVec;
	}

	if(pDim->mp_EdgeLVec)
		return true;

	return false;
} 

*/
long SRSectionBox::BuildSectionPolygon()
{
	CTypedPtrArray<CObArray, MREdge*> TempEdgeArr;
	if(mp_Section==NULL)
		mp_Section = new MRRcSection();

	long nEdge = ma_OutLine.GetSize();
	if(nEdge==0/* && mp_SectCircle!=NULL*/)
		return MSRcLineMemberM::CIRCLE;

	if(nEdge<3) return 0;

	for(long iEdge=0; iEdge<nEdge; iEdge++)
		TempEdgeArr.Add(new MREdge(ma_OutLine[iEdge]));

	for(long iEdge=0; iEdge<nEdge-1; iEdge++)
	{
		MREdge* pMyEdge = TempEdgeArr[iEdge];
		for(long jLine=iEdge+1; jLine<nEdge; jLine++)
		{
			MREdge* pUrEdge = TempEdgeArr[jLine];
			if(pMyEdge->m_P0.Distance(pUrEdge->m_P0) < m_LinkTol)
			{
				if(pMyEdge->mp_Linked0 || pUrEdge->mp_Linked0)
					return 0;
				pMyEdge->mp_Linked0 = pUrEdge;
				pMyEdge->m_Link0 = 0;
				pUrEdge->mp_Linked0 = pMyEdge;
				pUrEdge->m_Link0 = 0;
			}
			else if(pMyEdge->m_P0.Distance(pUrEdge->m_P1) < m_LinkTol)
			{
				if(pMyEdge->mp_Linked0 || pUrEdge->mp_Linked1)
					return 0;
				pMyEdge->mp_Linked0 = pUrEdge;
				pMyEdge->m_Link0 = 1;
				pUrEdge->mp_Linked1 = pMyEdge;
				pUrEdge->m_Link1 = 0;
			}
			else if(pMyEdge->m_P1.Distance(pUrEdge->m_P0) < m_LinkTol)
			{
				if(pMyEdge->mp_Linked1 || pUrEdge->mp_Linked0)
					return 0;
				pMyEdge->mp_Linked1 = pUrEdge;
				pMyEdge->m_Link1 = 0;
				pUrEdge->mp_Linked0 = pMyEdge;
				pUrEdge->m_Link0 = 1;
			}
			else if(pMyEdge->m_P1.Distance(pUrEdge->m_P1) < m_LinkTol)
			{
				if(pMyEdge->mp_Linked1 || pUrEdge->mp_Linked1)
					return 0;
				pMyEdge->mp_Linked1 = pUrEdge;
				pMyEdge->m_Link1 = 1;
				pUrEdge->mp_Linked1 = pMyEdge;
				pUrEdge->m_Link1 = 1;
			}
		}
	}

	MREdge* pCurEdge;
	CTypedPtrArray<CObArray, MREdge*> Linked0Arr;
	CTypedPtrArray<CObArray, MREdge*> Linked1Arr;
	CTypedPtrArray<CObArray, MREdge*> Linked2Arr;
	for(long iEdge=0; iEdge<nEdge; iEdge++)
	{
		pCurEdge = TempEdgeArr[iEdge];
		if(pCurEdge->mp_Linked0==NULL && pCurEdge->mp_Linked1==NULL)
			Linked0Arr.Add(pCurEdge);
		else if(pCurEdge->mp_Linked0 && pCurEdge->mp_Linked1)
			Linked2Arr.Add(pCurEdge);
		else
			Linked1Arr.Add(pCurEdge);
	}

	long nLinked0 = Linked0Arr.GetSize();
	long nLinked1 = Linked1Arr.GetSize();
	long nLinked2 = Linked2Arr.GetSize();

	if(nLinked2==nEdge)  // 모든 에지가 연결되어 있으므로 폴리곤 완성
	{
		MREdge* pCurEdge = Linked2Arr[0];
		long CurLinkEnd = 0;

		do 
		{
			mp_Section->ma_Edge.Add(pCurEdge);
			long NextLinkEnd = CurLinkEnd == 0 ? pCurEdge->m_Link1 : pCurEdge->m_Link0;
			pCurEdge = CurLinkEnd == 0 ? pCurEdge->mp_Linked1 : pCurEdge->mp_Linked0;
			CurLinkEnd = NextLinkEnd;
		} while (pCurEdge!=Linked2Arr[0]);

		GM2DPolygon SecPGon(TRUE);
		GetSectionPolygon(SecPGon);
		if(SecPGon.IsRectangle())
			return MSRcLineMemberM::RECT;

		return MSRcLineMemberM::POLYGON;
	}

	return 0;
}

bool SRSectionBox::AddSectionLine(GM2DGeometry* pSectionLine)
{
	long nSectionLine = ma_OutLine.GetSize();
	for(long iSectionLine=0; iSectionLine<nSectionLine; iSectionLine++)
	{
		if(ma_OutLine[iSectionLine]==pSectionLine)
			return false;
	}
	ma_OutLine.Add(pSectionLine);
	pSectionLine->m_Tag = 2;
	RemoveCurve(pSectionLine);
	return true;
}

void SRSectionBox::RemoveCurve(GM2DGeometry* pCurve)
{
	long nCurve = ma_Curve.GetSize();
	for(long iCurve=0; iCurve<nCurve; iCurve++)
	{
		if(ma_Curve[iCurve]==pCurve)
		{
			ma_Curve.RemoveAt(iCurve);
			return;
		}
	}
}

BOOL SRSectionBox::FindEdgesOn1Dim()
{
	return true;
}

long SRSectionBox::FindEdges_ColumnH()
{
	if (m_Shape == MSRcLineMemberM::RECT && mp_Section != nullptr)
		return m_Shape;
	if (m_Shape == MSRcLineMemberM::CIRCLE && mp_Section != nullptr && mp_SectCircle != nullptr)
		return m_Shape;

	long nEdge = ma_OutLine.GetSize();
	if(nEdge>0)  // 섹션 외곽선이 레이어 구분되어 읽힌 경우이므로 바로 리턴
	{
		BuildSectionPolygon();
		return TRUE;
	}

	long nDim = ma_SRDim.GetSize();

	if(nDim==0)
	{
		if(!FindEdgesOn0Dim())
			return FALSE;
	}
	else if(nDim==1)
	{
		if(!FindEdgesOn1Dim())
			return FALSE;
	}
	else if(nDim==2)
	{
		if(!FindEdgesOn2Dim())
			return FALSE;
	}
	else
	{
		if(!FindEdgesBy3MoreDim())
			return FALSE;
	}

	return BuildSectionPolygon();
}

static void MoveGMCircles(SRCircleKind* pCircleKind, CTypedPtrArray<CObArray, GMCircle*>& ToArr)
{
	long nObj = pCircleKind->ma_GM.GetSize();
 	for(long iObj=0; iObj<nObj; iObj++)
		ToArr.Add(pCircleKind->ma_GM[iObj]);
}

static bool IsAuxCircle(SRCircleKind* pVBarArr, SRCircleKind* pAuxArr)
{
	GMCircle* pAuxCircle = pAuxArr->ma_GM[0];
	if(pAuxCircle->m_A <= pVBarArr->ma_GM[0]->m_A)
		return FALSE;

	double DistTol = pVBarArr->ma_GM[0]->m_A / 10.;
	long nObj = pVBarArr->ma_GM.GetSize();
	for(long iObj=0; iObj<nObj; iObj++)
	{
		GMCircle* pVBarCircle = pVBarArr->ma_GM[iObj];
		if(pVBarCircle->m_Center.Distance(pAuxCircle->m_Center) < DistTol)
			break;
	}
	if(iObj==nObj) 
		return false;

	return true;
}

static bool IsAuxCircle(CTypedPtrArray<CObArray, GMCircle*>& pVBarArr, SRCircleKind* pAuxArr)
{
	if(pAuxArr->ma_GM.GetSize() == 0 || pVBarArr.GetSize() == 0)
		return false;

	GMCircle* pAuxCircle = pAuxArr->ma_GM[0];
	if(pAuxCircle->m_A <= pVBarArr[0]->m_A)
		return FALSE;

	double DistTol = pVBarArr[0]->m_A / 10.;
	long nObj = pVBarArr.GetSize();
	for(long iObj=0; iObj<nObj; iObj++)
	{
		GMCircle* pVBarCircle = pVBarArr[iObj];
		if(pVBarCircle->m_Center.Distance(pAuxCircle->m_Center) < DistTol)
			break;
	}
	if(iObj==nObj) 
		return false;

	return true;
}

void SRSectionBox::DeleteSRCircleKindArr(CTypedPtrArray<CObArray, SRCircleKind*>& KindArr)
{
	long nKind = KindArr.GetSize();
	for(long iKind=0; iKind<nKind; iKind++)
		delete KindArr[iKind];
	KindArr.RemoveAll();
}

long SRSectionBox::AnalyzeCrossBars()
{
	if(m_VBarDia < DTOL1)
		return 0;

	if(mp_Reader->m_Group == GROUP_COLUMN)
		return 0;
	CTypedPtrArray<CObArray, GM2DLineVector*> CrossLineArr;
	long nCurve = ma_Curve.GetSize();
	for(long iCurve=nCurve-1; iCurve>=0; iCurve--)
	{
		GM2DGeometry* pCurve = ma_Curve[iCurve];
		if(pCurve->m_ObjType != GT_LINE)
			continue;

		GM2DLineVector* pLine = (GM2DLineVector*)pCurve;
		if(pLine->Length() < m_VBarDia*4.)
		{
			CrossLineArr.Add(pLine);
			ma_Curve.RemoveAt(iCurve);
		}
	}

	if(CrossLineArr.GetSize()==0)
		return 0;

	double CrossLineLength = CrossLineArr[0]->Length();
	long nLine = CrossLineArr.GetSize();

	for(long iLine=0; iLine<nLine; iLine++)
	{
		GM2DLineVector* pLineI = CrossLineArr[iLine];
		for(long jLine=iLine+1; jLine<nLine; jLine++)
		{
			GM2DLineVector* pLineJ = CrossLineArr[jLine];
			if(pLineI->m_P0.Distance(pLineJ->m_P0) > CrossLineLength)
				continue;
			double MyLoc, UrLoc;
			GM2DVector CrossVec;
			long Result = pLineI->Intersect(*pLineJ, CrossVec, &MyLoc, &UrLoc);
			if(Result == 0 && fabs(MyLoc-0.5)<0.1 && fabs(UrLoc-0.5)<0.1)
			{
				CrossLineArr.RemoveAt(jLine);
				CrossLineArr.RemoveAt(iLine);
				nLine -= 2;
				iLine--;
				GMCircle* pSideBar = new GMCircle(CrossLineLength/2., CrossVec.m_X, CrossVec.m_Y);
				ma_SideVBarCircle.Add(pSideBar);
				break;
			}
		}
	}

	return ma_SideVBarCircle.GetSize();
}


BOOL SRSectionBox::AnalyzeCircles()
{
	if (m_IsMainBarShapeByLayer)
	{
		if (ma_MainVBarCircle.GetSize() > 0)
		{
			m_VBarDia = ma_MainVBarCircle[0]->m_A;

			if (m_Shape == MSRcLineMemberM::CIRCLE)
			{
				double edgeRadius = mp_SectCircle->m_A;
				double minDist = GMObject::ms_MaxSafeDBLValue;
				GMCircle* strpCircle = nullptr;
				long nCircle = ma_Circle.GetSize();
				for (long iCircle = 0; iCircle < nCircle; iCircle++)
				{
					GMCircle* pCircle = ma_Circle[iCircle];
					if (pCircle->m_Tag == 1)	continue;// fromPolyine
					if(pCircle == mp_SectCircle)	continue;
					if (edgeRadius < pCircle->m_A+0.1)	continue;

					double dist = edgeRadius - pCircle->m_A;
					if(dist < minDist)
					{
						minDist = dist;
						strpCircle = pCircle;
					}
				}
				if (strpCircle != nullptr)
					ma_StrpCircle.Add(strpCircle);
			}
			return TRUE;
		}
	}

 	long nCircle = ma_Circle.GetSize();
	if(nCircle==0) 
	{
		m_VBarDia = 25.;
		if (AnalyzeCrossBars() > 0)
		{
			int nCircle = ma_SideVBarCircle.GetSize();
			for (int i = nCircle - 1; i >= 0; i--)
				ma_MainVBarCircle.Add(ma_SideVBarCircle[i]);

			if (nCircle != 0)
				m_VBarDia = ma_MainVBarCircle[0]->m_A;
			ma_SideVBarCircle.RemoveAll();
			return TRUE;
		}
      m_VBarDia = -1;
	  return FALSE;
	}

	GMCircle* pCircle;
	map<int, int> radiusToNumberMap;
	for (long iCircle = 0; iCircle < nCircle; iCircle++)
	{
		pCircle = ma_Circle[iCircle];
		if(pCircle->m_Tag == 1)	continue;// fromPolyine

		int radius = ROUND(pCircle->m_A);
		auto it = radiusToNumberMap.find(radius);
		if (it != radiusToNumberMap.end())	it->second++;
		else                                                      
			radiusToNumberMap.insert(make_pair(radius, 1));
	}

	if (radiusToNumberMap.size() > 0)
	{
		int radius = radiusToNumberMap.begin()->first;
		for (long iCircle = 0; iCircle < nCircle; iCircle++)
		{
			pCircle = ma_Circle[iCircle];
			if (pCircle->m_Tag == 1)	pCircle->m_A = radius;
		}
	}

	CTypedPtrArray<CObArray, SRCircleKind*> KindArr;
    double tolerance = 1.0;
	for(long iCircle=0; iCircle<nCircle; iCircle++)
	{
		pCircle = ma_Circle[iCircle];
		long nKind = KindArr.GetSize();
		for(long iKind=0; iKind<nKind; iKind++)
		{
			if(fabs(KindArr[iKind]->m_Radius - pCircle->m_A) < tolerance)
			{
				KindArr[iKind]->ma_GM.Add(pCircle);
				break;
			}
		}
		if(iKind == nKind)
		{
			SRCircleKind* pKind = new SRCircleKind(pCircle);
			KindArr.Add(pKind);
		}
	}

	if(KindArr.GetSize()==1)
	{
		MoveGMCircles(KindArr[0], ma_MainVBarCircle);
		m_VBarDia = ma_MainVBarCircle[0]->m_A;
		AnalyzeCrossBars();
		DeleteSRCircleKindArr(KindArr);
		return TRUE;
	}

	long nKind = KindArr.GetSize();
	for(long iKind=0; iKind<nKind; iKind++)
	{
		SRCircleKind* pTempKind;
		long MaxNumBar=KindArr[iKind]->ma_GM.GetSize();
		long MaxIdx=iKind;
		double MaxRadius = KindArr[iKind]->ma_GM[0]->m_A;
		for(long jKind=iKind; jKind<nKind; jKind++)
		{
			if(KindArr[jKind]->ma_GM.GetSize() > MaxNumBar ||
				(KindArr[jKind]->ma_GM.GetSize() == MaxNumBar && KindArr[jKind]->ma_GM[0]->m_A<MaxRadius))
			{
				MaxIdx = jKind;
				MaxNumBar = KindArr[jKind]->ma_GM.GetSize();
			}
		}
		if(MaxIdx == iKind)
			continue;
		pTempKind = KindArr[iKind];
		KindArr[iKind] = KindArr[MaxIdx];
		KindArr[MaxIdx] = pTempKind;
	}

	if(nKind == 0)
		return FALSE;
	m_VBarDia = KindArr[0]->ma_GM[0]->m_A;

	if(m_Shape == MSRcLineMemberM::CIRCLE)
	{
		if(nKind < 2)
		{
			DeleteSRCircleKindArr(KindArr);
			return FALSE;
		}

		if(KindArr[nKind-1]->ma_GM.GetSize()==1 && KindArr[nKind-1]->ma_GM[0]->m_A > 10.*m_VBarDia)
		{
			if (mp_SectCircle == nullptr)
			{
				mp_SectCircle = KindArr[nKind - 1]->ma_GM[0];
				GMCircle* pEdgeCircle = new GMCircle(*mp_SectCircle);
				mp_Section->ma_Edge.Add(new MREdge(pEdgeCircle));
			}
		}
		else
		{
			DeleteSRCircleKindArr(KindArr);
			return FALSE;
		}

		long nStrpCircle=0;
		for(long iKind=nKind-2; iKind>=0; iKind--)
		{
			if(KindArr[iKind]->ma_GM.GetSize()==1 && KindArr[iKind]->ma_GM[0]->m_A > 10.*m_VBarDia)
			{
				ma_StrpCircle.Add(KindArr[iKind]->ma_GM[0]);
				nStrpCircle++;
			}
		}
		nKind -= (1+nStrpCircle);

		for(long iKind=nKind-1; iKind>=0; iKind--)
		{
			SRCircleKind* pKind = KindArr[iKind];
			long nCircle = pKind->ma_GM.GetSize();
			for(long iCircle=nCircle-1; iCircle>=0; iCircle--)
			{
				if(mp_SectCircle->Contains(pKind->ma_GM[iCircle]->m_Center) >= 0)
					pKind->ma_GM.RemoveAt(iCircle);
			}
			if(pKind->ma_GM.GetSize()==0)
			{
				KindArr.RemoveAt(iKind);
				nKind--;
			}
		}
	}
	else
	{
		GM2DPolygon SecPGon(TRUE);
		GetSectionPolygon(SecPGon);

		for(long iKind=nKind-1; iKind>=0; iKind--)
		{
			SRCircleKind* pKind = KindArr[iKind];
			long nCircle = pKind->ma_GM.GetSize();
			for(long iCircle=nCircle-1; iCircle>=0; iCircle--)
			{
				pCircle = pKind->ma_GM[iCircle];
				GM2DVector vCenter(pCircle->m_Center.m_X, pCircle->m_Center.m_Y);
				if(SecPGon.ContainsEx(vCenter)>=0) 
					pKind->ma_GM.RemoveAt(iCircle);
			}
			if(pKind->ma_GM.GetSize()==0)
			{
				KindArr.RemoveAt(iKind);
				nKind--;
			}
		}
		mp_SectCircle = NULL;
		AnalyzeCrossBars();
	}

	if(nKind>0)
		MoveGMCircles(KindArr[0], ma_MainVBarCircle);
	if(nKind==1)
	{
		DeleteSRCircleKindArr(KindArr);
		return TRUE;
	}
	else if(nKind == 2)
	{
		if(IsAuxCircle(KindArr[0], KindArr[1]) || IsAuxCircle(ma_SideVBarCircle, KindArr[1]))
			MoveGMCircles(KindArr[1], ma_AuxCircle);
		else
			MoveGMCircles(KindArr[1], ma_SubVBarCircle);
		DeleteSRCircleKindArr(KindArr);
		return TRUE;
	}
	else if(nKind >= 3)
	{
		if(IsAuxCircle(KindArr[0], KindArr[1]) || 
			IsAuxCircle(KindArr[2], KindArr[1]))
		{
			MoveGMCircles(KindArr[1], ma_AuxCircle);
			MoveGMCircles(KindArr[2], ma_SubVBarCircle);
		}
		else if(IsAuxCircle(KindArr[0], KindArr[2]) || 
			IsAuxCircle(KindArr[1], KindArr[2]))
		{
			MoveGMCircles(KindArr[2], ma_AuxCircle);
			MoveGMCircles(KindArr[1], ma_SubVBarCircle);
		}
		else if(IsAuxCircle(ma_SideVBarCircle, KindArr[1]))
			MoveGMCircles(KindArr[1], ma_AuxCircle);
		else if(IsAuxCircle(ma_SideVBarCircle, KindArr[2]))
			MoveGMCircles(KindArr[2], ma_AuxCircle);
		else
		{
			DeleteSRCircleKindArr(KindArr);
			return TRUE;
		}
		DeleteSRCircleKindArr(KindArr);
		return TRUE;
	}

	DeleteSRCircleKindArr(KindArr);
	m_VBarDia = -1.;
	return FALSE;
}

BOOL SRSectionBox::FindMainBars_ColumnH(long Type)
{
	CTypedPtrArray<CObArray, GMCircle*>* VBarCircleArr;
	if(Type == 1)
		VBarCircleArr = &ma_MainVBarCircle;
	else if(Type == 2)
		VBarCircleArr = &ma_SubVBarCircle;
	else if(Type == 3)
		VBarCircleArr = &ma_SideVBarCircle;
	else
		return FALSE;

	if(m_Shape != MSRcLineMemberM::CIRCLE)
	{
		GM2DPolygon SecPGon(TRUE);
		GetSectionPolygon(SecPGon);

		long nCircle = VBarCircleArr->GetSize();
		GMCircle* pCircle;
		for(long iCircle=0; iCircle<nCircle; iCircle++)
		{
			pCircle = (*VBarCircleArr)[iCircle];
 			GM2DVector vCenter(pCircle->m_Center.m_X, pCircle->m_Center.m_Y);

			double Dist, MinDist = 1.E10;
			MREdge* pClosestEdge = NULL;
			long nEdge = mp_Section->ma_Edge.GetSize();
			for(long iEdge=0; iEdge<nEdge; iEdge++)
			{
				GM2DGeometry* pCurve = mp_Section->ma_Edge[iEdge]->mp_Curve;
				if(pCurve->m_ObjType == GT_LINE)
				{
					GM2DLineVector* pLine = (GM2DLineVector*)pCurve;
					if(pLine->WhichSide(vCenter)!=1)
						continue;
					Dist = pLine->Distance(vCenter);
					if(Dist < MinDist)
					{
						MinDist = Dist;
						pClosestEdge = mp_Section->ma_Edge[iEdge];
					}
				}
			}
			if(pClosestEdge)
				pClosestEdge->AddVBarLayer(MinDist, m_VBarDia);
		}

		// 콩이 총 4개인 경우, 가장 가까운 거리를 찾아서 Edge에 ma_VBarSet을 추가하면, 무작위로 한쪽의 ma_VBarSet이 '0'인 경우가 생기는 오류가 존재
		// 각 Edge의 ma_VBarSet을 검사하여, 갯수가 '0'인 Edge의 ma_VBarSet을 추가한다.
		// 위에서 넣어준 콩의 위치는 RemoveCornerVBar()함수에서 자동으로 지워진다. - by Gracias
		if(mp_Section->ma_Edge.GetSize() == 4)
		{
			long nEdge = mp_Section->ma_Edge.GetSize();
			for(int iEdge = 0; iEdge < nEdge; iEdge++)
			{
				MREdge* pEdge = mp_Section->ma_Edge[iEdge];
				if(pEdge->ma_VBarSet.GetSize() == 0)
				{
					MREdge* pNextEdge = NULL, *pPrevEdge = NULL;
					if(iEdge +1 == nEdge)
					{
						pNextEdge = mp_Section->ma_Edge[0];
						pPrevEdge = mp_Section->ma_Edge[iEdge-1];
					}
					else
					{
						pNextEdge = mp_Section->ma_Edge[iEdge+1];
						pPrevEdge = mp_Section->ma_Edge[nEdge-1];
					}
					double MinDist = 100;
					if(pNextEdge->ma_VBarSet.GetSize())
						MinDist = pNextEdge->ma_VBarSet[0]->m_LayerDistFromEdge;
					else if(pPrevEdge->ma_VBarSet.GetSize())
						MinDist = pPrevEdge->ma_VBarSet[0]->m_LayerDistFromEdge;
					else
						continue;
					pEdge->AddVBarLayer(MinDist, m_VBarDia);
				}
			}
		}
		for(long iCircle=nCircle-1; iCircle>=0; iCircle--)
		{
			pCircle = (*VBarCircleArr)[iCircle];
			GM2DVector vCenter(pCircle->m_Center.m_X, pCircle->m_Center.m_Y);
			if(SecPGon.ContainsEx(vCenter)>=0) continue;

			long nEdge = mp_Section->ma_Edge.GetSize();
			for(long iEdge=0; iEdge<nEdge; iEdge++)
				mp_Section->ma_Edge[iEdge]->AddVBarLoc(vCenter, m_VBarDia);
		}
	}
	else
	{
		if(mp_Section->ma_Edge.GetSize()!=1 || mp_Section->ma_Edge[0]==NULL)
			return FALSE;

		MREdge* pCirCleEdge = mp_Section->ma_Edge[0];
		long nCircle = VBarCircleArr->GetSize();
		GMCircle* pCircle;
		for(long iCircle=0; iCircle<nCircle; iCircle++)
		{
			pCircle = (*VBarCircleArr)[iCircle];
			pCirCleEdge->AddVBarOnCircle(pCircle->m_Center, m_VBarDia);
		}
	}

	long nBar = 0;
	long nEdge = mp_Section->ma_Edge.GetSize();
	for(long iEdge=0; iEdge<nEdge; iEdge++)
	{
		MREdge* pEdge = mp_Section->ma_Edge[iEdge];

		int EdgeBar = 0;
		long nSet = pEdge->ma_VBarSet.GetSize();
		for(long iSet=0; iSet<nSet; iSet++)
		{
			MRVBar* pMRVBar = pEdge->ma_VBarSet[iSet];
			EdgeBar += pMRVBar->ma_VBarLoc.GetSize();
		}
		nBar += EdgeBar;
	}
	if(nEdge>1)
		nBar -= nEdge;

	if(mp_MainBar1)
	{
		long nMainBar = mp_MainBar1->GetTotalBars();
		if(nBar != nMainBar)
		{
			m_nErrorCode |= SRSectionTable::WARNING_MAINBAR_NUM;
		}
	}
	else
	{
		if (mp_Reader->IsPlainConcreteReinforceBar())	return TRUE;
		CString GroupName = _T("Column");
		CString errorMessage = _T("");
		if (mp_Name)
			errorMessage.Format(_T("%s 주근"), mp_Name->m_strText);
		else if (mp_Owner->mp_Name)
			errorMessage.Format(_T("%s 주근"), mp_Owner->mp_Name->m_strText);

		mp_Reader->CreateErrorObject(GroupName, ERROR_LEVEL, ERROR_IGNORE_REBARTEXT, errorMessage, mp_Owner->mp_Rect);
		m_nErrorCode = 1;
	}


	return TRUE;
}

BOOL SRSectionBox::FindMainBars_Beam()
{
	CTypedPtrArray<CObArray, GMCircle*>* VBarCircleArr;
	VBarCircleArr = &ma_MainVBarCircle;

	long nCircle = VBarCircleArr->GetSize();
	double MaxY = 0.0;
	double MinY = 0.0;
	for(int i = 0; i < nCircle; i++)
	{
		GMCircle* pCircle = (*VBarCircleArr)[i];
		if(i == 0)
		{
			MaxY = pCircle->m_Center.m_Y;
			MinY = pCircle->m_Center.m_Y;
			continue;
		}
		if(MaxY < pCircle->m_Center.m_Y)	MaxY = pCircle->m_Center.m_Y;
		if(MinY > pCircle->m_Center.m_Y)	MinY = pCircle->m_Center.m_Y;
	}

	double distTolerance = (MaxY - MinY) / 2;
	if(mp_Section->ma_Edge.GetSize() != 4) return FALSE;
	MREdge* pBottomEdge = mp_Section->ma_Edge[0];
	MREdge* pTopEdge = mp_Section->ma_Edge[2];
	for(int i = 0; i < nCircle; i++)
	{
		GMCircle* pCircle = (*VBarCircleArr)[i];
		GM2DVector cenPt(pCircle->m_Center.m_X, pCircle->m_Center.m_Y);
		double Dist, MinDist = 1.E10;
		MREdge* pClosestEdge = NULL;

		if(fabs(cenPt.m_Y - MaxY) > fabs(cenPt.m_Y - MinY))
			pClosestEdge = pBottomEdge;
		else
			pClosestEdge = pTopEdge;

		GM2DGeometry* pCurve = pClosestEdge->mp_Curve;
		if(pCurve->m_ObjType == GT_LINE)
		{
			GM2DLineVector* pLine = (GM2DLineVector*)pCurve;
			if(pLine->WhichSide(cenPt)!=1)	continue;
			Dist = pLine->Distance(cenPt);
			pClosestEdge->AddVBarLayer(Dist, m_VBarDia);
		}
	}

	GM2DPolygon SecPGon(TRUE);
	GetSectionPolygon(SecPGon);
	for(long iCircle=nCircle-1; iCircle>=0; iCircle--)
	{
		GMCircle* pCircle = (*VBarCircleArr)[iCircle];
		GM2DVector vCenter(pCircle->m_Center.m_X, pCircle->m_Center.m_Y);
		if(SecPGon.ContainsEx(vCenter)>=0) continue;

		long nEdge = mp_Section->ma_Edge.GetSize();
		for(long iEdge=0; iEdge<nEdge; iEdge += 2)
		{
			MREdge* pEdge = mp_Section->ma_Edge[iEdge];
			pEdge->AddVBarLoc(vCenter, m_VBarDia);

			int nCount = 0;
			long nSet = pEdge->ma_VBarSet.GetSize();
			for(long iSet=0; iSet<nSet; iSet++)
			{
				MRVBar* pVBar = pEdge->ma_VBarSet[iSet];
				nCount += pVBar->ma_VBarLoc.GetSize();
			}
		}
	}

	long nTopCircle = 0, nBotCircle = 0;
	long nEdge = mp_Section->ma_Edge.GetSize();
    vector<int> topLayerToBars;
    vector<int> bottomLayerToBars;
	for(long iEdge=0; iEdge<nEdge; iEdge++)
	{
		MREdge* pEdge = mp_Section->ma_Edge[iEdge];

		long nSet = pEdge->ma_VBarSet.GetSize();
		for(long iSet=0; iSet<nSet; iSet++)
		{
			MRVBar* pVBar = pEdge->ma_VBarSet[iSet];
            int count = pVBar->ma_VBarLoc.GetSize();
            if (iEdge == 0)
            {
                nBotCircle += count;
                bottomLayerToBars.push_back(count);
            }
            else if (iEdge == 2)
            {
                nTopCircle += count;
                topLayerToBars.push_back(count);
            }
		}
	}

	if (mp_MainBarSet1)
	{
		long nTopBar = mp_MainBarSet1->GetTotalBars();
		if (nTopBar != nTopCircle)
		{
			if (mp_Reader->IsPlainConcreteReinforceBar())	return TRUE;
			mp_Reader->AddErrorFigureIDs(GetTextFigureIDs(MTReader::ctTBar));
			CString SectionName = mp_Owner->GetBeamSectionName(m_BoxNumber);
			CString GroupName = _T("Beam");
			CString errorMessage = _T("");
			if (mp_Name)
				errorMessage.Format(_T("%s - %s 상부근"), mp_Name->m_strText, SectionName);
			else if (mp_Owner->mp_Name)
				errorMessage.Format(_T("%s -%s 상부근"), mp_Owner->mp_Name->m_strText, SectionName);

			mp_Reader->CreateErrorObject(GroupName, ERROR_LEVEL, ERROR_MAINBAR_NUM, errorMessage, mp_Owner->mp_Rect);
			m_nErrorCode = 1;
			m_nWarningCode |= SRSectionTable::WARNING_MAINBAR_NUM;

            auto diaName = MSMainBarSet::GetDiaNameOnSet(m_sTopBar, 0);
            auto mainBar = makeMainBarString(diaName, topLayerToBars);
            mp_MainBarSet1->SetBarString(mainBar);
		}
	}
	else
	{
		if (mp_Reader->IsPlainConcreteReinforceBar())	return TRUE;
		CString SectionName = mp_Owner->GetBeamSectionName(m_BoxNumber);
		CString GroupName = _T("Beam");
		CString errorMessage = _T("");
		if (mp_Name)
			errorMessage.Format(_T("%s - %s 상부근"), mp_Name->m_strText, SectionName);
		else if (mp_Owner->mp_Name)
			errorMessage.Format(_T("%s -%s 상부근"), mp_Owner->mp_Name->m_strText, SectionName);

		mp_Reader->CreateErrorObject(GroupName, ERROR_LEVEL, ERROR_IGNORE_REBARTEXT, errorMessage, mp_Owner->mp_Rect);
		m_nErrorCode = 1;
	}

	if ((mp_MainBarSet2))
	{
		long nBotBar = mp_MainBarSet2->GetTotalBars();

		if (nBotBar != nBotCircle)
		{
			if (mp_Reader->IsPlainConcreteReinforceBar())	return TRUE;
			mp_Reader->AddErrorFigureIDs(GetTextFigureIDs(MTReader::ctBBar));
			CString SectionName = mp_Owner->GetBeamSectionName(m_BoxNumber);
			CString GroupName = _T("Beam");
			CString errorMessage = _T("");
			if (mp_Name)
				errorMessage.Format(_T("%s - %s 하부근"), mp_Name->m_strText, SectionName);
			else if (mp_Owner->mp_Name)
				errorMessage.Format(_T("%s -%s 하부근"), mp_Owner->mp_Name->m_strText, SectionName);

			mp_Reader->CreateErrorObject(GroupName, ERROR_LEVEL, ERROR_MAINBAR_NUM, errorMessage, mp_Owner->mp_Rect);
			m_nErrorCode = 1;
			m_nWarningCode |= SRSectionTable::WARNING_MAINBAR_NUM;

            auto diaName = MSMainBarSet::GetDiaNameOnSet(m_sBotBar, 0);
            auto mainBar = makeMainBarString(diaName, bottomLayerToBars);
            mp_MainBarSet2->SetBarString(mainBar);
        }
	}
	else
	{
		if (mp_Reader->IsPlainConcreteReinforceBar())	return TRUE;
		CString SectionName = mp_Owner->GetBeamSectionName(m_BoxNumber);
		CString GroupName = _T("Beam");
		CString errorMessage = _T("");
		if (mp_Name)
			errorMessage.Format(_T("%s - %s 하부근"), mp_Name->m_strText, SectionName);
		else if (mp_Owner->mp_Name)
			errorMessage.Format(_T("%s -%s 하부근"), mp_Owner->mp_Name->m_strText, SectionName);

		mp_Reader->CreateErrorObject(GroupName, ERROR_LEVEL, ERROR_IGNORE_REBARTEXT, errorMessage, mp_Owner->mp_Rect);
		m_nErrorCode = 1;
	}

	return TRUE;

/*
	if(m_Shape != MSRcLineMemberM::CIRCLE)
	{
		GM2DPolygon SecPGon(TRUE);
		GetSectionPolygon(SecPGon);

		long nCircle = VBarCircleArr->GetSize();
		GMCircle* pCircle;
		for(long iCircle=0; iCircle<nCircle; iCircle++)
		{
			pCircle = (*VBarCircleArr)[iCircle];
			GM2DVector vCenter(pCircle->m_Center.m_X, pCircle->m_Center.m_Y);
			//  			if(SecPGon.ContainsEx(vCenter)>=0) continue;

			double Dist, MinDist = 1.E10;
			MREdge* pClosestEdge = NULL;
			long nEdge = mp_Section->ma_Edge.GetSize();
			for(long iEdge=0; iEdge<nEdge; iEdge += 2)
			{
				GM2DGeometry* pCurve = mp_Section->ma_Edge[iEdge]->mp_Curve;
				if(pCurve->m_ObjType == GT_LINE)
				{
					GM2DLineVector* pLine = (GM2DLineVector*)pCurve;
					if(pLine->WhichSide(vCenter)!=1)
						continue;
					Dist = pLine->Distance(vCenter);
					if(Dist < MinDist)
					{
						MinDist = Dist;
						pClosestEdge = mp_Section->ma_Edge[iEdge];
					}
				}
			}
			if(pClosestEdge)
				pClosestEdge->AddVBarLayer(MinDist, m_VBarDia);
		}
		for(long iCircle=nCircle-1; iCircle>=0; iCircle--)
		{
			pCircle = (*VBarCircleArr)[iCircle];
			GM2DVector vCenter(pCircle->m_Center.m_X, pCircle->m_Center.m_Y);
			if(SecPGon.ContainsEx(vCenter)>=0) continue;

			long nEdge = mp_Section->ma_Edge.GetSize();
			for(long iEdge=0; iEdge<nEdge; iEdge += 2)
				mp_Section->ma_Edge[iEdge]->AddVBarLoc(vCenter, m_VBarDia);
		}
	}

	long nTopCircle = 0, nBotCircle = 0;
	long nEdge = mp_Section->ma_Edge.GetSize();
	for(long iEdge=0; iEdge<nEdge; iEdge++)
	{
		MREdge* pEdge = mp_Section->ma_Edge[iEdge];

		long nSet = pEdge->ma_VBarSet.GetSize();
		for(long iSet=0; iSet<nSet; iSet++)
		{
			if(iEdge == 0)
				nBotCircle += pEdge->ma_VBarSet[iSet]->ma_VBarLoc.GetSize();
			else if(iEdge == 2)
				nTopCircle += pEdge->ma_VBarSet[iSet]->ma_VBarLoc.GetSize();
		}
	}

	long nTopBar = 0;
	if(mp_MainBar1)
		nTopBar = mp_MainBar1->GetTotalBars();
	long nBotBar = 0;
	if(mp_MainBar2)
		nBotBar = mp_MainBar2->GetTotalBars();

	bool bError = false;
	if(nTopBar != nTopCircle)
	{
		mp_Reader->AddErrorFigureID(GetTextFigureID(MTReader::ctTBar));
		bError = true;
	}
	if(nBotBar != nBotCircle)
	{
		mp_Reader->AddErrorFigureID(GetTextFigureID(MTReader::ctBBar));
		bError = true;
	}
	if(bError)
	{
		CString SectionName = mp_Owner->GetBeamSectionName(m_BoxNumber);
		CString GroupName = _T("Beam");
		CString errorMessage = _T("");
		if(mp_Name)
			errorMessage.Format(_T("%s - %s"), mp_Name->m_strText, SectionName);
		else if(mp_Owner->mp_Name)
			errorMessage.Format(_T("%s -%s"), mp_Owner->mp_Name->m_strText, SectionName);

		mp_Reader->CreateErrorObject(GroupName, ERROR_LEVEL, ERROR_MAINBAR_NUM, errorMessage, mp_Owner->mp_Rect);
		m_nErrorCode = 1;
		m_nWarningCode |= SRSectionTable::WARNING_MAINBAR_NUM;
	}
	return TRUE;
*/
}
CString SRSectionBox::makeMainBarString(CString diaName, vector<int>& barData)
{
    CString fullBar = _T("");
    for(int i = 0; i < barData.size(); i++)
    {
        CString sBar;
        sBar.Format(_T("%ld-HD%s"), barData[i], diaName);
        if (i != barData.size() - 1)
            fullBar += sBar + _T(";");
        else
            fullBar += sBar;
    }
    return fullBar;
}
BOOL SRSectionBox::FindMainBars_Buttress()
{
	CTypedPtrArray<CObArray, GMCircle*>* VBarCircleArr;
	VBarCircleArr = &ma_MainVBarCircle;

	if(m_Shape != MSRcLineMemberM::CIRCLE)
	{
		GM2DPolygon SecPGon(TRUE);
		GetSectionPolygon(SecPGon);

		long nCircle = VBarCircleArr->GetSize();
		GMCircle* pCircle;
		for(long iCircle=0; iCircle<nCircle; iCircle++)
		{
			pCircle = (*VBarCircleArr)[iCircle];
			GM2DVector vCenter(pCircle->m_Center.m_X, pCircle->m_Center.m_Y);
			//  			if(SecPGon.ContainsEx(vCenter)>=0) continue;

			double Dist, MinDist = 1.E10;
			MREdge* pClosestEdge = NULL;
			long nEdge = mp_Section->ma_Edge.GetSize();
			for(long iEdge=0; iEdge<nEdge; iEdge += 2)
			{
				GM2DGeometry* pCurve = mp_Section->ma_Edge[iEdge]->mp_Curve;
				if(pCurve->m_ObjType == GT_LINE)
				{
					GM2DLineVector* pLine = (GM2DLineVector*)pCurve;
					if(pLine->WhichSide(vCenter)!=1)
						continue;
					Dist = pLine->Distance(vCenter);
					if(Dist < MinDist)
					{
						MinDist = Dist;
						pClosestEdge = mp_Section->ma_Edge[iEdge];
					}
				}
			}
			if(pClosestEdge)
				pClosestEdge->AddVBarLayer(MinDist, m_VBarDia);
		}

		// 콩이 총 4개인 경우, 가장 가까운 거리를 찾아서 Edge에 ma_VBarSet을 추가하면, 무작위로 한쪽의 ma_VBarSet이 '0'인 경우가 생기는 오류가 존재
		// 각 Edge의 ma_VBarSet을 검사하여, 갯수가 '0'인 Edge의 ma_VBarSet을 추가한다.
		// 위에서 넣어준 콩의 위치는 RemoveCornerVBar()함수에서 자동으로 지워진다. - by Gracias
		if(mp_Section->ma_Edge.GetSize() == 4)
		{
			long nEdge = mp_Section->ma_Edge.GetSize();
			for(int iEdge = 0; iEdge < nEdge; iEdge++)
			{
				MREdge* pEdge = mp_Section->ma_Edge[iEdge];
				if(pEdge->ma_VBarSet.GetSize() == 0)
				{
					MREdge* pNextEdge = NULL, *pPrevEdge = NULL;
					if(iEdge +1 == nEdge)
					{
						pNextEdge = mp_Section->ma_Edge[0];
						pPrevEdge = mp_Section->ma_Edge[iEdge-1];
					}
					else
					{
						pNextEdge = mp_Section->ma_Edge[iEdge+1];
						pPrevEdge = mp_Section->ma_Edge[nEdge-1];
					}
					double MinDist = 100;
					if(pNextEdge->ma_VBarSet.GetSize())
						MinDist = pNextEdge->ma_VBarSet[0]->m_LayerDistFromEdge;
					else if(pPrevEdge->ma_VBarSet.GetSize())
						MinDist = pPrevEdge->ma_VBarSet[0]->m_LayerDistFromEdge;
					else
						continue;
					pEdge->AddVBarLayer(MinDist, m_VBarDia);
				}
			}
		}
		for(long iCircle=nCircle-1; iCircle>=0; iCircle--)
		{
			pCircle = (*VBarCircleArr)[iCircle];
			GM2DVector vCenter(pCircle->m_Center.m_X, pCircle->m_Center.m_Y);
			if(SecPGon.ContainsEx(vCenter)>=0) continue;

			long nEdge = mp_Section->ma_Edge.GetSize();
			for(long iEdge=0; iEdge<nEdge; iEdge += 2)
				mp_Section->ma_Edge[iEdge]->AddVBarLoc(vCenter, m_VBarDia);
		}
	}

	return TRUE;
}

BOOL SRSectionBox::FindStrpBars_ColumnH()
{
	if(m_Shape==MSRcLineMemberM::CIRCLE)
	{
		if(mp_Section->ma_Edge.GetSize()!=1 || mp_Section->ma_Edge[0]==NULL)
			return FALSE;

		MREdge* pCirCleEdge = mp_Section->ma_Edge[0];
		dynamic_cast<GMCircle*>(pCirCleEdge->mp_Curve);
		long nCircle = ma_StrpCircle.GetSize();
		GMCircle* pCircle;
		for(long iCircle=0; iCircle<nCircle; iCircle++)
		{
			pCircle = ma_StrpCircle[iCircle];

			double Dist = pCircle->m_A;
			long nLayer = pCirCleEdge->ma_VBarSet.GetSize();
			for(long iLayer=0; iLayer<nLayer; iLayer++)
			{
// 				double DistStrp2VBar = Dist - pCirCleEdge->ma_VBarSet[iLayer]->m_LayerDistFromEdge;
// 				if(DistStrp2VBar > 0. && DistStrp2VBar < m_VBarDia*2.)
				double DistStrp2VBar = pCirCleEdge->ma_VBarSet[iLayer]->m_LayerDistFromEdge;
				if(DistStrp2VBar < Dist )
				{
					pCirCleEdge->ma_VBarSet[iLayer]->mp_StrpCurve = pCircle;
					if(iLayer==0)
						pCirCleEdge->mp_StrpCurve = pCircle;
					pCircle->m_Tag = 2;
				}
			}
		}
		return TRUE;
	}

	CTypedPtrArray<CObArray, GM2DGeometry*>* pCurveArr;
	long nCurve = ma_StrpLine.GetSize();
	if(nCurve>0)  // 스터럽 선이 레이어 구분되어 읽힌 경우이므로 바로 리턴
		pCurveArr = &ma_StrpLine;
	else
		pCurveArr = &ma_Curve;

	GMArc* pArc;
	GM2DLineVector* pLine;
	double VBarRadius;
	if(m_VBarDia > 0.)
		VBarRadius = m_VBarDia;
	else
		return FALSE;

	MREdge* pEdge=NULL;
	long nEdge = mp_Section->ma_Edge.GetSize();
	double minLength = 10;
	for(long iEdge=0; iEdge<nEdge; iEdge++)
	{
		pEdge = mp_Section->ma_Edge[iEdge];
		GM2DGeometry* pEdgeCurve = pEdge->mp_Curve;
		if(pEdgeCurve->m_ObjType != GT_LINE)
			continue;

		GM2DLineVector* pEdgeLine = (GM2DLineVector*)pEdgeCurve;

		long nCurve = pCurveArr->GetSize();
		for(long iLine=nCurve-1; iLine>=0; iLine--)
		{
			GM2DGeometry* pCurve = (*pCurveArr)[iLine];
			if(pCurve->m_ObjType == GT_LINE) 
			{
				pLine = (GM2DLineVector*)pCurve;
				if (pLine->Length() < minLength)	continue;	// BH2874 스터럽라인의 길이
				if(pLine->IsParallel4DTol(*pEdgeLine) == 0) continue;
				if(pEdgeLine->WhichSide(pLine->m_P0) == -1) continue;
				double Dist = pEdgeLine->Distance(pLine->m_P0);
				long nLayer = pEdge->ma_VBarSet.GetSize();
				for(long iLayer=0; iLayer<nLayer; iLayer++)
				{
					MRVBar* pLayer = pEdge->ma_VBarSet[iLayer];
					int NumVBarLoc = pLayer->ma_VBarLoc.GetSize();
					if(NumVBarLoc == 0)	continue;
					double DistStrp2VBar = pLayer->m_LayerDistFromEdge - Dist;
					if(pLayer->m_LayerDistFromEdge > Dist)
// 					if((DistStrp2VBar > (VBarRadius-1.) && DistStrp2VBar < VBarRadius*5.) ||
// 						 (DistStrp2VBar > -(VBarRadius*1.2) && DistStrp2VBar < -(VBarRadius-1.)))
					{
						double UrLoc0 = pEdgeLine->OrthoBaseLocValue(pLine->m_P0);
						double UrLoc1 = pEdgeLine->OrthoBaseLocValue(pLine->m_P1);
						long Result = pEdgeLine->ProjectGroup(UrLoc0, UrLoc1, 0.01);
						if(Result==1 || Result==2 || Result==6 || Result==7 || Result==19 || Result==20 || Result==24 || Result==25)
							continue;
						double MinVBarLoc = pLayer->ma_VBarLoc[0];
						double MaxVBarLoc = pLayer->ma_VBarLoc[pLayer->ma_VBarLoc.GetSize()-1];
						if( UrLoc0 < UrLoc1 &&  (fabs(UrLoc0-MinVBarLoc)>0.1 || fabs(UrLoc1-MaxVBarLoc)>0.1) )
							continue;
						if( UrLoc0 > UrLoc1 &&  (fabs(UrLoc1-MinVBarLoc)>0.1 || fabs(UrLoc0-MaxVBarLoc)>0.1) )
							continue;

						if(pLayer->mp_StrpCurve && pLayer->mp_StrpCurve->m_ObjType==GT_LINE)
						{
							 GM2DLineVector* pOldStrp = (GM2DLineVector*)pLayer->mp_StrpCurve;
							 double DistOldStrp2VBar = pLayer->m_LayerDistFromEdge - pEdgeLine->Distance(pOldStrp->m_P0);
							 if(DistOldStrp2VBar < DistStrp2VBar)
								 continue;
						}

// 조건에 맞는 스터럽라인이 여러개일 경우 가장 가까운 라인을 세팅하므로,
// 리무브라인을 마지막에 한꺼번에 한다. 안그러면 조건 맞는 라인 모두 리무브된다.
//						RemoveCurve(pLine);
						pEdge->ma_VBarSet[iLayer]->mp_StrpCurve = pLine;
						if(iLayer==0)
							pEdge->mp_StrpCurve = pLine;
						pLine->m_Tag = 2;
					}
				}
			}
			else
			{
				pArc = (GMArc*)pCurve;
	// 			GM2DVector P0 = pArc->GetStart2DVec();
	// 			double Dist = pLVec->Distance(P0);
	// 			if(Dist > m_LinkTol) continue;
	// 			GM2DVector P1 = pArc->GetEnd2DVec();
	// 			Dist = pLVec->Distance(P1);
	// 			if(Dist > m_LinkTol) continue;
	// 
	// 			double UrLoc0 = pLVec->OrthoBaseLocValue(P0);
	// 			double UrLoc1 = pLVec->OrthoBaseLocValue(P1);
	// 			long Result = pLVec->ProjectGroup(UrLoc0, UrLoc1, 0.01);
	// 			if(!(Result==9 || Result==17))
	// 				continue;
	// 
	// 			AddSectionLine(pArc);
	//			return FALSE;
			}
		}

// 조건에 맞는 스터럽라인이 여러개일 경우 가장 가까운 라인을 세팅하므로,
// 리무브라인을 마지막에 한꺼번에 한다. 안그러면 조건 맞는 라인 모두 리무브된다.
		long nLayer = pEdge->ma_VBarSet.GetSize();
		for(long iLayer=0; iLayer<nLayer; iLayer++)
		{
			MRVBar* pLayer = pEdge->ma_VBarSet[iLayer];
			if(pLayer->mp_StrpCurve && iLayer == 0)
				RemoveCurve(pLayer->mp_StrpCurve);
		}
		if(!pEdge->mp_StrpCurve && mp_Reader->m_Group == GROUP_COLUMN)
			return FALSE;
	}

	return TRUE;
}

double SRSectionBox::Get6Db()
{
	double BarDia = 10.0;

	long nGroup = mp_Reader->m_Group;
	if(nGroup==GROUP_COLUMN)
	{
		if(mp_StrpBar1)	BarDia = _ttof(mp_StrpBar1->GetFirstBarDiaName());
		if(mp_StrpBar2)
			BarDia = max(BarDia, _ttof(mp_StrpBar2->GetFirstBarDiaName()));
	}
	else if(nGroup==GROUP_BEAM)
	{
		if(mp_StrpBar1)	BarDia = _ttof(mp_StrpBar1->GetFirstBarDiaName());
	}
	else if(nGroup == GROUP_BUTTRESS)
	{
		if(mp_StrpBar1)	BarDia = _ttof(mp_StrpBar1->GetFirstBarDiaName());
	}

	return 6 * BarDia;
}

BOOL SRSectionBox::FindTieBars_ColumnH()
{
	double d6Db = Get6Db() / GetScaleFactorFromDimension();

	CTypedPtrArray<CObArray, GM2DGeometry*>* pCurveArr;
	long nCurve = ma_TieLine.GetSize();
	if (m_IsTieBarShapeByLayer)
		pCurveArr = &ma_TieBarShape;
	else
	{
		if (nCurve > 0)  // 스터럽 선이 레이어 구분되어 읽힌 경우이므로 바로 리턴
			pCurveArr = &ma_StrpLine;
		else
			pCurveArr = &ma_Curve;
	}
	// line의 길이가 6Db보다 작으면 무시
	double width = m_Width;
	double depth = m_Depth;
	if(m_DimensionWidth > 0)	width = m_DimensionWidth;
	if(m_DimensionDepth > 0) depth = m_DimensionDepth;
	if(m_Shape==MSRcLineMemberM::CIRCLE)
	{
		if(mp_Section->ma_Edge.GetSize()!=1 || mp_Section->ma_Edge[0]==NULL)
			return FALSE;

		MREdge* pCirCleEdge = mp_Section->ma_Edge[0];
		GM2DCircle* pCircleCurve = dynamic_cast<GM2DCircle*>(pCirCleEdge->mp_Curve);
		if (pCircleCurve != nullptr)	width = pCircleCurve->GetRadius() * 2;
		nCurve = pCurveArr->GetSize();
		for(long iLine=nCurve-1; iLine>=0; iLine--)
		{
			GM2DGeometry* pCurve = (*pCurveArr)[iLine];
			if(pCurve->m_ObjType == GT_LINE) 
			{
				GM2DLineVector* pLine = (GM2DLineVector*)pCurve;
				double length = pLine->Length();
				long nLayer = pCirCleEdge->ma_VBarSet.GetSize();
				for(long iLayer=0; iLayer<nLayer; iLayer++)
				{
					MRVBar* pLayer = pCirCleEdge->ma_VBarSet[iLayer];
					GMCircle* pStrpCircle = (GMCircle*)pLayer->mp_StrpCurve;
					if(pStrpCircle == NULL)	continue;

					if(length < max(width/2, d6Db) )	continue;
					double Dist, Radian0, Radian1;
					GMVector vCenter(pLine->m_P0.m_X, pLine->m_P0.m_Y);
					pStrpCircle->Contains(vCenter, &Dist, &Radian0);
					Dist = fabs(Dist - pStrpCircle->m_A);
 					if(Dist > d6Db)		continue;

					vCenter.SetXYZ(pLine->m_P1.m_X, pLine->m_P1.m_Y);
					pStrpCircle->Contains(vCenter, &Dist, &Radian1);
					Dist = fabs(Dist - pStrpCircle->m_A);
					if(Dist > d6Db)	continue;

					MRHBarByPoint* pHBar = new MRHBarByPoint;
					MRHBarPointByLoc* pHBarPoint = new MRHBarPointByLoc;
					pHBarPoint->m_EdgeIdx = 0;
					pHBarPoint->m_LayerIdx = iLayer;
					pHBarPoint->m_Loc = Radian0;
					pHBar->ma_Point.Add(pHBarPoint);
					pHBarPoint = new MRHBarPointByLoc;
					pHBarPoint->m_EdgeIdx = 0;
					pHBarPoint->m_LayerIdx = iLayer;
					pHBarPoint->m_Loc = Radian1;
					pHBar->ma_Point.Add(pHBarPoint);
					mp_Section->ma_TieBar.Add(pHBar);
					RemoveCurve(pLine);
					break;
				}
			}
			else 
				continue;
		}
		if(m_nStrp != 0 && m_nStrp != mp_Section->ma_TieBar.GetSize())
		{
			if (mp_Reader->IsPlainConcreteReinforceBar())	return TRUE;
			CString GroupName = _T("Column");
			CString errorMessage = _T("");
			if(mp_Name)
				errorMessage.Format(_T("%s 띠철근의 갯수가 다릅니다."), mp_Name->m_strText);
			mp_Reader->CreateErrorObject(GroupName, ERROR_LEVEL, ERROR_STIRRUPBAR_NUM, errorMessage, mp_Owner->mp_Rect);
			m_nErrorCode = 1;
		}
		return TRUE;
	}

	GMArc* pArc;
	GM2DLineVector* pLine;

	GM2DLineVector* pEdgeCurve0 = dynamic_cast<GM2DLineVector*>(mp_Section->ma_Edge[0]->mp_Curve);
	GM2DLineVector* pEdgeCurve1 = dynamic_cast<GM2DLineVector*>(mp_Section->ma_Edge[1]->mp_Curve);
	if (pEdgeCurve0 != nullptr && pEdgeCurve1 != nullptr)
	{
		width = pEdgeCurve0->Length();
		depth = pEdgeCurve1->Length();
	}

	nCurve = pCurveArr->GetSize();
	for(long iLine=nCurve-1; iLine>=0; iLine--)
	{
		GM2DGeometry* pCurve = (*pCurveArr)[iLine];
		if(pCurve->m_ObjType == GT_LINE) 
		{
			pLine = (GM2DLineVector*)pCurve;
			double length = pLine->Length();

			MREdge* pEdge=NULL;
			MREdge *pStrpEdge0=NULL, *pStrpEdge1=NULL;
			double StrpLoc0, StrpLoc1;
			long StrpLayer0, StrpLayer1;
			long EdgeIdx0, EdgeIdx1;
			double minDist0 = 10e10;
			double minDist1 = 10e10;
			long nEdge = mp_Section->ma_Edge.GetSize();
			GM2DLineVector* pStrpCurve0=NULL;
			GM2DLineVector* pStrpCurve1=NULL;
			for(long iEdge=0; iEdge<nEdge; iEdge++)
			{
				if(iEdge % 2)
				{
					if(length < max(width/2, d6Db))	continue;
				}
				else
				{
					if(length < max(depth/2, d6Db))	continue;
				}

				pEdge = mp_Section->ma_Edge[iEdge];
				GM2DGeometry* pEdgeCurve = pEdge->mp_Curve;
				if(pEdgeCurve->m_ObjType != GT_LINE)
					continue; 
				long nLayer = pEdge->ma_VBarSet.GetSize();
				nLayer = min(1, nLayer);		// 1단만 검토하면 됨
				for(long iLayer=0; iLayer<nLayer; iLayer++)
				{
					MRVBar* pLayer = pEdge->ma_VBarSet[iLayer];
					if(pLayer->mp_StrpCurve)
					{
						if(pLayer->mp_StrpCurve->m_ObjType != GT_LINE)
							continue; 
						GM2DLineVector* pStrpCurve = (GM2DLineVector*)pLayer->mp_StrpCurve;
						double dist0 = pStrpCurve->Distance(pLine->m_P0);
						if(dist0 < d6Db && dist0 < minDist0)
						{
							EdgeIdx0 = iEdge;
							pStrpEdge0 = pEdge;
							StrpLayer0 = iLayer;
							StrpLoc0 = pStrpCurve->OrthoBaseLocValue(pLine->m_P0);
							pStrpCurve0 = pStrpCurve;
							minDist0 = dist0;
							break;
						}
						else
						{
							if(dist0 < pLayer->m_LayerDistFromEdge && dist0 < minDist0)
							{
								EdgeIdx0 = iEdge;
								pStrpEdge0 = pEdge;
								StrpLayer0 = iLayer;
								StrpLoc0 = pStrpCurve->OrthoBaseLocValue(pLine->m_P0);
								pStrpCurve0 = pStrpCurve;
								minDist0 = dist0;
								break;
							}
						}
						double dist1 = pStrpCurve->Distance(pLine->m_P1);
						if(dist1 < d6Db && dist1 < minDist1)
						{
							EdgeIdx1 = iEdge;
							pStrpEdge1 = pEdge;
							StrpLayer1 = iLayer;
							StrpLoc1 = pStrpCurve->OrthoBaseLocValue(pLine->m_P1);
							pStrpCurve1 = pStrpCurve;
							minDist1 = dist1;
							break;
						}
						else
						{
							if(dist1 < pLayer->m_LayerDistFromEdge && dist1 < minDist1)
							{
								EdgeIdx1 = iEdge;
								pStrpEdge1 = pEdge;
								StrpLayer1 = iLayer;
								StrpLoc1 = pStrpCurve->OrthoBaseLocValue(pLine->m_P1);
								pStrpCurve1 = pStrpCurve;
								minDist1 = dist1;
								break;
							}
						}
					}
				}
				if(pStrpEdge0 && pStrpEdge1)
				{
					if(StrpLoc0 < DTOL_GM || StrpLoc0 > 1.0+DTOL_GM || fabs(StrpLoc0 - 1) < DTOL_GM)
						continue;
					if(StrpLoc1 < DTOL_GM || StrpLoc1 > 1.0+DTOL_GM || fabs(StrpLoc1 - 1) < DTOL_GM)
						continue;
					if(pStrpCurve0 && pStrpCurve1)
					{
//						double dTol = (VBarRadius*3)/pStrpCurve0->Length();
						double dTol = (25 / GetScaleFactorFromDimension()) /  pStrpCurve0->Length();
						if(CheckTieBarLocation(EdgeIdx0, StrpLoc0, dTol))	continue;

//						dTol = (VBarRadius*3)/pStrpCurve1->Length();
						dTol = (25 / GetScaleFactorFromDimension()) / pStrpCurve1->Length();
						if(CheckTieBarLocation(EdgeIdx1, StrpLoc1, dTol))	continue;
					}

					MRHBarByPoint* pHBar = new MRHBarByPoint;
					MRHBarPointByLoc* pHBarPoint = new MRHBarPointByLoc;
					pHBarPoint->m_EdgeIdx = EdgeIdx0;
					pHBarPoint->m_LayerIdx = StrpLayer0;
					pHBarPoint->m_Loc = StrpLoc0;
					pHBar->ma_Point.Add(pHBarPoint);
					pHBarPoint = new MRHBarPointByLoc;
					pHBarPoint->m_EdgeIdx = EdgeIdx1;
					pHBarPoint->m_LayerIdx = StrpLayer1;
					pHBarPoint->m_Loc = StrpLoc1;
					pHBar->ma_Point.Add(pHBarPoint);
					mp_Section->ma_TieBar.Add(pHBar);
					RemoveCurve(pLine);
					break;
				}
			}
		}
		else
		{
			pArc = (GMArc*)pCurve;
			continue;
		}
	}

	return TRUE;
}

double SRSectionBox::GetScaleFactorFromDimension()
{
	double scaleFactor = 1.0;
	if (ma_SRDim.GetSize() < 1)	return scaleFactor;
	SRDimension* pDim = ma_SRDim[0];
	if (pDim->mp_Owner != nullptr)
		return pDim->mp_Owner->m_dFactor;
	return scaleFactor;
}
GM2DRectangle* SRSectionBox::MakeStrpRect()
{
	int NumEdge = mp_Section->ma_Edge.GetSize();
	if(NumEdge != 4)	return NULL;

	MREdge* botEdge = mp_Section->ma_Edge[0];
	MREdge* topEdge = mp_Section->ma_Edge[2];
	MREdge* rightEdge = mp_Section->ma_Edge[1];
	MREdge* leftEdge = mp_Section->ma_Edge[3];

	GM2DLineVector* botStrpLine = dynamic_cast<GM2DLineVector*>(botEdge->mp_StrpCurve);
	GM2DLineVector* topStrpLine = dynamic_cast<GM2DLineVector*>(topEdge->mp_StrpCurve);
	GM2DLineVector* rightStrpLine = dynamic_cast<GM2DLineVector*>(rightEdge->mp_StrpCurve);
	GM2DLineVector* leftStrpLine = dynamic_cast<GM2DLineVector*>(leftEdge->mp_StrpCurve);

	if(botStrpLine == NULL || topStrpLine == NULL || rightStrpLine == NULL || leftStrpLine == NULL)	return NULL;
	GM2DVector origin, corner;

	origin = botStrpLine->OrthoBaseVector(leftStrpLine->m_P0);
	corner = topStrpLine->OrthoBaseVector(rightStrpLine->m_P0);
	GM2DRectangle* pRect = new GM2DRectangle(origin.m_X, origin.m_Y, corner.m_X, corner.m_Y);

	GM2DLineVector botEdgeLineVec(botEdge->m_P0, botEdge->m_P1);
	m_EdgeToStrpDist = botEdgeLineVec.Distance(botStrpLine->m_P0);
	return pRect;
}

bool SRSectionBox::IsContainPolyineInRect(GM2DRectangle* pRect, GM2DPolyline* polyline, double tolernace)
{
	int numPoint = polyline->ma_Point.GetSize();
	for(int iPoint = 0; iPoint < numPoint; iPoint++)
	{
		if(pRect->Contains(polyline->ma_Point[iPoint], tolernace) == FALSE)
			return false;
	}
	return true;
}

bool SRSectionBox::IsContainPolyineInCircle(GMCircle* pCircle, GM2DPolyline* polyline, double tolernace)
{
	int numPoint = polyline->ma_Point.GetSize();
	for(int iPoint = 0; iPoint < numPoint; iPoint++)
	{
		GM2DVector* pPoint = polyline->ma_Point[iPoint];
		GMVector vec(pPoint->m_X, pPoint->m_Y);
		if(pCircle->Contains(vec, &tolernace) > 0 )
			return false;
	}
	return true;
}

bool SRSectionBox::IsExistSpecificTieBar(MSSpecificTieBar* newTieBar)
{
	for (int i = 0; i < ma_SpecificTieBar.size(); i++)
	{
		MSSpecificTieBar* oldTieBar = ma_SpecificTieBar[i];
		if (oldTieBar->m_sLoc == newTieBar->m_sLoc)
			return true;
	}
	return false;
}

int SRSectionBox::FindSpecificTieBars_Column()
{
	if(m_Shape == MSRcLineMemberM::RECT)
		return FindSpecificTieBars_RectColumn();
	else if(m_Shape == MSRcLineMemberM::CIRCLE)
		return FindSpecificTieBars_RoundColumn();
	return 0;
}

int SRSectionBox::FindSpecificTieBars_RoundColumn()
{
	if(mp_Section == NULL)	return 0;
	if(mp_Section->ma_Edge.GetSize() != 1)	return 0;
	if(mp_MainBar1 == NULL)	return 0;

	long nMainBar = mp_MainBar1->GetNumBarsOnALayer(0);
	if(nMainBar == 0)	return 0;
	MREdge* pCirCleEdge = mp_Section->ma_Edge[0];
	long nLayer = pCirCleEdge->ma_VBarSet.GetSize();
	if(nLayer == 0)	return 0;

	MRVBar* pLayer = pCirCleEdge->ma_VBarSet[nLayer-1];
//	GMCircle* pStrpCircle = (GMCircle*)pLayer->mp_StrpCurve;
	GMCircle* pEdgeCircle = (GMCircle*)pCirCleEdge->mp_Curve;

	int NumPolyline = ma_Polyline.GetSize();
	for(int iPoly = 0; iPoly < NumPolyline; iPoly++)
	{
		GM2DPolyline* polyline = ma_Polyline[iPoly];
		int numPoint = polyline->ma_Point.GetSize();
		if(numPoint < 2)	continue;

		if(IsContainPolyineInCircle(pEdgeCircle, polyline, m_EdgeToStrpDist)==false)
			continue;

		int RebarIndex1 = -1;
		bool ReversFind = false;
		if(FindRebarIndexOnCircle(polyline, RebarIndex1, ReversFind)==false)
			continue;

		ReversFind = true;
		int RebarIndex2 = -1;
		if(FindRebarIndexOnCircle(polyline, RebarIndex2, ReversFind)==false)
			continue;

		MSSpecificTieBar::eEdge edgeIndex = MSSpecificTieBar::Top;
		if(IsCircleType_SpecificTieBar(polyline))
			edgeIndex = MSSpecificTieBar::Circle;
			
		MSSpecificTieBar* pSpecificTieBar = new MSSpecificTieBar();
		if(pSpecificTieBar->AnalyizeTieBarByRecoginze((int)edgeIndex, RebarIndex1, (int)edgeIndex, RebarIndex2)==false)
			delete pSpecificTieBar;
		else
		{
			if (IsExistSpecificTieBar(pSpecificTieBar))
				delete pSpecificTieBar;
			else
				ma_SpecificTieBar.push_back(pSpecificTieBar);
		}
	}
	return ma_SpecificTieBar.size();
}

int SRSectionBox::FindSpecificTieBars_RectColumn()
{
	GM2DRectangle* pStrpRect = MakeStrpRect();
	if(pStrpRect == NULL)	return 0;

	int NumPolyline = ma_Polyline.GetSize();
	for(int iPoly = 0; iPoly < NumPolyline; iPoly++)
	{
		GM2DPolyline* polyline = ma_Polyline[iPoly];
		int numPoint = polyline->ma_Point.GetSize();
		if(numPoint < 2)	continue;

		if(IsContainPolyineInRect(pStrpRect, polyline, m_EdgeToStrpDist)==false)
			continue;

		int EdgeIndex1 = -1;
		int RebarIndex1 = -1;
		bool ReversFind = false;
		if(FindEdgeOnRebarCircle(polyline, EdgeIndex1, RebarIndex1, ReversFind)==false)
			continue;

		ReversFind = true;
		int EdgeIndex2 = -1;
		int RebarIndex2 = -1;
		if(FindEdgeOnRebarCircle(polyline, EdgeIndex2, RebarIndex2, ReversFind)==false)
			continue;

		MSSpecificTieBar* pSpecificTieBar = new MSSpecificTieBar();
		if(pSpecificTieBar->AnalyizeTieBarByRecoginze(EdgeIndex1, RebarIndex1, EdgeIndex2, RebarIndex2)==false)
			delete pSpecificTieBar;
		else
		{
			if (IsExistSpecificTieBar(pSpecificTieBar))
				delete pSpecificTieBar;
			else
				ma_SpecificTieBar.push_back(pSpecificTieBar);
		}
	}

	delete pStrpRect;
	return true;
}

bool SRSectionBox::IsCircleType_SpecificTieBar(GM2DPolyline* Polyline)
{
	if(Polyline == NULL)	return false;

	int nCount = 0;
	int numLine = Polyline->GetNumLines();
	for(int iLine =0; iLine < numLine-1; iLine++)
	{
		GM2DLineVector firstLine = Polyline->GetLine(iLine);
		GM2DLineVector nextLine = Polyline->GetLine(iLine+1);

		double degree = firstLine.GetDegree(nextLine);
		if(fabs(90-degree) < 5.0)
			nCount++;
	}
	if(nCount >= 2)
		return true;
	return false;
}

bool SRSectionBox::FindRebarIndexOnCircle(GM2DPolyline* Polyline, int& RebarIndex, bool IsRevereFind)
{
	if(mp_Section == NULL)	return false;
	if(mp_Section->ma_Edge.GetSize() != 1)	return false;
	if(mp_MainBar1 == NULL)	return false;

	long nMainBar = mp_MainBar1->GetNumBarsOnALayer(0);
	if(nMainBar == 0)	return false;
	MREdge* pCirCleEdge = mp_Section->ma_Edge[0];
	long nLayer = pCirCleEdge->ma_VBarSet.GetSize();
	if(nLayer == 0)	return false;
	MRVBar* pLayer = pCirCleEdge->ma_VBarSet[nLayer-1];
	GMCircle* pStrpCircle = (GMCircle*)pLayer->mp_StrpCurve;
	if(pStrpCircle == NULL)	return false;

	GM2DLineVector* pLongLine = GetLongLineInPolyline(Polyline);
	if (pLongLine == nullptr)	return false;

	GM2DLineVector lineVec(pLongLine->m_P0, pLongLine->m_P1);
	double degree = lineVec.GetDegree();
	if (degree > 90 + DTOL_GM && degree <= 270 + DTOL_GM)
		lineVec.ReverseDirection();

	GM2DVector checkVec = lineVec.m_P0;
	if (IsRevereFind)	checkVec = lineVec.m_P1;

	double tolerance = max(Get6Db(), 2* m_EdgeToStrpDist);// max(m_VBarDia + m_EdgeToStrpDist, 30 + m_EdgeToStrpDist);
	double minDist = 10e10;

	int NumLoc = pLayer->ma_VBarLoc.GetSize();
	for(int iLoc = 0; iLoc < NumLoc; iLoc++)		// circle일때는 loc값에 radian이 들어 있다
	{
		double dLoc = pLayer->ma_VBarLoc[iLoc];
		double localX=0, localY = 0;
		pStrpCircle->GetGlobalXY(dLoc, localX, localY);
		GM2DVector LocVec(localX, localY);
		double dist = checkVec.Distance(LocVec);
		if(dist > tolerance)	continue;
		if(minDist > dist)
		{
			RebarIndex = iLoc;
			minDist = dist;
		}
	}
	if (RebarIndex != -1)	return true;
	return false;
}
bool SRSectionBox::IsSameStrriuppLine(GM2DLineVector* pLine)
{
	int NumEdge = mp_Section->ma_Edge.GetSize();
	for (int iEdge = 0; iEdge < NumEdge; iEdge++)
	{
		MREdge* pEdge = mp_Section->ma_Edge[iEdge];
		if (pEdge->mp_StrpCurve != nullptr)
		{
			GM2DLineVector* strpLine = dynamic_cast<GM2DLineVector*>(pEdge->mp_StrpCurve);
			if (strpLine != nullptr)
			{
				if (strpLine->IsSameLineVector(*pLine))	return true;
			}
		}
	}
	return false;
}

bool SRSectionBox::FindEdgeOnRebarCircle(GM2DPolyline* Polyline, int& EdgeIndex, int& RebarIndex, bool IsRevereFind)
{
	if (mp_Section == NULL)	return false;
	int NumEdge = mp_Section->ma_Edge.GetSize();

	GM2DLineVector* pLongLine = GetLongLineInPolyline(Polyline);
	if (pLongLine == nullptr)	return false;

	GM2DLineVector lineVec(pLongLine->m_P0, pLongLine->m_P1);
	double degree = lineVec.GetDegree();
	if (degree > 90 + DTOL_GM && degree <= 270 + DTOL_GM)
		lineVec.ReverseDirection();

	if (IsSameStrriuppLine(&lineVec))	return false;
	GM2DVector checkVec = lineVec.m_P0;
	if (IsRevereFind)	checkVec = lineVec.m_P1;

	double tolerance = max(Get6Db(), 2* m_EdgeToStrpDist);// max(m_VBarDia + m_EdgeToStrpDist, 2 * m_EdgeToStrpDist);
	double minDist = 10e10;

	for(int iEdge = 0; iEdge < NumEdge; iEdge++)
	{
		MREdge* pEdge = mp_Section->ma_Edge[iEdge];

		GM2DLineVector EdgeLineVec(pEdge->m_P0, pEdge->m_P1);
		int NumVBarSet = pEdge->ma_VBarSet.GetSize();
		for(int iBarSet = 0; iBarSet < NumVBarSet; iBarSet++)
		{
			MRVBar* pMRVBar = pEdge->ma_VBarSet[iBarSet];
			int NumLoc = pMRVBar->ma_VBarLoc.GetSize();
			for(int iLoc = 1; iLoc < NumLoc-1; iLoc++)		// TieBar는 첫번째와 마지막에는 있을 수가 없다(유팀장 의견)
			{
				double dLoc = pMRVBar->ma_VBarLoc[iLoc];
				GM2DVector LocVec = EdgeLineVec.PointAtParam(dLoc);
				double dist = checkVec.Distance(LocVec);
				if(dist > tolerance)	continue;
				if(minDist > dist)
				{
					EdgeIndex = iEdge;
					RebarIndex = iLoc;
					minDist = dist;
				}
			}
			break;		// 1단 Layer에서만 찾는다.(2단에 걸치는 타이바는 없다(시공이 안됨))
		}
	}
	if (RebarIndex != -1)	return true;
	return false;
}

GM2DLineVector* SRSectionBox::GetLongLineInPolyline(GM2DPolyline* Polyline)
{
	vector<GM2DCurve*> segments;
	Polyline->GetSegments(segments);

	double maxLength = 0.0;
	GM2DLineVector* pLongLine = nullptr;
	for (int i = 0; i < segments.size(); i++)
	{
		GM2DCurve* pCurve = segments[i];
		if (pCurve->m_ObjType != GT_LINE)	continue;

		GM2DLineVector* pLine = (GM2DLineVector*)pCurve;
		if (pLongLine == nullptr || pLine->Length() > maxLength)
		{
			pLongLine = pLine;
			maxLength = pLongLine->Length();
		}
	}

	return pLongLine;
}

void SRSectionBox::FindSupportMainBar()
{
	if (m_VBarDia < DTOL1)
		return;

	CTypedPtrArray<CObArray, GM2DGeometry*>* pCurveArr;
	if (m_IsSubMainBarShapeByLayer)
		pCurveArr = &ma_SubMainBarShape;
	else
		pCurveArr = &ma_Curve;

	CTypedPtrArray<CObArray, GM2DLineVector*> CrossLineArr;
	long nCurve = pCurveArr->GetSize();
	for (long iCurve = nCurve - 1; iCurve >= 0; iCurve--)
	{
		GM2DGeometry* pCurve = (*pCurveArr)[iCurve];
		if (pCurve->m_ObjType != GT_LINE)
			continue;

		GM2DLineVector* pLine = (GM2DLineVector*)pCurve;
		if (pLine->Length() < m_VBarDia*4.)
		{
			CrossLineArr.Add(pLine);
			pCurveArr->RemoveAt(iCurve);
		}
	}

	double radius = m_VBarDia;
	GM2DPolygon polygon;
	GetSectionPolygon(polygon);
	if (CrossLineArr.GetSize() != 0)
	{
		double CrossLineLength = CrossLineArr[0]->Length();
		radius = CrossLineLength / 2;
		long nLine = CrossLineArr.GetSize();

		map<GM2DLineVector*, long> usedLineVecMap;
		for (long iLine = 0; iLine < nLine; iLine++)
		{
			GM2DLineVector* pLineI = CrossLineArr[iLine];
			if (usedLineVecMap.find(pLineI) != usedLineVecMap.end())	continue;
			for (long jLine = iLine + 1; jLine < nLine; jLine++)
			{
				GM2DLineVector* pLineJ = CrossLineArr[jLine];
				if (usedLineVecMap.find(pLineJ) != usedLineVecMap.end())	continue;
				if (pLineI->m_P0.Distance(pLineJ->m_P0) > CrossLineLength)	continue;
				double MyLoc, UrLoc;
				GM2DVector CrossVec;
				long Result = pLineI->Intersect(*pLineJ, CrossVec, &MyLoc, &UrLoc);
				if (Result == 0 && fabs(MyLoc - 0.5) < 0.1 && fabs(UrLoc - 0.5) < 0.1)
				{
					usedLineVecMap.insert(make_pair(pLineI, iLine));
					usedLineVecMap.insert(make_pair(pLineJ, jLine));

					if (IsExistMainBarByLoc(&CrossVec))	continue;
					if (polygon.Contains(CrossVec) <= 0)
					{
						GMCircle* pSideBar = new GMCircle(CrossLineLength / 2., CrossVec.m_X, CrossVec.m_Y);
						ma_SubMainBar.Add(pSideBar);
						break;
					}
				}
			}
		}
	}
	// 'X', 'x'도 추가철근으로 판단 추가
	long nText = ma_GMText.GetSize();
	for (long iText = 0; iText < nText; iText++)
	{
		GMText* pText = ma_GMText[iText];
		auto sText = pText->m_strText;
		if (sText.MakeLower() != "x")	continue;

		if (IsExistMainBarByLoc(&pText->m_Vec))	continue;
		if (polygon.Contains(pText->m_Vec) <= 0)
		{
			GMCircle* pSideBar = new GMCircle(radius / 2., pText->m_Vec.m_X, pText->m_Vec.m_Y);
			ma_SubMainBar.Add(pSideBar);
		}
	}
}
bool SRSectionBox::IsExistMainBarByLoc(GM2DVector* loc)
{
	GMVector vec(loc->m_X, loc->m_Y, 0);
	int numMainBar = ma_MainVBarCircle.GetSize();
	for (int i = 0; i < numMainBar; i++)
	{
		double radius = ma_MainVBarCircle[i]->m_A;
		if (ma_MainVBarCircle[i]->m_Center.SameVector4DTOL(vec, radius))
			return true;
	}
	return false;
}

bool SRSectionBox::CheckTieBarLocation(long EdgeIdx, double Location, double dTol)
{
	int NumTieBar = mp_Section->ma_TieBar.GetSize();
	for(int iTieBar = 0; iTieBar < NumTieBar; iTieBar++)
	{
		MRHBarByPoint* pMRHBar = mp_Section->ma_TieBar[iTieBar];
		int NumPoint = pMRHBar->ma_Point.GetSize();
		for(int iPoint = 0; iPoint < NumPoint; iPoint++)
		{
			MRHBarPointByLoc* pMRHBarLoc = pMRHBar->ma_Point[iPoint];
			if(pMRHBarLoc->m_EdgeIdx == EdgeIdx)
			{
				double temp = fabs(pMRHBarLoc->m_Loc - Location);
				if(fabs(pMRHBarLoc->m_Loc - Location) < dTol)
					return true;
			}
		}
	}
	return false;
}


BOOL SRSectionBox::Convert2Numeric(CString& strDim)
{
// 	CString Temp = strDim;
// 	Temp.Replace(",", "");
// 	long dDim = _ttol(Temp);
// 	if(dDim==0)
// 		return false;
// 	else
// 		strDim = Temp;
	return TRUE;
}
long SRSectionBox::GetFromToStory(long& nFrom, long& nTo)
{
	nFrom = nTo = 0;
	nFrom = m_nFromStory;
	nTo = m_nToStory;
	return TRUE;
}

long SRSectionBox::GetSectionType()
{
	return m_Shape;
	long NumDim = ma_SRDim.GetSize();
	if(NumDim==0)		return (long)MSRcLineMemberM::RECT;
	else if(NumDim==1)	return (long)MSRcLineMemberM::CIRCLE;
	else if(NumDim==2)	return (long)MSRcLineMemberM::RECT;
	else						return (long)MSRcLineMemberM::POLYGON;
}
double SRSectionBox::GetSectionWidth()
{
	long nDim = ma_SRDim.GetSize();
	if(nDim < 2)	return 0.0;
	SRDimension* pHoriDim=NULL, *pVertDim=NULL;

	double DotProduct = ma_SRDim[0]->mp_LVec->DirVector() * ma_SRDim[1]->mp_LVec->DirVector();
	if(fabs(DotProduct) > DTOL_GM)  // 수평/수직 디맨전 2개 아닌 경우 실패
		return FALSE;
	
	if(fabs(sin(ma_SRDim[0]->m_Radian)) < DTOL_GM)
		pHoriDim = ma_SRDim[0];
	else if(nDim >= 2)
		pHoriDim = ma_SRDim[1];
	else
		return 0.0;

	return pHoriDim->m_Length*m_dFactor;
}
double SRSectionBox::GetSectionHeight()
{
	long nDim = ma_SRDim.GetSize();
	if(nDim < 2)	return 0.0;
	SRDimension* pHoriDim=NULL, *pVertDim=NULL;

	double DotProduct = ma_SRDim[0]->mp_LVec->DirVector() * ma_SRDim[1]->mp_LVec->DirVector();
	if(fabs(DotProduct) > DTOL_GM)  // 수평/수직 디맨전 2개 아닌 경우 실패
		return FALSE;
	
	if(fabs(sin(ma_SRDim[0]->m_Radian)) < DTOL_GM)
		pVertDim = ma_SRDim[1];
	else if(nDim >= 2)
		pVertDim = ma_SRDim[0];
	else
		return 0.0;
	return pVertDim->m_Length*m_dFactor;
}

void SRSectionBox::GetSectionPolygon(GM2DPolygon& SecPGon)
{
	long nEdge = mp_Section->ma_Edge.GetSize();
	for(long iEdge=0; iEdge<nEdge; iEdge++)
		SecPGon.AddCopy(mp_Section->ma_Edge[iEdge]->m_P0);
}

int SRSectionBox::FindTextFigureID(GMText* pText)
{
	return mp_Reader->FindTextFigureID(pText);
}

vector<int> SRSectionBox::GetTextFigureIDs(int nCellType)
{
	vector<int> Ids;
	if(mp_Reader->m_Group == GROUP_WALL || mp_Reader->m_Group == GROUP_SLAB)
	{
		ASSERT(0);
		return Ids;
	}

	auto it = mm_CellTypeToFigureIDs.find(nCellType);
	if(it != mm_CellTypeToFigureIDs.end())
		return it->second;
	return Ids;
}

void SRSectionBox::MakeSteelDataInfo_fromName()
{
	long nText = ma_GMText.GetSize();
	for(long iText=0; iText<nText; iText++)
	{
		GMText* pText = ma_GMText[iText];
		long nCellType;
		nCellType = mp_Reader->GetCellType(pText);
		if(nCellType==0)	continue;
		if(nCellType == MTReader::ctName)
		{
			double dLoc = 0.0;
			dLoc = pText->m_Vec.m_Y;
			SRSteelInfo* pSteelInfo = MakeSteelInfo(NULL, pText, nCellType);
		}
	}
	for(long iText=0; iText<nText; iText++)
	{
		GMText* pText = ma_GMText[iText];
		long nCellType;
		nCellType = mp_Reader->GetCellType(pText);
		if(nCellType==0)	continue;

		CString strData = pText->m_strText.MakeUpper();
		if(nCellType == MTReader::ctStory)
		{
			mp_Reader->Parsing_strStory(strData, mp_Owner->m_nFromStory, mp_Owner->m_nToStory);
			mp_Story = pText;
			continue;
		}

		SRSteelInfo* pSteelInfo = FindSteelInfo(pText->m_Vec.m_Y);
		if(pSteelInfo == NULL)	continue;

		bool IsAdded = false;
		if(nCellType == MTReader::ctSectionName)
		{
			pSteelInfo->m_SectionName = strData;	IsAdded = true;
		}
		else if(nCellType == MTReader::ctStudBolt)
		{
			pSteelInfo->m_StudBolt = strData;	IsAdded = true;
		}
		else if(nCellType == MTReader::ctMaterial)
		{
			MSMaterial* pMate = mp_Reader->mp_Bldg->GetMaterial(strData);
			if(pMate)
			{
				pSteelInfo->m_nMaterialID = pMate->m_ID;
				IsAdded = true;
			}
		}
		if(IsAdded)
			pSteelInfo->AddTextFigureID(nCellType, FindTextFigureID(pText));
	}
}

void SRSectionBox::ReNameByFloorNum()
{
	CString PrefixFloorNum = _T("");
	int nFrom = mp_Owner->m_nFromStory;
	int nTo = mp_Owner->m_nToStory;
	if(nFrom != 0 && nTo != 0)
	{
		if(nFrom == nTo)
			PrefixFloorNum.Format(_T("%ld"), nFrom);
		else
			PrefixFloorNum.Format(_T("%ld~%ld"), nFrom, nTo);
	}

	CString reName;
	CStringArray strNames;
	int nSteelInfo = ma_SteelInfo.GetSize();
	for(int iInfo = 0; iInfo < nSteelInfo; iInfo++)
	{
		SRSteelInfo* pInfo = ma_SteelInfo[iInfo];
		strNames.RemoveAll();
		long nNames = mp_Owner->GetName(pInfo->m_Name, strNames);
		for(int iName = 0; iName < nNames; iName++)
		{
			reName.Format(_T("%s%s"), PrefixFloorNum, strNames[iName]);
			pInfo->ma_Name.push_back(reName);
		}
		if(pInfo->m_nMaterialID == 0)
			pInfo->m_nMaterialID = mp_Reader->m_nMaterialID;
	}

}

SRSteelInfo* SRSectionBox::FindSteelInfo(double dLoc)
{
	long NumInfo = ma_SteelInfo.GetSize();
	for(long iInfo=0; iInfo < NumInfo; iInfo++)
	{
		SRSteelInfo* pInfo = ma_SteelInfo[iInfo];
		if(pInfo->m_StartLoc <= dLoc && dLoc <= pInfo->m_EndLoc)
			return pInfo;
	}
	return NULL;
}

SRSteelInfo* SRSectionBox::MakeSteelInfo(SRSteelInfo* pSteelInfo, GMText* pText, long nCellType)
{
	CString strData = pText->m_strText;
//	strData.Replace(_T(","), _T(""));
	//		enum CellType{ctName=1, ctStory, ctSize, ctMaterial, ctThick, ctWidth, ctDepth, ctMainBar, ctVBar, ctHBar, ctSection, ctTBar, ctBBar, ctStirrup, ctSBar1, ctSBar2, ctLoc, ctTieBar};

	double dLoc = 0;
	double dTol = pText->m_dHeight;

	if(pSteelInfo==NULL)
	{
		pSteelInfo = new SRSteelInfo();
		dLoc = pText->m_Vec.m_Y;

		pSteelInfo->m_StartLoc = pText->m_Vec.m_Y-dTol/2;
		pSteelInfo->m_EndLoc = pText->m_Vec.m_Y+dTol/2;
		if(nCellType == MTReader::ctName)
			pSteelInfo->m_Name = pText->m_strText;
		ma_SteelInfo.Add(pSteelInfo);
		pSteelInfo->AddTextFigureID(nCellType, FindTextFigureID(pText));
	}

	return pSteelInfo;
}

GMText* SRSectionBox::GetGMText(int nCellType)
{
	long nText = ma_GMText.GetSize();
	for(long iText=0; iText<nText; iText++)
	{
		GMText* pText = ma_GMText[iText];
		long nType = mp_Reader->GetCellType(pText);

		if(nCellType == nType)
			return pText;
	}
	return NULL;
}

bool SRSectionBox::MakeData_RWall()
{
	m_nErrorCode = 0;
	m_nWarningCode = 0;

	if (!CheckLayerTexts())
	{
		m_nErrorCode = 1;

		return false;
	}
	AnalyzeTextsByLayer_RWall();
	AnalyzeTexts_RWall();
	return true;
}

// BH-750 레이어당 Box내의 TEXT는 하나만 허용. 하나이상인경우 에러메세지 및 인식x
bool SRSectionBox::CheckLayerTexts()
{
	map<CString, int> layerToTextCountMap;
	int nText = ma_GMText.GetSize();
	for (int i = 0; i < nText; i++)
	{
		GMText* pText = ma_GMText[i];
		long nCellType;
		nCellType = mp_Reader->GetCellType(pText);
		if (nCellType == 0)	continue;

		vector<GMText*> textArr = mp_Owner->GetTextsToLayerName(nCellType);
		if (textArr.size() > 1)
			return false;
	}
	return true;
}

void SRSectionBox::AnalyzeTextsByLayer_RWall()
{
	vector<int> cellTypeArr;
	cellTypeArr.push_back((int)MTReader::ctName);		cellTypeArr.push_back((int)MTReader::ctStory);
	cellTypeArr.push_back((int)MTReader::ctThick);		/*cellTypeArr.push_back((int)MTReader::ctMaterial);*/		

	for(int iCell = 0; iCell < cellTypeArr.size(); iCell++)
	{
		int nCellType = cellTypeArr[iCell];
		GMText* pLayerText = mp_Owner->GetFirstTextToLayerName(nCellType);
		if(pLayerText)
		{
			if(nCellType==MTReader::ctName)
			{
				mp_Owner->mp_Name = pLayerText;
				mp_Name = pLayerText;
			}
			else if(nCellType == MTReader::ctStory)
			{
				mp_Story = pLayerText;
				CString strData = mp_Story->m_strText;
//				strData.Replace(_T(","), _T(""));
				mp_Reader->Parsing_strStory(strData, m_nFromStory, m_nToStory);
				mp_Owner->m_nFromStory = m_nFromStory;
				mp_Owner->m_nToStory = m_nToStory;
			}
			else if(nCellType == MTReader::ctThick)
				mp_Size = pLayerText;

            vector<int> figureIds;
            figureIds.push_back(FindTextFigureID(pLayerText));
            mm_CellTypeToFigureIDs.insert(make_pair(nCellType, figureIds));
		}
	}
}

void SRSectionBox::AnalyzeTexts_RWall()
{
	long nText = ma_GMText.GetSize();
	for(long iText=0; iText<nText; iText++)
	{
		GMText* pText = ma_GMText[iText];
		long nCellType;
		nCellType = mp_Reader->GetCellType(pText);
		if(nCellType==0)	continue;

		if(nCellType==MTReader::ctName)
		{
			if(mp_Name == NULL)
			{
				mp_Name = pText;
                vector<int> figureIds;
                figureIds.push_back(FindTextFigureID(pText));
                mm_CellTypeToFigureIDs.insert(make_pair(nCellType, figureIds));
			}
		}
		else if(nCellType == MTReader::ctStory)
		{
			if(mp_Story == NULL)
			{
				mp_Story = pText;
                vector<int> figureIds;
                figureIds.push_back(FindTextFigureID(pText));
                mm_CellTypeToFigureIDs.insert(make_pair(nCellType, figureIds));
				CString strData = pText->m_strText;
//				strData.Replace(_T(","), _T(""));
				mp_Reader->Parsing_strStory(strData, m_nFromStory, m_nToStory);
				mp_Owner->m_nFromStory = m_nFromStory;
				mp_Owner->m_nToStory = m_nToStory;
			}
		}
		else if(nCellType == MTReader::ctThick)
		{
			if(mp_Size == NULL)
			{
				mp_Size = pText;
                vector<int> figureIds;
                figureIds.push_back(FindTextFigureID(pText));
                mm_CellTypeToFigureIDs.insert(make_pair(nCellType, figureIds));
			}
		}
	}
}
#pragma region --- set circle section
void SRSectionBox::SetSectionCircle(GMCircle* pCircle)
{
	mp_Section = new MRRcSection();
	mp_SectCircle = pCircle;

	GMCircle* pEdgeCircle = new GMCircle(*mp_SectCircle);
	mp_Section->ma_Edge.Add(new MREdge(pEdgeCircle));
	m_Shape = MSRcLineMemberM::CIRCLE;
}
#pragma endregion
#pragma region --- set rect section
void SRSectionBox::SetSectionRect(GM2DPolyline* pPolyline)
{
	if (!pPolyline->IsRectangle())	return;
	GM2DRectangle rect = pPolyline->GetRectangle();
	GM2DPolyline rectPoly;
	rect.GetBoundPolyline(rectPoly);
	if (rectPoly.ma_Point.GetSize() != 5)	return;
	GM2DLineVector* bottom = new GM2DLineVector(*rectPoly.ma_Point[0], *rectPoly.ma_Point[1]);
	GM2DLineVector* right = new GM2DLineVector(*rectPoly.ma_Point[1], *rectPoly.ma_Point[2]);
	GM2DLineVector* top = new GM2DLineVector(*rectPoly.ma_Point[2], *rectPoly.ma_Point[3]);
	GM2DLineVector* left = new GM2DLineVector(*rectPoly.ma_Point[3], *rectPoly.ma_Point[0]);

	MREdge* bottomEdge = new MREdge(bottom);
	MREdge* rightEdge = new MREdge(right);
	MREdge* topEdge = new MREdge(top);
	MREdge* leftEdge = new MREdge(left);

	bottomEdge->mp_Linked0 = leftEdge;
	bottomEdge->mp_Linked1 = rightEdge;
	rightEdge->mp_Linked0 = bottomEdge;
	rightEdge->mp_Linked1 = topEdge;
	topEdge->mp_Linked0 = rightEdge;
	topEdge->mp_Linked1 = leftEdge;
	leftEdge->mp_Linked0 = topEdge;
	leftEdge->mp_Linked1 = bottomEdge;

	mp_Section = new MRRcSection();
	mp_Section->ma_Edge.Add(bottomEdge);
	mp_Section->ma_Edge.Add(rightEdge);
	mp_Section->ma_Edge.Add(topEdge);
	mp_Section->ma_Edge.Add(leftEdge);
	m_Shape = MSRcLineMemberM::RECT;
}
#pragma endregion
#pragma region --- add mainbar
void SRSectionBox::SetMainBarShape(vector<GMCircle*> circles)
{
	ma_MainVBarCircle.RemoveAll();
	for (int i = 0; i < circles.size(); i++)
	{
		ma_MainVBarCircle.Add(circles[i]);
	}
	m_IsMainBarShapeByLayer = true;
}

#pragma endregion --- set mainbar
#pragma region --- set tiebar
void SRSectionBox::SetTieBarShape(vector<GM2DLineVector*> geoms)
{
	ma_TieBarShape.RemoveAll();
	for (int i = 0; i < geoms.size(); i++)
	{
		ma_TieBarShape.Add(geoms[i]);
	}
	m_IsTieBarShapeByLayer = true;;

}
void SRSectionBox::SetSubMainBarShape(vector<GM2DLineVector*> geoms)
{
	ma_SubMainBarShape.RemoveAll();
	for (int i = 0; i < geoms.size(); i++)
	{
		ma_SubMainBarShape.Add(geoms[i]);
	}
	m_IsSubMainBarShapeByLayer = true;;

}

#pragma endregion

