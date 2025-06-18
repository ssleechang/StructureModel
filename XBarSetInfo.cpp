// XBarSetInfo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "StructureModel.h"
#include "XBarSetInfo.h"


// XBarSetInfo
IMPLEMENT_SERIAL( XBarSetInfo, CObject, VERSIONABLE_SCHEMA | 1 )
// XBarSetInfo 멤버 함수
void XBarSetInfo::Serialize(CArchive &ar)
{
	if(ar.IsStoring())
	{
		ar << m_nCZoneID;
		ar << m_nGroupID;
		ar << m_strFilePath;
	}
	else
	{
		ar >> m_nCZoneID;
		ar >> m_nGroupID;
		ar >> m_strFilePath;
	}
}

CString XBarSetInfo::GetFileName()
{
	CString strFileName = m_strFilePath;
	CString RtnVal;
	int nLength = strFileName.GetLength();
	int pos = strFileName.ReverseFind('\\');
	RtnVal = strFileName.Mid(pos+1, nLength-pos);
	return RtnVal;
}