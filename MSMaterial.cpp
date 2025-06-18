#include "StdAfx.h"
#include "MSMaterial.h"
#include "MSBaseBuilding.h"
#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSMaterial, MSObject, VERSIONABLE_SCHEMA | 1)

long MSMaterial::ms_LastID = 0;
long MSMaterial::ms_GlobalLastID = 0;
MSMaterial::MSMaterial(void)
{
	m_ID = 0;
	m_GlobalID = 0;
	m_Name = _T("");
	m_Type = _T("");
	m_ElasModulus = 0.;
	m_ElasModulusAnal = 0.;
	m_PoisonRat = 0.;
	m_Density = 0.;
	m_pBldg = NULL;
	m_Alpha = 255;
	m_Color = RGB(255, 255, 255);
	m_bIsUserSetColor = false;
}

MSMaterial::~MSMaterial(void)
{
}

// m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
long MSMaterial::GetID()
{
	if(m_ID == 0) // 
	{
		m_ID = MSBaseBuilding::GetUID(UIDType::msMaterial);
		ms_LastID = m_ID;
		return m_ID;
	}

	return m_ID;
}

void MSMaterial::SetLastID( long NewID )
{
	if(NewID > ms_LastID)
		ms_LastID = NewID;
}

long MSMaterial::GetGlobalID()
{
	if(m_GlobalID == 0) // 
	{
		m_GlobalID = ms_GlobalLastID + 1;
		ms_GlobalLastID = m_GlobalID;
		return m_GlobalID;
	}

	return m_GlobalID;
}

void MSMaterial::SetGlobalLastID( long NewID )
{
	if(NewID > ms_GlobalLastID)
		ms_GlobalLastID = NewID;
}

MSObject* MSMaterial::Copy( bool bNewID /*= true*/ )
{
	MSMaterial* pNewObj;

	pNewObj = new MSMaterial();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;

}

void MSMaterial::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;

	MSMaterial* pSource = (MSMaterial*)pSource_in;
	//m_Name = pSource->m_Name;
	m_Name = LocalDic::GetLocal(pSource->m_Name);
	m_Type = pSource->m_Type;
	m_ElasModulus = pSource->m_ElasModulus;
	m_ElasModulusAnal = pSource->m_ElasModulusAnal;
	m_PoisonRat = pSource->m_PoisonRat;
	m_Density = pSource->m_Density;
	m_Alpha = pSource->m_Alpha;
	m_Color = pSource->m_Color;

	MSObject::CopyFromMe(pSource, bNewID);
}

void MSMaterial::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	buffer << m_Name << m_Type;
	buffer << m_ElasModulus << m_ElasModulusAnal << m_PoisonRat << m_Density << m_Alpha << m_Color << m_bIsUserSetColor;
}

void MSMaterial::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	buffer >> m_Name >> m_Type;
	buffer >> m_ElasModulus >> m_ElasModulusAnal >> m_PoisonRat >> m_Density >> m_Alpha >> m_Color >> m_bIsUserSetColor;
}

void MSMaterial::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	CString temp = _T("");
	if(ar.IsStoring())
	{
		ar <<		m_ID;
		ar <<       m_Name;
		ar <<		temp;
		ar <<		m_Type;
		ar <<		m_ElasModulus;
		ar <<		m_ElasModulusAnal;
		ar <<		m_PoisonRat;
		ar <<		m_Density;
		ar <<		m_IsAddedMat;
		ar << m_Alpha;
		ar << m_Color;
		ar << m_bIsUserSetColor;
	}
	else
	{
		ar >>		m_ID;
		ar >>       m_Name;
		m_Name = m_Name.MakeUpper();
		ar >>		temp;
		ar >>		m_Type;
		ar >>		m_ElasModulus;
		ar >>		m_ElasModulusAnal;
		ar >>		m_PoisonRat;
		ar >>		m_Density;
		
		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20210701)
			ar >> m_IsAddedMat;
		
		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231228 || MSVersionInfo::IsOldTotalVersion())
		{
			ar >> m_Alpha;
			ar >> m_Color;
			ar >> m_bIsUserSetColor;
		}
		else
		{
			m_Alpha = 255;
			m_Color = RGB(255, 255, 255);
			m_bIsUserSetColor = false;
		}

		SetLastID(m_ID);
	}
}

