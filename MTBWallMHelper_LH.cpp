#include "stdafx.h"
#include "MTBWallMHelper_LH.h"

#include "GMLib/GMText.h"
#include "SRSectionTable.h"

#include "MSCompoM.h"
#include "MSRcFaceMemberM.h"
#include "MSRcBWallM.h"
MTBWallMHelper_LH::MTBWallMHelper_LH(MSBaseBuilding* pBldg, MTReader* pReader) : MTReaderHelper(pBldg, pReader)
{
	mp_Table = NULL;
	mp_Name = nullptr;
	mp_Thick = nullptr;
	mp_VMarkText = nullptr;
	mp_HMarkText = nullptr;

	m_minThick = 20;
	m_isDummy = false;
}


MTBWallMHelper_LH::~MTBWallMHelper_LH()
{
	DeleteWallMInfos();
}

void MTBWallMHelper_LH::DeleteWallMInfos()
{
	for (int i = 0; i < ma_WallMInfo.GetSize(); i++)
		delete ma_WallMInfo[i];
	ma_WallMInfo.RemoveAll();
}

long MTBWallMHelper_LH::GetFromStory()
{
	return -(mp_Bldg->GetNumUnderStory() - 1);
}

long MTBWallMHelper_LH::GetToStory()
{
	return mp_Bldg->GetNumAboveStory() - 1;
}

CString MTBWallMHelper_LH::GetName()
{
	if (mp_Name)	return mp_Name->m_strText;
	return _T("");
}

int MTBWallMHelper_LH::GetThick()
{
	if (mp_Thick )	return _ttoi(mp_Thick->m_strText);
	return mp_Reader->GetDefaultWallThick_LH();
}

void MTBWallMHelper_LH::MakeData(SRSectionTable* pTable)
{
	mp_ResultElement->SetArea(pTable->mp_Rect);

	mp_Table = pTable;

	if (!MakeWallDatas())	m_IsMaked = false;
	if (MSCompoM::IsExistedSpacialCharacters(GetName()))
	{
		CString errorMessage;
		errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1의 이름에 사용할 수 없는 특수 기호가 존재합니다."), _T(" SectionRecognize")), GetName()));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		//		AddErrorFigureID(mp_WallInfo->GetTextFigureID(MTReader::ctName));
		m_IsMaked = false;
	}
}

void MTBWallMHelper_LH::MakeVBars(MSRcBWallM* pRebarM, CString rebarText)
{
	if (rebarText == "")	return;
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

void MTBWallMHelper_LH::MakeHBars(MSRcBWallM* pRebarM, CString rebarText)
{
	if (rebarText == "")	return;
	if (rebarText.Find('-') != -1)
	{
		pRebarM->m_bIsNumPlaceHBar = true;
		MakeMainBars(rebarText, &pRebarM->m_MainHBar, MTResultItem::HBars);
	}
	else
	{
		MakeStirrupBars(rebarText, &pRebarM->m_HBars, MTReader::ctHBar, MTResultItem::HBars);
		pRebarM->m_bIsNumPlaceHBar = false;
	}
}

void MTBWallMHelper_LH::MakeMainBars(CString rebarText, MSMainBar* pMainBar, MTResultItem::eItemType itemType)
{
	auto barResult = GetResultItem(itemType);
	CString sRebarText = rebarText.MakeUpper();
	sRebarText.Replace(_T(" "), _T(""));

	long nLength = sRebarText.GetLength();
	long nIndex = sRebarText.Find('D');
	if (nIndex == -1)
	{
		CString errorMessage;
		errorMessage.Format(_T("%s(%ld~%ld) %s %s"), mp_CompoM->m_Name, mp_CompoM->m_nFromStory, mp_CompoM->m_nToStory, LocalDic::GetLocal(_T("수직근"), _T("SectionRecognize")), LocalDic::GetLocal(_T("정보에 오류가 있습니다."), _T("SectionRecognize")));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
//		AddErrorFigureID(mp_WallInfo->GetTextFigureID(MTReader::ctVBar));
		m_IsMaked = false;
		barResult->SetItemState(MTResultItem::Error);

		return;
	}
	CString sDiaName = sRebarText.Mid(nIndex + 1, nLength - (nIndex + 1));
	if (!CheckDia(_ttof(sDiaName)))
	{
		CString errorMessage;
		errorMessage.Format(_T("%s %s(%s)"), mp_CompoM->m_Name, LocalDic::GetLocal(_T("수직근"), _T("SectionRecognize")), sDiaName);
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_IGNORE_REBARTEXT, errorMessage);
//		AddErrorFigureID(mp_WallInfo->GetTextFigureID(MTReader::ctVBar));
		m_IsMaked = false;
		barResult->SetItemState(MTResultItem::Error);
		return;
	}

	pMainBar->InitData(rebarText);
	barResult->SetItemText(pMainBar->GetTotalMainBarString());
}

