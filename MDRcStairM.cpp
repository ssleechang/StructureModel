#include "StdAfx.h"
#include "MDRcStairM.h"
#include "..\GMLib\MSVersionInfo.h"

IMPLEMENT_SERIAL(MDRcStairM, MDMember, VERSIONABLE_SCHEMA | 1)

MDRcStairM::MDRcStairM(void)
{
	CString sBar = _T("");
	sBar = MSDeformedBar::GetStirrupBarStr(200);
	m_TopX = sBar;
	m_TopY = sBar;
	m_BotX = sBar;
	m_BotY = sBar;	

	m_nStepRebarPlaceType = 0;
}


MDRcStairM::~MDRcStairM(void)
{
}

void MDRcStairM::Dump(EFS::EFS_Buffer& buffer)
{
	MDMember::Dump(buffer);

	m_TopX.Dump(buffer);
	m_TopY.Dump(buffer);
	m_BotX.Dump(buffer);
	m_BotY.Dump(buffer);
	m_ReinforceBar.Dump(buffer);

	buffer << m_nStepRebarPlaceType;
}

void MDRcStairM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDMember::Recovery(buffer);

	m_TopX.Recovery(buffer);
	m_TopY.Recovery(buffer);
	m_BotX.Recovery(buffer);
	m_BotY.Recovery(buffer);
	m_ReinforceBar.Recovery(buffer);

	buffer >> m_nStepRebarPlaceType;
}

void MDRcStairM::Serialize( CArchive& ar )
{
	MDMember::Serialize(ar);
	if(ar.IsStoring())
	{
		m_TopX.Serialize(ar);
		m_TopY.Serialize(ar);
		m_BotX.Serialize(ar);
		m_BotY.Serialize(ar);
		m_ReinforceBar.Serialize(ar);

		ar << m_nStepRebarPlaceType;
	}
	else
	{
		m_TopX.Serialize(ar);
		m_TopY.Serialize(ar);
		m_BotX.Serialize(ar);
		m_BotY.Serialize(ar);
		m_ReinforceBar.Serialize(ar);

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20200703)
		{
			ar >> m_nStepRebarPlaceType;
		}
	}
}

void MDRcStairM::CopyFromMe( MDMember* pSource )
{
	MDRcStairM* pSourceIsoStairM = (MDRcStairM*)pSource;
	m_TopX.CopyFromMe(&pSourceIsoStairM->m_TopX);
	m_TopY.CopyFromMe(&pSourceIsoStairM->m_TopY);
	m_BotX.CopyFromMe(&pSourceIsoStairM->m_BotX);
	m_BotY.CopyFromMe(&pSourceIsoStairM->m_BotY);;
	m_ReinforceBar.CopyFromMe(&pSourceIsoStairM->m_ReinforceBar);

	m_nStepRebarPlaceType = pSourceIsoStairM->m_nStepRebarPlaceType;

	MDMember::CopyFromMe(pSource);
}

vector<double> MDRcStairM::GetMainBarDiaAllLayer()
{
	vector<double> DiaArr;

	vector<double> TopXBarDiaArr = m_TopX.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), TopXBarDiaArr.begin(), TopXBarDiaArr.end());

	vector<double> TopYBarDiaArr = m_TopY.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), TopYBarDiaArr.begin(), TopYBarDiaArr.end());

	vector<double> BotXBarDiaArr = m_BotX.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), BotXBarDiaArr.begin(), BotXBarDiaArr.end());

	vector<double> BotYBarDiaArr = m_BotY.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), BotYBarDiaArr.begin(), BotYBarDiaArr.end());

	vector<double> ReinforceBarDiaArr = m_ReinforceBar.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), ReinforceBarDiaArr.begin(), ReinforceBarDiaArr.end());

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const long& a, const long& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MDRcStairM::GetMainBarDiaAllLayerByStep()
{
	return MDRcStairM::GetMainBarDiaAllLayer();
}

vector<double> MDRcStairM::GetMainBarDiaAllLayerByLanding()
{
	vector<double> DiaArr;

	vector<double> TopXBarDiaArr = m_TopX.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), TopXBarDiaArr.begin(), TopXBarDiaArr.end());

	vector<double> TopYBarDiaArr = m_TopY.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), TopYBarDiaArr.begin(), TopYBarDiaArr.end());

	vector<double> BotXBarDiaArr = m_BotX.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), BotXBarDiaArr.begin(), BotXBarDiaArr.end());

	vector<double> BotYBarDiaArr = m_BotY.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), BotYBarDiaArr.begin(), BotYBarDiaArr.end());

	//Landing은 m_ReinforceBar 값을 사용하지 않는다.
	// 	vector<long> ReinforceBarDiaArr = m_ReinforceBar.GetBarDiaAllLayer();
	// 	DiaArr.insert(DiaArr.end(), ReinforceBarDiaArr.begin(), ReinforceBarDiaArr.end());

	sort(DiaArr.begin(), DiaArr.end(),
		[](const long& a, const long& b)->bool {return a < b; }
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MDRcStairM::GetStirrupBarDiaAllLayer()
{
	vector<double> DiaArr;

	return DiaArr;
}

int MDRcStairM::GetStepRebarPlaceType()
{
	return m_nStepRebarPlaceType;
}

void MDRcStairM::SetStepRebarPlaceType(int stepRebarPlaceType)
{
	m_nStepRebarPlaceType = stepRebarPlaceType;
}
bool MDRcStairM::PlaceTopXBar()
{
	return m_TopX.ma_strBar.GetCount() > 0;
}
bool MDRcStairM::PlaceTopYBar()
{
	return m_TopY.ma_strBar.GetCount() > 0;
}
bool MDRcStairM::PlaceReinforceBar()
{
	return m_ReinforceBar.ma_strBar.GetCount() > 0;
}