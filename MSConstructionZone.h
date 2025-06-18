#pragma once
#include "MSElement.h"
#define YEAR_START 1
class MSFloor;
class StructureModel_CLASS MSConstructionZone : public MSElement
{
public:
	MSConstructionZone(void);
	~MSConstructionZone(void);

	enum eCZoneType { Horizontal = 0, Vertical = 1};
	enum eCZoneState { Plan = 0, UnderConstruction, Finished};

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSConstructionZone)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual Type_MSCompoG GetType(){return msConstructionZone;};

	virtual GM2DPolyline* GetProfile();
	virtual MSFloor* GetFloor(){return mp_Owner;};
	virtual MSBaseBuilding* GetBuilding();

	static CTime InitialDate(){ return  CTime(2000,1,1,0,0,0);}
    CString GetName(){return m_Name;};
	void SetName(CString name);
	CTime GetActualStartDate();
	CTime GetActualFinishDate();
	void SetActualStartDate(const CTime&  startDate);
	bool SetActualFinishDate(const CTime&  finishDate);

	CTime GetPlanStartDate();
	CTime GetPlanFinishDate();
	void SetPlanStartDate(const CTime&  startDate);
	bool SetPlanFinishDate(const CTime&  finishDate);

	bool Contains(double dXc, double dYc);
	bool OverLap(MSPolylineObject* pUrPolylineObject, double dTol = DTOL_GM);
	bool OverLap(GM2DPolyline* pUrPoly, double dTol = DTOL_GM);
	bool CheckData();
	bool CheckData(int oldID);
	virtual long GetPriorityNum() { return m_nNumber; };

	MSFloor* mp_Owner;	
	bool m_bDefaultCZone;

	virtual void Translate(GM2DVector &pDelta);
	virtual void MirrorByAxis(GM2DLineVector& pAxis);
	virtual void RotateZ(double theta, GM2DVector *pCenter);

	void UpdateOpeningProfile(GM2DPolyline* pProfile, long cZoneID);

	eCZoneType GetZoneType();
	void SetZoneType(eCZoneType zoneType);
	eCZoneState GetZoneState();
	void SetZoneState(eCZoneState zoneState);

	virtual void FixProfile() override;

	void RemoveDumpOpening();

	long GetNumber();
	void SetNumber(long number);
private:
    CString m_Name;
	CTime m_PlanStartDate;
	CTime m_PlanFinishDate;
	CTime m_ActualStartDate;
	CTime m_ActualFinishDate;
	eCZoneType m_eZoneType;
	eCZoneState m_eZoneState;
	long m_nNumber;
};
