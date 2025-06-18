#include "stdafx.h"
#include "MSRcFWBeamM.h"


IMPLEMENT_SERIAL(MSRcFWBeamM, MSRcLineMemberM, VERSIONABLE_SCHEMA | 1)
MSRcFWBeamM::MSRcFWBeamM()
{
	me_Shape = MSRcLineMemberM::POLYGON;
	m_nType = MSCompoM::msBeamM;
	me_FWBeamType = eFWBeamType::Type1;
	m_W = 500;
	m_W1 = m_W2 = 0;
	m_H = 700;
	m_H1 = m_H2 = 0;

	m_pProfile = new GM2DPolyline();
	MakeProfile();
}

MSRcFWBeamM::~MSRcFWBeamM()
{
	delete m_pProfile;
}

void MSRcFWBeamM::CopyFromMe(MSCompoM* pSource)
{
	if (pSource == NULL)	return;
	MSRcFWBeamM* pSourceLineM = (MSRcFWBeamM*)pSource;
	me_FWBeamType = pSourceLineM->me_FWBeamType;
	m_W = pSourceLineM->GetW();
	m_W1 = pSourceLineM->GetW1();
	m_W2 = pSourceLineM->GetW2();
	m_H = pSourceLineM->GetH();
	m_H1 = pSourceLineM->GetH1();
	m_H2 = pSourceLineM->GetH2();

	MSRcLineMemberM::CopyFromMe(pSource);
	MakeProfile();
}

void MSRcFWBeamM::Dump(EFS::EFS_Buffer& buffer)
{
	MSRcLineMemberM::Dump(buffer);
	int BeamType = (int)me_FWBeamType;
	buffer << BeamType;
	buffer << m_W << m_W1 << m_W2;
	buffer << m_H << m_H1 << m_H2;
	MakeProfile();
}

void MSRcFWBeamM::Recovery(EFS::EFS_Buffer& buffer)
{
	MSRcLineMemberM::Recovery(buffer);
	int BeamType = 0;
	buffer >> BeamType;
	me_FWBeamType = (eFWBeamType)BeamType;
	buffer >> m_W >> m_W1 >> m_W2;
	buffer >> m_H >> m_H1 >> m_H2;
	MakeProfile();
}

void MSRcFWBeamM::Serialize(CArchive &ar)
{
	MSRcLineMemberM::Serialize(ar);
	int BeamType = 0;
	if (ar.IsStoring())
	{
		BeamType = (int)me_FWBeamType;
		ar << BeamType;
		ar << m_W;
		ar << m_W1;
		ar << m_W2;
		ar << m_H;
		ar << m_H1;
		ar << m_H2;
	}
	else
	{
		ar >> BeamType;
		me_FWBeamType = (eFWBeamType)BeamType;
		ar >> m_W;
		ar >> m_W1;
		ar >> m_W2;
		ar >> m_H;
		ar >> m_H1;
		ar >> m_H2;
		MakeProfile();

		m_nType = Type_MSCompoM::msFWBeamM;
	}
}

bool MSRcFWBeamM::CheckProfile()
{
	bool returnVal = true;
	switch (me_FWBeamType)
	{
	case eFWBeamType::Type3: //이중 보
	case eFWBeamType::Type6: //이중 역보
		returnVal = CheckProfile_Type3();
		break;
	case eFWBeamType::Type4: //특수 보
		returnVal = CheckProfile_Type4();
		break;
	case eFWBeamType::Type7: //특수 역보
		returnVal = CheckProfile_Type7();
		break;
	case eFWBeamType::Type8: //경사 역보
		returnVal = CheckProfile_Type8();
		break;
	case eFWBeamType::Type9: //모따기 역보
		returnVal = CheckProfile_Type9();
		break;

	}
	return returnVal;
}

bool MSRcFWBeamM::CheckProfile_Type3()
{
	if (m_W <= m_W1 + m_W2) return false;
	return true;
}

bool MSRcFWBeamM::CheckProfile_Type4()
{
	return (CheckProfile_Type3() == true) && ((m_H1 < m_H) == true);
}

bool MSRcFWBeamM::CheckProfile_Type7()
{
	return (CheckProfile_Type3() == true) && (m_H1 < m_H == true);
}

