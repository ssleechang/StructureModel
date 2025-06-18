#include "StdAfx.h"
#include "MSWallOpeningG.h"
#include "MSOpeningM.h"
#include "MSDoorM.h"
#include "MSWallMemberG.h"
#include "MSOpeningM.h"
#include "MSFloor.h"

IMPLEMENT_SERIAL(MSWallOpeningG, MSCompoG, VERSIONABLE_SCHEMA | 1)
MSWallOpeningG::MSWallOpeningG(void)
{
    m_Reversed = false;
	m_IsUseFloorLevel = true;
	m_pOwnerCompoG = NULL;
}

MSWallOpeningG::~MSWallOpeningG(void)
{
}

void MSWallOpeningG::Dump(EFS::EFS_Buffer& buffer)
{
    MSCompoG::Dump(buffer);
    buffer << m_BaseOffset.m_X << m_BaseOffset.m_Y;
    buffer << m_Reversed;
	buffer << m_IsUseFloorLevel;
}

void MSWallOpeningG::Recovery(EFS::EFS_Buffer& buffer)
{
    MSCompoG::Recovery(buffer);
    buffer >> m_BaseOffset.m_X >> m_BaseOffset.m_Y;
    buffer >> m_Reversed;
	buffer >> m_IsUseFloorLevel;
}

void MSWallOpeningG::Serialize(CArchive &ar)
{
    MSCompoG::Serialize(ar);
    if(ar.IsStoring())
    {
        m_BaseOffset.Serialize(ar);
        ar << m_Reversed;
		ar << m_IsUseFloorLevel;
    }
    else
    {
        m_BaseOffset.Serialize(ar);
        ar >> m_Reversed;
		if (MSVersionInfo::GetCurrentVersion() >= 20150828)
			ar >> m_IsUseFloorLevel;
    }
}


void MSWallOpeningG::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;
	MSCompoG::CopyFromMe(pSource_in, bNewID);

	MSWallOpeningG* pSource = (MSWallOpeningG*)pSource_in;

    if(pSource)
    {
        //mp_CompoM = pSource->mp_CompoM;
        mp_Owner = pSource->mp_Owner;
        if(pSource->mp_Source)
            mp_Source = pSource->mp_Source;
        m_BaseOffset = pSource->m_BaseOffset;
        m_Reversed = pSource->m_Reversed;
		m_IsUseFloorLevel = pSource->m_IsUseFloorLevel;

		SetOwnerCompoG(pSource->m_pOwnerCompoG);
    }
}

void MSWallOpeningG::SetOwnerCompoG(MSElement* pOwnerCompoG)
{
	m_pOwnerCompoG = pOwnerCompoG;
}

