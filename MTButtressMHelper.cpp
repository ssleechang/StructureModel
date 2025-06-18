#include "stdafx.h"
#include "MTButtressMHelper.h"
#include "SRSectionTable.h"
#include "SRSectionBox.h"

#include "StructureModel/MSRcLineMemberM.h"
#include "StructureModel/MDRcButtressM.h"
#include "StructureModel/MDRcButtressSec.h"
#include "StructureModel/MRRcSection.h"
#include "StructureModel/MREdge.h"
#include "StructureModel/MRVBar.h"
#include "StructureModel/MRHBarByPoint.h"
#include "StructureModel/MRHBarPointByLoc.h"

MTButtressMHelper::MTButtressMHelper(MSBaseBuilding* pBldg, MTReader* pReader) : MTReaderHelper(pBldg, pReader)
{
	mp_Table = NULL;
}


MTButtressMHelper::~MTButtressMHelper()
{
}

long MTButtressMHelper::GetFromStory()
{
	return -(mp_Bldg->GetNumUnderStory() - 1);
}

long MTButtressMHelper::GetToStory()
{
	return mp_Bldg->GetNumAboveStory() - 1;
}

MSCompoM* MTButtressMHelper::MakeData(CString Name, SRSectionTable* pTable)
{
	mp_ResultElement->SetArea(pTable->mp_Rect);

	auto nameResult = AddResultItem(MTResultItem::Name, Name);
	vector<int> figureIds;
	CheckName(Name, mp_Reader->FindTextFigureID(pTable->mp_Name));

	mp_CompoM = mp_Bldg->CreateNewCompoM(MSCompoM::msButtressM);
	mp_CompoM->mp_RebarM = mp_CompoM->GetRebarM();
	mp_Table = pTable;

	SRSectionBox* pSectionBox = pTable->ma_Box[0];

	MSMaterial* pDefMaterial = mp_Bldg->GetMaterial(mp_Reader->m_nMaterialID);
	if (pDefMaterial == NULL)	pDefMaterial = mp_Bldg->GetDefMaterial(MSMaterial::msConcrete);

	mp_CompoM->m_Name = Name;
	pSectionBox->GetFromToStory(mp_CompoM->m_nFromStory, mp_CompoM->m_nToStory);
	CheckStory();

	mp_CompoM->SetMaterial(pDefMaterial);
	mp_CompoM->SetIsUsedStoryMaterial(mp_Reader->m_IsUsedStoryMaterial);


	CString dimWidth = _T(""), dimDepth = _T("");
	dimWidth = mp_Table->m_sDimensionWidth;
	dimDepth = mp_Table->m_sDimensionDepth;
	auto widthResult = AddResultItem(MTResultItem::Width, mp_Table->m_sWidth);
	auto depthResult = AddResultItem(MTResultItem::Depth, mp_Table->m_sDepth);
	auto dimensionWidthResult = AddResultItem(MTResultItem::DimensionWidth, dimWidth);
	auto dimensionDepthResult = AddResultItem(MTResultItem::DimensionDepth, dimDepth);

	((MSRcLineMemberM*)mp_CompoM)->SetWidth(mp_Table->m_Width);
	((MSRcLineMemberM*)mp_CompoM)->SetDepth(mp_Table->m_Depth);

	MakeSections();
	CopySideBars();

	return mp_CompoM;

}
bool MTButtressMHelper::CheckStory()
{
	CString fromStory, toStory;

	fromStory = mp_Bldg->GetFloorName(mp_CompoM->m_nFromStory);
	toStory = mp_Bldg->GetFloorName(mp_CompoM->m_nToStory);
	if (fromStory == _T(""))	fromStory = _T("None");
	if (toStory == _T(""))	toStory = _T("None");

	auto fromStoryResult = AddResultItem(MTResultItem::FromStory, fromStory);
	auto toStoryResult = AddResultItem(MTResultItem::ToStory, toStory);
	long nFrom = GetFromStory();
	long nTo = GetToStory();
	SRSectionBox* pSectionBox = mp_Table->ma_Box[0];
	if (mp_CompoM->m_nFromStory < nFrom)
	{
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, STORY_OUT_OF_RANGE, GetCompoMName());
		AddErrorFigureIDs(pSectionBox->GetTextFigureIDs(MTReader::ctStory));
		fromStoryResult->SetItemState(MTResultItem::Error);
	}
	if (fromStory == _T("None"))
	{
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, STORY_OUT_OF_RANGE, GetCompoMName());
		AddErrorFigureIDs(pSectionBox->GetTextFigureIDs(MTReader::ctStory));
		fromStoryResult->SetItemState(MTResultItem::Error);
	}

	// 	if(mp_CompoM->m_nFromStory==0 )
	// 	{
	// 		if(nFrom==0)	nFrom = 1;
	// 		mp_CompoM->m_nFromStory = nFrom;
	// 		CreateErrorObject(GetGroupName(), WARNING_LEVEL, STORY_ERROR, mp_CompoM->m_Name, mp_SectionBox->GetOwnerRect());
	// 		AddErrorFigureID(mp_SectionBox->GetTextFigureID(MTReader::ctStory));
	// 		fromStoryResult->SetItemState(MTResultItem::Warning);
	// 	}

	if (mp_CompoM->m_nToStory > nTo)
	{
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, STORY_OUT_OF_RANGE, GetCompoMName());
		AddErrorFigureIDs(pSectionBox->GetTextFigureIDs(MTReader::ctStory));
		toStoryResult->SetItemState(MTResultItem::Error);
	}
	if (toStory == _T("None"))
	{
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, STORY_OUT_OF_RANGE, GetCompoMName());
		AddErrorFigureIDs(pSectionBox->GetTextFigureIDs(MTReader::ctStory));
		toStoryResult->SetItemState(MTResultItem::Error);
	}

	// 	if (mp_CompoM->m_nToStory == 0)
	// 	{
	// 		if (nTo == 0)	nTo = 1;
	// 		mp_CompoM->m_nToStory = nTo;
	// 		CreateErrorObject(GetGroupName(), WARNING_LEVEL, STORY_ERROR, mp_CompoM->m_Name, mp_SectionBox->GetOwnerRect());
	// 		AddErrorFigureID(mp_SectionBox->GetTextFigureID(MTReader::ctStory));
	// 		toStoryResult->SetItemState(MTResultItem::Warning);
	// 	}

	return true;
}

