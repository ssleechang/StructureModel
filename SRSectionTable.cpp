#include "StdAfx.h"
#include "SRSectionTable.h"

#include "SRSectionBox.h"
#include "MTReader.h"

#include "GMLib/GM2DVector.h"
#include "GMLib/GMText.h"


SRSectionTable::SRSectionTable(MTReader* pReader)
{
	mp_Reader = pReader;
	m_dFactor = 1.0;

	m_nErrorCode = 0;
	m_nWarningCode = 0;

	mp_Rect = NULL;
	mp_Name = NULL;

	m_nMaterialID = 0;
	m_nFromStory = m_nToStory = 0;
	m_Width = m_Depth = 0.0;

	m_bStandardSection = false;
	m_Shape = 0;
	m_VdId = 0;

	m_SteelSectionName = _T("");
	m_StudBolt = _T("");
	m_LayerHelper.SetTable(this);

	m_sWidth = m_sDepth = _T("");
	m_sDimensionWidth = _T("");
	m_sDimensionDepth = _T("");

}


SRSectionTable::~SRSectionTable(void)
{
	if(mp_Rect)	delete mp_Rect;
	DeleteBoxArr();
	DeleteSRDimArr();
	DeleteTextArr();
	DeleteCurveArr();
	DeleteOutLineArr();
	DeleteStrpLineArr();
	DeleteTieLineArr();
	DeleteCircleArr();
	DeleteSubMainBarCircleArr();
}

long SRSectionTable::AddDimension(SRDimension* pNewDim)
{
	long Index = -1;
	bool bSameDim = false;
	long NumDimension = ma_SRDim.GetSize();
	for(long iDim = 0; iDim < NumDimension; iDim++)
	{
		SRDimension* pOldDim = ma_SRDim[iDim];
		if(pNewDim->mp_LVec->m_P0.SameVector4DTOL(pOldDim->mp_LVec->m_P0, DTOL1) && 
			pNewDim->mp_LVec->m_P1.SameVector4DTOL(pOldDim->mp_LVec->m_P1, DTOL1))
		{
			bSameDim = true;
			break;
		}
	}
	if(bSameDim)
		delete pNewDim;
	else
		Index = ma_SRDim.Add(pNewDim);
	return Index;
}

int SRSectionTable::AddLine(GM2DLineVector* pNewLine, int lineType)
{
	CTypedPtrArray<CObArray, GM2DGeometry*>* pGeomDataArr=NULL;
	switch(lineType)
	{
	case CURVE_TYPE:		pGeomDataArr = &ma_Curve;	break;
	case OUTLINE_TYPE:		pGeomDataArr = &ma_OutLine;	break;
	case STRPLINE_TYPE:		pGeomDataArr = &ma_StrpLine;break;
	case TIELINE_TYPE:		pGeomDataArr = &ma_TieLine;	break;
	default:
		return -1;
	}

	return pGeomDataArr->Add(pNewLine);
}

int SRSectionTable::AddPolyline(GM2DPolyline* polyline)
{
	return ma_Polyline.Add(polyline);
}

void SRSectionTable::MergeLineVector()
{
	CTypedPtrArray<CObArray, GM2DGeometry*>* pGeomDataArr=NULL;
	pGeomDataArr = &ma_Curve;
	MergeLineVector(pGeomDataArr);
	pGeomDataArr = &ma_OutLine;
	MergeLineVector(pGeomDataArr);
	pGeomDataArr = &ma_StrpLine;
	MergeLineVector(pGeomDataArr);
	pGeomDataArr = &ma_TieLine;	
	MergeLineVector(pGeomDataArr);
}

void SRSectionTable::MergeLineVector(CTypedPtrArray<CObArray, GM2DGeometry*>* pGeomDataArr)
{
	int NumData = pGeomDataArr->GetSize();
	for(int iData = 0; iData < NumData; iData++)
	{
		GM2DGeometry* pIGeom = pGeomDataArr->GetAt(iData);
		if(pIGeom->m_ObjType != GT_LINE)	continue;
		if(pIGeom->m_Tag == -1)	continue;
		GM2DLineVector* pILine= (GM2DLineVector*)pIGeom;

		for(int jData = iData+1; jData < NumData; jData++)
		{
			GM2DGeometry* pJGeom = pGeomDataArr->GetAt(jData);
			if(pJGeom->m_ObjType != GT_LINE)	continue;
			if(pJGeom->m_Tag == -1)	continue;
			GM2DLineVector* pJLine= (GM2DLineVector*)pJGeom;

			pILine->MergeWhenColinear(pJLine, DTOL1, 1.0, 1.0);
		}
	}
}

void SRSectionTable::DeleteBoxArr()
{
	long nBox = ma_Box.GetSize();
	for(long iBox=0; iBox<nBox; iBox++)
		delete ma_Box[iBox];
	ma_Box.RemoveAll();
}
void SRSectionTable::DeleteSRDimArr()
{
	long nSRDim = ma_SRDim.GetSize();
	for(long iSRDim=0; iSRDim<nSRDim; iSRDim++)
		delete ma_SRDim[iSRDim];
	ma_SRDim.RemoveAll();
}
void SRSectionTable::DeleteTextArr()
{
	long nText = ma_Text.GetSize();
	for(long iText=0; iText<nText; iText++)
		delete ma_Text[iText];
	ma_Text.RemoveAll();
}
void SRSectionTable::DeleteCurveArr()
{
	long nCurve = ma_Curve.GetSize();
	for(long iCurve=0; iCurve<nCurve; iCurve++)
	{
		if(ma_Curve[iCurve]->m_Tag==2)
			continue;
		delete ma_Curve[iCurve];
	}
	ma_Curve.RemoveAll();
}
void SRSectionTable::DeleteOutLineArr()
{
	long nOutLine = ma_OutLine.GetSize();
	for(long iOutLine=0; iOutLine<nOutLine; iOutLine++)
		delete ma_OutLine[iOutLine];
	ma_OutLine.RemoveAll();
}
void SRSectionTable::DeleteStrpLineArr()
{
	long nStrpLine = ma_StrpLine.GetSize();
	for(long iStrpLine=0; iStrpLine<nStrpLine; iStrpLine++)
		delete ma_StrpLine[iStrpLine];
	ma_StrpLine.RemoveAll();
}
void SRSectionTable::DeleteTieLineArr()
{
	long nTieLine = ma_TieLine.GetSize();
	for(long iTieLine=0; iTieLine<nTieLine; iTieLine++)
		delete ma_TieLine[iTieLine];
	ma_TieLine.RemoveAll();
}
void SRSectionTable::DeleteCircleArr()
{
	long nCircle = ma_Circle.GetSize();
	for(long iCircle=0; iCircle<nCircle; iCircle++)
	{
		if(ma_Circle[iCircle]->m_Tag==2)
			continue;
		delete ma_Circle[iCircle];
	}
	ma_Circle.RemoveAll();
}

