#include "StdAfx.h"
#include "MSHandRailG.h"
#include "MSRebarBuilding.h"
#include "MSRcHandRailM.h"
#include "MSFloor.h"
#include "MSLineLevelDifG.h"
#include "MSMemberLevelDif.h"
#include "GMLib/MSVersionInfo.h"
#include "gmlib/AcisGeomHelper.h"

IMPLEMENT_SERIAL(MSHandRailG, MSLineMemberG,  VERSIONABLE_SCHEMA | 1)
MSHandRailG::MSHandRailG()
{
	Init(true);
}

MSHandRailG::~MSHandRailG(void)
{
	DeleteParaPetInfo();
}

void MSHandRailG::Serialize(CArchive &ar)
{
	MSLineMemberG::Serialize(ar);
	bool bEachCompoM = false;
	if(ar.IsStoring())
	{
		if(mp_CompoM)
		{
			if(mp_CompoM->m_ID == 0)
				bEachCompoM = true;

			ar << bEachCompoM;
			if(bEachCompoM == true)
				Serialize_EachCompoM(ar);
		}
		ar << m_IsLeftParapet;

		GM2DPolyline::SerializeWithNullFlag(ar, m_pParapetPoly);
		GM2DCurve::SerializeWithNullFlag(ar, m_pParapetLeftCurve);
		GM2DCurve::SerializeWithNullFlag(ar, m_pParapetRightCurve);
		GM2DCurve::SerializeWithNullFlag(ar, m_pParapetLeftCurveTR);
		GM2DCurve::SerializeWithNullFlag(ar, m_pParapetRightCurveTR);

		ar << m_dTopOffset << m_dBottomOffset << m_nTopQuantityType << m_nBottomQuantityType;
	}
	else
	{
		ar >> bEachCompoM;
		if(bEachCompoM)
			Serialize_EachCompoM(ar);

		double temp = fabs(m_dTopOffset);
		if (isnan(m_dTopOffset) || temp < DTOL_GM || temp > INT_MAX)
			m_dTopOffset = 0;
		
		temp = fabs(m_dBottomOffset);
		if (isnan(m_dBottomOffset) || temp < DTOL_GM || temp > INT_MAX)
			m_dBottomOffset = 0;


		if (MSVersionInfo::GetCurrentVersion() >= 20150907)
			ar >> m_IsLeftParapet;

		if (MSVersionInfo::GetCurrentVersion() >= 20170303)
		{
			GM2DPolyline::SerializeWithNullFlag(ar, m_pParapetPoly);
			GM2DCurve::SerializeWithNullFlag(ar, m_pParapetLeftCurve);
			GM2DCurve::SerializeWithNullFlag(ar, m_pParapetRightCurve);
			GM2DCurve::SerializeWithNullFlag(ar, m_pParapetLeftCurveTR);
			GM2DCurve::SerializeWithNullFlag(ar, m_pParapetRightCurveTR);
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20180308)
		{
			ar >> m_dTopOffset  >> m_dBottomOffset >> m_nTopQuantityType >> m_nBottomQuantityType;
		}
		else
		{
			m_dTopOffset = 0;
			m_dBottomOffset = 0;
			m_nTopQuantityType = 0;
			m_nBottomQuantityType = 0; // Hor = 0 Vertical = 1
		}
	}
}

void MSHandRailG::Serialize_EachCompoM_Rebar(CArchive& ar)
{
	if (ar.IsStoring())
	{
		MSRcHandRailM* pRailM = (MSRcHandRailM*)mp_CompoM;
		long MType = pRailM->m_nType;

		ar << MType;
		mp_CompoM->Serialize(ar);
	}
	else
	{
		if(MSVersionInfo::GetCurrentVersion() < 20220321)
		{
			MSBaseBuilding* pBldg = GetBuilding();
			long MType = 0;
			ar >> MType;

			MSRcHandRailM* pRailM = (MSRcHandRailM*)pBldg->CreateNewCompoM(MType);
			pRailM->m_nType = (MSCompoM::Type_MSCompoM)MType;

			long nMaterialID = 0;
			ar >> nMaterialID;
			pRailM->SetMaterial(pBldg->GetMaterial(nMaterialID));
			ar >> pRailM->m_Name;

			double dThick = 0, dTopDepth = 0, dBotDepth = 0;
			ar >> dThick;
			ar >> dTopDepth;

			pRailM->SetThick(dThick);
			pRailM->SetTopLevel(dTopDepth);
			if (MSVersionInfo::GetCurrentVersion() >= 20140315)
			{
				ar >> dBotDepth;
				pRailM->SetBotLevel(dBotDepth);
			}

			pRailM->mp_Owner = pBldg;
			mp_CompoM = pRailM;
		}
		else
		{
			long nType = 0;
			ar >> nType;
			mp_CompoM = GetBuilding()->CreateNewCompoM(nType);
			mp_CompoM->Serialize(ar);
		}
	}
}

