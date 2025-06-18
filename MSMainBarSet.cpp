#include "StdAfx.h"
#include "MSMainBarSet.h"
#include "MSDeformedBar.h"

CString MSMainBarSet::Parser_PLUS = _T("+");
CString MSMainBarSet::Parser_SLASH = _T("/");
CString MSMainBarSet::Parser_SEMICOLON = _T(";");

IMPLEMENT_SERIAL(MSMainBarSet, MSRcObject, VERSIONABLE_SCHEMA | 1)
	MSMainBarSet::MSMainBarSet()
{
	m_Parser = _T("/");
}

MSMainBarSet::~MSMainBarSet(void)
{
}

void MSMainBarSet::SetDataByOldVersion(MSMainBar* pMainBar)
{
	if(pMainBar == NULL)	return;

	RemoveAll();
	SetParser(MSMainBarSet::Parser_SLASH);
	long NumLayer = pMainBar->GetNumLayers();
	for(int i = 0; i < NumLayer; i++)
	{
		MSMainBar myBar(pMainBar->GetstrLayerOnALayer(i));
		ma_Bars.push_back(myBar);
	}
}

void MSMainBarSet::CopyFromMe(MSMainBarSet* pSource)
{
	if(pSource==NULL)	return; 

	m_Parser = pSource->m_Parser;

	ma_Bars.clear();
	long NumStrBar = pSource->ma_Bars.size();
	for(long iStrBar=0; iStrBar<NumStrBar; iStrBar++)
	{
		MSMainBar aBar;
		aBar.CopyFromMe(&pSource->ma_Bars[iStrBar]);
		ma_Bars.push_back(aBar);
	}
}

void MSMainBarSet::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);

	buffer << m_Parser;
	int nSize = ma_Bars.size();
	buffer << nSize;
	for( int i = 0; i < nSize; i++)
		ma_Bars[i].Dump(buffer);
}

void MSMainBarSet::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);

	buffer >> m_Parser;
	int nSize = 0;
	buffer >> nSize;
	ma_Bars.clear();
	CString strData;
	for( int i = 0; i < nSize; i++)
	{
		MSMainBar aBar;
		aBar.Recovery(buffer);
		ma_Bars.push_back(aBar);
	}
}

void MSMainBarSet::Serialize(CArchive &ar)
{
	MSRcObject::Serialize(ar);
	CString strBar;
	if(ar.IsStoring())
	{
		ar << m_Parser;
		long NumStrBar = ma_Bars.size();
		ar << NumStrBar;
		for(long iStrBar=0; iStrBar<NumStrBar; iStrBar++)
		{
			ma_Bars[iStrBar].Serialize(ar);
		}
	}
	else
	{
		ar >> m_Parser;
		long NumStrBar = 0;
		ma_Bars.clear();
		ar >> NumStrBar;
		for(long iStrBar=0; iStrBar<NumStrBar; iStrBar++)
		{
			MSMainBar aBar;
			aBar.Serialize(ar);
			ma_Bars.push_back(aBar);
		}
	}
}

void MSMainBarSet::SetParser(CString parser)
{
	if(parser == _T("/") || parser == _T(";"))
		m_Parser = parser;
}

CString MSMainBarSet::GetParser()
{
	return m_Parser;
}

void MSMainBarSet::InitData(CString strBar)
{
	ma_Bars.clear();
	MSMainBar aBar;	aBar.InitData(strBar);
	ma_Bars.push_back(aBar);
}

void MSMainBarSet::Add(MSMainBar mainBar)
{
	ma_Bars.push_back(mainBar);
}
void MSMainBarSet::RemoveAll()
{
	ma_Bars.clear();
}

CString MSMainBarSet::GetBarString()
{
	if(m_Parser == MSMainBarSet::Parser_SLASH)
		return GetBarString_BySlash();
	else
		return GetBarString_BySemiColon();
}

