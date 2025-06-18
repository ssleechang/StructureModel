#include "stdafx.h"
#include "MSFWOpeningM.h"


IMPLEMENT_SERIAL(MSFWOpeningM, MSOpeningM, VERSIONABLE_SCHEMA | 1)
MSFWOpeningM::MSFWOpeningM()
{
	m_nType = MSCompoM::msFWOpeningM;
	m_eOpeningType = MSFWOpeningM::RECT;
	m_dWidth = 500;
	m_dHeight = 500;
	m_pProfile = new GM2DPolyline();
	MakeProfile();
}


MSFWOpeningM::~MSFWOpeningM()
{
	delete m_pProfile;
}
void MSFWOpeningM::CopyFromMe(MSCompoM* pSource)
{
	if (pSource == NULL)	return;

	MSFWOpeningM* pSourceM = (MSFWOpeningM*)pSource;
	m_eOpeningType = pSourceM->GetFWOpeningType();
	MSOpeningM::CopyFromMe(pSource);
	MakeProfile();
}

void MSFWOpeningM::Dump(EFS::EFS_Buffer& buffer)
{
	int type = (int)m_eOpeningType;
	buffer << type;
	MSOpeningM::Dump(buffer);
	MakeProfile();
}

void MSFWOpeningM::Recovery(EFS::EFS_Buffer& buffer)
{
	int type = 0;
	buffer >> type;
	m_eOpeningType = (eFWOpeningType)type;
	MSOpeningM::Recovery(buffer);
	MakeProfile();
}

void MSFWOpeningM::Serialize(CArchive &ar)
{
	MSOpeningM::Serialize(ar);
	int type = 0;
	if (ar.IsStoring())
	{
		type = (int)m_eOpeningType;
		ar << type;
	}
	else
	{
		ar >> type;
		m_eOpeningType = (eFWOpeningType)type;
		MakeProfile();
	}
}

void MSFWOpeningM::SetWidth(double width)
{
	MSOpeningM::SetWidth(width);
	MakeProfile();
}
void MSFWOpeningM::SetHeight(double height)
{
	MSOpeningM::SetHeight(height);
	MakeProfile();
}

void MSFWOpeningM::MakeProfile()
{
	m_pProfile->ResetPolyline();
	switch (m_eOpeningType)
	{
	case MSFWOpeningM::RECT:	return makeProfile_Rect();
	case MSFWOpeningM::CIRCLE:	return makeProfile_Circle();
	}
}
void MSFWOpeningM::makeProfile_Rect()
{
	GM2DVector* leftBottom = new GM2DVector(-m_dWidth / 2, -m_dHeight / 2);
	GM2DVector* rightBottom = new GM2DVector(m_dWidth / 2, -m_dHeight / 2);
	GM2DVector* rightTop = new GM2DVector(m_dWidth / 2, m_dHeight / 2);
	GM2DVector* leftTop = new GM2DVector(-m_dWidth / 2, m_dHeight / 2);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);
	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);
	m_pProfile->Add(leftTop);
	m_pProfile->MakeClosedPolyline(tolerance);
}
void MSFWOpeningM::makeProfile_Circle()
{
	int resolution = 24;
	GM2DVector Pnt;
	GMCircle SecCircle(GetWidth() / 2., 0., 0.);
	double RadStep = 2.0*PI / resolution;
	for (long PntNum = 0; PntNum < resolution; PntNum++)
	{
		SecCircle.GetGlobalXY(PntNum*RadStep, Pnt.m_X, Pnt.m_Y);
		m_pProfile->AddPointAndBulge((GM2DVector*)Pnt.Copy());
	}

	double tolerance = 1.0;
	m_pProfile->MakeClosedPolyline(tolerance);
}

CString MSFWOpeningM::ToShapeName(eFWOpeningType eType)
{
	switch (eType)
	{
	case eFWOpeningType::CIRCLE:
		return LocalDic::GetLocal(_T("원형 오프닝"));
	case eFWOpeningType::RECT:
		return LocalDic::GetLocal(_T("사각 오프닝"));
	default:
		break;
	}

	return LocalDic::GetLocal(_T("사용자 정의 오프닝"));
}

CString MSFWOpeningM::GetShapeName()
{
	return ToShapeName(m_eOpeningType);
}