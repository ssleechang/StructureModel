#include "stdafx.h"
#include "XWallMInfo.h"


XWallMInfo::XWallMInfo()
{
	Initialize();
}


XWallMInfo::~XWallMInfo()
{
}

void XWallMInfo::Initialize()
{
	m_Name = _T("");
	m_Thick = 0;
	m_VBarText = _T("");
	m_HBarText = _T("");
	m_nFromStory = 0;
	m_nToStory = 0;
	m_IsMerged = false;
}

XWallMInfo& XWallMInfo::operator = (XWallMInfo& aData)
{
	Initialize();
	m_Name = aData.m_Name;
	m_Thick = aData.m_Thick;
	m_VBarText = aData.m_VBarText;
	m_HBarText = aData.m_HBarText;
	m_nFromStory = aData.m_nFromStory;
	m_nToStory = aData.m_nToStory;

	return *this;
}

bool XWallMInfo::IsSameData(XWallMInfo* UrWall)
{
	if (m_Name != UrWall->m_Name)	return false;
	if (m_Thick != UrWall->m_Thick)	return false;
	if (m_nFromStory != UrWall->m_nFromStory)	return false;
	if (m_nToStory != UrWall->m_nToStory)	return false;
	if (m_VBarText != UrWall->m_VBarText)	return false;
	if (m_HBarText != UrWall->m_HBarText)	return false;

	return true;
}

bool XWallMInfo::IsMerged(XWallMInfo* UrWall)
{
	if (m_Name != UrWall->m_Name)	return false;
	if (m_Thick != UrWall->m_Thick)	return false;
	if (m_VBarText != UrWall->m_VBarText)	return false;
	if (m_HBarText != UrWall->m_HBarText)	return false;

	return true;
}

bool XWallMInfo::IsOverlapStory(XWallMInfo* UrWall)
{
	int myFrom = m_nFromStory;
	int myTo = m_nToStory;
	int urFrom = UrWall->m_nFromStory;
	int urTo = UrWall->m_nToStory;
	if (myFrom <= urFrom && urFrom <= myTo)	return true;
	if (myFrom <= urTo && urTo <= myTo)	return true;

	if (urFrom <= myFrom && myFrom <= urTo)	return true;
	if (urFrom <= myTo && myTo <= urTo)	return true;

	return false;
}

void XWallMInfo::CopyData(XWallMInfo* pSource)
{
	m_Name = pSource->m_Name;
	m_Thick = pSource->m_Thick;
	m_VBarText = pSource->m_VBarText;
	m_HBarText = pSource->m_HBarText;
	m_nFromStory = pSource->m_nFromStory;
	m_nToStory = pSource->m_nToStory;
}