MDRcButtressSec* MTButtressMHelper::GetButtressSection(int SectionIdx)
{
	MDRcButtressM* pRebarM = dynamic_cast<MDRcButtressM*>(mp_CompoM->GetRebarM());

	int nSectionCount = mp_Table->ma_Box.GetSize();
	if (nSectionCount == 1)
	{
		return pRebarM->mp_TopSec;
	}
	else if (nSectionCount == 2)
	{
		if (SectionIdx == 0)
			return pRebarM->mp_CenSec;
		else
		{
			return pRebarM->mp_BotSec;
		}
	}
	else if (nSectionCount == 3)
	{
		if (mp_Reader->m_eBeamSectionType == MTReader::Small_Mid_Large)
		{
			if (SectionIdx == 0)				return pRebarM->mp_BotSec;
			else if (SectionIdx == 1)		return pRebarM->mp_CenSec;
			else											return pRebarM->mp_TopSec;
		}
		else
		{
			if (SectionIdx == 0)				return pRebarM->mp_TopSec;
			else if (SectionIdx == 1)		return pRebarM->mp_CenSec;
			else											return pRebarM->mp_BotSec;
		}
	}

	return NULL;
}

// 0 : top , 1: center, 2 : bottom
MTResultItem::eItemType MTButtressMHelper::GetButtressSectionType(int SectionIdx)
{
	MDRcButtressM* pRebarM = dynamic_cast<MDRcButtressM*>(mp_CompoM->GetRebarM());

	int nSectionCount = mp_Table->ma_Box.GetSize();
	if (nSectionCount == 1)
	{
		return MTResultItem::MiddelSection;
	}
	else if (nSectionCount == 2)
	{
		if (SectionIdx == 0)
			return MTResultItem::LargeSection;
		else
		{
			return MTResultItem::MiddelSection;
		}
	}
	else if (nSectionCount == 3)
	{
		if (SectionIdx == 0)				return MTResultItem::LargeSection;
		else if (SectionIdx == 1)		return MTResultItem::MiddelSection;
		else											return MTResultItem::SmallerSection;

// 		if (mp_Reader->m_eBeamSectionType == MTReader::Small_Mid_Large)
// 		{
// 			if (SectionIdx == 0)				return MTResultItem::SmallerSection;
// 			else if (SectionIdx == 1)		return MTResultItem::MiddelSection;
// 			else											return MTResultItem::LargeSection;
// 		}
// 		else
// 		{
// 			if (SectionIdx == 0)				return MTResultItem::LargeSection;
// 			else if (SectionIdx == 1)		return MTResultItem::MiddelSection;
// 			else											return MTResultItem::SmallerSection;
// 		}
	}

	return MTResultItem::MiddelSection;
}

