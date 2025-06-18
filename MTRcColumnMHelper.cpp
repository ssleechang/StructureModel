#include "StdAfx.h"
#include "MTRcColumnMHelper.h"

#include "SRSectionBox.h"

#include "StructureModel/MSRebarBuilding.h"
#include "StructureModel/MSCompoM.h"
#include "StructureModel/MSConcrete.h"
#include "StructureModel/MSRcLineMemberM.h"
#include "StructureModel/MSRcColumnM.h"
#include "StructureModel/MSSpecificTieBar.h"

#include "StructureModel\MRRcSection.h"
#include "StructureModel/MREdge.h"
#include "StructureModel/MRHBarByPoint.h"
#include "StructureModel/MRHBarPointByLoc.h"
#include "StructureModel/MRVBar.h"

MTRcColumnMHelper::MTRcColumnMHelper(MSBaseBuilding* pBldg, MTReader* pReader) : MTReaderHelper(pBldg, pReader)
{
	mp_SectionBox = NULL;
}


MTRcColumnMHelper::~MTRcColumnMHelper(void)
{
}

MSCompoM* MTRcColumnMHelper::MakeData(CString Name, SRSectionBox* pSectionBox)
{
	mp_ResultElement->SetArea(pSectionBox->GetOwnerRect());

	CheckName(Name, pSectionBox->GetTextFigureIDs(MTReader::ctName));

	mp_CompoM = mp_Bldg->CreateNewCompoM(MSCompoM::msColumnM);
	mp_CompoM->mp_RebarM = MSCompoM::CreateRebarM(MSCompoM::msColumnM);
	mp_SectionBox = pSectionBox;
	mp_CompoM->m_Name = Name;

	pSectionBox->GetFromToStory(mp_CompoM->m_nFromStory, mp_CompoM->m_nToStory);

	MSMaterial* pDefMaterial = mp_Bldg->GetMaterial(mp_Reader->m_nMaterialID);
	if (pDefMaterial == NULL)	pDefMaterial = mp_Bldg->GetDefMaterial(MSMaterial::msConcrete);
	MSMaterial* pMaterial = mp_Bldg->GetMaterial(pSectionBox->m_nMaterialID);
	if (pMaterial != nullptr)	mp_CompoM->SetMaterial(pMaterial);
	else
		mp_CompoM->SetMaterial(pDefMaterial);

	mp_CompoM->SetIsUsedStoryMaterial(mp_Reader->m_IsUsedStoryMaterial);
	((MSRcLineMemberM*)mp_CompoM)->SetShape((MSRcLineMemberM::SECT_SHAPE)mp_SectionBox->GetSectionType());

	CheckStory();

	long Type = mp_SectionBox->GetSectionType();
	CString sShape = GetRectangleText();
	if (Type == MSRcLineMemberM::CIRCLE)	sShape = GetCircleText();
	AddResultItem(MTResultItem::Shape, sShape);

	if(Type == MSRcLineMemberM::RECT)					MakeRect_Data();
	else if(Type == MSRcLineMemberM::CIRCLE)		MakeRound_Data();
	else if(Type == MSRcLineMemberM::POLYGON)	MakePolygon_Data();
	else
		m_IsMaked = false;

	return mp_CompoM;
// 	if(m_IsMaked)	return mp_CompoM;
// 	else
// 	{
// 		delete mp_CompoM;
// 		return NULL;
// 	}
// 	auto TBResult = GetResultItem(MTResultItem::TBBars);
}

bool MTRcColumnMHelper::CheckStory()
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
	if(mp_CompoM->m_nFromStory < nFrom)
	{
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, STORY_OUT_OF_RANGE, GetCompoMName());
		AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctStory));
		fromStoryResult->SetItemState(MTResultItem::Error);
	}
	if (fromStory == _T("None"))
	{
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, STORY_OUT_OF_RANGE, GetCompoMName());
		AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctStory));
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
		AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctStory));
		toStoryResult->SetItemState(MTResultItem::Error);
	}
	if (toStory == _T("None"))
	{
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, STORY_OUT_OF_RANGE, GetCompoMName());
		AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctStory));
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

long MTRcColumnMHelper::GetFromStory()
{
	return -(mp_Bldg->GetNumUnderStory()-1);
}

long MTRcColumnMHelper::GetToStory()
{
	return mp_Bldg->GetNumAboveStory()-1;
}

void MTRcColumnMHelper::MakeRect_Data()
{
	MSRcLineMemberM* pColumnM = (MSRcLineMemberM*)mp_CompoM;
	MSRcColumnM* pRebarM = (MSRcColumnM*)pColumnM->GetRebarM();

	CString width = _T(""), depth = _T(""), dimWidth = _T(""), dimDepth = _T("");
	if (mp_SectionBox->m_hasDimensionSize == true)
	{
		dimWidth.Format(_T("%ld"), ROUND(mp_SectionBox->m_DimensionWidth));
		dimDepth.Format(_T("%ld"), ROUND(mp_SectionBox->m_DimensionDepth));
	}

	auto widthResult = AddResultItem(MTResultItem::Width, mp_SectionBox->m_sWidth);
	auto depthResult = AddResultItem(MTResultItem::Depth, mp_SectionBox->m_sDepth);
	auto dimensionWidthResult = AddResultItem(MTResultItem::DimensionWidth, dimWidth);
	auto dimensionDepthResult = AddResultItem(MTResultItem::DimensionDepth, dimDepth);

	pColumnM->SetWidth(mp_SectionBox->m_Width);
	pColumnM->SetDepth(mp_SectionBox->m_Depth);

	if (IsPlainConcreteReinforceBar())	return;

	MSMainBarSet* pTBar = &pRebarM->m_TBars;
	MSMainBarSet* pBBar = &pRebarM->m_BBars;
	MSMainBarSet* pLBar = &pRebarM->m_LBars;
	MSMainBarSet* pRBar = &pRebarM->m_RBars;
	MakeRect_MainBars(pTBar, pBBar, pLBar, pRBar, pRebarM->GetCover());

	MSStirrupBar* pCenBar = &pRebarM->m_CenHoopBar;
	MSStirrupBar* pEndBar = &pRebarM->m_EndHoopBar;
	MakeStirrupBars(pCenBar, pEndBar);

	long TopBars = pRebarM->m_TBars.GetNumBarsOnSet(0);
	long LeftBars = pRebarM->m_LBars.GetNumBarsOnSet(0);
	MakeRect_TieBarCount(pRebarM->m_nTieRowNum, pRebarM->m_nTieColNum, TopBars, LeftBars);

	MakeRect_TieBars(&pRebarM->m_RowCenTieBar, &pRebarM->m_ColCenTieBar, &pRebarM->m_RowEndTieBar, &pRebarM->m_ColEndTieBar, 
		&pRebarM->m_CenHoopBar, &pRebarM->m_EndHoopBar);

	// 대근 최소 간격
// 	CheckHoopBarMinSpacing(&pRebarM->m_TBars, &pRebarM->m_CenHoopBar);	

	MakeDoubleHoop(pRebarM->m_nDoubleHoopType);
	MakeSpecificTieBars(pRebarM->ma_SpecificTieBar);
	MakeSubMainBars(&pRebarM->m_SubMainBar);
}

