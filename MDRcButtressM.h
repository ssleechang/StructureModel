#pragma once
#include "mdmember.h"

#include "MSMainBar.h"
#include "MSStirrupBar.h"
#include "MSRebarShape.h"

class MDRcButtressSec;
class StructureModel_CLASS MDRcButtressM : public MDMember
{
public:
	MDRcButtressM(void);
	~MDRcButtressM(void);

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MDRcButtressM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	void CreateSections();
	MDRcButtressSec* mp_TopSec;
	MDRcButtressSec* mp_CenSec;
	MDRcButtressSec* mp_BotSec;

	bool m_IsNumPlaceSideBar;
	MSMainBar m_SideMainBars;		//m_IsNumPlaceSideBar = true
	MSStirrupBar m_SideStirrupBars; //m_IsNumPlaceSideBar = false
		
	BOOL m_bIsNumPlaceShearBar;
	MSMainBar m_ShearBars;
	long m_nShearBarRate;

	MSREBAR_SHAPECODE m_CoverShapeCode;
	MSREBAR_SHAPECODE m_StirrupShapeCode;

	long GetMaxTopLayer();
	long GetMaxBotLayer();

	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetStirrupBarDiaAllLayer();

	double GetMainBarDia();
	double GetStrpBarDia();
	double GetTopBarDia(int layerNum);
	double GetBotBarDia(int layerNum);

	double GetTopLayerDepth(long LayerNum, double dCover);
	double GetBotLayerDepth(long LayerNum, double dCover);
	long GetNumSideBar(double dDepth, double dCover);

	long GetNumHorShearBar(double dDepth, double dCover);
	double GetHorShearBarDia();
};