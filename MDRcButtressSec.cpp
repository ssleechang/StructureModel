#include "StdAfx.h"
#include "MDRcButtressSec.h"
#include "MSDeformedBar.h"
#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MDRcButtressSec, MSObject, VERSIONABLE_SCHEMA | 1)
MDRcButtressSec::MDRcButtressSec(void)
{
	CString sBar = _T("");
	sBar = MSDeformedBar::GetMainBarStr(2);
	m_TBars.InitData(sBar);
	m_BBars.InitData(sBar);
	sBar = MSDeformedBar::GetStirrupBarStr(200);
	m_SBars.InitData(sBar);
	m_SBarNum = 2;
}

MDRcButtressSec::~MDRcButtressSec(void)
{
}

void MDRcButtressSec::CopyFromMe(MDRcButtressSec* pSource)
{
	if(pSource==NULL)	return; 
	m_TBars.CopyFromMe(&pSource->m_TBars);
	m_BBars.CopyFromMe(&pSource->m_BBars);
	m_SBars.CopyFromMe(&pSource->m_SBars);
	m_SBarNum = pSource->m_SBarNum;
}

void MDRcButtressSec::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	m_TBars.Dump(buffer);
	m_BBars.Dump(buffer);
	m_SBars.Dump(buffer);
	buffer << m_SBarNum;
}
void MDRcButtressSec::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	m_TBars.Recovery(buffer);
	m_BBars.Recovery(buffer);
	m_SBars.Recovery(buffer);
	buffer >> m_SBarNum;
}

void MDRcButtressSec::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	MSMainBar TBars, BBars;
	if(ar.IsStoring())
	{
		TBars.Serialize(ar);
		BBars.Serialize(ar);

		m_SBars.Serialize(ar);
		ar << m_SBarNum;

		m_TBars.Serialize(ar);
		m_BBars.Serialize(ar);
	}
	else
	{
		TBars.Serialize(ar);
		BBars.Serialize(ar);
		m_SBars.Serialize(ar);
		ar >> m_SBarNum;

		if(MSVersionInfo::GetCurrentVersion() >= 20160701)
		{
			m_TBars.Serialize(ar);
			m_BBars.Serialize(ar);
		}
		else
		{
			m_TBars.SetDataByOldVersion(&TBars);
			m_BBars.SetDataByOldVersion(&BBars);
		}
	}
}

vector<double> MDRcButtressSec::GetMainBarDiaAllLayer()
{
	vector<double> DiaArr;

	vector<double> TBarDiaArr = m_TBars.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), TBarDiaArr.begin(), TBarDiaArr.end());

	vector<double> BBarDiaArr = m_BBars.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), BBarDiaArr.begin(), BBarDiaArr.end());

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const double& a, const double& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MDRcButtressSec::GetStirrupBarDiaAllLayer()
{
	vector<double> DiaArr;

	vector<double> SBarDiaArr = m_SBars.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), SBarDiaArr.begin(), SBarDiaArr.end());

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const double& a, const double& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}
double MDRcButtressSec::GetMaxBarDia()
{
	vector<double> DiaArr;
	DiaArr = GetMainBarDiaAllLayer();

	long NumDia = DiaArr.size();
	if (NumDia == 0)
		return MSDeformedBar::GetMinDia();

	return DiaArr[NumDia - 1];
}
double MDRcButtressSec::GetTMaxBarDia(int layerNum)
{
	return m_TBars.GetMaxDeformedBarDiameterOnSet(layerNum);
}
double MDRcButtressSec::GetBMaxBarDia(int layerNum)
{
	return m_BBars.GetMaxDeformedBarDiameterOnSet(layerNum);
}
double MDRcButtressSec::GetStrpDia()
{
	return m_SBars.GetDeformedBarDiameter();
}
int MDRcButtressSec::GetSBarNum()
{
	return m_SBarNum;
}