#include "StdAfx.h"
#include "MREdge.h"
#include "MRVBar.h"

#include "GMLib/GM2DLineVector.h"
#include "StructureModel\MSDeformedBar.h"

IMPLEMENT_SERIAL(MREdge, MRObject, VERSIONABLE_SCHEMA | 1)
MREdge::MREdge(void)
{
	mp_Curve = NULL;
	mp_StrpCurve = NULL;

	mp_HBarCurve = NULL;
	mp_VBarCurve = NULL;

	mp_Linked0 = NULL;
	mp_Linked1 = NULL;
	m_Link0 = 0;
	m_Link1 = 0;

	m_VBarDistTol = 1.;
}

MREdge::MREdge(GM2DGeometry* pCurve)
{
	mp_Curve = NULL;

	mp_HBarCurve = NULL;
	mp_VBarCurve = NULL;
	SetCurve(pCurve);
	mp_StrpCurve = NULL;

	mp_Linked0 = NULL;
	mp_Linked1 = NULL;
	m_Link0 = 0;
	m_Link1 = 0;

	m_VBarDistTol = 1.;
}

MREdge::~MREdge(void)
{
	if (mp_Curve)	 delete mp_Curve;
	if (mp_HBarCurve)	delete mp_HBarCurve;
	if (mp_VBarCurve)	delete mp_VBarCurve;
	if (mp_StrpCurve)	delete mp_StrpCurve;
	DeleteVBarVec();
}
void MREdge::Serialize(CArchive& ar)
{
	MRObject::Serialize(ar);
	bool UnUsed = 0;
	if (ar.IsStoring())
	{
		if (mp_Curve)
		{
			ar << mp_Curve->m_ObjType;
			mp_Curve->Serialize(ar);
		}
		else
			ar << UnUsed;
		long nVBarSet = ma_VBarSet.GetSize();
		ar << nVBarSet;
		for (long iVBarSet = 0; iVBarSet < nVBarSet; iVBarSet++)
		{
			MRVBar* pVBar = ma_VBarSet[iVBarSet];
			pVBar->Serialize(ar);
		}
	}
	else
	{
		long nObjType = 0;
		ar >> nObjType;
		if (nObjType == GT_LINE)			mp_Curve = new GM2DLineVector;
		else if (nObjType == GT_ARC)		mp_Curve = new GMArc();
		else if (nObjType == GT_CIRCLE)	mp_Curve = new GMCircle;

		if (mp_Curve)		mp_Curve->Serialize(ar);

		long nVBarSet = 0;
		ar >> nVBarSet;
		for (long iVBarSet = 0; iVBarSet < nVBarSet; iVBarSet++)
		{
			MRVBar* pVBar = new MRVBar;
			pVBar->Serialize(ar);
			if (iVBarSet == 0)	mp_StrpCurve = pVBar->mp_StrpCurve;
			pVBar->mp_Edge = this;
			ma_VBarSet.Add(pVBar);
		}
		CalcVBarVectors();
	}
}
void MREdge::DeleteVBarVec()
{
	long nLayer = ma_VBarSet.GetSize();
	for (long iLayer = 0; iLayer < nLayer; iLayer++)
	{
		MRVBar* pVBar = ma_VBarSet[iLayer];
		long nVBar = pVBar->ma_VBarLoc.GetSize();
		for (long iVBar = 0; iVBar < nVBar; iVBar++)
			delete pVBar->ma_VBarVec[iVBar];
	}
}