void MTRcColumnMHelper::MakeRound_Data()
{
	MSRcLineMemberM* pColumnM = (MSRcLineMemberM*)mp_CompoM;
	MSRcColumnM* pRebarM = (MSRcColumnM*)pColumnM->GetRebarM();

	CString dimWidth = _T("");
	if (mp_SectionBox->m_hasDimensionSize == true)
		dimWidth.Format(_T("%ld"), ROUND(mp_SectionBox->m_DimensionWidth));

	auto widthResult = AddResultItem(MTResultItem::Width, mp_SectionBox->m_sWidth);
	auto depthResult = AddResultItem(MTResultItem::Depth, mp_SectionBox->m_sDepth);
	auto dimensionWidthResult = AddResultItem(MTResultItem::DimensionWidth, dimWidth);
	auto dimensionDepthResult = AddResultItem(MTResultItem::DimensionDepth, dimWidth);

	pColumnM->SetWidth(mp_SectionBox->m_Width);	
	pColumnM->SetDepth(mp_SectionBox->m_Width);

	if (IsPlainConcreteReinforceBar())	return;

	MakeRound_MainBars(&pRebarM->m_TBars);

	MSStirrupBar* pCenBar = &pRebarM->m_CenHoopBar;
	MSStirrupBar* pEndBar = &pRebarM->m_EndHoopBar;
	MakeStirrupBars(pCenBar, pEndBar);

	MakeRound_TieBars(pRebarM->m_nTieRowNum, pRebarM->m_nTieColNum, &pRebarM->m_TBars);

	// 대근 최소 간격
// 	CheckHoopBarMinSpacing(&pRebarM->m_TBars, &pRebarM->m_CenHoopBar);

	MakeDoubleHoop(pRebarM->m_nDoubleHoopType);
	MakeSpecificTieBars(pRebarM->ma_SpecificTieBar);
	MakeSubMainBars(&pRebarM->m_SubMainBar);
}

void MTRcColumnMHelper::MakePolygon_Data()
{
	m_IsMaked = false;
}

bool MTRcColumnMHelper::MakeRect_MainBars(MSMainBarSet* pTBar, MSMainBarSet* pBBar, MSMainBarSet* pLBar, MSMainBarSet* pRBar, double CvrConc)
{
	CheckRect_MainBars();

	if (mp_SectionBox->mp_Section == nullptr)	return false;

	CString strBar;
	MREdge* pTopEdge = mp_SectionBox->mp_Section->ma_Edge[2];
	long nLayer = pTopEdge->ma_VBarSet.GetSize();
	CString sTopDiaName = _T("0");
	pTBar->RemoveAll();
	pBBar->RemoveAll();
	for(long iLayer=0; iLayer<nLayer; iLayer++)
	{
		strBar = pTopEdge->ma_VBarSet[iLayer]->GetBarString();
		MSMainBar mainBar(strBar);
		pTBar->Add(mainBar);
		pBBar->Add(mainBar);
		if(iLayer == 0)
			sTopDiaName = pTBar->GetMaxDiaNameOnSet(0);
	}

	MREdge* pLeftEdge = mp_SectionBox->mp_Section->ma_Edge[3];
	nLayer = pLeftEdge->ma_VBarSet.GetSize();
	pLBar->RemoveAll();
	pRBar->RemoveAll();
	for(long iLayer=0; iLayer<nLayer; iLayer++)
	{
		strBar = pLeftEdge->ma_VBarSet[iLayer]->GetBarString();
		MSMainBar mainBar(strBar);
		pLBar->Add(strBar);
		pRBar->Add(strBar);
	}
	if(nLayer == 0)
	{
		strBar.Format(_T("0-%s%s"), MSDeformedBar::m_sDefaultBarType, sTopDiaName);
		pLBar->Add(strBar);
		pRBar->Add(strBar);
	}

 	CheckRect_MainBar2Layer(pTBar, pLBar);

	// 주근의 최소간격
// 	CheckRect_MainBarMinSpacing(pTBar, pLBar, CvrConc);

	return true;
}

