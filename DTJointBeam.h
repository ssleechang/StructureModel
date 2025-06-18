#pragma once
#include "dtjoint.h"
class StructureModel_CLASS DTJointBeam :
	public DTJoint
{
public:
	DTJointBeam(DevType TopDevType, DevType BotDevType, CoverType CoverType);
	virtual ~DTJointBeam();

	//static DTJointBeam* GetDefaltTemplet(int NumSlab);
	static long MakeTestDefaultSample(vector<DTJointBeam*>& DTJointBeamArr);
	static vector<DTJointBeam*> ma_DTJointBeam;

	void SetTopUserDevLen(double dVal){m_dTopUserDevLen = dVal;};
	void SetBotUserDevLen(double dVal){m_dBotUserDevLen = dVal;};
	void SetTopUserHookLen(double dVal){m_dTopUserHookLen = dVal;};
	void SetBotUserHookLen(double dVal){m_dBotUserHookLen = dVal;};
	void SetUserCover(double dVal){m_dUserCover = dVal;};
private:
	long me_JointBeamType;
	//올수있는
	DevType me_TopDevType;
	DevType me_BotDevType;
	CoverType me_CoverType;

	double m_dTopUserDevLen;
	double m_dTopUserHookLen;
	double m_dBotUserDevLen;
	double m_dBotUserHookLen;
	double m_dUserCover; 

	//평행하는 보가 연결되어있다.
	//Level이 다른경우
	//

};

