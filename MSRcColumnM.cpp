#include "StdAfx.h"
#include "MSRcColumnM.h"

#include "MRColumnM.h"
#include "MRRcSection.h"
#include "MREdge.h"
#include "MRVBarByNum.h"
#include "MRHBarByEdge.h"

#include "GMLib/GM2DLineVector.h"
#include "GMLib/MSVersionInfo.h"
#include "MSSpecificTieBar.h"


IMPLEMENT_SERIAL(MSRcColumnM, MDMember, VERSIONABLE_SCHEMA | 1)
MSRcColumnM::MSRcColumnM(void)
{
	CString sBar = _T("");	
	sBar = MSDeformedBar::GetMainBarStr(4);
	m_TBars.InitData(sBar);
	m_BBars.InitData(sBar);
	sBar = GetLeftRightBarDefaultValue();
	m_LBars.InitData(sBar);
	m_RBars.InitData(sBar);
	sBar = MSDeformedBar::GetStirrupBarStr(300);
	m_EndHoopBar.InitData(sBar);
	m_CenHoopBar.InitData(sBar);
	m_nDoubleHoopType = 0;
	m_nHoopType = 0;
	m_nTieRowNum = 1;
	m_nTieColNum = GetColTieBarCountDefaultValue();

	m_nDowelSpliceType = 0;               
	m_nDowelSpliceDetail = 0;             
	m_nSpliceLengthA = 500;               
	m_nSpliceLengthB = 800;               
	m_IsPC = FALSE;

	sBar = GetColRowTieBarDefaultValue();
	m_RowEndTieBar.InitData(sBar);
	m_RowCenTieBar.InitData(sBar);
	m_ColEndTieBar.InitData(sBar);
	m_ColCenTieBar.InitData(sBar);

	m_IsCornerPlacing = false;
	m_nColumnMType = ColumnM_TYPE::General;
}

MSRcColumnM::~MSRcColumnM(void)
{
	DeleteSpecificTieBars();
}

void MSRcColumnM::DeleteSpecificTieBars()
{
	int nSTieBar = ma_SpecificTieBar.size();
	for(int iTiebar = 0; iTiebar < nSTieBar; iTiebar++)
		delete ma_SpecificTieBar[iTiebar];
	ma_SpecificTieBar.clear();
}

void MSRcColumnM::CopyFromMe(MDMember* pSource)
{
	if(pSource==NULL)	return;
	MSRcColumnM* pSourceColumnM = (MSRcColumnM*)pSource;

	m_TBars.CopyFromMe(&pSourceColumnM->m_TBars);
	m_BBars.CopyFromMe(&pSourceColumnM->m_BBars);
	m_LBars.CopyFromMe(&pSourceColumnM->m_LBars);
	m_RBars.CopyFromMe(&pSourceColumnM->m_RBars);
	m_EndHoopBar.CopyFromMe(&pSourceColumnM->m_EndHoopBar);
	m_CenHoopBar.CopyFromMe(&pSourceColumnM->m_CenHoopBar);
	m_nDoubleHoopType = pSourceColumnM->m_nDoubleHoopType;
	m_nHoopType = pSourceColumnM->m_nHoopType;
	m_nTieRowNum = pSourceColumnM->m_nTieRowNum;;
	m_nTieColNum = pSourceColumnM->m_nTieColNum;;
	m_nDowelSpliceType = pSourceColumnM->m_nDowelSpliceType;
	m_nDowelSpliceDetail = pSourceColumnM->m_nDowelSpliceDetail;
	m_nSpliceLengthA = pSourceColumnM->m_nSpliceLengthA;
	m_nSpliceLengthB = pSourceColumnM->m_nSpliceLengthB;
	m_IsPC = pSourceColumnM->m_IsPC;
	m_RowEndTieBar.CopyFromMe(&pSourceColumnM->m_RowEndTieBar);
	m_RowCenTieBar.CopyFromMe(&pSourceColumnM->m_RowCenTieBar);
	m_ColEndTieBar.CopyFromMe(&pSourceColumnM->m_ColEndTieBar);
	m_ColCenTieBar.CopyFromMe(&pSourceColumnM->m_ColCenTieBar);

	DeleteSpecificTieBars();
	int numTieBars = pSourceColumnM->ma_SpecificTieBar.size();
	for(int i = 0; i < numTieBars; i++)
	{
		MSSpecificTieBar* pUrTieBar = pSourceColumnM->ma_SpecificTieBar[i];
		MSSpecificTieBar* pMyTieBar = new MSSpecificTieBar();
		pMyTieBar->CopyFromMe(pUrTieBar);
		ma_SpecificTieBar.push_back(pMyTieBar);
	}

	m_SubMainBar.CopyFromMe(&pSourceColumnM->m_SubMainBar);
	m_IsCornerPlacing = pSourceColumnM->m_IsCornerPlacing;

	m_nColumnMType = pSourceColumnM->m_nColumnMType;

	MDMember::CopyFromMe(pSource);
}

