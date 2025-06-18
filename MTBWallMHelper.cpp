#include "stdafx.h"
#include "MTBWallMHelper.h"

#include "SRSectionTable.h"
#include "SRSectionBox.h"

#include "MSRcFaceMemberM.h"
#include "MSRcBWallM.h"

MTBWallMHelper::MTBWallMHelper(MSBaseBuilding* pBldg, MTReader* pReader) : MTReaderHelper(pBldg, pReader)
{
	mp_SectionBox = NULL;
	mp_WallInfo = NULL;
}


MTBWallMHelper::~MTBWallMHelper()
{
}

long MTBWallMHelper::GetFromStory()
{
	return -(mp_Bldg->GetNumUnderStory() - 1);
}

long MTBWallMHelper::GetToStory()
{
	return mp_Bldg->GetNumAboveStory() - 1;
}

MSCompoM* MTBWallMHelper::MakeData(CString Name, SRSectionBox* pSectionBox, SRWallInfo* pWallInfo)
{
	mp_ResultElement->SetArea(pSectionBox->GetOwnerRect());
	mp_SectionBox = pSectionBox;
	mp_WallInfo = pWallInfo;

	CheckName(Name, mp_WallInfo->GetTextFigureIDs(MTReader::ctName));

	mp_CompoM = mp_Bldg->CreateNewCompoM(MSCompoM::msBWallM);
	mp_CompoM->mp_RebarM = MSCompoM::CreateRebarM(MSCompoM::msBWallM);
	mp_CompoM->m_Name = Name;

	MSMaterial* pDefMaterial = mp_Bldg->GetMaterial(mp_Reader->m_nMaterialID);
	if (pDefMaterial == NULL)	pDefMaterial = mp_Bldg->GetDefMaterial(MSMaterial::msConcrete);
	MSMaterial* pMaterial = mp_Bldg->GetMaterial(pWallInfo->GetMaterialID());
	if (pMaterial != nullptr)	mp_CompoM->SetMaterial(pMaterial);
	else
		mp_CompoM->SetMaterial(pDefMaterial);

	mp_CompoM->SetIsUsedStoryMaterial(mp_Reader->m_IsUsedStoryMaterial);

	MakeWallData();

	return mp_CompoM;
// 	if (m_IsMaked)		return mp_CompoM;
// 	else
// 	{
// 		delete mp_CompoM;
// 		return NULL;
// 	}
}

void MTBWallMHelper::MakeWallData()
{
	CString name = mp_CompoM->GetName();
	long nFrom = GetFromStory();
	long nTo = GetToStory();

	CString fromStory, toStory;
	fromStory = mp_Bldg->GetFloorName(mp_WallInfo->GetFromStory());
	toStory = mp_Bldg->GetFloorName(mp_WallInfo->GetToStory());
	if (fromStory == _T(""))	fromStory = _T("None");
	if (toStory == _T(""))	toStory = _T("None");

	auto fromStoryResult = AddResultItem(MTResultItem::FromStory, fromStory);
	auto toStoryResult = AddResultItem(MTResultItem::ToStory, toStory);

	if (mp_WallInfo->GetFromStory() < nFrom)
	{
		CString errorMessage;
		errorMessage.Format(_T("%s(%ld~%ld)"), name, mp_WallInfo->GetFromStory(), mp_WallInfo->GetToStory());
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, STORY_OUT_OF_RANGE, errorMessage);
		AddErrorFigureIDs(mp_WallInfo->GetTextFigureIDs(MTReader::ctStory));
		fromStoryResult->SetItemState(MTResultItem::Error);
	}
	if (fromStory == _T("None"))
	{
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, STORY_OUT_OF_RANGE, GetCompoMName());
		AddErrorFigureIDs(mp_WallInfo->GetTextFigureIDs(MTReader::ctStory));
		fromStoryResult->SetItemState(MTResultItem::Error);
	}
	if (mp_WallInfo->GetToStory() > nTo)
	{
		CString errorMessage;
		errorMessage.Format(_T("%s(%ld~%ld)"), name, mp_WallInfo->GetFromStory(), mp_WallInfo->GetToStory());
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, STORY_OUT_OF_RANGE, errorMessage);
		AddErrorFigureIDs(mp_WallInfo->GetTextFigureIDs(MTReader::ctStory));
		toStoryResult->SetItemState(MTResultItem::Error);
	}
	if (toStory == _T("None"))
	{
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, STORY_OUT_OF_RANGE, GetCompoMName());
		AddErrorFigureIDs(mp_WallInfo->GetTextFigureIDs(MTReader::ctStory));
		toStoryResult->SetItemState(MTResultItem::Error);
	}

	CString thick;
	thick.Format(_T("%ld"), ROUND(mp_WallInfo->GetThick()));
	auto thickResult = AddResultItem(MTResultItem::Thick, thick);
	if (mp_WallInfo->GetThick() <= 0)
	{
		CString errorMessage;
		errorMessage.Format(_T("%s(%ld~%ld)"), name, mp_WallInfo->GetFromStory(), mp_WallInfo->GetToStory());
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_THICK, errorMessage);
		AddErrorFigureIDs(mp_WallInfo->GetTextFigureIDs(MTReader::ctThick));
		thickResult->SetItemState(MTResultItem::Error);
	}

	mp_CompoM->m_nFromStory = mp_WallInfo->GetFromStory();
	mp_CompoM->m_nToStory = mp_WallInfo->GetToStory();
	((MSRcFaceMemberM*)mp_CompoM)->SetThick(mp_WallInfo->GetThick());

	if (IsPlainConcreteReinforceBar())	return;
	MakeVBars(mp_WallInfo->GetVBar());
	MakeHBars(mp_WallInfo->GetHBar());
	MakeShearRatio(mp_WallInfo->GetShearRatio());
}

