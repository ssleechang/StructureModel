#include "StdAfx.h"
#include "SRTable.h"

#include "GMLib/GMText.h"
#include "GMLib/GM2DRectangle.h"
#include "MTReader.h"


SRTable::SRTable(void)
{
	mp_Reader = NULL;
	m_MinX = 1.E20;
	m_Row1Type = 0;
	m_Row2Type = 0;
	m_Col1Type = 0;
	m_Col2Type = 0;
	m_RemainText = 0;
	m_NumBoxRow = 1;
}

SRTable::~SRTable(void)
{
	DeleteLineArr();
	DeleteTextArr();
	DeleteHLineArr();
	DeleteVLineArr();
	DeleteBoxArr();
}
void SRTable::DeleteLineArr()
{
	long nLine = ma_Line.GetSize();
	for(long iLine=0; iLine<nLine; iLine++)
		delete ma_Line[iLine];
	ma_Line.RemoveAll();
}
void SRTable::DeleteTextArr()
{
	long nText = ma_Text.GetSize();
	for(long iText=0; iText<nText; iText++)
		delete ma_Text[iText];
	ma_Text.RemoveAll();
}
void SRTable::DeleteHLineArr()
{
	long nHLine = ma_HLine.GetSize();
	for(long iHLine=0; iHLine<nHLine; iHLine++)
		delete ma_HLine[iHLine];
	ma_HLine.RemoveAll();
}
void SRTable::DeleteVLineArr()
{
	long nVLine = ma_VLine.GetSize();
	for(long iVLine=0; iVLine<nVLine; iVLine++)
		delete ma_VLine[iVLine];
	ma_VLine.RemoveAll();
}
void SRTable::DeleteBoxArr()
{
	long nBox = ma_Box.GetSize();
	for(long iBox=0; iBox<nBox; iBox++)
		delete ma_Box[iBox];
	ma_Box.RemoveAll();
}
void SRTable::SetMinimumTolerence(CTypedPtrArray<CObArray, GMText*>& GMTextArr)
{
	int nText = GMTextArr.GetSize();
	m_TOL_MinCellHeight = 1.E20;
	for(int iText=0; iText<nText; iText++)
	{
		GMText* pText = GMTextArr[iText];
		if(pText->m_dHeight < m_TOL_MinCellHeight)
			m_TOL_MinCellHeight = pText->m_dHeight;
	}
	m_TOL_MinCellWidth = m_TOL_MinCellHeight*3.;
}

