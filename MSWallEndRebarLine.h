#pragma once
#include "msobject.h"

#include "MSMainBar.h"
class MSWallEndRebarM;
class MSBaseBuilding;

class StructureModel_CLASS MSWallEndRebarLine : public MSObject
{
public:
	MSWallEndRebarLine(void);
	~MSWallEndRebarLine(void);

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSWallEndRebarLine)

	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSWallEndRebarLine* pSource, bool bNewID=true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	long m_ID;
	long GetID();
	static long ms_LastID;
	static void SetLastID(long NewID); // NewID > ms_LastID 이면 치환. 아니면 무시

	void SetBuilding(MSBaseBuilding* pBuilding)	{ mp_Building = pBuilding;};
	void SetName(CString name)							{ m_Name = name;};

	CString GetName()											{ return m_Name;};
	virtual MSBaseBuilding* GetBuilding() override { return mp_Building;};

	void Add(MSWallEndRebarM* pRebarM);
	vector<MSWallEndRebarM*> GetEndRebarMs()	{ return ma_EndRebarM;};

	MSWallEndRebarM* GetWallEndRebarM(int story);
	bool SetWallEndRebarM(int story, CString rebarText);

	MSWallEndRebarM* CreateWallEndRebarM(int fromStory, int toStory, CString rebarText, int spacing=0);

	void SortRebarMArr();
	void MergeRebarMArr();
	void DeleteEndRebarMArr();
	bool IsSameData(MSWallEndRebarLine* urEndRebarLine);
	void DeleteEndRebarM(MSWallEndRebarM* delM);
protected:
	void CheckEndRebarMStory(MSWallEndRebarM* pRebarM);

protected:
	MSBaseBuilding* mp_Building;
	CString			 m_Name;
	vector<MSWallEndRebarM*> ma_EndRebarM;
};

