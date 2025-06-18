#pragma once
#include "MSElement.h"

class MSAssembly;
class MSElement;
class MSFloor;

class StructureModel_CLASS MSSectionView :	public MSElement
{
public:
	MSSectionView(void);
	~MSSectionView(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSSectionView)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true) override;
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	bool CheckData();

    virtual long Contains(double Xc, double Yc, double tolerance);
	virtual bool OverLap(GM2DCurve* pCurve, double tolerance);

	MSSectionView* mp_Source;
	vector<MSSectionView*> ma_Clone;

    MSAssembly* mp_Owner; 
    MSAssembly* GetOwner(){return mp_Owner;};
    void SetOwner(MSAssembly* val){mp_Owner = val;};
	virtual MSFloor* GetFloor();

	virtual Type_MSCompoG GetType(){ return msSectionView; };
	virtual void GetBoundary( GM2DPolyline& APolyLine );

	void RecoverPointerAfterSerialize();

	virtual void Translate(GM2DVector &pDelta);
	virtual void MirrorByAxis(GM2DLineVector& pAxis);
	virtual void RotateZ(double theta, GM2DVector *pCenter);

	int m_nType;
	vector<GM2DVector> m_PolyLine;
	bool m_bLeft;
	CString m_Name;
};

#pragma make_public(MSSectionView)