BOOL SRTable::SetupFrame( CTypedPtrArray<CObArray, GM2DLineVector*>& GMLineArr )
{
	Init();

	m_MaxX = -1.E20;
	m_MinX = 1.E20;
	m_MaxY = -1.E20;
	m_MinY = 1.E20;

	// m_TOL_MinCellHeight, m_TOL_MinCellWidth 값을 text 사이즈로 세팅
	// 	SetMinimumTolerence();  

	long NumLine = GMLineArr.GetSize();
	for(long iLine=0; iLine<NumLine; iLine++)
	{
		GM2DLineVector* pLine = GMLineArr[iLine];
		if(IsValidLine(pLine)==FALSE)  // 하위 테이블클래스 가상함수에서 무의미한 라인 제거
			continue;

		if(pLine->IsHorizontalLine())
		{
			if(pLine->m_P0.m_X > pLine->m_P1.m_X)
				pLine->ReverseDirection();

			if(pLine->m_P0.m_X < m_MinX) m_MinX = pLine->m_P0.m_X;
			if(pLine->m_P1.m_X > m_MaxX) m_MaxX = pLine->m_P1.m_X;

			int nLoc = ma_YLoc.GetSize();
			for(int iLoc=0; iLoc<nLoc; iLoc)
			{
				if(fabs(ma_YLoc[iLoc]-pLine->m_P0.m_Y) < m_TOL_MinCellHeight)
					continue;
				if(pLine->m_P0.m_Y > ma_YLoc[iLoc])
				{
					ma_YLoc.InsertAt(iLoc, pLine->m_P0.m_Y);
					ma_HLine.InsertAt(iLoc, new SRTableLine(pLine));
					break;
				}
			}
			if(iLoc==nLoc)
			{
				ma_YLoc.Add(pLine->m_P0.m_Y);
				ma_HLine.Add(new SRTableLine(pLine));
			}
		}

		else if(pLine->IsVerticalLine())
		{
			if(pLine->m_P0.m_Y < pLine->m_P1.m_Y)
				pLine->ReverseDirection();

			if(pLine->m_P0.m_Y < m_MinY) m_MinY = pLine->m_P0.m_Y;
			if(pLine->m_P1.m_Y > m_MaxY) m_MaxY = pLine->m_P1.m_Y;

			int nLoc = ma_XLoc.GetSize();
			for(int iLoc=0; iLoc<nLoc; iLoc)
			{
				if(fabs(ma_XLoc[iLoc]-pLine->m_P0.m_X) < m_TOL_MinCellWidth)
					continue;
				if(pLine->m_P0.m_X < ma_XLoc[iLoc])
				{
					ma_XLoc.InsertAt(iLoc, pLine->m_P0.m_X);
					ma_VLine.InsertAt(iLoc, new SRTableLine(pLine));
					break;
				}
			}
			if(iLoc==nLoc)
			{
				ma_XLoc.Add(pLine->m_P0.m_X);
				ma_VLine.Add(new SRTableLine(pLine));
			}
		}
	}

	if(ma_XLoc[0]-m_MinX > m_TOL_MinCellWidth)
	{
		ma_XLoc.InsertAt(0, m_MinX);
		ma_VLine.InsertAt(0, new SRTableLine(new GM2DLineVector(m_MinX, m_MaxY, m_MinX, m_MinY)));
	}

	if(m_MaxY-ma_YLoc[0] > m_TOL_MinCellHeight)
	{
		ma_YLoc.InsertAt(0, m_MaxY);
		ma_HLine.InsertAt(0, new SRTableLine(new GM2DLineVector(m_MinX, m_MaxY, m_MaxX, m_MaxY)));
	}

	int nXLoc = ma_XLoc.GetSize();
	if(m_MaxX-ma_XLoc[nXLoc-1] > m_TOL_MinCellWidth)
	{
		ma_XLoc.Add(m_MaxX);
		ma_VLine.Add(new SRTableLine(new GM2DLineVector(m_MaxX, m_MaxY, m_MaxX, m_MinY)));
	}
	int nYLoc = ma_YLoc.GetSize();
	if(ma_YLoc[nYLoc-1]-m_MinY >m_TOL_MinCellHeight)
	{
		ma_YLoc.Add(m_MinY);
		ma_HLine.Add(new SRTableLine(new GM2DLineVector(m_MinX, m_MinY, m_MaxX, m_MinY)));
	}


	return TRUE;
}

long SRTable::SortValueArr(CArray<double, double>& ValueArr, BOOL bMinOrder)
{
	long NumValue = ValueArr.GetSize();
	for(long iValue=0; iValue<NumValue; iValue++)
	{
		for(long jValue=iValue+1; jValue<NumValue; jValue++)
		{
			if(bMinOrder)
			{
				if(ValueArr[iValue] > ValueArr[jValue])
				{
					double dTemp = ValueArr[iValue];
					ValueArr[iValue] = ValueArr[jValue];
					ValueArr[jValue] = dTemp;
				}
			}
			else
			{
				if(ValueArr[iValue] < ValueArr[jValue])
				{
					double dTemp = ValueArr[iValue];
					ValueArr[iValue] = ValueArr[jValue];
					ValueArr[jValue] = dTemp;
				}
			}
		}
	}
	return ValueArr.GetSize();
}

void SRTable::Init()
{
	DeleteHLineArr();
	DeleteVLineArr();
	DeleteBoxArr();
	DeleteLineArr();
	DeleteTextArr();
	ma_XLoc.RemoveAll();
	ma_YLoc.RemoveAll();

	m_MinX = 1.E20;
	m_Row1Type = 0;
	m_Row2Type = 0;
	m_Col1Type = 0;
	m_Col2Type = 0;
	m_RemainText = 0;
	m_NumBoxRow = 1;

	// 	ma_ColGroupLoc.RemoveAll();
	// 	ma_RowGroupLoc.RemoveAll();
	// 	ma_ColGroupIdx.RemoveAll();
	// 	ma_RowGroupIdx.RemoveAll();
}

