#include "stdafx.h"
#include "MSRcFWColumnM.h"

#include "GMLib/GM2DCircle.h"
#include "GMLib/GM2DBound.h"
#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSRcFWColumnM, MSRcLineMemberM, VERSIONABLE_SCHEMA | 1)
MSRcFWColumnM::MSRcFWColumnM()
{
	me_Shape = MSRcLineMemberM::POLYGON;
	m_nType = MSCompoM::msColumnM;
	me_FWColumnType = eFWColumnType::Rect;
	m_W = 500;
	m_D = 700;
	m_pProfile = new GM2DPolyline();
	MakeProfile();
}

MSRcFWColumnM::~MSRcFWColumnM()
{
	delete m_pProfile;
}

void MSRcFWColumnM::CopyFromMe(MSCompoM* pSource)
{
	if (pSource == NULL)	return;
	MSRcFWColumnM* pSourceLineM = (MSRcFWColumnM*)pSource;
	me_FWColumnType = pSourceLineM->me_FWColumnType;
	m_W = pSourceLineM->GetW();
	m_D = pSourceLineM->GetD();

	MSRcLineMemberM::CopyFromMe(pSource);
	m_pProfile->CopyFrom(pSourceLineM->m_pProfile);
}

void MSRcFWColumnM::Dump(EFS::EFS_Buffer& buffer)
{
	MSRcLineMemberM::Dump(buffer);
	int columnType = (int)me_FWColumnType;
	buffer << columnType;
	buffer << m_W << m_D;
	m_pProfile->Dump(buffer);
}

void MSRcFWColumnM::Recovery(EFS::EFS_Buffer& buffer)
{
	MSRcLineMemberM::Recovery(buffer);
	int columnType = 0;
	buffer >> columnType;
	me_FWColumnType = (eFWColumnType)columnType;
	buffer >> m_W >> m_D;
	m_pProfile->Recovery(buffer);
}

void MSRcFWColumnM::Serialize(CArchive &ar)
{
	MSRcLineMemberM::Serialize(ar);
	int columnType = 0;
	if (ar.IsStoring())
	{
		columnType = (int)me_FWColumnType;
		ar << columnType;
		ar << m_W;
		ar << m_D;
		m_pProfile->Serialize(ar);
	}
	else
	{
		ar >> columnType;
		me_FWColumnType = (eFWColumnType)columnType;
		ar >> m_W;
		ar >> m_D;
		if (MSVersionInfo::GetCurrentVersion() >= 20191024)
			m_pProfile->Serialize(ar);
		else
			MakeProfile();

		m_nType = Type_MSCompoM::msFWColumnM;
	}
}

void MSRcFWColumnM::SetProfile(GM2DPolyline pProfile)
{
	me_FWColumnType = eFWColumnType::Custom;
	m_pProfile->CopyFrom(&pProfile);

	GM2DBound boundBox;
	pProfile.Get2DBound(boundBox);
	m_W = boundBox.GetWidth();
	m_D = boundBox.GetHeight();

}

void MSRcFWColumnM::MakeProfile()
{
	if (me_FWColumnType == eFWColumnType::Custom)
	{
		if(m_pProfile->GetNumLines() == 0)
			makeProfile_Rect();
		return;
	}

	m_pProfile->ResetPolyline();
	switch (me_FWColumnType)
	{
	case eFWColumnType::Rect:	return makeProfile_Rect();
	case eFWColumnType::Circle: return makeProfile_Circle();
	case eFWColumnType::Pentagon_Inner: return makeProfile_PentagonInner();
	case eFWColumnType::Pentagon_Outter: return makeProfile_PentagonOutter();
	case eFWColumnType::Hexagon_Inner: return makeProfile_HexagonInner();
	case eFWColumnType::Hexagon_Outter: return makeProfile_HexagonOutter();
	case eFWColumnType::Octagon_Inner: return makeProfile_OctagonInner();
	case eFWColumnType::Octagon_Outter: return makeProfile_OctagonOutter();
	}
}

void MSRcFWColumnM::makeProfile_Rect()
{
	GM2DVector* leftBottom = new GM2DVector(-m_W / 2, -m_D / 2);
	GM2DVector* rightBottom = new GM2DVector(m_W / 2, -m_D / 2);
	GM2DVector* rightTop = new GM2DVector(m_W / 2, m_D / 2);
	GM2DVector* leftTop = new GM2DVector(-m_W / 2, m_D / 2);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);
	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);
	m_pProfile->Add(leftTop);
	m_pProfile->MakeClosedPolyline(tolerance);
}
void MSRcFWColumnM::makeProfile_Circle()
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
void MSRcFWColumnM::makeProfile_InsideCircle(GM2DCircle* pCircle, double startAngle, int numEdge)
{
	if (numEdge == 0)	return;

	double angleSpacing = (2 * PI) / numEdge;
	double locationAngle = startAngle;
	for (int i = 0; i < numEdge; i++)
	{
		GM2DVector* vec = new GM2DVector();
		pCircle->GetGlobalXY(locationAngle, *vec);

		m_pProfile->Add(vec);
		locationAngle += angleSpacing;
	}
	double tolerance = 1.0;
	m_pProfile->MakeClosedPolyline(tolerance);
}

