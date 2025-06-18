#include "StdAfx.h"
#include "MTSrcColumnMHelper.h"

#include "SRSectionBox.h"

#include "StructureModel/MSRcLineMemberM.h"
#include "StructureModel/MSSrcColumnM.h"
#include "StructureModel/MSSteelUtility.h"

#include "StructureModel/MDSrcColumnM.h"


MTSrcColumnMHelper::MTSrcColumnMHelper(MSBaseBuilding* pBldg, MTReader* pReader) : MTRcColumnMHelper(pBldg, pReader)
{
}


MTSrcColumnMHelper::~MTSrcColumnMHelper(void)
{
}

MSCompoM* MTSrcColumnMHelper::MakeData(CString Name, SRSectionBox* pSectionBox)
{
	mp_ResultElement->SetArea(pSectionBox->GetOwnerRect());

	CheckName(Name, pSectionBox->GetTextFigureIDs(MTReader::ctName));

	mp_CompoM = mp_Bldg->CreateNewCompoM(MSCompoM::msSrcColumnM);
	mp_CompoM->mp_RebarM = MSCompoM::CreateRebarM(MSCompoM::msSrcColumnM);
	mp_CompoM->m_Name = Name;
	mp_SectionBox = pSectionBox;
	pSectionBox->GetFromToStory(mp_CompoM->m_nFromStory, mp_CompoM->m_nToStory);

	MSMaterial* pDefMaterial = mp_Bldg->GetMaterial(mp_Reader->m_nMaterialID);
	if (pDefMaterial == NULL)	pDefMaterial = mp_Bldg->GetDefMaterial(MSMaterial::msSRC);
	MSMaterial* pMaterial = mp_Bldg->GetMaterial(pSectionBox->m_nMaterialID);
	if (pMaterial != nullptr)	mp_CompoM->SetMaterial(pMaterial);
	else
		mp_CompoM->SetMaterial(pDefMaterial);

	mp_CompoM->SetIsUsedStoryMaterial(mp_Reader->m_IsUsedStoryMaterial);
	((MSSrcColumnM*)mp_CompoM)->SetShape((MSSrcColumnM::SECT_SHAPE)mp_SectionBox->GetSectionType());
	long Type = mp_SectionBox->GetSectionType();
	CString sShape = GetRectangleText();
	if (Type == MSRcLineMemberM::CIRCLE) 	sShape = GetCircleText();
	AddResultItem(MTResultItem::Shape, sShape);

	CheckStory();

// 	MSSteelUtility* pUtil = mp_Bldg->GetSteelUtili();
// 	long nSectionType = pUtil->GetSectionType(pSectionBox->m_SteelSectionName);
// 	MSSteelSection* pSteelSection = pUtil->GetSection(nSectionType, pSectionBox->m_SteelSectionName);
// 	if(pSteelSection == NULL)
// 	{
// 		vector<double> params = Parsing_SteelName(pSectionBox->m_SteelSectionName);
// 		pSteelSection = pUtil->BuiltUpSteelSection(nSectionType, params);
// 		if(pSteelSection == NULL)	return NULL;
// 
// 		mp_Reader->m_IsMakeBuiltUp = true;
// 	}
	AddResultItem(MTResultItem::SteelSection, pSectionBox->m_SteelSectionName);

	MSSrcColumnM* pColumnM = (MSSrcColumnM*)mp_CompoM;
// 	pColumnM->m_nSecType = nSectionType;
// 	pColumnM->m_pSection = pSteelSection;
	Parsing_StudBolt(pSectionBox->m_StudBolt, pColumnM->m_NumStudBoltBars, &pColumnM->m_StudBoltBars);

	if(Type == MSRcLineMemberM::RECT)					MakeSrcRect_Data();
	else if(Type == MSRcLineMemberM::CIRCLE)		MakeSrcRound_Data();
	else if(Type == MSRcLineMemberM::POLYGON)	MakeSrcPolygon_Data();
	else
		m_IsMaked = false;

	return mp_CompoM;
// 	if(m_IsMaked)	return mp_CompoM;
// 	else
// 	{
// 		delete mp_CompoM;
// 		return NULL;
// 	}
}

void MTSrcColumnMHelper::MakeSrcRect_Data()
{
	MSSrcColumnM* pColumnM = (MSSrcColumnM*)mp_CompoM;
	MDSrcColumnM* pRebarM = (MDSrcColumnM*)pColumnM->GetRebarM();

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

//	CheckHoopBarMinSpacing(&pRebarM->m_TBars, &pRebarM->m_CenHoopBar)				// 대근 최소 간격

	MakeDoubleHoop(pRebarM->m_nDoubleHoopType);
	MakeSpecificTieBars(pRebarM->ma_SpecificTieBar);
	MakeSubMainBars(&pRebarM->m_SubMainBar);
}

void MTSrcColumnMHelper::MakeSrcRound_Data()
{
	MSSrcColumnM* pColumnM = (MSSrcColumnM*)mp_CompoM;
	MDSrcColumnM* pRebarM = (MDSrcColumnM*)pColumnM->GetRebarM();

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

//	CheckHoopBarMinSpacing(&pRebarM->m_TBars, &pRebarM->m_CenHoopBar)				// 대근 최소 간격

	MakeDoubleHoop(pRebarM->m_nDoubleHoopType);
	MakeSpecificTieBars(pRebarM->ma_SpecificTieBar);
	MakeSubMainBars(&pRebarM->m_SubMainBar);
}

void MTSrcColumnMHelper::MakeSrcPolygon_Data()
{
	m_IsMaked = false;
}