BOOL SRTable::IsValidLine( GM2DLineVector* pLine )
{
	// 	if(pLine->Length() < m_MIN_VALIDLINE)
	// 		return FALSE;
	return TRUE;
}

BOOL SRTable::ClassifyVLines()
{
	return TRUE;
}

BOOL SRTable::ClassifyHLines()
{
	return TRUE;
}

bool SRTable::AddLine(double x1, double y1, double x2, double y2)
{
	GM2DVector SVec(x1, y1);
	GM2DVector EVec(x2, y2);

	GM2DLineVector* pLineVec = new GM2DLineVector(SVec, EVec);
	ma_Line.Add(pLineVec);

	if(SVec.m_X < m_MinX)
		m_MinX = SVec.m_X;
	if(EVec.m_X < m_MinX)
		m_MinX = EVec.m_X;

	return true;
}
bool SRTable::AddText(double cenX, double cenY, CString text, double degree, double height)
{
	GM2DVector CenPnt(cenX, cenY);
	long nCellType=0;
	nCellType = mp_Reader->GetCellTypebyDictionary(text);
	if(nCellType != 0)
	{
		GMText* GMTxt = new GMText();
		GMTxt->m_strText = text;
		GMTxt->m_Vec = CenPnt;
		GMTxt->m_dDegree = degree*Rad2Deg;
		GMTxt->m_dHeight = height;
		long nText = ma_Text.GetSize();
		for(long iText=0; iText<nText; iText++)
		{
			GMText* pOldText = ma_Text[iText];
			if(GMTxt->m_Vec.m_Y < pOldText->m_Vec.m_Y)
			{
				ma_Text.InsertAt(iText, GMTxt);
				break;
			}
		}
		if(iText==nText)
			ma_Text.Add(GMTxt);
		return true;
	}
	else
	{
		GMText* GMTxt = new GMText();
		GMTxt->m_strText = text;
		GMTxt->m_Vec = CenPnt;
		GMTxt->m_dDegree = degree*Rad2Deg;
		GMTxt->m_dHeight = height;
		long nText = ma_EtcText.GetSize();
		for(long iText=0; iText<nText; iText++)
		{
			GMText* pOldText = ma_EtcText[iText];
			if(GMTxt->m_Vec.m_Y < pOldText->m_Vec.m_Y)
			{
				ma_EtcText.InsertAt(iText, GMTxt);
				break;
			}
		}
		if(iText==nText)
			ma_EtcText.Add(GMTxt);
		return true;
	}
	return false;
}

