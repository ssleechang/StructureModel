#include "stdafx.h"
#include "MSFWSwitchBoxM.h"


IMPLEMENT_SERIAL(MSFWSwitchBoxM, MSCompoM, VERSIONABLE_SCHEMA | 1)
MSFWSwitchBoxM::MSFWSwitchBoxM()
{
	m_nType = MSCompoM::msFWSwitchBoxM;
	m_eSwitchType = MSFWSwitchBoxM::General;
	m_W = 500;
	m_H = 500;
	m_L = 25;
	m_pProfile = new GM2DPolyline();
	MakeProfile();
}

MSFWSwitchBoxM::~MSFWSwitchBoxM()
{
	delete m_pProfile;
}
void MSFWSwitchBoxM::CopyFromMe(MSCompoM* pSource)
{
	if (pSource == NULL)	return;

	MSFWSwitchBoxM* pSourceM = (MSFWSwitchBoxM*)pSource;
	m_eSwitchType = pSourceM->GetFWSwitchBoxType();
	m_W = pSourceM->GetW();
	m_H = pSourceM->GetH();
	m_L = pSourceM->GetL();

	MSCompoM::CopyFromMe(pSource);
	MakeProfile();
}

void MSFWSwitchBoxM::Dump(EFS::EFS_Buffer& buffer)
{
	int type = (int)m_eSwitchType;
	buffer << type;
	buffer << m_W << m_H << m_L;
	MSCompoM::Dump(buffer);
	MakeProfile();
}

void MSFWSwitchBoxM::Recovery(EFS::EFS_Buffer& buffer)
{
	int type = 0;
	buffer >> type;
	m_eSwitchType = (eFWSwitchBoxType)type;
	buffer >> m_W >> m_H >> m_L;
	MSCompoM::Recovery(buffer);
	MakeProfile();
}

void MSFWSwitchBoxM::Serialize(CArchive &ar)
{
	MSCompoM::Serialize(ar);
	int type = 0;
	if (ar.IsStoring())
	{
		type = (int)m_eSwitchType;
		ar << type;
		ar << m_W;
		ar << m_H;
		ar << m_L;
	}
	else
	{
		ar >> type;
		m_eSwitchType = (eFWSwitchBoxType)type;
		ar >> m_W;
		ar >> m_H;
		ar >> m_L;
		MakeProfile();
	}
}

void MSFWSwitchBoxM::SetW(double param)
{
	m_W = param;
	MakeProfile();
}
void MSFWSwitchBoxM::SetH(double param)
{
	m_H = param;
	MakeProfile();
}
void MSFWSwitchBoxM::SetL(double param)
{
	m_L = param;
}

bool MSFWSwitchBoxM::CheckProfile()
{
	switch (m_eSwitchType) {
		case MSFWSwitchBoxM::General: 
			return CheckProfile_General();
		default:
			return true;
	}
}

bool MSFWSwitchBoxM::CheckProfile_General()
{
	if (m_W / 2 <= m_L)
	{
		return false;
	}
	if (m_H / 2 <= m_L)
	{
		return false;
	}
	return true;
}

void MSFWSwitchBoxM::MakeProfile()
{
	m_pProfile->ResetPolyline();
	switch (m_eSwitchType)
	{
		case MSFWSwitchBoxM::General:	return makeProfile_General();
	}
}
void MSFWSwitchBoxM::makeProfile_General()
{
	GM2DVector* leftBottom = new GM2DVector(-m_W / 2, -m_H / 2);
	GM2DVector* rightBottom = new GM2DVector(m_W / 2, -m_H / 2);
	GM2DVector* rightTop = new GM2DVector(m_W / 2, m_H / 2);
	GM2DVector* leftTop = new GM2DVector(-m_W / 2, m_H / 2);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);
	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);
	m_pProfile->Add(leftTop);
	m_pProfile->MakeClosedPolyline(tolerance);
}

CString MSFWSwitchBoxM::ToShapeName(eFWSwitchBoxType eType)
{
	switch (eType)
	{
	case eFWSwitchBoxType::General:
		return LocalDic::GetLocal(_T("스위치 박스"));
			break;
	default:
		break;
	}

	return LocalDic::GetLocal(_T("사용자 정의 스위치 박스"));
}

CString MSFWSwitchBoxM::GetShapeName()
{
	return ToShapeName(m_eSwitchType);
}

double MSFWSwitchBoxM::GetWidth()
{
	return GetW();
}

double MSFWSwitchBoxM::GetHeight()
{
	return GetH();
}

void MSFWSwitchBoxM::SetWidth(double dWidth)
{
	SetW(dWidth);
}

void MSFWSwitchBoxM::SetHeight(double dHeight)
{
	SetH(dHeight);
}
