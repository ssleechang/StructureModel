#include "stdafx.h"

#include "StructureModelEnum.h"
#include "MTRcSlabMHelper.h"
#include "SRSectionTable.h"
#include "SRSectionBox.h"

#include "MSRcFaceMemberM.h"
#include "MSRcSlabM.h"

MTRcSlabMHelper::MTRcSlabMHelper(MSBaseBuilding* pBldg, MTReader* pReader) : MTReaderHelper(pBldg, pReader)
{
	mp_Table = NULL;
}


MTRcSlabMHelper::~MTRcSlabMHelper()
{
}

long MTRcSlabMHelper::GetFromStory()
{
	return -(mp_Bldg->GetNumUnderStory());
}

long MTRcSlabMHelper::GetToStory()
{
	return mp_Bldg->GetNumAboveStory() - 1;
}

MSCompoM* MTRcSlabMHelper::MakeData(CString Name, SRSectionTable* pTable)
{
	mp_ResultElement->SetArea(pTable->mp_Rect);

	auto nameResult = AddResultItem(MTResultItem::Name, Name);
	CheckName(Name, mp_Reader->FindTextFigureID(pTable->mp_Name));

	mp_CompoM = mp_Bldg->CreateNewCompoM(MSCompoM::msSlabM);
	mp_CompoM->mp_RebarM = MSCompoM::CreateRebarM(MSCompoM::msSlabM);

	MSRcSlabM* pRebarM = (MSRcSlabM*)mp_CompoM->GetRebarM();
	mp_Table = pTable;

	MSMaterial* pDefMaterial = mp_Bldg->GetMaterial(mp_Reader->m_nMaterialID);
	if (pDefMaterial == NULL)	pDefMaterial = mp_Bldg->GetDefMaterial(MSMaterial::msConcrete);

	mp_CompoM->m_Name = Name;
	mp_CompoM->m_nFromStory = GetFromStory();
	mp_CompoM->m_nToStory = GetToStory();
	mp_CompoM->SetMaterial(pDefMaterial);
	mp_CompoM->SetIsUsedStoryMaterial(mp_Reader->m_IsUsedStoryMaterial);

	SRSectionBox* pSectionBox = pTable->ma_Box[0];
	SRSlabInfo* pSlabInfo = pSectionBox->mp_SlabInfo;
	
	CString sThick;
	sThick.Format(_T("%ld"), pSlabInfo->m_nThick);
	auto thickResult = AddResultItem(MTResultItem::Thick, sThick);
	((MSRcFaceMemberM*)mp_CompoM)->SetThick(pSlabInfo->m_nThick);

	if (pSlabInfo->GetIsFlatSlabType())
	{
		mp_ResultElement->SetIsFlatSlab(true);
	}

	bool IsBent = false;
	if (mp_Reader->m_SlabX3Y3Type == SLAB_X3Y3_TYPE_BENT)
		IsBent = true;
	pSlabInfo->GetStripType(true, IsBent);
	pSlabInfo->GetStripType(false, IsBent);

	if (!IsPlainConcreteReinforceBar())
	{
		MakeRebarTextDataArr();

		MakeSlabRebars(true, pSlabInfo);
		MakeSlabRebars(false, pSlabInfo);
		//	((MSRcSlabM*)mp_CompoM->mp_RebarM)->CheckShortnLongBarFlag();
	}
	return mp_CompoM;
// 	if (m_IsMaked)		return mp_CompoM;
// 	else
// 	{
// 		delete mp_CompoM;
// 		return NULL;
// 	}
}

