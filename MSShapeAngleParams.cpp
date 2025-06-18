#include "stdafx.h"
#include "MSShapeAngleParams.h"


IMPLEMENT_SERIAL(MSShapeAngleParams, MSObject, VERSIONABLE_SCHEMA | 1)
MSShapeAngleParams::MSShapeAngleParams()
{
	m_eAngleType = eAngleType::Angle135;
	m_UserAngle = 135;
}

MSShapeAngleParams::~MSShapeAngleParams()
{
}

void MSShapeAngleParams::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;
	MSObject::CopyFromMe(pSource_in, bNewID);

	MSShapeAngleParams* pSource = dynamic_cast<MSShapeAngleParams*>(pSource_in);
	if (pSource == nullptr)	return;

	m_eAngleType = pSource->GetAngleType();
	m_UserAngle = pSource->GetUserAngle();
}

void MSShapeAngleParams::Dump(EFS::EFS_Buffer& ar)
{
	MSObject::Dump(ar);
	int angleType = (int)m_eAngleType;
	ar << angleType << m_UserAngle;
}

void MSShapeAngleParams::Recovery(EFS::EFS_Buffer& ar)
{
	MSObject::Recovery(ar);
	int angleType = 0;
	ar >> angleType >> m_UserAngle;

	m_eAngleType = (eAngleType)angleType;
}

void MSShapeAngleParams::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	int angleType = 0;
	if (ar.IsStoring())
	{
		angleType = (int)m_eAngleType;
		ar << angleType;
		ar << m_UserAngle;
	}
	else
	{
		ar >> angleType;
		ar >> m_UserAngle;

		m_eAngleType = (eAngleType)angleType;
	}
}