void MTButtressMHelper::MakeSections()
{
	MDRcButtressM* pRebarM = dynamic_cast<MDRcButtressM*>(mp_CompoM->GetRebarM());
	int nSectionBox = mp_Table->ma_Box.GetSize();
	MTResultElement* topElement = AddSubElement(MTResultItem::LargeSection);			// Top
	MTResultElement* midElement = AddSubElement(MTResultItem::MiddelSection);	// Center
	MTResultElement* bottomElement = AddSubElement(MTResultItem::SmallerSection);	// Bottom

	if (!IsPlainConcreteReinforceBar())
	{
		for (int iBox = 0; iBox < nSectionBox; iBox++)
		{
			SRSectionBox* pSectionBox = mp_Table->ma_Box[iBox];
			MDRcButtressSec* pButtressSec = GetButtressSection(iBox);
			if (pButtressSec == NULL)
			{
				m_IsMaked = false;	continue;
			}
			MTResultItem::eItemType buttressSecType = GetButtressSectionType(iBox);

			MakeTopBars(pSectionBox, pSectionBox->mp_MainBarSet1, &pButtressSec->m_TBars, buttressSecType);
			MakeBotBars(pSectionBox, pSectionBox->mp_MainBarSet2, &pButtressSec->m_BBars, buttressSecType);


			MakeStirrupBars(pSectionBox, pSectionBox->mp_StrpBar1, &pButtressSec->m_SBars, pButtressSec->m_SBarNum, buttressSecType);

			if (pSectionBox->mp_SideBar)
			{
				MakeSideBars(pSectionBox, pSectionBox->mp_SideBar, &pRebarM->m_SideMainBars);
				pRebarM->m_IsNumPlaceSideBar = TRUE;
			}
			else if (pSectionBox->m_SideStirrupBar)
			{
				MakeSideBars(pSectionBox, pSectionBox->m_SideStirrupBar, &pRebarM->m_SideStirrupBars);
				pRebarM->m_IsNumPlaceSideBar = FALSE;
			}
		}
	}
	if (nSectionBox == 1)
	{
		pRebarM->mp_TopSec->CopyFromMe(pRebarM->mp_CenSec);
		pRebarM->mp_BotSec->CopyFromMe(pRebarM->mp_CenSec);

		topElement->CopyResultItems(midElement);
		bottomElement->CopyResultItems(midElement);
	}
	else if (nSectionBox == 2)
	{
		pRebarM->mp_BotSec->CopyFromMe(pRebarM->mp_TopSec);
		bottomElement->CopyResultItems(topElement);
	}
}

