#pragma once
#include "mslinememberg.h"

class StructureModel_CLASS MSHandRailG :
	public MSLineMemberG
{
public:
	MSHandRailG();
	~MSHandRailG(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSHandRailG)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual void Init(bool bInConstructor=false);
	virtual Type_MSCompoG GetType(){return msHandRailG;};
	virtual bool GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal, bool bCheckLevelZone = true);
	virtual bool GetZLevelsbyLoc(double& TopLevel, double& BottomLevel, double dLoc = 0.5);
	
	virtual void SetLines();
	virtual bool MakeBoundary();
	virtual void InitTrimmedBoundary(bool IsTopLevel = true);
	virtual void GetBoundaryNSubMember(GM2DPolyline& APolyLine);
	virtual void FindMiterLineWithOtherLinkG(MSLinkMemberG* link, GM2DLineVector& rLine);
	
	virtual void RemoveInwardWedge() override;

	virtual bool MakeExtendedBoundary(GM2DPolyline& pExtendedBoundary, long nExtendType = 0, double dExtendLen = 0);
	virtual bool MakeExtendedBoundary_ToLine(GM2DPolyline& pExtendedBoundary, GM2DLineVector& line);
	
	void GetBoundaryWidthParaPet(GM2DPolyline& Poly);
	virtual void UpdateTrimmedBoundary_AfterCalculate(CON_POS ePos = CON_ALL);

	void SetDefParapetPoly();
	void DeleteParaPetInfo();
	bool HasParaPet();
	double GetParaPetWidth();

	bool m_IsLeftParapet;

	GM2DPolyline* m_pParapetPoly;
	GM2DCurve* m_pParapetLeftCurve;
	GM2DCurve* m_pParapetRightCurve;
	GM2DCurve* m_pParapetLeftCurveTR;
	GM2DCurve* m_pParapetRightCurveTR;

	double GetTopHeight();
	double GetBottomHeight();
	double GetTopDepth();
	double GetBottomDepth();

	double GetTopOffset();
	void SetTopOffset(double dTopOffSet);
	double GetBottomOffset();
	void SetBottomOffset(double dBottomOffSet);

	int GetTopQuantityType();
	void SetTopQuantityType(int nType);
	int GetBottomQuantityType();
	void SetBottomQuantityType(int nType);
	bool GetTopQuantitySeparation();

	enum LowWallCalcType
	{
		All = 0,
		Top = 1,
		Bottom = 2
	};

private:
	void Serialize_EachCompoM_Rebar(CArchive& ar);
	void MakeBoundaryWidthParaPet(GM2DPolyline* pPoly);

	double m_dTopOffset;
	double m_dBottomOffset;
	int m_nTopQuantityType; // Hor = 0 Vertical = 1
	int m_nBottomQuantityType; // Hor = 0 Vertical = 1

public:
	virtual MSCompoM::Type_MSCompoM GetDefCompoMType() { return MSCompoM::Type_MSCompoM::msHandRailM; };
};
