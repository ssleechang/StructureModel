#include "StdAfx.h"
#include "MTRWallMHelper.h"

#include "GMLib/GMText.h"

#include "SRSectionTable.h"
#include "SRSectionBox.h"

#include "MSRcFaceMemberM.h"
#include "MSRcRWallM.h"

MTRWallMHelper::MTRWallMHelper(MSBaseBuilding* pBldg, MTReader* pReader) : MTReaderHelper(pBldg, pReader)
{
	mp_SectionBox = NULL;
}

MTRWallMHelper::~MTRWallMHelper(void)
{
}

MSCompoM* MTRWallMHelper::MakeData(CString Name, SRSectionBox* pSectionBox)
{
	mp_ResultElement->SetArea(pSectionBox->GetOwnerRect());

	if(MSCompoM::IsExistedSpacialCharacters(Name))
	{
		CString errorMessage;
		errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1의 이름에 사용할 수 없는 특수 기호가 존재합니다."), _T(" SectionRecognize")), Name));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		AddErrorFigureIDs(pSectionBox->GetTextFigureIDs(MTReader::ctName));
		return NULL;
	}

	if (pSectionBox->GetErrorCode() != 0)
	{
		CString errorMessage;
		errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1의 한 레이어에 여러 개의 철근 정보들이 존재합니다."), _T(" SectionRecognize")), Name));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
	}

	mp_CompoM = mp_Bldg->CreateNewCompoM(MSCompoM::msRWallM);
	mp_CompoM->mp_RebarM = MSCompoM::CreateRebarM(MSCompoM::msRWallM);
	mp_SectionBox = pSectionBox;
	mp_CompoM->m_Name = Name;

	MSMaterial* pDefMaterial = mp_Bldg->GetMaterial(mp_Reader->m_nMaterialID);
	if (pDefMaterial == NULL)	pDefMaterial = mp_Bldg->GetDefMaterial(MSMaterial::msConcrete);
	MSMaterial* pMaterial = mp_Bldg->GetMaterial(pSectionBox->m_nMaterialID);
	if (pMaterial != nullptr)	mp_CompoM->SetMaterial(pMaterial);
	else
		mp_CompoM->SetMaterial(pDefMaterial);
	mp_CompoM->SetIsUsedStoryMaterial(mp_Reader->m_IsUsedStoryMaterial);

	SRSectionTable* pTable = pSectionBox->mp_Owner;
	GMText* pStoryText = pTable->GetFirstTextToLayerName(MTReader::ctStory);
	if(pStoryText == NULL)	pStoryText = pSectionBox->mp_Story;

	MakeStoryData(pStoryText);
	MakeThickData();

 	CheckStory();
	if (!IsPlainConcreteReinforceBar())
	{
		Make_ExtVertDatas();
		Make_IntVertDatas();

		Make_ExtHoriDatas();
		Make_IntHoriDatas();

		Make_ShearTopBotDatas();
		Make_ShearStartEndDatas();
	}
	return mp_CompoM;
// 	if(m_IsMaked)	return mp_CompoM;
// 	else
// 	{
// 		delete mp_CompoM;
// 		return NULL;
// 	}
}

bool MTRWallMHelper::MakeStoryData(GMText* pText)
{
	if(pText == NULL)
	{
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, STORY_ERROR, GetCompoMName());
		AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctStory));
		m_IsMaked = false;
		return false;
	}
	CString strStory = pText->m_strText;
	strStory.Replace(_T(" "), _T(""));
	mp_Reader->Parsing_strStory(strStory, mp_CompoM->m_nFromStory, mp_CompoM->m_nToStory);
	return true;
}