bool MTRcColumnMHelper::CheckRect_MainBars()
{
	bool IsOK = true;
	CString mainBars = _T("");
	if (mp_CompoM == nullptr)	 IsOK = false;
	if(mp_SectionBox == nullptr)	IsOK = false;
	if(mp_SectionBox->mp_Section == nullptr)	IsOK = false;
	int nEdge = mp_SectionBox->mp_Section->ma_Edge.GetSize();
	if(nEdge != 4)	IsOK = false;
	if(mp_SectionBox->mp_MainBar1 == NULL)	IsOK = false;

	if (IsOK)	mainBars = mp_SectionBox->mp_MainBar1->ma_strBar[0];
	auto mainBarResult = AddResultItem(MTResultItem::MainBars, mainBars);

	int TotalBars = 0;
	CString sDiaName = _T("");
	if (IsOK == false)
	{
		mainBarResult->SetItemState(MTResultItem::Error);
	}
	else
	{
		TotalBars = mp_SectionBox->mp_MainBar1->GetTotalBars();
		sDiaName = mp_SectionBox->mp_MainBar1->GetBarDiameterName();
	}
	MSCompoM* pColumnM = mp_CompoM;
	double dDia = _ttof(sDiaName);

	long nBotBar = mp_SectionBox->mp_Section->ma_Edge[0]->GetNumVBarOfEdge();
	long nRightBar = mp_SectionBox->mp_Section->ma_Edge[1]->GetNumVBarOfEdge();
	long nTopBar = mp_SectionBox->mp_Section->ma_Edge[2]->GetNumVBarOfEdge();
	long nLeftBar = mp_SectionBox->mp_Section->ma_Edge[3]->GetNumVBarOfEdge();

	CString TBBars, LRBars;
	TBBars.Format(_T("%ld-HD%s"), nTopBar, sDiaName);
	LRBars.Format(_T("%ld-HD%s"), nLeftBar, sDiaName);

	auto TBBarResult = AddResultItem(MTResultItem::TBBars, TBBars);
	auto LRBarResult = AddResultItem(MTResultItem::LRBars, LRBars);
	int maxTBBar = max(nTopBar, nBotBar);
	int maxLRBar = max(nLeftBar, nRightBar);
	if (nTopBar != nBotBar)		TBBarResult->SetItemState(MTResultItem::Error);
	if (nLeftBar != nRightBar)	LRBarResult->SetItemState(MTResultItem::Error);
	
	if ((maxTBBar*2 + maxLRBar*2) != TotalBars)
	{
		MTResultItem::eItemState stateTB = TBBarResult->GetItemState();
		MTResultItem::eItemState stateLR = LRBarResult->GetItemState();
		if (stateTB == MTResultItem::OK && stateLR == MTResultItem::OK)
		{
			TBBarResult->SetItemState(MTResultItem::Error);
			LRBarResult->SetItemState(MTResultItem::Error);
		}
		else if (stateTB == MTResultItem::OK)
			LRBarResult->SetItemState(MTResultItem::Error);
		else if (stateLR == MTResultItem::OK)
			TBBarResult->SetItemState(MTResultItem::Error);
	}
	CString errorText;
	if (CheckDia(dDia) == false)
	{
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_REBAR, GetCompoMName());
		AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctMainBar));
		mainBarResult->SetItemState(MTResultItem::Error);
		TBBarResult->SetItemState(MTResultItem::Error);
		LRBarResult->SetItemState(MTResultItem::Error);
	}
	if (TBBarResult->GetItemState() == MTResultItem::Error || LRBarResult->GetItemState() == MTResultItem::Error)
	{
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_REBAR, GetCompoMName());
		AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctMainBar));
	}

#pragma region --- 체크함수 제외

// 	if(TotalBars != nBotBar + nRightBar + nTopBar + nLeftBar)
// 	{
// 		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_MAINBAR_NUM, GetCompoMName(), mp_SectionBox->GetOwnerRect());
// 		AddErrorFigureID(mp_SectionBox->GetTextFigureID(MTReader::ctMainBar));
// 		mainBarResult->SetItemState(MTResultItem::Error);
// 		TBBarResult->SetItemState(MTResultItem::Error);
// 		LRBarResult->SetItemState(MTResultItem::Error);
// 	}
// 
// 	if(nTopBar != nBotBar || nLeftBar != nRightBar)
// 	{
// 		if (nTopBar != nBotBar)
// 		{
// 			CreateErrorObject(GetGroupName(), ERROR_LEVEL, TOPBAR_BOTBAR_DIFF, GetCompoMName(), mp_SectionBox->GetOwnerRect());
// 			TBBarResult->SetItemState(MTResultItem::Error);
// 		}
// 		if (nLeftBar != nRightBar)
// 		{
// 			CreateErrorObject(GetGroupName(), ERROR_LEVEL, LEFTBAR_RIGHTBAR_DIFF, GetCompoMName(), mp_SectionBox->GetOwnerRect());
// 			LRBarResult->SetItemState(MTResultItem::Error);
// 		}
// 		AddErrorFigureID(mp_SectionBox->GetTextFigureID(MTReader::ctMainBar));
// 		IsOK = false;
// 	}
#pragma endregion


	return IsOK;
}

bool MTRcColumnMHelper::CheckRect_MainBar2Layer(MSMainBarSet* pTBar, MSMainBarSet* pLBar)
{
	bool IsOK = true;
	if(pTBar->GetNumSets() == 2)
	{
		auto TBBarResult = GetResultItem(MTResultItem::TBBars);
		int num1LayerBar = pTBar->GetNumBarsOnSet(0);
		int num2LayerBar = pTBar->GetNumBarsOnSet(1);
		if(num2LayerBar > num1LayerBar)
		{
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_REBAR, GetCompoMName());
			AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctMainBar));

			TBBarResult->SetItemState(MTResultItem::Error);
		}
	}
	if(pLBar->GetNumSets() == 2)
	{
		auto LRBarResult = GetResultItem(MTResultItem::LRBars);

		int num1LayerBar = pLBar->GetNumBarsOnSet(0);
		int num2LayerBar = pLBar->GetNumBarsOnSet(1);
		if(num2LayerBar > num1LayerBar)
		{
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_REBAR, GetCompoMName());
			AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctMainBar));

			LRBarResult->SetItemState(MTResultItem::Error);
		}
	}

	return IsOK;
}

