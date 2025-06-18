#include "StdAfx.h"
#include "MSMainBar.h"

#include "MSDeformedBar.h"
#include <math.h>

IMPLEMENT_SERIAL(MSMainBar, MSRcObject, VERSIONABLE_SCHEMA | 1)
MSMainBar::MSMainBar(CString strBar)
{
	if(strBar != _T(""))
		ma_strBar.Add(strBar.MakeUpper());
}

MSMainBar::~MSMainBar(void)
{
}
void MSMainBar::CopyFromMe(MSMainBar* pSource)
{
	if(pSource==NULL)	return; 

	ma_strBar.RemoveAll();
	long NumStrBar = pSource->ma_strBar.GetSize();
	for(long iStrBar=0; iStrBar<NumStrBar; iStrBar++)
	{
		ma_strBar.Add(pSource->ma_strBar[iStrBar]);
	}
}

void MSMainBar::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	int nSize = ma_strBar.GetSize();
	buffer << nSize;
	for( int i = 0; i < nSize; i++)
		buffer << ma_strBar[i];
}

void MSMainBar::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	int nSize = 0;
	buffer >> nSize;
	ma_strBar.RemoveAll();
	CString strData;
	for( int i = 0; i < nSize; i++)
	{
		buffer >> strData;
		ma_strBar.Add(strData);
	}
}

void MSMainBar::Serialize(CArchive &ar)
{
	MSRcObject::Serialize(ar);
	CString strBar;
	if(ar.IsStoring())
	{
		long NumStrBar = ma_strBar.GetSize();
		ar << NumStrBar;
		for(long iStrBar=0; iStrBar<NumStrBar; iStrBar++)
		{
			strBar = ma_strBar[iStrBar];
			ar << strBar;
		}
	}
	else
	{
		long NumStrBar = 0;
		ma_strBar.RemoveAll();
		ar >> NumStrBar;
		for(long iStrBar=0; iStrBar<NumStrBar; iStrBar++)
		{
			ar >> strBar;
			ma_strBar.Add(strBar);
		}
	}
}

long MSMainBar::InitData(CString strBar)
{
	ma_strBar.RemoveAll();
	return ma_strBar.Add(strBar.MakeUpper());
}

long MSMainBar::Add(CString strBar)
{
	return ma_strBar.Add(strBar.MakeUpper());
}
void MSMainBar::RemoveAll()
{
	ma_strBar.RemoveAll();
}
void MSMainBar::SetALayer(long LayerNum, long NumBars, long nBundleNum, long nBundleSpace, CString _NewType, CString NewDiaName)
{
	CString NewType(_NewType);
	CString Type;
	if(NewType == _T("")) Type = GetBarType();
	else              Type = NewType;

	if(NewDiaName == _T("")) NewDiaName = GetBarDiameterName();

	Type = Type.MakeUpper();
	CString strData;
	if(nBundleNum == 0)
		strData.Format(_T("%ld-%s%s"), NumBars, Type, NewDiaName);
	else
		strData.Format(_T("%ld-%ld%s%s"), NumBars, nBundleNum, Type, NewDiaName);

	if (nBundleSpace != 0)
		strData.Format(_T("%s[B@=%ld]"), strData, nBundleSpace);

	ma_strBar.SetAtGrow(LayerNum, strData);
}

void MSMainBar::SetNumBars(long LayerNum, long NumBars)
{
	SetALayer(LayerNum, NumBars, GetBundleNum(), GetBundleSpace(), GetBarType(), GetBarDiameterName(LayerNum));
}
void MSMainBar::SetALayer(long LayerNum, CString strLayer)
{
	ma_strBar.SetAtGrow(LayerNum, strLayer.MakeUpper());
}

void MSMainBar::SetType(CString Type)
{
	CString strData, strNewData;

	CString sDia = GetBarDiameterName();
	long NumLayer = GetNumLayers();
	long nBundleNum = GetBundleNum();
	long nBundleSpace = GetBundleSpace();

	for(long iLayer=0; iLayer<NumLayer; iLayer++)
	{
		long NumBars = GetNumBarsOnALayer(iLayer);
		if(nBundleNum == 0)
			strData.Format(_T("%ld-%s%s"), NumBars, Type.MakeUpper(), sDia);
		else
			strData.Format(_T("%ld-%ld%s%s"), NumBars, nBundleNum, Type.MakeUpper(), sDia);

		if (nBundleSpace != 0)
			strData.Format(_T("%s[B@=%ld]"), strData, nBundleSpace);

		SetALayer(iLayer, strData);
	}
}

long MSMainBar::GetNumLayers()
{
	return ma_strBar.GetSize();
}

