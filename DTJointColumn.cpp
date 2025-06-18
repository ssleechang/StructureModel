#include "StdAfx.h"
#include "DTJointColumn.h"

vector<DTJointColumn*> DTJointColumn::ma_DTJointColumn;


DTJointColumn::DTJointColumn(JointColumnType Type)
{
	me_JointType = Type;
	mp_Dowel = NULL;
	m_Name.Format(_T("CJ_%ld"),ma_DTJointColumn.size()+1);
	m_dMaxBendDist = 75;
	m_dMaxRebarDist = 75;
	m_bIntegratedDowel = FALSE;
	me_CoverType = CoverType_User;
	m_dUserCover = 50;
	me_SpliceType = SpliceType_All_B;
	me_AnchorType = DevType_Hook_Cover;
}

DTJointColumn::~DTJointColumn()
{
}

long DTJointColumn::MakeTestDefaultSample( vector<DTJointColumn*>& DTJointColumnArr )
{
	//상부만 기둥이 있는 경우
	DTJointColumn* pJoint = new DTJointColumn(JointColumnType_TopColumn);
	DTJointColumnArr.push_back(pJoint);
	//상부 & 하부 기둥이 있는 경우
	pJoint = new DTJointColumn(JointColumnType_TopColumn_BotColumn);
	DTJointColumnArr.push_back(pJoint);

	//하부만 기둥이 있는 경우
	pJoint = new DTJointColumn(JointColumnType_BotColum);
	DTJointColumnArr.push_back(pJoint);

	return DTJointColumnArr.size();

}