bool MTRWallMHelper::MakeThickData()
{
	SRSectionTable* pTable = mp_SectionBox->mp_Owner;
	int thick = 0;
	SRDimension* pThickDimension = pTable->GetFirstDimensionsToLayerName(MTReader::ctThick);
	if(pThickDimension)
	{
		thick = (int)pThickDimension->m_Length;
	}
	else
	{
		GMText* pThickText = pTable->GetFirstTextToLayerName(MTReader::ctThick);
		if(pThickText == NULL)	pThickText = mp_SectionBox->mp_Size;
		if(pThickText == NULL)
		{
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_THICK, GetCompoMName());
			AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctThick));
			m_IsMaked = false;
			return false;
		}

		CString sThick = pThickText->m_strText;
		sThick.MakeUpper();
		sThick.Replace(_T(" "), _T(""));

		thick = MTReader::FindNumberData(0, sThick);
	}

	double thickTolerance = 50;
	if(thick <thickTolerance)
	{
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_THICK, GetCompoMName());
		AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctThick));
		m_IsMaked = false;
		return false;
	}
	((MSRcFaceMemberM*)mp_CompoM)->SetWidth(thick);
	return true;
}

bool MTRWallMHelper::CheckStory()
{
	bool IsOK = true;
	long nFrom = GetFromStory();
	long nTo = GetToStory();
	if(mp_CompoM->m_nFromStory < nFrom || mp_CompoM->m_nToStory > nTo	)
	{
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, STORY_OUT_OF_RANGE, GetCompoMName());
		AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctStory));
		m_IsMaked = false;
		IsOK = false;
	}
	if(mp_CompoM->m_nFromStory==0 || mp_CompoM->m_nToStory==0)
	{
		if(nFrom==0)	nFrom = 1;
		if(nTo==0)	nTo = -1;
		mp_CompoM->m_nFromStory = nFrom;
		mp_CompoM->m_nToStory = nTo;
		CreateErrorObject(GetGroupName(), WARNING_LEVEL, STORY_ERROR, mp_CompoM->m_Name);
		AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctStory));
	}
	return IsOK;
}

long MTRWallMHelper::GetFromStory()
{
	return -(mp_Bldg->GetNumUnderStory()-1);
}

long MTRWallMHelper::GetToStory()
{
	return mp_Bldg->GetNumAboveStory()-1;
}

bool MTRWallMHelper::Make_ExtVertDatas()
{
	SRSectionTable* pTable = mp_SectionBox->mp_Owner;
	MSRcRWallM* pRebarM = (MSRcRWallM*)mp_CompoM->GetRebarM();

	bool IsMainBar = MakeVerticalMainBar(MTReader::ctVExt_MainBar, &pRebarM->m_VOutMidBar);
	bool IsTopBar = MakeVerticalAddedBar(MTReader::ctVExt_TopBar, &pRebarM->m_VOutTopBar, pRebarM->m_nVTopBarLength);
	bool IsBotBar = MakeVerticalAddedBar(MTReader::ctVExt_BotBar, &pRebarM->m_VOutBotBar, pRebarM->m_nVBotBarLength);

	if(!IsMainBar)
	{
		CString sLoc = GetRebarLocation(MTReader::ctVExt_MainBar);
		CString errorMessage;
		errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1 부재의 %2"), _T(" SectionRecognize")), GetCompoMName(), sLoc));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_IGNORE_REBARTEXT, errorMessage);
		AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctVExt_MainBar));
		m_IsMaked = false;
		return false;
	}

	if(!IsTopBar && !IsBotBar)		pRebarM->m_nExtVBarType = 0;
	else if(IsTopBar && IsBotBar)	pRebarM->m_nExtVBarType = 3;
	else if(IsTopBar)							pRebarM->m_nExtVBarType = 1;
	else if(IsBotBar)							pRebarM->m_nExtVBarType = 2;

	return true;
}

