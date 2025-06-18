#pragma once
#include "msfloor.h"

class StructureModel_CLASS MSFloorBlock : public MSFloor
{
public:
	MSFloorBlock(double dNodeTol=100., double dDegTol=1.);
	~MSFloorBlock(void);

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSLevelZone)
	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSLevelZone* pSource, bool bNewID=true);
	virtual void Init(bool bInConstructor=false);

	GM2DVector m_BasePoint;
	vector<MSConnectivity*> ma_ExtJoint;
	vector<MSCompoG*> ma_ExtCompoG;

};

