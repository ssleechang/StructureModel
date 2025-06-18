#pragma once
#include "dtgroup.h"
class DTGroupWall :
	public DTGroup
{
public:
	DTGroupWall(void);
	~DTGroupWall(void);

private:

	//DTMember
	long m_nDTMemberID;
	//DTJointWallVer
	//수직연결
	//상부벽만 있는경우
	//--하부보가 있는경우,없는 경우
	long m_nDTJointWallTopID;//최상층 벽

	//상하부 벽이 있는경우
	//--보가 있는 경우,없는 경우
	//--최대벤트거리
	long m_nDTJointWallTopBotID;

	//하부벽만 있을경우
	//--상부 보가 있는경우,없는 경우
	long m_nDTJointWallBotID;

	//DTJointWallHor


	//DTStirrup
	long m_nDTStirrup2UbarID;//양유바사용
	long m_nDTStirrupBarID;
	//닭발철근형상을 쓸경우..

	//DTOpenning
	long m_nOpenningWallID;

	//DTSpacer
	long m_nDTSpacerWallID;
	
	//단차벽배근
	long m_nDTLevelDifWallID;


};

