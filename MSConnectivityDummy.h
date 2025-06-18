#pragma once
#include "msobject.h"

#include "GMLib\GM2DPolygon.h"
#include "GMLib\GM2DPolyline.h"
#include "GMLib\GM2DVector.h"

class GM2DCurve;
// Segment Information for DrawLines
typedef struct _SegStruct
{
public:
	_SegStruct()
	{
		Initialize();
	};
	_SegStruct(const _SegStruct& aData)	{*this = aData;};
	void Serialize(CArchive& ar)
	{
		if(ar.IsStoring())
		{
			ar << dDeg;
			ar << MemPartIdx;
			ar << isLLine;
			ar << isEndLine;
			GM2DCurve::SerializeWithNullFlag(ar, pCurve);
		}
		else
		{
			ar >> dDeg;
			ar >> MemPartIdx;
			ar >> isLLine;
			ar >> isEndLine;
			GM2DCurve::SerializeWithNullFlag(ar, pCurve);
		}
	};
	_SegStruct& operator = (const _SegStruct& aData)
	{
		Initialize();
		dDeg = aData.dDeg;
		MemPartIdx = aData.MemPartIdx;
		pCurve = aData.pCurve;
		isLLine = aData.isLLine;
		isEndLine = aData.isEndLine;
		return *this;
	};
	void Initialize()
	{
		dDeg = 0;
		MemPartIdx = -1;
		pCurve = NULL;
		isLLine = true;
		isEndLine = false;
	};

public:
	double dDeg;
	long MemPartIdx;
	bool isLLine;
	bool isEndLine;
	GM2DCurve* pCurve;

}SegStruct;

class MSFloor;
class MSFloorQT;
class MSLinkMemberG;
class MSColumnG;
class MSIsoFootG;
class MSJointMemberG;
class MSConnLevelZoneDummy;
class MSLinkMemPartDummy;

class MSSlabG;
class MSGenStairG;

class StructureModel_CLASS MSConnectivityDummy : public MSObject
{
public:
	MSConnectivityDummy(void);
	virtual ~MSConnectivityDummy(void);

	void Serialize( CArchive& archive );
	static void Serialize_AsID(CArchive& ar, MSConnectivityDummy*& con);
	static bool JOINT_SERIALIZE();

	long m_ID;
	long m_nFloorNum;
	double m_dXc, m_dYc;
	
	MSFloor* mp_Floor;
	MSConnectivityDummy* mp_Source;
	vector<MSConnectivityDummy*> ma_Clone;

	MSConnectivityDummy* mp_UpFloorJoint;
	MSConnectivityDummy* mp_DownFloorJoint;

	vector<MSConnectivityDummy*> ma_UpLinkJoint;
	vector<MSConnectivityDummy*> ma_DownLinkJoint;

	CTypedPtrArray<CObArray, MSFloorQT*> ma_CrossQT;
	CTypedPtrArray<CObArray, MSConnLevelZoneDummy*> ma_LevelZone;
	CTypedPtrArray<CObArray, MSConnLevelZoneDummy*> ma_LevelZoneFloor;
	vector<MSLinkMemPartDummy*> ma_MemPart;
	vector<double> ma_MemPartDegree;
	vector<long> ma_MemPart_Idx; // For Serialize Only

	vector<MSLinkMemberG*> ma_SptMember;
	vector<double> ma_Degree;

	CTypedPtrArray<CObArray, MSLinkMemberG*> ma_BuriedLinkMember;

	MSColumnG* mp_Column;
	MSIsoFootG* mp_IsoFoot;
	MSJointMemberG* mp_JointMember;

	vector<MSLinkMemberG*> ma_MidJointMember;//by LSS  MidJoint처리를 위해
	vector<MSSlabG*> ma_SlabMember;
	vector<MSGenStairG*> ma_StairMember;

	GM2DPolyline* mp_DefaultSection;

	bool m_bDeleted;
};

