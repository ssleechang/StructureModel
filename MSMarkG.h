#pragma once
#include "MSElement.h"

class MSBaseBuilding;
class MSMarkM;
class MSCompoG;
class StructureModel_CLASS MSMarkG : public MSElement
{
public:
	MSMarkG(void);
	~MSMarkG(void);

	virtual Type_MSCompoG GetType() { return msMarkG; };

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSMarkG)
	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSObject* pSource, bool bNewID=true);

	virtual void Init(bool bInConstructor=false);
	GM2DPolyline* GetCloud();
	virtual GM2DPolyline* GetProfile() override {return GetCloud();};

	MSBaseBuilding* mp_Bldg;
	MSMarkM* mp_MarkerM;
	GM2DPolyline* mp_Cloud;
	long m_FloorNum;
	bool m_IsSendMark;

	int m_nLevel;
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	virtual void Translate(GM2DVector &pDelta);
	virtual void RotateZ(double theta, GM2DVector *pCenter);
	virtual void MirrorByAxis(GM2DLineVector& pAxis);

public:
};


