#include "StdAfx.h"
#include "MSSlabLineLevelDifG.h"
#include "MSLevelDifHaunch.h"
#include "MSSlabG.h"

MSSlabLineLevelDifG::MSSlabLineLevelDifG(MSLineLevelDifG* pDif, MSSlabG* pSlab)
{
	m_dSLoc = 0;
	m_dELoc = 1;

	mp_LineLevelDifG = pDif;
	m_pSlab = pSlab;

	MSLineLevelDifG::CopyFromMe(pDif);

	// mylee temp - later change ProcessLineLevelDiffModify() in MainWindow.cs
	mp_LeftSlab = pSlab;
	mp_RightSlab = pSlab;

	pDif->mm_SlabGID.insert(make_pair(pSlab->m_ID, pSlab->m_ID));
}


MSSlabLineLevelDifG::~MSSlabLineLevelDifG(void)
{
}

void MSSlabLineLevelDifG::CopyFromMe( MSSlabLineLevelDifG* pSource, bool bNewID/*=true*/)
{
	MSLineLevelDifG::CopyFromMe(pSource);

	m_dSLoc = pSource->m_dSLoc;
	m_dELoc = pSource->m_dELoc;

	mp_LineLevelDifG = pSource->mp_LineLevelDifG;
	m_pSlab = pSource->m_pSlab;
}

bool MSSlabLineLevelDifG::isSameDif(MSSlabLineLevelDifG* pDif)
{
	if(pDif == NULL)
		return false;

	if(mp_LineLevelDifG != pDif->mp_LineLevelDifG)
		return false;

	GM2DCurve* pCurve = mp_LineLevelDifG->GetCurve();
	GM2DVector SVec = pCurve->PointAtParam(m_dSLoc);
	GM2DVector EVec = pCurve->PointAtParam(m_dELoc);

	GM2DCurve* pUrCurve = pDif->mp_LineLevelDifG->GetCurve();
	GM2DVector UrSVec = pUrCurve->PointAtParam(pDif->m_dSLoc);
	GM2DVector UrEVec = pUrCurve->PointAtParam(pDif->m_dELoc);

	if ((SVec.SameVector4DTOL(UrSVec) || SVec.SameVector4DTOL(UrEVec)) &&
		(EVec.SameVector4DTOL(UrSVec) || EVec.SameVector4DTOL(UrEVec)))
		return true;

	return false;

// 	if(fabs(m_dSLoc - pDif->m_dSLoc) > DTOL_GM)
// 		return false;
// 
// 	if(fabs(m_dELoc - pDif->m_dELoc) > DTOL_GM)
// 		return false;

	return true;
}

void MSSlabLineLevelDifG::SetLoc(double dSLoc, double dELoc)
{
	m_dSLoc = dSLoc;
	m_dELoc = dELoc;
	GM2DCurve* pCurve = mp_LineLevelDifG->GetCurve();
	GM2DCurve* pNewCurve = pCurve->Copy();
	GM2DVector SVec = pCurve->PointAtParam(m_dSLoc);
	GM2DVector EVec = pCurve->PointAtParam(m_dELoc);
	pNewCurve->SetP0(SVec);
	pNewCurve->SetP1(EVec);
	SetCenCurve(pNewCurve);
}
