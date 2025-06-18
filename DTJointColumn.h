#pragma once
#include "dtjoint.h"

class DTDowel;
class DTDowelDevelopment;

class StructureModel_CLASS DTJointColumn :
	public DTJoint
{
public:
	DTJointColumn(JointColumnType Type);
	virtual ~DTJointColumn();

	static long MakeTestDefaultSample(vector<DTJointColumn*>& DTJointColumnArr);
	static vector<DTJointColumn*> ma_DTJointColumn;


	//상부만 기둥이 있는 경우
	//mp_Dowel = DTDowelDevelopment
	//상부 & 하부 기둥이 있는 경우 & 동일한 boundary
	//상부 & 하부 기둥이 있는 경우 & 편심이 있는경우
	//mp_Dowel = DTDowelConnection
	//하부만 기둥이 있는 경우
	//mp_Dowel == NULL
	DTDowel* mp_Dowel;
	//상하 기둥면의 편차가 m_dMaxBendDist이하이면 주근을 Bent로 연결
	//상하부 기둥이 있는경우	
	//bent로 철근을 연결하는 최대거리
	double m_dMaxBendDist;
	BOOL m_bIntegratedDowel;//주근과 일체화된 다월

	//상하부 기둥이 있는경우 & 배근정보가 다른경우	
	double m_dMaxRebarDist;//하부주근과 상부주근의 거리차가 이하일경우 다월을 생성안한다.
	CoverType me_CoverType;
	double m_dUserCover;


	SpliceType me_SpliceType;
	DevType me_AnchorType;
	JointColumnType me_JointType;



// 	//상부기둥이 없는경우==하부기둥만 있는경우
// 	//하부기둥이 없는경우==상부기둥만 있는경우
// 	double m_dEndCover;//상부 또는 하부 피복
// 	//다월철근정보
// 	//상하부 기둥이 있고 기둥면 편차가 m_dMaxBeanDist를 초과한경우 연결다월
// 	DTDowelConnection* mp_DowelConnection; 
// 	//하부기둥이 없는경우 하부지지부재에 연결다월
// 	DTDowelDevelopment* mp_DowelDevelopment; 

};