MSObject* MSHandRailG::Copy( bool bNewID /*= true*/ )
{
	MSHandRailG* pNewObj;

	pNewObj = new MSHandRailG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;

}

void MSHandRailG::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;
	if( !pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() != msHandRailG) { ASSERT(0); return; }
	MSLineMemberG::CopyFromMe(pSource_in, bNewID);

	this->m_IsLeftParapet = ((MSHandRailG*)pSource_in)->m_IsLeftParapet;

	MSHandRailG* pSourceHandRail = (MSHandRailG*)pSource_in;

	DeleteParaPetInfo();
	if (pSourceHandRail->HasParaPet())
	{
		if (pSourceHandRail->m_pParapetPoly != nullptr)
		{
			m_pParapetPoly = new GM2DPolyline();
			m_pParapetPoly->CopyFrom(pSourceHandRail->m_pParapetPoly);
		}
		
		if (pSourceHandRail->m_pParapetLeftCurve != nullptr)
			m_pParapetLeftCurve = pSourceHandRail->m_pParapetLeftCurve->Copy();

		if (pSourceHandRail->m_pParapetLeftCurveTR != nullptr)
			m_pParapetLeftCurveTR = pSourceHandRail->m_pParapetLeftCurveTR->Copy();

		if (pSourceHandRail->m_pParapetRightCurve != nullptr)
			m_pParapetRightCurve = pSourceHandRail->m_pParapetRightCurve->Copy();

		if (pSourceHandRail->m_pParapetRightCurveTR != nullptr)
			m_pParapetRightCurveTR = pSourceHandRail->m_pParapetRightCurveTR->Copy();
	}
	
	m_dTopOffset = pSourceHandRail->GetTopOffset();
	m_dBottomOffset = pSourceHandRail->GetBottomOffset();
	m_nTopQuantityType = pSourceHandRail->GetTopQuantityType();
	m_nBottomQuantityType = pSourceHandRail->GetBottomQuantityType();
}

void MSHandRailG::Dump(EFS::EFS_Buffer& buffer)
{
	MSLineMemberG::Dump(buffer);

	buffer << m_IsLeftParapet;
	long isValidValue = false;
	if (m_pParapetPoly != nullptr)
	{
		isValidValue = 1;
		buffer << isValidValue;
		m_pParapetPoly->Dump(buffer);
	}
	else
	{
		isValidValue = 0;
		buffer << isValidValue;
	}

	GM2DCurve::Dump(buffer, m_pParapetLeftCurve);
	GM2DCurve::Dump(buffer, m_pParapetRightCurve);
	GM2DCurve::Dump(buffer, m_pParapetLeftCurveTR);
	GM2DCurve::Dump(buffer, m_pParapetRightCurveTR);

	buffer << m_dTopOffset;
	buffer << m_dBottomOffset;
	buffer << m_nTopQuantityType;
	buffer << m_nBottomQuantityType;
}

void MSHandRailG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSLineMemberG::Recovery(buffer);

	buffer >> m_IsLeftParapet;
	DeleteParaPetInfo();

	long isValidValue = 0;
	buffer >> isValidValue;
	if (isValidValue == 1)
	{
		m_pParapetPoly = new GM2DPolyline();
		m_pParapetPoly->Recovery(buffer);
	}
	else
		delete m_pParapetPoly;

	GM2DCurve::Recovery(buffer, m_pParapetLeftCurve);
	GM2DCurve::Recovery(buffer, m_pParapetRightCurve);
	GM2DCurve::Recovery(buffer, m_pParapetLeftCurveTR);
	GM2DCurve::Recovery(buffer, m_pParapetRightCurveTR);

	buffer >> m_dTopOffset;
	buffer >> m_dBottomOffset;
	buffer >> m_nTopQuantityType;
	buffer >> m_nBottomQuantityType;
}

void MSHandRailG::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSLineMemberG::Init(bInConstructor);

	m_pParapetPoly = nullptr;
	m_pParapetLeftCurve = nullptr;
	m_pParapetRightCurve = nullptr;
	m_pParapetLeftCurveTR = nullptr;
	m_pParapetRightCurveTR = nullptr;

	m_dTopOffset =0;
	m_dBottomOffset = 0;
	m_nTopQuantityType = 0;
	m_nBottomQuantityType =0; // Hor = 0 Vertical = 1
}

