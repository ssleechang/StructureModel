#pragma once
#include "MSRcObject.h"
class StructureModel_CLASS MSCustomLineBarLocation :public MSRcObject
{
public:
	MSCustomLineBarLocation();
	~MSCustomLineBarLocation();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSCustomLineBarLocation)
	virtual void CopyFromMe(MSCustomLineBarLocation* pSource);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	long GetPointBarUnitID() { return m_PointBarUnitID; };
	long GetPointBarIndex() { return m_PointBarIndex; };

	void SetPointBarUnitID(long Id) { m_PointBarUnitID = Id; };
	void SetPointBarIndex(long index) { m_PointBarIndex = index; };
protected:
	long m_PointBarUnitID;
	long m_PointBarIndex;
};

