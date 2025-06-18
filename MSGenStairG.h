#pragma once
#include "mspartcompog.h"

#include "GMLib/GMVector.h"
#include "GMLib/GM2DRectangle.h"

class GM2DPolyline;

enum StairStepShapeType
{
	BottomSlabLevel = 0, // 챌판으로 끝나지 않음, 슬래브 하부에 붙음
	TopSlabLevel = 1,    // 챌판으로 끝남, 슬래브 상부에 붙음
	BottomSlabLevelNStepHeight = 2 // 슬래브 하부에 붙고 높이 고려
};

class StructureModel_CLASS MSStairPart : public MSPartCompoG
{
public:

	enum eExtendType
	{
		ExtendType_Start = 0,
		ExtendType_End,
		ExtendType_Both
	};

	MSStairPart();
	~MSStairPart(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSStairPart)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual Type_MSCompoG GetType(){return msStairPart;};
	virtual bool MakeBoundary();
	virtual bool GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal = true, bool bCheckLevelZone = true  );
	double GetTopSurfaceZ(double x, double y); // m_SOffset 기준에서의 높이

	virtual MSFloor* GetFloor() override;
	virtual void Translate(GM2DVector &pDelta);
	virtual void MirrorByAxis(GM2DLineVector& pAxis);
	virtual void RotateZ(double theta, GM2DVector *pCenter);
	virtual void SetOwnerCompoG(MSElement* pOwner) { mp_OwnerCompoG = (MSCompoG*)pOwner; };
	virtual MSElement* GetOwnerCompoG() { return mp_OwnerCompoG; };
	virtual CString GetKey(bool bUseID = true);

	virtual double GetWidth();
	virtual double GetDepth();
	virtual double GetThick();

	GM2DPolyline* m_pPolyline;//Step의 경우는 사각형이라 가정
	long m_Thick;
    
	MSCompoG* mp_OwnerCompoG;

	int GetSequence();
	void SetSequence(int nSeq);

	GM2DCurve* GetPathCurve();
	void SetPathCurve(GM2DCurve* PathCurve);

	void SetWidth(double dWidth);
	bool IsLanding();

	long GetStepsNum();
	void SetStepsNum(long nStepsNum);

	long GetBottomLevel();
	void SetBottomLevel(long nBottomLevel);
	long GetTopLevel();
	void SetTopLevel(long nTopLevel);

	double GetOneStepHeight();
	void SetOneStepHeight(double nStepHeight);
	void CalcOneStep();
	
	void CalcOneStepHeight();

	double GetOneStepWidth();
	void CalcOneStepWidth();
	
	double GetCalcStepHeight();
	double GetCalcStepWidth();
	

	void CalcPathCurve();
	
	void CalcPathCurve_RectStair();
	void CalcPathCurve_LineStair();
	void CalcPathCurve_UserStair();

	GM2DPolyline* GetExtendLineProfile(eExtendType eType, double dExtendLen);
	bool IsConnect(MSStairPart* pUrStairPart);
	double GetSlopeAngle();
	//프로파일의 짧은 선의 각도
	double GetStairPartDegree();


	bool GetIsEndRiser();

	bool GetIsCalculateBottomStep();
	void SetIsCalculateBottomStep(bool value);

	StairStepShapeType GetStairPartStepType();
	void SetStairPartStepType(StairStepShapeType stepType);

	int GetUnitPrecision();
	void SetUnitPrecision(int value);

	MSStairPart* GetNextStairPart();
	MSStairPart* GetPreStairPart();

private:
	virtual void Serialize_EachCompoM_Rebar(CArchive& ar);
	double GetStepUnit();

	long m_nStepsNum;// Ending 부분의 경우 Step평면이 없다.

	int m_nSequence; // 시퀀스 번호 같은 번호대이면 같은 레벨에 있는 상태 임, ex) 같은레벨의 렌딩을 여러게 나누어 그렸을 경우
	GM2DCurve* mp_Curve;
	double m_dWidth;
	double m_dStepHeight;
	long m_nBottomLevel;// Landing level, StepRun Start Level, 윗면 기준
	long m_nTopLevel;// StepRun End Level, 첫 계단은 Start Level에서 바로 한 단이 올라감, 마지막 계단은 Landing에 의해서 생성됨

	double m_dStepWidth;

	/// 챌판으로 끝남
	//bool m_IsEndRiser;
	
	StairStepShapeType m_StairStepShapeType;

	/// 계단 정렬 방식
	/// true : 최하단 계단을 조절함 , false : 최상단 계단을 조절함
	bool m_IsCalculateBottomStep;

	// 자리수
	int m_nUnitPrecision;
};

