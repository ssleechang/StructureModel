#pragma once
#include "MSRcObject.h"
class MSCustomSection;
class StructureModel_CLASS MSCustomLocation : public MSRcObject
{
public:
	MSCustomLocation();
	~MSCustomLocation();

	MSCustomLocation(const MSCustomLocation& aData) { *this = aData; }
	MSCustomLocation& operator = (const MSCustomLocation& aData)
	{
		Initialize();
		m_RebarLineID1 = aData.m_RebarLineID1;
		m_RebarLineID2 = aData.m_RebarLineID2;
		return *this;
	}

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSCustomLocation)
	virtual void CopyFromMe(MSCustomLocation* pSource);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	void Initialize();

	void SetRebarLineID1(long rebarLineID) { m_RebarLineID1 = rebarLineID; };
	void SetRebarLineID2(long rebarLineID) { m_RebarLineID2 = rebarLineID; };

	long GetRebarLineID1() { return m_RebarLineID1; };
	long GetRebarLineID2() { return m_RebarLineID2; };

	bool HasRebarLineID(long rebarLineID);

	CString GetName(MSCustomSection* pSection);
protected:
	long m_RebarLineID1, m_RebarLineID2;
};

