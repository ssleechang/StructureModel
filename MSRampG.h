#pragma once
#include "mspartcompog.h"

#include "GMLib/GMPolylineArc.h"
#include "GMLib/GMVector.h"
#include "GMLib/GMSurface.h"

class MSBeamG;
class MSSlabG;
class StructureModel_CLASS MSRampG :
	public MSPartCompoG
{
public:
	MSRampG();
	~MSRampG(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSRampG)
	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSRampG* pSource, bool bNewID=true);
	virtual void Init(bool bInConstructor=false);
	virtual Type_MSCompoG GetType(){return msRampG;};
	virtual bool MakeBoundary();

	GMPolylineArc m_RPolyline;
	GMPolylineArc m_LPolyline;

	CArray<GM2DVector, GM2DVector> ma_Loc;

	CArray<long, long> ma_LBeamGID;
	CArray<long, long> ma_RBeamGID;
	CArray<long, long> ma_CrossBeamGID;
	CArray<long, long> ma_SlabGID;
	double m_dHeight;	// 입력값이 없으면 FloorHeight사용

	MSBeamG* m_pLBeamG;
	MSBeamG* m_pRBeamG;
	map<long, MSBeamG*> mm_CrossBeamG;
	map<long, MSSlabG*> mm_SlabG;

	GMSurface* mp_Surface;

public:
	virtual MSCompoM::Type_MSCompoM GetDefCompoMType() { return MSCompoM::Type_MSCompoM::msRampM; };
};
