#include "StdAfx.h"
#include "MSWallEndRebarM.h"
#include "MSDeformedBar.h"

IMPLEMENT_SERIAL(MSWallEndRebarM, MSObject, VERSIONABLE_SCHEMA | 1)
long MSWallEndRebarM::ms_LastID = 0;

MSWallEndRebarM::MSWallEndRebarM(void)
{
	m_Name = _T("");
	m_ID = 0;
	m_nFromStory = m_nToStory = 0;
	m_MainBar.InitData(MSDeformedBar::GetMainBarStr(4, 13));	
	m_Spacing = 0;
}


MSWallEndRebarM::~MSWallEndRebarM(void)
{
}

long MSWallEndRebarM::GetID()
{
	if(m_ID == 0) // 
	{
		m_ID = ms_LastID + 1;
		SetLastID(m_ID);
		return m_ID;
	}

	return m_ID;
}

void MSWallEndRebarM::SetLastID( long NewID )
{
	if(NewID > ms_LastID)
		ms_LastID = NewID;
}

MSObject* MSWallEndRebarM::Copy(bool bNewID /*= true*/)
{
	MSWallEndRebarM* pNewObj = new MSWallEndRebarM();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSWallEndRebarM::CopyFromMe(MSWallEndRebarM* pSource, bool bNewID/*=true*/)
{
	if(pSource==NULL)	return;
	m_Name				=	pSource->m_Name;
	m_nFromStory    = pSource->m_nFromStory;
	m_nToStory			=	pSource->m_nToStory;;
	m_MainBar.CopyFromMe(&pSource->m_MainBar);
	m_Spacing			= pSource->m_Spacing;
	if(bNewID)
		GetID();
}

void MSWallEndRebarM::Dump( EFS::EFS_Buffer& buffer )
{
	MSObject::Dump(buffer);
	buffer << m_Name << m_nFromStory << m_nToStory << m_Spacing;
	m_MainBar.Dump(buffer);
}

void MSWallEndRebarM::Recovery( EFS::EFS_Buffer& buffer )
{
	MSObject::Recovery(buffer);
	buffer >> m_Name >> m_nFromStory >> m_nToStory >> m_Spacing;
	m_MainBar.Recovery(buffer);
}


void MSWallEndRebarM::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_ID;
		ar << m_Name;
		ar << m_nFromStory;
		ar << m_nToStory;
		ar << m_Spacing;
		m_MainBar.Serialize(ar);
	}
	else
	{
		ar >> m_ID;
		ar >> m_Name;
		ar >> m_nFromStory;
		ar >> m_nToStory;
		ar >> m_Spacing;
		m_MainBar.Serialize(ar);
		SetLastID(m_ID);
	}
}

void MSWallEndRebarM::SetMainBar(CString rebar)
{
	rebar.Replace(_T(" "), _T(""));
	m_MainBar.InitData(rebar.MakeUpper());
}
bool MSWallEndRebarM::IsStoryIncluded(long nStory)
{
	if(m_nFromStory <= nStory && nStory <= m_nToStory)
		return true;
	return false;
}

CString MSWallEndRebarM::GetMainBarString()
{
	return m_MainBar.GetTotalMainBarString();
}

bool MSWallEndRebarM::IsSameData(MSWallEndRebarM* pUrRebarM)
{
	if(pUrRebarM == NULL)	return false;
	if(GetMainBarString() != pUrRebarM->GetMainBarString())
		return false;
	if(GetSpacing() != pUrRebarM->GetSpacing())
		return false;
	if (m_nFromStory != pUrRebarM->GetFromStory())		return false;
	if (m_nToStory != pUrRebarM->GetToStory())	return false;
	return true;
}
