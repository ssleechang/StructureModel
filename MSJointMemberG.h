#pragma once
#include "msverticallinememberg.h"

class MSJointMemPart;

class StructureModel_CLASS MSJointMemberG :
	public MSVerticalLineMemberG
{
public:
	MSJointMemberG();
	~MSJointMemberG(void);

	vector<MSJointMemPart*> ma_JointMemPart;
	void RemoveJointMemParts();

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSJointMemberG)
	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSJointMemberG* pSource, bool bNewID=true);
	virtual void Init(bool bInConstructor=false);
	virtual Type_MSCompoG GetType(){return msWallJointG;};


// 	vector<MSJoint*> ma_MidJoint;
// 	bool AddMidJoint( MSJoint* pJoint );
// 
// 
// 	virtual void RemoveMemParts();


// 	virtual bool SetJoints(long BotJointID, long TopJointID);
// 	virtual void LinkToJoint(bool IsOn);
// 	virtual void GetSecPolygon(GMPolygon& APGon);
// 	virtual bool MakeBoundary();//by LSS
// 	void GetKDir(double& KDirX, double& KDirY, double& KDirZ);
// 	long IsVertical();
// 
// 	BOOL VisibleInRect( GM2DRectangle* pRect);
// 
// 	double GetLength();
// 
// private:
// 	virtual bool CalcVolume();//by LSS
// 	virtual bool CalcFormWorkArea();//by LSS
};