void MSMaterial::GlobalOptionSerialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	CString temp = _T("");
	if(ar.IsStoring())
	{
		ar <<		m_GlobalID;
		ar <<       m_Name;
		ar <<		temp;
		ar <<		m_Type;
		ar <<		m_ElasModulus;
		ar <<		m_ElasModulusAnal;
		ar <<		m_PoisonRat;
		ar <<		m_Density;
		ar << m_Alpha;
		ar << m_Color;
		ar << m_bIsUserSetColor;
	}
	else
	{
		ar >>		m_GlobalID;
		ar >>       m_Name;
		m_Name = m_Name.MakeUpper();
		ar >>		temp;
		ar >>		m_Type;
		ar >>		m_ElasModulus;
		ar >>		m_ElasModulusAnal;
		ar >>		m_PoisonRat;
		ar >>		m_Density;

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231228 || MSVersionInfo::IsOldTotalVersion())
		{
			ar >> m_Alpha;
			ar >> m_Color;
			ar >> m_bIsUserSetColor;
		}
		else
		{
			m_Alpha = 255;
			m_Color = RGB(255, 255, 255);
			m_bIsUserSetColor = false;
		}

		SetGlobalLastID(m_GlobalID);
	}
}

void MSMaterial::SetName(CString name)
{
	m_Name = name;
}

CString MSMaterial::GetName()
{
	return m_Name;
}

long MSMaterial::GetMaterialIndex()
{
	long rValue = 0;

	if(m_Name == "C15")			 rValue = 0;
	else if(m_Name == "C18") rValue = 1;
	else if(m_Name == "C21") rValue = 2;
	else if(m_Name == "C24") rValue = 3;
	else if(m_Name == "C27") rValue = 4;
	else if(m_Name == "C30") rValue = 5;
	else if(m_Name == "C35") rValue = 6;
	else if(m_Name == "C40") rValue = 7;
	else if(m_Name == "C45") rValue = 8;
	else if(m_Name == "C50") rValue = 9;
	else if(m_Name == "C55") rValue = 10;
	else if(m_Name == "C60") rValue = 11;
	else ASSERT(0);

	return rValue;
}
void MSMaterial::SetMaterialIndex(int Index)
{
	if(Index == 0)				m_Name = "C15";
	else if(Index == 1)		m_Name = "C18";
	else if(Index == 2)		m_Name = "C21";
	else if(Index == 3)		m_Name = "C24";
	else if(Index == 4)		m_Name = "C27";
	else if(Index == 5)		m_Name = "C30";
	else if(Index == 6)		m_Name = "C35";
	else if(Index == 7)		m_Name = "C40";
	else if(Index == 8)		m_Name = "C45";
	else if(Index == 9)		m_Name = "C50";
	else if(Index == 10)	m_Name = "C55";
	else if(Index == 11)	m_Name = "C60";
}

vector<CString> MSMaterial::GetMaterialList()
{
	vector<CString> materialList;

	materialList.push_back(_T("15"));
	materialList.push_back(_T("18"));
	materialList.push_back(_T("21"));
	materialList.push_back(_T("24"));
	materialList.push_back(_T("27"));
	materialList.push_back(_T("30"));
	materialList.push_back(_T("35"));
	materialList.push_back(_T("40"));
	materialList.push_back(_T("45"));
	materialList.push_back(_T("50"));
	materialList.push_back(_T("55"));
	materialList.push_back(_T("60"));

	return materialList;
}

void MSMaterial::GetColor(int& alpha, int& red, int& green, int&blue)
{
	alpha = m_Alpha;
	red = GetRValue(m_Color);
	green = GetGValue(m_Color);
	blue = GetBValue(m_Color);
}
void MSMaterial::SetColor(int alpha, int red, int green, int blue)
{
	m_Alpha = alpha;
	m_Color = RGB(red, green, blue);
	m_bIsUserSetColor = true;
}
void MSMaterial::SetColor(int alpha, COLORREF color)
{
	m_Alpha = alpha;
	m_Color = color;
	m_bIsUserSetColor = true;
}