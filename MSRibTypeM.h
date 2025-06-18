#pragma once
#include "msobject.h"
class StructureModel_CLASS MSRibTypeM :
	public MSObject
{
public:
	MSRibTypeM();
	~MSRibTypeM();
public:
	CString GetName(){ return m_Name;};
	void SetName(CString Name){m_Name = Name;};
	long m_ID;
	long GetID();  // m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
	static long ms_LastID;
	static void SetLastID(long NewID); // NewID > ms_LastID 이면 치환. 아니면 무시

	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSRibTypeM)
	virtual void CopyFromMe(MSRibTypeM* pSource);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	enum Type_MSRib{Rib_Standard = 0};
	Type_MSRib GetRibType(){return m_RibType;};

protected:
	CString m_Name;
	Type_MSRib m_RibType;
};

