#include "StdAfx.h"
#include "MSFloorBlock.h"

IMPLEMENT_SERIAL(MSFloorBlock, MSFloor, VERSIONABLE_SCHEMA | 1)

MSFloorBlock::MSFloorBlock( double dNodeTol/*=100.*/, double dDegTol/*=1.*/ ) : MSFloor(dNodeTol, dDegTol)
{
	Init(true);
}

MSFloorBlock::~MSFloorBlock(void)
{
}

MSObject* MSFloorBlock::Copy(bool bNewID /*= true*/)
{
	MSFloorBlock* pNewObj = new MSFloorBlock();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSFloorBlock::CopyFromMe(MSFloorBlock* pSource, bool bNewID/*=true*/)
{
	if (pSource == NULL) return;
	m_SiteRect.CopyFrom(&(pSource->m_SiteRect));

	MSLevelZone* pNewZone;
	long nZone = pSource->ma_LevelZone.size();
	for (long iZone = 0; iZone < nZone; iZone++)
	{
		MSLevelZone* pZone = pSource->ma_LevelZone[iZone];
		pNewZone = (MSLevelZone*)pZone->Copy();
		pNewZone->mp_Source = pZone;
		pZone->ma_Clone.push_back(pNewZone);
		Add(pNewZone);
	}

	MSConnectivity* pNewConn;
	map<long, MSConnectivity*>::iterator itConn;
	for (itConn = pSource->mm_Joint.begin(); itConn != pSource->mm_Joint.end(); itConn++)
	{
		MSConnectivity* pConn = itConn->second;
		pNewConn = (MSConnectivity*)pConn->Copy();
		pNewConn->mp_Source = pConn;
		pNewConn->m_nFloorNum = m_nFloorNum;
		pConn->ma_Clone.push_back(pNewConn);
		Add(pNewConn);
	}

	MSCompoG* pNewMem;
	map<long, MSCompoG*>::iterator itMem;
	for (itMem = pSource->mm_CompoG.begin(); itMem != pSource->mm_CompoG.end(); itMem++)
	{
		MSCompoG* pMem = itMem->second;
		pNewMem = (MSCompoG*)pMem->Copy();
		pNewMem->mp_Source = pMem;
		pMem->ma_Clone.push_back(pNewMem);
		Add(pNewMem, true);
	}

	for (itConn = mm_Joint.begin(); itConn != mm_Joint.end(); itConn++)
	{
		MSConnectivity* pConn = itConn->second;
		pConn->TrimMembersEx();
	}
}

void MSFloorBlock::Init(bool bInConstructor/*=false*/)
{
	if (!bInConstructor)
		MSFloor::Init(bInConstructor);
}

void MSFloorBlock::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{

	}
	else
	{
	}
    MSAssembly::Serialize(ar);
}