class StructureModel_CLASS MSConnLevelZoneDummy :
	public MSObject
{
public:
	MSConnLevelZoneDummy(MSConnectivityDummy* pConn, double TopLevel, double BotLevel);
	~MSConnLevelZoneDummy(void);
	void Serialize( CArchive& archive );
	void RecoverPointerAfterSerialize(MSFloor* floor);


	MSConnectivityDummy* mp_Conn;
	double m_TopLevel;
	double m_BotLevel;

	GM2DPolyline m_Section;
	GM2DPolyline m_DrawSection;
	bool m_isBaseSec;
	bool m_UseTramInOut;

	bool m_hasBuriedLinkMember;
	bool m_hasArcLinkMember;
	long m_hasArcLinkMemPartCnt;
	map<long, long> m_ColinerMemMap;

	CTypedPtrArray<CObArray, MSLinkMemPartDummy*> ma_MemPart;
	CTypedPtrArray<CObArray, MSLinkMemPartDummy*> ma_BuriedMemPart;
	vector<long> ma_MemPart_idx; // For Serialize Only
	vector<long> ma_BuriedMemPart_idx; // For Serialize Only

	vector<SegStruct> ma_SegList;

	MSColumnG* mp_Column;
	MSIsoFootG* mp_IsoFoot;

	bool m_HasLevelDifG;
	bool m_bUsedDownLinkJoint;
	bool m_hasColinearDownMemPart;
	GM2DPolyline* mp_DownLinkJointBoundry;
private:
	long m_nNumMemberG;
	bool m_bDeleted;
};

class StructureModel_CLASS MSLinkMemPartDummy :
	public MSObject
{
public:
	MSLinkMemPartDummy(MSLinkMemberG* pLineMember = NULL);
	~MSLinkMemPartDummy(void);

	void Serialize( CArchive& archive );
	static void Serialize_AsID( CArchive& archive, MSLinkMemPartDummy*& part, long& rIdx);

	MSLinkMemberG* mp_LinkMem;
	MSConnectivityDummy* mp_SConn;
	MSConnectivityDummy* mp_EConn;

	GM2DCurve* mp_CenLine;
	long m_CenLineSetCnt;
	bool m_IsParallelCenLine;

	GM2DCurve* mp_LLine;
	GM2DCurve* mp_RLine;

	GM2DPolyline* mp_LPolyLine;
	GM2DPolyline* mp_RPolyLine;
	GM2DPolyline* mp_SPolyLine;
	GM2DPolyline* mp_EPolyLine;

	CTypedPtrArray<CObArray, MSConnLevelZoneDummy*> ma_StartJointLevel;
	CTypedPtrArray<CObArray, GM2DPolyline*> ma_StartJointEndPoly;
	CTypedPtrArray<CObArray, MSConnLevelZoneDummy*> ma_EndJointLevel;
	CTypedPtrArray<CObArray, GM2DPolyline*> ma_EndJointEndPoly;

	GM2DLineVector* mp_SEndLine;
	GM2DLineVector* mp_EEndLine;

	vector<GM2DCurve*> ma_LSegments;
	vector<GM2DCurve*> ma_RSegments;
	vector<GM2DCurve*> ma_SSegments;
	vector<GM2DCurve*> ma_ESegments;

	CTypedPtrArray<CObArray, GM2DCurve*> ma_LTrimLine;
	CTypedPtrArray<CObArray, GM2DCurve*> ma_RTrimLine;

	MSLinkMemPartDummy* mp_FloorPartUp;
	MSLinkMemPartDummy* mp_WallPartBelow;
	long mp_FloorPartUp_Idx; // for Serialize Only
	long mp_WallPartBelow_Idx;  // for Serialize Only

	MSSlabG* mp_LSlab;
	MSSlabG* mp_RSlab;

protected:
	GM2DPolyline* mp_Boundary;
	GM2DPolyline* mp_TrimmedBoundary;
};