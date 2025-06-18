#pragma once
#include "msrcobject.h"

class StructureModel_CLASS MSSpecificTieBar : public MSRcObject
{
public:
	MSSpecificTieBar(void);
	~MSSpecificTieBar(void);

	enum eEdge { Bottom = 0, Right, Top, Left, Circle };

	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSSpecificTieBar)
	virtual void CopyFromMe(MSSpecificTieBar* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	CString m_sLoc;
	double m_dDia;
	

	bool AnalyizeTieBarByRecoginze(int StartEdge, int StartRebar, int EndEdge, int EndRebar);
	CString GetEdgeKeyName(int EdgeIndex);
	
	bool GetUserSteelGradeS();
	void SetUserSteelGradeS(bool value);
private:
	bool m_bUserSteelGradeS;
};

