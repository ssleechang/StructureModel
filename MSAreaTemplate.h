#pragma once
#include "MSArea.h"

class StructureModel_CLASS MSAreaTemplate : public MSObject
{
public:
	MSAreaTemplate(void);
	~MSAreaTemplate(void);

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSAreaTemplate)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);

	long m_ID;
	long GetID();  // m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
	static long ms_LastID;
	static void SetLastID(long NewID); // NewID > ms_LastID 이면 치환. 아니면 무시

	void Add(MSArea* area);

	bool Remove(long Id);

	vector<MSArea*> GetAreaList();

	CString GetName() { return m_Name; };
	void SetName(CString name) { m_Name = name; };

	MSArea* GetArea(long Id);

	void DeleteAreaList();

private:
	CString m_Name;
	map<long, MSArea*> mm_Area;

};

