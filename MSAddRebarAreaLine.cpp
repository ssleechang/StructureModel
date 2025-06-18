#include "stdafx.h"
#include "MSAddRebarAreaLine.h"

#include "MSLineRebar.h"

MSAddRebarAreaLine* MSAddRebarAreaLine::ms_AddAreaLine = new MSAddRebarAreaLine();
IMPLEMENT_SERIAL(MSAddRebarAreaLine, MSAddRebarBase, VERSIONABLE_SCHEMA | 1)

MSAddRebarAreaLine* MSAddRebarAreaLine::GetStaticAddAreaLine()
{
	return ms_AddAreaLine;
}
MSAddRebarAreaLine::MSAddRebarAreaLine() : MSAddRebarBase()
{
	m_pTopMainBar = new MSLineRebar();
	m_pTopSubBar = new MSLineRebar();
	m_pBotMainBar = new MSLineRebar();
	m_pBotSubBar = new MSLineRebar();
}


MSAddRebarAreaLine::~MSAddRebarAreaLine()
{
	delete m_pTopMainBar;
	delete m_pTopSubBar;
	delete m_pBotMainBar;
	delete m_pBotSubBar;
}

MSObject* MSAddRebarAreaLine::Copy(bool bNewID /* = true*/)
{
	MSAddRebarAreaLine* pNewObj = new MSAddRebarAreaLine();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSAddRebarAreaLine::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;
	MSAddRebarBase::CopyFromMe(pSource_in, bNewID);

	MSAddRebarAreaLine* pSource = dynamic_cast<MSAddRebarAreaLine*>(pSource_in);
	if (pSource == nullptr)		return;
	m_pTopMainBar->CopyFromMe(pSource->GetTopMainBar());
	m_pTopSubBar->CopyFromMe(pSource->GetTopSubBar());
	m_pBotMainBar->CopyFromMe(pSource->GetBotMainBar());
	m_pBotSubBar->CopyFromMe(pSource->GetBotSubBar());
}

void MSAddRebarAreaLine::Dump(EFS::EFS_Buffer& ar)
{
	MSAddRebarBase::Dump(ar);
	m_pTopMainBar->Dump(ar);
	m_pTopSubBar->Dump(ar);
	m_pBotMainBar->Dump(ar);
	m_pBotSubBar->Dump(ar);
}

void MSAddRebarAreaLine::Recovery(EFS::EFS_Buffer& ar)
{
	MSAddRebarBase::Recovery(ar);
	m_pTopMainBar->Recovery(ar);
	m_pTopSubBar->Recovery(ar);
	m_pBotMainBar->Recovery(ar);
	m_pBotSubBar->Recovery(ar);
}

void MSAddRebarAreaLine::Serialize(CArchive &ar)
{
	MSAddRebarBase::Serialize(ar);
	if (ar.IsStoring())
	{
		m_pTopMainBar->Serialize(ar);
		m_pTopSubBar->Serialize(ar);
		m_pBotMainBar->Serialize(ar);
		m_pBotSubBar->Serialize(ar);
	}
	else
	{
		m_pTopMainBar->Serialize(ar);
		m_pTopSubBar->Serialize(ar);
		m_pBotMainBar->Serialize(ar);
		m_pBotSubBar->Serialize(ar);
	}
}

