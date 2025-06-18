#include "stdafx.h"
#include "MSFWShadeG.h"
#include "MSWallMemberG.h"
#include "MSCompoM.h"
#include "MSFWShadeM.h"
#include "GMLib/GM2DArc.h"
#include "StructureModel\MSModelChecker.h"

IMPLEMENT_SERIAL(MSFWShadeG, MSCompoG, VERSIONABLE_SCHEMA | 1)
MSFWShadeG::MSFWShadeG(void)
{
	m_pOwnerCompoG = NULL;
	m_IsLeftWallCurve = true;
	m_dHorizontalShadeLength = 0;
	m_dBotOffset = 0;
	m_dTopOffset = 0;
	m_dDistanceX = 0;
}

MSFWShadeG::~MSFWShadeG(void)
{
}

void MSFWShadeG::Dump(EFS::EFS_Buffer& buffer)
{
	MSCompoG::Dump(buffer);
	buffer << m_IsLeftWallCurve;
	buffer << m_dHorizontalShadeLength;
	buffer << m_dBotOffset << m_dTopOffset << m_dDistanceX;
}

void MSFWShadeG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSCompoG::Recovery(buffer);
	buffer >> m_IsLeftWallCurve;
	buffer >> m_dHorizontalShadeLength;
	buffer >> m_dBotOffset >> m_dTopOffset >> m_dDistanceX;
}

void MSFWShadeG::Serialize(CArchive &ar)
{
	MSCompoG::Serialize(ar);
	bool bEachCompoM = false;

	if (ar.IsStoring())
	{
		if (mp_CompoM)
		{
			if (mp_CompoM->m_ID == 0)
				bEachCompoM = true;
			ar << bEachCompoM;
			if (bEachCompoM == true)
				Serialize_EachCompoM(ar);
		}

		ar << m_dDistanceX;
		ar << m_dBotOffset;
		ar << m_dTopOffset;
		ar << m_IsLeftWallCurve;
		ar << m_dHorizontalShadeLength;
	}
	else
	{
		ar >> bEachCompoM;
		if (bEachCompoM)
			Serialize_EachCompoM(ar);

		ar >> m_dDistanceX;
		ar >> m_dBotOffset;
		ar >> m_dTopOffset;
		ar >> m_IsLeftWallCurve;
		ar >> m_dHorizontalShadeLength;
	}
}

MSObject* MSFWShadeG::Copy(bool bNewID)
{
	MSFWShadeG* pCopy = new MSFWShadeG();
	pCopy->CopyFromMe(this, bNewID);
	return pCopy;
}

void MSFWShadeG::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;
	MSCompoG::CopyFromMe(pSource_in, bNewID);

	MSFWShadeG* pSource = (MSFWShadeG*)pSource_in;
	mp_Owner = pSource->mp_Owner;
	if (pSource->mp_Source)
		mp_Source = pSource->mp_Source;
	m_dDistanceX = pSource->m_dDistanceX;
	m_dBotOffset = pSource->m_dBotOffset;
	m_dTopOffset = pSource->m_dTopOffset;
	m_IsLeftWallCurve = pSource->GetIsLeftWallCurve();
	m_dHorizontalShadeLength = pSource->m_dHorizontalShadeLength;
	SetOwnerCompoG(pSource->m_pOwnerCompoG);
}

void MSFWShadeG::SetOwnerCompoG(MSElement* pOwnerCompoG)
{
	m_pOwnerCompoG = pOwnerCompoG;
}

bool MSFWShadeG::MakeBoundary()
{
	MSWallMemberG* pWall = (MSWallMemberG*)m_pOwnerCompoG;
	GM2DPolyline* pBoundary = mp_Boundary;
	pBoundary->ResetPolyline();

	if (IsVerticalShade())
	{
		MSFWShadeM* shadeM = dynamic_cast<MSFWShadeM*>(mp_CompoM);
		pBoundary->CopyFrom(shadeM->GetProfile());
		if (GetIsLeftWallCurve() == false)
		{
			GM2DLineVector theLine = GM2DLineVector::MakeGM2DLineVector(0, *pBoundary->ma_Point[0]);
			pBoundary->MirrorByAxis(theLine);
		}
		pBoundary->RotateZ(GetDegree() * Deg2Rad, shadeM->GetProfile()->ma_Point[0]);
		pBoundary->Translate(GetStartPnt());
	}
	else
	{
		double dWidth = GetWidth();
		if (GetIsLeftWallCurve())
		{
			GM2DCurve* pRCurve = GetHorShadeGBaseCurve();
			GM2DCurve* pLCurve = pRCurve->Copy();
			pLCurve->NormalTranslateMe(dWidth);

			pBoundary->AddCopy(pRCurve->m_P0);
			pBoundary->ma_Bulge.Add(pRCurve->GetBulge());
			pBoundary->AddCopy(pRCurve->m_P1);
			pBoundary->ma_Bulge.Add(0);
			pBoundary->AddCopy(pLCurve->m_P1);
			pBoundary->ma_Bulge.Add(pLCurve->GetBulge() * -1);
			pBoundary->AddCopy(pLCurve->m_P0);
			pBoundary->ma_Bulge.Add(0);
			pBoundary->AddCopy(pRCurve->m_P0);

			delete pRCurve;
			delete pLCurve;
		}
		else
		{
			GM2DCurve* pLCurve = GetHorShadeGBaseCurve();
			GM2DCurve* pRCurve = pLCurve->Copy();
			pRCurve->NormalTranslateMe(-dWidth);

			pBoundary->AddCopy(pRCurve->m_P0);
			pBoundary->ma_Bulge.Add(pRCurve->GetBulge());
			pBoundary->AddCopy(pRCurve->m_P1);
			pBoundary->ma_Bulge.Add(0);
			pBoundary->AddCopy(pLCurve->m_P1);
			pBoundary->ma_Bulge.Add(pLCurve->GetBulge() * -1);
			pBoundary->AddCopy(pLCurve->m_P0);
			pBoundary->ma_Bulge.Add(0);
			pBoundary->AddCopy(pRCurve->m_P0);

			delete pRCurve;
			delete pLCurve;
		}
	}

	pBoundary->MakeCCW();

	return true;
}

