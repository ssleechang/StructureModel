#include "stdafx.h"
#include "MSCustomPointBarUnit.h"

#include "MSCustomSection.h"
#include "MSCustomReBarLine.h"
#include "MSCustomLocation.h"
#include "MSCustomRebar.h"
#include "MSDeformedBar.h"
#include "MSCustomLineSection.h"

#include "GMLib/GM2DLineVector.h"
#include "GMLib/GM2DArc.h"

IMPLEMENT_SERIAL(MSCustomPointBarUnit, MSCustomBarUnit, VERSIONABLE_SCHEMA | 1)

MSCustomPointBarUnit::MSCustomPointBarUnit(void)
{
	m_eBarType = MSCustomPointBarUnit::MainBar;
	m_eSpacingType = MSCustomPointBarUnit::Equal;
	m_StartOffset = 0;
	m_EndOffset = 0;
}

MSCustomPointBarUnit::~MSCustomPointBarUnit(void)
{
}

void MSCustomPointBarUnit::CopyFromMe(MSCustomBarUnit* pSource)
{
	if (pSource == NULL)	return;

	MSCustomBarUnit::CopyFromMe(pSource);

	MSCustomPointBarUnit* pSourceUnit = (MSCustomPointBarUnit*)pSource;
	m_eBarType = pSourceUnit->GetBarType();
	m_eSpacingType = pSourceUnit->GetSpacingType();
	m_StartOffset = pSourceUnit->GetStartOffset();
	m_EndOffset = pSourceUnit->GetEndOffset();
	m_LocationP0 = pSourceUnit->GetCustomLocationP0();
	m_LocationP1 = pSourceUnit->GetCustomLocationP1();
}

void MSCustomPointBarUnit::Dump(EFS::EFS_Buffer& buffer)
{
	MSCustomBarUnit::Dump(buffer);

	int barType = (int)m_eBarType;
	int spacingType = (int)m_eSpacingType;

	buffer << barType;
	buffer << spacingType;
	buffer << m_StartOffset << m_EndOffset;

	m_LocationP0.Dump(buffer);
	m_LocationP1.Dump(buffer);
}

void MSCustomPointBarUnit::Recovery(EFS::EFS_Buffer& buffer)
{
	MSCustomBarUnit::Recovery(buffer);

	int barType = 0, spacingType = 0;

	buffer >> barType;
	buffer >> spacingType;

	m_eBarType = (ePointBarType)barType;
	m_eSpacingType = (eSpacingType)spacingType;
	buffer >> m_StartOffset >> m_EndOffset;

	m_LocationP0.Recovery(buffer);
	m_LocationP1.Recovery(buffer);
}

void MSCustomPointBarUnit::SetCustomLocationP0(long rebarLineID1, long rebarLineID2)
{
	m_LocationP0.SetRebarLineID1(rebarLineID1);
	m_LocationP0.SetRebarLineID2(rebarLineID2);
}

void MSCustomPointBarUnit::SetCustomLocationP1(long rebarLineID1, long rebarLineID2)
{
	m_LocationP1.SetRebarLineID1(rebarLineID1);
	m_LocationP1.SetRebarLineID2(rebarLineID2);
}

void MSCustomPointBarUnit::Serialize(CArchive &ar)
{
	MSCustomBarUnit::Serialize(ar);
	int barType = 0, spacingType = 0;
	if (ar.IsStoring())
	{
		barType = (int)m_eBarType;
		spacingType = (int)m_eSpacingType;

		ar << barType;
		ar << spacingType;
		ar << m_StartOffset;
		ar << m_EndOffset;

		m_LocationP0.Serialize(ar);
		m_LocationP1.Serialize(ar);
	}
	else
	{
		ar >> barType;
		ar >> spacingType;

		m_eBarType = (ePointBarType)barType;
		m_eSpacingType = (eSpacingType)spacingType;
		ar >> m_StartOffset;
		ar >> m_EndOffset;

		m_LocationP0.Serialize(ar);
		m_LocationP1.Serialize(ar);
	}
}

bool MSCustomPointBarUnit::HasRebarLineID(long rebarLineID)
{
	if (m_LocationP0.HasRebarLineID(rebarLineID))	return true;
	if (m_LocationP1.HasRebarLineID(rebarLineID))	return true;
	return false;
}

GM2DLineVector MSCustomPointBarUnit::Get2DLineVector()
{
	MSCustomLineSection* pCustomLineSection = dynamic_cast<MSCustomLineSection*>(GetCustomSection());

	GM2DVector* pFirstPoint = pCustomLineSection->GetCrossPointByRebarLineIDs(GetCustomLocationP0().GetRebarLineID1(), GetCustomLocationP0().GetRebarLineID2());
	GM2DVector* pSecondPoint = pCustomLineSection->GetCrossPointByRebarLineIDs(GetCustomLocationP1().GetRebarLineID1(), GetCustomLocationP1().GetRebarLineID2());

	GM2DLineVector lineVec;
	lineVec.m_P0 = *pFirstPoint;
	lineVec.m_P1 = *pSecondPoint;

	lineVec.ExtendLineVector(TRUE, GetStartOffset(), TRUE);
	lineVec.ExtendLineVector(FALSE, GetEndOffset(), FALSE);

	delete pFirstPoint;
	delete pSecondPoint;
	return lineVec;
}