bool MSHandRailG::GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal, bool bCheckLevelZone)
{
	double dTopOffSetZ = 0, dBotOffSetZ = 0;
	dTopOffSetZ = max(m_SOffsetZ, m_EOffsetZ) + m_dTopOffset;
	dBotOffSetZ = min(m_SOffsetZ, m_EOffsetZ) - m_dBottomOffset;

	MSRcHandRailM* pCompoM = (MSRcHandRailM*)GetCompoM();
	double dTopDepth = GetTopDepth();
	double dBotDepth = GetBottomDepth();

	double dTopZLevelDif = 0;
	double dBotZLevelDif = 0;
	if(bCheckLevelZone)
	{
		dTopZLevelDif = GetLevelbyLevelDif(true);
		dBotZLevelDif = GetLevelbyLevelDif(false);
	}

	if(mp_Owner && mp_Owner->IsFloor())
	{
		MSFloor* pCurFloor = (MSFloor*)mp_Owner;
		TopLevel = GetSlabLevel() + ((MSFloor*)mp_Owner)->GetSlabHeight() + dTopDepth + dTopOffSetZ + dTopZLevelDif;
		BottomLevel = GetSlabLevel() + ((MSFloor*)mp_Owner)->GetSlabHeight() - dBotDepth + dBotOffSetZ + dBotZLevelDif;
	}
	else
	{
		TopLevel = GetSlabLevel() + dTopDepth + dTopOffSetZ + dTopZLevelDif;
		BottomLevel = GetSlabLevel() - dBotDepth + dBotOffSetZ + dBotZLevelDif;
	}

	return true;
}

bool MSHandRailG::GetZLevelsbyLoc(double& TopLevel, double& BottomLevel, double dLoc)
{
	double dOffSet = GetOffSetbyLoc(dLoc);

	MSRcHandRailM* pCompoM = (MSRcHandRailM*)GetCompoM();
	double dTopDepth = GetTopHeight();
	double dBotDepth = GetBottomHeight();
	double dLevelZoneZ = 0;

	if(mp_Owner && mp_Owner->IsFloor())
	{
		MSFloor* pCurFloor = (MSFloor*)mp_Owner;
		shared_ptr<GM2DCurve> pCenCurve = GetCenCurve();
		GM2DVector TheVec = pCenCurve->PointAtParam(dLoc);

		if(ma_MemberLevelDif.GetSize() > 0) // 조인트의 X,Y 좌표가 래벨존에 포함되나 MemberLevelDif를 생성안하는 경우가 존재함 --> 평행할경우
			dLevelZoneZ = pCurFloor->GetLevel4LevelZone(TheVec.m_X, TheVec.m_Y);
		else
			dLevelZoneZ = pCurFloor->GetLevel4LevelZone(this);

		TopLevel = GetSlabLevel() + pCurFloor->GetSlabHeight() + dTopDepth + dOffSet + dLevelZoneZ;
		BottomLevel = GetSlabLevel() + pCurFloor->GetSlabHeight() - dBotDepth + dOffSet + dLevelZoneZ;
	}
	else
	{
		TopLevel = GetSlabLevel() + dTopDepth + dOffSet;
		BottomLevel = GetSlabLevel() - dBotDepth + dOffSet;

	}
	return true;
}

void MSHandRailG::InitTrimmedBoundary(bool IsTopLevel)
{
	MSCompoG::InitTrimmedBoundary();
	InitTrimmedBoundaryVisibility();
	MakeBoundaryWidthParaPet(GetTrimmedBoundary());
}

void MSHandRailG::GetBoundaryNSubMember(GM2DPolyline& APolyLine)
{
	GetBoundaryWidthParaPet(APolyLine);
	//MakeBoundaryWidthParaPet(&APolyLine);
}