void MSRcColumnM::Dump(EFS::EFS_Buffer& buffer)
{
	MDMember::Dump(buffer);
	buffer << m_nDoubleHoopType << m_nHoopType << m_nTieRowNum << m_nTieColNum;
	buffer << m_nDowelSpliceType << m_nDowelSpliceDetail << m_nSpliceLengthA << m_nSpliceLengthB;
	buffer << m_IsPC;

	m_TBars.Dump(buffer);
	m_BBars.Dump(buffer);
	m_LBars.Dump(buffer);
	m_RBars.Dump(buffer);
	m_EndHoopBar.Dump(buffer);
	m_CenHoopBar.Dump(buffer);
	m_RowEndTieBar.Dump(buffer);
	m_ColEndTieBar.Dump(buffer);
	m_RowCenTieBar.Dump(buffer);
	m_ColCenTieBar.Dump(buffer);

	int nSTieBar = ma_SpecificTieBar.size();
	buffer << nSTieBar;
	for(int i = 0; i < nSTieBar; i++)
	{
		MSSpecificTieBar* pTieBar = ma_SpecificTieBar[i];
		pTieBar->Dump(buffer);
	}

	m_SubMainBar.Dump(buffer);
	buffer << m_IsCornerPlacing;
	buffer << m_nColumnMType;
}

void MSRcColumnM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDMember::Recovery(buffer);
	buffer >> m_nDoubleHoopType >> m_nHoopType >> m_nTieRowNum >> m_nTieColNum;
	buffer >> m_nDowelSpliceType >> m_nDowelSpliceDetail >> m_nSpliceLengthA >> m_nSpliceLengthB;
	buffer >> m_IsPC;

	m_TBars.Recovery(buffer);
	m_BBars.Recovery(buffer);
	m_LBars.Recovery(buffer);
	m_RBars.Recovery(buffer);
	m_EndHoopBar.Recovery(buffer);
	m_CenHoopBar.Recovery(buffer);
	m_RowEndTieBar.Recovery(buffer);
	m_ColEndTieBar.Recovery(buffer);
	m_RowCenTieBar.Recovery(buffer);
	m_ColCenTieBar.Recovery(buffer);

	DeleteSpecificTieBars();
	int nSTieBar = 0;
	buffer >> nSTieBar;
	for(int i = 0; i < nSTieBar; i++)
	{
		MSSpecificTieBar* pTieBar = new MSSpecificTieBar();
		pTieBar->Recovery(buffer);
		ma_SpecificTieBar.push_back(pTieBar);
	}

	m_SubMainBar.Recovery(buffer);
	buffer >> m_IsCornerPlacing;
	buffer >> m_nColumnMType;
}

