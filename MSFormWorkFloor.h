#pragma once
#include "MSFloor.h"

class MSFormWorkDesignUnit;
class StructureModel_CLASS MSFormWorkFloor :public MSFloor
{
public:
	MSFormWorkFloor();
	~MSFormWorkFloor();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSFormWorkFloor)

	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSFormWorkFloor* pSource, bool bNewID = true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSFloor* GetUpperFloor();
	virtual MSFloor* GetLowerFloor();

	virtual double GetFloorLevel() override;
	virtual double GetSlabLevel() override;

	virtual vector<MSFloor*> GetFloorsByLevel(double dStartLevel, double dEndLevel);

	MSFormWorkDesignUnit* GetOwnerUnit() { return m_pOwnerUnit; };
	void SetOwnerUnit(MSFormWorkDesignUnit* pOwnerUnit);

	int GetFloorCount();
	void SetFloorCount(int floorCount);

private:
	MSFormWorkDesignUnit* m_pOwnerUnit;
	int m_nFloorCount;
};