void MTRcSlabMHelper::MakeRebarTextDataArr()
{
	SRSectionBox* pSectionBox = mp_Table->ma_Box[0];
	SRSlabInfo* pSlabInfo = pSectionBox->mp_SlabInfo;

	ma_ShortRebarText.clear();
	ma_ShortRebarText.push_back(_T(""));
	ma_ShortRebarText.push_back(pSlabInfo->m_X1);		ma_ShortRebarText.push_back(pSlabInfo->m_X2);
	ma_ShortRebarText.push_back(pSlabInfo->m_X3);		ma_ShortRebarText.push_back(pSlabInfo->m_X4);
	ma_ShortRebarText.push_back(pSlabInfo->m_X5);		ma_ShortRebarText.push_back(pSlabInfo->m_X6);
	ma_ShortRebarText.push_back(pSlabInfo->m_X7);		ma_ShortRebarText.push_back(pSlabInfo->m_X8);
	ma_ShortRebarText.push_back(pSlabInfo->m_X9);		ma_ShortRebarText.push_back(pSlabInfo->m_X10);

	auto shortStripResult = AddResultItem(MTResultItem::SlabShortStrip, _T(""));
	AddSubResultItem(MTResultItem::SlabShortStrip, MTResultItem::SlabBar1, pSlabInfo->m_X1);
	AddSubResultItem(MTResultItem::SlabShortStrip, MTResultItem::SlabBar2, pSlabInfo->m_X2);
	AddSubResultItem(MTResultItem::SlabShortStrip, MTResultItem::SlabBar3, pSlabInfo->m_X3);
	AddSubResultItem(MTResultItem::SlabShortStrip, MTResultItem::SlabBar4, pSlabInfo->m_X4);
	AddSubResultItem(MTResultItem::SlabShortStrip, MTResultItem::SlabBar5, pSlabInfo->m_X5);
	AddSubResultItem(MTResultItem::SlabShortStrip, MTResultItem::SlabBar6, pSlabInfo->m_X6);
	AddSubResultItem(MTResultItem::SlabShortStrip, MTResultItem::SlabBar7, pSlabInfo->m_X7);
	AddSubResultItem(MTResultItem::SlabShortStrip, MTResultItem::SlabBar8, pSlabInfo->m_X8);
	AddSubResultItem(MTResultItem::SlabShortStrip, MTResultItem::SlabBar9, pSlabInfo->m_X9);
	AddSubResultItem(MTResultItem::SlabShortStrip, MTResultItem::SlabBar10, pSlabInfo->m_X10);

	ma_LongRebarText.clear();
	ma_LongRebarText.push_back(_T(""));
	ma_LongRebarText.push_back(pSlabInfo->m_Y1);		ma_LongRebarText.push_back(pSlabInfo->m_Y2);
	ma_LongRebarText.push_back(pSlabInfo->m_Y3);		ma_LongRebarText.push_back(pSlabInfo->m_Y4);
	ma_LongRebarText.push_back(pSlabInfo->m_Y5);		ma_LongRebarText.push_back(pSlabInfo->m_Y6);
	ma_LongRebarText.push_back(pSlabInfo->m_Y7);		ma_LongRebarText.push_back(pSlabInfo->m_Y8);
	ma_LongRebarText.push_back(pSlabInfo->m_Y9);		ma_LongRebarText.push_back(pSlabInfo->m_Y10);

	auto longtStripResult = AddResultItem(MTResultItem::SlabLongStrip, _T(""));
	AddSubResultItem(MTResultItem::SlabLongStrip, MTResultItem::SlabBar1, pSlabInfo->m_Y1);
	AddSubResultItem(MTResultItem::SlabLongStrip, MTResultItem::SlabBar2, pSlabInfo->m_Y2);
	AddSubResultItem(MTResultItem::SlabLongStrip, MTResultItem::SlabBar3, pSlabInfo->m_Y3);
	AddSubResultItem(MTResultItem::SlabLongStrip, MTResultItem::SlabBar4, pSlabInfo->m_Y4);
	AddSubResultItem(MTResultItem::SlabLongStrip, MTResultItem::SlabBar5, pSlabInfo->m_Y5);
	AddSubResultItem(MTResultItem::SlabLongStrip, MTResultItem::SlabBar6, pSlabInfo->m_Y6);
	AddSubResultItem(MTResultItem::SlabLongStrip, MTResultItem::SlabBar7, pSlabInfo->m_Y7);
	AddSubResultItem(MTResultItem::SlabLongStrip, MTResultItem::SlabBar8, pSlabInfo->m_Y8);
	AddSubResultItem(MTResultItem::SlabLongStrip, MTResultItem::SlabBar9, pSlabInfo->m_Y9);
	AddSubResultItem(MTResultItem::SlabLongStrip, MTResultItem::SlabBar10, pSlabInfo->m_Y10);

}