CString MSMainBar::GetBarType(long LayerNum)
{
	CString strBar = _T("NULL");
	long NumLayer = GetNumLayers();
	if(LayerNum < 0 || LayerNum >= NumLayer)
		return strBar;

	strBar = ma_strBar[LayerNum];
	CString Compare;
	BOOL bFindData = FALSE;
	int nLength = strBar.GetLength();
	int iLength;
	for(iLength=0; iLength<nLength; iLength++)
	{
		Compare = strBar.GetAt(iLength);
		if(Compare == 'D')
		{
			bFindData = TRUE;
			break;
		}
	}
	if(bFindData)
	{
		if(strBar.Mid(iLength-1, 1) == _T("H")) return _T("HD");
		else																return _T("D");
	}
	return _T("NULL");
}

long MSMainBar::GetTotalBars()
{
	long TotalBars=0;

	long NumLayer = GetNumLayers();
	for(long iLayer=0; iLayer<NumLayer; iLayer++)
	{
		TotalBars += GetNumBarsOnALayer(iLayer);
	}
	return TotalBars;
}

long MSMainBar::GetNumBarsOnALayer(long LayerNum)
{


	long NumLayer = GetNumLayers();
	if(LayerNum < 0 || LayerNum >= NumLayer)
		return 0;	
	
	long NumBar = 0;
	CString strBar = ma_strBar[LayerNum];
	NumBar = MSDeformedBar::GetNumBars(strBar);

	return NumBar;



	//CString strBar, Compare;
	//strBar = ma_strBar[LayerNum];
	//int Count = strBar.GetLength();
	//for(int i=0; i<Count; i++)
	//{
	//	Compare = strBar.GetAt(i);
	//	if(Compare == '-') break;
	//}

	//long NumBar = _ttoi(strBar.Left(i));
	//return NumBar;
}

///////////////////////////////////////////////////////////////////////////
/////////////////// 부속 함수들.. /////////////////////////////////////////
// SetALayer(long LayerNum, CString strLayer)를 하기 위해 한 Layer의 전체 String
// 을 읽어 오는 함수..
CString MSMainBar::GetstrLayerOnALayer(long LayerNum)
{
	long NumLayer = GetNumLayers();
	if(LayerNum < 0 || LayerNum >= NumLayer)
		return _T("NULL");
	CString strBar;
	strBar = ma_strBar[LayerNum];
	return strBar;
}

void MSMainBar::DeleteALayer(long LayerNum)
{
	long NumLayer = GetNumLayers();
	if(LayerNum < 0 || LayerNum >= NumLayer)
		return;

	ma_strBar.RemoveAt(LayerNum);
}

CString MSMainBar::GetTotalMainBarString()
{
	long nBarNum = GetTotalBars();
	long nBundleNum = GetBundleNum();
	long nBundleSpace = GetBundleSpace();
	CString strBarType = GetBarType();
	CString sBarDia = GetBarDiameterName();

	CString strBar;
	if(nBundleNum == 0)
		strBar.Format(_T("%ld-%s%s"), nBarNum, MSDeformedBar::m_sDefaultBarType, sBarDia);
	else
		strBar.Format(_T("%ld-%ld%s%s"), nBarNum, nBundleNum, MSDeformedBar::m_sDefaultBarType, sBarDia);

	if (nBundleSpace != 0)
		strBar.Format(_T("%s[B@=%ld]"), strBar, nBundleSpace);

	return strBar;
}

CString MSMainBar::GetToltalMainBarStringByCulture()
{
	long nBarNum = GetTotalBars();
	long nBundleNum = GetBundleNum();
	long nBundleSpace = GetBundleSpace();
	CString strBarType = MSDeformedBar::GetDefaultRebarNamePrefix(1);
	CString sBarDia = GetBarDiameterName();

	return GetToltalMainBarStringByCulture(nBundleNum, strBarType, nBarNum, sBarDia, nBundleSpace);
}

CString MSMainBar::GetMainBarStringExceptNumByCulture()
{
	CString strBar = _T("");

	long nBundleNum = GetBundleNum();
	long nBundleSpace = GetBundleSpace();
	CString strBarType = MSDeformedBar::GetDefaultRebarNamePrefix(1);
	CString sBarDia = GetBarDiameterName();

	if (nBundleNum == 0)
		strBar.Format(_T("%s%s"), strBarType, sBarDia);
	else
		strBar.Format(_T("%ld%s%s"), nBundleNum, strBarType, sBarDia);

	if (nBundleSpace != 0)
		strBar.Format(_T("%s[B@=%ld]"), strBar, nBundleSpace);

	return strBar;
}