void MSRcColumnM::Serialize(CArchive &ar)
{
	MDMember::Serialize(ar);
	MSMainBar TBars, BBars, LBars, RBars;
	if(ar.IsStoring())
	{
		TBars.Serialize(ar);
		BBars.Serialize(ar);
		LBars.Serialize(ar);
		RBars.Serialize(ar);

		m_EndHoopBar.Serialize(ar);	
		m_CenHoopBar.Serialize(ar);
		ar << m_nDoubleHoopType;
		ar << m_nHoopType;
		ar << m_nTieRowNum;
		ar << m_nTieColNum;
		ar << m_nDowelSpliceType;
		ar << m_nDowelSpliceDetail;
		ar << m_nSpliceLengthA;
		ar << m_nSpliceLengthB;

		// 20120730
		ar << m_IsPC;

		// 20130903 - by Gracias
		m_RowEndTieBar.Serialize(ar);
		m_ColEndTieBar.Serialize(ar);
		// 20130904 - by Gracias
		m_RowCenTieBar.Serialize(ar);
		m_ColCenTieBar.Serialize(ar);

		int nSTieBar = ma_SpecificTieBar.size();
		ar << nSTieBar;

		for(int i = 0; i < nSTieBar; i++)
		{
			MSSpecificTieBar* pTieBar = ma_SpecificTieBar[i];
			pTieBar->Serialize(ar);
		}

		m_SubMainBar.Serialize(ar);
		ar << m_IsCornerPlacing;

		m_TBars.Serialize(ar);
		m_BBars.Serialize(ar);
		m_LBars.Serialize(ar);
		m_RBars.Serialize(ar);

#pragma region 전이 부재
		ar << m_nColumnMType;
#pragma endregion
	}
	else
	{
		TBars.Serialize(ar);
		BBars.Serialize(ar);
		LBars.Serialize(ar);
		RBars.Serialize(ar);

		m_EndHoopBar.Serialize(ar);	
		m_CenHoopBar.Serialize(ar);
		ar >> m_nDoubleHoopType;
		ar >> m_nHoopType;
		ar >> m_nTieRowNum;
		ar >> m_nTieColNum;
		ar >> m_nDowelSpliceType;
		ar >> m_nDowelSpliceDetail;
		ar >> m_nSpliceLengthA;
		ar >> m_nSpliceLengthB;

 		// 20120730
 		ar >> m_IsPC;

		m_RowEndTieBar.Serialize(ar);
		m_ColEndTieBar.Serialize(ar);
		m_RowCenTieBar.Serialize(ar);
		m_ColCenTieBar.Serialize(ar);

		if(MSVersionInfo::GetCurrentVersion() >= 20150718)
		{
			int nSTieBar = 0;
			ar >> nSTieBar;
			for(int i = 0; i <nSTieBar; i++)
			{
				MSSpecificTieBar* pTieBar = new MSSpecificTieBar;
				pTieBar->Serialize(ar);
				ma_SpecificTieBar.push_back(pTieBar);
			}

			m_SubMainBar.Serialize(ar);
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20150805)
			ar >> m_IsCornerPlacing;

		if (LBars.GetDeformedBarDiameter() == 0)
		{
			CString sDiaName = TBars.GetBarDiameterName();
			CString strBar;
			strBar.Format(_T("0-%s%s"), MSDeformedBar::m_sDefaultBarType, sDiaName);
			LBars.InitData(strBar);
			RBars.InitData(strBar);
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20160701)
		{
			m_TBars.Serialize(ar);
			m_BBars.Serialize(ar);
			m_LBars.Serialize(ar);
			m_RBars.Serialize(ar);
		}
		else
		{
			m_TBars.SetDataByOldVersion(&TBars);
			m_BBars.SetDataByOldVersion(&BBars);
			m_LBars.SetDataByOldVersion(&LBars);
			m_RBars.SetDataByOldVersion(&RBars);
		}

		if (MSVersionInfo::GetCurrentVersion() < 20180830)
		{
			if (ma_SpecificTieBar.size() > 0)
			{
				m_nTieRowNum = 0;
				m_nTieColNum = 0;
			}
		}

#pragma region 전이 부재
		if (MSVersionInfo::GetCurrentVersion() >= 20240626)
			ar >> m_nColumnMType;
#pragma endregion
	}
}

long MSRcColumnM::TotalBarCount()
{
	long TotalBar = m_TBars.GetTotalBars() + m_BBars.GetTotalBars() + m_LBars.GetTotalBars() + m_RBars.GetTotalBars();

	return TotalBar;
}

BOOL MSRcColumnM::IsPC()
{
	return GetPC();
}
double MSRcColumnM::GetMaxMainBarDia()
{
	double dMaxDia = MSDeformedBar::GetMinDia();
	vector<double> diaArr = GetMainBarDiaAllLayer();
	for(int iDia = 0 ; iDia < diaArr.size() ; iDia++)
		dMaxDia = max(dMaxDia, diaArr[iDia]);
	return dMaxDia;
}
vector<double> MSRcColumnM::GetMainBarDiaAllLayer()
{
	vector<double> DiaArr;

	vector<double> TBarDiaArr = m_TBars.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), TBarDiaArr.begin(), TBarDiaArr.end());

	vector<double> BBarDiaArr = m_BBars.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), BBarDiaArr.begin(), BBarDiaArr.end());

	vector<double> LBarDiaArr = m_LBars.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), LBarDiaArr.begin(), LBarDiaArr.end());

	vector<double> RBarDiaArr = m_RBars.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), RBarDiaArr.begin(), RBarDiaArr.end());

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const long& a, const long& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MSRcColumnM::GetStirrupBarDiaAllLayer()
{
	vector<double> DiaArr;

	vector<double> EndHoopBarDiaArr = m_EndHoopBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), EndHoopBarDiaArr.begin(), EndHoopBarDiaArr.end());

	vector<double> CenHoopBarDiaArr = m_CenHoopBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), EndHoopBarDiaArr.begin(), EndHoopBarDiaArr.end());

	vector<double> RowEndTieBarDiaArr = m_RowEndTieBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), RowEndTieBarDiaArr.begin(), RowEndTieBarDiaArr.end());

	vector<double> RowCenTieBarDiaArr = m_RowCenTieBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), RowCenTieBarDiaArr.begin(), RowCenTieBarDiaArr.end());

	vector<double> ColEndTieBarDiaArr = m_ColEndTieBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), ColEndTieBarDiaArr.begin(), ColEndTieBarDiaArr.end());

	vector<double> ColCenTieBarDiaArr = m_ColCenTieBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), ColCenTieBarDiaArr.begin(), ColCenTieBarDiaArr.end());

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const double& a, const double& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

