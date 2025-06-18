#include "StdAfx.h"
#include "MSLinkMemberG.h"

#include "MSContainer.h"
#include "GMLib/GM2DLineVector.h"
#include "GMLib/GM2DArc.h"
#include "GMLib/GM2DPolyline.h"

#include "MSColumnG.h"
#include "MSFloor.h"
#include "MSIsoFootG.h"
#include "Logger.h"
#include "MSFloorQT.h"
#include "MSConnection.h"

//#include "GMLib/MSVersionInfo.h"
#include "MSRebarBuilding.h"
#include "MSLineLevelDifG.h"
#include "MSMemberLevelDif.h"
#include "MSLevelZone.h"
#include "MSConstructionZone.h"
#include "GMLib/MSVersionInfo.h"
#include "Radfcore_CLI/AppGlobalOptionWrapper.h"

#include "CmdProcess\CmdNewModifyVariable.h"
#include <stdint.h>
#include <memory>

double MSLinkMemberG::ms_dDefExtendLen = 10000.;

bool MSLinkMemberG::MakeTrimmedBoundary_Link2Link(MSLinkMemberG* pUrLink)
{
	if(IsParallel(pUrLink))
		return true;

	GM2DPolyline* pMyBoundary = GetTrimmedBoundary();
	GM2DPolyline* pUrBoundary = pUrLink->GetTrimmedBoundary();

	if(pMyBoundary->Contains(pUrBoundary))
		return true;
	if(pUrBoundary->Contains(pMyBoundary))
		return true;

	// jissi
	GM2DPolyline MyExtBoundary(TRUE);
	MakeExtendedBoundary(MyExtBoundary);
	
	GM2DPolyline UrExtBoundary(TRUE);
	pUrLink->MakeExtendedBoundary(UrExtBoundary);

	// find intersection area
	vector<GM2DPolyline*> aIntersect;
	MyExtBoundary.BoolOperation('I', &UrExtBoundary, aIntersect, MSFloor::ms_DefLineHitDistTol);
	long nIntersect = aIntersect.size();
	if (nIntersect<1)
		return false;
	
	// make My mp_Trimmed union
	MakeTrimmedUnion(aIntersect, pMyBoundary);

	// make Ur mp_Trimmed union
	MakeTrimmedUnion(aIntersect, pUrBoundary);
	GMObject::DeleteGMObject(aIntersect);
	return true;
}

void MSLinkMemberG::MakeTrimmedUnion(vector<GM2DPolyline*>& aIntersect, GM2DPolyline* pMyBoundary )
{
	// make mp_Trimmed union
	long nIntersect = aIntersect.size();
	for(long iIntersect=0;iIntersect<nIntersect;iIntersect++)
	{
		// check intersect with mp_Trimmed
		GM2DPolyline* pIntersect = aIntersect[iIntersect];
		CTypedPtrArray<CObArray, GM2DPolyline*> aMyIntersect;
		pMyBoundary->BoolOperation('I', pIntersect, aMyIntersect, 1.0);
		long nMyIntersect = aMyIntersect.GetCount();
		if (nMyIntersect<1)
			continue;

		// check union with mp_Trimmed
		CTypedPtrArray<CObArray, GM2DPolyline*> aMyUnion;
		pMyBoundary->BoolOperation('U', pIntersect, aMyUnion, 1.0);
		long nMyUnion = aMyUnion.GetCount();
		if (nMyUnion<1)
			continue;

		pMyBoundary->CopyFrom(aMyUnion[0]); // need to copy?
	}
}

GM2DPolyline* MSLinkMemberG::GetExtendedBoundary()
{
	if(GetTrimmedBoundary() == nullptr)
		InitTrimmedBoundary();

	return GetTrimmedBoundary();
}

void MSLinkMemberG::FindMiterLineWithOtherLinkG(MSLinkMemberG* link, GM2DLineVector& rLine)
{
	FindMiterLineWithOtherLinkG_NextStep(link, rLine, link->mp_LLine, link->mp_RLine);
	return;
}

void MSLinkMemberG::FindMiterLineWithOtherLinkG_NextStep(MSLinkMemberG* link, GM2DLineVector& rLine, GM2DCurve* pLCurve, GM2DCurve* pRCurve)
{
	GM2DLineVector CenLVec = GetCenLVec();
	GM2DLineVector otherLine = link->GetCenLVec();

	double dist_min = 0;
	double ang = 0;
	GM2DVector mid;
	int idx_i = 0;
	int idx_k = 0;
	for (int i = 0; i < 2; ++i)
	{
		GM2DVector& myP0 = i == 0 ? CenLVec.m_P0 : CenLVec.m_P1;
		GM2DVector& myP1 = i == 0 ? CenLVec.m_P1 : CenLVec.m_P0;
		for (int k = 0; k < 2; ++k)
		{
			GM2DVector& urP0 = k == 0 ? otherLine.m_P0 : otherLine.m_P1;
			GM2DVector& urP1 = k == 0 ? otherLine.m_P1 : otherLine.m_P0;
			double dist = myP0.Distance(urP0);
			if ((i == 0 && k == 0) || dist < dist_min)
			{
				idx_i = i;
				idx_k = k;
				dist_min = dist;

				double myLoc1 = 0, urLoc1 = 0;
				long nRval = CenLVec.IntersectEndWithLine(otherLine, mid, &myLoc1, &urLoc1);
				if (nRval != 1 && nRval != 2) // 1점 교차 또는  외부 교차가 안될 경우 각점의 중간 점으로 셋팅한다.
				{
					mid.m_X = (myP0.m_X + urP0.m_X) / 2.0;
					mid.m_Y = (myP0.m_Y + urP0.m_Y) / 2.0;
				}
				double ang1 = (myP1 - myP0).GetRadian();
				double ang2 = (urP0 - urP1).GetRadian();
				ang = (ang1 + ang2) / 2.0;
			}
		}
	}

	ang += PI*0.5;
	if (ang > PI*2.0) ang -= PI*2.0;
	rLine.SetP0(mid);
	GM2DVector newP1(mid.m_X + 1000.0*cos(ang), mid.m_Y + 1000.0*sin(ang));
	rLine.SetP1(newP1);

	/////////////////////////////////////////////////////////////////////////////
	// New Logic - Connect 2 Points (LRLine Meet)
	GM2DCurve* out1 = pLCurve;
	GM2DCurve* out2 = (idx_i == idx_k ? pRCurve : pLCurve);
	GM2DCurve* in1 = pRCurve;
	GM2DCurve* in2 = (idx_i == idx_k ? pLCurve : pRCurve);

	GM2DLineVector* line1 = dynamic_cast<GM2DLineVector*>(out1);
	GM2DLineVector* line2 = dynamic_cast<GM2DLineVector*>(out2);
	GM2DLineVector* line3 = dynamic_cast<GM2DLineVector*>(in1);
	GM2DLineVector* line4 = dynamic_cast<GM2DLineVector*>(in2);
	GM2DVector vCross1, vCross2;
	if (line1 != NULL && line2 != NULL && line3 != NULL && line4 != NULL)
	{
		double myLoc1 = 0, myLoc2 = 0;
		double urLoc1 = 0, urLoc2 = 0;
		GM2DVector vCross1;
		line1->IntersectEndWithLine(*line2, vCross1, &myLoc1, &urLoc1);
		line3->IntersectEndWithLine(*line4, vCross2, &myLoc2, &urLoc2);

		bool error = false;
		double TOL = 0.1; // Tolerance Ratio
		if (idx_i == 0 && (myLoc1 > 1.0 - TOL || myLoc2 > 1.0 - TOL)) error = true;
		if (idx_i == 1 && (myLoc1 < TOL || myLoc2 < TOL)) error = true;
		if (idx_k == 0 && (urLoc1 > 1.0 - TOL || urLoc2 > 1.0 - TOL)) error = true;
		if (idx_k == 1 && (urLoc1 < TOL || urLoc2 < TOL)) error = true;

		// 끝점에서 dist_diff_tol 이상 벗어난 곳에서 선이 생기면 위 기본로직 사용
		double dMyEndlineDist = 0.0, dUrEndlineDist = 0.0;
		if (idx_i == 0) dMyEndlineDist = fabs((myLoc1 + myLoc2) / 2.0) * CenLVec.Length();
		if (idx_i == 1) dMyEndlineDist = fabs(1 - (myLoc1 + myLoc2) / 2.0) * CenLVec.Length();
		if (idx_k == 0) dUrEndlineDist = fabs((urLoc1 + urLoc2) / 2.0) * otherLine.Length();
		if (idx_k == 1) dUrEndlineDist = fabs(1 - (urLoc1 + urLoc2) / 2.0) * otherLine.Length();
		double dist_diff_tol = 200.0;
		if (dMyEndlineDist > dist_diff_tol || dUrEndlineDist > dist_diff_tol)
			error = true;

		// 기본 로직 방법보다 ang_diff_tol 이상 틀어지면 위 기본로직 사용
		double ang_diff_tol = 30.0;
		double inner_product = fabs(rLine.UnitVector()*(vCross2 - vCross1).UnitVector());
		if (inner_product < cos(ang_diff_tol / 180.0*PI))
			error = true;

		if (!error)
		{
			rLine.SetP0(vCross1);
			rLine.SetP1(vCross2);
		}
	}
	/////////////////////////////////////////////////////////////////////////////
}

