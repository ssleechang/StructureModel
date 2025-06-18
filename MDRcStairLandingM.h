#pragma once
#include "mdrcstairm.h"

class StructureModel_CLASS MDRcStairLandingM : public MDRcStairM
{
public:
	MDRcStairLandingM(void);
	~MDRcStairLandingM(void);

	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MDRcStairLandingM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetStirrupBarDiaAllLayer();
};