void MSHandRailG::MakeBoundaryWidthParaPet(GM2DPolyline* pPoly)
{
	pPoly->ResetPolyline();
	if (isArc() == false)
	{
		if (HasParaPet())
		{
			// [BH-2230] 이슈로 SetLines();주석 삭제
			SetLines();
			if (m_IsLeftParapet)
			{
				pPoly->AddCopy(mp_RLine->m_P0);
				pPoly->ma_Bulge.Add(0);
				pPoly->AddCopy(mp_RLine->m_P1);
				pPoly->ma_Bulge.Add(0);
				pPoly->AddCopy(m_pParapetLeftCurve->m_P1);
				pPoly->ma_Bulge.Add(0);
				pPoly->AddCopy(m_pParapetLeftCurve->m_P0);
				pPoly->ma_Bulge.Add(0);
				pPoly->AddCopy(mp_RLine->m_P0);
			}
			else
			{
				pPoly->AddCopy(m_pParapetRightCurve->m_P0);
				pPoly->ma_Bulge.Add(0);
				pPoly->AddCopy(m_pParapetRightCurve->m_P1);
				pPoly->ma_Bulge.Add(0);
				pPoly->AddCopy(mp_LLine->m_P1);
				pPoly->ma_Bulge.Add(0);
				pPoly->AddCopy(mp_LLine->m_P0);
				pPoly->ma_Bulge.Add(0);
				pPoly->AddCopy(m_pParapetRightCurve->m_P0);
			}
		}
		else
		{
			pPoly->AddCopy(mp_RLine->m_P0);
			pPoly->ma_Bulge.Add(0);
			pPoly->AddCopy(mp_RLine->m_P1);
			pPoly->ma_Bulge.Add(0);
			pPoly->AddCopy(mp_LLine->m_P1);
			pPoly->ma_Bulge.Add(0);
			pPoly->AddCopy(mp_LLine->m_P0);
			pPoly->ma_Bulge.Add(0);
			pPoly->AddCopy(mp_RLine->m_P0);
		}

	}
	else
	{
		if (HasParaPet())
		{
			SetLines();
			if (m_IsLeftParapet)
			{
				pPoly->AddCopy(mp_RLine->m_P0);
				pPoly->ma_Bulge.Add(mp_RLine->GetBulge());
				pPoly->AddCopy(mp_RLine->m_P1);
				pPoly->ma_Bulge.Add(0);
				pPoly->AddCopy(m_pParapetLeftCurve->m_P1);
				pPoly->ma_Bulge.Add(m_pParapetLeftCurve->GetBulge() * -1);
				pPoly->AddCopy(m_pParapetLeftCurve->m_P0);
				pPoly->ma_Bulge.Add(0);
				pPoly->AddCopy(mp_RLine->m_P0);
			}
			else
			{
				pPoly->AddCopy(m_pParapetRightCurve->m_P0);
				pPoly->ma_Bulge.Add(m_pParapetRightCurve->GetBulge());
				pPoly->AddCopy(m_pParapetRightCurve->m_P1);
				pPoly->ma_Bulge.Add(0);
				pPoly->AddCopy(mp_LLine->m_P1);
				pPoly->ma_Bulge.Add(mp_LLine->GetBulge() * -1);
				pPoly->AddCopy(mp_LLine->m_P0);
				pPoly->ma_Bulge.Add(0);
				pPoly->AddCopy(m_pParapetRightCurve->m_P0);
			}
		}
		else
		{
			pPoly->AddCopy(mp_RLine->m_P0);
			pPoly->ma_Bulge.Add(mp_RLine->GetBulge());
			pPoly->AddCopy(mp_RLine->m_P1);
			pPoly->ma_Bulge.Add(0);
			pPoly->AddCopy(mp_LLine->m_P1);
			pPoly->ma_Bulge.Add(mp_LLine->GetBulge() * -1);
			pPoly->AddCopy(mp_LLine->m_P0);
			pPoly->ma_Bulge.Add(0);
			pPoly->AddCopy(mp_RLine->m_P0);
		}
	}
	pPoly->MakeCCW();
}

bool MSHandRailG::MakeBoundary()
{
	GM2DPolyline* pBoundary = mp_Boundary;
	pBoundary->ResetPolyline();
	if (mp_Owner->m_bIsSerializing == false) // Serialize 아닐 경우에만 처리
	{
		if (HasParaPet())
		{
			if (m_pParapetLeftCurve == nullptr || m_pParapetRightCurve == nullptr)
				SetLines();
		}
		else
			DeleteParaPetInfo();
	}

	if (isArc() == false)
	{
		pBoundary->AddCopy(mp_RLine->m_P0);
		pBoundary->ma_Bulge.Add(0);
		pBoundary->AddCopy(mp_RLine->m_P1);
		pBoundary->ma_Bulge.Add(0);
		pBoundary->AddCopy(mp_LLine->m_P1);
		pBoundary->ma_Bulge.Add(0);
		pBoundary->AddCopy(mp_LLine->m_P0);
		pBoundary->ma_Bulge.Add(0);
		pBoundary->AddCopy(mp_RLine->m_P0);
	}
	else
	{
		pBoundary->AddCopy(mp_RLine->m_P0);
		pBoundary->ma_Bulge.Add(mp_RLine->GetBulge());
		pBoundary->AddCopy(mp_RLine->m_P1);
		pBoundary->ma_Bulge.Add(0);
		pBoundary->AddCopy(mp_LLine->m_P1);
		pBoundary->ma_Bulge.Add(mp_LLine->GetBulge() * -1);
		pBoundary->AddCopy(mp_LLine->m_P0);
		pBoundary->ma_Bulge.Add(0);
		pBoundary->AddCopy(mp_RLine->m_P0);
	}
	pBoundary->MakeCCW();

	return true;
}

