#pragma once
#include "MSRcObject.h"

class StructureModel_CLASS MSStirrupBar :
	public MSRcObject
{
public:
	MSStirrupBar(CString strBar = _T(""));
	MSStirrupBar(MSStirrupBar& TheStirrupBar)	{*this = TheStirrupBar;}
	MSStirrupBar& operator = (CString StirrupBar)
	{
		ma_strBar.RemoveAll();
		ma_strBar.Add(StirrupBar.MakeUpper());
		return *this;
	}
	MSStirrupBar& operator = (MSStirrupBar& StirrupBar)
	{
		ma_strBar.RemoveAll();
		INT_PTR NumStrBar = StirrupBar.ma_strBar.GetSize();
		for(long iStrBar=0; iStrBar<NumStrBar; iStrBar++)
		{
			ma_strBar.Add(StirrupBar.ma_strBar[iStrBar].MakeUpper());
		}

		return *this;
	}
	virtual ~MSStirrupBar();
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSStirrupBar)
	virtual void CopyFromMe(MSStirrupBar* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 
	
	// HD13@200/HD19@200...
	CStringArray ma_strBar;

	CString GetBarType(long LayerNum=0);
	double GetSpacing(long LayerNum=0);
	long GetBundleNum(long LayerNum=0);
	long GetBundleSpace(long LayerNum = 0);
	int SetSpace(long LayerNum, double NewSpacing); // New Dist로 맞춤..
	void SetBarDiaIdx(CString sDiaName, long LayerNum);
	CString GetStringOnALayer(long LayerNum=0);

	void SetStirrup(long LayerNum, long nBundleNum, CString Type, CString sDiaName, double Spacing, long nBundleSpace);
	void SetStirrup(long LayerNum, CString strStirrup);

	void SetBarType(long LayerNum, CString hd);
	void SetBarType(long LayerNum, char hd);
	long GetBarTypeIndex(long LayerNum);

	CString GetStirrupBarString();
	CString GetStirrupBarStringbyCulture();
	
	bool GetUserSteelGradeS();
protected:
	CString GetStirrupBarStringByCulture(long nBundleNum, CString strBarType, vector<CString> diaArr, double dBarSpacing, long nBundleSpace);
public:
	long GetNumLayers();
	CString GetFirstBarDiaName();
	CString GetLastBarDiaName();
	
	double GetMaxBarDia();
	double GetMinBarDia();
	CString GetMaxBarType();

	static bool IsCorrectRebarBySpace(CString strBar);
	long InitData(CString strBar);
	BOOL IsSameBar(MSStirrupBar& aBar);
	long Add(CString strBar);

	long InitData(const char* strBar);
	long Add(const char* strBar);

	void SetAlternate(BOOL TheAlternate){m_bAlternate = TheAlternate;}
	BOOL GetAlternate(){return m_bAlternate;}

	virtual vector<double> GetBarDiaAllLayer();

	double GetDeformedBarDiameter(long LayerNum = 0);
	CString GetBarDiameterName(long LayerName = 0);

	static CString getFirstDiaName2StrpBar(CString theBar);
	static CString getSecondDiaName2StrpBar(CString theBar);
	static vector<int> getDiaArr2StrpBar(CString theBar);
	void OnlyFirstDiaSet();

	vector<double> GetAllDiaArr();

	vector<CString> GetAllDiameterName();
private:
	BOOL m_bAlternate;
};

#pragma make_public(MSStirrupBar)