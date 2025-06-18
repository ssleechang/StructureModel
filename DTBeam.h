#pragma once
#include "dtmember.h"
class DTBeam :
	public DTMember
{
public:
	DTBeam(void);
	~DTBeam(void);

private:
	//스트럽 배근관련
	RebarPlaceType me_StirrupPlaceType;
	double m_dUserSpacingStirrup;
	long

	//보의 단부구간비율 설정방법
	//0.25 0.3...
	double m_dTopSideEndRatio;
	double m_dTopInsideEndRatio;

	//단부섹션 적용방법-Bigger or Smaller

	//이음방식-B급이음,커플러,압축이음
	//지름이 두꺼운 보에 대해 커플러 사용할 수 있음. 지름별로 설정할 수 있도록 설정해야할듯
	//위사항은 매트에서도 해당

	//이단수평지지부재설정
	//간격배근,갯수배근



};

