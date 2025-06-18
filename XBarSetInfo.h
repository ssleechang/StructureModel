#pragma once

// XBarSetInfo 명령 대상입니다.

class StructureModel_CLASS XBarSetInfo : public CObject
{
public:
	void Serialize( CArchive& archive );
	DECLARE_SERIAL( XBarSetInfo )
	long m_nCZoneID;
	long m_nGroupID;
	CString m_strFilePath;
	void Initialize()
	{
		m_strFilePath = _T("");
		m_nCZoneID = 0;
		m_nGroupID = 0;
	}

	XBarSetInfo(){Initialize();};
	~XBarSetInfo(){};
	XBarSetInfo(XBarSetInfo& aData)	{*this = aData;}
	XBarSetInfo& operator = (XBarSetInfo& aData)
	{
		Initialize();

		m_nCZoneID = aData.m_nCZoneID;
		m_nGroupID = aData.m_nGroupID;
		m_strFilePath = aData.m_strFilePath;
		return *this;
	}
	CString GetFileName();

};


