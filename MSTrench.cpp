#include "StdAfx.h"
#include "MSTrench.h"
#include "MSPolylineObject.h"
#include "EFS_Buffer.h"
#include "MSFloor.h"
#include "MSSlabG.h"
#include "MSLinkMemberG.h"
#include "MSBeamG.h"
#include "Radfcore_CLI/AppGlobalOptionWrapper.h"

IMPLEMENT_SERIAL(MSTrench, MSElement, VERSIONABLE_SCHEMA | 1)

MSTrench::MSTrench(void)
{
	m_pOwner = NULL;
    mp_PolylineInfo = new MSPolylineObject();
	mp_PolylineInfo->SetOwner(this);
	m_pCenterPolyLine = new GM2DPolyline;
	m_Width = 200;
	m_Depth = 100;
	m_nType = 0;
	ma_MergedTrenchID.push_back(GetID());
}

MSTrench::~MSTrench(void)
{
	if(mp_PolylineInfo != NULL)
		delete mp_PolylineInfo;
	mp_PolylineInfo = NULL;
	if(m_pCenterPolyLine)
		delete m_pCenterPolyLine;
}

MSObject* MSTrench::Copy(bool bNewID /* = true*/)
{
	MSTrench* pNewObj;

	pNewObj = new MSTrench();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSTrench::CopyFromMe(MSTrench* pSource, bool bNewID /* = true*/)
{
	MSElement::CopyFromMe(pSource, bNewID);

	if(pSource)
	{
		m_pOwner = pSource->m_pOwner;
		m_Depth = pSource->m_Depth;
		m_Width = pSource->m_Width;
		m_nType = pSource->m_nType;
		m_pCenterPolyLine->CopyFrom(pSource->m_pCenterPolyLine);
	}
}

void MSTrench::Serialize(CArchive &ar)
{
	MSElement::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_Width;
		ar << m_Depth;
		mp_PolylineInfo->Serialize(ar);
		m_pCenterPolyLine->Serialize(ar);
	}
	else
	{
		if (IsMVAddToBHQ())
		{
			ar >> m_Width;
			ar >> m_Depth;
		}
		else
		{
			int width = 200;
			int depth = 100;
			ar >> width;
			ar >> depth;
			m_Width = width;
			m_Depth = depth;
		}

		mp_PolylineInfo->Serialize(ar);
		mp_PolylineInfo->SetOwner(this);
		m_pCenterPolyLine->Serialize(ar);
	}
}

void MSTrench::Dump(EFS::EFS_Buffer& buffer)
{
	MSElement::Dump(buffer);
	mp_PolylineInfo->Dump(buffer);
	m_pCenterPolyLine->Dump(buffer);
	buffer << m_Depth;
	buffer << m_Width;
	buffer << m_nType;
}

void MSTrench::Recovery(EFS::EFS_Buffer& buffer)
{
	MSElement::Recovery(buffer);
	mp_PolylineInfo->Recovery(buffer);
	mp_PolylineInfo->SetOwner(this);
	m_pCenterPolyLine->Recovery(buffer);
	buffer >> m_Depth;
	buffer >> m_Width;
	buffer >> m_nType;
}

GM2DPolyline* MSTrench::GetCenterPolyline()
{
	return m_pCenterPolyLine;
}
MSElement* MSTrench::GetOwner()
{
	return m_pOwner;
}

void MSTrench::SetOwner(MSElement* pElement)
{
	m_pOwner = pElement;
}

bool MSTrench::MergeToMe( MSTrench* Other )
{
	if(Other == this)
		return false;
	if(abs(m_Depth - Other->m_Depth) > DTOL_GM)
		return false;

	GM2DPolyline* myPoly = this->GetProfile();
	GM2DPolyline* urPoly = Other->GetProfile();

	if(!myPoly->OverlapEx(*urPoly, DTOL_GM, FALSE))
		return false;

	CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;
	myPoly->BoolOperation('U', urPoly, JoinedPolys, DTOL_GM);

	int count = JoinedPolys.GetSize();
	if(count != 1)
		return false;

	myPoly->CopyFrom(JoinedPolys[0]);

	return true;
}

