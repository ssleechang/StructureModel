#pragma once
#include "MSCompoM.h"
class StructureModel_CLASS MSFWShadeM :public MSCompoM
{
public:
	MSFWShadeM();
	~MSFWShadeM();

	enum eFWShadeType { Horizontal_Type1 = 0, Horizontal_Type2, 
		                                   Vertical_Type1 = 10, Vertical_Type2, Vertical_Type3};
	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSFWShadeM)
	virtual void CopyFromMe(MSCompoM* pSource);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	static CString ToShapeName(eFWShadeType eType);
	virtual CString GetShapeName();

	void MakeProfile();
	bool CheckProfile();

	GM2DPolyline* GetProfile() { return m_pProfile; };

	eFWShadeType GetFWShadeType() { return m_eShadeType; };
	bool IsVerticalShade();

	virtual double GetWidth();
	virtual double GetHeight();
	virtual double GetDepth();
	virtual void SetWidth(double dWidth);
	virtual void SetHeight(double dHeight);

	virtual double GetW() { return m_W; };
	virtual double GetH() { return m_H; };
	virtual double GetW1() { return m_W1; };
	virtual double GetH1() { return m_H1; };

	void SetFWShadeType(eFWShadeType type) { m_eShadeType = type; };
	virtual void SetW(double param);
	virtual void SetH(double param);
	virtual void SetW1(double param);
	virtual void SetH1(double param);

protected:
	void makeProfile_HorizontalType1();
	void makeProfile_HorizontalType2();
	void makeProfile_VerticalType1();
	void makeProfile_VerticalType2();
	void makeProfile_VerticalType3();

	bool CheckProfile_HorizontalType1();
	bool CheckProfile_HorizontalType2();
	bool CheckProfile_VerticalType1();
	bool CheckProfile_VerticalType2();
	bool CheckProfile_VerticalType3();
protected:
	GM2DPolyline* m_pProfile;
	eFWShadeType m_eShadeType;
	double m_W, m_H, m_W1, m_H1;
};

