#pragma once
#include "msobject.h"
#include "MSSteelGradeManager.h"
#include "MSSteelGradeManager.h"
class StructureModel_CLASS MSReinforceBar : public MSObject
{
public:
	MSReinforceBar(void);
	~MSReinforceBar(void);
	void Serialize( CArchive& archive );
	void GlobalOptionSerialize( CArchive& archive );
	DECLARE_SERIAL(MSReinforceBar)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);

	long m_ID;
	long GetID();  // m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
	static long ms_LastID;
	static void SetLastID(long NewID); // NewID > ms_LastID 이면 치환. 아니면 무시

	// GlobalOption ID 관리
	long m_GlobalID;
	long GetGlobalID();
	static long ms_GlobalLastID;
	static void SetGlobalLastID(long NewID);


	CString GetPrintName(bool bChinaUsed = false);
	void SetPrintName(CString printName);

	static MSReinforceBar* CreateSD600SReinforceBar(bool IsGloblaOption);

	MSSteelGradeManager::eSteelGrade GetSGrade();

	CString m_Name;//SD40...
	double m_dBarStrength;//fy값 400,500...
//	CString m_PrintName;//D,HD,SHD
	MSSteelGradeManager* m_pSGradeManager;
	MSSteelGradeManager::eSteelGrade m_eSGrade;

};