bool MSLinkMemberG::MakeExtendedBoundary_ToLine(GM2DPolyline& pExtendedBoundary, GM2DLineVector& line)
{
	bool bFirst = (GetTrimmedBoundary() == NULL); // only first - use org LR lines
	GM2DCurve* pLCurve = !bFirst && mp_LLineTR != NULL ? mp_LLineTR : mp_LLine;
	GM2DCurve* pRCurve = !bFirst && mp_RLineTR != NULL ? mp_RLineTR : mp_RLine;

	return MakeExtendedBoundary_ToLine_NextStep(pExtendedBoundary, line, pLCurve, pRCurve);
}

bool MSLinkMemberG::MakeExtendedBoundary_ToLine_NextStep(GM2DPolyline& pExtendedBoundary, GM2DLineVector& line, GM2DCurve* pLCurve, GM2DCurve* pRCurve)
{
	if (isArc() == false)
	{
		GM2DLineVector CenLVec = GetCenLVec();
		bool bFirst = (GetTrimmedBoundary() == NULL); // only first - use org LR lines
		if (line.Distance(CenLVec.m_P1) < line.Distance(CenLVec.m_P0))
		{
			pExtendedBoundary.AddCopy(pRCurve->m_P0);
			pExtendedBoundary.ma_Bulge.Add(0);

			GM2DVectors CrossVecs;
			CArray<double, double> MyLocArr, UrLocArr;
			pRCurve->Intersect(&line, CrossVecs, MyLocArr, UrLocArr, false);
			if (CrossVecs.GetSize() > 0)
			{
				pExtendedBoundary.AddCopy(*CrossVecs[0]);
				pExtendedBoundary.ma_Bulge.Add(0);
			}
			else
			{
				pExtendedBoundary.AddCopy(pRCurve->m_P1);
				pExtendedBoundary.ma_Bulge.Add(0);
			}

			pLCurve->Intersect(&line, CrossVecs, MyLocArr, UrLocArr, false);
			if (CrossVecs.GetSize() > 0)
			{
				pExtendedBoundary.AddCopy(*CrossVecs[0]);
				pExtendedBoundary.ma_Bulge.Add(0);
			}
			else
			{
				pExtendedBoundary.AddCopy(pLCurve->m_P0);
				pExtendedBoundary.ma_Bulge.Add(0);
			}

			pExtendedBoundary.AddCopy(pLCurve->m_P0);
			pExtendedBoundary.ma_Bulge.Add(0);

			pExtendedBoundary.MakeClosedPolyline(DTOL_GM);
		}
		else
		{
			GM2DVectors CrossVecs;
			CArray<double, double> MyLocArr, UrLocArr;
			pRCurve->Intersect(&line, CrossVecs, MyLocArr, UrLocArr, false);
			if (CrossVecs.GetSize() > 0)
			{
				pExtendedBoundary.AddCopy(*CrossVecs[0]);
				pExtendedBoundary.ma_Bulge.Add(0);
			}
			else
			{
				pExtendedBoundary.AddCopy(pRCurve->m_P0);
				pExtendedBoundary.ma_Bulge.Add(0);
			}


			pExtendedBoundary.AddCopy(pRCurve->m_P1);
			pExtendedBoundary.ma_Bulge.Add(0);

			pExtendedBoundary.AddCopy(pLCurve->m_P1);
			pExtendedBoundary.ma_Bulge.Add(0);

			pLCurve->Intersect(&line, CrossVecs, MyLocArr, UrLocArr, false);
			if (CrossVecs.GetSize() > 0)
			{
				pExtendedBoundary.AddCopy(*CrossVecs[0]);
				pExtendedBoundary.ma_Bulge.Add(0);
			}
			else
			{
				pExtendedBoundary.AddCopy(pLCurve->m_P0);
				pExtendedBoundary.ma_Bulge.Add(0);
			}

			pExtendedBoundary.MakeClosedPolyline(DTOL_GM);
		}
	}

	pExtendedBoundary.MakeCCW();

	return true;
}

//this의 안쪽라인이 other의 바깥쪽 라인과 만나는 점에서 수직으로 끊는다.
void MSLinkMemberG::MakeSquareEnd(MSLinkMemberG* link_other)
{
	if (isArc() == false && link_other->isArc() == false)
	{
		GM2DPolyline NewBoundary(TRUE);

		GM2DLineVector CenLVec = GetCenLVec();
		GM2DLineVector OtherCenLVec = link_other->GetCenLVec();
		GM2DVector thisCenter = CenLVec.PointAtParam(0.5);
		GM2DVector otherCenter = OtherCenLVec.PointAtParam(0.5);

		GM2DLineVector* insideLine = NULL;
		GM2DLineVector* outsideLine = NULL;

		if (mp_LLineTR == NULL)
			mp_LLineTR = mp_LLine->Copy();

		if (mp_RLineTR == NULL)
			mp_RLineTR = mp_RLine->Copy();

		if (this->mp_LLine->Distance(otherCenter) < this->mp_RLine->Distance(otherCenter))
		{
			insideLine = dynamic_cast<GM2DLineVector*>(mp_LLineTR);
			outsideLine = dynamic_cast<GM2DLineVector*>(mp_RLineTR);
		}
		else
		{
			insideLine = dynamic_cast<GM2DLineVector*>(mp_RLineTR);
			outsideLine = dynamic_cast<GM2DLineVector*>(mp_LLineTR);
		}

		GM2DLineVector* otherInsideLine = NULL;
		GM2DLineVector* otherOutsideLine = NULL;
		if (link_other->mp_LLine->Distance(thisCenter) < link_other->mp_RLine->Distance(thisCenter))
		{
			otherInsideLine = dynamic_cast<GM2DLineVector*>(link_other->mp_LLine);
			otherOutsideLine = dynamic_cast<GM2DLineVector*>(link_other->mp_RLine);
		}
		else
		{
			otherInsideLine = dynamic_cast<GM2DLineVector*>(link_other->mp_RLine);
			otherOutsideLine = dynamic_cast<GM2DLineVector*>(link_other->mp_LLine);
		}


		if (insideLine == nullptr || outsideLine == nullptr)	return;
		if (otherInsideLine == nullptr || otherOutsideLine == nullptr) return;
		double myLoc = 0, urLoc = 0;

		GM2DVector insideEndPnt;
		if (insideLine->IntersectEndWithLine(*otherOutsideLine, insideEndPnt, &myLoc, &urLoc) <= 0)//should be 1 or 2
			return;

		ReplaceNearEndPoint(insideLine, insideEndPnt);

		GM2DVector outsideEndPnt = outsideLine->OrthoBaseVector(insideEndPnt);
		ReplaceNearEndPoint(outsideLine, outsideEndPnt);

		GM2DPolyline trimmedBoundary = MakeBoundaryFrom2Lines(insideLine, outsideLine);
		
		this->GetTrimmedBoundary()->CopyFrom(&trimmedBoundary);
	}
}

GM2DPolyline MSLinkMemberG::MakeBoundaryFrom2Lines(GM2DLineVector* insideLine, GM2DLineVector* outsideLine)
{
	GM2DPolyline trimmedBoundary(TRUE);
	trimmedBoundary.AddCopy(insideLine->m_P0);
	trimmedBoundary.ma_Bulge.Add(0);

	trimmedBoundary.AddCopy(insideLine->m_P1);
	trimmedBoundary.ma_Bulge.Add(0);

	trimmedBoundary.AddCopy(outsideLine->m_P1);
	trimmedBoundary.ma_Bulge.Add(0);

	trimmedBoundary.AddCopy(outsideLine->m_P0);
	trimmedBoundary.ma_Bulge.Add(0);

	trimmedBoundary.MakeClosedPolyline(DTOL_GM);

	trimmedBoundary.MakeCCW();

	return trimmedBoundary;
}