bool MTButtressMHelper::MakeTopBars(SRSectionBox* pSectionBox, MSMainBarSet* pBoxBar, MSMainBarSet* pMainBar, MTResultItem::eItemType sectionType)
{
	bool IsOK = true;
	MTResultElement* sectionElement = GetSubElement(sectionType);
	CString sBoxText = _T("");
	if (pBoxBar != nullptr)	sBoxText = pBoxBar->GetBarString();
	MTResultItem* topResult = sectionElement->AddResultItem(MTResultItem::TopBars, sBoxText);
	MTResultItem* textTopResult = sectionElement->AddResultItem(MTResultItem::TextTopBar, pSectionBox->m_sTopBar);
	// 	sectionResult->
	if (pBoxBar == NULL)
	{
		CString errorMessage;
		errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1의 상부근 텍스트 정보가 없습니다."), _T(" SectionRecognize")), mp_CompoM->m_Name));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		topResult->SetItemState(MTResultItem::Error);
		return false;
	}
	if (!MSMainBarSet::CheckMainBarSet(pBoxBar->GetBarString()))
	{
		CString errorMessage;
		errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1의 상부근 텍스트 정보에 오류가 있습니다."), _T(" SectionRecognize")), mp_CompoM->m_Name));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		AddErrorFigureIDs(pSectionBox->GetTextFigureIDs(MTReader::ctTBar));
		topResult->SetItemState(MTResultItem::Error);
		// 
		// 		return false;
	}

	pMainBar->RemoveAll();
	if (pSectionBox->mp_Section == nullptr)	return false;

	MREdge* pTopEdge = pSectionBox->mp_Section->ma_Edge[2];
	int nCircleLayer = pTopEdge->ma_VBarSet.GetSize();
	int nTextLayer = pBoxBar->GetNumSets();

	if (!CheckMainBar(pBoxBar, pTopEdge))
	{
		CString errorMessage;
		errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1의 상부근 텍스트와 형상 정보에 오류가 있습니다."), _T(" SectionRecognize")), mp_CompoM->m_Name));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		AddErrorFigureIDs(pSectionBox->GetTextFigureIDs(MTReader::ctTBar));
		topResult->SetItemState(MTResultItem::Error);

		// 		return false;
	}

	for (int iText = 0; iText < nTextLayer; iText++)
	{
		map<CString, int> diaNameToNumBarMap = pBoxBar->GetDiaNameToNumBarsOnSet(iText);
		map<CString, int>::iterator it;
		for (it = diaNameToNumBarMap.begin(); it != diaNameToNumBarMap.end(); it++)
		{
			double dDia = _ttof(it->first);
			if (!CheckDia(dDia))
			{
				CString errorMessage;
				errorMessage.Format(_T("%s %s %s"), mp_CompoM->m_Name, LocalDic::GetLocal(_T("상부근 직경"), _T("SectionRecognize")), it->first);
				CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
				AddErrorFigureIDs(pSectionBox->GetTextFigureIDs(MTReader::ctTBar));
				topResult->SetItemState(MTResultItem::Error);

				// 				return false;
			}
		}
	}

	if (nCircleLayer > 1 && nTextLayer == 1)
	{
		map<CString, int> diaNameToNumBarMap = pBoxBar->GetDiaNameToNumBarsOnSet(0);
		map<CString, int>::iterator it = diaNameToNumBarMap.begin();
		CString diaName = it->first;

		CString strBar;
		for (int iCircleLayer = 0; iCircleLayer < nCircleLayer; iCircleLayer++)
		{
			int NumBar = pTopEdge->ma_VBarSet[iCircleLayer]->ma_VBarLoc.GetSize();
			strBar.Format(_T("%ld-HD%s"), NumBar, diaName);
			MSMainBar mainBar(strBar);
			pMainBar->Add(MSMainBar(strBar));
		}
	}
	else
		pMainBar->SetBarString(pBoxBar->GetBarString());

	topResult->SetItemText(pMainBar->GetBarString());
	return true;
}

