#include "StdAfx.h"
#include "MSRcRampM.h"

IMPLEMENT_SERIAL(MSRcRampM, MDMember, VERSIONABLE_SCHEMA | 1)
MSRcRampM::MSRcRampM(void)
{

}

MSRcRampM::~MSRcRampM(void)
{
}

void MSRcRampM::CopyFromMe(MDMember* pSource)
{
	if(pSource==NULL)	return;
	MSRcRampM* pSourceRampM = (MSRcRampM*)pSource;
	m_XTopBar.CopyFromMe(&pSourceRampM->m_XTopBar);
	m_XBotBar.CopyFromMe(&pSourceRampM->m_XBotBar);
	m_YTopBar.CopyFromMe(&pSourceRampM->m_YTopBar);
	m_YBotBar.CopyFromMe(&pSourceRampM->m_YBotBar);

	MDMember::CopyFromMe(pSource);
}

void MSRcRampM::Dump(EFS::EFS_Buffer& buffer)
{
	MDMember::Dump(buffer);

	m_XTopBar.Dump(buffer);
	m_XBotBar.Dump(buffer);
	m_YTopBar.Dump(buffer);
	m_YBotBar.Dump(buffer);
}

void MSRcRampM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDMember::Recovery(buffer);

	m_XTopBar.Recovery(buffer);
	m_XBotBar.Recovery(buffer);
	m_YTopBar.Recovery(buffer);
	m_YBotBar.Recovery(buffer);
}

void MSRcRampM::Serialize(CArchive &ar)
{
	MDMember::Serialize(ar);
	if(ar.IsStoring())
	{
		m_XTopBar.Serialize(ar);
		m_XBotBar.Serialize(ar);
		m_YTopBar.Serialize(ar);
		m_YBotBar.Serialize(ar);
	}
	else
	{
		m_XTopBar.Serialize(ar);
		m_XBotBar.Serialize(ar);
		m_YTopBar.Serialize(ar);
		m_YBotBar.Serialize(ar);
	}
}

vector<double> MSRcRampM::GetMainBarDiaAllLayer()
{
	vector<double> DiaArr;

	vector<double> XTopBarDiaArr = m_XTopBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), XTopBarDiaArr.begin(), XTopBarDiaArr.end());

	vector<double> XBotBarDiaArr = m_XBotBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), XBotBarDiaArr.begin(), XBotBarDiaArr.end());

	vector<double> YTopBarDiaArr = m_YTopBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), YTopBarDiaArr.begin(), YTopBarDiaArr.end());

	vector<double> YBotarDiaArr = m_YBotBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), YBotarDiaArr.begin(), YBotarDiaArr.end());

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const double& a, const double& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MSRcRampM::GetStirrupBarDiaAllLayer()
{
	vector<double> DiaArr;

	return DiaArr;
}
