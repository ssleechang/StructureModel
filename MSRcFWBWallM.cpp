#include "stdafx.h"
#include "MSRcFWBWallM.h"

IMPLEMENT_SERIAL(MSRcFWBWallM, MSRcFaceMemberM, VERSIONABLE_SCHEMA | 1)
MSRcFWBWallM::MSRcFWBWallM()
{
	m_nType = MSCompoM::msBWallM;
	me_FWBWallType = eFWBWallType::General;
	m_T = 200;
	m_pProfile = new GM2DPolyline();
	MakeProfile();
}

MSRcFWBWallM::~MSRcFWBWallM()
{
}

void MSRcFWBWallM::CopyFromMe(MSCompoM* pSource)
{
	if (pSource == NULL)	return;
	MSRcFWBWallM* pSourceLineM = (MSRcFWBWallM*)pSource;
	me_FWBWallType = pSourceLineM->me_FWBWallType;
	m_T = pSourceLineM->GetT();

	MakeProfile();
	MSRcFaceMemberM::CopyFromMe(pSource);
}

void MSRcFWBWallM::Dump(EFS::EFS_Buffer& buffer)
{
	MSRcFaceMemberM::Dump(buffer);
	int WallType = (int)me_FWBWallType;
	buffer << WallType;
	buffer << m_T;
	MakeProfile();
}

void MSRcFWBWallM::Recovery(EFS::EFS_Buffer& buffer)
{
	MSRcFaceMemberM::Recovery(buffer);
	int WallType = 0;
	buffer >> WallType;
	me_FWBWallType = (eFWBWallType)WallType;
	buffer >> m_T;
	MakeProfile();
}

void MSRcFWBWallM::Serialize(CArchive &ar)
{
	MSRcFaceMemberM::Serialize(ar);
	int WallType = 0;
	if (ar.IsStoring())
	{
		WallType = (int)me_FWBWallType;
		ar << WallType;
		ar << m_T;
	}
	else
	{
		ar >> WallType;
		me_FWBWallType = (eFWBWallType)WallType;
		ar >> m_T;
		MakeProfile();

		m_nType = Type_MSCompoM::msFWBWallM;
	}
}

void MSRcFWBWallM::MakeProfile()
{
	m_pProfile->ResetPolyline();
	switch (me_FWBWallType)
	{
	case eFWBWallType::General:	return makeProfile_General();
	case eFWBWallType::Brick: return makeProfile_Brick();
	}
}

void MSRcFWBWallM::makeProfile_General()
{
	double H = m_T * 5;
	GM2DVector* leftBottom = new GM2DVector(-m_T / 2, -H / 2);
	GM2DVector* rightBottom = new GM2DVector(m_T / 2, -H / 2);
	GM2DVector* rightTop = new GM2DVector(m_T / 2, H / 2);
	GM2DVector* leftTop = new GM2DVector(-m_T / 2, H / 2);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);
	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);
	m_pProfile->Add(leftTop);
	m_pProfile->MakeClosedPolyline(tolerance);
}
void MSRcFWBWallM::makeProfile_Brick()
{
	double H = m_T * 5;
	GM2DVector* leftBottom = new GM2DVector(-m_T / 2, -H / 2);
	GM2DVector* rightBottom = new GM2DVector(m_T / 2, -H / 2);
	GM2DVector* rightTop = new GM2DVector(m_T / 2, H / 2);
	GM2DVector* leftTop = new GM2DVector(-m_T / 2, H / 2);

	double tolerance = 1.0;
	m_pProfile->Add(leftBottom);
	m_pProfile->Add(rightBottom);
	m_pProfile->Add(rightTop);
	m_pProfile->Add(leftTop);
	m_pProfile->MakeClosedPolyline(tolerance);
}

CString MSRcFWBWallM::ToShapeName(eFWBWallType eType)
{
	switch (eType)
	{
	case MSRcFWBWallM::General:
		return LocalDic::GetLocal(_T("일반 벽체"));
	case MSRcFWBWallM::Brick:
		return LocalDic::GetLocal(_T("일반 조적벽"));
	}

	return LocalDic::GetLocal(_T("사용자 정의 벽체"));
}

vector<CString> MSRcFWBWallM::GetFormWorkWallShapeNameList()
{
	vector<CString> nameList;

	int fromIndex = eFWBWallType::General;
	int toIndex = eFWBWallType::Brick;

	for (int i = fromIndex; i <= toIndex; i++)
		nameList.push_back(ToShapeName((eFWBWallType)i));

	return nameList;
}

CString MSRcFWBWallM::GetShapeName()
{
	return ToShapeName(me_FWBWallType);
}