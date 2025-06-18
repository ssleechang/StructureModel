#include "StdAfx.h"
#include "MSSectionView.h"
#include "GMLib/GM2DVector.h"

#include "MSLineLevelDifG.h"
#include "MSFloor.h"
#include "MSFloorQT.h"
#include "MSPolylineObject.h"
#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSSectionView, MSElement, VERSIONABLE_SCHEMA | 1)

MSSectionView::MSSectionView(void)
{
    m_ID = 0;
    mp_Owner = NULL;
    mp_PolylineInfo = new MSPolylineObject();
	mp_PolylineInfo->SetOwner(this);
	mp_Source = NULL;

	m_nType = 0;
	m_bLeft = true;
	m_Name = _T("");
}

MSSectionView::~MSSectionView(void)
{
}

void MSSectionView::Serialize( CArchive& ar )
{
	MSElement::Serialize(ar);

	if(ar.IsStoring())
	{
		mp_PolylineInfo->Serialize(ar);
		ar << m_nType << m_bLeft;
		int count = m_PolyLine.size();
		ar << count;
		for (int i = 0; i < count; ++i)
			m_PolyLine[i].Serialize(ar);
		ar << m_Name;
	}
	else
	{
		mp_PolylineInfo->Serialize(ar);
		mp_PolylineInfo->SetOwner(this);
		ar >> m_nType >> m_bLeft;
		int count = 0;
		ar >> count;
		m_PolyLine.resize(count);
		for (int i = 0; i < count; ++i)
			m_PolyLine[i].Serialize(ar);
		ar >> m_Name;
	}
}

void MSSectionView::Dump(EFS::EFS_Buffer& buffer)
{
	MSElement::Dump(buffer);
	mp_PolylineInfo->Dump(buffer);
	buffer << m_nType << m_bLeft;
	int count = m_PolyLine.size();
	buffer << count;
	for (int i = 0; i < count; ++i)
		m_PolyLine[i].Dump(buffer);
	buffer << m_Name;
}

void MSSectionView::Recovery(EFS::EFS_Buffer& buffer)
{
	MSElement::Recovery(buffer);
	mp_PolylineInfo->Recovery(buffer);
	buffer >> m_nType >> m_bLeft;
	int count = 0;
	buffer >> count;
	m_PolyLine.resize(count);
	for (int i = 0; i < count; ++i)
		m_PolyLine[i].Recovery(buffer);
	buffer >> m_Name;
}

MSObject* MSSectionView::Copy( bool bNewID /*= true*/ )
{
	MSSectionView* pNewObj = new MSSectionView();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSSectionView::CopyFromMe(MSObject* pSource, bool bNewID/*=true*/ )
{
	if(pSource == nullptr) return;
	if( !pSource->IsElement() || ((MSElement*)pSource)->GetType() != msSectionView) { ASSERT(0); return; }
	MSSectionView* pSourceSV = dynamic_cast<MSSectionView*>(pSource);

	mp_Owner = pSourceSV->mp_Owner;
	MSElement::CopyFromMe(pSourceSV, bNewID);

	m_nType = pSourceSV->m_nType;
	m_bLeft = pSourceSV->m_bLeft;
	m_PolyLine = pSourceSV->m_PolyLine;
	m_Name = pSourceSV->m_Name;
}

bool MSSectionView::CheckData()
{
	return true;
}

long MSSectionView::Contains( double Xc, double Yc, double tolerance )
{
    if (this->mp_PolylineInfo == NULL)
        return -2;
    if (this->GetProfile() == NULL)
        return -2;

	return mp_PolylineInfo->Contains(Xc, Yc, tolerance);
}

bool MSSectionView::OverLap(GM2DCurve* pCurve, double tolerance)
{
	if (this->mp_PolylineInfo == NULL)
		return FALSE;
	if (this->GetProfile() == NULL)
		return FALSE;

	return mp_PolylineInfo->OverLap(pCurve, tolerance);
}

MSFloor* MSSectionView::GetFloor()
{
	MSFloor* pFloor = NULL;
	if(mp_Owner == NULL) 
	{
		ASSERT(0);
		return pFloor;
	}
	if(mp_Owner->GetType() == MSElement::msFloor)
		pFloor = (MSFloor*)mp_Owner;
	else
	{
		ASSERT(0);
		return pFloor;
	}
	return pFloor;
}

void MSSectionView::GetBoundary(GM2DPolyline& APolyLine)
{
	GM2DPolyline *pProfile = GetProfile();
	if(pProfile == NULL)
		return;
	APolyLine.CopyFrom(pProfile);
}

void MSSectionView::RecoverPointerAfterSerialize()
{
}

void MSSectionView::Translate( GM2DVector &pDelta )
{
	MSElement::Translate(pDelta);
	int nCount = m_PolyLine.size();
	for (int i = 0; i < nCount; i++)
	{
		m_PolyLine[i].Translate(pDelta);
	}

}

void MSSectionView::MirrorByAxis( GM2DLineVector& pAxis )
{
	MSElement::MirrorByAxis(pAxis);
	int nCount = m_PolyLine.size();
	for (int i = 0; i < nCount; i++)
	{
		m_PolyLine[i].MirrorByAxis(pAxis);
	}
}

void MSSectionView::RotateZ( double theta, GM2DVector *pCenter )
{
	MSElement::RotateZ(theta, pCenter);
	int nCount = m_PolyLine.size();
	for (int i = 0; i < nCount; i++)
	{
		m_PolyLine[i].RotateZ(theta, pCenter);
	}
}
