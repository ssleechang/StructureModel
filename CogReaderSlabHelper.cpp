#include "stdafx.h"
#include "CogReaderSlabHelper.h"
#include "MTReader.h"
/////////////////////////////////// CogReaderTextArray //////////////////////////////

void CogReaderTextArray::AddText(CString text, eTextType textType)
{
	switch (textType)
	{
	case eTextType::Except: ma_ExceptText.Add(text);	break;
	case eTextType::Same:	ma_SameText.Add(text);	break;
	case eTextType::Include:	ma_IncludeText.Add(text);	break;
	}
}

CogReaderTextArray::eTextType CogReaderTextArray::GetTextType(CString text)
{
	bool isInclude = true;
	bool isSame = false;
	if (IsRecognizeTextType(ma_ExceptText, text, isInclude))
		return CogReaderTextArray::Except;
	if (IsRecognizeTextType(ma_SameText, text, isSame))
		return CogReaderTextArray::Same;
	if (IsRecognizeTextType(ma_IncludeText, text, isInclude))
		return CogReaderTextArray::Include;
	return CogReaderTextArray::None;
}

bool CogReaderTextArray::IsRecognizeTextType(CStringArray& stringArr, CString text, bool IsIncluded)
{
	bool isRecognizeType = false;
	CString compareText = text;
	if (!IsIncluded)
	{
		int numberIndex = MTReader::FindFirstNumberIndex(text);
		if (numberIndex != -1)
		{
			if (numberIndex > 0)
			{
				if (text[numberIndex - 1] == '-')
					numberIndex = numberIndex - 1;
			}
			compareText = text.Mid(0, numberIndex);
		}

	}
	for (int i = 0; i < stringArr.GetSize(); i++)
	{
		CString optionText = stringArr[i];
		if (IsIncluded)
		{
			if (compareText.Find(stringArr[i]) != -1)
				return true;
		}
		else
		{
			if (compareText.Compare(stringArr[i]) == 0)
				return true;
		}
	}
	return isRecognizeType;
}

bool CogReaderTextArray::AnalyzeText(CString text, double& outData)
{
	eTextType type = GetTextType(text);
	if (type == CogReaderTextArray::None || type == CogReaderTextArray::Except)
		return false;

	long parsingData = MTReader::FindNumberData(0, text);
	int firstNumberIndex = MTReader::FindFirstNumberIndex(text);
	if (firstNumberIndex > 0)
	{
		if (text[firstNumberIndex - 1] == '-')
			parsingData *= -1;
	}
	CString temp;
	temp.Format(_T("%ld"), parsingData);
	if (temp != "0" && parsingData == 0)	return false;

	outData = (double)parsingData;
	return true;
}

/////////////////////////////////// CogReaderSlabHelper //////////////////////////////
CogReaderSlabHelper::CogReaderSlabHelper()
{
	Initialize();
}


CogReaderSlabHelper::~CogReaderSlabHelper()
{
}

void CogReaderSlabHelper::AddSlabThickOptionText(CString text, CogReaderTextArray::eTextType textType)
{
	m_ThickOption.AddText(text.MakeUpper(), textType);
}
void CogReaderSlabHelper::AddSlabOffsetOptionText(CString text, CogReaderTextArray::eTextType textType)
{
	m_OffsetOption.AddText(text.MakeUpper(), textType);
}

bool CogReaderSlabHelper::AnalyzeText(CString text, double& outData, bool IsThick)
{
	if (IsThick)
	{
		double minThick = 50.0;
		bool rtnVal = m_ThickOption.AnalyzeText(text, outData);
		if (rtnVal == true && outData < minThick)
		{
			outData = 0.0;	
			return false;
		}
		return rtnVal;
	}
	return m_OffsetOption.AnalyzeText(text, outData);
}