bool MTRcColumnMHelper::CheckRect_MainBarMinSpacing(MSMainBarSet* pTBar, MSMainBarSet* pLBar, double CvrConc)
{
	MSRcLineMemberM* pColumnM = (MSRcLineMemberM*)mp_CompoM;

	bool IsOK = true;
	double pureWidth = pColumnM->GetWidth()-(CvrConc*2);
	CString sMainBarDiaName = _T("0");

	long TopBars = pTBar->GetNumBarsOnSet(0);
	long LeftBars = pLBar->GetNumBarsOnSet(0);
	if(TopBars > 1)
	{
		auto TBBarResult = GetResultItem(MTResultItem::TBBars);

		double space = pureWidth/(TopBars-1);
		double dMainBarDia = _ttof(pTBar->GetMaxDiaNameOnSet(0));

		MSConcrete* pConc = (MSConcrete*)pColumnM->GetMaterial();
		double checkValue = max(dMainBarDia, 25);
		checkValue = max(checkValue, pConc->m_Aggregate*4/3);
		if(space < checkValue)
		{
			IsOK = false;
			CString errorMessage;
			errorMessage.Format(_T("%s %s"), GetCompoMName(), LocalDic::GetLocal(_T("주근의 최소 간격"), _T("SectionRecognize")));
			CreateErrorObject(GetGroupName(), WARNING_LEVEL, ERROR_INFO, errorMessage);
			TBBarResult->SetItemState(MTResultItem::Error);
		}
	}
	if(IsOK == true && LeftBars > 1)
	{
		auto LRBarResult = GetResultItem(MTResultItem::LRBars);

		double pureDepth = pColumnM->GetDepth()-(CvrConc*2);
		double space = pureDepth/(LeftBars-1);
		double dMainBarDia = _ttof(pLBar->GetMaxDiaNameOnSet(0));

		MSConcrete* pConc = (MSConcrete*)pColumnM->GetMaterial();
		double checkValue = max(dMainBarDia, 25);
		checkValue = max(checkValue, pConc->m_Aggregate*4/3);
		if(space < checkValue)
		{
			CString errorMessage;
			errorMessage.Format(_T("%s %s"), GetCompoMName(), LocalDic::GetLocal(_T("주근의 최소 간격"), _T("SectionRecognize")));
			CreateErrorObject(GetGroupName(), WARNING_LEVEL, ERROR_INFO, errorMessage);
			LRBarResult->SetItemState(MTResultItem::Error);
		}
	}

	return IsOK;
}

