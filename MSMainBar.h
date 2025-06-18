#pragma once
#include "MSRcObject.h"

class StructureModel_CLASS MSMainBar :
	public MSRcObject
{
public:
	MSMainBar(CString strBar = _T(""));
	MSMainBar(const MSMainBar& TheMainBar)	{*this = TheMainBar;}
	MSMainBar& operator = (CString TheStrMainBar)
	{
		ma_strBar.RemoveAll();
		ma_strBar.Add(TheStrMainBar.MakeUpper());
		return *this;
	}
	MSMainBar& operator = (const MSMainBar& TheMainBar)
	{
		ma_strBar.RemoveAll();
		INT_PTR NumStrBar = TheMainBar.ma_strBar.GetSize();
		for(long iStrBar=0; iStrBar<NumStrBar; iStrBar++)
		{
			CString aData = TheMainBar.ma_strBar[iStrBar];
			ma_strBar.Add(aData.MakeUpper());
		}

		return *this;
	}
	virtual ~MSMainBar();
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSMainBar)
	virtual void CopyFromMe(MSMainBar* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	// strBar = "1-3HD19+2-HD19+1-3HD19/2-HD19....
	CStringArray ma_strBar;

	long GetNumLayers();                       // 몇단 배근이냐..?
	CString GetBarType(long LayerNum=0);                     // HD or D
	long GetBundleNum(long LayerNum = 0);
	long GetBundleSpace(long LayerNum = 0);
	//long GetBarDiaIdxOnALayer(long LayerNum=0, CString strBar = _T(""));  // 한 Layer(단)이 갖고 있는 Dia(모두 동일)
	long GetTotalBars();                      // 전체 철근의 수..
	long GetNumBarsOnALayer(long LayerNum);   // 한 Layer(단)에 있는 철근의 수(Ea)
	CString GetNumBarOnAllLayers();

	void SetALayer(long LayerNum, long NumBars, long nBundleNum, long nBundleSpace, CString NewType=_T(""), CString NewDiaName = _T(""));
	void SetALayer(long LayerNum, CString strLayer);
	void SetNumBars(long LayerNum, long NumBars);
	void SetType(CString Type);
	CString GetstrLayerOnALayer(long LayerNum); // 한 Layer의 전체 String을 return;

	void DeleteALayer(long LayerNum);

	CString GetTotalMainBarString();
	CString GetToltalMainBarStringByCulture();
	CString GetMainBarStringExceptNumByCulture();

	bool GetUserSteelGradeS();
protected:
	CString GetToltalMainBarStringByCulture(long nBundleNum, CString strBarType, long nBarNum, CString sBarDia, long nBundleSpace);

public:
	long InitData(CString strBar);
	BOOL IsSameBar(MSMainBar& aBar);
	long Add(CString strBar);
	void RemoveAll();


	virtual vector<double> GetBarDiaAllLayer();
	bool IsSameBarDiameter();

//	double GetBarDiameter(long LayerNum = 0);

	double GetDeformedBarDiameter(long LayerNum = 0);
	CString GetBarDiameterName(long LayerName = 0);

};
