#include "StdAfx.h"
#include "XMSRowM.h"

#include "MSRebarBuilding.h"
long XMSRowM::GetIncludeMIDbyFloorNum(long FloorNum)
{
	if(m_pOwner==NULL)	return 0;
	long NumMID = ma_MID.GetSize();
	for(long iMID=0; iMID<NumMID; iMID++)
	{
		long nMID = ma_MID[iMID];
		MSCompoM* pCompoM = m_pOwner->GetCompoM(nMID);
		if(pCompoM==NULL)	continue;
		if(pCompoM->m_nFromStory <= FloorNum && FloorNum <= pCompoM->m_nToStory)
			return nMID;
	}
	return 0;
}

CString XMSRowM::GetName()
{
	CString strName = _T("");
	if(m_pOwner==NULL)	return strName;
	MSCompoM* pCompoM = m_pOwner->GetCompoM(m_nRepresentMID);
	if(pCompoM)	return pCompoM->m_Name;

	long NumMID = ma_MID.GetSize();
	for(long iMID=0; iMID<NumMID; iMID++)
	{
		long nMID = ma_MID[iMID];
		MSCompoM* pCompoM = m_pOwner->GetCompoM(nMID);
		if(pCompoM)	return pCompoM->m_Name;
	}
	return strName;
}

BOOL XMSRowM::GetStory(long& nFrom, long& nTo)
{
	nFrom = nTo = 0;
	long nMinStory=0, nMaxStory=0;

	int nCount = 0;
	long NumMID = ma_MID.GetSize();
	for(long iMID=0; iMID<NumMID; iMID++)
	{
		long nMID = ma_MID[iMID];
		MSCompoM* pCompoM = m_pOwner->GetCompoM(nMID);
		if(pCompoM==NULL)	continue;
		if(nCount==0)
		{
			nMinStory = pCompoM->m_nFromStory;
			nMaxStory = pCompoM->m_nToStory;
			nCount++;
			continue;
		}
		if(nMinStory > pCompoM->m_nFromStory && pCompoM->m_nFromStory != 0)
			nMinStory = pCompoM->m_nFromStory;
		if(nMaxStory < pCompoM->m_nToStory && pCompoM->m_nToStory != 0)
			nMaxStory = pCompoM->m_nToStory;

		nCount++;
	}
	if(nMaxStory != 0 && nMinStory != 0)
	{
		nFrom = nMinStory;
		nTo = nMaxStory;
		return TRUE;
	}
	return FALSE;
}

void XMSRowM::Sort()
{
	long NumMID = ma_MID.GetSize();
	for(long iMID=0; iMID<NumMID; iMID++)
	{
		for(long jMID=iMID+1; jMID<NumMID; jMID++)
		{
			MSCompoM* iCompoM = m_pOwner->GetCompoM(ma_MID[iMID]);
			MSCompoM* jCompoM = m_pOwner->GetCompoM(ma_MID[jMID]);
			if(iCompoM==NULL || jCompoM==NULL)	continue;
			if(iCompoM->m_nFromStory > jCompoM->m_nFromStory)
			{
				long nOldMID = iCompoM->m_ID;
				ma_MID[iMID] = ma_MID[jMID];
				ma_MID[jMID]	= nOldMID;
			}
		}
	}
}

BOOL XMSRowM::IsIncludeMID(long nMID)
{
	long NumMID = ma_MID.GetSize();
	for(long iMID=0; iMID<NumMID; iMID++)
	{
		if(ma_MID[iMID]==nMID)	return TRUE;
	}
	return FALSE;
}