#include "stdafx.h"
#include "MSAddRebarAreaBent.h"

#include "MSBentRebar.h"

MSAddRebarAreaBent* MSAddRebarAreaBent::ms_AddAreaBent= new MSAddRebarAreaBent();
MSAddRebarAreaBent* MSAddRebarAreaBent::GetStaticAddAreaBent()
{
	return ms_AddAreaBent;
}
IMPLEMENT_SERIAL(MSAddRebarAreaBent, MSAddRebarBase, VERSIONABLE_SCHEMA | 1)
MSAddRebarAreaBent::MSAddRebarAreaBent() : MSAddRebarBase()
{
	m_pMainBar = new MSBentRebar();
	m_pSubBar = new MSBentRebar();

	m_pMainBar->SetOwner(this);
	m_pSubBar->SetOwner(this);
}


MSAddRebarAreaBent::~MSAddRebarAreaBent()
{
	delete m_pMainBar;
	delete m_pSubBar;
}

MSObject* MSAddRebarAreaBent::Copy(bool bNewID /* = true*/)
{
	MSAddRebarAreaBent* pNewObj = new MSAddRebarAreaBent();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSAddRebarAreaBent::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;
	MSAddRebarBase::CopyFromMe(pSource_in, bNewID);

	MSAddRebarAreaBent* pSource = dynamic_cast<MSAddRebarAreaBent*>(pSource_in);
	if (pSource == nullptr)		return;

	m_pMainBar->CopyFromMe(pSource->GetMainBar());
	m_pSubBar->CopyFromMe(pSource->GetSubBar());
}

void MSAddRebarAreaBent::Dump(EFS::EFS_Buffer& ar)
{
	MSAddRebarBase::Dump(ar);
	m_pMainBar->Dump(ar);
	m_pSubBar->Dump(ar);
}

void MSAddRebarAreaBent::Recovery(EFS::EFS_Buffer& ar)
{
	MSAddRebarBase::Recovery(ar);
	m_pMainBar->Recovery(ar);
	m_pSubBar->Recovery(ar);
// 	CalculateBentLine(m_pMainBar);
// 	CalculateBentLine(m_pSubBar);
}

void MSAddRebarAreaBent::Serialize(CArchive &ar)
{
	MSAddRebarBase::Serialize(ar);
	if (ar.IsStoring())
	{
		m_pMainBar->Serialize(ar);
		m_pSubBar->Serialize(ar);
	}
	else
	{
		m_pMainBar->Serialize(ar);
		m_pSubBar->Serialize(ar);
	}
}

void MSAddRebarAreaBent::CalculateBentLine(MSBentRebar* pMSBentRebar)
{
	pMSBentRebar->CalculateBentLine();
}

bool MSAddRebarAreaBent::CheckBentLocation(GM2DPolyline* profile)
{
	bool IsOK = true;
	if (m_pMainBar->GetIsActivation())	IsOK = m_pMainBar->CheckBentLocation(profile);
	if (IsOK == false)	return false;

	if (m_pSubBar->GetIsActivation())
		return m_pSubBar->CheckBentLocation(profile);
	return true;
}