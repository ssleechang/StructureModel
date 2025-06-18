#pragma once

#include "MSPolySection.h"
#include "MSRcObject.h"
#include "GMLIb/GM2DVector.h"

class StructureModel_CLASS MSEdgeBar : public MSRcObject
{
public:
	MSEdgeBar(void);
	~MSEdgeBar(void);

	// HD13+16@200 : 간격 배근 (+는 교대배근 의미)
	// strBar = "1-3HD19+2-HD19+1-3HD19 : 갯수 배근
	CStringArray ma_strBar;  // 어레이 각 엘리먼트는 철근 단수 의미
};

class StructureModel_CLASS MSEdgeVBar : public MSEdgeBar
{
public:
	MSEdgeVBar(void);
	~MSEdgeVBar(void);

	double m_SEdgeLoc;  // 배근 구간 시작점
	double m_EEdgeLoc;
// 
// 	long m_NormalizedOrLength; 
// 	double m_SLengthLoc;  // 높이 방향 철근 시작 위치. 바닥점 0. 
// 	double m_ELengthLoc;  //
};

class StructureModel_CLASS MSEdgeHBar : public MSEdgeBar
{
public:
	MSEdgeHBar(void);
	~MSEdgeHBar(void);

	GMPolyline m_Shape;  // 꼭지점이 0개일 때는 full stirrup 의미
};

class MSTieBarPoint : public MSRcObject
{
	long m_EdgeIdx;
	long m_BarNum;
};

class StructureModel_CLASS MSEdgeTieBar : public MSEdgeBar
{
public:
	MSEdgeTieBar();
	~MSEdgeTieBar();

	CTypedPtrArray<CObArray, MSTieBarPoint*> ma_BarPoint;
};

class StructureModel_CLASS MSFaceTieBar : public MSEdgeBar
{
public:
	MSFaceTieBar();
	~MSFaceTieBar();

	long m_HBarNumStep; // 수평 방향 띠철근 반복 철근수. 1인 경우 모든 수직근
	long m_VBarNumStep; // 수직 방향 띠철근 반복 철근수. 2인 경우 수평근 하나 건너
	CTypedPtrArray<CObArray, MSTieBarPoint*> ma_BarPoint;  // ma_strBar : 1-D10, 2-D10(U자형 전단보강근)
};

class StructureModel_CLASS MSPSecEdge : public MSObject
{
public:
	MSPSecEdge();
	~MSPSecEdge();

	CTypedPtrArray<CObArray, MSEdgeVBar*> ma_VBar;
};

// 단면 내에 추가로 그려지는 자유에지
class StructureModel_CLASS MSPSecEdgeEx : public MSPSecEdge
{
public:
	MSPSecEdgeEx();
	~MSPSecEdgeEx();

	GM2DVector m_SVec;
	GM2DVector m_EVec;
};

class StructureModel_CLASS MSRcPolySection :	public MSPolySection
{
public:
	MSRcPolySection(void);
	~MSRcPolySection(void);

	CTypedPtrArray<CObArray, MSPSecEdge*> ma_EdgeVBar;  // 폴리곤 꼭지점 갯수 이상의 에지는 자유 에지.
	bool m_bFullStirrup;
	CTypedPtrArray<CObArray, MSEdgeHBar*> ma_EdgeHBar;
	CTypedPtrArray<CObArray, MSEdgeTieBar*> ma_SectTieBar;
	CTypedPtrArray<CObArray, MSFaceTieBar*> ma_FaceTieBar;
};

