#pragma once
#include "MSLinkMemberG.h"

class MSBeamLevelDif;
class MSLineLevelDifG;

class StructureModel_CLASS MSLineMemberG :
	public MSLinkMemberG
{
	public:
	MSLineMemberG();
	~MSLineMemberG(void);

	enum BCONDI {BC_RIGID=1, BC_PIN, BC_FREE};
	BCONDI m_SCondi;
	BCONDI m_ECondi;
//	virtual bool GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal = true) ;
	
// 	virtual double GetOffSetbyPos(char cPos);
// 	virtual bool GetZLevelsbyPos(double& TopLevel, double& BottomLevel, char cPos);
//	virtual double GetOffSetbyLoc(double dLoc);
//	virtual bool GetZLevelsbyLoc(double& TopLevel, double& BottomLevel, double dLoc = 0.5);

	double m_dAxisRotation;
	
	virtual void MirrorByAxis(GM2DLineVector& pAxis);

};