static long RemoveShareVBar(MRVBar* pMyVBar, MREdge* pLinked, int myLayer, double scaleFactor)
{
	long nRemove = 0;
	long nMyVBar = pMyVBar->ma_VBarVec.GetSize();
	double dTol = 15;
	long DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(pMyVBar->GetDia());
	if (DiaIndex != -1)
		dTol = MSDeformedBar::FindDeformedBarDiameterByIndex(DiaIndex);
	if (scaleFactor > DTOL10)
		dTol = dTol / scaleFactor;
	for (long iMyVBar = nMyVBar - 1; iMyVBar >= 0; iMyVBar--)
	{
		GM2DVector* pMyVec = pMyVBar->ma_VBarVec[iMyVBar];
		long nUrLayer = pLinked->ma_VBarSet.GetSize();
		for (long iUrLayer = 0; iUrLayer < nUrLayer; iUrLayer++)
		{
			MRVBar* pUrVBar = pLinked->ma_VBarSet[iUrLayer];
			long nUrVBar = pUrVBar->ma_VBarVec.GetSize();
			for (long iUrVBar = 0; iUrVBar < nUrVBar; iUrVBar++)
			{
				GM2DVector* pUrVec = pUrVBar->ma_VBarVec[iUrVBar];
				double Dist = pMyVec->Distance(*pUrVec);
				if (Dist < dTol)
				{
					if (myLayer >= iUrLayer)
					{
						delete pMyVec;
						pMyVBar->ma_VBarVec.RemoveAt(iMyVBar);
						pMyVBar->ma_VBarLoc.RemoveAt(iMyVBar);
						nRemove++;
						break;
					}
					else
					{
						delete pUrVec;
						pUrVBar->ma_VBarVec.RemoveAt(iUrVBar);
						pUrVBar->ma_VBarLoc.RemoveAt(iUrVBar);
						nRemove++;
						break;
					}
				}
			}
			if (iUrVBar < nUrVBar)
				break;
		}
	}

	return nRemove;
}

long MREdge::RemoveCornerVBar(bool bStart, bool bEnd, double scaleFactor)
{
	if (!mp_Linked0 || !mp_Linked1)
		return 0;

	long nLayer = ma_VBarSet.GetSize();
	long nRemove = 0;
	for (long iLayer = nLayer - 1; iLayer >= 0; iLayer--)
	{
		MRVBar* pMyVBar = ma_VBarSet[iLayer];
		long nMyVBar = pMyVBar->ma_VBarVec.GetSize();

		if (bStart)
			nRemove += RemoveShareVBar(pMyVBar, mp_Linked0, iLayer, scaleFactor);
		if (bEnd)
			nRemove += RemoveShareVBar(pMyVBar, mp_Linked1, iLayer, scaleFactor);

		if (pMyVBar->ma_VBarVec.GetSize() == 0)
			ma_VBarSet.RemoveAt(iLayer);
	}

	return nRemove;
}

void MREdge::ReverseLocValues()
{
	int nVBarSet = ma_VBarSet.GetSize();
	for (int iVBarSet = 0; iVBarSet < nVBarSet; iVBarSet++)
	{
		MRVBar* pMRVBar = ma_VBarSet[iVBarSet];

		vector<double> LocValueArr;
		int NumLoc = pMRVBar->ma_VBarLoc.GetSize();
		for (int iLoc = 0; iLoc < NumLoc; iLoc++)
		{
			double dLoc = pMRVBar->ma_VBarLoc[iLoc];
			LocValueArr.push_back(dLoc);
		}

		pMRVBar->m_LayerDistFromEdge *= -1;
		pMRVBar->ma_VBarLoc.RemoveAll();
		for (int iLoc = NumLoc - 1; iLoc >= 0; iLoc--)
		{
			double dReverseLoc = 1 - LocValueArr[iLoc];
			pMRVBar->ma_VBarLoc.Add(dReverseLoc);
		}
	}
}

void MREdge::Reverse()
{
	GM2DVector tempVec = m_P0;
	m_P0 = m_P1;
	m_P1 = tempVec;
	if (mp_Curve && mp_Curve->m_ObjType == GT_LINE)
	{
		GM2DLineVector* pLineCurve = (GM2DLineVector*)mp_Curve;
		pLineCurve->m_P0 = m_P0;
		pLineCurve->m_P1 = m_P1;
	}
}

