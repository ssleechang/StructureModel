#pragma once
#include "dtjointwall.h"
class DTJointWallHor :
	public DTJointWall
{
public:
	DTJointWallHor(void);
	~DTJointWallHor(void);

	BOOL m_HaveColumn;//기둥이 있는지 없는지
	//me_EndType == EndType_None --> 연결된 벽이 없을경우 일자커버
	//me_EndType == EndType_None --> 연결된 벽이 있는경우 이음철근이 있다는 뜻==DTDowel
	EndType me_EndType;//마구리타입-연결된 벽이 없을 경우, 수직하게 만나는 두벽
	//2Wall일경우 연결방법-연결철근, 서로정착,한쪽이음, Ubar
	//배근정보가 동일할경우-연결철근,한쪽이음이 가능하지만
	//수평근의 배근정보가 다를경우는 서로정착 또는 ubar로 처리해야함.
	//일자형-단부보강정보
	//유바
	//
	//T형벽체
	//직각모서리벽체







	//수평연결
		
	//수직한벽과 정착
		//유바
		//후크
		//한쪽이음
	//2개의 벽이 만나는
		//연결철근사용
		//한쪽이음
		//서로정착
		//유바

	//3개이상의 벽이 만나는 경우
		//모두유바
		//유바1+정작
		//연결철근사용


	//단부보강배근정보
	//교차보강배근정보
	//교차부배근정보(정착방법 & 연결철근 사용여부)
	//단부배근정보(유바)

	//조인트 부위의 수평근배근정보


};