bool MTRcColumnMHelper::CheckHoopBarMinSpacing(MSMainBarSet* pTBar, MSStirrupBar* pCenHoopBar)
{
	auto cenHoopResult = GetResultItem(MTResultItem::CenHoop);
	bool IsOK = true;

	CString sDiaName = pTBar->GetMaxDiaNameOnSet(0);
	double dMainBarDia = _ttof(sDiaName);

	CString sCenHoopDiaName = pCenHoopBar->GetBarDiameterName();
	double dCenHoopBarDia = _ttof(sCenHoopDiaName);
	double cenSpacing = pCenHoopBar->GetSpacing();
	if(cenSpacing > min(mp_CompoM->GetWidth(), mp_CompoM->GetDepth()))
		IsOK = false;
	if(cenSpacing > max(dMainBarDia*16, dCenHoopBarDia*48))
		IsOK = false;

	if(IsOK == false)
	{
		CString errorMessage;
		errorMessage.Format(_T("%s %s "), GetCompoMName(), LocalDic::GetLocal(_T("대근의 수직 간격"), _T("SectionRecognize")));
		CreateErrorObject(GetGroupName(), WARNING_LEVEL, ERROR_INFO, errorMessage);
		cenHoopResult->SetItemState(MTResultItem::Error);
	}

	return IsOK;
}
/*
void MTRcColumnMHelper::ParsingHoopBars(MSStirrupBar* pStirrupBar, CString& cenHoop, CString& endHoop)
{
	cenHoop = _T("");
	endHoop = _T("");
	if (pStirrupBar == nullptr)	return;
	if (mp_Reader->m_eRecognizeEndHoopType == MTReader::CenHoop)
	{
		cenHoop = endHoop = pStirrupBar->GetStirrupBarString();
	}
	else
	{
//		CString text = pStirrupBar->GetStirrupBarString();
		CString text = pStirrupBar->ma_strBar[0];
		int indexParentheses1 = text.Find(_T("("));
		int indexParentheses2 = text.Find(_T(")"));
		if (indexParentheses1 != -1 && indexParentheses2 != -1)
		{
			CString firstBar = _T(""), secondBar = _T("");
			firstBar = text.Mid(0, indexParentheses1);
			secondBar = text.Mid(indexParentheses1 + 1, indexParentheses2 - (indexParentheses1 + 1));

			if (mp_Reader->m_eRecognizeEndHoopType == MTReader::CenEndHoop)
			{
				cenHoop = firstBar;
				endHoop = secondBar;
			}
			else if (mp_Reader->m_eRecognizeEndHoopType == MTReader::CenEndHoop)
			{
				cenHoop = secondBar;
				endHoop = firstBar;
			}
		}
		else
			cenHoop = text;
	}
}
*/
bool MTRcColumnMHelper::ParsingHoopBars(SRSectionBox* pBox, CString& cenHoop, CString& endHoop)
{
	bool result = false;
	cenHoop = _T("");
	endHoop = _T("");

	auto cenBars = pBox->ma_StirrupBar1;
	auto endBars = pBox->ma_StirrupBar2;
	if (cenBars.size() == 0 && endBars.size() == 0)
		return false;

	if (cenBars.size() > 0 && endBars.size() > 0)
	{
		cenHoop = cenBars[0];
		endHoop = endBars[0];

		if (cenBars.size() == 1 && endBars.size() == 1)
			result = true;
		else
			result = false;

		MSStirrupBar cenBar(cenHoop);
		auto diaName = cenBar.GetBarDiameterName();
		long cenSpacing = (long)cenBar.GetSpacing();
		cenHoop.Format(_T("HD%s@%ld"), diaName, cenSpacing);
		MSStirrupBar endBar(endHoop);
		long endSpacing = (long)endBar.GetSpacing();
		endHoop.Format(_T("HD%s@%ld"), diaName, endSpacing);
	}

	if (endBars.size() == 0)
		result = AnalyizeStirrupBar(pBox->ma_StirrupBar1, cenHoop, endHoop);
	if(cenBars.size() == 0)
		result = AnalyizeStirrupBar(pBox->ma_StirrupBar2, cenHoop, endHoop);

	return result;
}
bool MTRcColumnMHelper::AnalyizeStirrupBar(vector<CString> textList, CString& cenHoop, CString& endHoop)
{
	int numText = textList.size();
	if (numText == 0)	return false;

	CString firstText = _T("");
	CString secondtext = _T("");
	if (numText == 1)
	{
		auto sText = textList[0];
		AnalyizeText(sText, cenHoop, endHoop);
		return true;
	}
	else
	{
		if (mp_Reader->m_eRecognizeEndHoopType == MTReader::CenEndHoop)
		{
			cenHoop = textList[0];
			endHoop = textList[1];
		}
		else if (mp_Reader->m_eRecognizeEndHoopType == MTReader::EndCenHoop)
		{
			cenHoop = textList[1];
			endHoop = textList[0];
		}
		MSStirrupBar cenBar(cenHoop);
		auto diaName = cenBar.GetBarDiameterName();
		long cenSpacing = (long)cenBar.GetSpacing();
		cenHoop.Format(_T("HD%s@%ld"), diaName, cenSpacing);
		MSStirrupBar endBar(endHoop);
		long endSpacing = (long)endBar.GetSpacing();
		endHoop.Format(_T("HD%s@%ld"), diaName, endSpacing);

		if (numText == 2)	return true;
	}
	return false;
}
void MTRcColumnMHelper::AnalyizeText(CString sBarText, CString& cenHoop, CString& endHoop)
{
	cenHoop = _T("");
	endHoop = _T("");
	CString text = sBarText;
	int indexParentheses1 = text.Find(_T("("));
	int indexParentheses2 = text.Find(_T(")"));
	if (indexParentheses1 != -1 && indexParentheses2 != -1)
	{
		CString firstBar = _T(""), secondBar = _T("");
		firstBar = text.Mid(0, indexParentheses1);
		CString secondBarSpacing = text.Mid(indexParentheses1 + 1, indexParentheses2 - (indexParentheses1 + 1));

		MSStirrupBar stirrupBar(firstBar);
		auto diaName = stirrupBar.GetBarDiameterName();
		secondBar.Format(_T("HD%s@%s"), diaName, secondBarSpacing);

		if (mp_Reader->m_eRecognizeEndHoopType == MTReader::CenEndHoop)
		{
			cenHoop = firstBar;
			endHoop = secondBar;
		}
		else if (mp_Reader->m_eRecognizeEndHoopType == MTReader::EndCenHoop)
		{
			cenHoop = secondBar;
			endHoop = firstBar;
		}
	}
	else
		cenHoop = text;
}
bool MTRcColumnMHelper::MakeStirrupBars(MSStirrupBar* pCenBar, MSStirrupBar* pEndBar)
{
	CString cenHoopBars = _T(""), endHoopBars = _T("");

	bool result = ParsingHoopBars(mp_SectionBox, cenHoopBars, endHoopBars);

	auto txtCenHoop = mp_SectionBox->m_sStirrupBar1;
	auto txtEndHoop = mp_SectionBox->m_sStirrupBar2;
	if (result == true)
	{
		txtCenHoop = cenHoopBars;
		txtEndHoop = endHoopBars;
	}
	auto cenHoopResult = AddResultItem(MTResultItem::CenHoop, txtCenHoop);
	auto endHoopResult = AddResultItem(MTResultItem::EndHoop, txtEndHoop);

	bool IsCenHoop = false, IsEndHoop = false;

	bool IsOK = true;
	if(cenHoopBars != _T(""))
	{
		if(cenHoopBars.Find('@') == -1)
		{
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_REBAR, GetCompoMName());
			AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctSBar1));
			cenHoopResult->SetItemState(MTResultItem::Error);
		}
		MSStirrupBar tempBar;
		tempBar.InitData(cenHoopBars);
		CString sDiaName = tempBar.GetBarDiameterName();
		int nSpacing = (int)tempBar.GetSpacing();
		CString theBar;
		theBar.Format(_T("D%s@%ld"), sDiaName, nSpacing);
		pCenBar->InitData(theBar);
        if(cenHoopResult->GetItemText() == _T(""))
            cenHoopResult->SetItemText(theBar);

		double dDia = _ttof(sDiaName);
		if(CheckDia(dDia) == false)
		{
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_REBAR, GetCompoMName());
			AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctSBar1));
			cenHoopResult->SetItemState(MTResultItem::Error);
		}
		else
			IsCenHoop = true;
	}

	if(endHoopBars != _T(""))	// 상,하부(단부)
	{
		if(endHoopBars.Find('@') == -1)
		{
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_REBAR, GetCompoMName());
			AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctSBar2));
			endHoopResult->SetItemState(MTResultItem::Error);
		}

		MSStirrupBar tempBar;
		tempBar.InitData(endHoopBars);
		CString sDiaName = tempBar.GetBarDiameterName();
		int nSpacing = (int)tempBar.GetSpacing();
		CString theBar;
		theBar.Format(_T("D%s@%ld"), sDiaName, nSpacing);
		pEndBar->InitData(theBar);

        if (endHoopResult->GetItemText() == _T(""))
            endHoopResult->SetItemText(theBar);

		double dDia = _ttof(sDiaName);
		if(CheckDia(dDia) == false)
		{
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_REBAR, GetCompoMName());
			AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctSBar2));
			endHoopResult->SetItemState(MTResultItem::Error);
		}
		else
			IsEndHoop = true;
	}

	if(IsCenHoop == false && IsEndHoop == false)	// RH-127
	{
		CString errorMessage;
		errorMessage.Format(_T("%s %s"), GetCompoMName(), LocalDic::GetLocal(_T("스터럽바"), _T("SectionRecognize")));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_REBAR, errorMessage);
		cenHoopResult->SetItemState(MTResultItem::Error);
		endHoopResult->SetItemState(MTResultItem::Error);
	}
	else if (IsCenHoop && IsEndHoop)
	{
		double cenSpacing = pCenBar->GetSpacing();
		double endSpacing = pEndBar->GetSpacing();
		if (cenSpacing < endSpacing)
		{
			CString errorMessage;
			errorMessage.Format(_T("%s %s"), GetCompoMName(), LocalDic::GetLocal(_T("띠철근(배근 간격)"), _T(" SectionRecognize")));
			CreateErrorObject(GetGroupName(), WARNING_LEVEL, ERROR_REBAR, errorMessage);
			AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctSBar1));
			AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctSBar2));
			cenHoopResult->SetItemState(MTResultItem::Error);
			endHoopResult->SetItemState(MTResultItem::Error);
		}
	}

 	if((IsCenHoop && !IsEndHoop) || (!IsCenHoop && IsEndHoop))
 	{
 		MSStirrupBar* pStrpBar = nullptr;
 		if(mp_SectionBox->mp_StrpBar1)
 			pStrpBar = mp_SectionBox->mp_StrpBar1;
 		else
 			pStrpBar = mp_SectionBox->mp_StrpBar2;

		if (pStrpBar != nullptr)
		{
			MSStirrupBar tempBar;
			tempBar.InitData(pStrpBar->GetStirrupBarString());
			CString sDiaName = pStrpBar->GetBarDiameterName();
			int nSpacing = (int)tempBar.GetSpacing();
			CString theBar;
			theBar.Format(_T("HD%s@%ld"), sDiaName, nSpacing);
			pCenBar->InitData(theBar);
			pEndBar->InitData(theBar);

			cenHoopResult->SetItemText(theBar);
			endHoopResult->SetItemText(theBar);
		}
 	}

	return IsOK;
}

