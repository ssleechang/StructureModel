#pragma once
#include "MDMember.h"

#include "MSStirrupBar.h"
#include "MSMainBar.h"

#include "MSRebarShape.h"

class StructureModel_CLASS MSRcLevelDifM : public MDMember
{
public:
	MSRcLevelDifM(void);
	~MSRcLevelDifM(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSRcLevelDifM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	//벽처럼 배근시
	MSStirrupBar m_InHBar;
	MSStirrupBar m_InVBar;
	MSStirrupBar m_OutHBar;
	MSStirrupBar m_OutVBar;

	//보처럼 배근시
	MSMainBar m_TBars;
	MSMainBar m_BBars;
	MSMainBar m_SideBars;
	MSStirrupBar m_StirrupBar;
	long m_SBarNum;
	MSREBAR_SHAPECODE m_CoverShapeCode;
	MSREBAR_SHAPECODE m_StirrupShapeCode;

	long m_IsHaunch;//0:Haunch없음. 1:있음(헌치각도는 항상 45도임)
	long m_HookType; //0:이음정착 1:표준후크
	long m_ObjectType;//0:벽처럼배근 1: 보처럼
	long m_PlaceBarType;

	double m_dLevel;//단차높이
	virtual double GetLevel(){return m_dLevel;};
	virtual void SetLevel(double dLevel){m_dLevel=dLevel;};

	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetStirrupBarDiaAllLayer();
};