GM2DPolyline* MSCustomPointBarUnit::GetProfile()
{
	MSCustomLineSection* pCustomLineSection = dynamic_cast<MSCustomLineSection*>(GetCustomSection());

	GM2DVector* pFirstPoint = pCustomLineSection->GetCrossPointByRebarLineIDs(GetCustomLocationP0().GetRebarLineID1(), GetCustomLocationP0().GetRebarLineID2());
	GM2DVector* pSecondPoint = pCustomLineSection->GetCrossPointByRebarLineIDs(GetCustomLocationP1().GetRebarLineID1(), GetCustomLocationP1().GetRebarLineID2());

	GM2DLineVector lineVec;
	lineVec.m_P0 = *pFirstPoint;
	lineVec.m_P1 = *pSecondPoint;

	lineVec.ExtendLineVector(TRUE, GetStartOffset(), TRUE);
	lineVec.ExtendLineVector(FALSE, GetEndOffset(), FALSE);

	pFirstPoint->SetXY(lineVec.m_P0.m_X, lineVec.m_P0.m_Y);
	pSecondPoint->SetXY(lineVec.m_P1.m_X, lineVec.m_P1.m_Y);
	if (pFirstPoint != nullptr && pSecondPoint != nullptr)
	{
		GM2DPolyline* pProfile = new GM2DPolyline();
		pProfile->Add(pFirstPoint);
		pProfile->Add(pSecondPoint);
		pProfile->ma_Bulge.Add(0.0);
		return pProfile;
	}
	else
	{
		delete pFirstPoint;
		delete pSecondPoint;
		return nullptr;
	}
}

vector<CString> MSCustomPointBarUnit::GetBarTypeNameList(MSCustomSection::eSectionType eType)
{
	vector<CString> typeNameList;
	if (eType == MSCustomSection::CUSTOM_COLUMN || eType == MSCustomSection::CUSTOM_WALLCOLUMN)
	{
		typeNameList.push_back(LocalDic::GetLocal(L"주근", L"ExtraMethods"));
		typeNameList.push_back(LocalDic::GetLocal(L"보강근", L"ExtraMethods"));
	}
	else if (eType == MSCustomSection::CUSTOM_BEAM)
	{
		typeNameList.push_back(LocalDic::GetLocal(L"상부 주근", L"MSDBManager"));
		typeNameList.push_back(LocalDic::GetLocal(L"하부 주근", L"MSDBManager"));
		typeNameList.push_back(LocalDic::GetLocal(L"측면근", L"NXBuilder"));
	}
	return typeNameList;
}
vector<CString> MSCustomPointBarUnit::GetSpacingNameList()
{
	vector<CString> spacingNameList;
	spacingNameList.push_back(LocalDic::GetLocal(L"등분포", L"DrawingManager"));
	spacingNameList.push_back(LocalDic::GetLocal(L"등분포 - 유효간격", L"ExtraMethods"));
	return spacingNameList;
}

int MSCustomPointBarUnit::GetInputPointCount(ePointBarType barType)
{
	if (barType == ePointBarType::MainBar)
		return 2;
	else if (barType == ePointBarType::AdditionalBar)
		return 2;
	else if (barType == ePointBarType::TopMainBar)
		return 2;
	else if (barType == ePointBarType::BottomMainBar)
		return 2;
	else if (barType == ePointBarType::SideBar)
		return 2;
	return 0;
}

CString MSCustomPointBarUnit::GetDefaultPointRebar(MSCustomPointBarUnit::ePointBarType barType)
{
	CString sBar = _T("");
	CString sDefDiaName = MSDeformedBar::GetDefaultRebarName();
	sBar.Format(_T("4-%s%s"), MSDeformedBar::m_sDefaultBarType,sDefDiaName);

	return sBar;
}
long MSCustomPointBarUnit::GetEdgeIndex()
{
	map<int, int> countMap;
	int nP0_ID1 = GetCustomLocationP0().GetRebarLineID1();
	int nP0_ID2 = GetCustomLocationP0().GetRebarLineID2();
	int nP1_ID1 = GetCustomLocationP1().GetRebarLineID1();
	int nP1_ID2 = GetCustomLocationP1().GetRebarLineID2();

	int nID = 0;
	if(nP0_ID1 == nP1_ID1 || nP0_ID1 == nP1_ID2)
		nID = nP0_ID1;
	else
		nID = nP0_ID2;
	MSCustomReBarLine* pLine = GetCustomSection()->GetMSRebarLine(nID);		
	
	return pLine != NULL ? pLine->GetEdgeIndex() : -1;
}