void MSHandRailG::SetLines()
{
	// 입력 라인 기준으로 LR라인 생성
	// => Center Line 기준으로 생성
	double dWidth = GetWidth();
	double dUsedParaPetWidth = GetParaPetWidth();

	// User 입력점 계산
	UpdateCurveInputByCenCurve();

	shared_ptr<GM2DCurve> pCurveCenter = GetCenCurve();

	if (mp_LLine) delete mp_LLine;
	if (mp_RLine) delete mp_RLine;

	mp_LLine = pCurveCenter->Copy();
	mp_RLine = pCurveCenter->Copy();
	mp_LLine->NormalTranslateMe(dWidth / 2);
	mp_RLine->NormalTranslateMe(-dWidth / 2);

	if (HasParaPet())
	{
		if (m_pParapetPoly == nullptr)
			m_pParapetPoly = new GM2DPolyline();

		if (m_pParapetLeftCurve)
			delete m_pParapetLeftCurve;
		if (m_pParapetRightCurve)
			delete m_pParapetRightCurve;
		m_pParapetLeftCurve = pCurveCenter->Copy();
		m_pParapetRightCurve = pCurveCenter->Copy();
		if (m_IsLeftParapet)
		{
			m_pParapetLeftCurve->NormalTranslateMe((dWidth / 2) + dUsedParaPetWidth);
			m_pParapetRightCurve->NormalTranslateMe(dWidth / 2);
		}
		else
		{
			m_pParapetLeftCurve->NormalTranslateMe(-dWidth / 2);
			m_pParapetRightCurve->NormalTranslateMe(-((dWidth / 2) + dUsedParaPetWidth));
		}
		m_pParapetLeftCurveTR = m_pParapetLeftCurve->Copy();
		m_pParapetRightCurveTR = m_pParapetRightCurve->Copy();
	}
	else
		DeleteParaPetInfo();
	
	if (mp_LLineTR != NULL)
	{
		delete mp_LLineTR;
		mp_LLineTR = NULL;
	}
	if (mp_RLineTR != NULL)
	{
		delete mp_RLineTR;
		mp_RLineTR = NULL;
	}
}

void MSHandRailG::DeleteParaPetInfo()
{
	delete m_pParapetPoly;
	delete m_pParapetLeftCurve;
	delete m_pParapetRightCurve;
	delete m_pParapetLeftCurveTR;
	delete m_pParapetRightCurveTR;
	m_pParapetPoly = nullptr;
	m_pParapetLeftCurve = nullptr;
	m_pParapetRightCurve = nullptr;
	m_pParapetLeftCurveTR = nullptr;
	m_pParapetRightCurveTR = nullptr;
}

bool MSHandRailG::HasParaPet()
{
	MSRcHandRailM* pCompoM = (MSRcHandRailM*)GetCompoM();
	if (pCompoM->m_nParapetType == 0)
		return false;
	
	return true;
}

double MSHandRailG::GetParaPetWidth()
{
	double dParaPetWidth = 0;
	MSRcHandRailM* pCompoM = (MSRcHandRailM*)GetCompoM();
	if (pCompoM == nullptr)
		return 0;

	switch (pCompoM->m_nParapetType)
	{
	case 0:
		dParaPetWidth = 0;
		break;
	case 1:
	case 2:
		dParaPetWidth = pCompoM->m_dParapetLengthA;
		break;
	case 3:
	case 4:
		dParaPetWidth = pCompoM->m_dParapetLengthB;
		break;

	default:
		break;
	}
	
	return fabs(dParaPetWidth);
}

void MSHandRailG::GetBoundaryWidthParaPet(GM2DPolyline& Poly)
{
	GM2DPolyline* pTrimmedBoundary = GetTrimmedBoundary();
	if (m_pParapetPoly != nullptr)
	{
		vector<GM2DPolyline*> Joined_Polys;
		pTrimmedBoundary->BoolOperation('U', m_pParapetPoly, Joined_Polys, DTOL_GM);
		if (Joined_Polys.size() == 1)
			Poly.CopyFrom(Joined_Polys[0]);
		else
		{
			Poly.CopyFrom(pTrimmedBoundary);
		}
		GMObject::DeleteGMObject(Joined_Polys);
	}
	else
		Poly.CopyFrom(pTrimmedBoundary);
}