bool MTRcColumnMHelper::MakeRect_TieBarCount(long& rowTie, long& colTie, long nTopBars, long nLeftBars)
{
	auto xTieBarResult = AddResultItem(MTResultItem::XTieBars, _T("0"));
	auto yTieBarResult = AddResultItem(MTResultItem::YTieBars, _T("0"));
	bool IsOK = true;

	if (mp_SectionBox->ma_SpecificTieBar.size() > 0)
	{
		rowTie = 0; colTie = 0;
		return IsOK;
	}
	if (mp_SectionBox->mp_Section == nullptr)	return false;
	int rowTieBar = 0, colTieBar = 0;
	int NumTieBar = mp_SectionBox->mp_Section->ma_TieBar.GetSize();
	for(int iTieBar = 0; iTieBar <  NumTieBar;  iTieBar++)
	{
		MRHBarByPoint* pTieBar = mp_SectionBox->mp_Section->ma_TieBar[iTieBar];
		long NumPoint = pTieBar->ma_Point.GetSize();
		for(long iPoint = 0; iPoint < NumPoint; iPoint++)
		{
			MRHBarPointByLoc* pTiePoint = pTieBar->ma_Point[iPoint];
            if (pTiePoint->m_EdgeIdx == 0)// || pTiePoint->m_EdgeIdx==2)
            {
                colTieBar++;
                break;
            }
            else if (pTiePoint->m_EdgeIdx == 1)
            {
                rowTieBar++;
                break;
            }
		}
	}

	rowTie = rowTieBar;
	colTie = colTieBar;

	CString sRowTie, sColTie;
	sRowTie.Format(_T("%ld"), rowTie);
	sColTie.Format(_T("%ld"), colTie);

	xTieBarResult->SetItemText(sRowTie);
	yTieBarResult->SetItemText(sColTie);
	if(colTieBar > nTopBars - 2)
		IsOK = false;
	if(rowTieBar > nLeftBars)
		IsOK = false;

	if(IsOK == false)
	{
		CString errorMessage;
		errorMessage.Format(_T("%s %s "), GetCompoMName(), LocalDic::GetLocal(_T("보조대근"), _T("SectionRecognize")));
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
		xTieBarResult->SetItemState(MTResultItem::Error);
		yTieBarResult->SetItemState(MTResultItem::Error);
	}

	return IsOK;
}

bool MTRcColumnMHelper::MakeRect_TieBars(MSStirrupBar* pRowCenBar, MSStirrupBar* pColCenBar, MSStirrupBar* pRowEndBar, MSStirrupBar* pColEndBar, MSStirrupBar* pCenHoop, MSStirrupBar* pEndHoop)
{
	// Tiebar 간격은 hoop로 설정
	CString cenHoop = pCenHoop->GetStirrupBarString();
	pRowCenBar->InitData(cenHoop);
	pColCenBar->InitData(cenHoop);
	CString endHoop = pEndHoop->GetStirrupBarString();
	pRowEndBar->InitData(endHoop);
	pColEndBar->InitData(endHoop);

	return true;
}

