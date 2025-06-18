#pragma once
#include "MSObject.h"
#include "MSSteelGradeManager.h"
class MSReinforceBar;

class StructureModel_CLASS MSReinforceBarSetting : public MSObject
{
public:
	MSReinforceBarSetting(void);
	~MSReinforceBarSetting(void);
	void Serialize( CArchive& archive );
	void GlobalOptionSerialize( CArchive& archive );
	DECLARE_SERIAL(MSReinforceBarSetting)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);

	long m_ID;
	long GetID();  // m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
	static long ms_LastID;
	static void SetLastID(long NewID); // NewID > ms_LastID 이면 치환. 아니면 무시

	//Global ID 관리
	long m_GlobalID;
	long GetGlobalID();
	static long ms_GlobalLastID;
	static void SetGlobalLastID(long NewID);

	MSBaseBuilding* m_pBldg;
	void SetBuilding(MSBaseBuilding* pBldg){m_pBldg = pBldg;};
	void ReinforceBarDiaValidateCheck(bool IsGlobalOption);
	vector<MSSteelGradeManager::eSteelGrade> GetSteelGradeList();
	CString m_Name;
	map<long, MSReinforceBar*> mm_IndexToReinforceBar;
//	map<CString, MSReinforceBar*> mm_ReinforceBar;
	void FixIndexToReinforceBar();

	MSSteelGradeManager::eSteelGrade GetSteelGrade(int barIndex);
	CString GetMSReinforceBarName(int barIndex);

	bool IsPlainConc();
};