void MTBWallMHelper_LH::MakeStirrupBars(CString rebarText, MSStirrupBar* pStirrupBar, int cellType, MTResultItem::eItemType itemType)
{
	auto barResult = GetResultItem(itemType);

	CString sLoc = LocalDic::GetLocal(_T("수평근"), _T("SectionRecognize"));
	if (cellType == MTReader::ctVBar)	sLoc = LocalDic::GetLocal(_T("수직근"), _T("SectionRecognize"));
	if (rebarText == _T(""))
	{
		CString errorMessage;
		errorMessage.Format(_T("%s(%ld~%ld) %s %s"), mp_CompoM->m_Name, mp_CompoM->m_nFromStory, mp_CompoM->m_nToStory, sLoc, LocalDic::GetLocal(_T("정보에 오류가 있습니다."), _T("SectionRecognize")));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
//		AddErrorFigureID(mp_WallInfo->GetTextFigureID(cellType));
		m_IsMaked = false;
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
//		AddErrorFigureID(mp_WallInfo->GetTextFigureID(cellType));
		m_IsMaked = false;
		barResult->SetItemState(MTResultItem::Error);
		return;
	}

	CString sFirstDiaName = MSStirrupBar::getFirstDiaName2StrpBar(rebarText);
	if (MSDeformedBar::FindBarDiameterIndexByName(sFirstDiaName) == -1)
	{
		CString errorMessage;
		errorMessage.Format(_T("%s(%ld~%ld) %s %s"), mp_CompoM->m_Name, mp_CompoM->m_nFromStory, mp_CompoM->m_nToStory, sLoc, LocalDic::GetLocal(_T("정보에 오류가 있습니다."), _T("SectionRecognize")));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_IGNORE_REBARTEXT, errorMessage);
//		AddErrorFigureID(mp_WallInfo->GetTextFigureID(cellType));
		m_IsMaked = false;
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
//			AddErrorFigureID(mp_WallInfo->GetTextFigureID(cellType));
			m_IsMaked = false;
			barResult->SetItemState(MTResultItem::Error);
			return;
		}
		strData.Format(_T("%s%s@%ld"), MSDeformedBar::m_sDefaultBarType, sSecondDiaName, spacing);
		pStirrupBar->Add(strData);
	}
	barResult->SetItemText(pStirrupBar->GetStirrupBarString());
}

bool MTBWallMHelper_LH::MakeWallDatas()
{
	if (!FindMarkText())	return false;

	if (!FindNameAndThickText())
		m_IsMaked = false;
	CTypedPtrArray<CObArray, GMText*> rebarTextArr, floorTextArr;
	AnalyzeTexts(rebarTextArr, floorTextArr);

	long nFloor = floorTextArr.GetSize();
	long nRebar = rebarTextArr.GetSize();
	if (nFloor != nRebar)
	{
		CString errorMessage;
		errorMessage.Format(_T("ERROR : "));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_STORY_REBAR_DIFF_COUNT, errorMessage);
		return false;
	}

	MakeWallMInfo(floorTextArr, rebarTextArr);
	FixWallMInfoRebarText();

	//CheckWallInfos();
	return true;
}

