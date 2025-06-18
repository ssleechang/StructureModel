#pragma once
#include "MSCompoG.h"

#include "GMLib/GM2DVector.h"

class MSAssembly;
class GM2DRectangle;

class StructureModel_CLASS MSPointMemberG :
	public MSCompoG
{
public:
	MSPointMemberG();
	~MSPointMemberG(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSPointMemberG)
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual CString ToString();

	virtual void Init(bool bInConstructor=false);
	virtual bool IsInvalidVecInput();
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	virtual double GetBotOffSet() { return m_SOffsetZ; };
	double m_SOffsetZ;
	double m_dAxisRotation;

    virtual bool VisibleInRect( GM2DRectangle* pRect);
    virtual bool MakeBoundary();

	virtual void MirrorByAxis(GM2DLineVector& pAxis);
	virtual void RotateZ(double theta, GM2DVector *pCenter);
	virtual void SetRotation(double dDeg);

	virtual vector<MSCompoG*> FindConnectMember(FindType_MSElement memberType, long nFloorType, double dTol = DTOL_GM, bool bUseTrimedBoundry = true, bool bFindPolyMemberOnlyOnLine = false, bool bIncludeOnLine = true);
	virtual double GetMemberDegree();

	virtual bool SetCompoM(long MID, bool bUpdateVecInput = true);
	virtual bool SetCompoM(MSCompoM* pCompoM, bool bUpdateVecInput = true);

	ePOINTALIGN_TYPE GetPointAlignType();
	void SetPointAlignType(ePOINTALIGN_TYPE eType, bool bUpdateVecInput = true);

	virtual void UpdateVecInput();
	virtual void UpdateVecCenter();

	virtual void GetSecPolylineByInsertVec(GM2DPolyline& APGon);
	virtual bool GetSteelSecPolylineByInsertVec(GM2DPolyline& APGon);
	virtual void GetHeaderPolylineByInsertVec(GM2DPolyline& APGon);
	virtual void GetTrimmedBoundaryZLevel(GM2DPolyline& APoly, double dTopLevel, double dBotLevel);
	virtual CString GetKey(bool bUseID = true);

	void UpdateVecInputByCenVec();
	void UpdateCenVecByVecInput();
	
	long GetColumnHeaderMID();

	bool IsSameGeometry(MSCompoG* urCompoG, double distTolerance, double degreeTolerance) override;

protected:
	ePOINTALIGN_TYPE m_ePointAlign;

	void TransformByInsertVecNRotation(GM2DPolyline& APGon);
};
