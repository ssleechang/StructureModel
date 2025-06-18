#include "stdafx.h"
#include "VerMemberLinkDowelTemplate.h"
#include "..\GMLib\MSVersionInfo.h"

VerMemberLinkDowelTemplate::VerMemberLinkDowelTemplate()
{
	m_pJointPartTopLinkDowelPlaceInfo = new LinkDowelPlaceInfo();
	m_pJointPartBotLinkDowelPlaceInfo = new LinkDowelPlaceInfo();
	m_pTopDownTopLinkDowelPlaceInfo = new LinkDowelPlaceInfo();
	m_pTopDownBotLinkDowelPlaceInfo = new LinkDowelPlaceInfo();
}
VerMemberLinkDowelTemplate::VerMemberLinkDowelTemplate(VerMemberLinkDowelTemplate *pTemplate)
{
	DeleteLinkDowelPlaceInfo();
	if(m_pJointPartTopLinkDowelPlaceInfo == nullptr)
		m_pJointPartTopLinkDowelPlaceInfo = new LinkDowelPlaceInfo();
	m_pJointPartTopLinkDowelPlaceInfo->CopyFromMe(pTemplate->GetJointPartTopLinkDowelPlaceInfo());
	if(m_pJointPartBotLinkDowelPlaceInfo == nullptr)
		m_pJointPartBotLinkDowelPlaceInfo = new LinkDowelPlaceInfo();
	m_pJointPartBotLinkDowelPlaceInfo->CopyFromMe(pTemplate->GetJointPartBotLinkDowelPlaceInfo());
	m_pTopDownTopLinkDowelPlaceInfo->CopyFromMe(pTemplate->GetTopDownTopLinkDowelPlaceInfo());
	if(m_pTopDownTopLinkDowelPlaceInfo == nullptr)
		m_pTopDownTopLinkDowelPlaceInfo = new LinkDowelPlaceInfo();
	m_pTopDownBotLinkDowelPlaceInfo->CopyFromMe(pTemplate->GetTopDownBotLinkDowelPlaceInfo());
	if(m_pTopDownBotLinkDowelPlaceInfo == nullptr)
		m_pTopDownBotLinkDowelPlaceInfo = new LinkDowelPlaceInfo();
}
void VerMemberLinkDowelTemplate::CopyFromMe(VerMemberLinkDowelTemplate *pTemplate)
{
	m_pJointPartTopLinkDowelPlaceInfo->CopyFromMe(pTemplate->GetJointPartTopLinkDowelPlaceInfo());
	m_pJointPartBotLinkDowelPlaceInfo->CopyFromMe(pTemplate->GetJointPartBotLinkDowelPlaceInfo());
	m_pTopDownTopLinkDowelPlaceInfo->CopyFromMe(pTemplate->GetTopDownTopLinkDowelPlaceInfo());
	m_pTopDownBotLinkDowelPlaceInfo->CopyFromMe(pTemplate->GetTopDownBotLinkDowelPlaceInfo());
}
VerMemberLinkDowelTemplate::~VerMemberLinkDowelTemplate()
{
	DeleteLinkDowelPlaceInfo();
}
void VerMemberLinkDowelTemplate::DeleteLinkDowelPlaceInfo()
{
	if(m_pJointPartTopLinkDowelPlaceInfo != nullptr)
	{
		delete m_pJointPartTopLinkDowelPlaceInfo;
		m_pJointPartTopLinkDowelPlaceInfo = nullptr;
	}
	if(m_pJointPartBotLinkDowelPlaceInfo != nullptr)
	{
		delete m_pJointPartBotLinkDowelPlaceInfo;
		m_pJointPartBotLinkDowelPlaceInfo = nullptr;
	}
	if(m_pTopDownTopLinkDowelPlaceInfo != nullptr)
	{
		delete m_pTopDownTopLinkDowelPlaceInfo;
		m_pTopDownTopLinkDowelPlaceInfo = nullptr;
	}
	if(m_pTopDownBotLinkDowelPlaceInfo != nullptr)
	{
		delete m_pTopDownBotLinkDowelPlaceInfo;
		m_pTopDownBotLinkDowelPlaceInfo = nullptr;
	}
}
void VerMemberLinkDowelTemplate::Serialize(CArchive& ar)
{
	m_pJointPartTopLinkDowelPlaceInfo->Serialize(ar);
	m_pJointPartBotLinkDowelPlaceInfo->Serialize(ar);
	m_pTopDownTopLinkDowelPlaceInfo->Serialize(ar);
	m_pTopDownBotLinkDowelPlaceInfo->Serialize(ar);
}
LinkDowelPlaceInfo *VerMemberLinkDowelTemplate::GetJointPartTopLinkDowelPlaceInfo()
{
	return m_pJointPartTopLinkDowelPlaceInfo;
}
LinkDowelPlaceInfo *VerMemberLinkDowelTemplate::GetJointPartBotLinkDowelPlaceInfo()
{
	return m_pJointPartBotLinkDowelPlaceInfo;
}
LinkDowelPlaceInfo *VerMemberLinkDowelTemplate::GetTopDownTopLinkDowelPlaceInfo()
{
	return m_pTopDownTopLinkDowelPlaceInfo;
}
LinkDowelPlaceInfo *VerMemberLinkDowelTemplate::GetTopDownBotLinkDowelPlaceInfo()
{
	return m_pTopDownBotLinkDowelPlaceInfo;
}

