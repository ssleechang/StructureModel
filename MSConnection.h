#pragma once
#include "msobject.h"

#include "StructureModelEnum.h"

class MSCompoG;
class MSLinkMemberG;
class GM2DPolyline;
class HistoryManager;
class MSFloor;

class StructureModel_CLASS MSConnection : public MSHistoryObject
{
	
public:
	enum TRIM_OPTION {TO_DEFAULT, TO_ORIGIN, TO_EXTENDCUT, TO_ENDCUT, TO_IGNORE, TO_MITER, TO_SQUAREOFF,            TO_EXTENDORIGIN, TO_DIRECT, TO_NOHIDDEN=1024};
	enum CON_ANGLE {RIGHT=90, ACUTE=45, OBTUSE=135, STRAIGHT=180, ZERO=1024};

	static TRIM_OPTION ms_DefaultLinkGOption;
	static TRIM_OPTION SetDefaultLinkGOption(TRIM_OPTION DefaultLinkGOption);

	static CString HistoryID(MSCompoG* pFirstCompoG, MSCompoG* pSecondCompoG);
	//생성시는 first/second반영, 이미 생성되어 있을 경우 HistoryManager에서 first/second상관없이 반환
	static MSConnection* GetConnection(MSCompoG* pFirstCompoG, MSCompoG* pSecondCompoG);
	static MSConnection* GetEmptyConnection();
	
	//일반 생성자는 non-historic mode로 사용하기 위한 것이다. 일반 delete를 사용해서 지운다.
	
	MSConnection(MSCompoG* pFirstCompoG, MSCompoG* pSecondCompoG);
	~MSConnection();
	void Serialize( CArchive& archive );
	virtual void RecoverPointerAfterCopyNPaste(map<long, MSCompoG*>& OldCompoGMap);
	virtual void RecoverPointerAfterSerialize(MSFloor* floor);
	bool RecoverPointerAfterSerialize_ForSubMembers(vector<MSCompoG*>& aSubMember);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual void CopyFromMe(MSObject* _pSource, bool bNewID=true);

	bool MakeIntersectBoundary();
	
	//함수 분리 하였음 
	//bool MakeTrimmedBoundary(); // 사용안하는 함수임
	void MergeIntersectToBoundary(MSCompoG* pCompoG);
	void MergeIntersectToBoundary_After(MSCompoG* pCompoG);
	void MergeIntersectToBoundary_DoExtendTrim(MSCompoG* pMyCompoG, GM2DPolyline* pMyTrimmedBoundary, GM2DPolyline* pIntersect);
	void RemoveInwardWedge_Old(MSCompoG* pCompoG);
	
	TRIM_OPTION GetTrimOption(MSCompoG* pCompoG);
	TRIM_OPTION GetFirstTrimOption();
	TRIM_OPTION GetSecondTrimOption();
	void SetFirstTrimOption(TRIM_OPTION Opt);
	void SetSecondTrimOption(TRIM_OPTION Opt);

	double GetLinkMemberLoc(MSCompoG* pCompoG);
	void SetLinkMemberLoc(MSCompoG* pCompoG, double loc);
	double GetLinkMemberLocTol(MSCompoG* pCompoG);

	CON_POS GetLinkMemberConPos(MSCompoG* pCompoG);
	void SetLinkMemberConPos(MSCompoG* pCompoG, CON_POS pos);
	bool IsEndConnection(MSCompoG* pCompoG);

	void SetBoundaryVisibilities_Old( MSCompoG* mp_FirstCompoG, MSCompoG* mp_SecondCompoG );
	void SetTrimOption(TRIM_OPTION FirstTrimOption=TO_DEFAULT, TRIM_OPTION SecondTrimOption=TO_DEFAULT);
	void AdjustTrimOptionAfter();
	//bool MakeTrimmedBoundary4Colinear();
	