bool MTButtressMHelper::MakeBotBars(SRSectionBox* pSectionBox, MSMainBarSet* pBoxBar, MSMainBarSet* pMainBar, MTResultItem::eItemType sectionType)
{
	bool IsOK = true;
	MTResultElement* sectionElement = GetSubElement(sectionType);
	CString sBoxText = _T("");
	if (pBoxBar != nullptr)	sBoxText = pBoxBar->GetBarString();
	MTResultItem* botResult = sectionElement->AddResultItem(MTResultItem::BotBars, sBoxText);
	MTResultItem* textBotResult = sectionElement->AddResultItem(MTResultItem::TextBotBar, pSectionBox->m_sBotBar);
	if (pBoxBar == NULL)
	{
		CString errorMessage;
		errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1의 하부근 텍스트 정보가 없습니다."), _T(" SectionRecognize")), mp_CompoM->m_Name));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		botResult->SetItemState(MTResultItem::Error);
		return false;
	}
	if (!MSMainBarSet::CheckMainBarSet(pBoxBar->GetBarString()))
	{
		CString errorMessage;
		errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1의 하부근 텍스트 정보에 오류가 있습니다."), _T(" SectionRecognize")), mp_CompoM->m_Name));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		AddErrorFigureIDs(pSectionBox->GetTextFigureIDs(MTReader::ctBBar));
		botResult->SetItemState(MTResultItem::Error);
		// 		return false;
	}

	pMainBar->RemoveAll();
	if (pSectionBox->mp_Section == nullptr)	return false;

	MREdge* pBotEdge = pSectionBox->mp_Section->ma_Edge[0];
	int nCircleLayer = pBotEdge->ma_VBarSet.GetSize();
	int nTextLayer = pBoxBar->GetNumSets();

	if (!CheckMainBar(pBoxBar, pBotEdge))
	{
		CString errorMessage;
		errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1의 하부근 텍스트와 형상 정보에 오류가 있습니다."), _T(" SectionRecognize")), mp_CompoM->m_Name));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		AddErrorFigureIDs(pSectionBox->GetTextFigureIDs(MTReader::ctBBar));
		botResult->SetItemState(MTResultItem::Error);
		//		return false;
	}

	for (int iText = 0; iText < nTextLayer; iText++)
	{
		map<CString, int> diaNameToNumBarMap = pBoxBar->GetDiaNameToNumBarsOnSet(iText);
		map<CString, int>::iterator it;
		for (it = diaNameToNumBarMap.begin(); it != diaNameToNumBarMap.end(); it++)
		{
			double dDia = _tstof(it->first);
			if (!CheckDia(dDia))
			{
				CString errorMessage;
				errorMessage.Format(_T("%s %s %s"), mp_CompoM->m_Name, LocalDic::GetLocal(_T("하부근 직경"), _T("SectionRecognize")), it->first);
				CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
				AddErrorFigureIDs(pSectionBox->GetTextFigureIDs(MTReader::ctBBar));
				botResult->SetItemState(MTResultItem::Error);
				//				return false;
			}
		}
	}

	if (nCircleLayer > 1 && nTextLayer == 1)
	{
		map<CString, int> diaNameToNumBarMap = pBoxBar->GetDiaNameToNumBarsOnSet(0);
		map<CString, int>::iterator it = diaNameToNumBarMap.begin();
		CString diaName = it->first;

		CString strBar;
		for (int iCircleLayer = 0; iCircleLayer < nCircleLayer; iCircleLayer++)
		{
			int NumBar = pBotEdge->ma_VBarSet[iCircleLayer]->ma_VBarLoc.GetSize();
			strBar.Format(_T("%ld-HD%s"), NumBar, diaName);
			MSMainBar mainBar(strBar);
			pMainBar->Add(MSMainBar(strBar));
		}
	}
	else
		pMainBar->SetBarString(pBoxBar->GetBarString());

	botResult->SetItemText(pMainBar->GetBarString());
	return true;
}

