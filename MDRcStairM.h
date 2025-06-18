#pragma once
#include "mdmember.h"
#include "MSStirrupBar.h"
#include "MSMainBar.h"

class StructureModel_CLASS MDRcStairM : public MDMember
{
public:
	MDRcStairM(void);
	~MDRcStairM(void);

	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MDRcStairM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 


	MSStirrupBar m_TopX;
	MSStirrupBar m_TopY;
	MSStirrupBar m_BotX;
	MSStirrupBar m_BotY;
	MSMainBar m_ReinforceBar;
	
	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetMainBarDiaAllLayerByStep();
	virtual vector<double> GetMainBarDiaAllLayerByLanding();
	virtual vector<double> GetStirrupBarDiaAllLayer();

	int GetStepRebarPlaceType();
	void SetStepRebarPlaceType(int stepRebarPlaceType);

	bool PlaceTopXBar();
	bool PlaceTopYBar();
	bool PlaceReinforceBar();

private:
	int m_nStepRebarPlaceType; //0:A 1:B
};

