#pragma once

#include "msrcobject.h"

#include "MSMainBar.h"
#include "MSStirrupBar.h"
#include "MSMainBarSet.h"
class StructureModel_CLASS MDRcButtressSec : public MSRcObject
{
public:
	MDRcButtressSec(void);
	~MDRcButtressSec(void);

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MDRcButtressSec)
	virtual void CopyFromMe(MDRcButtressSec* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetStirrupBarDiaAllLayer();

	MSMainBarSet m_TBars;
	MSMainBarSet m_BBars;
	MSStirrupBar m_SBars;
	int m_SBarNum;

	double GetMaxBarDia();
	double GetTMaxBarDia(int layerNum);
	double GetBMaxBarDia(int layerNum);
	double GetStrpDia();
	int GetSBarNum();
};

