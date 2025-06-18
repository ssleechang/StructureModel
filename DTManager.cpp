#include "StdAfx.h"
#include "DTManager.h"
#include "DTOpeningWall.h"


DTManager::DTManager()
{
	//현재는 모두 테스트 파일 생성시키는 걸로
	MakeTestDefaultSample();

}


DTManager::~DTManager()
{

}

bool DTManager::MakeTestDefaultSample()
{
	if(ma_OpenningWall.size()==0)
	{
		DTOpeningWall::MakeTestDefaultSample(ma_OpenningWall);
	}

	return true;
}