CString MTRcSlabMHelper::GetRebarTextFromSlabInfo(int BarNum, bool IsShortBar, SRSlabInfo* pSlabInfo)
{
	CString rebarText = _T("");
	if (BarNum < 0 || BarNum >= ma_ShortRebarText.size() || BarNum >= ma_LongRebarText.size())	return rebarText;

	if (IsShortBar)
		return ma_ShortRebarText[BarNum];
	else
		return ma_LongRebarText[BarNum];
}

void MTRcSlabMHelper::MakeSlabRebars(bool IsShortBar, SRSlabInfo* pSlabInfo)
{
	bool IsBent = false;
	if (mp_Reader->m_SlabX3Y3Type == SLAB_X3Y3_TYPE_BENT)
		IsBent = true;
	MSRcStrip::STRIP_TYPE StripType = pSlabInfo->GetStripType(IsShortBar, IsBent);
	int Level = -1;
	CString errorMessage;
	if (StripType == MSRcStrip::TYPE_NONE)
		StripType = MSRcStrip::TYPE_ALL;

	MSRcSlabM* pRebarM = (MSRcSlabM*)mp_CompoM->GetRebarM();
	MSRcStrip* pStrip = &pRebarM->m_ShortStrip;
	if (IsShortBar == false)
		pStrip = &pRebarM->m_LongStrip;

	pStrip->m_StripType = StripType;

	long BarNum = 1;
	if (pSlabInfo->GetIsFlatSlabType())
	{
		BarNum = 1;
		MakeStirrupBar(GetRebarTextFromSlabInfo(BarNum, IsShortBar, pSlabInfo), &pStrip->m_Bar1, BarNum, IsShortBar);
		BarNum = 4;
		MakeStirrupBar(GetRebarTextFromSlabInfo(BarNum, IsShortBar, pSlabInfo), &pStrip->m_Bar4, BarNum, IsShortBar);
		return;
	}
	
	if (StripType == MSRcStrip::TYPE_ALL)
	{
		MakeStirrupBar(GetRebarTextFromSlabInfo(BarNum, IsShortBar, pSlabInfo), &pStrip->m_Bar1, BarNum, IsShortBar);	BarNum++;
		MakeStirrupBar(GetRebarTextFromSlabInfo(BarNum, IsShortBar, pSlabInfo), &pStrip->m_Bar2, BarNum, IsShortBar);	BarNum++;
		MakeStirrupBar(GetRebarTextFromSlabInfo(BarNum, IsShortBar, pSlabInfo), &pStrip->m_Bar3, BarNum, IsShortBar);	BarNum++;
		MakeStirrupBar(GetRebarTextFromSlabInfo(BarNum, IsShortBar, pSlabInfo), &pStrip->m_Bar4, BarNum, IsShortBar);	BarNum++;
		MakeStirrupBar(GetRebarTextFromSlabInfo(BarNum, IsShortBar, pSlabInfo), &pStrip->m_Bar5, BarNum, IsShortBar);	BarNum++;
		MakeStirrupBar(GetRebarTextFromSlabInfo(BarNum, IsShortBar, pSlabInfo), &pStrip->m_Bar6, BarNum, IsShortBar);	BarNum++;
		MakeStirrupBar(GetRebarTextFromSlabInfo(BarNum, IsShortBar, pSlabInfo), &pStrip->m_Bar7, BarNum, IsShortBar);	BarNum++;
		MakeStirrupBar(GetRebarTextFromSlabInfo(BarNum, IsShortBar, pSlabInfo), &pStrip->m_Bar8, BarNum, IsShortBar);	BarNum++;
		MakeStirrupBar(GetRebarTextFromSlabInfo(BarNum, IsShortBar, pSlabInfo), &pStrip->m_Bar9, BarNum, IsShortBar);	BarNum++;
		MakeStirrupBar(GetRebarTextFromSlabInfo(BarNum, IsShortBar, pSlabInfo), &pStrip->m_Bar10, BarNum, IsShortBar);	BarNum++;
	}
	else
	{
		MakeStirrupBar(GetRebarTextFromSlabInfo(5, IsShortBar, pSlabInfo), &pStrip->m_Bar5, 5, IsShortBar);
		MakeStirrupBar(GetRebarTextFromSlabInfo(6, IsShortBar, pSlabInfo), &pStrip->m_Bar6, 6, IsShortBar);
		switch (StripType)
		{
		case MSRcStrip::TYPE_A:
			MakeStirrupBar(GetRebarTextFromSlabInfo(1, IsShortBar, pSlabInfo), &pStrip->m_Bar1, 1, IsShortBar);
			MakeStirrupBar(GetRebarTextFromSlabInfo(4, IsShortBar, pSlabInfo), &pStrip->m_Bar4, 4, IsShortBar);
			break;
		case MSRcStrip::TYPE_B:
			MakeStirrupBar(GetRebarTextFromSlabInfo(2, IsShortBar, pSlabInfo), &pStrip->m_Bar2, 2, IsShortBar);
			MakeStirrupBar(GetRebarTextFromSlabInfo(4, IsShortBar, pSlabInfo), &pStrip->m_Bar4, 4, IsShortBar);
			break;
		case MSRcStrip::TYPE_C:
			MakeStirrupBar(GetRebarTextFromSlabInfo(2, IsShortBar, pSlabInfo), &pStrip->m_Bar2, 2, IsShortBar);
			MakeStirrupBar(GetRebarTextFromSlabInfo(4, IsShortBar, pSlabInfo), &pStrip->m_Bar4, 4, IsShortBar);
			MakeStirrupBar(GetRebarTextFromSlabInfo(7, IsShortBar, pSlabInfo), &pStrip->m_Bar7, 7, IsShortBar);
			break;
		case MSRcStrip::TYPE_D:
			MakeStirrupBar(GetRebarTextFromSlabInfo(1, IsShortBar, pSlabInfo), &pStrip->m_Bar1, 1, IsShortBar);
			MakeStirrupBar(GetRebarTextFromSlabInfo(2, IsShortBar, pSlabInfo), &pStrip->m_Bar2, 2, IsShortBar);
			MakeStirrupBar(GetRebarTextFromSlabInfo(4, IsShortBar, pSlabInfo), &pStrip->m_Bar4, 4, IsShortBar);
			MakeStirrupBar(GetRebarTextFromSlabInfo(7, IsShortBar, pSlabInfo), &pStrip->m_Bar7, 7, IsShortBar);
			break;
		case MSRcStrip::TYPE_E:
			MakeStirrupBar(GetRebarTextFromSlabInfo(1, IsShortBar, pSlabInfo), &pStrip->m_Bar1, 1, IsShortBar);
			MakeStirrupBar(GetRebarTextFromSlabInfo(2, IsShortBar, pSlabInfo), &pStrip->m_Bar2, 2, IsShortBar);
			MakeStirrupBar(GetRebarTextFromSlabInfo(4, IsShortBar, pSlabInfo), &pStrip->m_Bar4, 4, IsShortBar);
			break;
		case MSRcStrip::TYPE_F:
			MakeStirrupBar(GetRebarTextFromSlabInfo(3, IsShortBar, pSlabInfo), &pStrip->m_Bar3, 3, IsShortBar);
			MakeStirrupBar(GetRebarTextFromSlabInfo(4, IsShortBar, pSlabInfo), &pStrip->m_Bar4, 4, IsShortBar);
			break;
		case MSRcStrip::TYPE_G:
			MakeStirrupBar(GetRebarTextFromSlabInfo(1, IsShortBar, pSlabInfo), &pStrip->m_Bar1, 1, IsShortBar);
			MakeStirrupBar(GetRebarTextFromSlabInfo(3, IsShortBar, pSlabInfo), &pStrip->m_Bar3, 3, IsShortBar);
			MakeStirrupBar(GetRebarTextFromSlabInfo(4, IsShortBar, pSlabInfo), &pStrip->m_Bar4, 4, IsShortBar);
			break;
		case MSRcStrip::TYPE_H:
			MakeStirrupBar(GetRebarTextFromSlabInfo(2, IsShortBar, pSlabInfo), &pStrip->m_Bar2, 2, IsShortBar);
			MakeStirrupBar(GetRebarTextFromSlabInfo(3, IsShortBar, pSlabInfo), &pStrip->m_Bar3, 3, IsShortBar);
			MakeStirrupBar(GetRebarTextFromSlabInfo(4, IsShortBar, pSlabInfo), &pStrip->m_Bar4, 4, IsShortBar);
			break;
		case MSRcStrip::TYPE_I:
			MakeStirrupBar(GetRebarTextFromSlabInfo(1, IsShortBar, pSlabInfo), &pStrip->m_Bar1, 1, IsShortBar);
			MakeStirrupBar(GetRebarTextFromSlabInfo(4, IsShortBar, pSlabInfo), &pStrip->m_Bar4, 4, IsShortBar);
			MakeStirrupBar(GetRebarTextFromSlabInfo(7, IsShortBar, pSlabInfo), &pStrip->m_Bar7, 7, IsShortBar);
			break;
		}
	}
}

