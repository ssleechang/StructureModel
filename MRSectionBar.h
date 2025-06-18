#pragma once
#include "MRObject.h"

enum AnchorType 
{
	AnchorType_End = 0,
	AnchorType_Hook_90, 
	AnchorType_Hook_135, 
	AnchorType_Hook_225, 
	AnchorType_Hook_270,
	AnchorType_Splice,
};

class StructureModel_CLASS MRSectionBar : 	public MRObject
{
public:

	MRSectionBar(void);
	~MRSectionBar(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MRSectionBar)

	void SetBarString(CString strBar);
	virtual bool ParsingBarString();
	void SetDia(CString sDiaName){m_DiaName = sDiaName;};
	CString GetDia(){return m_DiaName;};
	CString GetBarString(){return m_strBar;};
private:
	CString m_strBar;
	CString m_DiaName;
};