void SRSectionTable::DeleteSubMainBarCircleArr()
{
	long nCircle = ma_SubMainBarCircle.GetSize();
	for(long iCircle=0; iCircle<nCircle; iCircle++)
	{
		if(ma_SubMainBarCircle[iCircle]->m_Tag==2)
			continue;
		delete ma_SubMainBarCircle[iCircle];
	}
	ma_SubMainBarCircle.RemoveAll();
}

bool SRSectionTable::Contains(GM2DVector* pVec1, GM2DVector* pVec2)
{
	double tolerance = 1.0;
	if (this->mp_Reader->m_Group == GROUP_BEAM)
		tolerance = _ttol(mp_Reader->GetStringByCellType(MTReader::ctSpliteDistTol));
	else if (this->mp_Reader->m_Group != GROUP_WALL)
		tolerance = -DTOL1;

	if(mp_Rect==NULL)	return false;
	if (mp_Reader->m_Group == GROUP_WALL && pVec1 && pVec2)
	{
		GM2DLineVector line(*pVec1, *pVec2);
		GM2DVector midVec = line.MassCenter();
		if (mp_Rect->Contains(pVec1, tolerance) && mp_Rect->Contains(&midVec, tolerance))	return true;
		if (mp_Rect->Contains(pVec2, tolerance) && mp_Rect->Contains(&midVec, tolerance))	return true;
		return false;
	}

	GM2DVector* TestVec;
	if(pVec1)
	{
		TestVec = pVec1;
		BOOL RtnVal = mp_Rect->Contains(TestVec, tolerance);
		if(RtnVal == FALSE)	return false;
	}
	if(pVec2)
	{
		TestVec = pVec2;
		BOOL RtnVal = mp_Rect->Contains(TestVec, tolerance);
		if(RtnVal == FALSE)	return false;
	}
	return true;
}
GMCircle* SRSectionTable::FindGMCircle(GM2DVector CenPnt, double dRadius)
{
	double dTol = dRadius*2;
	long NumCircle = ma_Circle.GetSize();
	for(long iCircle=0; iCircle<NumCircle; iCircle++)
	{
		GMCircle* TheCir = ma_Circle[iCircle];
 		if (fabs(TheCir->m_A - dRadius) > DTOL1)	continue;
		if (fabs(TheCir->m_Center.m_X - CenPnt.m_X) < dTol && fabs(TheCir->m_Center.m_Y - CenPnt.m_Y) < dTol)
		{
			return TheCir;
		}
	}

	long NumSubCircle = ma_SubMainBarCircle.GetSize();
	for(iCircle = 0; iCircle < NumSubCircle; iCircle++)
	{
		GMCircle* TheCir = ma_SubMainBarCircle[iCircle];
// 		if(fabs(TheCir->m_A-dRadius) > DTOL1)	continue;
		if(fabs(TheCir->m_Center.m_X-CenPnt.m_X) < dTol && fabs(TheCir->m_Center.m_Y-CenPnt.m_Y) < dTol)
		{
			return TheCir;
		}
	}
	return NULL;
}
void SRSectionTable::MakeData()
{
	MakeSectionBox();
	long nBox = ma_Box.GetSize();
	for(long iBox=0; iBox<nBox; iBox++)
		ma_Box[iBox]->MakeData();
}
void SRSectionTable::MakeSectionBox()
{
	long nGroup = mp_Reader->m_Group;
	long nType = mp_Reader->m_Type;

	if(nGroup==GROUP_COLUMN)
	{
		if(mp_Reader->m_MaterialType == MSMaterial::msConcrete)
			MakeSectionBox_RcColumn();
		else if(mp_Reader->m_MaterialType == MSMaterial::msSteel)
			MakeSectionBox_SteelColumn();
		else if(mp_Reader->m_MaterialType == MSMaterial::msSRC)
			MakeSectionBox_SrcColumn();
	}
	else if(nGroup==GROUP_WALL)
	{
		SRSectionBox* pSectionBox = new SRSectionBox(this);
		pSectionBox->mp_Rect = new GM2DRectangle(*mp_Rect);
		CopyData(pSectionBox, false);
		ma_Box.Add(pSectionBox);
	}
	else if(nGroup==GROUP_BEAM || nGroup == GROUP_BUTTRESS)
	{
		if(mp_Reader->m_MaterialType == MSMaterial::msConcrete)
			MakeSectionBox_RcBeam();
		else if(mp_Reader->m_MaterialType == MSMaterial::msSteel)
			MakeSectionBox_SteelBeam();
		else if(mp_Reader->m_MaterialType == MSMaterial::msSRC)
			MakeSectionBox_SrcBeam();
	}
	else if(nGroup == GROUP_SLAB)
	{
		SRSectionBox* pSectionBox = new SRSectionBox(this);
		pSectionBox->mp_Rect = new GM2DRectangle(*mp_Rect);
		CopyData(pSectionBox, false);
		ma_Box.Add(pSectionBox);
	}
	else if(nGroup == GROUP_RWALL)
	{
		SRSectionBox* pSectionBox = new SRSectionBox(this);
		pSectionBox->mp_Rect = new GM2DRectangle(*mp_Rect);
		CopyData(pSectionBox, false);
		ma_Box.Add(pSectionBox);
	}
}

void SRSectionTable::MakeSectionBox_RcColumn()
{
	SRSectionBox* pSectionBox = new SRSectionBox(this);
	pSectionBox->mp_Rect = new GM2DRectangle(*mp_Rect);
	CopyData(pSectionBox, false);
	ma_Box.Add(pSectionBox);
}