void MSLinkMemberG::ReplaceNearEndPoint(GM2DLineVector* Line, GM2DVector &EndPoint)
{
	if (Line->m_P0.Distance(EndPoint) < Line->m_P1.Distance(EndPoint))
		Line->m_P0.SetXY(EndPoint.m_X, EndPoint.m_Y);
	else
		Line->m_P1.SetXY(EndPoint.m_X, EndPoint.m_Y);
}

//other의 안쪽 라인과 만나는 점에서 양쪽 라인을 끊고 연결한다.
void MSLinkMemberG::MakeCutEnd(MSLinkMemberG* link_other)
{
	if (isArc() == false && link_other->isArc() == false)
	{
		GM2DPolyline NewBoundary(TRUE);

		GM2DLineVector CenLVec = GetCenLVec();
		GM2DVector thisCenter = CenLVec.PointAtParam(0.5);
		
		if (mp_LLineTR == NULL)
			mp_LLineTR = mp_LLine->Copy();

		if (mp_RLineTR == NULL)
			mp_RLineTR = mp_RLine->Copy();

		GM2DLineVector* sideLineL = dynamic_cast<GM2DLineVector*>(mp_LLineTR);
		GM2DLineVector* sideLineR = dynamic_cast<GM2DLineVector*>(mp_RLineTR);

		
		GM2DLineVector* otherInsideLine = NULL;
		
		if (link_other->mp_LLine->Distance(thisCenter) < link_other->mp_RLine->Distance(thisCenter))
		{
			otherInsideLine = dynamic_cast<GM2DLineVector*>(link_other->mp_LLine);
		
		}
		else
		{
			otherInsideLine = dynamic_cast<GM2DLineVector*>(link_other->mp_RLine);
		}



		double myLoc = 0, urLoc = 0;
		if (otherInsideLine == nullptr)	return;
		GM2DVector cutPointL, cutPointR;
		if (otherInsideLine->IntersectEndWithLine(*sideLineL, cutPointL, &myLoc, &urLoc) <= 0)//should be 1 or 2
			return;
		if (otherInsideLine->IntersectEndWithLine(*sideLineR, cutPointR, &myLoc, &urLoc) <= 0)//should be 1 or 2
			return;

		ReplaceNearEndPoint(sideLineL, cutPointL);
		ReplaceNearEndPoint(sideLineR, cutPointR);

		GM2DPolyline trimmedBoundary = MakeBoundaryFrom2Lines(sideLineL, sideLineR);

		this->GetTrimmedBoundary()->CopyFrom(&trimmedBoundary);
	}
	
}

bool MSLinkMemberG::MakeExtendedBoundary(GM2DPolyline& pExtendedBoundary, long nExtendType, double dExtendLen)
{
	return MakeExtendedBoundary_NextStep(pExtendedBoundary, mp_LLine, mp_RLine, nExtendType, dExtendLen);
}

bool MSLinkMemberG::MakeExtendedBoundary_NextStep(GM2DPolyline& pExtendedBoundary, GM2DCurve* pLCurve, GM2DCurve* pRCurve, long nExtendType, double dExtendLen)
{
	pExtendedBoundary.ResetPolyline();

	GM2DLineVector CenLVec = GetCenLVec();
	GM2DVector DirVec = CenLVec.UnitVector();

	double dWidth = GetWidth();
	double ExtendLength = 0;
	if (fabs(dExtendLen) < DTOL_GM)
		ExtendLength = max(MSLinkMemberG::ms_dDefExtendLen, CenLVec.Length());
	else
		ExtendLength = dExtendLen;
	DirVec = DirVec * ExtendLength;

	if (isArc() == false)
	{
		if (nExtendType == 0 || nExtendType == -1)
		{
			pExtendedBoundary.AddCopy(pRCurve->m_P0 - DirVec);
			pExtendedBoundary.ma_Bulge.Add(0);
		}
		else
		{
			pExtendedBoundary.AddCopy(pRCurve->m_P0);
			pExtendedBoundary.ma_Bulge.Add(0);
		}

		if (nExtendType == 0 || nExtendType == 1)
		{
			pExtendedBoundary.AddCopy(pRCurve->m_P1 + DirVec);
			pExtendedBoundary.ma_Bulge.Add(0);
		}
		else
		{
			pExtendedBoundary.AddCopy(pRCurve->m_P1);
			pExtendedBoundary.ma_Bulge.Add(0);
		}

		if (nExtendType == 0 || nExtendType == 1)
		{
			pExtendedBoundary.AddCopy(pLCurve->m_P1 + DirVec);
			pExtendedBoundary.ma_Bulge.Add(0);
		}
		else
		{
			pExtendedBoundary.AddCopy(pLCurve->m_P1);
			pExtendedBoundary.ma_Bulge.Add(0);
		}

		if (nExtendType == 0 || nExtendType == -1)
		{
			pExtendedBoundary.AddCopy(pLCurve->m_P0 - DirVec);
			pExtendedBoundary.ma_Bulge.Add(0);
		}
		else
		{
			pExtendedBoundary.AddCopy(pLCurve->m_P0);
			pExtendedBoundary.ma_Bulge.Add(0);
		}
		pExtendedBoundary.MakeClosedPolyline(DTOL_GM);
	}
	else
	{
		GetBoundary(pExtendedBoundary);
	}
	pExtendedBoundary.MakeCCW();
	return true;
}

void MSLinkMemberG::BuildBoundaryByLevel( vector<GM2DPolyline>& BoundaryByLevelArr )
{

}

void MSLinkMemberG::GetOppositeRect( GM2DVector vec, GM2DPolyline &OppositeRect )
{
	long side = WhichSide(vec)*(-1);
	shared_ptr<GM2DCurve> pCenCurve = GetCenCurve();
	double dExtendLen = MSLinkMemberG::ms_dDefExtendLen;
	double dExtendLen2 = MSLinkMemberG::ms_dDefExtendLen * 10;
	if(isArc())
	{
		GM2DArc* pCenArc = (GM2DArc*)&*pCenCurve;
		GM2DArc* pNearLine = (GM2DArc*)pCenArc->Copy();
		pNearLine->NormalTranslateMe(GetWidth()*0.5*side);
		GM2DArc* pFarLine = (GM2DArc*)pCenArc->Copy();
		pFarLine->NormalTranslateMe(dExtendLen2*side);

		OppositeRect.ResetPolyline();
		OppositeRect.AddCopy(pNearLine->m_P0);
		OppositeRect.ma_Bulge.Add(pNearLine->GetBulge());
		OppositeRect.AddCopy(pNearLine->m_P1);
		OppositeRect.ma_Bulge.Add(0.);
		OppositeRect.AddCopy(pFarLine->m_P1);
		OppositeRect.ma_Bulge.Add(-pFarLine->GetBulge());
		OppositeRect.AddCopy(pFarLine->m_P0);
		OppositeRect.ma_Bulge.Add(0.);

		delete pNearLine;
		delete pFarLine;
	}
	else
	{
		GM2DLineVector* CenterLine = (GM2DLineVector*)&*pCenCurve;
		CenterLine->ExtendLineVector(TRUE, dExtendLen, FALSE);
		CenterLine->ExtendLineVector(FALSE, dExtendLen, TRUE);
		GM2DLineVector NearLine = CenterLine->NormalTranslate(GetWidth()*0.5*side);
		GM2DLineVector FarLine = CenterLine->NormalTranslate(dExtendLen2*side);

		OppositeRect.ResetPolyline();
		OppositeRect.AddCopy(NearLine.m_P0);
		OppositeRect.ma_Bulge.Add(0.);
		OppositeRect.AddCopy(NearLine.m_P1);
		OppositeRect.ma_Bulge.Add(0.);
		OppositeRect.AddCopy(FarLine.m_P1);
		OppositeRect.ma_Bulge.Add(0.);
		OppositeRect.AddCopy(FarLine.m_P0);
		OppositeRect.ma_Bulge.Add(0.);
	}

	OppositeRect.MakeClosedPolyline(DTOL_GM);
	if (OppositeRect.IsCCW() == false)
		OppositeRect.MakeCCW();
}