bool MTButtressMHelper::MakeStirrupBars(SRSectionBox* pSectionBox, MSStirrupBar* pBoxBar, MSStirrupBar* pStirrupBar, int& StirrupCount, MTResultItem::eItemType sectionType)
{
	MTResultElement* sectionElement = GetSubElement(sectionType);

	CString sBoxText = _T("");
	if (pBoxBar != nullptr)	sBoxText = pBoxBar->GetStirrupBarString();

	MTResultItem* stirrupResult = sectionElement->AddResultItem(MTResultItem::StirrupBars, sBoxText);
	MTResultItem* textStirrupResult = sectionElement->AddResultItem(MTResultItem::TextStirrupBar, pSectionBox->m_sStirrupBar1);
	if (pBoxBar == NULL)
	{
		CString errorMessage;
		errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1의 스터럽 텍스트 정보가 없습니다."), _T(" SectionRecognize")), mp_CompoM->m_Name));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		stirrupResult->SetItemState(MTResultItem::Error);
		return false;
	}

	bool IsOK = true;
	int NumLayer = pBoxBar->GetNumLayers();
	for (int iLayer = 0; iLayer < NumLayer; iLayer++)
	{
		CString sDiaName = pBoxBar->GetBarDiameterName();
		double dDia = _ttof(sDiaName);
		if (CheckDia(dDia) == FALSE)
		{
			CString errorMessage;
			errorMessage.Format(_T("%s %s "), mp_CompoM->m_Name, LocalDic::GetLocal(_T("스터럽 직경"), _T("SectionRecognize")));
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
			AddErrorFigureIDs(pSectionBox->GetTextFigureIDs(MTReader::ctStirrup));
			stirrupResult->SetItemState(MTResultItem::Error);
			//			IsOK = false;
		}
	}
	//	if(!IsOK)		return IsOK;

	pStirrupBar->ma_strBar.Copy(pBoxBar->ma_strBar);
	StirrupCount = pSectionBox->m_nStrp;

	long nTieCol = 2;
	if (pSectionBox->mp_Section != nullptr)
	{
		long NumTieBar = pSectionBox->mp_Section->ma_TieBar.GetSize();
		for (int iTieBar = 0; iTieBar < NumTieBar; iTieBar++)
		{
			MRHBarByPoint* pTieBar = pSectionBox->mp_Section->ma_TieBar[iTieBar];
			long NumPoint = pTieBar->ma_Point.GetSize();
			for (long iPoint = 0; iPoint < NumPoint; iPoint++)
			{
				MRHBarPointByLoc* pTiePoint = pTieBar->ma_Point[iPoint];
				if (pTiePoint->m_EdgeIdx == 0)// || pTiePoint->m_EdgeIdx==2)
					nTieCol++;
			}
		}
	}
	if (nTieCol != pSectionBox->m_nStrp)	// RH-126
	{
		CString errorMessage;
		errorMessage.Format(_T("%s %s"), mp_CompoM->m_Name, LocalDic::GetLocal(_T("스터럽 개수"), _T("SectionRecognize")));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		AddErrorFigureIDs(pSectionBox->GetTextFigureIDs(MTReader::ctStirrup));
		stirrupResult->SetItemState(MTResultItem::Error);
		//		IsOK = false;
	}
	CString sBar;
	sBar.Format(_T("%ld-%s"), nTieCol, pStirrupBar->GetStirrupBarString());
	stirrupResult->SetItemText(sBar);
	return IsOK;
}

