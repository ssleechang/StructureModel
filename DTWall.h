#pragma once
#include "dtmember.h"

class StructureModel_CLASS DTWall :
	public DTMember
{
public:
	DTWall(void);
	~DTWall(void);

	
	//수평근 시작 위치
	RebarPlaceType me_HorPlaceType;
	//수직근 시작 위치
	RebarPlaceType me_VerPlaceType;
	//교차부에서 시작시 수직근의 위치
	RebarPlaceType me_CrossPlaceType;
	//단부는 항상 Cover가 시작위치임.

	//

// 	수평시작 간격		-간격/2, 사용자입력
// 	수직근 시작간격		-간격/2, 사용자입력
// 	폭고정근 DT
// 	폭고정근 철근형상, 배근정보(지름,간격)
// 	짧은 벽체 2-Ubar로 배근-최대Ubar길이
// 	짧은 벽체 스터럽 배근 - 최대 스터럽 배근 벽길이

};