void MSLinkMemberG::RemoveOutwardWedge()
{
	GM2DPolyline MyOrgBoundary(TRUE);
	GetBoundary(MyOrgBoundary);
	GM2DPolyline* pMyBoundary = GetTrimmedBoundary();
	shared_ptr<GM2DCurve> pMyCenterCurve = GetCenCurve();
	GM2DVector MyMidVec = pMyCenterCurve->PointAtParam(0.5);

	GM2DLineVector SLine,ELine;
	shared_ptr<GM2DCurve> LLine, RLine;
	GetTrimmedLRLines(LLine, RLine, false);
	SLine.m_P0 = LLine->m_P0;
	SLine.m_P1 = RLine->m_P0;
	ELine.m_P0 = LLine->m_P1;
	ELine.m_P1 = RLine->m_P1;

	// Others 찾기
	long nOverlap = this->ma_Connections.size();

	for (long jOverlap=0; jOverlap<nOverlap; jOverlap++)
	{
		MSConnection* pOtherOverlap = this->ma_Connections[jOverlap];
		MSCompoG* pOtherG = pOtherOverlap->GetPartner(this);
		if (pOtherG == nullptr || false== pOtherG->IsLinkMember())
			continue;

		if (pOtherOverlap->IsLevelTouch()) continue;

		if(pOtherOverlap->GetTrimOption(this) != MSConnection::TO_EXTENDCUT && pOtherOverlap->GetTrimOption(this) != MSConnection::TO_MITER)
			continue;

		if(pOtherOverlap->GetLinkMemberConPos(this) == CON_MID)
			continue;

		GM2DPolyline OverlapPoly(TRUE);
		pOtherOverlap->GetOverlapPoly(OverlapPoly);

		MSLinkMemberG* pOtherLinkG = (MSLinkMemberG*)pOtherG;
		GM2DPolyline OppositeRect(TRUE);
		pOtherLinkG->GetOppositeRect(MyMidVec, OppositeRect);
			
		long nSRval0 = OppositeRect.Contains(&SLine.m_P0, DTOL_MODELING);
		long nSRval1 = OppositeRect.Contains(&SLine.m_P1, DTOL_MODELING);
		long nERval0 = OppositeRect.Contains(&ELine.m_P0, DTOL_MODELING);
		long nERval1 = OppositeRect.Contains(&ELine.m_P1, DTOL_MODELING);
			
// 			if(! ((nSRval0 != nSRval1)  || (nERval0 != nERval1)) )
// 				continue;
			
		// SLine, ELine이 OppositeRect에 1점만 포함될경우 삭제한다.
		// ==> 둘다 포함 또는 둘다 미포함은 처리 안함
		if(nSRval0 == nSRval1 && nERval0 == nERval1)
		{
			// 첫번째 선이 NearLine 이다. Org의 LLine RLine의 방향성 검토
			GM2DLineVector TheLine = OppositeRect.GetLine(0);
			long nLSide0 = TheLine.WhichSide(mp_LLine->m_P0);
			long nLSide1 = TheLine.WhichSide(mp_LLine->m_P1);
			long nRSide0 = TheLine.WhichSide(mp_RLine->m_P0);
			long nRSide1 = TheLine.WhichSide(mp_RLine->m_P1);

			long nMidSide = TheLine.WhichSide(MyMidVec);
				
			// 나의 MidVec의 위치가 Org LLine RLine와 같은 방향에 있으면 삭제한다.
			if(nMidSide != nLSide0 || nMidSide != nLSide1 || nMidSide != nRSide0 || nMidSide != nRSide1)
				continue;
		}
			
		vector<GM2DPolyline*> WedgePoly;
		OppositeRect.BoolOperation('I', &OverlapPoly, WedgePoly, DTOL_GM);
		long nWedge = WedgePoly.size();
		if(nWedge < 1)
			continue;

		GM2DPolyline Wedge(TRUE);
		Wedge.CopyFrom(WedgePoly[0]);
		GMObject::DeleteGMObject(WedgePoly);

		// 삭제 판단
		//long WedgeWhichSide = pLinkG->WhichSide(Wedge.MassCenter());
		//if(PartnerWhichSide != WedgeWhichSide)
		{
			// 잘라내기
			CTypedPtrArray<CObArray, GM2DPolyline*> JoinedPolys;
			pMyBoundary->BoolOperation('A', &OppositeRect, JoinedPolys, DTOL_GM);
			if (JoinedPolys.GetSize() > 0)
				pMyBoundary->CopyFrom(JoinedPolys[0]);
			GMObject::DeleteGMObject(JoinedPolys);
		}
	}
}

void MSLinkMemberG::UpdateBoundaryVisibilities(long nUpdateConnIdx)
{
	long nOverlap = this->ma_Connections.size();
	for (long iOverlap = nUpdateConnIdx; iOverlap<nOverlap; iOverlap++)
	{
		MSConnection* pConnection = this->ma_Connections[iOverlap];
		MSCompoG* pPartnerG = pConnection->GetPartner(this);
		if(pPartnerG == nullptr)
		{
			ASSERT(0);
			continue;
		}
		if(IsSlopeMember() == true && pPartnerG->IsSlopeMember() == true)
			continue;

		if (pPartnerG->IsPolyMember()) continue;
	
		double dTol = DTOL_MODELING;
		GM2DPolyline ThePoly1, ThePoly2;
		GM2DPolyline* pMyBoundary = &ThePoly1;
		GM2DPolyline* pUrBoundary = &ThePoly2;
		// 포인터 부재의 경우 TrimedBoundary를 사용하지 않는다.(주두부 계산때문..) 
		MSCompoG::GetMyUrBoundary(this, pPartnerG, pMyBoundary, pUrBoundary);
		// 같은층 부재, 층 부재는 전부 세그먼트를 나눔, 수직 부재는 수직부재끼리만 세크먼트를 나눔
// 		if(GetFloor() == pPartnerG->GetFloor() &&
// 			(IsFloorMember() || IsVerticalMember() && pPartnerG->IsVerticalMember()) )

		if (AppGlobalOptionWrapper::Instance()->GetAppMode() == AppGlobalOptionWrapper::Formwork)
		{
			// 층부재 이면서 라인부재와 만나는 수직 부재는 절점을 생성하지 않는다.
			if (IsVerticalMember() && pPartnerG->IsLinkMember() && pPartnerG->IsFloorMember())
				continue;
		}
		pMyBoundary->SegmentstoMe(pUrBoundary, dTol);
			
	}
	InitTrimmedBoundaryVisibility();

	for (long iOverlap = nUpdateConnIdx; iOverlap<nOverlap; iOverlap++)
	{
		MSConnection* pConnection = this->ma_Connections[iOverlap];
		MSCompoG* pPartnerG = pConnection->GetPartner(this);
		if(pPartnerG == nullptr)
		{
			ASSERT(0);
			continue;
		}
		
		if (pPartnerG->IsPolyMember()) continue;

		UpdateBoundaryVisibilities(pPartnerG, pConnection);
	}

	MSCompoG::UpdateBoundaryVisibilities(nUpdateConnIdx);
}

long MSLinkMemberG::GetZLevelInfo(MSCompoG* pUrCompoG, MSConnection* pConnection, bool bCheckConnection)
{
	//pConnection 이 있는 상태에서만 사용함

	bool bCheckLevelZone = false;
	MSFloor* pMyFloor = GetFloor();
	MSFloor* pUrFloor = pUrCompoG->GetFloor();
	if(pMyFloor != pUrFloor)
		bCheckLevelZone = true;
	// 체크할 부재의 층이 다를 경우 bCheckLevelZone을 True로 해준다.

	long nRval = 0;

	bool IsUrLinkMember = pUrCompoG->IsLinkMember(); 
	double MyTopLevel=0, MyBotLevel=0;
	double UrTopLevel=0, UrBotLevel=0;
	double dMyLoc = pConnection->GetLinkMemberLoc(this);
	double dUrLoc = pConnection->GetLinkMemberLoc(pUrCompoG);

	if(pConnection->IsParallel())
	{
		GetZLevels(MyTopLevel, MyBotLevel, false, bCheckLevelZone);
		pUrCompoG->GetZLevels(UrTopLevel, UrBotLevel, false, bCheckLevelZone);
	}
	else
	{
		GetZLevelsbyLoc(MyTopLevel, MyBotLevel, dMyLoc);
		
		if(IsUrLinkMember)
		{
			MSLinkMemberG* pUrLinkG = (MSLinkMemberG*)pUrCompoG;
			pUrLinkG->GetZLevelsbyLoc(UrTopLevel, UrBotLevel, dUrLoc);
		}
		else
		{
			pUrCompoG->GetZLevels(UrTopLevel, UrBotLevel, false);
		}
	}

	if(bCheckConnection == true)
	{
		if(fabs(MyBotLevel - UrTopLevel) < DTOL1 || (fabs(UrBotLevel - MyTopLevel) < DTOL1))
			nRval |= ZLevelConnection;
	}

	if(MyTopLevel > UrTopLevel &&
		UrTopLevel > MyBotLevel )
		nRval |= LowUrMember;
	
	if(!(MyTopLevel <= UrBotLevel + DTOL1) || (UrTopLevel <= MyBotLevel + DTOL1))
		nRval |= ZLevelOverLap;

	return nRval;
}