void MSHandRailG::RemoveInwardWedge()
{
	//MSLinkMemberG::RemoveInwardWedge(); // 참고하여 구현
	if (isArc())
		return;

	GM2DPolyline* pMyBoundary = this->GetTrimmedBoundary();
	int minOnLLineIdx = pMyBoundary->ma_Point.GetSize() + 1;
	int maxOnLLineIdx = -1;
	int minOnRLineIdx = pMyBoundary->ma_Point.GetSize() + 1;
	int maxOnRLineIdx = -1;

	double dMinLLoc = 100;
	double dMaxLLoc = -100;
	double dMinRLoc = 100;
	double dMaxRLoc = -100;

	GM2DCurve* pLLine = mp_LLine;
	GM2DCurve* pRLine = mp_RLine;
	if (m_IsLeftParapet == true && m_pParapetLeftCurve != nullptr)
		pLLine = m_pParapetLeftCurve;
	else if(m_IsLeftParapet == false && m_pParapetRightCurve != nullptr)
		pRLine = m_pParapetRightCurve;

	double dTol = DTOL_GM;
	for (int idx = 0; idx < pMyBoundary->ma_Point.GetSize() - 1; idx++)
	{
		GM2DVector* pVec = pMyBoundary->ma_Point[idx];
		if (pLLine->IsColinearPoint(*pVec, dTol))
		{
			double dLoc = pLLine->OrthoBaseLocValue(*pVec);
			if (dMinLLoc > dLoc)
			{
				dMinLLoc = dLoc;
				minOnLLineIdx = idx;
			}

			if (dMaxLLoc < dLoc)
			{
				dMaxLLoc = dLoc;
				maxOnLLineIdx = idx;
			}
		}
		if (pRLine->IsColinearPoint(*pVec, dTol))
		{
			double dLoc = pRLine->OrthoBaseLocValue(*pVec);

			if (dMinRLoc > dLoc)
			{
				dMinRLoc = dLoc;
				minOnRLineIdx = idx;
			}

			if (dMaxRLoc < dLoc)
			{
				dMaxRLoc = dLoc;
				maxOnRLineIdx = idx;
			}
		}
	}

	vector<int> IndicesToDelete;
	for (int idx = 0; idx < pMyBoundary->ma_Point.GetSize() - 1; idx++)
	{
		if (idx == minOnLLineIdx || idx == maxOnLLineIdx
			|| idx == minOnRLineIdx || idx == maxOnRLineIdx)
			continue;
		IndicesToDelete.push_back(idx);
	}

	// Delte Point and Bulge at Polyline
	long nIndex = IndicesToDelete.size();
	for (long iIndex = nIndex - 1; iIndex >= 0; iIndex--)
	{
		pMyBoundary->RemovePointBulgeAt(IndicesToDelete[iIndex]);
	}
	pMyBoundary->MakeClosedPolyline(dTol);
}

void MSHandRailG::FindMiterLineWithOtherLinkG(MSLinkMemberG* link, GM2DLineVector& rLine)
{
	if (HasParaPet() == false)
		MSLinkMemberG::FindMiterLineWithOtherLinkG(link, rLine);
	else
	{
		if (m_IsLeftParapet)
			MSLinkMemberG::FindMiterLineWithOtherLinkG_NextStep(link, rLine, m_pParapetLeftCurve, mp_RLine);
		else
			MSLinkMemberG::FindMiterLineWithOtherLinkG_NextStep(link, rLine, mp_LLine, m_pParapetRightCurve);
	}
}

bool MSHandRailG::MakeExtendedBoundary(GM2DPolyline& pExtendedBoundary, long nExtendType, double dExtendLen)
{
	if (HasParaPet() == false)
		return MSLinkMemberG::MakeExtendedBoundary(pExtendedBoundary, nExtendType, dExtendLen);
	else
	{
		if (m_IsLeftParapet)
			return MSLinkMemberG::MakeExtendedBoundary_NextStep(pExtendedBoundary, m_pParapetLeftCurve, mp_RLine, nExtendType, dExtendLen);
		else
			return MSLinkMemberG::MakeExtendedBoundary_NextStep(pExtendedBoundary, mp_LLine, m_pParapetRightCurve, nExtendType, dExtendLen);
	}
}

bool MSHandRailG::MakeExtendedBoundary_ToLine(GM2DPolyline& pExtendedBoundary, GM2DLineVector& line)
{
	if (HasParaPet() == false)
		return MSLinkMemberG::MakeExtendedBoundary_ToLine(pExtendedBoundary, line);
	else
	{
		bool bFirst = (GetTrimmedBoundary() == NULL); // only first - use org LR lines
		GM2DCurve* pLCurve = nullptr;
		GM2DCurve* pRCurve = nullptr;

		if (m_IsLeftParapet)
		{
			pLCurve = !bFirst && m_pParapetLeftCurveTR != NULL ? m_pParapetLeftCurveTR : m_pParapetLeftCurve;
			pRCurve = !bFirst && mp_RLineTR != NULL ? mp_RLineTR : mp_RLine;
		}
		else
		{
			pLCurve = !bFirst && mp_LLineTR != NULL ? mp_LLineTR : mp_LLine;
			pRCurve = !bFirst && m_pParapetRightCurveTR != NULL ? m_pParapetRightCurveTR : m_pParapetRightCurve;
		}

		return MSLinkMemberG::MakeExtendedBoundary_ToLine_NextStep(pExtendedBoundary, line, pLCurve, pRCurve);
	}
}