//LinkDowelPlaceInfo
LinkDowelPlaceInfo::LinkDowelPlaceInfo()
{
	m_nSpliceType = 1;
	m_nConstructionHeight = 300;
	m_bAlterSpliceSet = FALSE;
	m_nAlterSpliceDist = 400;
	m_nAlterSpliceType = 0;
}
LinkDowelPlaceInfo::LinkDowelPlaceInfo(LinkDowelPlaceInfo *pInfo)
{
	m_nSpliceType = pInfo->GetSpliceType();
	m_nConstructionHeight = pInfo->GetConstructionHeight();
	m_bAlterSpliceSet = pInfo->GetAlterSpliceSet();
	m_nAlterSpliceDist = pInfo->GetAlterSpliceDist();
	m_nAlterSpliceType = pInfo->GetAlterSpliceType();
}
void LinkDowelPlaceInfo::CopyFromMe(LinkDowelPlaceInfo *pInfo)
{
	m_nSpliceType = pInfo->GetSpliceType();
	m_nConstructionHeight = pInfo->GetConstructionHeight();
	m_bAlterSpliceSet = pInfo->GetAlterSpliceSet();
	m_nAlterSpliceDist = pInfo->GetAlterSpliceDist();
	m_nAlterSpliceType = pInfo->GetAlterSpliceType();
}
LinkDowelPlaceInfo::~LinkDowelPlaceInfo()
{
}
void LinkDowelPlaceInfo::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_nSpliceType;
		ar << m_nConstructionHeight;
		ar << m_bAlterSpliceSet;
		ar << m_nAlterSpliceDist;
		ar << m_nAlterSpliceType;
	}
	else
	{
		ar >> m_nSpliceType;
		ar >> m_nConstructionHeight;
		ar >> m_bAlterSpliceSet;
		ar >> m_nAlterSpliceDist;
		if (MSVersionInfo::GetCurrentVersion() >= 20250219)
			ar >> m_nAlterSpliceType;
		else
			m_nAlterSpliceType = 1;
	}
}
long LinkDowelPlaceInfo::GetSpliceType()
{
	return m_nSpliceType;
}
void LinkDowelPlaceInfo::SetSpliceType(long value)
{
	m_nSpliceType = value;
}
long LinkDowelPlaceInfo::GetConstructionHeight()
{
	return m_nConstructionHeight;
}
void LinkDowelPlaceInfo::SetConstructionHeight(long value)
{
	m_nConstructionHeight = value;
}
BOOL LinkDowelPlaceInfo::GetAlterSpliceSet()
{
	return m_bAlterSpliceSet;
}
void LinkDowelPlaceInfo::SetAlterSpliceSet(BOOL value)
{
	m_bAlterSpliceSet = value;
}
long LinkDowelPlaceInfo::GetAlterSpliceDist()
{
	return m_nAlterSpliceDist;
}
void LinkDowelPlaceInfo::SetAlterSpliceDist(long value)
{
	m_nAlterSpliceDist = value;
}
long LinkDowelPlaceInfo::GetAlterSpliceType()
{
	return m_nAlterSpliceType;
}

void LinkDowelPlaceInfo::SetAlterSpliceType(long value)
{
	m_nAlterSpliceType = value;
}