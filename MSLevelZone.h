#pragma once
#include "MSElement.h"

class MSAssembly;
class MSElement;
class MSLineLevelDifG;
class MSFloor;
class MSSlabG;

class StructureModel_CLASS MSLevelZone :	public MSElement
{
public:
	MSLevelZone(void);
	~MSLevelZone(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSLevelZone)
	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSLevelZone* pSource, bool bNewID=true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual void ModifyObject(bool isDoit = true);
	bool CheckData();

    //bool Contains(double Xc, double Yc) {return false;};
	void DeleteLevelZoneInfo();
	long MakeLevelZoneInfo(bool bMakeSlab = true);
	virtual void ReMakeLevelDif(bool bMakeSlab = true);

	void FindAndInsertLevelDif2LineMembers();

    virtual long Contains(double Xc, double Yc, double tolerance);
	virtual bool OverLap(GM2DCurve* pCurve, double tolerance);
	long LevelOverlap(MSLevelZone* pUrLevelZone);

	long MakeCrossThenAddLevelDif();

	double m_Level;
	double m_Width;
	double m_PreWidth;
	bool m_UseDefaultWidth;
	//GM2DPolyline m_Profile;
	//vector<MSLevelZone*> ma_InnerOpening;
    vector<MSLineLevelDifG*> ma_LineLevelDif;
	void AddLevelDifG(MSLineLevelDifG*);
//     void MakeLineLevelDifFromProfile();
// 	void MakeSlabLevelDifFromProfile();
	bool DeleteLineLevelDif(MSLineLevelDifG* pDeletedDifG);
    virtual void DeleteLineLevelDifAll();

	void DeleteLineLevelDifAllOnEdge();

	MSLevelZone* mp_Source;
	vector<MSLevelZone*> ma_Clone;

    MSAssembly* mp_Owner; 
    MSAssembly* GetOwner(){return mp_Owner;};
    void SetOwner(MSAssembly* val){mp_Owner = val;};
	virtual MSFloor* GetFloor();
	CString GetGroupName();
	CString GetUserFloorName();
	//vector<MSConnectivity*> ma_LevelDifJointArr; 
	void TrimLevelDifbySlabG();
	void TrimLevelDifbySlabGSetp2(map<long, MSSlabG*>& SlabMap);
	
// 	double GetLevel4LevelZone( double Xc, double Yc, double tolerance );
	virtual Type_MSCompoG GetType(){return msLevelZone;};
	virtual void GetBoundary( GM2DPolyline& APolyLine );

	void RecoverPointerAfterSerialize();

//    long m_ID;
    //long GetID();
//	static long ms_LastID;
    //long m_nFloorNum;

	virtual void Translate(GM2DVector &pDelta);
	virtual void MirrorByAxis(GM2DLineVector& pAxis);
	virtual void RotateZ(double theta, GM2DVector *pCenter);


};

#pragma make_public(MSLevelZone)


