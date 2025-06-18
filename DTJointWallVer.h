#pragma once
#include "dtjointwall.h"
class DTJointWallVer :
	public DTJointWall
{
public:
	DTJointWallVer(void);
	~DTJointWallVer(void);

	//수직연결
	//상부벽만 있는경우
	//--하부보가 있는경우,없는 경우

	//상하부 벽이 있는경우
	//--보가 있는 경우,없는 경우
	//--최대벤트거리

	//하부벽만 있을경우
	//--상부 보가 있는경우,없는 경우

	BOOL m_HaveBeam;//보(지중보)가 있는지 없는지
	BOOL m_bIntegratedDowel;//주근과 일체화된 다월
	double m_dMaxRebarDist;//하부주근과 상부주근의 거리차가 이하일경우 다월을 생성안한다.
	SpliceType me_SpliceType;
	DevType me_AnchorType;

	//교차보강배근정보

	//다월철근정보
	//상하부 벽이 있고 면의 편차가 m_dMaxBeanDist를 초과한경우 연결다월
	//DTDowelDevelopment	
	//하부벽이 없는경우 하부지지부재에 연결다월
	//DTDowelConnection
};

