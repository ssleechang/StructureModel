#include "StdAfx.h"
#include "MDRcStairStepM.h"

IMPLEMENT_SERIAL(MDRcStairStepM, MDRcStairM, VERSIONABLE_SCHEMA | 1)
MDRcStairStepM::MDRcStairStepM(void)
{
	CString sBar = MSDeformedBar::GetMainBarStr(4);
	m_ReinforceBar = sBar;
}


MDRcStairStepM::~MDRcStairStepM(void)
{
}

void MDRcStairStepM::Dump(EFS::EFS_Buffer& buffer)
{
	MDRcStairM::Dump(buffer);

}

void MDRcStairStepM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDRcStairM::Recovery(buffer);

}

void MDRcStairStepM::Serialize( CArchive& ar )
{
	MDRcStairM::Serialize(ar);
	if(ar.IsStoring())
	{
	}
	else
	{
	}
}

void MDRcStairStepM::CopyFromMe( MDMember* pSource )
{
	MDRcStairM::CopyFromMe(pSource);
}

vector<double> MDRcStairStepM::GetMainBarDiaAllLayer()
{
	vector<double> DiaArr = MDRcStairM::GetMainBarDiaAllLayerByStep();

	return DiaArr;
}

vector<double> MDRcStairStepM::GetStirrupBarDiaAllLayer()
{
	vector<double> DiaArr = MDRcStairM::GetStirrupBarDiaAllLayer();
	return DiaArr;
}
