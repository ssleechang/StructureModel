#include "StdAfx.h"
#include "MSRcLevelDifM.h"

IMPLEMENT_SERIAL(MSRcLevelDifM, MDMember, VERSIONABLE_SCHEMA | 1)
MSRcLevelDifM::MSRcLevelDifM(void)
{

	m_dCvrConc = 40;
	m_IsHaunch = FALSE;
	m_HookType = 0;
	m_ObjectType = 0;
	m_PlaceBarType = 0;
	m_dLevel = 1000;

	CString sStirrupBar = MSDeformedBar::GetStirrupBarStr(300, 13);
	CString sMainBar = MSDeformedBar::GetMainBarStr(4, 13);

	m_InHBar.InitData(sStirrupBar);
	m_InVBar.InitData(sStirrupBar);
	m_OutHBar.InitData(sStirrupBar);
	m_OutVBar.InitData(sStirrupBar);

	m_TBars.InitData(sMainBar);
	m_BBars.InitData(sMainBar);
	m_SideBars.InitData(sMainBar);
	m_StirrupBar.InitData(sStirrupBar);
	m_SBarNum = 2;

	m_CoverShapeCode = MST1_06;
	m_StirrupShapeCode = MST3_00;
}

MSRcLevelDifM::~MSRcLevelDifM(void)
{
}

void MSRcLevelDifM::CopyFromMe(MDMember* pSource)
{
	if(pSource==NULL)	return;
	MSRcLevelDifM * pSourceDifM = (MSRcLevelDifM*)pSource;
	m_IsHaunch = pSourceDifM->m_IsHaunch;
	m_HookType = pSourceDifM->m_HookType;
	m_ObjectType = pSourceDifM->m_ObjectType;
	m_PlaceBarType = pSourceDifM->m_PlaceBarType;
	m_dLevel = pSourceDifM->m_dLevel;

	m_InHBar.CopyFromMe(&pSourceDifM->m_InHBar);
	m_InVBar.CopyFromMe(&pSourceDifM->m_InVBar);
	m_OutHBar.CopyFromMe(&pSourceDifM->m_OutHBar);
	m_OutVBar.CopyFromMe(&pSourceDifM->m_OutVBar);

	m_TBars.CopyFromMe(&pSourceDifM->m_TBars);
	m_BBars.CopyFromMe(&pSourceDifM->m_BBars);
	m_SideBars.CopyFromMe(&pSourceDifM->m_SideBars);
	m_StirrupBar.CopyFromMe(&pSourceDifM->m_StirrupBar);

	m_SBarNum = pSourceDifM->m_SBarNum;

	m_CoverShapeCode = pSourceDifM->m_CoverShapeCode;
	m_StirrupShapeCode = pSourceDifM->m_StirrupShapeCode;

	MDMember::CopyFromMe(pSource);
}

void MSRcLevelDifM::Dump(EFS::EFS_Buffer& buffer)
{
	MDMember::Dump(buffer);

	buffer << m_IsHaunch << m_HookType << m_PlaceBarType << m_dLevel;
	buffer << m_SBarNum << m_CoverShapeCode << m_StirrupShapeCode;
	m_InHBar.Dump(buffer);
	m_InVBar.Dump(buffer);
	m_OutHBar.Dump(buffer);
	m_OutVBar.Dump(buffer);
	m_TBars.Dump(buffer);
	m_BBars.Dump(buffer);
	m_SideBars.Dump(buffer);
	m_StirrupBar.Dump(buffer);
}

void MSRcLevelDifM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDMember::Recovery(buffer);

	buffer >> m_IsHaunch >> m_HookType >> m_PlaceBarType >> m_dLevel;
	buffer >> m_SBarNum >> m_CoverShapeCode >> m_StirrupShapeCode;
	m_InHBar.Recovery(buffer);
	m_InVBar.Recovery(buffer);
	m_OutHBar.Recovery(buffer);
	m_OutVBar.Recovery(buffer);
	m_TBars.Recovery(buffer);
	m_BBars.Recovery(buffer);
	m_SideBars.Recovery(buffer);
	m_StirrupBar.Recovery(buffer);
}

void MSRcLevelDifM::Serialize(CArchive &ar)
{
	MDMember::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_IsHaunch;
		ar << m_HookType;
		ar << m_ObjectType;
		ar << m_PlaceBarType;
		ar << m_dLevel;

		m_InHBar.Serialize(ar);
		m_InVBar.Serialize(ar);
		m_OutHBar.Serialize(ar);
		m_OutVBar.Serialize(ar);
	 
		m_TBars.Serialize(ar);
	 	m_BBars.Serialize(ar);
	 	m_SideBars.Serialize(ar);
		m_StirrupBar.Serialize(ar);

		ar << m_SBarNum;
		ar << m_CoverShapeCode;
		ar << m_StirrupShapeCode;
	}
	else
	{
		ar >> m_IsHaunch;
		ar >> m_HookType;
		ar >> m_ObjectType;
		ar >> m_PlaceBarType;
		ar >> m_dLevel;

		m_InHBar.Serialize(ar);
		m_InVBar.Serialize(ar);
		m_OutHBar.Serialize(ar);
		m_OutVBar.Serialize(ar);

		m_TBars.Serialize(ar);
		m_BBars.Serialize(ar);
		m_SideBars.Serialize(ar);
		m_StirrupBar.Serialize(ar);

		ar >> m_SBarNum;
		long CoverShapeCode = 0, StirrupShapeCode = 0;

		ar >> CoverShapeCode;
		ar >> StirrupShapeCode;
		m_CoverShapeCode = (MSREBAR_SHAPECODE)CoverShapeCode;
		m_StirrupShapeCode = (MSREBAR_SHAPECODE)StirrupShapeCode;
	}
}

vector<double> MSRcLevelDifM::GetMainBarDiaAllLayer()
{
	vector<double> DiaArr;

	if(m_ObjectType == 0)//0:벽처럼배근 1: 보처럼
	{
		vector<double> InHBarDiaArr = m_InHBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), InHBarDiaArr.begin(), InHBarDiaArr.end());

		vector<double> InVBarDiaArr = m_InVBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), InVBarDiaArr.begin(), InVBarDiaArr.end());

		vector<double> OutHBarDiaArr = m_OutHBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), OutHBarDiaArr.begin(), OutHBarDiaArr.end());

		vector<double> OutVBarDiaArr = m_OutVBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), OutVBarDiaArr.begin(), OutVBarDiaArr.end());
	}
	else if(m_ObjectType == 1)
	{
		vector<double> TBarDiaArr = m_TBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), TBarDiaArr.begin(), TBarDiaArr.end());

		vector<double> BBarDiaArr = m_BBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), BBarDiaArr.begin(), BBarDiaArr.end());

		vector<double> SideDiaArr = m_SideBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), SideDiaArr.begin(), SideDiaArr.end());
	}
	else
		ASSERT(0);

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const double& a, const double& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MSRcLevelDifM::GetStirrupBarDiaAllLayer()
{
	vector<double> DiaArr;

	if(m_ObjectType == 0)//0:벽처럼배근 1: 보처럼
	{

	}
	else if(m_ObjectType == 1)
	{
		vector<double> StirrupDiaArr = m_StirrupBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), StirrupDiaArr.begin(), StirrupDiaArr.end());
	}
	else
		ASSERT(0);

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const double& a, const double& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}