void MTBWallMHelper::MakeVBars(CString rebarText)
{
	MSRcBWallM* pRebarM = (MSRcBWallM*)mp_CompoM->GetRebarM();
	auto vBarResult = AddResultItem(MTResultItem::VBars, rebarText);
	if (rebarText.Find('-') != -1)
	{
		pRebarM->m_bIsNumPlaceVBar = true;
		MakeMainBars(rebarText, &pRebarM->m_MainVBar, MTResultItem::VBars);
	}
	else
	{
		MakeStirrupBars(rebarText, &pRebarM->m_VBars, MTReader::ctVBar, MTResultItem::VBars);
		pRebarM->m_bIsNumPlaceVBar = false;
	}
}

void MTBWallMHelper::MakeHBars(CString rebarText)
{
	MSRcBWallM* pRebarM = (MSRcBWallM*)mp_CompoM->GetRebarM();
	auto vBarResult = AddResultItem(MTResultItem::HBars, rebarText);
	if (rebarText.Find('-') != -1)
	{
		pRebarM->m_bIsNumPlaceHBar = true;
		MakeWallHBars(rebarText, &pRebarM->m_MainHBar, MTResultItem::HBars);
	}
	else
	{
		MakeStirrupBars(rebarText, &pRebarM->m_HBars, MTReader::ctHBar, MTResultItem::HBars);
		pRebarM->m_bIsNumPlaceHBar = false;
	}
}