void MTBWallMHelper_LH::FixWallMInfoRebarText()
{
	// 철근정보가 없을때는 아래층의 정보로 설정
	SortWallMInfo();
	int numInfo = ma_WallMInfo.GetSize();
	for (int i = 0; i < numInfo - 1; i++)
	{
		auto curInfo = ma_WallMInfo[i];
		auto nxtInfo = ma_WallMInfo[i + 1];
		if (nxtInfo->m_HBarText == "")
			nxtInfo->m_HBarText = curInfo->m_HBarText;
		if (nxtInfo->m_VBarText == "")
			nxtInfo->m_VBarText = curInfo->m_VBarText;
	}
}
void MTBWallMHelper_LH::SortWallMInfo()
{
	std::vector<XWallMInfo*> vecWallInfo;
	const int nSize = ma_WallMInfo.GetSize();
	for (int i = 0; i < nSize; ++i)
		vecWallInfo.push_back(ma_WallMInfo[i]);

	// 정렬
	std::sort(vecWallInfo.begin(), vecWallInfo.end(),
		[](XWallMInfo* a, XWallMInfo* b) {
			return a->m_nFromStory < b->m_nFromStory;
		});

	// std::vector → 다시 CTypedPtrArray에 복사
	ma_WallMInfo.RemoveAll();
	for (auto* pInfo : vecWallInfo)
		ma_WallMInfo.Add(pInfo);
}
bool MTBWallMHelper_LH::CheckWallInfos()
{
	long nFrom = GetFromStory();
	long nTo = GetToStory();

	CString name = GetName();
	for (int iInfo = 0; iInfo < ma_WallMInfo.GetSize(); iInfo++)
	{
		XWallMInfo* pInfo = ma_WallMInfo[iInfo];
		if (pInfo->m_nFromStory == 0 || pInfo->m_nToStory == 0)
		{
			CString errorMessage;
			errorMessage.Format(_T("%s(%ld~%ld)"), name, pInfo->m_nFromStory, pInfo->m_nToStory);
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, STORY_ERROR, errorMessage);
			//			AddErrorFigureID(mp_WallInfo->GetTextFigureID(MTReader::ctStory));
			m_IsMaked = false;
		}
		if (pInfo->m_nFromStory < nFrom || pInfo->m_nToStory > nTo)
		{
			CString errorMessage;
			errorMessage.Format(_T("%s(%ld~%ld)"), name, pInfo->m_nFromStory, pInfo->m_nToStory);
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, STORY_OUT_OF_RANGE, errorMessage);
			//			AddErrorFigureID(pInfo->GetTextFigureID(MTReader::ctStory));
			m_IsMaked = false;
		}
	}
	if (GetThick() <= GetMinThick())
	{
		CString errorMessage;
		errorMessage.Format(_T("%s"), name);
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_THICK, errorMessage);
//			AddErrorFigureID(pInfo->GetTextFigureID(MTReader::ctThick));
		m_IsMaked = false;
	}

	if (m_IsMaked)
	{
		for (int iInfo = 0; iInfo < ma_WallMInfo.GetSize(); iInfo++)
		{
			XWallMInfo* pInfoI = ma_WallMInfo[iInfo];
			for (int jInfo = iInfo + 1; jInfo < ma_WallMInfo.GetSize(); jInfo++)
			{
				XWallMInfo* pInfoJ = ma_WallMInfo[jInfo];
				if (pInfoI->IsOverlapStory(pInfoJ))
				{
					CString errorMessage;
					errorMessage.Format(_T("%s"), name);
					CreateErrorObject(GetGroupName(), ERROR_LEVEL, STORY_OVERLAP, errorMessage);
					//			AddErrorFigureID(mp_WallInfo->GetTextFigureID(MTReader::ctStory));
					m_IsMaked = false;
				}
			}
		}
	}
	return m_IsMaked;
}