void MSLinkMemberG::UpdateBoundaryVisibilities(MSCompoG* pUrCompoG, MSConnection* pConnection)
{
	if(pConnection->GetTrimOption(this) == MSConnection::TO_IGNORE || GetRigidMember() && pUrCompoG->GetType() != MSElement::msColumnG)
		return;
	
	if (IsSlopeMember() == true && pUrCompoG->IsSlopeMember() == true)
		return;
	
	if (pUrCompoG->IsPolyMember()) return;

	double MyTopLevel = 0, MyBotLevel = 0;
	double UrTopLevel = 0, UrBotLevel = 0;
	GetZLevels(MyTopLevel, MyBotLevel, false, false);
	pUrCompoG->GetZLevels(UrTopLevel, UrBotLevel, false, false);

	long eZLevelInfo = GetZLevelInfo(pUrCompoG, pConnection, true);
	if(eZLevelInfo & ZLevelConnection ||
		eZLevelInfo & LowUrMember)
		return;

	int nVisibility = 0; // 0: 선 없음 1: 실선 2: 점선
	// 거푸집 물량 공재 로직 추가후 변경하도록 한다
    //if (MyBotLevel < UrBotLevel)
		//nVisibility = 2; // 점선
	
	if (AppGlobalOptionWrapper::Instance()->GetAppMode() == AppGlobalOptionWrapper::Finish ||
		AppGlobalOptionWrapper::Instance()->GetAppMode() == AppGlobalOptionWrapper::Formwork)
	{
		if (MyBotLevel < UrBotLevel)
			nVisibility = 2; // 점선
	}
	

	bool isParallel = pConnection->IsParallel();
	bool isFirstParallel = pConnection->IsParallel() && (pConnection->GetFirstCompoG() == this);
	bool IsEndConnection = pConnection->IsEndConnection(this);

	bool isFormworkMode = AppGlobalOptionWrapper::Instance()->GetAppMode() == AppGlobalOptionWrapper::Formwork ? true : false;
	//bool isFormworkMode = false;

	GM2DPolyline ThePoly1, ThePoly2;
	GM2DPolyline* pMyBoundary = &ThePoly1;
	GM2DPolyline* pUrBoundary = &ThePoly2;
	// 포인터 부재의 경우 TrimedBoundary를 사용하지 않는다.(주두부 계산때문..) 
	MSCompoG::GetMyUrBoundary(this, pUrCompoG, pMyBoundary, pUrBoundary);

	double dTol = DTOL_MODELING;
	
	bool bCheck = false;
	// 2개의 라인부재 옵션이 OrG 인경우, S,ELine을 안지워 주는 문제
	// 라인부재가 서로 포함 되는치 체크
	if (pConnection->GetFirstTrimOption() == MSConnection::TO_ORIGIN && pConnection->GetSecondTrimOption() == MSConnection::TO_ORIGIN ||
		(pConnection->GetFirstTrimOption() == MSConnection::TO_ENDCUT || pConnection->GetSecondTrimOption() == MSConnection::TO_ENDCUT))
	{
		shared_ptr<GM2DCurve> pMyCurve = GetCenCurve();
		double dLenTol = MSFloor::ms_DefLineHitDistTol/pMyCurve->Length();
		GM2DVector SP = pMyCurve->PointAtParam(0 + dLenTol);
		GM2DVector EP = pMyCurve->PointAtParam(1 - dLenTol);
		pMyCurve->SetP0(SP);
		pMyCurve->SetP1(EP);

		if(pUrBoundary->OverlapEx(&*pMyCurve) == 0)
			bCheck = true;
	}

	vector<GM2DCurve*> MySegments;
	pMyBoundary->GetSegments(MySegments); // need to delete

	long nSeg = MySegments.size();
	for (long iSeg =0;iSeg<nSeg;iSeg++)
	{
		if(ma_TrimmedBoundaryVisible[iSeg] == 0)
			continue;

		GM2DCurve* pCurrent = MySegments[iSeg];
		GM2DVector MidPt =pCurrent->PointAtParam(0.5);
		double dDeg = pCurrent->GetDegree();

		bool isMyPointOnLine = true;
		if (pConnection->GetFirstTrimOption() == MSConnection::TO_ORIGIN && pConnection->GetSecondTrimOption() == MSConnection::TO_ORIGIN ||
			(pConnection->GetFirstTrimOption() == MSConnection::TO_ENDCUT || pConnection->GetSecondTrimOption() == MSConnection::TO_ENDCUT))
			isMyPointOnLine = IsPointOnLRLines(MidPt);

		long test = pUrBoundary->Contains(&MidPt, dTol);
		if (-1 == test &&
				(
					(GetType() == pUrCompoG->GetType() || pConnection->GetFirstCompoG() == pUrCompoG) ||
					(pUrCompoG->IsPointMember())
				)
			) // in polyline
			ma_TrimmedBoundaryVisible[iSeg] = nVisibility;

		else if (0== test ) // on line
		{
			if(pUrCompoG->IsLinkMember()) // 라인부재의 LRLine 선상에 있는 경우에만 0으로 셋팅
			{
				MSLinkMemberG* pUrLinkG = (MSLinkMemberG*)pUrCompoG;
				bool HasArc = false;
				if (isArc() || pUrLinkG->isArc())
					HasArc = true;

				if (pConnection->GetFirstTrimOption() == MSConnection::TO_MITER || pConnection->GetSecondTrimOption() == MSConnection::TO_MITER ||
					pConnection->GetFirstTrimOption() == MSConnection::TO_SQUAREOFF || pConnection->GetSecondTrimOption() == MSConnection::TO_SQUAREOFF ||
					(pConnection->GetFirstTrimOption() != pConnection->GetSecondTrimOption() && pConnection->GetTrimOption(this) == MSConnection::TO_ORIGIN)
					&& HasArc == false)
				{
					if (AppGlobalOptionWrapper::Instance()->GetAppMode() == AppGlobalOptionWrapper::Formwork)
					{
						if (HasOverLapWall() == true && pUrLinkG->GetType() == MSElement::msWallG && pUrLinkG->IsPointOnLRLines(MidPt, true, dDeg) == false)
							continue;
					}
					
// 					if(pConnection->HasTrimOption(MSConnection::TO_ORIGIN) && pConnection->HasTrimOption(MSConnection::TO_ENDCUT))
// 						continue;

					ma_TrimmedBoundaryVisible[iSeg] = nVisibility;
					continue;
				}
				
				bool isPointOnLRLines = pUrLinkG->IsPointOnLRLines(MidPt, true, dDeg);

				// 평행하고 pUrBoundary선위에 있지만 LR라인선이 아닌 라인을 0으로 셋팅 
				//==> 평행하고 pUrBoundary선위에 있는 시작,끝 마무리선은 0으로 셋팅
				if(isParallel == true && isPointOnLRLines == false ||
					HasArc == true && isPointOnLRLines == false && IsEndConnection)
					ma_TrimmedBoundaryVisible[iSeg] = nVisibility;

				if (pConnection->GetFirstTrimOption() == MSConnection::TO_ORIGIN && pConnection->GetSecondTrimOption() == MSConnection::TO_ORIGIN ||
					(pConnection->GetFirstTrimOption() == MSConnection::TO_ENDCUT || pConnection->GetSecondTrimOption() == MSConnection::TO_ENDCUT) /*||
					isFormworkMode == true*/)
				{
					// 평면부재이면서 라인부재의 경우 끝선을 그려 준다. (Formwork 전용)
					/*if (isMyPointOnLine == false && isPointOnLRLines == true && isFormworkMode)
						continue;*/

					if(isMyPointOnLine == false ||
						(bCheck == true && isMyPointOnLine == true && isPointOnLRLines == false)
						|| (isFormworkMode == true && isParallel == false && (pConnection->GetLinkMemberConPos(this) != CON_MID || pConnection->GetFirstCompoGID() == m_ID))
						)
						ma_TrimmedBoundaryVisible[iSeg] = nVisibility;
				}
				else
				{
					// 첫번째 평행한 부재가 아니고 LR라인선인 라인은 라인을 0으로 셋팅.
					//==> 2번째 평행한 부재가 LR라인 위에 있으면 라인을 0으로 셋팅함(평행할때 중복선 거)
					if(isFirstParallel == false && isPointOnLRLines == true)
						ma_TrimmedBoundaryVisible[iSeg] = nVisibility;
				}

				if(isParallel && (pConnection->GetSecondCompoG() == this))
				{
					if(isPointOnLRLines)				
						ma_TrimmedBoundaryVisible[iSeg] = nVisibility;
				}

			}
			else // 그외에는 선상에 있으면 0으로 셋팅
				ma_TrimmedBoundaryVisible[iSeg] = nVisibility;
		}
	}

	for (long iSeg = 0; iSeg < MySegments.size(); iSeg++)
	{
		delete MySegments[iSeg];
	}
}

