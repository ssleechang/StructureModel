#pragma once
#include "MSElement.h"
#include "MSFormWorkFloor.h"
class MSFormWorkBuilding;

class StructureModel_CLASS MSFormWorkDesignUnit :public MSElement
{
public:
	MSFormWorkDesignUnit(double dHeight = 2800);
	~MSFormWorkDesignUnit();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSFormWorkDesignUnit)

	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSFormWorkDesignUnit* pSource, bool bNewID = true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	void SetBuilding(MSFormWorkBuilding* pBuilding);

	MSFormWorkFloor* GetFloorByID(long nID);

	MSFormWorkFloor* GetTopFloor() { return m_pTopFloor; };
	MSFormWorkFloor* GetWorkingFloor() { return m_pWorkingFloor; };
	MSFormWorkFloor* GetBottomFloor() { return m_pBottomFloor; };

	CString GetName() { return m_Name; };
	double GetTopFloorHeight() { return m_pTopFloor->GetHeight(); };
	double GetWorkingFloorHeight() { return m_pWorkingFloor->GetHeight(); };
	double GetBottomFloorHeight() { return m_pBottomFloor->GetHeight(); };

	void SetName(CString name) { m_Name = name; };
	void SetTopFloorHeight(double height) { m_pTopFloor->SetHeight(height); };
	void SetWorkingFloorHeight(double height) { m_pWorkingFloor->SetHeight(height); };
	void SetBottomFloorHeight(double height) { m_pBottomFloor->SetHeight(height); };

    void SetFloorIDs();

	double GetFloorLevel(MSFormWorkFloor* pFloor);
	vector<MSFloor*> GetFloorsByLevel(double dStartLevel, double dEndLevel);
private:
	void DeleteFloors();

private:
	MSFormWorkBuilding* m_pBuilding;
	CString m_Name;
	MSFormWorkFloor* m_pTopFloor;
	MSFormWorkFloor* m_pWorkingFloor;
	MSFormWorkFloor* m_pBottomFloor;

};

