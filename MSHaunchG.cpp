#include "stdafx.h"
#include "MSHaunchG.h"
#include "MSCompoM.h"
#include "MSFloor.h"
#include "MSRcHaunchM.h"
#include "MSBaseBuilding.h"
#include "MSMemberLevelDif.h"

IMPLEMENT_SERIAL(MSHaunchG, MSLineMemberG, VERSIONABLE_SCHEMA | 1)
MSHaunchG::MSHaunchG()
{
	m_QuantityType = eQuantityType::MatSlab;
	m_TopOffset = 0;
	m_BotOffset = 0;
}


MSHaunchG::~MSHaunchG()
{
}

MSObject* MSHaunchG::Copy(bool bNewID /*= true*/)
{
	MSHaunchG* pNewObj  = new MSHaunchG();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSHaunchG::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;
	MSHaunchG* pSource = dynamic_cast<MSHaunchG*>(pSource_in);
	if (pSource == nullptr)	return;

	if (!pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() != msHaunchG) { ASSERT(0); return; }

	m_QuantityType = pSource->GetQuantityType();
	m_TopOffset = pSource->GetTopOffset();
	m_BotOffset = pSource->GetBotOffset();

	MSLineMemberG::CopyFromMe(pSource_in, bNewID);

}

void MSHaunchG::Dump(EFS::EFS_Buffer& buffer)
{
	MSLineMemberG::Dump(buffer);
	int type = (int)m_QuantityType;
	buffer << type;
	buffer << m_TopOffset;
	buffer << m_BotOffset;
}

void MSHaunchG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSLineMemberG::Recovery(buffer);
	int type = 0;
	buffer >> type;
	m_QuantityType = (eQuantityType)type;
	buffer >> m_TopOffset;
	buffer >> m_BotOffset;
}
void MSHaunchG::Serialize(CArchive &ar)
{
	MSLineMemberG::Serialize(ar);
	bool bEachCompoM = false;
	int type = 0;
	if (ar.IsStoring())
	{
		if (mp_CompoM)
		{
			if (mp_CompoM->m_ID == 0)
				bEachCompoM = true;

			ar << bEachCompoM;
			if (bEachCompoM == true)
			{
				MSRcHaunchM* pHaunchM = (MSRcHaunchM*)mp_CompoM;

				long MType = pHaunchM->m_nType;

				ar << MType;

				ar << pHaunchM->GetMaterial()->m_ID;
				ar << pHaunchM->m_Name;
				ar << pHaunchM->GetDegree();
				ar << pHaunchM->GetDepth();
			}
		}
		type = (int)m_QuantityType;
		ar << type;
		ar << m_TopOffset;
		ar << m_BotOffset;
	}
	else
	{
		ar >> bEachCompoM;
		if (bEachCompoM)
		{
			MSBaseBuilding* pBldg = GetBuilding();
			long MType = 0;
			ar >> MType;

			MSRcHaunchM* pHaunchM = (MSRcHaunchM*)pBldg->CreateNewCompoM(MType);
			pHaunchM->m_nType = (MSCompoM::Type_MSCompoM)MType;

			long nMaterialID = 0;
			ar >> nMaterialID;
			pHaunchM->SetMaterial(pBldg->GetMaterial(nMaterialID));
			ar >> pHaunchM->m_Name;

			double dDegree = 0, dDepth = 0;
			ar >> dDegree;
			ar >> dDepth;

			pHaunchM->SetDegree(dDegree);
			pHaunchM->SetDepth(dDepth);

			pHaunchM->mp_Owner = pBldg;
			mp_CompoM = pHaunchM;
		}
		ar >> type;
		m_QuantityType = (eQuantityType)type;
		ar >> m_TopOffset;
		ar >> m_BotOffset;
	}
}