void SRSectionTable::MakeSectionBox_SteelColumn()
{
	SRSectionBox* pSectionBox = new SRSectionBox(this);
	pSectionBox->mp_Rect = new GM2DRectangle(*mp_Rect);
	CopyData(pSectionBox, false);
	ma_Box.Add(pSectionBox);
}

void SRSectionTable::MakeSectionBox_SrcColumn()
{
	SRSectionBox* pSectionBox = new SRSectionBox(this);
	pSectionBox->mp_Rect = new GM2DRectangle(*mp_Rect);
	CopyData(pSectionBox, false);
	ma_Box.Add(pSectionBox);
}

void SRSectionTable::MakeSectionBox_RcBeam()
{
	long nBox = GetBeamSectionCount();
	if(nBox==0)	
		return ;

	double Width = mp_Rect->Width()/nBox;
	double originY = mp_Rect->m_Origin.m_Y;
	double cornerY = mp_Rect->m_Corner.m_Y;
	double originX = mp_Rect->m_Origin.m_X;
	for(long iBox=0; iBox<nBox; iBox++)
	{
		double cornerX = originX+Width;
		SRSectionBox* pSectionBox = new SRSectionBox(this);
		pSectionBox->m_nSpliteDistTol = _ttol(mp_Reader->GetStringByCellType(MTReader::ctSpliteDistTol));
		pSectionBox->m_BoxNumber = iBox+1;
		pSectionBox->mp_Rect = new GM2DRectangle(originX, originY, cornerX, cornerY);
		CopyData(pSectionBox, true);
		ma_Box.Add(pSectionBox);
		originX = cornerX;
	}

	CopySRDimOfBeam();
	SetSizeInfo();
}

void SRSectionTable::MakeSectionBox_SteelBeam()
{
	SRSectionBox* pSectionBox = new SRSectionBox(this);
	pSectionBox->mp_Rect = new GM2DRectangle(*mp_Rect);
	CopyData(pSectionBox, false);
	ma_Box.Add(pSectionBox);
}

void SRSectionTable::MakeSectionBox_SrcBeam()
{
	long nBox = GetBeamSectionCount();
	if(nBox==0)	
		return ;

	double Width = mp_Rect->Width()/nBox;
	double originY = mp_Rect->m_Origin.m_Y;
	double cornerY = mp_Rect->m_Corner.m_Y;
	double originX = mp_Rect->m_Origin.m_X;
	for(long iBox=0; iBox<nBox; iBox++)
	{
		double cornerX = originX+Width;
		SRSectionBox* pSectionBox = new SRSectionBox(this);
		pSectionBox->m_nSpliteDistTol = _ttol(mp_Reader->GetStringByCellType(MTReader::ctSpliteDistTol));
		pSectionBox->m_BoxNumber = iBox+1;
		pSectionBox->mp_Rect = new GM2DRectangle(originX, originY, cornerX, cornerY);
		CopyData(pSectionBox, true);
		ma_Box.Add(pSectionBox);
		originX = cornerX;
	}

	CopySRDimOfBeam();
	SetSizeInfo();
}

void SRSectionTable::SetSizeInfo()
{
	long nText = ma_Text.GetSize();
	for(long iText=0; iText<nText; iText++)
	{
		GMText* pText = ma_Text[iText];
		long nCellType = mp_Reader->GetCellType(pText);
		if(nCellType == MTReader::ctSize)
		{
			ParsingSize(pText->m_strText);
			break;
		}
	}
}
void SRSectionTable::ParsingSize(CString strData)
{
    strData.Replace(_T(","), _T(""));
	long nSeperator = strData.Find(_T("X"));
	if(nSeperator == -1)
	{
		nSeperator = strData.Find(_T("×"));	// 특수문자 ×
	}

	if(nSeperator==-1)
		return;
	long nLen = strData.GetLength();
	m_sWidth = strData.Mid(0, nSeperator);
	m_sDepth = strData.Mid(nSeperator+1, nLen-(nSeperator+1));
	m_Width = _ttol(m_sWidth);
	m_Depth = _ttol(m_sDepth);
}

int SRSectionTable::GetBeamSectionCount()
{
	int nTBar = 0;
	int nBBar = 0;
	int nSBar = 0;
	int nText = ma_Text.GetSize();
	for (int iText = 0; iText < nText; iText++)
	{
		GMText* pText = ma_Text[iText];
		long nCellType = mp_Reader->GetCellType(pText);
		if (nCellType == MTReader::ctTBar)
			nTBar++;
		else if (nCellType == MTReader::ctBBar)
			nBBar++;
		else if (nCellType == MTReader::ctStirrup)
			nSBar++;
	}
	int maxSectionNum = 3;
	nTBar = min(nTBar, maxSectionNum);
	nBBar = min(nTBar, maxSectionNum);
	nSBar = min(nSBar, maxSectionNum);
	int nSection = max(nTBar, nBBar);
	nSection = max(nSection, nSBar);
	if (nSection == 0)	nSection = 1;
	return nSection;
}

