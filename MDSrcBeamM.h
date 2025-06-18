#pragma once
#include "mdmember.h"
#include "MSRcBeamSec.h"


class StructureModel_CLASS MDSrcBeamM :
	public MDMember
{
public:
	MDSrcBeamM(void);
	~MDSrcBeamM(void);

	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MDSrcBeamM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	void CreateSections();
	MSRcBeamSec* mp_SmallSec;
	MSRcBeamSec* mp_MidSec;
	MSRcBeamSec* mp_LargeSec;

	BOOL m_bCrossBar;

	double GetMainBarDia();
	double getStrpBarDia();

	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetStirrupBarDiaAllLayer();

	bool GetIsCornerPlacing()								{ return m_IsCornerPlacing; };
	void SetIsCornerPlacing(bool IsCornerPlacing)	{ m_IsCornerPlacing = IsCornerPlacing; };
	bool HasSubMainBar();
	MSSubMainBar* GetSubMainBar();

	vector<CString> GetLinkedExteriors();
	vector<CString> GetLinkedInteriors();
	void SetLinkedExteriors(vector<CString>& linkeds);
	void SetLinkedInteriors(vector<CString>& linkeds);

protected:
	bool m_IsCornerPlacing; // true: 코너 기준 배근 / false: 등간격 배근

	vector<CString> ma_LinkedExterior;
	vector<CString> ma_LinkedInterior;

};

