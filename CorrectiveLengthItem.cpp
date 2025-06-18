#include "StdAfx.h"
#include "CorrectiveLengthItem.h"
#include "GMLib/MSVersionInfo.h"
#include "MSObject.h"

using namespace RebarOption;
CorrectiveLengthItem::CorrectiveLengthItem(RebarOption::CorLen_Type type, bool IsCorrective, CString text, bool Ishalf50, int spliceAddLen)
{
	m_eType = type;
	m_bCorrective = IsCorrective;
	m_sText = text;
	m_bHalf50 = Ishalf50;
	m_nUnit = GetUnitbyText();
	m_eRoundMethod = UP;
	m_nSpliceAddLen = spliceAddLen;
}


CorrectiveLengthItem::~CorrectiveLengthItem(void)
{
}

void CorrectiveLengthItem::Serialize( CArchive& ar )
{

	if(ar.IsStoring())
	{
		ar << (int)m_eType;
		ar << m_nUnit;
		ar << (int)m_eRoundMethod;
		ar << m_sText;

		ar << m_bCorrective;
		ar << m_bHalf50;
		ar << m_nSpliceAddLen;
	}
	else
	{
		int tmpEnum;

		ar >> tmpEnum;
		m_eType = (CorLen_Type)tmpEnum;

		ar >> m_nUnit;

		ar >> tmpEnum;
		m_eRoundMethod = (CorLen_ROUND) tmpEnum;
		//보정 방법은 무조건 UP으로 변경 by swlee
		if(m_eRoundMethod != UP)
			m_eRoundMethod = UP;

		ar >> m_sText;

		if(MSVersionInfo::GetCurrentVersion() >= 20150102)
		{
			ar >> m_bCorrective;
			ar >> m_bHalf50;
		}

		m_nUnit = GetUnitbyText();
		m_eRoundMethod = UP;

		if (MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20191219)
		{
			ar >> m_nSpliceAddLen;
		}
	}
}

int CorrectiveLengthItem::GetUnitbyText()
{
	int nUnit = 100;

	if(m_sText == _T("10") || m_sText == _T("50"))
	{
		nUnit = 10;
	}

	return nUnit;
}

CString CorrectiveLengthItem::GetTextLine() const
{
	CString sTextLine;

	if(m_sText == _T("10") || m_sText == _T("100"))
	{
		sTextLine = _T("0");
	}
	else if(m_sText == _T("50") || m_sText == _T("500"))
	{
		sTextLine = _T("5");
	}
	else
		sTextLine = m_sText;

	return sTextLine;
}

int CorrectiveLengthItem::ParsingCorLenData(vector<long>& ValueArr) const
{
	ValueArr.clear();
	CString TextLine = GetTextLine();
	long nVal = 0;

	TextLine.TrimLeft();
	TextLine.TrimRight();
	if(TextLine==_T("")) return 0;

	long nPos = TextLine.Find(_T(","));
	if(nPos<0)
	{
		nVal = _ttol(TextLine);
		if(nVal > 0)//0은 뺀다
		{
			ValueArr.push_back(_ttol(TextLine));
			ValueArr.push_back(10);
		}
		return ValueArr.size();
	}
	CString strValidText = _T("");
	CString strValue = _T("");
	strValidText = TextLine;
	while(TRUE)
	{
		TextLine = strValidText;
		nPos = TextLine.Find(_T(","));

		if(nPos >= 0)
		{
			strValue = TextLine.Mid(0, nPos);
			strValidText = TextLine.Mid(nPos+1);
			nVal = _ttol(strValue);
			if(nVal < 1)//0은 10으로 입력
				ValueArr.push_back(10);
			else
				ValueArr.push_back(nVal);

		}
		else
		{
			strValue = TextLine;
			nVal = _ttol(strValue);
			if(nVal != 0)
				ValueArr.push_back(_ttol(strValue));
			else
				ValueArr.push_back(10);

			break;
		}
	}

	return ValueArr.size();
}

long CorrectiveLengthItem::Get50UpDownLength(long nLength) const
{
	long nRemainder = nLength%50;
	long nPortion = nLength/50;
	if(nRemainder >= 25)
		nPortion++;
	return nPortion*50;
}
long CorrectiveLengthItem::Get50UpLength(long nLength) const
{
	long nRemainder = nLength%50;
	long nPortion = nLength/50;
	if(nRemainder > 0)
		nPortion++;
	return nPortion*50;
}

long CorrectiveLengthItem::GetSpliceAddLen()
{
	return m_nSpliceAddLen;
}

void CorrectiveLengthItem::Copy(CorrectiveLengthItem desCorrectiveLengthItem)
{
	m_eType = desCorrectiveLengthItem.m_eType;
	m_nUnit = desCorrectiveLengthItem.m_nUnit;
	m_eRoundMethod = desCorrectiveLengthItem.m_eRoundMethod;
	m_sText = desCorrectiveLengthItem.m_sText;
	m_bCorrective = desCorrectiveLengthItem.m_bCorrective;
	m_bHalf50 = desCorrectiveLengthItem.m_bHalf50;
	m_nSpliceAddLen = desCorrectiveLengthItem.m_nSpliceAddLen;
}