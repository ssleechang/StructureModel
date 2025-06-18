#include "StdAfx.h"
#include "MSRcBeamSec.h"
#include "MSMainBar.h"
#include "GMLib/MSVersionInfo.h"
#include "MSDeformedBar.h"

IMPLEMENT_SERIAL(MSRcBeamSec, MSObject, VERSIONABLE_SCHEMA | 1)
MSRcBeamSec::MSRcBeamSec(void)
{
	CString sBar = _T("");
	CString sDefDiaName = MSDeformedBar::GetDefaultRebarName();

	m_nLocation = 1;
	m_dTopRange = 0;		
	m_dBotRange = 0;		
	m_dStirrupRange = 0;
	m_dStirrupCapRatio = 0;
	m_nBeamType = 0;
	sBar = MSDeformedBar::GetMainBarStr(2);
	m_TBars.InitData(sBar);
	m_BBars.InitData(sBar);
	sBar = MSDeformedBar::GetMainBarStr(0);
	m_SideBars.InitData(sBar);
	sBar = MSDeformedBar::GetStirrupBarStr(300);
	m_SBars.InitData(sBar);
	
	m_CoverShapeCode = MST1_09;
	m_StirrupShapeCode = MST3_00;

	m_SBarNum = 2;
	m_bIsNumPlaceSideBar = TRUE;
	sBar = MSDeformedBar::GetStirrupBarStr(200);
	m_SideStirrupBar.InitData(sBar);

	sBar = MSDeformedBar::GetMainBarStr(0);
	m_SubMainBar.m_MainBar.InitData(sBar);

	m_bIsNumPlaceShearBar = TRUE;
	sBar = MSDeformedBar::GetMainBarStr(0);
	m_ShearBars.InitData(sBar);
	m_nShearBarRate = 1;
}

MSRcBeamSec::~MSRcBeamSec(void)
{
}

void MSRcBeamSec::CopyFromMe(MSRcBeamSec* pSource)
{
	if(pSource==NULL)	return; 
	m_nLocation = pSource->m_nLocation;
	m_dTopRange = pSource->m_dTopRange;
	m_dBotRange = pSource->m_dBotRange;
	m_dStirrupRange = pSource->m_dStirrupRange;
	m_dStirrupCapRatio = pSource->m_dStirrupCapRatio;
	m_SBarNum = pSource->m_SBarNum;
	m_TBars.CopyFromMe(&pSource->m_TBars);
	m_BBars.CopyFromMe(&pSource->m_BBars);
	m_SBars.CopyFromMe(&pSource->m_SBars);
	m_SideBars.CopyFromMe(&pSource->m_SideBars);

	m_CoverShapeCode = pSource->m_CoverShapeCode;
	m_StirrupShapeCode = pSource->m_StirrupShapeCode;

	m_bIsNumPlaceSideBar = pSource->m_bIsNumPlaceSideBar;
	m_SideStirrupBar.CopyFromMe(&pSource->m_SideStirrupBar);
	m_SubMainBar.CopyFromMe(&pSource->m_SubMainBar);

	m_bIsNumPlaceShearBar = pSource->m_bIsNumPlaceShearBar;
	m_ShearBars.CopyFromMe(&pSource->m_ShearBars);
	m_nShearBarRate = pSource->m_nShearBarRate;
}

void MSRcBeamSec::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	buffer << m_SBarNum << m_bIsNumPlaceSideBar;
	m_TBars.Dump(buffer);
	m_BBars.Dump(buffer);
	m_SBars.Dump(buffer);
	m_SideBars.Dump(buffer);
	m_SideStirrupBar.Dump(buffer);
	m_SubMainBar.Dump(buffer);
	buffer << m_bIsNumPlaceShearBar << m_nShearBarRate;
	m_ShearBars.Dump(buffer);
}
void MSRcBeamSec::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	buffer >> m_SBarNum >> m_bIsNumPlaceSideBar;
	m_TBars.Recovery(buffer);
	m_BBars.Recovery(buffer);
	m_SBars.Recovery(buffer);
	m_SideBars.Recovery(buffer);
	m_SideStirrupBar.Recovery(buffer);
	m_SubMainBar.Recovery(buffer);
	buffer >> m_bIsNumPlaceShearBar >> m_nShearBarRate;
	m_ShearBars.Recovery(buffer);
}