void MTBWallMHelper::MakeShearRatio(CString shearText)
{
	auto shearResult = AddResultItem(MTResultItem::ShearBars, shearText);
	if (shearText == "")	return;
	int nTokenPos = 0;
	CString parser = _T(",");
	vector<CString> dataArr;
	CString strToken = shearText.Tokenize(parser, nTokenPos);

	dataArr.push_back(strToken);
	while (!strToken.IsEmpty())
	{
		strToken = shearText.Tokenize(parser, nTokenPos);
		if (!strToken.IsEmpty())
			dataArr.push_back(strToken);
	}

	if (dataArr.size() != 5)
	{
		shearResult->SetItemState(MTResultItem::Error);
		return;
	}
	CString dia = dataArr[0];
	int hSpacing = _ttol(dataArr[1]);
	int hRate = _ttol(dataArr[2]);
	int vSpacing = _ttol(dataArr[3]);
	int vRate = _ttol(dataArr[4]);

	if (hSpacing <= 0 || vSpacing <= 0 || hRate <= 0 || vRate <= 0)
	{
		shearResult->SetItemState(MTResultItem::Error);
		return;
	}
	
	if (!CheckDia(_ttof(dia)))
	{
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_REBAR, GetCompoMName());
		AddErrorFigureIDs(mp_WallInfo->GetTextFigureIDs(MTReader::ctShearRatio));
		shearResult->SetItemState(MTResultItem::Error);
		return;
	}

	MSRcBWallM* pRebarM = (MSRcBWallM*)mp_CompoM->GetRebarM();
	pRebarM->SetShearBarPlaceType(1);
	pRebarM->SetShearBarRateDiaIndex(MSDeformedBar::FindBarDiameterIndexByName(dia));
	pRebarM->SetHorShearBarRateSpacing(hSpacing);
	pRebarM->SetHorShearBarRate(hRate);
	pRebarM->SetVerShearBarRateSpacing(vSpacing);
	pRebarM->SetVerShearBarRate(vRate);

}
void MTBWallMHelper::MakeWallHBars(CString rebarText, MSMainBar* pMainBar, MTResultItem::eItemType itemType)
{
	auto barResult = GetResultItem(itemType);
	CStringArray strArr;
	CString sRebarText = rebarText.MakeUpper();
	sRebarText.Replace(_T(" "), _T(""));

	CString parser = _T("+");
	int nTokenPos = 0;
	CString strToken = rebarText.Tokenize(parser, nTokenPos);

	strArr.Add(strToken);

	while (!strToken.IsEmpty())
	{
		strToken = rebarText.Tokenize(parser, nTokenPos);
		if (!strToken.IsEmpty())
			strArr.Add(strToken);
	}

	for (int i = 0; i < strArr.GetSize(); i++)
	{
		CString theBar = strArr[i];
		long nLength = theBar.GetLength();
		long nIndex = theBar.Find('D');
		if (nIndex == -1)
		{
			CString errorMessage;
			errorMessage.Format(_T("%s(%ld~%ld) %s %s"), mp_CompoM->m_Name, mp_CompoM->m_nFromStory, mp_CompoM->m_nToStory, LANG2(VerticalBars, SectionRecognize), LANG2(sHaveErrors, SectionRecognize));
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
			AddErrorFigureIDs(mp_WallInfo->GetTextFigureIDs(MTReader::ctVBar));
			barResult->SetItemState(MTResultItem::Error);
			return;
		}
		CString sDiaName = theBar.Mid(nIndex + 1, nLength - (nIndex + 1));
		if (!CheckDia(_ttof(sDiaName)))
		{
			CString errorMessage;
			errorMessage.Format(_T("%s %s(%s)"), mp_CompoM->m_Name, LANG2(VerticalBars, SectionRecognize), sDiaName);
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_IGNORE_REBARTEXT, errorMessage);
			AddErrorFigureIDs(mp_WallInfo->GetTextFigureIDs(MTReader::ctVBar));
			barResult->SetItemState(MTResultItem::Error);
			return;
		}
	}

	for (int i = 0; i < strArr.GetSize(); i++)
	{
		if (i == 0)
			pMainBar->InitData(strArr[i]);
		else
			pMainBar->Add(strArr[i]);
	}
	barResult->SetItemText(pMainBar->GetTotalMainBarString());
}

void MTBWallMHelper::MakeMainBars(CString rebarText, MSMainBar* pMainBar, MTResultItem::eItemType itemType)
{
	auto barResult = GetResultItem(itemType);

	CString sRebarText = rebarText.MakeUpper();
	sRebarText.Replace(_T(" "), _T(""));

	CString parser = _T("+");
	int nTokenPos = 0;
	CString strToken = rebarText.Tokenize(parser, nTokenPos);

	pMainBar->InitData(strToken);

	while (!strToken.IsEmpty())
	{
		strToken = rebarText.Tokenize(parser, nTokenPos);
		if (!strToken.IsEmpty())
			pMainBar->Add(strToken);
	}

	long nLength = sRebarText.GetLength();
	long nIndex = sRebarText.Find('D');
	if (nIndex == -1)
	{
		CString errorMessage;
		errorMessage.Format(_T("%s(%ld~%ld) %s %s"), mp_CompoM->m_Name, mp_CompoM->m_nFromStory, mp_CompoM->m_nToStory, LocalDic::GetLocal(_T("수직근"), _T("SectionRecognize")), LocalDic::GetLocal(_T("정보에 오류가 있습니다."), _T("SectionRecognize")));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		AddErrorFigureIDs(mp_WallInfo->GetTextFigureIDs(MTReader::ctVBar));
		
		barResult->SetItemState(MTResultItem::Error);
		barResult->SetItemText(_T(""));
		return;
	}
	CString sDiaName = sRebarText.Mid(nIndex + 1, nLength - (nIndex + 1));
	if (!CheckDia(_ttof(sDiaName)))
	{
		CString errorMessage;
		errorMessage.Format(_T("%s %s(%s)"), mp_CompoM->m_Name, LocalDic::GetLocal(_T("수직근"), _T("SectionRecognize")),sDiaName);
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_IGNORE_REBARTEXT, errorMessage);
		AddErrorFigureIDs(mp_WallInfo->GetTextFigureIDs(MTReader::ctVBar));
		barResult->SetItemState(MTResultItem::Error);
	}

	pMainBar->InitData(rebarText);
	barResult->SetItemText(pMainBar->GetTotalMainBarString());
}