long SRSectionTable::GetTopBarNum()
{
	long nTBar = 0;
	long nText = ma_Text.GetSize();
	for(long iText=0; iText<nText; iText++)
	{
		GMText* pText = ma_Text[iText];
		long nCellType = mp_Reader->GetCellType(pText);
		if(nCellType != MTReader::ctTBar)	continue;

		nTBar++;
	}
	return nTBar;
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

long SRSectionTable::MakeDimensionFromLine()
{
	CTypedPtrArray<CObArray, GM2DLineVector*> LVecArr;

	long nCurve = ma_Curve.GetSize();
	for(long iLine=0; iLine<nCurve; iLine++)
	{
		GM2DGeometry* pCurve = ma_Curve[iLine];
		if(pCurve->m_ObjType != GT_LINE) 
			continue;

		GM2DLineVector* pLine = (GM2DLineVector*)pCurve;
		LVecArr.Add(pLine);
	}

	CArray<double, double> ValueArr;
	CTypedPtrArray<CObArray, GMText*> TextArr;
	long nText = ma_Text.GetSize();
	for(long iText=nText-1; iText>=0; iText--)
	{
		GMText* pText = ma_Text[iText];
		CString strText = pText->m_strText;
		if(!IsNumeric(strText))
			continue;

		long nLine = LVecArr.GetSize();
		for(long iLine=0; iLine<nLine; iLine++)
		{
			GM2DLineVector* pDimLine = LVecArr[iLine];
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
				GM2DLineVector* pAuxLine = LVecArr[jLine];
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
				SRDimension* pDim = new SRDimension(this);
				RemoveCurve(pDimLine);
				ma_SRDim.Add(pDim);
				pDim->mp_Text = pText;
				ma_Text.RemoveAt(iText);
				double MyLoc0, UrLoc0;
				GM2DVector CrossVec0;
				double MyLoc1, UrLoc1;
				GM2DVector CrossVec1;
				pDimLine->Intersect(*AuxLineArr[0], CrossVec0, &MyLoc0, &UrLoc0);
				pDimLine->Intersect(*AuxLineArr[1], CrossVec1, &MyLoc1, &UrLoc1);
				pDimLine->m_P0 = CrossVec0;
				pDimLine->m_P1 = CrossVec1;
				pDim->mp_LVec = pDimLine;
				pDim->m_Radian = pDimLine->GetRadian();
				pDim->m_Length = _ttof(strText);
				if(fabs(pDimLine->Length() - pDim->m_Length) < DTOL_GM)
					pDim->m_NonScale = false;
				else
				{
					pDim->m_NonScale = true;
					m_dFactor = pDim->m_Length/pDimLine->Length();
				}
				pDim->m_EdgePosition = UrLoc0<0.5 ? pDimLine->WhichSide(AuxLineArr[0]->m_P1) : pDimLine->WhichSide(AuxLineArr[0]->m_P0);
				break;
			}
		}
	}

	return ma_SRDim.GetSize();
}

void SRSectionTable::RemoveCurve(GM2DGeometry* pCurve)
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


void SRSectionTable::CopyData(SRSectionBox* pBox, bool bCalcLoc)
{
	// 	CTypedPtrArray<CObArray, SRDimension*> ma_SRDim;
	// 	CTypedPtrArray<CObArray, GMText*>   ma_Text;
	// 	CTypedPtrArray<CObArray, GM2DGeometry*> ma_Curve;
	// 	CTypedPtrArray<CObArray, GM2DGeometry*> ma_OutLine;
	// 	CTypedPtrArray<CObArray, GM2DGeometry*> ma_StrpLine;
	// 	CTypedPtrArray<CObArray, GM2DLineVector*> ma_TieLine;
	// 	CTypedPtrArray<CObArray, GMCircle*> ma_Circle;

	long nData, iData;

	nData = ma_SRDim.GetSize();
	if(mp_Reader->m_Group==GROUP_COLUMN)
	{
		for(iData=0; iData<nData; iData++)
		{
			SRDimension* pSRDim = ma_SRDim[iData];
			if(bCalcLoc==false)	pBox->ma_SRDim.Add(pSRDim);
			else
			{
				if(pBox->Contains(&pSRDim->mp_LVec->m_P0, &pSRDim->mp_LVec->m_P1))
					pBox->ma_SRDim.Add(pSRDim);
			}
		}

		int NumPolyline = ma_Polyline.GetSize();
		for(int ipoly = 0; ipoly < NumPolyline; ipoly++)
		{
			pBox->ma_Polyline.Add(ma_Polyline[ipoly]);
		}
	}
	nData = ma_Text.GetSize();
	for(iData=0; iData<nData; iData++)
	{
		GMText* pText = ma_Text[iData];
		long nCellType = mp_Reader->GetCellType(pText);
		if(nCellType==MTReader::ctName)
		{
			if (mp_Name == NULL)
			{
				mp_Name = pText;
				if (mp_Name->m_strText == _T("-1EG2A"))
					pBox->_sectionBoxID = 1;
			}
			else if (mp_Name != pText)
			{
				bool bExist = false;
				int nName = ma_Name.GetSize();
				for (int iName = 0; iName < nName; iName++)
				{
					if (pText == ma_Name[iName])
					{
						bExist = true;
						break;
					}
				}
				if (bExist == false)
					ma_Name.Add(pText);
			}
			else if (mp_Name != NULL && mp_Name->m_strText == _T("-1EG2A"))
				pBox->_sectionBoxID = 1;
			if(mp_Reader->m_Group == GROUP_SLAB)
			{
				if(pBox->Contains(&pText->m_Vec, NULL))
					pBox->ma_GMText.Add(pText);
			}
			else if(mp_Reader->m_MaterialType == MSMaterial::msSteel)
			{
				if(pBox->Contains(&pText->m_Vec, NULL))
					pBox->ma_GMText.Add(pText);
			}
			continue;
		}
		else if(nCellType==MTReader::ctStory)
		{
			mp_Reader->Parsing_strStory(pText->m_strText, m_nFromStory, m_nToStory);
			if(bCalcLoc==false)	pBox->ma_GMText.Add(pText);
			else
			{
				if(pBox->Contains(&pText->m_Vec, NULL))
					pBox->ma_GMText.Add(pText);
			}
		}
		else if(bCalcLoc==false)	pBox->ma_GMText.Add(pText);
		else
		{
			if(pBox->Contains(&pText->m_Vec, NULL))
				pBox->ma_GMText.Add(pText);
		}
	}

	CopyData(pBox, ma_Curve, bCalcLoc, CURVE_TYPE);
	CopyData(pBox, ma_OutLine, bCalcLoc, OUTLINE_TYPE);
	CopyData(pBox, ma_StrpLine, bCalcLoc, STRPLINE_TYPE);
	CopyData(pBox, ma_TieLine, bCalcLoc, TIELINE_TYPE);

	CopyCircleData(pBox, ma_Circle, bCalcLoc, MAIN_CIRCLE_TYPE);
	CopyCircleData(pBox, ma_SubMainBarCircle, bCalcLoc, SUB_CIRCLE_TYPE);
	CopyCellTypeToGeometry(pBox);
}