void MSLinkMemberG::GetLRLines4Rebar(shared_ptr<GM2DCurve>& pLCurve, shared_ptr<GM2DCurve>& pRCurve, double dCutMinLen)
{
	GetTrimmedLRLines(pLCurve, pRCurve, true, true, dCutMinLen);
}

void MSLinkMemberG::GetTrimmedLRLines(shared_ptr<GM2DCurve>& pLCurve, shared_ptr<GM2DCurve>& pRCurve, bool bUseVisibility, bool bMakeRebarLine, double dCutMinLen)
{
	pLCurve = shared_ptr<GM2DCurve>(this->mp_LLine->Copy());
	pRCurve = shared_ptr<GM2DCurve>(this->mp_RLine->Copy());

	vector<int> IndicesLLine;
	vector<int> IndicesRLine;
	vector<int> IndicesToDelete;
	vector<GM2DCurve*> pa_Segments;

	GM2DPolyline* pMyBoundary = this->GetTrimmedBoundary();
	pMyBoundary->GetSegments(pa_Segments);
	
	if (GetTrimmedBoundary()->ma_Bulge.GetSize() != ma_TrimmedBoundaryVisible.size())
		UpdateBoundaryVisibilities();

	// ON LLine or RLine Test
	long nSeg = pa_Segments.size();
	for(long iSeg=0; iSeg<nSeg; iSeg++)
	{
		GM2DCurve* pCurve = pa_Segments[iSeg];
		if(bUseVisibility == true && ma_TrimmedBoundaryVisible[iSeg] == false)
			continue;
		
		GM2DVector MidPt = pCurve->PointAtParam(0.5);
		if (IsPointOnLLine(MidPt))
			IndicesLLine.push_back(iSeg);
		else if (IsPointOnRLine(MidPt))
			IndicesRLine.push_back(iSeg);
	}

	// overlap shift
	long Shift = 0;
	long nLIndex = IndicesLLine.size();
	long nRIndex = IndicesRLine.size();
	if(nLIndex == 0 && nRIndex == 0)
		return;
	
	if (nLIndex > 0 && nRIndex > 0)
	{
		if (IndicesLLine[nLIndex - 1] < IndicesRLine[nRIndex - 1])
		{
			if (IndicesLLine[0] > IndicesRLine[0])
			{
				Shift = IndicesLLine[0];
			}
		}
		else if (IndicesRLine[0] > IndicesLLine[0])
		{
			Shift = IndicesRLine[0];
		}
	}

	if (nLIndex > 0)
	{
		for (long iLIndex = 0; iLIndex < nLIndex; iLIndex++)
			IndicesLLine[iLIndex] = (IndicesLLine[iLIndex] - Shift + nSeg) % nSeg;
		sort(IndicesLLine.begin(), IndicesLLine.end());
	}

	if (nRIndex > 0)
	{
		for (long iRIndex = 0; iRIndex < nRIndex; iRIndex++)
			IndicesRLine[iRIndex] = (IndicesRLine[iRIndex] - Shift + nSeg) % nSeg;
		sort(IndicesRLine.begin(), IndicesRLine.end());
	}


	if(bMakeRebarLine == true)
	{
		//Seg 길이가 dCutMinLen 보다 짧을 경우 인덱스 제거 시작과 끝 1번만 제거
		if(nLIndex > 1)
		{
			GM2DCurve* pSCurve = pa_Segments[(IndicesLLine[0] +Shift)%nSeg];
			if(pSCurve->Length() <= dCutMinLen)
			{
				IndicesLLine.erase(IndicesLLine.begin());
				nLIndex = IndicesLLine.size();
			}
		}

		if(nLIndex > 1)
		{
			GM2DCurve* pECurve = pa_Segments[(IndicesLLine[nLIndex-1] +Shift)%nSeg];
			if(pECurve->Length() <= dCutMinLen)
			{
				IndicesLLine.erase(IndicesLLine.begin() + (nLIndex-1) );
				nLIndex = IndicesLLine.size();
			}
		}

		if(nRIndex > 1)
		{
			GM2DCurve* pSCurve = pa_Segments[(IndicesRLine[0] +Shift)%nSeg];
			if(pSCurve->Length() <= dCutMinLen)
			{
				IndicesRLine.erase(IndicesRLine.begin());
				nRIndex = IndicesRLine.size();
			}
		}

		if(nRIndex > 1)
		{
			GM2DCurve* pECurve = pa_Segments[(IndicesRLine[nRIndex-1] +Shift)%nSeg];
			if(pECurve->Length() <= dCutMinLen)
			{
				IndicesRLine.erase(IndicesRLine.begin() + (nRIndex-1) );
				nRIndex = IndicesRLine.size();
			}
		}
	}

	if (nLIndex > 0)
	{
		pLCurve->m_P0 = pa_Segments[(IndicesLLine[0] + Shift) % nSeg]->m_P0;
		pLCurve->m_P1 = pa_Segments[(IndicesLLine[nLIndex - 1] + Shift) % nSeg]->m_P1;
	}

	if (nRIndex > 0)
	{
		pRCurve->m_P1 = pa_Segments[(IndicesRLine[0] + Shift) % nSeg]->m_P0;
		pRCurve->m_P0 = pa_Segments[(IndicesRLine[nRIndex - 1] + Shift) % nSeg]->m_P1;
	}

	shared_ptr<GM2DCurve> pCenCurve = GetCenCurve();
	double dDeg = pCenCurve->GetDegree();

	shared_ptr<GM2DArc> pLArc = dynamic_pointer_cast<GM2DArc>(pLCurve);
	shared_ptr<GM2DArc> pRArc = dynamic_pointer_cast<GM2DArc>(pRCurve);
	if(isArc() && (pLArc != NULL && pRArc != NULL) )
	{
		double dLSLoc = pCenCurve->OrthoBaseLocValue(pLArc->m_P0);
		if(dLSLoc < 0.5)
		{
			if(pLArc->Set3P(pLArc->m_P0, mp_LLine->PointAtParam(0.5), pLArc->m_P1) == FALSE)
			{
				// 아크 생성 실패시 직선으로 만들어 준다. ==> 아크가 직선 처럼 생겼을 경우 아크 생성 실패함
				GM2DLineVector TheLine(pLArc->m_P0, pLArc->m_P1);
				(pLCurve).reset();
				pLCurve = shared_ptr<GM2DCurve>(new GM2DLineVector(TheLine));

			}
		}
		else
		{
			if(pLArc->Set3P(pLArc->m_P1, mp_LLine->PointAtParam(0.5), pLArc->m_P0) == FALSE)
			{
				GM2DLineVector TheLine(pLArc->m_P1, pLArc->m_P0);
				(pLCurve).reset();
				pLCurve = shared_ptr<GM2DCurve>(new GM2DLineVector(TheLine));
			}
		}

		double dRSLoc = pCenCurve->OrthoBaseLocValue(pRArc->m_P0);
		if(dRSLoc < 0.5)
		{
			if(pRArc->Set3P(pRArc->m_P0, mp_RLine->PointAtParam(0.5), pRArc->m_P1) == FALSE)
			{
				GM2DLineVector TheLine(pRArc->m_P0, pRArc->m_P1);
				(pRCurve).reset();
				pRCurve = shared_ptr<GM2DCurve>(new GM2DLineVector(TheLine));
			}
		}
		else
		{
			if(pRArc->Set3P(pRArc->m_P1, mp_RLine->PointAtParam(0.5), pRArc->m_P0) == FALSE)
			{
				GM2DLineVector TheLine(pRArc->m_P1, pRArc->m_P0);
				(pRCurve).reset();
				pRCurve = shared_ptr<GM2DCurve>(new GM2DLineVector(TheLine));
			}
		}
	}
	else
	{
		if(fabs(dDeg - pLCurve->GetDegree() ) > MSFloor::ms_ColinearDegreeTol)
			pLCurve->ReverseDirection();

		if(fabs(dDeg - pRCurve->GetDegree() ) > MSFloor::ms_ColinearDegreeTol)
			pRCurve->ReverseDirection();
	}

	nSeg = pa_Segments.size();
	for (long iSeg = 0; iSeg < nSeg; iSeg++)
	{
		GM2DCurve* pCurve = pa_Segments[iSeg];
		delete pCurve;
	}
	pa_Segments.clear();
}

