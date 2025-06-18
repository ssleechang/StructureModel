#pragma once
#include "MSCompoM.h"

class StructureModel_CLASS MSFWTrenchM :public MSCompoM
{
public:
	MSFWTrenchM();
	~MSFWTrenchM();

	enum eFWTrenchType { General = 0, };
	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSFWTrenchM)
	virtual void CopyFromMe(MSCompoM* pSource);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	static CString ToShapeName(eFWTrenchType eType);
	virtual CString GetShapeName();

	void MakeProfile();
	bool CheckProfile();

	GM2DPolyline* GetProfile() { return m_pProfile; };

	eFWTrenchType GetFWTrenchType() { return m_eTrenchType; };
	virtual double GetWidth() { return GetW(); };
	virtual double GetDepth() { return GetT(); };
	virtual double GetThick() {return GetT();};
	virtual void SetWidth(double dWidth);
	virtual void SetDepth(double dDepth);
	virtual void SetThick(double dThick);

	virtual double GetW() { return m_W; };
	virtual double GetT() { return m_T; };

	void SetFWTrenchType(eFWTrenchType type) { m_eTrenchType = type; };
	virtual void SetW(double param);
	virtual void SetT(double param);

protected:
	void makeProfile_General();
	bool CheckProfile_General();

	GM2DPolyline* m_pProfile;
	eFWTrenchType m_eTrenchType;
	double m_W, m_T;
};

