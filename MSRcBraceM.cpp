#include "StdAfx.h"
#include "MSRcBraceM.h"

IMPLEMENT_SERIAL(MSRcBraceM, MDMember, VERSIONABLE_SCHEMA | 1)
MSRcBraceM::MSRcBraceM(void)
{
	m_nStirrupType = 0;
	m_nXStirrupNum = 0;
	m_nYStirrupNum = 0;
}

MSRcBraceM::~MSRcBraceM(void)
{
}

void MSRcBraceM::CopyFromMe(MDMember* pSource)
{
	if(pSource==NULL)	return;
	MSRcBraceM* pSourceBraceM = (MSRcBraceM*)pSource;
	m_nStirrupType = pSourceBraceM->m_nStirrupType;
	m_nXStirrupNum = pSourceBraceM->m_nXStirrupNum;;
	m_nYStirrupNum = pSourceBraceM->m_nYStirrupNum;;
	m_TBars.CopyFromMe(&pSourceBraceM->m_TBars);
	m_BBars.CopyFromMe(&pSourceBraceM->m_BBars);
	m_LBars.CopyFromMe(&pSourceBraceM->m_LBars);
	m_RBars.CopyFromMe(&pSourceBraceM->m_RBars);
	m_SBars.CopyFromMe(&pSourceBraceM->m_SBars);

	MDMember::CopyFromMe(pSource);
}

void MSRcBraceM::Dump(EFS::EFS_Buffer& buffer)
{
	MDMember::Dump(buffer);

	buffer << m_nStirrupType << m_nXStirrupNum << m_nYStirrupNum;
	m_TBars.Dump(buffer);
	m_BBars.Dump(buffer);
	m_LBars.Dump(buffer);
	m_RBars.Dump(buffer);
	m_SBars.Dump(buffer);;
}
void MSRcBraceM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDMember::Recovery(buffer);
	buffer >> m_nStirrupType >> m_nXStirrupNum >> m_nYStirrupNum;
	m_TBars.Recovery(buffer);
	m_BBars.Recovery(buffer);
	m_LBars.Recovery(buffer);
	m_RBars.Recovery(buffer);
	m_SBars.Recovery(buffer);;
}

void MSRcBraceM::Serialize(CArchive &ar)
{
	MDMember::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_nStirrupType;
		ar << m_nXStirrupNum;
		ar << m_nYStirrupNum;
		m_TBars.Serialize(ar);
		m_BBars.Serialize(ar);
		m_LBars.Serialize(ar);
		m_RBars.Serialize(ar);
		m_SBars.Serialize(ar);
	}
	else
	{
		ar >> m_nStirrupType;
		ar >> m_nXStirrupNum;
		ar >> m_nYStirrupNum;
		m_TBars.Serialize(ar);
		m_BBars.Serialize(ar);
		m_LBars.Serialize(ar);
		m_RBars.Serialize(ar);
		m_SBars.Serialize(ar);
	}
}

vector<double> MSRcBraceM::GetMainBarDiaAllLayer()
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

vector<double> MSRcBraceM::GetStirrupBarDiaAllLayer()
{
	vector<double> SBarDiaArr = m_SBars.GetBarDiaAllLayer();

	return SBarDiaArr;
}