void MTRcSlabMHelper::SetIsUsedStirrupBar(long BarNum, bool IsShort, bool IsUsed)
{
	MSRcSlabM* pRebarM = (MSRcSlabM*)mp_CompoM->GetRebarM();
	if (IsShort)
	{
		switch (BarNum)
		{
		case 1:	pRebarM->m_IsShort1 = IsUsed;	break;
		case 2:	pRebarM->m_IsShort2 = IsUsed;	break;
		case 3:	pRebarM->m_IsShort3 = IsUsed;	break;
		case 4:	pRebarM->m_IsShort4 = IsUsed;	break;
		case 5:	pRebarM->m_IsShort5 = IsUsed;	break;
		case 6:	pRebarM->m_IsShort6 = IsUsed;	break;
		case 7:	pRebarM->m_IsShort7 = IsUsed;	break;
		case 8:	pRebarM->m_IsShort8 = IsUsed;	break;
		case 9:	pRebarM->m_IsShort9 = IsUsed;	break;
		case 10:	pRebarM->m_IsShort10 = IsUsed;	break;
		}
	}
	else
	{
		switch (BarNum)
		{
		case 1:	pRebarM->m_IsLong1 = IsUsed;	break;
		case 2:	pRebarM->m_IsLong2 = IsUsed;	break;
		case 3:	pRebarM->m_IsLong3 = IsUsed;	break;
		case 4:	pRebarM->m_IsLong4 = IsUsed;	break;
		case 5:	pRebarM->m_IsLong5 = IsUsed;	break;
		case 6:	pRebarM->m_IsLong6 = IsUsed;	break;
		case 7:	pRebarM->m_IsLong7 = IsUsed;	break;
		case 8:	pRebarM->m_IsLong8 = IsUsed;	break;
		case 9:	pRebarM->m_IsLong9 = IsUsed;	break;
		case 10:	pRebarM->m_IsLong10 = IsUsed;	break;
		}
	}

}

