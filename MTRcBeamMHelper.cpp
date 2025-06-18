#include "StdAfx.h"
#include "MTRcBeamMHelper.h"
#include "SRSectionTable.h"
#include "SRSectionBox.h"

#include "StructureModel/MSCompoM.h"
#include "StructureModel/MSRcLineMemberM.h"
#include "StructureModel/MSRcBeamM.h"
#include "StructureModel/MSRcBeamSec.h"
#include "StructureModel/MRRcSection.h"
#include "StructureModel/MREdge.h"
#include "StructureModel/MRVBar.h"
#include "StructureModel/MRHBarByPoint.h"
#include "StructureModel/MRHBarPointByLoc.h"

MTRcBeamMHelper::MTRcBeamMHelper(MSBaseBuilding* pBldg, MTReader* pReader) : MTReaderHelper(pBldg, pReader)
{
	mp_Table = NULL;
}


MTRcBeamMHelper::~MTRcBeamMHelper()
{

}

long MTRcBeamMHelper::GetFromStory()
{
	return -(mp_Bldg->GetNumUnderStory());
}

long MTRcBeamMHelper::GetToStory()
{
	return mp_Bldg->GetNumAboveStory()-1;
}

MSCompoM* MTRcBeamMHelper::MakeData(CString Name, SRSectionTable* pTable)
{
	mp_ResultElement->SetArea(pTable->mp_Rect);

	CheckName(Name, mp_Reader->FindTextFigureID(pTable->mp_Name));

	mp_CompoM = mp_Bldg->CreateNewCompoM(MSCompoM::msBeamM);
	mp_CompoM->mp_RebarM = MSCompoM::CreateRebarM(MSCompoM::msBeamM);
	mp_Table = pTable;

	MSMaterial* pDefMaterial = mp_Bldg->GetMaterial(mp_Reader->m_nMaterialID);
	if (pDefMaterial == NULL)	pDefMaterial = mp_Bldg->GetDefMaterial(MSMaterial::msConcrete);

	mp_CompoM->m_Name = Name;
	mp_CompoM->m_nFromStory = GetFromStory();
	mp_CompoM->m_nToStory = GetToStory();
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
	CheckSideBars();

	MSRcBeamM* pRebarM = (MSRcBeamM*)mp_CompoM->GetRebarM();
	pRebarM->SetIsSubBeam(mp_Reader->IsSubBeam(Name));

	return mp_CompoM;
// 	if(m_IsMaked)		return mp_CompoM;
// 	else
// 	{
// 		delete mp_CompoM;
// 		return NULL;
// 	}
}

void MTRcBeamMHelper::MakeSections()
{
	int nSectionBox = mp_Table->ma_Box.GetSize();
	MTResultElement* largeElement = AddSubElement(MTResultItem::LargeSection);
	MTResultElement* middleElement = AddSubElement(MTResultItem::MiddelSection);
	MTResultElement* smallerElement = AddSubElement(MTResultItem::SmallerSection);

	if (!IsPlainConcreteReinforceBar())
	{
		for (int iBox = 0; iBox < nSectionBox; iBox++)
		{
			SRSectionBox* pSectionBox = mp_Table->ma_Box[iBox];
			MSRcBeamSec* pBeamSec = GetBeamSection(iBox);
			if (pBeamSec == NULL)
			{
				m_IsMaked = false;	continue;
			}
			MTResultItem::eItemType beamSecType = GetBeamSectionType(iBox);

			MakeTopBars(pSectionBox, pSectionBox->mp_MainBarSet1, &pBeamSec->m_TBars, beamSecType);
			MakeBotBars(pSectionBox, pSectionBox->mp_MainBarSet2, &pBeamSec->m_BBars, beamSecType);


			MakeStirrupBars(pSectionBox, pSectionBox->mp_StrpBar1, &pBeamSec->m_SBars, pBeamSec->m_SBarNum, beamSecType);

			if (pSectionBox->mp_SideBar)
			{
				MakeSideBars(pSectionBox, pSectionBox->mp_SideBar, &pBeamSec->m_SideBars);
				pBeamSec->m_bIsNumPlaceSideBar = TRUE;
			}
			else if (pSectionBox->m_SideStirrupBar)
			{
				MakeSideBars(pSectionBox, pSectionBox->m_SideStirrupBar, &pBeamSec->m_SideStirrupBar);
				pBeamSec->m_bIsNumPlaceSideBar = FALSE;
			}
			if (pSectionBox->mp_SubMainBar)
			{
				MakeSubMainBars(pSectionBox, pSectionBox->mp_SubMainBar, &pBeamSec->m_SubMainBar);
			}


		}
	}
	if(nSectionBox == 1)
	{
		GetLargeSection()->CopyFromMe(GetMidSection());
		GetSmallerSection()->CopyFromMe(GetMidSection());

		largeElement->CopyResultItems(middleElement);
		smallerElement->CopyResultItems(middleElement);
	}
	else if(nSectionBox == 2)
	{
		GetSmallerSection()->CopyFromMe(GetLargeSection());
		smallerElement->CopyResultItems(largeElement);
	}
}

