#include "StdAfx.h"
#include "MSRampG.h"
#include "MSFloor.h"
#include "MSBeamG.h"
#include "MSSlabG.h"


IMPLEMENT_SERIAL(MSRampG, MSPartCompoG, VERSIONABLE_SCHEMA | 1)
MSRampG::MSRampG()
{
	Init(true);
}

MSRampG::~MSRampG(void)
{
}

void MSRampG::Serialize(CArchive &ar)
{
	MSPartCompoG::Serialize(ar);

}

MSObject* MSRampG::Copy( bool bNewID /*= true*/ )
{
	MSRampG* pNewObj;

	pNewObj = new MSRampG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSRampG::CopyFromMe( MSRampG* pSource, bool bNewID/*=true*/)
{

}

void MSRampG::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSPartCompoG::Init(bInConstructor);

	mp_Surface = NULL;

	m_dHeight = 0.;

	m_pLBeamG = NULL;
	m_pRBeamG = NULL;
}

bool MSRampG::MakeBoundary()
{
	return false;
}