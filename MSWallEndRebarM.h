#pragma once
#include "MSObject.h"
#include "MSMainBar.h"

class StructureModel_CLASS MSWallEndRebarM : public MSObject
{
public:
	MSWallEndRebarM(void);
	~MSWallEndRebarM(void);

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSWallEndRebarM)

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSWallEndRebarM* pSource, bool bNewID=true);

	long m_ID;
	long GetID();
	static long ms_LastID;
	static void SetLastID(long NewID); // NewID > ms_LastID 이면 치환. 아니면 무시
	
	bool IsStoryIncluded(long nStory);

	void SetName(CString name)							{ m_Name = name;};
	void SetFromStory(int story)							{ m_nFromStory = story;};
	void SetToStory(int story)									{ m_nToStory = story;};
	void SetMainBar(CString rebar);
	void SetSpacing(int spacing)							{ m_Spacing = spacing;};

	CString GetName()											{ return m_Name;};
	int GetFromStory()												{ return m_nFromStory;};
	int GetToStory()													{ return m_nToStory;};
	MSMainBar GetMainBar()								{ return m_MainBar;};
	int GetSpacing()													{ return m_Spacing;};
	CString GetMainBarString();

	bool IsSameData(MSWallEndRebarM* pUrRebarM);
protected:
	CString m_Name;
	int m_nFromStory;
	int m_nToStory;
	int m_Spacing;
	MSMainBar m_MainBar;

};