bool MTRWallMHelper::Make_IntVertDatas()
{
	SRSectionTable* pTable = mp_SectionBox->mp_Owner;
	MSRcRWallM* pRebarM = (MSRcRWallM*)mp_CompoM->GetRebarM();

	bool IsMainBar = MakeVerticalMainBar(MTReader::ctVInt_MainBar, &pRebarM->m_VInMidBar);
	bool IsTopBar = MakeVerticalAddedBar(MTReader::ctVInt_TopBar, &pRebarM->m_VInTopBar, pRebarM->m_nVInTopBarLength);
	bool IsBotBar = MakeVerticalAddedBar(MTReader::ctVInt_BotBar, &pRebarM->m_VInBotBar, pRebarM->m_nVInBotBarLength);

	if(!IsMainBar)
	{
		CString sLoc = GetRebarLocation(MTReader::ctVInt_MainBar);
		CString errorMessage;
		errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1 부재의 %2"), _T(" SectionRecognize")), GetCompoMName(), sLoc));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_IGNORE_REBARTEXT, errorMessage);
		AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctVInt_MainBar));
		m_IsMaked = false;
		return false;
	}

	if(!IsTopBar && !IsBotBar)		pRebarM->m_nVInBarType = 0;
	else if(IsTopBar && IsBotBar)	pRebarM->m_nVInBarType = 3;
	else if(IsTopBar)							pRebarM->m_nVInBarType = 1;
	else if(IsBotBar)							pRebarM->m_nVInBarType = 2;

	return true;
}

bool MTRWallMHelper::Make_ExtHoriDatas()
{
	SRSectionTable* pTable = mp_SectionBox->mp_Owner;
	MSRcRWallM* pRebarM = (MSRcRWallM*)mp_CompoM->GetRebarM();

	bool IsTopBar = MakeHorizontalBar(MTReader::ctHExt_TopBar, &pRebarM->m_HOutTopBar);
	bool IsMidBar = MakeHorizontalBar(MTReader::ctHExt_MidBar, &pRebarM->m_HOutMidBar);
	bool IsBotBar = MakeHorizontalBar(MTReader::ctHExt_BotBar, &pRebarM->m_HOutBotBar);

	if (IsTopBar)
	{
		if(IsMidBar == false)
			pRebarM->m_HOutMidBar.CopyFromMe(&pRebarM->m_HOutTopBar);
		if (IsBotBar == false)
			pRebarM->m_HOutBotBar.CopyFromMe(&pRebarM->m_HOutTopBar);
		return true;
	}

	CString sLoc = GetRebarLocation(MTReader::ctHExt_TopBar);
	CString errorMessage;
	errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1 부재의 %2"), _T(" SectionRecognize")), GetCompoMName(), sLoc));
	CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_IGNORE_REBARTEXT, errorMessage);
	AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctHExt_TopBar));
	AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctHExt_MidBar));
	AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctHExt_BotBar));
	m_IsMaked = false;
	return false;
}

bool MTRWallMHelper::Make_IntHoriDatas()
{
	SRSectionTable* pTable = mp_SectionBox->mp_Owner;
	MSRcRWallM* pRebarM = (MSRcRWallM*)mp_CompoM->GetRebarM();

	bool IsTopBar = MakeHorizontalBar(MTReader::ctHInt_TopBar, &pRebarM->m_HInTopBar);
	bool IsMidBar = MakeHorizontalBar(MTReader::ctHInt_MidBar, &pRebarM->m_HInMidBar);
	bool IsBotBar = MakeHorizontalBar(MTReader::ctHInt_BotBar, &pRebarM->m_HInBotBar);

	if (IsTopBar)
	{
		if (IsMidBar == false)
			pRebarM->m_HInMidBar.CopyFromMe(&pRebarM->m_HInTopBar);
		if (IsBotBar == false)
			pRebarM->m_HInBotBar.CopyFromMe(&pRebarM->m_HInTopBar);
		return true;
	}
		
	CString sLoc = GetRebarLocation(MTReader::ctHInt_TopBar);
	CString errorMessage;
	errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1 부재의 %2"), _T(" SectionRecognize")), GetCompoMName(), sLoc));
	CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_IGNORE_REBARTEXT, errorMessage);
	AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctHInt_TopBar));
	AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctHInt_MidBar));
	AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctHInt_BotBar));
	m_IsMaked = false;
	return false;
}