bool MSRcFWBeamM::CheckProfile_Type8() 
{
	return !(m_H1 >= m_H == true);
}

bool MSRcFWBeamM::CheckProfile_Type9()
{
	return (CheckProfile_Type8() == true) && !(m_W1 >= m_W == true);
}

void MSRcFWBeamM::MakeProfile()
{
	m_pProfile->ResetPolyline();
	switch (me_FWBeamType)
	{
		case eFWBeamType::Type1:	return makeProfile_Type1();
		case eFWBeamType::Type2:	return makeProfile_Type2();
		case eFWBeamType::Type3:	return makeProfile_Type3();
		case eFWBeamType::Type4:	return makeProfile_Type4();
		case eFWBeamType::Type5:	return makeProfile_Type5();
		case eFWBeamType::Type6:	return makeProfile_Type6();
		case eFWBeamType::Type7:	return makeProfile_Type7();
		case eFWBeamType::Type8:	return makeProfile_Type8();
		case eFWBeamType::Type9:	return makeProfile_Type9();
	}
}

void MSRcFWBeamM::makeProfile_Type1()
{
	GM2DVector* leftBottom = new GM2DVector(-m_W / 2, -m_H);
	GM2DVector* rightBottom = new GM2DVector(m_W / 2, -m_H);
	GM2DVector* rightTop = new GM2DVector(m_W / 2, 0);
	GM2DVector* leftTop = new GM2DVector(-m_W / 2, 0);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);
	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);
	m_pProfile->Add(leftTop);
	m_pProfile->MakeClosedPolyline(tolerance);
}