	MSCompoG* GetPartner(MSCompoG* pCompoG);
	void RemoveOutwardWedge_Old( MSCompoG* mp_FirstCompoG );
	void CalculateContainedAngle();
	void CalculateLinkMemberTrimOption();
	bool IsCase_LineExtendToArc(MSLinkMemberG* line, MSLinkMemberG* arc);
	int GetConAngle();
	void GetOverlapPoly(GM2DPolyline& OverlapPoly);
	void UpdateTrimOptionsForUserBoundary();
	void SwitchFirstSecond();

	bool SetSteelMemberTrimOption();

	long GetFirstCompoGID();
	long GetSecondCompoGID();
	void SetFirstCompoG(MSCompoG* pCompoG);
	void SetSecondCompoG(MSCompoG* pCompoG);
	MSCompoG* GetFirstCompoG();
	MSCompoG* GetSecondCompoG();
	void SetHistoryID();

	double GetInnerDeg(){return m_dInnerDeg;};
	void SetInnerDeg(double innerDeg){ m_dInnerDeg = innerDeg; };

	bool IsParallel(){return m_IsParallel;};
	void SetIsParallel(bool isParallel){ m_IsParallel = isParallel; };

	static void SetConnPos(CON_POS& eConPos, double dLoc, double dLocTol, GM2DLineVector MySLine, GM2DLineVector MyELine, GM2DPolyline* pPoly);
	static void SetConnPos(CON_POS& eConPos, double dMyLoc, double dMyLocTol, double dUrLoc, MSLinkMemberG* pMyLink, MSLinkMemberG* pUrLink);

	static bool CONNECTION_SERIALIZE();
	static bool* IGNORE_HISTORY();

	static bool CalculateFirstSecondCompoG(MSCompoG*& pFirstCompoG, MSCompoG*& pSecondCompoG);

	bool m_SetByUser;

	void DeleteIntersects();
	void DeleteFiller();

	void CheckIntersect();
	double GetPointMemberArea();

	vector<GM2DPolyline*>& GetIntersects(){ return ma_Intersect;};
	void SetIntersects(vector<GM2DPolyline*>& intersects);
	bool IsSameFloor();

	bool IsErrorPos();

	bool IsSameConnection(MSConnection* pUrConn);
	CString GetLogingConnection();

	bool HasIntersect();
	void UpdateMembers();

	virtual void SetDeleted(bool isDeleted);
	void SubtractSecondTrimmedBoundary();

	bool IsLevelTouch();
	bool IsRightAngle();
	void SetDefTrimOption();
	bool HasTrimOption(TRIM_OPTION Opt);
	bool CompareTo(MSConnection* pTargetConn);
	
	void SetPointMemberArea();
protected:
	//HistoryObject는 HistoryManager를 통해서만 만들 수 있다.
	//historyID는 static MSConnection::HistoryID를 통해서 만든다.
	MSConnection(CString& historyID, MSCompoG* pFirstCompoG, MSCompoG* pSecondCompoG);

	void Initialize(MSCompoG* pFirstCompoG, MSCompoG* pSecondCompoG);


	vector<GM2DPolyline*> ma_Filler;
	vector<GM2DPolyline*> ma_Intersect;
	int m_ConAngle;
	bool m_IsParallel;


private: 

	// Dump, Recovery 에서 사용함
	long m_nFirstCompoGID;
	long m_nSecondCompoGID;
	// Dump, Recovery 에서 사용함

	MSCompoG* mp_FirstCompoG;
	MSCompoG* mp_SecondCompoG;

	TRIM_OPTION m_FirstTrimOption;
	TRIM_OPTION m_SecondTrimOption;

	double m_dFirstLoc;
	double m_dSecondeLoc;
	double m_dFirstLocTol;
	double m_dSecondeLocTol;
	double m_dInnerDeg;
	double m_dIgnoreDeg;

	CON_POS m_eFirstConPos;
	CON_POS m_eSecondeConPos;

	void CalculateContainedAngle( MSLinkMemberG* pFirstLinkG, MSLinkMemberG* pSecondLinkG );
	bool CheckTrimOption(TRIM_OPTION Opt, CON_POS conPos, MSCompoG* trimCompoG);

	double m_dPointMemberArea;
};

