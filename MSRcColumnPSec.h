#pragma once
#include "msrcpolysection.h"

#include "MSMainBar.h"
#include "MSStirrupBar.h"

class MSRcColumnPSec :
	public MSRcPolySection
{
public:
	MSRcColumnPSec(void);
	~MSRcColumnPSec(void);

	vector<MSMainBar> ma_MainBar;

	//단면정보
	MSStirrupBar m_EndHoopBar;	
	MSStirrupBar m_CenHoopBar;
	BOOL m_nDoubleHoopType;             // 0 :일반,  1:이중HOOP배근    
	BOOL m_nHoopType;              //long m_nHoopType; // 0 Hoop:   1 : 쪽후프
	long m_nTieRowNum;                  //long m_XStirrupNum;  X축방향 Stirrup수
	long m_nTieColNum;									//long m_YStirrupNum;  Y축방향 Stirrup수

	//배근정보
	long m_nDowelSpliceType;                 // 0:몰이음, 1: 엇이음
	long m_nDowelSpliceDetail;               // 0: 커플러  1: 가스용접
	long m_nSpliceLengthA;                   // 엇이음 길이A
	long m_nSpliceLengthB;                   // 엇이음 길이B
};
