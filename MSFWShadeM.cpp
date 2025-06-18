#include "stdafx.h"
#include "MSFWShadeM.h"


IMPLEMENT_SERIAL(MSFWShadeM, MSCompoM, VERSIONABLE_SCHEMA | 1)
MSFWShadeM::MSFWShadeM()
{
	m_nType = MSCompoM::msFWShadeM;
	m_eShadeType = MSFWShadeM::Horizontal_Type1;
	m_W = 200;
	m_H = 100;
	m_W1 = 10;
	m_H1 = 10;
	m_pProfile = new GM2DPolyline();
	MakeProfile();
}

MSFWShadeM::~MSFWShadeM()
{
	delete m_pProfile;
}
void MSFWShadeM::CopyFromMe(MSCompoM* pSource)
{
	if (pSource == NULL)	return;

	MSFWShadeM* pSourceM = (MSFWShadeM*)pSource;
	m_eShadeType = pSourceM->GetFWShadeType();
	m_W = pSourceM->GetW();
	m_H = pSourceM->GetH();
	m_W1 = pSourceM->GetW1();
	m_H1 = pSourceM->GetH1();

	MSCompoM::CopyFromMe(pSource);
	MakeProfile();
}

void MSFWShadeM::Dump(EFS::EFS_Buffer& buffer)
{
	int type = (int)m_eShadeType;
	buffer << type;
	buffer << m_W << m_H << m_W1 << m_H1;
	MSCompoM::Dump(buffer);
	MakeProfile();
}

void MSFWShadeM::Recovery(EFS::EFS_Buffer& buffer)
{
	int type = 0;
	buffer >> type;
	m_eShadeType = (eFWShadeType)type;
	buffer >> m_W >> m_H >> m_W1 >> m_H1;
	MSCompoM::Recovery(buffer);
	MakeProfile();
}

void MSFWShadeM::Serialize(CArchive &ar)
{
	MSCompoM::Serialize(ar);
	int type = 0;
	if (ar.IsStoring())
	{
		type = (int)m_eShadeType;
		ar << type;
		ar << m_W;
		ar << m_H;
		ar << m_W1;
		ar << m_H1;
	}
	else
	{
		ar >> type;
		m_eShadeType = (eFWShadeType)type;
		ar >> m_W;
		ar >> m_H;
		ar >> m_W1;
		ar >> m_H1;
		MakeProfile();
	}
}

void MSFWShadeM::SetW(double param)
{
	m_W = param;
	MakeProfile();
}
void MSFWShadeM::SetH(double param)
{
	m_H = param;
	MakeProfile();
}
void MSFWShadeM::SetW1(double param)
{
	m_W1 = param;
	MakeProfile();
}
void MSFWShadeM::SetH1(double param)
{
	m_H1 = param;
	MakeProfile();
}
bool MSFWShadeM::CheckProfile()
{
	switch (m_eShadeType)
	{
	case MSFWShadeM::Horizontal_Type1: return CheckProfile_HorizontalType1();
	case MSFWShadeM::Horizontal_Type2: return CheckProfile_HorizontalType2();
	case MSFWShadeM::Vertical_Type1: return CheckProfile_VerticalType1();
	case MSFWShadeM::Vertical_Type2: return CheckProfile_VerticalType2();
	case MSFWShadeM::Vertical_Type3: return CheckProfile_VerticalType3();
	}
	return false;
}

bool MSFWShadeM::CheckProfile_HorizontalType1()
{
	return true;
}
bool MSFWShadeM::CheckProfile_HorizontalType2()
{
	if (m_W < m_W1)	return false;
	if (m_H < m_H1)	return false;
	return true;
}

bool MSFWShadeM::CheckProfile_VerticalType1()
{
	return true;
}
bool MSFWShadeM::CheckProfile_VerticalType2()
{
	if (m_W < m_W1)	return false;
	if (m_H < m_H1) return false;
	return true;
}
bool MSFWShadeM::CheckProfile_VerticalType3()
{
	if (m_W < m_W1)	return false;
	if (m_H < m_H1) return false;
	return true;
}