bool MSFWShadeG::GetShadeLoc(double& dSLoc, double& dELoc)
{
	MSWallMemberG* pWall = (MSWallMemberG*)m_pOwnerCompoG;
	double dWallLen = 0;
	if (pWall->isArc())
		dWallLen = m_IsLeftWallCurve ? pWall->mp_LLine->Length() : pWall->mp_RLine->Length();
	else
		dWallLen = pWall->GetLength();

	double dShadeWidth = IsVerticalShade() == true ? GetWidth() : m_dHorizontalShadeLength;

	if (dWallLen < dShadeWidth) return false;

	dSLoc = (m_dDistanceX) / dWallLen;
	dELoc = (m_dDistanceX + dShadeWidth) / dWallLen;

	return true;
}

double MSFWShadeG::GetVerticalLength()
{
	if (m_pOwnerCompoG == nullptr) return 0;

	MSWallMemberG* pWall = (MSWallMemberG*)m_pOwnerCompoG;
	if (pWall == nullptr) return 0;

	double wallHeight = pWall->GetHeight();
	return wallHeight - GetTopOffSet() - GetBotOffSet();
}

double MSFWShadeG::GetHorizontalShadeLength()
{
	if (IsVerticalShade() == true)
		return 0;

	return m_dHorizontalShadeLength;
}

void MSFWShadeG::SetHorizontalShadeLength(double value)
{
	m_dHorizontalShadeLength = value;
}

GM2DVector MSFWShadeG::GetStartPnt()
{
	GM2DCurve* pBaseCurve = nullptr;
	MSWallMemberG* pWall = (MSWallMemberG*)m_pOwnerCompoG;
	if (m_IsLeftWallCurve)
		pBaseCurve = pWall->mp_LLine->Copy();
	else
		pBaseCurve = pWall->mp_RLine->Copy();

	double dBaseCurveLen = pBaseCurve->Length();
	double dStartLoc = GetDistanceX() / dBaseCurveLen;

	GM2DVector SPnt = pBaseCurve->PointAtParam(dStartLoc);
	delete pBaseCurve;
	return SPnt;
}

bool MSFWShadeG::IsVerticalShade()
{
	if (mp_CompoM != nullptr)
	{
		MSFWShadeM* shadeM = dynamic_cast<MSFWShadeM*>(mp_CompoM);
		if (shadeM != nullptr) {
			bool returnVal = shadeM->IsVerticalShade();
			return returnVal;
		}
	}
	return true;
}

GM2DCurve* MSFWShadeG::GetHorShadeGBaseCurve()
{
	if (IsVerticalShade() == true)
		return nullptr;

	GM2DCurve* pHorShadeGBaseCurve = nullptr;
	MSWallMemberG* pWall = (MSWallMemberG*)m_pOwnerCompoG;
	double dBaseCurveLen = pWall->GetLength();
	if (pWall->isArc())
	{
		if (m_IsLeftWallCurve)
			dBaseCurveLen = pWall->mp_LLine->Length();
		else
			dBaseCurveLen = pWall->mp_RLine->Length();
	}
	
	double dStartLoc = GetDistanceX() / dBaseCurveLen;
	double dEndLoc = (GetDistanceX() + m_dHorizontalShadeLength) / dBaseCurveLen;
	if (m_IsLeftWallCurve)
		pHorShadeGBaseCurve = pWall->mp_LLine->Copy();
	else
		pHorShadeGBaseCurve = pWall->mp_RLine->Copy();

	GM2DVector SPnt = pHorShadeGBaseCurve->PointAtParam(dStartLoc);
	GM2DVector EPnt = pHorShadeGBaseCurve->PointAtParam(dEndLoc);
	pHorShadeGBaseCurve->SetP0(SPnt);
	pHorShadeGBaseCurve->SetP1(EPnt);

	return pHorShadeGBaseCurve;
}