void MSRcBeamSec::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	MSMainBar TBars, BBars;
	if(ar.IsStoring())
	{
		ar << m_nLocation;
		ar << m_dTopRange;
		ar << m_dBotRange;
		ar << m_dStirrupRange;
		ar << m_dStirrupCapRatio;
		ar << m_SBarNum;
		ar << m_CoverShapeCode;
		ar << m_StirrupShapeCode;
	
		TBars.Serialize(ar);
		BBars.Serialize(ar);

		m_SBars.Serialize(ar);
		m_SideBars.Serialize(ar);

		ar << m_bIsNumPlaceSideBar;
		m_SideStirrupBar.Serialize(ar);
		m_SubMainBar.Serialize(ar);

		m_TBars.Serialize(ar);
		m_BBars.Serialize(ar);

		ar << m_bIsNumPlaceShearBar;
		m_ShearBars.Serialize(ar);
		ar << m_nShearBarRate;
	}
	else
	{
		long CoverShapeCode = 0, StirrupShapeCode = 0;

		ar >> m_nLocation;
		ar >> m_dTopRange;
		ar >> m_dBotRange;
		ar >> m_dStirrupRange;
		ar >> m_dStirrupCapRatio;
		ar >> m_SBarNum;
		
		ar >> CoverShapeCode;
		m_CoverShapeCode = (MSREBAR_SHAPECODE)CoverShapeCode;
 		ar >> StirrupShapeCode;
		m_StirrupShapeCode = (MSREBAR_SHAPECODE)StirrupShapeCode;

		TBars.Serialize(ar);
		BBars.Serialize(ar);

		m_SBars.Serialize(ar);
		m_SideBars.Serialize(ar);

		ar >> m_bIsNumPlaceSideBar;
		m_SideStirrupBar.Serialize(ar);
		if(MSVersionInfo::GetCurrentVersion() >= 20150811)
		{
			m_SubMainBar.Serialize(ar);
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20160701)
		{
			m_TBars.Serialize(ar);
			m_BBars.Serialize(ar);
		}
		else
		{
			m_TBars.SetDataByOldVersion(&TBars);
			m_BBars.SetDataByOldVersion(&BBars);
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20200111)
		{
			ar >> m_bIsNumPlaceShearBar;
			m_ShearBars.Serialize(ar);
			ar >> m_nShearBarRate;
		}
	}
}