long SRTable::MakeSectionTable()
{
	m_NumBoxRow = 1;
	long nText = ma_Text.GetSize();
	if(nText==0)	return false;
	double MinTextX = 1.E20;
	GMText* pLeftText = NULL;
	for(long iText=0; iText<nText; iText++)
	{
		if(ma_Text[iText]->m_Vec.m_X < MinTextX)
		{
			MinTextX = ma_Text[iText]->m_Vec.m_X;
			pLeftText = ma_Text[iText];
		}
	}
	if (pLeftText == nullptr)	return 0;

	for(long iText=nText-1; iText>=0; iText--)
	{
		double LabelBound = MinTextX + pLeftText->m_dHeight*4.;
		if(ma_Text[iText]->m_Vec.m_X > LabelBound)
			ma_Text.RemoveAt(iText);
	}
	nText = ma_Text.GetSize();
	if(nText < 2)	return false;
	GMText* pFirstText = ma_Text[0];
	CString FirstText = pFirstText->m_strText;
	for(long iText=1; iText<nText; iText++)
	{
		if(ma_Text[iText]->m_strText == FirstText)
			m_NumBoxRow++;
	}
	m_RemainText = nText%m_NumBoxRow;
	long NumCellInABox = nText/m_NumBoxRow;
	// 	if(nText <= NumCellInABox)	NumCellInABox = nText-1;

	GMText* pSecondText = ma_Text[1];
	double Cell1Y = ma_Text[1]->m_Vec.m_Y;
	double Cell0Y = ma_Text[0]->m_Vec.m_Y;
	double CellHeight = ma_Text[1]->m_Vec.m_Y - ma_Text[0]->m_Vec.m_Y;
	m_CellHeight = CellHeight;

	if(m_NumBoxRow > 1)
		m_BoxHeight = ma_Text[NumCellInABox]->m_Vec.m_Y - ma_Text[0]->m_Vec.m_Y;
	else if(m_NumBoxRow==1)
		m_BoxHeight = ma_Text[NumCellInABox-1]->m_Vec.m_Y - ma_Text[0]->m_Vec.m_Y + CellHeight;
	else
		return 0;

	CTypedPtrArray<CObArray, GM2DLineVector*> HLineArr;
	CTypedPtrArray<CObArray, GM2DLineVector*> VLineArr;
	double distTolerance = 1.0;
	double LinearDistTol = m_BoxHeight/100.;
	long nLine = ma_Line.GetSize();
	for(long iLine=0; iLine<nLine; iLine++)
	{
		if(ma_Line[iLine]->IsHorizontalLine())
			HLineArr.Add(ma_Line[iLine]);
		else if(ma_Line[iLine]->IsVerticalLine())
			VLineArr.Add(ma_Line[iLine]);
	}

	nLine = HLineArr.GetSize();
	for(long iLine=nLine-1; iLine>=0; iLine--)
	{
		for(long jLine=0; jLine<iLine-1; jLine++)
		{
			BOOL bColinear = HLineArr[iLine]->MergeWhenColinear(HLineArr[jLine], 1., distTolerance, LinearDistTol);
			if(bColinear)
			{
				HLineArr.RemoveAt(jLine);
				break;
			}
		}
	}
	nLine = VLineArr.GetSize();
	for(long iLine=nLine-1; iLine>=0; iLine--)
	{
		for(long jLine=0; jLine<iLine; jLine++)
		{
			BOOL bColinear = VLineArr[iLine]->MergeWhenColinear(VLineArr[jLine], 1., distTolerance, LinearDistTol);
			if(bColinear)
			{
				VLineArr.RemoveAt(jLine);
				break;
			}
		}
	}

	ma_XLoc.RemoveAll();
	ma_YLoc.RemoveAll();
	SetMinimumTolerence(ma_Text);

	m_MaxX = -1.E20;
	m_MinX = 1.E20;
	m_MaxY = -1.E20;
	m_MinY = 1.E20;

	long nVert = VLineArr.GetSize();
	for(long iVert=0; iVert<nVert; iVert++)
	{
		if(VLineArr[iVert]->Length() > m_BoxHeight*0.98)
		{
			GM2DLineVector* pLine = VLineArr[iVert];
			if(pLine->m_P0.m_Y > pLine->m_P1.m_Y)
				pLine->ReverseDirection();

			if(pLine->m_P0.m_Y < m_MinY) m_MinY = pLine->m_P0.m_Y;
			if(pLine->m_P1.m_Y > m_MaxY) m_MaxY = pLine->m_P1.m_Y;

			int nLoc = ma_XLoc.GetSize();
			for(int iLoc=0; iLoc<nLoc; iLoc++)
			{
				if(pLine->m_P0.m_X < ma_XLoc[iLoc])
				{
					ma_XLoc.InsertAt(iLoc, pLine->m_P0.m_X);
					ma_VLine.InsertAt(iLoc, new SRTableLine(pLine));
					break;
				}
			}
			if(iLoc==nLoc)
			{
				ma_XLoc.Add(pLine->m_P0.m_X);
				ma_VLine.Add(new SRTableLine(pLine));
			}
		}
	}

	long nHori = HLineArr.GetSize();
	for(long iHori=0; iHori<nHori; iHori++)
	{
		GM2DLineVector* pLine = HLineArr[iHori];
		double LineY = HLineArr[iHori]->m_P0.m_Y;
		for(long iText=0; iText<nText; iText+=NumCellInABox)
		{
			GMText* pText = ma_Text[iText];
			double dY = pText->m_Vec.m_Y;
			if( (iText<nText && (dY-LineY) < CellHeight && (dY-LineY) > 0.) ||
				(iText==nText && (LineY-dY) < CellHeight && (LineY-dY) > 0.) )

// 			if( (iText<nText && (ma_Text[iText]->m_Vec.m_Y-LineY) < CellHeight && (ma_Text[iText]->m_Vec.m_Y-LineY) > 0.) ||
// 				(iText==nText && (LineY-ma_Text[iText-1]->m_Vec.m_Y) < CellHeight && (LineY-ma_Text[iText-1]->m_Vec.m_Y) > 0.) )
			{
				if(pLine->m_P0.m_X > pLine->m_P1.m_X)
					pLine->ReverseDirection();

				if(pLine->m_P0.m_X < m_MinX) m_MinX = pLine->m_P0.m_X;
				if(pLine->m_P1.m_X > m_MaxX) m_MaxX = pLine->m_P1.m_X;

				int nLoc = ma_YLoc.GetSize();
				for(int iLoc=0; iLoc<nLoc; iLoc++)
				{
					if(pLine->m_P0.m_Y > ma_YLoc[iLoc])
					{
						ma_YLoc.InsertAt(iLoc, pLine->m_P0.m_Y);
						ma_HLine.InsertAt(iLoc, new SRTableLine(pLine));
						break;
					}
				}
				if(iLoc==nLoc)
				{
					ma_YLoc.Add(pLine->m_P0.m_Y);
					ma_HLine.Add(new SRTableLine(pLine));
				}
				break;
			}
		}
	}

	nHori = ma_HLine.GetSize();
	nVert = ma_VLine.GetSize();


	long nYLoc = ma_YLoc.GetSize();
	long nXLoc = ma_XLoc.GetSize();
	if(nYLoc == 0 || nXLoc == 0)	return false;

	double dMaxYLoc = ma_YLoc[0];
	if(fabs(dMaxYLoc - m_MaxY) > CellHeight*0.5)
	{
		GM2DLineVector* pMaxLVec = new GM2DLineVector(m_MinX, m_MaxY, m_MaxX, m_MaxY);
		ma_HLine.InsertAt(0, new SRTableLine(pMaxLVec));
		ma_YLoc.InsertAt(0, m_MaxY);
		nYLoc = ma_YLoc.GetSize();
	}
	double dMinYLoc = ma_YLoc[nYLoc-1];
	if(fabs(dMinYLoc - m_MinY) > CellHeight*0.5)
	{
		GM2DLineVector* pMinLVec = new GM2DLineVector(m_MinX, m_MinY, m_MaxX, m_MinY);
		ma_HLine.Add(new SRTableLine(pMinLVec));
		ma_YLoc.Add(m_MinY);
	}

	double dMinXLoc = ma_XLoc[0];
	if(fabs(dMinXLoc-m_MinX) > CellHeight)
	{
		GM2DLineVector* pMinLVec = new GM2DLineVector(m_MinX, m_MaxY, m_MinX, m_MinY);
		ma_VLine.InsertAt(0, new SRTableLine(pMinLVec));
		ma_XLoc.InsertAt(0, m_MinX);
	}
	double dMaxXLoc = ma_XLoc[nXLoc-1];
	if(fabs(dMaxXLoc - m_MaxX) > CellHeight)
	{
		GM2DLineVector* pMaxLVec = new GM2DLineVector(m_MaxX, m_MaxY, m_MaxX, m_MinY);
		ma_VLine.Add(new SRTableLine(pMaxLVec));
		ma_XLoc.Add(m_MaxX);
	}

	nHori = ma_HLine.GetSize();
	for(long iHori=0; iHori<nHori; iHori++)
	{
		SRTableLine* pHLine = ma_HLine[iHori];
		long nVert = ma_VLine.GetSize();
		for(long iVert=0; iVert<nVert; iVert++)
			ma_HLine[iHori]->CheckCrossWithVerticalLine(ma_VLine[iVert], CellHeight*0.5, CellHeight*3.);
	}

	MakeSectionBox();
	return true;
}

