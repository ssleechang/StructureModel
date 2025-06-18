#include "stdafx.h"
#include "MSShapeParams.h"

#include "MSShapeLengthParams.h"
#include "MSShapeAngleParams.h"

IMPLEMENT_SERIAL(MSShapeParams, MSObject, VERSIONABLE_SCHEMA | 1)
MSShapeParams::MSShapeParams()
{
	m_GCode = 1;
	m_SCode = 0;
}


MSShapeParams::~MSShapeParams()
{
	DeleteParams();
}

void MSShapeParams::DeleteParams()
{
	for (int i = 0; i < ma_LengthParam.size(); i++)
		delete ma_LengthParam[i];
	ma_LengthParam.clear();

	for (int i = 0; i < ma_AngleParam.size(); i++)
		delete ma_AngleParam[i];
	ma_AngleParam.clear();
}

void MSShapeParams::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;
	MSObject::CopyFromMe(pSource_in, bNewID);

	MSShapeParams* pSource = dynamic_cast<MSShapeParams*>(pSource_in);
	if (pSource == nullptr)	return;

	m_GCode = pSource->GetGCode();
	m_SCode = pSource->GetSCode();
	DeleteParams();
	for (int i = 0; i < pSource->ma_LengthParam.size(); i++)
	{
		MSShapeLengthParams* pLengthParam = new MSShapeLengthParams();
		pLengthParam->CopyFromMe(pSource->ma_LengthParam[i]);
		ma_LengthParam.push_back(pLengthParam);
	}

	for (int i = 0; i < pSource->ma_AngleParam.size(); i++)
	{
		MSShapeAngleParams* pAngleParam = new MSShapeAngleParams();
		pAngleParam->CopyFromMe(pSource->ma_AngleParam[i]);
		ma_AngleParam.push_back(pAngleParam);
	}

}

void MSShapeParams::Dump(EFS::EFS_Buffer& ar)
{
	MSObject::Dump(ar);

	ar << m_GCode << m_SCode;
	int count = ma_LengthParam.size();
	ar << count;
	for (int i = 0; i < count; i++)
	{
		ma_LengthParam[i]->Dump(ar);
	}

	count = ma_AngleParam.size();
	ar << count;
	for (int i = 0; i < count; i++)
	{
		ma_AngleParam[i]->Dump(ar);
	}

}

void MSShapeParams::Recovery(EFS::EFS_Buffer& ar)
{
	MSObject::Recovery(ar);
	
	ar >> m_GCode >> m_SCode;
	DeleteParams();
	int count = 0;
	ar >> count;
	for (int i = 0; i < count; i++)
	{
		MSShapeLengthParams* pLengthParam = new MSShapeLengthParams();
		pLengthParam->Recovery(ar);
		ma_LengthParam.push_back(pLengthParam);
	}

	ar >> count;
	for (int i = 0; i < count; i++)
	{
		MSShapeAngleParams* pAngleParam = new MSShapeAngleParams();
		pAngleParam->Recovery(ar);
		ma_AngleParam.push_back(pAngleParam);
	}
}

void MSShapeParams::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	int count = 0;
	if (ar.IsStoring())
	{
		ar << m_GCode;
		ar << m_SCode;
		count = ma_LengthParam.size();
		ar << count;
		for (int i = 0; i < count; i++)
		{
			ma_LengthParam[i]->Serialize(ar);
		}

		count = ma_AngleParam.size();
		ar << count;
		for (int i = 0; i < count; i++)
		{
			ma_AngleParam[i]->Serialize(ar);
		}
	}
	else
	{
		ar >> m_GCode;
		ar >> m_SCode;
		ar >> count;
		for (int i = 0; i < count; i++)
		{
			ma_LengthParam[i]->Serialize(ar);
		}
		ar >> count;
		for (int i = 0; i < count; i++)
		{
			ma_AngleParam[i]->Serialize(ar);
		}
	}
}