MSTrench* MSTrench::Merge(MSTrench* pMyTrench, MSTrench* pUrTrench)
{
	MSTrench* pNewTrench = NULL;

	if(pMyTrench == pUrTrench)
		return pNewTrench;
	if(abs(pMyTrench->m_Depth - pUrTrench->m_Depth) > DTOL_GM)
		return pNewTrench;

	GM2DPolyline* myPoly = pMyTrench->GetProfile();
	GM2DPolyline* urPoly = pUrTrench->GetProfile();

	CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;
	myPoly->BoolOperation('U', urPoly, JoinedPolys, DTOL_GM);

	int count = JoinedPolys.GetSize();
	if(count != 1)
		return pNewTrench;

	pNewTrench = (MSTrench*)pMyTrench->Copy();
	GM2DPolyline* pNewPoly = pNewTrench->GetProfile();

	pNewTrench->GetID();
	pNewPoly->CopyFrom(JoinedPolys[0]);
	pNewTrench->m_nType = 1;
	pMyTrench->m_nType = 2;
	pUrTrench->m_nType = 2;

	long NumID = pMyTrench->ma_MergedTrenchID.size();
	for(long i = 0 ; i < NumID ; i++)
		pNewTrench->ma_MergedTrenchID.push_back(pMyTrench->ma_MergedTrenchID[i]);
	NumID = pUrTrench->ma_MergedTrenchID.size();
	for(long i = 0 ; i < NumID ; i++)
		pNewTrench->ma_MergedTrenchID.push_back(pUrTrench->ma_MergedTrenchID[i]);

	return pNewTrench;

}

MSFloor* MSTrench::GetFloor()
{
	if(m_pOwner == NULL)
		return NULL;

	MSSlabG* pSlab = (MSSlabG*)m_pOwner;
	return pSlab->GetFloor();
}

void MSTrench::Translate( GM2DVector &pDelta )
{
	MSElement::Translate(pDelta);
	if (m_pCenterPolyLine != NULL)
		m_pCenterPolyLine->Translate(pDelta);
}

void MSTrench::MirrorByAxis( GM2DLineVector& pAxis )
{
	MSElement::MirrorByAxis(pAxis);
	if (m_pCenterPolyLine != NULL)
		m_pCenterPolyLine->MirrorByAxis(pAxis);
}

void MSTrench::RotateZ( double theta, GM2DVector *pCenter )
{
	MSElement::RotateZ(theta, pCenter);
	if (m_pCenterPolyLine != NULL)
		m_pCenterPolyLine->RotateZ(theta, pCenter);
}

CString MSTrench::GetKey(bool bUseID)
{
	// ID, FloorNum, Type, CompoM, 좌표, 옵셋
	GM2DPolyline *pPoly = GetProfile();
	GM2DVector CenVec = pPoly->MassCenter();

	CString strCompoMKey;
	strCompoMKey.Format(_T("%d_%d"), (int)m_Width, (int)m_Depth);
	CString strGeo;
	strGeo.Format(_T("%d_%d_%d_%d_%d"), pPoly->GetNumLines(), (int)pPoly->GetLength(), (int)pPoly->Area(), (int)CenVec.m_X, (int)CenVec.m_Y);
	CString strOffSet;
	//strOffSet.Format(_T("%d_%d_%d"), (int)IsSlopeMember(), m_SOffsetZ, m_EOffsetZ);

	CString strKey;
	strKey.Format(_T("%d_%d_%s_%s_%s"), GetFloorNum(), (int)GetType(), strCompoMKey, strGeo, strOffSet);
	if (bUseID)
	{
		CString strID;
		strID.Format(_T("%d_"), m_ID);
		strKey = strID + strKey;
	}

	return strKey;
}