void MSRcFWBeamM::makeProfile_Type2()
{
	GM2DVector* leftBottom = new GM2DVector(-m_W / 2, -max(m_H, m_H1));
	GM2DVector* rightBottom = new GM2DVector(m_W / 2, -max(m_H, m_H1));
	GM2DVector* rightTop = new GM2DVector(m_W / 2, 0);
	GM2DVector* leftTop = new GM2DVector(-m_W / 2, 0);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);
	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);
	m_pProfile->Add(leftTop);
	m_pProfile->MakeClosedPolyline(tolerance);
}
void MSRcFWBeamM::makeProfile_Type3()
{
	GM2DVector* leftBottom = new GM2DVector(-m_W / 2, -m_H);
	GM2DVector* rightBottom = new GM2DVector(m_W / 2, -m_H);
	GM2DVector* rightTop = new GM2DVector(m_W / 2, 0);
	GM2DVector* leftTop = new GM2DVector(-m_W / 2, 0);

	// 이중보 : 아래 작은 보 형상좌표
	GM2DVector* vec1 = new GM2DVector(leftBottom->m_X + m_W1, leftBottom->m_Y);
	GM2DVector* vec2 = new GM2DVector(vec1->m_X, leftBottom->m_Y - m_H1);
	GM2DVector* vec3 = new GM2DVector(vec2->m_X + m_W2, vec2->m_Y);
	GM2DVector* vec4 = new GM2DVector(vec3->m_X, leftBottom->m_Y);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);

	m_pProfile->Add(vec1);
	m_pProfile->Add(vec2);
	m_pProfile->Add(vec3);
	m_pProfile->Add(vec4);

	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);
	m_pProfile->Add(leftTop);
	m_pProfile->MakeClosedPolyline(tolerance);

}
void MSRcFWBeamM::makeProfile_Type4() // 특수보
{
	GM2DVector* leftBottom = new GM2DVector(-m_W / 2, -m_H);
	GM2DVector* rightBottom = new GM2DVector(m_W / 2, -m_H);
	GM2DVector* rightTop = new GM2DVector(m_W / 2, 0);
	GM2DVector* leftTop = new GM2DVector(-m_W / 2, 0);

	// 특수보 : 아래 작은 보 형상좌표
	GM2DVector* vec1 = new GM2DVector(leftBottom->m_X + m_W1, leftBottom->m_Y);
	GM2DVector* vec2 = new GM2DVector(vec1->m_X, leftBottom->m_Y + m_H1);
	GM2DVector* vec3 = new GM2DVector(vec2->m_X + m_W2, vec2->m_Y);
	GM2DVector* vec4 = new GM2DVector(vec3->m_X, leftBottom->m_Y);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);

	m_pProfile->Add(vec1);
	m_pProfile->Add(vec2);
	m_pProfile->Add(vec3);
	m_pProfile->Add(vec4);

	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);
	m_pProfile->Add(leftTop);
	m_pProfile->MakeClosedPolyline(tolerance);

}
void MSRcFWBeamM::makeProfile_Type5()
{
	GM2DVector* leftBottom = new GM2DVector(-m_W / 2, 0);
	GM2DVector* rightBottom = new GM2DVector(m_W / 2, 0);
	GM2DVector* rightTop = new GM2DVector(m_W / 2, m_H);
	GM2DVector* leftTop = new GM2DVector(-m_W / 2, m_H);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);
	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);
	m_pProfile->Add(leftTop);
	m_pProfile->MakeClosedPolyline(tolerance);
}
void MSRcFWBeamM::makeProfile_Type6() //이중 역보
{
	GM2DVector* leftBottom = new GM2DVector(-m_W / 2, 0);
	GM2DVector* rightBottom = new GM2DVector(m_W / 2, 0);
	GM2DVector* rightTop = new GM2DVector(m_W / 2, m_H);
	GM2DVector* leftTop = new GM2DVector(-m_W / 2, m_H);

	// 이중역보 : 위 작은 보 형상좌표
	GM2DVector* vec1 = new GM2DVector(leftTop->m_X + (m_W1+m_W2), rightTop->m_Y);
	GM2DVector* vec2 = new GM2DVector(vec1->m_X, rightTop->m_Y + m_H1);
	GM2DVector* vec3 = new GM2DVector(vec2->m_X - m_W2, vec2->m_Y);
	GM2DVector* vec4 = new GM2DVector(vec3->m_X, rightTop->m_Y);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);
	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);

	m_pProfile->Add(vec1);
	m_pProfile->Add(vec2);
	m_pProfile->Add(vec3);
	m_pProfile->Add(vec4);

	m_pProfile->Add(leftTop);
	m_pProfile->MakeClosedPolyline(tolerance);
}
void MSRcFWBeamM::makeProfile_Type7() // 특수역보
{
	GM2DVector* leftBottom = new GM2DVector(-m_W / 2, 0);
	GM2DVector* rightBottom = new GM2DVector(m_W / 2, 0);
	GM2DVector* rightTop = new GM2DVector(m_W / 2, m_H);
	GM2DVector* leftTop = new GM2DVector(-m_W / 2, m_H);

	// 특수역보 : 위 작은 보 형상좌표
	GM2DVector* vec1 = new GM2DVector(leftTop->m_X + (m_W1 + m_W2), rightTop->m_Y);
	GM2DVector* vec2 = new GM2DVector(vec1->m_X, rightTop->m_Y - m_H1);
	GM2DVector* vec3 = new GM2DVector(vec2->m_X - m_W2, vec2->m_Y);
	GM2DVector* vec4 = new GM2DVector(vec3->m_X, rightTop->m_Y);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);
	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);

	m_pProfile->Add(vec1);
	m_pProfile->Add(vec2);
	m_pProfile->Add(vec3);
	m_pProfile->Add(vec4);

	m_pProfile->Add(leftTop);
	m_pProfile->MakeClosedPolyline(tolerance);

}
void MSRcFWBeamM::makeProfile_Type8()
{
	GM2DVector* leftBottom = new GM2DVector(-m_W / 2, 0);
	GM2DVector* rightBottom = new GM2DVector(m_W / 2, 0);
	GM2DVector* rightTop = new GM2DVector(m_W / 2, m_H - m_H1);
	GM2DVector* leftTop = new GM2DVector(-m_W / 2, m_H);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);
	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);
	m_pProfile->Add(leftTop);
	m_pProfile->MakeClosedPolyline(tolerance);

}
void MSRcFWBeamM::makeProfile_Type9()
{
	GM2DVector* leftBottom = new GM2DVector(-m_W / 2, 0);
	GM2DVector* rightBottom = new GM2DVector(m_W / 2, 0);
	GM2DVector* rightTop = new GM2DVector(m_W / 2, m_H - m_H1);
	GM2DVector* leftTop = new GM2DVector(-m_W / 2, m_H);

	GM2DVector* vec1 = new GM2DVector(rightTop->m_X - m_W1, m_H);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);
	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);
	m_pProfile->Add(vec1);
	m_pProfile->Add(leftTop);
	m_pProfile->MakeClosedPolyline(tolerance);

}

