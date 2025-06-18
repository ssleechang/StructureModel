#pragma once

#include "MSObject.h"
#include "MSElement.h"
#include "MSCheckModelOption.h"

class MSRebarBuilding;
class MSFloor;
class MSCompoG;
class MSLevelZone;
class MSElement;
class MSSlabG;
class MSBeamG;
class MSColumnG;
class MSColineLinkMemberGs;
class MSObject;
class MSConstructionZone;
class MSSlabRebar;
class MSLinkMemberG;
class MSWallMemberG;
class MSWallOpeningG;
class MSPitG;
class MSBuiltUpBeamG;
class MSGenStairG;
class MSWallFootG;
class MSIsoFootG;
class MSSubHaunchG;
class MSCompoM;
class MSFWSoundAbsorptionG;
class MSFWShadeG;

class StructureModel_CLASS MSModelChecker
{
public:
	MSModelChecker(MSRebarBuilding* pBulid, bool bMakeErrorObject = true, bool bMakeOutputReport = false);
	~MSModelChecker(void);

	enum CheckModelMsgType
	{
		MsgType_ZOverLap = 0,
		MsgType_ZConnected,
		MsgType_BigCompoM,
		MsgType_SupportOnSlab,
		MsgType_SupportOnMember,
		MsgType_ColumnEccOver,
		MsgType_CheckSlab,
		MsgType_Joint,
		MsgType_ConnBeam_ZLevel,
		MsgType_ConnBeam_Width,
		MsgType_ConnBeam_Depth,
		MsgType_DefName,
	};

	int CheckGeometryModel();

	int CheckModel();

	// CheckElement Element의 정보가 잘못되어 있는지 체크 // true: 에러, false: 정상
	static bool CheckElement(MSElement* pElement);
	static bool CheckInValidElement(MSElement* pElement, bool checkSubElement = true);
	static bool checkInValidAddRebarArea(MSCompoG* compoG, bool checkAll);

	static bool CheckInValidElement(MSPitG* pPit, bool checkSubElement);
	static bool CheckInValidElement(MSWallMemberG* pWall, bool checkSubElement);
	static bool CheckInValidElement(MSSlabRebar* pRebar);
	static bool CheckInValidElement(MSBeamG* pBeam, bool checkSubElement);
	static bool CheckInValidElement(MSSlabG* pSlab, bool checkSubElement);
	static bool CheckInValidElement(MSGenStairG* pStair, bool checkSubElement);
	static bool CheckInValidElement(MSColumnG* pColumn, bool checkSubElement);
	static bool CheckInValidElement(MSWallFootG* pWallFoot, bool checkSubElement);
	static bool CheckInValidElement(MSIsoFootG* pIsoFoot, bool checkSubElement);
	static bool CheckInValidElement(MSFWSoundAbsorptionG* pFWSoundAbsorptionG);

	static bool CheckInValidHaunch(MSCompoG* pCompoG, vector<MSSubHaunchG*> Haunches);


	static bool CheckInValidBeamBuiltUp(MSBeamG* pBeam, MSBuiltUpBeamG* pBuiltUp);
	static bool CheckValidMSSlabRebarBentLine(MSSlabRebar* pRebar);

	//nElementType
	// 0 : LevelZone, 1 : CompoG, 2: CompoG1, CompoG2
	static void MakeErrorObject(CString strGroup, CString strErrMsg, long nElementType, long nErrorLevel , long nErrorCode, MSElement* pCompoG1 = NULL, MSElement* pCompoG2 = NULL);

