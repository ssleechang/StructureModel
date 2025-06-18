#include "StdAfx.h"
#include "MDRcStairLandingM.h"
#include "..\GMLib\MSVersionInfo.h"
IMPLEMENT_SERIAL(MDRcStairLandingM, MDRcStairM, VERSIONABLE_SCHEMA | 1)
MDRcStairLandingM::MDRcStairLandingM(void)
{
}


MDRcStairLandingM::~MDRcStairLandingM(void)
{
}

void MDRcStairLandingM::Dump(EFS::EFS_Buffer& buffer)
{
	MDRcStairM::Dump(buffer);

}

void MDRcStairLandingM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDRcStairM::Recovery(buffer);

}

void MDRcStairLandingM::Serialize( CArchive& ar )
{
	MDRcStairM::Serialize(ar);
	if(ar.IsStoring())
	{
	}
	else
	{
		if (MSVersionInfo::GetCurrentVersion() < 20241119)
		{
			m_ReinforceBar.RemoveAll();
		}
	}
}

void MDRcStairLandingM::CopyFromMe( MDMember* pSource )
{
	MDRcStairM::CopyFromMe(pSource);
}

vector<double> MDRcStairLandingM::GetMainBarDiaAllLayer()
{
	vector<double> DiaArr = MDRcStairM::GetMainBarDiaAllLayerByLanding();

	return DiaArr;
}

vector<double> MDRcStairLandingM::GetStirrupBarDiaAllLayer()
{
	vector<double> DiaArr = MDRcStairM::GetStirrupBarDiaAllLayer();

	return DiaArr;
}
