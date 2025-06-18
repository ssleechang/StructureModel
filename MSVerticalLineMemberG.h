#pragma once
#include "MSPointMemberG.h"
#include "GMLib/GM2DVector.h"

class StructureModel_CLASS MSVerticalLineMemberG : public MSPointMemberG
{
public:
	MSVerticalLineMemberG();
	~MSVerticalLineMemberG(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSVerticalLineMemberG)
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual void Init(bool bInConstructor=false);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 
	virtual bool GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal = true, bool bCheckLevelZone = true  );
	virtual double GetEndLevel(bool isMax = true);
	virtual double GetRelativeEndLevel(bool isMax = true);
	virtual double GetStartLevel(bool isMax = false);
	virtual double GetRelativeStartLevel(bool isMax = false);
	virtual double GetHeight();

	double m_EOffsetZ;
};