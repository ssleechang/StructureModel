#include "stdafx.h"
#include "MSRcFWSlabM.h"

IMPLEMENT_SERIAL(MSRcFWSlabM, MSRcFaceMemberM, VERSIONABLE_SCHEMA | 1)
MSRcFWSlabM::MSRcFWSlabM()
{
	m_nType = MSCompoM::msSlabM;
	me_FWSlabType = eFWSlabType::General;
	m_T = 200;
	m_T1 = 125;
	m_pProfile = new GM2DPolyline();
	MakeProfile();
}

MSRcFWSlabM::~MSRcFWSlabM()
{
}

void MSRcFWSlabM::CopyFromMe(MSCompoM* pSource)
{
	if (pSource == NULL)	return;
	MSRcFWSlabM* pSourceLineM = (MSRcFWSlabM*)pSource;
	me_FWSlabType = pSourceLineM->me_FWSlabType;
	m_T = pSourceLineM->GetT();
	m_T1 = pSourceLineM->GetT1();
	MSRcFaceMemberM::CopyFromMe(pSource);
	MakeProfile();
}

void MSRcFWSlabM::Dump(EFS::EFS_Buffer& buffer)
{
	MSRcFaceMemberM::Dump(buffer);
	int SlabType = (int)me_FWSlabType;
	buffer << SlabType;
	buffer << m_T << m_T1;
	MakeProfile();
}

void MSRcFWSlabM::Recovery(EFS::EFS_Buffer& buffer)
{
	MSRcFaceMemberM::Recovery(buffer);
	int SlabType = 0;
	buffer >> SlabType;
	me_FWSlabType = (eFWSlabType)SlabType;
	buffer >> m_T >> m_T1;
	MakeProfile();
}

void MSRcFWSlabM::Serialize(CArchive &ar)
{
	MSRcFaceMemberM::Serialize(ar);
	int SlabType = 0;
	if (ar.IsStoring())
	{
		SlabType = (int)me_FWSlabType;
		ar << SlabType;
		ar << m_T;
		ar << m_T1;
	}
	else
	{
		ar >> SlabType;
		me_FWSlabType = (eFWSlabType)SlabType;
		ar >> m_T;
		ar >> m_T1;
		MakeProfile();
		m_nType = Type_MSCompoM::msFWSlabM;
	}
}

void MSRcFWSlabM::MakeProfile()
{
	double dLength = m_T * 5;
	MakeProfile(m_pProfile, dLength);
}

void MSRcFWSlabM::MakeProfile(GM2DPolyline* pTargetProflie, double dLength)
{
	if (pTargetProflie == nullptr)
		return;
	
	pTargetProflie->ResetPolyline();
	switch (me_FWSlabType)
	{
	case eFWSlabType::General:	return makeProfile_General(pTargetProflie, dLength);
	case eFWSlabType::TopSlope: return makeProfile_TopSlope(pTargetProflie, dLength);
	case eFWSlabType::BottomSlope: return makeProfile_BottomSlope(pTargetProflie, dLength);
	}
}

void MSRcFWSlabM::makeProfile_General(GM2DPolyline* pTargetProflie, double dLength)
{
	GM2DVector* leftBottom = new GM2DVector(0, -m_T);
	GM2DVector* rightBottom = new GM2DVector(dLength , -m_T);
	GM2DVector* rightTop = new GM2DVector(dLength, 0);
	GM2DVector* leftTop = new GM2DVector(0, 0);

	double tolerance = 1.0;
	pTargetProflie->AddPointAndBulge(leftBottom);
	pTargetProflie->AddPointAndBulge(rightBottom);
	pTargetProflie->AddPointAndBulge(rightTop);
	pTargetProflie->AddPointAndBulge(leftTop);
	pTargetProflie->MakeClosedPolyline(tolerance);
}

void MSRcFWSlabM::makeProfile_TopSlope(GM2DPolyline* pTargetProflie, double dLength)
{
	GM2DVector* leftBottom = new GM2DVector(0, -m_T1);
	GM2DVector* rightBottom = new GM2DVector(dLength, -m_T1);
	GM2DVector* rightTop = new GM2DVector(dLength, 0);
	GM2DVector* leftTop = new GM2DVector(0, (m_T - m_T1));

	double tolerance = 1.0;
	pTargetProflie->AddPointAndBulge(leftBottom);
	pTargetProflie->AddPointAndBulge(rightBottom);
	pTargetProflie->AddPointAndBulge(rightTop);
	pTargetProflie->AddPointAndBulge(leftTop);
	pTargetProflie->MakeClosedPolyline(tolerance);
}

void MSRcFWSlabM::makeProfile_BottomSlope(GM2DPolyline* pTargetProflie, double dLength)
{
	GM2DVector* leftBottom = new GM2DVector(0, -m_T);
	GM2DVector* rightBottom = new GM2DVector(dLength, -m_T1);
	GM2DVector* rightTop = new GM2DVector(dLength, 0);
	GM2DVector* leftTop = new GM2DVector(0, 0);

	double tolerance = 1.0;
	pTargetProflie->AddPointAndBulge(leftBottom);
	pTargetProflie->AddPointAndBulge(rightBottom);
	pTargetProflie->AddPointAndBulge(rightTop);
	pTargetProflie->AddPointAndBulge(leftTop);
	pTargetProflie->MakeClosedPolyline(tolerance);
}

CString MSRcFWSlabM::ToShapeName(eFWSlabType eType)
{
	switch (eType)
	{
	case MSRcFWSlabM::General:
		return LocalDic::GetLocal(_T("일반 슬래브"));
	case MSRcFWSlabM::BottomSlope:
		return LocalDic::GetLocal(_T("하부 경사 슬래브"));
	case MSRcFWSlabM::TopSlope:
		return LocalDic::GetLocal(_T("상부 경사 슬래브"));
	default:
		break;
	}

	return LocalDic::GetLocal(_T("사용자 정의 슬래브"));
}

vector<CString> MSRcFWSlabM::GetFormWorkSlabShapeNameList()
{
	vector<CString> nameList;

	int fromIndex = eFWSlabType::General;
	int toIndex = eFWSlabType::TopSlope;

	for (int i = fromIndex; i <= toIndex; i++)
		nameList.push_back(ToShapeName((eFWSlabType)i));

	return nameList;
}

CString MSRcFWSlabM::GetShapeName()
{
	return ToShapeName(me_FWSlabType);
}