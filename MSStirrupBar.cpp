#include "StdAfx.h"
#include "MSStirrupBar.h"

#include "MSDeformedBar.h"
/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////Strirrup Bar /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(MSStirrupBar, MSRcObject, VERSIONABLE_SCHEMA | 1)
MSStirrupBar::MSStirrupBar(CString strBar)
{
	m_bAlternate = FALSE;
	ma_strBar.Add(strBar.MakeUpper());
}

MSStirrupBar::~MSStirrupBar()
{

}
void MSStirrupBar::CopyFromMe(MSStirrupBar* pSource)
{
	if(pSource==NULL)	return; 

	ma_strBar.RemoveAll();
	long NumStrBar = pSource->ma_strBar.GetSize();
	for(long iStrBar=0; iStrBar<NumStrBar; iStrBar++)
	{
		ma_strBar.Add(pSource->ma_strBar[iStrBar]);
	}

}

void MSStirrupBar::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	int nSize = ma_strBar.GetSize();
	buffer << nSize;
	for( int i = 0; i < nSize; i++)
		buffer << ma_strBar[i];
}

void MSStirrupBar::Recovery(EFS::EFS_Buffer& buffer)
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

void MSStirrupBar::Serialize(CArchive &ar)
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

long MSStirrupBar::InitData(CString strBar)
{
	ma_strBar.RemoveAll();
	CString tempBar = strBar.MakeUpper();
	
	int idxPlus = strBar.ReverseFind('+');
	int idxAt = strBar.Find('@');
	if (idxPlus == -1)
		idxPlus = 0;
	int idxS = strBar.Find('S', idxPlus);
	bool isSteelGradeS = false;
	if (idxS != -1 && idxS < idxAt)
		isSteelGradeS = true;

	vector<int> diaArr = getDiaArr2StrpBar(strBar);
	int diaCnt = diaArr.size();
	bool isAllSameDia = true;
	for (int idx = 0; idx < diaCnt - 1; idx++)
	{
		if (diaArr[idx] != diaArr[idx + 1])
		{
			isAllSameDia = false;
			break;
		}
	}

	int indexAt = tempBar.Find('@');
	if (indexAt >= 0)
	{
		if (isAllSameDia == false)
		{
			long nBundleNum = MSDeformedBar::GetBundleNum(tempBar);
			long nBundleSpace = MSDeformedBar::GetBundleSpace(tempBar);

			int length = tempBar.GetLength();
			CString strSpacing = tempBar.Mid(indexAt + 1, length - (indexAt + 1));

			for (int idx = 0; idx < diaCnt; idx++)
			{
				CString strDia;

				CString strDiaIdx;
				if (isSteelGradeS == true && (idx == diaCnt - 1))
					strDiaIdx.Format(_T("%dS"), diaArr[idx]);
				else
					strDiaIdx.Format(_T("%d"), diaArr[idx]);

				if (nBundleNum == 0)
					strDia.Format(_T("%s%s@%s"), MSDeformedBar::m_sDefaultBarType, strDiaIdx, strSpacing);
				else
					strDia.Format(_T("%ld%s%s@%s"), nBundleNum, MSDeformedBar::m_sDefaultBarType, strDiaIdx, strSpacing);

				if (nBundleSpace != 0)
					strDia.Format(_T("%s[B@=%ld]"), strDia, nBundleSpace);
				
				ma_strBar.Add(strDia);
			}
		}
		else
			ma_strBar.Add(tempBar);
	}

	return true;
}

long MSStirrupBar::Add(CString strBar)
{
	return ma_strBar.Add(strBar.MakeUpper());
}
long MSStirrupBar::InitData(const char* strBar)
{
	ma_strBar.RemoveAll();
	return ma_strBar.Add(CString(strBar).MakeUpper());
}
long MSStirrupBar::Add(const char* strBar)
{
	return ma_strBar.Add(CString(strBar).MakeUpper());
}

CString MSStirrupBar::GetBarType(long LayerNum)
{
	CString strBar = _T("NULL");
	if(LayerNum < 0 || ma_strBar.GetSize() <= LayerNum)	return strBar;

	strBar = ma_strBar[LayerNum];
	CString BarType;
	if(strBar.Left(1) == _T("H")) BarType = _T("HD");
	else                      BarType = _T("D");

	return BarType;
}

