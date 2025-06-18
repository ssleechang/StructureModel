#pragma once

class StructureModel_CLASS T_BarLoc
{
public:
	T_BarLoc(GM2DVector LocVec, double dBarDeg, double dDia, bool bUserSteelGradeS);
	T_BarLoc* Copy();


	~T_BarLoc();

	bool CheckGrouping(T_BarLoc* pVerBar);
	bool CheckDowelGrouping(T_BarLoc* pVerBar);
	bool CheckLinkDowelGrouping(T_BarLoc* pVerBar);
	bool CheckTopDownMainBarGrouping(T_BarLoc *pVerBar);
	T_BarLoc& operator = (T_BarLoc& aData)
	{
		m_dBarDeg = aData.m_dBarDeg;
		m_nTopDevType = aData.m_nTopDevType;
		m_Loc = aData.m_Loc;
		m_IsCornerBar = aData.m_IsCornerBar;
		m_dDia = aData.m_dDia;
		m_BarName = aData.m_BarName;
		m_isValue1 = aData.m_isValue1;
		m_needDowel = aData.m_needDowel;
		m_nBotDevType = aData.m_nBotDevType;
		m_nBotSpliceType = aData.m_nBotSpliceType;
		m_nTopSpliceType = aData.m_nTopSpliceType;
		m_nRootType = aData.m_nRootType;
		m_bUserSteelGradeS = aData.GetUserSteelGradeS();
		return *this;
	};
	double GetDia();
	void SetDia(double value);
	double GetBarDeg();
	void SetBarDeg(double value);
	long GetTopDevType();
	void SetTopDevType(long value);
	GM2DVector GetLoc();
	void SetLoc(GM2DVector value);
	GM2DVector GetBottomLoc();
	void SetBottomLoc(GM2DVector value);
	BOOL GetIsCornerBar();
	void SetIsCornerBar(BOOL value);
	CString GetBarName();
	void SetBarName(CString value);
	BOOL GetIsValue1();
	void SetIsValue1(BOOL value);
	BOOL GetNeedDowel();
	void SetNeedDowel(BOOL value);
	long GetBotDevType();
	void SetBotDevType(long value);
	long GetBotSpliceType();
	void SetBotSpliceType(long value);
	long GetTopSpliceType();
	void SetTopSpliceType(long value);
	long GetRootType();
	void SetRootType(long newVal);
	long GetTopLength();
	void SetTopLength(long newVal);
	long GetBotLength();
	void SetBotLength(long newVal);
	long GetTopAlterDist();
	void SetTopAlterDist(long newVal);
	long GetBotAlterDist();
	void SetBotAlterDist(long newVal);
	bool GetUserSteelGradeS();
	void SetUserSteelGradeS(bool value);
private:
	double m_dDia;
	double m_dBarDeg;
	long m_nTopDevType;//0:후크정착 1:일자정착 2:일자이음 3:기계식이음
	GM2DVector m_Loc;
	GM2DVector m_BottomLoc;
	BOOL m_IsCornerBar;
	CString m_BarName;
	BOOL m_isValue1;//엇배근시 H1(Value1)값을 쓸지 여부
	BOOL m_needDowel;
	long m_nBotDevType;//0:후크정착 1:일자정착 2:일자이음
	long m_nTopSpliceType, m_nBotSpliceType;//0:겹이음 1:커플러이음 2: 압접이음
	long m_nRootType;
	long m_nTopLength;
	long m_nBotLength;
	long m_nTopAlterDist;
	long m_nBotAlterDist;
	bool m_bUserSteelGradeS;
};