long SRTable::MakeSectionBox()
{
	long nVLine = ma_VLine.GetSize();
	if(nVLine < 2)	return 0;
	double nCellTypeWidth = ma_VLine[1]->mp_LVec->m_P0.m_X-ma_VLine[0]->mp_LVec->m_P0.m_X;

	long nHLine = ma_HLine.GetSize();
	for(long iHLine=nHLine-1; iHLine>=0; iHLine--)
	{
		SRTableLine* pHLine = ma_HLine[iHLine];
		SRTableLine* pLastHLine = NULL;
		if(m_RemainText && iHLine==2)
			pLastHLine = ma_HLine[0];
		long nTopLine = pHLine->ma_TopLeftLine.GetSize();
		for(long iTop=0; iTop<nTopLine-1; iTop++)
		{
			SRTableLine* pLeftVLine = ma_HLine[iHLine]->ma_TopLeftLine[iTop];
			SRTableLine* pRightVLine = ma_HLine[iHLine]->ma_TopLeftLine[iTop+1];
			long nLeftLine = pRightVLine->ma_TopLeftLine.GetSize();
			for(long iLeft=0; iLeft<nLeftLine; iLeft++)
			{
				if(pRightVLine->ma_TopLeftLine[iLeft] == ma_HLine[iHLine])
					break;
			}
			if(iLeft==0 || iLeft==nLeftLine)
				continue;
			GM2DRectangle* pRect = new GM2DRectangle();
			SRTableLine* pTopHLine = pRightVLine->ma_TopLeftLine[iLeft-1];
			pRect->m_Origin.m_X = pLeftVLine->mp_LVec->m_P0.m_X;
			pRect->m_Origin.m_Y = ma_HLine[iHLine]->mp_LVec->m_P0.m_Y;
			pRect->m_Corner.m_X = pRightVLine->mp_LVec->m_P0.m_X;
			if(pLastHLine)
				pRect->m_Corner.m_Y = pLastHLine->mp_LVec->m_P0.m_Y;
			else
				pRect->m_Corner.m_Y = pTopHLine->mp_LVec->m_P0.m_Y;

			if(pRect->Width() > nCellTypeWidth+m_CellHeight && pRect->Height() > (m_CellHeight*m_NumBoxRow)*0.9)
				ma_Box.Add(pRect);
			else
				delete pRect;
		}
		pLastHLine = NULL;
	}
	return ma_Box.GetSize();
}