BOOL MSRcBeamSec::GetBarXPosition(double Width, double CvrThk, double* pBarX, double Factor, double DrawRatio)
{
	long NumBarOnTopLayer = m_TBars.GetNumBarsOnSet(0);
	long NumBarOnBotLayer = m_BBars.GetNumBarsOnSet(0);
	long MaxBarOnLayer1 = max(NumBarOnTopLayer, NumBarOnBotLayer);
	if (MaxBarOnLayer1 == 1)	MaxBarOnLayer1 = 2;

	double dBarX = 0.0;
	long BarIndex = 0;
	MSMainBarSet* pMaxBar;
	if(NumBarOnTopLayer >= NumBarOnBotLayer)
		pMaxBar = &m_TBars;
	else
		pMaxBar = &m_BBars;

	long NumBar = 0;
	long NumBundle = 0, NumBundleOnALayer = 0;
	double BarDiasLengthOnLayer = 0.0;

	NumBar = pMaxBar->GetNumBarsOnSet(0);
	BarDiasLengthOnLayer = NumBar * pMaxBar->GetMaxDeformedBarDiameterOnSet(0) * Factor;

	double BarSpace = (double)(Width - 2*CvrThk - BarDiasLengthOnLayer) / (double)(MaxBarOnLayer1 - 1);

	double BarDia = pMaxBar->GetMaxDeformedBarDiameterOnSet(0)*Factor;
	double dStart = -Width / 2. + CvrThk + BarDia/2.;
	dBarX = dStart*DrawRatio;
	for(long BarNum = 1; BarNum <= NumBar; BarNum++)
	{
		pBarX[BarIndex] = dBarX;
		dBarX += (BarSpace+BarDia) * DrawRatio;

		BarIndex++;
	}
	return TRUE;
}
BOOL MSRcBeamSec::GetBarXPosition(double Width, double CvrThk, double BarDia, double* pBarX)
{
	long NumBarOnTopLayer = m_TBars.GetNumBarsOnSet(0);
	long NumBarOnBotLayer = m_BBars.GetNumBarsOnSet(0);
	long MaxBarOnLayer1 = max(NumBarOnTopLayer, NumBarOnBotLayer);
	if (MaxBarOnLayer1 == 1)	MaxBarOnLayer1 = 2;
	double dBarX = 0.0;
	long BarIndex = 0;
	MSMainBarSet* pMaxBar;
	if(NumBarOnTopLayer >= NumBarOnBotLayer)
		pMaxBar = &m_TBars;
	else
		pMaxBar = &m_BBars;

	long NumBar = 0;
	long NumBundle = 0, NumBundleOnALayer = 0;
	double BarDiasLengthOnLayer = 0.0;

	NumBar = pMaxBar->GetNumBarsOnSet(0);
	BarDiasLengthOnLayer = NumBar * BarDia;
	if (MaxBarOnLayer1 == 1)	MaxBarOnLayer1 = 2;
	double BarSpace = (double)(Width - 2*CvrThk - BarDiasLengthOnLayer) / (double)(MaxBarOnLayer1 - 1);

	NumBar = pMaxBar->GetNumBarsOnSet(0);
	double dStart = -Width / 2. + CvrThk + BarDia/2.;
	dBarX = dStart;
	for(long BarNum = 1; BarNum <= NumBar; BarNum++)
	{
		pBarX[BarIndex] = dBarX;
		dBarX += BarSpace+BarDia;

		BarIndex++;
	}
	return TRUE;
}
BOOL MSRcBeamSec::GetBarXPositionInLayer(double* pTotalBarX, long LayerNum, long NumBar, CMap<long,long,long,long>& BarIndexMap, double* pCurBarX)
{
	BarIndexMap.RemoveAll();

	long NumBarOnTopLayer = m_TBars.GetNumBarsOnSet(0);
	long NumBarOnBotLayer = m_BBars.GetNumBarsOnSet(0);
	long MaxBarOnLayer1 = max(NumBarOnTopLayer, NumBarOnBotLayer);

	long BarIndex = 0;
	if(LayerNum > 1)
	{
		BOOL bCenter = FALSE;
		if((MaxBarOnLayer1%2!=0) && (NumBar%2!=0)) bCenter = TRUE;
		for(long BarNum = 1; BarNum <= NumBar; BarNum++)
		{
			if(bCenter && BarNum==NumBar)
				pCurBarX[BarNum-1] = pTotalBarX[(MaxBarOnLayer1/2)];
			else if(BarNum%2==0)
			{
				pCurBarX[BarNum-1] = pTotalBarX[MaxBarOnLayer1-(BarNum/2)];
			}
			else
			{
				pCurBarX[BarNum-1] = pTotalBarX[(BarNum/2)];
			}
			BarIndexMap.SetAt(BarNum, BarIndex);
			BarIndex++;
		}
	}
	else if(m_SBarNum==2)
	{
		double dLX = pTotalBarX[0];
		double dRX = pTotalBarX[MaxBarOnLayer1-1];

		double dNetSpace = (dRX-dLX);
		double BarSpace = (NumBar <= 1 ? dNetSpace : dNetSpace / (double)(NumBar-1));

		for(long BarNum = 1; BarNum <= NumBar; BarNum++)
		{
			pCurBarX[BarNum-1] = dLX + BarSpace*(BarNum-1);
			BarIndexMap.SetAt(BarNum, BarIndex);
			BarIndex++;
		}
	}
	else
	{
		for(long iX = 0; iX < MaxBarOnLayer1; iX++)
		{
			pCurBarX[iX] = pTotalBarX[iX];
		}

		if(MaxBarOnLayer1==NumBar)
		{
			for(long BarNum = 1; BarNum <= NumBar; BarNum++)
			{
				BOOL bLeft = FALSE;

				if(BarNum==1)
				{
					BarIndex = 0;
					bLeft = TRUE;
				}
				else if(BarNum==2) 
				{
					BarIndex = MaxBarOnLayer1 - 1;
					bLeft = TRUE;
				}
				else
				{
					long iMidIdx = BarNum-2;
					BOOL bOdd = ((NumBar-2)%2==0 ? FALSE : TRUE);
					if(NumBar==3)
						BarIndex = (long)(MaxBarOnLayer1/2);
					else
					{
						if(bOdd)
						{
							if(iMidIdx==1)
							{
								BarIndex = (long)(MaxBarOnLayer1/2);
							}
							else
							{
								long nSide = (long)((iMidIdx-2)/2)+1;
								BarIndex = (long)(MaxBarOnLayer1/((NumBar-2)+1))*nSide;
								bLeft = (iMidIdx%2==0 ? TRUE : FALSE);
							}
						}
						else
						{
							long nSide = (long)((iMidIdx-1)/2)+1;
							BarIndex = (long)(MaxBarOnLayer1/((NumBar-2)+1))*nSide;
							bLeft = (iMidIdx%2==0 ? FALSE : TRUE);
						}
					}
					if(bOdd) bLeft = (iMidIdx%2==0 ? FALSE : TRUE);
				}

				if(bLeft)
					BarIndexMap.SetAt(BarNum, BarIndex);
				else
					BarIndexMap.SetAt(BarNum, (MaxBarOnLayer1-1)-BarIndex);
			}
		}
		else
		{
			CMap<long,long,long,long> TempBarIndexMap;
			CMap<long,long,long,long> ShearIndexMap;

			long BarCount = 0;

			for(long BarNum = 1; BarNum <= NumBar; BarNum++)
			{
				BOOL bLeft = FALSE;

				if(BarNum==1)
				{
					BarIndex = 0;
					BarIndexMap.SetAt(BarCount+1, BarIndex);
					BarCount++;
					continue;
				}
				else if(BarNum==2) 
				{
					BarIndex = MaxBarOnLayer1 - 1;
					BarIndexMap.SetAt(BarCount+1, BarIndex);
					BarCount++;
					continue;
				}
				else
				{
					long iMidIdx = BarNum-2;
					BOOL bOdd = ((NumBar-2)%2==0 ? FALSE : TRUE);
					if(NumBar==3)
						BarIndex = (long)(MaxBarOnLayer1/2);
					else
					{
						if(bOdd)
						{
							if(iMidIdx==1)
							{
								BarIndex = (long)(MaxBarOnLayer1/2);
							}
							else
							{
								long nSide = (long)((iMidIdx-2)/2)+1;
								BarIndex = (long)(MaxBarOnLayer1/((NumBar-2)+1))*nSide;
								bLeft = (iMidIdx%2==0 ? TRUE : FALSE);
							}
						}
						else
						{
							long nSide = (long)((iMidIdx-1)/2)+1;
							BarIndex = (long)(MaxBarOnLayer1/((NumBar-2)+1))*nSide;
							bLeft = (iMidIdx%2==0 ? FALSE : TRUE);
						}
					}
					if(bOdd) bLeft = (iMidIdx%2==0 ? FALSE : TRUE);
				}

				if(bLeft)
					TempBarIndexMap.SetAt(BarIndex, BarIndex);
				else
					TempBarIndexMap.SetAt((MaxBarOnLayer1-1)-BarIndex, (MaxBarOnLayer1-1)-BarIndex);
			}

			if(m_SBarNum > 2)
			{
				long NumSide = 0;
				long NumStirrup = m_SBarNum - 2;
				for(long iStirrup=1; iStirrup<=NumStirrup; iStirrup++)
				{
					BOOL bOdd = (NumStirrup%2==0 ? FALSE : TRUE);
					BOOL bLeft = FALSE;

					if(NumStirrup==1)
					{
						NumSide = (long)(MaxBarOnLayer1/2);
					}
					else
					{
						if(bOdd)
						{
							if(iStirrup==1)
							{
								NumSide = (long)(MaxBarOnLayer1/2);
							}
							else
							{
								long nSide = (long)((iStirrup-2)/2)+1;
								NumSide = (long)(MaxBarOnLayer1/(NumStirrup+1))*nSide;
								bLeft = (iStirrup%2==0 ? TRUE : FALSE);
							}
						}
						else
						{
							long nSide = (long)((iStirrup-1)/2)+1;
							NumSide = (long)(MaxBarOnLayer1/(NumStirrup+1))*nSide;
							bLeft = (iStirrup%2==0 ? FALSE : TRUE);
						}
					}

					if(bOdd) bLeft = (iStirrup%2==0 ? FALSE : TRUE);
					if(bLeft)
					{
						BarIndexMap.SetAt(BarCount+1, NumSide);
						ShearIndexMap.SetAt(NumSide, NumSide);
					}
					else
					{
						BarIndexMap.SetAt(BarCount+1, (MaxBarOnLayer1-1)-NumSide);
						ShearIndexMap.SetAt((MaxBarOnLayer1-1)-NumSide, (MaxBarOnLayer1-1)-NumSide);
					}

					BarCount++;
				}
			}

			//스터럽과 겹치는 지점은 제거 
			long rBarIndex = 0;
			POSITION Pos = ShearIndexMap.GetStartPosition();
			while(Pos)
			{
				ShearIndexMap.GetNextAssoc(Pos, BarIndex, BarIndex);

				if(TempBarIndexMap.Lookup(BarIndex, rBarIndex))
				{
					TempBarIndexMap.RemoveKey(BarIndex);
				}
			}

			Pos = TempBarIndexMap.GetStartPosition();
			while(Pos)
			{
				if(BarCount==NumBar) break;
				TempBarIndexMap.GetNextAssoc(Pos, BarIndex, rBarIndex);

				BarIndexMap.SetAt(BarCount+1, BarIndex);
				BarCount++;
			}
		}
	}

	return TRUE;
}
BOOL MSRcBeamSec::GetShearBarXPosition(double* pTotalBarX, CMap<long,long,long,long>& ShearBarIndexMap, CMap<long,long,long,long>& ShearBarLeftMap)
{
	ShearBarIndexMap.RemoveAll();
	ShearBarLeftMap.RemoveAll();

	long NumBarOnTopLayer = m_TBars.GetNumBarsOnSet(0);
	long NumBarOnBotLayer = m_BBars.GetNumBarsOnSet(0);
	long MaxBarOnLayer1 = max(NumBarOnTopLayer, NumBarOnBotLayer);

	if(m_SBarNum > 2)
	{
		long NumSide = 0;
		long NumStirrup = m_SBarNum - 2;
		for(long iStirrup=1; iStirrup<=NumStirrup; iStirrup++)
		{
			double TopX = 0.0, BotX=0.0;
			BOOL bOdd = (NumStirrup%2==0 ? FALSE : TRUE);
			BOOL bLeft = FALSE;

			if(NumStirrup==1)
			{
				NumSide = (long)(MaxBarOnLayer1/2);
			}
			else
			{
				if(bOdd)
				{
					if(iStirrup==1)
					{
						NumSide = (long)(MaxBarOnLayer1/2);
					}
					else
					{
						long nSide = (long)((iStirrup-2)/2)+1;
						NumSide = (long)(MaxBarOnLayer1/(NumStirrup+1))*nSide;
						bLeft = (iStirrup%2==0 ? TRUE : FALSE);
					}
				}
				else
				{
					long nSide = (long)((iStirrup-1)/2)+1;
					NumSide = (long)(MaxBarOnLayer1/(NumStirrup+1))*nSide;
					bLeft = (iStirrup%2==0 ? FALSE : TRUE);
				}
			}

			if(bOdd) bLeft = (iStirrup%2==0 ? FALSE : TRUE);
			if(bLeft)
			{
				ShearBarIndexMap.SetAt(iStirrup, NumSide);
			}
			else
			{
				ShearBarIndexMap.SetAt(iStirrup, (MaxBarOnLayer1-1)-NumSide);
			}
			ShearBarLeftMap.SetAt(iStirrup, bLeft);
		}
	}

	return TRUE;
}