void MTBWallMHelper::MakeStirrupBars(CString rebarText, MSStirrupBar* pStirrupBar, int cellType, MTResultItem::eItemType itemType)
{
	auto barResult = GetResultItem(itemType);

	CString sLoc = LocalDic::GetLocal(_T("수평근"), _T("SectionRecognize"));
	if (cellType == MTReader::ctVBar)	sLoc = LocalDic::GetLocal(_T("수직근"), _T("SectionRecognize"));
	if (rebarText == _T(""))
	{
		CString errorMessage;
		errorMessage.Format(_T("%s(%ld~%ld) %s %s"), mp_CompoM->m_Name, mp_CompoM->m_nFromStory, mp_CompoM->m_nToStory, sLoc, LocalDic::GetLocal(_T("정보에 오류가 있습니다."), _T("SectionRecognize")));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		AddErrorFigureIDs(mp_WallInfo->GetTextFigureIDs(cellType));
		barResult->SetItemState(MTResultItem::Error);
		return;
	}

	rebarText.MakeUpper();
	rebarText.Replace(_T(" "), _T(""));
	int indexD = rebarText.Find(_T("D"));
	int indexAt = rebarText.Find(_T("@"));
	int indexPlus = rebarText.Find(_T("+"));

	if (indexD == -1 || indexAt == -1)
	{
		CString errorMessage;
		errorMessage.Format(_T("%s(%ld~%ld) %s %s"), mp_CompoM->m_Name, mp_CompoM->m_nFromStory, mp_CompoM->m_nToStory, sLoc, LocalDic::GetLocal(_T("정보에 오류가 있습니다."), _T("SectionRecognize")));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		AddErrorFigureIDs(mp_WallInfo->GetTextFigureIDs(cellType));
		barResult->SetItemState(MTResultItem::Error);
		return;
	}

	CString sFirstDiaName = MSStirrupBar::getFirstDiaName2StrpBar(rebarText);
	if (MSDeformedBar::FindBarDiameterIndexByName(sFirstDiaName) == -1)
	{
		CString errorMessage;
		errorMessage.Format(_T("%s(%ld~%ld) %s %s"), mp_CompoM->m_Name, mp_CompoM->m_nFromStory, mp_CompoM->m_nToStory, sLoc, LocalDic::GetLocal(_T("정보에 오류가 있습니다."), _T("SectionRecognize")));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_IGNORE_REBARTEXT, errorMessage);
		AddErrorFigureIDs(mp_WallInfo->GetTextFigureIDs(cellType));
		barResult->SetItemState(MTResultItem::Error);
		return;
	}

	int spacing = 0;
	int indexParentheses = rebarText.Find(_T("("));
	if (indexParentheses == -1)
		spacing = _ttoi(rebarText.Mid(indexAt + 1, rebarText.GetLength() - indexAt));
	else
		spacing = _ttoi(rebarText.Mid(indexAt + 1, indexParentheses - (indexAt + 1)));

	CString strData;
	strData.Format(_T("%s%s@%ld"), MSDeformedBar::m_sDefaultBarType, sFirstDiaName, spacing);
	pStirrupBar->InitData(strData);

	if (indexPlus != -1)
	{
		CString sSecondDiaName = MSStirrupBar::getSecondDiaName2StrpBar(rebarText);
		if (MSDeformedBar::FindBarDiameterIndexByName(sSecondDiaName) == -1)
		{
			CString errorMessage;
			errorMessage.Format(_T("%s(%ld~%ld) %s %s"), mp_CompoM->m_Name, mp_CompoM->m_nFromStory, mp_CompoM->m_nToStory, sLoc, LocalDic::GetLocal(_T("정보에 오류가 있습니다."), _T("SectionRecognize")));
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_IGNORE_REBARTEXT, errorMessage);
			AddErrorFigureIDs(mp_WallInfo->GetTextFigureIDs(cellType));
			barResult->SetItemState(MTResultItem::Error);
			return;
		}
		strData.Format(_T("%s%s@%ld"), MSDeformedBar::m_sDefaultBarType, sSecondDiaName, spacing);
		pStirrupBar->Add(strData);
	}
	barResult->SetItemText(pStirrupBar->GetStirrupBarString());
}