double MSHaunchG::GetWidth()
{
	MSCompoM* pCompoM = GetCompoM();
	MSRcHaunchM* pRcHaunchM = dynamic_cast<MSRcHaunchM*>(pCompoM);
	if (pRcHaunchM != nullptr)
	{
		double depth = pRcHaunchM->GetDepth() + m_TopOffset - m_BotOffset;
		double width = MSRcHaunchM::GetCalcWidth(depth, pRcHaunchM->GetDegree());

		if (width < 10)
		{
			ASSERT(0);
			return 10;
		}
		return width;
	}

    if(pCompoM != nullptr)
		return pCompoM->GetWidth();
	return 10;
}

void MSHaunchG::SetTopOffset(double offset)
{
	m_TopOffset = offset;
	UpdateCenCurveByCurveInput();
}

void MSHaunchG::SetBotOffset(double offset)
{
	m_BotOffset = offset;
	UpdateCenCurveByCurveInput();
}

vector<GM2DCurve*> MSHaunchG::GetLevelDifCurves()
{
	vector<GM2DCurve*> CurveArr;
	long nLevelDif = ma_MemberLevelDif.GetSize();
	if (nLevelDif == 0)
		return CurveArr;

	shared_ptr<GM2DCurve> pOrgMyCurve = GetCenCurve();
	GM2DCurve* pMyCurve = nullptr;
	if (m_eLineAlign == eLINEALIGN_TYPE::LINEALIGN_RIGHT)
		pMyCurve = mp_RLine->Copy();
	else
		pMyCurve = mp_LLine->Copy();

	GM2DVector SVec, EVec;
	SVec = pMyCurve->m_P0;

	for (long iLevelDif = 0; iLevelDif < nLevelDif; iLevelDif++)
	{
		MSMemberLevelDif* pLevelDif = ma_MemberLevelDif[iLevelDif];
		GM2DVector difVec = pOrgMyCurve->PointAtParam(pLevelDif->m_Loc);
		double myLoc = pMyCurve->OrthoBaseLocValue(difVec);
		EVec = pMyCurve->PointAtParam(myLoc);

		GM2DCurve* pCurCurve = pMyCurve->Copy();

		pCurCurve->SetP0(SVec);
		pCurCurve->SetP1(EVec);

		CurveArr.push_back(pCurCurve);
		SVec = EVec;
	}

	GM2DCurve* pCurCurve = pMyCurve->Copy();
	pCurCurve->SetP0(SVec);
	pCurCurve->SetP1(pMyCurve->m_P1);

	CurveArr.push_back(pCurCurve);

	if (m_eLineAlign != eLINEALIGN_TYPE::LINEALIGN_RIGHT)
	{
		for(int idx = 0 ; idx < CurveArr.size() ; idx++)
			CurveArr[idx]->ReverseDirection();
	}

	delete pMyCurve;

	return CurveArr;
}

GM2DPolyline* MSHaunchG::GetXMLBoundary()
{
	GM2DCurve* pMyCurve = nullptr;
	GM2DCurve* pUrCurve = nullptr;
	if (m_eLineAlign == eLINEALIGN_TYPE::LINEALIGN_RIGHT)
	{
		pMyCurve = mp_RLine->Copy();
		pUrCurve = mp_LLineTR->Copy();
	}
	else
	{
		pMyCurve = mp_LLine->Copy();
		pUrCurve = mp_RLineTR->Copy();
		pMyCurve->ReverseDirection();
		pUrCurve->ReverseDirection();
	}
	GM2DPolyline* pPoly = new GM2DPolyline();
	
	GM2DLineVector* pSLine = new GM2DLineVector(pMyCurve->m_P0, pUrCurve->m_P0);
	GM2DLineVector* pELine = new GM2DLineVector(pMyCurve->m_P1, pUrCurve->m_P1);

	pPoly->AddSegment(pPoly, pMyCurve);
	pPoly->AddSegment(pPoly, pELine);
	pPoly->AddSegment(pPoly, pUrCurve);
	pPoly->AddSegment(pPoly, pSLine);

	pPoly->MakeClosedPolyline(DTOL_GM);
	pPoly->MakeCCW();

	delete pMyCurve;
	delete pUrCurve;
	delete pSLine;
	delete pELine;

	return pPoly;
}

