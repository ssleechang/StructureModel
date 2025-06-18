#include "stdafx.h"
#include "SlabRebarOption.h"


SlabRebarOption::SlabRebarOption()
{
	Init();
}


SlabRebarOption::~SlabRebarOption()
{
}

void SlabRebarOption::Init()
{
	ClearMatchingTexts();
	ClearTopBottomLines();
}
void SlabRebarOption::SetMatchingText(CString key, CString value)
{
	auto it = mm_MatchingText.find(key);
	if (it == mm_MatchingText.end())
		mm_MatchingText.insert(make_pair(key, value));
	else
		it->second = value;
}

bool SlabRebarOption::GetMatchingText(CString key, CString& value)
{
	value = "";
	auto it = mm_MatchingText.find(key);
	if (it != mm_MatchingText.end())
	{
		value = it->second;
		return true;
	}
	return false;
}

void SlabRebarOption::AddRebarLine(GM2DLineVector* pLineVec, CString layerName, CString lineType)
{
	if (m_eTopBotFilter == eTopBottomFilter::Line)
	{
		if (lineType.MakeUpper() == m_TopLineType.MakeUpper())
			ma_TopLine.push_back(pLineVec);
		else if (lineType.MakeUpper() == m_BotLineType.MakeUpper())
			ma_BottomLine.push_back(pLineVec);
	}
	else if (m_eTopBotFilter == eTopBottomFilter::BottomLayer)
	{
		if(layerName.MakeUpper() == m_BotRebarLayer.MakeUpper())
			ma_BottomLine.push_back(pLineVec);
		else
			ma_TopLine.push_back(pLineVec);
	}
}
void SlabRebarOption::ClearTopBottomLines()
{
	ma_TopLine.clear();
	ma_BottomLine.clear();
}

int SlabRebarOption::GetTopOrBottomRebar(GM2DLineVector* pLineVec, CString rebarText)
{
	switch (m_eTopBotFilter)
	{
	case SlabRebarOption::Line:
	case SlabRebarOption::BottomLayer:
		return GetTopOrBottomRebar(pLineVec);
	case SlabRebarOption::Text:
		return GetTopOrBottomRebar(rebarText);
	}
	return -1;
}

int SlabRebarOption::GetTopOrBottomRebar(GM2DLineVector* pLineVec)
{
	if (m_eTopBotFilter == SlabRebarOption::Line || m_eTopBotFilter == SlabRebarOption::BottomLayer)
	{
		for (auto it = ma_TopLine.begin(); it != ma_TopLine.end(); it++)
			if (*it == pLineVec)		return 1;
		for (auto it = ma_BottomLine.begin(); it != ma_BottomLine.end(); it++)
			if (*it == pLineVec)	return 0;
	}
	return -1;
}

int SlabRebarOption::GetTopOrBottomRebar(CString rebarText)
{
	if (m_eTopBotFilter == SlabRebarOption::Text)
	{
		CString token = _T(";");
		if (IsIncludeTextByTokenize(rebarText, m_TopBarTextInfo, token))
			return 1;
		if (IsIncludeTextByTokenize(rebarText, m_BotBarTextInfo, token))
			return 0;
		return -1;
	}
	return -1;
}

bool SlabRebarOption::IsIncludeTextByTokenize(CString topOrbot, CString text, CString token)
{
	int nTokenPos = 0;
	CString strToken = text.Tokenize(token, nTokenPos);
	if (topOrbot.Find(strToken) != -1)	return true;

	while (!strToken.IsEmpty())
	{
		strToken = text.Tokenize(token, nTokenPos);
		if (!strToken.IsEmpty())
			if (topOrbot.Find(strToken) != -1)	return true;
	}
	return false;
}

bool SlabRebarOption::IsRebarLayer(CString layerName)
{
	if (layerName.MakeUpper() == m_RebarLayer.MakeUpper())	return true;
	if (layerName.MakeUpper() == m_BotRebarLayer.MakeUpper())	return true;
	return false;
}