bool MTRcColumnMHelper::MakeDoubleHoop(BOOL& IsDoubleHoop)
{
	bool IsDouble = true;
	if (mp_SectionBox->mp_Section == nullptr)	return false;
	int nEdge = mp_SectionBox->mp_Section->ma_Edge.GetSize();
	for(int iEdge = 0; iEdge < nEdge; iEdge++)
	{
		MREdge* pEdge = mp_SectionBox->mp_Section->ma_Edge[iEdge];
		long nLayer = pEdge->ma_VBarSet.GetSize();
		if(nLayer >= 2)
		{
			MRVBar* pLayer = pEdge->ma_VBarSet[1];
			if(pLayer->mp_StrpCurve == NULL)
			{
				IsDouble = false;
				break;
			}
		}
		else
		{
			IsDouble = false;
			break;
		}
	}

	if(IsDouble)
		IsDoubleHoop = TRUE;
	else
		IsDoubleHoop = FALSE;

	return true;
}

bool MTRcColumnMHelper::MakeSpecificTieBars(vector<MSSpecificTieBar*>& SpecificTieBarArr)
{
	CString fullName = _T("");
	auto specificTieResult = AddResultItem(MTResultItem::SpecificTieBars, fullName);

	int nSTieBar = mp_SectionBox->ma_SpecificTieBar.size();
	for(int i = 0; i < nSTieBar; i++)
	{
		MSSpecificTieBar* pRecognizeTieBar = mp_SectionBox->ma_SpecificTieBar[i];

		for (int j = 0; j < SpecificTieBarArr.size(); j++)
		{
			if (pRecognizeTieBar->m_sLoc != SpecificTieBarArr[j]->m_sLoc) continue;

			CString errorMessage;
			errorMessage.Format(_T("%s %s "), GetCompoMName(), LocalDic::GetLocal(_T("상세 철근"), _T("Chang.RebarHUBCommon")));
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);

			specificTieResult->SetItemState(MTResultItem::Error);
// 			return false;
		}
		MSSpecificTieBar* pTieBar = new MSSpecificTieBar();
		pTieBar->CopyFromMe(pRecognizeTieBar);
		SpecificTieBarArr.push_back(pTieBar);

		if (i == nSTieBar - 1)
			fullName += pRecognizeTieBar->m_sLoc;
		else
			fullName += pRecognizeTieBar->m_sLoc + _T(",");
	}
	specificTieResult->SetItemText(fullName);
	return true;
}

bool MTRcColumnMHelper::MakeSubMainBars(MSSubMainBar* pSubMainBar)
{
	if (mp_SectionBox->mp_SubMainBar != nullptr)
	{
		CString diaName = mp_SectionBox->mp_SubMainBar->GetBarDiameterName();
		CString strBar;

		if (CheckDia(_ttof(diaName)) == FALSE)
		{
			CString errorMessage;
			errorMessage.Format(CFormatMsg(LocalDic::GetLocal(_T("%1 보조주근 직경"), _T(" SectionRecognize")), mp_CompoM->m_Name));
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
			AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctSBar2));
			return false;
		}
		strBar.Format(_T("%ld-%s%s"), pSubMainBar->ma_Loc.size(), MSDeformedBar::m_sDefaultBarType, diaName);
		pSubMainBar->m_MainBar.InitData(strBar);
	}
	GM2DPolygon SecPGon(TRUE);

	auto textSubMainResult = AddResultItem(MTResultItem::TextSubMainBar, mp_SectionBox->m_sSubMainBar);
	if (mp_SectionBox->mp_Section == nullptr)	return false;
	int numEdge = mp_SectionBox->mp_Section->ma_Edge.GetSize();
	for(int iEdge = 0; iEdge < numEdge; iEdge++)
	{
		if(iEdge == 0 || iEdge == 3)
			SecPGon.AddCopy(mp_SectionBox->mp_Section->ma_Edge[iEdge]->m_P0);
		else
			SecPGon.AddCopy(mp_SectionBox->mp_Section->ma_Edge[iEdge]->m_P1);
	}
	GM2DVector cenVec = SecPGon.MassCenter();
	int nSubBar = mp_SectionBox->ma_SubMainBar.GetSize();
	vector<GM2DVector> locArr;
	for(int i = 0; i < nSubBar; i++)
	{
		GMCircle* pCircle = mp_SectionBox->ma_SubMainBar[i];
		GM2DVector* pLocVec = new GM2DVector(pCircle->m_Center.m_X, pCircle->m_Center.m_Y);
		pLocVec->m_X = pCircle->m_Center.m_X-cenVec.m_X;
		pLocVec->m_Y = pCircle->m_Center.m_Y-cenVec.m_Y;
		pSubMainBar->ma_Loc.push_back(pLocVec);
		GM2DVector loc(*pLocVec);
		locArr.push_back(loc);
	}
	CString diaName = pSubMainBar->m_MainBar.GetBarDiameterName();
	if (mp_SectionBox->mp_SubMainBar != nullptr)
	{
		diaName = mp_SectionBox->mp_SubMainBar->GetBarDiameterName();
	}
	else
	{
		diaName  = MSDeformedBar::GetBarDiameterName(mp_SectionBox->m_sSubMainBar);
	}
	CString strBar;
	strBar.Format(_T("%ld-HD%s"), pSubMainBar->ma_Loc.size(), diaName);
	pSubMainBar->m_MainBar.InitData(strBar);
	auto subMainResult = AddResultItem(MTResultItem::SubMainBars, strBar);
	SetLocationList(MTResultItem::SubMainBars, locArr);

	bool resultSubMainBar = true;
	auto txtSubData = mp_SectionBox->m_sSubMainBar;
	auto subData = pSubMainBar->m_MainBar.GetTotalMainBarString();
	if (txtSubData == "" || txtSubData.Find('-') == -1 || txtSubData.Find('D') == -1)
	{
		if (txtSubData.Find('D') != -1 && txtSubData.Find('-') == -1)
		{
			diaName = MSDeformedBar::GetBarDiameterName(txtSubData);
			txtSubData.Format(_T("0-HD%s"), diaName);
		}
		else
		{
			if (locArr.size() > 0)
			{
				subMainResult->SetItemState(MTResultItem::Error);
				return false;
			}
			return true;
		}
	}

	MSMainBar tempBar(txtSubData);
	auto txtNumBar = tempBar.GetTotalBars();
	auto txtDia = tempBar.GetBarDiameterName();

	CString fixedSubMainBar;
	fixedSubMainBar.Format(_T("%ld-HD%s"), txtNumBar, txtDia);
	textSubMainResult->SetItemText(fixedSubMainBar);

	MSMainBar txtSubMainBar(fixedSubMainBar);
	MSMainBar subMainBar(subData);
	if (txtSubMainBar.GetTotalBars() != subMainBar.GetTotalBars()) resultSubMainBar = false;
	if (txtSubMainBar.GetBarDiameterName() != subMainBar.GetBarDiameterName()) resultSubMainBar = false;
	if (!resultSubMainBar) 	subMainResult->SetItemState(MTResultItem::Error);


	return true;
}

