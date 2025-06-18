#include "StdAfx.h"
#include "MSMarkM.h"
#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSMarkM, MSObject, VERSIONABLE_SCHEMA | 1)
long MSMarkM::ms_LastID = 0;

MSMarkM::MSMarkM()
{
	Init(true);
}

MSMarkM::~MSMarkM(void)
{
}

void MSMarkM::Init(bool bInConstructor)
{
	if(!bInConstructor)
		MSObject::Init(bInConstructor);

	mp_Bldg = NULL;
	m_ID = 0;
	m_nTagNum = 0;
	m_nType = msPlan;
	m_Comment = _T("");
	m_nErrorCode = 0;
	m_IsVisible = true;
}

// m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
long MSMarkM::GetID()
{
	if(m_ID == 0) // 
	{
		m_ID = ms_LastID + 1;
		ms_LastID = m_ID;
		return m_ID;
	}

	return m_ID;
}

void MSMarkM::SetLastID( long NewID )
{
	if(NewID > ms_LastID)
		ms_LastID = NewID;
}

MSObject* MSMarkM::Copy( bool bNewID /*= true*/ )
{
	MSMarkM* pNewObj = new MSMarkM();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSMarkM::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/ )
{
	if(pSource_in == nullptr) return;

	MSMarkM* pSource = (MSMarkM*)pSource_in;

	mp_Bldg = pSource->mp_Bldg;
	m_nTagNum = pSource->m_nTagNum;
	m_nType = pSource->m_nType;
	m_Comment = pSource->m_Comment;
	m_nErrorCode = pSource->m_nErrorCode;
	m_IsVisible = pSource->m_IsVisible;
	if(bNewID)
		GetID();
}

void MSMarkM::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	int nType = (int)m_nType;
	buffer << m_nTagNum << m_nType << m_Comment << m_nErrorCode;
	buffer << m_IsVisible;
}

void MSMarkM::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	int nType = 0;
	buffer >> m_nTagNum >> nType >> m_Comment >> m_nErrorCode;
	buffer >> m_IsVisible;

	m_nType = (Type_MSMark)nType;
}

void MSMarkM::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	int nType = 0;
	if(ar.IsStoring())
	{
		ar << m_ID;
		ar << m_nTagNum;
		nType = (int)m_nType;
		ar << nType;
		ar << m_Comment;
//		ar << m_nErrorCode;
		ar << m_IsVisible;
	}
	else 
	{
		ar >> m_ID;
		ar >> m_nTagNum;
		ar >> nType;
		m_nType = (Type_MSMark)nType;
		ar >> m_Comment;
		
//  		if(MSVersionInfo::GetCurrentVersion() >= 20140724)
//  			ar >> m_nErrorCode;

		if(MSVersionInfo::GetCurrentVersion() >= 20150805)
			ar >> m_IsVisible;

		SetLastID(m_ID);
	}
}

CString MSMarkM::GetMarkName()
{
	CString name = nullptr;

	if(m_nType == Type_MSMark::msPlan)
		name.Format(CFormatMsg(LocalDic::GetLocal(_T("A-평면오류-%1!d!")), m_nTagNum));
		//name.Format(_T("A-평면오류-%d"), m_nTagNum);
	else if(m_nType == Type_MSMark::msSection)
		name.Format(CFormatMsg(LocalDic::GetLocal(_T("B-단면오류-%1!d!")), m_nTagNum));
	else if(m_nType == Type_MSMark::msRebar)
		name.Format(CFormatMsg(LocalDic::GetLocal(_T("C-배근오류-%1!d!")), m_nTagNum));
	else if(m_nType == Type_MSMark::msEtc)
		name.Format(CFormatMsg(LocalDic::GetLocal(_T("D-기타오류-%1!d!")), m_nTagNum));
	
	return name;
}