double MSStirrupBar::GetSpacing(long LayerNum)
{
	double Spacing = 0.0;
	CString strBar = _T("NULL");
	if(LayerNum < 0 || ma_strBar.GetSize() <= LayerNum)	return Spacing;
	strBar = ma_strBar[LayerNum];

	Spacing = MSDeformedBar::GetBarSpacing(strBar);
	return Spacing;
}

long MSStirrupBar::GetBundleNum(long LayerNum)
{
	long nBundleNum = 0;
	CString strBar = _T("NULL");
	if (LayerNum < 0 || ma_strBar.GetSize() <= LayerNum)	return nBundleNum;
	strBar = ma_strBar[LayerNum];

	nBundleNum = MSDeformedBar::GetBundleNum(strBar);
	return nBundleNum;
}

long MSStirrupBar::GetBundleSpace(long LayerNum)
{
	long nBundleSpace = 0;
	CString strBar = _T("NULL");
	if (LayerNum < 0 || ma_strBar.GetSize() <= LayerNum)	return nBundleSpace;
	strBar = ma_strBar[LayerNum];

	nBundleSpace = MSDeformedBar::GetBundleSpace(strBar);
	return nBundleSpace;

}
void MSStirrupBar::SetBarDiaIdx(CString sDiaName, long LayerNum)
{
	if (sDiaName == _T(""))
		return;

	double Spacing = GetSpacing(LayerNum);
	long nBundleNum = GetBundleNum(LayerNum);
	long nBundleSpace = GetBundleSpace(LayerNum);
	CString Type = GetBarType(LayerNum);
	SetStirrup(LayerNum, nBundleNum, Type, sDiaName, Spacing, nBundleSpace);
}

void MSStirrupBar::SetStirrup(long LayerNum, long nBundleNum, CString Type, CString sDiaName, double Spacing, long nBundleSpace)
{
	CString strBar;
	if(nBundleNum == 0)
		strBar.Format(_T("%s%s@%ld"), Type, sDiaName, (long)Spacing);
	else
		strBar.Format(_T("%ld%s%s@%ld"), nBundleNum, Type, sDiaName, (long)Spacing);
		
	if (nBundleSpace != 0)
		strBar.Format(_T("%s[B@=%ld]"), strBar, nBundleSpace);
					
	ma_strBar.SetAtGrow(LayerNum, strBar);
}

void MSStirrupBar::SetStirrup(long LayerNum, CString strStirrup)
{
	ma_strBar.SetAtGrow(LayerNum, strStirrup);
}

int MSStirrupBar::SetSpace(long LayerNum, double NewSpacing)
{
	int Rvalue = 1;
	CString sDiaName = GetBarDiameterName(LayerNum);

	double Space = NewSpacing;
	if (Space < 50. || Space > 10000.) Rvalue = 0;
	else SetStirrup(LayerNum, GetBundleNum(LayerNum), GetBarType(), sDiaName, Space, GetBundleSpace(LayerNum));

	return Rvalue;
}

void MSStirrupBar::SetBarType(long LayerNum, char HD)
{
	CString Type;
	if (HD == 'H' || HD == 0) Type = _T("HD");
	else                      Type = _T("D");

	CString sDiaName = GetBarDiameterName(LayerNum);
	double Spacing = GetSpacing(LayerNum);
	long nBundleNum = GetBundleNum(LayerNum);
	long nBundleSpace = GetBundleSpace(LayerNum);

	SetStirrup(LayerNum, nBundleNum, Type, sDiaName, Spacing, nBundleSpace);
}
void MSStirrupBar::SetBarType(long LayerNum, CString hd)
{
	CString sDiaName = GetBarDiameterName(LayerNum);
	double Spacing = GetSpacing(LayerNum);
	long nBundleNum = GetBundleNum(LayerNum);
	long nBundleSpace = GetBundleSpace(LayerNum);

	SetStirrup(LayerNum, nBundleNum, hd.MakeUpper(), sDiaName, Spacing, nBundleSpace);
}