void SRSectionTable::CopyCircleData(SRSectionBox* pBox, CTypedPtrArray<CObArray, GMCircle*>& CircleArr, bool bCalcLoc, long nDataType)
{
	CTypedPtrArray<CObArray, GMCircle*>* pBoxDataArr=NULL;
	if(nDataType == MAIN_CIRCLE_TYPE)
		pBoxDataArr = &pBox->ma_Circle;
	else if(nDataType == SUB_CIRCLE_TYPE)
		pBoxDataArr = &pBox->ma_SubMainBar;
	if(pBoxDataArr == NULL)
		return;

	long nData = CircleArr.GetSize();
	for(long iData=0; iData<nData; iData++)
	{
		GMCircle* pCircle = CircleArr[iData];
		if(bCalcLoc==false)
		{
			pBoxDataArr->Add(pCircle);
		}
		else
		{
			GM2DVector CenVec(pCircle->m_Center.m_X, pCircle->m_Center.m_Y);
			// 콩의 경우에는 m_nSpliteDistTol 값을 적용 하지 않는다.
			double dOldTol = pBox->m_nSpliteDistTol;
			pBox->m_nSpliteDistTol = DTOL_GM;
			if(pBox->Contains(&CenVec, NULL))
			{
				pBoxDataArr->Add(pCircle);
			}
			pBox->m_nSpliteDistTol = dOldTol;
		}
	}
}

void SRSectionTable::CopyData(SRSectionBox* pBox, CTypedPtrArray<CObArray, GM2DGeometry*>& DataArr, bool bCalcLoc, long nDataType)
{
	CTypedPtrArray<CObArray, GM2DGeometry*>* pBoxDataArr=NULL;
	switch(nDataType)
	{
	case CURVE_TYPE:		pBoxDataArr = &pBox->ma_Curve;	break;
	case OUTLINE_TYPE:		pBoxDataArr = &pBox->ma_OutLine;	break;
	case STRPLINE_TYPE:		pBoxDataArr = &pBox->ma_StrpLine;break;
	case TIELINE_TYPE:		pBoxDataArr = &pBox->ma_TieLine;	break;
	default:
		return;
	}
	long nData = DataArr.GetSize();
	for(long iData=0; iData<nData; iData++)
	{
		GM2DGeometry* pGeom = DataArr[iData];
		if(bCalcLoc==false)	pBoxDataArr->Add(pGeom);
		else
		{
			GM2DVector* pVec1=NULL, *pVec2=NULL;
			if(pGeom->m_ObjType == GT_LINE)	
			{
				GM2DLineVector* pLVec = (GM2DLineVector*)pGeom;
				pVec1 = &pLVec->m_P0;
				pVec2 = &pLVec->m_P1;
			}
			else if(pGeom->m_ObjType == GT_ARC)
			{
				GMArc* pArc = (GMArc*)pGeom;
				pVec1 = &pArc->GetStart2DVec();
				pVec2 = &pArc->GetEnd2DVec();
			}
			else
			{
				ASSERT(0);
			}
			if(pBox->Contains(pVec1, pVec2))
				pBoxDataArr->Add(pGeom);
		}
	}
}

long SRSectionTable::GetSectionName(CStringArray& strNames)
{
	if(mp_Name == NULL)
		return 0;
	GetName(mp_Name->m_strText, strNames);
	int nName = ma_Name.GetSize();
	for(int iName = 0; iName < nName; iName++)
	{
		if(mp_Name == ma_Name[iName])
			continue;
		GetName(ma_Name[iName]->m_strText, strNames);
	}

	nName = strNames.GetSize();
	for( iName = nName-1; iName >= 0; iName--)
	{
		CString oldName = strNames.GetAt(iName);
		for(int jName = iName-1; jName >= 0; jName--)
		{
			if(oldName == strNames.GetAt(jName))
			{
				strNames.RemoveAt(iName);
				break;
			}
		}
	}

	return strNames.GetSize();
}

CString SRSectionTable::RemoveTextInBracket(CString text)
{
	CString rtnText = text;

	int firstBracket = rtnText.Find('(');
	int secondBracket = rtnText.Find(')');
	if (firstBracket != -1 && secondBracket != -1)
	{
		CString sTemp = rtnText.Mid(firstBracket, (secondBracket - firstBracket) + 1);
		rtnText.Replace(sTemp, _T(""));

		if (rtnText.Find('(') != -1)
			rtnText = RemoveTextInBracket(rtnText);
	}

	return rtnText;
}
long SRSectionTable::GetName(CString name, CStringArray& strNames)
{
	CString strName = _T("");
	strName = name;

	if(mp_Reader->m_RemoveNameOption == REMOVE_NAME_BRACKET)
	{
		strName = RemoveTextInBracket(strName);
// 		int firstBracket = -1;
// 		int secondBracket = -1;
// 		for (int i = 0; i < strName.GetLength(); i++)
// 		{
// 			if (strName[i] == '(')
// 				firstBracket = i;
// 			else if (strName[i] == ')')
// 				secondBracket = i;
// 
// 		}
// 		int firstBracket = strName.Find('(');
// 		int secondBracket = strName.Find(')');
// 		if(firstBracket != -1 && secondBracket != -1)
// 		{
// 			CString sTemp = strName.Mid(firstBracket, (secondBracket-firstBracket)+1);
// 			strName.Replace(sTemp, _T(""));
// 		}
	}


	int nSIdx=0;
	CStringArray SeperateNames;
	CString Compare;
	long nLength = strName.GetLength();
	for(long iIdx=0; iIdx<nLength; iIdx++)
	{
		Compare = strName.GetAt(iIdx);
		int nSerperate = mp_Reader->ma_NameSeperate.size();
		for(int iSeperate = 0; iSeperate < nSerperate; iSeperate++)
		{
			CString sSeperate = mp_Reader->ma_NameSeperate[iSeperate];
			if(Compare == sSeperate)
			{
				CString TheName = strName.Mid(nSIdx, iIdx-nSIdx);
				SeperateNames.Add(TheName);
				nSIdx = iIdx+1;
				break;
			}
		}
	}
	CString TheName = strName.Mid(nSIdx, iIdx-nSIdx);
	SeperateNames.Add(TheName);

	CString DivideName;
	int nName = SeperateNames.GetSize();
	for(int iName = 0; iName < nName; iName++)
	{
		CString Name = SeperateNames[iName];
		int tilde = Name.Find('~');
		if(tilde != -1)
		{
			long nFrom = 0;
			CString sFrom = Name.Mid(0, tilde);
			mp_Reader->Parsing_strStory(sFrom, nFrom, nFrom);
			CString sRemain = Name.Mid(tilde+1, Name.GetLength() - (tilde+1));

			int toIndex = 01;
			int nRemain = sRemain.GetLength();
			for(int iRemain = 0; iRemain < nRemain; iRemain++)
			{
				char aChar = sRemain.GetAt(iRemain);
				if(isdigit(aChar) || aChar == '-')
					continue;
				else if(aChar == 'B' && iRemain == 0)
					continue;
				else
				{
					toIndex = iRemain;
					break;
				}
			}
			CString sTo;	 sTo = sRemain.Mid(0, toIndex);
			long nTo = 0;
			mp_Reader->Parsing_strStory(sTo, nTo, nTo);

			if(nFrom != 0 && nTo != 0)
			{
				if (nFrom > nTo)
				{
					long nFromTemp = nFrom;
					nFrom = nTo;
					nTo = nFromTemp;
				}
				CString remainName = sRemain.Mid(toIndex, sRemain.GetLength()-(toIndex));
				for(int iStory = nFrom ; iStory <= nTo; iStory++)
				{
					if(iStory == 0)	continue;
					DivideName.Format(_T("%ld%s"), iStory, remainName);
                    if (DivideName != _T(""))
					    strNames.Add(DivideName);
				}
			}
            else
            {
                if (Name != _T(""))
                    strNames.Add(Name);
            }
		}
        else
        {
            if (Name != _T(""))
                strNames.Add(Name);
        }
	}
	return strNames.GetSize();
}