bool MTBWallMHelper_LH::FindMarkText()
{
	for (int i = 0; i < mp_Table->ma_Text.GetSize(); i++)
	{
		GMText* pText = mp_Table->ma_Text[i];
		if (pText->m_strText == mp_Reader->m_VMark_LH)
			mp_VMarkText = pText;
		else if (pText->m_strText == mp_Reader->m_HMark_LH)
			mp_HMarkText = pText;
	}

	if (mp_VMarkText == nullptr || mp_HMarkText == nullptr)
	{
		CString errorMessage = _T("ERROR : ");
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_STORY_REBAR_DIFF_COUNT, errorMessage);
		return false;
	}
	return true;
}

GM2DLineVector MTBWallMHelper_LH::GetRefLineVector()
{
	if (mp_VMarkText == nullptr || mp_HMarkText == nullptr)	 return GM2DLineVector();
	GM2DLineVector RefLVec(mp_HMarkText->m_Vec, mp_VMarkText->m_Vec);
	if (!IsVMarkUpper())
	{
		GM2DVector temp = RefLVec.m_P0;
		RefLVec.m_P0 = RefLVec.m_P1;
		RefLVec.m_P1 = temp;
	}
	return RefLVec;
}

// 숫자로만 이루어진 Thick(Thick은 없으면 DefaultThick사용), TextFilter로 Name
bool MTBWallMHelper_LH::FindNameAndThickText()
{
	vector<GMText*> nameArr, thickArr;

	for (int i = 0; i < mp_Table->ma_Text.GetSize(); i++)
	{
		GMText* pText = mp_Table->ma_Text[i];
		if (pText == mp_VMarkText || pText == mp_HMarkText)	continue;

		int thick = _ttol(pText->m_strText);
		if (thick > GetMinThick())
		{
			thickArr.push_back(pText);
		}
		else if (mp_Reader->GetCellTypeByTextFilter(pText) == MTReader::ctName)
		{
			nameArr.push_back(pText);
		}
	}
	bool isError = true;
	if (nameArr.size() == 1)
	{
		mp_Name = nameArr[0];
		isError = false;
	}
	if (thickArr.size() == 1)
	{
		mp_Thick = thickArr[0];
	}
	else if (thickArr.size() > 1)
		isError = true;

	if (isError)
	{
		CString errorMessage = _T("ERROR : ");
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_NOT_FOUND_NAME_THICK, errorMessage);
		return false;
	}
	return true;
}

bool MTBWallMHelper_LH::IsVMarkUpper()
{
	if (mp_VMarkText == nullptr || mp_HMarkText == nullptr)	return false;
	if (mp_VMarkText->m_Vec.m_Y > mp_HMarkText->m_Vec.m_Y)	return true;
	return false;
}

bool MTBWallMHelper_LH::AnalyzeTexts(CTypedPtrArray<CObArray, GMText*>& rebarTextArr, CTypedPtrArray<CObArray, GMText*>& floorTextArr)
{
	GM2DLineVector RefLVec = GetRefLineVector();

	AddTextFigureID(mp_Name);
	AddTextFigureID(mp_Thick);

	map<GMText*, double> rebarTextLocationMap, floorTextLocationMap;
	// yc가 낮은 순으로 정렬
	for (int i = 0; i < mp_Table->ma_Text.GetSize(); i++)
	{
		GMText* pText = mp_Table->ma_Text[i];
		if (pText == mp_VMarkText || pText == mp_HMarkText)	continue;
		if (pText == mp_Name || pText == mp_Thick)	continue;

		AddTextFigureID(pText);
		double curLocation = RefLVec.OrthoBaseLocValue(pText->m_Vec);
		bool IsAdded = false;
		if (pText->m_strText.Find('D') != -1)
		{
			long nRebar = rebarTextArr.GetSize();
			for (long iRebar = 0; iRebar < nRebar; iRebar++)
			{
				GMText* pUrText = rebarTextArr[iRebar];
				auto it = rebarTextLocationMap.find(pUrText);
				if (it != rebarTextLocationMap.end())
				{
					if (it->second > curLocation)
					{
						rebarTextArr.InsertAt(iRebar, pText);
						rebarTextLocationMap.insert(make_pair(pText, curLocation));
						IsAdded = true;
						break;
					}
				}
			}
			if (!IsAdded)
			{
				rebarTextArr.Add(pText);
				rebarTextLocationMap.insert(make_pair(pText, curLocation));
			}
		}
		else
		{
			long nRebar = floorTextArr.GetSize();
			for (long iRebar = 0; iRebar < nRebar; iRebar++)
			{
				GMText* pUrText = floorTextArr[iRebar];
				auto it = floorTextLocationMap.find(pUrText);
				if (it != floorTextLocationMap.end())
				{
					if (it->second > curLocation)
					{
						floorTextArr.InsertAt(iRebar, pText);
						floorTextLocationMap.insert(make_pair(pText, curLocation));
						IsAdded = true;
						break;
					}
				}
			}
			if (!IsAdded)
			{
				floorTextArr.Add(pText);
				floorTextLocationMap.insert(make_pair(pText, curLocation));
			}
		}
	}

	return true;
}