bool MTButtressMHelper::MakeSideBars(SRSectionBox* pSectionBox, MSMainBar* pBoxBar, MSMainBar* pSideBar)
{
	auto sideResult = AddResultItem(MTResultItem::SideBars, _T(""));
	auto textSideResult = AddResultItem(MTResultItem::TextSideBar, pSectionBox->m_sSideBar);
	if (pBoxBar == nullptr)
	{
		sideResult->SetItemState(MTResultItem::Error);
		return false;
	}
	CString sDiaName = pBoxBar->GetBarDiameterName();
	double dDia = _ttof(sDiaName);
	if (CheckDia(dDia) == FALSE)
	{
		CString errorMessage;
		errorMessage.Format(_T("%s %s"), mp_CompoM->m_Name, LocalDic::GetLocal(_T("측면근 직경"), _T("SectionRecognize")));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		AddErrorFigureIDs(pSectionBox->GetTextFigureIDs(MTReader::ctSBar2));
		sideResult->SetItemState(MTResultItem::Error);
		return false;
	}

	pSideBar->ma_strBar.RemoveAll();
	pSideBar->ma_strBar.Copy(pBoxBar->ma_strBar);

	sideResult->SetItemText(pSideBar->GetTotalMainBarString());
	return true;
}

bool MTButtressMHelper::MakeSideBars(SRSectionBox* pSectionBox, MSStirrupBar* pBoxBar, MSStirrupBar* pSideBar)
{
	bool IsOK = true;
	auto sideResult = AddResultItem(MTResultItem::SideBars, _T(""));
	auto textSideResult = AddResultItem(MTResultItem::TextSideBar, pSectionBox->m_sSideBar);
	if (pBoxBar == nullptr)
	{
		sideResult->SetItemState(MTResultItem::Error);
		return false;
	}
	int NumLayer = pBoxBar->GetNumLayers();
	for (int iLayer = 0; iLayer < NumLayer; iLayer++)
	{
		CString sDiaName = pBoxBar->GetBarDiameterName(iLayer);
		double dDia = _ttof(sDiaName);
		if (CheckDia(dDia) == FALSE)
		{
			CString errorMessage;
			errorMessage.Format(_T("%s %s"), mp_CompoM->m_Name, LocalDic::GetLocal(_T("측면근 직경"), _T("SectionRecognize")));
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
			AddErrorFigureIDs(pSectionBox->GetTextFigureIDs(MTReader::ctSBar2));
			sideResult->SetItemState(MTResultItem::Error);
			return false;
		}
	}
	pSideBar->ma_strBar.RemoveAll();
	pSideBar->ma_strBar.Copy(pBoxBar->ma_strBar);
	sideResult->SetItemText(pSideBar->GetStirrupBarString());

	return true;
}

void MTButtressMHelper::CopySideBars()
{
	MDRcButtressM* pRebarM = dynamic_cast<MDRcButtressM*>(mp_CompoM->GetRebarM());
	int SideBarType = GetSideBarType();
	if (SideBarType == MAINBAR)
		CopySideMainBars();
	else if (SideBarType == STIRRUPBAR)
		CopySideStirrupBars();
}

MTButtressMHelper::eSideBarType MTButtressMHelper::GetSideBarType()
{
	int nSectionBox = mp_Table->ma_Box.GetSize();
	for (int iBox = 0; iBox < nSectionBox; iBox++)
	{
		SRSectionBox* pSectionBox = mp_Table->ma_Box[iBox];
		if (pSectionBox->mp_SideBar)
			return MTButtressMHelper::MAINBAR;
		else if (pSectionBox->m_SideStirrupBar)
			return MTButtressMHelper::STIRRUPBAR;
	}
	return MTButtressMHelper::NONE;
}

void MTButtressMHelper::CopySideMainBars()
{
	MSMainBar* pFirstSideMainBar = NULL;
	SRSectionBox* pFirstSectionBox = NULL;
	int nSectionBox = mp_Table->ma_Box.GetSize();
	for (int iBox = 0; iBox < nSectionBox; iBox++)
	{
		SRSectionBox* pSectionBox = mp_Table->ma_Box[iBox];
		if (pSectionBox->mp_SideBar)
		{
			pFirstSideMainBar = pSectionBox->mp_SideBar;
			pFirstSectionBox = pSectionBox;
			break;
		}
	}
	if (pFirstSideMainBar == NULL)	return;

	MDRcButtressM* pRebarM = dynamic_cast<MDRcButtressM*>(mp_CompoM->GetRebarM());
	pRebarM->m_IsNumPlaceSideBar;

	MakeSideBars(pFirstSectionBox, pFirstSideMainBar, &pRebarM->m_SideMainBars);
}