long SRSectionTable::MakeErrorObject(long nNO)
{
	long nGroup = mp_Reader->m_Group;
	long nType = mp_Reader->m_Type;
	if(nGroup==GROUP_COLUMN)
		return MakeErrorObject_Column(nNO);
	else if(nGroup==GROUP_WALL)
		return MakeErrorObject_Wall(nNO);
	else if(nGroup==GROUP_BEAM)
		return MakeErrorObject_Beam(nNO);
	return NULL;
}

long SRSectionTable::MakeErrorObject_Column(long nNO)
{
// 	XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();
// 	long nBox = ma_Box.GetSize();
// 	if(nBox==0)
// 	{
// 		m_nErrorCode |= ERROR_NO_BOX;
// 		return m_nErrorCode;
// 	}
// 	SRSectionBox* pBox = ma_Box[0];
// 	XErrorObject* pErrorObject = NULL;
// 
// 	if(mp_Name==NULL)								m_nErrorCode |= ERROR_NO_NAME;
// 	if(m_nFromStory==0 || m_nToStory==0)		m_nErrorCode |= ERROR_STORY;
// 	if(pBox->m_Width < 50 ||pBox-> m_Depth < 50)	m_nErrorCode |= ERROR_SIZE;
// 	if(pBox->mp_MainBar1==NULL)						m_nErrorCode |= ERROR_MAINBAR;
// 	if(pBox->mp_StrpBar1==NULL)							m_nErrorCode |= ERROR_STRPBAR1;
// 	if(pBox->mp_StrpBar2==NULL)							m_nErrorCode |= ERROR_STRPBAR2;
// 
// 	long nError = 0;
// 	CString GroupName;
// 	GroupName.Format(_T("%ld. No Name"), nNO);
// 	if(mp_Name)	GroupName = mp_Name->m_strText;
// 
// 	long nErrorCode = nNO;
// 	CString strMsg;
// 	if(m_nErrorCode & ERROR_NO_NAME)
// 	{
// 		strMsg.Format("부재이름 설정이 되지 않았습니다."); 
// 		pErrorObject = pMessageContainer->CreateErrorObject(nErrorCode, MESSAGE_LEVEL_ERROR, strMsg);
// 		pErrorObject->SetGroupName(GroupName);
// 		pErrorObject->SetShowSelectCompoG(FALSE);
// 		pErrorObject->AddEtcID(m_VdId);
// 		pMessageContainer->AddErrorObject(pErrorObject);
// 		nError++;
// 	}	
// 	if(m_nErrorCode & ERROR_STORY)
// 	{
// 		strMsg.Format("층정보가 설정되지 않았습니다.");
// 		pErrorObject = pMessageContainer->CreateErrorObject(nErrorCode, MESSAGE_LEVEL_ERROR, strMsg);
// 		pErrorObject->SetGroupName(GroupName);
// 		pErrorObject->SetShowSelectCompoG(FALSE);
// 		pErrorObject->AddEtcID(m_VdId);
// 		pMessageContainer->AddErrorObject(pErrorObject);
// 		nError++;
// 	}
// 	if(m_nErrorCode & ERROR_SIZE)
// 	{
// 		strMsg.Format("치수정보와 치수선 정보가 모두 없습니다.");
// 		pErrorObject = pMessageContainer->CreateErrorObject(nErrorCode, MESSAGE_LEVEL_ERROR, strMsg);
// 		pErrorObject->SetGroupName(GroupName);
// 		pErrorObject->SetShowSelectCompoG(FALSE);
// 		pErrorObject->AddEtcID(m_VdId);
// 		pMessageContainer->AddErrorObject(pErrorObject);
// 		nError++;
// 	}
// 	if(m_nErrorCode & ERROR_MAINBAR)
// 	{
// 		strMsg.Format("주근 정보가 설정되지 않았습니다.");
// 		pErrorObject = pMessageContainer->CreateErrorObject(nErrorCode, MESSAGE_LEVEL_ERROR, strMsg);
// 		pErrorObject->SetGroupName(GroupName);
// 		pErrorObject->SetShowSelectCompoG(FALSE);
// 		pErrorObject->AddEtcID(m_VdId);
// 		pMessageContainer->AddErrorObject(pErrorObject);
// 		nError++;
// 	}
// 	if(m_nErrorCode & ERROR_STRPBAR1)
// 	{
// 		strMsg.Format("중앙부 철근 정보가 설정되지 않았습니다.");
// 		pErrorObject = pMessageContainer->CreateErrorObject(nErrorCode, MESSAGE_LEVEL_ERROR, strMsg);
// 		pErrorObject->SetGroupName(GroupName);
// 		pErrorObject->SetShowSelectCompoG(FALSE);
// 		pErrorObject->AddEtcID(m_VdId);
// 		pMessageContainer->AddErrorObject(pErrorObject);
// 		nError++;
// 	}
	// 	if(m_nErrorCode & ERROR_STRPBAR2)
	// 	{
	// 		strMsg.Format("단부 철근 정보가 설정되지 않았습니다.");
	// 		pErrorObject = pMessageContainer->CreateErrorObject(nErrorCode, MESSAGE_LEVEL_ERROR, strMsg);
	// 		pErrorObject->SetGroupName(GroupName);
	// 		pErrorObject->SetShowSelectCompoG(FALSE);
	// 		pErrorObject->AddEtcID(m_VdId);
	// 		pMessageContainer->AddErrorObject(pErrorObject);
	// 		nError++;
	// 	}
	// 	return nError;
	return 0;
}
long SRSectionTable::MakeErrorObject_Wall(long nNO)
{
	return NULL;
}
long SRSectionTable::MakeErrorObject_Beam(long nNO)
{
	return NULL;
}