double MSFWShadeG::GetDegree()
{
	MSWallMemberG* pWall = (MSWallMemberG*)m_pOwnerCompoG;
	if (pWall->isArc())
	{
		shared_ptr<GM2DCurve> pCurve = pWall->GetCenCurve();
		GM2DArc* pArc = (GM2DArc*)&*pCurve;
		if (pArc != nullptr)
		{
			double dDeg = pArc->m_Center.GetDegree(GetStartPnt());
			return dDeg + 90;
		}
	}

	return pWall->GetMemberDegree();
}

bool MSFWShadeG::CheckData(bool bMakeErrorObject)
{
	if (IsVerticalShade() == true && GetVerticalLength() <= 0)
	{		
		if(bMakeErrorObject)
		{
			CString strGroup = LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strErrMsg = _T("");			
			strErrMsg.Format(LocalDic::GetLocal(L"차양 수직길이가 0이하 입니다.", L"StructureModel"));
			MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 1, MESSAGE_LEVEL_WARNING, MESSAGE_WARNING_WALLOPENINGLOC, m_pOwnerCompoG);
		}
		return false;
	}
	bool IsOutLocation = false;
	double dSLoc= 2, dELoc = -1;
	bool bRavl = GetShadeLoc(dSLoc, dELoc);
	if (bRavl == false || dSLoc < 0 - DTOL5 || dELoc > 1 + DTOL5)
		IsOutLocation = true;

	MSWallMemberG *pWall = (MSWallMemberG*)m_pOwnerCompoG;
	double dFloorBotLevel = pWall->GetFloorLevel();
	double dFloorTopLevel = pWall->GetNextFloorLevel();		
	double dShadeTopLevel = 0, dShadeBotLevel = 0;
	GetZLevels(dShadeTopLevel, dShadeBotLevel);

	if ( MSModelChecker::CheckLength(dFloorTopLevel, _T("<"), dShadeTopLevel) == true
		|| MSModelChecker::CheckLength(dFloorTopLevel, _T("<"), dShadeBotLevel) == true
		|| MSModelChecker::CheckLength(dFloorBotLevel, _T(">"), dShadeTopLevel) == true
		|| MSModelChecker::CheckLength(dFloorBotLevel, _T(">"), dShadeBotLevel) == true)
		IsOutLocation = true;

	if(bMakeErrorObject && IsOutLocation)
	{
		CString strGroup = LocalDic::GetLocal(L"모델링 오류", L"StructureModel"), strErrMsg = _T("");			
		strErrMsg.Format(LocalDic::GetLocal(L"Wall(%%d) 밖에 차양이 존재 합니다.", L"StructureModel"));
		MSModelChecker::MakeErrorObject(strGroup, strErrMsg, 1, MESSAGE_LEVEL_WARNING, MESSAGE_WARNING_WALLOPENINGLOC, m_pOwnerCompoG);
	}
	return !IsOutLocation;
}
bool MSFWShadeG::GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal, bool bCheckLevelZone)
{
	double dFloorBotLevel = GetFloorLevel();
	double dFloorTopLevel = GetNextFloorLevel();
		
	BottomLevel = dFloorBotLevel+this->GetBotOffSet();
	TopLevel = this->IsVerticalShade() ? dFloorTopLevel+this->GetTopOffSet() : BottomLevel+this->GetHeight();			

	return true;
}
CString MSFWShadeG::GetKey(bool bUseID)
{
	// ID, FloorNum, Type, CompoM, 좌표, 옵셋
	CString strCompoMKey;
	if (mp_CompoM == NULL || mp_CompoM->m_ID == 0)
		strCompoMKey.Format(_T("%d"), 0);
	else
		strCompoMKey.Format(_T("%d"), &mp_CompoM->m_ID);

	double dSLoc = 0, dELoc = 1;
	GetShadeLoc(dSLoc, dELoc);		
	MSWallMemberG* pWall = (MSWallMemberG*)m_pOwnerCompoG;
	GM2DVector SVec(pWall->mp_LLine->PointAtParam(dSLoc));
	GM2DVector EVec(pWall->mp_LLine->PointAtParam(dELoc));

	CString strGeo;	
	strGeo.Format(_T("%d_%d_%d_%d_%d"), (int)m_IsLeftWallCurve, (int)SVec.m_X, (int)SVec.m_Y, (int)EVec.m_X, (int)EVec.m_Y);
	CString strOffSet;
	strOffSet.Format(_T("%d_%d_%d"), (int)IsSlopeMember(), (int)m_dBotOffset, (int)m_dTopOffset);
	
	CString strKey;
	strKey.Format(_T("%d_%d_%s_%s_%s"), GetFloorNum(), (int)GetType(), strCompoMKey, strGeo, strOffSet);
	if (bUseID)
	{
		CString strID;
		strID.Format(_T("%d_"), m_ID);
		strKey = strID + strKey;
	}

	return strKey;
}