bool MREdge::CalcVBarVectors()
{
	if (mp_Curve->m_ObjType != GT_LINE)
		return false;

	GM2DLineVector* pLine = (GM2DLineVector*)mp_Curve;

	long nLayer = ma_VBarSet.GetSize();
	for (long iLayer = 0; iLayer < nLayer; iLayer++)
	{
		MRVBar* pVBar = ma_VBarSet[iLayer];
		GM2DLineVector LayerLVec = pLine->NormalTranslate(pVBar->m_LayerDistFromEdge);
		long nVBar = pVBar->ma_VBarLoc.GetSize();
		for (long iVBar = 0; iVBar < nVBar; iVBar++)
		{
			double VBarLoc = pVBar->ma_VBarLoc[iVBar];
			pVBar->ma_VBarVec.Add(new GM2DVector(LayerLVec.PointAtParam(VBarLoc)));
		}
	}
	return true;
}

void MREdge::AddVBarSet(MRVBar* pRebar)
{
	pRebar->mp_Edge = this;
	ma_VBarSet.Add(pRebar);
}

bool MREdge::SetCurve(GM2DGeometry* pCurve)
{
	mp_Curve = pCurve;
	if (pCurve->m_ObjType == GT_LINE)
	{
		GM2DLineVector* pLine = (GM2DLineVector*)pCurve;
		m_P0 = pLine->m_P0;
		m_P1 = pLine->m_P1;
		m_VBarDistTol = 1.0;
		//		m_VBarDistTol = pLine->Length()/100.;
	}
	else
	{
		GMCircle* pArc = (GMCircle*)pCurve;
		GMVector vPoint = pArc->GetStartPoint();
		m_P0.SetXY(vPoint.m_X, vPoint.m_Y);
		vPoint = pArc->GetEndPoint();
		m_P1.SetXY(vPoint.m_X, vPoint.m_Y);
		m_VBarDistTol = 1.0;
		//		m_VBarDistTol = pArc->GetArcLength()/100.;
	}

	mp_Linked0 = NULL;
	mp_Linked1 = NULL;

	return true;
}

void MREdge::AddVBarOnCircle(GMVector& vVBar, double dTol)
{
	// 	if(mp_Curve->m_ObjType != GT_CIRCLE)
	// 		return;

	GMCircle* pSectCircle = (GMCircle*)mp_Curve;
	double LayerDist, Radian;
	if (pSectCircle->Contains(vVBar, &LayerDist, &Radian) >= 0)
		return;

	MRVBar* pVBar = nullptr;
	long iLoc = 0;
	long nLoc = ma_VBarSet.GetSize();
	for (iLoc = 0; iLoc < nLoc; iLoc++)
	{
		pVBar = ma_VBarSet[iLoc];
		if (fabs(pVBar->m_LayerDistFromEdge - LayerDist) < dTol)
			break;
		if (LayerDist < pVBar->m_LayerDistFromEdge)
		{
			pVBar = new MRVBar;
			pVBar->m_LayerDistFromEdge = LayerDist;
			pVBar->mp_Edge = this;
			ma_VBarSet.InsertAt(iLoc, pVBar);
			break;
		}
	}
	if (iLoc == nLoc)
	{
		pVBar = new MRVBar;
		pVBar->m_LayerDistFromEdge = LayerDist;
		pVBar->mp_Edge = this;
		ma_VBarSet.Add(pVBar);
	}
	if (pVBar == nullptr)	return;

	nLoc = pVBar->ma_VBarLoc.GetSize();

	for (iLoc = 0; iLoc < nLoc; iLoc++)
	{
		double theta = fabs(Radian - pVBar->ma_VBarLoc[iLoc]);
		double arcLength = pSectCircle->m_A * theta;
		if(arcLength < dTol)	return;
	}

	for (iLoc = 0; iLoc < nLoc; iLoc++)
	{
		if (Radian < pVBar->ma_VBarLoc[iLoc])
		{
			pVBar->ma_VBarLoc.InsertAt(iLoc, Radian);
			break;
		}
	}
	if (iLoc == nLoc)
		pVBar->ma_VBarLoc.Add(Radian);
}

