#pragma once
#include "MSCompoM.h"

class StructureModel_CLASS MSFWSoundAbsorptionM :public MSCompoM
{
public:
	MSFWSoundAbsorptionM();
	~MSFWSoundAbsorptionM();

	enum eFWSoundAbsorptionType { General = 0, };
	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSFWSoundAbsorptionM)
	virtual void CopyFromMe(MSCompoM* pSource);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	static CString ToShapeName(eFWSoundAbsorptionType eType);
	virtual CString GetShapeName();

	void MakeProfile();
	GM2DPolyline* GetProfile() { return m_pProfile; };

	eFWSoundAbsorptionType GetFWSoundAbsorptionType() { return m_eSoundAbsorptionType; };
	virtual double GetT() { return m_T; };
	virtual double GetThick()	override { return GetT();};

	void SetFWSoundAbsorptionType(eFWSoundAbsorptionType type) { m_eSoundAbsorptionType = type; };
	virtual void SetT(double param);
	virtual void SetThick(double param) override { SetT(param); };

protected:
	void makeProfile_General();
protected:
	GM2DPolyline* m_pProfile;
	eFWSoundAbsorptionType m_eSoundAbsorptionType;
	double m_T;
};