CString MSStirrupBar::GetStirrupBarString()
{
	CString strBar;
	CString strBarType = GetBarType(0);
	double dBarSpacing = GetSpacing(0);
	long nBundleNum = GetBundleNum(0);
	long nBundleSpace = GetBundleSpace(0);

	vector<CString> diaArr = GetAllDiameterName();
	int diaCnt = diaArr.size();
	bool isAllSameDia = true;
	for (int idx = 0; idx < diaCnt - 1; idx++)
	{
		if (diaArr[idx] != diaArr[idx + 1])
		{
			isAllSameDia = false;
			break;
		}
	}

	if (diaCnt > 2)
	{
		CString strDias = _T("");
		if (isAllSameDia)
			strDias = diaArr[0];
		else
		{
			for (int idx = 0; idx < diaCnt; idx++)
			{
				if(idx == diaCnt - 1)
					strDias += diaArr[idx];
				else
					strDias += diaArr[idx] + _T("+");
			}

			if (nBundleNum == 0)
				strBar.Format(_T("%s%s@%ld"), MSDeformedBar::m_sDefaultBarType, strDias, (long)dBarSpacing);
			else
				strBar.Format(_T("%ld%s%s@%ld"), nBundleNum, MSDeformedBar::m_sDefaultBarType, strDias, (long)dBarSpacing);

			if (nBundleSpace != 0)
				strBar.Format(_T("%s[B@=%ld]"), strBar, nBundleSpace);
		}
		return strBar;
	}

	CString firstDiaName = GetFirstBarDiaName();
	CString secondDiaName = GetLastBarDiaName();
	if (nBundleNum == 0)
	{
		if (firstDiaName == secondDiaName)
			strBar.Format(_T("%s%s@%ld"), MSDeformedBar::m_sDefaultBarType, firstDiaName, (long)dBarSpacing);
		else
			strBar.Format(_T("%s%s+%s@%ld"), MSDeformedBar::m_sDefaultBarType,firstDiaName, secondDiaName, (long)dBarSpacing);
	}
	else
	{
		if (firstDiaName == secondDiaName)
			strBar.Format(_T("%ld%s%s@%ld"), nBundleNum,MSDeformedBar::m_sDefaultBarType, firstDiaName, (long)dBarSpacing);
		else
			strBar.Format(_T("%ld%s%s+%s@%ld"), nBundleNum,MSDeformedBar::m_sDefaultBarType, firstDiaName, secondDiaName, (long)dBarSpacing);
	}
	
	if (nBundleSpace != 0)
		strBar.Format(_T("%s[B@=%ld]"), strBar, nBundleSpace);
		
	return strBar;
}

CString MSStirrupBar::GetStirrupBarStringbyCulture()
{
	long nBundleNum = GetBundleNum();
	long nBundleSpace = GetBundleSpace();
	CString strBarType = MSDeformedBar::GetDefaultRebarNamePrefix();
	double dBarSpacing = GetSpacing(0);

	vector<CString> diaArr = GetAllDiameterName();
	return GetStirrupBarStringByCulture(nBundleNum, strBarType, diaArr, dBarSpacing, nBundleSpace);
}

long MSStirrupBar::GetBarTypeIndex(long LayerNum)
{
	CString Type = GetBarType(LayerNum);
	if(Type == _T("HD"))	return 0;
	else							return 1;
}

CString MSStirrupBar::GetFirstBarDiaName()
{
	return GetBarDiameterName(0);
}

CString  MSStirrupBar::GetLastBarDiaName()
{
	long NumLayer = GetNumLayers();
	return GetBarDiameterName(NumLayer-1);
}

double MSStirrupBar::GetMaxBarDia()
{
	double dDiameter = 0;
	for (long iLayer = 0; iLayer < GetNumLayers(); iLayer++)
	{
		double dCurDiameter = GetDeformedBarDiameter(iLayer);
		if (dDiameter < dCurDiameter)
			dDiameter = dCurDiameter;
	}

	return dDiameter;
}

