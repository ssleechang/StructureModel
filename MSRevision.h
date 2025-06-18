#pragma once
#include "afx.h"
class MSRevision :
	public CObject
{
public:
	MSRevision(void);
	~MSRevision(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSRevision)

	CString m_Mark;
	CString m_Description;
	CTime m_TimeStamp;

	void SetTime();
};

