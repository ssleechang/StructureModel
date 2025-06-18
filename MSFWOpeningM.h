#pragma once
#include "MSOpeningM.h"
class StructureModel_CLASS MSFWOpeningM :	public MSOpeningM
{
public:
	MSFWOpeningM();
	~MSFWOpeningM();

	enum eFWOpeningType { RECT = 0, CIRCLE = 1 };


	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSFWOpeningM)
	virtual void CopyFromMe(MSCompoM* pSource);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	virtual double GetDiameter() { return GetHeight(); };

	static CString ToShapeName(eFWOpeningType eType);
	virtual CString GetShapeName();

	void MakeProfile();
	GM2DPolyline* GetProfile() { return m_pProfile; };

	eFWOpeningType GetFWOpeningType() { return m_eOpeningType; };
	void SetFWOpeningType(eFWOpeningType type) { m_eOpeningType = type; };
	virtual void SetWidth(double width)	override;
	virtual void SetHeight(double height) override;

protected:
	void makeProfile_Rect();
	void makeProfile_Circle();
protected:
	GM2DPolyline* m_pProfile;
	eFWOpeningType m_eOpeningType;
};