void SRSectionTable::CopySRDimOfBeam()
{
	CTypedPtrArray<CObArray, SRDimension*> HoriDimArr;
	CTypedPtrArray<CObArray, SRDimension*> VertDimArr;

	long nData = ma_SRDim.GetSize();
	if(nData==0)
		nData = MakeDimensionFromLine();

	for(long iData=0; iData<nData; iData++)
	{
		SRDimension* pSRDim = ma_SRDim[iData];
		double dDegree = fabs(pSRDim->mp_LVec->GetDegree());
		bool IsHorizontalDimension = true;
		if(45. <= dDegree && dDegree < 135)
			IsHorizontalDimension = false;
		else if(225. <= dDegree && dDegree < 315)
			IsHorizontalDimension = false;

		if(IsHorizontalDimension)
		{
			long nHori=HoriDimArr.GetSize();
			for(long iHori=0; iHori<nHori; iHori++)
			{
				if(pSRDim->mp_LVec->m_P0.m_X < HoriDimArr[iHori]->mp_LVec->m_P0.m_X)
				{
					HoriDimArr.InsertAt(iHori, pSRDim);
					break;
				}
			}
			if(iHori==nHori)
				HoriDimArr.Add(pSRDim);
		}
		else
		{
			long nVert=VertDimArr.GetSize();
			for(long iVert=0; iVert<nVert; iVert++)
			{
				if(pSRDim->mp_LVec->m_P0.m_X < VertDimArr[iVert]->mp_LVec->m_P0.m_X)
				{
					VertDimArr.InsertAt(iVert, pSRDim);
					break;
				}
			}
			if(iVert==nVert)
				VertDimArr.Add(pSRDim);
		}
	}

	long nBox = ma_Box.GetSize();
	long nHDim = HoriDimArr.GetSize();
	long nVDim = VertDimArr.GetSize();
	for(int iBox = 0; iBox < nBox; iBox++)
	{
		SRSectionBox* pBox = ma_Box[iBox];
		for(int iDim = 0; iDim < nHDim; iDim++)
		{
			SRDimension* pSRDim = HoriDimArr[iDim];
			if(pBox->Contains(&pSRDim->mp_LVec->m_P0, &pSRDim->mp_LVec->m_P1))
				pBox->ma_SRDim.Add(pSRDim);
		}
		for(int iDim = 0; iDim < nVDim; iDim++)
		{
			SRDimension* pSRDim = VertDimArr[iDim];
			if(pBox->Contains(&pSRDim->mp_LVec->m_P0, &pSRDim->mp_LVec->m_P1))
				pBox->ma_SRDim.Add(pSRDim);
		}
	}

	// Dimension이 없는 Box에 Dimension복사(수직 디맨젼)
	if(nVDim == 0)
		return;

	SRDimension* pVertDim = VertDimArr[0];
	for(int iBox = 0; iBox < nBox; iBox++)
	{
		SRSectionBox* pBox = ma_Box[iBox];
		int nDim = pBox->ma_SRDim.GetSize();
		if(nDim != 1)	continue;

		SRDimension* pHoriDim = pBox->ma_SRDim[0];
		SRDimension* pNewVert = pVertDim->Copy();
		pNewVert->mp_LVec->m_P0.m_X = pHoriDim->mp_LVec->m_P0.m_X;
		pNewVert->mp_LVec->m_P1.m_X = pHoriDim->mp_LVec->m_P0.m_X;

		pBox->ma_SRDim.Add(pNewVert);
	}

	if (HoriDimArr.GetSize() > 0)
	{
		SRDimension* pHoriDim = HoriDimArr[0];
        auto fixWidth = ROUND(pHoriDim->m_Length * 0.1, 1) * 10;
		m_sDimensionWidth.Format(_T("%ld"), fixWidth);
	}
	if (VertDimArr.GetSize() > 0)
	{
		SRDimension* pVertDim = VertDimArr[0];
        auto fixDepth = ROUND(pVertDim->m_Length * 0.1, 1) * 10;
		m_sDimensionDepth.Format(_T("%ld"), fixDepth);
	}
}

CString SRSectionTable::GetBeamSectionName(long BoxNumber)
{
	CString SectionName = _T("중앙부");
	int NumBox = ma_Box.GetSize();
	if(NumBox == 0)		return _T("No Section");
	if(NumBox == 1)		return SectionName;

	if(NumBox == 2)
	{
		if(BoxNumber == 1)
			SectionName = _T("양단부");
	}
	else if(NumBox == 3)
	{
		if(mp_Reader->m_eBeamSectionType == MTReader::Small_Mid_Large)
		{
			if(BoxNumber == 1)			SectionName = _T("외단부");
			else if(BoxNumber == 3)	SectionName = _T("내단부");
		}
		else
		{
			if(BoxNumber == 1)			SectionName = _T("내단부");
			else if(BoxNumber == 3)	SectionName = _T("외단부");
		}
	}
	return SectionName;
}

int SRSectionTable::AddBoxData(int type, int row, CString data)
{
	long nBox = ma_Box.GetSize();
	if(nBox == 0)	return -1;

	SRSectionBox* pBox = ma_Box[0];
	pBox->MakeWallInfo(type, row, data);
	return 1;
}

void SRSectionTable::MakeData_Excel()
{
	long nBox = ma_Box.GetSize();
	for(int iBox = 0; iBox < nBox; iBox++)
		ma_Box[iBox]->MakeData_Excel();
}

void SRSectionTable::AddTextByLayerName(CString layerName, GMText* pText)
{
	vector<int> cellTypes = mp_Reader->GetCellTypesByLayerName(layerName);
	for (int i = 0; i < cellTypes.size(); i++)
	{
		m_LayerHelper.Add(cellTypes[i], pText);
	}
}

