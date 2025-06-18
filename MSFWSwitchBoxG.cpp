#include "stdafx.h"
#include "MSFWSwitchBoxG.h"
#include "MSFWSwitchBoxM.h"
#include "MSWallMemberG.h"

IMPLEMENT_SERIAL(MSFWSwitchBoxG, MSWallOpeningG, VERSIONABLE_SCHEMA | 1)

MSFWSwitchBoxG::MSFWSwitchBoxG()
{
	m_IsLeftOwnerCurve = true;
}

MSFWSwitchBoxG::~MSFWSwitchBoxG()
{
}

void MSFWSwitchBoxG::Dump(EFS::EFS_Buffer& buffer)
{
	MSWallOpeningG::Dump(buffer);
}

void MSFWSwitchBoxG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSWallOpeningG::Recovery(buffer);
}
void MSFWSwitchBoxG::Serialize(CArchive &ar)
{
	MSWallOpeningG::Serialize(ar);
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

		ar << m_IsLeftOwnerCurve;
	}
	else
	{
		ar >> bEachCompoM;
		if (bEachCompoM)
			Serialize_EachCompoM(ar);

		ar >> m_IsLeftOwnerCurve;
	}
}

MSObject* MSFWSwitchBoxG::Copy(bool bNewID /*= true*/)
{
	MSFWSwitchBoxG* pNewObj = new MSFWSwitchBoxG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSFWSwitchBoxG::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;
	if (!pSource_in->IsElement()) { ASSERT(0); return; }

	MSFWSwitchBoxG* pSourceSwitchBox = dynamic_cast<MSFWSwitchBoxG*>(pSource_in);
	if (pSourceSwitchBox != nullptr)
	{
		m_IsLeftOwnerCurve = pSourceSwitchBox->GetIsLeftOwnerCurve();
		MSWallOpeningG::CopyFromMe(pSource_in, bNewID);
	}
}

void MSFWSwitchBoxG::SetIsLeftOwnerCurve(bool value)
{
	m_IsLeftOwnerCurve = value;
}

bool MSFWSwitchBoxG::GetIsLeftOwnerCurve()
{
	return m_IsLeftOwnerCurve;
}


bool MSFWSwitchBoxG::MakeBoundary()
{
	MSWallMemberG* pWall = (MSWallMemberG*)m_pOwnerCompoG;
	double dSLoc = 0;
	double dELoc = 1;
	GetOpeningLoc(dSLoc, dELoc);
	GM2DPolyline* pBoundary = mp_Boundary;
	pBoundary->ResetPolyline();

	double dExLoc = 0;
	MSFWSwitchBoxM* pFWSwitchBoxM = dynamic_cast<MSFWSwitchBoxM*>(GetCompoM());
	if (pFWSwitchBoxM != nullptr)
		dExLoc = pFWSwitchBoxM->GetL() / pWall->GetLength();

	GM2DCurve* pRCurve = pWall->mp_RLine->Copy();
	GM2DCurve* pLCurve = pWall->mp_LLine->Copy();
	if (m_IsLeftOwnerCurve)
	{
		pRCurve->SetP0(pWall->mp_RLine->PointAtParam(dSLoc + dExLoc));
		pRCurve->SetP1(pWall->mp_RLine->PointAtParam(dELoc - dExLoc));

		pLCurve->SetP0(pWall->mp_LLine->PointAtParam(dSLoc));
		pLCurve->SetP1(pWall->mp_LLine->PointAtParam(dELoc));
	}
	else
	{
		pRCurve->SetP0(pWall->mp_RLine->PointAtParam(dSLoc));
		pRCurve->SetP1(pWall->mp_RLine->PointAtParam(dELoc));

		pLCurve->SetP0(pWall->mp_LLine->PointAtParam(dSLoc + dExLoc));
		pLCurve->SetP1(pWall->mp_LLine->PointAtParam(dELoc - dExLoc));
	}

	pBoundary->AddCopy(pRCurve->m_P0);
	pBoundary->AddCopy(pRCurve->m_P1);
	pBoundary->AddCopy(pLCurve->m_P1);
	pBoundary->AddCopy(pLCurve->m_P0);
	pBoundary->AddCopy(*pBoundary->ma_Point[0]);

	if (pWall->isArc() == false)
	{
		pBoundary->ma_Bulge.Add(0);
		pBoundary->ma_Bulge.Add(0);
		pBoundary->ma_Bulge.Add(0);
		pBoundary->ma_Bulge.Add(0);
	}
	else
	{
		pBoundary->ma_Bulge.Add(pRCurve->GetBulge());
		pBoundary->ma_Bulge.Add(0);
		pBoundary->ma_Bulge.Add(pLCurve->GetBulge() * -1);
		pBoundary->ma_Bulge.Add(0);
	}

	delete pRCurve;
	delete pLCurve;

	pBoundary->MakeCCW();
	return true;
}