CString MSMainBarSet::GetBarString_BySlash()
{
	CString barString = _T("");
	if(ma_Bars.size() == 0)	return barString;

	map<CString, vector<int>> diaNameToNumBarsArrMap;
	map<CString, vector<int>>::iterator itArr;
	long NumSet = GetNumSets();
	for(long iSet = 0; iSet<NumSet; iSet++)
	{
		map<CString, int> setDataMap = GetDiaNameToNumBarsOnSet(iSet);
		map<CString, int>::iterator itSet;
		for(itSet = setDataMap.begin(); itSet != setDataMap.end(); itSet++)
		{
			itArr = diaNameToNumBarsArrMap.find(itSet->first);
			if(itArr == diaNameToNumBarsArrMap.end())
			{
				vector<int> NumBarsArr;	NumBarsArr.push_back(itSet->second);
				diaNameToNumBarsArrMap.insert(make_pair(itSet->first, NumBarsArr));
			}
			else
			{
				itArr->second.push_back(itSet->second);
			}
		}
	}

	int mapCount = 0;
	for(itArr = diaNameToNumBarsArrMap.begin(); itArr != diaNameToNumBarsArrMap.end(); itArr++)
	{
		CString setData = _T("");
		CString numData = _T("");
		int barCount = itArr->second.size();
		for(int i = 0; i < barCount; i++)
		{
			CString sTemp;
			sTemp.Format(_T("%ld"), itArr->second[i]);
			if(i + 1 == barCount)
				numData += sTemp;
			else
				numData += (sTemp + MSMainBarSet::Parser_SLASH);
		}
		setData.Format(_T("%s-%s%s"), numData, MSDeformedBar::m_sDefaultBarType, itArr->first);

		if(mapCount + 1 == diaNameToNumBarsArrMap.size())
			barString += setData;
		else
			barString += (setData + MSMainBarSet::Parser_PLUS);

		mapCount++;
	}

	return barString;
}

CString MSMainBarSet::GetBarString_BySemiColon()
{
	CString barString = _T("");
	long NumSet = GetNumSets();
	for(long iSet =0; iSet<NumSet; iSet++)
	{
		MSMainBar theBar = ma_Bars[iSet];
		int NumLayer = theBar.GetNumLayers();

		CString sLayerData = _T("");
		for(int iLayer = 0; iLayer < NumLayer; iLayer++)
		{
			CString strData = theBar.ma_strBar[iLayer];
			if(iLayer + 1== NumLayer)
				sLayerData += strData;
			else
				sLayerData += (strData + MSMainBarSet::Parser_PLUS);
		}
		if(iSet + 1== NumSet)
			barString += sLayerData;
		else
			barString += (sLayerData + m_Parser);

	}
	return barString;
}

long MSMainBarSet::GetNumSets()
{
	return ma_Bars.size();
}

long MSMainBarSet::GetTotalBars()
{
	long TotalBars=0;

	long NumSet = GetNumSets();
	for(long iSet =0; iSet<NumSet; iSet++)
	{
		TotalBars += ma_Bars[iSet].GetTotalBars();
	}
	return TotalBars;
}

long MSMainBarSet::GetNumBarsOnSet(long setNum)
{
	long NumSet = GetNumSets();
	if(setNum < 0 || setNum >= NumSet)
		return 0;	

	long NumBar = 0;
	return ma_Bars[setNum].GetTotalBars();
}

long MSMainBarSet::GetNumLayersOnSet(long setNum)
{
	long NumSet = GetNumSets();
	if (setNum < 0 || setNum >= NumSet)
		return 0;

	long NumBar = 0;
	return ma_Bars[setNum].GetNumLayers();
}

bool MSMainBarSet::IsSameBar(MSMainBarSet& aBarset)
{
	long NumMySet = GetNumSets();
	long NumUrSet = aBarset.GetNumSets();
	if(NumMySet != NumUrSet)	return false;

	CString strMyBar, strUrBar;
	for(long iSet =0; iSet < NumMySet; iSet++)
	{
		if(!ma_Bars[iSet].IsSameBar(aBarset.ma_Bars[iSet]))
			return false;
	}
	return true;
}