bool MTRWallMHelper::Make_ShearTopBotDatas()
{
	SRSectionTable* pTable = mp_SectionBox->mp_Owner;
	MSRcRWallM* pRebarM = (MSRcRWallM*)mp_CompoM->GetRebarM();

	long shearBarLength = 0;
	MakeShearBars(MTReader::ctShear_TopBar, &pRebarM->m_HTopStirrup, &pRebarM->m_VTopStirrup, shearBarLength);
	pRebarM->m_dTopSRegion = shearBarLength;

	MakeShearBars(MTReader::ctShear_BotBar, &pRebarM->m_HBotStirrup, &pRebarM->m_VBotStirrup, shearBarLength);
	pRebarM->m_dBotSRegion = shearBarLength;

	return true;
}

bool MTRWallMHelper::Make_ShearStartEndDatas()
{
	SRSectionTable* pTable = mp_SectionBox->mp_Owner;
	MSRcRWallM* pRebarM = (MSRcRWallM*)mp_CompoM->GetRebarM();

	long shearBarLength = 0;
	MakeShearBars(MTReader::ctShear_StartBar, &pRebarM->m_StartShearHBars, &pRebarM->m_StartShearVBars, pRebarM->m_nStartShearAddBarLength);

	MakeShearBars(MTReader::ctShear_EndBar, &pRebarM->m_EndShearHBars, &pRebarM->m_EndShearVBars, pRebarM->m_nEndShearAddBarLength);

	return true;
}

bool MTRWallMHelper::MakeVerticalMainBar(MTReader::CellType cellType, MSStirrupBar* pStirrupBar)
{
	SRSectionTable* pTable = mp_SectionBox->mp_Owner;
	if(IsDuplicatedRebarInfo(cellType))
	{
		m_IsMaked = false;
		return false;
	}

	vector<GMText*> textArr = pTable->GetTextsToLayerName(cellType);
	GMText* pText = FindRebarText(textArr, cellType);
	if(pText)
	{
		MakeStirrupBar(pText->m_strText, pStirrupBar);
		return true;
	}

	m_IsMaked = false;
	return false;
}

bool MTRWallMHelper::MakeVerticalAddedBar(MTReader::CellType cellType, MSStirrupBar* pStirrupBar, long& stirrupBarLength)
{
	SRSectionTable* pTable = mp_SectionBox->mp_Owner;
	if(IsDuplicatedRebarInfo(cellType))	return false;

	vector<GMText*> textArr = pTable->GetTextsToLayerName(cellType);
	GMText* pText = FindRebarText(textArr, cellType);
	if(pText)
	{
		MakeAddedStirrupBar(cellType, pText->m_strText, pStirrupBar, stirrupBarLength);
		return true;
	}
	return false;
}