bool MSRcBeamSec::IsSameRebarData(MSRcBeamSec* pUrSec)
{
	if(pUrSec == NULL)
		return false;

	if(!m_TBars.IsSameBar(pUrSec->m_TBars))	return false;
	if(!m_BBars.IsSameBar(pUrSec->m_BBars))	return false;
	if(!m_SBars.IsSameBar(pUrSec->m_SBars))	return false;
	if(m_SBarNum != pUrSec->m_SBarNum)	return false;

	if(m_bIsNumPlaceSideBar != pUrSec->m_bIsNumPlaceSideBar)
		return false;
	if(m_bIsNumPlaceSideBar)
	{
		if(!m_SideBars.IsSameBar(pUrSec->m_SideBars))	
		return false;
	}
	else
	{
		if(!m_SideStirrupBar.IsSameBar(pUrSec->m_SideStirrupBar))
			return false;
	}

	if (m_bIsNumPlaceShearBar != pUrSec->m_bIsNumPlaceShearBar)
		return false;
	if (m_bIsNumPlaceShearBar)
	{
		if (!m_ShearBars.IsSameBar(pUrSec->m_ShearBars))
			return false;
	}
	else
	{
		if (m_nShearBarRate != m_nShearBarRate)
			return false;
	}

	return true;
}

