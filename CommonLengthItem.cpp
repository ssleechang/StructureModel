#include "StdAfx.h"
#include "CommonLengthItem.h"


CommonLengthItem::CommonLengthItem(void)
{
	Initialize();
}


CommonLengthItem::~CommonLengthItem(void)
{
}

void CommonLengthItem::Serialize( CArchive& ar )
{
	if(ar.IsStoring())
	{
		ar << m_nCompLength;
		ar << m_nDevLength4Hook;
	}
	else
	{

		ar >> m_nCompLength;
		ar >> m_nDevLength4Hook;
	}

}

void CommonLengthItem::Initialize()
{
	m_nCompLength = 10.;
	m_nDevLength4Hook = 10.;
}

void CommonLengthItem::Copy(CommonLengthItem desCommonLengthItem)
{
	m_nCompLength = desCommonLengthItem.m_nCompLength;
	m_nDevLength4Hook = desCommonLengthItem.m_nDevLength4Hook;
}