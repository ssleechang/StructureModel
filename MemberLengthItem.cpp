#include "StdAfx.h"
#include "MemberLengthItem.h"


MemberLengthItem::MemberLengthItem(void)
{
	Initialize();
}


MemberLengthItem::~MemberLengthItem(void)
{
}

void MemberLengthItem::Serialize( CArchive& ar )
{
	if(ar.IsStoring())
	{
		ar << m_dDevelopmentLength;
		ar << m_dSpliceLength;
	}
	else
	{
		ar >> m_dDevelopmentLength;
		ar >> m_dSpliceLength;
	}
}

void MemberLengthItem::Initialize()
{
	m_dDevelopmentLength = 10.;
	m_dSpliceLength= 10.;
}

void MemberLengthItem::Copy(MemberLengthItem memberLengthItem)
{
	m_dDevelopmentLength = memberLengthItem.m_dDevelopmentLength;
	m_dSpliceLength = memberLengthItem.m_dSpliceLength;
}
