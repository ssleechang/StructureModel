#pragma once
#include "dtgroup.h"
class DTGroupColumn :
	public DTGroup
{
public:
	DTGroupColumn(void);
	~DTGroupColumn(void);

private:
	//DTMember
	long m_nDTMemberID;
	//DTJoint 
	//상부만 기둥이 있는 경우
	long m_nTopColumnDefaultID;
	//상부 & 하부 기둥이 있는 경우
	long m_nTopBotColumnDefaultID;
	//하부만 기둥이 있는 경우
	long m_nBotColumnDefaultID;

	//DTStirrup
	long m_nRectDefaltID;
	long m_nCircleDefaltID;
	long m_nPolygonDefaltID;

	//DTOpenning
	long m_nDTOpenningWall;


};