XWallMInfo* MTBWallMHelper_LH::GetWallMInfo(long fromStory, long toStory)
{
	for (int i = 0; i < ma_WallMInfo.GetSize(); i++)
	{
		if (ma_WallMInfo[i]->m_nFromStory == fromStory && ma_WallMInfo[i]->m_nToStory == toStory)
			return ma_WallMInfo[i];
	}
	return nullptr;
}
long MTBWallMHelper_LH::MakeWallMInfo(CTypedPtrArray<CObArray, GMText*>& FloorArr, CTypedPtrArray<CObArray, GMText*>& RebarArr)
{
	long NumText = FloorArr.GetSize();
	long FromStory = 0, ToStory = 0;
	long nFirstFrom = 0;
	bool IsHBarText = true;
	if(IsVMarkUpper() == false)	IsHBarText = false;

	CString Name = GetName();
	int Thick = GetThick();
	// Yc가 낮은 순으로 정렬되어 있다
	for (int iText = 0; iText < NumText; iText++)
	{
		CString strFloor = FloorArr[iText]->m_strText;
		CString strRebar = RebarArr[iText]->m_strText;

		mp_Reader->Parsing_strStory(strFloor, FromStory, ToStory);
		if (iText == 0)			nFirstFrom = FromStory;
		else if (FromStory <= nFirstFrom && nFirstFrom <= ToStory)
			IsHBarText = !IsHBarText;

		if (FromStory == 0 || ToStory == 0)
		{
			auto OldInfo = GetWallMInfo(FromStory, ToStory);
			if (OldInfo != nullptr)
			{
				IsHBarText = !IsHBarText;
				SetRebarText(OldInfo, strRebar, IsHBarText);
				continue;
			}
			XWallMInfo* NewWallInfo = new XWallMInfo();
			NewWallInfo->m_Name = Name;
			NewWallInfo->m_Thick = Thick;
			NewWallInfo->m_nFromStory = FromStory;
			NewWallInfo->m_nToStory = ToStory;
			SetRebarText(NewWallInfo, strRebar, IsHBarText);
			ma_WallMInfo.Add(NewWallInfo);
			continue;
		}
		BOOL bMerged = FALSE;
		int nBWallCount = ma_WallMInfo.GetSize();
		for (int iBWall = 0; iBWall < nBWallCount; iBWall++)
		{
			XWallMInfo* OldInfo = ma_WallMInfo[iBWall];
			if (OldInfo->m_nFromStory == 0 || OldInfo->m_nToStory == 0)	continue;
			if (OldInfo->m_nFromStory <= FromStory && ToStory <= OldInfo->m_nToStory) // 기존 구간에 포함
			{
				if (FromStory == OldInfo->m_nFromStory && ToStory == OldInfo->m_nToStory)
				{
					SetRebarText(OldInfo, strRebar, IsHBarText);

					bMerged = TRUE;
				}
				else if (FromStory == OldInfo->m_nFromStory && ToStory < OldInfo->m_nToStory)
				{
					SetRebarText(OldInfo, strRebar, IsHBarText);

					XWallMInfo* NewInfo = new XWallMInfo();
					NewInfo->CopyData(OldInfo);
					OldInfo->m_nFromStory = ToStory + 1;
					if (OldInfo->m_nFromStory == 0)	OldInfo->m_nFromStory = 1;

					NewInfo->m_nToStory = ToStory;
					ma_WallMInfo.Add(NewInfo);
					bMerged = TRUE;
				}
				else if (FromStory > OldInfo->m_nFromStory && ToStory < OldInfo->m_nToStory)
				{
					SetRebarText(OldInfo, strRebar, IsHBarText);

					XWallMInfo* NewInfo1 = new XWallMInfo();	// Old From & To copy
					NewInfo1->CopyData(OldInfo);

					XWallMInfo* NewInfo2 = new XWallMInfo();	// Old From & To copy
					NewInfo2->CopyData(OldInfo);

					OldInfo->m_nToStory = FromStory - 1;
					if (OldInfo->m_nToStory == 0)	OldInfo->m_nToStory = -1;
					ma_WallMInfo.SetAt(iBWall, OldInfo);

					NewInfo1->m_nFromStory = FromStory;
					NewInfo1->m_nToStory = ToStory;
					ma_WallMInfo.Add(NewInfo1);

					NewInfo2->m_nFromStory = ToStory + 1;
					if (NewInfo2->m_nFromStory == 0)	NewInfo2->m_nFromStory = 1;
					ma_WallMInfo.Add(NewInfo2);

					bMerged = TRUE;
				}
				else if (FromStory > OldInfo->m_nFromStory && ToStory == OldInfo->m_nToStory)
				{
					SetRebarText(OldInfo, strRebar, IsHBarText);

					XWallMInfo* NewInfo = new XWallMInfo();
					NewInfo->CopyData(OldInfo);
					OldInfo->m_nToStory = FromStory - 1;
					if (OldInfo->m_nToStory == 0)	OldInfo->m_nToStory = 1;
					ma_WallMInfo.SetAt(iBWall, OldInfo);

					NewInfo->m_nFromStory = FromStory;
					ma_WallMInfo.Add(NewInfo);
					bMerged = TRUE;
				}
				break;
			}
			else
			{
				if (FromStory <= OldInfo->m_nFromStory && OldInfo->m_nToStory <= ToStory)
				{
					SetRebarText(OldInfo, strRebar, IsHBarText);

					FromStory = OldInfo->m_nToStory + 1;
					if (FromStory == 0)	FromStory = 1;
				}
			}
		}
		if (bMerged == FALSE)
		{
			XWallMInfo* NewWallInfo = new XWallMInfo();
			NewWallInfo->m_Name = Name;
			NewWallInfo->m_Thick = Thick;
			NewWallInfo->m_nFromStory = FromStory;
			NewWallInfo->m_nToStory = ToStory;
			SetRebarText(NewWallInfo, strRebar, IsHBarText);
			ma_WallMInfo.Add(NewWallInfo);
		}
	}
	return ma_WallMInfo.GetSize();
}

