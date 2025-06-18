#include "stdafx.h"
#include "MSFWTrenchM.h"


IMPLEMENT_SERIAL(MSFWTrenchM, MSCompoM, VERSIONABLE_SCHEMA | 1)
MSFWTrenchM::MSFWTrenchM()
{
	m_nType = MSCompoM::msFWTrenchM;
	m_eTrenchType = MSFWTrenchM::General;
	m_W = 150;
	m_T = 50;
	m_pProfile = new GM2DPolyline();
	MakeProfile();
}

MSFWTrenchM::~MSFWTrenchM()
{
	delete m_pProfile;
}
void MSFWTrenchM::CopyFromMe(MSCompoM* pSource)
{
	if (pSource == NULL)	return;

	MSFWTrenchM* pSourceM = (MSFWTrenchM*)pSource;
	m_eTrenchType = pSourceM->GetFWTrenchType();
	m_W = pSourceM->GetW();
	m_T = pSourceM->GetT();

	MSCompoM::CopyFromMe(pSource);
	MakeProfile();
}

void MSFWTrenchM::Dump(EFS::EFS_Buffer& buffer)
{
	int type = (int)m_eTrenchType;
	buffer << type;
	buffer << m_W << m_T;
	MSCompoM::Dump(buffer);
	MakeProfile();
}

void MSFWTrenchM::Recovery(EFS::EFS_Buffer& buffer)
{
	int type = 0;
	buffer >> type;
	m_eTrenchType = (eFWTrenchType)type;
	buffer >> m_W >> m_T;
	MSCompoM::Recovery(buffer);
	MakeProfile();
}

void MSFWTrenchM::Serialize(CArchive &ar)
{
	MSCompoM::Serialize(ar);
	int type = 0;
	if (ar.IsStoring())
	{
		type = (int)m_eTrenchType;
		ar << type;
		ar << m_W;
		ar << m_T;
	}
	else
	{
		ar >> type;
		m_eTrenchType = (eFWTrenchType)type;
		ar >> m_W;
		ar >> m_T;
		MakeProfile();
	}
}

void MSFWTrenchM::SetW(double param)
{
	m_W = param;
	MakeProfile();
}
void MSFWTrenchM::SetT(double param)
{
	m_T = param;
	MakeProfile();
}

bool MSFWTrenchM::CheckProfile()
{
	switch (m_eTrenchType) {
		case MSFWTrenchM::General: 
			return CheckProfile_General();
		default:
			return true;
	}
}

bool MSFWTrenchM::CheckProfile_General()
{
	return true;
}

void MSFWTrenchM::MakeProfile()
{
	m_pProfile->ResetPolyline();
	switch (m_eTrenchType)
	{
	case MSFWTrenchM::General:	return makeProfile_General();
	}
}
void MSFWTrenchM::makeProfile_General()
{
	GM2DVector* leftBottom = new GM2DVector(-m_W / 2, -m_T / 2);
	GM2DVector* rightBottom = new GM2DVector(m_W / 2, -m_T / 2);
	GM2DVector* rightTop = new GM2DVector(m_W / 2, m_T / 2);
	GM2DVector* leftTop = new GM2DVector(-m_W / 2, m_T / 2);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);
	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);
	m_pProfile->Add(leftTop);
	m_pProfile->MakeClosedPolyline(tolerance);
}

CString MSFWTrenchM::ToShapeName(eFWTrenchType eType)
{
	switch (eType)
	{
	case eFWTrenchType::General:
		return LocalDic::GetLocal(_T("일반 배수로"));
		break;
	default:
		break;
	}

	return LocalDic::GetLocal(_T("사용자 정의 배수로"));
}

CString MSFWTrenchM::GetShapeName()
{
	return ToShapeName(m_eTrenchType);
}

void MSFWTrenchM::SetWidth(double dWidth)
{
	SetW(dWidth);
}

void MSFWTrenchM::SetDepth(double dDepth)
{
	SetT(dDepth);
}
void MSFWTrenchM::SetThick(double dThick)
{
	SetT(dThick);
}