CString MSMainBar::GetToltalMainBarStringByCulture(long nBundleNum, CString strBarType, long nBarNum, CString sBarDia, long nBundleSpace)
{
	CString strBar = _T("");

	if(nBundleNum == 0)
		strBar.Format(_T("%ld-%s%s"), nBarNum, strBarType, sBarDia);
	else
		strBar.Format(_T("%ld-%ld%s%s"), nBarNum, nBundleNum, strBarType, sBarDia);

	if (nBundleSpace != 0)
		strBar.Format(_T("%s[B@=%ld]"), strBar, nBundleSpace);

	return strBar;
}

// 2/3 or 2/4 
CString MSMainBar::GetNumBarOnAllLayers()
{
	CString strData = _T(""), aData = _T("");
	long NumLayer = GetNumLayers();
	for(long iLayer=0; iLayer<NumLayer; iLayer++)
	{
		if(iLayer == 0)
			aData.Format(_T("%ld"), GetNumBarsOnALayer(iLayer));
		else
			aData.Format(_T("%ld/"), GetNumBarsOnALayer(iLayer));
		strData += aData;
	}
	return strData;
}

BOOL MSMainBar::IsSameBar(MSMainBar& aBar)
{
	long NumMyLayer = GetNumLayers();
	long NumUrLayer = aBar.GetNumLayers();
	if(NumMyLayer != NumUrLayer)
		return FALSE;
	if(this->GetUserSteelGradeS() != aBar.GetUserSteelGradeS())
		return FALSE;
	for(long iLayer = 0 ; iLayer < NumMyLayer ; iLayer++)
	{
		if(GetNumBarsOnALayer(iLayer) != aBar.GetNumBarsOnALayer(iLayer)
			|| fabs(GetDeformedBarDiameter(iLayer) - aBar.GetDeformedBarDiameter(iLayer)) > DTOL5 )
			return FALSE;
	}

	return TRUE;
}

vector<double> MSMainBar::GetBarDiaAllLayer()
{
	vector<double> DiaArr;
	long NumLayer = GetNumLayers();
	for(long iLayer = 0 ; iLayer < NumLayer ; iLayer++)
	{
		double dDia = GetDeformedBarDiameter(iLayer);

		bool bAdd = true;
		long NumDia = DiaArr.size();
		for(long i = 0 ; i < NumDia ; i++)
		{
			if(fabs(DiaArr[i]-dDia)<DTOL_GM)
			{
				bAdd = false;
				break;
			}
		}
		if(bAdd == true)
			DiaArr.push_back(dDia);
	}
	return DiaArr;
}

double MSMainBar::GetDeformedBarDiameter(long LayerNum)
{
	double dDia = 0;
	if(ma_strBar.GetSize() < 0)	return dDia;
	if(ma_strBar.GetSize()-1 >= LayerNum)
		dDia = MSDeformedBar::GetDeformedBarDiameter(ma_strBar[LayerNum]);
	return dDia;
}

CString MSMainBar::GetBarDiameterName(long LayerNum)
{
	CString sDiaName = _T("");
	if(ma_strBar.GetSize() < 0)	return sDiaName;
	if(ma_strBar.GetSize()-1 >= LayerNum)
		sDiaName = MSDeformedBar::GetBarDiameterName(ma_strBar[LayerNum]);
	return sDiaName;
}

bool MSMainBar::IsSameBarDiameter()
{
	int NumLayer = ma_strBar.GetSize();
	if(NumLayer == 1)	return true;

	CString sFirstDiaName = GetBarDiameterName(0);
	for(int iLayer = 1; iLayer < NumLayer; iLayer++)
	{
		if(sFirstDiaName != GetBarDiameterName(iLayer))
			return false;
	}
	return true;
}

long MSMainBar::GetBundleNum(long LayerNum)
{
	long nBundleNum = 0;

	CString strBar = GetstrLayerOnALayer(LayerNum);
	if (strBar == _T("NULL"))
		return nBundleNum;

	nBundleNum = MSDeformedBar::GetBundleNum(strBar);
	return nBundleNum;
}

long MSMainBar::GetBundleSpace(long LayerNum)
{
	long nBundleSpace = 0;

	CString strBar = GetstrLayerOnALayer(LayerNum);
	if (strBar == _T("NULL"))
		return nBundleSpace;

	nBundleSpace = MSDeformedBar::GetBundleSpace(strBar);
	return nBundleSpace;
}
bool MSMainBar::GetUserSteelGradeS()
{
	for(int i = 0 ; i < ma_strBar.GetSize(); i++)
	{
		CString sBar = ma_strBar[i];
		int indexS = sBar.Find('S');
		if(indexS == sBar.GetLength() - 1)
			return true;		
	}
	return false;
}