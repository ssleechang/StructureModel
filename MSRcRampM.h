#pragma once
#include "MDMember.h"

#include "MSStirrupBar.h"

class StructureModel_CLASS MSRcRampM :
	public MDMember
{
public:
	MSRcRampM(void);
	~MSRcRampM(void);
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSRcRampM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	MSStirrupBar m_XTopBar;
	MSStirrupBar m_XBotBar;
	MSStirrupBar m_YTopBar;
	MSStirrupBar m_YBotBar;

	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetStirrupBarDiaAllLayer();
};
