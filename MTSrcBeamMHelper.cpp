#include "StdAfx.h"
#include "MTSrcBeamMHelper.h"
#include "SRSectionTable.h"
#include "SRSectionBox.h"

#include "StructureModel/MSSteelUtility.h"
#include "StructureModel/MSCompoM.h"
#include "StructureModel/MSSrcBeamM.h"
#include "StructureModel/MDSrcBeamM.h"
#include "StructureModel/MSRcBeamSec.h"
#include "StructureModel/MRRcSection.h"
#include "StructureModel/MREdge.h"
#include "StructureModel/MRVBar.h"
#include "StructureModel/MRHBarByPoint.h"
#include "StructureModel/MRHBarPointByLoc.h"
#include "StructureModel/MSSteelSection.h"

MTSrcBeamMHelper::MTSrcBeamMHelper(MSBaseBuilding* pBldg, MTReader* pReader) : MTRcBeamMHelper(pBldg, pReader)
{
}


MTSrcBeamMHelper::~MTSrcBeamMHelper(void)
{
}

MSCompoM* MTSrcBeamMHelper::MakeData(CString Name, SRSectionTable* pTable)
{
	mp_ResultElement->SetArea(pTable->mp_Rect);

	CheckName(Name, mp_Reader->FindTextFigureID(pTable->mp_Name));

	mp_CompoM = mp_Bldg->CreateNewCompoM(MSCompoM::msSrcBeamM);
	mp_CompoM->mp_RebarM = MSCompoM::CreateRebarM(MSCompoM::msSrcBeamM);
	mp_Table = pTable;

	MSMaterial* pDefMaterial = mp_Bldg->GetMaterial(mp_Reader->m_nMaterialID);
	if (pDefMaterial == NULL)	pDefMaterial = mp_Bldg->GetDefMaterial(MSMaterial::msSRC);

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
	((MSRcLineMemberM*)mp_CompoM)->SetShape(MSRcLineMemberM::RECT);

// 	MSSteelUtility* pUtil = mp_Bldg->GetSteelUtili();
// 	long nSectionType = pUtil->GetSectionType(pTable->m_SteelSectionName);
// 	MSSteelSection* pSteelSection = pUtil->GetSection(nSectionType, pTable->m_SteelSectionName);
// 	CString steelSectionName = pTable->m_SteelSectionName;
// 	if(pSteelSection == NULL)
// 	{
// 		vector<double> params = Parsing_SteelName(pTable->m_SteelSectionName);
// 		pSteelSection = pUtil->BuiltUpSteelSection(nSectionType, params);
// 		if (pSteelSection != NULL)
// 			steelSectionName = pSteelSection->GetName();
// 
// 		mp_Reader->m_IsMakeBuiltUp = true;
// 	}
// 	else
// 		steelSectionName = pSteelSection->GetName();
	AddResultItem(MTResultItem::SteelSection, pTable->m_SteelSectionName);

	MSSrcBeamM* pSrcM = (MSSrcBeamM*)mp_CompoM;
// 	pSrcM->m_nSecType = nSectionType;
// 	pSrcM->m_pSection = pSteelSection;
	Parsing_StudBolt(pTable->m_StudBolt, pSrcM->m_NumStudBoltBars, &pSrcM->m_StudBoltBars);

	MakeSections();

	MDSrcBeamM* pRebarM = (MDSrcBeamM*)mp_CompoM->GetRebarM();
//	pRebarM->SetIsSubBeam(mp_Reader->IsSubBeam(Name));

	return mp_CompoM;
}

MSRcBeamSec* MTSrcBeamMHelper::GetLargeSection()
{
	MDSrcBeamM* pRebarM = (MDSrcBeamM*)mp_CompoM->GetRebarM();
	return pRebarM->mp_LargeSec;
}
MSRcBeamSec* MTSrcBeamMHelper::GetMidSection()
{
	MDSrcBeamM* pRebarM = (MDSrcBeamM*)mp_CompoM->GetRebarM();
	return pRebarM->mp_MidSec;
}
MSRcBeamSec* MTSrcBeamMHelper::GetSmallerSection()
{
	MDSrcBeamM* pRebarM = (MDSrcBeamM*)mp_CompoM->GetRebarM();
	return pRebarM->mp_SmallSec;
}