bool MSWallOpeningG::MakeBoundary()
{
	MSWallMemberG* pWall = (MSWallMemberG*)m_pOwnerCompoG;
	double dSLoc = 0;
	double dELoc = 1;
	GetOpeningLoc(dSLoc, dELoc);
	GM2DPolyline* pBoundary = mp_Boundary;
	pBoundary->ResetPolyline();
	if (pWall->isArc() == false)
	{
		pBoundary->AddCopy(pWall->mp_RLine->PointAtParam(dSLoc));
		pBoundary->ma_Bulge.Add(0);
		pBoundary->AddCopy(pWall->mp_RLine->PointAtParam(dELoc));
		pBoundary->ma_Bulge.Add(0);
		pBoundary->AddCopy(pWall->mp_LLine->PointAtParam(dELoc));
		pBoundary->ma_Bulge.Add(0);
		pBoundary->AddCopy(pWall->mp_LLine->PointAtParam(dSLoc));
		pBoundary->ma_Bulge.Add(0);
		pBoundary->AddCopy(*pBoundary->ma_Point[0]);
	}
	else
	{
		GM2DCurve* pRCurve = pWall->mp_RLine->GetSegment(dSLoc, dELoc);
		GM2DCurve* pLCurve = pWall->mp_LLine->GetSegment(dSLoc, dELoc);

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
	pBoundary->MakeCCW();
	return true;
}

double MSWallOpeningG::GetHeightBySlabLevel(double dSlabLevel)
{
	MSOpeningM* pOpeningM = (MSOpeningM*)GetCompoM();
	/*
	if (pOpeningM->GetType() == MSCompoM::msDoorM)
	{
		MSDoorM* pDoorM = (MSDoorM*)pOpeningM;
		if(pDoorM->GetIsOpenToSlabLevel())
			return pOpeningM->GetHeightBySlabLevel(dSlabLevel) + m_BaseOffset.m_Y;
		else
			return pOpeningM->GetHeightBySlabLevel(dSlabLevel);
	}
	*/

	return pOpeningM->GetHeight();
}

void MSWallOpeningG::GetZLevel(double& dTopZLevel, double& dBotZLevel, bool bUseExtraSize, bool bMaxLevel, bool IsNotOverWall)
{
	MSWallMemberG* pOwnerWallG = (MSWallMemberG*)GetOwnerCompoG();
	if (pOwnerWallG == NULL)
	{
		ASSERT(0);
		return;
	}
	MSOpeningM* pOpeningM = (MSOpeningM*)GetCompoM();
	double dSLoc = 0;
	double dELoc = 0;

	double dExtraSize = 0;
	if (bUseExtraSize)
		dExtraSize = pOpeningM->GetExtraSize();
	double dCenLen = pOwnerWallG->GetCenLVec().Length();
	double dDist = m_BaseOffset.m_X - dExtraSize;


	if (m_Reversed)
	{
		dELoc = 1 - dDist / dCenLen;
		dSLoc = dELoc - ((pOpeningM->GetWidth() + 2 * dExtraSize) / (dCenLen));
	}
	else
	{
		dSLoc = dDist / dCenLen;
		dELoc = dSLoc + ((pOpeningM->GetWidth() + 2 * dExtraSize) / (dCenLen));
	}

	double dWallTopZLevelSLoc = 0;
	double dWallBotZLevelSLoc = 0;
	double dWallTopZLevelELoc = 0;
	double dWallBotZLevelELoc = 0;
	double dWallTopZLevel = 0;
	double dWallBotZLevel = 0;

	pOwnerWallG->GetZLevelsbyLoc(dWallTopZLevelSLoc, dWallBotZLevelSLoc, dSLoc);
	pOwnerWallG->GetZLevelsbyLoc(dWallTopZLevelELoc, dWallBotZLevelELoc, dSLoc);

	if (bMaxLevel)
	{
		dWallTopZLevel = max(dWallTopZLevelSLoc, dWallTopZLevelELoc);
		dWallBotZLevel = min(dWallBotZLevelSLoc, dWallBotZLevelELoc);
	}
	else
	{
		dWallTopZLevel = min(dWallTopZLevelSLoc, dWallTopZLevelELoc);
		dWallBotZLevel = max(dWallBotZLevelSLoc, dWallBotZLevelELoc);
	}


	MSFloor* pMSFloor = pOwnerWallG->GetFloor();
	double dCurSlabLevel = pMSFloor->GetSlabLevel();
	double dCurFloorLevel = pMSFloor->GetFloorLevel();
	double dSlabLevelOffset = dCurSlabLevel - dCurFloorLevel;

	if (m_IsUseFloorLevel)
		dBotZLevel = dCurFloorLevel + m_BaseOffset.m_Y - dExtraSize;
	else
		dBotZLevel = dCurSlabLevel + m_BaseOffset.m_Y - dExtraSize;

	dTopZLevel = dBotZLevel + pOpeningM->GetHeight() + 2 * dExtraSize;

	// 오프닝의 Top/Bot Level은 벽체의 Top/Bot Level을 초과할수 없음
	if (IsNotOverWall)
	{
		if (dBotZLevel < dWallBotZLevel)
			dBotZLevel = dWallBotZLevel;

		if (dTopZLevel < dWallBotZLevel)
			dTopZLevel = dWallBotZLevel;

		if (dBotZLevel > dWallTopZLevel)
			dBotZLevel = dWallTopZLevel;

		if (dTopZLevel > dWallTopZLevel)
			dTopZLevel = dWallTopZLevel;
	}

	if (pOpeningM->GetType() == MSCompoM::msDoorM)
	{
		MSDoorM* pDoorM = (MSDoorM*)pOpeningM;
		if (pDoorM->GetIsOpenToWallBottom())
			dBotZLevel = dWallBotZLevel;
	}
}

bool MSWallOpeningG::GetOpeningLoc(double& dSLoc, double& dELoc)
{
	MSWallMemberG* pWall = (MSWallMemberG*)m_pOwnerCompoG;
	double dWallLen = pWall->GetLength();
	double dOpeningWidth = GetWidth();
	double dExtraSize = GetExtraSize();

	if (dWallLen - (dOpeningWidth + (dExtraSize * 2)) < -DTOL_GM)
		return false;

	double sLen = m_BaseOffset.m_X - dExtraSize;
	double eLen = m_BaseOffset.m_X + dOpeningWidth + dExtraSize;
	if (fabs(sLen - dWallLen) < DTOL_GM)
		sLen = dWallLen;

	if (fabs(eLen - dWallLen) < DTOL_GM)
		eLen = dWallLen;

	dSLoc = sLen / dWallLen;
	dELoc = eLen / dWallLen;
	if (m_Reversed)
	{
		double dTmp = 1 - dSLoc;
		dSLoc = 1 - dELoc;
		dELoc = dTmp;
	}

	return true;
}

bool MSWallOpeningG::GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal /*= true*/, bool bCheckLevelZone /*= true*/)
{
	GetZLevel(TopLevel, BottomLevel, true, true, true);
	return false;
}

double MSWallOpeningG::GetExtraSize()
{
	MSOpeningM* pOpeningM = dynamic_cast<MSOpeningM*>(GetCompoM());
	if(pOpeningM != nullptr)
		return pOpeningM->GetExtraSize();

	return 0;
}
CString MSWallOpeningG::GetKey(bool bUseID)
{
	// ID, FloorNum, Type, CompoM, 좌표, 옵셋
	CString strCompoMKey;
	if (mp_CompoM == NULL || mp_CompoM->m_ID == 0)
		strCompoMKey.Format(_T("%d"), 0);
	else
		strCompoMKey.Format(_T("%d"), &mp_CompoM->m_ID);

	double dSLoc = 0, dELoc = 1;
	GetOpeningLoc(dSLoc, dELoc);		
	MSWallMemberG* pWall = (MSWallMemberG*)m_pOwnerCompoG;
	GM2DVector SVec(pWall->mp_LLine->PointAtParam(dSLoc));
	GM2DVector EVec(pWall->mp_LLine->PointAtParam(dELoc));

	CString strGeo;	
	strGeo.Format(_T("%d_%d_%d_%d"), (int)SVec.m_X, (int)SVec.m_Y, (int)EVec.m_X, (int)EVec.m_Y);
	CString strOffSet;
	strOffSet.Format(_T("%d_%d"), (int)IsSlopeMember(), (int)m_BaseOffset.m_Y);

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