void MTBWallMHelper_LH::SetRebarText(XWallMInfo* pInfo, CString rebarText, bool IsHBarText)
{
	if (IsHBarText)
		pInfo->m_HBarText = rebarText;
	else
		pInfo->m_VBarText = rebarText;
}
vector<MSCompoM*> MTBWallMHelper_LH::MakeBWallMs()
{
	vector<MSCompoM*> compoMs;

	//if (mp_Name == nullptr)
	//{
	//	MSRcFaceMemberM* pBWallM = CreateBWallM();
	//	pBWallM->SetName(_T(""));
	//	compoMs.push_back(pBWallM);

	//	MTResultElement* pResult = new MTResultElement();
	//	pResult->SetArea(mp_Table->mp_Rect);
	//	auto pNameResult = pResult->AddResultItem(MTResultItem::Name, _T(""));
	//	pNameResult->SetItemState(MTResultItem::Error);
	//	ma_Result.push_back(pResult);
	//}

	for (int iInfo = 0; iInfo < ma_WallMInfo.GetSize(); iInfo++)
	{
		MSCompoM* compoM = MakeBWallMData(ma_WallMInfo[iInfo]);
		if (compoM != nullptr)
		{
			compoMs.push_back(compoM);
		}
	}

	if (!m_IsMaked)	// 철근정보 에러
	{
		for (int i = 0; i < compoMs.size(); i++)
			delete compoMs[i];
		compoMs.clear();
	}
	return compoMs;
}