double MSLinkMemberG::GetExtendLenbyOffset(GM2DCurve* pCurve)
{
	if(m_SOffsetZ == m_EOffsetZ)
		return pCurve->Length();

	shared_ptr<GM2DCurve> pCenCurve = GetCenCurve();
	double dSLoc = pCenCurve->OrthoBaseLocValue(pCurve->m_P0);
	double dELoc = pCenCurve->OrthoBaseLocValue(pCurve->m_P1);
	double dSOffset = GetOffSetbyLoc(dSLoc);
	double dEOffset = GetOffSetbyLoc(dELoc);

	double dZLevel = fabs(dSOffset - dEOffset);

	double dLen = pCurve->Length();
	double dExLen = sqrt(pow(dZLevel, 2) + pow	(dLen, 2));

	return dExLen;
}

void MSLinkMemberG::RemoveInwardWedge()
{
	if(isArc())
		return;

	GM2DPolyline* pMyBoundary = this->GetTrimmedBoundary();
	vector<bool> onLLineArr;
	vector<bool> onRLineArr;
	double dTol = DTOL_GM;

	int numPnt = pMyBoundary->ma_Point.GetSize();
	for (int idx = 0; idx < numPnt - 1; idx++)
	{
		onLLineArr.push_back(false);
		onRLineArr.push_back(false);
		GM2DVector* pVec = pMyBoundary->ma_Point[idx];

		double dLoc = mp_LLine->OrthoBaseLocValue(*pVec);
		if (mp_LLine->IsColinearPoint(*pVec, dTol))
			onLLineArr[idx] = true;
		if (mp_RLine->IsColinearPoint(*pVec, dTol))
			onRLineArr[idx] = true;
	}

	vector<int> IndicesToDelete;
	for (int idx = 0; idx < numPnt - 1; idx++)
	{
		int preIdx = idx - 1;
		if (idx == 0) // 첫번째 점인경우
			preIdx = onLLineArr.size() - 1;
		int nextIdx = idx + 1;
		if (idx == numPnt - 2) // 마지막점인경우
			nextIdx = 0;

		if (onLLineArr[idx] == false)
		{
			bool isPreOnLine = onLLineArr[preIdx];
			bool isNextOnLine = onLLineArr[nextIdx];
			if (isPreOnLine == true && isNextOnLine == true)
			{
				IndicesToDelete.push_back(idx);
				continue;
			}
		}
		if (onRLineArr[idx] == false)
		{
			bool isPreOnLine = onRLineArr[preIdx];
			bool isNextOnLine = onRLineArr[nextIdx];
			if (isPreOnLine == true && isNextOnLine == true)
			{
				IndicesToDelete.push_back(idx);
				continue;
			}
		}
	}

	// Delte Point and Bulge at Polyline
	long nIndex = IndicesToDelete.size();
	for (long iIndex = nIndex - 1; iIndex >= 0; iIndex--)
		pMyBoundary->RemovePointBulgeAt(IndicesToDelete[iIndex]);

	pMyBoundary->MakeClosedPolyline(dTol);

	/* 이전 로직
	vector<int> IndicesLLine;
	vector<int> IndicesRLine;
	vector<int> IndicesToDelete;
	vector<GM2DCurve*> pa_Segments;

	GM2DPolyline* pMyBoundary = this->GetTrimmedBoundary();
	pMyBoundary->GetSegments(pa_Segments);

	// ON LLine or RLine Test
	long nSeg = pa_Segments.size();
	for(long iSeg=0; iSeg<nSeg; iSeg++)
	{
		GM2DVector MidPt = pa_Segments[iSeg]->PointAtParam(0.5);
		if (IsPointOnLLine(MidPt))
			IndicesLLine.push_back(iSeg);
		else if (IsPointOnRLine(MidPt))
			IndicesRLine.push_back(iSeg);
	}

	// overlap shift
	long Shift = 0;
	long nLIndex = IndicesLLine.size();
	long nRIndex = IndicesRLine.size();
	if(nLIndex == 0 || nRIndex == 0)
		return;
	if (IndicesLLine[nLIndex-1]<IndicesRLine[nRIndex-1])
	{
		if (IndicesLLine[0]>IndicesRLine[0])
		{
			Shift = IndicesLLine[0];
		}
	}
	else if (IndicesRLine[0]>IndicesLLine[0])
	{
		Shift = IndicesRLine[0];
	}

	for(long iLIndex=0; iLIndex<nLIndex; iLIndex++)
		IndicesLLine[iLIndex] = (IndicesLLine[iLIndex] - Shift+nSeg)%nSeg;
	sort(IndicesLLine.begin(), IndicesLLine.end());

	for(long iRIndex=0; iRIndex<nRIndex; iRIndex++)
		IndicesRLine[iRIndex] = (IndicesRLine[iRIndex] - Shift+nSeg)%nSeg;
	sort(IndicesRLine.begin(), IndicesRLine.end());

	// find index to delete
	for(long iLIndex=0; iLIndex<nLIndex-1; iLIndex++)
	{
		int current = IndicesLLine[iLIndex] + 1; // +1 추가 / 중간점만 지워준다. 시작점도 같이 지워주면 인터섹트와 부재의 절점이 삭제되는 문제가 있음
		int next = IndicesLLine[iLIndex+1];
		while (++current<next)
		{
			IndicesToDelete.push_back((current+Shift)%nSeg);
		}
	}
	for(long iRIndex=0; iRIndex<nRIndex-1; iRIndex++)
	{
		int current = IndicesRLine[iRIndex] + 1; // +1 추가 / 중간점만 지워준다. 시작점도 같이 지워주면 인터섹트와 부재의 절점이 삭제되는 문제가 있음
		int next = IndicesRLine[iRIndex+1];
		while (++current<next)
		{
			IndicesToDelete.push_back((current+Shift)%nSeg);
		}
	}
	sort(IndicesToDelete.begin(), IndicesToDelete.end());

	// Delte Point and Bulge at Polyline
	long nIndex = IndicesToDelete.size();
	for(long iIndex=nIndex-1; iIndex>=0; iIndex--)
	{
		pMyBoundary->RemovePointBulgeAt(IndicesToDelete[iIndex]);
	}

	nSeg = pa_Segments.size();
	for (long iSeg = 0; iSeg < nSeg; iSeg++)
	{
		GM2DCurve* pCurve = pa_Segments[iSeg];
		delete pCurve;
	}
	pa_Segments.clear();
	*/
}

