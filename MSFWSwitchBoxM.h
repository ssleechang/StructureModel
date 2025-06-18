#pragma once
#include "MSOpeningM.h"

class StructureModel_CLASS MSFWSwitchBoxM :public MSOpeningM
{
public:
	MSFWSwitchBoxM();
	~MSFWSwitchBoxM();

	enum eFWSwitchBoxType { General = 0, };
	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSFWSwitchBoxM)
	virtual void CopyFromMe(MSCompoM* pSource);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	static CString ToShapeName(eFWSwitchBoxType eType);
	virtual CString GetShapeName();

	void MakeProfile();
	bool CheckProfile();

	GM2DPolyline* GetProfile() { return m_pProfile; };

	eFWSwitchBoxType GetFWSwitchBoxType() { return m_eSwitchType; };
	virtual double GetWidth() override;
	virtual double GetHeight() override;
	virtual void SetWidth(double dWidth) override;
	virtual void SetHeight(double dHeight) override;

	virtual double GetW() { return m_W; };
	virtual double GetH() { return m_H; };
	virtual double GetL() { return m_L; };

	void SetFWSwitchBoxType(eFWSwitchBoxType type) { m_eSwitchType = type; };
	virtual void SetW(double param);
	virtual void SetH(double param);
	virtual void SetL(double param);

protected:
	void makeProfile_General();
	bool CheckProfile_General();

	GM2DPolyline* m_pProfile;
	eFWSwitchBoxType m_eSwitchType;
	double m_W, m_H, m_L;
};

