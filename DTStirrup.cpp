#include "StdAfx.h"
#include "DTStirrup.h"

long DTStirrup::ms_LastID = 0;
map<long,DTStirrup*> DTStirrup::ms_StirrupTempletMap;
DTStirrup::DTStirrup()
{
	Init(true);
}

DTStirrup::~DTStirrup(void)
{
}

void DTStirrup::Init( bool bInConstructor/*=false*/ )
{
	m_ID = 0;
	m_MemberType = 0;
	m_SectionType = 0;
	m_bFullStirrUp = false;
	m_StartAnchorType = AnchorType_End;
	m_EndAnchorType = AnchorType_End;
}

// m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
long DTStirrup::GetID()
{
	if(m_ID == 0) // 
	{
		m_ID = ms_LastID + 1;
		ms_LastID = m_ID;
		return m_ID;
	}

	return m_ID;
}

void DTStirrup::SetLastID( long NewID )
{
	if(NewID > ms_LastID)
		ms_LastID = NewID;
}
long DTStirrup::GetStirrupTempletArr( int MemberType, int SectionType, CTypedPtrArray<CObArray,DTStirrup*>& TempletArr )
{
	map<long,DTStirrup*>::iterator it;
	for(it = ms_StirrupTempletMap.begin();it!=ms_StirrupTempletMap.end();it++)
	{
		DTStirrup* pStirrup = it->second;
		if(pStirrup == NULL)	continue;
		if(pStirrup->m_MemberType == MemberType && pStirrup->m_SectionType == SectionType)
			TempletArr.Add(pStirrup);
	}
	return TempletArr.GetSize();	

}

DTStirrup* DTStirrup::GetStirrupTemplet( long nID )
{
	map<long,DTStirrup*>::iterator it;
	it = ms_StirrupTempletMap.find(nID);
	if(it != ms_StirrupTempletMap.end())
		return it->second;
	return NULL;
}

int DTStirrup::ReadStirrupTemplet()
{
	DTStirrup* pNewStirrupTemp = new DTStirrup();
	pNewStirrupTemp->m_bFullStirrUp = true;
	pNewStirrupTemp->m_StartAnchorType = AnchorType_Hook_90;
	pNewStirrupTemp->m_EndAnchorType = AnchorType_Hook_135;
	pNewStirrupTemp->GetID();
	AddStirrupTemplet(pNewStirrupTemp);

	pNewStirrupTemp = new DTStirrup();
	pNewStirrupTemp->m_bFullStirrUp = true;
	pNewStirrupTemp->m_StartAnchorType = AnchorType_Hook_90;
	pNewStirrupTemp->m_EndAnchorType = AnchorType_Hook_135;
	pNewStirrupTemp->GetID();
	AddStirrupTemplet(pNewStirrupTemp);

	return ms_StirrupTempletMap.size();

}

int DTStirrup::WriteStirrupTemplet()
{

	return 0;

}

void DTStirrup::AddStirrupTemplet( DTStirrup* pVal )
{
	ms_StirrupTempletMap.insert(make_pair(pVal->GetID(), pVal));
}

