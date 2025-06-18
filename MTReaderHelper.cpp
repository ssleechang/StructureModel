#include "StdAfx.h"
#include "MTReaderHelper.h"
#include "MTReader.h"
#include "MTResultElement.h"

#include "StructureModel\MSBaseBuilding.h"
#include "StructureModel\MSDeformedBar.h"
#include "StructureModel\MSMainBarSet.h"

MTReaderHelper::MTReaderHelper(MSBaseBuilding* pBldg, MTReader* pReader)
{
	mp_Bldg = pBldg;
	mp_Reader = pReader;
	mp_CompoM = NULL;
	m_IsMaked = true;
	mp_ResultElement = new MTResultElement();
}


MTReaderHelper::~MTReaderHelper(void)
{
// 	delete mp_ResultCompoM;
}

CString MTReaderHelper::GetCompoMName()
{
	CString sName;
	sName.Format(_T("%s(%ld~%ld)"), mp_CompoM->m_Name, mp_CompoM->m_nFromStory, mp_CompoM->m_nToStory);
	return sName;
}

GM2DRectangle* MTReaderHelper::GetResultArea()
{
	return mp_ResultElement->GetArea();
}
int MTReaderHelper::CreateErrorObject(CString GroupName, int Level, int ErrorCode, CString Message)
{
	auto pArea = GetResultArea();
	return mp_Reader->CreateErrorObject(GroupName, Level, ErrorCode, Message, pArea);
}

void MTReaderHelper::AddErrorFigureIDs(vector<int> figureIDs)
{
	if(figureIDs.size() == 0)	return;

	for(auto it = figureIDs.begin(); it != figureIDs.end(); it++)
		mp_Reader->AddErrorFigureID(*it);
}
void MTReaderHelper::AddErrorFigureID(int figureID)
{
	vector<int> figureIDs;
	figureIDs.push_back(figureID);
	AddErrorFigureIDs(figureIDs);
}

bool MTReaderHelper::CheckDia(double dDia)
{
	CString sDiaName;	
	long nDia = (long)dDia;
	if(fabs(dDia - nDia) < 0.1)
		sDiaName.Format(_T("%ld"), nDia);
	else
		sDiaName.Format(_T("%.1f"), dDia);

	if(MSDeformedBar::FindBarDiameterIndexByName(sDiaName)==-1)	return false;
	return true;
}

vector<double> MTReaderHelper::Parsing_SteelName(CString strData)
{
	vector<double> params;
	CString sectionData = strData.MakeUpper();
	int indexDash = sectionData.Find('-');
	if(indexDash == -1)	return params;

	CString sSectionData = sectionData.Mid(indexDash+1, sectionData.GetLength()-(indexDash+1));
	vector<CString> stringArr;
	int nData = ParsingTextData(sSectionData, stringArr, _T("X"));
	for(int iData = 0; iData < nData; iData++)
	{
		double value = _ttof(stringArr[iData]);
		params.push_back(value);
	}
	return params;
}

void MTReaderHelper::Parsing_StudBolt(CString strData, int& numBolt, MSStirrupBar* pStirrup)
{
	int indexColon = strData.Find(':');
	int indexDash = strData.Find('-');
	int indexPi = strData.Find('Ø');	// 못찾음

	if(indexColon == -1 || indexDash == -1)// || indexPi == -1)
		return;

	numBolt = _ttol(strData.Mid(indexColon+1, indexDash - (indexColon+1)));
	CString sBar = strData.Mid(indexDash+2, strData.GetLength()-(indexDash+2));
	CString StirrupBar;
	StirrupBar.Format(_T("%s%s"), MSDeformedBar::m_sDefaultBarType, sBar);
	pStirrup->InitData(StirrupBar);
}

int MTReaderHelper::ParsingTextData(CString text, vector<CString>& dataArr, CString parser)
{
	CString sText = text.MakeUpper();

	parser = parser.MakeUpper();
	int length = sText.GetLength();
	int startIdx = 0;
	for (int i = 0; i < length; i++)
	{
		if (sText.GetAt(i) == parser)
		{
			CString data = sText.Mid(startIdx, (i - startIdx));
			dataArr.push_back(data);
			startIdx = i + 1;
		}
	}
	CString data = sText.Mid(startIdx, (length - startIdx));
	dataArr.push_back(data);
	return (int)dataArr.size();
}

bool MTReaderHelper::IsPlainConcreteReinforceBar()
{
	return mp_Reader->IsPlainConcreteReinforceBar();
}

MTResultItem* MTReaderHelper::AddResultItem(MTResultItem::eItemType itemType, CString text)
{
	return mp_ResultElement->AddResultItem(itemType, text);
}

MTResultItem* MTReaderHelper::GetResultItem(MTResultItem::eItemType itemType)
{
	return mp_ResultElement->GetResultItem(itemType);
}

MTResultElement* MTReaderHelper::AddSubElement(MTResultItem::eItemType itemType)
{
	return mp_ResultElement->AddSubElement(itemType);
}
MTResultElement* MTReaderHelper::GetSubElement(MTResultItem::eItemType itemType)
{
	return mp_ResultElement->GetSubElement(itemType);
}

MTResultItem* MTReaderHelper::AddSubResultItem(MTResultItem::eItemType elementType, MTResultItem::eItemType itemType, CString text)
{
	return mp_ResultElement->AddSubItem(elementType, itemType, text);
}

MTResultItem* MTReaderHelper::GetSubResultItem(MTResultItem::eItemType elementType, MTResultItem::eItemType itemType)
{
	return mp_ResultElement->GetSubItem(elementType, itemType);
}

void MTReaderHelper::SetLocationList(MTResultItem::eItemType itemType, vector<GM2DVector>& locArr)
{
	mp_ResultElement->SetLocationList(itemType, locArr);
}

void MTReaderHelper::CheckName(CString Name, int figureID)
{
	vector<int> figureIDs;
	figureIDs.push_back(figureID);
	CheckName(Name, figureIDs);
}
void MTReaderHelper::CheckName(CString Name, vector<int> figureIDs)
{
	auto nameResult = AddResultItem(MTResultItem::Name, Name);
	if (Name == _T(""))
	{
		CString errorMessage;
		errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%이름 오류"), _T(" SectionRecognize")), Name));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		nameResult->SetItemState(MTResultItem::Error);
	}

	if (MSCompoM::IsExistedSpacialCharacters(Name))
	{
		CString errorMessage;
		errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1의 이름에 사용할 수 없는 특수 기호가 존재합니다."), _T(" SectionRecognize")), Name));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		AddErrorFigureIDs(figureIDs);
		nameResult->SetItemState(MTResultItem::Error);
	}
}
BOOL MTReaderHelper::IsDiffrenentMainBarSet(CString sMyBar, CString sUrBar)
{
    if (sMyBar == _T("") || sUrBar == _T(""))    return true;

    MSMainBarSet mySet, urSet;
    mySet.SetBarString(sMyBar);
    urSet.SetBarString(sUrBar);
    if (mySet.GetTotalBars() != urSet.GetTotalBars())	return true;

    auto myDias = mySet.GetBarDiaAllLayer();
    auto urDias = urSet.GetBarDiaAllLayer();

    for(int i = 0; i < myDias.size(); i++)
    {
        bool hasDia = false;
        auto myDia = myDias[i];
        for(int j = 0; j < urDias.size(); j++)
        if (urDias[i] == myDia)
        {
            hasDia = true;
            break;
        }
        if (hasDia == false) return true;
    }

    return false;
}