bool MSRcFWBeamM::IsPolygonSection()
{
	switch (me_FWBeamType)
	{
	case MSRcFWBeamM::Type1:
		return false;
	case MSRcFWBeamM::Type2:
	case MSRcFWBeamM::Type3:
	case MSRcFWBeamM::Type4:
	case MSRcFWBeamM::Type5:
	case MSRcFWBeamM::Type6:
	case MSRcFWBeamM::Type7:
	case MSRcFWBeamM::Type8:
	case MSRcFWBeamM::Type9:
		return true;
	}

	return false;
}

void MSRcFWBeamM::GetSecPolyline(GM2DPolyline& TheGon)
{
	TheGon.CopyFrom(m_pProfile);
}

void MSRcFWBeamM::SetFWBeamType(eFWBeamType type)
{
	if (type == Type1 || type == Type5)
		me_Shape = MSRcLineMemberM::RECT;
	else
		me_Shape = MSRcLineMemberM::POLYGON;

	me_FWBeamType = type;
	MakeProfile();
}

bool MSRcFWBeamM::IsReversBeamM()
{
	if (me_FWBeamType > Type4)
		return true;

	return false;
}

double MSRcFWBeamM::GetMaxH()
{
	switch (me_FWBeamType)
	{
	case MSRcFWBeamM::Type1:
		return m_H;
	case MSRcFWBeamM::Type2:
		return max(m_H, m_H1);
	case MSRcFWBeamM::Type3:
		return m_H + m_H1;
	case MSRcFWBeamM::Type4:
		return m_H;
	case MSRcFWBeamM::Type5:
		return m_H;
	case MSRcFWBeamM::Type6:
		return m_H + m_H1;
	case MSRcFWBeamM::Type7:
		return m_H;
	case MSRcFWBeamM::Type8:
		return m_H;
	case MSRcFWBeamM::Type9:
		return m_H;
	default:
		return m_H;
	}
}

CString MSRcFWBeamM::ToShapeName(eFWBeamType eType)
{
	switch (eType)
	{
	case MSRcFWBeamM::Type1:
		return LocalDic::GetLocal(_T("일반 보"));
	case MSRcFWBeamM::Type2:
		return LocalDic::GetLocal(_T("캔틸레버 보"));
	case MSRcFWBeamM::Type3:
		return LocalDic::GetLocal(_T("이중 보"));
	case MSRcFWBeamM::Type4:
		return LocalDic::GetLocal(_T("특수 보"));
	case MSRcFWBeamM::Type5:
		return LocalDic::GetLocal(_T("일반 역보"));
	case MSRcFWBeamM::Type6:
		return LocalDic::GetLocal(_T("이중 역보"));
	case MSRcFWBeamM::Type7:
		return LocalDic::GetLocal(_T("특수 역보"));
	case MSRcFWBeamM::Type8:
		return LocalDic::GetLocal(_T("경사 역보"));
	case MSRcFWBeamM::Type9:
		return LocalDic::GetLocal(_T("모따기 역보"));
	default:
		return LocalDic::GetLocal(_T("일반 보"));
	}
}

vector<CString> MSRcFWBeamM::GetFormWorkBeamShapeNameList()
{
	vector<CString> nameList;

	int fromIndex = eFWBeamType::Type1;
	int toIndex = eFWBeamType::Type9;

	for (int i = fromIndex; i <= toIndex; i++)
		nameList.push_back(ToShapeName((eFWBeamType)i));

	return nameList;
}

CString MSRcFWBeamM::GetShapeName()
{
	return ToShapeName(me_FWBeamType);
}

bool MSRcFWBeamM::IsCustomM()
{
	if (me_FWBeamType == eFWBeamType::Type1 ||
		me_FWBeamType == eFWBeamType::Type5)
		return false;

	return true;
}