void MSHandRailG::UpdateTrimmedBoundary_AfterCalculate(CON_POS ePos)
{
	if (HasParaPet() == false)
		return;
	GM2DPolyline* pTrimmedBoundary = GetTrimmedBoundary();

	shared_ptr<GM2DCurve> CenCurve = GetCenCurve();
	GM2DVector CenVec = CenCurve->PointAtParam(0.5);

	vector<GM2DCurve*> curveList;
	vector<GM2DPolyline*> poly_list;
	GM2DVector CheckVec;
	GM2DLineVector CheckLine;
	CheckLine.m_P0 = m_pParapetLeftCurve->PointAtParam(0.5);
	CheckLine.m_P1 = m_pParapetRightCurve->PointAtParam(0.5);
	CheckVec = CheckLine.PointAtParam(0.5);

	if (pTrimmedBoundary->Contains(&CheckVec) == 1)
		return;

	if (m_IsLeftParapet)
		curveList.push_back(m_pParapetRightCurve->Copy());
	else
		curveList.push_back(m_pParapetLeftCurve->Copy());

	double dExtendLen = 1000;
	if (curveList.size() > 0)
	{
		curveList[0]->ExtendLineVector(TRUE, dExtendLen, FALSE);
		curveList[0]->ExtendLineVector(FALSE, dExtendLen, TRUE);
	}
	
	AcisGeomHelper::DividePolyline(curveList, pTrimmedBoundary, poly_list); // curveList, tgt => poly_list (result)

	if (poly_list.size() == 2)
	{
		for (int iPoly = 0; iPoly < poly_list.size(); iPoly++)
		{
			GM2DPolyline* pPoly = poly_list[iPoly];
			if (pPoly->Contains(&CenVec) == -1)
				pTrimmedBoundary->CopyFrom(pPoly);
			else
			{
				if(m_pParapetPoly == nullptr)
					m_pParapetPoly = new GM2DPolyline();
				m_pParapetPoly->CopyFrom(pPoly);
			}
		}
	}
	else
	{
		//ASSERT(0);
		if(m_pParapetPoly == nullptr)
			SetDefParapetPoly();

		GMObject::DeleteGMObject(curveList);
		GMObject::DeleteGMObject(poly_list);
		return;
	}
	GMObject::DeleteGMObject(poly_list);

	SetTrimmedLines();

	bool IsUnionParapet = false;
	for (int iConn = 0; iConn < ma_Connections.size(); iConn++)
	{
		MSConnection* pConn = ma_Connections[iConn];
		vector<GM2DPolyline*> IntersectPolys = pConn->GetIntersects();
		if(IntersectPolys.size() == 0)
			continue;
		if(pConn->GetFirstCompoG() != this)
			continue;

		CON_POS ePos = pConn->GetLinkMemberConPos(this);
		if ( (ePos == CON_START || ePos == CON_END) &&
			(pConn->GetFirstTrimOption() == MSConnection::TO_EXTENDCUT && pConn->GetSecondTrimOption() == MSConnection::TO_EXTENDCUT) )
		{
			for (int i = 0; i < IntersectPolys.size(); i++)
			{
				m_pParapetPoly->BoolOperation('U', IntersectPolys[i], poly_list, DTOL_GM);
				if (poly_list.size() == 1)
				{
					m_pParapetPoly->CopyFrom(poly_list[0]);
					IsUnionParapet = true;
				}
				GMObject::DeleteGMObject(poly_list);
			}
		}
	}

	if (IsUnionParapet == true)
	{
		AcisGeomHelper::DividePolyline(curveList, m_pParapetPoly, poly_list); // curveList, tgt => poly_list (result)
		for (int iPoly = 0; iPoly < poly_list.size(); iPoly++)
		{
			GM2DPolyline* pPoly = poly_list[iPoly];
			if (pPoly->Contains(&CheckVec) == -1)
				m_pParapetPoly->CopyFrom(pPoly);
		}
		GMObject::DeleteGMObject(poly_list);
	}
	
	double dDegTol = DTOL_GM;
	double dDistTol = DTOL_GM;
	m_pParapetPoly->DeleteInLinePoint();
	m_pParapetPoly->DeleteDuplicatedPoints();
	m_pParapetPoly->CheckAndFixFoldedEdges(DTOL_GM);
	for (int iCurve = 0; iCurve < m_pParapetPoly->GetNumLines(); iCurve++)
	{
		GM2DCurve* pCurve = m_pParapetPoly->GetCurve(iCurve);
		
		long nRval = m_pParapetLeftCurve->IsColinear4DTol(pCurve, dDegTol, dDistTol);
		if (nRval == -1)
			pCurve->ReverseDirection();
		if (nRval != 0)
		{
			if (ePos == CON_START)
				m_pParapetLeftCurveTR->SetP0(pCurve->m_P0);
			else if(ePos == CON_END)
				m_pParapetLeftCurveTR->SetP1(pCurve->m_P1);
			else
				m_pParapetLeftCurveTR->CopyFrom(pCurve);
		}
		
		nRval = m_pParapetRightCurve->IsColinear4DTol(pCurve, dDegTol, dDistTol);
		if (nRval == -1)
			pCurve->ReverseDirection();
		if (nRval != 0)
		{
			if (ePos == CON_START)
				m_pParapetRightCurveTR->SetP0(pCurve->m_P0);
			else if(ePos == CON_END)
				m_pParapetRightCurveTR->SetP1(pCurve->m_P1);
			else
				m_pParapetRightCurveTR->CopyFrom(pCurve);
		}
		delete pCurve;
	}

	GMObject::DeleteGMObject(curveList);
	InitTrimmedBoundaryVisibility();
}

