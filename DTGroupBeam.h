#pragma once
#include "dtgroup.h"
class StructureModel_CLASS DTGroupBeam :
	public DTGroup
{
public:
	DTGroupBeam(void);
	~DTGroupBeam(void);
	//상하부 주근 스터럽 단부비율 & 스터럽 시작위치 설정
	
	
	//이단이상의 주근 배근 고정방식

	//


private:
	//DTMember
	long m_nDTMemberID;
	//Beam Joint 
	//캔틸레버보의 정착
	long m_nCantileverDefaltID;
	//큰 보에서 작은 보의 정착
	long m_nAnchorToGirderDefaltID;
	//보-기둥 접합부 상세
	long m_nAnchorToColumnDefaltID;
	//보-벽체 접합부 상세
	long m_nAnchorToWallDefaltID;
	//레벨이 다른 보의 배근

	//나란한 인접보에 정착하는 경우의 상세


	//폐쇄형 & 개방형 스터럽 선택
	//모두 폐쇄형, 일반보 & 양쪽슬래브가 있는 경우만 개방형 사용
	//DTStirrup
	long m_nBothSideSlabDefaltID;//양쪽에 슬래브가 있는 경우
	long m_nOneSideSlabDefaltID;//한쪽에 슬래브가 있는 경우
	long m_nNoneSlabDefaltID;//양쪽 모두 슬래브가 없는 경우
	//깊은보
	//폭이 넓은 보


};