vector<GM2DPolyline*> MSHaunchG::GetGetLevelDifXMLBoundarys()
{
	vector<GM2DPolyline*> polyArr;
	long nLevelDif = ma_MemberLevelDif.GetSize();
	if (nLevelDif == 0)
		return polyArr;

	shared_ptr<GM2DCurve> pOrgMyCurve = GetCenCurve();
	GM2DCurve* pMyCurve = nullptr;
	GM2DCurve* pUrCurve = nullptr;
	if (m_eLineAlign == eLINEALIGN_TYPE::LINEALIGN_RIGHT)
	{
		pMyCurve = mp_RLine->Copy();
		pUrCurve = mp_LLineTR->Copy();
	}
	else
	{
		pMyCurve = mp_LLine->Copy();
		pUrCurve = mp_RLineTR->Copy();
		pMyCurve->ReverseDirection();
		pUrCurve->ReverseDirection();
	}

	GM2DVector mySVec, myEVec;
	mySVec = pMyCurve->m_P0;

	GM2DVector urSVec, urEVec;
	urSVec = pUrCurve->m_P0;

	for (long iLevelDif = 0; iLevelDif < nLevelDif; iLevelDif++)
	{
		MSMemberLevelDif* pLevelDif = ma_MemberLevelDif[iLevelDif];
		GM2DVector difVec = pMyCurve->PointAtParam(pLevelDif->m_Loc);
		double myLoc = pMyCurve->OrthoBaseLocValue(difVec);
		myEVec = pMyCurve->PointAtParam(myLoc);
		urEVec = pUrCurve->PointAtParam(myLoc);

		GM2DCurve* pCurMyCurve = pMyCurve->Copy();
		pCurMyCurve->SetP0(mySVec);
		pCurMyCurve->SetP1(myEVec);

		GM2DCurve* pCurUrCurve = pUrCurve->Copy();
		pCurUrCurve->SetP0(urSVec);
		pCurUrCurve->SetP1(urEVec);

		GM2DLineVector* pSLine = new GM2DLineVector(pCurMyCurve->m_P0, pCurUrCurve->m_P0);
		GM2DLineVector* pELine = new GM2DLineVector(pCurMyCurve->m_P1, pCurUrCurve->m_P1);

		GM2DPolyline* pPoly = new GM2DPolyline();
		pPoly->AddSegment(pPoly, pCurMyCurve);
		pPoly->AddSegment(pPoly, pELine);
		pPoly->AddSegment(pPoly, pCurUrCurve);
		pPoly->AddSegment(pPoly, pSLine);
		pPoly->MakeClosedPolyline(DTOL_GM);
		pPoly->MakeCCW();

		delete pCurMyCurve;
		delete pCurUrCurve;
		delete pSLine;
		delete pELine;
		
		mySVec = myEVec;
		urSVec = urEVec;

		polyArr.push_back(pPoly);
	}

	GM2DCurve* pCurMyCurve = pMyCurve->Copy();
	pCurMyCurve->SetP0(mySVec);
	pCurMyCurve->SetP1(pMyCurve->m_P1);
	GM2DCurve* pCurUrCurve = pUrCurve->Copy();
	pCurUrCurve->SetP0(urSVec);
	pCurUrCurve->SetP1(pUrCurve->m_P1);

	GM2DLineVector* pSLine = new GM2DLineVector(pCurMyCurve->m_P0, pCurUrCurve->m_P0);
	GM2DLineVector* pELine = new GM2DLineVector(pCurMyCurve->m_P1, pCurUrCurve->m_P1);

	GM2DPolyline* pPoly = new GM2DPolyline();
	pPoly->AddSegment(pPoly, pCurMyCurve);
	pPoly->AddSegment(pPoly, pELine);
	pPoly->AddSegment(pPoly, pCurUrCurve);
	pPoly->AddSegment(pPoly, pSLine);
	pPoly->MakeClosedPolyline(DTOL_GM);
	pPoly->MakeCCW();

	delete pCurMyCurve;
	delete pCurUrCurve;
	delete pSLine;
	delete pELine;
	polyArr.push_back(pPoly);

	delete pMyCurve;
	delete pUrCurve;

	return polyArr;
}