void MSFWShadeM::MakeProfile()
{
	m_pProfile->ResetPolyline();
	switch (m_eShadeType)
	{
	case MSFWShadeM::Horizontal_Type1:	return makeProfile_HorizontalType1();
	case MSFWShadeM::Horizontal_Type2:	return makeProfile_HorizontalType2();
	case MSFWShadeM::Vertical_Type1:	return makeProfile_VerticalType1();
	case MSFWShadeM::Vertical_Type2:	return makeProfile_VerticalType2();
	case MSFWShadeM::Vertical_Type3:	return makeProfile_VerticalType3();
	}
}
void MSFWShadeM::makeProfile_HorizontalType1()
{
	GM2DVector* leftBottom = new GM2DVector(0, 0);
	GM2DVector* rightBottom = new GM2DVector(0, m_H);
	GM2DVector* rightTop = new GM2DVector(-m_W, m_H);
	GM2DVector* leftTop = new GM2DVector(-m_W, 0);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);
	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);
	m_pProfile->Add(leftTop);
	m_pProfile->MakeClosedPolyline(tolerance);
}
void MSFWShadeM::makeProfile_HorizontalType2()
{
	GM2DVector* leftBottom = new GM2DVector(0, 0);
	GM2DVector* rightBottom = new GM2DVector(0, m_H);
	GM2DVector* rightTop = new GM2DVector(-(m_W - m_W1), m_H);
	GM2DVector* w1Pt = new GM2DVector(-m_W, m_H - m_H1);
	GM2DVector* h1Pt = new GM2DVector(-m_W, 0);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);
	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);
	m_pProfile->Add(w1Pt);
	m_pProfile->Add(h1Pt);
	m_pProfile->MakeClosedPolyline(tolerance);
}
void MSFWShadeM::makeProfile_VerticalType1()
{
	GM2DVector* leftBottom = new GM2DVector(0, 0);
	GM2DVector* rightBottom = new GM2DVector(m_W, 0);
	GM2DVector* rightTop = new GM2DVector(m_W, m_H);
	GM2DVector* leftTop = new GM2DVector(0, m_H);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);
	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);
	m_pProfile->Add(leftTop);
	m_pProfile->MakeClosedPolyline(tolerance);
}
void MSFWShadeM::makeProfile_VerticalType2() //왼쪽이 대각선
{
	GM2DVector* leftBottom = new GM2DVector(0, 0);
	GM2DVector* rightBottom = new GM2DVector(m_W, 0);
	GM2DVector* rightTop = new GM2DVector(m_W, m_H);
	GM2DVector* w1Pt = new GM2DVector(m_W1, m_H);
	GM2DVector* h1Pt = new GM2DVector(0, m_H - m_H1);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);
	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);
	m_pProfile->Add(w1Pt);
	m_pProfile->Add(h1Pt);
	m_pProfile->MakeClosedPolyline(tolerance);
}
void MSFWShadeM::makeProfile_VerticalType3() //오른쪽이 대각선
{
	GM2DVector* leftBottom = new GM2DVector(0, 0);
	GM2DVector* rightBottom = new GM2DVector(m_W, 0);
	GM2DVector* h1Pt = new GM2DVector(m_W, m_H - m_H1);
	GM2DVector* w1Pt = new GM2DVector(m_W - m_W1, m_H);
	GM2DVector* leftTop = new GM2DVector(0, m_H);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);
	m_pProfile->Add(rightBottom);
	m_pProfile->Add(h1Pt);
	m_pProfile->Add(w1Pt);
	m_pProfile->Add(leftTop);
	m_pProfile->MakeClosedPolyline(tolerance);
}

CString MSFWShadeM::ToShapeName(eFWShadeType eType)
{
	switch (eType)
	{
	case eFWShadeType::Horizontal_Type1:
		return LocalDic::GetLocal(_T("수평 차양-1"));
		break;
	case eFWShadeType::Horizontal_Type2:
		return LocalDic::GetLocal(_T("수평 차양-2"));
		break;
	case eFWShadeType::Vertical_Type1:
		return LocalDic::GetLocal(_T("수직 차양-1"));
	case eFWShadeType::Vertical_Type2:
		return LocalDic::GetLocal(_T("수직 차양-2"));
	case eFWShadeType::Vertical_Type3:
		return LocalDic::GetLocal(_T("수직 차양-3"));
		break;
	default:
		break;
	}

	return LocalDic::GetLocal(_T("사용자 정의 차양"));
}

CString MSFWShadeM::GetShapeName()
{
	return ToShapeName(m_eShadeType);
}

double MSFWShadeM::GetWidth()
{
	return GetW();
}
double MSFWShadeM::GetDepth()
{
	return GetH();
}
double MSFWShadeM::GetHeight()
{
	return GetH();
}

void MSFWShadeM::SetWidth(double dWidth)
{
	SetW(dWidth);
}

void MSFWShadeM::SetHeight(double dHeight)
{
	SetH(dHeight);
}

bool MSFWShadeM::IsVerticalShade()
{
	if (m_eShadeType > eFWShadeType::Horizontal_Type2)
	{
		return true;
	}
	else
	{
		return false;
	}
}