double MSStirrupBar::GetMinBarDia()
{
	double dDiameter = 999999;
	for (long iLayer = 0; iLayer < GetNumLayers(); iLayer++)
	{
		double dCurDiameter = GetDeformedBarDiameter(iLayer);
		if (dDiameter > dCurDiameter)
			dDiameter = dCurDiameter;
	}

	return dDiameter;
}
CString MSStirrupBar::GetMaxBarType()
{
	double dDiameter = 0;
	CString strType = MSDeformedBar::m_sDefaultBarType;
	for (long iLayer = 0; iLayer < GetNumLayers(); iLayer++)
	{
		double dCurDiameter = GetDeformedBarDiameter(iLayer);
		if (dDiameter < dCurDiameter)
		{
			dDiameter = dCurDiameter;
			strType = GetBarType(iLayer);
		}
	}
	return strType;
}

CString MSStirrupBar::GetStringOnALayer(long LayerNum)
{
	CString strBar = _T("");
	long NumLayer = GetNumLayers();
	if(LayerNum < 0 || NumLayer <= LayerNum )
		return strBar;

	strBar = ma_strBar[LayerNum];
	return strBar;
}

long MSStirrupBar::GetNumLayers()
{
	return ma_strBar.GetSize();
}

BOOL MSStirrupBar::IsSameBar(MSStirrupBar& aBar)
{
	long NumMyLayer = GetNumLayers();
	long NumUrLayer = aBar.GetNumLayers();
	if(NumMyLayer != NumUrLayer)
		return FALSE;
	if(this->GetUserSteelGradeS() != aBar.GetUserSteelGradeS())
		return FALSE;
	for(long iLayer=0; iLayer<NumMyLayer; iLayer++)
	{
		if(fabs(GetSpacing(iLayer) - aBar.GetSpacing(iLayer)) > DTOL5
			|| fabs(GetDeformedBarDiameter(iLayer) - aBar.GetDeformedBarDiameter(iLayer)) > DTOL5)
			return FALSE;
	}
	return TRUE;
}

bool MSStirrupBar::IsCorrectRebarBySpace( CString strBar )
{
	strBar = strBar.Trim();
	long AtMarkPos = strBar.Find('@');

	if(AtMarkPos==-1)
		return false;

	CString strRebarName = strBar.Left(AtMarkPos);
	if(MSDeformedBar::IsCorrectRebarName(strRebarName)==false)
		return false;

// 	CString strRebarSpace = strBar.Right(strBar.ReverseFind('@'));
	CString strRebarSpace = strBar.Right(strBar.GetLength()-AtMarkPos-1);
	long RebarSpace = _ttol(strRebarSpace);
	if(RebarSpace==0)
		return false;

	return true;
}

vector<double> MSStirrupBar::GetBarDiaAllLayer()
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

double MSStirrupBar::GetDeformedBarDiameter(long LayerNum)
{
	double dDia = 0;
	if (ma_strBar.GetSize() <= 0 || LayerNum < 0)	return dDia;
	if(ma_strBar.GetSize()-1 >= LayerNum)
		dDia = MSDeformedBar::GetDeformedBarDiameter(ma_strBar[LayerNum]);
	return dDia;
}

CString MSStirrupBar::GetBarDiameterName(long LayerNum)
{
	CString sDiaName = _T("");
	if(ma_strBar.GetSize() <= 0 || LayerNum < 0)	return sDiaName;
	if(ma_strBar.GetSize()-1 >= LayerNum)
	sDiaName = MSDeformedBar::GetBarDiameterName(ma_strBar[LayerNum]);
	return sDiaName;
}

CString MSStirrupBar::getFirstDiaName2StrpBar(CString strBar)
{
	int Length = strBar.GetLength();
	int indexD = strBar.Find('D');
	int indexAt = strBar.Find('@');
	int indexPlus = strBar.Find('+');
	if (indexD == -1 || indexAt == -1)
	{
		ASSERT(0);	return _T("");
	}
	if (indexPlus == -1)
	{
		CString strDia = strBar.Mid(indexD + 1, (indexAt - (indexD + 1)));
		return strDia;
	}
	else//교대배근 일경우
	{
		CString strDia = strBar.Mid(indexD + 1, (indexPlus - (indexD + 1)));
		return strDia;
	}
}

