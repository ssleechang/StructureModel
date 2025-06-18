#pragma once
#include "msmaterial.h"

class MSReinforceBarSetting;

class StructureModel_CLASS MSConcrete :
	public MSMaterial
{
public:
	MSConcrete(void);
	~MSConcrete(void);
	void Serialize( CArchive& archive );
	virtual void GlobalOptionSerialize( CArchive& archive );
	DECLARE_SERIAL(MSConcrete)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);

	virtual double GetFck();
	virtual CString GetFckName();
	virtual double GetConcStrength()	{ return m_ConcStrength; };
	virtual MSReinforceBarSetting* GetReinforceBarSetting();

	double m_ConcStrength;

	double m_Es;
	CString m_ConcKind;
	CString GetConcKind();
	CString GetConcKindAggregateSlump();
	double m_UnitCostConc;
	double m_UnitCostReBar;
	double m_Aggregate;		// 굵은골재
	double m_Slump;				// 슬럼프
	MSReinforceBarSetting* m_pReinforceBarSetting;
	virtual bool IsPlainConcreteReinforceBar() override;

//	double m_MBarStrength;
//	double m_HBarStrength;
//	CString m_ConcStandard;
//	CString m_ReBarStandard;
//	CString m_HBarKind;
//	CString m_MBarKind;
};