void MSRcFWColumnM::makeProfile_PentagonInner()
{
	int numEdge = 5;
	double angle90 = (2 * PI) / 4;
	GM2DCircle circle(m_W / 2);
	makeProfile_InsideCircle(&circle, angle90, numEdge);
}
void MSRcFWColumnM::makeProfile_PentagonOutter()
{
	int numEdge = 5;
	double angle90 = (2 * PI) / 4;

	double angle = (2 * PI) / (numEdge * 2);
	double outRadius = (m_W / 2) / cos(angle);
	GM2DCircle circle(outRadius);
	makeProfile_InsideCircle(&circle, angle90, numEdge);
}
void MSRcFWColumnM::makeProfile_HexagonInner()
{
	int numEdge = 6;
	double angle0 = 0.0;
	GM2DCircle circle(m_W / 2);
	makeProfile_InsideCircle(&circle, angle0, numEdge);

}
void MSRcFWColumnM::makeProfile_HexagonOutter()
{
	int numEdge = 6;
	double angle0 = 0.0;

	double angle = (2 * PI) / (numEdge * 2);
	double outRadius = (m_W / 2) / cos(angle);
	GM2DCircle circle(outRadius);
	makeProfile_InsideCircle(&circle, angle0, numEdge);

}
void MSRcFWColumnM::makeProfile_OctagonInner()
{
	int numEdge = 8;
	double angle45 = (2 * PI) / 8;
	GM2DCircle circle(m_W / 2);
	makeProfile_InsideCircle(&circle, angle45, numEdge);

}
void MSRcFWColumnM::makeProfile_OctagonOutter()
{
	int numEdge = 8;
	double angle45 = (2 * PI) / 8;

	double angle = (2 * PI) / (numEdge * 2);
	double outRadius = (m_W / 2) / cos(angle);
	GM2DCircle circle(outRadius);
	makeProfile_InsideCircle(&circle, angle45, numEdge);

}

bool MSRcFWColumnM::IsPolygonSection()
{
	switch (me_FWColumnType)
	{
	case MSRcFWColumnM::Rect:
	case MSRcFWColumnM::Circle:
		return false;
	case MSRcFWColumnM::Pentagon_Inner:
	case MSRcFWColumnM::Pentagon_Outter:
	case MSRcFWColumnM::Hexagon_Inner:
	case MSRcFWColumnM::Hexagon_Outter:
	case MSRcFWColumnM::Octagon_Inner:
	case MSRcFWColumnM::Octagon_Outter:
	case MSRcFWColumnM::Custom:
		return true;
	}

	return false;
}

void MSRcFWColumnM::GetSecPolyline(GM2DPolyline& APGon)
{
	APGon.CopyFrom(m_pProfile);
}

void MSRcFWColumnM::SetFWColumnType(eFWColumnType type)
{
	if (type == eFWColumnType::Rect)
		me_Shape = SECT_SHAPE::RECT;
	else if(type == eFWColumnType::Circle)
		me_Shape = SECT_SHAPE::CIRCLE;
	else if(type == eFWColumnType::Custom)
		me_Shape = SECT_SHAPE::POLYGON;
	else
		me_Shape = SECT_SHAPE::POLYGON;

	me_FWColumnType = type;
	MakeProfile();
}

void MSRcFWColumnM::SetWidth(double param)
{
	if (me_FWColumnType != Custom)
	{
		m_W = param;
		MakeProfile();
	}
}

void MSRcFWColumnM::SetDepth(double param)
{
	if (me_FWColumnType != Custom)
	{
		m_D = param;
		MakeProfile();
	}
}

void MSRcFWColumnM::SetW(double param)
{
	SetWidth(param);
}

void MSRcFWColumnM::SetD(double param)
{
	SetDepth(param);
}

CString MSRcFWColumnM::ToShapeName(eFWColumnType eType)
{
	switch (eType)
	{
	case MSRcFWColumnM::Rect:
		return LocalDic::GetLocal(_T("사각기둥"));
	case MSRcFWColumnM::Circle:
		return LocalDic::GetLocal(_T("원형기둥"));
	case MSRcFWColumnM::Pentagon_Inner:
		return LocalDic::GetLocal(_T("오각(내접)기둥"));
	case MSRcFWColumnM::Pentagon_Outter:
		return LocalDic::GetLocal(_T("오각(외접)기둥"));
	case MSRcFWColumnM::Hexagon_Inner:
		return LocalDic::GetLocal(_T("육각(내접)기둥"));
	case MSRcFWColumnM::Hexagon_Outter:
		return LocalDic::GetLocal(_T("육각(외접)기둥"));
	case MSRcFWColumnM::Octagon_Inner:
		return LocalDic::GetLocal(_T("팔각(내접)기둥"));
	case MSRcFWColumnM::Octagon_Outter:
		return LocalDic::GetLocal(_T("팔각(외접)기둥"));
	case MSRcFWColumnM::Custom:
		return LocalDic::GetLocal(_T("사용자"));

	}

	return LocalDic::GetLocal(_T("사용자 정의 기둥"));
}

vector<CString> MSRcFWColumnM::GetFormWorkColumnShapeNameList()
{
	vector<CString> nameList;

	int fromIndex = eFWColumnType::Rect;
	int toIndex = eFWColumnType::Custom;

	for (int i = fromIndex; i <= toIndex; i++)
		nameList.push_back(ToShapeName((eFWColumnType)i));

	return nameList;
}

MSRcFWColumnM::eFWColumnType MSRcFWColumnM::ToFWColumnType(SECT_SHAPE eShapeType)
{
	switch (eShapeType)
	{
	case MSRcLineMemberM::RECT:
		return MSRcFWColumnM::Rect;
	case MSRcLineMemberM::CIRCLE:
		return MSRcFWColumnM::Circle;
	case MSRcLineMemberM::POLYGON:
		return MSRcFWColumnM::Pentagon_Inner;
	case MSRcLineMemberM::POLYGONINPLAN:
		return MSRcFWColumnM::Pentagon_Inner;
	}

	return MSRcFWColumnM::Pentagon_Inner;
}

CString MSRcFWColumnM::GetShapeName()
{
	MSCompoM::SetShapeName(ToShapeName(me_FWColumnType));
	return MSCompoM::GetShapeName();
}