void MREdge::AddVBarLayer(double LayerDist, double dTol)
{
	long nLoc = ma_VBarSet.GetSize();
	for (long iLoc = 0; iLoc < nLoc; iLoc++)
	{
		MRVBar* pVBar = ma_VBarSet[iLoc];
		if (fabs(pVBar->m_LayerDistFromEdge - LayerDist) < dTol)
			break;
		if (LayerDist < pVBar->m_LayerDistFromEdge)
		{
			MRVBar* pNewVBar = new MRVBar;
			pNewVBar->m_LayerDistFromEdge = LayerDist;
			pNewVBar->mp_Edge = this;
			ma_VBarSet.InsertAt(iLoc, pNewVBar);
			break;
		}
	}
	if (iLoc == nLoc)
	{
		MRVBar* pNewVBar = new MRVBar;
		pNewVBar->m_LayerDistFromEdge = LayerDist;
		pNewVBar->mp_Edge = this;
		ma_VBarSet.Add(pNewVBar);
	}
}

bool MREdge::AddVBarLoc(GM2DVector& vVBar, double dTol)
{
	double LayerDist;
	double BarLoc;
	GM2DLineVector* pLine = NULL;
	if (mp_Curve->m_ObjType == GT_LINE)
	{
		pLine = (GM2DLineVector*)mp_Curve;
		LayerDist = pLine->Distance(vVBar);
		if (pLine->WhichSide(vVBar) != 1)
			return false;
		BarLoc = pLine->OrthoBaseLocValue(vVBar);
		if (BarLoc < -0.2 || BarLoc > 1.2)
			return false;
	}
	else
	{
		ASSERT(0);
		return false;
	}

	long nSet = ma_VBarSet.GetSize();
	for (long iSet = 0; iSet < nSet; iSet++)
	{
		MRVBar* pVBar = ma_VBarSet[iSet];
		if (fabs(pVBar->m_LayerDistFromEdge - LayerDist) < dTol)
		{
			long nBar = pVBar->ma_VBarLoc.GetSize();
			for (long iBar = 0; iBar < nBar; iBar++)
			{
				if (BarLoc < pVBar->ma_VBarLoc[iBar])
				{
					pVBar->ma_VBarLoc.InsertAt(iBar, BarLoc);
					return true;
				}
			}
			if (iBar == nBar)
			{
				pVBar->ma_VBarLoc.Add(BarLoc);
				return true;
			}
		}
	}

	return false;
}

long MREdge::GetNumVBarOfEdge()
{
	long nVBar = 0;
	long nLayer = ma_VBarSet.GetSize();
	for (long iLayer = 0; iLayer < nLayer; iLayer++)
		nVBar += ma_VBarSet[iLayer]->ma_VBarLoc.GetSize();
	return nVBar;
}

bool MREdge::SetVBarByCount(CString sDiaName, CString BarType)
{
	long nLayer = ma_VBarSet.GetSize();
	for (long iLayer = 0; iLayer < nLayer; iLayer++)
	{
		CString strVBar;
		strVBar.Format(_T("%d-%s%s"), ma_VBarSet[iLayer]->ma_VBarLoc.GetSize(), BarType, sDiaName);
		ma_VBarSet[iLayer]->SetBarString(strVBar);
	}

	return true;
}

bool MREdge::SetVBarByCount(long NumBar, CString sDiaName, CString BarType)
{
	CString strVBar;
	long NumAddedBar = 0;
	long nLayer = ma_VBarSet.GetSize();
	for (long iLayer = 0; iLayer < nLayer - 1; iLayer++)
	{
		strVBar.Format(_T("%d-%s%s"), ma_VBarSet[iLayer]->ma_VBarLoc.GetSize(), BarType, sDiaName);
		NumAddedBar += ma_VBarSet[iLayer]->ma_VBarLoc.GetSize();
		ma_VBarSet[iLayer]->SetBarString(strVBar);
	}
	strVBar.Format(_T("%d-%s%s"), NumBar - NumAddedBar, BarType, sDiaName);
	ma_VBarSet[iLayer]->SetBarString(strVBar);

	return true;
}

