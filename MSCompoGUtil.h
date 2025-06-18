#pragma once
#include "afx.h"

#include "MSElement.h"

class MSCompoG;
class MSSubHaunchG;
class GM2DCurve;
class GM2DVector;
class MSFloor;
class MSFloorQT;
class MSElement;
class MSLinkMemberG;
class MSSlabG;
class StructureModel_CLASS MSCompoGUtil
{
public:
	MSCompoGUtil();
	~MSCompoGUtil();

	static GM2DCurve* GetCurveOnProfile(MSCompoG* pCompoG, double x, double y, double x2, double y2);
	static vector<MSSubHaunchG*> MakeHaunchG(MSCompoG* pCompoG, double x, double y, double x2, double y2, double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto);
	static vector<MSSubHaunchG*> MakeHaunchGAllProfile(MSCompoG* pCompoG, double haunchAngle, double haunchY, double haunchY_Top, bool bTopAuto);
	static void FindHaunchTopZ_Auto(MSCompoG* pCompoG, GM2DCurve* pCurve, vector<GM2DVector>& start_list, vector<GM2DVector>& end_list, vector<double>& haunch_top_y_list);
	static void TrimHaunches(vector<MSSubHaunchG*> Haunches, MSFloor* pFloor);

	//오름차순 : ASC, 내림차순 DES
	static void Sort(vector<double>& LocArr, bool isASC);

	static vector<MSCompoG*> FindCompoGInCrossQTArr(GMObject* pGeometry, CTypedPtrArray<CObArray, MSFloorQT*>& CrossQTArr, FindType_MSElement memberType, bool bFindOnlyOnLine = true, bool bCheckOverlap = true, double dTol = DTOL_GM, bool bIncludeOnLine = true);

	//memberType이 msNULL이면 모든 부재 리턴
	static vector<MSCompoG*> FindCompoG(MSFloor* pFloor, GM2DVector* pVec, FindType_MSElement memberType, bool bCheckOverlap = true, double dTol = DTOL_GM);
	static vector<MSCompoG*> FindCompoG(MSFloor* pFloor, GM2DPolyline* pPoly, FindType_MSElement memberType, bool bFindOnlyOnLine = true, bool bCheckOverlap = true, double dTol = DTOL_GM, bool bIncludeOnLine = true);
	static vector<MSCompoG*> FindCompoG(MSFloor* pFloor, GM2DCurve* pCurve, FindType_MSElement memberType, bool bCheckOverlap = true, double dTol = DTOL_GM);
	static vector<MSElement*> FindPolyElement(MSFloor* pFloor, GM2DPolyline* pPoly, bool bFindOnlyOnLine = true, bool bCheckOverlap = true, double dTol = DTOL_GM);

	static vector<MSFloor*> GetFloorArrByFindFloorType(MSCompoG* pCurCompoG, eFIND_FLOOR eFindFloorType);
	static vector<MSCompoG*> FindConnectMember(MSCompoG* pCurCompoG, FindType_MSElement memberType, long nFloorType, double dTol = DTOL_GM, bool bUseTrimedBoundry = true, bool bFindPolyMemberOnlyOnLine = false, bool bIncludeOnLine = true);
	static vector<MSCompoG*> FindConnectMemberByCurve(MSSlabG* pCurSlabG, GM2DCurve* pFindCurve, FindType_MSElement memberType, long nFloorType, double dTol = DTOL_GM, bool bUseInnerBoundry = true, bool bFindPolyMemberOnlyOnLine = false, bool bIncludeOnLine = true);
	static vector<MSCompoG*> FindConnectMember(MSLinkMemberG* pLinkG, CON_POS eConPos, FindType_MSElement memberType, long nFloorType, bool isReverse = false, double dTol = DTOL_GM, bool bUseTrimedBoundry = true, bool bFindPolyMemberOnlyOnLine = false, bool bIncludeOnLine = true);

	static vector<MSCompoG*> FindColinerLinkMember(MSLinkMemberG* pLinkG, CON_POS eConPos, FindType_MSElement memberType, long nFloorType, double dTolDeg, double DistTol, double LinkTestLength, bool isReverse = false, bool bUseTrimedBoundry = true);
	static vector<MSCompoG*> FindOverlapLinkMember(MSLinkMemberG* pLinkG, FindType_MSElement memberType, long nFloorType, double dTolDeg, double DistTol, double LinkTestLength, bool bCheckMemberWidth = false, bool bUseTrimedBoundry = true, bool bInline = true);

	static CON_POS ToConPos(char cLoc);

	static bool IsContainCurveinLinkMember(MSFloor* pFloor, GM2DCurve* pCurve, vector<MSElement::Type_MSCompoG> findTypeArr, double dRatio);
};