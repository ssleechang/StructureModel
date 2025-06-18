#include "stdafx.h"
#include "MSFormWorkType.h"
#include "Radfcore_CLI/AppOptionsWrapper.h"
#include "GMLib/MSVersionInfo.h"
IMPLEMENT_SERIAL(MSFormWorkType, MSObject, VERSIONABLE_SCHEMA | 1)

MSFormWorkType::MSFormWorkType()
{
	m_Name = _T("");
	m_Alpha = 255;
	m_Color = RGB(255, 255, 255);
	m_bReadOnly = false;
	m_bShow = true;
    m_ChangedName = _T("");
    m_ConcreteThick = 0;
    m_FormworkRatio = 0;
	m_Suffix = _T("");
}

MSFormWorkType::~MSFormWorkType()
{
}

MSObject* MSFormWorkType::Copy(bool bNewID /*= true*/)
{
	MSFormWorkType* pNewObj;

	pNewObj = new MSFormWorkType();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;

}

void MSFormWorkType::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;

	MSFormWorkType* pSource = (MSFormWorkType*)pSource_in;
	m_Name = pSource->m_Name;
	m_Alpha = pSource->m_Alpha;
	m_Color = pSource->m_Color;
	m_bReadOnly = pSource->m_bReadOnly;
	m_bShow = pSource->m_bShow;
    m_ChangedName = pSource->m_ChangedName;
    m_ConcreteThick = pSource->m_ConcreteThick;
    m_FormworkRatio = pSource->m_FormworkRatio;
	m_Suffix = pSource->m_Suffix;

	MSObject::CopyFromMe(pSource, bNewID);
}
void MSFormWorkType::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	buffer << m_Name << m_Alpha << m_Color << m_bReadOnly << m_bShow << m_ChangedName << m_ConcreteThick << m_FormworkRatio << m_Suffix;
}

void MSFormWorkType::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	buffer >> m_Name >> m_Alpha >> m_Color >> m_bReadOnly >> m_bShow >> m_ChangedName >> m_ConcreteThick >> m_FormworkRatio >> m_Suffix;
}

void MSFormWorkType::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	CString oldPrintName = _T("");
	if (ar.IsStoring())
	{
		ar << m_Name;
		ar << m_Alpha;
		ar << m_Color;
		ar << m_bReadOnly;
		ar << m_bShow;
        ar << m_ChangedName;
        ar << m_ConcreteThick;
        ar << m_FormworkRatio;
		ar << m_Suffix;
	}
	else
	{
        CString name;
        ar >> name;
        SetName(name);
		ar >> m_Alpha;
		ar >> m_Color;
		ar >> m_bReadOnly;

		CString language(AppOptionsWrapper::Instance()->GetLanguage());
		if (MSVersionInfo::GetCurrentVersion() < 20161125)
		{
			if (language.MakeLower() == "ko-kr")
			{
				if (m_Name == _T("데크"))
				{
					m_Name = "Deck";
					m_bReadOnly = true;
				}
			}
			else if (language.MakeLower() == "en-us")
			{
				if (m_Name == _T("Deck"))
					m_bReadOnly = true;
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20170608)
		{
			ar >> m_bShow;
		}

        if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20180308)
        {
            ar >> m_ChangedName;
            ar >> m_ConcreteThick;
            ar >> m_FormworkRatio;
        }

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20200707)
		{
			ar >> m_Suffix;
		}
	}
}

CString MSFormWorkType::GetName()
{
	return this->m_Name;
}
void MSFormWorkType::SetName(CString name)
{
    if (m_Name == m_ChangedName)
        this->m_ChangedName = name;
	this->m_Name = name;
}

void MSFormWorkType::GetColor(int& alpha, int& red, int& green, int&blue)
{
	alpha = m_Alpha;
	red = GetRValue(m_Color);
	green = GetGValue(m_Color);
	blue = GetBValue(m_Color);
}
void MSFormWorkType::SetColor(int alpha, int red, int green, int blue)
{
	m_Alpha = alpha;
	m_Color = RGB(red, green, blue);
}
void MSFormWorkType::SetColor(int alpha, COLORREF color)
{
	m_Alpha = alpha;
	m_Color = color;
}

bool MSFormWorkType::GetReadOnly()
{
	return this->m_bReadOnly;
}
void MSFormWorkType::SetReadOnly(bool bReadOnly)
{
	this->m_bReadOnly = bReadOnly;
}

bool MSFormWorkType::GetShow()
{
	return this->m_bShow;
}
void MSFormWorkType::SetShow(bool bShow)
{
	this->m_bShow = bShow;
}

CString MSFormWorkType::GetChangedName()
{
    return this->m_ChangedName;
}
void MSFormWorkType::SetChangedName(CString changedName)
{
    this->m_ChangedName = changedName;
}

double MSFormWorkType::GetConcreteThick()
{
    return this->m_ConcreteThick;
}
void MSFormWorkType::SetConcreteThick(double concreteThick)
{
    this->m_ConcreteThick = concreteThick;
}

double MSFormWorkType::GetFormworkRatio()
{
    return this->m_FormworkRatio;
}
void MSFormWorkType::SetFormworkRatio(double formworkRate)
{
    this->m_FormworkRatio = formworkRate;
}

CString MSFormWorkType::GetSuffix()
{
	return this->m_Suffix;
}
void MSFormWorkType::SetSuffix(CString suffix)
{
	this->m_Suffix = suffix;
}