CString MSStirrupBar::getSecondDiaName2StrpBar(CString strBar)
{
	int Length = strBar.GetLength();
	int indexD = strBar.Find('D');
	int indexAt = strBar.Find('@');
	int indexPlus = strBar.Find('+');
	if (indexD == -1 || indexAt == -1)
		return _T("");
	if (indexPlus == -1)
	{
		CString strDia = strBar.Mid(indexD + 1, (indexAt - (indexD + 1)));
		return strDia;
	}
	else
	{
		CString strDia = strBar.Mid(indexPlus + 1, (indexAt - (indexPlus + 1)));
		return strDia;
	}
}

void MSStirrupBar::OnlyFirstDiaSet()
{
	int nLength = ma_strBar.GetSize();

	if (nLength <= 1)	return;

	for (int index = 1; index < nLength; index++)
		ma_strBar.RemoveAt(index);
}
bool MSStirrupBar::GetUserSteelGradeS()
{
	for(int i = 0 ; i < ma_strBar.GetSize(); i++)
	{
		CString sBar = ma_strBar[i];
		int indexD = sBar.Find('D');
		int indexAt = sBar.Find('@');
		int indexS = sBar.Find('S');
		if(indexD < indexS && indexAt - 1 ==  indexS)
			return true;
	}
	return false;
}

vector<double> MSStirrupBar::GetAllDiaArr()
{
	vector<double> DiaArr;
	long NumLayer = GetNumLayers();
	for(long iLayer = 0 ; iLayer < NumLayer ; iLayer++)
	{
		double dDia = GetDeformedBarDiameter(iLayer);
		DiaArr.push_back(dDia);
	}
	return DiaArr;
}

CString MSStirrupBar::GetStirrupBarStringByCulture(long nBundleNum, CString strBarType, vector<CString> diaArr, double dBarSpacing, long nBundleSpace)
{
	CString strBar = _T("");

	long nBarSpacing = (long)(dBarSpacing + DTOL5);
	if (MSDeformedBar::sm_nRebarCode == MSDeformedBar::REBAR_MATERIAL_ASTM)
		nBarSpacing = (long)(dBarSpacing / 25.4 + DTOL5);

	CString strDia = _T("");
	CString str = _T("");
	for (int idx = 0; idx < diaArr.size(); idx++)
	{
		if (idx == diaArr.size() - 1)
			strDia += diaArr[idx];
		else
			strDia += diaArr[idx] + _T("+");
	}

	if (nBundleNum == 0)
		strBar.Format(_T("%s%s@%ld"), strBarType, strDia, nBarSpacing);
	else
		strBar.Format(_T("%ld%s%s@%ld"), nBundleNum, strBarType, strDia, nBarSpacing);

	if (nBundleSpace != 0)
		strBar.Format(_T("%s[B@=%ld]"), strBar, nBundleSpace);

	return strBar;
}

vector<int> MSStirrupBar::getDiaArr2StrpBar(CString strBar)
{
	vector<int> diaArr;
	int indexD = strBar.Find('D');
	int indexAt = strBar.Find('@');
	if (indexAt == -1)
		indexAt = strBar.GetLength();

	int iStart = indexD;
	while (iStart != -1)
	{
		int curIdx = strBar.Find(_T("+"), iStart + 1);
		if (curIdx == -1)
			break;

		CString strDia = strBar.Mid(iStart + 1, curIdx - (iStart + 1));
		iStart = curIdx;
		int nDia = _ttoi(strDia);
		diaArr.push_back(nDia);
	}

	CString strDia = strBar.Mid(iStart + 1, indexAt - (iStart + 1));
	int nDia = _ttoi(strDia);
	diaArr.push_back(nDia);

	return diaArr;
}

vector<CString> MSStirrupBar::GetAllDiameterName()
{
	vector<CString> diaArr;
	for (int idx = 0; idx < ma_strBar.GetSize(); idx++)
	{
		CString strDia = MSDeformedBar::GetBarDiameterName(ma_strBar[idx]);
		diaArr.push_back(strDia);
	}
	return diaArr;
}