vector<double> MSRcBeamSec::GetMainBarDiaAllLayer()
{
	vector<double> DiaArr;
	
	vector<double> TBarDiaIdxArr = m_TBars.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), TBarDiaIdxArr.begin(), TBarDiaIdxArr.end());
	
	vector<double> BBarDiaIdxArr = m_BBars.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), BBarDiaIdxArr.begin(), BBarDiaIdxArr.end());
	
	if(m_bIsNumPlaceSideBar)
	{
		vector<double> SideBarDiaIdxArr = m_SideBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), SideBarDiaIdxArr.begin(), SideBarDiaIdxArr.end());
	}
	else
	{
		vector<double> SideBarDiaIdxArr = m_SideStirrupBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), SideBarDiaIdxArr.begin(), SideBarDiaIdxArr.end());
	}

	if (m_bIsNumPlaceShearBar)
	{
		long nTotalBars = m_ShearBars.GetTotalBars();
		if (nTotalBars > 0)
		{
			vector<double> ShearBarDiaIdxArr = m_ShearBars.GetBarDiaAllLayer();
			DiaArr.insert(DiaArr.end(), ShearBarDiaIdxArr.begin(), ShearBarDiaIdxArr.end());
		}
	}
	else
	{
		vector<double> ShearBarDiaIdxArr = m_ShearBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), ShearBarDiaIdxArr.begin(), ShearBarDiaIdxArr.end());
	}

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const double& a, const double& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MSRcBeamSec::GetMainBarDiaAllLayerByPc()
{
	vector<double> DiaArr;

	vector<double> TBarDiaIdxArr = m_TBars.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), TBarDiaIdxArr.begin(), TBarDiaIdxArr.end());

	if (m_bIsNumPlaceSideBar)
	{
		vector<double> SideBarDiaIdxArr = m_SideBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), SideBarDiaIdxArr.begin(), SideBarDiaIdxArr.end());
	}
	else
	{
		vector<double> SideBarDiaIdxArr = m_SideStirrupBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), SideBarDiaIdxArr.begin(), SideBarDiaIdxArr.end());
	}

	sort(DiaArr.begin(), DiaArr.end(),
		[](const double& a, const double& b)->bool {return a < b; }
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MSRcBeamSec::GetStirrupBarDiaAllLayer()
{
	vector<double> SBarDiaIdxArr = m_SBars.GetBarDiaAllLayer();
	return SBarDiaIdxArr;
}

double MSRcBeamSec::GetMaxMainBarDia(bool IsPc)
{
	vector<double> DiaArr;
	if (IsPc)
		DiaArr = GetMainBarDiaAllLayerByPc();
	else
		DiaArr = GetMainBarDiaAllLayer();
	long NumDia = DiaArr.size();
	if (NumDia == 0)
		return MSDeformedBar::GetMinDia();

	return DiaArr[NumDia - 1];
}

vector<double> MSRcBeamSec::GetTBarsMaxDeformedBarDiameter()
{
	vector<double> DiaArr;

	vector<double> TBarDiaIdxArr = m_TBars.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), TBarDiaIdxArr.begin(), TBarDiaIdxArr.end());

	return DiaArr;
}