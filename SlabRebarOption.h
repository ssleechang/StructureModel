#pragma once
#include "afx.h"

class StructureModel_CLASS SlabRebarOption : public CObject
{
public:
	SlabRebarOption();
	~SlabRebarOption();

	void Init();
	int GetTopOrBottomRebar(GM2DLineVector* pLineVec, CString rebarText);
	bool IsRebarLayer(CString layerName);

	enum eTopBottomFilter { Line = 0, Text, BottomLayer};

	CString GetRebarLayer() {	return m_RebarLayer;	};
	CString GetInfoLayer() { return m_InfoLayer; };
	CString GetRangeLayer() { return m_RangerLayer; };
	int GetLeaderDist() { return m_LeaderDist; };

	void SetRebarLayer(CString layer) { m_RebarLayer  = layer; };
	void SetInfoLayer(CString layer) { m_InfoLayer = layer; };
	void SetRangeLayer(CString layer) { m_RangerLayer = layer; };
	void SetLeaderDist(int dist) { m_LeaderDist = dist; };

	eTopBottomFilter GetTopBottomFilter() { return m_eTopBotFilter; };
	CString GetTopLineType() { return m_TopLineType; };
	CString GetBotLineType() { return m_BotLineType; };
	CString GetTopBarTextInfo() { return m_TopBarTextInfo; };
	CString GetBotBarTextInfo() { return m_BotBarTextInfo; };
	CString GetBotRebarLayer() { return m_BotRebarLayer; };

	void SetTopBottomFilter(eTopBottomFilter filter) { m_eTopBotFilter = filter; };
	void SetTopLineType(CString lineType) { m_TopLineType = lineType; };
	void SetBotLineType(CString lineType) { m_BotLineType = lineType; };
	void SetTopBarTextInfo(CString textInfo) { m_TopBarTextInfo = textInfo; };
	void SetBotBarTextInfo(CString textInfo) { m_BotBarTextInfo = textInfo; };
	void SetBotRebarLayer(CString layer) { m_BotRebarLayer = layer; };

	int GetLeaderDistToText() { return m_LeaderDistToText; };
	bool GetIsUsedDefaultBar() { return m_IsUsedDefaultBar; };
	CString GetDefaultBar() { return m_DefaultBar; };

	void SetLeaderDistToText(int dist) { m_LeaderDistToText = dist; };
	void SetIsUsedDefaultBar(bool isUsed) { m_IsUsedDefaultBar = isUsed; };
	void SetDefaultBar(CString bar) { m_DefaultBar = bar; };

	bool GetIsUsedMatchingText() { return  m_IsUsedMatchingText; };
	map<CString, CString> GetMatchingText() { return mm_MatchingText; };
	void SetIsUsedMatchingText(bool isUsed) { m_IsUsedMatchingText = isUsed; };
	void SetMatchingText(CString key, CString value);

	bool GetMatchingText(CString key, CString& value);
	void ClearMatchingTexts() { mm_MatchingText.clear(); };

	void AddRebarLine(GM2DLineVector* pLineVec, CString layerName, CString lineType);
	void ClearTopBottomLines();
private:
	int GetTopOrBottomRebar(GM2DLineVector* pLineVec);
	int GetTopOrBottomRebar(CString rebarText);
	bool IsIncludeTextByTokenize(CString topOrbot, CString text, CString token);

private:
	// 라인필터
	CString m_RebarLayer;
	CString m_InfoLayer;
	CString m_RangerLayer;
	int m_LeaderDist;

	// 상/하부근 필터
	eTopBottomFilter m_eTopBotFilter;
	CString m_TopLineType;
	CString m_BotLineType;
	CString m_TopBarTextInfo;
	CString m_BotBarTextInfo;
	CString m_BotRebarLayer;

	// 텍스트 필터
	int m_LeaderDistToText;
	bool m_IsUsedDefaultBar;
	CString m_DefaultBar;
	bool m_IsUsedMatchingText;
	map<CString, CString> mm_MatchingText;

	vector<GM2DLineVector*> ma_TopLine, ma_BottomLine;
};