class MSWallMemberG;
class StructureModel_CLASS MSGenStairG :
	public MSPartCompoG
{
public:
	enum StairType
	{
		StairType_General = 0,
		StairType_Double = 1,
		StairType_Line = 2,
		StairType_User = 3
	};

	MSGenStairG();
	~MSGenStairG(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSGenStairG)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual void Init(bool bInConstructor=false);
	virtual Type_MSCompoG GetType(){return msStairG;};
	virtual bool MakeBoundary();

	virtual MSMaterial* GetMaterial();

	virtual void Translate(GM2DVector &pDelta);
	virtual void MirrorByAxis(GM2DLineVector& pAxis);
	virtual void RotateZ(double theta, GM2DVector *pCenter);

	virtual inline void CalcBoundRectBeforeAddQuadTree();
	virtual bool VisibleInRect( GM2DRectangle* pRect);
	virtual bool GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal = true, bool bCheckLevelZone = true );
	virtual bool GetZLevelsByPart(double& TopLevel, double& BottomLevel);
	virtual double GetDepth();
	virtual double GetDepthWithOutLowPart();
	virtual GM2DPolyline* GetProfile();
	virtual void MakeCCW();

	virtual void SetOwner(MSContainer* val);
	GM2DRectangle m_RectForQuadTree;

	double m_dBotOffset; // ZOffset
	//double m_dTopOffset; // not used
	long m_nSupportType;
	BOOL m_isComponentStair;//m_b2Layer2단계단
	
	MSStairPart* GetLowerPart();
	MSStairPart* GetUpperPart();

	void SetStairType(StairType eType);
	StairType GetStairType();
	void CalculateOffSetByUserStair();

	int GetEndComponentType();
	int GetStartComponentType();

	double GetStepThick();
	void SetStepThick(double nThick);
	int GetStepNum();
	void SetStepNum(int nStepNum);
	double GetStepHeight();
	void SetStepHeight(double dStepHeight);

	double GetLandingThick();
	void SetLandingThick(double nThick);

	void SetPartSequenceNum();
	MSStairPart* GetStairPartbySequence(int nFindSequence);

	MSStairPart* GetUpFloorStairLowLanding();
	MSStairPart* GetUpFloorStairLowStep();
	MSStairPart* GetLowFloorStairUpLanding();
	MSStairPart* GetLowFloorStairUpStep();
	double GetUpFloorStairLowLandingThick();

	virtual CString GetClassificationName() override;
	CString GetStepClassificationName();
	CString GetLandingClassificationName();
	double GetDownFloorStairUpLandingThick();

	MSCompoG* GetTopConnectSlab();
	MSCompoG* GetBottomConnectSlab();

private:
	virtual void Serialize_EachCompoM_Rebar(CArchive& ar);
	bool CalculatePartHeight_General();
	bool CalculatePartHeight_Double();
	bool CalculatePartHeight_Line();
	bool CalculatePartHeight_User();

	void CalculateRectStairInVecs();
	MSStairPart* GetEndStairPart(CTypedPtrArray<CObArray, MSStairPart*>& PartArr, bool IsLowerPart);

	void CheckPartProfile();

	void UpdateOldStairInfo();
	

	double m_dTopOffset; // TopZOffset
	StairType m_eStairType;
	vector<double> ma_LandingHeight;

	bool m_bRemoveStartLanding;
	bool m_bRemoveEndLanding;

	double m_dSetpHeight;
	int m_nStepNum;
public:

	GMPolygon m_InnerPoly;

	MSWallMemberG* m_pLastWall;//두번째 계단 옆벽
	MSWallMemberG* m_pFirstWall;//첫번째 계단 옆 벽
	MSWallMemberG* m_pStartWall;
	MSWallMemberG* m_pEndWall;
		
    long m_nStairFootNum;

	void SetInnerPGonCCW();

	MSWallMemberG* FindWallMemberG(GM2DLineVector InnerLine);
	double GetHeight();//Offset반영된 높이 리턴
	double GetOffsetLowZ();
	
	virtual GM2DVector* GetVec(long Index);

	// 새로운 계단 데이타
	bool IsUsedMID(long MID);
	CTypedPtrArray<CObArray, MSStairPart*> ma_Step;
	CTypedPtrArray<CObArray, MSStairPart*> ma_Landing;

	vector<MSStairPart*> GetStairPart();
	MSStairPart* GetFirstStep();
	MSStairPart* GetLastStep();

	MSStairPart* GetFirstLanding();
	MSStairPart* GetLastLanding();

	void DeleteStairArr();
	void DeleteLandingArr();

    void AddStep(MSStairPart* steps);
    void AddLanding(MSStairPart* steps);

	long GetStairLandingJointCnt();
	void ReSetVec();

	bool isComponentStair();

	MSStairPart* FindStairPartByPoly(GM2DPolyline* pPoly);

	bool CalculatePartHeight();
	bool CalculatePartHeight_Formwork();

	bool CalculatePartHeight_General_Formwork();
	bool CalculatePartHeight_Double_Formwork();
	bool CalculatePartHeight_Line_Formwork();

	void SetStepsInfo();
	bool GetRemoveStartLanding();
	void SetRemoveStartLanding(bool value);

	bool GetRemoveEndLanding();
	void SetRemoveEndLanding(bool value);

	double GetBotOffset();
	void SetBotOffset(double value);

	double GetTopOffset();
	void SetTopOffset(double value);

	vector<double> GetLandHeightList();
	void SetLandHeightList(vector<double> value);

	bool IsSameGeometry(MSCompoG* urCompoG, double distTolerance, double degreeTolerance) override;

	void CalculatePartPathCurve();
	vector<GM2DPolyline*> GetPathPolys();

	virtual void FixCompoGInfoAfterSerialize() override;
	double GetFirstPathDegree();

	virtual void FixProfile() override;
	vector<GM2DCurve*> GetPanelLineArr();

	GM2DCurve* GetStairOutLine(MSStairPart* pStairPart, bool bFindLongCurve);
};
