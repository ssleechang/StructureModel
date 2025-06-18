#include "StdAfx.h"
#include "DTJointBeam.h"

vector<DTJointBeam*> DTJointBeam::ma_DTJointBeam;

DTJointBeam::DTJointBeam(DevType TopDevType, DevType BotDevType, CoverType CoverType)
{
	m_Name.Format(_T("BJ_"),ma_DTJointBeam.size()+1);
	me_TopDevType = TopDevType;
	me_BotDevType = TopDevType;
	me_CoverType = CoverType;
	m_dTopUserDevLen = 0.0;
	m_dTopUserHookLen = 0.0;
	m_dBotUserDevLen = 0.0;
	m_dBotUserHookLen = 0.0;
	m_dUserCover = 50;
}


DTJointBeam::~DTJointBeam()
{
}

long DTJointBeam::MakeTestDefaultSample( vector<DTJointBeam*>& DTJointBeamArr )
{
	//상부하부 모두 자동정착
	DTJointBeam* pJointBeam1 = new DTJointBeam(DevType_Stright_Tension,DevType_Stright_Tension,CoverType_Member);
	DTJointBeamArr.push_back(pJointBeam1);
	DTJointBeam* pJointBeam2 = new DTJointBeam(DevType_Stright_Tension,DevType_Stright_Tension,CoverType_Member);
	DTJointBeamArr.push_back(pJointBeam2);
	DTJointBeam* pJointBeam3 = new DTJointBeam(DevType_Stright_Tension,DevType_Stright_User,CoverType_Member);
	pJointBeam3->SetBotUserDevLen(150);
	DTJointBeamArr.push_back(pJointBeam3);

	DTJointBeam* pJointBeam4 = new DTJointBeam(DevType_Hook_Cover,DevType_Stright_Cover,CoverType_Member);
	DTJointBeamArr.push_back(pJointBeam4);
	DTJointBeam* pJointBeam5 = new DTJointBeam(DevType_Hook_Cover,DevType_Stright_Cover,CoverType_Member);
	DTJointBeamArr.push_back(pJointBeam5);
	DTJointBeam* pJointBeam6 = new DTJointBeam(DevType_Hook_Cover,DevType_Stright_User,CoverType_Member);
	pJointBeam6->SetBotUserDevLen(150);
	DTJointBeamArr.push_back(pJointBeam6);

	return DTJointBeamArr.size();

}
