#pragma once
#include "afx.h"

#include "GMLib/GM2DRectangle.h"
#include "MSElement.h"

class MSLinkMemberG;
class MSCompoG;
//class MSElement;

class MSFloorQT :
	public CObject
{
public:
	static double sm_BoundMargin;
	static long m_MaxDepth;
	static long m_MinSize;

	MSFloorQT(long MinSize);
  MSFloorQT(long MinSize, GM2DVector Origin, GM2DVector Corner);
  ~MSFloorQT(void);

  GM2DRectangle m_Rect;
  long m_IsRoot;

	CArray<long, long> ma_Route;
	long GetRoute();

	CTypedPtrArray<CObArray, MSCompoG*> ma_FloorMem;   // IsoFoot, LineSupportMember
	void AddFloorMem(MSCompoG* pLinkMem);

	CTypedPtrArray<CObArray, MSCompoG*> ma_VerticalMem; // column, Wall
	void AddVerticalMem(MSCompoG* pCompoG);

public:
  MSFloorQT* m_pChildren[4];
  long m_IsLeaf;

  void Setup();
  void Build(double dTol);  
  void FindBoundsRect(double dTol);
  void BuildLeaves(long Zone, double dTol);
  void CreateChildren();

  void GetXYMinMax(vector<GM2DVector*>& VerArr, double& dXMin, double& dYMin, double& dXMax, double& dYMax);
  void SetBoundsRect(MSCompoG* pCompoG);

  void Remove(MSCompoG* pCompoG);

  void AddToLeaves( MSCompoG* pNewCompoG,bool bCreateChildrenWhenNeeded=true, bool bResetFirst = false );
  void FindCrossQTs( GM2DLineVector& aLVec, CTypedPtrArray<CObArray, MSFloorQT*>& CrossQTArr, bool bResetFirst );
  void FindCrossQTs( MSElement* pCompoG, bool bResetFirst );
  void FindCrossQTs( MSElement* pCompoG, CTypedPtrArray<CObArray, MSFloorQT*>& CrossQTArr, bool bResetFirst);
  void FindCrossQTs( GM2DVector* pVec, CTypedPtrArray<CObArray, MSFloorQT*>& CrossQTArr, bool bResetFirst );
  void FindCrossQTs( GM2DCurve* pCurve, CTypedPtrArray<CObArray, MSFloorQT*>& CrossQTArr, bool bResetFirst );
  void FindCrossQTs( GM2DRectangle* pRect, CTypedPtrArray<CObArray, MSFloorQT*>& CrossQTArr, bool bResetFirst );

  void FindCrossQTs(GMObject* pGeometry, CTypedPtrArray<CObArray, MSFloorQT*>& CrossQTArr, bool bFindOnlyOnLine, bool bResetFirst);

  vector<MSCompoG*> FindCompoG(GMObject* pGeometry, FindType_MSElement memberType, bool bFindOnlyOnLine = true, bool bCheckOverlap = true, double dTol = DTOL_GM, bool bIncludeOnLine = true);
  
};
