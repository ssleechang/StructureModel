#include "stdafx.h"
#include "MSLineRebar.h"

#include "MSAddRebarSetting.h"
#include "MSDeformedBar.h"

IMPLEMENT_SERIAL(MSLineRebar, MSObject, VERSIONABLE_SCHEMA | 1)
MSLineRebar::MSLineRebar()
{
	m_IsActivation = true;
	m_IsNumPlaceRebar = false;

	CString sBar = _T("");
	sBar = MSDeformedBar::GetStirrupBarStr(200);
	m_StirrupBars.InitData(sBar);
	sBar = MSDeformedBar::GetMainBarStr(4);
	m_MainBars.InitData(sBar);

	m_pStartSet = new MSAddRebarSetting();
	m_pEndSet = new MSAddRebarSetting();

	m_IsCutOpening = true;
}


MSLineRebar::~MSLineRebar()
{
	delete m_pStartSet;
	delete m_pEndSet;
}

void MSLineRebar::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;
	MSObject::CopyFromMe(pSource_in, bNewID);

	MSLineRebar* pSourceRebar = dynamic_cast<MSLineRebar*>(pSource_in);
	if (pSourceRebar == nullptr)	return;

	m_IsActivation = pSourceRebar->GetIsActivation();
	m_IsCutOpening = pSourceRebar->GetIsCutOpening();
	m_IsNumPlaceRebar = pSourceRebar->GetIsNumPlaceRebar();
	m_MainBars.CopyFromMe(&pSourceRebar->m_MainBars);
	m_StirrupBars.CopyFromMe(&pSourceRebar->m_StirrupBars);
	m_pStartSet->CopyFromMe(pSourceRebar->GetStartSetting());
	m_pEndSet->CopyFromMe(pSourceRebar->GetEndSetting());
}

void MSLineRebar::Dump(EFS::EFS_Buffer& ar)
{
	MSObject::Dump(ar);
	ar << m_IsActivation << m_IsCutOpening << m_IsNumPlaceRebar;
	m_MainBars.Dump(ar);
	m_StirrupBars.Dump(ar);
	m_pStartSet->Dump(ar);
	m_pEndSet->Dump(ar);
}

void MSLineRebar::Recovery(EFS::EFS_Buffer& ar)
{
	MSObject::Recovery(ar);
	ar >> m_IsActivation >> m_IsCutOpening >> m_IsNumPlaceRebar;
	m_MainBars.Recovery(ar);
	m_StirrupBars.Recovery(ar);
	m_pStartSet->Recovery(ar);
	m_pEndSet->Recovery(ar);
}

void MSLineRebar::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_IsActivation;
		ar << m_IsCutOpening;
		ar << m_IsNumPlaceRebar;
		m_MainBars.Serialize(ar);
		m_StirrupBars.Serialize(ar);
		m_pStartSet->Serialize(ar);
		m_pEndSet->Serialize(ar);
	}
	else
	{
		ar >> m_IsActivation;
		ar >> m_IsCutOpening;
		ar >> m_IsNumPlaceRebar;
		m_MainBars.Serialize(ar);
		m_StirrupBars.Serialize(ar);
		m_pStartSet->Serialize(ar);
		m_pEndSet->Serialize(ar);
	}
}