MSRcFaceMemberM* MTBWallMHelper_LH::CreateBWallM()
{
	MSRcFaceMemberM* pCompoM = (MSRcFaceMemberM*)mp_Bldg->CreateNewCompoM(MSCompoM::msBWallM);
	pCompoM->mp_RebarM = MSCompoM::CreateRebarM(MSCompoM::msBWallM);
	 
	pCompoM->SetMaterial(mp_Bldg->GetDefMaterial());
	pCompoM->SetIsUsedStoryMaterial(true);

	return pCompoM;
}

void MTBWallMHelper_LH::FixRebarTexts(XWallMInfo* pInfo)
{
	MTReader::CorrectiveMainBar(pInfo->m_VBarText);
	MTReader::CorrectiveStirrupBar(pInfo->m_VBarText);

	MTReader::CorrectiveMainBar(pInfo->m_HBarText);
	MTReader::CorrectiveStirrupBar(pInfo->m_HBarText);
}
MSCompoM* MTBWallMHelper_LH::MakeBWallMData(XWallMInfo *pInfo)
{
	MTResultElement* pResult = new MTResultElement();
	pResult->SetArea(mp_Table->mp_Rect);

	MSRcFaceMemberM* pBWallM = CreateBWallM();
	pBWallM->SetName(pInfo->m_Name);
	pBWallM->SetThick(pInfo->m_Thick);
	pBWallM->m_nFromStory = pInfo->m_nFromStory;;
	pBWallM->m_nToStory = pInfo->m_nToStory;

	MSRcBWallM* pRebarM = (MSRcBWallM*)pBWallM->GetRebarM();

	FixRebarTexts(pInfo);
	MakeVBars(pRebarM, pInfo->m_VBarText);
	MakeHBars(pRebarM, pInfo->m_HBarText);

	pResult->AddResultItem(MTResultItem::Name, pInfo->m_Name);

	CString sFrom = mp_Bldg->GetFloorName(pInfo->m_nFromStory);
	CString sTo  = mp_Bldg->GetFloorName(pInfo->m_nToStory);
	if (sFrom == _T(""))	sFrom = _T("None");
	if (sTo == _T(""))		sTo = _T("None");

	pResult->AddResultItem(MTResultItem::FromStory, sFrom);
	pResult->AddResultItem(MTResultItem::ToStory, sTo);

	CString sThick;
	sThick.Format(_T("%ld"), pInfo->m_Thick);
	pResult->AddResultItem(MTResultItem::Thick, sThick);

	pResult->AddResultItem(MTResultItem::VBars, pInfo->m_VBarText);
	pResult->AddResultItem(MTResultItem::HBars, pInfo->m_HBarText);

	pResult->SetIsDummy(m_isDummy);

	ma_Result.push_back(pResult);
	return pBWallM;
}

bool MTBWallMHelper_LH::IsSameData(MTBWallMHelper_LH* urHelper)
{
	int myCount = ma_WallMInfo.GetSize();
	int urCount = urHelper->ma_WallMInfo.GetSize();
	if (myCount != urCount)	return false;

	for (int i = 0; i < myCount; i++)
	{
		XWallMInfo* myInfo = ma_WallMInfo[i];
		XWallMInfo* urInfo = urHelper->ma_WallMInfo[i];
		if (!myInfo->IsSameData(urInfo))	return false;
	}
	return true;
}

void MTBWallMHelper_LH::AddTextFigureID(GMText* pText)
{
	if (pText == nullptr)	return;
	int figureID = mp_Reader->FindTextFigureID(pText);
	if (figureID == 0)	return;

	mm_FigureIDToData.insert(make_pair(figureID, pText->m_strText));
}