MSRcBeamSec* MTRcBeamMHelper::GetLargeSection()
{
	MSRcBeamM* pRebarM = (MSRcBeamM*)mp_CompoM->GetRebarM();
	return pRebarM->mp_LargeSec;
}
MSRcBeamSec* MTRcBeamMHelper::GetMidSection()
{
	MSRcBeamM* pRebarM = (MSRcBeamM*)mp_CompoM->GetRebarM();
	return pRebarM->mp_MidSec;
}
MSRcBeamSec* MTRcBeamMHelper::GetSmallerSection()
{
	MSRcBeamM* pRebarM = (MSRcBeamM*)mp_CompoM->GetRebarM();
	return pRebarM->mp_SmallSec;
}

MSRcBeamSec* MTRcBeamMHelper::GetBeamSection(int SectionIdx)
{
	int nSectionCount = mp_Table->ma_Box.GetSize();
	if(nSectionCount == 1)
	{
		return GetMidSection();
	}
	else if(nSectionCount == 2)
	{
		if (SectionIdx == 0)
			return GetLargeSection();
		else
		{
			return GetMidSection();
		}
	}
	else if(nSectionCount == 3)
	{
		if(mp_Reader->m_eBeamSectionType == MTReader::Small_Mid_Large)
		{
			if (SectionIdx == 0)				return GetSmallerSection();
			else if (SectionIdx == 1)		return GetMidSection();
			else											return GetLargeSection();
		}
		else
		{
			if (SectionIdx == 0)				return GetLargeSection();
			else if (SectionIdx == 1)		return GetMidSection();
			else											return GetSmallerSection();
		}
	}

	return NULL;
}

// 0 : large, 1: mid, 2 : smaller
MTResultItem::eItemType MTRcBeamMHelper::GetBeamSectionType(int SectionIdx)
{
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
		if (mp_Reader->m_eBeamSectionType == MTReader::Small_Mid_Large)
		{
			if (SectionIdx == 0)				return MTResultItem::SmallerSection;
			else if (SectionIdx == 1)		return MTResultItem::MiddelSection;
			else											return MTResultItem::LargeSection;
		}
		else
		{
			if (SectionIdx == 0)				return MTResultItem::LargeSection;
			else if (SectionIdx == 1)		return MTResultItem::MiddelSection;
			else											return MTResultItem::SmallerSection;
		}
	}

	return MTResultItem::MiddelSection;
}

bool MTRcBeamMHelper::MakeTopBars(SRSectionBox* pSectionBox, MSMainBarSet* pBoxBar, MSMainBarSet* pMainBar, MTResultItem::eItemType sectionType)
{
	bool IsOK = true;
	MTResultElement* sectionElement = GetSubElement(sectionType);
	CString sBoxText = _T("");
	if (pBoxBar != nullptr)	sBoxText = pBoxBar->GetBarString();

	MTResultItem* topResult = sectionElement->AddResultItem(MTResultItem::TopBars, sBoxText);
	MTResultItem* textTopResult = sectionElement->AddResultItem(MTResultItem::TextTopBar, pSectionBox->m_sTopBar);
// 	sectionResult->
	if(pBoxBar == NULL)
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
			strBar.Format(_T("%ld-%s%s"), NumBar, MSDeformedBar::m_sDefaultBarType,diaName);
			MSMainBar mainBar(strBar);
			pMainBar->Add(MSMainBar(strBar));
		}
	}
	else
		pMainBar->SetBarString(pBoxBar->GetBarString());

    if (IsDiffrenentMainBarSet(topResult->GetItemText(), textTopResult->GetItemText()))
    {
        topResult->SetItemState(MTResultItem::Error);
    }


	topResult->SetItemText(pMainBar->GetBarString());
	return true;
}

