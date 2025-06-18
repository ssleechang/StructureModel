#include "StdAfx.h"
#include "MSSubMainBar.h"
#include "GMLib/GM2DVector.h"
#include "MSDeformedBar.h"

IMPLEMENT_SERIAL(MSSubMainBar, MSRcObject, VERSIONABLE_SCHEMA | 1)

MSSubMainBar::MSSubMainBar(void)
{
	CString sBar = MSDeformedBar::GetMainBarStr(4);
	m_MainBar.InitData(sBar);
}


MSSubMainBar::~MSSubMainBar(void)
{
	DeleteLocArr();
}

void MSSubMainBar::DeleteLocArr()
{
	int nLoc = ma_Loc.size();
	for(int iLoc =0; iLoc < nLoc; iLoc++)
		delete ma_Loc[iLoc];
	ma_Loc.clear();
}

void MSSubMainBar::CopyFromMe(MSSubMainBar* pSource)
{
	if(pSource==NULL)	return; 

	m_MainBar.CopyFromMe(&pSource->m_MainBar);

	DeleteLocArr();
	int nLoc = pSource->ma_Loc.size();
	for(int iLoc = 0; iLoc < nLoc; iLoc++)
	{
		GM2DVector* pUrVec = pSource->ma_Loc[iLoc];
		GM2DVector* pMyVec = new GM2DVector(*pUrVec);
		ma_Loc.push_back(pMyVec);
	}
}

void MSSubMainBar::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);

	m_MainBar.Dump(buffer);

	int nLoc = ma_Loc.size();
	buffer << nLoc;
	for(int iLoc = 0; iLoc < nLoc; iLoc++)
		ma_Loc[iLoc]->Dump(buffer);
}

void MSSubMainBar::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);

	m_MainBar.Recovery(buffer);
	int nLoc = 0;
	buffer >> nLoc;
	DeleteLocArr();
	for(int iLoc = 0; iLoc < nLoc; iLoc++)
	{
		GM2DVector* pLocVec = new GM2DVector();
		pLocVec->Recovery(buffer);
		ma_Loc.push_back(pLocVec);
	}
}

void MSSubMainBar::Serialize(CArchive &ar)
{
	MSRcObject::Serialize(ar);
	int nLoc = 0;
	if(ar.IsStoring())
	{
		m_MainBar.Serialize(ar);
		nLoc = ma_Loc.size();
		ar << nLoc;
		for(int iLoc = 0; iLoc < nLoc; iLoc++)
			ma_Loc[iLoc]->Serialize(ar);
	}
	else
	{
		m_MainBar.Serialize(ar);
		ar >> nLoc;
		for(int iLoc = 0; iLoc < nLoc; iLoc++)
		{
			GM2DVector* pLocVec = new GM2DVector;
			pLocVec->Serialize(ar);
			ma_Loc.push_back(pLocVec);
		}
	}
}