	static bool IsInsertOverLapCompoGsMsgBox(map<MSCompoG*, MSObject*>& OverLapCompoGMap, CString strMsg = _T(""));
	static map<MSCompoG*, MSObject*> CheckCompoGArrOverlap_WithUpDownFloor(MSFloor* pFloor, vector<MSCompoG*>& CompoGArr, bool bMakeErrorObject = true, bool isCheckZLevel = true);
	static map<MSCompoG*, MSObject*> CheckCompoGArrOverlap_WithCurFloor(vector<MSCompoG*>& CompoGArr, bool bMakeErrorObject = true, bool isCheckZLevel = true);
	static map<MSCompoG*, MSObject*> CheckCompoGArrOverlap(vector<MSCompoG*>& CompoGArr);
	static MSObject* CheckCompoGOverlap_WithUpDownFloor(MSFloor* pFloor, MSCompoG* pCompoG, long nOldCompoGID = 0, bool bMakeErrorObject = true, bool isCheckZLevel = true);
	static MSObject* CheckCompoGOverlap(MSFloor* pFloor, MSCompoG* pUrOrgCompoG, long nOldCompoGID = 0, bool bMakeErrorObject = true, bool isCheckZLevel = true, bool bCheckJoint = true);
	static MSObject* CheckSlabGOverlap(MSFloor* pFloor, MSCompoG* pCompoG, long nOldCompoGID = 0);
	static MSObject* CheckSlabOpeningGOverlap(MSFloor* pFloor, MSCompoG* pCompoG, long nOldCompoGID = 0);
	static MSElement* CheckOverlapLevelZone(MSFloor* pFloor, MSLevelZone* pZone);
	static MSConstructionZone* CheckOverlapConstructionZone(MSFloor* pFloor, MSConstructionZone* pZone, long nOldCZoneID = 0);
	static MSSlabRebar* CheckSlabRebarOverlap(MSFloor* pFloor, MSSlabRebar* pUrSlabRebar, long nOldCompoGID, bool bMakeErrorObject=true);
	static MSWallOpeningG* CheckWallOpeningOverlap_ADD(MSWallMemberG* pWall, MSWallOpeningG* pOpening, bool bMakeErrorObject = true);
	static MSWallOpeningG* CheckWallOpeningOverlap_MOD(MSWallMemberG* pWall, bool bMakeErrorObject = true);
	static MSFWShadeG* CheckWallShadeOverlap_ADD(MSWallMemberG *pWall, MSFWShadeG *pShade, bool bMakeErrorObject = true);
	static MSFWShadeG* CheckWallShadeOverlap_MOD(MSWallMemberG *pWall, bool bMakeErrorObject = true);

	static vector<long> CheckExtendOverlap(MSFloor* pFloor, MSCompoG* pCompoG, bool isUpdateGeometry = true);

	static void FixPolylineGap(vector<MSElement*>& elementArr);
	static void FixPolylineGap_Sort(vector<MSElement*>& elementArr);
	static void FixPolylineGap_StartEdit(map<MSElement*, int>& mapProcessed, MSFloor* floor, MSElement* Element);
	
	static bool CheckModel_WallOpening(MSWallMemberG* pWall, bool bMakeErrorObject = true);
	static bool CheckModel_WallOpeningStep2(MSWallMemberG* pWall, MSWallOpeningG* pOpening, bool bMakeErrorObject = true);

	static bool HasOverLapCompoG(MSCompoG* pCompoG, long nOldCompoGID = 0);
	//static MSColineLinkMemberGs* MakeColineLinkMemberG(MSLinkMemberG* pFindLinkG, MSElement::Type_MSCompoG eType, map<long, MSLinkMemberG*>& beamMap);

	static vector<long> FindMinMemberProfileArea(MSFloor* pFloor, MSElement::Type_MSCompoG eType, int MinArea);
	static XErrorObject* CheckModel_MakeErrObject(MSElement* pElement, eCheckModel eBaseOpt, long eSecondOpt);

	static bool CheckLength(double dValue1, CString sSymbol, double dValue2);
private:
	MSRebarBuilding* mp_Build;
	long m_nBuildElementCnt;
	long m_nCheckCnt;
	int m_nlast;

	CString m_strGroup;
	CString m_strErrorMsg;

	int CheckGeometryModel_Floor(MSFloor* pFloor);
	int CheckGeometryModel_CompoM(MSCompoG* pCompoG);
	int CheckGeometryModel_CompoG(MSCompoG* pCompoG);
	int CheckGeometryModel_LinkMember(MSCompoG* pCompoG);
	int CheckGeometryModel_PointMember(MSCompoG* pCompoG);
	int CheckGeometryModel_Profile(MSElement* pCompoG);
	int CheckGeometryModel_FindSlabGap(MSCompoG* pCompoG);
	int CheckGeometryModel_LevelZone(MSLevelZone* pLevelZone);
	int CheckGeometryModel_LevelZoneDist(MSLevelZone* pLevelZone, int iZone = -1);



