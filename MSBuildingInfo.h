#pragma once
#include "MSObject.h"

class MSBuildingClassificationData;
class MSBuildingInfoManager;
class StructureModel_CLASS MSBuildingInfo : public MSObject
{
public:
	MSBuildingInfo(MSBuildingInfoManager* pBldgInfoManager = nullptr);
	~MSBuildingInfo(void);

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSBuildingInfo)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);

	long m_ID;
	long GetID();  // m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
	static long ms_LastID;
	static void SetLastID(long NewID); // NewID > ms_LastID 이면 치환. 아니면 무시

	CString GetName() { return m_Name; };
	void SetName(CString name) { m_Name = name; };
	
	double GetOverGrossFloor() { return m_dOverGrossFloor; }
	void SetOverGrossFloor(double overGrossFloor) { m_dOverGrossFloor = overGrossFloor; }
	double GetUnderGrossFloor() { return m_dUnderGrossFloor; }
	void SetUnderGrossFloor(double underGrossFloor) { m_dUnderGrossFloor = underGrossFloor; }

	void SetMSBuildingInfoManager(MSBuildingInfoManager* pMSBuildingInfoManager);

	CString GetBuildingInfoClassification();
	CString GetBuildingInfoType();	
	void SetBuildingInfoType(CString buildningType);
	
	long GetMSBuildingClassificationID();
	MSBuildingClassificationData* GetMSBuildingClassification(); 
	void SetBuildingClassification(MSBuildingClassificationData* pBuildingClassification);
	
	CString GetUsage() {return m_strUsage;};
private:
	
	MSBuildingInfoManager* m_pBldgInfoManager;
	long m_nBldgClassificationID;// 빌딩유형ID
	CString m_Name; // 동명 ==> 건물명
	
	double m_dOverGrossFloor;
	double m_dUnderGrossFloor;

	CString m_strUsage; // 용도 이전 데이타 V22.0.10 이후 부터는 사용하지 않는다.
};

