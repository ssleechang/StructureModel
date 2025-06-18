#include "StdAfx.h"
#include "MSWell.h"
#include "MSFloor.h"
#include "MSSlabG.h"
#include "MSPolylineObject.h"
#include "EFS_Buffer.h"

IMPLEMENT_SERIAL(MSWell, MSElement, VERSIONABLE_SCHEMA | 1)

MSWell::MSWell(void)
{
	m_pOwner = NULL;
    mp_PolylineInfo = new MSPolylineObject();
	mp_PolylineInfo->SetOwner(this);
	m_Depth = 1000;
}

MSWell::~MSWell(void)
{
	if(mp_PolylineInfo != NULL)
		delete mp_PolylineInfo;
	mp_PolylineInfo = NULL;
}

MSObject* MSWell::Copy(bool bNewID /* = true*/)
{
	MSWell* pNewObj;

	pNewObj = new MSWell();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSWell::CopyFromMe(MSWell* pSource, bool bNewID /* = true*/)
{
	MSElement::CopyFromMe(pSource, bNewID);

	if(pSource)
	{
		m_pOwner = pSource->m_pOwner;
		m_Depth = pSource->m_Depth;
	}
}

void MSWell::Serialize(CArchive &ar)
{
	MSElement::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_Depth;
		mp_PolylineInfo->Serialize(ar);
	}
	else
	{
		ar >> m_Depth;
		mp_PolylineInfo->Serialize(ar);
		mp_PolylineInfo->SetOwner(this);
	}
}

void MSWell::Dump(EFS::EFS_Buffer& buffer)
{
	MSElement::Dump(buffer);
	mp_PolylineInfo->Dump(buffer);
	buffer << m_Depth;
}

void MSWell::Recovery(EFS::EFS_Buffer& buffer)
{
	MSElement::Recovery(buffer);
	mp_PolylineInfo->Recovery(buffer);
	mp_PolylineInfo->SetOwner(this);
	buffer >> m_Depth;
}

MSElement* MSWell::GetOwner()
{
	return m_pOwner;
}

void MSWell::SetOwner(MSElement* pElement)
{
	m_pOwner = pElement;
}

bool MSWell::MergeToMe( MSWell* Other )
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

MSFloor* MSWell::GetFloor()
{
	if(m_pOwner == NULL)
		return NULL;

	MSSlabG* pSlab = (MSSlabG*)m_pOwner;
	return pSlab->GetFloor();
}

CString MSWell::GetKey(bool bUseID)
{
	// ID, FloorNum, Type, CompoM, 좌표, 옵셋
	GM2DPolyline *pPoly = GetProfile();
	GM2DVector CenVec = pPoly->MassCenter();
	
	CString strCompoMKey;
	strCompoMKey.Format(_T("%d"), (int)m_Depth);
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