int SRTable::GetBoxCount()
{
	return ma_Box.GetSize();
}

SRTableLine::SRTableLine( GM2DLineVector* pLVec )
{
	mp_LVec = new GM2DLineVector;
	mp_LVec->m_P0 = pLVec->m_P0;
	mp_LVec->m_P1 = pLVec->m_P1;
}

bool SRTableLine::CheckCrossWithVerticalLine( SRTableLine* pVLine, double MinHeight, double MinWidth )
{
	double DistTol = MinHeight/10.;
	double MyY = mp_LVec->m_P0.m_Y;
	double MyLeftX = min(mp_LVec->m_P0.m_X, mp_LVec->m_P1.m_X);
	double MyRightX = max(mp_LVec->m_P0.m_X, mp_LVec->m_P1.m_X);

	double UrX = pVLine->mp_LVec->m_P0.m_X;
	double UrTopY = max(pVLine->mp_LVec->m_P0.m_Y, pVLine->mp_LVec->m_P1.m_Y);
	double UrBotY = min(pVLine->mp_LVec->m_P0.m_Y, pVLine->mp_LVec->m_P1.m_Y);

	if(MyY >= (UrBotY-DistTol) && MyY <= (UrTopY-MinHeight) && UrX >= (MyLeftX-DistTol) && UrX <= (MyRightX+DistTol))
		ma_TopLeftLine.Add(pVLine);

	if(UrX >= (MyLeftX+MinWidth) && UrX <= (MyRightX+DistTol) && MyY >= (UrBotY-DistTol) && MyY <= (UrTopY+DistTol))
		pVLine->ma_TopLeftLine.Add(this);

	return true;
}


