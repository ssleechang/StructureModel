#include "stdafx.h"
#include "MSFWSoundAbsorptionM.h"


IMPLEMENT_SERIAL(MSFWSoundAbsorptionM, MSCompoM, VERSIONABLE_SCHEMA | 1)
MSFWSoundAbsorptionM::MSFWSoundAbsorptionM()
{
	m_nType = MSCompoM::msFWSoundAbsorptionM;
	m_eSoundAbsorptionType = MSFWSoundAbsorptionM::General;
	m_T = 30;
	m_pProfile = new GM2DPolyline();
	MakeProfile();
}

MSFWSoundAbsorptionM::~MSFWSoundAbsorptionM()
{
	delete m_pProfile;
}
void MSFWSoundAbsorptionM::CopyFromMe(MSCompoM* pSource)
{
	if (pSource == NULL)	return;

	MSFWSoundAbsorptionM* pSourceM = (MSFWSoundAbsorptionM*)pSource;
	m_eSoundAbsorptionType = pSourceM->GetFWSoundAbsorptionType();
	m_T = pSourceM->GetT();

	MSCompoM::CopyFromMe(pSource);
	MakeProfile();
}

void MSFWSoundAbsorptionM::Dump(EFS::EFS_Buffer& buffer)
{
	int type = (int)m_eSoundAbsorptionType;
	buffer << type;
	buffer << m_T;
	MSCompoM::Dump(buffer);
	MakeProfile();
}

void MSFWSoundAbsorptionM::Recovery(EFS::EFS_Buffer& buffer)
{
	int type = 0;
	buffer >> type;
	m_eSoundAbsorptionType = (eFWSoundAbsorptionType)type;
	buffer >> m_T;
	MSCompoM::Recovery(buffer);
	MakeProfile();
}

void MSFWSoundAbsorptionM::Serialize(CArchive &ar)
{
	MSCompoM::Serialize(ar);
	int type = 0;
	if (ar.IsStoring())
	{
		type = (int)m_eSoundAbsorptionType;
		ar << type;
		ar << m_T;
	}
	else
	{
		ar >> type;
		m_eSoundAbsorptionType = (eFWSoundAbsorptionType)type;
		ar >> m_T;
		MakeProfile();
	}
}

void MSFWSoundAbsorptionM::SetT(double param)
{
	m_T = param;
	MakeProfile();
}

void MSFWSoundAbsorptionM::MakeProfile()
{
	m_pProfile->ResetPolyline();
	switch (m_eSoundAbsorptionType)
	{
	case MSFWSoundAbsorptionM::General:	return makeProfile_General();
	}
}
void MSFWSoundAbsorptionM::makeProfile_General()
{
	double Length = m_T * 5;
	GM2DVector* leftBottom = new GM2DVector(-Length / 2, -m_T/2);
	GM2DVector* rightBottom = new GM2DVector(Length / 2, -m_T/2);
	GM2DVector* rightTop = new GM2DVector(Length / 2, m_T/2);
	GM2DVector* leftTop = new GM2DVector(-Length / 2, m_T/2);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);
	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);
	m_pProfile->Add(leftTop);
	m_pProfile->MakeClosedPolyline(tolerance);
}


CString MSFWSoundAbsorptionM::ToShapeName(eFWSoundAbsorptionType eType)
{
	switch (eType)
	{
	case eFWSoundAbsorptionType::General:
		return LocalDic::GetLocal(_T("흡음재"));
	default:
		break;
	}

	return LocalDic::GetLocal(_T("사용자 정의 흡음재"));
}

CString MSFWSoundAbsorptionM::GetShapeName()
{
	return ToShapeName(m_eSoundAbsorptionType);
}