map<CString, int> MSMainBarSet::GetDiaNameToNumBarsOnSet(long setNum)
{
	map<CString, int> diaNameToNumBarsMap;
	long NumSet = GetNumSets();
	if(setNum < 0 || setNum >= NumSet)
		return diaNameToNumBarsMap;	

	MSMainBar theBar = ma_Bars[setNum];
	int NumLayer = theBar.GetNumLayers();
	for(int iLayer = 0; iLayer < NumLayer; iLayer++)
	{
		CString sDiaName = theBar.GetBarDiameterName(iLayer);		
		int NumBars = theBar.GetNumBarsOnALayer(iLayer);

		map<CString, int>::iterator it = diaNameToNumBarsMap.find(sDiaName);
		if(it == diaNameToNumBarsMap.end())
			diaNameToNumBarsMap.insert(make_pair(sDiaName, NumBars));
		else
		{
			it->second += NumBars;
		}
	}
	return diaNameToNumBarsMap;
}

vector<double> MSMainBarSet::GetBarDiaAllLayer()
{
	vector<double> DiaArr;
	map<CString, int>::iterator it;
	long NumSet = GetNumSets();
	for(long iSet = 0 ; iSet < NumSet ; iSet++)
	{
		map<CString, int> diaNameToNumBarsMap = GetDiaNameToNumBarsOnSet(iSet);
		for(it = diaNameToNumBarsMap.begin(); it != diaNameToNumBarsMap.end(); it++)
		{
			double dDia = MSDeformedBar::FindDeformedBarDiameterByName(it->first);

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
	}
	return DiaArr;
}
double MSMainBarSet::GetMaxDeformedBarDiameterOnSet(long setNum)
{
	double dMaxDiameter = 0;
	long NumSet = GetNumSets();
	if(setNum < 0 || setNum >= NumSet)
		return dMaxDiameter;	

	MSMainBar theBar = ma_Bars[setNum];
	int NumLayer = theBar.GetNumLayers();
	for(int iLayer = 0; iLayer < NumLayer; iLayer++)
		dMaxDiameter = max(dMaxDiameter,theBar.GetDeformedBarDiameter(iLayer));

	return dMaxDiameter;
}

double MSMainBarSet::GetMaxDeformedBarDiameter()
{
	double dMaxDiameter = 0;
	long NumSet = GetNumSets();
	for(int iSet = 0; iSet < NumSet; iSet++)
	{
		MSMainBar theBar = ma_Bars[iSet];
		int NumLayer = theBar.GetNumLayers();
		for(int iLayer = 0; iLayer < NumLayer; iLayer++)
			dMaxDiameter = max(dMaxDiameter,theBar.GetDeformedBarDiameter(iLayer));
	}
	return dMaxDiameter;
}
bool MSMainBarSet::GetUserSteelGradeS()
{
	long NumSet = GetNumSets();
	for(int iSet = 0; iSet < NumSet; iSet++)
	{
		MSMainBar theBar = ma_Bars[iSet];
		if(theBar.GetUserSteelGradeS())
			return true;
	}
	return false;
}
CString MSMainBarSet::GetMaxDiaNameOnSet(long setNum)
{
	CString maxDiaName = _T("0");
	long NumSet = GetNumSets();
	if(setNum < 0 || setNum >= NumSet)
		return maxDiaName;


	MSMainBar theBar = ma_Bars[setNum];
	int NumLayer = theBar.GetNumLayers();
	double maxDia = 0.0;
	for(int iLayer = 0; iLayer < NumLayer; iLayer++)
	{
		CString sDiaName = theBar.GetBarDiameterName(iLayer);
		double curDia = _ttof(sDiaName);
		if(maxDia < curDia)
		{
			maxDia = curDia;
			maxDiaName = sDiaName;
		}
	}

	return maxDiaName;
}

vector<CString> MSMainBarSet::MakeLayerDiaArr(long setNum)
{
	map<CString, int> mSet = GetDiaNameToNumBarsOnSet(setNum); 
	if(mSet.size() == 0)
		return vector<CString>();
	map<CString, int>::iterator Iter;
	map<double, CString> mDia;//지름별 정렬

	for(Iter = mSet.begin() ; Iter != mSet.end() ; Iter++)   
	{
		MSDeformedBar bar(Iter->first);
		mDia.insert(make_pair(bar.m_Dia,Iter->first));
	}
	long nTotalBar = GetNumBarsOnSet(setNum);
	map<int,int> mRemoveLoc;//이미 배치된 자리저장
	map<int,int>::iterator itRemoveLoc;
	vector<CString> LayerDiaArr(nTotalBar);
	int numBar = 0;
	for(auto it = mDia.rbegin();it!=mDia.rend();it++)
	{
		Iter = mSet.find(it->second);
		if(Iter==mSet.end())
		{
			ASSERT(0);
			continue;
		}
		numBar = Iter->second;
		int LocRat = (nTotalBar-mRemoveLoc.size())/max(numBar-1,1);
		int index = 0;
		int iBar = 0;
		while(index<nTotalBar&&iBar < numBar/2)
		{
			itRemoveLoc = mRemoveLoc.find(index);
			if(itRemoveLoc == mRemoveLoc.end())//없을경우
			{
				LayerDiaArr[index] = it->second;
				mRemoveLoc.insert(make_pair(index,index));
				index+=LocRat;
				iBar++;
			}
			else
				index++;
		}
		index = nTotalBar-1;

		while(index>=0&&iBar< numBar)
		{
			itRemoveLoc = mRemoveLoc.find(index);
			if(itRemoveLoc == mRemoveLoc.end())//없을경우
			{
				LayerDiaArr[index] = it->second;
				mRemoveLoc.insert(make_pair(index,index));
				index-=LocRat;
				iBar++;
			}
			else
				index--;
		}
		//남은철근은 앞에서 부터 빈자리에 추가
		if(iBar < numBar)
		{
			index = 0;
			while(index<nTotalBar&&iBar<numBar)
			{
				itRemoveLoc = mRemoveLoc.find(index);
				if(itRemoveLoc == mRemoveLoc.end())//없을경우
				{
					LayerDiaArr[index] = it->second;
					mRemoveLoc.insert(make_pair(index,index));
					iBar++;
				}
				index++;
			}
		}
		if(iBar!=numBar)
			ASSERT(0);//갯수가 같아야 함.
	}
	if(mRemoveLoc.size()!=nTotalBar)
		ASSERT(0);//갯수가 같아야 함.


	return LayerDiaArr;
}
void MSMainBarSet::SetBarString(CString fullData)
{
	MSMainBar mainBar;

	fullData.Replace(_T(" "), _T(""));
	fullData.MakeUpper();

	if (fullData == _T(""))
		return;

	int indexSlash = fullData.Find(MSMainBarSet::Parser_SLASH);
	int indexSemiColon = fullData.Find(MSMainBarSet::Parser_SEMICOLON);
	if(indexSlash == -1 && indexSemiColon == -1)// 1단 레이어
	{
		RemoveAll();

		ParsingMainBar_ByOnlyPlus(&mainBar, fullData);
		Add(mainBar);
		return;
	}

	if(indexSemiColon != -1)
		ParsingMainBar_ContainSemiColon(fullData);
	else if(indexSlash != -1)
		ParsingMainBar_ContainSlash(fullData);
}

// "+"를 포함할수 있는 레이어 데이타 ("/"없다)
void MSMainBarSet::ParsingMainBar_ByOnlyPlus(MSMainBar* pMainBar, CString barData)
{
	int nTokenPos = 0;
	CString strToken = barData.Tokenize(MSMainBarSet::Parser_PLUS, nTokenPos);

	pMainBar->Add(strToken);
	while (!strToken.IsEmpty())
	{
		strToken = barData.Tokenize(MSMainBarSet::Parser_PLUS, nTokenPos);
		if (!strToken.IsEmpty())
			pMainBar->Add(strToken);
	}
}

// ex) 4/2-HD13+2-HD16
void MSMainBarSet::ParsingMainBar_ContainSlash(CString sData)
{
	RemoveAll();

	SetParser(MSMainBarSet::Parser_SLASH);

	int nTokenPos = 0;
	CString strToken = sData.Tokenize(MSMainBarSet::Parser_PLUS, nTokenPos);
	ParsingMainBar_BySlash(strToken);
	while (!strToken.IsEmpty())
	{
		strToken = sData.Tokenize(MSMainBarSet::Parser_PLUS, nTokenPos);
		if (!strToken.IsEmpty())
			ParsingMainBar_BySlash(strToken);
	}
}

// ex) 4-HD13+2-HD16;2-HD13
void MSMainBarSet::ParsingMainBar_ContainSemiColon(CString fullData)
{
	RemoveAll();

	SetParser(MSMainBarSet::Parser_SEMICOLON);
	int nTokenPos = 0;
	CString strToken = fullData.Tokenize(m_Parser, nTokenPos);

	MSMainBar mainBar;
	ParsingMainBar_ByOnlyPlus(&mainBar, strToken);
	ma_Bars.push_back(mainBar);
	while (!strToken.IsEmpty())
	{
		strToken = fullData.Tokenize(m_Parser, nTokenPos);
		if (!strToken.IsEmpty())
		{
			MSMainBar otherMainBar;
			ParsingMainBar_ByOnlyPlus(&otherMainBar, strToken);
			ma_Bars.push_back(otherMainBar);
		}
	}
}

void MSMainBarSet::ParsingMainBar_BySlash(CString sData)
{
	CString MainBar = sData;
	int indexD = MainBar.Find('D');
	int indexDash = MainBar.Find('-');
	int length = MainBar.GetLength();
	CString sDiaName = MainBar.Mid(indexD + 1, (length - (indexD + 1)));
	CString sBarData;

	int indexSet = 0;
	int startIndex = 0;
	for(int i = 0; i < indexD; i++)
	{
		if(sData[i] == MSMainBarSet::Parser_SLASH)
		{
			if(ma_Bars.size() <= indexSet)
				ma_Bars.push_back(MSMainBar());

			int NumEa = _ttol(sData.Mid(startIndex, i-startIndex));
			sBarData.Format(_T("%ld-%s%s"), NumEa, MSDeformedBar::m_sDefaultBarType, sDiaName);
			ma_Bars[indexSet].Add(sBarData);
			indexSet++;
			startIndex = i+1;
		}
	}
	int NumEa = _ttol(sData.Mid(startIndex, indexDash - startIndex));
	sBarData.Format(_T("%ld-%s%s"), NumEa, MSDeformedBar::m_sDefaultBarType, sDiaName);
	if(ma_Bars.size() <= indexSet)
		ma_Bars.push_back(MSMainBar());
	ma_Bars[indexSet].Add(sBarData);
}

bool MSMainBarSet::CheckMainBarSet(CString fullData)
{
	MSMainBarSet mainSet;
	fullData.Replace(_T(" "), _T(""));
	fullData.MakeUpper();

	int indexSemiColon = fullData.Find(MSMainBarSet::Parser_SEMICOLON);
	int indexSlash = fullData.Find(MSMainBarSet::Parser_SLASH);

	if(indexSemiColon != -1 && indexSlash != -1)	return false;

	if(indexSemiColon == -1 && indexSlash == -1)
		return mainSet.CheckMainBar_ByPlus(fullData);

	if(indexSemiColon != -1)
		return mainSet.CheckMainBar_ContainSemiColon(fullData);
	else if(indexSlash)
		return mainSet.CheckMainBar_ContainSlash(fullData);
	return false;
}

bool MSMainBarSet::CheckMainBar_ByPlus(CString sData)
{
	int nTokenPos = 0;
	CString strToken = sData.Tokenize(MSMainBarSet::Parser_PLUS, nTokenPos);

	if(!CheckMainBar(strToken))	return false;
	while (!strToken.IsEmpty())
	{
		strToken = sData.Tokenize(MSMainBarSet::Parser_PLUS, nTokenPos);
		if (!strToken.IsEmpty())
		{
			if(!CheckMainBar(strToken))	return false;
		}
	}
	return true;
}

bool MSMainBarSet::CheckMainBar(CString sData)
{
	int indexDash = sData.Find(_T("-"));
	int indexD = sData.Find(_T("D"));
	if(indexDash == -1 || indexD == -1)	return false;

	CString sDiaName = sData.Mid(indexD + 1, (sData.GetLength() - (indexD + 1)));
	if(MSDeformedBar::FindBarDiameterIndexByName(sDiaName)==-1)	return false;

	return true;
}

bool MSMainBarSet::CheckMainBar_ContainSemiColon(CString fullData)
{
	int nTokenPos = 0;
	CString strToken = fullData.Tokenize(MSMainBarSet::Parser_SEMICOLON, nTokenPos);

	if(!CheckMainBar_ByPlus(strToken))	return false;
	while (!strToken.IsEmpty())
	{
		strToken = fullData.Tokenize(MSMainBarSet::Parser_SEMICOLON, nTokenPos);
		if (!strToken.IsEmpty())
		{
			if(!CheckMainBar_ByPlus(strToken))	return false;
		}
	}
	return true;
}

bool MSMainBarSet::CheckMainBar_ContainSlash(CString fullData)
{
	int nTokenPos = 0;
	CString strToken = fullData.Tokenize(MSMainBarSet::Parser_PLUS, nTokenPos);

	if(!CheckMainBar_BySlash(strToken))	return false;
	while (!strToken.IsEmpty())
	{
		strToken = fullData.Tokenize(MSMainBarSet::Parser_PLUS, nTokenPos);
		if (!strToken.IsEmpty())
		{
			if(!CheckMainBar_BySlash(strToken))	return false;
		}
	}
	return true;
}

bool MSMainBarSet::CheckMainBar_BySlash(CString sData)
{
	if(!CheckMainBar(sData))	return false;

	// TODO(김신웅) : "/"에 대해서 어떤 체크를 해야 할지 고려
	return true;
}

int MSMainBarSet::GetNumBarsOnSet(CString fullData, int setNum)
{
	int NumBars = 0;
	MSMainBarSet mainBarSet;
	mainBarSet.SetBarString(fullData);

	map<CString, int> dicNameToNumBarsMap = mainBarSet.GetDiaNameToNumBarsOnSet(setNum);
	map<CString, int>::iterator it;
	for(it = dicNameToNumBarsMap.begin(); it != dicNameToNumBarsMap.end(); it++)
	{
		NumBars += it->second;
	}
	return NumBars;
}

CString MSMainBarSet::GetDiaNameOnSet(CString fullData, int setNum)
{
	int NumBars = 0;
	MSMainBarSet mainBarSet;
	mainBarSet.SetBarString(fullData);

	double dia = mainBarSet.GetMaxDeformedBarDiameterOnSet(setNum);
	return MSDeformedBar::FindBarDiameterName(dia);
}

int MSMainBarSet::GetNumSets(CString fullData)
{
	int NumBars = 0;
	MSMainBarSet mainBarSet;
	mainBarSet.SetBarString(fullData);

	return mainBarSet.GetNumSets();
}
