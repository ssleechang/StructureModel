#pragma once
#include "dtjoint.h"


class DTDowel;

class StructureModel_CLASS DTJointWall :
	public DTJoint
{
public:
	DTJointWall();
	virtual ~DTJointWall();

	JointWallType me_JointType;

	DTDowel* mp_Dowel;
	double m_dMaxBendDist;
	CoverType me_CoverType;
	double m_dUserCover;
	SpliceType me_SpliceType;
};

