#pragma once
#include "afx.h"

class MSCompoM;
class StructureModel_CLASS XWallMInfo : public CObject 
{
public:
	CString m_Name;
	long m_Thick;
	CString m_VBarText;
	CString m_HBarText;
	long m_nFromStory, m_nToStory;
	bool m_IsMerged;
	
	XWallMInfo();
	~XWallMInfo();
	XWallMInfo(XWallMInfo& aData) { *this = aData; }
	XWallMInfo& operator = (XWallMInfo& aData);
	void CopyData(XWallMInfo* pSource);
	
	void Initialize();
	bool IsSameData(XWallMInfo* UrWall);
	bool IsOverlapStory(XWallMInfo* UrWall);
	bool IsMerged(XWallMInfo* urWall);

protected:
};

