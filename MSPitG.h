#pragma once
#pragma once
#include "mspartcompog.h"
#include "MSSlabG.h"
#include "MDMember.h"

class MSSubHaunchG;
class StructureModel_CLASS MSPitG :
	public MSPartCompoG
{
public:
	MSPitG();
	~MSPitG(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSPitG)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
    virtual void Init(bool bInConstructor=false);
	virtual Type_MSCompoG GetType(){return msPitG;};
	virtual long ResetGetID();

	virtual void RecoverPointerAfterSerialize(MSFloor* floor);
	virtual void RecoverPointerAfterSerialize_SubMembers();

	virtual bool MakeBoundary();
	
	virtual void InitializeParameter();

	virtual inline void CalcBoundRectBeforeAddQuadTree();
	virtual bool VisibleInRect( GM2DRectangle* pRect);
	virtual void MakeCCW();

	virtual void AddHaunch(double x, double y, double x2, double y2, double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto);
	virtual void AddHaunchesOnProfile(double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto);
	virtual void DeleteHaunchs();
	virtual bool Remove(MSSubHaunchG* pSubHaunchG);
	virtual bool RemoveSubMember(MSElement* element) override;
	virtual bool HasHaunch();
	virtual double GetBotOffSet() { return m_dOffset; };

	void TrimHaunches();
	void RegenHaunch();

	virtual vector<MSCompoG*> GetPartnerCompoGs(long GetIdx = 0);

	GM2DPolyline* mp_OutPoly;
	CTypedPtrArray<CObArray, GM2DPolyline*> ma_InPoly;
	GM2DPolyline* mp_InnerPoly;//피트 내부에 집수정 ==> 사용 안함
	CTypedPtrArray<CObArray, GM2DPolyline*> ma_WellPoly;
	void DeletePolylineInfo();

	void RemoveSlabGID(long nSlabGID);
	void MakeConnectedSlabInfoStep2(MSPolylineObject* pUrPolylineObject);
	void MakeConnectedSlabInfo();
	void RemoveOpeningInfoAll();

	void UpdateBoundaryVisibilities(long nUpdateConnIdx = 0);

	double GetTopSlabThick();

	bool AddInPoly(GM2DPolyline* pInPoly);
	bool AddWellPoly(GM2DPolyline* pWellPoly);
	bool IsSameGeometry(MSCompoG* urCompoG, double distTolerance, double degreeTolerance) override;

	double m_dInnerDepth;	// 내부집수정 깊이
	double m_dBarDir;
	long m_IsHaunch;
	long m_numInPoly;//내부폴리곤 갯수
	double m_dOffset;

    double m_dPitDepth;	// Pit 깊이
    double m_dThick;	// 바닥 두께
	double m_dLxDeg;
	bool m_bIsCutSlab;

	vector<long> ma_SlabGID;

	vector<MSSubHaunchG*> ma_Haunches;

    virtual void Translate(GM2DVector &pDelta);
    virtual void MirrorByAxis(GM2DLineVector& pAxis);
    virtual void RotateZ(double theta, GM2DVector *pCenter);

	virtual double GetThick(){return m_dThick + m_dPitDepth;};

	virtual bool GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal = true, bool bCheckLevelZone = true  );
	virtual GM2DPolyline* GetProfile();
	virtual bool CheckData();


	double GetOffsetByContainingLevelZone();

	void MakePitBox();
	void DeletePitBox();

	bool HasSpacer();
	void MergeColinearHaunch();
	virtual bool HasSubMember() override;
	virtual void DeleteSubMembers() override;

	vector<GM2DCurve*> GetVisibileCurves(eVisibileCurveType nVisibile);

	virtual bool UsedMaterial(long materialID) override;
	void SubMemberMaterialValidCheck();

	virtual vector<MSElement*> GetSubMembers() override;

	virtual void FixProfile() override;
// 우마 철근
public:
	long m_TopSpacerID;
	long m_BotSpacerID;
	virtual long GetTopSpacerID() { return m_TopSpacerID; };
	virtual long GetBotSpacerID() { return m_BotSpacerID; };
	GM2DRectangle m_RectForQuadTree;

	void AddHaunchGs(vector<MSSubHaunchG*> HaunchGArr);

public:
	virtual MSCompoM::Type_MSCompoM GetDefCompoMType() { return MSCompoM::Type_MSCompoM::msPitM; };
};
