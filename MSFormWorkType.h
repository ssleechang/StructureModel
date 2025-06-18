#pragma once
#include "MSObject.h"
#include "MSFormWorkDisplay.h"

class StructureModel_CLASS MSFormWorkType : public MSObject
{
public:
	MSFormWorkType();
	~MSFormWorkType();
	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSFormWorkType)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);

	CString GetName();
	void SetName(CString name);
	
	void GetColor(int& alpha, int& red, int& green, int& blue);
	void SetColor(int alpha, int red, int green, int blue);
	void SetColor(int alpha, COLORREF color);
	
	bool GetReadOnly();
	void SetReadOnly(bool bReadOnly);

	bool GetShow();
	void SetShow(bool bShow);

    CString GetChangedName();
    void SetChangedName(CString changedName);

    double GetConcreteThick();
    void SetConcreteThick(double concreteThick);

    double GetFormworkRatio();
    void SetFormworkRatio(double formworkRate);

	CString GetSuffix();
	void SetSuffix(CString suffix);

private :
	CString m_Name;
	COLORREF m_Color;
	long m_Alpha;
	bool m_bReadOnly;
	bool m_bShow;
    CString m_ChangedName;
    double m_ConcreteThick;
    double m_FormworkRatio;
	CString m_Suffix;
};
