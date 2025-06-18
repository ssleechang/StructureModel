#pragma once
#include "MSObject.h"

class StructureModel_CLASS MSBuildingClassificationData : public MSObject
{
public:
	MSBuildingClassificationData();
	MSBuildingClassificationData(CString BldgType, CString BldgClassification);
	~MSBuildingClassificationData(void);

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSBuildingClassificationData)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);

	long m_ID;
	long GetID();  // m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
	static long ms_LastID;
	static void SetLastID(long NewID); // NewID > ms_LastID 이면 치환. 아니면 무시

	CString GetBuildingType() { return m_BldgType; };
	void SetBuildingType(CString value) { m_BldgType = value; };

	CString GetBuildingClassification() { return m_BldgClassification; };
	void SetBuildingClassification(CString value) { m_BldgClassification = value; };
private:

	CString m_BldgType; // 건물 유형으로 변경 (아파트, 지하주차장, 부속동, 관리사무소)
	CString m_BldgClassification; // 건물 분류 (동, 지하주차장, 부속동)
};

