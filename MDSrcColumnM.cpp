#include "StdAfx.h"
#include "MDSrcColumnM.h"
#include "MSSpecificTieBar.h"

#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MDSrcColumnM, MDMember, VERSIONABLE_SCHEMA | 1)
MDSrcColumnM::MDSrcColumnM(void)
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

	m_IsCornerPlacing = true;
}


MDSrcColumnM::~MDSrcColumnM(void)
{
	DeleteSpecificTieBars();
}

void MDSrcColumnM::DeleteSpecificTieBars()
{
	int nSTieBar = ma_SpecificTieBar.size();
	for(int iTiebar = 0; iTiebar < nSTieBar; iTiebar++)
		delete ma_SpecificTieBar[iTiebar];
	ma_SpecificTieBar.clear();
}

void MDSrcColumnM::CopyFromMe(MDMember* pSource)
{
	if(pSource==NULL)	return;
	MDSrcColumnM* pSourceColumnM = (MDSrcColumnM*)pSource;

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
	m_SideBar.CopyFromMe(&pSourceColumnM->m_SideBar);

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

	MDMember::CopyFromMe(pSource);
}

void MDSrcColumnM::Dump(EFS::EFS_Buffer& buffer)
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
	m_SideBar.Dump(buffer);

	int nSTieBar = ma_SpecificTieBar.size();
	buffer << nSTieBar;
	for(int i = 0; i < nSTieBar; i++)
	{
		MSSpecificTieBar* pTieBar = ma_SpecificTieBar[i];
		pTieBar->Dump(buffer);
	}

	m_SubMainBar.Dump(buffer);
	
	buffer << m_IsCornerPlacing;
}

void MDSrcColumnM::Recovery(EFS::EFS_Buffer& buffer)
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
	m_SideBar.Recovery(buffer);

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
}

void MDSrcColumnM::Serialize(CArchive &ar)
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
		m_SideBar.Serialize(ar);

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
		m_SideBar.Serialize(ar);

		if(MSVersionInfo::GetCurrentVersion() >= 20150723)
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
			strBar.Format(_T("0-%s%s"),MSDeformedBar::m_sDefaultBarType, sDiaName);
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
	}
}

long MDSrcColumnM::TotalBarCount()
{
	long TotalBar = m_TBars.GetTotalBars() + m_BBars.GetTotalBars() + m_LBars.GetTotalBars() + m_RBars.GetTotalBars();

	return TotalBar;
}

BOOL MDSrcColumnM::IsPC()
{
	return GetPC();
}
double MDSrcColumnM::GetMaxMainBarDia()
{
	double dMaxDia = MSDeformedBar::GetMinDia();
	vector<double> diaArr = GetMainBarDiaAllLayer();
	for(int iDia = 0 ; iDia < diaArr.size() ; iDia++)
		dMaxDia = max(dMaxDia, diaArr[iDia]);
	return dMaxDia;
}
vector<double> MDSrcColumnM::GetMainBarDiaAllLayer()
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

vector<double> MDSrcColumnM::GetStirrupBarDiaAllLayer()
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

CString MDSrcColumnM::GetLeftRightBarDefaultValue()
{
	CString sBar = _T("");
	CString sDefDiaName = MSDeformedBar::GetDefaultRebarName();

	sBar.Format(_T("2-%s%s"), MSDeformedBar::m_sDefaultBarType,sDefDiaName);

	return sBar;
}

long MDSrcColumnM::GetColTieBarCountDefaultValue()
{
	return 1;
}

CString MDSrcColumnM::GetColRowTieBarDefaultValue()
{
	CString sBar = _T("");
	CString sDefDiaName = MSDeformedBar::GetDefaultRebarName();

	sBar.Format(_T("%s%s@%ld"), MSDeformedBar::m_sDefaultBarType,sDefDiaName, (int)GetColRowTieBarSpacingDefaultValue());

	return sBar;
}

double MDSrcColumnM::GetColRowTieBarSpacingDefaultValue()
{
	return 300;
}