bool MTRcBeamMHelper::MakeBotBars(SRSectionBox* pSectionBox, MSMainBarSet* pBoxBar, MSMainBarSet* pMainBar, MTResultItem::eItemType sectionType)
{
	bool IsOK = true;
	MTResultElement* sectionElement = GetSubElement(sectionType);
	CString sBoxText = _T("");
	if (pBoxBar != nullptr)	sBoxText = pBoxBar->GetBarString();
	MTResultItem* botResult = sectionElement->AddResultItem(MTResultItem::BotBars, sBoxText);
	MTResultItem* textBotResult = sectionElement->AddResultItem(MTResultItem::TextBotBar, pSectionBox->m_sBotBar);
	if(pBoxBar == NULL)
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

	if(!CheckMainBar(pBoxBar, pBotEdge))
	{
		CString errorMessage;
		errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1의 하부근 텍스트와 형상 정보에 오류가 있습니다."), _T(" SectionRecognize")), mp_CompoM->m_Name));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		AddErrorFigureIDs(pSectionBox->GetTextFigureIDs(MTReader::ctBBar));
		botResult->SetItemState(MTResultItem::Error);
//		return false;
	}

	for(int iText = 0; iText < nTextLayer; iText++)
	{
		map<CString, int> diaNameToNumBarMap = pBoxBar->GetDiaNameToNumBarsOnSet(iText);
		map<CString, int>::iterator it;
		for (it = diaNameToNumBarMap.begin(); it != diaNameToNumBarMap.end(); it++)
		{
			double dDia = _tstof(it->first);
			if (!CheckDia(dDia))
			{
				CString errorMessage;
				errorMessage.Format(_T("%s %s %s"), mp_CompoM->m_Name, LocalDic::GetLocal(_T("하부근 직경"), _T("SectionRecognize")),it->first);
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
			strBar.Format(_T("%ld-%s%s"), NumBar, MSDeformedBar::m_sDefaultBarType,diaName);
			MSMainBar mainBar(strBar);
			pMainBar->Add(MSMainBar(strBar));
		}
	}
	else
		pMainBar->SetBarString(pBoxBar->GetBarString());

	botResult->SetItemText(pMainBar->GetBarString());
	return true;
}

bool MTRcBeamMHelper::MakeStirrupBars(SRSectionBox* pSectionBox, MSStirrupBar* pBoxBar, MSStirrupBar* pStirrupBar, long& StirrupCount, MTResultItem::eItemType sectionType)
{
	MTResultElement* sectionElement = GetSubElement(sectionType);
	CString sBoxText = _T("");
	if (pBoxBar != nullptr)	sBoxText = pBoxBar->GetStirrupBarString();

	MTResultItem* stirrupResult = sectionElement->AddResultItem(MTResultItem::StirrupBars, sBoxText);
	MTResultItem* textStirrupResult = sectionElement->AddResultItem(MTResultItem::TextStirrupBar, pSectionBox->m_sStirrupBar1);
	if(pBoxBar == NULL)
	{
		CString errorMessage;
		errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1의 스터럽 텍스트 정보가 없습니다."), _T(" SectionRecognize")), mp_CompoM->m_Name));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		stirrupResult->SetItemState(MTResultItem::Error);
 		return false;
	}

	bool IsOK = true;
	int NumLayer = pBoxBar->GetNumLayers();
	for( int iLayer = 0; iLayer < NumLayer; iLayer++)
	{
		CString sDiaName = pBoxBar->GetBarDiameterName();
		double dDia = _ttof(sDiaName);
		if(CheckDia(dDia) == FALSE)
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
	if(nTieCol != pSectionBox->m_nStrp)	// RH-126
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

bool MTRcBeamMHelper::MakeSideBars(SRSectionBox* pSectionBox, MSMainBar* pBoxBar, MSMainBar* pSideBar)
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
	if(CheckDia(dDia) == FALSE)
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

bool MTRcBeamMHelper::MakeSideBars(SRSectionBox* pSectionBox, MSStirrupBar* pBoxBar, MSStirrupBar* pSideBar)
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
	for( int iLayer = 0; iLayer < NumLayer; iLayer++)
	{
		CString sDiaName = pBoxBar->GetBarDiameterName(iLayer);
		double dDia = _ttof(sDiaName);
		if(CheckDia(dDia) == FALSE)
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

void MTRcBeamMHelper::CopySideBars()
{
	int SideBarType = GetSideBarType();
	if(SideBarType == MAINBAR)
		CopySideMainBars();
	else if(SideBarType == STIRRUPBAR)
		CopySideStirrupBars();
}

MTRcBeamMHelper::eSideBarType MTRcBeamMHelper::GetSideBarType()
{
	int nSectionBox = mp_Table->ma_Box.GetSize();
	for(int iBox = 0; iBox < nSectionBox; iBox++)
	{
		SRSectionBox* pSectionBox = mp_Table->ma_Box[iBox];
		if(pSectionBox->mp_SideBar)
			return MTRcBeamMHelper::MAINBAR;
		else if(pSectionBox->m_SideStirrupBar)
			return MTRcBeamMHelper::STIRRUPBAR;
	}
	return MTRcBeamMHelper::NONE;
}

void MTRcBeamMHelper::CopySideMainBars()
{
	MSMainBar* pFirstSideMainBar = NULL;
	SRSectionBox* pFirstSectionBox = NULL;
	int nSectionBox = mp_Table->ma_Box.GetSize();
	for(int iBox = 0; iBox < nSectionBox; iBox++)
	{
		SRSectionBox* pSectionBox = mp_Table->ma_Box[iBox];
		if(pSectionBox->mp_SideBar)
		{
			pFirstSideMainBar = pSectionBox->mp_SideBar;
			pFirstSectionBox = pSectionBox;
			break;
		}
	}
	if(pFirstSideMainBar == NULL)	return;

	MSRcBeamSec* pLargeSec = GetLargeSection();
	MSRcBeamSec* pMidSec = GetLargeSection();
	MSRcBeamSec* pSmallerSec = GetLargeSection();

	pLargeSec->m_bIsNumPlaceSideBar = true;
	pMidSec->m_bIsNumPlaceSideBar = true;
	pSmallerSec->m_bIsNumPlaceSideBar = true;

	MakeSideBars(pFirstSectionBox, pFirstSideMainBar, &pLargeSec->m_SideBars);
	MakeSideBars(pFirstSectionBox, pFirstSideMainBar, &pMidSec->m_SideBars);
	MakeSideBars(pFirstSectionBox, pFirstSideMainBar, &pSmallerSec->m_SideBars);
}

void MTRcBeamMHelper::CopySideStirrupBars()
{
	MSStirrupBar* pFirstSideStirrupBar = NULL;
	SRSectionBox* pFirstSectionBox = NULL;
	int nSectionBox = mp_Table->ma_Box.GetSize();
	for(int iBox = 0; iBox < nSectionBox; iBox++)
	{
		SRSectionBox* pSectionBox = mp_Table->ma_Box[iBox];
		if(pSectionBox->m_SideStirrupBar)
		{
			pFirstSideStirrupBar = pSectionBox->m_SideStirrupBar;
			pFirstSectionBox = pSectionBox;
			break;
		}
	}
	if(pFirstSideStirrupBar == NULL)	return;

	MSRcBeamSec* pLargeSec = GetLargeSection();
	MSRcBeamSec* pMidSec = GetLargeSection();
	MSRcBeamSec* pSmallerSec = GetLargeSection();

	pLargeSec->m_bIsNumPlaceSideBar = false;
	pMidSec->m_bIsNumPlaceSideBar = false;
	pSmallerSec->m_bIsNumPlaceSideBar = false;

	MakeSideBars(pFirstSectionBox, pFirstSideStirrupBar, &pLargeSec->m_SideStirrupBar);
	MakeSideBars(pFirstSectionBox, pFirstSideStirrupBar, &pMidSec->m_SideStirrupBar);
	MakeSideBars(pFirstSectionBox, pFirstSideStirrupBar, &pSmallerSec->m_SideStirrupBar);
}

bool MTRcBeamMHelper::MakeSubMainBars(SRSectionBox* pSectionBox, MSMainBar* pBoxBar, MSSubMainBar* pSubMainBar)
{
	CString sDiaName = pBoxBar->GetBarDiameterName();
	double dDia = _ttof(sDiaName);
	if (CheckDia(dDia) == FALSE)
	{
		CString errorMessage;
		errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1 보조주근 직경"), _T(" SectionRecognize")), mp_CompoM->m_Name));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		AddErrorFigureIDs(pSectionBox->GetTextFigureIDs(MTReader::ctSubMainBar));
		return false;
	}
	auto textSubMainResult = AddResultItem(MTResultItem::TextSubMainBar, pSectionBox->m_sSubMainBar);

	pSubMainBar->m_MainBar.ma_strBar.RemoveAll();
	pSubMainBar->m_MainBar.ma_strBar.Copy(pBoxBar->ma_strBar);

	auto subMainResult = AddResultItem(MTResultItem::SubMainBars, pSubMainBar->m_MainBar.GetTotalMainBarString());
	return true;
}

bool MTRcBeamMHelper::CheckSideBars()
{
	if (IsPlainConcreteReinforceBar())	return true;

	MSRcBeamSec* pLargeSec = GetLargeSection();
	MSRcBeamSec* pMidSec = GetMidSection();
	MSRcBeamSec* pSmallSec = GetSmallerSection();

	if((pLargeSec->m_bIsNumPlaceSideBar != pMidSec->m_bIsNumPlaceSideBar) || (pLargeSec->m_bIsNumPlaceSideBar != pSmallSec->m_bIsNumPlaceSideBar))
	{
		MakeSideBarError();
		m_IsMaked = false;
		return false;
	}

	bool IsOK = true;
	if(pLargeSec->m_bIsNumPlaceSideBar)
	{
		if(!pLargeSec->m_SideBars.IsSameBar(pMidSec->m_SideBars))
			IsOK = false;
		if(!pLargeSec->m_SideBars.IsSameBar(pSmallSec->m_SideBars))
			IsOK = false;
		if(!pMidSec->m_SideBars.IsSameBar(pSmallSec->m_SideBars))
			IsOK = false;
		if(!IsOK)
		{
			MakeSideBarError();
			m_IsMaked = false;
			return false;
		}
	}
	else
	{
		if(!pLargeSec->m_SideStirrupBar.IsSameBar(pMidSec->m_SideStirrupBar))
			IsOK = false;
		if(!pLargeSec->m_SideStirrupBar.IsSameBar(pSmallSec->m_SideStirrupBar))
			IsOK = false;
		if(!pMidSec->m_SideStirrupBar.IsSameBar(pSmallSec->m_SideStirrupBar))
			IsOK = false;
		if(!IsOK)
		{
			MakeSideBarError();
			m_IsMaked = false;
			return false;
		}
	}
	return true;
}

void MTRcBeamMHelper::MakeSideBarError()
{
	CString errorMessage;
	errorMessage.Format(_T("%s %s"), mp_CompoM->m_Name, LocalDic::GetLocal(_T("측면근 정보"), _T("SectionRecognize")));
	CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
	vector<int> figureIDs = GetSideBarFigureIDs();
	int NumFigureID = figureIDs.size();
	for(int i = 0; i < NumFigureID; i++)
		AddErrorFigureID(figureIDs[i]);
}

vector<int> MTRcBeamMHelper::GetSideBarFigureIDs()
{
	vector<int> figureIDs;
	int NumSection = mp_Table->ma_Box.GetSize();
	for(int iBox = 0; iBox < NumSection; iBox++)
	{
		SRSectionBox* pSectionBox = mp_Table->ma_Box[iBox];
		if (pSectionBox->mp_SideBar)
		{
			auto Ids = pSectionBox->GetTextFigureIDs(MTReader::ctSBar2);
			for(int i = 0; i < Ids.size(); i++)
				figureIDs.push_back(Ids[i]);
		}
	}
	return figureIDs;
}

bool MTRcBeamMHelper::CheckMainBar(MSMainBarSet* pMainBar, MREdge* pEdge)
{
	if(pMainBar == NULL || pEdge == NULL)	return false;

	int nTextLayer = pMainBar->GetNumSets();
	int nCircleLayer = pEdge->ma_VBarSet.GetSize();

	if (nTextLayer == 1 && nCircleLayer > 1)
	{
		map<CString, int> diaNameToNumBarMap = pMainBar->GetDiaNameToNumBarsOnSet(0);
		if (diaNameToNumBarMap.size() != 1)
			return false;
	}

	if(nTextLayer == 1)
	{
		int nTextBarEa = pMainBar->GetTotalBars();
		int nCircleBarEa = 0;
		for(int iLayer = 0; iLayer < nCircleLayer; iLayer++)
		{
			MRVBar* pVBar = pEdge->ma_VBarSet[iLayer];
			nCircleBarEa += pVBar->ma_VBarLoc.GetSize();
		}
		if(nTextBarEa == nCircleBarEa)	return true;
		return false;
	}
	else if(nTextLayer != nCircleLayer)
		return false;
	else
	{
		for(int iLayer = 0; iLayer < nTextLayer; iLayer++)
		{
			int nTextBarEa = pMainBar->GetNumBarsOnSet(iLayer);
			MRVBar* pVBar = pEdge->ma_VBarSet[iLayer];
			if(nTextBarEa != pVBar->ma_VBarLoc.GetSize())
				return false;
		}
	}

	return true;
}