void MSHandRailG::SetDefParapetPoly()
{
	if (m_pParapetPoly == nullptr)
		m_pParapetPoly = new GM2DPolyline();
	m_pParapetPoly->ResetPolyline();

	m_pParapetPoly->AddCopy(m_pParapetRightCurve->m_P0);
	m_pParapetPoly->ma_Bulge.Add(0);
	m_pParapetPoly->AddCopy(m_pParapetRightCurve->m_P1);
	m_pParapetPoly->ma_Bulge.Add(0);
	m_pParapetPoly->AddCopy(m_pParapetLeftCurve->m_P1);
	m_pParapetPoly->ma_Bulge.Add(0);
	m_pParapetPoly->AddCopy(m_pParapetLeftCurve->m_P0);
	m_pParapetPoly->ma_Bulge.Add(0);
	m_pParapetPoly->AddCopy(m_pParapetRightCurve->m_P0);

}

double MSHandRailG::GetTopDepth()
{
	MSRcHandRailM* pCompoM = (MSRcHandRailM*)GetCompoM();
	double dTopDepth = pCompoM->GetTopLevel();
	return dTopDepth;
}

double MSHandRailG::GetBottomDepth()
{
	MSRcHandRailM* pCompoM = (MSRcHandRailM*)GetCompoM();
	double dBottomDepth = pCompoM->GetBotLevel();
	return dBottomDepth;
}

double MSHandRailG::GetTopHeight()
{
	MSRcHandRailM* pCompoM = (MSRcHandRailM*)GetCompoM();
	double dTopDepth = pCompoM->GetTopLevel();
	return m_dTopOffset + dTopDepth;
}

double MSHandRailG::GetBottomHeight()
{
	MSRcHandRailM* pCompoM = (MSRcHandRailM*)GetCompoM();
	double dBottomDepth = pCompoM->GetBotLevel();
	return m_dBottomOffset + dBottomDepth;
}

double MSHandRailG::GetTopOffset()
{
	return m_dTopOffset;
}

void MSHandRailG::SetTopOffset(double dTopOffSet)
{
	m_dTopOffset = dTopOffSet;
}

double MSHandRailG::GetBottomOffset()
{
	return m_dBottomOffset;
}

void MSHandRailG::SetBottomOffset(double dBottomOffSet)
{
	m_dBottomOffset = dBottomOffSet;
}

int MSHandRailG::GetTopQuantityType()
{
	return m_nTopQuantityType;
}

void MSHandRailG::SetTopQuantityType(int nType)
{
	if (((MSFloor*)this->GetOwner())->GetUpperFloor()->GetUpperFloor() != nullptr)
		m_nTopQuantityType = nType;
	else
		m_nTopQuantityType = 0;
}

int MSHandRailG::GetBottomQuantityType()
{
	return m_nBottomQuantityType;
}

void MSHandRailG::SetBottomQuantityType(int nType)
{
	if (((MSFloor*)this->GetOwner())->GetUpperFloor()->GetUpperFloor() != nullptr)
		m_nBottomQuantityType = nType;
	else
		m_nBottomQuantityType = 0;
}

bool MSHandRailG::GetTopQuantitySeparation()
{
	bool retValue = false;

	if (m_nTopQuantityType == 1) //vertical
		retValue = true;

	return retValue;
}
