#pragma once
#include "dg2dobject.h"

class GMQuadTree;
class GM2DGeometry;
class GM2DArc;

enum {NM_LINKED0=1, NM_LINKED1=2, NM_GAPPED0=4, NM_GAPPED1=8, NM_STAGGERED0=16, NM_STAGGERED1=32};

class StructureModel_CLASS DG2DLine : public DG2DObject
{
public:
	DG2DLine(GM2DLineVector* BaseLine, GMQuadTree* pQT);
	DG2DLine( GM2DArc* BaseArc , GMQuadTree* pQT);
	~DG2DLine(void);

	double Length();
	double Distance(const GM2DVector& AVec, bool bSigned=false);
	GM2DVector PointAtParam(double LocValue);
	long Intersect(DG2DLine* pUrLine, GM2DVector& CrossVec, double* MyLoc, double* UrLoc);
	double OrthoBaseLocValue(const GM2DVector& AVec);
	GM2DVector OrthoBaseVector(const GM2DVector& AVec);
	bool SetP0(GM2DVector& NewP0);
	bool SetP1(GM2DVector& NewP1);
	GM2DVector DirVector();
	long IsParallel4DTol(DG2DLine& ALineVec, double dTol=1.0);
	long IsColinear4DTol(DG2DLine& ALineVec, double DegreeTol=1.0, double DistTol=1.0);
	long WhichSide( GM2DVector& PntVec, double dTol=DTOL_GM );
	BOOL OverlapTest4DTol(DG2DLine& RefLineVec, double dTol=1.0);


	void Init();
	double PairLength();
	BOOL   IsSameDirWithPair(long PairIdx); 
	BOOL   IsSameDir(DG2DLine* UrLine);
	BOOL   IsSameDir(GM2DLineVector* UrLine);

	void AddGMLine(GM2DGeometry* pNewLine);
	void AddQTs(GM2DGeometry* pNewLine);
	BOOL AddPairLine(DG2DLine* PairLine, double Loc0, double Loc1, long Result, double GapTol, double LinkTestTol=1.);
	BOOL AddPairWall(DG2DLine* PairLine, double Loc0, double Loc1, long OverlapResult, double GapTol);
	void InsertProperty(DG2DLine* PairLine, double Loc0, double Loc1, BOOL bSingleLine=FALSE);
	void RemovePairLine(long Idx);
	void RemovePairLineByLine( DG2DLine* ALine );

	long m_ID;
	GM2DCurve* m_pLine;
	GM2DVector m_P0;
	GM2DVector m_P1;
	GMQuadTree* m_pQT;
	GM2DLineVector* GetLVec();
	GMArc* GetArc();

	CTypedPtrArray<CObArray, DG2DLine*> ma_Pair; // ma_Pair본 DGLine에 포함되며 min, max 간격안에서 평행한 DGLine 들
	CArray<double, double> ma_PairLoc0;
	CArray<double, double> ma_PairLoc1;
	CArray<BOOL, BOOL> ma_bSingleLine;
	BOOL m_bSingleLine;

	unsigned short m_SharePntMask; // 아래 각 라인의 연결점 비트 0 또는 1
	GM2DLineVector* m_LinkColLVec0;
	GM2DLineVector* m_LinkColLVec1;
	DG2DLine* m_Linked0; // NM_LINKED0=1
	DG2DLine* m_Linked1; // NM_LINKED1=2
	DG2DLine* m_Gapped0; // NM_GAPPED0=4
	DG2DLine* m_Gapped1; // NM_GAPPED1=8
	DG2DLine* m_Staggered0; // NM_STAGGERED0=16
	DG2DLine* m_Staggered1; // NM_STAGGERED1=32

	void SetMask(long Kind, long Value);
	long GetMask(long Kind);

// 0포인트(시작점), 1포인트(끝점)에 링크라인의 다른 끝점
	GM2DVector* EndOfLink(long StartOrEnd);
// 나의 링크라인의 다른 끝점. 링크라인 아닌 경우 NULL
	GM2DVector* EndOfGivenLink(DG2DLine* pLinkLine);
	BOOL SetLinkLine(long StartOrEnd, DG2DLine* ALine, double dTol);
	BOOL ReleaseLinkLine(DG2DLine* UrLine);
	BOOL GetLinkLine(long StartOrEnd, GM2DLineVector& LinkLVec);
	DG2DLine* GetLinkLine(long StartOrEnd);
	DG2DLine* GetLinkNext(long StartOrEnd, bool bIncludeGap=TRUE, bool bIncludeStagger=TRUE);
	DG2DLine* GetGapEndLinkLine(long StartOrEnd);

	BOOL SetGapLine(long StartOrEnd, DG2DLine* ALine, double DegreeTol, double DistTol, double StepTol, double GapTol);
	BOOL GetGappedLine(long StartOrEnd, GM2DLineVector& LinkLVec);
	BOOL GetGapLVec(long StartOrEnd, GM2DLineVector& LinkLVec);
	BOOL SetStaggerLine(long StartOrEnd, DG2DLine* ALine, double DegreeTol, double StepTol, double GapTol);
	long GetLinkState();
	long GetColLinkState();
	long GetGapState();
	long GetStaggerState();
	BOOL GetCenterLVec(GM2DLineVector& CenLVec, long PairNum=-1);
	GM2DVector GetMidVectorNearest(GM2DVector* pVec, DG2DLine* UrLine);
	BOOL IsPair(DG2DLine* UrLine);
	BOOL IsLink(DG2DLine* UrLine);
	BOOL IsGapped(DG2DLine* UrLine);

	BOOL ExistCrossWall(long StartOrEnd, DG2DLine* UrLine, double DegreeTol, double StepTol);
	BOOL ExistCrossColumn(long StartOrEnd, DG2DLine* UrLine, double DegreeTol, double StepTol);
	long CheckYJoint(long StartOrEnd, GM2DVector& CrossPnt);
	long CheckEccCrossJoint(long StartOrEnd, GM2DVector& CrossPnt);
	BOOL GetCrossPointWithLink(long StartOrEnd, GM2DVector& CrossPnt);
	DG2DLine* GetAPairLine(GM2DVector& LocVec);
	DG2DLine* GetLinkNext(DG2DLine* pLinkLine);
	BOOL MergeWhenColinear1(GM2DLineVector* pNewLine, double DegreeTol, double DistTol, double linkTestLength);
	BOOL MergeWhenColinear2(GM2DLineVector* pNewLine, double DegreeTol, double DistTol, double linkTestLength);
	unsigned short m_TypeOfLink; 

};
