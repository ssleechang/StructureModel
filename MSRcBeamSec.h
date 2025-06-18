#pragma once
#include "MSRcObject.h"

#include "MSMainBar.h"
#include "MSStirrupBar.h"

#include "MSRebarShape.h"
#include "MSSubMainBar.h"
#include "MSMainBarSet.h"

class StructureModel_CLASS MSRcBeamSec : public MSRcObject
{
public:
	MSRcBeamSec(void);
	~MSRcBeamSec(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSRcBeamSec)
	virtual void CopyFromMe(MSRcBeamSec* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	long m_nLocation;     //1.All Section,  2.Mid & Bigger End, 3. Mid & Smaller End, 4.Both Ends, 5.Mid, 6.Smaller End, 7.Bigger End
	double m_dTopRange;		//상부근 구간대 비율 0~1
	double m_dBotRange;		//하부근 구간대 비율 0~1
	double m_dStirrupRange;		//스터럽 구간대 비율 0~1
	double m_dStirrupCapRatio; //스터럽 - 캡 비율 기본값 1, 0.5:스터럽 두개당 캡하나
	long m_nBeamType;//0:일반보 1:거더 2:내진보

	MSMainBarSet m_TBars;
	MSMainBarSet m_BBars;
	MSStirrupBar m_SBars;
	MSMainBar m_SideBars;
	long m_SBarNum;
	// 2013.09.10 - by Gracias
	BOOL m_bIsNumPlaceSideBar;
	MSStirrupBar m_SideStirrupBar;

	BOOL m_bIsNumPlaceShearBar;
	MSMainBar m_ShearBars;
	long m_nShearBarRate;

	// 보조주근 ( 균열 방지근)
	MSSubMainBar m_SubMainBar;

	MSREBAR_SHAPECODE m_CoverShapeCode;
	MSREBAR_SHAPECODE m_StirrupShapeCode;
	

	BOOL GetBarXPosition(double Width, double CvrThk, double* pBarX, double Factor, double DrawRatio);
	BOOL GetBarXPosition(double Width, double CvrThk, double BarDia, double* pBarX);
	BOOL GetBarXPositionInLayer(double* pTotalBarX, long LayerNum, long NumBar, CMap<long,long,long,long>& BarIndexMap, double* pCurBarX);
	BOOL GetShearBarXPosition(double* pTotalBarX, CMap<long,long,long,long>& ShearBarIndexMap, CMap<long,long,long,long>& ShearBarLeftMap);

	bool IsSameRebarData(MSRcBeamSec* pUrSec);

	double GetMaxMainBarDia(bool IsPc);
	vector<double> GetTBarsMaxDeformedBarDiameter();

	virtual vector<double> GetMainBarDiaAllLayer();
	vector<double> GetMainBarDiaAllLayerByPc();
	virtual vector<double> GetStirrupBarDiaAllLayer();
};
