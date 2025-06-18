#pragma once
#include "msobject.h"

#include "MSElement.h"
#include "GMLib\GM2DPolygon.h"
#include "GMLib\GM2DPolyline.h"
#include "GMLib\GM2DVector.h"
#include "MSLinkMemberG.h"

class MSColumnG;
class MSIsoFootG;
class MSWallMemberG;
class MSLineMemberG;
class MSLinkMemPart;
class MSConnectivity;
class MSCompoG;

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


class StructureModel_CLASS MSConnLevelZone :
    public MSObject
{
public:
    MSConnLevelZone(MSConnectivity* pConn, double TopLevel, double BotLevel);
    ~MSConnLevelZone(void);
	void Serialize( CArchive& archive );
	void RecoverPointerAfterSerialize(MSFloor* floor);
	MSConnLevelZone* CopyOnlyTopBot();

    MSConnectivity* mp_Conn;
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
	
    CTypedPtrArray<CObArray, MSLinkMemPart*> ma_MemPart;
	CTypedPtrArray<CObArray, MSLinkMemPart*> ma_BuriedMemPart;
	vector<long> ma_MemPart_idx; // For Serialize Only
	vector<long> ma_BuriedMemPart_idx; // For Serialize Only

	//vector<double> ma_SegDeg;
	//vector<long>   ma_SegMemPartIdx;
	//vector<GM2DCurve*> ma_SegCurve;
	vector<SegStruct> ma_SegList;

    MSColumnG* mp_Column;
    MSIsoFootG* mp_IsoFoot;

	bool m_HasLevelDifG;
	bool m_bUsedDownLinkJoint;
	bool m_hasColinearDownMemPart;
	GM2DPolyline* mp_DownLinkJointBoundry;

	double GetMemPartByWidth(bool isMax);
	double GetMemPartByMinWidth();
	double GetMemPartByMaxWidth();

    bool BuildSection(bool bCheck = false);
	bool BuildSectionByDownJoint();
    GM2DPolyline* GetSection();
	GM2DPolyline* GetDrawSection();
    bool BuildSectionOnGivenMemPart(GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr);
	bool BuildSectionOnGivenMemPart1(GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr);
	bool BuildSectionOnGivenMemPart2(GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr);
	bool MakeMemPartEndJointPoly(GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr);
	bool MakeMemPartEndJointPoly2(GM2DPolyline& TheSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr);
    
	long CutLineMemPart(GM2DCurve* pMyLine, GM2DCurve* pUrLine, MSLinkMemPart* pMyMember, MSLinkMemPart* pUrMember, long MyEndNum, long UrEndNum, double ElongDegree=60.);

	long CutParallelLine2(MSLinkMemPart* pMyMember, char MyLineSide, GM2DCurve* pMyLine, long MyEndNum, MSLinkMemPart* pUrMember, char UrLineSide, GM2DCurve* pUrLine, long UrEndNum, bool nOnlyShort);
	long CutLineWhenShortening2(MSLinkMemPart* pMyMember, char MyLineSide, GM2DCurve* pMyLine, long MyEndNum, MSLinkMemPart* pUrMember, char UrLineSide, GM2DCurve* pUrLine, long UrEndNum, bool bOnlyInCross=true, bool bCutWhenShorten=true, double ElongDegree=60.);
	bool TrimSection2_3(GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr, bool bCutMemberLine=true);
	bool TrimSection2_2(GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr, bool bCutMemberLine=true);
	bool TrimSection22(GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr, bool bCutMemberLine=true);
	bool TrimSection2(GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr, bool bCutMemberLine=true);
	bool TrimSection3(GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr);
    bool TrimSection4(GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr);

    void MakeConnProfileWhenCross( SegStruct * MySegData, SegStruct * UrSegData );
    void MakeConnProfileWhenParallel( SegStruct * MySegData, SegStruct * UrSegData, GM2DVector Center, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr );
    void MakeConnProfileWhenCoLinear( SegStruct * MySegData, SegStruct * UrSegData, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr );

    void TrimMemPartSegment(vector<GM2DCurve*>& Segments, GM2DVector MyCrossVec, MSLinkMemPart* pMyMemPart, MSLinkMemPart* pUrMemPart);

	bool TrimSection(GM2DPolyline& TheSection, GM2DPolyline& DrawSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr, bool bCutMemberLine=true);
	bool TrimSectionByDownLinkJoint(GM2DPolyline& TheSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr);
	long CutLineWhenShorteningByBuried(MSLinkMemPart* pMyMember, MSLinkMemPart* pUrMember);
	long CutLineWhenShortening(MSLinkMemPart* pMyMember, char MyLineSide, GM2DCurve* pMyLine, long MyEndNum, MSLinkMemPart* pUrMember, char UrLineSide, GM2DCurve* pUrLine, long UrEndNum, bool bOnlyInCross=true, bool bCutWhenShorten=true, double ElongDegree=60.);
	
    long CutLineByEndLine(MSLinkMemPart* pMyMember, char MyLineSide, GM2DCurve* pMyLine, long MyEndNum, GM2DCurve* pUrEndLine, bool bCutWhenShorten);
    bool CheckLevelOverlap(MSCompoG* pCompoG);
    long CutParallelLine(MSLinkMemPart* pMyMember, char MyLineSide, GM2DCurve* pMyLine, long MyEndNum, MSLinkMemPart* pUrMember, char UrLineSide, GM2DCurve* pUrLine, long UrEndNum, bool nOnlyShort);

    bool TrimByAMemPart(MSLinkMemPart* pMyPart, GM2DPolyline& TheSection, CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr);
	void GetSectionPolyLine(GM2DPolyline& APolyLine);

	long GetNumMemberG();
	void SetNumMemberG();
	
	void ReSetMemPartVec(CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr);

	vector<MSConnectivity*> GetDownLinkJoint();
	//MSConnectivity* GetDownLinkJoint();
	void DeleteDownLinkJointPoly();
	void MakeBasePoly(GM2DPolyline& TheSection, bool isBaseSec, long MemPartIdx = 0);
	void BuildSectionOnGivenMemPartByDownJoint(GM2DPolyline& TheSection);
	
	bool HasHandRail();
    void FilletInsideSegment( vector<GM2DCurve*>& Segments, MSLinkMemberG* pUrLinkMem );
    void FilletTriangularProtrusion( vector<GM2DCurve*>& Segments, MSLinkMemberG* pLinkMem, GM2DPolyline* pUrPoly);
    void DeleteSegmentByIndex( vector<GM2DCurve*>& Segments, vector<long> index);
    long CheckEndVectors( GM2DVector Seg1SVec, vector<GM2DVector> endVecs );
    
    GM2DVector CalculateSegmentList( CTypedPtrArray<CObArray, MSLinkMemPart*>& MemPartArr);
    void GatherSegmentList( vector<GM2DCurve*> Segments, long iPart, bool LLine, bool EndLine );
    void SortSegments(GM2DVector Center);
    void FilterEndLines( MSLinkMemPart* pMyMemPart, bool start );

	// 묻힌부재가 조인트 안에 포함되면 트림에서 제외시키고 다시 트림한다.
	void CheckBuriedMember();

	void CheckBasePoly();

	void DeleteEndJointPoly();
	void Delete();
	bool IsDeleted() { return m_bDeleted; };
	static void ClearAllDeleted();
private:
	long m_nNumMemberG;
	bool m_bDeleted;
	static map<MSConnLevelZone*, bool> _deleted_map;
};

