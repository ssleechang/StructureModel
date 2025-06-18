#include "stdafx.h"
#include "MSFormWorkDisplay.h"

IMPLEMENT_SERIAL(MSFormWorkDisplay, MSObject, VERSIONABLE_SCHEMA | 1)

//거푸집 Display 정보
MSFormWorkDisplay::MSFormWorkDisplay()
{
	m_Alpha = 255;
	m_Color = RGB(255, 255, 255);
}


MSFormWorkDisplay::~MSFormWorkDisplay()
{
}

MSObject* MSFormWorkDisplay::Copy(bool bNewID /*= true*/)
{
	MSFormWorkDisplay* pNewObj;

	pNewObj = new MSFormWorkDisplay();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;

}

void MSFormWorkDisplay::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;

	MSFormWorkDisplay* pSource = (MSFormWorkDisplay*)pSource_in;
	m_Alpha = pSource->m_Alpha;
	m_Color = pSource->m_Color;

	MSObject::CopyFromMe(pSource, bNewID);
}
void MSFormWorkDisplay::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	buffer << m_Alpha << m_Color;
}

void MSFormWorkDisplay::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	buffer >> m_Alpha >> m_Color;
}

void MSFormWorkDisplay::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	CString oldPrintName = _T("");
	if (ar.IsStoring())
	{
		ar << m_Alpha;
		ar << m_Color;
	}
	else
	{
		ar >> m_Alpha;
		ar >> m_Color;	
	}
}