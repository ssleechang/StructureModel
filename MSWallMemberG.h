#pragma once
#include "MSLinkMemberG.h"

#include "MSOpening.h"
#include "MSWallOpeningG.h"
#include "MSFWShadeG.h"
#include "GMLib/GMSurface.h"
#include "StructureModelEnum.h"
#include "MSMainBar.h"

class MSWallEndRebarLine;
class MSWallEndRebarM;

typedef struct _WallLevelDifStruct
{
public:
	_WallLevelDifStruct()
	{
		Initialize();
	};
	_WallLevelDifStruct(const _WallLevelDifStruct& aData)	{*this = aData;};

	_WallLevelDifStruct& operator = (const _WallLevelDifStruct& aData)
	{
		Initialize();
		pLevelDif = aData.pLevelDif;
		isTop = aData.isTop;
		return *this;
	};
	void Initialize()
	{
		isTop = 0;
		pLevelDif = NULL;
	};

public:
	// 0 둘다 1 top 2: bot
	long isTop;
	MSMemberLevelDif* pLevelDif;
}WallLevelDifStruct;

typedef struct _LevelDifWall
{
public:
	_LevelDifWall()
	{
		m_dBotLevel = 0;
		m_dTopLevel = 0;
		m_dSLoc = 0;
		m_dELoc = 1;
	}
	~_LevelDifWall()
	{
	}
	double GetSLoc(){return m_dSLoc;}
	double GetELoc(){return m_dELoc;}
	double GetBotLevel(){return m_dBotLevel;}
	double GetTopLevel(){return m_dTopLevel;}
	void SetSLoc(double dVal){m_dSLoc = dVal;}
	void SetELoc(double dVal){m_dELoc = dVal;}
	void SetBotLevel(double dVal){m_dBotLevel = dVal;}
	void SetTopLevel(double dVal){m_dTopLevel = dVal;}

	_LevelDifWall & operator = (const _LevelDifWall & TheCopy)
	{
		m_dBotLevel = TheCopy.m_dBotLevel;
		m_dTopLevel = TheCopy.m_dTopLevel;
		m_dSLoc = TheCopy.m_dSLoc;
		m_dELoc = TheCopy.m_dELoc;
		return *this;
	}
	//GM2DCurve* mp_Curve;
private:
	double m_dBotLevel;
	double m_dTopLevel;
	double m_dSLoc;
	double m_dELoc;

}LevelDifWall;


class StructureModel_CLASS MSWallMemberG :	public MSLinkMemberG
{
public:
	MSWallMemberG();
	~MSWallMemberG(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSWallMemberG)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual void Init(bool bInConstructor=false);
	virtual Type_MSCompoG GetType(){return msWallG;};
	virtual void InitializeParameter();

	virtual CString ToString();
	virtual bool CheckData();	
	bool CheckShadeOverlap();
	bool CheckShadeOverlap(MSFWShadeG *pShade);
	virtual bool CheckHeight(bool IsUseGroupName = false);
	virtual bool IsSlopeMember();
	virtual bool HasLevelDif();
	virtual void SetOwner(MSContainer* val);
	virtual bool MakeSubMemberBoundary();
	GMSurface* mp_Surface;

	vector<MSOpening*> ma_Opening;
	CTypedPtrArray<CObArray, MSMemberLevelDif*> ma_MemberLevelDifLow;

	void ReverseLevelDifLow();

	virtual bool IsBearingWall();
	virtual bool IsRetainingWall();
	virtual bool GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal = true, bool bCheckLevelZone = true  );
	/*virtual bool GetZLevelsbyPos(double& TopLevel, double& BottomLevel, char cPos);*/
	
	virtual double GetOffSetbyLoc(double dLoc);
	virtual double GetStartOffSetbyLoc(double dLoc);
	virtual bool GetZLevelsbyLoc(double& TopLevel, double& BottomLevel, double dLoc = 0.5);

	virtual void SetStartOffSet(double dOffSet);
	virtual void SetEndOffSet(double dOffSet);
	virtual double GetStartOffSet();
	virtual double GetEndOffSet();

	virtual double GetHeight();
	bool IsRWall();
	GM2DCurve* GetLongLVecByTrimOpt();
	virtual bool HasSubMember() override;
	virtual void DeleteSubMembers() override;

private:
	void Serialize_EachCompoM_Rebar(CArchive& ar);
	//double m_dThickSetback;  // CompoM과 단면 두께가 다른 벽체에 사용
public:
	double m_SOffsetZ_End; // bot end (m_SOffsetZ 과 동일, 이형벽체인경우만 다름)
	double m_EOffsetZ_End; // top end (m_EOffsetZ 과 동일, 이형벽체인경우만 다름)

