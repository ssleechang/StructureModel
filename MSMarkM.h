#pragma once
#include "msobject.h"
class MSBaseBuilding;
class StructureModel_CLASS MSMarkM : public MSObject
{
public:
	MSMarkM(void);
	~MSMarkM(void);

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSMarkM)
	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	
	long m_ID;
	static long ms_LastID;
	long GetID();  // m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
	static void SetLastID(long NewID); // NewID > ms_LastID 이면 치환. 아니면 무시
	virtual void Init(bool bInConstructor=false);

	CString GetMarkName();

	enum Type_MSMark {msPlan=0, msSection=1, msRebar, msEtc}; 
	Type_MSMark m_nType;
	int m_nTagNum;
	CString m_Comment;
	int m_nErrorCode;

	MSBaseBuilding* GetBuilding()						{ return mp_Bldg;};
	void SetBuilding(MSBaseBuilding* pBldg)	{ mp_Bldg = pBldg;};
	bool GetIsVisible()							{ return m_IsVisible;};
	void SetIsVisible(bool visible)		{ m_IsVisible = visible;};
protected:
	bool m_IsVisible;
	MSBaseBuilding* mp_Bldg;

};