CString MSRcColumnM::GetLeftRightBarDefaultValue()
{
	CString sBar = _T("");
	CString sDefDiaName = MSDeformedBar::GetDefaultRebarName();

	sBar.Format(_T("2-%s%s"), MSDeformedBar::m_sDefaultBarType, sDefDiaName);

	return sBar;
}

long MSRcColumnM::GetColTieBarCountDefaultValue()
{
	return 1;
}

CString MSRcColumnM::GetColRowTieBarDefaultValue()
{
	CString sBar = _T("");
	CString sDefDiaName = MSDeformedBar::GetDefaultRebarName();

	sBar.Format(_T("%s%s@%ld"), MSDeformedBar::m_sDefaultBarType, sDefDiaName, (int)GetColRowTieBarSpacingDefaultValue());

	return sBar;
}

double MSRcColumnM::GetColRowTieBarSpacingDefaultValue()
{
	return 300;
}

// void MSRcColumnM::ConvertToMRM()
// {
// 	if(mp_MRM)
// 		delete mp_MRM;
// 
// 	mp_MRM = new MRColumnM;
// 
// 	MRRcSection* pSect = new MRRcSection;
// 	GMPolygon SectPGon(true);
// 	GetSecPolygon(SectPGon);
// 	GM2DLineVector* pEdgeLine;
// 
// // 에지 만들기
// 	if(me_Shape==RECT)
// 	{
// 		long numLine = SectPGon.ma_Vector.GetSize();
// 		if(numLine != 4)
// 			ASSERT(0);
// 		// 주근 배근
// 		for(long iLine = 0;iLine<numLine;iLine++)
// 		{
// 			GMLineVector EdgeLine = SectPGon.GetLine(iLine);
// 			MREdge* pEdge = new MREdge;
// 			pEdgeLine = new GM2DLineVector(EdgeLine.m_P0.m_X,EdgeLine.m_P0.m_Y,EdgeLine.m_P1.m_X,EdgeLine.m_P1.m_Y);
// 			pEdge->mp_Curve = pEdgeLine;
// 			MSMainBar BarInfo;
// 			if(iLine == 0)//Bot
// 				BarInfo = m_BBars;
// 			else if(iLine == 1)//R
// 				BarInfo = m_RBars;
// 			else if(iLine == 2)//Top
// 				BarInfo = m_TBars;
// 			else if(iLine == 3)//L
// 				BarInfo = m_LBars;
// 			long numLayer = BarInfo.GetNumLayers();
// 			for(int iLayer = 0;iLayer<numLayer;iLayer++)
// 			{
// 				MRVBar* pRebar = new MRVBar;
// 				if((iLine == 1 || iLine == 3))
// 					pRebar->Set_bHasCornerBar(false);
// 				pRebar->SetNumBar(BarInfo.GetNumBarsOnALayer(iLayer));
// 				pRebar->m_LayerNum = iLayer+1;
// 				pRebar->mp_Edge = pEdge;
// 				pEdge->AddVBarSet(pRebar);
// 			}
// 			pSect->AddEdge(pEdge);
// 		}
// 		//스터럽 배근
// // 		MSStirrupBar m_EndHoopBar;  //-> 콤포저로 처리?	
// 		pSect->SetStirrupTempletID(0);//Default
// 		if(fabs(m_EndHoopBar.GetSpacing()-m_CenHoopBar.GetSpacing())<DTOL_GM)//비내진기둥
// 			pSect->m_StirrupSpace = m_EndHoopBar.GetSpacing();
// 		else//간격이 다른경우 내진
// 		{
// 			 //-> 콤포저로 처리?	
// 		}			
// 		// 타이바 배근
// // 		long m_nTieRowNum;                  //long m_XStirrupNum;  X축방향 Stirrup수 
// // 		long m_nTieColNum;									//long m_YStirrupNum;  Y축방향 Stirrup수
// 	}
// 	pSect->SetBarLine4Edge(GetCover(),(double)m_BBars.GetBarDiaIdxOnALayer(),(double)m_EndHoopBar.GetBarDia());
//  	mp_MRM->ma_Section.Add(pSect);
// }

#pragma region 전이 부재
int MSRcColumnM::GetColumnMType()
{
	return m_nColumnMType;
}

void MSRcColumnM::SetColumnMType(int nType)
{
	m_nColumnMType = nType;
}

bool MSRcColumnM::IsTransfer()
{
	if (m_nColumnMType == ColumnM_TYPE::Transfer)
		return true;

	return false;
}
#pragma endregion