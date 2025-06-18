#pragma once
#include "dtobject.h"
class StructureModel_CLASS DTSpacer :
	public DTObject
{
public:
	DTSpacer(void);
	virtual ~DTSpacer(void);


protected:

	long m_nShapeCode;//철근형상
	//Spacer의 Depth(유지시킬 폭)를 산정하는 방법
	SpacerDepthType me_SpacerDepthType;
	CoverType me_CoverType;

	double m_dUserCover;
	double m_dUserDepth;

	//발길이 산정방법
	SpacerLegLenType me_SpacerLegLenType1;
	SpacerLegLenType me_SpacerLegLenType2;

	//간격제의 발길이 계산방법
	//SpacerLegLenType == SpacerLegLenType_Spacing_Ratio일때 
	//dLegLen = 최대간격*m_dLegLen1
	//SpacerLegLenType == SpacerLegLenType_Spacing_AddLen일때 
	//dLegLen = 최대간격+m_dLegLen1
	//SpacerLegLenType == SpacerLegLenType_User일때 
	//dLegLen = m_dLegLen1
	double m_dLegLen1;
	double m_dLegLen2;



};

