#include "stdafx.h"
#include "MSShapeLengthParams.h"


IMPLEMENT_SERIAL(MSShapeLengthParams, MSObject, VERSIONABLE_SCHEMA | 1)
MSShapeLengthParams::MSShapeLengthParams()
{
	m_eParamType = eParamType::Hook;
	m_UserLength = 100;
}

MSShapeLengthParams::~MSShapeLengthParams()
{
}

void MSShapeLengthParams::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;
	MSObject::CopyFromMe(pSource_in, bNewID);

	MSShapeLengthParams* pSource = dynamic_cast<MSShapeLengthParams*>(pSource_in);
	if (pSource == nullptr)	return;

	m_eParamType = pSource->GetParamType();
	m_UserLength = pSource->GetUserLength();
}

void MSShapeLengthParams::Dump(EFS::EFS_Buffer& ar)
{
	MSObject::Dump(ar);
	int paramType = (int)m_eParamType;
	ar << paramType << m_UserLength;
}

void MSShapeLengthParams::Recovery(EFS::EFS_Buffer& ar)
{
	MSObject::Recovery(ar);
	int paramType = 0;
	ar >> paramType >> m_UserLength;

	m_eParamType = (eParamType)paramType;
}

void MSShapeLengthParams::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	int paramType = 0;
	if (ar.IsStoring())
	{
		paramType = (int)m_eParamType;
		ar << paramType;
		ar << m_UserLength;
	}
	else
	{
		ar >> paramType;
		ar >> m_UserLength;

		m_eParamType = (eParamType)paramType;
	}
}