void SRSectionTable::AddDimensionByLayerName(CString layerName, SRDimension* pDimension)
{
	vector<int> cellTypes = mp_Reader->GetCellTypesByLayerName(layerName);
	for (int i = 0; i < cellTypes.size(); i++)
	{
		m_LayerHelper.Add(cellTypes[i], pDimension);
	}
}

GMText* SRSectionTable::GetFirstTextToLayerName(int cellType)
{
	vector<GMText*> TextArr = GetTextsToLayerName(cellType);
	if(TextArr.size() == 0)	return NULL;
	return TextArr[0];
}

vector<GMText*> SRSectionTable::GetTextsToLayerName(int cellType)
{
	return m_LayerHelper.FindTexts(cellType);
}

vector<SRDimension*> SRSectionTable::GetDimensionsToLayerName(int cellType)
{
	return m_LayerHelper.FindDimensions(cellType);
}

SRDimension* SRSectionTable::GetFirstDimensionsToLayerName(int cellType)
{
	vector<SRDimension*> DimensionArr = GetDimensionsToLayerName(cellType);
	if(DimensionArr.size() == 0)	return NULL;
	return DimensionArr[0];
}

void SRSectionTable::AddCellTypeToGeometry(long cellType, GMObject* pGeom)
{
	auto it = mm_CellTypeToGeoms.find((MTReader::CellType)cellType);
	if (it == mm_CellTypeToGeoms.end())
	{
		vector<GMObject*> geoms;
		geoms.push_back(pGeom);
		mm_CellTypeToGeoms.insert(make_pair(cellType, geoms));
	}
	else
	{
		it->second.push_back(pGeom);
	}
}

void SRSectionTable::CopyCellTypeToGeometry(SRSectionBox* pBox)
{
	for(auto it = mm_CellTypeToGeoms.begin(); it != mm_CellTypeToGeoms.end(); it++)
	{
		MTReader::CellType cellType = (MTReader::CellType)it->first;
		vector<GMObject*> geoms = it->second;

		switch (cellType)
		{
			case MTReader::ctSection:					SetSectionBoxSection(pBox, geoms);	break;
			case MTReader::ctMainBarShape:		SetSectionBoxMainBar(pBox, geoms);	break;
			case MTReader::ctTieBarShape:			SetSectionBoxTieBar(pBox, geoms);	break;
			case MTReader::ctSubMainBarShape:	SetSectionBoxSubMainBar(pBox, geoms);	break;
		}
	}
}

bool SRSectionTable::IsContained(SRSectionBox* pBox, GM2DPolyline* polyline)
{
	for (int j = 0; j < polyline->ma_Point.GetSize(); j++)
	{
		if (!pBox->Contains(polyline->ma_Point[j], nullptr))
			return false;
	}
	return true;
}
bool SRSectionTable::IsContained(SRSectionBox* pBox, GMCircle* circle)
{
	GM2DCircle circle2D(circle->m_A, circle->m_Center.m_X, circle->m_Center.m_Y);
	if (!pBox->Contains(&circle2D.m_Center, nullptr))	return false;
	GM2DVectors vectors;
	circle2D.GetPoints(4, vectors);
	for (int j = 0; j < vectors.GetSize(); j++)
	{
		if (!pBox->Contains(vectors.GetAt(j), nullptr))
			return false;
	}
	return true;
}
bool SRSectionTable::IsContained(SRSectionBox* pBox, GM2DLineVector* line)
{
	return pBox->Contains(&line->m_P0, &line->m_P1);
}

#pragma region --- ctSection
void SRSectionTable::SetSectionBoxSection(SRSectionBox* pBox, vector<GMObject*> geoms)
{
	for (int i = 0; i < geoms.size(); i++)
	{
		GMObject* pCurve = geoms[i];
		GM2DPolyline* poly = dynamic_cast<GM2DPolyline*>(pCurve);
		if (poly != nullptr)
		{
			if (!poly->IsClosed())	poly->MakeClosedPolyline(DTOL_GM);
			if (!poly->IsRectangle())	continue;
			if(IsContained(pBox, poly))
				pBox->SetSectionRect(poly);
		}
		GMCircle* circle = dynamic_cast<GMCircle*>(pCurve);
		if (circle != nullptr)
		{
			if(IsContained(pBox, circle))
				pBox->SetSectionCircle(circle);
		}
	}
}
#pragma endregion
#pragma region --- ctMainBarShape
void SRSectionTable::SetSectionBoxMainBar(SRSectionBox* pBox, vector<GMObject*> geoms)
{	
	vector<GMCircle*> circles;
	for (int i = 0; i < geoms.size(); i++)
	{
		GMObject* pGeom = geoms[i];
		GMCircle* circle = dynamic_cast<GMCircle*>(pGeom);
		if (circle != nullptr)
		{
			if(IsContained(pBox, circle))
				circles.push_back(circle);
		}
	}
	if (circles.size() > 0)	pBox->SetMainBarShape(circles);
}

#pragma endregion
#pragma region --- ctTieBarShape
void SRSectionTable::SetSectionBoxTieBar(SRSectionBox* pBox, vector<GMObject*> geoms)
{
	vector<GM2DLineVector*> tieBars;
	for (int i = 0; i < geoms.size(); i++)
	{
		GMObject* pGeom = geoms[i];
		GM2DLineVector* line = dynamic_cast<GM2DLineVector*>(pGeom);
		if (line != nullptr)
		{
			if (IsContained(pBox, line))
				tieBars.push_back(line);
		}
	}
	if (tieBars.size() > 0)	pBox->SetTieBarShape(tieBars);
}
#pragma endregion
#pragma region --- ctSubMainBarShape
void SRSectionTable::SetSectionBoxSubMainBar(SRSectionBox* pBox, vector<GMObject*> geoms)
{
	vector<GM2DLineVector*> subMainBars;
	for (int i = 0; i < geoms.size(); i++)
	{
		GMObject* pGeom = geoms[i];
		GM2DLineVector* line = dynamic_cast<GM2DLineVector*>(pGeom);
		if (line != nullptr)
		{
			if (IsContained(pBox, line))
				subMainBars.push_back(line);
		}
	}
	if (subMainBars.size() > 0)	pBox->SetSubMainBarShape(subMainBars);
}
#pragma endregion