bool MTRcColumnMHelper::MakeRound_MainBars(MSMainBarSet* pTBar)
{
	MSRcColumnM* pRebarM = (MSRcColumnM*)mp_CompoM->GetRebarM();

	CheckRound_MainBars();

	if (mp_SectionBox->mp_MainBar1 == nullptr)	return false;
	CString mainBars = mp_SectionBox->mp_MainBar1->ma_strBar[0];
	auto mainBarResult = AddResultItem(MTResultItem::MainBars, mainBars);

	CString theBar;

	MSMainBar tempBar;
	tempBar.InitData(mp_SectionBox->mp_MainBar1->GetstrLayerOnALayer(0));
	CString sDiaName = tempBar.GetBarDiameterName();

	if (mp_SectionBox->mp_Section == nullptr)	return false;
	if (mp_SectionBox->mp_Section->ma_Edge.GetSize() == 0)	return false;

	MREdge* pCircleEdge = mp_SectionBox->mp_Section->ma_Edge[0];
	pRebarM->m_TBars.RemoveAll();
	long nLayer = pCircleEdge->ma_VBarSet.GetSize();
	for(int iLayer = nLayer-1; iLayer >= 0 ; iLayer--)
	{
		MRVBar* pLayer = pCircleEdge->ma_VBarSet[iLayer];
		int nBar = pLayer->ma_VBarLoc.GetSize();
		theBar.Format(_T("%ld-%s%s"), nBar, MSDeformedBar::m_sDefaultBarType, sDiaName);
		pTBar->Add(theBar);
	}
	auto TBBarResult = GetResultItem(MTResultItem::TBBars);
	CString barText = pTBar->GetBarString();
	TBBarResult->SetItemText(barText);
	return true;
}

bool MTRcColumnMHelper::CheckRound_MainBars()
{
	auto TBBars = GetResultItem(MTResultItem::TBBars);
	if(mp_SectionBox->mp_MainBar1 == NULL)	return false;
	if(mp_SectionBox->mp_Section == NULL)	return false;

	bool IsOK = true;
	MSMainBar tempBar;
	tempBar.InitData(mp_SectionBox->mp_MainBar1->GetstrLayerOnALayer(0));
	CString sDiaName = tempBar.GetBarDiameterName();
	int nBar = (int)tempBar.GetTotalBars();
	CString theBar;
	int nEdge = mp_SectionBox->mp_Section->ma_Edge.GetSize();
	if(nEdge == 0)	return false;

	MREdge* pCircleEdge = mp_SectionBox->mp_Section->ma_Edge[0];
	int nReadBars = pCircleEdge->GetNumVBarOfEdge();
	if(nBar != nReadBars)
	{
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_MAINBAR_NUM, GetCompoMName());
		AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctMainBar));
		TBBars->SetItemState(MTResultItem::Error);
		CString errorText;
		CString mainBars = mp_SectionBox->mp_MainBar1->ma_strBar[0];
		TBBars->SetItemText(errorText);
	}

	double dDia = _ttof(sDiaName);
	if(CheckDia(dDia) == false)
	{
		CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_MAINBAR_NUM, GetCompoMName());
		AddErrorFigureIDs(mp_SectionBox->GetTextFigureIDs(MTReader::ctMainBar));
		TBBars->SetItemState(MTResultItem::Error);
	}

	return IsOK;
}

bool MTRcColumnMHelper::MakeRound_TieBars(long& rowTieBar, long& colTieBar, MSMainBarSet* pTBar)
{
	auto xTieResult = GetResultItem(MTResultItem::XTieBars);
	auto yTieResult = GetResultItem(MTResultItem::YTieBars);
	if (mp_SectionBox->mp_Section == nullptr)	return false;
	long TieBarNum =mp_SectionBox->mp_Section->ma_TieBar.GetSize();
	rowTieBar = TieBarNum;
	colTieBar = 0;

	CString sRowTie;
	sRowTie.Format(_T("%ld"), rowTieBar);

	xTieResult->SetItemText(sRowTie);
	yTieResult->SetItemText(_T(""));

	long TopBarNum = pTBar->GetNumBarsOnSet(0);
	if(TieBarNum != 0)
	{
		if( TopBarNum / 2 < TieBarNum)
		{
			CString errorMessage;
			errorMessage.Format(_T("%s %s "), GetCompoMName(), LocalDic::GetLocal(_T("보조대근"), _T("SectionRecognize")));
			CreateErrorObject(GetGroupName(), ERROR_LEVEL, ERROR_INFO, errorMessage);
			xTieResult->SetItemState(MTResultItem::Error);
			yTieResult->SetItemState(MTResultItem::Error);
		}
	}
	return true;
}