void MTButtressMHelper::CopySideStirrupBars()
{
	MSStirrupBar* pFirstSideStirrupBar = NULL;
	SRSectionBox* pFirstSectionBox = NULL;
	int nSectionBox = mp_Table->ma_Box.GetSize();
	for (int iBox = 0; iBox < nSectionBox; iBox++)
	{
		SRSectionBox* pSectionBox = mp_Table->ma_Box[iBox];
		if (pSectionBox->m_SideStirrupBar)
		{
			pFirstSideStirrupBar = pSectionBox->m_SideStirrupBar;
			pFirstSectionBox = pSectionBox;
			break;
		}
	}
	if (pFirstSideStirrupBar == NULL)	return;

	MDRcButtressM* pRebarM = dynamic_cast<MDRcButtressM*>(mp_CompoM->GetRebarM());
	pRebarM->m_IsNumPlaceSideBar = false;

	MakeSideBars(pFirstSectionBox, pFirstSideStirrupBar, &pRebarM->m_SideStirrupBars);
}

void MTButtressMHelper::MakeSideBarError()
{
	CString errorMessage;
	errorMessage.Format(_T("%s %s"), mp_CompoM->m_Name, LocalDic::GetLocal(_T("측면근 정보"), _T("SectionRecognize")));
	CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
	vector<int> figureIDs = GetSideBarFigureIDs();
	int NumFigureID = figureIDs.size();
	for (int i = 0; i < NumFigureID; i++)
		AddErrorFigureID(figureIDs[i]);
}

vector<int> MTButtressMHelper::GetSideBarFigureIDs()
{
	vector<int> figureIDs;
	int NumSection = mp_Table->ma_Box.GetSize();
	for (int iBox = 0; iBox < NumSection; iBox++)
	{
		SRSectionBox* pSectionBox = mp_Table->ma_Box[iBox];
		if (pSectionBox->mp_SideBar)
		{
			auto Ids = pSectionBox->GetTextFigureIDs(MTReader::ctSBar2);
			for(auto it = Ids.begin(); it != Ids.end(); it++)
				figureIDs.push_back(*it);
		}
	}
	return figureIDs;
}


bool MTButtressMHelper::CheckMainBar(MSMainBarSet* pMainBar, MREdge* pEdge)
{
	if (pMainBar == NULL || pEdge == NULL)	return false;

	int nTextLayer = pMainBar->GetNumSets();
	int nCircleLayer = pEdge->ma_VBarSet.GetSize();

	if (nTextLayer == 1 && nCircleLayer > 1)
	{
		map<CString, int> diaNameToNumBarMap = pMainBar->GetDiaNameToNumBarsOnSet(0);
		if (diaNameToNumBarMap.size() != 1)
			return false;
	}

	if (nTextLayer == 1)
	{
		int nTextBarEa = pMainBar->GetTotalBars();
		int nCircleBarEa = 0;
		for (int iLayer = 0; iLayer < nCircleLayer; iLayer++)
		{
			MRVBar* pVBar = pEdge->ma_VBarSet[iLayer];
			nCircleBarEa += pVBar->ma_VBarLoc.GetSize();
		}
		if (nTextBarEa == nCircleBarEa)	return true;
		return false;
	}
	else if (nTextLayer != nCircleLayer)
		return false;
	else
	{
		for (int iLayer = 0; iLayer < nTextLayer; iLayer++)
		{
			int nTextBarEa = pMainBar->GetNumBarsOnSet(iLayer);
			MRVBar* pVBar = pEdge->ma_VBarSet[iLayer];
			if (nTextBarEa != pVBar->ma_VBarLoc.GetSize())
				return false;
		}
	}

	return true;
}

