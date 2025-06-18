#pragma once
#include "mdrcstairm.h"

class StructureModel_CLASS MDRcStairStepM : public MDRcStairM
{
public:
	MDRcStairStepM(void);
	~MDRcStairStepM(void);

	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MDRcStairStepM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetStirrupBarDiaAllLayer();
};


