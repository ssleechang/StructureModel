#pragma once
#include "afx.h"


class CogReaderTextArray : public CObject
{
public:
	enum eTextType { None = 0, Except, Same, Include };
	CogReaderTextArray()
	{
		Initialize();
	}
	CogReaderTextArray(CogReaderTextArray& aData) { *this = aData; }
	CogReaderTextArray& operator = (CogReaderTextArray& aData)
	{
		Initialize();

		ma_ExceptText.Copy(aData.ma_ExceptText);
		ma_SameText.Copy(aData.ma_SameText);
		ma_IncludeText.Copy(aData.ma_IncludeText);

		return *this;
	}

	void Initialize()
	{
		ma_ExceptText.RemoveAll();
		ma_SameText.RemoveAll();
		ma_IncludeText.RemoveAll();
	}
	void AddText(CString text, eTextType textType);
	eTextType GetTextType(CString text);
	bool AnalyzeText(CString text, double& outData);
private:
	bool IsRecognizeTextType(CStringArray& stringArr, CString text, bool IsIncluded);

	CStringArray ma_SameText;
	CStringArray ma_IncludeText;
	CStringArray ma_ExceptText;
};


class StructureModel_CLASS CogReaderSlabHelper :public CObject
{
public:
	CogReaderSlabHelper();
	~CogReaderSlabHelper();

	CogReaderSlabHelper(CogReaderSlabHelper& aData) { *this = aData; }
	CogReaderSlabHelper& operator = (CogReaderSlabHelper& aData)
	{
		Initialize();

		m_DefaultSlabThick = aData.m_DefaultSlabThick;
		m_DefaultSlabOffset = aData.m_DefaultSlabOffset;
		m_IsUsedThickOption = aData.m_IsUsedThickOption;
		m_IsUsedOffsetOption = aData.m_IsUsedOffsetOption;

		m_ThickOption = aData.m_ThickOption;
		m_OffsetOption = aData.m_OffsetOption;

		return *this;
	}

	void Initialize()
	{
		m_DefaultSlabThick = 200;
		m_DefaultSlabOffset = 0;
		m_IsUsedThickOption = false;
		m_IsUsedOffsetOption = false;
		m_ThickOption.Initialize();
		m_OffsetOption.Initialize();
	}
	void AddSlabThickOptionText(CString text, CogReaderTextArray::eTextType textType);
	void AddSlabOffsetOptionText(CString text, CogReaderTextArray::eTextType textType);

	double GetDefaultSlabThick() { return m_DefaultSlabThick; };
	double GetDefaultSlabOffset() { return m_DefaultSlabOffset; };
	bool GetIsUsedThickOption() { return m_IsUsedThickOption; };
	bool GetIsUsedOffsetOption() { return m_IsUsedOffsetOption; };

	void SetDefaultSlabThick(double thick) { m_DefaultSlabThick = thick; };
	void SetDefaultSlabOffset(double offset) { m_DefaultSlabOffset = offset; };
	void SetIsUsedThickOption(bool isUsed) { m_IsUsedThickOption = isUsed; };
	void SetIsUsedOffsetOption(bool isUsed) { m_IsUsedOffsetOption = isUsed; };

	bool AnalyzeText(CString text, double& outData, bool IsThick);

private:

	double m_DefaultSlabThick;
	double m_DefaultSlabOffset;

	bool m_IsUsedThickOption;
	bool m_IsUsedOffsetOption;

	CogReaderTextArray m_ThickOption;
	CogReaderTextArray m_OffsetOption;
};