void MTRWallMHelper::MakeStirrupBar(CString rebarText, MSStirrupBar* pStirrupBar)
{
	rebarText.MakeUpper();
	rebarText.Replace(_T(" "), _T(""));
	int indexD = rebarText.Find(_T("D"));
	int indexAt = rebarText.Find(_T("@"));
	int indexPlus = rebarText.Find(_T("+"));

	if(indexD == -1 || indexAt == -1)	return;

	CString sFirstDiaName = MSStirrupBar::getFirstDiaName2StrpBar(rebarText);
	if(MSDeformedBar::FindBarDiameterIndexByName(sFirstDiaName) == -1)			return ;

	int spacing = 0;
	int indexParentheses = rebarText.Find(_T("("));
	if(indexParentheses == -1)
		spacing = _ttoi(rebarText.Mid(indexAt+1, rebarText.GetLength() - indexAt));
	else
		spacing = _ttoi(rebarText.Mid(indexAt+1, indexParentheses-(indexAt+1)));

	int spacingTolerance = 50;
	if(spacing < spacingTolerance)
		return ;

	CString strData;
	strData.Format(_T("%s%s@%ld"),MSDeformedBar::m_sDefaultBarType, sFirstDiaName, spacing);
	pStirrupBar->InitData(strData);

	if(indexPlus != -1)
	{
		CString sSecondDiaName = MSStirrupBar::getSecondDiaName2StrpBar(rebarText);
		if (MSDeformedBar::FindBarDiameterIndexByName(sSecondDiaName) == -1)
		{
			return;
		}
		strData.Format(_T("%s%s@%ld"), MSDeformedBar::m_sDefaultBarType, sSecondDiaName, spacing);
		pStirrupBar->Add(strData);
	}
}

void MTRWallMHelper::MakeAddedStirrupBar(MTReader::CellType cellType, CString rebarText, MSStirrupBar* pStirrupBar, long& stirrupBarLength)
{
	SRSectionTable* pTable = mp_SectionBox->mp_Owner;
	MakeStirrupBar(rebarText, pStirrupBar);

	vector<SRDimension*> dimArr = pTable->GetDimensionsToLayerName(cellType);
	if(dimArr.size() == 1)
	{
		SRDimension* pDimension = dimArr[0];
		stirrupBarLength = (long)pDimension->m_Length;
	}
	else
		stirrupBarLength = Parsing_StirrupBarLength(rebarText);
}

bool MTRWallMHelper::IsDuplicatedRebarInfo(MTReader::CellType cellType)
{
	SRSectionTable* pTable = mp_SectionBox->mp_Owner;
	vector<GMText*> textArr = pTable->GetTextsToLayerName(cellType);
	int numText = textArr.size();
	int rebarTextCount = 0;
	for(int iText = 0; iText < numText; iText++)
	{
		GMText* pText = textArr[iText];
		if(IsRebarText(pText->m_strText))	rebarTextCount++;
	}
	if(rebarTextCount > 1)
	{
		CString errorMessage;
		CString sLoc = GetRebarLocation(cellType);

		errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1 부재의 %2 정보가 중복됩니다."), _T(" SectionRecognize")), GetCompoMName(), sLoc));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_DUPLICATED_DATA, errorMessage);
		for(int iText = 0; iText < numText; iText++)
		{
			GMText* pText = textArr[iText];
			AddErrorFigureID(mp_Reader->FindTextFigureID(pText));
		}
		return true;
	}
	return false;
}

GMText* MTRWallMHelper::FindRebarText(vector<GMText*>& textArr, MTReader::CellType cellType)
{
	bool IsOnlyMainBar = true;
	GMText* pMainText = NULL;
	int numText = textArr.size();
	for(int iText = 0; iText < numText; iText++)
	{
		GMText* pText = textArr[iText];
		if(IsRebarText(pText->m_strText))	return pText;
	}

// 	CString errorMessage;
// 	CString sLoc = GetRebarLocation(cellType);
// 
// 	errorMessage.Format(_T("%s 부재의 %s 정보가 없습니다."), GetCompoMName(), sLoc);
// 	CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_DUPLICATED_DATA, errorMessage, mp_SectionBox->GetOwnerRect());
	return NULL;
}

