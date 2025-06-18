#pragma once

#include "MSElement.h"
#include "MSSlabRebarInfo.h"
class MSMaterial;

class StructureModel_CLASS MSSlabRebar : public MSElement
{
public:
	MSSlabRebar(bool bReverse = false);
	~MSSlabRebar();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSSlabRebar)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);
	virtual void Init(bool bInConstructor = false);
	virtual Type_MSCompoG GetType() { return msSlabRebar; };

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	enum eSlabRebarType
	{
		Slab = 0, MaterialLift, EVFailPrevention
	};

	MSFloor* m_pFloor;
	virtual MSFloor* GetFloor() { return m_pFloor; };
	void SetFloor(MSFloor* pFloor) { m_pFloor = pFloor; };

	double GetLocValueByType(MSSlabRebarInfo::RebarType type);
	virtual bool isXDirection();

	bool GetReverse()								{ return m_bReverse; };
	void SetReverse(bool isReverse)		{ m_bReverse = isReverse; };
	bool GetPolylineType() { return m_isPolylineType; };
	void SetPolylineType(bool isPolyline) { m_isPolylineType = isPolyline; };
	bool GetStartHook() { return m_IsStartHook; };
	void SetStartHook(bool isHook) { m_IsStartHook = isHook; };
	bool GetEndHook() { return m_IsEndHook; };
	void SetEndHook(bool isHoook) { m_IsEndHook = isHoook; };
	bool GetIsUsedStoryMaterial() { return m_IsUsedStoryMaterial; };
	void SetIsUsedStoryMaterial(bool bUsedStoryMaterial) { m_IsUsedStoryMaterial = bUsedStoryMaterial; };

	MSMaterial* GetMaterial();
	long GetMaterialID();
	void SetMaterialID(long materialID) { m_nMaterialID = materialID; };
	bool isPlainConcrete();

	double GetOffset();
	void SetOffset(double offset);
	double GetDegree() { return m_dLxDeg; };
	void SetDegree(double degree) { m_dLxDeg = degree; };

	double GetStartCover() { return m_dStartCover; };
	void SetStartCover(double startCover) { m_dStartCover = startCover; };
	double GetEndCover() { return m_dEndCover; };
	void SetEndCover(double endCover) { m_dEndCover = endCover; };
	long GetTopCoverType() { return m_nTopCoverType; }
	void SetTopCoverType(long topCoverType) { m_nTopCoverType = topCoverType; }
	double GetTopCover() { return m_dTopCover; };
	void SetTopCover(double topCover) { m_dTopCover = topCover; };
	long GetBotCoverType() { return m_nBotCoverType; }
	void SetBotCoverType(long botCoverType) { m_nBotCoverType = botCoverType; }
	double GetBotCover() { return m_dBotCover; };
	void SetBotCover(double botCover) { m_dBotCover = botCover; };

	MSSlabRebarInfo* GetRebarInfo();
	void SetRebarInfo(MSSlabRebarInfo* pRebarInfo);
	void SetRebarInfo(MSSlabRebarInfo::RebarType type, CString rebarText);

	eSlabRebarType GetSlabRebarType() { return m_eSlabRebarType; };
	void SetSlabRebarType(int slabRebarType) { m_eSlabRebarType = (eSlabRebarType)slabRebarType; };

protected:
	bool m_bReverse;
	bool m_isPolylineType;
	bool m_IsStartHook;
	bool m_IsEndHook;
	bool m_IsUsedStoryMaterial;

	long m_nMaterialID;

	double m_dLxDeg;
	double m_dOffset;

	double m_dStartCover;
	double m_dEndCover;
	long m_nTopCoverType;
	double m_dTopCover;
	long m_nBotCoverType;
	double m_dBotCover;

	MSSlabRebarInfo* mp_RebarInfo;
	eSlabRebarType m_eSlabRebarType;
public:
	// 실제 반영된 Data 2014.07.11 - by Gracias
	vector<GM2DLineVector*> ma_BentCutLines;
	GM2DLineVector m_RebarZone;
	GM2DLineVector m_RebarLine;
	vector<double> ma_BentLocValue;

	// new
	GM2DPolyline m_BentLine;
	GM2DVector m_Direction;
	GM2DVector m_Normal;
	GM2DVector m_RebarOrigin;

public:

	void DeleteBentCutLines();

	bool AddBentPoint(GM2DVector& BentPoint);
	bool AddBentLocValue(double LocValue);

	vector<double> GetBentDistancesFromStart();
	vector<GM2DVector> GetBentPoints();

	GM2DPolyline* GetProfile();

	bool CheckOverlap(MSSlabRebar* pUrSlabRebar);
	bool SetZoneProfile(GM2DPolyline* pZoneProfile = NULL);
	void SetBoxProfile(GM2DLineVector vertical, GM2DLineVector horizon);

	virtual void Translate(GM2DVector &pDelta);
	virtual void MirrorByAxis(GM2DLineVector& pAxis);
	virtual void RotateZ(double theta, GM2DVector *pCenter);
	GM2DLineVector* GetBentCutLine(double LocValue);
	// new
	void CalcAutoDegree();
	void RotateZbyChangeDirection(double degree);
	void CorrectedProfile();

	bool AddBentLocations(vector<double> Locs);
	void FixBentLocByLastPntToCenPnt();
	void CalculateBentLine();
	bool IsValidBentLine();
	void SetRebarDirection(double degree);
	void SetRebarOrigin(GM2DVector vec);
	GM2DVector* GetFirstIntersectedPoint(GM2DLineVector Line, GM2DPolyline Poly);
	GM2DVector* GetFirstIntersectedPoint(GM2DVector Origin, GM2DVector Direction, GM2DPolyline Poly);

	bool CheckRebarLine(bool bMakeErrorObject);
	void CaluateRebarOrigin();
	bool CheckBentLocation();

	void CalulateRebarLineRebarZone();
	void MakeRebarLine(GM2DLineVector& line1, GM2DLineVector& line2, double LocValue);
	GM2DVector GetMidPoint(GM2DVector* p1, GM2DVector* p2);

	shared_ptr<GM2DCurve> GetStartLine();
	shared_ptr<GM2DCurve> GetEndLine();

	GM2DLineVector GetRebarCenterLine();

	bool IsValidProfile();

	void SetIsClipBoardSerialize(bool isClipboard) { IsClipBoardSerialize = isClipboard; }

	virtual void FixProfile() override;
private: 
	bool IsClipBoardSerialize;
};

#pragma make_public(MSSlabRebar)