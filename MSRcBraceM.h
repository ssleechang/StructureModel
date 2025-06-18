#pragma once
#include "MDMember.h"

#include "MSMainBar.h"
#include "MSStirrupBar.h"

class StructureModel_CLASS MSRcBraceM :	public MDMember
{
public:
	MSRcBraceM(void);
	~MSRcBraceM(void);
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSRcBraceM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	MSMainBar m_TBars;
	MSMainBar m_BBars;
	MSMainBar m_LBars;
	MSMainBar m_RBars;
	MSStirrupBar m_SBars;
	long m_nStirrupType;                 // 0 : Tie,  1 : Spiral
	long m_nXStirrupNum;                 // X축방향 Stirrup수
	long m_nYStirrupNum;                 // Y축방향 Stirrup수

	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetStirrupBarDiaAllLayer();
};
