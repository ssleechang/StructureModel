#pragma once
#include "msobject.h"

#include "GMLib/GM2DLineVector.h"
#include "MSLinkMemberG.h"

class MSConnectivity;
class MSLinkMemberG;
class MSWallMemberG;
class MSSlabG;
class GM2DCurve;
class MSConnLevelZone;

class StructureModel_CLASS MSLinkMemPart :
	public MSObject
{
public:
	MSLinkMemPart(MSLinkMemberG* pLineMember = NULL);
	~MSLinkMemPart(void);
	void Serialize( CArchive& archive );
	virtual void RecoverPointerAfterSerialize(MSFloor* floor);
	static void Serialize_AsID( CArchive& archive, MSLinkMemPart*& part, long& rIdx);
	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSLinkMemPart* pSource, bool bNewID=true);
	virtual void Init(bool bInConstructor=false);

	double GetDegree(long StartOrEnd=0);
	GM2DLineVector GetLeftLVec(MSConnectivity* pStart);

	MSConnectivity* mp_SConn;
	MSConnectivity* mp_EConn;

	MSConnectivity* GetSConn(){return mp_SConn;};
	MSConnectivity* GetEConn(){return mp_EConn;};
	GM2DLineVector GetEndLine( long StartOrEnd, BOOL bShort = TRUE );
//     long AddEndJointLine( MSConnectivity* pJoint ,GM2DPolygon& Section, long nMemPartInJoint);
    //long AddEndJointLine( MSConnLevelZone* pZone ,GM2DPolygon& Section, long nMemPartInJoint);
	long AddEndJointLine( MSConnLevelZone* pZone ,GM2DPolyline& Section, long nMemPartInJoint);
	long AddEndJointLine2( MSConnLevelZone* pZone ,GM2DPolyline& Section, long nMemPartInJoint, long NumMemberG);

	void SetSlabLines();
	void FindFarLine(GM2DLineVector &PartLVec, GM2DLineVector* TheLine, double &LFarDist, double &RFarDist );
    bool GetZLevels(double& TopLevel, double& BotLevel) { return mp_LinkMem->GetZLevels(TopLevel, BotLevel);}
	//bool GetZLevelsbyJoint(double& TopLevel, double& BotLevel, MSConnectivity* pJoint) { return mp_LinkMem->GetZLevelsbyJoint(TopLevel, BotLevel, pJoint);}
	GM2DPolyline* GetBoundary(BOOL bShort = TRUE);
	GM2DPolyline* GetBoundary2(GM2DCurve* pBaseCurve = NULL);
    GM2DPolyline* GetTrimmedBoundary();

	GM2DPolyline* GetMemberBoundary();
	GM2DLineVector GetMemberEndLine( long StartOrEnd);

	GM2DCurve* GetShortLRLine(long StartOrEnd);
	GM2DCurve* GetLongLRLine(long StartOrEnd);

	GM2DCurve* mp_CenLine;
	long m_CenLineSetCnt;
	bool m_IsParallelCenLine;

	GM2DCurve* mp_LLine;
	GM2DCurve* mp_RLine;

	GM2DPolyline* mp_LPolyLine;
	GM2DPolyline* mp_RPolyLine;
    GM2DPolyline* mp_SPolyLine;
    GM2DPolyline* mp_EPolyLine;

    CTypedPtrArray<CObArray, MSConnLevelZone*> ma_StartJointLevel;
    CTypedPtrArray<CObArray, GM2DPolyline*> ma_StartJointEndPoly;
    CTypedPtrArray<CObArray, MSConnLevelZone*> ma_EndJointLevel;
    CTypedPtrArray<CObArray, GM2DPolyline*> ma_EndJointEndPoly;

    void AddEndJointPoly( MSConnLevelZone* MyZone, long StartOrEnd, GM2DPolyline* pPoly );
    void DeleteEndJointPoly(MSConnLevelZone* MyZone, long StartOrEnd);
	void DeleteEndJointPolyAll(long StartOrEnd);

//     GM2DCurve* mp_InitLLine;
//     GM2DCurve* mp_InitRLine;
// 
//     GM2DCurve* mp_LSExtLine;
//     GM2DCurve* mp_LEExtLine;
//     GM2DCurve* mp_RSExtLine;
//     GM2DCurve* mp_REExtLine;

    GM2DLineVector* mp_SEndLine;
    GM2DLineVector* mp_EEndLine;

	vector<GM2DCurve*> ma_LSegments;
	vector<GM2DCurve*> ma_RSegments;
    vector<GM2DCurve*> ma_SSegments;
    vector<GM2DCurve*> ma_ESegments;

    CTypedPtrArray<CObArray, GM2DCurve*> ma_LTrimLine;
    CTypedPtrArray<CObArray, GM2DCurve*> ma_RTrimLine;
    void DeleteTrimLines();

//     bool SetLineXY( long StartOrEnd, char LineSide, GM2DVector& NewPoint, GM2DCurve* ExtLine=NULL, bool bOnlyShorten=false );
    bool PreCheckSetLine( long StartOrEnd, char LineSide, GM2DVector& NewPoint, bool bOnlyShorten=false );
	long SetLineXY( long StartOrEnd, char LineSide, GM2DVector& NewPoint, bool bOnlyShorten=false );
	long SetLineXY2( long StartOrEnd, char LineSide, GM2DVector& NewPoint, bool bOnlyShorten=false, bool bOnlyCross = false );
    void SetEdgeTrim(long StartOrEnd, char LineSide, GM2DVector& LineStartVec);
    void SetLinesOfAMemPart(bool SConn = true, bool EConn = true);
	void SetLRPolyLine(MSConnectivity* pWhichEndConn);
    bool IsWallAndGirderWith(MSLinkMemPart* pUrPart);
    MSLinkMemberG* mp_LinkMem;
	MSLinkMemPart* mp_FloorPartUp;
	MSLinkMemPart* mp_WallPartBelow;
	long mp_FloorPartUp_Idx; // for Serialize Only
	long mp_WallPartBelow_Idx;  // for Serialize Only

	MSSlabG* mp_LSlab;
	MSSlabG* mp_RSlab;

	void Delete();
	bool isDelete(){return m_bDeleted;};
	static void ClearAllDeletedMemParts();
	bool isBuriedLinkMember();
    //void MakeTrimmedPolyline();
    void SetSegments();

    bool IsStart();
    bool IsEnd();

protected:
	static map<MSLinkMemPart*, bool> _deleted_map;
	bool m_bDeleted;

	GM2DPolyline* mp_Boundary;
    GM2DPolyline* mp_TrimmedBoundary;
};