long MSLinkMemberG::CheckIntersectLinkMember(MSLinkMemberG* pUrLinkG, GM2DVector& CrossVec, double& dMyLoc, double& dUrLoc, double& dMyLocTol, double& dUrLocTol)
{
	dMyLoc = dUrLoc = dMyLocTol = dUrLocTol = 0;
	shared_ptr<GM2DCurve> pMyCurve = NULL;
	shared_ptr<GM2DCurve> pOldMyCurve = NULL;
	
	if(GetType() == MSElement::msLineLevelDifG)
	{
		MSLineLevelDifG* pDifG = (MSLineLevelDifG*)this;
		pMyCurve = shared_ptr<GM2DCurve>(pDifG->GetCurve()->Copy());
		pOldMyCurve = shared_ptr<GM2DCurve>(pMyCurve->Copy());
	}
	else
	{
		pMyCurve = GetCenCurve();
		pOldMyCurve = shared_ptr<GM2DCurve>(pMyCurve->Copy());
	}

	shared_ptr<GM2DCurve> pUrCurve = NULL;
	shared_ptr<GM2DCurve> pOldUrCurve = NULL;
	if(pUrLinkG->GetType() == MSElement::msLineLevelDifG)
	{
		MSLineLevelDifG* pUrDifG = (MSLineLevelDifG*)pUrLinkG;
		pUrCurve = shared_ptr<GM2DCurve>(pUrDifG->GetCurve()->Copy());
		pOldUrCurve = shared_ptr<GM2DCurve>(pUrCurve->Copy());
	}
	else
	{
		pUrCurve = pUrLinkG->GetCenCurve();
		pOldUrCurve = shared_ptr<GM2DCurve>(pUrCurve->Copy());
	}

	long nPVal = pMyCurve->IsParallel4DTol(&*pUrCurve);
	if(nPVal != 0)
	{
		double dMinDist = (double)INTMAX_MAX;
		GM2DVector MyVec, UrVec;
		double dSSDist = pMyCurve->m_P0.Distance(pUrCurve->m_P0);
		if(dMinDist > dSSDist)
		{
			dMinDist = dSSDist;
			MyVec = pMyCurve->m_P0;
			UrVec = pUrCurve->m_P0;
		}
		double dSEDist = pMyCurve->m_P0.Distance(pUrCurve->m_P1);
		if(dMinDist > dSEDist)
		{
			dMinDist = dSEDist;
			MyVec = pMyCurve->m_P0;
			UrVec = pUrCurve->m_P1;
		}
		double dESDist = pMyCurve->m_P1.Distance(pUrCurve->m_P0);
		if(dMinDist > dESDist)
		{
			dMinDist = dESDist;
			MyVec = pMyCurve->m_P1;
			UrVec = pUrCurve->m_P0;
		}
		double dEEDist = pMyCurve->m_P1.Distance(pUrCurve->m_P1);
		if(dMinDist > dEEDist)
		{
			dMinDist = dEEDist;
			MyVec = pMyCurve->m_P1;
			UrVec = pUrCurve->m_P1;
		}
		
		dMyLoc = pMyCurve->OrthoBaseLocValue(UrVec);
		dUrLoc = pUrCurve->OrthoBaseLocValue(MyVec);
		return 2;
	}
	
	// 연장길에서만 연결된 부재의 MaxWidth를 사용
	// 이후 LocTol 계산에서는 Width를 사용
	double dMyWidth = GetMaxPartnerWidth();
	double dUrWidth = pUrLinkG->GetMaxPartnerWidth();
// 	double dMyExtendLen = MSLinkMemberG::ms_dDefExtendLen;
// 	double dUrExtendLen = MSLinkMemberG::ms_dDefExtendLen;
	double dMyExtendLen = max(dMyWidth*1.5, dUrWidth*1.5);
	double dUrExtendLen = dMyExtendLen;

	double dLineHitDist = DTOL_GM;
	if(pMyCurve->m_ObjType == GT_ARC || pUrCurve->m_ObjType == GT_ARC)
		dLineHitDist = max(dMyWidth, dUrWidth);

	if(pMyCurve->m_ObjType != GT_ARC)
	{
		pMyCurve->ExtendLineVector(TRUE, dMyExtendLen*2, FALSE);
		pMyCurve->ExtendLineVector(FALSE, dMyExtendLen*2, TRUE);
	}

	if(pUrCurve->m_ObjType != GT_ARC)
	{
		pUrCurve->ExtendLineVector(TRUE, dUrExtendLen*2, FALSE);
		pUrCurve->ExtendLineVector(FALSE, dUrExtendLen*2, TRUE);
	}

	dMyWidth = GetWidth();
	dUrWidth = pUrLinkG->GetWidth();

	// 시작점 맞추기
	vector<GM2DVector> CrossVecs;
	vector<double> MyLocArr, UrLocArr;
	vector<long> ResultArr;
	long nRval = pMyCurve->IntersectEnd2(&*pUrCurve, CrossVecs, MyLocArr, UrLocArr, ResultArr, true, DTOL_GM, DTOL_GM, dLineHitDist);
	if (nRval % 10 != 1)
	{
		GM2DVector MySVec = pOldMyCurve->m_P0;
		GM2DVector MyEVec = pOldMyCurve->m_P1;

		GM2DVector UrSVec = pOldUrCurve->m_P0;
		GM2DVector UrEVec = pOldUrCurve->m_P1;

		double dSSDist = MySVec.Distance(UrSVec);
		double dSEDist = MySVec.Distance(UrEVec);
		double dESDist = MyEVec.Distance(UrSVec);
		double dEEDist = MyEVec.Distance(UrEVec);

		double dMinDist = min(dSSDist, min(dSEDist, min(dESDist, dEEDist)));

		if (fabs(dMinDist - dSSDist) < DTOL10 )
		{
			double dCheckLoc = pOldUrCurve->OrthoBaseLocValue(MySVec);
			if (dCheckLoc > 0 && dCheckLoc < 1)
				dMyLoc = 0;

			dCheckLoc = pOldMyCurve->OrthoBaseLocValue(UrSVec);
			if (dCheckLoc > 0 && dCheckLoc < 1)
				dUrLoc = 0;
		}
		else if (fabs(dMinDist - dSEDist) < DTOL10)
		{
			double dCheckLoc = pOldUrCurve->OrthoBaseLocValue(MySVec);
			if (dCheckLoc > 0 && dCheckLoc < 1)
				dMyLoc = 0;

			dCheckLoc = pOldMyCurve->OrthoBaseLocValue(UrEVec);
			if (dCheckLoc > 0 && dCheckLoc < 1)
				dUrLoc = 1;
		}
		else if (fabs(dMinDist - dESDist) < DTOL10)
		{
			double dCheckLoc = pOldUrCurve->OrthoBaseLocValue(MyEVec);
			if (dCheckLoc > 0 && dCheckLoc < 1)
				dMyLoc = 1;

			dCheckLoc = pOldMyCurve->OrthoBaseLocValue(UrSVec);
			if (dCheckLoc > 0 && dCheckLoc < 1)
				dUrLoc = 0;
		}
		else if (fabs(dMinDist - dEEDist) < DTOL10)
		{
			double dCheckLoc = pOldUrCurve->OrthoBaseLocValue(MyEVec);
			if (dCheckLoc > 0 && dCheckLoc < 1)
				dMyLoc = 1;

			dCheckLoc = pOldMyCurve->OrthoBaseLocValue(UrEVec);
			if (dCheckLoc > 0 && dCheckLoc < 1)
				dUrLoc = 1;
		}

		double dMyLen = pOldMyCurve->Length();
		double dUrLen = pOldUrCurve->Length();
		if (dMyLen > 0)
			dMyLocTol = (dUrWidth*0.5) / dMyLen + MSFloor::ms_DefLineHitDistTol / dMyLen;
		if (dUrLen > 0)
			dUrLocTol = (dMyWidth*0.5) / dUrLen + MSFloor::ms_DefLineHitDistTol / dUrLen;

		return nRval;
	}

	long nIdx = 0;
	for(long i = 0 ; i < ResultArr.size() ; i++)
	{
		if(ResultArr[i] == 1)
		{	
			nIdx = i;
			break;
		}
	}

	if(CrossVecs.size() == 0)
		return 0;

	CrossVec = CrossVecs[nIdx];
	dMyLoc = pOldMyCurve->OrthoBaseLocValue(CrossVec);
	dUrLoc = pOldUrCurve->OrthoBaseLocValue(CrossVec);
// 	dMyLoc = MyLocArr[nIdx];
// 	dUrLoc = UrLocArr[nIdx];

	double dMyLen = pOldMyCurve->Length();
	double dUrLen = pOldUrCurve->Length();
	if(dMyLen > 0)
		dMyLocTol = (dUrWidth*0.5)/dMyLen + MSFloor::ms_DefLineHitDistTol/dMyLen;
	if(dUrLen > 0)
		dUrLocTol = (dMyWidth*0.5)/dUrLen + MSFloor::ms_DefLineHitDistTol/dUrLen;

	return 1;
}