	long CheckModel_Floor(MSFloor* pFloor, bool bCheckVerticalMember);
	bool CheckModel_DefName(MSCompoG* pCompoG, CheckModelOpt &CheckFlag);
	bool CheckModel_PolyElement(MSElement* pElement, CheckModelOpt &CheckFlag);
	bool CheckModel_PolyElement_Profile(MSElement* pElement, CheckModelOpt &CheckFlag);
	bool CheckModel_PolyElement_Dist(MSElement* pElement, CheckModelOpt &CheckFlag);
	bool CheckModel_Slab_Area(MSElement* pElement, CheckModelOpt &CheckFlag);
	bool CheckModel_Slab_LevelDif(MSSlabG* pSlab, CheckModelOpt &CheckFlag);
	bool CheckModel_Support(MSCompoG* pCompoG, CheckModelOpt &CheckFlag);
	bool CheckModel_SupportOnMember(MSCompoG* pCompoG, MSFloor* pLowFloor, CheckModelOpt &CheckFlag);
	bool CheckModel_ColumnM(MSColumnG* pColumn, MSColumnG* pDnColumn, CheckModelOpt &CheckFlag);
	bool CheckModel_ColumnEccOver(MSCompoG* pCompoG, MSCompoG* pDnCompoG, CheckModelOpt &CheckFlag);
	bool CheckModel_ZLevel_ConnectedOverLap(MSCompoG* pCompoG, MSCompoG* pDnCompoG, CheckModelOpt &CheckFlag); // OverLap, Connected 체크 같이 함
	bool CheckModel_SupportedOnMemberInFloor(MSFloor* pFloor, MSCompoG* pCompoG, int nFloorType, vector<MSElement::Type_MSCompoG> CheckTypeArr, CheckModelOpt &CheckFlag);
	void CheckModel_MakeBeamLine(MSBeamG* pBeam, map<long, MSLinkMemberG*>& beamMap, vector<MSColineLinkMemberGs*>& BeamLineArr);
	long CheckModel_BeamLines(vector<MSColineLinkMemberGs*>& BeamLineArr);
	bool CheckModel_LineMemberOutLen(MSCompoG* pCompoG, CheckModelOpt& CheckFlag);
	bool CheckModel_HorOverLap(MSLinkMemberG* pLink, CheckModelOpt& CheckFlag);
	bool CheckModel_VerMember_Contain(MSCompoG* pCompoG, CheckModelOpt &CheckFlag);
	bool CheckModel_SameMember_OverLap(MSCompoG* pCompoG, CheckModelOpt &CheckFlag);

	void CheckModel_Connection(MSCompoG* pCompoG, CheckModelOpt &CheckFlag, bool bCheckCantil = true);
	
	void SetCheckModelOptFlag(double dVal, CheckModelOpt &CheckFlag, eCheckModel eBaseOpt, long eSecondOpt);

	long CheckCompoM(MSFloor* pFloor);

	long CheckModel_MakeErrorObjects(MSElement* pElement, CheckModelOpt& CheckFlag);
	long CheckModel_MakeErrObjectbyCheckModel(MSElement* pElement, CheckModelOpt& CheckFlag, eCheckModel eBaseOpt, long nCnt);
	

	void InitProgressReporter(CString strTitle = _T("Processing Check Model Start..."));
	void UpdateProgressReporter();
	void CloseProgressReporter();

public:
	static bool CheckModel_MSCompoM(MSCompoM* pCompoM);

private:
		static bool m_bMakeErrorObject;
		static bool m_bMakeOutputReport;
public:
	static bool GetIsMakeErrorObject();
	static void SetIsMakeErrorObject(bool value);
	static bool GetIsMakeOutputReport();
	static void SetIsMakeOutputReport(bool value);

	static CString GetStrGroupName(MSElement* pElement);
};