bool MTRWallMHelper::IsRebarText(CString rebarText)
{
	// sample : HD13+16@200(L:800),	HD13+16X200X300,	HD13@200X300, ...

	rebarText.MakeUpper();
	rebarText.Replace(_T(" "), _T(""));
	int indexD = rebarText.Find(_T("D"));
	int indexAt = rebarText.Find(_T("@"));
	int indexPlus = rebarText.Find(_T("+"));
	int indexX = rebarText.Find(_T("X"));

	if(indexD == -1 || (indexAt == -1 && indexX == -1))	return false;

	CString sDiaName = MSStirrupBar::getFirstDiaName2StrpBar(rebarText);
	if(MSDeformedBar::FindBarDiameterIndexByName(sDiaName) == -1)			return false;

	if(indexPlus != -1)
	{
		sDiaName = MSStirrupBar::getSecondDiaName2StrpBar(rebarText);
		if(MSDeformedBar::FindBarDiameterIndexByName(sDiaName) == -1)			return false;
	}

	long spacing = 0;
	if(indexAt != -1)			spacing = MTReader::FindNumberData(indexAt, rebarText);
	else if(indexX != -1)	spacing = MTReader::FindNumberData(indexX, rebarText);

	 int spacingTolerance = 50;
	 if(spacing < spacingTolerance)
		 return false;

	 return true;
}

bool MTRWallMHelper::MakeHorizontalBar(MTReader::CellType cellType, MSStirrupBar* pStirrupBar)
{
	SRSectionTable* pTable = mp_SectionBox->mp_Owner;
	if(IsDuplicatedRebarInfo(cellType))
	{
		m_IsMaked = false;
		return false;
	}

	vector<GMText*> textArr = pTable->GetTextsToLayerName(cellType);
	GMText* pText = FindRebarText(textArr, cellType);
	if(pText)
	{
		MakeStirrupBar(pText->m_strText, pStirrupBar);
		return true;
	}

	return false;
}

bool MTRWallMHelper::MakeShearBars(MTReader::CellType cellType, MSStirrupBar* pHStirrupBar, MSStirrupBar* pVStirrupBar, long& barLength)
{
	SRSectionTable* pTable = mp_SectionBox->mp_Owner;
	if(IsDuplicatedRebarInfo(cellType))
	{
		m_IsMaked = false;
		return false;
	}

	vector<GMText*> textArr = pTable->GetTextsToLayerName(cellType);
	GMText* pText = FindRebarText(textArr, cellType);
	if(pText)
	{
		Parsing_ShearBars(cellType, pText->m_strText, pHStirrupBar, pVStirrupBar, barLength);
		return true;
	}

	return false;
}

bool MTRWallMHelper::Parsing_ShearBars(MTReader::CellType cellType, CString rebarText, MSStirrupBar* pHStirrupBar, MSStirrupBar* pVStirrupBar, long& barLength)
{
	SRSectionTable* pTable = mp_SectionBox->mp_Owner;
	vector<CString> Parsers = GetShearBarParsers(rebarText);
	if(Parsers.size() != 2)	return false;

	int indexD = rebarText.Find(_T("D"));
	int indexFirstParser = rebarText.Find(Parsers[0]);
	int indexSecondParser = rebarText.Find(Parsers[1]);
	
	CString sDiaName = rebarText.Mid(indexD+1, indexFirstParser-(indexD+1));
	if(MSDeformedBar::FindBarDiameterIndexByName(sDiaName) == -1)	return false;

	long horizotalSpacing = MTReader::FindNumberData(indexFirstParser, rebarText);
	long verticalSpacing = MTReader::FindNumberData(indexSecondParser, rebarText);

	vector<SRDimension*> dimArr = pTable->GetDimensionsToLayerName(cellType);
	if(dimArr.size() == 1)
	{
		SRDimension* pDimension = dimArr[0];
		barLength = (long)pDimension->m_Length;
	}
	else
	{
		int indexMinus = rebarText.Find(_T("-"));
		int indexColon = rebarText.Find(_T(":"));
		if(indexColon != -1)
			barLength = MTReader::FindNumberData(indexColon+1, rebarText);
		if(indexMinus != -1)
			barLength = MTReader::FindNumberData(indexMinus+1, rebarText);
	}

	CString sShearBar;	sShearBar.Format(_T("%s%s@%ld"), MSDeformedBar::m_sDefaultBarType, sDiaName, horizotalSpacing);
	pHStirrupBar->InitData(sShearBar);
	sShearBar.Format(_T("%s%s@%ld"), MSDeformedBar::m_sDefaultBarType, sDiaName, verticalSpacing);
	pVStirrupBar->InitData(sShearBar);

	MSRcRWallM* pRebarM = (MSRcRWallM*)mp_CompoM->GetRebarM();
	if(cellType == MTReader::ctShear_TopBar)			pRebarM->m_bUseTopStirrup = TRUE;
	else if(cellType == MTReader::ctShear_BotBar)		pRebarM->m_bUseBotStirrup = TRUE;
	else if(cellType == MTReader::ctShear_StartBar)	pRebarM->m_bStartShearAddBar = TRUE;
	else if(cellType == MTReader::ctShear_EndBar)	pRebarM->m_bEndShearAddBar = TRUE;
	return true;
}