MTResultItem* MTRcSlabMHelper::GetSubResultItemByBarNum(MTResultItem::eItemType stripType, int barNum)
{
	MTResultItem::eItemType barType = MTResultItem::SlabBar1;
	switch (barNum)
	{
	case 1:	barType = MTResultItem::SlabBar1;	break;
	case 2:	barType = MTResultItem::SlabBar2;	break;
	case 3:	barType = MTResultItem::SlabBar3;	break;
	case 4:	barType = MTResultItem::SlabBar4;	break;
	case 5:	barType = MTResultItem::SlabBar5;	break;
	case 6:	barType = MTResultItem::SlabBar6;	break;
	case 7:	barType = MTResultItem::SlabBar7;	break;
	case 8:	barType = MTResultItem::SlabBar8;	break;
	case 9:	barType = MTResultItem::SlabBar9;	break;
	case 10:	barType = MTResultItem::SlabBar10;	break;
	}
	return GetSubResultItem(stripType, barType);
}

CString MTRcSlabMHelper::GetStripTypeName(int stripType)
{
	CString typeName = _T("NONE");
	MSRcStrip::STRIP_TYPE eStripType = (MSRcStrip::STRIP_TYPE)stripType;
	switch (eStripType)
	{
	case MSRcStrip::TYPE_NONE:	return _T("NONE");
	case MSRcStrip::TYPE_A:			return _T("A");
	case MSRcStrip::TYPE_B:			return _T("B");
	case MSRcStrip::TYPE_C:			return _T("C");
	case MSRcStrip::TYPE_D:			return _T("D");
	case MSRcStrip::TYPE_E:			return _T("E");
	case MSRcStrip::TYPE_F:			return _T("F");
	case MSRcStrip::TYPE_G:			return _T("G");
	case MSRcStrip::TYPE_H:			return _T("H");
	case MSRcStrip::TYPE_I:				return _T("I");
	case MSRcStrip::TYPE_ALL:		return _T("ALL");
	}
	return typeName;
}
void MTRcSlabMHelper::MakeStirrupBar(CString rebarText, MSStirrupBar* pStirrupBar, long BarNum, bool IsShort)
{
	CString sLoc = _T("Ly");
	if (IsShort)	sLoc = _T("Lx");

	MSRcSlabM* pRebarM = (MSRcSlabM*)mp_CompoM->GetRebarM();
	MSRcStrip* pStrip = &pRebarM->m_ShortStrip;
	MTResultItem::eItemType stripType = MTResultItem::SlabShortStrip;
	if (IsShort == false)
	{
		pStrip = &pRebarM->m_LongStrip;
		stripType = MTResultItem::SlabLongStrip;
	}
	auto stripResult = GetResultItem(stripType);
	stripResult->SetItemText(GetStripTypeName(pStrip->m_StripType));
	auto barResult = GetSubResultItemByBarNum(stripType, BarNum);
	bool IsUsedBar = true;
	barResult->SetItemText(rebarText);
	if (rebarText == _T("") && pStrip->m_StripType == MSRcStrip::TYPE_ALL)
	{
		SetIsUsedStirrupBar(BarNum, IsShort, false);
		return;
	}

	SetIsUsedStirrupBar(BarNum, IsShort, true);
	if (rebarText == _T(""))
	{
		CString errorMessage;
		errorMessage.Format(_T("%s : %sBar%ld %s"), mp_CompoM->m_Name, sLoc, BarNum, LANG2(StirrupBars,SectionRecognize));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		barResult->SetItemState(MTResultItem::Error);
		// 		m_IsMaked = false;
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
		errorMessage.Format(_T("%s : %sBar%ld %s"), mp_CompoM->m_Name, sLoc, BarNum, LocalDic::GetLocal(_T("스터럽바"), _T("SectionRecognize")));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		barResult->SetItemState(MTResultItem::Error);
		//		m_IsMaked = false;
		return;
	}

	CString sFirstDiaName = MSStirrupBar::getFirstDiaName2StrpBar(rebarText);
	if (MSDeformedBar::FindBarDiameterIndexByName(sFirstDiaName) == -1)
	{
		CString errorMessage;
		errorMessage.Format(_T("%s : %sBar%ld"), mp_CompoM->m_Name, sLoc, BarNum);
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_IGNORE_REBARTEXT, errorMessage);
		barResult->SetItemState(MTResultItem::Error);
		//		m_IsMaked = false;
		return;
	}

	int spacing = 0;
	int indexParentheses = rebarText.Find(_T("("));
	if (indexParentheses == -1)
		spacing = _ttoi(rebarText.Mid(indexAt + 1, rebarText.GetLength() - indexAt));
	else
		spacing = _ttoi(rebarText.Mid(indexAt + 1, indexParentheses - (indexAt + 1)));

	int spacingTolerance = 50;
	if (spacing < spacingTolerance)
	{
		m_IsMaked = false;
		CString errorMessage;
		errorMessage.Format(_T("%s : %sBar%ld %s(%ld < %ld)"), mp_CompoM->m_Name, sLoc, BarNum, LocalDic::GetLocal(_T("간격"), _T("SectionRecognize")),spacing, spacingTolerance);
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		barResult->SetItemState(MTResultItem::Error);
		// 		m_IsMaked = false;
		return;
	}

	CString strData;
	strData.Format(_T("%s%s@%ld"), MSDeformedBar::m_sDefaultBarType, sFirstDiaName, spacing);
	pStirrupBar->InitData(strData);

	if (indexPlus != -1)
	{
		CString sSecondDiaName = MSStirrupBar::getSecondDiaName2StrpBar(rebarText);
		if (MSDeformedBar::FindBarDiameterIndexByName(sSecondDiaName) == -1)
		{
			CString errorMessage;
			errorMessage.Format(_T("%s : %sBar%ld"), mp_CompoM->m_Name, sLoc, BarNum);
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_IGNORE_REBARTEXT, errorMessage);
			barResult->SetItemState(MTResultItem::Error);
			//			m_IsMaked = false;
			return;
		}
		strData.Format(_T("%s%s@%ld"), MSDeformedBar::m_sDefaultBarType, sSecondDiaName, spacing);
		pStirrupBar->Add(strData);
	}
	barResult->SetItemText(pStirrupBar->GetStirrupBarString());
}
