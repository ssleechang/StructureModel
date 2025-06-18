#pragma once
class StructureModel_CLASS BWallDowelRebarTemplate
{
public:
	BWallDowelRebarTemplate();
	~BWallDowelRebarTemplate();

	void DataInit();
	void CopyFromMe(BWallDowelRebarTemplate* pRebarTemplate);
	void Serialize(CArchive& ar);

	long GetBarSpliceType();
	void SetBarSpliceType(long value);
	long GetTopLenType();
	void SetTopLenType(long value);
	long GetDowelShape();
	void SetDowelShape(long value);
	long GetConstructionHeight();
	void SetConstructionHeight(long value);
	BOOL GetAlterSpliceSet();
	void SetAlterSpliceSet(BOOL value);
	long GetAlterSpliceDist();
	void SetAlterSpliceDist(long value);
	long GetSupportBarDiaIndex();
	void SetSupportBarDiaIndex(long value);
	long GetSupportBarNum();
	void SetSupportBarNum(long value);
	long GetBarAnchorType();
	void SetBarAnchorType(long value);
	BOOL GetHookAnchorPlaceSpacingSet();
	void SetHookAnchorPlaceSpacingSet(BOOL value);
	long GetHookAnchorPlaceSpacing();
	void SetHookAnchorPlaceSpacing(long value);
	BOOL GetUserHookLenSet();
	void SetUserHookLenSet(BOOL value);
	BOOL GetSupportBarUserSteelGradeS();
	void SetSupportBarUserSteelGradeS(BOOL value);

	long GetSpacerDiaIndex();
	long GetSpacerSpacing();
	BOOL GetSpacerUserSteelGradeS();
	long GetSpacerHeightType();
	long GetSpacerHeightSpliceAddLength();
	long GetSpacerUserHeight();
	long GetSpacerLengthA();
	long GetSpacerLengthB();
	long GetSpacerWidthType();
	long GetSpacerUserWidth();
		
	void SetSpacerDiaIndex(long value);
	void SetSpacerSpacing(long value);
	void SetSpacerUserSteelGradeS(BOOL value);
	void SetSpacerHeightType(long value);
	void SetSpacerHeightSpliceAddLength(long value);
	void SetSpacerUserHeight(long value);
	void SetSpacerLengthA(long value);
	void SetSpacerLengthB(long value);
	void SetSpacerWidthType(long value);
	void SetSpacerUserWidth(long value);

	BOOL GetPlaceSpacer();
	void SetPlaceSpacer(BOOL value);

	long GetAlterSpliceType();
	void SetAlterSpliceType(long value);
private:
	long m_nBarSpliceType;
	long m_nTopLenType;
	long m_nDowelShape;
	long m_nConstructionHeight;
	BOOL m_bAlterSpliceSet;
	long m_nAlterSpliceType;
	long m_nAlterSpliceDist;
	long m_nSupportBarDiaIndex;
	BOOL m_bSupportBarUserSteelGradeS;
	long m_nSupportBarNum;
	long m_nBarAnchorType;
	BOOL m_bHookAnchorPlaceSpacingSet;
	long m_nHookAnchorPlaceSpacing;
	BOOL m_bUserHookLenSet;
		
	BOOL m_bPlaceSpacer;
	long m_nSpacerDiaIndex;
	long m_nSpacerSpacing;
	BOOL m_bSpacerUserSteelGradeS;	
	long m_nSpacerHeightType;
	long m_nSpacerHeightSpliceAddLength;
	long m_nSpacerUserHeight;
	long m_nSpacerLengthA;
	long m_nSpacerLengthB;
	long m_nSpacerWidthType;
	long m_nSpacerUserWidth;
};

