#include "StdAfx.h"
#include "DTOpeningWall.h"

DTOpeningWall::DTOpeningWall()
{
	Init();
}


DTOpeningWall::~DTOpeningWall(void)
{

}

long DTOpeningWall::MakeTestDefaultSample( vector<DTOpeningWall*>& DTOpenningWallArr )
{

	DTOpeningWall* pDTOpeningWall = new DTOpeningWall();
	DTOpenningWallArr.push_back(pDTOpeningWall);

	pDTOpeningWall = new DTOpeningWall();
	pDTOpeningWall->SetHorEndType(EndType_Hook);
	pDTOpeningWall->SetVerEndType(EndType_Hook);
	DTOpenningWallArr.push_back(pDTOpeningWall);

	pDTOpeningWall = new DTOpeningWall();
	pDTOpeningWall->SetHorEndType(EndType_None);
	pDTOpeningWall->SetVerEndType(EndType_None);
	pDTOpeningWall->SetbUserEndBarNum(true);
	pDTOpeningWall->SetNumVerEndAddbar(4);
	pDTOpeningWall->SetNumVerEndAddbar(4);
	pDTOpeningWall->SetAddBarDiaType(1);
	pDTOpeningWall->SetAddBarDia(13);
	pDTOpeningWall->SetCoverBarDia(16);

	DTOpenningWallArr.push_back(pDTOpeningWall);

	return DTOpenningWallArr.size();

}

void DTOpeningWall::Init()
{
	m_nHorEndType = EndType_Ubar;//0:별도처리없음  1:표준후크 2:Ubar
	m_nVerEndType = EndType_Ubar;//0:별도처리없음  1:표준후크 2:Ubar
	m_bUserEndBarNum = false;//false:끊어지는 주근의 수 /2   true:사용자 입력
	m_numVerEndAddbar = 0;//수직보강근개수
	m_numHorEndAddbar = 0;//수평보강근개수

	m_numCornerAddbar = 4;

	m_nAddBarDiaType = 0;//0:끊어지는 철근의 지름과 동일 1:사용자 지정
	m_nAddBarDia = 0;
	m_nCoverBarDia = 0;	
}
