#pragma once
#include "MSLinkMemberG.h"
#include "MSStirrupBar.h"
#include "MSMainBar.h"

#include "GMLib/GM2DLineVector.h"

class MSLevelDifHaunch;
class MSLevelZone;

class StructureModel_CLASS MSLineLevelDifG :
	public MSLinkMemberG
{
public:
	MSLineLevelDifG(MSLevelZone* pZone=NULL);
	~MSLineLevelDifG(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSLineLevelDifG)
	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSLineLevelDifG* pSource, bool bNewID=true);
	virtual void Init(bool bInConstructor=false);
	virtual Type_MSCompoG GetType(){return msLineLevelDifG;};
	virtual void ModifyObject(bool isDoit = true);
	virtual bool GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal = true, bool bCheckLevelZone = true );
	virtual bool GetZLevelsbyLoc(double& TopLevel, double& BottomLevel, double dLoc = 0.5);
	
	virtual MSFloor* GetFloor();
	virtual double GetFloorLevel();
	virtual double GetWidth();
	virtual double GetDepth();
	virtual double GetThick();
	virtual double GetLength();

	virtual MSMaterial* GetMaterial();
	virtual void SetOwnerCompoG(MSElement* pOwner) { mp_LeftSlab = (MSSlabG*)pOwner; };
	virtual MSElement* GetOwnerCompoG();

	//virtual GM2DCurve* GetCenLineVec();
	virtual shared_ptr<GM2DCurve> GetCenCurve();
	virtual GM2DVector GetCenVec();
	GM2DCurve* GetLongLVec(bool bExtendToOrthoPart = true);
	GM2DCurve* GetCurve();

	bool isDelete();

	void CutLineBySlab2(GM2DCurve* pTranslateLine, GM2DPolyline* pPoly, MSSlabG* pSlab, bool isSlabPoly);
	void CutLineBySlab(GM2DCurve* pTranslateLine, MSSlabG* pSlab);
    void SetCenCurve(GM2DCurve* pCurve);
	void SetLines(MSElement::Type_MSCompoG BeamOrSlab, double MemberThickness);
	bool SplitAndSetRightLevelWhenOverlap(MSLineLevelDifG* pUrLevel, double& UrP0Loc, double& UrP1Loc);
	MSLineLevelDifG* Split(double FromLoc, double ToLoc, MSLevelZone* pRightZone);

	void MakeLRLine_UserType();
	void SetEdgeWallFromProfile(GM2DPolyline* profile, int idx, double dLoc1 = 0.0, double dLoc2= 1.0);

// 	GM2DLineVector m_TopLine;
// 	GM2DLineVector m_BotLine;

	map<long, long> mm_SlabGID;
	vector<long> ma_LinkMemberGID;

	MSLevelZone* mp_LeftLevelZone;
	MSLevelZone* mp_RightLevelZone;
	
	MSSlabG* mp_LeftSlab;
	MSSlabG* mp_RightSlab;

	double m_InfillBeamFactor;
	double m_InfillSlabFactor;

	MSLevelDifHaunch* mp_BeamHaunch;
	MSLevelDifHaunch* mp_SlabHaunch;

	double m_dWallWidth;
	double m_dWallHeight; // 20141121
	bool m_bHasHaunch;
	bool m_bHasWall;
	double m_dHaunchX;
	double m_dHaunchY;
	double m_dHaunchAngle;
	bool m_bUseHaunchMaterial;
	bool m_bUseBackFillFormwork;
	int m_nHaunchMaterialID;
	bool m_IsUsedStoryMaterial;

    int m_nPlaceType;

    bool m_IsNumPlaceBarVExt;
    MSStirrupBar m_SBarVExt;
    MSMainBar m_MainBarVExt;
    bool m_IsNumPlaceBarVInt;
    MSStirrupBar m_SBarVInt;
    MSMainBar m_MainBarVInt;
    bool m_IsNumPlaceBarHExt;
    MSStirrupBar m_SBarHExt;
    MSMainBar m_MainBarHExt;
    bool m_IsNumPlaceBarHInt;
    MSStirrupBar m_SBarHInt;
    MSMainBar m_MainBarHInt;

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 
	void LiveCntUp();
	double GetLeftLevel();
	double GetRightLevel();
	double GetDepth_XML();
	double GetLevelZoneZ();
	void RecoverPointerAfterSerialize(MSFloor* floor);

	double GetLevelDifHeight();
	bool SetLevelDifHeight(double dHeight);

	double GetLevelDifWidth();
	bool SetLevelDifWidth(double dWidth);

	bool GetLevelDifHasWall();
	bool SetLevelDifHasWall(bool HasWall);

	bool GetLevelDifHasHaunch();
	bool SetLevelDifHasHaunch(bool HasHaunch);

	double GetLevelDifHaunchX();
	bool SetLevelDifHaunchX(double HaunchX);

	double GetLevelDifHaunchY();
	bool SetLevelDifHaunchY(double HaunchY);

	double GetLevelDifHaunchAngle();
	bool SetLevelDifHaunchAngle(double HaunchAngle);

	bool GetLevelDifUseHaunchMaterial();
	bool SetLevelDifUseHaunchMaterial(bool value);

	bool GetLevelDifUseBackFillFormwork();
	void SetLevelDifUseBackFillFormwork(bool value);

	int GetLevelDifHaunchMaterialID();
	bool SetLevelDifHaunchMaterialID(int value);

	bool GetLevelDifHaunchUsedStoryMaterial();
	void SetLevelDifHaunchUsedStoryMaterial(bool value);

    int GetLevelDifPlaceType();
    void SetLevelDifPlaceType(int value);

    void SetLevelDifVBarExt(CString value);
    void SetLevelDifVBarInt(CString value);
    void SetLevelDifHBarExt(CString value);
    void SetLevelDifHBarInt(CString value);

	long GetHaunchMaterialID();
	void SetHaunchMaterialID(long materialID)		{ m_nHaunchMaterialID = materialID; };
	bool GetIsUsedStoryMaterial()												{ return m_IsUsedStoryMaterial; };
	void SetIsUsedStoryMaterial(bool bUsedStoryMaterial)	{ m_IsUsedStoryMaterial = bUsedStoryMaterial; };

	bool IsSameLRZLevel();

	bool IsOnEdgeLevelDifG();

	double GetLeftSlabLevel();
	double GetRightSlabLevel();
	MSSlabG* GetLowLevelSlab();
	int GetLevelZoneID();

	double GetMaxSlabThick();

	void DeleteSlabLineLevelDif();

	bool IsUserLevelDif(){return m_bUserLevelDif;};
	void SetIsUserLevelDif(bool isEdgeWall){ m_bUserLevelDif = isEdgeWall;};

	void AddConnectionWithMember( MSCompoG* pPartnerMember );
	void DeleteConnections();

	virtual void Translate(GM2DVector &pDelta);
	virtual void MirrorByAxis(GM2DLineVector& pAxis);
	virtual void RotateZ(double theta, GM2DVector *pCenter);
private:
	bool m_bUserLevelDif; // 20141121
	long m_nLiveCnt;
// 	long m_TopCompoGID;
// 	long m_BotCompoGID;
// 	double m_dOffset;
	int m_nLeftLevelZoneID, m_nRightLevelZoneID, m_nLeftSlabID, m_nRightSlabID; // temp use for save load

};