void MTRWallMHelper::CreateErrorObjectByShearBar(MTReader::CellType cellType)
{
	CString sLoc = GetRebarLocation(cellType);
	CString errorMessage;
	errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1 부재의 %2 정보가 올바르지 않습니다."), _T(" SectionRecognize")), GetCompoMName(), sLoc));
	CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_REBAR, errorMessage);
	AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(cellType));
}

CString MTRWallMHelper::GetRebarLocation(MTReader::CellType cellType)
{
	CString sLoc = _T("");
	switch(cellType)
	{
	case MTReader::ctVExt_MainBar:	return _T("Exterior Ver. Main");
	case MTReader::ctVExt_TopBar:	return _T("Exterior Ver. Top");
	case MTReader::ctVExt_BotBar:		return _T("Exterior Ver. Bot.");
	case MTReader::ctVInt_MainBar:	return _T("Interior Ver. Main");
	case MTReader::ctVInt_TopBar:		return _T("Interior Ver. Top");
	case MTReader::ctVInt_BotBar:		return _T("Interior Ver. Bot.");
	case MTReader::ctHExt_TopBar:	return _T("Exterior Hor. Top");
	case MTReader::ctHExt_MidBar:	return _T("Exterior Hor. Cen.");
	case MTReader::ctHExt_BotBar:		return _T("Exterior Hor. Bot.");
	case MTReader::ctHInt_TopBar:		return _T("Interior Hor. Top");
	case MTReader::ctHInt_MidBar:	return _T("Interior Hor. Cen.");
	case MTReader::ctHInt_BotBar:		return _T("Interior Hor. Bot.");
	case MTReader::ctShear_TopBar:	return _T("Top Shear Bar");
	case MTReader::ctShear_BotBar:	return _T("Bot. Shear Bar");
	case MTReader::ctShear_StartBar:	return _T("Start Shear Bar");
	case MTReader::ctShear_EndBar:	return _T("End Shear Bar");
	}
	return sLoc;
}

long MTRWallMHelper::Parsing_StirrupBarLength(CString rebarText)
{
	rebarText.MakeUpper();
	rebarText.Replace(_T(" "), _T(""));
	int indexMinus = rebarText.Find(_T("-"));
	int indexColon = rebarText.Find(_T(":"));

	if(indexColon != -1)
		return MTReader::FindNumberData(indexColon+1, rebarText);
	if(indexMinus != -1)
		return MTReader::FindNumberData(indexMinus+1, rebarText);

	return 0;
}

vector<CString> MTRWallMHelper::GetShearBarParsers(CString rebarText)
{
	vector<CString> Parsers;
	CString parserAt = _T("@");
	CString parserX = _T("X");

	for(int i =0; i < rebarText.GetLength(); i++)
	{
		if(rebarText.GetAt(i) == parserAt)		Parsers.push_back(parserAt);
		else if(rebarText.GetAt(i) == parserX)	Parsers.push_back(parserX);
	}
	return Parsers;
}