#include "StdAfx.h"
#include "MSMarkG.h"
#include "GMLib/MSVersionInfo.h"
#include "MSMarkM.h"
#include "MSBaseBuilding.h"
#include "MSFloor.h"
#include "MSCompoG.h"

IMPLEMENT_SERIAL(MSMarkG, MSElement, VERSIONABLE_SCHEMA | 1)

MSMarkG::MSMarkG()
{
	Init(true);
}

MSMarkG::~MSMarkG(void)
{
}

void MSMarkG::Init(bool bInConstructor)
{
	if(!bInConstructor)
		MSObject::Init(bInConstructor);

	m_ID = 0;
	mp_Bldg = NULL;
	mp_MarkerM = NULL;
	m_FloorNum = 0;
	m_IsSendMark = false;
	mp_Cloud = new GM2DPolyline;
	m_nLevel = MESSAGE_LEVEL_INFO;
}


MSObject* MSMarkG::Copy( bool bNewID /*= true*/ )
{
	MSMarkG* pNewObj = new MSMarkG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSMarkG::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/ )
{
	if(pSource_in == nullptr) return;

	MSMarkG* pSource = (MSMarkG*)pSource_in;
	mp_Bldg = pSource->mp_Bldg;
	m_FloorNum = pSource->m_FloorNum;
	m_IsSendMark = pSource->m_IsSendMark;

	if(pSource->mp_MarkerM)
		mp_MarkerM = pSource->mp_MarkerM;

	if(pSource->mp_Cloud)
		mp_Cloud->CopyFrom(pSource->mp_Cloud);
	m_nLevel = pSource->m_nLevel;

	if(bNewID)
		GetID();
}

void MSMarkG::Dump(EFS::EFS_Buffer& buffer)
{
	MSElement::Dump(buffer);
	buffer << &mp_MarkerM;
	buffer << m_FloorNum;
	mp_Cloud->Dump(buffer);
	buffer << m_nLevel;
}

void MSMarkG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSElement::Recovery(buffer);
	buffer >> &mp_MarkerM;
	buffer >> m_FloorNum;
	mp_Cloud->Recovery(buffer);
	buffer >> m_nLevel;
}

void MSMarkG::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	int nMarkerMID  = 0;
	if(ar.IsStoring())
	{
		ar << m_ID;
		if(mp_MarkerM)
			nMarkerMID = mp_MarkerM->m_ID;
		ar << nMarkerMID;
		ar << m_FloorNum;

		mp_Cloud->Serialize(ar);

		ar << m_IsSendMark;
		ar << m_nLevel;
	}
	else 
	{
		ar >> m_ID;
		ar >> nMarkerMID;
		mp_MarkerM = mp_Bldg->GetMarkM(nMarkerMID);

		ar >> m_FloorNum;
		mp_Cloud->Serialize(ar);

		if(MSVersionInfo::GetCurrentVersion() >= 20150319)
		ar >> m_IsSendMark;

		if(MSVersionInfo::GetCurrentVersion() >= 20140729)
			SetLastID(m_ID);
		if(MSVersionInfo::GetCurrentVersion() >= 20150419)
			ar >> m_nLevel;
	}
}

GM2DPolyline* MSMarkG::GetCloud()
{
	return mp_Cloud;
}

void MSMarkG::Translate( GM2DVector &pDelta )
{
	MSElement::Translate(pDelta);
	mp_Cloud->Translate(pDelta);
}

void MSMarkG::RotateZ( double theta, GM2DVector *pCenter )
{
	MSElement::RotateZ(theta, pCenter);
	mp_Cloud->RotateZ(theta, pCenter);
}

void MSMarkG::MirrorByAxis(GM2DLineVector& pAxis)
{
	MSElement::MirrorByAxis(pAxis);
	mp_Cloud->MirrorByAxis(pAxis);
}
