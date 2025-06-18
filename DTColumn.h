#pragma once
#include "dtmember.h"
class DTColumn :
	public DTMember
{
public:
	DTColumn(void);
	~DTColumn(void);

// 		스트럽 간격/2  	
	RebarPlaceType  me_StirrupPlaceType;
	double m_dUserSpacingStirrup;

	// 		횡방향 철근배근 최소구간 설정(Lomax>=1/6Ln, 450)
	// 		1/6Ln
	double m_dEndRatio;//6/1*L이상 최소450이상







// 	내진 비내진
// 		내부기둥, 외부기둥 구분
// 		4면 보 접합을 내부기둥으로 판단
// 		내부 
// 		외부
// 		스트럽 시작길이

// 		사용자 입력
// 		횡방향 철근배근 최소구간 설정(Lomax>=1/6Ln, 450)
// 		1/6Ln
// 		사용자입력

};