public:
	virtual double GetThick();
	virtual void SetThickOfCompoG(double dThick);
	virtual double GetWidth();

	virtual double GetEndLevel(bool isMax = true);
	virtual double GetStartLevel(bool isMax = false);

	virtual double GetRelativeEndLevel(bool isMax = true);
	virtual double GetRelativeStartLevel(bool isMax = false);

	virtual double GetRelativeEndLevelbyLoc(double dLocVal);
	virtual double GetRelativeStartLevelbyLoc(double dLocVal);

    map<long, MSWallOpeningG*> mm_WallOpening;
	bool Add(MSWallOpeningG* pOpening);
    bool Remove(MSWallOpeningG* pOpening);
    bool Delete(MSWallOpeningG* pOpening);
    void DeleteOpenings();
    bool HasOpening();
    MSWallOpeningG* GetOpening(long ID);

	static void ModifyOpeningWhenModifyWall(MSWallMemberG* org, MSWallMemberG* tgt);
	
	void RemoveLevelDifLow( MSLineLevelDifG* pLineLevelDifg );
	void DeleteLowMemberLevelDifG();

	long GetLevelDifWalls(vector<LevelDifWall>& LevelDifWalls);
	bool GetNextMemberLevelDif(double dLoc,double& dNextLoc, double& dBotLevel, double& dTopLevel);
	
	bool GetLeftInside();
	void SetLeftInside(bool isLeftInside);

	vector<WallLevelDifStruct> GetWallLevelDifSt();
	vector<double> GetWallLevelDifLevel(bool isTop);
	virtual vector<GM2DCurve*> GetLevelDifCurves();

	virtual bool IsContainSlabinLowFloor();
	virtual CString GetKey(bool bUseID = true);

	//해당 조인트에서 연결된 부재(기둥,벽)있는지 체크
	bool CheckConnectionMember(CON_POS eConPos);
	//vector<int> GetCenCurveByCZoneIDs();
	//vector<double> GetLevelDifLevelByCZone(int idx);

	void GetUrLinkMemberEcc(MSLinkMemberG* pUrLink, double& dLEcc,double& dREcc);

	MSWallEndRebarLine* GetWallEndRebarLine(int rebarLineID);
	MSWallEndRebarM* GetWallEndRebarM(int rebarLineID, int story);

	bool IsUsedEndRebarLine();
	void ResetEndRebarLine();
	int	  GetStartEndRebarLineID();
	int	  GetEndEndRebarLineID();

	bool GetIsStartEndAddedBar();
	bool GetIsEndEndAddedBar()	;
	MSMainBar GetStartEndAddedBar();
	MSMainBar GetEndEndAddedBar();
	int GetStartEndAddedBarSpacing();
	int GetEndEndAddedBarSpacing();

	void SetStartEndRebarLineID(int ID);
	void SetEndEndRebarLineID(int ID);

	void SetIsStartEndAddedBar(bool IsAddedBar);
	void SetIsEndEndAddedBar(bool IsAddedBar);
	void SetStartEndAddedBar(CString sMainBar);
	void SetEndEndAddedBar(CString sMainBar);
	void SetStartEndAddedBarSpacing(int spacing);
	void SetEndEndAddedBarSpacing(int spacing);

	bool IsSameBWallHorBar(MSWallMemberG* pUrWall);
	virtual bool IsSameInfo(MSLinkMemberG* pUrLink);
	
	vector<MSCompoG*> GetSupportSlab();
	void GetSupportCurve(vector<GM2DCurve*>& LCurveArr, vector<GM2DCurve*>& RCurveArr);

	virtual bool UsedMaterial(long materialID) override;
	void SubMemberMaterialValidCheck();

	virtual void InitTrimmedBoundary(bool IsTopLevel = true);

	bool IsSpecialBoundaryM();
	shared_ptr<GM2DCurve> GetLongCenCurvebySpecialRebar();

	map<long, MSFWShadeG*> mm_ShadeG;
	bool Add(MSFWShadeG* shadeG);
	bool Remove(MSFWShadeG* shadeG);
	bool Delete(MSFWShadeG* shadeG);
	void DeleteShadeGs();
	bool HasShadeG();
	MSFWShadeG* GetShadeG(long ID);

	virtual bool RemoveSubMember(MSElement* element) override;

	void DeleteOnlyWallOpenings();
	void DeleteSwitchBoxs();
	void DeleteHorShadeGs();
	void DeleteVerShadeGs();

	virtual CString GetGroupName();
protected:
	bool m_IsLeftInside;
	
	bool m_IsStartEndAddedBar, m_IsEndEndAddedBar;
	int m_StartEndRebarLineID, m_EndEndRebarLineID;
	

public:
	virtual MSCompoM::Type_MSCompoM GetDefCompoMType() { return MSCompoM::Type_MSCompoM::msBWallM; };
};
